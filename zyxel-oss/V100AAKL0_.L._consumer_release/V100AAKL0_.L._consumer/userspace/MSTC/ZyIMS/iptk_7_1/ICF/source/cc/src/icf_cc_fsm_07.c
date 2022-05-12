/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_07.c
 *
 * Purpose          : This file contains the event handlers in 
 *                    ICF_CC_07_AWT_DMS_RESP state.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 10-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 22-Dec-2004  Rohit Aggarwal                      Addition of stats
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 18-May-2005  Sumit Gupta     SPR 7583            Put an additional check to 
 *                                                  see if network cleared flag 
 *                                                  is set when DM resp is 
 *                                                  received CC should set next 
 *                                                  state as IDLE and send 
 *                                                  service terminate in status 
 *                                                  to CM
 * 07-Mar-2006	Anuradha Gupta						Changed the Invalid event number 
 * 08-Mar-2006  Aman Aggarwal					    ICF merging with ICF2.0
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 11-OCT-2007  Abhishek Dhammawat spr16946         Modified icf_cc_07_ssa_connect_ack
 *                                                  setting state to 08.
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_setup_resp
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Setup Ack
 *                  from SSA (SIP 100 Trying received from peer). 
 *                  In this case, it needs to send Call Cancel Request to SSA 
 *                  and change previous state to Await Alert Response 
 *                  if the previous state is Await Setup Response from SSA.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * In case the previous state is Await Setup Response from SSA, send CANCEL
     * to peer and change previous state to invalid.
     * Else do nothing.
     */
    
    /* Check previous state in call context pointer */
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PREVIOUS_STATE)
    if (ICF_CC_02_AWT_SSA_SETUP_RESP == ICF_CC_GET_PREV_STATE(p_cc_pdb))
    {
        /* Send call cancel request to peer */
        ret_val = icf_cc_send_ssa_cancel_req(p_cc_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /* Set previous state to Invalid State */
            ICF_CC_SET_PREV_STATE(p_cc_pdb, ICF_CC_18_CALL_STATE_INVALID) 
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_dms_resp
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Delete
 *                  Media Session Response from MMI.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_dms_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_NEXT_STATE)
    /* ---------------------------------------------------------------------- */
    /* Check if Await CMS response is next state - Handling of 302 response */
    /* ---------------------------------------------------------------------- */
    if (ICF_CC_01_AWT_CMS_RESP == ICF_CC_GET_NEXT_STATE(p_cc_pdb))
    {
        /* If media is commited remote sdp exist, 
           CC will send cms and we need to free sdp before that else 
           INVITe will be sent and we will reuse local sdp */
    
        /*Free mmi_context remote received SDP*/
        /* This scenario will be triggered when remote party sends
         * 183/180with sdp followed by 302. In this case we need to 
         * first clear the existing media and send a new offer */
        icf_cc_free_forked_resp_list(p_cc_pdb,p_cc_pdb->p_ecode);
        icf_cc_clear_all_stream_sdp(p_cc_pdb);
        p_cc_pdb->p_call_ctx->media_committed = ICF_FALSE;

	if (ICF_FAILURE == icf_cc_populate_local_sdp(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_cc_send_cms_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        } /* else if (ICF_FAILURE == ipp... */
        
        if (ICF_SUCCESS == ret_val)
        {
            /* Change call_state to Awaiting CMS response */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_01_AWT_CMS_RESP)
                
            /* Set next state to invalid */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_18_CALL_STATE_INVALID)
        }
    }
    /* ---------------------------------------------------------------------- */
    /* Check if Wait for Start proc signaling is next state */
    /* ---------------------------------------------------------------------- */
    else if (ICF_CC_12_WAIT_START_PROC_SIG == 
            ICF_CC_GET_NEXT_STATE(p_cc_pdb))
    {
        /* Set MM cleared bit in call_clear_status */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)

        /*CSR 1-4860705: clear the PA interface so that next
          call is allowed*/
        icf_cc_07_process_dms_resp(p_cc_pdb);  

        /* 
         * BYE has not yet been sent since signaling ongoing was true when 
         * termination was initiated, so go to wait for signal proc now
         */
         ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_12_WAIT_START_PROC_SIG)
    }
    else if(ICF_CC_02_AWT_SSA_SETUP_RESP ==
                ICF_CC_GET_PREV_STATE(p_cc_pdb))
        
    {
        /* Set MM cleared bit in call_clear_status */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)
        
        /* Send forceful call clear to SSA and mark it as cleared */
        ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
        /* Call function to handle dms resp */         
        ret_val = icf_cc_07_process_dms_resp(p_cc_pdb);
    }

    /* ---------------------------------------------------------------------- */
    /* Normal DMS processing */
    /* ---------------------------------------------------------------------- */
    else
    {
        /* Set MM cleared bit in call_clear_status */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)

        ret_val = icf_cc_07_process_dms_resp(p_cc_pdb);
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_call_term_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call 
 *                  Terminate Request from SSA (SIP BYE received from peer). 
 *                  In this case, it will send Call Terminate Response to SSA 
 *                  and invoke network clear handler. 
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send Call Terminate Response to SSA */
    ret_val = icf_cc_send_ssa_call_term_rsp(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Invoke the network clear handler to set Network cleared bit */
        ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_remote_reject
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Remote Call 
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer). 
 *                  In this case, it will acknowledge remote reject and invoke  
 *                  network clear handler.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send Remote Reject Response to SSA */
    ret_val = icf_cc_send_ssa_reject_ack(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Invoke the network clear handler to set Network cleared bit */
        ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_pa_call_term_req
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call 
 *                  Terminate Request from PA. 
 *                  In this case, simply mark release type as PA initiated.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set release type as PA initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_call_term_resp
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call  
 *                  Terminate Response from SSA (SIP 200 OK in response to BYE) 
 *                  In this case, simply invoke network clear handler.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Invoke the network clear handler to set Network cleared bit */
    ret_val = icf_cc_nw_clear_handler(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_offer_change
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a session
 *                  param change from SSA (SIP UPDATE/RE-INVITE received from 
 *                  peer). 
 *                  In this case, simply send error response to SSA (SIP 500 to 
 *                  be sent for UPDATE/REINVITE received from peer).
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st *p_offer_chg_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* 
     * Invoke the function that sends reject (SIP 3xx/4xx/5xx) to peer. For 
     * this, first fill appropriate call_context->release_cause that would be 
     * used by SSA to decide the exact message (500 in this case) 
     * to be sent to peer.
     */
    if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_offer_chg_req->bitmask))
    {
            icf_cc_delete_inc_media_sdp_info_list(
                    p_cc_pdb,&(p_offer_chg_req->p_sdp_list));
    }

    ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, ICF_CALL_CLEAR_REASON_SYSTEM_ERROR)

    p_cc_pdb->p_call_ctx->offer_failure_reason = 
        ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

    ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);     
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_timer_expiry
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Release Timer expires.
 *                  In this case, clear all interfaces.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_07_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send a forceful DMS request to MMI */
    p_cc_pdb->forceful_delete = ICF_TRUE;
    ret_val = icf_cc_send_dms_req(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* 
         * At this stage, we can assume the MM interface to be clear since DMS 
         * Request has already been sent to MMI, hence set the MM cleared bit 
         * in call_clear_status directly.
         */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_MM_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_MM_CLR)

        /*
         * Also, Call Terminate Request has already been sent to SSA before 
         * sending DMS Request to MMI. So, directly set the network clear bit 
         * in call_clear_status and clear signaling context and the Call 
         * Terminate Response from SSA received later would be ignored.
         */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)

        /*
         * Further, the PA interface needs to be cleared. If call clear was PA 
         * initiated, then send Call Terminate Response to PA else send Call 
         * Terminate Request to PA but do not wait for response. In either 
         * case, after sending the Request/Response to PA, set PA cleared bit 
         * in call_clear_status and invoke clear handler. The Call Terminate 
         * Response received later from PA will be ignored.
         */
        if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
        {
            /* Send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
        }
        else
        {
            /* Send Call Terminate Response to PA */
            ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
        }

        /* 
         * Now that all the three interfaces have been cleared, invoke the PA 
         * clear handler to do the call context clean up
         */
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
        }
    } /* for return value of send DMS req */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_generic_err
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a generic 
 *                  error indication.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_07_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* 
	* If generic error is received and next state is set as 
	* ICF_CC_12_WAIT_START_PROC_SIG means nothing would have been sent to 
	* SSA so generic error would be received from service so check if service
	* has cleared the signaling ongoing flag the send bye on network and set 
	* the next state as ICF_CC_08_AWT_CALL_TERMINATE_RESP
	*/ 
    if (ICF_CC_12_WAIT_START_PROC_SIG ==
            ICF_CC_GET_NEXT_STATE(p_cc_pdb) ||
            (ICF_INT_MODULE_INVALID == 
                     ICF_CC_GET_SIG_ONGOING(p_cc_pdb)))
    {
        /* Send Call Terminate Request to SSA */
        ret_val = icf_cc_send_ssa_call_term_req(p_cc_pdb);

        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
    }

    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_process_dms_resp
 * 
 * DESCRIPTION:     This is a utility function that is used in processing DMS 
 *                  response. It handles the response when Next state is not 
 *                  set to Await CMS response or Wait for Start Proc Signaling.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_process_dms_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* ---------------------------------------------------------------------- */
    /*
     * If it is an incoming call and prev_state is Awaiting CMS response, 
     * then there is no need to send Call Terminate to PA since Incoming Call 
     * indication has not yet been sent to PA. In this case, simply clear all 
     * the interfaces and clear the call.
     */
    /* ---------------------------------------------------------------------- */
    
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_DIR)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_PREVIOUS_STATE)

    if ((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, 
                    ICF_CALL_DIR)) 
           && (ICF_CC_01_AWT_CMS_RESP == ICF_CC_GET_PREV_STATE(p_cc_pdb)))
    {
        /* Set PA cleared bit */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)

        /*If network cleared bit is not set then set call state as 
         * ICF_CC_08_AWT_CALL_TERMINATE_RESP*/
        if (ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED))
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                    ICF_CC_08_AWT_CALL_TERMINATE_RESP)
        }
        /*Otherwise set call state as idle*/
        else
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_00_IDLE)
            ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
        }

    }
    /* ---------------------------------------------------------------------- */
    /* Clear NW and PA interfaces */
    /* ---------------------------------------------------------------------- */
    else
    {
        ret_val = icf_cc_07_clear_interfaces(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_clear_interfaces
 * 
 * DESCRIPTION:     This is a utility function that is used in processing DMS 
 *                  response. It handles the response when Previous state is 
 *                  not set to Await CMS response.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_clear_interfaces(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
    /* ---------------------------------------------------------------------- */
    /* The PA interface is not cleared and  PA intitiated release type not set */
    /* ---------------------------------------------------------------------- */
    if ((ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED))
					&& ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                ICF_REL_TYPE_PA_INITIATED))
    {
        /* Not PA initiated, send Call Terminate Request to PA */
        ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /* Go to state Awaiting Call Terminate Response */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                    ICF_CC_08_AWT_CALL_TERMINATE_RESP)
        }
    }
    /* ---------------------------------------------------------------------- */
    /* PA initiated release type set */
    /* ---------------------------------------------------------------------- */
    else if(ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED))
    {
        /* Send Call Terminate Response to PA */
        ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);

        if (ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ret_val;
        }

        /* Set PA cleared bit */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)
	}

	/* If PA cleared bit is set */
	if(ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED))
	{
        /* Check whether network cleared or not */
        if ((ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_NW_CLEARED)) 
                && (ICF_CC_02_AWT_SSA_SETUP_RESP == 
                    ICF_CC_GET_PREV_STATE(p_cc_pdb)))
        {
            /* NW clear bit not set, prev state is Await Setup resp from SSA */

            /*
             * If prev state is Await SSA Setup Response, it means that 
             * Call Cancel Request to SSA has not been sent yet, therefore 
             * directly clear the signaling context and entire call context.
             */
            ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
        }
        else if ((ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_NW_CLEARED)) 
                && (ICF_CC_02_AWT_SSA_SETUP_RESP != 
                    ICF_CC_GET_PREV_STATE(p_cc_pdb)))
        {
            /* Go to Await Call Terminate Response from SSA */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                    ICF_CC_08_AWT_CALL_TERMINATE_RESP)
        }
        else if (ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb, 
                        ICF_CALL_CLEAR_STATUS_NW_CLEARED))
        {
            /* NW clear bit is set */
            ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
            
        } /* for network_clear */

    } /* for PA initiated Release */
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_call_connect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a connect 
 *                  from SSA.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
  
    icf_cc_free_ssa_msg(p_cc_pdb);
 
    if (ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE ==icf_cc_send_ssa_call_term_req(p_cc_pdb))
    {
        /* Return error to top level function,
         * call will be forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
 
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_redirect
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives  
 *                  In this case, it will acknowledge remote reject and and it 
 *                  receives reroute call Req from SSA (SIP 302 received from 
 *                  peer) invoke  
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if (ICF_FAILURE == icf_cc_send_ssa_redirect_ack(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */
    else
    {
        /* Invoke the network clear handler to set Network cleared bit */
        ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting for DMS 
 *                  response  from MMI and it receives SIP ACK for 200 OK sent 
 *                  for incoming INVITE.
 *                  In this case, CC should send bye on network.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
  
    icf_cc_free_ssa_msg(p_cc_pdb);
		 
    /*Send bye on network*/
    if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
     {
         ret_val = ICF_FAILURE;
     }
    else
     {
         /* spr16946 Call context does not gets deallocated in case of 
            outgoing call when 200OK is retransmitting and term call request
            comes from PA before recieving ACK. Reason being besides sending BYE
            from SSA on receiving ACK next state is still 12 while it is not
            required becuase CC does not need to wait for signalling to start
            a transaction as BYE is send so set the state to 8 */
         ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
     }

    /* Set the stat for unsuccessfull call */
    ICF_STAT(p_cc_pdb->p_glb_pdb,
                     ICF_STAT_UNSUCC_CALLS)
    
     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting for DMS 
 *                  response  from MMI and it receives SIP ACK for 200 OK sent 
 *                  for incoming reINVITE.
 *                  In this case, CC should send bye on network.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REINVITE_ACK
 *
 ******************************************************************************/
 /*SPR 10922 fix*/
icf_return_t icf_cc_07_ssa_reinvite_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    
	
	if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
	{
		ret_val = ICF_FAILURE;
	}
	else
	{
		ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_08_AWT_CALL_TERMINATE_RESP)
	}

    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}
ICF_HANDLE_REINVITE_END
