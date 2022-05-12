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

/***************************************************************************
 * Include Files area
 ***************************************************************************/
#include "icf_types.h"
#include "icf_api.h"
#include "icf_clib.h"
#include "icf_error_id.h"
#include "mm_glb.h"
#include "mm_core_fsm_types.h"
#include "mm_macros.h"
#include "mm_port_wrapper.h"
#include "mm_core_fsm_defs.h"
#include "mm_core_prototypes.h"
#include "mm_port_prototypes.h"
#include "voice_dsp_core_fsm_types.h"
#include "tr104.h"
#include "switch.h"
#define CHIP_6368 "6368"
/*************************************************************************
*
*  FUNCTION NAME  : mmCtxTableDisplay
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
void mmCtxTableDisplay(
        mmCoreCallCtx_t   *p_call_context
){
	icf_list_st *p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	icf_list_st *p_ctx_codec_list = MM_NULL;	
	icf_list_st	*p_enc_attr_list = MM_NULL;
	icf_string_st	*p_string = MM_NULL;
	icf_uint8_t	strem_count=0;

	MM_PRINTF("\n\tCTX >  call_id=%d, call_direction=%d", p_call_context->call_id, p_call_context->call_direction);
	
	p_ctx_stream_list = p_call_context->p_stream_list;
	while (MM_NULL != p_ctx_stream_list)
	{
		strem_count++;
		p_ctx_stream = p_ctx_stream_list->p_data;
		MM_PRINTF("\n\tCTX >  Node %d", strem_count);
		MM_PRINTF("\n\tCTX >    stream_id=%d, stream_type=%d", p_ctx_stream->stream_id, p_ctx_stream->stream_type);
		
		MM_PRINTF("\n\tCTX >     stream_address.addr/port=%d.%d.%d.%d/%d", 
			p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1,
			p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2,
			p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3,
			p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4,
			p_ctx_stream->local_sdp.stream_address.port_num);
		MM_PRINTF("\n\tCTX >    rtcp_address.addr/port=%d.%d.%d.%d/%d", 
			p_ctx_stream->local_sdp.rtcp_address.addr.addr.ipv4_addr.octet_1,
			p_ctx_stream->local_sdp.rtcp_address.addr.addr.ipv4_addr.octet_2,
			p_ctx_stream->local_sdp.rtcp_address.addr.addr.ipv4_addr.octet_3,
			p_ctx_stream->local_sdp.rtcp_address.addr.addr.ipv4_addr.octet_4,
			p_ctx_stream->local_sdp.rtcp_address.port_num);
		MM_PRINTF("\n\tCTX >    trans_mode=%d", p_ctx_stream->local_sdp.trans_mode);	
		MM_PRINTF("\n\tCTX >    pref_ptime=%d, silence_suppression=%d, echo_canc=%d", 
			p_ctx_stream->local_sdp.media_stream_param.audio_profile.pref_ptime,
			p_ctx_stream->local_sdp.media_stream_param.audio_profile.silence_suppression,
			p_ctx_stream->local_sdp.media_stream_param.audio_profile.echo_canc);
			
		if(p_ctx_stream->local_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
		{
			p_enc_attr_list = p_ctx_stream->local_sdp.p_encoded_attributes;
			while(MM_NULL != p_enc_attr_list)
			{
				p_string = p_enc_attr_list->p_data;
				MM_PRINTF("\n\tCTX >     enc_attri=%s", p_string->str);
				p_enc_attr_list = p_enc_attr_list->p_next;
			}
		}

		p_ctx_codec_list = p_ctx_stream->local_sdp.p_codec_info_list;
		while (MM_NULL != p_ctx_codec_list)
		{
			p_ctx_codec = p_ctx_codec_list->p_data;
			
			MM_PRINTF("\n\tCTX >       codec_name.str=%s", p_ctx_codec->codec_name.str);
			MM_PRINTF("\n\tCTX >       codec_num=%d", p_ctx_codec->codec_num);
			if(p_ctx_codec->bit_mask & ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT)
			{
				if(p_ctx_codec->codec_params.audio_param.bit_mask & ICF_AUDIO_CODEC_PARAM_CHANNEL)
				{
					MM_PRINTF("\n\tCTX >              channels=%d", 
						p_ctx_codec->codec_params.audio_param.channels);
				}	
				if(p_ctx_codec->codec_params.audio_param.bit_mask & ICF_AUDIO_CODEC_PARAM_AMR_PARAM)
				{
					MM_PRINTF("\n\tCTX >              amr_params.amr_initial_mode=%d, amr_mode_count=%d", 
						p_ctx_codec->codec_params.audio_param.amr_params.amr_initial_mode,
						p_ctx_codec->codec_params.audio_param.amr_params.amr_mode_count);
					MM_PRINTF("\n\tCTX >              codec_mode=%d, crc_enabled=%d", 
						p_ctx_codec->codec_params.audio_param.amr_params.codec_mode,
						p_ctx_codec->codec_params.audio_param.amr_params.crc_enabled);
				}	
				if(p_ctx_codec->codec_params.audio_param.bit_mask & ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE)
				{
					MM_PRINTF("\n\tCTX >              sampling_rate=%d", 
						p_ctx_codec->codec_params.audio_param.sampling_rate);
				}	
				if(p_ctx_codec->codec_params.audio_param.bit_mask & ICF_AUDIO_CODEC_PARAM_VBD_VAL)
				{
					MM_PRINTF("\n\tCTX >              vbd_val=%d", 
						p_ctx_codec->codec_params.audio_param.vbd_val);
				}	
			}

			if(p_ctx_codec->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
			{
				p_enc_attr_list = p_ctx_codec->p_encoded_attributes;
				while(MM_NULL != p_enc_attr_list)
				{
					p_string = p_enc_attr_list->p_data;
					MM_PRINTF("\n\tCTX >	   enc_attri=%s", p_string->str);
					p_enc_attr_list = p_enc_attr_list->p_next;
				}
			}
			
			p_ctx_codec_list = p_ctx_codec_list->p_next;
		}

		MM_PRINTF("\n\tCTX >     media_mode=%d, qos_mark=%d, rtp_detection_reqd=%d, echo_cancellation=%d", 
			p_ctx_stream->media_mode, p_ctx_stream->qos_mark, p_ctx_stream->rtp_detection_reqd, p_ctx_stream->echo_cancellation);	

		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}

}


/*************************************************************************
*
*  FUNCTION NAME  : mmDumpsCtxTrace
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmDumpsCtxTrace(
        mmCoreCallCtx_t   *p_call_context
)
{
	if(MM_NULL != p_call_context){
		mmCtxTableDisplay(p_call_context);
	}
	
	dspPoolDumpDspPool();

	rtpTableDisplay();
	
	return MM_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : dspGetPhoneOffhookByDspId
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t dspGetPhoneOffhookByDspId(
	icf_call_id_t	call_id,
	icf_uint16_t	dspId
){
	voiceDsp_t	*voiceDsp_p = MM_NULL;
	dspChan_t	*dspChan_p = MM_NULL;
	icf_uint8_t count = 0;
	
	if(MM_NULL == gDspPool_p)
	{
		MM_PRINTF("\r\n dsp pool is not exist!!");
	}

	voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
	for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
	{
		dspChan_p = voiceDsp_p->dspChanList[count];
		if (MM_NULL != dspChan_p)
		{
			if(call_id == dspChan_p->call_id) 
			{
				MM_PRINTF("\n\tMM >Get phone offhooked here, dspId=%d", dspId);	
				return MM_SUCCESS;
			}
		}
	}

	return MM_FAILURE;
}

/*************************************************************************
*
*  FUNCTION NAME  : dspGetPhoneOffhookByPhyId
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t dspGetPhoneOffhookByPhyId(
	icf_uint16_t	phPhysicalId,
	icf_uint16_t	dspId
){
	voiceDsp_t	*voiceDsp_p = MM_NULL;
	dspChan_t	*dspChan_p = MM_NULL;
	icf_uint8_t count = 0;
	
	if(MM_NULL == gDspPool_p)
	{
		MM_PRINTF("\r\n dsp pool is not exist!!");
	}

	voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
	for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
	{
		dspChan_p = voiceDsp_p->dspChanList[count];
		if (MM_NULL != dspChan_p)
		{
			if(phPhysicalId == dspChan_p->phPhysicalId) 
			{
				MM_PRINTF("\n\tMM >Get phone offhooked here, physicalId=%d", phPhysicalId);	
				return MM_SUCCESS;
			}
		}
	}

	return MM_FAILURE;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmIsMatchedCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmIsMatchedCodec(
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	matched_codec_num,
	icf_short_string_st	*p_matched_codec_name
) {
	mm_return_t	ret_val = MM_FAILURE;

	if((p_input_codec->bit_mask & ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT)
		&& (p_input_codec->codec_num <= 96))
	{
		if(p_input_codec->codec_num == matched_codec_num)
		{
			ret_val = MM_SUCCESS;
		}
	}
	else
	{	/* Dynamic Codec Present */	
		if(0 == zyStrcasecmp(p_input_codec->codec_name.str, 
									p_matched_codec_name->str))
		{			
			ret_val = MM_SUCCESS;
		}
	}

	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmCheckCodecSupport
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCheckCodecSupport(
	mmCoreCallCtx_t   *p_call_context,
 	icf_stream_capab_st	*p_input_stream,
	icf_codec_attrib_st		*p_input_codec
){
	icf_cfg_codec_info_req_st	*p_cfg_codec_info = MM_NULL;
	icf_config_codec_attrib_st *p_cfg_codec_attrib = NULL;
	mm_return_t	ret_val = MM_FAILURE;

	p_cfg_codec_attrib = mmGetSupportedCodec(p_call_context, p_input_codec, p_input_stream->stream_type);
	if(MM_NULL != p_cfg_codec_attrib)
	{
		/* replace the codec_name to avoid returned codec_name is empty */
		if(0 == zyStrlen(p_input_codec->codec_name.str))
		{				
			zyStrcpy(p_input_codec->codec_name.str, 
							p_cfg_codec_attrib->codec_name.str);
			p_input_codec->codec_name.str_len = 
				zyStrlen(p_input_codec->codec_name.str);
		}

		
		#if 0 /*Eason, SPRID # 090826582, dynamic codec number selection*/
		/* Assign our configured codec_num when peer codec_num is not presented */
		p_input_codec->codec_num = p_cfg_codec_attrib->codec_num;	
		p_input_codec->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
		#endif
		
		ret_val = MM_SUCCESS;
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmGetSupportedCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_config_codec_attrib_st* mmGetSupportedCodec(
	mmCoreCallCtx_t   *p_call_context,
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	stream_codec_type
){
	icf_cfg_codec_info_req_st	*p_cfg_codec_info = MM_NULL;
	icf_config_codec_attrib_st *p_cfg_codec_attrib = NULL;
	icf_uint8_t	count = 0;

	p_cfg_codec_info = &p_call_context->p_mm_cfg_db->codec_info[p_call_context->line_id];
	for(count = 0; count < p_cfg_codec_info->count; count++)
	{
		p_cfg_codec_attrib = &p_cfg_codec_info->codec_supp[count];

		if((0 == p_cfg_codec_attrib->enable) || (stream_codec_type != p_cfg_codec_attrib->codec_type))
		{
			continue;
		}

		if(MM_SUCCESS == 
			mmIsMatchedCodec(p_input_codec, p_cfg_codec_attrib->codec_num, &p_cfg_codec_attrib->codec_name))
		{
			return p_cfg_codec_attrib;
		}
	}

	return MM_NULL;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmCheckDspCodecRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint16_t mmCheckDspCodecRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_uint8_t	codec_num
) {
	icf_uint16_t reqChannels = 0xff;
	icf_uint16_t dspId = 0;

	if (NULL == gDspPool_p)
	{	
		return MM_INVALID_DSP_ID;
	}

	for (dspId = 0; dspId < gDspPool_p->dspCount; dspId++)
	{	
		reqChannels = dspPoolChannelUsage(codec_num);
		if (MM_OUTGOING_CALL == p_call_context->call_direction)
		{	
			/* In off-hook dialing condition, dsp channel already opened using G.711 */
			if(MM_SUCCESS == dspGetPhoneOffhookByDspId(p_call_context->call_id, dspId))
			{
				reqChannels -= dspPoolChannelUsage(ZYNOS_CODEC_PCMU);
			}
		}

		if(MM_SUCCESS == dspPoolCheckChannelUsage(dspId, reqChannels))
		{
			return dspId;
		}
	}	

	return MM_INVALID_DSP_ID;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmAllocatedDspPool
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmAllocatedDspPool(
	icf_call_id_t	call_id,
	icf_uint16_t	dspId,
	icf_stream_capab_st	*p_input_stream,
	icf_uint8_t	codec_num
){
	icf_uint16_t	count = 0;
	voiceDsp_t	*voiceDsp_p = MM_NULL;	
	dspChan_t	*dspChan_p = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	
	if(MM_NULL == gDspPool_p)
	{
		return MM_FAILURE;
	}

	voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
	for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
	{
		if(MM_NULL == voiceDsp_p->dspChanList[count])
		{
			dspChan_p = dspPoolAllocateDspChan();
			dspChan_p->call_id = call_id;
			dspChan_p->stream_id = p_input_stream->stream_id;
			dspChan_p->stream_type = p_input_stream->stream_type;
			dspChan_p->codec_num = codec_num;
			#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/ 
			dspChan_p->codec_type = codec_num;
			#endif
			dspChan_p->dspId = dspId;
			dspChan_p->chanId = count;
			dspChan_p->phLogicalId = ID_NOT_USED;
			dspChan_p->phPhysicalId = ID_NOT_USED;
			dspChan_p->chanState = CHAN_RESERVED;
			dspChan_p->phPortType = 0;
			dspChan_p->timeslot = 0;
			dspChan_p->timeslotUseType = 0;
			dspChan_p->voipDspCallBack = MM_NULL;

			if(ICF_STREAM_TYPE_AUDIO == p_input_stream->stream_type)
			{
				dspChan_p->ptime = p_input_stream->media_stream_param.audio_profile.pref_ptime;
				dspChan_p->silenceSupp = p_input_stream->media_stream_param.audio_profile.silence_suppression;
				dspChan_p->echoCancel = p_input_stream->media_stream_param.audio_profile.echo_canc;
			}

			voiceDsp_p->dspChanList[count] = dspChan_p;


			ret_val = MM_SUCCESS;
			break;
		}	
	}

	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmFreeDspPoolByCallId
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmFreeDspPoolByCallId(
	icf_call_id_t	call_id
){
	voiceDsp_t	*voiceDsp_p = MM_NULL;	
	dspChan_t	*dspChan_p = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	dspId = 0;
	icf_uint16_t	count = 0;
	
	if(MM_NULL == gDspPool_p)
	{
		return MM_FAILURE;
	}

	for(dspId = 0; dspId < gDspPool_p->dspCount; dspId++)
	{
		voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
		for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
		{
			dspChan_p = voiceDsp_p->dspChanList[count];
			if(MM_NULL != dspChan_p)
			{
				if(call_id == dspChan_p->call_id)
				{
					if( CHAN_OPENED != dspChan_p->chanState)
					{
						zyFree(dspChan_p);
						voiceDsp_p->dspChanList[count] = MM_NULL;					

						ret_val = MM_SUCCESS;
					}
				}	
			}
		}	
	}

	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmFreeDspPoolByStreamId
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmFreeDspPoolByStreamId(
	icf_call_id_t	call_id,
	icf_stream_id_t	stream_id
){
	voiceDsp_t	*voiceDsp_p = MM_NULL;	
	dspChan_t	*dspChan_p = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	dspId = 0;
	icf_uint16_t	count = 0;
	
	if(MM_NULL == gDspPool_p)
	{
		return MM_FAILURE;
	}

	for(dspId = 0; dspId < gDspPool_p->dspCount; dspId++)
	{
		voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
		for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
		{
			dspChan_p = voiceDsp_p->dspChanList[count];
			if(MM_NULL != dspChan_p)
			{
				if((call_id == dspChan_p->call_id) && (stream_id == dspChan_p->stream_id))
				{
					if( CHAN_OPENED != dspChan_p->chanState)
					{
						zyFree(dspChan_p);
						voiceDsp_p->dspChanList[count] = MM_NULL;					

						ret_val = MM_SUCCESS;
					}				
				}	
			}
		}	
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmChangeDspPoolToReserved
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmChangeDspPoolToReserved(
	icf_call_id_t	call_id,
	mmCtxStream_t	*p_ctx_stream
){
	voiceDsp_t	*voiceDsp_p = MM_NULL;	
	dspChan_t	*dspChan_p = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	dspId = 0;
	icf_uint16_t	count = 0;

	if(MM_NULL == gDspPool_p)
	{
		return MM_FAILURE;
	}

	for(dspId = 0; dspId < gDspPool_p->dspCount; dspId++)
	{
		voiceDsp_p = &gDspPool_p->voiceDsp[dspId];
		for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
		{
			dspChan_p = voiceDsp_p->dspChanList[count];
			if(MM_NULL != dspChan_p)
			{
				if(( dspChan_p->call_id == call_id) 
					&& (dspChan_p->stream_id == p_ctx_stream->stream_id)
					&& (dspChan_p->stream_type == p_ctx_stream->stream_type))
				{
					if( CHAN_OPENED == dspChan_p->chanState)
					{
						/* Assign zero value as a unused DSP channel */
						dspChan_p->chanState = CHAN_CHANGE_TO_RESERVED;
						ret_val = MM_SUCCESS;
					}
				}	
			}
		}	
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmChangeDspPoolPosition
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmChangeDspPoolPosition(
	icf_uint16_t	select_dsp_id,
	dspChan_t	*p_input_dsp_chan
){
	mm_return_t	ret_val = MM_FAILURE;
	dspChan_t	*p_new_dsp_chan = MM_NULL;
	icf_uint16_t	chan_count = 0;
	
	p_new_dsp_chan = zyMalloc(sizeof(dspChan_t));
	zyMemcpy(p_new_dsp_chan, p_input_dsp_chan, sizeof(dspChan_t));
	p_input_dsp_chan->chanState = CHAN_COMMITTED;

	for(chan_count = 0; chan_count < MAX_DSP_CHAN_PER_DSP; chan_count++)
	{
		if(MM_NULL == gDspPool_p->voiceDsp[select_dsp_id].dspChanList[chan_count])
		{
			gDspPool_p->voiceDsp[select_dsp_id].dspChanList[chan_count] = p_new_dsp_chan;
			ret_val = MM_SUCCESS;
			break;
		}
	}

	/* Phyisical dsp channel need be closed when dsp channel re-Open */
	p_input_dsp_chan->chanState = CHAN_CHANGE_TO_RESERVED;
					
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmCheckDspConfRestriction
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCheckDspConfRestriction(
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
){
	mm_return_t	ret_val = MM_SUCCESS;

	/* This rule is Boardcom 6368 limitation, 
	* two G729 on conference at the same time is not allowed, allow one G729 and another is G711A/U */
	if(!strcmp(CHIP_ID, CHIP_6368)) {
    	if((ZYNOS_CODEC_G729 == dspChan_p->codec_num) 
    		&& (ZYNOS_CODEC_G729 == p_merge_dsp_chan->codec_num))
    	{
    		ret_val = MM_FAILURE;
    	}
    }
	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmCheckDspConfRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint16_t mmCheckDspConfRm(
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
){
	icf_uint16_t	select_dsp_id = MM_INVALID_DSP_ID;
	icf_uint16_t	dsp_channels = 0xff, merge_channels = 0xff, req_channels = 0xff;
	icf_uint16_t	dspCount = 0;
	
	if(dspChan_p->dspId == p_merge_dsp_chan->dspId)
	{
		select_dsp_id = dspChan_p->dspId;
	}
	else
	{
		dsp_channels = dspPoolChannelUsage(dspChan_p->codec_num);
		merge_channels = dspPoolChannelUsage(p_merge_dsp_chan->codec_num);

		for(dspCount = 0; dspCount < gDspPool_p->dspCount; dspCount++)
		{
			if(dspCount != dspChan_p->dspId)
			{
				req_channels += dsp_channels;		
			}

			if (dspCount != p_merge_dsp_chan->dspId)
			{
				req_channels += merge_channels;
			}

			if(MM_SUCCESS == dspPoolCheckChannelUsage(dspChan_p->dspId, req_channels))
			{
				select_dsp_id = dspChan_p->dspId;

				if(select_dsp_id != dspChan_p->dspId)
				{
					mmChangeDspPoolPosition(select_dsp_id, dspChan_p);
				}

				if(select_dsp_id != p_merge_dsp_chan->dspId)
				{
					mmChangeDspPoolPosition(select_dsp_id, p_merge_dsp_chan);
				}
				break;
			}
		}
	}
	MM_PRINTF("\n\tMM >Exit %s, select_dsp_id=%d\n", __FUNCTION__, select_dsp_id);
	return select_dsp_id;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmAllocatedConfDspPool
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmAllocatedConfDspPool(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_uint16_t	dspId,
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
){
	dspConf_t	*p_new_dsp_conf = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	conf_count = 0;

	for(conf_count = 0; conf_count < MAX_CONF_PER_DSP; conf_count++)
	{
		if(MM_NULL == gDspPool_p->voiceDsp[dspId].dspConfList[conf_count])
		{
			p_new_dsp_conf = (dspConf_t *)zyMalloc(sizeof(dspConf_t));
			p_new_dsp_conf->confId = conf_count;
			p_new_dsp_conf->bLogicalId = dspChan_p->phLogicalId;
			p_new_dsp_conf->cLogicalId = p_merge_dsp_chan->phLogicalId;
			p_new_dsp_conf->dspId = dspId;
			p_new_dsp_conf->call_id = call_id;
			p_new_dsp_conf->merged_call_id = merged_call_id;
			#if 0
			p_new_dsp_conf->bDspChan_p = dspChan_p;
			p_new_dsp_conf->cDspChan_p = p_merge_dsp_chan;
			#endif
			gDspPool_p->voiceDsp[dspId].dspConfList[conf_count] = p_new_dsp_conf;
			return MM_SUCCESS;
		}
	}
	
	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmCreateDspConf
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCreateDspConf(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_stream_id_t	stream_id,
	icf_stream_id_t	merge_stream_id
) {
	dspChan_t	*dspChan_p = MM_NULL;
	dspChan_t	*p_merge_dsp_chan = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	dspId = 0;

	dspChan_p = dspPoolFindDspChanByStreamId(call_id, stream_id);
	p_merge_dsp_chan = dspPoolFindDspChanByStreamId(merged_call_id, merge_stream_id);

	if((MM_NULL != dspChan_p) && (MM_NULL != p_merge_dsp_chan))
	{
		ret_val = mmCheckDspConfRestriction(dspChan_p, p_merge_dsp_chan);
		if(MM_SUCCESS == ret_val)
		{
			dspId = mmCheckDspConfRm(dspChan_p, p_merge_dsp_chan);
			if(MM_INVALID_DSP_ID == dspId)
			{
				ret_val = MM_FAILURE;
			}
			else
			{
				ret_val = mmAllocatedConfDspPool(
							call_id, 
							merged_call_id, 
							dspId, 
							dspChan_p, 
							p_merge_dsp_chan);
			}
		}
	}


	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;	
}
/*************************************************************************
*
*  FUNCTION NAME  : mmDeleteConfDspPool
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmDeleteConfDspPool(
	icf_uint16_t	dspId,
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
){
	dspConf_t	*dspConf_p = MM_NULL;
	icf_uint16_t	conf_count = 0;

	if((MM_NULL == dspChan_p) || (MM_NULL == p_merge_dsp_chan))
	{
		return MM_FAILURE;
	}

	for(conf_count = 0; conf_count < MAX_CONF_PER_DSP; conf_count++)
	{
		dspConf_p = gDspPool_p->voiceDsp[dspId].dspConfList[conf_count];
		if(MM_NULL != dspConf_p)
		{
			if((dspChan_p->phLogicalId == dspConf_p->bLogicalId)
				&& (p_merge_dsp_chan->phLogicalId == dspConf_p->cLogicalId))
			{
				zyFree(dspConf_p);
				gDspPool_p->voiceDsp[dspId].dspConfList[conf_count] = MM_NULL;

				return MM_SUCCESS;
			}
		}
	}
	return MM_FAILURE;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmDeleteDspConf
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmDeleteDspConf(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_stream_id_t	stream_id,
	icf_stream_id_t	merge_stream_id
){
	dspChan_t	*dspChan_p = MM_NULL;
	dspChan_t	*p_merge_dsp_chan = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	dspCount = 0;

	dspChan_p = dspPoolFindDspChanByStreamId(call_id, stream_id);
	p_merge_dsp_chan = dspPoolFindDspChanByStreamId(merged_call_id, merge_stream_id);

	if((MM_NULL != dspChan_p) && (MM_NULL != p_merge_dsp_chan))
	{
		for(dspCount = 0; dspCount < gDspPool_p->dspCount; dspCount++)
		{
			ret_val = mmDeleteConfDspPool(dspCount, dspChan_p, p_merge_dsp_chan);
		}
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
*
*  FUNCTION NAME  : mmCtxGetStream
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mmCtxStream_t* mmCtxGetStream(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_t		stream_id
){
	icf_list_st		*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;

	p_ctx_stream_list = p_call_context->p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;
		if(p_ctx_stream->stream_id == stream_id)
		{
			MM_PRINTF("\n\tMM >Exit %s, get stream_id=%d", __FUNCTION__, p_ctx_stream->stream_id);
			return p_ctx_stream;
		}
		
		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}

	return MM_NULL;
}
#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/
/*************************************************************************
*
*  FUNCTION NAME  : mmGetCodecTypeByName
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint8_t mmGetCodecTypeByName(char *codecName){
	if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_TELEPHONE_EVENT)){
		return ZYNOS_CODEC_RFC2833;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_G726_16)){
		return ZYNOS_CODEC_G726_16;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_G726_24)){
		return ZYNOS_CODEC_G726_24;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_G726_32)){
		return ZYNOS_CODEC_G726_32;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_G726_40)){
		return ZYNOS_CODEC_G726_40;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_PCMA)){
		return ZYNOS_CODEC_PCMA;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_PCMU)){
		return ZYNOS_CODEC_PCMU;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_G729)){
		return ZYNOS_CODEC_G729;
	}
	else if(!zyStrcasecmp(codecName, SDP_CODEC_NAME_T38)){
		return ZYNOS_CODEC_T38;
	}
	else{
		ZyPrintf("cannot get codec type by name : %s\n", codecName);
		return ZYNOS_CODEC_PCMU;
	}

}


/*************************************************************************
*
*  FUNCTION NAME  : mmCtxGetCommitCodecType
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint8_t	mmCtxGetCommitCodecType(
	mmCtxStream_t	*p_ctx_stream
){
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;

	if(MM_NULL != p_ctx_stream->local_sdp.p_codec_info_list)
	{
		p_ctx_codec = p_ctx_stream->local_sdp.p_codec_info_list->p_data;
		if(p_ctx_codec->codec_num >= 96){
			return mmGetCodecTypeByName(p_ctx_codec->codec_name.str);
		}
		else{
			return p_ctx_codec->codec_num;
		}
	} else {
		return ZYNOS_CODEC_UNKNOWN;
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCtxGetCommitDtmfCodecNum
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint8_t	mmCtxGetCommitDtmfCodecNum(
	mmCtxStream_t	*p_ctx_stream
){
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	icf_list_st *list = NULL;	


	list = p_ctx_stream->local_sdp.p_codec_info_list;
	while(NULL != list){		
		p_ctx_codec = list->p_data;
		if(NULL != p_ctx_codec){
			if(!zyStrcasecmp(p_ctx_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT)){
				return p_ctx_codec->codec_num;
			}
		}
		
		list = list->p_next;
	}
	
	return 0; /*if no telephone event found for dtmf codec num, should use inband*/

}

#endif

/*************************************************************************
*
*  FUNCTION NAME  : mmCtxGetCommitCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_uint8_t	mmCtxGetCommitCodec(
	mmCtxStream_t	*p_ctx_stream
){
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;

	if(MM_NULL != p_ctx_stream->local_sdp.p_codec_info_list)
	{
		p_ctx_codec = p_ctx_stream->local_sdp.p_codec_info_list->p_data;

		return p_ctx_codec->codec_num;
	} else {
		return ZYNOS_CODEC_UNKNOWN;
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCommitDspRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCommitDspRm(
        mmCoreCallCtx_t   *p_call_context,
   	icf_stream_commit_st	*p_commited_stream
){
	icf_list_st		*p_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	voiceDsp_t *voiceDsp_p=MM_NULL;
	dspChan_t *dspChan_p = MM_NULL;
	dspChan_t **p_resv_dsp_chan[MAX_DSP*MAX_DSP_CHAN_PER_DSP];
	mm_return_t	ret_val = MM_SUCCESS;
	icf_uint16_t	dspId = 0;
	icf_uint16_t	count = 0;
	icf_uint16_t	resv_count = 0;
	icf_uint16_t	selected_index = 0;
	icf_uint8_t	commit_codec = 0;	
	#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/
	icf_uint8_t	codecType = 0;
	#endif

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);	

	p_stream_list = p_call_context->p_stream_list;
	while(MM_NULL != p_stream_list)
	{
		p_ctx_stream = p_stream_list->p_data;
	
		if (p_ctx_stream->stream_id == p_commited_stream->stream_id){
		
		/* Filter rule 1: select reserved dsp channels for the stream */
		resv_count = 0;
		for(dspId = 0; dspId < gDspPool_p->dspCount; dspId++)
		{
			voiceDsp_p = &(gDspPool_p->voiceDsp[dspId]);
			for(count = 0; count < MAX_DSP_CHAN_PER_DSP; count++)
			{
				if(MM_NULL != voiceDsp_p->dspChanList[count] )
				{
					if((voiceDsp_p->dspChanList[count]->call_id == p_call_context->call_id)
						&& (voiceDsp_p->dspChanList[count]->stream_id == p_commited_stream->stream_id))
					{
						#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, codecType is predefined number ZYNOS_CODEC_XXXX, codecNum is input from iptk*/
						codecType = mmCtxGetCommitCodecType(p_ctx_stream);
						#endif
						commit_codec = mmCtxGetCommitCodec(p_ctx_stream);

						if(dspPoolChannelUsage(voiceDsp_p->dspChanList[count]->codec_num)
								>= dspPoolChannelUsage(commit_codec))
						{
							#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, dtmfCodecNum is telephone-event number*/
							voiceDsp_p->dspChanList[count]->codec_type = codecType;	
							voiceDsp_p->dspChanList[count]->codec_num = commit_codec;
							voiceDsp_p->dspChanList[count]->dtmfCodecNum = mmCtxGetCommitDtmfCodecNum(p_ctx_stream);
							#else
							voiceDsp_p->dspChanList[count]->codec_num = commit_codec;	
							#endif
							
							if(CHAN_OPENED != voiceDsp_p->dspChanList[count]->chanState){
								voiceDsp_p->dspChanList[count]->chanState = CHAN_COMMITTED;
							}
							voiceDsp_p->dspChanList[count]->ptime = 
								p_ctx_stream->local_sdp.media_stream_param.audio_profile.pref_ptime;
							voiceDsp_p->dspChanList[count]->silenceSupp =
								p_ctx_stream->local_sdp.media_stream_param.audio_profile.silence_suppression;
							voiceDsp_p->dspChanList[count]->echoCancel = 
								p_ctx_stream->local_sdp.media_stream_param.audio_profile.echo_canc;

							p_resv_dsp_chan[resv_count] = &voiceDsp_p->dspChanList[count];
							resv_count++;
						}
						else
						{
							zyFree(voiceDsp_p->dspChanList[count]);
						}
					}
				}
			}
		}
		/* Filter rule 2: choice one dsp channel is already opened before */		
		selected_index = 0;
		for(count = 0; count < resv_count; count++)
		{
			dspChan_p = *(p_resv_dsp_chan[count]);
			if(CHAN_OPENED == dspChan_p->chanState)
			{
				selected_index = count;
			}
		}
		/* Filter rule 3: free other dsp channel reserved before unless selected before */
		for(count = 0; count < resv_count; count++)
		{
			if(count != selected_index)
			{
				zyFree((*p_resv_dsp_chan[count]));
				*p_resv_dsp_chan[count] = MM_NULL;
			}
		}

		if(resv_count <= 0)
		{
			ret_val = MM_FAILURE;
		}

		}
		p_stream_list = p_stream_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);	
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmReserveDspRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmReserveDspRm(
	icf_call_id_t	call_id,
	icf_uint16_t		dspId,
 	icf_stream_capab_st	*p_input_stream,
	icf_codec_attrib_st		*p_input_codec
){
	dspChan_t *dspChan_p= MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;

	if(dspId > gDspPool_p->dspCount)
	{
		return MM_FAILURE;
	}

	dspChan_p = dspPoolFindDspChanByAllId(dspId, call_id, p_input_stream->stream_id,p_input_stream->stream_type);
	if(MM_NULL != dspChan_p)
	{
		if(0 == dspChan_p->stream_id)
		{
			dspChan_p->stream_id = p_input_stream->stream_id;
		}
		if(ICF_STREAM_TYPE_INVALID == dspChan_p->stream_type)
		{
			dspChan_p->stream_type = p_input_stream->stream_type;
		}		

		/* Check and record usage type on each DSP */			
		if (dspPoolChannelUsage(p_input_codec->codec_num) > 
								dspPoolChannelUsage(dspChan_p->codec_num))
		{
			/* Update DSP channel's codec_num */
			dspChan_p->codec_num = p_input_codec->codec_num;
		}

		ret_val = MM_SUCCESS;
	}
	else
	{
		ret_val = mmAllocatedDspPool(call_id, dspId, p_input_stream, p_input_codec->codec_num);
	}

	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCtxSetSelfIp
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCtxSetSelfIp(
	mmCoreCallCtx_t   *p_call_context,
	mmCtxStream_t		*p_ctx_stream
){
	icf_cfg_set_self_ip_req_st	tmp_self_ip;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);
	MM_PRINTF("p_call_context->line_id = %d\n", p_call_context->line_id);
	zyMemcpy(&tmp_self_ip, &p_call_context->p_mm_cfg_db->self_ip[p_call_context->line_id], sizeof(icf_cfg_set_self_ip_req_st));

	p_ctx_stream->local_sdp.stream_address.addr.addr_type = 
		tmp_self_ip.self_ip_addr.addr_type;

	p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1
		= atoi(zyStrtok((char *)tmp_self_ip.self_ip_addr.addr_val.str,(char *)".\0"));/*octet_info[0];*/

	p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2
       	= atoi(zyStrtok(MM_NULL,(char *)".\0"));/*octet_info[1];*/

	p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3
        	= atoi(zyStrtok(MM_NULL,(char *)".\0"));/*octet_info[2];*/

	p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4
        	= atoi(zyStrtok(MM_NULL,(char *)".\0"));/*octet_info[3];*/
				
	return MM_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCtxCreateNewStream
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mmCtxStream_t* mmCtxCreateNewStream(
	mmCoreCallCtx_t   *p_call_context
){
	mmCtxStream_t	*p_ctx_stream= MM_NULL;
	
	p_ctx_stream = zyMalloc(sizeof(mmCtxStream_t));

	mmAddNodeToList(&p_call_context->p_stream_list, p_ctx_stream);
	p_call_context->stream_count++;

	return p_ctx_stream;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mm_commit_stream_codec
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCtxCommitCodec(
	icf_list_st			*p_commit_codec_list,
	icf_stream_commit_st		*p_commited_stream,
	mmCtxStream_t	*p_ctx_stream
){
	icf_list_st			*p_input_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_input_codec = MM_NULL;
	icf_codec_attrib_st		*p_input_dtmf_codec = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);
	
	p_input_codec_list = p_commit_codec_list;
	while(MM_NULL != p_input_codec_list)
	{
		p_input_codec = p_input_codec_list->p_data;
		#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, commit one voice codec and telephone-event if any*/
		if(!zyStrcasecmp(p_input_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT)){
			if(MM_SUCCESS == mmCommitMatchedCodec(p_ctx_stream->local_sdp.p_codec_info_list, p_input_codec)){
				mmFillCommitedStream(p_commited_stream, p_input_codec);			
			}
		}
		else if ( (MM_SUCCESS!=ret_val) && (MM_SUCCESS == mmCommitMatchedCodec(p_ctx_stream->local_sdp.p_codec_info_list, p_input_codec)))
		/*only commit first voice codec except telephone event*/
		#else
		if (MM_SUCCESS == mmCommitMatchedCodec(p_ctx_stream->local_sdp.p_codec_info_list, p_input_codec))
		#endif
		{
			mmFillCommitedStream(p_commited_stream, p_input_codec);
			ret_val = MM_SUCCESS;			
			#if 0 /*Eason, SPRID # 090826582, dynamic codec number selection, commit all codecs input from iptk if supported*/
			break;
			#endif
						
		}			
		p_input_codec_list = p_input_codec_list->p_next;
	}

	#if 0 /*Eason, SPRID # 090826582, dynamic codec number selection*/
	if(MM_SUCCESS == ret_val)
	{
		/* Check RFC2800 "telephone-event" if support */
		if(MM_SUCCESS == mmCheckDtmfCodec(p_ctx_stream->local_sdp.p_codec_info_list, &p_input_dtmf_codec))
		{
			mmFillCommitedStream(p_commited_stream, p_input_dtmf_codec);
		}
	}
	#endif

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCtxCheckMatchedCodec
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCtxCheckMatchedCodec(
	icf_stream_commit_st	*p_commited_stream,
	icf_codec_attrib_st		*p_input_codec
){
	icf_uint8_t	count = 0;
	mm_return_t	ret_val = MM_FAILURE;

	for(count = 0; count < p_commited_stream->codec_count; count++)
	{
		if(MM_SUCCESS == 
			mmIsMatchedCodec(p_input_codec, 
									p_commited_stream->codec[count].codec_id, 
									&p_commited_stream->codec[count].codec_name))
		{
			ret_val = MM_SUCCESS;
			break;
		}
	}
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCtxCommitStreamList
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCtxCommitStreamList(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st	*p_input_stream_list
){
	icf_list_st	*p_commited_stream_list = MM_NULL;
	icf_list_st	*p_ctx_codec_info_list = MM_NULL;
	icf_list_st	*p_tmp_codec_info_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	icf_codec_attrib_st		*p_output_codec = MM_NULL;
	icf_codec_attrib_st		*p_output_dtmf_codec = MM_NULL;
	icf_stream_commit_st	*p_commited_stream = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	
	p_commited_stream_list = p_input_stream_list;
	while(MM_NULL != p_commited_stream_list)
	{
		p_commited_stream = p_commited_stream_list->p_data;

		p_ctx_stream = mmCtxGetStream(p_call_context, p_commited_stream->stream_id);
		if(MM_NULL != p_ctx_stream)
		{
			p_ctx_codec_info_list = p_ctx_stream->local_sdp.p_codec_info_list;
			while(MM_NULL != p_ctx_codec_info_list)
			{
				p_ctx_codec = p_ctx_codec_info_list->p_data;
				if(MM_SUCCESS == mmCtxCheckMatchedCodec(p_commited_stream, p_ctx_codec))
				{
					if(0 == zyStrcmp(p_ctx_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT))
					{
						p_output_dtmf_codec = zyMalloc(sizeof(icf_codec_attrib_st));
						zyMemcpy(p_output_dtmf_codec, p_ctx_codec, sizeof(icf_codec_attrib_st));	
					}
					else
					{
						p_output_codec = zyMalloc(sizeof(icf_codec_attrib_st));
						zyMemcpy(p_output_codec, p_ctx_codec, sizeof(icf_codec_attrib_st));	
					}
				}
				p_ctx_codec_info_list = p_ctx_codec_info_list->p_next;
			}

			if(MM_NULL != p_output_codec)
			{
				if(MM_SUCCESS == mmCheckMediaStreamParamSupport(p_call_context, 
																		p_output_codec,
																		ICF_STREAM_TYPE_INVALID,
																		MM_NULL, 
																		&p_ctx_stream->local_sdp.media_stream_param))
				{
					p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
				}
			}

			if(MM_NULL != p_output_codec)
			{
				mmAddNodeToList(&p_tmp_codec_info_list, p_output_codec);

				ret_val = MM_SUCCESS;
			}
			if(MM_NULL != p_output_dtmf_codec)
			{
				mmAddNodeToList(&p_tmp_codec_info_list, p_output_dtmf_codec);
			}
			mmDeleteCodecAttribList(&p_ctx_stream->local_sdp.p_codec_info_list);
			p_ctx_stream->local_sdp.p_codec_info_list = p_tmp_codec_info_list;
		}
		p_commited_stream_list = p_commited_stream_list->p_next;
	}
		
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmReserveCodecRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmReserveCodecRm(
	mmCoreCallCtx_t   *p_call_context,
 	icf_stream_capab_st	*p_input_stream,
	icf_stream_capab_st	*p_output_stream
){
	icf_list_st			*p_input_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_output_codec = MM_NULL;
	icf_codec_attrib_st		*p_input_codec = MM_NULL;
	icf_uint16_t		dspId = 0;
	icf_uint16_t		check_1stcodec_ready = 0;
	mm_return_t             ret_val = MM_FAILURE;

	MM_PRINTF("\n\tMM >Enter %s \n",__FUNCTION__);

	p_input_codec_list = p_input_stream->p_codec_info_list;
 	while(MM_NULL != p_input_codec_list)
	{
		p_input_codec = p_input_codec_list->p_data;

		if(MM_SUCCESS == mmCheckCodecSupport(p_call_context, p_input_stream, p_input_codec))
		{
			if((0 == zyStrcmp(p_input_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT))
				&& (ICF_STREAM_TYPE_AUDIO == p_output_stream->stream_type))
			{	
				/* Don't need to check DSP resource */
				p_output_codec = zyMalloc(sizeof(icf_codec_attrib_st));
				zyMemcpy(p_output_codec, p_input_codec, sizeof(icf_codec_attrib_st));
				p_output_codec->p_encoded_attributes = NULL;
				mmCloneEncAttribList(p_call_context , p_input_codec->p_encoded_attributes,&(p_output_codec->p_encoded_attributes));				
				p_output_codec->bit_mask |=ICF_CODEC_ENC_ATTRIB_PRESENT ;
				mmAddNodeToList(&(p_output_stream->p_codec_info_list), p_output_codec);	
			}
			else
			{
				dspId = mmCheckDspCodecRm(p_call_context, p_input_codec->codec_num);

				if (MM_INVALID_DSP_ID != dspId)
				{
					if(0 == check_1stcodec_ready)
					{
						/* Use 1st codec to apply media_stream_param */
						if(MM_SUCCESS == 
							mmCheckMediaStreamParamSupport(p_call_context, 
																	p_input_codec,
																	p_input_stream->stream_type,
																	&p_input_stream->media_stream_param,
																	&p_output_stream->media_stream_param))
						{
							p_output_stream->bit_mask |= ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT;
						}
						check_1stcodec_ready = 1;
					}
					p_output_codec = zyMalloc(sizeof(icf_codec_attrib_st));
					zyMemcpy(p_output_codec, p_input_codec, sizeof(icf_codec_attrib_st));
					
					p_output_codec->p_encoded_attributes = NULL;
					mmCloneEncAttribList(p_call_context , p_input_codec->p_encoded_attributes,&(p_output_codec->p_encoded_attributes));
					p_output_codec->bit_mask |=ICF_CODEC_ENC_ATTRIB_PRESENT ;
					
					/* Reserved resource by allocating DSP pool */
					ret_val = mmReserveDspRm(p_call_context->call_id, 
												dspId, 
												p_output_stream, 
												p_output_codec);

					if(MM_SUCCESS == ret_val)
					{
						mmAddNodeToList(&(p_output_stream->p_codec_info_list), p_output_codec);	
						p_output_stream->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
					}
					else
					{
						zyFree(p_output_codec);
					}
				}
			}
		}

		p_input_codec_list = p_input_codec_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__,ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmRestoreCodecRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmRestoreCodecRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_t		input_stream_id
){
	icf_list_st			*p_ctx_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	mm_return_t             ret_val = MM_FAILURE;
	dspChan_t*		dspChan_p = MM_NULL;
	mmCtxStream_t *p_ctx_stream = MM_NULL;

	MM_PRINTF("\n\tMM >Enter %s \n",__FUNCTION__);

	if(MM_SUCCESS != mmFreeDspPoolByStreamId(
								p_call_context->call_id, 
								input_stream_id))
	{
		/* Caution: Assume rtp stream only has one DSP channel and contain on kind of codec 
		Otherwise below fuction will get big troubble*/
		p_ctx_stream = mmCtxGetStream(p_call_context, input_stream_id);

		if(MM_NULL != p_ctx_stream)
		{
			dspChan_p = dspPoolFindDspChanByStreamId(
							p_call_context->call_id, p_ctx_stream->stream_id);
			
			/* Check if there is a previous call has established */
			if(MM_NULL != dspChan_p)
			{
				p_ctx_codec_list = p_ctx_stream->local_sdp.p_codec_info_list;
				if (MM_NULL != p_ctx_codec_list)
				{
					p_ctx_codec = p_ctx_codec_list->p_data;

					if (dspPoolChannelUsage(dspChan_p->codec_num) > 
											dspPoolChannelUsage(p_ctx_codec->codec_num))
					{
						dspChan_p->codec_num = p_ctx_codec->codec_num;
					}
				}
				ret_val = MM_SUCCESS;
			}
		}	
	}
	else
	{
		ret_val = MM_SUCCESS;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__,ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmFillRejectedStream
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmFillRejectedStream(
	icf_stream_id_list_st	*p_output_stream_list,
	icf_stream_capab_st		*p_input_stream
){
	icf_uint16_t	idx=0;
	icf_uint16_t	count=0;
	mm_return_t	ret_val = MM_SUCCESS;
	
	count = p_output_stream_list->count;
	for(idx= 0; idx < count; idx++)
	{
		if(p_output_stream_list->stream_id[idx] == p_input_stream->stream_id)
		{
			ret_val = MM_FAILURE;
		}
	}

	if(MM_SUCCESS == ret_val)
	{
		p_output_stream_list->stream_id[count] = p_input_stream->stream_id;
		p_output_stream_list->count++;	
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__,ret_val);
	return MM_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmFillCommitedStream
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmFillCommitedStream(
	icf_stream_commit_st		*p_output_stream,
	icf_codec_attrib_st		*p_input_codec
){
	icf_uint8_t	count=0;

	count = p_output_stream->codec_count;
	
	p_output_stream->codec[count].codec_id = p_input_codec->codec_num;
	p_output_stream->codec[count].bit_mask |= ICF_CODEC_ID_CODEC_ID_PRESENT;
	
	zyStrcpy(p_output_stream->codec[count].codec_name.str, p_input_codec->codec_name.str);
	
	p_output_stream->codec[count].codec_name.str_len = zyStrlen(p_input_codec->codec_name.str);
	
	p_output_stream->codec[count].bit_mask |= ICF_CODEC_ID_CODEC_NAME_PRESENT;
	p_output_stream->codec_count++; 

	return MM_SUCCESS;
}
/*************************************************************************
*
*  FUNCTION NAME  : mmSetTransportMode
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmSetTransportMode(
	icf_sdp_stream_st		*p_local_sdp,
	icf_media_type_t		stream_type
){

	switch(stream_type)
	{
		case ICF_STREAM_TYPE_AUDIO:
		case ICF_STREAM_TYPE_VIDEO:
			p_local_sdp->trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;
			break;

		case ICF_STREAM_TYPE_FAX:
			p_local_sdp->trans_mode = ICF_TRANSPORT_MODE_UDPTL;
			break;
		default:
			p_local_sdp->trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;
			break;
	}
	
	return MM_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCheckDtmfCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCheckDtmfCodec(
	icf_list_st		*p_ctx_codec_list,
	icf_codec_attrib_st		**p_p_input_dtmf_codec
){
	icf_list_st		*p_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	mm_return_t		ret_val = MM_FAILURE;

	p_codec_list = p_ctx_codec_list;
	while(MM_NULL != p_codec_list)
	{
		p_ctx_codec = p_codec_list->p_data;
		if(0 == zyStrcasecmp(p_ctx_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT))
		{
			*p_p_input_dtmf_codec = p_ctx_codec;
			ret_val = MM_SUCCESS;
			break;
		}
		p_codec_list = p_codec_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmVldtReservedCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmVldtReservedCodec(
	icf_list_st		*p_reserved_codec_list
){
	icf_list_st		*p_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_output_codec = MM_NULL;
	mm_return_t		ret_val = MM_FAILURE;

	p_codec_list = p_reserved_codec_list;
	while(MM_NULL != p_codec_list)
	{
		p_output_codec = p_codec_list->p_data;
		if(0 != zyStrcasecmp(p_output_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT))
		{
			ret_val = MM_SUCCESS;
			break;
		}
		p_codec_list = p_codec_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCommitMatchedCodec
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmCommitMatchedCodec(
	icf_list_st		*p_ctx_codec_list,
	icf_codec_attrib_st		*p_input_codec
){
	icf_list_st		*p_codec_list = MM_NULL;
	icf_codec_attrib_st		*p_ctx_codec = MM_NULL;
	mm_return_t		ret_val = MM_FAILURE;
	

	p_codec_list = p_ctx_codec_list;
	while(MM_NULL != p_codec_list)
	{
		p_ctx_codec = p_codec_list->p_data;

		if(MM_SUCCESS == 
			mmIsMatchedCodec(p_input_codec, p_ctx_codec->codec_num, &p_ctx_codec->codec_name))
		{
			#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, treat telephone event as normal codec*/
			ret_val = MM_SUCCESS;
			break;
			#else
			if(0 != zyStrcasecmp(p_input_codec->codec_name.str, SDP_CODEC_NAME_TELEPHONE_EVENT))
			{
				ret_val = MM_SUCCESS;
				break;
			}
			#endif
		}

		p_codec_list = p_codec_list->p_next;
	}

#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, use input codec number and update context codec number*/
	/* update ctx codec number to input codec num*/
	if(MM_SUCCESS == ret_val){
		p_ctx_codec->codec_num = p_input_codec->codec_num;
		p_ctx_codec->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
	}

#else
	/* Assign our configured codec_num when peer codec_num is not presented */
	p_input_codec->codec_num = p_ctx_codec->codec_num;	
	p_input_codec->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
#endif	

	MM_PRINTF("\n\tMM >Exit %s, codec_num=%d  ret_val=%d",__FUNCTION__, p_input_codec->codec_num ,ret_val);
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmReleaseCodecRm
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmReleaseCodecRm(
        mmCoreCallCtx_t   *p_call_context
){
	icf_list_st *p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	p_ctx_stream_list = p_call_context->p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;
		
		mmDeleteCodecAttribList(&(p_ctx_stream->local_sdp.p_codec_info_list));
		mmDeleteCodecAttribList(&(p_ctx_stream->remote_sdp.p_codec_info_list));

		mmDeleteList(&p_ctx_stream->local_sdp.p_encoded_attributes);
		mmDeleteList(&p_ctx_stream->remote_sdp.p_encoded_attributes);
	
		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}	

	/* Release all stream reserved dsp channel */
	mmFreeDspPoolByCallId(p_call_context->call_id);
	
	mmDeleteList(&(p_call_context->p_stream_list));
	
	MM_PRINTF("\n\tMM >Exit %s ", __FUNCTION__);	
	return MM_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmReleaseCodecRm_by_stream_id
*
*  DESCRIPTION    : 
*                  
*
*  RETURNS:       : 
*************************************************************************/
mm_return_t mmReleaseCodecRm_by_stream_id(
        mmCoreCallCtx_t   *p_call_context,
        icf_stream_id_t	input_stream_id
){
	icf_list_st *p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	p_ctx_stream_list = p_call_context->p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;

		if (p_ctx_stream->stream_id == input_stream_id)
		{
			/* Release stream reserved dsp channel */
			mmFreeDspPoolByStreamId(p_call_context->call_id, input_stream_id);
			mmDeleteCodecAttribList(&(p_ctx_stream->local_sdp.p_codec_info_list));
			mmDeleteCodecAttribList(&(p_ctx_stream->remote_sdp.p_codec_info_list));

			mmDeleteList(&p_ctx_stream->local_sdp.p_encoded_attributes);
			mmDeleteList(&p_ctx_stream->remote_sdp.p_encoded_attributes);
			mmDeleteNodeFrmList(&(p_call_context->p_stream_list), p_ctx_stream);

			ret_val = MM_SUCCESS;
			break;
		}
		
		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}	

	MM_PRINTF("\n\tMM >Exit %s, input_stream_id=%d, ret_val=%d", __FUNCTION__, input_stream_id, ret_val);	
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCloneCodecInfoList
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCloneCodecInfoList(
	icf_list_st		*p_input_codec_list,
	icf_list_st		**p_p_output_codec_list
){
	icf_list_st			*p_codec_info_list = MM_NULL;
	icf_codec_attrib_st		*p_output_codec = MM_NULL;
	icf_codec_attrib_st		*p_input_codec = MM_NULL;
	icf_list_st	*p_encoded_params_list = MM_NULL;
	icf_string_st *p_string = MM_NULL;
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	p_codec_info_list = p_input_codec_list;
	while(MM_NULL != p_codec_info_list)
	{
		p_input_codec = p_codec_info_list->p_data;

		p_output_codec = zyMalloc(sizeof(icf_codec_attrib_st));
		zyMemcpy(p_output_codec, p_input_codec, sizeof(icf_codec_attrib_st));
		p_output_codec->p_encoded_attributes= MM_NULL;

		if (p_input_codec->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
		{
			p_encoded_params_list = p_input_codec->p_encoded_attributes;			
			while(MM_NULL != p_encoded_params_list)
			{
				p_string = (icf_string_st *)p_encoded_params_list->p_data;
				if(MM_NULL != p_string){
					mmAddMediaAttributes(&p_output_codec->p_encoded_attributes, p_string->str);					
				}
				

				p_output_codec->bit_mask |= ICF_CODEC_ENC_ATTRIB_PRESENT;
				
				p_encoded_params_list = p_encoded_params_list->p_next;
			}
		}

		mmAddNodeToList(p_p_output_codec_list, p_output_codec);

		p_codec_info_list = p_codec_info_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s \n", __FUNCTION__);

	return MM_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCloneEncAttribList
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCloneEncAttribList(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st			*p_input_enc_attri_list,
	icf_list_st			**p_p_dest_list
){
	icf_list_st		*p_enc_attrib_list = MM_NULL;
	icf_string_st	*p_string = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;

	p_enc_attrib_list = p_input_enc_attri_list;
	while(MM_NULL != p_enc_attrib_list)
	{
		p_string = zyMalloc(sizeof(icf_string_st));
		zyMemcpy(p_string, p_enc_attrib_list->p_data, sizeof(icf_string_st));
		MM_PRINTF("\n\tMM > %s, attribute> %s\n", __FUNCTION__, p_string->str);

		mmAddNodeToList(p_p_dest_list, p_string);
		
		p_enc_attrib_list = p_enc_attrib_list->p_next;

		ret_val = MM_SUCCESS;
	}

	/* To Remove Complier Warning */
	p_call_context = p_call_context;

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mm_check_media_stream_param
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCheckMediaStreamParamSupport(
	mmCoreCallCtx_t   *p_call_context,
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	stream_codec_type,
	union icf_media_stream_param_ut   *p_input_media,
	union icf_media_stream_param_ut   *p_output_media
){
	mm_return_t	ret_val = MM_FAILURE;
	icf_config_codec_attrib_st	*p_cfg_codec_attrib = MM_NULL;

	p_cfg_codec_attrib = mmGetSupportedCodec(p_call_context, p_input_codec, stream_codec_type);
	if(MM_NULL != p_cfg_codec_attrib)
	{
		ret_val = MM_FAILURE;
		switch(p_cfg_codec_attrib->codec_type)
		{
			case  ICF_CODEC_TYPE_AUDIO:
				ret_val = mmGetAudioMediaParams(p_cfg_codec_attrib, 
														&p_input_media->audio_profile, 
														&p_output_media->audio_profile);
				break;
			case  ICF_CODEC_TYPE_VIDEO:
				MM_PRINTF("\n\tMM > ICF_CODEC_TYPE_VIDEO not support");
				break;
			case  ICF_CODEC_TYPE_IMAGE:
				MM_PRINTF("\n\tMM > ICF_CODEC_TYPE_IMAGE support");
				/** this will cause T38 failed due to mm shall return success to support image codec 
				* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
				* cindy modify 20100927 */
				ret_val = MM_SUCCESS; 
				break;
			default:
				break;
		}
	}
	
#if 0
	/* parse media attributes ex: a=rtpmap, a=fmtp, need to implement */
	if(p_input_stream->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
	{
		MM_PRINTF("\n\tMM > Enter ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT");	
		if (MM_NULL != p_input_stream->p_encoded_attributes)
		{
			mmCloneEncAttribList(
				p_call_context,
				p_input_stream->p_encoded_attributes,
				&p_ctx_stream->remote_sdp.p_encoded_attributes);

			p_ctx_stream->remote_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
		}
	}
#endif

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmAddMediaAttributes
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmAddMediaAttributes(
	icf_list_st         **p_p_encoded_attributes,
	icf_uint8_t *media_attri_str
){
	icf_string_st	*p_string = MM_NULL;

	p_string = zyMalloc(sizeof(icf_string_st));
	zyStrcpy(p_string->str, media_attri_str);
	p_string->str_len = zyStrlen((p_string->str));

	mmAddNodeToList(p_p_encoded_attributes, p_string);
	
	return MM_SUCCESS;
}
/*************************************************************************
 *
 *  FUNCTION NAME  : mmIsStreamCommited
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmIsStreamCommited(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_capab_st		*p_input_stream,
	icf_stream_capab_st		**p_p_output_stream
){
	icf_list_st		*p_ctx_stream_list = MM_NULL;
	icf_stream_capab_st	*p_ctx_stream = MM_NULL;
	mm_return_t ret_val = MM_FAILURE;
	
	p_ctx_stream_list = p_call_context->p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;
		if (p_ctx_stream->stream_id == p_input_stream->stream_id) 
		{
			*p_p_output_stream = p_ctx_stream;
			ret_val = MM_SUCCESS;
			break;
		}

		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}

	return ret_val;	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmIsMatchedCallId
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmIsMatchedCallId(
        mmCoreCallCtx_t   *p_call_context,
        icf_call_id_t call_id
){
	mm_return_t ret_val = MM_FAILURE;
	
	if(p_call_context->call_id == call_id)
	{
		ret_val = MM_SUCCESS;
	}
	else
	{
		ret_val = MM_FAILURE;
	}

	return ret_val;	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmGetAudioCodecParams
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
 
mm_return_t mmGetAudioCodecParams(
	icf_config_audio_codec_param_st   *p_input_params,
	icf_audio_codec_param_st	*p_ouput_params
){
	if(p_input_params->bit_mask & ICF_AUDIO_CODEC_CHANNEL)
	{
		p_ouput_params->channels = p_input_params->channels;
		p_ouput_params->bit_mask |= ICF_AUDIO_CODEC_PARAM_CHANNEL;
	}

	if(p_input_params->bit_mask & ICF_AUDIO_CODEC_SAMPLING_RATE)
	{
		if(p_input_params->num_of_supp_sampling_rate > 0)
		{	/* Use first sample rate by default */
			p_ouput_params->sampling_rate= p_input_params->sampling_rate[0];
			p_ouput_params->bit_mask |= ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE;
		}
	}

	return MM_SUCCESS;	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmGetAudioMediaParams
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
 
mm_return_t mmGetAudioMediaParams(
	icf_config_codec_attrib_st	*p_cfg_codec_attrib,
	icf_audio_profile_st   *p_input_media,
	icf_audio_profile_st   *p_output_media
){
	icf_uint8_t	ptime_cnt = 0;
	mm_return_t ret_val = MM_FAILURE;

	if(p_cfg_codec_attrib->bitmask & ICF_CODEC_PARAM_PRESENT)
	{
		if(p_cfg_codec_attrib->codec_param.audio_param.bit_mask & ICF_AUDIO_CODEC_BANDWIDTH)
		{
			if(MM_NULL == p_input_media)
			{
				p_output_media->bandwidth = p_cfg_codec_attrib->codec_param.audio_param.bandwidth;
				p_output_media->bit_mask |= ICF_AUDIO_PROF_BNDWDTH_PRESENT;
			}
			else if(p_input_media->bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)
			{
				if(p_input_media->bandwidth <= p_cfg_codec_attrib->codec_param.audio_param.bandwidth)
				{
					p_output_media->bandwidth = p_input_media->bandwidth;
					p_output_media->bit_mask |= ICF_AUDIO_PROF_BNDWDTH_PRESENT;
				}
				else
				{
					MM_PRINTF("\n\tMM > peer bandwidth =%d doesn't support, use default ", p_input_media->bandwidth);
				}
			}
			else 
			{
				p_output_media->bandwidth = p_cfg_codec_attrib->codec_param.audio_param.bandwidth;
				p_output_media->bit_mask |= ICF_AUDIO_PROF_BNDWDTH_PRESENT;
			}
		}

		if(p_cfg_codec_attrib->codec_param.audio_param.bit_mask & ICF_AUDIO_CODEC_PTIME)
		{
			if(MM_NULL == p_input_media)
			{
				/*p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.ptime[0];*/
				p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.pref_ptime;
			}
			else if(p_input_media->bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)
			{
				ptime_cnt = p_cfg_codec_attrib->codec_param.audio_param.num_of_supported_ptime;
				if( (p_cfg_codec_attrib->codec_param.audio_param.ptime[0] <= p_input_media->pref_ptime)
						&& (p_input_media->pref_ptime >= p_cfg_codec_attrib->codec_param.audio_param.ptime[ptime_cnt]))
				{
					p_output_media->pref_ptime = p_input_media->pref_ptime;
					p_output_media->bit_mask |= ICF_AUDIO_PROF_PTIME_PRESENT;
				}
				else
				{
					/*p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.ptime[0];*/
					p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.pref_ptime;
					p_output_media->bit_mask |= ICF_AUDIO_PROF_PTIME_PRESENT;

					MM_PRINTF("\n\tMM > peer pref_ptime =%d doesn't support, use default ", p_input_media->pref_ptime);
				}
			}
			else
			{
				/*p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.ptime[0];*/
				p_output_media->pref_ptime = p_cfg_codec_attrib->codec_param.audio_param.pref_ptime;
				p_output_media->bit_mask |= ICF_AUDIO_PROF_PTIME_PRESENT;
			}
		}

		if(p_cfg_codec_attrib->codec_param.audio_param.bit_mask & ICF_AUDIO_CODEC_SILENCE_SUPPRESSION)
		{
			if(MM_NULL == p_input_media)
			{				
				p_output_media->silence_suppression = p_cfg_codec_attrib->codec_param.audio_param.silence_suppression;
				#if 1 /*Eason, only present silence suppression if enabled and get from config*/
				MM_PRINTF("silence suppression not present if not enabled, get from config\n");
				if(p_output_media->silence_suppression)		
				p_output_media->bit_mask |= ICF_AUDIO_PROF_SILN_SUPP_PRESENT;
				#endif
			}
			else if(p_input_media->bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)
			{				
				p_output_media->silence_suppression = p_input_media->silence_suppression;
				#if 1 /*Eason, present silence suppression if get from input*/
				MM_PRINTF("silence suppression present on/off, get from input\n");
				p_output_media->bit_mask |= ICF_AUDIO_PROF_SILN_SUPP_PRESENT;
				#endif
			}
			else 
			{
				p_output_media->silence_suppression = p_cfg_codec_attrib->codec_param.audio_param.silence_suppression;
			}
			#if 0
			p_output_media->bit_mask |= ICF_AUDIO_PROF_SILN_SUPP_PRESENT;
			#endif
		}
		else
		{
			MM_PRINTF("\n\tMM > silence_suppression not support");
		}
	}

	if(MM_NULL != p_input_media)
	{
		if(p_input_media->bit_mask & ICF_AUDIO_PROF_ECHO_CANC_PRESENT)
		{
			p_output_media->echo_canc = p_input_media->echo_canc;
			p_output_media->bit_mask |= ICF_AUDIO_PROF_ECHO_CANC_PRESENT;
		}
	}

	if(0 != p_output_media->bit_mask)
	{
		ret_val = MM_SUCCESS;
	}

	
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;
}
/*************************************************************************
 *
 *  FUNCTION NAME  : mmGetVideoCodecParams
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmGetVideoCodecParams(
	icf_config_video_codec_param_st   *p_input_params,
	icf_video_codec_param_st	*p_ouput_params
){
	if(p_input_params->bit_mask & ICF_VIDEO_CFG_CODEC_FRAME_RATE_PRESENT)
	{
		p_ouput_params->frame_size = p_input_params->frame_size;
		p_ouput_params->bit_mask |= ICF_VIDEO_CODEC_FRAMESIZE_PRESENT;
	}

	if(p_input_params->bit_mask & ICF_VIDEO_CFG_CODEC_CLOCK_RATE_PRESENT)
	{
		if(p_input_params->num_of_supp_clock_rate > 0)
		{	/* Use first clock rate by default */
			p_ouput_params->clock_rate = p_input_params->clock_rate;
			p_ouput_params->bit_mask |= ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT;
		}
	}
	
	return MM_SUCCESS;	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmMakeStreamFormDefaultCodec
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmMakeStreamFormDefaultCodec(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_capab_st	*p_tmp_stream
){
	icf_codec_attrib_st		*p_output_codec = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint8_t	count = 0;
	icf_cfg_codec_info_req_st	*p_cfg_codec_Info = MM_NULL;
	icf_list_st	*p_encoded_params_list = MM_NULL;
	MM_PRINTF("\n\tMM > Enter mmMakeStreamFormDefaultCodec");

	p_cfg_codec_Info = &p_call_context->p_mm_cfg_db->codec_info[p_call_context->line_id];
	for(count = 0; count < p_cfg_codec_Info->count; count++)
	{
		p_output_codec = zyMalloc(sizeof(icf_codec_attrib_st));

		zyStrcpy(p_output_codec->codec_name.str, 
			p_cfg_codec_Info->codec_supp[count].codec_name.str);

		p_output_codec->codec_name.str_len = 
			p_cfg_codec_Info->codec_supp[count].codec_name.str_len;

		if(p_cfg_codec_Info->codec_supp[count].bitmask & ICF_CODEC_NUM_PRESENT)
		{
			p_output_codec->codec_num = p_cfg_codec_Info->codec_supp[count].codec_num;
			p_output_codec->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
		}
		
		if(p_cfg_codec_Info->codec_supp[count].bitmask & ICF_CODEC_PARAM_PRESENT)
		{
			switch(p_cfg_codec_Info->codec_supp[count].codec_type)
			{
				case  ICF_CODEC_TYPE_AUDIO:
					mmGetAudioCodecParams(&p_cfg_codec_Info->codec_supp[count].codec_param.audio_param, 
						&p_output_codec->codec_params.audio_param);
					break;
				case  ICF_CODEC_TYPE_VIDEO:
					mmGetVideoCodecParams(&p_cfg_codec_Info->codec_supp[count].codec_param.video_param, 
						&p_output_codec->codec_params.video_param);
					break;
				case  ICF_CODEC_TYPE_IMAGE:
					zyMemcpy(&p_output_codec->codec_params.t38_param, 
						&p_cfg_codec_Info->codec_supp[count].codec_param.t38_param, sizeof(icf_config_t38_attrib_param_st));
					break;
				default:
					MM_PRINTF("\n\tMM > ICF_CODEC_PARAM_PRESENT, codec_type is not exist!! \n");
					break;
			}
			p_output_codec->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
		}

		if(p_cfg_codec_Info->codec_supp[count].bitmask & ICF_ENCODED_PARAM_PRESENT)
		{
			p_encoded_params_list = p_cfg_codec_Info->codec_supp[count].p_encoded_params;
			while(MM_NULL != p_encoded_params_list)
			{
				icf_string_st *p_string=(icf_string_st *)(p_encoded_params_list->p_data);
				if(MM_NULL != p_string){
					mmAddMediaAttributes(&p_output_codec->p_encoded_attributes, p_string->str);				
				}

				p_output_codec->bit_mask |= ICF_CODEC_ENC_ATTRIB_PRESENT;
				
				p_encoded_params_list = p_encoded_params_list->p_next;
			}
		}
		
		mmAddNodeToList(&(p_tmp_stream->p_codec_info_list), p_output_codec);

		ret_val = MM_SUCCESS;	
	}

	if(MM_SUCCESS == ret_val)
	{
		ret_val = MM_FAILURE;
		/* Use priority 1 codec to fill up media_stream_param */
		switch(p_cfg_codec_Info->codec_supp[0].codec_type)
		{
			case  ICF_CODEC_TYPE_AUDIO:
				if(MM_SUCCESS == mmGetAudioMediaParams(&p_cfg_codec_Info->codec_supp[0], 
															MM_NULL, 
															&p_tmp_stream->media_stream_param.audio_profile))
				{
					p_tmp_stream->bit_mask |= ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT;
					ret_val = MM_SUCCESS;
				}
				break;

			case  ICF_CODEC_TYPE_VIDEO:
				MM_PRINTF("\n\tMM > ICF_CODEC_TYPE_VIDEO not support");
				break;

			case  ICF_CODEC_TYPE_IMAGE:
				MM_PRINTF("\n\tMM > ICF_CODEC_TYPE_IMAGE not support");
				break;
			default:
				break;
		}
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d", __FUNCTION__, ret_val);
	return ret_val;	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmRestoreReservedStreamRm
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmRestoreReservedStreamRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st	*p_input_stream_list
){
	icf_list_st	*p_stream_list = MM_NULL;
	icf_stream_capab_st	*p_reserved_stream = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;

	p_stream_list = p_input_stream_list;
	while(MM_NULL != p_stream_list)
	{
		p_reserved_stream = p_stream_list->p_data;
		mmRestoreCodecRm(p_call_context, p_reserved_stream->stream_id);
		ret_val = MM_SUCCESS;
		
		p_stream_list= p_stream_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmRestoreRejectedStreamRm
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmRestoreRejectedStreamRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_list_st	*p_rejected_stream_list
){
	mm_return_t	ret_val = MM_FAILURE;
	icf_uint16_t	idx=0;
	icf_uint16_t	count=0;
	
	count =  p_rejected_stream_list->count;
	for(idx= 0; idx < count; idx++)
	{
		mmRestoreCodecRm(p_call_context, p_rejected_stream_list->stream_id[count]);
		ret_val = MM_SUCCESS;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCloneCtxStreamList
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCloneCtxStreamList(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st	*p_input_stream_list
){
	icf_list_st	*p_resv_stream_list = MM_NULL;
	icf_stream_capab_st	*p_reserved_stream = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	mm_return_t	ret_val = MM_FAILURE;
	
	p_resv_stream_list = p_input_stream_list;
	while(MM_NULL != p_resv_stream_list)
	{
		p_reserved_stream = p_resv_stream_list->p_data;

		p_ctx_stream = mmCtxGetStream(p_call_context, p_reserved_stream->stream_id);
		if(MM_NULL == p_ctx_stream)
		{
			p_ctx_stream = mmCtxCreateNewStream(p_call_context);
			p_ctx_stream->stream_id = p_reserved_stream->stream_id;
			p_ctx_stream->stream_type = p_reserved_stream->stream_type;
		}

		p_ctx_stream->qos_mark = p_reserved_stream->qos_mark;

		/*delete codec info list before clone from input stream*/

		mmDeleteCodecAttribList(&p_ctx_stream->local_sdp.p_codec_info_list);

		mmCloneCodecInfoList( 
			p_reserved_stream->p_codec_info_list, 
			&p_ctx_stream->local_sdp.p_codec_info_list);

		zyMemcpy(&p_ctx_stream->local_sdp.media_stream_param, 
				&p_reserved_stream->media_stream_param, sizeof(union icf_media_stream_param_ut));

		p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
		
		if (MM_NULL == p_ctx_stream->local_sdp.p_encoded_attributes)
		{					
			mmCloneEncAttribList(
				p_call_context,
				p_reserved_stream->p_encoded_attributes,
				&p_ctx_stream->local_sdp.p_encoded_attributes);

			p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
		}
		ret_val = MM_SUCCESS;

		p_resv_stream_list = p_resv_stream_list->p_next;
	}

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillGclApi
 *
 *  DESCRIPTION    :  ICF_GET_CODEC_LIST_REQ
 *  This API is invoked by the IP Phone Toolkit towards RM to obtain a list of available codec(s), 
 *  and reserve the appropriate codec(s) for the new call. The request contains a list of streams 
 *  with stream type for which codec list is solicited. The request may also contain a list of codecs 
 *  specified either by application or received in remote SDP. The request may also contain other 
 *  parameters like ptime, bandwidth, codec attributes etc. In response RM returns the available 
 *  codecs and reserve them for further use in the call
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillGclApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_get_codec_list_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_stream_capab_st	*p_input_stream = MM_NULL;
	icf_stream_capab_st	*p_output_stream = MM_NULL;
	icf_codec_attrib_st		*p_output_dtmf_codec = MM_NULL;
	icf_list_st		*p_input_stream_list = MM_NULL;
	icf_uint8_t media_attri_str[ICF_MAX_STR_LEN];

	icf_list_st *p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_get_codec_list_resp_st),
		ret_val,
		p_err);
	
	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_get_codec_list_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;

	/* set SIP line id here and check whether it's available */
	p_call_context->line_id = ((icf_get_codec_list_req_st*)p_input_param)->line_id;
	MM_PRINTF("line_id = %d\n", p_call_context->line_id);
	p_input_stream_list = ((icf_get_codec_list_req_st*)p_input_param)->p_stream_capab_list;
	while (MM_NULL != p_input_stream_list)
	{				
		/* stream capapbility*/	
		p_input_stream = p_input_stream_list->p_data;	
		if((ICF_STREAM_TYPE_AUDIO == p_input_stream->stream_type)
			|| (ICF_STREAM_TYPE_FAX == p_input_stream->stream_type)
		){	
			p_output_stream = zyMalloc(sizeof(icf_stream_capab_st));
			p_output_stream->stream_id = p_input_stream->stream_id;
			p_output_stream->stream_type = p_input_stream->stream_type;
			
			if(p_input_stream->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
			{
				MM_PRINTF("\n\tMM > Enter ICF_STREAM_CAPAB_CODEC_INFO_PRESENT\n");

				if (MM_NULL != p_input_stream->p_codec_info_list)
				{
					if(MM_SUCCESS != 
						mmReserveCodecRm(p_call_context, p_input_stream, p_output_stream))
					 {
						p_payload->error_cause = ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE;			
					 }
				}
			}
			else
			{
				/* In GCL req if there are no local codecs provided for particular stream, 
				    RM returns all codecs it can support on that particular stream */			
				mmMakeStreamFormDefaultCodec(p_call_context, p_input_stream);

				p_input_stream->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT ;
				
				if (MM_SUCCESS != 
					mmReserveCodecRm(p_call_context, p_input_stream, p_output_stream))
				{
					p_payload->error_cause = ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE;
				}
			}

			/* Validate reserved codec, telephone-event is not a validated codec */
			if(MM_SUCCESS == mmVldtReservedCodec(p_output_stream->p_codec_info_list))
			{
				/* Additional Media attributes */
				if((ICF_STREAM_TYPE_AUDIO == p_output_stream->stream_type)
					&& (MM_SUCCESS == mmCheckDtmfCodec(p_output_stream->p_codec_info_list, &p_output_dtmf_codec)))
				{
					#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection*/
					sprintf(media_attri_str, "fmtp:%d 0-15", p_output_dtmf_codec->codec_num);
					#else
					sprintf(media_attri_str, "fmtp:%d 0-15", ZYNOS_CODEC_RFC2833);
					#endif
					mmAddMediaAttributes(&p_output_stream->p_encoded_attributes, media_attri_str);
				}
				/*mmAddMediaAttributes(&p_output_stream->p_encoded_attributes, "sendrecv");*/ /*Fix a call hold, SDP include both "sendonly"&"sendrecv" */
				p_output_stream->bit_mask |= ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT;
				
				mmAddNodeToList(&(p_payload->p_reserved_stream_list), p_output_stream);
			}
			else	
			{						
				mmFillRejectedStream(
					&p_payload->rejected_stream_list, 
					p_input_stream);
				
				p_payload->bit_mask |= ICF_CODEC_LIST_REJECTED_STREAM_LIST;
			}

		}
		else
		{
			mmFillRejectedStream(
				&p_payload->rejected_stream_list, 
				p_input_stream);
			
			p_payload->bit_mask |= ICF_CODEC_LIST_REJECTED_STREAM_LIST;
		}

		p_input_stream_list = p_input_stream_list->p_next;
	}

	if((0 != p_payload->error_cause) && (MM_NULL == p_payload->p_reserved_stream_list))
	{
		p_payload->result = ICF_FAILURE;
	}

	if(ICF_FAILURE == 
		mmCoreFillHdrAndSend(p_msg,
			ICF_GET_CODEC_LIST_RESP,
			app_id,
			call_id,
			p_err) )

	{
		MM_PRINTF("\n\tMM > mmCoreFillHdrAndSend ERROR!!\n");
		ret_val = MM_FAILURE;
	}

	if(MM_SUCCESS == ret_val)
	{
		if(MM_NULL != p_payload->p_reserved_stream_list)
		{
			mmCloneCtxStreamList(p_call_context, p_payload->p_reserved_stream_list);
		}

		if(0 != p_payload->rejected_stream_list.count)
		{
			mmRestoreRejectedStreamRm(p_call_context, &p_payload->rejected_stream_list);
		}
		
		if(0 != p_payload->error_cause)
		{


			p_ctx_stream_list = p_call_context->p_stream_list;
				p_ctx_stream_list = p_call_context->p_stream_list;
			while(MM_NULL != p_ctx_stream_list)
			{
				p_ctx_stream = p_ctx_stream_list->p_data;
				rtpClose(p_call_context->call_id, p_ctx_stream->stream_id, 
											p_ctx_stream->stream_type);
	
				p_ctx_stream_list = p_ctx_stream_list->p_next;
			}

			mmCoreRemoveCallContext(call_id);
		}
		else
		{
			mmDumpsCtxTrace(p_call_context);
		}
	}
	else
	{
		/* If we can't get GCL succ from IPTK, restore all of resources we allocated or changed */
		if(MM_NULL != p_payload->p_reserved_stream_list)
		{
			mmRestoreReservedStreamRm(p_call_context, p_payload->p_reserved_stream_list);
		}
		
		if(0 != p_payload->rejected_stream_list.count)
		{
			mmRestoreRejectedStreamRm(p_call_context, &p_payload->rejected_stream_list);
		}

		mmCoreRemoveCallContext(call_id);
	}


	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/
	
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillGclApi
 *
 *  DESCRIPTION    :  ICF_CODEC_COMMIT_REQ
 *  This API is invoked by IP Phone Toolkit towards RM to commit a list of codec(s) returned 
 *  in the ICF_GET_CODEC_LIST_RESP API. RM shall return the codecs which will be used in 
 *  the call and frees the other codecs reserved by API ICF_GET_CODEC_LIST_REQ earlier                  
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreCommitCodecApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_codec_commit_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_stream_capab_st		*p_input_stream = MM_NULL;
	icf_stream_commit_st		*p_commited_stream = MM_NULL;
	icf_list_st		*p_input_stream_list = MM_NULL;
	icf_list_st		*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	icf_list_st 			*p_commited_stream_list = MM_NULL;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_codec_commit_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_codec_commit_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;

	p_input_stream_list = ((icf_codec_commit_req_st*)p_input_param)->p_stream_commit_list;
	while (MM_NULL != p_input_stream_list)
	{	/* stream capapbility*/	
		p_input_stream = p_input_stream_list->p_data;

		if((ICF_STREAM_TYPE_AUDIO == p_input_stream->stream_type)
			|| (ICF_STREAM_TYPE_FAX == p_input_stream->stream_type)
		){
			p_ctx_stream_list = p_call_context->p_stream_list;
			while(MM_NULL != p_ctx_stream_list)
			{
				p_ctx_stream = p_ctx_stream_list->p_data;
				if (p_ctx_stream->stream_id == p_input_stream->stream_id) 
				{
					p_commited_stream = zyMalloc(sizeof(icf_stream_commit_st));
					if( NULL == p_commited_stream) {
						zyFree((icf_void_t*)p_msg);
						return MM_FAILURE;
					}
					p_commited_stream->stream_id = p_ctx_stream->stream_id;

					if(p_input_stream->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
					{	/* codec capapbility*/
						if (MM_SUCCESS == 
								mmCtxCommitCodec(
									p_input_stream->p_codec_info_list, 
									p_commited_stream,
									p_ctx_stream))
						{						
							mmAddNodeToList(
								&(p_payload->p_commited_stream_list), 
								p_commited_stream);
						}
						else
						{
							mmFillRejectedStream( 
								&p_payload->rejected_stream_list, 
								p_input_stream);
							
							p_payload->bit_mask |= ICF_CODEC_LIST_REJECTED_STREAM_LIST; 
						}
					}
				}

				p_ctx_stream_list = p_ctx_stream_list->p_next;
			}
		}
		else
		{
			mmFillRejectedStream( 
				&p_payload->rejected_stream_list, 
				p_input_stream);
			
			p_payload->bit_mask |= ICF_CODEC_LIST_REJECTED_STREAM_LIST; 
		}
		
		p_input_stream_list = p_input_stream_list->p_next;
	}

	if(0 != p_payload->error_cause)
	{
		p_payload->result = ICF_FAILURE;
	}


	if(ICF_FAILURE ==
		mmCoreFillHdrAndSend(p_msg,
			ICF_CODEC_COMMIT_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}

	

	if(MM_SUCCESS == ret_val)
	{	
		/* Update call context p_codec_info_list on each stream */
		mmCtxCommitStreamList(p_call_context, p_payload->p_commited_stream_list);

		/* Update DSP pool resource */
		ret_val = MM_SUCCESS;
		p_commited_stream_list = p_payload->p_commited_stream_list;
		while(MM_NULL != p_commited_stream_list)
		{
			p_commited_stream = p_commited_stream_list->p_data;

			if(MM_SUCCESS != mmCommitDspRm(p_call_context, p_commited_stream))
			{
				ret_val = MM_FAILURE;
			}
			p_commited_stream_list = p_commited_stream_list->p_next;
		}
	}

	mmDumpsCtxTrace(p_call_context);

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/
	MM_PRINTF("\n\tMM > mmCoreCommitCodecApi, ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreRelResApi
 *
 *  DESCRIPTION    :  ICF_REL_RES_REQ
 *  This API is invoked by IP Phone Toolkit towards RM to release the resources allocated for 
 *  the call. RM frees the codec(s) allocated for the call for use in other calls. The API can specify 
 *  releasing of resources for a specific stream only. IP Phone Toolkit does not expect a response 
 *  for this API from RM.If stream count is 0 in icf_stream_id_list_st, then all streams of the call 
 *  should be released                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreRelResApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_stream_id_list_st			*p_input_stream_list = MM_NULL;
	icf_uint8_t			count = 0;

	/* To Remove Complier Warning */
	app_id = app_id;
	call_id = call_id;
	p_err = p_err;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);
	
	p_input_stream_list = &((icf_rel_res_req_st*)p_input_param)->stream_list;

	if (0 == p_input_stream_list->count)
	{		
		/* If stream count is 0 in icf_stream_id_list_st, 
		then all streams of the call should be released */
		if(MM_SUCCESS == mmReleaseCodecRm(p_call_context))
		{	
			p_call_context->stream_count = 0;
		}
	}
	else
	{ 
		for ( count = 0; count < p_input_stream_list->count; count++)
		{
			if(MM_SUCCESS == mmReleaseCodecRm_by_stream_id(p_call_context, p_input_stream_list->stream_id[count]))
			{
				p_call_context->stream_count--;
			}
		}
	}

	mmDumpsCtxTrace(p_call_context);
	MM_PRINTF("\n\tMM > mmCoreRelResApi  Succ\n");	
	return MM_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillOmcApi
 *
 *  DESCRIPTION    :  ICF_OPEN_MEDIA_CHANNEL_REQ/ICF_OPEN_CHANNEL_REQ
 *  IP Phone Toolkit invokes this API towards MM to request MM to allocate ports for RTP and 
 *  RTCP channels in the call. IP Phone Toolkit will provide the list of streams and stream type 
 *  (audio, video, TBCP) for which ports need to be allocated. MM should allocate free ports for 
 *  RTP and RTCP. MM should also open UDP sockets on the ports, so that lateron IP Phone 
 *  Toolkit can request MM to send/receive non RTP/non RTCP UDP data on the socket                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillOmcApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_open_media_channel_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_list_st				*p_input_channel_list = MM_NULL;
	icf_channel_st			*p_input_channel = MM_NULL;
	icf_channel_addr_st 	*p_output_channel = MM_NULL;
	icf_list_st				*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;	
	
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);
	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_open_media_channel_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_open_media_channel_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;

	p_input_channel_list = ((icf_open_media_channel_req_st*)p_input_param)->p_channel_list;
	while (MM_NULL != p_input_channel_list)
	{
		p_input_channel = p_input_channel_list->p_data;
		
		p_ctx_stream_list = p_call_context->p_stream_list;
		while(MM_NULL != p_ctx_stream_list)
		{
			p_ctx_stream = p_ctx_stream_list->p_data;
			if(p_ctx_stream->stream_id == p_input_channel->stream_id)
			{
				p_ctx_stream->stream_type = p_input_channel->stream_type;
				p_ctx_stream->stream_id = p_input_channel->stream_id;

				/* Use Configuration Self IP */
				mmCtxSetSelfIp(p_call_context, p_ctx_stream);

				if (MM_SUCCESS == rtpOpen(call_id, p_call_context->line_id ,p_ctx_stream))
				{
					p_output_channel = zyMalloc(sizeof(icf_channel_addr_st));
					p_output_channel->stream_type = p_ctx_stream->stream_type;
					p_output_channel->stream_id = p_ctx_stream->stream_id;
					zyMemcpy(&p_output_channel->stream_address, 
						&p_ctx_stream->local_sdp.stream_address, sizeof(icf_transport_address_st));

					mmAddNodeToList(&(p_payload->p_channel_addr_list), p_output_channel);
				}
				else
				{
					zyMemset(&p_ctx_stream->local_sdp.stream_address, 0, sizeof(icf_transport_address_st));
					
					p_payload->error_cause = ICF_CAUSE_TRANSPORT_LAYER_FAILURE;
				}
			}
			p_ctx_stream_list = p_ctx_stream_list->p_next;
		}

		p_input_channel_list = p_input_channel_list->p_next;
	}

	if(0 != p_payload->error_cause)
	{
		p_payload->result = ICF_FAILURE;
	}

	mmDumpsCtxTrace(p_call_context);

	if(ICF_FAILURE == 
		mmCoreFillHdrAndSend(p_msg,
			ICF_OPEN_MEDIA_CHANNEL_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/

	MM_PRINTF("\n\tMM >Exit %s ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillCmcApi
 *
 *  DESCRIPTION    :  ICF_CLOSE_MEDIA_CHANNEL_REQ
 *  This API will be sent by IP Phone Toolkit to MM to close sockets for one or more than 
 *  one media channels for a existing call. MM shall close a socket for each channel ie type 
 *  of stream and ip, port combination.Once this is done it is no longer possible to send/receive 
 *  any packets on these channels                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillCmcApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_list_st				*p_input_channel_list = MM_NULL;
	icf_channel_st			*p_input_channel = MM_NULL;
	icf_list_st				*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;	
	
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__); 

	/* To Remove Complier Warning */
	app_id = app_id;
	call_id = call_id;
	p_err = p_err;
	
	p_input_channel_list = ((icf_close_media_channel_req_st*)p_input_param)->p_channel_list;
	while(MM_NULL != p_input_channel_list)
	{
		p_input_channel = p_input_channel_list->p_data;

		p_ctx_stream_list = p_call_context->p_stream_list;
		while(MM_NULL != p_ctx_stream_list)
		{
			p_ctx_stream = p_ctx_stream_list->p_data;

			if((p_input_channel->stream_id == p_ctx_stream->stream_id)
				&& (p_input_channel->stream_type == p_ctx_stream->stream_type))
			{
				if(MM_SUCCESS == rtpClose(call_id, p_input_channel->stream_id, p_input_channel->stream_type))
				{	
					zyMemset(&p_ctx_stream->local_sdp.stream_address,0,sizeof(icf_transport_address_st));	
				}
				else
				{
					ret_val = MM_FAILURE;
				}
			}
		
			p_ctx_stream_list = p_ctx_stream_list->p_next;
		}

		p_input_channel_list = p_input_channel_list->p_next;
	}


	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val); 
	return ret_val;
}
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillCmsApi
 *
 *  DESCRIPTION    :  ICF_CREATE_MEDIA_SESSION_REQ
 *  This API is sent by IP Phone Toolkit to MM to create a media session for a new call. 
 *  The API contains local media parameters such as the codec(s), codec attributes and mode 
 *  for each media stream in the session. Also the API includes the local IP and port for RTP 
 *  and RTCP for each stream. The API also contains remote media parameters such as the 
 *  remote codec(s), remote RTP and RTCP port and IP address. Some of these parameters 
 *  are optional, depending upon the parameters received from remote peer in SDP.
 * 
 *  The API is enhanced to handle multiple streams in the same request. Each stream will have 
 *  information for both the local and remote media parameters for that stream.                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillCmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_create_media_session_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_req_stream_st		*p_input_stream = MM_NULL;
	icf_resp_stream_st		*p_output_stream = MM_NULL;
	icf_list_st		*p_input_stream_list = MM_NULL;	
	icf_list_st					*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;	
	icf_codec_attrib_st		*p_input_codec = MM_NULL;
//Ssu-Ying, support Multiple Profile
	#if SRTP_SUPPORT
	icf_cfg_set_port_range_req_st *p_mm_port_range_cfg_db = MM_NULL;
	icf_line_id_t SipAccountId = -1;
	#endif
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);
	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_create_media_session_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}
//Ssu-Ying, support Multiple Profile
#if SRTP_SUPPORT
	if(((icf_create_media_session_req_st*)p_input_param)->bit_mask & ICF_LINE_ID_PRESENT){
		SipAccountId = ((icf_create_media_session_req_st*)p_input_param)->line_id;
		MM_PRINTF("\n\tMM > SipAccountId = %d\n", SipAccountId);
	}
#endif
	p_payload = (icf_create_media_session_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;

	p_input_stream_list = ((icf_create_media_session_req_st*)p_input_param)->p_stream_list_data;
	while (MM_NULL != p_input_stream_list)
	{
		p_input_stream = p_input_stream_list->p_data;
		MM_PRINTF("\n\tMM > input stream_id = %d, stream_type = %d\n", p_input_stream->stream_id, p_input_stream->stream_type);	
		p_ctx_stream_list = p_call_context->p_stream_list;
		while(MM_NULL != p_ctx_stream_list)
		{
			p_ctx_stream = p_ctx_stream_list->p_data;
			if((p_ctx_stream->stream_id == p_input_stream->stream_id)
				&& (p_ctx_stream->stream_type == p_input_stream->stream_type))
			{
				MM_PRINTF("\n\tMM > stream_id = %d, stream_type = %d\n", p_ctx_stream->stream_id, p_ctx_stream->stream_type);
				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT\n");

					/*delete codec info list before clone from input stream*/
					mmDeleteCodecAttribList(&p_ctx_stream->remote_sdp.p_codec_info_list);					

					/*delete p_encoded_attributes list before clone from input stream*/
					mmDeleteList(&p_ctx_stream->remote_sdp.p_encoded_attributes);
					
					zyMemcpy(&p_ctx_stream->remote_sdp, 
							&p_input_stream->remote_sdp, sizeof(icf_sdp_stream_st));

					p_ctx_stream->remote_sdp.bit_mask = 0;
					p_ctx_stream->remote_sdp.p_codec_info_list = MM_NULL;
					p_ctx_stream->remote_sdp.p_encoded_attributes = MM_NULL;					
			
				 	mmCloneCodecInfoList(
							p_input_stream->remote_sdp.p_codec_info_list,
							&p_ctx_stream->remote_sdp.p_codec_info_list);
					
					if(p_input_stream->remote_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
					{
						if(MM_NULL != p_input_stream->remote_sdp.p_encoded_attributes)
						{						
							mmCloneEncAttribList(
								p_call_context,
								p_input_stream->remote_sdp.p_encoded_attributes,
								&p_ctx_stream->remote_sdp.p_encoded_attributes);
						
							p_ctx_stream->remote_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
						}			
					}

					if(MM_NULL != p_input_stream->remote_sdp.p_codec_info_list)
					{
						/* Use 1st codec to apply media_stream_param */
						p_input_codec = p_input_stream->remote_sdp.p_codec_info_list->p_data;
						if(MM_SUCCESS == 
							mmCheckMediaStreamParamSupport(p_call_context, 
																	p_input_codec,
																	p_input_stream->stream_type,
																	&p_input_stream->remote_sdp.media_stream_param,
																	&p_ctx_stream->local_sdp.media_stream_param))
						{
							p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
						}
					}
					p_ctx_stream->remote_sdp.trans_mode= p_input_stream->remote_sdp.trans_mode;
					MM_PRINTF("\n\t MM > input stream remotesdp trans_mode = %d\n", p_ctx_stream->remote_sdp.trans_mode);
				}

				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT)
				{
					/*delete codec info list before clone from input stream*/

					mmDeleteCodecAttribList(&p_ctx_stream->local_sdp.p_codec_info_list);					

					/*delete p_encoded_attributes list before clone from input stream*/
					mmDeleteList(&p_ctx_stream->local_sdp.p_encoded_attributes);
					
					zyMemcpy(&p_ctx_stream->local_sdp, 
							&p_input_stream->local_sdp, sizeof(icf_sdp_stream_st));

					p_ctx_stream->local_sdp.bit_mask = 0;
					p_ctx_stream->local_sdp.p_codec_info_list = MM_NULL;
					p_ctx_stream->local_sdp.p_encoded_attributes = MM_NULL;
				
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_LOCAL_MEDIA_PROFILE_PRESENT\n");		

				 	mmCloneCodecInfoList(
							p_input_stream->local_sdp.p_codec_info_list,
							&p_ctx_stream->local_sdp.p_codec_info_list);
					
					if(p_input_stream->local_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
					{
						if(MM_NULL != p_input_stream->local_sdp.p_encoded_attributes)
						{
							mmCloneEncAttribList(
								p_call_context,
								p_input_stream->local_sdp.p_encoded_attributes,
								&p_ctx_stream->local_sdp.p_encoded_attributes);
						
							p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;	
						}		
					}
					#if SRTP_SUPPORT
					if(p_input_stream->stream_type == ICF_STREAM_TYPE_FAX)
					{
						mmSetTransportMode(&p_ctx_stream->local_sdp, p_input_stream->stream_type); 
					}
					else{
						if((p_input_stream->bit_mask & ICF_CREATE_MEDIA_REMOTE_SDP_PRESENT) && 
							(p_input_stream->remote_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_AVP)){
							p_ctx_stream->local_sdp.trans_mode= ICF_TRANSPORT_MODE_RTP_AVP;
							MM_PRINTF("\n\t MM > remote trans_mode is RTP_AVP, local sdp use RTP_AVP anyway\n");
						}
						else{
							p_ctx_stream->local_sdp.trans_mode=p_input_stream->local_sdp.trans_mode;
						}
						MM_PRINTF("\n\t MM > input stream localsdp trans_mode = %d\n", p_input_stream->local_sdp.trans_mode);					
						MM_PRINTF("\n\t MM > p_ctx_stream localsdp trans_mode = %d\n", p_ctx_stream->local_sdp.trans_mode);

						if(p_ctx_stream->local_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP){
	                                        //Ssu-Ying, support Multiple Profile
							p_mm_port_range_cfg_db = &gMmCfgDb_t.port_range[SipAccountId];
							MM_PRINTF("p_input_stream->local_sdp.trans_mode = %d\n", p_input_stream->local_sdp.trans_mode);
							MM_PRINTF("p_mm_port_range_cfg_db->srtp_encryption_key_size = %d\n",p_mm_port_range_cfg_db->srtp_encryption_key_size);						
							srtpAddCryptoAtrribute(call_id, p_ctx_stream, p_mm_port_range_cfg_db->srtp_encryption_key_size, p_mm_port_range_cfg_db->srtp_crypto_suite_name);							
							
						}
					}
					
					#else
					mmSetTransportMode(&p_ctx_stream->local_sdp, p_input_stream->stream_type);
					#endif
				}
				
				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_STREAM_MODE_PRESENT)
				{
					MM_PRINTF("input media_mode %d\n", p_input_stream->mode);
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_STREAM_MODE_PRESENT\n");
					p_ctx_stream->media_mode = p_input_stream->mode;
				}

				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_QOS_PARAMS_PRESENT\n");
					p_ctx_stream->qos_mark = p_input_stream->qos_mark;
				}

				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_RTP_DETECTION_PRESENT\n");
					p_ctx_stream->rtp_detection_reqd = p_input_stream->rtp_detection_reqd;
				}

				if (p_input_stream->bit_mask & ICF_CREATE_MEDIA_ECHO_CAN_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_CREATE_MEDIA_ECHO_CAN_PRESENT\n");
					p_ctx_stream->echo_cancellation = p_input_stream->echo_cancellation;
				}
				
				/* RTP */
				if(MM_SUCCESS == rtpModify(call_id, p_ctx_stream))
				{
					if(MM_SUCCESS == rtpConnect(call_id, p_ctx_stream))
					{
						#if SRTP_SUPPORT
						if(MM_FAILURE == srtpCreateSession(call_id, p_ctx_stream)){
							MM_PRINTF("\n\tMM > srtpCreateSession failed, rollback to RTP_AVP\n");
							if(p_ctx_stream->local_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP){
								p_ctx_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;
							}
						}
						#endif
						
						p_output_stream = zyMalloc(sizeof(icf_resp_stream_st));
						p_output_stream->stream_id = p_ctx_stream->stream_id;
						p_output_stream->stream_type = p_ctx_stream->stream_type;
						
						zyMemcpy(&p_output_stream->local_sdp, 
								&p_ctx_stream->local_sdp, sizeof(icf_sdp_stream_st));

						p_output_stream->local_sdp.p_codec_info_list = MM_NULL;
						
						mmCloneCodecInfoList(
								p_ctx_stream->local_sdp.p_codec_info_list,
								&p_output_stream->local_sdp.p_codec_info_list);

						p_output_stream->local_sdp.p_encoded_attributes = MM_NULL;
						
						mmCloneEncAttribList(
								p_call_context,
								p_ctx_stream->local_sdp.p_encoded_attributes,
								&p_output_stream->local_sdp.p_encoded_attributes);
						p_output_stream->local_sdp.bit_mask|=ICF_SDP_STREAM_ENCPARAMS_PRESENT;
						
						mmAddNodeToList(&(p_payload->p_accepted_stream_list), p_output_stream);

					}
					else
					{
						p_payload->rejected_stream_list.stream_id
							[p_payload->rejected_stream_list.count] = p_input_stream->stream_id;
						MM_PRINTF("\n\tMM > reject stream_id = %d\n", p_input_stream->stream_id);
						p_payload->rejected_stream_list.count++;	
					}
				}
				else
				{
					p_payload->rejected_stream_list.stream_id
						[p_payload->rejected_stream_list.count] = p_input_stream->stream_id;
					MM_PRINTF("\n\tMM > reject stream_id = %d\n", p_input_stream->stream_id);
					p_payload->rejected_stream_list.count++;	
				}
			}

			p_ctx_stream_list = p_ctx_stream_list->p_next;
		}

		p_input_stream_list = p_input_stream_list->p_next;
	}


	if(0 != p_payload->error_cause)
	{
		p_payload->result = ICF_FAILURE;
	}

	mmDumpsCtxTrace(p_call_context);

	if(ICF_FAILURE ==
		mmCoreFillHdrAndSend(p_msg,
			ICF_CREATE_MEDIA_SESSION_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}
	
	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/
	
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillMmsApi
 *
 *  DESCRIPTION    :  ICF_MODIFY_MEDIA_SESSION_REQ
 *  This API is sent by IP Phone Toolkit to MM to modify the media session parameters for an 
 *  existing call. The call is identified by the call_id parameter present in the API header. The API 
 *  may request that the following parameters be changed:
 *    - Media mode
 *    - Local media codec(s) and/or attributes
 *    - Remote media RTP/RTCP address
 *    - Remote media codec(s) and/or attributes                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillMmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_modify_media_session_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_modify_stream_st		*p_input_stream = MM_NULL;
	icf_resp_stream_st			*p_output_stream = MM_NULL;
	icf_list_st					*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;	
	icf_list_st		*p_input_stream_list = MM_NULL;
	icf_codec_attrib_st		*p_input_codec = MM_NULL;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);	
	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_modify_media_session_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_modify_media_session_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;


	p_input_stream_list = ((icf_modify_media_session_req_st*)p_input_param)->p_stream_data_list;
	while(MM_NULL != p_input_stream_list)
	{
		p_input_stream = p_input_stream_list->p_data;	
		MM_PRINTF("\n\tMM > input stream_id = %d, stream_type = %d\n", p_input_stream->stream_id, p_input_stream->stream_type);	
		p_ctx_stream_list = p_call_context->p_stream_list;
		while(MM_NULL != p_ctx_stream_list)
		{
			p_ctx_stream = p_ctx_stream_list->p_data;
			if((p_ctx_stream->stream_id == p_input_stream->stream_id)
				&& (p_ctx_stream->stream_type == p_input_stream->stream_type))
			{
				MM_PRINTF("\n\tMM > stream_id = %d, stream_type = %d\n", p_ctx_stream->stream_id, p_ctx_stream->stream_type);
				if (p_input_stream->bit_mask & ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_MODIFY_LOCAL_SDP_STREAM_PRESENT\n");
					
					/*delete codec info list before clone from input stream*/

					mmDeleteCodecAttribList(&p_ctx_stream->local_sdp.p_codec_info_list);					

					/*delete p_encoded_attributes list before clone from input stream*/
					mmDeleteList(&p_ctx_stream->local_sdp.p_encoded_attributes);
					
					MM_PRINTF("\n\tMM > input local_sdp trans_mode = %d\n", p_input_stream->sdp_stream.trans_mode);
					
					zyMemcpy(&p_ctx_stream->local_sdp, 
							&p_input_stream->sdp_stream, sizeof(icf_sdp_stream_st));

					p_ctx_stream->local_sdp.p_codec_info_list = MM_NULL;
					p_ctx_stream->local_sdp.p_encoded_attributes = MM_NULL;

					mmCloneCodecInfoList(
							p_input_stream->sdp_stream.p_codec_info_list,
							&p_ctx_stream->local_sdp.p_codec_info_list);

					if(p_input_stream->sdp_stream.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
					{
						if(MM_NULL != p_input_stream->sdp_stream.p_encoded_attributes)
						{
							mmCloneEncAttribList(
								p_call_context,
								p_input_stream->sdp_stream.p_encoded_attributes,
								&p_ctx_stream->local_sdp.p_encoded_attributes);
						
							p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
						}
					}
				}

				if (p_input_stream->bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT\n");
					
					/*delete codec info list before clone from input stream*/

					mmDeleteCodecAttribList(&p_ctx_stream->remote_sdp.p_codec_info_list);					

					/*delete p_encoded_attributes list before clone from input stream*/
					mmDeleteList(&p_ctx_stream->remote_sdp.p_encoded_attributes);
					
					MM_PRINTF("\n\tMM > input remote_sdp trans_mode = %d\n", p_input_stream->sdp_stream.trans_mode);					
					
					zyMemcpy(&p_ctx_stream->remote_sdp, 
							&p_input_stream->sdp_stream, sizeof(icf_sdp_stream_st));
					
					p_ctx_stream->remote_sdp.p_codec_info_list = MM_NULL;
					p_ctx_stream->remote_sdp.p_encoded_attributes = MM_NULL;

					mmCloneCodecInfoList(
							p_input_stream->sdp_stream.p_codec_info_list,
							&p_ctx_stream->remote_sdp.p_codec_info_list);

					if(p_input_stream->sdp_stream.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
					{
						if(MM_NULL != p_input_stream->sdp_stream.p_encoded_attributes)
						{
							mmCloneEncAttribList(
								p_call_context,
								p_input_stream->sdp_stream.p_encoded_attributes,
								&p_ctx_stream->remote_sdp.p_encoded_attributes);
						
							p_ctx_stream->remote_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
						}
					}

					if(MM_NULL != p_input_stream->sdp_stream.p_codec_info_list)
					{
						/* Use 1st codec to apply media_stream_param */
						p_input_codec = p_input_stream->sdp_stream.p_codec_info_list->p_data;
						if(MM_SUCCESS == 
							mmCheckMediaStreamParamSupport(p_call_context, 
																	p_input_codec,
																	p_input_stream->stream_type,
																	&p_input_stream->sdp_stream.media_stream_param,
																	&p_ctx_stream->local_sdp.media_stream_param))
						{
							p_ctx_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
						}
					}
				}

				if (p_input_stream->bit_mask & ICF_MODIFY_MEDIA_MODE_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_MODIFY_MEDIA_MODE_PRESENT\n");
					MM_PRINTF("input media_mode %d\n", p_input_stream->mode);

					/* Local call-hold need to close phyiscal DSP channel */
					if(!(p_input_stream->sdp_stream.bit_mask & ICF_MODIFY_REMOTE_SDP_STREAM_PRESENT))
					{
						/* Local call-hold: check if media mode will change from SEND_RECV to INACTIVE*/						
						if((ICF_MEDIA_MODE_SEND_RECV == p_ctx_stream->media_mode)
							&& (ICF_MEDIA_MODE_INACTIVE == p_input_stream->mode))
						{
							mmChangeDspPoolToReserved(p_call_context->call_id, p_ctx_stream);
							p_input_stream->mode = ICF_MEDIA_MODE_SEND_ONLY;	/* test - aircheng add for MOH*/
						}
					}				

					p_ctx_stream->media_mode = p_input_stream->mode;
					MM_PRINTF("media_mode change to %d\n", p_ctx_stream->media_mode);
				}

				if (p_input_stream->bit_mask & ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT)
				{
					MM_PRINTF("\n\tMM > ICF_MODIFY_MEDIA_ECHO_CAN_PRESENT\n");	
					p_ctx_stream->echo_cancellation = p_input_stream->mode;
				}

				if(MM_SUCCESS == rtpModify(call_id, p_ctx_stream))
				{
					if(MM_SUCCESS == rtpConnect(call_id, p_ctx_stream))
					{
						#if SRTP_SUPPORT
						if(MM_FAILURE == srtpCreateSession(call_id, p_ctx_stream)){
							MM_PRINTF("\n\tMM > srtpCreateSession failed, rollback to RTP_AVP\n");
							if(p_ctx_stream->local_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP){
								p_ctx_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;
							}
							if(p_ctx_stream->remote_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP){
								p_ctx_stream->remote_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;
							}
						}
						#endif
						p_output_stream = zyMalloc(sizeof(icf_resp_stream_st));
						p_output_stream->stream_id = p_ctx_stream->stream_id;
						p_output_stream->stream_type = p_ctx_stream->stream_type;

						zyMemcpy(&p_output_stream->local_sdp, 
												&p_ctx_stream->local_sdp, sizeof(icf_sdp_stream_st));	

						p_output_stream->local_sdp.p_codec_info_list = MM_NULL;
						p_output_stream->local_sdp.p_encoded_attributes = MM_NULL;

						mmCloneCodecInfoList(
								p_ctx_stream->local_sdp.p_codec_info_list,
								&p_output_stream->local_sdp.p_codec_info_list);

						if(p_ctx_stream->local_sdp.bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT)
						{
							if(MM_NULL != p_ctx_stream->local_sdp.p_encoded_attributes)
							{
								mmCloneEncAttribList(
									p_call_context,
									p_ctx_stream->local_sdp.p_encoded_attributes,
									&p_output_stream->local_sdp.p_encoded_attributes);
							
								p_output_stream->local_sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
							}
						}
						
						mmAddNodeToList(&(p_payload->p_accepted_stream_list), p_output_stream);	
					}
					else
					{
						p_payload->rejected_stream_list.stream_id
							[p_payload->rejected_stream_list.count] = p_input_stream->stream_id;
						MM_PRINTF("\n\tMM > reject stream_id = %d\n", p_input_stream->stream_id);
						
						p_payload->rejected_stream_list.count++;	
					}
					
				}
				else
				{
					p_payload->rejected_stream_list.stream_id
						[p_payload->rejected_stream_list.count] = p_input_stream->stream_id;
					MM_PRINTF("\n\tMM > reject stream_id = %d\n", p_input_stream->stream_id);
					p_payload->rejected_stream_list.count++;	
				}

			}
			p_ctx_stream_list = p_ctx_stream_list->p_next;
		}

		p_input_stream_list = p_input_stream_list->p_next;
	}

	
	if(0 != p_payload->error_cause)
	{
		p_payload->result = ICF_FAILURE;
	}
	else{
		MM_PRINTF("p_payload->error_cause = %d\n", p_payload->error_cause);
	}

	mmDumpsCtxTrace(p_call_context);

	if(ICF_FAILURE == 
		mmCoreFillHdrAndSend(p_msg,
			ICF_MODIFY_MEDIA_SESSION_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillMrmsApi
 *
 *  DESCRIPTION    :  ICF_MERGE_MEDIA_SESSION_REQ
 *  This API is sent by IP Phone Toolkit to MM to merge two pre-existing media sessions, 
 *  which are identified by their respective call IDs. This API is used in service calls when 
 *  more than two parties need to communicate at the same time. The mixing of audio 
 *  streams occurs at the local user equipment                  
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillMrmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_msg_resp_st	*p_payload = MM_NULL;
	icf_msg_st	*p_msg = MM_NULL;
	mm_return_t	ret_val = MM_SUCCESS;
	mmCoreCallCtxList_t	*p_merged_call_ctx_list = MM_NULL;
	icf_merge_media_session_req_st	*p_merge_media_session = MM_NULL;
	icf_call_id_t	merge_call_id = 0;
	icf_uint16_t	stream_count = 0;
	icf_uint16_t	idx = 0;
	
	MM_PRINTF("\n\tMM >Enter %s, call_id=%d\n", __FUNCTION__, call_id);
	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_msg_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_msg_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;


	p_merge_media_session = ((icf_merge_media_session_req_st*)p_input_param);
	merge_call_id = p_merge_media_session->merge_call_id;
	stream_count = p_merge_media_session->count;

	if (0 == stream_count)
	{
		zyFree((icf_void_t*)p_msg);
		return MM_FAILURE;
	}
	else
	{
		p_merged_call_ctx_list = mmCoreGetCallContext(p_call_context_list, merge_call_id);
		if(MM_NULL == p_merged_call_ctx_list)
		{
			p_payload->error_cause = ICF_CAUSE_INVALID_CALL_ID;
		}
		else
		{
			for(idx = 0; idx < stream_count; idx++)
			{
				ret_val = mmCreateDspConf(
							call_id, 
							merge_call_id, 
							p_merge_media_session->merge_stream_id[idx].stream_id,
							p_merge_media_session->merge_stream_id[idx].merge_stream_id);

				if(MM_FAILURE == ret_val)
				{
					p_payload->error_cause = ICF_CAUSE_INSUFFICIENT_RESOURCES;
					break;	/* return error once fail */
				}
			}
		}
	}

	if(0 != p_payload->error_cause)
	{
		for(idx = 0; idx < stream_count; idx++)
		{
			ret_val = mmDeleteDspConf(
						call_id, 
						merge_call_id, 
						p_merge_media_session->merge_stream_id[idx].stream_id,
						p_merge_media_session->merge_stream_id[idx].merge_stream_id);
		}

		p_payload->result = ICF_FAILURE;
	}

	mmDumpsCtxTrace(p_call_context);
	dspPoolDumpConfDspPool();
	
	if(ICF_FAILURE == 
		mmCoreFillHdrAndSend(p_msg,
			ICF_MERGE_MEDIA_SESSION_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/

	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillDmsApi
 *
 *  DESCRIPTION    :  ICF_DELETE_MEDIA_SESSION_REQ
 *  This API is sent by IP Phone Toolkit to MM to delete any/all the streams for an established 
 *  media session in a call. After receiving this API, MM deletes the media session and clears 
 *  its context. If stream count is 0 in icf_stream_id_list_st, then all streams of the call should 
 *  be deleted
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillDmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err)
{
	icf_msg_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_stream_id_list_st		*p_input_stream_id_list = MM_NULL;
	icf_uint16_t			count = 0;
	icf_list_st *p_ctx_stream_list = MM_NULL;
	mmCtxStream_t	*p_ctx_stream = MM_NULL;
	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);	
	/* get the memory for sending message to the ICF */
	MM_MSG_ALLOC(p_msg,
		sizeof(icf_msg_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_msg_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;
	p_payload->error_cause = 0;


	p_input_stream_id_list = &((icf_delete_media_session_req_st*)p_input_param)->stream_list;

	MM_PRINTF("\n\tMM > input_stream_id count=%d \n", p_input_stream_id_list->count);
	
	if (0 == p_input_stream_id_list->count)
	{	
		/* If stream count is 0 in icf_stream_id_list_st, 
		then all streams of the call should be deleted */
		p_ctx_stream_list = p_call_context->p_stream_list;
		while(MM_NULL != p_ctx_stream_list)
		{
			p_ctx_stream = p_ctx_stream_list->p_data;
			if(MM_SUCCESS == rtpClose(p_call_context->call_id, 
										p_ctx_stream->stream_id, 
										p_ctx_stream->stream_type))
			{
				zyMemset(&p_ctx_stream->local_sdp.stream_address,0,sizeof(icf_transport_address_st));	
			}
			else
			{
				p_payload->error_cause = ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION;
			}

			p_ctx_stream_list = p_ctx_stream_list->p_next;
		}
	}
	else
	{
		for (count = 0; count < p_input_stream_id_list->count; count++)
		{
			p_ctx_stream_list = p_call_context->p_stream_list;
			while(MM_NULL != p_ctx_stream_list)
			{
				p_ctx_stream = p_ctx_stream_list->p_data;
				if(p_ctx_stream->stream_id == p_input_stream_id_list->stream_id[count])
				{
					if(MM_SUCCESS == rtpClose(p_call_context->call_id, 
												p_input_stream_id_list->stream_id[count], 
												p_ctx_stream->stream_type))
					{
						#if SRTP_SUPPORT
						srtpDeleteSession(call_id, p_ctx_stream);
						#endif
						zyMemset(&p_ctx_stream->local_sdp.stream_address,0,sizeof(icf_transport_address_st));
					}
					else
					{
						p_payload->error_cause = ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION;
					}
				}
				
				p_ctx_stream_list = p_ctx_stream_list->p_next;
			}
		}
	}

	if(0 != p_payload->error_cause)
	{
		p_payload->result = ICF_FAILURE;
	}

	mmDumpsCtxTrace(p_call_context);

	if(ICF_FAILURE == 
		mmCoreFillHdrAndSend(p_msg,
			ICF_DELETE_MEDIA_SESSION_RESP,
			app_id,
			call_id,
			p_err))

	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/
	MM_PRINTF("\n\tMM >Exit %s, ret_val=%d \n", __FUNCTION__, ret_val);

	return ret_val;
}

//Ssu-Ying, support Multiple Profile
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillMsgRespApi
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillMsgRespApi(
        icf_api_id_t          api_id,
        icf_app_id_t         app_id,
        icf_call_id_t call_id,
        icf_line_id_t SipAccountId,
        icf_error_t    *p_err)
{
	icf_msg_resp_st                      *p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);	

	MM_MSG_ALLOC(p_msg,
		sizeof(icf_msg_resp_st),
		ret_val,
		p_err);

	if(MM_FAILURE == ret_val)
	{
		return MM_FAILURE;
	}

	p_payload = (icf_msg_resp_st*)(MM_MSG_GET_PAYLOAD(p_msg));
	p_payload->bit_mask = 0;
	p_payload->result = ICF_SUCCESS;

	if(MM_NULL != SipAccountId)
	{
		p_payload->bit_mask |= ICF_MSG_RESP_LINE_ID_PRESENT;
		MM_PRINTF(" mmCoreFillMsgRespApi> ENTER, SipAccountId %d \n", SipAccountId);
		p_payload->line_id = SipAccountId;
	}

	if(0 == *p_err)
	{
		p_payload->result = ICF_SUCCESS;
		p_payload->error_cause = 0;
	}
	else
	{
		p_payload->result = ICF_FAILURE;
		p_payload->error_cause = *p_err;
	}

	if(ICF_FAILURE== mmCoreFillHdrAndSend(p_msg,	api_id, app_id, call_id, p_err))
	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/

	MM_PRINTF("\n\tMM >Exit %s ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillHdrAndSend
 *
 *  DESCRIPTION    : This function will fill the API header and send it to 
 *                    to the IPTK
 *
 *  RETURNS          : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t 
mmCoreFillHdrAndSend(
        icf_msg_st            *p_msg, 
        icf_api_id_t          api_id,
        icf_app_id_t         app_id,
        icf_call_id_t         call_id,
        mm_error_t       *p_err)
{

	mm_return_t    ret_val = MM_SUCCESS;
	icf_uint8_t         *p_buffer = MM_NULL;
	icf_uint32_t        buf_len=0;
	icf_vldt_err_st     vldt_err;

	

	MM_MSG_FILL_HDR(p_msg,api_id,ICF_MODULE_MM,
	        ICF_MODULE_ICF,call_id,app_id);
	buf_len = MM_MSG_GET_PAYLOAD_LENGTH(p_msg);

	ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,p_err,&vldt_err);

	if( ret_val !=  MM_SUCCESS)
	{
	MM_PRINTF("\n\tMM > icf_encode_msg Fail *p_err=%d\n", *p_err);
		/* add the trace */
		return(MM_FAILURE);
	}

	MM_MSG_SEND(p_buffer,buf_len,ret_val,p_err);

	/* Error in sending */
	if(ret_val==MM_FAILURE)
	{
		MM_PRINTF("\n\tMM > MM_MSG_SEND Fail *p_err=%d\n", *p_err);
		icf_free_encoded_msg(p_buffer,p_err);  /* ICF - free message encoded */
		return(MM_FAILURE);
	}

	/* memory taken by this buffer needs to be freed after sending to IPTK
	*/

	icf_free_encoded_msg(p_buffer,p_err);   /* ICF - free encoded message*/


	MM_PRINTF("\n######################################################\n");
	MM_PRINTF("MM ->  IPTK : API id %d", api_id);
	MM_PRINTF("\n######################################################\n");

	return MM_SUCCESS;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreCallIdErrorMmi
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreCallIdErrorMmi(
        icf_call_id_t call_id,
        icf_api_id_t	api_id,
        icf_error_t    *p_err)
{
	icf_uint8_t	*p_payload = MM_NULL;
	icf_msg_st                                  *p_msg = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_api_id_t	return_api_id;

	MM_PRINTF("\n\tMM >Enter %s \n", __FUNCTION__);

	switch (api_id)
	{
		case ICF_GET_CODEC_LIST_REQ:	
			MM_MSG_ALLOC(p_msg, sizeof(icf_get_codec_list_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			
			p_payload = MM_MSG_GET_PAYLOAD(p_msg);
			((icf_get_codec_list_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_get_codec_list_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_CODEC_COMMIT_REQ:
			MM_MSG_ALLOC(p_msg, sizeof(icf_codec_commit_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			p_payload = (MM_MSG_GET_PAYLOAD(p_msg));
			((icf_codec_commit_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_codec_commit_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_OPEN_MEDIA_CHANNEL_REQ:
			MM_MSG_ALLOC(p_msg, sizeof(icf_open_media_channel_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			p_payload = (MM_MSG_GET_PAYLOAD(p_msg));
			((icf_open_media_channel_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_open_media_channel_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_CREATE_MEDIA_SESSION_REQ:
			MM_MSG_ALLOC(p_msg, sizeof(icf_create_media_session_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			p_payload = (MM_MSG_GET_PAYLOAD(p_msg));
			((icf_create_media_session_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_create_media_session_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_MODIFY_MEDIA_SESSION_REQ:
			MM_MSG_ALLOC(p_msg, sizeof(icf_modify_media_session_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			p_payload = (MM_MSG_GET_PAYLOAD(p_msg));
			((icf_modify_media_session_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_modify_media_session_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_DELETE_MEDIA_SESSION_REQ:
		case ICF_MERGE_MEDIA_SESSION_REQ:
			MM_MSG_ALLOC(p_msg, sizeof(icf_msg_resp_st), ret_val, p_err);
			if(MM_FAILURE == ret_val)
			{
				return MM_FAILURE;
			}
			p_payload = (MM_MSG_GET_PAYLOAD(p_msg));
			((icf_msg_resp_st*)p_payload)->result = ICF_FAILURE;
			((icf_msg_resp_st*)p_payload)->error_cause = ICF_CAUSE_INVALID_CALL_ID;
			break;
		case ICF_REL_RES_REQ:
		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		default:
			return MM_FAILURE;
	}

	/* Response API ID = Request API ID + 1 */
	return_api_id = api_id + 1;

	if(ICF_FAILURE == mmCoreFillHdrAndSend(p_msg,
			return_api_id,
			ICF_MODULE_ICF,
			call_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

	icf_free_decoded_msg(p_msg,p_err);   /* ICF - free decoded message*/

	MM_PRINTF("\n\tMM >Exit %s ret_val=%d\n", __FUNCTION__, ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreUnexpectedMsg
 *
 *  DESCRIPTION    :  ICF_MM_UNEXPECTED_MSG_IND
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreUnexpectedMsg(
	icf_msg_st *p_msg,
	icf_error_t    *p_err
){
	icf_mm_unexpected_msg_ind_st			*p_unexp_msg_payload = MM_NULL;

	/* To Remove Complier Warning */
	p_err = p_err;

	p_unexp_msg_payload = (icf_mm_unexpected_msg_ind_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));
	
	MM_PRINTF("\n\tMM >ICF_MM_UNEXPECTED_MSG_IND, api_id=%d,  error_cause=%d\n", 
		p_unexp_msg_payload->api_id, p_unexp_msg_payload->error_cause);
	
	return MM_SUCCESS;
}

