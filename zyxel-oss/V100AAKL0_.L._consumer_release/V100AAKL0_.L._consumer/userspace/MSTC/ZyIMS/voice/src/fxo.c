#include <stdio.h>
#include <sys/socket.h>
#include "switch.h"
#include "global.h"
#include "icf_types.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "callConf.h"
#include "phfsm.h"
#include "Phcmd.h"
#include "voiceApp_main.h"
#include "dsp_itf.h"
#include "tr104.h"
#include "dialplan.h"

#ifdef FXO_DSPRELAY
/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/
int fxoFsmIdle(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxoFsmDial(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxoFsmRing(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxoFsmData(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxoFsmDrop(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxoFsmDisc(evtMsg_t *msg, phoneCh_t *phoneCB);

/*_____ global variables _______________________________________________*/


/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/


/*______________________________________________________________________________
**	fxoFsmIdle
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
fxoFsmIdle(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	phoneCh_t *peerPhoneCB = NULL;	

	PH_FSM_DBG2("fxoFsmIdle - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);
	
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmIdle - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if (phoneCB->linkSt == PHONE_LINK_ST_IDLE){
		switch( phoneEvent ){
			case EVT_CCM_DIAL_REQ:
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					/* 1. PREFIX: Relay directly without dialing the digits */
					/* 2. Not Prefix: dial the digits and then relay. */
					/* 3. Both of them need to use fxoConnCheck() to check the PSTN line */
					if ( !( ccmConn->flags & CALL_CONN_FLAG_PREFIX ) ) {
						/* Not Prefix */
						PH_FSM_PRINTF("No Prefix\n");						
					}
					else{
						PH_FSM_PRINTF("FXO call with prefix, peerCcm portId 0x%x\n", ccmConn->peerCcmConn->origPortId);
						/* prefix */
						peerPhoneCB = find_phoneCB(ccmConn->peerCcmConn->origPortId, 0);
						if ( peerPhoneCB != NULL ){
														
							vDspChanOpen(phoneCB);
							phDisableDtmfDetect(phoneCB);
							phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
							fxofxsChannelConnect(phoneCB, 1);							
							//fxoOffhook
							fxoDial(phoneCB);
							phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
							phoneCB->phStatus =PHONE_STATUS_DATA;

							eventVoiceCCMSimpleSend(EVT_CCM_CONN_IND, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
						}
					}
					
				}
				break;
			case EVT_PHONE_FXO_DETECTCID:
				vDSPDetectCIDFSK(phoneCB);
				/*fall through*/
			case EVT_PHONE_FXO_RINGING:
				if ( !( ccmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ){
					ccmConn->flags |= CALL_CONN_FLAG_PREASSIGN;
					eventVoiceCCMSimpleSend(EVT_CCM_ASSIGN_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXO_PORT);
					phoneCB->linkSt = PHONE_LINK_ST_INCOMING;
					phoneCB->phStatus =PHONE_STATUS_RING;					
				}
				break;
			case EVT_PHONE_FXO_RECVCID:
				{
					callerIDInfo_t* info = (callerIDInfo_t*)msg->data_p;
					if (info->digitsFlag) {
						ccmConn->dialInform.dialDigitNumber.current_length = strlen(info->digits);
						memcpy(ccmConn->dialInform.dialDigitNumber.dial_digits, info->digits, strlen(info->digits));
					}
					
					if (info->nameFlag) {	
						memcpy(ccmConn->dialInform.dialDigitNumber.cname, info->name, strlen(info->name));

					}
					#ifdef FULL_CID_TYPE
						ccmConn->dialInform.dialDigitNumber.cid_type=info->cidType;
					#endif	
				}
				break;
			case EVT_PHONE_REMOVE:
				//#ifdef VOIP_ANALYSIS
				//analysis_AddNewCall(analysis_callRelease, &phoneCB->logicalId, NULL, NULL);
				//#endif
				vDspChanClose(phoneCB);
				delLogicPhone(phoneCB, ccmConn);					
				break;
			case EVT_PEERFXO_BUSYTONE_DETECT:
				PH_FSM_DBG("fxoFsmIdle   EVT_PEERFXO_BUSYTONE_DETECT\r\n");
				break;
			default:
				PH_FSM_DBG1("fxoFsmIdle-unknown event=", phoneEvent);
				break;
		}
	}
	
fail:
	return status;	

}

/*______________________________________________________________________________
**	fxoFsmDial
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
fxoFsmDial(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("fxoFsmDial - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmDial - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}
		
	switch( phoneEvent ){
		case EVT_PEERFXO_BUSYTONE_DETECT:
			DBG_pline_1("fxoFsmDial   EVT_PEERFXO_BUSYTONE_DETECT\r\n");
			break;
 		default:
			PH_FSM_DBG1("fxoFsmDial-unknown event=", phoneEvent);
			break;
	}

fail:
	return status;
}

/*______________________________________________________________________________
**	fxoFsmRing
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
fxoFsmRing(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	ccmConn_t *peerCcmConn = NULL;

	PH_FSM_DBG2("fxoFsmRing - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmRing - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}
	if ( phoneCB->linkSt == PHONE_LINK_ST_INCOMING ){
		switch( phoneEvent ){
			case EVT_CCM_ALERT_IND:
				/* 1. Play the RingBack Tone */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						//phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					}
				} else {
					PH_FSM_DBG("Not assign before alert");
				}
				break;
			case EVT_CCM_CONN_REQ:
				peerCcmConn = ccmConn->peerCcmConn;
				if ( ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ) && ( ccmConn->connRole == CCM_CONN_ROLE_ORIGINAL ) ) {										
					vDspChanOpen(phoneCB);					
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					fxofxsChannelConnect(phoneCB, 1);
					fxoOffHook(phoneCB);
					eventVoiceCCMSimpleSend(EVT_CCM_CONN_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					phoneCB->phStatus =PHONE_STATUS_DATA;
				}
				break;
			case EVT_PHONE_FXO_STOP_RING:
			case EVT_PHONE_FXO_PLUGOUT:
				reset_PhoneCB(phoneCB);
				eventVoiceCCMSimpleSend(EVT_CCM_REL_IND, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
				ccmConn->flags &= ~(CALL_CONN_FLAG_PREASSIGN);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
			case EVT_CCM_ASSIGN_FAIL:
				ccmConn->flags &= ~(CALL_CONN_FLAG_PREASSIGN|CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;			
				break;
			case EVT_CCM_DISC_REQ:
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phDisableDtmfDetect(phoneCB);
			/*Nedo: assign DSP channel dynamically*/			
					vDspChanOpen(phoneCB);
				}
				if ( phoneCB->callProTimer.fxoDialDigitTimer1 != 0) {
					timerStop(phoneCB->callProTimer.fxoDialDigitTimer1);
					phoneCB->callProTimer.fxoDialDigitTimer1 = 0;
				}
				if ( phoneCB->callProTimer.fxoDialDigitTimer2 != 0) {
					timerPeriodicStop(phoneCB->callProTimer.fxoDialDigitTimer2);
					phoneCB->callProTimer.fxoDialDigitTimer2 = 0;
				}
				fxoOnHook(phoneCB);
				reset_PhoneCB(phoneCB);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
			default:
				PH_FSM_DBG1("fxoFsmRing-unknown event=", phoneEvent);
				break;
		}
	}
fail:
	return status;	
}

/*______________________________________________________________________________
**	fxoFsmData
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
fxoFsmData(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	dialDigitBuf_t dialDigitBuf;
	uint8 *currentLen;
	ccmConn_t *peerCcmConn = NULL;
	phoneCh_t *peerPhoneCB = NULL;
	int i;	
	uint32 NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	ZyXEL_VoiceFXO_t* pVoiceFxo = tr104VoiceFXOPtrGet(phoneCB->physicalId - NumOfPhyIntf);
	uint16 onTime = pVoiceFxo->fxoDTMFDigitDuration;
		
	PH_FSM_DBG2("fxoFsmData - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmData - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT){
		switch( phoneEvent ){
			case EVT_CCM_DIAL_DIGIT:
				if ( (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) && ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) ) {
					if ( msg->data_p != NULL ){
						memcpy(&dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
						PH_FSM_DBG1("fxoFSMData-1 PH: DSP Digit = ", dialDigitBuf.digitStr);
						currentLen = &(phoneCB->dialInform.dialDigitNumber.current_length);
						/* Add the new digital to the dialInform.dialDigitNumber */
						if ( *currentLen < MAX_DIAL_STRING_LENGTH-1 ) { 		/* ignore the digits that are more than the MAX length */
							phoneCB->dialInform.dialDigitNumber.dial_digits[*currentLen] = dialDigitBuf.digitStr;
							phoneCB->dialInform.dialDigitNumber.dial_durations[*currentLen] = dialDigitBuf.duration;
							phoneCB->dialInform.dialDigitNumber.dial_timeStampOffsets[*currentLen] = dialDigitBuf.timeStampOffset;
							phoneCB->dialInform.dialDigitNumber.dialout_length = *currentLen;

							//dennis_DialPulse_generation
							//dbg_printf("\r\n fxoFSMData-1 PH: DSP Digit = %c", phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length]);
							//phoneCB->dialType=PULSE;							
							vDspStartDtmfTone(phoneCB, onTime);							
													
							(*currentLen)++;
						}
					}
				} 
				break;	
			case EVT_CCM_REL_IND:
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						fxoOnHook(phoneCB);						
						phDisableDtmfDetect(phoneCB);
						phoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE; 
						phoneCB->LinePolarity = LINE_POLARITY_REVERSE; 
						phPolarityReverse(phoneCB);	//hchsu 2005/03/23 for telefonica					
						vDspChanOpen(phoneCB);
					
					}					
					reset_PhoneCB(phoneCB);
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
					break;
			case EVT_CCM_DISC_REQ:
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phDisableDtmfDetect(phoneCB);
					vDspChanOpen(phoneCB);
					if ( phoneCB->callProTimer.fxoDialDigitTimer1 != 0) {
						timerStop(phoneCB->callProTimer.fxoDialDigitTimer1);
						phoneCB->callProTimer.fxoDialDigitTimer1 = 0;
					}
					if ( phoneCB->callProTimer.fxoDialDigitTimer2 != 0) {
						timerPeriodicStop(phoneCB->callProTimer.fxoDialDigitTimer2);
						phoneCB->callProTimer.fxoDialDigitTimer2 = 0;
					}
					fxoOnHook(phoneCB);
					reset_PhoneCB(phoneCB);
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
				}
				break;
			case EVT_PHONE_FLASH:
				if ( !( phoneCB->phoneFlags & PHONE_PHYSICAL_FXO_NONFLASH ) ){
					peerCcmConn = ccmConn->peerCcmConn;
					if ( peerCcmConn != NULL ) {
						currentLen = &(peerPhoneCB->dialInform.dialDigitNumber.current_length);
						fxoOnHook(phoneCB);						
						usleep(pVoiceFxo->fxoFlashMinInterval*1000);
						fxoOffHook(phoneCB);
						usleep(1000000);
					}
				}
				break;
			case EVT_CCM_ONHOLD_REQ:
				peerCcmConn = ccmConn->peerCcmConn;
				if ( peerCcmConn != NULL ) {
					peerPhoneCB = find_phoneCB(peerCcmConn->origPortId, 0);
					if ( peerPhoneCB != NULL ) {
						if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
							phDisableDtmfDetect(phoneCB);					
							vDspChanOpen(phoneCB);					
							fxofxsChannelConnect(phoneCB, 0);
						}
						phoneCB->linkSt = PHONE_LINK_ST_ONHOLD;
						//phoneCB->phoneFlags |= PHONE_PHYSICAL_FXO_NONFLASH;
						//phoneCB->callProTimer.fxoNonFlashCheckTimer = timerStartS(TIMEOUT_FXO_NO_FLASH, fxoNoFlashTimeout, phoneCB);
						//fxoOffHook(phoneCB);
						eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_CONF, MSG_SEND_TO_CCMCONN, ccmConn->origPortId, ccmConn->origPortType);
					}
				}
				break;
			case EVT_PHONE_DSP_DIGITS: /* Obsolete */
			case EVT_PHONE_FXS_DIGITS:
				peerCcmConn = ccmConn->peerCcmConn;	 
				PH_FSM_DBG("\r\nfxoFsmData  EVT_PHONE_DSP_DIGITS\r\n");
				memcpy(&dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
				PH_FSM_DBG1("\r\nPH: DSP Digit = ", dialDigitBuf.digitStr);  
				eventVoiceDataSend(EVT_CCM_DIAL_DIGIT, MSG_SEND_TO_CCMCONN, peerCcmConn->origPortId, &dialDigitBuf, sizeof(dialDigitBuf_t));  
			 	break;
			case EVT_CCM_CONN_CONF:        				
				vDspChanOpen(phoneCB);
				break;
			default:
			PH_FSM_DBG1("fxoFsmData-unknown event=", phoneEvent);
			break;
			}
	}else if ( phoneCB->linkSt == PHONE_LINK_ST_ONHOLD ) {
		switch( phoneEvent ){
			case EVT_CCM_UNHELD_REQ:
				peerCcmConn = ccmConn->peerCcmConn;
				if ( peerCcmConn != NULL ) {
					/* Need modify for the call-transfer by PBX */
					//fxoOnHook(phoneCB);
					//pause(160);
					//fxoOffHook(phoneCB);
					//pause(1000);
					/* Need modify for the call-transfer by PBX */
					peerPhoneCB = find_phoneCB(ccmConn->peerCcmConn->origPortId, 0);
					if ( peerPhoneCB != NULL ) {
						vDspChanOpen(phoneCB);
						phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
						fxofxsChannelConnect(phoneCB, 1);
					}
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					phoneCB->phoneFlags |= PHONE_PHYSICAL_FXO_NONFLASH;
					phoneCB->callProTimer.fxoNonFlashCheckTimer = timerStartS(TIMEOUT_FXO_NO_FLASH, pVoiceFxo->fxoFlashMaxInterval, phoneCB);			
					eventVoiceSimpleSend(EVT_CCM_UNHELD_CONF, MSG_SEND_TO_PHONE, peerCcmConn->origPortId);
				}
				break;
			case EVT_PEERFXO_BUSYTONE_DETECT:
				PH_FSM_DBG("fxoFsmData(ONHOLD)   EVT_PEERFXO_BUSYTONE_DETECT\r\n");
				break;
			default:
				PH_FSM_DBG1("fxoFsmData(ONHOLD)-unknown event=", phoneEvent);
				break;
		}
	}
	
fail:
	return status;	
}

/*______________________________________________________________________________
**	fxoFsmDrop
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
fxoFsmDrop(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("fxoFsmDrop - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmDrop - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}
		
		switch( phoneEvent ){
			default:
				PH_FSM_DBG1("fxoFsmDrop-unknown event=", phoneEvent);
				break;
		}

fail:
	return status;
}

/*______________________________________________________________________________
**	fxoFsmDisc
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
fxoFsmDisc(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("fxoFsmDisc - phoneEvent = ", phoneEvent, " phoneCB =", (uint32)phoneCB);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxoFsmDisc - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}
		
	switch( phoneEvent ){
		default:
			PH_FSM_DBG1("fxoFsmDisc-unknown event=", phoneEvent);
			break;
	}

fail:
	return status;

}

#endif /*FXO_DSPRELAY */

