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
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

#include "mm_macros.h"
#include "mm_glb.h"
#include "mm_core_fsm_types.h"
#include "mm_core_prototypes.h"
#include "mm_port_wrapper.h"
#include "mm_cfg_types.h"
#include "voice_dsp_common_api.h"
#include "switch.h"

/* Global Database for config parameters */
mmCfgDb_t gMmCfgDb_t;

uint8 mmRmDebug = 0;
uint8 mmRtpDebug = 0;
uint8 mmDspDebug = 0;
#if KSOCKET_RTP_SUPPORT
uint8 mmKSocketEnable = KSOCKET_RTP_ENABLE;
#endif
/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
uint8 log_dest = 0;
/******************************************************************************
 * mmCfgDebug
 *
 * Description: 
 * 
 * Parameters:
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
mmCfgDebug(
	void	*p_input_param,
	mm_error_t	*p_err)
{

	voiceMsg_t	*p_msg = NULL, *p_rsp_msg=NULL;
	mm_set_debug_req_st  *p_payload = NULL;
	mm_common_rsp_st *p_rsp_payload = NULL;
	mm_return_t ret_val = MM_FAILURE;	
	
	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}
	
	p_msg = (voiceMsg_t *)p_input_param;
	p_payload = (mm_set_debug_req_st *)p_msg->payload;

	/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
	log_dest = p_payload->dest;

	if(p_payload->bit_mask & MM_SET_IPTK_DEBUG){
		mmRmDebug = p_payload->iptk_debug;
	}

	if(p_payload->bit_mask & MM_SET_RTP_DEBUG){
		mmRtpDebug = p_payload->rtp_debug;
	}
	
	if(p_payload->bit_mask & MM_SET_DSP_DEBUG){
		mmDspDebug = p_payload->dsp_debug;
	}
	
	

	if(0 != p_payload->bit_mask){
		ret_val = MM_SUCCESS;
	}

	/* send response*/
	p_rsp_msg = voiceMsgAlloc(VOICE_MM_SET_DEBUG_RSP,sizeof(mm_common_rsp_st));
	p_rsp_payload = (mm_common_rsp_st *)p_rsp_msg->payload;
	p_rsp_payload->result = ret_val;

	voiceEventIpcSendToVoiceApp(p_rsp_msg);
	voiceMsgFree(p_rsp_msg);


	ZyPrintf("\r\n ==Dump Start==");
	ZyPrintf("\r\n Debug dest=%d", log_dest);
	ZyPrintf("\r\n Debug Rm=%d", mmRmDebug);
	ZyPrintf("\r\n Debug Rtp=%d", mmRtpDebug);
	ZyPrintf("\r\n Debug Dsp=%d", mmDspDebug);	
	ZyPrintf("\r\n ==Dump End==");
	
	return ret_val;	
}


/******************************************************************************
 * mmCfgDebugStatus
 *
 * Description: 
 * 
 * Parameters:
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
mmCfgDebugStatus(
	void	*p_input_param,
	mm_error_t	*p_err)
{

	voiceMsg_t	*p_msg = NULL, *p_rsp_msg=NULL;
	mm_debug_status_req_st  *p_payload = NULL;
	mm_common_rsp_st *p_rsp_payload = NULL;
	mm_return_t ret_val = MM_SUCCESS;	
	mmCoreCallCtxList_t   *p_tmp_call_context_list = MM_NULL;
	mmCoreCallCtx_t   *p_call_context = MM_NULL;
	uint8 flag =0;

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}
	
	p_msg = (voiceMsg_t *)p_input_param;
	p_payload = (mm_debug_status_req_st *)p_msg->payload;

	if(1 == p_payload->status_rm){
		p_tmp_call_context_list = p_call_context_list;
		while(MM_NULL != p_tmp_call_context_list)
		{
			p_call_context = &p_tmp_call_context_list->call_context;
			flag = mmRmDebug;
			mmRmDebug = 1;
			ZyPrintf("\r\n mmCtxTableDisplay");
			mmCtxTableDisplay(p_call_context);
			mmRmDebug = flag;
			p_tmp_call_context_list = p_tmp_call_context_list->next;
		}
	}
	else if(1 == p_payload->status_rtp){
		flag = mmRtpDebug;
		mmRtpDebug = 1;
		ZyPrintf("\r\n rtpTableDisplay");
		rtpTableDisplay();
		mmRtpDebug = flag;
	}
	else if(1 == p_payload->status_dsp){
		flag = mmDspDebug;
		mmDspDebug = 1;
		ZyPrintf("\r\n dspPoolDumpDspPool");
		dspPoolDumpDspPool();		
		mmDspDebug = flag;
	}
	else if(1 == p_payload->status_ivr){		
		ZyPrintf("\r\n ivrListAllRecord");
		ivrListAllRecord();		
	}

	/* send response*/
	p_rsp_msg = voiceMsgAlloc(VOICE_MM_SET_DEBUG_RSP,sizeof(mm_common_rsp_st));
	p_rsp_payload = (mm_common_rsp_st *)p_rsp_msg->payload;
	p_rsp_payload->result = ret_val;

	voiceEventIpcSendToVoiceApp(p_rsp_msg);
	voiceMsgFree(p_rsp_msg);

	
	return ret_val;	
}

#if KSOCKET_RTP_SUPPORT
/******************************************************************************
 * mmKSocketSet
 *
 * Description: 
 * 
 * Parameters:
 * Return Value: mm_return_t
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/

mm_return_t
mmKSocketSet(
	void	*p_input_param,
	mm_error_t	*p_err)
{

	voiceMsg_t	*p_msg = NULL, *p_rsp_msg=NULL;
	mm_ksocket_set_req_st  *p_payload = NULL;
	mm_common_rsp_st *p_rsp_payload = NULL;
	mm_return_t ret_val = MM_SUCCESS;	


	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}
	
	p_msg = (voiceMsg_t *)p_input_param;
	p_payload = (mm_ksocket_set_req_st *)p_msg->payload;
	MM_PRINTF("%s, Kernel Socket Current Enable = %d\n",__FUNCTION__, mmKSocketEnable);

		
	mmKSocketEnable = p_payload->enable;
	MM_PRINTF("%s, Kernel Socket Set Enable = %d\n",__FUNCTION__, mmKSocketEnable);	

	/* send response*/
	p_rsp_msg = voiceMsgAlloc(VOICE_MM_KSOCKET_SET_RSP,sizeof(mm_common_rsp_st));
	p_rsp_payload = (mm_common_rsp_st *)p_rsp_msg->payload;
	p_rsp_payload->result = ret_val;

	voiceEventIpcSendToVoiceApp(p_rsp_msg);
	voiceMsgFree(p_rsp_msg);

	
	return ret_val;	
}
#endif

