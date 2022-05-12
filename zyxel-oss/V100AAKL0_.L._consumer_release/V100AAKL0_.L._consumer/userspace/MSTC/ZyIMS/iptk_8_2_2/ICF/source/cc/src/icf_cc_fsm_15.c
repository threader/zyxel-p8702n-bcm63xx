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
 * 03-Jul-2009  Anuradha Gupta   SPR 19754       Modified  icf_cc_15_pa_alert        
 *
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
    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;
    icf_pa_cc_user_alerted_req_st	*p_user_alerted = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_user_alerted = ((icf_pa_cc_user_alerted_req_st *)
                p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

	/* check the call direction , it should be an incoming call modify request here */
	if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_DIR))
    {
        /* SPR 19754 : In case reinvite is received with 100rel in Require
         * header, application is communicated the same and sending provisional 
         * response is not expecetd from application. In case application does so
         * then 1) Reject the reinvite with 420
         *      2) Send Call Modify Cancel Indication to application
         *      3) Revert the media
         * In case PRACK value is specified in the user alert payload and it is
         * ICF_PRACK_IN_REQUIRE then reject the reinvite with 500 and do the similar handling
         * as specified above pointwise
         */      
        if((ICF_REL_PROV_RESP_REQUIRED_BY_PEER == 
                (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                         ICF_REL_PROV_RESP_REQUIRED_BY_PEER))) ||
                ((ICF_LOCAL_USER_ALERTED_PRACK_VALUE_PRESENT & 
                        p_user_alerted->bit_mask) && 
                 (ICF_PRACK_IN_REQUIRE == p_user_alerted ->prack_value)))
        {
            /* in case 100rel in require header is present then the reject
             * response code should be 420 and in case Prack value is specified
             * in the User Alert Request then the reject response code should
             * be 500
             */ 
            if(ICF_REL_PROV_RESP_REQUIRED_BY_PEER == 
                (ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,
                         ICF_REL_PROV_RESP_REQUIRED_BY_PEER)))
            {
               p_cc_pdb->p_call_ctx->response_code = 420;
            }
            else
            {
               p_cc_pdb->p_call_ctx->response_code = 500;
            }

            if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else if(ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_ind(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else if( ICF_FALSE == p_cc_pdb->p_call_ctx->media_updated)
            {
                /*
                 * Update the states in the call context
                 */
                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
            }
            else
            {
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
                /* revert the changes and then send cancel indication to PA */
                if(ICF_FAILURE == icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb))
                {
                     ret_val = icf_cc_14_generic_err(p_cc_pdb);
                }
                else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
                {
                      ret_val = ICF_FAILURE;
                }
                else
                {
                     if(ICF_TRUE == p_cc_pdb->mms_sent)
                     {
                         /*
                          * Update the states in the call context
                          */
                          ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                          ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                          ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                        ICF_CC_SSA_CALL_MODIFY_CANCEL_REQ)

                          /* Start the media timer  */
                          ret_val = icf_cc_start_media_timer(p_cc_pdb);
                    }
                }/* MMS was handled succesfully */
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
                ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, 
                          p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                          ret_val)
                if(ICF_FAILURE == ret_val)
                {
                     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                     return ret_val;
                }
                p_stream_sdp_info = (icf_stream_sdp_info_st *)p_cc_pdb->p_call_ctx
                                       ->p_stream_sdp_info->p_payload;
                 /* copy the last committed to local sdp as
                 * call modify cancel has been received
                 */
                if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb,
                            p_stream_sdp_info->p_last_commited_sdp,
                            &(p_stream_sdp_info->p_local_sdp)))
                {
                    ret_val = ICF_FAILURE;
                }
                /*  Its handlinh is done in tunnel mode 
                 *  because MMS is not done so
                 *  there is no handling that is pending 
                 *  that is why resetting this bitmask
                 */
                ICF_CC_RESET_COMMON_BITMASK_2(p_cc_pdb,
                        ICF_INCOMING_CALL_MOD_CANCEL_REQ)
ICF_CHECK_TUNNEL_MODE_END
            } /* end of else */

            /* reset the prov resp for call modify flag */
            p_cc_pdb->p_call_ctx->prov_resp_status &= (~(ICF_CALL_MODIFY_PROV_RESP_RECVD));

        }
        else if(ICF_FAILURE == icf_cc_03_pa_alert(p_cc_pdb))
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
