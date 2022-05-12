/***************************************************************************
 *
 * File name        : icf_ssa_prototypes.h
 *
 * Purpose          : This file contains the prototypes
 *
 * Revision History :
 *
 * Date           Name          Ref#                 Comments
 * --------    ------------     ----                 ---------------
 * 22-Dec-2004  Umang Singh     SSA LLD              Initial
 * 29-JAN-2005  Aashish Suchdev 
 * 02-Jun-2005  Jalaj Negi      IMS CLIENT           Added prototypes for 
 *                                                   IMS CLIENT
 * 07-Jun-2005  Umang Singh     SPR 7650
 * 07-JUL-2005  Aashish Suchdev                      TCP checkin
 * 12-AUG-2005  Shekhar Thakur  ICF				     ICF additions 
 * 13-AUG-2005  Ram Narayan     ICF				     ICF additions 
 * 06-DEC-2005	Jyoti Narula	ICF 4.0				 Under ICF_4_0_CRM_START/
 *                                                      END
 * 06-Dec-2005  Navneet Gupta   ICF 4.0              Multiple App Support
 * 10-Jan-2005  Aman Arora      IPSEC               added utility functions 
 *                                                  for IPSEC
 * 16-Jan-2006  Priyanka Dua    MPH					added "make request digest"
 *													utility for MPH
 
 * 18-Jan-2006	Jyoti Narula	SPR 9042			New functions 
 *													for fixing the 
 *													duplicity of 
 *													application 
 *													specfied 
 *													tags in 
 *													From/To header.
 * 18-Jan-2006	Aman Arora		SPR 9591			modified the function proto,
 *													for funct:
 *												icf_ssa_set_refresher_method
 *												to allow setting the refresher 
 *												properly.
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0             Call Modify Feature changes
 * 30-Mar-2006  Deepti Goyal    ICF 5.0             Function Prototypes for mes
 *                                                  sage body header support.
 *
 *
 * 4-Sep-2006   Deepti Goyal    SPR 12759           Merged Options timer code.
 * 06-Sep-2006  Umang Singh     SPR 12855	    Parsing of Require hdr for 100rel
 * 27-Nov-2006  Deepti Goyal    ICF Rel 6.2         NAT Feature Support
 * 24-Nov-2006  Umang Singh     SPR 13908/CSR-1-3704087
 * 04-Jan-2007  Amit Mahajan                        INFO confirmation changes.
 * 06-Jan-2007  Krishna Ram Dhunju   ICF Rel 6.3    code related to DNS Buffer
 * 15-Jan-2007  Abhishek Dhammawat ICF Rel 6.3     Per Line Feature Additions  
 * 12-Jul-2007     Puneet Sharma
 *                 & Amit Mahajan      SPR-15259   IPTK should send 488 for 
                                                   incorrect media format.
 * 7-Sep-2007   Deepti Goyal        SPR 16019      Function to Abort reg 
 *                                                 transaction.
 * 25-Jun-2008  Rajiv Kumar         SPR 18722      Fix for CSR-1-6068965                                                    
 * 12-Jun-2009	Aman Aggarwal		SPR 19736	Added function for handling 
 *													challenge response
 * 03-Jun-2009  Rajiv Kumar         SPR 19788      Fix for CSR-1-7444867. 
 * Copyright (c) 2009, Aricent.
 **************************************************************************/
#ifndef __ICF_SSA_PROTOTYPES_H__
#define __ICF_SSA_PROTOTYPES_H__
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

/* Message Body Header Changes
 */
#define SIP_MIME_PARSING
	
#include "uatk_headers.h"
#include "stack_headers.h"
#include "icf_common_types.h"
#include "icf_ssa_types.h"

/***************************************************************************
**
** FUNCTION: icf_ssa_get_media_parameters_from_sdp
** DESCRIPTION: This function gets the media parameters from the SipMessage.
**              It converts the SDP to the SDP structure of the call
**              context.The calling function needs to pass the entire SIP
**              msg to the function.The function extracts the SDP from the
**              SIP message and then then fills the appropiate fields of 
**              the stream-level SDP structure(icf_inc_media_sdp_info_st) 
**
***************************************************************************/
icf_return_t icf_ssa_get_media_parameters_from_sdp(
        INOUT icf_ssa_pdb_st    *p_ssa_pdb,
        INOUT SipMessage        *p_sipMessage,
        INOUT    icf_uint8_t       msg_identifier,
        OUT   icf_ssa_inc_media_stream_info_st		**p_p_inc_media_sdp_info, 
        /* list of type icf_inc_media_sdp_info_st */
        OUT   icf_error_t       *p_error);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_convert_to_tran_addr
 *
 * DESCRIPTION:     This function converts the IP address
 *                  stored in the SDP body of SIP message
 *                  to the transport address understandable by the
 *                  ICF
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_convert_to_tran_addr(
        INOUT icf_uint8_t *p_remoteIP,
        OUT icf_transport_address_st *p_rtp_address);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_convert_to_IP_addr
 *
 * DESCRIPTION:     This function converts the transport address to a string
 *                  to be  sored in the SDP body of the SIP message
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_convert_to_IP_addr(
        INOUT icf_transport_address_st *p_rtp_address,
        OUT icf_uint8_t *p_remoteIP);

/*****************************************************************************
 * FUNCTION:         icf_ssa_check_no_of_m_lines
 *
 * DESCRIPTION:     Function to check the number of m-lines in SDP coming
 *                  in SIP messages from n/w.
 ******************************************************************************/
extern icf_return_t   icf_ssa_check_no_of_m_lines(
         INOUT SdpMessage *p_sdp_message,
         OUT Sdf_ty_u32bit *count);


/***************************************************************************
**
** FUNCTION:     icf_ssa_form_sdp_from_stream_sdp_info
** DESCRIPTION:  This function forms the SDP from the media parameters 
**               present in the SDP structure of the call context.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**               This function forms Sip Sdp from p_local_sdp in the list 
**               of type icf_stream_sdp_info_st
**
***************************************************************************/
extern icf_return_t icf_ssa_form_sdp_from_stream_sdp_info(
        INOUT  SdpMessage				**p_p_sdpmsg,
        INOUT    icf_ssa_pdb_st			*p_ssa_pdb,
        INOUT    icf_uint8_t			    hold_option,
        OUT   Sdf_st_error				*pError);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_otg_reg_req
 **
 ** DESCRIPTION:        This function send REG/DE-REG request on network
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_otg_reg_req(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_setup
 **
 ** DESCRIPTION:        This function sends the call setup
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_call_setup(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_change
 **
 ** DESCRIPTION:        This function sends Re-INVITE/UPDATE
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_media_change(
        icf_ssa_pdb_st *p_ssa_pdb);

#ifdef ICF_SESSION_TIMER
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_session_refresh_req
 **
 ** DESCRIPTION:        This function sends Re-INVITE/UPDATE when session
 **						timer expired and ICF acts as session timer refresher
 **						or when 422 response had received for session refresh
 **						request
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_session_refresh_req(
		icf_ssa_pdb_st *p_ssa_pdb);
/*******************************************************************************
 * FUNCTION:        icf_ssa_start_session_timer
 *
 * DESCRIPTION:     This function Starts the Session timer if it is not 
 *					started by UATK
 *
 ******************************************************************************/
extern Sdf_ty_retVal icf_ssa_start_session_timer(
											 Sdf_st_callObject **ppCallObj,
											 icf_glb_pdb_st *p_glb_pdb,
#ifdef ICF_WITH_MUATK_2_2
											 Sdf_st_eventContext *pEventContext,
#endif
											 Sdf_st_error *pErr);
/*******************************************************************************
 * FUNCTION:        icf_ssa_set_refresher_method
 *
 * DESCRIPTION:     This function sets the subsequest session refresh request
 *					for session timer
 *					
 *
 ******************************************************************************/
/* SPR 9591- modified prototype for setting the refreshed method */
Sdf_ty_retVal icf_ssa_set_refresher_method(SipMessage *p_transaction,
											  icf_glb_pdb_st *p_glb_pdb,
											  icf_ssa_ctxt_st **p_ssa_ctxt,
											  Sdf_st_error *pErr);
/*******************************************************************************
 * FUNCTION:        icf_ssa_set_session_timer_details
 *
 * DESCRIPTION:     This function sets the session timer related headers 
 *					to the default values
 *
 ******************************************************************************/

icf_return_t icf_ssa_set_session_timer_details(
	icf_ssa_pdb_st		*p_ssa_pdb,
	Sdf_st_callObject 	*p_call_obj); 

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_set_min_se
 **
 ** DESCRIPTION:        This function sets the GlbMinSe of UATK
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_set_min_se(
		icf_ssa_pdb_st *p_ssa_pdb);

#endif /*SESSION TIMER*/

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_update_abort
 **
 ** DESCRIPTION:        This function sends CANCEL for Re-INVITE/UPDATE
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_media_update_abort(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_terminate_resp
 **
 ** DESCRIPTION:        This function sends the 200 OK for the BYE message
 **                     received from peer
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_call_terminate_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_setup_resp
 **
 ** DESCRIPTION:        This function sends the call setup response
 **                     (100 TRYING) to peer
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_call_setup_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_redirect
 **
 ** DESCRIPTION:       
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_redirect(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prov_response
 **
 ** DESCRIPTION:        This function is used to send the provisional 18x
 **                     messages to the peer
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_prov_response(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_ack
 **
 ** DESCRIPTION:        This function is used to send the acknowledgement
 **                     (ACK) messages to peer.
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_ack(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_connect
 **
 ** DESCRIPTION:        This function sends the 200 OK for the INVITE
 **                     indicating that the call has been accepted.
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_connect(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_reject
 **
 ** DESCRIPTION:        This function is used to reject an incoming call
 **                     the call rejection can be because of various
 **                     reasons.The appropiate error response(3xx/4xx/5xx)
 **                     needs to be sent corresponding to the reason code
 **                     indicated by CC
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_reject(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_terminate
 **
 ** DESCRIPTION:        This function sends the BYE message to peer
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_call_terminate(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_abort
 **
 ** DESCRIPTION:        This function sends CANCEL message to peer
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_abort(
        icf_ssa_pdb_st *p_ssa_pdb);

#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_error
 *
 * DESCRIPTION:     This function is used by SSA for error handling.
 *
 ******************************************************************************/
extern icf_void_t icf_ssa_error(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        IN     Sdf_ty_s8bit   *p_method, 
        INOUT    icf_error_t ecode);
#endif

extern icf_return_t icf_ssa_process_msg(
        INOUT  icf_internal_msg_st        *p_internal_msg);

extern icf_return_t icf_ssa_process_sip_msg(
        INOUT  icf_internal_msg_st    *p_internal_msg);

extern icf_return_t icf_ssa_process_int_msg(
        INOUT  icf_internal_msg_st    *p_internal_msg);

extern icf_return_t icf_ssa_handle_internal_api(
        INOUT  icf_ssa_pdb_st     *p_ssa_pdb);

extern icf_return_t icf_ssa_process_init_complete(
        icf_ssa_pdb_st *p_ssa_pdb);

extern icf_return_t icf_ssa_set_to_in_callobj(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb);

extern icf_return_t icf_ssa_update_callobj_for_profile(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb);

extern icf_return_t icf_ssa_get_new_call_obj
        (INOUT   icf_ssa_pdb_st      *p_ssa_pdb,
         INOUT   Sdf_st_callObject      **p_p_call_obj);

extern icf_return_t icf_ssa_handle_successful_decode(
        icf_ssa_pdb_st       *p_ssa_pdb,
        Sdf_st_eventContext     *p_event_ctxt,
        Sdf_st_callObject       *p_call_obj,
        Sdf_st_error            *p_sdf_error,
        icf_error_t          *p_ecode);

extern icf_return_t icf_ssa_process_otg_intrnl_msg(
        icf_ssa_pdb_st       *p_ssa_pdb);

extern icf_return_t icf_ssa_handle_stray_msg(
        icf_ssa_pdb_st       *p_ssa_pdb,
        Sdf_st_eventContext     *p_event_ctxt,
        Sdf_st_callObject       *p_call_obj,
        Sdf_st_error            *p_sdf_error,
        icf_error_t          *p_ecode);

extern icf_return_t icf_ssa_handle_decode_failure(
        icf_ssa_pdb_st       *p_ssa_pdb,
        Sdf_st_eventContext     *p_event_ctxt,
        Sdf_st_callObject       *p_call_obj,
        Sdf_st_error            *p_sdf_error,
        icf_error_t          *p_ecode);

extern icf_return_t icf_ssa_init_pdb(
        icf_ssa_pdb_st       *p_ssa_pdb,
        icf_glb_pdb_st        *p_glb_pdb);

extern icf_void_t icf_ssa_deinit_pdb(
        INOUT    icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            *p_ssa_data,
        OUT     icf_error_t           *p_ecode);

extern icf_return_t icf_ssa_process_init_complete(
        icf_ssa_pdb_st *p_ssa_pdb);

extern icf_return_t icf_ssa_add_callid_callobj_map(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,        
        OUT   icf_ssa_ctxt_st  *p_ssa_ctxt,
        OUT   icf_error_t    *p_ecode);

extern icf_return_t icf_ssa_map_nw_callid(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
#ifdef ICF_LOOPBACK_SUPPORT
        INOUT  Sdf_st_callObject       *p_call_obj,
#endif
        INOUT  icf_uint8_t    *p_nw_call_id,
        OUT   icf_ssa_ctxt_st    **p_p_mapped_ssa_ctxt,
#ifdef ICF_LOOPBACK_SUPPORT
        OUT   Sdf_st_error            *p_sdf_error,
#endif        
		OUT   icf_error_t    *p_ecode);

#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_trace
 *
 * DESCRIPTION:     This function is used by SSA for trace handling.
 *
 ******************************************************************************/
extern icf_void_t icf_ssa_trace(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT icf_trace_id_t trace_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_app_trace
 *
 * DESCRIPTION:     This function is used by SSA for trace handling.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_app_trace(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT icf_trace_id_t trace_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_capture_trace
 *
 * DESCRIPTION:     This function is used by SSA for trace handling.
 *
 ******************************************************************************/
extern icf_void_t icf_ssa_capture_trace(
	  icf_ssa_pdb_st *p_ssa_pdb,
          SipMessage     *p_sip_msg,
          icf_uint8_t    call_dir);

/******************************************************************************
 *
 * FUNCTION:        icf_ssa_create_criteria_data
 *
 * DESCRIPTION:     This function is invoked to populate criteria data
 *
 *****************************************************************************/
icf_return_t icf_ssa_create_criteria_data(
        INOUT icf_ssa_pdb_st *p_ssa_pdb);

#endif

/*****************************************************************************
 ** FUNCTION:           icf_ssa_isHeaderPresent
 **
 ** DESCRIPTION:        This function checks for the presence of given header
 **                                     inside a SipMessage structure.
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_isHeaderPresent
        (SipMessage             *p_sipMsg,
         en_HeaderType          dHeader,
         Sdf_st_error           *p_err);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_terminate_resp
 **
 ** DESCRIPTION:        This function is used to send a 200 OK for
 **                     the BYE received
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_call_terminate_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_map_sip_resp_code
 **
 ** DESCRIPTION:        This function maps the SIP response codes to
 **                     CC specific reason codes
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_map_sip_resp_code(
        Sdf_ty_u16bit dRespCode,
        icf_ssa_reason_t *p_reason_code);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_map_cc_reason_code
 **
 ** DESCRIPTION:        This function maps the CC reason codes to
 **                     SIP response codes
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_map_cc_reason_code(
        icf_ssa_reason_t reason_code,
        Sdf_ty_u16bit *p_respcode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_map_interface_reason_code
 **
 ** DESCRIPTION:        This function maps the reason codes of interface messages 
 **                     to SIP response codes
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_map_interface_reason_code(
        icf_ssa_reason_t reason_code,
        Sdf_ty_u16bit *p_respcode);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prack
 **
 ** DESCRIPTION:        This functions sends a PRACK for the Reliable 
 **                     provisional response(RPR) received
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_prack(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_prack_resp
 **
 ** DESCRIPTION:        This functions sends a response for PRACK received 
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_prack_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:		icf_ssa_form_sip_uri_frm_e164
 ** 
 ** DESCRIPTION:	This function takes a string (E164 user ID), an instance
 **                 of icf_transport_address_st ( proxy address, self address)
 **                 address of a variable to return port number if any.
 **                 Based on the type of address in the instance of
 **                 icf_transport_address_st, it appends appropriate string
 **                 to the received user ID to return the comeplte SIP uri
 *****************************************************************************/
extern icf_return_t icf_ssa_form_sip_uri_frm_e164(
        icf_ssa_pdb_st               *p_ssa_pdb,
        icf_uint8_t                  *p_address_str,
        icf_transport_address_st     *p_transport_addr,
        icf_uint16_t                 *p_port_num);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_media_change_resp
 **
 ** DESCRIPTION:        This function is called to send resp to received 
 **                     UPDATE/RE-INVITE
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_media_change_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_refer
 **
 ** DESCRIPTION:      Sends a refer message to network
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_refer(
        icf_ssa_pdb_st *p_ssa_pdb);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_refer_resp
 **
 ** DESCRIPTION:        This function sends refer response
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_process_refer_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_make_addr_str_frm_url
 ** DESCRIPTION:  This function analyses the type filled in p_addr_spec
 **                 and based on it it accesses the data in this struct
 **                 to get a string for icf_address_st
 ***************************************************************************/
extern  icf_return_t    icf_ssa_make_addr_str_frm_url(
                   icf_glb_pdb_st        *p_glb_pdb,
                   SipAddrSpec              *p_addr_spec,
                   icf_address_type_t    *p_address_type,
                   icf_uint8_t           *p_refer_to_string);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_timer_expiry
 **
 ** DESCRIPTION:        This function process the timer expiry event
 **                     from MRM.
 **                     Currently, only UATK timers are there
 **                     but later on there will be timers for TCP
 **                     handling also.
 *****************************************************************************/
extern icf_return_t icf_ssa_process_timer_expiry(
        INOUT  icf_internal_msg_st      *p_internal_msg);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_clone_profile_data
 ** DESCRIPTION:  This function is a cut down version of the Clone
 *                Init Data API.  It is used to copy selected information
 *                from the global profile to the call object specific profile
 *                Currently we only copy default transport
 ***************************************************************************/
extern icf_return_t    icf_ssa_clone_profile_data(
                    INOUT    icf_glb_pdb_st       *p_glb_pdb,
                    INOUT      Sdf_st_initData         *p_dest_profile,
                    INOUT      Sdf_st_initData          *p_source_profile,
                    OUT     icf_error_t           *p_ecode);


/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_send_to_peer
 ** DESCRIPTION:  This function is used to send any message to peer
 ***************************************************************************/
extern icf_return_t icf_ssa_send_to_peer(
        icf_ssa_pdb_st *p_ssa_pdb,
        Sdf_st_callObject *p_call_obj,
        SipMessage *pSipMsg,
        SipOptions *p_options,
        Sdf_st_error *p_sdf_error);


icf_return_t icf_ssa_set_to_in_reg_callobj(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

icf_return_t icf_ssa_send_register_req(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

icf_return_t icf_ssa_send_register_req_with_auth(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_notify
 ** DESCRIPTION:        
 *****************************************************************************/
extern icf_return_t icf_ssa_process_notify(
        INOUT    icf_ssa_pdb_st *p_ssa_pdb);

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_process_clear_reg_obj
 *
 * DESCRIPTION:     This function processes request from REGM sub-module to 
 *                  destroy the registration call object.
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_clear_reg_obj(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);



icf_return_t icf_ssa_check_if_ip(icf_uint8_t *p_ip_address);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_reject_ack
 **
 ** DESCRIPTION:        This function is used to send the acknowledgement
 **                     (ACK)for 3xx/4xx/5xx/6xx messages to peer.
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_reject_ack(
        icf_ssa_pdb_st *p_ssa_pdb);

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_process_forceful_call_clear
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_process_forceful_call_clear(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_del_all_other_call_obj
 ** DESCRIPTION:  This function finds all other call object except for one which is 
 **                passed as paramter. And then forcefully deletes all of them
 ******************************************************************************/
  icf_return_t    icf_ssa_del_all_other_call_obj(
                 INOUT  icf_ssa_pdb_st      *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_status_frm_notify
 * DESCRIPTION:     This function checks the given SipMessage for Fragmented
 *                  Sip body and then checks it for the response code
 *                  It returns TRUE if a 200 OK is found, FALSE if any non 2xx
 *                  is found or no message body is found at all
 ******************************************************************************/
extern icf_return_t icf_ssa_get_status_frm_notify(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      SipMessage              *p_sip_message,
         OUT     icf_boolean_t        *p_status,
        OUT     icf_boolean_t        *p_indicate_service);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_notify_resp
 ** DESCRIPTION:       Currently this is used for sending Notify in 
 **     context of Refer procedures only 
 *****************************************************************************/
extern icf_return_t icf_ssa_process_notify_resp(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_and_populate_self_ip
 * DESCRIPTION:     This function calls UATK callbacks, implemented in SSA,
 *                  to get the system host name, then system ip address
 *                  and then sets the resolved ip address in config data
 ******************************************************************************/
extern icf_return_t icf_ssa_get_and_populate_self_ip(
        INOUT  icf_ssa_pdb_st             *p_ssa_pdb,
        INOUT  icf_transport_address_st *p_self_address,
        INOUT  icf_uint8_t *p_self_ip_str);

/*******************************************************************************
 * FUNCTION:        icf_ssa_abort_overlap_txn
 * DESCRIPTION:     This function aborts the overlap transaction in the call 
 * obj extracted from the PDB after matching it with the Method string passed.
 * Currently we assume that there will be only one overlap transaction at at time
 * so we are aborting the first transaction (at index 0).Later on we can 
 * have a link list traversal and comparison of the method type to get the 
 * excat overlap transino
 ******************************************************************************/
extern icf_return_t icf_ssa_abort_overlap_txn(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb, 
        INOUT      icf_uint8_t           *p_method);

/*******************************************************************************
 * FUNCTION:        icf_ssa_process_refer_abort_req
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_process_refer_abort_req(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

/**********************************************************************
 * FUNCTION:        icf_ssa_get_spec_unprocessed_hdr_list
 * DESCRIPTION:     This routine traverses the given instance of
 * list of Sdf_st_headerList  structures and returns the one instance 
 * where it finds the given header type
 **********************************************************************/
extern icf_return_t icf_ssa_populate_div_hdr(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT 	SipMessage		*p_sip_msg);

/**********************************************************************
 * FUNCTION:        icf_ssa_get_div_count
 * DESCRIPTION:     This routine traverses the Route header structure 
 * slParam and looks for diversion counter. If it is not there
 * it set the default value from configuration into it
 **********************************************************************/
extern icf_return_t icf_ssa_get_div_count(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      SipRouteHeader          *p_route_hdr,
        OUT     icf_diversion_info_st  *p_div_info);

/*******************************************************************************
 * FUNCTION:        icf_ssa_form_and_insert_div_hdr
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_form_and_insert_div_hdr(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipMessage      *p_otg_sip_msg);

extern icf_return_t icf_ssa_get_hostname_str_frm_trans_addr(
        icf_ssa_pdb_st       *p_ssa_pdb,
        icf_transport_address_st     *p_transp_addr,
        icf_uint8_t          *p_hostname);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_abort_resp
 **
 ** DESCRIPTION:        This function sends a 487 ReqTrerminated message
 **                     to peer. THis is triggered by CC 
 *****************************************************************************/
extern icf_return_t icf_ssa_process_abort_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_populate_address_from_sip_addr_spec
 ** DESCRIPTION:  This function populates the icf_address_st structure
 **               from the SipAddrSpec structure passed to it
 **               
 **
 ***************************************************************************/

extern icf_return_t icf_ssa_populate_address_from_sip_addr_spec(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT SipAddrSpec            *p_sip_addr_spec,
        INOUT  icf_address_st   *p_addr);
/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_form_and_insert_ref_by_hdr
 ** DESCRIPTION:  This function forms and inserts a referred by header 
 **               in the SipMsg passed to it
 **
 ***************************************************************************/

extern icf_return_t icf_ssa_form_and_insert_ref_by_hdr(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  SipMessage          *p_sip_msg);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_form_and_insert_replaces_hdr
 ** DESCRIPTION:  This function forms and inserts a referred by header 
 **               in the SipMsg passed to it
 **
 ***************************************************************************/

extern icf_return_t icf_ssa_form_and_insert_replaces_hdr(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  SipMessage          *p_sip_msg);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_form_and_send_response
 ** DESCRIPTION:  This function forms and sends a response to peer
 **
 ***************************************************************************/

extern icf_return_t icf_ssa_form_and_send_response(
        INOUT  icf_ssa_pdb_st         *p_ssa_pdb,
        INOUT    icf_uint16_t           resp_code,
        INOUT  Sdf_st_callObject         *p_call_obj,
        INOUT  Sdf_st_overlapTransInfo   *p_overlap_txn,
        INOUT    Sdf_ty_s8bit              *p_method,
        INOUT    Sdf_ty_bool               rel_resp);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_parse_p_asserted_identity_header
 ** DESCRIPTION:  This function iterates through all the unknown headers
 **               If it doesn't find p-asserted identity header,it returns
 **               success.In this case SSA will parse the From header for
 **               the ascertaining the calling party address.
 **               Otherwise,if the p-asserted identity hdr is present,SSA
 **               will parse it like an unknown header
 **
 ***************************************************************************/

icf_return_t icf_ssa_parse_p_asserted_identity_header(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_callObject   *p_call_obj,
        INOUT  SipHeader           **p_p_assert_hdr);

/*****************************************************************************
 ** FUNCTION:  	icf_ssa_formStatusLineMessage
 ** 
 ** DESCRIPTION:This function forms SipResponseMessage only with StatusLine
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_formStatusLineMessage(
        INOUT  SipMessage	**p_p_sip_msg,
        INOUT Sdf_ty_u16bit	resp_code,
        INOUT  Sdf_st_error	*pError);

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_validate_via
 ** DESCRIPTION:  This function validates the via header value in an incoming 
 **               request.
 **
 ******************************************************************************/
Sdf_ty_messageValidity icf_ssa_validate_via(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_callObject   *p_call_obj,
        OUT Sdf_st_error          *pErr);

#ifdef IMS_CLIENT

/*****************************************************************************
 ** FUNCTION:           headerListFreeFunction
 **
 ** DESCRIPTION:        This function free memeory allocated for SIP Header.
 **
 *****************************************************************************/
void headerListFreeFunction(
        INOUT  void *pData);


/***************************************************************************
 ** FUNCTION:           icf_ssa_start_timer
 **
 ** DESCRIPTION:        This function will be used to start
 **                     the timer.
 ***************************************************************************/
icf_return_t icf_ssa_start_timer(
        INOUT  icf_glb_pdb_st *p_glb_pdb,
        INOUT    icf_timer_type_t timer_type,
        INOUT    icf_duration_t   timer_dur,
        INOUT  icf_ssa_ctxt_st  *p_ssa_ctxt,
        INOUT  icf_error_t *p_ecode);

/****************************************************************************
 *
 * DESCRIPTION:        The function would be called to fetch the address of the
 *                     the registration context block for the specified user
 *
 ***************************************************************************/
extern icf_return_t
icf_dbm_fetch_rgn_blk(
        INOUT  icf_glb_pdb_st     *p_glb_pdb,
        IN    icf_address_st     *p_user_addr,
        IN    icf_line_id_t       line_id,
        OUT   icf_rgm_context_st **p_p_rgn_blk_addr,
        OUT   icf_error_t        *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_set_unknown_hdr
 *
 * DESCRIPTION:     This function creates Unknown header.
 *
 ******************************************************************************/
icf_return_t icf_ssa_set_unknown_hdr(
	INOUT	icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      Sdf_ty_s8bit            *pHdr,
        INOUT      Sdf_ty_s8bit            *pVal,
        INOUT    Sdf_st_headerList       **ppHeaderList);

/*****************************************************************************
 ** FUNCTION:		icf_ssa_set_ValInUnknownHdr
 ** 
 ** DESCRIPTION:	This function forms unknown header based on input hdr and value
 *****************************************************************************/
icf_return_t icf_ssa_set_ValInUnknownHdr(
		INOUT		icf_ssa_pdb_st   *p_ssa_pdb,
		INOUT 	SipHeader			*pHeader,
        INOUT      Sdf_ty_s8bit    	*pHdr,
        INOUT      Sdf_ty_s8bit    	*pVal);

/*****************************************************************************
 ** FUNCTION:		icf_ssa_get_reg_body
 ** 
 ** DESCRIPTION:	This function gets the non-SDP/ISUP buffers and returns
 **					"application/reginfo+xml" message body.
 **
 *****************************************************************************/
icf_return_t icf_ssa_get_reg_body (
			INOUT		SipMessage               *pMessage,
 			OUT		Sdf_ty_s8bit			 **pp_Buffer,
			OUT		Sdf_ty_u32bit			 *p_dBufLength,
			INOUT 	icf_error_t           *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_subs_state
 *
 * DESCRIPTION:     This function parses REG info message body and initiate
 *					appropriate action.
 *
 ******************************************************************************/
icf_return_t icf_ssa_handle_subs_state(
			INOUT    icf_ssa_ctxt_st          *p_ssa_ctxt,
			INOUT		Sdf_ty_s8bit			 *pBuffer,
			INOUT		Sdf_ty_u32bit			 *pBufLength,
			INOUT 	icf_error_t           *p_ecode);

#endif
/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_parse_privacy_header
 ** DESCRIPTION:  This function iterates through all the unknown headers
 **               If it doesn't find privacy header,it returns
 **               failure.
 **               Otherwise,if the privacy hdr is present,SSA
 **               will parse it like an unknown header.If privacy header
 **               contains the value "id",then return SUCCESS.
 **               In all other cases failure is returned.In case UATK failure
 **               occurs,ecode is set as ICF_ERROR_UATK_FAILURE
 **
 ***************************************************************************/
icf_return_t icf_ssa_parse_privacy_header(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_callObject   *p_call_obj);

/*******************************************************************************
 * FUNCTION:     icf_ssa_form_and_insert_priv_hdr
 * DESCRIPTION:  This function forms and inserts a privacy header into the SIP
 *               message .
 ******************************************************************************/
icf_return_t icf_ssa_form_and_insert_priv_hdr(
        INOUT      icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT    SipMessage          *p_otg_sip_msg);

/*******************************************************************************
 * FUNCTION:        icf_ssa_dispatch_pdu_to_es
 * DESCRIPTION:    The ppdu memory is allocated to this function
 * by athe calling routine and then it frees it.
 ******************************************************************************/
extern icf_return_t icf_ssa_dispatch_pdu_to_es(
        INOUT    icf_glb_pdb_st   *p_glb_pdb,
        INOUT    icf_ssa_ctxt_st  *p_ssa_ctxt,
        INOUT      SipTranspAddr		*pTranspaddr,
        INOUT      Sdf_ty_protocol	    dTransptype,
        INOUT      icf_uint8_t      *p_pdu,
        INOUT      icf_uint32_t     pdu_len,
	    IN      icf_line_id_t    line_id,
        OUT     icf_error_t      *p_ecode);

/*******************************************************************************
 * FUNCTION:        icf_ssa_search_conn_db_node
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_search_conn_db_node(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT      icf_int32_t      fd,
        OUT     icf_uint32_t     *p_conn_db_index);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_00_inc_conn_ind
 **
 ** DESCRIPTION: Move to State Connected.Store the remote IP/Port. 
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_00_inc_conn_ind(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_04_write_to_conn
 **
 ** DESCRIPTION:  Store the SSA ctxt in the connection DB node.
 **                 Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_04_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_03_write_to_conn
 **
 ** DESCRIPTION:  Store the SSA ctxt in the connection DB node.
 **                 Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_03_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_02_write_to_conn
 **
 ** DESCRIPTION:  Write the message on connection. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_02_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_01_succ_open_conn_resp
 **
 ** DESCRIPTION:  Check if there are any messages in the list.
 **             If so, dispatch all and release the memory.Change state to 
 **             Connected
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_01_succ_open_conn_resp(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_send_dialog_initiator_msg
 * DESCRIPTION:   
 ******************************************************************************/
extern icf_return_t icf_ssa_send_dialog_initiator_msg(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_01_fail_open_conn_resp
 **
 ** DESCRIPTION:  Check if there are any messages in the list.
 **             If so, delete all, Trigger generic failure ti release the call 
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_01_fail_open_conn_resp(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_reset_conn_db_node
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_reset_conn_db_node(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb,
        OUT     icf_uint32_t      conn_db_index);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_01_close_req
 **
 ** DESCRIPTION: Check if any messages are pending in the queue, if so, 
 ** release the memory for them and then invoke ES to close the conn
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_01_close_req(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_01_write_to_conn
 ** DESCRIPTION:  Add the message to queue. SM has been invoked from
 **                 callback SendMsgOnNetwork
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_01_write_to_conn(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_00_connect_req
 **
 ** DESCRIPTION:  Will be invoked from INVITE/REGISTER code legs or from 
 **     Connect call back in UATK
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_00_connect_req(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_03_purge_timer_xpry
 **
 ** DESCRIPTION:  Move to state 4
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_03_purge_timer_xpry(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_04_purge_timer_xpry
 **
 ** DESCRIPTION:  Close the connection, issue close conn to ES, 
 **     reset the node and the socket in CallObject, if any is associated
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_04_purge_timer_xpry(
        icf_ssa_pdb_st   *p_ssa_pdb);

extern icf_return_t icf_ssa_tcpm_ret_success(
        icf_ssa_pdb_st   *p_ssa_pdb);

extern icf_return_t icf_ssa_tcpm_ret_fail(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_init_tcp_sm
 **
 ** DESCRIPTION: This routine does the pointer assignments for TCP SM
 **              Called at init time in SSA INIT
 *****************************************************************************/
extern icf_return_t     icf_ssa_init_tcp_sm
(icf_ssa_pdb_st  *p_ssa_pdb);


/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_01_replace_conn_fd
 **
 ** DESCRIPTION:  Replace the connection in ssa_ctx with connection in
 **               ssa_pdb
 **     
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_01_replace_conn_fd(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_xx_replace_conn_fd
 **
 ** DESCRIPTION:  Replace the connection in ssa_ctx with connection in
 **               ssa_pdb
 **     
 *****************************************************************************/
extern icf_return_t icf_ssa_tcpm_xx_replace_conn_fd(
        icf_ssa_pdb_st   *p_ssa_pdb);



/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_tcpm_04_data_ind
 **
 ** DESCRIPTION:   DataReceived first time on incoming connection
 **                move to INUSE state from NOT_INUSE
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_04_data_ind(
        icf_ssa_pdb_st   *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_add_to_pdu_array
 * DESCRIPTION:    This routine adds the given pdu at the end of array
 ******************************************************************************/
extern icf_return_t icf_ssa_add_to_pdu_array(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb,
        OUT     icf_uint8_t      *p_pdu);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_close_nw_servers
 **
 ** DESCRIPTION:        Sent by CFG To trigger the network servers
 *****************************************************************************/
extern icf_return_t icf_ssa_process_close_nw_servers(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_open_nw_servers
 **
 ** DESCRIPTION:        Sent by CFG To trigger the network servers
 *****************************************************************************/
extern icf_return_t icf_ssa_process_open_nw_servers(
        icf_ssa_pdb_st *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_tcp_event_frm_mrm
 * DESCRIPTION:     
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 ******************************************************************************/
extern icf_return_t icf_ssa_handle_tcp_event_frm_mrm(
        INOUT  icf_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_conn_db_node
 * DESCRIPTION:    
 ******************************************************************************/
extern icf_return_t icf_ssa_get_conn_db_node(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb,
        OUT     icf_uint32_t      *p_conn_db_index);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_purge_timer_expiry_handler
 ** DESCRIPTION:        This function travereses through the conn DB
 **                     database in global data and invokes the SM
 **                     for each valid Conn DB block with PurgeTimer
 **                     Expiry Event
 *****************************************************************************/
icf_return_t icf_ssa_purge_timer_expiry_handler(
        icf_ssa_pdb_st *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_revert_to_udp
 * DESCRIPTION:   This function is used to revert the callobject from TCP
 *          to UDP. Don;t Know if a clear transaction is required prior to the
 *          second the MakeCal ( in this call)
 ******************************************************************************/
extern icf_return_t icf_ssa_revert_to_udp(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb,
        Sdf_st_callObject   *p_call_obj,
        Sdf_st_overlapTransInfo     **p_p_overlap_txn,
        icf_uint8_t      *p_method,
        Sdf_st_error                *p_error);

/*********************************************************************
 * FUNCTION:        icf_ssa_send_failure_to_app_module
 * DESCRIPTION:    This routine is invoked in case there is a failure 
 * either in the handling of a successful open conn resp or there is 
 * a failure response for a pending open connection request.  The 
 * routine forms and sends appr. failure message to the module whose 
 * request resulted in a tcp conn attempt 
 ********************************************************************/
extern icf_return_t icf_ssa_send_failure_to_app_module(
        icf_ssa_pdb_st   *p_ssa_pdb);

extern icf_return_t icf_ssa_start_purge_timer(
        INOUT  icf_ssa_pdb_st*    p_ssa_pdb);


extern icf_return_t   icf_ssa_check_for_new_conn(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT  icf_ssa_ctxt_st *p_ssa_ctxt,
        INOUT  Sdf_st_callObject *p_call_obj);


extern icf_return_t   icf_ssa_copy_conn_fd_to_callobj(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT  Sdf_st_callObject *p_call_obj,
        INOUT    icf_int32_t   conn_fd);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_parse_unknown_header_like_from_hdr
 ** DESCRIPTION:  This function iterates through all the unknown headers
 **               If it doesn't find the required header,it returns
 **               success.
 **               Otherwise,if the required header is present,SSA
 **               will parse it like an FROM header and return it.
 **
 ***************************************************************************/
extern icf_return_t icf_ssa_parse_unknown_header_like_from_hdr(
        INOUT  icf_ssa_pdb_st*,
        INOUT  SipMessage*,
        INOUT    Sdf_ty_s8bit*,
        INOUT  SipHeader**);

/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_dealloc_call_cntxt_blk
 *
 * DESCRIPTION: The function would free the msgbodylist,tags,headers 
 *				in call context and then frees hash mapping for outgoing call 
 *              and adds the block to free blocks list
 *
************************************************************************/  
extern icf_return_t icf_ssa_dealloc_call_cntxt_blk(
        INOUT  icf_glb_pdb_st*,
        INOUT  icf_call_ctx_st*,
        OUT icf_error_t*);

/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_populate_app_specific_hdrs
 *
 * DESCRIPTION: The function populates the application specific headers as  
 *				provided by the application during APP ADD in the call context.  
 *              
 *
************************************************************************/  
extern icf_return_t icf_ssa_populate_app_specific_hdrs(
       INOUT  icf_ssa_pdb_st*,
		INOUT icf_uint8_t,
        INOUT icf_void_t*,
		INOUT SipMessage*,	    
        OUT icf_error_t*);




ICF_SESSION_CONTROL_START


/*****************************************************************************
 * FUNCTION: 		icf_ssa_insert_unknown_hdr_in_sip_msg
 *
 * DESCRIPTION:     This function forms and inserts an unknown 
 *					header into the SIP message passed  to it
 *					The name of the header and the contents are
 *					passed to this function.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_insert_unknown_hdr_in_sip_msg(
         INOUT      icf_ssa_pdb_st*		p_ssa_pdb,
         INOUT      icf_uint8_t*		p_hdr_name,
         INOUT      icf_uint8_t*		p_hdr_contents,
         INOUT    SipMessage*			p_sip_msg);






/*****************************************************************************
 * FUNCTION: 		icf_ssa_insert_header_list_in_sip_msg
 *
 * DESCRIPTION:     	This function inserts the header linked list into a
 *			SIP message passed  to it. The clear_list parameter specifies whether 
 *			the memory associated with this list is to be freed or not
 *			if Set to true the memory is freed.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_insert_header_list_in_sip_msg(
         INOUT      icf_ssa_pdb_st*		 p_ssa_pdb,
         INOUT      icf_header_list_st*  p_hdr_list,
         INOUT      icf_boolean_t        clear_list,
		 INOUT    SipMessage*		     p_sip_msg);




		 
		 
/*****************************************************************************
 * FUNCTION: 		icf_ssa_insert_tag_list_in_sip_msg
 *
 * DESCRIPTION:     	This function inserts the tag linked list into a
 *			SIP message passed  to it. The clear_list parameter specifies whether 
 *			the memory associated with this list is to be freed or not
 *			if Set to true the memory is freed.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_insert_tag_list_in_sip_msg(
		INOUT      icf_ssa_pdb_st*      p_ssa_pdb,
        INOUT      icf_tag_list_st*     p_tag_list,
        INOUT      icf_boolean_t        clear_list,
        INOUT    SipMessage*				p_sip_msg);




/*****************************************************************************
 * FUNCTION:     icf_ssa_get_hdr_type_from_name
 * DESCRIPTION:  This function gets the Sip Header Type based on the header 
 *               string passed to it
 *****************************************************************************/
void icf_ssa_get_hdr_type_from_name
        (icf_uint8_t *pName, en_HeaderType *pType, SipError *pError);


/*****************************************************************************
 * FUNCTION:     icf_append_tag_params_to_hdr
 * DESCRIPTION:  This function adds a new tag passed to it to a known header
 *				value, which is also passed as a parameter
 *****************************************************************************/
icf_return_t icf_append_tag_params_to_hdr(
		INOUT 	icf_ssa_pdb_st*		p_ssa_pdb,
		INOUT		icf_tag_st*			p_tag_st,
		INOUT 	SipMessage*				p_sip_msg);







/*****************************************************************************
 * FUNCTION: 		icf_ssa_insert_body_list_in_sdf_list
 *
 * DESCRIPTION:     	This function inserts the message body linked list into 
 *			a Message body list passed  to it. The clear_list parameter specifies
 *          whether the memory associated with this list is to be freed or not
 *			if Set to true the memory is freed.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_insert_body_list_in_sdf_list(
         INOUT		icf_ssa_pdb_st*			p_ssa_pdb,
         INOUT     icf_msg_body_list_st*	p_body_list,
         INOUT     icf_boolean_t			clear_list,
		 INOUT   Sdf_ty_slist*			    p_list);





/*Message Body Header Changes*/


/*****************************************************************************
 * FUNCTION:     icf_add_body_to_list
 * DESCRIPTION:  This function adds a Message body to an Existing Message body
 *				 list.
 *****************************************************************************/
icf_return_t icf_add_body_to_list(
		INOUT 	icf_ssa_pdb_st*		p_ssa_pdb,
		INOUT	icf_msg_body_st*	p_body,
                INOUT    Sdf_ty_slist*           p_body_list);








/*******************************************************************************
 * FUNCTION:     icf_ssa_form_and_insert_pref_identity_hdr
 * DESCRIPTION:  This function forms and inserts a P-Preferred identity header
 *               into the SIP message .
 ******************************************************************************/
icf_return_t icf_ssa_form_and_insert_pref_identity_hdr(
        INOUT      icf_ssa_pdb_st*	p_ssa_pdb,
        INOUT    SipMessage*			p_otg_sip_msg);






/*******************************************************************************
 * FUNCTION:     icf_ssa_form_and_insert_request_disposition_hdr
 * DESCRIPTION:  This function forms and inserts a Request-Disposition header
 *               into the SIP message .
 ******************************************************************************/

icf_return_t icf_ssa_form_and_insert_request_disposition_hdr(
        INOUT      icf_ssa_pdb_st*	p_ssa_pdb,
        INOUT    SipMessage*			p_otg_sip_msg);



/*****************************************************************************
 * FUNCTION:     icf_ssa_form_and_insert_access_nw_info_hdr
 * DESCRIPTION:  This function forms and inserts a P-Access-Network-Info 
 *               header into the SIP message.
 *****************************************************************************/
icf_return_t icf_ssa_form_and_insert_access_nw_info_hdr(
        INOUT      icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT    SipMessage          *p_otg_sip_msg);






/*****************************************************************************
 * FUNCTION:     icf_insert_header_tag_body_list
 * DESCRIPTION:  This function inserts header/tag/body list collectively
 *               into either a SIP message or SIP transaction.
 *****************************************************************************/
icf_return_t icf_insert_header_tag_body_list(
        INOUT      icf_ssa_pdb_st*          p_ssa_pdb,
        INOUT      icf_boolean_t            clear_list,
        INOUT 	Sdf_st_transaction*		    p_txn,
        INOUT    Sdf_st_overlapTransInfo*    p_overlap_txn);



ICF_SESSION_CONTROL_END




/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_extract_challenge
 *
 * DESCRIPTION: The function extracts the challenge from the auth_params_lis
 *				for the header type sepcified by the calling function.
 *              
 *
 * RETURNS: icf_return_t 
 *
************************************************************************/  
icf_return_t icf_ssa_extract_challenge(
       INOUT  icf_ssa_pdb_st                    *p_ssa_pdb,
       INOUT	 icf_uint8_t						  header_type);


/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_extract_qop
 *
 * DESCRIPTION: The function extracts the QoP from the auth_params_lis
 *				for the header type sepcified by the calling function.
 *              
 *
 * RETURNS: icf_return_t 
 *
************************************************************************/  
icf_return_t icf_ssa_extract_qop(
       INOUT  icf_ssa_pdb_st                    *p_ssa_pdb,
	   INOUT  icf_string_st					  *p_qop,	   
	   INOUT  Sdf_ty_slist						  *p_auth_params_list,
	   INOUT icf_uint8_t						  header_type,
	   OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_compute_nc
 *
 * DESCRIPTION: The function computes the nc string from nonce_count
 *              
 *
 * RETURNS: icf_return_t 
 *
************************************************************************/  
icf_return_t icf_ssa_compute_nc(
	   INOUT  icf_glb_pdb_st					  *p_glb_pdb,
       INOUT  icf_ssa_ctxt_st                   *p_ssa_ctxt,
	   INOUT  icf_string_st					  *p_nc,
	   INOUT  icf_uint32_t					  nonce_count,
	   OUT   icf_error_t                       *p_ecode);


ICF_SIC_START
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_send_subscribe_req
 *
 * DESCRIPTION:     This function constructs and sends Subscribe req on network.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_send_subscribe_req(
     INOUT   icf_ssa_pdb_st   *p_ssa_pdb);

/****************************************************************************
 * FUNCTION:           icf_ssa_init_ssa_context
 *
 * DESCRIPTION:        This routine initializes SSA Context
 * 
 *****************************************************************************/
extern icf_void_t   icf_ssa_init_ssa_context(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT   icf_ssa_ctxt_st  *p_ssa_ctxt);

/****************************************************************************
 * FUNCTION:           icf_ssa_form_and_insert_unknown_hdr
 *
 * DESCRIPTION:        This routine form an unknown header and inserts it in 
 *                     the given SIP message
 *
 *****************************************************************************/
extern icf_return_t icf_ssa_form_and_insert_unknown_hdr(
     icf_ssa_pdb_st   *p_ssa_pdb,
     icf_uint8_t      *p_hdr_name,
     icf_uint8_t      *p_hdr_val,
     SipMessage          *pSipMsg);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_subs_req
 *
 * DESCRIPTION:     This function processes outgoing subscribe request
 *        1. Fetch subm context
 *        2. Mark in ssa_pdb that access user address from app ctx
 *        3. Check if it is initial subscribe request
 *        4.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_process_otg_subs_req(
        INOUT  icf_ssa_pdb_st  *p_ssa_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_msg_req
 *
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_process_otg_msg_req(
        INOUT  icf_ssa_pdb_st  *p_ssa_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_publish_req
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_process_otg_publish_req(
        INOUT  icf_ssa_pdb_st  *p_ssa_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_options_req
 *
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_process_otg_options_req(
        INOUT  icf_ssa_pdb_st  *p_ssa_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_otg_refer_req
 *
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_process_otg_refer_req(
        INOUT  icf_ssa_pdb_st  *p_ssa_pdb);

/*Message body header changes Start*/

extern icf_return_t	icf_ssa_get_msg_body_frm_sip_message(
		INOUT 	icf_ssa_pdb_st	     *p_ssa_pdb,
		INOUT	SipMessage	     *p_sip_msg,
		/*INOUT      Sdf_st_callObject    *p_call_object,*/

		en_SipMessageType dReqRespType,
		OUT	icf_msg_body_list_st *p_body_list
		);

/*Message body header changes Ends*/

ICF_SIC_END





/*******************************************************************************
 * FUNCTION:        icf_ssa_add_ContactHeaderToSipMsg
 * DESCRIPTION:     This function adds a List of contacts to the SIP message
 ******************************************************************************/

icf_return_t icf_ssa_add_ContactHeaderToSipMsg
	(Sdf_st_callObject *pObject,
	 Sdf_ty_slist *pslContactList,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr);
/****************************************************************************
 * FUNCTION:           icf_ssa_AddParamToContactHdrList
 *
 * DESCRIPTION:        This routine fetch contact headers from InitData
 *                     Adds comp=sigcomp paramater in contact header
 * 
 *****************************************************************************/
icf_return_t   icf_ssa_AddParamToContactHdrList(
        Sdf_ty_slist             *pContactHdrList,
        icf_uint8_t           *p_param,
        Sdf_st_error              *pError);


/****************************************************************************
 * FUNCTION:           icf_ssa_add_transport_param_in_contact_hdr
 *
 * DESCRIPTION:        This routine fetch contact headers from InitData
 *                     Check if transport is TCP or UDP then insert 
 *                     transport=tcp/udp paramater in contact header
 * 
 *****************************************************************************/
extern icf_return_t   icf_ssa_add_transport_param_in_contact_hdr(
        icf_ssa_pdb_st        *p_ssa_pdb,
        Sdf_st_initData          *p_init_data,
        Sdf_st_error             *pError);

/*******************************************************************************
 * FUNCTION:        icf_ssa_insert_param_in_addr_spec
 *
 * DESCRIPTION:     This function inserts parametrs into the addr spec passed
 *                  to it as a paramter
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_insert_param_in_addr_spec(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipAddrSpec    *p_addr_spec,
        icf_uint8_t *p_param,
        icf_uint8_t *p_value,
        Sdf_st_error   *p_sdf_error);
/*******************************************************************************
 * FUNCTION:        icf_ssa_append_par_to_sip_list
 *
 * DESCRIPTION:     This function appends a parameter to given SIP list
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_append_par_to_sip_list(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipList                *pDest,
        icf_uint8_t         *p_param_name,
        icf_uint8_t         *p_paramValue,
        Sdf_st_error           *pErr);


/*******************************************************************************
 * FUNCTION:        icf_ssa_fetch_matching_overlap_txn
 *
 * DESCRIPTION:     This function fetches the matching overlap transaction
 *                  corresponding to the msg type and transaction state
 *                  state from the list of overlapped transactions in the
 *                  call object
 *
 ******************************************************************************/
icf_return_t icf_ssa_fetch_matching_overlap_txn(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      Sdf_ty_messageType       msg_type,
        INOUT      Sdf_ty_state             trans_state,
        INOUT      Sdf_st_callObject        *p_call_obj,
        INOUT    Sdf_st_overlapTransInfo  **p_p_match_overlap_txn);



/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_send_message_req
 *
 * DESCRIPTION:     This function constructs and sends MESSAGE req on network.
 *
 ******************************************************************************/
icf_return_t icf_ssa_send_message_req(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_send_publish_req
 *
 * DESCRIPTION:     This function constructs and sends PUBLISH req on network.
 *
 ******************************************************************************/
icf_return_t icf_ssa_send_publish_req(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_send_refer_req
 *
 * DESCRIPTION:     This function constructs and sends PUBLISH req on network.
 *
 ******************************************************************************/
icf_return_t icf_ssa_send_refer_req(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_send_options_req
 *
 * DESCRIPTION:     This function constructs and sends OPTIONS req on network.
 *
 ******************************************************************************/
icf_return_t icf_ssa_send_options_req(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_message_resp
 
 *****************************************************************************/
icf_return_t icf_ssa_process_message_resp(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_delink_req
 
 *****************************************************************************/
icf_return_t   icf_ssa_process_delink_req(
        INOUT   icf_ssa_pdb_st    *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_notify_for_refer
 *
 * DESCRIPTION:     This function parses NOTIFY req received for REFER
 *
 ******************************************************************************/
icf_return_t   icf_ssa_handle_notify_for_refer(
        icf_ssa_pdb_st              *p_ssa_pdb,
        icf_nw_inc_notify_ind_st    *p_notify_ind,
        Sdf_st_overlapTransInfo        *pOverlapTransInfo,
        SipMessage                     *pSipMsg);




/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_notify_for_subs
 *
 * DESCRIPTION:     This function parses NOTIFY req received for SUBSCRIPTION
 *
 ******************************************************************************/
icf_return_t   icf_ssa_handle_notify_for_subs(
        icf_ssa_pdb_st              *p_ssa_pdb,
        icf_nw_inc_notify_ind_st    *p_notify_ind,
        Sdf_st_overlapTransInfo        *pOverlapTransInfo,
        SipMessage                     *pSipMsg);







/******************************************************************************
 ** FUNCTION: 		icf_ssa_find_sigsa_ctxt_from_sip_msg
 **
 ** DESCRIPTION: 	This function coding the decodes the 
 **                 SIP message and if decode is successfull then
 **                 it fetches the callObject from the map by passing the
 **                 call id,If the CallObject exists then it fetches the actual
 **                 SSA context and sends the SIGSA context stored in it. 
 **                 if the SSA context does not exist then 
 **					
 ******************************************************************************/
icf_return_t icf_ssa_find_sigsa_ctxt_from_sip_msg(
                              OUT           icf_void_t   **p_p_sigsa_ctxt,
                              INOUT            icf_uint8_t   *pDecompMsg,
                              INOUT            icf_uint32_t  dDecompMsgLen,
                              INOUT            icf_void_t    *p_AppData,
                              OUT           icf_error_t   *p_ecode);



#ifdef ICF_SIGCOMP_SUPPORT
/****************************************************************************
 * FUNCTION:           icf_ssa_send_create_comp_req_to_sigsa
 *
 * DESCRIPTION:        This routine Sends CREATE_COMP_REQUEST TO
 *                     SIGSA for Creating the compartment for sending
 *                     Compressed Message
 *****************************************************************************/
icf_return_t icf_ssa_send_create_comp_req_to_sigsa(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);

/****************************************************************************
 * FUNCTION:           icf_ssa_send_delete_comp_ind_to_sigsa
 *
 * DESCRIPTION:        This routine Sends ICF_DELETE_COMP_IND TO
 *                     SIGSA for deleting the compartment 
 *****************************************************************************/
icf_return_t icf_ssa_send_delete_comp_ind_to_sigsa(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb);


/* External Utility Function dependant upon SIGSA */
extern 
icf_return_t icf_sigsa_decr_ref_count_to_sigsa_ctxt(
                                INOUT  icf_void_t *p_sigsa_ctxt);
icf_return_t icf_sigsa_incr_ref_count_to_sigsa_ctxt(
                                INOUT  icf_void_t *p_sigsa_ctxt);
#endif







/*******************************************************************************
 * FUNCTION:        icf_ssa_set_RouteHeaderListToInitData
 *
 * DESCRIPTION:     This function makes the Route header from the
 *                  Service Route if presnent in the RGN block
 *                  Else set the default Outbound Proxt as
 *                  Preloaded Route,Currently It Does not handle
 *                  Record Route
 ******************************************************************************/
icf_return_t icf_ssa_set_RouteHeaderListToInitData(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      icf_address_st        *p_local_address,
        INOUT      icf_line_id_t        line_id,
        INOUT    icf_error_t*         p_ecode);



/*****************************************************************************
 * FUNCTION:        icf_ssa_flush_alive_connections
 *
 * DESCRIPTION:     This function flushes all alive connections corresponding
 *                  to a call on TCP.This scenario occurs if we have 2 
 *                  TCP connections for the same dialog
 *
 *****************************************************************************/
icf_return_t icf_ssa_flush_alive_connections(
        icf_ssa_pdb_st        *p_ssa_pdb);



/*******************************************************************************
 * FUNCTION:        icf_print_pdu
 * DESCRIPTION:     This function is create an external message to send network 
 *                  PDUs to application with API-ICF_TRACE_NW_BUFF_IND
 ******************************************************************************/
#ifdef VOIP_SYSLOG
icf_return_t icf_print_pdu(icf_glb_pdb_st *p_glb_pdb,
                               icf_uint8_t* p_buf, 
                               icf_uint8_t* p_host, 
                               icf_uint32_t port, 
                               icf_uint32_t len,
				icf_boolean_t direction
			);
#else
icf_return_t icf_print_pdu(icf_glb_pdb_st *p_glb_pdb,
                               icf_uint8_t* p_buf, 
                               icf_uint8_t* p_host, 
                               icf_uint32_t port, 
                               icf_uint32_t len);
#endif
/*******************************************************************************
 * FUNCTION:        icf_ssa_match_string
 *
 * DESCRIPTION:     This function matches two strings based on the match_type  
 *                  given. Returns ICF_SUCCESS if the strings satisfy the 
 *                  match criteria, otherwise ICF_FAILURE is returned.
 *
 ******************************************************************************/

icf_return_t icf_ssa_match_string(
										INOUT icf_uint8_t *p_target, 
										INOUT icf_uint8_t *p_match_str, 
										INOUT icf_match_type_t match_type,
                                       OUT icf_boolean_t *p_match_success);
/*******************************************************************************
 * FUNCTION:        icf_ssa_match_header_filter
 *
 * DESCRIPTION:     This function matches a SIP message against filters  
 *                  on Header Name and Header Content
 *
 ******************************************************************************/
icf_return_t icf_ssa_match_header_filter(
										   INOUT  SipMessage			*p_sip_msg, /* Sip message */
										   INOUT  icf_int_filter_st	*p_int_filter, /* Filter to be matched */
										   OUT icf_boolean_t		*p_match,
										   OUT icf_error_t		*p_ecode);


/*******************************************************************************
 * FUNCTION:        icf_ssa_match_req_line_filter
 *
 * DESCRIPTION:     This function matches a SIP message against filters  
 *                  on Request Line 
 *
 ******************************************************************************/

icf_return_t icf_ssa_match_req_line_filter(
											  INOUT  SipMessage			*p_sip_msg,
											  INOUT  icf_int_filter_st	*p_int_filter,
											  OUT icf_boolean_t		*p_match,
											  OUT icf_error_t		*p_ecode);


/*******************************************************************************
 * FUNCTION:        icf_ssa_map_req_to_app_filter
 *
 * DESCRIPTION:     This function matches a SIP message against the application  
 *                  filter criteria to find out which application is the correct
 *                  one for handling the message. 
 *                  * The app id of first matching filter criteria is filled in 
 *                    p_app_id. 
 *                  * If none of the filters match then the default
 *                    application id is filled in p_app_id. 
 *                  * If none of the filters match and no default application is 
 *                    configured, then ICF_INVALID_APP_ID is filled in p_app_id.
 *
 ******************************************************************************/
icf_return_t icf_ssa_map_req_to_app_filter (INOUT  icf_ssa_pdb_st	*p_ssa_pdb,
												  INOUT  SipMessage 	    *p_sip_msg,
												  OUT icf_app_id_t   *p_app_id,
												  OUT icf_error_t	*p_ecode);

#ifdef ICF_QOS_SUPPORT
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_extract_media_authorization_token
 *
 * DESCRIPTION: The function extracts P-Media-Authorization header from
 *				sip message and fills its value in call context.
 *              
 *
 * RETURNS: icf_return_t 
 *
************************************************************************/  
icf_return_t 
icf_ssa_extract_media_authorization_token(
	   INOUT  icf_ssa_pdb_st          *p_ssa_pdb,
	   INOUT   SipMessage					*pSipMsg);
#endif
#endif

/*****************************************************************************
 ** FUNCTION:        icf_ssa_prepare_qos_status_lines_from_sdp
 ** 
 ** DESCRIPTION:    This function forms Current, Desired and Confirm status
 **                 lines for Qos Support from sdp structure.
 **
 *****************************************************************************/
icf_return_t icf_ssa_prepare_qos_status_lines_from_sdp(
        INOUT    icf_internal_sdp_stream_st *p_sdp,
        INOUT  Sdf_ty_s8bit  pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE]);


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_init_remote_sdp_with_default_qos_values
 *
 * DESCRIPTION:     This function would initialize the Qos parameters present
 *                  in remote SDP structure with default Qos preconditions.
 *
 ******************************************************************************/
icf_return_t icf_ssa_init_remote_sdp_with_default_qos_values(
        INOUT  icf_internal_sdp_stream_st *p_remote_received_sdp);


/*****************************************************************************
 ** FUNCTION:        icf_ssa_prepare_sdp_from_qos_status_lines
 ** 
 ** DESCRIPTION:    This function prepares SDP structure from Current, Desired
 **                     and Confirm status lines for Qos Support.
 **
 *****************************************************************************/
icf_return_t icf_ssa_prepare_sdp_from_qos_status_lines(
   		INOUT Sdf_ty_s8bit  pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE],
		INOUT  icf_internal_sdp_stream_st *p_sdp,
   		INOUT icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:        icf_ssa_validate_qos_status_lines
 ** 
 ** DESCRIPTION:    .This function validates that all the mandatory qos
 **                 lines have been recieved in SDP
 **
 *****************************************************************************/
icf_return_t icf_ssa_validate_qos_status_lines
(
        INOUT icf_ssa_pdb_st    *p_ssa_pdb,
        INOUT Sdf_ty_s8bit      pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE]
);

#endif
ICF_4_0_CRM_START
/*******************************************************************************
 * FUNCTION:        icf_ssa_make_ims_auth_hdr
 *
 * DESCRIPTION:     This function makes the authorization header for
 *                  IMS scenario
 *
 ******************************************************************************/
 icf_return_t icf_ssa_make_ims_auth_hdr(
			INOUT    icf_ssa_pdb_st			*p_ssa_pdb,
			INOUT    SipMessage					*p_sip_msg);

/*******************************************************************************
 * FUNCTION:        icf_ssa_add_rgm_app_data
 *
 * DESCRIPTION:     This function adds the app related data: hdr/ tag/ msg list
 *                  to the overlap transaction: In case of REGISTER message
 *
 ******************************************************************************/
 icf_return_t		icf_ssa_add_rgm_app_data(
	 INOUT 	icf_ssa_pdb_st					*p_ssa_pdb,
	 INOUT 	Sdf_st_overlapTransInfo             *p_overlap_trans_info);

 /*******************************************************************************
 * FUNCTION:        icf_ssa_fill_uac_auth
 *
 * DESCRIPTION:     This function fills the UAC auth info member using
 *                  auth_sent member of rgm_context
 *
 ******************************************************************************/
 icf_return_t icf_ssa_fill_uac_auth(
			INOUT    icf_ssa_pdb_st				*p_ssa_pdb,
			INOUT 	Sdf_st_uacAuthInfo              *p_uac_auth_info);

 /*******************************************************************************
 * FUNCTION:        icf_ssa_chk_n_append_auts
 *
 * DESCRIPTION:     This function appends the AUTS parameter to Authorization header 
 *					if it is present in the IMS scenario.
 *
 ******************************************************************************/
icf_return_t icf_ssa_chk_n_append_auts(
			INOUT    icf_ssa_pdb_st					*p_ssa_pdb,
			INOUT 	Sdf_st_overlapTransInfo             *p_overlap_trans_info);

/*******************************************************************************
 * FUNCTION:        icf_ssa_rgm_remove_tags
 *
 * DESCRIPTION:     This function removes the parameters from the mandatory headers
 *                  To/ From/  for REGISTER message. Only the param with
 *					name "tag" is left as such.
 *					
 ******************************************************************************/
 icf_return_t        icf_ssa_rgm_remove_tags(
     INOUT     icf_ssa_pdb_st                    *p_ssa_pdb,
	 INOUT     SipMessage						   *p_sip_msg);


/*******************************************************************************
 * FUNCTION:        icf_ssa_clear_tags_for_hdr
 *
 * DESCRIPTION:     This function removes the parameters from the header type
 *                  passed in the parameter list.
 *					
 ******************************************************************************/
icf_return_t        icf_ssa_clear_tags_for_hdr(
     INOUT     icf_ssa_pdb_st                    *p_ssa_pdb,
	 INOUT     SipMessage						   *p_sip_msg,
	 INOUT		  icf_uint32_t						hdr_type);


ICF_4_0_CRM_END
/*****************************************************************************
 * FUNCTION:        icf_ssa_check_for_trans_param
 *
 * DESCRIPTION:     Function to derive transport mode based on proxy mode
 *                  and mode received in remote party address in create call
 * This function checks for combinations of trnasport param given in req UR
 * peer mode ( proxy/registrar) as configured with ICF to determine if
 * revert is possible and to store the decided transport in ssa ctxt
 * transport_mode - this should be given by calling fn, based on  whether it 
 * is registrar or for proxy
 * p_param_val - is the pointer to transport string given by App for the
 * request in processing as part of the request URI
 *****************************************************************************/
icf_return_t icf_ssa_check_for_trans_param(
        icf_ssa_pdb_st        *p_ssa_pdb,
        icf_transport_mode_et transport_mode,
        icf_uint8_t           *p_param_val,
        icf_boolean_t         is_outbound_defined);

/* function added for TCP support in ICF rel 4.0*/

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_method_name
 *
 * DESCRIPTION:     This utility function will be invoked to populate the method
 * 					name in the string function argument. The function will 
 * 					check the bitmaks and accordingly get the function name.
 * 					The function is invoked from within the function 
 * 					icf_ssa_send_dialog_initiator_msg, when attempting to 
 * 					revert to UDP if the connection on TCP has failed.
 *
 ******************************************************************************/
icf_return_t	icf_ssa_get_method_name(
		INOUT   icf_uint32_t  bitmask,
		INOUT   icf_uint8_t	*pMethod
);


#ifdef ICF_IPSEC_ENABLED
/*******************************************************************************
 * FUNCTION:        icf_ssa_get_sec_client_string
 *
 * DESCRIPTION:     This function will create the string (from the security 
 *					param structure) that will be inserted in the Security 
 *					Client Header in the outgoing Register Request
 *
 ******************************************************************************/
icf_return_t	icf_ssa_get_sec_client_string (
		INOUT 		icf_ssa_pdb_st					*p_ssa_pdb,
		INOUT 		icf_uint8_t						**p_str,
		INOUT			icf_ipsec_security_params_st		*p_sec_params
		);

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_sec_string
 *
 * DESCRIPTION:     This function will create the string (from the security 
 *					param structure) that will be inserted in the Security 
 *					Client/Verfiy Header in the outgoing Protected messages  
 *
 ******************************************************************************/
icf_void_t	icf_ssa_get_sec_string(
		INOUT 	icf_uint8_t						*pString,
		INOUT 	icf_ipsec_security_params_st		*p_sec_params
);

/*******************************************************************************
 * FUNCTION:        icf_ssa_get_sec_verify_string
 *
 * DESCRIPTION:     This function will create the string (from the security 
 *					param structure) that will be inserted in the Security 
 *					Verify Header in the outgoing Protected Messages  
 *
 ******************************************************************************/
icf_return_t	icf_ssa_get_sec_verify_string (
		INOUT 		icf_ssa_pdb_st			*p_ssa_pdb,
		INOUT 		icf_uint8_t				**p_str,
		INOUT 		icf_ipsec_sa_context_st	*p_sec_context
		);

/*******************************************************************************
 * FUNCTION:        icf_ssa_fetch_sa_context
 *
 * DESCRIPTION:     This function will fetch a SA context for an incoming 
 *					message from the network. The fetched SA context and the 
 *					corresponding REGM context are save in the SSA PDB for 
 *					future use in SSA processing of the message. 
 *
 ******************************************************************************/
icf_return_t	icf_ssa_fetch_sa_context(
		INOUT   icf_ssa_pdb_st  *p_ssa_pdb,
		INOUT   icf_nw_interface_info_st	*p_nw_info
);

/*******************************************************************************
 * FUNCTION:        icf_ssa_set_sa_ctxt_for_otg_req
 *
 * DESCRIPTION:     Each new transaction will be sent on the network using the 
 *					new SA if it is active by then. Incase the new SA is not 
 *					active by then, the new transaction will be sent out on the 
 *					old SA 
 *					The function would be invoked each time a non-Register 
 *					request is being sent out from ICF. For a register request 
 *					the RGM context would be fetched from the SSA context 
 *					itself.
 *					The extracted SA context and the corresponding REG context
 *					will be saved in the SSA PDB for future reference.
 *
 ******************************************************************************/
icf_return_t	icf_ssa_set_sa_ctxt_for_otg_req(
		INOUT   icf_ssa_pdb_st  *p_ssa_pdb,
		INOUT   icf_address_st  *p_address
);

/*******************************************************************************
 * FUNCTION:        icf_ssa_rgm_delete_old_sa_ind
 *
 * DESCRIPTION:     This utility function will be invoked from within all 
 *					UACCallbacks that handle responses from the network, like 
 *					sdf_ivk_txnComplete. The function will check the count of 
 *					the active transactions on a give SA. If the count is 0, 
 *					SSA will send an indication to REGM to delete the SA 
 *					
 *					This indication will be sent to REGM incase of transaction 
 *					completion of non-REGISTER transactions. For REGISTER 
 *					transactions, the REGM internally takes care to delete the 
 *					old SA when there no pending transactions are active on it.
 *					
 *					This indication is also sent to RGM after the 
 *					send_call_to_peer() is successful for an outgoing response 
 *					to the received non-Invite transaction or 200 OK for the 
 *					Invite transaction that was originally initiated by ICF. 
 *
 ******************************************************************************/
icf_return_t	icf_ssa_rgm_delete_old_sa_ind(
		INOUT   icf_ssa_pdb_st  *p_ssa_pdb		
);


/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_parse_sec_serv_header
 ** DESCRIPTION:  This function iterates through all the unknown headers
 **               If it doesn't find privacy header,it returns
 **               failure.
 **               Otherwise,if the privacy hdr is present,SSA
 **               will parse it like an unknown header.If privacy header
 **               contains the value "id",then return SUCCESS.
 **               In all other cases failure is returned.In case UATK failure
 **               occurs,ecode is set as ICF_ERROR_UATK_FAILURE
 **
 ***************************************************************************/
icf_return_t icf_ssa_parse_sec_serv_header(
        INOUT  icf_ssa_pdb_st           *p_ssa_pdb,
        INOUT  icf_ipsec_sa_context_st  *p_sa_context,
        INOUT  Sdf_st_callObject   	*p_call_obj,
		INOUT  SipHeader           	dHeader);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_sec_value
 ** DESCRIPTION:  
 **
 ***************************************************************************/
icf_void_t icf_ssa_get_sec_value(
	INOUT 	SIP_S8bit		*pBuffer,
	INOUT 	icf_int8_t	**p_pToken);

/*****************************************************************************
 ** FUNCTION: 	 icf_ssa_sa_set_curr_act_serv
 **
 ** DESCRIPTION: This function is invoked to open a new UDP server for the 
 **				 protected port once the SA has been established
 *****************************************************************************/
icf_return_t icf_ssa_sa_set_curr_act_serv(
		INOUT 	icf_ssa_pdb_st	    *p_ssa_pdb,
		INOUT 	icf_rgm_context_st	*p_rgm_ctxt,
		INOUT 	icf_glb_pdb_st	*p_glb_pdb ); 

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_sa_conn_db
 ** DESCRIPTION: 
 **
 **************************************************************************/
icf_return_t	icf_ssa_get_sa_conn_db(
	INOUT 	icf_ssa_glb_data_st	*p_ssa_glb_data,
	INOUT 	icf_uint16_t			port_num,
	INOUT 	icf_uint8_t			*pIndex);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_sa_tcp_conn_db
 ** DESCRIPTION: 
 **
 **************************************************************************/
icf_return_t	icf_ssa_get_sa_tcp_conn_db(
	INOUT 	icf_ssa_glb_data_st	*p_ssa_glb_data,
	INOUT 	icf_uint16_t			port_num,
	INOUT 	icf_uint8_t			*pIndex);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_sa_conn_db_node_index
 ** DESCRIPTION:  
 **
 ***************************************************************************/
icf_return_t icf_ssa_get_sa_conn_db_node_index (
	INOUT 	icf_ssa_pdb_st	*p_ssa_pdb,
	INOUT 	icf_uint8_t	    *pIndex );

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_sa_tcp_conn_db_node_index
 ** DESCRIPTION:  
 **
 ***************************************************************************/
icf_return_t icf_ssa_get_sa_tcp_conn_db_node_index (
	INOUT 	icf_ssa_pdb_st	*p_ssa_pdb,
	INOUT 	icf_uint8_t	    *pIndex );


/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_checkNdeleteRouteHdr
 ** DESCRIPTION:  The function is invoked to delete the existing Route Set 
 **               from the Message, Persistent Route Set and Pre-Loaded
 **               Route Set and then create a new Route Set for sending
 **               the Register on the SA and protected ports.
 **
 ***************************************************************************/
icf_result_t   icf_ssa_checkNdeleteRouteHdr(
          Sdf_st_callObject           *pCallObj);


icf_return_t icf_ssa_sa_open_udp_channel(
		INOUT 	    icf_uint16_t	        port_num,
		INOUT 	    icf_transport_mode_t	trans_mode,
        INOUT       icf_uint8_t          *p_transp_addr_str,
        INOUT    icf_uint32_t         *pFd_id,
		INOUT 	icf_error_t	        *p_ecode);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_sa_set_ssa_ctxt_node
 ** DESCRIPTION:  The function will set the SSA context to the TCP conn DB
 **               when the same connection is shared by more than one SSA 
 **               contexts 
 **
 ***************************************************************************/
icf_void_t   icf_ssa_sa_set_ssa_ctxt_node(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb );
        
/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_open_sa_server
 ** DESCRIPTION:  The function will be invoked on request from REGM (after SA
 **               creation the first time). Here we will open both TCP and
 **               UDP servers on the protected ports
 **
 ***************************************************************************/
icf_return_t icf_ssa_open_sa_server(
    INOUT    icf_ssa_pdb_st   *p_ssa_pdb);

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_close_sa_server
 ** DESCRIPTION:  The function will be invoked on request from REGM (after SA
 **               deletions). Here we will close the servers (both TCP and
 **               UDP on the protected ports).
 **
 ***************************************************************************/
icf_return_t icf_ssa_close_sa_server(
    INOUT    icf_ssa_pdb_st   *p_ssa_pdb);


#endif /*ICF_IPSEC_ENABLED */

/*******************************************************************************
 * FUNCTION:        icf_ssa_make_contact_list_for_reg
 *
 * DESCRIPTION:     This function makes contact list for Register request
 *                  
 ******************************************************************************/

icf_return_t icf_ssa_make_contact_list_for_reg(
	 INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
     INOUT    Sdf_ty_slist 		     *p_contact_addr_list);

/*******************************************************************************
 * FUNCTION:        icf_ssa_reset_fd_in_uaCallObj
 *
 * DESCRIPTION:     This function will invoke sdf_ivk_uaResetSockFd in a loop so 
 *					that all the fd's are reset in the CallObj. 
 *
 ******************************************************************************/
void icf_ssa_reset_fd_in_uaCallObj(
	INOUT 	Sdf_st_callObject		*pCallObject,
	INOUT		icf_uint16_t			sockFd);
#if 0
/****************************************************************************
 * FUNCTION:           icf_ssa_init_ssa_context
 *
 * DESCRIPTION:        This routine initializes SSA Context
 * 
 *****************************************************************************/
icf_void_t   icf_ssa_init_ssa_context(
        INOUT   icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT   icf_ssa_ctxt_st  *p_ssa_ctxt);
#endif
/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_process_before_sending_auth_req
 *
 * DESCRIPTION:     This function perform misc function before sending auth req
 *                  for REGISTER and INVITE METHOD
 *
 ******************************************************************************/
icf_return_t icf_ssa_process_before_sending_auth_req(
        icf_ssa_pdb_st         *p_ssa_pdb,
        Sdf_st_callObject         **p_p_call_obj,
        Sdf_st_overlapTransInfo   **p_overlap_trans_info);

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_make_send_auth_req
 *
 * DESCRIPTION:     This function makes and send Authorized Req
 *
 ******************************************************************************/
icf_return_t icf_ssa_make_send_auth_req(
        icf_ssa_pdb_st         *p_ssa_pdb,
        Sdf_st_overlapTransInfo   *p_overlap_trans_info,
        Sdf_st_error               *p_err);

/*******************************************************************************
 * FUNCTION:        icf_ssa_append_par_to_sip_list
 *
 * DESCRIPTION:     This function appends a parameter to given SIP list
 *
 ******************************************************************************/
icf_return_t icf_ssa_append_par_to_sip_list(
        icf_ssa_pdb_st      *p_ssa_pdb,
        SipList                *p_dest,
        icf_uint8_t         *p_param_name,
        icf_uint8_t         *p_paramValue,
        Sdf_st_error           *p_err);

/*******************************************************************************
 * FUNCTION:        icf_ssa_insert_param_in_addr_spec
 *
 * DESCRIPTION:     This function inserts parametrs into the addr spec passed
 *                  to it as a paramter
 *                  1. The parmater name and value are also passed to this
 *                     function
 *
 ******************************************************************************/
icf_return_t icf_ssa_insert_param_in_addr_spec(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipAddrSpec    *p_addr_spec,
        icf_uint8_t *p_param,
        icf_uint8_t *p_value,
        Sdf_st_error *p_sdf_error);

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_fetch_auth_param_list
 *
 * DESCRIPTION:     This function extracts auth param received in 401/407
 *
 ******************************************************************************/
icf_return_t icf_ssa_fetch_auth_param_list(
        icf_ssa_pdb_st         *p_ssa_pdb,
        Sdf_st_callObject         **p_p_call_obj,
        Sdf_st_overlapTransInfo   *pOverlapTransInfo
        );

/*******************************************************************************
 * 
 * FUNCTION:        icf_ssa_send_authorized_req
 *
 * DESCRIPTION:     This function constructs and sends req with 
 *                  Authentication info on network. DIGEST and basic schemes are
 *                  supported.
 *                  1.Initialize the UAC auth info list 
 *                  2.Iterate through the authorization params list
 *                  stored in the SSA ctxt.This list was returned when
 *                  the PROXY/REGISTRAR/UAS challenged us with 401/407 response 
 *                  asking for authorization parameters.
 *                  3.If the scheme present in the elements of this list
 *                  is "Digest" or "Basic",populate the authorization parameters from
 *                  that configured on the line and append the same to the
 *                  UAC auth info list initialized earlier. 
 *                  4. Invoke sdf_ivk_uaFormRequestWithAuthorization with the
 *                  authorization paramters and send the request to peer.
 *
 ******************************************************************************/
icf_return_t icf_ssa_send_authorized_req(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_fetch_matching_overlap_txn
 *
 * DESCRIPTION:     This function fetches the matching overlap transaction
 *                  corresponding to the msg type and transaction state
 *                  state from the list of overlapped transactions in the
 *                  call object
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_fetch_matching_overlap_txn(
        INOUT    icf_ssa_pdb_st       *p_ssa_pdb,
        INOUT      Sdf_ty_messageType       msg_type,
        INOUT      Sdf_ty_state             trans_state,
        INOUT      Sdf_st_callObject        *p_call_obj,
        INOUT    Sdf_st_overlapTransInfo  **p_p_match_overlap_txn);

/****************************************************************************
 * FUNCTION:           icf_ssa_add_param_in_contact_hdr
 *
 * DESCRIPTION:        This routine fetch contact headers from InitData
 *                     Check if transport is TCP or UDP then insert 
 *                     transport=tcp/udp paramater in contact header
 * 
 *****************************************************************************/
extern icf_return_t   icf_ssa_add_param_in_contact_hdr(
        icf_ssa_pdb_st        *p_ssa_pdb,
        Sdf_st_initData          *p_init_data,
        Sdf_st_error             *pError);


/****************************************************************************
 * FUNCTION:           icf_ssa_get_last_node_of_timer_list
 *
 * DESCRIPTION:        This routine fetches last node of timer list
 * 
 *****************************************************************************/
extern icf_ssa_timer_list_st** icf_ssa_get_last_node_of_timer_list(
        icf_ssa_timer_list_st **p_timer_list);


/****************************************************************************
 * FUNCTION:           icf_ssa_insert_timer_in_list
 *
 * DESCRIPTION:        This routine inserts a timer in list
 * 
 *****************************************************************************/
extern icf_return_t icf_ssa_insert_timer_in_list(
        icf_glb_pdb_st        *p_glb_pdb,
        icf_ssa_timer_list_st **p_timer_list,
        icf_timer_id_t        timer_id,
        icf_void_t            *p_timer_data);


/****************************************************************************
 * FUNCTION:           icf_ssa_delete_timer_frm_list
 *
 * DESCRIPTION:        This routine deletes a timer from list of which timer
 *                     ID is passed
 * 
 *****************************************************************************/
extern icf_void_t icf_ssa_delete_timer_frm_list(
        icf_ssa_pdb_st   *p_ssa_pdb,
        icf_ssa_ctxt_st  *p_ssa_ctxt,
        icf_timer_id_t   tim_id);

/****************************************************************************
 * FUNCTION:           icf_ssa_check_if_any_timers_exist
 *
 * DESCRIPTION:        THis fucntion checks whether any timer is running for 
 *                     the given p_ssa_ctxt or not, it can be checked if the 
 *                     timer list is NULL or not 
 *
 *****************************************************************************/
extern icf_boolean_t   icf_ssa_check_if_any_timers_exist(
       icf_ssa_ctxt_st  *p_ssa_ctxt);

/*****************************************************************************
 * FUNCTION:        icf_ssa_clear_options_context
 *
 * DESCRIPTION:     This function in invoked when call termination is
 *                  initiated and OPTIONS is ongoing.This function sends the 
 *                  failure resp (487 Request Terminated)for the OPTIONS request
 *                  1. Stop the OPTIONS timer and flush entry from the map
 *                  2. Send failure resp to peer
 *
 *****************************************************************************/
extern icf_return_t icf_ssa_clear_options_context(
        icf_ssa_pdb_st *p_ssa_pdb);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_insert_params_frm_address_st
 * DESCRIPTION:  1.This functions checks the presence flag
 *                 present in the p_address paramter and on the
 *                 basis of this inserts paramters into the
 *                 p_addr_spec paramter passed to it
 *
 ***************************************************************************/
extern icf_return_t icf_ssa_insert_params_frm_address_st(
        icf_ssa_pdb_st *p_ssa_pdb,
        SipAddrSpec       *p_addr_spec,
        icf_address_st *p_address);

/*****************************************************************************
 * FUNCTION:        icf_ssa_fetch_ssa_ctxt_frm_map
 *
 * DESCRIPTION:     This function fetches the SSA ctxt corresponding
 *                  to the context identifier passed as an argument from 
 *                  the map maintained in the SSA global data
 * 
 *****************************************************************************/
extern icf_return_t  icf_ssa_fetch_ssa_ctxt_frm_map(
         icf_ssa_pdb_st *p_ssa_pdb,
         icf_uint16_t context_identifier,
         icf_ssa_ctxt_st **p_p_ssa_ctxt);

/*****************************************************************************
 * FUNCTION:        icf_ssa_fetch_ssa_ctxt_frm_map
 *
 * DESCRIPTION:     This function fetches the SSA ctxt corresponding
 *                  to the context identifier passed as an argument from 
 *                  the map maintained in the SSA global data
 * 
 *****************************************************************************/
extern icf_return_t  icf_ssa_fetch_ssa_ctxt_frm_map(
         icf_ssa_pdb_st *p_ssa_pdb,
         icf_uint16_t context_identifier,
         icf_ssa_ctxt_st **p_p_ssa_ctxt);

/*****************************************************************************
 * FUNCTION:        icf_ssa_flush_entry_frm_map
 *
 * DESCRIPTION:     This function flushes the entry in the map
 *                  between the context identifier and the SSA ctxt
 *                  and resets the bit ICF_SSA_OPTIONS_ONGOING
 *                  in the SSA ctxt
 * 
 *****************************************************************************/
extern icf_void_t icf_ssa_flush_entry_frm_map(
         icf_ssa_pdb_st *p_ssa_pdb,
         icf_ssa_ctxt_st *p_ssa_ctxt);

/*****************************************************************************
 * FUNCTION:        icf_ssa_form_and_insert_info_msg_body
 * DESCRIPTION:     This function is used to form and insert a message body
 *                  into the outgoing INFO request.
 *                  The contents and the content type are passed to SSA
 *                  in the unternal message
 *****************************************************************************/
extern icf_return_t icf_ssa_form_and_insert_info_msg_body(
        icf_ssa_pdb_st   *p_ssa_pdb,
        Sdf_ty_s8bit        *p_content_type,
        Sdf_ty_s8bit        *p_info_msg_body,
        SipMessage          *p_otg_sip_msg);

/*****************************************************************************
 * FUNCTION:        icf_ssa_process_options_resp
 *
 * DESCRIPTION:     This function sends the response to the OPTIONS request
 *                  corresponding to the response received from MMI
 *
 *****************************************************************************/
extern icf_return_t icf_ssa_process_options_resp(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 * FUNCTION:        icf_ssa_process_info
 * DESCRIPTION:     This function is used to send an INFO request
 *                  with/without payload to the peer.
 *                  The payload along with the content type is indicated
 *                  to SSA in the internal message
 *****************************************************************************/
extern icf_return_t icf_ssa_process_info(
        INOUT    icf_ssa_pdb_st   *p_ssa_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_process_initiate_clean_up
 *
 * DESCRIPTION:     This function processes clean up request received from CFG
 *                  This API fetches all the ssa contexts from its map and
 *                  check if any timer exists then stops it, simulate timer
 *                  expiry and then free all the memory for it.
 *                  API: ICF_CFG_SSA_INITIATE_CLEAN_UP
 *
 ******************************************************************************/
extern icf_return_t  icf_ssa_process_initiate_clean_up(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb);

/*****************************************************************************
 * FUNCTION:           icf_ssa_port_stack_trace
 *
 * DESCRIPTION:        This function is used to print stack traces
 *
 *****************************************************************************/
extern icf_return_t icf_ssa_port_stack_trace(
              Sdf_ty_uaTraceLevel dLevel,
              Sdf_ty_traceType dType,
              const Sdf_ty_s8bit *pStr,
              Sdf_ty_u32bit dLen,
              Sdf_st_error *pErr);

/*****************************************************************************
 * FUNCTION:        icf_ssa_options_timer_expiry_handler
 *
 * DESCRIPTION:     This function handles the timer expiry of the
 *                  OPTIONS timer and sends failure response to peer.
 * 
 *****************************************************************************/
extern icf_return_t icf_ssa_options_timer_expiry_handler(
        icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 * FUNCTION:        icf_ssa_clear_options_context
 *
 * DESCRIPTION:     This function in invoked when call termination is
 *                  initiated and OPTIONS is ongoing.This function sends the 
 *                  failure resp (487 Request Terminated)for the OPTIONS request
 *                  1. Stop the OPTIONS timer and flush entry from the map
 *                  2. Send failure resp to peer
 *
 *****************************************************************************/
extern icf_return_t icf_ssa_clear_options_context(
        icf_ssa_pdb_st *p_ssa_pdb);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_replaces_header
 * DESCRIPTION:  1.Fetch the Replaces header 
 *               2.Extract n/w call id and map it to ipphone internal call id. 
 *               3.If not matched then send failure response to n/w.
 *
 ***************************************************************************/
extern icf_return_t icf_ssa_parse_replaces_header(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_error		  *pErr);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_ref_by_header
 * DESCRIPTION:  1.Fetch the Referred By header 
 *               2.Extract referred by address and display name and
 *               populate it in call context.
 *
 ***************************************************************************/
extern icf_return_t icf_ssa_parse_ref_by_header(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_error		  *pErr);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_supp_header
 * DESCRIPTION:  1.Invoke the SDF API which returns the list
 *                 of entries present in the Supported header.
 *               2.Iterate through the list and check for the presence
 *                 of 100 rel.If present ,set the bit 
 *                 ICF_REL_PROV_RESP_SUPP_BY_PEER in common bitmask
 *                 of the call context
 *
 ***************************************************************************/
extern icf_return_t icf_ssa_parse_supp_header(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT    SipMessage          *p_sip_msg,
        INOUT  Sdf_st_error		  *pErr);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_n_validate_requri
 * DESCRIPTION:  This function does the folowing :
 *               1.Populates the local name of cal context on the 
 *                 basis of the dispaly name in the To header
 *               2.Extracts the Request URI and does validations on the
 *                 same.In case of failure ,sets failure bitmask accordingly
 *               3.Populates local address of call context on the 
 *                 basis of the ReqURI
 *               4.Invokes the utility function to update the Contact
 *                 header into the profile of the call object
 *
 ***************************************************************************/
icf_return_t icf_ssa_parse_n_validate_requri(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT  Sdf_st_error		  *pErr);

/*****************************************************************************
 * FUNCTION:        icf_ssa_parse_unknown_hdr
 * DESCRIPTION:     This function is used to parse an unknown header 
 *                  for a particular token.
 *                  1. On finding the requisite token,the boolean variable
 *                     p_token_found is set to true
 *****************************************************************************/
 extern icf_return_t icf_ssa_parse_unknown_hdr(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipMessage          *p_sip_msg,
        icf_uint8_t      *p_hdr_name,
        icf_uint8_t      *p_token_name,
        icf_boolean_t    *p_token_found);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_phone_context_frm_sip_uri
 * 
 * DESCRIPTION:  1.This function parses the SIP URI to extract the
 *                 user part and phone-context parameter and updates
 *                 the icf_address_st structure accordingly
 *
 ***************************************************************************/
icf_return_t icf_ssa_parse_phone_context_frm_sip_uri(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT    SipAddrSpec       *p_sip_addr_spec,
        INOUT  icf_address_st *p_addr);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_tel_url
 * 
 * DESCRIPTION:  1.This function parses the TEL URL and
 *               populates the icf_address_st structure with
 *               the required information(along with any paramters received)
 *               
 *
 ***************************************************************************/
icf_return_t icf_ssa_parse_tel_url(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT    SipAddrSpec       *p_sip_addr_spec,
        INOUT  icf_address_st *p_addr);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_unknown_param
 * DESCRIPTION:  Function is used to parse unknown parameters and return
 *               them as name value(if present) pair
 ***************************************************************************/
icf_boolean_t icf_ssa_parse_unknown_param(
        icf_ssa_pdb_st   *p_ssa_pdb,
        icf_uint8_t      *p_unk_str,
        icf_uint8_t      *p_param_name,
        icf_uint8_t      *p_param_val,
        icf_uint8_t      *p_index);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_params_from_addr_spec
 * DESCRIPTION:  1.Extract the parameters stored in SipAddrSpec structure
 *                 as a list and search for the user and phone-context 
 *                 paramters.
 *               2.On encountering the params,set the appr bit in the 
 *                 presence flag of the icf_address_st structure
 *               3.If user=phone is present ,set the address type as E164
 *
 ***************************************************************************/
icf_return_t icf_ssa_parse_params_from_addr_spec(
        INOUT  icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT    SipAddrSpec         *p_addr_spec,
        INOUT  icf_address_st   *p_addr);
 /*****************************************************************************
 * FUNCTION:        icf_ssa_get_reqline_as_string
 *
 * DESCRIPTION:     This function gets the request line from the SIP Message
 *                  and fills it as a string in the parameter p_target_str
 *					
 *****************************************************************************/
icf_return_t icf_ssa_get_reqline_as_string(SipMessage *p_sip_msg, 
                                                 icf_uint8_t *p_target_str);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_add_default_header
 **
 ** DESCRIPTION:        This function adds a headrer with the name p_hdr_name
 **                     and populates it with the values specified in the 
 **                     string list p_hdr_content
 **
 *****************************************************************************/
icf_return_t icf_ssa_add_default_header(Sdf_st_initData *p_profile,
                                              icf_uint8_t *p_name,
                                              icf_string_list_st *p_hdr_content,
                                              icf_error_t *p_ecode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_profile_for_reg
 **
 ** DESCRIPTION:        This function makes a SIP profile for a registration
 **                     message
 **
 *****************************************************************************/
icf_return_t icf_ssa_make_profile_for_reg(Sdf_st_initData *p_profile,icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_app_profile
 **
 ** DESCRIPTION:        This function makes a SIP profile for an ICF application
 **
 *****************************************************************************/
icf_return_t icf_ssa_make_app_profile(Sdf_st_initData *p_profile,icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_profile_for_internal_subscribe
 **
 ** DESCRIPTION:        This function makes a SIP profile for ICF initated 
 **                     SUBSCRIBE
 **
 *****************************************************************************/
icf_return_t icf_ssa_make_profile_for_internal_subscribe(Sdf_st_initData *p_profile,
                                                               icf_ssa_pdb_st *p_ssa_pdb);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_validate_basic_method
 **
 ** DESCRIPTION:        This function checks if the method in sip message is 
 **                     supported by the application. This fn checks only the 
 **                     basic methods (INVITE,ACK,CANCEL,OPTIONS,BYE)
 **                     p_invalid is set to true if method recieved one of the 
 **                     basic methods and the application supports basic
 **                     methods. Otherwise it is set to false
 **
 *****************************************************************************/
icf_return_t icf_ssa_validate_basic_method(
                                                 icf_ssa_pdb_st *p_ssa_pdb,
                                                 SipMessage *p_sip_msg, 
                                                 icf_boolean_t *p_invalid);
/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_allow_hdr_from_app_profile
 **
 ** DESCRIPTION:        This function makes an allow header from the methods 
 **                     specified in the supported methods list of an application
 *****************************************************************************/

icf_return_t icf_ssa_make_allow_hdr_from_app_profile(icf_ssa_pdb_st *p_ssa_pdb,
                                                           SipHeader **p_p_allow_hdr);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_allow_hdr_for_int_subscribe
 **
 ** DESCRIPTION:        This function makes an allow header for SUBSCRIBE
 *****************************************************************************/
icf_return_t icf_ssa_make_allow_hdr_for_int_subscribe(icf_ssa_pdb_st *p_ssa_pdb,
                                                           SipHeader **p_p_allow_hdr);
/*****************************************************************************
 ** FUNCTION:           icf_ssa_make_allow_hdr_for_reg
 **
 ** DESCRIPTION:        This function makes an allow header for REGISTER
 *****************************************************************************/

icf_return_t icf_ssa_make_allow_hdr_for_reg(icf_ssa_pdb_st *p_ssa_pdb,
                                                           SipHeader **p_p_allow_hdr);

/*****************************************************************************
** FUNCTION: icf_ssa_addDefaultHeader
**
** DESCRIPTION: Adds Header information to the call profile.
**
******************************************************************************/
icf_return_t icf_ssa_addDefaultHeader
	(Sdf_st_initData *pInitData,
	icf_int8_t *pName,
	icf_int8_t *pValue,
	Sdf_ty_bool dAddAlways,
	Sdf_st_error *pErr);
/*****************************************************************************
** FUNCTION: icf_ssa_match_app_filter
**
** DESCRIPTION: Match application Filiter
**
******************************************************************************/
icf_return_t icf_ssa_match_app_filter(icf_ssa_pdb_st *p_ssa_pdb,
                                        SipMessage *p_sip_msg,
                                        icf_app_conf_st *p_app_conf,
                                        icf_boolean_t *p_match_success,
                                        icf_error_t *p_ecode);

/*Message body header changes Start*/


/*****************************************************************************
** FUNCTION: icf_ssa_get_media_type_from_content_type
**
** DESCRIPTION: This function will get the media type from the content-type
**              string as provided by the application to be used in the outgoing
**              SIP message for both SIP message header and message body header.
**
******************************************************************************/
icf_return_t icf_ssa_get_media_type_from_content_type(
		INOUT  icf_uint8_t   *p_content_type,
		OUT Sdf_ty_s8bit     **p_p_media_type
		);

/*****************************************************************************
** FUNCTION: icf_ssa_get_content_type_from_msg
**
** DESCRIPTION: The function extracts the content_type from incoming SIP message
**              or message body. This will extract the Media Type and parameters
**              from the Content-Type, if any are present in the content-type 
**              header. This function will invoke SIP APIS directly to get the 
**              content-type header values.
**
******************************************************************************/

icf_return_t icf_ssa_get_content_type_frm_msg(
			/*INOUT  SipMessage			*pSipMsg, */
			INOUT   en_SipMessageType dReqRespType,
	        INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
	        INOUT  SipHeader           *pHeader,
		OUT icf_int8_t         **p_p_content_type
		);
/*Message body header changes Ends*/

/*****************************************************************************
** FUNCTION: icf_ssa_set_unknown_hdr_in_msg
**
** DESCRIPTION: This function populates the unknown header in the SIP message 
**              body. These headers will be provided by the application. This
**              function will invoke the SIP APIs directly to set the header.
**
******************************************************************************/

icf_return_t icf_ssa_set_unknown_hdr_in_msg(
		INOUT    icf_header_st  *p_msg_hdr,
		INOUT  SipMimeHeader     *pMimeHdr
		);

/*****************************************************************************
** FUNCTION: icf_ssa_form_and_insert_content_disp_hdr_in_msg
**
** DESCRIPTION: This function form and insert the content disposition header
**              in the SIP message body headers in the outgoing request.
**
******************************************************************************/
icf_return_t icf_ssa_form_and_insert_content_disp_hdr_in_msg(
		INOUT    icf_header_st       *p_msg_hdr,
		INOUT  SipMimeHeader          *pMimeHdr
		);

/*****************************************************************************
** FUNCTION: icf_ssa_fill_msg_body_hdrs
**
** DESCRIPTION: This function insert the message body headers in the outgoing 
**              Sip Message.
**
******************************************************************************/
icf_return_t icf_ssa_fill_msg_body_hdrs(
		INOUT    icf_ssa_pdb_st        *p_ssa_pdb,
		INOUT    icf_header_list_st    *p_hdr_list,
	        INOUT  SipMsgBody               *pMsgBody
		);

/*****************************************************************************
** FUNCTION: icf_ssa_get_mime_hdrs_from_sip_msg
**
** DESCRIPTION: This function retrieves the MIME headers from the SIP message.
**              This function will invoke the SIP API's directly to get these 
**              headers.
**
******************************************************************************/
icf_return_t icf_ssa_get_mime_hdrs_from_sip_msg(
		INOUT   en_SipMessageType dReqRespType, 
		INOUT   icf_ssa_pdb_st    *p_ssa_pdb,
		INOUT   SipMsgBody        *pMsgBody,
		OUT  icf_msg_body_st   *p_msg_body
		);

/************************************************************************
** FUNCTION: icf_ssa_get_content_type_from_mime_body
**
** DESCRIPTION: The function extracts the content-tpe for an individual
**              Msg Body
**
************************************************************************/
icf_return_t icf_ssa_get_content_type_from_mime_body(
	/*INOUT  Sdf_st_callObject   *pObj,*/
	INOUT   en_SipMessageType dReqRespType,
	INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
	INOUT  SipMsgBody		*pMsgBody,
	INOUT  SipMimeHeader       *pMime,
	OUT icf_int8_t	**p_p_content_type);


/*******************************************************************************
 ** FUNCTION:           icf_ssa_make_otg_app_profile
 **
 ** DESCRIPTION:        This function makes a SIP profile for an ICF application
 **                     for an outgoing message. The Supported and Accept 
 **                     headers are formed from the header list in request API 
 **                     instead of the stored app profile.
 **
 ******************************************************************************/
icf_return_t icf_ssa_make_otg_app_profile(
                Sdf_st_initData         *p_profile,
                icf_ssa_pdb_st       *p_ssa_pdb);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_append_sip_uri_param_in_sip_hdr
 **
 ** DESCRIPTION:        This Function Appends uri_parameters in the SIP Header
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_append_sip_uri_param_in_sip_hdr(
		icf_ssa_pdb_st	*p_ssa_pdb,
        icf_uint8_t	*p_sip_uri_param,
		SipHeader		*p_header);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_append_sip_uri_hdr_param_in_sip_hdr
 **
 ** DESCRIPTION:        This Function Appends uri_parameters in the SIP Header
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_append_sip_uri_hdr_param_in_sip_hdr(
		icf_ssa_pdb_st	*p_ssa_pdb,
        icf_uint8_t	*p_sip_uri_hdr_param,
		SipHeader		*p_header);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_append_sip_uri_param_in_addr_spec
 **
 ** DESCRIPTION:        This Function Appends uri_parameters in the addr spec
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_append_sip_uri_param_in_addr_spec(
		icf_ssa_pdb_st	*p_ssa_pdb,
        icf_uint8_t	*p_sip_uri_param,
		SipAddrSpec		*p_addr_spec);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_append_sip_uri_hdr_param_in_addr_spec
 **
 ** DESCRIPTION:        This Function Appends uri_parameters in addr_spec
 **
 *****************************************************************************/
extern icf_return_t icf_ssa_append_sip_uri_hdr_param_in_addr_spec(
		icf_ssa_pdb_st	*p_ssa_pdb,
        icf_uint8_t	*p_sip_uri_hdr_param,
		SipAddrSpec		*p_addr_spec);
/*****************************************************************************
 ** FUNCTION:           icf_ssa_validate_sip_uri_param_in_sip_url
 **
 ** DESCRIPTION:        This Function validates uri_parameters 
 **						in the SipUrl. 
 **
 *****************************************************************************/
icf_return_t icf_ssa_validate_sip_uri_param_in_sip_url(
		icf_ssa_pdb_st	*p_ssa_pdb,
		SipUrl				*p_sip_url,
		icf_uint32_t		dType);


ICF_HANDLE_REINVITE_START
/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_call_modify_req
 **
 ** DESCRIPTION:        This function is called to send UPDATE/RE-INVITE to
 **                     the remote party.The decision as to whether to send
 **                     an UPDATE or RE-INVITE is made on the basis of a 
 **                     configuration parameter
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_call_modify_req(
        icf_ssa_pdb_st *p_ssa_pdb);
ICF_HANDLE_REINVITE_END

/*Message body header changes Start*/

	
/*****************************************************************************
 ** FUNCTION:           icf_ssa_SetAllMsgBody
 **
 ** DESCRIPTION:        This function is called to set the message bodies in the
 **                     outgoing SIP message.
 **
 *****************************************************************************/
icf_return_t icf_ssa_SetAllMsgBody
	(
	 Sdf_st_transaction      *pTransaction,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
   	 Sdf_ty_s8bit            *p_content_type,
         Sdf_st_error            *pError);

/*Message body header changes Ends*/

	
/*******************************************************************************
 * FUNCTION:        icf_ssa_check_for_param_in_sipurl
 * DESCRIPTION:     This function gets the parameters present in the referred-to
					address.
 ******************************************************************************/

icf_return_t icf_ssa_check_for_param_in_sipurl( 
				INOUT			  SipUrl		*pUrl, 
				INOUT  		  icf_uint8_t	*p_param_name, 
				INOUT 		  icf_uint8_t	*p_out_param_string, 
				INOUT 		  icf_error_t	*p_ecode);


/*NOTIFY_OUT_OF_DIALOG_START*/

/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_notify_out_of_dialog
 *
 * DESCRIPTION:     This function parses NOTIFY req received out of dialog
 *
 ******************************************************************************/
icf_return_t   icf_ssa_handle_notify_out_of_dialog(
        icf_ssa_pdb_st              *p_ssa_pdb,
        icf_nw_inc_notify_ind_st    *p_notify_ind,
        Sdf_st_overlapTransInfo        *pOverlapTransInfo,
        SipMessage                     *pSipMsg);

/*NOTIFY_OUT_OF_DIALOG_END*/


/*****************************************************************************
** FUNCTION: icf_ssa_match_app_methods
**
** DESCRIPTION: Match application's Supported Methods 
**
******************************************************************************/
icf_return_t icf_ssa_match_app_methods(icf_ssa_pdb_st *p_ssa_pdb,
                                        SipMessage *p_sip_msg,
                                        icf_app_conf_st *p_app_conf,
                                        icf_boolean_t *p_match_success,
                                        icf_error_t *p_ecode);
/*HANDLE_INFO_IND_START*/

/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_info_ind
 *
 * DESCRIPTION:     This function parses INFO req received
 *
 ******************************************************************************/
icf_return_t   icf_ssa_handle_info_ind(
        icf_ssa_pdb_st              *p_ssa_pdb,
        icf_nw_inc_info_ind_st		*p_info_ind,
        Sdf_st_overlapTransInfo     *pOverlapTransInfo);
/*HANDLE_INFO_IND_END*/


/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_info
**
** DESCRIPTION: This Function handles All response received for 
**              INFO Request Sent to Network
**
**********************************************************/
Sdf_ty_retVal icf_ssa_transaction_Completed_for_info(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);

icf_return_t icf_ssa_get_replaces_header_from_referto(
     SipReferToHeader *pReferToHdr,
	 SipReplacesHeader **ppReplacesHdr,
	 Sdf_st_error *pErr);

/*******************************************************************************
 * FUNCTION:        icf_ssa_validate_mode_n_addr_type
 *
 * DESCRIPTION:     
 *
 ******************************************************************************/
icf_return_t   icf_ssa_validate_mode_n_addr_type(
        icf_ssa_pdb_st              *p_ssa_pdb);

Sdf_ty_retVal sdf_cbk_uaSendCallToPeer  (Sdf_st_callObject  \
                *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo,
                SipMessage *pSipMsg, SipOptions dOptions, \
                Sdf_ty_s8bit transptype, Sdf_st_eventContext *pContext, \
                Sdf_st_error *pErr);

#ifdef ICF_DNS_LOOKUP_ENABLED

icf_return_t icf_ssa_resend_ongoing_txn(
		Sdf_st_callObject *pCallObj,
		SipMessage *pMessage,
		Sdf_st_overlapTransInfo *pOverlapTransaction,
		SipOptions dSendOptions);

icf_return_t    icf_ssa_process_dns_fail_resp(icf_ssa_pdb_st *p_ssa_pdb,
                Sdf_st_dnsEventContext *p_dns_evt_ctxt);

icf_return_t    icf_ssa_get_msg_mthd_type_frm_txn(icf_ssa_pdb_st *p_ssa_pdb,
                Sdf_st_transaction      *p_txn,
                Sdf_st_overlapTransInfo *p_overlap_txn,
                en_SipMessageType       *p_msg_type,
                Sdf_ty_s8bit            *p_mthd);

/*******************************************************************************
 * FUNCTION:        icf_ssa_post_dns_resp_processing
 *
 * DESCRIPTION:     This function is triggered after a DNS resposne has
 * received and transaction has been updated successfully.
 ******************************************************************************/
icf_void_t icf_ssa_post_dns_resp_processing(
	Sdf_st_dnsEventContext *p_dns_evt_ctxt, 
	Sdf_ty_bool            dns_resp_flag);

icf_return_t icf_ssa_get_next_dest_from_list(
				icf_ssa_pdb_st			*p_ssa_pdb,
                Sdf_st_callObject		*pCallObj,
                Sdf_st_transaction		*p_txn,
                Sdf_st_overlapTransInfo *p_overlap_txn);
/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_send_to_peer_no_dns_lookup
 * DESCRIPTION:  1.This function internally calls sdf_ivk_uaSendCallToPeer
 *               after initialising an Event Context and setting the 
 *               SSA ctxt into it.
 *               2.In the case of TCP as transport,
 *               we are populating the current transactions' SockFd
 *               with the one in the ConnDb,  if there is one,if not
 *               we reset it to an invalid value so that UATK attempts a 
 *               connect by itself
 *             
 *               The functionality of this function is similar to function 
 *               icf_ssa_send_to_peer except that is would not do DNS query 
 ***************************************************************************/
icf_return_t icf_ssa_send_to_peer_no_dns_lookup(
        icf_ssa_pdb_st *p_ssa_pdb,
        Sdf_st_callObject *p_call_obj,
        SipMessage *p_sip_msg,
        SipOptions *p_options,
        Sdf_st_error *p_sdf_error);


#endif

/***************************************************************************
 **
 ** FUNCTION:     icf_ssa_set_pending_in_ctxt
 * DESCRIPTION:  
 * 		This function is written to be a generic function which
 * can be called from multiple places in SSA. It sets the bitmask to indicate
 *  what kind of method is pending on successful TCP COnnect completion
 *  so that approriate action could be taken when asynch response for TCP 
 *  connect is received
***************************************************************************/
icf_return_t	icf_ssa_set_pending_in_ctxt(icf_ssa_pdb_st	*p_ssa_pdb,
				Sdf_ty_s8bit	*p_method,
				icf_ssa_ctxt_st	*p_ssa_ctxt);

icf_return_t icf_ssa_attempt_out_of_uatk_connect(
        icf_ssa_pdb_st			*p_ssa_pdb,
        Sdf_st_callObject		*p_call_obj,
        Sdf_st_transaction		*p_txn,
        Sdf_st_overlapTransInfo	*p_overlap_txn,
		icf_boolean_t			*p_ready_to_send);

/***************************************************************************
 ** FUNCTION:     icf_ssa_initiate_dispatch_sequence
 * DESCRIPTION:  1.This function is the last stop before invocation of 
* SendCallToPeer API. It expects that p_ssa_pdb contains pointer to 
* glb_pdb, global cfg data and global ssa data
*       The function does following steps:
*       1. sets options field for compaction of msg if required
        2. set the conn FD in all transactions in case of TCP/TLS call
        3. Invokes Send Call To Peer
        4. In case of IPSEC checks if count of number of transctions on the SA
                need to be increased or decreased
 ***************************************************************************/
icf_return_t icf_ssa_initiate_dispatch_sequence(
        icf_ssa_pdb_st				*p_ssa_pdb,
        Sdf_st_callObject           *p_call_obj,
        Sdf_st_transaction          *p_txn,
        Sdf_st_overlapTransInfo     *p_overlap_txn,
		Sdf_st_eventContext 		*p_event_context,
	    icf_error_t					*p_ecode);



/*****************************************************************************
 ** FUNCTION:           icf_ssa_fill_stream_sdp_info_in_cctx
 **
 ** DESCRIPTION:        When an incoming SIP message with SDP is received by 
 **                     SSA, it gets the media parameters from SDP and creates
 **                     a list of type icf_inc_media_sdp_info_st. Once this is
 **                     done, this function updates the remote_received_sdp in
 **                     call_ctx->p_stream_sdp_info for each received stream.
 **
 **                     This function calls different utility functions 
 **                     depending on whether the p_stream_sdp_info list in
 **                     call_ctx has already been created before or needs to be 
 **                     created afresh by SSA.
 **
 *****************************************************************************/
icf_return_t icf_ssa_fill_stream_sdp_info_in_cctx(
    INOUT    icf_ssa_pdb_st	            *p_ssa_pdb,
    INOUT      icf_list_st		            *p_media_sdp_info, /* list of type icf_inc_media_sdp_info_st */
    INOUT    icf_call_ctx_st             *p_call_ctx);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_create_stream_sdp_info_in_cctx
 **
 ** DESCRIPTION:        When an incoming SIP message with SDP is received by 
 **                     SSA, it gets the media parameters from SDP and creates
 **                     a list of type icf_inc_media_sdp_info_st. Once this is
 **                     done, this function updates the remote_received_sdp in
 **                     call_ctx->p_stream_sdp_info for each received stream.
 **
 **                     This function also allocates memory to the list
 **                     p_stream_sdp_info before setting remote SDP in it.
 **
 **                     This function should be used in scenarios when the list 
 **                     p_stream_sdp_info in call_ctx is null
 **                     (eg. on receiving initial offer from peer)
 **
 *****************************************************************************/
icf_return_t icf_ssa_create_stream_sdp_info_in_cctx(
    INOUT    icf_ssa_pdb_st	            *p_ssa_pdb,
    INOUT      icf_list_st		            *p_media_sdp_info, /* list of type icf_inc_media_sdp_info_st */
    INOUT    icf_call_ctx_st             *p_call_ctx);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_replace_stream_sdp_info_in_cctx
 **
 ** DESCRIPTION:        When an incoming SIP message with SDP is received by 
 **                     SSA, it gets the media parameters from SDP and creates
 **                     a list of type icf_inc_media_sdp_info_st. Once this is
 **                     done, this function updates the remote_received_sdp in
 **                     call_ctx->p_stream_sdp_info for each received stream.
 **
 **                     If the remote_received_sdp is not already null in 
 **                     call_ctx->p_stream_sdp_info, this function will first 
 **                     free the old pointer and then reassign the new pointer.
 **
 **                     This function should be used in scenarios when the list 
 **                     p_stream_sdp_info in call_ctx is not null.
 **                     (eg. on receiving answer or re-offer from peer)
 **
 *****************************************************************************/
icf_return_t icf_ssa_replace_stream_sdp_info_in_cctx(
    INOUT    icf_ssa_pdb_st	            *p_ssa_pdb,
    INOUT      icf_list_st		            *p_media_sdp_info, /* list of type icf_inc_media_sdp_info_st */
    INOUT    icf_call_ctx_st             *p_call_ctx);


/*****************************************************************************
 ** FUNCTION:     icf_ssa_form_sdp_from_media_sdp_info
 **
 ** DESCRIPTION:  This function forms the SDP from the stream-level parameters 
 **               present in the media info list input to this function.
 **
 **               This function forms Sip Sdp from p_int_sdp_stream in the list 
 **               of type icf_inc_media_sdp_info_st
 **
 *****************************************************************************/
icf_return_t icf_ssa_form_sdp_from_media_sdp_info(
    INOUT    SdpMessage                  **p_p_sdpmsg,
    INOUT      icf_ssa_pdb_st              *p_ssa_pdb,
    OUT     icf_list_st		            *p_stream_sdp_info, /* list of type icf_inc_media_sdp_info_st */
    OUT     Sdf_st_error                *pError);


/***************************************************************************
**
** FUNCTION:     icf_ssa_form_audio_from_sdp_info
** DESCRIPTION:  This function forms the SDP for the audio media parameters 
**               present in the SDP structure of the call context.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**               This function forms Sip Sdp from p_local_sdp in the list 
**               of type icf_stream_sdp_info_st
**
***************************************************************************/
icf_return_t icf_ssa_form_audio_from_sdp_info(
        INOUT    icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT     icf_stream_sdp_info_st    *p_stream_sdp,
        INOUT 	  Sdf_ty_s8bit        	    *pSAddr,
        INOUT     icf_uint8_t               hold_option,
        OUT SdpMedia                  **p_p_media,
        /* list of type icf_stream_sdp_info_st */
        OUT   Sdf_st_error              *p_error);



/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_video_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for vidoe stream.
 *   
 ******************************************************************/

icf_return_t icf_ssa_form_video_from_sdp_info(
		INOUT	icf_ssa_pdb_st			*p_ssa_pdb, 
		INOUT 	icf_stream_sdp_info_st  *p_stream_sdp,
		INOUT 		Sdf_ty_s8bit			*pSAddr,
                INOUT       icf_uint8_t             hold_option,
		OUT		SdpMedia				**p_p_media,
		OUT		Sdf_st_error			*p_error);


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_t38_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for t38 stream.In case of t.38,
 *					there would be no stream which is mid-call rejected.
 *   
 ******************************************************************/
#ifdef ICF_FAX_SUPPORT

icf_return_t icf_ssa_form_t38_from_sdp_info(
		INOUT	icf_ssa_pdb_st			*p_ssa_pdb, 
		INOUT 	icf_stream_sdp_info_st  *p_stream_sdp,
		INOUT 		Sdf_ty_s8bit			*pSAddr,
		OUT		SdpMedia				**p_p_media,
		OUT		Sdf_st_error			*p_error);


#endif


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_tbcp_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for tbcp stream
 *   
 ******************************************************************/
 icf_return_t icf_ssa_form_tbcp_from_sdp_info(
		INOUT icf_ssa_pdb_st          *p_ssa_pdb, 
		INOUT 	icf_stream_sdp_info_st  *p_stream_sdp,
		INOUT 		Sdf_ty_s8bit        	*pSAddr,
		OUT		SdpMedia                **p_p_media,
		OUT		Sdf_st_error			*p_error);



icf_return_t icf_ssa_get_mode_from_stream_info(
        INOUT icf_ssa_pdb_st    *p_ssa_pdb,
        INOUT  icf_uint8_t         media_mode,
        INOUT  icf_uint8_t         hold_option,
        OUT icf_uint8_t         *p_mode
        );

icf_return_t icf_ssa_form_audio_codec_attrs_in_media_stream(
        INOUT icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT  SdpMedia                  *p_media,
        INOUT  icf_list_st               *p_codec_info_list,
        OUT   Sdf_st_error              *p_error);

icf_return_t icf_ssa_form_encoded_attrs_in_sdp_stream(
        INOUT icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT  SdpMedia                  *p_media,
        INOUT     icf_list_st             *p_encoded_attr_list,
        OUT   Sdf_st_error              *p_error);


icf_return_t icf_ssa_form_media_attrs_in_audio_stream(
        INOUT icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT  icf_stream_sdp_info_st    *p_stream_sdp,
        /* list of type icf_stream_sdp_info_st */
        INOUT  SdpMedia                  *p_media,
        OUT   Sdf_st_error              *p_error);
        
icf_return_t icf_ssa_init_n_form_c_line(
        INOUT		icf_ssa_pdb_st	            *p_ssa_pdb,
        INOUT		icf_transport_address_st	*p_conn_addr, 	
        OUT		SdpConnection               **p_p_connection,
        OUT     Sdf_st_error                *p_error);

/*Aman*/


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_populate_media_sdp_info 
 *
 * DESCRIPTION:     This function populates the media related   
 *                  parameters in the structures.
 *
 ******************************************************************/

icf_return_t icf_ssa_populate_media_sdp_info(
    INOUT      icf_ssa_pdb_st              *p_ssa_pdb,
    INOUT    SipMessage                  *p_sip_msg,
    INOUT    SdpMessage                  *p_sdpMsg,
    INOUT    SdpMedia                    *p_media_line,
    INOUT    Sdf_ty_s8bit                *p_mvalue,
    INOUT    Sdf_ty_s8bit                *p_mformat,
    INOUT    icf_inc_media_sdp_info_st	*p_media_sdp_info);


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_rtp_addr_n_port_frm_media_line 
 *
 * DESCRIPTION:     This function populates the rtp addr and port from the connection line.
 *					It first checks at the session level otherwise it checks at the
 *					media level.
 *
 ******************************************************************/

icf_return_t icf_ssa_get_rtp_addr_n_port_frm_media_line(
					INOUT		icf_ssa_pdb_st				 *p_ssa_pdb,
					INOUT      SdpMessage					 *p_sdpMsg,
					INOUT		SdpMedia					 *p_media_line,
		       		OUT		icf_internal_sdp_stream_st   *p_int_sdp);


/*********************************************************************************
 *
 * FUNCTION:        icf_ssa_get_bandwidth_frm_media_line 
 *
 * DESCRIPTION:     This function fetches the bandwidth parameter from the 
 *					media line and populates the bandwidth value in the corresponding
 *					stream.If the ongoing stream is AUDIO, then value is populated in 
 *					parameters and if stream is VIDEO, then value is populated in its 
 *					media parameters.
 **********************************************************************************/

icf_return_t icf_ssa_get_bandwidth_frm_media_line(
				INOUT		icf_ssa_pdb_st				*p_ssa_pdb,
				INOUT		SdpMedia					*pSdpMedia,
				INOUT		Sdf_ty_s8bit				*p_mvalue,
				OUT		icf_internal_sdp_stream_st  *p_int_sdp);

/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_media_parameters_frm_attrb_line
 *
 * DESCRIPTION:     This function checks for any attributes parameters   
 *                  corresponding to the media-line received.
 *                  It checks for all the valid attr that are supposed   
 *                  to be received.
 *
 ******************************************************************/

icf_return_t icf_ssa_get_media_parameters_frm_attrb_line(
					 INOUT		icf_ssa_pdb_st		        *p_ssa_pdb,
                     INOUT 	    SdpMedia			        *p_media_line,
					 INOUT 		Sdf_ty_s8bit			    *p_mvalue,
					 INOUT   icf_internal_sdp_stream_st  *p_int_sdp_info);


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_media_n_encoded_attrb_frm_media_line
 *
 * DESCRIPTION:     The function checks the attribute line.
 *					If a-line contains mode value, then populate the 
 *					corresponding mode in the internal stream sdp
 *					else it is known in our present scenerio.So, populate
 *					encoded parameters at media level.
 ******************************************************************/

icf_return_t icf_ssa_get_media_n_encoded_attrb_frm_media_line(
          INOUT  icf_ssa_pdb_st        *p_ssa_pdb,
          INOUT  SdpAttr         *p_tempAttr,
          INOUT    icf_internal_sdp_stream_st  *p_int_sdp_info);


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_validate_n_alloc_inc_media_line
 * DESCRIPTION:     This function validates the incoming SDP for duplicate
 *                  and rejected streams.This function also validates the 
 *                  protocol for the received stream. IF any validation 
 *                  fails then 415 "Unsupported Media Type" bitmask will be
 *                  set in ssa context.
 *
 ***********************************************************************/

icf_return_t icf_ssa_validate_n_alloc_inc_media_line(
			INOUT     icf_ssa_pdb_st                      *p_ssa_pdb,
			INOUT     SdpMedia                            *p_media,
			INOUT     Sdf_ty_s8bit                        *p_mvalue,
			INOUT     icf_uint8_t                         msg_identifier,
			INOUT     icf_uint8_t                         stream_id,
			OUT    icf_ssa_inc_media_stream_info_st    *p_inc_media_info,
			OUT    icf_error_t                         *p_error);

			
/*******************************************************************************
 * FUNCTION:        icf_ssa_modify_callobj_profile_after_call_accept
 * DESCRIPTION:     This function updates the call object profile for the supported
 *                  headers.
 ******************************************************************************/
icf_return_t icf_ssa_modify_callobj_profile_after_call_accept
(
 INOUT   icf_ssa_pdb_st       *p_ssa_pdb,
 INOUT   Sdf_st_initData      *pInitData,
 OUT     Sdf_st_error         *p_error);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_init_n_form_o_line
 **
 ** DESCRIPTION:        This function forms the origin line.
 **						In case of failure, the memory that is allocated p_origin is 
 **						freed here and failure is returned above and the callee function
 **						free the other memories.
 *****************************************************************************/
icf_return_t icf_ssa_init_n_form_o_line(
		INOUT		icf_ssa_pdb_st			*p_ssa_pdb,
		INOUT      icf_uint8_t				hold_option,
		INOUT	icf_transport_address_st		 stream_addr,
		INOUT 	SdpOrigin				**p_origin,
		INOUT	Sdf_st_error			*p_error);

/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_amr_codec_attr_in_media_stream
 * DESCRIPTION:     This function forms the amr codec attributes in the media
 *                  stream.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_amr_codec_attr_in_media_stream(
        INOUT	  icf_ssa_pdb_st	        *p_ssa_pdb,
        INOUT     SdpMedia                  *p_media,
        INOUT     icf_codec_attrib_st       *p_codec_info,
        OUT   Sdf_st_error              *p_error);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_free_inc_stream_stream_info
 **
 ** DESCRIPTION:        This function free the allocated memory to the structure
 **						icf_ssa_inc_media_stream_info_st.	
 *****************************************************************************/
icf_return_t icf_ssa_free_inc_media_stream_info(
			INOUT		icf_ssa_pdb_st *p_ssa_pdb,
			INOUT 	icf_ssa_inc_media_stream_info_st **p_p_inc_media_sdp_info);


/*****************************************************************************
 ** FUNCTION:           icf_ssa_get_amr_attrib_from_media_stream
 **
 ** DESCRIPTION:        This function will retrieve the AMR attributes 
 **                     received in the audio stream.This will onl extract the
 **                     octet-align,crc and mode-set attribute parameters.
 **
 *****************************************************************************/

icf_return_t icf_ssa_get_amr_attrib_from_media_stream
(
 INOUT     icf_ssa_pdb_st         *p_ssa_pdb,
 INOUT  icf_codec_attrib_st    *p_codec_attr,
 INOUT  icf_uint8_t            *p_codec_str
);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_form_transport_mode_for_stream
 **
 ** DESCRIPTION:        This function will form the transport mode for the 
 **                     requested stream.
 *****************************************************************************/

icf_return_t icf_ssa_form_transport_mode_for_stream
(
 INOUT icf_ssa_pdb_st                  *p_ssa_pdb,
 INOUT icf_media_transport_mode_et     mode,
 OUT icf_uint8_t                    *p_transport_mode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_fill_encoding_name_for_standard_codecs
 **
 ** DESCRIPTION:        This function fills codec encoding name for the codecs
 **                     for which the encoding name is not received in the SDP.
 **
 *****************************************************************************/

 icf_return_t   icf_ssa_fill_encoding_name_for_standard_codecs
 (
 INOUT     icf_ssa_pdb_st  *p_ssa_pdb,
 INOUT   icf_list_st     *p_codec_list
 );


/*********************************************************
** FUNCTION: icf_ssa_transaction_Completed_for_info
**
** DESCRIPTION: This Function handles All response received for 
**              INFO Request Sent to Network
**
**********************************************************/
Sdf_ty_retVal icf_ssa_transaction_Completed_for_info(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    resp_code,
        Sdf_st_error                    *pErr);

icf_return_t icf_ssa_clear_header_from_profile(Sdf_st_initData *pInitData,
                                              icf_uint8_t *pName,
                                              icf_error_t *p_ecode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_add_string_list_in_default_header
 **
 ** DESCRIPTION:        This function adds a headrer with the name p_hdr_name
 **                     and populates it with the values specified in the 
 **                     string list p_hdr_content
 **
 *****************************************************************************/
icf_return_t icf_ssa_add_string_list_in_default_header(Sdf_st_initData *p_profile,
                                              icf_uint8_t *p_name,
                                              icf_string_list_st *p_hdr_content,
                                              Sdf_ty_bool dAddAlways,
                                              icf_error_t *p_ecode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_refresh_supported_content_and_events
 **
 ** DESCRIPTION:        This function refreshes the supported and accept headers 
 **                     in the callobject profile from the header list passed as
 **                     a parameter. The accept and supported headers are removed
 **                     from the header list passed and put in the callobjects 
 **                     profile
 **
 *****************************************************************************/
icf_return_t icf_ssa_refresh_supported_content_and_events(icf_ssa_pdb_st *p_ssa_pdb,
                                                          icf_header_list_st *p_hdr_list,
                                                          icf_error_t *p_ecode);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_form_sdp_for_options_resp
 **
 ** DESCRIPTION:        This function is used form the SDP in the outgoing resp.
 **			for the incoming OPTIONS received from the network. 
 **
 *****************************************************************************/
icf_return_t icf_ssa_form_sdp_for_options_resp(
        INOUT  SdpMessage                **p_p_sdpmsg,
	INOUT    icf_ssa_pdb_st		*p_ssa_pdb,
        INOUT  icf_list_st            	*p_stream_sdp_info,
        OUT   Sdf_st_error              *p_error);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_form_video_for_options_from_sdp_info
 **
 ** DESCRIPTION:        This function is used to form the Video SDP in the 
 **			outgoing response for the incoming OPTIONS received 
 **			from the network. 
 **
 *****************************************************************************/
icf_return_t icf_ssa_form_video_for_options_from_sdp_info(
	INOUT	icf_ssa_pdb_st			*p_ssa_pdb, 
        INOUT 	icf_inc_media_sdp_info_st	*p_stream_sdp, 
	OUT	SdpMedia			**p_p_media,
	OUT	Sdf_st_error			*p_error);


/***************************************************************************
**
** FUNCTION:     icf_ssa_form_audio_for_options_from_sdp_info
** DESCRIPTION:  This function forms the SDP for the audio media parameters 
**               present in the SDP structure of the Options Response.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**
***************************************************************************/
icf_return_t icf_ssa_form_audio_for_options_from_sdp_info(
        INOUT    icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT    icf_inc_media_sdp_info_st *p_stream_sdp,
        INOUT  SdpMedia                  **p_p_media,
        OUT   Sdf_st_error              *p_error);


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_media_attrs_for_options_in_audio_stream
 * DESCRIPTION:     This function forms the media attributes for the audio 
 *                  stream. This function will form the ptime,
 *                  silence_suppresion attributes in the media stream.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_media_attrs_for_options_in_audio_stream(
        INOUT		icf_ssa_pdb_st	            *p_ssa_pdb,
        INOUT    icf_inc_media_sdp_info_st   *p_stream_sdp,
        INOUT  SdpMedia                       *p_media,
        OUT   Sdf_st_error                   *p_error);

/***************************************************************************
 *
 * FUNCTION:     icf_ssa_parse_require_header
 * DESCRIPTION:  1.Invoke the SDF API which returns the list
 *                 of entries present in the Require header.
 *               2.Iterate through the list and check for the presence
 *                 of 100 rel.If present and config doesn't support 
 *		   PRACK ,retun FAILURE
 *
 ***************************************************************************/
icf_return_t icf_ssa_parse_require_header(
        INOUT icf_ssa_pdb_st   *p_ssa_pdb,
        INOUT    SipMessage          *p_sip_msg,
	INOUT  Sdf_st_error		  *p_err);

/* ICF_6_1_OPTIONS_SUPPORT start */


/*****************************************************************************
 ** FUNCTION:           icf_ssa_handle_successful_incoming_options_req 
 **
 * DESCRIPTION:         This function will send an internal
 *                                      Event to SIC module.
 *
 ****************************************************************************/
icf_return_t icf_ssa_handle_successful_incoming_options_req(
        icf_ssa_pdb_st *p_ssa_pdb,
        Sdf_st_callObject **ppCallObj,
        Sdf_st_overlapTransInfo *pOverlapTransInfo);

/* ICF_6_1_OPTIONS_SUPPORT end */
icf_uint8_t icf_ssa_get_transport_mode(Sdf_st_callObject *pCallObj);
#ifdef ICF_DNS_LOOKUP_ENABLED
Sdf_ty_retVal icf_ssa_cbk_FillDestInfoFromUnknownDnsRec (
		Sdf_st_callObject *pCallObj,
		Sdf_st_overlapTransInfo *pOverlapTransaction,
		Sdf_ty_slist *pslDnsRecords,
		Sdf_st_transportInfo *pDestInfo,
		Sdf_st_error            *pErr);	

Sdf_ty_retVal icf_ssa_FreeUnknownDnsList (
		Sdf_st_DnsRecord        *pDnsRecord);
#endif


#ifdef ICF_LOOPBACK_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_util_map_addresses
 *
 * DESCRIPTION:     This function is used by SSA for mapping local and remote
 *                  address with the address received in message.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_util_map_addresses(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT  Sdf_st_callObject       *p_call_obj,
        INOUT  SipHeader      *pFrom,
        INOUT  SipHeader      *pTo,
        OUT   icf_ssa_ctxt_st    *p_mapped_ssa_ctxt,
        OUT   Sdf_st_error            *p_sdf_error,
        OUT   icf_error_t    *p_ecode);
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_util_cmp_address
 *
 * DESCRIPTION:     This function is used by SSA for comparing ny two address
 *                  specs.
 *
 ******************************************************************************/
extern icf_return_t icf_ssa_util_cmp_address(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT  SipHeader      *pAddr1,
        INOUT  SipHeader      *pAddr2,
        OUT   icf_error_t    *p_ecode);


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_validate_expires
 ** DESCRIPTION:  This function validates the Expires header value in an 
 **               incoming request.
 **
 ******************************************************************************/
icf_return_t icf_ssa_validate_expires(
        INOUT  icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT  Sdf_st_callObject         *p_call_obj,
        OUT   Sdf_ty_messageValidity    *p_validity,
        OUT   Sdf_st_error              *pErr);

/*****************************************************************************
 ** FUNCTION:    icf_ssa_tcpm_01_close_ind
 **
 ** DESCRIPTION: Close connection from peer, we trigger UATK to 
 **  reset the fds in all Socket structures fo the call object
 *****************************************************************************/
icf_return_t icf_ssa_tcpm_01_close_ind(
        icf_ssa_pdb_st   *p_ssa_pdb);


/***************************************************************************
**                                      
** FUNCTION: icf_ssa_fetch_curr_node_from_list  
** DESCRIPTION: This function fetch the current node from the list of 
**              sdp streams based on the stream id.
**      
***************************************************************************/
icf_return_t icf_ssa_fetch_curr_node_from_list
(
 INOUT     icf_ssa_pdb_st                      *p_ssa_pdb,
 INOUT     icf_ssa_inc_media_stream_info_st    *p_inc_media_info,
 INOUT     icf_stream_id_t                     stream_id,
 OUT    icf_inc_media_sdp_info_st           **p_p_media,
 OUT    Sdf_st_error                        *p_error);

/*****************************************************************************
 ** FUNCTION:           icf_ssa_set_to_in_sic_callobj
 **
 ** DESCRIPTION:        This  routine's purpose is to set the TO header
 **                     for outgoing call.For that, we need to get the 
 **                     remote party uri, scheme-sip/tel, remote's port
 **                     number, remote party display name.
 **                     Sets the proxy, if it is to be used, in the PreLoaded
 **                         Route set of the profile
*****************************************************************************/

icf_return_t icf_ssa_set_to_in_sic_callobj(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb,
                INOUT      icf_uint8_t             *p_proto);

/*******************************************************************************
 * FUNCTION:        icf_ssa_make_contact_of_app_data
 *
 * DESCRIPTION:     This function creates a contact header using the app_data
 *
 * RETURNS            SUCCESS: Contact header created successfully
 *                    FAILURE:
 *
 ******************************************************************************/
icf_return_t     icf_ssa_make_contact_of_app_data(
                        INOUT        icf_ssa_pdb_st       *p_ssa_pdb,
                        INOUT           icf_rgm_app_ctxt_st     *p_app_data,
                        INOUT           SipHeader               **p_p_contact_hdr,
                        OUT                     icf_error_t                 *p_ecode);

/************************************************************************
** FUNCTION: icf_ssa_set_unknown_hdr_in_MimeHdr
**
** DESCRIPTION: This function populates the unknown header in the SIP 
**              message body. These headers will be provided by the 
**              application. This function will invoke the SIP APIs 
**              directly to set the header.
**
************************************************************************/

icf_return_t icf_ssa_set_unknown_hdr_in_MimeHdr(
                INOUT    icf_header_st     *p_msg_hdr,
                INOUT  SipMimeHeader     *pMimeHdr
                );

/*****************************************************************************
 * FUNCTION:           icf_ssa_update_contact_in_profile
 *
 * DESCRIPTION:        This  routine:
 *                     1. Adds the contact header into the profile of call 
 *                     object
 *****************************************************************************/
icf_return_t icf_ssa_update_contact_in_profile(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb);



/***************************************************************************
 *
 * FUNCTION:     icf_ssa_populate_contact_addr
 * DESCRIPTION:  1.This functions populates the icf_address_st
 *                 on the basis of the Contact header (if any)
 *                 present in the SIP message passed to it
 *
 ***************************************************************************/
icf_return_t icf_ssa_populate_contact_addr(
        INOUT  icf_ssa_pdb_st *p_ssa_pdb,
        INOUT    SipMessage        *p_sip_msg,
        INOUT  icf_address_st **p_p_contact_addr,
        INOUT  Sdf_st_error      *p_err);

/*****************************************************************************
 * FUNCTION:        icf_ssa_check_for_reg_subs_trans_param
 *
 * DESCRIPTION:     Function to derive transport mode based on proxy mode
 *                  and mode received in remote party address in create call
 *
 *****************************************************************************/
icf_return_t icf_ssa_check_for_reg_subs_trans_param(
        icf_ssa_pdb_st        *p_ssa_pdb,
        icf_transport_mode_et reg_mode,
        icf_transport_mode_et self_mode);


#ifdef ICF_NAT_RPORT_SUPPORT

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_get_params_from_Via
 ** DESCRIPTION:  This function extracts the Via header parameters from the Sip
 **               Message and set the various flags for rport status as received 
 **               request/response.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_get_params_from_Via(
       INOUT     SipMessage          *p_sip_msg,
       INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
       IN     en_SipMessageType   dMsgType,
       OUT    SipError            *sip_error
       );


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_compare_transport_addr
 ** DESCRIPTION:  This function compares the contact address and 
 **               the address received in the Via Hdrs Parameters.
 **
 ******************************************************************************/
icf_boolean_t icf_ssa_compare_transport_addr(
         INOUT icf_ssa_pdb_st              *p_ssa_pdb,
         INOUT icf_transport_address_st    *p_contact_addr,
         INOUT icf_transport_address_st    *p_received_addr
         );
/*******************************************************************************
 * FUNCTION:        icf_ssa_start_binding_refresh_timer
 * DESCRIPTION:    This routine is used for starting the binding refresh timer
 ******************************************************************************/
icf_return_t icf_ssa_start_binding_refresh_timer(
        INOUT icf_ssa_pdb_st*    p_ssa_pdb);

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_handle_binding_refresh_timer_expiry
 ** DESCRIPTION:  This function handles the expiry of binding refresh timer.
 ** 
 ******************************************************************************/
icf_return_t icf_ssa_handle_binding_refresh_timer_expiry(
       INOUT icf_ssa_pdb_st            *p_ssa_pdb
       );
/*******************************************************************************
 * FUNCTION:        icf_ssa_handle_register_response_for_rport
 * DESCRIPTION:     This routine is used for checking the rport and received 
 *                  parameters in the register response. If the values are 
 *                  different from the contact address then set the binding 
 *                  change boolean flag in the global config data and set the
 *                  rport status bitmask to initial-pending.
 * RETURN Types:    ICF_SUCCESS/ICF_FAILURE                  
 ******************************************************************************/
icf_return_t icf_ssa_handle_register_response_for_rport(
        INOUT icf_ssa_pdb_st        *p_ssa_pdb
        );

#endif
/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_del_params_in_Via
 ** DESCRIPTION:  This function extracts the Via header parameters from the Sip
 **               Message and set the various flags for rport status as received 
 **               request/response.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_del_params_in_Via(
       INOUT     SipMessage          *p_sip_msg,
       INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
       OUT    SipError            *sip_error
       );


/*******************************************************************************
 ** FUNCTION:    icf_ssa_process_info_cfm 
 ** DESCRIPTION:  This function process info_cfm or info_cfm err received from 
 **               CC and send response accordingly on n/w.
 **
 ******************************************************************************/
icf_return_t icf_ssa_process_info_cfm(icf_ssa_pdb_st *p_ssa_pdb);

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_is_contact_present_in_hdr_list
 ** DESCRIPTION:  This function checks if Contact header present in header_list
 **               and if present then return its header value
 **
 ******************************************************************************/
icf_boolean_t  icf_ssa_is_contact_present_in_hdr_list(
                    icf_ssa_pdb_st   *p_ssa_pdb,
                    icf_uint8_t      **p_p_str,
                    icf_uint8_t      *p_disp_name,
                    icf_uint8_t      *p_tpt_scheme);

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_parse_contact_in_hdr_list
 ** DESCRIPTION:  This function parses contact header value string and fetches
 **               display name and contact address from it
 **
 ******************************************************************************/
icf_return_t  icf_ssa_parse_contact_in_hdr_list(
                      icf_ssa_pdb_st     *p_ssa_pdb, 
                      icf_uint8_t        *p_contact_str,
                      icf_uint8_t        *p_disp_name, 
                      icf_uint8_t        *p_cont_addr,
                      icf_uint8_t        *p_tpt_scheme);



/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_validate_content_disp
 ** DESCRIPTION:  This function validates the Content Disposition header value in an 
 **               incoming request.
 **
 ******************************************************************************/
icf_return_t icf_ssa_validate_content_disp(
        INOUT icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT Sdf_st_callObject         *p_call_obj,
        OUT   Sdf_st_error              *p_err);

#ifdef ICF_DNS_LOOKUP_ENABLED
/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_handle_dns_query_resp
 *
 * DESCRITION:      This function calls the ARES PROCESS for processing the
 *                  dns query response
 *
 ******************************************************************************/
icf_return_t icf_ssa_handle_dns_query_resp(
        INOUT icf_internal_msg_st    *p_internal_msg);
#endif

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_get_ip_from_fqdn
 ** DESCRIPTION:  This function will search for the corresponding IP address for 
 **               the given FQDN. If found p_active_ip_addr will contain the 
 **               current active IP address else failure will be returned.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_get_ip_from_fqdn(
       IN     icf_uint8_t        *p_fqdn,
       OUT    icf_dns_buffer_st  **p_dns_record,
       OUT    icf_error_t        *p_error
       );

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_add_fqdn_to_ip_mapping
 ** DESCRIPTION:  This function will populate the data structures, containing
 **               mapping of each FQDN to multiple IP addresses. SSA will invoke
 **               this function to provide list of IP addresses for the 
 **               corresponding FQDN to DNS buffer.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_add_fqdn_to_ip_mapping(
       IN     icf_dns_buffer_st     *p_query_dns_resp,
       INOUT     icf_ssa_ctxt_st          *p_ssa_ctxt,
       IN     Sdf_st_callObject       *p_call_obj,
       OUT    icf_error_t        *p_error
       );


/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_delete_fqdn_entry
 ** DESCRIPTION:  This function will receive the pointer of the node whose next 
 ** points to the record to be deleted. If the p_fqdn is NULL then the very first 
 ** node of the DNS list will be deleted.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_delete_fqdn_entry(
       IN     icf_string_st     	 *p_fqdn,
       OUT    icf_error_t        *p_error
       );

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_start_timer
 ** DESCRIPTION:  This function will be used to start dns timer.
 **
 ******************************************************************************/
icf_return_t icf_ssa_dns_start_timer(
	INOUT icf_glb_pdb_st *p_glb_pdb,
        IN  icf_duration_t   timer_dur,
        OUT  icf_error_t *p_ecode
        );

/*******************************************************************************
 **
 ** FUNCTION:     icf_ssa_dns_purge_timer_expire
 ** DESCRIPTION:  This is the call back function and will be registered by the DNS 
 ** buffer to the genlib timer manager as soon as there comes an entry in the DNS 
 ** buffer. This function will decrement the timer_count in each DNS record.
 **
 ******************************************************************************/
icf_return_t    icf_ssa_dns_purge_timer_expire(
		INOUT	icf_glb_pdb_st   *p_glb_pdb,	
		OUT    icf_error_t        *p_error 
		);

/*****************************************************************************
 * FUNCTION:        icf_ssa_dns_stop_timer
 *
 * DESCRIPTION:     This function will stop the dns timer.
 * 
 *****************************************************************************/
icf_return_t icf_ssa_dns_stop_timer(
	INOUT	icf_glb_pdb_st  *p_glb_pdb,
        OUT     icf_error_t    *p_ecode); 

icf_return_t  icf_ssa_get_line_id_from_glb_pdb( INOUT   icf_glb_pdb_st   *p_glb_pdb,
                                                INOUT icf_ssa_ctxt_st       *p_ssa_ctxt,
                                                OUT     icf_line_id_t   * p_line_id,
                                                OUT     icf_error_t      *p_ecode );

icf_uint16_t
icf_ssa_dns_2_pow(icf_uint8_t  pow_of_2_for_timer);

#ifdef ICF_SESSION_TIMER
icf_return_t   icf_ssa_session_timer_get(icf_ssa_pdb_st *p_ssa_pdb);

icf_return_t   icf_ssa_session_timer_set(icf_ssa_pdb_st *p_ssa_pdb);

icf_return_t   icf_ssa_session_timer_get_update(icf_ssa_pdb_st *p_ssa_pdb,
                                                Sdf_st_overlapTransInfo *pOverlapTransInfo);

#endif

icf_return_t    icf_ssa_process_detach_call_ctx_frm_sip_ctx(
       icf_ssa_pdb_st     *p_ssa_pdb);

/*******************************************************************************
 * FUNCTION:        icf_ssa_process_options_abort_req
 * DESCRIPTION:     This function aborts the OPTIONS transaction
 *                  1.Invoke the utility function which aborts the OPTIONS trans.
 * 
 ******************************************************************************/
icf_return_t icf_ssa_process_options_abort_req(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb);

/*****************************************************************************
 * FUNCTION:        icf_ssa_is_update_present_in_allow_hdr
 *
 * DESCRIPTION:     This function checks that whether update
 *                  method is present in the ALLOW header of 
 *                  a received SIP message.
 *
 *****************************************************************************/
Sdf_ty_retVal icf_ssa_is_update_present_in_allow_hdr(SipMessage *p_message,
                                                     icf_glb_pdb_st *p_glb_pdb,
                                                     icf_ssa_ctxt_st **p_ssa_ctxt,
                                                     Sdf_st_error *pErr);

/******************************************************************
** FUNCTION: icf_ssa_get_reason_string_from_sip_message
**
** DESCRIPTION:  This function retrives the status code from the
** Sip message
**
******************************************************************/
icf_return_t icf_ssa_get_reason_string_from_sip_message
                                            (SipMessage *pMsg,
                                             SIP_S8bit  **p_reason);


icf_return_t icf_ssa_append_to_ssa_ctxt_list(icf_ssa_ctxt_list_st *p_source,
        icf_ssa_ctxt_list_st * p_dest);

icf_return_t icf_ssa_remove_ctxt_list_entry(icf_glb_pdb_st *p_glb_pdb,
                   icf_ssa_ctxt_st *p_ssa_ctxt);

/******************************************************************
** FUNCTION: icf_ssa_util_cmp_tags
**
** DESCRIPTION: Compares the to and from tags to identiy a matching 
**              transaction 
**
******************************************************************/
icf_return_t icf_ssa_cmp_replaces_tags(
    icf_ssa_pdb_st *p_ssa_pdb,
    Sdf_st_callObject *pNewCallObject, 
    icf_uint8_t       *p_from_tag,
    icf_uint8_t       *p_to_tag,
    Sdf_st_error *p_sdf_error);


/*******************************************************************************
 ** FUNCTION:    icf_ssa_process_detach_call_ctx_frm_sip_ctx
 ** DESCRIPTION:  This function set NULL to ssa_ctxt->p_call_ctx
 **
 ******************************************************************************/
icf_return_t    icf_ssa_process_detach_call_ctx_frm_sip_ctx(
       icf_ssa_pdb_st     *p_ssa_pdb);

#ifdef ICF_DNS_LOOKUP_ENABLED
/***************************************************************************
 ** FUNCTION:           icf_ssa_dns_start_dns_resp_timer
 **
 ** DESCRIPTION:        This function will be used to start
 **                     the dns resp timer i.e. this timer will be started 
 **                     for the duration upto which we will wait for DNS resp
 **                     from DNS server. 
 ***************************************************************************/
icf_return_t icf_ssa_dns_start_dns_resp_timer(
		INOUT icf_ssa_ctxt_st *p_ssa_ctxt,
                IN  Sdf_st_dnsEventContext *p_dns_evt_ctxt,
                IN  icf_void_t *p_ares_ctxt,
		INOUT  icf_duration_t   timer_dur,
		OUT icf_error_t      *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_dns_stop_dns_resp_timer
 *
 * DESCRIPTION:     This function will be used to stop dns response timer.
 *
 ******************************************************************************/
icf_return_t icf_ssa_dns_stop_dns_resp_timer(
	INOUT	icf_ssa_ctxt_st *p_ssa_ctxt,
        OUT     icf_error_t    *p_ecode);

#endif

#ifdef ICF_LOOPBACK_SUPPORT
/******************************************************************
** FUNCTION: icf_ssa_util_cmp_tags
**
** DESCRIPTION: Compares the to and from tags to identiy a matching 
**              transaction 
**
******************************************************************/
icf_return_t icf_ssa_util_cmp_tags(
    icf_ssa_pdb_st *p_ssa_pdb,
    Sdf_st_callObject *pNewCallObject, 
    Sdf_st_callObject *pExistingCallObject, 
    Sdf_st_error *p_sdf_error);
#endif

/*****************************************************************************
 * FUNCTION:       icf_ssa_validate_challenge
 *
 * DESCRIPTION:     This function checks if the stale parameter is true in
 *the authentication header if yes then it returns the success else it returns
 *the failure
 *****************************************************************************/

 icf_return_t icf_ssa_validate_challenge(
	Sdf_st_callObject               **ppCallObj,
    Sdf_st_overlapTransInfo          *pOverlapTransInfo,
    Sdf_ty_u16bit                   resp_code,
    icf_ssa_pdb_st                  *p_ssa_pdb,
    Sdf_st_error                    *pErr,
    icf_uint8_t                     *p_method);         

/************************************************************************
 *
 * FUNCTION NAME: icf_ssa_extract_nonce_from_challenge
 *
 * DESCRIPTION: The function extracts the nonce value from the challenge
 *              from the auth_params_list and stores it in the ssa_ctxt.
 *              
 * RETURNS: icf_return_t 
 *
************************************************************************/  
icf_return_t icf_ssa_extract_nonce_from_challenge(
             icf_ssa_pdb_st                    *p_ssa_pdb,
             Sdf_ty_u16bit                     resp_code);


/******************************************************************
** FUNCTION: icf_invite_resp_stats
**
** DESCRIPTION:  This function updates the number of responses 
**               received for incoming/outgoing INVITE in appropriate
**               field.
**
******************************************************************/


icf_void_t icf_ssa_invite_resp_stats(INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
                               IN  icf_uint32_t    response,
                               IN  icf_boolean_t        direction);

/******************************************************************
** FUNCTION: icf_resp_stats
**
** DESCRIPTION:  This function updates the number of responses 
**               received for incoming/outgoing methods other than INVITE
**               in appropriate field.
**
******************************************************************/
icf_void_t icf_ssa_resp_stats(INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
                            IN  icf_uint8_t  	*p_method,
                            IN  icf_uint32_t         response_code,
                            IN  icf_boolean_t        direction);
extern icf_void_t icf_ssa_stat_dns_type(IN   icf_uint32_t      dns_query_type);

/***********************************************************************************
* FUNCTION    :  icf_ssa_validate_join_header_and_get_call_id
*
* IN          :  icf_ssa_pdb  *p_ssa_pdb
*                icf_uint8_t  *join_call_id
*                icf_uint8_t  orig_call_id
*
* OUTPUT      :  ICF_SUCCESS/ICF_FAILURE
*
* DESCRIPTION :  This function validates whether or not join header can be applied
*                or not on this call.If any error is found, error is returned else,
*                internal call id to be joined is returned
************************************************************************************/
icf_return_t  icf_ssa_validate_join_header_and_get_call_id(
                    icf_ssa_pdb_st  *p_ssa_pdb,  icf_call_id_t  *join_call_id,
                    icf_call_id_t orig_call_id);



icf_uint16_t  icf_ssa_get_all_ssa_ctxt(icf_ssa_pdb_st  *p_ssa_pdb,
        icf_ssa_ctxt_st ***p_ssa_ctxt);

icf_return_t  icf_ssa_fill_conn_info_from_Via(icf_ssa_pdb_st *p_ssa_pdb,
         icf_ssa_tcpm_conn_data_st *conn);
en_SipMessageType icf_ssa_get_msg_type(icf_ssa_pdb_st *,Sdf_st_callObject  *p_call_obj);

icf_return_t  icf_ssa_remove_conn_entry ( icf_ssa_pdb_st * ,
        icf_uint32_t fd);

icf_ssa_tcpm_conn_data_st *  icf_ssa_add_conn_entry (
        icf_ssa_tcpm_conn_data_st *table );

icf_ssa_tcpm_conn_data_st * icf_ssa_get_conn_db_by_ip (icf_ssa_pdb_st * p_ssa_pdb,
                              icf_ssa_tcpm_conn_data_st * table);

icf_ssa_tcpm_conn_data_st * icf_ssa_get_conn_db (icf_uint32_t fd ,
                                     icf_ssa_tcpm_conn_data_st * table);

icf_void_t      icf_ssa_init_conn_db (icf_ssa_tcpm_conn_data_st *p_conn);
icf_return_t  icf_ssa_check_isfocus_in_contact_header(icf_ssa_pdb_st    *p_ssa_pdb);

icf_return_t icf_ssa_is_alias_present_in_Via(SipMessage *);
icf_void_t  icf_ssa_free_list(icf_ssa_pdb_st *, icf_list_st *);
icf_void_t      icf_ssa_print_conn_db (const icf_ssa_tcpm_conn_data_st *p_conn);
icf_void_t  icf_ssa_print_all_connection(icf_ssa_pdb_st *p_ssa_pdb);
icf_return_t    icf_ssa_set_param_in_Via(
                 INOUT     SipMessage          *p_sip_msg,
                 INOUT  icf_ssa_pdb_st      *p_ssa_pdb,
                 INOUT     SipParam            *p_param,
                 OUT    SipError            sip_error
                 );

icf_return_t icf_ssa_append_to_ssa_ctxt_list(icf_ssa_ctxt_list_st *p_source,
        icf_ssa_ctxt_list_st * p_dest);

icf_return_t icf_ssa_remove_ctxt_list_entry(icf_glb_pdb_st *p_glb_pdb,
                   icf_ssa_ctxt_st *p_ssa_ctxt);

/******************************************************************
** FUNCTION: icf_ssa_util_cmp_tags
**
** DESCRIPTION: Compares the to and from tags to identiy a matching 
**              transaction 
**
******************************************************************/
icf_return_t icf_ssa_cmp_replaces_tags(
    icf_ssa_pdb_st *p_ssa_pdb,
    Sdf_st_callObject *pNewCallObject, 
    icf_uint8_t       *p_from_tag,
    icf_uint8_t       *p_to_tag,
    Sdf_st_error *p_sdf_error);

icf_return_t icf_ssa_util_cmp_tags(
    icf_ssa_pdb_st *p_ssa_pdb,
    Sdf_st_callObject *pNewCallObject, 
    Sdf_st_callObject *pExistingCallObject, 
    Sdf_st_error *p_sdf_error);

/*****************************************************************************
* Function    : icf_ssa_delete_param_from_addr_spec
*
* Description : This function is a utility function which deletes any parameter
*               from Sip Address Specification.It takes the parameter to be
*               deleted as an argument.
*
******************************************************************************/

icf_return_t icf_ssa_delete_param_from_addr_spec(
    SipAddrSpec *p_addr_spec,
    icf_uint8_t *param);


/*****************************************************************************
 * FUNCTION:        icf_ssa_parse_p_early_media_hdr
 *
 * DESCRIPTION:     This function will parse the Header list present in the  
 *                  sip message for header P-Early-Media.It then parses that 
 *                  header and populates the corresponding value in the call ctxt.
 *****************************************************************************/
icf_boolean_t icf_ssa_parse_p_early_media_hdr(
        icf_ssa_pdb_st   *p_ssa_pdb,
        SipMessage       *p_sip_msg,
        icf_internal_sdp_stream_st *p_int_sdp);

/*****************************************************************************
 * FUNCTION: 	    icf_ssa_form_and_insert_p_early_media_hdr 
 *
 * DESCRIPTION:     This function forms and inserts an P-Early-Media
 *                  header into the SIP message passed  to it
 * 
 *****************************************************************************/
icf_return_t icf_ssa_form_and_insert_p_early_media_hdr(
         icf_ssa_pdb_st  *p_ssa_pdb,
         SipMessage 	    *p_sip_msg);
         
/*****************************************************************************
 * FUNCTION: 		icf_ssa_form_and_insert_warning_hdr
 *
 * DESCRIPTION:    	This function forms and inserts a warning 
 *                  header into the SIP message passed  to it.
 *****************************************************************************/
icf_return_t icf_ssa_form_and_insert_warning_hdr (
         icf_ssa_pdb_st  *p_ssa_pdb,
         SipMessage 	 *p_sip_msg);         

/* SPR 16019 */
/*******************************************************************************
 * FUNCTION:        icf_ssa_process_register_abort_req
 * DESCRIPTION:     This function aborts the REGISTER transaction
 *                  1.Invoke the utility function which aborts the REGISTER trans.
 * 
 ******************************************************************************/
icf_return_t icf_ssa_process_register_abort_req(
        INOUT   icf_ssa_pdb_st       *p_ssa_pdb);

/*****************************************************************************
 * FUNCTION:     icf_ssa_remove_port_from_address
 *
 * DESCRIPTION:  The function checks if the address contains the port in
 *               end then it removes. 
 *                  
 *****************************************************************************/
icf_return_t icf_ssa_remove_port_from_address(
               icf_ssa_pdb_st   *p_ssa_pdb,
               icf_uint8_t      *p_req_uri);


/*****************************************************************************
 * FUNCTION:        icf_ssa_parse_AcceptHdr_for_supported_Msgbody
 *
 * DESCRIPTION:     This function parse the Accept header of incoming
 *                  SIP message and determines whether the msg body
 *                  present in Accept header is supported by application
 *                  or not.
 *
 *****************************************************************************/
Sdf_ty_retVal icf_ssa_parse_AcceptHdr_for_supported_Msgbody(SipMessage *p_message,
                                                     icf_glb_pdb_st *p_glb_pdb,
                                                     Sdf_st_error *pErr);

#ifdef ICF_SESSION_TIMER
/*****************************************************************************
 * FUNCTION: 	 icf_ssa_chk_n_insert_timer_in_supp_hdr
 *
 * DESCRIPTION:  The function checks if the negotiated rerfesher is none for 
 *               incoming INVITE/UPDATE requests (that is the refresher
 *               received from network and the one set by application is
 *               none). In such cases no session timer needs to be started and
 *               hence no session timer related (expiry, refresh method etc) 
 *               information is provided to UATK. UATK if it doesn't
 *               have session timer related information does not send timer in 
 *               supported header so this would be inserted here.
 *                  
 *****************************************************************************/
icf_return_t icf_ssa_chk_n_insert_timer_in_supp_hdr(icf_ssa_pdb_st *p_ssa_pdb);
#endif
/*Fix for CSR 1-6792719 referred from SPR : 18432*/
/*****************************************************************************
 * FUNCTION:        icf_ssa_free_sip_txn_data_on_failure
 *
 * DESCRIPTION:     This function Iterate thru all TxnKeys in UATK
 *                  pGlbToolkitData and compare if it is the given key
 *                  to be freed then free both transaction key and buffer
 *
 *****************************************************************************/
icf_void_t     icf_ssa_free_sip_txn_data_on_failure(
        SipTxnKey *pSipTxnKey);

/*********************************************************
** FUNCTION: icf_ssa_handle_chal_resp
**
** DESCRIPTION: This Function handles Challange response for
**              the request origionated in call 
**              
**
**********************************************************/
Sdf_ty_retVal icf_ssa_handle_chal_resp(
        Sdf_st_callObject               **ppCallObj,
        Sdf_st_eventContext             *pEventContext, 
        Sdf_st_overlapTransInfo         *pOverlapTransInfo,
        Sdf_ty_u16bit                    dRespCode, 
        Sdf_st_error                    *pErr);

/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_clear_div_info
 *
 * DESCRIPTION:     This function would clear the diversion information from 
 *                  call context
 *
 ******************************************************************************/
icf_return_t icf_ssa_clear_div_info(
        INOUT icf_ssa_pdb_st *p_ssa_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_insert_hdr_in_persistent_list_in_call_obj
 *
 * DESCRIPTION:     The function inserts an the header into persistent
 *                  header list in call objects with the passed correlation
 *
 ******************************************************************************/
icf_return_t icf_ssa_insert_hdr_in_persistent_list_in_call_obj(
                Sdf_st_callObject       *pCallObject,
                SipHeader               *pHeader,
                en_HeaderType           dType,
                Sdf_ty_s8bit            *pCorrelation,
                Sdf_st_error            *pErr);



#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*  __ICF_SSA_PROTOTYPES_H__ */
