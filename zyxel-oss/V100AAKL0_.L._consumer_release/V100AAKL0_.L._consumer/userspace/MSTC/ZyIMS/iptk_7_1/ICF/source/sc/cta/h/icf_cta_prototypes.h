/*******************************************************************************
*
* FILE NAME    : icf_cta_prototypes.h
*
* DESCRIPTION  : Contains the funciton prototypes used in CTA service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*  24-Feb-2005  Amit Sharma     None           Created Origional
* 02-Jun-2005    Amit Sharma     SPR:7535        Wait for TWC service term
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006, Aricent .
*
*******************************************************************************/

#ifndef __ICF_CTA_PROTOTYPES_H__
#define __ICF_CTA_PROTOTYPES_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_cta_types.h"


#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

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
        );


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_00_call_transfer_attended_request
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_00_IDLE
*                 Event         ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_cta_00_call_transfer_attended_request(
        INOUT  icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_call_hold_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CALL_HOLD_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_call_hold_resp(
        INOUT   icf_cta_pdb_st   *p_pdb);



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
#ifdef ICF_TRACE_ENABLE
icf_return_t    icf_cta_app_trace(
        INOUT icf_cta_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);

#endif
/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_call_resume_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CALL_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_call_resume_resp(
        INOUT   icf_cta_pdb_st   *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_02_start_proc_sig
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTA_START_PROC_SIG
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_02_start_proc_sig(
        INOUT   icf_cta_pdb_st   *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_03_refer_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTA_REFER_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_03_refer_resp(
        INOUT   icf_cta_pdb_st   *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_04_notify
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_NOTIFY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_04_notify(
        INOUT   icf_cta_pdb_st   *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_pa_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_pa_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);


/******************************************************************************
*
* FUNCTION NAME : icf_cta_02_pa_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTA_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_02_pa_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_pa_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_pa_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_01_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_01_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_02_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_02_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);



/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_cc_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_cc_call_released(
        INOUT   icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF_CTA_02_WAIT_FOR_START_PROC_SIG
*                               ICF_CTA_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTA_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_xx_timer_expiry(
        INOUT   icf_cta_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_03_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTA_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_03_timer_expiry(
        INOUT   icf_cta_pdb_st    *p_pdb);


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
        INOUT icf_cta_pdb_st     *p_pdb);


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
        INOUT  icf_cta_pdb_st   *p_pdb);


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
        INOUT  icf_cta_pdb_st    *p_pdb);


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
        INOUT icf_cta_pdb_st     *p_pdb);


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
        INOUT icf_cta_pdb_st     *p_pdb);


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
        INOUT   icf_call_ctx_st       *p_call_ctx);


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
        );


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
        );


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
        INOUT   icf_cta_pdb_st    *p_pdb);


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
        IN     icf_error_t      error_cause);


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
        );


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
        IN    icf_trace_id_t     trace_id);


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
        INOUT icf_trace_data_st   *p_trace_data);


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
        INOUT icf_trace_data_st   *p_trace_data);
#endif

/******************************************************************************
*
* FUNCTION NAME : icf_cta_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_unexpected_event(
        INOUT   icf_cta_pdb_st   *p_pdb);

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
        INOUT icf_cta_pdb_st     *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_cta_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear 
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_xx_forceful_call_clear(
        INOUT  icf_cta_pdb_st    *p_pdb);


/******************************************************************************
*
* FUNCTION NAME : icf_cta_03_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear 
*                 indication in ICF_CTA_03_WAIT_FOR_REFER_RESP state
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_cta_03_forceful_call_clear(
        INOUT  icf_cta_pdb_st    *p_pdb);


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
        INOUT icf_cta_pdb_st     *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_cta_05_twc_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTA_05_WAIT_FOR_TWC_RESP
*                 Event         ICF_CTA_TWC_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_cta_05_twc_resp(
        INOUT   icf_cta_pdb_st   *p_pdb);

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
        INOUT icf_cta_pdb_st     *p_pdb);

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
    icf_cta_pdb_st   *p_cta_pdb);
    
icf_return_t       icf_cta_apply_early_cta_check(icf_cta_pdb_st *p_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_CTA_PROTOTYPES_H__ */
