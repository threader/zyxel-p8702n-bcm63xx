/*******************************************************************************
*
* FILE NAME    : icf_cta_util.c
*
* DESCRIPTION  : Contains the definitions of all utility functions
*                of CTA service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 19-Feb-2005     Amit Sharma     None            Initial
* 08-Mar-2005    Amit Sharma     None            Incorporated review comments
* 06-May-2005    Amit Sharma     SPR:7514        Updation in return path payload
*                                                call context return
* 10-May-2005    Amit Sharma     SPR:7522        Handling for referred_by 
*                                                header
* 11-May-2005    Amit Sharma     SPR:7522        Updated handling for 
*                                                referred_by line_data block is
*                                                fetch
* 02-Jun-2005    Amit Sharma     SPR:7535        Wait for TWC service term
* 08-Nov-2005    Amit Sharma   SPR:8129
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
* 24-Aug-2007  Amit Sharma                     IPTK merge from PQT6.5
* 8-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging
* 24-Jun-2008  Anurag Khare    SPR 18684       CSR 1-6045445 Merge 
* 01-Apr-2009  Kamal Ashraf    Rel 8.2         Klocwork Fix
* 22-May-2009  Ashutosh Mohan  CSR-1-6743474   Modified icf_cta_send_refer_req
* 10-Jun-2009 Abhishek Dhammawat SPR 19590     Modified icf_cta_send_cta_resp
* 21-Jun-2009 Abhishek Dhammawat SPR 19590     Modified
*                                              icf_cta_clone_app_spec_hdr
*
* Copyright 2009,Aricent .
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cta_types.h"
#include "icf_cta_prototypes.h"
#include "icf_cta_macro.h"
#include "icf_cta_defs.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_send_refer_req
*
* DESCRIPTION   : Function is used to send REFER request to SSA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t     icf_cta_send_refer_req(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_nw_otg_refer_req_st   refer_req;
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_line_data_st          *p_line_data = ICF_NULL;
    icf_error_t               *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    refer_req.bitmask = ICF_NULL;

    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_REFER_REQ_SENT)

    /* Set signaling ongoing in CTA service */
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)

    ICF_CTA_SET_CURR_STATE(p_pdb, ICF_CTA_03_WAIT_FOR_REFER_RESP)

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_NW_REFER)

    /*Assign message id for SSA*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REFER;

    /*Assign payload length*/
    p_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(icf_nw_otg_refer_st);

    refer_req.p_sic_ctx = ICF_NULL;

    /*REFER will be sent with REPLACES header*/
    refer_req.bitmask = ICF_NW_OTG_REFER_RELACE_CALL_ID;

    /*If transferred call id is same as call in p_glb_pdb
     * then set call-id of party-C, as current transferred call-id is
     * of call B*/
    if (p_glb_pdb->p_call_ctx->call_id == p_pdb->p_cta_ctx->transferred_id)
    {
        /* CSR-1-6743473: Glb pdb should contain the call ctx on which 
         * REFER needs to be sent */
        p_glb_pdb->p_call_ctx = p_pdb->p_cta_ctx->p_call_ctx_c; 
    }

    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_glb_pdb, 
                p_pdb->p_cta_ctx->p_call_ctx_c->line_id, 
                (icf_line_data_st **)&p_line_data,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        if ((ICF_TOGGLE_PI & p_glb_pdb->p_call_ctx->common_bitmask) ^
                (ICF_CFG_CC_USER_PRESENTATION_IND &
                 p_line_data->default_settings))
        {
            /*Update Local party name and address in call context*/
            if(ICF_SUCCESS == icf_dbm_fill_local_user_info(p_glb_pdb))
            {
                /*If local user name and address filled is successful then
                 *send REFER by referred by header else send REFER without
                 * referred by*/
                refer_req.bitmask |= ICF_NW_OTG_REFER_REF_BY;
            } /* if(ICF_SUCCESS == icf_db... */
        } /* p_glb_pdb->p_recv_msg->payload... */

        /*Copy transferred party address*/
        refer_req.p_referred_to = 
            &(p_pdb->p_cta_ctx->p_call_ctx_b->remote_party_addr);

        /*Copy REPLACES call_id*/
        refer_req.replace_call_id = p_pdb->p_cta_ctx->transferred_id;

        /*Assign refer_req to p_msg_data of internal message*/
        p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&refer_req;

        /*Call entry point function of SSA*/
        ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);
/*
        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cta_apply_both_calls(p_pdb);
        }*/ /* if (ICF_SUCCESS == ret_val) */
    } /* else */
#ifdef ICF_LOAD_STAT
    p_glb_pdb->load_data.otg_cta_attempts++;
#endif    

    ICF_FUNCTION_EXIT(p_glb_pdb)

        return ret_val;
} /* icf_cta_send_refer_req */


/*******************************************************************************
 *
 * FUNCTION NAME : icf_cta_check_chr_service
 *
 * DESCRIPTION   : Function is used to check CHR service whether it is active
 *                 or not
 *
 * RETURNS       : icf_return_t
 *
*******************************************************************************/
icf_return_t     icf_cta_check_chr_service(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_service_ctx_st     *p_service_b = ICF_NULL;
    icf_service_ctx_st     *p_service_c = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_service_b = p_pdb->p_cta_ctx->p_call_ctx_b->p_service_ctx;

    p_service_c = p_pdb->p_cta_ctx->p_call_ctx_c->p_service_ctx;

    /*If CHR service is active in call B*/
    if ((ICF_FALSE == p_pdb->p_cta_ctx->chr_for_call_b) && 
            (ICF_SERVICE_CHR & 
            p_service_b->active_service_list.active_service_status))
    {
        /*If CHR service is active wait for its response*/
        ICF_CTA_SET_CURR_STATE(p_pdb,
                ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP)

        /*If CHR is active for call hold as call is currently active*/
        if (ICF_CALL_STATUS_LOCAL_HELD ==
                p_pdb->p_cta_ctx->p_call_ctx_b->call_status)
        {
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_CALL_RESUME_REQ)
        }
        else
        {
            /*CHR is active for call resume*/
            ICF_CTA_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_HOLD_REQ)
        }
        
        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_cta_ctx->p_call_ctx_b->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_b);
        }

        p_pdb->p_cta_ctx->chr_for_call_b = ICF_TRUE;
        
    } /* p_service_b->active_service_li... */
    /*If CHR service is active in call C*/
    else if ((ICF_FALSE == p_pdb->p_cta_ctx->chr_for_call_c) && 
            (ICF_SERVICE_CHR & 
            p_service_c->active_service_list.active_service_status))
    {
        /*If CHR service is active wait for its response*/
        ICF_CTA_SET_CURR_STATE(p_pdb,
                ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP)

        /*If CHR is active for call hold as call is currently active*/
        if (ICF_CALL_STATUS_LOCAL_HELD ==
                p_pdb->p_cta_ctx->p_call_ctx_c->call_status)
        {
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_CALL_RESUME_REQ)
        }
        else
        {
            /*CHR is active for call resume*/
            ICF_CTA_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_CALL_HOLD_REQ)
        }

        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_cta_ctx->p_call_ctx_c->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_c);
        }
        p_pdb->p_cta_ctx->chr_for_call_c = ICF_TRUE;
    }
    else
    {
        /*Following function checks for signaling ongoing flag
         * and if not ongoing then send REFER request else
         * wait for start proc signaling*/
        
        ret_val = icf_cta_check_sig_ong_n_send_refer(p_pdb);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;        
} /* icf_cta_check_chr_service */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_check_sig_ong_n_send_refer
*
* DESCRIPTION   : Function is used to check if signaling ongoing in any other
*                 module otherwise send REFER with REPLACES header to SSA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_cta_check_sig_ong_n_send_refer(
        INOUT  icf_cta_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)


    /*If signaling ongoing in any other module*/
    /*  CSR 1-6045445 Merge */
    /*Now not only signaling ongoing is to be look for
    It must also be checked that call is established*/
    if ((ICF_CALL_CONNECT_DONE & p_pdb->p_cta_ctx->p_call_ctx_b->common_bitmask) &&
        (ICF_INT_MODULE_INVALID != 
                        ICF_CTA_GET_SIG_ONGOING(p_pdb->p_cta_ctx->p_call_ctx_b)))
    {
        /*
         * Signaling Ongoing in CALL CONTEXT-C so wait for start proc signaling
         * and if this call-C is not same as the call in p_glb_pdb set
         * status as other call request*/

        ICF_CTA_SET_CURR_STATE(p_pdb, ICF_CTA_02_WAIT_FOR_START_PROC_SIG)

        /*Set status for start proc signaling*/        
        ICF_CTA_SET_STATUS(p_pdb,
                ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
       
        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_cta_ctx->p_call_ctx_b->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_cta_ctx->p_call_ctx_b);
        }
    } /* else if(ICF_MODULE_ID_INVAL... */
    /* CSR 1-6045445 Merge */
    /* In case the second call has not been fully established and 18x has been 
     * received for INVITE, then in this case REFER should be sent on network */
    else if (ICF_INT_MODULE_INVALID != 
             ICF_CTA_GET_SIG_ONGOING(p_pdb->p_cta_ctx->p_call_ctx_c))
    {
        /*
         * Signaling Ongoing in CALL CONTEXT-C so wait for start proc signaling
         * and if this call-C is not same as the call in p_glb_pdb set
         * status as other call request*/

        ICF_CTA_SET_CURR_STATE(p_pdb, ICF_CTA_02_WAIT_FOR_START_PROC_SIG)

        /*Set status for start proc signaling*/        
        ICF_CTA_SET_STATUS(p_pdb,
                ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)

        if (p_glb_pdb->p_call_ctx->call_id != 
                p_pdb->p_cta_ctx->p_call_ctx_c->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_c);
        }
    }     
    else
    {
        /*Else send REFER request*/
        ret_val = icf_cta_send_refer_req(p_pdb);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_check_sig_ong_n_send_refer */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_set_both_call_ctx
*
* DESCRIPTION   : Function is used to set both call context in cta_ctx
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t  icf_cta_set_both_call_ctx(
        INOUT  icf_cta_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;
    icf_error_t  *p_ecode = ICF_NULL;

    /*Copy p_glb_pdb from internal msg to local ptr*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*If call id received in Call Transfer Attended request is Invalid*/
    if (ICF_INVALID_CALL_ID == p_pdb->p_cta_ctx->transferred_id)
    {
        /*Send Failure Call Transfer response to PA*/
         ret_val = icf_cta_send_cta_resp(p_pdb,
        (icf_boolean_t)ICF_FAILURE,
        (icf_error_t)ICF_CAUSE_INVALID_CALL_ID);

        /*Send service ended normal to both the calls*/
        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
    } /* if (ICF_INVALID_CALL_ID == ... */
    /*Transferred call ID is valid then fetch its call ctx pointer
     * thru DBM function*/
    else if (ICF_FAILURE == 
            icf_dbm_find_call_cntxt_blk(p_glb_pdb, 
                p_pdb->p_cta_ctx->transferred_id,
                (icf_call_ctx_st **)&p_pdb->p_cta_ctx->p_call_ctx_b, 
                p_ecode))
    {
        /*If call context not found for given call id, send failure CTA
         * response to PA and service is terminated notmal, else if
         * failure due to any other reason return failure*/
        if (ICF_NO_CALL_CTX_EXIST == *p_ecode)
        {
            /*Send Failure Call Transfer response to PA*/
            ret_val = icf_cta_send_cta_resp(p_pdb,
                    (icf_boolean_t)ICF_FAILURE,
                    (icf_error_t)ICF_CAUSE_INVALID_CALL_ID);

            /*Send service ended normal to both the calls*/
            ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        /*Call CTX-C will be the call in p_glb_pdb*/
        p_pdb->p_cta_ctx->p_call_ctx_c = p_glb_pdb->p_call_ctx;

        ICF_CTA_TRACE(p_pdb, ICF_TRACE_CTA_RECVD)
    }
     
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_set_both_call_ctx */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_start_cta_timer
*
* DESCRIPTION   : Function is used to allocate memory for CTA timer data and
*                 start CTA timer
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_start_cta_timer(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_config_data_st   *p_config_data = ICF_NULL;
    icf_error_t          *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_FAILURE ==
            icf_dbm_get_module_glb_data(p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_CFG),
                (icf_void_t *)&p_config_data, p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /*Call DBM to allocate timer buffer value*/
    else if(ICF_FAILURE ==
            icf_dbm_get_mem_from_pool(p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t **)&(p_pdb->p_cta_ctx->p_cta_timer_data),
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        
        p_pdb->p_cta_ctx->p_cta_timer_data->timer_type = ICF_CTA_TIMER;
        p_pdb->p_cta_ctx->p_cta_timer_data->module_id = ICF_INT_MODULE_CTA;
        /*
         * Store call context pointer in p_timer buffer
         * so that when timer expiry receives module can
         * identify the call data
         */
        p_pdb->p_cta_ctx->p_cta_timer_data->p_timer_buffer =
            (icf_void_t *)p_glb_pdb->p_call_ctx;
        p_pdb->p_cta_ctx->p_cta_timer_data->p_glb_pdb = p_glb_pdb;

        /*Start ICF_CTA_TIMER */
        ICF_START_TIMER(p_glb_pdb, p_pdb->p_cta_ctx->p_cta_timer_data,
                p_pdb->p_cta_ctx->cta_timer_id,
                p_config_data->cta_timer,
                p_ecode, ret_val)

        ICF_STAT(p_glb_pdb, ICF_STAT_CTA_TMR_STARTED)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_cta_start_cta_timer */

/******************************************************************************
*
* FUNCTION NAME : icf_cta_stop_cta_timer
*
* DESCRIPTION   : Function is used to de-allocate memory of CTA timer data and
*                 stop CTA timer
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_return_t    icf_cta_stop_cta_timer(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_error_t      *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_cta_ctx->cta_timer_id, p_ecode, ret_val)

    ICF_STAT(p_glb_pdb, ICF_STAT_CTA_TMR_STOPPED)

    p_pdb->p_cta_ctx->cta_timer_id = 0;
    
    if(ICF_NULL != p_pdb->p_cta_ctx->p_cta_timer_data)
    {
        if(ICF_FAILURE == ret_val)
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_cta_ctx->p_cta_timer_data),
                    p_ecode);

            p_pdb->p_cta_ctx->p_cta_timer_data = ICF_NULL;

            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == ret_val) */
        else
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_cta_ctx->p_cta_timer_data),
                    p_ecode);

            p_pdb->p_cta_ctx->p_cta_timer_data = ICF_NULL;
        } /* else */
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
} /* icf_cta_stop_cta_timer */


/*****************************************************************************
*
* FUNCTION NAME:   icf_cta_fill_other_call_info
*
* DESCRIPTION  : This function is used to allocate return payload and
*                set call ctx pointer in it
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_cta_fill_other_call_info(
        INOUT   icf_internal_msg_st   *p_internal_msg,
        INOUT   icf_call_ctx_st       *p_call_ctx)
{
    icf_service_call_data_st   *p_msg_data = ICF_NULL;
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_glb_pdb_st             *p_glb_pdb = ICF_NULL;
    icf_error_t                *p_ecode = ICF_NULL;

    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Allocate memory for payload structure*/
    ICF_MEMGET(p_glb_pdb, sizeof(icf_service_call_data_st),
            ICF_MEM_COMMON, p_msg_data,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Assign call context pointer*/
    p_msg_data->p_other_call_ctx = p_call_ctx;

    /*Fill payload length*/
    p_internal_msg->msg_hdr.payload_length = 
        sizeof(icf_service_call_data_st);

    /*Assign p_msg_data into p_msg_data in internal message*/
    p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_fill_other_call_info */


/*****************************************************************************
*
* FUNCTION NAME:   icf_cta_apply_both_calls
*
* DESCRIPTION  : This function is used to fill status as both calls and also
*                fills the return path payload
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_cta_apply_both_calls(
        INOUT   icf_cta_pdb_st   *p_pdb
        )
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

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
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_cta_apply_both_calls */

    
/*****************************************************************************
*
* FUNCTION NAME:   icf_cta_process_call_rel_flags
*
* DESCRIPTION  : This function is used to process the call release flags
*
*
* RETURNS:  
*
******************************************************************************/
icf_void_t  icf_cta_process_call_rel_flags(
        INOUT   icf_cta_pdb_st   *p_pdb
        )
{

#ifdef ICF_TRACE_ENABLE
    icf_return_t    ret_val = ICF_SUCCESS;
#endif

    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    /*If PA has already initiated call clear then set release type
     *as PA initiated in both call context*/
    if (ICF_TRUE == p_pdb->p_cta_ctx->call_rel_initiated)
    {
        /*If call B is not already cleared*/
        if (ICF_TRUE != p_pdb->p_cta_ctx->call_clear_initiator_b)
        {
            /*Mark PA Initiated call released in Call Context B*/
            p_pdb->p_cta_ctx->p_call_ctx_b->release_type = 
                ICF_REL_TYPE_PA_INITIATED;
            
            p_pdb->p_cta_ctx->p_call_ctx_b->release_cause = 
                p_pdb->p_cta_ctx->call_clear_reason;
        }

        /*If call C is not already cleared*/
        if (ICF_TRUE != p_pdb->p_cta_ctx->call_clear_initiator_c)
        {
            /*Mark PA Initiated call released in Call Context B*/
            p_pdb->p_cta_ctx->p_call_ctx_c->release_type = 
                ICF_REL_TYPE_PA_INITIATED;
            
            p_pdb->p_cta_ctx->p_call_ctx_b->release_cause = 
                p_pdb->p_cta_ctx->call_clear_reason;
        }
    }
    else
    {
        /*IF PA has not initiated call clear then mark release type as internal
         *initiated and cause as call transferred*/
        if (ICF_TRUE != p_pdb->p_cta_ctx->call_clear_initiator_b)
        {
            p_pdb->p_cta_ctx->p_call_ctx_b->release_type =
                ICF_REL_TYPE_INTERNAL_INITIATED;

            p_pdb->p_cta_ctx->p_call_ctx_b->release_cause =
                ICF_CALL_CLEAR_REASON_CALL_TRANSFERRED;
        }

        if (ICF_TRUE != p_pdb->p_cta_ctx->call_clear_initiator_c)
        {
            p_pdb->p_cta_ctx->p_call_ctx_c->release_type =
                ICF_REL_TYPE_INTERNAL_INITIATED;

            p_pdb->p_cta_ctx->p_call_ctx_c->release_cause =
                ICF_CALL_CLEAR_REASON_CALL_TRANSFERRED;
        }
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_cta_process_call_rel_flags */


/******************************************************************************
*
* FUNCTION NAME:   icf_cta_abort_refer_trans
*
* DESCRIPTION  : This function is used to send REFER ABORT request to SSA
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_cta_abort_refer_trans(
        INOUT   icf_cta_pdb_st    *p_pdb)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_internal_msg_st *p_internal_msg = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
   
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Code to send REFER ABORT request to SSA*/
    p_internal_msg = p_pdb->p_internal_msg;

    p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REFER_ABORT_REQ;

    p_internal_msg->msg_hdr.payload_length = 0;

    ret_val = icf_ssa_process_mesg(p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_abort_refer_trans */


/******************************************************************************
*
* FUNCTION NAME : icf_cta_send_cta_resp
*
* DESCRIPTION   : Function is used to CTA response to PA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_send_cta_resp(
        INOUT  icf_cta_pdb_st   *p_pdb,
        IN     icf_boolean_t    result,
        IN     icf_error_t      error_cause)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          *p_msg = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_uint8_t         api_len = 0;
    icf_cta_resp_st     *p_cta_resp = ICF_NULL;
    icf_list_st         *p_temp_list_node = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /* Changes For XCONF */	
    if(ICF_TRUE == p_pdb->p_cta_ctx->internal_refer_req)
    {
    	if(ICF_SUCCESS == result)
    	{
	    	ICF_CTA_SET_STATUS_2(p_pdb,
                ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP);
	    }
	    else
	    {
		    ICF_CTA_SET_STATUS_2(p_pdb,
                ICF_STATUS_INTERNAL_REFER_FAILURE_RESP);
	    }
	    p_pdb->p_internal_msg->msg_hdr.ecode = error_cause;
    }
    /*  Changes For XCONF End  */
    else
    {

        api_len = sizeof(icf_api_header_st) + sizeof(icf_cta_resp_st);

        /*Update Stats*/
        if (ICF_SUCCESS == result)
        {
            ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTA_RESP)
            ICF_STAT(p_glb_pdb, ICF_STAT_SUCC_CTA)
#ifdef ICF_LOAD_STAT
            p_glb_pdb->load_data.otg_cta_success++;
#endif                        
    	}
    	else
    	{
            ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CTA_RESP)
            ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CTA)
    	}

	#ifdef ICF_TRACE_ENABLE
    	/* Added to send traces */
    	p_pdb->result=result;
    	p_pdb->error_cause=error_cause;
    	ICF_CTA_APP_TRACE(p_pdb, ICF_TRACE_CTA_RESP)
	#endif
    	/*Allocate memory for API header strucure */
    	ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON,
        	    p_msg, ICF_RET_ON_FAILURE,
            	p_ecode, ret_val)

    	/*Fill CTA response message to send to PA*/
    	icf_cmn_fill_resp_msg(p_glb_pdb, p_msg,
            (icf_module_id_t)(ICF_MODULE_PA),
            (icf_api_id_t)(ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP),
            result, error_cause);

    	p_msg->hdr.api_length = api_len;

    	p_cta_resp = (icf_cta_resp_st*)&(p_msg->payload[0]);

        if (0 != p_glb_pdb->p_call_ctx->retry_after_duration)
        {
            p_cta_resp->retry_after_duration =
                p_glb_pdb->p_call_ctx->retry_after_duration;
            p_glb_pdb->p_call_ctx->retry_after_duration = 0;
        }

    	/* clone the header list */
        /*SPR 19590 change start*/
        ret_val = icf_cmn_fetch_curr_hdr_list_node_from_call_ctx(
                         p_glb_pdb,
                         &p_temp_list_node);
        /*SPR 19590 change end*/
    	if((ICF_NULL != p_temp_list_node) &&
           (ICF_NULL != p_temp_list_node->p_data) &&     
           (ICF_NULL != ((icf_header_link_list_st*)(p_temp_list_node->p_data))
                    ->header_list.count))
    	{
       	     if(ICF_SUCCESS == icf_cmn_clone_header_list(
            	p_glb_pdb,
         &(((icf_header_link_list_st*)(p_temp_list_node->p_data))->header_list),
            	&(p_cta_resp->header_list),
            	p_ecode))
        	{
            	    p_cta_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
            
                    /* free the list in call ctx */
                    /*SPR 19590 change start*/
                    ret_val = icf_cmn_del_header_list_node_in_call_ctx(
                               p_glb_pdb,
                               p_temp_list_node);
                    /*SPR 19590 change end*/
        	}
        	else
        	{
            	    ret_val = ICF_FAILURE;
        	}
    	}
    	else
	{
	        p_cta_resp->header_list.hdr = ICF_NULL;
        	p_cta_resp->header_list.count = 0;
    	}
        
        /*SPR 19835 reset the txn_id in call ctx as the node is deleted for
         * the header/body list.*/
        ICF_RESET_TXN_ID_IN_CALL_CTX(p_glb_pdb)

    	/*Assign p_msg to p_msg_data of icf-internal_msg_st*/

    	if (ICF_SUCCESS == ret_val)
    	{
       	    p_pdb->p_internal_msg->msg_hdr.msg_id =
            ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP;
            /* Fix for CSR1-5249829 */
            p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_call_ctx->call_id;
        
            p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;
        
            p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;
        
            /*Call icf_adm_process_mesg(p_internal_msg)*/
            if (ICF_FAILURE ==
                icf_adm_process_mesg(p_pdb->p_internal_msg))
            {
                /* free the list in call ctx */
                ret_val = icf_cmn_free_header_list(p_glb_pdb,
                    &(p_cta_resp->header_list),
                    p_ecode);

                   ICF_MEMFREE(p_glb_pdb, p_msg,
                   ICF_MEM_COMMON, p_ecode, ret_val)
            
                   ret_val = ICF_FAILURE;
            }
            else
            {
                /* free the list in call ctx */
                ret_val = icf_cmn_free_header_list(p_glb_pdb,
                &(p_cta_resp->header_list),
                p_ecode);

                ICF_MEMFREE(p_glb_pdb, p_msg,
                ICF_MEM_COMMON, p_ecode, ret_val)
            }
        }
        else
        {
            ICF_MEMFREE(p_glb_pdb, 
                        p_msg,
                        ICF_MEM_COMMON, 
                        p_ecode, 
                        ret_val)
            ret_val = ICF_FAILURE;
        }
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_send_cta_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_send_notify_resp
*
* DESCRIPTION   : Function is used to send response for NOTIFY received to SSA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t     icf_cta_send_notify_resp(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_return_t              ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTA_TRACE(p_pdb, ICF_CTA_TRACE_NOTIFY_RESP_SENT)

    /*Assign message id for SSA*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_NOTIFY_RESP;

    /*Call entry point function of SSA*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_send_notify_resp */


/****************************************************************************
*
* FUNCTION NAME:   icf_cta_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize CTA context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_cta_deinit_ctx(
        INOUT icf_cta_pdb_st  *p_pdb
        )
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t   *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
  
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Set state as ICF_CTA_00_IDLE*/
    p_pdb->p_cta_ctx->state = ICF_CTA_00_IDLE;

    /*If timer ID is not 0 then stop timer*/
    if(0 != p_pdb->p_cta_ctx->cta_timer_id)
    {
        ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_cta_ctx->cta_timer_id, p_ecode, ret_val)

        p_pdb->p_cta_ctx->cta_timer_id = ICF_NULL;
    }
    
    /*If timer data has not been freed free it*/
    if(ICF_NULL != p_pdb->p_cta_ctx->p_cta_timer_data)
    {
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_cta_ctx->p_cta_timer_data),
                p_ecode);
        p_pdb->p_cta_ctx->p_cta_timer_data = ICF_NULL;
    }

    /*Call context NULL*/
    /* p_pdb->p_cta_ctx->p_call_ctx_b = ICF_NULL;
    p_pdb->p_cta_ctx->p_call_ctx_c = ICF_NULL;*/
    p_pdb->p_cta_ctx->transferred_id = ICF_INVALID_CALL_ID;
    
    p_pdb->p_cta_ctx->call_rel_initiated = ICF_NULL;
    p_pdb->p_cta_ctx->call_clear_initiator_b = ICF_NULL;
    p_pdb->p_cta_ctx->call_clear_initiator_c = ICF_NULL;

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
} /* icf_cta_deinit_ctx */

/*******************************************************************************
 *
 * FUNCTION NAME : icf_cta_check_twc_service
 *
 * DESCRIPTION   : Function is used to check TWC service whether it is active
 *                 or not
 *
 * RETURNS       : icf_return_t
 *
*******************************************************************************/
icf_return_t     icf_cta_check_twc_service(
        INOUT icf_cta_pdb_st     *p_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_service_ctx_st     *p_service_b = ICF_NULL;
    icf_service_ctx_st     *p_service_c = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

        p_service_b = p_pdb->p_cta_ctx->p_call_ctx_b->p_service_ctx;

    p_service_c = p_pdb->p_cta_ctx->p_call_ctx_c->p_service_ctx;

    /*If TWC service is active in call B*/
    if (ICF_SERVICE_TWC &
            p_service_b->active_service_list.active_service_status)
    {
        /*If TWC service is active wait for its response*/
        ICF_CTA_SET_CURR_STATE(p_pdb,
                ICF_CTA_05_WAIT_FOR_TWC_RESP)

        ICF_CTA_SET_STATUS(p_pdb,
                ICF_STATUS_SERVICE_AWT_TWC_RESP)

        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_cta_ctx->p_call_ctx_b->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_b);
        }

    } /* p_service_b->active_service_li... */
    /*If TWC service is active in call C*/
    else if (ICF_SERVICE_TWC &
            p_service_c->active_service_list.active_service_status)
    {
        /*If TWC service is active wait for its response*/
        ICF_CTA_SET_CURR_STATE(p_pdb,
                ICF_CTA_05_WAIT_FOR_TWC_RESP)

        ICF_CTA_SET_STATUS(p_pdb,
                ICF_STATUS_SERVICE_AWT_TWC_RESP)

        if (p_glb_pdb->p_call_ctx->call_id !=
                p_pdb->p_cta_ctx->p_call_ctx_c->call_id)
        {
            /*Set status for OTHER CALL REQ*/
            ICF_CTA_SET_STATUS(p_pdb,
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)

            ret_val = icf_cta_fill_other_call_info(p_pdb->p_internal_msg,
                p_pdb->p_cta_ctx->p_call_ctx_c);
        }
    }
    else
    {
        /*Check if CHR service is active*/
        ret_val = icf_cta_check_chr_service(p_pdb);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;       
} /* icf_cta_check_twc_service */


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME:   icf_cta_trace
*
* DESCRIPTION  : This function is used to define the trace data for CTA service
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_void_t    icf_cta_trace(
        INOUT icf_cta_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    switch(trace_id)
    {
        case ICF_TRACE_CTA_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_cta_ctx->p_call_ctx_c->call_id;

            /* Transferred Party Call ID */
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_cta_ctx->p_call_ctx_b->call_id;
            break;

        case ICF_CTA_TRACE_CALL_RESUME_RESP_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_CTA_TRACE_CALL_HOLD_RESP_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_CTA_TRACE_REFER_REQ_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_cta_ctx->p_call_ctx_b->call_id;

            break;

         case ICF_CTA_TRACE_NOTIFY_RESP_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_cta_ctx->p_call_ctx_b->call_id;

            break;

        case ICF_CTA_TRACE_PA_INITIATED_CALL_REL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_CTA_TRACE_CC_INITIATED_CALL_REL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            break;

        case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;

        case ICF_TRACE_CTA_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_cta_ctx->state;
            icf_cta_state_trace_string(p_pdb->p_cta_ctx->state, &trace_data);
            break;
            
        case ICF_TRACE_CTA_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_cta_ctx->event;
            trace_data.int_val[1] = 
                (icf_uint32_t)p_glb_pdb->p_call_ctx->call_id;
            icf_cta_event_trace_string(p_pdb->p_cta_ctx->event, &trace_data);
            break;
        
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */       

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data)
} /* icf_cta_trace */

/******************************************************************************
*
* FUNCTION NAME:   icf_cta_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for CTA service
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_cta_app_trace(
        INOUT icf_cta_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_glb_pdb_st         *p_glb_pdb = ICF_NULL;
    icf_trace_criteria_data_st    *p_criteria_data = p_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
    /* Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */ 

    ret_val = icf_cmn_check_app_trace(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
        return ret_val;
    }
    else
    {

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;


    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;

    switch(trace_id)
    {
        case ICF_TRACE_CTA_REQ:

            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
  
            trace_data.int_val[0] =p_pdb->p_cta_ctx->transferred_id;
    
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

            /*icf_port_strcpy(trace_data.string_val[0].str,
                                                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->remote_party_addr.addr_val.str);*/
	      icf_port_strcpy(trace_data.string_val[0].str,
			                        p_pdb->p_cta_ctx->p_call_ctx_b->remote_party_addr.addr_val.str);

            trace_data.string_val[0].str_len =(icf_uint16_t)
                    icf_port_strlen(trace_data.string_val[0].str);

               /*icf_port_strcpy(trace_data.string_val[1].str,
                                       p_pdb->p_cta_ctx->p_call_ctx_b->remote_party_name);
             trace_data.string_val[1].str_len =
                        icf_port_strlen(trace_data.string_val[1].str);*/
          
	    break;
       
         case ICF_TRACE_CTA_RESP:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = p_pdb->result;

            trace_data.int_val[1] = p_pdb->error_cause;
    
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

	case ICF_TRACE_CTA_TIMER_EXPIRE:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;


          
         default:
 
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

    } /* switch(trace_id) */

    ICF_APP_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb, trace_id, &trace_data, p_criteria_data)
}
    return ret_val;

} /* IN    icf_trace_id_t     tr... */

/******************************************************************************
*
* FUNCTION NAME    :   icf_cta_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_cta_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_CTA_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_00_IDLE");
            break;
            
        case ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP");
            break;

        case ICF_CTA_02_WAIT_FOR_START_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_CTA_02_WAIT_FOR_START_PROC_SIG");
            break;

        case ICF_CTA_03_WAIT_FOR_REFER_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_03_WAIT_FOR_REFER_RESP");
            break;

        case ICF_CTA_04_WAIT_FOR_NOTIFY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_04_WAIT_FOR_NOTIFY");
            break;
    } /* switch(trace_id) */
    
}/*icf_cta_state_trace_string*/

/******************************************************************************
*
* FUNCTION NAME    :   icf_cta_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_cta_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST:
            icf_port_strcpy(p_trace_data->string_val[0].str,
            (icf_uint8_t *)"ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST");
            break;

        case ICF_CTA_CALL_HOLD_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_CALL_HOLD_RESP");
            break;

        case ICF_CTA_CALL_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_CALL_RESUME_RESP");
            break;

        case ICF_CTA_REFER_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_REFER_RESP");
            break;

        case ICF_CTA_NOTIFY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_NOTIFY");
            break;

        case ICF_CTA_START_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_START_PROC_SIG");
            break;

        case ICF_CTA_CC_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_CC_CALL_RELEASED");
            break;

        case ICF_CTA_TIMER_EXPIRY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_TIMER_EXPIRY");
            break;

        case ICF_CTA_PA_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTA_PA_CALL_RELEASED");
            break;

        default:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"Invalid Event");
            break;
    } /* switch(trace_id) */
} /* INOUT icf_trace_data_st   *... */
#endif

/******************************************************************************
*
* FUNCTION NAME: icf_cta_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_cta_clone_app_spec_hdr_lists(
    icf_cta_pdb_st   *p_cta_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = p_cta_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_call_transfer_attended_req_st *p_cta_msg = 
        (icf_call_transfer_attended_req_st *)&(p_glb_pdb->p_recv_msg->payload);

    /*SPR 19590 -If any of the header/body/tag lists are present in the
      CTA request then we shall increment the txn id in call ctx
      so that a unique node is created for the hdr/body/tag list
      in their respective lists of list in call ctx. Additionally
     we should always increment the last_txn_id which is global for 
     call and then copy the same in txn_id because txn_id is for the
     current node and not necessary its next number is unused.*/
    if((ICF_HDR_LIST_PRESENT & p_cta_msg->bit_mask) ||
       (ICF_TAG_LIST_PRESENT & p_cta_msg->bit_mask) ||    
       (ICF_BODY_LIST_PRESENT & p_cta_msg->bit_mask))
    {
        if(ICF_NULL != p_glb_pdb->p_call_ctx)
        {    
            p_glb_pdb->p_call_ctx->last_txn_id++;
            p_glb_pdb->p_call_ctx->txn_id = 
               p_glb_pdb->p_call_ctx->last_txn_id;
        }   

    }
    /* clone the header list into call ctx, if received from app */
	if (ICF_HDR_LIST_PRESENT & p_cta_msg->bit_mask)
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_hdr_list_node(
                     p_glb_pdb,
                     &(p_cta_msg->header_list));
        /*SPR 19590 change end */
    }
    
    /* clone the tag list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_TAG_LIST_PRESENT & p_cta_msg->bit_mask))
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_tag_list_node(
                     p_glb_pdb,
                     &(p_cta_msg->tag_list));
        /*SPR 19590 change end */
    }
    
    /* clone the body list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_BODY_LIST_PRESENT & p_cta_msg->bit_mask))
    {
        /*SPR 19590 change start */
        ret_val = icf_cmn_make_body_list_node(
                     p_glb_pdb,
                     &(p_cta_msg->body_list));
        /*SPR 19590 change end */
    }   
   
    return ret_val;
}

icf_return_t       icf_cta_apply_early_cta_check(icf_cta_pdb_st *p_pdb)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_pa_cc_call_transfer_attended_req_st  *p_cta_req = ICF_NULL;
    icf_call_ctx_st  *p_temp_connect_ctxt = ICF_NULL;
    icf_call_ctx_st  *p_temp_unconnect_ctxt = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb);
    p_cta_req = (icf_pa_cc_call_transfer_attended_req_st *)
        &(p_glb_pdb->p_recv_msg->payload[0]);

    if(!(ICF_CALL_CONNECT_DONE & 
                p_pdb->p_cta_ctx->p_call_ctx_b->common_bitmask)
            && !(ICF_CALL_CONNECT_DONE & 
                p_pdb->p_cta_ctx->p_call_ctx_c->common_bitmask))
    {
        ICF_PRINT(((icf_uint8_t*)"Neither of the calls are connected\n"));
        ret_val =  ICF_FAILURE;
    }
    if((ICF_CALL_CONNECT_DONE & 
                p_pdb->p_cta_ctx->p_call_ctx_b->common_bitmask)
            && (ICF_CALL_CONNECT_DONE & 
                p_pdb->p_cta_ctx->p_call_ctx_c->common_bitmask))
    {
        ICF_PRINT(((icf_uint8_t*)"Both of the calls are connected\n"));
        ret_val =  ICF_SUCCESS;
    }
    else
    {
        if (ICF_CALL_CONNECT_DONE & p_pdb->p_cta_ctx->p_call_ctx_b->common_bitmask)
        {
            p_temp_connect_ctxt = p_pdb->p_cta_ctx->p_call_ctx_b;
            p_temp_unconnect_ctxt = p_pdb->p_cta_ctx->p_call_ctx_c;
        }
        else 
        {
            p_temp_connect_ctxt = p_pdb->p_cta_ctx->p_call_ctx_c;
            p_temp_unconnect_ctxt = p_pdb->p_cta_ctx->p_call_ctx_b;
        }
        if (p_temp_connect_ctxt->call_id == p_glb_pdb->p_recv_msg->hdr.call_id)
        {
            if ((ICF_ALERT_RECEIVED & p_temp_unconnect_ctxt->prov_resp_status)
                    || (ICF_MEDIA_PARAM_RECEIVED & p_temp_unconnect_ctxt->prov_resp_status))
            {
                ICF_PRINT(((icf_uint8_t*)"Unconnected party has given alerted\n"));
                ret_val =  ICF_SUCCESS;
            }
            else
            {
                ICF_PRINT(((icf_uint8_t*)"Unconnected party has not given alert\n"));
                ret_val = ICF_FAILURE;
            }
        }
        else
        {
            ICF_PRINT(((icf_uint8_t*)"Refer request is not for connected call"));
            ret_val =  ICF_FAILURE;
        }
        /*Send failure CTA response to PA*/

    }
    ICF_FUNCTION_EXIT(p_glb_pdb);
    return ret_val;
}

#endif

