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

/**************************************************************************
* Static Variables declaration and extern area
**************************************************************************/

/*************************************************************************
* The Static Function declaration area 
*************************************************************************/

static mm_return_t
mmCoreGclMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreCmtMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreRelResMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreOmcMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreCmsMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreMmsMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreMrmsMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreDmsMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreCmcMmi(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

static mm_return_t
mmCoreIgnore(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err);

/***********************************************************************/

/* State-Event Table for CORE CALL */
mmCoreStateEventFprtTy  mmRmCoreStateEvent[RM_CORE_LAST_EVENT] =
{
	mmCoreGclMmi,				/* ICF_GET_CODEC_LIST_REQ */
	mmCoreCmtMmi,				/* ICF_CODEC_COMMIT_REQ */
	mmCoreRelResMmi			/* ICF_REL_RES_REQ*/
};

mmCoreStateEventFprtTy  mmRtpCoreStateEvent[RTP_CORE_LAST_EVENT] =
{
	mmCoreOmcMmi,				/* ICF_OPEN_CHANNEL_REQ */
	mmCoreCmsMmi, 				/* ICF_CREATE_MEDIA_SESSION_REQ */
	mmCoreMmsMmi,			/* ICF_MODIFY_MEDIA_SESSION_REQ */
	mmCoreDmsMmi,				/* ICF_DELETE_MEDIA_SESSION_REQ */
	mmCoreCmcMmi,				/* ICF_CLOSE_MEDIA_CHANNELS_REQ. */
	mmCoreMrmsMmi,			/* ICF_MERGE_MEDIA_SESSION_REQ */
	mmCoreIgnore,				/* ICF_MEDIA_CAPABILITIES_REQ. */
};

/*************************************************************************
*
*  FUNCTION NAME  : mmGetCallDirection
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_core_calldir_et
mmGetCallDirection(
	icf_call_id_t	call_id
){
	mm_core_calldir_et	call_direction;
	
	if (call_id & MM_INC_CALL_ID_START)	/* 0x8000 */
	{	
		call_direction = MM_INCOMING_CALL;	
	}
	else	{
		call_direction = MM_OUTGOING_CALL;	
	}
	
	return call_direction;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreGclMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreGclMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	mmCoreCallCtxList_t			*p_new_call_ctx = MM_NULL;
	icf_call_id_t								call_id = 0;
	icf_get_codec_list_req_st				*p_gcl_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}
	
	p_gcl_payload = (icf_get_codec_list_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	/* get the memory to create the call context */
	if(MM_NULL == 
		(p_new_call_ctx = mmCoreGetCallContext(p_call_context_list, call_id)))
	{
		if(MM_FAILURE == mmCoreCreateNewCallContext(&p_new_call_ctx))
		{
			return MM_FAILURE;
		}
		else
		{
			p_new_call_ctx->call_context.call_id = call_id;
			p_new_call_ctx->call_context.call_direction = mmGetCallDirection(call_id);
		}
	}
	
	if(MM_FAILURE == 
		mmCoreFillGclApi(
			&(p_new_call_ctx->call_context),
			p_gcl_payload,
			app_id,
			call_id,
			p_err))
	{  
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreCmtMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreCmtMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_codec_commit_req_st				*p_cmt_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_cmt_payload = (icf_open_media_channel_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id	= MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreCommitCodecApi(
				&(p_call_context_node->call_context),
				p_cmt_payload, 
				app_id,
				call_id,
				p_err))
	{  
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;	
}


/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRelResMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreRelResMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_rel_res_req_st					*p_rel_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	
	icf_list_st		*p_ctx_stream_list = MM_NULL;
	mmCtxStream_t		*p_ctx_stream = MM_NULL;

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_rel_payload = (icf_rel_res_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreRelResApi(
				&(p_call_context_node->call_context), 
				p_rel_payload, 
				app_id,
				call_id, 
				p_err))

	{  
		return MM_FAILURE;
	}
#if 1
	p_ctx_stream_list = p_call_context_node->call_context.p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;
		if(0 != p_ctx_stream->local_sdp.stream_address.port_num)
		{
			rtpClose(p_call_context_node->call_context.call_id, p_ctx_stream->stream_id, p_ctx_stream->stream_type);
			MM_PRINTF("\n\tMM > Close RTP/RTCP");
		}

		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}
	
	mmCoreRemoveCallContext(call_id);
	if(MM_NULL != p_call_context_list)
	{
		mmCoreDumpCallContext(p_call_context_list);
	}
#else
	/* Check whether RTP/RTCP is closed or not, if yes, wait IPTK will send ICF_CLOSE_MEDIA_CHANNEL_REQ*/
	p_ctx_stream_list = p_call_context_node->call_context.p_stream_list;
	while(MM_NULL != p_ctx_stream_list)
	{
		p_ctx_stream = p_ctx_stream_list->p_data;
		if(0 != p_ctx_stream->local_sdp.stream_address.port_num)
		{
			need_close_media_channel_req = 1;
			MM_PRINTF("\n\tMM >RTP/RTCP session still alive on stream_id=%d  \n", p_ctx_stream->stream_id);
		}
		
		p_ctx_stream_list = p_ctx_stream_list->p_next;
	}

	if(0 == need_close_media_channel_req)
	{
		mmCoreRemoveCallContext(call_id);
		if(MM_NULL != p_call_context_list)
		{
			mmCoreDumpCallContext(p_call_context_list);
		}
	}
#endif
	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreOmcMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreOmcMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_open_media_channel_req_st			*p_oms_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_oms_payload = (icf_open_media_channel_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreFillOmcApi(
			&(p_call_context_node->call_context),
			p_oms_payload, 
			app_id,
			call_id,
			p_err))

	{  
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreCmsMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreCmsMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_create_media_session_req_st			*p_cms_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_cms_payload = (icf_create_media_session_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);

	if(MM_FAILURE == 
		mmCoreFillCmsApi(
			&(p_call_context_node->call_context),
			p_cms_payload, 
			app_id,
			call_id,
			p_err))
	{  
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;	
}


/*************************************************************************
*
*  FUNCTION NAME  : mmCoreMmsMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreMmsMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_modify_media_session_req_st			*p_mms_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_mms_payload = (icf_modify_media_session_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreFillMmsApi(
			&(p_call_context_node->call_context),
			p_mms_payload, 
			app_id,
			call_id,
			p_err))

	{ 
		return MM_FAILURE;
	}

	return MM_SUCCESS;	
}


/*************************************************************************
*
*  FUNCTION NAME  : mmCoreMrmsMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreMrmsMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_modify_media_session_req_st			*p_mms_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_mms_payload = (icf_modify_media_session_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreFillMrmsApi(
			&(p_call_context_node->call_context),
			p_mms_payload, 
			app_id,
			call_id,
			p_err))

	{ 
		return MM_FAILURE;
	}

	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreDmsMmi
*
*  DESCRIPTION    :  
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreDmsMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_delete_media_session_req_st			*p_dms_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	

	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_dms_payload = (icf_delete_media_session_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);
	
	if(MM_FAILURE == 
		mmCoreFillDmsApi(
			&(p_call_context_node->call_context),
			p_dms_payload, 
			app_id,
			call_id,
			p_err))

	{  
		return MM_FAILURE;
	}

	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreCmcMmi
*
*  DESCRIPTION    :  This API will be sent by IP Phone Toolkit only if channels need to be closed before CMS. 
*	If CMS has already happened, IP Phone Toolkit will send DMS for deleting streams and closing channels.
*					
*					
*
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/
mm_return_t
mmCoreCmcMmi(
  		icf_void_t						     *p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)

{
	icf_call_id_t								call_id = 0;
	icf_close_media_channel_req_st			*p_cmc_payload = MM_NULL;
	icf_app_id_t         app_id = 0;	
	
	if(MM_NULL == p_input_param )
	{
		return MM_FAILURE;
	}

	p_cmc_payload = (icf_delete_media_session_req_st*)
							(MM_MSG_GET_PAYLOAD(p_input_param));

	call_id = MM_MSG_GET_CALL_ID(p_input_param);
	app_id = MM_MSG_GET_APP_ID(p_input_param);

	p_call_context_node->call_context.call_direction = mmGetCallDirection(call_id);

	if(MM_FAILURE == 
		mmCoreFillCmcApi(
			&(p_call_context_node->call_context),
			p_cmc_payload, 
			app_id,
			call_id,
			p_err))
	{  
		return MM_FAILURE;
	}

	mmCoreRemoveCallContext(call_id);
	if(MM_NULL != p_call_context_list)
	{
		mmCoreDumpCallContext(p_call_context_list);
	}

	return MM_SUCCESS;	
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreIgnore
*
*  DESCRIPTION    : This function will simply ignore the message received
*	
*  RETURNS:       : Either MM_SUCCESS or MM_FAILURE		 	
*************************************************************************/
mm_return_t
mmCoreIgnore(
		icf_void_t						*p_input_param,
		mmCoreCallCtxList_t	*p_call_context_node,
		mm_error_t					*p_err)
{
MM_PRINTF("\n\tMM >Enter %s\n", __FUNCTION__);
    p_input_param = p_input_param;
    p_err = p_err;
    p_call_context_node = p_call_context_node;

	return MM_FAILURE;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreCreateNewCallContext
*
*  DESCRIPTION    : The function creates the new call context and initialize it.
*					Also this fucntion makes the entry of this new context in the
*					existing call context list
*
*  RETURNS		  : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/

mm_return_t
mmCoreCreateNewCallContext(
	mmCoreCallCtxList_t	**pp_new_call_context)
{
	
	mmCoreCallCtxList_t			*p_temp_call_context = MM_NULL;
	
	*pp_new_call_context = zyMalloc(sizeof(mmCoreCallCtxList_t));
	if(MM_NULL == pp_new_call_context)
	{
		return MM_FAILURE;
	}
	
	(*pp_new_call_context)->next = MM_NULL;
	(*pp_new_call_context)->call_context.p_mm_cfg_db = &gMmCfgDb_t;
	
    	if(MM_NULL == p_call_context_list)
	{
		p_call_context_list = *pp_new_call_context;
	}
    	else
	{
		for(p_temp_call_context = p_call_context_list;
			p_temp_call_context->next != MM_NULL; p_temp_call_context = p_temp_call_context->next);
			
		p_temp_call_context->next = *pp_new_call_context;
	}
	return MM_SUCCESS;
}
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
	icf_call_id_t	call_id)
{
	mmCoreCallCtxList_t			*p_temp_call_context = MM_NULL;

	p_temp_call_context = p_list_start;
	while(MM_NULL != p_temp_call_context)
	{
		if(p_temp_call_context->call_context.call_id == call_id)
		{
			break;
		}
	
		p_temp_call_context = p_temp_call_context->next;
	}

	return p_temp_call_context;

}

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
){
	mmCoreCallCtxList_t			*p_temp_call_context = MM_NULL;

	p_temp_call_context = p_list_start;
	while(MM_NULL != p_temp_call_context)
	{
		mmDumpsCtxTrace(&p_temp_call_context->call_context);
	
		p_temp_call_context = p_temp_call_context->next;
	}

	return MM_SUCCESS;

}

mm_return_t
mmCoreFreeCtxStream(
	mmCoreCallCtxList_t	*p_call_ctx_list
){
	icf_list_st	*p_stream_list = MM_NULL;
	mmCtxStream_t *p_ctx_stream = MM_NULL;
	
	/* free stream list and codec list */
	p_stream_list = p_call_ctx_list->call_context.p_stream_list;
	while(MM_NULL != p_stream_list)
	{
		p_ctx_stream = p_stream_list->p_data;

		if(MM_NULL != p_ctx_stream->local_sdp.p_codec_info_list)
		{
			mmDeleteCodecAttribList(&(p_ctx_stream->local_sdp.p_codec_info_list));
		}
		if(MM_NULL != p_ctx_stream->local_sdp.p_encoded_attributes)
		{		
			mmDeleteList(&(p_ctx_stream->local_sdp.p_encoded_attributes));
		}
		if(MM_NULL != p_ctx_stream->remote_sdp.p_codec_info_list)
		{
			mmDeleteCodecAttribList(&(p_ctx_stream->remote_sdp.p_codec_info_list));
		}
		if(MM_NULL != p_ctx_stream->remote_sdp.p_encoded_attributes)
		{	
			mmDeleteList(&(p_ctx_stream->remote_sdp.p_encoded_attributes));
		}
		
		p_stream_list = p_stream_list->p_next;
	}

	if(MM_NULL != p_call_ctx_list->call_context.p_stream_list)
	{
		mmDeleteList(&(p_call_ctx_list->call_context.p_stream_list));
	}
	p_call_ctx_list->call_context.stream_count = 0;

	return MM_SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRemoveCallContext
*
*  DESCRIPTION    : The function will remove call context
*
*  RETURNS		  : Either MM_SUCCESS or MM_FAILURE
*************************************************************************/

mm_return_t
mmCoreRemoveCallContext(
	icf_call_id_t 	call_id)
{
	mmCoreCallCtxList_t *p_prev_node = MM_NULL, *p_temp_pointer = MM_NULL;
	mm_return_t ret_val = MM_FAILURE;

	MM_PRINTF("\n\tMM >Enter mmCoreRemoveCallContext, call_id=%d\n", call_id);

	p_temp_pointer = p_call_context_list;
	while(p_temp_pointer != MM_NULL)
	{
		if(p_temp_pointer->call_context.call_id == call_id)
		{
			break;
		}
		else 
		{
			p_prev_node = p_temp_pointer;
			p_temp_pointer = p_temp_pointer->next;
		}
	}
	/* if a match is found, then delete the node */
	if ( MM_NULL != p_temp_pointer )
	{
		/* if this is not the first/last node then we need to move the pointers
		 * here and remove this node from the list */ 
		if (MM_NULL != p_prev_node)
		{
			if(MM_NULL != p_temp_pointer->next)
			{
				p_prev_node->next = p_temp_pointer->next;
			}
			else
			{
				/* if this is the last node and not the fist node of the list, then
				 * we need to set the pointer of the prev. node to NULL */
				p_prev_node->next = MM_NULL;
			}
		}
		else if (MM_NULL == p_prev_node)
		{
			/* if this is the first node of the list then the list header
			 * needs to be updated */
			 if(MM_NULL == p_temp_pointer->next)
			 {
			 	p_call_context_list = MM_NULL;
			 }
			 else
			 {
				p_call_context_list = p_temp_pointer->next;
			 }
		}
		 
		/* Now free the memory for the complete node */
		mmCoreFreeCtxStream(p_temp_pointer);
		zyFree(p_temp_pointer);
		p_temp_pointer = MM_NULL;
		
		ret_val = MM_SUCCESS;
	}

	MM_PRINTF("\n\tMM >Exist mmCoreRemoveCallContext, ret_val=%d\n", ret_val);
	return ret_val;
}

