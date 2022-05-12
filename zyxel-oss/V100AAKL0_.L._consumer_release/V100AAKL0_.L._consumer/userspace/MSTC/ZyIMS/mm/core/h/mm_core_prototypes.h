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


#ifndef __MM_CORE_PROTOTYPES_H__
#define __MM_CORE_PROTOTYPES_H__

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

icf_return_t
mmCoreRemoveCallContext(
	icf_call_id_t 	call_id);


/*************************************************************************
*
*  FUNCTION NAME  : mmCoreCreateNewCallContext
*
*  DESCRIPTION    : The function creates the new call context and initialize it.
*					Also this fucntion makes the entry of this new context in the
*					existing call context list
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

icf_return_t
mmCoreCreateNewCallContext(
	mmCoreCallCtxList_t	**pp_new_call_context);

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreGetCallContext
*
*  DESCRIPTION    : 
*					
*					
*
*  RETURNS		  : 
*************************************************************************/

mmCoreCallCtxList_t*
mmCoreGetCallContext(
	mmCoreCallCtxList_t	*p_list_start,
	icf_call_id_t	call_id);

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreDumpCallContext
*
*  DESCRIPTION    : 
*					
*					
*
*  RETURNS		  : 
*************************************************************************/
mm_return_t
mmCoreDumpCallContext(
	mmCoreCallCtxList_t	*p_list_start
);

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreFreeCtxStream
*
*  DESCRIPTION    : 
*					
*					
*
*  RETURNS		  : 
*************************************************************************/
mm_return_t
mmCoreFreeCtxStream(
	mmCoreCallCtxList_t	*p_call_ctx_list
);

/*************************************************************************
*
*  FUNCTION NAME  : mm_fill_hdr_and_send_cc
*
*  DESCRIPTION    : This function will fill the API header and send it to 
*					to the IPTK
*
*  RETURNS		  : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
icf_return_t 
mmCoreFillHdrAndSend(
	  icf_msg_st			*p_msg, 
	  icf_api_id_t			api_id,
	  icf_app_id_t         app_id,
	  icf_call_id_t		call_id,
	  icf_error_t			*p_err);

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRmMsgHandler
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
mmCoreRmMsgHandler(icf_msg_st *p_msg,icf_error_t *p_err);


/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRtpMsgHandler
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
mmCoreRtpMsgHandler(icf_msg_st *p_msg,icf_error_t *p_err);


icf_return_t
mmConvertApiToMmEvent
		(icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf);

icf_return_t
mmConvertApiToRtpEvent
		(icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf);

mm_core_calldir_et
mmGetCallDirection(
	icf_call_id_t	call_id
);

icf_return_t
voiceDspConvertApiToEvent(
		icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillGclApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillGclApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreCommitCodecApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreCommitCodecApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreRelResApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreRelResApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillOmcApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillOmcApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillCmcApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillCmcApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillCmsApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillCmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillMmsApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillMmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillMrmsApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillMrmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreFillDmsApi
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreFillDmsApi(
        mmCoreCallCtx_t   *p_call_context,
        icf_void_t   *p_input_param,
        icf_app_id_t	app_id,
        icf_call_id_t call_id,
        icf_error_t    *p_err);

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
        icf_error_t    *p_err);

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
        icf_error_t    *p_err
);

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCoreUnexpectedMsg
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCoreUnexpectedMsg(
	icf_msg_st *p_msg,
	icf_error_t    *p_err
);

mm_return_t dspGetPhoneOffhookByDspId(
	icf_call_id_t	call_id,
	icf_uint16_t	dspId
);

mm_return_t dspGetPhoneOffhookByPhyId(
	uint16	phPhysicalId,
	icf_uint16_t	dspId
);

mm_return_t mmIsMatchedCodec(
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	matched_codec_num,
	icf_short_string_st	*p_matched_codec_name
) ;

icf_config_codec_attrib_st* mmGetSupportedCodec(
	mmCoreCallCtx_t   *p_call_context,
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	stream_codec_type
);

mm_return_t mmCheckCodecSupport(
	mmCoreCallCtx_t   *p_call_context,
 	icf_stream_capab_st	*p_input_stream,
	icf_codec_attrib_st		*p_input_codec
);

icf_uint16_t mmCheckDspCodecRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_uint8_t	codec_num
);

mm_return_t mmAllocatedDspPool(
	icf_call_id_t	call_id,
	icf_uint16_t	dspId,
	icf_stream_capab_st	*p_input_stream,
	icf_uint8_t	codec_num
);

mm_return_t mmFreeDspPoolByCallId(
	icf_call_id_t	call_id
);

mm_return_t mmFreeDspPoolByStreamId(
	icf_call_id_t	call_id,
	icf_stream_id_t	stream_id
);

mm_return_t mmChangeDspPoolToReserved(
	icf_call_id_t	call_id,
	mmCtxStream_t	*p_ctx_stream
);

mm_return_t mmChangeDspPoolPosition(
	icf_uint16_t	select_dsp_id,
	dspChan_t	*p_input_dsp_chan
);

mm_return_t mmCheckDspConfRestriction(
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
);

icf_uint16_t mmCheckDspConfRm(
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
);

mm_return_t mmAllocatedConfDspPool(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_uint16_t	dspId,
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
);

mm_return_t mmCreateDspConf(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_stream_id_t	stream_id,
	icf_stream_id_t	merge_stream_id
) ;

mm_return_t mmDeleteConfDspPool(
	icf_uint16_t	dspId,
	dspChan_t	*dspChan_p,
	dspChan_t	*p_merge_dsp_chan
);

mm_return_t mmDeleteDspConf(
	icf_call_id_t	call_id,
	icf_call_id_t	merged_call_id,
	icf_stream_id_t	stream_id,
	icf_stream_id_t	merge_stream_id
);

mm_return_t mmIsStreamCommited(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_capab_st		*p_input_stream,
	icf_stream_capab_st		**p_p_output_stream
);

mm_return_t mmCtxCommitCodec(
	icf_list_st			*p_commit_codec_list,
	icf_stream_commit_st		*p_commited_stream,
	mmCtxStream_t	*p_ctx_stream
);

mmCtxStream_t* mmCtxGetStream(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_t		stream_id
);

icf_uint8_t	mmCtxGetCommitCodec(
	mmCtxStream_t	*p_ctx_stream
);

mm_return_t mmCtxCommitStreamList(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st	*p_input_stream_list
);

mm_return_t mmCtxCheckMatchedCodec(
	icf_stream_commit_st	*p_commited_stream,
	icf_codec_attrib_st		*p_input_codec
);

mm_return_t mmCtxCommitMediaStreamParam(
	mmCoreCallCtx_t   *p_call_context,
	mmCtxStream_t	*p_ctx_stream,
	icf_codec_attrib_st		*p_input_codec
);

mm_return_t mmReserveDspRm(
	icf_call_id_t	call_id,
	icf_uint16_t		dspId,
 	icf_stream_capab_st	*p_input_stream,
	icf_codec_attrib_st		*p_input_codec
);

mm_return_t mmCommitDspRm(
        mmCoreCallCtx_t   *p_call_context,
   		icf_stream_commit_st	*p_commited_stream
);

mm_return_t mmReserveCodecRm(
	mmCoreCallCtx_t   *p_call_context,
 	icf_stream_capab_st	*p_input_stream,
	icf_stream_capab_st	*p_output_stream
);

mm_return_t mmRestoreReservedStreamRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st	*p_input_stream_list
);

mm_return_t mmRestoreRejectedStreamRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_list_st	*p_rejected_stream_list
);

mm_return_t mmRestoreCodecRm(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_id_t		input_stream_id
);

mm_return_t mmFillRejectedStream(
	icf_stream_id_list_st	*p_output_stream_list,
	icf_stream_capab_st		*p_input_stream
);

mm_return_t mmFillCommitedStream(
	icf_stream_commit_st		*p_output_stream,
	icf_codec_attrib_st		*p_input_codec
);

mm_return_t mmSetTransportMode(
	icf_sdp_stream_st		*p_local_sdp,
	icf_media_type_t		stream_type
);

mm_return_t mmVldtReservedCodec(
	icf_list_st		*p_reserved_codec_list
);

mm_return_t mmCheckDtmfCodec(
	icf_list_st		*p_ctx_codec_list,
	icf_codec_attrib_st		**p_p_input_dtmf_codec
);

mm_return_t mmCommitMatchedCodec(
	icf_list_st		*p_ctx_codec_list,
	icf_codec_attrib_st		*p_input_codec
);

mm_return_t mmReleaseCodecRm(
        mmCoreCallCtx_t   *p_call_context
);

mm_return_t mmReleaseCodecRm_by_stream_id(
        mmCoreCallCtx_t   *p_call_context,
        icf_stream_id_t	input_stream_id
);

mm_return_t mmIsMatchedCallId(
        mmCoreCallCtx_t   *p_call_context,
        icf_call_id_t call_id
);

mm_return_t mmGetAudioCodecParams(
	icf_config_audio_codec_param_st   *p_input_params,
	icf_audio_codec_param_st	*p_ouput_params
);

mm_return_t mmGetAudioMediaParams(
	icf_config_codec_attrib_st	*p_input_codec_attrib,
	icf_audio_profile_st   *p_input_media,
	icf_audio_profile_st   *p_output_media
);

mm_return_t mmGetVideoCodecParams(
	icf_config_video_codec_param_st   *p_input_params,
	icf_video_codec_param_st	*p_ouput_params
);

mm_return_t mmMakeStreamFormDefaultCodec(
	mmCoreCallCtx_t   *p_call_context,
	icf_stream_capab_st	*p_tmp_stream
);

mm_return_t mmCheckMediaStreamParamSupport(
	mmCoreCallCtx_t   *p_call_context,
	icf_codec_attrib_st		*p_input_codec,
	icf_uint8_t	stream_codec_type,
	union icf_media_stream_param_ut   *p_input_media,
	union icf_media_stream_param_ut   *p_output_media
);

mm_return_t mmAddMediaAttributes(
	icf_list_st         **p_p_encoded_attributes,
	icf_uint8_t *media_attri_str
);

mm_return_t mmCloneCodecInfoList(
	icf_list_st		*p_input_codec_list,
	icf_list_st		**p_p_output_codec_list
);

mm_return_t mmCloneEncAttribList(
	mmCoreCallCtx_t   *p_call_context,
	icf_list_st			*p_input_attri_list,
	icf_list_st			**p_p_dest_list
);

mm_return_t mmCtxSetSelfIp(
	mmCoreCallCtx_t   *p_call_context,
	mmCtxStream_t		*p_ctx_stream
);

mmCtxStream_t* mmCtxCreateNewStream(
	mmCoreCallCtx_t   *p_call_context
);

void dspPoolDisplay(
	void
);

void mmCtxTableDisplay(
        mmCoreCallCtx_t   *p_call_context
);

mm_return_t mmDumpsCtxTrace(
        mmCoreCallCtx_t   *p_call_context
);

/* RTP functions */
//Ssu-Ying, support Multiple Profile
mm_return_t  rtpOpen(
       icf_call_id_t call_id,
       icf_line_id_t SipAccountId,       
	mmCtxStream_t	*p_ctx_stream
);

mm_return_t  rtpModify(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream
);

mm_return_t  rtpConnect(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream
) ;

mm_return_t  rtpClose(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type
);

/* All memory related macros will go here */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif 

