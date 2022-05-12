#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "switch.h"
#include "call_mgnt.h"
#include "callTrans.h"
#include "Voipccm_sip_itf.h"
#include "phfsm.h"
#include "voiceApp_iptk_api.h"
#include "tr104.h"
/*Jaosn , syslog*/
#ifdef VOIP_SYSLOG
#include "syslog.h"
#include "voip_syslog.h"
#include <sys/time.h>
#endif

/*sipAccountNumber is a global variable in TR104 Module which is stored the total sip account number */
#include "tr104_init.h"
extern uint32 sipAccountNumber;	

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%	data declaration
%%____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/
sipCCMConvert_t sipCCMConvert;
static int sipCcmPortId = SIP_CCM_CONVERT_PORTID_LOWBOUND;
int sipDebug = 0;

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**	sipEvtToCCM
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
sipEvtToCCM(
	int SipAccountId, 
	int callId, 
	int eventType, 
	void *msg,
	int length
){
	sipCCMConvertEntry_t *tmpEntry = NULL;
#ifdef MULTI_SIP_SESSION
	sipCCMConvertEntry_t *tmpNewEntry = NULL;
	ccmConn_t* newCcmConn_p = NULL;
#endif

	SIP_FSM_DBG1("sipEvtToCCM, event =", eventType);
	SIP_FSM_DBG2("               SipAccountId=", SipAccountId, ", callId=", callId);

	if ( (eventType == EVT_CCM_ASSIGN_REQ) || (eventType == EVT_CCM_SIPTOPSTN)
#ifdef VISUAL_MWI
	|| (eventType == EVT_CCM_NEW_VOICE_MESSAGE) || (eventType == EVT_CCM_NO_NEW_VOICE_MESSAGE) 
#endif
	) {
		tmpEntry = changeSipCCMConvert(SipAccountId, callId, SIP_CCM_CONVERT_ST_CALLEE);
	} else if  ( ( eventType == EVT_CCM_UNREG_SUCC ) || ( eventType == EVT_CCM_REG_SUCC ) ) {
		tmpEntry = findPermanentSipCCMConvertByLineId(SipAccountId);
	} else {
		tmpEntry = findSipCCMConvertByCallId(callId);
	}
	
	if ( tmpEntry != NULL ) {
		if ( msg == NULL ) {
			eventVoiceCCMSimpleSend(eventType, MSG_SEND_TO_CCMCONN, tmpEntry->sipCcmPortId, SIP_PORT);
		} else {
			eventVoiceCCMDataSend(eventType, MSG_SEND_TO_CCMCONN, tmpEntry->sipCcmPortId, SIP_PORT,msg,length);
		}

		if ( eventType == EVT_CCM_DISC_ACK ) {
#ifdef MULTI_SIP_SESSION
			SIP_FSM_DBG1("sipEvtToCCM, tmpEntry->flags =", tmpEntry->flags);
			if ( tmpEntry->flags == SIP_CCM_TABLE_PERMANENT ) {
				resetSipCCMConvert(tmpEntry->SipAccountId, callId, tmpEntry->RoleSt);
			} else {
				deleteSipCCMConvert(tmpEntry->SipAccountId, callId, tmpEntry->RoleSt, SIP_CCM_TABLE_DELETE_CHECK);
			}
#else
			resetSipCCMConvert(tmpEntry->SipAccountId, callId, tmpEntry->RoleSt);
#endif
		}
	} else {
#ifdef MULTI_SIP_SESSION

	       if ( ( eventType == EVT_CCM_ASSIGN_REQ ) || ( eventType == EVT_CCM_ATTEND_CALL_TRANS_REPLACE )  || (eventType == EVT_CCM_CALL_TRANS_IND)
#ifdef VISUAL_MWI
		|| ( eventType == EVT_CCM_NEW_VOICE_MESSAGE) || ( eventType == EVT_CCM_NO_NEW_VOICE_MESSAGE )
#endif
		) {
			SIP_FSM_DBG2("sipEvtToCCM - MULTI_SIP_SESSION: SipAccountId=", SipAccountId, "callId=", callId);
			newCcmConn_p = addSipCCMConvert(SIP_CCM_CONVERT_ST_CALLEE, SIP_CCM_TABLE_TEMPORARY, SipAccountId, callId);
			if(newCcmConn_p != NULL){
				tmpNewEntry = findSipCCMConvertByCallId(callId);
				if ( msg == NULL ) {
					eventVoiceCCMSimpleSend(eventType, MSG_SEND_TO_CCMCONN, tmpNewEntry->sipCcmPortId, SIP_PORT);
				} else {
					eventVoiceCCMDataSend(eventType, MSG_SEND_TO_CCMCONN, tmpNewEntry->sipCcmPortId, SIP_PORT,msg,length);
				}
			} else {
				SIP_FSM_DBG("sipEvtToCCM(): addSipCCMConvert fail");
			}
		} else {
			SIP_FSM_DBG("sipEvtToCCM(): Not found the tmpEntry if the tmpEntry does not exist.");
		}
#else
		dbg_pline_1("sipEvtToCCM(): Not found the tmpEntry\n\r");
#endif
	}

	return;
}


/*______________________________________________________________________________
**	CCMEvtToSip
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
CCMEvtToSip(
evtMsg_t *msg
){
	uint16 event = msg->event;
	uint16 portId = msg->chann_id;
	ccmConn_t *ccmConn = NULL;
	sipCCMConvertEntry_t *tmpEntry_p = NULL;
	uint16 VpItr = 0, LineItr = 0;
	uint16 SipAccountId = 0;
	uint16 callId = 0;
	bool	cidflag=0;
	phoneCh_t *phoneCB = NULL;	/* aircheng add */
	uint16 phyId = 0;				/* aircheng add */
		
	SIP_FSM_DBG1("CCMEvtToSip, event =", event);
	SIP_FSM_DBG1("               portId=", portId);

	tmpEntry_p = findSipCCMConvertByPortId(portId);
	if ( tmpEntry_p == NULL ) {
		SIP_FSM_DBG("voiceAppCCMMsgProcess: chann_id is wrong!");
		return RET_FAILURE;
	} else {
		VpItr = tmpEntry_p->VpItr;
		SipAccountId = tmpEntry_p->SipAccountId;
		SIP_FSM_DBG1(" CCMEvtToSip> ENTER , SipAccountId %d\n", SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);
		callId = tmpEntry_p->callId;
	}
	
	/*proc event*/
	switch(event) {
		case EVT_CCM_ASSIGN_CONF:
			break;
		case EVT_CCM_REJ_REQ:
		case EVT_CCM_ASSIGN_FAIL:
			#if 1 /*Jason , [SPRID # 090716208]*/
#if 1 /*Jennifer, Anonymous Call Block*/
			voiceAppTerminateCall(callId, SipAccountId, ICF_CALL_CLEAR_REASON_USER_BUSY,486);  /*ICF_CALL_CLEAR_REASON_USER_BUSY may be also ok */
#else
			voiceAppTerminateCall(callId, SipAccountId, ICF_CALL_CLEAR_REASON_USER_BUSY);  /*ICF_CALL_CLEAR_REASON_USER_BUSY may be also ok */
#endif
			#else
			voiceAppTerminateCall(callId, lineId, ICF_CALL_CLEAR_REASON_LINE_BUSY);  /*ICF_CALL_CLEAR_REASON_USER_BUSY may be also ok */			
			#endif
			break;
		case EVT_CCM_DIAL_REQ: {
				VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
				
				ccmConn = findOrigCcmConn(SIP_PORT, portId);
				if ( ccmConn == NULL ) {
					SIP_FSM_DBG("ccmConn is not found");
					return RET_FAILURE;
				}
				if((pLine != NULL) && (pProfSip != NULL)) {
					/*Jason ,[SPRID # 090622474]*/
					if ( strcmp((char *)ccmConn->dialInform.dialDigitNumber.dial_digits, (char *)pLine->directoryNumber) == 0 ) {
						SIP_FSM_DBG("Call calls itself");
						sipEvtToCCM(-1, callId, EVT_CCM_REJ_IND, NULL, 0);
					} else {
						voiceAppCreateCallwithDialflag(callId, SipAccountId, VpItr,ccmConn->dialInform.dialDigitNumber.dial_digits,ccmConn->dialInform.dialDigitNumber.dialDigitFlags);
					}					
				} else {
					/*Jason ,[SPRID # 090622474]*/
					voiceAppCreateCallwithDialflag(callId, SipAccountId, VpItr,ccmConn->dialInform.dialDigitNumber.dial_digits,ccmConn->dialInform.dialDigitNumber.dialDigitFlags);								
				}
			
			}
			break;
		case EVT_CCM_ALERT_REQ: {
				uint16 earlyMediaSuccess = RET_FAILURE;
				VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
				if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE){
					if( RET_SUCCESS == vDspStartEarlyMedia(callId, pLineCallingFeatures->ZyXEL_remoteRingbackToneIndex) ){	/* aircheng modify*/
						voiceAppMediaConnect(callId);
						earlyMediaSuccess = RET_SUCCESS;
					}
				}
				voiceAppLocalAlerted(callId, SipAccountId, earlyMediaSuccess);
			}
			break;
		case EVT_CCM_CONN_REQ: {
				VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
				if(pLineCallingFeatures->ZyXEL_remoteRingbackTone == TR104I_REMOTE_RINGBACK_ACTIVE){
					vDspStopEarlyMedia(callId, pLineCallingFeatures->ZyXEL_remoteRingbackToneIndex);	/* aircheng modify*/
				}
				voiceAppConnect(callId);
			}
			break;
		case EVT_CCM_CONN_ACK:
			break;
		case EVT_CCM_REL_REQ:
		case EVT_CCM_DISC_REQ:
			voiceAppTerminateCall(callId, SipAccountId, ICF_CALL_CLEAR_REASON_UNDEFINED);
			break;
		case EVT_CCM_DISC_ACK:
			ccmDiscAckToSIP(SipAccountId,callId);
			break;
		case EVT_CCM_FAX_RELAY:
			voiceAppStartFAX(callId, SipAccountId, ICF_FAX_PT_CALL);
			break;
		case EVT_CCM_FAX_T38:
			voiceAppStartFAX(callId, SipAccountId, ICF_T38_FAX_CALL);
			break;
		case EVT_CCM_ONHOLD_REQ:
			voiceAppCallHold(callId);
 			break;
		case EVT_CCM_UNHELD_REQ:{
#if 1
				/* aircheng add */
				VoiceProfLineCallingFeaturesObj_t* pLineCallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
				if((pLineCallingFeatures != NULL) && (pLineCallingFeatures->ZyXEL_musicOnHoldTone == TR104I_MUSIC_ONHOLD_ACTIVE)){
					ccmConn = findOrigCcmConn(SIP_PORT, portId);
					if((ccmConn != NULL) && (ccmConn->peerCcmConn != NULL)) {
						phoneCB = find_phoneCB(ccmConn->peerCcmConn->origPortId, 0);
						if (phoneCB != NULL){
							phyId = phoneCB->physicalId;
							vDspStopMusicOnHold(callId, phyId, pLineCallingFeatures->ZyXEL_musicOnHoldToneIndex);
						}
					}
				}
#endif				
			voiceAppCallResume(callId);
			}
			break;
		case EVT_CCM_DIAL_DIGIT: {
				dialInfor_t *dialInform = NULL;
				dialInform = (dialInfor_t *)msg->data_p;
				voiceAppInfo(callId,&(dialInform->dialDigitNumber.dial_digits[0]));
			}
			break;
		case EVT_CCM_UNATTEND_CALL_TRANS_REQ:{
				VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				char transferTargetUrl[300] = {"\0"};

				ccmConn = findOrigCcmConn(SIP_PORT, portId);
				if(ccmConn->callTransferCB != NULL) {
					if((pLine != NULL) && (pProfSip != NULL)) {
						sprintf(transferTargetUrl, "%s@%s", ccmConn->callTransferCB->dialDigitNumber.dial_digits, pProfSip->userAgentDomain);
						voiceAppUnAttendedTransfer(callId, SipAccountId, transferTargetUrl);
					} else {
						sprintf(transferTargetUrl, "%s", (char *)msg->data_p);
						voiceAppUnAttendedTransfer(callId, SipAccountId, transferTargetUrl);
					}
				}
			
			}
			break;
		case EVT_CCM_ATTEND_CALL_TRANS_REQ:{
				uint16 portIdC = 0, callIdC = 0;
				sipCCMConvertEntry_t *tmpEntry_p = NULL;
				
				/*if(msg->data_p != NULL) {*/
				if(msg->data_size !=  0 ){
					portIdC = *((uint16 *)msg->data_p);

					tmpEntry_p = findSipCCMConvertByPortId(portIdC);
					if ( tmpEntry_p == NULL ) {
					SIP_FSM_DBG("EVT_CCM_ATTEND_CALL_TRANS_REQ : PortId is wrong!");
						return RET_FAILURE;
					} else {
						callIdC = tmpEntry_p->callId;
					}

					SIP_FSM_DBG2("EVT_CCM_ATTEND_CALL_TRANS_REQ callId B = ", callId, ", callId C = ", callIdC);
					voiceAppAttendedTransfer(callId, callIdC);
				}
			
			}
			break;
		case EVT_CCM_CONFERENCE_IND:{
				uint16 portIdC = 0, callIdC = 0;
				sipCCMConvertEntry_t *tmpEntry_p = NULL;
				SIP_FSM_DBG("EVT_CCM_CONFERENCE_IND");
				
				/*if(msg->data_p != NULL) {*/
				if(msg->data_size !=  0 ){
					portIdC = *((uint16 *)msg->data_p);

					tmpEntry_p = findSipCCMConvertByPortId(portIdC);
					if ( tmpEntry_p == NULL ) {
					SIP_FSM_DBG("EVT_CCM_CONFERENCE_IND : PortId is wrong!");
						return RET_FAILURE;
					} else {
						callIdC = tmpEntry_p->callId;
					}

					SIP_FSM_DBG2("EVT_CCM_CONFERENCE_IND callId B = ", callId, "callId C = ", callIdC);
					voiceAppConference(callIdC, callId);
				}
			
			}
			break;
		default:
			break;
	}
	return RET_SUCCESS;
}

/*______________________________________________________________________________
**	initSipCCMConvert
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
void initSipCCMConvert() {
	int SipAccountId;
	
	sipCCMConvert.sipCCMConvertNum = 0;
	sipCCMConvert.sipCCMConvertEntry = NULL;

	for ( SipAccountId=0; SipAccountId < sipAccountNumber; SipAccountId++) {
		SIP_FSM_DBG1("sipAccountNumber = ", sipAccountNumber);
		addSipCCMConvert(SIP_CCM_CONVERT_ST_IDLE, SIP_CCM_TABLE_PERMANENT, SipAccountId, 0);
	}

	return;
}

/*______________________________________________________________________________
**	addSipCCMConvert
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
ccmConn_t* 
addSipCCMConvert(
	int	RoleStatus,
	int	flags,
	int	SipAccountId,
	int	callId
) {
	sipCCMConvertEntry_t *sipCCMConvertEntry_p = NULL, *tmpEntry_p = NULL, **hp=NULL;
	int i_state;
	uint16 LineItr = 0, VpItr=0;

	SIP_FSM_DBG1("addSipCCMConvert > ENTER, SipAccountId", SipAccountId);
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	uint8 phoneSelect = 0;
	ccmConn_t* newCcmConn_p = NULL;

	SIP_FSM_DBG2("addSipCCMConvert  SipAccountId = ", SipAccountId, ", callId = ", callId);

	/* allocate new sipCCMConvertEntry_p */
	if ( ( sipCCMConvertEntry_p = (sipCCMConvertEntry_t *)zyMalloc( sizeof(sipCCMConvertEntry_t) ) ) == NULL ) {
		dbg_pline_1("allocate sipCCMConvertEntry_p failed");
		return NULL;
	}

	sipCCMConvertEntry_p->sipCcmPortId = sipCcmPortId;
	sipCCMConvertEntry_p->RoleSt = RoleStatus;
	sipCCMConvertEntry_p->flags = flags;
	sipCCMConvertEntry_p->VpItr = VpItr;
	sipCCMConvertEntry_p->SipAccountId = SipAccountId;
	sipCCMConvertEntry_p->callId = callId;
	#if 1/*Eason, sipCallStatus*/
	if(pLine != NULL){
		zyStrncpy(sipCCMConvertEntry_p->sipCallStatus.selfNumber, pLine->directoryNumber, sizeof(sipCCMConvertEntry_p->sipCallStatus.selfNumber));
		sipCCMConvertEntry_p->sipCallStatus.selfNumber[sizeof(sipCCMConvertEntry_p->sipCallStatus.selfNumber)-1]=0;
	}
	#endif

	sipCcmPortId++;
	if ( sipCcmPortId > SIP_CCM_CONVERT_PORTID_UPBOUND ) {
		sipCcmPortId = SIP_CCM_CONVERT_PORTID_LOWBOUND;
	}

	/* add the new entry to the link list */
	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if ( sipCCMConvertEntry_p->SipAccountId < tmpEntry_p->SipAccountId) {
				tmpEntry_p = tmpEntry_p->prev;
				break;
			}
			if ( tmpEntry_p ->next != NULL ) {
				tmpEntry_p = tmpEntry_p->next;
			} else {
				break;
			}
		}
	}

	if ( tmpEntry_p == NULL ) {
		hp = &(sipCCMConvert.sipCCMConvertEntry);
		i_state = dirps();
		sipCCMConvertEntry_p->prev = NULL;
		sipCCMConvertEntry_p->next = *hp;
		if(sipCCMConvertEntry_p->next != NULL)
			sipCCMConvertEntry_p->next->prev = sipCCMConvertEntry_p;
		*hp = sipCCMConvertEntry_p;
		restore(i_state);
	} else {
		i_state = dirps();
		sipCCMConvertEntry_p->next = tmpEntry_p->next;
		sipCCMConvertEntry_p->prev = tmpEntry_p;
		if(sipCCMConvertEntry_p->prev != NULL)
			sipCCMConvertEntry_p->prev->next = sipCCMConvertEntry_p;
		if(sipCCMConvertEntry_p->next != NULL)
			sipCCMConvertEntry_p->next->prev = sipCCMConvertEntry_p;
		restore(i_state);
	}
	
	sipCCMConvert.sipCCMConvertNum += 1;

	phoneSelect = pLine->ZyXEL_phyReferenceFXSList;

	/* create ccmConn for the relative sipCcmPortId */
	if ( ( newCcmConn_p = create_ccmConn(SIP_PORT, sipCCMConvertEntry_p->sipCcmPortId, ORIG_FUNC_SIP, phoneSelect) )== NULL ) {
		printf("create_ccmConn() error for addSipCCMConvert() \n\r");
	}
	return newCcmConn_p;
}

/*______________________________________________________________________________
**	findSipCCMConvertByLineId
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
sipCCMConvertEntry_t *
findSipCCMConvertByCallId(
	int callId
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;

	SIP_FSM_DBG1("findSipCCMConvertByCallId callId = ", callId);

	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if ( callId == tmpEntry_p->callId ){
				return tmpEntry_p;
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	return NULL;
}

/*______________________________________________________________________________
**	findPermanentSipCCMConvertByLineId
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
sipCCMConvertEntry_t *
findPermanentSipCCMConvertByLineId(
	int SipAccountId
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;
	int found = 0;

	SIP_FSM_DBG1("findPermanentSipCCMConvertByLineId SipAccountId = ", SipAccountId);

	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if ( ( SipAccountId == tmpEntry_p->SipAccountId) && ( tmpEntry_p->flags & SIP_CCM_TABLE_PERMANENT ) ){
				found = 1;
				return tmpEntry_p;
			}
			if ( tmpEntry_p->SipAccountId> SipAccountId ) {
				found = 0;
				break;
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	return NULL;
}

/*______________________________________________________________________________
**	findSipCCMConvertByPortId
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
sipCCMConvertEntry_t *
findSipCCMConvertByPortId(
	uint16	sipCcmPortId
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;

	SIP_FSM_DBG1("findSipCCMConvertByPortId sipCcmPortId = ", sipCcmPortId);

	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if ( sipCcmPortId == tmpEntry_p->sipCcmPortId) {
				return tmpEntry_p;
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	return NULL;
}

/*______________________________________________________________________________
**	changeSipCCMConvert
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
sipCCMConvertEntry_t *
changeSipCCMConvert(
	int SipAccountId,
	int callId,
	int type
) {
	ccmConn_t *tmpCcmConn=NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	SIP_FSM_DBG2("changeSipCCMConvert, SipAccountId = ", SipAccountId, "callId = ", callId);

	tmpCcmConn = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn != NULL ) {
		if ( ( tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn->origPortId) ) != NULL ){
			if ( ( tmpConvertEntry_p->SipAccountId== SipAccountId ) && ( tmpConvertEntry_p->callId == 0 ) && 
				( tmpConvertEntry_p->RoleSt == SIP_CCM_CONVERT_ST_IDLE  ) && ( tmpCcmConn->connSt == CALL_CONN_ST_IDLE ) && 
				( !( tmpCcmConn->flags & CALL_CONN_FLAG_ASSIGN) ) && ( !( tmpCcmConn->flags & CALL_CONN_FLAG_PREASSIGN ) ) ){
				SIP_FSM_DBG("changeSipCCMConvert(): Match Convert Table");
				tmpConvertEntry_p->callId = callId;
				tmpConvertEntry_p->RoleSt = type;
				return tmpConvertEntry_p;
			}
		}
		tmpCcmConn = tmpCcmConn->next;
	}

	return NULL;
}

/*______________________________________________________________________________
**	deleteSipCCMConvert
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
deleteSipCCMConvert(
	int SipAccountId,
	int callId,
	int type,
	int option  
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;
	int found = 0, i_state;

	SIP_FSM_DBG2("deleteSipCCMConvert, SipAccountId = ", SipAccountId, "callId = ", callId);
	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if(option == SIP_CCM_TABLE_DELETE_NOT_CHECK) {
				if ( ( SipAccountId == tmpEntry_p->SipAccountId) && ( callId == tmpEntry_p->callId) && ( tmpEntry_p->RoleSt == type ) ) {
					found = 1;
					break;
				}
			}
			else {	
				if ( ( SipAccountId == tmpEntry_p->SipAccountId) && ( callId == tmpEntry_p->callId ) 
					&& ( tmpEntry_p->RoleSt == type ) && ( tmpEntry_p->flags == SIP_CCM_TABLE_TEMPORARY ) ) {
					found = 1;
					break;
				}
			}
			if ( tmpEntry_p->SipAccountId> SipAccountId ) {
				found = 0;
				break;
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	SIP_FSM_DBG1("deleteSipCCMConvert, found = ", found);
	if ( found == 1 ) {
		/* Remove the relative CCM  */
		delete_ccmConn(SIP_PORT, tmpEntry_p->sipCcmPortId);

		/* Remove the tmpEntry_p from the link list */
		i_state = dirps();
		if ( tmpEntry_p->prev != NULL ) {
			tmpEntry_p->prev->next = tmpEntry_p->next;
		} else {
			sipCCMConvert.sipCCMConvertEntry = tmpEntry_p->next;
		}
		if ( tmpEntry_p->next != NULL ) {
			tmpEntry_p->next->prev = tmpEntry_p->prev;
		}
		tmpEntry_p->next = NULL;
		tmpEntry_p->prev = NULL;
		restore(i_state);

		sipCCMConvert.sipCCMConvertNum -= 1;
		SIP_FSM_DBG1("sipCCMConvertNum", sipCCMConvert.sipCCMConvertNum);
		zyFree(tmpEntry_p);
		tmpEntry_p = NULL;
	}

	return;
}

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**	deleteAllSipCCMConvert
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
deleteAllSipCCMConvert(void) {
	sipCCMConvertEntry_t *delSipCcm = NULL, *delNextSipCcm = NULL;
	int i_state;

	SIP_FSM_DBG("deleteAllSipCCMConvert > ENTER");
	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		delSipCcm = sipCCMConvert.sipCCMConvertEntry;
		while ( delSipCcm != NULL ) {
			SIP_FSM_DBG("sipCCMConvert.sipCCMConvertEntry != NULL");
			delNextSipCcm = delSipCcm->next;
			/* Remove the relative CCM  */
			delete_ccmConn(SIP_PORT, delSipCcm->sipCcmPortId);
			i_state = dirps();
			if ( delSipCcm->prev != NULL ) {
				delSipCcm->prev->next = delSipCcm->next;
			} else {
				sipCCMConvert.sipCCMConvertEntry = delSipCcm->next;
			}
			if ( delSipCcm->next != NULL ) {
				delSipCcm->next->prev = delSipCcm->prev;
			}
			delSipCcm->next = NULL;
			delSipCcm->prev = NULL;
			restore(i_state);

			sipCCMConvert.sipCCMConvertNum -= 1;
			SIP_FSM_DBG1("sipCCMConvertNum", sipCCMConvert.sipCCMConvertNum);
			delSipCcm = delNextSipCcm;						
		}
		zyFree(delSipCcm);
		delSipCcm = NULL;
	}
	SIP_FSM_DBG("deleteAllSipCCMConvert end");
	return;
}
/*______________________________________________________________________________
**	resetSipCCMConvert
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
resetSipCCMConvert(
	int SipAccountId,
	int callId,
	int type
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;
	int found = 0;

	SIP_FSM_DBG2("resetSipCCMConvert, SipAccountId = ", SipAccountId, "callId = ", callId);
	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			if ( ( SipAccountId == tmpEntry_p->SipAccountId) && ( callId == tmpEntry_p->callId ) && ( tmpEntry_p->RoleSt == type ) ){
				found = 1;
				tmpEntry_p->callId = 0;
				tmpEntry_p->RoleSt = SIP_CCM_CONVERT_ST_IDLE;
				break;
			}
			if ( tmpEntry_p->SipAccountId> SipAccountId ) {
				found = 0;
				break;
			}
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	return;
}

/*______________________________________________________________________________
**	ccmDiscAckToSIP
**
**	descriptions: send SIP uaDlg Free Event to CCM
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ccmDiscAckToSIP(int SipAccountId, int callId){
	sipEvtToCCM(SipAccountId,  callId, EVT_CCM_DISC_ACK, NULL, 0);

	return;
}
/*Jaosn , syslog*/
#ifdef VOIP_SYSLOG
	void 
	sipEvtToVoIPLOG(
		int callId, 
		void *msg,
		int length
	){
		sipCCMConvertEntry_t *tmpEntry = NULL;
		ccmConn_t *ccmConn = NULL;
		phoneCh_t *tmpPhoneCB = NULL;
		VoiceProfLineObj_t* pLine = NULL;
		SIP_FSM_DBG1(" sipEvtToVoIPLOG   callId=", callId);

		if(callId==0xffff)
			return;

		tmpEntry = findSipCCMConvertByCallId(callId);
		if ( tmpEntry != NULL ) {
			/*
			printf(" sipEvtToVoIPLOG  Profile ID:%d, SIP Account ID:%d, Call ID:%d\n", 
					tmpEntry->profileId, tmpEntry->lineId, tmpEntry->callId);
				printf("  sipCcm PortId:%d, RoleSt:%d, flags:%08x\n", 
					tmpEntry->sipCcmPortId, tmpEntry->RoleSt, tmpEntry->flags);
			*/
			ccmConn = findOrigCcmConn(SIP_PORT, tmpEntry->sipCcmPortId);
			if(ccmConn!=NULL){
				if ( ccmConn->peerCcmConn != NULL ) {
			//		printf("\r\n peer ccm dsptype is %d",ccmConn->peerCcmConn->origPortType);
					
					tmpPhoneCB = find_phoneCB(ccmConn->peerCcmConn->origPortId, 0);
					if(tmpPhoneCB!=NULL){
						#if 1 /*Eason, get registered line obj then enabled line obj*/
						pLine = getAssociatedRegLineObj(tmpPhoneCB);
						#else
						pLine = getAssociatedLineObj(tmpPhoneCB);
						#endif

			//			printf("\r\n tmpPhoneCB phyid is %d\n",tmpPhoneCB->physicalId);
						if(NULL != pLine){
							voipSyslog(LOG_SIP_CALL_SIGNALING,LOG_INFO,(char *)pLine->directoryNumber,tmpPhoneCB->physicalId+1,msg);
						}
						else{
							printf("\r\nsipEvtToVoIPLOG  cannot find associated and registered line, phone logicalId = %d\n", tmpPhoneCB->logicalId);
						}
					}else{
						printf("\r\nsipEvtToVoIPLOG tmpPhoneCB is null");
						voipSyslog(LOG_SIP_CALL_SIGNALING,LOG_INFO,"N/A",0,msg);	
					}
				}else{
					printf("\r\nsipEvtToVoIPLOG  ccmConn->peerCcmConn is null");
					voipSyslog(LOG_SIP_CALL_SIGNALING,LOG_INFO,"N/A",0,msg);			
				}
			}else{
				printf("\r\nsipEvtToVoIPLOG ccm find fail");
				voipSyslog(LOG_SIP_CALL_SIGNALING,LOG_INFO,"N/A",0,msg);	
			}
		}else{
			voipSyslog(LOG_SIP_CALL_SIGNALING,LOG_INFO,"N/A",0,msg);			
			printf("sipEvtToVoIPLOG , findSipCCMConvertByCallId fail");
			
		}


		return;
	}
#endif


