/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_prototypes.h
 *
 * Purpose          : This file contains the prototypes of all functions
 *                    used by CC.
 *
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 14-Dec-2004  Pankaj Negi     Non Service LLD     Initial
 * 20-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for utility functions
 * 21-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for MMI request functions
 * 22-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for ecode function
 * 22-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for utility functions
 * 23-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for utility functions
 * 24-Dec-2004  Rohit Aggarwal                      Added function prototypes
 *                                                  for ADM, SSA functions
 * 28-Dec-2004  Rohit Aggarwal                      Added trace compilation flag
 * 04-Mar-2005  Sumit Gupta                         Added prototype for utility
 *                                                  function
 * 05-Apr-2005  Sumit Gupta      SPR 7354           Added new function prototype
 *                                                  for the function to clear up
 *                                                  diversion info
 * 01-Jun-2005  Jalaj Negi       IMS CLIENT         Added prototypes for IMS
 *                                                  CLIENT
 * 10-Aug-2005	Rohit Aggarwal	 ICF				Added new functions
 * 07-Mar-2006	Anuradha Gupta	 None				Added new functions for incoming
 *													Re-invite handling
 * 08-Mar-2006  Aman Aggarwal						ICF merging with ICF2.0
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0             Call Modify Feature changes
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs        Structure changes for multiple
 *                                                  m-lines
 * 03-Jan-2007  Amit Mahajan                        INFO confirmation changes.
 * 04-Jan-2007   Priyanka Dua    ICF 6.3            Code changes for Forking and 
 *												    Multiple answer
 * 26-Mar-2007  Deepti Goyal     ICF 6.4            Changes for Media connect
 *                                                  request API.
 * 09-Jul-2007  Neha Choraria    ICF 7.0            Changes for Multiple Answer &
 *                                                  Forking
 * 10-Jul-2007  Neha Choraria    ICF 7.0            Changes for Re-Invite w/o
 *                                                  SDP
 * 11-Jul-2007  Neha Choraria    ICF 7.0            Changes for Retry-After
 * 10-Jul-2007  Abhishek Dutta  ICF 7.0             Changes for Cancel call
 *                                                  modify request API.
 * 16-Oct-2009  Anurag Khare     SPR 20190          Fix for CSR 1-7894024                                                 
 *
 * Copyright (c) 2009, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_CC_PROTOTYPES_H__
#define __ICF_CC_PROTOTYPES_H__

#include "icf_cc_types.h"


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


/* -------------------------------------------------------------------------- */
/* **** FSM handlers **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_init
 *
 * DESCRIPTION:     This function creates memory block for storing global data.
 *
 ******************************************************************************/
icf_return_t icf_cc_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_cc_data,
        OUT     icf_error_t           *p_ecode);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of CC that is invoked by
 *                  Call Manager. The function checks the API Id in the message
 *                  received and invokes appropriate handler based on current
 *                  call state and the event (mapped from API Id).
 *
 ******************************************************************************/
icf_return_t icf_cc_process_mesg(
        INOUT icf_internal_msg_st *p_internal_msg);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_convert_api_to_event
 *
 * DESCRIPTION:     This function maps internal APIs to CC-specific events.
 *
 ******************************************************************************/
icf_return_t  icf_cc_convert_api_to_event(
        INOUT  icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_ignore_msg
 *
 * DESCRIPTION:     This function is invoked when an event needs to be ignored.
 *
 ******************************************************************************/
icf_return_t  icf_cc_ignore_msg(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_pa_setup_req
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in IDLE state and it receives a setup
 *                  request from PA
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_PA_SETUP_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_00_pa_setup_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_ssa_setup_req
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in IDLE state and it receives a setup
 *                  request from SSA
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_SSA_SETUP_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_00_ssa_setup_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is in IDLE state
 *                  and CM ask it to clear call context. Once CM is sure that
 *                  call has been cleared in CC and SC, it will invoke this
 *                  API to clear call context. This function will delete call
 *                  context and detach line id
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_00_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_00_clear_call
 *
 * DESCRIPTION:     This function is invoked when the CC is in IDLE state
 *                  and CM want to clear call due to system error
 *                  Current State: ICF_CC_00_IDLE
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_00_clear_call(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_create_media_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  create media session resp from MMI.
 *                  In the case of an outgoing call,setup request
 *                  has to be sent to the SSA and in the case of an
 *                  incoming call,an incoming call indication has to
 *                  be sent to PA
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_CMS_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_01_create_media_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  Call Terminate Request from PA.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_01_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Mute Request from PA.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_01_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call UnMute Request from PA.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_01_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_01_mm_err_ind (
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  the setup timer expires.
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_01_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_01_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_01_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives
 *                  setup resp from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives alert without
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_alrt_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives alert with
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives connect
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/ icf_return_t icf_cc_02_ssa_call_connect(
         INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives Call
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_02_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives a Call
 *                  Mute Request from PA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_02_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and it receives a Call
 *                  Unmute Request from PA.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_02_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and MM error indication
 *                  is received from MMI
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_02_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from the SSA and Setup Timer expires.
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_02_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_02_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives alert without
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_alrt_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives alert with
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives connect
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and it receives Call
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_03_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and it receives Call
 *                  Mute Request from PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and it receives Call
 *                  Unmute Request from PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)and MM error indication
 *                  is received from MMI
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_03_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT(SIP 180) from the SSA or ALERT RESP from
 *                  the PA(incoming call)aand Setup Timer expires.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_03_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  ALERT RESP from the PA is received.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  call CONNECT is received from the PA.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_03_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_media_prack_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ALERT
 *                  Response from SSA and remote sends response for PRACK
 *                  of 183.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_03_media_prack_resp(icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_media_update_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ALERT
 *                  Response from SSA and remote sends response for UPDATE
 *                  for Qos processing.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_MEDIA_UPDATE_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_03_media_update_resp(icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  Media Connect Request API is received.
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives media 
 *					param (SIP 183) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_media_param(icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives alert with
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_alrt_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives alert with
 *                  media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives connect
 *                  with media (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_mms_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and it receives Response
 *                  for the same from MMI
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MMS_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_04_mms_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from SSA (SIP BYE received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a session
 *                  param change from SSA (SIP UPDATE/RE-INVITE).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Call
 *                  Mute Request from PA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Call
 *                  Unmute Request from PA.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Release Timer expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_04_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Release Timer expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_04_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Setup/Alert Timer
 *                  expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_ALERT_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_media_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  MMS RESP from the PA(incoming call) and
 *                  Media Connect Request API is received.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_media_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Setup/Alert Timer
 *                  expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_media_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Setup/Alert Timer
 *                  expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_04_media_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and PRACK for 180 is
 *                  received
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_04_alert_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Setup/Alert Timer
 *                  expires.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and remote sends Cancel
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_04_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


#ifdef ICF_QOS_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_media_prack_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Modify
 *                  Media Session Response from MMI and remote sends
 *                  response for PRACK of 183.
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_04_media_prack_resp(icf_cc_pdb_st *p_cc_pdb);
#endif

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and offer change
 *                  is recvd from the PA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_04_pa_call_modify_req(
		INOUT icf_cc_pdb_st *p_cc_pdb);
ICF_HANDLE_REINVITE_END

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the SSA and it receives media param
 *                  (SIP 183 ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_MEDIA_PARAM
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Connect
 *					from MMI and it receives alert without
 *                  media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_alert(icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_alert_with_media
 * 
 * DESCRIPTION:     This function is invoked when the CC is awaiting  
 *                  CONNECT from the SSA and it receives alert with media
 *                  (SIP 180 with SDP ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_alert_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the SSA and it receives connect
 *                  (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and it receives reroute call
 *                  Req from SSA (SIP 302 received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA(incoming call) or CONNECT
 *                  from the SSA(SIP 200 OK) and it receives Call
 *                  Terminate Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from PA and it receives a session param
 *                  change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_05_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and it receives Call
 *                  Mute Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and it receives Call
 *                  Unmute Request from PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and MM error indication
 *                  is received from MMI
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_05_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA and Setup Timer expires.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_05_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA(incoming call) and
 *                  call CONNECT is received from the PA.
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_05_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  CONNECT from the PA(incoming call) and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_05_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_05_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_05_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_06_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Mute Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives Call Unmute Request from PA.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and MM error indication is received from MMI
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_06_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_06_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#ifdef ICF_SESSION_TIMER
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_network_reject
 *
 * DESCRIPTION:     This function is invoked when
 *					session refresh request is rejected
 *
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_NW_INC_CALL_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_network_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_session_refresh_req
 *
 * DESCRIPTION:     This function is invoked when
 *					session timer had expired. This will set the signalling
 *					ongoing flag to CC Module.
 *
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SESSION_REFRESH_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_06_ssa_session_refresh_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_session_refresh_resp
 *
 * DESCRIPTION:     This function is invoked when
 *					session refresh request is accepted (200 Ok)
 *
 *                  ..
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SESSION_REFRESH_RESP
 *
 *****************************************************************************/

icf_return_t icf_cc_06_ssa_session_refresh_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a call modify request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
ICF_HANDLE_REINVITE_END

#ifdef ICF_FAX_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a fax request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a fax request from SSA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_ssa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a stop fax request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_STOP_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_pa_stop_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_06_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in CONNECTED state
 *                  and it receives a stop fax request from SSA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_SSA_STOP_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_06_ssa_stop_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_audio_stream_status
 *
 * DESCRIPTION:     This function checks if audio line is present
 *
 ******************************************************************************/
icf_return_t icf_cc_check_audio_stream_status(
                INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function processes the stop fax req received from remote.
 ******************************************************************************/
icf_return_t     icf_cc_process_ssa_stop_fax_req(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Setup Ack
 *                  from SSA (SIP 100 Trying received from peer).
 *                  In this case, it needs to send Call Cancel Request to SSA
 *                  and change previous state to Await Alert Response
 *                  if the previous state is Await Setup Response from SSA.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_dms_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Delete
 *                  Media Session Response from MMI.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_dms_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from SSA (SIP BYE received from peer).
 *                  In this case, it will send Call Terminate Response to SSA
 *                  and invoke network clear handler.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Remote Call
 *                  Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  In this case, it will acknowledge remote reject and invoke
 *                  network clear handler.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Request from PA.
 *                  In this case, simply mark release type as PA initiated.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives Call
 *                  Terminate Response from SSA (SIP 200 OK in response to BYE)
 *                  In this case, simply invoke network clear handler.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a session
 *                  param change from SSA (SIP UPDATE/RE-INVITE received from
 *                  peer).
 *                  In this case, simply send error response to SSA (SIP 500 to
 *                  be sent for UPDATE/REINVITE received from peer).
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Call
 *                  Mute Request from PA.
 *                  In this case, simply set CALL MUTE bit in media_state if
 *                  next state is Await CMS Response.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a Call
 *                  Unmute Request from PA.
 *                  In this case, simply reset CALL MUTE bit in media_state if
 *                  next state is Await CMS Response.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_07_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and Release Timer expires.
 *                  In this case, clear all interfaces.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_07_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_07_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_07_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives a connect
 *                  from SSA.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Delete
 *                  Media Session Response from MMI and it receives
 *                  In this case, it will acknowledge remote reject and and it
 *                  receives reroute call Req from SSA (SIP 302 received from
 *                  peer) invoke
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  a Setup Ack from SSA (SIP 100 Trying received from peer).
 *                  In this case, it needs to send Call Cancel Request to SSA
 *                  and change previous state to Await Alert Response. This is
 *                  same as ICF_CC_SSA_SETUP_RESP event in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_SETUP_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  Call Terminate Request from SSA (SIP BYE recd from peer).
 *                  In this case, it will send Call Terminate Response to SSA
 *                  and invoke network clear handler. This is same as
 *                  ICF_CC_SSA_CALL_TERMINATE_REQ event in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  Call Reject from SSA (SIP 3xx/4xx/5xx received from peer).
 *                  In this case, it will acknowledge remote reject and invoke
 *                  network clear handler. This is same as
 *                  ICF_CC_SSA_REMOTE_REJECT event received in state
 *                  ICF_CC_07_AWT_DMS_RESP.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  Call Terminate Request from PA.
 *                  In this case, there is a race between CC sending Call
 *                  Terminate Request to PA on getting DMS response and PA
 *                  sending Call Terminate Request to CC.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_08_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_pa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  Call Terminate response from PA.
 *                  In this case, simply set PA clear bit in call_clear_status
 *                  and invoke call context clear handler if not already done.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  Call Terminate response from SSA (SIP 200 OK in response
 *                  to BYE).
 *                  In this case, simply invoke network clear handler.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and it receives
 *                  session param change from SSA (SIP UPDATE/RE-INVITE recd).
 *                  In this case, send an error response back to SSA. This is
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_08_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_08_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call
 *                  terminate response from SSA or PA and Release timer expires.
 *                  In this case, clear all interfaces.
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_08_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_08_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_08_AWT_CALL_TERMINATE_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_08_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  In this case, send an error response back to SSA. This is
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_09_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives a Call Mute Request from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives a Call Unmute Request from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives an error indication from MMI.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_09_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and Alert Timer expires.
 *                  In this case, send DMS request and initiate call clear.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_09_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives a call connect from PA.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_09_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_media_param_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives that response from SSA
 *                  (SIP PRACK received in response to 183).
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_09_media_param_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a session progress sent
 *                  to peer and it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer). This is same as the event
 *                  ICF_CC_SSA_CALL_CANCEL_REQ received in
 *                  ICF_CC_01_AWT_CMS_RESP state.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_09_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_09_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_09_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_media_connect_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  Media Connect Request API is received.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_MEDIA_CONNECT_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_09_pa_media_connect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);



/******************************************************************************
 *
 * FUNCTION:        icf_cc_09_pa_alert_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  ALERT RESP from the PA(incoming call) and
 *                  Media Connect Request API is received.
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_PA_ALERT_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_09_pa_alert_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives Call Terminate Request from PA.
 *                  This is same as receiving the event
 *                  ICF_CC_PA_CALL_TERMINATE_REQ in
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives a session param change from SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_10_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives a Call Mute Request from PA.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives a Call Unmute Request from PA.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives an error indication from MMI.
 *                  This is same as the event ICF_CC_MM_ERR_IND received in
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_10_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and Alert Timer expires.
 *                  In this case, send DMS request and initiate call clear. This
 *                  is same as the event ICF_CC_TIMER_EXPIRY received in
 *                  ICF_CC_09_AWT_MEDIA_PARAM_PRACK state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_10_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives a call connect from PA.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 ******************************************************************************/
icf_return_t icf_cc_10_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives that response from SSA
 *                  (SIP PRACK received in response to 180).
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_ALERT_PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_10_alert_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_10_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  provisional response from SSA for a user alert sent
 *                  to peer and it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer). This is same as the event
 *                  ICF_CC_SSA_CALL_CANCEL_REQ received in
 *                  ICF_CC_01_AWT_CMS_RESP state.
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_10_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_10_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_10_AWT_ALERT_PRACK
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_10_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  Call Terminate Request from PA.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_11_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is waitiug for ACK
 *					of 200 Ok
 *                  and it receives Call Terminate Request from N/W.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_11_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  session param change from SSA (SIP UPDATE/RE-INVITE recd).
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_11_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  Call Mute Request from PA.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_11_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  Call Unmute Request from PA.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_11_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  an error indication from MMI.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_11_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and timer expires.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_11_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  the same (SIP ACK recd for 200 OK sent for incoming INVITE)
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_11_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting ack for
 *                  Connect sent to SSA for an incoming call and it receives
 *                  Call Cancel Request from SSA (SIP CANCEL recd from peer).
 *                  This is same as the event ICF_CC_SSA_CALL_CANCEL_REQ recd
 *                  in ICF_CC_01_AWT_CMS_RESP state.
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_11_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_11_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_11_AWT_SSA_CONNECT_ACK
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_11_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_start_call_proc_sig
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives the same.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_START_CALL_PROC_SIG
 *
 ******************************************************************************/
icf_return_t icf_cc_12_start_call_proc_sig(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives Call
 *                  Terminate Request from SSA (SIP BYE received from peer).
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives Call Terminate
 *                  Request from PA.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_term_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives Call Terminate
 *                  Response from PA.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives a session
 *                  param change from SSA (SIP UPDATE/RE-INVITE received from
 *                  peer).
 *                  In this case, send an error response back to SSA. This is
 *                  same as the event ICF_CC_SSA_OFFER_CHANGE in
 *                  ICF_CC_07_AWT_DMS_RESP state.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_12_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and Release timer expires.
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 ******************************************************************************/
icf_return_t icf_cc_12_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_12_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_12_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives a session
 *                  param change from PA

 *                  In this case, mark the event as pending and process it when
 *					ongoing signalling is over..
 *                  Current State: ICF_CC_12_WAIT_START_PROC_SIG
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_12_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
ICF_HANDLE_REINVITE_END

/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  alert without media (SIP 180 without SDP) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_alrt_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it
 *                  receives alert with media (SIP 180 with SDP) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_alrt_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  connect (SIP 200 OK ) from SSA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_start_proc_media
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  the same
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_START_CALL_PROC_MEDIA
 *
 *****************************************************************************/
icf_return_t icf_cc_13_start_proc_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it receives
 *                  call termination request from SSA(SIP BYE)
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_remote_reject
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and it
 *                  receives Remote Call Reject from SSA
 *                  (SIP 3xx/4xx/5xx received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_REMOTE_REJECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_remote_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_redirect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives reroute call Req from SSA
 *                  (SIP 302 received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_REDIRECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_redirect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives Call Terminate Request from PA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and offer change
 *                  is recvd from the SSA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_13_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_call_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives Call Mute Request from PA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_CALL_MUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_call_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_call_unmute_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and it receives Call Unmute Request from PA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_CALL_UNMUTE_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_call_unmute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and MM error indication is received from MMI
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_13_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure
 *                  and Timer expires.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_13_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  ALERT RESP from the PA is received.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  call CONNECT is received from the PA.
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_CALL_CONNECT
 *
 *****************************************************************************/
icf_return_t icf_cc_13_pa_call_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_media_param_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  provisional response from SSA for a session
 *                  progress sent to peer is received
 *                  (SIP PRACK received in response to 183).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_MEDIA_PARAM_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_13_media_param_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_alert_prack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  PRACK for 180 is received
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_ALERT_PRACK
 *
 *****************************************************************************/
icf_return_t icf_cc_13_alert_prack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  ACK for 200 OK of INVITE is recvd
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_13_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_13_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_13_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

ICF_HANDLE_REINVITE_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_13_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  trigger to start its media procedure and offer change
 *                  is recvd from the PA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_13_WAIT_START_PROC_MEDIA
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_13_pa_call_modify_req(
		INOUT icf_cc_pdb_st *p_cc_pdb);
ICF_HANDLE_REINVITE_END


#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting an UPDATE 
 *                  request from remote and receives the same.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_16_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_16_mm_err_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting an UPDATE
 *                  request from remote and receives MM error indication from MMI
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 ******************************************************************************/
icf_return_t icf_cc_16_mm_err_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  an UPDATE request from the SSA and Setup Timer expires.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_16_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  an UPDATE request from SSA and it receives a Call Cancel 
 *					Request from SSA(SIP CANCEL received from peer).
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_16_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_generic_err
 *
 * DESCRIPTION:     This function is invoked when CC is awaiting an UPDATE request
 *                  from SSA and it receives an error indication from MMI.
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_16_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_16_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_16_AWT_UPDATE 
 *                  Current State: ICF_CC_16_AWT_UPDATE
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_16_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif


/* -------------------------------------------------------------------------- */
/* **** Utility Functions **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_process_dms_resp
 *
 * DESCRIPTION:     This is a utility function that is used in processing DMS
 *                  response. It handles the response when Next state is not
 *                  set to Await CMS response or Wait for Start Proc Signaling.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_process_dms_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_07_clear_interfaces
 *
 * DESCRIPTION:     This is a utility function that is used in processing DMS
 *                  response. It handles the response when Previous state is
 *                  not set to Await CMS response.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_07_clear_interfaces(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_09_call_term_handler
 *
 * DESCRIPTION:     This is a utility function that does common processing for
 *                  termination in states ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  and ICF_CC_10_AWT_ALERT_PRACK.
 *                  The function
 *                  a. Sends Ack to SSA for provisional response received
 *                  b. Sends Call Reject to SSA with appropriate release cause
 *                  c. Sends DMS Request to MMI
 *                  d. Resets MM error and Call Terminate Request pending events
 *                  e. Changes call state to Awaiting DMS Response
 *
 ******************************************************************************/
icf_return_t icf_cc_09_call_term_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_11_call_term_handler
 *
 * DESCRIPTION:     This is a utility function that does common processing for
 *                  call termination in state ICF_CC_11_AWT_SSA_CONNECT_ACK.
 *                  The function
 *                  a. Starts Release timer
 *                  b. Sends Call Terminate Request/Call Reject (due to Server
 *                     Internal Error) to SSA in the current state (whatever
 *                     stack allows)
 *                  c. Sends DMS Request to MMI
 *                  d. Changes call state to Awaiting DMS Response
 *
 ******************************************************************************/
icf_return_t icf_cc_11_call_term_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb);


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_state_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a state and
 *                  is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cc_state_to_str(
        IN icf_cc_fsm_state_t state_val,
        OUT icf_uint8_t *p_state_str);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_event_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to an event
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cc_event_to_str(
        IN icf_uint8_t event_val,
        OUT icf_uint8_t *p_event_str);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_media_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to media mode
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cc_media_to_str(
        IN icf_media_mode_t mode_val,
        OUT icf_uint8_t *p_mode_str);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_module_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a module
 *                  and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cc_module_to_str(
        IN icf_uint8_t module_id,
        OUT icf_uint8_t *p_module_str);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_timer_to_str
 *
 * DESCRIPTION:     This function returns a string corresponding to a timer
 *                  type and is used only for traces.
 *
 ******************************************************************************/
icf_void_t icf_cc_timer_to_str(
        IN icf_timer_type_t timer_type,
        OUT icf_uint8_t *p_timer_str);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_TRACE_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_trace
 *
 * DESCRIPTION:     This function is used by CC for trace handling.
 *
 ******************************************************************************/
icf_void_t icf_cc_trace(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN icf_trace_id_t trace_id);
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#ifdef ICF_ERROR_ENABLE
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_error
 *
 * DESCRIPTION:     This function is used by CC for error handling.
 *
 ******************************************************************************/
icf_void_t icf_cc_error(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN icf_error_t ecode);
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_current_timer
 *
 * DESCRIPTION:     This function stops any currently running timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_current_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_setup_timer
 *
 * DESCRIPTION:     This function starts Setup timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_setup_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_alert_timer
 *
 * DESCRIPTION:     This function stops Setup timer if it is currently running
 *                  and starts the Alert timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_alert_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_boolean_t is_pa_timer,
        IN    icf_duration_t pa_timer);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_media_timer
 *
 * DESCRIPTION:     This function stops any timer if it is currently running
 *                  and starts the Modify Media timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_media_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_release_timer
 *
 * DESCRIPTION:     This function stops Setup/Alert/ModifyMedia timer if it is
 *                  currently running and starts the Release timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_release_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_nw_clear_handler
 *
 * DESCRIPTION:     This function is invoked everytime the network clear bit is
 *                  set in call_context->call_clear_status.
 *
 ******************************************************************************/
icf_return_t icf_cc_nw_clear_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_clear_handler
 *
 * DESCRIPTION:     This function is invoked everytime the PA clear bit is
 *                  set in call_context->call_clear_status.
 *
 ******************************************************************************/
icf_return_t icf_cc_pa_clear_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_call_context
 *
 * DESCRIPTION:     This function does the call context clean up once the PA,
 *                  MM and NW interfaces are all cleared.
 *
 ******************************************************************************/
icf_return_t icf_cc_clear_call_context(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_clear_req_to_ssa
 *
 * DESCRIPTION:     This function is used to send a Call Terminate Request to
 *                  SSA. It sends CANCEL/BYE/4xx depending on the call type and
 *                  current call state.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_clear_req_to_ssa(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_init_pdb
 *
 * DESCRIPTION:     This is a utility function that initializes CC pdb with
 *                  default values.
 *
 ******************************************************************************/
icf_return_t icf_cc_init_pdb(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_init_call_ctx
 *
 * DESCRIPTION:     This is a utility function that initializes call context
 *                  with default values.
 *
 ******************************************************************************/
icf_return_t icf_cc_init_call_ctx(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_redirect_req
 *
 * DESCRIPTION:     This function validates the re-route request(SIP 302)
 *                  It validates the maximum hops and and checks
 *                  if the forwarded address is the same as the local address
 *
 ******************************************************************************/
icf_return_t icf_cc_validate_redirect_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_pending_events
 *
 * DESCRIPTION:     This function send error indication to remote
 *                  for update/invite if update is in pending list
 *                  or ongoing event
 *
 ******************************************************************************/
icf_return_t icf_cc_clear_pending_events(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_otg_clear_pend_evt_mms_rsp
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for outgoing call for ALERT_WITH_MEDIA and MEDIA_COMMIT.
 *                  We have got mms for call alert and media
 *                  commit so pending event can be Call mute or call
 *                  unmute or offer change
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_otg_clear_pend_evt_mms_rsp(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI 
 *					for ssa_media_param.
 *					Pending events can occur in case of forking/multiple_answer
 *					This function will clear pending events
 *                  a. If pending event is ssa media param, send MMS
 *                  b. If pending event is ssa alert with media, send MMS
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_ssa_media_param(INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI 
 *					for ssa_alert_with_media.
 *					Pending events can occur in case of forking/multiple_answer
 *					This function will clear pending events
 *                  a. If pending event is ssa media param, send MMS
 *                  b. If pending event is ssa alert with media, send MMS
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_ssa_alert_with_media(INOUT icf_cc_pdb_st *p_cc_pdb);
#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_pend_un_mute_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for mute or unmute request. This function will cleare
 *                  pending events
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_un_mute_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
#endif
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_update_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for mute or unmute request. this function will clear
 *                  pending events
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_update_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_alert_prack_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for ALERT_PRACK request. This function will
 *                  cleare pending events
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_alert_prack_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_media_prack_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for MEDIA_PRACK request. This function will clear
 *                   pending events
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_media_prack_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_ssa_call_con_ack
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for SSA_CONNECT/SSA_CONNECT_ACK request. This function will
 *                  clear pending events
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_ssa_call_con_ack(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_rem_media_ind
 *
 * DESCRIPTION:     This function is invoked when the CC need to remote hold ind
 *                  This function check the need to send Remote hold/resume ind
 *                  to PA and call corresponding function
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_rem_media_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_set_pa_init_call_rel_cause
 *
 * DESCRIPTION:     This function set the call clear reason given by PA
 *                  in call context in case it is not already set
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_set_pa_init_call_rel_cause(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_response_code_in_term_req
 *
 * DESCRIPTION:     This function checks for the presence and validation of
 *                  the response code that is given by PA in terminate call request.
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_validate_response_code_in_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_set_annonymous_local_addr
 * DESCRIPTION:     This function set the annonymous addr in local addr

 *
 *
 *****************************************************************************/
icf_return_t icf_cc_set_annonymous_local_addr(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_call_no_remote_sdp
 *
 * DESCRIPTION:     This function clears call in case remote connect didn't
 *                  contained sdp and we havent got any sdp till now
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_clear_call_no_remote_sdp(
        INOUT  icf_cc_pdb_st *p_cc_pdb);


/* -------------------------------------------------------------------------- */
/* **** CC-MMI interface Functions **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_cms_req
 *
 * DESCRIPTION:     This function is used to make a Create Media Session
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_cms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_mms_req
 *
 * DESCRIPTION:     This function is used to make a Modify Media Session
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_mms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_dms_req
 *
 * DESCRIPTION:     This function is used to make a Modify Media Session
 *                  request and send it to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_dms_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);



/* -------------------------------------------------------------------------- */
/* **** CC-SSA interface Functions **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_setup_req
 *
 * DESCRIPTION:     This function is used to send Call Setup Request to SSA.
 *                  SSA will create INVITE using media parameters present in
 *                  call context.
 *                  ICF_SSA_CALL_SETUP (SIP INVITE to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_setup_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_setup_resp
 *
 * DESCRIPTION:     This function is invoked by Call Control after INVITE has
 *                  been validated to send Call Setup response to SSA.
 *                  ICF_SSA_CALL_SETUP_ACK (SIP 100 Trying to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_setup_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert
 *
 * DESCRIPTION:     This function is invoked by Call Control to send alert
 *                  without media to remote party.
 *                  ICF_SSA_ALERT (SIP 180 without SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_media
 *
 * DESCRIPTION:     This function is invoked by Call Control to send alert
 *                  with media to remote party.
 *                  ICF_SSA_ALERT_WITH_MEDIA (SIP 180 with SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked by Call Control to send Session
 *                  Progress (with media) to remote party.
 *                  ICF_SSA_MEDIA_PARAM (SIP 183 to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_param_wo_sdp
 *
 * DESCRIPTION:     This function is invoked by Call Control to send Session
 *                  Progress (without media) to remote party.
 *                  ICF_SSA_MEDIA_PARAM_WO_SDP (SIP 183 without SDP to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_param_wo_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_prov_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send provisional/reliable
 *                  response to remote party.
 *                  ICF_SSA_PRACK (SIP PRACK to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prov_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prov_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to acknowledge
 *                  provisional/reliable response received from remote party.
 *                  ICF_SSA_PRACK_ACK (SIP ACK to be sent for PRACK)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prov_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_media_prov_resp_ack
 *
 * DESCRIPTION:     Invoked by CC toward SSA to acknowledge
 *                  provisional/reliable response received from remote party.
 *                  ICF_SSA_PRACK_ACK (SIP ACK to be sent for PRACK)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prov_resp_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prov_resp_ack
 *
 * DESCRIPTION:     Invoked by CC toward SSA to acknowledge
 *                  provisional/reliable response received from remote party.
 *                  ICF_SSA_PRACK_ACK (SIP ACK to be sent for PRACK)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prov_resp_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_offer_chg_req
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send media change to peer.
 *                  ICF_SSA_MEDIA_CHANGE (SIP UPDATE/REINVITE to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_offer_chg_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_connect
 *
 * DESCRIPTION:     Invoked by the call control when the call is accepted by
 *                  IP Packet Phone Signaling Toolkit user and results in
 *                  Connect being sent to SSA.The CC shall also indicate
 *                  whether the Media needs to be sent in this or not.
 *                  ICF_SSA_CONNECT (SIP 200 OK to be sent for INVITE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_connect_ack
 *
 * DESCRIPTION:     Invoked by the Call Control to acknowledge call connect
 *                  received from SSA.
 *                  ICF_SSA_CONNECT_ACK (SIP ACK for 200 OK of INVITE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_redirect_ack
 *
 * DESCRIPTION:     Invoked by the Call Control to acknowledge redirect
 *                  received from SSA.
 *                  ICF_SSA_REDIRECT_ACK (ACK for SIP 302 to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_redirect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_reject
 *
 * DESCRIPTION:     Invoked by the call control when the incoming call is
 *                  rejected/cleared by IP Packet Phone Signaling Toolkit user
 *                  and results in 3xx/4xx/5xx being sent to Remote Party. The
 *                  SSA shall check the release cause in call context to send
 *                  appropriate error response.
 *                  ICF_SSA_REJECT(SIP 3xx/4xx/5xx be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_reject(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_reject_ack
 *
 * DESCRIPTION:     Invoked by the call control to acknowledge final failure
 *                  response (remote reject) received from SSA.
 *                  ICF_SSA_REJECT_ACK(SIP ACK to be sent for 3xx/4xx/5xx)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_reject_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_cancel_req
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send CANCEL for outgoing call.
 *                  ICF_SSA_ABORT (SIP CANCEL to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_cancel_resp
 *
 * DESCRIPTION:     Invoked by CC toward SSA to send response for incoming call
 *                  rejection by peer.
 *                  ICF_SSA_ABORT_RESP (SIP 200 OK for CANCEL to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_cancel_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_call_term_req
 *
 * DESCRIPTION:     Should be invoked when SIP CC wants to release the call and
 *                  indicate the same to the peer.
 *                  ICF_SSA_CALL_TERMINATE (SIP BYE to be sent)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_call_term_rsp
 *
 * DESCRIPTION:     Invoked by the Call Control to indicate that release
 *                  procedure is completed on a given leg of the call.
 *                  ICF_SSA_CALL_TERMINATE_RESP (SIP 200 OK for BYE)
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_call_term_rsp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_offer_chg_rsp
 *
 * DESCRIPTION:     Invoked by the Call Control to send offer change resp
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_offer_chg_rsp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_offer_chg_err
 *
 * DESCRIPTION:     Invoked by the Call Control to send error resp for offer
 *                  change
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_offer_chg_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_force_clear
 *
 * DESCRIPTION:     Invoked by the Call Control to forceflly clear call
 *                  in SSA. CC will not expect any response now from SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_force_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/* -------------------------------------------------------------------------- */
/* **** CC-ADM interface Functions **** */
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_incoming_call_ind
 *
 * DESCRIPTION:     This function is used to send an incoming call
 *                  indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_incoming_call_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

ICF_HANDLE_REINVITE_START
/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_ind
 *
 * DESCRIPTION:     This function is used to send reinvite indication to
 *                  the PA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_cancel_ind
 *
 * DESCRIPTION:     This function is used to send reinvite cancel indication to
 *                  the PA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_cancel_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_modify_resp
 *
 * DESCRIPTION:     This function is used to send reinvite resp to
 *                  the PA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_fax_resp
 *
 * DESCRIPTION:     This function is used to send fax resp to
 *                  the PA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_fax_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_start_fax_ind
 *
 * DESCRIPTION:     This function is used to send start fax indication to
 *                  the PA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_send_pa_start_fax_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

ICF_HANDLE_REINVITE_END

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_alert_ind
 *
 * DESCRIPTION:     This function is used to send an alert indication to
 *                  the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_alert_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_media_connect
 *
 * DESCRIPTION:     This function is used to send a connect indiaction
 *                  to the PA on receiving CONNECT from the SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_media_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_connect_ind
 *
 * DESCRIPTION:     This function is used to send a connect indiaction
 *                  to the PA on receiving CONNECT from the SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_connect_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_ind
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  indication to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_resp
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_call_term_resp_invalid_callid
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA when PA sends call_term_req.
 *                  This request will come with an invalid call-id, CC will 
 *                  simply send a call_term_resp with the same call-id as was 
 *                  sent by PA in call_term_req.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_call_term_resp_invalid_callid(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_fax_resp_invalid_callid
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  response to the PA when PA sends fax_req.
 *                  This request will come with an invalid call-id, CC will 
 *                  simply send a fax_resp with the same call-id as was 
 *                  sent by PA in fax_req.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_fax_resp_invalid_callid(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_forwarding_ind
 *
 * DESCRIPTION:     This function is used to a forwarding indication to PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_forwarding_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_remote_hold_ind
 *
 * DESCRIPTION:     This function is used to send a remote hold indication
 *                  to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_remote_hold_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_remote_resume_ind
 *
 * DESCRIPTION:     This function is used to send a remote resume indication
 *                  to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_remote_resume_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_wo_ctx_call_term
 *
 * DESCRIPTION:     This function is used to send a call termination
 *                  indication to the PA when call ctx is not avilable
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_wo_ctx_call_term(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_call_id_t  call_id,
        IN    icf_call_clear_reason_t release_cause);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_chk_prack_req_media_param
 *
 * DESCRIPTION:     This function is used to send a prack to SSA in case
 *                  remote party want reliable resp for provisional resp 183
 *
 ******************************************************************************/
icf_return_t  icf_cc_chk_prack_req_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_prack_req_alert
 *
 * DESCRIPTION:     This function is used to send a prack to SSA in case
 *                  remote party want reliable resp for provisional resp 180
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_prack_req_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_snd_mms_pa_connect
 *
 * DESCRIPTION:     This function is used to send mms to mmi for for PA call
 *                  connect
 *
 ******************************************************************************/
icf_return_t  icf_cc_snd_mms_pa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_pa_connect
 *
 * DESCRIPTION:     This function is used to send a 200 OK on receciving mms
 *                  response for PA call connect
 *
 ******************************************************************************/
icf_return_t  icf_cc_mms_rsp_pa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_timer_id
 *
 * DESCRIPTION:     This function resets the timer id for the expired timer in
 *                  current call context
 *
 ******************************************************************************/
icf_return_t icf_cc_reset_timer_id(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_chk_user_busy_n_srvc
 *
 * DESCRIPTION:     This is a utility function which would check the call clear
 *                  reason returned by PA to be
 *                  ICF_CALL_CLEAR_REASON_USER_BUSY in call terminate API. If
 *                  so would check call forwarding to be active. If call
 *                  forwarding is active reduce the call waiting count if it was
 *                  a call waiting call
 *
 *****************************************************************************/
icf_return_t
icf_cc_chk_user_busy_n_srvc(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_div_info
 *
 * DESCRIPTION:     This function would clear the diversion information from
 *                  call context
 *
 ******************************************************************************/
icf_return_t icf_cc_clear_div_info(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_handle_reinvite_ack
 *
 * DESCRIPTION:     This function is used to handle reinvite ack
 *
 ******************************************************************************/
icf_return_t  icf_cc_handle_reinvite_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/*******************************************************************************
*
* FUNCTION NAME    :   icf_cc_ssa_send_update_msg
*
* DESCRIPTION  : This function triggers SSA to send UPDATE request on NW,
*                So it creates an internal message for SSA entry point function
*                for API ICF_NW_OTG_MEDIA_UPDATE.
*
* RETURNS:
*
*******************************************************************************/

icf_return_t icf_cc_ssa_send_update_msg(
        INOUT  icf_cc_pdb_st *p_cc_pdb);


#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_with_default_qos_values
 *
 * DESCRIPTION:     This function would populate the Qos parameters present
 *                  in local SDP structure with default Qos preconditions
 *                  before sending the initial INVITE.
 *
 ******************************************************************************/
icf_return_t icf_cc_fill_local_sdp_with_default_qos_values(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_and_update_qos_params
 *
 * DESCRIPTION:     This function would compare the local SDP structure with 
 *                  remote received SDP structure and then update local SDP
 *                  structure for Qos values.
 *
 ******************************************************************************/
icf_return_t icf_cc_compare_and_update_qos_params(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_match_curr_des_status_on_remote_nw
 *
 * DESCRIPTION:     This function would compare current and desired status
 *                  lines of local SDP on remote network and will return 
 *					ICF_SUCCESS if current status exceeds or matches desired status.
 *
 ******************************************************************************/
icf_return_t icf_cc_match_curr_des_status_on_remote_nw(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_match_curr_des_status_on_local_nw
 *
 * DESCRIPTION:     This function would compare current and desired status
 *                  lines of local SDP on local network and will return
 *                  ICF_SUCCESS if current status exceeds or matches 
 *                  desired status. This function is basically used to check
 *                  whether or not an UPDATE request can be sent for QOS
 *                  processing.
 *
 ******************************************************************************/
icf_return_t icf_cc_match_curr_des_status_on_local_nw(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_with_curr_values_inc_call
 *
 * DESCRIPTION:     This function would set the current_status values in the 
 *					local send/recv directions of local_sdp to
 *					ICF_RESOURCE_RESERVED, for an incoming call, since resource 
 *					reservation has already been completed.This will ensure 
 *					that 183 contains current status as RESERVED 
 *
 ******************************************************************************/
icf_return_t icf_cc_fill_local_sdp_with_curr_values_inc_call(
		INOUT icf_cc_pdb_st *p_cc_pdb);

#endif


ICF_SESSION_CONTROL_START
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_proc_pa_connect_payload
 *
 * DESCRIPTION:     This function copies additional lists in app connect req
 *					payload into corresponding members of call ctx. This can
 *					be called from any fsm handler that handles Connect frm app
 *
 ******************************************************************************/
icf_return_t icf_cc_proc_pa_connect_payload(
        INOUT	icf_cc_pdb_st		*p_cc_pdb);


/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_01_AWT_CMS_RESP
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_01_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_03_AWT_ALERT_RESP
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_03_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_04_AWT_MMS_RESP
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_04_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_05_AWT_CONNECT
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_05_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_06_CONNECTED
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_06_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is
 *                  received in state ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current State: ICF_CC_09_AWT_MEDIA_PARAM_PRACK
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_09_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting for DMS
 *                  response  from MMI and it receives SIP ACK for 200 OK sent
 *                  for incoming INVITE.
 *                  In this case, CC should send bye on network.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_CONNECT_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_connect_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*ICF_HANDLE_REINVITE_START*/
/*******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting for DMS
 *                  response  from MMI and it receives SIP ACK for 200 OK sent
 *                  for incoming reINVITE.
 *                  In this case, CC should send bye on network.
 *                  Current State: ICF_CC_07_AWT_DMS_RESP
 *                  Current Event: ICF_CC_SSA_REINVITE_ACK
 *
 ******************************************************************************/
icf_return_t icf_cc_07_ssa_reinvite_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/*ICF_HANDLE_REINVITE_END*/

/*******************************************************************************
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send error for the offer
 *                 received  in media PRACK from remote party.
 *                 4xx/5xx to be sent for PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_media_prack_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_alert_prack_err
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send error for the offer
 *                 received  in media PRACK from remote party.
 *                 4xx/5xx to be sent for PRACK
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_alert_prack_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_info_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting MMS
 *                  response from MMI and it receives info request from PA
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_INFO_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_04_pa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_pa_info_req
 *
 * DESCRIPTION:     This function is invoked when the CC is waiting for
 *                  START_PROC_MEDIA response from MMI and it receives info
 *                  request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_INFO_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_pa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_info_resp
 *
 * DESCRIPTION:     This function is used to send an info response to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_info_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_req
 *
 * DESCRIPTION:     This funtion would send info request to SSA when the same is
 *                  received from PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * DESCRIPTION:     This function starts Setup timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_info_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_info_timer
 *
 * DESCRIPTION:     This function stops info timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_info_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * DESCRIPTION:     This function is invoked when the CC is waiting for
 *                  START_PROC_MEDIA response from MMI and it receives info
 *                  request from PA
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_PA_INFO_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_06_ssa_info_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_06_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is connected state
 *					and INFO timer expires.
 *                  Timer expires.
 *                  Current State: ICF_CC_06_CONNECTED
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_06_timer_expiry(
		INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_handle_update_rem_sdp
 *
 * DESCRIPTION:     This function copy remote sdp pointer if SDP is recevied
 *                  in update and media was committed
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_handle_update_rem_sdp(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_info_req
 *
 * DESCRIPTION:    This function would validate the info request before it could
 *                 be sent to network
 *
 ******************************************************************************/
icf_return_t
icf_cc_validate_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

ICF_SESSION_CONTROL_END

ICF_HANDLE_REINVITE_START

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_modify_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives the RSP from PA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_RSP
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_modify_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives the RSP from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_MEDIA_UPDATE_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_modify_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives another reinvite from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_OFFER_CHANGE
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_offer_change(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives the CALL TERM REQ from PA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives  error
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 ******************************************************************************/
icf_return_t icf_cc_14_generic_err(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  reinvite resp and the it receives timer expire event.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/
icf_return_t icf_cc_14_timer_expiry(
        INOUT icf_cc_pdb_st *p_cc_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives the CALL TERM REQ from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_CALL_TERMINATE_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_term_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request and it receives SIP CANCEL from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_forceful_clear
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request CM want to clear call due to system error
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_14_forceful_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_mm_error_ind
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response
 *                  for the REINVITE request MM err indication is received.
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_MM_ERR_IND
 *
 *****************************************************************************/
icf_return_t icf_cc_14_mm_error_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#ifdef ICF_FAX_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_fax_resp
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the FAX request and it receives the RSP from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_FAX_RESP
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_fax_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the FAX request and it receives the FAX req from SSA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the response 
 *                  for the FAX request and it receives the FAX req from PA
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_FAX_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_14_pa_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_pa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the provisional response
 *                  for the REINVITE requestand it receives the alert RSP from PA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_PA_ALERT
 *
 ******************************************************************************/
icf_return_t icf_cc_15_pa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the provisional response
 *                  for the REINVITE request and it receives the alert from SSA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 ******************************************************************************/
icf_return_t icf_cc_15_ssa_alert(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_15_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when the CC is in awaiting the provisional response
 *                  for the REINVITE request and it receives the alert from SSA
 *                  Current State: ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_ALERT_WITH_MEDIA
 *
 ******************************************************************************/
icf_return_t icf_cc_15_ssa_alert_with_media(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_proc_pa_media_change_payload
 *
 * DESCRIPTION:     This function copies additional lists in media change req/rsp
 *					payload into corresponding members of call ctx. This can
 *					be called from any fsm handler that handles Re-Invite frm app
 *
 ******************************************************************************/
icf_return_t icf_cc_proc_pa_media_change_payload(
        INOUT	icf_cc_pdb_st		*p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_call_at_call_modify
 *
 * DESCRIPTION:     This function clears call during different error scenarios
 *					at call modify handling. The release cause will be filled by
 *					respective calling modules.
 *
 ******************************************************************************/
icf_return_t  icf_cc_clear_call_at_call_modify(
        INOUT  icf_cc_pdb_st *p_cc_pdb);
/*Function added in REl6.0*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_committed_sdp
 *
 * DESCRIPTION:     This function copies the current commited SDP data into current call
 *                  context's p_last_commited_sdp
 *
 ******************************************************************************/
icf_return_t  icf_cc_copy_committed_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_remote_sdp_in_all_streams
 *
 * DESCRIPTION:     This function checks in all the streams that remote SDP
 *                  is received or not
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_remote_sdp_in_all_streams(
        INOUT icf_cc_pdb_st *p_cc_pdb);


ICF_HANDLE_REINVITE_END



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_revert_sdp_stream
 *
 * DESCRIPTION:     This function copies applied sdp for each stream to remote
 *                  sdp stream
 *
 ******************************************************************************/
icf_return_t   icf_cc_revert_remote_sdp_stream(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_all_stream_sdp
 *
 * DESCRIPTION:     This function frees all memory allocated for all the streams
 *
 ******************************************************************************/
icf_return_t     icf_cc_clear_all_stream_sdp(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_remote_received_sdp_present
 *
 * DESCRIPTION:     This function check if remote SDP is present in any of the
 *                  streams
 ******************************************************************************/
icf_return_t   icf_cc_check_if_remote_received_sdp_present(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_applied_sdp_present
 *
 * DESCRIPTION:     This function check if applied SDP is present in any of the
 *                  streams
 ******************************************************************************/
icf_return_t   icf_cc_check_if_applied_sdp_present(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_rem_mode_to_mm_applied_mode
 *
 * DESCRIPTION:     This function copies remote_mode from remote_received_sdp
 *                  to mm_applied_mode for that stream
 ******************************************************************************/
icf_void_t     icf_cc_copy_rem_mode_to_mm_applied_mode(
         INOUT icf_cc_pdb_st *p_cc_pdb);

icf_return_t   icf_cc_compare_rem_apply_mode_n_mm_applied_mode(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*icf_inc_media_sdp_info_st*/
icf_void_t     icf_cc_free_inc_media_sdp(
         INOUT icf_cc_pdb_st *p_cc_pdb,
         INOUT icf_list_st  *p_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_populate_local_sdp
 *
 * DESCRIPTION:     This function populates local SDP from preferred media
 *                  profile if exists, else if media profile exists, populate
 *                  from it, otherwise fill default audio stream. This function
 *                  returns failure if preferred media profile is not subset of
 *                  media profile received in app_add.
 *
 ******************************************************************************/
icf_return_t    icf_cc_populate_local_sdp(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_rem_mode_to_mm_applied_mode
 *
 * DESCRIPTION:     This function copies remote_mode from remote_received_sdp
 *                  to mm_applied_mode for that stream
 ******************************************************************************/
icf_void_t     icf_cc_copy_mm_applied_mode_to_rem_mode(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_internal_sdp_stream
 *
 * DESCRIPTION:     This function will be copying internal_sdp_stream from
 *					p_src to *p_p_target.
 ******************************************************************************/
icf_return_t
icf_cc_copy_internal_sdp_stream(
	                icf_cc_pdb_st	*p_cc_pdb,
					icf_internal_sdp_stream_st   *p_src,
					icf_internal_sdp_stream_st   **p_p_target);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_memfree_internal_sdp_stream
 *
 * DESCRIPTION:     This function frees internal SDP stream
 *
 ******************************************************************************/
icf_return_t   icf_cc_memfree_internal_sdp_stream(
         icf_glb_pdb_st                *p_glb_pdb,
         icf_internal_sdp_stream_st    *p_int_sdp);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_sdp_info
 *
 * DESCRIPTION:     This function frees stream SDP info
 *
 ******************************************************************************/
icf_return_t    icf_cc_free_stream_sdp_info(
        icf_cc_pdb_st          *p_cc_pdb,
        icf_stream_sdp_info_st *p_stream_sdp);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_mm_applied_mode
 *
 * DESCRIPTION:     This function copies remote_mode from remote_received_sdp
 *                  to mm_applied_mode for that stream
 ******************************************************************************/
icf_void_t     icf_cc_set_mm_and_app_mode(
         INOUT   icf_cc_pdb_st     *p_cc_pdb,
         IN      icf_media_mode_t  media_mode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_local_sdp_media_mode
 *
 * DESCRIPTION:     This function copies given mode in local_sdp
 ******************************************************************************/
icf_void_t     icf_cc_set_local_sdp_media_mode(
         INOUT   icf_cc_pdb_st     *p_cc_pdb,
         IN      icf_media_mode_t  media_mode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_recvd_media_with_profile
 *
 * DESCRIPTION:     This function compares remote received media streams with
 *                  media profile received in app_add request.
 ******************************************************************************/
icf_return_t   icf_cc_check_recvd_media_with_profile(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_applied_mode_to_rem_mode
 *
 * DESCRIPTION:     This function copies committed media mode to remote mode
 *
 ******************************************************************************/
icf_void_t   icf_cc_copy_applied_mode_to_rem_mode(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_preferred_profile_with_app_media_profile
 *
 * DESCRIPTION:     This function compares preferred media profile with
 *                  media profile received in app_add
 *
 ******************************************************************************/
icf_return_t   icf_cc_check_preferred_profile_with_app_media_profile(
		icf_list_st    *p_pref_profile,
		icf_list_st    *p_app_profile);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_from_app_profile
 *
 * DESCRIPTION:     This function fills local SDP in call context from
 *                  media profile received in app_add
 *
 ******************************************************************************/
icf_return_t    icf_cc_fill_local_sdp_from_app_profile(
      icf_cc_pdb_st                 *p_cc_pdb,
      icf_stream_id_t               stream_id,
      icf_stream_capab_st      *p_app_profile,
      icf_stream_sdp_info_st        **p_stream_sdp_info);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_call_modify_offer
 *
 * DESCRIPTION:     This function processes the local call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_process_local_call_modify_offer(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_audio_stream_params
 *
 * DESCRIPTION:     This function compares two audio profiles
 ******************************************************************************/
icf_boolean_t   icf_cc_compare_audio_stream_params(
         icf_audio_profile_st	new_profile,
		 icf_audio_profile_st   existing_profile);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_video_stream_params
 *
 * DESCRIPTION:     This function compares two video profiles
 ******************************************************************************/
icf_boolean_t   icf_cc_compare_video_stream_params(
         icf_video_profile_st	new_profile,
		 icf_video_profile_st   existing_profile);

icf_void_t
icf_cc_compare_n_compute_mode(
			icf_media_mode_t  application_mode,
			icf_media_mode_t  applied_mode,
			icf_media_mode_t  *p_mode_to_be_applied);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_local_offer_change
 *
 * DESCRIPTION:     This function processes the local call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_make_mms_type_for_local_offer_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_fax_req
 *
 * DESCRIPTION:     This function checks if the incoming offer was for starting
 *					a fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_fax_resp
 *
 * DESCRIPTION:     This function checks if the incoming answer was a fax response
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_fax_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_fax_req
 *
 * DESCRIPTION:     This function validates and processes the local fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_local_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_req
 *
 * DESCRIPTION:     This function validates and processes the remote fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_resp
 *
 * DESCRIPTION:     This function validates and processes the remote fax
 *					success resposne.
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_err_resp
 *
 * DESCRIPTION:     This function validates and processes the remote fax
 *					error resposne.
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_err_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_offer
 *
 * DESCRIPTION:     This function processes the remote call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_offer(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_resp
 *
 * DESCRIPTION:     This function processes the remote call modify success
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_resp(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_err_resp
 *
 * DESCRIPTION:     This function processes the remote call modify error
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_err_resp(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_ith_level_m_line_check
 *
 * DESCRIPTION:     This function shall validate the ith level
 *                  check for the mlines received from the network
 *                  and also copy the SDP streams into the call context
 ******************************************************************************/
icf_return_t icf_cc_validate_ith_level_m_line_check(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_media_modify_type_for_all_streams
 * DESCRIPTION:     This function resets media_modify_type bitmask for all 
 *					streams
 *
 ******************************************************************************/
icf_return_t icf_cc_reset_media_modify_type_for_all_streams(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_remote_offer_change
 * DESCRIPTION:     This function sets the modify_media_type bitmask which
 *		    shall indicate which fields of the SDP have changed
 *		    and shall aid the icf_cc_send_mms_req for populating the
 *		    payload for MMS request sent to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_remote_offer_change(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_int_sdp_stream_to_remote
 *
 * DESCRIPTION:     This function shall delete the memory for the
 * 		    p_remote_received_sdp member of the p_stream_sdp_info
 * 		    passed in the PDB and assign the p_int_sdp_stream ptr
 * 		    in the PDB to the same
 ******************************************************************************/
icf_return_t icf_cc_copy_int_sdp_stream_to_remote(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_no_of_nodes_in_list
 *
 * DESCRIPTION:    This function shall return failure if number of nodes present in
 * 		   the  2 listd passed through the CC PDB
 * 		   are different.Internally it calls icf_cmn_get_list_count
 ******************************************************************************/
icf_boolean_t icf_cc_compare_no_of_nodes_in_list(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_sdp_n_populate_mod_fileds
 * DESCRIPTION :    This function shall compare the 2 SDP’s
 *		    (p_applied_sdp and p_remote_sdp) present in the
 *		    p_stream_sdp_info in p_cc_pdb and set the  media_modify_type
 *		    bitmask which shall indicate which fields of the SDP have changed.
 *		    If the stream type is FAX,it shall be ignored
 *		    In the case of audio and video only shall the codecs be compared.
 *		    Silence supp shall be compared only in the case of audio.
 *		    RTP and RTCP(if present) IP and port shall be compared for
 *		    audio,video and TBCP.
 *		    Mode shall be compared only in the case of sudio and video
 *
 ******************************************************************************/
icf_return_t icf_cc_compare_sdp_n_populate_mod_fileds(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_codec_lists
 * DESCRIPTION:     This function shall compare the codec lists passed as
 * 		    paramters in the CC PDB.
 * 		    First it shall compare the number of codecs
 * 		    The it shall compare the codec name and num(if present)
 * 		    in both the lists
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_codec_lists(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_codec_attrib
 * DESCRIPTION:     This function shall compare the codec attributes
 *		    passed as generic ptrs through the PDB.
 *		    It shall compare the codec name and the codec number
 *		    (if present)
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_codec_attrib(
        INOUT   icf_cc_pdb_st   *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_rem_media_params
 *
 * DESCRIPTION:     This function checks if mms is for remote SDP change
 *                  Also checks the silence suppression for audio stream
 *
 ******************************************************************************/

icf_boolean_t icf_cc_compare_rem_media_params(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_ip_and_port
 *
 * DESCRIPTION:     Assumption:
 *                  remote_rtp_addr != commited_rtp_addr &&
 *                  remote_rtp_addr != ICF_NULL)
 *                  ( For ip and port )
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_ip_and_port(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_analyse_mms_resp
 *
 * DESCRIPTION:     This function checks the MMS response received from MMI
 *                  and return the result as if MMS resp is successful or
 *                  rejected
 *
 *****************************************************************************/
icf_void_t icf_cc_analyse_mms_resp(
        INOUT  icf_cc_pdb_st        *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_alert_with_media
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert with media
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_alert_with_media(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_offer_change(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_offer_change_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_offer_change_resp(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is connect from application.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_call_modify_req(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_alert
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event Alert from application
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_alert(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_set_reject_stream_status
 *
 * DESCRIPTION:     This function iterates thru all the streams in the
 *                  icf_stream_sdp_info_st in call context and set the
 *                  given status in the rejected stream in MMS response
 *
 *****************************************************************************/
icf_void_t    icf_cc_set_reject_stream_status(
         icf_cc_pdb_st    *p_cc_pdb,
         icf_uint8_t      stream_status);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_reject_stream_status
 *
 * DESCRIPTION:     This function iterates thru all the streams in the
 *                  icf_stream_sdp_info_st in call context and reset the
 *                  given status in the rejected stream in MMS response
 *
 *****************************************************************************/
icf_void_t    icf_cc_reset_reject_stream_status(
         icf_cc_pdb_st    *p_cc_pdb,
         icf_uint8_t      stream_status);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_media_param
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA media param
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_media_param(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_offer_change
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_offer_change(
	icf_cc_pdb_st      *p_cc_pdb);


#ifdef ICF_FAX_SUPPORT
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_start_fax_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Start FAX request
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_start_fax_req(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_start_fax_req
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Start FAX request from remote
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_start_fax_req(
	icf_cc_pdb_st      *p_cc_pdb);

#endif

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_alert_prack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert prack
 *                  1. If FS call and MMS response contains all stream
 *                      rejection, send failure PRACK response
 *                  2. IF NFS call, complete offer rejected or some stream
 *                      deleted, as it is the first remote answer, send PRACK
 *                      response and clear the call
 *                  3. Otherwise, send success PRACK response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_alert_prack(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_param_prack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is alert prack
 *                  1. If FS call and MMS response contains all stream
 *                      rejection, send failure PRACK response
 *                  2. IF NFS call, complete offer rejected or some stream
 *                      deleted, as it is the first remote answer, send PRACK
 *                      response and clear the call
 *                  3. Otherwise, send success PRACK response
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_param_prack(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA connect ACK. In case of NFS
 *                  call remote answered SDP in ACK.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_connect_ack(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_call_connect
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA connect ACK. In case of NFS
 *                  call remote answered SDP in ACK.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_call_connect(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_call_connect
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is Call connect from application
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_call_connect(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_revert
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event media_revert local/remote
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_revert(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_initiate_call_clear
 *
 * DESCRIPTION:     This function initiates normal call clear
 *
 *****************************************************************************/
icf_return_t icf_cc_initiate_call_clear(
	icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_update_local_sdp
 *
 * DESCRIPTION:     This function updates local SDP codec from applied
 *                  SDP for all streams
 *
 *****************************************************************************/
icf_return_t icf_cc_update_local_sdp(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_update_to_be_sent
 *
 * DESCRIPTION:     This function checks if UPDATE is required to be sent
 *                  to peer if media is changed in MMS response
 *
 *****************************************************************************/
icf_return_t icf_cc_check_if_update_to_be_sent(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:			icf_cc_update_stream_sdps_info_considering_cms_resp
*
* DESCRIPTION:		This function is invoked to to mark streams Active, Deleted, NW
*					allow, Disallow and free memory assigned to local and remote
* 					sdps depending upon cms response for each stream.
* *****************************************************************************/
icf_result_t icf_cc_update_stream_sdps_info_considering_cms_resp(
	icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        	icf_cc_make_mms_type_for_rem_succ_ans
*
* DESCRIPTION:     This function is invoked to derive appropriate MMS type after
*				   receiving Answer to our INVITE from remote i.e. it is First
*				   answer And that answer will be 200 ok or 183, NOT 4xx.
*****************************************************************************/

icf_return_t icf_cc_make_mms_type_for_rem_succ_ans(
	icf_cc_pdb_st 	*p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_check_for_streams_deleted_by_user
 *
 * DESCRIPTION:    This function is invoked when Alert is received from the PA for
 * 				   incoming call and it will be used to mark if application has deleted
 * 				   any streams then we need to mark those streams deleted in the
 * 				   media_modify_type of their respective icf_stream_sdp_info
 * 				   structure.
*****************************************************************************/
icf_return_t icf_cc_check_for_streams_deleted_by_user(
 icf_cc_pdb_st		*p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        	icf_cc_make_mms_type_for_nth_rem_succ_ans
*
* DESCRIPTION:      This function is invoked to derive appropriate MMS type after
*					receiving Answer to our reINVITE offer from remote i.e. after
*					receiving 200 OK.
*					This is for nth answer from remote.
*
*					This function will receive remote answer list from
*					p_cc_pdb->p_list_1.
*******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_nth_rem_succ_ans
(
	icf_cc_pdb_st *p_cc_pdb
);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_app_media_data
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
icf_cc_create_app_media_data(
	    icf_cc_pdb_st	*p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_deleted_list
 *
 * DESCRIPTION:     This function copies the deleted stream list info from
 *                  PDB structure to message struture.
******************************************************************************/
icf_return_t
icf_cc_copy_deleted_list(
                         icf_cc_pdb_st *p_cc_pdb,
                         icf_stream_list_st *p_deleted_list);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_media_modify_type
 *
 * DESCRIPTION:     This function marks the MMS type on per stream basis in MMI
 *                  payload.
******************************************************************************/
icf_return_t
icf_cc_set_media_modify_type(
                         icf_cc_pdb_st * p_cc_pdb,
                         icf_mmi_mms_request_st *p_mms_payload);


/*****************************************************************************
 ** FUNCTION:     icf_cc_delete_inc_media_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_inc_media_sdp_info.
*****************************************************************************/
icf_return_t
icf_cc_delete_inc_media_sdp_info_list(
	        icf_cc_pdb_st	*p_cc_pdb,
            icf_list_st	    **p_p_inc_media_sdp);

/*****************************************************************************
 ** FUNCTION:     icf_cc_make_mms_type_for_remote_answer
 **
 ** DESCRIPTION:  This function decides the MMS type for first remote answer.
*****************************************************************************/
icf_return_t
icf_cc_make_mms_type_for_remote_answer(
	        icf_cc_pdb_st	*p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_send_mms_to_revert_audio_mode
 **
 ** DESCRIPTION:  This function is for reverting audio mode.
*****************************************************************************/
icf_return_t
icf_cc_send_mms_to_revert_audio_mode(
	        icf_cc_pdb_st	*p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_send_mms_req_for_fax_failover
 **
 ** DESCRIPTION:  This function is for sending MMS request for fax failover
*****************************************************************************/
icf_return_t
icf_cc_send_mms_req_for_fax_failover(
	        icf_cc_pdb_st	*p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_generate_stream_id
 *
 * DESCRIPTION:     This function generates a new stream ID for the newly added
 *                  stream
******************************************************************************/
icf_return_t
icf_cc_generate_stream_id(
                         icf_cc_pdb_st * p_cc_pdb,
                         icf_uint8_t    *p_stream_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_from_profile
 *
 * DESCRIPTION:     This function fills local Sdp in call context either from
 *                  preferred media profile or app_add profile depending upon the
 *					preferred_profile_exists boolean flag.
 *
 *					Before calling this function we need to set following things:
 *					1) p_cc_pdb->stream_id, if it pref profile does NOT exists.
 *					2) p_cc_pdb->flag, with ICF_TRUE if pref profile exists
 *					   otherwise ICF_FALSE.
 * 					3) p_cc_pdb->p_generic_ptr_1 with appropriate stream's stream
 *					   capab pointer either from pref profile or from
 *					   app_add profile.
 *
 * 					Most importantly pointer to structure of type
 *					icf_stream_sdp_info will be returned in p_cc_pdb->
 *					p_stream_sdp_info
 ******************************************************************************/
icf_return_t
icf_cc_fill_local_sdp_from_profile(
	icf_cc_pdb_st                 *p_cc_pdb  );

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_add_new_fax_stream
 *
 * DESCRIPTION:     This function adds new FAX stream
******************************************************************************/
icf_return_t
icf_cc_add_new_fax_stream(
                         icf_cc_pdb_st * p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_from_stream_capab
 *
 * DESCRIPTION:     This function fills local SDP in call context from
 *                  		media profile received in app_add
 *
 *					Before calling this function we need to set
 *					p_cc_pdb->p_strea_sdp_info with the streams p_stream_sdp_info
 *					whose local sdp is to be created from stream capab list.
 *
 *					Also we need to initialise p_cc_pdb->p_generic_ptr_1 with
 *					appropriate stream capab from which local sdp is to be
 *					created.This generic pointer will be type casted to icf_stream_
 *					capab_st pointer.
******************************************************************************/
icf_return_t    icf_cc_fill_local_sdp_from_stream_capab
(
	icf_cc_pdb_st                 *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_id_to_type_str
 *
 * DESCRIPTION:     This fucntion gets the stream type string value from
 *                  stream_id
 *
 ******************************************************************************/
icf_return_t  icf_cc_id_to_type_str(
        INOUT  icf_cc_pdb_st *p_cc_pdb,
        INOUT   icf_uint8_t *p_stream_type);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stream_type_to_str
 *
 * DESCRIPTION:     This function converts the stream type to string value
 *
 ******************************************************************************/
icf_return_t  icf_cc_stream_type_to_str(
        INOUT  icf_uint8_t  stream_type,
        INOUT   icf_uint8_t *p_steam_type);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_call_modify_err_resp
 *
 * DESCRIPTION:     This function processes the local call modify error
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_local_call_modify_err_resp(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_derive_mms_type_for_revert_remote_change
 *
 * DESCRIPTION:     This function derives the MMS type for reverting the remote
 *                  media change
 ******************************************************************************/
icf_return_t
icf_cc_derive_mms_type_for_revert_remote_change(
                                      icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_derive_mms_type_for_rem_reject(icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_check_remote_mode_change(icf_cc_pdb_st * p_cc_pdb);


/*HANDLE_INFO_IND_START*/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_info_ind
 *
 * DESCRIPTION:     This function is used to send an info request to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_info_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*HANDLE_INFO_IND_END*/


#ifdef ICF_FAX_PASS_THROUGH
/*******************************************************************************
 *
 * FUNCTION:        icf_validate_incoming_sdp_in_rsp
 *
 * DESCRIPTION:     This function validates the incoming SDP in 200ok of a reINVITE
 *					If codec is not what is sent/mode is diff, then clear the call.
 *					If port is changed, then send mms_req
 *
 ******************************************************************************/

 icf_return_t icf_validate_incoming_sdp_in_rsp(
		        INOUT  icf_cc_pdb_st *p_cc_pdb,
				INOUT  icf_boolean_t *p_is_mms_send);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_silence_supp
 *
 * DESCRIPTION:     This function validates whether there is need to do mms
 *					for silenceSupp or not.If the incoming silenceSupp is not
 *					same as that we have previously configured, then we send the mms
 *					otherwise we dont send mms.
 ******************************************************************************/
icf_boolean_t icf_cc_validate_silence_supp(
				INOUT  icf_cc_pdb_st *p_cc_pdb);
#endif
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_party_replaced_ind
 *
 * DESCRIPTION:     This function is used to send party replaced indication
 *                  to the PA on receiving Invite with Replaces from SSA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_party_replaced_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        icf_cc_derive_mms_remote_rejected
*
* DESCRIPTION:     This function is invoked when get 4xx response to our offer.
*******************************************************************************/
icf_return_t icf_cc_derive_mms_remote_rejected
(
 icf_cc_pdb_st *p_cc_pdb
);

icf_return_t icf_cc_derive_mode(icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_calc_mode_app_reject_change(icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_calc_mode_app_change(icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_check_remote_mode_change(icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_calc_mode_remote_reject_change(icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_back_local_sdp
 *
 * DESCRIPTION:     This function takes backup of local SDP from last
 *                  committed SDP
 *
 *****************************************************************************/
icf_return_t icf_cc_back_local_sdp(
        INOUT  icf_cc_pdb_st        *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_get_call_type
 *
 * DESCRIPTION:     This function converts stream type to call type
 *
 *****************************************************************************/
icf_void_t 
icf_cc_get_call_type(INOUT  icf_cc_pdb_st        *p_cc_pdb,
                     IN  icf_media_type_t			stream_type);

/******************************************************************************
*
* FUNCTION:			icf_cc_update_stream_sdps_aftr_cms_resp
*
* DESCRIPTION:		This function is invoked to to mark streams Active, Deleted, NW
*					allow, Disallow and free memory assigned to local and remote
* 					sdps depending upon cms response for each stream.
* *****************************************************************************/
icf_result_t icf_cc_update_stream_sdps_aftr_cms_resp
(
	icf_cc_pdb_st *p_cc_pdb
);

#ifdef ICF_TRACE_ENABLE
/******************************************************************************
*
* FUNCTION:         icf_cc_update_trace_info_aftr_mms_resp
*
* DESCRIPTION:      This function is invoked to to mark streams Active, Deleted,NW
*                   allow, Disallow and free memory assigned to local and remote
*                   sdps depending upon cms response for each stream.
* *****************************************************************************/
 icf_result_t icf_cc_update_trace_info_aftr_mms_resp
 (
     icf_cc_pdb_st *p_cc_pdb
 );
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_data_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream data list
 ******************************************************************************/
 icf_return_t
    icf_cc_free_stream_data_list(
             icf_cc_pdb_st   *p_cc_pdb);
#endif
              
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_delete_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to delete the app media data
******************************************************************************/
icf_return_t
icf_cc_delete_app_media_data(
	    icf_cc_pdb_st	*p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_call_modify_data_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the call modify data
******************************************************************************/
icf_return_t
icf_cc_free_call_modify_data_list(
	    icf_cc_pdb_st	*p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_call_modify_data_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the call modify data
******************************************************************************/
icf_return_t
icf_cc_free_call_modify_data_list(
	    icf_cc_pdb_st	*p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_capab_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream capab data
******************************************************************************/
icf_return_t
icf_cc_free_stream_capab_list(
	    icf_cc_pdb_st	*p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_media_change_ack
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event media_change_ack
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_media_change_ack(
	icf_cc_pdb_st      *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_connect_resp
 *
 * DESCRIPTION:     This function is used to send a connect response
 *                  to the PA on receiving CONNECT ACK from the SSA for an 
 *                  incoming call.
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_connect_resp(
        INOUT icf_cc_pdb_st *p_cc_pdb);


 /******************************************************************************
 *
 * FUNCTION:        icf_cc_update_applied_sdp_mode
 *
 * DESCRIPTION:     This function updates the media mode in applied sdp
 *
 *****************************************************************************/
icf_void_t icf_cc_update_applied_sdp_mode(
INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_connect_send_mms_req
 *
 * DESCRIPTION:     This function checks for rejected streams present
 *                  in the call context and marks the rejected streams 
 *                  for deletion.This function also updates the application 
 *                  mode and invokes function to derive the mode.
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_connect_send_mms_req(
        INOUT  icf_cc_pdb_st        *p_cc_pdb);





/******************************************************************************
 *
 * FUNCTION:        icf_cc_chk_if_stream_is_rejected
 *
 * DESCRIPTION:     This function checks for rejected streams present
 *                  in the call context
 *
 *****************************************************************************/
icf_boolean_t icf_cc_chk_if_stream_is_rejected(
        INOUT  icf_cc_pdb_st        *p_cc_pdb);
        
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_start_fax_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event SSA FAX RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_start_fax_resp(
	icf_cc_pdb_st      *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_local_sdp_media_mode_from_rem_mode
 *
 * DESCRIPTION:     This function copies remote received media mode in local_sdp
 ******************************************************************************/
icf_void_t     icf_cc_set_local_sdp_media_mode_from_rem_mode(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_get_remote_mode
 *
 * DESCRIPTION:     This function sets the remote mode as required byMMI
 *                  Inactive and send recv remain same
 *                  while send_only change to recv_only and
 *                  recv_only changes to send_only
 *
 ******************************************************************************/
icf_media_mode_t icf_cc_get_remote_mode(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/******************************************************************************
 *
 * FUNCTION:        icf_cc_01_dms_resp
 *
 * DESCRIPTION:     This function is invoked when the CC has send DMS request 
 *                  and MM sends REL_RES or CLOSE_CHANNEL_IND and no DMS resp 
 *                  is awaited from Network
 *                  Current State: ICF_CC_01_AWT_CMS_RESP
 *                  Current Event: ICF_CC_DMS_RESP
 *
 *****************************************************************************/

icf_return_t icf_cc_01_dms_resp (
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_process_app_stream_capab_req
 *
 * DESCRIPTION:     This Function will be getting pointer to list of type
 *					icf_stream_capab_st through p_cc_pdb->p_list_1.
 *					Now this function will get the stream capab list's 
 *                  node one by one and extract the corresponding stream from
 *                  p_cc_pdb->p_call_ctx->p_stream_sdp_info.
 *                  if the corresponding stream does not exist then we have to 
 *                  clear the call by setting some bit in p_cc_pdb->common_bitmask.
 *                  if the corresponding stream exists then copy encoded attributes's
 *                  pointer from stream capab list into the local sdp's encoded
 *                  attributes.
 *****************************************************************************/
icf_return_t icf_cc_process_app_stream_capab_req(
				icf_cc_pdb_st	*p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_fax_passthrough_req
 *
 * DESCRIPTION:     This function validates and processes the fax passthrough 
 *                  request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_fax_passthrough_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_get_node_from_stream_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *					icf_stream_capab_st, if corresponding matching stream type
 *					is found in the list.
******************************************************************************/
icf_internal_list_st* icf_cc_get_node_from_stream_list(
	icf_internal_list_st	*p_list_node,
	icf_media_type_t	stream_type);

/*****************************************************************************
 ** FUNCTION:     icf_cc_calc_mode_local_fax_fail
 **
 ** DESCRIPTION:  This function is called when remote  rejects media 
 **                modify request from Application
*****************************************************************************/

icf_return_t icf_cc_calc_mode_local_fax_fail(icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_ssa_msg
 *
 * DESCRIPTION:     This function is invoked to free ssa message if it is ignored.
 *
 ******************************************************************************/
icf_return_t  icf_cc_free_ssa_msg(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_streams_for_reoccurence
 *
 * DESCRIPTION:     This function will check if a stream is occuring
 *                                      multiple times then it will return ICF_TRUE otherwise
 *                                      ICF_FALSE.And if it is outgoing call and a stream is
 *                                      recurring then we have to clear the call.
 ******************************************************************************/
icf_boolean_t icf_cc_check_streams_for_reoccurence
(
 icf_cc_pdb_st *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_audio_call_by_default
 *
 * DESCRIPTION:     This function creates icf_stream_sdp_info node for
 *                                      Audio stream by default.
 ******************************************************************************/
icf_return_t icf_cc_create_audio_call_by_default
(
 icf_cc_pdb_st  *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_audio_call
 *
 * DESCRIPTION:     This function is invoked if outgoing call is initiated
 *                  by some service NOT by PA. This function will create
 *                  Audio stream from pref(if exists),otherwise from app_add,
 *                  if app_add does not contain Audio then it will create
 *                  default Audio stream.
******************************************************************************/
icf_return_t icf_cc_create_audio_call(
 icf_cc_pdb_st  *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:      icf_cc_fill_local_sdp_checking_incoming_and_appadd_profile
*
* DESCRIPTION:   This function is invoked to validate the incoming streams with
*                app_add profile that which streams are allowed and which are
*               not.
******************************************************************************/

icf_boolean_t icf_cc_fill_local_sdp_checking_incoming_and_appadd_profile
(
 icf_cc_pdb_st          *p_cc_pdb,
 icf_app_conf_st        *p_app_conf
);



/*******************************************************************************
*
* FUNCTION:    icf_cc_remote_revert_modify_type
*
* DESCRIPTION: This function is invoked to revert to previous stream parameters,
*              if any modification was done on this stream. Before invoking this
*              function we need to set p_cc_pdb->p_stream_sdp_info
********************************************************************************/
icf_return_t icf_cc_remote_revert_modify_type
(
 icf_cc_pdb_st  *p_cc_pdb
);

/***********************************************************************
* FUNCTION:     icf_cc_modification_accepted_for_stream
*
* DESCRIPTION:  This is valid in case our modification offer for this particular
*               stream has been accepted.
*               if in our offer we sent local codec change then
*               set the bit remote answer Codec change
*               And Reset the bit local codec change.
*               else
*               call the function which will compare remote sdp with applied sdp
*               call the function remote_mode_change()
* NOTE:         Before calling this function we need to set 
*               p_cc_pdb->p_stream_sdp_info with structure of this stream.
************************************************************************/
icf_return_t icf_cc_modification_accepted_for_stream
(                               
    icf_cc_pdb_st   *p_cc_pdb
);



/*******************************************************************************
*
* FUNCTION:    icf_cc_check_for_modify_type
*
* DESCRIPTION: This function is invoked to check if modification(if there was 
*              any) has been accepted or rejected, if it was rejected then call
*              function for reverting to older parameters And if it was accepted
*              then call function for acting accordingly. Before invoking this
*              function we need to set p_cc_pdb->p_stream_sdp_info
********************************************************************************/
icf_return_t icf_cc_check_for_modify_type
(
    icf_cc_pdb_st   *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_stream_capab_from_applied_sdp
 *  
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *                  structure from applied sdp of p_cc_pdb->p_stream_sdp_info
 *                  ->p_applied_sdp and return pointer to newly created structure
 *                  in p_cc_pdb->p_generic_ptr_1.
******************************************************************************/
icf_return_t icf_cc_fill_stream_capab_from_applied_sdp
(
    icf_cc_pdb_st   *p_cc_pdb
);   

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_call_modify_media_from_applied_sdp
 *  
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *                  structure from applied sdp of p_cc_pdb->
 *                  p_stream_sdp_info->p_applied_sdp and return pointer to newly 
 *                  created structure in p_cc_pdb->p_generic_ptr_1.
******************************************************************************/
icf_return_t icf_cc_fill_call_modify_media_from_applied_sdp
(
    icf_cc_pdb_st   *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_cfm
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send info_cfm.
 *                 This function will pass the response code depending upon
 *                 whether result received is true or not. 
 *                  
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_cfm(
                            INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_info_cfm_err
 *
 * DESCRIPTION:    Invoked by CC toward SSA to send info_cfm error reponse.
 *                 This function will pass the response code depending upon
 *                 whether it is due to timer expiry or info_cfm is already pending. 
 *                  
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_info_cfm_err(
                             INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_info_cfm_timer
 *
 * DESCRIPTION:     This function starts info confirmation timer.This timer will 
 *                  will be for duration upto which we will wait for confirmation
 *                  of info_ind from application. 
 *
 ******************************************************************************/
icf_return_t icf_cc_start_info_cfm_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_info_cfm_timer
 *
 * DESCRIPTION:     This function stops info confirmation timer.This timer will 
 *                  will be for duration upto which we will wait for confirmation
 *                  of info_ind from application. 
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_info_cfm_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*SPR -13995  Aman 04-01-07*/
/*******************************************************************************
 *
 * FUNCTION:       icf_cc_handle_ssa_connect_failure
 *
 * DESCRIPTION:    This function handles the failure of 200ok.It sends ACK and 
 *                 send BYE on the network.                     
 * 
 ******************************************************************************/
icf_return_t  icf_cc_handle_ssa_connect_failure(  
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_ssa_connect_failure 
 *
 * DESCRIPTION:     This function validate for the failure of 200ok. 
 *
 ******************************************************************************/
icf_return_t  icf_cc_validate_ssa_connect_failure(
        INOUT  icf_cc_pdb_st *p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_set_set_remote_call_hold_status
 **
 ** DESCRIPTION:  This function is called to set remote call hold status 
 **               in stream data
 *****************************************************************************/
icf_return_t icf_cc_set_remote_call_hold_status(
             icf_cc_pdb_st   *p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_fill_remote_call_hold_status
 **
 ** DESCRIPTION:  This function is called to fill remote call hold status 
 **               in call modify request
 *****************************************************************************/

icf_return_t icf_cc_fill_remote_call_hold_status(
             icf_cc_pdb_st   *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_handle_non_fs_reinvite
 *
 * DESCRIPTION:     This function handles non FS reinvite as a special 
 *                  case by sending mode as sendrecv in 200 OK
 ******************************************************************************/
icf_void_t     icf_cc_handle_non_fs_reinvite(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_ssa_detach_call_ctx
 *
 * DESCRIPTION:     This function is used to send an detach call context to SSA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_ssa_detach_call_ctx(
                             INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_early_call_ind
 *
 * DESCRIPTION:     This function is used to send an early call
 *                  indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_early_call_ind(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_early_ind_cfm
 *
 * DESCRIPTION:     This function is invoked when the CC is
 *                  in AWT_EARLY_IND_CFM state and it receives a setup
 *                  request from PA
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_EARLY_CALL_RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_20_early_ind_cfm(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_ssa_call_cancel_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  for the early call response from the APP and
 *                  it receives a Call Cancel Request from SSA
 *                  (SIP CANCEL received from peer).
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_SSA_CALL_CANCEL_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_20_ssa_call_cancel_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_generic_err
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Early call cfm from the app and it receives
 *                  an error indication from MMI..
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_GENERIC_ERROR
 *
 *****************************************************************************/

icf_return_t icf_cc_20_generic_err (
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_forceful_call_clear
 *
 * DESCRIPTION:     This function is invoked when Forceful call clear is 
 *                  received in state ICF_CC_20_AWT_EARLY_IND_CFM 
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_CFM
 *                  Current Event: ICF_CC_FORCEFUL_CLEAR
 *
 *****************************************************************************/
icf_return_t icf_cc_20_forceful_call_clear(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  create media session resp from MMI and 
 *                  the setup timer expires.
 *                  Current State: ICF_CC_01_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_TIMER_EXPIRY
 *
 *****************************************************************************/

icf_return_t icf_cc_20_timer_expiry (
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_20_pa_call_term_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Early call cfm from the app and it receives
 *                  Terminate request from PA.
 *                  Current State: ICF_CC_20_AWT_EARLY_IND_RESP
 *                  Current Event: ICF_CC_PA_CALL_TERMINATE_REQ
 *
 *****************************************************************************/

icf_return_t icf_cc_20_pa_call_term_req (
        INOUT icf_cc_pdb_st *p_cc_pdb);


icf_return_t
icf_cc_is_any_line_without_user(INOUT icf_cc_pdb_st *p_cc_pdb,
                                OUT   icf_error_t    *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_unexpected_msg_ind
 *
 * DESCRIPTION:     This function is used to send an unexpected
 *                  message indication to the PA
 *
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_unexpected_msg_ind(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_handle_unexpected_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting in any
 *                  state for which call_modify_req is an unexpected event
 *                  and call_modify_req is received.
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_handle_unexpected_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_handle_unexpected_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting in any
 *                  state for which pa_fax_req is an unexpected event
 *                  and pa_fax_req is received.
 *                  Current Event: ICF_CC_PA_FAX_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_handle_unexpected_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);
/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_handle_unexpected_info_req
 *                  
 * DESCRIPTION:     This function is invoked when the CC is awaiting in any
 *                  state in which info req from user is a invalid event
 *                  and it receives info request from PA.
 *                  Current Event: ICF_CC_PA_INFO_REQ
 *      
 *****************************************************************************/
icf_return_t icf_cc_pa_handle_unexpected_info_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_create_pref_profile_node_from_local_sdp
 **
 ** DESCRIPTION:  This Function will be creating node of type icf_stream_capab_st
 **               from the local sdp of the stream whose icf_stream_sdp_info_st
 **               block is being passed to this function through p_cc_pdb->p_stream_sdp_info.
 **               After making this node, it will be added to the 
 **               p_cc_pdb->p_call_ctx->p_pref_media_profile.
*****************************************************************************/
icf_return_t 
icf_cc_create_pref_profile_node_from_local_sdp(icf_cc_pdb_st *p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_update_pref_profile_node_from_local_sdp
 **
 ** DESCRIPTION:  This Function will be updating nodes pref_media_profile from local sdp
 **               only for Audio and Video streams and moreover only if codecs for that
 **               particular stream are NULL in the preffered profile. 
*****************************************************************************/
icf_return_t 
icf_cc_update_pref_profile_node_from_local_sdp(icf_cc_pdb_st *p_cc_pdb);

/*****************************************************************************
 ** FUNCTION:     icf_cc_make_duplicate_of_offered_sdp_per_stream
 **
 ** DESCRIPTION:  This Function will be creating deep copy of p_local_sdp
 **               in p_offer_sdp on per stream basis.
*****************************************************************************/
icf_return_t 
icf_cc_make_duplicate_of_offered_sdp_per_stream(icf_cc_pdb_st *p_cc_pdb);

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_load_call_duration_timer
 *
 * DESCRIPTION:     This function starts load call duration timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_load_call_duration_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*****************************************************************************
 *
 * FUNCTION:        icf_cc_stop_load_call_dur_timer
 *
 * DESCRIPTION:     This function stopscurrently running timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_load_call_dur_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb);

#endif

icf_return_t
icf_cc_abort_options_trans(
        INOUT icf_cc_pdb_st     *p_cc_pdb);

icf_return_t
icf_cc_reset_media_pending_events(
        INOUT icf_cc_pdb_st     *p_cc_pdb);

icf_return_t    icf_cc_validate_n_add_rejected_streams_in_call_ctxt(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_stream_list_st rejected_stream_list[ICF_MAX_STREAM_PER_CALL],
        IN    icf_uint8_t   rej_count);


/*call_mod_cancel*/
/******************************************************************************
*
* FUNCTION:        icf_cc_04_pa_call_modify_cancel_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  reINVITE is sent over network and call modify cancel req 
*                  is received.
*****************************************************************************/
icf_return_t icf_cc_04_pa_call_modify_cancel_req(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        icf_cc_mms_resp_ssa_call_modify_cancel_req
* DESCRIPTION:     This function is invoked when MMS response is received
*                  and ongoing event media_revert for SSA call cancel.
*
*****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_call_modify_cancel_req(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_call_mod_cancel_req_ongoing_event_handling
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is cancel call modify request from PA.
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_call_mod_cancel_req_ongoing_event_handling(
    icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_remote_media_revert_check_bitmask_2
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  for revert media.This function checks the result of
 *                  mms response as well as common bitmask_2.
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_remote_media_revert_check_bitmask_2(
    icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_ssa_call_mod_cancel_resp_ongoing_event_handling
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is cancel call modify resp from SSA.
 *
 *****************************************************************************/
icf_return_t icf_cc_ssa_call_mod_cancel_resp_ongoing_event_handling(
    icf_cc_pdb_st      *p_cc_pdb);


/******************************************************************************
*
* FUNCTION:        icf_cc_06_pa_call_modify_cancel_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  reINVITE is sent over network and call modify cancel req
*                  is received.
*****************************************************************************/
icf_return_t icf_cc_06_pa_call_modify_cancel_req(
         INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
*
* FUNCTION:        icf_cc_14_pa_call_modify_cancel_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  reINVITE is sent over network and call modify cancel req
*                  is received.
*****************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_cancel_req(
         INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
*
* FUNCTION:        icf_cc_14_ssa_call_modify_cancel_resp
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  reINVITE is sent over network and call modify cancel req
*                  is received.
*****************************************************************************/
icf_return_t icf_cc_14_ssa_call_modify_cancel_resp(
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        icf_cc_send_pa_call_modify_cancel_resp
*
* DESCRIPTION:     This function is used for sending the response of cancel
*                  call modify request to PA. 
*****************************************************************************/
icf_return_t icf_cc_send_pa_call_modify_cancel_resp(
              INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
*
* FUNCTION:        icf_cc_revert_mms_for_call_mod_cancel_req
*
* DESCRIPTION:     This function is used for reverting the media to last 
*                  committed sdp when recvd cancel call modify request from PA 
*****************************************************************************/
icf_return_t icf_cc_revert_mms_for_call_mod_cancel_req(
              INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
*
* FUNCTION:        icf_cc_14_pa_stop_fax_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  reINVITE is sent over network and stop_fax_req is received
*                  to cancel the request.
*****************************************************************************/
icf_return_t icf_cc_14_pa_stop_fax_req (
         INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
*
* FUNCTION:        icf_cc_04_pa_stop_fax_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  immediately after sending the start fax req , application
*                  sends the cancel fax request. Here we check the ongoing
*                  event for start fax and put the stop fax as pending event.
*****************************************************************************/
icf_return_t icf_cc_04_pa_stop_fax_req(
           INOUT icf_cc_pdb_st *p_cc_pdb);
           
/******************************************************************************
*
* FUNCTION:        icf_cc_04_ssa_stop_fax_req
*
* DESCRIPTION:     This function is used for handling the scenario when
*                  immediately after sending the start fax req , application
*                  sends the cancel fax request. Here we check the ongoing
*                  event for start fax and put the stop fax as pending event.
*****************************************************************************/
icf_return_t icf_cc_04_ssa_stop_fax_req(
           INOUT icf_cc_pdb_st *p_cc_pdb);
           
/******************************************************************************
*
* FUNCTION:        icf_cc_send_pa_stop_fax_resp
*
* DESCRIPTION:     This function is used for sending stop fax resp to PA. 
*****************************************************************************/
icf_return_t icf_cc_send_pa_stop_fax_resp(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_pa_stop_fax_req
 *
 * DESCRIPTION:     This function is used for handling the PA responses for
 *                  stop fax after mms is done for reverting fax media. Also
 *                  if cancel is not send to network  it sends the same.
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_pa_stop_fax_req(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is used for handling the PA responses for
 *                  stop fax after mms is done for reverting fax media. Also
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_stop_fax_req(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_stop_fax_resp
 *
 * DESCRIPTION:     This function is called when 200 OK for cancel is recvd and
 *                  stop and start fax responses needs to be sent to PA.
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_stop_fax_resp(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function is called when 200 OK for cancel is recvd and
 *                  stop and start fax responses needs to be sent to PA.
 ******************************************************************************/
icf_return_t icf_cc_14_ssa_stop_fax_req(
           INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_revert_mms_fax_cancel
 *
 * DESCRIPTION:     This function is for reverting media when stop fax request is
 *                  received from application.
 ******************************************************************************/
icf_return_t  icf_cc_make_revert_mms_fax_cancel(
           INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
*
* FUNCTION:        icf_cc_process_remote_fax_revert
*
* DESCRIPTION:     This function validates and processes the reverting of 
*                  remote fax request.     
******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_revert(
                   INOUT  icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
*
* FUNCTION:        icf_cc_check_is_fax_stop_req
*
* DESCRIPTION:     This function checks if the incomming request is of fax
* type or not.     
******************************************************************************/
icf_return_t  icf_cc_check_is_fax_stop_req(
                   INOUT  icf_cc_pdb_st *p_cc_pdb);

icf_return_t icf_cc_check_audio_present_with_nw_allow(
                INOUT icf_cc_pdb_st *p_cc_pdb);
/******************************************************************************
 *
 * FUNCTION:        icf_cc_04_ssa_fax_resp
 *
 * DESCRIPTION:      
 *
 *****************************************************************************/
icf_return_t icf_cc_04_ssa_fax_resp(
       INOUT icf_cc_pdb_st *p_cc_pdb);




/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_stop_fax_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event SSA FAX RESP
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_resp_ssa_stop_fax_resp(
	icf_cc_pdb_st      *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_handle_unexpected_stop_fax_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting in any
 *                  state in which stop fax req from user is a invalid event
 *                  and it receives stop fax req request from PA.
 *                  Current Event: ICF_CC_PA_STOP_FAX_REQ
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_handle_unexpected_stop_fax_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


icf_return_t icf_cc_send_pa_stop_fax_ind(
              INOUT icf_cc_pdb_st *p_cc_pdb);

/*twc_update handling*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_01_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Create Media Session Response and call modify req 
 *                  is recvd from the PA
 *                  Current State: ICF_CC_01_AWT_CMS_RESP/
                                   ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_01_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  ALERT RESP from the PA(incoming call) and
 *                  it receives a call modify request from PA.
 *
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ 
 *
 *****************************************************************************/
icf_return_t icf_cc_03_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_04_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and offer change
 *                  is recvd from the PA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_04_AWT_MMS_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_04_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_05_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Modify Media Session Response and offer change
 *                  is recvd from the PA
 *                  (SIP UPDATE/RE-INVITE received from peer).
 *                  Current State: ICF_CC_05_AWT_CONNECT
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ
 *
 ******************************************************************************/
icf_return_t icf_cc_05_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_pa_call_modify_req
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting 
 *                  call modify resp and
 *                  it receives a call modify request from PA.
 *
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_PA_CALL_MODIFY_REQ 
 *
 *****************************************************************************/
icf_return_t icf_cc_14_pa_call_modify_req(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/*twc_update handling*/

/******************************************************************************
 *
 * FUNCTION:        icf_cc_14_ssa_alert
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting call modify resp
 *                  and 180 wo SDP is received
 *                  Current State: ICF_CC_14_AWT_CALL_MODIFY_RESP
 *                  Current Event: ICF_CC_SSA_ALERT
 *
 *****************************************************************************/
icf_return_t icf_cc_14_ssa_alert(icf_cc_pdb_st *p_cc_pdb);


/******************************************************************
 * FUNCTION:        icf_cc_process_nw_rejected_stream_list
 *
 * DESCRIPTION:     this function processes the new streams that are
 *                  recvd with port 0 from remote. We will add the
 *                  stream node temporarily to be removed after
 *                  sending the answer
 *****************************************************************/
 icf_return_t   icf_cc_process_nw_rejected_stream_list
 (
      INOUT   icf_cc_pdb_st     *p_cc_pdb
 );

#ifdef ICF_TRACE_ENABLE

/******************************************************************************
 *
 * FUNCTION:        icf_cc_create_criteria_data
 *
 * DESCRIPTION:     This function is invoked to populate criteria data
 *
 *****************************************************************************/
 icf_return_t icf_cc_create_criteria_data(
              INOUT icf_cc_pdb_st *p_cc_pdb);
/******************************************************************************
 *
 * FUNCTION:        icf_cc_create_call_connect_data
 *
 * DESCRIPTION:     This function is invoked to populate call connect data
 *
 *****************************************************************************/
icf_return_t icf_cc_create_call_connect_data(
            INOUT icf_cc_pdb_st  *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_trace_timer_type
 *
 * DESCRIPTION:     This function is invoked to populate criteria data
 *
 *****************************************************************************/
icf_void_t icf_cc_trace_timer_type(
              INOUT icf_cc_pdb_st *p_cc_pdb,
              IN icf_uint8_t timer_type);
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_app_trace
 *
 * DESCRIPTION:     This function is used by CC for trace handling.
 *
 ******************************************************************************/
icf_return_t icf_cc_app_trace(
          INOUT icf_cc_pdb_st *p_cc_pdb,
          IN icf_trace_id_t trace_id);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_sdp_info_data
 *
 * DESCRIPTION:     This Function will be invoked to create SDP info for Traces
 *                  in p_cc_pdb->p_generic_ptr_1.
 *****************************************************************************/
icf_return_t icf_cc_fill_sdp_info_data
(
           icf_cc_pdb_st    *p_cc_pdb,
           icf_uint8_t      sdp_type
);
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_sdp_info_list
 *
 * DESCRIPTION:     This Function will be invoked to create
 *                  offer/answer SDP information
 ******************************************************************************/
icf_return_t
    icf_cc_create_sdp_info_list(
        icf_cc_pdb_st   *p_cc_pdb,
        icf_uint8_t      sdp_type);

#endif
 
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_12_ssa_connect_ack
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting Start of
 *                  Signaling Transaction event and it receives a ACK for connect
 ******************************************************************************/
 icf_return_t icf_cc_12_ssa_connect_ack
 (
     INOUT icf_cc_pdb_st *p_cc_pdb
 );

#ifdef ICF_SESSION_TIMER
/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_resp_ssa_session_refresh_resp
 *
 * DESCRIPTION:     This function is invoked when MMS response is received
 *                  and ongoing event is SSA Offer Change response
 *
 *****************************************************************************/
 icf_return_t icf_cc_mms_resp_ssa_session_refresh_resp
 (
     icf_cc_pdb_st      *p_cc_pdb
 );    
#endif

/*********** Changes for Re-Invite w/o SDP Start (Rel7.0) ***********/

/*****************************************************************************
 * FUNCTION:        icf_cc_make_mms_type_for_remote_offer_change_wo_sdp
 * DESCRIPTION:     This function sets the media_modify_type bitmask which
 *                  shall indicate which fields of the SDP have changed
 *                  and shall aid the icf_cc_send_mms_req for populating the
 *                  payload for MMS request sent to MMI.
 *
******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_remote_offer_change_wo_sdp(
                               INOUT   icf_cc_pdb_st   *p_cc_pdb);

/************ Changes for Re-Invite w/o SDP End (Rel7.0) ************/


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_derive_mms_type_app_partial_reject
 *
 * DESCRIPTION:     This function derives the MMS type for reverting the remote
 *                  media change on partial reject from application
 ******************************************************************************/

icf_return_t
icf_cc_derive_mms_type_app_partial_reject(
        icf_cc_pdb_st *p_cc_pdb);

/************* Changes for Forking and Multiple Answer Start (Rel7.0) *********/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_forked_resp_id_exists
 *
 * DESCRIPTION:     This function checks if an SDP already exists for a given
 *                  forked_resp_id. Incase an SDP is present this function 
 *                  returns TRUE else it returns FALSE
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_if_forked_resp_id_exists(
	INOUT   icf_cc_pdb_st           *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cmn_free_forked_resp_list
 *
 * DESCRIPTION:     This is a utility function that frees the forked_resp_list.
 *                  It first checks if the p_data is same as the remote_sdp, 
 *                  if so, does not delete it and deletes the remaining nodes.
 *
 ******************************************************************************/

icf_return_t icf_cc_free_forked_resp_list
(
                INOUT	icf_cc_pdb_st	*p_cc_pdb,
                OUT	icf_error_t	*p_ecode
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_empty_node_forked_resp
 *
 * DESCRIPTION:     This function is called to create an empty node in the
 *                  forked_resp_list when a media stream is inactive/deleted
 *
 ******************************************************************************/

icf_return_t icf_cc_create_empty_node_forked_resp(
	INOUT   icf_cc_pdb_st   *p_cc_pdb
);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_mms_req_for_forked_resp
 *
 * DESCRIPTION:     This function checks if mms is required.
 *                  It copies the forked_resp into the remote_sdp when 200 ok
 *                  recieved with out SDP and incase remote_sdp do not match\
 *                  forked_resp
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_mms_req_for_forked_resp(
	INOUT   icf_cc_pdb_st   *p_cc_pdb
);

/*********** Changes for Forking and Multiple Answer End (Rel7.0) *************/


/************* Changes for Retry-After header Start (Rel7.0) **************/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_retry_after_timer
 *
 * DESCRIPTION:     This function starts Retry After timer.
 *
 ******************************************************************************/
 icf_return_t icf_cc_start_retry_after_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_duration_t retry_after_timer
);

/************** Changes for Retry-After header End (Rel7.0) ***************/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_pa_media_change
 *
 * DESCRIPTION:     This function processes the local media change
 ******************************************************************************/
icf_return_t     icf_cc_process_pa_media_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_pa_media_change
 *
 * DESCRIPTION:     This function processes the local media_change from PA.
 ******************************************************************************/
icf_return_t     icf_cc_make_mms_type_for_pa_media_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_pa_media_change
 *
 * DESCRIPTION:     This function processes the local media change
 ******************************************************************************/
icf_return_t     icf_cc_process_pa_media_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_pa_media_change
 *
 * DESCRIPTION:     This function processes the local media_change from PA.
 ******************************************************************************/
icf_return_t     icf_cc_make_mms_type_for_pa_media_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:       icf_cc_apply_pa_media_mode
 *
 * DESCRIPTION:     The funciton applies the media mode from PA for a 
 *                  given list of streams
 ******************************************************************************/

icf_return_t
icf_cc_apply_pa_media_mode(
        icf_internal_list_st  *p_stream_sdp_info,
        icf_media_mode_t        media_mode);

/******************************************************************
 * FUNCTION:        icf_cc_process_nw_added_invalid_stream_list
 *
 * DESCRIPTION:     this function processes the new fax streams that
 *                  are added without holding/removing the audio
 *                  stream. We will send port 0 for this fax stream.
 *****************************************************************/
icf_return_t  icf_cc_process_nw_added_invalid_stream_list
(
    INOUT   icf_cc_pdb_st     *p_cc_pdb
);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_02_ssa_call_progress
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  setup resp from SSA and it receives call progress indication
 *					- SIP 181/182/183 without sdp received from peer
 *                  Current State: ICF_CC_02_AWT_SSA_SETUP_RESP
 *                  Current Event: ICF_CC_SSA_CALL_PROGRESS
 *
 *****************************************************************************/
icf_return_t icf_cc_02_ssa_call_progress(
        INOUT icf_cc_pdb_st *p_cc_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_cc_03_ssa_call_progress
 *
 * DESCRIPTION:     This function is invoked when the CC is awaiting
 *                  Alert from the SSA and it receives call progress indication
 *					- SIP 181/182/183 without sdp received from peer
 *                  Current State: ICF_CC_03_AWT_ALERT_RESP
 *                  Current Event: ICF_CC_SSA_CALL_PROGRESS
 *
 *****************************************************************************/
icf_return_t icf_cc_03_ssa_call_progress(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_send_pa_progress_ind
 *
 * DESCRIPTION:     This function is used to send a call progress message 
 *                  indication to the PA
 *
 ******************************************************************************/
icf_return_t icf_cc_send_pa_progress_ind(
        INOUT 	icf_cc_pdb_st 	*p_cc_pdb,
		IN		icf_uint16_t	response_code);

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_ecan_present_in_encoded_attribs
 *
 * DESCRIPTION:     This function checks if ecan is received from remote then do
 *                  mms for this
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_ecan_present_in_encoded_attribs(
        INOUT icf_cc_pdb_st *p_cc_pdb);


/*****************************************************************************
 ** FUNCTION:     icf_cc_update_nw_allow_for_stream(
 **
 ** DESCRIPTION:  This function would search the stream of type stream_type and
 **               would set/unset the network allow flag for the same
 **               based on the value of input parameter set_value
*****************************************************************************/
icf_void_t
icf_cc_update_nw_allow_for_stream(
	        icf_cc_pdb_st	*p_cc_pdb,
                icf_uint8_t     stream_type,
                icf_boolean_t   set_value);

/*Fix for CSR 7894024 -SPR 20190*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_ssa_offer_change
 *
 * DESCRIPTION:     This function processes checks the ssa offer change received from
 *                   remote.This is needs to be checked only in case of T.38 Fax Call.
 *                   If T.38 Fax call is established and IPTK receives ReInvite From
 *                   network, then CC shall check for any change in SDP for ReInvite.
 *                   If there is any change, then CC shall send 488 else sends 200ok.
 *********************************************************************************/

icf_boolean_t   icf_cc_check_ssa_offer_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_CC_PROTOTYPES_H__ */
