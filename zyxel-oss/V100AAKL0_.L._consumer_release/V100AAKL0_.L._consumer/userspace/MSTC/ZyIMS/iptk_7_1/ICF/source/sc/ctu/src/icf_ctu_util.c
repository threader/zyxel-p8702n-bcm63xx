/*******************************************************************************
*
* FILE NAME    : icf_ctu_util.c
*
* DESCRIPTION  : Contains the definitions of all utility functions
*                of CTU service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 27-Jan-2005    Amit Sharma     None            Initial
*  2-Feb-2005    Amit Sharma     None          To replace icf_port_isxdigit
*                                              with ICF_ISXDIGIT
* 10-May-2005    Amit Sharma     SPR:7485      Transferred party address is 
*                                              validated against remote and 
*                                              local addresses
* 11-May-2005    Amit Sharma     SPR:7537      Updation in the handling of
*                                              referred_by header
* 15-May-2005    Amit Sharma     SPR:7444      Update STATS
* 08-Jul-2005    Amit Sharma                   Initialize refer_req structure
* 15-Nov-2005    Amit Sharma     SPR:8185
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
* 26-May-2009    Anurag Khare    CSR 1-7297673  Changed function icf_validate
*                                               icf_ctu_validate_referred_address
*
* Copyright 2009, Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_ctu_types.h"
#include "icf_ctu_macro.h"
#include "icf_ctu_prototypes.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_port_prototypes.h"
#include "icf_port_macros.h"
#include "icf_dbm_prototypes.h"


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_send_refer_req
*
* DESCRIPTION   : Function is used to send REFER request to SSA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t     icf_ctu_send_refer_req(
        INOUT icf_ctu_pdb_st     *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_nw_otg_refer_req_st       refer_req;
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_error_t               *p_ecode = ICF_NULL;
    icf_line_data_st          *p_line_data = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_REFER_REQ_SENT)

    /* Set signaling ongoing in CTU service */
    ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ)

    ICF_CTU_SET_CURR_STATE(p_pdb, ICF_CTU_03_WAIT_FOR_REFER_RESP)

    refer_req.p_sic_ctx = ICF_NULL;
    refer_req.bitmask = ICF_NULL;
    /* Changes For XCONF */
    /* check the bitmask in incoming mesg that REFER with BYE is
       to be sent, if so then set the bitmask for SSA in thei
       new internal mesg (refer_rq) */ 
    if (ICF_TRUE == p_pdb->p_ctu_ctx->internal_refer_req)
    {
        if((ICF_NULL != p_pdb->p_internal_msg->p_msg_data) &&
            ((icf_internal_refer_req_st*)(p_pdb->p_internal_msg->
                p_msg_data))->bitmask & ICF_REFER_METHOD_BYE)
        {
            refer_req.bitmask |= ICF_NW_OTG_REFER_METHOD_BYE;
        }
    }
    /* Changes for XCONF End */

    refer_req.replace_call_id = ICF_INVALID_CALL_ID;

    /*Assign message id for SSA*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_REFER;

    p_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(icf_nw_otg_refer_st);

    /* Fetch config_data structure from DBM to get presentation
     * indication flag value in default_settings*/
    if(ICF_FAILURE ==
             icf_dbm_fetch_line_cntxt_blk(p_glb_pdb,
                p_glb_pdb->p_call_ctx->line_id,
                (icf_line_data_st **)&p_line_data,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*If Presentation indicator in CTU req is TRUE and toggle PI is false
         * in call context then req SSA to send referred-by header*/
        if ((ICF_TRUE == p_pdb->p_ctu_ctx->toggle_pi) ^ 
                ((ICF_TOGGLE_PI & p_glb_pdb->p_call_ctx->common_bitmask) ^
                (ICF_CFG_CC_USER_PRESENTATION_IND & 
                 p_line_data->default_settings)))
        {
            /*Update Local party name and address in call context*/
            if(ICF_SUCCESS == icf_dbm_fill_local_user_info(p_glb_pdb))
            {
                /*If local user name and address filled is successful then
                 *send REFER by referred by header else send REFER without
                 * referred by*/
                refer_req.bitmask = ICF_NW_OTG_REFER_REF_BY;
                
                ICF_CTU_TRACE(p_pdb, ICF_CTU_TRACE_REFER_REQ_WITH_REF_BY)
            } /* if(ICF_SUCCESS == icf_db... */
        } /* p_glb_pdb->p_recv_msg->payload... */

        /*Copy transferred party address*/
        refer_req.p_referred_to = &(p_pdb->p_ctu_ctx->transferred_addr);

        if(p_pdb->p_ctu_ctx->p_uri_parameter)
        {
            refer_req.bitmask |= ICF_REFER_TO_ADDNL_URI_PARAMTR_PRES;
            refer_req.p_uri_parameter = p_pdb->p_ctu_ctx->p_uri_parameter;
        }
        /*Assign refer_req to p_msg_data of internal message*/
        p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&refer_req;

        /*Call entry point function of SSA*/
        ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);
    } /* else */


    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_ctu_send_refer_req */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_start_ctu_timer
*
* DESCRIPTION   : Function is used to allocate memory for CTU timer data and
*                 start CTU timer
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_start_ctu_timer(
        INOUT icf_ctu_pdb_st     *p_pdb)
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
                (icf_void_t **)&(p_pdb->p_ctu_ctx->p_ctu_timer_data),
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        
        p_pdb->p_ctu_ctx->p_ctu_timer_data->timer_type = ICF_CTU_TIMER;
        p_pdb->p_ctu_ctx->p_ctu_timer_data->module_id = ICF_INT_MODULE_CTU;
        /*
         * Store call context pointer in p_timer buffer
         * so that when timer expiry receives module can
         * identify the call data
         */
        p_pdb->p_ctu_ctx->p_ctu_timer_data->p_timer_buffer =
            (icf_void_t *)p_glb_pdb->p_call_ctx;
        p_pdb->p_ctu_ctx->p_ctu_timer_data->p_glb_pdb = p_glb_pdb;

        /*Start ICF_CTU_TIMER */
        ICF_START_TIMER(p_glb_pdb, p_pdb->p_ctu_ctx->p_ctu_timer_data,
                p_pdb->p_ctu_ctx->ctu_timer_id,
                p_config_data->ctu_timer,
                p_ecode, ret_val)

        ICF_STAT(p_glb_pdb, ICF_STAT_CTU_TMR_STARTED)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
}


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_stop_ctu_timer
*
* DESCRIPTION   : Function is used to de-allocate memory of CTU timer data and
*                 stop CTU timer
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_stop_ctu_timer(
        INOUT icf_ctu_pdb_st     *p_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_error_t      *p_ecode = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_ctu_ctx->ctu_timer_id, p_ecode, ret_val)

    ICF_STAT(p_glb_pdb, ICF_STAT_CTU_TMR_STOPPED)

    p_pdb->p_ctu_ctx->ctu_timer_id = 0;
    
    if(ICF_NULL != p_pdb->p_ctu_ctx->p_ctu_timer_data)
    {
        if(ICF_FAILURE == ret_val)
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_ctu_ctx->p_ctu_timer_data),
                    p_ecode);

            p_pdb->p_ctu_ctx->p_ctu_timer_data = ICF_NULL;

            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == ret_val) */
        else
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_ctu_ctx->p_ctu_timer_data),
                    p_ecode);

            p_pdb->p_ctu_ctx->p_ctu_timer_data = ICF_NULL;
        } /* else */
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    return ret_val;
}

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_send_ctu_resp
*
* DESCRIPTION   : Function is used to CTU response to PA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_send_ctu_resp(
        INOUT  icf_ctu_pdb_st   *p_pdb,
        IN     icf_boolean_t    result,
        IN     icf_error_t      error_cause)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          *p_msg = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_uint8_t         api_len = 0;
    icf_ctu_resp_st     *p_ctu_resp = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)
    /* Changes For XCONF */	
    if(ICF_TRUE == p_pdb->p_ctu_ctx->internal_refer_req)
    {
    	if(ICF_SUCCESS == result)
    	{
		ICF_CTU_SET_STATUS_2(p_pdb,
                ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP);
	}
	else
	{
		ICF_CTU_SET_STATUS_2(p_pdb,
                ICF_STATUS_INTERNAL_REFER_FAILURE_RESP);
	}
	 p_pdb->p_internal_msg->msg_hdr.ecode = error_cause;
    }
    /*  Changes For XCONF End  */
    else
    {
    	api_len =
    	sizeof(icf_api_header_st) + 
    	sizeof(icf_ctu_resp_st);
    	/*sizeof(icf_cc_pa_call_transfer_unattended_resp_st);*/
	#ifdef ICF_TRACE_ENABLE
    	   /* Added to send traces */
    	   p_pdb->result=result;
    	   p_pdb->error_cause=error_cause;
    	   ICF_CTU_APP_TRACE(p_pdb, ICF_TRACE_CTU_RESP)
	#endif


    	/*Allocate memory for API header strucure */
    	ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON,
            p_msg, ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

    	if(ICF_NULL != p_msg)
    	{

            /*
            icf_ctu_fill_pa_msg_hdr(p_pdb, p_msg,
                (icf_uint8_t)(ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP));

            icf_ctu_fill_pa_ctu_resp(p_pdb,
                (icf_cc_pa_call_transfer_unattended_resp_st *)
                &p_msg->payload[0],
                result, error_cause);
            */

            icf_cmn_fill_resp_msg(p_glb_pdb, p_msg, 
                (icf_module_id_t)(ICF_MODULE_PA),
                (icf_api_id_t)(ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP),
                result, (icf_error_t)error_cause);

            p_msg->hdr.api_length = sizeof(icf_api_header_st) +
            sizeof(icf_ctu_resp_st);

            p_ctu_resp = (icf_ctu_resp_st*)&(p_msg->payload[0]);
 
            if (0 != p_glb_pdb->p_call_ctx->retry_after_duration)
            {
                p_ctu_resp->retry_after_duration = 
                    p_glb_pdb->p_call_ctx->retry_after_duration;
                p_glb_pdb->p_call_ctx->retry_after_duration = 0;
                p_ctu_resp->bit_mask |=
                               ICF_RETRY_AFTER_DURATION_PRESENT;
            }

            /* clone the header list */
            if (ICF_NULL != p_glb_pdb->p_call_ctx->header_list.count)
            {
                if (ICF_SUCCESS == icf_cmn_clone_header_list(
                    p_glb_pdb,
                    &(p_glb_pdb->p_call_ctx->header_list),
                    &(p_ctu_resp->header_list),
                    p_ecode))
                {
                    p_ctu_resp->bit_mask |= ICF_HDR_LIST_PRESENT;
                
                    /* free the list in call ctx */
                    ret_val = icf_cmn_free_header_list(p_glb_pdb,
                        &(p_glb_pdb->p_call_ctx->header_list),
                        p_ecode);
                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
            }
            else
  	    {
                p_ctu_resp->header_list.hdr = ICF_NULL;
                p_ctu_resp->header_list.count = 0;
            }
                
            /*Assign p_msg to p_msg_data of icf-internal_msg_st*/

            if (ICF_SUCCESS == ret_val)
            {
                p_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP;
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
                        &(p_ctu_resp->header_list),
                        p_ecode);

                    ICF_MEMFREE(p_glb_pdb, p_msg,
                        ICF_MEM_COMMON, p_ecode, ret_val)
                    
                        ret_val = ICF_FAILURE;
                } /* icf_adm_process_mesg(p_pdb-... */
                else
                {
                    /* free the list in call ctx */
                    ret_val = icf_cmn_free_header_list(p_glb_pdb,
                        &(p_ctu_resp->header_list),
                        p_ecode);

                    ICF_MEMFREE(p_glb_pdb, p_msg,
                        ICF_MEM_COMMON, p_ecode, ret_val)
                } /* else */
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
        } /* if(ICF_NULL != p_msg) */

    }
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_ctu_send_ctu_resp */

#if 0
/******************************************************************************
*
* FUNCTION NAME    :   icf_ctu_fill_pa_ctu_resp
*
* DESCRIPTION  : This function fills the 
*                icf_cc_pa_call_transfer_unattended_resp_st
*
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_ctu_fill_pa_ctu_resp(
        INOUT   icf_ctu_pdb_st                              *p_pdb,
        INOUT   icf_cc_pa_call_transfer_unattended_resp_st  *p_ctu_resp,
        IN      icf_uint8_t                                 resp,
        IN      icf_uint8_t                                 error_cause
        )
{
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_return_t     ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*resp contains Success or Failure for conf request*/
    p_ctu_resp->result = resp;

    /*error_cause in case of failure return for conf request*/
    p_ctu_resp->error_cause = error_cause;

    ICF_FUNCTION_EXIT(p_glb_pdb)
} /*icf_ctu_fill_pa_resume_resp*/


/******************************************************************************
 *
 * FUNCTION NAME    :   icf_ctu_fill_pa_msg_hdr
 *
 * DESCRIPTION  : This function fills the icf_api_header_st to send message
 *                to PA
 *
 *
 * RETURNS:  ICF_SUCCESS
 *
 *****************************************************************************/

icf_void_t icf_ctu_fill_pa_msg_hdr(
        INOUT   icf_ctu_pdb_st  *p_pdb,
        INOUT   icf_msg_st      *p_msg,
        IN      icf_uint8_t     api_id
        )
{
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;
    icf_return_t       ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_msg->hdr.api_id = api_id;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PA;

    p_msg->hdr.payload_length = sizeof(icf_api_header_st) +
        sizeof(icf_cc_pa_call_transfer_unattended_resp_st);

    p_msg->hdr.call_id = p_glb_pdb->p_call_ctx->call_id;

    ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_ctu_fill_pa_msg_hdr */
#endif

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_process_hold_resume_resp
*
* DESCRIPTION   : Function is used to process hold and resume reponse
* 
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_process_hold_resume_resp(
        INOUT  icf_ctu_pdb_st   *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    /*Function to check signaling ongoing flag, and send REFER request
     *if signaling onoing is false*/
    ret_val = icf_ctu_check_sig_ong_n_send_refer(p_pdb);

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)   

    return ret_val;
} /* icf_ctu_process_hold_resume_resp */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_check_sig_ong_n_send_refer
*
* DESCRIPTION   : Function is used to check signaling ongoing and send REFER
*                 request to SSA
* 
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_check_sig_ong_n_send_refer(
        INOUT  icf_ctu_pdb_st   *p_pdb)
{
   icf_return_t    ret_val = ICF_SUCCESS;

   ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

   /*If signaling ongoing in any other module*/
   if(ICF_INT_MODULE_INVALID != ICF_CTU_GET_SIG_ONGOING(p_pdb))
   {
       ICF_CTU_SET_CURR_STATE(p_pdb, 
               ICF_CTU_01_WAIT_FOR_START_PROC_SIG)
    
       /*Set status for start proc signaling*/
       ICF_CTU_SET_STATUS(p_pdb, 
               ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ)
   } /* else if(ICF_MODULE_ID_INVAL... */
   else
   {
       /*Else send REFER request*/
       ret_val = icf_ctu_send_refer_req(p_pdb);
   }

   ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

   return ret_val;
} /* icf_ctu_check_sig_ong_n_send_refer */


/*****************************************************************************
*
* FUNCTION NAME:   icf_ctu_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize CTU context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_ctu_deinit_ctx(
        INOUT icf_ctu_pdb_st  *p_pdb
        )
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t   *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
  
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Set state as ICF_CTU_00_IDLE*/
    p_pdb->p_ctu_ctx->state = ICF_CTU_00_IDLE;

    /*If timer ID is not 0 then stop timer*/
    if(0 != p_pdb->p_ctu_ctx->ctu_timer_id)
    {
        ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_ctu_ctx->ctu_timer_id, p_ecode, ret_val)

        p_pdb->p_ctu_ctx->ctu_timer_id = ICF_NULL;
    } /* if(0 != p_pdb->p_cut_ctx->ctu_... */
    
    /*If timer data has not been freed free it*/
    if(ICF_NULL != p_pdb->p_ctu_ctx->p_ctu_timer_data)
    {
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                p_pdb->p_ctu_ctx->p_ctu_timer_data,
                p_ecode);
        p_pdb->p_ctu_ctx->p_ctu_timer_data = ICF_NULL;
    }

    /*Call context NULL*/
    p_pdb->p_ctu_ctx->p_call_ctx = ICF_NULL;
    
    p_pdb->p_ctu_ctx->call_rel_initiated = ICF_NULL;

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
} /* icf_chr_deinit_ctx */


/*****************************************************************************
*
* FUNCTION NAME:   icf_ctu_validate_referred_address
*
* DESCRIPTION  : This function is used to validate the remote address
* 
* RETURNS:
******************************************************************************/

icf_return_t  icf_ctu_validate_referred_address(
        INOUT icf_ctu_pdb_st  *p_pdb,
        INOUT icf_address_st  *p_address)
{
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_error_t   *p_ecode = ICF_NULL;

    icf_return_t  ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*If received address is null then return failure*/
    if (ICF_NULL == p_address)
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_NULL == p_address) */
     else if ((ICF_FAILURE == icf_cmn_validate_sip_addr(p_glb_pdb,
                    p_address, p_ecode)))
    {
        ret_val = ICF_FAILURE;
    }
    if(ICF_SUCCESS == ret_val)
    {
        /*  Fix for CSR 1-7297673 */
        /*If transferred party address is same as remote party address
         *or local user then  return 
     * failure*/
        if (ICF_TRUE == icf_cmn_compare_address(
                        p_address,
                        &(p_glb_pdb->p_call_ctx->remote_party_addr)))
    {
        ret_val = ICF_FAILURE;
    }
	else if (ICF_TRUE == icf_cmn_compare_address(
                             p_address,
                             &(p_glb_pdb->p_call_ctx->local_address)))
    {
        ret_val = ICF_FAILURE;
    }
    }
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_ctu_validate_referred_address */


/******************************************************************************
*
* FUNCTION NAME:   icf_ctu_abort_refer_trans
*
* DESCRIPTION  : This function is used to send REFER ABORT request to SSA
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_ctu_abort_refer_trans(
        INOUT   icf_ctu_pdb_st    *p_pdb)
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
} /* icf_ctu_abort_refer_trans */

/******************************************************************************
*
* FUNCTION NAME:   icf_ctu_trace
*
* DESCRIPTION  : This function is used to define the trace data for CTU service
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_void_t    icf_ctu_trace(
        INOUT icf_ctu_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data={{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0};

    switch(trace_id)
    {
        case ICF_TRACE_CTU_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;

            /* Transferred Party Address */
            icf_port_strcpy(trace_data.string_val[0].str, 
                                        p_pdb->p_ctu_ctx->transferred_addr.addr_val.str); 

            break;

        case ICF_CTU_TRACE_CALL_RESUME_RESP_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;
            break;

        case ICF_CTU_TRACE_CALL_HOLD_RESP_RECVD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;
            break;

        case ICF_CTU_TRACE_REFER_REQ_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;

            /* Transferred Party Address */
            icf_port_strcpy(trace_data.string_val[0].str, 
                    p_pdb->p_ctu_ctx->transferred_addr.addr_val.str);
            break;

        case ICF_CTU_TRACE_REFER_REQ_WITH_REF_BY:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;
            break;

        case ICF_CTU_TRACE_PA_INITIATED_CALL_REL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;
            break;

        case ICF_CTU_TRACE_CC_INITIATED_CALL_REL:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;
            break;

        case ICF_CTU_TRACE_INVALID_ADDRESS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_ctu_ctx->p_call_ctx->call_id;

            /* Transferred Party Address */
            icf_port_strcpy(trace_data.string_val[0].str, 
                    p_pdb->p_ctu_ctx->transferred_addr.addr_val.str);
            break;

        case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;

        case ICF_TRACE_CTU_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_ctu_ctx->state;
            icf_ctu_state_trace_string(p_pdb->p_ctu_ctx->state, &trace_data);
            break;
            
        case ICF_TRACE_CTU_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_ctu_ctx->event;
            trace_data.int_val[1] = 
                (icf_uint32_t)
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;
            icf_ctu_event_trace_string(p_pdb->p_ctu_ctx->event, &trace_data);
            break;
        
        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */       

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data)
} /* icf_ctu_trace */


/******************************************************************************
*
* FUNCTION NAME:   icf_ctu_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for CTU service
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_ctu_app_trace(
        INOUT icf_ctu_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data = {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0};
    icf_trace_criteria_data_st    *p_criteria_data = p_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
    /* Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;

    ret_val = icf_cmn_check_app_trace(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
        return ret_val;
    }
    else
    {

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;
 
   
    switch(trace_id)
    {
           case ICF_TRACE_CTU_REQ:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;

            icf_port_strcpy(trace_data.string_val[0].str,
                    p_pdb->p_ctu_ctx->transferred_addr.addr_val.str);

            trace_data.string_val[0].str_len = (icf_uint16_t)
                    icf_port_strlen(trace_data.string_val[0].str);

            /*icf_port_strcpy(trace_data.string_val[1].str,
                                        p_pdb->p_ctu_ctx->transferred_name.str);
            trace_data.string_val[1].str_len =
                        icf_port_strlen(trace_data.string_val[1].str);*/
        
            break;


          case ICF_TRACE_CTU_RESP:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = p_pdb->result;

            trace_data.int_val[1] = p_pdb->error_cause;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;

        case ICF_TRACE_CTU_TIMER_EXPIRE:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;


	default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */

    ICF_APP_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data , p_criteria_data)
}
    return ret_val;
} /* icf_ctu_trace */


/******************************************************************************
*
* FUNCTION NAME    :   icf_ctu_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_ctu_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_CTU_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_00_IDLE");
            break;
            
        case ICF_CTU_01_WAIT_FOR_START_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_01_WAIT_FOR_START_PROC_SIG");
            break;

        case ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                (icf_uint8_t *)"ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP");
            break;

        case ICF_CTU_03_WAIT_FOR_REFER_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_03_WAIT_FOR_REFER_RESP");
            break;
        case ICF_CTU_04_WAIT_FOR_NOTIFY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_04_WAIT_FOR_NOTIFY");
            break;
        default:
            break;

    } /* switch(trace_id) */
    
}/*icf_twc_state_trace_string*/

/******************************************************************************
*
* FUNCTION NAME    :   icf_ctu_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_ctu_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST:
            icf_port_strcpy(p_trace_data->string_val[0].str,
            (icf_uint8_t *)"ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST");
            break;

        case ICF_CTU_CALL_HOLD_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_CALL_HOLD_RESP");
            break;

        case ICF_CTU_CALL_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_CALL_RESUME_RESP");
            break;

        case ICF_CTU_REFER_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_REFER_RESP");
            break;

        case ICF_CTU_START_PROC_SIG:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_START_PROC_SIG");
            break;

        case ICF_CTU_CC_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_CC_CALL_RELEASED");
            break;

        case ICF_CTU_TIMER_EXPIRY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_TIMER_EXPIRY");
            break;

        case ICF_CTU_PA_CALL_RELEASED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_CTU_PA_CALL_RELEASED");
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
* FUNCTION NAME: icf_ctu_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_ctu_clone_app_spec_hdr_lists(
    icf_ctu_pdb_st   *p_ctu_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = p_ctu_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_error_t    *p_ecode = &(p_ctu_pdb->p_internal_msg->msg_hdr.ecode);
    icf_call_transfer_unattended_req_st *p_ctu_msg = 
        (icf_call_transfer_unattended_req_st *)&(p_glb_pdb->p_recv_msg->payload);

    /* clone the header list into call ctx, if received from app */
	if (ICF_HDR_LIST_PRESENT & p_ctu_msg->bit_mask)
    {
        ret_val = icf_cmn_free_header_list(
            p_glb_pdb, &(p_glb_pdb->p_call_ctx->header_list), p_ecode);

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cmn_clone_header_list(
                p_glb_pdb,
                &(p_ctu_msg->header_list),
                &(p_glb_pdb->p_call_ctx->header_list),
                p_ecode);
        }
    }
    
    /* clone the tag list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_TAG_LIST_PRESENT & p_ctu_msg->bit_mask))
    {
        ret_val = icf_cmn_free_tag_list(
            p_glb_pdb,
            &(p_glb_pdb->p_call_ctx->tag_list),
            p_ecode);

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cmn_clone_tag_list(
                p_glb_pdb,
                &(p_ctu_msg->tag_list),
                &(p_glb_pdb->p_call_ctx->tag_list),
                p_ecode);
        }
    }
    
    /* clone the body list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) && 
        (ICF_BODY_LIST_PRESENT & p_ctu_msg->bit_mask))
    {
        ret_val = icf_cmn_free_msg_body_list(
            p_glb_pdb,
            &(p_glb_pdb->p_call_ctx->body_list),
            p_ecode);

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_cmn_clone_body_list(
                p_glb_pdb,
                &(p_ctu_msg->body_list),
                &(p_glb_pdb->p_call_ctx->body_list),
                p_ecode);
        }
    }   
   
    return ret_val;
}


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_send_notify_resp
*
* DESCRIPTION   : Function is used to send response for NOTIFY received to SSA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t     icf_ctu_send_notify_resp(
        INOUT icf_ctu_pdb_st     *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_return_t              ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Assign message id for SSA*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_OTG_NOTIFY_RESP;

    /*Call entry point function of SSA*/
    ret_val = icf_ssa_process_mesg(p_pdb->p_internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_ctu_send_notify_resp */

#endif
