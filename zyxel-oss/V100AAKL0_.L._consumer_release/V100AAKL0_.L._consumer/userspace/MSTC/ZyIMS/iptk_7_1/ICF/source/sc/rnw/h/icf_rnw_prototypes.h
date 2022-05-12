/*****************************************************************************
*
* FILE NAME    : icf_rnw_prototypes.h
*
* DESCRIPTION  : Contains the prototypes used in RNW service
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*  10-Mar-2005  Amit Sharma     None           Created Origional
*  30-Mar-2005  Amit Sharma     None           Added new prototypes
*  26-May-2005  Amit Sharma     None           Changed prototype of get_other_call
* 29-May-2005    Amit Sharma   SPR:7568      Forceful call clear in idle state
* 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_RNW_PROTOTYPES_H__
#define __ICF_RNW_PROTOTYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_rnw_defs.h"
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_sc_common_types.h"

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_00_refer
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_00_IDLE
*                 Event         ICF_RNW_REFER
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_00_refer(
        INOUT  icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_refer
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*
*                 Event         ICF_RNW_REFER
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_xx_refer(
        INOUT  icf_rnw_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_01_media_committed
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                 Event         ICF_RNW_MEDIA_COMMITTED
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_01_media_committed(
        INOUT  icf_rnw_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_02_start_proc_media
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_02_AWT_START_PROC_MEDIA
*                 Event         ICF_RNW_START_PROC_MEDIA
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_02_start_proc_media(
        INOUT   icf_rnw_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_merge_media_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                 Event         ICF_RNW_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_03_merge_media_resp (
        INOUT  icf_rnw_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_start_proc_signaling
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_START_PROC_SIGNALING
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t icf_rnw_xx_start_proc_signaling(
        INOUT  icf_rnw_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_notify_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                 Event         ICF_RNW_NOTIFY_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_notify_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_06_notify_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_06_AWT_NOTIFY_ACK
*                               
*                 Event         ICF_RNW_NOTIFY_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_06_notify_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_invite_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_INVITE_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_invite_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_04_invite_succ_resp
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_04_AWT_FOR_CONNECT
*                               
*                 Event         ICF_RNW_INVITE_SUCCESS_RESP
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_04_invite_succ_resp(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_media_error
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_MEDIA_ERROR_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_media_error(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               
*                 Event         ICF_RNW_CALL_RELEASED_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_03_call_released(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_call_released
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_CALL_RELEASED_REFERING_CALL
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_call_released(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_RNW_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_timer_expiry(
        INOUT    icf_rnw_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_07_timer_expiry
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM
*                 Event         ICF_RNW_TIMER_EXPIRY
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_07_timer_expiry(
        INOUT    icf_rnw_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_internal_rnw_rej
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      ICF_RNW_00_IDLE
*                               ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_internal_rnw_rej(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_xx_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      
*                               ICF_RNW_01_AWT_MEDIA_COMMIT
*                               ICF_RNW_02_AWT_START_PROC_MEDIA
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                               ICF_RNW_04_AWT_FOR_CONNECT
*                               ICF_RNW_05_AWT_START_PROC_SIGNALING
*                               ICF_RNW_06_AWT_NOTIFY_ACK
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_xx_forceful_call_clear(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_verify_inc_refer
*
* DESCRIPTION   : This function is used to verify incoming REFER message
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_verify_inc_refer(
        INOUT  icf_rnw_pdb_st    *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_send_refer_resp
*
* DESCRIPTION   : This function is used to send REFER response message to SSA
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_send_refer_resp(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        IN     icf_uint8_t       result);


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_send_refer_ind
*
* DESCRIPTION   : This function is used to send REFER indication message to PA
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_send_refer_ind(
        INOUT  icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_fill_pa_msg_hdr
*
* DESCRIPTION  : This function fills the icf_api_header_st to send message 
*                to PA
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_void_t icf_rnw_fill_pa_msg_hdr(
        INOUT   icf_rnw_pdb_st *p_pdb,
        INOUT   icf_msg_st     *p_msg,
        IN      icf_api_id_t    api_id,
        IN      icf_uint16_t    api_len
        );


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_fill_call_ctx
*
* DESCRIPTION   : This function is used to fill newly allocated call context
*
* RETURNS       : icf_return_t
*
*****************************************************************************/
icf_return_t icf_rnw_fill_call_ctx(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st   *p_call_ctx);


/****************************************************************************
*
* FUNCTION NAME    :   icf_rnw_check_med_for_merge_req
*
* DESCRIPTION  : This function is used to check media ongoing for both the
*                calls and if media is not ongoing then send merge media req
*                
*
* RETURNS:  ICF_SUCCESS
*
*****************************************************************************/
icf_return_t  icf_rnw_check_med_for_merge_req(
        INOUT icf_rnw_pdb_st    *p_pdb);



/*****************************************************************************
 *
 * FUNCTION NAME    :   icf_rnw_send_merge_media_req
 *
 * DESCRIPTION  : This function sends Call merge Indication to MMI,
 *                which triggers Merge Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MERGE_MEDIA_SESSION_REQ.
 *               
 *
 * RETURNS:
 *
 ****************************************************************************/
icf_return_t  icf_rnw_send_merge_media_req(
        INOUT  icf_rnw_pdb_st    *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_get_other_call_id
*
* DESCRIPTION  : This function is used to get the call id of the call other
*                then one in p_glb_pdb
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_call_id_t   icf_rnw_get_other_call_id(
        INOUT   icf_rnw_pdb_st    *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_alloc_n_fill_serv_ctx
*
* DESCRIPTION  : This function is used to allocate new service context for
*                new call and fill all the information
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_rnw_alloc_n_fill_serv_ctx(
        INOUT   icf_rnw_pdb_st    *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME:   icf_rnw_send_merge_media_ind
*
* DESCRIPTION  : This function is used to send merge media ind to PA
*
* RETURNS:  ICF_SUCCESS
*
****************************************************************************/
icf_uint8_t   icf_rnw_send_merge_media_ind(
        INOUT   icf_rnw_pdb_st    *p_pdb);


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_apply_both_calls
*
* DESCRIPTION  : This function is used to fill status as both calls and also
*                fills the return path payload
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_apply_both_calls(
        INOUT   icf_rnw_pdb_st   *p_pdb
        );


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_fill_other_call_info
*
* DESCRIPTION  : This function is used to allocate return payload and
*                set call ctx pointer in it
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_fill_other_call_info(
        INOUT   icf_internal_msg_st   *p_internal_msg,
        INOUT   icf_call_ctx_st       *p_call_ctx);


/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_send_notify_msg
*
* DESCRIPTION  : This function is used to send notify message to SSA
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_send_notify_msg(
        INOUT   icf_rnw_pdb_st   *p_pdb,
        IN      icf_uint8_t      reason
        );


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_unexpected_event
*
* DESCRIPTION   : Handler for invalid (unexpected) event-state combinations.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_rnw_unexpected_event(
        INOUT   icf_rnw_pdb_st   *p_pdb);

/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_clear_call_ctx
*
* DESCRIPTION   : Function is used to deallocate memory for newly 
*                 allocated call context
*
* RETURNS       : icf_retrun_t
*
******************************************************************************/
icf_return_t   icf_rnw_clear_call_ctx(
        INOUT  icf_rnw_pdb_st    *p_pdb,
        INOUT  icf_call_ctx_st   *p_call_ctx);


/**********************************************************************
 *FUNCTION:- icf_rnw_init_pdb
 *
 *Description:- This function initialises the structure icf_rnw_pdb_st.
 *
*********************************************************************/
icf_void_t icf_rnw_init_pdb(
	   icf_rnw_pdb_st *p_rnw_pdb);


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME:   icf_rnw_trace
*
* DESCRIPTION  : This function is used to define the trace data for RNW service
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_void_t    icf_rnw_trace(
        INOUT icf_rnw_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);
/******************************************************************************
*
* FUNCTION NAME:   icf_rnw_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for RNW service
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_rnw_app_trace(
        INOUT icf_rnw_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);

/******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_rnw_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data);


/******************************************************************************
*
* FUNCTION NAME    :   icf_rnw_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_rnw_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data);


#endif

/****************************************************************************
*
* FUNCTION NAME:   icf_rnw_check_to_send_notify
*
* DESCRIPTION  : This function is used to check sognaling ongoing in other
*                modules to notify message to SSA
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_rnw_check_to_send_notify(
        INOUT   icf_rnw_pdb_st   *p_pdb);


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_start_rnw_timer
*
* DESCRIPTION   : Function is used to allocate memory for rnw timer data and
*                  start RNW timer
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_return_t    icf_rnw_start_rnw_timer(
        INOUT icf_rnw_pdb_st     *p_pdb);


/***************************************************************************
*
* FUNCTION NAME:   icf_rnw_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize RNW context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_rnw_deinit_ctx(
        INOUT icf_rnw_pdb_st  *p_pdb
        );


/*****************************************************************************
*
* FUNCTION NAME : icf_rnw_validate_incoming_refer
*
* DESCRIPTION   : Function is used to validate REFER received from network
*
* RETURNS       : icf_return_t
*
******************************************************************************/
icf_boolean_t   icf_rnw_validate_incoming_refer(
        INOUT  icf_rnw_pdb_st    *p_pdb);

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
        );


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_03_forceful_call_clear
*
* DESCRIPTION   : Handler Funtion for
*                 State(s)      
*                               ICF_RNW_03_AWT_MERGE_MEDIA_RESP
*                 Event         ICF_SC_INTERNAL_RNW_REJ
*
* RETURNS       : icf_return_t
*
*******************************************************************************/
icf_return_t    icf_rnw_03_forceful_call_clear(
        INOUT    icf_rnw_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME : icf_rnw_07_cta_initiated_cfm
*
* DESCRIPTION   : This function process the cta initiated cfm response from PA.
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_rnw_07_cta_initiated_cfm(
        INOUT   icf_rnw_pdb_st   *p_pdb);


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_RNW_PROTOTYPES_H__ */

