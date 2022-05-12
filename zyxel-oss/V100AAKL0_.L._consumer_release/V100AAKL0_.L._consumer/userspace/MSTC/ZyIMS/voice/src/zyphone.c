#include <stdio.h>
#include <sys/socket.h>
#include "switch.h"
#include "global.h"
#include "icf_types.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "Phcmd.h"
#include "voiceApp_main.h"
#include "dsp_itf.h"
#include "ivrcmd.h"
#include "Voipccm_sip_itf.h"
#if CCBS_SUPPORT /*CCBS*/
#include "webDial_fsm.h"
#include "tr104.h"	
#include "tr104_init.h"
#endif

/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/
int ZyXELPhoneFsmIdle(evtMsg_t *msg, phoneCh_t *phoneCB);
int ZyXELPhoneFsmDial(evtMsg_t *msg, phoneCh_t *phoneCB);
int ZyXELPhoneFsmData(evtMsg_t *msg, phoneCh_t *phoneCB);
static int dialDigitsIvrTimeout (tm_id_t timer_id, void *arg);

uint8 ivrRecordFlag = 0;			/*remember is anybody recording now*/
extern uint32 sipAccountNumber;	

/*______________________________________________________________________________
**	ZyXELPhoneFsmIdle
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ZyXELPhoneFsmIdle(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("ZyXELPhoneFsmIdle - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);
	PH_FSM_DBG1("ZyXELPhoneFsmIdle -  phoneFlags = ",  phoneCB->phoneFlags);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("ZyXELPhoneFsmIdle - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( phoneCB->linkSt == PHONE_LINK_ST_IDLE ){
		switch( phoneEvent ){
			case EVT_PHONE_REMOVE:
				reset_PhoneCB(phoneCB);
				delete_PhoneCB(phoneCB, ccmConn);
				break;
			case EVT_CCM_DIAL_IND:
				
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					if (vDspChanOpen(phoneCB) != 0)		
					{
							PH_FSM_DBG("There is no dsp channel to receive this call.\n");
							eventVoiceCCMSimpleSend(EVT_CCM_REJ_IND, ORIG_FUNC_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
							break;
					}
				}
				
				if ( ( phoneCB->phoneFlags & PHONE_IVR_EMBEDDED ) && ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ) ) {
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
								
					eventVoiceCCMSimpleSend(EVT_CCM_CONN_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
					eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					phoneCB->phStatus = PHONE_STATUS_DATA;
				}else if( ( phoneCB->phoneFlags & PHONE_PHONE_CONFIG ) && ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ) ) {
					PH_FSM_DBG("\r\n send EVT_PHONE_PHONE_CONFIG_FUNC ");
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					eventVoiceCCMSimpleSend(EVT_CCM_CONN_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);			
					eventVoiceSimpleSend(EVT_PHONE_PHONE_CONFIG_FUNC, ORIG_FUNC_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					phoneCB->phStatus = PHONE_STATUS_DATA;
			
				}				
				#ifdef DO_NOT_DISTURB
				else{
					if( ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ) ) {
						 eventVoiceCCMSimpleSend(EVT_CCM_ALERT_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT);
						 phoneCB->linkSt = PHONE_LINK_ST_INCOMING;
						 phoneCB->phStatus = PHONE_STATUS_RING;
					} else {
						 PH_FSM_DBG("ccmConn is not bundled");
					}
				}
				#endif /* DO_NOT_DISTURB */		
				break;
			#if CCBS_SUPPORT
			case EVT_PHONE_DIAL_REQ:
				if(ccmConn->WebDialCB != NULL){
					eventVoiceCCMDataSend(EVT_CCM_ASSIGN_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));
				}							
				break;
			#endif
			default:
				PH_FSM_DBG1("ZyXELPhoneFsmIdle (IDLE) - unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
} /* ZyXELPhoneFsmIdle */

/*______________________________________________________________________________
**	ZyXELPhoneFsmDial
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ZyXELPhoneFsmDial(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	#if CCBS_SUPPORT
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	#endif

	PH_FSM_DBG2("ZyXELPhoneFsmDial - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("ZyXELPhoneFsmDial - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( ( phoneCB->linkSt == PHONE_LINK_ST_CALLING ) || ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) ){
		switch( phoneEvent ){
			case EVT_PHONE_ZYXEL_ONHOOK:
				if ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) {		/* FXO_LIFELINE */
					eventVoiceCCMSimpleSend(EVT_CCM_REL_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				break;
			case EVT_CCM_DISC_REQ:
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_CCM_DISC_IND:
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceCCMSimpleSend(EVT_CCM_REL_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
#if CCBS_SUPPORT
			case EVT_CCM_ALERT_IND:
				PH_FSM_DBG("ZyXELPhoneFsmDial - EVT_CCM_ALERT_IND");				   
				phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				if(ccmConn->WebDialCB != NULL){	  //Creat ZyPhone 2
					if(_getWebDialStatus(ccmConn)==0){
						PH_FSM_DBG("ZyXELPhoneFsmDial - Start to create ZyPhone 2");
						ccmConn->peerCcmConn->flags |= CALL_CONN_FLAG_WEBDIAL;
						eventVoiceCCMSimpleSend(EVT_CCM_ALERT_IND, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);
					}
				}					   
										   
				break;
#if 1	/* aircheng add, [SPRID # 100504121] */
			case EVT_CCM_SDP_ALERT_IND:
				PH_FSM_DBG("ZyXELPhoneFsmDial - EVT_CCM_SDP_ALERT_IND");				   
				phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				phoneCB->phoneFlags |= PHONE_CALLEE_EARLY_MEDIA;
				if(ccmConn->WebDialCB != NULL){	  //Creat ZyPhone 2
					if(_getWebDialStatus(ccmConn)==0){
						PH_FSM_DBG("ZyXELPhoneFsmDial - Start to create ZyPhone 2");
						ccmConn->peerCcmConn->flags |= CALL_CONN_FLAG_WEBDIAL;
						eventVoiceCCMSimpleSend(EVT_CCM_SDP_ALERT_IND, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);
					}
				}					   
										   
				break;
#endif
			case EVT_CCM_ALERT_REQ://AA caller
			    PH_FSM_DBG("ZyXELPhoneFsmDial - EVT_CCM_ALERT_REQ");
				phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				if(ccmConn->WebDialCB != NULL){
					ccmConn->peerCcmConn->flags |= CALL_CONN_FLAG_WEBDIAL;
					eventVoiceCCMSimpleSend(EVT_CCM_ALERT_REQ, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);
				}
				break;
			case EVT_CCM_ASSIGN_CONF:
			     /* 1. Setup the voipDspCallBack Function for the outgoing call */
			    /* 2. If the peerPort is FXO_LifeLine, change to CONNECT status directly */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){				     
				     phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;				     
				}
				break;
			case EVT_CCM_ASSIGN_FAIL://AA caller
                PH_FSM_DBG("\r\nZyXELPhoneFsmDial - EVT_CCM_ASSIGN_FAIL\r\n");			
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;							
			#if CCBS_SUPPORT
				if(ccmConn->WebDialCB != NULL && (ccmConn->WebDialCB->PhonePhyID < NumOfPhyIntf)){
					if(webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialFlags & WEB_REPEAT_DIAL){
						eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL_STOP, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
					}
					eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_FAIL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
				}				
			#endif				
			    break;
			case EVT_CCM_REJ_IND:
			case EVT_CCM_REL_IND:
				PH_FSM_DBG("\r\nZyXELPhoneFsmDial -EVT_CCM_REJ_IND");
				#if CCBS_SUPPORT
				if(ccmConn->WebDialCB != NULL){
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
                    phoneCB->phStatus = PHONE_STATUS_IDLE;
								
					if(phoneEvent==EVT_CCM_REJ_IND){   /*Theresa_0212*/						
							
						if((ccmConn->WebDialCB->PhonePhyID < NumOfPhyIntf) && (webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialFlags & WEB_REPEAT_DIAL)) {
							if(webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes<webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialTimes)
							{	
								eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
							}
							else{
								eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL_STOP, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
							}
						}							
						eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_FAIL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
					
					}
					else{
						eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_FAIL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
					}
				}
				#endif
			    break;
			case EVT_CCM_REJ_REQ:
				PH_FSM_DBG("\r\nZyXELPhoneFsmDial()  EVT_CCM_REJ_REQ\r\n");
				#if CCBS_SUPPORT
				if(ccmConn->WebDialCB != NULL){    //WebDial intenal call-peer phone is busy
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
                    phoneCB->phStatus = PHONE_STATUS_IDLE;
					if((ccmConn->WebDialCB->PhonePhyID < NumOfPhyIntf) && (webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialFlags & WEB_REPEAT_DIAL)) {
						if(webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes<webRepeatDial[ccmConn->WebDialCB->PhonePhyID].webRepeatDialTimes)
						{	
							eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
						}
						else{
							eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL_STOP, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
						}
					}		
					_copyWebDialLog("\r\nInternal peer phone is busy.");
					eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_FAIL, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);	
				}
				#endif
				break;
#endif
			default:
				PH_FSM_DBG1("ZyXELPhoneFsmDial - unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
} /* ZyXELPhoneFsmDial */

/*______________________________________________________________________________
**	ZyXELPhoneFsmData
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ZyXELPhoneFsmData(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("ZyXELPhoneFsmData - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("ZyXELPhoneFsmData - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}
	
	if ( phoneCB->phoneFlags & PHONE_IVR_EMBEDDED ) {
			ZyXELPhoneFsmIvr(msg, phoneCB);
	}
//for phone Config 
	if ( phoneCB->phoneFlags & PHONE_PHONE_CONFIG ) {
		ZyXELPhoneFsmPhoneConfig(msg, phoneCB);
	}

	if( ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) || ( phoneCB->linkSt == PHONE_LINK_ST_ONHOLD ) ) {
		switch( phoneEvent ){
			case EVT_PHONE_ZYXEL_ONHOOK:
				if ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) {		/* FXO_LIFELINE */
					eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				break;
			case EVT_CCM_DISC_REQ:
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_CCM_DISC_IND:
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceCCMSimpleSend(EVT_CCM_REL_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, ZYXEL_PHONE_PORT);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			default:
				PH_FSM_DBG1("ZyXELPhoneFsmData (CONNECT) - unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
} /* ZyXELPhoneFsmData */


/*______________________________________________________________________________
**	ZyXELPhoneFsmIvr -- IVR Dial-Pad User's Guide
**
**	descriptions:
**				* * * *				: Enter Ivr main menu
**
**				1 1 0 1 #~ 1 1 1 0 #	: Record user ivr data (item "00") ~ (item "09").   Press "#" to stop and save the record
**																			   Press "*" to stop and ignore the record
**																			   Press Other keys don't care.
**				1 2 0 1 # ~1 2 1 0 # 	: Play user ivr data.  (item "00") ~ (item "09"). 
**				1 3 0 1 # ~1 3 1 0 # 	: Delete user ivr data.  (item "00") ~ (item "09"). 
**				1 4 # 				: Clear all user ivr data.
**										
**										Notice once use UART upload firmware at bootext then this command needs execute
**										firstly to avoid exception.
**
**				1 1 5 1 # ~ 1 1 7 0 #	: Record system ivr data (item "00") ~ (item "19"). Press "#" to stop and save the record
**																			    Press "*" to stop and ignore the record
**																			    Press Other keys don't care.
**				1 2 5 1 # ~ 1 2 7 0 # 	: Play system ivr data.  (item "00") ~ (item "19").
**				1 3 5 1 # ~ 1 3 7 0 # 	: Delete system ivr data.  (item "00") ~ (item "19").
**				1 9 # 				: Clear all system ivr data.
**										
**										Notice Clear all commands need execute before record system ivr data.
**				
**				5 #					: IP presentation.
**
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ZyXELPhoneFsmIvr(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
){
		int status = 0;
		uint16 phoneEvent = msg->event;		
		voiceIvrCh_t *tmpVoiceIvrCh_p = NULL, *newVoiceIvrCh_p = NULL;		
		dialDigitBuf_t *dialDigitBuf;
		uint8 dialFinished = 0;
		char prifunc[2]={0}; 
		char	secfunc[2]={0};
		uint16 prifuncInt, secfuncInt, itemN;

		phoneCh_t *tmpPeerPhoneCB = NULL; 
		ccmConn_t *ccmConn=NULL;
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);

		PH_FSM_DBG2("ZyXELPhoneFsmIvr - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);
		tmpVoiceIvrCh_p = findVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);

		if(ccmConn->peerCcmConn!=NULL){
			if(ccmConn->peerCcmConn->origPortType != SIP_PORT){
				tmpPeerPhoneCB = find_phoneCB(phoneCB->peerMediaChId, 0);
			}
		}
		if (tmpPeerPhoneCB == NULL) {
			PH_FSM_DBG("Can't find peer phoneCB from peer phone logical id");
			return -1;
		}
		
		if ( ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) && ( phoneCB->phoneFlags & PHONE_IVR_EMBEDDED )  ) {
			switch ( phoneEvent ) {
				case EVT_CCM_DISC_REQ:
				case EVT_CCM_DISC_IND:				
					
					ivrRecordFlag=0;
					if(tmpVoiceIvrCh_p!=NULL){//Auto Attendant		
						if ( tmpVoiceIvrCh_p->func == IVR_FUNC_RECORD){							
							ivrCmdHaltRecord(tmpVoiceIvrCh_p);		/*free the record data and not save.*/							
						}
						
						tmpVoiceIvrCh_p->func=IVR_FUNC_NONE;		

						deleteVoiceIvrCh(tmpVoiceIvrCh_p);

					}					
					
					break;
					
				case EVT_PHONE_IVR_MAIN:
					
					if(tmpVoiceIvrCh_p!=NULL){

						deleteVoiceIvrCh(tmpVoiceIvrCh_p);

					}

					newVoiceIvrCh_p = createVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);
							
					ivrCmdPlayStart(newVoiceIvrCh_p, IVRPRON_MAIN_PROMPT, TYPE_IVR_SYS_DEFAULT, 3000);	/*repeat interval=3000ms*/					
					
					break;

				case EVT_CCM_DIAL_DIGIT:						
					if ( tmpVoiceIvrCh_p != NULL ) {					
						if ( tmpVoiceIvrCh_p->func == IVR_FUNC_RECORD) {		/*Under the record functionality*/
							dialDigitBuf = (dialDigitBuf_t *)msg->data_p;
							if( ((char)dialDigitBuf->digitStr)== '#') {				/* '#' means save and exit*/
								PH_FSM_DBG("Press #, stop record and save.");
				
								phStartBeepTone(tmpPeerPhoneCB);		
											
								phStopTone(tmpPeerPhoneCB);
								ivrCmdStopRecord(tmpVoiceIvrCh_p);				/*save the record data*/
								ivrRecordFlag=0;
								
								tmpVoiceIvrCh_p->func=IVR_FUNC_NONE;

								deleteVoiceIvrCh(tmpVoiceIvrCh_p);

								eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
							} else if (  ((char)dialDigitBuf->digitStr)== '*') {/* '*' means not save and exit*/
								PH_FSM_DBG("Press *, stop record and not save.");
				
								phStartBeepTone(tmpPeerPhoneCB);				
											
								phStopTone(tmpPeerPhoneCB);									
								ivrCmdHaltRecord(tmpVoiceIvrCh_p);					/*free the record data and not save.*/
								ivrRecordFlag=0;
								
								tmpVoiceIvrCh_p->func=IVR_FUNC_NONE;

								deleteVoiceIvrCh(tmpVoiceIvrCh_p);

								eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
							}
							memset(&(phoneCB->dialInform), 0, sizeof(dialInfor_t));
							break;
						} else {
						#if 0
							if ( tmpVoiceIvrCh_p != NULL ) {
								tmpVoiceIvrCh_p->voipIvrCallBack = NULL;
							}
						#endif
							dialDigitBuf = (dialDigitBuf_t *)msg->data_p;
							timerStop(phoneCB->callProTimer.dialDigitTimer);
							phoneCB->callProTimer.dialDigitTimer = 0;
				
							phStopTone(tmpPeerPhoneCB);
							
							if(zyStrlen(phoneCB->dialInform.dialDigitNumber.dial_digits)==0){ /*first digit, stop play menu promt*/
								vDspIVRFunc(phoneCB, 1, 2, IVR_FUNC_ACTION_STOP, 0 /*dont care*/); /* stop play*/
							}
							else{
								PH_FSM_PRINTF("Dial Digit = %s\n", phoneCB->dialInform.dialDigitNumber.dial_digits);
							}
				
							dialFinished = dialDigitsIvrAdj(phoneCB, dialDigitBuf);
							PH_FSM_DBG1("Receive key Press, dialFinished=", dialFinished);
							switch(dialFinished) {
								case 1:
									eventVoiceSimpleSend(EVT_PHONE_IVR_FUNC, ORIG_FUNC_PHONE, phoneCB->logicalId);
									break;
								case 0:
								default:								
									phoneCB->callProTimer.dialDigitTimer = timerStartS(1000*10, dialDigitsIvrTimeout, phoneCB);								
									break;
							}
						}
					}
							
				
					break;
					
				case EVT_PHONE_IVR_FUNC:
					zyMemcpy(prifunc, &phoneCB->dialInform.dialDigitNumber.dial_digits[0],1);
					prifuncInt=atoi(prifunc);
					zyMemcpy(secfunc, &phoneCB->dialInform.dialDigitNumber.dial_digits[1],1);
					secfuncInt=atoi(secfunc);
					itemN = (int)atoi(&phoneCB->dialInform.dialDigitNumber.dial_digits[2]);
					PH_FSM_PRINTF("Dial Digit = %s\n", phoneCB->dialInform.dialDigitNumber.dial_digits);
					PH_FSM_DBG1("fxsFsmIvr - #prifunc = ", prifuncInt);
					PH_FSM_DBG1("fxsFsmIvr - #secfunc = ", secfuncInt);
					PH_FSM_DBG1("fxsFsmIvr - #itemN = ", itemN);
					switch (prifuncInt) {
						case 1:			/* voice maintain */
							switch (secfuncInt) {
								case 1:											/*record*/
									PH_FSM_DBG("fxsFsmIvr - Record ivr.");

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);
	
									if (ivrRecordFlag==1) {							/*someone is already recording.*/
										IVR_FSM_DBG("fxsFsmIvr - Someone is recording now.");
										eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
										break;
									}
									phStartBeepTone(tmpPeerPhoneCB);							
									newVoiceIvrCh_p = createVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);
									if (ivrCmdStartRecord(newVoiceIvrCh_p, itemN-1)!=TRUE) {
										PH_FSM_DBG("fxsFsmIvr - Record ivr failed.");

										deleteVoiceIvrCh(newVoiceIvrCh_p);
						
										phStopTone(tmpPeerPhoneCB);							
										eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
										break;
									}
									
									
									phStopTone(tmpPeerPhoneCB);									
									
									ivrRecordFlag=1;
									
									newVoiceIvrCh_p->func = IVR_FUNC_RECORD;
									
									break;
								case 2:											/*play*/
									PH_FSM_DBG("fxsFsmIvr - Play ivr");

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);
				
									newVoiceIvrCh_p = createVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);
									status = ivrCmdPlayVoiceByKeyPress(newVoiceIvrCh_p, itemN-1);									
									if (status != TRUE) {
										PH_FSM_DBG("fxsFsmIvr - Play ivr error, item empty.");

										deleteVoiceIvrCh(newVoiceIvrCh_p);
					
										eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
									}
									break;
							
								case 3:											/*delete specific*/
									PH_FSM_DBG("fxsFsmIvr - delete specific ivr");

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);
														
									if (ivrCmdDelRecord(itemN-1)!=TRUE){
										PH_FSM_DBG("fxsFsmIvr - delete specific ivr failed");
									}
									
									eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
									break;
								case 4:											/*clear all usr ivr*/
									PH_FSM_DBG("fxsFsmIvr - clear all usr ivr ");

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);

									vDspIVRFunc(NULL, 1, 4, 0 /*dont care*/, 0 /*dont care*/); /* clear all usr ivr*/
									#if 0
									if (ivrUsrClearToDefault()!=TRUE)
										IVR_FSM_DBG("fxsFsmIvr - delete ivr, clear all usr ivr failed ");
									#endif
									
									eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
									break;
								
								case 9:											/*clear all sys ivr*/
									PH_FSM_DBG("fxsFsmIvr - clear all sys ivr ");

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);

									vDspIVRFunc(NULL, 1, 9, 0 /*dont care*/, 0 /*dont care*/); /* clear all sys ivr*/
									#if 0
									if (ivrSysClearToDefault()!=TRUE)
										IVR_FSM_DBG("fxsFsmIvr - delete ivr, clear all sys ivr failed ");
									#endif
									
									eventVoiceSimpleSend(EVT_PHONE_IVR_MAIN, ORIG_FUNC_PHONE, phoneCB->logicalId);
									break;
								default:											/*unknown*/
									PH_FSM_DBG1("fxsFsmIvr - unknown ivr maintain sub command, secfunc = ",secfuncInt);

									deleteVoiceIvrCh(tmpVoiceIvrCh_p);
				
									newVoiceIvrCh_p = createVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);
											
									ivrCmdPlayStart(newVoiceIvrCh_p, IVRPRON_INVALID, TYPE_IVR_SYS_DEFAULT, 0);									

									break;
								}
							break;
						case 5:	/*IP presentation*/
						default:	// prifunc
							PH_FSM_DBG1("fxsFsmIvr - unknown ivr main command, prifunc = ",prifuncInt);

							deleteVoiceIvrCh(tmpVoiceIvrCh_p);

							newVoiceIvrCh_p = createVoiceIvrCh(phoneCB->logicalId, phoneCB->dspPortType);
									
							ivrCmdPlayStart(newVoiceIvrCh_p, IVRPRON_INVALID, TYPE_IVR_SYS_DEFAULT, 0);							

							break;
						}
						memset(&(phoneCB->dialInform), 0, sizeof(dialInfor_t));
					break; //EVT_PHONE_IVR_FUNC
					
				default:
					break;
			}
		}
		
fail:
	return status;
}



/*______________________________________________________________________________
**	ZyXELPhoneFsmPhoneConfig
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int
ZyXELPhoneFsmPhoneConfig(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
){

		int status = 0;
		uint16 phoneEvent = msg->event;			
		dialDigitBuf_t *dialDigitBuf;
		uint8 dialFinished = 0;
		char prifunc[2]={0}; 
		char	secfunc[2]={0};
		int callFeature=0;
		uint16 prifuncInt, secfuncInt, itemN;
		VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
		int lineId = -1;
	
		int ret=0;


		int SipAccountId = 0;
		int LineItr = 0, VpItr=0;
		uint32 NumOfLine = 0;

		phoneCh_t *tmpPeerPhoneCB = NULL; 
		ccmConn_t *ccmConn=NULL;
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);

		PH_FSM_DBG2("ZyXELPhoneFsmPhoneConfig - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);
		
		if(ccmConn->peerCcmConn!=NULL){
			if(ccmConn->peerCcmConn->origPortType != SIP_PORT){
				tmpPeerPhoneCB = find_phoneCB(phoneCB->peerMediaChId, 0);	
			}
			callFeature=ccmConn->peerCcmConn->callFeatures;
		}
		if (tmpPeerPhoneCB == NULL) {
			PH_FSM_DBG("Can't find peer phoneCB from peer phone logical id");
			return -1;
		}
		
	
		if ( ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) && ( phoneCB->phoneFlags & PHONE_PHONE_CONFIG )  ) {
			switch ( phoneEvent ) {
				case EVT_CCM_DISC_REQ:
				case EVT_CCM_DISC_IND:		
						break;
				case EVT_PHONE_PHONE_CONFIG_FUNC:
					PH_FSM_DBG1("callFeatures is ",callFeature);
					switch (callFeature) {
						
						case CALL_FEATURE_CALL_WAITING_ENABLE:
							/* read tr-104 callingfeature object */
							/* get the SIP Profile ID and Line ID  */		
					
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);

							PH_FSM_PRINTF("receive EVT_PHONE_PHONE_CONFIG_FUNC, SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->callWaitingEnable is  ",pLineFeature->callWaitingEnable);
								/* enable callwaiting */
								pLineFeature->callWaitingEnable=1;
								/*save the pLineFeature */
								/* save function TBD */
								PH_FSM_DBG1("new pLineFeature->callWaitingEnable is  ",pLineFeature->callWaitingEnable);

								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for SipAccountId %d\n", SipAccountId);
								PH_FSM_DBG2("cannot get lineCallingFeatureObj for VpItr %d\n", VpItr , "LineItr %d", LineItr);
							}

							break;
					
	
						case CALL_FEATURE_CALL_WAITING_DISABLE:
							/* read tr-104 callingfeature object */
							/* get the SIP Profile ID and Line ID  */
												
						
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_CALL_WAITING_DISABLE , SipAccountId = %d\n", SipAccountId);
							
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->callWaitingEnable is  ",pLineFeature->callWaitingEnable);
								/* disable callwaiting */
								pLineFeature->callWaitingEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->callWaitingEnable is  ",pLineFeature->callWaitingEnable);							
								
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for SipAccountId %d\n", SipAccountId);
								PH_FSM_DBG2("cannot get lineCallingFeatureObj for VpItr %d\n", VpItr , "LineItr %d", LineItr);
							}
										
							break;
						case CALL_FEATURE_ENABLE_DND:
							{
								/* read tr-104 callingfeature object */
								/* 1. get the SIP Profile ID and Line ID  */

								for(SipAccountId=0;SipAccountId<sipAccountNumber;SipAccountId++){
									if ( ccmConn->peerCcmConn->SipFxsMap & (1<<SipAccountId) ) {
										/* this api should modify for CCC */										
										VpItr = mapSipAccountIdToVpItr(SipAccountId);
										LineItr = mapSipAccountIdToLineItr(SipAccountId);	
										PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
										
										pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
										if(pLineFeature != NULL){
											PH_FSM_PRINTF("LineItr %d=>old pLineFeature->DoNotDistrubEnable is %d\n ", LineItr, pLineFeature->doNotDisturbEnable);
											if(!pLineFeature->doNotDisturbEnable){
												/* disable DoNotDistrubEnable */
												pLineFeature->doNotDisturbEnable=1;
												/*save the pLineFeature */
												/* save function TBD */			
												PH_FSM_PRINTF("LineItr %d=>new pLineFeature->DoNotDistrubEnable is %d\n", LineItr, pLineFeature->doNotDisturbEnable);
												ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr, LineItr, pLineFeature);
												if ( ret != 0 ) {
														PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
												}
											}
											/* if success , play some tone to tmpPeerPhoneCB */	
											phStartConfirmTone(tmpPeerPhoneCB);
										}
										else{
											PH_FSM_PRINTF("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
										}
									}
								}
							
							
							}
							break;
						case CALL_FEATURE_DISABLE_DND:
							{						
								/* read tr-104 callingfeature object */
								/* 1. get the SIP Profile ID and Line ID  */
								for(SipAccountId=0;SipAccountId<sipAccountNumber;SipAccountId++){
									if ( ccmConn->peerCcmConn->SipFxsMap & (1<<SipAccountId) ) {
										/* this api should modify for CCC */										
										VpItr = mapSipAccountIdToVpItr(SipAccountId);
										LineItr = mapSipAccountIdToLineItr(SipAccountId);	
										PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);

										pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
										if(pLineFeature != NULL){
											PH_FSM_PRINTF("LineItr %d=>old pLineFeature->DoNotDistrubEnable is %d\n ", LineItr, pLineFeature->doNotDisturbEnable);
											if(pLineFeature->doNotDisturbEnable){
												/* disable DoNotDistrubEnable */
												pLineFeature->doNotDisturbEnable=0;
												/*save the pLineFeature */
												/* save function TBD */			
												PH_FSM_PRINTF("LineItr %d=>new pLineFeature->DoNotDistrubEnable is %d\n", LineItr, pLineFeature->doNotDisturbEnable);
												ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr, LineItr, pLineFeature);
												if ( ret != 0 ) {
														PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
												}
											}
											/* if success , play some tone to tmpPeerPhoneCB */	
											phStartConfirmTone(tmpPeerPhoneCB);
										}
										else{
											PH_FSM_PRINTF("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
										}
									}
								}
							
							
							}
							break;
						case CALL_FEATURE_CID_DISPLAY:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */												
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_CID_DISPLAY , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->CallerIDEnable is  ",pLineFeature->anonymousCallEnable);
								/* enable CallerIDEnable */
								pLineFeature->anonymousCallEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->CallerIDEnable is  ",pLineFeature->anonymousCallEnable);
						
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */	
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							
							break;
						case CALL_FEATURE_CID_HIDDEN:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_CID_HIDDEN , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->CallerIDEnable is  ",pLineFeature->anonymousCallEnable);
								/* disable CallerIDEnable */
								pLineFeature->anonymousCallEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->CallerIDEnable is  ",pLineFeature->anonymousCallEnable);
							
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);	
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */	
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;

						case CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								if(zyStrlen(pLineFeature->callForwardUnconditionalNumber) != 0){
									PH_FSM_DBG1("old pLineFeature->callForwardUnconditionalEnable is  ",pLineFeature->callForwardUnconditionalEnable);
									/* enable callForwardUnconditional */
									pLineFeature->callForwardUnconditionalEnable=1;
									/*save the pLineFeature */
									/* save function TBD */			
									PH_FSM_DBG1("new pLineFeature->callForwardUnconditionalEnable is  ",pLineFeature->callForwardUnconditionalEnable);
								
									ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
									if ( ret != 0 ) {
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
										phStartNegativeAckTone(tmpPeerPhoneCB);
									}
									/* if success , play some tone to tmpPeerPhoneCB */	
									else{
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
										phStartConfirmTone(tmpPeerPhoneCB);
									}
								}
								else{
									PH_FSM_PRINTF("pLineFeature->callForwardUnconditionalNumber is not set \n");
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;

						case CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->callForwardUnconditionalEnable is  ",pLineFeature->callForwardUnconditionalEnable);
								/* disable callForwardUnconditional */
								pLineFeature->callForwardUnconditionalEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->callForwardUnconditionalEnable is  ",pLineFeature->callForwardUnconditionalEnable);
							
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */	
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;					

						case CALL_FEATURE_NOANSWER_FORWARD_ENABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_NOANSWER_FORWARD_ENABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								if(zyStrlen(pLineFeature->callForwardOnNoAnswerNumber) != 0){
									PH_FSM_DBG1("old pLineFeature->callForwardOnNoAnswerEnable is  ",pLineFeature->callForwardOnNoAnswerEnable);
									/* enable callForwardOnNoAnswer */
									pLineFeature->callForwardOnNoAnswerEnable=1;
									/*save the pLineFeature */
									/* save function TBD */			
									PH_FSM_DBG1("new pLineFeature->callForwardOnNoAnswerEnable is  ",pLineFeature->callForwardOnNoAnswerEnable);
								
									ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
									if ( ret != 0 ) {
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
										phStartNegativeAckTone(tmpPeerPhoneCB);
									}
									/* if success , play some tone to tmpPeerPhoneCB */	
									else{
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
										phStartConfirmTone(tmpPeerPhoneCB);
									}
								}
								else{
									PH_FSM_PRINTF("pLineFeature->callForwardOnNoAnswerNumber is not set \n");
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;	
						case CALL_FEATURE_NOANSWER_FORWARD_DISABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
							
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_NOANSWER_FORWARD_DISABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->callForwardOnNoAnswerEnable is  ",pLineFeature->callForwardOnNoAnswerEnable);
								/* disable callForwardOnNoAnswer */
								pLineFeature->callForwardOnNoAnswerEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->callForwardOnNoAnswerEnable is  ",pLineFeature->callForwardOnNoAnswerEnable);
							
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */	
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							
							break;	


						case CALL_FEATURE_BUSY_FORWARD_ENABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
						
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_BUSY_FORWARD_ENABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								if(zyStrlen(pLineFeature->callForwardOnBusyNumber) != 0){
									PH_FSM_DBG1("old pLineFeature->callForwardOnBusyEnable is  ",pLineFeature->callForwardOnBusyEnable);		
									/* enable callForwardOnBusy */
									pLineFeature->callForwardOnBusyEnable=1;
									/*save the pLineFeature */
									/* save function TBD */			
									PH_FSM_DBG1("new pLineFeature->callForwardOnBusyEnable is  ",pLineFeature->callForwardOnBusyEnable);
								
									ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr,LineItr, pLineFeature);
									if ( ret != 0 ) {
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
										phStartNegativeAckTone(tmpPeerPhoneCB);
									}
									/* if success , play some tone to tmpPeerPhoneCB */	
									else{
										PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
										phStartConfirmTone(tmpPeerPhoneCB);
									}
								}
								else{
									PH_FSM_PRINTF("pLineFeature->callForwardOnBusyNumber is not set \n");
									phStartNegativeAckTone(tmpPeerPhoneCB);

								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;	


						case CALL_FEATURE_BUSY_FORWARD_DISABLE:
							/* read tr-104 callingfeature object */
							/* 1. get the SIP Profile ID and Line ID  */
						
							/* this api should modify for CCC */
							SipAccountId = getAssociateRegedLineId(tmpPeerPhoneCB);
							PH_FSM_PRINTF(" receive CALL_FEATURE_BUSY_FORWARD_DISABLE , SipAccountId = %d\n", SipAccountId);
							VpItr = mapSipAccountIdToVpItr(SipAccountId);
							LineItr = mapSipAccountIdToLineItr(SipAccountId);	
							PH_FSM_PRINTF("VpItr = %d, LineItr = %d\n", VpItr, LineItr);
							pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
							if(pLineFeature != NULL){
								PH_FSM_DBG1("old pLineFeature->callForwardOnBusyEnable is  ",pLineFeature->callForwardOnBusyEnable);
								/* disable callForwardOnBusy */
								pLineFeature->callForwardOnBusyEnable=0;
								/*save the pLineFeature */
								/* save function TBD */			
								PH_FSM_DBG1("new pLineFeature->callForwardOnBusyEnable is  ",pLineFeature->callForwardOnBusyEnable);
							
								ret=tr104VoiceLineCallingFeatureObjPtrSet(VpItr, LineItr,pLineFeature);
								if ( ret != 0 ) {
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash fail, status = ", ret);
									phStartNegativeAckTone(tmpPeerPhoneCB);
								}
								/* if success , play some tone to tmpPeerPhoneCB */	
								else{
									PH_FSM_DBG1("ZyPhoneVoiceLineCallingFeatureObjWrite save flash success, status = ", ret);
									phStartConfirmTone(tmpPeerPhoneCB);
								}
							}
							else{
								PH_FSM_DBG1("cannot get lineCallingFeatureObj for LineItr %d\n", LineItr);
							}
							break;	


						case CALL_FEATURE_ENABLE_WLAN:
						case CALL_FEATURE_DISABLE_WLAN:
							/* read wlan configure and change it */
							break;
						#if CCBS_SUPPORT
						case CALL_FEATURE_CCBS_DEACTIVATE:
							{
								phoneCh_t * phoneCB = NULL;
								if(ccmConn->peerCcmConn != NULL){
									phoneCB = find_phoneCB(ccmConn->peerCcmConn->origPortId, 0);
									if(NULL != phoneCB){
										PH_FSM_PRINTF("CALL_FEATURE_CCBS_DEACTIVATE for phone phyId %d\n", phoneCB->physicalId);
										WebPhoneDialStop(phoneCB->physicalId);
										/* if success , play some tone to tmpPeerPhoneCB */
										phStartConfirmTone(tmpPeerPhoneCB);
									}
								}
								eventVoiceSimpleSend(EVT_PHONE_PHONECONFIG_TRUE, ORIG_FUNC_PHONE, tmpPeerPhoneCB->logicalId);
							}
							break;
						#endif	
						default:
							ZyPrintf("\n no match call feature ");
							break;
					}					
					break;					
			
				default:
					break;
			}
		}

		
fail:
	return status;
}
/*______________________________________________________________________________
**	dialDigitsIvrTimeout
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
dialDigitsIvrTimeout (
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;

	tmpPhoneCB->callProTimer.dialDigitTimer = 0;
	eventVoiceSimpleSend(EVT_PHONE_IVR_FUNC, ORIG_FUNC_PHONE, tmpPhoneCB->logicalId);
	
	return 0;
} /* dialDigitsIvrTimeout */

/*______________________________________________________________________________
**  dialDigitsIvrAdj
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
dialDigitsIvrAdj(
	phoneCh_t *phoneCB,
	dialDigitBuf_t	*dialDigitBuf
){
	uint8 *currentLen;

	if ( dialDigitBuf != NULL ) {
		currentLen = &(phoneCB->dialInform.dialDigitNumber.current_length);

		if (  (*currentLen) != 0) {
			if(((char)dialDigitBuf->digitStr)=='#')
					return 1;	//call is finished
		}

		if ( *currentLen < MAX_DIAL_STRING_LENGTH-1 ) { 		/* ignore the digit that is more than the MAX length */
			phoneCB->dialInform.dialDigitNumber.dial_digits[*currentLen] = dialDigitBuf->digitStr;
			phoneCB->dialInform.dialDigitNumber.dial_durations[*currentLen] = dialDigitBuf->duration;
			phoneCB->dialInform.dialDigitNumber.dial_timeStampOffsets[*currentLen] = dialDigitBuf->timeStampOffset;
			(*currentLen)++;
			IVR_FSM_DBG2("the index of digit = ", (*currentLen), "DialDigitBuf->digitStr = ", dialDigitBuf->digitStr);
		}
	}

	return 0;
}/* dialDigitsIvrAdj */


