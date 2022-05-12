/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_fsm_15.c
 *
 * Purpose          : This file contains the event handlers in
 *                    ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP state.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 22-Feb-2006  Anuradha						 Initial
 * 30-Mar-2006  Anuradha						 Call Modify Additions
 * 09-Apr-2006  Sumit Gupta						 Updations for defects found in UT
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs     Structure changes for multiple
 *                                               m-lines
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_ecode.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

ICF_HANDLE_REINVITE_START


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the provisional response 
 *                  for the REINVITE requestand it receives the alert RSP from PA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_PA_ALERT
 *
 ******************************************************************************/
icf_return_t icf_cc_15_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* check the call direction , it should be an incoming call modify request here */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		if(ICF_FAILURE == icf_cc_03_pa_alert(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
		else
		{
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)
		}
	}
	

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

	return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the provisional response 
 *                  for the REINVITE request and it receives the alert from SSA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 ******************************************************************************/
icf_return_t icf_cc_15_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

	/* check the call direction , it should be an outgoing call modify request here */
	if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
		if(ICF_FAILURE == icf_cc_03_ssa_alert(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
        else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ))
        {
            if(ICF_FAILURE == icf_cc_14_pa_call_modify_cancel_req(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
        }
		else
		{
			ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)
		}
	}
	

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the 
 *                  provisional response 
 *                  for the REINVITE request and it receives the alert from SSA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 ******************************************************************************/
icf_return_t icf_cc_15_ssa_alert_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t     ret_val = ICF_SUCCESS;
    icf_nw_inc_alert_media_st *p_sdp = (icf_nw_inc_alert_media_st *)
                p_cc_pdb->p_internal_msg->p_msg_data;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_cc_pdb->p_list_1 = p_sdp->p_sdp_list;

	/* check the call direction , it should be an outgoing call modify request here */
	if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
	{
	    if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check (p_cc_pdb))
	    {
            ret_val = ICF_FAILURE;

            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_NETWORK_ERROR;
            ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
            icf_cc_send_pa_call_modify_resp(p_cc_pdb);

		    if (ICF_CC_STREAM_POS_VALIDATION_FAIL == 
		            (p_cc_pdb->common_bitmask & ICF_CC_STREAM_POS_VALIDATION_FAIL))
		    {
			    ret_val = icf_cc_14_generic_err(p_cc_pdb);
		    }
	    }
		else if(ICF_FAILURE == icf_cc_15_ssa_alert(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
	}
	

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

	return ret_val;
}


ICF_HANDLE_REINVITE_END
