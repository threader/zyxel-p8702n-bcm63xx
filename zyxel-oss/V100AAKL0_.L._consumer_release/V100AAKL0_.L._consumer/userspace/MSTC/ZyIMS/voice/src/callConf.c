#include <stdio.h>
#include "global.h"
#include "event.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "callConf.h"
#include "callTrans.h"
#include "phfsm.h"
#include "Voipccm_sip_itf.h"
#include "tr104.h"

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  chkPeerisLocal
**
**  descriptions: check if the dial digits belong to the speed dial. If yes, change it to the original numbers.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
chkPeerisLocal(
	dialDigitNum_t *dialDigitNum_p
) {
	int LineItr = 0, VpItr=0 , phonenumLen = 0;

	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;
	VoiceProfLineObj_t* pLine = NULL;

	NumOfProfile = tr104GetNumProfile();
	for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
		NumOfLine = tr104GetVPNumOfLine(VpItr);
			for ( LineItr=0; LineItr<NumOfLine; LineItr++ ) {
				pLine = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);
				phonenumLen = zyStrlen(pLine->directoryNumber);
				if((pLine->enable) && (dialDigitNum_p->current_length == phonenumLen)){
					if ( memcmp(pLine->directoryNumber, dialDigitNum_p->dial_digits, phonenumLen ) == 0 )
						return 1;
			}
		}
	}	
	return 0;
}/* chkPeerisLocal */

/*______________________________________________________________________________
**  createConferenceCallCB
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
createConferenceCallCB(
	ccmConn_t *ActiveCCM,
	uint16 NextPhID
){
	ccmConn_t *OnHoldCCM=NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	VoiceProfLineSipObj_t *pLineSip = NULL;
	conferencecall_t *confcall_p = NULL;
	uint16 CallFeature = 0;
	
	tmpConvertEntry_p = findSipCCMConvertByPortId(ActiveCCM->peerCcmConn->origPortId);
	if(tmpConvertEntry_p != NULL)
	{
		int LineItr = 0;

		CCM_FSM_DBG1("createConferenceCallCB > ENTER, SipAccountId %d\n", tmpConvertEntry_p->SipAccountId);
		LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
		pLineSip = tr104VoiceLineSipObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
	}
	if(pLineSip == NULL){
		dbg_pline_1("\r\n Conference Call Request Failed");
		return -1;
	}

	OnHoldCCM = findOrigCcmConn(DSP_FXS_PORT, NextPhID);
	if(OnHoldCCM == NULL){
		CCM_FSM_DBG("\r\n Did not find OnHoldCCM");
		return -1;
	}

	CCM_FSM_DBG3("createConferenceCallCB, ActiveCCM=", (uint32)ActiveCCM, " ,OnHoldCCM=", (uint32)OnHoldCCM, "Mixer Mode: ", pLineSip->ZyXEL_mixerMode);
	CCM_PRINTF("ActiveCCM logicalId = 0x%x, OnHoldCCM logicalId = 0x%x\n", ActiveCCM->origPortId, OnHoldCCM->origPortId);
	confcall_p = (conferencecall_t *)calloc(1, sizeof(conferencecall_t) );
	if(confcall_p == NULL){
		CCM_FSM_DBG("\r\n Can't Create Conference Call Control Block");
		return -2;
	}
	CallFeature = (pLineSip->ZyXEL_mixerMode+1) << 2;
	
	confcall_p->confcallStatus = CONF_CALL_ST_IDLE;
	confcall_p->confcallActiveCCM = ActiveCCM;
	confcall_p->confcallOnHoldCCM = OnHoldCCM;
	ActiveCCM->callFeatures = CallFeature | CCM_CALLCONF_ACTIVE_CALL;
	ActiveCCM->ConfCallCB = confcall_p;
	OnHoldCCM->callFeatures = CallFeature | CCM_CALLCONF_ONHOLD_CALL;
	OnHoldCCM->ConfCallCB = confcall_p;

	if(pLineSip->ZyXEL_mixerMode == TR104I_MIXMODE_REMOTE) {
		confcall_p->confcallFlags  |= CONF_CALL_CHECK_MIXER_MODE;
	} else {
		confcall_p->confcallFlags  &= ~CONF_CALL_CHECK_MIXER_MODE;
	}
	
	if((ActiveCCM->flags & CALL_CONN_FLAG_PEERLOCAL) || (OnHoldCCM->flags & CALL_CONN_FLAG_PEERLOCAL)){
		confcall_p->confcallFlags |= CONF_CALL_CHECK_PEER_ISLOCAL;
	} else {
		confcall_p->confcallFlags &= ~CONF_CALL_CHECK_PEER_ISLOCAL;
	}

	if (confcall_p->confcallFlags & CONF_CALL_CHECK_MIXER_MODE) { /* Remote */
		confcall_p->ContactHeader = zyMalloc(128);
		zyMemset(confcall_p->ContactHeader, 0, 128);
		confcall_p->ConfAddr = zyMalloc(8);
		zyStrcpy(confcall_p->ConfAddr, "conf");
		confcall_p->confcallCreatorCCM = create_ZyPhone(ActiveCCM, CALL_FEATURE_THREE_WAY_CONFERENCE);
		confcall_p->confcallCreatorCCM->callFeatures = CallFeature | CCM_CALLCONF_CREATOR_CALL;
		confcall_p->confcallCreatorCCM->ConfCallCB = confcall_p;
	}
	return 0;
} /* createConferenceCallCB */

/*______________________________________________________________________________
**  swapConferenceCallCB
**
**  descriptions: when hold and resume, ActiveCCM and OnHoldCCM need swap
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
swapConferenceCallCB(
	conferencecall_t *confcall_p
){
	ccmConn_t *OnHoldCCM=NULL, *ActiveCCM = NULL, *tmpCCM = NULL;
	sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
	VoiceProfLineSipObj_t *pLineSip = NULL;
	uint16 CallFeature = 0;
	
	CCM_PRINTF("entering %s\n",__FUNCTION__);
	if(confcall_p == NULL){
		CCM_PRINTF("confcall NULL, Failed\n");
		return -1;
	}
	ActiveCCM = confcall_p->confcallActiveCCM;
	OnHoldCCM = confcall_p->confcallOnHoldCCM;

	if(ActiveCCM != NULL && ActiveCCM->peerCcmConn != NULL){
		tmpConvertEntry_p = findSipCCMConvertByPortId(ActiveCCM->peerCcmConn->origPortId);
		if(tmpConvertEntry_p != NULL){
			int LineItr = 0;

			CCM_FSM_DBG1("swapConferenceCallCB > ENTER, SipAccountId %d\n", tmpConvertEntry_p->SipAccountId);
			LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
			pLineSip = tr104VoiceLineSipObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
		}
		if(pLineSip == NULL){
			CCM_PRINTF("\r\n swapConferenceCallCB Request Failed");
			return -1;
		}
	}
	else{
		CCM_PRINTF("\r\n swapConferenceCallCB Request Failed");
		return -1;
	}
	
	CCM_PRINTF("swapConferenceCallCB ActiveCCM logicalId = 0x%x, OnHoldCCM logicalId = 0x%x\n", ActiveCCM->origPortId, OnHoldCCM->origPortId);
	
	CallFeature = (pLineSip->ZyXEL_mixerMode+1) << 2;
	
	
	tmpCCM = confcall_p->confcallActiveCCM;
	confcall_p->confcallActiveCCM = confcall_p->confcallOnHoldCCM;
	confcall_p->confcallOnHoldCCM = tmpCCM;

	ActiveCCM = confcall_p->confcallActiveCCM;
	OnHoldCCM = confcall_p->confcallOnHoldCCM;
	confcall_p->threeWayCallBaseLogicId = ActiveCCM->origPortId;
	
	ActiveCCM->callFeatures = CallFeature | CCM_CALLCONF_ACTIVE_CALL;
	ActiveCCM->ConfCallCB = confcall_p;
	OnHoldCCM->callFeatures = CallFeature | CCM_CALLCONF_ONHOLD_CALL;
	OnHoldCCM->ConfCallCB = confcall_p;

	if(pLineSip->ZyXEL_mixerMode == TR104I_MIXMODE_REMOTE) {
		confcall_p->confcallFlags  |= CONF_CALL_CHECK_MIXER_MODE;
	} else {
		confcall_p->confcallFlags  &= ~CONF_CALL_CHECK_MIXER_MODE;
	}
	
	if((ActiveCCM->flags & CALL_CONN_FLAG_PEERLOCAL) || (OnHoldCCM->flags & CALL_CONN_FLAG_PEERLOCAL)){
		confcall_p->confcallFlags |= CONF_CALL_CHECK_PEER_ISLOCAL;
	} else {
		confcall_p->confcallFlags &= ~CONF_CALL_CHECK_PEER_ISLOCAL;
	}

	return 0;
} /* createConferenceCallCB */


/*______________________________________________________________________________
**  removeConferenceCallCB
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
removeConferenceCallCB(
	conferencecall_t *confcall_p
){
	CCM_FSM_DBG("Conference Call: removeConferenceCallCB");
	if(confcall_p == NULL) {
		CCM_FSM_DBG("removeConferenceCallCB : confcall_p is NULL");
		return;
	}

	if ( confcall_p->confcallActiveCCM != NULL ) {		
		confcall_p->confcallActiveCCM->ConfCallCB = NULL;
		confcall_p->confcallActiveCCM->callFeatures = 0;
	}
	
	if ( confcall_p->confcallOnHoldCCM != NULL ) {
		confcall_p->confcallOnHoldCCM->ConfCallCB = NULL;
		confcall_p->confcallOnHoldCCM->callFeatures = 0;
	}

	if(confcall_p->confcallFlags & CONF_CALL_CHECK_MIXER_MODE){ /* Remote */
		if ( confcall_p->confcallCreatorCCM != NULL ) {
			confcall_p->confcallCreatorCCM->ConfCallCB = NULL;
			confcall_p->confcallCreatorCCM->callFeatures = 0;
		}
		if(confcall_p->ConfAddr != NULL) {
			zyFree(confcall_p->ConfAddr);
		}
		if(confcall_p->ContactHeader != NULL) {
			zyFree(confcall_p->ContactHeader);
		}
	}
	
	if(confcall_p != NULL){
		zyFree(confcall_p);
		confcall_p = NULL;
	}
	return;
} /* removeConferenceCallCB */

/*______________________________________________________________________________
**  ConferenceCallFsmProc
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
ConferenceCallFsmProc(
	conferencecall_t *confcall_p, 
	uint16 ConfCallEvent, 
	void *data_p,
	uint16 data_size
){
	CCM_FSM_DBG2("ConferenceCallFsmProc: status=", confcall_p->confcallStatus, " , Event=", ConfCallEvent);

	switch( confcall_p->confcallStatus) {
		case CONF_CALL_ST_IDLE:
			ConferenceCallFsmIdle(confcall_p, ConfCallEvent, data_p, data_size);
			break;
		case CONF_CALL_ST_MEET_ROOM_CREATED:
			ConferenceCallFsm1stParty(confcall_p, ConfCallEvent, data_p, data_size);
			break;
		case CONF_CALL_ST_1stPARTY_JOINED:
			ConferenceCallFsm2ndParty(confcall_p, ConfCallEvent, data_p, data_size);
			break;
		case CONF_CALL_ST_ALL_MEMBER_JOINED:
			ConferenceCallFsmMemberJoined(confcall_p, ConfCallEvent, data_p, data_size);
			break;
		default:
			CCM_FSM_DBG1("\r\n ConferenceCallFsmProc Unknown Status: ", confcall_p->confcallStatus);
			break;
	}
	return;
} /* ConferenceCallFsmProc */

/*______________________________________________________________________________
**  ConferenceCallFsmIdle
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
ConferenceCallFsmIdle(
	conferencecall_t *confcall_p, 
	uint16 ConfCallEvent, 
	void *data_p,
	uint16 data_size
){
	ccmConn_t *peerCcmConn_p=NULL;
	uint8 tmpPeerisLocal = (confcall_p->confcallFlags & CONF_CALL_CHECK_PEER_ISLOCAL);

	CCM_FSM_DBG1("ConferenceCallFsmIdle: Event=", ConfCallEvent);

	switch( ConfCallEvent) {
		case EVT_LOCAL_MEETROOM_REQ:
			if(tmpPeerisLocal){
				if(confcall_p->confcallActiveCCM->flags & CALL_CONN_FLAG_PEERLOCAL){
					eventVoiceSimpleSend(EVT_CCM_3WAYCALL_SWITCH_ACTIVE_PHONE, MSG_SEND_TO_PHONE, confcall_p->confcallActiveCCM->origPortId);
				}else{
					eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_CONF, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, &tmpPeerisLocal, sizeof(uint8));
					confcall_p->confcallStatus=CONF_CALL_ST_ALL_MEMBER_JOINED;
				}
			}else{
				eventVoiceSimpleSend(EVT_CCM_CONFCALL_MEETROOM_REQ, confcall_p->confcallActiveCCM->origFuncId, confcall_p->confcallActiveCCM->origPortId);
			}
			break;
		case EVT_REMOTE_MEETROOM_REQ:
				associateFxs2Sip(confcall_p->confcallCreatorCCM, 0);
				peerCcmConn_p = confcall_p->confcallCreatorCCM->peerCcmConn;
				zyMemcpy(peerCcmConn_p->dialInform.dialDigitNumber.dial_digits, confcall_p->ConfAddr, zyStrlen(confcall_p->ConfAddr));
				eventVoiceSimpleSend(EVT_CCM_DIAL_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId);
				break;
		case EVT_CCM_CONFCALL_MEETROOM_CONF:
			confcall_p->confcallStatus = CONF_CALL_ST_MEET_ROOM_CREATED;
			eventVoiceSimpleSend(EVT_CCM_CONFCALL_JOIN_MEMBER_REQ, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId);
			break;
		case EVT_REMOTE_MEETROOM_CONF:{
			char *Temp;
			confcall_p->confcallStatus = CONF_CALL_ST_MEET_ROOM_CREATED;
			Temp =zyMalloc(data_size+4);
			zyMemset(Temp,0,data_size+4);
			zyMemcpy(Temp,data_p,data_size);
			Temp[data_size] = '\0';
			sprintf(confcall_p->ContactHeader, "%s;isfocus",Temp);
			zyFree(Temp);
			Temp = NULL;
			peerCcmConn_p = confcall_p->confcallActiveCCM->peerCcmConn;
			eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, confcall_p->ContactHeader, zyStrlen(confcall_p->ContactHeader));
		}
			break;
		case EVT_CCM_UNHELD_REQ:
			eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId, confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId);
			break;
		case EVT_CCM_CONFERENCE_IND:
			eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId, confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId);
			break;
		case EVT_CCM_UNHELD_CONF:{
			ccmConn_t*	tmpconfcallCCM;
			uint16 tmpCallFeatures;
			eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->origFuncId, confcall_p->confcallOnHoldCCM->origPortId);
			tmpconfcallCCM = confcall_p->confcallOnHoldCCM;
			confcall_p->confcallOnHoldCCM = confcall_p->confcallActiveCCM;
			confcall_p->confcallActiveCCM = tmpconfcallCCM;
			tmpCallFeatures = confcall_p->confcallOnHoldCCM->callFeatures;
			confcall_p->confcallOnHoldCCM->callFeatures = confcall_p->confcallActiveCCM->callFeatures;
			confcall_p->confcallActiveCCM->callFeatures = tmpCallFeatures;

			eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_CONF, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, &tmpPeerisLocal, sizeof(uint8));
			confcall_p->confcallStatus=CONF_CALL_ST_ALL_MEMBER_JOINED;
		}
			break;
		case EVT_CCM_CONFERENCE_CONF:{
				ccmConn_t*	tmpconfcallCCM;
				uint16 tmpCallFeatures;
				eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->origFuncId, confcall_p->confcallOnHoldCCM->origPortId);
				tmpconfcallCCM = confcall_p->confcallOnHoldCCM;
				confcall_p->confcallOnHoldCCM = confcall_p->confcallActiveCCM;
				confcall_p->confcallActiveCCM = tmpconfcallCCM;
				tmpCallFeatures = confcall_p->confcallOnHoldCCM->callFeatures;
				confcall_p->confcallOnHoldCCM->callFeatures = confcall_p->confcallActiveCCM->callFeatures;
				confcall_p->confcallActiveCCM->callFeatures = tmpCallFeatures;

				eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_CONF, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, &tmpPeerisLocal, sizeof(uint8));
				confcall_p->confcallStatus=CONF_CALL_ST_ALL_MEMBER_JOINED;
			}
			break;
		case EVT_CCM_CONFCALL_MEETROOM_FAIL:
		case EVT_CCM_REJ_IND:
		case EVT_CCM_REL_IND:
		case EVT_CCM_DISC_IND:
			removeConferenceCallCB(confcall_p);
			break;
		default:
			CCM_FSM_DBG1("\r\n ConferenceCallFsmIdle Unknown Event: ", ConfCallEvent);
			break;
	}
	return;
} /* ConferenceCallFsmIdle */

/*______________________________________________________________________________
**  ConferenceCallFsm1stParty
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
ConferenceCallFsm1stParty(
	conferencecall_t *confcall_p, 
	uint16 ConfCallEvent, 
	void *data_p,
	uint16 data_size
){
	ccmConn_t *tmpCcmConn_p=NULL;
	ccmConn_t *peerCcmConn_p=NULL;
	uint8 tmpPeerisLocal = (confcall_p->confcallFlags & CONF_CALL_CHECK_PEER_ISLOCAL);

	CCM_FSM_DBG1("ConferenceCallFsm1stParty: Event=", ConfCallEvent);

	switch( ConfCallEvent) {
		case EVT_REMOTE_ACTIVE_JOIN:
			if((confcall_p->confcallCreatorCCM->peerCcmConn != NULL) && (confcall_p->confcallCreatorCCM != NULL)){
				phoneCh_t *tmpphoneCB;
				/*rtp_table_t *table;
				extern int RtpSendToDsp(UINT16 chId, UINT8 *InMessage, UINT16 MessageLen, UINT32 TimeStamp);
				extern rtp_table_t *rtp_table_get_by_origChId(uint16 chId);*/

				/*if( (table = rtp_table_get_by_origChId(confcall_p->confcallCreatorCCM->origPortId)) == NULL ){
					CCM_FSM_DBG("\r\n ConferenceCallFsm1stParty: Can't Get RTP Table");
					removeConferenceCallCB(confcall_p);
					return;
				}
				table->ori_chId = confcall_p->confcallActiveCCM->origPortId;
				table->voipRtpCallBack = RtpSendToDsp;*/

				tmpCcmConn_p = confcall_p->confcallActiveCCM->peerCcmConn;
				confcall_p->confcallActiveCCM->peerCcmConn = confcall_p->confcallCreatorCCM->peerCcmConn;
				confcall_p->confcallCreatorCCM->peerCcmConn = tmpCcmConn_p;
				confcall_p->confcallActiveCCM->peerCcmConn->peerCcmConn = confcall_p->confcallActiveCCM;
				confcall_p->confcallCreatorCCM->peerCcmConn->peerCcmConn = confcall_p->confcallCreatorCCM;

				tmpphoneCB = find_phoneCB(confcall_p->confcallActiveCCM->origPortId, 0);
				tmpphoneCB->peerMediaChId = confcall_p->confcallActiveCCM->peerCcmConn->origPortId;
			#if 0
				if(confcall_p->confcallCreatorCCM != NULL){
					confcall_p->confcallCreatorCCM->ConfCallCB = NULL;
					confcall_p->confcallCreatorCCM = NULL;
				}
			#endif
			}
			confcall_p->confcallStatus = CONF_CALL_ST_1stPARTY_JOINED;
			peerCcmConn_p = confcall_p->confcallOnHoldCCM->peerCcmConn;
			eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_REQ, peerCcmConn_p->origFuncId, peerCcmConn_p->origPortId, confcall_p->ContactHeader, zyStrlen(confcall_p->ContactHeader));
		#if 0
			if(confcall_p->confcallActiveCCM != NULL){
				confcall_p->confcallActiveCCM->ConfCallCB = NULL;
				confcall_p->confcallActiveCCM = NULL;
			}
		#endif
			break;
		case EVT_CCM_UNHELD_REQ:
			eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId, confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId);
			break;
		case EVT_CCM_CONFERENCE_IND:
			//eventVoiceSimpleSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId, confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId);
			eventVoiceDataSend(ConfCallEvent, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId, confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId, &confcall_p->confcallActiveCCM->peerCcmConn->origPortId, sizeof(uint16));
			break;
		case EVT_CCM_UNHELD_CONF:
			eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_CONF, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, &tmpPeerisLocal, sizeof(uint8));
			confcall_p->confcallStatus=CONF_CALL_ST_ALL_MEMBER_JOINED;
			break;
		case EVT_CCM_CONFERENCE_CONF:
			eventVoiceDataSend(EVT_CCM_CONFCALL_JOIN_MEMBER_CONF, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, &tmpPeerisLocal, sizeof(uint8));
			confcall_p->confcallStatus=CONF_CALL_ST_ALL_MEMBER_JOINED;
			break;
		case EVT_CCM_3WAYCALL_MEETROOM_DESTROY:
			eventVoiceSimpleSend(ConfCallEvent,confcall_p->confcallActiveCCM->origFuncId,confcall_p->confcallActiveCCM->origPortId);
			removeConferenceCallCB(confcall_p);
			break;
		case EVT_CCM_CONFCALL_MEETROOM_FAIL:
		case EVT_CCM_REJ_IND:
		case EVT_CCM_REL_IND:
		case EVT_CCM_DISC_IND:
			removeConferenceCallCB(confcall_p);
			break;
		default:
			CCM_FSM_DBG1("\r\n ConferenceCallFsm1stParty Unknown Event: ", ConfCallEvent);
			break;
	}
	//UNUSED_VAR(data_p);UNUSED_VAR(data_size);
	return;
} /* ConferenceCallFsm1stParty */

/*______________________________________________________________________________
**  ConferenceCallFsm2ndParty
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
ConferenceCallFsm2ndParty(
	conferencecall_t *confcall_p, 
	uint16 ConfCallEvent, 
	void *data_p,
	uint16 data_size
){
	
	CCM_FSM_DBG1("ConferenceCallFsm2ndParty: Event=", ConfCallEvent);

	switch( ConfCallEvent) {
		case EVT_REMOTE_ONHOLD_JOIN:
		#if 0
			if(confcall_p->confcallOnHoldCCM != NULL){
				confcall_p->confcallOnHoldCCM->ConfCallCB = NULL;
				confcall_p->confcallOnHoldCCM = NULL;
			}
		#endif
			confcall_p->confcallStatus = CONF_CALL_ST_ALL_MEMBER_JOINED;
			break;
		case EVT_CCM_REJ_IND:
		case EVT_CCM_REL_IND:
		case EVT_CCM_DISC_IND:
		case EVT_REMOTE_ACTIVE_LEAVE:
			removeConferenceCallCB(confcall_p);
			break;
		default:
			CCM_FSM_DBG1("\r\n ConferenceCallFsm2ndParty Unknown Event: ", ConfCallEvent);
			break;
	}
	//UNUSED_VAR(data_p);UNUSED_VAR(data_size);
	return;
} /* ConferenceCallFsm2ndParty */

/*______________________________________________________________________________
**  ConferenceCallFsmMemberJoined
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
ConferenceCallFsmMemberJoined(
	conferencecall_t *confcall_p, 
	uint16 ConfCallEvent, 
	void *data_p,
	uint16 data_size
){
	uint16 threeWayCallBaseLogicId;
	callTransfer_t *callTransfer_p = NULL;
	ccmConn_t joinCcmConn, *callTransferCcmConn1 = NULL, *callTransferCcmConn2 = NULL;
	uint8 tmpPeerisLocal = (confcall_p->confcallFlags & CONF_CALL_CHECK_PEER_ISLOCAL);

	CCM_FSM_DBG1("ConferenceCallFsmMemberJoined: Event=", ConfCallEvent);

	switch( ConfCallEvent) {
		case EVT_LOCAL_MEMBER1_LEAVE:
		case EVT_LOCAL_MEMBER2_LEAVE:{
			uint16 origPortId = *(uint16 *)data_p;

			if(origPortId == confcall_p->confcallActiveCCM->origPortId){
				zyMemset(&joinCcmConn, 0, sizeof(ccmConn_t) );
				zyMemcpy(&(joinCcmConn), confcall_p->confcallOnHoldCCM, sizeof(ccmConn_t) );
				if(!tmpPeerisLocal){
					eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_REMOVE_BASEPHONE, confcall_p->confcallActiveCCM->origFuncId,confcall_p->confcallActiveCCM->origPortId, DSP_FXS_PORT, &(joinCcmConn), sizeof(ccmConn_t ));
				}
				eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ,confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, DSP_FXS_PORT, &tmpPeerisLocal, sizeof(uint8));
			}else if(origPortId == confcall_p->confcallOnHoldCCM->origPortId){
				eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ,confcall_p->confcallActiveCCM->origFuncId,confcall_p->confcallActiveCCM->origPortId, DSP_FXS_PORT, &tmpPeerisLocal, sizeof(uint8));
			}
		}
			break;
		case EVT_LOCAL_ACTIVE_LEAVE:
			threeWayCallBaseLogicId = confcall_p->threeWayCallBaseLogicId;
			PH_FSM_PRINTF("EVT_LOCAL_ACTIVE_LEAVE, threeWayCallBaseLogicId = %d\n", confcall_p->threeWayCallBaseLogicId);
			eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_REQ, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, DSP_FXS_PORT, &(threeWayCallBaseLogicId), sizeof(uint16));
		#if 1			
			if(!tmpPeerisLocal){
				eventVoiceSimpleSend(EVT_CCM_3WAYCALL_REMOVE_NEWPHONE, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId);
			}
			else{
				eventVoiceSimpleSend(EVT_CCM_DISC_REQ, confcall_p->confcallOnHoldCCM->peerCcmConn->origFuncId,confcall_p->confcallOnHoldCCM->peerCcmConn->origPortId);
				eventVoiceSimpleSend(EVT_CCM_DISC_IND, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId);
				confcall_p->confcallOnHoldCCM->connSt = CALL_CONN_ST_IDLE;
				confcall_p->confcallOnHoldCCM->connRole = CCM_CONN_ROLE_UNDEF;
				if ( confcall_p->confcallOnHoldCCM->peerCcmConn->origPortType == DSP_FXO_LIFELINE ) {
					confcall_p->confcallOnHoldCCM->peerCcmConn->connSt = CALL_CONN_ST_IDLE;
					confcall_p->confcallOnHoldCCM->peerCcmConn->connRole = CCM_CONN_ROLE_UNDEF;
				} else {
					confcall_p->confcallOnHoldCCM->peerCcmConn->connSt = CALL_CONN_ST_CONNREL;
					confcall_p->confcallOnHoldCCM->peerCcmConn->connRole = CCM_CONN_ROLE_PEER;
				}
				ccm_releaseConn(confcall_p->confcallOnHoldCCM);
			}
		#else
			callTransferCcmConn1 = confcall_p->confcallActiveCCM;
			callTransferCcmConn2 = confcall_p->confcallOnHoldCCM;
			if ( callTransferCcmConn1 != NULL ) {	
				if ( callTransferCcmConn1->connType== CCM_CONN_TYPE_CALLER_CALLER ) {		/* CALL_TRANSFER_AFTER_CONFERENCE */
					if ( ( callTransferCcmConn2 != NULL ) && ( callTransferCcmConn1->peerCcmConn != NULL ) && ( callTransferCcmConn2->peerCcmConn != NULL ) ){
						callTransfer_p = createCallTransferCB( callTransferCcmConn1->peerCcmConn, callTransferCcmConn2->peerCcmConn, CALL_TRANSFEROR, CALL_TRANSFER_TYPE_CONSULT);
						callTransfer_p->callTransferStatus = CALL_TRANSFER_ST_SECOND_CALL;
					} else {
						eventVoiceSimpleSend(EVT_CCM_3WAYCALL_REMOVE_NEWPHONE, confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId);
					}
				}
			}
		#endif
			break;		
		case EVT_LOCAL_CONFCALL_SPLIT:
			zyMemset(&joinCcmConn, 0, sizeof(ccmConn_t) );
			zyMemcpy(&(joinCcmConn), confcall_p->confcallOnHoldCCM, sizeof(ccmConn_t) );
			if(!tmpPeerisLocal){
				eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_REMOVE_BASEPHONE, confcall_p->confcallActiveCCM->origFuncId,confcall_p->confcallActiveCCM->origPortId, DSP_FXS_PORT, &(joinCcmConn), sizeof(ccmConn_t ));
			}
			eventVoiceCCMDataSend(EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_REQ,confcall_p->confcallOnHoldCCM->origFuncId,confcall_p->confcallOnHoldCCM->origPortId, DSP_FXS_PORT, &tmpPeerisLocal, sizeof(uint8));
			break;
		case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MEMBER_LEAVE_CONF:{
			uint16 origPortId = *(uint16 *)data_p;
			
			if(tmpPeerisLocal && (origPortId == confcall_p->confcallOnHoldCCM->origPortId)){
				eventVoiceSimpleSend(EVT_PHONE_FLASH, MSG_SEND_TO_PHONE, confcall_p->confcallActiveCCM->origPortId);
			}			
			removeConferenceCallCB(confcall_p);			

		}
			break;
		case EVT_CCM_3WAYCALL_MEETROOM_DESTROY_MIXER_LEAVE_CONF:
		case EVT_REMOTE_ACTIVE_LEAVE:
		case EVT_CCM_REJ_IND:
		case EVT_CCM_REL_IND:
		case EVT_REMOTE_ACTIVE_JOIN:
		case EVT_REMOTE_ONHOLD_JOIN:
			removeConferenceCallCB(confcall_p);
			break;
		default:
			CCM_FSM_DBG1("\r\n ConferenceCallFsmMemberJoined Unknown Event: ", ConfCallEvent);
			break;
	}
	
	return;
} /* ConferenceCallFsmMemberJoined */

