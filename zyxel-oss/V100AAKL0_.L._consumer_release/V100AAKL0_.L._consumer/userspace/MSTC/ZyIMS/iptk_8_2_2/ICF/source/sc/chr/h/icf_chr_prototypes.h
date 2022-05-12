/*******************************************************************************
*
* FILE NAME    : icf_chr_prototypes.h
*
* DESCRIPTION  : Contains the prototypes of functions used in CHR service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Dec-2004  Amit Sharma     None           Created Origional
*  19-May-2004  Amit Sharma     SPR:7461       Added 00_call_release
*  11-Jul-2007  Neha Choraria   ICF 7.0        Function icf_chr_generate_timer
*                                              commented for changes in retry-
*                                              after.
*  24-Aug-2007  Amit Sharma                    IPTK merge from PQT6.5
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_CHR_PROTOTYPES_H__
#define __ICF_CHR_PROTOTYPES_H__

#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_chr_types.h"

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
* FUNCTION NAME    : icf_chr_process_mesg
*
* DESCRIPTION  : This is the Entry Point function of CHR service module
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_chr_process_msg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        );    

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_00_call_hold
*
* DESCRIPTION  : This function is called when call hold request receives in 
*                idle state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_HOLD
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_00_call_hold(
        INOUT   icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_hold
*
* DESCRIPTION  : This function is called when call hold request receives in 
*                following states
*                State: ICF_CHR_00_IDLE
*                       ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                       
*                Event: ICF_CHR_CALL_HOLD
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_call_hold(
        INOUT   icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_mms_resp
*
* DESCRIPTION  : This function is called when mms response receives in 
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_MMS_RESP
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_01_mms_resp(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_media_error
*
* DESCRIPTION  : This function is called when media error received
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_MEDIA_ERROR
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_01_media_error(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_media_error
*
* DESCRIPTION  : This function is called when media error received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_MEDIA_ERROR
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_media_error(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_nw_hld_rsm_suc_resp
*
* DESCRIPTION  : This function is called when success response for 
*                call hold or resume is received from nw in 
*                following state
*                State: ICF_CHR_01_AWT_NW_ACK
*                Event: ICF_CHR_NW_HOLD_RESUME_SUCC_RESP
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_nw_hldrsm_suc_resp(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_00_call_resume
*
* DESCRIPTION  : This function is called when call resume request receives in 
*                idle state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_RESUME
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_00_call_resume(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_resume
*
* DESCRIPTION  : This function is called when call resume receives in 
*                following state
*                State: ICF_CHR_00_IDLE
*                       ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                       
*                Event: ICF_CHR_CALL_RESUME
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_call_resume(
         INOUT  icf_chr_pdb_st *p_pdb);

icf_return_t icf_chr_01_hld_rsm_req_frm_nw(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_hld_rsm_req_from_nw
*
* DESCRIPTION  : This function is called when request from nw is received
*                for call hold or resume in 
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                       ICF_CHR_02_AWT_NW_ACK
*                       ICF_CHR_03_AWT_FOR_CANCEL_RESP
*                       ICF_CHR_04_AWT_START_PROC_SIGNALING
*                       ICF_CHR_05_AWT_START_PROC_MEDIA
*                Event: ICF_CHR_HOLD_RESUME_REQ_FROM_NW
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_hldrsm_req_frm_nw(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_03_nw_req_cnl_resp
*
* DESCRIPTION  : This function is called when success response for 
*                CANCEL is received from nw in 
*                following state
*                State: ICF_CHR_03_ACK_FOR_CANCEL_RESP
*                Event: ICF_CHR_NW_ACK_FOR_REQ_CANCEL
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_03_nw_req_cnl_resp(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_nw_og_setup_fail
*
* DESCRIPTION  : This function is called when failure response for 
*                call hold or resume is received from nw in 
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_NW_OUTGOING_SETUP_FAIL
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_nw_og_setup_fail(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_03_nw_og_setup_fail
*
* DESCRIPTION  : This function is called when failure response for INVITE
*                received in 
*                following state
*                State: ICF_CHR_03_ACK_FOR_CANCEL_RESP
*                Event: ICF_CHR_NW_OUTGOING_SETUP_FAIL
*                
*
* RETURNS:  
*
*******************************************************************************/

icf_return_t icf_chr_03_nw_og_setup_fail(
        INOUT  icf_chr_pdb_st    *p_pdb
        );

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_01_AWT_MMS_RESP
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_01_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_03_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_03_AWT_CANCEL_RESP
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_03_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb);


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_timer_expiry
*
* DESCRIPTION  : This function is called when timer expiry received
*                following state
*                State: ICF_CHR_04_AWT_ST_PROC_SIG
*                       ICF_CHR_05_AWT_ST_PROC_MEDIA
*                Event: ICF_CHR_TIMER_EXPIRY
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_timer_expiry(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_xx_call_release
*
* DESCRIPTION  : This function is called when call release is received
*                following state
*                State: ICF_CHR_04_AWT_ST_PROC_SIG
*                       ICF_CHR_05_AWT_ST_PROC_MEDIA
*                Event: ICF_CHR_ALL_RELEASED
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_xx_call_release(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_02_call_release
*
* DESCRIPTION  : This function is called when call release received
*                following state
*                State: ICF_CHR_02_AWT_NW_ACK
*                Event: ICF_CHR_CALL_RELEASED
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_02_call_release(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_04_st_proc_sig
*
* DESCRIPTION  : This function is called when Start Proc signaling  
*                is received from CM in 
*                following state
*                State: ICF_CHR_04_START_PROC_SIGNALING
*                Event: ICF_CHR_START_PROC_SIGNALING
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_04_st_proc_sig(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_05_st_proc_media
*
* DESCRIPTION  : This function is called when Start Proc media
*                is received from CM in 
*                following state
*                State: ICF_CHR_05_START_PROC_MEDIA
*                Event: ICF_CHR_START_PROC_MEDIA
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_05_st_proc_media(
         INOUT  icf_chr_pdb_st *p_pdb);



/******************************************************************************
 ******************Prototypes of all utility function**************************
 ******************************************************************************/

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_send_resume_resp
*
* DESCRIPTION  : This function sends Call Resume response to ADM 
*                which sends it to PA. To send response function
*                creates an internal message for ADM entry point function
*                and fill it for PP_CC_PA_CALL_RESUME_RESP API.
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_send_resume_resp(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_uint8_t    resp,
        IN     icf_error_t    reason );

#if 0
/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_fill_pa_msg_hdr
*
* DESCRIPTION  : This function fills the icf_api_header_st to send message 
*                to PA
*                
*
* RETURNS:
*
*******************************************************************************/
icf_void_t icf_chr_fill_pa_msg_hdr(
        INOUT   icf_chr_pdb_st   *p_pdb,
        INOUT   icf_uint8_t *p_msg,
        IN      icf_uint8_t  api_id
        );

/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_fill_pa_resume_resp
*
* DESCRIPTION  : This function fills the icf_cc_fill_pa_call_resume_resp
*                
*
* RETURNS: 
*
*******************************************************************************/
icf_void_t icf_chr_fill_pa_resume_resp(
        INOUT   icf_chr_pdb_st                 *p_pdb,
        INOUT   icf_cc_pa_call_resume_resp_st  *p_call_resume_resp,
        IN      icf_uint8_t    resp,
        IN      icf_uint8_t    error_cause
        );
#endif

/*******************************************************************************
*
* FUNCTION :  icf_chr_fill_internal_msg_hdr
*
* DESCRIPTION  : This function fills the icf_internal_msg_st to send message 
*                 to ADM
*                
*
* RETURNS: 
*
*******************************************************************************/
icf_void_t icf_chr_fill_internal_msg_hdr (
        INOUT  icf_internal_msg_st *p_int_msg,
        IN     icf_uint8_t         msg_id,
        IN     icf_uint8_t         len
        );

/*******************************************************************************
*
* FUNCTION NAME    :  icf_chr_send_hold_resp
*
* DESCRIPTION  : This function sends Call Hold response to ADM 
*                which sends it to PA. To send response function
*                creates an internal message for ADM entry point function
*                and fill it for PP_CC_PA_CALL_HOLD_RESP API.
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_send_hold_resp(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_uint8_t    resp,
        IN     icf_error_t    reason );
#if 0
/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_fill_pa_call_hold_resp
*
* DESCRIPTION  : This function fills the icf_cc_fill_pa_call_hold_resp
*                
*
* RETURNS:  ICF_SUCCESS
*
*******************************************************************************/

icf_void_t icf_chr_fill_pa_call_hld_resp(
        INOUT  icf_chr_pdb_st *p_pdb,
        INOUT   icf_cc_pa_call_hold_resp_st  *p_call_hold_resp,
        IN      icf_uint8_t    resp,
        IN      icf_uint8_t    error_cause
        );
#endif

/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_mmi_send_resume_mesg
*
* DESCRIPTION  : This function sends Call Resume Indication to MMI,
*                which triggers Modify Media req. To send indicaion function
*                creates an internal message for MMI entry point function
*                for API ICF_SC_MODIFY_MEDIA_SESSION_REQ.
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_mmi_send_resume_mesg(
        INOUT  icf_chr_pdb_st *p_pdb );

/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_mmi_send_hold_mesg
*
* DESCRIPTION  : This function sends Call Hold Indication to MMI,
*                which triggers Modify Media req. To send indicaion function
*                creates an internal message for MMI entry point function
*                for API <TO DO>.
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_mmi_send_hold_msg(
        INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_reinvite_mesg
*
* DESCRIPTION  : This function triggers SSA to send INVITE request on NW,
*                So it creates an internal message for SSA entry point function
*                for API ICF_SA_MEDIA_CHANGE.
*                Check mode needed or not
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_ssa_send_reinvite_msg(
        INOUT  icf_chr_pdb_st *p_pdb,
        IN     icf_boolean_t  call_state
        );

/*******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_cancel_mesg
*
* DESCRIPTION  : This function triggers SSA to send CANCEL request on NW 
*                to cancel already sent INVITE,
*                So it creates an internal message for SSA entry point function
*                for API ICF_SA_ABORT.
*                
*
* RETURNS:  
*******************************************************************************/
icf_return_t icf_chr_ssa_send_cancel_msg(
        INOUT  icf_chr_pdb_st *p_pdb);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_invite_reject 
*
* DESCRIPTION  : This function triggers SSA to send INVITE REJECTED response
*                for an incoming INVITE request,
*                So it creates an internal message for SSA entry point function
*                for API ICF_SA_REJECT.
*                
*
* RETURNS:  
*******************************************************************************/
 icf_return_t icf_chr_ssa_send_invite_reject
    (
     INOUT  icf_chr_pdb_st *p_pdb
    );


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_og_setup_succ_ack 
*
* DESCRIPTION  : This function triggers SSA to send 
*                ICF_NW_INC_CALL_SETUP_ACK
*                response for an outgoing INVITE request Success response,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_INC_CALL_SETUP_ACK.
*                
*
* RETURNS:  
*******************************************************************************/

icf_return_t icf_chr_ssa_send_og_setup_succ_ack(
        INOUT  icf_chr_pdb_st *p_pdb
        );


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_ssa_send_og_setup_fail_ack 
*
* DESCRIPTION  : This function triggers SSA to send 
*                ICF_NW_OTG_CALL_REJECT_ACK
*                response for an outgoing INVITE request failure response,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_OTG_CALL_REJECT_ACK.
*                
*
* RETURNS:  
*******************************************************************************/

icf_return_t icf_chr_ssa_send_og_setup_fail_ack(
        INOUT  icf_chr_pdb_st *p_pdb
        );
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_unexpected_event
*
* DESCRIPTION  : This function is used when any unexpected event receives
*
*
* RETURNS:
******************************************************************************/ 
icf_return_t  icf_chr_unexpected_event(
        INOUT  icf_chr_pdb_st *p_pdb);

#if 0
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_generate_timer
*
* DESCRIPTION  : This function is used to generate random timer 
*
* RETURNS:
******************************************************************************/

icf_return_t icf_chr_generate_timer(
        INOUT icf_duration_t     *timer_val,
        IN    icf_uint16_t     st_val,
        IN    icf_uint16_t     end_val);

#endif

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_check_other_call_act
*
* DESCRIPTION  : This function is used to check whether any other call 
*                is active on same line
*
* RETURNS:
******************************************************************************/
icf_return_t icf_chr_check_other_call_act(
        INOUT icf_chr_pdb_st     *p_pdb,
        OUT     icf_boolean_t    *other_call_active);


/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_check_recvd_sdp_info
*
* DESCRIPTION  : This function is used to check whether remote SDP is same
*                as previous or not
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_check_recvd_sdp_info(
       INOUT icf_chr_pdb_st   *p_pdb);

/******************************************************************************
* FUNCTION NAME    :   icf_chr_check_recvd_mode
* DESCRIPTION  : This function is used to check whether remote mode is same
*                as previous or not
* RETURNS: ICF_FAILURE in case of any change in remote media
*          else ICF_SUCCESS
******************************************************************************/
icf_return_t   icf_chr_check_recvd_mode(
       INOUT icf_chr_pdb_st   *p_pdb);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_check_recvd_codec_info
*
* DESCRIPTION  : This function is used to check whether remote CODEC is same
*                as previous or not
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_check_recvd_codec_info(
       INOUT icf_chr_pdb_st   *p_pdb);


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_trace
*
* DESCRIPTION  : This function is used to initialize trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_chr_trace(
        INOUT icf_chr_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_app_trace
*
* DESCRIPTION  : This function is used to initialize trace data
*
* RETURNS:
******************************************************************************/
icf_return_t icf_chr_app_trace(
        INOUT icf_chr_pdb_st     *p_pdb,
        IN    icf_trace_id_t     trace_id);
#endif

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_external_api_to_event
* 
* DESCRIPTION  : This function converts incoming APIs to internal EVENTs
*
* RETURNS:
*******************************************************************************/
icf_uint8_t  icf_chr_external_api_to_event(
        INOUT  icf_chr_pdb_st     *p_pdb,
        INOUT  icf_api_header_st  *p_api_hdr,
        OUT    icf_uint8_t        *p_event
        );


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_convert_api_to_event
* 
* DESCRIPTION  : This function converts incoming APIs to internal EVENTs
*
* RETURNS:
*******************************************************************************/
icf_return_t  icf_chr_convert_api_to_event(
        INOUT  icf_chr_pdb_st   *p_pdb,
        INOUT  icf_uint8_t      *p_event
        );

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_deinit_ctx
*
* DESCRIPTION  : This function is used to Deinitialize CHR context data
*
* RETURNS:
******************************************************************************/
icf_void_t  icf_chr_deinit_ctx(
        icf_chr_pdb_st  *p_pdb
        );


#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_state_trace_string
*
* DESCRIPTION  : This function is used to initialize state trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_chr_state_trace_string(
        IN    icf_uint8_t      trace_id, 
        INOUT icf_trace_data_st   *p_trace_data);



/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_event_trace_string
*
* DESCRIPTION  : This function is used to initialize event trace data
*
* RETURNS:
******************************************************************************/
icf_void_t icf_chr_event_trace_string(
        IN    icf_uint8_t      trace_id, 
        INOUT icf_trace_data_st   *p_trace_data);

#endif
/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_start_timer
*
* DESCRIPTION  : This function is used to start CHR timer
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_start_timer(
        icf_chr_pdb_st     *p_pdb,
        icf_uint8_t        timer_type);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_compare_tranport_addr
*
* DESCRIPTION  : This function is used to compare two transport addresses and
*                return false if different else returns true
*
* RETURNS:
******************************************************************************/
icf_boolean_t  icf_chr_compare_tranport_addr(
        INOUT icf_chr_pdb_st       *p_pdb,
        IN  icf_transport_address_st  *p_addr_first,
        IN  icf_transport_address_st  *p_addr_second);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_get_remote_mode
*
* DESCRIPTION  : This function is used to get the media mode to be sent with
*                network request
*
* RETURNS:
******************************************************************************/
icf_media_mode_t icf_chr_get_remote_mode(
        INOUT icf_chr_pdb_st *p_pdb);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_send_mms_for_remote_resp
*
* DESCRIPTION  : This function is used to send MMS request after final response
*                from peer is received and media parameters are changed
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_send_mms_for_remote_resp(
        INOUT icf_chr_pdb_st  *p_pdb);

/******************************************************************************
*
* FUNCTION NAME    :  icf_chr_init_pdb_data
*
* DESCRIPTION  : This function is used to initialize CHR pdb data
*
* RETURNS:
******************************************************************************/
icf_return_t   icf_chr_init_pdb_data(
        INOUT  icf_chr_pdb_st          *p_pdb,
        INOUT  icf_internal_msg_st     *p_internal_msg);

/******************************************************************************
*
* FUNCTION NAME    :   icf_chr_internal_req_handler
*
* DESCRIPTION  : This function is used to initialize internal call hold and
*                resume data
*
* RETURNS:
******************************************************************************/
icf_boolean_t  icf_chr_internal_req_handler(
        INOUT icf_chr_pdb_st   *p_pdb,
        INOUT icf_uint32_t      event
        );

/*******************************************************************************
*
* FUNCTION NAME : icf_chr_xx_forceful_call_clear
*
* DESCRIPTION   : This function is used to handle forceful call clear
*                 indication
*
* RETURNS       : icf_return_t
*******************************************************************************/
icf_return_t   icf_chr_xx_forceful_call_clear(
        INOUT  icf_chr_pdb_st    *p_pdb);

/******************************************************************************
*
* FUNCTION NAME    : icf_chr_00_call_release
*
* DESCRIPTION  : This function is called when call release received
*                following state
*                State: ICF_CHR_00_IDLE
*                Event: ICF_CHR_CALL_RELEASE
*
* RETURNS:
*******************************************************************************/
icf_return_t icf_chr_00_call_release(
         INOUT  icf_chr_pdb_st *p_pdb
         );

/******************************************************************************
*
* FUNCTION NAME: icf_chr_clone_app_spec_hdr_lists
*
* DESCRIPTION  : This function is used to update application specific headers
*                in call context
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_clone_app_spec_hdr_lists(
    icf_chr_pdb_st   *p_chr_pdb);

/*******************************************************************************
*
* FUNCTION NAME    :  icf_chr_free_api_resp
*
* DESCRIPTION  : This function frees API response message
*                
*
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_free_api_resp(
       icf_chr_pdb_st *p_pdb, 
       icf_api_resp_st *p_api_resp);

/******************************************************************************
*
* FUNCTION NAME: icf_chr_compare_audio_profile
*
* DESCRIPTION  : This function is used to compare audio profiles
*
* RETURNS:      ICF_TRUE/ICF_FALSE
******************************************************************************/
icf_boolean_t icf_chr_compare_audio_profile(icf_chr_pdb_st *p_pdb,
                    icf_audio_profile_st  *p_audio_profile_1,
                    icf_audio_profile_st  *p_audio_profile_2);

/******************************************************************************
*
* FUNCTION NAME: icf_chr_compare_codec_list
*
* DESCRIPTION  : This function is used to compare codec lists
*
* RETURNS:      
******************************************************************************/
icf_return_t icf_chr_compare_codec_list(icf_chr_pdb_st *p_pdb,
                    icf_list_st  *p_codec_list_1,
                    icf_list_st  *p_codec_list_2);

/*******************************************************************************
 *
 * FUNCTION:        icf_chr_copy_internal_sdp_stream
 *
 * DESCRIPTION:     This function will be copying internal_sdp_stream from
 *					p_src to *p_p_target.
 ******************************************************************************/
icf_return_t icf_chr_copy_remote_sdp(
	          icf_chr_pdb_st  *p_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_chr_check_remote_status
 *
 * DESCRIPTION:     This function will return success if remote mode is inactive
 ******************************************************************************/
icf_return_t icf_chr_check_remote_status(
              icf_chr_pdb_st  *p_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_chr_check_mms_resp
 *
 * DESCRIPTION:     This function will check MMS response is failure or success
 ******************************************************************************/
icf_return_t   icf_chr_check_mms_resp(
                icf_chr_pdb_st  *p_pdb,
                icf_uint8_t     *p_event);


/******************************************************************************
*
* FUNCTION NAME: icf_chr_send_app_resp_on_call_term
*
* DESCRIPTION  : This function is used to send response to the application when 
*                application has requested hold/resume but call termination is 
*                triggered.
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_send_app_resp_on_call_term(
    INOUT  icf_chr_pdb_st   *p_pdb);

/******************************************************************************
*
* FUNCTION NAME: icf_chr_is_given_audio_mode
*
* DESCRIPTION  : This function is checks whether AUDIO mode is same as given mode
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_is_given_audio_mode(
    INOUT  icf_chr_pdb_st   *p_pdb,
    icf_media_mode_t        media_mode);

/******************************************************************************
*
* FUNCTION NAME: icf_chr_set_app_media_mode
*
* DESCRIPTION  : This function sets AUDIO mode as given mode
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_set_app_media_mode(
    INOUT  icf_chr_pdb_st   *p_pdb,
    icf_media_mode_t        media_mode);

/******************************************************************************
*
* FUNCTION NAME: icf_chr_set_app_media_mode
*
* DESCRIPTION  : This function sets AUDIO mode as given mode
*
* RETURNS:
******************************************************************************/
icf_return_t  icf_chr_set_app_media_mode(
    INOUT  icf_chr_pdb_st   *p_pdb,
    icf_media_mode_t        media_mode);
/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_01_hld_rsm_req_from_nw
*
* DESCRIPTION  : This function is called when request from nw is received
*                for call hold or resume in 
*                following state
*                State: IPPSTK_CHR_01_AWT_MMS_RESP
*                Event: IPPSTK_CHR_HOLD_RESUME_REQ_FROM_NW
* RETURNS:  
*
*******************************************************************************/
icf_return_t icf_chr_01_hldrsm_req_frm_nw(
         INOUT  icf_chr_pdb_st *p_pdb);

/*******************************************************************************
 *
 * FUNCTION NAME    :   icf_chr_cc_send_reinvite_msg
 * 
 * DESCRIPTION  : This function will send the reinvite message to CC
 *                Also set the signalling ongoing status.
 *
 * RETURNS:  
 *
 *******************************************************************************/

icf_return_t icf_chr_cc_send_reinvite_msg(
        INOUT  icf_chr_pdb_st *p_pdb
        );


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* __ICF_CHR_PROTOTYPES_H__  */


