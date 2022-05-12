/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_12.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_12_WAIT_START_PROC_SIG state.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 13-Dec-2004  Rohit Aggarwal  Non Service LLD     Beginning of coding
 * 24-Dec-2004  Rohit Aggarwal                      Compilation fixes
 * 
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 30-Mar-2006	Anuradha Gupta	ICF 5.0				Call Modify additions
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_start_call_proc_sig
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives the same.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_START_CALL_PROC_SIG
 *
 ******************************************************************************/
icf_return_t icf_cc_12_start_call_proc_sig(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    

		/* Send Call Terminate Request to SSA */
		ret_val = icf_cc_send_ssa_call_term_req(p_cc_pdb);

		/* 
		 * IF PA cleared flag is not set the send call term indication or response 
		 * to PA
		 */ 
		if (ICF_SUCCESS == ret_val && (ICF_NULL == 
					ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
						ICF_CALL_CLEAR_STATUS_PA_CLEARED)))
		{
			/* Check whether release is PA initiated or not */
			if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
						ICF_REL_TYPE_PA_INITIATED))
			{
				/* Not PA initiated, send Call Terminate Request to PA */
				ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
			}
			else
			{
				/* PA initiated release, send Call Terminate Response to PA */
				ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
			}
        
			if ((ICF_NULL != ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
						ICF_REL_TYPE_PA_INITIATED)) 
					&& (ICF_SUCCESS == ret_val))
			{
				/* Call terminate response successfully sent to PA */
            
				/* Set PA cleared bit */
				ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
						ICF_CALL_CLEAR_STATUS_PA_CLEARED)
				ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)
			}
            
			if (ICF_SUCCESS == ret_val)
			{
				/* Go to state Awaiting Call Terminate Response */
				ICF_CC_SET_CALL_STATE(p_cc_pdb, 
						ICF_CC_08_AWT_CALL_TERMINATE_RESP)
			}
        
		} /* for return value of send SSA call term request */
    
		if (ICF_SUCCESS == ret_val)
		{
			/* Go to state Awaiting Call Terminate Response */
			ICF_CC_SET_CALL_STATE(p_cc_pdb, 
					ICF_CC_08_AWT_CALL_TERMINATE_RESP)
		}

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives Call
 *                  Terminate Request from SSA (SIP BYE received from peer).
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Send Call Terminate Response to SSA */
    ret_val = icf_cc_send_ssa_call_term_rsp(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Set NW cleared bit */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)
            
        /* Check whether release is PA initiated or not */
        if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
        {
            /* Not PA initiated, send Call Terminate Request to PA */
            ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
        }
        else
        {
            /* PA initiated release, send Call Terminate Response to PA */
            ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
        }

        
        if ((ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
                && (ICF_SUCCESS == ret_val))
        {
            /* Call terminate request successfully sent to PA */
            
            /* Go to state Awaiting Call Terminate Response */
            ICF_CC_SET_CALL_STATE(p_cc_pdb, 
                    ICF_CC_08_AWT_CALL_TERMINATE_RESP)
        }
        else if ((ICF_NULL != ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED)) 
                && (ICF_SUCCESS == ret_val))
        {
            /* Call terminate response successfully sent to PA */
            
            /* Set PA cleared bit and clear call context */
            ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
        }
            
    } /* End of if for return value of send SSA call term resp */
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives Call Terminate 
 *                  Request from PA.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     ret_val = icf_cc_chk_user_busy_n_srvc(p_cc_pdb);
    
    /* Set PA initiated bit in release type */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives Call Terminate 
 *                  Response from PA.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set PA cleared bit in call_clear_status */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, ICF_CALL_CLEAR_STATUS_PA_CLEARED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives a session 
 *                  param change from SSA (SIP UPDATE/RE-INVITE received from 
 *                  peer).
 *                  In this case, send an error response back to SSA. This is
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_12_ssa_offer_change(
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
 * FUNCTION:        icf_cc_12_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and Release timer expires.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_12_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /**** MM interface ****/
    /* 
     * Here, MM cleared bit is already set since this state has been reached 
     * after receiving DMS response
     */

    /**** NW interface ****/
    /* Send Call Terminate Request to SSA */
    icf_cc_send_ssa_call_term_req(p_cc_pdb);
    icf_cc_send_ssa_force_clear(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Set NW cleared bit */
        ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb, 
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)

        if(ICF_NULL == (ICF_CALL_CLEAR_STATUS_PA_CLEARED &
                        p_cc_pdb->p_call_ctx->call_clear_status))
        {
              /**** PA interface ****/
              /* Check whether release is PA initiated or not */
              if (ICF_NULL == ICF_CC_CHECK_RELEASE_TYPE(p_cc_pdb, 
                    ICF_REL_TYPE_PA_INITIATED))
              {
                    /* Not PA initiated, send Call Terminate Request to PA */
                    ret_val = icf_cc_send_pa_call_term_ind(p_cc_pdb);
               }
              else
              {
                    /* PA initiated release, send Call Terminate Response to PA */
                    ret_val = icf_cc_send_pa_call_term_resp(p_cc_pdb);
               }

        }
        /* 
         * Since the Release timer has expired and all interfaces cleared, 
         * do the call context clean up without waiting for any responses
         */
        if (ICF_SUCCESS == ret_val)
        {
            /* Set PA cleared bit and clear call context */
            ret_val = icf_cc_pa_clear_handler(p_cc_pdb);
        }
        
    } /* End of if for return value of send SSA call term request */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives a generic 
 *                  error indication.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_12_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    if(ICF_NULL != p_cc_pdb->p_call_ctx->release_type)
    {
       /* Send call clear request to the SSA */
       if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
       {
          ret_val = ICF_FAILURE;
       }/* if(ICF_FAILURE == icf_cc... */
       else
       {
          /* Update the next state in the call context */
          ICF_CC_SET_CALL_STATE(p_cc_pdb,
                  ICF_CC_08_AWT_CALL_TERMINATE_RESP)
       }/* else */
    }
    else
    {
       ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
       ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    }   

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of 
 *                  Signaling Transaction event and it receives a session 
 *                  param change from PA 
 *
 *                  In this case, mark the event as pending and process it when 
 *					ongoing signalling is over..
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

       
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_CALL_MODIFY_REQ_PENDING;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

    /* Send failure response for update to PA */
    if(ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
    {

         /* This is error at interface level, call will be cleared
          * forcefully*/
         ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}
ICF_HANDLE_REINVITE_END
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives a ACK for connect
 ******************************************************************************/

 icf_return_t icf_cc_12_ssa_connect_ack
 (
    INOUT icf_cc_pdb_st *p_cc_pdb
 )
 {
     icf_return_t         ret_val = ICF_SUCCESS;

     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     if(ICF_NULL != p_cc_pdb->p_call_ctx->release_type)
     {
         /* Send call clear request to the SSA */
         if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }/* if(ICF_FAILURE == icf_cc... */
         else
         {
             /* Update the next state in the call context */
             ICF_CC_SET_CALL_STATE(p_cc_pdb,
                      ICF_CC_08_AWT_CALL_TERMINATE_RESP)
         }/* else */
     }
     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
     return ret_val;
 }

