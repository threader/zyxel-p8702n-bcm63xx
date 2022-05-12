/*******************************************************************************
*
* FILE NAME    : icf_ctu_prototypes.h
*
* DESCRIPTION  : Contains the funciton prototypes used in CTU service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*  27-Jan-2005  Amit Sharma     None           Created Origional
*  19-May-2005  Amit Sharma     SPR:7561       Added 03_forceful function
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006, Aricent .
*
*******************************************************************************/

#ifndef __ICF_CTU_PROTOTYPES_H__
#define __ICF_CTU_PROTOTYPES_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_ctu_types.h"


#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

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
        );

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
         );



/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_00_call_transfer_unattended_request
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_00_IDLE
*                 Event         ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_00_call_transfer_unattended_request(
        INOUT  icf_ctu_pdb_st    *p_pdb);




/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_00_internal_refer_req
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_00_IDLE
*                 Event         ICF_CTU_INTERNAL_REFER_REQUEST 
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_00_internal_refer_req(
        INOUT  icf_ctu_pdb_st    *p_pdb);




/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_02_call_resume_resp
*
* DESCRIPTION   : Handler function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTU_RESUME_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_02_call_resume_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_02_call_hold_resp
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                 Event         ICF_CTU_HOLD_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_02_call_hold_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_refer_resp
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_REFER_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_03_refer_resp(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_01_start_proc_sig
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_01_WAIT_FOR_START_PROC_SIG
*                 Event         ICF_CTU_START_PROC_SIG
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_01_start_proc_sig(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_cc_call_released
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF _CTU_02_WAIT_FOR_START_PROC_SIG
*                               ICF _CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_CC_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_xx_cc_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_pa_call_released
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF _CTU_02_WAIT_FOR_START_PROC_SIG
*                               ICF _CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_PA_CALL_RELEASED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_ctu_xx_pa_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_timer_expiry
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF _CTU_01_WAIT_FOR_HOLD_RESUME_RESP
*                               ICF _CTU_02_WAIT_FOR_START_PROC
*                               ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_xx_timer_expiry(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_timer_expiry
*
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_03_WAIT_FOR_REFER_RESP
*                 Event         ICF_CTU_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/

icf_return_t icf_ctu_03_timer_expiry(
        INOUT  icf_ctu_pdb_st    *p_pdb
        );


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_unexpected_event(
        INOUT   icf_ctu_pdb_st   *p_pdb);


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
        INOUT icf_ctu_pdb_st     *p_pdb);

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
        INOUT icf_ctu_pdb_st     *p_pdb);


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
        INOUT icf_ctu_pdb_st     *p_pdb);


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
        IN     icf_error_t      error_cause);


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
        INOUT  icf_ctu_pdb_st   *p_pdb);

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
        INOUT  icf_ctu_pdb_st   *p_pdb);


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
        INOUT icf_address_st  *p_address);

#ifdef ICF_TRACE_ENABLE
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
icf_void_t    icf_ctu_trace(
        INOUT icf_ctu_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);
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
        IN    icf_trace_id_t     trace_id);

#endif
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
        );
/*****************************************************************************
*
* FUNCTION NAME    :   icf_ctu_fill_pa_ctu_resp
*
* DESCRIPTION  : This function fills the 
*                icf_msg_resp_st
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_void_t icf_ctu_fill_pa_ctu_resp(
        INOUT   icf_ctu_pdb_st                              *p_pdb,
        INOUT   icf_msg_resp_st                             *p_ctu_resp,
        IN      icf_uint8_t                                 resp,
        IN      icf_uint8_t                                 error_cause);

#ifdef ICF_TRACE_ENABLE
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
        INOUT icf_trace_data_st   *p_trace_data);

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
        INOUT icf_trace_data_st   *p_trace_data);

#endif

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
        INOUT   icf_ctu_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_xx_forceful_call_clear(
        INOUT  icf_ctu_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_03_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_ctu_03_forceful_call_clear(
        INOUT  icf_ctu_pdb_st    *p_pdb);

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
    icf_ctu_pdb_st   *p_ctu_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_ctu_04_notify
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_NOTIFY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_ctu_04_notify(
        INOUT   icf_ctu_pdb_st   *p_pdb);


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
        INOUT icf_ctu_pdb_st     *p_pdb);

/*******************************************************************************
* FUNCTION NAME : icf_ctu_04_cc_call_released
* DESCRIPTION   : Handler Function for
*                 State(s)      ICF_CTU_04_WAIT_FOR_NOTIFY
*                 Event         ICF_CTU_CC_CALL_RELEASED
* RETURNS       : icf_return_t
*******************************************************************************/

icf_return_t icf_ctu_04_cc_call_released(
        INOUT  icf_ctu_pdb_st    *p_pdb);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_CTU_PROTOTYPES_H__ */
