/******************************************************************************
 *
 *
 * FILE NAME:   icf_sic_prototypes.h
 *
 * DESCRIPTION: This file contains the prototypes of the functions used
 *              to implement the functionality of SIC.
 *
 * REVISION HISTORY:
 * Date             Name        Reference      Comments
 * 11-Aug-2005  Syed Ali Rizvi     LLD         Initial
 * 26-Jan-2008  Amit Mahajan       ICF Rel 8.0  Changes for reg event package support
 * 20-Nov-2008  Rajiv Kumar        SPR 19188    Rel 7.1.2 SPR merging (SPR
 *                                              18931) 
 * 25-Nov-2008 Abhishek Dhammawat  SPR 19189   Merge SPR 18882 cycle3  
 * 10-Feb-2009  Anurag Khare       ICF Rel 8.1  Function introduced
 *                                              icf_sic_04_early_session_cfm
 * 28-May-2009  Tarun Gupta        Rel 8.2      Async NOTIFY Support 
 * 02-Jun-2009  Preksha            Rel 8.2      Async MESSAGE Support 
 * Copyright (c) 2009, Aricent.
 ******************************************************************************/

#ifndef __ICF_SIC_PROTOTYPES_H__
#define __ICF_SIC_PROTOTYPES_H__

#include "icf_common_prototypes.h"
#include "icf_sic_types.h"
#include "icf_internal_api_id.h"
/*#include "icf_api_id.h"*/

#include "icf_dbm_prototypes.h"
#include "icf_sic_macro.h"
/*#include "icf_trace_id.h"*/


#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_map_api_to_event
 *
 * DESCRIPTION:     This function maps the received API to the event.
 *
 ******************************************************************************/
icf_return_t icf_sic_map_api_to_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_validate_event
 *
 * DESCRIPTION:     This function does the validations on the event received. 
 *                  These validations are independant of the state machine.
 *
 ******************************************************************************/
icf_return_t icf_sic_validate_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_validate_mwi_event
 *
 * DESCRIPTION:     This function does the validations on the message-summary
 *                  event package.  
 *                  These validations are independent of the state machine.
 *                  It also creates a new SIC context if the context does not
 *                  exists.
 *
 ******************************************************************************/
icf_return_t icf_sic_validate_mwi_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb);


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_app_trace
 *
 * DESCRIPTION:     This function is used by SIC for trace handling.
 *
 ******************************************************************************/
icf_return_t icf_sic_app_trace(
        INOUT icf_sic_pdb_st *p_sic_pdb,
        IN icf_trace_id_t trace_id);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_validate_reg_event
 *
 * DESCRIPTION:     This function does the validations on the reg
 *                  event package.  
 *                  These validations are independent of the state machine.
 *                  It also creates a new ctx if the ctx does not exists.
 *
 *****************************************************************************/
icf_return_t icf_sic_validate_reg_event(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_00_subs_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_00_IDLE
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  This function is called after a new ctx is created.
 *                  It calls the functions to start the response timer and 
 *                  to send Subscribe request to SSA. It also handles the 
 *                  failure to start the response timer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_00_subs_req(
        INOUT icf_sic_pdb_st  *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_subs_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  This function will reset the event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_subs_req(
        INOUT icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_unsub_req
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  This function will buffer the unsub req till the response
 *                  of the Subscribe req send is received.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_success_resp
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP
 *                  This function checks if unsubscribe request is buffered.
 *                  The handling is done on the basis of the current status of
 *                  the buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_failure_resp_no_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY
 *                  The handling is done on the basis of the event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_failure_resp_with_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY
 *                  The handling is done on the basis of the cuurent state of
 *                  the buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_notify_active 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  Notify (active) is handled before the response of 
 *                  subscribe and is treated as the success response.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_resp_timer_exp 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_RESP_TIMER_EXP
 *                  The handling is done on the basis of event buffer.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_resp_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_subs_req 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_SUBS_REQ
 *                  It triggers SSA to send the REFRESH Subscribe request.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_03_notify_timer_exp 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_01_SUBSCRIBING
 *                  Event - ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP
 *                  .
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_03_notify_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_unsub_req 
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  It sends Unsubscribe request to SSA.
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_success_resp
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_active
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_no_retry
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_retry_imm
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_02_duration_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_DURATION_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_02_duration_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/******************************************************************************
 * FUNCTION:        icf_sic_02_await_notify_timer_exp 
 * DESCRIPTION:     This function is invoked when await notify timer expires
 *		    in state 02, it will clear the sic ctxt.
 *                  State - ICF_SIC_02_SUBSCRIBED
 *                  Event - ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP  
 *                         
 ******************************************************************************/
icf_return_t icf_sic_02_await_notify_timer_exp(
                         INOUT icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_subs_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUBS_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_success_resp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_failure_resp_no_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_failure_resp_with_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_active
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING 
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_no_retry 
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_retry_imm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_03_resp_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_RESP_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_03_resp_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_01_notify_term_retry_imm
 *
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_03_UNSUBSCRIBING
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_01_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_active
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_ACTIVE
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_active(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_no_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_retry_imm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_notify_term_retry_after
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_04_retry_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_04_SUBSCRIBE_RETRY
 *                  Event - ICF_SIC_EVENT_RETRY_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_04_retry_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_handle_forceful_delete
 *  
 * DESCRIPTION:     This function is called when - 
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ
 *                  
 ******************************************************************************/
icf_return_t icf_sic_handle_forceful_delete(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_populate_context
 *
 * DESCRIPTION:     This function populates the data initially in the SIC ctx
 *                  when DBM allocates a new SIC ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_populate_context(
        INOUT icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_req
 *
 * DESCRIPTION:     This function sneds the Subscribe Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_unsub_req
 *
 * DESCRIPTION:     This function sneds the Unsubscribe Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_options_req
 *
 * DESCRIPTION:     This function sends the Options Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_options_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_message_req
 *
 * DESCRIPTION:     This function sends the MESSAGE Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_message_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_publish_req
 *
 * DESCRIPTION:     This function sends the PUBLISH Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_publish_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_refer_req
 *
 * DESCRIPTION:     This function sends the REFER Request to SSA and 
 *                  handles the failure to send if to be handled.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_refer_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);
		
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_start_timer
 *
 * DESCRIPTION:     This function starts the timer of the type in the SIC 
 *                  ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_start_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_stop_curr_timer
 *
 * DESCRIPTION:     This function stops the current running timer if there is
 *                  any.
 *
 ******************************************************************************/
icf_return_t icf_sic_stop_curr_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_state_to_app 
 *
 * DESCRIPTION:     This function sends the subscription state to the
 *                  corresponding module (MWI or REG)
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_state_to_app(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_delink_to_ssa  
 *
 * DESCRIPTION:     This function sends the delink API to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_delink_to_ssa(
        INOUT icf_sic_pdb_st      *p_sic_pdb,
        IN  icf_boolean_t        forceful_delete);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_notify_resp   
 *
 * DESCRIPTION:     This function sends the notify resp to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_notify_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_notify_msg_to_app    
 *
 * DESCRIPTION:     This function sends the notify msg to the application.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_notify_msg_to_app(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_message_resp   
 *
 * DESCRIPTION:     This function sends the MESSAGE resp to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_message_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_get_min_vms_subs_duration
 *
 * DESCRIPTION:     This function gets the minimum subscription duration
 *
 ******************************************************************************/
icf_void_t icf_sic_get_min_vms_subs_duration(
        INOUT icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *  
 * FUNCTION:        icf_sic_get_line_for_grp_subs
 *
 * DESCRIPTION:     This function finds the line id for the address received
 *                  in the message account field. 
 *
 *****************************************************************************/
icf_void_t icf_sic_get_line_for_grp_subs(
    INOUT icf_sic_pdb_st  *p_sic_pdb,
    OUT icf_line_id_t    *p_line_id_recd);

/*******************************************************************************
 *  
 * FUNCTION:        icf_sic_get_line_for_line_subs
 *
 * DESCRIPTION:     This function finds the line id corresponding to the 
 *                  VMS server address in the pdb
 * 
 *****************************************************************************/
icf_void_t icf_sic_get_line_for_line_subs(
    INOUT icf_sic_pdb_st  *p_sic_pdb,
    OUT icf_line_id_t    *p_line_id_recd);

/*******************************************************************************
 *  
 * FUNCTION:        icf_sic_dealloc_context
 *
 * DESCRIPTION:     This function de-allocates the pointers in the SIC ctx
 *                  and then calls DBM to de-allocate the SIC ctx.
 * 
 *****************************************************************************/
icf_void_t icf_sic_dealloc_context(
        INOUT icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_subs_state_in_failure 
 *
 * DESCRIPTION:     This function sends the subscription state to the
 *                  corresponding module (MWI or REG) in case of failure.
 *                  Invalid line id will go in this case due to failure.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_subs_state_in_failure(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_unexpected_msg
 *
 * DESCRIPTION:     This function is called when unexpected msg is received.
 *
 ******************************************************************************/
icf_return_t icf_sic_unexpected_msg(
        INOUT icf_sic_pdb_st    *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_adm_map_sic_event_to_ext_event
 *
 * DESCRIPTION:     This function is use to map SIC API to external API.
 *
 ******************************************************************************/
icf_return_t icf_adm_map_sic_event_to_ext_event(
        icf_sic_pdb_st *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_refresh_sic_ctx 
 *
 * DESCRIPTION:     This function is called to refresh the SIC ctx incase it 
 *					has received a SUBSCRIBE request for an already existing a SIC ctx.
 *
 ******************************************************************************/
icf_return_t icf_sic_refresh_sic_ctx(
        INOUT icf_sic_pdb_st    *p_sic_pdb);

/****************************************************************
 *
 * FUNCTION:        icf_sic_process_clear_all
 *
 * DESCRIPTION:     This function handles the FORCEFUL CALL clear
 *                  message ID
 *
 **************************************************************/
icf_return_t icf_sic_process_clear_all(
        INOUT icf_sic_pdb_st      *p_sic_pdb);
/****************************************************************
 *
 * FUNCTION:        icf_sic_process_reinit_all
 *
 * DESCRIPTION:     This function handles the FORCEFUL reinitialize clear
 *                  message ID
 *
 **************************************************************/
icf_return_t icf_sic_process_reinit_all(
        INOUT icf_sic_pdb_st      *p_sic_pdb);


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_trace
 *
 * DESCRIPTION:     This function fills the data in the trace data and calls
 *                  the macro to print the traces.
 *
 ******************************************************************************/
icf_void_t icf_sic_trace(
        IN icf_sic_pdb_st       *p_sic_pdb,
        IN icf_trace_id_t        trace_id);

#endif


/******************************
 * Processing functions 
 ******************************/

icf_return_t icf_sic_process_options_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_publish_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_message_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_refer_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_notify_cfm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_notify_out_of_dialog(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/********************************
 * Response processing functions
 ********************************/

icf_return_t icf_sic_process_options_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_publish_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_message_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_refer_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

icf_return_t icf_sic_process_new_message_ind(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_create_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to create list of stream capab
 *					corresponding to applied sdps and also for creating list of deleted
 *					streams from the p_cc_pdb->p_call_ctx->p_stream_sdp_info
 *
 *					This function will do following things:
 *					1)Traverse the p_cc_pdb->p_call_ctx->p_stream_sdp_info list
 *					2)Check from the stream_status bitmask whether this particular
 *					  stream is Acitive or not.
 *					  if stream is INACTIVE then
 *						increase the count field in p_cc_pdb by one
 *						AND fill deleted_media_streams[count].stream_id,
 *						deleted_media_streams[count].stream_type for this stream
 *						kept in p_cc_pdb.
 *					  else
 *						create data of type stream capab from the applied sdp of
 * 						this stream and add the pointer to newly created stream
 *						capab data into the p_cc_pdb->p_accepted_streams list.
******************************************************************************/
icf_return_t
icf_sic_create_app_media_data(
	    icf_sic_pdb_st	*p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_delete_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to delete the app media data
******************************************************************************/
icf_return_t
icf_sic_delete_app_media_data(
	    icf_sic_pdb_st	*p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_sic_fill_stream_capab_from_local_sdp
 *
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *					structure from applied sdp of p_sic_pdb->p_stream_sdp_info
 *					->p_local_sdp and return pointer to newly created structure
 *					in p_cc_pdb->p_generic_ptr_1.
******************************************************************************/
icf_return_t icf_sic_fill_stream_capab_from_local_sdp(
		icf_sic_pdb_st	*p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_free_stream_capab_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream capab data
******************************************************************************/
icf_return_t
icf_sic_free_stream_capab_list(
	    icf_sic_pdb_st	*p_sic_pdb);


/* ICF_6_1_OPTIONS_SUPPORT_START */

/*****************************************************************************
 ** FUNCTION:     icf_sic_start_options_timer
 **
 ** DESCRIPTION:  This routine is used for starting the options timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_start_options_timer(
         icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:		icf_sic_00_inc_options_req
 *
 * DESCRIPTION:		This Function will be invoked when an SIP OPTIONS 
 *			Request is received from network. 
 *
 * CURRENT_STATE:	ICF_SIC_00_OPTIONS_IDLE
 *
 * CURRENT_EVENT:	ICF_SIC_EVENT_INC_OPTIONS_REQ
 *	
 * NEXT_STATES:
 *			1) ICF_SIC_01_AWT_MEDIA_CAPAB_RSP 
 *			2) ICF_SIC_02_AWT_OPTIONS_APP_CFM 
 * 
 ******************************************************************************/
 icf_return_t icf_sic_00_inc_options_req(
                icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_00_otg_options_req
 *
 * DESCRIPTION:         This Function will be invoked when application 
 *                      sends icf_options_req_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_00_OPTIONS_IDLE
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OTG_OPTIONS_REQ 
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_03_AWT_OPTIONS_NW_CFM 
 *
 ******************************************************************************/
 icf_return_t icf_sic_00_otg_options_req(
                icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_media_capabilites_resp
 *
 * DESCRIPTION:         This Function will be invoked when MMI sebds 
 *                      media capabilites response.
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP 
 *
 * CURRENT_EVENT:      	ICF_SIC_EVENT_MEDIA_CAPAB_RSP 
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_02_AWT_OPTIONS_APP_CFM 
 *                      2) ICF_SIC_00_OPTIONS_IDLE 
 *
 ******************************************************************************/
 icf_return_t icf_sic_01_media_capabilites_resp(
                icf_sic_pdb_st  *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_02_timer_expiry
 *
 * DESCRIPTION:         This Function will be invoked when options timer  
 *                    	expiry event is received 
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP or
 *			ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_TIMER_EXPIRY 
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE 
 *
 *****************************************************************************/
 icf_return_t icf_sic_01_02_timer_expiry(
                icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_02_ssa_clear_options_ctx
 *
 * DESCRIPTION:         This Function will be invoked when an SSA 
 *                      sends ICF_SIC_CLEAR_OPTIONS_CTX
 *
 * CURRENT_STATE:       ICF_SIC_01_AWT_MEDIA_CAPAB_RSP or
 *						ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_CLEAR_OPTIONS_CTX
 *
 * NEXT_STATES:
 *                      same state
 *
 *****************************************************************************/
icf_return_t icf_sic_01_02_ssa_clear_options_ctx(
                icf_sic_pdb_st  *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_02_options_app_cfm
 *
 * DESCRIPTION:         This Function will be invoked when application sends  
 *                      icf_options_cfm_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_02_AWT_OPTIONS_APP_CFM 
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OPTIONS_APP_CFM 
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_02_options_app_cfm(
                icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_03_options_resp
 *
 * DESCRIPTION:         This Function will be invoked when an SIP OPTIONS
 *                      Request is received from network.
 *
 * CURRENT_STATE:       ICF_SIC_03_AWT_OPTIONS_NW_CFM 
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_OPTIONS_NW_RSP 
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_OPTIONS_IDLE 
 *
 ******************************************************************************/
 icf_return_t icf_sic_03_options_resp(
                icf_sic_pdb_st  *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_xx_unexpected_options_event
 *
 * DESCRIPTION:         This Function will be invoked when an SIP OPTIONS
 *                      Request is received from network.
 *
 * CURRENT_STATE:       Any State 
 *
 * CURRENT_EVENT:       Unexpected event 
 *
 * NEXT_STATES:
 *                      NONE 
 *
 ******************************************************************************/
 icf_return_t icf_sic_xx_unexpected_options_event(
                icf_sic_pdb_st  *p_sic_pdb);
 

/*****************************************************************************
 ** FUNCTION:		icf_sic_map_to_options_event
 **
 * DESCRIPTION:		This function will Map external event to options fsm
 *					EVENT. if external event is not related to options
 *					fsm then this function will return 
 *					ICF_SIC_MAX_OPTIONS_EVENTS to p_options_event.
 *
 ****************************************************************************/
icf_return_t icf_sic_map_to_options_event(
	icf_sic_pdb_st *p_sic_pdb,
	icf_uint32_t *p_options_event);


/*****************************************************************************
 ** FUNCTION:     icf_sic_process_invalid_options_req
 **
 ** DESCRIPTION:  this function will be invoked when options FSM 
 **				  receives icf_options_req when fsm in 01 or 02 or 03 state.
 **				 
 **
 *****************************************************************************/
icf_return_t	icf_sic_process_invalid_options_req
(
icf_sic_pdb_st  *p_sic_pdb
);

/*****************************************************************************
 ** FUNCTION:     icf_sic_send_failure_response_to_ssa
 **
 ** DESCRIPTION:  This routine is used for sending failure indication to 
 **				  SSA.
 **
 *****************************************************************************/
icf_return_t icf_sic_send_failure_response_to_ssa(
	icf_sic_pdb_st  *p_sic_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_failure_response_for_unxepected_inc_options
 **
 ** DESCRIPTION:  This routine is used for sending failure indication to
 **               SSA for a unexpected inc options requset.
 **
 *****************************************************************************/
icf_return_t icf_sic_send_failure_response_for_unxepected_inc_options(
        icf_sic_pdb_st  *p_sic_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_options_ind_to_app
 **
 ** DESCRIPTION:  This routine is used to send incoming options
 **				  Indication to Application
 **
 *****************************************************************************/
icf_return_t icf_sic_send_options_ind_to_app(
         icf_sic_pdb_st  *p_sic_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_media_capab_req_to_mmi
 **
 ** DESCRIPTION:  This routine is used for sending media_capablities 
 **				  request to MMI
 **
 *****************************************************************************/
icf_return_t icf_sic_send_media_capab_req_to_mmi(
         icf_sic_pdb_st  *p_sic_pdb,
		 icf_list_st	 *p_app_media_capab);


/*****************************************************************************
 ** FUNCTION:     icf_sic_stop_options_timer
 **
 ** DESCRIPTION:  This routine is used for stoping the options timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_stop_options_timer(
         icf_sic_pdb_st  *p_sic_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_sic_map_icf_internal_list_icf_list
 **
 ** DESCRIPTION:  This routine is used to create icf_list from
 **				  icf_internal_list_st 
 **
 *****************************************************************************/
icf_return_t icf_sic_map_icf_internal_list_icf_list(
         icf_sic_pdb_st  *p_sic_pdb,
	 icf_internal_list_st *p_internal_list,
	 icf_list_st **p_list);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_extract_mm_stream_capab
 *
 * DESCRIPTION:     This function is invoked for extracting
 *                   media capablities of MM and it will be stored in
 *                  p_mm_media_capab of sic_ctx
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_extract_mm_stream_capab(
        INOUT icf_sic_pdb_st  *p_sic_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_form_final_capabilities
 *
 * DESCRIPTION:     This function is invoked for extracting
 *                   media capablities of MM and it will be stored in
 *                  p_mm_media_capab of sic_ctx
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
******************************************************************************/

icf_return_t icf_sic_form_final_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_send_options_success_resp_to_ssa
 *
 * DESCRIPTION:     This function is invoked for 
 *                  sending 200 OK for OPTIONS request
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_send_options_success_resp_to_ssa(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st *p_sdp_list);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_form_options_ind_media_capab_list
 *
 * DESCRIPTION:     This function is invoked for creating
 *                   media capablities list from p_mm_media_capab. 
 *					
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
*****************************************************************************/

icf_return_t icf_sic_form_options_ind_media_capab_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_app_list);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_find_stream_id_from_deleted_list
 *
 * DESCRIPTION:     This function is invoked for finding 
 *                  stream_id for deleted stream ids list 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_boolean_t icf_sic_find_stream_id_from_deleted_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
	INOUT icf_stream_list_st *p_deleted_list,
	INOUT icf_uint32_t count,
	INOUT icf_stream_id_t stream_id);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_find_stream_id_from_app_list
 *
 * DESCRIPTION:     This function is invoked for finding 
 *                  stream_id from app modified list
 *
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ****************************************************************************/
icf_boolean_t icf_sic_find_stream_id_from_app_list(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
	INOUT icf_list_st *p_app_list,
	INOUT icf_stream_id_t stream_id,
	INOUT icf_stream_capab_st **p_app_stream);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_free_final_capabilities
 *
 * DESCRIPTION:     This function frees the  extracted
 *                   media capablities of MM 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_free_final_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list);

/******************************************************************************
 *
 * FUNCTION:        icf_sic_free_mm_capabilities
 *
 * DESCRIPTION:     This function frees the  extracted
 *                   media capablities 
 *
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_sic_free_mm_capabilities(
        INOUT icf_sic_pdb_st  *p_sic_pdb,
		INOUT icf_list_st **p_sdp_list);

/* ICF_6_1_OPTIONS_SUPPORT_END */
/*******************************************************************************
 *
 * FUNCTION:        icf_sic_send_options_resp   
 *
 * DESCRIPTION:     This function sends the OPTIONS resp to SSA.
 *
 ******************************************************************************/
icf_return_t icf_sic_send_options_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

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
        OUT     icf_error_t          *p_ecode);

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
        OUT     icf_error_t           *p_ecode);

/******************************************************************************
 * FUNCTION:        icf_sic_send_failure_response_to_app
 * DESCRIPTION:     This function sends the failure response to APP
 * 					this function takes input only for api_id and the error 
 * 					code to be sent to the APP so fill the api_id in internal msg
 * 					and error_code in the sic context before triggering this function
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 ******************************************************************************/

icf_return_t icf_sic_send_failure_response_to_app
(
	INOUT icf_sic_pdb_st  *p_sic_pdb
);

/******************************************************************************
 * FUNCTION:        icf_sic_free_sdp_list
 * DESCRIPTION:     This function is invoked to free the p_sic_pdb
 *					p_sic_pdb->p_sic_ctx->p_sdp_list	
 * RETURNS:         ICF_SUCCESS  
 *                         
 ******************************************************************************/

icf_return_t icf_sic_free_sdp_list
(
	icf_sic_pdb_st *p_sic_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_init_pdb
 *
 * DESCRIPTION:     This is a utility function that initializes SIC pdb with
 *                  default values.
 *
 ******************************************************************************/
icf_return_t icf_sic_init_pdb(
        INOUT icf_sic_pdb_st *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_04_early_session_cfm
 *
 * DESCRIPTION:         This Function will be invoked when OUt of
 *                      dialog OPTIONS received from network
 *                      ICF_EARLY_SESSION_IND sent to application
 *                      and Application responded with
 *                      ICF_EARLY_SESSION_CFM
 *
 * CURRENT_STATE:       ICF_SIC_04_AWT_EARLY_SESSION_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_EARLY_SESSION_CFM
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_01_AWT_MEDIA_CAPAB_RSP
 *                      2) ICF_SIC_02_AWT_OPTIONS_APP_CFM
 *
 ******************************************************************************/
 icf_return_t icf_sic_04_early_session_cfm(
                icf_sic_pdb_st  *p_sic_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_sic_send_early_session_ind_to_app
 **
 ** DESCRIPTION:  This routine is used to send ICF_EARLY_SESSION_IND
 **                               Indication to Application
 **
 *****************************************************************************/
icf_return_t icf_sic_send_early_session_ind_to_app(
         icf_sic_pdb_st  *p_sic_pdb);

/* Added for Asynchronous NOTIFY support */

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_00_inc_message_req
 *
 * DESCRIPTION:         This Function will be invoked when an SIP MESSAGE
 *                      Request is received from network.
 *
 * CURRENT_STATE:       ICF_SIC_00_MESSAGE_IDLE
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_INC_MESSAGE_REQ
 *     
 * NEXT_STATES:
 *                      1) ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 ******************************************************************************/
icf_return_t icf_sic_00_inc_message_req(
                icf_sic_pdb_st  *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_message_cfm
 *
 * DESCRIPTION:         This Function will be invoked when application sends
 *                      icf_message_cfm_st API to ICF.
 *
 * CURRENT_STATE:       ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_MESSAGE_APP_CFM
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_MESSAGE_IDLE
 *
 ******************************************************************************/
 icf_return_t icf_sic_01_message_cfm(
                icf_sic_pdb_st  *p_sic_pdb);


/*******************************************************************************
 *
 * FUNCTION:            icf_sic_01_msg_cfm_timer_exp
 *
 * DESCRIPTION:         This Function will be invoked when Message timer
 *                      expiry event is received
 *
 * CURRENT_STATE:       ICF_SIC_01_AWAIT_MESSAGE_CFM
 *
 * CURRENT_EVENT:       ICF_SIC_EVENT_AWT_MESSAGE_CFM_TIMER_EXP
 *
 * NEXT_STATES:
 *                      1) ICF_SIC_00_MESSAGE_IDLE
 *
 ******************************************************************************/
icf_return_t icf_sic_01_msg_cfm_timer_exp(
                icf_sic_pdb_st  *p_sic_pdb);

/*****************************************************************************
 ** FUNCTION:		icf_sic_map_to_message_event
 **
 * DESCRIPTION:		This function will Map external event to message fsm
 *					EVENT. if external event is not related to message
 *					fsm then this function will return 
 *					ICF_SIC_MAX_MESSAGE_EVENTS to p_message_event.
 *
 ****************************************************************************/
icf_return_t icf_sic_map_to_message_event(
	icf_sic_pdb_st *p_sic_pdb,
	icf_uint32_t *p_message_event);

/*****************************************************************************
 ** FUNCTION:     icf_sic_start_message_timer
 **
 ** DESCRIPTION:  This routine is used for starting the message timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_start_message_timer(
         icf_sic_pdb_st  *p_sic_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_sic_stop_message_timer
 **
 ** DESCRIPTION:  This routine is used for stopping the message timer
 **				 
 **
 *****************************************************************************/
icf_return_t icf_sic_stop_message_timer(
         icf_sic_pdb_st  *p_sic_pdb);





/*****************************************************************************
 * FUNCTION:     icf_sic_start_notify_cfm_timer
 *
 * DESCRIPTION:  This function is used for starting the Notify Confirm
 *
 *****************************************************************************/
icf_return_t icf_sic_start_notify_cfm_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*****************************************************************************
 * FUNCTION:     icf_sic_stop_notify_cfm_timer
 *
 * DESCRIPTION:  This function is used for stopping the Notify Confirm
 *
 *****************************************************************************/
icf_return_t icf_sic_stop_notify_cfm_timer(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_notify_cfm
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_NOTIFY_CFM
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_notify_cfm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_timer_expiry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_NOTIFY_CFM_TIMER_EXP
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_timer_expiry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_subs_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_SUBS_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_subs_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_unsub_req
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_UNSUB_REQ 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_unsub_req(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_success_resp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_SUCCESS_RESP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_success_resp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_failure_resp_no_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_failure_resp_no_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_failure_resp_with_retry
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_failure_resp_with_retry(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_duration_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_DURATION_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_duration_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 * FUNCTION:        icf_sic_05_retry_timer_exp
 * 
 * DESCRIPTION:     This function is called when - 
 *                  State - ICF_SIC_05_AWAIT_NOTIFY_CFM
 *                  Event - ICF_SIC_EVENT_RETRY_TIMER_EXP 
 *                  
 ******************************************************************************/
icf_return_t icf_sic_05_retry_timer_exp(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*****************************************************************************
 * FUNCTION:     icf_sic_move_to_state_await_notify_cfm
 *
 * DESCRIPTION:  This function performs the actions required before moving into
 *               SIC FSM state ICF_SIC_05_AWAIT_NOTIFY_CFM.
 *               
 *               The actions performed are:
 *               - Starting notify cfm timer
 *               - Storing current state in prev state
 *               - Setting new current state as ICF_SIC_05_AWAIT_NOTIFY_CFM
 *
 *****************************************************************************/
icf_return_t icf_sic_move_to_state_await_notify_cfm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*****************************************************************************
 * FUNCTION:     icf_sic_post_notify_resp_processing
 *
 * DESCRIPTION:  This function performs the actions required after an in dialog 
 *               NOTIFY has been responded to. This function is called only when
 *               the application has enabled support of asynchronous NOTIFY.
 *               SIC FSM state ICF_SIC_05_AWAIT_NOTIFY_CFM.
 *               
 *               The actions performed are:
 *               - Setting of current state
 *               - Deallocation of SIC context if required
 *               - Handling of pending events
 *
 *****************************************************************************/
icf_return_t icf_sic_post_notify_resp_processing(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*********************************************************************************
 *
 * FUNCTION:        icf_sic_process_02_notify_term_retry_imm
 *
 * DESCRIPTION:     This is a utility function that performs the handling done 
 *                  by the FSM handler function icf_sic_02_notify_term_retry_imm.
 *                  This function is called in the following two scenarios:
 *                  - In state 02 when application has disabled support
 *                    of asynchronous NOTIFY
 *                  - In state 05 when application has enabled support
 *                    of asynchronous NOTIFY
 *
 ********************************************************************************/
icf_return_t icf_sic_process_02_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*********************************************************************************
 *
 * FUNCTION:        icf_sic_process_03_notify_term_retry_imm
 *
 * DESCRIPTION:     This is a utility function that performs the handling done 
 *                  by the FSM handler function icf_sic_03_notify_term_retry_imm.
 *                  This function is called in the following two scenarios:
 *                  - In state 03 when application has disabled support
 *                    of asynchronous NOTIFY
 *                  - In state 05 when application has enabled support
 *                    of asynchronous NOTIFY
 *
 ********************************************************************************/
icf_return_t icf_sic_process_03_notify_term_retry_imm(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/**********************************************************************************
 *
 * FUNCTION:        icf_sic_process_02_notify_term_retry_after
 *
 * DESCRIPTION:     This is a utility function that performs the handling done 
 *                  by the FSM handler function icf_sic_02_notify_term_retry_after.
 *                  This function is called in the following two scenarios:
 *                  - In state 02 when application has disabled support
 *                    of asynchronous NOTIFY
 *                  - In state 05 when application has enabled support
 *                    of asynchronous NOTIFY
 *
 *********************************************************************************/
icf_return_t icf_sic_process_02_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_sic_process_03_notify_term_retry_after
 *
 * DESCRIPTION:     This is a utility function that performs the handling done 
 *                  by the FSM handler function icf_sic_03_notify_term_retry_after.
 *                  This function is called in the following two scenarios:
 *                  - In state 03 when application has disabled support
 *                    of asynchronous NOTIFY
 *                  - In state 05 when application has enabled support
 *                    of asynchronous NOTIFY
 *
 ******************************************************************************/
icf_return_t icf_sic_process_03_notify_term_retry_after(
        INOUT icf_sic_pdb_st      *p_sic_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SIC_PROTOTYPES_H__ */

