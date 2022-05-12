/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/
#include"mm_core_fsm_types.h"
#include"mm_macros.h"
#include"mm_port_wrapper.h"
#include"mm_core_fsm_defs.h"
#include"mm_core_prototypes.h"
#include"mm_port_prototypes.h"
#include"icf_types.h"
#include"icf_api.h"
#include"mm_glb.h"
#include"icf_defs.h"
#include "voice_dsp_core_fsm_types.h"
#include "global.h"
#include "switch.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#include "endpt.h"
#include <pthread.h>
#include "dsp_driver_itf.h"
#include "tr104_object.h"

/********************************************************************
*						extern functions prototypes
*********************************************************************/
extern
dspSendToDsp(
	icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type,
	uint32 null_pload_type, 
	uint8 *InMessage, 
	uint16 MessageLen, 
	uint32 TimeStamp, 
	void* handle
);


/********************************************************************
*						voice dsp core mmi functions prototypes
*********************************************************************/

mm_return_t
voiceDspCoreInit(
  		void						     *inputParam_p,
		mm_error_t					*err_p);


mm_return_t
voiceDspCoreChanOpen(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreChanClose(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreCreateMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreDestroyMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreCreateLocalpeerMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreDestroyLocalpeerMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreCreateInternalCall(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreIvrFunc(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreSetFaxParameter(
  		void						     *inputParam_p,
		mm_error_t					*err_p);
#ifdef HOWLER_TONE
mm_return_t
voiceDspCoreStartHowlerTone(
  		void						     *inputParam_p,
		mm_error_t					*err_p);
mm_return_t
voiceDspCoreStopHowlerTone(
  		void						     *inputParam_p,
		mm_error_t					*err_p);
#endif

mm_return_t
voiceDspCoreStartEarlyMedia(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreStopEarlyMedia(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

#if 1	/* aircheng add for MOH*/
mm_return_t
voiceDspCoreStartMusicOnHold(
  		void						     *inputParam_p,
		mm_error_t					*err_p);

mm_return_t
voiceDspCoreStopMusicOnHold(
  		void						     *inputParam_p,
		mm_error_t					*err_p);
#endif

mm_return_t
voice_dsp_core_ignore(
		void						*inputParam_p,
		mm_error_t					*err_p);

#ifdef VOIP_SYSLOG /*Jason , syslog*/
mm_return_t voiceDspLogStatsFunc(
  		void						     *inputParam_p,
		mm_error_t					*err_p);
#endif

/********************************************************************
*						utility functions prototypes
*********************************************************************/


mm_return_t voiceEventIpcSendToVoiceApp(voiceMsg_t * msg_p);


/********************************************************************
*						dsp pool functions prototypes
*********************************************************************/
void dspPoolInit(void);
dspChan_t *dspPoolAllocateDspChan(void);
dspChan_t * dspPoolCreateDspChan(uint16 physicalId,uint16 logicalId, uint8 portType, icf_call_id_t call_id, icf_stream_id_t stream_id, uint8 codec_num);

void dspPoolDeleteDspChan(dspChan_t *dspChan_p);
dspConf_t * dspPoolCreateDspConf(voiceDsp_t * voiceDsp_p, dspChan_t *bDspChan_p, dspChan_t *cDspChan_p);
void dspPoolDeleteDspConf(dspConf_t * dspConf_p);

dspChan_t * dspPoolFindDspChanByLogicalId(uint16 logicalId);
#if 1	/* aircheng add */
dspChan_t * dspPoolFindDspChanByCallIdPhysicalId(uint16 call_id, uint16 physicalId);
dspChan_t * dspPoolFindAnotherDspChanByCallIdPhysicalId(uint16 call_id, uint16 physicalId);
dspChan_t * dspPoolFindDspChanByLogicalIdPhysicalId(uint16 logicalId, uint16 physicalId);
dspChan_t * dspPoolFindAnotherDspChanByLogicalIdPhysicalId(uint16 logicalId, uint16 physicalId);
#endif
dspChan_t * dspPoolFindDspChanByCallId(uint16 call_id);
dspChan_t * dspPoolFindDspChan(icf_call_id_t	call_id, uint16 logicalId,	uint16 physicalId);
dspChan_t * dspPoolFindAnotherDspChanByCallId(uint16 call_id, uint16 logicalId);
dspChan_t * dspPoolFindDspChanByCallIdLogicalId(uint16 call_id, uint16 logicalId);
dspChan_t * dspPoolFindDspChanByChanId(uint16 dspId, uint16 chanId);
dspChan_t * dspPoolFindDspChanByStreamId(uint16 call_id, uint16 stream_id);
voiceDsp_t *dspPoolFindDspByDspId(uint16 dspId);
dspConf_t *dspPoolFindDspConfByLogicalId(uint16 confId);
voiceDsp_t *dspPoolFindVoiceDspByDspId(uint16 dspId);
dspChan_t * dspPoolFindDspChanByAllId(icf_uint16_t dspId, icf_call_id_t call_id,icf_stream_id_t stream_id,icf_media_type_t stream_type);
mm_return_t dspPoolCheckDspCodecRm(uint16 dspId, uint8 codec_num);
uint16 dspPoolCheckCodecRm(uint8 codec_num);
void dspPoolDumpDspChan(dspChan_t *dspChan_p);
dspChan_t *dspPoolFindLogicalDspChan(dspChan_t *dspChan_p);
mm_return_t dspPoolCheckChannelUsage(uint16 dspId, uint16 req_channel_usage);
uint16 dspPoolChannelUsage(uint8 codec_num);
void dspPoolDumpDspPool(void);
void dspPoolDumpConfDspPool(void);
void dspPoolDumpDspStatus(void);
void dspPoolInitMutex(void);
void dspPoolAcquireMutex(void);
void dspPoolReleaseMutex(void);
void dspPoolCopyMediaInfoFromDspChan(dspChan_t *p_to_dsp_chan, dspChan_t *p_from_dsp_chan);
dspChan_t* dspPoolDspChanReplaceAudioByT38(dspChan_t *dspChan_p);
mm_return_t dspPoolModifyDspCodecRm(uint16 dspId, uint8 codec_num, uint8 new_codec_num);
mm_return_t dspPoolCreateEarlyMediaChan(uint16 dspId, uint8 codec_num);
mm_return_t dspPoolCheckDspCodecRm(uint16 dspId, uint8 codec_num);


/********************************************************************
*						Glocal variables
*********************************************************************/
static dspPool_t gDspPool;
dspPool_t *gDspPool_p = &gDspPool;
extern mmCoreCallCtxList_t *p_call_context_list;
extern icf_int32_t gMmFd ;
extern struct sockaddr_un  g_voiceApp_addr;
extern int	rtpRecord(icf_call_id_t call_id, icf_stream_id_t stream_id, icf_media_type_t stream_type, uint16 logicalId, icf_uint32_t pload_type, icf_uint8_t *packet, icf_uint16_t len, icf_uint32_t timestamp, void* handle);
extern int mmKSocketEnable;

pthread_mutex_t mutexDspChan = PTHREAD_MUTEX_INITIALIZER; 

voiceDspCoreStateEventFprtTy  voiceDspCoreStateEvent[DSP_LAST_STATE][DSP_CORE_LAST_EVENT] =
{
	/* Idle state */
	{
		voiceDspCoreInit,
		voiceDspCoreChanOpen,			/* VOICE_DSP_CHAN_OPEN_REQ  */
		voiceDspCoreChanClose,  		/* VOICE_DSP_CHAN_CLOSE_REQ  */		
		voiceDspCoreCreateMeetingRoom,			/* VOICE_DSP_CREATE_MEETING_ROOM_REQ */
		voiceDspCoreDestroyMeetingRoom,			/* VOICE_DSP_DESTROY_MEETING_ROOM_REQ */		
		voiceDspCoreCreateInternalCall,					/*VOICE_DSP_CREATE_INTERNAL_CALL_REQ*/
		voiceDspCoreSetFaxParameter,						/*VOICE_DSP_SET_FAX_PARAMETER_REQ*/
		#ifdef HOWLER_TONE
		voiceDspCoreStartHowlerTone,							/*VOICE_DSP_START_HOWLER_TONE_REQ*/
		voiceDspCoreStopHowlerTone,							/*VOICE_DSP_STOP_HOWLER_TONE_REQ*/
		#endif
		voiceDspCoreIvrFunc,								/*VOICE_DSP_IVR_FUNC_REQ*/
		voiceDspCoreStartEarlyMedia,						/*VOICE_DSP_START_EARLY_MEDIA_REQ*/
		voiceDspCoreStopEarlyMedia,							/*VOICE_DSP_STOP_EARLY_MEDIA_REQ*/
#if 1	/* aircheng add for MOH */
		voiceDspCoreStartMusicOnHold,						/*VOICE_DSP_START_MUSIC_ONHOLD_REQ*/
		voiceDspCoreStopMusicOnHold,						/*VOICE_DSP_STOP_MUSIC_ONHOLD_REQ*/
#endif
#ifdef VOIP_SYSLOG /*Jason , syslog*/
		voiceDspLogStatsFunc,			/*DSP_MM_STREAMLOG*/
#endif
	}	

};
#define MM_DSP_DBG(fs)						if (mmDspDebug & 1)	{		ZyPrintf((fs));			\
																	ZyPrintf("\n\r"); }
#define MM_DSP_DBG1(fs, v1)				if (mmDspDebug & 1)	{	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf("\n\r"); }
#define MM_DSP_DBG2(fs, v1, f2, v2)			if (mmDspDebug & 1) {	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf(f2); \
																ZyPrintf("0x%x", v2); \
																ZyPrintf("\n\r"); }

/******************************************************************************
 * voiceEventIpcSendToVoiceApp
 *
 * Description: 
 *	IPC function, send voice message (VOICE_DSP api response) to voiceApp
 *
 * Parameters:
 * 	Input
 *    voiceMsg_t * msg_p - voice message to send
 *
 * Return Value: uint8
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceEventIpcSendToVoiceApp(voiceMsg_t * msg_p){
	return voiceEventIpcSend(gMmFd,(struct sockaddr *)&g_voiceApp_addr,sizeof(struct sockaddr_un),msg_p);
}

/******************************************************************************
 * voiceDspCoreInit
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_INIT_REQ
 *	This function should do dsp init, return init data (in response message) needed for phfsm
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -not used here
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreInit(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{	
	voiceMsg_t *rspMsg_p = NULL, *msg_p = NULL;
	voiceDspInitRsp_t *rspPayload_p = NULL;
	voiceDspInitReq_t *payload_p = NULL;		
	VDSP_PRINTF("MM:Entering %s\n",__FUNCTION__);


	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	msg_p = (voiceMsg_t *)inputParam_p;
	payload_p = (voiceDspInitReq_t *)msg_p->payload;
	VDSP_PRINTF("voiceDspCoreInit  country = %d\n", payload_p->country);	
	if(MM_SUCCESS != dspDriverInit(payload_p->country))
	{
		ZyPrintf("dspDriverInit failed, exiting\n");
		exit(EXIT_FAILURE); /*dsp init fail, exit program*/
	}	
	
	/*prepare voice msg*/
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_INIT_RSP,sizeof(voiceDspInitRsp_t));
	rspPayload_p = (voiceDspInitRsp_t *)rspMsg_p->payload;	

	/*dsp initilization*/

	dspDriverGetInitData(&(rspPayload_p->initData), &(rspPayload_p->initDataLength));

	rspMsg_p->hdr.api_length += rspPayload_p->initDataLength;
	rspPayload_p->result = MM_SUCCESS;
	
	/*voice msg ipc send*/
	voiceEventIpcSendToVoiceApp(rspMsg_p);

	/*free voice msg */	
	voiceMsgFree(rspMsg_p);
	VDSP_PRINTF("voiceDspCoreInit  country = %d\n", payload_p->country);
	VDSP_PRINTF("MM:Exiting %s\n",__FUNCTION__);
	return MM_SUCCESS;	
}


/******************************************************************************
 * voiceDspCoreChanOpen
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CHAN_OPEN_REQ
 *	Create dsp_chan structure in dsp pool, Open dsp channel through dsp driver
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspChanOpenReq_t 
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreChanOpen(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{
	voiceMsg_t						*msg_p = NULL, *rspMsg_p=NULL;
	voiceDspChanOpenReq_t 			*payload_p = NULL;
	voiceDspChanOpenRsp_t 			*rspPayload_p = NULL;	
	
	mm_return_t ret_val = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *p_same_call_dsp_chan = NULL, *dspChanFound_p = NULL;	
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;

	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);		
	dspPoolDumpDspPool();

	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	msg_p = (voiceMsg_t *)inputParam_p;
	payload_p = (voiceDspChanOpenReq_t *)msg_p->payload;
	
	MM_DSP_DBG("\r\n	DSP OPEN>");
	MM_DSP_DBG1("	DSP OPEN> logicalId = ", payload_p->logicalId);
	MM_DSP_DBG1("	DSP OPEN> physicalId = ", payload_p->physicalId);
	MM_DSP_DBG1("	DSP OPEN> portType = ", payload_p->portType);
	MM_DSP_DBG1("	DSP OPEN> call_id = ", payload_p->call_id);
	MM_DSP_DBG1("	DSP OPEN> dtmfMethodG711 = ", payload_p->dtmfMethodG711);
	MM_DSP_DBG1("	DSP OPEN> dtmfMethod = ", payload_p->dtmfMethod);
	MM_DSP_DBG1("	DSP OPEN> silenceSupp = ", payload_p->silenceSupp);
	MM_DSP_DBG1("	DSP OPEN> echoCancel = ", payload_p->echoCancel);
	MM_DSP_DBG1("	DSP OPEN> mode = ", payload_p->mode);
	ret_val = MM_SUCCESS;	

	/*traversal dsp pool, found dsp chan state of WAIT_FOR_CLOSE, delete dsp chan and close it in driver*/
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &gDspPool_p->voiceDsp[i];
		if(NULL == voiceDsp_p){
			MM_DSP_DBG("voice dsp null\n");
			continue;
		}
		
		for(j=0;j<MAX_DSP_CHAN_PER_DSP;j++){
			if(NULL != voiceDsp_p->dspChanList[j]){
				dspChan_p = voiceDsp_p->dspChanList[j];
				/*find matched call-id, stream_id no value*/
				if(CHAN_WAIT_FOR_CLOSE == dspChan_p->chanState){					
					dspPoolAcquireMutex();
					dspDriverChanClose(dspChan_p);
					dspPoolDeleteDspChan(dspChan_p);
					dspPoolReleaseMutex();
				}	
				
			} /*NULL != voiceDsp_p->dspChanList[j]*/
		} /*for j*/
	} /*for i*/
	
	dspChan_p = NULL;
	
	dspPoolAcquireMutex();
	/* find dsp chan matching call_id, physicalId, logicalId or COMMITTED */
	dspChan_p = dspPoolFindDspChan(payload_p->call_id, payload_p->logicalId, payload_p->physicalId);

	

	/*replace t38 channel to original audio channel*/
	if(NULL != dspChan_p && ZYNOS_CODEC_T38 == dspChan_p->codec_num && CHAN_COMMITTED == dspChan_p->chanState){	
		dspChan_p->phLogicalId = payload_p->logicalId;
		dspChan_p->phPhysicalId = payload_p->physicalId;
		dspChan_p->phPortType = payload_p->portType;
		dspChan_p = dspPoolDspChanReplaceAudioByT38(dspChan_p);
		if(NULL == dspChan_p){
			MM_DSP_DBG("MM > t.38 ERROR\n");			
		}
	}

	
	/* dsp channel not found, create new dsp channel, and open dsp channel in driver*/
	if(NULL == dspChan_p){	
		MM_DSP_DBG("MM VDSP > DSP create new dsp chan\n");
		
		/*check for same call_id dsp chan, incomming call, 2 fxs open dsp chan when ringing, same call_id, different logicalId, physicalId*/
		p_same_call_dsp_chan = dspPoolFindAnotherDspChanByCallId(payload_p->call_id,payload_p->logicalId);
		if(NULL != p_same_call_dsp_chan ){
			dspChan_p = dspPoolCreateDspChan(payload_p->physicalId, payload_p->logicalId, payload_p->portType, payload_p->call_id, STREAM_ID_NOT_USED, p_same_call_dsp_chan->codec_num);
			dspPoolCopyMediaInfoFromDspChan(dspChan_p, p_same_call_dsp_chan);
		}
		else{		
			dspChan_p = dspPoolCreateDspChan(payload_p->physicalId, payload_p->logicalId, payload_p->portType, payload_p->call_id, STREAM_ID_NOT_USED, ZYNOS_CODEC_PCMU);
		}
		
#if 1	/* aircheng add for MOH when dsp resouce is not enough */
		if(NULL == dspChan_p){
			/* delete mon dsp chan, and close mon dsp chan in driver */
			for( i = 0; i < gDspPool_p->dspCount; i++ ){
				voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
				for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
					if( (voiceDsp_p->dspChanList[j] != NULL) && 
						(voiceDsp_p->dspChanList[j]->phPhysicalId == MOH_PHY_ID)){
						VDSP_PRINTF("MM: %s : dsp chan[%d] found, to be deleted\n", __FUNCTION__, j);
						dspChanFound_p = voiceDsp_p->dspChanList[j];
						ivrPlayStop(dspChanFound_p);
						dspDriverChanClose(dspChanFound_p);
						dspPoolDeleteDspChan(dspChanFound_p);
					}
				}
			}
			/* create new dsp channel, and open dsp channel in driver*/
			MM_DSP_DBG("MM VDSP > DSP create new dsp chan\n");
			if(NULL != p_same_call_dsp_chan ){
				dspChan_p = dspPoolCreateDspChan(payload_p->physicalId, payload_p->logicalId, payload_p->portType, payload_p->call_id, STREAM_ID_NOT_USED, p_same_call_dsp_chan->codec_num);
				dspPoolCopyMediaInfoFromDspChan(dspChan_p, p_same_call_dsp_chan);
			}
			else{		
				dspChan_p = dspPoolCreateDspChan(payload_p->physicalId, payload_p->logicalId, payload_p->portType, payload_p->call_id, STREAM_ID_NOT_USED, ZYNOS_CODEC_PCMU);
			}
		}
#endif

		/*dsp driver: open dsp channel */
		if(NULL != dspChan_p){
			ret_val = MM_SUCCESS;				
			dspChan_p->dtmfMethod = payload_p->dtmfMethod;
			dspChan_p->dtmfMethodG711 = payload_p->dtmfMethodG711;
			dspChan_p->silenceSupp = payload_p->silenceSupp;
			dspChan_p->echoCancel = payload_p->echoCancel;
			if(dspDriverChanOpen(dspChan_p, payload_p->mode) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan open.....FAILED\n");
				ret_val=MM_FAILURE;
			}
			dspChan_p->chanState = CHAN_OPENED;

			
						
		}
		else{
			MM_DSP_DBG("MM VDSP > DSP Pool create dsp chan....FAILED\n");
			ret_val=MM_FAILURE;
		}

		

	}
	else{ 	
		/*check dsp chan state*/
		MM_DSP_DBG1("MM VDSP > DSP chan state = ", dspChan_p->chanState);
		dspChan_p->dtmfMethod = payload_p->dtmfMethod;
		dspChan_p->dtmfMethodG711 = payload_p->dtmfMethodG711;
		dspChan_p->silenceSupp = payload_p->silenceSupp;
		dspChan_p->echoCancel = payload_p->echoCancel;
		switch(dspChan_p->chanState){			
			case CHAN_OPENED:				
				ret_val = dspDriverChanModify(dspChan_p, payload_p->mode);
				break;
			case CHAN_COMMITTED:
			case CHAN_RESERVED:	
			case CHAN_IDLE:				
				dspChan_p->phLogicalId = payload_p->logicalId;
				dspChan_p->phPhysicalId = payload_p->physicalId;		
				dspChan_p->phPortType = payload_p->portType;						
				ret_val = dspDriverChanOpen(dspChan_p, payload_p->mode);
				dspChan_p->chanState = CHAN_OPENED;				
				break;
			case CHAN_WAIT_FOR_CLOSE:		
			case CHAN_CHANGE_TO_RESERVED:	
			default:
				MM_DSP_DBG1("dsp channel unexpected state = ", dspChan_p->chanState);
				break;
		}
			
		
	}

	dspPoolReleaseMutex();
	
	/*print dsp poll information*/
	dspPoolDumpDspPool();
	

	/* send response*/

	/*1. allocate voice msg*/
	rspMsg_p=voiceMsgAlloc(VOICE_DSP_CHAN_OPEN_RSP,sizeof(voiceDspChanOpenRsp_t));

	/*2. fill msg payload*/
	rspPayload_p = (voiceDspChanOpenRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret_val;
	
	/*3. send voice event*/
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	
	/*4. free voice message buffer*/			
	voiceMsgFree(rspMsg_p);
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret_val);	
	return MM_SUCCESS;	
}


/******************************************************************************
 * voiceDspCoreChanClose
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CHAN_CLOSE_REQ
 *	Delete dsp_chan structure in dsp pool, Close dsp channel through dsp driver
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspChanCloseReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreChanClose(
  		void						     *inputParam_p,	
		mm_error_t					*err_p)

{
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspChanCloseReq_t *payload_p = NULL;
	voiceDspChanCloseRsp_t *rspPayload_p = NULL;
	dspChan_t	*dspChan_p = NULL, *earlyMediaChan_p = NULL;
	dspChan_t	*musicOnHoldChan_p = NULL, *dspChanCopy_p = NULL;
	mm_return_t ret = MM_SUCCESS;
	int i = 0, j = 0;
	voiceDsp_t *voiceDsp_p;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);		
	dspPoolDumpDspPool();
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	msg_p = (voiceMsg_t *)inputParam_p;
	payload_p = (voiceDspChanCloseReq_t *)msg_p->payload;

	MM_DSP_DBG1("\r\nMM VDSP > logicalId = ", payload_p->logicalId);
			
	dspPoolAcquireMutex();

	/*search dsp pool for logical id*/
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &gDspPool_p->voiceDsp[i];
		

		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			
			dspChan_p = voiceDsp_p->dspChanList[j];			
			if((NULL != dspChan_p) && (payload_p->logicalId == dspChan_p->phLogicalId) &&
				(MOH_PHY_ID != dspChan_p->phPhysicalId)){		/* aircheng modify for MOH*/
				MM_DSP_DBG2("MM VDSP > dsp chan found[",dspChan_p->chanId,"], state = ", dspChan_p->chanState);
				ivrRecordHalt(0);
				ivrPlayStop(dspChan_p);
				
#ifdef EARLY_MEDIA_NO_CHANGE_SDP
				/* close replicated early media channel if it exists */
				earlyMediaChan_p = dspPoolFindDspChan(ID_EARLYMEDIA, ID_EARLYMEDIA, dspChan_p->phPhysicalId);
				
				if(NULL != earlyMediaChan_p) {
					ivrPlayStop(earlyMediaChan_p);
					/*1. dsp driver: close early media chan*/
					ret = dspDriverChanClose(earlyMediaChan_p);
					earlyMediaChan_p->chanState = CHAN_RESERVED;
					
					if(MM_SUCCESS != ret){
						MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED\n");
					}		
					else{
						/*2. dsp pool delete early media chan*/
						MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
						dspPoolDeleteDspChan(earlyMediaChan_p);	
					}			
				}
#endif

				if(CHAN_CHANGE_TO_RESERVED == dspChan_p->chanState){
					
					ret = dspDriverChanClose(dspChan_p);
					if(MM_SUCCESS == ret){
						dspChan_p->chanState = CHAN_RESERVED;
					}
					else{
						MM_DSP_DBG("MM VDSP > change to reserved failed\n");
					}
					
				}
				else if(CHAN_OPENED == dspChan_p->chanState){
					/*1. dsp driver: close dsp chan*/
					
					ret=dspDriverChanClose(dspChan_p);
					dspChan_p->chanState = CHAN_RESERVED;
					
					if(MM_SUCCESS != ret){
						MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED");
					}		
					else{
						/*2. dsp pool delete dsp chan*/
						MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
						dspPoolDeleteDspChan(dspChan_p);	
					}			
					
				}
				else{					
					dspPoolDeleteDspChan(dspChan_p);					
				}
			}
			
			
		}
		
	}
	
#if 1	/* aircheng add - search dsp pool for logical id & physical id to close dspChanCopy_p & musicOnHoldChan_p */

	dspChanCopy_p = dspPoolFindDspChanByLogicalIdPhysicalId(payload_p->logicalId, MOH_PHY_ID);	/* find dspChanCopy */
	musicOnHoldChan_p = dspPoolFindAnotherDspChanByLogicalIdPhysicalId(payload_p->logicalId, MOH_PHY_ID);	/* find musicOnHoldChan */

	if( (NULL != dspChanCopy_p) && (NULL != musicOnHoldChan_p)) {

		/* close dspChanCopy channel if it exists - aircheng add */
					
		/*1. dsp driver: close music on hold chan*/
		ret = dspDriverChanClose(dspChanCopy_p);
		dspChanCopy_p->chanState = CHAN_RESERVED;
					
		if(MM_SUCCESS != ret){
			MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED\n");
		}		
		else{
			/*2. dsp pool delete music on hold chan*/
			MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
			dspPoolDeleteDspChan(dspChanCopy_p);	
		}	

		/* close replicated music on hold channel if it exists - aircheng add */
					
		ivrPlayStop(musicOnHoldChan_p);
		/*1. dsp driver: close music on hold chan*/
		ret = dspDriverChanClose(musicOnHoldChan_p);
		musicOnHoldChan_p->chanState = CHAN_RESERVED;
					
		if(MM_SUCCESS != ret){
			MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED\n");
		}		
		else{
			/*2. dsp pool delete music on hold chan*/
			MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
			dspPoolDeleteDspChan(musicOnHoldChan_p);	
		}
	}
#endif
	
	dspPoolReleaseMutex();	

	ret = MM_SUCCESS;
	/* sending response*/
	/*1. alloc voice msg*/
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_CHAN_CLOSE_RSP,sizeof(voiceDspChanCloseRsp_t));
	/*2. fill voice msg payload */
	rspPayload_p = (voiceDspChanCloseRsp_t *)rspMsg_p->payload;
	rspPayload_p->result = ret;
	
	/*3. send voice msg*/	
	voiceEventIpcSendToVoiceApp(rspMsg_p);

	/*4. free voice msg*/	
	voiceMsgFree(rspMsg_p);

	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);

	return MM_SUCCESS;	
}

/******************************************************************************
 * voiceDspCoreCreateMeetingRoom
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CREATE_MEETING_ROOM_REQ
 *	Create dsp_conf structure in dsp pool, Modify dsp channels(peer-b, peer-c) through dsp driver to support conference
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCreateMeetingRoomReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreCreateMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCreateMeetingRoomReq_t *payload_p = NULL;
	voiceDspCreateMeetingRoomRsp_t *rspPayload_p = NULL;	
	dspChan_t *bDspChan_p = NULL, *cDspChan_p = NULL;
	dspConf_t *dspConf_p = NULL;
	uint16 bLogicalId = 0;
	mm_return_t ret = MM_SUCCESS;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p = (voiceMsg_t *)inputParam_p;
	payload_p = (voiceDspCreateMeetingRoomReq_t *)msg_p->payload;
	
	
	bLogicalId = payload_p->bLogicId;
	VDSP_PRINTF("MM: %s :b logicalId %d\n",__FUNCTION__,bLogicalId);
	
	dspConf_p = dspPoolFindDspConfByLogicalId(bLogicalId);
	if(dspConf_p==NULL){
		return MM_FAILURE;
	}
	
	#if 0
	bDspChan_p=dspConf_p->bDspChan_p;
	cDspChan_p=dspConf_p->cDspChan_p;
	#else
	bDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->bLogicalId);
	cDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->cLogicalId);
	#endif
	if( bDspChan_p == NULL || cDspChan_p == NULL ){
		return MM_FAILURE;
	}

	/*active_dsp_chan & onhold_dsp_chan should be in the same dsp*/
	if(bDspChan_p->dspId != cDspChan_p->dspId){
		ret=MM_FAILURE;
	}

	VDSP_PRINTF("MM: %s : b_physical_id %d, bLogicalId %d\n",__FUNCTION__, bDspChan_p->phPhysicalId, bDspChan_p->phLogicalId);
	VDSP_PRINTF("MM: %s : c_physical_id %d, cLogicalId %d\n",__FUNCTION__, cDspChan_p->phPhysicalId, cDspChan_p->phLogicalId);


	if(ret!=MM_FAILURE){
		VDSP_PRINTF("MM: %s : dsp conf id %d\n",__FUNCTION__,dspConf_p->confId);
		ret=dspDriverCreateMeetingRoom(dspConf_p);
	}
	else{
		ret=MM_FAILURE;
	}
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_CREATE_MEETING_ROOM_RSP,sizeof(voiceDspCreateMeetingRoomRsp_t));
	rspPayload_p = (voiceDspCreateMeetingRoomRsp_t*)rspMsg_p->payload;	
	rspPayload_p->result = ret;	
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	
}

/******************************************************************************
 * voiceDspCoreDestroyMeetingRoom
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_DESTROY_MEETING_ROOM_REQ
 *	Delete dsp_conf structure in dsp pool, Modify dsp channels(peer-b, peer-c) through dsp driver to disable conference
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspDestroyMeetingRoomReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreDestroyMeetingRoom(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspDestroyMeetingRoomReq_t *payload_p = NULL;
	voiceDspDestroyMeetingRoomRsp_t *rspPayload_p = NULL;		
	dspConf_t *dspConf_p = NULL;
		
	mm_return_t ret = MM_SUCCESS;
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}

	msg_p = (voiceMsg_t*)inputParam_p;
	payload_p = (voiceDspDestroyMeetingRoomReq_t *)msg_p->payload;
	VDSP_PRINTF("MM: %s :logicalId %d\n",__FUNCTION__,payload_p->bLogicId);
	dspConf_p = dspPoolFindDspConfByLogicalId(payload_p->bLogicId);
	if(dspConf_p!=NULL){
		VDSP_PRINTF("MM: %s: dsp driver destroy meeting room, confId %d, bLogicalId %d\n",__FUNCTION__, dspConf_p->confId, dspConf_p->bLogicalId);
		ret = dspDriverDestroyMeetingRoom(dspConf_p);
		dspPoolDeleteDspConf(dspConf_p);		
	}
	else{
		ret = MM_FAILURE;
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_DESTROY_MEETING_ROOM_RSP,sizeof(voiceDspDestroyMeetingRoomRsp_t));
	rspPayload_p = (voiceDspDestroyMeetingRoomRsp_t *)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	
}



/******************************************************************************
 * voiceDspCoreCreateInternalCall
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CREATE_INTERNAL_CALL_REQ
 *	hook voipDspCallback in dsp_chan to dspSendtoDsp
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCreateInternalCallReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreCreateInternalCall(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCreateInternalCallReq_t *payload_p = NULL;
	voiceDspCreateInternalCallRsp_t *rspPayload_p = NULL;
	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspCreateInternalCallReq_t *)msg_p->payload;	
	
	dspPoolAcquireMutex();
	dspChan_p = dspPoolFindDspChanByLogicalId(payload_p->logicalId);

	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan logicalId %d not found \n", payload_p->logicalId);
		ret = MM_FAILURE;
	}
	else{		
		VDSP_PRINTF("MM DSP > set voipDspCallBack to dspSendToDsp, logicalId %d\n",dspChan_p->phLogicalId);

		dspChan_p->voipDspCallBack = dspSendToDsp;
	}
	dspPoolReleaseMutex();
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_CREATE_INTERNAL_CALL_RSP,sizeof(voiceDspCreateInternalCallRsp_t));
	rspPayload_p = (voiceDspCreateInternalCallRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;	
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}

/******************************************************************************
 * voiceDspCoreIvrFunc
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CREATE_INTERNAL_CALL_REQ
 *	hook voipDspCallback in dsp_chan to dspSendtoDsp
 * 
 **	descriptions:
**				* * * *				: Enter Ivr main menu
**				0 				 	: Hook dspCallBack function rtpRecord
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
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCreateInternalCallReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreIvrFunc(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspIvrFuncReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL;
	int priFunc = 0, secFunc = 0, action = 0, item = 0, type=0;
	int err=FALSE;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspIvrFuncReq_t *)msg_p->payload;	

	
	priFunc = payload_p->priFunc;
	secFunc = payload_p->secFunc;
	action = payload_p->action;
	item = payload_p->itemNum;
	MM_DSP_DBG("	MM IVR>");
	MM_DSP_DBG1("	priFunc = ", priFunc);
	MM_DSP_DBG1("	secFunc = ", secFunc);
	MM_DSP_DBG1("	action = ", action);
	MM_DSP_DBG1("	item = ", item);
	
	dspChan_p = dspPoolFindDspChanByCallId(payload_p->call_id);

	switch(priFunc){
		case 0: /* hook callback function*/	
			/*hook dspCallBack to rtpRecord*/
			
			if(NULL != dspChan_p){
				dspChan_p->voipDspCallBack = rtpRecord;
				/*if resource available use G729*/
				if(MM_SUCCESS == dspPoolModifyDspCodecRm(dspChan_p->dspId, dspChan_p->codec_num, ZYNOS_CODEC_G729)){ 
					dspChan_p->codec_num = ZYNOS_CODEC_G729;					
					#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/ 
					dspChan_p->codec_type = ZYNOS_CODEC_G729;
					#endif
				}
				dspChan_p->silenceSupp = 0;
				dspChan_p->echoCancel = 1;
				dspChan_p->dtmfMethod = TR104I_DTMF_RFC2833;
				dspChan_p->dtmfCodecNum = ZYNOS_CODEC_RFC2833;
				dspPoolAcquireMutex();
				dspDriverChanModify(dspChan_p, SENDRECV);
				dspPoolReleaseMutex();
				MM_DSP_DBG1("setting voipDspCallBack to rtpRecord, logicalId = ",dspChan_p->phLogicalId);
				ret = MM_SUCCESS;
			}		
			break;
		case 1: /*play/record functions*/
			if(NULL != dspChan_p){
					ivrPlayStop(dspChan_p);
			}
			
			switch(secFunc){				
				case 1: /*record functions*/
					MM_DSP_DBG2("IVR Record item = ",item, ", action = ", action);
					err = ivrRecordFunctions(item, action);
					break;
				case 2: /*play functions*/
					MM_DSP_DBG2("IVR Play item = ",item, ", action = ", action);					
					err = ivrPlayFunctions(dspChan_p, item, action);
					break;
				case 3: /*delete ivr item*/
					MM_DSP_DBG1("IVR Delete item = ",item);
					err = ivrDeleteItem(item);
					break;
				case 4: /*clear all usr ivr*/
					MM_DSP_DBG("IVR Usr Clear To Default");
					err = ivrUsrClearToDefault();
					break;
				case 9: /*clear all sys ivr*/
					MM_DSP_DBG("IVR Sys Clear To Default");
					err = ivrSysClearToDefault();
					break;
			} /* switch secFunc*/			
			
			if(TRUE == err){
				ret = MM_SUCCESS;
			}else{
				ret = MM_FALSE;
			}
			
			break;
		default:
			break;
	}/*switch priFunc */

	rspMsg_p = voiceMsgAlloc(VOICE_DSP_IVR_FUNC_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;	
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}


/******************************************************************************
 * voiceDspCoreSetFaxParameter
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_CREATE_INTERNAL_CALL_REQ
 *	hook voipDspCallback in dsp_chan to dspSendtoDsp
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCreateInternalCallReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreSetFaxParameter(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspSetFaxParameterReq_t *payload_p = NULL;
	voiceDspSetFaxParameterRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspSetFaxParameterReq_t *)msg_p->payload;	
	
	dspChan_p = dspPoolFindDspChanByLogicalId(payload_p->logicalId);

	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan logicalId %d not found \n", payload_p->logicalId);
		ret = MM_FAILURE;
	}
	else{		
		VDSP_PRINTF("MM VDSP > set Fax Parameter, logicalId %d\n",dspChan_p->phLogicalId);
		ret = dspDriverSetFaxParameter(dspChan_p);
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_CREATE_INTERNAL_CALL_RSP,sizeof(voiceDspCreateInternalCallRsp_t));
	rspPayload_p = (voiceDspCreateInternalCallRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;	
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}

#ifdef HOWLER_TONE
/******************************************************************************
 * voiceDspCoreStartHowlerTone
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_START_HOWLER_TONE
 *	generate tone from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCommonReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
voiceDspCoreStartHowlerTone(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCommonReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspCommonReq_t *)msg_p->payload;	

	
	dspChan_p = dspPoolFindDspChanByLogicalId(payload_p->logicalId);

	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan logicalId %d not found \n", payload_p->logicalId);
		ret = MM_FAILURE;
	}
	else{		
		VDSP_PRINTF("MM VDSP > gen Howler Tone, logicalId %d\n",dspChan_p->phLogicalId);
		if(CHAN_OPENED == dspChan_p->chanState){	
			//dspChan_p->howlerToneTimer = timerPeriodicStartS(1000, dspDriverGenHowlerTone, dspChan_p);
			//ret=dspDriverGenHowlerTone(dspChan_p); 
			/*ivrPlayStart(dspChan_p, 0, 1, 0);*/
			ret = ivrPlayStart(dspChan_p, IVRSYS_OFFSET+IVRPRON_HOWLER_TONE, 1, 0); /*repeat play*/
		}
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_START_HOWLER_TONE_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}


/******************************************************************************
 * voiceDspCoreStopHowlerTone
 *
 * Description: 
 *	Media Manager Interface to Phfsm, used to handler VOICE_DSP_STOP_HOWLER_TONE
 *	generate tone from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspCommonReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreStopHowlerTone(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspCommonReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspCommonReq_t *)msg_p->payload;	

	
	dspChan_p = dspPoolFindDspChanByLogicalId(payload_p->logicalId);

	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan logicalId %d not found \n", payload_p->logicalId);
		ret = MM_FAILURE;
	}
	else{		
		VDSP_PRINTF("MM VDSP > gen Howler Tone, logicalId %d\n",dspChan_p->phLogicalId);
		if(CHAN_OPENED == dspChan_p->chanState){			
			/* ret=dspDriverGenHowlerTone(dspChan_p); */
			/*ivrPlayStart(dspChan_p, 0, 1, 0);*/
			ret = ivrPlayStop(dspChan_p); 
		}
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_STOP_HOWLER_TONE_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}
#endif /*ifdef HOWLER_TONE*/

/******************************************************************************
 * voiceDspCoreStartEarlyMedia
 *
 * Description: 
 *	Media Manager Interface to voiceApp, used to handle VOICE_DSP_START_EARLY_MEDIA
 *	generate early media from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspEarlyMediaReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
voiceDspCoreStartEarlyMedia(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspEarlyMediaReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *earlyMediaChan_p = NULL;
	uint16 channel_usage = 0;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspEarlyMediaReq_t *)msg_p->payload;	
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_id = %d\n", msg_p->hdr.api_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_length = %d\n", msg_p->hdr.api_length);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->call_id = %d\n", payload_p->call_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->ivrPlayIndex = %d\n", payload_p->ivrPlayIndex);	/* aircheng add */

	dspPoolAcquireMutex();
	dspChan_p = dspPoolFindDspChanByCallId(payload_p->call_id);
	dspPoolReleaseMutex();
	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan call_id %d not found \n", payload_p->call_id);
		ret = MM_FAILURE;
	}
	else{		
#ifdef EARLY_MEDIA_NO_CHANGE_SDP
		VDSP_PRINTF("MM VDSP > dsp chan phPhysicalId %d\n",dspChan_p->phPhysicalId);
		
		dspPoolAcquireMutex();
		channel_usage = dspPoolChannelUsage(ZYNOS_CODEC_G729);
		if(MM_FAILURE == dspPoolCheckChannelUsage(dspChan_p->dspId, channel_usage)){
			ret = MM_FAILURE;
		} else {
			earlyMediaChan_p = dspPoolCreateDspChan(dspChan_p->phPhysicalId, ID_EARLYMEDIA, dspChan_p->phPortType, ID_EARLYMEDIA, STREAM_ID_NOT_USED, ZYNOS_CODEC_G729);
		}
		dspPoolReleaseMutex();
		
		/*dsp driver: open early media channel */
		if(NULL != earlyMediaChan_p){
			ret = MM_SUCCESS;				
			earlyMediaChan_p->dtmfMethod = TR104I_DTMF_RFC2833;
			earlyMediaChan_p->dtmfCodecNum = ZYNOS_CODEC_RFC2833;
			earlyMediaChan_p->silenceSupp = 0;
			earlyMediaChan_p->echoCancel = 0;
			if(dspDriverChanOpen(earlyMediaChan_p, SENDRECV) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan open.....FAILED\n");
				dspPoolAcquireMutex();
				dspPoolDeleteDspChan(earlyMediaChan_p);
				dspPoolReleaseMutex();
				ret = MM_FAILURE;
			} else {
				dspDriverChanModify(dspChan_p, REPLICATE);
				earlyMediaChan_p->chanState = CHAN_OPENED;
				ret = ivrPlayStart(earlyMediaChan_p, (uint16)payload_p->ivrPlayIndex, 1, 0); /*repeat play - aircheng modify*/
			}
		}
		else{
			MM_DSP_DBG("MM VDSP > DSP Pool create dsp chan....FAILED\n");
			ret = MM_FAILURE;
		}
#endif
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_START_EARLY_MEDIA_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}


/******************************************************************************
 * voiceDspCoreStopEarlyMedia
 *
 * Description: 
 *	Media Manager Interface to voiceApp, used to handle VOICE_DSP_STOP_EARLY_MEDIA
 *	generate early media from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspEarlyMediaReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreStopEarlyMedia(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspEarlyMediaReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *earlyMediaChan_p = NULL;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspEarlyMediaReq_t *)msg_p->payload;	

	dspPoolAcquireMutex();
	dspChan_p = dspPoolFindDspChanByCallId(payload_p->call_id);
	dspPoolReleaseMutex();
	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan call_id %d not found \n", payload_p->call_id);
		ret = MM_FAILURE;
	}
	else{		
#ifdef EARLY_MEDIA_NO_CHANGE_SDP
		VDSP_PRINTF("MM VDSP > dsp chan phPhysicalId %d\n",dspChan_p->phPhysicalId);
		dspPoolAcquireMutex();
		earlyMediaChan_p = dspPoolFindDspChan(ID_EARLYMEDIA, ID_EARLYMEDIA, dspChan_p->phPhysicalId);
		dspPoolReleaseMutex();
		
		if(NULL == earlyMediaChan_p){
			VDSP_PRINTF("MM VDSP > dsp chan phPhysicalId %d not found \n",dspChan_p->phPhysicalId);
			ret = MM_FAILURE;
		}
		else{		
			ivrPlayStop(earlyMediaChan_p);
			dspDriverChanModify(dspChan_p, SENDRECV);
			/*1. dsp driver: close early media chan*/
			ret = dspDriverChanClose(earlyMediaChan_p);
			earlyMediaChan_p->chanState = CHAN_RESERVED;
			
			if(MM_SUCCESS != ret){
				MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED\n");
			}		
			else{
				/*2. dsp pool delete early media chan*/
				MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
				dspPoolAcquireMutex();
				dspPoolDeleteDspChan(earlyMediaChan_p);	
				dspPoolReleaseMutex();
			}			
		}
#endif
	}
	
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_STOP_EARLY_MEDIA_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}

/******************************************************************************
 * voiceDspCoreStartMusicOnHold - aircheng add
 *
 * Description: 
 *	Media Manager Interface to voiceApp, used to handle VOICE_DSP_START_MUSIC_ONHOLD
 *	generate music on hold tone from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspMusicOnHoldReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
voiceDspCoreStartMusicOnHold(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{
	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspMusicOnHoldReq_t *payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *musicOnHoldChan_p = NULL, *dspChanCopy_p = NULL;
	uint16 channel_usage = 0;
	rtpTable_t *table = MM_NULL;
	int chanId = 0;
	uint32 lost, jitter;
	uint16 phyId;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspMusicOnHoldReq_t *)msg_p->payload;
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_id = %d\n", msg_p->hdr.api_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_length = %d\n", msg_p->hdr.api_length);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->call_id = %d\n", payload_p->call_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->physicalId = %d\n", payload_p->physicalId);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->ivrPlayIndex = %d\n", payload_p->ivrPlayIndex);	/* aircheng add */

	dspPoolAcquireMutex();
	dspChan_p = dspPoolFindDspChanByCallIdPhysicalId(payload_p->call_id, payload_p->physicalId);
	dspPoolReleaseMutex();
	
	if(NULL == dspChan_p){
		VDSP_PRINTF("MM VDSP > dsp chan call_id %d, physicalId %d not found \n", payload_p->call_id, payload_p->physicalId);
		ret = MM_FAILURE;
	}
	else{
		phyId = dspChan_p->phPhysicalId;
		VDSP_PRINTF("MM VDSP > dsp chan phPhysicalId %d\n", phyId);
		
		dspPoolAcquireMutex();

		musicOnHoldChan_p = dspPoolFindDspChanByCallIdPhysicalId(ID_MUSICONHOLD + phyId, MOH_PHY_ID);	/* find musicOnHoldChan */
		dspChanCopy_p = dspPoolFindAnotherDspChanByCallIdPhysicalId(ID_MUSICONHOLD + phyId, MOH_PHY_ID);	/* find dspChanCopy */

		if( (NULL == musicOnHoldChan_p) && (NULL == dspChanCopy_p) ){

			MM_DSP_DBG("MM VDSP > (NULL == musicOnHoldChan_p) && (NULL == dspChanCopy_p)\n");

			/* dsp pool: crate a new dsp chan for MOH (musicOnHoldChan_p) */

			musicOnHoldChan_p = dspPoolCreateDspChan(MOH_PHY_ID, ID_MUSICONHOLD + phyId, dspChan_p->phPortType, ID_MUSICONHOLD + phyId, STREAM_ID_NOT_USED, ZYNOS_CODEC_G729);
			if(MM_NULL == musicOnHoldChan_p){
				dspPoolReleaseMutex();
				ret = MM_FAILURE;
				goto pass;
			}
			musicOnHoldChan_p->dtmfMethod = TR104I_DTMF_RFC2833;
			musicOnHoldChan_p->dtmfCodecNum = ZYNOS_CODEC_RFC2833;
			musicOnHoldChan_p->silenceSupp = 0;
			musicOnHoldChan_p->echoCancel = 0;

			/* dsp pool: create a new dsp chan copy with diff phyId (dspChanCopy_p) */
			
			dspChanCopy_p = dspPoolCreateDspChan(MOH_PHY_ID, dspChan_p->phLogicalId, dspChan_p->phPortType, dspChan_p->call_id, dspChan_p->stream_id, dspChan_p->codec_num);
			if(MM_NULL == dspChanCopy_p){
				dspPoolDeleteDspChan(musicOnHoldChan_p);
				dspPoolReleaseMutex();
				ret = MM_FAILURE;
				goto pass;
			}
			chanId = dspChanCopy_p->chanId;
			zyMemcpy(dspChanCopy_p, dspChan_p, sizeof(dspChan_t));
			dspChanCopy_p->chanId = chanId;
			dspChanCopy_p->phPhysicalId = MOH_PHY_ID;

			/*dsp driver: open dsp chan (musicOnHoldChan_p) */
			
			if(dspDriverChanOpen(musicOnHoldChan_p, TRANSCODE) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan open .....FAILED musicOnHoldChan_p\n");
				ret = MM_FAILURE;
				
				dspPoolDeleteDspChan(musicOnHoldChan_p);
				musicOnHoldChan_p = NULL;

				dspPoolDeleteDspChan(dspChanCopy_p);
				dspChanCopy_p = NULL;
				dspPoolReleaseMutex();
				goto pass;
			} 
			else{
				musicOnHoldChan_p->chanState = CHAN_OPENED;
			}
						

			/*dsp driver: open dsp chan (dspChanCopy_p) */
			
			if(dspDriverChanOpen(dspChanCopy_p, TRANSCODE) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan open.....FAILED dspChanCopy_p\n");
				ret = MM_FAILURE;
				
				/*close and delete channels*/
				dspDriverChanClose(musicOnHoldChan_p);
				dspPoolDeleteDspChan(musicOnHoldChan_p);
				musicOnHoldChan_p = NULL;

				dspPoolDeleteDspChan(dspChanCopy_p);
				dspChanCopy_p = NULL;
				dspPoolReleaseMutex();
				goto pass;
			}
			else{
				musicOnHoldChan_p->chanState = CHAN_OPENED;
			}
			
			
			
		}
		else if( (NULL != musicOnHoldChan_p) && (NULL != dspChanCopy_p) ){

			MM_DSP_DBG("MM VDSP > (NULL != musicOnHoldChan_p) && (NULL != dspChanCopy_p)\n");

			/* ivrPlayStop & dspDriverKSocketDeleteSession */
			
			ivrPlayStop(musicOnHoldChan_p);
			
			
			
			/* dsp pool: modify dsp chan (dspChanCopy_p)*/
			
			chanId = dspChanCopy_p->chanId;
			zyMemcpy(dspChanCopy_p, dspChan_p, sizeof(dspChan_t));
			dspChanCopy_p->chanId = chanId;
			dspChanCopy_p->phPhysicalId = MOH_PHY_ID;

			/*dsp driver: modify dsp chan (musicOnHoldChan_p) */

			if(dspDriverChanModify(musicOnHoldChan_p, TRANSCODE) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan modify.....FAILED\n");
				ret = MM_FAILURE;

				dspPoolDeleteDspChan(musicOnHoldChan_p);
				musicOnHoldChan_p = NULL;
				
				dspPoolDeleteDspChan(dspChanCopy_p);
				dspChanCopy_p = NULL;
				
				dspPoolReleaseMutex();
				goto pass;
			} 
			else{
				musicOnHoldChan_p->chanState = CHAN_OPENED;
			}

			/*dsp driver: modify dsp chan (dspChanCopy_p) */
			
			if(dspDriverChanModify(dspChanCopy_p, TRANSCODE) != MM_SUCCESS){
				MM_DSP_DBG("MM VDSP > DSP driver chan modify.....FAILED\n");
				ret = MM_FAILURE;

				/*close opened channel*/
				dspDriverChanClose(musicOnHoldChan_p);
				dspPoolDeleteDspChan(musicOnHoldChan_p);
				musicOnHoldChan_p = NULL;

				dspPoolDeleteDspChan(dspChanCopy_p);
				dspChanCopy_p = NULL;
				dspPoolReleaseMutex();
				goto pass;
			} 
			else{
				musicOnHoldChan_p->chanState = CHAN_OPENED;
			}
			

			if( (NULL!=dspChanCopy_p)&&(table = rtpTableGetById(dspChanCopy_p->call_id, dspChanCopy_p->stream_id, dspChanCopy_p->stream_type)) != MM_NULL ){
				MM_DSP_DBG("rtp_table found!");
				//dspDriverKSocketDeleteSession(dspChanCopy_p, table, &lost, &jitter);				
				#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT, SYSLOG - RTP Statistics*/
				if(mmKSocketEnable){
					dspDriverKSocketDeleteSession(dspChanCopy_p->call_id, table, &lost, &jitter);
				}
				#endif
				
			}
			
		}
		
		dspDriverChanClose(dspChan_p);
		dspChan_p->chanState = CHAN_RESERVED;
		
		dspPoolReleaseMutex();
		
		if( (NULL != dspChanCopy_p) && (table = rtpTableGetById(dspChanCopy_p->call_id, dspChanCopy_p->stream_id, dspChanCopy_p->stream_type)) == MM_NULL ){
			MM_DSP_DBG("ERROR!! rtp_table not found!");			
		}
		
		

		if((NULL!=table) && (NULL != dspChanCopy_p)){
				
		//dspDriverKSocketMakeSession(dspChanCopy_p, 
			#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT, SYSLOG - RTP Statistics*/
			if(mmKSocketEnable){
				dspDriverKSocketMakeSession(dspChanCopy_p->call_id, 
										table->stream_id,
										table->rtp_socket.local_addr,
										table->rtp_socket.local_port,
										table->rtp_socket.remote_addr,
										table->rtp_socket.remote_port,
										table->media_mode,
										0/*qos_mark*/, table->ifname
										);
				}
			#endif
		}

		if(NULL != musicOnHoldChan_p){			
			ivrPlayStart(musicOnHoldChan_p, (uint16)payload_p->ivrPlayIndex, 1, 0); /*repeat play - aircheng add*/
		}

	}
	
pass:
	rspMsg_p = voiceMsgAlloc(VOICE_DSP_START_MUSIC_ONHOLD_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}

/******************************************************************************
 * voiceDspCoreStopMusicOnHold - aircheng add
 *
 * Description: 
 *	Media Manager Interface to voiceApp, used to handle VOICE_DSP_STOP_MUSIC_ONHOLD
 *	generate music on hold tone from PCMU file
 * 
 * Parameters:
 * 	Input
 *    void	 *inputParam_p -received voice_msg, the type of payload is voiceDspMusicOnHoldReq_t
 *	mm_error_t *err_p -not used here
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
mm_return_t voiceDspCoreStopMusicOnHold(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{

	voiceMsg_t *msg_p = NULL, *rspMsg_p = NULL;
	voiceDspMusicOnHoldReq_t*payload_p = NULL;
	voiceDspCommonRsp_t *rspPayload_p = NULL;	
	mm_return_t ret = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *musicOnHoldChan_p = NULL, *dspChanCopy_p = NULL;
	uint16 phyId;
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	if(MM_NULL == inputParam_p )
	{
		return MM_FAILURE;
	}
	
	msg_p=(voiceMsg_t *)inputParam_p;
	payload_p=(voiceDspMusicOnHoldReq_t *)msg_p->payload;	
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_id = %d\n", msg_p->hdr.api_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > msg_p->hdr.api_length = %d\n", msg_p->hdr.api_length);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->call_id = %d\n", payload_p->call_id);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->physicalId = %d\n", payload_p->physicalId);	/* aircheng add */
	VDSP_PRINTF("MM VDSP > payload_p->ivrPlayIndex = %d\n", payload_p->ivrPlayIndex);	/* aircheng add */

	dspPoolAcquireMutex();
	dspChanCopy_p = dspPoolFindDspChanByCallIdPhysicalId(payload_p->call_id, MOH_PHY_ID);	/* find dspChanCopy */
	dspPoolReleaseMutex();
	
	if(NULL == dspChanCopy_p){
		VDSP_PRINTF("MM VDSP > dsp chan call_id %d, physicalId %d not found \n", payload_p->call_id, MOH_PHY_ID);
		ret = MM_FAILURE;	
	}
	else{
		phyId = payload_p->physicalId;
		
		dspPoolAcquireMutex();
		musicOnHoldChan_p = dspPoolFindDspChanByCallIdPhysicalId(ID_MUSICONHOLD + phyId, MOH_PHY_ID);	/* find musicOnHoldChan */
		dspPoolReleaseMutex();
		
		if(NULL == musicOnHoldChan_p){
			VDSP_PRINTF("MM VDSP > dsp chan call_id %d, physicalId %d not found \n", ID_MUSICONHOLD + phyId, MOH_PHY_ID);
			ret = MM_FAILURE;
		}
		else{		
			ivrPlayStop(musicOnHoldChan_p);
			dspDriverChanClose(dspChanCopy_p);
			/*1. dsp driver: close early media chan*/
			ret = dspDriverChanClose(musicOnHoldChan_p);
			musicOnHoldChan_p->chanState = CHAN_RESERVED;
			
			if(MM_SUCCESS != ret){
				MM_DSP_DBG("MM VDSP > DSP driver close dsp chan FAILED\n");
			}		
			else{
				/*2. dsp pool delete early media chan*/
				MM_DSP_DBG("MM VDSP > DSP driver close dsp chan SUCCESS");
				dspPoolAcquireMutex();
				dspPoolDeleteDspChan(musicOnHoldChan_p);	
				dspPoolDeleteDspChan(dspChanCopy_p);	
				dspPoolReleaseMutex();
			}			
		}
	}

	rspMsg_p = voiceMsgAlloc(VOICE_DSP_STOP_MUSIC_ONHOLD_RSP,sizeof(voiceDspCommonRsp_t));
	rspPayload_p = (voiceDspCommonRsp_t*)rspMsg_p->payload;
	rspPayload_p->result = ret;
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	voiceMsgFree(rspMsg_p);
	dspPoolDumpDspPool();
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret);
	return MM_SUCCESS;	

}

/******************************************************************************
 * dspPoolAllocateDspChan
 *
 * Description: 
 *	Dsp pool function, used to allocate dsp chan, and fill default values
 *	
 * Parameters:
 *
 * Return Value: dspChan_t *
 *    dsp channel allocated 
 *    
 *****************************************************************************/
dspChan_t *dspPoolAllocateDspChan(void){
	dspChan_t *dspChan_p = NULL;
	dspChan_p = (dspChan_t *)zyMalloc(sizeof(dspChan_t));
	zyMemset(dspChan_p,0,sizeof(dspChan_t));
	dspChan_p->call_id = ID_NOT_USED;
	dspChan_p->phLogicalId= ID_NOT_USED;
	dspChan_p->phPhysicalId = ID_NOT_USED;
	dspChan_p->phPortType = 0;
	dspChan_p->dspId = ID_NOT_USED;
	dspChan_p->chanId = ID_NOT_USED;
	dspChan_p->chanState = CHAN_IDLE;
	dspChan_p->timeslot = 0;
	dspChan_p->timeslotUseType = 0;
	dspChan_p->stream_id = ID_NOT_USED;
	dspChan_p->stream_type = 0;
	dspChan_p->codec_num = 0;
	#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/ 
	dspChan_p->codec_type = 0;
	#endif
	dspChan_p->silenceSupp = 1;
	dspChan_p->echoCancel = 1;
	dspChan_p->ptime = 20;
	dspChan_p->dtmfMethod = TR104I_DTMF_INBAND;
	dspChan_p->dtmfMethodG711 = TR104I_DTMF_NULL;
	dspChan_p->voipDspCallBack = NULL;

	return dspChan_p;	
	
}

/******************************************************************************
 * dspPoolCreateDspChan
 *
 * Description: 
 *	Dsp pool function, used to create dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *    uint16 physicalId - phone physical id
 *	uint16 logicalId -phone logical id
 *	uint8 portType -phone physical type
 *	icf_call_id_t call_id -call_id used for outgoing sip call
 *	icf_stream_id_t stream_id -stream_id used for media session
 *	uint8 codec_num -codec number used in sdp
 *
 * Return Value: dspChan_t *
 *    dsp channel created in dsp pool
 *    
 *****************************************************************************/
dspChan_t * dspPoolCreateDspChan(uint16 physicalId,uint16 logicalId, uint8 portType, icf_call_id_t call_id, icf_stream_id_t stream_id, uint8 codec_num){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, pcm_timeslot = -1, chanId = -1;
	dspChan_t *newDspChan_p = NULL;
	uint16 dspId = 0;	
	
	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);
	
	dspId = dspPoolCheckCodecRm(codec_num);
	if(dspId > gDspPool_p->dspCount ){
		return MM_NULL;
	}
	
	voiceDsp_p = &(gDspPool_p->voiceDsp[dspId]);
	

	/*search for un-used pcmtimeslot*/
	for( i = 0; i < MAX_TIMESLOT_NUM; i++ ){
		if(gDspPool_p->pcmTimeslotUsed[i]==0){			
			pcm_timeslot=i;
			break;
		}
	}

	if(i == MAX_TIMESLOT_NUM){
		return MM_NULL;
	}

	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if(voiceDsp_p->dspChanList[i]==NULL){
			chanId=i;
			break;
		}
	}

	if(i==MAX_DSP_CHAN_PER_DSP){
		return MM_NULL;
	}
	/* create dsp_chan, allocate pcm_timeslot*/

	newDspChan_p=dspPoolAllocateDspChan();	
	newDspChan_p->dspId = dspId;
	newDspChan_p->phPhysicalId = physicalId;
	newDspChan_p->phLogicalId = logicalId;
	newDspChan_p->call_id = call_id;
	newDspChan_p->stream_id = stream_id;
	#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/ 
	newDspChan_p->codec_type = codec_num;	
	#endif
	newDspChan_p->codec_num = codec_num;
	newDspChan_p->phPortType = portType;
	newDspChan_p->timeslot = pcm_timeslot;
	newDspChan_p->timeslotUseType = 0;	
	newDspChan_p->voipDspCallBack = NULL;
	newDspChan_p->stream_type = ICF_STREAM_TYPE_AUDIO;

	voiceDsp_p->dspChanList[chanId] = newDspChan_p;
	newDspChan_p->chanId = chanId;
	
	VDSP_PRINTF("MM VDSP > exiting %s, chanId = %d\n",__FUNCTION__,newDspChan_p->chanId);
	return newDspChan_p;
}


/******************************************************************************
 * dspPoolFindDspChanById
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanById(
	icf_call_id_t	call_id, 
	icf_stream_id_t	stream_id, 
	icf_media_type_t	stream_type
){
	voiceDsp_t *voiceDsp_p;
	int i,j;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if(NULL != voiceDsp_p->dspChanList[j]){
				if((voiceDsp_p->dspChanList[j]->call_id == call_id)
					&& (voiceDsp_p->dspChanList[j]->stream_id== stream_id)
					&& (voiceDsp_p->dspChanList[j]->stream_type== stream_type))
				{
					return voiceDsp_p->dspChanList[j];
				}
			}
		}
	}

	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChan
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool, OPENED first, COMMITTED second, others last
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id
 *	uint16 logicalId
 *	uint16 physicalId
 *
 * Return Value: dspChan_t *
 *    find dsp channel COMMITTED or call_id, logicalId, physicalId matched
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChan(
	icf_call_id_t	call_id, 
	uint16 logicalId,
	uint16 physicalId	
){
	voiceDsp_t *voiceDsp_p = NULL;
	int i,j;
	dspChan_t *dspChan_p = NULL, *committedDspChan_p = NULL, *openedDspChan_p = NULL;
	
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if(NULL != voiceDsp_p->dspChanList[j]){
				if((voiceDsp_p->dspChanList[j]->call_id == call_id) && 
					(CHAN_COMMITTED == voiceDsp_p->dspChanList[j]->chanState)){					
					VDSP_PRINTF("%s: found committed dsp chan \n", __FUNCTION__);
					committedDspChan_p = voiceDsp_p->dspChanList[j];
				}
				
				if((voiceDsp_p->dspChanList[j]->call_id == call_id)
					&& (voiceDsp_p->dspChanList[j]->phLogicalId == logicalId)
					&& (voiceDsp_p->dspChanList[j]->phPhysicalId == physicalId)){

					if(CHAN_OPENED == voiceDsp_p->dspChanList[j]->chanState){
						openedDspChan_p = voiceDsp_p->dspChanList[j];
					}
					else{
						dspChan_p = voiceDsp_p->dspChanList[j];
					}
					VDSP_PRINTF("%s: found dsp chan \n", __FUNCTION__);
					
				}				
			}
		}
	}
	
	if(NULL != openedDspChan_p){
		return openedDspChan_p;
	}
	else if(NULL != committedDspChan_p ){
		return committedDspChan_p;
	}
	else{
		return dspChan_p;
	}
}

/******************************************************************************
 * dspPoolFindDspChanByLogicalId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 logicalId -find dsp chan by phone logical id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByLogicalId(uint16 logicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0,j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j]!=NULL) && (voiceDsp_p->dspChanList[j]->phLogicalId == logicalId)){
				
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByCallIdPhysicalId - aircheng add for MOH
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByCallIdPhysicalId(uint16 call_id, uint16 physicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->call_id == call_id) &&
				(voiceDsp_p->dspChanList[j]->phPhysicalId == physicalId)){
				VDSP_PRINTF("MM: %s : dsp chan[%d] found, call_id %d, physicalId %d\n", __FUNCTION__, j, call_id, physicalId);
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, call_id %d, physicalId %d\n",__FUNCTION__, call_id, physicalId);
	return NULL;
}

/******************************************************************************
 * dspPoolFindAnotherDspChanByCallIdPhysicalId - aircheng add for MOH
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindAnotherDspChanByCallIdPhysicalId(uint16 call_id, uint16 physicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->call_id != call_id) &&
				(voiceDsp_p->dspChanList[j]->phPhysicalId == physicalId)){
				VDSP_PRINTF("MM: %s : dsp chan[%d] found, call_id %d, physicalId %d\n", __FUNCTION__, j, voiceDsp_p->dspChanList[j]->call_id, physicalId);
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, call_id %d, physicalId %d\n",__FUNCTION__, call_id, physicalId);
	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByLogicalIdPhysicalId - aircheng add for MOH
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByLogicalIdPhysicalId(uint16 logicalId, uint16 physicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->phLogicalId == logicalId) &&
				(voiceDsp_p->dspChanList[j]->phPhysicalId == physicalId)){
				VDSP_PRINTF("MM: %s : dsp chan[%d] found, logicalId %d, physicalId %d\n", __FUNCTION__, j, logicalId, physicalId);
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, logicalId %d, physicalId %d\n",__FUNCTION__, logicalId, physicalId);
	return NULL;
}

/******************************************************************************
 * dspPoolFindAnotherDspChanByLogicalIdPhysicalId - aircheng add for MOH
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindAnotherDspChanByLogicalIdPhysicalId(uint16 logicalId, uint16 physicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->phLogicalId != logicalId) &&
				(voiceDsp_p->dspChanList[j]->phPhysicalId == physicalId)){
				VDSP_PRINTF("MM: %s : dsp chan[%d] found, logicalId %d, physicalId %d\n", __FUNCTION__, j, voiceDsp_p->dspChanList[j]->phLogicalId, physicalId);
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, logicalId %d, physicalId %d\n",__FUNCTION__, logicalId, physicalId);
	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByCallId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByCallId(uint16 call_id){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && (voiceDsp_p->dspChanList[j]->call_id == call_id)){
				//VDSP_PRINTF("MM: %s : dsp chan[%d] found, call_id %d\n",__FUNCTION__,j,call_id);
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, call_id %d \n",__FUNCTION__, call_id);
	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByCallIdLogicalId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByCallIdLogicalId(uint16 call_id, uint16 logicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->call_id == call_id) &&
				(voiceDsp_p->dspChanList[j]->phLogicalId == logicalId)){
				//VDSP_PRINTF("MM: %s : dsp chan[%d] found, call_id %d, logicalId %d\n",__FUNCTION__, j, call_id, logicalId);	/* aircheng modify */
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	VDSP_PRINTF("MM: %s : dsp chan not found, call_id %d, logicalId %d \n",__FUNCTION__, call_id, logicalId);
	return NULL;
}


/******************************************************************************
 * dspPoolFindAnotherDspChanByCallId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindAnotherDspChanByCallId(uint16 call_id, uint16 logicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if( (voiceDsp_p->dspChanList[j] != NULL) && 
				(voiceDsp_p->dspChanList[j]->call_id == call_id) &&
				(voiceDsp_p->dspChanList[j]->phLogicalId != logicalId)){
				return voiceDsp_p->dspChanList[j];
			}
		}
	}

	return NULL;
}


/******************************************************************************
 * dspPoolFindDspChanByChanId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 chanId -find dsp chan by chan id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByChanId(uint16 dspId, uint16 chanId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0;
	if(dspId >= gDspPool_p->dspCount){
		return NULL;
	}
	
	voiceDsp_p = &(gDspPool_p->voiceDsp[dspId]);
	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if( (NULL != voiceDsp_p->dspChanList[i]) && (voiceDsp_p->dspChanList[i]->chanId == chanId)){				
			return voiceDsp_p->dspChanList[i];
		}
	}

	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByStreamId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 stream_id -find dsp chan by stream id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByStreamId(uint16 call_id, uint16 stream_id){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if(NULL != voiceDsp_p->dspChanList[j]){
				if( (voiceDsp_p->dspChanList[j]->call_id == call_id) && (voiceDsp_p->dspChanList[j]->stream_id == stream_id)){
					VDSP_PRINTF("MM: %s : dsp chan[%d] found, stream_id %d\n",__FUNCTION__,j,stream_id);
					return voiceDsp_p->dspChanList[j];
				}
			}
		}
	}

	return NULL;
}


/******************************************************************************
 * dspPoolDeleteDspChan
 *
 * Description: 
 *	Dsp pool function, used to delete dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	dspChan_t *dspChan_p - dsp chan to delete
 *
 * Return Value: 
 *    
 *    
 *****************************************************************************/
void dspPoolDeleteDspChan(dspChan_t *dspChan_p){
	voiceDsp_t *voiceDsp_p = NULL;	
	int i = 0;
	uint16 chanId = 0;
	if(NULL == dspChan_p){
		VDSP_PRINTF("parameter NULL\n");
		return;
	}

	chanId = dspChan_p->chanId;	
	voiceDsp_p = &(gDspPool_p->voiceDsp[dspChan_p->dspId]);
	VDSP_PRINTF("MM: %s : dspId %d, chanId %d\n",__FUNCTION__,dspChan_p->dspId, chanId);

	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if(	voiceDsp_p->dspChanList[i] == dspChan_p){
			VDSP_PRINTF("MM : %s : found dsp chan to delete\n",__FUNCTION__);
			zyFree(dspChan_p);
			voiceDsp_p->dspChanList[chanId]=NULL;
			return;
		}

	}
}


/******************************************************************************
 * dspPoolCreateDspConf
 *
 * Description: 
 *	Dsp pool function, used to create dsp conf in dsp pool
 *	
 * Parameters:
 * 	Input
 *	voiceDsp_t * voiceDsp_p - specify which dsp to create conference
 *	dspChan_t *bDspChan_p - peer-b dsp channel
 *	dspChan_t *cDspChan_p - peer-c dsp channel
 *
 * Return Value: 
 *    
 *    
 *****************************************************************************/
dspConf_t * dspPoolCreateDspConf(voiceDsp_t * voiceDsp_p, dspChan_t *bDspChan_p, dspChan_t *cDspChan_p){
	dspConf_t *dspConf_p = NULL,*p_tmp_dsp_conf= NULL;
	int i = 0;
	uint16 confId = 0, dspId = 0;
	dspConf_p=(dspConf_t *)zyMalloc(sizeof(dspConf_t));
	dspConf_p->bLogicalId=bDspChan_p->phLogicalId;
	dspConf_p->cLogicalId=cDspChan_p->phLogicalId;
	#if 0
	dspConf_p->bDspChan_p=bDspChan_p;
	dspConf_p->cDspChan_p=cDspChan_p;
	#endif
	dspId=voiceDsp_p->dspId;

	for( i = 0; i < MAX_CONF_PER_DSP; i++ ){
		if(NULL == voiceDsp_p->dspConfList[i]){
			confId=i;
			break;
		}
	}

	dspConf_p->confId = confId;
	dspConf_p->dspId = dspId;

	voiceDsp_p->dspConfList[confId] = dspConf_p;

	VDSP_PRINTF("MM: %s :confId %d\n",__FUNCTION__,dspConf_p->confId);
	VDSP_PRINTF("MM: %s :bLogicalId %d\n",__FUNCTION__,dspConf_p->bLogicalId);
	VDSP_PRINTF("MM: %s :cLogicalId %d\n",__FUNCTION__,dspConf_p->cLogicalId);
	return dspConf_p;
}

/******************************************************************************
 * dspPoolDeleteDspConf
 *
 * Description: 
 *	Dsp pool function, used to delete dsp conf in dsp pool
 *	
 * Parameters:
 * 	Input
 *	dspConf_t *dspConf_p - dsp conf to delete
 *
 * Return Value: 
 *    
 *    
 *****************************************************************************/
void dspPoolDeleteDspConf(dspConf_t *dspConf_p){
	uint16 confId = 0;
	voiceDsp_t *voiceDsp_p = NULL;	

	if(NULL == dspConf_p){
		/*no conf found*/
		return;
	}
	voiceDsp_p = &(gDspPool_p->voiceDsp[dspConf_p->dspId]);
	confId = dspConf_p->confId;

	zyFree(dspConf_p);
	voiceDsp_p->dspConfList[confId] = NULL;	
}

/******************************************************************************
 * dspPoolInit
 *
 * Description: 
 *	Dsp pool function, used to init dsp pool
 *	
 * Parameters:
 *
 * Return Value: 
 *    
 *    
 *****************************************************************************/
void dspPoolInit(void){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;
	
	gDspPool_p->dspCount = 1;

	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&gDspPool_p->voiceDsp[i];
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			voiceDsp_p->dspChanList[i] = NULL;
		}

		
		for(j=0;j<MAX_CONF_PER_DSP;j++){
			voiceDsp_p->dspConfList[i] = NULL;
		}
	}

	dspPoolInitMutex();
}


/******************************************************************************
 * dspPoolFindVoiceDspByDspId
 *
 * Description: 
 *	Dsp pool function, used to find dsp structure (voiceDsp_t) in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 dspId - find matched dsp id in dsp pool
 *
 * Return Value: voiceDsp_t *
 *    dsp structure in dsp pool
 *    
 *****************************************************************************/
voiceDsp_t *dspPoolFindVoiceDspByDspId(uint16 dspId){
	int i = 0;
	voiceDsp_t *voiceDsp_p = NULL;
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &(gDspPool_p->voiceDsp[i]);
		if(voiceDsp_p->dspId == dspId){
			return voiceDsp_p;
		}
	}

	return NULL;
}


/******************************************************************************
 * dspPoolFindDspConfByLogicalId
 *
 * Description: 
 *	Dsp pool function, used to find dsp conf in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 logicalId - find peer-b or peer-c logical id in dsp conference list in dsp pool
 *
 * Return Value: dspConf_t *
 *    found dsp conference
 *    
 *****************************************************************************/
dspConf_t *dspPoolFindDspConfByLogicalId(uint16 logicalId){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;	
	VDSP_PRINTF("MM: %s\n",__FUNCTION__);
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_CONF_PER_DSP; j++){
			if(NULL != (voiceDsp_p->dspConfList[j])){
				VDSP_PRINTF("MM: %s :bLogicId %d, cLogicId %d\n",__FUNCTION__,voiceDsp_p->dspConfList[j]->bLogicalId, voiceDsp_p->dspConfList[j]->cLogicalId);
				if((voiceDsp_p->dspConfList[j]->bLogicalId == logicalId) || (voiceDsp_p->dspConfList[j]->cLogicalId == logicalId) ){
					return voiceDsp_p->dspConfList[j];
				}
			}
		}
		
	}

	return NULL;
}

/******************************************************************************
 * dspPoolCheckCodecRm
 *
 * Description: 
 *	Dsp pool function, used to check dsp resource in all dsp
 *	
 * Parameters:
 * 	Input
 *	uint8 codec_num - codec number in sdp
 *
 * Return Value: uint16
 *    dspId if success
 *    -1 if failure
 *****************************************************************************/
uint16 dspPoolCheckCodecRm(uint8 codec_num){
	int i = 0;
	for( i = 0; i < MAX_DSP; i++ ){
		VDSP_PRINTF("MM: %s :i=%d\n",__FUNCTION__,i);
		if(MM_SUCCESS == dspPoolCheckDspCodecRm(i,codec_num)){
			return i;
		}
	}

	return MM_INVALID_DSP_ID;
}

/******************************************************************************
 * dspPoolCheckDspCodecRm
 *
 * Description: 
 *	Dsp pool function, used to check dsp resource in specified dsp
 *	
 * Parameters:
 * 	Input
 *	uint16 dspId - specify dsp id
 *	uint8 codec_num - codec number in sdp
 *
 * Return Value: uint16
 *    MM_SUCCESS if success
 *    MM_FAILURE if failure
 *****************************************************************************/
mm_return_t dspPoolCheckDspCodecRm(uint16 dspId, uint8 codec_num){
	voiceDsp_t *voiceDsp_p = NULL;	
	uint16 channel_usage=0;	
	
	voiceDsp_p = &gDspPool_p[dspId];

	channel_usage = dspPoolChannelUsage(codec_num);

	if(MM_FAILURE == dspPoolCheckChannelUsage(dspId, channel_usage)){
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;
}

/******************************************************************************
 * dspPoolModifyDspCodecRm
 *
 * Description: 
 *	Dsp pool function, used to modify dsp codec and check dsp resource in specified dsp
 *	
 * Parameters:
 * 	Input
 *	uint16 dspId - specify dsp id
 *	uint8 codec_num - codec number in sdp
 *
 * Return Value: uint16
 *    MM_SUCCESS if success
 *    MM_FAILURE if failure
 *****************************************************************************/
mm_return_t dspPoolModifyDspCodecRm(uint16 dspId, uint8 codec_num, uint8 new_codec_num){
	voiceDsp_t *voiceDsp_p = NULL;	
	uint16 channel_usage = 0;	
	
	voiceDsp_p = &gDspPool_p[dspId];
	if(dspPoolChannelUsage(new_codec_num) > dspPoolChannelUsage(codec_num)){
		channel_usage = dspPoolChannelUsage(new_codec_num) - dspPoolChannelUsage(codec_num);
	}

	if(MM_FAILURE == dspPoolCheckChannelUsage(dspId, channel_usage)){
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;
}


/******************************************************************************
 * dspPoolDumpDspChan
 *
 * Description: 
 *	Dsp pool function, used to dump dsp_chan information
 *	
 * Parameters:
 * 	Input
 *	dspChan_t *dspChan_p - dsp chan to dump
 *
 * Return Value: 
 *
 *****************************************************************************/
void dspPoolDumpDspChan(dspChan_t *dspChan_p){
	VDSP_PRINTF("\r\n");
	VDSP_PRINTF("	DSP POOL> chanId = %d\n",dspChan_p->chanId);
	VDSP_PRINTF("	DSP POOL> dspId = %d\n",dspChan_p->dspId);
	VDSP_PRINTF("	DSP POOL> call_id = %d\n",dspChan_p->call_id);
	VDSP_PRINTF("	DSP POOL> codec_num = %d\n",dspChan_p->codec_num);
	VDSP_PRINTF("	DSP POOL> logicalId = %d\n",dspChan_p->phLogicalId);
	VDSP_PRINTF("	DSP POOL> physicalId = %d\n",dspChan_p->phPhysicalId);
	VDSP_PRINTF("	DSP POOL> portType = %d\n",dspChan_p->phPortType);
	VDSP_PRINTF("	DSP POOL> stream_id = %d\n",dspChan_p->stream_id);
	VDSP_PRINTF("	DSP POOL> stream_type = %d\n",dspChan_p->stream_type);
	VDSP_PRINTF("	DSP POOL> timeslot = %d\n",dspChan_p->timeslot);
	VDSP_PRINTF("	DSP POOL> timeslotUseType = %d\n",dspChan_p->timeslotUseType);
}

/******************************************************************************
 * dspPoolDumpDspPool
 *
 * Description: 
 *	Dsp pool function, used to dump dsp_pool information
 *	
 * Parameters:
 * 	Input
 *	
 *
 * Return Value: 
 *
 *****************************************************************************/
void dspPoolDumpDspPool(void){
	int i = 0, j = 0;
	voiceDsp_t *voiceDsp_p = NULL;
	dspChan_t *dspChan_p = NULL;
	dspConf_t *dspConf_p = NULL;

	if(NULL == gDspPool_p)
	{
		VDSP_PRINTF("\r\n dsp pool is not exist!!");
	}

	VDSP_PRINTF("\r\n	DSP POOL DUMP\r\n");

#if 1	/* aircheng add for print format*/
	VDSP_PRINTF("	%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s\n", \
					"dsp", "chan", "call", "logical", "phy", "stream", "stream", "codec", "codec", "chan", "ptime", "silence", "echo", "dtmf");
	VDSP_PRINTF("	%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s%8s\n", \
					" ", " ", "id", "Id", "Id", "id", "type", "type", "num", "State", " ", "Supp", "Cancel", "CodecNum");
	VDSP_PRINTF("	========================================================");
	VDSP_PRINTF("========================================================\n");
#endif

	for( i = 0; i< gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &gDspPool_p->voiceDsp[i];
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if(voiceDsp_p->dspChanList[j] != NULL){
				dspChan_p = voiceDsp_p->dspChanList[j];

#if 1	/* aircheng add for print format*/
				VDSP_PRINTF("	%8d", i);
				VDSP_PRINTF("%8d", j);
				VDSP_PRINTF("%8d", dspChan_p->call_id);
				VDSP_PRINTF("%8d", dspChan_p->phLogicalId);
				VDSP_PRINTF("%8d", dspChan_p->phPhysicalId);
				VDSP_PRINTF("%8d", dspChan_p->stream_id);
				VDSP_PRINTF("%8d", dspChan_p->stream_type);
				#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/
				VDSP_PRINTF("%8d", dspChan_p->codec_type);				
				#endif
				VDSP_PRINTF("%8d", dspChan_p->codec_num);				
				VDSP_PRINTF("%8d", dspChan_p->chanState);
				VDSP_PRINTF("%8d", dspChan_p->ptime);
				VDSP_PRINTF("%8d", dspChan_p->silenceSupp);
				VDSP_PRINTF("%8d", dspChan_p->echoCancel);
				#if 1/*Eason, SPRID # 090826582, dynamic codec number selection*/
				VDSP_PRINTF("%8d", dspChan_p->dtmfCodecNum);
				#endif
				VDSP_PRINTF("\n");
#endif
				
#if 0	/* aircheng delete for print format*/
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d call_id = %d\n",i, j, dspChan_p->call_id);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d logicalId = %d\n",i, j, dspChan_p->phLogicalId);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d physicalId = %d\n",i, j, dspChan_p->phPhysicalId);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d stream_id = %d\n",i, j, dspChan_p->stream_id);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d stream_type = %d\n",i, j, dspChan_p->stream_type);
				#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d codec_type = %d\n",i, j, dspChan_p->codec_type);				
				#endif
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d codec_num = %d\n",i, j, dspChan_p->codec_num);				
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d chanState = %d\n",i, j, dspChan_p->chanState);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d ptime = %d\n",i, j, dspChan_p->ptime);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d silenceSupp = %d\n",i, j, dspChan_p->silenceSupp);
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d echoCancel = %d\n",i, j, dspChan_p->echoCancel);
				#if 1/*Eason, SPRID # 090826582, dynamic codec number selection*/
				VDSP_PRINTF("	DSP POOL> dsp%d chan%d dtmfCodecNum = %d\n",i, j, dspChan_p->dtmfCodecNum);
				#endif
#endif
			}			
		}

		for( j = 0; j< MAX_CONF_PER_DSP; j++){
			if(NULL != voiceDsp_p->dspConfList[j]){
				dspConf_p = voiceDsp_p->dspConfList[j];
				VDSP_PRINTF("	DSP POOL> dsp%d dspConf%d callId = %d\n", i, j, dspConf_p->call_id);
				VDSP_PRINTF("	DSP POOL> dsp%d dspConf%d merged_call_id = %d\n", i, j, dspConf_p->merged_call_id);
				VDSP_PRINTF("	DSP POOL> dsp%d dspConf%d bLogicalId = %d\n", i, j, dspConf_p->bLogicalId);
				VDSP_PRINTF("	DSP POOL> dsp%d dspConf%d cLogicalId = %d\n", i, j, dspConf_p->cLogicalId);
			}
		}
	}
	VDSP_PRINTF("	DSP POOL DUMP END\r\n");
}

/******************************************************************************
 * dspPoolDumpConfDspPool
 *
 * Description: 
 *	Dsp pool function, used to dump dsp_pool information
 *	
 * Parameters:
 * 	Input
 *	
 *
 * Return Value: 
 *
 *****************************************************************************/
void dspPoolDumpConfDspPool(void){
	int i = 0, j = 0;
	voiceDsp_t *voiceDsp_p = NULL;
	dspConf_t *dspConf_p = NULL;

	if(NULL == gDspPool_p)
	{
		VDSP_PRINTF("\r\n dsp pool is not exist!!");
	}

	VDSP_PRINTF("\r\n	CONF DSP POOL DUMP\r\n");
	for( i = 0; i< gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &gDspPool_p->voiceDsp[i];
		for( j = 0; j < MAX_CONF_PER_DSP; j++ ){
			if(NULL != voiceDsp_p->dspConfList[j]){
				dspConf_p = voiceDsp_p->dspConfList[j];
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d dspId = %d\n",i, j, dspConf_p->dspId);
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d confId = %d\n",i, j, dspConf_p->confId);
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d bLogicalId = %d\n",i, j, dspConf_p->bLogicalId);
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d cLogicalId = %d\n",i, j, dspConf_p->cLogicalId);
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d call_id = %d\n",i, j, dspConf_p->call_id);
				VDSP_PRINTF("	CONF POOL> dsp%d conf%d merged_call_id = %d\n",i, j, dspConf_p->merged_call_id);	
				#if 0
				if(NULL != dspConf_p->bDspChan_p){
					VDSP_PRINTF("	CONF POOL> dsp%d b_conf%d phLogicalId = %d\n",i, j, dspConf_p->bDspChan_p->phLogicalId);
				}
				if(NULL != dspConf_p->cDspChan_p){
					VDSP_PRINTF("	CONF POOL> dsp%d c_conf%d phLogicalId = %d\n",i, j, dspConf_p->cDspChan_p->phLogicalId);
				}
				#endif
			}
		}
	}
	VDSP_PRINTF("	DSP POOL DUMP END\r\n");
}

/******************************************************************************
 * dspPoolDumpDspStatus
 *
 * Description: 
 *	Dsp pool function, used to dump driver dsp information in dsp pool
 *	
 * Parameters:
 * 	Input
 *	
 *
 * Return Value: 
 *
 *****************************************************************************/
void dspPoolDumpDspStatus(void){
	int i = 0, j = 0;
	voiceDsp_t *voiceDsp_p = NULL;
	dspChan_t *dspChan_p = NULL;

	if(NULL == gDspPool_p)
	{
		VDSP_PRINTF("\r\n dsp pool is not exist!!");
	}

	VDSP_PRINTF("\r\n	DSP POOL DUMP DSP STATUS\r\n");
	for( i = 0; i< gDspPool_p->dspCount; i++ ){
		voiceDsp_p = &gDspPool_p->voiceDsp[i];
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			if(voiceDsp_p->dspChanList[j] != NULL){
				dspChan_p = voiceDsp_p->dspChanList[j];
				if(dspChan_p->chanState == CHAN_OPENED){
					VDSP_PRINTF("	DSP RTP STATUS> dsp%d chan%d \n",i, j);
					dspDriverDumpRtpStatus(dspChan_p);
				}
				
			}
		}
	}
	VDSP_PRINTF("	DSP POOL DUMP DSP STATUS END\r\n");
}


/******************************************************************************
 * dspPoolFindLogicalDspChan
 *
 * Description: 
 *	Dsp pool function, used to find logical dsp chan with same physical id
 *	
 * Parameters:
 * 	Input
 *	dspChan_t *dspChan_p - find another dsp chan with same physical id
 *
 * Return Value: 
 *	found dsp chan
 *
 *****************************************************************************/
dspChan_t *dspPoolFindLogicalDspChan(dspChan_t *dspChan_p){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0;
	voiceDsp_p = &gDspPool_p->voiceDsp[dspChan_p->dspId];
	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if((voiceDsp_p->dspChanList[i]->phPhysicalId == dspChan_p->phPhysicalId) && (voiceDsp_p->dspChanList[i] != dspChan_p) ){
			return voiceDsp_p->dspChanList[i];
		}
	}

	return NULL;
}


/******************************************************************************
 * dspPoolChannelUsage
 *
 * Description: 
 *	Dsp pool function, used to get channel usage in dsp of specified codec
 *	
 * Parameters:
 * 	Input
 *	uint8 codec_num - codec number in sdp
 *
 * Return Value: uint16
 *	translate codec_num to channel usage in dsp
 *
 *****************************************************************************/
uint16 dspPoolChannelUsage(uint8 codec_num){
	uint16 channel_usage = 0;
	switch(codec_num){
		case ZYNOS_CODEC_PCMA:
		case ZYNOS_CODEC_PCMU:
		case ZYNOS_CODEC_T38:
			channel_usage=1;
			break;
		case ZYNOS_CODEC_G729:
		case ZYNOS_CODEC_G723:
		default:
			channel_usage=2;
			break;
	}

	return channel_usage;
}


/******************************************************************************
 * dspPoolCheckChannelUsage
 *
 * Description: 
 *	Dsp pool function, used to check resource availability of dsp with requested channel usage
 *	
 * Parameters:
 * 	Input
 *	uint16 dspId - dsp id
 *	uint16 req_channel_usage - requested channel usage
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
mm_return_t dspPoolCheckChannelUsage(uint16 dspId, uint16 req_channel_usage){
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0;
	uint16 channel_usage = 0;
	
	voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
	
	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if(NULL != voiceDsp_p->dspChanList[i]){
			channel_usage += dspPoolChannelUsage(voiceDsp_p->dspChanList[i]->codec_num);
		} 
		
	} /*for end*/
	
	VDSP_PRINTF("MM: %s :dsp %d usage %d\n",__FUNCTION__,dspId,channel_usage);
	VDSP_PRINTF("MM: %s :request usage %d\n",__FUNCTION__,req_channel_usage);

	if(channel_usage+req_channel_usage > MAX_DSP_USAGE_PER_DSP){
		return MM_FAILURE;
	}

	return MM_SUCCESS;
}

/******************************************************************************
 * dspPoolInitMutex
 *
 * Description: 
 *	Dsp pool function, used to acquire mutex
 *	
 * Parameters:
 * 	Input
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
void dspPoolInitMutex(void){
	//mutexDspChan = PTHREAD_MUTEX_INITIALIZER;
}

/******************************************************************************
 * dspPoolAcquireMutex
 *
 * Description: 
 *	Dsp pool function, used to acquire mutex
 *	
 * Parameters:
 * 	Input
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
void dspPoolAcquireMutex(void){		
	pthread_mutex_lock(&mutexDspChan);
}

/******************************************************************************
 * dspPoolReleaseMutex
 *
 * Description: 
 *	Dsp pool function, used to release mutex
 *	
 * Parameters:
 * 	Input
 *
 * Return Value: mm_return_t
 *	MM_SUCCESS if success
 *	MM_FAILURE if failure
 *
 *****************************************************************************/
void dspPoolReleaseMutex(void){		
	pthread_mutex_unlock(&mutexDspChan);	
}


/* use for MM by vincent */

/******************************************************************************
 * dspPoolFindDspChanByAllId
 *
 * Description: 
 *	Dsp pool function, used to find dsp chan in dsp pool
 *	
 * Parameters:
 * 	Input
 *	uint16 call_id -find dsp chan by call id
 *
 * Return Value: dspChan_t *
 *    dsp channel found
 *    
 *****************************************************************************/
dspChan_t * dspPoolFindDspChanByAllId(
	icf_uint16_t dspId, 
	icf_call_id_t call_id,
	icf_stream_id_t stream_id,
	icf_media_type_t stream_type
){
	voiceDsp_t *voiceDsp_p = MM_NULL;
	int i = 0;

	if( dspId > gDspPool_p->dspCount){
		return NULL;
	}

	voiceDsp_p = &(gDspPool_p->voiceDsp[dspId]);
	for( i = 0; i < MAX_DSP_CHAN_PER_DSP; i++ ){
		if(voiceDsp_p->dspChanList[i] != NULL){
			if((voiceDsp_p->dspChanList[i]->call_id == call_id)
#if 0
				&& (voiceDsp_p->dspChanList[i]->stream_id == stream_id)
#endif
					&& (voiceDsp_p->dspChanList[i]->stream_type == stream_type)
			){
				return voiceDsp_p->dspChanList[i];
			}
		}
	}	

	VDSP_PRINTF("MM: %s : dsp chan not found, call_id %d \n",__FUNCTION__, call_id);
	return NULL;
}

/******************************************************************************
 * dspPoolFindDspChanByCallId
 *
 * Description: 
 *	Dsp pool function, used to copy media information from dsp chan to dsp chan
 *	
 * Parameters:
 * 	Input
 *	dspChan_t *p_to_dsp_chan,  copy media information to this dsp chan 
 *	dspChan_t *p_from_dsp_chan,  copy media information from this dsp chan
 *
 * Return Value: void
 *    
 *****************************************************************************/

void dspPoolCopyMediaInfoFromDspChan(dspChan_t *p_to_dsp_chan, dspChan_t *p_from_dsp_chan){
	if(NULL != p_to_dsp_chan && NULL != p_from_dsp_chan){
		p_to_dsp_chan->codec_num = p_from_dsp_chan->codec_num;
		#if 1/*Eason, SPRID # 090826582, dynamic codec number selection*/
		p_to_dsp_chan->codec_type = p_from_dsp_chan->codec_type;
		#endif
		p_to_dsp_chan->stream_id = p_from_dsp_chan->stream_id;
		p_to_dsp_chan->stream_type = p_from_dsp_chan->stream_type;
		#if 1	/* aircheng add */
		p_to_dsp_chan->dtmfCodecNum = p_from_dsp_chan->dtmfCodecNum;
		#endif
	}
}

/******************************************************************************
 * dspPoolFindDspChanByCallId
 *
 * Description: 
 *	Dsp pool function, used to replace original audio dsp chan to committed t.38 dsp chan
 *	
 * Parameters:
 * 	Input
 *	dspChan_t *dspChan_p,  the t.38 committed dsp chan
 *
 * Return Value: dspChan_t *
 *	return the replaced dsp chan
 *    
 *****************************************************************************/
dspChan_t* dspPoolDspChanReplaceAudioByT38(dspChan_t *dspChan_p){
	int i =0, j =0;
	voiceDsp_t *voiceDsp_p = NULL;
	dspChan_t *p_audio_dsp_chan = NULL, tmp_dsp_chan;
	

	if(dspChan_p ==NULL){
		return NULL;
	}
	
	for( i = 0; i < gDspPool_p->dspCount; i++ ){
			voiceDsp_p = &gDspPool_p->voiceDsp[i];
			for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){				
				if( (NULL != voiceDsp_p->dspChanList[j]) &&
					(voiceDsp_p->dspChanList[j]->call_id == dspChan_p->call_id) &&
					(voiceDsp_p->dspChanList[j]->phLogicalId == dspChan_p->phLogicalId) &&
					(voiceDsp_p->dspChanList[j]->phPhysicalId == dspChan_p->phPhysicalId) &&
					(voiceDsp_p->dspChanList[j]->codec_num != ZYNOS_CODEC_T38)
					){						
						p_audio_dsp_chan = voiceDsp_p->dspChanList[j];
						VDSP_PRINTF("found replaced audio channel\n");
						break;										
					}
				

			}

			if(NULL != p_audio_dsp_chan ){
				break;
			}
	}
		
	if(NULL != p_audio_dsp_chan){			
		/*copy t38 dsp chan to tmp dsp chan*/
		zyMemcpy(&tmp_dsp_chan, dspChan_p, sizeof(dspChan_t));		
		
		/*copy original audio chan to committed t38 chan and set state to RESERVED*/
		zyMemcpy(dspChan_p, p_audio_dsp_chan, sizeof(dspChan_t));
		dspChan_p->chanId = tmp_dsp_chan.chanId;
		dspChan_p->chanState = CHAN_RESERVED;

		/*set original audio chan to t38 codec*/
		p_audio_dsp_chan->codec_num = tmp_dsp_chan.codec_num;
		#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/ 
		p_audio_dsp_chan->codec_type = tmp_dsp_chan.codec_type;
		#endif
		p_audio_dsp_chan->stream_id = tmp_dsp_chan.stream_id;
		p_audio_dsp_chan->stream_type = tmp_dsp_chan.stream_type;		

		if(CHAN_CHANGE_TO_RESERVED == p_audio_dsp_chan->chanState){ /*original audio dsp chan state, CHANGE_TO_RESERVED change back to OPENED*/
			p_audio_dsp_chan->chanState = CHAN_OPENED;
		}
	}	
	dspPoolDumpDspPool();	
	
	return p_audio_dsp_chan;
}

#ifdef VOIP_SYSLOG /*Jason , syslog*/
/*BL2348_Config*/

mm_return_t voiceDspLogStatsFunc(
  		void						     *inputParam_p,
		mm_error_t					*err_p)

{
	voiceMsg_t	     *msg_p = NULL, *rspMsg_p=NULL;
	voiceDspMmStatsReq_t *payload_p = NULL;
	uint32 line_id;
	uint8  phy_port;
	voiceDspMmStatsRsp_t *rspPayload_p = NULL;	
	
	mm_return_t ret_val = MM_SUCCESS;	
	dspChan_t *dspChan_p = NULL, *p_same_call_dsp_chan = NULL;	
	voiceDsp_t *voiceDsp_p = NULL;
	int i = 0, j = 0;

	VDSP_PRINTF("MM VDSP > entering %s\n",__FUNCTION__);


	if(MM_NULL == inputParam_p )
	{
		VDSP_PRINTF("input fail\n");
		return MM_FAILURE;
	}
	msg_p = (voiceMsg_t *)inputParam_p;
	payload_p = (voiceDspMmStatsReq_t *)msg_p->payload;
	
	MM_DSP_DBG("\r\n	MM Statistics>");
	MM_DSP_DBG1("	mm > Line id = ", payload_p->line_id);
	MM_DSP_DBG1("	mm > physical port = ", payload_p->phy_port);
	ret_val = MM_SUCCESS;	

	line_id = payload_p->line_id;
	phy_port = payload_p->phy_port;

	/*1. allocate voice msg*/
	rspMsg_p = voiceMsgAlloc(VOICE_MM_GET_STREAM_LOG_RSP,sizeof(voiceDspMmStatsRsp_t));

	if( NULL != rspMsg_p ){
		/*2. fill msg payload*/
		rspPayload_p = (voiceDspMmStatsRsp_t*)rspMsg_p->payload;

		dspPoolAcquireMutex();

		ret_val = rtpGetStreamStats(line_id, rspPayload_p);
		if( MM_SUCCESS == ret_val ){
			ZyPrintf("rtpGetSessionStats ok\n");
		}
		else{
			MM_DSP_DBG("get rtp session fail\n");
		}
		dspPoolReleaseMutex();
	}
	else{
		VDSP_PRINTF("alloc fail\n");
		ret_val = MM_FAILURE;
	}
	
	rspPayload_p->result = ret_val;
	
	/*3. send voice event*/
	voiceEventIpcSendToVoiceApp(rspMsg_p);
	
	/*4. free voice message buffer*/			
	voiceMsgFree(rspMsg_p);
	VDSP_PRINTF("MM VDSP > exiting %s, ret %d\n",__FUNCTION__, ret_val);	
	return MM_SUCCESS;	
}
/*BL2348_Config*/
#endif

#if 1	/* aircheng add */
/******************************************************************************
 * dspPoolModifyDspChanCallBack
 *
 * Description: 
 *	Dsp pool function, used to modify CallBack in all dsp chan
 *	
 * Parameters:
 * 	Input
 *	DSPCALLBACK *voipDspCallBack,
 *	DSPCALLBACK *voipDspRtcpCallBack
 *
 * Return Value: void
 *    
 *****************************************************************************/
void dspPoolModifyDspChanCallBack(
	DSPCALLBACK *voipDspCallBack,
	DSPCALLBACK *voipDspRtcpCallBack
){
	voiceDsp_t *voiceDsp_p = NULL;
	dspChan_t *dspChan_p = NULL;
	int i, j;

	for( i = 0; i < gDspPool_p->dspCount; i++ ){
		voiceDsp_p=&(gDspPool_p->voiceDsp[i]);
		for( j = 0; j < MAX_DSP_CHAN_PER_DSP; j++ ){
			dspChan_p = voiceDsp_p->dspChanList[j];
			if(NULL != dspChan_p){
				#if KSOCKET_RTP_SUPPORT
				if(!mmKSocketEnable){
				#endif
					RTP_PRINTF("\n\tMM > %s, modify callback in voiceDsp[%d].dspChan[%d] \n", __FUNCTION__, i, j); 
					dspChan_p->voipDspCallBack = voipDspCallBack;

#ifdef RTP_BCM_RTCP_DRIVER_TASK
					dspChan_p->voipDspRtcpCallBack = voipDspRtcpCallBack;
#endif
				#if KSOCKET_RTP_SUPPORT
				}
				#endif
			}
		}
	}
}
#endif
