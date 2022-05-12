/*******************************************************************************
*
* FILE NAME    : icf_twc_main.c
*
* DESCRIPTION  : Contains the definitions of TWC service entry point function
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 12-Jan-2004    Amit Sharma     None            Initial
* 16-Feb-2005    Amit Sharma     None            Added deinit function
* 25-Feb-2005  Amit Sharma                    Added handling for forceful 
*                                               call clear
* 21-Apr-2005  Amit Sharma                    Updated handling of timer_expiry
*                                             to incorporate changes of SCM
* 29-Apr-2005  Amit Sharma     SPR:7461      Handling of call_rel in idle state
* 19-May-2005  Amit Sharma     SPR:7568      Handling of forceful call clear
*                                            deinit_ctx not called in ret_val
*                                            failure
* 27-Feb-2006  Amit Sharma                  Merged code for ICF
*
* Copyright 2006,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_twc_types.h"
#include "icf_twc_defs.h"
#include "icf_twc_prototypes.h"
#include "icf_twc_macro.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_init 
*
* DESCRIPTION  : This is the initialization function of TWC service module
*                which is called by DBM at the time of system startup
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_twc_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_twc_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_twc_data_st         *p_twc_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t             state_count = 0;
    icf_uint8_t             event_count = 0;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_TWC)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
             sizeof(icf_twc_data_st),
            p_twc_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Initialize all funciton pointers with icf_twc_unexpected_event*/
    for (state_count = 0; state_count < ICF_TWC_MAX_STATE; state_count++)
    {
        for (event_count = 0; event_count < ICF_TWC_MAX_EVENT; 
                event_count++)
        {
            p_twc_data->twc_fsm_table[state_count][event_count] = 
                icf_twc_unexpected_event;
        } /* event_count++) */
    } /* for (state_count = 0; state_co... */
    

    /*Assignment of function pointers of FSM*/
    /*ICF_TWC_00_IDLE*/
    p_twc_data->twc_fsm_table[ICF_TWC_00_IDLE][ICF_TWC_CREATE_CONFERENCE]
        = icf_twc_00_create_conference;

    p_twc_data->twc_fsm_table[ICF_TWC_00_IDLE]
        [ICF_TWC_CALL_RELEASE] = icf_twc_00_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_MERGE_MEDIA_RESP] = icf_twc_01_merge_media_resp;

    /*Call resume response received in awt cal resume response*/
    p_twc_data->twc_fsm_table[ICF_TWC_02_AWT_CALL_RESUME_RESP]
        [ICF_TWC_CALL_RESUME_RESP] = icf_twc_02_call_resume_resp;

    p_twc_data->twc_fsm_table[ICF_TWC_03_AWT_MEDIA_COMMITTED]
        [ICF_TWC_MEDIA_COMMITTED] = icf_twc_03_media_committed;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_MEDIA_COMMITTED] = icf_twc_05_awt_start_proc_media;
    
    p_twc_data->twc_fsm_table[ICF_TWC_04_AWT_CALL_HOLD_RESP]
        [ICF_TWC_CHD_RESP] = icf_twc_04_call_hold_resp;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_START_PROC_MEDIA] = icf_twc_05_awt_start_proc_media;

    p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_CALL_RELEASE] = icf_twc_01_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_02_AWT_CALL_RESUME_RESP]
        [ICF_TWC_CALL_RELEASE] = icf_twc_xx_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_03_AWT_MEDIA_COMMITTED]
        [ICF_TWC_CALL_RELEASE] = icf_twc_xx_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_04_AWT_CALL_HOLD_RESP]
        [ICF_TWC_CALL_RELEASE] = icf_twc_xx_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_CALL_RELEASE] = icf_twc_xx_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_06_CONFERENCE]
        [ICF_TWC_CALL_RELEASE] = icf_twc_06_call_release;

    p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_MM_ERR_IND] = icf_twc_01_mm_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_TIMER_EXPIRY] = icf_twc_01_timer_expiry;

    p_twc_data->twc_fsm_table[ICF_TWC_02_AWT_CALL_RESUME_RESP]
        [ICF_TWC_TIMER_EXPIRY] = icf_twc_xx_timer_expiry;

    p_twc_data->twc_fsm_table[ICF_TWC_03_AWT_MEDIA_COMMITTED]
        [ICF_TWC_TIMER_EXPIRY] = icf_twc_xx_timer_expiry;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_TIMER_EXPIRY] = icf_twc_xx_timer_expiry;

    p_twc_data->twc_fsm_table[ICF_TWC_04_AWT_CALL_HOLD_RESP]
        [ICF_TWC_TIMER_EXPIRY] = icf_twc_xx_timer_expiry;

    p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_02_AWT_CALL_RESUME_RESP]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_03_AWT_MEDIA_COMMITTED]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_04_AWT_CALL_HOLD_RESP]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

    p_twc_data->twc_fsm_table[ICF_TWC_06_CONFERENCE]
        [ICF_TWC_SERVICE_ERR_IND] = icf_twc_xx_service_err_ind;

   p_twc_data->twc_fsm_table[ICF_TWC_00_IDLE]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear; 

     p_twc_data->twc_fsm_table[ICF_TWC_01_AWT_MERGE_MEDIA_RESP]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_01_forceful_call_clear;

    p_twc_data->twc_fsm_table[ICF_TWC_02_AWT_CALL_RESUME_RESP]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear;

    p_twc_data->twc_fsm_table[ICF_TWC_03_AWT_MEDIA_COMMITTED]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear;

    p_twc_data->twc_fsm_table[ICF_TWC_05_AWT_START_PROC_MEDIA]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear;

    p_twc_data->twc_fsm_table[ICF_TWC_04_AWT_CALL_HOLD_RESP]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear;

    p_twc_data->twc_fsm_table[ICF_TWC_06_CONFERENCE]
        [ICF_TWC_FORCEFUL_CALL_CLEAR] = icf_twc_xx_forceful_call_clear;

    /*********End of Function pointer assignment*************/

    /*Assign fsm table pointer to global data pointer*/
    *p_p_twc_data = (icf_void_t *)p_twc_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
} /* icf_twc_init */


/******************************************************************************
*
* FUNCTION NAME    : icf_twc_process_mesg
*
* DESCRIPTION  : This is the Entry Point function of TWC service module
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_twc_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_twc_pdb_st          twc_pdb, *p_pdb = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;
    icf_twc_data_st         *p_twc_data = ICF_NULL;
    icf_uint8_t             event;
    /*icf_twc_fsm_func_ptr_t  **p_twc_fsm_table;*/

    /*
     * Copy p_glb_pdb
     * */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_TWC)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Copy ecode pointer
     * */
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    p_pdb = &twc_pdb;
    p_pdb->conf_internal_req = ICF_FALSE;
    
    /*
     * Copy internal msg pointer to p_pdb
     */
    p_pdb->p_internal_msg = p_internal_msg;

    /*
     * Convert the incoming API to the Event
     */
    ret_val = icf_twc_convert_api_to_event(p_pdb, &event);

    ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

    /*
     * Call appropriate State machine function based on
     * State and event
     */
    if(ICF_EVENT_UNDEFINED != event)
    {        
        /*
         * Copy service context pointer to p_pdb
         */
        p_pdb->p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_service_ctx))

        /*
         * Copy TWC context pointer to p_pdb
         */
        p_pdb->p_twc_ctx = 
        (icf_twc_ctx_st *)p_pdb->p_service_ctx->
        active_service_list.p_sl_ctx[ICF_INT_MODULE_TWC - 
        ICF_INT_MODULE_SL_START - 1];

        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pdb->p_twc_ctx))

        p_pdb->p_twc_ctx->event = event;
       
        /*Call DBM function to get TWC global data pointer*/
        ret_val = 
            icf_dbm_get_module_glb_data(p_internal_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_TWC), 
                    (icf_void_t *)&p_twc_data, p_ecode);

        /*Call FSM function for this event and state*/
        if(ICF_FAILURE != ret_val)
        {
            ICF_TWC_TRACE(p_pdb, ICF_TRACE_TWC_STATE)
            
            ICF_TWC_TRACE(p_pdb, ICF_TRACE_TWC_EVENT)

            ret_val = p_twc_data->twc_fsm_table[p_pdb->p_twc_ctx->state]
                [p_pdb->p_twc_ctx->event](p_pdb);
        } /* if(ICF_FAILURE != ret_val) */       
    } /* if(ICF_FAILURE != ret_val) */

    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if (ICF_FAILURE == ret_val)
    {
        /*
         * Set status as service ended error Reset service continue
         * status
         */
        ICF_TWC_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE) 
        
        ICF_TWC_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR
                | ICF_STATUS_SERVICE_APPLY_BOTH_CALLS)

        ICF_TWC_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        /*icf_twc_deinit_ctx(p_pdb);*/

    }
    else if(ICF_TWC_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_TWC_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_TWC_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_TWC_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_twc_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */    
    else if(ICF_TWC_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        /*
         * If normal then reset service continue 
         */
        ICF_TWC_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_TWC_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)

        icf_twc_deinit_ctx(p_pdb);        
    } /* else if(ICF_STATUS_SERVICE_... */   
    else
    {        
        ICF_TWC_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
            
        ICF_TWC_TRACE(p_pdb, ICF_TRACE_TWC_STATE)
        
        ICF_TWC_TRACE(p_pdb, ICF_TRACE_TWC_EVENT)
    } /* else */
    
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return ret_val;
} /* icf_twc_process_msg */


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_convert_api_to_event
*
* DESCRIPTION  : This function converts incoming APIs to internal EVENTs
*
* RETURNS:
*******************************************************************************/
icf_return_t  icf_twc_convert_api_to_event(
        INOUT  icf_twc_pdb_st   *p_pdb,
        INOUT  icf_uint8_t      *p_event
        )
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;

    /*Copy p_glb_pdb*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)


    /*Convert APIs(Msg-IDs receved) to EVENT*/
    switch(p_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_USER_CALL_CONF:            
#ifdef ICF_TRACE_ENABLE
            p_pdb->merge_call_id=((icf_pa_cc_conference_req_st *)
            p_glb_pdb->p_recv_msg->payload)->merge_call_id;
#endif            
	    ICF_TWC_APP_TRACE(p_pdb, ICF_TRACE_CONFERENCE_REQ)

            *p_event = ICF_TWC_CREATE_CONFERENCE;
            break;

        case ICF_CREATE_CONF_RESP:
            *p_event = ICF_TWC_MERGE_MEDIA_RESP;
            break;

        case ICF_SC_INTERNAL_CALL_RESUME_RESP:
            *p_event = ICF_TWC_CALL_RESUME_RESP;
            break;

        case ICF_SC_INTERNAL_MEDIA_COMMIT:
            *p_event = ICF_TWC_MEDIA_COMMITTED;
            break;

        case ICF_SC_INTERNAL_CALL_HOLD_RESP:
            *p_event = ICF_TWC_CHD_RESP;
            break;

        case ICF_START_PROC_MEDIA:
            *p_event = ICF_TWC_START_PROC_MEDIA;
            break;

        case ICF_CC_CALL_RELEASED:
            *p_event = ICF_TWC_CALL_RELEASE;
            break;

        case ICF_TIMER_EXPIRY_IND:
            /* Added to send traces */
            ICF_TWC_APP_TRACE(p_pdb, ICF_TRACE_CONF_TIMER_EXPIRE) 

            *p_event = ICF_TWC_TIMER_EXPIRY;
            break;

        case ICF_MEDIA_ERROR_IND:
            *p_event = ICF_TWC_MM_ERR_IND;
            break;

        case ICF_SC_SERVICE_ERROR_IND :
            *p_event = ICF_TWC_SERVICE_ERR_IND;
            break;

        case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_TWC_FORCEFUL_CALL_CLEAR;
            break;
        	
        case ICF_INVOKE_TWC_FOR_JOIN:
             p_pdb->conf_internal_req = ICF_TRUE;
	     *p_event = ICF_TWC_CREATE_CONFERENCE;
             break;

        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_twc_convert_api_to_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_twc_deinit
*
* DESCRIPTION   : This is the deinit function of TWC service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_twc_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        INOUT     icf_void_t            *p_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
     ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_data,
            p_ecode,
            ret_val)

     ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_twc_deinit */
#endif

