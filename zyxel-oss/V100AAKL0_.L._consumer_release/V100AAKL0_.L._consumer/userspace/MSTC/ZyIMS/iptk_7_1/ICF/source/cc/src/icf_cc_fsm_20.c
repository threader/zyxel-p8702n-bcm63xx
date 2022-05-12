/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_20.c
 *
 * Purpose          : This file contains all the handling of all valid events in  
 *                    wait for early call indication state
 *
 * Revision History :
 *
 * Date         Name            Ref#        Comments
 * --------     ------------    ----        ---------------
 * 16-02-2007  Vidyut Gupta   ICF_LLD  File created and initial version.
 * 19-03-2207  Rajat Chandna           Added func. icf_cc_20_pa_call_term_req 
 *                                     as a part of response code enhancement.   *                              
 * 03-04-2007  Rajat Chandna           Changes done for wildcarded user enhance- *                                     ment.
 * 17-04-2007  Rajat Chandna           Changes for addition of services in early *                                     call ind as a part of wildcarded user en. *                                            
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_early_ind_cfm
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in AWT_EARLY_IND_CFM state and it receives a setup
 *                  request from PA
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_EARLY_CALL_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_20_early_ind_cfm(
        INOUT icf_cc_pdb_st *p_cc_pdb)  
{
	icf_return_t				 ret_val = ICF_SUCCESS;
    icf_early_incoming_call_cfm_st		 *p_early_call_cfm = ICF_NULL;
	icf_line_data_st			 *p_line_data = ICF_NULL;
	icf_config_data_st			 *p_config_data = ICF_NULL;
	icf_uint32_t				  status = ICF_STATUS_CONTINUE;
    icf_internal_list_st	     *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_data = ICF_NULL;
    icf_boolean_t                    atleast_one_hit = ICF_FALSE;
    icf_boolean_t                 wildcard_user_present = ICF_FALSE;
    icf_user_db_node_st          *p_usr_db_node = ICF_NULL;
    icf_line_id_t                 line_id = ICF_NULL;
    icf_boolean_t                 user_found = ICF_FALSE;
    icf_boolean_t                 user_not_found_on_any_line = ICF_FALSE;
    icf_uint8_t                   trigger_cause = ICF_CAUSE_LINE_FREE;
    icf_address_st                 search_address;
    icf_call_ctx_st              *p_call_ctx = ICF_NULL;


#ifdef ICF_SESSION_TIMER
    icf_internal_msg_st internal_msg; 
     internal_msg.msg_hdr.msg_id = ICF_SSA_SESSION_TIMER_GET;
     internal_msg.msg_hdr.payload_length = ICF_NULL;
     internal_msg.p_msg_data = ICF_NULL;
     internal_msg.msg_hdr.p_glb_pdb = p_cc_pdb->p_glb_pdb; 
     internal_msg.msg_hdr.status = ICF_NULL; 
     internal_msg.msg_hdr.ecode = ICF_NULL;
#endif

  if (ICF_TRUE != p_cc_pdb->p_call_ctx->call_waiting_call)
    {
    p_early_call_cfm = (icf_early_incoming_call_cfm_st*)
                        p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
   
    if(p_early_call_cfm->result == ICF_SUCCESS)
    {

		p_cc_pdb->p_call_ctx->line_id = p_early_call_cfm->line_id;
        icf_port_memcpy(
                (icf_void_t*)(&search_address),
                (icf_void_t*)(&p_cc_pdb->p_call_ctx->local_address),
                (icf_uint32_t)sizeof(icf_address_st));


		if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                    p_cc_pdb->p_glb_pdb, p_early_call_cfm->line_id,
                    &p_line_data, p_cc_pdb->p_ecode))
        {
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                     ICF_CALL_CLEAR_REASON_LINE_BUSY)
            p_cc_pdb->p_call_ctx->line_id = ICF_INVALID_LINE_ID;

            ret_val = ICF_FAILURE;
        }	
         else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                p_cc_pdb->p_glb_pdb,
                                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                (icf_void_t *)&p_config_data,
                                p_cc_pdb->p_ecode))
                {
                  ret_val = ICF_FAILURE;
                }

         else if(ICF_NULL != p_line_data->p_user_db)
        { 
          if(ICF_TRUE == p_config_data->early_inc_ind_for_all_users)
             {
               if(ICF_FAILURE ==
                icf_dbm_get_line_for_inc_call(p_cc_pdb->p_glb_pdb,
                                        (icf_boolean_t)(ICF_FALSE),
                    &(p_cc_pdb->p_call_ctx->local_address),
                    &(line_id),
                    &status,
                    p_cc_pdb->p_ecode))
                  {

                      /*
                      * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
                      */
                      ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                            ICF_CALL_CLEAR_REASON_USER_BUSY)

                      /*
                      * Send call clear request to SSA to reject this call
                      */
                      status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                      ret_val = ICF_FAILURE;

                  }/* end of if for ICF_FAILURE == icf_dbm_get_line_for_inc_call..*/
                 
               #ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
        /* In case the exact userinfo received in req-uri of incoming
         * INVITE did not match any of the configured user addresses,
         * there is still a possibility that the req-uri contained
         * phone-context parameter. In such a case, we should also try to
         * match the reconstructed address with the configured users.
         */
        else if ((ICF_NO_USER_ON_LINE == status) &&
                 (ICF_NULL != (ICF_ALTERNATE_ADDR_FORMAT_POSSIBLE &
                                p_cc_pdb->p_call_ctx->common_bitmask)))
        {
            ICF_PRINT(("\n[CC]: Received called party address not configured."\
                        "\n[CC]: Check if alternate address is configured\n"));
            if (ICF_FAILURE == icf_dbm_get_line_for_inc_call(
                            p_cc_pdb->p_glb_pdb,
                            (icf_boolean_t)(ICF_FALSE),
                            &(p_cc_pdb->p_call_ctx->alternate_local_address),
                            &(line_id),
                            &status,
                            p_cc_pdb->p_ecode))
            {
                /*
                 * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
                 */
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_USER_BUSY)

                /*
                 * Send call clear request to SSA to reject this call
                 */
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* Copy alternate address in search address for further validations.
                 */
                icf_port_memcpy(
                        (icf_void_t*)(&search_address),
                        (icf_void_t*)(&p_cc_pdb->p_call_ctx->alternate_local_address),
                        (icf_uint32_t)sizeof(icf_address_st));


            }
        }
        ICF_PRINT(("\n[CC]: icf_dbm_get_line_for_inc_call returned line_id "\
                        "= %d and status = %x\n", \
                        p_cc_pdb->p_call_ctx->line_id, status));

        if (ICF_FAILURE == ret_val)
        {
            /* do nothing */
            ret_val = ICF_FAILURE;
        }
#endif /* ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING */



               else if(ICF_INVALID_LINE_ID == line_id ||
                                                    ICF_NO_USER_ON_LINE == status )
                      {
                        user_not_found_on_any_line = ICF_TRUE;
                      }
                      else
                      {
                        if(ICF_FAILURE != icf_dbm_check_if_user_exits_on_line(
                                                             p_cc_pdb->p_glb_pdb,
                                                         &(search_address),
                                                                      p_line_data,
                                                              p_cc_pdb->p_ecode))
                                   {
                                     user_found = ICF_TRUE;
                                     ret_val = ICF_SUCCESS;
                                   }


                             if(ICF_FALSE == user_found)      
                             {
                                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                                          ICF_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED)
                                ret_val = ICF_FAILURE;
                             }
                      }

             }
          if((ICF_FALSE == p_config_data->early_inc_ind_for_all_users) ||
             (ICF_TRUE == user_not_found_on_any_line))
             {
               p_usr_db_node = p_line_data->p_user_db;
               while(ICF_NULL != p_usr_db_node)
                          {
                            if (!icf_port_strncmp(p_usr_db_node->public_user.addr_val.str, \
                                              (icf_uint8_t *)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t *)ICF_WILDCARD_USER)))
                                          {
                                            wildcard_user_present = ICF_TRUE;
                                            break;
                                          }
                            p_usr_db_node = p_usr_db_node->p_next;
                          }
              if(ICF_FALSE == wildcard_user_present)
                {
                  /* this line is busy send the call release to the application */
                   ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                       ICF_CALL_CLEAR_REASON_LINE_BUSY)
                   ret_val = ICF_FAILURE;
                }
             }
         status = ICF_STATUS_CONTINUE;
         }
         else if(ICF_NULL == p_line_data->p_user_db)
         {
               /* this line is busy send the call release to the application */
                  ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                       ICF_CALL_CLEAR_REASON_LINE_BUSY)
                  ret_val = ICF_FAILURE;
         }

     if (ICF_SUCCESS == ret_val)
     {    
    	 if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                p_cc_pdb->p_glb_pdb,
                                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                                (icf_void_t *)&p_config_data,
                                 p_cc_pdb->p_ecode))
        {
             ret_val = ICF_FAILURE;
        }
		/*If self transport is not received for line, clear the call*/
        else if ((ICF_CFG_SELF_ADDR_PER_LINE == 
                  p_config_data->self_addr_per_line ) && 
                 (ICF_FALSE == p_line_data->line_data_init_complete))
        {
			/* release the call here */
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                       ICF_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED)
            
             
             icf_cc_pa_clear_handler(p_cc_pdb);
	        /* this will trigger the forceful call clear */ 
            return ICF_FAILURE;

        }
        else
        {
            if ((p_line_data->num_of_calls > 0) || (p_line_data->p_call_ctx_list != ICF_NULL))
            {
                p_call_ctx = p_line_data->p_call_ctx_list->p_call_ctx;
                if (ICF_REL_TYPE_INVALID == p_call_ctx->release_type)
                {
                    trigger_cause = (icf_uint8_t)ICF_CAUSE_LINE_BUSY;
                }
            }
            /*
             * Call DBM's function to check service activated on line
             */
            ret_val = icf_dbm_chk_line_service(
                    p_cc_pdb->p_glb_pdb,
                    trigger_cause,
                    p_cc_pdb->p_call_ctx->line_id,
                    &status,
                    p_cc_pdb->p_ecode);
            /*
             * If the service status is not successfully returned by DBM or
             * DBM has reurned ICF_STATUS_CALL_R_SRV_TERMINATE in status
             * which means the call needs to be cleared then mark release cause
             * as ICF_REL_CAUSE_USER_BUSY and send reject to SSA
             */
            if (ICF_FAILURE == ret_val ||
                        ICF_STATUS_CALL_R_SRV_TERMINATE == status)
            {
                /*
                 * Fill the release cause as ICF_REL_CAUSE_USER_BUSY
                 */
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                       ICF_CALL_CLEAR_REASON_USER_BUSY)

                /*
                 * Send call clear request to SSA to reject this call
                 */
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;

            }
        } 
     }

	 /*
         * Attach the call context to the line given by DBM in 
         * get line for incoming call
         */
        if (ICF_SUCCESS == ret_val)
        {
            if(ICF_FAILURE == icf_dbm_attach_cc_to_line_wo_user(
                    p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->line_id,
                    p_cc_pdb->p_call_ctx,p_cc_pdb->p_ecode))
            {
              /* For all failures before call create, release the call */
              ret_val = ICF_FAILURE;
            }
            /************************************/
            else
            {
            if(ICF_STATUS_CONTINUE != status
                        &&
                        ICF_FAILURE != ret_val)
                {
                    /*
                     * Copy the status into internal message status to return to
                     * CM
                     */
                     p_cc_pdb->p_internal_msg->msg_hdr.status = status;

                    /*This is the case where in some service needs to be
                     * triggered by CM for the call*/
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SRVC_TRIG_STAT_RET)
                    
                     
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                    return ret_val;
                }

            if (ICF_FAILURE == icf_cc_check_recvd_media_with_profile(p_cc_pdb))
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
            p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
            while (ICF_NULL != p_stream_sdp_info_node)
            {
                p_stream_sdp_data = (icf_stream_sdp_info_st *)p_stream_sdp_info_node->p_payload;
                if (ICF_MEDIA_STREAM_ACTIVE == (ICF_MEDIA_STREAM_ACTIVE 
                            & p_stream_sdp_data->stream_status))
                {
                    atleast_one_hit = ICF_TRUE;
                }
                p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
            }
            if (ICF_FALSE == atleast_one_hit)
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    ret_val = icf_cc_00_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
            }
        }
     }
        /***********************************/

        if(ICF_SUCCESS == ret_val)
        {
            if(ICF_FAILURE == icf_cc_send_ssa_setup_resp(p_cc_pdb))
            {
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
            else if(ICF_FAILURE == icf_cc_send_cms_req(p_cc_pdb))
            {
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }

            else
            {
                /*
                 * Update the states in the call context
                 */
#ifdef ICF_SESSION_TIMER
                if (ICF_FAILURE == icf_ssa_process_mesg(&internal_msg))
                {
                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                    ret_val = ICF_FAILURE;
                }
#endif
                ICF_PRINT(((icf_uint8_t *)"[CC] LINE Id=%d marked busy \n",
                                p_cc_pdb->p_call_ctx->line_id));
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
            }
        }
		if(ICF_FAILURE == ret_val)
		{
			/* send the terminate call to the PA */
			ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

            /* This will take care of stopping any currently running timer */
            ret_val = icf_cc_start_release_timer(p_cc_pdb);

            ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);

            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
		}
	

	}
	else
    {
        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                ICF_CALL_CLEAR_REASON_USER_NOT_FOUND);

        /* This will take care of stopping any currently running timer */
        ret_val = icf_cc_start_release_timer(p_cc_pdb);

        ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
        ret_val = icf_cc_pa_clear_handler(p_cc_pdb);

        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
    }
    }
    else
    {
          if (ICF_FAILURE == icf_cc_check_recvd_media_with_profile(p_cc_pdb))
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
            p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
            while (ICF_NULL != p_stream_sdp_info_node)
            {
                p_stream_sdp_data = (icf_stream_sdp_info_st *)p_stream_sdp_info_node->p_payload;
                if (ICF_MEDIA_STREAM_ACTIVE == (ICF_MEDIA_STREAM_ACTIVE
                            & p_stream_sdp_data->stream_status))
                {
                    atleast_one_hit = ICF_TRUE;
                }
                p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
            }
            if (ICF_FALSE == atleast_one_hit)
            {
                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                        ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE)

                    ret_val = icf_cc_00_generic_err(p_cc_pdb);
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
            }
      if(ICF_SUCCESS == ret_val)
        {
            if(ICF_FAILURE == icf_cc_send_ssa_setup_resp(p_cc_pdb))
            {
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }
            else if(ICF_FAILURE == icf_cc_send_cms_req(p_cc_pdb))
            {
                status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                ret_val = ICF_FAILURE;
            }

            else
            {
                /*
                 * Update the states in the call context
                 */
#ifdef ICF_SESSION_TIMER
                if (ICF_FAILURE == icf_ssa_process_mesg(&internal_msg))
                {
                    status = ICF_STATUS_CALL_R_SRV_TERMINATE;
                    ret_val = ICF_FAILURE;
                }
#endif
                ICF_PRINT(((icf_uint8_t *)"[CC] LINE Id=%d marked busy \n",
                                p_cc_pdb->p_call_ctx->line_id));
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_01_AWT_CMS_RESP)
            }
        }
        if(ICF_FAILURE == ret_val)
        {
            /* send the terminate call to the PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

            /* This will take care of stopping any currently running timer */
            ret_val = icf_cc_start_release_timer(p_cc_pdb);

            ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);

            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
        }
    }
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  for the early call response from the APP and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_20_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        
    /*
     * Set release type in call context as network initiated
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_NW_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_NW_INITIATED) 

    /*
     * Fill the release cause as ICF_CALL_CLEAR_REASON_REMOTE_REJECTED
     */
     ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
             ICF_CALL_CLEAR_REASON_REMOTE_REJECTED)

    
	ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

     /* Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } 

     /* Send call cancellation response to SSA */
    if(ICF_FAILURE == icf_cc_send_ssa_cancel_resp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
 
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Early call cfm from the app and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_20_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /*
     * Set release type in call context as MM INITIATED
     */

    /* Set release type as internal error */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

    ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
    
    /* Stop the call setup timer and start the release timer
     */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } 
 
    ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
    /* No Need to change the state */
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_20_AWT_EARLY_IND_CFM 
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_CFM
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_20_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    return	icf_cc_00_clear_call (p_cc_pdb);

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and 
 *                  the setup timer expires.
 *                  Current State: ICF_CC_01_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/

icf_return_t icf_cc_20_timer_expiry (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * Set release type in call context as TIMER EXPIRY
     */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_TIMER_EXPIRY)

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)

    ret_val= icf_cc_start_release_timer(p_cc_pdb);
    
    if(ICF_SUCCESS == ret_val)
    {
    
    ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
	/*
     * Start the release timer
     */
    ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Early call cfm from the app and it receives
 *                  Terminate request from PA.
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_20_pa_call_term_req (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
 

   /*
    * Set release type in call context as PA_INITIAITED
    */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_PA_INITIATED)

             /*
             * Set release cause in call context in the case of
             * incoming call
             */
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
            if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
            {
               /* validate the response code(if present) */
                icf_cc_validate_response_code_in_term_req(p_cc_pdb);

                ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                     ICF_CALL_CLEAR_REASON_USER_NOT_FOUND)
            }

    /* Stop the call setup timer and start the release timer */
    if (ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    ret_val = icf_cc_send_clear_req_to_ssa(p_cc_pdb);
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP);
    /* No Need to change the state */
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

