/*******************************************************************************
*
* FILE NAME    : icf_twc_prototypes.h
*
* DESCRIPTION  : Contains the prototypes of functions used in TWC service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Jan-2005  Amit Sharma     None           Created Origional
*  17-Feb-2005  Amit Sharma     None           Change in convert API function
*  25-Apr-2005  Amit Sharma     SPR:7430       Updated prototype of
*                                              icf_twc_get_other_call_id
*  27-Feb-2006  Amit Sharma                    Merged code for ICF
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_TWC_PROTOTYPES_H__
#define __ICF_TWC_PROTOTYPES_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_twc_types.h"

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

/******************************************************************************
 **********************Prototypes of all State Machine functions***************
 ******************************************************************************/

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_process_mesg
*
* DESCRIPTION  : This is the Entry Point function of TWC service module
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_twc_process_msg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        );

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_00_call_hold
*
* DESCRIPTION  : This function is called when create conference request 
*                receives in idle state
*                
*                State: ICF_TWC_00_IDLE
*                Event: ICF_TWC_CREATE_CONFERENCE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_00_create_conference(
        INOUT   icf_twc_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_01_merge_media_resp
*
* DESCRIPTION  : This function is called when merge media response receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_MERGE_MEDIA_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_merge_media_resp(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_02_call_resume_resp
*
* DESCRIPTION  : This function is called when Call Resume response receives in 
*                following state
*                State: ICF_TWC_02_AWT_CALL_RESUME_RESP
*                Event: ICF_TWC_CALL_RESUME_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_02_call_resume_resp(
        INOUT   icf_twc_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_03_media_committed
*
* DESCRIPTION  : This function is called when media committed response 
*                for second call receives in 
*                following state
*                State: ICF_TWC_03_AWT_MEDIA_COMMITTED
*                Event: ICF_TWC_MEDIA_COMMITTED
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_03_media_committed(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_04_call_hold_resp
*
* DESCRIPTION  : This function is called when call hold response receives in 
*                following state
*                State: ICF_TWC_04_AWT_CALL_HOLD_RESP
*                Event: ICF_TWC_CHD_RESP
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_04_call_hold_resp(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_05_awt_start_proc_media
*
* DESCRIPTION  : This function is called when start proc media receives in 
*                following state
*                State: ICF_TWC_05_AWT_START_PROC_MEDIA
*                Event: ICF_TWC_START_PROC_MEDIA
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_05_awt_start_proc_media(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_01_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_call_release(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_xx_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                       ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_CHD_RESP
*                       ICF_TWC_05_AWT_START_PROC_MEDIA
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_call_release(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_06_call_release
*
* DESCRIPTION  : This function is called when call release receives in 
*                following state
*                State: ICF_TWC_06_CONFERENCE
*                Event: ICF_TWC_CALL_RELEASE
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_06_call_release(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_xx_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                       ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_06_CONFERENCE
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_03_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_03_AWT_MEDIA_COMMITTED
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_03_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_04_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_04_AWT_CHD_RESP
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_04_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_05_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_05_AWT_START_PROC_MEDIA
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_05_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_xx_service_err_ind
*
* DESCRIPTION  : This function is called when service error indication 
*                receives from other services
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                       ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_CHD_RESP
*                       ICF_TWC_05_AWT_START_PROC_MEDIA
*                       ICF_TWC_06_CONFERENCE
*                Event: ICF_TWC_SERVICE_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_service_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_set_call_ctx_in_twc_ctx
*
* DESCRIPTION  : This function is used to store call contexts of both 
*                the calls in TWC context
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_twc_set_call_ctx_in_twc_ctx(
        INOUT   icf_twc_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_start_twc_timer
*
* DESCRIPTION  : This function is used to start TWC timer
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t  icf_twc_start_twc_timer(
        INOUT icf_twc_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_send_adm_conference_resp
*
* DESCRIPTION  : This function is used to send TWC response to ADM            
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t  icf_twc_send_adm_conference_resp(
        INOUT icf_twc_pdb_st    *p_pdb, 
        IN    icf_boolean_t     result,
        IN    icf_error_t       error_cause);

/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_check_med_for_merge_req
*
* DESCRIPTION  : This function is used to check media ongoing for both the
*                calls and if media isnot ongoing then send merge media req
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/
icf_return_t  icf_twc_check_med_for_merge_req(
        INOUT icf_twc_pdb_st    *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_check_chr_service
*
* DESCRIPTION  : This function is used to check CHR service is active or not,
*                if yes then check media state, if local held then wait for 
*                resume response else wait for hold response. If HCR not active
*                then send merge media request
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_check_chr_service(
        INOUT    icf_twc_pdb_st    *p_pdb);


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME:   icf_twc_trace
*
* DESCRIPTION  : This function is used to define the trace data for TWC service
*
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_void_t    icf_twc_trace(
        INOUT icf_twc_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);
/******************************************************************************
*
* FUNCTION NAME:   icf_twc_app_trace
*
* DESCRIPTION  : This function is used to define the trace data for TWC service
*
*
* RETURNS:
*
******************************************************************************/
icf_return_t    icf_twc_app_trace(
        INOUT icf_twc_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);


#endif

/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_mm_err_ind
*
* DESCRIPTION  : This function is called when mm error indication receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_MM_ERR_IND
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_mm_err_ind(
        INOUT   icf_twc_pdb_st *p_pdb);



/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_terminate_service
*
* DESCRIPTION  : This function is used to send error indication to terminate
*                both the calls
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_terminate_service(
        INOUT    icf_twc_pdb_st    *p_pdb,
        IN       icf_uint32_t       error_id);

/*******************************************************************************
*
* FUNCTION NAME: icf_twc_01_timer_expiry
*
* DESCRIPTION  : This function is called when TWC timer expiry receives in 
*                following state
*                State: ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*                Event: ICF_TWC_TIMER_EXPIRY
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_01_timer_expiry(
        INOUT   icf_twc_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME: icf_twc_xx_timer_expiry
*
* DESCRIPTION  : This function is called when TWC timer expiry receives in 
*                following state
*                State: ICF_TWC_02_AWT_CALL_RESUME_RESP
*                       ICF_TWC_03_AWT_MEDIA_COMMITTED
*                       ICF_TWC_04_AWT_START_PROC_MEDIA
*                       ICF_TWC_05_AWT_CHD_RESP
*                Event: ICF_TWC_TIMER_EXPIRY
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_twc_xx_timer_expiry(
        INOUT   icf_twc_pdb_st *p_pdb);



/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_resp_to_pa_n_stop_timer
*
* DESCRIPTION  : This function is used to stop TWC timer and send failure
*                response to PA
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t    icf_twc_resp_to_pa_n_stop_timer(
        INOUT    icf_twc_pdb_st    *p_pdb,
        IN       icf_error_t       error_id);

/*******************************************************************************
*
* FUNCTION NAME    :   icf_twc_stop_twc_timer
*
* DESCRIPTION  : This function is used to stop TWC timer and free allocated 
*                timer block
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_twc_stop_twc_timer(
        INOUT   icf_twc_pdb_st    *p_pdb);


/*******************************************************************************
 *
 * FUNCTION NAME    :   icf_twc_send_merge_media_req
 *
 * DESCRIPTION  : This function sends Call merge Indication to MMI,
 *                which triggers Merge Media req. To send indicaion function
 *                creates an internal message for MMI entry point function
 *                for API ICF_SC_MERGE_MEDIA_SESSION_REQ.
 *               
 *
 * RETURNS:
 *
 *****************************************************************************/
icf_return_t  icf_twc_send_merge_media_req(
        INOUT  icf_twc_pdb_st    *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME:   icf_twc_get_other_call_ctx
*
* DESCRIPTION  : This function is used to get the call id of the call other
*                then one in p_glb_pdb
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_call_ctx_st*   icf_twc_get_other_call_ctx(
        INOUT   icf_twc_pdb_st    *p_pdb);

/******************************************************************************
*
* FUNCTION NAME:   icf_twc_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize TWC context data
*
* RETURNS:
******************************************************************************/

icf_void_t  icf_twc_deinit_ctx(
        icf_twc_pdb_st  *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_twc_unexpected_event
*
* DESCRIPTION  : This function is called when any unexpected event receives
*                
* RETURNS:  
*******************************************************************************/
icf_return_t   icf_twc_unexpected_event(
        INOUT   icf_twc_pdb_st   *p_pdb);


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
        );

#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME    :   icf_twc_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_twc_event_trace_string(
        IN    icf_uint8_t         event, 
        INOUT icf_trace_data_st   *p_trace_data);

/******************************************************************************
*
* FUNCTION NAME    :   icf_twc_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_twc_state_trace_string(
        IN    icf_uint8_t          state, 
        INOUT icf_trace_data_st   *p_trace_data);

#endif
/******************************************************************************
*
* FUNCTION NAME:   icf_twc_fill_other_call_info
*
* DESCRIPTION  : This function is used to allocate return payload and 
*                set call ctx pointer in it
*                
*
* RETURNS:  ICF_SUCCESS
*
******************************************************************************/
icf_return_t  icf_twc_fill_other_call_info(
        INOUT   icf_internal_msg_st   *p_internal_msg,
        INOUT   icf_call_ctx_st       *p_call_ctx);

/*******************************************************************************
*
* FUNCTION NAME : icf_twc_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_twc_xx_forceful_call_clear(
        INOUT  icf_twc_pdb_st    *p_pdb);


/******************************************************************************
*
* FUNCTION NAME : icf_twc_00_call_release
*
* DESCRIPTION   : This function is used to handle call clear
*                 indication received in state 
*                 ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
******************************************************************************/
icf_return_t   icf_twc_00_call_release(
        INOUT  icf_twc_pdb_st    *p_pdb);

/******************************************************************************
*
* FUNCTION NAME : icf_twc_01_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication received in state 
*                 ICF_TWC_01_AWT_MERGE_MEDIA_RESP
*
* RETURNS       : icf_return_t
******************************************************************************/
icf_return_t   icf_twc_01_forceful_call_clear(
        INOUT  icf_twc_pdb_st    *p_pdb);

/******************************************************************************
*
* FUNCTION NAME: icf_twc_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_twc_clone_app_spec_hdr_lists(
    icf_twc_pdb_st   *p_twc_pdb);

/**********************************************************************************
*
* FUNCTION NAME : icf_twc_send_merge_media_ind
*
* DESCRIPTION   : This function sends merge media indication to PA after merging
*                 of media
*
************************************************************************************/

icf_uint8_t   icf_twc_send_merge_media_ind(
        INOUT   icf_twc_pdb_st    *p_pdb);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ICF_SERVICE_ENABLE */
#endif /* __ICF_TWC_PROTOTYPES_H__  */
