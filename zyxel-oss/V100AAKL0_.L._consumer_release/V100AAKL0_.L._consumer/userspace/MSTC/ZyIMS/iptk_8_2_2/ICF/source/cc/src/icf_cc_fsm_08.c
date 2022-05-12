/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_08.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_08_AWT_CALL_TERMINATE_RESP state.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 13-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 24-Dec-2004  Rohit Aggarwal  Non Service LLD     Compilation fixes
 * 
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 07-Jul-2009  Anuradha Gupta  SPR 18929           Modified icf_cc_08_ssa_call_term_resp
 * 24-Jul-2009  Anuradha Gupta  SPR 19097           Modified icf_cc_08_ssa_call_term_resp
 * 19-Aug-2009 Abhishek Dhammawat SPR 19941         Modified
 *                                                  icf_cc_08_pa_call_term_resp
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_cc_macro.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  a Setup Ack from SSA (SIP 100 Trying received from peer).
 *                  In this case, it needs to send Call Cancel Request to SSA
 *                  and change previous state to Await Alert Response. This is 
 *                  same as ICF_CC_SSA_SETUP_RESP event in 
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_07_ssa_setup_resp() that does same handling */
    ret_val = icf_cc_07_ssa_setup_resp(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  Call Terminate Request from SSA (SIP BYE recd from peer). 
 *                  In this case, it will send Call Terminate Response to SSA
 *                  and invoke network clear handler. This is same as 
 *                  ICF_CC_SSA_CALL_TERMINATE_REQ event in  
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_07_ssa_call_term_req() that does same */
    ret_val = icf_cc_07_ssa_call_term_req(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  Call Reject from SSA (SIP 3xx/4xx/5xx received from peer). 
 *                  In this case, it will acknowledge remote reject and invoke
 *                  network clear handler. This is same as 
 *                  ICF_CC_SSA_REMOTE_REJECT event received in state 
 *                  ICF_CC_07_AWT_DMS_RESP.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_07_ssa_remote_reject() that does same handling */
    ret_val = icf_cc_07_ssa_remote_reject(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  Call Terminate Request from PA.
 *                  In this case, there is a race between CC sending Call 
 *                  Terminate Request to PA on getting DMS response and PA 
 *                  sending Call Terminate Request to CC.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_08_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send Call Terminate Response to PA */
    ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Invoke PA clear handler */
        ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_pa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  Call Terminate response from PA. 
 *                  In this case, simply set PA clear bit in call_clear_status 
 *                  and invoke call context clear handler if not already done.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Invoke PA call clear handler */
    ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
    /*SPR 19941 - If the call context is cleared then the flag for
      detaching call ctx from line has been set in the icf_cc_pa_clear_handler
      therfore we shall be using the same here and invoke 
      icf_dbm_detach_cc_from_line for detaching. This is used to resolve
     the issue reported in SPR 19463 where there was 8 bytes leak during xconf
     add api cases running in succession leading to leak*/
    if((ICF_SUCCESS == ret_val) &&
       (ICF_NULL != p_cc_pdb->p_call_ctx) &&     
       (ICF_TRUE == p_cc_pdb->p_call_ctx->detach_cc_from_line))
    {
          p_cc_pdb->p_call_ctx->detach_cc_from_line = ICF_FALSE;
          icf_dbm_detach_cc_from_line(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->line_id,
                p_cc_pdb->p_call_ctx,
                p_cc_pdb->p_ecode);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  Call Terminate response from SSA (SIP 200 OK in response 
 *                  to BYE).
 *                  In this case, simply invoke network clear handler.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Invoke network clear handler */
    ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
    /* SPR 19097: Set the status as CALL or SERVICE terminate
     * only when all the inetrfaces have been cleared.
     */ 
   if ((ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED))
            && (ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_MM_CLEARED)))
   {
       /* SPR 18929: Set the call state to IDLE as all the interfaces have been
        * cleared and also set the status as CALL_R_SRV terminate so that
        * CM can send trigger to SCM to clear any pending service for that call
        */ 
       ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_00_IDLE)

        /*Return service terminate status to CM*/
        ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
   }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and it receives 
 *                  session param change from SSA (SIP UPDATE/RE-INVITE recd). 
 *                  In this case, send an error response back to SSA. This is 
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in 
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Simply invoke icf_cc_07_ssa_offer_change() that does same handling */
    ret_val = icf_cc_07_ssa_offer_change(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call 
 *                  terminate response from SSA or PA and Release timer expires.
 *                  In this case, clear all interfaces.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_08_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* 
     * At this stage, MM interface has already been cleared since DMS response 
     * would have been received before arriving in this state, hence the MM 
     * cleared bit is already set in call_clear_status.
     */
    
    /* 
     * Also, PA interface can be assumed to be cleared since Call Terminate
     * Request or Response would have been sent to PA on getting DMS response, 
     * hence we directly set the PA cleared bit in call_clear_status just in 
     * case Call Terminate Response has not been received from PA. The Call 
     * Terminate Response received later from PA will be ignored.
     */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_PA_CLEARED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)

    /*
     * So, only network interface needs to be cleared. For this, simply 
     * invoke network clear handler that will take care of clearing the 
     * signaling context. The Call Terminate Response received later from 
     * SSA will be ignored.
     */
    ret_val = icf_cc_nw_clear_handler(p_cc_pdb);
	
	ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_00_IDLE)
    /*Return service terminate status to CM*/
    ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Call 
 *                  Terminate Response from SSA or PA and it receives a generic
 *                  error indication.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_08_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

