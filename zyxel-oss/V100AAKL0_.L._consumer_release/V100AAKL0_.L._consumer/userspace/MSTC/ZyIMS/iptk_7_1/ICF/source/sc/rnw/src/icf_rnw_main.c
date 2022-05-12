/******************************************************************************
*
* FILE NAME    : icf_rnw_main.c
*
* DESCRIPTION  : Contains the definitions of RNW service entry point functions
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 10-Mar-2005     Amit Sharma     None            Initial
* 27-Mar-2005     Amit Sharma                 Updated handling of timer_expiry
*                                             to incorporate changes of SCM
* 29-May-2005    Amit Sharma   SPR:7568      Forceful call clear in idle state
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_rnw_types.h"
#include "icf_rnw_macro.h"
#include "icf_rnw_defs.h"
#include "icf_rnw_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_init
*
* DESCRIPTION   : This is the initialization function of RNW service module
*                 which is called by DBM at the time of system startup
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_rnw_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_rnw_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_rnw_data_st         *p_rnw_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t             state_count = 0;
    icf_uint8_t             event_count = 0;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_RNW)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
            sizeof(icf_rnw_data_st),
            p_rnw_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Initialize all funciton pointers with icf_rnw_unexpected_event*/
    for (state_count = 0; state_count < ICF_RNW_MAX_STATE; state_count++)
    {
        for (event_count = 0; event_count < ICF_RNW_MAX_EVENT; 
                event_count++)
        {
            p_rnw_data->rnw_fsm_table[state_count][event_count] = 
                icf_rnw_unexpected_event;
        } /* event_count++) */
    } /* for (state_count = 0; state_co... */

    /* ICF_RNW_REFER */
    p_rnw_data->rnw_fsm_table[ICF_RNW_00_IDLE][ICF_RNW_REFER]
        = icf_rnw_00_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT][ICF_RNW_REFER]
        = icf_rnw_xx_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_REFER] = icf_rnw_xx_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_REFER] = icf_rnw_xx_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT][ICF_RNW_REFER]
        = icf_rnw_xx_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_REFER] = icf_rnw_xx_refer;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_REFER] = icf_rnw_xx_refer;

    /* ICF_RNW_START_PROC_SIGNALING */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_START_PROC_SIGNALING]
        = icf_rnw_xx_start_proc_signaling;

    /* ICF_RNW_START_PROC_MEDIA */
    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_START_PROC_MEDIA]
        = icf_rnw_02_start_proc_media;

    /* ICF_RNW_NOTIFY_SUCCESS_RESP */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_xx_notify_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_xx_notify_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_xx_notify_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_xx_notify_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_xx_notify_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_NOTIFY_SUCCESS_RESP]
        = icf_rnw_06_notify_succ_resp;

    /* ICF_RNW_MEDIA_COMMITTED */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_MEDIA_COMMITTED]
        = icf_rnw_01_media_committed;

    /* ICF_RNW_MERGE_MEDIA_RESP */
    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_MERGE_MEDIA_RESP]
        = icf_rnw_03_merge_media_resp;

    /* ICF_RNW_INVITE_SUCCESS_RESP */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_xx_invite_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_xx_invite_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_xx_invite_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_04_invite_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_xx_invite_succ_resp;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_INVITE_SUCCESS_RESP]
        = icf_rnw_xx_invite_succ_resp;

    /* ICF_RNW_MEDIA_ERROR */
    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_MEDIA_ERROR]
        = icf_rnw_xx_media_error;

    /* ICF_RNW_CALL_RELEASED */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_xx_call_released;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_xx_call_released;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_03_call_released;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_xx_call_released;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_xx_call_released;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_CALL_RELEASED]
        = icf_rnw_xx_call_released;

    /* ICF_RNW_TIMER_EXPIRY */
    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_xx_timer_expiry;

    /* ICF_RNW_FORCEFUL_CALL_CLEAR */
    p_rnw_data->rnw_fsm_table[ICF_RNW_00_IDLE]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_03_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_FORCEFUL_CALL_CLEAR]
        = icf_rnw_xx_forceful_call_clear;

     /* ICF_RNW_INTERNAL_RNW_REJ */
    p_rnw_data->rnw_fsm_table[ICF_RNW_00_IDLE]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_01_AWT_MEDIA_COMMIT]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_02_AWT_START_PROC_MEDIA]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_03_AWT_MERGE_MEDIA_RESP]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_04_AWT_FOR_CONNECT]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_05_AWT_START_PROC_SIGNALING]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_06_AWT_NOTIFY_ACK]
        [ICF_RNW_INTERNAL_RNW_REJ]
        = icf_rnw_xx_internal_rnw_rej;

    p_rnw_data->rnw_fsm_table[ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM]
        [ICF_RNW_CTA_INITIATED_CFM]
        = icf_rnw_07_cta_initiated_cfm;
    p_rnw_data->rnw_fsm_table[ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM]
        [ICF_RNW_TIMER_EXPIRY]
        = icf_rnw_07_timer_expiry;
    /*********End of Function pointer assignment*************/

    /*Assign fsm table pointer to global data pointer*/
    *p_p_rnw_data = (icf_void_t *)p_rnw_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;

} /* icf_rnw_init */

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_process_mesg
*
* DESCRIPTION   : This is the Entry Point function of RNW service module
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_rnw_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_rnw_pdb_st          rnw_pdb,*p_pdb = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;
    icf_rnw_data_st         *p_rnw_data = ICF_NULL;
    icf_uint8_t             event = 0;
   
    /* Copy p_glb_pdb */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_RNW)

    ICF_FUNCTION_ENTER(p_glb_pdb)
   
    icf_rnw_init_pdb(&rnw_pdb);
    /* Copy ecode pointer */   
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    p_pdb = &rnw_pdb;

    p_pdb->p_internal_msg = p_internal_msg;

    /* Convert the incoming API to the Event */
    ret_val = icf_rnw_convert_api_to_event(p_pdb, &event);

    if (ICF_EVENT_UNDEFINED != event)
    {
        /* Copy call context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))
        
        p_pdb->p_call_ctx = p_glb_pdb->p_call_ctx;

        /* Copy service context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, 
                (ICF_NULL != p_pdb->p_call_ctx->p_service_ctx))
            
        p_pdb->p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

        /* Copy RNW context pointer to p_pdb */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != 
                    (icf_rnw_ctx_st *)p_pdb->p_service_ctx->
                    active_service_list.p_sl_ctx[ICF_INT_MODULE_RNW 
                    - ICF_INT_MODULE_SL_START - 1]))

        p_pdb->p_rnw_ctx =
        (icf_rnw_ctx_st *)p_pdb->p_service_ctx->
        active_service_list.p_sl_ctx[ICF_INT_MODULE_RNW - 
        ICF_INT_MODULE_SL_START - 1];

        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        
        /*Call DBM function to get RNW global data pointer*/
        ret_val =
            icf_dbm_get_module_glb_data(p_internal_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_RNW),
                    (icf_void_t *)&p_rnw_data, p_ecode);


        if(ICF_FAILURE != ret_val)
        {
            p_pdb->p_rnw_ctx->event = event;
            
            ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_STATE)

            ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_EVENT)

            /* Call appropriate State machine function based on
             * State and event */
             ret_val = p_rnw_data->rnw_fsm_table[p_pdb->p_rnw_ctx->state]
                [p_pdb->p_rnw_ctx->event](p_pdb);
        } /* if(ICF_FAILURE != ret_val) */
    } /* if(ICF_FAILURE != ret_val) */

    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if (ICF_FAILURE == ret_val)
    {
        /* Set status as service ended error Reset service continue status */
        ICF_RNW_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_RNW_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_RNW_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_rnw_deinit_ctx(p_pdb);
    }
    else if(ICF_RNW_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_RNW_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_RNW_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_RNW_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_rnw_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else if(ICF_RNW_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        /*
         * If normal then reset service continue
         */
        ICF_RNW_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_RNW_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)
        
        icf_rnw_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else
    {
        ICF_RNW_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
        
        ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_STATE)
        
        ICF_RNW_TRACE(p_pdb, ICF_TRACE_RNW_EVENT)
    } /* else */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
} /* icf_rnw_process_msg */

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_convert_api_to_event
*
* DESCRIPTION   : This function converts incoming APIs to internal events
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t  icf_rnw_convert_api_to_event(
        INOUT  icf_rnw_pdb_st   *p_pdb,
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
        case ICF_NW_INC_REFER:
            ICF_RNW_APP_TRACE(p_pdb, ICF_TRACE_REFER_RECVD)
            *p_event = ICF_RNW_REFER;
            break;

        case ICF_START_PROC_SIGNALING:
            *p_event = ICF_RNW_START_PROC_SIGNALING;
            break;

        case ICF_START_PROC_MEDIA:
            *p_event = ICF_RNW_START_PROC_MEDIA;
            break;

        case ICF_NW_INC_NOTIFY_RESP:
            *p_event = ICF_RNW_NOTIFY_SUCCESS_RESP;
            break;
            
        case ICF_SC_INTERNAL_MEDIA_COMMIT:
            *p_event = ICF_RNW_MEDIA_COMMITTED;
            break;
            
        case ICF_CREATE_CONF_RESP:
            *p_event = ICF_RNW_MERGE_MEDIA_RESP;
            break;

        case ICF_NW_INC_CONNECT:
             *p_event = ICF_RNW_INVITE_SUCCESS_RESP;
            break;

        case ICF_MEDIA_ERROR_IND:
             *p_event = ICF_RNW_MEDIA_ERROR;
            break;

        case ICF_CC_CALL_RELEASED:
             *p_event = ICF_RNW_CALL_RELEASED;
            break;

        case ICF_SC_INTERNAL_RNW_REJ:
             *p_event = ICF_RNW_INTERNAL_RNW_REJ;
            break;
        
        case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_RNW_FORCEFUL_CALL_CLEAR;
            break;
            
        case ICF_TIMER_EXPIRY_IND:
            *p_event = ICF_RNW_TIMER_EXPIRY;
            break;
        case ICF_USER_CALL_TRANSFER_INITIATED_CFM:
            *p_event = ICF_RNW_CTA_INITIATED_CFM;
            break;
        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_rnw_convert_api_to_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_deinit
*
* DESCRIPTION   : This is the deinit function of RNW service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_rnw_deinit(
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
} /* icf_rnw_deinit */
#endif


