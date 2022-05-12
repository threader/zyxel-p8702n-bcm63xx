/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_03_awt_commit_resp.c
 * 
 * DESCRIPTION: This file contains functions to handle events in the AWT COMMIT 
 * 				response state in MMI.  
 *              
 * REVISION HISTORY:    
 *   Date            Name          Reference       Comments
 * 25-july-2006     Puneet       ICF Rel 6.0    Initial Version- support for
 * 25-Aug-2006      Amit Sharma    SPR 12554
 *                  Umang Singh
 * 												multiple m-lines
 * 11-Apr-2007      Deepti Goyal   SPR 14905    Codec change in media connect req.
 * 24-Jan-2008      Abhishek Dhammawat Rel 8.0  Modified
 *                                              icf_mmi_commit_success_resp
 * 4-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 22-Apr-2008      Shruti Thakral SPR 18455    Klocwork Warning Removal Rel 8.0
 * 26-Jun-2008      Amit Sharma    SPR 18725      Populated warning codes
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349      Klocworks fix
 * 						
 * Copyright (c) 2008, Aricent.  
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
 * FUNCTION:        icf_mmi_03_dms_request
 *
 * DESCRIPTION:     This function is invoked when the MMI is 
 *                  awaiting COMMIT resp from RM and it receives a DMS request
 *                  Current State: ICF_MMI_03_AWT_COMMIT_RESP
 *                  Current Event: ICF_MMI_DMS_REQ
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                   
 *
 *****************************************************************************/
icf_return_t icf_mmi_03_dms_request
(
	INOUT icf_mmi_pdb_st       *p_mmi_pdb
)
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

        /* If this COMMIT was sent during call setup. i.e. FS INC call then no 
		 * need to send DMS_REQ */

		/* If call is incoming and FS */
		if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			icf_mmi_send_rel_res_ind(p_mmi_pdb);
			/* No need to Send CLOSE_CHANNEL and REL_PUBLIC_IP_PORT besause 
			 * COMMIT not done yet */
			ICF_MMI_SET_STATE(p_mmi_pdb,ICF_MMI_00_IDLE)
		}
		else
		{
			icf_mmi_send_forceful_dms_req(p_mmi_pdb);
        }
    }   
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_03_commit_resp
 *
 * DESCRIPTION:     This function is invoked when the MMI is
 *                  awaiting COMMIT resp from RM and it receives a COMMIT 
 *                  response from RM
 *                  Current State: ICF_MMI_03_AWT_COMMIT_RESP
 *                  Current Event: ICF_MMI_COMMIT_RESP
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

icf_return_t icf_mmi_03_commit_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_codec_commit_resp_st       *p_recv_msg_payload = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    /* Request DBM for the ctx with the call id in the recvd msg */
    
    /* Check the call state of CC. If CC is in AWT DMS state then we need to 
     * clear the session
     */
   
    if(ICF_CC_07_AWT_DMS_RESP == p_mmi_pdb->p_call_ctx->call_state)
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_CC_AWT_DMS)
            
        /* If the media session is already established, then send DMS request.
         * This means that CMS response is received and we have a media 
         * session.In this case MMI can be in AWT commit response state
         * in case of MMS req for remote media application(more than one
         * local codecs.
         */ 
        if(ICF_MEDIA_ESTABLISHED & p_mmi_pdb->p_mmi_ctx->mmi_bitmask)
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MEDIA_ESTABLISHED)
			ret_val = icf_mmi_send_awaited_dms_req(p_mmi_pdb);
        }/* end if to check media established */
        else
        {
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_MEDIA_NOT_ESTABLISHED)
            /* No session is created at present so only send release resource */     
			icf_mmi_send_rel_res_ind(p_mmi_pdb);
			ret_val = icf_mmi_send_delete_media_response(p_mmi_pdb);
        }/* End else */
        
        /* No further processing is needed if CC is in AWT DMS state */
        return ret_val;
    }/* end if */ 
    
    p_recv_msg_payload = (icf_codec_commit_resp_st *)
        (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);
    
   
    /* If success Response is recvd then function to process success response 
     * is called
     */
    if(ICF_SUCCESS == p_recv_msg_payload->result)
    {
#ifndef ICF_UNIT_TEST
        ret_val = icf_mmi_check_api_total_streams(p_mmi_pdb, 
                    ICF_CODEC_COMMIT_RESP);
		
        if(ICF_FAILURE == ret_val)
        {
            p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

            ICF_ALARM(((icf_uint8_t *)"[MMI]: MEDIA MANAGER failure"));
            icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
            return ICF_SUCCESS;
        }
#endif
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_SUCCESS_RESP)
            
        ret_val = icf_mmi_commit_success_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* failure to process */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }
    
    /* If failure response is recvd then function to process failure response 
     * is called
     */
    else  
    {
        ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_RESP)

        p_mmi_pdb->p_call_ctx->warning_header_code = 
            ICF_WARNING_CODE_INCOMPATIBLE_MEDIA_FMT;

#ifdef ICF_TRACE_ENABLE
        p_mmi_pdb->stream_type = 0;
        p_mmi_pdb->result = ICF_FAILURE;
        /*ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_COMMIT)*/		
#endif
            
        ret_val = icf_mmi_commit_failure_resp(p_mmi_pdb);
        if(ICF_FAILURE == ret_val)
        {
            /* Failure to process */
            ICF_MMI_TRACE(p_mmi_pdb,ICF_TRACE_FAILURE_TO_PROCESS_MSG)
        }
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_commit_success_resp
 * 
 * DESCRIPTION:     This function processes the success response of commit.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_commit_success_resp
(
     INOUT icf_mmi_pdb_st             *p_mmi_pdb
)
{
    icf_return_t                        ret_val = ICF_FAILURE,
										dummy_ret_val = ICF_FAILURE;
	icf_codec_commit_resp_st			*p_recv_msg_payload = ICF_NULL;
	icf_mmi_internal_mms_resp_st		*p_mms_resp = ICF_NULL;
	icf_mmi_internal_cms_resp_st		*p_cms_resp = ICF_NULL;
	icf_stream_id_t						stream_count = ICF_NULL,
										stream_id = ICF_NULL;
	icf_media_type_t					stream_type = ICF_STREAM_TYPE_INVALID;
	icf_boolean_t						initiate_call_clear = ICF_FALSE,
										send_mms = ICF_FALSE,
										send_commit = ICF_FALSE,
										codec_count_mismatch = ICF_FALSE;
	icf_uint32_t						bitmask = ICF_NULL,
										loc_stream= ICF_NULL;
	icf_stream_id_t 					loc_count1 = ICF_NULL,
										loc_count2 = ICF_NULL;
	/*icf_codec_for_commit_st				commit[ICF_MAX_STREAM_PER_CALL];*/
    icf_codec_for_commit_st				*commit = ICF_NULL;
	icf_stream_sdp_info_st				*p_stream_sdp = ICF_NULL;
	icf_internal_list_st				*p_node = ICF_NULL;

    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

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
        if(ICF_NULL != commit)
        {
            ICF_MEMFREE(
                p_mmi_pdb->p_glb_pdb,
                commit,
                ICF_MEM_COMMON,
                p_mmi_pdb->p_ecode,
                dummy_ret_val)
        }
        return ret_val;        
    }/* End if to check CC state */

    p_recv_msg_payload = (icf_codec_commit_resp_st *)
            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

    ICF_MEMGET(p_mmi_pdb->p_glb_pdb,
			ICF_MAX_STREAM_PER_CALL * ICF_PORT_SIZEOF(icf_codec_for_commit_st),
			ICF_MEM_COMMON,
			commit,
			ICF_RET_ON_FAILURE,
			p_mmi_pdb->p_ecode,
			dummy_ret_val)
 
#ifdef ICF_TRACE_ENABLE
    /* Start traces for data received in CMS Response */
    p_mmi_pdb->p_mmi_msg_payload = (icf_void_t *)p_recv_msg_payload;

	icf_mmi_trace_api_sdp(p_mmi_pdb, ICF_CODEC_COMMIT_RESP);
#endif

	/* if the streams are being restored following commit failure, we need
     * to commit again for the older codecs. In which case we need not send 
     * MMS request to MM, for the restored streams. But if during restoration
	 * the commit response is not equal to the initially committed codecs
	 * then MMI needs to send MMS for the newly committed codecs */
	/* Save local backup of codec_for_commit */
	for(stream_count =0; (stream_count < p_mmi_pdb->p_mmi_ctx->stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
		stream_count++)
	{
		commit[stream_count] = 
					p_mmi_pdb->p_mmi_ctx->codec_for_commit[stream_count];
	}

	p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;
	dummy_ret_val = icf_mmi_update_ctx_from_commit_resp(
							p_mmi_pdb, 
							p_recv_msg_payload->p_commited_stream_list);

	if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & ICF_COMMIT_RESTORATION_IN_PROGRESS)
	{
		for(loc_count1 = 0; (loc_count1 < p_mms_resp->mod_stream_count) && (loc_count1 < ICF_MAX_STREAM_PER_CALL); loc_count1++)
		{
			for(loc_count2 = 0; (loc_count2 < p_mms_resp->mod_stream_count) && (loc_count2 < ICF_MAX_STREAM_PER_CALL);
				loc_count2++)
			{
				if(commit[loc_count1].stream_id == 
					p_mmi_pdb->p_mmi_ctx->codec_for_commit[loc_count2].stream_id)
				{
					if(commit[loc_count1].codec_count != 
						p_mmi_pdb->p_mmi_ctx->codec_for_commit[loc_count2].codec_count)
					{
						codec_count_mismatch = ICF_TRUE;
					}
					else
					{
						icf_mmi_unmark_send_mms_in_mms_resp(p_mmi_pdb, 
							commit[loc_count1].stream_id);
					}

					break;
				}
			}
		}

		if(ICF_TRUE == codec_count_mismatch)
		{
			send_mms = ICF_TRUE;
			ret_val = ICF_SUCCESS;
		}
		else
		{
			/* update local sdp for codecs only from last commited for only
			 * those codec no.s returned in the commmit resp */

			for(stream_count = 0; (stream_count < p_mmi_pdb->p_mmi_ctx->stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
				stream_count++)
			{
				loc_stream = 
					p_mmi_pdb->p_mmi_ctx->codec_for_commit[stream_count].stream_id;

				p_node = 
					icf_cmn_get_node_in_internal_list(
						p_mmi_pdb->p_call_ctx->p_stream_sdp_info,
						&loc_stream);

				if(ICF_NULL != p_node)
				{
					p_stream_sdp = p_node->p_payload;
				}

				if ((ICF_NULL != p_stream_sdp) && 
					(p_stream_sdp->p_last_commited_sdp))
				{
                    /*
					icf_mmi_clone_codec_info_list(
							p_mmi_pdb,
							p_stream_sdp->p_last_commited_sdp->sdp_stream.p_codec_info_list,
							&p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list);
                    */
                    icf_cmn_clone_codec_attrib_list(
							p_mmi_pdb->p_glb_pdb,
							&p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list, 
                            p_stream_sdp->p_last_commited_sdp->sdp_stream.p_codec_info_list
);

					/* This will check that only those codecs remain which are 
					 * in commited list */
					icf_mmi_check_sdp_for_commited_codecs(
							p_mmi_pdb,
							&p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list);
                    ICF_KLOCWORK_FALSE_POSITIVE(p_mmi_pdb->p_glb_pdb,
                            p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list)
				}
			}


			for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
				stream_count++)
			{
				if(ICF_TRUE == 
					p_mms_resp->modified_stream_resp[stream_count].status_success)
				{
					ret_val = ICF_SUCCESS;
					send_mms = ICF_TRUE;
					break;
				}
			}
		}
	}
	else
	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		/* if any stream failure in case of RESTORATION or REVERT or ANSWER 
		 * for OTG Re-INVITE than initiate call clear */
		if(p_recv_msg_payload->bit_mask & 
				ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
		{
			if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & ICF_RESTORATION_IN_PROGRESS)
			{
				initiate_call_clear = ICF_TRUE;
			}
			else
			{
				for(stream_count = 0; 
						(stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
						stream_count++)
				{
					stream_id = 
					p_mms_resp->modified_stream_resp[stream_count].stream_id;
					
					bitmask = 
						p_mms_resp->modified_stream_resp[stream_count].bitmask;

					dummy_ret_val = icf_mmi_search_failure_stream(p_mmi_pdb, 
								&p_recv_msg_payload->rejected_stream_list,
								stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
#ifdef ICF_TRACE_ENABLE
						p_mmi_pdb->stream_type = 
							p_mms_resp->modified_stream_resp[stream_count].stream_type;
						p_mmi_pdb->result = ICF_FAILURE;
						/*ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_COMMIT)*/
#endif
						icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb, 
										stream_id);

						if((bitmask & ICF_REMOTE_ANSWER_CODEC_CHANGE)||
						   (bitmask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
						   (bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE))
						{
							initiate_call_clear = ICF_TRUE;
						}
					}
				}
			}
		}

		if(ICF_TRUE == initiate_call_clear)
		{
			/* send mm_err_ind to CC for call clear */
			ret_val = icf_mmi_send_err_ind_call_clear(p_mmi_pdb);
            if(ICF_NULL != commit)
            {
                ICF_MEMFREE(
                    p_mmi_pdb->p_glb_pdb,
                    commit,
                    ICF_MEM_COMMON,
                    p_mmi_pdb->p_ecode,
                    dummy_ret_val)
            }
			return ret_val;
		}

		for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			stream_id = 
					p_mms_resp->modified_stream_resp[stream_count].stream_id;
			stream_type = 
					p_mms_resp->modified_stream_resp[stream_count].stream_type;

			bitmask = p_mms_resp->modified_stream_resp[stream_count].bitmask;

			if((bitmask & ICF_REMOTE_SDP_CODEC_CHANGE)||(bitmask & ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
			{
				if(p_recv_msg_payload->bit_mask & 
					ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
				{
					if(ICF_SUCCESS == icf_mmi_search_failure_stream(p_mmi_pdb, 
										&p_recv_msg_payload->rejected_stream_list,
										stream_id))
					{
						ret_val = ICF_SUCCESS;

						if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & 
										ICF_RESTORATION_IN_PROGRESS)
						{
							/* Make MMS payload codecs from last_commited for this 
							 * stream Populate rest parameters from local SDP */
							send_mms = ICF_TRUE;
							/* SDP has been restored for this stream */
						}
						else
						{
							/* Make COMMIT req from last commited SDP for this 
							 * stream */
							icf_mmi_mark_failure_stream_mms_mod_resp(p_mmi_pdb,stream_id);
							send_commit = ICF_TRUE;
						}
					}
					else if(ICF_STREAM_TYPE_TBCP != stream_type)
					{
						send_mms = ICF_TRUE;
						ret_val = ICF_SUCCESS;
					}
				}
				else if(ICF_STREAM_TYPE_TBCP != stream_type)
				{
					send_mms = ICF_TRUE;
					ret_val = ICF_SUCCESS;
				}
			}
			else if((bitmask & ICF_REMOTE_ANSWER_CODEC_CHANGE)||
					(bitmask & ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
					(bitmask & ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE))
			{
				send_mms = ICF_TRUE;
				ret_val = ICF_SUCCESS;
			}
		}
	}	
	else/* CMS_ONGOING - incoming FS call/Incoming Re-Invite for T.38 */
	{
		/* Marking failure streams */
		if(p_recv_msg_payload->bit_mask & 
						ICF_CODEC_COMMIT_RESP_REJ_STREAM_LIST_PRESENT)
		{
			if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
			{
				p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;
		
				/* Mark the failure streams in CMS_resp in MMI ctxt*/
				for(stream_count = 0; (stream_count < p_cms_resp->count) && (stream_count < ICF_MAX_STREAM_PER_CALL);	
					stream_count++)
				{
					dummy_ret_val = 
						icf_mmi_search_failure_stream(p_mmi_pdb, 
							&p_recv_msg_payload->rejected_stream_list,
							p_cms_resp->stream_status[stream_count].stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						p_cms_resp->stream_status[stream_count].status_success =
							ICF_FALSE;
#ifdef ICF_TRACE_ENABLE
						p_mmi_pdb->stream_type = 
							p_cms_resp->stream_status[stream_count].stream_type;
						p_mmi_pdb->result = ICF_FAILURE;
						/*ICF_MMI_APP_TRACE(p_mmi_pdb,ICF_TRACE_CODEC_COMMIT)*/
#endif
					}
				}
			}
			else if(ICF_MMI_MMS_ADD_ONGOING == 
							p_mmi_pdb->p_mmi_ctx->ongoing_event)
			{
				p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

				/* Mark the failure streams in MMS_resp in MMI ctxt*/
				for(stream_count = 0; (stream_count<p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL)
					;stream_count++)
				{
					dummy_ret_val = 
						icf_mmi_search_failure_stream(p_mmi_pdb, 
						&p_recv_msg_payload->rejected_stream_list,
						p_mms_resp->added_stream_resp[stream_count].stream_id);

					if(ICF_SUCCESS == dummy_ret_val)
					{
						p_mms_resp->added_stream_resp[stream_count].status_success = 
								ICF_FALSE;
					}
				}
			}
			/* Send REL_RES_IND for failure streams */
			icf_mmi_send_rel_res_ind(p_mmi_pdb);
		}

#if 0
#ifdef ICF_TRACE_ENABLE
		else
		{
			if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
            {
				icf_list_st *p_accpt_list = 
					((icf_codec_commit_resp_st *)p_mmi_pdb->p_glb_pdb->p_recv_msg->\
           			 payload)->p_commited_stream_list;

                p_cms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

				while(p_accpt_list)
				{
					 for(stream_count = 0; stream_count < p_cms_resp->count;
                   		 stream_count++)
                	{
						if(p_cms_resp->stream_status[stream_count].stream_id ==
							((icf_stream_commit_st*)p_accpt_list->p_data)->stream_id)
						{
							p_mmi_pdb->result = ICF_SUCCESS;
						}
					}
				}

                /* Mark the failure streams in CMS_resp in MMI ctxt*/
                for(stream_count = 0; stream_count < p_cms_resp->count;
                    stream_count++)
                {
                    dummy_ret_val =
                        icf_mmi_search_failure_stream(p_mmi_pdb,
                            &p_recv_msg_payload->rejected_stream_list,
                            p_cms_resp->stream_status[stream_count].stream_id);

                    if(ICF_SUCCESS == dummy_ret_val)
                    {
                        p_cms_resp->stream_status[stream_count].status_success =
                            ICF_FALSE;
                    }
                }
            }
            else if(ICF_MMI_MMS_ADD_ONGOING ==
                            p_mmi_pdb->p_mmi_ctx->ongoing_event)
            {
                p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

                /* Mark the failure streams in MMS_resp in MMI ctxt*/
                for(stream_count = 0; stream_count<p_mms_resp->add_stream_count
                    ;stream_count++)
				{

				}
			}	
		}
#endif
#endif

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
		if((!(ICF_CALL_CONNECT_DONE & p_mmi_pdb->p_call_ctx->common_bitmask))&&
			(ICF_SUCCESS == icf_mmi_qos_check_av_stream(p_mmi_pdb)))
		{	
           /* Rel8.0: This is hook for resource reservation. Previously 
              icf_mmi_send_qos_reserve_req function was invoked here.
              However now we do not require to do PDP activation for IMS
              mode and keeping behaviour same for both the IMS/NON IMS
              mode we invoke non ims function*/
			ret_val = icf_mmi_send_qos_reserve_res_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				/* change state */
				ICF_MMI_SET_STATE(p_mmi_pdb,
						 ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP)

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
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)

		ret_val = icf_mmi_send_open_channel_req(p_mmi_pdb);
		
		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_06_AWT_OPEN_CHANNEL_RESP)
		}

ICF_CHECK_QOS_SUPPORT_END
#else
		ret_val = icf_mmi_send_open_channel_req(p_mmi_pdb);
		
		if(ICF_SUCCESS == ret_val)
		{
			ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_06_AWT_OPEN_CHANNEL_RESP)
		}
#endif
	}	

	if(ICF_SUCCESS == ret_val)
	{
		if(ICF_TRUE == send_commit)
		{
			ICF_MMI_SET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)

			ICF_MMI_SET_BIT(p_mmi_pdb, ICF_COMMIT_RESTORATION_IN_PROGRESS)

			ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_03_AWT_COMMIT_RESP)
			}
		}
		else
		if(ICF_TRUE == send_mms)
				 /*	and also in case this is the 
				  * commit response for restoration so now codecs have been 
				  * restored and MMS shall be sent with reverted and applied 
				  * codecs and params */
		{
				/* MMI shall take action depending on the bitmask as follows
				 * if(bitmask & ICF_REMOTE_SDP_MEDIA_CHANGE)
				 * {
				 *    Fill MMS payload for parameters except codecs from
				 *    remote recvd SDP for this stream
				 *    Fill codecs from applied SDP
			     * } 
				 * else if((bitmask & ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE)||
				 *		   (bitmask & ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE))
				 * {
				 *	  Fill MMS payload for parameters except codecs from last 
				 *	  commited SDP for this stream
				 *	  Fill codecs from applied SDP
				 * }
				 * if(bitmask & ICF_MODE_CHANGE)
				 * {
				 *    Fill mode in MMS payload from mm_mode_to_be_applied
				 * }
				 */

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_TRUE)
			if((!(ICF_CALL_CONNECT_DONE & p_mmi_pdb->p_call_ctx->common_bitmask))&&
				(ICF_SUCCESS == icf_mmi_qos_check_av_stream(p_mmi_pdb)))
			{
                /* Rel8.0: This is hook for resource reservation. Previously 
                   icf_mmi_send_qos_reserve_req function was invoked here.
                   However now we do not require to do PDP activation for IMS
                   mode and keeping behaviour same for both the IMS/NON IMS
                   mode we invoke non ims function*/
				ret_val = icf_mmi_send_qos_reserve_res_req(p_mmi_pdb);
				if(ICF_SUCCESS == ret_val)
				{
					/* change state */
					ICF_MMI_SET_STATE(p_mmi_pdb,
							ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP)

				}
			}
			else
			{
				/* send MMS */
				ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
				if(ICF_SUCCESS == ret_val)
				{
					ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_04_AWT_MMS_RESP)
				}
			}
ICF_CHECK_QOS_SUPPORT_END
ICF_CHECK_QOS_SUPPORT_START(p_mmi_pdb->p_glb_pdb,ICF_FALSE)
			/* send MMS */
			ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_04_AWT_MMS_RESP)
			}

ICF_CHECK_QOS_SUPPORT_END
#else
			/* send MMS */
			ret_val = icf_mmi_send_mms_req(p_mmi_pdb);
			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_04_AWT_MMS_RESP)
			}
#endif
		}
		else
		{
			ICF_MMI_RESET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)
		}
		
	}
	else
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
    if(ICF_NULL != commit)
    {
        ICF_MEMFREE(
                    p_mmi_pdb->p_glb_pdb,
                    commit,
                    ICF_MEM_COMMON,
                    p_mmi_pdb->p_ecode,
                    dummy_ret_val)
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        icf_mmi_commit_failure_resp
 *
 *  
 * DESCRIPTION:     This function processes the failure response of commit.
 *                    
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_mmi_commit_failure_resp(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
        icf_return_t                        ret_val = ICF_SUCCESS;
	icf_mmi_internal_mms_resp_st	    *p_mms_resp = ICF_NULL;
	icf_stream_id_t			    stream_count = ICF_NULL;
	icf_boolean_t			    initiate_call_clear = ICF_FALSE,
          						send_commit = ICF_TRUE;
        icf_codec_commit_resp_st            *p_recv_msg_payload = ICF_NULL;

 
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

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

	if(ICF_MMI_MMS_MOD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

		if(p_mmi_pdb->p_mmi_ctx->mmi_bitmask & ICF_RESTORATION_IN_PROGRESS)
		{
			initiate_call_clear = ICF_TRUE;
		}
		else
		{
			for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL);
					stream_count++)
			{
				if((ret_val == ICF_SUCCESS)&&
				   ((p_mms_resp->modified_stream_resp[stream_count].bitmask & 
										ICF_REMOTE_ANSWER_CODEC_CHANGE)||
				   (p_mms_resp->modified_stream_resp[stream_count].bitmask & 
										ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE)||
		           (p_mms_resp->modified_stream_resp[stream_count].bitmask & 
										ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE)))
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

		for(stream_count = 0; (stream_count < p_mms_resp->mod_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL); 
			stream_count++)
		{
			if((p_mms_resp->modified_stream_resp[stream_count].bitmask & 
					ICF_REMOTE_SDP_CODEC_CHANGE)||\
                    (p_mms_resp->modified_stream_resp[stream_count].bitmask & \
                    ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE))
			{
				if(ICF_TRUE == 
				p_mms_resp->modified_stream_resp[stream_count].status_success)
				{
					p_mms_resp->modified_stream_resp[stream_count].status_success
						= ICF_FALSE;
					send_commit = ICF_TRUE;
				}
			}
		}

		if(ICF_TRUE == send_commit)
		{
			ICF_MMI_SET_BIT(p_mmi_pdb, ICF_RESTORATION_IN_PROGRESS)

			ICF_MMI_SET_BIT(p_mmi_pdb, ICF_COMMIT_RESTORATION_IN_PROGRESS)

			/* Make COMMIT req from last commited SDP for this stream */

			ret_val = icf_mmi_send_commit_req(p_mmi_pdb);

			if(ICF_SUCCESS == ret_val)
			{
				ICF_MMI_SET_STATE(p_mmi_pdb, ICF_MMI_03_AWT_COMMIT_RESP)
			}
		}
    }
	else if(ICF_MMI_MMS_ADD_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
	{
		p_mms_resp = p_mmi_pdb->p_mmi_ctx->p_mmi_resp;

                p_recv_msg_payload = (icf_codec_commit_resp_st *)
                            (p_mmi_pdb->p_glb_pdb->p_recv_msg->payload);

                /* SPR 17399 : Send release cause to send 488
                 */
                if((ICF_CAUSE_RES_NOT_AVAILABLE_FOR_T38_FAX == 
                      p_recv_msg_payload->error_cause) &&
                      (ICF_NULL == p_mmi_pdb->p_call_ctx->release_cause))
                {
                      p_mmi_pdb->p_call_ctx->release_cause = 
                           ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE_FOR_T38_FAX;

                      p_mmi_pdb->p_call_ctx->release_type |= 
                          ICF_REL_TYPE_INTERNAL_INITIATED;
                }
                icf_mmi_send_rel_res_ind(p_mmi_pdb);

                ret_val = ICF_FAILURE;
       }                
       else if(ICF_MMI_CMS_ONGOING == p_mmi_pdb->p_mmi_ctx->ongoing_event)
       {
		/* send failure cms_resp to CC */
		((icf_mmi_internal_cms_resp_st *)p_mmi_pdb->p_mmi_ctx->p_mmi_resp)->\
			result = ICF_FAILURE;

		ret_val = ICF_FAILURE;
	}

	if(ICF_FAILURE == ret_val)
	{
		switch(p_mmi_pdb->p_mmi_ctx->ongoing_event)
		{
			case ICF_MMI_CMS_ONGOING:
				
				ret_val = icf_mmi_send_create_media_response(p_mmi_pdb);
				
				break;

			case ICF_MMI_MMS_ADD_ONGOING:

			case ICF_MMI_MMS_MOD_ONGOING:
				/* Update MM_resp in MMI ctxt with failure for add action and 
				 * send MM_resp to CC */

				if(ICF_NULL == p_mms_resp)
				{
					ret_val = ICF_FAILURE;
					break;
				}

				for(stream_count = 0; (stream_count<p_mms_resp->add_stream_count) && (stream_count < ICF_MAX_STREAM_PER_CALL) ;
					stream_count++)
				{
					p_mms_resp->added_stream_resp[stream_count].status_success =
							ICF_FALSE;
				}

				ret_val = icf_mmi_send_modify_media_response(p_mmi_pdb);

				break;
		}
	}

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}/* End function */    

  
/******************************************************************************
 *
 * FUNCTION:        icf_mmi_update_ctx_from_commit_resp
 * 
 * DESCRIPTION:            
 *
 ******************************************************************************/
					
icf_return_t	icf_mmi_update_ctx_from_commit_resp
 (
	icf_mmi_pdb_st		*p_mmi_pdb, 
	icf_list_st			*p_list
)
{
	icf_return_t			ret_val = ICF_SUCCESS;
	icf_stream_commit_st	*p_data = ICF_NULL;
	icf_mmi_context_st		*p_ctx = p_mmi_pdb->p_mmi_ctx;
	icf_uint8_t				codec_count = ICF_NULL;
	icf_stream_id_t			stream_count = ICF_NULL;

	ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)

	p_ctx->stream_count = ICF_NULL;

	while(ICF_NULL != p_list)
	{
		stream_count = p_ctx->stream_count;

		p_data = p_list->p_data;

		if ( stream_count < ICF_MAX_STREAM_PER_CALL ) 
		{
		p_ctx->codec_for_commit[stream_count]. stream_id =
			p_data->stream_id;

		p_ctx->codec_for_commit[stream_count].codec_count =
			p_data->codec_count;
		for(codec_count = 0; (codec_count < p_data->codec_count) && (codec_count < ICF_MAX_CODECS); codec_count++)
		{
			if(p_data->codec[codec_count].bit_mask & 
					ICF_CODEC_ID_CODEC_ID_PRESENT)
			{
				p_ctx->codec_for_commit[stream_count].codec_info[codec_count].
					codec_num = p_data->codec[codec_count].codec_id;
				p_ctx->codec_for_commit[stream_count].codec_info[codec_count].
					bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
			}
			if(p_data->codec[codec_count].bit_mask & 
					ICF_CODEC_ID_CODEC_NAME_PRESENT)
			{
				p_ctx->codec_for_commit[stream_count].codec_info[codec_count].
					codec_name.str_len = 
					p_data->codec[codec_count].codec_name.str_len;
				
				icf_port_strcpy(
				p_ctx->codec_for_commit[stream_count].codec_info[codec_count].
				codec_name.str,
				p_data->codec[codec_count].codec_name.str);
			}
		}
		}
		p_ctx->stream_count++;

		p_list = p_list->p_next;
	}
	ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)

	return ret_val;
}
