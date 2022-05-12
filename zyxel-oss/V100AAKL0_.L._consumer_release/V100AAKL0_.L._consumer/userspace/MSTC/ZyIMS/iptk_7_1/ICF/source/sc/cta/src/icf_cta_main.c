/*******************************************************************************
*
* FILE NAME    : icf_cta_main.c
*
* DESCRIPTION  : Contains the definitions of CTA service entry point functions
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 23-Feb-2005    Amit Sharma     None            Initial
* 08-Mar-2005    Amit Sharma     None            Incorporated review comments
* 30-Mar-2005    Amit Sharma                    Update timer expiry handling
* 10-May-2005    Amit Sharma     SPR:7512        PA call termination is
*                                                buffered even if REFER has not
*                                                been sent to network yet
* 19-May-2005    Amit Sharma     SPR:7568        forceful call clear handled in
*                                                IDLE state
* 19-May-2005    Amit Sharma     SPR:7568        Change in handling of ret_val 0
* 02-Jun-2005    Amit Sharma     SPR:7535        Wait for TWC service term
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006,Aricent .
*******************************************************************************/
/*Include Header Files */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cta_types.h"
#include "icf_cta_macro.h"
#include "icf_cta_defs.h"
#include "icf_cta_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_init
*
* DESCRIPTION   : This is the initialization function of CTA service module
*                 which is called by DBM at the time of system startup
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_cta_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_cta_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_cta_data_st         *p_cta_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t             state_count = 0;
    icf_uint8_t             event_count = 0;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CTA)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
             sizeof(icf_cta_data_st),
            p_cta_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Initialize all funciton pointers with icf_cta_unexpected_event*/
    for (state_count = 0; state_count < ICF_CTA_MAX_STATE; state_count++)
    {
        for (event_count = 0; event_count < ICF_CTA_MAX_EVENT; 
                event_count++)
        {
            p_cta_data->cta_fsm_table[state_count][event_count] = 
                icf_cta_unexpected_event;
        } /* event_count++) */
    } /* for (state_count = 0; state_co... */

    
    /* ICF_CTA_CALL_TRANSFER_UNATTENDED_REQUEST */
    p_cta_data->cta_fsm_table[ICF_CTA_00_IDLE]
        [ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST]
        = icf_cta_00_call_transfer_attended_request;

    p_cta_data->cta_fsm_table[ICF_CTA_00_IDLE]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_xx_forceful_call_clear;

   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_CALL_RESUME_RESP]
        = icf_cta_01_call_resume_resp;

   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_CALL_HOLD_RESP]
        = icf_cta_01_call_hold_resp;

   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_CC_CALL_RELEASED]
        = icf_cta_01_cc_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_PA_CALL_RELEASED]
        = icf_cta_xx_pa_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_TIMER_EXPIRY]
        = icf_cta_xx_timer_expiry;
   
   p_cta_data->cta_fsm_table[ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_xx_forceful_call_clear;

   p_cta_data->cta_fsm_table[ICF_CTA_02_WAIT_FOR_START_PROC_SIG]
        [ICF_CTA_START_PROC_SIG]
        = icf_cta_02_start_proc_sig;

   p_cta_data->cta_fsm_table[ICF_CTA_02_WAIT_FOR_START_PROC_SIG]
        [ICF_CTA_CC_CALL_RELEASED]
        = icf_cta_02_cc_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_02_WAIT_FOR_START_PROC_SIG]
        [ICF_CTA_PA_CALL_RELEASED]
        = icf_cta_xx_pa_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_02_WAIT_FOR_START_PROC_SIG]
        [ICF_CTA_TIMER_EXPIRY]
        = icf_cta_xx_timer_expiry;

   p_cta_data->cta_fsm_table[ICF_CTA_02_WAIT_FOR_START_PROC_SIG]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_xx_forceful_call_clear;

   p_cta_data->cta_fsm_table[ICF_CTA_03_WAIT_FOR_REFER_RESP]
        [ICF_CTA_REFER_RESP]
        = icf_cta_03_refer_resp;

   p_cta_data->cta_fsm_table[ICF_CTA_03_WAIT_FOR_REFER_RESP]
        [ICF_CTA_CC_CALL_RELEASED]
        = icf_cta_xx_cc_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_03_WAIT_FOR_REFER_RESP]
        [ICF_CTA_PA_CALL_RELEASED]
        = icf_cta_xx_pa_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_03_WAIT_FOR_REFER_RESP]
        [ICF_CTA_TIMER_EXPIRY]
        = icf_cta_03_timer_expiry;

   p_cta_data->cta_fsm_table[ICF_CTA_03_WAIT_FOR_REFER_RESP]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_03_forceful_call_clear;

   p_cta_data->cta_fsm_table[ICF_CTA_04_WAIT_FOR_NOTIFY]
        [ICF_CTA_NOTIFY]
        = icf_cta_04_notify;

    p_cta_data->cta_fsm_table[ICF_CTA_04_WAIT_FOR_NOTIFY]
        [ICF_CTA_CC_CALL_RELEASED]
        = icf_cta_xx_cc_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_04_WAIT_FOR_NOTIFY]
        [ICF_CTA_PA_CALL_RELEASED]
        = icf_cta_xx_pa_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_04_WAIT_FOR_NOTIFY]
        [ICF_CTA_TIMER_EXPIRY]
        = icf_cta_xx_timer_expiry;

   p_cta_data->cta_fsm_table[ICF_CTA_04_WAIT_FOR_NOTIFY]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_xx_forceful_call_clear;

   p_cta_data->cta_fsm_table[ICF_CTA_05_WAIT_FOR_TWC_RESP]
        [ICF_CTA_TWC_RESP]
        = icf_cta_05_twc_resp;

   p_cta_data->cta_fsm_table[ICF_CTA_05_WAIT_FOR_TWC_RESP]
        [ICF_CTA_CC_CALL_RELEASED]
        = icf_cta_xx_cc_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_05_WAIT_FOR_TWC_RESP]
        [ICF_CTA_PA_CALL_RELEASED]
        = icf_cta_xx_pa_call_released;

   p_cta_data->cta_fsm_table[ICF_CTA_05_WAIT_FOR_TWC_RESP]
        [ICF_CTA_TIMER_EXPIRY]
        = icf_cta_xx_timer_expiry;
   
   p_cta_data->cta_fsm_table[ICF_CTA_05_WAIT_FOR_TWC_RESP]
        [ICF_CTA_FORCEFUL_CALL_CLEAR]
        = icf_cta_xx_forceful_call_clear;
   /*********End of Function pointer assignment*************/

    /*Assign fsm table pointer to global data pointer*/
    *p_p_cta_data = (icf_void_t *)p_cta_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;

} /* icf_cta_init */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_process_mesg
*
* DESCRIPTION   : This is the Entry Point function of CTA service module
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_cta_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_cta_pdb_st          cta_pdb, *p_pdb = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;
    icf_cta_data_st         *p_cta_data = ICF_NULL;
    icf_uint8_t             event = 0;
   
    /* Copy p_glb_pdb */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CTA)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Copy ecode pointer */   
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    p_pdb = &cta_pdb;

    p_pdb->p_internal_msg = p_internal_msg;
    
    /* Convert the incoming API to the Event */
    ret_val = icf_cta_convert_api_to_event(p_pdb, &event);

    if (ICF_EVENT_UNDEFINED != event)
    {
        /* Copy call context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))
        
        p_pdb->p_call_ctx = p_glb_pdb->p_call_ctx;

        /* Copy service context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, 
                (ICF_NULL != p_pdb->p_call_ctx->p_service_ctx))
            
        p_pdb->p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

        /* Copy CTA context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != 
                    (icf_cta_ctx_st *)p_pdb->p_service_ctx->
                    active_service_list.p_sl_ctx[ICF_INT_MODULE_CTA 
                    - ICF_INT_MODULE_SL_START - 1]))

        p_pdb->p_cta_ctx =
        (icf_cta_ctx_st *)p_pdb->p_service_ctx->
        active_service_list.p_sl_ctx[ICF_INT_MODULE_CTA - 
        ICF_INT_MODULE_SL_START - 1];

    /* Changes for XCONF */
    /*changed the msg_id to  ICF_INTERNAL_REFER_W_REP_REQ as for
     * add_party_request in XCONF we do REFER with replaces and same is in
     * SCM. Also we set flag to further identify XCONF scenario*/   
    if(p_pdb->p_internal_msg->msg_hdr.msg_id == ICF_INTERNAL_REFER_W_REP_REQ)
     {
         p_pdb->p_cta_ctx->internal_refer_req = ICF_TRUE;
     }
    else if(p_pdb->p_internal_msg->msg_hdr.msg_id == ICF_USER_CALL_TRANSFER_ATTENDED)
     {
          p_pdb->p_cta_ctx->internal_refer_req = ICF_FALSE;
     }
    /*Changes for XCONF End */

        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        
        /*Call DBM function to get CTA global data pointer*/
        ret_val =
            icf_dbm_get_module_glb_data(p_internal_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_CTA),
                    (icf_void_t *)&p_cta_data, p_ecode);


        if(ICF_FAILURE != ret_val)
        {
            p_pdb->p_cta_ctx->event = event;
            
            ICF_CTA_TRACE(p_pdb, ICF_TRACE_CTA_STATE)

            ICF_CTA_TRACE(p_pdb, ICF_TRACE_CTA_EVENT)

            /* Call appropriate State machine function based on
             * State and event */
            ret_val = p_cta_data->cta_fsm_table[p_pdb->p_cta_ctx->state]
                [p_pdb->p_cta_ctx->event](p_pdb);
        } /* if(ICF_FAILURE != ret_val) */
    } /* if(ICF_FAILURE != ret_val) */

    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if (ICF_FAILURE == ret_val)
    {
        /* Set status as service ended error Reset service continue status */
        ICF_CTA_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_CTA_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CTA_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_cta_deinit_ctx(p_pdb);
    }
    else if(ICF_CTA_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_CTA_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_CTA_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_CTA_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_cta_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else if(ICF_CTA_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        /*
         * If normal then reset service continue
         */
        ICF_CTA_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_CTA_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)
        
        icf_cta_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else
    {
        ICF_CTA_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
        
        ICF_CTA_TRACE(p_pdb, ICF_TRACE_CTA_STATE)
        
        ICF_CTA_TRACE(p_pdb, ICF_TRACE_CTA_EVENT)
    } /* else */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
} /* icf_cta_process_msg */

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_convert_api_to_event
*
* DESCRIPTION   : This function converts incoming APIs to internal events
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t  icf_cta_convert_api_to_event(
        INOUT  icf_cta_pdb_st   *p_pdb,
        INOUT  icf_uint8_t      *p_event
        )
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;

    /*Copy p_glb_pdb*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_event))

    switch(p_pdb->p_internal_msg->msg_hdr.msg_id)
    {
	/* Changes For XCONF - ADD party request case*/
	case ICF_INTERNAL_REFER_W_REP_REQ:
	/* Changes for XCONF End */
        case ICF_USER_CALL_TRANSFER_ATTENDED:
            *p_event = ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST;
            break;

        case ICF_SC_INTERNAL_CALL_RESUME_RESP:
            *p_event = ICF_CTA_CALL_RESUME_RESP;
            break;

        case ICF_SC_INTERNAL_CALL_HOLD_RESP:
            *p_event = ICF_CTA_CALL_HOLD_RESP;
            break;

        case ICF_NW_INC_REFER_RESP:
            *p_event = ICF_CTA_REFER_RESP;
            break;
            
        case ICF_START_PROC_SIGNALING:
            *p_event = ICF_CTA_START_PROC_SIG;
            break;
            
        case ICF_CC_CALL_RELEASED:
            *p_event = ICF_CTA_CC_CALL_RELEASED;
            break;

        case ICF_USER_CALL_TERMINATE:
             *p_event = ICF_CTA_PA_CALL_RELEASED;
            break;

        case ICF_NW_INC_NOTIFY_IND:
             *p_event = ICF_CTA_NOTIFY;
            break;

        case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_CTA_FORCEFUL_CALL_CLEAR;
            break;
        
	/*  CHanges for XCONF */
	case ICF_INTERNAL_ABORT_REFER_REQ:    
        case ICF_TIMER_EXPIRY_IND:
            ICF_CTA_APP_TRACE(p_pdb, ICF_TRACE_CTA_TIMER_EXPIRE)

                *p_event = ICF_CTA_TIMER_EXPIRY;
            break;
	/*  CHANGES FOR xconf Ended */

        case ICF_SC_INTERNAL_TWC_RESP:
                *p_event = ICF_CTA_TWC_RESP;
            break;

        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_cta_convert_api_to_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_deinit
*
* DESCRIPTION   : This is the deinit function of CTA service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_cta_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        INOUT   icf_void_t            *p_data,
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
} /* icf_cta_deinit */

#endif
