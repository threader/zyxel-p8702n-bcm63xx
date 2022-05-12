/*******************************************************************************
*
* FILE NAME    : icf_ctu_sm.c
*
* DESCRIPTION  : Contains the definitions of all State Machine functions
*                of CTU service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 25-Jan-2005    Amit Sharma     None            Initial
* 28-Feb-2005    Amit Sharma                    Added handling for forceful
*                                               Call Clear event
* 29-Apr-2005    Amit Sharma     SPR:7471       CTU failure response when call
*                                               is in setup or release phase
* 19-May-2005  Amit Sharma     SPR:7561       Added 03_forceful function
* 28-May-2005  Amit Sharma     SPR:7627       Returned signal_ong ended
* 12-Nov-2005  Amit Sharma     SPR:8185       
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
* 01-Sep-200r65  Amit Sharma     SPR:12668       
* 24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
* 11-Sept-2007 Abhishek Dhammawat SPR16304    Modified 
*                                             icf_ctu_00_internal_refer_req
* Copyright 2006,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_ctu_types.h"
#include "icf_ctu_macro.h"
#include "icf_ctu_prototypes.h"
#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif 

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_00_internal_refer_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_00_IDLE
*                 Event         ICF_CTU_INTERNAL_REFER_REQUEST
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_00_internal_refer_req (INOUT  icf_ctu_pdb_st    *p_pdb)
{
        icf_internal_refer_req_st   *p_ctu_req = ICF_NULL;
	    icf_error_t         *p_ecode = ICF_NULL; 
	    icf_return_t        ret_val = ICF_SUCCESS;
       
        /*Copy transferred party address from p_pdb->p_internal_msg->p_msg_data which is 
         *icf_internal_refer_req_st*/
        p_ctu_req = (icf_internal_refer_req_st*)(p_pdb->p_internal_msg->p_msg_data);
        p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

        icf_port_memcpy(&(p_pdb->p_ctu_ctx->transferred_addr),
                                     &(p_ctu_req->refer_uri),
                                    (icf_uint32_t)sizeof(icf_address_st));

        /*Set toggle_pi as false, as there is no such request from application*/
        p_pdb->p_ctu_ctx->toggle_pi = ICF_FALSE;
        p_pdb->p_ctu_ctx->internal_refer_req = ICF_TRUE;

        if (ICF_FAILURE == icf_ctu_validate_referred_address(p_pdb,
                    &(p_pdb->p_ctu_ctx->transferred_addr)))
       { 
              /*Return CTU failure to XCONF*/
              ICF_CTU_SET_STATUS_2(p_pdb, 
                        ICF_STATUS_INTERNAL_REFER_FAILURE_RESP)

              *p_ecode = ICF_CAUSE_INVALID_ADDRESS_RECEIVED;
              ret_val = ICF_FAILURE;
       }
        else if (ICF_SUCCESS == icf_ctu_start_ctu_timer(p_pdb))
        {
        /*Check if CHR service is active*/
        if (ICF_SERVICE_CHR & 
            p_pdb->p_service_ctx->active_service_list.active_service_status)
        {
            /*If CHR service is active wait for its response*/
            ICF_CTU_SET_CURR_STATE(p_pdb,
                    ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP)

            /*If CHR is active for call hold as call is currently active*/
            if (ICF_CALL_STATUS_LOCAL_HELD ==
                    p_pdb->p_call_ctx->call_status)
            {
                ICF_CTU_SET_STATUS(p_pdb,
                        ICF_STATUS_SERVICE_CALL_RESUME_REQ)
            } /* if(ICF_CALL_STATUS_ACTIVE =... */
            else
            {
                /*CHR is active for call resume*/
                ICF_CTU_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_CALL_HOLD_REQ)
            } /* else */
        } /* p_pdb->p_service_ctx->active_s... */
        else
        {
            /*Following function checks for signaling ongoing flag
             * and if not ongoing then send REFER request else
             * wait for start proc signaling*/
            ret_val = icf_ctu_check_sig_ong_n_send_refer(p_pdb);
        } /* if(ICF_SUCCESS == icf_ct... */
    }
    else
    {
        ret_val = ICF_FAILURE;
    }
    return ret_val;
}



/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_00_call_transfer_unattended_request
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_00_IDLE
*                 Event         ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_00_call_transfer_unattended_request(
        INOUT  icf_ctu_pdb_st    *p_pdb)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_pa_cc_call_transfer_unattended_req_st  *p_ctu_req = ICF_NULL;
    
    /*Copy p_glb_pdb from internal msg to local ptr*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STAT(p_glb_pdb, ICF_STAT_PA_CC_CTU_REQ)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
        
    /*Copy incoming CTU request pointer*/
    p_ctu_req = (icf_pa_cc_call_transfer_unattended_req_st *)
        &(p_glb_pdb->p_recv_msg->payload[0]);
    
    /*Copy transferred party address to ctu_ctx from recv message*/
    icf_port_memcpy(&(p_pdb->p_ctu_ctx->transferred_addr),
            &(p_ctu_req->transferred_party_addr),
            (icf_uint32_t)sizeof(icf_address_st));

       p_pdb->p_ctu_ctx->p_uri_parameter = ICF_NULL;
   
    ICF_CTU_APP_TRACE(p_pdb, ICF_TRACE_CTU_REQ)

    if(ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES & p_ctu_req->bit_mask)
    {
         /*Allocate memory for API header strucure */
	 ICF_MEMGET(p_glb_pdb, ICF_MAX_LARGE_STR_LEN, ICF_MEM_COMMON,
            p_pdb->p_ctu_ctx->p_uri_parameter, ICF_DONOT_RET_ON_FAILURE,
            p_ecode, ret_val)
        if(ICF_FAILURE != ret_val)
        {
  	    icf_port_memcpy(p_pdb->p_ctu_ctx->p_uri_parameter,
			    p_ctu_req->p_uri_parameter,
                            sizeof(icf_large_string_st));
        }
        else
        {
  	    /*Return CTU failure to PA*/
	    ret_val = icf_ctu_send_ctu_resp(p_pdb, 
			    (icf_boolean_t)(ICF_FAILURE),
			    (icf_error_t)(ICF_CAUSE_INTERNAL_ERROR));
        
	    ICF_CTU_SET_STATUS(p_pdb, 
			    ICF_STATUS_SERVICE_ENDED_NORMAL)

            /*
         * Here we would be returning success if icf_ctu_send_ctu_resp has
         * returned success as the response to application was sent
         * successfully and call should only get dropped if application
         * wants to terminate it
         */
        return ret_val;

        }
    }
    /*Copy presentaiton indicator to CTU ctx, if presence flag is on*/
    if(ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR
            & p_ctu_req->bit_mask)
    {
        p_pdb->p_ctu_ctx->toggle_pi = p_ctu_req->toggle_presentation_indicator;
    } /* == p_ctu_req->presence_flag) */
    else
    {
        p_pdb->p_ctu_ctx->toggle_pi = ICF_FALSE;
    } /* else */
    
    ICF_CTU_TRACE(p_pdb, ICF_TRACE_CTU_RECVD)
    
    /* If CHR service is active, wait for hold resume response
     * else check if signaling ongoing is false send REFER to NW
     * and wait for REFER response
     * else wait for start proc signaling*/
   
     if (ICF_FAILURE == icf_ctu_clone_app_spec_hdr_lists(p_pdb))
     {
         ret_val = ICF_FAILURE;
     }
    /*If call is not connected or is in releasing state then send failure
     * CTU response and service terminated normal*/
    else if (!(ICF_CALL_CONNECT_DONE & p_glb_pdb->p_call_ctx->common_bitmask)
            || (ICF_REL_TYPE_INVALID != p_glb_pdb->p_call_ctx->release_type))
    {
        /*Return CTU failure to PA*/
        ret_val = icf_ctu_send_ctu_resp(p_pdb, 
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_INVALID_SERVICE_ACTION));
        
        ICF_CTU_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
    } /* || (ICF_REL_TYPE_INVALID !=... */    
    /*Validate received address of referred party*/
    else if (ICF_FAILURE == icf_ctu_validate_referred_address(p_pdb,
                    &(p_pdb->p_ctu_ctx->transferred_addr)))
    {
        ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_INVALID_ADDRESS)

        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
        ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)
        
        /*Return CTU failure to PA*/
        ret_val = icf_ctu_send_ctu_resp(p_pdb, 
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_INVALID_ADDRESS_RECEIVED));
        
        ICF_CTU_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_ENDED_NORMAL)
    } /* p_pdb->p_ctu_ctx->p_transferre... */
    else if (ICF_SUCCESS == icf_ctu_start_ctu_timer(p_pdb))
    {
        /*Check if CHR service is active*/
        if (ICF_SERVICE_CHR & 
            p_pdb->p_service_ctx->active_service_list.active_service_status)
        {
            /*If CHR service is active wait for its response*/
            ICF_CTU_SET_CURR_STATE(p_pdb,
                    ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP)

            /*If CHR is active for call hold as call is currently active*/
            if (ICF_CALL_STATUS_LOCAL_HELD ==
                    p_pdb->p_call_ctx->call_status)
            {
                ICF_CTU_SET_STATUS(p_pdb,
                        ICF_STATUS_SERVICE_CALL_RESUME_REQ)
            } /* if(ICF_CALL_STATUS_ACTIVE =... */
            else
            {
                /*CHR is active for call resume*/
                ICF_CTU_SET_STATUS(p_pdb, 
                        ICF_STATUS_SERVICE_CALL_HOLD_REQ)
            } /* else */
        } /* p_pdb->p_service_ctx->active_s... */
        else
        {
            /*Following function checks for signaling ongoing flag
             * and if not ongoing then send REFER request else
             * wait for start proc signaling*/
            ret_val = icf_ctu_check_sig_ong_n_send_refer(p_pdb);
        } /* if(ICF_SUCCESS == icf_ct... */
    }
    else
    {
        ret_val = ICF_FAILURE;
    } /* else */
    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_ctu_00_call_transfer_unattended_request */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_02_call_resume_resp
*
* DESCRIPTION   : Handler function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTU_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_02_call_resume_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t      ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_CALL_RESUME_RESP_RECVD)
    
    /*Funciton to send REFER request to SSA*/
    ret_val = icf_ctu_process_hold_resume_resp(p_pdb);
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_ctu_01_resume_resp */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_02_call_hold_resp
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTU_HOLD_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_02_call_hold_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t      ret_val = ICF_SUCCESS;
        
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
   
    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_CALL_HOLD_RESP_RECVD)
        
    /*Funciton to send REFER request to SSA*/
    ret_val = icf_ctu_process_hold_resume_resp(p_pdb);
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_refer_resp
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_REFER_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_03_refer_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_nw_inc_refer_resp_st  *p_refer_resp;

    /*Copy global pdb pointer*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Assign REFER response structure*/
    p_refer_resp = 
    (icf_nw_inc_refer_resp_st *)p_pdb->p_internal_msg->p_msg_data;

    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    /*Stop CTU timer*/
    if (ICF_FAILURE == p_refer_resp->status)
    {
        if (ICF_FAILURE == icf_ctu_stop_ctu_timer(p_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /*IF FAILURE RESPONSE IS RECEIVED*/
        else 
        {
            ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
                ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)

                ret_val = icf_ctu_send_ctu_resp(p_pdb, 
                        (icf_boolean_t)(ICF_FAILURE),
                        (icf_error_t)(ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ));
        }

        if (ICF_SUCCESS == ret_val)
        {
            /*If PA has initiated call clear*/
            if(ICF_TRUE == p_pdb->p_ctu_ctx->call_rel_initiated)
            {
                /*Mark PA Initiated call released in Call Context*/
                p_pdb->p_call_ctx->release_type = 
                    ICF_REL_TYPE_PA_INITIATED;
                ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
            }
            else
            {
                ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
            }
        }
        ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)

    } /* if(ICF_FAILURE == p_refer_r... */
        /*IF SUCCESS REFER RESPONSE RECVD*/
       else
    {
        /*BUFFER the call_terminate request further till recieving
         *NOTIFY */
#if 0
        /*If PA has initiated call clear*/
        if(ICF_TRUE == p_pdb->p_ctu_ctx->call_rel_initiated)
        {
            /*Mark PA Initiated call released in Call Context*/
            p_pdb->p_call_ctx->release_type = 
                ICF_REL_TYPE_PA_INITIATED;

            /*Return service ended error so that CM can initiate
             * call clear*/
            ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

            ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)
        }
        else
#endif
        {
            ICF_CTU_SET_CURR_STATE(p_pdb,
                    ICF_CTU_04_WAIT_FOR_NOTIFY)            
        }  /* else */
    } /* if(ICF_SUCCESS == icf_ct... */

    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val; 
} /* icf_ctu_03_refer_resp */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_01_start_proc_sig
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_01_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTU_START_PROC_SIG
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_01_start_proc_sig(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t      ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*Send REFER response to SSA*/
    ret_val = icf_ctu_send_refer_req(p_pdb);
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_ctu_01_start_proc_sig */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_cc_call_released
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_01_WAIT_FOR_START_PROC_SIG
*                               ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF_CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_xx_cc_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_FAILURE;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    

	p_glb_pdb = p_glb_pdb; 

	
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_CC_INITIATED_CALL_REL)

    /*Stop CTU timer*/
    if(ICF_SUCCESS == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
        ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)

        if (ICF_REL_TYPE_MM_INITIATED == p_glb_pdb->p_call_ctx->release_type)
        {
           /*Send failure CTU response to PA*/
           ret_val = icf_ctu_send_ctu_resp(p_pdb, 
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_MEDIA_FAILURE));
        }
        else
        {
           /*Send failure CTU response to PA*/
           ret_val = icf_ctu_send_ctu_resp(p_pdb, 
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ));
        }
    } /* if(ICF_SUCCESS == icf_ct... */

    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_ctu_abort_refer_trans(p_pdb);
    } /* if (ICF_SUCCESS == ret_val) */    

    ICF_CTU_SET_STATUS(p_pdb, 
               ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_CTU_SET_STATUS(p_pdb, 
               ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_ctu_xx_cc_call_released */


/******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_pa_call_released
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_01_WAIT_FOR_START_PROC_SIG
*                               ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF_CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_xx_pa_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_PA_INITIATED_CALL_REL)
    
    /*Mark That PA has initiated Call released*/
    p_pdb->p_ctu_ctx->call_rel_initiated = ICF_TRUE;
    
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_ctu_xx_pa_call_released */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_timer_expiry
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF _CTU_02_WAIT_FOR_START_PROC_SIG
                                ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_xx_timer_expiry(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    

	p_glb_pdb = p_glb_pdb; 

	
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STAT(p_glb_pdb, ICF_STAT_CTU_TMR_EXPIRY)
    
    ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
    
    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)  
        
    /*Send failure CTU response to PA*/
    ret_val = icf_ctu_send_ctu_resp(p_pdb, 
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));

    /*If PA has initiated call clear*/
    if(ICF_TRUE == p_pdb->p_ctu_ctx->call_rel_initiated)
    {
        /*Mark PA Initiated call released in Call Context*/
        p_pdb->p_call_ctx->release_type =
                ICF_REL_TYPE_PA_INITIATED;

        ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    }
    else
    {
        ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    }
    
    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)
     
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_xx_timer_expiry */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_timer_expiry
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_03_timer_expiry(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    

	p_glb_pdb = p_glb_pdb;   

	
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_STAT(p_glb_pdb, ICF_STAT_CTU_TMR_EXPIRY)
    
    ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)
    
    /*Send failure CTU response to PA*/
    ret_val = icf_ctu_send_ctu_resp(p_pdb, 
            (icf_boolean_t)(ICF_FAILURE),
            (icf_error_t)(ICF_CAUSE_TIMER_EXPIRY));

    if(ICF_SUCCESS == ret_val)
    {
        /*Send message to SSA to clear the REFER transaction*/
        ret_val = icf_ctu_abort_refer_trans(p_pdb);
    } /* if(ICF_SUCCESS == ret_val) */        

    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_03_timer_expiry */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_unexpected_event(
        INOUT   icf_ctu_pdb_st   *p_pdb)
{
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*To remove warning*/
    p_pdb = p_pdb;
    
    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    return ICF_SUCCESS;
} /* icf_ctu_unexpected_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_xx_forceful_call_clear(
        INOUT  icf_ctu_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Send service ended error */
    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
    if (ICF_FAILURE == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }

    if (ICF_FAILURE == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_xx_forceful_call_clear */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_03_forceful_call_clear(
        INOUT  icf_ctu_pdb_st    *p_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Send service ended error */
    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

    /*Send message to SSA to clear the REFER transaction*/
    if (ICF_FAILURE == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*Send message to SSA to clear the REFER transaction*/
        ret_val = icf_ctu_abort_refer_trans(p_pdb);
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_03_forceful_call_clear */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_04_notify
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_NOTIFY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_04_notify(
        INOUT   icf_ctu_pdb_st   *p_pdb)
{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_nw_inc_notify_ind_st  *p_notify = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_STAT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
             ICF_STAT_NW_CC_NOTIFY)

    /*Assign NOTIFY structure*/
    p_notify = (icf_nw_inc_notify_ind_st *)p_pdb->p_internal_msg->p_msg_data;
  
    /*Send final response for NOTIFY*/
    if (ICF_FAILURE == icf_ctu_send_notify_resp(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*Stop CTU timer*/
    else if (ICF_FAILURE == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*If notify is for successful call transfer*/
    else if (ICF_SUCCESS == p_notify->status)
    {
        if(ICF_TRUE == p_pdb->p_ctu_ctx->call_rel_initiated)
        {
            /*Mark PA Initiated call released in Call Context*/
            p_pdb->p_call_ctx->release_type = 
                ICF_REL_TYPE_PA_INITIATED;
            ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        }
        else
        {
            ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }

        ICF_STAT(p_glb_pdb, ICF_STAT_SUCC_CTU)
        ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)

        icf_ctu_send_ctu_resp(p_pdb, 
            (icf_boolean_t)ICF_SUCCESS,
            (icf_error_t)(ICF_ERROR_UNDEFINED));
    }
    else
    {
        /*If PA has initiated call clear*/
        if(ICF_TRUE == p_pdb->p_ctu_ctx->call_rel_initiated)
        {
            /*Mark PA Initiated call released in Call Context*/
      	    p_pdb->p_call_ctx->release_type = 
           	    ICF_REL_TYPE_PA_INITIATED;

            ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        }
        else
        {
            ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }

        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
        ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)

        /*Failure response to PA that new call could not be established*/
        /*Send failure CTu response to PA*/
        ret_val = icf_ctu_send_ctu_resp(p_pdb,
                (icf_boolean_t)(ICF_FAILURE),
                (icf_error_t)(ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ));
    }

    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_ctu_04_notify */

/*******************************************************************************
* FUNCTION NAME : icf_ctu_04_cc_call_released
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_CC_CALL_RELEASED
* RETURNS       : icf_return_t
*******************************************************************************/

icf_return_t icf_ctu_04_cc_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb
        )
{
    icf_return_t ret_val = ICF_FAILURE;
    icf_glb_pdb_st  *p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_glb_pdb = p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_CC_INITIATED_CALL_REL)

    /*Stop CTU timer*/
    if(ICF_SUCCESS == icf_ctu_stop_ctu_timer(p_pdb))
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTU)
        ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTU_RESP)

        /*Send  Success CTU response to PA*/
        ret_val = icf_ctu_send_ctu_resp(p_pdb,
                (icf_boolean_t)(ICF_SUCCESS),
                (icf_error_t)(ICF_ERROR_UNDEFINED));
    } /* if(ICF_SUCCESS == icf_ct... */

    ICF_CTU_SET_STATUS(p_pdb,
               ICF_STATUS_SERVICE_ENDED_NORMAL)

    ICF_CTU_SET_STATUS(p_pdb,
               ICF_STATUS_SIGNAL_ONGOING_ENDED)

    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_04_cc_call_released */
#endif
