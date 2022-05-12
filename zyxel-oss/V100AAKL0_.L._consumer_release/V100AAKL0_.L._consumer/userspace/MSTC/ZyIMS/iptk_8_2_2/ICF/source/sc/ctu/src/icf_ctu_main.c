/*******************************************************************************
*
* FILE NAME    : icf_ctu_main.c
*
* DESCRIPTION  : Contains the definitions of CTU service entry point functions
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 25-Jan-2005    Amit Sharma   None            Initial
* 28-Feb-2005    Amit Sharma                    Added handling for forceful
*                                               Call Clear event
* 29-Apr-2005    Amit Sharma                   Updated handling of timer expiry
* 19-May-2005  Amit Sharma     SPR:7561       Added 03_forceful function
* 19-May-2005  Amit Sharma     SPR:7568       Change in handling of ret_val 0
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
* 04-Mar-2009  Saurabh Gupta   SPR 19349      Klocworks fix
* Copyright 2006,Aricent .
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_ctu_types.h"
#include "icf_ctu_macro.h"
#include "icf_ctu_defs.h"
#include "icf_ctu_prototypes.h"
#include "icf_common_interface_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_init
*
* DESCRIPTION   : This is the initialization function of CTU service module
*                 which is called by DBM at the time of system startup
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_ctu_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_ctu_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_ctu_data_st         *p_ctu_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t             state_count = 0;
    icf_uint8_t             event_count = 0;


    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CTU)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
             sizeof(icf_ctu_data_st),
            p_ctu_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    /*Initialize all funciton pointers with icf_ctu_unexpected_event*/
    for (state_count = 0; state_count < ICF_CTU_MAX_STATE; state_count++)
    {
        for (event_count = 0; event_count < ICF_CTU_MAX_EVENT; 
                event_count++)
        {
            p_ctu_data->ctu_fsm_table[state_count][event_count] = 
                icf_ctu_unexpected_event;
        } /* event_count++) */
    } /* for (state_count = 0; state_co... */

    
    /* ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST */
    p_ctu_data->ctu_fsm_table[ICF_CTU_00_IDLE]
        [ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST]
        = icf_ctu_00_call_transfer_unattended_request;

    p_ctu_data->ctu_fsm_table[ICF_CTU_00_IDLE]
        [ICF_CTU_FORCEFUL_CALL_CLEAR]
        = icf_ctu_xx_forceful_call_clear;
    p_ctu_data->ctu_fsm_table[ICF_CTU_00_IDLE]
	[ICF_CTU_INTERNAL_REFER_REQUEST]
	= icf_ctu_00_internal_refer_req;

   p_ctu_data->ctu_fsm_table[ICF_CTU_01_WAIT_FOR_START_PROC_SIG]
        [ICF_CTU_START_PROC_SIG]
        = icf_ctu_01_start_proc_sig;

   p_ctu_data->ctu_fsm_table[ICF_CTU_01_WAIT_FOR_START_PROC_SIG]
        [ICF_CTU_CC_CALL_RELEASED]
        = icf_ctu_xx_cc_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_01_WAIT_FOR_START_PROC_SIG]
        [ICF_CTU_PA_CALL_RELEASED]
        = icf_ctu_xx_pa_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_01_WAIT_FOR_START_PROC_SIG]
        [ICF_CTU_TIMER_EXPIRY]
        = icf_ctu_xx_timer_expiry;

   p_ctu_data->ctu_fsm_table[ICF_CTU_01_WAIT_FOR_START_PROC_SIG]
        [ICF_CTU_FORCEFUL_CALL_CLEAR]
        = icf_ctu_xx_forceful_call_clear;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_CALL_RESUME_RESP]
        = icf_ctu_02_call_resume_resp;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_CALL_HOLD_RESP]
        = icf_ctu_02_call_hold_resp;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_CC_CALL_RELEASED]
        = icf_ctu_xx_cc_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_PA_CALL_RELEASED]
        = icf_ctu_xx_pa_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_TIMER_EXPIRY]
        = icf_ctu_xx_timer_expiry;

   p_ctu_data->ctu_fsm_table[ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP]
        [ICF_CTU_FORCEFUL_CALL_CLEAR]
        = icf_ctu_xx_forceful_call_clear;
  
   p_ctu_data->ctu_fsm_table[ICF_CTU_03_WAIT_FOR_REFER_RESP]
        [ICF_CTU_REFER_RESP]
        = icf_ctu_03_refer_resp;

   p_ctu_data->ctu_fsm_table[ICF_CTU_03_WAIT_FOR_REFER_RESP]
        [ICF_CTU_CC_CALL_RELEASED]
        = icf_ctu_xx_cc_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_03_WAIT_FOR_REFER_RESP]
        [ICF_CTU_PA_CALL_RELEASED]
        = icf_ctu_xx_pa_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_03_WAIT_FOR_REFER_RESP]
        [ICF_CTU_TIMER_EXPIRY]
        = icf_ctu_03_timer_expiry;

   p_ctu_data->ctu_fsm_table[ICF_CTU_03_WAIT_FOR_REFER_RESP]
        [ICF_CTU_FORCEFUL_CALL_CLEAR]
        = icf_ctu_03_forceful_call_clear;
     p_ctu_data->ctu_fsm_table[ICF_CTU_04_WAIT_FOR_NOTIFY]
        [ICF_CTU_NOTIFY]
        = icf_ctu_04_notify;

   p_ctu_data->ctu_fsm_table[ICF_CTU_04_WAIT_FOR_NOTIFY]
        [ICF_CTU_CC_CALL_RELEASED]
        = icf_ctu_04_cc_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_04_WAIT_FOR_NOTIFY]
        [ICF_CTU_PA_CALL_RELEASED]
        = icf_ctu_xx_pa_call_released;

   p_ctu_data->ctu_fsm_table[ICF_CTU_04_WAIT_FOR_NOTIFY]
        [ICF_CTU_TIMER_EXPIRY]
        = icf_ctu_xx_timer_expiry;

   p_ctu_data->ctu_fsm_table[ICF_CTU_04_WAIT_FOR_NOTIFY]
        [ICF_CTU_FORCEFUL_CALL_CLEAR]
        = icf_ctu_xx_forceful_call_clear;

   /*********End of Function pointer assignment*************/

    /*Assign fsm table pointer to global data pointer*/
    *p_p_ctu_data = (icf_void_t *)p_ctu_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;

} /* icf_ctu_init */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_process_mesg
*
* DESCRIPTION   : This is the Entry Point function of CTU service module
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_ctu_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_ctu_pdb_st          ctu_pdb, *p_pdb = ICF_NULL;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;
    icf_ctu_data_st         *p_ctu_data = ICF_NULL;
    icf_uint8_t             event = 0;
   
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_internal_msg))
    
    /*
     * Copy p_glb_pdb
     */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CTU)

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_internal_msg->msg_hdr.p_glb_pdb->p_ecode =
        &(p_internal_msg->msg_hdr.ecode);
#endif

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*
     * Copy ecode pointer
     */   
    p_ecode = &(p_internal_msg->msg_hdr.ecode);

    p_pdb = &ctu_pdb;

    p_pdb->p_internal_msg = p_internal_msg;

    /* Convert the incoming API to the Event */
    ret_val = icf_ctu_convert_api_to_event(p_pdb, &event);

    if((ICF_EVENT_UNDEFINED != event)
            && (ICF_FAILURE != ret_val))
    {
        /*
         * Copy call context pointer to p_pdb
         */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb->p_call_ctx))
            p_pdb->p_call_ctx = p_glb_pdb->p_call_ctx;

        /*
         * Copy service context pointer to p_pdb
         */
        ICF_ASSERT(p_glb_pdb, 
                (ICF_NULL != p_pdb->p_call_ctx->p_service_ctx))
            
        p_pdb->p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

        /*
         * Copy CTU context pointer to p_pdb
         */
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != 
                    (icf_ctu_ctx_st *)p_pdb->p_service_ctx->
                    active_service_list.p_sl_ctx[ICF_INT_MODULE_CTU 
                    - ICF_INT_MODULE_SL_START - 1]))

        p_pdb->p_ctu_ctx =
        (icf_ctu_ctx_st *)p_pdb->p_service_ctx->
        active_service_list.p_sl_ctx[ICF_INT_MODULE_CTU - 
        ICF_INT_MODULE_SL_START - 1];

        p_pdb->p_ctu_ctx->p_call_ctx = p_glb_pdb->p_call_ctx;

        ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        
        /*
         * Call appropriate State machine function based on
         * State and event
         */

        /*Call DBM function to get CTU global data pointer*/
        ret_val =
            icf_dbm_get_module_glb_data(p_internal_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_CTU),
                    (icf_void_t *)&p_ctu_data, p_ecode);


        if(ICF_FAILURE != ret_val)
        {
            p_pdb->p_ctu_ctx->event = event;
            
            ICF_CTU_TRACE(p_pdb, ICF_TRACE_CTU_STATE)

            ICF_CTU_TRACE(p_pdb, ICF_TRACE_CTU_EVENT)

            /*  ctu_fsm_table[ICF_CTU_MAX_STATE][ICF_CTU_MAX_EVENT]; */
			if ( (p_pdb->p_ctu_ctx->state < ICF_CTU_MAX_STATE) && 
				 (p_pdb->p_ctu_ctx->event < ICF_CTU_MAX_EVENT) )
			{
				ret_val = p_ctu_data->ctu_fsm_table[p_pdb->p_ctu_ctx->state]
                [p_pdb->p_ctu_ctx->event](p_pdb);
			}
        } /* if(ICF_FAILURE != ret_val) */
    } /* if(ICF_FAILURE != ret_val) */

    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if (ICF_FAILURE == ret_val)
    {
        /*
         * Set status as service ended error Reset service continue
         * status
         */
        ICF_CTU_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_CTU_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CTU_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_ctu_deinit_ctx(p_pdb);
    }
    else if(ICF_CTU_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_CTU_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_CTU_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_CTU_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        icf_ctu_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else if(ICF_CTU_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        /*
         * If normal then reset service continue
         */
        ICF_CTU_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        ICF_CTU_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)
        
        icf_ctu_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else
    {
        ICF_CTU_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
        
        ICF_CTU_TRACE(p_pdb, ICF_TRACE_CTU_STATE)
        
        ICF_CTU_TRACE(p_pdb, ICF_TRACE_CTU_EVENT)
    } /* else */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
} /* icf_ctu_process_msg */

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_convert_api_to_event
*
* DESCRIPTION   : This function converts incoming APIs to internal events
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t  icf_ctu_convert_api_to_event(
        INOUT  icf_ctu_pdb_st   *p_pdb,
        INOUT  icf_uint8_t      *p_event
        )
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;

    /*Copy p_glb_pdb*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*p_event = &(p_pdb->p_ctu_ctx->event);*/


    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_event))

    switch(p_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_USER_CALL_TRANSFER_UNATTENDED:
            *p_event = ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST;
            break;

        case ICF_SC_INTERNAL_CALL_RESUME_RESP: /* ICF_CTU_RESUME_RESP */
            *p_event = ICF_CTU_CALL_RESUME_RESP;
            break;

        case ICF_SC_INTERNAL_CALL_HOLD_RESP:
            *p_event = ICF_CTU_CALL_HOLD_RESP;
            break;

        case ICF_NW_INC_REFER_RESP:
            *p_event = ICF_CTU_REFER_RESP;
            break;
            
        case ICF_START_PROC_SIGNALING:
            *p_event = ICF_CTU_START_PROC_SIG;
            break;
            
        case ICF_CC_CALL_RELEASED:
            *p_event = ICF_CTU_CC_CALL_RELEASED;
            break;

        case ICF_USER_CALL_TERMINATE:
             *p_event = ICF_CTU_PA_CALL_RELEASED;
            break;

         case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_CTU_FORCEFUL_CALL_CLEAR;
            break;
	/*  Changes for XCONF */
	case ICF_INTERNAL_ABORT_REFER_REQ:
        case ICF_TIMER_EXPIRY_IND:
            ICF_CTU_APP_TRACE(p_pdb, ICF_TRACE_CTU_TIMER_EXPIRE)
            *p_event = ICF_CTU_TIMER_EXPIRY;
            break;
	/* Changes for XCONF enD */

        case ICF_NW_INC_NOTIFY_IND:
             *p_event = ICF_CTU_NOTIFY;
            break;
        /*  Change for XCONF */
        /*  Converting to the new event for Deleting a Party */
	case ICF_INTERNAL_REFER_REQ:
    	     *p_event = ICF_CTU_INTERNAL_REFER_REQUEST;
             break;
        /*  Change XCONF End */

        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_ctu_convert_api_to_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_deinit
*
* DESCRIPTION   : This is the deinit function of CTU service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_ctu_deinit(
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
} /* icf_ctu_deinit */
#endif
