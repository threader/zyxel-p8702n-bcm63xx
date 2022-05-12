/******************************************************************************
 *
 * FILE NAME:   icf_sic_init.c
 *
 * DESCRIPTION: This file contains the initialization functions of SIC.
 *
 * REVISION HISTORY:
 *              Date            Name            Reference       Comments
 *          11-Aug-2005     Syed Ali Rizvi       LLD          Initial
 *
 * 26-Jan-2008  Amit Mahajan       ICF Rel 8.0  Changes for reg event package support
 * 20-Nov-2008  Rajiv Kumar        SPR 19188    Rel 7.1.2 SPR merging (SPR
 *                                              18931)  
 * 09-Feb-2009  Anurag Khare       ICF Rel 8.1  changes in FSM of OPTIONS
 * 24-Apr-2009 Abhishek Dhammawat  SPR 19488    Additions for ICF_SIC_EVENT_EARLY_SESSION_CFM
 * 28-May-2009  Tarun Gupta        Rel 8.2      Async NOTIFY Support
 * 02-Jun-2009  Preksha            Rel 8.2      Asynchronus Message enh
 *
 ******************************************************************************/

#include "icf_sic_prototypes.h"
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_init
 *
 * DESCRIPTION:     This is the initialiozation function of SIC.
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
icf_return_t icf_sic_init(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        OUT     icf_void_t           **p_p_glb_data,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_sic_data_st                 *p_sic_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /* Allocate memory to table */
    ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_sic_data_st),
            p_sic_data, ICF_RET_ON_FAILURE,p_ecode, ret_val)
        
    /* Initilize the table */
    
    /* 09:In idle state only subscription request is a valid event. In case
     * of any other event function for unexpected msg will be called 
     */
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_00_subs_req; 
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = 
        icf_sic_unexpected_msg;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = 
        icf_sic_unexpected_msg;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_unexpected_msg;
    
	p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_unexpected_msg;

    /* Asynchronous NOTIFY support */
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_00_IDLE]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_01_subs_req;        

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_01_unsub_req;        

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_01_success_resp; 

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = 
        icf_sic_01_failure_resp_no_retry;        

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = 
        icf_sic_01_failure_resp_with_retry;        

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_01_notify_active;

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = icf_sic_01_notify_term_retry_imm;

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = 
        icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_01_resp_timer_exp;

	p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_01_03_notify_timer_exp;

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = icf_sic_unexpected_msg; 

    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_unexpected_msg;

    /* Asynchronous NOTIFY support */
    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_01_SUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_unexpected_msg;
 
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_02_subs_req;

    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_02_unsub_req;

    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_02_success_resp;

    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = 
        icf_sic_unexpected_msg;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_02_notify_active;
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = 
        icf_sic_02_notify_term_no_retry;        

    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = 
        icf_sic_02_notify_term_retry_imm;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = 
        icf_sic_02_notify_term_retry_after;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = 
        icf_sic_02_duration_timer_exp;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_unexpected_msg; 
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_02_await_notify_timer_exp;

    /* Asynchronous NOTIFY support */
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_02_SUBSCRIBED]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_unexpected_msg;
                                   
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_03_subs_req;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_03_unsub_req;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_03_success_resp;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = 
        icf_sic_03_failure_resp_no_retry;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = 
        icf_sic_03_failure_resp_with_retry;    
    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18931)*/ 
    /* SPR 18931: Notify with state active used to be treated as 
    * unexpected message so no response used to be sent. new
    * function has been added to reject the notify with 500
    */      
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_03_notify_active;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = 
        icf_sic_03_notify_term_no_retry;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = 
        icf_sic_03_notify_term_retry_imm;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = 
        icf_sic_03_notify_term_retry_after;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_03_resp_timer_exp;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_unexpected_msg;

	p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_01_03_notify_timer_exp;

    /* Asynchronous NOTIFY support */
    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_03_UNSUBSCRIBING]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_04_unsub_req;
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = 
        icf_sic_unexpected_msg;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_04_notify_active;
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = 
        icf_sic_04_notify_term_no_retry;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = 
        icf_sic_04_notify_term_retry_imm;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = 
        icf_sic_04_notify_term_retry_after;        
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_unexpected_msg; 
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = icf_sic_unexpected_msg;
    
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_04_retry_timer_exp;
    
 	p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_unexpected_msg;

    /* Asynchronous NOTIFY support */
    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_04_SUBSCRIBE_RETRY]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_unexpected_msg;

    /* Handler function for new state 05 */
    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_CFM] = icf_sic_05_notify_cfm;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP] = icf_sic_05_timer_expiry;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_SUBS_REQ] = icf_sic_05_subs_req;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_UNSUB_REQ] = icf_sic_05_unsub_req;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_SUCCESS_RESP] = icf_sic_05_success_resp;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY] = icf_sic_05_failure_resp_no_retry;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY] = icf_sic_05_failure_resp_with_retry;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_ACTIVE] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER] = icf_sic_unexpected_msg;        

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_RESP_TIMER_EXP] = icf_sic_unexpected_msg;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP] = icf_sic_unexpected_msg ;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_DURATION_TIMER_EXP] = icf_sic_05_duration_timer_exp;

    p_sic_data->sic_fsm_table[ICF_SIC_05_AWAIT_NOTIFY_CFM]
        [ICF_SIC_EVENT_RETRY_TIMER_EXP] = icf_sic_05_retry_timer_exp;

    /* ICF_6_1_OPTIONS_SUPPORT_START */
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
	[ICF_SIC_EVENT_INC_OPTIONS_REQ] = icf_sic_00_inc_options_req;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_OTG_OPTIONS_REQ] = icf_sic_00_otg_options_req;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_MEDIA_CAPAB_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_OPTIONS_APP_CFM] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_OPTIONS_NW_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_TIMER_EXPIRY] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_CLEAR_OPTIONS_CTX] = icf_sic_xx_unexpected_options_event;
    /*SPR 19488 init the other FSM states for the event ICF_SIC_EVENT_EARLY_SESSION_CFM
      to avoid crash when early session cfm is recevied in other FSM states*/
    p_sic_data->sic_options_fsm_table[ICF_SIC_00_OPTIONS_IDLE]
        [ICF_SIC_EVENT_EARLY_SESSION_CFM] = icf_sic_xx_unexpected_options_event;

    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_INC_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_OTG_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_MEDIA_CAPAB_RSP] = icf_sic_01_media_capabilites_resp;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_OPTIONS_APP_CFM] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_OPTIONS_NW_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_TIMER_EXPIRY] = icf_sic_01_02_timer_expiry;
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_CLEAR_OPTIONS_CTX] = icf_sic_01_02_ssa_clear_options_ctx;
    /*SPR 19488 init the other FSM states for the event ICF_SIC_EVENT_EARLY_SESSION_CFM
      to avoid crash when early session cfm is recevied in other FSM states*/
    p_sic_data->sic_options_fsm_table[ICF_SIC_01_AWT_MEDIA_CAPAB_RSP]
        [ICF_SIC_EVENT_EARLY_SESSION_CFM] = icf_sic_xx_unexpected_options_event;
   
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_INC_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_OTG_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_MEDIA_CAPAB_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_OPTIONS_APP_CFM] = icf_sic_02_options_app_cfm;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_OPTIONS_NW_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_TIMER_EXPIRY] = icf_sic_01_02_timer_expiry;
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_CLEAR_OPTIONS_CTX] = icf_sic_01_02_ssa_clear_options_ctx;
    /*SPR 19488 init the other FSM states for the event ICF_SIC_EVENT_EARLY_SESSION_CFM
      to avoid crash when early session cfm is recevied in other FSM states*/
    p_sic_data->sic_options_fsm_table[ICF_SIC_02_AWT_OPTIONS_APP_CFM]
        [ICF_SIC_EVENT_EARLY_SESSION_CFM] = icf_sic_xx_unexpected_options_event;
   
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_INC_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_OTG_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_MEDIA_CAPAB_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_OPTIONS_APP_CFM] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_OPTIONS_NW_RSP] = icf_sic_03_options_resp;
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_TIMER_EXPIRY] = icf_sic_xx_unexpected_options_event; 
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_CLEAR_OPTIONS_CTX] = icf_sic_xx_unexpected_options_event;
    /*SPR 19488 init the other FSM states for the event ICF_SIC_EVENT_EARLY_SESSION_CFM
      to avoid crash when early session cfm is recevied in other FSM states*/
    p_sic_data->sic_options_fsm_table[ICF_SIC_03_AWT_OPTIONS_NW_CFM]
        [ICF_SIC_EVENT_EARLY_SESSION_CFM] = icf_sic_xx_unexpected_options_event;
    /* ICF_6_1-OPTIONS_SUPPORT_END */  

    /*  Rel 8.1
     *  OPTIONS FSM changes 
     */
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_INC_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_OTG_OPTIONS_REQ] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_MEDIA_CAPAB_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_OPTIONS_APP_CFM] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_OPTIONS_NW_RSP] = icf_sic_xx_unexpected_options_event;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_TIMER_EXPIRY] = icf_sic_01_02_timer_expiry;
    p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_CLEAR_OPTIONS_CTX] = icf_sic_xx_unexpected_options_event;
   p_sic_data->sic_options_fsm_table[ICF_SIC_04_AWT_EARLY_SESSION_CFM]
        [ICF_SIC_EVENT_EARLY_SESSION_CFM] = icf_sic_04_early_session_cfm;

   /* ASYNCHRONUS_MESSAGE enhancements */
   p_sic_data->sic_message_fsm_table[ICF_SIC_00_MESSAGE_IDLE]
       [ICF_SIC_EVENT_INC_MESSAGE_REQ] = icf_sic_00_inc_message_req;
   p_sic_data->sic_message_fsm_table[ICF_SIC_00_MESSAGE_IDLE]
       [ICF_SIC_EVENT_MESSAGE_APP_CFM] = icf_sic_unexpected_msg;
   p_sic_data->sic_message_fsm_table[ICF_SIC_00_MESSAGE_IDLE]
       [ICF_SIC_EVENT_AWT_MESSAGE_CFM_TIMER_EXP] = icf_sic_unexpected_msg;

   p_sic_data->sic_message_fsm_table[ICF_SIC_01_AWAIT_MESSAGE_CFM]
       [ICF_SIC_EVENT_INC_MESSAGE_REQ] = icf_sic_unexpected_msg;/* send 500 to second message request handled in SSA*/
   p_sic_data->sic_message_fsm_table[ICF_SIC_01_AWAIT_MESSAGE_CFM]
       [ICF_SIC_EVENT_MESSAGE_APP_CFM] = icf_sic_01_message_cfm;
   p_sic_data->sic_message_fsm_table[ICF_SIC_01_AWAIT_MESSAGE_CFM]
       [ICF_SIC_EVENT_AWT_MESSAGE_CFM_TIMER_EXP] = icf_sic_01_msg_cfm_timer_exp;

    
    *p_p_glb_data = (icf_void_t *)p_sic_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;   
}

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_deinit
 *
 * DESCRIPTION:     This function would free the modules global data
 *
 ******************************************************************************/
icf_void_t icf_sic_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            *p_sic_data,
        OUT     icf_error_t           *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_glb_pdb) 

    /* Dellocate memory for the modules global data*/
    ICF_STATIC_MEMFREE(p_glb_pdb,p_sic_data,p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb) 
}
