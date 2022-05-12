/*******************************************************************************
*
* FILE NAME    : icf_twc_utils.c
*
* DESCRIPTION  : Contains all the utility functions used in TWC service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   11-Jan-2005  Amit Sharma     None           Created Origional
*   16-Feb-2005  Amit Sharma     None           Added functionality for other
*                                               call and both calls
*   25-Apr-2005  Amit Sharma     SPR:7423       Updated other call info in 
*                                               return path
*   25-Apr-2005  Amit Sharma     SPR:7430       Updated prototype of 
*                                               icf_twc_get_other_call_id
*   28-Apr-2005  Amit Sharma     SPR:7465       Failure response when conf req
*                                               received when one of two  call
*                                               is in release state
*   05-May-2005  Amit Sharma     SPR:7423       Updation for other call info
*                                               added payload_length
*   15-May-2005  Amit Sharma     SPR:7444       Update STATS
*   19-May-2005  Amit Sharma     SPR:7430       Updation for chr active cond
*   27-Feb-2006  Amit Sharma                  Merged code for ICF
*   24-Dec-2007  Aman Aggarwal   SPR:17865      Call_ctx_req in twc context
*												filled
*   01-Apr-2009  Kamal Ashraf    Rel 8.2        Klocwork warning Fix
*   04-May-2009  Tarun Gupta     SPR 19573      Modified icf_twc_send_merge_media_ind
*   26-May-2009  Anuradha Gupta  SPR 19672      CSR 1-6667668 Merge
*   10-Jun-2009  Abhishek Dhammawat SPR 19590   header list overwrite enh
*   21-Jun-2009  Abhishek Dhammawat SPR 19590   header list overwrite enh
*
* Copyright 2009,Aricent.
*
******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_twc_types.h"
#include "icf_twc_prototypes.h"
#include "icf_twc_macro.h"
#include "icf_dbm_prototypes.h"
#include "icf_sc_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_set_call_ctx_in_twc_ctx
*
* DESCRIPTION  : This function is used to properly assign call contexts
*                for the service logic. If call hold service is active in 
*                any call context, it is stored in call contexts b, otherwise
*                if media is not commited for any of two calls, set this in 
*                call ctx C
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_twc_set_call_ctx_in_twc_ctx(
        INOUT   icf_twc_pdb_st *p_pdb)
{
    icf_return_t      ret_val = ICF_SUCCESS;
    icf_call_ctx_st   *p_call_ctx_b = ICF_NULL;
    icf_call_ctx_st   *p_call_ctx_c = ICF_NULL;
    icf_call_id_t     other_call_id;
    icf_glb_pdb_st    *p_glb_pdb = ICF_NULL;
    icf_error_t       *p_ecode = ICF_NULL;
  
    /*Assign p_glb_pdb from internal message received*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Assign ecode pointer from internal message ecode*/
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /*Assign call context pointer on which conference request
     *received*/
    p_call_ctx_b = p_glb_pdb->p_call_ctx;

    /*Save the call context on which Conference req is initiated
     *This is required to send the conference response to the correct
     *call-id
     */ 
    p_pdb->p_twc_ctx->p_call_ctx_req = p_call_ctx_b;


    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_b))

    if (ICF_TRUE == p_pdb->conf_internal_req)
    {
        other_call_id = p_call_ctx_b->call_to_be_joined;
    }
    else
    {
        /*Get other call context and store it in p_call_ctx_c*/
        other_call_id = ((icf_pa_cc_conference_req_st *)
            p_glb_pdb->p_recv_msg->payload)->merge_call_id;
    }

    if(ICF_INVALID_CALL_ID == other_call_id)
    {
        ret_val = icf_twc_send_adm_conference_resp(p_pdb,
        (icf_boolean_t)ICF_FAILURE,
        (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);
 
        /*Send service ended normal to both the calls*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

    } /* if (ICF_INVALID_CALL_ID == other_call_id) */    
    /*Call DBM function to get the call context pointer*/
    else if(ICF_FAILURE == 
            icf_dbm_find_call_cntxt_blk(p_glb_pdb, other_call_id,
            &p_call_ctx_c, p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    /*Return failure if call context is null*/
    else if(ICF_NULL == p_call_ctx_c)
    {
        ret_val = ICF_FAILURE;
    } 
    /* Conference failure response when call is in releasing state*/
    else if ((ICF_REL_TYPE_INVALID != p_call_ctx_b->release_type)
            || (ICF_REL_TYPE_INVALID != p_call_ctx_c->release_type)
            || ((((icf_service_ctx_st *)p_call_ctx_b->p_service_ctx)->
            active_service_list.active_service_status 
            & ICF_SERVICE_CHR)
                && ((icf_service_ctx_st *)p_call_ctx_c->p_service_ctx)->
            active_service_list.active_service_status 
            & ICF_SERVICE_CHR))
    {
        /*Store this call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_b;

        /*Store this call context in p_call_ctx_c*/
        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_c;
        
        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO)

        ret_val = icf_twc_send_adm_conference_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        /*Send service ended normal to both the calls*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = 
                icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_call_ctx_c);
        }
    } /* || (ICF_REL_TYPE_INVALID !=... */    
    /*Check if CHR service is active in current call ctx*/
    else if(((icf_service_ctx_st *)p_call_ctx_b->p_service_ctx)->
            active_service_list.active_service_status 
            & ICF_SERVICE_CHR)
    {
        /*Store this call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_b;

        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_c))
        
        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_c;
    } /* p_pdb->p_internal_msg->msg_hdr... */
    /*Check if CHR service is active in call ctx C*/
    else if(((icf_service_ctx_st *)p_call_ctx_c->p_service_ctx)->
            active_service_list.active_service_status & ICF_SERVICE_CHR)
    {
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_c))
        
        /*Store this call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_c;

        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_b;
    } /* if(ICF_INT_MODULE_TWC == p_... */
    /*Checking whether media is committed for call ctx b*/
    else if(ICF_TRUE == p_call_ctx_b->media_committed)
    {
        /*Store p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx
         * in p_call_ctx_b*/
        /*Store this call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_b;

        /*Store this call context in p_call_ctx_c*/
        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_c;
    } /* else */
    /*Checking whether media is committed for call ctx b*/
    else if(ICF_TRUE == p_call_ctx_c->media_committed)
    {
        /*If media not committed in call_ctx_b then it
         * may be new call*/
         /*Store other call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_c;

        /*Store this call context in p_call_ctx_c*/
        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_b;
    } /* else */
    else
    {
        /*Store this call context in p_call_ctx_b*/
        p_pdb->p_twc_ctx->p_call_ctx_b = p_call_ctx_b;

        /*Store this call context in p_call_ctx_c*/
        p_pdb->p_twc_ctx->p_call_ctx_c = p_call_ctx_c;

        /*If media not committed for both the calls then send failure
         *to PA with no service action*/
        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO)

        ret_val = icf_twc_send_adm_conference_resp(p_pdb, 
                (icf_boolean_t)ICF_FAILURE,
                (icf_error_t)ICF_CAUSE_INVALID_SERVICE_ACTION);

        /*Send service ended normal to both the calls*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = 
                icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
        }
    } /* else */
        
    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_set_call_ctx_in_twc_ctx */

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_start_twc_timer
*
* DESCRIPTION  : This function is used to start TWC timer
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t  icf_twc_start_twc_timer(
        INOUT icf_twc_pdb_st    *p_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
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
                ((icf_void_t **)&(p_pdb->p_twc_ctx->p_twc_timer_data)),
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        ICF_TWC_TRACE(p_pdb, ICF_TRACE_START_TIMER)
            
        p_pdb->p_twc_ctx->p_twc_timer_data->timer_type = ICF_TWC_TIMER;
        p_pdb->p_twc_ctx->p_twc_timer_data->module_id = ICF_INT_MODULE_TWC;
        /*
         * Store call context pointer in p_timer buffer
         * so that when timer expiry receives module can
         * identify the call data
         */
        p_pdb->p_twc_ctx->p_twc_timer_data->p_timer_buffer = 
            (icf_void_t *)p_glb_pdb->p_call_ctx;
        p_pdb->p_twc_ctx->p_twc_timer_data->p_glb_pdb = p_glb_pdb;

        /*Start ICF_TWC_TIMER */
        ICF_START_TIMER(p_glb_pdb, p_pdb->p_twc_ctx->p_twc_timer_data, 
                p_pdb->p_twc_ctx->twc_timer_id, 
                p_config_data->twc_timer,
                p_ecode, ret_val)

        ICF_STAT(p_glb_pdb, ICF_STAT_TWC_TMR_STARTED)
    } /* else */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /*icf_twc_start_twc_timer*/

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_stop_twc_timer
*
* DESCRIPTION  : This function is used to stop TWC timer
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t  icf_twc_stop_twc_timer(
        INOUT icf_twc_pdb_st    *p_pdb)
{
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_return_t     ret_val;
    icf_error_t      *p_ecode = ICF_NULL;
   
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
 
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_twc_ctx->twc_timer_id, p_ecode, ret_val)

    ICF_STAT(p_glb_pdb, ICF_STAT_TWC_TMR_STOPPED)

    ICF_TWC_TRACE(p_pdb, ICF_TRACE_STOP_TIMER)

    p_pdb->p_twc_ctx->twc_timer_id = 0;

    if(ICF_NULL != p_pdb->p_twc_ctx->p_twc_timer_data)
    {
        if(ICF_FAILURE == ret_val)
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_twc_ctx->p_twc_timer_data),
                    p_ecode);
            
            p_pdb->p_twc_ctx->p_twc_timer_data = ICF_NULL;

            ret_val = ICF_FAILURE;
        } /* if(ICF_FAILURE == ret_val) */
        else
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                    (icf_void_t *)(p_pdb->p_twc_ctx->p_twc_timer_data),
                    p_ecode);
            
            p_pdb->p_twc_ctx->p_twc_timer_data = ICF_NULL;
        } /* else */        
    }

    return ret_val;
} /* INOUT icf_twc_pdb_st    *p_... */


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_send_adm_conference_resp
*
* DESCRIPTION  : This function is used to send TWC response to ADM            
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t  icf_twc_send_adm_conference_resp(
        INOUT icf_twc_pdb_st    *p_pdb, 
        IN    icf_boolean_t     result,
        IN    icf_error_t       error_cause)
{
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_msg_st          *p_msg = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_error_t         *p_ecode = ICF_NULL;
    icf_uint8_t         api_len = 0;
    icf_call_ctx_st     *p_orig_call_ctx = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    /* Copy the origingal call_ctx into p_glb_ctx for sending
     * conference response.
     */
    p_orig_call_ctx = p_glb_pdb->p_call_ctx;
    p_glb_pdb->p_call_ctx = p_pdb->p_twc_ctx->p_call_ctx_req;

    p_ecode =  &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_FUNCTION_ENTER(p_glb_pdb)

    api_len = 
    sizeof(icf_api_header_st) + sizeof(icf_msg_resp_st);

    ICF_STAT(p_glb_pdb, ICF_STAT_CC_PA_CONF_RESP)

    if (ICF_FAILURE == result)
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_UNSUCC_CONF)
    } /* if (ICF_FAILURE == result) */
    else
    {
        ICF_STAT(p_glb_pdb, ICF_STAT_SUCC_CONF)
    } /* else */           
#ifdef ICF_TRACE_ENABLE
    /* Added to send traces */
    p_pdb->result=result;
    p_pdb->error_cause=error_cause;
    ICF_TWC_APP_TRACE(p_pdb, ICF_TRACE_CONFERENCE_RESP)
#endif
    /*Allocate memory for API header strucure */
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON,
            p_msg, ICF_RET_ON_FAILURE, 
            p_ecode, ret_val)

    if(ICF_NULL != p_msg)
    {
        icf_cmn_fill_resp_msg(p_glb_pdb, p_msg,
                (icf_module_id_t)(ICF_MODULE_PA),
                (icf_api_id_t)(ICF_CC_PA_CONFERENCE_RESP),
                (icf_result_t)result, (icf_error_t)error_cause);

        /*Assign p_msg to p_msg_data of icf-internal_msg_st*/

        p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CONFERENCE_RESP;
        /* Fix for CSR1-5249829 */
        p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_msg_lst->p_msg->hdr.call_id =
                                        p_pdb->p_twc_ctx->p_call_ctx_b->call_id;

        p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

        p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

        /*Call icf_adm_process_mesg(p_internal_msg)*/
        if (ICF_FAILURE == 
                icf_adm_process_mesg(p_pdb->p_internal_msg))
        { 
            ICF_MEMFREE(p_glb_pdb, p_msg, 
                    ICF_MEM_COMMON, p_ecode, ret_val)

            ret_val = ICF_FAILURE;
        } /* icf_adm_process_mesg(p_pdb-... */
        else
        {
            ICF_MEMFREE(p_glb_pdb, p_msg, 
                    ICF_MEM_COMMON, p_ecode, ret_val)
        } /* else */           
    } /* if(ICF_NULL != p_msg) */

    /* Restore the call ctx in global pbd */
    p_glb_pdb->p_call_ctx = p_orig_call_ctx;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;

} /* icf_twc_send_adm_conference_resp*/

/**********************************************************************************
*
* FUNCTION NAME : icf_twc_send_merge_media_ind
*
* DESCRIPTION   : This function sends merge media indication to PA after merging
*                 of media
*
************************************************************************************/

icf_uint8_t   icf_twc_send_merge_media_ind(
        INOUT   icf_twc_pdb_st    *p_pdb)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_glb_pdb_st               *p_glb_pdb = ICF_NULL;
    icf_cc_pa_call_merged_ind_st *p_merge_ind = ICF_NULL;
    icf_msg_st                   *p_msg = ICF_NULL;
    icf_uint16_t                 api_len;
    icf_error_t                  *p_ecode = ICF_NULL;
    icf_call_ctx_st              *p_sec_call_ctx = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);
        
    api_len = sizeof(icf_api_header_st) +
    sizeof(icf_cc_pa_call_merged_ind_st);
   
    /*Allocate memory to send message to PA*/
    ICF_MEMGET(p_glb_pdb, api_len, ICF_MEM_COMMON, p_msg,
            ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Set conference information in both the call contexts*/
    p_pdb->p_twc_ctx->p_call_ctx_b->call_status |=
        ICF_CALL_STATUS_CONFERENCE;

    p_pdb->p_twc_ctx->p_call_ctx_c->call_status |=
        ICF_CALL_STATUS_CONFERENCE;

    p_pdb->p_twc_ctx->p_call_ctx_b->p_conf_other_call_ctx =
        p_pdb->p_twc_ctx->p_call_ctx_c;

    p_pdb->p_twc_ctx->p_call_ctx_c->p_conf_other_call_ctx =
        p_pdb->p_twc_ctx->p_call_ctx_b;


    /*Assign pointer of the payload of the PA message*/
    p_merge_ind = (icf_cc_pa_call_merged_ind_st *)&(p_msg->payload[0]);

    /*Fill PA API header in p_msg*/
    p_msg->hdr.api_id = ICF_CC_PA_CALL_MERGED_IND;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PA;


    p_msg->hdr.api_length = sizeof(icf_api_header_st) 
        + sizeof(icf_cc_pa_call_merged_ind_st);

    p_msg->hdr.call_id = p_glb_pdb->p_call_ctx->call_id;

    /* SPR 19573 */
    p_msg->hdr.app_id = p_glb_pdb->p_call_ctx->app_id;
    /* End SPR 19573 */

    /*Fill merge_call_id same as refer ind*/
    p_sec_call_ctx = icf_twc_get_other_call_ctx(p_pdb);
    p_merge_ind->merge_call_id = p_sec_call_ctx->call_id;
    
    /*Fill Internal message*/
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CC_PA_CALL_MERGED_IND;

    p_pdb->p_internal_msg->msg_hdr.payload_length = api_len;

    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)p_msg;

    ICF_TWC_TRACE(p_pdb, ICF_TRACE_MERGE_IND_SENT)

    /*Call icf_adm_process_mesg(p_internal_msg)*/
    if (ICF_FAILURE == 
            icf_adm_process_mesg(p_pdb->p_internal_msg))
    {
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)

        ret_val = ICF_FAILURE;
    }/* icf_adm_process_mesg(p_pdb->... */
    else
    {
        ICF_MEMFREE(p_glb_pdb, p_msg, ICF_MEM_COMMON,
                p_ecode, ret_val)
    }/* else */

    ICF_FUNCTION_EXIT(p_glb_pdb);

    return ret_val;
}/*End of Function icf_twc_send_merge_media_ind */



#if 0

/******************************************************************************
 *
 * FUNCTION NAME    :   icf_twc_fill_pa_msg_hdr
 *
 * DESCRIPTION  : This function fills the icf_api_header_st to send message 
 *                to PA
 *                
 *
 * RETURNS:  ICF_SUCCESS
 *
 *****************************************************************************/

icf_void_t icf_twc_fill_pa_msg_hdr(
        INOUT   icf_twc_pdb_st  *p_pdb,
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

    p_msg->hdr.api_length = sizeof(icf_api_header_st) 
        + sizeof(icf_cc_pa_conference_resp_st);

    p_msg->hdr.call_id = p_glb_pdb->p_call_ctx->call_id;

    ICF_FUNCTION_EXIT(p_glb_pdb)
}


/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_fill_pa_twc_resp
*
* DESCRIPTION  : This function fills the icf_cc_pa_conference_resp_st
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_twc_fill_pa_twc_resp(
        INOUT   icf_twc_pdb_st                 *p_pdb,
        INOUT   icf_cc_pa_conference_resp_st   *p_twc_resp,
        IN      icf_uint8_t                    resp,
        IN      icf_uint8_t                    error_cause
        )
{
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    icf_return_t     ret_val = ICF_SUCCESS;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*resp contains Success or Failure for conf request*/
    p_twc_resp->result = resp;

    /*error_cause in case of failure return for conf request*/
    p_twc_resp->error_cause = error_cause;

    ICF_FUNCTION_EXIT(p_glb_pdb)
}/*icf_twc_fill_pa_resume_resp*/
#endif

/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_check_med_for_merge_req
*
* DESCRIPTION  : This function is used to check media ongoing for both the
*                calls and if media isnot ongoing then send merge media req
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_return_t  icf_twc_check_med_for_merge_req(
        INOUT icf_twc_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    /*If media is going on in Call B*/
    if(((ICF_INT_MODULE_INVALID != 
            ICF_TWC_GET_MEDIA_ONGOING(p_pdb->p_twc_ctx->p_call_ctx_b))
            && (ICF_INT_MODULE_TWC != 
            ICF_TWC_GET_MEDIA_ONGOING(p_pdb->p_twc_ctx->p_call_ctx_b))))
    {
        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_MEDIA_ONGOING)
                
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        /*If start proc media received on the other call Set service request
         * for the other call*/        
        if(p_pdb->p_twc_ctx->p_call_ctx_b->call_id != 
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
            
            ICF_TWC_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)
        } /* p_pdb->p_internal_msg->msg_hdr... */

        ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_05_AWT_START_PROC_MEDIA)
    } /* ICF_TWC_GET_MEDIA_ONGOING(p... */
    /*If media is ongoing in Call C*/
    else if (((ICF_INT_MODULE_INVALID !=
            ICF_TWC_GET_MEDIA_ONGOING(p_pdb->p_twc_ctx->p_call_ctx_c))
            && (ICF_INT_MODULE_TWC != 
            ICF_TWC_GET_MEDIA_ONGOING(p_pdb->p_twc_ctx->p_call_ctx_c))))
    {
        ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_MEDIA_ONGOING)
        
        /*Set  start proc media*/
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ)

        /*If start proc media recieved for the other call set service request
         * for the other call*/
        if(p_pdb->p_twc_ctx->p_call_ctx_c->call_id != 
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
        {
            ret_val = 
                icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
            
            ICF_TWC_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)
        } /* p_pdb->p_internal_msg->msg_hdr... */
        
        ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_05_AWT_START_PROC_MEDIA)
    } /* ICF_TWC_GET_MEDIA_ONGOING(p... */
    /*If media is not onoing then send merge media request to MMI*/
    else if(ICF_FAILURE == icf_twc_send_merge_media_req(p_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* icf_twc_send_merge_media_re... */
    else
    {
        /*Set media events req in both the calls*/
        ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_01_AWT_MERGE_MEDIA_RESP)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* icf_twc_check_med_for_merge_req */


/*******************************************************************************
 *
 * FUNCTION NAME    :   icf_twc_send_merge_media_req
 *
 * DESCRIPTION  : This function sends Call merge Indication to MMI,
 *                which triggers Merge Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MERGE_MEDIA_SESSION_REQ.
 *               
 *
 * RETURNS:
 *
 *****************************************************************************/
icf_return_t  icf_twc_send_merge_media_req(
        INOUT  icf_twc_pdb_st    *p_pdb)
{
    icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
    icf_mmi_merge_request_st  merge_req;
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_call_ctx_st           *p_sec_call_ctx = ICF_NULL;
    icf_internal_list_st      *p_list = ICF_NULL;
    icf_stream_sdp_info_st    *p_stream_sdp = ICF_NULL;

    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_MERGE_MEDIA_SENT)

    /* Set media ongoing in TWC service in both service contexts */
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ)
    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

    /*Assign message id for MMI*/    
    p_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CREATE_CONF_REQ;

    p_pdb->p_internal_msg->msg_hdr.payload_length =
        sizeof(icf_mmi_merge_request_st);

    /*Assign second call id*/
    p_sec_call_ctx = icf_twc_get_other_call_ctx(p_pdb);
    merge_req.merge_call_id_2 = p_sec_call_ctx->call_id;

    merge_req.count = 1;

    p_list = p_glb_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) && 
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            merge_req.merge_stream[0].stream_id = 
                p_stream_sdp->stream_id;
            break;
        }
        p_list = p_list->p_next;
    }

    p_list = p_sec_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) && 
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            merge_req.merge_stream[0].merge_stream_id = 
                p_stream_sdp->stream_id;
            break;
        }
        p_list = p_list->p_next;
    }

    /*Assign merge_media_req to p_msg_data of internal message*/
    p_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&merge_req;

    /*Call entry point function of MMI*/
    ret_val = icf_mmi_process_mesg(p_pdb->p_internal_msg);

    if (ICF_SUCCESS == ret_val)
    {
        ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
    }
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_twc_send_merge_media_req */

/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_get_other_call_ctx
*
* DESCRIPTION  : This function is used to get the call id of the call other
*                then one in p_glb_pdb
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_call_ctx_st*   icf_twc_get_other_call_ctx(
        INOUT   icf_twc_pdb_st    *p_pdb)
{
    icf_call_ctx_st  *p_call_ctx = ICF_NULL;
    icf_glb_pdb_st   *p_glb_pdb = ICF_NULL;
    
    icf_return_t    ret_val = ICF_SUCCESS;
    
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    if(p_pdb->p_twc_ctx->p_call_ctx_b->call_id == 
            p_glb_pdb->p_call_ctx->call_id)
    {
        p_call_ctx = p_pdb->p_twc_ctx->p_call_ctx_c;
    } /* p_pdb->p_internal_msg->msg_hdr... */
    else
    {
        p_call_ctx = p_pdb->p_twc_ctx->p_call_ctx_b;
    } /* else */

	ret_val = ret_val;
    ICF_FUNCTION_EXIT(p_glb_pdb)

    return p_call_ctx;
} /* icf_twc_get_other_call_id */


/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_resp_to_pa_n_stop_timer
*
* DESCRIPTION  : This function is used to stop TWC timer and send failure
*                response to PA
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_resp_to_pa_n_stop_timer(
        INOUT    icf_twc_pdb_st    *p_pdb,
        IN       icf_error_t       error_id)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_FAILURE_CONF_RESP_SENT)
   
    /*In case of timer expiry no need to stop timer*/
    if(ICF_CAUSE_TIMER_EXPIRY != error_id)
    {
        /*Stop TWC timer and free timer block*/
        ret_val = icf_twc_stop_twc_timer(p_pdb);
    } /* if(ICF_CAUSE_TIMER_EXPIRY !... */
    
    if(ICF_SUCCESS == ret_val)
    {
        /*Send Conference success response to PA*/
        ret_val = icf_twc_send_adm_conference_resp(p_pdb, 
                (icf_boolean_t)(ICF_FAILURE), (icf_error_t)error_id);
    }

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* INOUT    icf_twc_pdb_st    ... */

/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_terminate_service
*
* DESCRIPTION  : If the parameter 'conf_fail_call_clear_not_reqd' is set by Application
*                and twc timer expires/merge media response is failure, then 
*                conference failure response is sent to Application with both calls 
*                remaining in their current state.
*                Else, error indication is sent to terminate both the calls
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_terminate_service(
        INOUT    icf_twc_pdb_st    *p_pdb,
        IN       icf_uint32_t       error_id)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_config_data_st      *p_config_data = ICF_NULL;
	icf_error_t             *p_ecode = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
  
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    /* Get the config data */
	ret_val = icf_dbm_get_module_glb_data(
					p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
					(icf_int_module_id_t)ICF_INT_MODULE_CFG,
					(icf_void_t *)&p_config_data, p_ecode);

    ICF_ASSERT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,\
                                   (ICF_NULL != p_config_data))

    if(ICF_TRUE != p_config_data->conf_fail_call_clear_not_reqd)
	{
		ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_BOTH_CALLS_ENDED)
	}    
    /*
     * This function stops TWC timer
     * and sends conference response to PA
     */
    ret_val = icf_twc_resp_to_pa_n_stop_timer(p_pdb, 
            (icf_error_t)error_id);

    /*If call context in p_glb_pdb is same as call context C then
     *set call context B in return payload other wise set call context C
     *in return payload*/
    if (ICF_SUCCESS == ret_val)
    {
        if(ICF_TRUE == p_config_data->conf_fail_call_clear_not_reqd)
		{
			ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        }
        else
		{
			ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
		}
        
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)
    
        if (p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id 
                == p_pdb->p_twc_ctx->p_call_ctx_c->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);
        } /* == p_pdb->p_twc_ctx->p_call_ct... */
        else
        {
            ret_val = 
                icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                        p_pdb->p_twc_ctx->p_call_ctx_c);
        } /* else */
    } /* if (ICF_SUCCESS == ret_val) */    

    ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_00_IDLE)

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /*icf_twc_terminate_service*/

/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_check_chr_service
*
* DESCRIPTION  : This function is used to check CHR service is active or not,
*                if yes then check media state, if local held then wait for 
*                resume response else wait for hold response. If HCR not active
*                then send merge media request
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_check_chr_service(
        INOUT    icf_twc_pdb_st    *p_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
    
    if(ICF_SERVICE_CHR &
            ((icf_service_ctx_st *)p_pdb->p_twc_ctx->p_call_ctx_b->
            p_service_ctx)->active_service_list.active_service_status)
    {
        /*If call is not held wait for call hold response
         * else wait for call resume response*/
        if(ICF_CALL_STATUS_LOCAL_HELD & 
                p_pdb->p_twc_ctx->p_call_ctx_b->call_status)
        {
            ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CRM)
            
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_RESUME_REQ)
           
            ICF_TWC_SET_NEXT_STATE(p_pdb, 
                    ICF_TWC_01_AWT_MERGE_MEDIA_RESP)
                
            ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_02_AWT_CALL_RESUME_RESP)
        } /* if(ICF_CALL_STATUS_LOCAL_HE... */
        else
        {
            ICF_TWC_TRACE(p_pdb, ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CHD)
            
            ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CALL_HOLD_REQ)
           
            ICF_TWC_SET_CURR_STATE(p_pdb, ICF_TWC_04_AWT_CALL_HOLD_RESP)
        } /* else */

        /*If call hold request to be send for the call other then
         * present call context*/
        if(p_pdb->p_twc_ctx->p_call_ctx_b->call_id != 
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id)
        {
            ret_val = icf_twc_fill_other_call_info(p_pdb->p_internal_msg,
                    p_pdb->p_twc_ctx->p_call_ctx_b);

            ICF_TWC_SET_STATUS(p_pdb, 
                    ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL)
        } /* p_pdb->p_internal_msg->msg_hdr... */
    } /* active_service_list.active_ser... */
    /*Check if media is ongiong for any of two calls*/
    else
    {
        ret_val = icf_twc_check_med_for_merge_req(p_pdb);
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)

    return ret_val;
} /* INOUT    icf_twc_pdb_st    ... */


/******************************************************************************
*
* FUNCTION NAME:   icf_twc_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize TWC context data
*
* RETURNS:
******************************************************************************/

icf_void_t  icf_twc_deinit_ctx(
        icf_twc_pdb_st  *p_pdb
        )
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_error_t   *p_ecode = ICF_NULL;
    icf_glb_pdb_st  *p_glb_pdb = ICF_NULL;

    ICF_FUNCTION_ENTER(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
  
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    p_ecode = &(p_pdb->p_internal_msg->msg_hdr.ecode);

    ICF_TWC_RESET_NEXT_STATE(p_pdb)
    
    p_pdb->p_twc_ctx->state = ICF_TWC_00_IDLE;

    if (0 != p_pdb->p_twc_ctx->twc_timer_id)
    {
        ICF_STOP_TIMER(p_glb_pdb,
                    p_pdb->p_twc_ctx->twc_timer_id, p_ecode, ret_val)
    } /* if(0 != p_pdb->p_twc_ctx->twc_... */
    
    if (ICF_NULL != p_pdb->p_twc_ctx->p_twc_timer_data)
    {
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                (icf_void_t *)(p_pdb->p_twc_ctx->p_twc_timer_data),
                p_ecode);

        p_pdb->p_twc_ctx->p_twc_timer_data = ICF_NULL;
    }

    p_pdb->p_twc_ctx->p_call_ctx_b = ICF_NULL;

    p_pdb->p_twc_ctx->p_call_ctx_c = ICF_NULL;

    p_pdb->p_twc_ctx->p_call_ctx_req = ICF_NULL;

    ICF_FUNCTION_EXIT(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb)
} /* icf_chr_deinit_ctx */


/******************************************************************************
*
* FUNCTION NAME:   icf_twc_fill_other_call_info
*
* DESCRIPTION  : This function is used to allocate return payload and 
*                set call ctx pointer in it
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_twc_fill_other_call_info(
        INOUT   icf_internal_msg_st   *p_internal_msg,
        INOUT      icf_call_ctx_st       *p_call_ctx)
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
    p_internal_msg->p_msg_data = (icf_void_t*)p_msg_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_twc_fill_other_call_info */


/******************************************************************************
*
* FUNCTION NAME:   icf_twc_trace
*
* DESCRIPTION  : This function is used to define the trace data for TWC service
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
#ifdef ICF_TRACE_ENABLE
icf_void_t    icf_twc_trace(
        INOUT icf_twc_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    /*  SPR 18064 Merge */
    icf_trace_data_st      trace_data;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */

       switch(trace_id)
    {
        case ICF_TRACE_TWC_RECVD:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            /*Second Call ID*/
            trace_data.int_val[1] = 
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TRACE_MERGE_MEDIA_RESP:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            /*Second Call ID*/
            trace_data.int_val[1] = 
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;
            
        case ICF_TWC_TRACE_MEDIA_NOT_COMMITTED:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;
            
        case ICF_TWC_TRACE_CALL_LOCAL_HOLD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            break;

        case ICF_TWC_TRACE_END_OTHER_CALL:
            {
                icf_call_ctx_st *p_call_ctx = ICF_NULL;
                trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
                trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
                /*First call ID*/
                p_call_ctx = icf_twc_get_other_call_ctx(p_pdb);
                trace_data.int_val[0] = p_call_ctx->call_id;
            }
            break;

        case ICF_TWC_TRACE_TERMINATE_SERVICE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            /*Second call ID*/
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TWC_TRACE_MEDIA_ONGOING:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TWC_TRACE_MERGE_MEDIA_SENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            /*Second call ID*/
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TWC_TRACE_FAILURE_CONF_RESP_SENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            /*Second call ID*/
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TWC_TRACE_BOTH_CALLS_ENDED:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            /*Second call ID*/
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        case ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CHD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            break;
        
        case ICF_TWC_TRACE_CHR_SERVICE_ACTIVE_CRM:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            break;

        case ICF_TRACE_START_TIMER:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_STOP_TIMER:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_CURR_RET_STATUS:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_internal_msg->msg_hdr.status;
            break;

        case ICF_TRACE_TWC_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_twc_ctx->state;
            icf_twc_state_trace_string(p_pdb->p_twc_ctx->state, &trace_data);
            break;
            
        case ICF_TRACE_TWC_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_twc_ctx->event;
            trace_data.int_val[1] = 
                (icf_uint32_t)
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id;
            icf_twc_event_trace_string(p_pdb->p_twc_ctx->event, &trace_data);
            break;

        case ICF_TRACE_NEXT_STATE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            trace_data.int_val[0] = 
                (icf_uint32_t)p_pdb->p_twc_ctx->next_state;
            break;

        case ICF_TWC_TRACE_INVALID_SERVICE_SCENARIO:
             trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            /*First call ID*/
            trace_data.int_val[0] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_b->call_id;
            
            /*Second call ID*/
            trace_data.int_val[1] =
                (icf_uint32_t)p_pdb->p_twc_ctx->p_call_ctx_c->call_id;
            break;

        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;
    } /* switch(trace_id) */       

    ICF_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
            trace_id, &trace_data)
} /* IN    icf_trace_id_t     tr... */


/******************************************************************************
*
* FUNCTION NAME:   icf_twc_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for TWC service
*
*
* RETURNS:  
*
******************************************************************************/
icf_return_t    icf_twc_app_trace(
        INOUT icf_twc_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id)
{
    icf_trace_data_st      trace_data;
    icf_trace_criteria_data_st    *p_criteria_data = p_pdb->p_internal_msg->
                           msg_hdr.p_glb_pdb->p_call_ctx->p_trace_criteria_data;
    /* Fix for CSR1-5249829 */
    icf_return_t ret_val = ICF_SUCCESS;

    ret_val = icf_cmn_check_app_trace(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb,
                                        trace_id,
					p_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
      return  ret_val;
    }
    else
    {

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.app_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->app_id;

    p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_glb_data->app_trace_info.call_id =
                p_pdb->p_internal_msg->msg_hdr.p_glb_pdb->p_call_ctx->call_id; 
    

    memset((void *)&trace_data,0,sizeof(icf_trace_data_st)); 
    switch(trace_id)
    {

         case ICF_TRACE_CONFERENCE_REQ:

	    trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;

            trace_data.int_val[0] = p_pdb->merge_call_id;

	    trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;
     
        case ICF_TRACE_CONFERENCE_RESP:

            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;

            trace_data.int_val[0] = p_pdb->result;

            trace_data.int_val[1] = p_pdb->error_cause;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
                           
            break;

        
          case ICF_TRACE_CONF_TIMER_EXPIRE:

            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

            break;


         default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* switch(trace_id) */

    ICF_APP_TRACE(p_pdb->p_internal_msg->msg_hdr.p_glb_pdb, trace_id, 
                     &trace_data, p_criteria_data)
}
    return ret_val;

} /* IN    icf_trace_id_t     tr... */

/******************************************************************************
*
* FUNCTION NAME    :   icf_twc_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_twc_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data)
{

    /*Switch for trace_id*/
    switch(state)
    {
        case ICF_TWC_00_IDLE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_00_IDLE");
            break;
            
        case ICF_TWC_01_AWT_MERGE_MEDIA_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_01_AWT_MERGE_MEDIA_RESP");
            break;

        case ICF_TWC_02_AWT_CALL_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_02_AWT_CALL_RESUME_RESP");
            break;

        case ICF_TWC_03_AWT_MEDIA_COMMITTED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_03_AWT_MEDIA_COMMITTED");
            break;

        case ICF_TWC_04_AWT_CALL_HOLD_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_04_AWT_CALL_HOLD_RESP");
            break;

        case ICF_TWC_05_AWT_START_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_05_AWT_START_PROC_MEDIA");
            break;

        case ICF_TWC_06_CONFERENCE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_06_CONFERENCE");
            break;
    } /* switch(trace_id) */
    
}/*icf_twc_state_trace_string*/

/******************************************************************************
*
* FUNCTION NAME    :   icf_twc_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_twc_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data)
{
    switch(event)
    {
        case ICF_TWC_CREATE_CONFERENCE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_CREATE_CONFERENCE");
            break;

        case ICF_TWC_MERGE_MEDIA_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_MERGE_MEDIA_RESP");
            break;

        case ICF_TWC_CALL_RESUME_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_CALL_RESUME_RESP");
            break;

        case ICF_TWC_MEDIA_COMMITTED:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_MEDIA_COMMITTED");
            break;

        case ICF_TWC_CHD_RESP:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_CHD_RESP");
            break;

        case ICF_TWC_START_PROC_MEDIA:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_START_PROC_MEDIA");
            break;

        case ICF_TWC_CALL_RELEASE:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_CALL_RELEASE");
            break;

        case ICF_TWC_TIMER_EXPIRY:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_TIMER_EXPIRY");
            break;

        case ICF_TWC_MM_ERR_IND:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_MM_ERR_IND");
            break;

        case ICF_TWC_SERVICE_ERR_IND:
            icf_port_strcpy(p_trace_data->string_val[0].str,
                    (icf_uint8_t *)"ICF_TWC_SERVICE_ERR_IND");
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
* FUNCTION NAME: icf_twc_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_twc_clone_app_spec_hdr_lists(
    icf_twc_pdb_st   *p_twc_pdb)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_glb_pdb_st *p_glb_pdb = p_twc_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_pa_cc_conference_req_st *p_twc_msg =
        (icf_pa_cc_conference_req_st *)&(p_glb_pdb->p_recv_msg->payload[0]);
    /*SPR 19590 -If any of the header/body/tag lists are present in the
      TWC request then we shall increment the txn id in call ctx
      so that a unique node is created for the hdr/body/tag list
      in their respective lists of list in call ctx. Additionally
     we should always increment the last_txn_id which is global for 
     call and then copy the same in txn_id because txn_id is for the
     current node and not necessary its next number is unused.*/
    if((ICF_HDR_LIST_PRESENT & p_twc_msg->bit_mask) ||
       (ICF_TAG_LIST_PRESENT & p_twc_msg->bit_mask) ||    
       (ICF_BODY_LIST_PRESENT & p_twc_msg->bit_mask))
    {
        if(ICF_NULL != p_glb_pdb->p_call_ctx)
        {    
            p_glb_pdb->p_call_ctx->last_txn_id++;
            p_glb_pdb->p_call_ctx->txn_id = 
               p_glb_pdb->p_call_ctx->last_txn_id;
        }   
    }

    /* clone the header list into call ctx, if received from app */
    if (ICF_HDR_LIST_PRESENT & p_twc_msg->bit_mask)
    {
         /*SPR 19590 change start */
        ret_val = icf_cmn_make_hdr_list_node(
                     p_glb_pdb,
                     &(p_twc_msg->header_list));
        /*SPR 19590 change end */

    }

    /* clone the tag list into call ctx, if received from app */
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_TAG_LIST_PRESENT & p_twc_msg->bit_mask))
    {
         /*SPR 19590 change start */
        ret_val = icf_cmn_make_tag_list_node(
                     p_glb_pdb,
                     &(p_twc_msg->tag_list));
         /*SPR 19590 change end */
    }
    return ret_val;
}

#endif

