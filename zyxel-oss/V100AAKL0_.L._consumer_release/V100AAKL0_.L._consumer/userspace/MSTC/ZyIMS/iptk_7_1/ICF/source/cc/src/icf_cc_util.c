/*------------------------------------------------------------------------------
 *
 * File name        : icf_cc_util.c
 *
 * Purpose          : This file contains the utility function required by CC.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 10-Dec-2004  Pankaj Negi     Non Service LLD     Initial
 * 13-Dec-2004  Umang Singh                         Added certain functions
 * 18-Dec-2004  Rohit Aggarwal                      Beginning of coding
 * 21-Dec-2004  Rohit Aggarwal                      Added trace IDs for MMI req
 * 22-Dec-2004  Rohit Aggarwal                      Added trace IDs and ecode
 * 22-Dec-2004  Rohit Aggarwal                      Coding of utility functions
 * 23-Dec-2004  Rohit Aggarwal                      Coding of utility functions
 * 27-Dec-2004  Rohit Aggarwal                      Coding guidelines violation
 *                                                  fixes
 * 27-Dec-2004  Rohit Aggarwal                      Header files changed
 * 28-Dec-2004  Rohit Aggarwal                      Added trace compilation flag
 * 29-Dec-2004  Rohit Aggarwal                      Added timer handling code
 * 14-Jan-2005  Rohit Aggarwal                      Removed insure compilation
 *                                                  warnings
 * 16-Jan-2005  Rohit Aggarwal                      Changed trace string params
 * 23-Jan-2005  Sumit Gupta                         Added code for freeing call
 *                                                  context in case of start 
 *                                                  timer failure
 * 05-Apr-2005  Sumit Gupta        SPR No 7354      Removed the code clearing up
 *                                                  diversion info from call 
 *                                                  context from the function 
 *                                                  icf_cc_clear_call_context
 *                                                  and created a new function 
 *                                                  to clear diversion info 
 *                                                  which is called after 
 *                                                  sending setup_req to ssa
 * 13-Apr-2005  Sumit Gupta                         Updation for change in 
 *                                                  diversion data structure in 
 *                                                  icf_common_types.h
 * 20-Apr-2005  Sumit Gupta        SPR No 7412     Set p_sip_ctx pointer to NULL
 *                                                 if validation for redirect 
 *                                                 request was successful
 * 25-Apr-2005  Sumit Gupta        SPR No 7446     Updation in function 
 *                                                 icf_set_mms_type
 * 26-Apr-2005  Sumit Gupta        SPR No 7449     mms_type for mode change 
 *                                                 would be set only in case its
 *                                                 not already set for other 
 *                                                 types in function 
 *                                                 icf_cc_set_mms_type
 * 27-Apr-2005 Sumit Gupta       Changes in common p_replace_nw_call_id in 
 *                               form: cf_0001101  icf_common_types.h was 
 *                                                 replaced with 
 *                                                 p_replaces_header. 
 *                                                 Corresponding change done
 * 05-May-2005 Sumit Gupta       Spr No 7498       Previously CC was checking 
 *                                                 that call should not be 
 *                                                 redirected to the user who 
 *                                                 has made the call. This is 
 *                                                 been enhanced to check the 
 *                                                call should not be redirected
 *                                                 to any of the users on any 
 *                                                 line in system and also none
 *                                                 of the diverted from addr in
 *                                                list should be any local user
 * 06-May-2005 Sumit Gupta      Spr No 7497       Added the code for the same 
 * 15-May-2005 Sumit Gupta      Spr No 7545       Before starting release timer 
 *                                                ckeck is put that the timer in
 *                                                execution should not be 
 *                                                release timer        
 * 16-May-2005 Sumit Gupta      SPR 7504          Correction check for
 * 25-May-2005 Sumit Gupta      SPR 7440          Updated Stats
 * 25-May-2005 Sumit Gupta      SPR 7614          In case of SSA_CONNECT_ACK 
 *                                                received with SDP and media 
 *                                                was not comitted, if there was
 *                                                some SDP in 
 *                                                remote_received_sdp then free 
 *                                                that
 * 26-May-2005 Sumit Gupta      SPR 7599          Remote held/resume indication 
 *                                                would not be PA from function 
 *                                                icf_set_mms_type if the 
 *                                                CALL_CONNECT_DONE flag is not 
 *                                                set also no mms would be sent 
 *                                                in case mms is for mode change
 *                                                and rem_media_mode is same as 
 *                                                rem_committed_mode
 * 01-Jun-2005 Sumit Gupta      SPR 7635          180 message would be sent with 
 *                                                require header as 100 Rel if 
 *                                                previous 183 message had 100 
 *                                                rel in require header no matter
 *                                                what the current configuration 
 *                                                states 
 * 01-Jun-2005 Jalaj Negi       IMS CLIENT        Added code for IMS CLIENT
 * 10-Jun-2005 Sumit Gupta      SPR 7649          Updated code for function
 *                                                icf_set_mms_type    
 * 13-Jun-2005 Sumit Gupta      SPR 7662          Update for clearing conference 
 *                                                status in other active call in
 *                                                conference in case one of the 
 *                                                calls in conference is released
 * 15-Jun-2005 Sumit Gupta      SPR 7660          Stop the timer when handling 
 *                                                pending event SSA connect for 
 *                                                an otg call
 * 22-Jun-2005 Sumit Gupta      SPR 7702          Updation in function 
 *                                                icf_cc_set_mms_type
 * 10-Aug-2005	Rohit Aggarwal	ICF				  Code changes specific to ICF
 * 17-Aug-2005	Rohit Aggarwal	ICF				  Init p_called_party_id in call ctx
 *
 * 09-Dec-2005  Jalaj Negi      ICF_4_0           Freed P-Media-Authorization header
 *                                                for QoS support
 * 08-Mar-2006  Aman Aggarwal					  ICF merging with ICF2.0
 * 30-Mar-2006    Anuradha Gupta	ICF 5.0   Call Modify Feature changes
 * 07-Apr-2005	Anuradha Gupta	 ICF 5.0		Call Modify changes
 * 26-Jun-2006  Amit Sharma     ICF 6.0 APIs      Structure changes for multiple
 *                                                m-lines
 * 25-Jul-2006  Umang Singh	    ICF 6.0		      Multiple m-lines 
 * 31-Jul-2006  Amit Sharma     ICF 6.0           Code changes for Multiple m-lines
 * 05-Jun-2005	Aman Aggarwal	ICF 5.1		  Verification of silenceSupp in SDP.
 * 30-Aug-2006  Umang Singh	 SPR 12683
 * 01-Sep-2006  Amit Sharma      SPR:12670
 * 20-Nov-2006  Umang Singh	 SPR 13794
 * 03-Jan-2007  Amit Mahajan                      INFO confirmation changes.
 * 04-Jan-2007   Priyanka Dua    ICF 6.3		  Code changes for Forking and 
 *												  Multiple answer
 * 06-Mar-2007  Abhishek Dhammawat SPR14647       Added Null check for local_sdp
 *                                                before icf_cc_copy_internal_sdp_stream
 * 26-Mar-2007  Deepti Goyal     ICF 6.4          Changes for media connect req
 *                                                API from PA.
 * 26-Mar_2007  Rajat Chandna                     Added func. icf_cc_validate_response_code_in_term_req
 *                                                as a part of response code enhancement.
 * 10-Jul-2007  Neha Choraria    ICF 7.0          Changes for Re-Invite w/o SDP
 * 11-Jul-2007  Neha Choraria    ICF 7.0          Changes for Support for Retry-After
 *                                                header
* 20-Jun-2007  Tarun Gupta      SPR 15124    Change in function icf_cc_mms_rsp_ssa_media_param:
 *                                            Added handling when the pending event is
 *                                            ICF_PENDING_EVENT_FMA_SSA_CONNECT.Changes done so
 *                                            that IPTK sends REINVITE for final codec
 *                                            negotiation in multiple answer.
 *31-Jul-2007   Amit Mahajan     SPR 15478
 *11-Dec-2007   Jagmohan Chauhan  SPR 17616    Fix for traces in start fax req from app 
 *06-May-2008	Deepti Goyal	 SPR 18519	    Fix for sending signalling mode received in
 *												call modify cfm in case reinvite wo SDP is
 *												received.
 * 09-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 * 27-May-2009  Anurag Khare    CSR 1-7302347  Changes done in function
 *                              SPR 19705      icf_cc_validate_redirect_req
 * 27-Jun-2009  Rajiv Kumar     SPR 19788      Fix for CSR-1-7444867.
 *                                             Resetting of bitmask ICF_CALL_AUTHENTICATION_REQ
 * 07-Jul-2009  Rohit Aggarwal	SPR 19827	Correcting alert timer duration for
 *											outgoing call.
 * 27-JUly-2009 Anurag Khare    CSR 1-7576638  Fix for CSR 1-7576638
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_common_interface_types.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_macros.h"
#include "icf_internal_api_id.h"
#include "icf_api.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif
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
        OUT icf_uint8_t *p_state_str)
{
    switch(state_val)
    {
        case ICF_CC_00_IDLE:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_00_IDLE");
            break;

        case ICF_CC_01_AWT_CMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_01_AWT_CMS_RESP");
            break;

        case ICF_CC_02_AWT_SSA_SETUP_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_02_AWT_SSA_SETUP_RESP");
            break;

        case ICF_CC_03_AWT_ALERT_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_03_AWT_ALERT_RESP");
            break;

        case ICF_CC_04_AWT_MMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_04_AWT_MMS_RESP");
            break;

        case ICF_CC_05_AWT_CONNECT:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_05_AWT_CONNECT");
            break;

        case ICF_CC_06_CONNECTED:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_06_CONNECTED");
            break;

        case ICF_CC_07_AWT_DMS_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_07_AWT_DMS_RESP");
            break;

        case ICF_CC_08_AWT_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_08_AWT_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_09_AWT_MEDIA_PARAM_PRACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_09_AWT_MEDIA_PARAM_PRACK");
            break;

        case ICF_CC_10_AWT_ALERT_PRACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_10_AWT_ALERT_PRACK");
            break;

        case ICF_CC_11_AWT_SSA_CONNECT_ACK:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_11_AWT_SSA_CONNECT_ACK");
            break;

        case ICF_CC_12_WAIT_START_PROC_SIG:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_12_WAIT_START_PROC_SIG");
            break;

        case ICF_CC_13_WAIT_START_PROC_MEDIA:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_13_WAIT_START_PROC_MEDIA");
            break;

ICF_HANDLE_REINVITE_START

		case ICF_CC_14_AWT_CALL_MODIFY_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_14_AWT_CALL_MODIFY_RESP");
            break;

		case ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP");
            break;

ICF_HANDLE_REINVITE_END

#ifdef ICF_QOS_SUPPORT

		case ICF_CC_16_AWT_UPDATE:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_16_AWT_UPDATE");
            break;

#endif


        case ICF_CC_18_CALL_STATE_INVALID:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_14_CALL_STATE_INVALID");
            break;

        case ICF_CC_20_AWT_EARLY_IND_CFM :
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_20_AWT_EARLY_IND_CFM");
            break;


        default:
            icf_port_strcpy(p_state_str,
                    (icf_uint8_t *)"ICF_CC_14_CALL_STATE_INVALID");
            break;

    } /* end of switch case for state_val */
} /* end of icf_cc_state_to_str() */
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
        OUT icf_uint8_t *p_event_str)
{
    switch(event_val)
    {
		case ICF_CC_PA_INFO_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_INFO_REQ");
            break;

         case ICF_CC_SSA_INFO_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_INFO_RESP");
            break;

        case ICF_CC_PA_SETUP_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_SETUP_REQ");
            break;

        case ICF_CC_SSA_SETUP_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_SETUP_RESP");
            break;

        case ICF_CC_SSA_MEDIA_PARAM:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_MEDIA_PARAM");
            break;

        case ICF_CC_SSA_ALERT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_ALERT");
            break;

        case ICF_CC_SSA_ALERT_WITH_MEDIA:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_ALERT_WITH_MEDIA");
            break;

        case ICF_CC_SSA_CALL_CONNECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CONNECT");
            break;

        case ICF_CC_CMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CMS_RESP");
            break;

        case ICF_CC_START_CALL_PROC_SIG:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_START_CALL_PROC_SIG");
            break;

        case ICF_CC_START_CALL_PROC_MEDIA:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_START_CALL_PROC_MEDIA");
            break;

        case ICF_CC_MMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MMS_RESP");
            break;

        case ICF_CC_DMS_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_DMS_RESP");
            break;

        case ICF_CC_SSA_CALL_TERMINATE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_TERMINATE_REQ");
            break;

        case ICF_CC_SSA_REMOTE_REJECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REMOTE_REJECT");
            break;

        case ICF_CC_SSA_REDIRECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REDIRECT");
            break;

        case ICF_CC_PA_CALL_TERMINATE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_TERMINATE_REQ");
            break;

        case ICF_CC_SSA_CALL_CANCEL_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CANCEL_RESP");
            break;

        case ICF_CC_PA_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_SSA_CALL_TERMINATE_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_TERMINATE_RESP");
            break;

        case ICF_CC_SSA_OFFER_CHANGE:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_OFFER_CHANGE");
            break;

        case ICF_CC_CALL_MUTE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CALL_MUTE_REQ");
            break;

        case ICF_CC_CALL_UNMUTE_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_CALL_UNMUTE_REQ");
            break;

        case ICF_CC_MM_ERR_IND:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MM_ERR_IND");
            break;

        case ICF_CC_TIMER_EXPIRY:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_TIMER_EXPIRY");
            break;

        case ICF_CC_SSA_SETUP_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_SETUP_REQ");
            break;

        case ICF_CC_PA_ALERT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_ALERT");
            break;

        case ICF_CC_PA_CALL_CONNECT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_CONNECT");
            break;

        case ICF_CC_MEDIA_PARAM_PRACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MEDIA_PARAM_PRACK");
            break;

        case ICF_CC_ALERT_PRACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_ALERT_PRACK");
            break;

        case ICF_CC_SSA_CONNECT_ACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CONNECT_ACK");
            break;

        case ICF_CC_SSA_CALL_CANCEL_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_CANCEL_REQ");
            break;

        case ICF_CC_GENERIC_ERROR:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_GENERIC_ERROR");
            break;

        case ICF_CC_FORCEFUL_CLEAR:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_FORCEFUL_CLEAR");
            break;

ICF_HANDLE_REINVITE_START
		case ICF_CC_SSA_REINVITE_ACK:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_REINVITE_ACK");
            break;


		case ICF_CC_PA_CALL_MODIFY_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_MODIFY_REQ");
            break;

		case ICF_CC_PA_CALL_MODIFY_RSP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_CALL_MODIFY_RSP");
            break;

		case ICF_CC_MEDIA_UPDATE_RESP:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_MEDIA_UPDATE_RESP");
            break;
ICF_HANDLE_REINVITE_END

        case ICF_CC_INVALID_EVENT:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_INVALID_EVENT");
            break;
        
        case ICF_CC_PA_EARLY_CALL_CFM:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_EARLY_CALL_CFM");
            break;
           
        case ICF_CC_PA_MEDIA_CONNECT_REQ:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_PA_MEDIA_CONNECT_REQ");
            break;

        case ICF_CC_SSA_CALL_PROGRESS:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_SSA_CALL_PROGRESS");
            break;

        default:
            icf_port_strcpy(p_event_str,
                    (icf_uint8_t *)"ICF_CC_INVALID_EVENT");
            break;

    } /* end of switch case for event_val */
} /* end of icf_cc_event_to_str() */
#endif /* end of #ifdef ICF_TRACE_ENABLE */


#if 0
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
        OUT icf_uint8_t *p_mode_str)
{
    switch(mode_val)
    {
        case ICF_MEDIA_MODE_INVALID:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INVALID");
            break;

        case ICF_MEDIA_MODE_INACTIVE:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INACTIVE");
            break;

        case ICF_MEDIA_MODE_SEND_ONLY:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_SEND_ONLY");
            break;

        case ICF_MEDIA_MODE_RECV_ONLY:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_RECV_ONLY");
            break;

        case ICF_MEDIA_MODE_SEND_RECV:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_SEND_RECV");
            break;

        default:
            icf_port_strcpy(p_mode_str,
                    (icf_uint8_t *)"ICF_MEDIA_MODE_INVALID");
            break;

    } /* end of switch case for mode_val */
} /* end of icf_cc_media_to_str() */

#endif 


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
        OUT icf_uint8_t *p_module_str)
{
    switch(module_id)
    {
        case ICF_INT_MODULE_INVALID:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_INVALID");
            break;
#ifdef CC_UT
        case ICF_INT_MODULE_MRM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_MRM");
            break;

        case ICF_INT_MODULE_CM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CM");
            break;
#endif

        case ICF_INT_MODULE_CC:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CC");
            break;

#ifdef CC_UT            
        case ICF_INT_MODULE_DBM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_DBM");
            break;

        case ICF_INT_MODULE_SM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SM");
            break;

        case ICF_INT_MODULE_REGM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_REGM");
            break;

        case ICF_INT_MODULE_CFG:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CFG");
            break;

        case ICF_INT_MODULE_SSA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SSA");
            break;

        case ICF_INT_MODULE_SIP_UA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SIP_UA");
            break;

        case ICF_INT_MODULE_SIP_STACK:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SIP_STACK");
            break;

        case ICF_INT_MODULE_MMI:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_MMI");
            break;

        case ICF_INT_MODULE_SCM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_SCM");
            break;

        case ICF_INT_MODULE_ADM:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_ADM");
            break;
#endif

        case ICF_INT_MODULE_CHR:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CHR");
            break;

#ifdef CC_UT            
        case ICF_INT_MODULE_RNW:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_RNW");
            break;

        case ICF_INT_MODULE_TWC:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_TWC");
            break;

        case ICF_INT_MODULE_CTA:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CTA");
            break;

        case ICF_INT_MODULE_CTU:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CTU");
            break;

        case ICF_INT_MODULE_CFW:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_CFW");
            break;
#endif

        default:
            icf_port_strcpy(p_module_str,
                    (icf_uint8_t *)"ICF_INT_MODULE_INVALID");
            break;
    } /* end of switch case for module_id */
} /* end of icf_cc_module_to_str() */
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
        OUT icf_uint8_t *p_timer_str)
{
    switch(timer_type)
    {
		case ICF_TIMER_TYPE_INFO:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_INFO");
            break;

        case ICF_TIMER_TYPE_SETUP:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_SETUP");
            break;

        case ICF_TIMER_TYPE_ALERTING:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_ALERTING");
            break;

        case ICF_TIMER_TYPE_MEDIA:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_MEDIA");
            break;

        case ICF_TIMER_TYPE_RELEASE:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"ICF_TIMER_TYPE_RELEASE");
            break;

        default:
            icf_port_strcpy(p_timer_str,
                    (icf_uint8_t *)"Invalid timer type");
            break;

    } /* end of switch-case for timer_type */
} /* end of icf_cc_timer_to_str() */
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
        IN icf_trace_id_t trace_id)
{
   icf_trace_data_st   trace_data=      {{0,0},0,{0},0,{{0,"\0"}},{0,"\0"},0}; 
    
   switch(trace_id)
    {
        case ICF_TRACE_FSM_TRIGGERED:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_state);
            trace_data.int_val[2] =
                (icf_uint32_t)(p_cc_pdb->current_event);

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_cc_state_to_str(p_cc_pdb->p_call_ctx->call_state,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            icf_cc_event_to_str(p_cc_pdb->current_event,
                    trace_data.string_val[1].str);
            trace_data.string_val[1].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[1].str);
            break;

        case ICF_TRACE_CALL_DIR:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"Incoming");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"Outgoing");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_MEDIA_SETUP_TYPE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_MEDIA_SETUP_TYPE))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"Non Fast Start");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"Fast Start");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_MEDIA_COMMITTED:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->p_call_ctx->media_committed)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_SDP_IN_PEER_MSG:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->sdp_present)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_PREVIOUS_STATE:
        case ICF_TRACE_SET_PREVIOUS_STATE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->prev_state);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_state_to_str(p_cc_pdb->p_call_ctx->prev_state,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_CURRENT_STATE:
        case ICF_TRACE_SET_CURRENT_STATE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_state);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_state_to_str(p_cc_pdb->p_call_ctx->call_state,
                    trace_data.string_val[0].str);

            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_NEXT_STATE:
        case ICF_TRACE_SET_NEXT_STATE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->next_state);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_state_to_str(p_cc_pdb->p_call_ctx->next_state,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_MEDIA_MODE:
        case ICF_TRACE_SET_MEDIA_MODE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
/*
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_media_to_str(p_cc_pdb->p_call_ctx->media_mode,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
*/
            break;

        case ICF_TRACE_SET_MODE_TO_BE_APP:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
/*
            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_media_to_str(
                    p_cc_pdb->p_call_ctx->media_mode_to_be_applied,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
*/
            break;

        case ICF_TRACE_SET_RELEASE_CAUSE:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->release_cause);

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_MEDIA_ONGOING:
        case ICF_TRACE_SET_MEDIA_ONGOING:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->media_ongoing);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_module_to_str(p_cc_pdb->p_call_ctx->media_ongoing,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_SIG_ONGOING:
        case ICF_TRACE_SET_SIG_ONGOING:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->signalling_ongoing);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_module_to_str(
                    p_cc_pdb->p_call_ctx->signalling_ongoing,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;


        case ICF_TRACE_ONGOING_EVENT:
        case ICF_TRACE_SET_ONGOING_EVENT:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->ongoing_event);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_event_to_str(p_cc_pdb->current_event,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_INBAND_INFO:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->p_call_ctx->inband_info_present)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_PRACK_REQD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->prack_reqd)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_MEDIA_TO_BE_SENT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->media_to_be_sent)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_MEDIA_MODIFY_SENT:
/*
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] = p_cc_pdb->p_call_ctx->rem_media_mode;
            trace_data.int_val[1] = p_cc_pdb->mms_type;
            trace_data.int_val[2] = p_cc_pdb->p_call_ctx->media_mode;
            trace_data.int_val[3] = 
                p_cc_pdb->p_call_ctx->media_mode_to_be_applied;
*/
            

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->mms_sent)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;


        case ICF_TRACE_EVENT_PA_CONNECT:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_CALL_CONNECT))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_EVENT_PA_TERM_REQ:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_PA_CALL_TERM_REQ))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_EVENT_MM_ERR_IND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_MM_ERR_IND))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t)icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_EVENT_OFFER_CHANGE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_EVENT_INT_ERR:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_INT_ERR))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;
#if 0
        case ICF_TRACE_EVENT_CALL_MUTE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_CALL_MUTE))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_EVENT_CALL_UNMUTE:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                        ICF_PENDING_EVENT_CALL_UNMUTE))
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"TRUE");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FALSE");
            }
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);
            break;
#endif
        case ICF_TRACE_TIMER_EXPIRED:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)p_cc_pdb->timer_type;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_timer_to_str(p_cc_pdb->timer_type,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_START_TIMER:
        case ICF_TRACE_STOP_TIMER:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)p_cc_pdb->timer_type;

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            icf_cc_timer_to_str(p_cc_pdb->timer_type,
                    trace_data.string_val[0].str);
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_CALL_STATUS_PA_CLR:
        case ICF_TRACE_CALL_STATUS_MM_CLR:
        case ICF_TRACE_CALL_STATUS_NW_CLR:
        case ICF_TRACE_CALL_STATUS_ALL_CLR:
        case ICF_TRACE_REL_TYPE_PA_INIT:
        case ICF_TRACE_REL_TYPE_MM_INIT:
        case ICF_TRACE_REL_TYPE_NW_INIT:
        case ICF_TRACE_REL_TYPE_TIMER_INIT:
        case ICF_TRACE_REL_TYPE_INT_ERR:
        case ICF_TRACE_CALL_MUTE_SET:
        case ICF_TRACE_CALL_MUTE_RESET:
        case ICF_TRACE_CALL_FAIL_NO_SDP:
        case ICF_PA_MEDIA_CONNECT:
        case ICF_PA_CALL_HOLD_IND:
        case ICF_PA_CALL_RESUME_IND:
        case ICF_PA_CONNECT_IND:
        case ICF_PA_TERMINATE_CALL_RESP:

            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_TRACE_SEND_MMI_REQ:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_ERROR_NONE ==
                    p_cc_pdb->p_internal_msg->msg_hdr.ecode)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"SUCCEEDED");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"FAILED");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

        case ICF_TRACE_DMS_REQ_PAYLOAD:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
            if (ICF_TRUE == p_cc_pdb->forceful_delete)
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"DMS type = Forceful");
            }
            else
            {
                icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"DMS type = Normal");
            }
            trace_data.string_val[0].str_len =
                (icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);
            break;

            
#if 0
        case ICF_TRACE_MMS_REQ_PAYLOAD:
            {
            icf_mmi_mms_request_st *p_mms_req = (icf_mmi_mms_request_st *)
                    p_cc_pdb->p_internal_msg->p_msg_data;
            
            icf_uint8_t count=0, string_count=0;

            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            /* PANKAJ trace_data.int_val[1] = (icf_uint32_t)
                (((icf_mmi_mms_request_st *)
                  (p_cc_pdb->p_internal_msg->p_msg_data))->modify_media_type);
*/

            icf_port_strcpy(trace_data.string_val[0].str,
                        (icf_uint8_t *)"STREAM MMS INFO:");
            trace_data.string_val[0].str_len =
                icf_port_strlen(trace_data.string_val[0].str);

			/*
            icf_port_strcpy(trace_data.string_val[1].str,
                        (icf_uint8_t *)"TYPE\t\tID\t\tSTATUS");
            trace_data.string_val[1].str_len =
                icf_port_strlen(trace_data.string_val[1].str);
				*/

            string_count = 1;

            if(ICF_MMI_MMS_ACTION_ADD == 
                (ICF_MMI_MMS_ACTION_ADD & p_mms_req->bit_mask))
            {
                icf_uint8_t stream_type[20];

                for(count = 0;count < p_mms_req->p_add_media->count;count++)
                {
                    p_cc_pdb->stream_id = 
                            p_mms_req->p_add_media->stream_id[count];

                    icf_cc_id_to_type_str(p_cc_pdb,stream_type);

                    icf_port_sprintf(trace_data.string_val[string_count].str,
                         "%s----%d----ADD",
                         stream_type,p_mms_req->p_add_media->stream_id[count]);
                        
                    trace_data.string_val[string_count].str_len =
                        icf_port_strlen(trace_data.string_val[string_count].str);

                    string_count++;
                }
            }
            
            if(ICF_MMI_MMS_ACTION_DELETE == 
                (ICF_MMI_MMS_ACTION_DELETE & p_mms_req->bit_mask))
            {
                icf_uint8_t stream_type[20];

                for(count = 0;count < p_mms_req->p_delete_media->count;count++)
                {
                    p_cc_pdb->stream_id = 
                            p_mms_req->p_delete_media->stream_id[count];

                    icf_cc_id_to_type_str(p_cc_pdb,stream_type);

                    icf_port_sprintf(trace_data.string_val[string_count].str,
                         "%s----%d----DELETE",
                         stream_type,p_mms_req->p_delete_media->stream_id[count]);
                        
                    trace_data.string_val[string_count].str_len =
                        icf_port_strlen(trace_data.string_val[string_count].str);

                    string_count++;
                }
            }

            if(ICF_MMI_MMS_ACTION_MODIFY == 
                (ICF_MMI_MMS_ACTION_MODIFY & p_mms_req->bit_mask))
            {
                icf_uint8_t stream_type[20];

                for(count = 0;count < p_mms_req->modify_stream_count;count++)
                {
                    icf_cc_stream_type_to_str(
                        (icf_uint8_t)p_mms_req->modify_media[count].stream_type,
                        stream_type);

                    icf_port_sprintf(trace_data.string_val[string_count].str,
                         "%s----%d----MODIFY",
                         stream_type,p_mms_req->modify_media[count].stream_id);
                        
                    trace_data.string_val[string_count].str_len =
                        icf_port_strlen(trace_data.string_val[string_count].str);

                    string_count++;
                }
            }

            trace_data.number_of_string_values = string_count;
            break;
            }
#endif
		case ICF_TRACE_CMS_REQ_SENT:
			{
				icf_uint8_t	counter = 0;
				icf_uint8_t	string_count = 0;
				icf_stream_sdp_info_st	*p_stream_sdp_data = ICF_NULL;
				icf_internal_list_st	*p_stream_sdp_info_node = ICF_NULL;
				/*icf_uint8_t stream_type[20];*/

				trace_data.int_val[counter] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
				p_stream_sdp_info_node = p_cc_pdb->p_call_ctx ->
												p_stream_sdp_info; 
				counter = 1;
				while(ICF_NULL != p_stream_sdp_info_node)
				{
					p_stream_sdp_data = (icf_stream_sdp_info_st*)
										(p_stream_sdp_info_node->p_payload);
					if (p_stream_sdp_data->stream_status & ICF_MEDIA_STREAM_ACTIVE)
					{
/*						icf_cc_stream_type_to_str(
                        (icf_uint8_t)p_stream_sdp_data->stream_type,
                        stream_type);

						icf_port_sprintf(trace_data.string_val[string_count].str,
                         "stream_type:%s--stream_id:%d",
                         stream_type,p_stream_sdp_data->stream_id);
                        
						trace_data.string_val[string_count].str_len =
                        icf_port_strlen(trace_data.string_val[string_count].str);

						string_count++;
*/
						trace_data.int_val[counter] =
						(icf_uint32_t)(p_stream_sdp_data->stream_type);

						++counter;
					}
					p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
				}
				trace_data.num_of_integer_values = counter;
				trace_data.number_of_string_values = string_count;
			}
			
			break;

		case ICF_TRACE_CMS_RESP_RECVD:
			{
				icf_mmi_cms_resp_st		*p_mmi_cms_resp = ICF_NULL;
				icf_uint8_t				counter = 0;
				icf_uint8_t				string_count = 0;

				p_mmi_cms_resp = (icf_mmi_cms_resp_st *)
						p_cc_pdb->p_internal_msg->p_msg_data;
				trace_data.int_val[counter++] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				if (ICF_FALSE == p_mmi_cms_resp->result)
				{
					icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
                        "Failure in CMS Response");

					trace_data.string_val[string_count].str_len =
                        (icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
					string_count++;
				}        
				else
				{
					/*trace_data.int_val[counter++] =
					(icf_uint32_t)p_mmi_cms_resp->error_cause ;*/
					icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
                         "stream_id:%d--status:%d",
	                     p_mmi_cms_resp->stream_status[p_cc_pdb->count].stream_id,
						 (icf_uint32_t)(p_mmi_cms_resp->
						 stream_status[p_cc_pdb->count].status_success));
                        
					trace_data.string_val[string_count].str_len =
                        (icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);

					string_count++;
				}
				trace_data.num_of_integer_values = counter;
				trace_data.number_of_string_values = string_count;
			}
			break;

		case ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED:	
			{
				icf_uint8_t		string_count = 0;
				icf_uint8_t		counter = 0;

				trace_data.int_val[counter++] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				if (ICF_NULL == p_cc_pdb->p_generic_ptr_2)
				{
					icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
                        "Validation Successul");

					trace_data.string_val[string_count].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[string_count].str);
					string_count++;

				}
				else
				{
					icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
                        (const icf_int8_t *) "Validation Unsuccessul, Mismatch at stream_id:%d",
						(icf_uint32_t)((icf_stream_sdp_info_st*)
						(p_cc_pdb->p_generic_ptr_2))->stream_id);

					trace_data.string_val[string_count].str_len =
                        (icf_uint16_t)icf_port_strlen(trace_data.string_val[string_count].str);
					string_count++;
				}
				trace_data.num_of_integer_values = counter;
				trace_data.number_of_string_values = string_count;
			}
			break;

		case ICF_TRACE_MMS_RESP_RECVD:
			{
				icf_mmi_mms_resp_st     *p_mms_resp = ICF_NULL;
				icf_uint8_t				string_count = 0;
				icf_uint8_t				counter = 0;

				p_mms_resp =
				(icf_mmi_mms_resp_st*)p_cc_pdb->p_internal_msg->p_msg_data;

				trace_data.int_val[counter++] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				if (ICF_FALSE == p_mms_resp->result)
				{
					icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
                        "Failure in MMS Response");

					trace_data.string_val[string_count].str_len =
                        (icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
					string_count++;
				}
				else
				{
					/* result was success in mms response.*/
					if (ICF_FALSE == p_cc_pdb->flag)
					{
						/* Stream was modified.*/
						if ((p_mms_resp->modified_stream_resp
							[p_cc_pdb->count]).status_success)
						{
							icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
								(const icf_int8_t *) "Modification Accepted for stream id:%d",
								(p_mms_resp->modified_stream_resp
								[p_cc_pdb->count]).stream_id);

							trace_data.string_val[string_count].str_len =
							(icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
						}
						else
						{
							icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
							(const icf_int8_t *) "Modification Rejected for stream id:%d",
							(p_mms_resp->modified_stream_resp[p_cc_pdb->count]).stream_id);

							trace_data.string_val[string_count].str_len =
								(icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
						}
						string_count++;
					}
					else
					{
						/* Stream was Added.*/
						if ((p_mms_resp->added_stream_resp[p_cc_pdb->count]).status_success)
						{
							icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
							"Modification Accepted for stream id:%d",
							(p_mms_resp->added_stream_resp[p_cc_pdb->count]).stream_id);

							trace_data.string_val[string_count].str_len =
							(icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
							
						}
						else
						{
							icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
							"Modification Rejected for stream id:%d",
							(p_mms_resp->added_stream_resp[p_cc_pdb->count]).stream_id);

							trace_data.string_val[string_count].str_len =
								(icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);
							
						}
						string_count++;
					}
				}
				trace_data.num_of_integer_values = counter;
				trace_data.number_of_string_values = string_count;
			}
			break;
	
		case ICF_TRACE_APP_REJECTED_STREAMS:
			{
				icf_uint8_t				string_count = 0;
				icf_uint8_t				counter = 0;
				icf_stream_sdp_info_st	*p_stream_sdp_info =
										p_cc_pdb->p_generic_ptr_2; 

				trace_data.int_val[counter++] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				trace_data.int_val[counter++] =
					(icf_uint32_t)(p_cc_pdb->count);

				icf_port_sprintf((icf_int8_t *)trace_data.string_val[string_count].str,
				"Application rejected stream_type:%d",
				(icf_uint32_t)(p_stream_sdp_info->stream_type));

				trace_data.string_val[string_count].str_len =
					(icf_uint16_t) icf_port_strlen(trace_data.string_val[string_count].str);

				string_count++;

				trace_data.num_of_integer_values = counter;
				trace_data.number_of_string_values = string_count;
				
			}
			break;

		case ICF_TRACE_NO_ACTIVE_STREAMS_LEFT:
			{

				trace_data.int_val[0] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
				"All streams have been rejected.");

				trace_data.string_val[0].str_len =
					(icf_uint16_t) icf_port_strlen(trace_data.string_val[0].str);

				trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
			}
			break;

		case ICF_TRACE_OFFER_CHANGE_FAILURE:
			{
				trace_data.int_val[0] =
					(icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

				icf_port_sprintf((icf_int8_t *)trace_data.string_val[0].str,
				"Offer has been completetely rejected by remote");

				trace_data.string_val[0].str_len = (icf_uint16_t)
					icf_port_strlen(trace_data.string_val[0].str);

				trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
				trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
				
			}
		break;

        case ICF_INTERFACE_FAILURE:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)
                  p_cc_pdb->p_call_ctx->line_id;
            trace_data.int_val[2] = (icf_uint32_t)
                 p_cc_pdb->p_internal_msg->msg_hdr.msg_id;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;


        case ICF_PA_INCOMING_CALL_IND:
            trace_data.num_of_integer_values = ICF_FOUR_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)p_cc_pdb->p_call_ctx->call_id;
            trace_data.int_val[2] =
                (icf_uint32_t)p_cc_pdb->p_call_ctx->call_waiting_call;
            trace_data.int_val[3] =
                (icf_uint32_t)p_cc_pdb->p_call_ctx->remote_aserted_address;
            
			trace_data.number_of_string_values = ICF_ONE_STRING_PRESENT;
                        
            icf_port_strcpy(trace_data.string_val[0].str,
                    (icf_uint8_t*)p_cc_pdb->p_call_ctx->
                                remote_party_addr.addr_val.str );
            trace_data.string_val[0].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[0].str);
            
            if (ICF_NULL != p_cc_pdb->p_call_ctx->remote_party_name.str_len)
			{
			    icf_port_strcpy(trace_data.string_val[1].str,
					(icf_uint8_t*)p_cc_pdb->p_call_ctx->
					remote_party_name.str );
				trace_data.string_val[1].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[1].str);

				trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
			}
            break;


        case ICF_PA_ALERT_USER_IND:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)
                p_cc_pdb->p_call_ctx->inband_info_present;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_PA_CALL_TERM_IND:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)
                 p_cc_pdb->p_call_ctx->release_cause;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

        case ICF_PA_CALL_TERM_IND_NO_CTX:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_glb_pdb->p_recv_msg->hdr.call_id);

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

            
#ifdef ICF_SERVICE_ENABLE /* this apis is not supported in ICF */
        case ICF_PA_CALL_REDIRECTION_IND:
            trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                  (icf_uint8_t*)((icf_cc_pa_call_redirection_ind_st*)
                   p_cc_pdb->p_internal_msg->p_msg_data)->redirected_party_addr.
                   addr_val.str);
            
            trace_data.string_val[0].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[0].str);
            
            icf_port_strcpy(trace_data.string_val[1].str,
                    (icf_uint8_t*)((icf_cc_pa_call_redirection_ind_st*)
                   p_cc_pdb->p_internal_msg->p_msg_data)->redirected_party_addr.
                   addr_val.str);

            trace_data.string_val[1].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[1].str);

            break;
#endif

        case ICF_UNEXPECTED_EVENT:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)
                p_cc_pdb->current_event;
            trace_data.int_val[2] = (icf_uint32_t)
                p_cc_pdb->p_call_ctx->call_state;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;


        case ICF_TRANSPORT_ADDRESS:
            trace_data.num_of_integer_values = ICF_THREE_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] = (icf_uint32_t)
                  p_cc_pdb->p_call_ctx->remote_party_addr.addr_type;
            trace_data.int_val[2] = (icf_uint32_t)
                  p_cc_pdb->p_call_ctx->local_address.addr_type;

            trace_data.number_of_string_values = ICF_TWO_STRING_PRESENT;
            icf_port_strcpy(trace_data.string_val[0].str,
                    (icf_uint8_t*)p_cc_pdb->p_call_ctx->remote_party_addr.
                                addr_val.str );
            trace_data.string_val[0].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[0].str);
            
            icf_port_strcpy(trace_data.string_val[1].str,
                    (icf_uint8_t*)p_cc_pdb->p_call_ctx->local_address.
                                addr_val.str );
            trace_data.string_val[1].str_len =(icf_uint16_t)
                icf_port_strlen(trace_data.string_val[1].str);
            break;

        case ICF_TRACE_SRVC_TRIG_STAT_RET:
            trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
            trace_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);
            trace_data.int_val[1] =
                (icf_uint32_t)(p_cc_pdb->p_internal_msg->msg_hdr.status); 
            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;         
            break;

        default:
            trace_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_TRACE(p_cc_pdb->p_glb_pdb, trace_id, &trace_data)
} /* end of icf_cc_trace() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_app_trace
 *
 * DESCRIPTION:     This function is used by CC for trace handling.
 *
 ******************************************************************************/
icf_return_t icf_cc_app_trace(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN icf_trace_id_t trace_id)
{
    icf_trace_data_st        *p_trace_data = ICF_NULL;
    icf_call_connect_data_st *p_call_connect_data = ICF_NULL;
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_list_st     *p_sdp_info = ICF_NULL;
    /*Fix for CSR1-5249829 */
    ret_val = icf_cmn_check_app_trace(p_cc_pdb->p_glb_pdb,
                                        trace_id,
					p_cc_pdb->p_call_ctx->p_trace_criteria_data);
    if( ICF_FAILURE == ret_val)
    {
        return ret_val;
    }
    else
    {
    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
           ICF_PORT_SIZEOF(icf_trace_data_st), 
           ICF_MEM_COMMON,
           p_trace_data,
           ICF_RET_ON_FAILURE,
           p_cc_pdb->p_ecode,
           ret_val)

    p_cc_pdb->p_glb_pdb->p_glb_data->app_trace_info.app_id =
		p_cc_pdb->p_call_ctx->app_id;

    p_cc_pdb->p_glb_pdb->p_glb_data->app_trace_info.call_id =
		p_cc_pdb->p_call_ctx->call_id;

    switch(trace_id)
    {
        
        case ICF_TRACE_CALL_CONNECTED:
	    {
            p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
	       
           
           ICF_MEMGET(p_cc_pdb->p_glb_pdb,
               ICF_PORT_SIZEOF(icf_call_connect_data_st),
               ICF_MEM_COMMON,
               p_call_connect_data,
               ICF_DONOT_RET_ON_FAILURE,
               p_cc_pdb->p_ecode,
               ret_val)
            
            p_trace_data->p_var_data = p_call_connect_data;

            p_cc_pdb->p_generic_ptr_1 = p_call_connect_data;
	    icf_cc_create_call_connect_data(p_cc_pdb);

            break;
        }
       
         case ICF_TRACE_CREATE_CALL_REQ:
    
             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->line_id);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_type);

             p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;

             icf_port_strcpy(p_trace_data->string_val[0].str,
					p_cc_pdb->p_call_ctx->local_address.addr_val.str);
             p_trace_data->string_val[0].str_len =(icf_uint16_t)
                    icf_port_strlen(p_trace_data->string_val[0].str);

             icf_port_strcpy(p_trace_data->string_val[1].str,
					p_cc_pdb->p_call_ctx->remote_party_addr.addr_val.str);
             p_trace_data->string_val[1].str_len =(icf_uint16_t)
             		icf_port_strlen(p_trace_data->string_val[1].str);
 
             break;


         case ICF_TRACE_INCOMING_CALL_IND:

             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->line_id);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_type);

             p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;

             icf_port_strcpy(p_trace_data->string_val[0].str,
                       p_cc_pdb->p_call_ctx->remote_party_addr.addr_val.str);
             p_trace_data->string_val[0].str_len =(icf_uint16_t)
                    icf_port_strlen(p_trace_data->string_val[0].str);

             icf_port_strcpy(p_trace_data->string_val[1].str,
                       p_cc_pdb->p_call_ctx->local_address.addr_val.str);
             p_trace_data->string_val[1].str_len =
                        (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[1].str);

             break;
	
         case ICF_TRACE_EARLY_INC_CALL_IND:
   
             p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;

             p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;
             
             icf_port_strcpy(p_trace_data->string_val[0].str,
                    p_cc_pdb->p_call_ctx->remote_party_addr.addr_val.str);
             p_trace_data->string_val[0].str_len =(icf_uint16_t)
                    icf_port_strlen(p_trace_data->string_val[0].str);

             icf_port_strcpy(p_trace_data->string_val[1].str,
                     p_cc_pdb->p_call_ctx->local_address.addr_val.str);
             p_trace_data->string_val[1].str_len =(icf_uint16_t)
                        icf_port_strlen(p_trace_data->string_val[1].str);

             break;

       case ICF_TRACE_EARLY_INC_CALL_CFM:
   
             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->line_id);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->result);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;


       case ICF_TRACE_STREAM_CREATE:
       case ICF_TRACE_STREAM_MODIFY:
       {
           p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;

            p_trace_data->int_val[0] = p_cc_pdb->stream_type;
            p_trace_data->int_val[1] = p_cc_pdb->result;

            if( ICF_SUCCESS == p_cc_pdb->result)
            {
               icf_uint8_t  port[10];

                p_trace_data->p_var_data = 
                   (icf_list_st *)p_cc_pdb->p_codec_info_list;

               icf_cmn_convert_transport_addr_to_ip_string
                   (&(p_cc_pdb->local_rtp_addr.addr),
                    p_trace_data->string_val[0].str);

                icf_port_sprintf((icf_int8_t *)port,":%d",p_cc_pdb->local_rtp_addr.port_num);

                icf_port_strcat(p_trace_data->string_val[0].str,port);
                p_trace_data->string_val[0].str_len =
                    (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[0].str);

                icf_cmn_convert_transport_addr_to_ip_string
                   (&(p_cc_pdb->remote_rtp_addr.addr),
                    p_trace_data->string_val[1].str);

                icf_port_sprintf((icf_int8_t *) port,(const icf_int8_t *)":%d",p_cc_pdb->remote_rtp_addr.port_num);

                icf_port_strcat(p_trace_data->string_val[1].str,port);
                p_trace_data->string_val[1].str_len =
                    (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[1].str);
            }
           break;
       }
       case ICF_TRACE_LOCAL_SDP_OFFER:
       {
            p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
             
            icf_cc_create_sdp_info_list(p_cc_pdb, ICF_CC_LOCAL_SDP_OFFER);

            p_sdp_info = p_cc_pdb->p_list_1; 
            p_trace_data->p_var_data = p_cc_pdb->p_list_1;
             
            break;
       }

       case ICF_TRACE_REMOTE_SDP_OFFER:
       {
             p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            icf_cc_create_sdp_info_list(p_cc_pdb, ICF_CC_REMOTE_SDP_OFFER);

            p_sdp_info = p_cc_pdb->p_list_1;
            p_trace_data->p_var_data = p_cc_pdb->p_list_1; 
             
            break;
       }

       case ICF_TRACE_LOCAL_SDP_ANSWER:
       {
            p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            icf_cc_create_sdp_info_list(p_cc_pdb, ICF_CC_LOCAL_SDP_ANSWER);

            p_sdp_info = p_cc_pdb->p_list_1;
            p_trace_data->p_var_data = p_cc_pdb->p_list_1;
 
            break;
       }

       case ICF_TRACE_REMOTE_SDP_ANSWER:
       {
            p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
            p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

            icf_cc_create_sdp_info_list(p_cc_pdb, ICF_CC_REMOTE_SDP_ANSWER);

            p_sdp_info = p_cc_pdb->p_list_1;
            p_trace_data->p_var_data = p_cc_pdb->p_list_1;

            break;
       }

       case ICF_TRACE_ALERT_FROM_APP:
   
             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->inband_info_present);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->early_media);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;



       case ICF_TRACE_ALERT_TO_APP :

             p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->inband_info_present);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

    case ICF_TRACE_CALL_TERMINATION_INITIATED:
                  
             p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;
             p_trace_data->int_val[0] = p_cc_pdb->call_clear_dir;

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;
    
   case ICF_TRACE_CALL_MODIFY_IND:

             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->stream_type);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->call_hold_status);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;


    case ICF_TRACE_MEDIA_CONNECT_FROM_APP :
    case ICF_TRACE_MEDIA_CONNECT_TO_APP :
    case ICF_TRACE_CALL_MODIFY_REQ:
    case ICF_TRACE_CALL_MODIFY_RESP:
    case ICF_TRACE_CALL_MODIFY_CFM:
    case ICF_TRACE_DMS_IND:
    case ICF_TRACE_SETUP_TIMER_EXPIRE:
    case ICF_TRACE_ALERTING_TIMER_EXPIRE:
    case ICF_TRACE_RELEASE_TIMER_EXPIRE:
    case ICF_TRACE_MEDIA_TIMER_EXPIRE:
    case ICF_TRACE_STOP_FAX_TO_APP:
    case ICF_TRACE_STOP_FAX_FROM_APP:
    case ICF_TRACE_INFO_TIMER_EXPIRE:
    case ICF_TRACE_CALL_WAITING_IND:
             p_trace_data->num_of_integer_values = ICF_NO_INT_PRESENT;
             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;
             break;

     case ICF_TRACE_START_FAX_TO_APP:

            p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;

             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->fax_type);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

    case ICF_TRACE_START_FAX_FROM_APP:
             if(ICF_FAX_PT_CALL == p_cc_pdb->p_call_ctx->fax_type)
             {
                 p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
                 p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->fax_type);
                 p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->pref_codec);
             }
             else
             {
                  p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;
                  p_trace_data->int_val[0] =
                      (icf_uint32_t)(p_cc_pdb->p_call_ctx->fax_type);
             }

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;


   case ICF_TRACE_START_FAX_RESP_TO_APP:
   case ICF_TRACE_STOP_FAX_RESP_TO_APP:

             p_trace_data->num_of_integer_values = ICF_TWO_INT_PRESENT;
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->result);
             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->resp_code);

             p_trace_data->number_of_string_values = ICF_NO_STRING_PRESENT;

             break;

   case ICF_TRACE_CALL_INITIATED_FOR_INC_REFER:
             
             p_trace_data->num_of_integer_values = ICF_THREE_INT_PRESENT;

             p_trace_data->int_val[0] = p_cc_pdb->p_call_ctx->line_id;

             p_trace_data->int_val[1] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_type);
             
             if(ICF_NULL != p_cc_pdb->p_call_ctx->p_replaces_header)
             { 
                  p_trace_data->int_val[2] = 1;
             }
             else
             {
                  p_trace_data->int_val[2] = 0;
             }

             p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;

             icf_port_strcpy(p_trace_data->string_val[0].str,
                                        p_cc_pdb->p_call_ctx->local_address.addr_val.str);
             p_trace_data->string_val[0].str_len =
                    (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[0].str);

             icf_port_strcpy(p_trace_data->string_val[1].str,
                                        p_cc_pdb->p_call_ctx->remote_party_addr.addr_val.str);
             p_trace_data->string_val[1].str_len =
                        (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[1].str);

             break;



   case ICF_TRACE_PARTY_REPLACED_IND:

             p_trace_data->num_of_integer_values = ICF_ONE_INT_PRESENT;
             
             p_trace_data->int_val[0] =
                     (icf_uint32_t)(p_cc_pdb->p_call_ctx->replace_initiated_call_id);

             p_trace_data->number_of_string_values = ICF_TWO_STRING_PRESENT;

             icf_port_strcpy(p_trace_data->string_val[0].str,
		   p_cc_pdb->p_call_ctx->remote_party_addr.addr_val.str);

             p_trace_data->string_val[0].str_len =
                    (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[0].str);

             icf_port_strcpy(p_trace_data->string_val[1].str,
		  p_cc_pdb->p_call_ctx->remote_party_name.str);

             p_trace_data->string_val[1].str_len =
                    (icf_uint16_t) icf_port_strlen(p_trace_data->string_val[1].str);

             break;

    

    default:
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                 p_trace_data, ICF_MEM_COMMON,
                 p_cc_pdb->p_ecode, ret_val)
            return ICF_SUCCESS;

    } /* end of switch-case for trace_id */

    /* Finally, invoke the common macro for trace handling */
    ICF_APP_TRACE(p_cc_pdb->p_glb_pdb, trace_id, p_trace_data,
		p_cc_pdb->p_call_ctx->p_trace_criteria_data)

    switch(trace_id)
    {
          case ICF_TRACE_CALL_CONNECTED:

                ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                      p_call_connect_data, ICF_MEM_COMMON,
                      p_cc_pdb->p_ecode, ret_val)
                break;
          
          case ICF_TRACE_LOCAL_SDP_OFFER:
          case ICF_TRACE_REMOTE_SDP_OFFER:
          case ICF_TRACE_REMOTE_SDP_ANSWER:
          case ICF_TRACE_LOCAL_SDP_ANSWER:

                p_cc_pdb->p_list_1 = p_sdp_info;
                icf_cc_free_call_modify_data_list(p_cc_pdb);
                break;

    }

    ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
            p_trace_data, ICF_MEM_COMMON,
            p_cc_pdb->p_ecode, ret_val)

    return ret_val;
} /* end of icf_cc_app_trace() */
}

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
        IN icf_error_t ecode)
{
    icf_error_data_st                error_data;

    switch(ecode)
    {
		case ICF_ERROR_INVALID_TRNSPT_ADDR:
        case ICF_ERROR_CALL_FAIL_NO_SDP:
            error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
            error_data.int_val[0] =
                (icf_uint32_t)(p_cc_pdb->p_call_ctx->call_id);

            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;                  
			break;

        default:
            error_data.num_of_integer_values = ICF_NO_INT_PRESENT;

            error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
            break;

    } /* end of switch-case for ecode */

    /* Finally, invoke the common macro for error handling */
    ICF_SET_ECODE(p_cc_pdb->p_glb_pdb, &error_data, ecode, p_cc_pdb->p_ecode)
} /* end of icf_cc_error() */
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_current_timer
 *
 * DESCRIPTION:     This function stops any currently running timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_current_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id;
    icf_uint8_t                      ctr;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Iterate through the list of timers maintained by CC */
    for (ctr=0; ctr<ICF_CC_NUMBER_OF_TIMERS; ctr++)
    {
        timer_id = p_cc_pdb->p_call_ctx->timer_list.timer_id[ctr];

        /* check if the timer is running or not */
        if (ICF_TIMER_ID_INVALID != timer_id)
        {
            /* Non-zero id means that the timer is running, so stop it */
            ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, timer_id, p_cc_pdb->p_ecode, ret_val)

            if (ICF_SUCCESS == ret_val)
            {
                p_cc_pdb->timer_type = ctr;
                p_cc_pdb->timer_id = timer_id;

                ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_STOP_TIMER)

                /* reset the timer id in call context */
                p_cc_pdb->p_call_ctx->timer_list.timer_id[ctr] =
                    ICF_TIMER_ID_INVALID;
            }
        } /* end of if for timer_id */

    } /* end of for loop */

    /* free the memory allocated to any existing timer data block */
    if (ICF_NULL != p_cc_pdb->p_call_ctx->p_timer_data)
    {
      icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_cc_pdb->p_call_ctx->p_timer_data, p_cc_pdb->p_ecode);
        p_cc_pdb->p_call_ctx->p_timer_data = ICF_NULL;
    }


    if (ICF_NULL != p_cc_pdb->p_call_ctx->p_retry_after_timer_data &&
        ICF_TIMER_ID_INVALID !=  p_cc_pdb->p_call_ctx->retry_after_timer_id )
     {
        /* Non-zero id means that the timer is running, so stop it */
        ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->retry_after_timer_id, 
               p_cc_pdb->p_ecode, ret_val)

        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_cc_pdb->p_call_ctx->p_retry_after_timer_data,
                 p_cc_pdb->p_ecode);

        p_cc_pdb->p_call_ctx->p_retry_after_timer_data = ICF_NULL;
        p_cc_pdb->p_call_ctx->retry_after_timer_id = ICF_TIMER_ID_INVALID;
      }
    

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_stop_current_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_setup_timer
 *
 *DESCRIPTION:     This function starts Setup timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_setup_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_SETUP;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
       /* Now, start the Setup timer */
	if ( ICF_FALSE == p_cc_pdb->is_call_modify_timer)
	{
		ICF_PRINT(((icf_uint8_t*)"\n[CC]: Setup Timer duration: %d\n",p_config_data->setup_timer));
       		ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
            	p_config_data->setup_timer, p_cc_pdb->p_ecode, ret_val)
	}
	else
	{
		ICF_PRINT(((icf_uint8_t*)"\n[CC]: Setup TImer Duration: %d\n",ICF_CC_PA_CALL_MODIFY_SETUP_TIMER));
		ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
            	ICF_CC_PA_CALL_MODIFY_SETUP_TIMER, p_cc_pdb->p_ecode, ret_val)
	}
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->timer_list.timer_id[ICF_TIMER_TYPE_SETUP] =
            timer_id;

        p_cc_pdb->p_call_ctx->p_timer_data =
            (icf_void_t *)(p_timer_data);
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_SETUP;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
       icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_timer_data, p_cc_pdb->p_ecode); 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_setup_timer() */


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
        IN    icf_duration_t pa_timer)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;
    icf_duration_t                   timer_duration=0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* This will take care of stopping the Setup timer */
    ret_val = icf_cc_stop_current_timer(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {
        /* Get a timer block from DBM, this will be freed on clring call ctx */
       ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_ALERTING;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;
	
		if (ICF_TRUE == is_pa_timer)
		{
				timer_duration = pa_timer;
		}
		/* Fix for CSR 1-7480178: SPR 19827 */
		/* Check for call direction before fetching the Alerting timer value.
		 * For outgoing call, simply use the configured alerting_timer val.
		 * For incoming call, the timer value depends on service configuration.
		 */
		else if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR))
		{
			/* outgoing call: use the default alerting_timer value */
			#if 1 /*Eason, SPRID#090824353, outgoing call alert timer set to max*/
			timer_duration = 0x0fffffff;			
			#else
			timer_duration = p_cc_pdb->p_config_data->alerting_timer;
			#endif
		}
		else
		{	
			/* incoming call: use the value based on service on this line */
				ret_val = icf_dbm_get_alert_timer_dur(
								p_cc_pdb->p_glb_pdb,
								&timer_duration,
								p_cc_pdb->p_ecode);
		}
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* Now, start the Alert timer */
        ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                timer_duration, p_cc_pdb->p_ecode, ret_val)
		
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->timer_list.timer_id[ICF_TIMER_TYPE_ALERTING] =
            timer_id;

        p_cc_pdb->p_call_ctx->p_timer_data =
            (icf_void_t *)(p_timer_data);
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_ALERTING;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                p_timer_data, p_cc_pdb->p_ecode); 
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_alert_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_media_timer
 *
 * DESCRIPTION:     This function stops any timer if it is currently running
 *                  and starts the Modify Media timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_media_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* This will take care of stopping any currently running timer */
    ret_val = icf_cc_stop_current_timer(p_cc_pdb);

    if (ICF_SUCCESS == ret_val)
    {

        /* Get a timer block from DBM, this will be freed on clring call ctx */
        ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_MEDIA;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* Now, start the Media timer */
        ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                p_config_data->modify_media_timer, p_cc_pdb->p_ecode, ret_val)
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->timer_list.timer_id[ICF_TIMER_TYPE_MEDIA] =
            timer_id;

        p_cc_pdb->p_call_ctx->p_timer_data =
            (icf_void_t *)(p_timer_data);
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_MEDIA;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
    }
     /*
     * If an error occured in starting up the timer
     */ 
    else
    {
       icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                p_timer_data, p_cc_pdb->p_ecode); 
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_media_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_release_timer
 *
 * DESCRIPTION:     This function stops Setup/Alert/ModifyMedia timer if it is
 *                  currently running and starts the Release timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_release_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*Check if currently running timer is not release timer then stop the 
     * currently executing timer and start the release timer*/
    if(ICF_TIMER_ID_INVALID == 
           p_cc_pdb->p_call_ctx->timer_list.timer_id[ICF_TIMER_TYPE_RELEASE])
    {
        /* This will take care of stopping any currently running timer */
        ret_val = icf_cc_stop_current_timer(p_cc_pdb);

        if (ICF_SUCCESS == ret_val)
        {
            /* Get a timer block from DBM, this will be freed on clring call ctx
             */
            ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
                    (icf_pool_id_t)ICF_MEM_TIMER,
                    (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
            ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))
        }

        if (ICF_SUCCESS == ret_val)
        {
            /* fill the timer data structure */
            p_timer_data->timer_type = ICF_TIMER_TYPE_RELEASE;
            p_timer_data->module_id = ICF_INT_MODULE_CC;
            p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
            p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

            /* specify the timer duration using config data from DBM */
            ret_val = icf_dbm_get_module_glb_data(
                    p_cc_pdb->p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

            ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
        }

        if (ICF_SUCCESS == ret_val)
        {
            /* Now, start the Release timer */
            ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                    p_config_data->release_timer, p_cc_pdb->p_ecode, ret_val)
        }

        if (ICF_SUCCESS == ret_val)
        {
            /* save the timer id returned by macro */
            p_cc_pdb->p_call_ctx->timer_list.timer_id[ICF_TIMER_TYPE_RELEASE]
                = timer_id;

            p_cc_pdb->p_call_ctx->p_timer_data =
                (icf_void_t *)(p_timer_data);
            p_cc_pdb->timer_type = ICF_TIMER_TYPE_RELEASE;
            p_cc_pdb->timer_id = timer_id;
            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
        }
        /*
         * If an error occured in starting up the timer
         */ 
        else
        {
           icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                 p_timer_data, p_cc_pdb->p_ecode); 
        }
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_release_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_nw_clear_handler
 *
 * DESCRIPTION:     This function is invoked everytime the network clear bit is
 *                  set in call_context->call_clear_status.
 *
 ******************************************************************************/
icf_return_t icf_cc_nw_clear_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /*if timer expiry is current event and nw cleard bit is not set
     * then send forceful call clear to SSA  */
    if(ICF_CC_TIMER_EXPIRY == p_cc_pdb->current_event &&
        (ICF_NULL == ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED)))
    {
    /* Send forceful call clear to SSA */
        ret_val = icf_cc_send_ssa_force_clear(p_cc_pdb);
    }

    /* Set network clear bit */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_NW_CLEARED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_NW_CLR)

    /*
     * If PA clear and MM clear bit are also set and call state is not idle,
     * then invoke function that clears the entire call context
     */
    if ((ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_PA_CLEARED))
            && (ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_MM_CLEARED))
            && (ICF_CC_00_IDLE != ICF_CC_GET_CALL_STATE(p_cc_pdb)))
    {
        ret_val = icf_cc_clear_call_context(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_clear_handler
 *
 * DESCRIPTION:     This function is invoked everytime the PA clear bit is
 *                  set in call_context->call_clear_status.
 *
 ******************************************************************************/
icf_return_t icf_cc_pa_clear_handler(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set PA clear bit */
    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_PA_CLEARED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_PA_CLR)

    /*
     * If NW clear and MM clear bit are also set and call state is not idle,
     * then invoke function that clears the entire call context
     */
    if ((ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_NW_CLEARED))
            && (ICF_NULL != ICF_CC_CHECK_CALL_CLR_STATUS(p_cc_pdb,
                    ICF_CALL_CLEAR_STATUS_MM_CLEARED))
            && (ICF_CC_00_IDLE != ICF_CC_GET_CALL_STATE(p_cc_pdb)))
    {
        ret_val = icf_cc_clear_call_context(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_call_context
 *
 * DESCRIPTION:     This function does the call context clean up once the PA,
 *                  MM and NW interfaces are all cleared.
 *
 ******************************************************************************/
icf_return_t icf_cc_clear_call_context(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     dummy_retval = ICF_SUCCESS;
    icf_call_ctx_st		*p_tmp_call_ctxt = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * This will take care of stopping any currently running timer. Ideally,
     * there should not be any timer running at this point. It will also free
     * the memory allocated to timer data block
     */
    ret_val = icf_cc_stop_current_timer(p_cc_pdb);

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
        icf_cc_stop_load_call_dur_timer(p_cc_pdb);
#endif

    /* chk if there is in-dialog option if yes then clear state for the same */
    if (p_cc_pdb->p_call_ctx->common_bitmask & ICF_IN_DLG_OPTIONS_SEND ||
           p_cc_pdb->p_call_ctx->common_bitmask & ICF_IN_DLG_OPTIONS_RECV )
    {
        icf_uint8_t                 *p_msg_data = ICF_NULL;
        icf_internal_msg_st         internal_msg;
        internal_msg.msg_hdr.msg_id =
                ICF_CLEAR_SIC_CTX;
        internal_msg.msg_hdr.payload_length =
                sizeof(icf_sic_clear_ctx_st);
        internal_msg.msg_hdr.p_glb_pdb = p_cc_pdb->p_glb_pdb;
        internal_msg.msg_hdr.ecode = ICF_ERROR_UNDEFINED;
        internal_msg.msg_hdr.status = ICF_STATUS_CONTINUE;
        internal_msg.p_msg_data = ICF_NULL;
         
        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                internal_msg.msg_hdr.payload_length,
                ICF_MEM_COMMON,
                p_msg_data,
                ICF_RET_ON_FAILURE,
                p_cc_pdb->p_ecode,
                ret_val
                )
        internal_msg.p_msg_data = p_msg_data;
        ((icf_sic_clear_ctx_st *)(internal_msg.p_msg_data))->call_id = 
			p_cc_pdb->p_call_ctx->call_id;

        p_cc_pdb->p_call_ctx->common_bitmask_2 |= ICF_CC_CLEAR_OPTIONS_CTXT;

        ret_val = icf_sic_process_mesg(&internal_msg);
        /* if this was outgoing OPTIONS In dialog then abort the SSA txn
         * for incoming options this is done in SIC*/
        if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_IN_DLG_OPTIONS_SEND)
        {
           /* this is the best effrort no need to chk the return value */
           icf_cc_abort_options_trans(p_cc_pdb);
        }
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    (p_msg_data),
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)

        p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_IN_DLG_OPTIONS_SEND;
        p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_IN_DLG_OPTIONS_RECV;
    }

    /*Update the STATS*/
   
    /*if the call connect done flag was not set. Update stat for 
     * unsuccessfull call*/
    if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        /* Set the stat for unsuccessfull call */
        ICF_STAT(p_cc_pdb->p_glb_pdb,
                     ICF_STAT_UNSUCC_CALLS)
    }

	icf_dbm_hash_remove_call_ctxt(p_cc_pdb->p_glb_pdb,p_cc_pdb->p_call_ctx->call_id,
									p_tmp_call_ctxt,p_cc_pdb->p_ecode);	

    /* **** Clear the fields of call context **** */

    /*
     * Note that this will not clear the following fields: call_id, line_id,
     * p_sip_ctx (memory freed by SSA so that it can take care of UATK timers),
     * p_service_ctx (memory freed by SC)
     */

    /* reset call state to idle */
    ICF_CC_SET_CALL_STATE(p_cc_pdb, ICF_CC_00_IDLE)

    /******* Changes for Forking & Multiple Answer Start (Rel7.0) *******/

    /* p_forked_resp_list is deleted */
    ICF_PRINT(((icf_uint8_t*)"\n[CC]: Freeing Forked resp list in clear_call_context "));
    icf_cc_free_forked_resp_list(p_cc_pdb,p_cc_pdb->p_ecode); 

    /******** Changes for Forking & Multiple Answer End (Rel7.0) ********/

    icf_cc_clear_all_stream_sdp(p_cc_pdb);

	/* Deleting Pref media profile, if it exists.*/
	if (ICF_NULL != p_cc_pdb->p_call_ctx ->p_pref_media_profile)
	{
		p_cc_pdb->p_list_1 = p_cc_pdb->p_call_ctx ->p_pref_media_profile; 
		icf_cc_free_stream_capab_list(p_cc_pdb);
		p_cc_pdb->p_call_ctx ->p_pref_media_profile = ICF_NULL;
	}
  
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_response_str)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
            p_cc_pdb->p_call_ctx->p_response_str, ICF_MEM_COMMON,
            p_cc_pdb->p_ecode, dummy_retval)
    }

    /* Clear the div info */
    icf_cc_clear_div_info(p_cc_pdb);

    /* Free the  p_referred_party_addr */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_referred_party_addr)
    {
                ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_referred_party_addr, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_referred_party_addr = ICF_NULL;
    }

    /* Free the  p_referred_party_name */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_referred_party_name)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_referred_party_name, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_referred_party_name = ICF_NULL;
    }

    /* Free the  p_replace_nw_call_id */
    if(ICF_NULL != 
            p_cc_pdb->p_call_ctx->p_replaces_header)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_replaces_header,
                ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_replaces_header =
                ICF_NULL;
    }
    /* reset the common bitmask */
    p_cc_pdb->p_call_ctx->common_bitmask = ICF_NULL;
    p_cc_pdb->p_call_ctx->common_bitmask_2 = ICF_NULL;
    p_cc_pdb->p_call_ctx->forked_resp_counter = ICF_NULL; 
    /* reset the call modify method bitmask */
    p_cc_pdb->p_call_ctx->call_modify_method_bitmask = ICF_NULL;

    /* reset the media ongoing flag */
    ICF_CC_RESET_MEDIA_ONGOING(p_cc_pdb)

    /* reset the pending event bitmask */
    ICF_CC_RST_ALL_PENDING_EVENT(p_cc_pdb)

    /* reset the ongoing event */
    ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)

    /* reset the media committed flag */
    p_cc_pdb->p_call_ctx->media_committed = ICF_FALSE;

    /* reset the media updated flag */
    p_cc_pdb->p_call_ctx->media_updated = ICF_FALSE;

    /* reset the inband info flag */
    p_cc_pdb->p_call_ctx->inband_info_present = ICF_FALSE;
    p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;

    p_cc_pdb->p_call_ctx->force_clr_rcvd = ICF_FALSE;

    
    /*If this was a call in conference then clear the 
     * ICF_CALL_STATUS_CONFERENCE status from the other call in conference*/
    if(ICF_CALL_STATUS_CONFERENCE & p_cc_pdb->p_call_ctx->call_status)
    {
        /*Reset the ICF_CALL_STATUS_CONFERENCE status from the other call as
         *that would execute as a normal call*/
        p_cc_pdb->p_call_ctx->p_conf_other_call_ctx->call_status &= 
             ~(ICF_CALL_STATUS_CONFERENCE);
        /*Reset the pointer to other call context */
        p_cc_pdb->p_call_ctx->p_conf_other_call_ctx->p_conf_other_call_ctx = 
            ICF_NULL;
        /*Set p_conf_other_call_ctx to Null in current call context*/
        p_cc_pdb->p_call_ctx->p_conf_other_call_ctx = ICF_NULL;
    }

	/* Free the  p_referred_party_addr */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_contact_address)
    {
		ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
		p_cc_pdb->p_call_ctx->p_contact_address, ICF_MEM_COMMON,
		p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_contact_address = ICF_NULL;
    }

    /* status should be set to this only if INVITE with Replaces is
     * not being handled for this call */
    if (ICF_INVALID_CALL_ID == 
        p_cc_pdb->p_call_ctx->replace_initiated_call_id)
    {
        ICF_CC_SET_STATUS(p_cc_pdb,ICF_STATUS_CALL_R_SRV_TERMINATE)
    }
	
ICF_SESSION_CONTROL_START

	/* reset the app id */
        /*p_cc_pdb->p_call_ctx->app_id = ICF_APP_ID_INVALID;*/

	/* reset the call type */
	p_cc_pdb->p_call_ctx->call_type = ICF_CALL_TYPE_INVALID;

	/* reset the privacy header indication flag */
	p_cc_pdb->p_call_ctx->send_privacy_hdr = ICF_FALSE;

        /* reset the fax resp boolean flag */ 
    p_cc_pdb->p_call_ctx->stop_fax_resp_recvd = ICF_FALSE;

	/* free the preferred identity */
	if(ICF_NULL != p_cc_pdb->p_call_ctx->p_preferred_identity)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_preferred_identity, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_preferred_identity = ICF_NULL;
    }

	/* free the asserted identity */
	if(ICF_NULL != p_cc_pdb->p_call_ctx->p_asserted_identity)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_asserted_identity, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_asserted_identity = ICF_NULL;
    }

	/* free the called party identity */
	if(ICF_NULL != p_cc_pdb->p_call_ctx->p_called_party_id)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_called_party_id, ICF_MEM_COMMON,
                p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_called_party_id = ICF_NULL;
    }
	/* free the list of additional headers */
	ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb, 
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);

	/* free the list of additional tags */
	ret_val = icf_cmn_free_tag_list(p_cc_pdb->p_glb_pdb, 
				&(p_cc_pdb->p_call_ctx->tag_list),
				p_cc_pdb->p_ecode);

	/* free the list of additional message bodies */
	ret_val = icf_cmn_free_msg_body_list(p_cc_pdb->p_glb_pdb, 
				&(p_cc_pdb->p_call_ctx->body_list),
				p_cc_pdb->p_ecode);
    /* free the list of persistent headers */
	ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb, 
				&(p_cc_pdb->p_call_ctx->persistent_header_list),
				p_cc_pdb->p_ecode);

    if (ICF_NULL != p_cc_pdb->p_call_ctx->p_pref_media_profile)
    {
        p_cc_pdb->p_list_1 = p_cc_pdb->p_call_ctx->p_pref_media_profile;
        ret_val = icf_cc_free_stream_capab_list(p_cc_pdb);
        p_cc_pdb->p_call_ctx->p_pref_media_profile = ICF_NULL;
        p_cc_pdb->p_list_1 = ICF_NULL;
    }

#ifdef ICF_QOS_SUPPORT	
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
ICF_CHECK_IMS_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
	/* Free memory for P-Media-Authorization header */
	if (ICF_NULL != p_cc_pdb->p_call_ctx->p_auth_token)
	{
		ICF_MEMFREE(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->p_auth_token,
			ICF_MEM_COMMON, p_cc_pdb->p_ecode, ret_val)

		p_cc_pdb->p_call_ctx->p_auth_token = ICF_NULL;
	}
ICF_CHECK_IMS_END	
#endif
#endif
#endif	

    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_pref_media_profile)
    {
        p_cc_pdb->p_list_1 = p_cc_pdb->p_call_ctx->p_pref_media_profile;
        icf_cc_free_stream_capab_list(p_cc_pdb);
        p_cc_pdb->p_call_ctx->p_pref_media_profile = ICF_NULL;
    }

#ifdef ICF_SESSION_TIMER
    if (ICF_TRUE == p_cc_pdb->p_call_ctx->session_timer_enable)
    {
        if (ICF_NULL != p_cc_pdb->p_call_ctx->p_session_timer_data)
        {
           ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                       p_cc_pdb->p_call_ctx->p_session_timer_data,
                       ICF_MEM_COMMON,
                       p_cc_pdb->p_ecode,
		       ret_val)
           p_cc_pdb->p_call_ctx->p_session_timer_data = ICF_NULL;
        } 
    }
#endif
ICF_SESSION_CONTROL_END
    if (ICF_NULL != p_cc_pdb->p_call_ctx->p_sip_ctx)
    {
         ret_val = icf_cc_send_ssa_detach_call_ctx(p_cc_pdb);
         /*If the validation was successfull set p_sip_ctx in current call 
          *context to NULL*/
         p_cc_pdb->p_call_ctx->p_sip_ctx = ICF_NULL;
    }
#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
	/* reset alternate called party address for an incoming call */
    icf_cmn_init_address(&(p_cc_pdb->p_call_ctx->alternate_local_address));
#endif

#ifdef ICF_TRACE_ENABLE
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_trace_criteria_data)
    {
       if (ICF_NULL != 
           p_cc_pdb->p_call_ctx->p_trace_criteria_data->p_stream_data)
       {
          p_cc_pdb->p_list_1 = p_cc_pdb->p_call_ctx->
             p_trace_criteria_data->p_stream_data;
#ifndef ICF_UNIT_TEST
          icf_cc_free_stream_data_list(p_cc_pdb);
#endif
       }

       ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                p_cc_pdb->p_call_ctx->p_trace_criteria_data,
		        ICF_MEM_COMMON, p_cc_pdb->p_ecode, ret_val)
   }
#endif
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_response_str)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
        p_cc_pdb->p_call_ctx->p_response_str, ICF_MEM_COMMON,
        p_cc_pdb->p_ecode, ret_val);
    }
    p_cc_pdb->p_call_ctx->call_to_be_joined = ICF_INVALID_CALL_ID;

    p_cc_pdb->p_call_ctx->p_initial_sip_ctx = ICF_NULL;

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_STATUS_ALL_CLR)

    ICF_CC_SET_CALL_CLR_STATUS(p_cc_pdb,
                ICF_CALL_CLEAR_STATUS_CC_CLEARED)

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ICF_SUCCESS;
}


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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * if incoming call send call reject to SSA 
     */
        
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_DIR))       
    {
		/*p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED;*/
		/*ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb, ICF_CALL_CLEAR_REASON_REQUEST_TERMINATED)*/

        /* Send call reject to SSA */
       ret_val = icf_cc_send_ssa_reject(p_cc_pdb);
    }
	/* The checking of call connect done flag would ensure either connect is received from network or it has 
	 * been send on network. In either cases we would send bye.*/
    else if (
        (ICF_CC_SSA_CALL_CONNECT == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb)) ||
        (ICF_NULL != 
			ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE)))

    {
        /* It is a outgoing call if we have got Call Connect from peer and 
         * also sent Ack to peer, send bye to remote party */    
        ret_val = icf_cc_send_ssa_call_term_req(p_cc_pdb);
    }
    else if(ICF_CC_GET_CALL_STATE(p_cc_pdb))
    {
        /* It is a outgoing call if we are not waiting for ssa setup ack 
         * send cancel to remote party */    
        ret_val = icf_cc_send_ssa_cancel_req(p_cc_pdb);
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_init_pdb
 *
 * DESCRIPTION:     This is a utility function that initializes CC pdb with
 *                  default values.
 *
 ******************************************************************************/
icf_return_t icf_cc_init_pdb(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    p_cc_pdb->p_ecode = &(p_cc_pdb->p_internal_msg->msg_hdr.ecode);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_pdb->p_ecode))

    /*
     * The p_internal_msg, p_call_ctx, p_glb_pdb, current_event fields
     * are assigned in icf_cc_process_msg()
     */
	
    *(p_cc_pdb->p_ecode) = ICF_ERROR_NONE;

	/* reset common bitmask.*/
	p_cc_pdb->common_bitmask = ICF_NULL;

	/* Set list pointer to be NULL.*/
	p_cc_pdb->p_list_1 = ICF_NULL;

	/* Set list pointer to be NULL.*/
	p_cc_pdb->p_list_2 = ICF_NULL;

	/* Set this generic pointer to be NULL.*/
	p_cc_pdb->p_generic_ptr_1 = ICF_NULL;

	/* Set this generic pointer to be NULL.*/
	p_cc_pdb->p_generic_ptr_2 = ICF_NULL;

	/* Set this pointer to variable of type 
	 * icf_internal_sdp_stream_st to be NULL.
	 */
	p_cc_pdb->p_int_sdp_stream = ICF_NULL;

	/* Set this pointer to variable of type 
	 * icf_stream_sdp_info_st to be NULL.
	 */
	p_cc_pdb->p_stream_sdp_info = ICF_NULL;

	/* Set this with 0.*/
	p_cc_pdb->mms_resp_type = 0;

	/* Set this with 0.*/
	p_cc_pdb->call_type = 0;

	/* Set this with 0.*/
	p_cc_pdb->count = 0;

	/* By default this flag will be ICF_FALSE.*/
	p_cc_pdb->flag = ICF_FALSE;

	p_cc_pdb->stream_id = 0;


    /* reset modify_media_type bitmask */
    p_cc_pdb->modify_media_type = ICF_NULL;

    /* set forceful_delete type to false */
    p_cc_pdb->forceful_delete = ICF_FALSE;

    /* set sdp_present flag to false */
    p_cc_pdb->sdp_present = ICF_FALSE;

    /* set media_to_be_sent flag to false */
    p_cc_pdb->media_to_be_sent = ICF_FALSE;

    /* set media_to_be_sent flag to false */
    p_cc_pdb->mms_sent = ICF_FALSE;

    /* set timer_type to setup timer */
    p_cc_pdb->timer_type = ICF_TIMER_TYPE_SETUP;

    /* set timer id to invalid */
    p_cc_pdb->timer_id = ICF_TIMER_ID_INVALID;

    /*Set refer init call as false by default */
    p_cc_pdb->refer_init_call = ICF_FALSE;

    /* set redirection address to null */
    p_cc_pdb->p_redirection_addr = ICF_NULL;

    /* set redirection name to null */
    p_cc_pdb->p_redirection_name = ICF_NULL;

    /*This flag would be set when MMI returns failure due to no context 
     * available
     */
    p_cc_pdb->dms_fail_no_mmi_ctx = ICF_FALSE;
    
    p_cc_pdb->pa_connect_ong = ICF_FALSE;
   
	p_cc_pdb->mms_type = 0;

	p_cc_pdb->p_redirection_addr = ICF_NULL;

	p_cc_pdb->p_redirection_name = ICF_NULL;

	p_cc_pdb->pa_connect_ong = ICF_FALSE;

	p_cc_pdb->timer_type = 0;

    p_cc_pdb->is_call_modify_timer = ICF_FALSE;

    /* set the prack reqd flag as per config data */
    ret_val = icf_dbm_get_module_glb_data(
                p_cc_pdb->p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))

    p_cc_pdb->prack_reqd = ICF_FALSE;
    p_cc_pdb->application_mode = ICF_MEDIA_MODE_INVALID;
    p_cc_pdb->derived_mode = ICF_MEDIA_MODE_INVALID;
    p_cc_pdb->remote_mode = ICF_MEDIA_MODE_INVALID;

#ifdef ICF_TRACE_ENABLE
    icf_port_memset(&(p_cc_pdb->remote_rtp_addr.addr),0,sizeof(icf_transport_addr_st));
    p_cc_pdb->remote_rtp_addr.port_num = 0;
#endif
    p_cc_pdb->pa_call_mod_wo_media = ICF_FALSE;

    ICF_HANDLE_REINVITE_START
    if (ICF_SUCCESS == ret_val)
    {
		if(ICF_NULL != p_cc_pdb->p_call_ctx)
		{
			if( ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
							ICF_CALL_CONNECT_DONE))
			{
				if ((ICF_CFG_CC_PRACK_REQD ==
						(ICF_CFG_CC_PRACK_REQD & p_config_data->default_settings)) &&
						ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
						   ICF_REL_PROV_RESP_SUPP_BY_PEER))
				{
					p_cc_pdb->prack_reqd = ICF_TRUE;
				}
				
			}
		}
	} /* for return value of get config data ptr from dbm */

ICF_HANDLE_REINVITE_END

#ifdef ICF_TRACE_ENABLE
     p_cc_pdb->resp_code = 0;
     p_cc_pdb->result = ICF_FAILURE;
     p_cc_pdb->call_hold_status = ICF_FALSE;
     p_cc_pdb->early_media = ICF_FALSE;
     p_cc_pdb->call_clear_dir = 0;
     p_cc_pdb->stream_type = ICF_STREAM_TYPE_INVALID;
     p_cc_pdb->p_replacing_party_addr = ICF_NULL;
     p_cc_pdb->p_replacing_party_name = ICF_NULL;
#endif

	/* Fix for CSR 1-6727350: SPR 19316 */
    p_cc_pdb->p_config_data = ICF_NULL;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_diversion_info_st            *p_div_info = ICF_NULL;
    icf_diversion_data_st            *p_div_data = ICF_NULL;
    icf_address_st                   *p_diversion_addr = ICF_NULL;
    icf_boolean_t                    call_thru_proxy = ICF_TRUE;
    icf_uint8_t                      param_val[5];
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Read diversion info in local pointer */
    p_div_info = p_cc_pdb->p_call_ctx->p_diversion_header_list;
    /*Check if diversion info is present or not */
    if(ICF_NULL == p_div_info)
    {
        ICF_CC_TRACE(p_cc_pdb,ICF_DIVERSION_INFO_VALIDATION_FAILED)
         ret_val = ICF_FAILURE;
    }  
    /* Check if contact info is presnet or not */
    else if(ICF_NULL == p_div_info->p_contact_addr)
    {
        ICF_CC_TRACE(p_cc_pdb,ICF_DIVERSION_INFO_VALIDATION_FAILED)
         ret_val = ICF_FAILURE;
    }  
    /* 
     * Validate contact address against our remote address or local address
     * If so return failure. 
     */
    else 
    {
        /*  Fix for CSR 1-7302347 */
        /*If contact address is same as remote party address
         *or local user then  return
         * failure*/
        if(ICF_TRUE == icf_cmn_compare_address(
                p_div_info->p_contact_addr,
                &(p_cc_pdb->p_call_ctx->remote_party_addr)))
    {    
         ICF_CC_TRACE(p_cc_pdb,ICF_DIVERSION_INFO_VALIDATION_FAILED)
         ret_val = ICF_FAILURE;
    }
        else if(ICF_TRUE == icf_cmn_compare_address(
                p_div_info->p_contact_addr,
                    &(p_cc_pdb->p_call_ctx->local_address)))
    {    
         ICF_CC_TRACE(p_cc_pdb,ICF_DIVERSION_INFO_VALIDATION_FAILED)
         ret_val = ICF_FAILURE;
    }

    /* 
     * Check if mode is present in contact address and if it is present
     * then validate it against the proxy mode
     */
    else if(ICF_TRUE == icf_cmn_check_if_param_present(
        &(p_div_info->p_contact_addr->addr_val), (icf_uint8_t*)"transport", param_val, ICF_NULL, ICF_NULL))    
    {
        call_thru_proxy = icf_dbm_chk_ogn_call_thru_proxy(
                p_cc_pdb->p_glb_pdb,        
                p_cc_pdb->p_ecode);

        if(ICF_FAILURE == icf_dbm_validate_mode_ogn_call(
                    p_cc_pdb->p_glb_pdb,        
                    param_val,
                    p_cc_pdb->p_call_ctx->line_id,
                    call_thru_proxy,
                    p_cc_pdb->p_ecode))
        {
            ICF_CC_SET_RELEASE_CAUSE(p_cc_pdb,
                    ICF_CALL_CLEAR_REASON_INCONSISTENT_TRANSPORT_MODE)
                ret_val = ICF_FAILURE;
        }
    }
    }
    
    if(ICF_FAILURE != ret_val &&
            ICF_NULL != p_div_info->no_of_diversion_data)
	{       
        /* Diversion header was present so we should validate contact address
         * against diversion info */
        p_div_data = p_div_info->p_diversion_data;
        /* Check if no of hops is zero then return failure */
        if (ICF_NULL == p_div_info->no_of_hops)
        {
            ICF_CC_TRACE(p_cc_pdb,ICF_DIVERSION_INFO_VALIDATION_FAILED)
                ret_val = ICF_FAILURE;
        }
        else
        {  
            while(ICF_NULL != p_div_data)
            {
                p_diversion_addr = &p_div_data->diverted_frm_addr;
                /* Compare diversion address */
                if(ICF_TRUE == icf_cmn_compare_address(
                            p_diversion_addr,
                            p_div_info->p_contact_addr))
                {    
                    ICF_CC_TRACE(p_cc_pdb,
                            ICF_DIVERSION_INFO_VALIDATION_FAILED)
                        ret_val = ICF_FAILURE;
                    break; /* Exit the while loop */
                }
                /*  Fix for CSR 1-7302347 */
                /*If diversion address is same as local address
                 * then return failure
                 */

                else if(ICF_TRUE == icf_cmn_compare_address(
                            p_diversion_addr,
                            &(p_cc_pdb->p_call_ctx->local_address)))
                {    
                    ICF_CC_TRACE(p_cc_pdb,
                            ICF_DIVERSION_INFO_VALIDATION_FAILED)
                        ret_val = ICF_FAILURE;
                }
                /* Point to next element in list */
                p_div_data = p_div_data->p_next;

            }/* End of while loop */
        }
    }
    
    /* Check if validation were succesful */
    if(ICF_SUCCESS == ret_val)    
    {
        /* 
         * All validation were succesful 
         * Copy contact info in local address and decrement max hop 
         */
        icf_port_memcpy((icf_void_t *)&(p_cc_pdb->p_call_ctx->
                    remote_party_addr),
                p_div_info->p_contact_addr,
                (icf_uint32_t)sizeof(icf_address_st));

        if (ICF_NULL != p_cc_pdb->p_call_ctx->p_sip_ctx)
        {
             ret_val = icf_cc_send_ssa_detach_call_ctx(p_cc_pdb);
             /*If the validation was successfull set p_sip_ctx in current call 
              *context to NULL*/
             p_cc_pdb->p_call_ctx->p_sip_ctx = ICF_NULL;
        }
        /* Fix For SPR:19788
         * Reset the  ICF_CALL_AUTHENTICATION_REQ bitmask of call context.
         * Successfull validation of redirected address means we will
         * initiate the new INVITE on new address and in this scenario there
         * is no significance of this bitmask as the request is send to new
         * address. 
         */
        if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_AUTHENTICATION_REQ))
        {
           ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_AUTHENTICATION_REQ) 
        }
    }
     
   /* 
    * RESET provisional response status as new call would be initiated now 
	*/
	p_cc_pdb->p_call_ctx->prov_resp_status = ICF_NULL;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t                      ongoing_event;
    icf_mmi_reason_t	             media_error= 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     ongoing_event = ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    if(ICF_CC_SSA_STOP_FAX_REQ== ongoing_event )
	 {
		 /*Send 500 on network*/
		 p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

         if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
         {
         	/*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
         }

	 }

	 if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_STOP_FAX_REQ))
	 {
		p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;

        ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
        /* Send failure response to PA */
        if(ICF_FAILURE == icf_cc_send_pa_stop_fax_resp(p_cc_pdb))
        {
        	ret_val = ICF_FAILURE;
        }

	 }
	if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_SSA_STOP_FAX_REQ))
	 {
		 /*Send 500 on network*/
		 p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;

         if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
         {
         	/*Return ret_val as failure. Call will be now forcefully cleared*/
            ret_val = ICF_FAILURE;
         }

	 }
	 if(ICF_CC_PA_FAX_REQ == ongoing_event || ICF_CC_SSA_FAX_RESP == ongoing_event)
      {
           	 	p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;

            	ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)        
            	/* Send failure response to PA */
            	if(ICF_FAILURE == icf_cc_send_pa_fax_resp(p_cc_pdb))
            	{
                	ret_val = ICF_FAILURE;
            	}
     }
     /* if remote Fax is ongoing event send error resp for same */
     if(ICF_CC_SSA_FAX_REQ == ongoing_event)
     {
         /*Send 500 on network*/
         p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;
         
         if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
         {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
             ret_val = ICF_FAILURE;
         }

     }
     if(ICF_CC_MEDIA_PARAM_PRACK == ongoing_event)
     {
		 media_error = ((icf_mm_err_ind_st*)
                      (p_cc_pdb->p_internal_msg->p_msg_data))->reason;
     }
     if((ICF_CC_ONGOING_EVENT_REMOTE_MEDIA_REVERT == ongoing_event) &&
         (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK_2
                             (p_cc_pdb,ICF_INCOMING_CALL_MOD_CANCEL_REQ)))
     {
           p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ;
           if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
           {
              /*Return ret_val as failure. Call will be now forcefully cleared*/
              ret_val = ICF_FAILURE;
           }
     }

     /* if update is ongoing event or pending evnet send error resp for same */
     if((((ICF_CC_SSA_OFFER_CHANGE == ongoing_event) && 
         (ICF_CC_SSA_CALL_TERMINATE_REQ != p_cc_pdb->current_event)) || 
         (ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, ICF_PENDING_EVENT_OFFER_CHANGE_REQ))))
     {
         /*Send 500 on network*/
         p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_SA_SERVER_INTERNAL_ERROR;
         
         if(ICF_FAILURE == icf_cc_send_ssa_offer_chg_err(p_cc_pdb))
         {
            /*Return ret_val as failure. Call will be now forcefully cleared*/
             ret_val = ICF_FAILURE;
	      /* 
	      * For failure call would be released so reset the pending event.
	      * This is done to make sure if if the failure was in sending some 
	      * response on network and genric call clear is initiated the n
	      * same response should not be initiated again
	      */
             ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
         }
     }
ICF_HANDLE_REINVITE_START
	 else if(ICF_CC_PA_CALL_MODIFY_REQ == ongoing_event ||
          ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ))
     {         
		
		 p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INTERNAL_FAILURE;

		 ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
		 ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);
		
     }
	 else if(ICF_CC_PA_CALL_MODIFY_CANCEL_REQ == ongoing_event)
     {
         /*Send call modify cancel response followed by call modify response*/
         if (ICF_CC_PA_CALL_TERMINATE_REQ == p_cc_pdb->current_event)
         {
            p_cc_pdb->p_call_ctx->app_failure_cause = 
                                      ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
         }
         else
         {
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
         }
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

         /* Send failure response to PA */
         if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
 
         if (ICF_CC_PA_CALL_TERMINATE_REQ == p_cc_pdb->current_event)
         {
            p_cc_pdb->p_call_ctx->app_failure_cause = 
                                      ICF_CAUSE_USER_INITIATED_CALL_TERM_REQ;
         }
         else
         {
            p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_MEDIA_FAILURE;
         }
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

         /* Send failure response to PA */
         if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }

         /*Send cancel request to remote as it has already sent ReInvite*/
        if(ICF_FAILURE == icf_cc_send_ssa_cancel_req(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
     }
	 else if((ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP == ongoing_event) &&
             (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ)))
     {
         /*Send call modify cancel response followed by call modify response*/
         p_cc_pdb->p_call_ctx->app_failure_cause = 
                                      ICF_CAUSE_REMOTE_RELEASED;
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_CANCEL_RESP_FAIL)

         /* Send failure response to PA */
         if (ICF_FAILURE == icf_cc_send_pa_call_modify_cancel_resp(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
 
         p_cc_pdb->p_call_ctx->app_failure_cause = 
                                      ICF_CAUSE_REMOTE_RELEASED;
         ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)

         /* Send failure response to PA */
         if (ICF_FAILURE == icf_cc_send_pa_call_modify_resp(p_cc_pdb))
         {
             ret_val = ICF_FAILURE;
         }
     }
ICF_HANDLE_REINVITE_END

     if (ret_val == ICF_SUCCESS)
     {
		 p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_CLEAR_MEDIA_MISMATCH;

         if (ICF_CC_ALERT_PRACK == ongoing_event ||
                 ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                     ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP))
         {
             if(ICF_FAILURE == icf_cc_send_ssa_alert_prack_err(
                         p_cc_pdb))
             {
                 /* Return error to top level function,
                  * call will be forcefully cleared*/
                 ret_val = ICF_FAILURE;

		  /* 
		  * For failure call would be released so reset the pending event.
		  * This is done to make sure if if the failure was in sending some 
		  * response on network and genric call clear is initiated the n
		  * same response should not be initiated again
		  */
		 ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP)
             }
         }
         else if (ICF_CC_MEDIA_PARAM_PRACK == ongoing_event ||
                 ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                     ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP))
         {
             if(ICF_FAILURE == icf_cc_send_ssa_media_prack_err(
                         p_cc_pdb))
             {
                 /* Return error to top level function,
                  * call will be forcefully cleared*/
                 ret_val = ICF_FAILURE;

		 /* 
		  * For failure call would be released so reset the pending event.
		  * This is done to make sure if if the failure was in sending some 
		  * response on network and genric call clear is initiated the n
		  * same response should not be initiated again
		  */
		 ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP)
             }
             else if(ICF_CC_MEDIA_PARAM_PRACK == ongoing_event)
			 {				  
				 if(media_error == 1)/*PANKAJ ICF_ECODE_MMS_RESP_FAILURE_MEDIA_NORMAL)*/
				 {
					 /* Send 180 on network */
					 ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
				 }
				 else
				 {
					 /* 
					  * If there was some media error then we wouldn't be sending 
					  * alert as the call has to be released
					  */
					 ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                      ICF_CC_08_AWT_CALL_TERMINATE_RESP)
				 }
			 }
         }

     }
    if(ICF_FAILURE == ret_val)
     {
          /* 
           * For failure call would be released so reset the ongoing event.
           * This is done to make sure if if the failure was in sending some 
           * response on network and genric call clear is initiated the n
           * same response should not be initiated again
           */
          ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)
     }
     ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
     return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_otg_clear_pend_evt_mms_rsp
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for outgoing call for ALERT_WITH_MEDIA and MEDIA_COMMIT.
 *                  We have got mms for call alert and media commit
 *                  so pending event
 *                  can be Call mute or call unmute or offer change
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_otg_clear_pend_evt_mms_rsp(
           INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*
     * Check pendign list for connect req from remote
     */

    if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
        ICF_PENDING_EVENT_SSA_CONNECT))
    {
        /*Remove connect from pending event list */
        if(ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
	/*If media is modified with the offer sent to remote then
	 *send UPDATE request to remote, with the latest media
	 *applied at local end*/
	else if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
	{
		/*Send UPDATE request*/
		/* RESET MEDIA MODIFY FLAG */
		ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
		if (ICF_FAILURE == icf_cc_ssa_send_update_msg(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
		else
		{
			ICF_CC_SET_NEXT_STATE(p_cc_pdb,
					ICF_CC_14_AWT_CALL_MODIFY_RESP)
		}
	}
	ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_SSA_CONNECT)
    }

    if(ICF_SUCCESS == ret_val &&
           ICF_NULL != ICF_CHECK_PENDING_LIST_IS_EMPTY(p_cc_pdb))
    {
        ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
        if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
 
        /* If pending event can be offer change only,
         * Send MMS for Offer change request*/
        else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
        {
            /* Set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else
        {
             /*
              *  able to send mms succesfully. set the ongoining event
              * Mark ongoing event depending on pending event
              */
            if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
            {
                /* RESET Pending event SSA_OFFER_CHANGE*/
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
            
			    /* 
            	 * If mms request was initiated towards mmi then
                 * set ongoing event as ICF_CC_SSA_OFFER_CHANGE
                 */
		 if (ICF_TRUE == p_cc_pdb->mms_sent)
                {	
        	        /* Set Offer Change as Ongoing event and 
                	 * remove it from pending list */

	                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
    	                ICF_CC_SSA_OFFER_CHANGE)
		}
               	else
	    	{
                    /* Send response for offer change request*/
                	ret_val = icf_cc_send_ssa_offer_chg_rsp(p_cc_pdb);
	    	}    
            }			
            else
            {
                /* Raise alarm of Unexpected event */
                ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
            }

        }/* Successful processing of start proc req*/

        /*
         * If MMS was sent to MMI and NEXT  state was connected then start media
         * timer
         */
        if(ICF_TRUE == p_cc_pdb->mms_sent &&
                ICF_CC_06_CONNECTED == ICF_CC_GET_NEXT_STATE(p_cc_pdb))
        {
             /*start media timer*/
             ret_val = icf_cc_start_media_timer(p_cc_pdb);            
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


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
icf_return_t icf_cc_mms_rsp_ssa_media_param(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint32_t     pending_event = 0; /* Store ongoing event */
    icf_uint8_t      ongoing_event = 0; /* Store ongoing event */
	icf_boolean_t	 handle_sdp = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
       
	/*Get pending event */
	pending_event = p_cc_pdb->p_call_ctx->pending_event;
    ongoing_event = ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);
	
	/* If pendling list is not empty */
	if(ICF_NULL != pending_event)
	{
		if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM))
		{
			ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
				ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM)
				
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_MEDIA_PARAM)    
                p_cc_pdb->current_event = ICF_CC_SSA_MEDIA_PARAM;
				ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_03_AWT_ALERT_RESP)
				handle_sdp = ICF_TRUE;
		}
		else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA))
		{
			ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
				ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA)
				
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_ALERT_WITH_MEDIA)  
                p_cc_pdb->current_event = ICF_CC_SSA_ALERT_WITH_MEDIA;
				ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_05_AWT_CONNECT)
				handle_sdp = ICF_TRUE;
		}
		else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_FMA_SSA_CONNECT))
		{
			ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
				ICF_PENDING_EVENT_FMA_SSA_CONNECT)
				
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)   
                p_cc_pdb->current_event = ICF_CC_SSA_CALL_CONNECT;
                if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
                {
                   /*Send UPDATE request*/
                   if (ICF_FAILURE ==
                         icf_cc_ssa_send_update_msg(p_cc_pdb))
                    {
                       ret_val = ICF_FAILURE;
                    }
                   else
                   {
                     ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                              ICF_CC_14_AWT_CALL_MODIFY_RESP)
                   }           
                }
                else
                {
                   ICF_CC_SET_NEXT_STATE
                              (p_cc_pdb,ICF_CC_06_CONNECTED)
                }
				handle_sdp = ICF_TRUE;
		}
                else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
			ICF_PENDING_EVENT_SSA_CONNECT))
		{
			ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
				ICF_PENDING_EVENT_SSA_CONNECT)
				
			ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)   
                        p_cc_pdb->current_event = ICF_CC_SSA_CALL_CONNECT;

				/* Stop the alert timer */
			icf_cc_stop_current_timer(p_cc_pdb);

			/* Fix corresponding to spr 14476.*/
			if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd) 
	        {
		       /*Send UPDATE request*/
		       if (ICF_FAILURE == 
                      icf_cc_ssa_send_update_msg(p_cc_pdb))
		       {
			      ret_val = ICF_FAILURE;
		       }
		       else
		       {
			      ICF_CC_SET_NEXT_STATE(p_cc_pdb,
					     ICF_CC_14_AWT_CALL_MODIFY_RESP)
		       }
	        }
            else
            {
               ICF_CC_SET_NEXT_STATE 
                        (p_cc_pdb,ICF_CC_06_CONNECTED)
            }
			/* End of Fix corresponding to spr 14476.*/

       			/* Send COnnect ind to PA */
	       		if(ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
	        	{
			       /* Set return value as failure */
				    ret_val = ICF_FAILURE;
			}
            /* Below line is being commented corresponding to spr 14476.*/
			/* ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)*/
		}
		else
		{
			/* Raise alarm of Unexpected event */
			ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
			/* Invoke function to clear pending event for outgoing call */
                       if(ICF_FAILURE == icf_cc_otg_clear_pend_evt_mms_rsp(p_cc_pdb))
                       {
                            ret_val = ICF_FAILURE;
                       }
		}  
		
		if ((ICF_SUCCESS == ret_val) && (p_cc_pdb->p_call_ctx->ongoing_event == ICF_CC_SSA_ALERT_WITH_MEDIA))
		{
			/* Stop the call setup timer and start the alert timer */
			if(ICF_FAILURE == icf_cc_start_alert_timer(p_cc_pdb, ICF_FALSE, ICF_NULL))
			{
				/*Return ret_val as failure. Call will be now forcefully cleared*/
				ret_val = ICF_FAILURE;
			}
		}
		
		if(ICF_TRUE == handle_sdp)
		{
			/* Send MMS again */
			/* This Function is deriving appropriate mms type.*/

			icf_cc_reset_media_modify_type_for_all_streams(p_cc_pdb);
			ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
		
			if (ICF_CC_ALL_STREAMS_DELETED == 
				(p_cc_pdb->common_bitmask & ICF_CC_ALL_STREAMS_DELETED))
			{
				ret_val = icf_cc_02_generic_err(p_cc_pdb);
			}
			else if (ICF_FAILURE != ret_val)
			{
				/* Send Modify Media Session to the MMI */
				/* TBD - post 6.3 */
				/* Being uncommented for spr 14476.*/
 
                                /* IPTK will send the local codecs on the GCL iff
                  p_prefered media profile is not null that means
                  ICF_NO_LOCAL_CODEC_IN_GCL is not set */

                        if(ICF_FALSE == (p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL))
                            {
                                 /* TBD - post 6.3 */
                                /* Uncommented corresponding to spr 14476 fix.*/
                                p_cc_pdb->p_call_ctx->common_bitmask 
                                       |= ICF_OFFER_CODECS_IN_GCL;
                            }

                            if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
                              {
                                p_cc_pdb->p_call_ctx->common_bitmask 
                                      &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
                               }

				if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
				{
					ret_val = ICF_FAILURE;
				}

				if(ICF_FALSE == p_cc_pdb->mms_sent)
				{
					if (ICF_CC_SSA_MEDIA_PARAM == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
					{
						if(ICF_FAILURE == icf_cc_send_pa_media_connect(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						}
					}
					else if (ICF_CC_SSA_ALERT_WITH_MEDIA == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
					{
						/* Alert with media. So inband info present = true ind
						* will be sent to PA */
						p_cc_pdb->p_call_ctx->inband_info_present = ICF_TRUE;

						/*Fill all remote and local accepted and rejected streams*/
						if(ICF_FAILURE == icf_cc_send_pa_alert_ind(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						}
					}
					else if (ICF_CC_SSA_CALL_CONNECT == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb))
					{
						/* Stop the alert timer */
						icf_cc_stop_current_timer(p_cc_pdb);
						
						/* Send CONNECT to PA */
						if (ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
						{
							ret_val = ICF_FAILURE;
						} /* else if (ICF_FAILURE == ipp... */
					}
                                        /* Fix for SPR 16743 starts. This is to handle 
                                           the  pending event 200 OK w/o SDP received 
                                           when 180 with SDP is already a pending event */
                                        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                              ICF_PENDING_EVENT_SSA_CONNECT))
                                        {
                                              ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                                                   ICF_PENDING_EVENT_SSA_CONNECT)

                                              ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_SSA_CALL_CONNECT)
                                              p_cc_pdb->current_event = ICF_CC_SSA_CALL_CONNECT;

                                              /* Stop the alert timer */
                                              icf_cc_stop_current_timer(p_cc_pdb);

                                              if (ICF_TRUE == p_cc_pdb->p_call_ctx->update_reqd)
                                              {
                                                    /*Send UPDATE request*/
                                                    if (ICF_FAILURE ==
                                                        icf_cc_ssa_send_update_msg(p_cc_pdb))
                                                    {
                                                         ret_val = ICF_FAILURE;
                                                    }
                                                    else
                                                    {
                                                         ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                                            ICF_CC_14_AWT_CALL_MODIFY_RESP)
                                                     }
                                               }
                                               else
                                               {
                                                     ICF_CC_SET_NEXT_STATE
                                                         (p_cc_pdb,ICF_CC_06_CONNECTED)
                                               }

                                               /* Send COnnect ind to PA */
                                               if(ICF_FAILURE == icf_cc_send_pa_connect_ind(p_cc_pdb))
                                               {
                                                     /* Set return value as failure */
                                                      ret_val = ICF_FAILURE;
                                                }
                                         }

				}
			}
		}
	}
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


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
icf_return_t icf_cc_mms_rsp_ssa_alert_with_media(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
	
    ret_val = icf_cc_mms_rsp_ssa_media_param(p_cc_pdb);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_update_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for mute or unmute request. this function will clear 
 *                  pending events
                    a. Set the media updated flag.
                    b. send ACK TO SSA. send remote call hold/ remote resume to 
                       PA
                    c. If pending event is 200 ok send Connect to PA
                    d. if next state is not 10 pending event is user_connect, 
                       then send 200 OK to NW and goto 11
                    f. If pending event is call mute, then send MMS(RECV ONLY) 
                       to MMA
                    g. If pending event is call unmute, then send MMS(SendRecv) 
                       to MMA
 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_update_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      next_state = 0;  /* variable to store next_state */
    icf_uint32_t     pending_event = 0; /* Store ongoing event */
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        
    next_state = ICF_CC_GET_NEXT_STATE(p_cc_pdb);
    
    /* Set media updated flag as true */
    p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;
    
    /*Get pending event */
    pending_event = p_cc_pdb->p_call_ctx->pending_event;
    
    /*
    * If next state is not 10 and usewr_connect is pending event
    * theb sebd 200 OK to NW and set next state as 11
    */
    if(ICF_CC_10_AWT_ALERT_PRACK != next_state)
    {
        if(pending_event == ICF_PENDING_EVENT_PA_CALL_CONNECT)
        {
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_11_AWT_SSA_CONNECT_ACK)
        }
    }
    
    /* If ret_val is success and pendling list is not empty */
    if(ICF_SUCCESS == ret_val && ICF_NULL != pending_event)
    {
        /* If pending list is not empty send mms again for
        *PA Connect it will be for mode change and rejected
        *streams, if exists in call context*/
        if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
        {
            /* Set return value as failure. call will be cleared forcefully*/
            ret_val = ICF_FAILURE;
        }
        else
        {            
            /* able to send mms succesfully. set the ongoing event
            * Mark ongoing event depending on pending event
            */
            if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_CONNECT))
            {
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)
                    
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)                    
            }
            else
            {
                /* Raise alarm of Unexpected event */
                ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
            }            
        }/* Successful updated pending events */        
    }/* Successful processing of pending events*/
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_alert_prack_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for ALERT_PRACK request. This function will cleare pending events

      a. if pending event is user_connected, send 200 OK with SDP and change next state to 11
      b. If pending event is call mute, then send MMS(RECV ONLY) to MMA
      c. If pending event is call unmute, then send MMS(SendRecv) to MMA
      d. If pending event is 200 ack with SDP, send MMS.(This is not possible
             as we havent sent 200 ok yet)
      e. If pending event is media_connect then send 183 with SDP.
      f. If pending event is alert with media send 180 with SDP.
      g. If pending event is alert wo media send 180 wo SDP.


 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_alert_prack_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint32_t     pending_event = 0; /* Store ongoing event */

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Set media updated flag as true */
    p_cc_pdb->p_call_ctx->media_updated = ICF_TRUE;

    /*Get pending event */
    pending_event = p_cc_pdb->p_call_ctx->pending_event;

   
    if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send 183 with SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_media_param(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)

        }
    }

    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP))
            && (ICF_SUCCESS == ret_val))
	{
		/* Send 183 without SDP with prov response required */
		p_cc_pdb->prack_reqd = ICF_TRUE;

		if (ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
		{
			ICF_CC_SET_CALL_STATE(p_cc_pdb,
				ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
			ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
				ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}

    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send Alert without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)

        }
    }
    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send Alert with SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_alert_media(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)
        }
    }
    else if(ICF_PENDING_EVENT_PA_CALL_CONNECT & pending_event)
    {
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                ICF_CC_11_AWT_SSA_CONNECT_ACK)  

        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)

        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT))
        {
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_CALL_CONNECT)

            ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)

            ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
            if(ICF_FALSE ==  p_cc_pdb->mms_sent)
            {
                /*If mms was not sent*/
                icf_cc_mms_rsp_pa_connect(p_cc_pdb);
            }

        }           
        else
        {
            /* Raise alarm of Unexpected event */
            ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
        }
    }

   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
   return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_media_prack_req
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for ALERT_PRACK request. This function will cleare pending
 *                  events
      a. if pending event is user_connected, send 200 OK with SDP and change
          next state to 11
      b. If next state is 9, send 180 without sdp and change next state to 10
      c. if pending event is user_connected, send 200 OK with SDP and change
          next state to 11
      d. If pending event is call mute, then send MMS(RECV ONLY) to MMA
      e. If pending event is call unmute, then send MMS(SendRecv) to MMA
      f. If pending event is 200 ack with SDP, send MMS.
          (This is not possible as we havent sent 200 ok yet)


 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_media_prack_req(INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint8_t      next_state = 0;  /* variable to store next_state */
    icf_uint32_t     pending_event = 0; /* Store ongoing event */

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    next_state = ICF_CC_GET_NEXT_STATE(p_cc_pdb);

    /*Get pending event */
    pending_event = p_cc_pdb->p_call_ctx->pending_event;

    if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send 183 with SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_media_param(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)

        }
    }
      else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP))
                    && (ICF_SUCCESS == ret_val))
    {
        /* Send 183 without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if (ICF_SUCCESS == icf_cc_send_ssa_media_param_wo_sdp(p_cc_pdb))
        {
            ICF_CC_SET_CALL_STATE(p_cc_pdb,
                ICF_CC_09_AWT_MEDIA_PARAM_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }

    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send Alert without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_alert(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)

        }
    }
    else if ((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                    ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
            && (ICF_SUCCESS == ret_val))
    {
        /* Send Alert with SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;

        if(ICF_SUCCESS == icf_cc_send_ssa_alert_media(p_cc_pdb))
        {
            /* Change call state to Awaiting prov response
             *  for alert
             */
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_10_AWT_ALERT_PRACK)
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)
        }
    }
    else if((ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb, 
                           ICF_PENDING_EVENT_PA_CALL_CONNECT)) 
            && (ICF_SUCCESS == ret_val))
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_EVENT_PA_CONNECT)
            /* Send Alert without SDP with prov response required */
        p_cc_pdb->prack_reqd = ICF_TRUE;
        ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_CONNECT)

        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,ICF_CC_PA_CALL_CONNECT)
        ret_val = icf_cc_snd_mms_pa_connect(p_cc_pdb);
        if(ICF_FALSE ==  p_cc_pdb->mms_sent)
        {
            /*If mms was not sent*/
            icf_cc_mms_rsp_pa_connect(p_cc_pdb);
            /* Update the states in the call context */
            p_cc_pdb->mms_sent = ICF_TRUE;
            ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
        }
    }
    else
    { 
        if(ICF_CC_10_AWT_ALERT_PRACK == next_state)
        {
            /*
             * Set prack required flag to true. This would ensure even if the 
             * configuration has changed the prack required flag to FALSE we would 
             * still continue sending the message with 100 rel in require header
             * This would ensure both provisional messages would either be sent with
             * 100 rel or without 100 rel
             */
            if(ICF_PRACK_FOR_MEDIA_CONNECT != (ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,\
                           ICF_PRACK_FOR_MEDIA_CONNECT)))
            {
                p_cc_pdb->prack_reqd = ICF_TRUE;
                /* Send Alert to PA and change state to await alert prack*/

                ret_val = icf_cc_send_ssa_alert(p_cc_pdb);
            }
            else
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb, ICF_CC_05_AWT_CONNECT)
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_PRACK_FOR_MEDIA_CONNECT)

            }
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_ssa_call_con_ack
 *
 * DESCRIPTION:     This function is invoked when the CC get mms resp from MMI
 *                  for SSA_CONNECT/SSA_CONNECT_ACK request. This function will
                    clear pending events
      a. if pending event is user_connected, send 200 OK with SDP and change
            next state to 11
      b. If pending event is call mute, then send MMS(RECV ONLY) to MMA
      c. If pending event is call unmute, then send MMS(SendRecv) to MMA

 *
 *
 *****************************************************************************/
icf_return_t icf_cc_mms_rsp_ssa_call_con_ack(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

    if(ICF_NULL != (ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                              ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ)))
     {
       ICF_CC_RESET_COMMON_BITMASK (p_cc_pdb, ICF_CALL_MODIFY_DIR) 

       /* Set the call modify direction flag */
       p_cc_pdb->p_call_ctx->common_bitmask |= ICF_CALL_MODIFY_DIR;
       p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;

       ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                  ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ)
       p_cc_pdb->pa_call_mod_wo_media = ICF_TRUE;

       /*Send call_modify_ind to application*/
       ret_val = icf_cc_send_pa_call_modify_ind(p_cc_pdb);
       ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_14_AWT_CALL_MODIFY_RESP)

#ifdef ICF_PATCH_FOR_OFFER_IN_200
       icf_cc_handle_non_fs_reinvite(p_cc_pdb);
#endif       

     }
    else if(ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb)) 
     {
        ret_val = ICF_FAILURE;
     }
    /* If pending list is not empty, it means SSA Offer Change is 
     * pending event then send MMS for SSA Offer Change*/
    else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
         /* Set return value as failure. call will be cleared forcefully*/
          ret_val = ICF_FAILURE;
    }
    else
    {
        /* able to send mms succesfully. set the ongling event
        * Mark ongoing event depending on pending event
        */
        if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
            ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
        {            
            if(ICF_FALSE == p_cc_pdb->mms_sent)
            {
                p_cc_pdb->is_call_modify_timer = ICF_TRUE;

                if(ICF_FAILURE == icf_cc_start_setup_timer(p_cc_pdb))
		{
		    ret_val = ICF_FAILURE;
		}
		else if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
		{
		    ret_val = ICF_FAILURE;
		}
		else
		{
	  	    ICF_CC_SET_CALL_STATE(p_cc_pdb,
				    ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
	  	    ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                                    ICF_CC_14_AWT_CALL_MODIFY_RESP)            
		}
            }
            else
            {
		/* Set Offer Change as Ongoing event 
		 * and remove it from pending list */            
		 ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
				 ICF_CC_SSA_OFFER_CHANGE)
            }
    
            ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_OFFER_CHANGE_REQ)
                
            p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
        }        
        else
        {
            /* Raise alarm of Unexpected event */
            ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
        }        
    }/* Successful updated pending events */     

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_pa_cc_terminate_call_req_st *p_call_term_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    p_call_term_req = (icf_pa_cc_terminate_call_req_st*)
             (p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

    /* If call clear reason is not set, then set the release
     * cause provied by PA */
    if(ICF_CALL_CLEAR_REASON_INVALID == p_cc_pdb->p_call_ctx->release_cause)
    {
         p_cc_pdb->p_call_ctx->release_cause =
             p_call_term_req->call_clear_reason;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
     icf_return_t               ret_val = ICF_SUCCESS;
     icf_pa_cc_terminate_call_req_st *p_call_term_req = ICF_NULL;

     ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

     p_call_term_req = (icf_pa_cc_terminate_call_req_st*)
                    (p_cc_pdb->p_glb_pdb->p_recv_msg->payload);

     if(p_call_term_req->bit_mask & ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT)
     {
        if((p_call_term_req->response_code >= 400) && (p_call_term_req->response_code <= 699))
         {
           p_cc_pdb->p_call_ctx->response_code =
                  p_call_term_req->response_code;
         }
     }   

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_with_default_qos_values
 *
 * DESCRIPTION:     This function would populate the Qos parameters present
 *                  in local SDP structure with default Qos preconditions
 *                  before sending the initial INVITE, or upon receiving an INVITE.
 *					for outgoing INVITE, the values would be:
 *					local and remote send/recv current_status in local_sdp set as
 *					ICF_RESOURCE_NOT_RESERVED.
 *					local and remote send/recv desired_status in local_sdp set as
 *					ICF_RESOURCE_RESERVATION_MANDATORY
 *					local and remote send/recv confirmation_status in local_sdp set
 *					as ICF_RESOURCE_RESERVATION_CONF_NOT_REQ.
 *					for incoming INVITE, the values would be:
 *					local send/recv current_status in local_sdp set as
 *					ICF_RESOURCE_RESERVED
 *					remote send/recv current_status in local_sdp set as
 *					ICF_RESOURCE_NOT_RESERVED.
 *					local send/recv desired_status set to
 *					ICF_RESOURCE_RESERVATION_MANDATORY.
 *					remote send/recv desired_status set to
 *					ICF_RESOURCE_RESERVATION_NONE
 *					remote send/recv confirmation_status set to
 *					ICF_RESOURCE_RESERVATION_CONF_REQ if remote has desired mandatory 
 *					reservation at its end.
 *					
 *
 ******************************************************************************/
icf_return_t icf_cc_fill_local_sdp_with_default_qos_values(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Fetch the internal sdp stream from list in call context
    * iterate thru the list and fill qos values in local sdp
    * of each stream */
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
    icf_internal_list_st        *p_list = ICF_NULL;
    
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    while (ICF_NULL != p_list)
    {       
        p_sdp = 
            (icf_stream_sdp_info_st*)(p_list->p_payload);

		if(ICF_NULL == p_sdp->p_local_sdp)
		{
			p_list = p_list->p_next;
			continue;
		}

		p_sdp->p_local_sdp->local_send.current_status = 
									ICF_RESOURCE_NOT_RESERVED;
		p_sdp->p_local_sdp->local_recv.current_status =
									ICF_RESOURCE_NOT_RESERVED;
		
		p_sdp->p_local_sdp->remote_send.current_status = (icf_uint8_t)
			ICF_RESOURCE_NOT_RESERVED;
		p_sdp->p_local_sdp->remote_recv.current_status = (icf_uint8_t)
			ICF_RESOURCE_NOT_RESERVED;

		/* Initializing Desired Status */
		p_sdp->p_local_sdp->local_send.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_MANDATORY;
		p_sdp->p_local_sdp->local_recv.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_MANDATORY;

		if(ICF_NULL!= ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
							ICF_CALL_DIR))
		{
			/* remote status is set to NONE so as to facilitate the
			 * comparison and subsequent overwriting of the same */
			p_sdp->p_local_sdp->remote_send.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_NONE;
			p_sdp->p_local_sdp->remote_recv.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_NONE;
		}
		else
		{
			/* remote status is set to Mandatory in case of outgoing call */
			p_sdp->p_local_sdp->remote_send.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_MANDATORY;
			p_sdp->p_local_sdp->remote_recv.desired_status = (icf_uint8_t)
			ICF_RESOURCE_RESERVATION_MANDATORY;
		}

		/* Initializing Confirmation Status */
		/* remote status is set to CONF_REQ if the received SDP contained local desired 
		* status as MANDATORY. It remains CONF_NOT_REQ for the other two status types 
		* of OPTIONAL and NONE. This remote status is reset to CONF_NOT_REQ once an UPDATE
		* is received, for an incoming call, since we can change it only after comparing
		* it with remote received values */
		if(ICF_NULL!= ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
							ICF_CALL_DIR))
		{
			if((p_sdp->p_remote_received_sdp->local_send.desired_status == 
				ICF_RESOURCE_RESERVATION_MANDATORY) || 
			  (p_sdp->p_remote_received_sdp->local_recv.desired_status == 
				ICF_RESOURCE_RESERVATION_MANDATORY))
			{
				p_sdp->p_local_sdp->remote_send.confirmation_status = (icf_uint8_t)
					ICF_RESOURCE_RESERVATION_CONF_REQ;
				p_sdp->p_local_sdp->remote_recv.confirmation_status = (icf_uint8_t)
					ICF_RESOURCE_RESERVATION_CONF_REQ;
			}
			else
			{
				p_sdp->p_local_sdp->remote_send.confirmation_status = (icf_uint8_t)
					ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
				p_sdp->p_local_sdp->remote_recv.confirmation_status = (icf_uint8_t)
					ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
			}
		}
		else
		{
			/* for outgoing calls, default Conf_value will remain CONF_NOT_REQ */
			p_sdp->p_local_sdp->remote_send.confirmation_status = (icf_uint8_t)
						ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
			p_sdp->p_local_sdp->remote_recv.confirmation_status = (icf_uint8_t)
						ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
		}

		/* go to the next node in list */
		p_list = (icf_internal_list_st*)p_list->p_next;
	}

	return ret_val;
}

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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Fetch the internal sdp stream from list in call context
    * iterate thru the list and fill qos values in local sdp
    * of each stream */
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
    icf_internal_list_st        *p_list = ICF_NULL;
    
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    while (ICF_NULL != p_list)
    {       
        p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

		if(ICF_NULL == p_sdp->p_local_sdp)
		{
			p_list = p_list->p_next;
			continue;
		}    

		/* Updating Current status lines in remote send and remote recv 
		 * direction in local SDP
		 */
		p_sdp->p_local_sdp->remote_send.current_status = 
			    p_sdp->p_remote_received_sdp->local_send.current_status;
		p_sdp->p_local_sdp->remote_recv.current_status = 
			    p_sdp->p_remote_received_sdp->local_recv.current_status;
    
		/* Updating Desired Status lines in Local SDP */
		/* Desired status will be updated on the basis of strength-tag. Higher
		 * strength will replace the one with lower strength. e.g - Mandatory
		 * strength can replace None strength but not vice-versa. Refer RFC - 3312
		 * for this. In our case higher strengths have low numerical value so
		 * we will compare numerical values and the lower numerical value
		 * replaces higher numerical value
		 */
		p_sdp->p_local_sdp->local_send.desired_status = (icf_uint8_t) ((
			    p_sdp->p_local_sdp->local_send.desired_status <=
				p_sdp->p_remote_received_sdp->remote_send.desired_status) ?
			(p_sdp->p_local_sdp->local_send.desired_status):
			(p_sdp->p_remote_received_sdp->remote_send.desired_status));
        
		p_sdp->p_local_sdp->local_recv.desired_status = (icf_uint8_t) ((
			    p_sdp->p_local_sdp->local_recv.desired_status <=
				p_sdp->p_remote_received_sdp->remote_recv.desired_status) ?
			(p_sdp->p_local_sdp->local_recv.desired_status):
			(p_sdp->p_remote_received_sdp->remote_recv.desired_status));
    
		p_sdp->p_local_sdp->remote_send.desired_status = (icf_uint8_t) ((
			    p_sdp->p_local_sdp->remote_send.desired_status <=
				p_sdp->p_remote_received_sdp->local_send.desired_status) ?
			(p_sdp->p_local_sdp->remote_send.desired_status):
			(p_sdp->p_remote_received_sdp->local_send.desired_status));
    
		p_sdp->p_local_sdp->remote_recv.desired_status = (icf_uint8_t) ((
			    p_sdp->p_local_sdp->remote_recv.desired_status <=
				p_sdp->p_remote_received_sdp->local_recv.desired_status) ?
			(p_sdp->p_local_sdp->remote_recv.desired_status):
			(p_sdp->p_remote_received_sdp->local_recv.desired_status));

		/* go to the next node in list */
		p_list = (icf_internal_list_st*)p_list->p_next;
	}
	    
    return ret_val;
}

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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /* Fetch the internal sdp stream from list in call context
     * iterate thru the list and fill qos values in local sdp
     * of each stream */
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
    icf_internal_list_st        *p_list = ICF_NULL;
    
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    while (ICF_NULL != p_list)
    {       
        p_sdp = 
            (icf_stream_sdp_info_st*)(p_list->p_payload);
		
		if(ICF_NULL == p_sdp->p_local_sdp)
		{
			p_list = p_list->p_next;
			continue;
		}    

		if(p_sdp->stream_id == ICF_STREAM_TYPE_AUDIO || 
			p_sdp->stream_id == ICF_STREAM_TYPE_VIDEO)
		{
			if ((p_sdp->p_local_sdp->local_send.current_status <=
				p_sdp->p_local_sdp->local_send.desired_status) &&
			   (p_sdp->p_local_sdp->local_recv.current_status <=
			    p_sdp->p_local_sdp->local_recv.desired_status))
			{
				ret_val = ICF_SUCCESS;
			}
			else
			{
				ret_val = ICF_FAILURE;
			}
		}

		if (ICF_FAILURE == ret_val)
            break;

		/* go to the next node in list */
		p_list = (icf_internal_list_st*)p_list->p_next;

	}

    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_match_curr_des_status_on_remote_nw
 *
 * DESCRIPTION:     This function would compare current and desired status
 *                  lines of local SDP on remote network and will return ICF_SUCCESS
 *					if current status exceeds or matches desired status. This 
 *					function is basically used to check whether or not 200 OK for an 
 *                  UPDATE request can be sent for QOS processing.
 *
 ******************************************************************************/
icf_return_t icf_cc_match_curr_des_status_on_remote_nw(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /*Fetch the internal sdp stream from list in call context
    * iterate thru the list and fill qos values in local sdp
    * of each stream*/
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
    icf_internal_list_st        *p_list = ICF_NULL;
    
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    while (ICF_NULL != p_list)
    {       
        p_sdp = 
            (icf_stream_sdp_info_st*)(p_list->p_payload);

		if(ICF_NULL == p_sdp->p_local_sdp)
		{
			p_list = p_list->p_next;
			continue;
		}    

        /* Checking for local send direction 
        if ((ICF_RESOURCE_RESERVATION_MANDATORY == 
            p_sdp->p_local_sdp->local_send.desired_status) && 
            (ICF_RESOURCE_NOT_RESERVED == 
            p_sdp->p_local_sdp->local_send.current_status))
        {
            ret_val = ICF_FAILURE;
        }
         Checking for local recv direction 
        else if ((ICF_RESOURCE_RESERVATION_MANDATORY == 
            p_sdp->p_local_sdp->local_recv.desired_status) && 
            (ICF_RESOURCE_NOT_RESERVED == 
            p_sdp->p_local_sdp->local_recv.current_status))
        {
            ret_val = ICF_FAILURE;
        } */

        /* Checking for remote send direction */
		if ((ICF_RESOURCE_RESERVATION_MANDATORY == 
            p_sdp->p_local_sdp->remote_send.desired_status) && 
            (ICF_RESOURCE_NOT_RESERVED == 
            p_sdp->p_local_sdp->remote_send.current_status))
        {
            ret_val = ICF_FAILURE;
        } 
        /* Checking for remote recv direction */
        else if ((ICF_RESOURCE_RESERVATION_MANDATORY == 
            p_sdp->p_local_sdp->remote_recv.desired_status) && 
            (ICF_RESOURCE_NOT_RESERVED == 
            p_sdp->p_local_sdp->remote_recv.current_status))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = ICF_SUCCESS;
        }

        if (ICF_FAILURE == ret_val)
            break;
        
        p_list = (icf_internal_list_st*)p_list->p_next;
    }

    return ret_val;
}

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
		INOUT icf_cc_pdb_st *p_cc_pdb)
{
	icf_return_t  ret_val = ICF_SUCCESS;

	/* Fetch the internal sdp stream from list in call context
	 * iterate thru the list and fill qos values in local sdp
	 * of each stream */
	icf_stream_sdp_info_st      *p_sdp = ICF_NULL;                
	icf_internal_list_st        *p_list = ICF_NULL;
    
	p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
	while (ICF_NULL != p_list)
	{       
		p_sdp = (icf_stream_sdp_info_st*)(p_list->p_payload);

		if(ICF_NULL == p_sdp->p_local_sdp)
		{
			p_list = p_list->p_next;
			continue;
		}    

		/* Initializing Current Status */
		/* in case of incoming call,we will set the local current status as RESERVED,
		 * since resource reservation has already been completed.this will ensure 
		 * that 183 contains current status as RESERVED */
			   
		p_sdp->p_local_sdp->local_send.current_status = 
								 ICF_RESOURCE_RESERVED;
		p_sdp->p_local_sdp->local_recv.current_status = 
								 ICF_RESOURCE_RESERVED;

		p_list = (icf_internal_list_st*)p_list->p_next;
	}

	return ret_val;
		
}

#endif


/******************************************************************************
 *
 * FUNCTION:        icf_cc_set_annonymous_local_addr
 * DESCRIPTION:     This function set the annonymous addr in local addr

 *
 *
 *****************************************************************************/
icf_return_t icf_cc_set_annonymous_local_addr(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    /* Fix for CSR-1-4776701  */
     icf_port_strcpy(p_cc_pdb->p_call_ctx->local_name.str,(icf_uint8_t *)"anonymous");
	p_cc_pdb->p_call_ctx->local_name.str_len = 
	      (icf_uint16_t) icf_port_strlen(p_cc_pdb->p_call_ctx->local_name.str);
          /*  Fix for CSR 1-7576638 */
          /*  Fill the address type as SIP uri when the address type
           *  is not set as SIPS_URL. If address type is SIPS URI then 
           *  do not chnage the address type as SIP URI */
          if (ICF_ADDRESS_PLAN_SIPS_URI != p_cc_pdb->p_call_ctx->local_address.addr_type)
          {
          p_cc_pdb->p_call_ctx->local_address.addr_type =
              ICF_ADDRESS_PLAN_SIP_URI;
          }
          icf_port_strcpy(
              p_cc_pdb->p_call_ctx->local_address.addr_val.str,
              (icf_uint8_t *)"anonymous@anonymous.invalid");

           p_cc_pdb->p_call_ctx->local_address.addr_val.str_len =
               (icf_uint16_t) icf_port_strlen(
               p_cc_pdb->p_call_ctx->local_address.addr_val.str);
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




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
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    
    /* ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_CALL_FAIL_NO_SDP) */
    ICF_CC_SET_ECODE(p_cc_pdb, ICF_ERROR_CALL_FAIL_NO_SDP)
                
    /* Send DMS Request to MMI */
    p_cc_pdb->forceful_delete = ICF_FALSE;

    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    
    p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_MEDIA_MISMATCH; 
    /*
     * Stop the call setup timer and start the release timer
     */
    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb
                                                               ))
    {
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Update the call state
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
     }
     
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_chk_prack_req_media_param
 *
 * DESCRIPTION:     This function is used to send a prack to SSA in case
 *                  remote party want reliable resp for provisional resp 183
 *
 ******************************************************************************/
icf_return_t  icf_cc_chk_prack_req_media_param(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    
        /* If bitmask is set that RPR is required, send the prack to remote*/
    if(ICF_NW_INC_MEDIA_PARAM_RPR_REQD & (((icf_nw_inc_media_param_st *)
                  (p_cc_pdb->p_internal_msg->p_msg_data))->bitmask))
    {
        ret_val = icf_cc_send_ssa_media_prov_resp(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_prack_req_alert
 *
 * DESCRIPTION:     This function is used to send a prack to SSA in case
 *                  remote party want reliable resp for provisional resp 180
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_prack_req_alert(
		INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
		
    /* If bitmask is set that RPR is required, send the prack to remote*/
	if(ICF_NW_INC_ALERT_RPR_REQD &(((icf_nw_inc_call_alert_st *)
			(p_cc_pdb->p_internal_msg->p_msg_data))->bitmask))
	{
		ret_val = icf_cc_send_ssa_alert_prov_resp(p_cc_pdb);
	}
		
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
	
	return ret_val;    
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_mms_rsp_pa_connect
 *
 * DESCRIPTION:     This function is used to send a 200 OK on receciving mms
 *                  response for PA call connect 
 *
 ******************************************************************************/
icf_return_t  icf_cc_mms_rsp_pa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    /* Fix for CSR 1-5160601 */
    /* Also check whether SDP has been sent or not */
    if ((ICF_FALSE == p_cc_pdb->p_call_ctx->media_updated) ||
            (ICF_NULL == (ICF_CC_FINAL_SDP_SENT & 
                          p_cc_pdb->p_call_ctx->common_bitmask)))        
    {
        p_cc_pdb->media_to_be_sent = ICF_TRUE;
    } /* if(ICF_FALSE == p_cc_pdb->p... */
    else
    {
        p_cc_pdb->media_to_be_sent = ICF_FALSE;
    } /* else */

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_TO_BE_SENT)
    
    /*
     * Send CONNECT to the SSA
     */
    if (ICF_FAILURE == icf_cc_send_ssa_connect(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_REQ))
    {
        ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
        if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {

        /*If pending_event is offer_change, then send MMS request
        *for offer change and remain in the same state and set
            *next_state as 11_connect_ack*/
            ret_val = icf_cc_send_mms_req(p_cc_pdb);
            
            if (ICF_SUCCESS == ret_val)
            {
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                    ICF_CC_11_AWT_SSA_CONNECT_ACK)
                    
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                    ICF_CC_SSA_OFFER_CHANGE)
                    
                    ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_REQ)            
                    
                    p_cc_pdb->p_call_ctx->update_reqd = ICF_FALSE;
                /*REL6.0: if any other pending_event raise alarm*/
            }
        }
    }
    else
    {
        /* Update the states in the call context */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
    }
        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_snd_mms_pa_connect
 *
 * DESCRIPTION:     This function is used to send mms to mmi for for PA call 
 *                  connect 
 *
 ******************************************************************************/
icf_return_t  icf_cc_snd_mms_pa_connect(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

     ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
             ICF_PENDING_EVENT_PA_CALL_CONNECT)

     p_cc_pdb->pa_connect_ong = ICF_TRUE;

    /* 
     * Mode claculation needs to be done here
     */
     
    /* Send MMS Request to MMI */
    if(ICF_FAILURE == icf_cc_pa_connect_send_mms_req(p_cc_pdb))
    {        
        ret_val = ICF_FAILURE;
    }
    else if(ICF_TRUE == p_cc_pdb->mms_sent)
    {
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
        
        ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)

        ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                                ICF_CC_PA_CALL_CONNECT)

    }
    else if (ICF_FALSE == p_cc_pdb->mms_sent)
    {
        ICF_CC_RESET_ONGOING_EVENT(p_cc_pdb)

        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_11_AWT_SSA_CONNECT_ACK)
		
        ret_val = icf_cc_mms_rsp_pa_connect(p_cc_pdb);
    }
            
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_timer_id
 *
 * DESCRIPTION:     This function resets the timer id for the expired timer in 
 *                  current call context
 *
 ******************************************************************************/
icf_return_t icf_cc_reset_timer_id(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id;
    icf_uint8_t                      ctr;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_TIMER_EXPIRED)
    
    /* Iterate through the list of timers maint:e!
     * ained by CC */
    for (ctr=0; ctr<ICF_CC_NUMBER_OF_TIMERS; ctr++)
    {
        timer_id = p_cc_pdb->p_call_ctx->timer_list.timer_id[ctr];

        /* check if the timer is running or not */
        if (ICF_TIMER_ID_INVALID != timer_id)
        {
            /* Non-zero id means that the timer is running, so stop it */

            p_cc_pdb->timer_type = ctr;
            p_cc_pdb->timer_id = timer_id;

            /* reset the timer id in call context */
            p_cc_pdb->p_call_ctx->timer_list.timer_id[ctr] =
                ICF_TIMER_ID_INVALID;            
        } /* end of if for timer_id */

    } /* end of for loop */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_reset_timer_id */

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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t               ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    /*
     * If the release cause is ICF_CALL_CLEAR_REASON_USER_BUSY the check for 
     * call dbm's function to check services active 
     */ 
    if(ICF_CALL_CLEAR_REASON_USER_BUSY == 
            p_cc_pdb->p_call_ctx->release_cause || 
            ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE == 
            p_cc_pdb->p_call_ctx->release_cause)
    {
        /*
         * Call DBM's function to check service activated on line
         */ 
        ret_val = icf_dbm_chk_line_service(
                p_cc_pdb->p_glb_pdb,
                (icf_uint8_t)ICF_CAUSE_CALL_REJECT_BY_USER,
                p_cc_pdb->p_call_ctx->line_id,
                &p_cc_pdb->p_internal_msg->msg_hdr.status,
                p_cc_pdb->p_ecode);
        /*
         * Check if the call was a call waiting call.If so decrement call 
         * waiting count
         */
        if(ICF_SUCCESS == ret_val && 
                ICF_STATUS_CALL_FWD_BUSY_LINE == 
                p_cc_pdb->p_internal_msg->msg_hdr.status)
        {

            if(ICF_TRUE == p_cc_pdb->p_call_ctx->call_waiting_call)
            {
                icf_dbm_decrease_call_waiting(
                        p_cc_pdb->p_glb_pdb,
                        p_cc_pdb->p_call_ctx->line_id,
                        p_cc_pdb->p_ecode);   
                p_cc_pdb->p_call_ctx->call_waiting_call = ICF_FALSE;             
            }
        }
    } 

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_div_info
 *
 * DESCRIPTION:     This function would clear the diversion information from 
 *                  call context
 *
 ******************************************************************************/
icf_return_t icf_cc_clear_div_info(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_diversion_data_st            *p_div_data = ICF_NULL;
    icf_diversion_data_st            *p_div_data_temp = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
      

    /* Free the diversion header list - This is a list */
    if(ICF_NULL != p_cc_pdb->p_call_ctx->p_diversion_header_list)
    {
                  
        /* Free the contact addr if present */
        if(ICF_NULL != p_cc_pdb->p_call_ctx->
                p_diversion_header_list->p_contact_addr)
        {
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_cc_pdb->p_call_ctx->p_diversion_header_list->
                    p_contact_addr, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, ret_val)

            p_cc_pdb->p_call_ctx->p_diversion_header_list->p_contact_addr
                    = ICF_NULL;
        }
        
        /* Free the contact name if present */
        if(ICF_NULL != p_cc_pdb->p_call_ctx->
                p_diversion_header_list->p_contact_name)
        {
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_cc_pdb->p_call_ctx->p_diversion_header_list->
                    p_contact_name, ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode, ret_val)

            p_cc_pdb->p_call_ctx->p_diversion_header_list->p_contact_name
                    = ICF_NULL;
        }

        /*Read the next element in list */
        p_div_data = 
            p_cc_pdb->p_call_ctx->p_diversion_header_list->p_diversion_data;
  
       if(ICF_NULL !=  p_div_data)
       {
           /* If next element is not null, copy it in list */ 
           while(ICF_NULL != p_div_data->p_next)
           {
               p_div_data_temp = p_div_data->p_next;

               ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                      p_div_data, ICF_MEM_COMMON, p_cc_pdb->p_ecode, ret_val)
               p_div_data = p_div_data_temp;
           } 
           ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_div_data,
                ICF_MEM_COMMON, p_cc_pdb->p_ecode, ret_val)
           p_cc_pdb->p_call_ctx->p_diversion_header_list->p_diversion_data =
               ICF_NULL;
       }
        
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->p_diversion_header_list,
                ICF_MEM_COMMON, p_cc_pdb->p_ecode, ret_val)

        p_cc_pdb->p_call_ctx->p_diversion_header_list = ICF_NULL;
    }

    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_clear_div_info */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_handle_reinvite_ack
 *
 * DESCRIPTION:     This function is used to handle reinvite ack           
 *
 ******************************************************************************/
icf_return_t  icf_cc_handle_reinvite_ack(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
           
    /*Reinvite could only be received in a connected call so if call was 
     *connected RESET the Signaling ongoing flag*/
    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                    ICF_CALL_CONNECT_DONE))
    {
        ICF_CC_RESET_SIG_ONGOING(p_cc_pdb)
    
        /*Return status signaling ongoing ended to CM*/
        ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_SIGNAL_ONGOING_ENDED)  
    }
if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                         ICF_CALL_MOD_REQ_WO_SDP))
{
    if(p_cc_pdb->p_internal_msg->msg_hdr.payload_length)
    { 
        icf_nw_inc_media_change_ack_st *p_call_modify_ack = 
                (icf_nw_inc_media_change_ack_st *)p_cc_pdb->
                        p_internal_msg->p_msg_data;

        p_cc_pdb->p_list_1 = p_call_modify_ack->p_sdp_list;

        if(ICF_FAILURE == icf_cc_make_mms_type_for_nth_rem_succ_ans(p_cc_pdb))
        {
            ret_val = ICF_FAILURE;
        }
		else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
		{
			ret_val = ICF_FAILURE;
		}
		else
		{
			if(ICF_TRUE == p_cc_pdb->mms_sent)
			{
				/*
				* Update the states in the call context
				*/
				ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
				ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
				ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,p_cc_pdb->current_event)
				/* Start the media timer  */
				ret_val = icf_cc_start_media_timer(p_cc_pdb);
			}
                        else
                        {
                              p_cc_pdb->common_bitmask |= ICF_CC_CALL_MODIFY_CFM_NT_REQD;

			      if(ICF_FAILURE == icf_cc_send_pa_call_modify_ind(p_cc_pdb))
        		      {
			           ret_val = ICF_FAILURE;
        		      }

                        }
			
		}/* MMS was handled succesfully */

    }
    
    /********** Changes for Re-Invite w/o SDP Start (Rel7.0) **********/

    /* Code added to check if ReINVITE without SDP is received and there 
    * is no SDP in ACK then clear the call.*/

    else
    {
         if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                         ICF_CALL_MOD_REQ_WO_SDP))
          {

                      ret_val = icf_cc_06_generic_err(p_cc_pdb);
          }
    }
}

   /*reset the common bitmask if it is set as ICF_CALL_MOD_REQ_WO_SDP*/

      if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK
                           (p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP))
      {
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,
                                          ICF_CALL_MOD_REQ_WO_SDP)
      }

   
    /*********** Changes for Re-Invite w/o SDP End (Rel7.0) ***********/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}

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
        INOUT	icf_cc_pdb_st		*p_cc_pdb)
{
    icf_return_t						ret_val = ICF_SUCCESS;
	icf_pa_cc_connect_req_st			*p_connect = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

	p_connect = (icf_pa_cc_connect_req_st *)
						p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

    /* 
     * Check the rejected streams present in connect request 
     * and populate the same in the rejected stream array in call context 
     */
     if (ICF_NULL != p_connect->count )
     {
        icf_uint8_t count =0;
        /* Some deleted streams are present */
        for ( count =0;count<p_connect->count;count ++)
        {
            p_cc_pdb->p_call_ctx->rejected_stream_list
                [p_cc_pdb->p_call_ctx->rejected_stream_count++]=
                p_connect->deleted_media_streams[count];
        }
    }

	/* check if privacy header needs to be sent */
	if (ICF_PRIVACY_IND_PRESENT & p_connect->bit_mask)
	{
		p_cc_pdb->p_call_ctx->send_privacy_hdr = p_connect->privacy_ind;
	}

	/* clone the header list into call ctx, if received from app */
	if (ICF_HDR_LIST_PRESENT & p_connect->bit_mask)
	{
        ret_val = icf_cmn_free_header_list(p_cc_pdb->p_glb_pdb,
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);	
   
      	ret_val = icf_cmn_clone_header_list(
					p_cc_pdb->p_glb_pdb,
					&(p_connect->header_list),
					&(p_cc_pdb->p_call_ctx->header_list),
					p_cc_pdb->p_ecode);
	}

	/* clone the tag list into call ctx, if received from app */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_TAG_LIST_PRESENT & p_connect->bit_mask))
	{
		ret_val = icf_cmn_clone_tag_list(
					p_cc_pdb->p_glb_pdb,
					&(p_connect->tag_list),
					&(p_cc_pdb->p_call_ctx->tag_list),
					p_cc_pdb->p_ecode);
	}

	/* clone the body list into call ctx, if received from app */
	if ((ICF_SUCCESS == ret_val) && 
		(ICF_BODY_LIST_PRESENT & p_connect->bit_mask))
	{
		ret_val = icf_cmn_clone_body_list(
					p_cc_pdb->p_glb_pdb,
					&(p_connect->body_list),
					&(p_cc_pdb->p_call_ctx->body_list),
					p_cc_pdb->p_ecode);
	}
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;
} /* icf_cc_proc_pa_connect_payload() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_info_timer
 *
 * DESCRIPTION:     This function starts Setup timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_info_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_INFO;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* Now, start the Setup timer */
        ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                p_config_data->info_timer, p_cc_pdb->p_ecode, ret_val)
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->info_timer_id = timer_id;
        p_cc_pdb->p_call_ctx->p_info_timer_data = p_timer_data;
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_INFO;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                p_timer_data, p_cc_pdb->p_ecode);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_info_timer() */


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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_INFO_CFM;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* Now, start the info confirmation timer, but we will start it for the same duration
         * that is being used for info_timer for outgoing info_req.
         */
        ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                p_config_data->info_timer, p_cc_pdb->p_ecode, ret_val)
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->info_cfm_timer_id = timer_id;
        p_cc_pdb->p_call_ctx->p_info_cfm_timer_data = p_timer_data;
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_INFO_CFM;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_START_TIMER)
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                p_timer_data, p_cc_pdb->p_ecode);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_info_cfm_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_info_timer
 *
 * DESCRIPTION:     This function stops info timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_info_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    timer_id = p_cc_pdb->p_call_ctx->info_timer_id;

    /* check if the timer is running or not */
    if (ICF_TIMER_ID_INVALID != timer_id)
    {
        /* Non-zero id means that the timer is running, so stop it */
        ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, timer_id, p_cc_pdb->p_ecode, 
                ret_val)

        if (ICF_SUCCESS == ret_val)
        {
            p_cc_pdb->timer_type = ICF_TIMER_TYPE_INFO;
            p_cc_pdb->timer_id = timer_id;

            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_STOP_TIMER)

            /* reset the timer id in call context */
            p_cc_pdb->p_call_ctx->info_timer_id = ICF_TIMER_ID_INVALID;              
		}
        /* Free timer data */
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                p_cc_pdb->p_call_ctx->p_info_timer_data, p_cc_pdb->p_ecode);
            p_cc_pdb->p_call_ctx->p_info_timer_data = ICF_NULL;

    } /* end of if for timer_id */


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_stop_info_timer() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stop_info_cfm_timer
 *
 * DESCRIPTION:     This function stops info confirmation timer, which was started 
 *                  while giving info_ind to application.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_info_cfm_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_id_t                   timer_id;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    timer_id = p_cc_pdb->p_call_ctx->info_cfm_timer_id;

    /* check if the timer is running or not */
    if (ICF_TIMER_ID_INVALID != timer_id)
    {
        /* Non-zero id means that the timer is running, so stop it */
        ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, timer_id, p_cc_pdb->p_ecode, 
                ret_val)

        if (ICF_SUCCESS == ret_val)
        {
            p_cc_pdb->timer_type = ICF_TIMER_TYPE_INFO_CFM;
            p_cc_pdb->timer_id = timer_id;

            ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_STOP_TIMER)

            /* reset the timer id in call context */
            p_cc_pdb->p_call_ctx->info_cfm_timer_id = ICF_TIMER_ID_INVALID;              
		}
        /* Free timer data */
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_TIMER,
                p_cc_pdb->p_call_ctx->p_info_cfm_timer_data, p_cc_pdb->p_ecode);
        
        p_cc_pdb->p_call_ctx->p_info_cfm_timer_data = ICF_NULL;

    } /* end of if for timer_id */


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_stop_info_cfm_timer() */




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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_boolean_t                    ret_val = ICF_SUCCESS;
    icf_pa_cc_info_req_st           *p_pa_info_req = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

     p_pa_info_req = 
       (icf_pa_cc_info_req_st *)p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
    
    /*If bodylist is not present or count is 0 then return failure*/
    if(ICF_BODY_LIST_PRESENT & p_pa_info_req->bit_mask)
    {
        if (0 == p_pa_info_req->body_list.count)
            ret_val = ICF_FAILURE;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }

    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*ICF_SESSION_CONTROL_END*/

ICF_HANDLE_REINVITE_START
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
        INOUT	icf_cc_pdb_st		*p_cc_pdb)
{
    icf_return_t						ret_val = ICF_SUCCESS;

	icf_call_modify_cfm_st	*p_call_modify_cfm = ICF_NULL;
	icf_call_modify_req_st *p_call_modify_req = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

	if(ICF_CC_PA_CALL_MODIFY_REQ == p_cc_pdb->current_event)
	{
		p_call_modify_req = (icf_call_modify_req_st *)
			p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

        /* clone the header list into call ctx, if received from app */
		if (ICF_HDR_LIST_PRESENT & p_call_modify_req->bit_mask)
		{
			ret_val = icf_cmn_clone_header_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_req->header_list),
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);
		}
		
		/* clone the tag list into call ctx, if received from app */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_TAG_LIST_PRESENT & p_call_modify_req->bit_mask))
		{
			ret_val = icf_cmn_clone_tag_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_req->tag_list),
				&(p_cc_pdb->p_call_ctx->tag_list),
				p_cc_pdb->p_ecode);
		}
		
		/* clone the body list into call ctx, if received from app */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_BODY_LIST_PRESENT & p_call_modify_req->bit_mask))
		{
			ret_val = icf_cmn_clone_body_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_req->body_list),
				&(p_cc_pdb->p_call_ctx->body_list),
				p_cc_pdb->p_ecode);
		}

	}
	else if(ICF_CC_PA_CALL_MODIFY_RSP == p_cc_pdb->current_event)
	{
		p_call_modify_cfm = (icf_call_modify_cfm_st *)
			p_cc_pdb->p_glb_pdb->p_recv_msg->payload;
		
		
		/* clone the header list into call ctx, if received from app */
		if (ICF_HDR_LIST_PRESENT & p_call_modify_cfm->bit_mask)
		{
			ret_val = icf_cmn_clone_header_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_cfm->header_list),
				&(p_cc_pdb->p_call_ctx->header_list),
				p_cc_pdb->p_ecode);
		}
		
		/* clone the tag list into call ctx, if received from app */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_TAG_LIST_PRESENT & p_call_modify_cfm->bit_mask))
		{
			ret_val = icf_cmn_clone_tag_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_cfm->tag_list),
				&(p_cc_pdb->p_call_ctx->tag_list),
				p_cc_pdb->p_ecode);
		}
		
		/* clone the body list into call ctx, if received from app */
		if ((ICF_SUCCESS == ret_val) && 
			(ICF_BODY_LIST_PRESENT & p_call_modify_cfm->bit_mask))
		{
			ret_val = icf_cmn_clone_body_list(
				p_cc_pdb->p_glb_pdb,
				&(p_call_modify_cfm->body_list),
				&(p_cc_pdb->p_call_ctx->body_list),
				p_cc_pdb->p_ecode);
		}

		/* Fix for CSR 1-5924713 SPR 18519: Check the call modify wo SDP bitmask
		 * If set and signaling mode received in call modify confirm from application
		 * then set the media mode and application mode to signalling mode received.
		 * this will be set for all the streams.
		 */
        if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MOD_REQ_WO_SDP))
        {
            /* This bitmask will be set in the Confirm only if this is 
               The application has taken the decision with its logic that
               it needs to overwrite the IPTK logic functionality*/
            if((ICF_SUCCESS == ret_val) && 
               (ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT & 
                p_call_modify_cfm->bit_mask))
            {
                icf_stream_sdp_info_st  *p_sdp = ICF_NULL;
                icf_internal_list_st    *p_list = ICF_NULL;
				p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

				while (ICF_NULL != p_list)
				{
						p_sdp =
								(icf_stream_sdp_info_st*)(p_list->p_payload);

						if(ICF_NULL != p_sdp->p_local_sdp)
						{
								p_sdp->p_local_sdp->media_mode =
										p_call_modify_cfm->signaling_mode;
						}

						p_sdp->application_mode = 
								p_call_modify_cfm->signaling_mode;

						/* If mode received is send-recv or recvonly then 
						 * reset the local held assuming that application
						 * wants to resume the call.
						 */
						if((ICF_MEDIA_MODE_SEND_RECV ==
										p_call_modify_cfm->signaling_mode)||
							(ICF_MEDIA_MODE_RECV_ONLY ==
							 			p_call_modify_cfm->signaling_mode))
						{
								p_cc_pdb->p_call_ctx->media_state &=
										~ICF_MEDIA_STATE_LOCAL_HELD;
						}

						p_list = (icf_internal_list_st *)p_list->p_next;
				}
			}
		}

	}

   	
	
	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;
} /* icf_cc_proc_pa_media_change_payload() */


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_committed_sdp
 *
 * DESCRIPTION:     This function copies the current commited SDP data into current call 
 *                  context's p_last_commited_sdp
 *
 ******************************************************************************/
icf_return_t  icf_cc_copy_committed_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st      *p_stream_sdp_info = ICF_NULL;                
    icf_internal_list_st                 *p_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
	    p_stream_sdp_info = 
		    (icf_stream_sdp_info_st*)(p_list->p_payload);
        /* SPR14647: Added the stream_status Active check & null check for source *
         * stream in icf_cc_copy_internal_sdp_stream                              */
         if((p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE) &&
            (ICF_NULL != p_stream_sdp_info->p_local_sdp))
          {  

             if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb, 
						    	p_stream_sdp_info->p_local_sdp, /*Source*/
						    	&(p_stream_sdp_info->p_last_commited_sdp) 
							    /*Destination*/))
		      {
					return ICF_FAILURE;
		      }
          }    

	    p_list = (icf_internal_list_st*)p_list->p_next;
    }

        
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}

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
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    
                
    /* Send DMS Request to MMI */
    p_cc_pdb->forceful_delete = ICF_FALSE;
    
    /*p_cc_pdb->p_call_ctx->release_cause = ICF_CALL_CLEAR_MEDIA_MISMATCH; */
    /*
     * Stop the call setup timer and start the release timer
     */
    if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
	{
		ret_val = ICF_FAILURE;
	} /* if(ICF_FAILURE == icf_cc... */
	else
    {
         /*
          * Update the states in the call context
          */
         ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
         /*
          * Send BYE to network if signaling ongoing is FALSE
          */
         ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_SIG_ONGOING)
         if(ICF_INT_MODULE_INVALID == 
                 ICF_CC_GET_SIG_ONGOING(p_cc_pdb))
         {
             /*
              * Send call clear request to the SSA
              */
             if(ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
             {
                 ret_val = ICF_FAILURE;
             } /* if(ICF_FAILURE == icf_cc... */
             else
             {
                 /*
                  * Update the next state in the call context
                  */
                 ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                         ICF_CC_08_AWT_CALL_TERMINATE_RESP)
             } /* else */
         } /* if(ICF_NULL == ICF_CC_GET_SIG_ON... */
         else
         {
             /*Return service terminate status to CM*/
            ICF_CC_SET_STATUS(p_cc_pdb,
                    ICF_STATUS_CALL_R_SRV_TERMINATE)
             /*
              * Mark next state as ICF_CC_12_WAIT_START_PROC_SIG
              */
             ICF_CC_SET_NEXT_STATE(p_cc_pdb,
                     ICF_CC_12_WAIT_START_PROC_SIG)

			ICF_CC_SET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_CALL_TERM_REQ)
            /*
             * TO DO indicate to CM
             */
         } /* else */
     } /* else */
     
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

    return ret_val;    
                
}

ICF_HANDLE_REINVITE_END

#if 0
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
        INOUT   icf_uint8_t *p_stream_type)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_internal_list_st *p_sdp_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    icf_internal_list_st *p_sdp_node = icf_cmn_get_node_in_internal_list(
                            p_sdp_list,&p_cc_pdb->stream_id);
    icf_stream_sdp_info_st *p_sdp_info = ICF_NULL; 
	
    if(ICF_NULL == p_sdp_node)
    {
	return ICF_FAILURE;
    }

    p_sdp_info = (icf_stream_sdp_info_st *)p_sdp_node->p_payload;

    switch(p_sdp_info->stream_type)
    {
    case ICF_STREAM_TYPE_AUDIO:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_AUDIO");
        break;

    case ICF_STREAM_TYPE_VIDEO:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_VIDEO");
        break;

    case ICF_STREAM_TYPE_TBCP:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_TBCP");
        break;

    case ICF_STREAM_TYPE_FAX:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_FAX");
        break;

    }

    return ret_val;    
                
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_stream_type_to_str
 *
 * DESCRIPTION:     This fucntion converts the stream type to string value
 *                  
 ******************************************************************************/
icf_return_t  icf_cc_stream_type_to_str(
        INOUT  icf_uint8_t  stream_type,
        INOUT   icf_uint8_t *p_stream_type)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    switch(stream_type)
    {
    case ICF_STREAM_TYPE_AUDIO:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_AUDIO");
        break;

    case ICF_STREAM_TYPE_VIDEO:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_VIDEO");
        break;

    case ICF_STREAM_TYPE_TBCP:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_TBCP");
        break;

    case ICF_STREAM_TYPE_FAX:
        icf_port_strcpy(p_stream_type,"ICF_STREAM_TYPE_FAX");
        break;

    }

    return ret_val;    
                
}
#endif

/*SPR-13995 Aman 04-01-07*/
/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_ssa_connect_failure 
 *
 * DESCRIPTION:     This function validate for the failure of 200ok. 
 *
 ******************************************************************************/
icf_return_t  icf_cc_validate_ssa_connect_failure(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_FAILURE;
    icf_nw_inc_connect_st  *p_connect_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

    p_connect_req = (icf_nw_inc_connect_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;

    if(ICF_NW_INC_CONNECT_GENERIC_ERROR == 
    	(p_connect_req->bitmask & ICF_NW_INC_CONNECT_GENERIC_ERROR))
    {
		ret_val = ICF_SUCCESS;
    }
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_handle_ssa_connect_failure
 *
 * DESCRIPTION:    This function handles the failure of 200ok.It sends ACK and 
 * 		   send BYE on the network. 			
 *
 ******************************************************************************/
icf_return_t  icf_cc_handle_ssa_connect_failure( 
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{ 
    icf_return_t  ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
 
     /* Set release type as internal error */
    ICF_CC_SET_RELEASE_TYPE(p_cc_pdb, ICF_REL_TYPE_INTERNAL_INITIATED)
    ICF_CC_TRACE(p_cc_pdb, ICF_TRACE_REL_TYPE_INT_ERR)
    
    if (ICF_FAILURE == icf_cc_send_ssa_connect_ack(p_cc_pdb))
    {
    	ret_val = ICF_FAILURE;
    }
    /*Start Release timer */
    else if(ICF_FAILURE == icf_cc_start_release_timer(p_cc_pdb))
    {
       ret_val = ICF_FAILURE;
    }
    /*
     * Send a DMS request to MMI
     */
    else if (ICF_FAILURE == icf_cc_send_dms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /*
     * Send call clear request to the SSA
     */
    else if (ICF_FAILURE == icf_cc_send_ssa_call_term_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } /* if (ICF_FAILURE == icf_c... */    
    else
    {
        /*
         * Update the states in the call context
         */
        ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_07_AWT_DMS_RESP)
    }
	
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
    return ret_val;
}


icf_return_t
icf_cc_is_any_line_without_user(INOUT icf_cc_pdb_st *p_cc_pdb,
                                OUT   icf_error_t    *p_ecode)
{
       
   icf_config_data_st          *p_config_data = ICF_NULL;
   icf_return_t                 ret_val = ICF_FAILURE;
   icf_line_data_st            *p_line_blk = ICF_NULL;
   icf_uint32_t                 line_count = 0, count = 0;
   icf_user_db_node_st          *p_user_db_node = ICF_NULL;


    /* Retreive the pointer for global CFG data*/
    ret_val = icf_dbm_get_module_glb_data(     
                p_cc_pdb->p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_FAILURE != ret_val))   
    ret_val = ICF_FAILURE;    

    line_count = p_config_data->max_lines;
    for (count = 0; count < line_count; count++)
    {
           if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_cc_pdb->p_glb_pdb,
                       (icf_line_id_t const)(count),
                        &p_line_blk,
                        p_ecode))
            {
                /* Check for next line block. It is possible that there
                 * may be holes in line data configuration. So leave
                 * that. */
                continue;
            }
           else
           {
                  p_user_db_node = p_line_blk->p_user_db;
                  while(ICF_NULL !=  p_user_db_node)
                     {
                       if (!icf_port_strncmp(p_user_db_node->public_user.addr_val.str, \
                            (icf_uint8_t*)ICF_WILDCARD_USER, icf_port_strlen((icf_uint8_t*)ICF_WILDCARD_USER)))
                         {
                          ret_val = ICF_SUCCESS;
                          break;
                         }
                      p_user_db_node = p_user_db_node->p_next;
                     }
                  if(ICF_SUCCESS ==  ret_val)
                  {
                   break;
                  }
               
           }
    }

    return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_abort_options_trans
 *
 * DESCRIPTION:     This function is used to delete the ssa transaction for OPTIONS
 *
 *
 ******************************************************************************/
icf_return_t
icf_cc_abort_options_trans(
        INOUT icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_internal_msg_st     internal_msg;

    p_glb_pdb = p_cc_pdb->p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)

    internal_msg.msg_hdr.p_glb_pdb = p_glb_pdb;

    internal_msg.msg_hdr.msg_id = ICF_NW_OTG_OPTIONS_ABORT_REQ;

    internal_msg.msg_hdr.payload_length = 0;

    ret_val = icf_ssa_process_mesg(&internal_msg);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
}

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_load_call_duration_timer
 *
 * DESCRIPTION:     This function starts load call duration timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_load_call_duration_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_LOAD_CALL_DURATION;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }

    if (ICF_SUCCESS == ret_val)
    {
	ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
       	ICF_CC_LOAD_CALL_DURATION_TIMER, p_cc_pdb->p_ecode, ret_val)
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->load_call_dur_timer_id = timer_id;

        p_cc_pdb->p_call_ctx->p_load_timer_data =
            (icf_void_t *)(p_timer_data);
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
       icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_timer_data, p_cc_pdb->p_ecode); 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of icf_cc_start_setup_timer() */
/*****************************************************************************
 *
 * FUNCTION:        icf_cc_stop_load_call_dur_timer
 *
 * DESCRIPTION:     This function stopscurrently running timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_stop_load_call_dur_timer(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->load_call_dur_timer_id, 
        p_cc_pdb->p_ecode, ret_val)

    /* free the memory allocated to any existing timer data block */
    if (ICF_NULL != p_cc_pdb->p_call_ctx->p_load_timer_data)
    {
      icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_cc_pdb->p_call_ctx->p_load_timer_data, p_cc_pdb->p_ecode);
        p_cc_pdb->p_call_ctx->p_load_timer_data = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_stop_current_timer() */

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_media_pending_events
 *
 * DESCRIPTION:     This function reset the Media_Connect/Alert
 *                  pending events.
 *
 ******************************************************************************/
icf_return_t icf_cc_reset_media_pending_events(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

     /* if update is ongoing event or pending evnet send error resp for same */
     if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ))
     {
         ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ)
     }
     else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                 ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP))
     {
         ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP)
     }
     else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ))
     {
         ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                 ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ)

     }
     else if(ICF_NULL != ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)) 
     {
         ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,
                 ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ)
     }
     else
     {
         ret_val = ICF_SUCCESS;
     }
    return ret_val;

}
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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*set error cause in cc_pdb*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MODIFY_RESP_FAIL)
    ret_val = icf_cc_send_pa_call_modify_resp(p_cc_pdb);

    /* Reset the failure cause to ICF_NULL*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

    return ret_val;
}
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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*set error cause in cc_pdb*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
    ret_val = icf_cc_send_pa_fax_resp(p_cc_pdb);

    /* Reset the failure cause to ICF_NULL*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

    return ret_val;
}
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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*set error cause in cc_pdb*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
    ret_val = icf_cc_send_pa_info_resp(p_cc_pdb);

    /* Reset the failure cause to ICF_NULL*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

    return ret_val;
}
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
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;

    /*set error cause in cc_pdb*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_ACTION;
    ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_FAX_RESP_FAIL)
    ret_val = icf_cc_send_pa_stop_fax_resp(p_cc_pdb);

    /* Reset the failure cause to ICF_NULL*/
    p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

    return ret_val;
}
#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_start_load_call_duration_timer
 *
 * DESCRIPTION:     This function starts load call duration timer.
 *
 ******************************************************************************/
icf_return_t icf_cc_start_load_call_duration_timer(
         INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
                   (icf_pool_id_t)ICF_MEM_TIMER,
                   (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))
    if (ICF_SUCCESS == ret_val)
    { 
       /* fill the timer data structure */
       p_timer_data->timer_type = ICF_TIMER_TYPE_LOAD_CALL_DURATION;
       p_timer_data->module_id = ICF_INT_MODULE_CC;
       p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
       p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;
       
       /* specify the timer duration using config data from DBM */
       ret_val = icf_dbm_get_module_glb_data(
                              p_cc_pdb->p_glb_pdb,
                              (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                              (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);
       ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }
    if (ICF_SUCCESS == ret_val)
    {
       ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
       ICF_CC_LOAD_CALL_DURATION_TIMER, p_cc_pdb->p_ecode, ret_val)
    }
    if (ICF_SUCCESS == ret_val)
    {
       /* save the timer id returned by macro */
       p_cc_pdb->p_call_ctx->load_call_dur_timer_id = timer_id;
       p_cc_pdb->p_call_ctx->p_load_timer_data = 
                            (icf_void_t *)(p_timer_data);
    }
    /* If an error occured in starting up the timer */
    else
    {
        icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                      (icf_pool_id_t)ICF_MEM_TIMER,
                      p_timer_data, p_cc_pdb->p_ecode);
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}/* end of icf_cc_start_setup_timer() */    
/*****************************************************************************
 *
 * FUNCTION:        icf_cc_stop_load_call_dur_timer
 *
 * DESCRIPTION:     This function stopscurrently running timer.
 *
 ******************************************************************************/
 icf_return_t icf_cc_stop_load_call_dur_timer(
          INOUT icf_cc_pdb_st *p_cc_pdb)
 {
      icf_return_t                     ret_val = ICF_SUCCESS;

      ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

      ICF_STOP_TIMER(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_call_ctx->load_call_dur_timer_id,
                     p_cc_pdb->p_ecode, ret_val)
      /* free the memory allocated to any existing timer data block */
      if (ICF_NULL != p_cc_pdb->p_call_ctx->p_load_timer_data)
      {
          icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                         (icf_pool_id_t)ICF_MEM_TIMER,
                          p_cc_pdb->p_call_ctx->p_load_timer_data, p_cc_pdb->p_ecode);
          p_cc_pdb->p_call_ctx->p_load_timer_data = ICF_NULL;           
      }

      ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
      return ret_val;
}/* end of icf_cc_stop_current_timer() */

#endif

#ifdef ICF_TRACE_ENABLE

/******************************************************************************
 *
 * FUNCTION:        icf_cc_create_criteria_data
 *
 * DESCRIPTION:     This function is invoked to populate criteria data
 *
 *****************************************************************************/
icf_return_t icf_cc_create_criteria_data(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_config_data_st *p_config_data =ICF_NULL;
    icf_trace_criteria_data_st *p_trace_data = ICF_NULL;
        
	/* Fix for CSR 1-6727350: SPR 19316 */
	if (ICF_NULL != p_cc_pdb->p_call_ctx->p_trace_criteria_data)
		return ret_val;

	ret_val = icf_dbm_get_module_glb_data(
                   p_cc_pdb->p_glb_pdb,
                   (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                   (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
          ICF_PORT_SIZEOF(icf_trace_criteria_data_st),
          ICF_MEM_COMMON,
          p_cc_pdb->p_call_ctx->p_trace_criteria_data,
          ICF_RET_ON_FAILURE,
          p_cc_pdb->p_ecode,
          ret_val)

    p_trace_data = p_cc_pdb->p_call_ctx->p_trace_criteria_data;

    p_trace_data->p_calling_party_address =
		&(p_cc_pdb->p_call_ctx->local_address);
    p_trace_data->bitmask |= ICF_TRACE_CALLING_PARTY_ADDR_PRESENT;

    p_trace_data->p_called_party_address =
                &(p_cc_pdb->p_call_ctx->remote_party_addr);
    p_trace_data->bitmask |= ICF_TRACE_CALLED_PARTY_ADDR_PRESENT;

    p_trace_data->p_local_sip_ip_address = 
				&(p_config_data->self_ip_address);
    p_trace_data->bitmask |= ICF_TRACE_LOCAL_SIP_IP_PRESENT;
    p_trace_data->bitmask |= ICF_TRACE_LOCAL_SIP_PORT_PRESENT;

    p_trace_data->line_id = p_cc_pdb->p_call_ctx->line_id; 
    p_trace_data->bitmask |= ICF_TRACE_LINE_ID_PRESENT;

    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_create_call_connect_data
 *            
 * DESCRIPTION:     This function is invoked to populate call connect data
 *
 *****************************************************************************/
icf_return_t icf_cc_create_call_connect_data(
              INOUT icf_cc_pdb_st            *p_cc_pdb)
{
    icf_return_t	ret_val = ICF_SUCCESS;
	
    icf_call_connect_data_st *p_call_data = 
             (icf_call_connect_data_st *)p_cc_pdb->p_generic_ptr_1;
    
    icf_trace_criteria_data_st *p_criteria_data =
             p_cc_pdb->p_call_ctx->p_trace_criteria_data;

	icf_port_memcpy(&p_call_data->calling_party_address,
			p_criteria_data->p_calling_party_address,
			ICF_PORT_SIZEOF(icf_address_st));

	icf_port_memcpy(&p_call_data->called_party_address,
			p_criteria_data->p_called_party_address,
			ICF_PORT_SIZEOF(icf_address_st));

    p_call_data->line_id = p_criteria_data->line_id;
    
    p_call_data->call_direction = 
				p_criteria_data->call_direction;
	
    p_call_data->call_transport_mode = 
				p_criteria_data->call_transport_mode;

	/* populate stream data */
    p_call_data->p_stream_data = p_criteria_data->p_stream_data;

	icf_port_memcpy(&(p_call_data->local_sip_ip_address),
                    p_criteria_data->p_local_sip_ip_address,
                    ICF_PORT_SIZEOF(icf_transport_address_st));

                
	return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_trace_timer_type
 *
 * DESCRIPTION:     This function gives the traces to the SM module as per the
 *                  timer type.
 *
 *
 ******************************************************************************/

icf_void_t  icf_cc_trace_timer_type(
	INOUT icf_cc_pdb_st *p_cc_pdb,
	IN icf_uint8_t timer_type )
{
     switch(timer_type)
      {
          case  ICF_TIMER_TYPE_INFO:
                    ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_INFO_TIMER_EXPIRE);
                    break;

          case ICF_TIMER_TYPE_INFO_CFM:
                   ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_INFO_TIMER_EXPIRE);
                   break;

	case ICF_TIMER_TYPE_ALERTING:
                   ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_ALERTING_TIMER_EXPIRE);
                   break;

          case ICF_TIMER_TYPE_SETUP:
                   ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_SETUP_TIMER_EXPIRE);
                   break;

          case ICF_TIMER_TYPE_RELEASE:
                   ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_RELEASE_TIMER_EXPIRE);
                   break;

          case ICF_TIMER_TYPE_MEDIA:
                   ICF_CC_APP_TRACE(p_cc_pdb, ICF_TRACE_MEDIA_TIMER_EXPIRE);
                   break;

          case ICF_TIMER_TYPE_RETRY_AFTER:
                   break;

          default:
                   printf("\n\t NO VALID TIMER TYPE FOUND");
                   break;

    }/* end of switch case for timer type */
}/* end of icf_cc_trace_timer_type() */
#endif

/************* Changes for Forking & Multiple Answer Start (Rel7.0) *************/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_forked_resp_list
 *
 * DESCRIPTION:     This is a utility function that frees the forked_resp_list.
 *                  It first checks if the p_data is same as the remote_sdp, 
 *                  if so, does not delete it and deletes the remaining nodes.
 *
 ******************************************************************************/

icf_return_t icf_cc_free_forked_resp_list(
                INOUT	icf_cc_pdb_st	*p_cc_pdb,
                OUT	icf_error_t	*p_ecode)
{
        icf_return_t           ret_val = ICF_SUCCESS;
	icf_list_st            *p_fork = ICF_NULL;
        icf_list_st            *p_next = ICF_NULL;
        icf_internal_list_st   *p_cc_list = ICF_NULL;
        icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;

	ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))

        while(ICF_NULL != p_cc_list)
        {
             p_stream_sdp_info = p_cc_list->p_payload;
	     p_fork = p_stream_sdp_info->p_forked_resp_list;
	
	     /* free the elements in the list */
	     while (ICF_NULL != p_fork)
	     {
		   p_next = p_fork->p_next;
               
                   if(ICF_NULL ==  p_fork->p_data)
                   {
			return ICF_FAILURE;
		   }
		
                   /* check if the payload is same as remote_sdp */
                   if(((icf_forked_resp_data_st *)p_fork->p_data)->p_sdp_info == 
                          p_stream_sdp_info->p_remote_received_sdp)
                   {
                          ((icf_forked_resp_data_st *)p_fork->p_data)->p_sdp_info = ICF_NULL;
                   }
                   else
                   {
                          if(ICF_NULL != ((icf_forked_resp_data_st *)p_fork->p_data)->p_sdp_info)
                          {
                          /* delete the p_sdp_info */
                          ret_val = icf_cmn_delete_internal_sdp_stream(
                                            p_cc_pdb->p_glb_pdb,
                                            &(((icf_forked_resp_data_st *)p_fork->p_data)->p_sdp_info),
                                            p_ecode);

                          icf_dbm_ret_mem_to_pool(
                                        p_cc_pdb->p_glb_pdb,
                                        ICF_MEM_SDP,
                                        ((icf_forked_resp_data_st *)p_fork->p_data)->p_sdp_info,
                                        p_ecode);
                          }
                          
                          if(ICF_FAILURE == ret_val)
                          {
                               ICF_PRINT(((icf_uint8_t*)"\n[CC]: icf_cmn_delete_internal_sdp_stream in icf_cc_free_forked_resp_list returned Failure"));
                               break;
                          }

                     }
             
		       p_fork = p_next;
	     }

             if(ICF_FAILURE == ret_val)
                  break;
             else
             {
                   /* delete the payload */
                   ret_val = icf_cmn_delete_list(
                          p_cc_pdb->p_glb_pdb,&p_stream_sdp_info->p_forked_resp_list,ICF_MEM_COMMON);

                   if(ICF_FAILURE == ret_val)
                   {
                          ICF_PRINT(((icf_uint8_t*)"\n[CC]: icf_cmn_delete_list in icf_cc_free_forked_resp_list returned Failure "));
                          break;
                   }
                   p_cc_list = p_cc_list->p_next;
             }
        }

	ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
	return ret_val;

}/*end of icf_cc_free_forked_resp_list()*/

/************** Changes for Forking & Multiple Answer End (Rel7.0) **************/


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
        IN    icf_duration_t retry_after_timer)
 {
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_config_data_st               *p_config_data = ICF_NULL;
    icf_timer_id_t                   timer_id = ICF_TIMER_ID_INVALID;
    icf_timer_data_st                *p_timer_data = ICF_NULL;
    icf_duration_t                   timer_dur;
    icf_uint16_t                     st_timer_range;
    icf_uint16_t                     end_timer_range;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_cc_pdb->p_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, p_cc_pdb->p_ecode);
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_timer_data))

    if (ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_TIMER_TYPE_RETRY_AFTER;
        p_timer_data->module_id = ICF_INT_MODULE_CC;
        p_timer_data->p_timer_buffer = p_cc_pdb->p_call_ctx;
        p_timer_data->p_glb_pdb = p_cc_pdb->p_glb_pdb;

        /* specify the timer duration using config data from DBM */
        ret_val = icf_dbm_get_module_glb_data(
                     p_cc_pdb->p_glb_pdb,
                     (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                     (icf_void_t *)&p_config_data, p_cc_pdb->p_ecode);

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_config_data))
    }
    if (ICF_SUCCESS == ret_val)
    {

        if(ICF_NULL != retry_after_timer)
        {
                ICF_PRINT(((icf_uint8_t*)"\n[CC]: Retry After Timer duration: %d\n", \
                                                  retry_after_timer));
                timer_dur = retry_after_timer;
       	}

      /* If the above check fails it means there is no retry header 
       * field in 491 response.It will generate the random value of retry 
       * after timer after deciding that the call esatablished is outgoing 
       * or incoming.*/

        else
        { 
            if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR))
            {
            /* outgoing call */
                st_timer_range = ICF_CMN_OG_RACE_TIMER_LOW_LIMIT;
                end_timer_range = ICF_CMN_OG_RACE_TIMER_UP_LIMIT;
            }
            else
            {
            /* incoming call */
                st_timer_range = ICF_CMN_INC_RACE_TIMER_LOW_LIMIT;
                end_timer_range = ICF_CMN_INC_RACE_TIMER_UP_LIMIT;
            }

           /*Get random time value for reinvite race timer*/
            if(ICF_FAILURE == icf_cmn_generate_timer(&timer_dur,
                        st_timer_range, end_timer_range))
            {
                ret_val = ICF_FAILURE;
            } /* ICF_CMN_RACE_TIMER_UP_LIMIT... */

         }
                   
        if(ICF_FAILURE != ret_val)
         {
           ICF_START_TIMER(p_cc_pdb->p_glb_pdb, p_timer_data, timer_id,
                       	timer_dur, p_cc_pdb->p_ecode, ret_val)
        
         }
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* save the timer id returned by macro */
        p_cc_pdb->p_call_ctx->retry_after_timer_id =
            timer_id;
        p_cc_pdb->p_call_ctx->p_retry_after_timer_data =
            (icf_void_t *)(p_timer_data);
        p_cc_pdb->timer_type = ICF_TIMER_TYPE_RETRY_AFTER;
        p_cc_pdb->timer_id = timer_id;
        ICF_CC_TRACE(p_cc_pdb, ICF_TIMER_TYPE_RETRY_AFTER)
    }
    /*
     * If an error occured in starting up the timer
     */ 
    else
    {
       icf_dbm_ret_mem_to_pool(p_cc_pdb->p_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_timer_data, p_cc_pdb->p_ecode); 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
} /* end of function */

/************* Changes for Retry-After header End (Rel7.0) **************/

