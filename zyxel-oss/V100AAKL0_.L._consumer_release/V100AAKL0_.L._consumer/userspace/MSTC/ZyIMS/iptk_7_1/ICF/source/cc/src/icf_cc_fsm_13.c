/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_13.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_13_WAIT_START_PROC_MEDIA state.
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 10-Dec-2004  Pankaj Negi     Non Service LLD      Initial
 * 22-dec-2004  Rohit Aggarwal                       Addition of stats
 * 30-Mar-2006	Anuradha Gupta	ICF 5.0				Call Modify additions
 *
 * 26-Mar-2007  Deepti Goyal    ICF 6.4             Changes for Mute/Unmute req.
 *
 * Copyright (c) 2007, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  alert without media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_alert(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ret_val = icf_cc_04_ssa_alert(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it
 *                  receives alert with media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_alrt_with_media(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * As the handling for this event is the same in the case of
     * ICF_CC_SSA_ALERT,CC will call icf_cc_13_ssa_alert
     */
    ret_val = icf_cc_04_ssa_alrt_with_media(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  connect (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_call_connect(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_connect_st   *p_connect_ack = ICF_NULL;
    p_connect_ack = (icf_nw_inc_connect_st *)
                            (p_cc_pdb->p_internal_msg->p_msg_data);

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* We are in this state it means media is already commited */
    /*
     * Stop the call setup/alert timer . No need to check return value
     */
    icf_cc_stop_current_timer(p_cc_pdb);
    /*
     * Send ACK to SSA
     */

    if(ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
        /*Return ret_val as failure. Call will be now forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Send CONNECT to PA
     */
    else if(ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
    {
        /*Return ret_val as failure. Call will be now forcefully cleared*/
        ret_val = ICF_FAILURE;
    }
    /*
     * Update the next state in the call context
     */
    else
    {
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_CONNECT_DONE)
        /*
         * Set next state to connected
         */

        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
    }
    /*Delete SDP*/
    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,
                                      &p_connect_ack->p_sdp_list);
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_start_proc_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  the same
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_START_CALL_PROC_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_13_start_proc_media(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

#if 0
    if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
          ICF_PENDING_EVENT_CALL_MUTE))
    {
            /* Set call mute n media state  */
        ICF_CC_SET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
    }

    else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_CALL_UNMUTE))
    {
            /* Set call mute n media state  */
            ICF_CC_RESET_MEDIA_STATE(p_cc_pdb,ICF_MEDIA_STATE_CALL_MUTE)
    }
#endif
    /*
     * Send Modify Media Session to the MMI
     */
    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        /* Set return value as failure. call will be cleared forcefully*/
        ret_val = ICF_FAILURE;
    }
    else
    {
         /* able to send mms succesfully. set the ongling event
         * Mark ongoing event depending on pending event
         */

        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
        {

            /* Set Offer Change as Ongoing event and remove it from pending list */

            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_OFFER_CHANGE)

            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_REQ)

        }
ICF_HANDLE_REINVITE_START

		else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ))
        {

            /* Set Call modify request as Ongoing event and remove it from pending list */

            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_MODIFY_REQ)

            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ)

        }
ICF_HANDLE_REINVITE_END
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP))
        {
            /* Set Offer Change as Ongoing event
             * Call mute and unmute will be applied in this mms
             * so remove call mute  and unmute from pending list */

            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                ICF_CC_ALERT_PRACK)

            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP)

        }
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP))
        {
            /* Set Offer Change as Ongoing event
             * Call mute and unmute will be applied in this mms
             * so remove call mute  and unmute from pending list */

            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                ICF_CC_MEDIA_PARAM_PRACK)

            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP)

        }
        else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                        ICF_PENDING_EVENT_PA_CALL_CONNECT))) 
        {
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                    ICF_CC_PA_CALL_CONNECT)
                
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)

        }
        /* User answered is a pending event */

#if 0
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_CALL_MUTE))
        {
            /* Set call mute as ongoing req */
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                ICF_CC_CALL_MUTE_REQ)
        }
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_CALL_UNMUTE))
        {
            /* Set call unmute as ongoing req */
            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb, ICF_CC_CALL_UNMUTE_REQ)
        }
#endif
    }/* Successful processing of start proc req*/

    /* Call mute and unmute will be applied in this mms
     * so remove call mute  and unmute from pending list */
#if 0
    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_CALL_MUTE)

    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_CALL_UNMUTE)
#endif
    /*
     * Update the states in the call context
     */
    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  call termination request from SSA(SIP BYE)
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_call_term_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Handling is same as for await mms state */
    ret_val = icf_cc_04_ssa_call_term_req(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it
 *                  receives Remote Call Reject from SSA
 *                  (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_remote_reject(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * invoke icf_cc_04_ssa_remote_reject as it is same handling to when
     * we are awaiting mms resp
     */
    ret_val = icf_cc_04_ssa_remote_reject(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives reroute call Req from SSA
 *                  (SIP 302 received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_redirect(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * As the handling for this event is the same in the state
     * ICF_CC_04_AWT_MMS_RESP,we will call their
     * state handler
     */
    ret_val = icf_cc_04_ssa_redirect(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_pa_call_term_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * As the handling for this event is the same in the state
     * ICF_CC_04_AWT_MMS_RESP,we will call their
     * state handler
     */
    ret_val = icf_cc_04_pa_call_term_req(p_cc_pdb);


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and offer change
 *                  is recvd from the SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_13_ssa_offer_change(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ret_val = icf_cc_04_ssa_offer_change(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and MM error indication is received from MMI
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_13_mm_error_ind(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ret_val = icf_cc_04_mm_error_ind(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and Timer expires.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_13_timer_expiry(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * As the handling for this event is the same in the state
     * ICF_CC_04_AWT_MMS_RESP,we will call their
     * state handler
     */
    ret_val = icf_cc_04_timer_expiry(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  PRACK for 180 is received
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_ALERT_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_13_alert_prack(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*
     * As the handling for this event is the same in the state
     * ICF_CC_04_AWT_MMS_RESP,we will call their
     * state handler
     */
    ret_val = icf_cc_04_alert_prack(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}
/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  ACK for 200 OK of INVITE is recvd
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_connect_ack(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * As the handling for this event is the same in the state
     * ICF_CC_04_AWT_MMS_RESP,we will call their
     * state handler
     */
    icf_cc_04_ssa_connect_ack(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_13_ssa_call_cancel_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * As the handling for this event is the same in the state
     * ICF_CC_03_AWT_ALERT_RESP,we will call their
     * state handler
     */
    ret_val = icf_cc_01_ssa_call_cancel_req(p_cc_pdb);

	/*
	 * Hadling of cancel request in state 1 set PA cleared bit for incomming call. 
	 * Since we are call the same handler in state 13 so we should reset this bit 
	 * as the PA interface would also need to be cleared
	 * So Reset PA cleared bit 
	 */
    ICF_CC_RESET_CALL_CLR_STATUS(p_cc_pdb,
            ICF_CALL_CLEAR_STATUS_PA_CLEARED)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_13_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
       
    ret_val = icf_cc_04_generic_err(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;

}

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and offer change
 *                  is recvd from the PA
 *
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_13_pa_call_modify_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ret_val = icf_cc_04_pa_call_modify_req(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
ICF_HANDLE_REINVITE_END
