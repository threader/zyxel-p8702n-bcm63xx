/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_04_awt_mms_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT MMS
 * 				response state in MMI.
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 25-Aug-2006      Amit Sharma    SPR 12554
 *                  Umang Singh
 * 												multiple m-lines
 * 28-Aug-2006      Aman Arora     SPR 12588	setting the transport address in 
 *  											applied SDP from the remote SDP
 *												from the CC ctxt	
 * 11-Apr-2007      Deepti Goyal   SPR 14905    Codec Change in media connect req.
 *
 * Copyright (c) 2007, Aricent  
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

#ifndef ICF_MM_REQUEST_BUFFERING
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_pending_req
 *
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting MMS resp from MM and it receives DMS 
 *                  request from CC or media_session_req from app.
 *.
 *                  Current State: ICF_MMI_04_AWT_MMS_RESP
 *                  Current Event: ICF_MMI_DMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_04_pending_req(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
	/* local variables */
	icf_return_t							ret_val = ICF_SUCCESS;
	icf_mmi_dms_request_st					*p_msg_data_recd = ICF_NULL;
    p_msg_data_recd = 
			(icf_mmi_dms_request_st *)(p_mmi_pdb->p_internal_msg->p_msg_data);

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
  
	if ( ICF_DELETE_MEDIA_SESSION_REQ ==
			p_mmi_pdb->p_mmi_ctx->p_media_session_req->hdr.api_id)
	 {
		icf_mmi_send_dms_req(p_mmi_pdb);

        ICF_MMI_SET_STATE(p_mmi_pdb,
			ICF_MMI_05_AWT_DMS_RESP)
			
		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)  
			
		ret_val = icf_mmi_send_rel_res_ind(p_mmi_pdb);
        
        if(ICF_SUCCESS == ret_val)
        {
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)

			ret_val = icf_mmi_send_qos_release_req(p_mmi_pdb);

ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)

            ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
				
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)  
	
ICF_CHECK_IMS_END
#else

		ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)

		ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_API_SEND)  
#endif
        }
	 }
	 else
	 {
		 ret_val = icf_mmi_send(
			 p_mmi_pdb,
			 p_mmi_pdb->p_mmi_ctx->p_media_session_req,
			 p_mmi_pdb->p_ecode);
	 }
	 ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
		 p_mmi_pdb->p_mmi_ctx->p_media_session_req,ICF_MEM_COMMON,
		 p_mmi_pdb->p_ecode,ret_val)
		 p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;


    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_dms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is 
 *                  awaiting CMS resp from MM and it receives a DMS request
 *                  Current State: ICF_MMI_04_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                 
 *
 *****************************************************************************/
icf_return_t icf_mmi_04_dms_request(
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
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_DMS_NOT_FORCEFUL)
        /* Dump the request */
    }
    /* If it is a forceful delete */
    else
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FORCEFUL_DMS)

		icf_mmi_send_forceful_dms_req(p_mmi_pdb);
        
		/* No need to send DMS resp to CC for forceful delete */
    }/* End else */ 
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_mms_resp
 *
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting MMS resp from MM and it receives the same.
 *                  Current State: ICF_MMI_04_AWT_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_return_t icf_mmi_04_mms_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
	/* local variables */	
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_modify_media_session_resp_st 	*p_recv_msg_payload = ICF_NULL;

    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    
    /* Check the call state of CC. If the call state is AWT DMS then the 
     * present session needs to be cleared 
     */
	
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
#ifndef ICF_MM_REQUEST_BUFFERING
		if ( ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
		{
			icf_mmi_04_pending_req(p_mmi_pdb);
		}
		else
#endif
		{
			ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
			icf_mmi_send_awaited_dms_req(p_mmi_pdb);
        }
    }/* End if to check state of CC */
    
    /* Rest processing will be done only if CC is not in AWT DMS RESP state */
    else
    {
        p_recv_msg_payload = (icf_modify_media_session_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
        
        /* check if the response is failure. In case of Failure response 
         * failure will be send to CC
         */
        if(ICF_FAILURE == p_recv_msg_payload->result)
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)
            
			ret_val = icf_mmi_handle_mms_failure_resp(p_mmi_pdb);

        }/* End if */
        /* If response is success, success response will be send to CC and the 
         * SDP will be updated in case of remote media change or remote media 
         * application
         */
        else
        {
#ifndef ICF_UNIT_TEST
			ret_val = icf_mmi_check_api_total_streams(p_mmi_pdb, 
											ICF_MODIFY_MEDIA_SESSION_RESP);
			
			if(ICF_FAILURE == ret_val)
			{
				ICF_ALARM(((icf_uint8_t *)"[MMI]: MEDIA MANAGER failure"));
				icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
				return ICF_SUCCESS;
			}
#endif  
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_SUCCESS_RESP)
            ret_val = icf_mmi_handle_mms_success_resp(p_mmi_pdb); 
#ifndef ICF_MM_REQUEST_BUFFERING
            /* Send success response to CC */
			if (ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
			{
				icf_mmi_04_pending_req(p_mmi_pdb);
			}	
#endif
        }/* End else */               
    }/* End else */

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;    
}


#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_nm_deact_ind
 * 
 * DESCRIPTION:     This function is invoked when the MMI is awaiting MMS resp 
 *                  from MM and it receives a Secondary PDP Context Deactivation
 *					Indication from NM   .
 *                  Current State: ICF_MMI_04_AWT_MMS_RESP
 *                  Current Event: ICF_MMI_NM_SEC_DEACT_IND
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_04_nm_deact_ind(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t                         ret_val;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    if ( ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
	{
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_mmi_pdb->p_mmi_ctx->p_media_session_req,ICF_MEM_COMMON,
			p_mmi_pdb->p_ecode,ret_val)
			p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;
	}
			
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
        
    ret_val = icf_mmi_xx_nm_deact_ind(p_mmi_pdb);
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;         
}/* End function */

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_04_nm_modify_ind
 * 
 * DESCRIPTION:     This function is invoked when the MMI is awaiting MMS resp 
 *                  from MM and it receives a Secondary PDP Context Modification
 *					Indication from NM   .
 *                  Current State: ICF_MMI_04_AWT_MMS_RESP
 *                  Current Event: ICF_MMI_NM_SEC_MODIFY_IND
 *                                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_04_nm_modify_ind(
        INOUT icf_mmi_pdb_st         *p_mmi_pdb)
{
    icf_return_t                         ret_val;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

    if ( ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_media_session_req)
	{
		ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
			p_mmi_pdb->p_mmi_ctx->p_media_session_req,ICF_MEM_COMMON,
			p_mmi_pdb->p_ecode,ret_val)
			p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;
	}			
        
    ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
        
    ret_val = icf_mmi_xx_nm_modify_ind(p_mmi_pdb);
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;         
}/* End function */

#endif

/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mms_failure_resp
 *
 * DESCRIPTION:     This function is invoked to handle the failure MMS response
 * 					within MMI from MM
 *                  Current State: 	ICF_MMI_04_AWT_MMS_RESP
 *                  EVENT:			ICF_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_result_t icf_mmi_handle_mms_failure_resp
(
 	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* local variables */
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_mmi_context_st					*p_mmi_ctxt = p_mmi_pdb->p_mmi_ctx;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_modify_media_session_resp_st	*p_recv_msg_payload = ICF_NULL;
	icf_boolean_t						send_GCL = ICF_FALSE,
										send_COMMIT = ICF_FALSE;
	icf_stream_id_t							count = ICF_NULL;
	icf_uint32_t						bitmask = ICF_NULL,
										resp_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mms_resp = (icf_mmi_internal_mms_resp_st*)p_mmi_ctxt->p_mmi_resp;
	
	p_mms_resp->result = ICF_FAILURE;

    p_recv_msg_payload = (icf_modify_media_session_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

	p_mms_resp->error_cause = p_recv_msg_payload->error_cause;	

	/* check if the call is established or not yet */
	if (ICF_MEDIA_ESTABLISHED != (ICF_MEDIA_ESTABLISHED &
							p_mmi_ctxt->mmi_bitmask))
	{
		/* MMS failed during call setup phase 
		 * Update the MMS resp in MMI ctxt- top level failure        
		 * Free info for respective streams in MMS req and response 
		 * in the MMI Ctxt        
		 * Send response to CC
		 * Move to IDLE state        
		 * Return */
		ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		return ret_val;
	}
            
	for (count =0; count < p_mms_resp->mod_stream_count; count++)
	{	
		bitmask = p_mms_resp->modified_stream_resp[count].bitmask;
		/* revert failure cases */
		if ((bitmask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE) ||
		   (bitmask & ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE) ||
		   (bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE) ||
		   (bitmask & ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE) ||
			/* remote answer apply failure */	
		   (bitmask & ICF_REMOTE_ANSWER_CODEC_CHANGE))		   
		{
			/* This is a failure condition, and we need to clear the 
			 * call. Call clear trigger to be received from CC.
			 * FREE MM_req and MM_resp from MMI ctxt      
			 * Send MM_ERR_IND to CC for clear_call.      
			 * Move to IDLE state.      
			 * Return. */
			/* clear the MMS request and response in the MMI ctxt */
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
					
			return ret_val;
		}
			
		/* now check for individual streams */
        if (ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)
		{
			if (ICF_RESTORATION_IN_PROGRESS & p_mmi_ctxt->mmi_bitmask)
			{
				/* send ERR_IND to CC */
				ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
				return ret_val;
			}
			/* incase the restoration bit is not set */
			/* Update MM_resp in MMI ctxt- failure for all streams
			 * Make GCL payload for these streams from last_commited 
			 * SDP      
			 * Send_GCL == TRUE; */
			p_mms_resp->modified_stream_resp[count].status_success = 
						ICF_FAILURE;
			
			/* make GCL request for the failed stream to restore its status */
			send_GCL =ICF_TRUE;
		} /* if loop */
		else 
		if ((ICF_LOCAL_SDP_CODEC_CHANGE & bitmask)|| (bitmask & \
                    ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE)) 
		{
			/* application initiated Re-Inivte/Update scenario */
			/* Update MM_resp in MMI ctxt      
			 * Make COMMIT payload for these streams from last_commited 
			 * SDP.      
			 * Send_COMMIT == TRUE */
			p_mms_resp->modified_stream_resp[count].status_success = 
						ICF_FAILURE;
			/* make GCL request for the failed stream to restore its status */
			send_COMMIT =ICF_TRUE;
		}
		else
		if ((ICF_REMOTE_SDP_MEDIA_CHANGE & bitmask ) ||
		   (ICF_LOCAL_SDP_MEDIA_CHANGE & bitmask ) ||
		   (ICF_MODE_CHANGE & bitmask))
		{
			/* Update MM_resp in MMI ctxt */
			p_mms_resp->modified_stream_resp[count].status_success = 
						ICF_FAILURE;
			/* free the MMS req from the MMI context */
		}
	} /* for loop ends */

	if (p_mmi_ctxt->ongoing_event == ICF_MMI_MMS_MOD_ONGOING)
	{
		/* delete the add request from the MMS request, as we will not move to 
		 * the next state of the processing */
		if (((icf_mmi_mms_request_st *)p_mmi_ctxt->p_mms_req)->p_add_media)
		{
			ICF_MEMFREE(p_mmi_pdb->p_glb_pdb,
				((icf_mmi_mms_request_st *)p_mmi_ctxt->p_mms_req)->p_add_media,
				ICF_MEM_COMMON,
				p_mmi_pdb->p_ecode,
				ret_val)
		}
		((icf_mmi_mms_request_st *)p_mmi_ctxt->p_mms_req)->p_add_media = 
				ICF_NULL;
		
		/* clear the add parameters in the MMS response */
		for (resp_count =0; 
				 resp_count < p_mms_resp->add_stream_count;
				 resp_count++)
		{
			p_mms_resp->added_stream_resp[resp_count].status_success = 
						ICF_FAILURE;
		}
	}

	if (ICF_TRUE == send_GCL)
	{
		p_mmi_ctxt->mmi_bitmask |= ICF_RESTORATION_IN_PROGRESS;
		
		ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
		}
	}
	else
	if (ICF_TRUE == send_COMMIT)
	{
		p_mmi_ctxt->mmi_bitmask |= ICF_RESTORATION_IN_PROGRESS;
		
		ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_03_AWT_COMMIT_RESP)
		}
	}
	else
	{
		/* sending final response to CC, need to reset the restoration bit here */
		ICF_MMI_RESET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)
		ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
		return ret_val;
	}

	if(ICF_FAILURE == ret_val)
	{
		/* sending final response to CC, need to reset the restoration bit here */
		ICF_MMI_RESET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)
		icf_mmi_mark_all_stream_mms_add_resp(p_mmi_pdb);
		icf_mmi_send_modify_media_response(p_mmi_pdb);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}/* function ends */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_handle_mms_success_resp
 *
 * DESCRIPTION:     This function is invoked to handle the sucessful MMS 
 * 					response within MMI from MM
 *                  Current State: 	ICF_MMI_04_AWT_MMS_RESP
 *                  EVENT:			ICF_MMS_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
icf_result_t icf_mmi_handle_mms_success_resp
(
 	INOUT	icf_mmi_pdb_st	*p_mmi_pdb
)
{
	/* local variables */
	icf_return_t						ret_val = ICF_SUCCESS;
	icf_mmi_context_st					*p_mmi_ctxt = p_mmi_pdb->p_mmi_ctx;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_modify_media_session_resp_st 	*p_recv_msg_payload = ICF_NULL;
	icf_boolean_t						send_GCL = ICF_FALSE,
										send_COMMIT = ICF_FALSE;
	icf_stream_id_t						rej_stream_id = ICF_NULL;
	icf_list_st							*p_accpt_list_hdr = ICF_NULL;
	icf_uint32_t						bitmask = ICF_NULL;
	icf_resp_stream_st					*p_stream_node = ICF_NULL;
	icf_stream_id_t						stream_count =0, resp_count =0;
	icf_stream_id_t						accpt_stream_id =0;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_mms_resp = (icf_mmi_internal_mms_resp_st*)p_mmi_ctxt->p_mmi_resp;

    p_recv_msg_payload = (icf_modify_media_session_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

#ifdef ICF_TRACE_ENABLE
    /* Start traces for data received in CMS Response */
    p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_recv_msg_payload;
	icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_MODIFY_MEDIA_SESSION_RESP);
#endif

	if (p_recv_msg_payload->bit_mask & 
					ICF_MODIFY_MEDIA_RESP_REJECT_STREAM_PRESENT)
	{
		/* check the stream_id's in the rejected list */
		for(stream_count =0; 
			stream_count <p_recv_msg_payload->rejected_stream_list.count;
			stream_count++)
		{
			/* check the action for the streams in the rejected list */
			resp_count = 0;
			/* get the stream id from the rejected stream list */
			rej_stream_id = 
				p_recv_msg_payload->rejected_stream_list.stream_id[stream_count];
			while (resp_count < p_mms_resp->mod_stream_count)
			{
				/* now fetch the corresponding stream_id in the mms_resp */
				if (p_mms_resp->modified_stream_resp[resp_count].stream_id ==
					rej_stream_id)
				{
					bitmask = 
						p_mms_resp->modified_stream_resp[resp_count].bitmask;

					/* check the bitmask for this stream */	
					if ((ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE & bitmask ) ||
				   		(ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & bitmask) ||
				   		((ICF_REMOTE_ANSWER_CODEC_CHANGE &  bitmask ) &&
						(p_mmi_ctxt->mmi_bitmask & ICF_MEDIA_ESTABLISHED)) ||				   		
				   		(ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE & bitmask)||
				   		(ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE & bitmask)||
						/* if restoration was in progress was this stream */
						(ICF_FALSE == 
							p_mms_resp->modified_stream_resp[resp_count].
							status_success))
					{
						/* this is a scenario, where MMS on the whole is 
						 * successful but it has failed for a particular stream 
						 * where ICF was trying to revert to the older set of 
						 * media capab or trying to apply the peer's answer. 
						 * So here we will clear the call
						 */ 
						/* send ERR_IND to CC */
						ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
						return ret_val;	
					}
					else 
					if ((ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask) &&
						(!(ICF_MEDIA_ESTABLISHED & p_mmi_ctxt->mmi_bitmask)))
					{
						/* when call setup is ongoing */
						/* Update the MMS resp in MMI ctxt- top level failure 
						 * Free info for respective streams in MMS req and 
						 * response in the MMI Ctxt        
						 * Send response to CC        
						 * Move to IDLE state        
						 * Return */
						p_mms_resp->result = ICF_FAILURE;

						ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
						return ret_val;
					}
					if (ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)
					{
					/* update the MMS response for the stream */
						p_mms_resp->modified_stream_resp[resp_count].
								status_success = ICF_FALSE;
					/* as a part of the restoration, we will send GCL for this
					 * stream */
					/* create GCL request for this stream */
						send_GCL = ICF_TRUE;					
					}
					else
					if ((ICF_LOCAL_SDP_CODEC_CHANGE & bitmask)||\
                        (ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE & bitmask))
					{
					/* update the MMS response for the stream */
						p_mms_resp->modified_stream_resp[resp_count].
								status_success = ICF_FALSE;
					/* as a part of the restoration, we will send commit for 
					 * this stream */
					/* create COMMIT request for this stream */
						send_COMMIT = ICF_TRUE;					
					}
					if (ICF_REMOTE_SDP_MEDIA_CHANGE & bitmask)
					{
					/* update the MMS response for the stream */
						p_mms_resp->modified_stream_resp[resp_count].
								status_success = ICF_FALSE;
					}
					else
					if (ICF_LOCAL_SDP_MEDIA_CHANGE & bitmask)
					{
					/* update the MMS response for the stream */
						p_mms_resp->modified_stream_resp[resp_count].
								status_success = ICF_FALSE;
					}
					if (ICF_MODE_CHANGE & bitmask || 
              ICF_LOCAL_MEDIA_CONNECT_MODE_CHANGE & bitmask)
					{
					/* update the MMS response for the stream */
						p_mms_resp->modified_stream_resp[resp_count].
								status_success = ICF_FALSE;
					}
					break;
				}
				resp_count ++;
			}/* while ends */
		}/* for loop ends */
	}/* if rejected streams were present */

	/* Checking for number of codecs per stream:
	 * should match the commited codecs */

	for(resp_count = 0; resp_count < p_mms_resp->mod_stream_count;
		resp_count++)
	{
		accpt_stream_id = 
			p_mms_resp->modified_stream_resp[resp_count].stream_id;
		bitmask = p_mms_resp->modified_stream_resp[resp_count].bitmask;

		p_accpt_list_hdr = p_recv_msg_payload->p_accepted_stream_list;

		while(p_accpt_list_hdr)
		{
			p_stream_node = ICF_CMN_GET_DATA_NODE_FOR_LIST(p_accpt_list_hdr);

			if(p_stream_node)
			{
				if(accpt_stream_id == p_stream_node->stream_id)
				{
					if(ICF_FALSE == icf_mmi_validate_commited_no_of_codecs
										(p_mmi_pdb,
										accpt_stream_id,
										p_stream_node->local_sdp.p_codec_info_list))
					{
						/* check the bitmask for this stream */	
						if ((ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE & bitmask ) ||
				   			(ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & bitmask) ||
				   			((ICF_REMOTE_ANSWER_CODEC_CHANGE &  bitmask ) &&
							(p_mmi_ctxt->mmi_bitmask & ICF_MEDIA_ESTABLISHED)) ||				   		
				   			(ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE & bitmask)||
				   			(ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE & bitmask)||
							/* if restoration was in progress was this stream */
							(ICF_FALSE == 
								p_mms_resp->modified_stream_resp[resp_count].
								status_success))
						{
							ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
							return ret_val;	
						}
						else 
						if ((ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask) &&
							(!(ICF_MEDIA_ESTABLISHED & p_mmi_ctxt->mmi_bitmask)))
						{
							p_mms_resp->result = ICF_FAILURE;

							ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);
							return ret_val;
						}
						if (ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)
						{
						/* update the MMS response for the stream */
							p_mms_resp->modified_stream_resp[resp_count].
									status_success = ICF_FALSE;
						/* as a part of the restoration, we will send GCL for this
						 * stream */
						/* create GCL request for this stream */
							send_GCL = ICF_TRUE;					
						}
#if 0
						else
						if ((ICF_LOCAL_SDP_CODEC_CHANGE & bitmask)|| \
                                (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
						{
						/* update the MMS response for the stream */
							p_mms_resp->modified_stream_resp[resp_count].
									status_success = ICF_FALSE;
						/* as a part of the restoration, we will send commit for 
						 * this stream */
						/* create COMMIT request for this stream */
							send_COMMIT = ICF_TRUE;					
						}
#endif
						if (ICF_REMOTE_SDP_MEDIA_CHANGE & bitmask)
						{
						/* update the MMS response for the stream */
							p_mms_resp->modified_stream_resp[resp_count].
									status_success = ICF_FALSE;
						}
#if 0
						else
						if (ICF_LOCAL_SDP_MEDIA_CHANGE & bitmask)
						{
						/* update the MMS response for the stream */
							p_mms_resp->modified_stream_resp[resp_count].
									status_success = ICF_FALSE;
						}
						if (ICF_MODE_CHANGE & bitmask)
						{
						/* update the MMS response for the stream */
							p_mms_resp->modified_stream_resp[resp_count].
									status_success = ICF_FALSE;
						}
#endif
					}

					break;
				}
			}
			p_accpt_list_hdr = p_accpt_list_hdr->p_next;
		}
	}

	/* now check for the accepted streams in the request */
	p_accpt_list_hdr = p_recv_msg_payload->p_accepted_stream_list;

	/* fetch the payload, get the id from the node and check the bitmask
	 * for the stream in the mms_resp in the mmi ctxt. Now update the
	 * approp. SDP in CC */
	resp_count = 0;
	

	/* now fetch for this stream_id in the mms_resp in the mmi_ctxt */
	while ((resp_count < p_mms_resp->mod_stream_count) && 
			(p_accpt_list_hdr))
	{
		p_stream_node = ICF_CMN_GET_DATA_NODE_FOR_LIST(p_accpt_list_hdr);
	
		accpt_stream_id = p_stream_node->stream_id;

		bitmask = p_mms_resp->modified_stream_resp[resp_count].bitmask;
		/* now fetch the corresponding stream_id in the mms_resp */
		if (p_mms_resp->modified_stream_resp[resp_count].stream_id ==
			accpt_stream_id)
		{
			if(ICF_FAILURE == icf_mmi_validate_transport_addr(p_mmi_pdb, 
				&(p_stream_node->local_sdp.stream_address)))
			{
				icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb, 
					accpt_stream_id);

				ICF_ALARM(((icf_uint8_t *)"[MMI]: Corrupt Address in MMS resp"));

				resp_count++;
				p_accpt_list_hdr = ICF_CMN_GET_NXT_NODE_FOR_LIST(p_accpt_list_hdr);
				continue;
			}

			/* check the action for the stream in the MMS response for this 
			 * accepted stream */
			if ((ICF_LOCAL_SDP_MEDIA_CHANGE & bitmask)||
				(ICF_LOCAL_SDP_CODEC_CHANGE & bitmask) ||
				(ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE & bitmask)||
				(ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE & bitmask)||
                (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
			{
				if (ICF_FAILURE == icf_mmi_update_sdp_from_mms_resp_for_stream_id(
											p_mmi_pdb,
											accpt_stream_id,
											ICF_SDP_LOCAL,
											bitmask,
											p_stream_node))
				{
					ICF_ALARM(((icf_uint8_t *)"Failed to Update Local SDP from MMS response\n"));
					ret_val = ICF_FAILURE ;
				}

				if ((ICF_LOCAL_SDP_CODEC_CHANGE & bitmask) ||
					(ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE & bitmask)||\
                    (bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
				{
					icf_cmn_update_codecs_for_stream_id_in_call_ctxt(
							p_mmi_pdb->p_glb_pdb,
							accpt_stream_id,
							ICF_SDP_LOCAL,
							&p_stream_node->local_sdp);

				}
			}
			else
			/* for all left bitmask
				((ICF_REMOTE_SDP_MEDIA_CHANGE & bitmask)||
				(ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)||
			    (ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask)||
			    (ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE & bitmask)||
				(ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & bitmask))*/
			{
				/* this function checks if there is a valid pointer for the
				 * applied_sdp in the call_ctxt, if not- then allocate memory for
				 * the variable else simply return SUCCESS */
				if (ICF_FAILURE == icf_mmi_check_sdp_in_call_ctx(p_mmi_pdb,
											accpt_stream_id,
											ICF_SDP_APPLIED))
				{
					/* failed to fetch memory */	
					return ICF_FAILURE;
				}

				if (ICF_FAILURE == 
					icf_mmi_update_sdp_from_mms_resp_for_stream_id(
											p_mmi_pdb,
											accpt_stream_id,
											ICF_SDP_APPLIED,
											bitmask,
											p_stream_node))
				{
					ICF_ALARM(((icf_uint8_t *)"Failed to Update Applied SDP from MMS response\n"));
					return ICF_FAILURE;
				}
				
				if (ICF_FAILURE == 
					icf_mmi_update_sdp_from_mms_resp_for_stream_id(
											p_mmi_pdb,
											accpt_stream_id,
											ICF_SDP_LOCAL,
											bitmask,
											p_stream_node))
				{
					ICF_ALARM(((icf_uint8_t *)"Failed to Update Local SDP from MMS response\n"));
					return ICF_FAILURE;
				}

				if (ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask)
				{
					p_mmi_ctxt->mmi_bitmask |= ICF_MEDIA_ESTABLISHED;
				}
			}
		}
		resp_count++;
		p_accpt_list_hdr = ICF_CMN_GET_NXT_NODE_FOR_LIST(p_accpt_list_hdr);
	}/* while loop ends */		

	if (ICF_TRUE == send_GCL)
	{
		p_mmi_ctxt->mmi_bitmask |= ICF_RESTORATION_IN_PROGRESS;
		for (stream_count=0; stream_count <p_mms_resp->mod_stream_count; 
			stream_count ++)
		{
			if (p_mms_resp->modified_stream_resp[stream_count].status_success)
			{
				p_mms_resp->modified_stream_resp[stream_count].send_mms =
					ICF_FALSE;
			}
		}
		ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
		}
		else
		{
			icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
		}		
	}
	else
	if (ICF_TRUE == send_COMMIT)
	{
		p_mmi_ctxt->mmi_bitmask |= ICF_RESTORATION_IN_PROGRESS;
		
		for (stream_count=0; stream_count <p_mms_resp->mod_stream_count; 
			stream_count ++)
		{
			if (p_mms_resp->modified_stream_resp[stream_count].status_success)
			{
				p_mms_resp->modified_stream_resp[stream_count].send_mms =
					ICF_FALSE;
			}
		}
		ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_03_AWT_COMMIT_RESP)
		}
		else
		{
			icf_mmi_clear_buffered_req_resp_in_mmi_ctx(p_mmi_pdb);
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
		}
	}	
	else
	/* If MMS has been successful, then check if ADD is pending, so send GCL 
	 * for those streams */
	if (ICF_MMI_MMS_ACTION_ADD & 
		((icf_mmi_mms_request_st *)p_mmi_pdb->p_mmi_ctx->p_mms_req)->bit_mask)
	{
		/* create GCL request for the stream_id's in the add_request */

		p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_MMI_MMS_ADD_ONGOING;
		ret_val = icf_mmi_send_gcl_req(p_mmi_pdb);

		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_01_AWT_GCL_RESP)
		}
	}
	else
	{
		ICF_MMI_RESET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)
		icf_mmi_send_modify_media_response(p_mmi_pdb);
	}

	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;	
}/* function ends */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_sdp_from_mms_resp_for_stream_id
 *
 * DESCRIPTION:                      
 *
 *****************************************************************************/
icf_return_t icf_mmi_update_sdp_from_mms_resp_for_stream_id
(
	icf_mmi_pdb_st		*p_mmi_pdb,
	icf_stream_id_t		stream_id,
	icf_uint8_t			sdp_type,
	icf_uint32_t		bitmask,
	icf_resp_stream_st	*p_stream_node
)
{
	/* local variables */
	icf_return_t						ret_val = ICF_FAILURE;
	icf_sdp_stream_st					sdp;

	icf_port_memset(&sdp, 0, ICF_PORT_SIZEOF(sdp));

	switch (sdp_type)
	{
		case ICF_SDP_LOCAL:
		{
			/* update parameters from the MMS response into the local SDP
			 * donot update the codec(s) so far in the local SDP. */
			/* donot update the IP/port here for local SDP, that is already
			 * updated in the open_channel_response and CMS response */
			
			/* Puneet: iam updating IP/PORT also seems logical to me coz
			 * otherwise IP/PORT should also not be updated in APPLIED */
			ret_val = icf_cmn_update_media_params_for_stream_id_in_call_ctxt(
							p_mmi_pdb->p_glb_pdb,
							stream_id,
							ICF_SDP_LOCAL,
							&p_stream_node->local_sdp);

			break;
		}


		case ICF_SDP_APPLIED:
		{
			if	((ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)||
				(ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask)||
				(ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE & bitmask)||
				(ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & bitmask))
			{
				if(ICF_STREAM_TYPE_TBCP != 
				icf_mmi_get_stream_type_for_stream_id(p_mmi_pdb,stream_id))
				{
					/* update codec(s) from the MMS response. */
					ret_val = 
					
					icf_cmn_update_codecs_for_stream_id_in_call_ctxt(
							p_mmi_pdb->p_glb_pdb,
							stream_id,
							sdp_type,
							&p_stream_node->local_sdp);
				}
			}

			if	((ICF_REMOTE_SDP_MEDIA_CHANGE & bitmask)||
				(ICF_REMOTE_SDP_CODEC_CHANGE & bitmask)||
				(ICF_REMOTE_ANSWER_CODEC_CHANGE & bitmask))
			{
				/* populate the rest of the parameters including IP/Port 
				* from the REMOTE_SDP */

				/* fetch the remote SDP from the CC ctxt */
				if (ICF_SUCCESS == icf_mmi_get_sdp_for_stream_id(
									p_mmi_pdb,
									stream_id,
									ICF_SDP_REMOTE,
									&sdp))
				{
					ret_val =
					
					icf_cmn_update_media_params_for_stream_id_in_call_ctxt(
						p_mmi_pdb->p_glb_pdb,
						stream_id,
						sdp_type,
						&sdp);
				}
			}
			else
			if ((ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE & bitmask) ||
			   (ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & bitmask))	
			{
				/* populate the rest of the parameters including IP/Port 
				* from the LAST_COMMITTED_SDP */
				/* fetch the last commited SDP from the CC ctxt */
				if (ICF_SUCCESS == icf_mmi_get_sdp_for_stream_id(
									p_mmi_pdb,
									stream_id,
									ICF_SDP_LAST_COMMITED,
									&sdp))
				{
					ret_val =

					icf_cmn_update_media_params_for_stream_id_in_call_ctxt(
						p_mmi_pdb->p_glb_pdb,
						stream_id,
						sdp_type,
						&sdp);
				}
			}
		}
		break;
	}

	if(ICF_MODE_CHANGE == bitmask || 
    ICF_LOCAL_MEDIA_CONNECT_MODE_CHANGE == bitmask)
	{
		ret_val = ICF_SUCCESS;
	}

	return ret_val;
}



