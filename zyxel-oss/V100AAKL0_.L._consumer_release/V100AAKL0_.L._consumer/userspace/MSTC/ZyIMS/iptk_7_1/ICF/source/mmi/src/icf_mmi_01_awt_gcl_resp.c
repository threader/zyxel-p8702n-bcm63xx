/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_01_awt_gcl_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT GCL
 * 				response state in MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 												multiple m-lines
 * 						
 * 11-Apr-2007      Deepti Goyal   ICF 6.3      Codec change in media connect
 * 24-Sep-2009      Anurag Khare   SPR 20071    Fix for CSR 1-7798174
 *
 * Copyright (c) 2009, Aricent.  
 *
 ******************************************************************************/

/* include files */
#include "icf_internal_api_id.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_ecode.h"

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_01_dms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is 
 *                  awaiting GCL resp from RM and it receives a DMS request
 *                  Current State: ICF_MMI_01_AWT_GCL_RESP
 *                  Current Event: ICF_MMI_DMS_REQ
 *                  
 * RETURNS:        ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 ******************************************************************************/
icf_return_t icf_mmi_01_dms_request(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    icf_mmi_dms_request_st               *p_msg_data_recd = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_msg_data_recd = (icf_mmi_dms_request_st *)
        (p_mmi_pdb->p_internal_msg->p_msg_data);

    /* Check if this is not forceful delete */
    if(ICF_FALSE == p_msg_data_recd->forceful_delete)
    {
          /* Dump the request.On getting the awaited response,
           * MMI will check CC's call state and will be sending DMS to MM 
           * followed by REL_RES to RM.
           */
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)
    }
    /* if this is a forceful delete */
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)

		/*Send CLOSE_CHANNEL for all streams among active streams in 
		 * call_context (since we are not waiting for dms resp) Close channel 
		 * needs to be sent only in case this is AWT_GCL for mid call request 
		 */

		if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_forceful_dms_req(p_mmi_pdb);
			/* No need to send DMS response for forceful delete */
		}
		else
		{
			p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_DMS_ONGOING;

			/* Send REL_RES_IND so that even if success response 
			 * is received for the GCL req,the resources can be released.
			 */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
		}
    }/* End else */
   
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_01_gcl_resp
 *
 * DESCRIPTION:     This function is invoked when the MMI is 
 *                  awaiting GCL resp from RM and it receives 
 *                  GCL RESP from RM  
 *                  Current State: ICF_MMI_01_AWT_GCL_RESP
 *                  Current Event: ICF_MMI_GCL_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_mmi_01_gcl_resp(
        INOUT icf_mmi_pdb_st            *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_get_codec_list_resp_st			*p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    p_recv_msg_payload = (icf_get_codec_list_resp_st *)
        (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
    
    /* Request DBM for the ctx with the call id in the recvd msg */
    
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
    
    /* If failure response is recvd then function to process failure response
     * is triggered */

    if(ICF_FAILURE == p_recv_msg_payload->result)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)

        /* 
         * If the failure cause was resource unavailable then set release cause 
         * as ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE 
         */ 
        if(ICF_CAUSE_INSUFFICIENT_RESOURCES == 
                p_recv_msg_payload->error_cause &&
                ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause)
        {
            p_mmi_pdb->p_call_ctx->release_cause = 
                ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE;

            p_mmi_pdb->p_call_ctx->release_type |= 
                ICF_REL_TYPE_INTERNAL_INITIATED;
        }
        
        /* SPR 17399: set the release cause to send 488
         */
        else if((ICF_CAUSE_RES_NOT_AVAILABLE_FOR_T38_FAX == 
                p_recv_msg_payload->error_cause) &&
                (ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause))
        {
             p_mmi_pdb->p_call_ctx->release_cause = 
                ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE_FOR_T38_FAX;

            p_mmi_pdb->p_call_ctx->release_type |= 
                ICF_REL_TYPE_INTERNAL_INITIATED;
        }
        /* Fix for CSR 1-7798174: SPR 20071
         * set the release cause to send 488 when GCL resp 
         * received with error cause ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE
         * or ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE
         */
        else if(((ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE == 
                    p_recv_msg_payload->error_cause) || 
                    (ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE == 
                    p_recv_msg_payload->error_cause)) && 
                (ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause))
        {
            p_mmi_pdb->p_call_ctx->release_cause = 
                ICF_CALL_CLEAR_REASON_CODEC_NOT_AVAILABLE;

            p_mmi_pdb->p_call_ctx->release_type |= 
                ICF_REL_TYPE_INTERNAL_INITIATED;
        }

                
        ret_val = icf_mmi_gcl_failure_resp(p_mmi_pdb);

        if(ICF_FAILURE == ret_val)
        {
            /* Failure to process gcl failure response */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }
    /* If the response is success then function to process sucess response is 
     * called
     */
    else
    {
#ifndef ICF_UNIT_TEST
		ret_val = icf_mmi_check_api_total_streams(p_mmi_pdb, 
												ICF_GET_CODEC_LIST_RESP);
		
		if(ICF_FAILURE == ret_val)
		{
			ICF_ALARM(((icf_uint8_t *)"[MMI]: MEDIA MANAGER failure"));
			icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
			return ICF_SUCCESS;
		}
#endif
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_SUCCESS_RESP)
            
        ret_val = icf_mmi_gcl_success_resp(p_mmi_pdb);

        if(ICF_FAILURE == ret_val)
        {
            /* Failure to process gcl success response */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_gcl_failure_resp
 * 
 * DESCRIPTION:     This function processesthe failure response of GCL.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_mmi_gcl_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
	icf_mmi_mms_request_st				*p_mms_req = ICF_NULL;
	icf_boolean_t						initiate_call_clear = ICF_FALSE;
	icf_stream_id_t						stream_count = ICF_NULL;
	icf_boolean_t						success_stream_present = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
		else /* Mid call GCL response */
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}
        return ret_val;        
    }/* End if */

#ifdef ICF_TRACE_ENABLE
	p_mmi_pdb->stream_type = 0;
	p_mmi_pdb->result = ICF_FAILURE;
	ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_CODEC_RESERVE);
#endif	

	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;

		if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & ICF_RESTORATION_IN_PROGRESS)
		{
			initiate_call_clear = ICF_TRUE;
		}
		else
		{
			for(stream_count = 0; stream_count < 
					p_mms_req->modify_stream_count; stream_count++)
			{
				if(p_mms_req->modify_media[stream_count].bitmask & 
						ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)
				{
					initiate_call_clear = ICF_TRUE;
				}
			}
		}

		if(ICF_TRUE == initiate_call_clear)
		{
			/* Send error indication to CC */
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
			return ret_val;
		}

		/* This is the failure for first GCL only */

		for(stream_count = 0; stream_count < p_mms_req->modify_stream_count; 
				stream_count++)
		{
			if((p_mms_req->modify_media[stream_count].bitmask & 
												ICF_REMOTE_SDP_CODEC_CHANGE)||
				(p_mms_req->modify_media[stream_count].bitmask & 
												ICF_LOCAL_SDP_CODEC_CHANGE)||
                (p_mms_req->modify_media[stream_count].bitmask & 
                 ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
            {
				((icf_mmi_internal_mms_resp_st*)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->
							modified_stream_resp[stream_count].status_success = ICF_FALSE;

				((icf_mmi_internal_mms_resp_st*)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->
							modified_stream_resp[stream_count].send_mms = ICF_FALSE;	
			}
			else
			{
				success_stream_present = ICF_TRUE;
			}
		}
		/* case when the GCL has failed for certain streams but rest of 
		 * the streams are pending to be processed in MM_req in MMI ctxt
		 * These are those streams for which the modify required sending
		 * MMS only */
		if(ICF_TRUE == success_stream_present)
		{
			ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
		}
		else
		{
			/* send MM_resp to CC */
			((icf_mmi_internal_mms_resp_st *)p_mmi_pdb->p_mmi_ctx->\
			 p_mmi_resp)->result = ICF_FAILURE;

			ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;
		/* Update MM_resp in MMI ctxt for this stream as failure */
		icf_mmi_mark_failure_stream_mms_add_resp(p_mmi_pdb, 
							p_mms_req->p_add_media->stream_id[stream_count]);
		
		ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
	}
	else if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		/* Send failure CMS_RESP to CC */
		((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->\
		 p_mmi_resp)->result = ICF_FAILURE;
	
		ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */  


ICF_SESSION_CONTROL_START
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_gcl_success_resp
 * 
 * DESCRIPTION:     This function processes the success response of GCL.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_gcl_success_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                        ret_val = ICF_SUCCESS,
										dummy_ret_val = ICF_FAILURE;
    icf_get_codec_list_resp_st			*p_recv_msg_payload = ICF_NULL;
	icf_mmi_mms_request_st				*p_mms_req = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL;
	icf_boolean_t						initiate_call_clear = ICF_FALSE,
										send_commit = ICF_FALSE,
										send_mms = ICF_FALSE;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_uint16_t						bitmask = ICF_NULL;
    icf_list_st                         *p_src_list = ICF_NULL;
    icf_stream_capab_st                 *p_src_data = ICF_NULL;
	icf_sdp_stream_st					sdp;
 
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    p_recv_msg_payload = (icf_get_codec_list_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
    
    /* If CC is waiting for DMS response then send success response to it */
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)

		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_rel_res_ind(p_mmi_pdb);            
			/* Make and send DMS resp to CC */
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
		}
		else
		{
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
		}
        return ret_val;        
    }/* End if to check CC state */

#ifdef ICF_TRACE_ENABLE    
    /* Start traces to print received data */
    p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_recv_msg_payload;

    ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_GCL_DATA_RECVD)
	icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_GET_CODEC_LIST_RESP);
#endif

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_req = p_mmi_pdb->p_mmi_ctx->p_mms_req;
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		/* if any stream failure in case of RESTORATION or REVERT than 
		 * initiate call clear */
		if(p_recv_msg_payload->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
		{
			for(stream_count = 0; stream_count < p_mms_req->modify_stream_count;
				stream_count++)
			{
				if((p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
										ICF_RESTORATION_IN_PROGRESS)||
					(p_mms_req->modify_media[stream_count].bitmask & 
										ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE))
				{
					initiate_call_clear = ICF_TRUE;
					break;
				}
			}
		}

		if(ICF_TRUE == initiate_call_clear)
		{
			/* send mm_err_ind to CC for call clear */
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
			return ret_val;
		}

		if(p_recv_msg_payload->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
		{
			/* Mark the failure streams in MM_resp in MMI ctxt*/
			for(stream_count = 0; stream_count < p_mms_req->modify_stream_count;
					stream_count++)
			{
				if((p_mms_req->modify_media[stream_count].bitmask &
												ICF_REMOTE_SDP_CODEC_CHANGE)||
					(p_mms_req->modify_media[stream_count].bitmask & 
												ICF_LOCAL_SDP_CODEC_CHANGE) ||
                    (p_mms_req->modify_media[stream_count].bitmask & \
												ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					dummy_ret_val = icf_mmi_search_failure_stream(p_mmi_pdb, 
								&p_recv_msg_payload->rejected_stream_list,
							p_mms_req->modify_media[stream_count].stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
#ifdef ICF_TRACE_ENABLE
                        p_mmi_pdb->result = ICF_FAILURE;
                        p_mmi_pdb->stream_type = 
                            p_mms_req->modify_media[stream_count].stream_type;
                        ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_CODEC_RESERVE)
#endif
						/* update MM_resp in MMI ctxt for this stream */
						icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb,
							p_mms_req->modify_media[stream_count].stream_id);
						icf_mmi_unmark_send_mms_in_mms_resp(p_mmi_pdb,
							p_mms_req->modify_media[stream_count].stream_id);

					}
				}
			}
		}
		/* Further processing only for success non-TBCP streams */
		for(stream_count = 0; stream_count < p_mms_resp->mod_stream_count;
				stream_count++)
		{
			stream_id = 
					p_mms_resp->modified_stream_resp[stream_count].stream_id;
			stream_type = 
					p_mms_resp->modified_stream_resp[stream_count].stream_type;
			bitmask = p_mms_resp->modified_stream_resp[stream_count].bitmask;

			if(ICF_STREAM_TYPE_TBCP != stream_type)
			{
				if((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE)||
				   (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					dummy_ret_val = 
						icf_mmi_update_sdp_from_gcl_resp_for_stream_id(
								p_mmi_pdb, 
								stream_id,
								stream_type,
								ICF_SDP_LOCAL, 
								p_recv_msg_payload->p_reserved_stream_list);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* Make commit request from remote recvd SDP */
						send_commit = ICF_TRUE;
						ret_val = ICF_SUCCESS;
					}
				}
				else if(bitmask & ICF_LOCAL_SDP_CODEC_CHANGE)
				{
					/* Update local SDP from GCL response */
					dummy_ret_val = 
						icf_mmi_update_sdp_from_gcl_resp_for_stream_id(
								p_mmi_pdb, 
								stream_id,
								stream_type,
								ICF_SDP_LOCAL, 
								p_recv_msg_payload->p_reserved_stream_list);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* Make MMS request from local SDP */
						send_mms = ICF_TRUE;
						ret_val = ICF_SUCCESS;
					}
				}
				else if(bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)
				{
					/* Update applied SDP from GCL response */
					dummy_ret_val = 
							icf_mmi_update_sdp_from_gcl_resp_for_stream_id(
								p_mmi_pdb, 
								stream_id,
								stream_type,
								ICF_SDP_APPLIED, 
								p_recv_msg_payload->p_reserved_stream_list);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						/* Make commit request from applied SDP */
						send_commit = ICF_TRUE;
						ret_val = ICF_SUCCESS;
					}
				}
			}
		}

		if(ICF_SUCCESS == ret_val)
		{
			if(ICF_TRUE == send_commit)
			{
				ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

				if(ICF_SUCCESS == ret_val)
				{
					ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_03_AWT_COMMIT_RESP)
				}
			}
			else if(ICF_TRUE == send_mms)
			{
				ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
				if(ICF_SUCCESS == ret_val)
				{
					ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_04_AWT_MMS_RESP)
				}
			}
		}
	}
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		if(p_recv_msg_payload->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
		{
			/* Mark the failure streams in CMS_resp in MMI ctxt*/
			for(stream_count = 0; stream_count < p_mms_resp->add_stream_count;
				stream_count++)
			{
				stream_id = 
						p_mms_resp->added_stream_resp[stream_count].stream_id;

				dummy_ret_val = icf_mmi_search_failure_stream(p_mmi_pdb, 
							&p_recv_msg_payload->rejected_stream_list,
							stream_id);

				if(ICF_SUCCESS == dummy_ret_val)
				{
#ifdef ICF_TRACE_ENABLE
                    p_mmi_pdb->result = ICF_FAILURE;
                    p_mmi_pdb->stream_type = 
                       p_mms_resp->added_stream_resp[stream_count].stream_type;
                    ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_CODEC_RESERVE)
#endif
					p_mms_resp->added_stream_resp[stream_count].status_success =
							ICF_FALSE;
				}
			}
		}

		for(stream_count = 0; stream_count < p_mms_resp->add_stream_count;
			stream_count++)
		{
			dummy_ret_val = ICF_FAILURE;

			if(ICF_TRUE == 
				p_mms_resp->added_stream_resp[stream_count].status_success)
			{
				stream_id = 
						p_mms_resp->added_stream_resp[stream_count].stream_id;
				stream_type = 
						p_mms_resp->added_stream_resp[stream_count].stream_type;

				ret_val = icf_mmi_update_sdp_from_gcl_resp_for_stream_id(
								p_mmi_pdb, 
								stream_id,
								stream_type,
								ICF_SDP_LOCAL, 
								p_recv_msg_payload->p_reserved_stream_list);

				/* Check if the reinvite for adding the stream was INC then 
				 * MMI shall send commit*/
				dummy_ret_val = icf_mmi_get_sdp_for_stream_id(
											p_mmi_pdb,
											stream_id,
											ICF_SDP_REMOTE,
											&sdp);
			}
		}

		if(ICF_SUCCESS == dummy_ret_val)
		{
			ret_val = icf_mmi_send_commit_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_03_AWT_COMMIT_RESP)
			}			
		}
		else
		{
			ret_val = icf_mmi_send_open_channel_req(p_mmi_pdb);
			
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_06_AWT_OPEN_CHANNEL_RESP)
			}
		}
	}
	else if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

        p_src_list = p_recv_msg_payload->p_reserved_stream_list;

        /* Check whether gcl_resp contains only "telephone-event" codec */
        while(p_src_list)
        {
            p_src_data = ((icf_stream_capab_st *)p_src_list->p_data);

            icf_cmn_get_list_count(p_src_data->p_codec_info_list,
                                            &p_mmi_pdb->num_of_codecs);

            if ((ICF_STREAM_TYPE_AUDIO == p_src_data->stream_type)&&
                (1 == p_mmi_pdb->num_of_codecs) &&
                (0 == icf_port_strcmp(
                ((icf_codec_attrib_st *)p_src_data->p_codec_info_list->\
				 p_data)->codec_name.str, (icf_uint8_t *)"telephone-event")))
            {
                ICF_PRINT(((icf_uint8_t *)"\n[MMI]: Only telephone-event codec received in GCL response"));
                
                icf_mmi_send_rel_res_ind(p_mmi_pdb);

		        /* Send failure CMS_RESP to CC */
		        ((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->\
				 p_mmi_resp)->result = ICF_FAILURE;
		        
		        ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
                return ret_val;
            }
            p_src_list = p_src_list->p_next;
        }
        

		if(p_recv_msg_payload->bit_mask & ICF_CODEC_LIST_REJECTED_STREAM_LIST)
		{
			/* Mark the failure streams in CMS_resp in MMI ctxt*/
			for(stream_count = 0; stream_count < p_cms_resp->count;	
				stream_count++)
			{
				dummy_ret_val = icf_mmi_search_failure_stream(p_mmi_pdb, 
							&p_recv_msg_payload->rejected_stream_list,
							p_cms_resp->stream_status[stream_count].stream_id);

				if(ICF_SUCCESS == dummy_ret_val)
				{
#ifdef ICF_TRACE_ENABLE
                        p_mmi_pdb->result = ICF_FAILURE;
                        p_mmi_pdb->stream_type = 
                            p_cms_resp->stream_status[stream_count].stream_type;
                        ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_CODEC_RESERVE)
#endif
					p_cms_resp->stream_status[stream_count].status_success = 
							ICF_FALSE;
				}
			}
		}

		for(stream_count = 0; stream_count < p_cms_resp->count; stream_count++)
		{
			stream_id = p_cms_resp->stream_status[stream_count].stream_id;
			stream_type = p_cms_resp->stream_status[stream_count].stream_type;

			if((ICF_TRUE == 
				p_cms_resp->stream_status[stream_count].status_success)&&
				(ICF_STREAM_TYPE_TBCP != stream_type))
			{
				ICF_MMI_SET_BIT(p_mmi_pdb,ICF_MMI_RES_RESERVED)

				ret_val = icf_mmi_update_sdp_from_gcl_resp_for_stream_id(
								p_mmi_pdb, 
								stream_id,
								stream_type,
								ICF_SDP_LOCAL, 
								p_recv_msg_payload->p_reserved_stream_list);
			}
		}

        /* If call is incoming and FS */
        if((ICF_CALL_DIR & p_mmi_pdb->p_call_ctx->common_bitmask) && 
			(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp))
		{
			ret_val = icf_mmi_send_commit_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_03_AWT_COMMIT_RESP)
			}
		}
		else
		{
			ret_val = icf_mmi_send_open_channel_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_06_AWT_OPEN_CHANNEL_RESP)
			}
		}
	}


	if(ICF_FAILURE == ret_val)
	{
		switch(p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			case ICF_MMI_CMS_ONGOING:

				icf_mmi_mark_all_stream_cms_resp(p_mmi_pdb);
				ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
				break;

			case ICF_MMI_MMS_MOD_ONGOING:

				/* Update MM_resp in MMI ctxt with failure for add action and 
				 * send MM_resp to CC */
				icf_mmi_mark_all_stream_mms_add_resp(p_mmi_pdb);

			case ICF_MMI_MMS_ADD_ONGOING:

				ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);

				break;
		}
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */ 

ICF_SESSION_CONTROL_END


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_gcl_resp_for_stream_id
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_sdp_from_gcl_resp_for_stream_id
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_stream_id_t		stream_id,
	icf_media_type_t	stream_type,
	icf_uint8_t			sdp_type,
	icf_list_st			*p_list
)
{
	icf_return_t					ret_val = ICF_SUCCESS;
	icf_sdp_stream_st				sdp;
	icf_stream_capab_st				*p_data = ICF_NULL;


	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	while(ICF_NULL != p_list)
	{
		p_data = p_list->p_data;

		if(stream_id == p_data->stream_id)
		{
			sdp.bit_mask = ICF_NULL;
			if(p_data->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT)
			{
				sdp.p_codec_info_list = p_data->p_codec_info_list;
			}

			if(p_data->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT)
			{
				sdp.p_encoded_attributes = p_data->p_encoded_attributes;
				sdp.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
			}

			if(p_data->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT)
			{
				if(ICF_STREAM_TYPE_AUDIO == stream_type)
				{
					icf_port_memcpy(
					(icf_void_t *)&(sdp.media_stream_param.audio_profile),
					(icf_void_t *)&(p_data->media_stream_param.audio_profile),
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_audio_profile_st));
				}
				else
				{
					icf_port_memcpy(
					(icf_void_t *)&(sdp.media_stream_param.video_profile),
					(icf_void_t *)&(p_data->media_stream_param.video_profile),
					(icf_uint32_t)ICF_PORT_SIZEOF(icf_video_profile_st));				
				}
				sdp.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
			}

			if(ICF_SDP_LOCAL == sdp_type)
			{
				ret_val = icf_mmi_check_sdp_in_call_ctx(
											p_mmi_pdb,
											stream_id,
											sdp_type);
			}

			if(ICF_SUCCESS == ret_val)
			{
				icf_cmn_update_codecs_for_stream_id_in_call_ctxt(
												p_mmi_pdb->p_glb_pdb, 
												stream_id,
												sdp_type,
												&sdp);

				icf_mmi_update_media_params_for_stream_id_in_call_ctxt(
												p_mmi_pdb->p_glb_pdb,
												stream_id,
												sdp_type,
												&sdp);
			}
			break;
		}
		p_list = p_list->p_next;
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
	return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_mmi_update_media_params_for_stream_id_in_call_ctxt
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function. The function also fetches
 **				  the node corresponding to the given stream_id from the
 **				  icf_sdp_stream_info_st in the call_ctxt
 **
 *****************************************************************************/
icf_return_t	icf_mmi_update_media_params_for_stream_id_in_call_ctxt
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	IN		icf_stream_id_t		stream_id,
	IN		icf_uint8_t			sdp_type,
	INOUT	icf_sdp_stream_st	*p_src_sdp
)
{
	/* local variables */
	icf_sdp_stream_st			*p_sdp = ICF_NULL;
	icf_internal_sdp_stream_st	*p_internal_sdp = ICF_NULL;
	icf_return_t				ret_val = ICF_FAILURE;
	
	/* get the node for the stream_id from the CC- sdp_stream_info */
	if (ICF_SUCCESS == icf_cmn_fetch_sdp_in_list_for_stream_id(
							p_glb_pdb,
							stream_id,
							sdp_type,
							&p_internal_sdp))
	{
		if(p_internal_sdp)
		{
			p_sdp = &p_internal_sdp->sdp_stream;			
		}
	
		if(p_sdp)
		{
			/* copy parameters in the SDP stream ,minus the codec params */
			icf_mmi_clone_sdp_stream_params(
							p_glb_pdb,
							p_sdp,
							p_src_sdp);
			
			ret_val = ICF_SUCCESS;			
		} /* while loop ends */
	}/* if case ends */
	
	return ret_val;

}/* function ends */


/*****************************************************************************
 ** FUNCTION:     icf_mmi_clone_sdp_stream_params
 **
 ** DESCRIPTION:  This function clones the parameters of icf_sdp_stream_st
 **				  minus the codec_attribute_st. the codec parameters are
 **	              seperately in another function.
 **
 *****************************************************************************/
icf_return_t	icf_mmi_clone_sdp_stream_params
(
	INOUT	icf_glb_pdb_st		*p_glb_pdb,
	INOUT	icf_sdp_stream_st	*p_dest_sdp_stream,
	INOUT	icf_sdp_stream_st	*p_src_sdp_stream
)
{
	/* local variables */
	icf_error_t		ecode = ICF_ERROR_UNDEFINED;
	
	if (ICF_SDP_STREAM_MEDIA_STREAM_PRESENT & p_src_sdp_stream->bit_mask)
	{
		p_dest_sdp_stream->media_stream_param = 
			p_src_sdp_stream->media_stream_param;
		p_dest_sdp_stream->bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
	}
	if (ICF_SDP_STREAM_RTCP_INFO_PRESENT & p_src_sdp_stream->bit_mask)
	{
		p_dest_sdp_stream->rtcp_address = p_src_sdp_stream->rtcp_address;
		p_dest_sdp_stream->bit_mask |= ICF_SDP_STREAM_RTCP_INFO_PRESENT;
	}
	if (ICF_SDP_STREAM_ENCPARAMS_PRESENT & p_src_sdp_stream->bit_mask)
	{
		icf_cmn_copy_string_list(p_glb_pdb,
						&(p_dest_sdp_stream->p_encoded_attributes),
						p_src_sdp_stream->p_encoded_attributes,
						&ecode);

		p_dest_sdp_stream->bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
	}
	return ICF_SUCCESS;
}/* function ends */


