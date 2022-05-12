/*------------------------------------------------------------------------------
 *
 * File name        : icf_ippstk_map_defs.h
 *
 * Purpose          : This file contains the mapping of IPPSTK and ICF defs.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 03-Aug-2005  Rohit Aggarwal                  Initial creation
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD Added constants for IPSec
 * 02-Feb-2006  Amit Sharma                     CFG merged with IPPSTK 2.0
 * 22-Feb-2005	Amit Sharma                     Added code for auth support
 * 07-Mar-2006  Anuradha Gupta  ICF Rel 5.0 CDD Added constants for Reinvite Handling
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/



#ifndef __ICF_IPPSTK_MAP_DEFS_H__
#define __ICF_IPPSTK_MAP_DEFS_H__


#include "icf_defs.h"
#include "icf_error_id.h"



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
	


/* These hash defines are used for indicating presence of some common optional 
 * parameters in the ICF APIs. These values should be used in the bit_mask if
 * any of the elements is to be included in an API structures containing
 * header/tag/msg-body/privacy-ind/pref-identity 
 */

#define IPPSTK_PRIVACY_IND_PRESENT		ICF_PRIVACY_IND_PRESENT

#define IPPSTK_PREF_IDENTITY_PRESENT	ICF_PREF_IDENTITY_PRESENT

#define IPPSTK_HDR_LIST_PRESENT			ICF_HDR_LIST_PRESENT

#define IPPSTK_TAG_LIST_PRESENT			ICF_TAG_LIST_PRESENT

#define IPPSTK_BODY_LIST_PRESENT		ICF_BODY_LIST_PRESENT

#define IPPSTK_STREAM_LIST_PRESENT		ICF_STREAM_LIST_PRESENT



/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for ippstk_defs.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/


/* Maximum number of integer fields present in the ippstk_display_data_st*/
#define IPPSTK_MAX_INT_DISPLAY_DATA ICF_MAX_INT_DISPLAY_DATA


/* Maximum number of integer fields present in the ippstk_error_data_st*/
#define IPPSTK_MAX_INT_ERROR_DATA ICF_MAX_INT_ERROR_DATA

 
/* Maximum number of integer fields present in the trace data in the 
 * ippstk_trace_data_st*/
#define IPPSTK_MAX_INT_TRACE_DATA ICF_MAX_INT_TRACE_DATA


/* Maximum number of user names per line */
#define IPPSTK_MAX_NAME_PER_LINE ICF_MAX_NAME_PER_LINE


/* Maximum number of dailed/received/missed calls to be logged */
#define IPPSTK_MAX_NUM_OF_CALLS_TO_LOG ICF_MAX_NUM_OF_CALLS_TO_LOG


/* Maximum number of configurations elements per line present in configuration
   data */
#define IPPSTK_MAX_NUM_OF_CONFIG_ELEMENTS ICF_MAX_NUM_OF_CONFIG_ELEMENTS


/* Maximun number of lines supported by phone */
#define IPPSTK_MAX_NUM_OF_LINES ICF_MAX_NUM_OF_LINES


/* Maximun number of self numbers per line */
#define IPPSTK_MAX_NUM_PER_LINES ICF_MAX_NUM_PER_LINES


/* Maximum number of string fields present in the ippstk_display_data_st */
#define IPPSTK_MAX_STR_DISPLAY_DATA ICF_MAX_STR_DISPLAY_DATA


/* Maximum number of string fields present in the ippstk_error_data_st */
#define IPPSTK_MAX_STR_ERROR_DATA ICF_MAX_STR_ERROR_DATA


/* Maximun number of charactersstored in structure ippstk_string_st */
#define IPPSTK_MAX_STR_LEN ICF_MAX_STR_LEN		 


/* Maximum number of string data field present in the trace data in 
 * ippstk_trace_data_st */
#define IPPSTK_MAX_STR_TRACE_DATA ICF_MAX_STR_TRACE_DATA 


/*Maximum number of users per line that can be configured by user */
#define IPPSTK_MAX_NUM_OF_USERS_PER_LINE ICF_MAX_NUM_OF_USERS_PER_LINE

/* Denotes maximum number of stats in a stats group */
#define IPPSTK_MAX_STATS_IN_STATS_GROUP ICF_MAX_STATS_IN_STATS_GROUP


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for supported range of initialization parameters
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/

/* Denotes minimum number of applications supported */
#define IPPSTK_MIN_NUM_APPLICATION ICF_MIN_NUM_APPLICATION

/* Denotes maximum number of applications supported */
#define IPPSTK_MAX_NUM_APPLICATION ICF_MAX_NUM_APPLICATION

/* Denotes maximum number of networks supported */
#define IPPSTK_MAX_NUM_NETWORK ICF_MAX_NUM_NETWORK

/* Denotes minimum number of calls supported */
#define IPPSTK_MIN_NUM_CALL ICF_MIN_NUM_CALL

/* Denotes maximum number of calls supported */
#define IPPSTK_MAX_NUM_CALL ICF_MAX_NUM_CALL

/* Denotes minimum number of uris supported */
#define IPPSTK_MIN_NUM_URI ICF_MIN_NUM_URI

/* Denotes maximum number of uris supported */
#define IPPSTK_MAX_NUM_URI ICF_MAX_NUM_URI

/* Denotes minimum number of calls to log supported */
#define IPPSTK_MIN_NUM_OF_CALLS_TO_LOG ICF_MIN_NUM_OF_CALLS_TO_LOG

/* Denotes maximum number of registration supported */
#define IPPSTK_MAX_NUM_OF_REGISTRATION ICF_MAX_NUM_OF_REGISTRATION

/* Default values */
#define IPPSTK_DEFAULT_MAX_LINES ICF_DEFAULT_MAX_LINES

#define IPPSTK_DEFAULT_MAX_APPLICATIONS ICF_DEFAULT_MAX_APPLICATIONS

#define IPPSTK_DEFAULT_MAX_REGISTRATIONS ICF_DEFAULT_MAX_REGISTRATIONS

#define IPPSTK_DEFAULT_MAX_CALLS ICF_DEFAULT_MAX_CALLS

#define IPPSTK_DEFAULT_MAX_NETWORKS ICF_DEFAULT_MAX_NETWORKS

#define IPPSTK_DEFAULT_SYS_RESOURCE_LIMIT ICF_DEFAULT_SYS_RESOURCE_LIMIT

#define IPPSTK_DEFAULT_MAX_URIS ICF_DEFAULT_MAX_URIS

#define IPPSTK_DEFAULT_MAX_CALLS_TO_LOG ICF_DEFAULT_MAX_CALLS_TO_LOG

#define IPPSTK_DEFAULT_AUTO_ACTIVATE ICF_DEFAULT_AUTO_ACTIVATE

#define IPPSTK_DEFAULT_COMPACT_SIP_HDRS ICF_DEFAULT_COMPACT_SIP_HDRS

/* ICF_4_0_CRM_START */
#define IPPSTK_DEFAULT_AUTO_REGISTER ICF_DEFAULT_AUTO_REGISTER
#define IPPSTK_DUMMY_APP_ID	     ICF_DUMMY_APP_ID
/* ICF_4_0_CRM_END */

#define IPPSTK_DEFAULT_APN_STR_LEN ICF_DEFAULT_APN_STR_LEN

#define IPPSTK_DEFAULT_SIGCOMP_REQUIRED ICF_DEFAULT_SIGCOMP_REQUIRED

#define IPPSTK_DEFAULT_AUTH_PASSWD_STR_LEN ICF_DEFAULT_AUTH_PASSWD_STR_LEN

/* Default packetization period */
#define IPPSTK_DEFAULT_PACKET_PERIOD    200

/*------------------------------------------------------------------------------
 * Constants needed for IPSec Support in ICF Release 4.0
 *----------------------------------------------------------------------------*/

/* This defines the maximum number of integrity algorithms supported by ICF */
#define IPPSTK_MAX_SUPP_I_ALGO ICF_MAX_SUPP_I_ALGO

/* This defines the maximum number of encryption algorithms supported by ICF */
#define IPPSTK_MAX_SUPP_E_ALGO ICF_MAX_SUPP_E_ALGO

/* This shall be a Boolean value, 0 or 1, indicating whether ICF currently 
 * supports IP Security or not */
#define IPPSTK_DEFAULT_IPSEC_ENABLED ICF_DEFAULT_IPSEC_ENABLED

/* This shall be a finite positive number indicating the limit up to which 
 * successive transaction timeouts will be acceptable. In case the number of 
 * consecutive transactions for which no response has been received exceeds this
 * configured value, it shall imply a network failure */
#define IPPSTK_DEFAULT_IPSEC_TXN_THRESHOLD ICF_DEFAULT_IPSEC_TXN_THRESHOLD

/* This shall be the lower limit of a pre-determined range of port numbers that 
 * will be used as protected server and client ports */
#define IPPSTK_DEFAULT_IPSEC_START_PORT_VAL ICF_DEFAULT_IPSEC_START_PORT_VAL

/* This shall specify the number of ports that will be used as protected server 
 * and client ports */
#define IPPSTK_DEFAULT_IPSEC_PORT_COUNT ICF_DEFAULT_IPSEC_PORT_COUNT

/* This shall specify the duration of SA Create Timer in milliseconds */
#define IPPSTK_DEFAULT_IPSEC_SA_CREATE_TIMER ICF_DEFAULT_IPSEC_SA_CREATE_TIMER

/* This shall specify the duration of SA Modify Timer in milliseconds */
#define IPPSTK_DEFAULT_IPSEC_SA_MODIFY_TIMER ICF_DEFAULT_IPSEC_SA_MODIFY_TIMER

/* This shall specify the duration of SA Delete Timer in milliseconds */
#define IPPSTK_DEFAULT_IPSEC_SA_DELETE_TIMER ICF_DEFAULT_IPSEC_SA_DELETE_TIMER


/* ------------------ End: Constants for IP Security Support ---------------- */


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for ippstk_common_api_defs.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/


/*    This is version of the API. */
#define IPPSTK_VERSION_1_0					ICF_VERSION_1_0


/*
 * This is used at Eshell interface while filling the ip address
 * of the source of a received SIP message so that that SSA can further
 * use the IP address
 */
#define IPPSTK_MAX_IPV6_ADDRESS_LENGTH    ICF_MAX_IPV6_ADDRESS_LENGTH
#define IPPSTK_MAX_IPV4_ADDRESS_LENGTH    ICF_MAX_IPV4_ADDRESS_LENGTH

/*------------------------------------------------------------------------------
 * Description: Values for data type  ippstk_status_t and ippstk_boolean_t
 *----------------------------------------------------------------------------*/

#define IPPSTK_TRUE              ICF_TRUE
#define IPPSTK_FALSE             ICF_FALSE

    
/*------------------------------------------------------------------------------
 * Description: Values for data type  ippstk_result_t
 *----------------------------------------------------------------------------*/

#define IPPSTK_SUCCESS              ICF_SUCCESS
#define IPPSTK_FAILURE              ICF_FAILURE

    
/*------------------------------------------------------------------------------
 * Description: Values for data type  ippstk_module_id_t
 *----------------------------------------------------------------------------*/

/* These module ids are only for internal processing */
   
#define IPPSTK_MODULE_IPPSTK          ICF_MODULE_ICF

#define IPPSTK_GENERIC_APP_MODULE_ID  ICF_GENERIC_APP_MODULE_ID

/* Dummy Module Id for auto activate and auto-register */
#define IPPSTK_DUMMY_MODULE_ID        0xFE

#define IPPSTK_MODULE_PLATFORM        ICF_MODULE_PLATFORM

#define IPPSTK_MAX_MODULE             ICF_MAX_MODULE


/*------------------------------------------------------------------------------
 *
 *  ippstk_address_type_t values
 *
 *----------------------------------------------------------------------------*/

#define IPPSTK_ADDRESS_PLAN_UNDEFINED      ICF_ADDRESS_PLAN_UNDEFINED
#define IPPSTK_ADDRESS_PLAN_NOT_PRESENT    ICF_ADDRESS_PLAN_NOT_PRESENT
#define IPPSTK_ADDRESS_PLAN_RESTRICTED     ICF_ADDRESS_PLAN_RESTRICTED_TO_BE_PRESENTED
#define IPPSTK_ADDRESS_PLAN_SIP_URI        ICF_ADDRESS_PLAN_SIP_URI
#define IPPSTK_ADDRESS_PLAN_E164           ICF_ADDRESS_PLAN_E164
#define IPPSTK_ADDRESS_PLAN_TEL_URI        ICF_ADDRESS_PLAN_TEL_URI


/*ICF_CRM_START*/
/*----------------------------------------------------------------------------
 *
 *   This values specifies the type of network access
 *
 *--------------------------------------------------------------------------*/

/* Specifies the network access is GPRS. */
#define IPPSTK_GPRS             ICF_GPRS

/* This indicates UDP IPC. */
#define IPPSTK_IPC_UDP          ICF_IPC_UDP

/* This is the invalid APP ID. */
#define IPPSTK_GENERIC_APP_ID   ICF_GENERIC_APP_ID

/*ICF_CRM_END*/


/*----------------------------------------------------------------------------
 *
 *  ippstk_call_clear_reason_t This data type is used to define a value of type 
 *  ippstk_uint8_t that indicates the reason for call clear
 *
 *--------------------------------------------------------------------------*/

/* Invalid value */
#define IPPSTK_CALL_CLEAR_REASON_INVALID ICF_CALL_CLEAR_REASON_INVALID

/* Sent or received from PA. Specifies an undefined call clear reason. */
#define IPPSTK_CALL_CLEAR_REASON_UNDEFINED ICF_CALL_CLEAR_REASON_UNDEFINED

/* Received from PA. Specifies that local user has gone onhook after the call 
 * is answered by the remote user */
#define IPPSTK_CALL_CLEAR_REASON_ONHOOK_BY_USER ICF_CALL_CLEAR_REASON_ONHOOK_BY_USER

/* Sent to PA. Specifies that BYE is received from the remote user */
#define IPPSTK_CALL_CLEAR_REASON_REMOTE_RELEASED ICF_CALL_CLEAR_REASON_REMOTE_RELEASED

/* Sent to PA. Specifies that CANCEL is received from the remote user in 
 * response to the INVITE sent by local user */
#define IPPSTK_CALL_CLEAR_REASON_REMOTE_REJECTED ICF_CALL_CLEAR_REASON_REMOTE_REJECTED

/* Received from PA. Specifies that the call is cleared by the local user before
 * the call is answered */
#define IPPSTK_CALL_CLEAR_REASON_CALL_ABANDONED ICF_CALL_CLEAR_REASON_CALL_ABANDONED

/* Sent to PA. Specifies that the call is released because resources are not 
 * available to handle the call.*/
#define IPPSTK_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE

/* Sent to PA. Specifies that the SIP proxy or the end user agent is unable to 
 * find the user id specified in “to” header. Toolkit use this reason when
 * it receives 300, 301, 404, 410, 480, 485 or 604 response code from network.*/
#define IPPSTK_CALL_CLEAR_REASON_USER_NOT_FOUND ICF_CALL_CLEAR_REASON_USER_NOT_FOUND

/* Sent to PA. Specifies that the INVITE should be sent to correct proxy server.
 * Toolkit use this reason when it receives 305 response code from network. */
#define IPPSTK_CALL_CLEAR_REASON_CORRECT_PROXY_REQD ICF_CALL_CLEAR_REASON_CORRECT_PROXY_REQD

/* Sent to PA. Specifies that the payment is required from the user. Toolkit use
 * this reason when it receives 402 response code from network. */
#define IPPSTK_CALL_CLEAR_REASON_PAYMENT_REQD ICF_CALL_CLEAR_REASON_PAYMENT_REQD

/* Sent to PA. Specifies that there is no response from remote entity. Toolkit 
 * use this reason when it receives 408 response code from network. */
#define IPPSTK_CALL_CLEAR_REMOTE_UNREACHABLE ICF_CALL_CLEAR_REMOTE_UNREACHABLE

/* Sent to PA. Specifies that an error has happened in network while processing 
 * of this call. Toolkit use this reason when it receives 380, 400, 413, 414, 
 * 416, 481, 482, 483, 484, 493 and 5xx response code from network. */
#define IPPSTK_CALL_CLEAR_REASON_NW_ERROR ICF_CALL_CLEAR_REASON_NW_ERROR

/* Sent to PA. Specifies that the remote user is busy. Tookit use this reason 
 * when it receives 486 and 600 response code from network. */
#define IPPSTK_CALL_CLEAR_REMOTE_BUSY ICF_CALL_CLEAR_REMOTE_BUSY

/* Sent to PA. Specifies that the local media parameters sent are not supported 
 * by remote or the remote media parameters received are not supported by local 
 * phone. Toolkit use this reason when it receives 488 response code from 
 * network or media commit fails at local phone */
#define IPPSTK_CALL_CLEAR_MEDIA_MISMATCH ICF_CALL_CLEAR_MEDIA_MISMATCH

/* Sent to PA. Specifies that the request message has been rejected by the proxy
 * server. Toolkit use this reason when it receives 401, 403, 407, 420 and 421 
 * response code from network. */
#define IPPSTK_CALL_CLEAR_REASON_NW_REJECTED ICF_CALL_CLEAR_REASON_NW_REJECTED

/* Sent to PA. Specifies that the remote or local user has not answered the call
 * before expiry of alerting timer. */
#define IPPSTK_CALL_CLEAR_REASON_NOANSWER ICF_CALL_CLEAR_REASON_NOANSWER

/* Sent to PA. Specifies that some system error has happened in toolkit like 
 * memory allocation/deallocation failure, message send failure, timer 
 * start/stop failure etc */
#define IPPSTK_CALL_CLEAR_REASON_SYSTEM_ERROR ICF_CALL_CLEAR_REASON_SYSTEM_ERROR

/* Sent to PA. Specifies that some service failure has happened in toolkit 
 * because of which call is released */
#define IPPSTK_CALL_CLEAR_REASON_SERVICE_FAILURE ICF_CALL_CLEAR_REASON_SERVICE_FAILURE

/* Sent to PA. Specifies that the toolkit cannot accept a new call because 
 * number of calls have exceeded max_audio_calls configured */
#define IPPSTK_CALL_CLEAR_REASON_MAX_AUDIO_CALLS_EXCEEDED ICF_CALL_CLEAR_REASON_MAX_CALLS_EXCEEDED

/* Sent to PA. Specifies that the system has not yet initialized and hence 
 * cannot handle a new call */
#define IPPSTK_CALL_CLEAR_REASON_SYSTEM_NOT_INITIALIZED ICF_CALL_CLEAR_REASON_SYSTEM_NOT_INITIALIZED

/* This is reason code sent by PA when it rejects the incoming call and the user
 * is already busy in other call in some other line or when PA does not want to
 * present the CW call to user and wants to give it a busy treatment */
#define IPPSTK_CALL_CLEAR_REASON_USER_BUSY ICF_CALL_CLEAR_REASON_USER_BUSY

/* This is reason code sent to PA when the call clear is initiated because of   
 * call transfer unattended has been executed succefully and call has been 
 * transferred */
#define IPPSTK_CALL_CLEAR_REASON_CALL_TRANSFERRED ICF_CALL_CLEAR_REASON_CALL_TRANSFERRED

/* This is reason code sent to PA when call clear is initiated because there is
 * no proxy address configured for the line and call_thru_proxy is configured */
#define IPPSTK_CALL_CLEAR_REASON_PROXY_ADDRESS_NOT_CONFIGURED ICF_CALL_CLEAR_REASON_PROXY_ADDRESS_NOT_CONFIGURED

/* This is reason code specifying that call is successfully forwarded
 */    
#define IPPSTK_CALL_CLEAR_REASON_CALL_FORWARDED ICF_CALL_CLEAR_REASON_CALL_FORWARDED

/* This is reason code sent to PA specifying that there is no user configured for
 * this line and hence the call cannot be orignated.
 */    
#define IPPSTK_CALL_CLEAR_REASON_SELF_ADDR_NOT_CFGRD_FOR_LINE ICF_CALL_CLEAR_REASON_SELF_ADDR_NOT_CFGRD_FOR_LINE

/* Indicates that the line corresponding to a given line id is not yet
 * configured */
#define IPPSTK_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED ICF_CALL_CLEAR_REASON_LINE_NOT_CONFIGURED


/* Implies that an empty TBCP stream list was sent by the app for a call 
   that required TBCP support */
#define IPPSTK_CALL_CLEAR_REASON_EMPTY_TBCP_STREAM ICF_CALL_CLEAR_REASON_EMPTY_TBCP_STREAM


/* Implies that a non-empty TBCP stream list was sent by the app for a 
   call that did not support TBCP */
#define IPPSTK_CALL_CLEAR_REASON_CALL_TYPE_MISMATCH ICF_CALL_CLEAR_REASON_CALL_TYPE_MISMATCH


/* Indicates that a user is not registered */
#define IPPSTK_CALL_CLEAR_REASON_USER_NOT_REGISTERED ICF_CALL_CLEAR_REASON_USER_NOT_REGISTERED



/* Indicates that the first element of stream_list was not tbcp stream */
#define IPPSTK_CALL_CLEAR_REASON_INVALID_TBCP_DATA  ICF_CALL_CLEAR_REASON_INVALID_TBCP_DATA



/* Indicates that the call_type in api was invalid */
#define IPPSTK_CALL_CLEAR_REASON_INVALID_CALL_TYPE  ICF_CALL_CLEAR_REASON_INVALID_CALL_TYPE


/* Indicates that the stream_id or stream_type in api was invalid */
#define IPPSTK_CALL_CLEAR_REASON_INVALID_STREAM		ICF_CALL_CLEAR_REASON_INVALID_STREAM

/* Indicates session timer is expired*/
#ifdef IPPSTK_SESSION_TIMER
#define IPPSTK_CALL_CLEAR_REASON_SESSION_TIMER_EXPIRED ICF_CALL_CLEAR_REASON_SESSION_TIMER_EXPIRED
#endif

/* Indicates that the line is already in use by some other call */
#define IPPSTK_CALL_CLEAR_REASON_LINE_BUSY             ICF_CALL_CLEAR_REASON_LINE_BUSY
		
#define IPPSTK_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD  ICF_CALL_CLEAR_REASON_AUTH_PARAM_NOT_CFGRD
#define IPPSTK_CALL_CLEAR_REASON_PROXY_AUTH_FAILURE    ICF_CALL_CLEAR_REASON_PROXY_AUTH_FAILURE

#define IPPSTK_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE ICF_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE
#define IPPSTK_CALL_CLEAR_REASON_INCONSISTENT_TRANSPORT_MODE	ICF_CALL_CLEAR_REASON_INCONSISTENT_TRANSPORT_MODE
#define IPPSTK_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE		ICF_CALL_CLEAR_REASON_ATTEMPT_ON_ALTERNATE_LINE

/*******************************************************************************
 *
 * Values of data type ippstk_error_t. Error cause used in all response API's
 *
*******************************************************************************/

/* Undefined error cause */
#define IPPSTK_CAUSE_UNDEFINED ICF_ERROR_UNDEFINED

/* Call Id does not exist */
#define IPPSTK_CAUSE_INVALID_CALL_ID ICF_CAUSE_INVALID_CALL_ID

/* Remote party rejected call hold request. This happens when CALCEL or error
 *  response code is received on network in response to the INVITE sent.
 */
#define IPPSTK_CAUSE_REMOTE_REJECT_CALL_HOLD_REQ ICF_CAUSE_REMOTE_REJECT_CALL_HOLD_REQ

/* Local party is already on hold, so another hold request cannot be handled */
#define IPPSTK_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD ICF_CAUSE_PARTY_ALREADY_ON_LOCAL_HOLD

/* The request is invalid in this service scenario */
#define IPPSTK_CAUSE_INVALID_SERVICE_ACTION ICF_CAUSE_INVALID_SERVICE_ACTION

/* The service requested is not subscribed by user */
#define IPPSTK_CAUSE_SERVICE_NOT_SUBSCRIBED ICF_CAUSE_SERVICE_NOT_SUBSCRIBED

/* The service requested is not activated by user */
#define IPPSTK_CAUSE_SERVICE_NOT_ACTIVATED ICF_CAUSE_SERVICE_NOT_ACTIVATED

/* The service cannot be invoked because of another service already active */
#define IPPSTK_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION ICF_CAUSE_PROHIBITED_BY_SERVICE_INTERACTION

/* Conference already exists */
#define IPPSTK_CAUSE_CONFRENCE_EXIST ICF_CAUSE_CONFRENCE_EXIST

/* Call resume rejected by remote party. This happens when CANCEL or error
 * response code is received on network in response to the INVITE sent
 */
#define IPPSTK_CAUSE_REMOTE_REJECT_CALL_RESUME_REQ ICF_CAUSE_REMOTE_REJECT_CALL_RESUME_REQ

/* Error response received from network */
#define IPPSTK_CAUSE_NETWORK_ERROR ICF_CAUSE_NETWORK_ERROR

/* Response not received within timeout */
#define IPPSTK_CAUSE_TIMER_EXPIRY ICF_CAUSE_TIMER_EXPIRY

/* The call for which resume is requested is not on hold */
#define IPPSTK_CAUSE_CALL_ALREADY_ACTIVE ICF_CAUSE_CALL_ALREADY_ACTIVE

/* One call is already active on same line so another call cannot be made active
 */
#define IPPSTK_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE

/* Not able to create conference */
#define IPPSTK_CAUSE_CALL_MERGE_FAIL ICF_CAUSE_CALL_MERGE_FAIL

/* Call transfer request rejected by remote party */
# define IPPSTK_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ ICF_CAUSE_REMOTE_REJECT_CALL_TRANFER_REQ


/* No free audio codec is available for the new RTP session or the codec
 * requested is not available.
 */
#define IPPSTK_CAUSE_AUDIO_CODEC_NOT_AVAILABLE ICF_CAUSE_AUDIO_CODEC_NOT_AVAILABLE

/* No free video codec is available for the new RTP session or the codec
 * requested is not available.
 */
#define IPPSTK_CAUSE_VIDEO_CODEC_NOT_AVAILABLE ICF_CAUSE_VIDEO_CODEC_NOT_AVAILABLE

/* Resource manager is unable to release media resources for the call */
#define IPPSTK_CAUSE_UNABLE_TO_RELEASE_RESOURCES ICF_CAUSE_UNABLE_TO_RELEASE_RESOURCES

/* Mode requested for the RTP session is not inline with the SDP parameters
 * Eg mode of send only cannot be applied when remote RTP and codec information
 * is not available.
 */
#define IPPSTK_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP ICF_CAUSE_MODE_NOT_COMPATIBLE_TO_SDP

/* The system is unable to create new media session */
#define IPPSTK_CAUSE_UNABLE_TO_CREATE_MEDIA ICF_CAUSE_UNABLE_TO_CREATE_MEDIA

/* Insufficient pramaters are present in the API */
#define IPPSTK_CAUSE_INSUFFICIENT_PARAMETERS ICF_CAUSE_INSUFFICIENT_PARAMETERS

/* Insufficient reources are present to perform the request */
#define IPPSTK_CAUSE_INSUFFICIENT_RESOURCES ICF_CAUSE_INSUFFICIENT_RESOURCES

/* Action requested is invalid for the current call scenario */
#define IPPSTK_CAUSE_INVALID_SCENARIO_FOR_MERGE ICF_CAUSE_INVALID_SCENARIO_FOR_MERGE

/* Merge not supported by system.*/
#define IPPSTK_CAUSE_MERGE_NOT_SUPPORTED ICF_CAUSE_MERGE_NOT_SUPPORTED

/* Media Manager is unable to delete ongoing media session */
#define IPPSTK_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION ICF_CAUSE_UNABLE_TO_DELETE_MEDIA_SESSION

/* System not able to initialized */
#define IPPSTK_CAUSE_INITIALIZATION_FAILURE ICF_CAUSE_INITIALIZATION_FAILURE

/* Not able to bind with the self IP address provided in the API */
#define IPPSTK_CAUSE_SELF_IP_ADDR_BIND_FAILURE ICF_CAUSE_SELF_IP_ADDR_BIND_FAILURE

/* The stats id specified in the API does no exist */
#define IPPSTK_CAUSE_INVALID_STATS_ID IPPSTK_CAUSE_INVALID_STATS_ID

/* IP port specified in the API is out of range */
#define IPPSTK_CAUSE_IP_PORT_OUT_OF_RANGE ICF_CAUSE_IP_PORT_OUT_OF_RANGE

/* IP port specified in the API is out of range */
#define IPPSTK_CAUSE_MEDIA_FAILURE ICF_CAUSE_MEDIA_FAILURE
 
/* Indicates that max_lines received in API IPPSTK_CFG_CC_SET_SCALE_PARAMS is 
 * not in range 1 to 24 */
#define IPPSTK_CAUSE_MAX_LINES_OUT_OF_RANGE ICF_CAUSE_MAX_LINES_OUT_OF_RANGE
 
/* Indicates that the line_id received in any of the configuration API is out of
 * the range 1 to max_lines */
#define IPPSTK_CAUSE_INVALID_LINE_ID ICF_CAUSE_INVALID_LINE_ID
 
/* Indicates that max_audio_calls received in API IPPSTK_CFG_CC_SET_SCALE_PARAMS
 * is 0 */
#define IPPSTK_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE ICF_CAUSE_MAX_AUDIO_CALLS_OUT_OF_RANGE
 
/* Indicates that max_registration_calls received in API IPPSTK_CFG_CC_SET_SCALE
 * _PARAMS is 0 */
#define IPPSTK_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE ICF_CAUSE_MAX_REGISTRATION_CALLS_OUT_OF_RANGE
 
/* Indicates that max_users_per_line received in API 
 * IPPSTK_CFG_CC_SET_SCALE_PARAMS is 0 */
#define IPPSTK_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE ICF_CAUSE_MAX_USERS_PER_LINE_OUT_OF_RANGE
 
/* Indicates that because of new registrar added or the new user added to a 
 * line, the number of registrations active exceeds he configured value of 
 * max_registration_calls */
#define IPPSTK_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED ICF_CAUSE_MAX_REGISTRATION_CALLS_EXCEEDED
 
/* Indicates that the because of new user configured, the value of users on one
 *  of a line has exceeded configured value of max_users_per_line */
#define IPPSTK_CAUSE_MAX_USERS_PER_LINE_EXCEEDED ICF_CAUSE_MAX_USERS_PER_LINE_EXCEEDED
 
/* Indicates that because of a new service request the number of active services
 * has exceeded configured value of max_service_calls */
#define IPPSTK_CAUSE_MAX_SERVICE_CALLS_EXCEEDED ICF_CAUSE_MAX_SERVICE_CALLS_EXCEEDED
 
/* Indicates a generic cause for invalid action requested by a API */
#define IPPSTK_CAUSE_INVALID_ACTION ICF_CAUSE_INVALID_ACTION
 
/* Indicates that invalid address is received in PAI from PA */
#define IPPSTK_CAUSE_INVALID_ADDRESS_RECEIVED ICF_CAUSE_INVALID_ADDRESS_RECEIVED
 
/* Indicates that num_of_calls_to_log in API IPPSTK_CFG_CC_SET_CALL_PARAMS
 * is greater than MAX_NUM_OF_CALLS_TO_LOG */
#define IPPSTK_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE ICF_CAUSE_MAX_CALLS_TO_LOG_OUT_OF_RANGE

/* Indicates that the line corresponding to a given line id is not yet 
 * configured */
#define IPPSTK_CAUSE_LINE_NOT_CONFIGURED ICF_CAUSE_LINE_NOT_CONFIGURED

/* Indicates that self_address cannot be configured in domain name format */
#define IPPSTK_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED ICF_CAUSE_SELF_ADDR_DOMAIN_NAME_NOT_HANDLED

/* Indicates that timer value is not valid. Usually used when timer duration is
 * configured as 0 */
#define IPPSTK_CAUSE_INVALID_TIMER_VALUE ICF_CAUSE_INVALID_TIMER_VALUE

/* Indicates that the specified configuration action cannot be performed during
 * registration or deregistration is ongoing for that user and registrar */
#define IPPSTK_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG ICF_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG

/*ICF_CRM_START*/
/* Indicates that the network access type is invalid */
#define IPPSTK_CAUSE_INVALID_ACCESS_TYPE ICF_CAUSE_INVALID_ACCESS_TYPE

/* Indicates that the network access info is invalid */
#define IPPSTK_CAUSE_INVALID_ACCESS_INFO ICF_CAUSE_INVALID_ACCESS_INFO

/* Indicates that application has issued a request in duplicate */
#define IPPSTK_CAUSE_DUPLICATE_REQUEST ICF_CAUSE_DUPLICATE_REQUEST

/* Indicates that network activate request has been denied to the 
 * application */
#define IPPSTK_CAUSE_ACTIVATE_DENIED ICF_CAUSE_ACTIVATE_DENIED

/* Indicates that network activate request cannot be granted as maximum  
 * number of networks are already activated */
#define IPPSTK_CAUSE_MAX_NETWORK_EXCEEDED ICF_CAUSE_MAX_NETWORK_EXCEEDED

/* Indicates that network activate request cannot be granted to this 
 * application as maximum number of applications are already using this network */
#define IPPSTK_CAUSE_MAX_APP_EXCEEDED ICF_CAUSE_MAX_APP_EXCEEDED

/* Indicates that network could not be activated */
#define IPPSTK_CAUSE_ACTIVATION_FAILURE ICF_CAUSE_ACTIVATION_FAILURE

/* Indicates that application add request cannot be granted to this 
 * application as maximum number of applications already have their context in the system */
#define IPPSTK_CAUSE_MAX_NUM_APPLICATION_EXCEEDED ICF_CAUSE_MAX_NUM_APPLICATION_EXCEEDED

/* Indicates that network could not be de-activated */
#define IPPSTK_CAUSE_DEACTIVATION_FAILURE ICF_CAUSE_DEACTIVATION_FAILURE

/* Indicates that context is invalid */
#define IPPSTK_CAUSE_INVALID_CONTEXT ICF_CAUSE_INVALID_CONTEXT

/* Indicates that application ID is invalid */
#define IPPSTK_CAUSE_INVALID_APP_ID ICF_CAUSE_INVALID_APP_ID

/* Indicates that call type in application add request is invalid */
#define IPPSTK_CAUSE_INVALID_CALL_TYPE ICF_CAUSE_INVALID_CALL_TYPE

/* Indicates that routing info is invalid(non-UDP) */
#define IPPSTK_CAUSE_INVALID_ROUTING_INFO ICF_CAUSE_INVALID_ROUTING_INFO

/* Indicates that given application ID is not entitled to call network de-activation */
#define IPPSTK_CAUSE_ILLEGAL_REQUEST ICF_CAUSE_ILLEGAL_REQUEST

/* Indicates that the context is not activated because it is 
also being used by some other application */
#define IPPSTK_CAUSE_CONTEXT_BUSY ICF_CAUSE_CONTEXT_BUSY

/* Indicates that the version ID is invalid */
#define IPPSTK_CAUSE_INVALID_VERSION_ID ICF_CAUSE_INVALID_VERSION_ID

/* Indicates that the source ID is invalid */
#define IPPSTK_CAUSE_INVALID_SOURCE_ID ICF_CAUSE_INVALID_SOURCE_ID

/* Indicates that the destination ID is invalid */
#define IPPSTK_CAUSE_INVALID_DESTINATION_ID ICF_CAUSE_INVALID_DESTINATION_ID

/* ICF_CONF_STATUS_API_START */
/* Indicates that network is not available at the moment */
#define IPPSTK_CAUSE_NW_CURRENTLY_NOT_AVAILABLE ICF_CAUSE_NW_CURRENTLY_NOT_AVAILABLE
/* ICF_CONF_STATUS_API_END */

/* Indicates that the destination ID is invalid */
#define IPPSTK_CAUSE_INTERNAL_FAILURE ICF_CAUSE_INTERNAL_FAILURE


/* To indicate that SA creation failed or timed-out */
#define IPPSTK_CAUSE_SA_CREATION_FAILURE      ICF_CAUSE_SA_CREATION_FAILURE

/* To indicate failure for REG req when old and new SAs exist - limitation */
#define IPPSTK_CAUSE_MULTIPLE_SA_EXIST        ICF_CAUSE_MULTIPLE_SA_EXIST

/* To indicate general IPSec failure */
#define IPPSTK_CAUSE_IPSEC_FAILURE            ICF_CAUSE_IPSEC_FAILURE


#define IPPSTK_CAUSE_INCONSISTENT_TRANSPORT_MODE ICF_CAUSE_INCONSISTENT_TRANSPORT_MODE

#define IPPSTK_CAUSE_CALL_MERGE_FAIL          ICF_CAUSE_CALL_MERGE_FAIL	

#ifdef IPPSTK_SERVICE_ENABLE

#define IPPSTK_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED	ICF_CAUSE_ADDR_FOR_SERV_NOT_CONFIGURED	
#define IPPSTK_CAUSE_SERVICE_NOT_SUBSCRIBED			ICF_CAUSE_SERVICE_NOT_SUBSCRIBED
#endif
/* To indicate that list of supported headers in app_add_request was empty */
#define IPPSTK_CAUSE_EMPTY_SUPP_HDR_LIST      ICF_CAUSE_EMPTY_SUPP_HDR_LIST
    
/* To indicate the Media change request is not acceptable */
#define IPPSTK_CAUSE_REJECT_MEDIA_CHANGE_REQ  ICF_CAUSE_REJECT_MEDIA_CHANGE_REQ

/*ICF_CRM_END*/
/* -------------------------------------------------------------------------
 * #defines of Management Plane handler   error ids
 * ------------------------------------------------------------------------*/

/* caused when an invalid api_id is passed to ippstk_mph_msg_handler */

#define IPPSTK_CAUSE_INVALID_ACTION				ICF_CAUSE_INVALID_ACTION
	
/* caused when a timer expiry mesg is received from MRM by 
ippstk_mph_process_msg against no response timer */

#define IPPSTK_CAUSE_NO_RESP_FROM_SERVER		ICF_CAUSE_NO_RESP_FROM_SERVER

/* caused when no empty application node exists in mph_data to 
support a new entry */

#define IPPSTK_CAUSE_MAX_APPLICATION_EXCEEDED	ICF_CAUSE_MAX_APPLICATION_EXCEEDED

/* caused when no empty server node exists against an application in 
mph_data to support a new connection/server entry */

#define IPPSTK_CAUSE_MAX_CONNECTION_EXCEEDED	ICF_CAUSE_MAX_CONNECTION_EXCEEDED

/* caused when application tries to overwrite auth data against 
a server connection with pending request */

#define IPPSTK_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA	ICF_CAUSE_REQ_PENDING_ON_CURRENT_AUTH_DATA

/* caused when application tries to remove server data which is not actually present */

#define IPPSTK_CAUSE_SERVER_INFO_MISSING		ICF_CAUSE_SERVER_INFO_MISSING

/* caused when all pending/ongoing requests against a server 
	connection are failed when the application issues ippstk_mph_set_auth_param_hdlr 
	to remove server data */
	
#define IPPSTK_CAUSE_SERVER_INFO_REMOVED		ICF_CAUSE_SERVER_INFO_REMOVED

/* caused when connection creation fails */

#define IPPSTK_CAUSE_CONNECTION_FAILURE			ICF_CAUSE_CONNECTION_FAILURE

/* caused when connection already exists and application issues 
connection create again */

#define IPPSTK_CAUSE_CONNECTION_ALREADY_EXISTS	ICF_CAUSE_CONNECTION_ALREADY_EXISTS 

/* caused when there is an error related to the connection closure from the port side */
	
#define IPPSTK_CAUSE_FROM_PORT					ICF_CAUSE_FROM_PORT

/* caused when connection closure type bit is anything else except CLOSE or ABORT */
	
#define IPPSTK_CAUSE_INVALID_CONN_CLOSURE_TYPE	ICF_CAUSE_INVALID_CONN_CLOSURE_TYPE

/* caused when send_http_req API of the port fails due to any reason */

#define IPPSTK_CAUSE_PORT_SEND_HTTP_REQ			ICF_CAUSE_PORT_SEND_HTTP_REQ
	
/* caused when make_http_req API of the port fails, i.e. it is unable to 
   form a valid HTTP request */

#define IPPSTK_CAUSE_PORT_MAKE_HTTP_REQ			ICF_CAUSE_PORT_MAKE_HTTP_REQ
	
/* caused when mandatory SyncML headers are not present in the HTTP message */

#define IPPSTK_CAUSE_SYNCML_HEADERS_NOT_PRESENT	ICF_CAUSE_SYNCML_HEADERS_NOT_PRESENT
	
/* caused when connection id does not exist */

#define IPPSTK_CAUSE_INVALID_CONNECTION_ID		ICF_CAUSE_INVALID_CONNECTION_ID

/* caused when mandatory headers (eg req_uri), required as a must to form a 
HTTP request are not present */

#define IPPSTK_CAUSE_MANDATORY_HEADERS_NOT_PRESENT	ICF_CAUSE_MANDATORY_HEADERS_NOT_PRESENT

/* caused when connection create is re-attempted after a connection close 
indication from peer */

#define IPPSTK_CAUSE_CONNECTION_REATTEMPT_FAILURE	ICF_CAUSE_CONNECTION_REATTEMPT_FAILURE

/* caused when WWW-Authenticate header is not present in 401 from server */

#define IPPSTK_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401	ICF_CAUSE_WWW_AUTHENTICATE_NOT_PRESENT_IN_401

/* caused when stale bit is set to false in 401	*/

#define IPPSTK_CAUSE_INVALID_USERNAME_PASSWD	ICF_CAUSE_INVALID_USERNAME_PASSWD

/* caused when ippstk_mph_make_network_resp() retirns failure */
#define IPPSTK_CAUSE_NW_RESP_FAILED		ICF_CAUSE_NW_RESP_FAILED

/* caused when auth parameters have not been configured and server sends 401 */

#define IPPSTK_CAUSE_AUTH_DATA_NOT_PRESENT		ICF_CAUSE_AUTH_DATA_NOT_PRESENT

/* caused when auth scheme is not present in 401 */

#define IPPSTK_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401	ICF_CAUSE_UNKNOWN_AUTH_SCHEME_IN_401
	
/* caused when realm is not present in 401 */

#define IPPSTK_CAUSE_REALM_NOT_PRESENT_IN_401	ICF_CAUSE_REALM_NOT_PRESENT_IN_401

/* caused when nonce is not present in 401 */

#define IPPSTK_CAUSE_NONCE_NOT_PRESENT_IN_401	ICF_CAUSE_NONCE_NOT_PRESENT_IN_401

/* caused when all pending/ongoing requests against an application are 
failed when the application issues application remove */

#define IPPSTK_CAUSE_APPLICATION_REMOVED	ICF_CAUSE_APPLICATION_REMOVED

/* caused when all pending/ongoing requests against an application are 
failed when network gets deactivated */

#define IPPSTK_CAUSE_NETWORK_DEACTIVATED	ICF_CAUSE_NETWORK_DEACTIVATED

/* caused when all pending/ongoing requests against an application are 
failed when the application adds itself again */

#define IPPSTK_CAUSE_APPLICATION_ADDED_AGAIN	ICF_CAUSE_APPLICATION_ADDED_AGAIN

/* caused when pending requests on a connection are failed due to conn close req */

#define IPPSTK_CAUSE_CLOSE_CONN_REQ			ICF_CAUSE_CLOSE_CONN_REQ

/* caused when connection close fails */

#define IPPSTK_CAUSE_CLOSE_CONN_FAILURE		ICF_CAUSE_CLOSE_CONN_FAILURE

#define IPPSTK_CAUSE_MAKE_AUTHORISATION_FAILED	ICF_CAUSE_MAKE_AUTHORISATION_FAILED

#define IPPSTK_CAUSE_INVALID_CONNECTION_USAGE	ICF_CAUSE_INVALID_CONNECTION_USAGE

/* ------------------------------------------------------------------------------------
 * The below mentioned error causes that were used by IPPSTK have been deprecated
 * in ICF
 */

#define IPPSTK_MISC_CAUSES_START				IPPSTK_CAUSE_ACTION_NOT_ALLWD_DURING_REG_ONG

/* Scale parameters are not configured and before that some other configuration
 *  API is received */
#define IPPSTK_CAUSE_SCALE_PARAMS_NOT_CONFIGURED IPPSTK_MISC_CAUSES_START + 1

/* Indicates that IPPSTK_CFG_CC_SET_SCALE_PARAMS is already received earlier and
 * cannot be handled twice */
#define IPPSTK_CAUSE_SCALE_PARAMS_ALREADY_CONFIGURED  IPPSTK_MISC_CAUSES_START + 2

/* Indicates that INFO request from PA cannot be handled because there is 
 * already one ongoing INFO request for which response is awaited from network*/
#define IPPSTK_CAUSE_INFO_ALREADY_PENDING		IPPSTK_MISC_CAUSES_START + 3

/* Indicates that INFO request send on network has been rejected */
#define IPPSTK_CAUSE_INFO_REJECTED_BY_REMOTE	IPPSTK_MISC_CAUSES_START + 4	

/* Indicates that timebased licence violation has been detected and the user is
 * not authorised to use the software */
#define IPPSTK_CAUSE_LICENCE_VIOLATED			IPPSTK_MISC_CAUSES_START + 5

#define IPPSTK_MISC_CAUSES_END					IPPSTK_CAUSE_LICENCE_VIOLATED

/*------------------------------------------------------------------------------------*/

#define IPPSTK_CAUSE_EXTERNAL_MAX_LIMIT ICF_CAUSE_EXTERNAL_MAX_LIMIT

#define IPPSTK_CAUSE_FILTER_PARSE_FAILED ICF_CAUSE_FILTER_PARSE_FAILED


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for ippstk_error_id.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/



/* No error */
#define IPPSTK_ERROR_NONE   0
    

/*******************************************************************************
 *
 * Error ids used in API IPPSTK_CC_SM_REPORT_ERROR_IND
 *
*******************************************************************************/


/* Start of Error id */
#define IPPSTK_ERROR_START                             ICF_ERROR_START

/* invalid error id */
#define IPPSTK_ERROR_UNDEFINED                         ICF_ERROR_UNDEFINED

/* memory allocation failure */
#define IPPSTK_ERROR_MEM_ALLOC_FAILURE                 ICF_ERROR_MEM_ALLOC_FAILURE

/* memory free failure */
#define IPPSTK_ERROR_MEM_FREE_FAILURE                  ICF_ERROR_MEM_FREE_FAILURE

/* timer start failure */
#define IPPSTK_ERROR_TMR_START_FAILURE                 ICF_ERROR_TMR_START_FAILURE

/* timer stop failure */
#define IPPSTK_ERROR_TMR_STOP_FAILURE                  ICF_ERROR_TMR_STOP_FAILURE

/* API with invalid id received */
#define IPPSTK_ERROR_INVALID_API_ID                    ICF_ERROR_INVALID_API_ID

/* call failed because peer SDP was never received */
#define IPPSTK_ERROR_CALL_FAIL_NO_SDP                  ICF_ERROR_CALL_FAIL_NO_SDP

/*address validation fails for an outgoing call*/
#define IPPSTK_ERROR_INVALID_TRNSPT_ADDR			   ICF_ERROR_INVALID_TRNSPT_ADDR

/*Porting module's receive failure*/
#define IPPSTK_MSG_RECEIVE_ERROR                       ICF_MSG_RECEIVE_ERROR

/*Error for invalid source id is received*/
#define IPPSTK_INVALID_SRC_MODULE_ID                   ICF_INVALID_SRC_MODULE_ID
    
/*Error for failure in Memory Mgr Init*/
#define IPPSTK_ERROR_MEM_INIT                          ICF_ERROR_MEM_INIT
    
/*Error for failure in Memory Mgr DeInit*/
#define IPPSTK_ERROR_MEM_DEINIT                        ICF_ERROR_MEM_DEINIT
    
/*Error for failure in Timer Mgr Init*/
#define IPPSTK_ERROR_TMR_INIT                          ICF_ERROR_TMR_INIT
    
/*Error for failure in init time FIFO creation*/
#define IPPSTK_ERROR_FIFO_CREATION                     ICF_ERROR_FIFO_CREATION
    
/*Error for failure in UDP msg TX*/
#define IPPSTK_ERROR_UDP_SEND                          ICF_ERROR_UDP_SEND
    
/*Error for failure in TCP msg TX*/
#define IPPSTK_ERROR_TCP_SEND                          ICF_ERROR_TCP_SEND
    
/*Error for failure in FIFO msg TX*/
#define IPPSTK_ERROR_FIFO_SEND                         ICF_ERROR_FIFO_SEND
    
/*Error for failure socket call*/
#define IPPSTK_ERROR_SOCKET_CREATE                     ICF_ERROR_SOCKET_CREATE
    
/*Error for failure in socket bind*/
#define IPPSTK_ERROR_SOCKET_BIND                       ICF_ERROR_SOCKET_BIND
    
/*Error for failure in set socket*/
#define IPPSTK_ERROR_SET_SOCKET                        ICF_ERROR_SET_SOCKET
    
/*Error for failure in UDP recv */
#define IPPSTK_ERROR_UDP_RECV                          ICF_ERROR_UDP_RECV
    
/*Error for failure in FIFO recv */
#define IPPSTK_ERROR_FIFO_RECV                         ICF_ERROR_FIFO_RECV

/* Matching MM AL context not found */
#define IPPSTK_CAUSE_MM_AL_CONTEXT_NOT_FOUND           ICF_CAUSE_MM_AL_CONTEXT_NOT_FOUND

/* Free MM AL context not available */
#define IPPSTK_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND       ICF_CAUSE_MM_AL_NO_FREE_CONTEXT_FOUND

/* AL memory allocation failed */
#define IPPSTK_CAUSE_MM_AL_MEMORY_FAILURE              ICF_CAUSE_MM_AL_MEMORY_FAILURE

/* No valid codec returned by RM */
#define IPPSTK_MM_AL_NO_VALID_CODEC_RETURNED           ICF_MM_AL_NO_VALID_CODEC_RETURNED
 
/* Unknown request from MMI */
#define IPPSTK_MM_AL_UNKNOWN_REQUEST_FROM_MMI          ICF_MM_AL_UNKNOWN_REQUEST_FROM_MMI

/* Unknown request from MMI */
#define IPPSTK_MM_AL_UNKNOWN_MESSAGE_FROM_RM           ICF_MM_AL_UNKNOWN_MESSAGE_FROM_RM

/* Unknown request from MMI */
#define IPPSTK_MM_AL_UNKNOWN_MESSAGE_FROM_MM           ICF_MM_AL_UNKNOWN_MESSAGE_FROM_MM

/* All call context block in DBM pool used up*/
#define IPPSTK_CALL_CTX_BLK_EXHAUSTED                  ICF_CALL_CTX_BLK_EXHAUSTED

/* All service context block in DBM pool used up*/
#define IPPSTK_SRVC_CTX_BLK_EXHAUSTED                  ICF_SRVC_CTX_BLK_EXHAUSTED

/* All service logic context block in DBM pool used up*/
#define IPPSTK_SRVC_CTX_LGC_BLK_EXHAUSTED              ICF_SRVC_CTX_LGC_BLK_EXHAUSTED

/* All registration context block in DBM pool used up*/
#define IPPSTK_RGN_CTX_BLK_EXHAUSTED                   ICF_RGN_CTX_BLK_EXHAUSTED

/* No data for the specified module available in DBM's pool*/    
#define IPPSTK_NO_MODULE_DATA_AVAILABLE                ICF_NO_MODULE_DATA_AVAILABLE

/*Call context does not exist in global pdb*/
#define IPPSTK_NO_CALL_CTX_EXIST                       ICF_NO_CALL_CTX_EXIST

/*There is no service context in call context*/
#define IPPSTK_NO_SRVC_CTX_EXIST                       ICF_NO_SRVC_CTX_EXIST

/*No registration context available for specified user*/
#define IPPSTK_NO_RGN_CTX_EXIST                        ICF_NO_RGN_CTX_EXIST
  
/*If user is not in the list of users on the line*/    
#define IPPSTK_USER_NOT_ADDED                          ICF_USER_NOT_ADDED

/*For duplicate registration request*/    
#define IPPSTK_DUPLICATE_USER                          ICF_DUPLICATE_USER
 
/*Util function for linked array initializtion returns failure*/    
#define IPPSTK_LA_INIT_FAILURE                         ICF_LA_INIT_FAILURE

/* There is no field for the specified type in any structure/union */
#define IPPSTK_MSG_PROC_FIELD_TYPE_MISMATCH            ICF_MSG_PROC_FIELD_TYPE_MISMATCH

/* Invalid operator used for message checking */
#define IPPSTK_MSG_PROC_OPR_MISMATCH                   ICF_MSG_PROC_OPR_MISMATCH

/* Would be returned by DBM when there were no blocks left in pool*/
#define IPPSTK_POOL_EXHAUSTED						   ICF_POOL_EXHAUSTED

/* -------------------------------------------------------------------------
 * Hash defined values of the error ids for REGM module
 * ------------------------------------------------------------------------*/

/*Following is generated by REGM on failure at 
 * registration process at INIT COMPLETE*/
#define IPPSTK_INIT_COMPLETE_FAILED                    ICF_INIT_COMPLETE_FAILED

/*Modification of User Address API FAILED */
#define IPPSTK_MODIFY_USER_FAILED                      ICF_MODIFY_USER_FAILED

/*Processing of Modify User Address Block FAILED*/
#define IPPSTK_PROCESS_MODIFY_USER_BLK_FAILED          ICF_PROCESS_MODIFY_USER_BLK_FAILED

/*Addition/Deletion of Registrar Address  API FAILED*/
#define IPPSTK_MODIFY_REGISTRAR_FAILED                 ICF_MODIFY_REGISTRAR_FAILED

/*Processing of ADD/DELETE Registrar Address Block FAILED*/
#define IPPSTK_PROCESS_MODIFY_REG_BLK_FAILED           ICF_PROCESS_MODIFY_REG_BLK_FAILED

/*Modification of Registration Duration API FAILED*/
#define IPPSTK_MODIFY_DURATION_FAILED                  ICF_MODIFY_DURATION_FAILED

/*Processing of Registrar Duration Modification Block FAILED*/
#define IPPSTK_PROCESS_MODIFY_DUR_BLK_FAILED           ICF_PROCESS_MODIFY_DUR_BLK_FAILED
   
/*Authentication Information is NOT Available */
#define IPPSTK_AUTHENTICATION_INFO_ABSENT              ICF_AUTHENTICATION_INFO_ABSENT

/*Invalid Network Response Received from SSA */   
#define IPPSTK_INVALID_NETWORK_RESPONSE                ICF_INVALID_NETWORK_RESPONSE

/*Unable To Send Registration Request: SSA Returned FAILURE*/
#define IPPSTK_UNABLE_TO_SEND_REG_REQ                  ICF_UNABLE_TO_SEND_REG_REQ

/*Unable To Send de-Registration Request: SSA Returned FAILURE*/
#define IPPSTK_UNABLE_TO_SEND_DEREG_REQ                ICF_UNABLE_TO_SEND_DEREG_REQ


/* Message processing fails since Init Complete has not occurred */
#define IPPSTK_MSG_PROC_INIT_NOT_COMPLETE              ICF_MSG_PROC_INIT_NOT_COMPLETE

/* Terminal failure in SSA :Call should be cleared            */
#define IPPSTK_TERMINAL_FAILURE_CALL_CLEAR             ICF_TERMINAL_FAILURE_CALL_CLEAR

/* Failure in sending response to an external module */
#define IPPSTK_RESPONSE_SEND_FAILURE                   ICF_RESPONSE_SEND_FAILURE

/* invalid internal message id received by a toolkit module */
#define IPPSTK_INVALID_MSG_ID_RECD                     ICF_INVALID_MSG_ID_RECD

/* invalid external api id received by a toolkit module */
#define IPPSTK_INVALID_API_ID_RECD                     ICF_INVALID_API_ID_RECD
    

/* -------------------------------------------------------------------------
 * #defines of SCM  error ids
 * ------------------------------------------------------------------------*/
/* SCM Error ID Start */
#define IPPSTK_ERROR_START_SCM                         ICF_ERROR_START_SCM
/* Service Request recvd for unsubscribed service */
#define IPPSTK_ERROR_SERVICE_NOT_SUBSCRIBED            ICF_ERROR_SERVICE_NOT_SUBSCRIBED
/* Service Request recvd for inactive service */
#define IPPSTK_ERROR_SERVICE_NOT_ACTIVE                ICF_ERROR_SERVICE_NOT_ACTIVE
/* Service Request failed in Service Interaction Rules */
#define IPPSTK_ERROR_INVALID_SERVICE_INTERACTION       ICF_ERROR_INVALID_SERVICE_INTERACTION
/* SCM was unable to find a Service Logic Handler for the recvd API */
#define IPPSTK_ERROR_NO_API_HANDLER                    ICF_ERROR_NO_API_HANDLER
/* SCM was unable to route the message as the Handler Service Logic
is not running */
#define IPPSTK_ERROR_SL_NOT_RUNNING                    ICF_ERROR_SL_NOT_RUNNING
/* Invalid Payload recvd in API*/
#define IPPSTK_ERROR_INV_PAYLOAD_RECVD                 ICF_ERROR_INV_PAYLOAD_RECVD
/* Call waiting request was not validated */
#define IPPSTK_ERROR_CALL_WAITING_FAILURE             ICF_ERROR_CALL_WAITING_FAILURE
/* SCM Error ID End */
#define IPPSTK_ERROR_END_SCM                         ICF_ERROR_END_SCM


 /*-----------------------------------------------------------------
  * #defines for Error_id raised by SM
  * --------------------------------------------------------------*/
#define IPPSTK_ERROR_START_SM               ICF_ERROR_START_SM
#define IPPSTK_ERROR_INVALID_PARAM          ICF_ERROR_INVALID_PARAM
#define IPPSTK_ERROR_CALL_LOGGING_FAILURE   ICF_ERROR_CALL_LOGGING_FAILURE
#define IPPSTK_ERROR_SYSCALL                ICF_ERROR_SYSCALL
#define IPPSTK_ERROR_LIC_EXPIRED            ICF_ERROR_LIC_EXPIRED
#define IPPSTK_ERROR_LIC_INTEGRTY           ICF_ERROR_LIC_INTEGRTY
#define IPPSTK_ERROR_END_SM                 ICF_ERROR_END_SM

    /*-----------------------------------------------------------------
     * #defines for error IDs raised by CFG 
     * ----------------------------------------------------------------*/
    
/* Error for failure returned by init nw interface during CFG Init complete */
#define IPPSTK_ERROR_NW_INTERFACE   ICF_ERROR_NW_INTERFACE
/* Error for failure returned by RGM Init Complete during CFG Init complete */
#define IPPSTK_ERROR_RGM_INIT_CMPLT ICF_ERROR_RGM_INIT_CMPLT
/* Error for failure returned in getting DBM port data during CFG Init complete
 *  */
#define IPPSTK_ERROR_DBM_PORT_DATA  ICF_ERROR_DBM_PORT_DATA
/* Error for failure returned by ES init complete during CFG Init complete */
#define IPPSTK_ERROR_ES_INIT_CMPLT  ICF_ERROR_ES_INIT_CMPLT

/* Error if all conditions to perform init complete are not met */
#define IPPSTK_ERROR_INIT_CMPLT  ICF_ERROR_INIT_CMPLT

/* Error if there is any UATK failure */
#define IPPSTK_ERROR_UATK_FAILURE   ICF_ERROR_UATK_FAILURE

/*Error to indicate to IPPTK that a non blocking connect call 
 * returned E_INPROGRESS*/
#define IPPSTK_CONNECT_RESP_PENDING  ICF_CONNECT_RESP_PENDING

/* Error code when some failure occurs in UATK processing */
#define IPPSTK_ERROR_UATK_FAILURE		ICF_ERROR_UATK_FAILURE

/*Error to indicate to IPPTK that a non blocking connect call
 * returned E_INPROGRESS*/
#define IPPSTK_CONNECT_RESP_PENDING		ICF_CONNECT_RESP_PENDING

    /* Add last ecode */
#define IPPSTK_MAX_NUM_OF_ERROR_IDS        ICF_MAX_NUM_OF_ERROR_IDS




/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for ippstk_api_id.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------
 * The follwoing definitions list the API ids for various Interfaces to
 * or from SIP Packet Phone Signaling Toolkit.
 * ----------------------------------------------------------------------*/


#define IPPSTK_API_ID_UNDEFINED                  ICF_API_ID_INVALID

/* Start of API id */
#define IPPSTK_EXTERNAL_API_START                ICF_EXTERNAL_API_START

/*********** APIs for MPH ***********/

#define IPPSTK_SET_MGMT_PLANE_AUTH_PARAM		ICF_SET_MGMT_PLANE_AUTH_PARAM

#define IPPSTK_CREATE_SECURE_CONN_REQ			ICF_CREATE_SECURE_CONN_REQ  

#define IPPSTK_CREATE_SECURE_CONN_RESP			ICF_CREATE_SECURE_CONN_RESP 

#define IPPSTK_CLOSE_SECURE_CONN_REQ			ICF_CLOSE_SECURE_CONN_REQ	

#define IPPSTK_CLOSE_SECURE_CONN_RESP			ICF_CLOSE_SECURE_CONN_RESP

#define IPPSTK_SECURE_CONN_STATUS_IND			ICF_SECURE_CONN_STATUS_IND

#define IPPSTK_XCAP_CREATE_REPLACE_RES			ICF_XCAP_CREATE_REPLACE_RES 

#define IPPSTK_XCAP_DELETE_RES					ICF_XCAP_DELETE_RES	

#define IPPSTK_XCAP_FETCH_RES					ICF_XCAP_FETCH_RES

#define IPPSTK_SYNCML_SEND_REQ					ICF_SYNCML_SEND_REQ	

#define IPPSTK_MGMT_PLANE_API_PROC_FAILURE		ICF_MGMT_PLANE_API_PROC_FAILURE 

#define IPPSTK_MGMT_PLANE_NW_RESP				ICF_MGMT_PLANE_NW_RESP

/********** APIs for MPH **********/
	
    /* The following APIs are from Configuration Module to SIP Packet Phone
 * Signaling Toolkit and also from SIP Packet Phone Signaling Toolkit to
 * Configuration Module. */

#define IPPSTK_CFG_API_START                     ICF_CFG_API_START

/*ICF_CRM_START*/
/* API for network activation request */

#define IPPSTK_NETWORK_ACTIVATE_REQ              ICF_NETWORK_ACTIVATE_REQ

/* API for network activation response */

#define IPPSTK_NETWORK_ACTIVATE_CFM              ICF_NETWORK_ACTIVATE_CFM

/* API for network de-activation request */

#define IPPSTK_NETWORK_DEACTIVATE_REQ            ICF_NETWORK_DEACTIVATE_REQ

/* API for network de-activation response */

#define IPPSTK_NETWORK_DEACTIVATE_CFM            ICF_NETWORK_DEACTIVATE_CFM

/* API for application add request */

#define IPPSTK_APP_ADD_REQ                       ICF_APP_ADD_REQ

/* API for application add response */

#define IPPSTK_APP_ADD_CFM                       ICF_APP_ADD_CFM

/* API for application remove request */

#define IPPSTK_APP_REMOVE_REQ                    ICF_APP_REMOVE_REQ

/* API for application remove response */

#define IPPSTK_APP_REMOVE_CFM                    ICF_APP_REMOVE_CFM

/* API for  unavailable indication to application */

#define IPPSTK_RES_UNAVAIL_IND                   ICF_RES_UNAVAIL_IND
		
/* API for event indication */

#define IPPSTK_EVENT_INDICATION                  ICF_EVENT_INDICATION
/*ICF_CRM_END*/


/* This API is used to configure transport related parameters like proxy 
 * address per line, self IP address and port on which network (SIP) 
 * messages will be received/Sent. */

#define IPPSTK_CFG_CC_SET_TRANSPORT_PARAMS_REQ   ICF_SET_TRANSPORT_REQ

/* This API is used to configure system wide call related parameters like
 * timer values, call logging information, handling of SIP procedures etc. */

#define IPPSTK_CFG_CC_SET_CALL_PARAMS_REQ        ICF_SET_CALL_PARAM_REQ

/* This API is used to configure user identity related parameters like 
 * user address and user name. It is configured against each line id. */

#define IPPSTK_CFG_CC_SET_SELF_ID_PARAMS_REQ     ICF_SET_SELF_ID_REQ

#define IPPSTK_CFG_CC_SET_SERVICE_PARAMS_REQ     ICF_SET_SERVICE_PARAMS_REQ

/* This API is used to configure registration related parameters like 
 * registrar address, registration duration, protocol(TCP/UDP) for each 
 * registrar etc. */

#define IPPSTK_CFG_CC_SET_REGISTRATION_PARAMS_REQ ICF_SET_REGISTRATION_REQ

/* This is an API used by SIP Packet Phone Signaling Toolkit to respond 
 * back to Configuration Module for any API from Configuration Module. */

#define IPPSTK_CC_CFG_SET_CONFIG_PARAM_RESP      ICF_CONFIGURE_CFM

/* ICF_NW_RECOVERY_START */
/* New API from APP to ICF to fetch the configured system settings */
#define IPPSTK_GET_SYSTEM_SETTINGS_REQ 	 ICF_GET_SYSTEM_SETTINGS_REQ

/* New API from APP to ICF to fetch the configured line settings */
#define IPPSTK_GET_LINE_SETTINGS_REQ 	 ICF_GET_LINE_SETTINGS_REQ
/* ICF_NW_RECOVERY_END */

#define IPPSTK_CFG_API_END                       ICF_CFG_API_END

/* Following APIs are sent from SM to SIP Packet Phone Signaling Toolkit to
 * enable certain system management processing. */

#define IPPSTK_SM_API_START                      ICF_SM_API_START

/* This API is sent by system administration module to enable or disable 
 * the reporting of traces of a specific level or type, during system 
 * runtime. There is no response for this message. */

#define IPPSTK_SM_CC_SET_TRACE_LEVEL_REQ         ICF_SET_TRACE_LEVEL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to report a 
 * trace of given level and type.  */

#define IPPSTK_CC_SM_REPORT_TRACE_IND            ICF_REPORT_TRACE_IND

/* This API is sent by system administration module to enable or disable 
 * the reporting of errors of a specific level or type, during system 
 * runtime. There is no response for this message. */

#define IPPSTK_SM_CC_SET_ERROR_REPORT_LEVEL_REQ  ICF_SET_ERROR_REPORT_LEVEL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to report an 
 * error of given level and type.  */

#define IPPSTK_CC_SM_REPORT_ERROR_IND            ICF_REPORT_ERROR_IND

/* This API is sent by system administration module to enable or disable 
 * the updation of a specific stats or stats group. It also specifies whether 
 * the current stats values in the counters needs to be resetor not. There 
 * is no response for this message. */

#define IPPSTK_SM_CC_SET_STATS_REQ               ICF_SET_STATS_REQ

/* This API is sent by system administration module to get the current value 
 * of a specific stats or stats group. It also specifies whether the current
 * stats values in the counters needs to be reset or not. The response to 
 * this API contains the stats value. */

#define IPPSTK_SM_CC_GET_STATS_REQ               ICF_SM_GET_STATS_REQ

/* The API contains the response for IPPSTK_SM_CC_GET_STATS_REQ to 
 * indicate the current value of the requested stats. */

#define IPPSTK_CC_SM_GET_STATS_RESP              ICF_SM_GET_STATS_RESP

/* This API is sent by the Syatem Manager to check the health of the 
 * SIP Packet Phone Signaling Toolkit at regular intervals. */

#define IPPSTK_SM_CC_CHECK_MODULE_STATUS_REQ     ICF_CHECK_MODULE_STATUS_REQ

/* This API is sent in response tp IPPSTK_SM_CC_CHECK_MODULE_STATUS_REQ to
 * indicate that SIP Packet Phone Signaling Toolkit is alive. */

#define IPPSTK_CC_SM_CHECK_MODULE_STATUS_RESP    ICF_CHECK_MODULE_STATUS_RESP

#define IPPSTK_SM_API_END                        ICF_SM_API_END

/* The following are list of APIs used to indicate the APIs from PA to 
 * SIP Packet Phone Signaling Toolkit and back. */

#define IPPSTK_PA_API_START                      ICF_PA_API_START

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to inform about the incoming call from network so that the phone 
 * application csn start alerting the user. */

#define IPPSTK_CC_PA_INCOMING_CALL_IND           ICF_INCOMING_CALL_IND


/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to inform about the MEDIA_CHANGE RE-invite from network so that the phone 
 * application can get the changed media params*/
#define IPPSTK_CC_PA_INCOMING_CALL_MODIFY_IND			ICF_INCOMING_CALL_MODIFY_IND

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit to get response of
 * the Re-Invite request */
#define IPPSTK_PA_CC_CALL_MODIFY_CFM					ICF_CALL_MODIFY_CFM

#define IPPSTK_CC_PA_INCOMING_CALL_MODIFY_CANCEL_IND ICF_INCOMING_CALL_MODIFY_CANCEL_IND


/* This API is send by Phone Application towards SIP Packet Phone Signaling 
 * Toolkit to inform about the call initiated by user so that the SIP 
 * Packet Phone Signaling Toolkit can initiate call towards remote party. */

#define IPPSTK_PA_CC_CREATE_CALL_REQ             ICF_CREATE_CALL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit towards PA to 
 * inform that the remote party has been alerted. */

#define IPPSTK_CC_PA_ALERT_USER_IND              ICF_REMOTE_USER_ALERTED_IND

/* This API indicates that Phone Application has alerted the user for an 
 * earlier informed incoming call. It also specifies whether the remote party 
 * should play the ringback tone locally or Phone Application will play 
 * the tone to the remote party. */

#define IPPSTK_PA_CC_USER_ALERTED_REQ            ICF_LOCAL_USER_ALERTED_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone 
 * Application when remote SDP has been received and applied on the 
 * media session. This signifies that the media with remote party is 
 * connected. */

#define IPPSTK_CC_PA_MEDIA_CONNECTED_IND         ICF_MEDIA_CONNECTED_IND

/* This API indicates that Remote party has accepted the call. This API 
 * is sent from SIP Packet Phone Signaling Toolkit to Phone Application
 * on getting call acception from remote party. */

#define IPPSTK_CC_PA_CONNECT_IND                 ICF_CONNECT_IND

/* This API is sent from Phone Application to SIP Packet Phone Signaling 
 * Toolkit to inform that the user has accepted the incoming call. */

#define IPPSTK_PA_CC_CONNECT_REQ                 ICF_CONNECT_REQ

/* This API is invoked by SIP Packet Phone Signaling Toolkit to Phone
 * Application to indicate that the call clearing procedure has to
 * be started at Phone Application too. */

#define IPPSTK_CC_PA_TERMINATE_CALL_IND          ICF_TERMINATE_CALL_IND

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * in response to IPPSTK_CC_PA_TERMINATE_CALL_IND to indicate that the
 * user has also cleared the call so that a new call can be presented to
 * the user. */

#define IPPSTK_PA_CC_TERMINATE_CALL_CFM         ICF_LOCAL_USER_TERMINATE_CALL_RESP

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to indicate that the call has to be cleared. */

#define IPPSTK_PA_CC_TERMINATE_CALL_REQ          ICF_TERMINATE_CALL_REQ

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * in response to IPPSTK_PA_CC_TERMINATE_CALL_REQ to indicate that the
 * call has been cleared. */

#define IPPSTK_CC_PA_TERMINATE_CALL_RESP         ICF_REMOTE_USER_TERMINATE_CALL_RESP

/* This API is used by SIP Packet Phone Signaling Toolkit to request 
 * Phone Application to display certain information to the phone user. 
 * It can also optionally specify a tone to be played. */
/*
#define IPPSTK_CC_PA_DISPLAY_IND                 ICF_DISPLAY_IND
*/

/* This is a utility API that can be used by Phone Application to request 
 * SIP Packet Phone Signaling Toolkit to send data for all the calls 
 * received, dialed and missed. */

#define IPPSTK_PA_CC_GET_CALL_LOG_REQ            ICF_GET_CALL_LOG_REQ

/* This API is sent in response to IPPSTK_GET_CALL_LOG_REQ with the
 * log information of the calls received or dialled or missed. */

#define IPPSTK_CC_PA_CALL_LOG_RESP               ICF_CALL_LOG_RESP

/* This API is used by the Phone Application to SIP Packet Phone 
 * Signaling Toolkit when it wants to put the current call on hold. After
 * putting the current call on hold the Phone Application may or maynot want
 * to initiate a new call. */

#define IPPSTK_PA_CC_CALL_HOLD_REQ               ICF_CALL_HOLD_REQ

/* This API is sent in respone to IPPSTK_CALL_HOLD_REQ to indicate 
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * get the current call into held state. */

#define IPPSTK_CC_PA_CALL_HOLD_RESP              ICF_CALL_HOLD_RESP

/* This API is used by the Phone Application to SIP Packet Phone 
 * Signaling Toolkit when it wants to resume an held call. */

#define IPPSTK_PA_CC_CALL_RESUME_REQ             ICF_CALL_RESUME_REQ

/* This API is sent in respone to IPPSTK_CALL_RESUME_REQ to indicate
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * resume the held call. */

#define IPPSTK_CC_PA_CALL_RESUME_RESP            ICF_CALL_RESUME_RESP

/* This API is sent from SIP Packet Phone Signaling Toolkit to 
 * Phone Application to indicate that the outgoing call initiated by the
 * user has undergone a call diversion and is now being forwarded to
 * a remote party. */

#define IPPSTK_CC_PA_CALL_REDIRECTION_IND        ICF_CALL_REDIRECTION_IND

/* PA invokes this API toward SIP Packet Phone Signaling Toolkit when 
 * user give request to merge and ongoing call with a held call. */

#define IPPSTK_PA_CC_CONFERENCE_REQ              ICF_CONFERENCE_REQ

/* This API is sent in respone to IPPSTK_CONFERENCE_REQ to indicate
 * that the SIP Packet Phone Signaling Toolkit was able to successfully
 * merge the calls. */

#define IPPSTK_CC_PA_CONFERENCE_RESP             ICF_CONFERENCE_RESP

/* PA invokes this API towards SIP Packet Phone Signaling Toolkit when 
 * user wants to transfer call an active call to another remote party
 * without talking to remote party. */

#define IPPSTK_PA_CC_CALL_TRANSFER_UNATTENDED_REQ ICF_CALL_TRANSFER_UNATTENDED_REQ

/* This API is sent in respone to IPPSTK_CALL_TRANSFER_UNATTENDED_REQ
 * to indicate that the SIP Packet Phone Signaling Toolkit was able to
 * successfully tranfer the call. This indicates that the remote parties
 * are in conversation. */

#define IPPSTK_CC_PA_CALL_TRANSFER_UNATTENDED_RESP ICF_CALL_TRANSFER_UNATTENDED_RESP

/* PA invokes this API towards SIP Packet Phone Signaling Toolkit when
 * user wants to transfer call an active call to a remote party
 * which is held state. */

#define IPPSTK_PA_CC_CALL_TRANSFER_ATTENDED_REQ  ICF_CALL_TRANSFER_ATTENDED_REQ

/* This API is sent in respone to IPPSTK_CALL_TRANSFER_ATTENDED_REQ 
 * to indicate that the SIP Packet Phone Signaling Toolkit was able to
 * successfully tranfer the call. This indicates that the remote parties
 * are in conversation. */

#define IPPSTK_CC_PA_CALL_TRANSFER_ATTENDED_RESP ICF_CALL_TRANSFER_ATTENDED_RESP

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has held the call. */

#define IPPSTK_CC_PA_CALL_HOLD_IND               ICF_CALL_HOLD_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has resumed the earlier held call. */

#define IPPSTK_CC_PA_CALL_RESUME_IND             ICF_CALL_RESUME_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the remote party has transferred the current call to
 * a remote party. */

#define IPPSTK_CC_PA_REMOTE_CALL_TRANSFER_INITIATED_IND ICF_REMOTE_CALL_TRANSFER_INITIATED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the calls have been successfully merged after the remote
 * party has initiated call transfer. */

#define IPPSTK_CC_PA_CALL_MERGED_IND             ICF_CALL_MERGED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * to indicate that the current call has the remote party replaced due to 
 * request from remote party. */

#define IPPSTK_CC_PA_PARTY_REPLACED_IND          ICF_PARTY_REPLACED_IND

/* This API is sent by SIP Packet Phone Signaling Toolkit to Phone Application
 * in response to the IPPSTK_INFO_REQ sent by PA */

#define IPPSTK_CC_PA_INFO_RESP					 ICF_INFO_RESP

/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to inform that the call should be muted. */

#define IPPSTK_PA_CC_CALL_MUTE_REQ               ICF_CALL_MUTE_REQ

#define IPPSTK_CC_PA_DISPLAY_IND					ICF_DISPLAY_IND

#define IPPSTK_TRACE_NW_BUFF_IND                 ICF_TRACE_NW_BUFF_IND
/* This API is sent by Phone Application to SIP Packet Phone Signaling Toolkit
 * to inform that the muted call should be unmuted. */


#define IPPSTK_CONTROL_PLANE_API_BASE            ICF_CONTROL_PLANE_API_BASE            
#define IPPSTK_REGISTER_REQ                      ICF_REGISTER_REQ                      
#define IPPSTK_REGISTER_CFM                      ICF_REGISTER_CFM                      
#define IPPSTK_REGISTER_STATUS_IND               ICF_REGISTER_STATUS_IND               
#define IPPSTK_CREATE_CALL_REQ                   ICF_CREATE_CALL_REQ                   
#define IPPSTK_LOCAL_USER_ALERTED_REQ            ICF_LOCAL_USER_ALERTED_REQ            
#define IPPSTK_TERMINATE_CALL_REQ                ICF_TERMINATE_CALL_REQ                
#define IPPSTK_LOCAL_USER_TERMINATE_CALL_RESP ICF_LOCAL_USER_TERMINATE_CALL_RESP    
#define IPPSTK_CONNECT_REQ                       ICF_CONNECT_REQ                       
#define IPPSTK_SUBSCRIBE_REQ                     ICF_SUBSCRIBE_REQ                     
#define IPPSTK_PUBLISH_REQ                       ICF_PUBLISH_REQ                       
#define IPPSTK_MESSAGE_REQ                       ICF_MESSAGE_REQ                       
#define IPPSTK_REFER_REQ                         ICF_REFER_REQ                         

#define IPPSTK_INCOMING_CALL_IND                 ICF_INCOMING_CALL_IND                 
#define IPPSTK_REMOTE_USER_ALERTED_IND           ICF_REMOTE_USER_ALERTED_IND           
#define IPPSTK_TERMINATE_CALL_IND                ICF_TERMINATE_CALL_IND                
#define IPPSTK_REMOTE_USER_TERMINATE_CALL_RESP   ICF_REMOTE_USER_TERMINATE_CALL_RESP   
#define IPPSTK_MEDIA_CONNECTED_IND               ICF_MEDIA_CONNECTED_IND               
#define IPPSTK_CONNECT_IND                       ICF_CONNECT_IND                       
#define IPPSTK_SUBSCRIPTION_STATUS_IND           ICF_SUBSCRIPTION_STATUS_IND           
#define IPPSTK_NOTIFY_IND                        ICF_NOTIFY_IND                        
#define IPPSTK_PUBLISH_RESP                      ICF_PUBLISH_RESP                      
#define IPPSTK_MESSAGE_RESP                      ICF_MESSAGE_RESP                      
#define IPPSTK_MESSAGE_IND                       ICF_MESSAGE_IND                       
#define IPPSTK_REFER_RESP                        ICF_REFER_RESP                        
#define IPPSTK_CONTROL_PLANE_API_END             ICF_CONTROL_PLANE_API_END             

#define IPPSTK_PA_CC_CALL_UNMUTE_REQ             ICF_CALL_UNMUTE_REQ
#define IPPSTK_PA_CC_INFO_REQ                    ICF_INFO_REQ
#define IPPSTK_PA_API_END                        ICF_PA_API_END

#define IPPSTK_MEDIA_PLANE_API_BASE        ICF_MEDIA_PLANE_API_BASE       
#define IPPSTK_CFG_CODEC_INFO_REQ          ICF_CFG_CODEC_INFO_REQ      
#define IPPSTK_CFG_CODEC_INFO_RESP         ICF_CFG_CODEC_INFO_RESP        
#define IPPSTK_CFG_RINGER_TONES_REQ        ICF_CFG_RINGER_TONES_REQ       
#define IPPSTK_CFG_RINGER_TONES_RESP       ICF_CFG_RINGER_TONES_RESP
#define IPPSTK_CFG_TONES_REQ			   ICF_CFG_TONES_REQ
#define IPPSTK_CFG_TONES_RESP			   ICF_CFG_TONES_RESP
#define IPPSTK_CFG_JBM_MGMT_REQ            ICF_CFG_JBM_MGMT_REQ           
#define IPPSTK_CFG_JBM_MGMT_RESP           ICF_CFG_JBM_MGMT_RESP          
#define IPPSTK_CFG_MAX_EARP_VOL_REQ        ICF_CFG_MAX_EARP_VOL_REQ       
#define IPPSTK_CFG_MAX_EARP_VOL_RESP       ICF_CFG_MAX_EARP_VOL_RESP      
#define IPPSTK_CFG_MAX_SPEAKER_VOL_REQ     ICF_CFG_MAX_SPEAKER_VOL_REQ    
#define IPPSTK_CFG_MAX_SPEAKER_VOL_RESP    ICF_CFG_MAX_SPEAKER_VOL_RESP   
#define IPPSTK_CFG_PORT_RANGE_REQ          ICF_CFG_PORT_RANGE_REQ         
#define IPPSTK_CFG_PORT_RANGE_RESP         ICF_CFG_PORT_RANGE_RESP
#define IPPSTK_CFG_SET_SELF_IP_REQ		   ICF_CFG_SET_SELF_IP_REQ
#define IPPSTK_CFG_SET_SELF_IP_RESP		   ICF_CFG_SET_SELF_IP_RESP
#define IPPSTK_MEDIA_PLANE_API_BASE        ICF_MEDIA_PLANE_API_BASE
#define IPPSTK_START_RINGING_REQ           ICF_START_RINGING_REQ       
#define IPPSTK_START_RINGING_RESP          ICF_START_RINGING_RESP     
#define IPPSTK_MEDIA_SESSION_REQ           ICF_MEDIA_SESSION_REQ      
#define IPPSTK_MEDIA_SESSION_RESP          ICF_MEDIA_SESSION_RESP 
#define IPPSTK_ECHO_CANC_REQ			   ICF_ECHO_CANC_REQ
#define IPPSTK_ECHO_CANC_RESP			   ICF_ECHO_CANC_RESP
#define IPPSTK_VAD_SS_CNG_REQ			   ICF_VAD_SS_CNG_REQ
#define IPPSTK_VAD_SS_CNG_RESP			   ICF_VAD_SS_CNG_RESP
#define IPPSTK_GET_STATS_REQ               ICF_GET_STATS_REQ          
#define IPPSTK_GET_STATS_RESP              ICF_GET_STATS_RESP   
#define IPPSTK_PLAY_FILE_REQ			   ICF_PLAY_FILE_REQ
#define IPPSTK_PLAY_FILE_RESP			   ICF_PLAY_FILE_RESP
#define IPPSTK_PLAY_TONE_REQ			   ICF_PLAY_TONE_REQ
#define IPPSTK_PLAY_TONE_RESP			   ICF_PLAY_TONE_RESP
#define IPPSTK_STOP_TONE_REQ			   ICF_STOP_TONE_REQ
#define IPPSTK_STOP_TONE_RESP			   ICF_STOP_TONE_RESP
#define IPPSTK_STOP_FILE_REQ			   ICF_STOP_FILE_REQ
#define IPPSTK_STOP_FILE_RESP			   ICF_STOP_FILE_RESP
#define IPPSTK_STOP_PLAY_FILE_IND		   ICF_STOP_PLAY_FILE_IND
#define IPPSTK_STOP_RINGING_REQ            ICF_STOP_RINGING_REQ       
#define IPPSTK_STOP_RINGING_RESP           ICF_STOP_RINGING_RESP  
#define IPPSTK_STOP_TONE_IND			   ICF_STOP_TONE_IND
#define IPPSTK_STOP_RINGING_IND            ICF_STOP_RINGING_IND       
#define IPPSTK_MUTE_REQ                    ICF_MUTE_REQ               
#define IPPSTK_MUTE_RESP                   ICF_MUTE_RESP              
#define IPPSTK_PLAY_MEDIA_SPKR_REQ         ICF_PLAY_MEDIA_SPKR_REQ    
#define IPPSTK_PLAY_MEDIA_SPKR_RESP        ICF_PLAY_MEDIA_SPKR_RESP   
#define IPPSTK_SET_EARP_VOLUME_REQ         ICF_SET_EARP_VOLUME_REQ    
#define IPPSTK_SET_EARP_VOLUME_RESP        ICF_SET_EARP_VOLUME_RESP   
#define IPPSTK_SET_SPEAKER_VOLUME_REQ      ICF_SET_SPEAKER_VOLUME_REQ 
#define IPPSTK_SET_SPEAKER_VOLUME_RESP     ICF_SET_SPEAKER_VOLUME_RESP
#define IPPSTK_SEND_APP_DATA_REQ           ICF_SEND_APP_DATA_REQ      
#define IPPSTK_SEND_APP_DATA_RESP          ICF_SEND_APP_DATA_RESP     
#define IPPSTK_RCV_APP_DATA_IND            ICF_RCV_APP_DATA_IND       
#define IPPSTK_SEND_STATS_REQ              ICF_SEND_STATS_REQ         
#define IPPSTK_SEND_STATS_RESP             ICF_SEND_STATS_RESP        
#define IPPSTK_STOP_STATS_REQ              ICF_STOP_STATS_REQ         
#define IPPSTK_STOP_STATS_RESP             ICF_STOP_STATS_RESP   
#define IPPSTK_CFG_MAX_MIC_GAIN_REQ		   ICF_CFG_MAX_MIC_GAIN_REQ
#define IPPSTK_SET_MIC_GAIN_REQ			   ICF_SET_MIC_GAIN_REQ		
#define IPPSTK_MEDIA_PLANE_API_END         ICF_MEDIA_PLANE_API_END     


/*--------------------------------------------------------------------------*/
#define IPPSTK_MISC_API_START                   ICF_EXTERNAL_API_END

/* This is the timer expiry message */
#define IPPSTK_TIMER_MSG						IPPSTK_MISC_API_START + 1

/* ------------------------------------------------------------------------- */
/*
 * The following list of APIs is for events at TCP/UDP interfaces.
 * like reception of a message or some event at TCP interface
 */
/*This is used for both TCP and UDP messages*/
#define IPPSTK_NW_API_MSG_FROM_PEER    IPPSTK_MISC_API_START + 2
    /*Indicates a connect req from peer*/
#define IPPSTK_NW_API_OPEN_CONN_IND    IPPSTK_MISC_API_START + 3
    /*Indicates success in pending outgoing connect*/
#define IPPSTK_NW_API_CONNECT_SUCCESS     IPPSTK_MISC_API_START + 4
    /*Indicates failure in pending outgoing connect*/
#define IPPSTK_NW_API_CONNECT_FAILURE     IPPSTK_MISC_API_START + 5
    /*Indicates connection closure by peer*/
#define IPPSTK_NW_API_CLOSE_CONN_IND   IPPSTK_MISC_API_START + 6

#define IPPSTK_MSG_FRM_PLTFM			ICF_MSG_FRM_PLTFM

#define IPPSTK_DEPRECATED_API_START     IPPSTK_MISC_API_START + 8
/*
 * The following are configuration APIs at IPPSTK-CFG interface that have been
 * deprecated in ICF. These are not exposed outside and not used anymore
 */


 /* This API is sent by Configuration Module to toolkit to specify that 
  * the configuration is complete and the toolkit should start processing. 
  * After this API, toolkit shall do registration with configured registrar 
  * and make itself ready to receive/originate calls. */
#define IPPSTK_CFG_CC_INIT_COMPLETE_REQ        IPPSTK_DEPRECATED_API_START
/* This API is used to configure service related parameters like service 
 * enable/disable service, activate/deactivate service. service related 
 * timer values etc. This is also configured for each line. */

/*#define IPPSTK_CFG_CC_SET_SERVICE_PARAMS_REQ   IPPSTK_DEPRECATED_API_START + 1*/

/* This API is sent by Configuration module to clear all the ongoing 
 * calls and deallocate all the call-related buffers, anytime after the 
 * initial configuration is complete. However the configuration will be
 * retained and phone will not be deregistered. */
#define IPPSTK_SM_CC_RESTART_REQ               IPPSTK_DEPRECATED_API_START + 2

/* This API is used to delete the previous configuration of the SIP Packet
 * Phone Signaling Toolkit and bring it to idle state. All the ongoing calls
 * would be cleared, buffers for calls would be deallocated and SIP Packet 
 * Phone signaling Toolkit would deregister from Registrar. */
#define IPPSTK_SM_CC_DELETE_CONFIG_PARAMS_REQ  IPPSTK_DEPRECATED_API_START + 3

#define IPPSTK_DEPRECATED_API_END              IPPSTK_DEPRECATED_API_START + 3  

/* API for configuration module to provide the values of the 
 * scalable parameters to SIP Packet Phone Signaling Toolkit */
#define IPPSTK_CFG_CC_SET_SCALE_PARAMS_REQ     ICF_SET_SCALE_PARAMS_REQ

/* ------------------------------------------------------------------------- */
#define IPPSTK_MISC_API_END            IPPSTK_DEPRECATED_API_END

#define IPPSTK_EXTERNAL_API_END        ICF_EXTERNAL_API_END

/*ICF_INIT_START*/
/* maximum number if lines in the configuration file */
#define IPPSTK_CONF_MAX_LINES			40
/* maximum characters in a line in the configuration file */
#define IPPSTK_CONF_MAX_LINE_SIZE		100
/* comment character in the configuration file */
#define IPPSTK_CONF_COMMENT_CHAR		'#'

#define IPPSTK_IMS_AUTH_OPTION			0
#define IPPSTK_NONIMS_AUTH_OPTION		1
/*ICF_INIT_END*/



/*------------------------------------------------------------------------------
 * 
 * typedef Name: ippstk_media_type_t
 *
 * Description: defines the type of media information.
 * 
 *----------------------------------------------------------------------------*/

/* Denotes audio stream */
#define IPPSTK_MEDIA_TYPE_AUDIO				ICF_MEDIA_TYPE_AUDIO

/* Denotes video stream */
#define IPPSTK_MEDIA_TYPE_VIDEO				ICF_MEDIA_TYPE_VIDEO

/* Denotes application stream */
#define IPPSTK_MEDIA_TYPE_APPLICATION		ICF_MEDIA_TYPE_APPLICATION

/* Denotes invalid stream */
#define IPPSTK_MEDIA_TYPE_INVALID			ICF_MEDIA_TYPE_INVALID



/*------------------------------------------------------------------------------
 * 
 * typedef Name: ippstk_stream_id_t
 *
 * Description: defines the type of media information.
 * 
 *----------------------------------------------------------------------------*/

/* Denotes invalid stream id */
#define IPPSTK_STREAM_ID_INVALID			ICF_STREAM_ID_INVALID

/* Denotes audio stream id */
#define IPPSTK_AUDIO_STREAM_ID				ICF_AUDIO_STREAM_ID

/* Denotes video stream id */
#define IPPSTK_VIDEO_STREAM_ID				ICF_VIDEO_STREAM_ID

/* Denotes TBCP stream id */
#define IPPSTK_TBCP_STREAM_ID				ICF_TBCP_STREAM_ID



/*------------------------------------------------------------------------------
 * 
 * typedef Name: ippstk_call_type_t
 *
 * Description: defines the type of media information.
 * 
 *----------------------------------------------------------------------------*/

/* Denotes audio call */
#define IPPSTK_CALL_TYPE_AUDIO				ICF_CALL_TYPE_AUDIO

/* Denotes video call */
#define IPPSTK_CALL_TYPE_VIDEO				ICF_CALL_TYPE_VIDEO

/* Denotes audio-video call */
#define IPPSTK_CALL_TYPE_AUDIO_VIDEO		ICF_CALL_TYPE_AUDIO_VIDEO

/* Denotes audio-tbcp call */
#define IPPSTK_CALL_TYPE_AUDIO_TBCP			ICF_CALL_TYPE_AUDIO_TBCP

/* Denotes video-tbcp call */
#define IPPSTK_CALL_TYPE_VIDEO_TBCP			ICF_CALL_TYPE_VIDEO_TBCP

/* Denotes default call type = audio */
#define IPPSTK_CALL_TYPE_DEFAULT			ICF_CALL_TYPE_DEFAULT

/* Denotes invalid call type */
#define IPPSTK_CALL_TYPE_INVALID			ICF_CALL_TYPE_INVALID



/*------------------------------------------------------------------------------
 * 
 * typedef Name: ippstk_app_id_t
 *
 * Description: defines the type of media information.
 * 
 *----------------------------------------------------------------------------*/

/* Denotes invalid app id */
#define IPPSTK_APP_ID_INVALID				ICF_INVALID_APP_ID



/* Denotes the number of Application specific streams per call */
/* Currently used for TBCP stream information */
#define IPPSTK_MAX_STREAMS_PER_CALL			ICF_MAX_STREAMS_PER_CALL

/* Denotes the attributes for a "media" line in SDP */
#define IPPSTK_MAX_ATTRIBUTES_PER_CALL		ICF_MAX_ATTRIBUTES_PER_CALL


/*ICF_4_0_CRM_START*/
/* For REGISTER_CFM/IND the member request_type may take the following values  for FAILURE */
	
#define IPPSTK_REQUEST_TYPE_REGISTERING			ICF_REQUEST_TYPE_REGISTERING
#define IPPSTK_REQUEST_TYPE_AUTHORIZING			ICF_REQUEST_TYPE_AUTHORIZING
#define IPPSTK_REQUEST_TYPE_DEREGISTERING		ICF_REQUEST_TYPE_DEREGISTERING
#define	IPPSTK_REQUEST_TYPE_DEREGISTER			ICF_REQUEST_TYPE_DEREGISTER

/*ICF_4_0_CRM_END*/

#ifdef IPPSTK_SESSION_TIMER
#define	IPPSTK_CALL_CLEAR_REASON_SESSION_TIMER_EXPIRED ICF_CALL_CLEAR_REASON_SESSION_TIMER_EXPIRED
#endif
/* These API will only be used for SI testing purposes on windows. 
 * ICF will send these API's towards IAS which will be simulating 
 * GPRS network.
 */
#define IPPSTK_GPRS_PRI_PDP_ACTIVATE_REQ	ICF_GPRS_PRI_PDP_ACTIVATE_REQ			
#define IPPSTK_GPRS_PRI_PDP_ACTIVATE_RESP	ICF_GPRS_PRI_PDP_ACTIVATE_RESP
#define IPPSTK_GPRS_SEC_PDP_ACTIVATE_REQ	ICF_GPRS_SEC_PDP_ACTIVATE_REQ
#define IPPSTK_GPRS_SEC_PDP_ACTIVATE_RESP	ICF_GPRS_SEC_PDP_ACTIVATE_RESP
#define IPPSTK_GPRS_PDP_DEACTIVATE_REQ		ICF_GPRS_PDP_DEACTIVATE_REQ	
#define IPPSTK_GPRS_PDP_DEACTIVATE_RESP		ICF_GPRS_PDP_DEACTIVATE_RESP
#define IPPSTK_GPRS_PDP_MODIFY_IND			ICF_GPRS_PDP_MODIFY_IND		
#define IPPSTK_GPRS_PDP_DEACTIVATE_IND		ICF_GPRS_PDP_DEACTIVATE_IND	
#define IPPSTK_GPRS_PLATFORM_EVENT_IND		ICF_GPRS_PLATFORM_EVENT_IND	


/* Configuration merge related New Type defs */
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_ADDR_ACTION      ICF_SET_SLF_ID_PRM_ADDR_ACTION
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_ADDR             ICF_SET_SLF_ID_PRM_ADDR
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_PRIORITY         ICF_SET_SLF_ID_PRM_PRIORITY
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_NAME             ICF_SET_SLF_ID_PRM_NAME
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_DEFAULT_SETTINGS ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_AUTH_KEY         ICF_SET_SLF_ID_PRM_AUTH_KEY
#define  IPPSTK_CFG_CC_SET_SLF_ID_PRM_AUTH_PASSWORD    ICF_SET_SLF_ID_PRM_AUTH_PASSWORD

#define IPPSTK_USER_PARAM_PHONE ICF_USER_PARAM_PHONE

/* ------------------------------------------------------------------------ */
/* Maximun number of characters stored in structure icf_short_string_st */
#define IPPSTK_MAX_SHORT_STR_LEN                 ICF_MAX_SHORT_STR_LEN

/* Maximun number of characters stored in structure icf_long_string_st */
#define IPPSTK_MAX_LONG_STR_LEN                  ICF_MAX_LONG_STR_LEN

/*------------------------------------------------------------------------------
 *
 *  ippstk_tag_st values
 *
 *----------------------------------------------------------------------------*/
/* Invalid Tag */
#define IPPSTK_TAG_TYPE_INVALID					ICF_TAG_TYPE_INVALID
/* Tag is a uri parameter */
#define IPPSTK_TAG_TYPE_SIP_URI_PARAM			ICF_TAG_TYPE_SIP_URI_PARAM
/* Tag is a uri header parameter */
#define IPPSTK_TAG_TYPE_SIP_URI_HDR_PARAM		ICF_TAG_TYPE_SIP_URI_HDR_PARAM
/* Tag is a header parameter */
#define IPPSTK_TAG_TYPE_HDR_PARAM				ICF_TAG_TYPE_HDR_PARAM

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__ICF_IPPSTK_MAP_DEFS_H__*/

