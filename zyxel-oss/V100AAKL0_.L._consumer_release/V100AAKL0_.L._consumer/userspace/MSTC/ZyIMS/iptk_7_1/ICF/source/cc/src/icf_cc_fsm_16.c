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
 * 
 * Copyright (c) 2006, Aricent.
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

ICF_CHECK_QOS_SUPPORT_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

	/* This function is checking if there is any mismatch in the 
	 * oreder of streams.
	 */

	p_cc_pdb->p_list_1 = p_inc_media->p_sdp_list;

	ret_val = icf_cc_validate_ith_level_m_line_check(p_cc_pdb);
	
	if (ICF_FAILURE == ret_val)
	{
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
        }

        /* Now we will check if current status exceeds or matches desired
         * status in remote network
         */
        if(ICF_SUCCESS == 
                icf_cc_match_curr_des_status_on_remote_nw(p_cc_pdb))
		{

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

					/* Reset the prack_reqd flag in p_cc_pdb as we do not
					 * have to send a reliable 180 response next*/
					p_cc_pdb->prack_reqd = ICF_FALSE;
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
		}
		else
		{
			/* do nothing, wait for UPDATE in same state */

		}

                /*Would direct SSA to send 580 on NW*/
                /*  p_cc_pdb->offer_failure_reason = 
                    ICF_SA_PRECONDITIONS_FAILURE;
                ret_val = icf_cc_send_ssa_offer_chg_err(p_cc_pdb);
				ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
				ret_val = ICF_SUCCESS; */
	}
ICF_CHECK_QOS_SUPPORT_END    
	/*
    else
    {
        ret_val = ICF_FAILURE;
    }
	*/

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
	icf_cc_01_forceful_call_clear(p_cc_pdb);
ICF_CHECK_QOS_SUPPORT_END


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */    

    return ret_val;    
}
#endif
