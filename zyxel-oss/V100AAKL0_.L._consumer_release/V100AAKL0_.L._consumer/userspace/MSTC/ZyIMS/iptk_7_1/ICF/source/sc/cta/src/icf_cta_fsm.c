/*******************************************************************************
*
* FILE NAME    : icf_cta_fsm.c
*
* DESCRIPTION  : Contains the definitions of all State Machine functions
*                of CTA service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 19-Feb-2005    Amit Sharma     None            Initial
* 08-Mar-2005    Amit Sharma     None            Incorporated review comments
* 05-Apr-2005    Amit Sharma     None            Updated handling for PA call
*                                                termination
* 06-May-2005    Amit Sharma     SPR:7514        Updation in return path payload
*                                                call context return
* 06-May-2005    Amit Sharma     SPR:7502        Failure response to PA when
*                                                CTA req in call setup phase
* 10-May-2005    Amit Sharma     SPR:7512        PA call termination is 
*                                                buffered even if REFER has not
*                                                been sent to network yet
* 10-May-2005    Amit Sharma     SPR:7512        PA 01 and 02 state function
*                                                removed
* 12-May-2005    Amit Sharma     SPR:7550        CTA failure response to PA
*                                                when call release is received
*                                                also abort transaction is sent
*                                                to SSA in this case
* 12-May-2005    Amit Sharma     SPR:7550        If state is refer_resp then
*                                                only abort is sent
* 19-May-2005    Amit Sharma     SPR:7444        STATS updated
* 27-May-2005    Amit Sharma     SPR:7568        Updation for forceful_call_clear
* 31-May-2005    Amit Sharma     SPR:7627        Reset signal ongoing when 
*                                                timer expiry recvd
* 02-Jun-2005    Amit Sharma     SPR:7535        Wait for TWC service term
* 23-Jun-2005    Amit Sharma                     Remove call_clear_reason flag
* 08-Nov-2005    Amit Sharma   SPR:8129
* 24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
* 08-Jul-2009    Anurag Khare    CSR 1-7489962   Modified function 
*                                                icf_cta_03_refer_resp
* 15-Jul-2009    Anurag Khare    SPR 19856       Modified function 
*                                                icf_cta_xx_cc_call_released
*
* Copyright 2006,Aricent .
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cta_types.h"
#include "icf_cta_prototypes.h"
#include "icf_cta_macro.h"
#include "icf_cta_defs.h"
#include "icf_common_interface_types.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_00_call_transfer_attended_request
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_00_IDLE
*                 Event         ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_cta_00_call_transfer_attended_request(
        INOUT  icf_cta_pdb_st    *p_pdb)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    
    icf_pa_cc_call_transfer_attended_req_st  *p_cta_req = ICF_NULL;

    /*Copy p_glb_pdb from internal msg to local ptr*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    /* Changes for XCONF */
    if(ICF_TRUE == p_pdb->p_cta_ctx->internal_refer_req)
    {
	    p_pdb->p_cta_ctx->transferred_id = ((icf_internal_refer_req_st*)
           (p_pdb->p_internal_msg->p_msg_data))->replaces_call_id;
    }
    /*  Changes for XCONF End */
    else
    {
        /*Copy incoming CTA request pointer*/
        p_cta_req = (icf_pa_cc_call_transfer_attended_req_st *)
                    &(p_glb_pdb->p_recv_msg->payload[0]);

        p_pdb->p_cta_ctx->transferred_id = p_cta_req->transferred_party_id;
    }

    if (ICF_FAILURE == icf_cta_clone_app_spec_hdr_lists(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    if (ICF_FAILURE == icf_cta_set_both_call_ctx(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if (ICF_CTA_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        ret_val = ICF_SUCCESS;
    }


    /* If release has been started any of two calls then service can
     * not continue and returned service ended normal*/
     else if ((ICF_REL_TYPE_INVALID != 
                p_pdb->p_cta_ctx->p_call_ctx_b->release_type)
            || (ICF_REL_TYPE_INVALID != 
                p_pdb->p_cta_ctx->p_call_ctx_c->release_type)
            || (ICF_FAILURE == icf_cta_apply_early_cta_check(p_pdb)))

    {
        /*Send failure CTA response to PA*/
        ret_val = icf_cta_send_cta_resp(p_pdb,
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_INVALID_SERVICE_ACTION));

        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        /*Service ended normal for both the calls*/
        ret_val = icf_cta_apply_both_calls(p_pdb);
    }
    else if (ICF_FAILURE == icf_cta_start_cta_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*Check if CHR service is active*/
    else
    {
        /*Check whether TWC service is active*/
        ret_val = icf_cta_check_twc_service(p_pdb);
    }

    if(ICF_FAILURE != ret_val)
    {
       ICF_CTA_APP_TRACE(p_pdb, ICF_TRACE_CTA_REQ)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* INOUT  icf_cta_pdb_st    *p... */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_call_hold_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CALL_HOLD_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_call_hold_resp(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_CALL_HOLD_RESP_RECVD)
        
    ret_val = icf_cta_check_chr_service(p_pdb);
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_cta_01_call_hold_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_call_resume_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CALL_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_call_resume_resp(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_CALL_RESUME_RESP_RECVD)

    ret_val = icf_cta_check_chr_service(p_pdb);
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_cta_01_call_resume_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_02_start_proc_sig
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTA_START_PROC_SIG
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_02_start_proc_sig(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Check if signaling is ongoing for any other call*/
    ret_val = icf_cta_check_sig_ong_n_send_refer(p_pdb);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_02_start_proc_sig */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_03_refer_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTA_REFER_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_03_refer_resp(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_nw_inc_refer_resp_st  *p_refer_resp = ICF_NULL;
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_internal_msg->p_msg_data))
    
    /*Assign REFER response structure*/
    p_refer_resp =
    (icf_nw_inc_refer_resp_st *)p_pdb->p_internal_msg->p_msg_data;
    
    /*Set return status that signaling is completed*/
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    /*If failure REFER response received*/
    if (ICF_FAILURE == p_refer_resp->status)
    {
        /*Check if PA has already initiated call clear*/
        /*  Fix for CSR 1-7489962*/
        /*  The release cause shall be set only when PA
         *  has already initiated the call clear.
         */
        if (ICF_TRUE == p_pdb->p_cta_ctx->call_rel_initiated)
        {
            icf_cta_process_call_rel_flags(p_pdb);
            ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        }
        else
        {
            ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }

        if (ICF_SUCCESS == ret_val)
        {
            /*Send failure CTA response to PA*/
            ret_val = icf_cta_send_cta_resp(p_pdb,
                    (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ));
        }
    } /* if(ICF_FAILURE == p_refer_r... */
    else
    {
        ICF_CTA_SET_CURR_STATE(p_pdb, ICF_CTA_04_WAIT_FOR_NOTIFY)
    }

    /*Apply return status on both the calls*/
    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cta_apply_both_calls(p_pdb);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_03_refer_resp */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_04_notify
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_NOTIFY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_04_notify(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st  *p_notify = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_STAT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
             ICF_STAT_NW_CC_NOTIFY)

    /*Assign NOTIFY structure*/
    p_notify = (icf_nw_inc_notify_ind_st *)p_pdb->p_internal_msg->p_msg_data;
  
    /*Send final response for NOTIFY*/
    
    if (ICF_FAILURE == icf_cta_send_notify_resp(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    /*Apply call clear for both the calls*/
    /*Stop CTA timer*/
    if (ICF_FAILURE == icf_cta_stop_cta_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*If notify is for successful call transfer*/
    else if (ICF_SUCCESS == p_notify->status)
    {
        /*Send failure CTA response to PA*/
        ret_val = icf_cta_send_cta_resp(p_pdb,
                (icf_boolean_t)(ICF_SUCCESS),
                (icf_error_t)(ICF_CAUSE_UNDEFINED));

	    /* Changes For XCONF */
	    if(ICF_TRUE == p_pdb->p_cta_ctx->internal_refer_req)
        {   
           ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }      
	    else
        {    
	       /* Changes For XCONF End */
           /*Terminate service and clear both the calls*/
           /* In case of xconf we donot want to release the main call
              therefore this function is moved to else part*/
           icf_cta_process_call_rel_flags(p_pdb);
           ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        }   
    }
    else
    {
        /*Failure response to PA that new call could not be established*/
        /*Send failure CTA response to PA*/
        ret_val = icf_cta_send_cta_resp(p_pdb,
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ));

	/*Terminate service */
        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    }

    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cta_apply_both_calls(p_pdb);
    } /* if (ICF_SUCCESS == ret_val) */    

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_cta_04_notify */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_pa_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_pa_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st                   *p_glb_pdb = ICF_NULL;
    icf_pa_cc_terminate_call_req_st  *p_call_terminate = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_PA_INITIATED_CALL_REL)

    /*Buffer it in CTA Context*/
    p_pdb->p_cta_ctx->call_rel_initiated = ICF_TRUE;

    /*Copy call terminate reason*/
    p_call_terminate = (icf_pa_cc_terminate_call_req_st *)
        &(p_glb_pdb->p_recv_msg->payload[0]);
    
    /*Fill call clear reason received from PA*/
    p_pdb->p_cta_ctx->call_clear_reason = 
        p_call_terminate->call_clear_reason;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_xx_pa_call_released */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_CC_INITIATED_CALL_REL)
  
    /*If call is cleared by remote party before REFER is sent
     * terminate service and other call will remain unaffected*/
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
    {
        /*Send failure CTA response to PA*/
        ret_val = icf_cta_send_cta_resp(p_pdb,
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    }
    else
    {
        /*Send failure CTA response to PA*/
        ret_val = icf_cta_send_cta_resp(p_pdb,
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
    }

    if (ICF_SUCCESS == ret_val)
    {
        /*Set return status as both calls and fill call context pointer
         * of other call in payload*/
        ret_val = icf_cta_apply_both_calls(p_pdb);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_01_cc_call_released */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_02_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_02_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
 
    ret_val = icf_cta_01_cc_call_released(p_pdb);
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_02_cc_call_released */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_CC_INITIATED_CALL_REL)

    /*Buffer it in CTA Context*/
    if (p_glb_pdb->p_call_ctx->call_id == 
            p_pdb->p_cta_ctx->p_call_ctx_b->call_id)
    {
        p_pdb->p_cta_ctx->call_clear_initiator_b = ICF_TRUE;
    } /* if (p_glb_pdb->p_call_ctx->cal... */
    else
    {
        /*  Fix for CSR 1-7506519 : SPR 19856 */
        /*If call with trasfer target is already released then return service_ended_error
         *so that CM can initiate generic_error for both the calls, otherwise return
         *service_ended_normal so that SCM can release both service contexts but
         *CM do not clear the other call*/
        if (ICF_TRUE == p_pdb->p_cta_ctx->call_clear_initiator_b)
        {
            ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        }
        else
        {
        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }
        
        if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
        { 
            /*Send failure CTA response to PA*/
            ret_val = icf_cta_send_cta_resp(p_pdb,
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
        }
        else
        {
            /*Send failure CTA response to PA*/
            ret_val = icf_cta_send_cta_resp(p_pdb,
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        }

        if (ICF_SUCCESS == ret_val)
        {
            if (ICF_CTA_03_WAIT_FOR_REFER_RESP == 
                    p_pdb->p_cta_ctx->state)
            {
                ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

                ret_val = icf_cta_abort_refer_trans(p_pdb);
            } /* p_pdb->p_ctu_ctx->state) */
            
            /*Send message to SSA to clear the REFER transaction*/
            if (ICF_SUCCESS == ret_val)
            {
                /*Set return status as both calls and fill call context pointer
                 *of other call in payload*/
                ret_val = icf_cta_apply_both_calls(p_pdb);
            } /* if (ICF_SUCCESS == icf_c... */
        }
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_xx_cc_call_released */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_timer_expiry(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

	p_glb_pdb =  p_glb_pdb;
	
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_STAT(p_glb_pdb, ICF_STAT_CTA_TMR_EXPIRY)
   
    /*Send failure CTA response to PA*/
    ret_val = icf_cta_send_cta_resp(p_pdb,
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));

    icf_cta_process_call_rel_flags(p_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /*Fill return status as both calls and fill call context pointer of 
         * other call in return payload*/
        ret_val = icf_cta_apply_both_calls(p_pdb);
    }
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_cta_xx_timer_expiry */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_03_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTA_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_03_timer_expiry(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_STAT(p_glb_pdb, ICF_STAT_CTA_TMR_EXPIRY)
    
    /*Send service ended error for both the calls*/
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    icf_cta_process_call_rel_flags(p_pdb);
        
    /*Send failure CTA response to PA*/
    ret_val = icf_cta_send_cta_resp(p_pdb,
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));
    
    if(ICF_SUCCESS == ret_val)
    {
        /*Send message to SSA to clear the REFER transaction*/
        ret_val = icf_cta_abort_refer_trans(p_pdb);
    }

    if(ICF_SUCCESS == ret_val)
    {
        /*Fill return status as both calls and fill call context pointer 
         * * of other call in return payload*/
        ret_val = icf_cta_apply_both_calls(p_pdb);
    } /* if(ICF_SUCCESS == ret_val) */    
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_03_timer_expiry */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear 
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_xx_forceful_call_clear(
        INOUT  icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Set return status as both calls and fill call context pointer
     *of other call in payload*/
    if (p_pdb->p_cta_ctx->p_call_ctx_b->call_id
            == p_glb_pdb->p_call_ctx->call_id)
    {
        ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_c);
    }
    else
    {
        ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_b);
    }
    
    /*Send service ended error for both the calls*/
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_xx_forceful_call_clear */


/******************************************************************************
*
* FUNCTION NAME : icf_cta_03_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear 
*                 indication in ICF_CTA_03_WAIT_FOR_REFER_RESP state
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_03_forceful_call_clear(
        INOUT  icf_cta_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Send message to SSA to clear the REFER transaction*/
    icf_cta_abort_refer_trans(p_pdb);

    /*Set return status as both calls and fill call context pointer
     *of other call in payload*/
    if (p_pdb->p_cta_ctx->p_call_ctx_b->call_id
            == p_glb_pdb->p_call_ctx->call_id)
    {
        ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_c);
    }
    else
    {
        ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_b);
    }
    
    /*Send service ended error for both the calls*/
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_03_forceful_call_clear */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_unexpected_event(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
   
    /*To remove warning*/
    p_pdb = p_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_cta_unexpected_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_05_twc_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_05_WAIT_FOR_TWC_RESP
*                 Event         ICF_CTA_TWC_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_05_twc_resp(
        INOUT   icf_cta_pdb_st   *p_pdb)
{
    icf_return_t          ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Check whether CHR service is active*/
    ret_val = icf_cta_check_chr_service(p_pdb);

    return ret_val;
} /* icf_cta_05_twc_resp */
#endif
