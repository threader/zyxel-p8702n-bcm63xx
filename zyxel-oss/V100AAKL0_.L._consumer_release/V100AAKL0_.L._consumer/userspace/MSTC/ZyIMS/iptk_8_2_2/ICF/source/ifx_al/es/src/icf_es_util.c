

/****************************************************************************a

 ** FUNCTION:	

 **			Util funcitons for ES

 **

 *****************************************************************************

 **

 ** FILENAME:		icf_es_util.c

 **

 ** DESCRIPTION:	This is the main file of the HSS UA Application. It 

 **			initializes the toolkit and relevant data structures and 

 **			then spawns the various threads to execute the UA.

 **

 ** DATE		NAME		    REFERENCE	REASON

 ** ----		----		    ---------	------

 ** 20-Feb-2005 Pankaj Negi     ICF LLD  Inital 

 ** 29-AUG-2005 Ram Narayan     NONE        ICF Related changes

 ** 08-Dec-2005 Rohit Aggarwal  ICF 4.0 CDD Added callback for handling platform

 **                                         messages

 ** 08-DEC-2005 Puneet Sharma   ICF 4.0 CDD Added send to application function

 ** 26-DEC-2005 Aman Arora		ICF 4.0		for TCP changes in SI

 ** 09-Jan-2006 Aman Arora		ICF 4.0		for self port server for IPsec

 ** 07-Mar-2006	Anuradha Gupta				Added new traces for Re-invite handling

 ** 30-Mar-2006	Anuradha Gupta	ICF 5.0		Call Modify Feature changes

 ** 27-Nov-2006	Umang Singh     SPR 13736
 ** 04-Jan-2007 Amit Mahajan    ICF 6.2         INFO confirmation changes. 
 ** 17-Jan-2008 Neha Gupta      SPR 18033       Added support for function 
 **						failure simulation for UT.
 **
 ** 08-Mar-2008 Alok Tiwari     Rel8.0          Function icf_es_recv modified
 ** 11-Mar-2008 Vipul Varshney  SPR 18122   AKA Enhancement
 ** 13-Mar-2008 Alok Tiwari     Rel8.0     Memory leak of 2904 bytes on NAT 
 **                                        for  p_msg resolved.
 **                                        Modified function icf_es_recv.  
 ** 27-Mar-2008 Shruti Thakral  SPR 18302   Klocworks warning removal
 ** 4-Apr-2008  Sumant Gupta    ICF 8.0     Rel7.1 Windows porting Merging
 ** 8-Apr-2008  Shruti Thakral  SPR 18350   Klocwork warning removal
 ** 22-Apr-2008 Alok Tiwari     SPR 18455   Klocwork warning removal
 ** 22-Apr-2008 Alok Tiwari     SPR 18456   Klocwork warning removal
 ** 30-Apr-2008 Shruti Thakral  SPR 18302   Klocwork warning removal
 ** 04-Jun-2008 Tarun Gupta     SPR 18585   Console trace printing control as per 
 **                                         ICF_SET_TRACE_REQ API
 ** 30-Jun-2008 Tarun Gupta     SPR 18579   Modified icf_print_msg_hdr
 ** 08-Dec-2008 Rajiv Kumar     SPR 19218   Merged CSR 1-6431768
 ** 30-Jan-2009 Rajiv Kumar     IPTK Rel8.1 Changes done for testing UATK
 **                                         API under ICF_UT_TEST flag
 ** 09-Feb-2009 Tarun Gupta     Rel 8.1 LLD SDP Tunnelling Enhancements
 ** 13-Feb-2009 Anurag Khare    ICF Rel 8.1 Call mod/OPTIONS Enhancements
 ** 03-Mar-2009 Kamal Ashraf    IPTK REL 8.1 Function calls and variable used
 **                                          spacific to Ares has been removed
 **                                          for open source replacemnent
 ** 04-Mar-2009 Saurabh Gupta	SPR 19349      Klocworks fix
 ** 14-Mar-2009 Alok Tiwari     IPTK Rel8.1 Modified function icf_send
 **                                         and icf_es_recv to fix the crash 
 **                                         if p_port_data is NULL.
 ** 17-Mar-2009 Saurabh Gupta	SPR 19349      Klocworks fix
 ** 31-Mar-2009 Anurag Khare    Rel 8.2     changes done for PRACK Enh
 ** 1-Apr-2009  Rajiv kumar     SPR 19305   Fix done for SPR 19305.
 ** 06-Apr-2009 Kamal Ashraf    Rel 8.2     Klocwork Fix
 ** 02-May-2009 Rajiv Kumar     SPR 19531   Klocwork Fix
 ** 26-May-2009 Preksha         Rel 8.2     Asynchronus Message enhancements
 ** 11-Jun-2009 Ashutosh Mohan  SPR 19737     Changes done for GCC 4.1 warnings removal
 ** 13-Jul-2009 Rajiv Kumar     IPTK Rel 8.2  Openssl Removal
 ** 15-Jul-2009 Tarun Gupta     SPR 19854   Modified es_stop_timer,es_start_timer and icf_send
 ** 21-Jul-2009 Rajiv Kumar     IPTK Rel 8.2  Openssl Removal
 ** 21-Jul-2009 Abhishek Dhammawat SPR 19873 Modified icf_es_open_tcp_conn
 ** 07-Aug-2009 Kamal Ashraf    UT Defect    Modified icf_es_close_nw_server
 ** 17-Aug-2009 Anuradha Gupta  SPR 19927    Klocworks warning removal
 ** 20-Aug-2009 Tarun Gupta     SPR 19932    Modified icf_es_close_nw_servers
 ** 25-Aug-2009 Anuradha Gupta  SPR 19969    Modified icf_es_send_to_app
 ** 25-Aug-2009 Abhishek Dhammawat SPR 19953 Modified icf_print_api_string
 ** 09-Sep-2009 Rajiv Kumar     SPR 19999    Merged SPR 19996
 ** 05-Oct-2009 Abhishek Dhammawat SPR 20063 Merged CSR 1-7436195
 ** 12-Oct-2009 Tarun Gupta     SPR 20140    Modified icf_print_api_string
 ** 05-Apr-2010 Alok Tiwari     SPR 20354    Modified icf_es_close_nw_servers 
 **
 *****************************************************************************
 **			Copyrights 2009, Aricent.
 *****************************************************************************/

#ifdef __cplusplus

extern "C" {

#endif 


/* Fix for SPR: 19996*/
/* Uncomment the following flag if the Media Manager API need to be loop back
 * to IPTK instead of ES.This flag is used for internal testing by IPTK team only.
 * This flag is included to cater the delay on IPC channel for Media Manager
 * API while running  Codenomicon, Now all the Media Manager API's will be
 * triggered to Encapsulating shell ie es module of IPTK  instead on MM if
 * this flag is defined */
    
/*#define ICF_MM_STUBED_OUT  1*/
    
#define TCP_RECV_BUFFER    (8192*ICF_MAX_NUM_CALL)     /* Windows supports TCP recv buffer size of 8K */

#ifndef ICF_PORT_FWD
#include "icf_es_prototypes.h"

#include "icf_es_macro.h"
#endif
#include "icf_es_intf_trace.h"
#include "icf_port.h"

#include "icf_port_prototypes.h"

#include "icf_port_struct.h"

#include "icf_port_macros.h"   

#include "icf_port_intf_prototypes.h"   

#include "icf_port_http_utils.h"

#include "icf_clib.h"

#include "icf_common_prototypes.h"

#include "icf_port_pltfm_defs.h"

#include "icf_macro.h"

/*#include "icf_es_intf_trace.h"*/
	
#include "icf_ecode.h"

#ifdef ICF_DNS_LOOKUP_ENABLED    
#include "sdf_dnsclient.h"    
#endif
    
#ifdef ICF_NAT_MEDIA_TRAVERSAL
#include "nat_glb_mgmt_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "nat_common_defs.h"
#endif

#ifdef ICF_PORT_WINDOWS

#include<stdio.h> /* for printf */

#endif

#define DEST_IP "127.0.0.1"

extern icf_glb_pdb_st *p_persistent_glb_pdb ;


#ifdef ICF_NAT_MEDIA_TRAVERSAL
extern nat_glb_pdb_st        *p_nat_glb_pdb;
#endif


#ifdef ICF_LOAD_DEBUG
icf_uint32_t    g_tcp_closed=0;
#endif

#ifdef ICF_MM_STUBED_OUT
icf_return_t icf_mm_stub_process_n_append_to_msg_list(
                            icf_msg_st        *p_msg_data); 
#endif




extern icf_int32_t icf_port_recvfrom(icf_int32_t fd, icf_uint8_t* p_msg);

/*******************************************************************************

 *

 * FUNCTION:        icf_print_msg_hdr

 *

 * DESCRIPTION:     This Function Prints the API ID 

 *

*******************************************************************************/


#ifndef ICF_PORT_FWD
icf_return_t icf_print_api_string(icf_api_id_t api_id)

{
    /* In tunnel mode, we will not print interface traces for DMS request and response */
ICF_CHECK_TUNNEL_MODE_START(p_persistent_glb_pdb,ICF_TRUE) 
    if((ICF_DELETE_MEDIA_SESSION_REQ == api_id) ||
       (ICF_DELETE_MEDIA_SESSION_RESP == api_id))
    {
        return ICF_SUCCESS;
    }
ICF_CHECK_TUNNEL_MODE_END

    switch(api_id)

    {
    case ICF_CFG_CC_SET_SCALE_PARAMS_REQ         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_CC_SET_SCALE_PARAMS_REQ"));break;

    case ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ"));break;

    case  ICF_CALL_TRANSFER_ATTENDED_REQ         :ICF_ES_TRACE(((icf_uint8_t *)" ICF_CALL_TRANSFER_ATTENDED_REQ"));break;

    case  ICF_REMOTE_CALL_TRANSFER_INITIATED_IND   :ICF_ES_TRACE(((icf_uint8_t *)" ICF_REMOTE_CALL_TRANSFER_INITIATED_IND "));break;

    case ICF_CALL_MERGED_IND                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MERGED_IND "));break;

    case  ICF_CONFERENCE_REQ                    :ICF_ES_TRACE(((icf_uint8_t *)" ICF_CONFERENCE_REQ "));break;

    case ICF_CONFERENCE_RESP			: ICF_ES_TRACE(((icf_uint8_t *)" ICF_CONFERENCE_RESP "));break;

    case ICF_SET_SERVICE_PARAMS_REQ             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_SERVICE_PARAMS_REQ "));break;

    case ICF_CREATE_XCONF_REQ        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_XCONF_REQ "));break;
    case ICF_ADD_XCONF_PARTY_REQ     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_ADD_XCONF_PARTY_REQ "));break;
    case ICF_DELETE_XCONF_PARTY_REQ  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DELETE_XCONF_PARTY_REQ "));break;
    case ICF_RELEASE_XCONF_IND       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RELEASE_XCONF_IND "));break;
    case ICF_CREATE_XCONF_RESP       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_XCONF_RESP "));break;
    case ICF_ADD_XCONF_PARTY_RESP    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_ADD_XCONF_PARTY_RESP "));break;
    case ICF_DELETE_XCONF_PARTY_RESP :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DELETE_XCONF_PARTY_RESP "));break;
    case ICF_RELEASE_XCONF_RESP      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RELEASE_XCONF_RESP "));break;
    case ICF_RELEASE_XCONF_REQ      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RELEASE_XCONF_REQ "));break;
                                                 
    case ICF_DISPLAY_IND                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DISPLAY_IND "));break;

    case ICF_CALL_REDIRECTION_IND               :ICF_ES_TRACE(((icf_uint8_t *)" ICF_CALL_REDIRECTION_IND "));break;

    case ICF_SET_MGMT_PLANE_AUTH_PARAM          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_MGMT_PLANE_AUTH_PARAM ")); break;

    case ICF_CREATE_SECURE_CONN_REQ             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_SECURE_CONN_REQ ")); break;

    case ICF_CREATE_SECURE_CONN_RESP            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_SECURE_CONN_RESP ")); break;

    case ICF_CLOSE_SECURE_CONN_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CLOSE_SECURE_CONN_REQ ")); break;

    case ICF_CLOSE_SECURE_CONN_RESP             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CLOSE_SECURE_CONN_RESP ")); break;

    case ICF_SECURE_CONN_STATUS_IND             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SECURE_CONN_REQ ")); break;

    case ICF_XCAP_CREATE_REPLACE_RES            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_XCAP_CREATE_REPLACE_RES ")); break;

    case ICF_XCAP_DELETE_RES                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_XCAP_DELETE_RES")); break;

    case ICF_XCAP_FETCH_RES                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_XCAP_FETCH_RES ")); break;

    case ICF_SYNCML_SEND_REQ                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SYNCML_SEND_REQ")); break;

   case ICF_MGMT_PLANE_API_PROC_FAILURE        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MGMT_PLANE_API_PROC_FAILURE")); break;

    case ICF_MGMT_PLANE_NW_RESP                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MGMT_PLANE_NW_RESP"));break;

    case ICF_RES_UNAVAIL_IND                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RES_UNAVAIL_IND"));break;

    case ICF_SET_STATS_REQ                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_STATS_REQ"));break;

    case ICF_CALL_TRANSFER_UNATTENDED_RESP     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_TRANSFER_UNATTENDED_RESP")); break;

    case ICF_CALL_TRANSFER_ATTENDED_RESP       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_TRANSFER_ATTENDED_RESP")); break;

    case ICF_CALL_FORWARDED_IND                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_FORWARDED_IND")); break;

    case ICF_LOGS_IND                          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_LOGS_IND"));break;

    case ICF_CFG_SET_VIDEO_ATTRIB_REQ          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_VIDEO_ATTRIB_REQ"));break;

    case ICF_MM_SET_NW_STATUS                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MM_SET_NW_STATUS"));break;

    case ICF_SET_LINE_PARAM_REQ                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_LINE_PARAM_REQ")); break;

    case ICF_EARLY_INC_CALL_IND                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_EARLY_INC_CALL_IND  "));break;

    case ICF_EARLY_INC_CALL_CFM                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_EARLY_INC_CALL_CFM  "));break;

    case ICF_API_ID_INVALID                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_API_ID_INVALID"));break;

#ifdef ICF_NW_ACTIVATE

    case ICF_EVENT_INDICATION                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_EVENT_INDICATION"));break;

#endif

    case ICF_APP_ADD_REQ                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_APP_ADD_REQ"));break;

    case ICF_APP_ADD_CFM                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_APP_ADD_CFM"));break;

    case ICF_APP_REMOVE_REQ                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_APP_REMOVE_REQ"));break;

    case ICF_APP_REMOVE_CFM                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_APP_REMOVE_CFM"));break;

#ifdef ICF_NW_ACTIVATE

    case ICF_NETWORK_ACTIVATE_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NETWORK_ACTIVATE_REQ"));break;

    case ICF_NETWORK_ACTIVATE_CFM              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NETWORK_ACTIVATE_CFM"));break;

    case ICF_NETWORK_DEACTIVATE_REQ            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NETWORK_DEACTIVATE_REQ"));break;

    case ICF_NETWORK_DEACTIVATE_CFM            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NETWORK_DEACTIVATE_CFM"));break;

#endif

    case ICF_SET_TRANSPORT_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_TRANSPORT_REQ"));break;

    case ICF_SET_CALL_PARAM_REQ                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_CALL_PARAM_REQ"));break;

    case ICF_SET_SELF_ID_REQ                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_SELF_ID_REQ"));break;

    case ICF_SET_REGISTRATION_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_REGISTRATION_REQ"));break;

    case ICF_CONFIGURE_CFM                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONFIGURE_CFM"));break;

	case ICF_GET_SYSTEM_SETTINGS_REQ           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_SYSTEM_SETTINGS_REQ"));break;  

	case ICF_GET_LINE_SETTINGS_REQ             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_LINE_SETTINGS_REQ"));break;   

	case ICF_GET_SYSTEM_SETTINGS_RESP          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_SYSTEM_SETTINGS_RESP"));break; 

	case ICF_GET_LINE_SETTINGS_RESP            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_LINE_SETTINGS_RESP"));break; 

	case ICF_TRACE_NW_BUFF_IND                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_TRACE_NW_BUFF_IND"));break; 





    case ICF_REGISTER_REQ                      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REGISTER_REQ"));break;

    case ICF_REGISTER_CFM                      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REGISTER_CFM"));break;

    case ICF_REGISTER_STATUS_IND               :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REGISTER_STATUS_IND"));break;

    case ICF_AKA_AUTH_RESP                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_AKA_AUTH_RESP"));break;

    case ICF_CREATE_CALL_REQ                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_CALL_REQ"));break;

    case ICF_LOCAL_USER_ALERTED_REQ            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_LOCAL_USER_ALERTED_REQ"));break;
    
    case ICF_MEDIA_CONNECT_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_CONNECT_REQ"));break;
 
    case ICF_UNEXPECTED_MSG_IND                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_UNEXPECTED_MSG_IND"));break;

    case ICF_TERMINATE_CALL_REQ                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_TERMINATE_CALL_REQ"));break;

    case ICF_LOCAL_USER_TERMINATE_CALL_RESP    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_LOCAL_USER_TERMINATE_CALL_RESP"));break;

    case ICF_CONNECT_REQ                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONNECT_REQ"));break;

    case ICF_SUBSCRIBE_REQ                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SUBSCRIBE_REQ"));break;

    case ICF_PUBLISH_REQ                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_PUBLISH_REQ"));break;

    case ICF_MESSAGE_REQ                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MESSAGE_REQ"));break;

    case ICF_REFER_REQ                         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REFER_REQ"));break;

    case ICF_CALL_HOLD_REQ                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_HOLD_REQ"));break;

    case ICF_CALL_HOLD_RESP                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_HOLD_RESP"));break;

    case ICF_CALL_RESUME_REQ                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_RESUME_REQ"));break;

    case ICF_CALL_RESUME_RESP                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_RESUME_RESP"));break;

    case ICF_INCOMING_CALL_IND                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INCOMING_CALL_IND"));break;

	case ICF_NOTIFY_CFM                         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NOTIFY_CFM"));break;

	case ICF_INFO_REQ                          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INFO_REQ"));break;

	case ICF_INFO_RESP                          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INFO_RESP"));break;

	case ICF_INFO_IND	                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INFO_IND"));break;
	case ICF_OPTIONS_REQ	                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_OPTIONS_REQ"));break;
    case ICF_OPTIONS_RESP	                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_OPTIONS_RESP"));break;
    case ICF_INC_OPTIONS_IND                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INC_OPTIONS_IND"));break;
    case ICF_OPTIONS_CFM                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_OPTIONS_CFM"));break;
    case ICF_INFO_CFM                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INFO_CFM"));break;
    case ICF_RESTART_REQ                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RESTART_REQ"));break;
    case ICF_RESTART_IND                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RESTART_IND"));break;
    case ICF_RESTART_RESP                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RESTART_RESP"));break;
    case ICF_REINITIALISE_REQ                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REINITIALISE_REQ"));break;
    case ICF_REINIT_IND                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REINIT_IND"));break;
    case ICF_REINIT_RESP                           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REINIT_RESP"));break;

ICF_HANDLE_REINVITE_START

	case ICF_CALL_MODIFY_REQ	                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MODIFY_REQ"));break;

	case ICF_CALL_MODIFY_RESP	                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MODIFY_RESP"));break;

	case ICF_INCOMING_CALL_MODIFY_IND           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INCOMING_CALL_MODIFY_IND"));break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_INCOMING_CALL_MODIFY_CANCEL_IND"));break;

	case ICF_CALL_MODIFY_CFM		            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MODIFY_CFM"));break;

ICF_HANDLE_REINVITE_END



    case ICF_REMOTE_USER_ALERTED_IND           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REMOTE_USER_ALERTED_IND"));break;

    case ICF_TERMINATE_CALL_IND                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_TERMINATE_CALL_IND"));break;

    case ICF_REMOTE_USER_TERMINATE_CALL_RESP   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REMOTE_USER_TERMINATE_CALL_RESP"));break;

    case ICF_MEDIA_CONNECTED_IND               :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_CONNECTED_IND"));break;

    case ICF_CONNECT_IND                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONNECT_IND"));break;

    case ICF_PROGRESS_IND                	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_PROGRESS_IND"));break;

    case ICF_PARTY_REPLACED_IND            	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_PARTY_REPLACED_IND "));break;

    case ICF_SUBSCRIPTION_STATUS_IND           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SUBSCRIPTION_STATUS_IND"));break;

    case ICF_NOTIFY_IND                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NOTIFY_IND"));break;

    case ICF_PUBLISH_RESP                      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_PUBLISH_RESP"));break;

    case ICF_MESSAGE_RESP                      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MESSAGE_RESP"));break;

    case ICF_MESSAGE_IND                       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MESSAGE_IND"));break;

    case ICF_REFER_RESP                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REFER_RESP"));break;

	
	case ICF_CONNECT_RESP                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONNECT_RESP"));break;
    
	case ICF_START_FAX_REQ                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_START_FAX_REQ"));break;
	
	case ICF_START_FAX_RESP                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_START_FAX_RESP"));break;

	case ICF_START_FAX_IND                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_START_FAX_IND"));break;


	case ICF_CFG_CODEC_INFO_REQ                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_CODEC_INFO_REQ"));break;

    case ICF_CFG_CODEC_INFO_RESP               :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_CODEC_INFO_RESP"));break;

    case ICF_CFG_RINGER_TONES_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_RINGER_TONES_REQ"));break;

    case ICF_CFG_RINGER_TONES_RESP             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_RINGER_TONES_RESP"));break;

    case ICF_CFG_JBM_MGMT_REQ                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_JBM_MGMT_REQ"));break;

    case ICF_CFG_JBM_MGMT_RESP                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_JBM_MGMT_RESP"));break;

    case ICF_CFG_MAX_EARP_VOL_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_EARP_VOL_REQ"));break;

    case ICF_CFG_MAX_EARP_VOL_RESP             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_EARP_VOL_RESP"));break;

    case ICF_CFG_MAX_SPEAKER_VOL_REQ           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_SPEAKER_VOL_REQ"));break;

    case ICF_CFG_MAX_SPEAKER_VOL_RESP          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_SPEAKER_VOL_RESP"));break;

    case ICF_CFG_SET_PORT_RANGE_REQ                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_PORT_RANGE_REQ"));break;

    case ICF_CFG_SET_PORT_RANGE_RESP               :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_PORT_RANGE_RESP"));break;

    case ICF_START_RINGING_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_START_RINGING_REQ"));break;

    case ICF_START_RINGING_RESP                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_START_RINGING_RESP"));break;

    case ICF_MEDIA_SESSION_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_SESSION_REQ"));break;

    case ICF_MEDIA_SESSION_RESP                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_SESSION_RESP"));break;

    case ICF_RTP_START_IND                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RTP_START_IND"));break;

        

    case ICF_GET_STATS_REQ                     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_STATS_REQ"));break;

    case ICF_GET_STATS_RESP                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_STATS_RESP"));break;

    case ICF_STOP_RINGING_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_RINGING_REQ"));break;

    case ICF_STOP_RINGING_RESP                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_RINGING_RESP"));break;

    case ICF_STOP_RINGING_IND                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_RINGING_IND"));break;

	
	case ICF_MUTE_MEDIA_REQ                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MUTE_MEDIA_REQ"));break;


    case ICF_MUTE_MEDIA_RESP                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MUTE_MEDIA_RESP"));break;

	
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ"));break;

    case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP"));break;
    
	
	case ICF_PLAY_MEDIA_REQ					  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_PLAY_MEDIA_REQ"));break;
  
    case ICF_PLAY_MEDIA_RESP			      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_PLAY_MEDIA_RESP"));break;


    case ICF_SET_EARP_VOLUME_REQ              :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_EARP_VOLUME_REQ"));break;

    case ICF_SET_EARP_VOLUME_RESP             :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_EARP_VOLUME_RESP"));break;

    case ICF_SET_SPEAKER_VOLUME_REQ           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_SPEAKER_VOLUME_REQ"));break;

    case ICF_SET_SPEAKER_VOLUME_RESP          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_SPEAKER_VOLUME_RESP"));break;

    case ICF_SET_MIC_GAIN_REQ				   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_MIC_GAIN_REQ"));break;

   	case ICF_SET_MIC_GAIN_RESP			       :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SET_MIC_GAIN_RESP"));break;

    case ICF_CFG_MAX_MIC_GAIN_REQ			   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_MIC_GAIN_REQ"));break;

	case ICF_CFG_MAX_MIC_GAIN_RESP			   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MAX_MIC_GAIN_RESP"));break;

    case ICF_SEND_APP_DATA_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_APP_DATA_REQ"));break;

    case ICF_SEND_APP_DATA_RESP                :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_APP_DATA_RESP"));break;

    case ICF_RCV_APP_DATA_IND                  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RCV_APP_DATA_IND"));break;

    case ICF_SEND_STATS_REQ                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_STATS_REQ"));break;

    case ICF_SEND_STATS_RESP                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_STATS_RESP"));break;

    case ICF_STOP_STATS_REQ                    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_STATS_REQ"));break;

    case ICF_STOP_STATS_RESP                   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_STATS_RESP"));break;

	case ICF_MSG_FRM_PLTFM					   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MSG_FRM_PLTFM"));break;	

    case ICF_NW_API_MSG_FROM_PEER            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NW_API_MSG_FROM_PEER"));break;

    case ICF_NW_API_OPEN_CONN_IND            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NW_API_OPEN_CONN_IND"));break;

    case ICF_NW_API_CONNECT_SUCCESS          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NW_API_CONNECT_SUCCESS"));break;

    case ICF_NW_API_CONNECT_FAILURE          :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NW_API_CONNECT_FAILURE"));break;

    case ICF_NW_API_CLOSE_CONN_IND            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_NW_API_CLOSE_CONN_IND"));break;

    case ICF_TIMER_MSG                        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_TIMER_MSG"));break;

    case ICF_GET_REGISTER_STATUS_REQ           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_REGISTER_STATUS_REQ"));break;
    
    case ICF_GET_REGISTER_STATUS_RESP           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_REGISTER_STATUS_RESP"));break;

#ifdef ICF_DEBUG_MEMPOOL

	case ICF_MEM_POOL_STATS_PRINT            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEM_POOL_STATS_PRINT"));break;

#endif        

    case ICF_VIBRATION_REQ					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_VIBRATION_REQ"));break;

    case ICF_VIBRATION_RESP					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_VIBRATION_RESP"));break;

    case ICF_ECHO_CANC_REQ					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_ECHO_CANC_REQ"));break;

    case ICF_ECHO_CANC_RESP					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_ECHO_CANC_RESP"));break;

    case ICF_VAD_SS_CNG_REQ					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_VAD_SS_CNG_REQ"));break;

    case ICF_VAD_SS_CNG_RESP				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_VAD_SS_CNG_RESP"));break;
 
    case ICF_STOP_MEDIA_PLAY_REQ			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_MEDIA_PLAY_REQ"));break;

    case ICF_STOP_MEDIA_PLAY_RESP			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_MEDIA_PLAY_RESP"));break;

    case ICF_STOP_MEDIA_PLAY_IND			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_MEDIA_PLAY_IND"));break;

    case ICF_STOP_VIBRATION_REQ				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_VIBRATION_REQ"));break;

    case ICF_STOP_VIBRATION_RESP			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_VIBRATION_RESP"));break;

    case ICF_CFG_MEDIA_REQ					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MEDIA_REQ"));break;

    case ICF_CFG_MEDIA_RESP					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_MEDIA_RESP"));break;

    case ICF_CFG_SET_DIGIT_TRANS_REQ		:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_DIGIT_TRANS_REQ"));break;

    case ICF_CFG_SET_DIGIT_TRANS_RESP		:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_DIGIT_TRANS_RESP"));break;

    case ICF_CFG_SET_SELF_IP_REQ			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_SELF_IP_REQ"));break;

    case ICF_CFG_SET_SELF_IP_RESP			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_SELF_IP_RESP"));break;


    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ"));break;

    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP"));break;
	
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ"));break;

    case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP	:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP"));break;

	
	case ICF_DGTS_IND						:ICF_ES_TRACE(((icf_uint8_t *)"ICF_DGTS_IND"));break;  

    case ICF_CONNECT_MEDIA_DEVICE_REQ		:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONNECT_MEDIA_DEVICE_REQ"));break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP		:ICF_ES_TRACE(((icf_uint8_t *)"ICF_CONNECT_MEDIA_DEVICE_RESP"));break;

    case ICF_MEDIA_RECORD_REQ				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_RECORD_REQ"));break;

	case ICF_MEDIA_RECORD_RESP				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_RECORD_RESP"));break;

	case ICF_PAUSE_MEDIA_REQ				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_PAUSE_MEDIA_REQ"));break;

	case ICF_PAUSE_MEDIA_RESP				:ICF_ES_TRACE(((icf_uint8_t *)"ICF_PAUSE_MEDIA_RESP"));break;


    case ICF_SEND_DGTS_REQ					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_DGTS_REQ"));break;

    case ICF_SEND_DGTS_RESP					:ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_DGTS_RESP"));break;

    case ICF_STOP_MEDIA_RECORD_REQ			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_MEDIA_RECORD_REQ"));break;

	case ICF_STOP_MEDIA_RECORD_RESP			:ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_MEDIA_RECORD_RESP"));break;


    case ICF_CREATE_MEDIA_SESSION_REQ    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_MEDIA_SESSION_REQ"));break;     

    case ICF_CREATE_MEDIA_SESSION_RESP   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CREATE_MEDIA_SESSION_RESP"));break;  

    case ICF_MODIFY_MEDIA_SESSION_REQ    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MODIFY_MEDIA_SESSION_REQ"));break;     

    case ICF_MODIFY_MEDIA_SESSION_RESP   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MODIFY_MEDIA_SESSION_RESP"));break;  

    case ICF_DELETE_MEDIA_SESSION_REQ    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DELETE_MEDIA_SESSION_REQ"));break;   

    case ICF_DELETE_MEDIA_SESSION_RESP  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DELETE_MEDIA_SESSION_RESP"));break;  

    case ICF_DELETE_MEDIA_SESSION_IND    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_DELETE_MEDIA_SESSION_IND"));break;     

    case ICF_MERGE_MEDIA_SESSION_REQ    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MERGE_MEDIA_SESSION_REQ"));break;    

    case ICF_MERGE_MEDIA_SESSION_RESP    :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MERGE_MEDIA_SESSION_RESP"));break;    

    case ICF_GET_CODEC_LIST_REQ         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_CODEC_LIST_REQ"));break;   

    case ICF_GET_CODEC_LIST_RESP        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_GET_CODEC_LIST_RESP"));break;    

    case ICF_CODEC_COMMIT_REQ            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CODEC_COMMIT_REQ"));break;     

    case ICF_CODEC_COMMIT_RESP           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CODEC_COMMIT_RESP"));break;  

    case ICF_REL_RES_REQ                 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_REL_RES_REQ"));break;    
	
	case ICF_MEDIA_CAPABILITIES_REQ      :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_CAPABILITIES_REQ"));break;     

    case ICF_MEDIA_CAPABILITIES_RESP     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MEDIA_CAPABILITIES_RESP"));break;  

	case ICF_OPEN_MEDIA_CHANNEL_REQ			 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_OPEN_CHANNEL_REQ"));break;     

    case ICF_OPEN_MEDIA_CHANNEL_RESP     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_OPEN_MEDIA_CHANNEL_RESP"));break;  

	case ICF_CLOSE_MEDIA_CHANNEL_REQ     :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CLOSE_MEDIA_CHANNEL_REQ"));break;     


    case ICF_SEND_UDP_DATA_REQ			 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_SEND_UDP_DATA_REQ"));break;  

	case ICF_RECV_UDP_DATA_IND			 :ICF_ES_TRACE(((icf_uint8_t *)"ICF_RECV_UDP_DATA_IND"));break;

    case ICF_MM_UNEXPECTED_MSG_IND         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MM_UNEXPECTED_MSG_IND"));break;  
	    
	case ICF_PORT_PLTFM_SA_CREATE_REQ         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_CREATE_REQ"));break;

    case ICF_PORT_PLTFM_SA_CREATE_RESP        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_CREATE_RESP"));break;

    case ICF_PORT_PLTFM_SA_MODIFY_REQ         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_MODIFY_REQ"));break;

    case ICF_PORT_PLTFM_SA_MODIFY_RESP        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_MODIFY_RESP"));break;

    case ICF_PORT_PLTFM_SA_DELETE_REQ         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_DELETE_REQ"));break;

    case ICF_PORT_PLTFM_SA_DELETE_RESP        :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_DELETE_RESP"));break;

    case ICF_PORT_PLTFM_SA_EXPIRY_IND         :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_EXPIRY_IND"));break;

    case ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_REQ   :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_FETCH_ENC_ALGO"));break;

    case ICF_PORT_PLTFM_SA_FETCH_AUTH_ALGO_RESP :ICF_ES_TRACE(((icf_uint8_t *)"ICF_WRPR_SA_FETCH_AUTH_ALGO"));break;

	case ICF_MM_INIT_COMPLETE :ICF_ES_TRACE(((icf_uint8_t *)"ICF_MM_INIT_COMPLETE"));break;

#ifdef ICF_ERROR_ENABLE
    case ICF_REPORT_ERROR_IND:
                             ICF_ES_TRACE(((icf_uint8_t *)"ICF_REPORT_ERROR_IND"));  break;
#endif
#ifdef ICF_TRACE_ENABLE
    case ICF_REPORT_TRACE_IND:
                             ICF_ES_TRACE(((icf_uint8_t *)"ICF_REPORT_TRACE_IND"));  break;
#endif
#ifdef ICF_STAT_ENABLE
    case ICF_STATS_REQ:
                             ICF_ES_TRACE(((icf_uint8_t *)"ICF_STATS_REQ"));  break;
    case ICF_STATS_RESP:
                             ICF_ES_TRACE(((icf_uint8_t *)"ICF_STATS_RESP"));  break;
#endif

	case ICF_STOP_FAX_REQ            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_FAX_REQ"));break;
	case ICF_STOP_FAX_RESP           :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_FAX_RESP"));break;
	case ICF_STOP_FAX_IND            :ICF_ES_TRACE(((icf_uint8_t *)"ICF_STOP_FAX_IND"));break;
	case ICF_CALL_MODIFY_CANCEL_REQ  :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MODIFY_CANCEL_REQ"));break;
	case ICF_CALL_MODIFY_CANCEL_RESP :ICF_ES_TRACE(((icf_uint8_t *)"ICF_CALL_MODIFY_CANCEL_RESP"));break;
#ifdef ICF_UT_TEST
	case ICF_UT_SIMULATE :ICF_ES_TRACE(((icf_uint8_t *)"ICF_UT_SIMULATE"));break; 
	case ICF_UATK_API_TEST :ICF_ES_TRACE(((icf_uint8_t *)"ICF_UATK_API_TEST"));break; 
                          
#endif
#ifdef ICF_DNS_LOOKUP_ENABLED
    case ICF_DNS_MAPPING_CHANGE_IND:
		ICF_ES_TRACE(((icf_uint8_t *)"ICF_DNS_MAPPING_CHANGE_IND"));
		break;
#endif /* ICF_DNS_LOOKUP_ENABLED */
        
    /* Traces for new tunnel mode specific APIs */    
    case ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ"));
         break;     

    case ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP"));
         break;  

    case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ"));break;     

    case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP"));
         break;                          

    case ICF_TUNNELLED_OPTIONS_RESP :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_OPTIONS_RESP"));
         break;

    case ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP"));
         break;

    case ICF_EARLY_SESSION_IND :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_EARLY_SESSION_IND"));
         break;

    case ICF_EARLY_SESSION_CFM :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_EARLY_SESSION_CFM"));
         break;
#ifdef ICF_SESSION_TIMER
    case ICF_SESSION_TIMER_TOGGLE_REQ :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_SESSION_TIMER_TOGGLE_REQ"));
         break;    

    case ICF_SESSION_TIMER_TOGGLE_RESP :
         ICF_ES_TRACE(((icf_uint8_t *)"ICF_SESSION_TIMER_TOGGLE_RESP"));
         break;   
#endif    
    case ICF_GENERIC_MSG_REQ:
	 ICF_ES_TRACE(((icf_uint8_t *)" ICF_GENERIC_MSG_REQ "));
	 break;    
    case ICF_GENERIC_MSG_RESP:
	 ICF_ES_TRACE(((icf_uint8_t *)" ICF_GENERIC_MSG_RESP"));
	 break;    
    case ICF_GENERIC_MSG_IND:
	 ICF_ES_TRACE(((icf_uint8_t *)" ICF_GENERIC_MSG_IND "));
	 break;    
    case ICF_GENERIC_MSG_CFM:
	 ICF_ES_TRACE(((icf_uint8_t *)" ICF_GENERIC_MSG_CFM "));
	 break;
    case ICF_PRACK_REQ_RECV_IND:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_PRACK_REQ_RECV_IND "));
         break;
    case ICF_PRACK_RESP_RECV_IND:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_PRACK_RESP_RECV_IND "));
         break;
    case ICF_PRACK_RESP_SENT_IND:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_PRACK_RESP_SENT_IND "));
         break;
    case ICF_PRACK_REQ_SENT_IND:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_PRACK_REQ_SENT_IND "));
         break;
    case ICF_MESSAGE_CFM:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_MESSAGE_CFM "));
         break;
         /*SPR 19953: Added the case for NAT init API to avoid
          printing UNKNOWN_API_ID when IPTK comes up for NAT 
          enabled binary*/
#ifdef ICF_NAT_MEDIA_TRAVERSAL         
    case ICF_NAT_STUN_INIT_RESP:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_NAT_STUN_INIT_RESP "));
         break;
#endif   
    /* SPR 20140 : Added the case for ICF_SET_TRACE_REQ to avoid
          printing UNKNOWN_API_ID 
    */      
    case ICF_SET_TRACE_REQ:
         ICF_ES_TRACE(((icf_uint8_t *)" ICF_SET_TRACE_REQ "));
         break;
    /* End SPR 20140 */     

    default:ICF_ES_TRACE(((icf_uint8_t *)"*****UNKNOWN_API_ID****"));break ;    

    }

    return ICF_SUCCESS;

}


/*******************************************************************************

 *

 * FUNCTION:        icf_print_msg_hdr

 *

 * DESCRIPTION:     This Function Prints the Message Header

 *

*******************************************************************************/

icf_return_t icf_print_msg_hdr(icf_msg_st *p_msg_data)

{

    /* In tunnel mode, we will not print interface traces
       for DMS request and response.
    */
ICF_CHECK_TUNNEL_MODE_START(p_persistent_glb_pdb,ICF_TRUE) 
    if((ICF_DELETE_MEDIA_SESSION_REQ == p_msg_data->hdr.api_id) ||
       (ICF_DELETE_MEDIA_SESSION_RESP == p_msg_data->hdr.api_id))
    {
        return ICF_SUCCESS;
    }
ICF_CHECK_TUNNEL_MODE_END
    

    ICF_ES_TRACE(((icf_uint8_t *)"\n\n***********ICF_PRINT_HDR_DATA_START**************\n"));

    ICF_ES_TRACE(((icf_uint8_t *)"API_ID = %d  :",p_msg_data->hdr.api_id));

    icf_print_api_string(p_msg_data->hdr.api_id);

    ICF_ES_TRACE(((icf_uint8_t *)"\n"));



    ICF_ES_TRACE(((icf_uint8_t *)"SOURCE_ID =%d  :",p_msg_data->hdr.source_id));

    ICF_ES_TRACE(((icf_uint8_t *)"  DEST_ID =%d  :",p_msg_data->hdr.destination_id));

    ICF_ES_TRACE(((icf_uint8_t *)"  APP_ID =%d  :",p_msg_data->hdr.app_id));



    ICF_ES_TRACE(((icf_uint8_t *)"CALL_ID= %d \n",p_msg_data->hdr.call_id));

    ICF_ES_TRACE(((icf_uint8_t *)"API_LENGTH= %d \n",p_msg_data->hdr.api_length));

    ICF_ES_TRACE(((icf_uint8_t *)"***********ICF_PRINT_HDR_DATA_END***********\n\n"));

#ifdef ICF_TRACE_ENABLE
#ifdef ICF_INTERFACE_TRACE_ENABLE 
#ifndef ICF_NAT_RPORT_SUPPORT
    if(ICF_TRUE == p_persistent_glb_pdb->trace_info.status)
    {
	    icf_print_msg(p_msg_data);
    }
#endif
#endif
#endif
	
    return ICF_SUCCESS;
}


#endif


/*******************************************************************************

 *

 * FUNCTION:        icf_send

 *

 * DESCRIPTION:     This function take message from NW and give them to 

 *                  corresponding AL's so that they can convert data to 

 *                  Format of external entity

 *

*******************************************************************************/

icf_return_t icf_send(IN icf_void_t *p_port_data,

                            INOUT icf_msg_st *p_msg_data,

                            OUT icf_error_t *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;



	icf_uint8_t* p_encoded_buf = ICF_NULL;

    icf_uint32_t encoded_buf_length = ICF_NULL;

    icf_error_t  ecode = ICF_ERROR_NONE;

    icf_global_port_info_st *p_glb_port_data = ICF_NULL;
    /* Rel 8.2 Klocworks fix */
    icf_interface_info_st  interface_data = {0,0,0,0,"\0",0,0};
    icf_string_st channel_path;  //ZyXEL porting
    /* Rel 8.2 Klocworks fix end */
    if(ICF_NULL == p_port_data)
    {
      ICF_PRINT(((icf_uint8_t*)"\n[Function:icf_send]:p_port_data is NULL"))
      return ICF_FAILURE;
    }

    p_glb_port_data = (icf_global_port_info_st *)p_port_data;
    interface_data.p_port_info = p_glb_port_data;

#ifndef ICF_PORT_FWD
    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_send"));

    
    if(p_msg_data->hdr.destination_id != ICF_MODULE_NAT)
    {
       icf_print_msg_hdr(p_msg_data);
    }
#endif
    

#ifdef ICF_INT_TRACE

    ICF_INTERFACE_TRACE(p_msg_data)

#endif



    /* Check the destination id and send message to corresponding module */

    switch(p_msg_data->hdr.destination_id)

    {

        

        /*   Message from NW can be sent on TCP or UDP */

        case ICF_MODULE_SIP_NW:

            {

                icf_nw_interface_info_st	*p_nw_intf = 

                    (icf_nw_interface_info_st*)(p_msg_data->payload);    



                if (p_nw_intf->bitmap & ICF_INTF_INFO_PROTOCOL_UDP)

                {

					if (ICF_TRUE == 

                        p_glb_port_data->win_ipc_info.ipsec_enabled)

					{

						interface_data.fd_id = p_glb_port_data->win_ipc_info.

							curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER];

					}

					else

					{
						if ( p_nw_intf->curr_server_index < (ICF_WIN_TCP_SIP_SERVER + 1))
						{

						interface_data.fd_id = p_glb_port_data->

						win_ipc_info.self_fd_nw[p_nw_intf->curr_server_index].fd_id;
						}
					}

                    interface_data.interface_type = ICF_INTF_TYPE_UDP; 

                    interface_data.msg_len = p_msg_data->

					hdr.api_length - sizeof(icf_nw_interface_info_st);



                    interface_data.dest_port = p_nw_intf->remote_port_num;

                    strcpy((icf_int8_t *)interface_data.dest_ip, (const icf_int8_t*)p_nw_intf->remote_ip_addr);
                      
                    /* Fix For SPR 19305*/
                    /* Check whether destination IP address is valid IP
                     * address or not.If the function icf_port_check_if_ip
                     * return failure it means that the IP address in
                     * interface_data.dest_ip is not a valid IP address and
                     * SIP message cannot be send over network using this IP
                     * address.A common scenario is when  incoming INVITE received
                     * with via header having some invalid in received
                     * parameter ie  ;received=[FFF023401:0:0:0:0:0:0:2] . In this
                     * scenario while trying to send any failure response we
                     * are trying on [FF01:0:0:0:0:0:0:2] adress which is
                     * invalid */
                    if (ICF_FAILURE == icf_port_check_if_ip((icf_uint8_t *)interface_data.dest_ip))
                    {
                        ICF_ES_TRACE(((icf_uint8_t *)"[ES]: Unable to send SIP message.Destinatio IP is not valid\n"));
                        ret_val = ICF_FAILURE;
                        break;
                    }
                }

                else if (p_nw_intf->bitmap & ICF_INTF_INFO_PROTOCOL_TCP)

                {



                    interface_data.fd_id = p_nw_intf->receiver_fd;

							/*TCP conn ID*/

                    interface_data.interface_type = ICF_INTF_TYPE_TCP; 



                    interface_data.msg_len = ((icf_nw_interface_info_st *)

                            p_msg_data->payload)->pdu_len;


                }

				 else if (p_nw_intf->bitmap & ICF_INTF_INFO_PROTOCOL_TLS)

                {
                    interface_data.fd_id = p_nw_intf->receiver_fd;

							/*TCP conn ID*/

                    interface_data.interface_type = ICF_INTF_TYPE_TLS; 

                    interface_data.msg_len = ((icf_nw_interface_info_st *)
							p_msg_data->payload)->pdu_len;
                }


                ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to SIP AL"));
                /* Added for SIP DSCP parameter */
                interface_data.sip_dscp = p_nw_intf->sip_dscp;
                ret_val = icf_sip_nw_process_outgoing_msg(

                        &interface_data, (icf_void_t*)p_msg_data->payload,

                         p_ecode);
                if( ICF_FAILURE == ret_val )
                 {
                   ICF_ES_TRACE(((icf_uint8_t *)"[ES]: Unable to send SIP message\n"));
                 }
                else
                 {
                   ICF_ES_TRACE(((icf_uint8_t *)"[ES]: SIP message sent successfully\n"));
                 }   
            }

            break;



#ifndef ICF_SSA_ES_UT
        case ICF_MODULE_MM:
            if (ICF_FAILURE == 
                icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, &ecode, ICF_NULL)) 
            {
				printf("ICF clib:Message encoding failure for PA\n");
            }
			else
			{
                /* In tunnel mode, we will not send a DMS request to the MM/application.
				   From here, we will just create a dummy DMS response and append it to
				   MRM's message list and break from the loop.
				*/   
ICF_CHECK_TUNNEL_MODE_START(p_persistent_glb_pdb,ICF_TRUE)
                if(ICF_DELETE_MEDIA_SESSION_REQ == p_msg_data->hdr.api_id)
				{
                    /* Freeing memory for the encoded DMS request */
                    icf_free_encoded_msg(p_encoded_buf, &ecode);

                    /* Now appending a dummy response to GLB PDB's global
                       message list. Note that the memory acquired for this
                       node will be freed in the MRM module (while polling
                       in the while loop) after it has been processed.
                    */   
					ret_val = icf_cmn_append_msg_to_list(p_msg_data);
					if(ICF_SUCCESS == ret_val)
					{
						/* Dummy DMS response has been successfully posted to MRM's msg list */
						break;
					}
					else /* failure in msg posting */
					{
						/* Raise alarm and break from the loop */
						printf("\nICF CLIB:Message posting to MRM's list failed for API ID \
								ICF_DELETE_MEDIA_SESSION_REQ\n");
						break;
					}
				}
ICF_CHECK_TUNNEL_MODE_END

#ifdef ICF_MM_STUBED_OUT
                /* Fix for SPR: 19996*/
                /* If ICF_MM_STUBED_OUT is defined append a dummy MM response
                 * depending upon MM API in the internal message list;*/
                icf_mm_stub_process_n_append_to_msg_list(p_msg_data);
                /*Free the encoded messgae before breaking from the loop.*/
                icf_free_encoded_msg(p_encoded_buf, &ecode);
                break;    
#endif 
				
/*ZyXEL porting*/
#if 0
				if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,
					
					p_encoded_buf, encoded_buf_length,
					
					p_glb_port_data->win_ipc_info.port_mm,
					
					p_glb_port_data->win_ipc_info.mm_addr, /* destination ip address */
					
					p_ecode )==ICF_SUCCESS)
					
#else
				memset(&channel_path, 0, sizeof(channel_path));
				strncpy((char *)(&channel_path.str[0]), ICF_MM_CHANNEL_PATH, sizeof(ICF_MM_CHANNEL_PATH));
				channel_path.str_len = sizeof(ICF_MM_CHANNEL_PATH);
				if(icf_port_ipc_send(p_glb_port_data->win_ipc_info.self_fd_ipc,
					p_encoded_buf,
					encoded_buf_length,
					p_ecode,
					channel_path
					)==ICF_SUCCESS)
#endif
/*ZyXEL porting end*/						
				{
#ifndef    ICF_LOAD_STAT
					ICF_PRINT(((icf_uint8_t*)"Send to MM successfully \n"));
#endif
				}
				
				else
					
				{
					printf("Unable to send to the MM \n");
					
				}
				
				icf_free_encoded_msg(p_encoded_buf, &ecode);
				
				
				
				ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to RTP AL"));
				
				break;
				
			}

#ifdef ICF_NAT_MEDIA_TRAVERSAL
        case ICF_MODULE_NAT:

            {
                nat_msg_st *p_nat_msg;
                nat_uint32_t api_length = p_msg_data->hdr.api_length -
                    ICF_PORT_SIZEOF(icf_api_header_st) +
                    ICF_PORT_SIZEOF(nat_msg_hdr_st);


                p_nat_msg = (nat_msg_st *)icf_port_static_memget
                    (api_length,p_ecode);

                if(ICF_NAT_TIMER_EXP_IND == p_msg_data->hdr.api_id)
                {
                p_nat_msg->msg_hdr.api_id = NAT_TIMER_EXP_IND;
                }
                else
                {
                p_nat_msg->msg_hdr.api_id = (icf_uint8_t)p_msg_data->hdr.api_id;
                }
                p_nat_msg->msg_hdr.app_context_id = p_msg_data->hdr.app_id;
                p_nat_msg->msg_hdr.source_id = NAT_MODULE_APP;
                p_nat_msg->msg_hdr.destination_id = NAT_MODULE_NAT;
                p_nat_msg->msg_hdr.api_length = p_msg_data->hdr.api_length - 
                    ICF_PORT_SIZEOF(icf_api_header_st) +
                    ICF_PORT_SIZEOF(nat_msg_hdr_st);
         
                icf_port_memcpy(p_nat_msg->p_msg_data , p_msg_data->payload,
                    p_msg_data->hdr.api_length - ICF_PORT_SIZEOF(icf_api_header_st));
                if(ICF_SUCCESS == icf_port_udp_send(p_glb_port_data->win_ipc_info.\
                        nat_fd_ipc,p_nat_msg, p_nat_msg->msg_hdr.api_length,				
                        p_glb_port_data->win_ipc_info.port_nat,\
                        p_glb_port_data->win_ipc_info.nat_addr,p_ecode))
					
				{
#ifndef ICF_LOAD_STAT					
					printf("Send to NAT Client successfully \n");
#endif
					
				}
				
				else					
				{					
					printf("Unable to send to the NAT Client \n");					
				}
				
				icf_port_static_memfree(p_nat_msg,p_ecode);
				ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to NAT Client AL"));
            
            break;
            }

       
#endif

		case ICF_MODULE_PLATFORM:


#ifdef ICF_PLTFM_TEST_ENABLE
	if (ICF_FAILURE == 
	           icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, &ecode, ICF_NULL)) 

         {

		printf("ICF clib:Message encoding failure for PA\n");

         }

	else
	{
#endif

#ifdef ICF_PLTFM_TEST_ENABLE

		if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,

		p_encoded_buf, encoded_buf_length,
	
                p_glb_port_data->win_ipc_info.port_mm,

                p_glb_port_data->win_ipc_info.mm_addr, /* destination ip address */

                p_ecode )==ICF_SUCCESS)

            {

                printf("Send to network successfully \n");

            }
#else
/*ZyXEL porting*/
#if 0
            if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,

                (icf_msg_st*)p_msg_data, ((icf_msg_st*)p_msg_data)->hdr.api_length,

                p_glb_port_data->win_ipc_info.port_mm,

                p_glb_port_data->win_ipc_info.mm_addr, /* destination ip address */

                p_ecode )==ICF_SUCCESS)
#else
		memset(&channel_path, 0, sizeof(channel_path));
		strncpy((char *)(&channel_path.str[0]), ICF_MM_CHANNEL_PATH, sizeof(ICF_MM_CHANNEL_PATH));
		channel_path.str_len = sizeof(ICF_MM_CHANNEL_PATH);
		if(icf_port_ipc_send(p_glb_port_data->win_ipc_info.self_fd_ipc,
			(icf_msg_st*)p_msg_data,
			((icf_msg_st*)p_msg_data)->hdr.api_length,
			p_ecode,
			channel_path
			)==ICF_SUCCESS)
#endif
/*ZyXEL porting end*/

            {
#ifndef    ICF_LOAD_STAT
                ICF_PRINT(((icf_uint8_t*)"Send to MM successfully \n"));
#endif

            }
#endif
            else

            {

                printf("Unable to send to the MM \n");

            }
            
            if(ICF_NULL != p_encoded_buf)
            {
                icf_free_encoded_msg(p_encoded_buf, &ecode);
            }

#ifdef ICF_PLTFM_TEST_ENABLE
	}
#endif	

            ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to RTP AL"));

            break;



        case ICF_MODULE_PA:



            if (ICF_FAILURE == 

                icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, &ecode, ICF_NULL)) 

            {
               /*Klocwork warning resolved*/
               ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,p_encoded_buf)

				printf("ICF clib:Message encoding failure for PA\n");

            }

            else

			{
/*ZyXEL porting*/
#if 0
				if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,

					p_encoded_buf, encoded_buf_length,

					p_glb_port_data->win_ipc_info.port_pa,

					(icf_uint8_t*)DEST_IP, /* destination ip address */

					p_ecode )==ICF_SUCCESS)
#else
				memset(&channel_path, 0, sizeof(channel_path));
				strncpy((char *)(&channel_path.str[0]), VOICEAPP_IPTK_CHANNEL_PATH, sizeof(VOICEAPP_IPTK_CHANNEL_PATH));
				channel_path.str_len = sizeof(VOICEAPP_IPTK_CHANNEL_PATH);
				if(icf_port_ipc_send(p_glb_port_data->win_ipc_info.self_fd_ipc,
					p_encoded_buf,
					encoded_buf_length,
					p_ecode,
					channel_path
					)==ICF_SUCCESS)
#endif
/*ZyXEL porting end*/
				{
#ifndef    ICF_LOAD_STAT
					ICF_PRINT(((icf_uint8_t*)"Send to PA successfully \n"));
#endif
				}

				else

				{

					printf("Unable to send to the PA \n");

				}

				icf_free_encoded_msg(p_encoded_buf, &ecode);

			}



            ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to PA AL"));



            break;



#if 0

            /* Navneet -> There is no static configuration module in ICF */

        case ICF_MODULE_CONF:

#ifndef ICF_PORT_WINDOWS

            interface_data.fd_id = p_glb_port_data->

                   ext_info.write_module_fd[ICF_MODULE_CONF];            

            interface_data.interface_type = ICF_INTF_TYPE_FIFO; 

            interface_data.msg_len = p_msg_data->hdr.api_length;



            /* CONF AL will be invoked later */

            ret_val = icf_os_send(

                    &interface_data, (icf_void_t*)p_msg_data, p_ecode);

			ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to Config AL"));



#else

            if (ICF_FAILURE == 

                icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, &ecode, ICF_NULL)) 

            {

				printf("ICF clib:Message encoding failure for CONF\n");

            }

            else

			{

				if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,

					p_encoded_buf, encoded_buf_length,

					p_glb_port_data->win_ipc_info.port_cfg,

					DEST_IP, /* destination ip address */

					p_ecode )==ICF_SUCCESS)

				{

                    printf("Send to CONF successfully \n");

				}

				else

				{

					printf("Unable to send to the CONF \n");

				}

				icf_free_encoded_msg(p_encoded_buf, &ecode);

			}

#endif

			ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to Config AL"));

            break;

#endif



#if 0 /* there is no external SM in ICF */

        case ICF_MODULE_SM:

			/* SM is nothing but the PA so send it to PA */

#ifndef ICF_PORT_WINDOWS

            interface_data.fd_id = p_glb_port_data->

                   ext_info.write_module_fd[ICF_MODULE_SM];  



            interface_data.interface_type = ICF_INTF_TYPE_FIFO; 



            interface_data.msg_len = p_msg_data->hdr.api_length;



            ret_val = icf_os_send(

                  &interface_data,(icf_void_t*)p_msg_data,p_ecode);          

#else

			interface_data.msg_len = p_msg_data->

										hdr.api_length;



			if(icf_port_udp_send(p_glb_port_data->win_ipc_info.self_fd_ipc,

                  p_msg_data, interface_data.msg_len,

                  p_glb_port_data->win_ipc_info.port_pa,

                  DEST_IP, /* destination ip address */

                  p_ecode )==ICF_SUCCESS)

				  {

					printf("Sennd to SM successfully \n");

					}

				else

				{

					printf("Unable to send to the SM \n");

				}

#endif

            ICF_ES_TRACE(((icf_uint8_t *)"\nSending message to SM"));

            break;

#endif
#endif

    }  



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_send with ret_val %d",

                (icf_int32_t)ret_val));

    return ret_val;

}



/*******************************************************************************

 *

 * FUNCTION:        icf_al_send

 *

 * DESCRIPTION:     This function writes the AL output message to ICF MSG 

 *                  LIST

 * 

*******************************************************************************/

icf_return_t icf_al_send(IN icf_interface_info_st *p_interface_info,

                 icf_msg_st *p_msg,

                 icf_error_t *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    /* Globad port data */ 

    icf_global_port_info_st *p_glb_port_data = 

        (icf_global_port_info_st *)p_interface_info->p_port_info;

    p_ecode = p_ecode;

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_al_send"));



    if(ICF_NULL == p_glb_port_data->internal_msg_list.p_msg)

    {

        p_glb_port_data->internal_msg_list.p_msg = p_msg;

    }

    else

    {

        icf_port_memfree(p_glb_port_data,p_msg,ICF_MEM_COMMON,

              p_ecode);

        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:INTERNAL MSG LIST NOT NULL, can't add msg\n"));

    }

    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_al_send with ret_val %d",

                (icf_int32_t)ret_val));

    return ret_val;

}



/*******************************************************************************

 *

 * FUNCTION:        icf_os_send

 *

 * DESCRIPTION:     This function take message from AL and invoke corresponding

 *                  message of port to write msg on NW interface

 * 

*******************************************************************************/

icf_return_t icf_os_send(IN icf_interface_info_st *p_interface_info,

                 icf_void_t *p_msg,

                 icf_error_t *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;



    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_os_send"));

    

	if(ICF_INTF_TYPE_UDP == p_interface_info->interface_type)

    {

        ICF_ES_TRACE(((icf_uint8_t *)"\nSending message on UDP fd_id %d",

                (icf_int32_t)p_interface_info->fd_id));
        /* Need to handle sip_dscp present in p_interface_info
        in icf_port_udp_send and icf_port_tcp_send function */

/* ZyXEL porting */
#if 1 
printf("\r\n icf_es_util.c call icf_port_udp_send dscp is %x\n",p_interface_info->sip_dscp);
	//eric.chung: set SIP TOS/DSCP 	
		icf_port_udp_SetDSCP( p_interface_info->fd_id,p_interface_info->sip_dscp);
#endif
/* ZyXEL porting end*/
        ret_val = icf_port_udp_send(p_interface_info->fd_id,

                  p_msg, p_interface_info->msg_len,

                  p_interface_info->dest_port,

                  (icf_uint8_t*)p_interface_info->dest_ip,

                  p_ecode );

    }
#ifdef ICF_SECURE_SUPPORT
    else if(ICF_INTF_TYPE_TLS == p_interface_info->interface_type)

    {

        icf_global_port_info_st *p_glb_port_data = 

                (icf_global_port_info_st *)p_interface_info->p_port_info;

        icf_void_t      *p_secure_data = ICF_NULL;



        ICF_ES_TRACE(((icf_uint8_t *)"\nSending message on TLS fd_id %d",

                (icf_int32_t)p_interface_info->fd_id));

        /* 

         * Now extract the SSL Data corrresponding to this

         * socket fd from the map 

         */

        p_secure_data = icf_es_fetch_secure_data_from_map(

            p_glb_port_data,

            p_interface_info->fd_id,

            p_ecode) ;

        if (ICF_NULL == p_secure_data)

        {

            ret_val = ICF_FAILURE;

        }

        else if (ICF_FAILURE == icf_port_secure_send(p_secure_data,

                p_msg, p_interface_info->msg_len, p_ecode))

        {

            ret_val = ICF_FAILURE;

        }

    }

#endif
    else

    {

        ICF_ES_TRACE(((icf_uint8_t *)"\nSending message on TCP fd_id %d",

                (icf_int32_t)p_interface_info->fd_id));



        ret_val = icf_port_tcp_send(p_interface_info->fd_id,

                p_msg, p_interface_info->msg_len, p_ecode); 

#ifdef ICF_LOAD_STAT
        if(ICF_FAILURE == ret_val)
        {
            icf_global_port_info_st *p_port_info= p_interface_info->p_port_info;

            printf("\n--TCP send failed----- active FD count: %d\n",
                    p_port_info->win_ipc_info.act_fd_count);
        }
#endif
        

    }

 

    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_os_send with ret_val %d",

                (icf_int32_t)ret_val));

    return ret_val;

}





/*******************************************************************************

 *

 * FUNCTION:        icf_es_recv

 *

 * DESCRIPTION:    This function will read message from socket, give the message

 *                 to corresponding AL and return message to MRM

 *                 of port to write msg on NW interface

 * 

*******************************************************************************/

icf_return_t icf_es_recv(IN icf_void_t *p_port_data,

       icf_uint16_t select_timer_duration,

       icf_msg_list_st *p_icf_msg_list,

        icf_error_t *p_ecode)

{
    icf_return_t   ret_val = ICF_SUCCESS;

	/* SPR 19927: The variable name changed as there are variables
	 * with the same name used in the function that are declared
     * as local scope within brackets  {}. The name is changed
     * to avoid any confusion.
     */	 
    icf_msg_st*  p_recv_msg = ICF_NULL;
    icf_msg_st*  p_msg;  //ZyXEL porting
    icf_global_port_info_st *p_glb_port_data = ICF_NULL;

    icf_int32_t  encoded_buf_length = -1;

#if ICF_PORT_WINDOWS 
    icf_int32_t  pdu_length = 0;
#endif
    icf_error_t  ecode;

    /* Variable to store fd set */

    fd_set      read_fd_set,write_fd_set;

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)
    fd_set      except_fd_set;
#endif

    icf_interface_info_st   interface_info;

    /* Local variable to store data read from network data */

    icf_uint8_t     *p_interface_msg = ICF_NULL;

    /* Globad port data */ 

    icf_uint8_t     i = 0;
    icf_boolean_t   fd_found = ICF_FALSE;
    
#ifdef ICF_DNS_LOOKUP_ENABLED
    icf_uint32_t  dnsclib_fd_id = 0;
#endif
    if(ICF_NULL == p_port_data)
    {
       ICF_PRINT(((icf_uint8_t*)"\n[Function:icf_es_recv]:p_port_data is NULL"))
       return ICF_FAILURE;
    }
    p_glb_port_data = (icf_global_port_info_st *)p_port_data;

    /* IF port data has any data in messge list return it to mrm */

#ifdef ICF_LOAD_DEBUG
    if(p_glb_port_data->win_ipc_info.num_of_open_servers > 3)
    {
        printf("break");
    }
#endif

    if(ICF_NULL != p_glb_port_data->internal_msg_list.p_msg)
    {

         ICF_ES_TRACE(((icf_uint8_t *)"\nES : Reading data from internal queue"));

         /*Assign the pointer in message list and return */

         p_icf_msg_list->p_msg = p_glb_port_data->internal_msg_list.p_msg;

         p_glb_port_data->internal_msg_list.p_msg = ICF_NULL;

         return ICF_SUCCESS;

    }

    
    /* This is used by AL while calling memory related functions*/

    interface_info.p_port_info = (icf_void_t*)p_port_data;

    /* Fill interface type as FIFO by default */    

    interface_info.interface_type = ICF_INTF_TYPE_FIFO;

    /* Initialize the read and write FDSET */

    ICF_PORT_FD_ZERO(&read_fd_set);

    ICF_PORT_FD_ZERO(&write_fd_set);

    /* Set the read fd set from ext info data */

    read_fd_set = p_glb_port_data->win_ipc_info.win_read_fd;

    write_fd_set = p_glb_port_data->win_ipc_info.win_write_fd;

	ICF_PORT_FD_ZERO(&except_fd_set);

	/* this list is added to handle connect failure incase of

	 * non-blcking connect 

	 */

	except_fd_set = p_glb_port_data->win_ipc_info.win_write_fd;

    /* Call select function to check if data has been received on any fd */

    ret_val = icf_port_select(&read_fd_set, &write_fd_set, &except_fd_set,
            select_timer_duration, p_ecode);

    /* Select returns due to timeout or on reciving message on fd */

    /* If select has retruned due to timeout - How to TELL MRM */

    if(ICF_SUCCESS == ret_val)
    {
         
         /* allocate memory only when msg is recd */
         p_interface_msg = icf_port_memget(p_glb_port_data,
                           ICF_BLK_SIZE_MAX+1, ICF_MEM_COMMON, p_ecode);

         if(ICF_NULL == p_interface_msg)
         {
             return ICF_FAILURE ;
         }
        /* Select has inidicated that some data has received - Now check the 
         * data against each fd set to identify which al should be invoked */

#ifdef ICF_DNS_LOOKUP_ENABLED
        
         /*Get the clib fd.Dns Client has written the response on clib fd.*/
         dnsclib_fd_id = p_glb_port_data->pDnsclient_fds->dns_clib_fd;
         
         /*Check whether any response is present*/
         if(ICF_PORT_FD_ISSET(dnsclib_fd_id, &read_fd_set))
         {
             /*If present, handle the same*/
             if (ICF_FAILURE == 
                     sdf_fn_uaRecvDnsQueryResp())
             {
                 ICF_ES_TRACE(((icf_uint8_t *)"\nES : Failure return while"
                             "handling query response from DNS CLIENT"));
             }
         }
#endif             

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

        if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.read_module_fd
                    [ICF_MODULE_MM], &read_fd_set))
        {
             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_MM];

             /* Copy the write fd id in interface info in case we send 
                Al send message */ 
             interface_info.fd_id = 
                p_glb_port_data->ext_info.write_module_fd[ICF_MODULE_MM];

             /* Read data from fifo and pass it to mmi al */

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,p_interface_msg,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_MM],

                        p_ecode))

             {

                 interface_info.msg_len = 
                 p_glb_port_data->ext_info.msg_write_size[ICF_MODULE_MM];

#ifdef ICF_IFX_INT_TRACE 

                 ICF_IFX_INTERFACE_TRACE(8, ICF_MODULE_ICF, ((x_IFX_RTP_SsMsg*)p_interface_msg)->ucMsgType , p_interface_msg, interface_info.msg_len) 

#endif

                 if(ICF_FAILURE == icf_mmial_mm_process_incoming_msg(

                          &interface_info,

                          p_interface_msg,

                          p_icf_msg_list,

                          p_ecode))

                 {

                    ICF_ES_TRACE(((icf_uint8_t *)"\nES : MM AL returned failure"));

                     

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nProblem in reading data on MM pipe"));

                 ret_val = ICF_FAILURE;

             }

        }

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_MODULE_RM], &read_fd_set))

        {

             /* Message has been received on RM message pipe */

             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_RM];

             /* Copy the write fd in interface info in case Al 

                 send dummy response */ 

             interface_info.fd_id = 

                p_glb_port_data->ext_info.write_module_fd[ICF_MODULE_RM];

             

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,p_interface_msg,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_RM],

                        p_ecode))

             {

                 interface_info.msg_len = p_glb_port_data->

                 ext_info.msg_write_size[ICF_MODULE_MM];



#ifdef ICF_IFX_INT_TRACE 

ICF_IFX_INTERFACE_TRACE(ICF_MODULE_RM, ICF_MODULE_ICF, ((x_IFX_RM_RxMsg*)p_interface_msg)->ucMsgType , p_interface_msg, interface_info.msg_len) 

#endif


                 if(ICF_FAILURE == icf_mmial_rm_process_incoming_msg(

                                   &interface_info,

                                   p_interface_msg,

                                   p_icf_msg_list,

                                   p_ecode))

                 {

                     ICF_ES_TRACE(((icf_uint8_t *)"\nES : RM AL returned failure"));

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nES: Problem in reading data on RM pipe"));

                 ret_val = ICF_FAILURE;

             }

        }

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_MODULE_CONF], &read_fd_set))

        {

             /* Message has been received on RM message pipe */

             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_CONF];

             printf("\n<FD-CONF> %d", fd_id);

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,&api_header,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_CONF],

                        p_ecode))

             {

                 if(ICF_FAILURE == icf_cfgal_process_incoming_msg(

                          &interface_info,   

                          (icf_uint8_t*)&api_header,

                          p_icf_msg_list,

                          p_ecode))

                 {

                     ICF_ES_TRACE(((icf_uint8_t *)"\nES: CFG AL returned failure"));

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on Conf pipe"));

                 ret_val = ICF_FAILURE;

             }

        }

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_MODULE_SM], &read_fd_set))
        {

             /* Message has been received on SM message pipe */

             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_SM];

             printf("\n<FD-SM> %d", fd_id);

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,&api_header,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_SM],

                    p_ecode))

             {

                 if(ICF_FAILURE == icf_smal_process_incoming_msg(

                          &interface_info,   

                          (icf_uint8_t*)&api_header,

                          p_icf_msg_list,

                          p_ecode))

                 {

                     ICF_ES_TRACE(((icf_uint8_t *)"\nES: SM AL returned failure"));

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on SM pipe"));

                 ret_val = ICF_FAILURE;

             }

        }



        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_MODULE_PA], &read_fd_set))

        {

             /* Message has been received on RM message pipe */

             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_PA];

             printf("\n<FD-PA> %d", fd_id);

             /* Get length for message using peek and getting payload 

              * length from message heaader */ 

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,&api_header,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_PA],

                        p_ecode))

             {

                 if(ICF_FAILURE == icf_paal_process_incoming_msg(

                            &interface_info,

                          (icf_uint8_t*) &api_header,

                          p_icf_msg_list,

                          p_ecode))

                 {

                     ICF_ES_TRACE(((icf_uint8_t *)"\nES: PA AL returned failure"));

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on PA pipe"));

                 ret_val = ICF_FAILURE;

             }

        } 

       /* TODO SM will come later */ 

#if 0

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_MODULE_SM], &read_fd_set))

        {

             /* Message has been received on RM message pipe */

             fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_SM];

             /* Get length for message using peek and getting payload 

              * length from message heaader */ 

             if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,&interface_msg,

                    p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_SM],

                        p_ecode))

             {

                         

                 if(ICF_FAILURE == icf_smal_process_incoming_msg(

                          &interface_info,

                          &interface_msg,

                          p_icf_msg_list,

                          p_ecode))

                 {

                     ICF_ES_TRACE(((icf_uint8_t *)"\nES: SM AL returned failure"));

                 }

              }

             else

             {

                 ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on SM pipe"));

                 ret_val = ICF_FAILURE;

             }

        }   

#endif

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_UDP_SIP_SERVER], &read_fd_set))

        {

            printf("\n<FD-NW> %d", fd_id);



            /* Message has been received on UDP socket */

            interface_info.fd_id = p_glb_port_data->ext_info.

                read_module_fd[ICF_UDP_SIP_SERVER];

            interface_info.interface_type = ICF_INTF_TYPE_UDP;



            /* port to fill dest port and dest ip */

            if(ICF_SUCCESS == icf_port_udp_recv(&interface_info,

                        p_interface_msg, p_ecode))

            {

                if(ICF_FAILURE == icf_sipal_process_incoming_msg(

                            &interface_info,

                            p_interface_msg,

                            p_icf_msg_list,

                            p_ecode))

                {

                    ICF_ES_TRACE(((icf_uint8_t *)"\nES: Sip AL returned failure"));

                }

            }

            else

            {
                ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on UDP"));

                ret_val = ICF_FAILURE;

            }

        }

        else if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                    read_module_fd[ICF_TCP_SIP_SERVER], &read_fd_set))

        {

            /* Message has been received on TCP server

             * this means an incoming connection */

            /*SPR 19873 Init the p_msg*/
            icf_msg_st               *p_msg = ICF_NULL;

            icf_nw_interface_info_st *p_nw_msg = ICF_NULL;



            ICF_ES_TRACE(((icf_uint8_t *)"\nIncomingTCPConnection"));



            if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == 

                    p_glb_port_data->ext_info.num_of_pending_conn + 

                    p_glb_port_data->ext_info.num_of_conn)

            {

                ret_val = ICF_FAILURE;

                ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));

            }

            if ( ICF_SUCCESS == ret_val)

            {

                p_msg = (icf_msg_st*)icf_port_memget(p_port_data,

                        ICF_PORT_SIZEOF(icf_api_header_st) +

                        ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                        ICF_MEM_COMMON,p_ecode);



                p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

                if ( ICF_SUCCESS == icf_port_accept(

                            p_glb_port_data->ext_info.

                            read_module_fd[ICF_TCP_SIP_SERVER],

                            &(p_nw_msg->receiver_fd),

                            p_nw_msg->remote_ip_addr,

                            &(p_nw_msg->remote_port_num)))

                {

                    p_msg->hdr.api_id = ICF_NW_API_OPEN_CONN_IND;

                    p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 

                    p_msg->hdr.destination_id = ICF_MODULE_ICF; 

                    p_msg->hdr.api_length  = 

                        ICF_PORT_SIZEOF(icf_api_header_st) +\

                        ICF_PORT_SIZEOF(icf_nw_interface_info_st);

                    p_msg->hdr.call_id  = ICF_INVALID_CALL_ID;

					icf_es_add_fd_to_es_tcp_db(p_glb_port_data->ext_info.read_tcp_conn, 
                            (icf_uint16_t)p_nw_msg->receiver_fd,ICF_FALSE);


                    p_glb_port_data->ext_info.num_of_conn++;

                    FD_SET(p_nw_msg->receiver_fd, 

                            &(p_glb_port_data->ext_info.read_fdset));



                    p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TCP;

                    p_nw_msg->pdu_len = 0;

                    p_nw_msg->p_sip_msg = ICF_NULL;



                    p_icf_msg_list->p_msg = p_msg;

                    p_icf_msg_list->p_msg_next = ICF_NULL;

                }

                else

                {
                    ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));

                    icf_port_memfree(p_port_data,p_msg,

                            ICF_MEM_COMMON,p_ecode);

                    ret_val = ICF_FAILURE;

                }
				ICF_KLOCWORK_FALSE_POSITIVE (p_persistent_glb_pdb, p_nw_msg->receiver_fd)

            }

        }
#ifdef ICF_SECURE_SUPPORT
        else if(ICF_PORT_FD_ISSET(

			p_glb_port_data->win_ipc_info.self_fd_nw[ICF_WIN_TLS_SIP_SERVER],

			&read_fd_set))

        {

            /* Message has been received on TLS server

             * this means an incoming connection */
            /*SPR 19873 Init the p_msg*/
            icf_msg_st               *p_msg = ICF_NULL;

            icf_nw_interface_info_st *p_nw_msg = ICF_NULL;



            ICF_ES_TRACE(((icf_uint8_t *)"\nIncomingTCPConnection"));



            if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == 

                    p_glb_port_data->ext_info.num_of_pending_conn + 

                    p_glb_port_data->ext_info.num_of_conn)

            {

                ret_val = ICF_FAILURE;

                ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));

            }

            if ( ICF_SUCCESS == ret_val)

            {

                p_msg = (icf_msg_st*)icf_port_memget(p_port_data,

                        ICF_PORT_SIZEOF(icf_api_header_st) +

                        ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                        ICF_MEM_COMMON,p_ecode);
p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

                if ( ICF_SUCCESS == icf_port_accept(

                            p_glb_port_data->win_ipc_info.\

							  self_fd_nw[ICF_WIN_TLS_SIP_SERVER],

                            &(p_nw_msg->receiver_fd),

                            p_nw_msg->remote_ip_addr,

                            &(p_nw_msg->remote_port_num)))

                {

                    icf_void_t      *p_secure_ctx = ICF_NULL;

                    /* 

                     * Now extract the SSL ctx corrresponding to this

                     * socket fd from the map 

                     */

                    p_secure_ctx = icf_es_fetch_secure_data_from_map(

                            p_glb_port_data,

                            p_glb_port_data->win_ipc_info.self_fd_nw[ICF_WIN_TLS_SIP_SERVER],

                            p_ecode);

                    if (ICF_NULL == p_secure_ctx)

                    {

                        ret_val = ICF_FAILURE;

                    }

                    else

                    {

                        icf_void_t     *p_secure_data = ICF_NULL ;

                        /* Invoke porting function to issue SSL accept */

                        p_secure_data = icf_port_secure_accept(

                                        p_secure_ctx,p_nw_msg->receiver_fd,p_ecode);

                        if (ICF_NULL == p_secure_data)

                        {ret_val = ICF_FAILURE;

                        }

                        else

                        {

                            /*

                             * Add a maaping between the SSL data and the socket fd in 

                             * the mapping maintained in the ES

                             */

                            if (ICF_FAILURE == icf_es_add_secure_data_to_map(

                                        p_secure_data,p_glb_port_data,

                                        p_nw_msg->receiver_fd,

                                        p_ecode))

                            {

                               icf_port_free_secure_data(p_secure_data);

                               ret_val = ICF_FAILURE;

                            }

                         }

                      } 

                      if (ICF_SUCCESS == ret_val)

                      {

                            p_msg->hdr.api_id = ICF_NW_API_OPEN_CONN_IND;

                            p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 

                            p_msg->hdr.destination_id = ICF_MODULE_ICF; 

                            p_msg->hdr.call_id = ICF_INVALID_CALL_ID;



                            p_msg->hdr.api_length  = 

                                ICF_PORT_SIZEOF(icf_api_header_st) +\

                                ICF_PORT_SIZEOF(icf_nw_interface_info_st);
                                icf_es_add_fd_to_es_tcp_db(p_glb_port_data->ext_info.read_tcp_conn, 

                                    (icf_uint16_t)p_nw_msg->receiver_fd,ICF_TRUE);

                            p_glb_port_data->ext_info.num_of_conn++;

                            FD_SET(p_nw_msg->receiver_fd, 

                                    &(p_glb_port_data->win_ipc_info.win_read_fd));

					        /* each time u write to win_read_fd, add the fd to the

					         * act_read_fd also. This will be used for clearing the

					         * the FD set. Since FD_CLR is not working */
                            /*SPR 19873 The below code leg is not required 
                              because act_fd_count is never decremented,
                              during SOAK it shall exceed the FD_SETSIZE array
                              limit of act_read_fd and result in memory
                              corruption*/ 
#if 0
					        p_glb_port_data->win_ipc_info.act_read_fd[

						        p_glb_port_data->win_ipc_info.act_fd_count] = 

						        p_nw_msg->receiver_fd;

					        p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif                            


                            p_nw_msg->bitmap |= ICF_INTF_INFO_PROTOCOL_TLS;

                            p_nw_msg->pdu_len = 0;

                            p_nw_msg->p_sip_msg = ICF_NULL;



                            p_icf_msg_list->p_msg = p_msg;

                            p_icf_msg_list->p_msg_next = ICF_NULL;

                        }

                        else

                        {

                            ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));

                            icf_port_memfree(p_port_data,p_msg,

                                    ICF_MEM_COMMON,p_ecode);

                            ret_val = ICF_FAILURE;

                        }
		}
		else
        {
        }
				ICF_KLOCWORK_FALSE_POSITIVE (p_persistent_glb_pdb, p_nw_msg->receiver_fd)        
            }
            

        }
#endif
    else

        {
            /*SPR 19873 init the p_msg */
            icf_msg_st               *p_msg = ICF_NULL;

            icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

            icf_uint32_t              count = 0;

            /* Now we loop over write fd set to check if message has

             * been received on TCP write FDs

             * this means an outgoing connection has reached some status

             * Now we will check it against the pending conn FD list.

             * At the first match we call getpeername.If it returns

             * success, this means connect was successful else we send

             * failure to SSA */



            p_msg = (icf_msg_st*)icf_port_memget(p_port_data,

                    ICF_PORT_SIZEOF(icf_api_header_st) +

                    ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                    ICF_MEM_COMMON,p_ecode);



            p_msg->hdr.api_id = ICF_API_ID_UNDEFINED;



            p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

            for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count++)

            {

                if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                            pending_connect_fd[count], &write_fd_set))

                {

                    if ( ICF_TRUE == icf_port_isconnected

                            (p_glb_port_data->ext_info.pending_connect_fd[count]))

                    {

                        icf_es_add_fd_to_es_tcp_db(
                                p_glb_port_data->ext_info.read_tcp_conn, 
                                p_glb_port_data->ext_info.pending_connect_fd[count],
                                ICF_FALSE);

                        p_glb_port_data->ext_info.num_of_conn++;

                        FD_SET(p_glb_port_data->ext_info.pending_connect_fd[count],

                                &(p_glb_port_data->ext_info.read_fdset));

                        p_msg->hdr.api_id = ICF_NW_API_CONNECT_SUCCESS;

                        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:ConnectSuccessInd"));

                    }

                    else

                    {

                        p_msg->hdr.api_id = ICF_NW_API_CONNECT_FAILURE;

                        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:ConnectFailureInd"));

                    }

                    p_nw_msg->receiver_fd = 

                        p_glb_port_data->ext_info.pending_connect_fd[count];

                    icf_es_rem_fd_from_es_db(

                            p_glb_port_data->ext_info.pending_connect_fd, 

                           p_glb_port_data->ext_info.pending_connect_fd[count]);

                    p_glb_port_data->ext_info.num_of_pending_conn--;

                    FD_CLR(p_nw_msg->receiver_fd,

                            &(p_glb_port_data->ext_info.write_fdset));



                    p_nw_msg->pdu_len = 0;

                    p_nw_msg->p_sip_msg = ICF_NULL;

                    break;

                }

            }

            if ( ICF_FAILURE != ret_val &&

                    count == ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS)

            {/*No status update on outgoing connects, now we check for data on

               existing connections*/

                for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count++)

                {

                    if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                                 read_tcp_conn[count].fd_id, &read_fd_set))
                    {

                           interface_info.fd_id = p_glb_port_data->ext_info.

                            read_tcp_conn[count].fd_id;

#ifdef ICF_SECURE_SUPPORT			

                        if (ICF_TRUE == p_glb_port_data->ext_info.

                                read_tcp_conn[count].is_secure_conn)

                        {
                           icf_void_t      *p_secure_data = ICF_NULL;

                            interface_info.interface_type = ICF_INTF_TYPE_TLS;

                            /* 

                             * Now extract the SSL Data corrresponding to this

                             * socket fd from the map 

                             */

                            p_secure_data == icf_es_fetch_secure_data_from_map(

                                    p_glb_port_data,

                                    p_glb_port_data->ext_info.read_tcp_conn[count].fd_id,

                                    p_ecode) ;

                            if (ICF_NULL == p_secure_data)

                            {
						        ret_val = ICF_FAILURE;
                            }

                            else if (ICF_FAILURE == icf_port_secure_peek(

                                p_secure_data,&interface_info, (icf_void_t*)(p_glb_port_data->

                                ext_info.p_tcp_recv_buffer), p_ecode))

                            {
                                ret_val = ICF_FAILURE;
                            }
						}	

                        else
                        { 
#endif 			
						interface_info.interface_type = ICF_INTF_TYPE_TCP;

						 ret_val == icf_port_tcp_peek(

                                &interface_info, (icf_void_t*)(p_glb_port_data->

                                ext_info.p_tcp_recv_buffer), p_ecode)

#ifdef ICF_SECURE_SUPPORT			

                        }

#endif 			

						if (ICF_SUCCESS == ret_val)
                        {
                                p_nw_msg->receiver_fd = 

                                    p_glb_port_data->ext_info.read_tcp_conn[count].fd_id;

                                p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

                                p_msg->hdr.destination_id = ICF_MODULE_ICF;

                                p_msg->hdr.source_id = ICF_MODULE_SIP_NW;



                                if ( 0 != interface_info.msg_len)

                                {

                                    p_msg->hdr.api_id = ICF_NW_API_MSG_FROM_PEER;

                                    p_nw_msg->pdu_len = interface_info.msg_len;

                                    p_nw_msg->p_sip_msg =  

                                        p_glb_port_data->ext_info.p_tcp_recv_buffer;

                                }

                        if ( ICF_FAILURE != icf_port_tcp_peek

                         (&interface_info, (icf_void_t*)(p_glb_port_data->

                          ext_info.p_tcp_recv_buffer), p_ecode))

                        {

                            p_nw_msg->receiver_fd = 

                                p_glb_port_data->ext_info.read_tcp_conn[count];

                            p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

                            p_msg->hdr.destination_id = ICF_MODULE_ICF;

                            p_msg->hdr.source_id = ICF_MODULE_SIP_NW;



                            if ( 0 != interface_info.msg_len)

                            {

                                p_msg->hdr.api_id = ICF_NW_API_MSG_FROM_PEER;

                                p_nw_msg->pdu_len = interface_info.msg_len;

                                p_nw_msg->p_sip_msg =  

                                    p_glb_port_data->ext_info.p_tcp_recv_buffer;

                            }

                           else

                                {

                                    p_msg->hdr.api_id = ICF_NW_API_CLOSE_CONN_IND;

                                    icf_es_rem_fd_from_es_tcp_db(

                                    p_glb_port_data->ext_info.read_tcp_conn,p_glb_port_data,

                                            interface_info.fd_id);

                                    FD_CLR(interface_info.fd_id,

                                   &(p_glb_port_data->ext_info.read_fdset));

                                    p_glb_port_data->ext_info.num_of_conn--;



                                    p_nw_msg->pdu_len = 0;

                                    p_nw_msg->p_sip_msg = ICF_NULL;

                                }
                         }

                        break;

                    }

                }/*Loop over all the readable TCP connections*/

            }

            if ( ICF_FAILURE != ret_val &&

                    ICF_API_ID_UNDEFINED != p_msg->hdr.api_id)

            {

                p_msg->hdr.source_id = ICF_MODULE_SIP_NW;

                p_msg->hdr.destination_id = ICF_MODULE_ICF;

                p_msg->hdr.api_length  = 

                    ICF_PORT_SIZEOF(icf_api_header_st) +\

                    ICF_PORT_SIZEOF(icf_nw_interface_info_st);

                p_msg->hdr.call_id  = ICF_INVALID_CALL_ID;

				if (ICF_INTF_TYPE_TCP == interface_info.interface_type)
					
				{
					p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
				}
				
				else if (ICF_INTF_TYPE_TLS == interface_info.interface_type)			
				{
					p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TLS;				
				}

                p_icf_msg_list->p_msg = p_msg;

                p_icf_msg_list->p_msg_next = ICF_NULL;

            }

            else

            {

         /*       ICF_ES_TRACE(((icf_uint8_t *)"\nES:NoStatusUpdate on PendingConn or NoDataOnExistingConn"));*/

                icf_port_memfree(p_port_data,p_msg,

                        ICF_MEM_COMMON,p_ecode);

            }

        }




        /* Now its posssible that Some TCP set is there in read set,

         * so we need to access them */



#else /* end of PORT_ICF */        

         /* In case of TLS call IPTK open three ports for receiving request from
          * peer one each for UDP,TCP and TLS.Loop should be continue for
          * ICF_WIN_TLS_SIP_SERVER number of times.*/		
/*ZyXEL porting*/
#if 1 
		 if(ICF_PORT_FD_ISSET(p_glb_port_data->win_ipc_info.self_fd_ipc, &read_fd_set))

		{

                /* the data is received from the IPC */

			/* call win ported function to recive the message */

               if ((encoded_buf_length = icf_port_recvfrom(p_glb_port_data->win_ipc_info.self_fd_ipc, 
                    p_interface_msg)) == -1) 
               {
#ifdef ICF_PORT_WINDOWS
                    int ecode = WSAGetLastError();
					ecode=ecode;
				    
#endif
                    ICF_ES_TRACE(((icf_uint8_t *)"\nProblem in reading data from Application interface"));
                    ret_val = ICF_FAILURE;
                }
                else
                {
                	
                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:MSG received successfuly from App: %d bytes",

						encoded_buf_length));                    
#ifndef ICF_SSA_ES_UT                   

                    if (ICF_FAILURE == 

                        icf_decode_msg(&p_msg, p_interface_msg, encoded_buf_length, &ecode)) 

                    {

                        printf("ICF clib:Message decoding failure for ICF\n");

                    }

                    else

                    {

                        p_icf_msg_list->p_msg = (icf_msg_st *)p_msg;

                        p_icf_msg_list->p_msg_next = ICF_NULL;
                        if(p_msg->hdr.source_id != ICF_MODULE_NAT)
                        { 
                            icf_print_msg_hdr(p_icf_msg_list->p_msg);
                        }  				



#ifdef ICF_DEBUG_MEMPOOL

                        if(ICF_MEM_POOL_STATS_PRINT == p_icf_msg_list->p_msg->hdr.api_id)

                        {

                            icf_port_print_mem_pool_stats();

                            icf_free_decoded_msg(p_msg, &ecode);

                            p_icf_msg_list->p_msg = ICF_NULL;

                            /* No Need to process in MRM process Message */

                            icf_port_memfree(p_port_data,p_interface_msg,ICF_MEM_COMMON,p_ecode);

                            return ICF_SUCCESS;

                        }

#endif   
                    }
#endif
                }
                 fd_found = ICF_TRUE;
		}
#endif
         if ((ICF_FAILURE == ret_val) || (ICF_TRUE == fd_found))
         {
              /*Do nothing*/
         }
	 else
	 {
/*ZyXEL porting end*/
         for (i = 0; (i < p_glb_port_data->win_ipc_info.num_of_open_servers)
                 && (i<(ICF_WIN_TLS_SIP_SERVER + 1));
              i++)
         {
             if (ICF_PORT_FD_ISSET(
                 p_glb_port_data->win_ipc_info.self_fd_nw[i].fd_id, 
                 &read_fd_set))
             {
                 if (ICF_WIN_UDP_SIP_SERVER == 
                     p_glb_port_data->win_ipc_info.self_fd_nw[i].server_type)
                 {
                     /* Message has been received on UDP socket */
                     interface_info.fd_id =  p_glb_port_data->win_ipc_info.
                     self_fd_nw[i].fd_id;

                     interface_info.interface_type = ICF_INTF_TYPE_UDP;

                     /* port to fill dest port and dest ip */
                     if(ICF_SUCCESS == icf_port_udp_recv(&interface_info,
					p_interface_msg, p_ecode))
                     {
                          if(ICF_FAILURE == icf_sipal_process_incoming_msg(
						&interface_info,
						p_interface_msg,
						p_icf_msg_list,
						p_ecode))
                          {
				ICF_ES_TRACE(((icf_uint8_t *)"\nES: Sip AL returned failure"));
                          }
                     }
                     else
                     {
			ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on UDP"));
			ret_val = ICF_FAILURE;
                     }
                 }
                 else if (ICF_WIN_TCP_SIP_SERVER == 
                     p_glb_port_data->win_ipc_info.self_fd_nw[i].server_type)
                 {
                      /* Message has been received on TCP server
                       * this means an incoming connection */
                      /*SPR 19873 init the p_msg*/
                      icf_msg_st               *p_msg = ICF_NULL;

                      icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

                      ICF_ES_TRACE(((icf_uint8_t *)"\nIncomingTCPConnection"));

                      if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == 
                          p_glb_port_data->ext_info.num_of_pending_conn + 
                          p_glb_port_data->ext_info.num_of_conn)
                      {
                          ret_val = ICF_FAILURE;
                          ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));
                      }
                     if ( ICF_SUCCESS == ret_val)
                     {
                         p_msg = (icf_msg_st*)icf_port_memget(p_port_data,
                                              ICF_PORT_SIZEOF(icf_api_header_st) +
                                              ICF_PORT_SIZEOF(icf_nw_interface_info_st),
                                              ICF_MEM_COMMON,p_ecode);

                         if (p_msg) {
						 p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);
                
                         if ( ICF_SUCCESS == icf_port_accept(
                              p_glb_port_data->win_ipc_info.\
                              self_fd_nw[i].fd_id,
                              &(p_nw_msg->receiver_fd),
                              p_nw_msg->remote_ip_addr,
                              &(p_nw_msg->remote_port_num)))
                         {
                              p_msg->hdr.api_id = ICF_NW_API_OPEN_CONN_IND;
                              p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 
                              p_msg->hdr.destination_id = ICF_MODULE_ICF; 
                              p_msg->hdr.call_id = ICF_INVALID_CALL_ID;


                              p_msg->hdr.api_length  = 
                                  ICF_PORT_SIZEOF(icf_api_header_st) +\
                                  ICF_PORT_SIZEOF(icf_nw_interface_info_st);
                 
                              icf_es_add_fd_to_es_tcp_db(p_glb_port_data->ext_info.read_tcp_conn, 
                                          (icf_uint16_t)p_nw_msg->receiver_fd,ICF_FALSE);
                              p_glb_port_data->ext_info.num_of_conn++;

                              FD_SET(p_nw_msg->receiver_fd, 
                                     &(p_glb_port_data->win_ipc_info.win_read_fd));
				
                               /* each time u write to win_read_fd, add the fd to the
                                * act_read_fd also. This will be used for clearing the
                                * the FD set. Since FD_CLR is not working */
#if 0
							  /*Changes done in load of TCP Rel6.4*/
                               p_glb_port_data->win_ipc_info.act_read_fd[
                                   p_glb_port_data->win_ipc_info.act_fd_count] = 
                               p_nw_msg->receiver_fd;

                               p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif
                               p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
                               p_nw_msg->pdu_len = 0;
                               p_nw_msg->p_sip_msg = ICF_NULL;

                               p_icf_msg_list->p_msg = p_msg;
                               p_icf_msg_list->p_msg_next = ICF_NULL;
                         }
                         else
                         {
                             ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));
                             icf_port_memfree(p_port_data,p_msg,
                                              ICF_MEM_COMMON,p_ecode);
                             ret_val = ICF_FAILURE;
                         }
						 }
                     }
                 }
#ifdef ICF_SECURE_SUPPORT
                 else if (ICF_WIN_TLS_SIP_SERVER == 
                     p_glb_port_data->win_ipc_info.self_fd_nw[i].server_type)
                 {
                     /* Message has been received on TLS server
                      * this means an incoming connection */
                     /*SPR 19873 init the p_msg */
                     icf_msg_st               *p_msg = ICF_NULL;
                     icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

                     ICF_ES_TRACE(((icf_uint8_t *)"\nIncomingTCPConnection"));

                     if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == 
                         p_glb_port_data->ext_info.num_of_pending_conn + 
                         p_glb_port_data->ext_info.num_of_conn)
                     {
                         ret_val = ICF_FAILURE;
                         ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));
                     }
                     if ( ICF_SUCCESS == ret_val)
                     {
                         p_msg = (icf_msg_st*)icf_port_memget(p_port_data,
                                 ICF_PORT_SIZEOF(icf_api_header_st) +
                                 ICF_PORT_SIZEOF(icf_nw_interface_info_st),
                                 ICF_MEM_COMMON,p_ecode);
						if (p_msg) {
                         p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);
                         if ( ICF_SUCCESS == icf_port_accept(
                                   p_glb_port_data->win_ipc_info.\
                                   self_fd_nw[i].fd_id,
                                   &(p_nw_msg->receiver_fd),
                                   p_nw_msg->remote_ip_addr,
                            &(p_nw_msg->remote_port_num)))
                         {
                             icf_void_t      *p_secure_ctx = ICF_NULL;
                             /* Now extract the SSL ctx corrresponding to this
                              * socket fd from the map */
                             p_secure_ctx = icf_es_fetch_secure_data_from_map(
                                  p_glb_port_data,
                                  p_glb_port_data->win_ipc_info.self_fd_nw[i].fd_id,
                                  p_ecode);
                             if (ICF_NULL == p_secure_ctx)
                             {
                                 ret_val = ICF_FAILURE;
                             }
                             else
                             {
                                icf_void_t     *p_secure_data = ICF_NULL ;
                                /* Invoke porting function to issue SSL accept */
                                p_secure_data = icf_port_secure_accept(
                                         p_secure_ctx,p_nw_msg->receiver_fd,p_ecode);
                               if (ICF_NULL == p_secure_data)
                               {
                                   ret_val = ICF_FAILURE;
                               }
                               else
                               {
                                  /* Add a maaping between the SSL data and the socket fd in 
                                   * the mapping maintained in the ES */
                                  if (ICF_FAILURE == icf_es_add_secure_data_to_map(
                                        p_secure_data,p_glb_port_data,
                                        p_nw_msg->receiver_fd,
                                        p_ecode))
                                  {
                                      icf_port_free_secure_data(p_secure_data);
                                      ret_val = ICF_FAILURE;
                                  }
                               }
                            }
                            if (ICF_SUCCESS == ret_val)
                            {
                                p_msg->hdr.api_id = ICF_NW_API_OPEN_CONN_IND;
                                p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 
                                p_msg->hdr.destination_id = ICF_MODULE_ICF; 
                                p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

                                p_msg->hdr.api_length  = 
                                    ICF_PORT_SIZEOF(icf_api_header_st) +\
                                    ICF_PORT_SIZEOF(icf_nw_interface_info_st);
                    
                                icf_es_add_fd_to_es_tcp_db(p_glb_port_data->ext_info.read_tcp_conn,
                                    (icf_uint16_t)p_nw_msg->receiver_fd,ICF_TRUE);
                                p_glb_port_data->ext_info.num_of_conn++;
                                FD_SET(p_nw_msg->receiver_fd, 
                                    &(p_glb_port_data->win_ipc_info.win_read_fd));

#if 0
                                /* each time u write to win_read_fd, add the fd to the
                                 * act_read_fd also. This will be used for clearing the
                                 * the FD set. Since FD_CLR is not working */
                               p_glb_port_data->win_ipc_info.act_read_fd[
                                     p_glb_port_data->win_ipc_info.act_fd_count] = 
                                     p_nw_msg->receiver_fd;
                               p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif
                               p_nw_msg->bitmap |= ICF_INTF_INFO_PROTOCOL_TLS;
                               p_nw_msg->pdu_len = 0;
                               p_nw_msg->p_sip_msg = ICF_NULL;

                               p_icf_msg_list->p_msg = p_msg;
                               p_icf_msg_list->p_msg_next = ICF_NULL;
                           }
                           else
                           {
                               ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));
                               icf_port_memfree(p_port_data,p_msg,
                                    ICF_MEM_COMMON,p_ecode);
                               ret_val = ICF_FAILURE;
                            }
                         }
						 /* SPR 19927 : Free the message data in case of failure in
						  * accepting connection
						  */
                         else
                         {
                             ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));
                             icf_port_memfree(p_port_data,p_msg,
                                              ICF_MEM_COMMON,p_ecode);
                             ret_val = ICF_FAILURE;
                         }

				ICF_KLOCWORK_FALSE_POSITIVE (p_persistent_glb_pdb, p_nw_msg->receiver_fd)                         
						 }
                     }
                 }
#endif
                 fd_found = ICF_TRUE;
                 break;
             }
         }
	}//ZyXEL porting


         if ((ICF_FAILURE == ret_val) || (ICF_TRUE == fd_found))
         {
              /*Do nothing*/
         }
/*ZyXEL porting*/
#if 0 
		 else if(ICF_PORT_FD_ISSET(p_glb_port_data->win_ipc_info.self_fd_ipc, &read_fd_set))

		{

                /* the data is received from the IPC */

			/* call win ported function to recive the message */

               if ((encoded_buf_length = icf_port_recvfrom(p_glb_port_data->win_ipc_info.self_fd_ipc, 
                    p_interface_msg)) == -1) 
               {
#ifdef ICF_PORT_WINDOWS
                    int ecode = WSAGetLastError();
	            ecode=ecode;
#endif
                    ICF_ES_TRACE(((icf_uint8_t *)"\nProblem in reading data from Application interface"));
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:MSG received successfuly from App: %d bytes",

						encoded_buf_length));                    
#ifndef ICF_SSA_ES_UT                   

                    if (ICF_FAILURE == 

                        icf_decode_msg(&p_recv_msg, p_interface_msg, encoded_buf_length, &ecode)) 

                    {

                        printf("ICF clib:Message decoding failure for ICF\n");

                    }

                    else

                    {

                        p_icf_msg_list->p_msg = (icf_msg_st *)p_recv_msg;

                        p_icf_msg_list->p_msg_next = ICF_NULL;
                        if(p_recv_msg->hdr.source_id != ICF_MODULE_NAT)
                        { 
                            icf_print_msg_hdr(p_icf_msg_list->p_msg);
                        }  				



#ifdef ICF_DEBUG_MEMPOOL

                        if(ICF_MEM_POOL_STATS_PRINT == p_icf_msg_list->p_msg->hdr.api_id)

                        {

                            icf_port_print_mem_pool_stats();

                            icf_free_decoded_msg(p_recv_msg, &ecode);

                            p_icf_msg_list->p_msg = ICF_NULL;

                            /* No Need to process in MRM process Message */

                            icf_port_memfree(p_port_data,p_interface_msg,ICF_MEM_COMMON,p_ecode);

                            return ICF_SUCCESS;

                        }

#endif   
                    }
#endif
                }

		}
#endif
/*ZyXEL porting end*/

#ifdef ICF_NAT_MEDIA_TRAVERSAL
        else if(ICF_PORT_FD_ISSET(p_glb_port_data->win_ipc_info.nat_fd_ipc, &read_fd_set))
        {
            /* the data is received for NAT Library */
            /* call win ported function to recive the message */
            
            if ((encoded_buf_length = 
                icf_port_recvfrom(p_glb_port_data->win_ipc_info.nat_fd_ipc, 
                p_interface_msg)) == -1) 
            {
#ifdef ICF_PORT_WINDOWS
                int ecode = WSAGetLastError();
		  ecode = ecode;	//ZyXEL porting
#endif
                ICF_ES_TRACE(((icf_uint8_t *)"\nProblem in reading data from Application interface"));
                ret_val = ICF_FAILURE;
            }
            else
            {
                nat_msg_st *p_nat_msg = ICF_NULL;

                
                p_nat_msg = (nat_msg_st *)icf_port_static_memget(
                               encoded_buf_length,p_ecode);

                icf_port_memcpy(p_nat_msg,(nat_msg_st *)p_interface_msg,encoded_buf_length);

                p_nat_msg->msg_hdr.source_id = NAT_MODULE_APP;
                p_nat_msg->msg_hdr.destination_id = NAT_MODULE_NAT;
                
                ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:MSG received successfuly For NAT: %d bytes",
                    
                    encoded_buf_length));
                
                
                if (ICF_FAILURE == nat_process_mesg(p_nat_msg)) 
                {
                    /*printf("ICF NAT: unable to process the message");*/                }
            }
        }
#endif

		else if (ICF_PORT_FD_ISSET(\

			p_glb_port_data->win_ipc_info.

			curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER],

			&read_fd_set))

		{

			/* Message has been received on UDP socket */

			interface_info.fd_id = 

				p_glb_port_data->win_ipc_info.\

				curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER];

			interface_info.interface_type = ICF_INTF_TYPE_UDP;

			

			/* port to fill dest port and dest ip */

			if(ICF_SUCCESS == icf_port_udp_recv(&interface_info,

				p_interface_msg, p_ecode))

			{

				if(ICF_FAILURE == icf_sipal_process_incoming_msg(

					&interface_info,

					p_interface_msg,

					p_icf_msg_list,

					p_ecode))

				{

					ICF_ES_TRACE(((icf_uint8_t *)"\nES: Sip AL returned failure"));

				}

			}

			else

			{

				ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on UDP"));

				ret_val = ICF_FAILURE;

			}

        }

        else if (ICF_PORT_FD_ISSET(p_glb_port_data->win_ipc_info.
            curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER],&read_fd_set))
        {

           /* Message has been received on TCP server

            * this means an incoming connection */
            /*SPR 19873 init the p_msg */
            icf_msg_st               *p_msg = ICF_NULL;

            icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

            

            ICF_ES_TRACE(((icf_uint8_t *)"\nIncomingTCPConnection"));

            

            if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == 

                p_glb_port_data->ext_info.num_of_pending_conn + 

                p_glb_port_data->ext_info.num_of_conn)

            {

                ret_val = ICF_FAILURE;

                ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));

            }

            if ( ICF_SUCCESS == ret_val)

            {

                p_msg = (icf_msg_st*)icf_port_memget(p_port_data,

                    ICF_PORT_SIZEOF(icf_api_header_st) +

                    ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                    ICF_MEM_COMMON,p_ecode);

                
				if (p_msg) {
                p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

                if ( ICF_SUCCESS == icf_port_accept(

                    p_glb_port_data->win_ipc_info.\

                    curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER],

                    &(p_nw_msg->receiver_fd),

                    p_nw_msg->remote_ip_addr,

                    &(p_nw_msg->remote_port_num)))

                {

                    p_msg->hdr.api_id = ICF_NW_API_OPEN_CONN_IND;

                    p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 

                    p_msg->hdr.destination_id = ICF_MODULE_ICF; 

                    p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

                    

                    p_msg->hdr.api_length  = 

                        ICF_PORT_SIZEOF(icf_api_header_st) +\

                        ICF_PORT_SIZEOF(icf_nw_interface_info_st);

                    

                    icf_es_add_fd_to_es_db((icf_uint32_t*)p_glb_port_data->

                        ext_info.read_tcp_conn, 

                        p_nw_msg->receiver_fd);

                    p_glb_port_data->ext_info.num_of_conn++;

                    FD_SET(p_nw_msg->receiver_fd, 

                        &(p_glb_port_data->win_ipc_info.win_read_fd));

                        /* each time u write to win_read_fd, add the fd to the

                        * act_read_fd also. This will be used for clearing the

                    * the FD set. Since FD_CLR is not working */

                    p_glb_port_data->win_ipc_info.act_read_fd[

                        p_glb_port_data->win_ipc_info.act_fd_count] = 

                        p_nw_msg->receiver_fd;

                    p_glb_port_data->win_ipc_info.act_fd_count += 1;

                    

                    p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TCP;

                    p_nw_msg->pdu_len = 0;

                    p_nw_msg->p_sip_msg = ICF_NULL;

                    

                    p_icf_msg_list->p_msg = p_msg;

                    p_icf_msg_list->p_msg_next = ICF_NULL;

                }

                else

                {

                    ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in accepting connection"));

                    icf_port_memfree(p_port_data,p_msg,

                        ICF_MEM_COMMON,p_ecode);

                    ret_val = ICF_FAILURE;

                }
				}

            }

        }
        /*Now check if selected FD is FD related to TCP or TLS connections, either
         *TCP/TLS connection response pending or some SIP message received on this TCP/TLS
         *connection*/
        if (ICF_SUCCESS == ret_val)
        {

            /*SPR 19873 init the p_msg */
            icf_msg_st               *p_msg = ICF_NULL;

            icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

            icf_uint32_t              count = 0;

            /*Fix for SPR 19531 (Klocwork Warning removal)*/
			icf_uint32_t				except_conn_fd[ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS];

#if ICF_PORT_WINDOWS
			/* MPH changes Start */
                icf_uint8_t                     is_mph_msg   = ICF_FALSE;
                icf_uint32_t						count_mph    = 0;
				static icf_uint8_t              mph_frag_chk = 0;
                static icf_uint32_t             http_pdu_len = 0;
                static icf_uint8_t              *p_http_msg  = ICF_NULL;
				static icf_uint8_t              *p_buffer    = ICF_NULL;
                for (count_mph = 0; count_mph < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count_mph++)
                {
                    
                    if(ICF_PORT_FD_ISSET(p_glb_port_data->win_ipc_info.
                        mph_tcp_conn_fd[count_mph], &read_fd_set))
                    {
                        is_mph_msg = ICF_TRUE;
                        break;
                    }
                }
                
                if (ICF_TRUE == is_mph_msg)
                {
                    icf_return_t                ret_val     = ICF_SUCCESS;
                    
                    icf_secure_nw_interface_st  *p_sec_nw_msg   = ICF_NULL; 
                    
                    ICF_ES_TRACE(((icf_uint8_t *)"[ES]: Event Received Indication on TLS conn\n"));

                    p_msg = (icf_msg_st*)icf_port_memget(p_port_data,
                        ICF_PORT_SIZEOF(icf_api_header_st) +
                        ICF_PORT_SIZEOF(icf_secure_nw_interface_st),
                        ICF_MEM_COMMON,&ecode);

                    if (ICF_NULL == p_msg)
                    {
                        ret_val = ICF_FAILURE;			
                    }
                    else
                    {
                        p_msg->hdr.source_id      = ICF_MODULE_MGMT_SERVER; 
                        p_msg->hdr.destination_id = ICF_MODULE_ICF; 
                        
                            
                        p_msg->hdr.api_length  = 
                                ICF_PORT_SIZEOF(icf_api_header_st) +\
                                ICF_PORT_SIZEOF(icf_secure_nw_interface_st);
                            
                        p_msg->hdr.call_id  = (icf_uint16_t )ICF_INVALID_CALL_ID;
                            
                        p_sec_nw_msg  = (icf_secure_nw_interface_st*)(p_msg->payload);
                            
                        p_sec_nw_msg->conn_handle = p_glb_port_data->win_ipc_info.
                                mph_tcp_conn_fd[count_mph];
                        
                        p_sec_nw_msg->pdu_len = 0;

                        p_sec_nw_msg->p_http_msg = ICF_NULL;

                        p_buffer = (icf_uint8_t*)icf_port_memget(
													p_port_data,
													TCP_RECV_BUFFER,     /* Windows supports TCP recv buffer size of 8K */
													ICF_MEM_COMMON,&ecode);

                        if (ICF_NULL == p_buffer)
                        {
                            ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Memory Allocation failure for MPH Buffering\n"));
                            return ICF_FAILURE;
                        }

                        
                        if ((pdu_length = 
                            icf_port_recvfrom(p_glb_port_data->win_ipc_info.mph_tcp_conn_fd[count_mph],                       
                            p_buffer)) == -1) 
                        {
                            ICF_ES_TRACE(((icf_uint8_t *)"\nProblem in reading data from Application interface\n"));
                            ret_val = ICF_FAILURE;                            
                        }
                        else if (0 == pdu_length)
                        {
                            ICF_ES_TRACE(((icf_uint8_t *)"Connection from Remote closed\n"));

                            p_msg->hdr.api_id = ICF_NW_API_CLOSE_CONN_IND;

                            FD_CLR(p_glb_port_data->win_ipc_info.mph_tcp_conn_fd[count_mph], 
                                    &p_glb_port_data->win_ipc_info.win_read_fd);
                            
                            p_icf_msg_list->p_msg = p_msg;
                            p_icf_msg_list->p_msg_next = ICF_NULL;
                        }
                        else
                        {
						   p_sec_nw_msg->pdu_len = (icf_uint16_t)pdu_length;
                            
                            ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:MSG received successfuly from App: %d bytes",
                                http_pdu_len));  

                            if (mph_frag_chk == 0)
                            {
                                p_http_msg = (icf_uint8_t*)icf_port_memget(
													p_port_data,
													p_sec_nw_msg->pdu_len,
													ICF_MEM_COMMON,&ecode);
                                if (ICF_NULL == p_http_msg)
                                {
                                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Memory Allocation failure for MPH Buffering\n"));
                                    return ICF_FAILURE;
                                }
                                else
                                {
                                    http_pdu_len = p_sec_nw_msg->pdu_len;
                                    memcpy(p_http_msg, p_buffer, p_sec_nw_msg->pdu_len);
									p_http_msg[http_pdu_len] = '\0';
                                    icf_port_memfree(p_port_data, p_buffer, ICF_MEM_COMMON, &ecode);
                                }
                            }
                            else  /* Fragmentation Issue */
                            {
                                icf_uint8_t	 *p_temp_http_msg = ICF_NULL;
                                icf_uint32_t cpy_from = 0; 
                                
                                http_pdu_len += p_sec_nw_msg->pdu_len;
                                cpy_from     = (http_pdu_len - (p_sec_nw_msg->pdu_len));
                                p_sec_nw_msg->pdu_len = (icf_uint16_t)http_pdu_len;

                                p_temp_http_msg = (icf_uint8_t*)icf_port_memget(
															p_port_data,
															(http_pdu_len - p_sec_nw_msg->pdu_len),
															ICF_MEM_COMMON,&ecode);
                                if (ICF_NULL == p_temp_http_msg)
                                {
                                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Memory Allocation failure for MPH Buffering\n"));
                                    return ICF_FAILURE;
                                }
                                else
                                {
                                    icf_port_memcpy(p_temp_http_msg, 
                                                    p_http_msg, 
                                                    (http_pdu_len - p_sec_nw_msg->pdu_len));

                                    icf_port_memfree(p_port_data, p_http_msg, ICF_MEM_COMMON, &ecode);

                                    p_http_msg = ICF_NULL;
                                    p_http_msg = (icf_uint8_t*)icf_port_memget(
                                                                  p_port_data,
                                                                  (http_pdu_len+1),
                                                                  ICF_MEM_COMMON,&ecode);
                                    
                                    if (ICF_NULL == p_http_msg)
                                    {
                                        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Memory Allocation failure for MPH Buffering\n"));
                                        return ICF_FAILURE;
                                    }
                                    else
                                    {
                                        icf_port_memcpy(p_http_msg, 
                                                    p_temp_http_msg, 
                                                    (http_pdu_len - p_sec_nw_msg->pdu_len));

                                        memcpy((p_http_msg + cpy_from), 
										            p_buffer, 
										            p_sec_nw_msg->pdu_len);
                                    }
                                }
                            }
                        
                            if (ICF_SUCCESS == (icf_port_isCompleteHTTPResp(
                               (icf_void_t*)p_port_data, p_http_msg, &ecode)))
                            {
                                mph_frag_chk = 0;

                                p_sec_nw_msg->p_http_msg = (icf_uint8_t*)icf_port_memget(
                                    p_port_data,
                                    http_pdu_len,
                                    ICF_MEM_COMMON,
                                    &ecode);
                                
                                if (ICF_NULL == p_sec_nw_msg->p_http_msg)
                                {
                                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Complete HTTP message BUT memory Allocation failed\n"));
                                }
                                else
                                {
                                    memcpy(p_sec_nw_msg->p_http_msg, p_http_msg, http_pdu_len);
                                    p_sec_nw_msg->pdu_len = http_pdu_len;

                                    p_msg->hdr.api_id         = ICF_NW_API_MSG_FROM_PEER;

                                    p_icf_msg_list->p_msg = p_msg;
                                    p_icf_msg_list->p_msg_next = ICF_NULL;

                                    icf_port_memfree(p_port_data, p_http_msg, ICF_MEM_COMMON, &ecode);
                                    
                                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Complete HTTP message...calling mph_process_msg()\n"));
                                }
                            } 
                            else
                            {
                                ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]: Invalid/Incomplete HTTP message\n"));

                                /* Probably the message is fragmented */
                                mph_frag_chk++;
                            }
                        }
                    }
                }
                else
                {
#endif
                    /* MPH changes End */

            /* Now we loop over write fd set to check if message has

             * been received on TCP write FDs

             * this means an outgoing connection has reached some status

             * Now we will check it against the pending conn FD list.

             * At the first match we call getpeername.If it returns

             * success, this means connect was successful else we send

			 * failure to SSA */



            p_msg = (icf_msg_st*)icf_port_memget(p_port_data,

                    ICF_PORT_SIZEOF(icf_api_header_st) +

                    ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                    ICF_MEM_COMMON,p_ecode);

            if (ICF_NULL == p_msg)
            {
                icf_port_memfree(p_port_data,p_interface_msg,ICF_MEM_COMMON,
                        p_ecode);
                return ICF_FAILURE;
            }

            p_msg->hdr.api_id = ICF_API_ID_UNDEFINED;

            p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

            p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

            p_msg->hdr.destination_id = ICF_MODULE_ICF;

            p_msg->hdr.source_id = ICF_MODULE_SIP_NW;

			for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count++)

            {



 				/* check for exception first */

				if (ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                            pending_connect_fd[count],&except_fd_set))

				{
                    /* Closing the connection fd if there is a connect
                     * failure*/
                    icf_port_close_socket(p_glb_port_data->ext_info.pending_connect_fd[count],p_ecode);
                    p_msg->hdr.api_id = ICF_NW_API_CONNECT_FAILURE;





                    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:ConnectFailureInd"));



					except_conn_fd[count] = 

                        p_glb_port_data->ext_info.pending_connect_fd[count];



					p_nw_msg->receiver_fd = except_conn_fd[count];



					icf_es_rem_fd_from_es_db(

                            except_conn_fd,

							except_conn_fd[count]);



					/*Add to except fd set*/

				/*	FD_SET(except_conn_fd[count],

                                &(p_glb_port_data->win_ipc_info.win_except_fd));*/

               

					p_glb_port_data->ext_info.num_of_pending_conn--;

                    /*FD_CLR(p_nw_msg->receiver_fd,

                            &(p_glb_port_data->ext_info.write_fdset));*/

					FD_CLR(p_nw_msg->receiver_fd, 

						&(p_glb_port_data->win_ipc_info.win_write_fd));

					



					p_nw_msg->pdu_len = 0;

                    p_nw_msg->p_sip_msg = ICF_NULL;

                    break;

				}

			    else	

			    if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.

                            pending_connect_fd[count], &write_fd_set))

                {

                    if ( ICF_TRUE == icf_port_isconnected

                            (p_glb_port_data->ext_info.pending_connect_fd[count]))

                    {

                        icf_es_add_fd_to_es_tcp_db(

                                p_glb_port_data->ext_info.read_tcp_conn, 
                                p_glb_port_data->ext_info.pending_connect_fd[count],
                                ICF_FALSE);


                        p_glb_port_data->ext_info.num_of_conn++;

                        FD_SET(p_glb_port_data->ext_info.pending_connect_fd[count],

                                &(p_glb_port_data->win_ipc_info.win_read_fd));

						/* each time u write to win_read_fd, add the fd to the

					 * act_read_fd also. This will be used for clearing the

					 * the FD set. Since FD_CLR is not working */
#if 0

						/*Changes for load*/
					p_glb_port_data->win_ipc_info.act_read_fd[

						p_glb_port_data->win_ipc_info.act_fd_count] = 

						p_glb_port_data->ext_info.pending_connect_fd[count];

					p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif


                        p_msg->hdr.api_id = ICF_NW_API_CONNECT_SUCCESS;

                        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:ConnectSuccessInd"));
#ifdef ICF_LOAD_DEBUG
                        printf("\n[ES]:ConnectSuccessInd");
#endif

                    }

                    else

                    {
                        /* Closing the connection fd if there is a connect
                         * failure */
                        icf_port_close_socket(p_glb_port_data->ext_info.pending_connect_fd[count],p_ecode);

                        p_msg->hdr.api_id = ICF_NW_API_CONNECT_FAILURE;

                        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:ConnectFailureInd"));

                    }

                    

					p_nw_msg->receiver_fd = 

                        p_glb_port_data->ext_info.pending_connect_fd[count];

                    icf_es_rem_fd_from_es_db(

                            p_glb_port_data->ext_info.pending_connect_fd, 

                           p_glb_port_data->ext_info.pending_connect_fd[count]);

                    p_glb_port_data->ext_info.num_of_pending_conn--;

                    /*FD_CLR(p_nw_msg->receiver_fd,

                            &(p_glb_port_data->ext_info.write_fdset));*/

					FD_CLR(p_nw_msg->receiver_fd, 

						&(p_glb_port_data->win_ipc_info.win_write_fd));

#if 0
					memset(&p_glb_port_data->win_ipc_info.win_write_fd,

						0, FD_SETSIZE);
#endif


				    p_nw_msg->pdu_len = 0;

                    p_nw_msg->p_sip_msg = ICF_NULL; 

					break;

				}

	        }

            if ( ICF_FAILURE != ret_val &&

                    count == ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS)

            {/*No status update on outgoing connects, now we check for data on

               existing connections*/

                for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count++)

                {

                   if(ICF_PORT_FD_ISSET(p_glb_port_data->ext_info.
                                read_tcp_conn[count].fd_id, &read_fd_set))
                    {

                        interface_info.fd_id = p_glb_port_data->ext_info.
                            read_tcp_conn[count].fd_id;
#ifdef ICF_SECURE_SUPPORT

                        if (ICF_TRUE == p_glb_port_data->ext_info.

                                read_tcp_conn[count].is_secure_conn)

                        {

                            icf_void_t      *p_secure_data = ICF_NULL;

                            interface_info.interface_type = ICF_INTF_TYPE_TLS;

                            /* 
                             * Now extract the SSL Data corrresponding to this
                             * socket fd from the map 
                             */
                            p_secure_data = icf_es_fetch_secure_data_from_map(

                                    p_glb_port_data,

                                    p_glb_port_data->ext_info.read_tcp_conn[count].fd_id,

                                    p_ecode) ;

                            if (ICF_NULL == p_secure_data)

                            {
                                ret_val = ICF_FAILURE;
                            }
                            else if (ICF_FAILURE == icf_port_secure_peek(

                                p_secure_data,&interface_info, (icf_void_t*)(p_glb_port_data->

                                ext_info.p_tcp_recv_buffer), p_ecode))

                            {
                               ret_val = ICF_FAILURE;
	                         }
                        }
                        else
                        { 

#endif				

                        interface_info.interface_type = ICF_INTF_TYPE_TCP;
						ret_val = icf_port_tcp_peek(&interface_info,
								(icf_void_t*)(p_glb_port_data->\
                                ext_info.p_tcp_recv_buffer), p_ecode);

#ifdef ICF_SECURE_SUPPORT

                        }

#endif
                        if (ICF_SUCCESS == ret_val)
                        {
							p_nw_msg->receiver_fd = 
								
								p_glb_port_data->ext_info.read_tcp_conn[count].fd_id;
							
							p_msg->hdr.call_id = ICF_INVALID_CALL_ID;
							
							p_msg->hdr.destination_id = ICF_MODULE_ICF;
							
							p_msg->hdr.source_id = ICF_MODULE_SIP_NW;
							
							
							
							if ( 0 != interface_info.msg_len)
								
							{
								
								p_msg->hdr.api_id = ICF_NW_API_MSG_FROM_PEER;
								
								p_nw_msg->pdu_len = (icf_uint16_t)interface_info.msg_len;
								
								p_nw_msg->p_sip_msg =  
										p_glb_port_data->ext_info.p_tcp_recv_buffer;
								
							}
							
							else
								
							{
								
								p_msg->hdr.api_id = ICF_NW_API_CLOSE_CONN_IND;
								
								p_nw_msg->pdu_len = 0;
								
								p_nw_msg->p_sip_msg = ICF_NULL;
								
							}
						}
                        /* Changes for CSR 1-5323169 */
                        else /* Handling for TCP peek failure */
                        {
                            /* Fix for TCP load issue seen in testing with
                            * Nortel CS1K. It was noticed that CS1K was sending
                            * [RST,ACK] for a TCP connection immediately after
                            * opening it.
                            * So select() was returning that FD as set but TCP
                            * recv() invoked for peek was failing with system
                            * error code ECONNRESET. This was happening in a
                            * loop for sometime during which IPTk's CPU usage
                            * was very high.
                            */

                            /* Send close conn ind to SSA */
                            p_nw_msg->receiver_fd = p_glb_port_data->ext_info.read_tcp_conn[count].fd_id;
                            p_msg->hdr.call_id = ICF_INVALID_CALL_ID;
                            p_msg->hdr.destination_id = ICF_MODULE_ICF;
                            p_msg->hdr.source_id = ICF_MODULE_SIP_NW;
                            p_msg->hdr.api_id = ICF_NW_API_CLOSE_CONN_IND;
                            p_nw_msg->pdu_len = 0;
                            p_nw_msg->p_sip_msg = ICF_NULL;

                            /* Also reset ret_val to SUCCESS otherwise this msg
                            * won't get added to p_icf_msg_list below.
                            */
                            ret_val = ICF_SUCCESS;
                        }

                        break;

                    }

                }/*Loop over all the readable TCP connections*/

            }

            if ( ICF_FAILURE != ret_val &&

                    ICF_API_ID_UNDEFINED != p_msg->hdr.api_id)

            {
                p_msg->hdr.source_id = ICF_MODULE_SIP_NW;

                p_msg->hdr.destination_id = ICF_MODULE_ICF;

                p_msg->hdr.api_length  = 

                    ICF_PORT_SIZEOF(icf_api_header_st) +\

                    ICF_PORT_SIZEOF(icf_nw_interface_info_st);

                p_msg->hdr.call_id  = ICF_INVALID_CALL_ID;


               if (ICF_INTF_TYPE_TCP == interface_info.interface_type)
			   {
					p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
				}
				else if (ICF_INTF_TYPE_TLS == interface_info.interface_type)
				{
					p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
				}
              /* append external message p_msg in mrm message list*/
 
                if(ICF_FAILURE == icf_es_append_to_msg_list(p_msg,
                                          p_persistent_glb_pdb,
                                          &ecode))
                 {
                     ret_val = ICF_FAILURE;
                 }

            }

            else

            {

       #ifdef ICF_NAT_MEDIA_TRAVERSAL
         /*       ICF_ES_TRACE(((icf_uint8_t *)"\nES:NoStatusUpdate on PendingConn or NoDataOnExistingConn"));*/

                /* Here now check for NAT Message */
                icf_uint8_t count = 1;

                for(count =1; count < p_glb_port_data->win_ipc_info.nat_ipc_count;count++)
                {
                    if(ICF_PORT_FD_ISSET(\
                                p_glb_port_data->win_ipc_info.nat_ipc_info[count].binding_fd,
                                &read_fd_set))
                    {
                        break;
                    }
                }


                /* Message has been received on UDP socket from NAT SERVER */
             if(count == p_glb_port_data->win_ipc_info.nat_ipc_count)
             {
                 icf_port_memfree(p_port_data,p_msg,
                        ICF_MEM_COMMON,p_ecode);

                 /* unidentified message received */
                 return NAT_RETURN_FAILURE;
             }
             else
             {
                 interface_info.fd_id =
                                p_glb_port_data->win_ipc_info.nat_ipc_info[count].binding_fd;
             }

             interface_info.interface_type = ICF_INTF_TYPE_UDP;

             /* port to fill dest port and dest ip */
             if(NAT_RETURN_SUCCESS == icf_port_udp_recv(&interface_info,
                         p_interface_msg, p_ecode))
             {
                nat_msg_st *p_nat_msg;
                
                p_nat_msg = (nat_msg_st *)icf_port_static_memget(
                        ICF_PORT_SIZEOF(nat_msg_hdr_st) + encoded_buf_length,
                        p_ecode);

                p_nat_msg->msg_hdr.source_id = NAT_MODULE_NW_UDP;
                p_nat_msg->msg_hdr.destination_id = NAT_MODULE_NAT;
                p_nat_msg->msg_hdr.api_id = NAT_RECV_UDP_PKT_IND;
                p_nat_msg->msg_hdr.api_length = ICF_PORT_SIZEOF(nat_msg_hdr_st) 
                                                    + encoded_buf_length;
                
                icf_port_memcpy(p_nat_msg->p_msg_data,
                            (nat_msg_st *)p_interface_msg,encoded_buf_length);

                ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:MSG received successfuly For NAT: %d bytes",
                    encoded_buf_length));
                
                
                if (ICF_FAILURE == nat_process_mesg(p_nat_msg)) 
                {
                    /*printf("ICF NAT: unable to process the message");*/
                }
                else
                {
                    *p_ecode = ICF_ECODE_NAT_MESSAGE_RECEIVED;
                }
              }
             else
             {
                 /*NAT_ES_TRACE(("\nES:Problem in reading data on UDP"));*/
                 ret_val = NAT_RETURN_FAILURE;
             }
#endif
            /* free memory allocated to p_msg*/
            icf_port_memfree(p_port_data,p_msg,ICF_MEM_COMMON,p_ecode);

            }
#if ICF_PORT_WINDOWS
        }

#endif
#endif

	}
    icf_port_memfree(p_port_data,p_interface_msg,ICF_MEM_COMMON,p_ecode); 
    }

 /*   ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_recv with ret_val %d",

                (icf_int32_t)ret_val)); */

#ifdef ICF_INT_TRACE

    {

        icf_msg_list_st* temp = p_icf_msg_list;

        while (temp) {

            ICF_INTERFACE_TRACE(temp->p_msg)

            temp = temp->p_msg_next;

        }

    }

#endif





    return ret_val;

}





icf_return_t icf_sip_nw_process_outgoing_msg(

      IN   icf_interface_info_st *p_interface_info,

      IN   icf_void_t  *p_msg_data,

      INOUT   icf_error_t *p_ecode)

{



    icf_return_t   ret_val = ICF_SUCCESS;

    icf_nw_interface_info_st *p_nw_msg=ICF_NULL;  

    

    p_nw_msg = (icf_nw_interface_info_st *)p_msg_data;

     

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_sip_nw_process_outgoing_msg"));



    ret_val = icf_os_send(p_interface_info,

            (icf_void_t *)p_nw_msg->p_sip_msg, 

            p_ecode);



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_sip_nw_process_outgoing_msg with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}





#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

icf_return_t icf_cfgal_process_incoming_msg(

                        icf_interface_info_st *p_interface_info,        

                        icf_uint8_t      *p_api_header,

                        icf_msg_list_st  *p_icf_msg_list,

                        icf_error_t      *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_uint32_t   fd_id = ICF_NULL;

    icf_msg_st     *p_cfg_msg = ICF_NULL;

    icf_global_port_info_st *p_glb_port_data = 

        (icf_global_port_info_st *)p_interface_info->p_port_info;

    

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_cfgal_process_incoming_msg"));



    /*Get the memory of size received in api header */

    p_cfg_msg = (icf_msg_st *)icf_port_memget(p_glb_port_data,

                         ((icf_api_header_st*)p_api_header)->api_length ,

                          ICF_MEM_COMMON,p_ecode);



    /* Copy the api header in icf_msg_st header */

    icf_port_memcpy(&(p_cfg_msg->hdr),p_api_header,

            ICF_PORT_SIZEOF(icf_api_header_st));





    /* Message has been received on CFG message pipe */

    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_CONF];

    

    /* Get length for message using peek and getting payload 

     * length from message heaader */ 

    if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,p_cfg_msg->payload,

                 ((icf_api_header_st*)p_api_header)->api_length -

                   ICF_PORT_SIZEOF(icf_api_header_st), p_ecode))

    {

        /* Copy the pa message in msg_list */

        p_icf_msg_list->p_msg = p_cfg_msg;

        p_icf_msg_list->p_msg_next = ICF_NULL;         

    } 

    else

    {

        ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on CFG pipe"));

        ret_val = ICF_FAILURE;

    }    



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_cfgal_process_incoming_msg\
                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}





icf_return_t icf_paal_process_incoming_msg(

                        icf_interface_info_st *p_interface_info,

                        icf_uint8_t      *p_api_header,

                        icf_msg_list_st  *p_icf_msg_list,

                        icf_error_t      *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_uint32_t   fd_id = ICF_NULL;

    icf_msg_st     *p_pa_msg = ICF_NULL;

    icf_global_port_info_st *p_glb_port_data = 

        (icf_global_port_info_st *)p_interface_info->p_port_info;

    

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_paal_process_incoming_msg"));



    /*Get the memory of size received in api header */

    p_pa_msg = (icf_msg_st *)icf_port_memget(p_glb_port_data,

                         ((icf_api_header_st*)p_api_header)->api_length ,

                          ICF_MEM_COMMON,p_ecode);



    /* Copy the api header in icf_msg_st header */

    icf_port_memcpy(&(p_pa_msg->hdr),p_api_header,

            ICF_PORT_SIZEOF(icf_api_header_st));





    /* Message has been received on PA message pipe */

    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_PA];

    

    /* Get length for message using peek and getting payload 

     * length from message heaader */ 

    if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,p_pa_msg->payload,

                 ((icf_api_header_st*)p_api_header)->api_length -

                   ICF_PORT_SIZEOF(icf_api_header_st), p_ecode))

    {

        /* Copy the pa message in msg_list */

        p_icf_msg_list->p_msg = p_pa_msg;

        p_icf_msg_list->p_msg_next = ICF_NULL;         

    } 

    else

    {

        ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on PA pipe"));

        ret_val = ICF_FAILURE;

    }    



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_paal_process_incoming_msg\
                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}





/*------------------------------------------------------------------------

 * FUNCTION:            icf_smal_process_incoming_msg()

 * ------------------------------------------------------------------------*/

icf_return_t icf_smal_process_incoming_msg(

                        icf_interface_info_st *p_interface_info,        

                        icf_uint8_t      *p_api_header,

                        icf_msg_list_st  *p_icf_msg_list,

                        icf_error_t      *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_uint32_t   fd_id = ICF_NULL;

    icf_msg_st     *p_sm_msg = ICF_NULL;

    icf_global_port_info_st *p_glb_port_data = 

        (icf_global_port_info_st *)p_interface_info->p_port_info;

    

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_smal_process_incoming_msg"));



    /*Get the memory of size received in api header */

    p_sm_msg = (icf_msg_st *)icf_port_memget(p_glb_port_data,

                         ((icf_api_header_st*)p_api_header)->api_length ,

                          ICF_MEM_COMMON,p_ecode);



    /* Copy the api header in icf_msg_st header */

    icf_port_memcpy(&(p_sm_msg->hdr),p_api_header,

            ICF_PORT_SIZEOF(icf_api_header_st));





    /* Message has been received on SM message pipe */

    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_SM];

    

    /* Get length for message using peek and getting payload 

     * length from message heaader */ 

    if(ICF_SUCCESS == icf_port_fifo_recv(fd_id,p_sm_msg->payload,

                 ((icf_api_header_st*)p_api_header)->api_length -

                   ICF_PORT_SIZEOF(icf_api_header_st), p_ecode))

    {

        /* Copy the pa message in msg_list */

        p_icf_msg_list->p_msg = p_sm_msg;

        p_icf_msg_list->p_msg_next = ICF_NULL;         

    } 

    else

    {

        ICF_ES_TRACE(((icf_uint8_t *)"\nES:Problem in reading data on SM pipe"));

        ret_val = ICF_FAILURE;

    }    



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_smal_process_incoming_msg\
                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}/* icf_smal_process_incoming_msg() ends here. */





#endif /* end of ICF_PORT_WINDOWS */



icf_return_t icf_sipal_process_incoming_msg(

                        icf_interface_info_st *p_interface_info,

                        icf_uint8_t      *p_nw_interface_msg,

                        icf_msg_list_st  *p_icf_msg_list,

                        icf_error_t      *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_msg_st     *p_msg = ICF_NULL;

    icf_nw_interface_info_st *p_nw_msg = ICF_NULL;

    

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_sipal_process_incoming_msg"));



    p_msg = (icf_msg_st*)icf_port_memget(p_interface_info->p_port_info,

                             ICF_PORT_SIZEOF(icf_api_header_st) +

                             ICF_PORT_SIZEOF(icf_nw_interface_info_st),

                             ICF_MEM_COMMON,p_ecode);


	if ( p_msg)
	{
    p_msg->hdr.api_id = ICF_NW_API_MSG_FROM_PEER;

    p_msg->hdr.source_id = ICF_MODULE_SIP_NW; 

    p_msg->hdr.destination_id = ICF_MODULE_ICF;

    p_msg->hdr.call_id = ICF_INVALID_CALL_ID;

    p_msg->hdr.api_length  = ICF_PORT_SIZEOF(icf_api_header_st) +\

          ICF_PORT_SIZEOF(icf_nw_interface_info_st);

                             

    p_nw_msg  = (icf_nw_interface_info_st*)(p_msg->payload);

	}

	if ( p_nw_msg )
	{
    p_nw_msg->bitmap = ICF_INTF_INFO_PROTOCOL_UDP;

    p_nw_msg->pdu_len = (icf_uint16_t)p_interface_info->msg_len;

    p_nw_msg->remote_port_num  = p_interface_info->dest_port;

    icf_port_memcpy(p_nw_msg->remote_ip_addr,

            p_interface_info->dest_ip,

#ifdef ICF_IPV6_ENABLED

			ICF_MAX_IPV6_ADDRESS_LENGTH);

#else

            ICF_MAX_IPV4_ADDRESS_LENGTH);

#endif



    p_nw_msg->receiver_fd = p_interface_info->fd_id;



    p_nw_msg->p_sip_msg = (icf_uint8_t*)icf_port_memget(

                             p_interface_info->p_port_info,

                             p_interface_info->msg_len + 1,

                             ICF_MEM_COMMON,p_ecode);
	if ( p_nw_msg->p_sip_msg )
	{
    icf_port_memcpy(p_nw_msg->p_sip_msg,

            p_nw_interface_msg,p_interface_info->msg_len + 1);
	}

    /*Klocwork warning resolved*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,p_nw_msg->p_sip_msg)
	}

    p_icf_msg_list->p_msg = p_msg;

    p_icf_msg_list->p_msg_next = ICF_NULL;



    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_sipal_process_incoming_msg\
                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}







/*

icf_return_t icf_smal_process_incoming_msg(

                        icf_uint8_t      *p_nw_interface_msg,

                        icf_msg_list_st  *p_icf_msg_list,

                        icf_error_t      *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_paal_process_incoming_msg"));





    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_paal_process_incoming_msg

                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;

}



*/



icf_return_t icf_es_init_complete_handler(

                        icf_void_t      *p_port_data,

                        icf_error_t      *p_ecode)



{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_uint32_t   fd_id = ICF_NULL;

    icf_global_port_info_st *p_glb_port_data = 

        (icf_global_port_info_st *)p_port_data;



    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_es_init_completed"));

    

    /* This function will add PA, RM, RTP pipes to the read fd set and 

     * flush any data received on that fd */

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

    /* ICF will use one socket to listen from PA, RM

								RPT */

    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_PA];

    FD_SET(fd_id, &(p_glb_port_data->ext_info.read_fdset));



    icf_port_flush(fd_id,

           p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_PA],p_ecode);









    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_RM];

    FD_SET(fd_id, &(p_glb_port_data->ext_info.read_fdset));

        

    icf_port_flush(fd_id,

           p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_RM],p_ecode);



    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_MM];

    FD_SET(fd_id, &(p_glb_port_data->ext_info.read_fdset));



    icf_port_flush(fd_id,

           p_glb_port_data->ext_info.msg_read_size[ICF_MODULE_MM],p_ecode);
#else
/*To Remove warnings*/
p_glb_port_data=p_glb_port_data;
fd_id=fd_id;
p_ecode=p_ecode;
#endif 

    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_init_completed\
                with ret_val %d", (icf_int32_t)ret_val));

    return ret_val;    

}


/*******************************************************************************

 * FUNCTION:        icf_es_close_nw_servers

 * DESCRIPTION:   

 *******************************************************************************/

icf_return_t icf_es_close_nw_servers(

        INOUT icf_void_t                 *p_port_data,
        IN    icf_uint16_t               server_index)
{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_global_port_info_st *p_glb_port_data =

        (icf_global_port_info_st *)p_port_data;

    icf_error_t      ecode;

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_es_close_nw_servers"));

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)
    if ( ICF_NULL != 
            p_glb_port_data->ext_info.read_module_fd[ICF_UDP_SIP_SERVER])
    {
        icf_port_close_fd(p_glb_port_data->ext_info.
                read_module_fd[ICF_UDP_SIP_SERVER]);
	
        FD_CLR(p_glb_port_data->ext_info.read_module_fd[ICF_UDP_SIP_SERVER], \
        &(p_glb_port_data->ext_info.read_fdset));

        p_glb_port_data->ext_info.read_module_fd[ICF_UDP_SIP_SERVER]
            = ICF_NULL;

    }

    if ( ICF_NULL != 
            p_glb_port_data->ext_info.read_module_fd[ICF_TCP_SIP_SERVER])
    {
        icf_port_close_fd(p_glb_port_data->ext_info.
                read_module_fd[ICF_TCP_SIP_SERVER]);
	
        FD_CLR(p_glb_port_data->ext_info.read_module_fd[ICF_TCP_SIP_SERVER], \
                &(p_glb_port_data->ext_info.read_fdset));
	
        p_glb_port_data->ext_info.read_module_fd[ICF_TCP_SIP_SERVER]
            = ICF_NULL;
	
        /*icf_port_memfree(p_glb_port_data,
                p_glb_port_data->ext_info.p_tcp_recv_buffer,
                ICF_MEM_COMMON, &ecode);*/
    }

#else
    
    icf_port_close_fd(p_glb_port_data->win_ipc_info.\
         self_fd_nw[server_index].fd_id);

    FD_CLR(p_glb_port_data->win_ipc_info.\
           self_fd_nw[server_index].fd_id, \
           &(p_glb_port_data->win_ipc_info.win_read_fd));

#ifdef ICF_SECURE_SUPPORT
    /* For tls remove the entry from secure dat map while closing the fd so that
     * redundant entry is not available.
     */
    /* SPR 19932 : The following function call removes the secure data
       for the passed FD. After removing the secure data from the map, 
       the FD is set to ICF_INVALID_SOCKFD in the subsequent code leg.

       However the server type is not reset. Thus a preventive check has been
       put wherein, we will attempt to remove the secure data from the map
       only if the server type is ICF_WIN_TLS_SIP_SERVER and the FD is not set
       to invalid.
    */   
    if((ICF_WIN_TLS_SIP_SERVER == 
            p_glb_port_data->win_ipc_info.self_fd_nw[server_index].server_type) &&
       ((icf_uint16_t)ICF_INVALID_SOCKFD != 
        p_glb_port_data->win_ipc_info.self_fd_nw[server_index].fd_id))     

    {
    	#if 1 /*Jason , temporary solution , #883 When transport type is TLS , EGT will crash if user change configures*/
        /* Fix for CSR 1-8414198 : SPR-20354 */
        /* When TLS server socket was opened, only SSL_CTX was created which 
         * is using as input when a new TLS client connection is created.The
         * new TLS client connection gives output as new SSL data which is
         * freed by using the openssl API SSL_free. To free the SSL_CTX data
         * (created When TLS server socket was opened), use SSL_CTX_free API.
         */
        icf_uint32_t              count = 0;
        icf_global_port_info_st *p_glb_port_info =
                             (icf_global_port_info_st *)p_port_data;
        icf_uint32_t              fd_id = 
                p_glb_port_data->win_ipc_info.self_fd_nw[server_index].fd_id;

		for (count = 0; count < ICF_MAX_TLS_CONN; count++)
		{
		      if (fd_id  == p_glb_port_info->ext_info.secure_map[count].fd_id)
		      {
		         icf_port_free_secure_data_ctx(p_glb_port_info->ext_info.secure_map[count].p_secure_data);

		         p_glb_port_info->ext_info.secure_map[count].p_secure_data = ICF_NULL;

		         p_glb_port_info->ext_info.secure_map[count].fd_id = ICF_INVALID_FD;

		         break;

		      }

		}		
	#else	
        icf_es_rem_secure_data_from_map(
                p_port_data,
		        p_glb_port_data->win_ipc_info.self_fd_nw[server_index].fd_id);
	#endif
    }
    /* Changes done for tls end */
#endif
    if ( ICF_WIN_UDP_SIP_SERVER != 
            p_glb_port_data->win_ipc_info.self_fd_nw[server_index].server_type)
    {
        icf_port_memfree(p_glb_port_data,
                p_glb_port_data->ext_info.p_tcp_recv_buffer,
                ICF_MEM_COMMON, &ecode);

        p_glb_port_data->ext_info.p_tcp_recv_buffer = ICF_NULL;
    }

   p_glb_port_data->win_ipc_info.self_fd_nw[server_index].fd_id
            = (icf_uint16_t)ICF_INVALID_SOCKFD;


#endif

    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_close_nw_servers"));

    return ret_val;

}


/*******************************************************************************
 *
 * FUNCTION:        icf_es_open_nw_port
 *
 * DESCRIPTION:    This function will open up a UDP/TCP socket ( as specified)
 *                  by transport_type, and bind to it and add the FD in the li
 *                  istening FDSET maintained in global port info. It will 
 *                  also return the bound fd to calling context through the 
 *                  pointer of the nw interface structure
 *                  Only the Port and transport type info is passed
*******************************************************************************/

icf_return_t icf_es_open_nw_port(
        INOUT icf_void_t                 *p_port_data,
        INOUT icf_nw_interface_info_st   *p_nw_intf_info,
        INOUT icf_int8_t                 *p_self_ip,
        OUT   icf_error_t                *p_ecode)
{
    icf_return_t   ret_val = ICF_SUCCESS;
    icf_uint32_t   fd_id = ICF_NULL;
    icf_uint16_t   *p_cnt_serv_fds = ICF_NULL;
    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_port_data;

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_es_open_nw_port"));

    if(ICF_NULL == p_self_ip)
    {
		return ICF_FAILURE;
	}

    p_cnt_serv_fds = (icf_uint16_t*)&(p_glb_port_data->win_ipc_info.num_of_open_servers);

	/*If max number of servers already opened, return failure*/
    if (ICF_MAX_NUM_OF_SERVERS == *p_cnt_serv_fds)
    {
         ret_val = ICF_FAILURE;
    }


    if (( ICF_INTF_INFO_PROTOCOL_TCP & p_nw_intf_info->bitmap) &&
             ICF_NULL == p_glb_port_data->ext_info.p_tcp_recv_buffer)
    {
        /* Fix for SPR 17583 (ABR in icf_port_tcp_peek)*/
        p_glb_port_data->ext_info.p_tcp_recv_buffer = 
            icf_port_memget(p_glb_port_data,
                ICF_MAX_TCP_MSG_LNGTH + 1, ICF_MEM_COMMON, p_ecode);

        if ( ICF_NULL == p_glb_port_data->ext_info.p_tcp_recv_buffer)
        {
            ICF_ES_TRACE(((icf_uint8_t *)"\nFailure in MemAlloc for static TCP BUFFER"));
            ret_val = ICF_FAILURE;
        }
        if ( ICF_FAILURE != ret_val)
        {
            if ( ICF_FAILURE == icf_port_open_channel( 
                    p_nw_intf_info->remote_port_num,
                    ICF_TRANSPORT_TYPE_TCP, ICF_TRUE, 
                    (icf_uint8_t*)p_self_ip, &fd_id, p_ecode))

            {
                ICF_ES_TRACE(((icf_uint8_t *)"[ES]:Failure in TCP open server\n"));

                icf_port_memfree(p_glb_port_data,
                        p_glb_port_data->ext_info.p_tcp_recv_buffer,
                        ICF_MEM_COMMON, p_ecode);

				ret_val = ICF_FAILURE ;

            }
            else
            {
                /*p_nw_intf_info->receiver_fd = fd_id;*/

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)


                p_glb_port_data->ext_info.
                    read_module_fd[ICF_TCP_SIP_SERVER] = fd_id;

                FD_SET(fd_id, &(p_glb_port_data->ext_info.read_fdset));

#else
                 p_nw_intf_info->server_index[ICF_WIN_TCP_SIP_SERVER] = *p_cnt_serv_fds;
                 if (*p_cnt_serv_fds < (ICF_WIN_TCP_SIP_SERVER + 1))
				 {
				 p_glb_port_data->win_ipc_info.self_fd_nw[*p_cnt_serv_fds].fd_id\
						= fd_id;

                 p_glb_port_data->win_ipc_info.self_fd_nw[(*p_cnt_serv_fds)++].server_type\
                                = ICF_WIN_TCP_SIP_SERVER;
				}
#ifdef ICF_LOAD_DEBUG
                 if(*p_cnt_serv_fds > 3){ 
                     printf("break"); }
#endif

				FD_SET(fd_id,&(p_glb_port_data->win_ipc_info.win_read_fd));

				/* each time u write to win_read_fd, add the fd to the
				 * act_read_fd also. This will be used for clearing the
				 * the FD set. Since FD_CLR is not working */

				p_glb_port_data->win_ipc_info.act_read_fd[

					p_glb_port_data->win_ipc_info.act_fd_count] = 
					fd_id;
				p_glb_port_data->win_ipc_info.act_fd_count += 1;

#endif

            	ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:Successfully opened TCP server for SIP msgs:%d",
					p_nw_intf_info->remote_port_num));
			}

        }

    }

    if ( ICF_INTF_INFO_PROTOCOL_UDP & p_nw_intf_info->bitmap)
    {
        /* if transport type is UDP, ask porting layer to open 
           the port and add port to readfd set */

        if (ICF_FAILURE == icf_port_open_channel( 
                p_nw_intf_info->remote_port_num,
                ICF_TRANSPORT_TYPE_UDP, ICF_TRUE, 
                 (icf_uint8_t *)p_self_ip, &fd_id, p_ecode))
        {
            ICF_ES_TRACE(((icf_uint8_t *)"[ES]:Failure in UDP open server\n"));
            ret_val = ICF_FAILURE;
        }

        else
        {
            /*p_nw_intf_info->receiver_fd = fd_id;*/

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

            p_glb_port_data->ext_info.
                read_module_fd[ICF_UDP_SIP_SERVER] = fd_id;

            FD_SET(fd_id, &(p_glb_port_data->ext_info.read_fdset));

#else

            p_nw_intf_info->server_index[ICF_WIN_UDP_SIP_SERVER] = *p_cnt_serv_fds;
            if (*p_cnt_serv_fds < (ICF_WIN_TCP_SIP_SERVER + 1))
			{
            p_glb_port_data->win_ipc_info.self_fd_nw[*p_cnt_serv_fds].fd_id\
                              = (icf_uint16_t)fd_id;

            p_glb_port_data->win_ipc_info.self_fd_nw[(*p_cnt_serv_fds)++].server_type\
                              = ICF_WIN_UDP_SIP_SERVER;
			}
			FD_SET(fd_id, &(p_glb_port_data->win_ipc_info.win_read_fd));



			/* each time u write to win_read_fd, add the fd to the
			 * act_read_fd also. This will be used for clearing the
			 * the FD set. Since FD_CLR is not working */

			p_glb_port_data->win_ipc_info.act_read_fd[
				p_glb_port_data->win_ipc_info.act_fd_count] = 
				(icf_uint16_t)fd_id;
			p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif

		ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:Successfully opened UDP server for SIP msgs:%d",
			p_nw_intf_info->remote_port_num));

        }

    }

#ifdef ICF_SECURE_SUPPORT

    if ( ICF_INTF_INFO_PROTOCOL_TLS & p_nw_intf_info->bitmap)

    {

	    if ( ICF_NULL == p_glb_port_data->ext_info.p_tcp_recv_buffer)

	    {

		    p_glb_port_data->ext_info.p_tcp_recv_buffer = 

			    icf_port_memget(p_glb_port_data,

					    ICF_MAX_TCP_MSG_LNGTH, ICF_MEM_COMMON, p_ecode);

		    if ( ICF_NULL == p_glb_port_data->ext_info.p_tcp_recv_buffer)

		    {

			    ICF_ES_TRACE(((icf_uint8_t *)"\nFailure in MemAlloc for static TCP BUFFER"));

			    ret_val = ICF_FAILURE;

		    }

	    }

	    if ( ICF_FAILURE != ret_val)

	    {

		    if ( ICF_FAILURE == icf_port_open_channel( 

					    p_nw_intf_info->tls_server_port_num,   
#if 1 /*ZyXEL, Jsaon , SIP over TLS*/
					    ICF_TRANSPORT_TYPE_TLS, ICF_TRUE, 
#else
					    ICF_TRANSPORT_TYPE_TCP, ICF_TRUE, 
#endif
					     (icf_uint8_t *)p_self_ip, &fd_id, p_ecode))

		    {

			    ICF_ES_TRACE(((icf_uint8_t *)"[ES]:Failure in TLS open server\n"));

			    icf_port_memfree(p_glb_port_data,

					    p_glb_port_data->ext_info.p_tcp_recv_buffer,

					    ICF_MEM_COMMON, p_ecode);
                /*Fix for SPR 17691 : 
                *Here,Since there is a failure in icf_port_open_channel and
                *we are calling the icf_port_memfree which is freeing the
                *p_glb_port_data and the value of ret_val is set to
                *ICF_FAILURE .So in the return path if we are again calling the
                *icf_port_memfree(through icf_es_close_nw_servers) for the 
                *failure return by this function,it will give 
                *FUM(Freeing unallocated memmory),and there wiil be
                *a core dump. This particular scenario happens in function 
                *icf_ssa_process_open_nw_servers and in the failure leg of
                *icf_es_open_nw_port we try to close all the open network server
                *based on the check of server_index ,which are alredy being
                *freed here.
                */  
                p_nw_intf_info->server_index[ICF_WIN_TCP_SIP_SERVER]=ICF_MAX_NUM_OF_SERVERS;
			    ret_val = ICF_FAILURE ;

		    }

		    else

		    {
                /*p_nw_intf_info->receiver_fd = fd_id;*/
                /*Klockwork Warning Removal*/
                if (ICF_WIN_TLS_SIP_SERVER >= *p_cnt_serv_fds)
                {
                p_nw_intf_info->server_index[ICF_WIN_TLS_SIP_SERVER] = *p_cnt_serv_fds;
                p_glb_port_data->win_ipc_info.self_fd_nw[*p_cnt_serv_fds].fd_id\
                    = (icf_uint16_t)fd_id;
                p_glb_port_data->win_ipc_info.self_fd_nw[(*p_cnt_serv_fds)++].server_type\
                    = ICF_WIN_TLS_SIP_SERVER;
                }
                else
                { 
                    ICF_ES_TRACE(((icf_uint8_t *)"\nInvalid value while adding the TLS fd")); 
                }
                

			    FD_SET(fd_id,&(p_glb_port_data->win_ipc_info.win_read_fd));

			    /* each time u write to win_read_fd, add the fd to the

			     * act_read_fd also. This will be used for clearing the

			     * the FD set. Since FD_CLR is not working */

			    p_glb_port_data->win_ipc_info.act_read_fd[

				    p_glb_port_data->win_ipc_info.act_fd_count] = 

				   (icf_uint16_t) fd_id;

			    p_glb_port_data->win_ipc_info.act_fd_count += 1;


			    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES_TRACE]:Successfully opened TLS server for SIP msgs:%d",

						    p_nw_intf_info->tls_server_port_num));

			    /* Now add the mapping between the SSL data and the socket fd */

			    if (ICF_FAILURE == icf_es_add_secure_data_to_map(

						    p_nw_intf_info->p_secure_data,p_glb_port_data,fd_id,p_ecode))

			    {

				    ret_val = ICF_FAILURE;

			    }

		    }

	    }

    }

#endif


    ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_open_nw_port\
                 with ret_val %d", (icf_int32_t)ret_val));


    return ret_val;



}

/*******************************************************************************



 *



 * FUNCTION:        icf_es_deinit



 *



 * DESCRIPTION:    This function will close udp/tcp socket/ delete timer list



 *                 and delete task specific memory



*******************************************************************************/



icf_return_t icf_es_deinit(
        INOUT icf_void_t                 *p_port_data,
        OUT   icf_error_t                *p_ecode)
{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_uint32_t   fd_id = ICF_NULL;

    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_port_data;

    icf_uint32_t    i = 0;

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_es_deinit"));

    /* Destroy timer list */

    icf_port_timer_deinit(p_port_data, p_ecode);

    /*Destroy task specific memory */
    icf_mem_deinit(p_port_data, p_ecode);

    /*Close UDP socket */

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

    fd_id = p_glb_port_data->ext_info.read_module_fd[ICF_MODULE_SIP_NW] ;


    icf_port_close_fd(fd_id);

    FD_CLR(fd_id, &(p_glb_port_data->ext_info.read_fdset));
    icf_es_close_nw_servers(p_port_data);

#else
  
    for (i = 0; (i < p_glb_port_data->win_ipc_info.num_of_open_servers) && (i<(ICF_WIN_TCP_SIP_SERVER + 1)) ;i++)
    {
        fd_id = 
        p_glb_port_data->win_ipc_info.self_fd_nw[i].fd_id;

        if (ICF_UDP_SIP_SERVER == 
            p_glb_port_data->win_ipc_info.self_fd_nw[i].server_type)
        {
            icf_port_close_fd(fd_id);
            FD_CLR(fd_id, &(p_glb_port_data->win_ipc_info.win_read_fd));

            FD_ZERO(&(p_glb_port_data->win_ipc_info.win_read_fd));

            memset((void*)(&p_glb_port_data->win_ipc_info.win_read_fd),
                0, FD_SETSIZE);

            FD_CLR(fd_id, &(p_glb_port_data->win_ipc_info.win_write_fd));

            FD_ZERO(&(p_glb_port_data->win_ipc_info.win_write_fd));
            /* Fix for CSR 1-7436195 */
            /* Coverity bug fix */
            memset((void*)(&p_glb_port_data->win_ipc_info.win_write_fd),
                0, ICF_PORT_SIZEOF(fd_set));
        }

        icf_es_close_nw_servers(p_port_data, (icf_uint16_t)i);
     }
#endif

    /* Free p_glb_port_info */
    icf_port_static_memfree(p_glb_port_data,p_ecode);

   ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_deinit \
                with ret_val %d", (icf_int32_t)ret_val));

   return ret_val;

}


/*******************************************************************************

 *

 * FUNCTION:        icf_es_open_tcp_conn

 *

 * DESCRIPTION:    This function will attempt to open UP a TCP stream socket,

 * then attempt to issue connect on it. The socket is opened in non-blocking

 * mode

*******************************************************************************/

icf_return_t icf_es_open_tcp_conn(

        INOUT icf_void_t                 *p_port_data,

        INOUT icf_nw_interface_info_st   *p_nw_intf_info,

        OUT   icf_error_t                *p_ecode)

{

    icf_return_t   ret_val = ICF_SUCCESS;

    icf_global_port_info_st *p_glb_port_data =

        (icf_global_port_info_st *)p_port_data;

    icf_uint16_t   temp_fd_for_comp = (icf_uint16_t)ICF_INVALID_SOCKFD;

#ifdef ICF_IPV6_ENABLED

	icf_uint32_t	dType = 0;

#endif



    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_es_open_tcp_conn"));



    if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == p_glb_port_data->ext_info.num_of_pending_conn + 

            p_glb_port_data->ext_info.num_of_conn)

    {

        ret_val = ICF_FAILURE;

        ICF_ES_TRACE(((icf_uint8_t *)"\nNumOfConnections Maxed!!"));

#ifdef ICF_LOAD_STAT
        printf("\nNumOfConnections Maxed!!");
#endif

    }

    if (temp_fd_for_comp == (icf_uint16_t)p_nw_intf_info->receiver_fd)

    {

        /*The socket is already there, this will be the case when connect

         * is triggered by UATK callback and not by SSA in INVITE/REG processing

         */

#ifdef ICF_IPV6_ENABLED

		if(icf_port_get_family_type(p_nw_intf_info->remote_ip_addr,

			0,&dType) == ICF_FAILURE)

		{

			ret_val = ICF_FAILURE;

			ICF_ES_TRACE(((icf_uint8_t *)"\n Failure in getting family Type"));

		}

		else{

#endif

			if ( ICF_FAILURE == icf_port_open_socket (

#ifdef ICF_IPV6_ENABLED

				dType,

#endif

				ICF_TRANSPORT_TYPE_TCP,

                &(p_nw_intf_info->receiver_fd),

                p_ecode))

			{

			    ret_val = ICF_FAILURE;

			    ICF_ES_TRACE(((icf_uint8_t *)"\n Failure in opening TCP conn socket"));

			}

#ifdef ICF_IPV6_ENABLED

		}

#endif

    }

    /* if SA is establised, we will bind to that self port and use that port 
     * instead of an ephemeral port
     */

if (ICF_INTF_INFO_BLK_CONN == (p_nw_intf_info->bitmap & ICF_INTF_INFO_BLK_CONN))

	{

		if(ICF_FAILURE == icf_port_blk_connect(

					(icf_uint16_t)p_nw_intf_info->receiver_fd,





					p_nw_intf_info->remote_port_num,





					p_nw_intf_info->remote_ip_addr, 



					p_nw_intf_info->self_prot_port,



					p_nw_intf_info->self_ip_addr,   



					p_ecode))

		{

			ret_val = ICF_FAILURE;

		}

	}

	else

	{

		if(ICF_FAILURE == icf_port_nonblk_connect(

					(icf_uint16_t)p_nw_intf_info->receiver_fd,

					p_nw_intf_info->remote_port_num,

					p_nw_intf_info->remote_ip_addr, 

					p_nw_intf_info->self_prot_port,

					p_nw_intf_info->self_ip_addr,   

					p_ecode))

		{

			ret_val = ICF_FAILURE;



			if (ICF_CONNECT_RESP_PENDING == *p_ecode)

			{

				icf_es_add_fd_to_es_db(

						p_glb_port_data->ext_info.pending_connect_fd, 

						p_nw_intf_info->receiver_fd);



				p_glb_port_data->ext_info.num_of_pending_conn++;



#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)




				FD_SET(p_nw_intf_info->receiver_fd, 



						&(p_glb_port_data->ext_info.write_fdset));



#else




				FD_SET(p_nw_intf_info->receiver_fd, 



						&(p_glb_port_data->win_ipc_info.win_write_fd));



#endif





				ICF_ES_TRACE(((icf_uint8_t *)"\nConnRespAwaited, Moved to pending\n"));





			}





			else



			{



				ICF_ES_TRACE(((icf_uint8_t *)"\n Failure in non blocking connect call"));




			}



			/*Comment: Add code to close the FD*/



		}

	}



	if ( ICF_SUCCESS == ret_val)



	{

#ifdef ICF_SECURE_SUPPORT		

		/* 

		 * Now,check whether the protocol is TLS.

		 * In that case,invoke a porting function 

		 * which issues an SSL connect

		 */

		if (ICF_INTF_INFO_PROTOCOL_TLS == (p_nw_intf_info->bitmap & ICF_INTF_INFO_PROTOCOL_TLS))

		{

			icf_void_t *p_secure_data = ICF_NULL;

			p_secure_data = icf_port_secure_connect(

					p_nw_intf_info->p_secure_data,

					p_nw_intf_info->receiver_fd,

					p_ecode);

			if (ICF_NULL == p_secure_data)

			{

				ret_val = ICF_FAILURE;

			}

			else

			{

				/*

				 * Add a mapping between the SSL data and the socket fd in 

				 * the mapping maintained in the ES

				 */

				if (ICF_FAILURE == icf_es_add_secure_data_to_map(

							p_secure_data,p_glb_port_data,

							p_nw_intf_info->receiver_fd,

							p_ecode))

				{

					icf_port_free_secure_data(p_secure_data);

					ret_val = ICF_FAILURE;

				}

			}

		}

#endif		

		if (ICF_SUCCESS == ret_val)

		{

			icf_es_add_fd_to_es_tcp_db(

					p_glb_port_data->ext_info.read_tcp_conn, 

					p_nw_intf_info->receiver_fd,

					(icf_uint8_t)((ICF_INTF_INFO_PROTOCOL_TLS == (p_nw_intf_info->bitmap & ICF_INTF_INFO_PROTOCOL_TLS))

					 ? ICF_TRUE:ICF_FALSE));



			p_glb_port_data->ext_info.num_of_conn++;

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

			FD_SET(p_nw_intf_info->receiver_fd, 

					&(p_glb_port_data->ext_info.read_fdset));



#else

			FD_SET(p_nw_intf_info->receiver_fd, 

					&(p_glb_port_data->win_ipc_info.win_read_fd));

			/* each time u write to win_read_fd, add the fd to the

			 * act_read_fd also. This will be used for clearing the

			 * the FD set. Since FD_CLR is not working */
            /*SPR 19873 The problem was that during TLS SOAK there was memory
             * corruption after 2050 calls, on analysis it was observed that
             global_port_data was getting corrupt due to act_fd_count
             continuously increasing, this was due to below code chunk. Also
             the act_read_fd limit was crossed which is of FD_SETSIZE. The
             below code is not required so commented the same.*/
#if 0            

			p_glb_port_data->win_ipc_info.act_read_fd[

				p_glb_port_data->win_ipc_info.act_fd_count] = 

				(icf_uint16_t)p_nw_intf_info->receiver_fd;

			p_glb_port_data->win_ipc_info.act_fd_count += 1;
#endif             

#endif
		}

	}

	ICF_ES_TRACE(((icf_uint8_t *)"\nExiting function icf_es_open_tcp_conn"));
	return ret_val;
}


icf_return_t   icf_es_rem_fd_from_es_db(

        icf_uint32_t     *p_fd_array,

        icf_uint32_t     fd)

{

    icf_uint32_t      count = 0;

    icf_return_t     ret_val = ICF_SUCCESS;

    for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count ++)

    {

        /*loop to find a vacant entry in the pending conn list*/

        if (fd == p_fd_array[count])

        {

            p_fd_array[count] = ICF_NULL;

            break;

        }

    }

    if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == count)

    {

        ret_val = ICF_FAILURE;

    }

    return ret_val;

}


icf_return_t   icf_es_rem_fd_from_es_tcp_db(

        icf_tcp_data_st     *p_fd_array,

        icf_global_port_info_st *p_port_info,

        icf_uint32_t     fd)

{

    icf_uint32_t      count = 0;

    icf_return_t     ret_val = ICF_SUCCESS;
#ifndef ICF_SECURE_SUPPORT
      p_port_info=p_port_info;
#endif

    for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count ++)

    {

        /*loop to find a vacant entry in the pending conn list*/

        if (fd == p_fd_array[count].fd_id)

        {

#ifdef ICF_SECURE_SUPPORT

		if (ICF_TRUE == p_fd_array[count].is_secure_conn)

		{

			icf_es_rem_secure_data_from_map(

					p_port_info,fd);

		}

#endif

		p_fd_array[count].fd_id = ICF_NULL;

		p_fd_array[count].is_secure_conn = ICF_FALSE;

		break;

        }

    }

    if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == count)

    {
        ret_val = ICF_FAILURE;
    }
    return ret_val;
}


icf_return_t   icf_es_add_fd_to_es_db(

        icf_uint32_t     *p_fd_array,

        icf_uint32_t     fd)

{

    icf_uint32_t      count = 0;

    icf_return_t     ret_val = ICF_SUCCESS;

    for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count ++)

    {

        /*loop to find a vacant entry in the pending conn list*/

        if (ICF_NULL == p_fd_array[count])

        {

            p_fd_array[count] = fd;

            break;

        }

    }

    if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == count)

    {

        ret_val = ICF_FAILURE;

    }

    return ret_val;

}

icf_return_t   icf_es_add_fd_to_es_tcp_db(

        icf_tcp_data_st     *p_fd_array,

        icf_uint32_t        fd,

        icf_boolean_t       is_secure_fd)

{
	
    icf_uint32_t      count = 0;
	
    icf_return_t     ret_val = ICF_SUCCESS;
	
    for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count ++)
		
    {
		/*loop to find a vacant entry in the pending conn list*/
		
        if (ICF_NULL == p_fd_array[count].fd_id)
			
        {
			p_fd_array[count].fd_id = (icf_uint16_t)fd;
			
			p_fd_array[count].is_secure_conn = is_secure_fd;
			
			break;			
		}
		
    }
	
    if (ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS == count)
    {
		ret_val = ICF_FAILURE;
		
    }
    return ret_val;
}


/*******************************************************************************

 * FUNCTION:        icf_es_close_tcp_conn

 *

 * DESCRIPTION:    This function will close a TCP connection, it searches

 * through both the read conn array and pending conn array

*******************************************************************************/

icf_return_t icf_es_close_tcp_conn(

        INOUT icf_void_t                 *p_port_data,

        INOUT icf_nw_interface_info_st   *p_nw_intf_info,

        OUT   icf_error_t                *p_ecode)

{




    icf_return_t   ret_val = ICF_SUCCESS;





    icf_global_port_info_st *p_glb_port_data =





        (icf_global_port_info_st *)p_port_data;





    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:Entering function icf_es_close_tcp_conn\n"));
    p_ecode = p_ecode;



#ifdef ICF_LOAD_DEBUG
    printf("\nES:FD close: %d\n", p_nw_intf_info->receiver_fd);
#endif

    if ( ICF_SUCCESS == icf_es_rem_fd_from_es_tcp_db(

                p_glb_port_data->ext_info.read_tcp_conn,p_glb_port_data,

            p_nw_intf_info->receiver_fd))





    {

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

	    FD_CLR(p_nw_intf_info->receiver_fd,

			    &(p_glb_port_data->ext_info.read_fdset));

#else

	    FD_CLR(p_nw_intf_info->receiver_fd,

			    &(p_glb_port_data->win_ipc_info.win_read_fd));

#endif

	    p_glb_port_data->ext_info.num_of_conn--;


#ifdef ICF_LOAD_DEBUG
            printf("Total TCP connections closed   %d\n", ++g_tcp_closed);
#endif



	    ICF_ES_TRACE(((icf_uint8_t *)"[ES]:Success!!Fd:%d removed from Read DB\n",
				    p_nw_intf_info->receiver_fd ));

	    icf_port_close_fd(p_nw_intf_info->receiver_fd);

    }
    else if (ICF_SUCCESS == icf_es_rem_fd_from_es_db(
                p_glb_port_data->ext_info.pending_connect_fd,

            p_nw_intf_info->receiver_fd))

    {

#if !defined(ICF_PORT_LINUX) && !defined(ICF_PORT_WINDOWS) && !defined(ICF_PORT_VXWORKS)

	    FD_CLR(p_nw_intf_info->receiver_fd,

			    &(p_glb_port_data->ext_info.write_fdset));

#else

	    FD_CLR(p_nw_intf_info->receiver_fd,

			    &(p_glb_port_data->win_ipc_info.win_write_fd));

#endif
	    p_glb_port_data->ext_info.num_of_pending_conn--;

	    ICF_ES_TRACE(((icf_uint8_t *)"[ES]:Success!!Fd:%d removed from Pending DB\n",p_nw_intf_info->receiver_fd ));

#ifdef ICF_LOAD_DEBUG
            printf("Total TCP connections closed   %d\n", ++g_tcp_closed);
#endif

	    icf_port_close_fd(p_nw_intf_info->receiver_fd);
    }

    else

    {

		/* this needs to be done incase the fd to be closed is already removed 

		 * without closing the socket. this will do no harm here.

		 */

		icf_port_close_fd(p_nw_intf_info->receiver_fd);

        ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:FD to be closed already removed frm ES DB\n"));

    }

    ICF_ES_TRACE(((icf_uint8_t *)"\n[ES]:Exiting function icf_es_close_tcp_conn\n"));

    return ret_val;

}


#ifdef ICF_SSA_ES_UT

icf_return_t icf_mmial_process_outgoing_msg(

        IN icf_interface_info_st *p_interface_data, /* interface info */

        IN icf_msg_st *p_icf_out_msg,

        OUT icf_error_t *p_ecode)

{

    p_interface_data = p_interface_data;

    p_icf_out_msg = p_icf_out_msg;

    p_ecode = p_ecode;

}



icf_return_t icf_mmial_mm_process_incoming_msg(

        IN icf_interface_info_st *p_interface_data,

        INOUT icf_interface_msg_st *p_msg,

        OUT icf_msg_list_st *p_icf_msg_list,

        OUT icf_error_t *p_ecode)

{

    p_interface_data = p_interface_data;

    p_msg = p_msg;

    p_icf_msg_list = p_icf_msg_list;

    p_ecode = p_ecode;

}



icf_return_t icf_mmial_rm_process_incoming_msg(

        IN icf_interface_info_st *p_interface_data,

        INOUT icf_interface_msg_st *p_msg,

        OUT icf_msg_list_st *p_icf_msg_list,

        OUT icf_error_t *p_ecode)

{

    p_interface_data = p_interface_data;

    p_msg = p_msg;

    p_icf_msg_list = p_icf_msg_list;

    p_ecode = p_ecode;

}

#endif



/* These are the porting function  to be implemented by the client */

icf_void_t* icf_memget(icf_uint32_t size)

{

	icf_error_t   ecode;

    icf_void_t*    p_buff = icf_cmn_memget(p_persistent_glb_pdb ,size,

        ICF_MEM_COMMON, &ecode);

    if(NULL != p_buff)

    {

        memset(p_buff,0,size);

    }

    return p_buff;

}



icf_void_t icf_memfree(icf_void_t* ptr)

{

    icf_error_t   ecode;

	icf_cmn_memfree(p_persistent_glb_pdb ,ptr, ICF_MEM_COMMON, &ecode);

}



icf_void_t* icf_memcpy(icf_void_t* dst, icf_void_t* src, icf_uint32_t len)

{

    return memcpy(dst,src,len);

    /*icf_port_memcpy ((icf_void_t *)dst,(icf_void_t*)src,len);*/

	/*return dst;*/

}

#ifdef ICF_MM_STUBED_OUT
icf_return_t icf_mm_stub_fill_ext_msg(
        icf_msg_st         *p_org_msg,
        icf_api_id_t       api_id,
        icf_call_id_t      call_id,
        icf_app_id_t       app_id,
        icf_msg_st         **p_p_mm_stub_resp)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_get_codec_list_resp_st          *p_gcl_resp =  ICF_NULL;
	icf_msg_resp_st						*p_dms_resp = ICF_NULL;
    icf_open_media_channel_resp_st		*p_open_channel_resp = ICF_NULL;
    icf_create_media_session_resp_st    *p_cms_resp =  ICF_NULL;    
	icf_codec_commit_resp_st			*p_commit_resp = ICF_NULL;    
    icf_modify_media_session_resp_st	*p_mms_resp =  ICF_NULL;    

    switch(api_id)
    {
        case ICF_GET_CODEC_LIST_REQ: /* Get codec List Success Response with One codec */
        {
            icf_get_codec_list_req_st	*p_gcl_req;
			icf_stream_capab_st        *p_temp_capab;
            icf_list_st			 *p_list1 = ICF_NULL,*p_list2 = ICF_NULL;
            icf_stream_capab_st  *p_stream_capab = ICF_NULL;    
            icf_list_st			 *p_codec_info_list = ICF_NULL;
            icf_codec_attrib_st  *p_codec_info = ICF_NULL;
            icf_boolean_t        first_list = ICF_FALSE,second_list = ICF_FALSE;
            icf_list_st			 *p_temp_list=NULL;
            /* Get codec List Success Response with One Video codec */
            IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + 
                    sizeof(icf_get_codec_list_resp_st));
			(*p_p_mm_stub_resp)->hdr.api_id = ICF_GET_CODEC_LIST_RESP ;  
            (*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
			(*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
            (*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
            (*p_p_mm_stub_resp)->hdr.call_id = call_id;
			(*p_p_mm_stub_resp)->hdr.app_id = app_id;
            (*p_p_mm_stub_resp)->hdr.api_length = 
                sizeof(icf_api_header_st) + sizeof(icf_get_codec_list_resp_st);

            p_gcl_resp = (icf_get_codec_list_resp_st*)((*p_p_mm_stub_resp))->payload;
            p_gcl_req =	(icf_get_codec_list_req_st*)(p_org_msg->payload);
            p_temp_capab = (icf_stream_capab_st*)(p_gcl_req->p_stream_capab_list->p_data);
            p_temp_list = p_gcl_req->p_stream_capab_list->p_next;

            p_gcl_resp->result = ICF_SUCCESS;
            p_gcl_resp->bit_mask = ICF_NULL;

            while(p_temp_capab)
            {
				if (ICF_STREAM_TYPE_VIDEO == p_temp_capab->stream_type)
                {
                    IMSC_MM_MEMGET((p_stream_capab),sizeof(icf_stream_capab_st));
                    
                    p_stream_capab->bit_mask = ICF_NULL;
                    p_stream_capab->bit_mask |=
                        ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
                    
                    p_stream_capab->stream_id = p_temp_capab->stream_id;
                    p_stream_capab->stream_type = ICF_STREAM_TYPE_VIDEO;
                    
                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 0;
                    strcpy(p_codec_info->codec_name.str, "PCMU");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);

                    p_codec_info->codec_params.video_param.clock_rate = 90000;
                    p_codec_info->codec_params.video_param.bit_mask = 0;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT;
                    p_codec_info->codec_params.video_param.frame_size.height = 176;
                    p_codec_info->codec_params.video_param.frame_size.width = 144;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_FRAMESIZE_PRESENT;
 
                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;

                    p_stream_capab->p_codec_info_list = p_codec_info_list;

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_stream_capab;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_stream_capab;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }
                    p_stream_capab = ICF_NULL;
                    p_codec_info_list = ICF_NULL;
                    p_codec_info = ICF_NULL;
                    
                }
                else if(ICF_STREAM_TYPE_AUDIO == p_temp_capab->stream_type)
                {
                    /* Fill up for video stream */
                    IMSC_MM_MEMGET((p_stream_capab),sizeof(icf_stream_capab_st));
                    
                    p_stream_capab->bit_mask = ICF_NULL;
                    p_stream_capab->bit_mask |=
                        ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
                    
                    p_stream_capab->stream_id = p_temp_capab->stream_id;
                    p_stream_capab->stream_type = ICF_STREAM_TYPE_VIDEO;
                    
                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 8;
                    strcpy(p_codec_info->codec_name.str, "PCMA");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);

                    p_codec_info->codec_params.audio_param.channels = 0;
                    p_codec_info->codec_params.audio_param.bit_mask  = 0;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_CHANNEL;
                    p_codec_info->codec_params.audio_param.sampling_rate = 10;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE;
 
                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;

                    p_stream_capab->p_codec_info_list = p_codec_info_list;

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_stream_capab;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_stream_capab;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }
                }
				if ( ICF_NULL == p_temp_list)
				{
					p_temp_capab = ICF_NULL;
				}
				else
				{
					p_temp_capab = (icf_stream_capab_st*)(p_gcl_req->p_stream_capab_list->p_next->p_data);
					p_temp_list = p_temp_list->p_next;
				}   
                p_stream_capab = ICF_NULL;
                p_codec_info_list = ICF_NULL;
                p_codec_info = ICF_NULL;
			}
			p_gcl_resp->p_reserved_stream_list = p_list1;
        }
		break;
        case ICF_DELETE_MEDIA_SESSION_REQ: 
        {
            /* Delete Media Session Success Response */
            IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + sizeof(icf_msg_resp_st));
            (*p_p_mm_stub_resp)->hdr.api_id = ICF_DELETE_MEDIA_SESSION_RESP ;  
            (*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
            (*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
            (*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
            (*p_p_mm_stub_resp)->hdr.call_id = call_id;
            (*p_p_mm_stub_resp)->hdr.app_id = app_id;
            (*p_p_mm_stub_resp)->hdr.api_length = 
            sizeof(icf_api_header_st) + sizeof(icf_msg_resp_st);

            p_dms_resp = (icf_msg_resp_st*)((*p_p_mm_stub_resp))->payload;
            p_dms_resp->result = ICF_SUCCESS;
        }
        break;
        case ICF_OPEN_MEDIA_CHANNEL_REQ:
        {
            icf_open_media_channel_req_st *p_open_media_req= ICF_NULL;
            icf_channel_st *p_temp_channel=ICF_NULL;
            icf_channel_addr_st *p_channel_addr = ICF_NULL;
            icf_boolean_t        first_list = ICF_FALSE,second_list = ICF_FALSE;
            icf_list_st			 *p_temp_list = ICF_NULL;
            icf_list_st			 *p_list1 = ICF_NULL,*p_list2 = ICF_NULL;
            /* Open Channel Success Response */
            IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + 
                    sizeof(icf_open_media_channel_resp_st));

            (*p_p_mm_stub_resp)->hdr.api_id = ICF_OPEN_MEDIA_CHANNEL_RESP ;  
            (*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
            (*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
            (*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
            (*p_p_mm_stub_resp)->hdr.call_id = call_id;
            (*p_p_mm_stub_resp)->hdr.app_id = app_id;
            (*p_p_mm_stub_resp)->hdr.api_length = 
                sizeof(icf_api_header_st) + sizeof(icf_open_media_channel_resp_st);

            p_open_media_req =	(icf_open_media_channel_req_st*)(p_org_msg->payload);
            p_temp_channel = (icf_channel_st*)(p_open_media_req->p_channel_list->p_data);
            p_temp_list = p_open_media_req->p_channel_list->p_next;

            p_open_channel_resp = (icf_open_media_channel_resp_st*)
                ((*p_p_mm_stub_resp))->payload;
            p_open_channel_resp->result = ICF_SUCCESS;

            while(p_temp_channel)
            {
                if(ICF_STREAM_TYPE_VIDEO == p_temp_channel->stream_type)
                {
                    IMSC_MM_MEMGET((p_channel_addr),sizeof(icf_channel_addr_st));
                    p_channel_addr->stream_id = p_temp_channel->stream_id;
                    p_channel_addr->stream_type = ICF_STREAM_TYPE_VIDEO;
                    p_channel_addr->stream_address.port_num = 1024;
                    p_channel_addr->stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_4 = 1;

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_channel_addr;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_channel_addr;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_channel_addr = ICF_NULL;
                }
                else if(ICF_STREAM_TYPE_AUDIO == p_temp_channel->stream_type)
                {
                    IMSC_MM_MEMGET((p_channel_addr),sizeof(icf_channel_addr_st));
                    p_channel_addr->stream_id = p_temp_channel->stream_id;
                    p_channel_addr->stream_type = ICF_STREAM_TYPE_AUDIO;
                    p_channel_addr->stream_address.port_num = 1024;
                    p_channel_addr->stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_channel_addr->stream_address.addr.addr.ipv4_addr.octet_4 = 1;

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_channel_addr;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_channel_addr;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_channel_addr = ICF_NULL;                    
                }
                        
                if ( ICF_NULL == p_temp_list)
                {
                    p_temp_channel = ICF_NULL;
                }
                else
                {
                    p_temp_channel = (icf_channel_st*)(p_open_media_req->p_channel_list->p_next->p_data);
                    p_temp_list = p_temp_list->p_next;
                }
            }
            p_open_channel_resp->p_channel_addr_list = p_list1;
        }
		break;
        case ICF_CREATE_MEDIA_SESSION_REQ: 
		{
            icf_req_stream_st			*p_temp_stream =NULL;
            icf_boolean_t        first_list = ICF_FALSE,second_list = ICF_FALSE;
            icf_list_st			 *p_temp_list = ICF_NULL;
            icf_list_st			 *p_list1 = ICF_NULL,*p_list2 = ICF_NULL;     
            icf_resp_stream_st   *p_resp_stream = ICF_NULL;
            icf_list_st			 *p_codec_info_list = ICF_NULL;
            icf_codec_attrib_st  *p_codec_info = ICF_NULL;
            
            IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + 
                    sizeof(icf_create_media_session_resp_st));
            (*p_p_mm_stub_resp)->hdr.api_id = ICF_CREATE_MEDIA_SESSION_RESP ;  
			(*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
            (*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
            (*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
            (*p_p_mm_stub_resp)->hdr.call_id = call_id;
            (*p_p_mm_stub_resp)->hdr.app_id = app_id;
            (*p_p_mm_stub_resp)->hdr.api_length = 
                sizeof(icf_api_header_st) + sizeof(icf_create_media_session_resp_st);
        
            p_cms_resp = (icf_create_media_session_resp_st*)((*p_p_mm_stub_resp))->payload;
            p_cms_resp->result = ICF_SUCCESS;
            p_cms_resp->bit_mask = ICF_NULL;

			p_temp_stream = (icf_req_stream_st*)(((icf_create_media_session_req_st*)
                        (p_org_msg->payload))->p_stream_list_data->p_data);
            p_temp_list = ((icf_create_media_session_req_st*)
                    (p_org_msg->payload))->p_stream_list_data->p_next;

            while(p_temp_stream)
			{				
                if (ICF_STREAM_TYPE_VIDEO == p_temp_stream->stream_type)
                {
                    /* VIDEO stream*/
                    IMSC_MM_MEMGET((p_resp_stream),sizeof(icf_resp_stream_st));
                    p_resp_stream->stream_id = p_temp_stream->stream_id;
                    p_resp_stream->stream_type = p_temp_stream->stream_type;

                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 0;
                    strcpy(p_codec_info->codec_name.str, "PCMU");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);

                    p_codec_info->codec_params.video_param.clock_rate = 90000;
                    p_codec_info->codec_params.video_param.bit_mask = 0;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT;
                    p_codec_info->codec_params.video_param.frame_size.height = 176;
                    p_codec_info->codec_params.video_param.frame_size.width = 144;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_FRAMESIZE_PRESENT;
                    
                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;     

                    p_resp_stream->local_sdp.bit_mask = ICF_NULL;
                    p_resp_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;                    
                    p_resp_stream->local_sdp.p_codec_info_list = p_codec_info_list;			
                    p_resp_stream->local_sdp.stream_address.port_num = 1027;
                    p_resp_stream->local_sdp.stream_address.addr.addr_type = 
                        ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4 = 1;                    

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_resp_stream;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_resp_stream;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_resp_stream = ICF_NULL;
                }
                else if(ICF_STREAM_TYPE_AUDIO == p_temp_stream->stream_type)
				{
                    IMSC_MM_MEMGET((p_resp_stream),sizeof(icf_resp_stream_st));
                    p_resp_stream->stream_id = p_temp_stream->stream_id;
                    p_resp_stream->stream_type = p_temp_stream->stream_type;

                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 8;
                    strcpy(p_codec_info->codec_name.str, "PCMA");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);

                    p_codec_info->codec_params.audio_param.bit_mask = 0;
                    p_codec_info->codec_params.audio_param.channels = 0;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_CHANNEL;
                    p_codec_info->codec_params.audio_param.sampling_rate = 10;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE;

                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;     

                    p_resp_stream->local_sdp.bit_mask = ICF_NULL;
                    p_resp_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;                    
                    p_resp_stream->local_sdp.p_codec_info_list = p_codec_info_list;			
                    p_resp_stream->local_sdp.stream_address.port_num = 1027;
                    p_resp_stream->local_sdp.stream_address.addr.addr_type = 
                        ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4 = 1;                    
                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_resp_stream;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_resp_stream;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_resp_stream = ICF_NULL;
                }
                if (ICF_NULL == p_temp_list)
				{
                    p_temp_stream = ICF_NULL;
                }
                else
                {
                    p_temp_stream = (icf_req_stream_st*)(((icf_create_media_session_req_st*)
                                (p_org_msg->payload))->p_stream_list_data->p_next->p_data);
                    p_temp_list = p_temp_list->p_next;
                }
            }

            p_cms_resp->p_accepted_stream_list = p_list1;
        }
        break;
        case ICF_CODEC_COMMIT_REQ: 
        {
            icf_codec_commit_req_st      *p_codec_commit_req = ICF_NULL;
            icf_stream_capab_st          *p_temp_capab = ICF_NULL;
            icf_boolean_t                first_list = ICF_FALSE,
                                         second_list = ICF_FALSE;
            icf_list_st			         *p_temp_list = ICF_NULL;
            icf_list_st			         *p_list1 = ICF_NULL,
                                         *p_list2 = ICF_NULL;  
            icf_stream_commit_st         *p_stream_commit = ICF_NULL;
			
            IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + 
                    sizeof(icf_codec_commit_resp_st));
			(*p_p_mm_stub_resp)->hdr.api_id = ICF_CODEC_COMMIT_RESP ;  
            (*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
            (*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
            (*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
			(*p_p_mm_stub_resp)->hdr.call_id = call_id;
            (*p_p_mm_stub_resp)->hdr.app_id = app_id;
            (*p_p_mm_stub_resp)->hdr.api_length = 
                sizeof(icf_api_header_st) + sizeof(icf_codec_commit_resp_st);

			p_commit_resp = (icf_codec_commit_resp_st*)((*p_p_mm_stub_resp))->payload;
            p_commit_resp->result = ICF_SUCCESS;

			p_commit_resp->bit_mask = ICF_NULL;

			p_codec_commit_req =	(icf_codec_commit_req_st*)(p_org_msg->payload);
			p_temp_capab = (icf_stream_capab_st*)(p_codec_commit_req->p_stream_commit_list->p_data);
			p_temp_list = p_codec_commit_req->p_stream_commit_list->p_next;

			while(p_temp_capab)
			{
				if (ICF_STREAM_TYPE_VIDEO == p_temp_capab->stream_type)
				{
					/* Fill up for video stream */
                    IMSC_MM_MEMGET((p_stream_commit),sizeof(icf_stream_commit_st));
					p_stream_commit->stream_id = p_temp_capab->stream_id;
					p_stream_commit->codec_count = 1;
					p_stream_commit->codec[0].bit_mask = ICF_NULL;
					p_stream_commit->codec[0].bit_mask |= ICF_CODEC_ID_CODEC_NAME_PRESENT;
					strcpy(p_stream_commit->codec[0].codec_name.str,"PCMU");
					p_stream_commit->codec[0].codec_name.str_len = 
					strlen(p_stream_commit->codec[0].codec_name.str);

					p_stream_commit->codec[0].bit_mask |= ICF_CODEC_ID_CODEC_ID_PRESENT;
					p_stream_commit->codec[0].codec_id = 0;
                    
                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_stream_commit;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_stream_commit;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_stream_commit = ICF_NULL;
  
				}
				else if(ICF_STREAM_TYPE_AUDIO == p_temp_capab->stream_type)
				{
                    IMSC_MM_MEMGET((p_stream_commit),sizeof(icf_stream_commit_st));
                    p_stream_commit->stream_id = p_temp_capab->stream_id;
                    p_stream_commit->codec_count = 1;
                    p_stream_commit->codec[0].bit_mask = ICF_NULL;
                    p_stream_commit->codec[0].bit_mask |= ICF_CODEC_ID_CODEC_NAME_PRESENT;
                    strcpy(p_stream_commit->codec[0].codec_name.str,"PCMA");
                    p_stream_commit->codec[0].codec_name.str_len = 
                        strlen(p_stream_commit->codec[0].codec_name.str);

                    p_stream_commit->codec[0].bit_mask |= ICF_CODEC_ID_CODEC_ID_PRESENT;
                    p_stream_commit->codec[0].codec_id = 8;

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_stream_commit;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_stream_commit;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_stream_commit = ICF_NULL;
                }
                if (ICF_NULL == p_temp_list)
                {
					p_temp_capab = ICF_NULL;
				}
                else
                {
                    p_temp_capab = (icf_stream_capab_st*)
                        (p_codec_commit_req->p_stream_commit_list->p_next->p_data);
                    p_temp_list = p_temp_list->p_next;
                }
            }	
            p_commit_resp->p_commited_stream_list = p_list1;
        }
        break;
        
        case ICF_MODIFY_MEDIA_SESSION_REQ:
        {
            icf_modify_stream_st	*p_temp_stream = ICF_NULL;
            icf_boolean_t        first_list = ICF_FALSE,second_list = ICF_FALSE;
            icf_list_st			 *p_temp_list = ICF_NULL;
            icf_list_st			 *p_list1 = ICF_NULL,*p_list2 = ICF_NULL;     
            icf_resp_stream_st   *p_resp_stream = ICF_NULL;
            icf_list_st			 *p_codec_info_list = ICF_NULL;
            icf_codec_attrib_st  *p_codec_info = ICF_NULL;            
            
			IMSC_MM_MEMGET((*p_p_mm_stub_resp),sizeof(icf_api_header_st) + 
                    sizeof(icf_modify_media_session_resp_st));
			(*p_p_mm_stub_resp)->hdr.api_id = ICF_MODIFY_MEDIA_SESSION_RESP ;  
			(*p_p_mm_stub_resp)->hdr.version = ICF_VERSION_1_0;
			(*p_p_mm_stub_resp)->hdr.source_id = ICF_MODULE_MM;
			(*p_p_mm_stub_resp)->hdr.destination_id = ICF_MODULE_ICF;
			(*p_p_mm_stub_resp)->hdr.call_id = call_id;
			(*p_p_mm_stub_resp)->hdr.app_id = app_id;
			(*p_p_mm_stub_resp)->hdr.api_length = sizeof(icf_api_header_st) + 
                sizeof(icf_modify_media_session_resp_st);
        
			p_mms_resp = (icf_modify_media_session_resp_st*)((*p_p_mm_stub_resp))->payload;
			p_mms_resp->result = ICF_SUCCESS;

			p_mms_resp->bit_mask = ICF_NULL;

			p_temp_stream = (icf_modify_stream_st*)(((icf_modify_media_session_req_st*)
									(p_org_msg->payload))->p_stream_data_list->p_data);
			p_temp_list = ((icf_modify_media_session_req_st*)
									(p_org_msg->payload))->p_stream_data_list->p_next;

			while(p_temp_stream)
			{				
				if (ICF_STREAM_TYPE_VIDEO == p_temp_stream->stream_type)
				{
                    /* VIDEO stream*/
                    IMSC_MM_MEMGET((p_resp_stream),sizeof(icf_resp_stream_st));
                    p_resp_stream->stream_id = p_temp_stream->stream_id;
                    p_resp_stream->stream_type = p_temp_stream->stream_type;

                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 0;
                    strcpy(p_codec_info->codec_name.str, "PCMU");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);

                    p_codec_info->codec_params.video_param.bit_mask = 0;
                    p_codec_info->codec_params.video_param.clock_rate = 90000;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT;
                    p_codec_info->codec_params.video_param.frame_size.height = 176;
                    p_codec_info->codec_params.video_param.frame_size.width = 144;
                    p_codec_info->codec_params.video_param.bit_mask |= 
                        ICF_VIDEO_CODEC_FRAMESIZE_PRESENT;
                    
                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;     

                    p_resp_stream->local_sdp.bit_mask = ICF_NULL;
                    p_resp_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;                    
                    p_resp_stream->local_sdp.p_codec_info_list = p_codec_info_list;			
                    p_resp_stream->local_sdp.stream_address.port_num = 1027;
                    p_resp_stream->local_sdp.stream_address.addr.addr_type = 
                        ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4 = 1;                    

                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_resp_stream;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_resp_stream;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_resp_stream = ICF_NULL;
				}
				else if (ICF_STREAM_TYPE_AUDIO == p_temp_stream->stream_type)
				{
                    IMSC_MM_MEMGET((p_resp_stream),sizeof(icf_resp_stream_st));
                    p_resp_stream->stream_id = p_temp_stream->stream_id;
                    p_resp_stream->stream_type = p_temp_stream->stream_type;

                    IMSC_MM_MEMGET((p_codec_info_list),sizeof(icf_list_st));

                    IMSC_MM_MEMGET((p_codec_info),sizeof(icf_codec_attrib_st)); 

                    p_codec_info->bit_mask = ICF_NULL;
                    p_codec_info->bit_mask |=
                        ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT | ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                    
                    p_codec_info->codec_num = 8;
                    strcpy(p_codec_info->codec_name.str, "PCMA");
                    p_codec_info->codec_name.str_len = 
                        strlen(p_codec_info->codec_name.str);
 
                    p_codec_info->codec_params.audio_param.bit_mask = 0;
                    p_codec_info->codec_params.audio_param.channels = 0;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_CHANNEL;
                    p_codec_info->codec_params.audio_param.sampling_rate = 10;
                    p_codec_info->codec_params.audio_param.bit_mask |= 
                        ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE;

                    p_codec_info_list->p_data = p_codec_info;
                    p_codec_info_list->p_next = ICF_NULL;     

                    p_resp_stream->local_sdp.bit_mask = ICF_NULL;
                    p_resp_stream->local_sdp.trans_mode = ICF_TRANSPORT_MODE_RTP_AVP;                    
                    p_resp_stream->local_sdp.p_codec_info_list = p_codec_info_list;			
                    p_resp_stream->local_sdp.stream_address.port_num = 1027;
                    p_resp_stream->local_sdp.stream_address.addr.addr_type = 
                        ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1 = 127; 
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3 = 0;     
                    p_resp_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4 = 1;                    
                    if (ICF_FALSE == first_list)
                    {
                        IMSC_MM_MEMGET((p_list1),sizeof(icf_list_st));
                        p_list1->p_data = p_resp_stream;
                        p_list1->p_next = ICF_NULL;
                        first_list = ICF_TRUE;
                    }
                    else
                    {
                        IMSC_MM_MEMGET((p_list2),sizeof(icf_list_st));
                        p_list2->p_data = p_resp_stream;
                        p_list1->p_next = p_list2;
                        p_list2->p_next = ICF_NULL;
                        second_list = ICF_TRUE;
                    }                    
                    p_resp_stream = ICF_NULL;
				}
				if (ICF_NULL == p_temp_list)
				{
					p_temp_stream = ICF_NULL;
				}
				else
				{
					p_temp_stream = (icf_modify_stream_st*)(((icf_modify_media_session_req_st*)
										(p_org_msg->payload))->p_stream_data_list->p_next->p_data);
					p_temp_list = p_temp_list->p_next;
				}
			}			
			p_mms_resp->p_accepted_stream_list = p_list1;
		}
		break;
        
        default:
        {
            break;
        }
    } 
    return ret_val;    
} 

icf_return_t icf_mm_stub_process_n_append_to_msg_list(
        icf_msg_st        *p_msg_data)
{
    icf_msg_st           *p_msg = ICF_NULL;  
    icf_msg_list_st      *p_temp_msg_lst = ICF_NULL;
    icf_error_t          ecode;
    icf_glb_pdb_st       *p_glb_pdb = p_persistent_glb_pdb;

    icf_mm_stub_fill_ext_msg(p_msg_data,
            p_msg_data->hdr.api_id,
            p_msg_data->hdr.call_id,
            p_msg_data->hdr.app_id,
            &p_msg);

    p_temp_msg_lst = p_glb_pdb->p_msg_lst;

    /*  Traverse the link list till last node */

    while (ICF_NULL != p_temp_msg_lst->p_msg_next)
    {        
        p_temp_msg_lst = p_temp_msg_lst->p_msg_next;
    }

    /*If already there are messages in the list*/
    if(ICF_NULL != p_temp_msg_lst->p_msg)
    {
        /*Create a new node*/
        p_temp_msg_lst->p_msg_next = (icf_msg_list_st *)
            icf_cmn_memget(
                    p_glb_pdb,
                    (icf_uint32_t const)sizeof(icf_msg_list_st),
                    (icf_pool_id_t const)ICF_MEM_COMMON, &ecode); 

        /*If there's a memory allocation failure*/
        if (ICF_NULL == p_temp_msg_lst->p_msg_next) 
        { 
             free(p_msg);
             p_msg = ICF_NULL; 
        }/*End If*/
        else
        {
            p_temp_msg_lst->p_msg_next->p_msg = p_msg;
            p_temp_msg_lst->p_msg_next->p_msg_next = ICF_NULL;
        }

    }/*The message is the first message in the list*/
    else
    {
        p_temp_msg_lst->p_msg = p_msg;
    }
    return ICF_SUCCESS;
}

#endif

#ifndef ICF_SSA_ES_UT
/*******************************************************************************

 *

 * FUNCTION:        icf_send_to_app

 *

 * DESCRIPTION:     This function take message from NW and give them to 

 *                  corresponding AL's so that they can convert data to 

 *                  Format of external entity

 *

*******************************************************************************/

icf_return_t icf_send_to_app(icf_glb_pdb_st*	p_glb_pdb,

				icf_msg_st*	p_msg_data,

				icf_void_st*	p_routing_info,

				icf_error_t*	p_ecode

)

{

    icf_return_t   ret_val = ICF_SUCCESS;



	icf_uint8_t*			p_encoded_buf = ICF_NULL;

    icf_uint32_t			encoded_buf_length = ICF_NULL;

	icf_route_info_st*	p_route_info;

	icf_uint16_t			dest_port = 0;

	icf_string_st		dest_ip;

	icf_uint8_t      		*p_dest_ip = ICF_NULL;

	icf_uint16_t			fd_id;

    icf_global_port_info_st*	p_glb_port_data;

	icf_string_st channel_path;   //ZyXEL porting

    ICF_ES_TRACE(((icf_uint8_t *)"\nEntering function icf_send"));


    /* SPR 19969: Added NULL check for p_routing_info->ptr as in the case
     * of reinitialise / application remove requests
     * the memory allocated for ptr field is freed whereas
     * routing info is a static field in application information structure
     * and its address is passed to ES for sending messages to app. 
     * Effectively, this can never be NULL. 
     */ 
    if ((ICF_NULL == p_routing_info) || 
            (ICF_NULL == (icf_route_info_st*)p_routing_info->ptr))
    {
        return ICF_FAILURE;
    }   	

	p_dest_ip = dest_ip.str;

	*p_dest_ip = '\0';

    icf_print_msg_hdr(p_msg_data);







	p_route_info = (icf_route_info_st*)p_routing_info->ptr;



	if(ICF_IPC_UDP == p_route_info->type)

	{

		dest_port = p_route_info->ipc.udp.port;

		

		switch(p_route_info->ipc.udp.ip.addr_type)

		{

		

 		case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:

			icf_cmn_convert_transport_addr_to_ip_string(

				&(((*p_route_info).ipc).udp).ip,

				p_dest_ip);

			break;

		case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:

			icf_cmn_convert_transport_addr_to_ip_string(

				&(((*p_route_info).ipc).udp).ip,

				p_dest_ip);

			break;

#ifdef ICF_IPV6_ENABLED

		case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:

			icf_cmn_convert_transport_addr_to_ip_string(

				&(((*p_route_info).ipc).udp).ip,

				p_dest_ip);

			break;

#endif

		}

		

	}

	p_glb_port_data = (icf_global_port_info_st*)p_glb_pdb->p_glb_data->p_glb_port_info;

	fd_id = p_glb_port_data->win_ipc_info.self_fd_ipc;



	if (ICF_FAILURE == 

		icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, p_ecode, ICF_NULL)) 

	{
       /*Klocwork warning resolved*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,p_encoded_buf)

		printf("ICF clib:Message encoding failure for PA\n");

	}

	else

	{
/*ZyXEL porting*/
#if 0 
		if(icf_port_udp_send(fd_id,

			p_encoded_buf, encoded_buf_length,

			dest_port,

			p_dest_ip,

			p_ecode )==ICF_SUCCESS)
#else
		memset(&channel_path, 0, sizeof(channel_path));
		strncpy((char *)(&channel_path.str[0]), VOICEAPP_IPTK_CHANNEL_PATH, sizeof(VOICEAPP_IPTK_CHANNEL_PATH));
		channel_path.str_len = sizeof(VOICEAPP_IPTK_CHANNEL_PATH);
		if(icf_port_ipc_send(p_glb_port_data->win_ipc_info.self_fd_ipc,
			p_encoded_buf,
			encoded_buf_length,
			p_ecode,
			channel_path
			)==ICF_SUCCESS)
#endif
/*ZyXEL porting end*/
		{
#ifndef    ICF_LOAD_STAT
			ICF_PRINT(((icf_uint8_t*)"Send to PA successfully \n"));
#endif
		}

		else

		{
			printf("Unable to send to the PA \n");

		}

		icf_free_encoded_msg(p_encoded_buf, p_ecode);

	}

    return ret_val;



}
#endif







/*------------------------------------------------------------------------------

 * FUNCTION:    icf_es_cbk_recv_from_pltfm

 *

 * DESCRIPTION: This function is the callback invoked by wrapper whenver it 

 *				receives a response or unsolicited indication from Port.

 *

 * RETURNS:     ICF_SUCCESS or ICF_FAILURE

 *              

 *----------------------------------------------------------------------------*/

icf_return_t icf_es_cbk_recv_from_pltfm(

		INOUT	icf_msg_st	*p_msg)

{

	icf_return_t					 ret_val = ICF_SUCCESS;

    icf_msg_st                       *p_mrm_msg = ICF_NULL;

    icf_error_t                      ecode = ICF_ERROR_NONE;


    if (ICF_NULL == p_msg)

    {

        printf("[ES]: Null message received from Wrapper, returning failure");

        return ICF_FAILURE;

    }

    /* Klocwork warning removal.*/
    if (ICF_NULL == p_persistent_glb_pdb)
    {
       return ICF_FAILURE;
    }


    /* --------------------------------------------- */

    /* Create the external message to be sent to MRM */

    

    p_mrm_msg = (icf_void_t *)icf_cmn_memget(

                                p_persistent_glb_pdb,

		                        p_msg->hdr.api_length,

		                        ICF_MEM_COMMON,

                                &ecode);

    if (ICF_NULL == p_mrm_msg)   
    {
       ICF_PRINT(((icf_uint8_t*)"\nicf_cmn_memget failed.\n"))
       return ICF_FAILURE;
    }  

    /* external message header */

    p_mrm_msg->hdr.api_id = p_msg->hdr.api_id;  

    p_mrm_msg->hdr.version = ICF_VERSION_1_0;

    p_mrm_msg->hdr.source_id = ICF_MODULE_PLATFORM;

    p_mrm_msg->hdr.destination_id = ICF_MODULE_ICF;

    p_mrm_msg->hdr.call_id = ICF_INVALID_CALL_ID;

    p_mrm_msg->hdr.app_id = ICF_APP_ID_INVALID;

    p_mrm_msg->hdr.api_length = p_msg->hdr.api_length;

    

	/* external message payload */

    icf_port_memcpy(

        (icf_void_t *)(p_mrm_msg->payload),

        (icf_void_t *)(p_msg->payload),

        p_msg->hdr.api_length-sizeof(icf_api_header_st));





    /* ---------------------------------------------- */

    /* Add the message at the end of MRM message list */

    /* ---------------------------------------------- */

    if (ICF_FAILURE == icf_es_append_to_msg_list(

                                    p_mrm_msg,

                                    p_persistent_glb_pdb,

                                    &ecode))

    {

        printf("[ES]: Failed to append new Platform msg to MRM msg list");

        ret_val = ICF_FAILURE;

    }



	return ret_val;

} /* End function icf_es_cbk_recv_from_pltfm() */





/*******************************************************************************

 *

 * FUNCTION:        icf_es_append_to_msg_list

 *

 * DESCRIPTION:     This is a utility function that appends an external message 

 *                  at the end of MRM message processing list.

 *

 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure

 *

 ******************************************************************************/

icf_return_t icf_es_append_to_msg_list(

        INOUT      icf_msg_st           *p_msg,

        INOUT   icf_glb_pdb_st       *p_glb_pdb,

        OUT     icf_error_t          *p_ecode)

{

    icf_return_t                     ret_val = ICF_SUCCESS;

    icf_msg_list_st                  *p_temp_msg_lst = ICF_NULL;

    

    p_temp_msg_lst = p_glb_pdb->p_msg_lst;

    

    /*  Traverse the link list till last node */

    while (ICF_NULL != p_temp_msg_lst->p_msg_next)

    {        

        p_temp_msg_lst = p_temp_msg_lst->p_msg_next;

    }

    /* If already there are messages in the list */

    if (ICF_NULL != p_temp_msg_lst->p_msg)

    {

        /* Create a new node */

        p_temp_msg_lst->p_msg_next = (icf_msg_list_st *)

        icf_cmn_memget(

            p_glb_pdb,

            (icf_uint32_t const)sizeof(icf_msg_list_st),

            (icf_pool_id_t const)ICF_MEM_COMMON,

            p_ecode); 

        

        /* If there's a memory allocation failure */

        if (ICF_NULL == p_temp_msg_lst->p_msg_next) 

        { 

             free(p_msg);

             p_msg = ICF_NULL; 

        }/* End If */

        else

        {

            p_temp_msg_lst->p_msg_next->p_msg = p_msg;

            p_temp_msg_lst->p_msg_next->p_msg_next = ICF_NULL;

        }

    }/* The message is the first message in the list */

    else

    {

        p_temp_msg_lst->p_msg = p_msg;

    }



    if(ICF_NULL != p_msg)

    {

        icf_print_msg_hdr(p_msg);

    }



    return ret_val;

} /* End function icf_es_append_to_msg_list() */



/*******************************************************************************

 *

 * FUNCTION:        icf_es_port_close_sa_servers 

 *

 * DESCRIPTION:     This function is invoked from SSA to close the servers openned

 *                  on the protected ports

 *

 * RETURNS:         ICF_SUCCESS

 *

 ******************************************************************************/

icf_return_t icf_es_port_close_sa_servers (

    INOUT icf_void_t     *p_port_data)

{ 

    /* local variables */

    icf_global_port_info_st *p_glb_port_data =

        (icf_global_port_info_st *)p_port_data;

    icf_uint8_t      count = 0;
    /*Fix For SPR: 19531 (Klocwork Warning Removal)*/
    icf_uint32_t     fd_id = 0;

    fd_set			tmp_read_fdset;



    ICF_PRINT(((icf_uint8_t*)"\nES: Entered Function: \
        icf_es_port_close_sa_servers"));



#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)

    if ( ICF_NULL != 

        p_glb_port_data->win_ipc_info.curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER])

    {

        fd_id = p_glb_port_data->win_ipc_info.

            curr_act_serv_fd[ICF_WIN_UDP_SA_SERVER] ;



        FD_CLR(fd_id, \

            &(p_glb_port_data->win_ipc_info.win_read_fd));

    }

    else

    if ( ICF_NULL != 

        p_glb_port_data->win_ipc_info.curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER])

    {

        fd_id = p_glb_port_data->win_ipc_info.

            curr_act_serv_fd[ICF_WIN_TCP_SA_SERVER] ;



        FD_CLR(fd_id, \

            &(p_glb_port_data->win_ipc_info.win_read_fd));

    }



    icf_port_close_fd(fd_id);

    /* need to remove the hole from the win_read_fd set */

    /* rearrange the read FD set so that it has no holes */

    /* make a new fd_set without holes and assign this new fd_set to the

    * old read_fd */

    FD_ZERO(&tmp_read_fdset);
    /* Fix for CSR 1-7436195 */
    /* Coverity bug fix */
    memset(&tmp_read_fdset, 0, ICF_PORT_SIZEOF(fd_set));

    for (count=0;count<p_glb_port_data->win_ipc_info.act_fd_count;count++)

    { 

        if (p_glb_port_data->win_ipc_info.act_read_fd[count] == 

            (icf_uint16_t)fd_id)

        {

            if (count == (p_glb_port_data->win_ipc_info.act_fd_count -1))

            {

                p_glb_port_data->win_ipc_info.act_read_fd[count] = 0;

            }

            while (count <p_glb_port_data->win_ipc_info.act_fd_count-1)

            {

                p_glb_port_data->win_ipc_info.act_read_fd[count] = 

                    p_glb_port_data->win_ipc_info.act_read_fd[count+1] ;

                FD_SET(p_glb_port_data->win_ipc_info.act_read_fd[count],

                    &(tmp_read_fdset));

                count ++;

            }

            p_glb_port_data->win_ipc_info.act_fd_count -=1 ;

            break;

        }

        else

        {

            FD_SET(p_glb_port_data->win_ipc_info.act_read_fd[count],

                &(tmp_read_fdset));

        }

    } /* for loop ends */

    p_glb_port_data->win_ipc_info.win_read_fd = tmp_read_fdset;

#endif

    ICF_PRINT(((icf_uint8_t*)"\nES: SA server closed successfully"));



    return ICF_SUCCESS;



}





#if 0 /* defined in icf_port_util.c */

icf_return_t icf_port_pltfm_send(void * p_msg)

{



   icf_return_t		        ret_val = ICF_SUCCESS;

   icf_error_t			ecode = ICF_NULL;

  /* Send to GPRS network */

   icf_cmn_send(p_persistent_glb_pdb,

				(icf_msg_st*)p_msg,

				&ecode);



   return ICF_SUCCESS;

}

#endif

#ifdef ICF_SECURE_SUPPORT

icf_return_t icf_es_add_secure_data_to_map(
        icf_void_t *p_secure_data,
        icf_void_t *p_port_info,
        icf_uint32_t fd_id,
        icf_error_t *p_ecode)

{
    icf_uint32_t              count = 0;
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_port_info;
  

    for (count = 0; count < ICF_MAX_TLS_CONN; count++)
    {
        if ( ICF_INVALID_FD  == p_glb_port_data->ext_info.secure_map[count].fd_id)         
        {
            /*
             * Empty entry found, let's populate it now
             */
            p_glb_port_data->ext_info.secure_map[count].fd_id = fd_id;
            p_glb_port_data->ext_info.secure_map[count].p_secure_data = p_secure_data;
            break;
        }
    }
    if ( ICF_MAX_TLS_CONN == count)
    {
        /*Set Error Code*/
        ret_val = ICF_FAILURE;
    }
	p_ecode = p_ecode;
    return ret_val;
}

icf_void_t* icf_es_fetch_secure_data_from_map(

        icf_void_t *p_port_info,

        icf_uint32_t fd_id,

        icf_error_t *p_ecode)

{

    icf_uint32_t              count = 0;

    icf_global_port_info_st *p_glb_port_data =

        (icf_global_port_info_st *)p_port_info;

    

    for (count = 0; count < ICF_MAX_TLS_CONN; count++)

    {

        if (fd_id  == p_glb_port_data->ext_info.secure_map[count].fd_id)

        {

            return p_glb_port_data->ext_info.secure_map[count].p_secure_data;

        }

    }

	p_ecode = p_ecode;

    return ICF_NULL;

}

icf_void_t icf_es_rem_secure_data_from_map(

        icf_void_t *p_port_info,

        icf_uint32_t fd_id)

{

    icf_uint32_t              count = 0;

    icf_global_port_info_st *p_glb_port_data =

        (icf_global_port_info_st *)p_port_info;

    

    for (count = 0; count < ICF_MAX_TLS_CONN; count++)

    {

        if (fd_id  == p_glb_port_data->ext_info.secure_map[count].fd_id)

	{

		icf_port_free_secure_data(p_glb_port_data->ext_info.secure_map[count].p_secure_data);

		p_glb_port_data->ext_info.secure_map[count].p_secure_data = ICF_NULL;

		p_glb_port_data->ext_info.secure_map[count].fd_id = ICF_INVALID_FD;

		break;

	}

    }

}

#endif

#ifndef ICF_SSA_ES_UT
/*******************************************************************************
 *
 * FUNCTION:        icf_send_to_app_for_logs
 *
 * DESCRIPTION:     This function is used for sending LOGS to the application.
 *
*******************************************************************************/
icf_return_t icf_send_to_app_for_logs(icf_glb_pdb_st*	p_glb_pdb,
				icf_msg_st*	    p_msg_data,
				icf_void_st*	p_routing_info,
				icf_error_t*	p_ecode)
{
    icf_return_t				ret_val            = ICF_SUCCESS;
    icf_route_info_st           *p_route_info      = ICF_NULL;
    icf_uint8_t                 *p_encoded_buf     = ICF_NULL;
   	icf_uint8_t      		    *p_dest_ip         = ICF_NULL;
    icf_global_port_info_st     *p_glb_port_data   = ICF_NULL;
    icf_uint16_t			    dest_port;
    icf_string_st		        dest_ip;
    icf_uint16_t			    fd_id;
    icf_uint32_t			    encoded_buf_length = ICF_NULL;


    p_dest_ip = dest_ip.str;
	*p_dest_ip = '\0';

    p_route_info = (icf_route_info_st*)p_routing_info->ptr;
    dest_port = p_route_info->ipc.udp.port;
    switch(p_route_info->ipc.udp.ip.addr_type)
    {
        
    case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
        icf_cmn_convert_transport_addr_to_ip_string(
            &(((*p_route_info).ipc).udp).ip,
            p_dest_ip);
        break;
        
    case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
        icf_cmn_convert_transport_addr_to_ip_string(
            &(((*p_route_info).ipc).udp).ip,
            p_dest_ip);
        break;
        
#ifdef ICF_IPV6_ENABLED
    case ICF_TRANSPORT_ADDRESS_IPV6_ADDR:
        icf_cmn_convert_transport_addr_to_ip_string(
            &(((*p_route_info).ipc).udp).ip,
            p_dest_ip);
        break;
#endif
        
    }
    
    p_glb_port_data = (icf_global_port_info_st*)p_glb_pdb->p_glb_data->p_glb_port_info;
    fd_id = p_glb_port_data->win_ipc_info.self_fd_ipc;
    
    if (ICF_FAILURE == 
        icf_encode_msg(p_msg_data, &p_encoded_buf, &encoded_buf_length, p_ecode, ICF_NULL)) 
    {
        /*Klocwork warning resolved*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_persistent_glb_pdb,p_encoded_buf)
        /*printf("ICF clib:Message encoding failure for PA\n");*/
        ret_val = ICF_FAILURE;
    }
    else
    {
        if(icf_port_udp_send(fd_id,
            p_encoded_buf, encoded_buf_length,
            dest_port,
            p_dest_ip,
            p_ecode )==ICF_SUCCESS)
        {
            /*fflush(stdout);
            printf("Send to PA successfully \n");*/
        }
        else
        {
            /*printf("Unable to send to the PA \n");*/
        }
        icf_free_encoded_msg(p_encoded_buf, p_ecode);
    }

    return ret_val;
}
#endif

#ifdef ICF_NAT_MEDIA_TRAVERSAL

/******************************************************************************

 * SOCKET HANDLLING FUNCTIIONS TO BE REGISTERED WITH GLB DATA OF NAT LIBRARY

 ******************************************************************************/
/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_open_udp_socket 
 * Description :  function to open a UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_open_udp_socket( 
        nat_ip_port_address_st *p_local_addr)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_uint8_t ip_str[50];
    icf_uint32_t    fd_id;
    icf_error_t ecode = ICF_ERROR_NONE;
    icf_transport_address_st trn_addr;

    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_persistent_glb_pdb->p_glb_data->p_glb_port_info;


    icf_port_sprintf((icf_int8_t *)ip_str,"%d.%d.%d.%d",p_local_addr->ipv4_addr.octet_1,
            p_local_addr->ipv4_addr.octet_2,
            p_local_addr->ipv4_addr.octet_3,
            p_local_addr->ipv4_addr.octet_4);

    trn_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;


    if(icf_port_open_channel(p_local_addr->port,
                ICF_TRANSPORT_TYPE_UDP,ICF_TRUE,
                ip_str, &fd_id,&ecode)==NAT_RETURN_FAILURE)

    {
        printf("the open channel faied \n");
        return NAT_RETURN_FAILURE;
    }
    else
    {
        icf_uint8_t count = 0;

        for(count = 0;count <ICF_MAX_NAT_CONTEXT; count++)
        {
            if(ICF_TRUE == 
                    p_glb_port_data->win_ipc_info.nat_ipc_info[count].is_free)
            {
                p_glb_port_data->win_ipc_info.nat_ipc_info[count].is_free = 
                    ICF_FALSE;

                p_glb_port_data->win_ipc_info.nat_ipc_info[count].binding_fd = 
                    fd_id;

                trn_addr.addr.addr.ipv4_addr.octet_1 = 
                    p_local_addr->ipv4_addr.octet_1;
                trn_addr.addr.addr.ipv4_addr.octet_2 = 
                    p_local_addr->ipv4_addr.octet_2;
                trn_addr.addr.addr.ipv4_addr.octet_3 = 
                    p_local_addr->ipv4_addr.octet_3;
                trn_addr.addr.addr.ipv4_addr.octet_4 = 
                                    p_local_addr->ipv4_addr.octet_4;
                trn_addr.port_num = p_local_addr->port;
                
                icf_port_memcpy(
                    &(p_glb_port_data->win_ipc_info.
                        nat_ipc_info[count].local_addr),
                    &trn_addr,ICF_PORT_SIZEOF(icf_transport_address_st));

                p_glb_port_data->win_ipc_info.nat_ipc_count ++;

                FD_SET(fd_id, &(p_glb_port_data->win_ipc_info.win_read_fd));

                break;
            }
        }
    }


    return ret_val;
}

/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_close_udp_socket 
 * Description :  function to close a UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_close_udp_socket( 
                                 nat_ip_port_address_st *p_local_addr)
{

    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_persistent_glb_pdb->p_glb_data->p_glb_port_info;    
    icf_uint8_t count = 0;
    icf_boolean_t match_found = ICF_FALSE;
    icf_error_t ecode = ICF_ERROR_NONE;
    icf_uint32_t fd = 0;
    icf_transport_address_st trn_addr;

    trn_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
    trn_addr.port_num = p_local_addr->port;
    trn_addr.addr.addr.ipv4_addr.octet_1 = 
                                    p_local_addr->ipv4_addr.octet_1;
    trn_addr.addr.addr.ipv4_addr.octet_2 = 
                        p_local_addr->ipv4_addr.octet_2;
    trn_addr.addr.addr.ipv4_addr.octet_3 = 
                        p_local_addr->ipv4_addr.octet_3;
    trn_addr.addr.addr.ipv4_addr.octet_4 = 
                        p_local_addr->ipv4_addr.octet_4;

    for(count = 1; count < ICF_MAX_NAT_CONTEXT;count++)
    {
         icf_uint32_t       rem_ip_addr = 0;
         icf_uint32_t        com_ip_addr = 0;
         icf_transport_address_st local_trn_addr = 
             p_glb_port_data->win_ipc_info.nat_ipc_info[count].local_addr;


        /* Check for port: port number is not zero and remote port is not same
         * as committed port */
        if(trn_addr.port_num !=  local_trn_addr.port_num)
        {
	        match_found = ICF_FALSE;
        }
        else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == trn_addr.addr.addr_type)
        {
	        rem_ip_addr = ICF_MAKE_32BIT_INT(
		        trn_addr.addr.addr.ipv4_addr.octet_1,
		        trn_addr.addr.addr.ipv4_addr.octet_2,
		        trn_addr.addr.addr.ipv4_addr.octet_3,
                trn_addr.addr.addr.ipv4_addr.octet_4);

	        com_ip_addr = ICF_MAKE_32BIT_INT(
		        local_trn_addr.addr.addr.ipv4_addr.octet_1,
		        local_trn_addr.addr.addr.ipv4_addr.octet_2,
		        local_trn_addr.addr.addr.ipv4_addr.octet_3,
		        local_trn_addr.addr.addr.ipv4_addr.octet_4);

	        if(rem_ip_addr != com_ip_addr)
	        {
	            match_found = ICF_FALSE;
	        }
        }

        if(ICF_TRUE == match_found)
        {
            fd = p_glb_port_data->win_ipc_info.nat_ipc_info[count].binding_fd;
            p_glb_port_data->win_ipc_info.nat_ipc_info[count].is_free = ICF_TRUE;
            p_glb_port_data->win_ipc_info.nat_ipc_count --;
            FD_CLR(fd,
                    &(p_glb_port_data->win_ipc_info.win_read_fd));
            icf_port_close_socket(fd,&ecode);
            break;
        }
    }
    
    if(count == ICF_MAX_NAT_CONTEXT)
    {
        ret_val = NAT_RETURN_FAILURE;
    }


    return ret_val;
}


/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_udp_data 
 * Description :  function to send data on UDP socket
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_udp_data(
                              nat_ip_port_address_st *p_local_addr,
                              nat_ip_port_address_st *p_dest_addr,
                              nat_void_t *p_msg_data)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    icf_global_port_info_st *p_glb_port_data =
        (icf_global_port_info_st *)p_persistent_glb_pdb->p_glb_data->p_glb_port_info;    
    icf_uint8_t count = 0;
    icf_boolean_t match_found = ICF_FALSE;
    icf_error_t ecode = ICF_ERROR_NONE;
    icf_uint32_t fd = 0;
    icf_transport_address_st trn_addr;

    trn_addr.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
    trn_addr.port_num = p_local_addr->port;
    trn_addr.addr.addr.ipv4_addr.octet_1 = 
                                    p_local_addr->ipv4_addr.octet_1;
    trn_addr.addr.addr.ipv4_addr.octet_2 = 
                        p_local_addr->ipv4_addr.octet_2;
    trn_addr.addr.addr.ipv4_addr.octet_3 = 
                        p_local_addr->ipv4_addr.octet_3;
    trn_addr.addr.addr.ipv4_addr.octet_4 = 
                        p_local_addr->ipv4_addr.octet_4;

    for(count = 1; count < ICF_MAX_NAT_CONTEXT;count++)
    {
         icf_uint32_t       rem_ip_addr = 0;
         icf_uint32_t        com_ip_addr = 0;
         icf_transport_address_st local_trn_addr = 
             p_glb_port_data->win_ipc_info.nat_ipc_info[count].local_addr;


        /* Check for port: port number is not zero and remote port is not same
         * as committed port */
        if(trn_addr.port_num !=  local_trn_addr.port_num)
        {
	        match_found = ICF_FALSE;
        }
        else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == trn_addr.addr.addr_type)
        {
	        rem_ip_addr = ICF_MAKE_32BIT_INT(
		        trn_addr.addr.addr.ipv4_addr.octet_1,
		        trn_addr.addr.addr.ipv4_addr.octet_2,
		        trn_addr.addr.addr.ipv4_addr.octet_3,
                trn_addr.addr.addr.ipv4_addr.octet_4);

	        com_ip_addr = ICF_MAKE_32BIT_INT(
		        local_trn_addr.addr.addr.ipv4_addr.octet_1,
		        local_trn_addr.addr.addr.ipv4_addr.octet_2,
		        local_trn_addr.addr.addr.ipv4_addr.octet_3,
		        local_trn_addr.addr.addr.ipv4_addr.octet_4);

	        if(rem_ip_addr != com_ip_addr)
	        {
	            match_found = ICF_FALSE;
	        }
        }

        if(ICF_TRUE == match_found)
        {
            icf_uint8_t ip_str[50];

            icf_port_sprintf((icf_int8_t *)ip_str,"%d.%d.%d.%d",p_dest_addr->ipv4_addr.octet_1,
                                            p_dest_addr->ipv4_addr.octet_2,
                                            p_dest_addr->ipv4_addr.octet_3,
                                            p_dest_addr->ipv4_addr.octet_4);

            fd = p_glb_port_data->win_ipc_info.nat_ipc_info[count].binding_fd;

            if(icf_port_udp_send(fd,p_msg_data, ICF_PORT_SIZEOF(p_msg_data),
					p_dest_addr->port,ip_str, /* destination ip address */
					&ecode )==ICF_SUCCESS)
			{
				printf("Send to STUN SERVER successfully \n");
			}
			else
			{
				printf("Unable to send to the STUN SERVER \n");
			}
            break;
        }
    }
    
    if(count == ICF_MAX_NAT_CONTEXT)
    {
        ret_val = NAT_RETURN_FAILURE;
    }


    return ret_val;
}


/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_app_data 
 * Description :  function to send data to APP
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_app_data( 
                              nat_void_t *p_msg_data)
{

    icf_return_t		    ret_val = ICF_SUCCESS;
    icf_msg_st              *p_mrm_msg = ICF_NULL;
    icf_error_t             ecode = ICF_ERROR_NONE;
    nat_msg_st              *p_nat_msg = (nat_msg_st *)p_msg_data;
    icf_uint16_t            api_id = ICF_API_ID_INVALID;
    nat_msg_st              *p_payload = ICF_NULL;

    if (ICF_NULL == p_persistent_glb_pdb || ICF_NULL == p_msg_data)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[ES]: Null message received from NAT Lib, returning failure\n"));
        return ICF_FAILURE;
    }

    /* --------------------------------------------- */
    /* Create the external message to be sent to MRM */

    p_mrm_msg = (icf_msg_st *)icf_cmn_memget(
               p_persistent_glb_pdb,
	           p_nat_msg->msg_hdr.api_length + 
                     ICF_PORT_SIZEOF(icf_api_header_st),
	           ICF_MEM_COMMON,
               &ecode);

    if (ICF_NULL == p_mrm_msg)
    {
        ICF_PRINT(((icf_uint8_t *)"\n[ES]: p_mrm_msg memory allocation failure \n"));
        return ICF_FAILURE;
    }    

    /* external message header */

    switch(p_nat_msg->msg_hdr.api_id)
    {
        case NAT_STUN_INIT_RESP:
            api_id = ICF_NAT_STUN_INIT_RESP;
            break;
        case NAT_GET_PUBLIC_IP_PORT_RESP:
            api_id = ICF_NAT_GET_PUBLIC_IP_PORT_RESP;
            break;
        case NAT_ERROR_IND:
            api_id = ICF_NAT_ERROR_IND;
            break;
        case NAT_STUN_EXEC_PROC_RESP:
            api_id = ICF_NAT_STUN_EXEC_PROC_RESP;
            break;
        case NAT_STUN_PROC_STATUS_IND:
            api_id = ICF_NAT_STUN_PROC_STATUS_IND;
            break;
        default:
            ICF_PRINT(((icf_uint8_t*)"\nInvalid API received from NAT Client.\n"));

    }
    p_mrm_msg->hdr.api_id = api_id; 
    p_mrm_msg->hdr.version = ICF_VERSION_1_0;
    p_mrm_msg->hdr.source_id = ICF_MODULE_NAT;
    p_mrm_msg->hdr.destination_id = ICF_MODULE_ICF;
    p_mrm_msg->hdr.call_id = 
        icf_cmn_fetch_call_id_from_nat_context_id(p_nat_msg->msg_hdr.app_context_id);
    p_mrm_msg->hdr.app_id = ICF_APP_ID_INVALID;
    p_mrm_msg->hdr.api_length = p_nat_msg->msg_hdr.api_length + 
                                ICF_PORT_SIZEOF(icf_api_header_st);
   

	/* external message payload */

    p_payload = (nat_msg_st *)p_mrm_msg->payload;

    icf_port_memcpy(
        (icf_void_t *)(p_payload),
        (icf_void_t *)(p_nat_msg),
        p_nat_msg->msg_hdr.api_length);

    /* ---------------------------------------------- */
    /* Add the message at the end of MRM message list */
    /* ---------------------------------------------- */

    if (ICF_FAILURE == icf_es_append_to_msg_list(
                                    p_mrm_msg,
                                    p_persistent_glb_pdb,
                                    &ecode))

    {
        ICF_PRINT(((icf_uint8_t *)"\n[ES]: Failed to append new Platform msg to MRM msg list. \n"));
		ICF_MEMFREE(p_persistent_glb_pdb, \
                    p_mrm_msg, ICF_MEM_COMMON, &ecode, ret_val)
        ret_val = ICF_FAILURE;
    }

    return ret_val;
}


/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_send_mm_data 
 * Description :  function to send data to APP
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_send_mm_data(nat_ip_port_address_st *p_local_addr,
							 nat_ip_port_address_st * p_dest_addr,
                              nat_void_t *p_msg_data)
{
    icf_return_t		        ret_val = ICF_SUCCESS;
    icf_msg_st                  *p_mrm_msg = ICF_NULL;
    icf_error_t                 ecode = ICF_ERROR_NONE;
    nat_msg_st                  *p_nat_msg = (nat_msg_st *)p_msg_data;
    icf_send_udp_data_req_st    *p_send_udp_data_req =ICF_NULL;
    nat_octet_string_st    *p_msg_payload =
        ((nat_nw_msg_data_st *)p_nat_msg->p_msg_data)->p_buffer_data;

    if (ICF_NULL == p_persistent_glb_pdb)
    {
        return ICF_FAILURE;
    }

    if (ICF_NULL == p_msg_data)
    {
        printf("[ES]: Null message received from NAT Lib, returning failure");
        return ICF_FAILURE;
    }

    p_mrm_msg = (icf_void_t *)icf_cmn_memget(
                                p_persistent_glb_pdb,
								p_msg_payload->octet_len +
                                ICF_PORT_SIZEOF(icf_send_udp_data_req_st) +
                                + ICF_PORT_SIZEOF(icf_api_header_st),
		                        ICF_MEM_COMMON,
                                &ecode);

    if (ICF_NULL == p_mrm_msg)
    {
       ICF_PRINT(((icf_uint8_t*)"\nicf_common_memget fails.\n"))
       return ICF_FAILURE;
    }
        
     p_send_udp_data_req = (icf_send_udp_data_req_st *)p_mrm_msg->payload;

     p_send_udp_data_req->p_app_data = 
         icf_port_static_memget(p_msg_payload->octet_len + 1,&ecode);

	if (ICF_NULL == p_send_udp_data_req->p_app_data)
    {
       ICF_PRINT(((icf_uint8_t*)"\nicf_port_static_memget fails.\n"))
	   ICF_MEMFREE(p_persistent_glb_pdb,
                    p_mrm_msg,
                    ICF_MEM_COMMON,
                    &ecode,
                    ret_val)	   
       return ICF_FAILURE;
    } 
      /* --------------------------------------------- */
    /* Create the external message to be sent to MRM */
    /* --------------------------------------------- */
     icf_port_memcpy((icf_void_t *)p_send_udp_data_req->p_app_data,
                        (icf_void_t *)p_msg_payload->str,
                        p_msg_payload->octet_len);

    p_send_udp_data_req->app_data_len = p_msg_payload->octet_len;
    p_send_udp_data_req->source_port = p_local_addr->port;
    p_send_udp_data_req->dest_addr.port_num = p_dest_addr->port;
    p_send_udp_data_req->dest_addr.addr.addr_type = 
                                ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
    icf_port_memcpy(&p_send_udp_data_req->dest_addr.addr.addr.ipv4_addr, 
                &p_dest_addr->ipv4_addr,
                ICF_PORT_SIZEOF(p_dest_addr->ipv4_addr));

    /* external message header */

    p_mrm_msg->hdr.api_id = ICF_SEND_UDP_DATA_REQ;  
    p_mrm_msg->hdr.version = ICF_VERSION_1_0;
    p_mrm_msg->hdr.source_id = ICF_MODULE_NAT;
    p_mrm_msg->hdr.destination_id = ICF_MODULE_MM;
    p_mrm_msg->hdr.call_id = 
        icf_cmn_fetch_call_id_from_nat_context_id(p_nat_msg->msg_hdr.app_context_id);
    p_mrm_msg->hdr.app_id = ICF_APP_ID_INVALID;
    p_mrm_msg->hdr.api_length = p_msg_payload->octet_len +
                                ICF_PORT_SIZEOF(icf_send_udp_data_req_st) +
                                + ICF_PORT_SIZEOF(icf_api_header_st);
        /*ICF_PORT_SIZEOF(p_send_udp_data_req) + 
                                ICF_PORT_SIZEOF(icf_api_header_st);
        */
    
    /* ---------------------------------------------- */
    /* Add the message at the end of MRM message list */
    /* ---------------------------------------------- */

    if (ICF_FAILURE == icf_es_append_to_msg_list(
                                    p_mrm_msg,
                                    p_persistent_glb_pdb,
                                    &ecode))

    {
        ICF_PRINT(((icf_uint8_t*)"[ES]: Failed to append new Platform msg to MRM msg list"));

		icf_port_static_memfree(p_send_udp_data_req->p_app_data, &ecode);
		ICF_MEMFREE(p_persistent_glb_pdb,
                    p_mrm_msg,
                    ICF_MEM_COMMON,
                    &ecode,
                    ret_val)

        ret_val = ICF_FAILURE;
    }

    return ret_val;

}


/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_start_timer 
 * Description :  function pointer to start a timer
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_start_timer(nat_void_t *p_timer_data_buf, 
                                         nat_duration_t  timer_dur,
                                         nat_timer_id_t *p_timer_id)
{
    icf_return_t      ret_val = NAT_RETURN_SUCCESS;
    icf_error_t       ecode = ICF_ERROR_NONE;
    nat_timer_id_t    timer_id = NAT_TIMER_STATUS_INVALID;
    icf_timer_data_st *p_timer_data = ICF_NULL;

    /* Get a timer block from DBM, this will be freed on clearing call contxt */
    ret_val = icf_dbm_get_mem_from_pool(p_persistent_glb_pdb,
            (icf_pool_id_t)ICF_MEM_TIMER,
            (icf_void_t **)&p_timer_data, &ecode);
    ICF_ASSERT(p_persistent_glb_pdb, (ICF_NULL != p_timer_data))

    if(ICF_SUCCESS == ret_val)
    {
        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_NAT_TIMER;
        p_timer_data->module_id = ICF_MODULE_NAT;
        p_timer_data->p_timer_buffer = p_timer_data_buf;
        p_timer_data->p_glb_pdb = p_persistent_glb_pdb;
    }

    if(ICF_SUCCESS == ret_val)
    {
        /* Now, start the Setup timer */
        ICF_START_TIMER(p_persistent_glb_pdb, p_timer_data, timer_id,
                timer_dur, &ecode, ret_val)
    }

    if(ret_val == NAT_RETURN_SUCCESS)
    {
        icf_uint16_t i = 0;
        for(i = 0; i < NAT_MAX_TIMER_INFO_BLOCKS;i ++)
        {
            if(NAT_TIMER_STATUS_INVALID ==
                    p_nat_glb_pdb->timer_info[i].timer_status)
            {
                p_nat_glb_pdb->timer_info[i].timer_status =
                        NAT_TIMER_STATUS_VALID;
                p_nat_glb_pdb->timer_info[i].timer_id = timer_id;
                p_nat_glb_pdb->timer_info[i].p_timer_data = p_timer_data;
                *p_timer_id = timer_id;
                break;
            }
        }
    }
    return ret_val;
}


/*------------------------------------------------------------------------------
 *
 * Type Name   :  es_stop_timer 
 * Description :  function pointer to stop a timer
 *
 *----------------------------------------------------------------------------*/
nat_return_t es_stop_timer(nat_timer_id_t timer_id)
{
    icf_return_t ret_val = NAT_RETURN_SUCCESS;
    icf_error_t  ecode = ICF_ERROR_NONE;
    icf_uint16_t i = 0;

    ICF_STOP_TIMER(p_persistent_glb_pdb, timer_id, &ecode, ret_val)

    for(i = 0; i < NAT_MAX_TIMER_INFO_BLOCKS;i ++)
    {
        if(NAT_TIMER_STATUS_VALID ==
                p_nat_glb_pdb->timer_info[i].timer_status)
        {
            if(p_nat_glb_pdb->timer_info[i].timer_id == timer_id)
            {
                break;
            }
        }
    }

	/* SPR 19927: In case the for loop is complete and timer data is
	 * not found then return from here itself.
     */	 
	if(NAT_MAX_TIMER_INFO_BLOCKS == i)
	{
		return NAT_RETURN_FAILURE;
	}
	
    /* free the memory allocated to any existing timer data block */
    if (ICF_NULL != p_nat_glb_pdb->timer_info[i].p_timer_data)
    {
        icf_dbm_ret_mem_to_pool(p_persistent_glb_pdb,
                (icf_pool_id_t)ICF_MEM_TIMER,
                 p_nat_glb_pdb->timer_info[i].p_timer_data, &ecode);

        p_nat_glb_pdb->timer_info[i].p_timer_data = ICF_NULL;
        p_nat_glb_pdb->timer_info[i].timer_status =
                        NAT_TIMER_STATUS_INVALID;
        p_nat_glb_pdb->timer_info[i].timer_id = NAT_INVALID_TIMER_ID;
    }
    return ret_val;
}


#endif


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_print_api_from_api_id 
 * Description :  identify the API and calls the appropriate print handler
 *
 *----------------------------------------------------------------------------*/

#ifdef ICF_INTERFACE_TRACE_ENABLE


void icf_print_api_from_api_id(icf_api_id_t api_id,icf_msg_st* p_msg)
{
	icf_return_t value;
    /* In tunnel mode, we will not print interface traces for
       DMS request and response.
    */
ICF_CHECK_TUNNEL_MODE_START(p_persistent_glb_pdb,ICF_TRUE) 
    if((ICF_DELETE_MEDIA_SESSION_REQ == api_id) ||
       (ICF_DELETE_MEDIA_SESSION_RESP == api_id))
    {
        return;
    }
ICF_CHECK_TUNNEL_MODE_END

	ICF_PRINT_MESSAGE(("\n{\n"));
	switch(api_id)
	{
		#ifdef ICF_DEBUG_MEMPOOL
			case ICF_MEM_POOL_STATS_PRINT: 
			value=icf_print_api_header(p_msg);
			break;		
		#endif
		case ICF_CFG_CC_SET_SCALE_PARAMS_REQ:
			value=icf_print_set_scale_params(p_msg);
			break;
		case ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ:
			value=icf_print_set_trans_params(p_msg);
			break;
		case ICF_CFG_CC_SET_CALL_PARAMS_REQ:
			value=icf_print_set_call_params(p_msg);

			break;
		case ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ:
			value=icf_print_set_self_id_params(p_msg);

			break;
		case ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ:
			value=icf_print_set_reg_params(p_msg);
			break;
		case ICF_CFG_CC_SET_SERVICE_PARAMS_REQ:
			value=icf_print_set_service_params(p_msg);
			break;
		case ICF_CFG_CC_INIT_COMPLETE_REQ:
			value=icf_print_no_payload_msg(p_msg);
			break;
		case ICF_CC_CFG_SET_CONFIG_PARAM_RESP:
			value=icf_print_config_params_resp(p_msg);
			break;
		case ICF_PA_CC_CALL_HOLD_REQ:
			value=icf_print_chd_req(p_msg);
			break;
		case ICF_CC_PA_CALL_HOLD_RESP:
			value=icf_print_api_resp_msg(p_msg);
			break;
		case ICF_PA_CC_CALL_RESUME_REQ:
			value=icf_print_chd_req(p_msg);
			break;
		case ICF_CC_PA_CALL_RESUME_RESP:
			value=icf_print_api_resp_msg(p_msg);
			break;
		case ICF_CC_PA_CALL_REDIRECTION_IND:
			value=icf_print_call_redirection_ind(p_msg);
			break;
		case ICF_PA_CC_CONFERENCE_REQ:
			value=icf_print_conference_req(p_msg);
			break;
		case ICF_CC_PA_CONFERENCE_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case  ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ:
			value=icf_print_ctu_req(p_msg);
			break;
		case ICF_CC_PA_CALL_TRANSFER_UNATTENDED_RESP:
			value=icf_print_ctu_resp(p_msg);
			break;
		case ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ:
			value=icf_print_cta_req(p_msg);
			break;
		case ICF_CC_PA_CALL_TRANSFER_ATTENDED_RESP:
			value=icf_print_cta_resp(p_msg);
			break;
		case ICF_CC_PA_CALL_HOLD_IND:
			value=icf_print_api_ind(p_msg);
			break;
		case ICF_CC_PA_CALL_RESUME_IND:
			value=icf_print_api_ind(p_msg);
			break;
		case ICF_CC_PA_REMOTE_CALL_TRANSFER_INITIATED_IND:
			value=icf_print_rem_call_transfer_init(p_msg);
			break;
		case ICF_CC_PA_CALL_MERGED_IND:
			value=icf_print_call_merged_ind(p_msg);
			break;
		case  ICF_PA_CC_CALL_MUTE_REQ:
			value=icf_print_no_payload_msg(p_msg);
			break;
		case ICF_PA_CC_CALL_UNMUTE_REQ:
			value=icf_print_no_payload_msg(p_msg);
			break;
		case ICF_PA_CC_REMOTE_CALL_TRANSFER_INITIATED_CFM:
			value=icf_print_rem_call_transfer_init_cfm(p_msg);
			break;

		case  ICF_GET_LINE_SETTINGS_REQ:
			value=icf_print_get_line_settings(p_msg);
			break;
		case ICF_GET_LINE_SETTINGS_RESP:
			/* FIX for spr 17944 */
			value=icf_print_get_line_settings_resp(p_msg);
			/* spr 17944 fix end */
			break;
		case ICF_GET_SYSTEM_SETTINGS_REQ:
			value=icf_print_get_sys_settings(p_msg);
			break;
		case ICF_GET_SYSTEM_SETTINGS_RESP:
			value=icf_print_get_sys_settings_resp(p_msg);
			break;
		case ICF_APP_ADD_REQ:
			value=icf_print_app_add_req(p_msg);
			break;
		case ICF_APP_ADD_CFM:
			value=icf_print_app_add_cfm(p_msg);
			break;
		case ICF_APP_REMOVE_REQ:
			value=icf_print_app_remove_req(p_msg);
			break;
		case ICF_APP_REMOVE_CFM:
			value=icf_print_app_remove_cfm(p_msg);
 			break;
  		#ifdef ICF_NW_ACTIVATE 
        	case ICF_NETWORK_ACTIVATE_REQ:
	                value=value=icf_print_network_activate_req(p_msg);
			break;
    		case ICF_EVENT_INDICATION:   
            	 	value=icf_print_event_indication
			break;
         	case  ICF_NETWORK_ACTIVATE_CFM:
               		value=icf_print_network_activate_cfm(p_msg);
    			break;
 
        	case ICF_NETWORK_DEACTIVATE_REQ:
                	value=icf_print_network_deactivate_req(p_msg);
    			break;
        	case ICF_NETWORK_DEACTIVATE_CFM:
             		value=icf_print_network_deactivate_cfm(p_msg);
    			break;
		#endif 
        	case ICF_REGISTER_REQ:
             		value=icf_print_register_req(p_msg);
    			break;
		case ICF_REGISTER_CFM:
             		value=icf_print_register_cfm(p_msg);
    			break;
        	case ICF_REGISTER_STATUS_IND:
              		value=icf_print_register_status_ind(p_msg);
    			break;
        	case ICF_AKA_AUTH_RESP:
             		value=icf_print_aka_auth_resp(p_msg);
    			break;
		case ICF_SUBSCRIBE_REQ:
               		value=icf_print_subscribe_req(p_msg);
    			break;
		case ICF_TRACE_NW_BUFF_IND:
			value=icf_print_nw_buff_ind(p_msg);
			break;
		case ICF_SUBSCRIPTION_STATUS_IND:
        		value=icf_print_subscribe_resp(p_msg);
    			break;
    		case    ICF_REFER_REQ:
              		value=icf_print_refer_req(p_msg);
    			break;
        	case ICF_REFER_RESP:
        		value=icf_print_refer_resp(p_msg);
    			break;
       		case ICF_NOTIFY_IND:
        		value=icf_print_notify_ind(p_msg);
   	 		break;
		case ICF_PUBLISH_REQ:
            		value=icf_print_publish_req(p_msg);
    			break;
    		case ICF_PUBLISH_RESP:
        		value=icf_print_publish_resp(p_msg);
    			break; 
        	case ICF_MESSAGE_REQ:
              		value=icf_print_message_req(p_msg);
    			break;
    		case ICF_MESSAGE_RESP:
        		value=icf_print_message_resp(p_msg);
    			break;
    		case ICF_MESSAGE_IND:
        		value=icf_print_message_ind(p_msg);
    			break;
		case ICF_CFG_CODEC_INFO_REQ:
			value=icf_print_codec_info_req(p_msg);
			break;
		case ICF_CFG_RINGER_TONES_REQ:
			value=icf_print_ringer_tones_req(p_msg);
			break;
		case ICF_CFG_JBM_MGMT_REQ:
			value=icf_print_jbm_mgmt_req(p_msg);
			break;
		case ICF_CFG_MAX_EARP_VOL_REQ:
			value=icf_print_max_earp_vol_req(p_msg);
			break;
		case ICF_CFG_MAX_SPEAKER_VOL_REQ:
			value=icf_print_max_speaker_vol_req(p_msg);
			break;
		case ICF_CFG_SET_PORT_RANGE_REQ:
			value=icf_print_port_range_req(p_msg);
			break;
	
		case ICF_CFG_SET_SELF_IP_REQ:
			value=icf_print_self_ip_req(p_msg);
			break;
		case ICF_CFG_SET_SELF_IP_RESP:
			value=icf_print_resp_msg(p_msg);
			break;

		case ICF_CFG_SET_DIGIT_TRANS_REQ:
			value=icf_print_digit_trans_req(p_msg);
			break;
		case ICF_CFG_SET_DIGIT_TRANS_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
			value=icf_print_remote_video_diaplay_req(p_msg);     
			break;
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
			value=icf_print_resp_msg(p_msg);     
			break;
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
			value=icf_print_preview_video_diaplay_req(p_msg);     
			break;
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
			value=icf_print_resp_msg(p_msg);     
			break;
		case ICF_CFG_MEDIA_REQ:
			value=icf_print_cfg_media_req(p_msg);     
			break;
		case ICF_CFG_MEDIA_RESP:
			value=icf_print_resp_msg(p_msg);     
			break;
		case ICF_PLAY_MEDIA_REQ:
			value=icf_print_play_media_req(p_msg);     
			break;
		case ICF_PLAY_MEDIA_RESP:
			value=icf_print_resp_msg(p_msg);     
			break;
		case ICF_STOP_MEDIA_PLAY_REQ:
			value=icf_print_stop_media_play_req(p_msg);     
			break;
		case ICF_STOP_MEDIA_PLAY_RESP:
			value=icf_print_resp_msg(p_msg);     
			break;
		case ICF_STOP_MEDIA_PLAY_IND:
			value=icf_print_stop_media_play_ind(p_msg);     
			break;
		case ICF_SEND_DGTS_REQ:
			value=icf_print_send_dgts_req(p_msg);
			break;
		case ICF_START_RINGING_REQ:
			value=icf_print_start_ringing_req(p_msg);
			break;
		case ICF_GET_STATS_REQ:
			value=icf_print_get_stats_req(p_msg);
			break;
		case ICF_STOP_RINGING_REQ:
			value=icf_print_stop_ringing_req(p_msg);
			break;
		case ICF_SET_EARP_VOLUME_REQ:
			value=icf_print_set_earp_volume_req(p_msg);
			break;
		case ICF_SET_SPEAKER_VOLUME_REQ:
			value=icf_print_set_speaker_volume_req(p_msg);
			break;
		case ICF_SEND_APP_DATA_REQ:
			value=icf_print_send_app_data_req(p_msg);
			break;
		case ICF_SEND_STATS_REQ:
			value=icf_print_send_stats_req(p_msg);
			break;
    		case ICF_STOP_STATS_REQ:
			value=icf_print_stop_stats_req(p_msg);
			break;
		case ICF_STOP_RINGING_IND:
			value=icf_print_stop_ringing_ind(p_msg);
			break;
		case ICF_RCV_APP_DATA_IND:
			value=icf_print_rcv_app_data_ind(p_msg);
			break;
		case ICF_CFG_CODEC_INFO_RESP:
			value=icf_print_codec_info_resp(p_msg);
			break;
		case ICF_CFG_RINGER_TONES_RESP:
			value=icf_print_ringer_tones_resp(p_msg);
			break;
		case ICF_CFG_JBM_MGMT_RESP:
			value=icf_print_jbm_mgmt_resp(p_msg);
			break;
		case ICF_CFG_MAX_EARP_VOL_RESP:
			value=icf_print_max_earp_vol_resp(p_msg);
			break;
		case ICF_CFG_MAX_SPEAKER_VOL_RESP:
			value=icf_print_max_speaker_vol_resp(p_msg);
			break;
		case ICF_CFG_SET_PORT_RANGE_RESP:
			value=icf_print_set_port_range_resp(p_msg);
			break;
		case ICF_MEDIA_SESSION_RESP:
			value=icf_print_media_session_resp(p_msg);
			break;
    		case ICF_RTP_START_IND:
			value=icf_print_rtp_start_ind(p_msg);
			break;
		case ICF_GET_STATS_RESP:
			value=icf_print_get_stats_resp(p_msg);
			break;

		case ICF_START_RINGING_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_VIBRATION_REQ:
			value=icf_print_vibration_req(p_msg);
			break;
		case ICF_VIBRATION_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_STOP_VIBRATION_REQ:
			value=icf_print_stop_vibration_req(p_msg);
			break;
		case ICF_STOP_VIBRATION_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_ECHO_CANC_REQ:
			value=icf_print_echo_canc_req(p_msg);
			break;
		case ICF_ECHO_CANC_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_VAD_SS_CNG_REQ:
			value=icf_print_vad_ss_cng_req(p_msg);
			break;
		case ICF_VAD_SS_CNG_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_STOP_RINGING_RESP:
			value=icf_print_stop_ringing_resp(p_msg);
			break;
		case ICF_MUTE_MEDIA_RESP:
			value=icf_print_mute_resp(p_msg);
			break;
		case ICF_SET_EARP_VOLUME_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_SET_SPEAKER_VOLUME_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_SEND_APP_DATA_RESP:
			value=icf_print_send_app_data_resp(p_msg);
			break;
		case ICF_SEND_STATS_RESP:
			value=icf_print_send_stats_resp(p_msg);
			break;
		case ICF_STOP_STATS_RESP:
			value=icf_print_stop_stats_resp(p_msg);
			break;
		#ifdef ICF_NW_ACTIVATE
    		case ICF_GPRS_SEC_PDP_ACTIVATE_REQ:  
              		value=icf_print_sec_pdp_activate_req(p_msg);            
    			break;
   		 case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:  
              		 value=icf_print_sec_pdp_activate_resp(p_msg);            
   			 break;
    		case ICF_GPRS_PDP_DEACTIVATE_REQ:  
                	value=icf_print_pdp_deactivate_req(p_msg);            
   			break;
    		case ICF_GPRS_PDP_DEACTIVATE_RESP:  
           		value=icf_print_pdp_deactivate_resp(p_msg);            
   		 	break;
    		case ICF_PORT_PLTFM_PDP_MODIFY_IND:  
                	value=icf_print_pdp_modify_ind(p_msg);            
    			break;
     		case ICF_PORT_PLTFM_PDP_DEACT_IND:  
           		value=icf_print_pdp_deactivate_ind(p_msg);            
   			 break;
    		case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:  
               		value=icf_print_platform_event_ind(p_msg);            
    			break;
		case ICF_MM_SET_NW_STATUS:  
              		value=icf_print_nw_status(p_msg);
    			break;

		#endif
    		case ICF_CC_PA_INCOMING_CALL_IND:  
              		value=icf_print_incoming_call(p_msg);
			break;
    		case ICF_PA_CC_CREATE_CALL_REQ: 
              		value=icf_print_create_call(p_msg);
    			break;
  		case ICF_CC_PA_ALERT_USER_IND: 
               		value=icf_print_alert_user(p_msg);
    			break;
        case ICF_MEDIA_CONNECT_REQ: 
                    value=icf_print_media_connect_req(p_msg);
                break;
        case ICF_PA_CC_USER_ALERTED_REQ: 
                    value=icf_print_user_alerted(p_msg);
                break;  
		case ICF_CC_PA_MEDIA_CONNECTED_IND:  
              		value=icf_print_media_connected(p_msg);
    			break;  
		case ICF_CC_PA_CONNECT_IND:
            	 	value=icf_print_connect_ind(p_msg);
    			break;

		case ICF_CONNECT_MEDIA_DEVICE_REQ: 
               		value=icf_print_connect_media_device_req(p_msg);
    			break; 
		case ICF_CONNECT_MEDIA_DEVICE_RESP: 
              		value=icf_print_resp_msg(p_msg);
   		 	break; 

		case ICF_CC_PA_INCOMING_CALL_MODIFY_IND:
            		value=icf_print_incoming_call_modify_ind(p_msg);
    			break;
		case ICF_PA_CC_CALL_MODIFY_CFM:
	        	value=icf_print_call_modify_cfm(p_msg);
    			break;
		case ICF_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND:
              		value=icf_print_incoming_call_modify_cancel_ind(p_msg);
    			break;
		case ICF_PA_CC_CALL_MODIFY_REQ:
        		value=icf_print_call_modify_req(p_msg);
    			break;
		case ICF_CC_PA_CALL_MODIFY_RESP:
              		value=icf_print_call_modify_resp(p_msg);
    			break;
		case ICF_PA_CC_CONNECT_REQ:
             		value=icf_print_connect_req(p_msg);
    			break;
		case ICF_CONNECT_RESP:
              		value=icf_print_connect_resp(p_msg);
    			break;
		case ICF_REMOTE_USER_TERMINATE_CALL_RESP: 
           		value=icf_print_no_payload_msg(p_msg);
    			break;
		case ICF_LOCAL_USER_TERMINATE_CALL_RESP: 
             		value=icf_print_no_payload_msg(p_msg);
    			break;
    		case ICF_CC_PA_TERMINATE_CALL_IND: 
           		value=icf_print_term_call_ind(p_msg);
    			break;
                case ICF_PROGRESS_IND: 
           		value=icf_print_progress_ind(p_msg);
    			break;
		case ICF_PA_CC_TERMINATE_CALL_REQ: 
             		value=icf_print_term_call_req(p_msg);
    			break;
    		case ICF_GET_CODEC_LIST_REQ:  
              		value=icf_print_get_codec_list_req(p_msg);            
    			break;
		case ICF_GET_CODEC_LIST_RESP: 
              		value=icf_print_get_codec_list_resp(p_msg);
    			break;
 		case ICF_CODEC_COMMIT_REQ:    
               		value=icf_print_codec_commit_req(p_msg);            
    			break;
   		case ICF_CODEC_COMMIT_RESP:  
            		value=icf_print_codec_commit_resp(p_msg);
    			break;
		case ICF_OPEN_MEDIA_CHANNEL_REQ: 
         	     	value=icf_print_open_channel_req(p_msg);
		     	break;
		case ICF_OPEN_MEDIA_CHANNEL_RESP: 
            		value=icf_print_open_media_channel_resp(p_msg);
			break;
		case ICF_CLOSE_MEDIA_CHANNEL_REQ: 
              		value=icf_print_close_media_channel_req(p_msg);
			break;
		case ICF_SEND_UDP_DATA_REQ: 
             		value=icf_print_send_udp_data_req(p_msg);
			break;
		case ICF_RECV_UDP_DATA_IND: 
           		value=icf_print_recv_udp_data_ind(p_msg);
			break;
		case ICF_CREATE_MEDIA_SESSION_REQ: 
              		value=icf_print_cms_req(p_msg);  
    			break;
    		case ICF_CREATE_MEDIA_SESSION_RESP: 
            		value=icf_print_cms_resp(p_msg);      
    			break;
   		case ICF_MODIFY_MEDIA_SESSION_REQ:  
           		value=icf_print_mms_req(p_msg);
    			break;
		case ICF_MODIFY_MEDIA_SESSION_RESP: 
            		value=icf_print_mms_resp(p_msg);
    			break;
    		case ICF_DELETE_MEDIA_SESSION_REQ:  
            		value=icf_print_dms_req(p_msg);
    			break;
		case ICF_DELETE_MEDIA_SESSION_RESP: 
                	value=icf_print_resp_msg(p_msg);
    			break;
    		case ICF_MEDIA_CAPABILITIES_REQ:
	        	value=icf_print_media_capabilities_req(p_msg);
			break;
		case ICF_MEDIA_CAPABILITIES_RESP:
        		value=icf_print_med_cap_resp(p_msg);
			break;
		case ICF_DELETE_MEDIA_SESSION_IND:  
              		value=icf_print_dms_ind(p_msg);   
    			break;
    		case ICF_MERGE_MEDIA_SESSION_REQ:   
            		value=icf_print_merge_media_req_msg(p_msg);   
   	 		break;
		case ICF_MERGE_MEDIA_SESSION_RESP:
                	value=icf_print_resp_msg(p_msg);   
    			break;    
		/*case ICF_API_ID_NETFUSION: 
           		value=icf_print_common_msg(p_msg);
    			break;
		case IMS_API_ID_DUMMY_MSG: 
              		value=icf_print_common_msg(p_msg);
    			break;*/
		#ifdef ICF_IPSEC_ENABLED
    		case ICF_PORT_PLTFM_SA_CREATE_REQ:
            		value=icf_print_sa_create_req(p_msg);
    			break;

        	case ICF_PORT_PLTFM_SA_CREATE_RESP:
             		value=icf_print_sa_cmd_resp(p_msg);
    			break;

		#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
    			case ICF_PORT_PLTFM_SA_MODIFY_REQ:
              		value=icf_print_sa_modify_req(p_msg);
    			break;

    			case ICF_PORT_PLTFM_SA_MODIFY_RESP:
               		value=icf_print_sa_cmd_resp(p_msg);
   		 	break;
		#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */

   		case ICF_PORT_PLTFM_SA_DELETE_REQ:
              		value=icf_print_sa_delete_req(p_msg);
    			break;
		case ICF_PORT_PLTFM_SA_DELETE_RESP:
             		value=icf_print_sa_cmd_resp(p_msg);
    			break;
		case ICF_PORT_PLTFM_SA_EXPIRY_IND:
            		value=icf_print_sa_expiry_ind(p_msg);
    			break;
		#endif /* ICF_IPSEC_ENABLED */
		#ifdef ICF_FAX_SUPPORT
		case ICF_START_FAX_REQ:
        		value=icf_print_start_fax_req(p_msg);
    			break;
		case ICF_START_FAX_RESP:
        		value=icf_print_start_fax_resp(p_msg);
    			break;
		case ICF_START_FAX_IND:
        		value=icf_print_start_fax_ind(p_msg);
    			break;
		case ICF_STOP_FAX_RESP:
			value=icf_print_stop_fax_resp(p_msg);
                        break;
		case ICF_STOP_FAX_IND:
			value=icf_print_stop_fax_ind(p_msg);
                        break;
		#endif
/*call_mod_cancel*/
		case ICF_CALL_MODIFY_CANCEL_RESP:
		        value=icf_print_call_mod_cancel_resp(p_msg);
                        break;

/*************** Management Plane Handler (MPH) APIs Starts here *************/
	
	
		case ICF_CREATE_SECURE_CONN_RESP:
                	value=icf_print_create_secure_conn_resp(p_msg);
    			break;
	
		case ICF_CLOSE_SECURE_CONN_RESP:
              		value=icf_print_set_close_secure_conn_resp(p_msg);
    			break;
		case ICF_SECURE_CONN_STATUS_IND:
             		value=icf_print_secure_conn_status_ind(p_msg);
    			break;

		case ICF_MGMT_PLANE_API_PROC_FAILURE:
             		value=icf_print_mgmt_plane_api_proc_failure(p_msg);
    			break;
		case ICF_MGMT_PLANE_NW_RESP:
             		value=icf_print_set_mgmt_plane_nw_resp(p_msg);
    			break;
	
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
    /* MM API's */
		case ICF_CFG_MAX_MIC_GAIN_REQ:
			value=icf_print_max_mic_gain_req(p_msg);
			break;
		case ICF_SET_MIC_GAIN_REQ:
			value=icf_print_mic_gain_req(p_msg);
			break;
		case ICF_CFG_MAX_MIC_GAIN_RESP:
			value=icf_print_max_mic_gain_resp(p_msg);
			break;
		case ICF_SET_MIC_GAIN_RESP:
			value=icf_print_resp_msg(p_msg);
			break;    
		#ifdef ICF_STAT_ENABLED
    		case ICF_GET_ICF_STATS_REQ:
			value=icf_print_get_icf_stats_req(p_msg);
			break;
  	  	case ICF_GET_ICF_STATS_RESP:
			value=icf_print_get_icf_stats_resp(p_msg);
			break;
		#endif
    		case ICF_INFO_REQ:     
              		value=icf_print_info_req(p_msg);
    			break;
    		case ICF_INFO_RESP:    
               		value=icf_print_info_resp(p_msg);
    			break; 
		case ICF_MEDIA_RECORD_REQ:
			value=icf_print_media_record_req(p_msg);
			break;
		case ICF_MEDIA_RECORD_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_STOP_MEDIA_RECORD_REQ:
			value=icf_print_stop_media_record_req(p_msg);
			break;
		case ICF_STOP_MEDIA_RECORD_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_PAUSE_MEDIA_REQ:
			value=icf_print_pause_media_req(p_msg);
			break;
		case ICF_PAUSE_MEDIA_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
			value=icf_print_set_media_playback_device_req(p_msg);
			break;
		case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
			value=icf_print_resp_msg(p_msg);
			break;
		case ICF_MUTE_MEDIA_REQ:
			value=icf_print_mute_media_req(p_msg);
			break;
		case ICF_DGTS_IND:
			value=icf_print_dgts_ind(p_msg);
			break;
		case ICF_CALL_FORWARDED_IND:
        		value=icf_print_call_fwd_ind(p_msg);
    			break;
		case ICF_REINITIALISE_REQ:
	        	value=icf_print_no_payload_msg(p_msg);
    			break;
		case ICF_INFO_IND:    
                	value=icf_print_info_ind(p_msg);
    			break; 
		case ICF_INFO_CFM:    
             		value=icf_print_info_cfm(p_msg);     
    			break;
		case ICF_NOTIFY_CFM:
	        	value=icf_print_notify_cfm(p_msg);
    			break;
		case ICF_OPTIONS_REQ:
              		value=icf_print_options_req(p_msg);
    			break;
		case ICF_OPTIONS_RESP:
	        	value=icf_print_options_resp(p_msg);
    			break;
		case ICF_OPTIONS_CFM:
              		value=icf_print_options_cfm(p_msg);
    			break;
		case ICF_INC_OPTIONS_IND:
              		value=icf_print_options_ind(p_msg);
    			break;
		case ICF_SET_LINE_PARAM_REQ:
              		value=icf_print_set_line_param_req(p_msg);
    			break;
	    case ICF_EARLY_INC_CALL_IND:
           value=icf_print_early_call_ind(p_msg);
           break;
        case ICF_EARLY_INC_CALL_CFM:
           value=icf_print_early_call_cfm(p_msg);
           break;
        case ICF_UNEXPECTED_MSG_IND:
           value=icf_print_unexpected_msg_ind(p_msg);
           break;
        case ICF_MM_UNEXPECTED_MSG_IND:
           value=icf_print_mm_unexpected_msg_ind(p_msg);
           break;
        case ICF_GET_REGISTER_STATUS_REQ:
           value = icf_print_get_reg_status_req(p_msg);
           break;
        case ICF_GET_REGISTER_STATUS_RESP:
           value = icf_print_get_reg_status_resp(p_msg);
           break;
#ifdef ICF_DNS_LOOKUP_ENABLED
        case ICF_DNS_MAPPING_CHANGE_IND:
           value = icf_print_dns_mapping_change_ind(p_msg);
           break;
#endif /* ICF_DNS_LOOKUP_ENABLED */           
        case  ICF_API_ID_UNDEFINED:                            
           value=icf_print_api_header(p_msg);
           break;
        /* Traces for new tunnel mode specific APIs */
        case ICF_TUNNELLED_CREATE_MEDIA_SESSION_REQ: 
           value=icf_print_tunnelled_cms_req(p_msg);  
    	   break;
    	case ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP: 
           value=icf_print_tunnelled_cms_resp(p_msg);      
    	   break;
   		case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_REQ:  
           value=icf_print_tunnelled_mms_req(p_msg);
    	   break;
		case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP: 
           value=icf_print_tunnelled_mms_resp(p_msg);
    	   break;
           case ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP:
           value = icf_print_tunnelled_media_capabilities_resp(p_msg);
           break;
           case ICF_TUNNELLED_OPTIONS_RESP:
           value = icf_print_tunnelled_options_resp(p_msg);
           break;
           case ICF_EARLY_SESSION_IND:
           value = icf_print_early_session_ind(p_msg);
           break;
           case ICF_EARLY_SESSION_CFM:
           value = icf_print_early_session_cfm(p_msg);
           break;
        case ICF_SESSION_TIMER_TOGGLE_REQ:
	   value = icf_print_session_timer_toggle_req(p_msg);
	   break;
	case ICF_SESSION_TIMER_TOGGLE_RESP:
	   value = icf_print_session_timer_toggle_resp(p_msg);
	   break;

	case ICF_GENERIC_MSG_REQ:
	   value = icf_print_generic_msg_req(p_msg);
	   break;
	case ICF_GENERIC_MSG_RESP:
	   value = icf_print_generic_msg_resp(p_msg);
	   break;
	case ICF_GENERIC_MSG_IND:
	   value = icf_print_generic_msg_ind(p_msg);
	   break;
	case ICF_GENERIC_MSG_CFM:
	   value = icf_print_generic_msg_cfm(p_msg);
	   break;
        /*  Rel 8.2 changes Start */
        case ICF_PRACK_REQ_RECV_IND: 
            value=icf_print_prack_req_recv_ind(p_msg);
        break;
        case ICF_PRACK_RESP_SENT_IND: 
            value=icf_print_prack_resp_sent_ind(p_msg);
        break;
        case ICF_PRACK_REQ_SENT_IND: 
            value=icf_print_prack_req_sent_ind(p_msg);
        break;
        case ICF_PRACK_RESP_RECV_IND: 
            value=icf_print_prack_resp_recv_ind(p_msg);
        break;
        /*  Rel 8.2 changes End */
        case ICF_MESSAGE_CFM:
        value = icf_print_message_cfm(p_msg);
        break;

	}

	ICF_PRINT_MESSAGE(("\n}\n"));
}
#endif



