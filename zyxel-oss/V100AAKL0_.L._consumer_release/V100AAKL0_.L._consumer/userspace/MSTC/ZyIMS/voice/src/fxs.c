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

/*Jason , syslog*/
#ifdef VOIP_SYSLOG
#include "syslog.h"
#include "voip_syslog.h"
#include <sys/time.h>
#endif
#if CCBS_SUPPORT
#include "webDial_fsm.h"
#endif
#include "config_sys.h"

/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/
int fxsFsmIdle(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxsFsmDial(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxsFsmRing(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxsFsmData(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxsFsmDrop(evtMsg_t *msg, phoneCh_t *phoneCB);
int fxsFsmDisc(evtMsg_t *msg, phoneCh_t *phoneCB);
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
int fxsFsmTest(evtMsg_t *msg, phoneCh_t *phoneCB);
static int fxsDTMFTimeout(tm_id_t timer_id, void *arg);
static int fxsONHOOKTimeout(tm_id_t timer_id, void *arg);
#endif

/*Jason , syslog*/
#ifdef VOIP_SYSLOG
int fxsPhoneEventLog(evtMsg_t *msg, phoneCh_t *phoneCB);
#endif
static int fxsMWIStutterToneTimeout (tm_id_t timer_id, void *arg);
static int firstDigitTimeout (tm_id_t timer_id, void *arg);
#ifdef HOWLER_TONE
static int howlerToneTimeout(tm_id_t timer_id, void	*arg);
static int howlerToneRaiseVolumeTimeout(tm_id_t timer_id, void	*arg);
#endif
static int dialDigitsTimeout(tm_id_t timer_id, void *arg);
static int checkCallWaitingTimeout(tm_id_t timer_id, void *arg);
static int connConfTimeout(tm_id_t timer_id, void *arg);
static int featureKeyTimeout(tm_id_t timer_id, void *arg);
static int getNewVoiceMessage(uint16 chId);
static int dialDigitsAdj(phoneCh_t *phoneCB, dialDigitBuf_t *dialDigitBuf);
int checkPoundKey(phoneCh_t* phoneCB,dialDigitBuf_t *dialDigitBuf,uint8 *length);
#ifdef SPEED_DIAL
int SpeedDialCheck(uint8   *input, uint8   *length);
#endif
void processFeatureKey(phoneCh_t* phoneCB, dialDigitBuf_t *dialDigitBuf);
static void processFlashKey(phoneCh_t* phoneCB);
static uint8 getCallWaitingInterval(phoneCh_t* phoneCB);
static int callerIDDTASStartTimer(tm_id_t, void*);
static int callerIDoffHookStartTimer(tm_id_t,void *);	/* For OffHook CID */
static void stopCallerID(phoneCh_t*);
static int vDspDenmarkSendOffHookCID(phoneCh_t *phoneCB);
static uint16 checkNextWaitingCallCCM(uint16 chId);
#ifdef TERMINAL_PORTABILITY
static int TerminalPortabilityTimeout (tm_id_t timer_id, void	*arg);
#endif

#ifdef NORWAY_VOIP_CUSTOMIZATION
static boolean isTelenorNorwayCongestionToneCase(uint16 rspcode);
#endif

#ifdef VISUAL_MWI
static int generateVMWI(uint16 chId,boolean enable);
#endif
uint8 setNewVoiceMessage(int value, boolean flag);
//extern int rtp_send(uint16 chId, uint32 pload_type, uint8 *packet, uint16 len, uint32 timestamp, void *handle);

/*_____ global variables _______________________________________________*/
uint8 voiceMessageFlag = 0;
extern int threeWayCallState = 0;

#ifdef CALL_RETURN_SUPPORT
CallHistoryLastCall_t *callHistoryLastCall;
#endif
#ifdef TERMINAL_PORTABILITY
extern int isTerminalPortabilitySupport;
#endif


/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/
/*______________________________________________________________________________
**	fxsFsmIdle
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
fxsFsmIdle(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	dialDigitBuf_t *dialDigitBuf = NULL;
	uint8 dialFinished = 0, callWaitingInterval = 0;			/*0:not finished, 1 finished*/
#if 0
	VoiceProfNumPlanObj_t* pProfNumPlan = tr104VoiceProfNumPlanObjPtrGet(phoneCB->associatedVpItr);
#else	/* DSL-2492HNU-B1B, [SPR 110406341] */
	VoiceProfObj_t *pVoiceProf = getAssociatedProfileObj(phoneCB);
	VoiceProfNumPlanObj_t* pProfNumPlan = pVoiceProf->numberingPlanObj;
#endif
	VoicePhyIntfObj_t*	pphyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = NULL;	/* aircheng add */
	bool autoDial = FALSE;	
	int dspChanResult = -1;
	phoneCh_t *tmpPhoneCh_p = NULL;
	int dialingLongInterval = 0;
	

	PH_FSM_DBG2("fxsFsmIdle - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);
	PH_FSM_DBG1("fxsFsmIdle - dspPortType = ",phoneCB->dspPortType);

	if ( ( phoneCB->linkSt == PHONE_LINK_ST_IDLE ) || ( phoneCB->linkSt == PHONE_LINK_ST_DIGIT ) ){
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if ( ccmConn == NULL ) {
			PH_FSM_DBG("fxsFsmIdle - ccmConn could not found");
			status = ERROR_CCM_CONN_FIND;
			goto fail;
		}
		
		switch( phoneEvent ){
			case EVT_PHONE_FXS_OFFHOOK:				
#ifdef VISUAL_MWI
				voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 0;	/* only change status if receiving notify during call */
#endif							
				if (vDspChanOpenRecvonly(phoneCB) != 0)  {
					printf("There is no dsp channel to make this call.\n");
					phStartCongestTone(phoneCB);
				} else if ( !( ccmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ){
					ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();
					/* 1. Assign the CALL_CONN_FLAG_PREASSIGN */
					/* 2. Check whether VoIP and PSTN is registered */
					/* 3. Start the dial tone. And start the firstDigitTimer */
					ccmConn->flags |= CALL_CONN_FLAG_PREASSIGN;					
					zyMemset(&(phoneCB->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t));
					if ((check_associateCCMRegSt(ccmConn) == 0) && !(pCommon->callFallBack & SPT_FALLBACK_FORCE_DIALTONE)) {
						if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
							phStartCongestTone(phoneCB);
						}
					} else {
						if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
							#if 1 /*Eason, get registered line obj then enabled line obj*/
							VoiceProfLineObj_t *pLine = getAssociatedRegLineObj(phoneCB);
							#else
							VoiceProfLineObj_t *pLine = getAssociatedLineObj(phoneCB);
							#endif
							if(pLine!=NULL){
								/* Check for Hotline */
								if((NULL != pLine->callingFeatures) && (pLine->callingFeatures->ZyXEL_hotLineEnable) && (zyStrlen(pLine->callingFeatures->ZyXEL_hotLineNum) > 0)){
									PH_FSM_PRINTF("hotline enabled number %s\n", pLine->callingFeatures->ZyXEL_hotLineNum);
									autoDial = TRUE;
									zyStrncpy(phoneCB->dialInform.dialDigitNumber.dial_digits, pLine->callingFeatures->ZyXEL_hotLineNum, MAX_DIAL_STRING_LENGTH);
									phoneCB->dialInform.dialDigitNumber.current_length = zyStrlen(phoneCB->dialInform.dialDigitNumber.dial_digits);
									dialingLongInterval = 0;
								}	
								else if((NULL != pLine->callingFeatures) && (pLine->callingFeatures->ZyXEL_warmLineEnable) && (zyStrlen(pLine->callingFeatures->ZyXEL_warmLineNum) > 0)){
									PH_FSM_PRINTF("hotline enabled number %s\n", pLine->callingFeatures->ZyXEL_warmLineNum);
									/* Check for Warmline */
									autoDial = TRUE;									
									dialingLongInterval = pLine->callingFeatures->ZyXEL_timerFirstDigitWarmLine/1000;
									PH_FSM_PRINTF("WarmLine timeout %d sec\n", dialingLongInterval);
								}
								else{
									dialingLongInterval = pProfNumPlan->ZyXEL_dialingLongInterval;
								}

								PH_FSM_PRINTF("dialingLongInterval = %d\n", dialingLongInterval);
							}							
							if(autoDial && (dialingLongInterval == 0)){								
								if(zyStrlen(phoneCB->dialInform.dialDigitNumber.dial_digits)>0){
								phoneCB->linkSt = PHONE_LINK_ST_DIGIT;
								eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, MSG_SEND_TO_PHONE, phoneCB->logicalId);
								}
							}else{
								phStopTone(phoneCB);
								phStopRing(phoneCB);								
								phEnableDtmfDetect(phoneCB);
								if (getNewVoiceMessage(phoneCB->physicalId)){		/* Check if the voice message is existed */
									phStartMWITone(phoneCB);
									if ( phoneCB->callProTimer.fxsMWIStutterToneTimer== 0 ) {
										phoneCB->callProTimer.fxsMWIStutterToneTimer = timerStartS(MWI_STUTTER_TONE_TIME, fxsMWIStutterToneTimeout, phoneCB);
									}
								}else{
                         			#ifdef DO_NOT_DISTURB
                         			if(NULL!= pLine && pLine->callingFeatures->doNotDisturbEnable){
										phStartSpecialDialTone(phoneCB);
									}else{
										phStartDialTone(phoneCB);
									}
									#else
									phStartDialTone(phoneCB);
									#endif
									if ( phoneCB->callProTimer.firstDigitTimer == 0 ) {
										phoneCB->callProTimer.firstDigitTimer = timerStartS(1000*dialingLongInterval, firstDigitTimeout, phoneCB);
									}
								}
							}
							#ifdef HOWLER_TONE							
							phoneCB->callProTimer.fxsOffhookHowlerToneTimer = timerStartS(1000*(pphyInterface->ZyXEL_HowlerToneTimeout), howlerToneTimeout, phoneCB);
							#endif
						}
					}

				} else {
					PH_FSM_DBG("Detect FXS_OFFHOOK again");
				}		
				break;
			case EVT_PHONE_FXS_DIGITS:
				if ( ( ccmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) {
					/* 1. Stop all dial digit timers. And stop the dial tone */
					/* 2. Adjust the dial digits. And check the remove Pound. */
					/* 3. Change the phoneCB->linkSt */
					timerStop(phoneCB->callProTimer.firstDigitTimer);
					timerStop(phoneCB->callProTimer.dialDigitTimer);
					timerStop(phoneCB->callProTimer.fxsMWIStutterToneTimer);						
					phoneCB->callProTimer.firstDigitTimer = 0;
					phoneCB->callProTimer.dialDigitTimer = 0;
					phoneCB->callProTimer.fxsMWIStutterToneTimer = 0;					
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						phStopTone(phoneCB);
					}
					dialDigitBuf = (dialDigitBuf_t *)msg->data_p; 
					dialFinished = dialDigitsAdj(phoneCB, dialDigitBuf);
					PH_FSM_DBG1("fxsFsmIdle - dialFinished = ",dialFinished);
					phoneCB->linkSt = PHONE_LINK_ST_DIGIT;
					switch(dialFinished){
						/*dial finish, send EVT_PHONE_DIAL_REQ*/
						case 1:
							PH_FSM_DBG(" Dial finished, send Dial Request immediatly ");
						eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, MSG_SEND_TO_PHONE, phoneCB->logicalId);						
							break;
						case 2:
							/* match NumberingPlan minNumber , change the interDigitTimer */
							if(pProfNumPlan->interDigitTimerOpen>0){
								phoneCB->callProTimer.dialDigitTimer = timerStartS(pProfNumPlan->interDigitTimerOpen, dialDigitsTimeout, phoneCB);													
							} else {						
								phoneCB->callProTimer.dialDigitTimer = timerStartS(pProfNumPlan->interDigitTimerStd, dialDigitsTimeout, phoneCB);																		
							}
							break;
						#if 1 /*Eason, match inactive phonebook speed number, do not send invite with speed number, reject call with congestion tone*/
						case 3: 
							PH_FSM_DBG(" Dial Inactive Speed Dial Number, Reject call, Congetion tone");
							phStartCongestTone(phoneCB);
							break;
						#endif			
#ifdef NORWAY_VOIP_CUSTOMIZATION
						case 4:
							PH_FSM_DBG(" Run OpenDialPlanTimer = 5s");
							phoneCB->callProTimer.dialDigitTimer = timerStartS(5*1000, dialDigitsTimeout, phoneCB);
							break;
						/*there were still digit waiting to dial, the previous digit partial match the close dial plan. Use 20 sec as inter-digit-timer*/
						case 5:
							PH_FSM_DBG("Run CloseDialPlanTimer = 20s");
							phoneCB->callProTimer.dialDigitTimer = timerStartS(20*1000, dialDigitsTimeout, phoneCB);
							break;
#endif						
							/*there were still digit waiting to dial, the previous digit partial match the open dial plan. Use 5 sec as inter-digit-timer*/
						default:
							PH_FSM_DBG1("fxsFsmIdle: interDigitTimerStd = ",pProfNumPlan->interDigitTimerStd);
							phoneCB->callProTimer.dialDigitTimer = timerStartS(pProfNumPlan->interDigitTimerStd, dialDigitsTimeout, phoneCB);
							break;
					}
				} else {
					PH_FSM_DBG("FXS_OFFHOOK is not detected previously");
				}
				break;
			case EVT_PHONE_DIAL_REQ:
				if ( ccmConn->flags & CALL_CONN_FLAG_PREASSIGN ){
					/* 1. Send EVT_CCM_ASSIGN_REQ with dialDigitNumber */
					/* 2. Change phoneCB->linkSt and phoneCB->phStatus */
					phDisableDtmfDetect(phoneCB);					
					phoneCB->linkSt = PHONE_LINK_ST_CALLING;
					phoneCB->phStatus = PHONE_STATUS_DIAL;
					#if CCBS_SUPPORT
					if(webRepeatDial[phoneCB->physicalId].webRepeatDialFlags & WEB_REPEAT_DIAL){
						/*CCBS activate to repeat dial to B, and A dial B manually again, stop CCBS*/
						if(zyStrcmp(webRepeatDial[phoneCB->physicalId].webRepeatDialDigitNumber.dial_digits, phoneCB->dialInform.dialDigitNumber.dial_digits) == 0){
							PH_FSM_PRINTF("CCBS, Manually Redial to B again, stop CCBS redial\n");
							WebPhoneDialStop(phoneCB->physicalId);
						}
					}
					eventVoiceCCMDataSend(EVT_CCM_ASSIGN_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));					
					#else
                    eventVoiceCCMDataSend(EVT_CCM_ASSIGN_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));
					#endif
				} else {
					PH_FSM_DBG("FXS_OFFHOOK is not detected previously");
				}
				break;
			case EVT_PHONE_FXS_ONHOOK:
				ccmConn->flags &= ~CALL_CONN_FLAG_PREASSIGN;
				reset_PhoneCB(phoneCB);
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phDisableDtmfDetect(phoneCB);
				}
#ifdef VISUAL_MWI
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 1) {
						if ( (SP_VSIGNAL_PTR(phoneCB->physicalId))->vmwiAbbrTone == 1 ) {
							phStartRingingPulseAlertingSignal(phoneCB); /* Ring splash */
							//pause(100);
							usleep(100*1000);
						}
						generateVMWI(phoneCB->physicalId, TRUE);
					}
					else if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 2) {
						generateVMWI(phoneCB->physicalId, FALSE);
					}
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 0;
				}
#endif				
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_PHONE_FLASH:
				/* Check if there are other phoneCB existed.*/
				/* If yes, onHook this phoneCB and change the physical to other existed phoneCB */
				if ( checkLogicPhoneNum(phoneCB->physicalId) >= 2 ) {
					phStopTone(phoneCB); /* add by Jacky 2008/11/14 */
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				break;
			case EVT_CCM_DIAL_IND:
				/* 1. Play the Ringing or the call-waiting tone */
				/* 2. Show the Before Ringing Caller-ID or Call-Waiting Caller-ID. Keyword: "Support Caller-ID" */
				/* 3. Change the phoneCB->linkSt and phoneCB->phoneSt */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){					
					
					/* 2006/11/06 Modify by Ting-Yi : reduce the time of UA send 180 ring to caller*/
					eventVoiceCCMSimpleSend(EVT_CCM_ALERT_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
				#ifdef CALL_RETURN_SUPPORT
				
					if (strlen(ccmConn->peerCcmConn->dialInform.dialDigitNumber.dial_digits) != 0) {
						if (ccmConn->peerCcmConn->origPortType == SIP_PORT) {
							memset(callHistoryLastCall[phoneCB->physicalId].callHistoryLastCallDigit, 0, MAX_DIAL_STRING_LENGTH);
							memcpy(callHistoryLastCall[phoneCB->physicalId].callHistoryLastCallDigit, &(ccmConn->peerCcmConn->dialInform.dialDigitNumber.dial_digits), strlen(ccmConn->peerCcmConn->dialInform.dialDigitNumber.dial_digits));
							callHistoryLastCall[phoneCB->physicalId].current_length = strlen(callHistoryLastCall[phoneCB->physicalId].callHistoryLastCallDigit);
						}
					}
				#endif
					if(phoneCB->associatedSipAccountId>=0){
						int LineItr = 0;
						LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
						PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
						PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, associatedVpItr = %d\n", phoneCB->associatedVpItr);
						PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, LineItr = %d\n", LineItr);
						pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(phoneCB->associatedVpItr, LineItr);	/* aircheng modify */
					}
					else{
						VoiceProfLineObj_t *lineObj = NULL;
						#if 1 /*Eason, get registered line obj then enabled line obj*/
						lineObj = getAssociatedRegLineObj(phoneCB);
						#else
						lineObj = getAssociatedLineObj(phoneCB);
						#endif
						if(NULL != lineObj ){
							pLineCallingFeatures = lineObj->callingFeatures;	/* aircheng modify */
						}
					}
					
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
							if((phoneCB->dspPortType == DSP_FXS_PORT)|| (phoneCB->dspPortType==DSP_DECT_PORT)){
								PH_FSM_DBG1("Prior Ring chId=", phoneCB->physicalId);
								if(ccmConn->peerCcmConn != NULL) {
									phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
								}
								if(phoneCB->dspPortType==DSP_FXS_PORT){											
									if(pLineCallingFeatures != NULL) {		/* aircheng modify */										
										if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE) {
											dspChanResult = vDspChanOpen(phoneCB);
										} else {
											dspChanResult = vDspChanOpenRecvonly(phoneCB);
										}									
									} else {
										dspChanResult = vDspChanOpenRecvonly(phoneCB);
									}									
									if (dspChanResult != 0) {
										printf("There is no dsp channel to receive this call.\n");
										eventVoiceSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_PHONE, phoneCB->logicalId);
										break;
									}
								}

								if(ccmConn->peerCcmConn != NULL) {
									if(ccmConn->peerCcmConn->dialInform.dialDigitNumber.current_length > 0) {
										zyMemcpy(&(phoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );								
										phSendCID(phoneCB);
									} else {
										zyMemcpy(&(phoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
										phoneCB->dialInform.dialDigitNumber.current_length = 0;
										phSendCID(phoneCB);
									}
								}
							}
					} else {
						#ifdef TERMINAL_PORTABILITY
						if(isTerminalPortabilitySupport){
	                        if((tmpPhoneCh_p = find_phoneCB(phoneCB->physicalId, 1)) != NULL){/* find Physical_Usage phone channel*/
	                            if(tmpPhoneCh_p->phoneFlags & PHONE_TERMINAL_PORTABILITY){
	                                /* do nothing here if Terminal Portability is triggerred */
	                            }else{
	                                phStartCptTone(phoneCB, LINE_EVT_PHONE_CALL_WAITING_TONE);
#if 0 /* SPR 120607479, DUT is crash when have call waiting */
	                                //if ( ( callWaitingInterval = getCallWaitingInterval(phoneCB->physicalId) ) > 0 ) {
#else
	                                if ( ( callWaitingInterval = getCallWaitingInterval(phoneCB) ) > 0 ) {	
#endif
	                                    phoneCB->callProTimer.fxsCallWaittingRejectTimer = timerStartS(1000*callWaitingInterval, checkCallWaitingTimeout, phoneCB);
	                                }
#if 1 /* SPR 120607479, DUT is crash when have call waiting */
									
									phoneCB->linkSt = PHONE_LINK_ST_INCOMING;
									phoneCB->phStatus = PHONE_STATUS_RING;
#endif
	                            }
	                        }
							goto fail;
						}
						#endif
						
						if(phoneCB->dspPortType==DSP_FXS_PORT){							
							if(pLineCallingFeatures != NULL) {		/* aircheng modify */
								if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE) {
									dspChanResult = vDspChanOpen(phoneCB);
								} else {
									dspChanResult = vDspChanOpenRecvonly(phoneCB);
								}
							} else {
								dspChanResult = vDspChanOpenRecvonly(phoneCB);
							}
							if (dspChanResult != 0) 
							{
								printf("There is no dsp channel to receive this call.\n");
								eventVoiceSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_PHONE, phoneCB->logicalId);
								break;
							}
							
							if(ccmConn->peerCcmConn->dialInform.dialDigitNumber.current_length > 0) {   //	Erin
								zyMemcpy(&(phoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
								#if 1 /*Eason, Support Call Waiting Caller ID via BRCM classstm */
								timerStartS(1300, callerIDDTASStartTimer, (void*)phoneCB);
						   		phStartCptTone(phoneCB, LINE_EVT_PHONE_CALL_WAITING_TONE);
								#endif
								timerStartS(1450, &callerIDoffHookStartTimer, (void*)phoneCB);				 
							} 
							
						}
						
						if ( ( callWaitingInterval = getCallWaitingInterval(phoneCB) ) > 0 ) {	/* Start the No-Answer Reject Timer */
							phoneCB->callProTimer.fxsCallWaittingRejectTimer = timerStartS(1000*callWaitingInterval, checkCallWaitingTimeout, phoneCB);
						}	
					}
					/* 2006/11/06 Modify by Ting-Yi : reduce the time of UA send 180 ring to caller*/
					//eventVoiceCCMSimpleSend(EVT_CCM_ALERT_REQ, callMgnCb.sig.func_id, phoneCB->logicalId, phoneCB->dspPortType);
					phoneCB->linkSt = PHONE_LINK_ST_INCOMING;
					phoneCB->phStatus = PHONE_STATUS_RING;
					voipLedBlinking(phoneCB->physicalId);
				} else {
					PH_FSM_DBG("ccmConn is not bundled");
				}
				break;
			case EVT_CCM_REJ_REQ:
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				reset_PhoneCB(phoneCB);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceCCMSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				break;
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif	
			case EVT_PHONE_REMOVE:
#ifdef HOWLER_TONE
				vDspStopHowlerTone(phoneCB);
#endif
				vDspChanClose(phoneCB);
				#if 1 /*Jason, P2812HNU-51c_OBM , [SPRID # 090622466]*/	
				PH_FSM_PRINTF("\n phoneCB->phoneFlags = %x\n",phoneCB->phoneFlags);				
				if(phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLD_RING) {
					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;
				}
				if(phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLDING){
					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLDING;
				}
				#endif
#ifdef CCBS_SUPPORT		/* aircheng add, [SPRID # 100504121] */
				if(phoneCB->phoneFlags & PHONE_CALLEE_EARLY_MEDIA){
					phoneCB->phoneFlags &= ~PHONE_CALLEE_EARLY_MEDIA;
				}
#endif
				delLogicPhone(phoneCB, ccmConn);				
				break;
			default:
				PH_FSM_DBG1("fxsFsmIdle-unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
}


/*______________________________________________________________________________
**	fxsFsmDial
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
fxsFsmDial(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	uint16 peerPortType;
	ccmConn_t *ccmConn = NULL;	

#ifdef NORWAY_VOIP_CUSTOMIZATION
	uint16 rspcode = 0;
#endif	
	
	PH_FSM_DBG2("fxsFsmDial - phoneEvent = ", phoneEvent, ", logicId=", phoneCB->logicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxsFsmDial - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( ( phoneCB->linkSt == PHONE_LINK_ST_CALLING ) || ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) ){
		switch( phoneEvent ){
			case EVT_CCM_ALERT_IND:
				/* 1. Play the RingBack Tone */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						#ifdef HOWLER_TONE
						timerStop(phoneCB->callProTimer.fxsOffhookHowlerToneTimer);
						phoneCB->callProTimer.fxsOffhookHowlerToneTimer = 0;
						timerPeriodicStop(phoneCB->callProTimer.fxsHowlerTonePeriodicTimer);
						phoneCB->callProTimer.fxsHowlerTonePeriodicTimer = 0;
						#endif						
						phStopTone(phoneCB);
						phStartRingBackTone(phoneCB);
						phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					}
				} else {
					PH_FSM_DBG("Not assign before alert");
				}
				break;
			case EVT_CCM_ASSIGN_CONF:
				/* 1. If the peerPort is FXO_LifeLine, change to CONNECT status directly */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					peerPortType = *((uint16 *)msg->data_p);
					if ( peerPortType == DSP_FXO_LIFELINE ) {	/* FXO_LIFELINE */
						zyMemset(&(phoneCB->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t));
					 	phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
						phoneCB->phStatus = PHONE_STATUS_DATA;
					}
				}				
				break;
			case EVT_CCM_REJ_IND:
			case EVT_CCM_REL_IND:
			case EVT_CCM_DISC_IND:
			case EVT_CCM_ASSIGN_FAIL:
			case EVT_CCM_DISC_REQ:				
				/* 1. Start Congestion Tone */
				/* 2. Change phoneCB->phStatus to DISC */
#ifdef NORWAY_VOIP_CUSTOMIZATION
				if(msg->data_p != NULL && msg->data_size != 0){
					memcpy(&rspcode,msg->data_p,sizeof(uint16));
				}	
#endif
				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phStopTone(phoneCB);
#ifdef NORWAY_VOIP_CUSTOMIZATION
					if ( ( phoneEvent == EVT_CCM_ASSIGN_FAIL ) || ( phoneEvent == EVT_CCM_REL_IND ) ){
						phStartCongestTone(phoneCB);
					} else if( isTelenorNorwayCongestionToneCase(rspcode) ){
						phStartCongestTone(phoneCB);
					} else {
						phStartBusyTone(phoneCB);
					}
#else
					if ( ( phoneEvent == EVT_CCM_ASSIGN_FAIL ) || ( phoneEvent == EVT_CCM_REL_IND ) ){
						if(NULL != ccmConn && ccmConn->callFeatures == CALL_FEATURE_CALL_TRANSFER){
							ccmConn->callFeatures = 0;
							phStartBusyTone(phoneCB);
						}
						else{
							phStartCongestTone(phoneCB);
						}
					} else {
						phStartBusyTone(phoneCB);
					}
#endif
				}				
				ccmConn->flags &= ~(CALL_CONN_FLAG_PREASSIGN|CALL_CONN_FLAG_ASSIGN|CALL_CONN_FLAG_PREFIX);
				phoneCB->linkSt = PHONE_LINK_ST_WAITING;
				phoneCB->phStatus = PHONE_STATUS_DISC;
				eventVoiceSimpleSend(EVT_PHONE_ST_CHECK, MSG_SEND_TO_PHONE, phoneCB->logicalId);				
				#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
				if(phoneCB->phoneFlags & PHONE_ATTENDED_TRANSFER){
					phoneCB->phoneFlags &= ~PHONE_ATTENDED_TRANSFER;
					PH_FSM_PRINTF("fxsFsmDial take off PHONE_ATTENDED_TRANSFER flag, send EVT_PHONE_FXS_ONHOOK\n");
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				#endif
				break;				
			case EVT_PHONE_FXS_ONHOOK:	
				/* 1. Stop all tones */
				/* 2. Reset phone values */
				/* 3. Send event to CCM and Phone */
				/* 4. Change phoneCB->linkSt and phoneCB->phStatus */				
				#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
				if ( phoneCB->phoneFlags & PHONE_ATTENDED_TRANSFER) {
					PH_FSM_PRINTF("send EVT_CCM_ATTEND_CALL_TRANS_REQ\n");
					eventVoiceCCMDataSend(EVT_CCM_ATTEND_CALL_TRANS_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));					
					break;					
				}
				#endif
				#if CCBS_SUPPORT
				if(ccmConn->flags & CALL_CONN_FLAG_WEBDIAL){
					_setWebDialStates_End;
					_copyWebDialLog("\r\nBye");
				}
				#endif
				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phStopTone(phoneCB);
					phDisableDtmfDetect(phoneCB);					
				}
				/* brianlin@20060929: SPR 060322743, close dsp channel while fxs on-hook */
				vDspChanOpen(phoneCB); /*reopen dsp Channel, in ZyNOS vDspChanClose(phoneCB, PASSING);*/
				reset_PhoneCB(phoneCB);
#ifdef VISUAL_MWI
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 1) {
						if ( (SP_VSIGNAL_PTR(phoneCB->physicalId))->vmwiAbbrTone == 1 ) {
							phStartRingingPulseAlertingSignal(phoneCB); /* Ring splash */
							//pause(100);
							usleep(100*1000);
						}
						generateVMWI(phoneCB->physicalId, TRUE);
					}
					else if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 2) {
						generateVMWI(phoneCB->physicalId, FALSE);
					}
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 0;
				}
#endif
				eventVoiceCCMSimpleSend(EVT_CCM_REL_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif
			case EVT_CCM_CONN_IND:
				/* 1. Set peerMediaChId */
				/* 2. Stop all tones and open DSP Channel */
				/* 3. Clear the dial digits */
				/* 4. Change the phStatus and linkSt */
				if ( ccmConn->peerCcmConn->origPortType == SIP_PORT ) {
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						phPolarityReverse(phoneCB);
						phStopTone(phoneCB);
						vDspChanOpen(phoneCB);
					}
				} else if (( ccmConn->peerCcmConn->origPortType == DSP_FXS_PORT )|| 
 					       ( ccmConn->peerCcmConn->origPortType == DSP_DECT_PORT )||
				           ( ccmConn->peerCcmConn->origPortType == DSP_FXO_PORT )) {
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						if(ccmConn->peerCcmConn->origPortType != DSP_FXO_PORT ){
							/*fxo is dialing, stop tone will interrupt pstn dial*/
							phStopTone(phoneCB);
						}
						vDspChanOpen(phoneCB);
					}
					vDspCreateInternalCall(phoneCB);

				} else if ( ccmConn->peerCcmConn->origPortType == ZYXEL_PHONE_PORT ){
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						//phStopTone(phoneCB); /*Phone Config enable/disable feature play confirm tone*/

						vDspChanOpen(phoneCB);
					}
					//hook dspCallBack to rtpRecord in MM
					vDspIVRFunc(phoneCB, 0, 0, 0, 0);
				}				
#if CCBS_SUPPORT				
				if (( ccmConn->peerCcmConn->origPortType == SIP_PORT )||( ccmConn->peerCcmConn->origPortType == ISDN_TE_PORT )
									||( ccmConn->peerCcmConn->origPortType== ISDN_NT_PORT )||( ccmConn->peerCcmConn->origPortType== DSP_FXS_PORT ))
				{

					 if(ccmConn->flags & CALL_CONN_FLAG_WEBDIAL) {
						if( slicHookStatus(phoneCB) == 0 )	 {	/* 1=OffHook,  0=OnHook*/	
							
							VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = getAssociatedLineFeatures(phoneCB);
							if(NULL != pLineCallingFeatures){
								PH_FSM_PRINTF("CCBS, START early media\n");
								vDspStartEarlyMedia(phoneCB->callId, pLineCallingFeatures->ZyXEL_remoteRingbackToneIndex);
							}
							
							_copyWebDialLog("\r\nPeer Phone pick up");
						}
						else{
							_copyWebDialLog("\r\nConnecting...");
						}
					}
				}
#endif
				zyMemset(&(phoneCB->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t));
				voipSetLed(phoneCB->physicalId, LED_REG, VOIP_LED_STATE_FAST_BLINK);
				phoneCB->phStatus = PHONE_STATUS_DATA;
				phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				break;
			case EVT_PHONE_FLASH:
				/* Check if there are other phoneCB existed.*/
				/* If yes, onHook this phoneCB and change the physical to other existed phoneCB */
				if ( checkLogicPhoneNum(phoneCB->physicalId) >= 2 ) {
					if(phoneCB->linkSt == PHONE_LINK_ST_CALLING){
						//eventVoiceCCMSimpleSend(EVT_CCM_REL_TRANSFER, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);	/* ccw debug: 05/02/2005 */
						eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					} else {
						processFlashKey(phoneCB);
					}
				} else {
					processFlashKey(phoneCB);
				}
				break;
			case EVT_CCM_SDP_ALERT_IND:
					if ( ccmConn->peerCcmConn->origPortType == SIP_PORT ) {
						
					phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
					if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {

						phPolarityReverse(phoneCB);
						phStopTone(phoneCB);
						vDspChanOpen(phoneCB);
						phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
					}
				} 
				break;
			case EVT_PHONE_DSP_DIGITS: /* Obsolete */
			case EVT_PHONE_FXS_DIGITS: {
					dialDigitBuf_t dialDigitBuf;
					//if(msg->data_p != NULL){
					if(zyStrlen(msg->data_p) > 0){
						zyMemcpy(&dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
						PH_FSM_DBG1("PH: FXS Digit = ", dialDigitBuf.digitStr);					    
					        processFeatureKey(phoneCB, (dialDigitBuf_t *)msg->data_p);												
					}
				}
				break;
			#if CCBS_SUPPORT
			case EVT_PHONE_FXS_OFFHOOK:
				/* 1. Play the RingBack Tone */
				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phStopRing(phoneCB);
					phStopTone(phoneCB);
					if(!(phoneCB->phoneFlags & PHONE_CALLEE_EARLY_MEDIA)){
						phStartRingBackTone(phoneCB);
					} else {
						PH_FSM_DBG("fxsFsmDial - PHONE_CALLEE_EARLY_MEDIA on");
					}
					_copyWebDialLog("\r\nWaiting for callee to pick up phone");
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				}
				break;	
			#endif
			default:
				PH_FSM_DBG1("fxsFsmDial(CALLING)-unknown event=", phoneEvent);
				break;
		}
	} 

fail:
	return status;
} /* fxsFsmDial */


/*______________________________________________________________________________
**	fxsFsmRing
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
fxsFsmRing(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	VoicePhyIntfObj_t*	pphyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxsFsmRing - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	PH_FSM_DBG2("fxsFsmRing - phoneEvent = ", phoneEvent, ", linkSt=", phoneCB->linkSt);
	PH_FSM_DBG1("             logicId=", phoneCB->logicalId);

	if ( phoneCB->linkSt == PHONE_LINK_ST_INCOMING ){
		switch( phoneEvent ){
			case EVT_CCM_REL_IND:
			case EVT_CCM_DISC_IND:
			case EVT_CCM_REJ_IND:
				/* 1. Stop callerID Start timer */
				/* 2. Stop callerID Stop timer */
				/* 3. Stop callerID ring timer */
				/* 4. Stop Ring */
				/* 5. Reset PhoneCB values */
				/* 6. Change phoneCB->linkSt and phoneCB->phStatus */
				/* 7. release DECT channel */

				if ( phoneCB->callProTimer.fxsCallerIDStartTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDStartTimer);
					phoneCB->callProTimer.fxsCallerIDStartTimer = 0;
					timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
					phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
					vDspChanOpen(phoneCB);
				}else if ( phoneCB->callProTimer.fxsCallerIDStopTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
					phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
					stopCallerID(phoneCB);
				}
				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phPolarityReverse(phoneCB);
					phStopRing(phoneCB);
					vDspChanClose(phoneCB);
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				} else {
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				reset_PhoneCB(phoneCB);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
			case EVT_CCM_REJ_REQ:
			case EVT_CCM_REL_REQ: 
				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phPolarityReverse(phoneCB);
					phStopRing(phoneCB);
					vDspChanClose(phoneCB);
				} else {
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				reset_PhoneCB(phoneCB);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceCCMSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				break;
			case EVT_CCM_UNHELD_REQ:
				/* Trigger this event when the original phoneCB is onhooked. The existed call-waiting phoneCB needs to ring */
				phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLDING;
				eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_PHONE_FXS_OFFHOOK:
				/* 1. Stop callerID Start timer */
				/* 2. Stop callerID Stop timer */
				/* 3. Stop callerID ring timer */
				/* 4. Stop all tones and ring */
				/* 5. Change phoneCB->linkSt */
				/* 6. Start connConfTimeout */
				/* 7. Led on for the Phone */
#ifdef VISUAL_MWI
				voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 0;	/* only change status if receiving notify during call */
#endif
				if(ccmConn->flags & CALL_CONN_FLAG_PEERLOCAL){
					dbg_plinel_1("\r\npeerlocal logicalId=",ccmConn->origPortId);
					recordLocalConfPeerLocalPhLogicalId(ccmConn->origPortId, &(ccmConn->peerCcmConn->dialInform.dialDigitNumber));
				}

				if ( phoneCB->callProTimer.fxsCallerIDStartTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDStartTimer);
					phoneCB->callProTimer.fxsCallerIDStartTimer = 0;
					timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
					phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
					vDspChanOpen(phoneCB);
				}else if ( phoneCB->callProTimer.fxsCallerIDStopTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
					phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
					stopCallerID(phoneCB);
				}
				if ( phoneCB->callProTimer.fxsCallerIDRingTimer != 0 ) {
					timerStop(phoneCB->callProTimer.fxsCallerIDRingTimer);
					phoneCB->callProTimer.fxsCallerIDRingTimer = 0;
				}
				if ( phoneCB->callProTimer.fxsCallerIDTACSecTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDTACSecTimer);
					phoneCB->callProTimer.fxsCallerIDTACSecTimer = 0;
				}
				if(phoneCB->callProTimer.fxsCallWaittingRepeater) {
					timerStop(phoneCB->callProTimer.fxsCallWaittingRepeater);
					phoneCB->callProTimer.fxsCallWaittingRepeater = 0;
				}
				if(phoneCB->callProTimer.fxsCallWaittingRejectTimer) {
					timerStop(phoneCB->callProTimer.fxsCallWaittingRejectTimer);
					phoneCB->callProTimer.fxsCallWaittingRejectTimer = 0;
				}
				if ( phoneCB->callProTimer.fxsCallerIDRingStopTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDRingStopTimer);
					phoneCB->callProTimer.fxsCallerIDRingStopTimer = 0;
				}
				if ( phoneCB->callProTimer.fxsCallerIDDTMFToneOnTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDDTMFToneOnTimer);
					phoneCB->callProTimer.fxsCallerIDDTMFToneOnTimer = 0;
				}
				if ( phoneCB->callProTimer.fxsCallerIDDTMFToneOffTimer != 0) {
					timerStop(phoneCB->callProTimer.fxsCallerIDDTMFToneOffTimer);
					phoneCB->callProTimer.fxsCallerIDDTMFToneOffTimer = 0;
				}
				if ( phoneCB->callProTimer.fxsToneStartTimer!= 0) {
					timerStop(phoneCB->callProTimer.fxsToneStartTimer);
					phoneCB->callProTimer.fxsToneStartTimer = 0;
				}

				if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phStopRing(phoneCB);
					phStopTone(phoneCB);
				}
				phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;

				eventVoiceCCMSimpleSend(EVT_CCM_CONN_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
				phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				phoneCB->callProTimer.connConfTimer = timerStartS(TIMEOUT_CONN_CONF, connConfTimeout, phoneCB);

				break;
			case EVT_PHONE_FXS_SENDCID:
				/* 1. Open the DSP channel */
				/* 2. Stop Ring */
				/* 3. Send During-Ring CallerID. Keyword: "Support Caller-ID" */
				/* 4. Close the DSP channel */
				/* 5. Start Ring */

				if( (pphyInterface->ZyXEL_CallerIDType== DURING_RING) &&(phoneCB->dspPortType==DSP_FXS_PORT)){
					if(ccmConn->peerCcmConn != NULL) {
						if ( (ccmConn->peerCcmConn->dialInform.dialDigitNumber.current_length > 0) && ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) ){
							zyMemcpy(&(phoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
							phSendCID(phoneCB);
						} else if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ){
							zyMemcpy(&(phoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
							phoneCB->dialInform.dialDigitNumber.current_length = 0;
							phSendCID(phoneCB);
						}
					} else {
						PH_FSM_DBG("\r\n peerCcmConn is NULL");
					}
				}
				break;
			
			default:
				PH_FSM_DBG1("fxsFsmRing(INCOMING)-unknown event=", phoneEvent);
				break;
		}
	} else if ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) {
		switch( phoneEvent ) {
			case EVT_CCM_CONN_CONF:
				/* 1. Stop connConfTimer */
				/* 2. Copy media Information from CCM to Phone */
				/* 3. Open DSP channel */
				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					if ( phoneCB->callProTimer.connConfTimer != 0 ) {
						timerStop(phoneCB->callProTimer.connConfTimer);
						phoneCB->callProTimer.connConfTimer = 0;
					}
					if ( ccmConn->peerCcmConn->origPortType == SIP_PORT ) {
						phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
						if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {

							vDspChanOpen(phoneCB);
						}
					} else if( ( ccmConn->peerCcmConn->origPortType == DSP_FXS_PORT )  ||
					           ( ccmConn->peerCcmConn->origPortType == DSP_DECT_PORT ) ||
					           ( ccmConn->peerCcmConn->origPortType == DSP_FXO_PORT )) {
						phoneCB->peerMediaChId = ccmConn->peerCcmConn->origPortId;
						if  ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						
						vDspChanOpen(phoneCB);
						
						}
						vDspCreateInternalCall(phoneCB);
					}

					voipSetLed(phoneCB->physicalId, LED_REG, VOIP_LED_STATE_FAST_BLINK);
					phoneCB->phStatus = PHONE_STATUS_DATA;
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
				}
				break;
			case EVT_CCM_DISC_IND:
			case EVT_CCM_REL_IND:
				/* 1. Stop connConfTimer */
				/* 2. Stop all tone and Ring. And close the dsp channel */
				/* 3. Change phoneCB->linkSt and phoneCB->phStatus */
				if ( phoneCB->callProTimer.connConfTimer != 0 ) {
					timerStop(phoneCB->callProTimer.connConfTimer);
					phoneCB->callProTimer.connConfTimer = 0;
				}
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phStopTone(phoneCB);
					phStartBusyTone(phoneCB);
					vDspChanOpen(phoneCB);
				} else {
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				phoneCB->linkSt = PHONE_LINK_ST_WAITING;
				phoneCB->phStatus = PHONE_STATUS_DISC;
				eventVoiceSimpleSend(EVT_PHONE_ST_CHECK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_PHONE_FXS_ONHOOK:
				/* 1. Close the DSP channel */
				/* 2. Reset the phone values */
				/* 3. Change the phone Status */

				if ( ccmConn->flags & CALL_CONN_FLAG_ASSIGN ){
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						vDspChanOpen(phoneCB); /*reopen dsp Channel, in ZyNOS vDspChanClose(phoneCB, PASSING);*/
					}
					reset_PhoneCB(phoneCB);
#ifdef VISUAL_MWI
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 1) {
						if ( (SP_VSIGNAL_PTR(phoneCB->physicalId))->vmwiAbbrTone == 1 ) {
							phStartRingingPulseAlertingSignal(phoneCB); /* Ring splash */
							//pause(100);
							usleep(100*1000);
						}
						generateVMWI(phoneCB->physicalId, TRUE);
						} else if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 2) {
						generateVMWI(phoneCB->physicalId, FALSE);
					}
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 0;
				}
#endif
					eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
				}
				break;
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif
			default:
				PH_FSM_DBG1("fxsFsmRing(CONNECT)-unknow event=", phoneEvent);
				break;
		}
	}

fail:
	return status;

}


/*______________________________________________________________________________
**	fxsFsmData
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
fxsFsmData(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {

	int status = 0, ret = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;
	dialDigitBuf_t dialDigitBuf;
#ifdef CALL_CONFERENCE
	phoneCh_t *newPhoneCB=NULL;
	uint16 *threeWayCallBaseLogicId=NULL;
#endif	

	VoiceProfObj_t *pVoiceProf = tr104VoiceProfObjPtrGet(phoneCB->associatedVpItr);

	uint8 *currentLen;

	PH_FSM_DBG2("fxsFsmData - phoneEvent = ", phoneEvent, ", phoneCB=", (uint32)phoneCB);
	PH_FSM_DBG2("                    linkSt = ", phoneCB->linkSt, ", logicId=", phoneCB->logicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxsFsmData - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ){
		switch( phoneEvent ){
			case EVT_PHONE_FXS_ONHOOK:
				PH_FSM_DBG1("phoneCB->phoneFlag =", phoneCB->phoneFlags);				
				#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
				if ( phoneCB->phoneFlags & PHONE_ATTENDED_TRANSFER) {
					PH_FSM_PRINTF("send EVT_CCM_ATTEND_CALL_TRANS_REQ and turn off flag PHONE_ATTENDED_TRANSFER\n");
					eventVoiceCCMDataSend(EVT_CCM_ATTEND_CALL_TRANS_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));					
					break;
				}
				#endif
				#ifdef TERMINAL_PORTABILITY
				PH_FSM_PRINTF("TerminalPortabilitySupport = %d\n", isTerminalPortabilitySupport);
				if(isTerminalPortabilitySupport){
	                if( !(phoneCB->phoneFlags & PHONE_TERMINAL_PORTABILITY)){
	                    if ( ccmConn->connType == CCM_CONN_TYPE_CALLEE_CALLEE && /* Only happens in callee */
	                         phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE){
	                        phoneCB->phoneFlags |= PHONE_TERMINAL_PORTABILITY; /* Triggers Terminal Portability */
	                        phoneCB->callProTimer.fxsTerminalPortabilityTimer = timerStartS(120000, TerminalPortabilityTimeout, phoneCB);
	                        goto fail;
	                    }
	                }else{
	                    phoneCB->phoneFlags &= ~PHONE_TERMINAL_PORTABILITY;
	                    if(phoneCB->callProTimer.fxsTerminalPortabilityTimer != 0){
	                        timerStop(phoneCB->callProTimer.fxsTerminalPortabilityTimer);
	                        phoneCB->callProTimer.fxsTerminalPortabilityTimer = 0;
	                    }
	                }
				}
				#endif
				
				if ( !( phoneCB->phoneFlags & PHONE_PHYSICAL_HOOK_NOT_CARE ) ) {		/* SIP */
					/* 1. Close the DSP channel */
					/* 2. Reset the phoneCB values */
					/* 3. Change Phone Status */
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						phDisableDtmfDetect(phoneCB);
						phoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
						phoneCB->LinePolarity = LINE_POLARITY_REVERSE;
						phPolarityReverse(phoneCB);	//hchsu 2005/03/23 for telefonica
						vDspChanClose(phoneCB);
					}
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
                    reset_PhoneCB(phoneCB);
					eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
#ifdef VISUAL_MWI
					if(phHookStatus(phoneCB) == 0) {	/* onhook */
						if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 1) {
							if ( (SP_VSIGNAL_PTR(phoneCB->physicalId))->vmwiAbbrTone == 1 ) {
								phStartRingingPulseAlertingSignal(phoneCB); /* Ring splash */
								//pause(100);
								usleep(100*1000);
							}
							generateVMWI(phoneCB->physicalId, TRUE);
						} else if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 2) {
							generateVMWI(phoneCB->physicalId, FALSE);
						}
						voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 0;
					}
#endif
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
				} else {							
				/* FXO_LIFELINE */

					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_HOOK_NOT_CARE;
				}
				break;
			case EVT_PHONE_FXS_OFFHOOK:
				#ifdef TERMINAL_PORTABILITY
				if(isTerminalPortabilitySupport){
					if(phoneCB->phoneFlags & PHONE_TERMINAL_PORTABILITY){
						if(phoneCB->callProTimer.fxsTerminalPortabilityTimer != 0){
							timerStop(phoneCB->callProTimer.fxsTerminalPortabilityTimer);
							phoneCB->callProTimer.fxsTerminalPortabilityTimer = 0;
						}
						phoneCB->phoneFlags &= ~PHONE_TERMINAL_PORTABILITY;
						/* 1. check the other logical phone channels for this physical phone */
						/* 2. if the logical phone in ring state, play call_waiting tone*/
						if ( voicePhoneCh.phoneChEntry != NULL ) {
							phoneCh_t *tmpPhoneCh_p;
							tmpPhoneCh_p = voicePhoneCh.phoneChEntry;
							while ( tmpPhoneCh_p != NULL ) {
								if ( phoneCB->physicalId < tmpPhoneCh_p->physicalId ) {
									break;
								} else if ( phoneCB->physicalId == tmpPhoneCh_p->physicalId ) {

									if( !(tmpPhoneCh_p->phoneFlags & PHONE_PHYSICAL_USAGE) &&
										(tmpPhoneCh_p->phStatus == PHONE_STATUS_RING) &&
										(tmpPhoneCh_p->linkSt == PHONE_LINK_ST_INCOMING) ){
										uint8 callWaitingInterval = 0;
										phStartCptTone(tmpPhoneCh_p, LINE_EVT_PHONE_CALL_WAITING_TONE);
										if ( ( callWaitingInterval = getCallWaitingInterval(tmpPhoneCh_p->physicalId) ) > 0 ) {
											tmpPhoneCh_p->callProTimer.fxsCallWaittingRejectTimer = timerStartS(1000*callWaitingInterval, checkCallWaitingTimeout, tmpPhoneCh_p);
										}
									}
								}
								if ( tmpPhoneCh_p ->next != NULL ) {
									tmpPhoneCh_p = tmpPhoneCh_p->next;
								} else {
									break;
								}
							}
						}
					}
				}
				#endif
				#ifdef CCBS_SUPPORT
				if( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE){
					PH_FSM_PRINTF("CCBS, fxs offhook, stop early media\n");
					vDspStopEarlyMedia(phoneCB->callId);

				}
				#endif

				/* The physical is in the onHook status. Use Ring to remind the user. Then the user offHook the phone */
				if ( ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) && (phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLD_RING) ) {
					phStopRing(phoneCB);
					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;
					enterLogicPhone(phoneCB, ccmConn);

				}
				break;
		#ifdef CALL_CONFERENCE
			case EVT_CCM_CONFCALL_MEETROOM_REQ: {
					//if(createMeetingRoom(phoneCB->physicalId, &phoneCB->mediaInfo,Voice_Data)==1){
					if(1){
						ccmConn->ConfCallCB->threeWayCallBaseLogicId = phoneCB->logicalId;						
						eventVoiceCCMSimpleSend(EVT_CCM_CONFCALL_MEETROOM_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
					} else {
						eventVoiceCCMSimpleSend(EVT_CCM_CONFCALL_MEETROOM_FAIL, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
					}
				}
				break;
			case EVT_CCM_3WAYCALL_REMOVE_BASEPHONE:
				ccmConn = (ccmConn_t *)msg->data_p;
				if( (newPhoneCB=find_phoneCB(ccmConn->origPortId, 0)) == NULL){
					printf("wrong logicId[%d] at fxsFsmData", ccmConn->origPortId);
					return -1;
				}
				phoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE;
				phoneCB->linkSt = PHONE_LINK_ST_ONHOLD;
				newPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
				break;
			case EVT_CCM_3WAYCALL_REMOVE_NEWPHONE:
				eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ:{
				/* Once the conference has been established between A, B and C, 
				if A puts A-C call on hold and then resumes it, the conference is also resumed between A, B and C
				IPTK expects the media to remain merged at Media Manager for calls A-B and A-C. 
				And hence expects conference ongoing and don't need to destory meeting room
				*/
				#if 1
				uint8 *tmpPeerisLocal = (uint8 *)msg->data_p;
				if(*tmpPeerisLocal){
					vDspDestroyLocalPeerMeetingRoom(phoneCB->logicalId);
				}else{
					vDspDestroyMeetingRoom(phoneCB->logicalId);
				}
				#endif
				eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &phoneCB->logicalId, sizeof(uint16) );
				}
				break;
			case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ:
				threeWayCallBaseLogicId = (uint16 *)msg->data_p;				

				vDspDestroyMeetingRoom(*threeWayCallBaseLogicId);
				eventVoiceCCMSimpleSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				break;
			case EVT_CCM_3WAYCALL_SWITCH_ACTIVE_PHONE:
				eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLDING;
				break;
		#endif				

			case EVT_CCM_DISC_IND:
				/* 1. Close the DSP channel */
				/* 2. Play Busy tone */
				/* 3. Change the phone status */
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) {
					#if 0
					phDisableDtmfDetect(phoneCB);
					phPolarityReverse(phoneCB);
					vDspChanOpen(phoneCB, Voice_Data);
					#endif
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLD_RING ) {
						phStopRing(phoneCB);
						phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;
						eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
						timerStop(phoneCB->callProTimer.fxsflashRingTimer);
					#ifdef TERMINAL_PORTABILITY
					} else if(isTerminalPortabilitySupport && phoneCB->phoneFlags & PHONE_TERMINAL_PORTABILITY){
						/* don't play busy tone */
						eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
						break;
					#endif

					} else {
						phStartBusyTone(phoneCB);
						voipLedBlinking(phoneCB->physicalId);
					}
				} else {
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				
				phoneCB->linkSt = PHONE_LINK_ST_WAITING;
				phoneCB->phStatus = PHONE_STATUS_DISC;
				eventVoiceSimpleSend(EVT_PHONE_ST_CHECK,MSG_SEND_TO_PHONE, phoneCB->logicalId);				
				#ifdef ZYNOS_CALL_TRANSFER /*Eason, ZyNOS_Transfer*/
				if(phoneCB->phoneFlags & PHONE_ATTENDED_TRANSFER){
					phoneCB->phoneFlags &= ~PHONE_ATTENDED_TRANSFER;
					PH_FSM_PRINTF("fxsFsmData take off PHONE_ATTENDED_TRANSFER flag, send EVT_PHONE_FXS_ONHOOK\n");
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				#endif
				break;
			case EVT_PHONE_FXO_DETECT_ONHOOK:
				/* 1. Reset phone values */
				/* 2. Change phone Status */
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) {
					PH_FSM_DBG1("ccmConn->connType= ",ccmConn->connType);
					if ( ccmConn->connType == CCM_CONN_TYPE_CALLEE_CALLEE ) {
						PH_FSM_DBG("PR");
						phPolarityReverse(phoneCB);
					}
				}
				eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				reset_PhoneCB(phoneCB);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
			case EVT_PHONE_FXO_PLUGOUT:
				eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) {
					phStartCongestTone(phoneCB);
				} else {
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				phoneCB->linkSt = PHONE_LINK_ST_WAITING;
				phoneCB->phStatus = PHONE_STATUS_DISC;
				eventVoiceSimpleSend(EVT_PHONE_ST_CHECK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_CCM_DIAL_DIGIT:
				#if 0 /* CALL_CONFERENCE */
				if(checkCCMThreeWayCallStatus(phoneCB->physicalId)==1){ 	//skip action if the other phone(DSP) is used for threeWay calling
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						goto fail;
					}
				}
				#endif
				/* Play SIP INFO dial digits */
				if ( (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) && ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) ) {
						zyMemcpy(&dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
						PH_FSM_DBG1("PH: DSP Digit = ", dialDigitBuf.digitStr);
						currentLen = &(phoneCB->dialInform.dialDigitNumber.current_length);
						/* Add the new digital to the dialInform.dialDigitNumber */
						if ( *currentLen < MAX_DIAL_STRING_LENGTH-1 ) { 		/* ignore the digits that are more than the MAX length */
							phoneCB->dialInform.dialDigitNumber.dial_digits[*currentLen] = dialDigitBuf.digitStr;
							phoneCB->dialInform.dialDigitNumber.dial_durations[*currentLen] = dialDigitBuf.duration;
							phoneCB->dialInform.dialDigitNumber.dial_timeStampOffsets[*currentLen] = dialDigitBuf.timeStampOffset;
							phoneCB->dialInform.dialDigitNumber.dialout_length = *currentLen;
							vDspStartDtmfTone(phoneCB, 0);
						}
				}
				break;

			case EVT_PHONE_FAX_DETECT: {
				/* Use for FAX Relay */
					VoiceProfFaxT38Obj_t* pProfFax = tr104VoiceProfFaxT38ObjPtrGet(phoneCB->associatedVpItr);
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						vDspDisableToneDetection(phoneCB);
						vDspSetFAXParameter(phoneCB);
						if(phoneCB->callId < CALL_ID_UPBOUND) {	/* outgoing call */
							if(pProfFax->enable){
								eventVoiceCCMSimpleSend(EVT_CCM_FAX_T38, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
							}else{
								eventVoiceCCMSimpleSend(EVT_CCM_FAX_RELAY, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
							}
						}
					}
				}
				break;
			case EVT_PHONE_FAX_FINISH:
				/* Nedo marked:
					Nedo added this to change profile from FAX profile to previous voice profile
					This function is in phcmd_ti.c
					This Event is called by sc_ind_fax_cleardown() in SC_CHCTL.C
				*/
				//(void)vDspChanOpen(chId, voiceDspChCb[chId].compressType);
				break;
			case EVT_CCM_MEDIA_CHANGE:
				if ( ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) && ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) ){
					vDspChanOpen(phoneCB);
				}
				break;
			case EVT_PHONE_FLASH: {
					PH_FSM_DBG("FLASH key");
					processFlashKey(phoneCB);
				}
				break;
			case EVT_PHONE_DSP_DIGITS: /* Obsolete */
			case EVT_PHONE_FXS_DIGITS:
				//if(msg->data_p != NULL){
				if(zyStrlen(msg->data_p) > 0){
					if ( ccmConn->peerCcmConn != NULL ) {
						zyMemcpy(&dialDigitBuf, (dialDigitBuf_t *)msg->data_p, sizeof(dialDigitBuf_t));
						PH_FSM_DBG1("PH: FXS Digit = ", dialDigitBuf.digitStr);
						if((char)(dialDigitBuf.digitStr)=='D'){
							phoneCh_t *nextPhoneCB = NULL;
							nextPhoneCB = getNextLogicPhone(phoneCB);
							if((nextPhoneCB != NULL) && (nextPhoneCB->callProTimer.fxsWaitPhoneACKTimer)){
								PH_FSM_DBG1("Off Hook Caller ID Receive Phone ACK Logical ID: ", nextPhoneCB->logicalId);
								timerStop(nextPhoneCB->callProTimer.fxsWaitPhoneACKTimer);
								nextPhoneCB->callProTimer.fxsWaitPhoneACKTimer = 0;
								nextPhoneCB->callProTimer.fxsOffHookCallerIDTimer = timerStartS(150, &callerIDoffHookStartTimer, (void*)nextPhoneCB);
							}
						}
					        processFeatureKey(phoneCB, (dialDigitBuf_t *)msg->data_p);
						if(ccmConn->peerCcmConn->origPortType == SIP_PORT){
							PH_FSM_DBG1("SIP INFO-----pVoiceProf->DTMFMethod: ", pVoiceProf->DTMFMethod);
							if(pVoiceProf->DTMFMethod == TR104I_DTMF_SIPINFO){
								eventVoiceCCMDataSend(EVT_CCM_DIAL_DIGIT, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType, &dialDigitBuf, sizeof(dialDigitBuf_t) );
							}
						}
						else{
							eventVoiceCCMDataSend(EVT_CCM_DIAL_DIGIT, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType, &dialDigitBuf, sizeof(dialDigitBuf_t) );
						}
					}
				}
				break;
			case EVT_CCM_ONHOLD_CONF:
				/* 1. Change phoneCB->linkSt */
				/* 2. Close the DSP channels */
				/* 3. Swith to the next logical phoneCB */
				if ( ( phoneCB->phoneFlags & PHONE_PHYSICAL_ONHOLDING ) && (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) ){
					phoneCB->linkSt = PHONE_LINK_ST_ONHOLD;

#if 1	/* aircheng add for MOH */
					int LineItr = 0;
					LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
					PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
					PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, associatedVpItr = %d\n", phoneCB->associatedVpItr);
					PH_FSM_PRINTF(" receive EVT_CCM_DIAL_IND, LineItr = %d\n", LineItr);
					VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(phoneCB->associatedVpItr, LineItr);
					if(pLineCallingFeatures->ZyXEL_musicOnHoldTone == TR104I_MUSIC_ONHOLD_ACTIVE){
						ret = vDspStartMusicOnHold(phoneCB->callId, phoneCB->physicalId, pLineCallingFeatures->ZyXEL_musicOnHoldToneIndex);
						if(FALSE == ret){
							vDspChanClose(phoneCB);
						}
					}
					else{
								vDspChanClose(phoneCB);
					}
#endif					
								logicPhoneSwap(phoneCB, ccmConn);

				}
				break;

			default:
				PH_FSM_DBG1("fxsFsmData(CONNECT) - unknown event=", phoneEvent);
				break;
		}
	}else if ( phoneCB->linkSt == PHONE_LINK_ST_ONHOLD ){
		switch( phoneEvent ){
			case EVT_CCM_ONHOLD_CONF:
				/* Need to close DspChan for local held call phoneCB */
				vDspChanClose(phoneCB);
				break;
			case EVT_PHONE_FXS_ONHOOK:
				/* 1. Close the DSP channels */
				/* 2. Reset the phoneCB values */
				/* 3. Change the phone status */

				if ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE ) {		/* SIP */
					if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
						phDisableDtmfDetect(phoneCB);
						vDspChanOpen(phoneCB); /*reopen dsp Channel, in ZyNOS vDspChanClose(phoneCB, PASSING);*/
					}
					reset_PhoneCB(phoneCB);
					eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
					eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					phoneCB->linkSt = PHONE_LINK_ST_IDLE;
					phoneCB->phStatus = PHONE_STATUS_IDLE;
				} else {															/* FXO_LIFELINE */

				}
				break;
			case EVT_PHONE_FXS_OFFHOOK:
				/* The physical is in the onHook status. Use Ring to remind the user. Then the user offHook the phone */
				if ( ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) && (phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLD_RING) ) {
					phStopRing(phoneCB);
					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;
					eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
				}
				break;
			case EVT_CCM_DISC_IND:
				/* 1. Open the DSP Channel */
				/* 2. Play the busy tone */
				/* 3. Change the phoneCB status */

				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) {
					phDisableDtmfDetect(phoneCB);
					vDspChanOpen(phoneCB);
					phStartBusyTone(phoneCB);
				} else {
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				phoneCB->linkSt = PHONE_LINK_ST_WAITING;
				phoneCB->phStatus = PHONE_STATUS_DISC;
				eventVoiceSimpleSend(EVT_PHONE_ST_CHECK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				break;
			case EVT_PHONE_FXO_DETECT_ONHOOK:
				/* 1. Reset the phone values */
				/* 2. Change the phone status */
				reset_PhoneCB(phoneCB);
				eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
			case EVT_PHONE_FLASH:
				/* If UNHELD is failed, support the UNHELD again */
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
					phoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLDING;
				}
				break;
		#ifdef  CALL_CONFERENCE
			case EVT_CCM_CONFCALL_JOIN_MEMBER_REQ:
				phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD);
				/*phoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;*/ /* bugfix: EVT_LOCAL_ACTIVE_LEAVE will find wrong phoneCB */
				if ( phoneCB->phStatus == PHONE_STATUS_DATA ) {
					phoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLDING;
					//eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
					eventVoiceCCMSimpleSend(EVT_CCM_CONFERENCE_IND, ORIG_FUNC_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				}
				break;
			case EVT_CCM_CONFCALL_JOIN_MEMBER_CONF: {
					uint8 *peerIsLocal = (uint8 *)msg->data_p;
					if(*peerIsLocal){
						if ((newPhoneCB = getNextLogicPhone(phoneCB)) != NULL) {
							if(newPhoneCB->localConfPeerLocalPhLogicalId!=0){
								ccmConn->ConfCallCB->threeWayCallBaseLogicId = newPhoneCB->logicalId;
								vDspCreateLocalPeerMeetingRoom(newPhoneCB);
							}else{
								newPhoneCB->localConfPeerLocalPhLogicalId=phoneCB->localConfPeerLocalPhLogicalId;
								ccmConn->ConfCallCB->threeWayCallBaseLogicId = newPhoneCB->logicalId;					
								vDspCreateLocalPeerMeetingRoom(newPhoneCB);
							}
						}
					}else{
						if ( phoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLDING) {
							phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLDING;
							phoneCB->linkSt = PHONE_LINK_ST_CONNECT;
							vDspCreateMeetingRoom(phoneCB);
						}
					}
				}
				break;
			case EVT_CCM_3WAYCALL_MEETROOM_DESTROY:
				vDspDestroyMeetingRoom(phoneCB->logicalId);
				break;
			case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ:{
					/* Once the conference has been established between A, B and C, 
					if A puts A-C call on hold and then resumes it, the conference is also resumed between A, B and C
					IPTK expects the media to remain merged at Media Manager for calls A-B and A-C. 
					And hence expects conference ongoing and don't need to destory meeting room
					*/
					#if 1
					uint8 *tmpPeerisLocal = (uint8 *)msg->data_p;
					if(*tmpPeerisLocal){
						vDspDestroyLocalPeerMeetingRoom(phoneCB->logicalId);
					}else{
						vDspDestroyMeetingRoom(phoneCB->logicalId);
					}
					#endif
					eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &phoneCB->logicalId, sizeof(uint16) );
				}
				break;
			case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ: /* Peer is Local */
				threeWayCallBaseLogicId = (uint8 *)msg->data_p;

				vDspDestroyLocalPeerMeetingRoom(*threeWayCallBaseLogicId);
				eventVoiceCCMSimpleSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_CONF, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				break;
		#endif
			case EVT_CCM_MEDIA_CHANGE:
				break;
			case EVT_CCM_UNHELD_CONF:
				/* 1. Remove the UNHOLDING */
				/* 2. Change phoneCB->linkSt */
				/* 3. Action for this phoneCB */
				PH_FSM_PRINTF("EVT_CCM_UNHELD_CONF, phoneFlags = 0x%x\n", phoneCB->phoneFlags);
				if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
					phoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLDING;
					phoneCB->linkSt = PHONE_LINK_ST_CONNECT;					
					if(phHookStatus(phoneCB) == 1) {	/* offhook */
						enterLogicPhone(phoneCB, ccmConn);
					}

				}
				else if(ccmConn->ConfCallCB != NULL){ /*Eason, 2009/07/07, to support hold/resume one call without remove conference*/
					if ((newPhoneCB = getNextLogicPhone(phoneCB)) != NULL) {
						/*Eason, conference call created, call hold and call resume, change physical usage phone to the resumed one*/
						phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_UNHOLDING|PHONE_PHYSICAL_ONHOLD);
						phoneCB->linkSt = PHONE_LINK_ST_CONNECT;

						/* After conference, flash+2 
						  * phone1 active, phone2 onhold 
						  * then flash + 3, resume phone2 (phone1 active PHYSICAL_USAGE)
						  * phone2 receive UNHELD_CONF -> set confCallStatus to ALL_MEMBER_JOINED
						  */
						CCM_PRINTF("EVT_CCM_UNHELD_CONF, ConfCall state changed to CONF_CALL_ST_ALL_MEMBER_JOINED\n");
						ccmConn->ConfCallCB->confcallStatus = CONF_CALL_ST_ALL_MEMBER_JOINED;	
						vDspChanOpen(phoneCB);						
					}
				}
				break;
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif
			default:
				PH_FSM_DBG1("fxsFsmData(ONHOLD) - unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
} /* fxsFsmData */


/*______________________________________________________________________________
**	fxsFsmDrop
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
fxsFsmDrop(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;

	PH_FSM_DBG2("fxsFsmDrop - phoneEvent = ", phoneEvent, "phoneCB=", (uint32)phoneCB);
	PH_FSM_DBG1("             logicId=", phoneCB->logicalId);

	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxsFsmDrop - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	if ( phoneCB->linkSt == PHONE_LINK_ST_IDLE ){
		switch( phoneEvent ){
			case EVT_CCM_DISC_ACK:
				reset_PhoneCB(phoneCB);
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				break;
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif
			default:
				PH_FSM_DBG1("fxsFsmDrop(CONNECT)-unknown event=", phoneEvent);
				break;
		}
	}

fail:
	return status;
} /* fxsFsmDrop */


/*______________________________________________________________________________
**	fxsFsmDisc
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
fxsFsmDisc(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	ccmConn_t *ccmConn = NULL;	

	PH_FSM_DBG2("fxsFsmDisc - phoneEvent = ", phoneEvent, " phoneCB=", (uint32)phoneCB);
	PH_FSM_DBG2("             linkSt = ", phoneCB->linkSt, ", logicId=", phoneCB->logicalId);
	//printf("Nick debug:fxsFsmDisc::phoneCB=%d,phoneCB->linkSt=%d,phoneEvent=%d,phoneCB->logicId=%d\n",phoneCB,phoneCB->linkSt,phoneEvent,phoneCB->logicalId);
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);

	if ( ccmConn == NULL ) {
		PH_FSM_DBG("fxsFsmDisc - ccmConn could not found");
		status = ERROR_CCM_CONN_FIND;
		goto fail;
	}

	switch( phoneEvent ){
		case EVT_PHONE_FLASH:
			if ( checkLogicPhoneNum(phoneCB->physicalId) >= 2 ) {
				phStopTone(phoneCB); /* add by Jacky 2008/12/11 */
				eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
			}
			break;
#if 0
		case EVT_CCM_UNHELD_REQ:
			/* 1. Change the phoneFlags */
			/* 2. Play the busy tone */
			phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD|PHONE_PHYSICAL_UNHOLDING);
			phoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
			phStartBusyTone(phoneCB);
			break;
#endif
		case EVT_PHONE_FXS_ONHOOK:
			/* 1. Stop all tones */
			/* 2. Change phoneCB->linkSt and phoneCB->phStatus */
			/* 3. reset phoneCB vaules */
			/* 4. Send REL_REQ to CCM and PHONE_REMOVE to PhoneTask */
		
			if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) {
				phDisableDtmfDetect(phoneCB);
				phPolarityReverse(phoneCB);
				vDspChanOpen(phoneCB);
			}
			ccmConn->flags &=  ~(CALL_CONN_FLAG_PREASSIGN|CALL_CONN_FLAG_ASSIGN);
			phoneCB->linkSt = PHONE_LINK_ST_IDLE;
			phoneCB->phStatus = PHONE_STATUS_IDLE;
			reset_PhoneCB(phoneCB);
#ifdef VISUAL_MWI
			if(phHookStatus(phoneCB) == 0) {	/* onhook */
				if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 1) {
					if ( (SP_VSIGNAL_PTR(phoneCB->physicalId))->vmwiAbbrTone == 1 ) {
						phStartRingingPulseAlertingSignal(phoneCB); /* Ring splash */
						//pause(100);
						usleep(100*1000);
					}
					generateVMWI(phoneCB->physicalId, TRUE);
				} else if(voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 2) {
					generateVMWI(phoneCB->physicalId, FALSE);
				}
				voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] == 0;
			}
#endif
			eventVoiceCCMSimpleSend(EVT_CCM_REL_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
			eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
			break;
		case EVT_PHONE_ST_CHECK: /* Protection Only */
			if ( (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE) && (phHookStatus(phoneCB) == 0)) { /* On Hook */
				ccmConn->flags &=  ~(CALL_CONN_FLAG_PREASSIGN|CALL_CONN_FLAG_ASSIGN);
				phoneCB->linkSt = PHONE_LINK_ST_IDLE;
				phoneCB->phStatus = PHONE_STATUS_IDLE;
				eventVoiceCCMSimpleSend(EVT_CCM_DISC_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				eventVoiceSimpleSend(EVT_PHONE_REMOVE, MSG_SEND_TO_PHONE, phoneCB->logicalId);
			}
			break;
		#if CCBS_SUPPORT
		case EVT_PHONE_FXS_DIGITS:
			{
				dialDigitBuf_t *dialDigitBuf = (dialDigitBuf_t *)msg->data_p; 
				int LineItr = 0;
				LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
				PH_FSM_PRINTF(" receive EVT_PHONE_FXS_DIGITS, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
				PH_FSM_PRINTF(" receive EVT_PHONE_FXS_DIGITS, associatedVpItr = %d\n", phoneCB->associatedVpItr);
				PH_FSM_PRINTF(" receive EVT_PHONE_FXS_DIGITS, LineItr = %d\n", LineItr);
				VoiceProfLineCallingFeaturesObj_t * lineCallingFeaturesObj = tr104VoiceLineCallingFeatureObjPtrGet(phoneCB->associatedVpItr, LineItr);
				PH_FSM_PRINTF("DIGIT: %c\n", dialDigitBuf->digitStr);
				if( (NULL != lineCallingFeaturesObj) && lineCallingFeaturesObj->ZyXEL_CCBSEnable){					
					if(dialDigitBuf->digitStr == '5'){
						PH_FSM_PRINTF("CCBS activate\n");
						if(DSP_FXS_PORT == phoneCB->dspPortType){ /*CCBS support FXS now*/
							PH_FSM_PRINTF("CCBS digits %s\n", phoneCB->dialInform.dialDigitNumber.dial_digits);
							if(zyStrlen( phoneCB->dialInform.dialDigitNumber.dial_digits)){
								if (webPhoneDialStart(phoneCB->dspPortType, phoneCB->physicalId, phoneCB->dialInform.dialDigitNumber.dial_digits)==-1){
									printf("WebPhoneDial init fail!\n\r");
									phStopTone(phoneCB);
									phStartNegativeAckTone(phoneCB);
								}
								else{
									PH_FSM_PRINTF("CCBS activate\n");
									phStopTone(phoneCB);									
									phStartPositiveAckTone(phoneCB);
								}
							}
							else{
								printf("WebPhoneDial fail, dial digit empty!\n\r");
								phStopTone(phoneCB);
								phStartNegativeAckTone(phoneCB);
							}
						}
					}							
				}
				else{
					phStopTone(phoneCB);
					phStartNegativeAckTone(phoneCB);
					PH_FSM_PRINTF("CCBS not enabled\n");
				}
			}
		break;
		#endif
#ifdef VISUAL_MWI
			case EVT_PHONE_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, TRUE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 1;	/* offhook. Wait until onhook to send icon */
				}
				break;
			case EVT_PHONE_NO_NEW_VOICE_MESSAGE:
				if(phHookStatus(phoneCB) == 0) {	/* onhook */
					generateVMWI(phoneCB->physicalId, FALSE);
				} else {
					voicePhoneCh.mwiChangeStatus[phoneCB->physicalId] = 2;	/* offhook. Wait until onhook to send icon */
				}
				break;				
#endif
		default:
			PH_FSM_DBG1("fxsFsmDisc - unknown event=", phoneEvent);
			break;
	}

fail:
	return status;
} /* fxsFsmDisc */

#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
/*______________________________________________________________________________
**	fxsFsmTest
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
fxsFsmTest(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {
	int status = 0;
	uint16 phoneEvent = msg->event;
	dialDigitBuf_t *dialDigitBuf = NULL;
	uint8 *currentLen;
	int autoTestDigitLen = 0;
	uint8 *autoTestDigit = NULL;
	int i = 0;
	int toneId = 0;
	
	PH_FSM_DBG("Enter fxsFsmTest\n");

	switch( phoneEvent ){
		case EVT_PHONE_FXS_OFFHOOK:
			PH_FSM_DBG("Detect FXS_OFFHOOK\n");

			/* stop ring timer */
			if( phoneCB->physicalId == 0 )
				timerStop(VoIP_autoTest.fxsAutoTestTimerOne);
			else if( phoneCB->physicalId == 1 )	
				timerStop(VoIP_autoTest.fxsAutoTestTimerTwo);			
			
			phoneCB->dialInform.dialDigitNumber.current_length = 0;
			eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, MSG_SEND_TO_PHONE, phoneCB->logicalId);
			break;

		case EVT_PHONE_DIAL_REQ:
			/* generate DTMF tone */

			autoTestDigit = strdup(VoIP_autoTest.autoTest_digits);
			PH_FSM_DBG1("autoTestDigit: ",autoTestDigit);
			
			/* sleep 1s */
			sleep(1);
			for( i = 0; i < strlen(autoTestDigit) ; i++ )
			{
				/* check number */
				switch(autoTestDigit[i])
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						toneId = 43 + (autoTestDigit[i]-'0');
						PH_FSM_DBG1("toneID= ",toneId);
						break;
					case '#':
						break;
					case '*':
						break;
					default:
						printf("Enter wrong digit...\n");
						printf("change digit to 1...\n");
						toneId = 43;
						break;
				}
				
				PhoneStartTone(phoneCB, toneId);
				/* sleep 200ms */
				usleep(200000);
			}
			free(autoTestDigit);
		
			/* start timer - DTMF detect timeout */
			PH_FSM_DBG1("phoneCB->physicalId: ",phoneCB->physicalId);
			if( phoneCB->physicalId == 0 )
				VoIP_autoTest.fxsAutoTestTimerOne = timerStartS(5000, fxsDTMFTimeout, phoneCB);
			else if( phoneCB->physicalId == 1 ) 	
				VoIP_autoTest.fxsAutoTestTimerTwo = timerStartS(5000, fxsDTMFTimeout, phoneCB);
	
			break;
		case EVT_PHONE_FXS_DIGITS:
			
			dialDigitBuf = (dialDigitBuf_t *)msg->data_p;
			PH_FSM_DBG1("Receive digit: ",dialDigitBuf->digitStr);

			/* stop dtmf timer */
			if( phoneCB->physicalId == 0 )
				timerStop(VoIP_autoTest.fxsAutoTestTimerOne);
			else if( phoneCB->physicalId == 1 )	
				timerStop(VoIP_autoTest.fxsAutoTestTimerTwo);
		
			if ( dialDigitBuf != NULL ) {
				currentLen = &(phoneCB->dialInform.dialDigitNumber.current_length);
				
				autoTestDigitLen = strlen(VoIP_autoTest.autoTest_digits);
				if ( *currentLen < autoTestDigitLen ) {
					phoneCB->dialInform.dialDigitNumber.dial_digits[*currentLen] = dialDigitBuf->digitStr;
					(*currentLen)++;
					
					if ( *currentLen == autoTestDigitLen ) {
						PH_FSM_DBG1("Receive dial string: ",phoneCB->dialInform.dialDigitNumber.dial_digits);
						
						if( strncmp(phoneCB->dialInform.dialDigitNumber.dial_digits,VoIP_autoTest.autoTest_digits,autoTestDigitLen) != 0 )
						{
							printf("Error: digit does not map\n");
							printf("dial_digits:%s\n",phoneCB->dialInform.dialDigitNumber.dial_digits);
							if( phoneCB->physicalId == 0 )
								VoIP_autoTest.isFXSONESucc = FALSE;
							else if( phoneCB->physicalId == 1 )	
								VoIP_autoTest.isFXSTWOSucc = FALSE;
						}else{
							PH_FSM_DBG("Digit is the same\n");
						}
					}
				}
			}			
			/* start timer - ONHOOK detect timeout */
			if( phoneCB->physicalId == 0 )
				VoIP_autoTest.fxsAutoTestTimerOne = timerStartS(5000, fxsONHOOKTimeout, phoneCB);
			else if( phoneCB->physicalId == 1 )
				VoIP_autoTest.fxsAutoTestTimerTwo = timerStartS(5000, fxsONHOOKTimeout, phoneCB);
			
			break;
		case EVT_PHONE_FXS_ONHOOK:
			PH_FSM_DBG("Detect onhook\n");

			/* stop ONHOOK timer */
			if( phoneCB->physicalId == 0 )
			{
				timerStop(VoIP_autoTest.fxsAutoTestTimerOne);
				if( VoIP_autoTest.isFXSONESucc == TRUE )
				{
					PH_FSM_DBG("FXS1 TEST PASS!!\n");
					system("echo \"FXS1 TEST PASS!!\"> /tmp/autofxs1");
				}else{
					PH_FSM_DBG("FXS1 TEST FAIL!!\n");
					system("echo \"FXS1 TEST FAIL!!\"> /tmp/autofxs1");
				}
			}	
			else if( phoneCB->physicalId == 1 )	
			{
				timerStop(VoIP_autoTest.fxsAutoTestTimerTwo);
				if( VoIP_autoTest.isFXSTWOSucc == TRUE )
				{
					PH_FSM_DBG("FXS2 TEST PASS!!\n");
					system("echo \"FXS2 TEST PASS!!\"> /tmp/autofxs2");
				}else{
					PH_FSM_DBG("FXS2 TEST FAIL!!\n");
					system("echo \"FXS2 TEST FAIL!!\"> /tmp/autofxs2");
				}
			}			
			
			phoneCB->phStatus = PHONE_STATUS_IDLE;
			phoneCB->dialInform.dialDigitNumber.current_length = 0;
			
			
			break;	
		default:
			PH_FSM_DBG1("fxsFsmTest - unknown event=", phoneEvent);
			break;
	}

fail:
	return status;
} /* fxsFsmTest */

static int
fxsDTMFTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	
	printf("phoneCB->physicalId %d wait DTMF timeout!!!!\n",tmpPhoneCB->physicalId);
	
	if( tmpPhoneCB->physicalId == 0 )
	{
		system("echo \"FXS1 wait DTMF timeout!!\"> /tmp/autofxs1");
		VoIP_autoTest.isFXSONESucc = FALSE;
	}	
	else if( tmpPhoneCB->physicalId == 1 )
	{
		system("echo \"FXS2 wait DTMF timeout!!\"> /tmp/autofxs2");
		VoIP_autoTest.isFXSTWOSucc = FALSE;
	}
	
	/* reset state and */
	tmpPhoneCB->phStatus = PHONE_STATUS_IDLE;
	tmpPhoneCB->dialInform.dialDigitNumber.current_length = 0;
	
	return 0;
}

static int
fxsONHOOKTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	
	printf("phoneCB->physicalId %d wait ONHOOK timeout!!!!\n",tmpPhoneCB->physicalId);
	
	if( tmpPhoneCB->physicalId == 0 )
	{
		system("echo \"FXS1 wait ONHOOK timeout!!\"> /tmp/autofxs1");
	}else{
		system("echo \"FXS2 wait ONHOOK timeout!!\"> /tmp/autofxs2");
	}
	
	/* reset state and */
	tmpPhoneCB->phStatus = PHONE_STATUS_IDLE;
	tmpPhoneCB->dialInform.dialDigitNumber.current_length = 0;
	
	return 0;
}

#endif

/*Jason , syslog*/
#ifdef VOIP_SYSLOG

int
fxsPhoneEventLog(
	evtMsg_t	*msg,				/* command buffer pointer	*/
	phoneCh_t *phoneCB			/* the phoneCB of this cbuf */
) {

	uint16 phoneEvent = msg->event;
	char loginfo[250]="";//length to be define
	VoiceProfLineObj_t *pLine =NULL;
	dialDigitBuf_t *dialDigitBuf = NULL;
	#if 1 /*Eason, get registered line obj then enabled line obj*/
	pLine = getAssociatedRegLineObj(phoneCB);
	#else
	pLine = getAssociatedLineObj(phoneCB);
	#endif

	if(pLine==NULL){
		printf("\r\n fxsPhoneEventLog -- getAssociatedLineObj fail logicalId = 0x%x\n", phoneCB->logicalId);
		return -1;
	}
	
	switch( phoneEvent ){
		case EVT_PHONE_FXS_OFFHOOK:
				voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,phoneCB->physicalId+1,"OFFHOOK");
				break;
		case EVT_PHONE_FXS_ONHOOK:
				voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,phoneCB->physicalId+1,"ONHOOK");
				break;
		case EVT_PHONE_FLASH:
				voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,phoneCB->physicalId+1,"FLASH");
				break;
		case EVT_PHONE_FXS_DIGITS:
				dialDigitBuf = (dialDigitBuf_t *)msg->data_p; 
				sprintf(loginfo,"dtmf[%c]", dialDigitBuf->digitStr);
				/* __MSTC__, Klose,DSL-2492GNU-B1B_Eircom [SPR 121114290], DTMF tones cannot be received and sent as Syslog messages, 20121122 */
				if(phoneCB->phStatus != PHONE_STATUS_DATA){
				   voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,phoneCB->physicalId+1,loginfo);
				}
				break;
		default:
			break;
	}
	
	return 0;
}
#endif

/*______________________________________________________________________________
**	fxsMWIShutterToneTimeout
**
**	descriptions:    Use to check MWI Stutter Tone Timeout.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
fxsMWIStutterToneTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	VoiceProfNumPlanObj_t* pProfNumPlan = tr104VoiceProfNumPlanObjPtrGet(tmpPhoneCB->associatedVpItr);
	PH_FSM_DBG("fxsMWIStutterToneTimeout");
	tmpPhoneCB->callProTimer.fxsMWIStutterToneTimer = 0;
	if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		phStopTone(tmpPhoneCB);

		phStartDialTone(tmpPhoneCB);
		if ( tmpPhoneCB->callProTimer.firstDigitTimer == 0 ) {
			tmpPhoneCB->callProTimer.firstDigitTimer = timerStartS(1000*(pProfNumPlan->ZyXEL_dialingLongInterval), firstDigitTimeout, tmpPhoneCB);
		}
	}
	return 0;
} /* fxsMWIShutterToneTimeout */

/*______________________________________________________________________________
**	firstDigitTimeout
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
static int
firstDigitTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	#if 1 /*Eason, get registered line obj then enabled line obj*/
	VoiceProfLineObj_t *pLine = getAssociatedRegLineObj(tmpPhoneCB);
	#else
	VoiceProfLineObj_t *pLine = getAssociatedLineObj(tmpPhoneCB);
	#endif
	
	PH_FSM_DBG("firstDigitTimeout");
	tmpPhoneCB->callProTimer.firstDigitTimer = 0;
	if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		phStopTone(tmpPhoneCB);

	}
	PH_FSM_DBG1("tmpPhoneCB->physicalId: ", tmpPhoneCB->physicalId);
	if(NULL != pLine){
		PH_FSM_DBG1("WarmLine enable: ", pLine->callingFeatures->ZyXEL_warmLineEnable);
	}
	if((NULL!= pLine) && (NULL!= pLine->callingFeatures) && (pLine->callingFeatures->ZyXEL_warmLineEnable) && (zyStrlen(pLine->callingFeatures->ZyXEL_warmLineNum) > 0)){
		zyStrncpy(tmpPhoneCB->dialInform.dialDigitNumber.dial_digits, pLine->callingFeatures->ZyXEL_warmLineNum, MAX_DIAL_STRING_LENGTH);
		tmpPhoneCB->dialInform.dialDigitNumber.current_length = zyStrlen(tmpPhoneCB->dialInform.dialDigitNumber.dial_digits);
		tmpPhoneCB->linkSt = PHONE_LINK_ST_DIGIT;
		eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
	}else{
		if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
			phStartBusyTone(tmpPhoneCB);			
		}
		tmpPhoneCB->phStatus = PHONE_STATUS_DISC;
	}
	return 0;
} /* firstDigitTimeout */

#ifdef HOWLER_TONE
/*______________________________________________________________________________
**	howlerToneTimeout
**
**	descriptions:    Use to check the fxs offhook and no valid call for a long time
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
howlerToneTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;


	PH_FSM_DBG("howlerToneTimeout");
	tmpPhoneCB->callProTimer.fxsOffhookHowlerToneTimer= 0;
	if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		phStopTone(tmpPhoneCB);

	}
	PH_FSM_DBG1("tmpPhoneCB->physicalId: ", tmpPhoneCB->physicalId);
	if(tmpPhoneCB->phStatus != PHONE_STATUS_DATA){
		PH_FSM_DBG("start play howler tone");
		tmpPhoneCB->howlerToneSeconds = 0;
		vDspStartHowlerTone(tmpPhoneCB);
		PH_FSM_DBG("start fxsHowlerTonePeriodicTimer to raise volume for howler tone");
		if(tmpPhoneCB->callProTimer.fxsHowlerTonePeriodicTimer !=0){
			timerPeriodicStop(tmpPhoneCB->callProTimer.fxsHowlerTonePeriodicTimer);			
			tmpPhoneCB->callProTimer.fxsHowlerTonePeriodicTimer = 0;
		}
		tmpPhoneCB->callProTimer.fxsHowlerTonePeriodicTimer = timerPeriodicStartS(1000, howlerToneRaiseVolumeTimeout, tmpPhoneCB);
	}
	
	return 0;
} /* howlerToneTimeout */

/*______________________________________________________________________________
**	howlerToneRaiseVolumeTimeout
**
**	descriptions:    Use to raise volume for howler tone
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
#define HOWLER_TONE_MAX_STEP_CYCLES 12
#define HOWLER_TONE_STEP_LEVEL 3
#define HOWLER_TONE_MINIMUN_LEVEL -21
#define GAINUNIT 10
static int
howlerToneRaiseVolumeTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	PH_FSM_DBG("howlerToneRaiseVolumeTimeout"); 

	if(NULL != tmpPhoneCB){	
		PH_FSM_PRINTF("tmpPhoneCB->physicalId:%d, seconds = %d\n", tmpPhoneCB->physicalId, tmpPhoneCB->howlerToneSeconds);
		if(tmpPhoneCB->phStatus != PHONE_STATUS_DATA){
			PH_FSM_DBG("raise volume for howler tone");
			phSetTxGain(tmpPhoneCB, (HOWLER_TONE_MINIMUN_LEVEL + (HOWLER_TONE_STEP_LEVEL * tmpPhoneCB->howlerToneSeconds))*GAINUNIT );		
		}

		tmpPhoneCB->howlerToneSeconds++;
		if(tmpPhoneCB->howlerToneSeconds > HOWLER_TONE_MAX_STEP_CYCLES){
			tmpPhoneCB->howlerToneSeconds = HOWLER_TONE_MAX_STEP_CYCLES;
		}

	}
	return 0;
} /* howlerToneTimeout */


#endif

#ifdef TERMINAL_PORTABILITY
/*______________________________________________________________________________
**	TerminalPortabilityTimeout
**
**	descriptions: Enter Terminal Portability.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
TerminalPortabilityTimeout (
	tm_id_t timer_id,
	void	*arg
) {
    	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
		if(tmpPhoneCB != NULL){
        	eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
		}

	return 0;
} /* TerminalPortabilityTimeout */
#endif /* TERMINAL_PORTABILITY */

/*______________________________________________________________________________
**	dialDigitsTimeout
**
**	descriptions:   Use to check if the final dial digits.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
dialDigitsTimeout (
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	PH_FSM_DBG("\r\ndialDigitsTimeout\r\n");
	tmpPhoneCB->callProTimer.dialDigitTimer = 0;

	eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
	//eventVoiceSimpleSend(EVT_PHONE_DIAL_REQ, phoneCb.sig.func_id, tmpPhoneCB->logicalId);
	
	return 0;
} /* dialDigitsTimeout */

/*______________________________________________________________________________
**	callerIDDTASStartTimer
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
callerIDDTASStartTimer( 
	tm_id_t tmId, 
	void *param 
) {
#ifdef ZyNOS
#if (SUPPORT_VDSP != VDSP_DANUBE)
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)param;

	tmpPhoneCB->callProTimer.fxsCallerIDTACSecTimer= 0;
	
	slicStartDualPluseAlertingSignal(tmpPhoneCB);

#endif
#endif
	return 0;
}

/*______________________________________________________________________________
**	callerIDoffHookStartTimer
**
**	descriptions: OffHook send Caller ID during Call Waiting.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
callerIDoffHookStartTimer( 
	tm_id_t tmId, 
	void *param 
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)param;

	if ( tmpPhoneCB != NULL ) {

		tmpPhoneCB->callProTimer.fxsCIDFSKOffHookWaitACKTimer= 0;
		if (vDSPStartOffHookCIDFSK(tmpPhoneCB) != 0) {

			PH_FSM_DBG("WARN: callerIDFSKStartTimer() - Fail to send the callerID");
		}

	}
	return 0;
}

/*______________________________________________________________________________
**	checkCallWaitingTimeout
**
**	descriptions: If the user does not receive this call-waiting call, send REJ_REQ to this call after the timeout.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
checkCallWaitingTimeout(
	tm_id_t timer_id, 
	void *arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;

	if(tmpPhoneCB != NULL){
		eventVoiceSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
		if(tmpPhoneCB->callProTimer.fxsCallWaittingRepeater) {
			timerStop(tmpPhoneCB->callProTimer.fxsCallWaittingRepeater);
			tmpPhoneCB->callProTimer.fxsCallWaittingRepeater = 0;
		}
		timerStop(tmpPhoneCB->callProTimer.fxsCallWaittingRejectTimer);
		tmpPhoneCB->callProTimer.fxsCallWaittingRejectTimer = 0;
	}

	return 0;
}/* checkCallWaitingTimeout */

/*______________________________________________________________________________
**	connConfTimeout
**
**	descriptions: Use to check the media message to open the dsp channel
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
connConfTimeout (
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;

	PH_FSM_DBG("connConfTimeout");
	eventVoiceCCMSimpleSend(EVT_CCM_CONN_REQ_TO, MSG_SEND_TO_CCMCONN, tmpPhoneCB->logicalId, DSP_FXS_PORT);
	tmpPhoneCB->callProTimer.connConfTimer = 0;
	if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		phStopTone(tmpPhoneCB);
		phStartBusyTone(tmpPhoneCB);
	}
	tmpPhoneCB->linkSt = PHONE_LINK_ST_WAITING;
	tmpPhoneCB->phStatus = PHONE_STATUS_DISC;

	return 0;
} /* connConfTimeout */

/*______________________________________________________________________________
**  getNewVoiceMessage
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision: for MWI
**____________________________________________________________________________*/
static int
getNewVoiceMessage(
	uint16	chId
){
	int ret = 0;
	VoicePhyIntfObj_t*	pphyInterface = tr104VoicePhyIntfObjPtrGet(chId);

	if ( pphyInterface->ZyXEL_LineSelect& voiceMessageFlag ) {
		ret = 1;
	}

	return ret;
}

/*______________________________________________________________________________
**  setNewVoiceMessage
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision: for MWI
**____________________________________________________________________________*/
uint8
setNewVoiceMessage(
	int sipIndex, 
	boolean flag
){
	if (flag) {
		voiceMessageFlag |= (1 << sipIndex);
	} else {
		voiceMessageFlag &= (~(1 << sipIndex));
	}

	return 0;
}

/*______________________________________________________________________________
**  dialDigitsAdj
**
**  descriptions: Add the new dial digits to the phoneCB->dialInform.dialDigitNumber. And check the removePound action.
**  parameters:
**  local:
**  global:
**  return: 1 - success, 2 - numberingPlan partial match, 3 - speedDial match but not active
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static int
dialDigitsAdj(
	phoneCh_t *phoneCB,
	dialDigitBuf_t	*dialDigitBuf
){
	uint8 *currentLen;
	int ret=0;
	uint8 phFeatureLen = 0;
	
#ifdef NORWAY_VOIP_CUSTOMIZATION
	ZyXEL_VoiceCommon_t * tr104VoiceComm = tr104VoiceCommonObjPtrGet();
	if(tr104VoiceComm == NULL){
		printf("error : tr104VoiceComm is NULL \n");
		return -1;
	}
#endif
	
	PH_FSM_DBG1("dialDigitsAdj : phoneCB->logicalId=",phoneCB->logicalId);
	if ( dialDigitBuf != NULL ) {		
		currentLen = &(phoneCB->dialInform.dialDigitNumber.current_length);
		/* Check the remove pound */
		if ( zyxelTE == 1 ) {
					if ( (char)(dialDigitBuf->digitStr) == '#' ) {
								return 1;
							}
						}				
		ret=checkPoundKey(phoneCB,dialDigitBuf,currentLen);
		if(ret) {
			PH_FSM_DBG1(" CheckPoundKey ,ret= ",ret);
			return ret;
		}		

		/* Add the new digital to the dialInform.dialDigitNumber */
		if ( *currentLen < MAX_DIAL_STRING_LENGTH-1 ) { 		/* ignore the digits that are more than the MAX length */
			phoneCB->dialInform.dialDigitNumber.dial_digits[*currentLen] = dialDigitBuf->digitStr;
			phoneCB->dialInform.dialDigitNumber.dial_durations[*currentLen] = dialDigitBuf->duration;
			phoneCB->dialInform.dialDigitNumber.dial_timeStampOffsets[*currentLen] = dialDigitBuf->timeStampOffset;
			(*currentLen)++;
		}
#ifdef SPEED_DIAL		
		PH_FSM_DBG(" SpeedDialCheck \n ");

		/* check the speed dial/phone book to see if we need to send digit immediately*/
		ret = SpeedDialCheck(&(phoneCB->dialInform.dialDigitNumber.dial_digits[0]), currentLen);
		if(ret) {
			PH_FSM_DBG1(" SpeedDialCheck ,ret= ",ret);
			return ret;
		}
#endif		

		/*check the new digitail is in Numbering plan(phoneconfig) */
		ret=checkDialDigitsNumberingPlan(phoneCB->associatedVpItr,(char *)&phoneCB->dialInform.dialDigitNumber.dial_digits[0], &phFeatureLen );
				
		if(ret) {
			PH_FSM_DBG1(" Check Numbering Plan ,ret= ",ret);
			return ret;
		}

#ifdef DIAL_PLAN
		/* check for instant transformation of dial plan */
#ifdef NORWAY_VOIP_CUSTOMIZATION 		
		if(tr104VoiceComm->specialFlag & SPT_SPECIAL_IF_TELENOR){
			int SipAccountId = 0, VpItr = 0;
		    SipAccountId = getAssociateRegedLineId(phoneCB);			
			VpItr = mapSipAccountIdToVpItr(SipAccountId);						
			PH_FSM_PRINTF("DIAL_PLAN associatedVpItr = %d\n",VpItr);
		
			/* check for instant transformation of dial plan */
			ret = DialOutPartialCheck(VpItr, &(phoneCB->dialInform.dialDigitNumber.dial_digits[0]), currentLen);
			if(ret) {
				PH_FSM_DBG1(" DialOutPartialCheck ,ret= ",ret);
				return ret;
			}
		}else
#endif		
		{
			ret = 0;

			int instantPartialCheck = -1;
			int SipAccountId = 0, VpItr = 0;
		//Ssu-Ying, support Multiple Profile
		    SipAccountId = getAssociateRegedLineId(phoneCB);			
			VpItr = mapSipAccountIdToVpItr(SipAccountId);						
			PH_FSM_PRINTF("DIAL_PLAN associatedVpItr = %d\n",VpItr);
			instantPartialCheck=instantDialOutPartialCheck(VpItr, &(phoneCB->dialInform.dialDigitNumber.dial_digits[0]), currentLen);
			instantDial = FALSE;
			if(instantPartialCheck == 0 ){ /*partial match*/	
				PH_FSM_DBG1("dialDigisAdj : DIAL_PLAN :instantPartialCheck=%d \n",instantPartialCheck);
			}else if(instantPartialCheck == 1){ /*matched*/
				instantDial = TRUE;
				return 1;
			}
		}
#endif

	}
	

	return 0;

}

#ifdef SPEED_DIAL

/*______________________________________________________________________________
**	DialOutPartialCheck
**
**	descriptions:check the dialing digits if match the phone book or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
SpeedDialCheck(uint8   *input, uint8   *length)
{
	int index = 0, phoneBookLen = 0;
	ZyXEL_VoicePhoneBook_t* voicePhBook_p = NULL;
	PH_FSM_PRINTF("extering %s, input %s, length %d\n",__FUNCTION__, input, *length);	

	for ( index=0; index<TR104_VPHONE_BOOK_CNT; index++ ) {
		voicePhBook_p = tr104VoicePhoneBookPtrGet(index);
		if(voicePhBook_p==NULL)
				continue;
		phoneBookLen =  zyStrlen(&voicePhBook_p->speedNumber[0]);		
		
		if ( ( voicePhBook_p->flags & SPEED_DIAL_ACTIVE ) && ( *length == phoneBookLen ) ) {
			if ( memcmp(&(voicePhBook_p->speedNumber[0]), input,*length ) == 0 ) {
				return 1;/*whole match the speed dial rule, send to CCM immediately*/
			}
		}

		#if 1 /*Eason, match inactive phonebook speed number, do not send invite with speed number, reject call with congestion tone*/
		if ( ( voicePhBook_p->flags & SPEED_DIAL_CONFIGURE) && ( *length == phoneBookLen ) ) {
			if ( memcmp(&(voicePhBook_p->speedNumber[0]), input,*length ) == 0 ) {
				return 3;/*whole match the speed dial rule, but not active, dial fail and congestion tone*/
			}
		}
		#endif
	}
	return 0;
}

#endif
/*______________________________________________________________________________
**  checkPoundKey
**
**  descriptions: check the Pound Key "#" at the Data mode.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
checkPoundKey(
	phoneCh_t* phoneCB,
	dialDigitBuf_t *dialDigitBuf,
	uint8 *length
){

	ZyXEL_VoiceCommon_t * tr104VoiceComm = tr104VoiceCommonObjPtrGet();
	ZyXEL_VoicePSTN_t *voicePstn_p=NULL;
	int index =0, phoneBookLen=0, pstnMatch=0, featureLen = 0;

	if ( tr104VoiceComm != NULL ) {
		PH_FSM_PRINTF("%s, pound enable = %d\n", __FUNCTION__, tr104VoiceComm->ZyXEL_pound);
		if ( (tr104VoiceComm->ZyXEL_pound != 0) && (*length != 0) ){
			if ( (char)(dialDigitBuf->digitStr) == '#' ) {
				/* 1. Remove-Pound could NOT work if the pound exists in the PSTN Table. */
				pstnMatch = 0;
				#ifdef PSTN_PHONEBOOK
				for ( index = 0; index < TR104_VPSTN_CNT; index++) {
					voicePstn_p = tr104VoicePSTNPtrGet(index);
					phoneBookLen =  zyStrlen(voicePstn_p->forcePSTNTab);
					if ( voicePstn_p->flags & PSTN_PHBOOK_ACTIVE) {
						if ( *length < phoneBookLen-1 ) {
						if ( ( voicePstn_p->forcePSTNTab[*length] == '#' ) && 
							  memcmp(&(voicePstn_p->forcePSTNTab[0]), &(phoneCB->dialInform.dialDigitNumber.dial_digits[0]), *length) == 0 ) 
						{
							/* forcePSTNTable has the '#' */
							pstnMatch = 1;
							break;
						}
						} else if ( *length == phoneBookLen ) {
							if ( memcmp(&(voicePstn_p->forcePSTNTab[0]), &(phoneCB->dialInform.dialDigitNumber.dial_digits[0]), *length) == 0 ) {
								/* dial number == forcePSTNTable + '#' */
								return 1;
							}
						}
					}/*voicePstn_p->flags & PSTN_PHBOOK_ACTIVE*/
				}
				#endif

				
				if (  pstnMatch == 0  ) {
					/* 3. Dial Immediately if the first dial digit is not '#' or '*' */
					if( (phoneCB->dialInform.dialDigitNumber.dial_digits[0] != '#') && (phoneCB->dialInform.dialDigitNumber.dial_digits[0] != '*') ){
						return 1;
					}
				}
			}
		}
	}
	return 0;

}

/*______________________________________________________________________________
**  processFeatureKey
**
**  descriptions: process the Feature Key at the Data mode.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
processFeatureKey(
	phoneCh_t* phoneCB,
	dialDigitBuf_t *dialDigitBuf
){
	phoneCh_t *nextPhoneCB=NULL;
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
#ifdef CALL_TRANSFER
	uint8 dialFinished = 0;			/*0:not finished, 1 finished*/
#endif	

	pLineFeature = getAssociatedLineFeatures(phoneCB);
	PH_FSM_PRINTF("%s, phoneFlags = 0x%x, dialDigitBuf = 0x%x\n", __FUNCTION__, phoneCB->phoneFlags, dialDigitBuf);
	/* OBM, SWISSCOM, Telefonica */
	if ( (phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) && ( phoneCB->phoneFlags & PHONE_PHYSICAL_FEATURE_KEY ) && 
		( dialDigitBuf != NULL ) && ( ( nextPhoneCB = getNextLogicPhone(phoneCB) ) != NULL ) ){
		
		timerStop(phoneCB->callProTimer.fxsFeatureKeyTimer);
		PH_FSM_DBG1("processFeatureKey: digitStr=", dialDigitBuf->digitStr);

		timerStop(phoneCB->callProTimer.dialDigitTimer);
		phoneCB->callProTimer.dialDigitTimer = 0;
		dialFinished = dialDigitsAdj(phoneCB, dialDigitBuf);

		switch ( nextPhoneCB->phStatus ) {
			case PHONE_STATUS_RING: {
					switch ( dialDigitBuf->digitStr) {
						case '0':		/* Reject the Waiting call */
							eventVoiceSimpleSend(EVT_CCM_REJ_REQ, MSG_SEND_TO_PHONE, nextPhoneCB->logicalId);
							dialFinished=1;
							break;
						case '1':		/* Accept the waiting call, clear active call */
							eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
							dialFinished=1;
							break;
						case '2':		/* Accept Waiting, hold active call */
							eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
							phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLDING;
							dialFinished=1;
							break;
						case '9':{		/* Danish DTFM Call waiting Caller-ID */
								ccmConn_t *ccmConn = NULL;
								ccmConn = findOrigCcmConn(nextPhoneCB->dspPortType, nextPhoneCB->logicalId);
								zyMemcpy(&(nextPhoneCB->dialInform.dialDigitNumber), &(ccmConn->peerCcmConn->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
								vDspDenmarkSendOffHookCID(nextPhoneCB);
							}
							break;
						default:
							break;
					}
				}
				break;
			case PHONE_STATUS_DATA: {
					switch ( dialDigitBuf->digitStr) {
						case '2':	/* Toggle between user B and user C */
							eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, phoneCB->dspPortType);
							phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLDING;
							dialFinished=1;
							break;
						case '0':	/* Clear held & Stay on active call */
							
								eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, nextPhoneCB->logicalId);							
														
							dialFinished=1;
							break;
						case '1': 	/* Clear active & Switch to held call */
							
								eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);							

							dialFinished=1;
							break;
					#ifdef CALL_CONFERENCE
					case '3':	/* Create Conference */
						if (pLineFeature->maxSessions > 0){
							ccmConn_t *ccmConn = NULL;
							ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
							if(ccmConn->ConfCallCB == NULL){
								eventVoiceCCMDataSend(EVT_CCM_CONFCALL_MEETROOM_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &nextPhoneCB->logicalId, sizeof(uint16));
							}
							else{								
								eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, nextPhoneCB->logicalId, nextPhoneCB->dspPortType);								
							}
						}
						else{
							PH_FSM_DBG("Call Conference disabled");
						}
						dialFinished=1;
						break;
					#endif
						default:							
							break;
					}
				}
				break;
			default:
				PH_FSM_DBG("\r\n wrong status");
				break;
		}			
	
		if ( dialFinished != 0){						
			if (getNumberingPlanPhoneFeatureIdBydialdigits(phoneCB->associatedVpItr,&(phoneCB->dialInform.dialDigitNumber.dial_digits[0]))==CALL_FEATURE_CALL_TRANSFER) {
				if(pLineFeature->callTransferEnable){
					PH_FSM_DBG("Attendant transfer");
					eventVoiceCCMDataSend(EVT_CCM_ATTEND_CALL_TRANS_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &(phoneCB->dialInform), sizeof(dialInfor_t));
				}
				else{
					PH_FSM_PRINTF("Call Transfer not enabled!! ignore Call Transfer feature key\n");
				}
			}
			
			
			phoneCB->callProTimer.fxsFeatureKeyTimer = 0;
			phoneCB->phoneFlags &= ~ PHONE_PHYSICAL_FEATURE_KEY;
		} else {
				VoiceProfNumPlanObj_t* pProfNumPlan = tr104VoiceProfNumPlanObjPtrGet(phoneCB->associatedVpItr);
				phoneCB->callProTimer.dialDigitTimer = timerStartS(pProfNumPlan->interDigitTimerStd, featureKeyTimeout, phoneCB);
		}

		if (phoneCB->dialInform.dialDigitNumber.current_length != 1){
			return;
		}


	}

	return;
}

/*______________________________________________________________________________
**  processFlashKey
**
**  descriptions: process the Flash Key at the Data mode.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static void processFlashKey(phoneCh_t* phoneCB) {
	VoiceProfNumPlanObj_t* pProfNumPlan = tr104VoiceProfNumPlanObjPtrGet(phoneCB->associatedVpItr);
	ZyXEL_VoiceCommon_t * pVoiceCommon = tr104VoiceCommonObjPtrGet();
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	phoneCh_t *nextPhoneCB = NULL;
	
	pLineFeature = getAssociatedLineFeatures(phoneCB);
	PH_FSM_DBG("processFlashKey");

	if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		if(NULL != pVoiceCommon){
			if(pVoiceCommon->dialMethod == CALL_FEATURE_DIAL_METHOD_EUROP){
				PH_FSM_DBG("processFlashKey DIAL_METHOD_EUROP");
				if ( checkLogicPhoneNum(phoneCB->physicalId)  < N_PHONE_LOGIC_NUM ) {
					/* 1. If this could create the new logic phone, change the status to onHold. And then create the new logical phone */
					eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
					phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLDING;			
				} else {
					/* 2. If the logical phoneCB could not be created, prepare to accept the feature Key */
					phoneCB->phoneFlags |= PHONE_PHYSICAL_FEATURE_KEY;
					phoneCB->callProTimer.fxsFeatureKeyTimer =  timerStartS(1000*(pProfNumPlan->ZyXEL_dialingLongInterval), featureKeyTimeout, phoneCB);
#ifdef CALL_TRANSFER			/* Add for attedant transfer 2005/05/05 */
					zyMemset(&(phoneCB->dialInform.dialDigitNumber), 0, sizeof(dialDigitNum_t));
#endif
				}
			}
			else if(pVoiceCommon->dialMethod == CALL_FEATURE_DIAL_METHOD_USA){
				PH_FSM_DBG("processFlashKey DIAL_METHOD_USA");
#ifdef CALL_CONFERENCE
				if ( (NULL != pLineFeature) && (pLineFeature->maxSessions > 0)){
					ccmConn_t *ccmConn = NULL;
					ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
					nextPhoneCB = getNextLogicPhone(phoneCB);
					if(NULL!= nextPhoneCB){						
						if(ccmConn->ConfCallCB == NULL ){
							if (nextPhoneCB->phStatus == PHONE_STATUS_DATA){
								PH_FSM_DBG("processFlashKey CONFCALL_MEETROOM");
								eventVoiceCCMDataSend(EVT_CCM_CONFCALL_MEETROOM_REQ, MSG_SEND_TO_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT, &nextPhoneCB->logicalId, sizeof(uint16));
								return;
							}
						}
						else{ /*Eason, 2009/07/07, to support hold/resume one call without remove conference*/
							if(nextPhoneCB->linkSt == PHONE_LINK_ST_ONHOLD){
								PH_FSM_DBG("processFlashKey resume conference, send EVT_CCM_UNHELD_REQ");
								eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, nextPhoneCB->logicalId, nextPhoneCB->dspPortType);
								phoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLDING;
								return;
							}
						}
						
					}					
				}else{
					PH_FSM_DBG("Call Conference disabled");
				}
#endif
				PH_FSM_DBG("processFlashKey ONHOLD");
				/* 1. Start to do the OnHold */
				eventVoiceCCMSimpleSend(EVT_CCM_ONHOLD_REQ, ORIG_FUNC_CCMCONN, phoneCB->logicalId, DSP_FXS_PORT);
				phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLDING;
				
			}/*pVoiceCommon->dialMethod*/
			else{
				PH_FSM_DBG("Error !! Unknown  dialMethod Type");
			}
		}/*NULL != pVoiceCommon*/
	}
	
	return;
}

/*______________________________________________________________________________
**	featueKeyTimeout
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
static int
featureKeyTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;

	PH_FSM_DBG("featureKeyTimeout");
	tmpPhoneCB->callProTimer.fxsFeatureKeyTimer= 0;
	tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_FEATURE_KEY;

#ifdef FXO_DSPRELAY
	eventVoiceCCMSimpleSend(EVT_PHONE_FLASH, MSG_SEND_TO_CCMCONN, tmpPhoneCB->logicalId, DSP_FXS_PORT);
#endif

	return 0;
} /* featueKeyTimeout */

/*______________________________________________________________________________
**  getCallWaitingInterval
**
**  descriptions: get ZyXEL_Call_Waiting_Reject_Time from VoiceProfLineCallingFeaturesObj
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static uint8
getCallWaitingInterval(
	phoneCh_t* phoneCB
){
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = getAssociatedLineFeatures(phoneCB);

	return (pLineFeature->ZyXEL_Call_Waiting_Reject_Time);
}

/*______________________________________________________________________________
**	stopCallerID
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
void 
stopCallerID( 
	phoneCh_t *phoneCB
) {
#ifdef ZyNOS
#if 1
	vDSPStopCIDFSK(phoneCB);
#else
#endif

	vDspChanClose(phoneCB);
		phSetSLICStatus(phoneCB->physicalId, FORWARD_ACTIVE);
#endif
	return;
}

/* The below: Support Denish DTMF Call Waiting Caller-ID */
/*______________________________________________________________________________
**  vDspDenamrkSendOffHookCID
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
static int
vDspDenmarkSendOffHookCID(
	phoneCh_t *phoneCB
) {
	VoicePhyIntfObj_t*pPhyIntf;
	uint16 callerIDInterval = 0, startCIDInterval = 0;
	uint8 done = 0;
#ifdef ZyNOS
#if (SUPPORT_VDSP != VDSP_DANUBE)
	pPhyIntf = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	if(pPhyIntf->ZyXEL_CallerIDPayload==DTMF_CID){
		startCIDInterval = pPhyIntf->ZyXEL_FirstRingTimoutInterval;
		callerIDInterval = startCIDInterval + (phoneCB->dialInform.dialDigitNumber.current_length+2)*140+210;
		phoneCB->callProTimer.fxsCallerIDStartTimer = timerStartS(startCIDInterval, &callerIDDTMFStartTimer, (void*)phoneCB);
		phoneCB->callProTimer.fxsCallerIDStopTimer = timerStartS(callerIDInterval, &DenmarkOffHookCallerIDStopTimer, (void*)phoneCB);
		done = 1;
	}
#endif
#endif
	return done;
}

/*______________________________________________________________________________
**  checkNextWaitingCallCCM
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
static uint16
checkNextWaitingCallCCM(
	uint16 chId
){
	phoneCh_t *phoneCB=NULL;
	phoneCh_t *nextPhoneCB=NULL;

	phoneCB=find_phoneCB(chId,0);
	if(phoneCB!=NULL){
		nextPhoneCB=getNextLogicPhone(phoneCB);
		if(nextPhoneCB!=NULL){
				return nextPhoneCB->logicalId;;
		}
	}
	return 0;
}

#ifdef VISUAL_MWI
/*______________________________________________________________________________
**	generateVMWI
**
**	descriptions: call low level API to generate/remove VMWI from phone.
**	parameters: Tcid: the physical phone id, enable: True to switch on the icon on phone. False to switch off.
**	local:
**	global:
**	return:
**	called by: functions in phfsm that want to turn it on/off
**	call:
**	revision:
**____________________________________________________________________________*/
static int
generateVMWI(uint16 chId, boolean enable){
	/* 
	The generateVMWI is supposed to call the correct VMWI generation function in lower layer API.
	When generating VMWI, the device should:
	
	1. Do a OSI (Open Switching Interval) for about 150 milliseconds.
	2. Turn on the the power again (Set slic to On Hook Transmission)
	3. Fill in the VMWI FSK information, and send it to the phone.
	*/
	if(enable) {
		dbg_pline_1("VMWI isn't implemented in lower layer API. If it was, then the phone's VMWI icon would be activated now\r\n");
	} else {
		dbg_pline_1("VMWI isn't implemented in lower layer API. If it was, then the phone's VMWI icon would be de-activated now\r\n");
	}
	
	return 0;
}
#endif

#ifdef NORWAY_VOIP_CUSTOMIZATION
static boolean isTelenorNorwayCongestionToneCase(uint16 rspcode)
{
	switch(rspcode){
		case 404:
		case 480:
		case 484:
			return 1;
		default:
			return 0;
	}		
}				
#endif
