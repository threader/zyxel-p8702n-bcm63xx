/*
$Id: dsp_danube_itf.c 1.18 2006/12/08 11:47:06 ohsieh Exp $
*/

/*******************************************************************************
 * FILE PURPOSE:    DSP Initialization
 *
 *******************************************************************************
 * FILE NAME:       dsp.c
 *
 * DESCRIPTION:     
 *
 * Copyright (C) 2000-2005 ZyXEL Communications, Corp.
 ******************************************************************************/
/*
 ** Initial revision
*/
#include <stdio.h>
#include "endpt.h"
#include "Endpoint_itf.h"
#include <switch.h>
#include <global.h>
#include <event.h>
#include "sys_msg.h"
#include <call_mgnt.h>
#include <phfsm.h>
#include <dsp_itf.h>
#include "icf_types.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#include "sys_ipc.h"
#include "mm_macros.h"

#include "tr104_init.h"
extern uint32 sipAccountNumber;//Ssu-Ying, support Multiple Profile

extern int dspReinitNeeded;

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**	Function
**____________________________________________________________________________*/


/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**	Variables
**____________________________________________________________________________*/



/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
**	MAIN PROGRAM
**____________________________________________________________________________*/

/*______________________________________________________________________________
**  voiceDspInit
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: 0 success, -1 fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int 
voiceDspInit(
	void
) {
	int err = -1;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspInitRsp_t *rspPayload_p = NULL;
	voiceDspInitReq_t *reqPayload_p = NULL;
	VoiceProfObj_t *voiceProfObj_p = tr104VoiceProfObjPtrGet(0); /*get country from first profile */
        #if 1 /*Eason, support user-defined phone flash interval*/
	VoicePhyIntfObj_t *phyIntfObj = NULL;
	int i=0;
	ZyPrintf("entering %s\n",__FUNCTION__);	
	PhoneOpenDriver();
	/*set provision values for flash min/max to endpoint driver before the dsp driver initialized*/
	for(i = 0;i < tr104GetNumOfPhyIntf();i++){
		phyIntfObj = tr104VoicePhyIntfObjPtrGet(i);
		if((NULL != phyIntfObj) && 
			(phyIntfObj->ZyXEL_FlashMinInterval>=0) &&  
			(phyIntfObj->ZyXEL_FlashMaxInterval>0) &&
			(phyIntfObj->ZyXEL_FlashMaxInterval>phyIntfObj->ZyXEL_FlashMinInterval))
		{
			ZyPrintf("%s, Set phone flash interval\n",__FUNCTION__);
			PhoneFlashIntervalSet(i, phyIntfObj->ZyXEL_FlashMinInterval, phyIntfObj->ZyXEL_FlashMaxInterval);
		}
	}
	#endif
	
	PH_FSM_DBG("vDspInit\n");
	msg_p=voiceMsgAlloc(VOICE_DSP_INIT_REQ, sizeof(voiceDspInitReq_t));
	reqPayload_p = (voiceDspInitReq_t *)msg_p->payload;
	reqPayload_p->country = PhoneRegionToCountryCode(voiceProfObj_p->region);
	if(eventVoiceDspDataSend((uint8*)msg_p, (uint8**)&rspMsg_p) != 0){
		PH_FSM_DBG("voiceDspInit ipc send failed\n");
		return -1;
	}
	voiceMsgFree(msg_p);
	
	if(VOICE_DSP_INIT_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspInitRsp_t *)rspMsg_p->payload;

		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
			PhoneDataInit(rspPayload_p->initData, rspPayload_p->initDataLength);			
			PhoneInitEventCb();			
			AppRegisterSignal();
			#ifdef FXO_DSPRELAY
			PhonePSTNSetRelay(FALSE);
			#endif
			dspReinitNeeded = 0;
		}
		else{
			PH_FSM_PRINTF("voiceDspInit FAILED !!! \n");
		}

	}
	
	voiceMsgFree(rspMsg_p);
	return err;
}

/*______________________________________________________________________________
**  voiceDspDeinit
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: 0 success, -1 fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

void 
voiceDspDeinit(
	void
) {
	PH_FSM_DBG("voiceDsp Deinit...\n");
	PhoneDeinit();	
}

/*______________________________________________________________________________
**  vDspChanOpen
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: 0 success, -1 fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
vDspChanOpen(
	phoneCh_t *phoneCB
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspChanOpenReq_t *payload_p = NULL;
	voiceDspChanOpenRsp_t *rspPayload_p = NULL;
//Ssu-Ying, support Multiple Profile
	int lineItr = 0;
	VoiceProfObj_t *pVoiceProf = getAssociatedProfileObj(phoneCB);
	#if 1 /*Eason, get registered line obj then enabled line obj*/
	VoiceProfLineObj_t *pLine = getAssociatedRegLineObj(phoneCB);	
	#else
	VoiceProfLineObj_t *pLine = getAssociatedLineObj(phoneCB);	
	#endif
	VoiceProfLineVoiceProcessingObj_t *pVoiceProfLineProcessing = NULL;

	PH_FSM_PRINTF("vDspChanOpen>ENTER\n");
	if(NULL != pLine){
		PH_FSM_PRINTF("vDspChanOpen>ENTER, SipAccountId = %d\n", pLine->ZyXEL_CMAcntNum);
	}
	
	if(NULL != pLine){
		pVoiceProfLineProcessing = pLine->voiceProcessing;
	}

	PH_FSM_DBG("vDspChanOpen>>\n");

	/* Generate call_id when outgoing call */
	if(phoneCB->callId == 0) {
		phoneCB->callId = phoneCB->logicalId;
	}
	
	msg_p=voiceMsgAlloc(VOICE_DSP_CHAN_OPEN_REQ,sizeof(voiceDspChanOpenReq_t));
	payload_p=(voiceDspChanOpenReq_t *)msg_p->payload;
	payload_p->logicalId=phoneCB->logicalId;
	payload_p->physicalId=phoneCB->physicalId;
	payload_p->portType=phoneCB->dspPortType;
	payload_p->call_id=phoneCB->callId;
	/* Follow TR-104 supoort DTMFMethod and DTMFMethodG711 both */
	if(NULL !=pVoiceProf ){
		payload_p->dtmfMethod = pVoiceProf->DTMFMethod;
		payload_p->dtmfMethodG711 = pVoiceProf->DTMFMethodG711;
	}
	else{
		payload_p->dtmfMethod = 0;
		payload_p->dtmfMethodG711 = 0;
		ZyPrintf("Error!! no profile associated, dtmfMethod set to 0\n");
	}

	if(NULL != pVoiceProfLineProcessing){
		payload_p->echoCancel = pVoiceProfLineProcessing->echoCancellationEnable;
	}
	else{
		payload_p->echoCancel = 0;
	}
	if(NULL != pVoiceProf && NULL != pLine){
		lineItr = mapSipAccountIdToLineItr(pLine->ZyXEL_CMAcntNum);
		payload_p->silenceSupp = tr104GetLineCodecSilenceSuppression(pVoiceProf->ZyXEL_SPNum, lineItr);
	}
	
	payload_p->mode = SENDRECV;
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);	

	if(VOICE_DSP_CHAN_OPEN_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspChanOpenRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	else{
		PH_FSM_DBG1("vDspChanOpen >> wrong api response received, api = ", rspMsg_p->hdr.api_id);
	}
	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspChanOpen return %d\n", err);

	return err;
} /* vDspChanOpen */

/*______________________________________________________________________________
**  vDspChanOpenRecvonly
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: 0 success, -1 fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int 
vDspChanOpenRecvonly(
	phoneCh_t *phoneCB
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspChanOpenReq_t *payload_p = NULL;
	voiceDspChanOpenRsp_t *rspPayload_p = NULL;
//Ssu-Ying, support Multiple Profile
	VoiceProfObj_t *pVoiceProf = tr104VoiceProfObjPtrGet(phoneCB->associatedVpItr);
	int LineItr = 0;
	/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
	VoiceProfLineObj_t *pLine = getAssociatedLineObj(phoneCB);

	PH_FSM_PRINTF("vDspChanOpenRecvonly>ENTER, associatedVpItr = %d\n", phoneCB->associatedVpItr);
	PH_FSM_PRINTF("vDspChanOpenRecvonly>ENTER, SipAccountId = %d\n", phoneCB->associatedSipAccountId);
	if((phoneCB->associatedSipAccountId < 0) || (phoneCB->associatedSipAccountId > sipAccountNumber))
	{
		PH_FSM_DBG("vDspChanOpen>>\n");

		/* Generate call_id when outgoing call */
		if(phoneCB->callId == 0) {
			phoneCB->callId = phoneCB->logicalId;
		}
		
		msg_p=voiceMsgAlloc(VOICE_DSP_CHAN_OPEN_REQ,sizeof(voiceDspChanOpenReq_t));
		payload_p=(voiceDspChanOpenReq_t *)msg_p->payload;
		payload_p->logicalId=phoneCB->logicalId;
		payload_p->physicalId=phoneCB->physicalId;
		payload_p->portType=phoneCB->dspPortType;
		payload_p->call_id=phoneCB->callId;
		/* Follow TR-104 supoort DTMFMethod and DTMFMethodG711 both */
		payload_p->dtmfMethod = pVoiceProf->DTMFMethod;
		payload_p->dtmfMethodG711 = pVoiceProf->DTMFMethodG711;
		payload_p->silenceSupp = tr104GetLineCodecSilenceSuppression(phoneCB->associatedVpItr , LineItr);
	}else {
		LineItr = mapSipAccountIdToLineItr(phoneCB->associatedSipAccountId);
		VoiceProfLineVoiceProcessingObj_t *pVoiceProfLineProcessing = tr104VoiceLineProcessingObjPtrGet(phoneCB->associatedVpItr , LineItr);
			
		PH_FSM_DBG("vDspChanOpen>>\n");

		/* Generate call_id when outgoing call */
		if(phoneCB->callId == 0) {
			phoneCB->callId = phoneCB->logicalId;
		}
		
		msg_p=voiceMsgAlloc(VOICE_DSP_CHAN_OPEN_REQ,sizeof(voiceDspChanOpenReq_t));
		payload_p=(voiceDspChanOpenReq_t *)msg_p->payload;
		payload_p->logicalId=phoneCB->logicalId;
		payload_p->physicalId=phoneCB->physicalId;
		payload_p->portType=phoneCB->dspPortType;
		payload_p->call_id=phoneCB->callId;
		/* Follow TR-104 supoort DTMFMethod and DTMFMethodG711 both */
		payload_p->dtmfMethod = pVoiceProf->DTMFMethod;
		payload_p->dtmfMethodG711 = pVoiceProf->DTMFMethodG711;
		payload_p->echoCancel = pVoiceProfLineProcessing->echoCancellationEnable;
		payload_p->silenceSupp = tr104GetLineCodecSilenceSuppression(phoneCB->associatedVpItr , LineItr);
	}
	payload_p->mode = RECVONLY;
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);	

	if(VOICE_DSP_CHAN_OPEN_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspChanOpenRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	else{
		PH_FSM_DBG1("vDspChanOpen >> wrong api response received, api = ", rspMsg_p->hdr.api_id);
	}
	voiceMsgFree(rspMsg_p);

   /* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
	if(pLine != NULL && NULL != pLine->voiceProcessing){
		phSetEptRxGain(phoneCB, pLine->voiceProcessing->receiveGain); /*receiveGain in 0.1db unit*/
		phSetEptTxGain(phoneCB, pLine->voiceProcessing->transmitGain);/*transmitGain in 0.1db unit*/				
	}

	PH_FSM_PRINTF("vDspChanOpen return %d\n", err);

	return err;
} /* vDspChanOpenRecvonly */

/*______________________________________________________________________________
**  vDspChanClose
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
vDspChanClose(
	phoneCh_t *phoneCB
){
	int err = -1;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspChanCloseReq_t *payload_p = NULL;
	voiceDspChanCloseRsp_t *rspPayload_p = NULL;

	/*enable tone detection, it may be disabled when fax detected*/
	vDspEnableToneDetection(phoneCB);

	PH_FSM_DBG("vDspChanClose >>\n");
	/* Eric Chen mark, for PCM timeslots can't duplicate */
	msg_p=voiceMsgAlloc(VOICE_DSP_CHAN_CLOSE_REQ, sizeof(voiceDspChanCloseReq_t));
	payload_p=(voiceDspChanCloseReq_t *)msg_p->payload;
	payload_p->logicalId=phoneCB->logicalId;
	if(eventVoiceDspDataSend((uint8*)msg_p,(uint8**)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	voiceMsgFree(msg_p);
	PH_FSM_DBG("vDspChanClose >> send ok, recv resp ok\n");

	PH_FSM_PRINTF("vDspChanClose >> response api id %d\n",rspMsg_p->hdr.api_id);

	if(VOICE_DSP_CHAN_CLOSE_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspChanCloseRsp_t *)rspMsg_p->payload;
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);
	
	PH_FSM_PRINTF("vDspChanClose return %d\n", err);

	return err;
}

/*______________________________________________________________________________
**  vDspCreateInternalCall
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
vDspCreateInternalCall(
	phoneCh_t *phoneCB
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspCreateInternalCallReq_t *payload_p = NULL;
	voiceDspCreateInternalCallRsp_t *rspPayload_p = NULL;
	
	PH_FSM_DBG("vDspCreateInternalCall>>\n");

	msg_p = voiceMsgAlloc(VOICE_DSP_CREATE_INTERNAL_CALL_REQ, sizeof(voiceDspCreateInternalCallReq_t));
	payload_p = (voiceDspCreateInternalCallReq_t *)msg_p->payload;
	payload_p->logicalId = phoneCB->logicalId;
	
	PH_FSM_DBG2("vDspCreateInternalCall >> logicalId = ", phoneCB->logicalId, ", peer_logical_id ", phoneCB->peerMediaChId);
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_PRINTF("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspCreateInternalCall >> send ok, recv resp ok\n");

	if(VOICE_DSP_CREATE_INTERNAL_CALL_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspCreateInternalCallRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspCreateInternalCall return %d\n", err);

	return err;	
}
/*______________________________________________________________________________
**  vDspIVRFuncStart
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: TRUE indicate success, FALSE indicate fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int vDspIVRFunc(
	phoneCh_t *phoneCB,
	int priFunc,
	int secFunc,
	int action,
	int itemNum
){

	int err= FALSE;

	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspIvrFuncReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspIVRFunc>>\n");

	
	msg_p = voiceMsgAlloc(VOICE_DSP_IVR_FUNC_REQ, sizeof(voiceDspIvrFuncReq_t));
	payload_p = (voiceDspIvrFuncReq_t *)msg_p->payload;
	if(phoneCB == NULL){
		payload_p->call_id = 0; /*don't care, DELETE ivr item*/
	}
	else{
		payload_p->call_id = phoneCB->callId;
	}
	payload_p->priFunc = priFunc;
	payload_p->secFunc = secFunc;
	payload_p->action = action;
	payload_p->itemNum = itemNum;
	PH_FSM_DBG("vDspIVRFunc >>");
	PH_FSM_DBG1("call_id = ", payload_p->call_id);
	PH_FSM_DBG1("priFunc = ", priFunc);
	PH_FSM_DBG1("secFunc = ", secFunc);
	PH_FSM_DBG1("action = ", action);
	PH_FSM_DBG1("item = ", itemNum);
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return FALSE;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspIVRFunc >> send ok, recv resp ok\n");

	if(VOICE_DSP_IVR_FUNC_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = TRUE;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspIVRFunc return %d\n", err);

	return err;
}

/*______________________________________________________________________________
**  vDspStartEarlyMedia - aircheng modify
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: TRUE indicate success, FALSE indicate fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int vDspStartEarlyMedia(
	uint16 callId,
	uint8 ivrPlayIndex	/* aircheng add */
){

	int err= FALSE;

	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspEarlyMediaReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStartEarlyMedia>>\n");

	
	msg_p = voiceMsgAlloc(VOICE_DSP_START_EARLY_MEDIA_REQ, sizeof(voiceDspEarlyMediaReq_t));
	payload_p = (voiceDspEarlyMediaReq_t *)msg_p->payload;
	payload_p->call_id = callId;
	payload_p->ivrPlayIndex = ivrPlayIndex;	/* aircheng add */
	PH_FSM_DBG("vDspStartEarlyMedia >>");
	PH_FSM_DBG1("call_id = ", payload_p->call_id);
	PH_FSM_DBG1("ivrPlayIndex = ", payload_p->ivrPlayIndex);		/* aircheng add */
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return FALSE;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspStartEarlyMedia >> send ok, recv resp ok\n");

	if(VOICE_DSP_START_EARLY_MEDIA_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = TRUE;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStartEarlyMedia return %d\n", err);

	return err;
}

/*______________________________________________________________________________
**  vDspStopEarlyMedia - aircheng modify
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: TRUE indicate success, FALSE indicate fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int vDspStopEarlyMedia(
	uint16 callId
){

	int err= FALSE;

	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspEarlyMediaReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStopEarlyMedia>>\n");

	
	msg_p = voiceMsgAlloc(VOICE_DSP_STOP_EARLY_MEDIA_REQ, sizeof(voiceDspEarlyMediaReq_t));
	payload_p = (voiceDspEarlyMediaReq_t *)msg_p->payload;
	payload_p->call_id = callId;
	PH_FSM_DBG("vDspStopEarlyMedia >>");
	PH_FSM_DBG1("call_id = ", payload_p->call_id);
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return FALSE;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspStopEarlyMedia >> send ok, recv resp ok\n");

	if(VOICE_DSP_STOP_EARLY_MEDIA_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = TRUE;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStopEarlyMedia return %d\n", err);

	return err;
}

/*______________________________________________________________________________
**  vDspStartMusicOnHold - aircheng add
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: TRUE indicate success, FALSE indicate fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int vDspStartMusicOnHold(
	uint16 callId,
	uint16 physicalId,
	uint8 ivrPlayIndex
){

	int err= FALSE;

	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspMusicOnHoldReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStartMusicOnHold>>\n");

	
	msg_p = voiceMsgAlloc(VOICE_DSP_START_MUSIC_ONHOLD_REQ, sizeof(voiceDspMusicOnHoldReq_t));
	payload_p = (voiceDspMusicOnHoldReq_t *)msg_p->payload;
	payload_p->call_id = callId;
	payload_p->physicalId = physicalId;
	payload_p->ivrPlayIndex = ivrPlayIndex;
	PH_FSM_DBG("vDspStartMusicOnHold >>");
	PH_FSM_DBG1("call_id = ", payload_p->call_id);
	PH_FSM_DBG1("physicalId = ", payload_p->physicalId);
	PH_FSM_DBG1("ivrPlayIndex = ", payload_p->ivrPlayIndex);
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return FALSE;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspStartMusicOnHold >> send ok, recv resp ok\n");

	if(VOICE_DSP_START_MUSIC_ONHOLD_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = TRUE;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStartMusicOnHold return %d\n", err);

	return err;
}

/*______________________________________________________________________________
**  vDspStopMusicOnHold - aircheng add
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: TRUE indicate success, FALSE indicate fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int vDspStopMusicOnHold(
	uint16 callId, 
	uint16 physicalId
){

	int err= FALSE;

	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspMusicOnHoldReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStopMusicOnHold>>\n");

	
	msg_p = voiceMsgAlloc(VOICE_DSP_STOP_MUSIC_ONHOLD_REQ, sizeof(voiceDspMusicOnHoldReq_t));
	payload_p = (voiceDspMusicOnHoldReq_t *)msg_p->payload;
	payload_p->call_id = callId;
	payload_p->physicalId = physicalId;
	PH_FSM_DBG("vDspStopMusicOnHold >>");
	PH_FSM_DBG1("call_id = ", payload_p->call_id);
	PH_FSM_DBG1("physicalId = ", payload_p->physicalId);
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return FALSE;
	}
	
	voiceMsgFree(msg_p);

	PH_FSM_DBG("vDspStopMusicOnHold >> send ok, recv resp ok\n");

	if(VOICE_DSP_STOP_MUSIC_ONHOLD_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = TRUE;
		}
	}

	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStopMusicOnHold return %d\n", err);

	return err;
}

#ifdef HOWLER_TONE
/*______________________________________________________________________________
**  vDspStartHowlerTone
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
vDspStartHowlerTone(
	phoneCh_t *phoneCB
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspCommonReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStartHowlerTone>>\n");

	
	msg_p=voiceMsgAlloc(VOICE_DSP_START_HOWLER_TONE_REQ,sizeof(voiceDspCommonReq_t));
	payload_p=(voiceDspCommonReq_t *)msg_p->payload;
	payload_p->logicalId=phoneCB->logicalId;	
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);	

	if(VOICE_DSP_START_HOWLER_TONE_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStartHowlerTone return %d\n", err);

	return err;
	
	
} /* vDspStartHowlerTone */

/*______________________________________________________________________________
**  vDspStopHowlerTone
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
vDspStopHowlerTone(
	phoneCh_t *phoneCB
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspCommonReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;
	

	PH_FSM_DBG("vDspStopHowlerTone>>\n");

	/*stop howler tone timer*/
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

	msg_p=voiceMsgAlloc(VOICE_DSP_STOP_HOWLER_TONE_REQ,sizeof(voiceDspCommonReq_t));
	payload_p=(voiceDspCommonReq_t *)msg_p->payload;
	payload_p->logicalId=phoneCB->logicalId;	
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);	

	if(VOICE_DSP_STOP_HOWLER_TONE_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspCommonRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspStartHowlerTone return %d\n", err);

	return err;
	
	
} /* vDspStopHowlerTone */

#endif

//Ssu-Ying, support Multiple Profile
/* The below: Support Caller-ID */
/*______________________________________________________________________________
**  vDspSendCID
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
vDspSendCID(
	phoneCh_t *phoneCB
) {
	VoicePhyIntfObj_t * phyInterface;
	uint8 done = 0;
	time_t ltime;
	struct tm* sTime;
	
	if( phoneCB == NULL){
		return -1;
	}

	phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	if(phyInterface == NULL){
		return -1;
	}
	
	time(&ltime);
	sTime=localtime(&ltime);
	sprintf( phoneCB->dialInform.dialDigitNumber.dateTime,"%02i%02i%02i%02i", sTime->tm_mon+1, sTime->tm_mday, sTime->tm_hour, sTime->tm_min);
	
	switch(phyInterface->ZyXEL_CallerIDPayload) {
				case MDMF_CID:				
				case ETSI_MDMF_CID:			
				case SDMF_CID:					
					vDSPStartCIDFSK(phoneCB);
					done = 1;					
					break;
				case DTMF_CID:
					vDSPStartCIDDTMF(phoneCB);					
					done = 1;
					break;
				default:
					if ( ( phoneCB->phoneFlags & PHONE_PHYSICAL_USAGE ) && ( phoneCB->phStatus == PHONE_STATUS_RING ) ) {
						phStartRing(phoneCB);
					}
					done = 0;
					break;
	}
	return done;
}


/*______________________________________________________________________________
**  vDSPStartCIDFSK
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
/*for Broadcom, call PhoneStartRing anyway. country code will be referenced in driver*/

int 
vDSPStartCIDFSK(
	phoneCh_t *phoneCB
) {	
	int err = -1;
	VoicePhyIntfObj_t * phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	PH_FSM_DBG("\r\n vDSPStartCIDFSK ");
	if(NULL != phyInterface){
		switch(phyInterface->ZyXEL_CallerIDType) {
			case PRIOR_RING:
			case DURING_RING:		
					 err=PhoneStartRing(phoneCB);	
				break;
		}
	}

	return err;

}

/*______________________________________________________________________________
**  vDSPStartOffHookCIDFSK
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
vDSPStartOffHookCIDFSK(
	phoneCh_t *phoneCB
) {

	return 0;
}


/*______________________________________________________________________________
**  vDSPStopCIDFSK
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
vDSPStopCIDFSK(
	phoneCh_t *phoneCB
) {
	return 0;
}
//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  vDSPStartCIDDTMF
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

/*for Broadcom, call PhoneStartRing anyway. country code will be referenced in driver*/
int vDSPStartCIDDTMF(phoneCh_t *phoneCB)
{
	int err = -1;
	VoicePhyIntfObj_t * phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	PH_FSM_DBG("\r\n vDSPStartCIDDTMF ");
	if(NULL != phyInterface){
		switch(phyInterface->ZyXEL_CallerIDType) {
			case PRIOR_RING:
			case DURING_RING:		
					 err=PhoneStartRing(phoneCB);	
				break;
		}
	}
	
	return err;
}
/*______________________________________________________________________________
**  vDSPStopCIDDTMF
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

int vDSPStopCIDDTMF(phoneCh_t *phoneCB)
{

   return 0;  
}

/*______________________________________________________________________________
**  vDSPDetectCIDFSK
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
vDSPDetectCIDFSK(
	phoneCh_t *phoneCB
) {

	return 0;
}




/*______________________________________________________________________________
**  phStartDtmfTone
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
vDspStartDtmfTone(
	phoneCh_t *phoneCB,
	uint16 onTime
){
//Ssu-Ying, support Multiple Profile
	int err = -1;
	VoicePhyIntfObj_t*	phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
	PH_FSM_DBG("\r\n vDspStartDtmfTone ");
		PH_FSM_DBG("\r\n ================================================= ");
	if(NULL != phyInterface){
		switch(phyInterface->ZyXEL_CallerIDType) {
			case PRIOR_RING:
			case DURING_RING:		
					 err=PhoneStartDtmfTone(phoneCB);	
				break;
		}
	}

	return err;
}



/*______________________________________________________________________________
**  dspStopDTMFTone
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
vDspStopDtmfTone(
	phoneCh_t *phoneCB
) {

	return 0;
}



/*______________________________________________________________________________
**  vDSPToneOn
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
vDSPToneOn(
	phoneCh_t *phoneCB,
	uint8 Type,
	uint16 onTime,
	uint16 offTime,
	uint16 repNum
){

	return 0;
}

/*______________________________________________________________________________
**  vDSPToneOff
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
vDSPToneOff(
	phoneCh_t *phoneCB
) {

	return 0;
}


/*______________________________________________________________________________
**  vDspEnableToneDetection
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
vDspEnableToneDetection(
	phoneCh_t *phoneCB
){
	PhoneEnableToneDetection(phoneCB);
}


/*______________________________________________________________________________
**  vDspDisableToneDetection
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
vDspDisableToneDetection(
	phoneCh_t *phoneCB
) {
	PhoneDisableToneDetection(phoneCB);
}



/*______________________________________________________________________________
**  vDspSetFAXParameter
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
vDspSetFAXParameter(
	phoneCh_t *phoneCB
){
#if 0
		int err= -1;
		voiceMsg_t *msg_p,*rspMsg_p = NULL;
		voiceDspSetFaxParameterReq_t *payload_p = NULL;
		voiceDspSetFaxParameterRsp_t *rspPayload_p = NULL;
		
	
		ZyPrintf("vDspSetFAXParameter>>\n");
	
		
		msg_p = voiceMsgAlloc(VOICE_DSP_SET_FAX_PARAMETER_REQ, sizeof(voiceDspSetFaxParameterReq_t));
		payload_p = (voiceDspSetFaxParameterReq_t *)msg_p->payload;
		payload_p->logicalId = phoneCB->logicalId;
		
		ZyPrintf("vDspSetFAXParameter >> logicalId %d\n", phoneCB->logicalId);
		if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
			ZyPrintf("eventVoiceDspDataSend failed\n");
			return -1;
		}
		
		voiceMsgFree(msg_p);
	
		ZyPrintf("vDspSetFAXParameter >> send ok, recv resp ok\n");
	
		if(VOICE_DSP_SET_FAX_PARAMETER_RSP == rspMsg_p->hdr.api_id){
			rspPayload_p = (voiceDspSetFaxParameterRsp_t *)rspMsg_p->payload;
			ZyPrintf("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
			if(MM_SUCCESS == rspPayload_p->result){
				err = 0;
			}
		}	
		voiceMsgFree(rspMsg_p);	
		return err;
#endif

	return 0;
}

/*______________________________________________________________________________
**  vDspSetFAXT38Mode
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
vDspSetFAXT38Mode(
	phoneCh_t *phoneCB, 
	T38Info_t *T38Info_p
) {

	return 0;
}



/*______________________________________________________________________________
**  vDspEnableVAD
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
vDspEnableVAD(
	phoneCh_t *phoneCB
) {

	return 0;
}

/*______________________________________________________________________________
**  vDspDisableVAD
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
vDspDisableVAD(
	phoneCh_t *phoneCB
) {

	return 0;
}

/*******************************************************************************
* FUNCTION: DSPGetAvailCodecList
********************************************************************************
* DESCRIPTION:
*   Get DSP Available Codec List
* ARGUMENTS:
*		None
* Return:
*			Codec List in unit16
*			-1: error while calling functions
*   - None.
*******************************************************************************/
uint32
DSPGetAvailCodecList(
	void
){
   return 0;
}/* DSPGetAvailCodecList */



/*******************************************************************************
* FUNCTION: DSPCheckCodecAvail
********************************************************************************
* DESCRIPTION:
*   Get DSP Available Codec List
* ARGUMENTS:
*		None
* Return:
*			  uint32: Codec Available
*			  0: Codec Not Available
*   - None.
*******************************************************************************/
uint32
DSPCheckCodecAvail(
	uint8 Codec
){

	return 0;
}/* DSPCheckCodecAvail */

#ifdef CALL_CONFERENCE
/*______________________________________________________________________________
**  mixChannel3WayCalling
**
**  descriptions:
**  parameters: type: 0 - PCM, 1 - Network.
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
mixChannel3WayCalling(
	uint16 channelin, 
	uint16 channelintype, 
	uint16 channelout, 
	uint16 channelouttype
) {
  	return 0;
}

/*______________________________________________________________________________
**  separateChannel3WayCalling
**
**  descriptions:
**  parameters: type: 0 - PCM, 1 - Network.
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
separateChannel3WayCalling(
	UINT16 channelin, 
	UINT16 channelintype, 
	UINT16 channelout, 
	UINT16 channelouttype
) {
	return 0;
}
int 
vDspCreateMeetingRoom(
	phoneCh_t *bPhoneCB_p
) {
	int err = -1;
	phoneCh_t *cPhoneCB_p = NULL;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCreateMeetingRoomReq_t *payload_p = NULL;
	voiceDspCreateMeetingRoomRsp_t *rspPayload_p = NULL;

	if ((cPhoneCB_p = getNextLogicPhone(bPhoneCB_p)) == NULL) {
		ZyPrintf("%s : Can't find cPhoneCB_p\n", __FUNCTION__);
		return -2;
	}
	
	/* Open held call physical dsp channel */
	if(vDspChanOpen(bPhoneCB_p) != 0){
		ZyPrintf("%s : Hold call vDspChanOpen fail\n", __FUNCTION__);
		return -3;
	}	

	/* Re-Open DSP channel again to support Multiple DSP(dspId > 1)
	* Somtimes channel on DSP pool will be moved to another DSP  
	* because DSP resource is not enough on oringnal DSP for
	* 3 way conference call.
	*/
	if(vDspChanOpen(cPhoneCB_p) != 0){
		ZyPrintf("%s : Active call vDspChanOpen fail\n", __FUNCTION__);
		return -4;
	}

	msg_p=voiceMsgAlloc(VOICE_DSP_CREATE_MEETING_ROOM_REQ, sizeof(voiceDspCreateMeetingRoomReq_t));
	payload_p=(voiceDspCreateMeetingRoomReq_t *)msg_p->payload;
	payload_p->bLogicId=bPhoneCB_p->logicalId;

	if(eventVoiceDspDataSend((uint8*)msg_p, (uint8**)&rspMsg_p) != 0){
		ZyPrintf("%s : eventVoiceDspDataSend failed\n", __FUNCTION__);
		return -1;
	}
	voiceMsgFree(msg_p);

	if(VOICE_DSP_CREATE_MEETING_ROOM_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCreateMeetingRoomRsp_t *)rspMsg_p->payload;
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);

	return err;	
}	


int 
vDspDestroyMeetingRoom(
	uint16 baseLogicalId
) {
	int err = -1;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspDestroyMeetingRoomReq_t *payload_p = NULL;
	voiceDspDestroyMeetingRoomRsp_t *rspPayload_p = NULL;
	msg_p=voiceMsgAlloc(VOICE_DSP_DESTROY_MEETING_ROOM_REQ, sizeof(voiceDspDestroyMeetingRoomReq_t));
	payload_p=(voiceDspDestroyMeetingRoomReq_t *)msg_p->payload;

	payload_p->bLogicId=baseLogicalId;
	PH_FSM_PRINTF("%s, bLogicalId = %d\n", __FUNCTION__, payload_p->bLogicId);
	if(eventVoiceDspDataSend((uint8*)msg_p,(uint8**)&rspMsg_p)!=0){
		ZyPrintf("%s : eventVoiceDspDataSend failed\n", __FUNCTION__);
		return -1;
	}
	voiceMsgFree(msg_p);

	if(VOICE_DSP_DESTROY_MEETING_ROOM_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspDestroyMeetingRoomRsp_t *)rspMsg_p->payload;
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);
	return err;
}


int 
vDspCreateLocalPeerMeetingRoom(
	phoneCh_t* bPhoneCB_p
) {
	int err = -1;
	phoneCh_t *cPhoneCB_p = NULL, *peerLocalPhoneCB = NULL;;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCreateMeetingRoomReq_t *payload_p = NULL;
	voiceDspCreateMeetingRoomRsp_t *rspPayload_p = NULL;

	if ((cPhoneCB_p = getNextLogicPhone(bPhoneCB_p)) == NULL) {
		ZyPrintf("%s : Can't find newPhoneCB\n", __FUNCTION__);
		return -2;
	}

	if((peerLocalPhoneCB = find_phoneCB(bPhoneCB_p->localConfPeerLocalPhLogicalId, 0)) == NULL ){
		ZyPrintf("%s : Can't find peerLocalPhoneCB\n", __FUNCTION__);
		return -3;
	}

/* MULTIPLE_DSP */
	if(vDspChanOpen(cPhoneCB_p) != 0){
		ZyPrintf("%s : Active call vDspChanOpen fail\n", __FUNCTION__);
		return -4;
	}

	if (vDspChanOpen(peerLocalPhoneCB) != 0) 
	{
		ZyPrintf("%s : peerLocal call vDspChanOpen fail\n", __FUNCTION__);
		return -5;
	}	
/* MULTIPLE_DSP */

	msg_p=voiceMsgAlloc(VOICE_DSP_CREATE_MEETING_ROOM_REQ, sizeof(voiceDspCreateMeetingRoomReq_t));
	payload_p=(voiceDspCreateMeetingRoomReq_t *)msg_p->payload;
	payload_p->bLogicId=bPhoneCB_p->logicalId;

	if(eventVoiceDspDataSend((uint8*)msg_p, (uint8**)&rspMsg_p) != 0){
		ZyPrintf("%s : eventVoiceDspDataSend failed\n", __FUNCTION__);
		return -1;
	}
	voiceMsgFree(msg_p);

	if(VOICE_DSP_CREATE_MEETING_ROOM_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspCreateMeetingRoomRsp_t *)rspMsg_p->payload;
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);

	return err;	
}

int 
vDspDestroyLocalPeerMeetingRoom(
	uint16 baseLogicalId
) {
	int err = -1;
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspDestroyMeetingRoomReq_t *payload_p = NULL;
	voiceDspDestroyMeetingRoomRsp_t *rspPayload_p = NULL;
	msg_p=voiceMsgAlloc(VOICE_DSP_DESTROY_MEETING_ROOM_REQ, sizeof(voiceDspDestroyMeetingRoomReq_t));
	payload_p=(voiceDspDestroyMeetingRoomReq_t *)msg_p->payload;

	payload_p->bLogicId=baseLogicalId;
	
	if(eventVoiceDspDataSend((uint8*)msg_p,(uint8**)&rspMsg_p)!=0){
		ZyPrintf("%s : eventVoiceDspDataSend failed\n", __FUNCTION__);
		return -1;
	}
	voiceMsgFree(msg_p);

	if(VOICE_DSP_DESTROY_MEETING_ROOM_RSP == rspMsg_p->hdr.api_id){
		rspPayload_p = (voiceDspDestroyMeetingRoomRsp_t *)rspMsg_p->payload;
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;
		}
	}
	voiceMsgFree(rspMsg_p);
	return err;
}
#endif

#ifdef VOIP_SYSLOG /*Jason , syslog*/

/*______________________________________________________________________________
**  vDspStreamStatsReq
**
**  descriptions:
**  parameters:
**  local:
**  global:
**  return: 0 success, -1 fail
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int 
vDspStreamStatsReq(
	uint32 lineID,
	uint8  phy_port,
	Config_VoIP_Stats_t *stats
){
	int err= -1;
	voiceMsg_t *msg_p,*rspMsg_p = NULL;
	voiceDspMmStatsReq_t *payload_p = NULL;
	voiceDspMmStatsRsp_t *rspPayload_p = NULL;

	
#if 1
	PH_FSM_DBG("vDspStreamStatsReq>>\n");

	msg_p = voiceMsgAlloc(VOICE_MM_GET_STREAM_LOG_REQ, sizeof(voiceDspMmStatsReq_t));
	if( NULL == msg_p ){
		return -1;
	}
	payload_p=(voiceDspMmStatsReq_t *)msg_p->payload;
	payload_p->line_id = lineID;
	payload_p->phy_port = phy_port;
	
	if(eventVoiceDspDataSend((uint8*)msg_p ,(uint8 **)&rspMsg_p) != 0){
		voiceMsgFree(msg_p);	
		PH_FSM_DBG("eventVoiceDspDataSend failed\n");
		return -1;
	}
	
	voiceMsgFree(msg_p);	

	if(VOICE_MM_GET_STREAM_LOG_RSP == rspMsg_p->hdr.api_id){
 		rspPayload_p = (voiceDspMmStatsRsp_t *)rspMsg_p->payload;
		PH_FSM_PRINTF("%s >> result = %d\n",__FUNCTION__, rspPayload_p->result);
		if(MM_SUCCESS == rspPayload_p->result){
			err = 0;

			stats->SessionDuration = rspPayload_p->SessionDuration;
			stats->PacketsSent = rspPayload_p->txPktCnt;
			stats->PacketsReceived = rspPayload_p->rxPktCnt;
			stats->BytesSent = rspPayload_p->txOctetCnt;
			stats->BytesReceived = rspPayload_p->rxOctetCnt;
			stats->PacketsLost = rspPayload_p->lostPktCnt;

			stats->AverageReceiveInterarrivalJitter = rspPayload_p->avgRcvJitter;

		
			PH_FSM_DBG("get data success...>>\n");
		}
		else{
			PH_FSM_DBG1("vDspStreamStatsReq >> wrong result(%d)", rspPayload_p->result);
		}
	}
	else{
		PH_FSM_DBG1("vDspStreamStatsReq >> wrong api response received, api = ", rspMsg_p->hdr.api_id);
	}
fx_exti:
	voiceMsgFree(rspMsg_p);

	PH_FSM_PRINTF("vDspChanOpen return %d\n", err);
#endif	
	return err;
} /* vDspChanOpen */

#endif

