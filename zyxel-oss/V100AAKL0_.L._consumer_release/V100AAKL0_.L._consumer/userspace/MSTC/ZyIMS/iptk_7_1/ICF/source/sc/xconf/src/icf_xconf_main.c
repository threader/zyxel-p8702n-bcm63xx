/******************************************************************************
*
* FILE NAME    : icf_xconf_main.c
*
* DESCRIPTION  : Contains the definitions of XCONF service entry point functions
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 4-Jul-2007  Abhishek Dhammawat  CDD Rel7.0    Initial Creation
* 18-sept-2007 Abhishek Dhammawat SPR16508      added
*                                               icf_xconf_01_del_party_req
*                                               call in icf_xconf_init
* Copyright 2007,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/*Include Header Files */
#include "icf_xconf_types.h"
#include "icf_xconf_macro.h"
#include "icf_xconf_defs.h"
#include "icf_xconf_prototypes.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_init
*
* DESCRIPTION   : This is the initialization function of XCONF service module
*                 which is called by DBM at the time of system startup
*
* RETURNS       : ICF_SUCCESS/ICF_FAILURE
*
*******************************************************************************/
icf_return_t   icf_xconf_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_xconf_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_xconf_data_st       *p_xconf_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t             state_count = 0;
    icf_uint8_t             event_count = 0;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_XCONF)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
                      sizeof(icf_xconf_data_st),
                      p_xconf_data, 
                      ICF_RET_ON_FAILURE, 
                      p_ecode, 
                      ret_val)

    /*Initialize all funciton pointers with icf_xconf_unexpected_event*/
    for (state_count = 0; state_count < ICF_XCONF_MAX_STATE; state_count++)
    {
        for (event_count = 0; event_count < ICF_XCONF_MAX_EVENT; 
                event_count++)
        {
            p_xconf_data->xconf_fsm_table[state_count][event_count] = 
                icf_xconf_unexpected_event;
        } /* event_count++) */
    } /* for (state_count = 0; state_co... */

    p_xconf_data->xconf_fsm_table[ICF_XCONF_00_IDLE]
	    [ICF_XCONF_INITIATE_CONF_REQ]
        = icf_xconf_00_init_conf_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
	    [ICF_XCONF_CONF_SERVER_CONNECTED]
        = icf_xconf_01_conf_server_connected;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
        [ICF_XCONF_ADD_PARTY_REQ] = icf_xconf_01_add_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_ADD_PARTY_REQ] = icf_xconf_02_add_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_ADD_PARTY_REQ] = icf_xconf_03_add_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
	    [ICF_XCONF_ADD_PARTY_REQ]
        = icf_xconf_04_add_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP]
        [ICF_XCONF_ADD_PARTY_REQ] = icf_xconf_05_add_party_req;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_ADD_PARTY_REQ] = icf_xconf_02_add_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_ADD_PARTY_RESP] = icf_xconf_02_add_party_resp;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_01_del_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_02_del_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_03_del_party_req;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_06_del_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_04_del_party_req;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP]
        [ICF_XCONF_DELETE_PARTY_REQ]
        = icf_xconf_05_del_party_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_DELETE_PARTY_RESP]
        = icf_xconf_03_del_party_resp;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
        [ICF_XCONF_RELEASE_CONF_REQ]
        = icf_xconf_01_release_conf_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_RELEASE_CONF_REQ]
        = icf_xconf_xx_release_conf_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_RELEASE_CONF_REQ]
        = icf_xconf_03_release_conf_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_RELEASE_CONF_REQ]
        = icf_xconf_04_release_conf_req;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_RELEASE_CONF_REQ]
        = icf_xconf_xx_release_conf_req;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_NOTIFY_FOR_SUBS]
        = icf_xconf_xx_notify_for_sub;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_NOTIFY_FOR_SUBS]
        = icf_xconf_xx_notify_for_sub;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_NOTIFY_FOR_SUBS]
        = icf_xconf_xx_notify_for_sub;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_NOTIFY_FOR_SUBS]
        = icf_xconf_xx_notify_for_sub;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_SUBS_RESP]
        = icf_xconf_xx_subs_resp;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_SUBS_RESP]
        = icf_xconf_xx_subs_resp;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_SUBS_RESP]
        = icf_xconf_xx_subs_resp;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_SUBS_RESP]
        = icf_xconf_xx_subs_resp;
    
   p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_01_call_released;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_02_call_released;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_03_call_released;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_xx_call_released;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_06_call_released;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_05_AWT_CONFERENCE_TERMINATE_RESP]
        [ICF_XCONF_CALL_RELEASED]
        = icf_xconf_05_call_released;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_01_AWT_CONFERENCE_CONNECT]
        [ICF_XCONF_FORCEFUL_CALL_CLEAR]
        = icf_xconf_01_forceful_call_clear;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_02_AWT_ADD_PARTY_RESP]
        [ICF_XCONF_FORCEFUL_CALL_CLEAR]
        = icf_xconf_xx_forceful_call_clear;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_03_AWT_DEL_PARTY_RESP]
        [ICF_XCONF_FORCEFUL_CALL_CLEAR]
        = icf_xconf_xx_forceful_call_clear;

    p_xconf_data->xconf_fsm_table[ICF_XCONF_04_CONFERENCE_CONNECTED]
        [ICF_XCONF_FORCEFUL_CALL_CLEAR]
        = icf_xconf_xx_forceful_call_clear;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_FORCEFUL_CALL_CLEAR]
        = icf_xconf_xx_forceful_call_clear;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_00_IDLE]
        [ICF_XCONF_INCOMING_CONF_REQ] 
        = icf_xconf_00_incoming_conf_req;
    
    p_xconf_data->xconf_fsm_table[ICF_XCONF_06_AWT_CALL_RESUME_RESP]
        [ICF_XCONF_CALL_RESUME_RESP]
        = icf_xconf_06_call_resume_resp;


    /*********End of Function pointer assignment*************/

    /*Assign fsm table pointer to global data pointer*/

    *p_p_xconf_data = (icf_void_t *)p_xconf_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;

} /* icf_xconf_init */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_process_mesg
*
* DESCRIPTION   : This is the Entry Point function of XCONF service module
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t   icf_xconf_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_xconf_pdb_st        *p_pdb = ICF_NULL;
    icf_xconf_data_st       *p_xconf_data = ICF_NULL;
    icf_uint8_t             event = 0;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_xconf_pdb_st        xconf_pdb = {0, 0, 0, 0, 0, 0, 0,0};
   
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_XCONF)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_pdb = &xconf_pdb;
    p_pdb->p_internal_msg = p_internal_msg;
    
    /* Copy p_glb_pdb */
    p_pdb->p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    /* Copy ecode pointer */   
    p_pdb->p_ecode = &(p_internal_msg->msg_hdr.ecode);

    if(p_pdb->p_internal_msg->msg_hdr.msg_id ==
             ICF_INTERNAL_REFER_RESP)
    {
        /* Copy call context pointer to p_pdb */
        p_pdb->p_call_ctx = p_pdb->p_glb_pdb->p_call_ctx;
        p_pdb->p_service_ctx = p_pdb->p_glb_pdb->p_call_ctx->p_service_ctx;

        /* Copy XCONF context pointer to p_pdb */
        p_pdb->p_xconf_ctx =
            (icf_xconf_ctx_st *)p_pdb->p_service_ctx->
            active_service_list.p_sl_ctx[ICF_INT_MODULE_XCONF - 
            ICF_INT_MODULE_SL_START - 1];
    }
          

    /* Convert the incoming API to the Event */
    ret_val = icf_xconf_convert_api_to_event(p_pdb, &event);

    if (ICF_EVENT_UNDEFINED != event)
    {
       if((p_pdb->p_internal_msg->msg_hdr.msg_id !=
             ICF_USER_INITIATE_EXTERNAL_CONF_REQ) && 
          (p_pdb->p_internal_msg->msg_hdr.msg_id != 
             ICF_INTERNAL_REFER_RESP)) 
       {
          /* Copy call context pointer to p_pdb */
          p_pdb->p_call_ctx = p_pdb->p_glb_pdb->p_call_ctx;
          p_pdb->p_service_ctx = p_pdb->p_glb_pdb->p_call_ctx->p_service_ctx;
          /* Copy XCONF context pointer to p_pdb */
           p_pdb->p_xconf_ctx =
            (icf_xconf_ctx_st *)p_pdb->p_service_ctx->
            active_service_list.p_sl_ctx[ICF_INT_MODULE_XCONF - 
            ICF_INT_MODULE_SL_START - 1];
       }

         ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        
         /*Call DBM function to get XCONF global data pointer*/
          ret_val = icf_dbm_get_module_glb_data(p_pdb->p_glb_pdb,
                                  (icf_module_id_t)(ICF_INT_MODULE_XCONF),
                                  (icf_void_t *)&p_xconf_data, p_pdb->p_ecode);

        if(ICF_FAILURE != ret_val)
        {
            
            ICF_XCONF_TRACE(p_pdb, ICF_TRACE_XCONF_STATE)
            ICF_XCONF_TRACE(p_pdb, ICF_TRACE_XCONF_EVENT)

            /* Call appropriate State machine function based on
             * State and event */
            if(p_pdb->p_xconf_ctx == ICF_NULL)
             {
                ret_val = p_xconf_data->xconf_fsm_table[ICF_XCONF_00_IDLE]
                          [event](p_pdb);
             }
            else
             {
                p_pdb->p_xconf_ctx->event = event;
                ret_val = 
                  p_xconf_data->xconf_fsm_table[p_pdb->p_xconf_ctx->state]
                [p_pdb->p_xconf_ctx->event](p_pdb);
             }
        } /* if(ICF_FAILURE != ret_val) */
    } /* if(ICF_FAILURE != ret_val) */

    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if ((ICF_FAILURE == ret_val) &&
             (ICF_NULL != p_pdb->p_call_ctx)) 
    {
        /* Set status as service ended error Reset service continue status */
        ICF_XCONF_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_XCONF_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)
        icf_xconf_deinit_ctx(p_pdb);
    }
    else if(ICF_XCONF_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_XCONF_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_XCONF_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)
        icf_xconf_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else if(ICF_XCONF_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    {
        /*
         * If normal then reset service continue
         */
        ICF_XCONF_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)

        /* for resolving scenario when add_party is ongoing and remote
           clears the call, after receiving release_xconf_ind again
           icf_xconf_00_init_conf_req function is invoked */ 
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_W_REP_REQ)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_REQ)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_EXTERNAL_CONFERENCE_CONNECTED)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_SUCCESS_RESP)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_INTERNAL_REFER_FAILURE_RESP)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_CALL_ENDED_DUE_TO_RELEASE)
       ICF_XCONF_RESET_STATUS_2(p_pdb, ICF_STATUS_INCOMING_EXTERNAL_CONF_CALL)

        icf_xconf_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */
    else
    {
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_XCONF_STATE)
        ICF_XCONF_TRACE(p_pdb, ICF_TRACE_XCONF_EVENT)
    } /* else */

    ICF_FUNCTION_EXIT(p_pdb->p_glb_pdb)

    ICF_RESET_MODULE(p_pdb->p_glb_pdb)

    return ret_val;
} /* icf_xconf_process_msg */

/*******************************************************************************
*
* FUNCTION NAME : icf_xconf_convert_api_to_event
*
* DESCRIPTION   : This function converts incoming APIs to internal events
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t  icf_xconf_convert_api_to_event(
        INOUT  icf_xconf_pdb_st *p_pdb,
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
        case ICF_USER_INITIATE_EXTERNAL_CONF_REQ:
            *p_event = ICF_XCONF_INITIATE_CONF_REQ;
            break;

        case ICF_EXTERNAL_CONFERENCE_CONNECTED:
            *p_event = ICF_XCONF_CONF_SERVER_CONNECTED;
            break;

        case ICF_USER_ADD_PARTY_REQ:
            *p_event = ICF_XCONF_ADD_PARTY_REQ;
            break;

        case ICF_INTERNAL_REFER_RESP:
            /* check the current state or ongoing_action in xconf_ctx */
            if(p_pdb->p_xconf_ctx->state == ICF_XCONF_02_AWT_ADD_PARTY_RESP)
            { 
                 *p_event = ICF_XCONF_ADD_PARTY_RESP;
            }
           else if(p_pdb->p_xconf_ctx->state == ICF_XCONF_03_AWT_DEL_PARTY_RESP)
            {
                 *p_event = ICF_XCONF_DELETE_PARTY_RESP;
            } 
            break;
        
        case ICF_USER_DELETE_PARTY_REQ:
            *p_event = ICF_XCONF_DELETE_PARTY_REQ;
            break;

        case ICF_INTERNAL_SUBSCRIBE_RESP:
            *p_event = ICF_XCONF_SUBS_RESP;
            break;
            
        case ICF_CC_CALL_RELEASED:
            *p_event = ICF_XCONF_CALL_RELEASED;
            break;
            
        case ICF_USER_RELEASE_CONFERENCE_REQ:
            *p_event = ICF_XCONF_RELEASE_CONF_REQ;
            break;

        case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_XCONF_FORCEFUL_CALL_CLEAR;
             break;

        case ICF_INTERNAL_NOTIFY_IND:
             *p_event = ICF_XCONF_NOTIFY_FOR_SUBS;
             break;
        case ICF_INCOMING_EXTERNAL_CONFERENCE_REQ:
             *p_event = ICF_XCONF_INCOMING_CONF_REQ;
             break;
        case ICF_SC_INTERNAL_CALL_RESUME_RESP:
             *p_event = ICF_XCONF_CALL_RESUME_RESP; 
             break;

        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_xconf_convert_api_to_event */

#endif
