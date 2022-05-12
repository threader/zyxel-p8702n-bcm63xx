/*******************************************************************************
*
* FILE NAME    : icf_chr_sm.c
*
* DESCRIPTION  : Contains the definitions of all State Machine functions 
*                of CHR service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 8-Dec-2004    Amit Sharma     None            Initial
* 25-Jan-2005  Amit Sharma                      Changes for call_status field
*                                               in call context
* 25-Feb-2005    Amit Sharma                    Added handling for forceful
*                                               Call Clear event
* 07-Apr-2005    Amit Sharma                    Update handling for cancel
* 10-Apr-2005    Amit Sharma                    Fix for SPR:7376
* 20-Apr-2005    Amit Sharma   SPR:7376         Updation for code review
*                                               comments for SPR:7376
* 20-Apr-2005    Amit Sharma   SPR:7388         Call release when timer expire
* 20-Apr-2005    Amit Sharma   SPR:7400         CHR service release when call
*                                               release is initiated
* 20-Apr-2005    Amit Sharma   SPR:7401         Handling of media_error after
*                                               remote failure response
*                                               Updation to start reinvite
*                                               race timer
* 04-May-2005    Amit Sharma   SPR:7495         Other call active on same line
*                                               and TWC service is active then
*                                               resume other call
* 15-May-2005    Amit Sharma   SPR:7444         STATS updation
* 15-May-2005    Amit Sharma   SPR:7461         Handling for ecode generic
*                                               error from SSA/MMI
* 15-May-2005  Amit Sharma   SPR:7560          chr_timer_id is assigned NULL
*                                               after it is stopped
* 28-May-2005  Amit Sharma   SPR:7627          returned sig ong ended
* 28-May-2005  Amit Sharma                     calcel req in 02_timer_expiry
*
* 29-Sep-2005   Shefali Aggarwal                Code changes for rel 2.0
* 4-Oct-2005    Shefali Aggarwal                Changes done at the time of UT-
*                                               Function - 
*                                               icf_chr_00_call_hold, 
*                                               icf_chr_00_call_resume,
*                                               icf_chr_02_nw_hldrsm_suc_resp
* 25-Oct-2005  Amit Sharma   SPR:8149           failure response sent to PA
* 25-Oct-2005  Amit Sharma   SPR:8150           Success responses sent to PA
*                                               for call hold and resume
* 25-Oct-2005  Amit Sharma   SPR:8152           failure responses sent to PA
*                                               for resume and serice cont.
* 21-Nov-2005  Amit Sharma   SPR:8306           
* 07-Sep-2006  Amit Sharma   SPR:12840   
* 11-Jul-2007  Neha Choraria ICF 7.0            icf_chr_generate_timer changed
*                                               to icf_cmn_generate_timer for 
*                                               changes in retry-after
* 24-Aug-2007  Amit Sharma                      IPTK merge from PQT6.5
* 03-Jul-2008  Anurag Khare  SPR 18684          Compilation issue on gcc 4.2.3
* Copyright 2008,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"
#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_chr_types.h"
#include "icf_chr_macro.h"
#include "icf_chr_prototypes.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_internal_api_id.h"


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_00_call_hold
*
* DESCRIPTION  : This function is called when call hold request receives in 
*                idle state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_HOLD
*                
*
* RETURNS: icf_return_t 
*
*******************************************************************************/
icf_return_t icf_chr_00_call_hold(
        INOUT  icf_chr_pdb_st    *p_pdb)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_return_t        ret_val;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_call_hold_resume_int_payload_st  *p_chr_payload = ICF_NULL;
	icf_app_conf_st                *p_app_conf = ICF_NULL;
   
    ret_val = ICF_SUCCESS;

    /*Copy p_glb_pdb from internal msg to local ptr*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_RECVD)
    
    ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_CALL_HOLD)

    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_HOLD_REQ)

    ret_val = icf_cfg_fetch_app_conf(
                    p_glb_pdb,
					p_glb_pdb->p_call_ctx->app_id,
                    &p_app_conf,
                    p_ecode);

    if (ICF_NULL != p_pdb->p_internal_msg->p_msg_data)
    {
        p_chr_payload = (icf_call_hold_resume_int_payload_st *)
            (p_pdb->p_internal_msg->p_msg_data);
    }

    if ((ICF_NULL != p_glb_pdb->p_recv_msg) && 
        (ICF_CALL_HOLD_REQ == p_glb_pdb->p_recv_msg->hdr.api_id))
    {
        if (ICF_FAILURE == icf_chr_clone_app_spec_hdr_lists(p_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }
    
    /*If call is not connected yet send failure response to PA*/
    if (!(ICF_CALL_CONNECT_DONE & p_pdb->p_call_ctx->common_bitmask) ||
        (ICF_FALSE == p_pdb->p_call_ctx->media_committed))
    {
        /*Send call hold failure to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_CALL_ID);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    }
    /*Check if the call is in releasing state, send failure hold response
     * and terminate service*/
    else if (ICF_REL_TYPE_INVALID != p_pdb->p_call_ctx->release_type)
    {
        /*Send call hold failure to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    } /* else if (ICF_REL_CAUSE_INVA... */    
	/*If call_modify_reqd flag is false received from application
	 * then send call hold failure response to application*/
	else if (ICF_TRUE == p_app_conf->app_profile.call_modify_support_reqd)
    {
        /*Send call hold failure to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    } /* else if (ICF_REL_CAUSE_INVA... */    
    /*If call is already local held*/
    else if ((ICF_CALL_STATUS_LOCAL_HELD & p_pdb->p_call_ctx->call_status)
            && (ICF_CHR_PE_CALL_RESUME != ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*TRACE*/
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_ALREADY_HOLD)

        /*Send cael hold failure to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    } /* if(ICF_LOCAL_HOLD == p_pdb->p_call_ctx->call_status) */
   
    /*If call modify request is pending i.e. call hold request is 
      recieved when application is already initiated call modify request
      and IPTK has sent ReInvite to remote which is responded by 491.*/
    else if (ICF_NULL != 
                (ICF_CC_491_RESP_PROCESS_ONGOING & 
                 p_pdb->p_call_ctx->common_bitmask_2))
    {
        /*Send call hold failure to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_CALL_MODIFY_REQ_PENDING);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    } /* if 491 0f reInvite call_modify request is pending event */ 

    /* Check if the payload contains buffered event */
    else if ((ICF_NULL != p_chr_payload) && 
            (ICF_TRUE == p_chr_payload->buffer_event))
    {
        /* If the PE is call resume then reset it */
        if (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, 
                (icf_error_t)(ICF_CAUSE_UNDEFINED));

            if (ICF_SUCCESS == ret_val)
            {
                ret_val = icf_chr_send_hold_resp(p_pdb, 
                        (icf_boolean_t)ICF_SUCCESS, 
                        (icf_error_t)(ICF_CAUSE_UNDEFINED));
                
                ICF_CHR_RESET_PEND_EVT(p_pdb)
            
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
            } /* if (ICF_SUCCESS == ret_val) */
        }
        else if (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);
        }
        /* In all other cases PE will be overwritten */
        else
        {
            ICF_CHR_SET_PEND_EVT(p_pdb,ICF_CHR_PE_CALL_HOLD)   
        }
        
        /* Set the status to service continue and return */
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        /* Reset Ongoing action */
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    }

    /*Start CHR timer*/
    else if(ICF_FAILURE == 
            icf_chr_start_timer(p_pdb, 
                (icf_uint8_t)ICF_CHR_HOLD_RESUME_TIMER))
    {
        ret_val = ICF_FAILURE;
    } /* if(ICF_FAILURE != ret_val) */
    /*If call is remote held or muted*/
    else if (ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
    { /*Remote hold*/
	p_pdb->p_call_ctx->media_state |= ICF_MEDIA_STATE_LOCAL_HELD;

        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_REMOTE_HOLD)
  
        icf_chr_set_app_media_mode(p_pdb, ICF_MEDIA_MODE_INACTIVE); 
         /*If Signaling ongoing in some other module*/
        if(ICF_INT_MODULE_INVALID != ICF_CHR_GET_SIG_ONGOING(p_pdb))
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SIG_ONGOING)

            ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)

            ICF_CHR_SET_CURR_STATE(
                    p_pdb, ICF_CHR_04_AWT_ST_PROC_SIG)
        } /* if(ICF_INT_MODULE_INVALID !=... */        
        /*Send Reinvite to NW*/        
        else if(ICF_FAILURE == icf_chr_ssa_send_reinvite_msg(p_pdb,
                    (icf_boolean_t)ICF_TRUE))
        { 
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SSA_FAIL_RESP)

            /*Stop CHR timer*/
            ICF_STOP_TIMER(p_glb_pdb, 
                    p_pdb->p_chr_ctx->chr_timer_id, p_ecode, ret_val)

            p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

            ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)
            
            ICF_CHR_RESET_ONGOING_ACT(p_pdb)
            
            ret_val = ICF_FAILURE;
        } /* else if(ICF_FAILURE == ipps... */
        /*If function returns success*/
        else
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SSA_SUCC_RESP)
                
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
        }  /* else */        
    }/*Remote Hold */
    /*If media ongoing in some other module*/
    else if(ICF_INT_MODULE_INVALID != ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_MED_ONGOING)
       
        ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        ICF_CHR_SET_CURR_STATE(p_pdb, 
                ICF_CHR_05_AWT_ST_PROC_MEDIA)
    } /* else if(ICF_INT_MODULE_INVALID... */
    /*Send MMS*/
    else if(ICF_FAILURE == icf_chr_mmi_send_hold_msg(p_pdb))
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_MMI_FAIL_RESP)

        /*Stop CHR timer*/
        ICF_STOP_TIMER(p_glb_pdb, 
                p_pdb->p_chr_ctx->chr_timer_id, p_ecode, ret_val)

        p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

        ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ret_val = ICF_FAILURE;
    } /* else if(ICF_FAILURE == ipps... */      
    /*mmi send return success*/
    else
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_MMI_SUCC_RESP)

        /*Go to awt mms resp state*/
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
    } /* else */    

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_glb_pdb->load_data.call_hold_attempts++;
    }
#endif

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}/* icf_chr_00_call_hold */

/*******************************************************************************
 *
 * FUNCTION NAME    : icf_chr_00_call_resume
 *
 * DESCRIPTION  : This function is called when call resume request receives in 
*                idle state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_RESUME
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_00_call_resume(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_error_t          *p_ecode = ICF_NULL;
    icf_boolean_t        other_call_active;
    icf_call_hold_resume_int_payload_st  *p_chr_payload = ICF_NULL;
    icf_app_conf_st                *p_app_conf = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
        
    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_RESUME_RECVD)

    ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_CALL_RESUME)

    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_RESUME_REQ)

	ret_val = icf_cfg_fetch_app_conf(
                    p_glb_pdb,
					p_glb_pdb->p_call_ctx->app_id,
                    &p_app_conf,
                    p_ecode);

    if (ICF_NULL != p_pdb->p_internal_msg->p_msg_data)
    {
        p_chr_payload = (icf_call_hold_resume_int_payload_st *)
            (p_pdb->p_internal_msg->p_msg_data);
    }
   
    if ((ICF_NULL != p_glb_pdb->p_recv_msg) && 
        (ICF_CALL_RESUME_REQ == p_glb_pdb->p_recv_msg->hdr.api_id))
    {
        if (ICF_FAILURE == icf_chr_clone_app_spec_hdr_lists(p_pdb))
        {
            ret_val = ICF_FAILURE;
        }
    }

    /*Check if the call is in releasing state, send failure hold response
     * and terminate service*/
    if (ICF_REL_TYPE_INVALID != p_pdb->p_call_ctx->release_type)
    {
        /*Send call hold failure to PA*/
        ret_val = icf_chr_send_resume_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    }
    /*If call_modify_reqd flag is false received from application
	 * then send call resume failure response to application*/
	else if (ICF_TRUE == p_app_conf->app_profile.call_modify_support_reqd)
    {
        /*Send call resume failure to PA*/
        ret_val = icf_chr_send_resume_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        if(ICF_FAILURE != ret_val)
        {
            /*Set return status as service ended normal*/
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* if(ICF_FAILURE != ret_val) */

        /*Reset ongoing action invalid*/
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    } /* else if (ICF_REL_CAUSE_INVA... */   
    /* Check if the payload has buffered event set to true */
    else if ((ICF_NULL != p_chr_payload) && 
            (ICF_TRUE == p_chr_payload->buffer_event))
    {
        /* If the PE is call hold then reset it */
        if (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, 
                (icf_error_t)ICF_CAUSE_UNDEFINED);

            if (ICF_SUCCESS == ret_val)
            {
                ret_val = icf_chr_send_resume_resp(p_pdb, 
                        (icf_boolean_t)ICF_SUCCESS, 
                        (icf_error_t)(ICF_CAUSE_UNDEFINED));
                
                ICF_CHR_RESET_PEND_EVT(p_pdb)
            
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
            } /* if (ICF_SUCCESS == ret_val) */            
        }
        
        else if (ICF_CHR_PE_INVALID == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
            if (!(ICF_CALL_STATUS_LOCAL_HELD & 
                        p_pdb->p_call_ctx->call_status))
            {
                ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_ALRADY_ACTIVE)

                /*Send call resume failure to PA*/
                if(ICF_FAILURE != icf_chr_send_resume_resp(p_pdb, 
                            (icf_boolean_t)ICF_FAILURE, 
                            (icf_error_t)ICF_CAUSE_CALL_ALREADY_ACTIVE))
                {
                    ICF_CHR_SET_STATUS(p_pdb, 
                            ICF_STATUS_SERVICE_ENDED_NORMAL)
                }
        
                ICF_CHR_RESET_ONGOING_ACT(p_pdb)

                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)       
            }

            else
            {
                ICF_CHR_SET_PEND_EVT(p_pdb,ICF_CHR_PE_CALL_RESUME)      
            }
        }

        else if (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb))
        {
            /*Send call resume failure to PA*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                        (icf_boolean_t)ICF_FAILURE, 
                        (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);
        
            ICF_CHR_RESET_ONGOING_ACT(p_pdb)
                
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)       
        }

        else
        {
            ICF_CHR_SET_PEND_EVT(p_pdb,ICF_CHR_PE_CALL_RESUME)
        }
        
        /* Set the status to service continue and return */
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        /* Reset Ongoing action */
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
    }

     /*If call is not already local held*/
    /*if Application mode is SEND_RECV or RECV only then it means that 
      call is already resumed on network and MM so just send resume 
      success response
    */
    else if((ICF_CALL_STATUS_LOCAL_HELD & p_pdb->p_call_ctx->call_status) &&
            ((ICF_SUCCESS == 
             icf_chr_is_given_audio_mode(p_pdb, ICF_MEDIA_MODE_SEND_RECV))||
            (ICF_SUCCESS ==
             icf_chr_is_given_audio_mode(p_pdb, ICF_MEDIA_MODE_RECV_ONLY))))
/*            !(ICF_MEDIA_STATE_LOCAL_HELD & p_pdb->p_call_ctx->media_state))*/
    {
        p_pdb->p_call_ctx->call_status &= ~ICF_CALL_STATUS_LOCAL_HELD;
        
        /*Send call resume failure to PA*/
        if(ICF_FAILURE != 
                icf_chr_send_resume_resp(p_pdb,
                    (icf_boolean_t)ICF_SUCCESS, 
                (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* else */
        
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

    } /* ICF_FAILURE == icf_chr_c... */   

    /*If call is not already local held*/
    else if(!(ICF_CALL_STATUS_LOCAL_HELD & p_pdb->p_call_ctx->call_status)) 
    {
        
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_ALRADY_ACTIVE)

        /*Send call resume failure to PA*/
        if(ICF_FAILURE != 
                icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_CALL_ALREADY_ACTIVE))
        {
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* else */
        
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

    } /* ICF_FAILURE == icf_chr_c... */   

    /*Check whether any other call is active on same line*/
    else if(ICF_FAILURE == icf_dbm_check_other_call_act(p_glb_pdb, 
                &other_call_active, p_ecode))
    {
        /*ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE*/
        ret_val = ICF_FAILURE;
    }
    /*If other call is active then send resume failure response
     * id TWC service is active then call can be resumed*/
     else if(((ICF_NULL == (ICF_SERVICE_TWC & 
            p_pdb->p_service_ctx->active_service_list.active_service_status))
            && (ICF_NULL == (ICF_CALL_STATUS_CONFERENCE & 
                             p_pdb->p_service_ctx->p_call_ctx->call_status)))
            && (ICF_TRUE == other_call_active))
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_ALRADY_ACTIVE)

        /*Send call resume failure to PA*/
        if(ICF_FAILURE != 
                icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE))
        {
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        } /* else */

        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* if(ICF_TRUE == other_call_a... */ 
    /*Start CHR timer*/
    else if(ICF_FAILURE == 
            icf_chr_start_timer(p_pdb, 
                (icf_uint8_t)ICF_CHR_HOLD_RESUME_TIMER))
    {
        ret_val = ICF_FAILURE;
    } /* if(ICF_FAILURE != ret_val) */
    /*If call is remote held or muted*/
    else if(ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
    {
	p_pdb->p_call_ctx->media_state &= ~ICF_MEDIA_STATE_LOCAL_HELD;

        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_REMOTE_HOLD)
            
        /*If Signaling ongoing in some other module*/
        if( ICF_INT_MODULE_INVALID != ICF_CHR_GET_SIG_ONGOING(p_pdb))
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SIG_ONGOING)
            
            
            ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
            
            ICF_CHR_SET_CURR_STATE(
                    p_pdb, ICF_CHR_04_AWT_ST_PROC_SIG)
        }
        else if(ICF_FAILURE == icf_chr_ssa_send_reinvite_msg(p_pdb,
                    (icf_boolean_t)ICF_FALSE))
        { 
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SSA_FAIL_RESP)
            
            /*Stop CHR timer*/
            ICF_STOP_TIMER(p_glb_pdb, 
                    p_pdb->p_chr_ctx->chr_timer_id, p_ecode, ret_val)

            p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

            ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

            ICF_CHR_RESET_ONGOING_ACT(p_pdb)
            
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_ch... */
        else
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SSA_SUCC_RESP)
            
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
        } /* else */

    } /* else if(ICF_REMOTE_HOLD == ... */        
    else if(ICF_INT_MODULE_INVALID != ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_MED_ONGOING)
           
        ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_05_AWT_ST_PROC_MEDIA)
    }
    else if(ICF_FAILURE == icf_chr_mmi_send_resume_mesg(p_pdb))       
    {
        
        /*Stop CHR timer*/
        ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
                p_ecode, ret_val)

        p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

        ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ret_val = ICF_FAILURE;

    } /* else if(ICF_FAILURE == ipps... */        
    else
    {            
        /*Go to awt mms resp state*/
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
    }

#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS==ret_val)
    {
        p_glb_pdb->load_data.call_resume_attempts++;
    }
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_00_call_resume */

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_hold
*
* DESCRIPTION  : This function is called when call hold request receives in 
*                following states
*                State: ICF_CHR_00_IDLE
*                       ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                       
*                Event: ICF_CHR_CALL_HOLD
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_xx_call_hold(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_return_t      ret_val = ICF_SUCCESS;

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
        
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_RECVD)
        
    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_HOLD_REQ)
    
    /*If ongoing_action is call resume and pending_event is not call hold*/
    if((ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD != ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_HOLD)
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_ONGOING_ACT)
    }
    /*ongoing_action is call hold and pending_event is call resume*/
    else if((ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb)))
    { /*ongoing_action is call hold and pending_event is call resume*/
        /*Reset call resume from pending_event*/
        ICF_CHR_RESET_PEND_EVT(p_pdb)
    
        /*Send success response of call hold */
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, 
               (icf_error_t)ICF_CAUSE_UNDEFINED);

        if(ICF_FAILURE != ret_val)
        {
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                     (icf_boolean_t)ICF_SUCCESS,
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));

        } /* if(ICF_FAILURE != ret_val) */
    } /* *ongoing_action is call hold and pending_event is call resume */
    /*No pending event then it is a repetative request*/
    else
    {
        /*Send failure response of call hold*/
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                    (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);
    }  /* else */ 

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_xx_call_hold */


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_mms_resp
*
* DESCRIPTION  : This function is called when mms response receives in 
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_MMS_RESP
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_01_mms_resp(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_error_t       *p_ecode = ICF_NULL;

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*reset status as media completed*/
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)
    
    /*If MMS was sent because of media change in remote response*/
    if (ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        /*Send call hold success response to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                        (icf_boolean_t)ICF_SUCCESS,
                        (icf_error_t)ICF_CAUSE_UNDEFINED);

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    else if (ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        /*Send call resume success response to PA*/
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                        (icf_boolean_t)ICF_SUCCESS, 
                        (icf_error_t)ICF_CAUSE_UNDEFINED);

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    /*If network outgoing setup failure response received and hold or resume
     * is not pending then service terminated normal*/
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call hold*/
        
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_NETWORK_ERROR);
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    else if ((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            &&  (ICF_CHR_PE_CALL_RESUME_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call resume*/
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */    
    /*If MMS is due to nw req for hold or resume failure*/
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*If pending_event is call resume, send call hold and resume success
         * response to PA*/
        /*Reset call resume from ongoing_action*/            
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)
        
        ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
                p_ecode, ret_val)
        
        p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

        ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)
        
        if(ICF_FAILURE == ret_val)
        {
            ret_val = ICF_FAILURE;           
        }
        else if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb,
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        }
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    /*If pending_event is call hold, send call resume and hold
     * success response to PA*/
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb)) && 
            (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Reset call resume from ongoing_action*/            
        ICF_CHR_RESET_ONGOING_ACT(p_pdb)

        ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

        p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

        ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

        if(ICF_FAILURE == ret_val)
        {
            ret_val = ICF_FAILURE;           
        }
        else if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb,
                        (icf_boolean_t)ICF_SUCCESS, 
                        (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        }
       
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    
    } /* (ICF_CHR_PE_CALL_HOLD == IP... */    
    /*No event is pending*/
    else if(ICF_CHR_PE_INVALID == ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        /*If Signaling_ongoing is true*/
        if(ICF_INT_MODULE_INVALID != ICF_CHR_GET_SIG_ONGOING(p_pdb))
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_SIG_ONGOING)

            ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
                
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_04_AWT_ST_PROC_SIG)
        } /* if(p_call_ctx->signaling_ongoi... */
        else if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            p_pdb->p_call_ctx->media_state |= ICF_MEDIA_STATE_LOCAL_HELD;
            
            if(ICF_FAILURE == icf_chr_ssa_send_reinvite_msg(p_pdb,
                    (icf_boolean_t)ICF_TRUE))
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

                ret_val = ICF_FAILURE;    
            } /* else */
            else
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
            } /* else */        
        } /* else if(ICF_CHR_PE_INVALID ... */    
        /*Ongoing Action is call resume*/
        else if(ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            p_pdb->p_call_ctx->media_state &= ~ICF_MEDIA_STATE_LOCAL_HELD;
            
            if(ICF_FAILURE == icf_chr_ssa_send_reinvite_msg(p_pdb,
                        (icf_boolean_t)ICF_FALSE))
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

                    ret_val = ICF_FAILURE;    
            } /* else */
            else
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
            } /* else */
        } /* else */        
    } /* else if(ICF_CHR_PE_INVALID ... */    
    /*If call hold is pending*/
    else if(ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        /*Reset media state from local_held*/
        p_pdb->p_call_ctx->media_state &= ~ICF_MEDIA_STATE_LOCAL_HELD;
        
        /*Set Ongoing action as call hold*/
        ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_CALL_HOLD)

        ICF_CHR_RESET_PEND_EVT(p_pdb)

        ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_HOLD_NO_NW_OP)

        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)

        /*Send success response of call resume */        
        if(ICF_FAILURE == icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        } /* ICF_SUCCESS, ICF_ERR_NO_... */
        /*If media is not onoing then send MMS*/
        else if(ICF_FAILURE == icf_chr_mmi_send_hold_msg(p_pdb))
        { 
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_ch... */
        /*Set await MMS resp state*/
        else
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
        } /* else */
    } /* else if(ICF_CHR_PE_CALL_HOL... */    
    /*If call resume is ongoing*/
    else if(ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        /*Set media state as local held as media is incative now*/
        p_pdb->p_call_ctx->media_state |= ICF_MEDIA_STATE_LOCAL_HELD;
        
        /*Set ongoing action as call resume*/
        ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_CALL_RESUME)

        ICF_CHR_RESET_PEND_EVT(p_pdb)

        ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_RESUME_NO_NW_OP)

        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)

        /*Send success response of call hold */        
        if(ICF_FAILURE == icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED))
        {
            ret_val = ICF_FAILURE;
        } /* ICF_SUCCESS, ICF_ERR_NO_... */
        else if(ICF_FAILURE == icf_chr_mmi_send_resume_mesg(p_pdb))
        { 
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_ch... */
        else
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
        } /* else */
    } /* else */   
    else if(ICF_CHR_PE_CALL_HOLD_NO_NW_OP == ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        p_pdb->p_call_ctx->media_state |= ICF_MEDIA_STATE_LOCAL_HELD;
        
        ICF_CHR_RESET_PEND_EVT(p_pdb)
        
        /*Send success response of call hold */        
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED);

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        if(ICF_SUCCESS == ret_val)
        {
            ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

            p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

            ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)
        } /* if(ICF_SUCCESS == ret_val) */        
    }
    else if(ICF_CHR_PE_CALL_RESUME_NO_NW_OP == 
            ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        p_pdb->p_call_ctx->media_state &= ~ICF_MEDIA_STATE_LOCAL_HELD;
        
        ICF_CHR_RESET_PEND_EVT(p_pdb)
        
        /*Send success response of call resume */        
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)ICF_CAUSE_UNDEFINED);

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        
        if(ICF_SUCCESS == ret_val)
        {
            ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

            p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

            ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)
        } /* if(ICF_SUCCESS == ret_val) */
    }
   
    if(ICF_CHR_PE_REINVITE_RECVD ==  ICF_CHR_GET_PEND_EVT(p_pdb))
    {
	/* Invoke the function to set the return status to send CC to 
         * handle the reinvite.
	 */
        if(ICF_FAILURE != ret_val)
        {
            ret_val = icf_chr_cc_send_reinvite_msg(p_pdb);
          
        }
    } 

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_chr_01_mms_resp */



/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_resume
*
* DESCRIPTION  : This function is called when call resume receives in 
*                following state
*                State: ICF_CHR_00_IDLE
*                       ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                       
*                Event: ICF_CHR_CALL_RESUME
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_xx_call_resume(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_return_t      ret_val = ICF_SUCCESS; 
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CALL_RESUME_REQ)

    /*If ongoing_action is call hold and pending_event is not call resume
     * Set pending_event = call resume as call resume is already pending*/
    if((ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME != ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
         ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_RESUME)
    }
    /*ongoing_action is call resume and pending_event is call hold*/
    else if((ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Reset call hold from pending_event*/
        ICF_CHR_RESET_PEND_EVT(p_pdb)
    
        /*Send success response of call resume */
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS, 
                (icf_error_t)ICF_CAUSE_UNDEFINED);

        if(ICF_FAILURE != ret_val)
        {
            /*Send success reponse of call hold*/
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS,
                (icf_error_t)ICF_CAUSE_UNDEFINED);
        } /* if(ICF_FAILURE == ret_val) */        
    }
    else
    {
        /*Send failure response of call resume*/
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_chr_xx_call_resume */



/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_nw_hldrsm_suc_resp
*
* DESCRIPTION  : This function is called when success response for 
*                call hold or resume is received from nw in 
*                following state
*                State: ICF_CHR_01_AWT_NW_ACK
*                Event: ICF_CHR_NW_HOLD_RESUME_SUCC_RESP
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_02_nw_hldrsm_suc_resp(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_error_t       *p_ecode = ICF_NULL;

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id, 
            p_ecode, ret_val)

    p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

    ICF_CHR_RESET_STATUS(p_pdb, 
            ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)    

    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, 
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_chr_ctx->p_chr_timer_data), 
                p_ecode);

        p_pdb->p_chr_ctx->p_chr_timer_data = ICF_NULL;
    }

    if(ICF_FAILURE == ret_val)
    {
        ret_val = ICF_FAILURE;
    } /* if(ICF_FAILURE == ret_val) */ 
    else if (ICF_FAILURE == icf_chr_copy_remote_sdp(p_pdb))
    {
         ret_val = ICF_FAILURE;
    }
    /*Send Outgoing Setup Ack to SSA*/
    /*
    else if(ICF_FAILURE == icf_chr_ssa_send_og_setup_succ_ack(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }*/   
    else if((ICF_FAILURE != ret_val) 
            && (ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb)))
    {
        /*Reset active from call state*/
        p_pdb->p_call_ctx->call_status &= ~ICF_CALL_STATUS_ACTIVE;

        /*Set local held in call state*/
        p_pdb->p_call_ctx->call_status |= ICF_CALL_STATUS_LOCAL_HELD;

        if (ICF_FAILURE == icf_chr_check_recvd_sdp_info(p_pdb))
        {
            ICF_CHR_SET_ONGOING_ACT(p_pdb,
                    ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP)

            if (ICF_FAILURE != ret_val)
            {
                /*if media is ongoing in other module*/
                if(ICF_INT_MODULE_INVALID !=
                    ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
                {
                    ICF_CHR_TRACE(p_pdb, ICF_TRACE_MED_ONGOING)

                    ICF_CHR_SET_STATUS(p_pdb,
                        ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

                    ICF_CHR_SET_CURR_STATE(p_pdb,
                        ICF_CHR_05_AWT_ST_PROC_MEDIA)
                } /* else if(p_call_ctx->media_ongo... */
                /*If media is not onoing then send MMS*/
                else if(ICF_FAILURE ==
                    icf_chr_send_mms_for_remote_resp(p_pdb))
                {
                    ret_val = ICF_FAILURE;
                } /* if(ICF_FAILURE == icf_ch... */
                /*Set await MMS resp state*/
                else
                {
                    ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
                } /* else */
            }
        } /* else if (ICF_FAILURE == ipp... */
        /*Send success reponse of call hold*/
        else if (ICF_SUCCESS == icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_SUCCESS,
                (icf_error_t)(ICF_CAUSE_UNDEFINED)))
        {
            if (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb))
            {     
                /*Call call resume function of IDLE state */
                ret_val = icf_chr_00_call_resume(p_pdb);
            } /* && (ICF_CHR_PE_CALL_RESUME !=... */
            else
            {
                ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
            }
        } /* else */
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */
    } /* if(p_chr_ctx->ongoing_action =... */
    else if((ICF_FAILURE != ret_val) 
            && (ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb)))
    {
        /*Reset local held from call state*/
        p_pdb->p_call_ctx->call_status &= ~ICF_CALL_STATUS_LOCAL_HELD;

        /*Set active in call state*/
        p_pdb->p_call_ctx->call_status |= ICF_CALL_STATUS_ACTIVE;

        /*
         * Check if remote has changed the CODEC
         * then return service error indication and call
         * will be released else continue
         */
#if 0
        if (ICF_FAILURE == icf_chr_check_recvd_codec_info(p_pdb))
        {
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        } /* if(ICF_FAILURE == icf_ch... */
        /*If some media paramters has been changed then send MMS again*/
        else 
#endif
        if((ICF_FAILURE == icf_chr_check_recvd_sdp_info(p_pdb))||
           (ICF_FAILURE == icf_chr_check_recvd_mode(p_pdb)))
        {
            ICF_CHR_SET_ONGOING_ACT(p_pdb, 
                   ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP)

            /*Copy remote received SDP to call context*/
            /*ret_val = icf_chr_copy_remote_sdp(p_pdb);*/
            
            if (ICF_FAILURE != ret_val)
            {
                /*if media is ongoing in other module*/
                if(ICF_INT_MODULE_INVALID !=
                    ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
                {
                    ICF_CHR_TRACE(p_pdb, ICF_TRACE_MED_ONGOING)
                        
                    ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)
                        
                    ICF_CHR_SET_CURR_STATE(p_pdb, 
                        ICF_CHR_05_AWT_ST_PROC_MEDIA)
                } /* else if(p_call_ctx->media_ongo... */
                /*If media is not onoing then send MMS*/
                else if(ICF_FAILURE == 
                    icf_chr_send_mms_for_remote_resp(p_pdb))
                { 
                    ret_val = ICF_FAILURE;
                } /* if(ICF_FAILURE == icf_ch... */
                /*Set await MMS resp state*/
                else
                {
                    ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
                } /* else */
            }
        } /* else if (ICF_FAILURE == ipp... */
        /*Send success reponse of call resume*/
        else if (ICF_SUCCESS == icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_SUCCESS,
                (icf_error_t)(ICF_CAUSE_UNDEFINED)))
        {
            if (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb))
            {      
                /*Call call hold function of IDLE state */
                ret_val = icf_chr_00_call_hold(p_pdb);
            } /* && (ICF_CHR_PE_CALL_HOLD !=... */
            else
            {
                ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
            }
        } /* else */
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */        
    } /* else */

    /*Set return status flag for signaling ongoing ended*/
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_02_nw_hldrsm_suc_resp */



/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_nw_og_setup_fail
*
* DESCRIPTION  : This function is called when failure response for 
*                call hold or resume is received from nw in 
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_NW_OUTGOING_SETUP_FAIL
*                
*
* RETURNS:  icf_return_t(ICF_SUCCESS or ICF_FAILURE)
*
*******************************************************************************/

icf_return_t icf_chr_02_nw_og_setup_fail(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_duration_t       timer_dur;
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_error_t       *p_ecode = ICF_NULL;
    icf_uint16_t      st_timer_range;
    icf_uint16_t      end_timer_range;

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Set return status flag for signaling ongoing ended*/
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    if(ICF_CALL_CLEAR_REASON_AUTH_FAILURE == 
            ((icf_nw_inc_media_change_resp_st *)
             (p_pdb->p_internal_msg->p_msg_data))->reason_type)
    {
        if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            /*Send failure resp for call hold*/
                ret_val = icf_chr_send_hold_resp(p_pdb, 
                        (icf_boolean_t)ICF_FAILURE, 
                        (icf_error_t)(ICF_CAUSE_AUTH_FAILED));
            
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_ERROR)
        }
        else if(ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
             /*Send failure resp for call hold*/
                ret_val = icf_chr_send_resume_resp(p_pdb, 
                        (icf_boolean_t)ICF_FAILURE, 
                        (icf_error_t)(ICF_CAUSE_AUTH_FAILED));
            
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_ERROR)
        }
    }
    /*If ongoing action is call hold and pending event is call resume
     * Send success response to PA for both*/
    else if((ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        && (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb)))       
    {
        ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_NW_OG_SETUP_FAIL)

        if(ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_REMOTE_HOLD)
            
            /*Reste pending_event*/
            ICF_CHR_RESET_PEND_EVT(p_pdb)
 
            /*Send success resp for call hold*/
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));

            /*Send success resp for call resume*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));

            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        } /* if(ICF_REMOTE_HOLD == p_cal... */
        /*Media is ongoing in some other module */
        else if(ICF_INT_MODULE_INVALID != 
                ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
        {
            /*SET START PROC MEDIA IN SERVICE CONTEXT*/
            ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)
            
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_05_AWT_ST_PROC_MEDIA)
        } /* else if(p_call_ctx->media_ongo... */
        /*If media not going on in other module then send MMS*/
        else if(ICF_FAILURE == icf_chr_mmi_send_resume_mesg(p_pdb))
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

            ret_val = ICF_FAILURE;
        } /* else if(ICF_FAILURE == ipps... */
        else
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
        } /* else */
    } /* if(ICF_CHR_PE_CALL_RESUME =... */
    /*ongoing_action is call resume*/
    else if((ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_NW_OG_SETUP_FAIL)

        if(ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
        {
            ICF_CHR_TRACE(p_pdb, ICF_TRACE_CALL_REMOTE_HOLD)
            
            /*Reset pending_event*/
            ICF_CHR_RESET_PEND_EVT(p_pdb)
            
            p_pdb->p_call_ctx->call_status &= ~ICF_CALL_STATUS_ACTIVE;
            
            p_pdb->p_call_ctx->call_status |= ICF_CALL_STATUS_LOCAL_HELD;
            
            /*Send success resp for call resume*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));
            
            /*Send success resp for call hold*/
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));
            
            ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        } /* if(ICF_REMOTE_HOLD == p_cal... */
        /*If media is ongoing in some other module*/
        else if(ICF_INT_MODULE_INVALID != 
                ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
        {
            /*SET START PROC MEDIA IN SERVICE CONTEXT*/
            ICF_CHR_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)
                
            ICF_CHR_SET_CURR_STATE(p_pdb,
                    ICF_CHR_05_AWT_ST_PROC_MEDIA)
        } /* else if(p_call_ctx->media_ongo... */
        /*If media not ongoing then send MMS*/
        else if(ICF_FAILURE == icf_chr_mmi_send_hold_msg(p_pdb))
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
            
            ret_val = ICF_FAILURE;
        } /* else if(ICF_FAILURE == ipps... */
        else
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
        } /* else */
    }
    /*
     * Check the reason code in failure and if it is 491 then
     * Calculate some random timer to send reinvite again
     * else send service error in status and send call hold failure
     */
    else if(ICF_CALL_CLEAR_REASON_REQUEST_PENDING == 
            ((icf_nw_inc_media_change_resp_st *)
             (p_pdb->p_internal_msg->p_msg_data))->reason_type)
    {
        p_pdb->p_chr_ctx->p_race_timer_data = ICF_NULL;

        if(ICF_FAILURE ==
              icf_dbm_get_mem_from_pool(p_glb_pdb, 
                  (icf_pool_id_t)ICF_MEM_TIMER,
                  (icf_void_t *)&p_pdb->p_chr_ctx->p_race_timer_data,
                  p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            p_pdb->p_chr_ctx->p_race_timer_data->timer_type = 
                ICF_CHR_RACE_TIMER;
            
            p_pdb->p_chr_ctx->p_race_timer_data->module_id = 
                ICF_INT_MODULE_CHR;
            /*
             * Store call context pointer in p_timer buffer
             * so that when timer expiry receives module can
             * identify the call data
             */
            p_pdb->p_chr_ctx->p_race_timer_data->p_timer_buffer = 
                (icf_void_t *)p_pdb->p_call_ctx;

            p_pdb->p_chr_ctx->p_race_timer_data->p_glb_pdb = p_glb_pdb;

            if (ICF_CALL_DIR & p_pdb->p_call_ctx->common_bitmask)
            {
                st_timer_range = ICF_CMN_INC_RACE_TIMER_LOW_LIMIT;
                end_timer_range = ICF_CMN_INC_RACE_TIMER_UP_LIMIT;
            } /* if (ICF_CALL_DIR & p_pdb->p... */
            else
            {
                st_timer_range = ICF_CMN_OG_RACE_TIMER_LOW_LIMIT;
                end_timer_range = ICF_CMN_OG_RACE_TIMER_UP_LIMIT;
            } /* if (ICF_CALL_DIR & p_pdb->p... */

            /*Get random time value for reinvite race timer*/
            if(ICF_FAILURE == icf_cmn_generate_timer(&timer_dur, 
                        st_timer_range, end_timer_range))
            {
                ret_val = ICF_FAILURE;
            } /* ICF_CHR_RACE_TIMER_UP_LIMIT... */
            else
            {
                    
                ICF_START_TIMER(p_glb_pdb, 
                        p_pdb->p_chr_ctx->p_race_timer_data, 
                        p_pdb->p_chr_ctx->race_timer_id, 
                        timer_dur, p_ecode, ret_val)

                ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STARTED)
            }
        } /* else */
    } /* else if(ICF_REQUEST_PENDING... */
    else
    {
        if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_NW_OG_SETUP_FAIL)
        
            ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_HOLD_NO_NW_OP)

            if(ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
            {
                /*Send failure resp for call hold*/
                ret_val = icf_chr_send_hold_resp(p_pdb, 
                        (icf_boolean_t)ICF_FAILURE, 
                        (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
            
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
            }                
            else if(ICF_INT_MODULE_INVALID != 
                    ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
            {
                /*SET START PROC MEDIA IN SERVICE CONTEXT*/
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

                ICF_CHR_SET_CURR_STATE(p_pdb,
                        ICF_CHR_05_AWT_ST_PROC_MEDIA)
            } /* else if(p_call_ctx->media_ongo... */
            /*If media not ongoing then send MMS*/
            else if(ICF_FAILURE == 
                    icf_chr_mmi_send_resume_mesg(p_pdb))
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

                ret_val = ICF_FAILURE;
            } /* else if(ICF_FAILURE == ipps... */
            else
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
            }
        }
        else if(ICF_CHR_OA_CALL_RESUME == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            ICF_CHR_SET_ONGOING_ACT(p_pdb, ICF_CHR_OA_NW_OG_SETUP_FAIL)

            ICF_CHR_SET_PEND_EVT(p_pdb, ICF_CHR_PE_CALL_RESUME_NO_NW_OP)

            if(ICF_SUCCESS == icf_chr_check_remote_status(p_pdb))
            {
                /*Send failure resp for call resume*/
                ret_val = icf_chr_send_resume_resp(p_pdb, 
                        (icf_boolean_t)ICF_FAILURE,
                        (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));

                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
            }
            else if(ICF_INT_MODULE_INVALID != 
                    ICF_CHR_GET_MEDIA_ONGOING(p_pdb))
            {
                /*SET START PROC MEDIA IN SERVICE CONTEXT*/
                ICF_CHR_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

                ICF_CHR_SET_CURR_STATE(p_pdb,
                        ICF_CHR_05_AWT_ST_PROC_MEDIA)
            } /* else if(p_call_ctx->media_ongo... */
            /*If media not ongoing then send MMS*/
            else if(ICF_FAILURE == 
                    icf_chr_mmi_send_hold_msg(p_pdb))
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

                ret_val = ICF_FAILURE;
            } /* else if(ICF_FAILURE == ipps... */
            else
            {
                ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
            }
        }
    } /* if(ICF_CHR_OA_CALL_HOLD == ... */

    /*
     * Send ACK for outgoing request failure response
     */
    /*
    if(ICF_FAILURE != ret_val)
    {
        ret_val = icf_chr_ssa_send_og_setup_fail_ack(p_pdb);
    }*/
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_02_nw_og_setup_fail */

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_03_nw_og_setup_fail
*
* DESCRIPTION  : This function is called when failure response for INVITE
*                received in 
*                following state
*                State: ICF_CHR_03_ACK_FOR_CANCEL_RESP
*                Event: ICF_CHR_NW_OUTGOING_SETUP_FAIL
*                
*
* RETURNS:  
*
*******************************************************************************/
#if 0
icf_return_t icf_chr_03_nw_og_setup_fail(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;
    icf_error_t        *p_ecode = ICF_ERROR_NONE;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_ecode))
    
    /*Set return status flag for signaling ongoing ended*/
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    if(ICF_TRUE == p_pdb->p_chr_ctx->ret_error)
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    } /* if(ICF_TRUE == p_chr_ctx->r... */
    else
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    } /* else */

    ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

    ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

    p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;

} /* icf_chr_03_nw_og_setup_fail */
#endif

 

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_03_nw_req_cnl_resp
*
* DESCRIPTION  : This function is called when success response for 
*                CANCEL is received from nw in 
*                following state
*                State: ICF_CHR_03_ACK_FOR_CANCEL_RESP
*                Event: ICF_CHR_NW_ACK_FOR_REQ_CANCEL
*                
*
* RETURNS:  
*
*******************************************************************************/
#if 0 
icf_return_t icf_chr_03_nw_req_cnl_resp(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    p_pdb = p_pdb;

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_chr_03_nw_req_cnl_resp */
#endif

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_04_st_proc_sig
*
* DESCRIPTION  : This function is called when Start Proc signaling  
*                is received from CM in 
*                following state
*                State: ICF_CHR_04_START_PROC_SIGNALING
*                Event: ICF_CHR_START_PROC_SIGNALING
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_04_st_proc_sig(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Set signaling event request in return status*/
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)

    if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        if(ICF_FAILURE != icf_chr_ssa_send_reinvite_msg(p_pdb,
                (icf_boolean_t)(ICF_TRUE)))
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
        }
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */
    } /* if(ICF_CHR_OA_CALL_HOLD == ... */
    else  
    {
        if(ICF_FAILURE !=   icf_chr_ssa_send_reinvite_msg(p_pdb,
                (icf_boolean_t)(ICF_FALSE)))
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_02_AWT_NW_ACK)
        }
        else
        {
            ret_val = ICF_FAILURE;
        } /* else */
    } /* else */    

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
} /* icf_chr_04_st_proc_sig */


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_05_st_proc_media
*
* DESCRIPTION  : This function is called when Start Proc media
*                is received from CM in 
*                following state
*                State: ICF_CHR_05_START_PROC_MEDIA
*                Event: ICF_CHR_START_PROC_MEDIA
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_05_st_proc_media(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)

    if ((ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))||
        (ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb)))
    {
        if(ICF_FAILURE == icf_chr_send_mms_for_remote_resp(p_pdb))
        { 
            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == icf_ch... */
        /*Set await MMS resp state*/
        else
        {
            ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
        } /* else */ 
    } /* if (ICF_CHR_OA_MMS_FOR_REMO... */
    /*Network outgoing setup failure received and hold or resume not pending*/
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ret_val = icf_chr_mmi_send_hold_msg(p_pdb);
    } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ret_val = icf_chr_mmi_send_resume_mesg(p_pdb);
    }  
    /*Handle NW failure: send MMS as per the pending_event*/
    else if ((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
        && (ICF_CHR_PE_CALL_RESUME == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ret_val = icf_chr_mmi_send_resume_mesg(p_pdb);
    } /* && (ICF_CHR_OA_CALL_RESUME ... */
    else if ((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
        && (ICF_CHR_PE_CALL_HOLD == ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        ret_val = icf_chr_mmi_send_hold_msg(p_pdb);
    } /* && (ICF_CHR_OA_CALL_RESUME ... */    
    /*
     * If ongoing_action is call hold then send call hold mms else
     * send call resume mms
     */
    else if (ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        ret_val = icf_chr_mmi_send_hold_msg(p_pdb);
    } /* if(ICF_CHR_OA_CALL_HOLD == ... */
    else  
    {
        ret_val = icf_chr_mmi_send_resume_mesg(p_pdb);
    } /* else */    

    if (ICF_FAILURE == ret_val)
    {
         ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* if(ICF_FAILURE == ret_val) */
    else
    {
         ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_01_AWT_MMS_RESP)
    }
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /*  icf_chr_05_st_proc_media */



/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_hldrsm_req_frm_nw
*
* DESCRIPTION  : This function is called when request from nw is received
*                for call hold or resume in 
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                Event: ICF_CHR_HLD_RSM_REQ_FROM_NW
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_xx_hldrsm_req_frm_nw(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ret_val = icf_chr_ssa_send_invite_reject(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
} /* icf_chr_xx_hldrsm_req_frm_nw */

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_hldrsm_req_frm_nw
*
* DESCRIPTION  : This function is called when request from nw is received
*                for call hold or resume in 
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_HLD_RSM_REQ_FROM_NW
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_01_hldrsm_req_frm_nw(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_req_st *p_inc_req = ICF_NULL,
                                   *p_pending_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Buffer this remote reinvite only if MMS is ongoing for a recvd response
      and send 491 in case this MMS is for an outgoing hold/resume request*/
    if((ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb))||
      (ICF_CHR_OA_MMS_FOR_RESUME_REMOTE_RESP == ICF_CHR_GET_ONGOING_ACT(p_pdb)))
    {
        p_inc_req = (icf_nw_inc_media_change_req_st*) p_pdb->p_internal_msg->p_msg_data;
        p_pdb->p_call_ctx->common_bitmask_2 |= ICF_CC_BUFFERED_INC_MEDIA_UPDATE;

	ICF_MEMGET(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
		ICF_PORT_SIZEOF(icf_nw_inc_media_change_req_st),
		ICF_MEM_COMMON,
		(p_pdb->p_chr_ctx->p_pending_req),
		ICF_RET_ON_FAILURE,
		&(p_pdb->p_internal_msg->msg_hdr.ecode),
		ret_val);

        p_pending_req = p_pdb->p_chr_ctx->p_pending_req;

	icf_port_memcpy(p_pdb->p_chr_ctx->p_pending_req,
		p_pdb->p_internal_msg->p_msg_data,
		ICF_PORT_SIZEOF(icf_nw_inc_media_change_req_st));

       /* The p_added_sdp_list pointer is freed by SSA in return path so clone
        * the data in the list to pass it on to CC. This cloned list would be
        * freed by CC 
        */
       p_pending_req->p_added_sdp_list = ICF_NULL;

       if(ICF_NULL != p_inc_req->p_added_sdp_list)
       {
           icf_cmn_clone_media_sdp_info_list(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                              &(p_pending_req->p_added_sdp_list),
                              p_inc_req->p_added_sdp_list,
                              &(p_pdb->p_internal_msg->msg_hdr.ecode));
        }

        ICF_CHR_SET_PEND_EVT(p_pdb,ICF_CHR_PE_REINVITE_RECVD)
    }
    else
    {
        ret_val = icf_chr_ssa_send_invite_reject(p_pdb);
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
} /* icf_chr_01_hldrsm_req_frm_nw */

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_media_error
*
* DESCRIPTION  : This function is called when media error received in
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_MEDIA_ERROR
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_01_media_error(
        INOUT  icf_chr_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t   *p_ecode;
    icf_mm_err_ind_st   *p_mm_error = ICF_NULL;
    icf_mmi_reason_t    mmi_reason = 0;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
        
    if (ICF_TRUE == p_pdb->mm_err_recvd)
    {
        p_mm_error = (icf_mm_err_ind_st *)p_pdb->p_internal_msg->p_msg_data;
        mmi_reason = p_mm_error->reason;
    }

    ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

    p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)

    /*If timer can not start properly*/
    if (ICF_FAILURE == ret_val)
    {
        ret_val = ICF_FAILURE;
    } /* if(ICF_FAILURE == ret_val) */
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call hold*/
        
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
            
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_RESUME ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call hold*/
        
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
            
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    /*If network outgoing setup failure response received and hold or resume
     * is not pending then service terminated error*/
    else if((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            && (ICF_CHR_PE_CALL_HOLD_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call hold*/
        
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
            
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    else if ((ICF_CHR_OA_NW_OG_SETUP_FAIL == 
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            &&  (ICF_CHR_PE_CALL_RESUME_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call resume*/
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_NETWORK_ERROR));
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        
        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */
    /*If MMS was sent because of media change in remote response*/
     else if ((ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP == 
              ICF_CHR_GET_ONGOING_ACT(p_pdb))||
             (ICF_CHR_OA_MMS_FOR_HOLD_REMOTE_RESP ==
              ICF_CHR_GET_ONGOING_ACT(p_pdb)))
    {
        /*Initiate call termination*/
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    }
    /* if MMS failure response is received when ongonig event is 
     * call hold and reinvite is not to be sent on network,send 
     * failure response of call hold to application.*/
    else if ((ICF_CHR_OA_CALL_HOLD ==
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            &&  (ICF_CHR_PE_CALL_HOLD_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call resume*/
        ret_val = icf_chr_send_hold_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));

        /* If mms for call resume was done successfully and reinvite was not
           send on network, clear the call*/
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */

    /* if MMS failure response is received when ongonig event is 
     * call resume and reinvite is not to be sent on network,send 
     * failure response of call resume to application.*/
    else if ((ICF_CHR_OA_CALL_RESUME ==
                ICF_CHR_GET_ONGOING_ACT(p_pdb))
            &&  (ICF_CHR_PE_CALL_RESUME_NO_NW_OP ==
                ICF_CHR_GET_PEND_EVT(p_pdb)))
    {
        /*Send failure resp for call resume*/
        ret_val = icf_chr_send_resume_resp(p_pdb,
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));

        /* If mms for call hold was done successfully and reinvite was not
           send on network, clear the call*/
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* ICF_CHR_GET_PEND_EVT(p_pdb)... */    
    else if ((ICF_ECODE_DMS_IND_RECVD == mmi_reason) || 
         (ICF_ECODE_MMI_INITIATED_CALL_CLEAR == mmi_reason))
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
    } /* if(ICF_ECODE_MMS_RESP_FAILURE_MEDIA_ERROR == p_mm_erro... */    
    /*If pending event*/
    else if (ICF_CHR_PE_INVALID != ICF_CHR_GET_PEND_EVT(p_pdb))
    {
        /*Then return status is service ended normal*/
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        
        /*Ongoing action is call hold*/
        if (ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        {
            /*Send hold success response to PA*/
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));

            /*If send successfully to PA*/
            if (ICF_FAILURE != ret_val)
            {
                /*Send resume success response to PA*/
                ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));
            }
        } /* if(ICF_CHR_OA_CALL_HOLD == ... */
        /*If ongoing action is call resume*/
        else
        {
            /* Send resume success response to PA*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));

            if (ICF_FAILURE != ret_val)
            {
                /*Send hold success response to PA*/
                ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_SUCCESS, 
                    (icf_error_t)(ICF_CAUSE_UNDEFINED));
            } /* if(ICF_FAILURE != ret_val) */            
        }
    } /* && (ICF_CHR_PE_INVALID != I... */
    /*
     * If ongoing action is call hold service ended with error
     */
    else if (ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        /*If no pending event then service ended with error*/
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
       
        /*Send hold failure response to PA*/
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    } /* else if(ICF_CHR_OA_CALL_HOL... */
    /*
     * If ongoing action is call resume service ended with error
     */
    else
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
     
        /*
         * Send resume failure response to PA
         */
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    }
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    return ret_val;
        
} /* icf_chr_01_media_error */

 
/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_media_error
*
* DESCRIPTION  : This function is called when media error received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_MEDIA_ERROR
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_02_media_error(
        INOUT  icf_chr_pdb_st    *p_pdb
        ) 
{ 
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_error_t            *p_ecode = ICF_NULL;
    icf_mm_err_ind_st      *p_mm_error = ICF_NULL;
    icf_mmi_reason_t        mmi_reason = 0;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Earlier CHR has set the request of all media events
     * hence has the lock for all media events
     * after getting the media error reset the media event req flag
     * in return status
     */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_MEDIA_ONGOING_ENDED)
    
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*
     * Get the icf_mm_err_ind_st from p_msg_data received
     * in intternbal message
     */

    if (ICF_TRUE == p_pdb->mm_err_recvd)
    {
        p_mm_error = (icf_mm_err_ind_st *)p_pdb->p_internal_msg->p_msg_data;
        mmi_reason = p_mm_error->reason;
    }
    /*
     * Stop timer as the service is going to be terminated with error
     */
    ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

    p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

    /*
     * If the timer could not stop successfully
     */
    if(ICF_FAILURE == ret_val)
    {
        ret_val = ICF_FAILURE;
    } /* if(ICF_FAILURE == ret_val) */
    else if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
    {
        /*Send failure resp for call hold*/
        ret_val = icf_chr_send_hold_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    } /* if(ICF_CHR_OA_CALL_HOLD == ... */
    else
    {
        /*Send failure resp for call hold*/
        ret_val = icf_chr_send_resume_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE, 
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
    } /* else */

    /*
     * Send cancel to remote, if failure then return failure to SCM
     */
    if (ICF_FAILURE != ret_val )
    {
        if((ICF_ECODE_DMS_IND_RECVD == mmi_reason) || 
            (ICF_ECODE_MMI_INITIATED_CALL_CLEAR == mmi_reason))
        {
            p_pdb->p_chr_ctx->ret_error = ICF_TRUE;   
        } /* if(ICF_DMS_IND == p_mm_erro... */
        else
        {
            p_pdb->p_chr_ctx->ret_error = ICF_FALSE;       
        } /* else */

        if (ICF_NULL != p_pdb->p_call_ctx->p_sip_ctx)
            ret_val = icf_chr_ssa_send_cancel_msg(p_pdb);

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
 
    } /* if (ICF_FAILURE != ret_val ... */
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;

} /* icf_chr_02_media_error */


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb
         )
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          *p_msg = ICF_NULL;
    icf_timer_data_st   *p_timer_data = ICF_NULL;
    icf_error_t        *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);    
    
    /*
     * Copy the timer expiry message from p_recv_msg of
     * global PDB*/
    p_msg = p_glb_pdb->p_recv_msg;
    
    /*p_msg = (icf_msg_st *)p_pdb->p_internal_msg->p_msg_data;*/
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_msg))
    
    /*Get p_timer_data from p_msg->payload*/
    p_timer_data = *(icf_timer_data_st **)(p_msg->payload);

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_timer_data))

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_EXPIRY)
   
    /*If expired timer is call hold resume timer*/
    if (ICF_CHR_HOLD_RESUME_TIMER == p_timer_data->timer_type)
    {
        /*And Call hold was ongoing*/
        if (ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        { 
            /*Send Call hold failure response*/
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_FAILURE, 
                    (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));
        } /* else if(ICF_CHR_OA_CALL_HOL... */
        else            
        {
            /*
             * Or Call resume was ongoing while timer expiry received
             * send failure call resume response to PA*/
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_FAILURE, 
                    (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));
        }
       
        /*Trigger SSA to abort invite transaction*/ 
        if ((ICF_SUCCESS == ret_val) && 
            (ICF_NULL != p_pdb->p_call_ctx->p_sip_ctx))
        {
            ret_val = icf_chr_ssa_send_cancel_msg(p_pdb);
        }

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)

    }/*if (ICF_CHR_HOLD_RESUME_TIM... */
    /*If expired timer is reinvite race timer*/
    else if (ICF_CHR_RACE_TIMER == p_timer_data->timer_type)
    {
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, 
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_chr_ctx->p_race_timer_data), 
                p_ecode);

        p_pdb->p_chr_ctx->p_race_timer_data = ICF_NULL;

        if(ICF_FAILURE != ret_val)
        {
            /*Send nw request again to hold or resume the call*/
            if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
            {
                ret_val = icf_chr_ssa_send_reinvite_msg(p_pdb,
                        (icf_boolean_t)(ICF_TRUE));
            } /* if(ICF_CHR_OA_CALL_HOLD == ... */
            else
            {
                ret_val = icf_chr_ssa_send_reinvite_msg(p_pdb,
                        (icf_boolean_t)(ICF_FALSE));
            } /* else */            
        } /* if(ICF_FAILURE != ret_val) */
    }/*else if(ICF_CHR_REINVITE_RA... */

    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}/*icf_chr_02_timer_expiry*/
    


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_03_AWT_CANCEL_RESP
*                       ICF_CHR_04_AWT_ST_PROC_SIG
*                       ICF_CHR_05_AWT_ST_PROC_MEDIA
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb
         )
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          * p_msg = ICF_NULL;
    icf_timer_data_st   *p_timer_data = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
 
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Copy the timer expiry message from p_recv_msg of
     * global PDB*/
    p_msg = p_glb_pdb->p_recv_msg;     
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_msg))
    
    /*Get p_timer_data from p_msg->payload*/
    p_timer_data = *(icf_timer_data_st **)&(p_msg->payload[0]);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_timer_data))

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_EXPIRY)
   
    /*If expired timer is call hold resume timer*/
    if(ICF_CHR_HOLD_RESUME_TIMER == p_timer_data->timer_type)
    {
        /*And call hold is received*/
        if(ICF_CHR_OA_CALL_HOLD == ICF_CHR_GET_ONGOING_ACT(p_pdb))
        { 
            /*
             * Send call hold failure response with timer expiry
             * as error cause
             */
            ret_val = icf_chr_send_hold_resp(p_pdb, 
                    (icf_boolean_t)ICF_FAILURE, 
                    (icf_error_t)ICF_CAUSE_TIMER_EXPIRY);
        } /* else if(ICF_CHR_OA_CALL_HOL... */
        else
        {
            /*
             * Send call resume failure response with timer expiry
             * as error cause
             */
            ret_val = icf_chr_send_resume_resp(p_pdb, 
                    (icf_boolean_t)ICF_FAILURE, 
                    (icf_error_t)ICF_CAUSE_TIMER_EXPIRY);
        }

        /*ICF_CHR_SET_ECODE(p_pdb, ICF_CAUSE_TIMER_EXPIRY)*/

        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
            
    }/*if(ICF_CHR_HOLD_RESUME_TIME... */    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}/*icf_chr_xx_timer_expiry*/

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_release
*
* DESCRIPTION  : This function is called when call release received
*                following state
*                State: ICF_CHR_03_AWT_CANCEL_RESP
*                       ICF_CHR_04_AWT_ST_PROC_SIG
*                       ICF_CHR_05_AWT_ST_PROC_MEDIA
*                Event: ICF_CHR_CALL_RELEASE
*                
* RETURNS:  
*******************************************************************************/
icf_return_t icf_chr_xx_call_release(
         INOUT  icf_chr_pdb_st *p_pdb
         )
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_error_t     *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
         
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
   
    ICF_STOP_TIMER(p_glb_pdb, p_pdb->p_chr_ctx->chr_timer_id,
            p_ecode, ret_val)

    p_pdb->p_chr_ctx->chr_timer_id = ICF_NULL;

    ICF_STAT(p_glb_pdb, ICF_STAT_CHR_TMR_STOPPED)

    if (ICF_FAILURE == icf_chr_send_app_resp_on_call_term(p_pdb))
    {
      ret_val = ICF_FAILURE;
    }
    else
    {
    	ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    	ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
	}

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_call_release
*
* DESCRIPTION  : This function is called when call release received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_CALL_RELEASE
*                
* RETURNS:  
*******************************************************************************/
icf_return_t icf_chr_02_call_release(
         INOUT  icf_chr_pdb_st *p_pdb
         )
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    /*
     * If call release received when CHR is waiting for response from
     * network, it sends cancel on network
     */
    if (ICF_NULL != p_pdb->p_call_ctx->p_sip_ctx)
    {
        ret_val = icf_chr_ssa_send_cancel_msg(p_pdb);
    }

    ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        
    if (ICF_FAILURE == icf_chr_send_app_resp_on_call_term(p_pdb))
    {
      ret_val = ICF_FAILURE;
    }
    else
    { 
    	ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    	ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)
    
		p_pdb->p_chr_ctx->ret_error = ICF_TRUE;
	}
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;
}

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_unexpected_event
*
* DESCRIPTION  : This function is called when any unexpected event receives
*                
* RETURNS:  
*******************************************************************************/
icf_return_t   icf_chr_unexpected_event(
        INOUT   icf_chr_pdb_st   *p_pdb)
{

    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
        
    p_pdb = p_pdb;
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* INOUT   icf_chr_pdb_st   *p... */

/*******************************************************************************
*
* FUNCTION NAME : icf_chr_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_chr_xx_forceful_call_clear(
        INOUT  icf_chr_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Send service ended error */
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_chr_xx_forceful_call_clear */

/******************************************************************************
*
* FUNCTION NAME    : icf_chr_00_call_release
*
* DESCRIPTION  : This function is called when call release received
*                following state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_RELEASE
*                
* RETURNS:  
*******************************************************************************/
icf_return_t icf_chr_00_call_release(
         INOUT  icf_chr_pdb_st *p_pdb
         )
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    ICF_CHR_SET_CURR_STATE(p_pdb, ICF_CHR_00_IDLE)
        
    ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    return ret_val;
} /* icf_chr_00_call_release */
#endif
