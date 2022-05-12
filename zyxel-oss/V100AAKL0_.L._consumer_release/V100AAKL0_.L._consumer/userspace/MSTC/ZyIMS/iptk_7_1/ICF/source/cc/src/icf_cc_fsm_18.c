/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_18.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 28-Feb-2006  Anuradha						 Initial
 * 30-Mar-2006  Anuradha						 Call Modify Additions
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"

ICF_HANDLE_REINVITE_START

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_18_ssa_call_modify_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting the ACK for the response 
 *                  for the REINVITE request of PA and it receives the ACK from SSA
 *                  Current State: ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK
 *                  Current Event: ICF_CC_SSA_REINVITE_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_18_ssa_call_modify_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	icf_cc_handle_reinvite_ack(p_cc_pdb);

	if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_TERM_REQ))
	{
		p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_CALL_ABANDONED;
		ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);     
	}
	else
	{
		ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
	}

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_18_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the Ack for the response 
 *                  for the REINVITE request of PA and it does not receive the ack
 *                  Current State: ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_18_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_18_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the ACK for the response 
 *                  for the REINVITE request of PA and it receives  error
 *                  Current State: ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_18_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ret_val = icf_cc_09_generic_err(p_cc_pdb);
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

	return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_18_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the ACK from SSA 
 *                  for the REINVITE request of SSA and it receives CALL TERM from SSA
 *                  Current State: ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_18_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	ret_val = icf_cc_06_ssa_call_term_req(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_18_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the ACK from SSA 
 *                  for the REINVITE request of SSA and it receives CALL TERM from PA
 *                  Current State: ICF_CC_18_AWT_SSA_CALL_MODIFY_ACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_18_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Set release type as PA initiated */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_PA_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_PA_INIT)
    
    /*
     * Set this event as pending event. It will be handled on receiving 
     *  acknowledgement
     */
    ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 
            ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_TERM_REQ)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

ICF_HANDLE_REINVITE_END

