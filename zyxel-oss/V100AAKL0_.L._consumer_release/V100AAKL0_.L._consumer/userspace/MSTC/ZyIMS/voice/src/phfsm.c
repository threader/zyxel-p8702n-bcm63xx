#include <stdio.h>
#include <sys/socket.h>
#include "switch.h"
#include "global.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "Voipccm_sip_itf.h"
#include "icf_types.h"
#include "voiceApp_main.h"
#include "dsp_itf.h"
#include "tr104.h"
#include "callConf.h"
#if CCBS_SUPPORT
#include "webDial_fsm.h"
#endif
/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/
int phoneInit(void);
void phonePortInit(void);
static int checkFlashRingTimeout (tm_id_t timer_id, void *arg);

/*____________________________________________________________________________*/
/*	data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/
//phoneCb_t phoneCb;
voicePhoneCh_t voicePhoneCh;
int phoneDebug=0;
int zyxelTE = 0;
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
autoTest_t VoIP_autoTest;
#endif

uint16 logicPhoneIndex = 0;
#ifdef TERMINAL_PORTABILITY
int isTerminalPortabilitySupport = 0;
#endif
#if CCBS_SUPPORT
extern webRepeatDial_t *webRepeatDial;
#endif


/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/
/*______________________________________________________________________________
**  create_phoneCB
**
**  descriptions: Create the new phoneCB.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
static phoneCh_t*
create_phoneCB(
	uint16 physicalId,
	uint8 PortType
) {
	phoneCh_t *phoneCB=NULL, *tmpPhoneCB=NULL, **hp = NULL;
	int i_state;

	if ( ( phoneCB = (phoneCh_t *)calloc( 1, sizeof(phoneCh_t) ) ) == NULL ) {
		ZyPrintf("alloc voiceDspCh_t[%d] error\n\r", physicalId);
		return NULL;
	}

	/* Assign the phoneCB->logicalId */
	if ( ( logicPhoneIndex >= LOGIC_PHONE_LOWBOUND ) &&  ( logicPhoneIndex < LOGIC_PHONE_UPBOUND ) ) {
		phoneCB->logicalId = logicPhoneIndex;
	} else {
		phoneCB->logicalId = LOGIC_PHONE_LOWBOUND;
		logicPhoneIndex = LOGIC_PHONE_LOWBOUND;
	}
	logicPhoneIndex += 1;

	/* Assign the phoneCB->physicalId */
	if ( PortType == ZYXEL_PHONE_PORT ){
		phoneCB->physicalId = phoneCB->logicalId;
	} else {
		phoneCB->physicalId = physicalId;
	}
	
	/* Assing other phoneCB values */
	phoneCB->callId = 0;
	phoneCB->associatedVpItr= 0;
	phoneCB->associatedSipAccountId= -1;
	phoneCB->phStatus = PHONE_STATUS_IDLE;
	phoneCB->linkSt = PHONE_LINK_ST_IDLE;
	phoneCB->peerMediaChId = 0;
	phoneCB->dspPortType = PortType;
	phoneCB->phoneFlags = 0;
	phoneCB->LinePolarity = LINE_POLARITY_FORWARD;	
	zyMemset(&(phoneCB->dialInform), 0, sizeof(dialInfor_t));
	zyMemset(&(phoneCB->callProTimer), 0, sizeof(callProTimer_t) );
	phoneCB->localConfPeerLocalPhLogicalId = 0;

	/* find the position to bind the link list: physicalId (small->large). Then logicalId (small->large) */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( phoneCB->physicalId < tmpPhoneCB->physicalId ) {
				tmpPhoneCB = tmpPhoneCB->prev;
				break;
			} else if ( phoneCB->physicalId == tmpPhoneCB->physicalId ) {
				if ( phoneCB->logicalId < tmpPhoneCB->logicalId ) {
					tmpPhoneCB = tmpPhoneCB->prev;
					break;
				}
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}

	/* bind this entry to the header of the link list */
	if ( tmpPhoneCB == NULL ) {
		hp = &(voicePhoneCh.phoneChEntry);
		i_state = dirps();
		phoneCB->prev = NULL;
		phoneCB->next = *hp;
		if(phoneCB->next != NULL)
			phoneCB->next->prev = phoneCB;
		*hp = phoneCB;
		restore(i_state);
	} else {
		i_state = dirps();
		phoneCB->next = tmpPhoneCB->next;
		phoneCB->prev = tmpPhoneCB;
		if(phoneCB->prev != NULL)
			phoneCB->prev->next = phoneCB;
		if(phoneCB->next != NULL)
			phoneCB->next->prev = phoneCB;
		restore(i_state);
	}

	/* Correct the total phoneCB numbers */
	voicePhoneCh.phoneChNum += 1;

	return phoneCB;
}

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  callCtrlInit
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
int phoneInit(void) {
	int rCode;
	int PhyIdx=0,FxoIdx=0;
	#ifdef VISUAL_MWI	
	voicePhoneCh.mwiChangeStatus=zyMalloc(NUM_FXS_CHANNELS);	
	#endif
	#ifdef CALL_RETURN_SUPPORT
	callHistoryLastCall=zyMalloc(sizeof(CallHistoryLastCall_t) * NUM_FXS_CHANNELS);
	#endif
	#if CCBS_SUPPORT		
	webRepeatDial = zyMalloc(sizeof(webRepeatDial_t) * NUM_FXS_CHANNELS);
	#endif
	phonePortInit();
	rCode = phoneIpcInit();
	return 0;
}

/*______________________________________________________________________________
**  phonePortInit
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
phonePortInit(
	void
){
	int i=0;
	uint32 NumOfFxo = 0, NumOfPhyIntf = 0;
	phoneCh_t *tmpPhoneCB=NULL;

	NumOfFxo = tr104GetNumOfFXO();
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	/* Initialize the phone resource */
	voicePhoneCh.phoneChNum = 0;
	voicePhoneCh.phoneChEntry = NULL;
	ZyPrintf("phonePortInit NumOfPhyIntf = %d, NumOfFxo = %d\n", NumOfPhyIntf, NumOfFxo);

	/* Create the FXS PhoneCB */
	for ( i=0; i<NumOfPhyIntf; i++ ){
		VoicePhyIntfObj_t *phyInterface = tr104VoicePhyIntfObjPtrGet(i);
			
		tmpPhoneCB = create_phoneCB(i, DSP_FXS_PORT);
		tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
		tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
		if ( create_ccmConn(tmpPhoneCB->dspPortType, tmpPhoneCB->logicalId, ORIG_FUNC_PHONE, phyInterface->ZyXEL_LineSelect) == NULL ) {
#ifdef ZyNOS
			ERRLOG(ERR_INFO, ERRLOG_INFO_FXS_INIT_FAIL, NULL, 0);
#endif
		}
	}

	for ( i=0; i<NumOfFxo; i++ ){
		ZyXEL_VoiceFXO_t *ZyXEL_VoiceFXO = tr104VoiceFXOPtrGet(i);
		tmpPhoneCB = create_phoneCB(i, DSP_FXO_PORT);
		tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
		tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
		if ( create_ccmConn(tmpPhoneCB->dspPortType, tmpPhoneCB->logicalId, ORIG_FUNC_PHONE, ZyXEL_VoiceFXO->fxoPort->fxoPhoneSelect) == NULL ) {
#ifdef ZyNOS
			ERRLOG(ERR_INFO, ERRLOG_INFO_FXS_INIT_FAIL, NULL, 0);
#endif
		}
	}

	return;
}

/*______________________________________________________________________________
**  find_phoneCB
**
**  descriptions: Find the phoneCB by logicalId or physicalId.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
phoneCh_t*
find_phoneCB(
	uint16 chanId,
	uint16 type			/* 0: logicalId, 1:physicalId */
) {
	phoneCh_t *tmpPhoneCB=NULL;
	uint32 NumOfFxo = 0, NumOfPhyIntf = 0;

	
	NumOfFxo = tr104GetNumOfFXO();
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		if ( type == 0 ) {
			if ( ( chanId >= LOGIC_PHONE_LOWBOUND ) && ( chanId < LOGIC_PHONE_UPBOUND ) ) {
				while ( tmpPhoneCB != NULL ) {
					if ( ( tmpPhoneCB->logicalId == chanId ) ) {
						return tmpPhoneCB;
					}
					tmpPhoneCB = tmpPhoneCB->next;
				}
			}
		} else if ( type == 1 ) {
			if ( ( chanId >= 0 ) && ( chanId < (NumOfFxo + NumOfPhyIntf) ) ) {
				while ( tmpPhoneCB != NULL ) {
					if ( ( tmpPhoneCB->physicalId == chanId ) && ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) ){
						return tmpPhoneCB;
					} else if ( tmpPhoneCB->physicalId > chanId ) {
						return NULL;
					}
					tmpPhoneCB = tmpPhoneCB->next;
				}
			}
		}
	}

	return NULL;
}

/*______________________________________________________________________________
**  find_phonePhyId
**
**  descriptions: Find the PhoneCB->physicalId by the logicalID. (CCM/Phone Use)
**  parameters:
**  local:
**  global:
**  return: 0: Not found.  Others: (phoneCB->physcialId + 1).
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
uint16
find_phonePhyId(
	uint16 logicalId
) {
	phoneCh_t *tmpPhoneCB=NULL;

	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		if ( ( logicalId >= LOGIC_PHONE_LOWBOUND ) && ( logicalId < LOGIC_PHONE_UPBOUND ) ) {
			while ( tmpPhoneCB != NULL ) {
				if ( ( tmpPhoneCB->logicalId == logicalId ) ) {
					return (tmpPhoneCB->physicalId+1);
				}
				tmpPhoneCB = tmpPhoneCB->next;
			}
		}
	}

	return 0;
}


/*______________________________________________________________________________
**	reset_PhoneCB
**
**	descriptions: Reset the specific phoneCB.
**                       Note: ccmConn_p = The phoneCB's CCM.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
reset_PhoneCB(
	phoneCh_t *phoneCB
) {
	ccmConn_t *ccmConn = NULL;

	if ( phoneCB != NULL ) {
		/* Reset phoneCB vaules */
		PH_FSM_DBG("reset_PhoneCB()");
		phoneCB->callId = 0;
		phoneCB->associatedVpItr= 0;
		phoneCB->associatedSipAccountId= -1;
		phoneCB->peerMediaChId = 0;
		zyMemset(&(phoneCB->dialInform), 0, sizeof(dialInfor_t));
		phoneCB->localConfPeerLocalPhLogicalId = 0;

		/* stop all timers of the phoneCB */
		if ( phoneCB->callProTimer.firstDigitTimer != 0) {
			timerStop(phoneCB->callProTimer.firstDigitTimer);
			phoneCB->callProTimer.firstDigitTimer = 0;
		}
		if ( phoneCB->callProTimer.dialDigitTimer != 0 ) {
			timerStop(phoneCB->callProTimer.dialDigitTimer);
			phoneCB->callProTimer.dialDigitTimer = 0;
		}
		if ( phoneCB->callProTimer.connConfTimer != 0) {
			timerStop(phoneCB->callProTimer.connConfTimer);
			phoneCB->callProTimer.connConfTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDStartTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDStartTimer);
			phoneCB->callProTimer.fxsCallerIDStartTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDStopTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
			phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDRingTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDRingTimer);
			phoneCB->callProTimer.fxsCallerIDRingTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDTACSecTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDTACSecTimer);
			phoneCB->callProTimer.fxsCallerIDTACSecTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsflashRingTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsflashRingTimer);
			phoneCB->callProTimer.fxsflashRingTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsFeatureKeyTimer != 0 ){
			timerStop(phoneCB->callProTimer.fxsFeatureKeyTimer);
			phoneCB->callProTimer.fxsFeatureKeyTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsNonHookDetectTimer != 0 ) {
			timerStop(phoneCB->callProTimer.fxsNonHookDetectTimer);
			phoneCB->callProTimer.fxsNonHookDetectTimer = 0;
		}
		if ( phoneCB->callProTimer.fxoDialDigitTimer1 != 0) {
			timerStop(phoneCB->callProTimer.fxoDialDigitTimer1);
			phoneCB->callProTimer.fxoDialDigitTimer1 = 0;
		}
		if ( phoneCB->callProTimer.fxoDialDigitTimer2 != 0) {
			timerPeriodicStop(phoneCB->callProTimer.fxoDialDigitTimer2);
			phoneCB->callProTimer.fxoDialDigitTimer2 = 0;
		}
		if ( phoneCB->callProTimer.fxoConnCheckTimer != 0) {
			timerStop(phoneCB->callProTimer.fxoConnCheckTimer);
			phoneCB->callProTimer.fxoConnCheckTimer = 0;
		}
		if ( phoneCB->callProTimer.fxoNonFlashCheckTimer != 0) {
			timerStop(phoneCB->callProTimer.fxoNonFlashCheckTimer);
			phoneCB->callProTimer.fxoNonFlashCheckTimer = 0;
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
		if ( phoneCB->callProTimer.fxsMWIStutterToneTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsMWIStutterToneTimer);
			phoneCB->callProTimer.fxsMWIStutterToneTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsToneStartTimer!= 0) {
			timerStop(phoneCB->callProTimer.fxsToneStartTimer);
			phoneCB->callProTimer.fxsToneStartTimer = 0;
		}
 		if ( phoneCB->callProTimer.fxsRingStartTimer!= 0) {
			timerStop(phoneCB->callProTimer.fxsRingStartTimer);
			phoneCB->callProTimer.fxsRingStartTimer = 0;
		}

		#ifdef HOWLER_TONE
		if ( phoneCB->callProTimer.fxsOffhookHowlerToneTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsOffhookHowlerToneTimer);
			phoneCB->callProTimer.fxsOffhookHowlerToneTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsHowlerTonePeriodicTimer != 0) {
			timerPeriodicStop(phoneCB->callProTimer.fxsHowlerTonePeriodicTimer);
			phoneCB->callProTimer.fxsHowlerTonePeriodicTimer = 0;
		}
		#endif

		#ifdef TERMINAL_PORTABILITY
        if ( phoneCB->callProTimer.fxsTerminalPortabilityTimer != 0) {
            timerStop(phoneCB->callProTimer.fxsTerminalPortabilityTimer);
            phoneCB->callProTimer.fxsTerminalPortabilityTimer = 0;
        }
		#endif
	phoneCB->limit_tone.Times=0;

		//phStopTone(phoneCB);  /* move to delLogicPhone() by Jacky 2008/11/14 */
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		voipLedOnOff(phoneCB->physicalId);
	}

	return;
}

/*______________________________________________________________________________
**  create_ZyPhone
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
ccmConn_t *
create_ZyPhone(
	ccmConn_t* ccmConn_p,
	int phoneFeature
){
	phoneCh_t *tmpPhoneCB = NULL, *origTmpPhoneCB = NULL;
	ccmConn_t *tmpCcmConn_p = NULL;
	PH_FSM_DBG("create_ZyPhone()");
	if ( ( tmpPhoneCB = create_phoneCB(LOGIC_PHONE_LOWBOUND, ZYXEL_PHONE_PORT) ) != NULL ) {
		tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
		 /* other Phonefeature need CCM struct */
		if ( ccmConn_p != NULL ) {
			/* If ccmConn_p is not NULL, create ZyPhone to replace the the ccmConn_p call control */
			if ( ccmConn_p->origPortType == DSP_FXS_PORT ) {
				origTmpPhoneCB = find_phoneCB(ccmConn_p->origPortId, 0);
				if ( origTmpPhoneCB != NULL ) {
					if(phoneFeature<CALL_FEATURE_NUMBERS){
						/* most phone config only use this case */
						tmpPhoneCB->peerMediaChId = origTmpPhoneCB->peerMediaChId;
						zyMemcpy(&(tmpPhoneCB->dialInform.dialDigitNumber), &(origTmpPhoneCB->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
						tmpPhoneCB->phoneFlags |= PHONE_PHONE_CONFIG;
						tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE;
					}else{
					tmpPhoneCB->phStatus = origTmpPhoneCB->phStatus;
					tmpPhoneCB->linkSt = origTmpPhoneCB->linkSt;
					tmpPhoneCB->peerMediaChId = origTmpPhoneCB->peerMediaChId;
					tmpPhoneCB->phoneFlags = origTmpPhoneCB->phoneFlags;
					zyMemcpy(&(tmpPhoneCB->dialInform.dialDigitNumber), &(origTmpPhoneCB->dialInform.dialDigitNumber), sizeof(dialDigitNum_t) );
					tmpPhoneCB->dialInform.prevLogicId = 0;
					}
				}
			}else if ( ccmConn_p->origPortType == SIP_PORT ){			
				if(phoneFeature==CALL_FEATURE_ENABLE_DND)
					tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE; /* if create_Zyphone is called by associateSip2ZyPhone,then we don't need the phoneflag!!*/
			} else {
				CCM_FSM_DBG1("Warn: Not implement the ZyPhone replace for ccmType =", ccmConn_p->origPortType);
			}
		}
		
		/*For Specific phone feature*/
		switch(phoneFeature){
			case CALL_FEATURE_IVR:
				/* IVR not need use ccm */
				tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE;
				tmpPhoneCB->phoneFlags |= PHONE_IVR_EMBEDDED;
				break;
			case CALL_FEATURE_PARK:
			case CALL_FEATURE_PARK_RETRIEVE:
				/*  Call pickup not need use CCM , only change phone status */
				tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
				tmpPhoneCB->linkSt = PHONE_LINK_ST_CONNECT;
       			tmpPhoneCB->phStatus = PHONE_STATUS_DIAL;
				break;
			default:
				PH_FSM_DBG("There were no special phone feature need to be treated!");
				break;
		}
		
		
		if ( ( tmpCcmConn_p = create_ccmConn(tmpPhoneCB->dspPortType, tmpPhoneCB->logicalId, ORIG_FUNC_PHONE, 3)  ) == NULL ) {
			ZyPrintf("\rcreate_ZyPhone: create_ccmConn fail!!\n");
			/*ERRLOG(ERR_INFO, ERRLOG_INFO_ZYPHONE_CREATE_FAIL, NULL, 0);*/			
		}
		
	}else {
			ZyPrintf("\rcreate_ZyPhone create_phoneCB fail!!\n");
		/*ERRLOG(ERR_INFO, ERRLOG_INFO_ZYPHONE_CREATE_FAIL, NULL, 0);*/		
	}
	return tmpCcmConn_p;
}

/*______________________________________________________________________________
**  checkLogicPhoneNum
**
**  descriptions: check all the logical phoneCB with the specific physicalId.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
checkLogicPhoneNum(
	uint16 physicalId
) {
	int number = 0;
	phoneCh_t *tmpPhoneCB=NULL;

	/* check the existed logical phone channels for this physical phone */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( physicalId < tmpPhoneCB->physicalId ) {
				break;
			} else if ( physicalId == tmpPhoneCB->physicalId ) {
				number += 1;
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}

	return number;
}

/*______________________________________________________________________________
**  createLogicPhone
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
//Ssu-Ying, support Multiple Profile
ccmConn_t*
createLogicPhoneSwap(
	uint16	physicalId, 
	uint16	phStatus,
	uint16	linkStatus
) {
	int number = 0;
	phoneCh_t *tmpPhoneCB=NULL, *newPhoneCB=NULL;
	ccmConn_t *newCcmConn_p=NULL;
	VoicePhyIntfObj_t *phyInterface = tr104VoicePhyIntfObjPtrGet(physicalId);

	/* check if this physical channel could create the new phoneCB */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( physicalId < tmpPhoneCB->physicalId ) {
				break;
			} else if ( physicalId == tmpPhoneCB->physicalId ) {
				number += 1;
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}

	if ( number < N_PHONE_LOGIC_NUM ) {
		/* Create new phoneCB for usage */
		newPhoneCB = create_phoneCB(physicalId, DSP_FXS_PORT);
		if ( newPhoneCB != NULL ) {
			newPhoneCB->phStatus = phStatus;
			newPhoneCB->linkSt = linkStatus;
			newPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE;
			if ( ( newCcmConn_p = create_ccmConn(newPhoneCB->dspPortType, newPhoneCB->logicalId, ORIG_FUNC_PHONE, phyInterface->ZyXEL_LineSelect) )== NULL ) {
				ZyPrintf("create_ccmConn() error for logicPhoneSwap() \n\r");
			}
		}
	}

	return newCcmConn_p;
}

/*______________________________________________________________________________
**  delLogicPhone
**
**  descriptions: Change to other phoneCB with the same physicalID.
**                       If other phoneCB existed, delete the original phoneCB.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
void
delLogicPhone(
	phoneCh_t *phoneCB,
	ccmConn_t *ccmConn
) {
	int number = 0;
	phoneCh_t *tmpPhoneCB=NULL, *headPhoneCB=NULL;
	PH_FSM_DBG1("delLogicPhone logicalId = ", phoneCB->logicalId);
	/* check the existed logical phone channels for this physical phone */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( phoneCB->physicalId < tmpPhoneCB->physicalId ) {
				break;
			} else if ( phoneCB->physicalId == tmpPhoneCB->physicalId ) {
				number += 1;
				if ( number == 1 ) {
					headPhoneCB = tmpPhoneCB;
				}
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}	

	PH_FSM_DBG1("      delLogicPhone(), number = ", number);
	if ( number == 1 ) {
		if ( phoneCB != NULL ) {
			/* Reset all phoneCB->phoneFlags */ 
			/* Because the other vaules are reset at fxsFsmIdle()-EVT_PHONE_FXS_ONHOOK except the phoneFlags */
			phStopTone(phoneCB); /* move from reset_PhoneCB() by Jacky 2008/11/14 */
			phStopRing(phoneCB); /* add by Jacky 2007/12/21 */
			phoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE; /*by Eason, reset phone flags*/
		}
	} else if ( number > 1 ) {
		/* Change to use the next existed phoneCB as the USAGE channel */
		if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
			tmpPhoneCB = NULL;
			if ( phoneCB->next != NULL ) {
				if ( phoneCB->next->physicalId == phoneCB->physicalId ) {
					tmpPhoneCB = phoneCB->next;
				} else if ( headPhoneCB != NULL ) {
					tmpPhoneCB = headPhoneCB;
				}
			} else if ( headPhoneCB != NULL )
				tmpPhoneCB = headPhoneCB;

			if ( tmpPhoneCB != NULL ) {
				tmpPhoneCB->phoneFlags &= ~(PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD);
				tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
				tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
				tmpPhoneCB->LinePolarity = LINE_POLARITY_FORWARD;

				phoneCB->phoneFlags &= ~PHONE_PHYSICAL_USAGE;
				PH_FSM_DBG1("      next phoneCB->phStatus = ", tmpPhoneCB->phStatus);
				if (( tmpPhoneCB->phStatus == PHONE_STATUS_IDLE ) || (tmpPhoneCB->phStatus == PHONE_STATUS_DISC) ) {
					/* OnHook the other channel directly */
					eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
				} else if ( tmpPhoneCB->phStatus == PHONE_STATUS_DATA ) {
					/* Check if the phone is in the onHook status. If yes, use Ring to remind the user */
					if ( phHookStatus(phoneCB) == PHONE_HOOK_ST_ON ) {
						if((NULL == ccmConn->ConfCallCB) ||
							(CONF_CALL_ST_ONE_CALL_ONHOLD == ccmConn->ConfCallCB->confcallStatus)){
							PH_FSM_PRINTF("%s, UNHOLD_RING\n",__FUNCTION__);
							tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLD_RING;
							phStartRing(tmpPhoneCB);
							#if 1 /*Jason ,[SPRID # 090812816] */
							tmpPhoneCB->callProTimer.fxsflashRingTimer = timerStartS(TIMEOUT_FLASH_RING_CHECK, checkFlashRingTimeout, tmpPhoneCB);
							#endif
						}
						else{
							if(NULL == ccmConn->ConfCallCB){
								PH_FSM_PRINTF("%s, ConfCallCB NULL\n",__FUNCTION__);
							}
							else{
								PH_FSM_PRINTF("%s, ConfCallCB confcallStatus %d\n",__FUNCTION__, ccmConn->ConfCallCB->confcallStatus);
							}
						}
					}	
					
					if ( phHookStatus(phoneCB) == PHONE_HOOK_ST_OFF ) {						
						tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLDING;
						eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, tmpPhoneCB->logicalId, tmpPhoneCB->dspPortType);
					}
				} else if ( tmpPhoneCB->phStatus == PHONE_STATUS_RING ) {
					/* Check if the phone is in the onHook status. If yes, use Ring to remind the user */
					if ( phHookStatus(phoneCB) == PHONE_HOOK_ST_ON ) {
						PH_FSM_PRINTF("UNHOLD_RING flag turns on to remind user\n");
						tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLD_RING;
						phStartRing(tmpPhoneCB);
						tmpPhoneCB->callProTimer.fxsflashRingTimer = timerStartS(TIMEOUT_FLASH_RING_CHECK, checkFlashRingTimeout, tmpPhoneCB);
					} else {
						eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
					}
				} else {		
					ZyPrintf("delLogicPhone(): Unknow phStatus=%d\n\r", tmpPhoneCB->phStatus);
				}

				/* Remove the phoneCB */
				delete_PhoneCB(phoneCB, ccmConn);
			}
		} else {
			delete_PhoneCB(phoneCB, ccmConn);
		}
	}

	return;
}

/*______________________________________________________________________________
**	checkFlashRingTimeout
**
**	descriptions: If the user onHook the phone, use ring to remind him other existed phoneCB. But it has the timeout.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
checkFlashRingTimeout (
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;

	PH_FSM_DBG("checkFlashRingTimeout");
	if ( tmpPhoneCB != NULL ) {
		if ( tmpPhoneCB->phoneFlags & PHONE_PHYSICAL_UNHOLD_RING ) {
			phStopRing(tmpPhoneCB);
			tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_UNHOLD_RING;
			eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
		}
	}
	
	return 0;
} /* dialDigitsTimeout */

#if 0 /* replaced by voipLedOnOff(), Jacky */
/*______________________________________________________________________________
**	AdjustPhoneLEDBySipMap
**
**	descriptions: Check if Phone LED Should be On.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
AdjustPhoneLEDBySipMap(
	int physicalId,
	uint32 sipSelect
){
#ifdef ZyNOS
	int i, outgoingcount;
	phoneCh_t *phoneCB;
	ccmConn_t *ccmConn = NULL;

	outgoingcount = 0;
	for ( i=0; i<SP_VSIGNAL_CNT; i++ ){
		if((sipSelect & 0x01) && (voipGetRegisStatusBySipIndex(i) == 1)){
			outgoingcount ++;
		}
		sipSelect >>= 0x01;
	}
	if(!outgoingcount){
		}
#endif
	return;
}/* AdjustPhoneLEDBySipMap */
#endif

/*______________________________________________________________________________
**	delete_PhoneCB
**
**	descriptions: Delete the specific phoneCB.
**                       Note: ccmConn_p = The phoneCB's CCM.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
delete_PhoneCB(
	phoneCh_t *phoneCB, 
	ccmConn_t *ccmConn_p
) {
	int i_state;

	if ( phoneCB != NULL ) {
		/* Remove the phoneCB from the voicePhoneCB link list */
		i_state = dirps();
		if ( phoneCB->prev != NULL ) {
			phoneCB->prev->next = phoneCB->next;
		} else {
			voicePhoneCh.phoneChEntry = phoneCB->next;
		}
		if ( phoneCB->next != NULL ) {
			phoneCB->next->prev = phoneCB->prev;
		}
		phoneCB->next = NULL;
		phoneCB->prev = NULL;
		restore(i_state);

		/* Correct the total phoneCB numbers */
		voicePhoneCh.phoneChNum -= 1;
		PH_FSM_DBG1("phoneChNum",voicePhoneCh.phoneChNum);

		/* stop all timers of the phoneCB */
		if ( phoneCB->callProTimer.firstDigitTimer != 0) {
			timerStop(phoneCB->callProTimer.firstDigitTimer);
			phoneCB->callProTimer.firstDigitTimer = 0;
		}
		if ( phoneCB->callProTimer.dialDigitTimer != 0 ) {
			timerStop(phoneCB->callProTimer.dialDigitTimer);
			phoneCB->callProTimer.dialDigitTimer = 0;
		}
		if ( phoneCB->callProTimer.connConfTimer != 0) {
			timerStop(phoneCB->callProTimer.connConfTimer);
			phoneCB->callProTimer.connConfTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDStartTimer!= 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDStartTimer);
			phoneCB->callProTimer.fxsCallerIDStartTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDStopTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDStopTimer);
			phoneCB->callProTimer.fxsCallerIDStopTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDRingTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDRingTimer);
			phoneCB->callProTimer.fxsCallerIDRingTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsCallerIDTACSecTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsCallerIDTACSecTimer);
			phoneCB->callProTimer.fxsCallerIDTACSecTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsflashRingTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsflashRingTimer);
			phoneCB->callProTimer.fxsflashRingTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsFeatureKeyTimer != 0 ){
			timerStop(phoneCB->callProTimer.fxsFeatureKeyTimer);
			phoneCB->callProTimer.fxsFeatureKeyTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsNonHookDetectTimer != 0 ) {
			timerStop(phoneCB->callProTimer.fxsNonHookDetectTimer);
			phoneCB->callProTimer.fxsNonHookDetectTimer = 0;
		}
		if ( phoneCB->callProTimer.fxoDialDigitTimer1 != 0) {
			timerStop(phoneCB->callProTimer.fxoDialDigitTimer1);
			phoneCB->callProTimer.fxoDialDigitTimer1 = 0;
		}
		if ( phoneCB->callProTimer.fxoDialDigitTimer2 != 0) {
			timerPeriodicStop(phoneCB->callProTimer.fxoDialDigitTimer2);
			phoneCB->callProTimer.fxoDialDigitTimer2 = 0;
		}
		if ( phoneCB->callProTimer.fxoConnCheckTimer != 0) {
			timerStop(phoneCB->callProTimer.fxoConnCheckTimer);
			phoneCB->callProTimer.fxoConnCheckTimer = 0;
		}
		if ( phoneCB->callProTimer.fxoNonFlashCheckTimer != 0) {
			timerStop(phoneCB->callProTimer.fxoNonFlashCheckTimer);
			phoneCB->callProTimer.fxoNonFlashCheckTimer = 0;
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
		if ( phoneCB->callProTimer.fxsMWIStutterToneTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsMWIStutterToneTimer);
			phoneCB->callProTimer.fxsMWIStutterToneTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsToneStartTimer!= 0) {
			timerStop(phoneCB->callProTimer.fxsToneStartTimer);
			phoneCB->callProTimer.fxsToneStartTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsRingStartTimer!= 0) {
			timerStop(phoneCB->callProTimer.fxsRingStartTimer);
			phoneCB->callProTimer.fxsRingStartTimer = 0;
		}
		#ifdef HOWLER_TONE
		if ( phoneCB->callProTimer.fxsOffhookHowlerToneTimer != 0) {
			timerStop(phoneCB->callProTimer.fxsOffhookHowlerToneTimer);
			phoneCB->callProTimer.fxsOffhookHowlerToneTimer = 0;
		}
		if ( phoneCB->callProTimer.fxsHowlerTonePeriodicTimer != 0) {
			timerPeriodicStop(phoneCB->callProTimer.fxsHowlerTonePeriodicTimer);
			phoneCB->callProTimer.fxsHowlerTonePeriodicTimer = 0;
		}
		#endif
		
		#ifdef TERMINAL_PORTABILITY
        if ( phoneCB->callProTimer.fxsTerminalPortabilityTimer != 0) {
            timerStop(phoneCB->callProTimer.fxsTerminalPortabilityTimer);
            phoneCB->callProTimer.fxsTerminalPortabilityTimer = 0;
        }
		#endif
#if (SUPPORT_SLIC== SLIC_PEF4268)
		phStopTone(phoneCB);
#endif
		/* Free the phoneCB */
		zyFree(phoneCB);
		phoneCB = NULL;

		/* Delete the relative CCM */
		delete_ccmConn(ccmConn_p->origPortType, ccmConn_p->origPortId);
	}

	return;
}

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**	delete_AllPhoneCB
**
**	descriptions: Delete all phoneCB.
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
delete_AllPhoneCB(void) {
	int i_state;
	phoneCh_t *delphoneCB = NULL, *delNextphoneCB = NULL;
	ccmConn_t *ccmConn=NULL;

	PH_FSM_DBG("delete_AllPhoneCB > ENTER");
	
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		delphoneCB = voicePhoneCh.phoneChEntry;
		while ( delphoneCB != NULL ) {
			SIP_FSM_DBG("voicePhoneCh.phoneChEntry != NULL");
			delNextphoneCB = delphoneCB->next;
			ccmConn = findOrigCcmConn(delphoneCB->dspPortType, delphoneCB->logicalId);
			delete_PhoneCB(delphoneCB, ccmConn);
			delphoneCB = delNextphoneCB;						
		}
		zyFree(delphoneCB);
		delphoneCB = NULL;
	}	
	PH_FSM_DBG("delete_AllPhoneCB end");
	return;
}
/*______________________________________________________________________________
**  enterLogicPhone
**
**  descriptions:  Action for the ongoing phoneCB.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
enterLogicPhone(
	phoneCh_t *phoneCB,
	ccmConn_t *ccmConn
) {
	/* Change to play the correct tone/ring or open the DSP channel */
	if ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) {
		switch (phoneCB->phStatus) {
			case PHONE_STATUS_IDLE:
				if ( phoneCB->linkSt == PHONE_LINK_ST_IDLE ) {
					   	
					eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				break;
			case PHONE_STATUS_DIAL:
				 if ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ) {
				 	phStartRingBackTone(phoneCB);
				 }
				break;
			case PHONE_STATUS_RING:
				if ( phoneCB->linkSt == PHONE_LINK_ST_INCOMING ){
					eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
				break;
			case PHONE_STATUS_DATA:
				if ( phoneCB->linkSt == PHONE_LINK_ST_CONNECT ){
					if ( ccmConn->peerCcmConn != NULL ) {
						if ( ccmConn->peerCcmConn->origPortType != DSP_FXO_LIFELINE )  {
#if defined(EMBEDDED_IVR) && defined(MUSIC_ON_HOLD)
							if (voipGetMusicOnHoldActive(ccmConn->peerCcmConn->origPortId)){
								#ifdef MULTIPLE_DSP
									vDspChanOpen(phoneCB,-1);
								#else
									vDspChanOpen(phoneCB);
								#endif
							} else
#endif
							#ifdef MULTIPLE_DSP
								if (vDspChanOpen(phoneCB, -1) != 0)
							#else
								if (vDspChanOpen(phoneCB) != 0) 
							#endif
								{
									ZyPrintf("vDspChanOpen error\n");
								}
						}
					}
				}
				break;
			case PHONE_STATUS_DISC:
				phStartBusyTone(phoneCB);
				break;
			default:
				PH_FSM_DBG1("enterLogicPhone(), unknow phStatus =", phoneCB->phStatus);
				break;
		}
	}

	return 0;
}

/*______________________________________________________________________________
**  getNextLogicPhone
**
**  descriptions: get the next logic Phone with the same physicalId
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
phoneCh_t*
getNextLogicPhone(
	phoneCh_t* phoneCB
) {
	int number = 0;
	phoneCh_t *tmpPhoneCB=NULL, *headPhoneCB=NULL;

	/* check the existed logical phone channels for this physical phone */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( phoneCB->physicalId < tmpPhoneCB->physicalId ) {
				break;
			} else if ( phoneCB->physicalId == tmpPhoneCB->physicalId ) {
				number += 1;
				if ( number == 1 ) {
					headPhoneCB = tmpPhoneCB;
				}
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}

	tmpPhoneCB = NULL;
	if ( number > 1 ) {
		if ( phoneCB->next != NULL ) {
			if ( phoneCB->next->physicalId == phoneCB->physicalId ) {
				tmpPhoneCB = phoneCB->next;
			} else if ( headPhoneCB != NULL ) {
				tmpPhoneCB = headPhoneCB;
			}
		} else if ( headPhoneCB != NULL )
			tmpPhoneCB = headPhoneCB;
	}

	return tmpPhoneCB;
}

/*______________________________________________________________________________
**  getAssociatedLineObj
**
**  descriptions: get line obj from associated sipaccount ID
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
VoiceProfLineObj_t*
getAssociatedLineObj(
	phoneCh_t* phoneCB
){
	VoiceProfLineObj_t* pLine = NULL;
	ccmConn_t *ccmConn = NULL;
	int index = 0;
	int16 SipAccountId = 0, VpItr=0 , LineItr = 0;
	uint32 NumOfSipAccount = 0;
	if(phoneCB->associatedSipAccountId>= 0){
		VpItr = mapSipAccountIdToVpItr(phoneCB->associatedSipAccountId);
		LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, VpItr = %d\n", VpItr);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, LineItr = %d\n", LineItr);
		pLine = tr104VoiceLineObjPtrGet(VpItr , LineItr);
	}
	if(pLine == NULL){
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if(ccmConn != NULL){
			NumOfSipAccount = tr104GetMaxLine();
			for ( SipAccountId=0; SipAccountId<NumOfSipAccount; SipAccountId++ ) {
				if(ccmConn->SipFxsMap & (1<<SipAccountId)) {
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);					
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, SipAccountId = %d\n", SipAccountId);
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, VpItr = %d\n", VpItr);
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, LineItr = %d\n", LineItr);
					#if 1 /*Eason, return enabled and mapped line*/
					if(pLine->enable){ /*return enabled and mapped line*/
						return pLine;
					}
					#endif
				}
			}			
		}
	}	
	return pLine;
}


/*______________________________________________________________________________
**  getAssociatedRegLineObj
**
**  descriptions: get line obj from associated sipaccount ID
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
VoiceProfLineObj_t*
getAssociatedRegLineObj(
	phoneCh_t* phoneCB
){
	VoiceProfLineObj_t* pLine = NULL, * pEnabledLine = NULL;
	ccmConn_t *ccmConn = NULL;
	int index = 0;
	int16 SipAccountId = 0, VpItr=0 , LineItr = 0;
	uint32 NumOfSipAccount = 0;
	if(phoneCB->associatedSipAccountId>= 0){
		VpItr = mapSipAccountIdToVpItr(phoneCB->associatedSipAccountId);
		LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, VpItr = %d\n", VpItr);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, LineItr = %d\n", LineItr);
		pLine = tr104VoiceLineObjPtrGet(VpItr , LineItr);
	}
	if(pLine == NULL){
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if(ccmConn != NULL){
			NumOfSipAccount = tr104GetMaxLine();
			for ( SipAccountId=0; SipAccountId<NumOfSipAccount; SipAccountId++ ) {
				if(ccmConn->SipFxsMap & (1<<SipAccountId)) {
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, SipAccountId = %d\n", SipAccountId);
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, VpItr = %d\n", VpItr);
					VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, LineItr = %d\n", LineItr);

					if(pLine->enable){
						
						if(pLine->status == TR104I_STATUS_UP){ /*return enabled and mapped line*/
							return pLine;
						}

						if(NULL == pEnabledLine){ /*return enabled line if no registered line found*/
							pEnabledLine = pLine;
						}
					}
					
				}
			}			
		}
	}

	if(NULL != pLine){
		return pLine;
	}
	else{
		return pEnabledLine;
	}
	
}


/*______________________________________________________________________________
**  getAssociatedProfileObj
**
**  descriptions: get profile obj from associated sipaccount ID
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
VoiceProfObj_t*
getAssociatedProfileObj(
	phoneCh_t* phoneCB
){
	VoiceProfObj_t* pProf = NULL;
	ccmConn_t *ccmConn = NULL;
	int16 VpItr=0;
	uint32 NumOfProfile = 0;
	int16 SipAccountId = 0;
	uint32 NumOfSipAccount = 0;
	if(phoneCB->associatedSipAccountId>= 0){
		VpItr = mapSipAccountIdToVpItr(phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
		VOICEAPP_PRINTF("getAssociatedLineObj > ENTER, VpItr = %d\n", VpItr);
		pProf = tr104VoiceProfObjPtrGet(VpItr);
	}
	if(pProf == NULL){
#if 0
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if(ccmConn != NULL){
			NumOfProfile = tr104GetNumProfile();
			for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
				pProf = tr104VoiceProfObjPtrGet(VpItr);
				return pProf;				
			}
		}
#else	/* DSL-2492HNU-B1B, [SPR 110406341] */
		SipAccountId = getAssociateRegedLineId(phoneCB);
		if(SipAccountId == -1){
			VOICEAPP_PRINTF("getAssociatedProfileObj: can't find reged line id, set to 0 \n");
			SipAccountId = 0;
		}
		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		VOICEAPP_PRINTF("getAssociatedProfileObj: SipAccountId = %d\n", SipAccountId);
		VOICEAPP_PRINTF("getAssociatedProfileObj: ENTER, VpItr = %d\n", VpItr);
		pProf = tr104VoiceProfObjPtrGet(VpItr);
#endif
	}
	return pProf;
}

/*______________________________________________________________________________
**  getAssociatedLineFeatures
**
**  descriptions: get line calling feature obj from associated lind ID
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
VoiceProfLineCallingFeaturesObj_t*
getAssociatedLineFeatures(
	phoneCh_t* phoneCB
){
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	ccmConn_t *ccmConn = NULL;
	int16 SipAccountId = 0, VpItr = 0, LineItr = 0;
	uint32 NumOfSipAccount = 0;

	LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
	pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(phoneCB->associatedVpItr, LineItr);

	if(pLineFeature == NULL){
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if(ccmConn != NULL){
			NumOfSipAccount = tr104GetMaxLine();
			for ( SipAccountId=0; SipAccountId<NumOfSipAccount; SipAccountId++ ) {
				if(ccmConn->SipFxsMap & (1<<SipAccountId)) {
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
					return pLineFeature;
				}		
			}
		}
	}
	return pLineFeature;
}

/*______________________________________________________________________________
**  getAssociateRegedLineId
**
**  descriptions: get associated registered lind ID
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
getAssociateRegedLineId(
	phoneCh_t* phoneCB
){
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	ccmConn_t *ccmConn = NULL, *tmpCcmConn = NULL;;
	int sipLineId = 0;
	sipCCMConvertEntry_t * tmpConvertEntry_p = NULL;
	
	ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);

	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId);
		if ( tmpConvertEntry_p != NULL ) {
			VOICEAPP_PRINTF("getAssociateRegedLineId > ENTER, SipAccountId = %d\n", sipLineId);
			sipLineId = tmpConvertEntry_p->SipAccountId;
			if ( ccmConn->SipFxsMap & (1<<sipLineId) ) {
/* A special case to supporte internal-call under SIP unregistered status for hardware C2 test.
KeyYang@MSTC, 20120907 */
#ifdef MSTC_UNREG_INTERNAL_CALL
				return sipLineId;
#else
				if ( tmpCcmConn->flags & CALL_CONN_FLAG_REGISTER ) {
					return sipLineId;
				}
#endif /*MSTC_UNREG_INTERNAL_CALL*/
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	return -1;
}


/*______________________________________________________________________________
**  logicPhoneSwap
**
**  descriptions: Use in the Flash Key to swith between two logical channels.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
logicPhoneSwap(
	phoneCh_t *phoneCB,
	ccmConn_t *ccmConn
) {
	int number = 0;
	phoneCh_t *tmpPhoneCB=NULL, *newPhoneCB=NULL, *headPhoneCB=NULL;
	ccmConn_t *newccmConn = NULL, *tmpCcmConn = NULL;
	VoicePhyIntfObj_t *phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);


	/* check if this physical channel could create the new phoneCB */
	if ( voicePhoneCh.phoneChEntry != NULL ) {
		tmpPhoneCB = voicePhoneCh.phoneChEntry;
		while ( tmpPhoneCB != NULL ) {
			if ( phoneCB->physicalId < tmpPhoneCB->physicalId ) {
				break;
			} else if ( phoneCB->physicalId == tmpPhoneCB->physicalId ) {
				number += 1;
				if ( number == 1 ) {
					headPhoneCB = tmpPhoneCB;
				}
			}
			if ( tmpPhoneCB ->next != NULL ) {
				tmpPhoneCB = tmpPhoneCB->next;
			} else {
				break;
			}
		}
	}	
	
	if ( number < N_PHONE_LOGIC_NUM ) {
		/* Create new phoneCB and CCM for usage */
		newPhoneCB = create_phoneCB(phoneCB->physicalId, phoneCB->dspPortType);
		if ( newPhoneCB != NULL ) {
			newPhoneCB->dialInform.prevLogicId = phoneCB->logicalId;
			newPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
			/* change the old physical channel to logical channel */
			phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_USAGE|PHONE_PHYSICAL_ONHOLDING);
			phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLD;
			phoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
			phoneCB->LinePolarity = LINE_POLARITY_REVERSE;
			newPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_REVERSE;

			newccmConn=create_ccmConn(newPhoneCB->dspPortType, newPhoneCB->logicalId, ORIG_FUNC_PHONE, phyInterface->ZyXEL_LineSelect);
			if (newccmConn == NULL) {
				ZyPrintf("create_ccmConn() error for logicPhoneSwap() \n\r");
			}
			
			/* Enter the new logical channel */
			enterLogicPhone(newPhoneCB, ccmConn);
		}
	} else {
		/* Change to the next existed phoneCB*/
		tmpPhoneCB = NULL;
		if ( phoneCB->next != NULL ) {
			if ( phoneCB->next->physicalId == phoneCB->physicalId ) {
				tmpPhoneCB = phoneCB->next;
			} else if ( headPhoneCB != NULL ) {
				tmpPhoneCB = headPhoneCB;
			}
		} else  if ( headPhoneCB != NULL ) {
			tmpPhoneCB = headPhoneCB;
		}

		/* Send event to unHold the tmpPhoneCB */
		if ( tmpPhoneCB != NULL ) {
			/* Swap the usage phoneCB */
			phoneCB->phoneFlags &= ~(PHONE_PHYSICAL_USAGE|PHONE_PHYSICAL_ONHOLDING);
			phoneCB->phoneFlags |= PHONE_PHYSICAL_ONHOLD;
			phoneCB->phoneFlags |= PHONE_PHYSICAL_REVERSE;
			phoneCB->LinePolarity = LINE_POLARITY_REVERSE;
#if 0 /* Jacky, moved below*/
			tmpPhoneCB->phoneFlags &= ~(PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD);
#endif
			tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_USAGE;
			tmpPhoneCB->phoneFlags &= ~PHONE_PHYSICAL_REVERSE;
			
			tmpCcmConn = findOrigCcmConn(tmpPhoneCB->dspPortType, tmpPhoneCB->logicalId);
			if ( tmpCcmConn != NULL ) {
				tmpPhoneCB->peerMediaChId = tmpCcmConn->peerCcmConn->origPortId;
			}

			if ( tmpPhoneCB->phStatus == PHONE_STATUS_DATA ) {				
				if(tmpPhoneCB->phoneFlags & (PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD)){ /*Eason, don't send UNHELD if not in ONHOLD state*/
					tmpPhoneCB->phoneFlags &= ~(PHONE_PHYSICAL_ONHOLDING|PHONE_PHYSICAL_ONHOLD);
					tmpPhoneCB->phoneFlags |= PHONE_PHYSICAL_UNHOLDING;
					eventVoiceCCMSimpleSend(EVT_CCM_UNHELD_REQ, MSG_SEND_TO_CCMCONN, tmpPhoneCB->logicalId, tmpPhoneCB->dspPortType);				
				}
			}else if ( tmpPhoneCB->phStatus == PHONE_STATUS_RING ){
			#ifdef MULTIPLE_DSP
				vDspChanOpen(tmpPhoneCB, -1);
			#else
				vDspChanOpen(tmpPhoneCB);
			#endif
				eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, tmpPhoneCB->logicalId);
			} else {
				enterLogicPhone( tmpPhoneCB, NULL );
				ZyPrintf("logicPhoneSwap(): Unknow phStatus=%d\n\r", tmpPhoneCB->phStatus);
			}
		}
	}

	return 0;
}
#if 0
int
checkInternalPhoneFeature(
	char *dialDigits,
	uint8 *length
) {
	int ret = CALL_FEATURE_NUMBERS;

#ifdef EMBEDDED_IVR

	if ( (zyStrlen(dialDigits) >= zyStrlen(callFeatureKey[CALL_FEATURE_IVR])) && (zyStrlen(callFeatureKey[CALL_FEATURE_IVR]) > 0) ) {
		if ( strncmp(dialDigits, callFeatureKey[CALL_FEATURE_IVR], zyStrlen(callFeatureKey[CALL_FEATURE_IVR]) ) == 0 ) {
			ret = CALL_FEATURE_IVR;
		}
	}
#endif

	if ( (zyStrlen(dialDigits) >= zyStrlen(callFeatureKey[CALL_FEATURE_INTERNAL_CALL])) && (zyStrlen(callFeatureKey[CALL_FEATURE_INTERNAL_CALL]) > 0) ) {
		if ( strncmp(dialDigits, callFeatureKey[CALL_FEATURE_INTERNAL_CALL],  zyStrlen(callFeatureKey[CALL_FEATURE_INTERNAL_CALL]) ) == 0 ) {
			ret = CALL_FEATURE_INTERNAL_CALL;
		}
	}

	return ret;
}
#endif

/*______________________________________________________________________________
**	phoneFsmProc
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
int phoneFsmProc(evtMsg_t *msg) {
	int status = 0;
	uint16 logicalId = msg->chann_id;
	phoneCh_t *phoneCB=NULL;
	uint32 NumOfFxo = 0, NumOfPhyIntf = 0;
	
	NumOfFxo = tr104GetNumOfFXO();
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	if( (phoneCB=find_phoneCB(logicalId, 0)) == NULL){
		dbg_plinel_1("\r\n wrong logicId at phoneFsmProc: ", logicalId);
		dbg_plinel_1("\r\n Event: ", msg->event);
		return -1;
	}
/*Jason , syslog*/
#ifdef VOIP_SYSLOG
		fxsPhoneEventLog(msg, phoneCB);
#endif	

	if(( phoneCB->dspPortType == DSP_FXS_PORT )||( phoneCB->dspPortType ==DSP_DECT_PORT)){/* FXS and DECT*/
		if ( ( phoneCB->physicalId < 0 ) || ( phoneCB->physicalId >= NumOfPhyIntf ) ) {
			ZyPrintf("wrong physicalId[%d] at phoneFsmProc (FXS)\n\r", phoneCB->physicalId );
			return -1;
		}
		switch( phoneCB->phStatus ) {
			case PHONE_STATUS_IDLE:
				status = fxsFsmIdle(msg, phoneCB);
				break;
			case PHONE_STATUS_DIAL:
				status = fxsFsmDial(msg, phoneCB);
				break;
			case PHONE_STATUS_RING:
				status = fxsFsmRing(msg, phoneCB);
				break;
			case PHONE_STATUS_DATA:
				status = fxsFsmData(msg, phoneCB);
				break;
			case PHONE_STATUS_DROP:
				status = fxsFsmDrop(msg, phoneCB);
				break;
			case PHONE_STATUS_DISC:
				status = fxsFsmDisc(msg, phoneCB);
				break;
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
			case PHONE_STATUS_TEST:
				status = fxsFsmTest(msg, phoneCB);
				break;
#endif
			default:
				ZyPrintf("undefined fxsPhSt =%d\n\r", phoneCB->phStatus);
				break;
		}
#ifdef FXO_DSPRELAY
	}else	if(( phoneCB->dspPortType == DSP_FXO_PORT )){/* FXS and DECT*/
		if ( ( phoneCB->physicalId < NumOfPhyIntf ) || ( phoneCB->physicalId >= (NumOfPhyIntf + NumOfFxo)) ) {
			ZyPrintf("wrong physicalId[%d] at phoneFsmProc (FXO)\n\r", phoneCB->physicalId );
			return -1;
		}
		switch( phoneCB->phStatus ) {
			case PHONE_STATUS_IDLE:
				status = fxoFsmIdle(msg, phoneCB);
				break;
			case PHONE_STATUS_DIAL:
				status = fxoFsmDial(msg, phoneCB);
				break;
			case PHONE_STATUS_RING:
				status = fxoFsmRing(msg, phoneCB);
				break;
			case PHONE_STATUS_DATA:
				status = fxoFsmData(msg, phoneCB);
				break;
			case PHONE_STATUS_DROP:
				status = fxoFsmDrop(msg, phoneCB);
				break;
			case PHONE_STATUS_DISC:
				status = fxoFsmDisc(msg, phoneCB);
				break;
			default:
				ZyPrintf("undefined fxsPhSt =%d\n\r", phoneCB->phStatus);
				break;
		}
#endif
	} else if ( phoneCB->dspPortType == ZYXEL_PHONE_PORT ) {		/* ZYXEL_PHONE */
		if ( ( phoneCB->physicalId < LOGIC_PHONE_LOWBOUND ) || ( phoneCB->physicalId >= LOGIC_PHONE_UPBOUND ) ) {
			ZyPrintf("wrong physicalId[%d] at phoneFsmProc (ZYXEL_PHONE)\n\r", phoneCB->physicalId);
			return -1;
		}
		switch( phoneCB->phStatus ) {
			case PHONE_STATUS_IDLE:
				status = ZyXELPhoneFsmIdle(msg, phoneCB);
				break;
			case PHONE_STATUS_DIAL:
				status = ZyXELPhoneFsmDial(msg, phoneCB);
				break;
			case PHONE_STATUS_RING:
				status = 0;
				break;
			case PHONE_STATUS_DATA:
				status = ZyXELPhoneFsmData(msg, phoneCB);
				break;
			default:
				ZyPrintf("undefined ZyPhone Status =%d\n\r", phoneCB->phStatus);
				break;
		}
	} else {
		ZyPrintf("phoneCh->dspPortType=%d is not defined. And its logicId=%d\n\r", phoneCB->dspPortType, logicalId);
	}

	return status;
}
