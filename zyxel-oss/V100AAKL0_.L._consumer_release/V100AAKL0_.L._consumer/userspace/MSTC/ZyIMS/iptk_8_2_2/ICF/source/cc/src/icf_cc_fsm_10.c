/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_10.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_10_AWT_ALERT_PRACK state.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 16-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 09-Mar-2004  Sumit Gupta                         Added handling to check 
 *                                                  media ongoing status before
 *                                                  sending mms request
 * 26-APR-2005   Sumit Gupta    SPR 7448            Updated the rem_media_mode 
 *                                                  in call context when mode is
 *                                                  received for alert_prack
 * 18-May-2005   Sumit Gupta    SPR 7589            In case failure in return
 *                                                  path of mms in prack we 
 *                                                  would set our state as await
 *                                                  MMS
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 25-Jul-2006  Umang Singh	    ICF 6.0		        Multiple m-lines
 *
 * 26-Mar-2007  Deepti Goyal    ICF 6.4             Changes for media connect 
 *                                                  req/User-ALert API from PA.
 * 04-Apr-2007  Deepti Goyal    ICF 6.3             Changes for media connect 
 *                                                  req/User-ALert API from PA.
 * 11-Feb-2009  Anurag Khare    ICF Rel 8.1         changes done for call_modify
 *                                                  enhancements
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD         SDP Tunnelling Enhancements
 * 16-Feb-2009  Anurag Khare    REl 8.1             Call modify Enhancements
 *                                                  in forking Scenario
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
 * 18-Mar-2009 Anurag Khare        SPR 19395        Changes done for SDP Tunnelling
 * 31-Mar-2009 Anurag Khare     Rel 8.2             Changes Done for PRACK Enh
 * 22-Apr-2009 Abhishek Dhammawat SPR 19473         Modified 
 *                                                  icf_cc_10_pa_call_modify_req
 * 08-Jun-2009 Kamal Ashraf     SPR 19590           Modified icf_cc_10_alert_prack
 *                                                  and icf_cc_10_pa_call_modify_req 
 * 14-Jun-2009 Abhishek Dhammawat SPR19590          header list overwrite enh
 * 10-Oct-2009 Rajiv Kumar        SPR 20101         Fix for SPR: 20101  
 * 22-Oct-2009 Anurag Khare       SPR 20078         Fix for SPR:20078
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives Call Terminate Request from PA.
 *                  This is same as receiving the event 
 *                  ICF_CC_PA_CALL_TERMINATE_REQ in 
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* Simply invoke icf_cc_09_pa_call_term_req() that does same */
    ret_val = icf_cc_09_pa_call_term_req(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives a session param change from SSA 
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_10_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st   *p_inc_med_chng =
	    (icf_nw_inc_media_change_req_st *)
	    p_cc_pdb->p_internal_msg->p_msg_data;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_INBAND_INFO)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_COMMITTED)
/*  Rel 8.1
 *  if UPDATE received without SDP the send ind to application
 *  start setup timer and remain in the same state 
 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)  
    if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                              ICF_CALL_MOD_REQ_WO_SDP))

    {
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Fix for SPR: 20101*/
            /* Set the value of is_call_modify_ongoing and call_modify_req_ongoing
             * as true indicating that we are handling the incoimg UPDATE
             * with SDP for this call.*/
            p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_TRUE;
            
            /* make call state to next state 
             * and move to next state so that when
             * call_mod_cfm received then again 
             * reach to the same state back
             */
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_GET_CALL_STATE(p_cc_pdb))
            /* Mark this Offer Change event as ongoing event */
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                           p_cc_pdb->current_event)
             /*SPR 19395 the below change  is done to resolve the
              problem whenever update is received without SDP 
              then IPTK is sending 200 OK with SDP to peer 
              This was happening because the following boolean 
              governs the decision to send 200 OK with/without SDP
              Thus if UPDATE received without SDP then the boolean 
              update_recv must be set and once 200 OK is  
              sent then it must get reset */
            p_cc_pdb->p_call_ctx->update_recv = ICF_TRUE;

        }
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_END
 
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)
    p_cc_pdb->sdp_present = ICF_TRUE;

    p_cc_pdb->p_list_1 = p_inc_med_chng->p_sdp_list;
   /*  By passing this code in tunnel mode
    *  because these bitmask will not be used
    *  in non tunnel mode
    */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    /* Now if added or rejected streams are present 
     * fail the offer.We are entertaining FAX in only the 
     * connected state
     */
    if ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM  ==
			    (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
			    p_inc_med_chng->bitmask)) || 
		    (ICF_NW_INC_MEDIA_ADD_STREAM  ==
		     (ICF_NW_INC_MEDIA_ADD_STREAM &
		     p_inc_med_chng->bitmask)))
    {
		icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
											&(p_inc_med_chng->p_sdp_list));
	    ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
	    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	    return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_END
    /* This function shall validate the i'th level
     * matching and copy the received SDP into the 
     * call context
     */
    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
    if (ICF_FAILURE == 
	    icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
    {
        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
	    /* Check if ith level check has failed */
	    if (ICF_CC_STREAM_POS_VALIDATION_FAIL == (p_cc_pdb->common_bitmask & 
                                ICF_CC_STREAM_POS_VALIDATION_FAIL))
	    {
	        /* Invoke the generic call clear handler corresponding 
            to that state */
	        ret_val = icf_cc_10_generic_err(p_cc_pdb); 
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ret_val;
	    }
	    else
	    {
	        ret_val = ICF_FAILURE;
	    }
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	    return ret_val;
    }
    /*  Bypassing this code in tunnel mode 
     *  because in tunnel mode UPDATE with SDP will be handled
     *  in tunnel mode
     */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    /* Check inband info flag to find if it was Alert with or without SDP */
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->inband_info_present)
    {
        /* Alert with SDP had been sent to peer */

        /* Send 500 error response to peer */
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

        ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_END
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
        /* Alert without SDP had been sent to peer */
        /* Media has been committed */
        /* 
         * Now invoke the function which shall compute the 
         * MMS type while comparing the applied and the network received SDP's
         */
        if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
ICF_CHECK_TUNNEL_MODE_END
        /*  Set the mms type as LOCAL sdp required
         *  and Remote SDP present
         */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
        if(ICF_FAILURE == icf_cc_make_mms_type_for_offer_in_tunnel_mode(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }

ICF_CHECK_TUNNEL_MODE_END
        if(ICF_FAILURE == ret_val)
        {
            /*  Do Nothing */
        }
        else
        {
            /*TO DO Check for media state*/
            if(ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
            {
                /*
                 * MMS cannot be sent to nw as request is pending from MM
                 * Mark offer change as pending event
                 */
                ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_OFFER_CHANGE_REQ)

                /* Set call state */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                        ICF_CC_13_WAIT_START_PROC_MEDIA)

                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
            }
            else
            {
     /*  In tunnel mode if UPDATE received with SDP then
      *  First indiaction shall be sent to application
      *  and then MMS was done.
      */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                     p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
                }
ICF_CHECK_TUNNEL_MODE_END

                /* Go to state Awaiting MMS response */
                ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_04_AWT_MMS_RESP)
 
                /* Send MMS request to MMI */
                ret_val = icf_cc_send_mms_req(p_cc_pdb);

                if (ICF_SUCCESS == ret_val)
                {
                    if( ICF_TRUE == p_cc_pdb->mms_sent)
                    { 

                        /* Set next_state as Await Alert Prack */
                        ICF_CC_SET_NEXT_STATE(p_cc_pdb, 
                                ICF_CC_10_AWT_ALERT_PRACK)

                        /* Go to state Awaiting MMS response */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                                ICF_CC_04_AWT_MMS_RESP)

                        /* Mark this Offer Change event as ongoing event */
                        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, 
                                p_cc_pdb->current_event)
                    }
                    else
                    {
   /*  In tunnel mode bypassing the code
    *  as this IPTK do not understand
    *  the stream deletion
    */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                       if(ICF_CC_ALL_STREAMS_DELETED == 
                                (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
                       {
                            /* Send 500 error response to peer */
                            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;
                            ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
                            ret_val = icf_cc_10_generic_err(p_cc_pdb);
                            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
                       }
                       else
                       {

                             /* Sned ack for offer req */
                            ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);

                            /* IN case mms is not send, revert the current state */
                            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                  ICF_CC_10_AWT_ALERT_PRACK)
                        }
ICF_CHECK_TUNNEL_MODE_END
                    }
                }

            } /* for inband info present or not */
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives an error indication from MMI.
 *                  This is same as the event ICF_CC_MM_ERR_IND received in 
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_10_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_09_mm_err_ind() that does same handling */
    ret_val = icf_cc_09_mm_err_ind(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and Alert Timer expires.
 *                  In this case, send DMS request and initiate call clear. This
 *                  is same as the event ICF_CC_TIMER_EXPIRY received in 
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_10_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* Simply invoke icf_cc_09_timer_expiry() that does the same handling */
    ret_val = icf_cc_09_timer_expiry(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives a call connect from PA.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_10_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_connect_req_st               *p_call_connect_req =
                (icf_connect_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;       

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);
    /* Set this as pending event, it will be handled on receiving PRACK */
    ICF_CC_SET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_PA_CALL_CONNECT)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)

    /* In tunnel mode, we will not process rejected streams */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    /* 
     * Check the rejected streams present in connect request 
     * and populate the same in the rejected stream array in call context 
     */
     if (ICF_NULL != p_call_connect_req->count )
     {
        icf_uint8_t count =0;
        /* Some deleted streams are present */
        for ( count =0;(count<p_call_connect_req->count) && (count < ICF_MAX_STREAM_PER_CALL );count ++)
        {
            if ( p_cc_pdb->p_call_ctx->rejected_stream_count < ICF_MAX_STREAM_PER_CALL ) p_cc_pdb->p_call_ctx->rejected_stream_list
                [p_cc_pdb->p_call_ctx->rejected_stream_count++] =
             p_call_connect_req->deleted_media_streams[count];
        }
    }
ICF_CHECK_TUNNEL_MODE_END    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives that response from SSA 
 *                  (SIP PRACK received in response to 180).
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_ALERT_PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_10_alert_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_inc_prack_st              *p_alert_prack =ICF_NULL;
    icf_stream_sdp_info_st      *p_stream_sdp_info = ICF_NULL;
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb);
 
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_alert_prack = (icf_nw_inc_prack_st *) 
                         p_cc_pdb->p_internal_msg->p_msg_data;
        
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_MM_ERR_IND)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_TERM_REQ)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_INT_ERR)
    /*  Rel 8.2 changes start */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(p_alert_prack->bitmask & ICF_NW_INC_PRACK_SDP)
    {
        p_cc_pdb->sdp_present = ICF_TRUE;
    }
    ret_val = icf_cc_send_pa_prack_req_recv_ind(p_cc_pdb);
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
ICF_CHECK_TUNNEL_MODE_END
    /*  Rel 8.2 changes End */

    
    /* Check if Call Term or MM Error or Internal Error is a pending event */
    if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_MM_ERR_IND)) 
            || (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ))
            || (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_INT_ERR)))
    {  
        /* SPR 19590 :Extract the txn id from pending event
           list corresponding to input bit so that we work on
           correct header list using the extracted txn-id */
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                ICF_PENDING_EVENT_PA_CALL_TERM_REQ)) 
        {
            icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                ICF_PENDING_EVENT_PA_CALL_TERM_REQ);
        }
        /* SPR 19590 : End */
        icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&(p_alert_prack->p_sdp_list));
        ret_val = icf_cc_09_call_term_handler(p_cc_pdb);

        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }
    else
    {

    
        /* Check if Ack for prov response can be sent or not */
        /* Fill p_cc_pdb->sdp_present */

        if(p_alert_prack->bitmask & ICF_NW_INC_PRACK_SDP)
        {
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SDP_IN_PEER_MSG)
	        p_cc_pdb->sdp_present = ICF_TRUE;

            /* Now if added or rejected streams are present 
	         * fail the offer.Ignore the same in an answer
	         */
            if (ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
	    {
                /* Rel 8.1
                 * if pending event is call_mod_req
                 * and PRACK received with SDP then
                 * send 500 for PRACK and do MMS 
                 * for CALL_MOD-REQ
                 */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                       ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)) 
                && (ICF_SUCCESS == ret_val))
                {
                    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                                        &(p_alert_prack->p_sdp_list));
                    p_cc_pdb->p_call_ctx->offer_failure_reason = 
                                                ICF_SA_SERVER_INTERNAL_ERROR;
                    ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
                    if(ICF_SUCCESS == ret_val)
                    {
                        /* SPR 19590 :Extract the txn id from pending event
                           list corresponding to input bit so that we work on
                           correct header list using the extracted txn-id */
                        icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                       ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ);
                        /* SPR 19590 : End */
                        /* Rel 8.1
                         * IT means local SDP is required by CC 
                         * Now the processing starts for the Call_modify_req 
                         * set the mmstype ICF_LOCAL_SDP_REQUIRED
                         * send mms request.
                         * move to state 4
                         * make ongoing event PA_CALL_MODIFY_REQ.
                         */
                        p_stream_sdp_info =
                         (icf_stream_sdp_info_st *)
                                         (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
                        p_stream_sdp_info->media_modify_type = ICF_NULL;
                        p_stream_sdp_info->media_modify_type |= ICF_LOCAL_SDP_REQD;
                        if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                        {
                            ret_val = ICF_FAILURE;
                            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                            return ret_val;

                        }
                        else
                        {
                            /* Fix for SPR:20101*/
                            /* No timer started for MMS response , currently setup timer will
                             * be running so we cannot start a new timer.Setup
                             * timer will guard the MMS request.*/
                            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_MODIFY_REQ)
                            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                   ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)
                            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                            return ret_val;
            
                        }
                    }
                }
ICF_CHECK_TUNNEL_MODE_END
                /* In tunnel mode, we do not understand streams and 
                   their status.
                 */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)

	            if ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM  ==
			        (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
			        p_alert_prack->bitmask)) || 
			            (ICF_NW_INC_MEDIA_ADD_STREAM  ==
			        (ICF_NW_INC_MEDIA_ADD_STREAM &
			        p_alert_prack->bitmask)))
                {
					icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
											&(p_alert_prack->p_sdp_list));
                    p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_SA_REJECT;

		            ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);
		            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
		            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace*/
		            return ret_val;
                }
ICF_CHECK_TUNNEL_MODE_END
	    }

	        p_cc_pdb->p_list_1 = ((icf_nw_inc_prack_st *)
		        p_cc_pdb->p_internal_msg->p_msg_data)->p_sdp_list;

	        /* This function shall validate the i'th level
	         * matching and copy the received SDP into the 
	         * call context
	         */
            ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
	        if (ICF_FAILURE == 
		        icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
	        {
                p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_SA_REJECT;
                ret_val = icf_cc_send_ssa_alert_prack_err(p_cc_pdb);

		        /* Check if ith level check has failed */
		        if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
                        (p_cc_pdb->common_bitmask & 
                            ICF_CC_STREAM_POS_VALIDATION_FAIL))
		        {
		            /* Invoke the generic call clear handler corresponding 
                    to that state */
		            ret_val = icf_cc_10_generic_err(p_cc_pdb); 
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;
		        }
		        else
		        {
		            ret_val = ICF_FAILURE;
		        }
	        }
        }
    }
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
   
        
    if (ICF_SUCCESS == ret_val && (ICF_FALSE == p_cc_pdb->sdp_present)) 
    {
        /* Either PRACK came without SDP or it is NFS call */
        
        /* Send Ack for provisional response recd */
	    ret_val = icf_cc_send_ssa_alert_prov_resp_ack(p_cc_pdb);

        
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)
        
            if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                            ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
                    && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                       ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ);
                /* SPR 19590 : End */
                /* Send 183 with SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_media_param(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)

                }
                /*Fix merged for CSR_1-5417099*/
                else 
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                           ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP))
                   && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                 ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
                /* SPR 19590 : End */
                /* Send 183 without SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                           ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)

                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                            ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
                    && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                 ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ);
                /* SPR 19590 : End */
                /* Send Alert without SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_10_AWT_ALERT_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)

                }
                /*Fix merged for CSR_1-5417099*/
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                            ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
                    && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                 ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ);
                /* SPR 19590 : End */
                /* Send Alert with SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_alert_media(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_10_AWT_ALERT_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)

                }
                /*Fix merged for CSR_1-5417099*/
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else if((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                            ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
                    && (ICF_SUCCESS == ret_val))
            {
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)
                    /* Send Alert without SDP with prov response required */
                    p_cc_pdb->prack_reqd = ICF_TRUE;
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                   /*  If Incoming call modify is ongoing
                    *  then set next call state as 05 because
                    *  once call modify cfmm will be received then 
                    *  after processing that next state shall be set as 
                    *  current state
                    */
                   if(ICF_CC_SSA_OFFER_CHANGE != p_cc_pdb->p_call_ctx->ongoing_event)
                   {
	               /* Go to Awaiting Connect from PA if prack without SDP */
                       ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
                   }
                   else
                   {
                       ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                   }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
	           /* Go to Awaiting Connect from PA if prack without SDP */
                   ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
ICF_CHECK_TUNNEL_MODE_END

            }
            else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                            ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
                    && (ICF_SUCCESS == ret_val))
            {
                /* If PA call connect was the pending event */
                /*If media ongoing is false*/
                if(ICF_INT_MODULE_INVALID == 
                        ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
                {
                   /* SPR 19590 :Extract the txn id from pending event
                      list corresponding to input bit so that we work on
                      correct header list using the extracted txn-id */
                   icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                 ICF_PENDING_EVENT_PA_CALL_CONNECT);
                   /* SPR 19590 : End */
                    p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
                    ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
                    /* Check for streams deleted and invoke generic error */
                    if(ICF_CC_ALL_STREAMS_DELETED == 
                            (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
                    {
                        ret_val = icf_cc_09_generic_err(p_cc_pdb);
                    }
                }
            }
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
            /*  REl 8.1
             * handling of peding event ICF_PA_CALL_MODIFY_REQ
             */ 
            if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                           ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)) 
                    && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                 ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ);
                /* SPR 19590 : End */
                /* Rel 8.1
                 * This will be call mod req with sdp
                 * IT means local SDP is required by CC 
                 * set the mmstype ICF_LOCAL_SDP_REQUIRED
                 * send mms request.
                 * move to state 4
                 * make ongoing event PA_CALL_MODIFY_REQ.
                 */
                p_stream_sdp_info =
                             (icf_stream_sdp_info_st *)
                             (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
                p_stream_sdp_info->media_modify_type = ICF_NULL;
                p_stream_sdp_info->media_modify_type |= ICF_LOCAL_SDP_REQD;
                            if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ret_val = icf_cc_start_media_timer(p_cc_pdb);
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_MODIFY_REQ)
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                           ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)
    
                }
            }
ICF_CHECK_TUNNEL_MODE_END
    }/* End Else */
    /*************************/
    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* SPR 19590 :Extract the txn id from pending event
           list corresponding to input bit so that we work on
           correct header list using the extracted txn-id */
        icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                             ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
        /* SPR 19590 : End */
        /* Send Alert without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP) 

        }
        /*Fix merged for CSR_1-5417099*/
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    /***************************/

    else if(ICF_SUCCESS == ret_val && (ICF_TRUE == p_cc_pdb->sdp_present))
    {
        /* PRACK contained SDP */
        /* 
         * Now invoke the function which shall compute the 
         * MMS type while comparing the applied and the network received SDP's
         */
        if (ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
	    {
            /* Media has been committed. This is a new offer from remote.
               In tunnel mode, we will set the media_modify_type field 
               which will determine whether the tunnelled MMS request
               and response will contain the tunnelled SDP or not.
               Also copy the local SDP in the call context to the last
               committed SDP whenever a new offer is received.  
            */ 
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)             
		    if (ICF_FAILURE == 
                icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
		    {
			    ret_val = ICF_FAILURE;
		    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
            if(ICF_FAILURE == icf_cc_make_mms_type_for_offer_in_tunnel_mode(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;	
            }
ICF_CHECK_TUNNEL_MODE_END
	    }
	    else
	    {
            /* Media has not been committed. This is an answer from remote.
               In tunnel mode, we will set the media_modify_type field 
               which will determine whether the tunnelled MMS request
               and response will contain the tunnelled SDP or not.
            */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
		    if (ICF_FAILURE == icf_cc_make_mms_type_for_rem_succ_ans(p_cc_pdb))
		    {
			    ret_val = ICF_FAILURE;
		    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
            if(ICF_FAILURE == icf_cc_make_mms_type_for_answer_in_tunnel_mode(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;	
            }
ICF_CHECK_TUNNEL_MODE_END
	    }
	if (ICF_FAILURE != ret_val)
	{  
	    /* Mark current event as ongoing event */
	    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, p_cc_pdb->current_event)

		if(ICF_INT_MODULE_INVALID != ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
		{
		    /*
		     * MMS cannot be sent to nw as request is pending from MM
                 * Mark offer change as pending event
                 */
                ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP)

                /* Set call state */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_13_WAIT_START_PROC_MEDIA)
            
            }
            else
            {

                /* Send MMS Request to MMI */
                ret_val = icf_cc_send_mms_req(p_cc_pdb);

                if (ICF_SUCCESS == ret_val)
                {
                    if(ICF_TRUE == p_cc_pdb->mms_sent)
                    {
                        /* Change state to Awaiting MMS response */
                        ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                                ICF_CC_04_AWT_MMS_RESP)
                    }
                    else 
                    {
                        if(ICF_CC_ALL_STREAMS_DELETED == 
                                (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
                        {                         
                            ret_val = icf_cc_send_ssa_alert_prov_resp_ack(p_cc_pdb);
                            /* Fix for CSR 1-7042401: Set Release cause */
                       	    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                                ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED)
                            ret_val = icf_cc_10_generic_err(p_cc_pdb);
                            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                            return ret_val;
	                    }
                        else
                        {
                            ret_val = icf_cc_send_ssa_alert_prov_resp_ack(p_cc_pdb);
                            /*  Awaiting Connect from PA if mms not sent */
                            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
                        }
                    }
                }
                if ((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
                        && (ICF_SUCCESS == ret_val))
                {
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
                }
            } /* for prack with sdp */
        }
    }

    /*If media is already commited*/
    if ((ICF_SUCCESS == ret_val) && 
            ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
    {
    
        if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
                        && (ICF_SUCCESS == ret_val))
            {
              /* SPR 19590 :Extract the txn id from pending event
                 list corresponding to input bit so that we work on
                 correct header list using the extracted txn-id */
              icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ);
              /* SPR 19590 : End */
               /* Send 183 with SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;
                    
                if(ICF_SUCCESS == icf_cc_send_ssa_media_param(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)
                }
                /*Fix merged for CSR_1-5417099*/
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
        else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                        ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)) 
                && (ICF_SUCCESS == ret_val))
        {
              /* SPR 19590 :Extract the txn id from pending event
                 list corresponding to input bit so that we work on
                 correct header list using the extracted txn-id */
              icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
              /* SPR 19590 : End */
            /* Send 183 with out SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                         ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)
                }
                /*Fix merged for CSR_1-5417099*/
                else
                {
                    ret_val = ICF_FAILURE;
                }
        }

           else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                           ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
                        && (ICF_SUCCESS == ret_val))
            {
                /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ);
                /* SPR 19590 : End */
                /* Send Alert without SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
                {
                    /* Change call state to Awaiting prov response
                     *  for alert
                     */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_10_AWT_ALERT_PRACK)
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)
                }
                /*Fix merged for CSR_1-5417099*/
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
           else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                           ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
                   && (ICF_SUCCESS == ret_val))
           {
               /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ);
               /* SPR 19590 : End */
               /* Send Alert with SDP with prov response required */
               p_cc_pdb->prack_reqd = ICF_TRUE;

               if(ICF_SUCCESS == icf_cc_send_ssa_alert_media(p_cc_pdb))
               {
                   /* Change call state to Awaiting prov response
                    *  for alert
                    */
                   ICF_CC_SET_CALL_STATE(p_cc_pdb,
                           ICF_CC_10_AWT_ALERT_PRACK)
                   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)

               }
               /*Fix merged for CSR_1-5417099*/
               else
               {
                   ret_val = ICF_FAILURE;
               }
           }
           /* 
            * IF signaling ongoing was not set and still mms was not sent means 
            * either prack was received without SDP or the was no such change in 
            * remote SDP due to which mms had to be sent to MM so no mms would have
            * been sent. In this scenarion since media committed is true so if PA 
            * call connect was pending then send mms request for PA call connect 
            */ 
        else
        {
            if(ICF_INT_MODULE_INVALID == 
                    ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb) &&
                    ICF_FALSE == p_cc_pdb->mms_sent && 
                    ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                        ICF_PENDING_EVENT_PA_CALL_CONNECT))
            {
               /* SPR 19590 :Extract the txn id from pending event
                   list corresponding to input bit so that we work on
                   correct header list using the extracted txn-id */
               icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                            ICF_PENDING_EVENT_PA_CALL_CONNECT);
               /* SPR 19590 : End */
                ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
            }
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent 
 *                  to peer and it receives a Call Cancel Request from SSA 
 *                  (SIP CANCEL received from peer). This is same as the event 
 *                  ICF_CC_SSA_CALL_CANCEL_REQ received in 
 *                  ICF_CC_01_AWT_CMS_RESP state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_01_ssa_call_cancel_req() that does same */
    ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);
	/*
	 * Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
	 * Since we are call the same handler in state 10 so we should reset this bit 
	 * as the PA interface would also need to be cleared
	 * So Reset PA cleared bit 
	 */
    ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  provisional response from SSA for a user alert sent to peer 
 *                  and it receives a generic error indication.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_10_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_09_generic_err() that does same */
    ret_val = icf_cc_09_generic_err(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
  *
  * FUNCTION:        icf_cc_10_pa_call_modify_req
  *
  * DESCRIPTION:     This function is invoked when the CC is awaiting
  *                  PRACK state and ICF_CALL_MODIFY_REQ is received
  *                  from PA
  *
  *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
  *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
  *
  *****************************************************************************/
icf_return_t icf_cc_10_pa_call_modify_req(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_config_data_st          *p_config_data = ICF_NULL;
    icf_call_modify_req_st      *p_call_modify_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                   p_cc_pdb->p_glb_pdb->p_recv_msg,
                                   p_cc_pdb->p_glb_pdb->p_recv_msg->payload,
                                   p_cc_pdb->p_call_ctx,
                                   ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
    }
    p_call_modify_req =
        (icf_call_modify_req_st*)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* Get pointer to config data from DBM */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
               (icf_int_module_id_t)ICF_INT_MODULE_CFG,
               (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    /*  If UPDATE is allowed to be sent */
    /*SPR 19473 if ALLOW hdr is not all present it is assumed
      that peer does not supports UPDATE.*/
    if(ICF_UPDATE_PRESENT_IN_ALLOW_HDR & p_cc_pdb->p_call_ctx->allowed_value)
    {
        if(p_call_modify_req->bit_mask & 
                ICF_CALL_MOD_REQ_TO_TAG_PRESENT)
        {
            /*Rel 8.1
             *Get the memory for to tag in call context & 
             *Copy the receive to tag in call context
             *so that when trigger is given to for UPDATE
             *to SSA then it can be used
             */
           if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
           {
               ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
               p_cc_pdb->p_call_ctx->p_to_tag, ICF_MEM_COMMON,
               p_cc_pdb->p_ecode, ret_val)
           }
           /*  Create the memory of to tag in
            *  in call context
            */
           ICF_MEMGET(p_cc_pdb->p_glb_pdb,
           (p_call_modify_req->to_tag.str_len+1),
           ICF_MEM_COMMON,
           p_cc_pdb->p_call_ctx->p_to_tag,
           ICF_RET_ON_FAILURE,
           p_cc_pdb->p_ecode,
           ret_val)
           /* copy the to tag in call context */
           icf_port_strcpy(p_cc_pdb->p_call_ctx->p_to_tag,
                       p_call_modify_req->to_tag.str);

        }
 
        /*  If call modify req is received without
         *  SDP then send UPDATE
         */
       if(ICF_FALSE == 
          p_call_modify_req->tunnelled_sdp_present)
       {
            /*Set common_bitmask as UPDATE_WO_SDP*/
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP)
            /* SPR 19590 : code removed for cloning header/tag/body list from 
             * here as this shall be taken care in icf_cc_convert_api_to_event
             * function.
             */
            if(ICF_SUCCESS == ret_val)
            {
                if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
                /*  Fix for SPR 20078*/
                /*  if call modify request is received from application then set
                 *  this boolean to track this call modify at the time of timer
                 *  expiry or generic error. THis boolean will be reset once
                 *  ICF_CALL_MODIFY_RESP will be sent to application.
                 */
                else
                {
                    p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_TRUE;
                }
            }
            else
            {
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                }
            }
        }
        else
        {
             /*  If media is not committed yet then
              *  send failure response to application
              */
             if(ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
             {
                 /* Rel 8.1
                 * reject the request with error cause 
                 * OFFER_ANSWER_NOT_COMPLETED
                 */
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_OFFER_ANSWER_NOT_COMPLETED;
                ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
      
                /* Send failure response for update to user */
                if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
                {
                      ret_val = ICF_FAILURE;
                }
             }
             else
             {
                 /*  MARK IT PENDING ONCE PRACK 
                  *  would be received the it would be processed */
                 /*  Fix for SPR 20078 */
                 /*  Set this boolean as ICF_CALL_MODIFY_REQ is receieved. */
                 p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_TRUE;
                 ICF_CC_SET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)
             }
        }
        if(ICF_FAILURE == ret_val)
        {
            if(ICF_NULL != p_cc_pdb->p_call_ctx->p_to_tag)
            {
                ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                      p_cc_pdb->p_call_ctx->p_to_tag, ICF_MEM_COMMON,
                      p_cc_pdb->p_ecode, ret_val)
                ret_val = ICF_FAILURE;
            }
        }
    }
    else
    {
         /* reject the request*/
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
  
        /* Send failure response for update to user */
        if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    /* reject the request*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
  
    /* Send failure response for update to user */
    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
     
ICF_CHECK_TUNNEL_MODE_END
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;
}
