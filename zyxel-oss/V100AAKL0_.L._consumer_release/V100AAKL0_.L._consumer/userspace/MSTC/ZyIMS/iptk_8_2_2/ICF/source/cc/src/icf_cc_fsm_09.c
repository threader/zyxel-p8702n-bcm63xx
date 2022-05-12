/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_09.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *
 * Revision History :
 * 
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 16-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 22-Dec-2004  Rohit Aggarwal                      Addition of stats
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 29-Dec-2004  Rohit Aggarwal                      Timer data handling
 * 3-March-2004 Sumit Gupta                         Prack with SDP handling
 * 26-APR-2005  Sumit Gupta     SPR 7448            Updated the rem_media_mode 
 *                                                  in call context when mode is
 *                                                  received for media_prack
 * 18-May-2005   Sumit Gupta    SPR 7589            In case failure in return
 *                                                  path of mms in prack we 
 *                                                  would set our state as await
 *                                                  MMS
 * 27-May-2005   Sumit Gupta    SPR 7624            Update in handling of 
 *                                                  generic error in state 9
 *
 * 31-May-2005   Sumit Gupta    SPR 7629            Pa initiated release type flag
 *                                                  would bet set even if call
 *                                                  forwarding indication is sent
 *                                                  back to CM when call terminate
 *                                                  is received from PA
 * 01-Jun-2005   Sumit Gupta    SPR 7635            180 message would be sent with 
 *                                                  require header as 100 Rel if 
 *                                                  previous 183 message had 100 
 *                                                  rel in require header no matter
 *                                                  what the current configuration 
 *                                                  states  
 * 10-Jun-2005  Jalaj Negi      IMS CLIENT          After getting PRACK for 183
 *                                                  changed state to 
 *                                                  AWT_CMS_RESP
 * 
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 25-Jul-2006  Umang Singh	    ICF 6.0		        Multiple m-lines
 *
 * 27-Mar-2007  Deepti Goyal    ICF 6.4             Changes for Media connect
 *                                                  request/User-Alert API from
 *                                                  PA.
 * 04-Apr-2007  Deepti Goyal    ICF 6.3             Changes for Media connect
 *                                                  request/User-Alert API from
 *                                                  PA.
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
 * 25-Jun-2008  Abhishek Dhammawat IPTK Rel8.0  Precondition Enhancements
 *                                              Modified icf_cc_09_media_param_prack
 * 12-Feb-2009  Tarun Gupta     Rel 8.1 LLD     SDP Tunnelling Enhancements
 * 04-Mar-2009  Anurag Khare    Rel 8.1         Changes done for Call
 *                                              modify Enhancements
 * 31-Mar-2009  Anurag Khare    Rel 8.2         Changes done PRACK Enh
 * 28-Apr-2009 Abhishek Dhammawat SPR 19502    Modified icf_cc_09_media_param_prack
 * 6-May-2009  Abhishek Dhammawat SPR 19574    Modified icf_cc_09_media_param_prack
 * 25-May-2009 Ashutosh Mohan   SPR 19672      Modified icf_cc_09_media_param_prack
 * 08-Jun-2009 Kamal Ashraf     SPR 19590      Modified icf_cc_09_media_param_prack
 * 14-Jun-2009 Abhishek Dhammawat SPR19590     header overwrite enh
 * 10-Oct-2009 Rajiv Kumar        SPR 20101    Fix for SPR: 20101  
 * 22-Oct-2009 Anurag Khare       SPR 20078    Fix for SPR: 20078
 * 
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Set release type as PA initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
    
    ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);

    /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE continue 
     *with normal call clearing*/
    if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE != 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
    {


        /* Stop Alert timer and start Release timer */
        ret_val = icf_cc_start_release_timer(p_cc_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /*
             * Set this event as pending event. It will be handled on receiving 
             * provisional acknowledgement
             */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_TERM_REQ)
        }
    }/*End if(ICF_SUCCESS == ret_val ... */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a session param change from SSA 
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  In this case, send an error response back to SSA. This is
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_09_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)

    /* Simply invoke icf_cc_07_ssa_offer_change() that does same handling */
    ret_val = icf_cc_07_ssa_offer_change(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    ret_val = icf_cc_handle_ssa_offer_change_in_tunnel_mode(p_cc_pdb);
ICF_CHECK_TUNNEL_MODE_END
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives an error indication from MMI.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_09_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
	icf_mm_err_ind_st                *p_mm_err_ind = ICF_NULL;
    icf_mmi_reason_t                 media_error;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	p_mm_err_ind = (icf_mm_err_ind_st*)
                      (p_cc_pdb->p_internal_msg->p_msg_data);
    
	media_error = p_mm_err_ind->reason;

    /* Set release type as MM initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_MM_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_MM_INIT)

	ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_CALL_CLEAR_MEDIA_MISMATCH)

    /* Stop Alert timer and start Release timer */
    ret_val = icf_cc_start_release_timer(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
         /*
         * If mmi has send mm_err_ind with reason as ICF_ECODE_DMS_IND_RECVD
         */
        if(ICF_ECODE_DMS_IND_RECVD == media_error)
        {
            /* Mark MM interface as cleared so as we don’t send DMS when we 
             * receive prack from network*/
            ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                    ICF_CALL_CLEAR_STATUS_MM_CLEARED)
                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_MM_ERR_IND)
        }
        else
        {
            /*
             * Set this event as pending event. It will be handled on receiving 
             * provisional acknowledgement
             */
            ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_MM_ERR_IND)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_MM_ERR_IND)
        }
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and Alert or Release Timer expires.
 *                  In this case, send DMS request and initiate call clear.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_09_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Fix For SPR:20101*/    
    /* While handling the timer expiry in ICF_CC_09_AWT_MEDIA_PARAM_PRACK if the
     * value of is_call_modify_ongoing is set to TRUE it indicates that we
     * have received incoming UPDATE with/without sdp and this UPDATE need to
     * be responded with 504 response.We are handling this scenario in
     * icf_cc_14_timer_expiry where UPDATE is responded with failure response
     * and call will be cleared.*/          
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->is_call_modify_ongoing)
    {
        ret_val = icf_cc_14_timer_expiry(p_cc_pdb);
        p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;
    }
    else
    {
        /* Set release_type as timer expiry */
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_TIMER_EXPIRY)
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_TIMER_INIT)
            /*p_timer_data = (icf_timer_data_st *)
              (p_cc_pdb->p_glb_pdb->p_recv_msg->payload);*/

            p_timer_data = ((icf_timer_data_st *)
                    (p_cc_pdb->p_call_ctx->p_timer_data));
        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

            p_cc_pdb->timer_type =  p_timer_data->timer_type;  
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_TIMER_EXPIRED)

            /* Check whether Alert timer has expired or Release timer */
            if (ICF_TIMER_TYPE_ALERTING == p_timer_data->timer_type)
            {
                /* Stop Alert timer and start Release timer */
                ret_val = icf_cc_start_release_timer(p_cc_pdb);

                if (ICF_SUCCESS == ret_val)
                {
                    /* 
                     * Send error response to SSA (SIP 500). For this, first fill 
                     * appropriate call_context->release_cause that would be used by 
                     * SSA to decide the exact message (500 in this case) to be sent to 
                     * peer.
                     */
                    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, 
                            ICF_CALL_CLEAR_REASON_SYSTEM_ERROR)
                        ret_val = icf_cc_send_ssa_reject(p_cc_pdb);
                }

                /* Send DMS request and go to Awaiting DMS response */
                if (ICF_SUCCESS == ret_val)
                {
                    p_cc_pdb->forceful_delete = ICF_FALSE;
                    ret_val = icf_cc_send_dms_req(p_cc_pdb);
                }

                if (ICF_SUCCESS == ret_val)
                {
                    /* Change state to Await DMS response */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_07_AWT_DMS_RESP)
                }

            } /* for Alert timer expiry */
            else if (ICF_TIMER_TYPE_RELEASE == p_timer_data->timer_type)
            {
                /* 
                 * We had started the Release timer earlier during some termination 
                 * initiation but are still in this state awaiting PRACK. In the 
                 * meanwhile, even the Release timer has expired while we haven't 
                 * received the PRACK yet.
                 */

                /* Clear MM interface */
                p_cc_pdb->forceful_delete = ICF_TRUE;
                ret_val = icf_cc_send_dms_req(p_cc_pdb);
                ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_MM_CLEARED)
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)

                    /* Clear NW interface */
                    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, ICF_CALL_CLEAR_REASON_NOANSWER)
                    ret_val = icf_cc_send_ssa_reject(p_cc_pdb);
                ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_NW_CLEARED)
                    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)

                    /* Clear PA interface */
                    if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                                ICF_REL_TYPE_PA_INITIATED))
                    {
                        ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
                    }
                    else
                    {
                        ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
                    }

                ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
            } /* for Release timer expiry */
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a call connect from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_connect_req_st         *p_call_connect_req =
                (icf_connect_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;         

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* Set this as pending event, it will be handled on receiving PRACK */
    ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);
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
         ret_val = icf_cc_validate_n_add_rejected_streams_in_call_ctxt(\
                p_cc_pdb,p_call_connect_req->deleted_media_streams,\
                p_call_connect_req->count);
    }
ICF_CHECK_TUNNEL_MODE_END
                   
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_media_param_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives that response from SSA 
 *                  (SIP PRACK received in response to 183).
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_09_media_param_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st      *p_stream_sdp_info = ICF_NULL; 
    icf_nw_inc_prack_st		     *p_inc_prack = 
	    (icf_nw_inc_prack_st *)p_cc_pdb->p_internal_msg->p_msg_data;
	    	
	/* Fetch the internal sdp stream from list in call context
     * iterate thru the list */
#ifdef ICF_QOS_SUPPORT
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
#endif    
    icf_internal_list_st		*p_list = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb);
    /*  Rel 8.2 changes Start */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    if(ICF_NW_INC_PRACK_SDP == (p_inc_prack->bitmask &
                    ICF_NW_INC_PRACK_SDP ))
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
        /* SPR 19590 : Checking for Pending event */
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                         ICF_PENDING_EVENT_PA_CALL_TERM_REQ))
        {
            /* Extract transaction id from the pending event list
               corresponding to the input bit to work on the 
               header list using this extracted transaction id */
            icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                ICF_PENDING_EVENT_PA_CALL_TERM_REQ);
        }
        /* SPR 19590 : End */
        ret_val = icf_cc_09_call_term_handler(p_cc_pdb);
        
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }
    else
    {
        /*
         * If prack has comewith SDP set sdp present flag in cc_pdb
         */
        if(ICF_NW_INC_PRACK_SDP == (p_inc_prack->bitmask & 
                    ICF_NW_INC_PRACK_SDP ))
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
                                        &(p_inc_prack->p_sdp_list));
                    p_cc_pdb->p_call_ctx->offer_failure_reason = 
                                                ICF_SA_SERVER_INTERNAL_ERROR;
                    ret_val = icf_cc_send_ssa_media_prack_err(p_cc_pdb);
                    if(ICF_SUCCESS == ret_val)
                    {
                        /* Rel 8.1
                         * IT means local SDP is required by CC 
                         * Now the processing starts for the Call_modify_req 
                         * set the mmstype ICF_LOCAL_SDP_REQUIRED
                         * send mms request.
                         * move to state 4
                         * make ongoing event PA_CALL_MODIFY_REQ.
                         */
                        if(ICF_NULL != p_cc_pdb->p_call_ctx->p_stream_sdp_info &&
                           p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload)
                        {
                            /* Extract transaction id from the pending event list
                               corresponding to the input bit to work on the 
                               header list using this extracted transaction id */
                            icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                              ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ);
                            /* SPR 19590 : End */
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
                                    ret_val = icf_cc_start_media_timer(p_cc_pdb);
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
                }
ICF_CHECK_TUNNEL_MODE_END
            /* In tunnel mode, we do not understand streams and 
               their status.
            */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE) 

	            if ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM  ==
			        (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM &
			        p_inc_prack->bitmask)) || 
			            (ICF_NW_INC_MEDIA_ADD_STREAM  ==
			        (ICF_NW_INC_MEDIA_ADD_STREAM &
			        p_inc_prack->bitmask)))
                {
					icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
											&(p_inc_prack->p_sdp_list));
                            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_REJECT;
		            ret_val = icf_cc_send_ssa_media_prack_err(p_cc_pdb);
                            if (ICF_FAILURE != ret_val)
                            {
                                 p_cc_pdb->prack_reqd = ICF_TRUE;
                                 ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
                                 if (ICF_FAILURE != ret_val)
                                 {
                                     ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_10_AWT_ALERT_PRACK)
                                 }
                            }
                                 

		            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
		            return ret_val;
	            }
ICF_CHECK_TUNNEL_MODE_END
            }
	        /* This function shall validate the i'th level
             * matching and copy the received SDP into the 
             * call context
             */
            p_cc_pdb->p_list_1 = p_inc_prack->p_sdp_list;
            ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
                     
	        if (ICF_FAILURE == 
		        icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
	        {
                p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_SA_REJECT;
		        ret_val = icf_cc_send_ssa_media_prack_err(p_cc_pdb);
		        /* Check if ith level check has failed */
		        if (ICF_CC_STREAM_POS_VALIDATION_FAIL == (p_cc_pdb->common_bitmask & 
                            ICF_CC_STREAM_POS_VALIDATION_FAIL))
		        {
		            /* Invoke the generic call clear handler corresponding to that state */
		            ret_val = icf_cc_09_generic_err(p_cc_pdb); 
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

    /* Check if Ack for prov response can be sent or not */

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_SETUP_TYPE)
     
    if (ICF_SUCCESS == ret_val && (ICF_FALSE == p_cc_pdb->sdp_present))
    {
        /* Either PRACK came without SDP or it is NFS call */
        
        /* Send Ack for provisional response recd */
        ret_val = icf_cc_send_ssa_media_prov_resp_ack(p_cc_pdb);

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    /*SPR19574 If we have not completed resource resv UPDATE-200Ok
     Then only we should go in state 16 otherwise not*/
    if(ICF_FALSE == p_cc_pdb->p_call_ctx->resource_reservation_completed)
    {    
			while (ICF_NULL != p_list)
			{
				p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

				if(ICF_NULL == p_sdp->p_local_sdp)
				{
					p_list = p_list->p_next;
					continue;
				}

				/* we set the cc state to AWT_UPDATE only if remote has indicated 
				 * that the desired status at its end is MANDATORY */
                /*Precondition Enhancements: Set the state to AWT_UPDATE 
                  also when the desired status at remote end is OPTIONAL*/
				if((ICF_RESOURCE_RESERVATION_MANDATORY == 
                    p_sdp->p_local_sdp->remote_send.desired_status) ||  
				   (ICF_RESOURCE_RESERVATION_MANDATORY == 
                    p_sdp->p_local_sdp->remote_recv.desired_status) ||
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_send.desired_status) ||  
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_recv.desired_status))
				{

					/* Set Call State to AWT_UPDATE */
					ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_16_AWT_UPDATE)

					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			        return ret_val;
				}
                else
                {
                        /*This below leg is added for the scenario when
                         * desired status is mentioned as "none" by remote
                         * in initial INVITE and so it is not mandatory
                         * for peer to send the UPDATE now in this case if
                         * the alert or connect req are already buffered
                         * we need to process them as would have done in state 
                         * in state 14 after sending 200OK for UPDATE in
                         * "optional" and "mandatory" desired status case*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                    /*Process the pending event alert/connect set in 03 state*/
                    if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)))
                    {
                            /* Extract transaction id from the pending event list
                               corresponding to the input bit to work on the 
                               header list using this extracted transaction id */
                            icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                           ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
                            /* SPR 19590 : End */
                         /* Send 183 without SDP with prov response required */
                         p_cc_pdb->prack_reqd = ICF_TRUE;
                         if(ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
                         {
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
                    else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                               ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
                        && (ICF_SUCCESS == ret_val))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
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
                       else
                       {
                           ret_val = ICF_FAILURE;
                       }    
                     }
                     else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,\
                               ICF_PENDING_EVENT_PA_CALL_CONNECT))
                              && (ICF_SUCCESS == ret_val))
                     {
                         /* Extract transaction id from the pending event list
                            corresponding to the input bit to work on the 
                            header list using this extracted transaction id */
                         icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                              ICF_PENDING_EVENT_PA_CALL_CONNECT);
                         /* SPR 19590 : End */
                           ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
                           ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                           ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                      ICF_PENDING_EVENT_PA_CALL_CONNECT)
                           ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                           ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
                     }
                     else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                               ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
                               && (ICF_SUCCESS == ret_val))
                     {
                         /* Extract transaction id from the pending event list
                            corresponding to the input bit to work on the 
                            header list using this extracted transaction id */
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
                           else
                           {
                              ret_val = ICF_FAILURE;
                           }
                     }
                    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                 ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
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
                         else
                         {
                             ret_val = ICF_FAILURE;
                         }
                    } 
                    else
                    {
                         /*SPR 19502 When QOS attributes does have "none" in
                          * desired attributes received in initial INVITE and
                          * initially we have recvd the media_connect_req
                          * mainly connect/alert are not buffered then we
                          * shall be moving to state 03 as we expect alert and
                          * UPDATE to be send by peer is not mandatory.*/
                           ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
                     }
					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) 
			        return ret_val;
ICF_CHECK_TUNNEL_MODE_END
                }
				/* go to the next node in list */
				p_list = (icf_internal_list_st*)p_list->p_next;
			}

			/* Since remote did not indicate mandatory desired_status, 
			 * we do not wait for UPDATE from remote. send incoming_call_ind 
			 * to PA and set state to AWT_ALERT_RESP to await user_alerted 
			 * response from PA */
			/* Send an incoming call indication to PA */
			if (ICF_FAILURE  == 
						icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
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
    }
    else
    {
	    ICF_CC_SET_CALL_STATE(
			 p_cc_pdb,ICF_CC_05_AWT_CONNECT)
    }

ICF_CHECK_QOS_SUPPORT_END
#endif

        if (ICF_FAILURE == ret_val)
        {
        }
        else 
        {
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)  
           if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
                        && (ICF_SUCCESS == ret_val))
            {
                /* Extract transaction id from the pending event list
                   corresponding to the input bit to work on the 
                   header list using this extracted transaction id */
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
                /* Extract transaction id from the pending event list
                   corresponding to the input bit to work on the 
                   header list using this extracted transaction id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                           ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
                /* SPR 19590 : End */
               	/* Send 183 without SDP with prov response required */
                p_cc_pdb->prack_reqd = ICF_TRUE;

                if (ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
                {
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
                /* Extract transaction id from the pending event list
                   corresponding to the input bit to work on the 
                   header list using this extracted transaction id */
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
               /* Extract transaction id from the pending event list
                  corresponding to the input bit to work on the 
                  header list using this extracted transaction id */
               icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                               ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ);
               /* SPR 19590 : End */
               /* Send Alert with SDP with prov response required */
               p_cc_pdb->prack_reqd = ICF_TRUE;
               /*Fix merged for CSR_1-5417099*/
               ret_val = icf_cc_send_ssa_alert_media(p_cc_pdb);
               if(ICF_SUCCESS == ret_val)
               {
                   /* Change call state to Awaiting prov response
                    *  for alert
                    */
                   ICF_CC_SET_CALL_STATE(p_cc_pdb,
                           ICF_CC_10_AWT_ALERT_PRACK)
                   ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                          ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)
               }
           }
           else if((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,\
                           ICF_PENDING_EVENT_PA_CALL_CONNECT))
                   && (ICF_SUCCESS == ret_val))
           {
               /* Extract transaction id from the pending event list
                  corresponding to the input bit to work on the 
                  header list using this extracted transaction id */
               icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_CALL_CONNECT);
               /* SPR 19590 : End */
               ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)
               if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,\
                           ICF_PRACK_FOR_MEDIA_CONNECT)))
               {
                   /* Send Alert without SDP with prov response required */
                   p_cc_pdb->prack_reqd = ICF_TRUE;

                   if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
                   {
                       /* Change call state to Awaiting prov response
                        *  for alert
                        */
                       ICF_CC_SET_CALL_STATE(p_cc_pdb,
                               ICF_CC_10_AWT_ALERT_PRACK)
                   }
                   /*Fix merged for CSR_1-5417099*/
                   else
                   {
                       ret_val = ICF_FAILURE;
                   }
               }
               else
               {
                   ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                   /*  If Incoming call modify is ongoing
                    *  then set next call state as 05 because
                    *  once call modify cfmm will be received then 
                    *  after processing that next state shall be set as 
                    *  current state
                    */
                   if(ICF_CC_SSA_OFFER_CHANGE != p_cc_pdb->p_call_ctx->ongoing_event)
                   {
                       ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
                   }
                   else
                   {
                       ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
                   }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                   ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
ICF_CHECK_TUNNEL_MODE_END
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
                /* Extract transaction id from the pending event list
                   corresponding to the input bit to work on the 
                   header list using this extracted transaction id */
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

ICF_CHECK_QOS_SUPPORT_END            
        }
    } /* for prack without SDP or NFS call */
    else if (ICF_SUCCESS == ret_val && ICF_TRUE == p_cc_pdb->sdp_present)
    {
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

			if(ICF_INT_MODULE_INVALID != 
					 ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
            {
                    /*
                     * MMS cannot be sent to nw as request is pending from MM
                     * Mark offer change as pending event
                     */
                    ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                            ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP)

                    /* Set call state */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,
                            ICF_CC_13_WAIT_START_PROC_MEDIA)
            }
            else
            {          
                    /* Send MMS Request to MMI */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    ret_val = icf_cc_send_mms_req(p_cc_pdb);
                    if (ICF_SUCCESS == ret_val)
                    {
                        if(ICF_TRUE == p_cc_pdb->mms_sent)
                        {
                            /* Change state to Awaiting MMS response */
                            /* Need to set next state also */

                            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                    ICF_CC_10_AWT_ALERT_PRACK)


#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
    /*SPR19574 If we have not completed resource resv UPDATE-200Ok
     Then only we should go in state 16 otherwise not*/
    if(ICF_FALSE == p_cc_pdb->p_call_ctx->resource_reservation_completed)
    {    
			while (ICF_NULL != p_list)
			{
				p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

				if(ICF_NULL == p_sdp->p_local_sdp)
				{
					p_list = p_list->p_next;
					continue;
				}

				/* we set the cc state to AWT_UPDATE only if remote has indicated 
				 * that the desired status at its end is MANDATORY */
                /*Precondition Enhancements: Set the state to AWT_UPDATE 
                  also when the desired status at remote end is OPTIONAL*/
				if((ICF_RESOURCE_RESERVATION_MANDATORY == 
                    p_sdp->p_local_sdp->remote_send.desired_status) || 
				   (ICF_RESOURCE_RESERVATION_MANDATORY ==
                    p_sdp->p_local_sdp->remote_recv.desired_status) ||
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_send.desired_status) || 
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_recv.desired_status))
				{

					/* Set Next State to AWT_UPDATE */
					ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_16_AWT_UPDATE)
					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			        return ret_val;
				}
                else
                {
                        /*This below leg is added for the scenario when
                         * desired status is mentioned as "none" by remote
                         * in initial INVITE and so it is not mandatory
                         * for peer to send the UPDATE now in this case if
                         * the alert or connect req are already buffered
                         * we need to process them as would have done in state 
                         * in state 14 after sending 200OK for UPDATE in
                         * "optional" and "mandatory" desired status case*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                    /*Process the pending event media connect/alert/connect set in 03 state*/
                    if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
                           icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                       ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
                        /* SPR 19590 : End */
                         /* Send 183 without SDP with prov response required */
                         p_cc_pdb->prack_reqd = ICF_TRUE;
                         if(ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
                         {
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
                    else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                               ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
                        && (ICF_SUCCESS == ret_val))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
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
                       else
                       {
                           ret_val = ICF_FAILURE;
                       }    
                    }
                    else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,\
                               ICF_PENDING_EVENT_PA_CALL_CONNECT))
                              && (ICF_SUCCESS == ret_val))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
                        icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                             ICF_PENDING_EVENT_PA_CALL_CONNECT);
                        /* SPR 19590 : End */
                           ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
                           ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                           ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                      ICF_PENDING_EVENT_PA_CALL_CONNECT)
                           ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
                           ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
                    }
                    else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                               ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
                               && (ICF_SUCCESS == ret_val))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
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
                           else
                           {
                              ret_val = ICF_FAILURE;
                           }
                    }
                    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                 ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)))
                    {
                        /* Extract transaction id from the pending event list
                           corresponding to the input bit to work on the 
                           header list using this extracted transaction id */
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
                         else
                         {
                             ret_val = ICF_FAILURE;
                         }
                    }    
                    else
                    {    
                          ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
                    }          
					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			        return ret_val;
ICF_CHECK_TUNNEL_MODE_END
                }
				/* go to the next node in list */
				p_list = (icf_internal_list_st*)p_list->p_next;
			}
    }
    else
    {
	    ICF_CC_SET_CALL_STATE(
			 p_cc_pdb,ICF_CC_05_AWT_CONNECT)
    }
ICF_CHECK_QOS_SUPPORT_END
#endif
                        }
                        else
                        {
						    if(ICF_CC_ALL_STREAMS_DELETED == 
                                (ICF_CC_ALL_STREAMS_DELETED & p_cc_pdb->common_bitmask))
                            {
                                ret_val = icf_cc_send_ssa_media_prov_resp_ack(p_cc_pdb);
                                /* Fix for CSR 1-7042401 */
			        ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
				    ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED)
                                ret_val = icf_cc_09_generic_err(p_cc_pdb);
                                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                                return ret_val;
	                        }
                            else
                            {                                
                                p_cc_pdb->media_to_be_sent = ICF_TRUE;
                                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                                    ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
                                ret_val = icf_cc_send_ssa_media_prov_resp_ack(
                                        p_cc_pdb);
                            }
                        }
                     }
                 
             } /* for prack with sdp */
         }

        /*
         * If Signaling ongoing was not set and PA connect is not the pending 
         * event, still mms was not send means there was no change to applied 
         * towards media manager then send alert to SSA and set state as 
         * ICF_CC_10_AWT_ALERT_PRACK 
         */
        if(ICF_INT_MODULE_INVALID == 
                ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb) &&
                ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                        ICF_PENDING_EVENT_PA_CALL_CONNECT)
                && ICF_FALSE == p_cc_pdb->mms_sent)
        {

             /* Extract transaction id from the pending event list
                corresponding to the input bit to work on the 
                header list using this extracted transaction id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                                      ICF_PENDING_EVENT_PA_CALL_CONNECT);
             /* SPR 19590 : End */
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
			while (ICF_NULL != p_list)
			{
				p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

				if(ICF_NULL == p_sdp->p_local_sdp)
				{
					p_list = p_list->p_next;
					continue;
				}

				/* we set the cc state to AWT_UPDATE only if remote has indicated 
				 * that the desired status at its end is MANDATORY */
                /*Precondition Enhancements: Set the state to AWT_UPDATE 
                  also when the desired status at remote end is OPTIONAL*/
				if((ICF_RESOURCE_RESERVATION_MANDATORY ==
                    p_sdp->p_local_sdp->remote_send.desired_status) || 
				   (ICF_RESOURCE_RESERVATION_MANDATORY ==
                    p_sdp->p_local_sdp->remote_recv.desired_status) ||
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_send.desired_status) || 
                   (ICF_RESOURCE_RESERVATION_OPTIONAL ==
                    p_sdp->p_local_sdp->remote_recv.desired_status))
				{

					/* Set Call State to AWT_UPDATE */
					ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_16_AWT_UPDATE)

					ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
			        return ret_val;
				}
				/* go to the next node in list */
				p_list = (icf_internal_list_st*)p_list->p_next;
			}

			/* Since remote did not indicate mandatory desired_status, 
			 * we do not wait for UPDATE from remote. send incoming_call_ind 
			 * to PA and set state to AWT_ALERT_RESP to await user_alerted 
			 * response from PA */
			/* Send an incoming call indication to PA */
			if (ICF_FAILURE  == 
						icf_cc_send_pa_incoming_call_ind(p_cc_pdb))
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

ICF_CHECK_QOS_SUPPORT_END
#endif

ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
            /*
             * Set prack required flag to true. This would ensure even if the 
             * configuration has changed the prack required flag to FALSE we 
             * would still continue sending the message with 100 rel in require 
             * header. This would ensure both provisional messages would either 
             * be sent with 100 rel or without 100 rel
             */
            
if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
        && (ICF_SUCCESS == ret_val))
{
    /* Extract transaction id from the pending event list
       corresponding to the input bit to work on the 
       header list using this extracted transaction id */
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
    /* Extract transaction id from the pending event list
       corresponding to the input bit to work on the 
       header list using this extracted transaction id */
    icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                     ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP);
    /* SPR 19590 : End */
    /* Send 183 without SDP with prov response required */
    p_cc_pdb->prack_reqd = ICF_TRUE;

    if (ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
    {
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
    /* Extract transaction id from the pending event list
       corresponding to the input bit to work on the 
       header list using this extracted transaction id */
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
    /* Extract transaction id from the pending event list
       corresponding to the input bit to work on the 
       header list using this extracted transaction id */
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
else if((ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,\
                ICF_PENDING_EVENT_PA_CALL_CONNECT))
        && (ICF_SUCCESS == ret_val))
{
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)
    if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,\
                ICF_PRACK_FOR_MEDIA_CONNECT)))
    {
        /* Send Alert without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
        }
        /*Fix merged for CSR_1-5417099*/
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
        ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)
    }
}
ICF_CHECK_QOS_SUPPORT_END

}
}    
if (ICF_FAILURE == ret_val)
{
}
else 
{
    ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
       /*If media is already commited*/
		if ((ICF_SUCCESS == ret_val) &&
            ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
        {
               /*PA call connect is pending event ant no mms was sent
                * either due to SDP 
                    * being absent or same media parameters received in SDP*/
            if((ICF_SUCCESS == ret_val) && 
                    ((ICF_NULL != 
                    ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_CALL_CONNECT))
                    &&
                    ICF_FALSE == p_cc_pdb->mms_sent))
            {
                /* Extract transaction id from the pending event list
                   corresponding to the input bit to work on the 
                   header list using this extracted transaction id */
                icf_cc_extract_txnid_from_pending_event_list(p_cc_pdb,
                             ICF_PENDING_EVENT_PA_CALL_CONNECT);
                /* SPR 19590 : End */
                /*If media ongoing is false*/
                if(ICF_INT_MODULE_INVALID == 
                ICF_CC_GET_MEDIA_ONGOING( p_cc_pdb))
                {
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
        }
ICF_CHECK_QOS_SUPPORT_END        
    }
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a Call Cancel Request from SSA 
 *                  (SIP CANCEL received from peer). This is same as the event 
 *                  ICF_CC_SSA_CALL_CANCEL_REQ received in 
 *                  ICF_CC_01_AWT_CMS_RESP state.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_01_ssa_call_cancel_req() that does same */
    ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);

   /*
	* Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
	* Since we are call the same handler in state 9 so we should reset this bit 
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
 * FUNCTION:        icf_cc_09_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a generic error indication.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_09_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Set release type as internal error */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

    /*
     * Send a DMS request to MMI
     */
    p_cc_pdb->forceful_delete = ICF_FALSE;

	if ( ICF_SA_NETWORK_ERROR == 
		p_cc_pdb->p_call_ctx->offer_failure_reason)
	{
		if ( ICF_FAILURE == icf_cc_send_ssa_alert_prack_err(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
	}
    /* Fix For SPR:20078*/    
    /* While handling the generic error in ICF_CC_03_AWT_ALERT_RESP if the
     * value of is_call_modify_ongoing is set to TRUE it indicates that we
     * have received ICF_CALL_MODIFY_REQ with/without sdp and this  API need to
     * be responded with failure ICF_CALL_MODIFY_RESP .We are handling this scenario in
     * icf_cc_14_generic_err.
     */
     if (ICF_SUCCESS == ret_val)
     {

         if(ICF_TRUE == p_cc_pdb->p_call_ctx->is_call_modify_ongoing)
         {
             ret_val = icf_cc_14_generic_err(p_cc_pdb);
             p_cc_pdb->p_call_ctx->is_call_modify_ongoing = ICF_FALSE;
         }
         else
         {

             if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
             {
                 ret_val = ICF_FAILURE;
             } /* if(ICF_FAILURE == icf_st... */
             else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
             {
                 ret_val = ICF_FAILURE;
             } /* if(ICF_FAILURE == icf_cc... */
             else if(ICF_FAILURE == icf_cc_send_ssa_reject(p_cc_pdb))
             {
                 /* Return error to top level function,
                  * call will be forcefully cleared*/
                 ret_val = ICF_FAILURE;
             }
             else
             {
                 /*
                  * Update the states in the call context
                  */
                 ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
                 ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                             ICF_CC_08_AWT_CALL_TERMINATE_RESP)
             } /* else */
         }
     }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_call_term_handler
 *
 * DESCRIPTION:     This is a utility function that does common processing for
 *                  termination in states ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  and ICF_CC_10_AWT_ALERT_PRACK.
 *                  The function
 *                  a. Sends Ack to SSA for provisional response received
 *                  b. Sends Call Reject to SSA with appropriate release cause
 *                  c. Sends DMS Request to MMI
 *                  d. Resets MM error and Call Terminate Request pending events
 *                  e. Changes call state to Awaiting DMS Response
 *
 ******************************************************************************/
icf_return_t icf_cc_09_call_term_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send Ack for provisional response recd */
    ret_val = icf_cc_send_ssa_media_prov_resp_ack(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Send Call Reject to SSA with appropriate release cause */
        if (ICF_NULL != ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
        {
            /*
             *Check if call clear reason was user bsy. If it is we need to check
             *if call forwarding is active on line
             */ 
            if(ICF_CALL_CLEAR_REASON_USER_BUSY == 
                    p_cc_pdb->p_call_ctx->release_cause)
            {
                ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
            }
        }
        else
        {
            /* SIP 500 needs to be sent to peer */
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_SYSTEM_ERROR )
        }

        /*If status was not returned as ICF_STATUS_CALL_FWD_BUSY_LINE 
         * continue with normal call clearing*/
        if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE != 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
        {

            ret_val = icf_cc_send_ssa_reject(p_cc_pdb);

            /* Send DMS Request to MMI */
            if (ICF_SUCCESS == ret_val && 
                    ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                                        ICF_CALL_CLEAR_STATUS_MM_CLEARED))
            {
                p_cc_pdb->forceful_delete = ICF_FALSE;
            

                if (ICF_SUCCESS == icf_cc_send_dms_req(p_cc_pdb))
                {
                    /* Change state to Awaiting DMS response */
                    ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                            ICF_CC_07_AWT_DMS_RESP)
                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else
            {
                /* Change state to Awaiting Call Terminate response */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,
                        ICF_CC_08_AWT_CALL_TERMINATE_RESP)
            }
        }/* if(ICF_SUCCESS == ret_val && ... */
    }/* * End if(ICF_SUCCESS == ret_val) */

    if(ICF_SUCCESS == ret_val) 
    {
        /* Reset pending events */
        ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, 
                ICF_PENDING_EVENT_MM_ERR_IND)

        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_MM_ERR_IND)

        ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, 
                ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_TERM_REQ)

        ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, 
                ICF_PENDING_EVENT_INT_ERR)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_INT_ERR)

    } /* End if(ICF_SUCCESS == ret_val) */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_09_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_09_AWT_MEDIA_PARAM_PRACK 
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_09_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,ICF_REL_CAUSE_SERV_INT_ERR)

    icf_cc_send_ssa_reject(p_cc_pdb);
    
    /*Handling would be same as that of forceful call clear in atate IDLE*/
    icf_cc_00_clear_call (p_cc_pdb);

    return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a media connect from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_media_connect_req_st         *p_media_connect_req =
                (icf_media_connect_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;         

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    if(ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
			    ICF_PENDING_EVENT_PA_CALL_CONNECT))
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
        
        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)

        /* In tunnel mode, we will not process rejected streams */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
		/* 
		 * Check the rejected streams present in media_connect request 
		 * and populate the same in the rejected stream array in call context 
		 */
		if (ICF_NULL != p_media_connect_req->count )
		{
            ret_val = icf_cc_validate_n_add_rejected_streams_in_call_ctxt(\
		    		    p_cc_pdb,p_media_connect_req->deleted_media_streams,\
					    p_media_connect_req->count);
	    }
ICF_CHECK_TUNNEL_MODE_END        
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_alert_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent 
 *                  to peer and it receives a media connect from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_ALERT_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_alert_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_pa_cc_user_alerted_req_st    *p_alert_req =
                (icf_pa_cc_user_alerted_req_st*)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;         

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    if(ICF_NULL == ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
			    ICF_PENDING_EVENT_PA_CALL_CONNECT))
    {  
        ret_val = icf_cc_reset_media_pending_events(p_cc_pdb);
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

        /* In tunnel mode, we will not process rejected streams */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)    
        /* 
         * Check the rejected streams present in alert request 
         * and populate the same in the rejected stream array in call context 
         */
        if (ICF_NULL != p_alert_req->count )
        {
            ret_val = icf_cc_validate_n_add_rejected_streams_in_call_ctxt(\
                   p_cc_pdb,p_alert_req->deleted_media_streams,\
                   p_alert_req->count);
        }
ICF_CHECK_TUNNEL_MODE_END
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

