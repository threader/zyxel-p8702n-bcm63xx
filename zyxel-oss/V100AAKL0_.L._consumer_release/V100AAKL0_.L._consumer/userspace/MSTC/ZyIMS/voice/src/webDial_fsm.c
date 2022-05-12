/*
  $Id: $
*/
/******************************************************************************/
/*
 *  Copyright (C) 2007 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */
/******************************************************************************/
/*
  $Log: $
 
*/

/***********************************************************
	 includes.
*******************************************************/
#include <time.h>
#include "global.h"
#include "switch.h"
#include "phfsm.h"
#include "Phcmd.h"
#include "Voipccm_sip_itf.h"
#include "call_mgnt.h"
#include "webDial_fsm.h"
#include "tr104.h"	

/*____________________________________________________________________________*/
/*	data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/
webRepeatDial_t *webRepeatDial = NULL;
char *rpWebDialStatusBuf_p = NULL;

/*_____ initial  variables _______________________________________________*/
int WebphoneStatus=WEBDIAL_STATE_IDLE;
int WebphoneGUIStatus=GUI_WEBDIAL_STATE_IDLE;   
#if 0
int FXO_Busy_Here=Busy_Here;
#endif
extern int ccm_releaseConn (ccmConn_t* ccmConn_p);
/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/
/*_
/*______________________________________________________________________________
**  createWebDialCB
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
int createWebDialCB(ccmConn_t *ccmConn_p, int physicalID)
{
	webdial_t *WebDialCB_p = NULL;

	CCM_FSM_DBG1("createWebDialCB, ccmConn_p=", (uint32)ccmConn_p);
	
	/* create the WebDial control block */
	WebDialCB_p = (webdial_t *)calloc(1, sizeof(webdial_t) );
//	WebDialCB_p->webdialStatus = WEB_DIAL_ST_1stZYPHONE_CREATE;
	ccmConn_p->WebDialCB=WebDialCB_p;
	if(ccmConn_p->WebDialCB != NULL){
		#if 0
		ccmConn_p->WebDialCB->PhonePhyID=1<<(physicalID); 
		#else
		ccmConn_p->WebDialCB->PhonePhyID = physicalID; 
		#endif
	}
	else{
		ZyPrintf("ccmConn_p->WebDialCB == NULL\n\r"); 
	}
	
	return 0;
}
/*______________________________________________________________________________
**  removeWebDialCB
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
void
removeWebDialCB(
	webdial_t *WebDialCB_p
){
	CCM_FSM_DBG("Web phone Dial: removeWebDialCB");
	
	WebDialCB_p->webdialPeerZyPhoneCCM = NULL;
	zyFree(WebDialCB_p);
	WebDialCB_p = NULL;

	return;
} /* removeConferenceCallCB */

/*______________________________________________________________________________
**  WebDialFsmProc
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
void
WebDialFsmProc(
	ccmConn_t *ccmConn_p,
	uint16 WebDialEvent,
	void *data_p,
	uint16 data_size
){
	
	CCM_FSM_DBG2("WebPhoneDialFsmProc: status=", ccmConn_p->WebDialCB->webdialStatus, " , Event=", WebDialEvent);
	
	switch( ccmConn_p->WebDialCB->webdialStatus) {
		case WEB_DIAL_ST_1stZYPHONE_CREATE:
			WebDialFsm1stZyPhoneCreate(ccmConn_p, WebDialEvent, data_p, data_size);
			break;
		case WEB_DIAL_ST_2ndZYPHONE_CREATE:
			WebDialFsm2ndZyPhoneCreate(ccmConn_p, WebDialEvent, data_p, data_size);
			break;
		case WEB_DIAL_ST_ZYPHONE_REMOVE:
			WebDialFsmZyPhoneRemove(ccmConn_p, WebDialEvent, data_p, data_size);
			break;
		default:
			CCM_FSM_DBG1("\r\n WebDialFsmProc Unknown Status: ", ccmConn_p->WebDialCB->webdialStatus);
			break;
	}
	
	return;
} /* WebDialFsmProc */



extern int WebRedialTimeout (tm_id_t timer_id, void *arg);
extern int WebDialTonedetetTimeout( tm_id_t tmId, void *param);
extern uint8 getWebRedialInterval(uint16 phyId);

/*______________________________________________________________________________
**  WebDialFsm1stZyPhoneCreate
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
void
WebDialFsm1stZyPhoneCreate(
	ccmConn_t *ccmConn_p,
	uint16 WebDialEvent,
	void *data_p,
	uint16 data_size
){
	ccmConn_t *newCcmConn_p = NULL;
	phoneCh_t *TempPhoneCB = NULL, *FXSPhoneCB=NULL;
	VoiceProfLineCallingFeaturesObj_t* lineFeatureObj = NULL;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	CCM_FSM_DBG2("WebDialFsm1stZyPhoneCreate: Event=", WebDialEvent," logicalld=",ccmConn_p->origPortId);

	if((ccmConn_p->WebDialCB==NULL) || ccmConn_p->WebDialCB->PhonePhyID >= NumOfPhyIntf){
		ZyPrintf("WebDial fatal error, exit\n");
		return;		
	}

	switch( WebDialEvent) {	
		case EVT_CCM_WEBDIAL_CHECK:
			if( (FXSPhoneCB=find_phoneCB((ccmConn_p->WebDialCB->PhonePhyID), 1)) == NULL){				
	            CCM_FSM_DBG("Can't find FXSPhoneCB at WebDialFsm1stZyPhoneCreate");
			    return;
	        }							
            else{			/* slicHookStatus()=1:OffHook,  0:OnHook*/
				lineFeatureObj = getAssociatedLineFeatures(FXSPhoneCB);

				#if 1 /*according to ES, retry CCBS only when phone is in IDLE */
				if(( slicHookStatus(FXSPhoneCB) == 0 && FXSPhoneCB->phStatus==PHONE_STATUS_IDLE ))
				#else
				if(( slicHookStatus(FXSPhoneCB) == 0 && FXSPhoneCB->phStatus==PHONE_STATUS_IDLE )
					||
					( ((FXSPhoneCB->phStatus==PHONE_STATUS_DATA) && 
					(FXSPhoneCB->linkSt==PHONE_LINK_ST_CONNECT)) && 
					(( checkLogicPhoneNum(FXSPhoneCB->physicalId-1) ==1 ) && 
					( ((NULL != lineFeatureObj) && (lineFeatureObj->callWaitingEnable)) || (FXSPhoneCB->dialInform.dialDigitNumber.dialDigitFlags & DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE)))))
				#endif
				{ 	   
					TempPhoneCB=find_phoneCB(ccmConn_p->origPortId, 0);
						
					if((NULL!=TempPhoneCB) && (TempPhoneCB->callId == 0)){
						TempPhoneCB->callId = FXSPhoneCB->logicalId; /*set ZyPhone1 callId to fxs phone logicalId*/
					}
					CCM_PRINTF("send EVT_PHONE_DIAL_REQ\n");
					eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, ccmConn_p->origFuncId, ccmConn_p->origPortId);
				}
				else{
					if(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialFlags & WEB_REPEAT_DIAL) {
						if(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes<webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialTimes)
						{
							CCM_PRINTF("send EVT_CCM_WEBDIAL_REDIAL\n");
							WebDialFsmProc(ccmConn_p,EVT_CCM_WEBDIAL_REDIAL, NULL, NULL);
						}
						else{
							CCM_PRINTF("send EVT_CCM_WEBDIAL_REDIAL_STOP\n");
							WebDialFsmProc(ccmConn_p,EVT_CCM_WEBDIAL_REDIAL_STOP, NULL, NULL);
						}
					}
					/*remove webdailCB, retry next time*/
					if(ccmConn_p->WebDialCB != NULL){
						removeWebDialCB(ccmConn_p->WebDialCB);
					}	
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, ccmConn_p->origFuncId, ccmConn_p->origPortId);								
					ccm_releaseConn(ccmConn_p);
                  	CCM_FSM_DBG("FXS is not ready!");
					_setWebDialStates_End;
					_copyWebDialLog("\r\nLocal phone is not ready!");
				}
			}
			break;
			
		case EVT_CCM_ASSIGN_REQ:  
			if( (TempPhoneCB=find_phoneCB(ccmConn_p->origPortId, 0)) == NULL){
	                 	 ZyPrintf("Can't find TempPhoneCB at WebDialFsm1stZyPhoneCreate");
			    	return;
	                }
			else{
				TempPhoneCB->linkSt = PHONE_LINK_ST_CALLING;
				TempPhoneCB->phStatus = PHONE_STATUS_DIAL;
			}
			break;

		case EVT_CCM_DIAL_REQ:  /* FXO */ /*Start  fxsWebDialCheckToneTimer*/
			if ( ccmConn_p->peerCcmConn->origPortType == DSP_FXO_PORT ) {	/* FXO */ /* Change Phone Status */
				if( (FXSPhoneCB=find_phoneCB((ccmConn_p->WebDialCB->PhonePhyID), 1)) == NULL){				
					 CCM_FSM_DBG("Can't find FXSPhoneCB at WebDialFsm1stZyPhoneCreate");
					 return;
				}							
	            else{
					FXSPhoneCB->callProTimer.fxsWebDialCheckToneTimer=  timerStartS(15000, WebDialTonedetetTimeout, ccmConn_p);
				}
			}
			break;
	
		case EVT_CCM_ALERT_IND:  
		case EVT_CCM_SDP_ALERT_IND:	/* aircheng add for that callee enable early media, [SPRID # 100504121] */
			ccmConn_p->WebDialCB->webdialStatus=WEB_DIAL_ST_2ndZYPHONE_CREATE;
			eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_CONF, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);	
			if ( ccmConn_p->peerCcmConn->origPortType == DSP_FXO_PORT ) {	/* FXO */ /* Change Phone Status */
				eventVoiceSimpleSend(EVT_CCM_CONN_IND, ccmConn_p->peerCcmConn->origFuncId, ccmConn_p->peerCcmConn->origPortId);
			}
			break;
			
		case EVT_CCM_WEBDIAL_FAIL:    
			if( (TempPhoneCB=find_phoneCB(ccmConn_p->origPortId, 0)) == NULL){
	                  ZyPrintf("Can't find TempPhoneCB at WebDialFsm1stZyPhoneCreate");
			    return;
	                }					
			if(ccmConn_p->WebDialCB != NULL){
				removeWebDialCB(ccmConn_p->WebDialCB);
			}		
			eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, TempPhoneCB->logicalId, TempPhoneCB->dspPortType);
			eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, TempPhoneCB->logicalId);
			
			if (data_p !=  NULL) {	
				if ( ccmConn_p->peerCcmConn!= NULL ) {
					#if 0
					if ( ccmConn_p->peerCcmConn->origPortType == DSP_FXO_PORT ) {
						_setWebDialStates_End;	
						//_copyWebDialLog("\r\nWeb dial fail, reason: Detect peer phone is busy.");
						_copyWebDialLog("\r\nWeb dial fail, reason: %s.", (getFXOStatusCodeToString(*(int *)data_p )));
						
					}
					#endif
				}
				else{
					_setWebDialStates_End;
					#if 0
					_copyWebDialLog("\r\nWeb dial fail, reason: %s.", (uaStatusCodeToString(*(uint32 *)data_p )));
					#else
					_copyWebDialLog("\r\nWeb dial fail.");
					#endif
				}
			}
			else{
				_setWebDialStates_End;	
				_copyWebDialLog("\r\nWeb dial fail!");
			}
			break;
			
		case EVT_CCM_REL_REQ:
		case EVT_CCM_DISC_REQ:
			ccmConn_p->connSt = CALL_CONN_ST_IDLE;
			ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
			if(ccmConn_p->flags & CALL_CONN_FLAG_WEBDIAL){
				ccmConn_p->flags &= ~(CALL_CONN_FLAG_WEBDIAL);
			}
			ccm_releaseConn(ccmConn_p);
			break;	
			
		case EVT_CCM_WEBDIAL_REDIAL: 
			if( (FXSPhoneCB=find_phoneCB((ccmConn_p->WebDialCB->PhonePhyID), 1)) == NULL){				
				 CCM_FSM_DBG("Can't find FXSPhoneCB at WebDialFsm1stZyPhoneCreate");
				 return;
			}							
            else{			
				FXSPhoneCB->callProTimer.fxsWebRepeatDialTimer = timerStartS(1000*getWebRedialInterval(ccmConn_p->WebDialCB->PhonePhyID) , WebRedialTimeout, ccmConn_p->WebDialCB->PhonePhyID);	
				
			}
			break;	
			
		case EVT_CCM_WEBDIAL_REDIAL_STOP: 
			_copyWebDialLog("\r\nStop repeat dial!");
			/* Reset the webRepeatDial initial value */
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialInterval=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhonePhyID=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhoneType=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialTimes=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes=0;
			memset(&(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));
			break;
					
		default:
			CCM_FSM_DBG1("\r\n WebDialFsm1stZyPhoneCreate Unknown Event: ", WebDialEvent);
			break;
	}

	return;
} /* WebDialFsm1stZyPhoneCreate*/

/*______________________________________________________________________________
**  WebDialFsm2ndZyPhoneCreate
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
void
WebDialFsm2ndZyPhoneCreate(
	ccmConn_t *ccmConn_p,
	uint16 WebDialEvent,
	void *data_p,
	uint16 data_size
){
	ccmConn_t *new2CcmConn_p = NULL;
	phoneCh_t *TempPhoneCB = NULL, *phoneCB=NULL, *FXSPhoneCB=NULL;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

CCM_FSM_DBG2("WebDialFsm2ndZyPhoneCreate: Event=", WebDialEvent," logicalld=",ccmConn_p->origPortId);
	if((ccmConn_p->WebDialCB==NULL) || ccmConn_p->WebDialCB->PhonePhyID >= NumOfPhyIntf){
		ZyPrintf("WebDial fatal error, exit\n");
		return; 	
	}

	switch( WebDialEvent) {		
		case EVT_CCM_WEBDIAL_CONF:  
			if ( (new2CcmConn_p = create_ZyPhone(NULL, 0) ) != NULL ) {
				createWebDialCB(new2CcmConn_p,webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhonePhyID);
				new2CcmConn_p->WebDialCB->webdialStatus = WEB_DIAL_ST_2ndZYPHONE_CREATE;
				new2CcmConn_p->flags |= CALL_CONN_FLAG_WEBDIAL;
				{
					if( (phoneCB=find_phoneCB(new2CcmConn_p->origPortId, 0)) == NULL){
			            ZyPrintf("Can't find PhoneCB at WebDialFsm2ndZyPhoneCreate");
					    return;
		            }
					else
					{
						memset(&(phoneCB->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t));
					}
				}

				/*set ZyPHONE2 callId to fxs phone logicalId*/
				TempPhoneCB = find_phoneCB(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhonePhyID, 1);
				if((NULL != phoneCB) && (phoneCB->callId == 0)){
					phoneCB->callId = TempPhoneCB->logicalId;
				}
						
				new2CcmConn_p->WebDialCB->webdialPeerZyPhoneCCM=ccmConn_p;			//co-copy peerZyPhoneCCM
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM=new2CcmConn_p;
				eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, new2CcmConn_p->origFuncId, new2CcmConn_p->origPortId);				
				_copyWebDialLog("\r\nPeer Phone is ringing");
			}
			else{
				ZyPrintf("WebDialFsm2ndZyPhoneCreate error\n\r"); 
			}
			break;

		case EVT_CCM_ASSIGN_REQ:  
			if( (TempPhoneCB=find_phoneCB(ccmConn_p->origPortId, 0)) == NULL){
	                 	 ZyPrintf("Can't find TempPhoneCB at WebDialFsm1stZyPhoneCreate");
			    	return;
	                }
			else{
				TempPhoneCB->linkSt = PHONE_LINK_ST_CALLING;
				TempPhoneCB->phStatus = PHONE_STATUS_DIAL;
			}
			break;

		case EVT_CCM_ALERT_REQ:
			if (ccmConn_p->peerCcmConn!=NULL){
				ccmConn_p->peerCcmConn->flags |= CALL_CONN_FLAG_WEBDIAL;
				ccmConn_p->WebDialCB->webdialStatus = WEB_DIAL_ST_ZYPHONE_REMOVE;
				if(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialFlags &WEB_REPEAT_DIAL){
					eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL_STOP, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);	
				}
				eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REMOVE, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);	
			}
			break;	
		
		case EVT_CCM_DISC_IND:   //For ISEN_TE busy 
			if (ccmConn_p->peerCcmConn->origPortType==ISDN_TE_PORT){
				if( (FXSPhoneCB=find_phoneCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortId, 0)) == NULL){
	                ZyPrintf("Can't find FXSPhoneCB at WebDialFsm2ndZyPhoneCreate");
			    	return;
	            }   
				else{  //Release Local FXS phone & ccm
					phStopTone(FXSPhoneCB);
					phStopRing(FXSPhoneCB);		
					eventVoiceSimpleSend(EVT_CCM_DISC_IND, MSG_SEND_TO_PHONE, FXSPhoneCB->logicalId);
					
					ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->connSt = CALL_CONN_ST_IDLE;
					ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->connRole = CCM_CONN_ROLE_UNDEF;
					ccm_releaseConn(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn);
				}

				////////delete ZyPhone1 CCM and ZyPhone2 CCM///////////   ZyPhone1 CCM =ccmConn_p
				//delete Zyphone2                                                                      ZyPhone2 CCM =ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn=NULL;
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB->webdialPeerZyPhoneCCM=NULL;
				if(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB != NULL){
					removeWebDialCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB);
				}
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->origFuncId, ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->origPortId);

				//delete Zyphone1
				ccmConn_p->peerCcmConn=NULL;
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM=NULL;
				if(ccmConn_p->WebDialCB != NULL){
					removeWebDialCB(ccmConn_p->WebDialCB);
				}
				 eventVoiceSimpleSend(EVT_PHONE_REMOVE, ccmConn_p->origFuncId, ccmConn_p->origPortId);								
				//////////////////////////////////////////////////// 
			}			
			break;
				
		case EVT_CCM_WEBDIAL_FAIL:    	
			if (data_p !=  NULL) {	
				if ( zyStrcmp((char *)data_p, "EVT_PEERFXO_BUSYTONE_DETECT") == 0 ){			
					_setWebDialStates_End;			
					_copyWebDialLog("\r\nWeb dial fail, reason: Detect peer phone is busy.");
				}
				else{
					_setWebDialStates_End;	
					#if 0
					_copyWebDialLog("\r\nWeb dial fail, reason: %s.", (uaStatusCodeToString(*(uint32 *)data_p )));
					#else
					_copyWebDialLog("\r\nWeb dial fail." );
					#endif
					//dbg_printf("\r\n%s",uaStatusCodeToString(*(uint32 *)data_p ));
				}
			}
			else{
				_setWebDialStates_End;			
				_copyWebDialLog("\r\nWeb dial fail!");
			}
			break;
			
		case EVT_CCM_WEBDIAL_REDIAL: 
			if( (FXSPhoneCB=find_phoneCB((ccmConn_p->WebDialCB->PhonePhyID), 1)) == NULL){				
				 CCM_FSM_DBG("Can't find FXSPhoneCB at WebDialFsm1stZyPhoneCreate");
				 return;
			}							
			else{
				FXSPhoneCB->callProTimer.fxsWebRepeatDialTimer = timerStartS(1000*getWebRedialInterval(ccmConn_p->WebDialCB->PhonePhyID) , WebRedialTimeout, ccmConn_p->WebDialCB->PhonePhyID);	
								
			}
			break;	
			
		case EVT_CCM_WEBDIAL_REDIAL_STOP: 
			_copyWebDialLog("\r\nStop repeat dial!");
			/* Reset the webRepeatDial initial value */
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialInterval=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhonePhyID=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhoneType=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialTimes=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes=0;
			zyMemset(&(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));
			break;

#if 1	/* aircheng add for call_id display */
		case EVT_CCM_DIAL_IND:

			//ZyPrintf("origPortId = %d, origPortType = %d\n", ccmConn_p->origPortId, ccmConn_p->origPortType);
			//ZyPrintf("current_length = %d, dial_digits = %s\n", ccmConn_p->dialInform.dialDigitNumber.current_length, ccmConn_p->dialInform.dialDigitNumber.dial_digit);

			if( (ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB != NULL) && ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB->PhonePhyID < NumOfPhyIntf){
				zyMemcpy(&(ccmConn_p->dialInform.dialDigitNumber), 
					&(webRepeatDial[ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB->PhonePhyID].webRepeatDialDigitNumber), 
					sizeof(dialDigitNum_t));
			}
			else{
				CCM_FSM_DBG("Can't copy webRepeatDialDigitNumber to ccm of the 2nd zyphone");
			}

			//ZyPrintf("origPortId = %d, origPortType = %d\n", ccmConn_p->origPortId, ccmConn_p->origPortType);
			//ZyPrintf("current_length = %d, dial_digits = %s\n", ccmConn_p->dialInform.dialDigitNumber.current_length, ccmConn_p->dialInform.dialDigitNumber.dial_digit);

			break;
#endif

		default:
			CCM_FSM_DBG1("\r\n WebDialFsm2ndZyPhoneCreate Unknown Event: ", WebDialEvent);
			break;
	}

	//WebDialFsmProc(ccmConn_p,WebDialEvent, NULL, NULL);
	return;
} /* WebDialFsm2ndZyPhoneCreate */

/*______________________________________________________________________________
**  WebDialFsmZyPhoneRemove
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
void
WebDialFsmZyPhoneRemove(
	ccmConn_t *ccmConn_p,
	uint16 WebDialEvent,
	void *data_p,
	uint16 data_size
){

	ccmConn_t *tmpCcmConn=NULL, *tmpPeerCcmConn=NULL;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	CCM_FSM_DBG2("WebDialFsmZyPhoneRemove: Event=", WebDialEvent," logicalld=",ccmConn_p->origPortId);
	if((ccmConn_p->WebDialCB==NULL) || ccmConn_p->WebDialCB->PhonePhyID >= NumOfPhyIntf){
		ZyPrintf("WebDial fatal error, exit\n");
		return;		
	}

	switch( WebDialEvent) {
		case EVT_CCM_WEBDIAL_REDIAL_STOP: 
			_copyWebDialLog("\r\nStop repeat dial!");
			/* Reset the webRepeatDial initial value */
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialInterval=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhonePhyID=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialPhoneType=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialTimes=0;
			webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialCurrentTimes=0;
			zyMemset(&(webRepeatDial[ccmConn_p->WebDialCB->PhonePhyID].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));
			break;	
			
		case EVT_CCM_WEBDIAL_REMOVE:  
			////////re-associate caller PhoneCB CCM and SIP ccm///////
			//PhoneCB CCM =SIP ccm             // ZyPhone2 ccm= ccmConn_p
			                                                    //  ZyPhone1 ccm= ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM
			ccmConn_p->peerCcmConn->peerCcmConn=ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn;		
			ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->peerCcmConn=ccmConn_p->peerCcmConn;

			////////Copy the ZyPhone1 CCM to  PhoneCB CCM ///////
			ccmConn_p->peerCcmConn->connSt=ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->connSt;
			ccmConn_p->peerCcmConn->connRole=ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->connRole;
			

			/*check if it has bundled ccm. If yes, continue the bundle!*/
			tmpPeerCcmConn = ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn; //  ZyPhone1 ccm->peerCcmConn
			while(tmpPeerCcmConn->bundleCcmList.bundleCcmNext!=NULL){	
				tmpCcmConn = tmpPeerCcmConn->bundleCcmList.bundleCcmNext;		
				tmpCcmConn->peerCcmConn = ccmConn_p->peerCcmConn;  /*The final bundleCCM*/
				tmpPeerCcmConn = tmpCcmConn;
			}
				
			////////Copy the ZyPhone1 state to PhoneCB///////
			{
         		phoneCh_t *fxsCallerPhoneCB=NULL, *ZyPhone1CB=NULL, *FXOCalleePhoneCB=NULL, *FXSCalleePhoneCB=NULL, *ISDN_TECalleePhoneCB=NULL;
				#if 0
				voiceIvrCh_t *newVoiceIvrCh_p = NULL;
				#endif
				
			      //find ZyPhone1CB
				if( (ZyPhone1CB=find_phoneCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->origPortId, 0)) == NULL){
		            ZyPrintf("Can't find ZyPhone1CB at WebDialFsmZyPhoneRemove");
				    return;
	            }
				//find fxsCallerPhoneCB
                if( (fxsCallerPhoneCB=find_phoneCB(ccmConn_p->peerCcmConn->origPortId, 0)) == NULL){
		            ZyPrintf("Can't find fxsCallerPhoneCB at WebDialFsmZyPhoneRemove");
				    return;
	            }					
				fxsCallerPhoneCB->phStatus =ZyPhone1CB->phStatus;	
				fxsCallerPhoneCB->linkSt = ZyPhone1CB->linkSt;
				fxsCallerPhoneCB->callId = ZyPhone1CB->callId;
#ifdef CCBS_SUPPORT		/* aircheng add, [SPRID # 100504121] */
				/* only set PHONE_CALLEE_EARLY_MEDIA in phoneFlags */
				fxsCallerPhoneCB->phoneFlags |= (ZyPhone1CB->phoneFlags & PHONE_CALLEE_EARLY_MEDIA);
#endif				
				CCM_PRINTF("fxs phoneCB logicalId = %d, callId set to %d\n", fxsCallerPhoneCB->logicalId, fxsCallerPhoneCB->callId);
				fxsCallerPhoneCB->peerMediaChId= ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortId;  //Doris Debug0228					
   				
				CCM_FSM_DBG1("callee= ",ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortType);
				if ( ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortType == DSP_FXO_PORT ) {	/* callee = FXO */
					/* Change FXS ccm Status */
					ccmConn_p->peerCcmConn->connSt = CALL_CONN_ST_CONNRDY;
					
					//fxsCallerPhoneCB->voipDspCallBack=dspSendToDsp;

					if( (FXOCalleePhoneCB=find_phoneCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortId, 0)) == NULL){	//find CalleePhoneCB
				    	ZyPrintf("Can't find FXOCalleePhoneCB at WebDialFsmZyPhoneRemove");
						return;
			        }
					FXOCalleePhoneCB->peerMediaChId= ccmConn_p->peerCcmConn->origPortId;  
					#if 0
					FXOCalleePhoneCB->voipDspCallBack=dspSendToDsp;	
					#endif
					if( slicHookStatus(fxsCallerPhoneCB) == 0 )   { 	/* 1=OffHook,  0=OnHook*/	
						#if 0
						if ( (newVoiceIvrCh_p = createVoiceIvrCh( fxsCallerPhoneCB->logicalId, fxsCallerPhoneCB->dspPortType, RtpSendToDsp)) != NULL ) {		
							PH_FSM_DBG("FXO->PlayIvr: IVRPRON_DEF_RINGBACKTONE");						
						    ivrPlayStart(newVoiceIvrCh_p, IVRPRON_DEF_RINGBACKTONE, 1, 100);	/*repeat interval=100ms*/
							
						}
						#endif
					}
					
					vDspChanOpen(fxsCallerPhoneCB);
					
					
					//vDspDisableToneDetection(FXOCalleePhoneCB);// protect disable FXO ToneDetection
					
				}
				else if ( ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortType == SIP_PORT ) {	/* callee = SIP */
					//fxsCallerPhoneCB->voipDspCallBack=rtp_send;	 
				}   
				else if ( ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortType == DSP_FXS_PORT ) {	/* callee = FXS */
					/* internal callee = DSP_FXS_PORT */
					if( (FXSCalleePhoneCB=find_phoneCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn->origPortId, 0)) == NULL){	//find CalleePhoneCB
				                  ZyPrintf("Can't find FXSCalleePhoneCB at WebDialFsmZyPhoneRemove");
						    return;
			              }
					FXSCalleePhoneCB->peerMediaChId= ccmConn_p->peerCcmConn->origPortId;  
					//FXSCalleePhoneCB->voipDspCallBack=dspSendToDsp;
					
					//fxsCallerPhoneCB->voipDspCallBack=dspSendToDsp;	 
				} 
				else {  /* internal callee = ISDN_NT_PORT */  /*ISDN_TE_PORT*/	
					//fxsCallerPhoneCB->voipDspCallBack=dspSendToDsp;	 
				}  
				zyMemcpy(&(fxsCallerPhoneCB->dialInform), &(ZyPhone1CB->dialInform), sizeof(dialInfor_t));	
			}	
			
				////////delete ZyPhone1 CCM and ZyPhone2 CCM///////////
				//delete Zyphone1
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->peerCcmConn=NULL;
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB->webdialPeerZyPhoneCCM=NULL;
				if(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB != NULL){
					removeWebDialCB(ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->WebDialCB);
				}
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->origFuncId, ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM->origPortId);

				//delete Zyphone2
				ccmConn_p->peerCcmConn=NULL;
				ccmConn_p->WebDialCB->webdialPeerZyPhoneCCM=NULL;
				if(ccmConn_p->WebDialCB != NULL){
					removeWebDialCB(ccmConn_p->WebDialCB);
				}
				 eventVoiceSimpleSend(EVT_PHONE_REMOVE, ccmConn_p->origFuncId, ccmConn_p->origPortId);								
				//////////////////////////////////////////////////// 			
				_copyWebDialLog("\r\nLocal Phone is ringing");
			break;

		default:
			CCM_FSM_DBG1("\r\n WebDialFsm2ndZyPhoneCreate Unknown Event: ", WebDialEvent);
			break;
	}

} /* WebDialFsmZyPhoneRemove */


/*______________________________________________________________________________
**	WebRedialTimeout
**
**	descriptions:    Use to check the first dial digits to input.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
WebRedialTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	int WebDialPhoneType;
	uint16 physicalID;
	char DialNumber_p[MAX_DIAL_STRING_LENGTH];
	ccmConn_t *tmpWebReDialCcm_p=NULL;
	phoneCh_t *FXSPhoneCB = NULL;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	PH_FSM_DBG("*****WebRepeatDialTimeout******"); 
	physicalID = (uint16)arg;
	PH_FSM_PRINTF("Phone PhysicalId = %d\n", physicalID);

	if((physicalID >= NumOfPhyIntf)){
		ZyPrintf("WebDial fatal error, exit\n");
		return 0;
	}
	FXSPhoneCB = find_phoneCB(physicalID, 1); 
	PH_FSM_PRINTF("*****Times %d/%d******\n",webRepeatDial[physicalID].webRepeatDialCurrentTimes, webRepeatDial[physicalID].webRepeatDialTimes); 
	if(webRepeatDial[physicalID].webRepeatDialCurrentTimes<webRepeatDial[physicalID].webRepeatDialTimes){
		zyStrncpy(DialNumber_p, webRepeatDial[physicalID].webRepeatDialDigitNumber.dial_digits, MAX_DIAL_STRING_LENGTH);
		WebDialPhoneType = webRepeatDial[physicalID].webRepeatDialPhoneType;
		physicalID = webRepeatDial[physicalID].webRepeatDialPhonePhyID;
		
		//PH_FSM_DBG1("Doris-> webRepeatDial.webRepeatDialTimes1= ", webRepeatDial.webRepeatDialTimes); //Doris Debug
		//PH_FSM_DBG1("Doris-> webRepeatDial.webRepeatDialTimes1= ", webRepeatDial.webRepeatDialCurrentTimes); //Doris Debug
		webRepeatDial[physicalID].webRepeatDialCurrentTimes++;
		//PH_FSM_DBG1("Doris-> webRepeatDial.webRepeatDialCurrentTimes2= ", webRepeatDial.webRepeatDialCurrentTimes); //Doris Debug

		_copyWebDialLog("\r\n******Web Repeat Dial Timeout******");
		_copyWebDialLog("\r\nRepeat Dial Start...");/*486*/
		_copyWebDialLog("\r\nRepeat Dial Times=%d/%d\r\n",webRepeatDial[physicalID].webRepeatDialCurrentTimes,webRepeatDial[physicalID].webRepeatDialTimes);

		if (WebPhoneDialint(WebDialPhoneType, physicalID,DialNumber_p)==-1){
			ZyPrintf("WebPhoneDial init fail!\n\r");
		}
			
	}
	else{
		PH_FSM_PRINTF("retry timeout, stop redial, phone physicalId = %d\n", physicalID);
		/* Reset the webRepeatDial initial value */
		webRepeatDial[physicalID].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
		webRepeatDial[physicalID].webRepeatDialInterval=0;
		webRepeatDial[physicalID].webRepeatDialPhonePhyID=0;
		webRepeatDial[physicalID].webRepeatDialPhoneType=0;
		webRepeatDial[physicalID].webRepeatDialTimes=0;
		webRepeatDial[physicalID].webRepeatDialCurrentTimes=0;
		zyMemset(&(webRepeatDial[physicalID].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));
		
	} 	
	
	return 0;
} /* WebRedialTimeout */

/*______________________________________________________________________________
**	WebDialTonedetetTimeout
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
WebDialTonedetetTimeout( 
	tm_id_t tmId, 
	void *param 
) {

	ccmConn_t *ccmConn_p=(ccmConn_t *)param;
	phoneCh_t *FXSPhoneCB=NULL, *FXOPhoneCB=NULL;
	
PH_FSM_DBG1("WebDialTonedetetTimeout: logicalld=",ccmConn_p->origPortId);
	
	//Send EVT_CCM_ALERT_IND to create Zyphone2
	if (ccmConn_p->WebDialCB!=NULL){
		PH_FSM_DBG("WebDialTonedetetTimeout: ccmConn_p->WebDialCB!=NULL");	
		if( (FXSPhoneCB=find_phoneCB((ccmConn_p->WebDialCB->PhonePhyID), 1))!= NULL){			
			FXSPhoneCB->callProTimer.fxsWebDialCheckToneTimer=0;
		}
		if( (FXOPhoneCB=find_phoneCB((ccmConn_p->peerCcmConn->origPortId), 0))!= NULL){	
			vDspDisableToneDetection(FXOPhoneCB);
		}
		eventVoiceCCMSimpleSend(EVT_CCM_ALERT_IND, MSG_SEND_TO_CCMCONN, ccmConn_p->origPortId, ccmConn_p->origPortType);		
	}


	return 0;	
}

/*______________________________________________________________________________
**	getWebRedialInterval
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
uint8 getWebRedialInterval(uint16 phyId){
	uint8 RedialInterval;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	if((phyId >= NumOfPhyIntf)){
		ZyPrintf("fatal error, paramater out of range\n");
		return 0;
	}

	RedialInterval = webRepeatDial[phyId].webRepeatDialInterval;
       PH_FSM_DBG1("\r\nReDial Interval:", RedialInterval);
	return RedialInterval;
}	

/*______________________________________________________________________________
**	getCallMgntWebDailFlag
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
uint8 getCallMgntWebDailFlag(){
	uint8 ret=0;
	ccmConn_t *tmpCcmConn=NULL;
	PH_FSM_DBG("getCallMgntWebDailFlag");

	tmpCcmConn = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn != NULL ) {
		if(tmpCcmConn->flags & CALL_CONN_FLAG_WEBDIAL){  //Enable FXS WebDial
			ret=1;	
			break;
		}
		else {
			tmpCcmConn = tmpCcmConn->next;
		}
	}	
	PH_FSM_DBG1("getCallMgntWebDailFlag, ret=",ret);
	return ret;
}

/*______________________________________________________________________________
**	copyWebDialLog
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
void copyWebDialLog(char *fmt, ...){
	char tmpsbuffer[1024];
	va_list argptr;
	int len_insert=0, len_current=0, len_sum=0;
//ZyPrintf("\r\n copyWebDialLog, WebphoneGUIStatus=%d", WebphoneGUIStatus);
	if( (WebphoneGUIStatus == GUI_WEBDIAL_STATE_IDLE) && (WebphoneStatus==WEBDIAL_STATE_IDLE)){
		return;
	}
	
	if (rpWebDialStatusBuf_p != NULL){
		va_start(argptr, fmt);
		vsprintf(tmpsbuffer, fmt, argptr);
	//ZyPrintf("\r\n copyWebDialStates=%s", tmpsbuffer);

		len_insert = zyStrlen(tmpsbuffer);
		len_current = zyStrlen(rpWebDialStatusBuf_p);

		len_sum= len_current + len_insert;
		
		if(len_sum < 1024){		
			zyStrcat(rpWebDialStatusBuf_p, tmpsbuffer);
	//ZyPrintf("\r\n rpWebDialStatusBuf_p=%s", rpWebDialStatusBuf_p);
			if (WebphoneStatus==WEBDIAL_STATE_START){
				ZyPrintf("\r\n%s", tmpsbuffer);
			}
		}
		else{
			ZyPrintf("\r\ncopyWebDialStates: string buffer over 1024 ");
		}
		
		va_end(argptr);
	}
	else{
	//ZyPrintf("\r\n rpWebDialStatusBuf_p = NULL");
	}
}

/*______________________________________________________________________________
**	setWebDialStates_START
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
void setWebDialStates_Start(void){
//ZyPrintf("\r\n setWebDialStates_Start, WebphoneGUIStatus=%d", WebphoneGUIStatus);
	if (WebphoneGUIStatus == GUI_WEBDIAL_STATE_IDLE){
		return;
	}

	 if (WebphoneGUIStatus == GUI_WEBDIAL_STATE_REDAILSTART){
	 	//do nothing
	 }
	 else{
		/*alloc Dial States buffer-rpWebDialStatusBuf_p*/
		if (rpWebDialStatusBuf_p != NULL){
			zyFree(rpWebDialStatusBuf_p);
			rpWebDialStatusBuf_p = NULL;
		}
		
		if (rpWebDialStatusBuf_p == NULL){
			if ( ( rpWebDialStatusBuf_p = (char *)malloc(1024*sizeof(char))) == NULL ) {
				ZyPrintf("alloc rpWebDialStatusBuf_p error\n\r");
				return;
			}
			zyMemset(rpWebDialStatusBuf_p, 0, sizeof(rpWebDialStatusBuf_p));
			//memset(rpWebDialStatusBuf_p, 0, sizeof(1024*sizeof(char)));
			//ZyPrintf("\r\n rpWebDialStatusBuf_p size=%d", sizeof(rpWebDialStatusBuf_p));
		}
	 }
	
}

/*______________________________________________________________________________
**	setWebDialStates_START
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
void setWebDialStates_End(void){

	if (WebphoneGUIStatus == GUI_WEBDIAL_STATE_IDLE){
		// do nothing
	}
	else{
		WebphoneGUIStatus = GUI_WEBDIAL_STATE_END;
	}
	return;
}

/*______________________________________________________________________________
**	getWebDialStatus
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
int getWebDialStatus(ccmConn_t *ccmConn){

	uint8 webdialStatus;
	
	webdialStatus=ccmConn->WebDialCB->webdialStatus;
	PH_FSM_DBG1("getWebDialwebdialStatus = ", webdialStatus);
	
	if (webdialStatus==WEB_DIAL_ST_1stZYPHONE_CREATE){
		return 0;
	}
	else{
		return -1;
	}	
}
	
#if 0
extern int zyxelTE;
/*______________________________________________________________________________
**	WebDialAssociate
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
int WebDialAssociate(ccmConn_t* ccmConn_p)
{
	int connType = CCM_CONN_TYPE_FAIL;
	int fxsTranslate = 0, phoneFeature = 0, PhoneSipSelect=-1;
	//dialInfor_t *dialInform = NULL;
	uint8 phFeatureLen = 0;
	phoneCh_t *ZyPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);

CCM_FSM_DBG1("WebDialAssociate, origPortType=", ccmConn_p->origPortType);

	/* check the Speed Dial Phone Book */
	checkSpeedDialPhBook( &(ccmConn_p->dialInform.dialDigitNumber) );

	/* check the PSTN Phone Book */
	#ifdef ISDN_TE_SUPPORT
		if(checkIsdnLinePlugIn(0)){
			fxsTranslate = checkISDNPhoneBook( &(ccmConn_p->dialInform.dialDigitNumber) );
		}
		#ifdef CROSS_INTERFACE /*Theresa_mod_111506*/
			if(fxsTranslate == 0)
		#else
			if((phoneFeature == 0) && (fxsTranslate == 0)) 
		#endif
	#endif /*ISDN_TE_SUPPORT*/
	fxsTranslate = checkPSTNPhoneBook( &(ccmConn_p->dialInform.dialDigitNumber) );
	
	#ifdef FXS_PHONEBOOK
		if ((phoneFeature == 0) &&(fxsTranslate == 0)) {
			fxsTranslate = checkFXSPhoneBook(ccmConn_p);
		}
	#endif /* FXS_PHONEBOOK */		
	
	#ifdef DIAL_PLAN
		if( (phoneFeature == 0) && (fxsTranslate == 0)){
              	  fxsTranslate = checkDialPlan(&(ccmConn_p->dialInform.dialDigitNumber));
            	}
	#endif

	/* Check if the special numbers exist to trigger the phone feature */
	if ( zyxelTE == 0 ) {
		phoneFeature = checkPhoneFeature( (char *)&(ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]), &phFeatureLen );
	}
	else{
		phoneFeature = ZyXELTeTest( (char*)(&ccmConn_p->dialInform.dialDigitNumber.dial_digits) );
	}	
	
CCM_FSM_DBG2("WebDialAssociate, fxsTranslate=", fxsTranslate, ", phoneFeature =", phoneFeature);	
	
	if((fxsTranslate==1)||(fxsTranslate==2)){	
		connType = associateZyPhone2Fxo(ccmConn_p,0);
	}
#ifdef ISDN_TE_SUPPORT
	else if ( fxsTranslate == 0x11 )  {
		connType = associateZyPhone2IsdnTe(ccmConn_p);
	}
#endif /*ISDN_TE_SUPPORT*/
#ifdef INTERNAL_CALL_FEATURE	
	else if(((fxsTranslate==3)||(fxsTranslate==4))){   //internal FXS call || internal ISDN call
		if (ccmConn_p->fxsFxsMap==ccmConn_p->WebDialCB->PhonePhyID){   //WebDial intenal call-Call calls itself
			ZyPhoneCB->linkSt = PHONE_LINK_ST_IDLE;
                 	ZyPhoneCB->phStatus = PHONE_STATUS_IDLE;
			_copyWebDialLog("\r\nCall calls itself");
			if(webRepeatDial.webRepeatDialFlags & WEB_REPEAT_DIAL){
				eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_REDIAL_STOP, callMgnCb.sig.func_id, ccmConn_p->origPortId, ccmConn_p->origPortType);	
			}	
			eventVoiceCCMSimpleSend(EVT_CCM_WEBDIAL_FAIL, callMgnCb.sig.func_id, ccmConn_p->origPortId, ccmConn_p->origPortType);	

		}else{
		
			    ccmConn_p->SipFxsMap=ccmConn_p->fxsFxsMap;
			    ccmConn_p->SipFxsMap = ccmConn_p->SipFxsMap & ~(ccmConn_p->WebDialCB->PhonePhyID);					
	                  connType = associateZyPhone2Fxs(ccmConn_p);
			    #if defined (ISDN_SUPPORT)		
			    connType = associateZyPhone2IsdnNt(ccmConn_p,connType);
			    #endif			 
		}
				 }
#endif /*INTERNAL_CALL_FEATURE*/
	else if (( phoneFeature != 0) && (fxsTranslate==0)){  //internal #### call 
		if  ( phoneFeature == INTERNAL_CALL ) {
			ccmConn_p->SipFxsMap = ccmConn_p->SipFxsMap & ~(ccmConn_p->WebDialCB->PhonePhyID);					
			connType = associateZyPhone2Fxs( ccmConn_p );
			#ifdef ISDN_NT_SUPPORT
				ccmConn_p->dialInform.dialDigitNumber.current_length=0;
				memset( &(ccmConn_p->dialInform.dialDigitNumber.dial_digits[0]), 0, MAX_DIAL_STRING_LENGTH);	
				connType = associateZyPhone2IsdnNt(ccmConn_p,connType);
			#endif
		} 
	}
	else if(strlen(ccmConn_p->dialInform.dialDigitNumber.dial_digits)!=0){
		PhoneSipSelect=voipGetPhoneSipSelectByPhonePort((ccmConn_p->WebDialCB->PhonePhyID)-1);   //PhonePort= 0~ (PH_FXS_NUM_CHANNELS-1)		
		ccmConn_p->SipFxsMap=PhoneSipSelect;
		connType = associateZyPhone2Sip(ccmConn_p);
		}	
	else {
		ccmConn_p->SipFxsMap=ccmConn_p->WebDialCB->PhonePhyID;  //ZyPhone2 call local phone
		connType = associateZyPhone2Fxs(ccmConn_p);
	}	

	if ( connType == CCM_CONN_TYPE_FAIL ) {
		ccmConn_p->connType = CCM_CONN_TYPE_FAIL;
		ccmConn_p->connRole = CCM_CONN_ROLE_UNDEF;
		memset( &(ccmConn_p->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t) );
	}

	CCM_FSM_DBG1("WebDialAssociate, connType=", connType);
	
	return connType;
}
#endif

#if 0
const char* getFXOStatusCodeToString(int FXOStatusCode)
{
	switch(FXOStatusCode){

	case Busy_Here:
		//return  "Busy here";		
		return  "Detect peer phone is busy";
		break;
		
	default:
		return "Error Status Code";	
		break;
	}
}
#endif

int webPhoneDialStart(int WebDialPhoneType, int physicalID, char *DialNumber_p){
	webRepeatDial[physicalID].webRepeatDialFlags |= WEB_REPEAT_DIAL;
	webRepeatDial[physicalID].webRepeatDialInterval= 60; /*should read from config*/
	webRepeatDial[physicalID].webRepeatDialTimes= 10; /*should read from config*/
	webRepeatDial[physicalID].webRepeatDialCurrentTimes=0;
	return WebPhoneDialint(WebDialPhoneType, physicalID, DialNumber_p);

}

int  WebPhoneDialint(int WebDialPhoneType, int physicalID, char *DialNumber_p)
{
	ccmConn_t *newCcmConn_p = NULL;
	phoneCh_t *ZyPhoneCB;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	PH_FSM_DBG("WebPhoneDialint ");
	if((physicalID >= NumOfPhyIntf)){
		ZyPrintf("%s fail, physicalId out of range\n", __FUNCTION__);
		return -1;
	}

	/* protect webRepeatDial initial values=o*/
	zyMemset(&(webRepeatDial[physicalID].webRepeatDialDigitNumber.dial_digits[0]), 0, MAX_DIAL_STRING_LENGTH);
	webRepeatDial[physicalID].webRepeatDialDigitNumber.current_length= 0;
	webRepeatDial[physicalID].webRepeatDialPhoneType=0;
	webRepeatDial[physicalID].webRepeatDialPhonePhyID=0;

       /*copy dial info.*/
	zyMemcpy(&(webRepeatDial[physicalID].webRepeatDialDigitNumber.dial_digits[0]), &(DialNumber_p[0]), min(zyStrlen(DialNumber_p), POTS_PORT_NAME_LEN) );
	webRepeatDial[physicalID].webRepeatDialDigitNumber.current_length= zyStrlen(DialNumber_p);
	webRepeatDial[physicalID].webRepeatDialPhoneType=WebDialPhoneType;
	webRepeatDial[physicalID].webRepeatDialPhonePhyID=physicalID;
	
	PH_FSM_PRINTF(" [%d]webRepeatDialPhonePhyID = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialPhonePhyID);	
	PH_FSM_PRINTF(" [%d]webRepeatDialPhoneType = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialPhoneType);	
	PH_FSM_PRINTF(" [%d]dialDigitNumber =%s\n\r", physicalID, webRepeatDial[physicalID].webRepeatDialDigitNumber.dial_digits); //Doris Debug
	PH_FSM_PRINTF(" [%d]dialDigitNumberLength =%d\n\r", physicalID, webRepeatDial[physicalID].webRepeatDialDigitNumber.current_length); //Aircheng Debug
	PH_FSM_PRINTF(" [%d]webRepeatDialFlags = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialFlags);	
	PH_FSM_PRINTF(" [%d]webRepeatDialInterval = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialInterval);	
	PH_FSM_PRINTF(" [%d]webRepeatDialTimes = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialTimes);	
	PH_FSM_PRINTF(" [%d]webRepeatDialCurrentTimes = %d\n ", physicalID, webRepeatDial[physicalID].webRepeatDialCurrentTimes);	
	
	//dbg_pline_1("\webRepeatDial.webRepeatDialDigitNumber :");
	//dbg_pline_1(webRepeatDial.webRepeatDialDigitNumber.dial_digits);
	//dbg_pline_1("\r\n");
	
//	ZyPrintf("WebDialPhoneType=%d\n\r", webRepeatDial.webRepeatDialPhoneType); //Doris Debug
//	ZyPrintf("physicalID=%d\n\r", webRepeatDial.webRepeatDialPhonePhyID); //Doris Debug

	if ( (newCcmConn_p = create_ZyPhone(NULL, 0) ) != NULL ) {
		_setWebDialStates_Start;
		_copyWebDialLog("Dialing");
		ZyPhoneCB = find_phoneCB(newCcmConn_p->origPortId, 0);
		//copy dial info to ZyPhone
		zyMemcpy(&(ZyPhoneCB->dialInform.dialDigitNumber), &(webRepeatDial[physicalID].webRepeatDialDigitNumber), sizeof(webRepeatDial[physicalID].webRepeatDialDigitNumber)); 
		//DBG_pline_1("\r\ndialed number:");//wcli
		PH_FSM_PRINTF("ZyPhoneCB->dialInform.dialDigitNumber.dial_digits = %s\n", ZyPhoneCB->dialInform.dialDigitNumber.dial_digits);//wcli
		//DBG_pline_1("\r\n");//wcli	

		zyMemset(&(newCcmConn_p->dialInform.dialDigitNumber.dial_digits), 0, MAX_DIAL_STRING_LENGTH);
		zyMemcpy(&(newCcmConn_p->dialInform.dialDigitNumber), &(webRepeatDial[physicalID].webRepeatDialDigitNumber), sizeof(webRepeatDial[physicalID].webRepeatDialDigitNumber));
		createWebDialCB(newCcmConn_p,webRepeatDial[physicalID].webRepeatDialPhonePhyID);
		newCcmConn_p->WebDialCB->webdialStatus = WEB_DIAL_ST_1stZYPHONE_CREATE;
		newCcmConn_p->flags |= CALL_CONN_FLAG_WEBDIAL;
		WebDialFsmProc(newCcmConn_p,EVT_CCM_WEBDIAL_CHECK, NULL, NULL);
	}else{
		ZyPrintf("create_ZyPhone 1 error\n\r"); 
	}
					
	return 0;
	
}/* WebPhoneDialint */

void  WebPhoneDialStop(int physicalID){
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	if((physicalID >= NumOfPhyIntf)){
		ZyPrintf("%s fail, physicalId out of range\n", __FUNCTION__);
		return;
	}
	_copyWebDialLog("\r\nStop repeat dial!");
	/* Reset the webRepeatDial initial value */
	webRepeatDial[physicalID].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
	webRepeatDial[physicalID].webRepeatDialInterval=0;
	webRepeatDial[physicalID].webRepeatDialPhonePhyID=0;
	webRepeatDial[physicalID].webRepeatDialPhoneType=0;
	webRepeatDial[physicalID].webRepeatDialTimes=0;
	webRepeatDial[physicalID].webRepeatDialCurrentTimes=0;
	memset(&(webRepeatDial[physicalID].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));

}




