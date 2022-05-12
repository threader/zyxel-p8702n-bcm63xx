/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_16.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_16_AWT_UPDATE state. CC sets its state to 
 *					  ICF_CC_16_AWT_UPDATE only during an incoming call 
 *					  with QoS Preconditions, wherein ICF needs to get
 *					  a confirmation from remote upon successful resource 
 *					  reservation at its(remote) end.
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 11-Jul-2006  Prashanth						 Initial
 * 25-Jan-2008  Neha Gupta      ICF Rel8.0      Modifications for precondition 
 *						support.
 * 11-Feb-2008  Abhishek Dhammawat Rel8.0       Modified
 *                                              icf_cc_16_ssa_offer_change
 * 08-Mar-2008  Alok Tiwari     Rel8.0          Modified 
 *                                              icf_cc_16_ssa_offer_change
 *                                              for freeing memory when
 *                                              function 
 *                                       cc_validate_ith_level_m_line_check
 *                                              returns failure.
 * 11-Mar-2008  Abhishek Dhammawat Rel8.0       Modified 
 *                                              icf_cc_16_ssa_offer_change
 * 24-Mar-2008  Shruti Thakral  SPR 18283       Klocworks Warning removal
 * 28-Mar-2008  Shruti Thakral  SPR 18303       Klocworks Warning removal
 * 19-Feb-2009  Abhishek Dhammawat Rel8.1       Modified 
 *                                              icf_cc_16_ssa_offer_change
 * 19-Feb-2009  Anurag Khare     Rel 8.1        KlocWorks Warning Removal
 * 24-Apr-2009  Abhishek Dhammawat SPR 19502       Modified 
 *                                              icf_cc_16_ssa_offer_change
 * 6-May-2009  Abhishek Dhammawat SPR 19574       Added 
 *                                              icf_cc_16_pa_alert
 *                                              
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting an UPDATE 
 *                  request from remote and receives the same.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_16_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

    icf_nw_inc_media_change_req_st	*p_inc_media = 
	    (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb); /* function entry trace */

	/* This function is checking if there is any mismatch in the 
	 * oreder of streams.
	 */

	p_cc_pdb->p_list_1 = p_inc_media->p_sdp_list;
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    /*In tunnel mode copy the committed SDP because in 200OK
      we shall be overwriting the local SDP */
    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
    if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
     {
         ret_val = 
            icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);
         p_cc_pdb->p_call_ctx->offer_failure_reason = 
                      ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
         if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
         {
              ret_val = ICF_FAILURE;
         }
         ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
         return ICF_FAILURE;
     }
ICF_CHECK_TUNNEL_MODE_END

	ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
	
	if (ICF_FAILURE == ret_val)
	{
             
            icf_error_t		ecode = ICF_ERROR_NONE;
            
            /* Free the memory allocated to p_added_sdp_list and 
               p_nw_reject_stream_list  which is allocated in ssa module
               when incoming ReINVITE/UPDATE received from network*/

               if(ICF_NULL != p_inc_media->p_nw_reject_stream_list)	
                {		
		         icf_cmn_free_media_sdp_info_list(p_cc_pdb->p_glb_pdb,
		                       &(p_inc_media->p_nw_reject_stream_list),
		                                                      &ecode);
	            }
                                              
            if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
    			(p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
		     {
                    p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

                    if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        ret_val = icf_cc_16_generic_err(p_cc_pdb);
                    }
	        }
	}
	else
	{
		/* Fetch the internal sdp stream from list in call context
         * iterate thru the list and fill qos values in local sdp
         * of each stream*/
        icf_stream_sdp_info_st      *p_sdp = ICF_NULL;            
        icf_internal_list_st        *p_list = ICF_NULL;

        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

        while (ICF_NULL != p_list)
        {
			p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);
            
            if (ICF_NULL != p_sdp)
            {
                if(ICF_NULL == p_sdp->p_local_sdp)
                {
                    p_list = p_list->p_next;
                    continue;
                }

                /* Updating Current status lines in remote send and remote recv 
                 * direction in local SDP. We do not need to modify any other status 
                 * lines */
                p_sdp->p_local_sdp->remote_send.current_status = 
                    p_sdp->p_remote_received_sdp->local_send.current_status;
                p_sdp->p_local_sdp->remote_recv.current_status = 
                    p_sdp->p_remote_received_sdp->local_recv.current_status;
            }

			/* Now we will be updating the Qos parameters in local SDP
             * structure by comparing it with Qos parameters in remote
             * received SDP
             */
            /* icf_cc_compare_and_update_qos_params(p_cc_pdb);*/

            /* Reserve Network resources and update local SDP */
			/* Currently this function is returning SUCCESS but
			 * afterwards an interface towards Network Manager will 
			 * be defined.
			 */
            /*ret_val = icf_cc_reserve_nw_res_and_update_qos_params(*/
            /*        p_cc_pdb);*/
            
			/* go to the next node in list */
			p_list = (icf_internal_list_st*)p_list->p_next;
            /*SPR 19502 Added protective check to iterate through SDP list
              only one time in tunnel mode*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
            break;
ICF_CHECK_TUNNEL_MODE_END
        }
        /* Now we will check if current status exceeds or matches desired
         * status in remote network
         */
        if(ICF_SUCCESS == 
                icf_cc_match_curr_des_status_on_remote_nw(p_cc_pdb))
		{
             /* 200OK for UPDATE should have qos lines as 
                reserved for current local because till this
                stage resource reservation is already done */
            if (ICF_NULL != p_sdp && ICF_NULL != p_sdp->p_local_sdp )
            {
                p_sdp->p_local_sdp->local_send.current_status = 
                    ICF_RESOURCE_RESERVED;
                p_sdp->p_local_sdp->local_recv.current_status = 
                    ICF_RESOURCE_RESERVED;
            }
            /*Do not send 200OK for UPDATE in tunnel mode from here*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
			/* We have to Send response for UPDATE request to Peer */
			if(ICF_SUCCESS == icf_cc_send_ssa_offer_chg_rsp(
				      p_cc_pdb))
			{
				/* Send an incoming call indication to PA */
				if (ICF_FAILURE  == icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}
				else
				{
					/*
					* Update the states in the call context
					*/
					ICF_CC_SET_CALL_STATE(
						  p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)

				}
				/* Set media_updated flag to TRUE in order to avoid sending
				 * of SDP in further responses
				 */
				p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
			}
			else
			{
				 ret_val = ICF_FAILURE;
			}
ICF_CHECK_TUNNEL_MODE_END
    /*In case we are sure of precondition attributes are valid in
      UPDATE and can send 200OK then do MMS to get answer SDP, if
     we are rejecting UPDATE by 580 then no need to do MMS*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
        if(ICF_NULL == p_sdp)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ICF_FAILURE;
        }
        /*Do the tunnel MMS req with SDP required for 200OK */
        p_sdp->media_modify_type = ICF_NULL;
        p_sdp->media_modify_type |= ICF_LOCAL_SDP_REQD;
        p_sdp->media_modify_type |= ICF_TUNNELLED_REMOTE_SDP_PRESENT;
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR)
        p_cc_pdb->p_call_ctx->update_recv = ICF_TRUE;
        /*Send incoming call modyfy ind to  application follwed by MMS */
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
        {
             ret_val = ICF_FAILURE;
        }
        else
        {
             if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
             {
                  ret_val = ICF_FAILURE;
             }
             else
             {
                  ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                         p_cc_pdb->current_event)
                  /* make call state to next state 
                   * and move to next state so that when
                   * call_mod_cfm received then again 
                   * reach to the same state back
                   */
                  ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                       ICF_CC_04_AWT_MMS_RESP)
                  ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                          ICF_CC_03_AWT_ALERT_RESP)
             }
        }
ICF_CHECK_TUNNEL_MODE_END

		}
		else
		{
            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                 ICF_SA_PRECONDITION_FAILURE;
            /* to send error response for the update */
            if (ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
             {
                  ret_val = ICF_FAILURE;
             }
            else
            {
                /*generic error is invoked to clear INVITE txn
                 when UPDATE for QOS is rejected*/
                  ret_val = icf_cc_16_generic_err(p_cc_pdb);
            }
		}
	}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting an UPDATE
 *                  request from remote and receives MM error indication from MMI
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_16_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
	
    /*
     * As the handling for this event is the same as in the state
     * ICF_CC_03_AWT_ALERT_RESP,call their state handler.
     * In this case, we should not need to send 580 since sec_pdp
	 * has already been activated.
     */
    ret_val = icf_cc_03_mm_error_ind(p_cc_pdb);
ICF_CHECK_QOS_SUPPORT_END    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  an UPDATE request from the SSA and Setup Timer expires.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_16_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

	/*
     * As the handling for this event is the same as in the state
     * ICF_CC_03_AWT_ALERT_RESP,call their state handler.
     */
    ret_val = icf_cc_03_timer_expiry(p_cc_pdb);

ICF_CHECK_QOS_SUPPORT_END    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  an UPDATE request from SSA and it receives a Call Cancel 
 *					Request from SSA(SIP CANCEL received from peer).
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_16_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

    /*
     * As the handling for this event is the same as in state
     * ICF_CC_01_AWT_CMS_RESP,call its
     * state-event handler
     */
    ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);

	/*
	* Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
	* Since we are call the same handler in state 16 so we should reset this bit 
	* as the PA interface would also need to be cleared
	* So Reset PA cleared bit 
	*/
    ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
		ICF_CALL_CLEAR_STATUS_PA_CLEARED)

ICF_CHECK_QOS_SUPPORT_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_generic_err
 *
 * DESCRIPTION:     This function is invoked when CC is awaiting an UPDATE request
 *                  from SSA and it receives an error indication from MMI.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_16_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* call handler of 01 state */
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
	ret_val = icf_cc_01_generic_err(p_cc_pdb);
ICF_CHECK_QOS_SUPPORT_END

       
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_16_AWT_UPDATE 
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_16_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* call handler of 01 state */
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
	ret_val = icf_cc_01_forceful_call_clear(p_cc_pdb);
ICF_CHECK_QOS_SUPPORT_END


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */    

    return ret_val;    
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  UPDATE from SSA for a precondition negotiation  and
 *                  receives alert req
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_PA_ALERT
 *
 ******************************************************************************/
icf_return_t icf_cc_16_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      ongoing_event = ICF_NULL;
    icf_pa_cc_user_alerted_req_st    *p_alert_req = ICF_NULL;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                             p_cc_pdb->p_glb_pdb->p_recv_msg,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    p_alert_req = 
      (icf_pa_cc_user_alerted_req_st*)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
        /* In tunnel mode, we will not process rejected streams */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    if((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_CALL_CONNECT)) &&
       (ICF_CC_PA_CALL_CONNECT != ongoing_event))

    {  
        ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);

            /* Set this as pending event, it will be handled on receiving PRACK */
        if(ICF_TRUE == p_alert_req->early_media_reqd)
        {
            /* Set this as pending event, it will be handled on receiving PRACK */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)
        }
        else
        {
            /* Set this as pending event, it will be handled on receiving PRACK */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)
        }
    }    
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  UPDATE from SSA for a precondition negotiation  and
 *                  receives connect req
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_PA_CONNECT_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_16_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        /* In tunnel mode, we will not process rejected streams */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)    
        /* To process 200 wo SDP after sending 200 OK for precondition
             UPDATE */
    ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);  
    ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_CONNECT)

ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  for UPDATE for QOS and it receives media_connect_req
 *                  from PA
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_16_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      ongoing_event = ICF_NULL;
    icf_media_connect_req_st         *p_media_connect_req = ICF_NULL;
    ongoing_event = ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                             p_cc_pdb->p_glb_pdb->p_recv_msg,
                             ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    p_media_connect_req =         
        (icf_media_connect_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;         
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)    

    if((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
			    ICF_PENDING_EVENT_PA_CALL_CONNECT)) &&
       (ICF_CC_PA_CALL_CONNECT != ongoing_event))
    {    
        ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);    
        if (ICF_TRUE == p_media_connect_req->send_without_sdp)
        {
            ICF_PRINT(((icf_uint8_t*)"\n[CC]: Request to send SIP response without SDP\n"));
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 
                   ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)
        }
        else
        {
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 	
                  ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)
        }           
    }
    if(ICF_TRUE == p_cc_pdb->prack_reqd)
    {
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
           ret_val = icf_cc_send_pa_unexpected_msg_ind(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


#endif
