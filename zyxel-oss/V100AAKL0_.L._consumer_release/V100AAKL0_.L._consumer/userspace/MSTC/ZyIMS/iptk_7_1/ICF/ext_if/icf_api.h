/******************************************************************************
*                                    
* FILE NAME    : icf_api.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in IMS CLIENT
*                  FRAMEWORK APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 22-JUL-2005      Ram Narayan/Shekhar        None           Initial
* 02-Feb-2006      Amit Sharma                               CFG merged with ICF
* 																2.0
* 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
* 07-Mar-2006	   Anuradha Gupta							Added new APIs for 
* 															Reinvite Handling
* 30-Mar-2006  Anuradha Gupta				ICF 5.0         Call Modify Feature 
* 															changes
* 02-Apr-2006  Amit Sharma               Updation for API doc3.1
* 07-Aug-2006  Amit Sharma               Added call forwarded ind
*                                        and updated local_user_alerted
*
* 29-Nov-2006  Deepti Goyal         ICF Rel 6.2         Changes for set_call
* 15-Dec-2006  Amit Sharma                              Updation for set_trans_param
*                                                       params API.
* 03-Jan-2007  Amit Mahajan         ICF Rel 6.2         INFO confirmation API added. 
* 04-Jan-2007  Priyanka Dua         ICF 6.3             Changes for set_call
*                                                       params API(multiple_answer_enable).
* 15-Jan-2007  Abhishek Dhammawat   ICF 6.3             Changes for Per Line Feature
* 23-Mar-2007  Deepti Goyal         ICF 6.4             Changes for Media_Connect_Req
*                                                       API
* 24-Mar-2007  Rajat Chandna                            Changes for response code
*                                                       enhancement.
* 26-Mar-2007  Rajat Chandna                            Changes for update/reinvite
*                                                       enhancement.
* 04-Apr-2007  Rajat Chandna                            Changes for wildcarded user
*                                                       enhancement.
* 08-May-2007  Abhishek Dutta                           Changes for remote cta cfm
* 15-May-2007  Sudhesh Singh                            Changes for early call
*                                                       transfer
* 14-Jun-2007  Alok Tiwari                              Changes for get 
*                                                       register status request
*                                                       and response.
* 09-Jul-2007  Anuradha Gupta	   ICF 7.0              Initial release specific changes
* 13-Jul-2007 Abhishek Dhammawat   Rel7.0               Addtion for  xconf ftr.
* 27-Aug-2007  Deepti Goyal     SPR 15997               Line id added in 
*                                                       ICF_CONFIGURE_CFM API
* 07-Jan-2007  Amit Sikka       SPR 17945               Support for PUBLISH/MESSAGE
* 07-Nov-2008  Anurag Khare     SPR 19168/CSR 1-6456839 New Field added in set_call_params
* 
* 06-Jan-2009  Vinay Jindal     SPR 19250               Added a new parameter in
*                                                       set_service_params API
* 05-Feb-2009	Rohit Aggarwal	SPR 19316	Changes for INVITE with Replaces
* 19-Dec-2009  Rohit Aggarwal	SPR 20246  Configuration to strip Route header
* 23-Mar-2010  Rohit Aggarwal	SPR 20343  Event name in subscription ind
* 26-Mar-2010  Kamal Ashraf     SPR:20364               Added a new parameter in
*                                                       ICF_REGISTER_REQ  API
* 09-Apr-2010  Alok Tiwari      SPR 20362               Added new field remove_hdr_list
*                                                       in icf_options_req_st
*
* Copyright 2010, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ICF_API_H__
#define __ICF_API_H__

#include "icf_types.h"
#include "icf_defs.h"
#include "icf_mm_api.h"


/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
    
#ifdef ICF_NW_ACTIVATE
/************************
*    NETWORK ACCESS APIs *
************************/
/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_network_activate_req_st
 *
 * Description: This API provides mechanism to activate Network access. For 
 *                example if the access network is a GPRS network, then this 
 *                API shall trigger PDP activation.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This specifies the access network type.This can be ICF_GPRS. Options related 
     * to other access network types shall be supported in future.
     */
    icf_uint8_t access_type;

    /* This specifies the parameters for accessing network. For example if access_type 
     * is ICF_GPRS then access_info is APN (Access Point Name).
     */
    icf_string_st access_info;

    icf_uint8_t       bit_mask;
#define ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT	0x01

    /* routing_info contains the IPC information to reach application.*/
    icf_void_st routing_info;

} icf_network_activate_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_network_activate_cfm_st
 *
 * Description: This API is sent by ICF to the application in confirmation to
 *                ICF_NETWORK_ACTIVATE_REQ API.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This indicates the presence of the optional parameters. */
    icf_uint8_t bit_mask;

#define ICF_NET_ACT_CFM_ERR_CAUSE_PRESENT 0x01
#define ICF_NET_ACT_CFM_NUM_NW_PRESENT 0x02
#define ICF_NET_ACT_CFM_NW_INFO_PRESENT 0x04

    /* Denotes the Success or Failure status of the execution of the API */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure. */
    icf_error_t error_cause;

    /* Stores the number of network access specified in the nw_info list. */
    icf_uint8_t num_nw;

    /* List of activated network access. */
    icf_network_info_st nw_info[ICF_MAX_APN];

} icf_network_activate_cfm_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_network_deactivate_req_st
 *
 * Description: This API provides mechanism to de-activate Network access. For 
 *              example if the access network is a GPRS network, then this API 
 *              shall trigger PDP deactivation.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This is the logical PDP context id. */
    icf_uint32_t ctxt_id;

    icf_uint8_t			bit_mask;
#define ICF_NET_ACT_REQ_ROUTE_INFO_PRESENT	0x01

    /* routing_info contains the IPC information to reach application. */
    icf_void_st routing_info;

} icf_network_deactivate_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_network_deactivate_cfm_st
 *
 * Description: This API is sent by ICF to the application in confirmation to
 *                ICF_NETWORK_DEACTIVATE_REQ API
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This indicates the presence of the optional parameters. */
    icf_uint8_t bit_mask;

#define ICF_NET_DEACT_CFM_ERR_CAUSE_PRESENT 0x01
#define ICF_NET_DEACT_CFM_NUM_NW_PRESENT 0x02
#define ICF_NET_DEACT_CFM_NW_INFO_PRESENT 0x04

    /* Denotes the Success or Failure status of the execution of the API. */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure. */
    icf_error_t error_cause;

    /* Stores the number of network access specified in the nw_info list. */
    icf_uint8_t num_nw;

    /* List of activated network access. */
    icf_network_info_st nw_info[ICF_MAX_APN];

} icf_network_deactivate_cfm_st;
#endif/*#ifdef ICF_NW_ACTIVATE*/


/************************

* APPLICATION SETUP APIs *

************************/

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_app_add_req_st
 *
 * Description: This datatype is used to define a structure, which specifies the 
 *                payload data of the ICF_APP_ADD_REQ API.
 *-----------------------------------------------------------------------------*/
typedef struct
{
#define ICF_APP_ADD_MEDIA_PROFILE_PRESENT      0x01
	icf_uint8_t      bit_mask;
    /* This is an Application name, used for tracing purposes. */
    icf_string_st app_name;

    /* routing_info contains the IPC information to reach application. */
    icf_void_st routing_info ;

    /* ICF uses filter to decide the application corresponding to the 
     * message obtained from the network and where call_id field is not 
     * maintained by ICF.    
     */
    icf_filter_list_st filter;

    /* This field is a List of additional headers that the application wants 
     * to receive in the APIs from ICF. If the application doesn't require any 
	 * additional headers from the incoming SIP messages then count should be 
	 * set to zero in the icf_header_name_list_st    
    */
    icf_header_name_list_st header_name_list;

    /* List of SIP Methods which are supported by the application */
    icf_string_list_st  supported_methods;
    
    /* List of Events which are supported by the application */
    icf_string_list_st  supported_events;
    
    /* List of Content Types (Message Bodies) which are supported by the 
	 * application */
    icf_string_list_st  supported_content_types;
    
    /* List of message encodings supported by the application */
    icf_string_list_st  supported_encodings;

    /* List of Languages supported by the application */
    icf_string_list_st  supported_languages;

	/* ICF REL 6.0: multiple m lines
	 * the following structure defines the media profile for an application.
	 * ICF will save this information within its database and it is expected
	 * that the application uses only those codecs in any stream which have 
	 * been configured with MM (by the application). ICF will  not validate 
	 * this as it is not aware of the configured infomration. this is a list
	 * of the data type icf_stream_capab_st. This is an optional parameter
	 * in this structure. The media streams received in the app_profile will 
	 * determine the call_type bitmap in the app_conf within ICF
	 */
	icf_list_st				*p_media_profile;

	/* rtp_detection will be applicable only for the audio streams within the
	 * call */
	icf_boolean_t			rtp_detection_reqd;

    /* Application can set this boolean to ICF_TRUE if it requires multiple 
     * simultaneous session support using ICF's internal line arbritration.
     * Else application can set this flag to ICF_FALSE and configure multiple 
     * lines in ICF and use app's own line arbritration mechanism for running
     * multiple calls simultaneously on different lines. */
    /* If set to TRUE, ICF will do line management and allow simultaneous 
     * calls on single line.
     * If set to FALSE, application will do line management and configure
     * multiple lines */
    icf_boolean_t  			icf_simul_call_line_arbitration;

    /*Application can set this boolean to enable ICF to send merge_media_req
     *before receiving remote media paramaters, if this flag is not set ICF
     *will send merge_media_req when it has received remote SDP*/
    icf_boolean_t                       merge_req_wo_rem_sdp;

} icf_app_add_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_app_add_cfm_st
 *
 * Description: This datatype is used to define a structure, which specifies the 
 *                payload data of the ICF_APP_ADD_CFM API.
 *-----------------------------------------------------------------------------*/
typedef struct
{
    /* This indicates the presence of the optional parameters. */
    icf_uint8_t bit_mask ;

#define ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT 0x01

#ifdef ICF_NW_ACTIVATE
    #define ICF_APP_ADD_CFM_NUM_NW_PRESENT 0x02
    #define ICF_APP_ADD_CFM_NET_INFO_PRESENT 0x04
#endif
    
    /* Denotes the Success or Failure status of the execution of the API. */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure. */
    icf_error_t error_cause;

#ifdef ICF_NW_ACTIVATE
    /* Stores the number of network access specified in the nw_info list. */
    icf_uint8_t num_nw;

    /* List of activated network access. */
    icf_network_info_st nw_info[ICF_MAX_APN];
#endif
    /* Public uri and P- Associated uri list */
/*    icf_pub_uri_list_node_st   *p_uri_list;*/

} icf_app_add_cfm_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_app_remove_req_st
 *
 * Description: This datatype is used to define a structure, which specifies the 
 *                payload data of the ICF_APP_REMOVE_REQ API.
 *-----------------------------------------------------------------------------*/
typedef struct
{
    /* This is for future use, currently coded as 0. */
    icf_uint32_t option ;

} icf_app_remove_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_app_remove_cfm_st
 *
 *    This datatype is used to define a structure, which specifies the 
 *    payload data of the ICF_APP_REMOVE_CFM API.
 *-----------------------------------------------------------------------------*/
typedef struct
{
    /* Denotes the Success or Failure status of the execution of the API. */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure.*/
    icf_error_t error_cause;

} icf_app_remove_cfm_st;

/************************
*    CONFIGURATION APIs *
************************/
/*-----------------------------------------------------------------------------
 * Structure Name: icf_set_self_id_req_st
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_SET_SELF_ID_REQ
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_uint32_t                   bit_mask;
#define  ICF_SET_SLF_ID_PRM_ADDR_ACTION         0x01
#define  ICF_SET_SLF_ID_PRM_ADDR                0x02
#define  ICF_SET_SLF_ID_PRM_PRIORITY            0x04
#define  ICF_SET_SLF_ID_PRM_NAME                0x08
#define  ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS    0x10
#define  ICF_SET_SLF_ID_PRM_AUTH_KEY            0x20
#define  ICF_SET_SLF_ID_PRM_AUTH_PASSWORD       0x40
#define  ICF_SET_SLF_ID_PRM_REALM               0x80
#define  ICF_SET_SLF_ID_PRM_AUTH_PER_USER       0x100

     /* line id for which the parameters present in this struct are applicable */
    icf_line_id_t                  line_id;

     /* action applicable for self_addr and self_name field */
     icf_config_action_t           action;

     /* user address for a line */
     icf_address_st                self_addr;

     /* user name for a line */
     icf_string_st                 self_name;
     
     /* priority of user for this line */
     icf_priority_id_t             priority;

     /* Settings for a line */

#define ICF_SETTINGS_CONFIG_PRESENTATION_IND           0x01
   /* if ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO is set then Notify 
    * will be given to application without matching its dialog.*/
#define ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO 0x02

     icf_uint8_t                   default_settings;

     /* Contains the authentication key configured per line */
     icf_string_st                 authentication_key;

     /* Contains the authentication password configured per line */
     icf_string_st                 authentication_password;

    /* Contains the realm_name configured per line */
     icf_string_st                 configured_realm;

} icf_set_self_id_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_set_transport_req_st 
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_SET_TRANSPORT_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    /* This indicates the presence of the optional parameters. */

    icf_uint8_t                  bit_mask;

#define ICF_SET_TRP_REQ_PROXY_ADDR              0x01
#define ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS   0x02
#define ICF_SET_TRP_REQ_SLF_MODE                0x04
#define ICF_SET_TRP_REQ_SLF_PORT                0x08
    /*Bit will be set if self transport address is per line Once this bit
     *is set, it has to be set every time*/
#define ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE       0x10
/* If this bit is enabled in the bit_mask then it means application wants to configure  
 * DNS port from where DNS queries will be issued.*/
#define ICF_SET_TRP_REQ_DNS_SRC_PORT             0x20

    /*SIP proxy IP address, configured per line*/
    icf_config_proxy_transport_address_st   proxy_addr;

    /*Self transport address and port, when transport addr not configured 
     * system call is used to obtain self transport addr*/

    icf_transport_addr_st               self_transport_address;

    /*Transport mode for incoming calls*/
    icf_transport_mode_t                   self_mode;

    /*Transport port for incoming calls*/
    icf_uint16_t                           self_port;

    /*Self transport address per line, will be used only if 
     * ICF_SET_TRP_REQ_SLF_ADDR_PER_LINE bit is set in bit_mask*/
    icf_config_transport_address_st   self_addr_per_line;
     /* Following variable will be containing dns port that application wants to configure. 
     * IPTK will send DNS query using this port.*/  
    icf_uint16_t                      dns_source_port; 

} icf_set_transport_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_set_registration_req_st 
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_SET_REGISTRATION_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* line id for which the registration params are applicable */
    icf_line_id_t                  line_id;

    /* Action can be add, delete or modify.
     * If action is add then registrar add is mandatory. Other params are
     * optional.
     * If action is modify then registrar add is not present. Other params are
     * optional.
     * If action is delete, then no other param is required. All the reg data
     * for the line is deleted */
    icf_config_action_t            action;

    /* SIP registrar IP address. When action is add this field is mandatory */
    icf_transport_address_st       registrar_addr;

#define ICF_SET_REG_REQ_MODE            0x01
#define ICF_SET_REG_REQ_DURATION        0x02
#define ICF_SET_REG_REQ_ROUTE_ADDR      0x04

/* this bit mask will be used to send the registrion 
 * head start duration */
#define ICF_SET_REG_HS_DURATION		0x08

    icf_uint8_t                   bit_mask;
    
    /* transport mode for registration request */
    icf_transport_mode_t           reg_mode;
    
    /* duration, in milliseconds, of registration which is line specific */
    icf_duration_t                registration_duration;
	
	/* Number of seconds prior to expriation of 
 	* registration */
	icf_duration_t			register_head_start_timer;

    /* Indicates the next hop where the REGISTER request should be sent 
     * This will be set in the outgoing Route header
     */
    icf_transport_address_st  	reg_route_addr;

} icf_set_registration_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_set_call_params_req_st 
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_SET_CALL_PARAM_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    /* This indicates the presence of the optional parameters. */
    icf_uint32_t        bit_mask;

#define ICF_SET_CALL_PRM_DEFAULT_SETTINGS                0x01
#define ICF_SET_CALL_PRM_QOS_MARK                        0x02    
#define ICF_SET_CALL_PRM_SETUP_TIMER                     0x04
#define ICF_SET_CALL_PRM_ALERTING_TIMER                  0x08
#define ICF_SET_CALL_PRM_RELEASE_TIMER                   0x10
#define ICF_SET_CALL_PRM_MODIFY_MEDIA_TIMER              0x20
#define ICF_SET_CALL_PRM_REGISTRATION_RESP_DURATION      0x40
#define ICF_SET_CALL_PRM_REGISTRATION_RETRY_DURATION     0x80

#ifdef ICF_SESSION_TIMER
#define ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL          0x0200
#define ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL      0x0400
#endif

#define ICF_SET_CALL_PRM_INFO_TIMER						 0x0800 	
#define ICF_SET_CALL_PRM_PHONE_INFO						 0x1000  
#define ICF_SET_CALL_PRM_OPTIONS_TIMER	            	 0x2000
#ifdef ICF_NAT_RPORT_SUPPORT
#define ICF_SET_BINDING_REFRESH_TIMER                    0x4000
#endif
#define ICF_SET_CALL_PRM_UPDATE_REQD	            	 0x8000
#define ICF_SET_CALL_PRM_EARLY_INC_IND_FOR_ALL_USERS     0x10000
#define ICF_SET_CALL_PRM_MULTIPLE_ANSWER_ENABLE          0x20000
#define ICF_SET_CALL_PRM_DNS_MIN_TIMER                   0x40000
#define ICF_SET_CALL_PRM_DNS_MAX_TIMER                   0x80000
    
#ifdef ICF_SESSION_TIMER
#define ICF_SET_CALL_PRM_SESSION_TIMER_ENABLE            0x010000
#define ICF_SET_CALL_PRM_SESSION_REFRESHER               0x020000
#define ICF_SET_CALL_PRM_SESSION_TIMER_LOGIC             0x040000
#endif
/*  Bitmask for the optional field added port_to_be_sent_in_from_hdr */
#define ICF_SET_PORT_IN_FROM_HDR                        0x080000
/*  Bitmask for the optional parameter app_connect_reqd_for_replaces */
#define ICF_SET_CALL_PRM_APP_CONNECT_REQD_FOR_REPLACES 	0x100000

    icf_uint16_t           default_settings;
#define ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY              0x01
#define ICF_CALL_CONFIG_PRIVACY_HEADER_PRESENT           0x02
#define ICF_CALL_CONFIG_PRACK_REQD                       0x04
#define ICF_CALL_CONFIG_CALL_MODIFY_METHOD_UPDATE        0x08
#define ICF_CALL_CONFIG_EARLY_MEDIA_HDR_SUPPORTED        0x10
#ifdef ICF_SESSION_TIMER
#define ICF_SET_CALL_PRM_SESSION_REFRESHER_NOT_REQUIRED  0x20
#endif
/* This bit indicates whether the stale parameter in the 
   401/407 response has to be ignored or not.
   If it is set,IPTK will simply ignore the stale parameter
   in 401/407 challenge response and recalculate the response
   if there exists a new nonce value.
*/
#define ICF_IGNORE_STALE_PARAM_IN_CHALLENGE              0x40
    icf_qos_mark_t        qos_mark;

    icf_duration_t        setup_timer ;

    icf_duration_t        alerting_timer ;

    icf_duration_t        release_timer ;

    icf_duration_t        modify_media_timer ;

    icf_duration_t        registration_resp_timer ;

    icf_duration_t        registration_retry_timer ;

	icf_duration_t		  info_timer;

	icf_duration_t		  options_timer;

	icf_string_st		  phone_info;

#ifdef ICF_SESSION_TIMER
     icf_duration_t       session_timer;

     icf_duration_t       min_se;
     
     /* Following boolean 'session_timer_enable is added in this structure that   
      * will be indicate whether session timer needs to be enabled or not. If it is 1 then session  
      *  timer feature should be enabled, if it 0 then session timer needs to be disabled.
      */
     icf_boolean_t               session_timer_enable;

     /*
      * A new variable will be added in that will specify the refresher whether it is uac, uas or    
      * none. Through this, application can configure the refresher. This variable will be   
      * checked only if ICF_SET_CALL_PRM_SESSION_REFRESHER bitmask is set.
      * As this API will be fired, IPTK will update session refresher on each line as specified 
      * by application.  
      */
     icf_refresher_type_t        session_refresher;


     /* This bitmask specifies the timer logic for starting session timer when IPTK is
      * acting as UAS. 
      */
     icf_boolean_t               session_timer_logic; 
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
     icf_duration_t       binding_refresh_timer;
     icf_boolean_t        binding_query_enable;
#endif

     icf_boolean_t        multiple_answer_enable;
     icf_duration_t          dns_min_retrans;
     icf_duration_t          dns_max_retrans;

     icf_uint8_t          update_reqd;
#define ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC        0
#define ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC      1
#define ICF_NO_UPDATE                                       2

    icf_boolean_t         early_inc_ind_for_all_users;
/*  THis field will indicate that self port would be sent in subsequent 
    requests or Not
    True: means port should be sent
    false: Port should not be sent 
*/
    icf_boolean_t          port_to_be_sent_in_from_hdr;

	/* This parameter indicates whether IPTK should directly send 200 OK for 
	 * INVITE with Replaces and create the media session, or present INVITE with
	 * Replaces as a normal incoming call and wait for APP request to connect 
	 * the call.
	 * -  ICF_TRUE means call will be connected on getting ICF_CONNECT_REQ.
	 * -  ICF_FALSE means call will be connected by IPTK.
	 * Default value for this boolean is ICF_FALSE.
	 */
	icf_boolean_t			app_connect_reqd_for_replaces;

} icf_set_call_params_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_get_system_settings_req_st   
 * Description: This data type is used to define a structure, which contains 
 *              the request by application to fetch system settings.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{

    /* Whether ICF supports PRACK or not */
#define ICF_REQD_PRACK_ENABLED_STATUS                0x00000001
    
    /* Authentication type (IMS or non-IMS mode)*/
#define ICF_REQD_AUTH_TYPE_STATUS                    0x00000002
    
    /* Privacy is supported in SIP signaling or not */
#define ICF_REQD_PRIVACY_SUPPORTED                   0x00000004
    
    /* Maximum Lines supported by ICF */
#define ICF_REQD_MAX_LINES                           0x00000008
    
    /* The value of Setup Timer */
#define ICF_REQD_SETUP_TIMER                         0x00000010
    
    /* The value of Alerting Timer */
#define ICF_REQD_ALERTING_TIMER                      0x00000020
    
    /* The value of Release Timer */
#define ICF_REQD_RELEASE_TIMER                       0x00000040
    
    /* The value of Media Modify Timer */
#define ICF_REQD_MODIFY_MEDIA_TIMER                  0x00000080
    
    /* The value of Register response Timer */
#define ICF_REQD_REG_RESP_TIMER                      0x00000100
    
    /* The value of Register retry Timer */
#define ICF_REQD_REG_RETRY_TIMER                     0x00000200
    
    /* The value of Subscribe retry Timer */
#define ICF_REQD_SUBS_RETRY_TIMER                    0x00000400
    
    /* The value of Subscribe response Timer */
#define ICF_REQD_SUBS_RESP_TIMER                     0x00000800
    
    /* Whether signal compression is enabled in ICF */
#define ICF_REQD_SIG_COMP_STATUS                     0x00001000
    
    /* Whether security is enabled in ICF */
#define ICF_REQD_SECURITY_STATUS                     0x00002000
    
    /* What is the IP Address of ICF */
#define ICF_REQD_SELF_IP_ADDRESS                     0x00004000
    
    /* what is the list of Associated URIs list */
#define ICF_REQD_ASSO_URI_LIST                       0x00008000
    
    /* what is the list of APNs list */
#define ICF_REQD_APN_LIST                            0x00010000
    
    /* what is self transport_mode */
#define ICF_REQD_SELF_TRANSPORT_MODE                 0x00020000
    
    /* what is self port */
#define ICF_REQD_SELF_PORT                           0x00040000
    
    /* Whether call to be routed thru proxy */
#define ICF_REQD_CALL_THRU_PROXY_STATUS              0x00080000
    
    /* Whether SIP compact headers to be used */
#define ICF_REQD_SIP_COMPACT_HDR_STATUS              0x00100000
    
    /* what is QOS mark value*/
#define ICF_REQD_QOS_MARK                            0x00200000
    
    /* what is session interval timer duration */
#define ICF_REQD_SESSION_INTERVAL_TIMER              0x00400000
    
    /* what is minimum session interval timer duration */
#define ICF_REQD_MIN_SESSION_INTERVAL_TIMER          0x00800000
    
    icf_uint32_t                    bit_mask;

#ifdef ICF_NAT_RPORT_SUPPORT

#define ICF_REQD_BINDING_QUERY_ENABLED_STATUS         0x1000000
#define ICF_REQD_BINDING_REFRESH_TIMER                0x2000000

#endif
    
    /* Specifies the presence of the routing information */
    /* MSbit WILL BE SET FOR ROUTING info */
#define ICF_GET_SETTINGS_ROUTE_INFO_PRESENT          0x8000000
    icf_void_st                    routing_info;

} icf_get_system_settings_req_st; 

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_get_system_settings_resp_st   
 * Description: This data type is used to define a structure, which contains 
 *              the response to be sent 
 *              to application for system settings requested.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{

    /* Result of this API - Success of failure */
    /* MSbit WILL BE SET For Success  */
#define ICF_API_GET_SYS_STG_RESULT_SUCCESS    0x8000000

    icf_uint32_t                          bit_mask;

    icf_boolean_t                         prack_enabled;
    icf_uint16_t                          auth_type;
    icf_boolean_t                         privacy_supported;
    icf_uint16_t                          max_lines;
    icf_duration_t                        setup_timer;
    icf_duration_t                        alerting_timer;
    icf_duration_t                        release_timer;
    icf_duration_t                        modify_media_timer;
    icf_duration_t                        registration_resp_timer;
    icf_duration_t                        registration_retry_timer;
    icf_duration_t                        subs_retry_timer;
    icf_duration_t                        subs_resp_timer;
    icf_boolean_t                         sig_comp_enabled;
    icf_boolean_t                         security_enabled;
    icf_transport_address_st              self_ip_address;
    icf_pub_uri_list_node_st              *p_uri_list;
    icf_apn_list_st                       apn_list;
    icf_transport_mode_t                  self_transport_mode;    
    icf_uint16_t                          self_port;            
    icf_boolean_t                         call_thru_proxy;        
    icf_boolean_t                         sip_compact_header;    
    icf_qos_mark_t                        qos_mark;
    icf_duration_t                        session_interval_timer;    
    icf_duration_t                        min_session_interval_timer;
#ifdef ICF_NAT_RPORT_SUPPORT
    icf_duration_t                        binding_refresh_timer;
    icf_boolean_t                         binding_query_enable;
#endif

} icf_get_system_settings_resp_st;

/*############################################################## */

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_get_line_settings_req_st   
 * Description: This data type is used to define a structure, which contains 
 *              the request by applications to fetch line settings of ICF.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* For current line, what is Proxy-CSCF's address */
#define ICF_REQD_PROXY_ADDR                             0x00000001
    
    /* For current line, what is Registrar's address */
#define ICF_REQD_REGISTRAR_ADDR                         0x00000002
    
    /* For current line, what is transfer mode (UDP/TCP/Both) */
#define ICF_REQD_PROXY_TRANSPORT_MODE                   0x00000004
    
    /* For current line, what is self address list */
#define ICF_REQD_SELF_ADDR_LIST                         0x00000008
    
    /* For current line, what is self name */
#define ICF_REQD_SELF_NAME                              0x00000010
    
    /* For current line, whether presentation is allowed */
#define ICF_REQD_PRESENTATION_STATUS                    0x00000020
    
    /* For current line, what is authentication key */
#define ICF_REQD_AUTH_KEY                               0x00000040
    
    /* For current line, what is authentication password */
#define ICF_REQD_AUTH_PASSWD                            0x00000080

    /* For current line, what is registration duration */
#define ICF_REQD_REG_MODE                               0x00000100

    /* For current line, what is registration duration */
#define ICF_REQD_REG_DURATION                           0x00000200

    /* For current line, what is the next hop for registration request */
#define ICF_REQD_REG_ROUTE_ADDR                         0x00000400

    /* what is SIP T1 timer duration */
#define ICF_REQD_SIP_T1_TIMER                           0x00000800

   /* what is SIP B timer duration */
#define ICF_REQD_SIP_B_TIMER                            0x00001000

   /* what is SIP F timer duration */
#define ICF_REQD_SIP_F_TIMER                            0x00002000

    /* Note: Do not use 0x10000000, it has been used in response structure */

    icf_uint32_t               bit_mask;

    icf_line_id_t               line_id;
/* Specifies the presence of the routing information */
/* Defined in icf_get_system_settings_req_st, So commented*/
/*#define ICF_GET_SETTINGS_ROUTE_INFO_PRESENT 0x8000000*/

    icf_void_st             routing_info;

} icf_get_line_settings_req_st; 

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_get_line_settings_resp_st   
 * Description: This data type is used to define a structure, which contains 
 *              the request by applications to fetch line settings of ICF.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{

    /* Result of this API - Success of failure */
#define ICF_API_GET_LINE_STG_RESULT_SUCCESS    0x10000000

    icf_uint32_t                    bit_mask;

    icf_transport_address_st        proxy_address;
    icf_transport_address_st        registrar_address;
    icf_transport_mode_t            proxy_transport_mode;
    icf_address_st                  self_address_list;
    icf_string_st                   self_name ;
    icf_boolean_t                   presentation_allowed ;
    icf_string_st                   auth_key ;
    icf_string_st                   auth_password ;
    icf_transport_mode_t            reg_mode ;
    icf_duration_t                  reg_duration ;
    icf_transport_address_st        reg_route_addr;
    icf_duration_t                  sip_T1_timer;
    icf_duration_t                  sip_B_timer;
    icf_duration_t                  sip_F_timer;

} icf_get_line_settings_resp_st;

/* ############################################################## */

/* ICF_CONF_STATUS_API_END */


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_configure_cfm_st 
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_CONFIGURE_CFM 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Denotes the API identifier for which the response is sent. */
    icf_api_id_t api_id;

    /* SPR 15997 : Line id in configure confirm API */
    /* Denotes the line_id identifier for which the response is sent. */
    icf_line_id_t line_id;

    /* Denotes the Success or Failure status of the execution of the API. */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure.*/
    icf_error_t error_cause;

} icf_configure_cfm_st;


/*********************
* CONTROL PLANE APIs *
* ********************/

/* These hash defines are used for indicating presence of some common optional 
 * parameters in the ICF APIs. These values should be used in the bit_mask if
 * any of the elements is to be included in an API structures containing
 * header/tag/msg-body/privacy-ind/pref-identity 
 */
#define ICF_PRIVACY_IND_PRESENT       0x01000000
#define ICF_PREF_IDENTITY_PRESENT     0x02000000
#define ICF_HDR_LIST_PRESENT          0x04000000
#define ICF_TAG_LIST_PRESENT          0x08000000
#define ICF_BODY_LIST_PRESENT         0x10000000
#define ICF_STREAM_LIST_PRESENT       0x20000000


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_register_req_st
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_REGISTER_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This indicates the presence of the optional parameters. */

    icf_uint32_t bit_mask;

#define ICF_REG_REQ_TO_PRESENT 0X01
#define ICF_REG_REQ_FROM_PRESENT 0x02
#define ICF_REG_REQ_EXPIRY_PRESENT 0x04
#define ICF_REG_REQ_DEREG_SPECIFIC_CONTACT_PRESENT 0x08
    
    /* Indicates the line to be used for the outgoing call. Always coded 
     * as ICF_USE_LINE1
     */    
    icf_line_id_t line_id;

    /* This parameter specifies the public-URI to be registered. */
    icf_address_st to;

    /* This parameter specifies the expiry to be sent in the SIP REGISTER 
     * message.
     */
    icf_duration_t expiry ;

    /* List of additional headers that the application wants to send in the API. */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the API. */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the API. */
    icf_msg_body_list_st body_list ;

    /* SPR:20364 changes start */
    /* A boolean value used by the application to deregister a specific contact
     * in the API */
    icf_boolean_t dereg_specific_contact;
    /* SPR:20364 changes end */

} icf_register_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_create_call_req_st
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_CREATE_CALL_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t 			bit_mask;

#define ICF_CALLED_PARTY_NAME_PRESENT  0X00000001
#define ICF_TOGGLE_PRES_IND_PRESENT    0x00000002
#define ICF_QOS_MARK_PRESENT           0x00000004
#define ICF_CALLING_PARTY_ADDR         0x00000010
#define ICF_CALLING_PARTY_NAME         0x00000020
	/* ICF REL 6.0: new bitmask for the pref_media_profile from APP */
#define ICF_PREF_MEDIA_PROFILE_PRESENT 0x00000040
#define ICF_PRIVACY_HDR_FLAG_PRESENT   0x00000080
/* new bitmask for requesting resource reservation at call time */
#define ICF_RESOURCE_RESERV_FLAG_PRESENT	0x00000100

    /* Use common hash defines if header/msg-body/tag/stream 
     * list present in the API. 
     */

    /* Indicates the line to be used for the outgoing call. Always 
     * coded as ICF_USE_LINE1
     */
    icf_line_id_t			line_id;

    /* Indicates the E.164 number or SIP URL of the remote user. */
    icf_address_st 			called_party_addr;
    /* Indicates the name of the remote user entered by local user when
     * dialing the SIP URI
     */
    icf_name_st 			called_party_name;
    /* Indicates whether the local user wants to toggle the presentation
     * indication status for the current call.
     */
    icf_boolean_t 			toggle_presentation_indicator;
    /* Indicates the identifier to be sent for the current call in the
     * type of service flag.
     */
    icf_qos_mark_t 			qos_mark ;
    /* Indicates the Preferred Identity, which shall be used at the
     * P-CSCF for asserted identity in trusted networks.
     */
    icf_address_st 			pref_identity;
    icf_address_st  		calling_party_address;
    icf_string_st  			calling_party_name;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st 		header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st 		tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st 	body_list;

	/* ICF REL 6.0: this will contain the preferred media profile for the 
	 * application. This is an optional entity in the request. If the 
	 * app, sends this information, then ICF shall use this info to form 
	 * the SDP in the outgoing Invite. This profile will over-ride the info
	 * from APP as set in the media profile.
	 */
	/* this will be a list of icf_stream_capab_st */
	icf_list_st				*p_pref_media_profile;

    /* In case outgoing INVITE carries Privacy header, this flag indicates
     * whether Privacy header should be sent in BYE, REFER, RE-INVITE also.
     * The default value is ICF_FALSE */
    icf_boolean_t           privacy_hdr_all_req;

	/* indicates whether the local user wants to enable QoS precondition 
	 * feature */
	icf_boolean_t			resource_reservation_reqd;
	
} icf_create_call_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_local_user_alerted_req_st
 *
 * Description: This API is invoked by Application towards the IMS Module
 *                in response to the ICF_INCOMING_CALL_IND API.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
		/* icf REL 6.0: multiple m lines */
#define ICF_USER_ALERTED_DELETED_STREAM_PRESENT		0x01
#define ICF_USER_ALERTED_ALERT_TIMER_PRESENT		0x02
#define ICF_USER_ALERTED_EARLY_MEDIA_SUPPORTED          0x04
#define ICF_USER_ALERTED_MEDIA_MODE_PRESENT             0x08

    icf_uint32_t bit_mask;
    /* Use common hash defines if privacy/header/msg-body/tag/stream 
     * list present in the API. 
     */

	/* if early_media_reqd is ICF_FALSE then ICF will not send
	 * 183 with SDP/180 with SDP
	 */
	icf_boolean_t early_media_reqd;
    
	/* Gives the status of inband info. ICF will ignore this value
	 * If early_media_reqd is ICF_FALSE
	 */
    icf_inband_info_status_t inband_info_status;

    /* Indicates whether privacy is required. */
    icf_boolean_t privacy_ind;

    /*Alert timer duration*/
    icf_duration_t	alert_timer_duration;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;

    /* List of encoded_attributes for a media stream that application wishes 
     * to set in the outgoing response to the remote offer. Node is of type
     * icf_stream_capab_st */
    icf_list_st			*p_stream_list;

	/* icf REL 6.0: multiple m lines */
	/* the application may delete any stream during the call setup. This is 
	 * optional. The application will only return the rejected/deleted stream 
	 * information. The application will not be allowed to change/modify any
	 * codecs or any other parameter within the stream. The application if 
	 * desires can reject a stream only */
	/* the application should not indicate those stream_id's that have been
	 * already deleted in the onging session */
	icf_stream_id_t	         count;
	icf_stream_list_st       deleted_media_streams[ICF_MAX_STREAM_PER_CALL];
           
      /*If the applications wants to send the P-Early-Media Header in 18x 
      * response, then application can specify the mode in this.If this mode is
      * different from the current mode, then IPTK do MMS for the mode.
      */ 
      icf_media_mode_t          early_mode;
      icf_media_mode_t          media_mode;
        
	
} icf_local_user_alerted_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_terminate_call_req_st
 *
 * Description: This API is invoked by Application towards IMS Module
 *                when the local user rejects an incoming call or terminate
 *                an ongoing call.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;
    #define ICF_CALL_CLEAR_RESPONSE_CODE_PRESENT      0x00000002

    /* Indicates the reason for terminating the call. */
    icf_call_clear_reason_t call_clear_reason;

    /* Denotes the line ID on which an active call is to be cleared. */
    icf_line_id_t line_id;

   /* Indicates the response code to be sent to peer for terminating the call */
    icf_uint16_t response_code;
} icf_terminate_call_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_connect_req_st
 *
 * Description: This API is invoked by Application to IMS Module when the
 *                local user accepts the call.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
	/* icf REL 6.0: multiple m lines */
#define ICF_CONNECT_DELETED_STREAM_PRESENT		0x01

    icf_uint32_t bit_mask;
    /* Use common hash defines if privacy/header/msg-body/tag/stream 
     * list present in the API. 
     */

    /* Indicates whether privacy is required. */
    icf_boolean_t privacy_ind;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;


    /* List of encoded_attributes for a media stream that application wishes 
     * to set in the outgoing response to the remote offer. Node is of type
     * icf_stream_capab_st */
    icf_list_st			*p_stream_list ;

	/* icf REL 6.0: multiple m lines */
	/* the application may delete any stream during the call setup. This is 
	 * optional. The application will only return the rejected/deleted stream 
	 * information. The application will not be allowed to change/modify any
	 * codecs or any other parameter within the stream. The application if 
	 * desires can reject a stream only */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
} icf_connect_req_st;


/* icf REL 6.0: multiple m lines */
/*-----------------------------------------------------------------------------
 *
 * Structure Name:	icf_connect_resp_st
 *
 * Description: 	This API is invoked by ICF towards the Application to 
 * 					indicate that an ACK has been received for the incoming
 * 					cal. The API shall also inc. the commited media information
 * 					for the call.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
#define ICF_CONNECT_RESP_DELETED_STREAM_PRESENT		0x01
    icf_uint32_t 			bit_mask;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_call_modify_media_data__st */
	icf_list_st				*p_accepted_stream_list;

	/* icf REL 6.0: multiple m lines */
	/* the application may delete any stream during the call setup. This is 
	 * optional. The application will only return the rejected/deleted stream 
	 * information. The application will not be allowed to change/modify any
	 * codecs or any other parameter within the stream. The application if 
	 * desires can reject a stream only */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
             
       /* List of additional headers that the application wants to send in the
        * message request
        */
      icf_header_list_st     header_list;
 
} icf_connect_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_subscribe_req_st
 *
 * Description: This API is used by the application to subscribe to a
 *                particular event package.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;

#define ICF_SUB_REQ_DEST_INFO_PRESENT 0X01
#define ICF_SUB_REQ_EVENT_PRESENT 0x02
#define ICF_SUB_REQ_EXPIRES_HDR_NOT_TO_BE_SEND 0x04
#define ICF_SUB_REQ_RESUBSCRIPTION_NOT_REQUIRED 0x08
    
    /* Use common hash defines if header/msg-body/tag list present */
	icf_line_id_t line_id;

    /* Indicates the SIP URL of the remote entity to which subscription 
     * request has to be sent.
     */
    icf_address_st destination_info;

    /* Package name for which subscription request has to be sent. */
    icf_string_st event_pkg_name;

    /* Defines the duration for this subscription. */
    icf_uint32_t duration;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;

} icf_subscribe_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_publish_req_st
 *
 * Description: This API is used by the application for the publication of
 *                event state to a particular Event State Compositor ( ESC ).
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;

    /* Use common hash defines if header/msg-body/tag list present */

	/* Indicates the SIP URL of the remote entity to which publication 
     * request has to be sent.
     */
    icf_line_id_t line_id;

    /* Indicates the user on the line for which publish has to be sent */
    icf_address_st user_address;
#define ICF_USER_ADDRESS_PRESENT       0x01

    /* indicates the destination URL */
    icf_address_st destination_info;

    /* Package name for which publication request has to be sent. */
    icf_string_st event_pkg_name;

    /* Defines the duration for this publication. */
    icf_uint32_t duration;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;

} icf_publish_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_message_req_st
 *
 * Description: This API is used by the application to send an instant 
 *                message or a group advertisement
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;
    /* Use common hash defines if header/msg-body/tag list present */

	icf_line_id_t line_id;

    /* Indicates the SIP URL of the remote user to which MESSAGE request has
     *  to be sent.
     */

    /* Indicates the user on the line for which message request has to be sent */
    icf_address_st user_address;
#define ICF_USER_ADDRESS_PRESENT       0x01

    icf_address_st destination_info;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;

} icf_message_req_st;

/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_refer_req_st

 *

 * Description: This API is used by the application to refer an entity 

 *                to a resource .

 *

 *---------------------------------------------------------------------------*/



typedef struct

{



    icf_uint32_t bit_mask;

    
    #define ICF_REF_REQ_DEST_INFO_PRESENT 0X01
    #define ICF_REF_REQ_SUBS_REQD_PRESENT 0x02

    /* Use common hash defines if header/msg-body/tag list present */

	icf_line_id_t line_id;

    /* Indicates the SIP URL of the remote user to which refer request

     * has to be sent.

     */

    icf_address_st destination_info;



    /* Resource that the addressed user is referred to. */

    icf_address_st refer_to ;



    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for

     * asserted identity in trusted networks.

     */

    icf_address_st pref_identity;



    /* Identifies whether a Subscription needs to be created with this REFER

     * request.

     */

    icf_boolean_t subscription_required ;



    /* List of additional headers that the application wants to send in 

     * the refer request

     */

    icf_header_list_st header_list;



    /* List of additional tags that the application wants to send in the refer

     * request.

     */

    icf_tag_list_st tag_list;



    /* List of message bodies that the application wants to send in the refer

     * request.

     */

    icf_msg_body_list_st body_list;

    

} icf_refer_req_st;











/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_register_cfm_st

 *

 * Description: This API is used by the ICF to indicate an register confirm event

 *                to the application.

 *

 *---------------------------------------------------------------------------*/



typedef struct

{



    icf_uint32_t bit_mask;

    #define   ICF_REG_CFM_ERROR_CAUSE_PRESENT    0x001

    #define   ICF_REG_CFM_ASSO_URI_PRESENT        0x002

    #define   ICF_REG_CFM_TO_PRESENT               0X004

    #define   ICF_REG_CFM_FROM_PRESENT              0x008

    /* Use common hash defines if header/msg-body list present */



    /* Indicates the type of ongoing registration procedure, */

    icf_uint8_t request_type ;



    /* Denotes the Failure status only. */

    icf_result_t result;



    /* Denotes the error cause if the status of the API is Failure. */

    icf_error_t error_cause ;



    /* Public uri and P- Associated uri list */
    icf_pub_uri_list_node_st   *p_uri_list;



    /* Indicates the line to be used for the outgoing call. 

     * Always coded as ICF_USE_LINE1

     */

    icf_line_id_t line_id;



    /* This parameter specifies the public-URI that is being registered. */

    icf_address_st to;


    /* List of additional headers that the application wants to send in the API */

    icf_header_list_st header_list;



    /* List of message bodies that the application wants to send in the API. */

    icf_msg_body_list_st body_list ;

    

} icf_register_cfm_st;















/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_register_status_ind_st
 *
 * Description: IMS Client Framework sends unsolicited registration failure
 *                indications to the application for all re-registrations.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;
#define   ICF_REG_IND_ERROR_CAUSE_PRESENT    0x001
#define   ICF_REG_IND_ASSO_URI_PRESENT       0x002
#define   ICF_REG_IND_TO_PRESENT             0X004
#define   ICF_REG_IND_FROM_PRESENT           0x008
    /* Use common hash defines if header list present */
    /* Indicates the type of ongoing registration procedure, */
    icf_uint8_t request_type ;

    /* Denotes the Failure status only. */
    icf_result_t result;

    /* Denotes the error cause if the status of the API is Failure. */
    icf_error_t error_cause ;

    /* Public uri and P- Associated uri list */
    icf_pub_uri_list_node_st   *p_uri_list;

    /* Indicates the line to be used for the outgoing call. */
    icf_line_id_t line_id;

    /* This parameter specifies the public-URI that is being registered. */
    icf_address_st to;

    /* List of additional headers that are of importance to the application.*/
    icf_header_list_st header_list;
} icf_register_status_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_incoming_call_ind_st
 *
 * Description: This API is sent by IMS Module to inform Application about
 *                an incoming call from network, so that Application can update
 *                its UI display to indicate the same to the local user.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask ;
#define ICF_TYPE_OF_CALL_IND_PRESENT        0x01
#define ICF_CALL_WAITING_IND_PRESENT        0x02
#define ICF_CALLING_ADD_ASRTD_IND_PRESENT   0x04
#define ICF_CALLING_PARTY_ADD_PRESENT       0x08
#define ICF_CALLING_PARTY_NAME_PRESENT      0x10
#define ICF_CALLED_PARTY_ADD_PRESENT        0x20
#define ICF_TRANSF_PARTY_ADD_PRESENT        0x40
#define ICF_TRANSF_PARTY_NAME_PRESENT       0x80
	/* ICF REL 6.0: multiple m lines */
#define ICF_REJECTED_MEDIA_STREAM_PRESENT	0x0100

    /* Use common hash defines if header/msg-body/stream list present */
    /* Denotes the line ID on which the local user has received the call. */
    icf_line_id_t 				line_id;

	/* For the current ICF release, this field will be a bitmap and not an
	 * enumeration. This is to facilitate the large combination of call types
	 * possible in this release. */
    /* Indicates the type of call. */
	icf_uint8_t					call_type;

    /* Indicates whether the incoming call is in call waiting. */
    icf_boolean_t 				call_waiting_ind;

    /* Indicates whether the originator (calling party) identity is 
     * asserted/verified by registrar. 
     */
    icf_boolean_t 				calling_party_addr_asserted;

    /* Denotes the address of the remote party calling the local user. */
    icf_address_st 				calling_party_addr;

    /* Denotes the name of the remote party calling the local user. */
    icf_name_st 				calling_party_name;

    /* Denotes the address of the local user. */
    icf_address_st 				called_party_addr;

    /* Contains the address of the transferring party if the incoming
     * call is a transferred call.
     */
    icf_address_st 				transferring_party_addr;

    /* Contains the name of the transferring party if the incoming call
     * is a transferred call.
     */
    icf_name_st 				transferring_party_name;

    /* List of additional headers that are of importance to the application. */
    icf_header_list_st 			header_list;

    /* List of additional message bodies that are of importance to the 
	 * application. */
    icf_msg_body_list_st 		body_list ;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_stream_capab_st */
	icf_list_st					*p_accepted_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t				count;
	icf_stream_list_st			deleted_stream_list[ICF_MAX_STREAM_PER_CALL];

    /*Following field will contain the internal call ID of the call 
     * present in Join Header*/ 
    icf_call_id_t       call_id_to_be_joined;
} icf_incoming_call_ind_st;



/*-----------------------------------------------------------------------------
*
* NAME :		 icf_info_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*                which specifies payload data of API
*                ICF_INFO_REQ.
-----------------------------------------------------------------------------*/

typedef struct
{
    icf_uint32_t		bit_mask;
        
#define   ICF_HDR_LIST_PRESENT   	0x04000000
#define   ICF_TAG_LIST_PRESENT 	    0x08000000
#define   ICF_BODY_LIST_PRESENT    	0x10000000
        
    icf_header_list_st	    header_list;
    icf_tag_list_st		    tag_list;
    icf_msg_body_list_st	body_list;

} icf_info_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_remote_user_alerted_ind_st
 *
 * Description: This API is sent by IMS Module to inform Application that
 *                the remote party has been alerted. This API is invoked after
 *                ICF receives the 180 ringing response from remote party.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;
	/* ICF REL 6.0: multiple m lines */
#define ICF_REM_USER_ALRTED_DELETED_STREAM_PRESENT		0x01

    /* Use common hash defines if header/msg-body list present */
    /* Gives status of inband info */
    icf_inband_info_status_t inband_info_status;

    /* List of additional headers that are of importance to the application. */
    icf_header_list_st header_list;

    /* List of additional message bodies that are of importance to the 
	 * application. */
    icf_msg_body_list_st body_list ;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_stream_capab_st */
	icf_list_st					*p_accepted_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
} icf_remote_user_alerted_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_terminate_call_ind_st
 *
 * Description: This API is invoked by IMS Module towards Application when
 *                IM wants to clear a call on receiving SIP BYE/4xx/5xx/6xx 
 *                request from remote user or due to some local error.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask ;
#define ICF_ADDNL_INFO_PRESENT 0X01
#define ICF_REMOTE_CALL_CLEAR_RESPONSE_CODE_PRESENT 0x02
#define ICF_RETRY_AFTER_DURATION_PRESENT       0x40000000 
    /* Is used by IM to inform Application about the reason for terminating
     * the call. Application can use this reason to play the appropriate 
     * error tone or message on the UI display.
     */
    icf_call_clear_reason_t call_clear_reason;

    /* Specifies the additional information like display string for 
     * terminating the call.
     */
    icf_string_st additional_info;
   /* response code for peer initiated call clear */
    icf_uint16_t   response_code;

   /* duration received in retry-after header*/
   icf_duration_t    retry_after_duration;
} icf_terminate_call_ind_st;


/*-----------------------------------------------------------------------------
 *
 * API Name: ICF_REMOTE_USER_TERMINATE_CALL_RESP
 *
 * Description:  This API is used to indicate the response of 
 * icf_terminate_call_req API. This API does not contain any parameters.
 *
 *---------------------------------------------------------------------------*/

#ifdef ICF_NW_ACTIVATE
/*-----------------------------------------------------------------------------
 *
 * API Name: ICF_EVENT_INDICATION
 *
 * Description:  This API is used to indicate the ICF system/Network events to 
 * 				 the Appplication.  
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
#define ICF_NW_UP                           0x01
#define ICF_NW_DOWN                         0x02
#define ICF_NW_MEMORY_FAILURE               0x04
#define ICF_NW_CURRENTLY_NOT_AVAILABLE      0x08
#define ICF_NW_NOW_AVAILABLE                0x10
#define ICF_NW_SUSPENDED                   0x20
    /* Event code */
    icf_uint32_t    event_code ;

    /* This denotes the event specific data for the application.
       This is for future use */
    icf_void_st     event_data;
} icf_event_indication_st;
#endif
 
 /*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_media_connected_ind_st
 *
 * Description: This API signifies to the application that the media parameters 
 * are committed and media with remote user is connected. 
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    icf_uint32_t            bit_mask ;
	/* ICF REL 6.0: multiple m lines */
#define ICF_MEDIA_CONECT_DELETED_STREAM_PRESENT		0x01

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_stream_capab_st */
	icf_list_st					*p_accepted_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
} icf_media_connected_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_connect_ind_st
 *
 * Description: This API is invoked by IMS Module to Application after 
 *                receiving a 200 OK message from the network. This message is 
 *                sent as a response to the INVITE message generated after the 
 *                request for creating a new call is invoked. This API indicates
 *                that the remote user has accepted the call.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;

#define ICF_ASSERTED_ID_PRESENT			0x01
#define ICF_CONNECT_IND_DELETED_STREAM_PRESENT		0x02
    /* Use common hash defines if header/msg-body/stream list present */

    /* Indicates the Asserted Identity of the remote party. */
    icf_address_st asserted_identity;

    /* List of additional headers that are of importance to the application. */
    icf_header_list_st header_list;

    /* List of additional message bodies that are of importance to the 
	 * application. */
    icf_msg_body_list_st body_list ;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_call_modify_media_data_st */
	icf_list_st					*p_accepted_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	

    icf_boolean_t	   inband_dtmf_supported;

} icf_connect_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_subscription_status_ind_st
 *
 * Description: This API is used by the framework to inform the application
 *                about the status of the Subscription.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t bit_mask;
#define ICF_SUBS_ST_IND_RESP_CODE_PRESENT 0X01
#define ICF_SUBS_ST_IND_ERROR_CODE_PRESENT 0X02

    /* Use common hash defines if header list  present */
    icf_uint32_t error_code ;
    
    /* Indicates the status of the subscription */
#define    ICF_SUBS_ACCEPTED            0
#define    ICF_SUBS_SUBSCRIBED            1
#define    ICF_SUBS_FAIL_RETRY            2
#define    ICF_SUBS_FAIL_NO_RETRY        3
#define    ICF_SUBS_UNSUBSCRIBED        4

    icf_uint8_t status;
    icf_uint32_t duration ;

    /* Response code received for the SUBCRIBE request. */
    icf_uint32_t response_code ;

    /* List of headers received in the subscription response which are of
     * importance to the application.
     */
    icf_header_list_st header_list;

	/* Event package name for which subscribe request was sent */
    icf_string_st 		event_pkg_name;

} icf_subscription_status_ind_st;


















/*NOTIFY_OUT_OF_DIALOG_START*/
/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_notify_ind_st

 *

 * Description: This API is used by the framework to inform the application

 *                about an incoming NOTIFY.

 *

 *---------------------------------------------------------------------------*/


typedef struct

{

#define ICF_SUBS_DURATION_PRESENT		 0x01000
#define ICF_NOTIFY_IND_SENDER_PRESENT	 0x02000
#define ICF_NOTIFY_IND_OUT_OF_DIALOG	 0x04000
#define ICF_NOTIFY_IND_USER_ADDR_PRESENT 0x08000
#define ICF_NOTIFY_IND_STRAY             0x10000


    icf_uint32_t bit_mask;

	/*line ID on which the user is present */
	icf_line_id_t                  line_id;
        
    /* Package name for which the notification is received. */

    icf_string_st event_pkg_name;


#define ICF_SUBS_STATE_ACTIVE			1
#define ICF_SUBS_STATE_TERMINATED		0

    /* This is the subscription state received in notify */
    icf_uint8_t      subs_state;

#define ICF_NW_INC_NOTIFY_RETRY_IMM		1
#define ICF_NW_INC_NOTIFY_RETRY_AFTER	2
#define ICF_NW_INC_NOTIFY_NO_RETRY		3

    /* Reason code is present in case of terminated state */
    icf_uint8_t      reason_code;

    /* This duration can be the remaining subscription duration
     * in case of active state and retry after duration in case of
     * terminated state
     */
    icf_duration_t   duration;

    /* Denotes the address of the sender.The address of the remote party 
     * can be an E.164 number or a SIP URL.
     */

    icf_address_st sender;

	/* Denotes the address of the User.The address of the  party 
     * can be an E.164 number or a SIP URL.
     */

    icf_address_st user_address;

    /* List of headers received in the notification which are of importance to
     * the application.
     */
     icf_header_list_st header_list;



    /* List of message bodies received in the notification. */

    icf_msg_body_list_st body_list;

    

} icf_notify_ind_st;

/*NOTIFY_OUT_OF_DIALOG_END*/









/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_publish_resp_st

 *

 * Description: This API is used by the framework to inform the application.

 *                about the result of an earlier publish request issued by 

 *                the application or on PUBLISH timeout.

 *

 *---------------------------------------------------------------------------*/



typedef struct

{



    icf_uint32_t bit_mask;

    #define ICF_PUB_RESP_RESP_CODE_PRESENT 0X01

    #define ICF_PUB_RESP_ERROR_CODE_PRESENT 0x02

   
    /* Use common hash defines if header list  present */



    /* Indicates the result of the publication (success/failure). */

    icf_return_t result;

    icf_uint32_t error_code ;

    icf_uint32_t duration ;



    /* Response code received for the PUBLISH request. */

    icf_uint32_t response_code ;



    /* Entity-tag received from the ESC after a Initial publication. */

    icf_string_st entity_tag;



    /* List of headers received in the subscription response which are of

     * importance to the application.

     */

     icf_header_list_st header_list;

      /* duration received in retry-after header*/
     icf_duration_t    retry_after_duration; 

} icf_publish_resp_st;
















/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_message_resp_st

 *

 * Description: This API is used by the framework to inform the application 

 *                about the result of an earlier message request issued by 

 *                the application.

 *

 *---------------------------------------------------------------------------*/



typedef struct

{



    icf_uint32_t bit_mask;

#define ICF_MSG_RESP_RESP_CODE_PRESENT          0X01
#define ICF_MSG_RESP_ERROR_CODE_PRESENT        0x02

    /* Use common hash defines if header list  present */



    /* Indicates the result of the message attempt ( success/failure). */

    icf_return_t result ;


    icf_uint32_t error_code ;



    /* Response code received for the MESSAGE request. */

    icf_uint32_t response_code ;



    /* List of headers received in the message response which are of

     * importance to the application.

     */

     icf_header_list_st header_list;

     /* duration received in retry-after header*/
     icf_duration_t    retry_after_duration; 

} icf_message_resp_st;















/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_message_ind_st

 *

 * Description: This API is used by the framework to inform the 

 *                application about an incoming MESSAGE.

 *

 *---------------------------------------------------------------------------*/



typedef struct

{



    icf_uint32_t bit_mask;

    #define ICF_MSG_IND_SENDER_PRESENT 0X01

    /* Use common hash defines if header/msg-body list  present */



    /* Denotes the address of the local user.The address of the remote party 

     * can be an E.164 number or a SIP URL.

     */

    icf_address_st sender;



    /* List of headers received in the message which are of importance to

     * the application.

     */

    icf_header_list_st header_list;



    /* List of message bodies received in the message. */

    icf_msg_body_list_st body_list;

    

} icf_message_ind_st;















/*-----------------------------------------------------------------------------

 *

 * Structure Name:icf_refer_resp_st

 *

 * Description: This API is used by the framework to inform the application 

 *                about the result of an earlier REFER request issued by the 

 *                application.

 *

 *---------------------------------------------------------------------------*/



typedef struct

{

    

    icf_uint32_t bit_mask;

    
#define ICF_REF_RESP_RESP_CODE_PRESENT          0X01
#define ICF_REF_RESP_ERROR_CODE_PRESENT        0x02

    /* Use common hash defines if header list  present */



    /* Indicates the result of the REFER (success/failure). */

    icf_return_t result ;



    /* Response code received for the REFER request. */

    icf_uint32_t response_code ;


    /* error cause incase the status is Failure */
     icf_uint32_t error_code ;



    /* List of headers received in the REFER response which are 

     * of importance to the application.

     */

    icf_header_list_st header_list;

    /* duration received in retry-after header*/
    icf_duration_t    retry_after_duration; 

} icf_refer_resp_st;

/*----------------------------------------------------------------------------
 * NAME : ICF_cfg_cc_set_scale_params_req_st
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_CFG_CC_SET_SCALE_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
#define ICF_SET_SCALE_PRM_MAX_LINES                    0x0001
#define ICF_SET_SCALE_PRM_MAX_AUDIO_CALLS            0x0002
#define ICF_SET_SCALE_PRM_MAX_VIDEO_CALLS            0x0004
#define ICF_SET_SCALE_PRM_MAX_SERVICE_CALLS            0x0008
#define ICF_SET_SCALE_PRM_MAX_REGISTRATION_CALLS    0x0010
#define ICF_SET_SCALE_PRM_MAX_USERS_PER_LINE        0x0020
#define ICF_SET_SCALE_PRM_NUM_OF_CALLS_TO_LOG        0x0040
#define ICF_SET_SCALE_PRM_MAX_NUM_OF_APPS            0x0080

     icf_uint16_t            bit_mask;

     /*Maximum number of lines supported by phone*/
     icf_line_id_t      max_lines;
     
     /*Max number of simultaneous calls*/
     icf_uint16_t       max_audio_calls;

     /*Max number of simultaneous video calls*/
     icf_uint16_t       max_video_calls;

     /*Max number of simultaneous service calls*/
     icf_uint16_t       max_service_calls;

     /*Max number of simultaneous registration calls done by phone*/
     icf_uint8_t        max_registration_calls;

     /*Max number of users configured per line*/
     icf_uint8_t        max_users_per_line;

    /* Number of calls to be loged in call log history */
    icf_uint8_t                num_of_calls_to_log;

    /*Indicates the maximum number of applications to be 
    supported which can run simultaneously. 
    The default value of this parameter is 2*/
    icf_uint8_t            max_num_of_apps;

} icf_set_scale_params_req_st;


/*----------------------------------------------------------------------------
 * NAME : icf_trace_nw_buff_ind_st
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_TRACE_NW_BUFF_IND
 *
 *---------------------------------------------------------------------------*/
typedef struct {
    icf_transport_address_st   nw_address;
    icf_generic_string_st       nw_buffer;
}icf_trace_nw_buff_ind_st;


/*-----------------------------------------------------------------------------
*
* NAME : icf_call_transfer_unattended_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ.
-----------------------------------------------------------------------------*/

typedef struct
{
    
     /* This is the E.164 number or SIP URI of transferred party.*/
    icf_address_st transferred_party_addr;
    
    /* This field bitwise contains the presence information of optional fields 
     * in this structure.*/

#define ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR 0x01

#define ICF_ADDRESS_ADDNL_URI_PARAMTR_PRES 0x02
    icf_uint32_t bit_mask;
    
    /* This parameter indicates whether user wants to toggle the presentation 
     * indication status for rest of the call.This is optional field.
     * presence_flag: 
     * ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR 
     */
    icf_boolean_t           toggle_presentation_indicator;

    icf_header_list_st	    header_list;
    icf_tag_list_st	    tag_list;
    icf_msg_body_list_st    body_list;
    icf_large_string_st     *p_uri_parameter;
} icf_call_transfer_unattended_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_call_transfer_attended_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_CALL_TRANSFER_ATTENDED_REQ.
-----------------------------------------------------------------------------*/

typedef struct
{

    /* This is call id of call put on hold by IP Phone user.*/
    icf_call_id_t transferred_party_id; 

    icf_uint32_t		bit_mask;

    icf_header_list_st	header_list;
    icf_tag_list_st		tag_list;
    icf_msg_body_list_st	body_list;
}icf_call_transfer_attended_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_remote_call_transfer_initiated_ind_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_REMOTE_CALL_TRANSFER_INITIATED_IND.
-----------------------------------------------------------------------------*/

typedef struct
{

    /* This is the new call id generated when remote party has initiated call 
     * transfer by sending REFER.*/
    icf_call_id_t new_call_id;        

    /* This is the number of the party to which the call transfer is initiated.
     *  This is received in REFER message.                                      
     */
    icf_address_st referred_party_addr;   
    
    /* This field bitwise contains the presence information of optional fields 
     * in this structure.*/

#define ICF_REFER_INITIATED_REFERRED_PARTY_NAME    0x01

    icf_uint32_t bit_mask;        
    
    /* This parameter contain the name of the party in REFER message.This is 
     * optional field.
     * Presence_flag: ICF_REFER_INITIATED_REFERRED_PARTY_NAME (0x1)
     */
    icf_name_st                 referred_party_name;     

    icf_header_list_st	        header_list;
    icf_msg_body_list_st		body_list;
    
} icf_remote_call_transfer_initiated_ind_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_call_merged_ind_st
*
* DESCRIPTION  : This data type is used to define a structure,
*                which specifies payload data of API
*                ICF_CALL_MERGED_IND.
-----------------------------------------------------------------------------*/

typedef struct
{

   /* This is call id of “referred to party in REFER. This call_id is same as
    * new_call_id in ICF_REFER_INTIATED API.
    */
    icf_call_id_t merge_call_id;   

} icf_call_merged_ind_st; 

/*-----------------------------------------------------------------------------
*
* NAME : icf_conference_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*              which specifies payload data of API  ICF_CONFERENCE_REQ.
-----------------------------------------------------------------------------*/

typedef struct
{
   icf_uint32_t            bit_mask;

   /* Identifier of second call which need to be merged with the call with 
    * call_id in API header. */
   icf_call_id_t           merge_call_id;   
   icf_header_list_st      header_list;
   icf_tag_list_st         tag_list;
}icf_conference_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_cfg_cc_set_service_params_req_st 
 *
 * Description: This structure specifies payload data of 
 *              API ICF_SET_SERVICE_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{

#define ICF_SET_SER_PRM_SUBSCRIPTION_FLAG       0x00000001
#define ICF_SET_SER_PRM_ACTIVATION_FLAG         0x00000002
#define ICF_SET_SER_PRM_GLB_SUBS_FLAG           0x00000004
#define ICF_SET_SER_PRM_GLB_ACT_FLAG            0x00000008
#define ICF_SET_SER_PRM_CALL_HOLD_OPTION        0x00000010
#define ICF_SET_SER_PRM_MAX_NUM_OF_HOPS         0x00000020
#define ICF_SET_SER_PRM_MAX_CALL_WAITING        0x00000040
#define ICF_SET_SER_PRM_CFU_ADDR                0x00000080
#define ICF_SET_SER_PRM_CFB_ADDR                0x00000100
#define ICF_SET_SER_PRM_CFD_ADDR                0x00000200
#define ICF_SET_SER_PRM_CHR_TIMER               0x00000400
#define ICF_SET_SER_PRM_REINVITE_RACE_TIMER     0x00000800
#define ICF_SET_SER_PRM_TWC_TIMER               0x00001000
#define ICF_SET_SER_PRM_CTU_TIMER               0x00002000
#define ICF_SET_SER_PRM_CTA_TIMER               0x00004000
#define ICF_SET_SER_PRM_RNW_TIMER               0x00008000
#define ICF_SET_SER_PRM_CW_ALERT_TIMER          0x00010000
#define ICF_SET_SER_PRM_CND_ALERT_TIMER         0x00020000
#define ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER    0x00040000
#define ICF_SET_SER_PRM_SUBS_RESP_TIMER         0x00080000
#define ICF_SET_SER_PRM_SUBS_RETRY_TIMER        0x00100000
#define ICF_SET_SER_PRM_CONF_REQD_AT_TRANFEREE            0x00200000
#define ICF_SET_SER_PRM_CONF_CFM_REQD_AT_TRANSFEREE       0x00400000
#define ICF_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF     0x00800000
#define ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT            0x01000000

/* This bitmask should be set if Application wants to over-ride the default
 * behavior of terminating calls if conference request fails
 */
#define ICF_SET_SER_PRM_CONF_FAIL_CALL_CLEAR_NOT_REQD     0x02000000
   
      icf_uint32_t                  bit_mask;

     /*Bitmask specifies services subscribed by the user
      *Presence flag:ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG
      */
     
      /*This is a bitmask, which specifies which services are subscribed
       * by the user. Following are the values which can be filled in status
       *  field in icf_config_status_st  
       */

      
#define ICF_SERVICE_SUBS_CALL_HOLD_RESUME              0x0001
#define ICF_SERVICE_SUBS_THREE_WAY_CALL                0x0002
#define ICF_SERVICE_SUBS_CALL_TRF_ATTENDED             0x0004
#define ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED           0x0008
#define ICF_SERVICE_SUBS_INVITE_WITH_REPLACE           0x0010
#define ICF_SERVICE_SUBS_CALL_TRF_FROM_NW              0x0020
#define ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL        0x0040
#define ICF_SERVICE_SUBS_CALL_FWD_BUSY                 0x0080
#define ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER            0x0100
#define ICF_SERVICE_SUBS_CALL_WAITING                  0x0200
#define ICF_SERVICE_SUBS_EXTERNAL_CONFERENCE           0x0400
#define ICF_SERVICE_SUBS_JOIN_HEADER                   0x0800      
     icf_config_status_st      service_subscription_flag;
   

     /*Bitmask specifies services activated by the user
      *Presence flag:ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG
      */
     
     /* This is a bitmask,which specifies the service activated by user
      *  Following are the values which can be filled in status field
      *  in icf_config_status_st  */

#define ICF_SERVICE_ACT_CALL_HOLD_RESUME              0x0001
#define ICF_SERVICE_ACT_THREE_WAY_CALL                0x0002
#define ICF_SERVICE_ACT_CALL_TRF_ATTENDED             0x0004 
#define ICF_SERVICE_ACT_CALL_TRF_UNATTENDED           0x0008
#define ICF_SERVICE_ACT_INVITE_WITH_REPLACE           0x0010 
#define ICF_SERVICE_ACT_CALL_TRF_FROM_NW              0x0020
#define ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL        0x0040
#define ICF_SERVICE_ACT_CALL_FWD_BUSY                 0x0080
#define ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER     0x0100
#define ICF_SERVICE_ACT_CALL_WAITING                  0x0200
#define ICF_SERVICE_ACT_EXTERNAL_CONFERENCE           0x0400
#define ICF_SERVICE_ACT_JOIN_HEADER                   0x0800 
     icf_config_status_st      service_activation_flag;

     /* This flag decide which option should be used by CC to send call
      * hold indciation to remote party
      * presence_flag: ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION
      */

     /*icf_hold_option_t             call_hold_option;*/
     /* Per Line Feature - Modified the call hold option to structure */ 
     icf_hold_option_st             call_hold_option;
     
     /* Maximum number of hops for which a call can be forwarded.
      * This is optional field. 
      * presence_flag: ICF_SET_SER_PRM_MAX_NUM_OF_HOPS */

     icf_uint8_t		      max_num_of_call_forwarding_hops;

      /* Maximum number of calls in call waiting simultaneously.
       * After this threshold Call Forwarding with Busy Line
       * will be invoked.
       * This is optional field.
       * presence_flag: ICF_SET_SER_PRM_MAX_CALL_WAITING */

     icf_uint8_t                   max_num_of_calls_in_call_waiting;

     /* address to which the call will be forwarded
      * when call forwarding unconditional service is triggered.
      * This is optional field.
      * presence_flag: ICF_CFG_CC_SET_SER_PRM_CFU_ADDR */

     icf_config_address_st          addr_for_cfu;

     /* address to which the call will be forwarded
      * when call forwarding busy service is triggered.
      * This is optional field.
      * presence_flag: ICF_SET_SER_PRM_CFB_ADDR */

     icf_config_address_st          addr_for_cfb;

     /* address to which the call will be forwarded 
      * when call forwarding don't answer service is triggered.
      * This is optional field.
      * presence_flag: ICF_CFG_CC_SET_SER_PRM_CFD_ADDR*/

     icf_config_address_st          addr_for_cfd;

     /*Duration of call hold resume timer in milliseconds.
      *Presence flag: ICF_SET_SER_PRM_CHR_TIMER
      */

     icf_duration_t                 call_hold_resume_timer;

     /* Duration of reinvite race timer in milliseconds.
      * Presence flag: ICF_SET_SER_PRM_REINVITE_RACE_TIMER
      */

      icf_duration_t                reinvite_race_timer;

     /*Duration of twc timerin milliseconds.
      *Presence flag:ICF_CFG_CC_SET_SER_PRM_TWC_TIMER*/

      icf_duration_t                twc_timer;

     /* Duration of ctu timerin milliseconds. 
      * Presence flag: ICF_CFG_CC_SET_SERVICE_PARAMS_CTU_TIMER */

      icf_duration_t                ctu_timer;

      /*Duration of replace release timerin milliseconds.
       *Presence flag: ICF_SET_SERVICE_PARAMS_CTA_TIMER */

      icf_duration_t                cta_timer;

      /*Duration of rnw timerin milliseconds.
       *Presence flag: ICF_CFG_CC_SET_SER_PRM_RNW_TIMER */

      icf_duration_t                rnw_timer;

      /* Duration of cw alert timerin milliseconds.
       * Presence flag: ICF_SET_SER_PRM_CW_ALERT_TIMER
       */

      icf_duration_t                cw_alert_timer;

      /* Duration of cnd alert timerin milliseconds.
       * Presence flag:
       *  ICF_SET_SER_PRM_CND_ALERT_TIMER */

      icf_duration_t                cnd_alert_timer;

      /* Duration of cw and cnd alert timerin milliseconds.
       * Presence flag:
       *  ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER */

      icf_duration_t                cw_n_cnd_alert_timer;

      /* Duration of subscribe response timer.in milliseconds.
       * Presence flag: ICF_SET_SER_PRM_SUBS_RESP_TIMER */

      icf_duration_t                subs_resp_timer;

      /* Duration of subscribe retry timerin milliseconds..
       * Presence flag: ICF_SET_SER_PRM_SUBS_RETRY_TIMER */

      icf_duration_t                subs_retry_timer;
     /* flag to establish if media merging is required at transferee
        for creating a local conference */
      icf_boolean_t                 conf_reqd_at_transferee;

     /* flag to estabslish if confirmation is required at transferee
       for call transfer request*/
      icf_boolean_t                 conf_cfm_reqd_at_transferee;

     /*Application can configure URI of conference server, this is per line*/
     icf_config_address_st        conference_uri;

    /* subscription duration for external conference case */
     icf_duration_t                 xconf_subs_duration;

    /* Set this parameter Application does not want calls to be cleared
     * in case conference request fails due to merge media failure or
     * twc timer expiry.
     */  
      icf_boolean_t                 conf_fail_call_clear_not_reqd;

} icf_set_service_params_req_st;


/*-----------------------------------------------------------------------------
 *
 * NAME : icf_display_ind_st
 *
 * DESCRIPTION  :  This data type is used to define a structure,which specifies
 *                 payload data of API ICF_DISPLAY_IND. 
 *                 
-----------------------------------------------------------------------------*/

typedef struct
{

    
    /* This parameter identifies the display information.*/
    icf_display_id_t     display_id;    

    /* This field bitwise contains the presence information of optional fields 
     * in this structure.
     */

#define ICF_DISPLAY_IND_DISPLAY_DATA               0x01
#define ICF_DISPLAY_IND_TONE                       0x02

    icf_uint8_t                bit_mask;
    
     /* This parameter contains the integer and string display data. It can
      * store upto 4 integers and 2 strings.This is optional field.
      * presence_flag: ICF_DISPLAY_IND_DISPLAY_DATA (0x01)
      */
    icf_display_data_st        display_data;       
    
     /* This parameter identifies the tone to be  played to  the phone user.
      * This is optional field.
      * presence_flag: ICF_DISPLAY_IND_TONE (0x02)
      */
    icf_tone_id_t              tone_id;   
    
} icf_display_ind_st;

/*------------------------------------------------------------------------------
*
*NAME : icf_call_redirection_ind_st
*
*DESCRIPTION : This data type is used to define a structure,
*               which specifies payload data of 
*               API ICF_CALL_REDIRECTION_IND.
*
------------------------------------------------------------------------------*/

typedef struct
{

      /*This bitwise field contains presence information*/
     
#define ICF_CALL_RDR_IND_PTY_ADDR  0x01 
#define ICF_CALL_RDR_IND_PTY_NAME  0x02 

      icf_uint32_t  bit_mask ;

      /*Address to which call is redirected*/
      icf_address_st  redirected_party_addr;

      /*Name of the party to which call is redirected*/
      icf_name_st  redirected_party_name;

      icf_header_list_st	header_list;

} icf_call_redirection_ind_st;
/***********************************
*    MANAGEMENT PLANE HANDLER APIs *
************************************/


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_set_mgmt_plane_auth_param_req_st
 *
 * Description: This API is used to configure the management plane authentication 
 *				parameters. The application is required to inform ICF about the 
 *				values of certain parameters to be used in processing of a 
 *				challenge received from a XDM or SyncMl server in a HTTP response. 
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{

/* bitmask for indicating valid/invalid values for optional parameters */

#define ICF_MGMT_PLANE_AUTH_DATA_PRESENT 0x01

	icf_uint16_t		bitmask;

/* if true, indicates that the configuration for this server name is to be deleted, 
else this should always be set to FALSE */
	
	icf_boolean_t        remove_flag;

/* indicates whether this configuration is intended for XCAP / SyncMl based operations */

/*
#define ICF_XCAP_USAGE	0x01
#define ICF_SYNCML_USAGE 0x02 */

	icf_uint8_t		server_type;

/* address of management server */

	icf_transport_address_st	server_addr;

/* authentication algorithm is to be used - for future use */

	icf_uint8_t		auth_method;

/* user name to be used in case the server challenges a request sent by ICF */
	
	icf_string_st		auth_username;

/* password to be used in case the server challenges a request sent by ICF */
	
	icf_string_st		auth_password;

} icf_set_mgmt_plane_auth_param_req_st; 



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_create_secure_conn_req_st
 *
 * Description: This API is used by Application to request ICF to set up a 
 *				secure TCP connection to peer.  This connection can be used 
 *				either for XCAP or for SyncML messages. 
 *
 *---------------------------------------------------------------------------*/

typedef struct
{

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */
	
	icf_uint16_t	exchange_id;
	
/* Indicates the type of usage intended of the connection. Currently only 
	connections to SyncMl and XCAP servers are supported */		

#define ICF_XCAP_USAGE	0x01
#define ICF_SYNCML_USAGE 0x02

	icf_uint8_t		server_type;
	
/* Address of the server with which a secure connection is desired */

	icf_transport_address_st	 server_addr;

} icf_create_secure_conn_req_st;
				
				

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_create_secure_conn_resp_st
 *
 * Description: This API is used by ICF to send a secure connection set-up 
 *				response.
 *
 *---------------------------------------------------------------------------*/
			
typedef struct 
{

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* result to convey whether the connection set-up was successful or failure */
 	
	icf_result_t	conn_req_status;

/* A 32-bit handle to the connection context created by ICF. This field is 
	checked when conn_req_status is success*/
	
	icf_uint32_t	conn_handle;

/* defines the error cause when conn_req_status is failure */	
	icf_uint32_t	error_cause;

} icf_create_secure_conn_resp_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_close_secure_conn_req_st
 *
 * Description: This API is used by Application to close an established secure 
 *				connection.
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

} icf_close_secure_conn_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_close_secure_conn_resp_st
 *
 * Description: This API is used by ICF to update the application about the 
 *				status of a previously issued Close Connection Request.
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{
	
/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* result to convey whether the connection close was successful or failure */	

	icf_result_t	close_conn_status;

/* A 32-bit handle to the connection for which a close was requested */	

	icf_uint32_t	conn_handle;

/* defines the error cause when close_conn_status is failure */	
	
	icf_uint32_t	error_cause;

} icf_close_secure_conn_resp_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_secure_conn_status_ind_st
 *
 * Description: This API is used by ICF to update the application about the 
 *				status of an established connection. This is triggered due 
 *				to change in the status of the connection (for e.g closure of 
 *				connection by peer).
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{

/* status field to convey the status of the connection */

#define ICF_SECURE_CONN_CLOSED_BY_PEER		        0x01
#define ICF_SECURE_CONN_TIME_OUT			        0x02
#define	ICF_REATTEMPTING_SECURE_CONNECTION          0x03
#define	ICF_REATTEMPTED_SECURE_CONNECTION_SUCCESS	0x04
#define	ICF_REATTEMPTED_SECURE_CONNECTION_FAILURE	0x05
#define ICF_CONN_STATUS_GPRS_SUSPENDED              0x08

	icf_uint8_t		conn_status;

/* A 32-bit handle to the connection context created by ICF */
	
	icf_uint32_t	conn_handle;

/* A 32-bit handle to the connection context created by ICF */
	icf_uint32_t	old_conn_handle;

} icf_secure_conn_status_ind_st;



/*----------------------------------------------------------------------------
 *
 *  icf_xcap_res_et - This data type is used to define value of type 
 *					  icf_uint8_t that indicates the resource type of 
 *					  MPH request.
 *
 *--------------------------------------------------------------------------*/

typedef enum{

ICF_INVALID_RES,
ICF_XCAP_RES_DOC,
ICF_XCAP_RES_ELEM,
ICF_XCAP_RES_ATTR

}icf_xcap_res_et;



/*----------------------------------------------------------------------------
 *
 *  icf_stretchable_hdr_st - This data type is used to define a stretchable 
 *							 header for which length is indicated.
 *
 *--------------------------------------------------------------------------*/

typedef struct
{
icf_uint8_t   hdr_len;
icf_uint8_t  *p_hdr;         
} icf_stretchable_hdr_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_create_replace_res_req_st
 *
 * Description: This API is used by ICF to update the application about the 
 *				status of a previously issued Close Connection Request.
 *				Indicates whether the request pertains to an operation on a
 *				document, element within a document or attribute of an element 
 *				within a document 
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{

/* bitmask for indicating valid/invalid values for optional parameters */

#define ICF_NO_RESP_DURATION_PRESENT	0x01
#define ICF_ACCEPT_PRESENT				0x02
#define ICF_IF_MATCH_PRESENT			0x04
#define ICF_IF_NONE_MATCH_PRESENT		0x08
#define ICF_ACCEPT_ENCODING_PRESENT		0x10
#define ICF_CONTENT_ENCODING_PRESENT	0x20
#define ICF_HIGH_PRIORITY_REQ			0x40

	icf_uint8_t		bitmask;

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

/* escape-encoded requested URI to be sent in outgoing HTTP request */

	icf_req_uri_st	request_uri;

/* origin server/host server name to be sent in the outgoing HTTP request */

	icf_transport_address_st	host_name;

/* whether the request pertains to an operation on a document, element 
	within a document or attribute of an element within a document */

	icf_xcap_res_et	res_type;

/* value for content type header in outgoing HTTP request */

	icf_string_st	content_type;

/* duration of time for which ICF will wait for response on connection */

	icf_duration_t	timer_duration;

/* value for Accept header in outgoing HTTP request. This field will be ignored if res_type is ICF_XCAP_RES_DOC */

	icf_stretchable_hdr_st	accept;

/* value for if_match header in outgoing HTTP request.  */

	icf_stretchable_hdr_st	if_match;

/* value for if_none_match header in outgoing HTTP request.  */

	icf_stretchable_hdr_st	if_none_match;

/* value for accept_encoding header in outgoing HTTP request. */

	icf_stretchable_hdr_st	accept_encoding;

/* value for content_encoding header in outgoing HTTP request */

	icf_stretchable_hdr_st	content_encoding;

/* length of the payload  */

	icf_uint32_t		payload_length;

/* stretchable array for the XML payload. This payload will be 
	transparently attached to the outgoing HTTP message as message body. */

	icf_uint8_t 	*payload;

} icf_create_replace_res_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_delete_res_req_st
 *
 * Description: This API is used by the application to delete an XCAP resource 
 *				on the document server. 
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{
/* bitmask for indicating valid/invalid values for optional parameters */
/* 
ICF_IF_MATCH_PRESENT 
ICF_IF_NONE_MATCH_PRESENT
ICF_NO_RESP_DURATION_PRESENT */

	icf_uint8_t		bitmask;

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

/* escape-encoded requested URI to be sent in outgoing HTTP request */

	icf_req_uri_st	request_uri;

/* origin server/host server name to be sent in the outgoing HTTP request */

	icf_transport_address_st	host_name;

/* whether the request pertains to an operation on a document, element 
	within a document or attribute of an element within a document */

	icf_xcap_res_et	res_type;

/* duration of time for which ICF will wait for response on connection */

	icf_duration_t	timer_duration;

/* value for Accept header in outgoing HTTP request. This field will be ignored if res_type is ICF_XCAP_RES_DOC */

	icf_stretchable_hdr_st	accept;

/* value for if_match header in outgoing HTTP request.  */

	icf_stretchable_hdr_st	if_match;

/* value for if_none_match header in outgoing HTTP request.  */

	icf_stretchable_hdr_st	if_none_match;

} icf_delete_res_req_st;
	


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_fetch_res_req_st
 *
 * Description: This API is used by the application to fetch an XCAP resource 
 *				on the document server. 
 *
 *---------------------------------------------------------------------------*/
	
typedef struct 
{
/* bitmask for indicating valid/invalid values for optional parameters */
/* 
ICF_IF_MATCH_PRESENT 
ICF_IF_NONE_MATCH_PRESENT
ICF_NO_RESP_DURATION_PRESENT */

	icf_uint8_t		bitmask;

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

/* escape-encoded requested URI to be sent in outgoing HTTP request */

	icf_req_uri_st	request_uri;

/* origin server/host server name to be sent in the outgoing HTTP request */

	icf_transport_address_st	host_name;

/* whether the request pertains to an operation on a document, element 
	within a document or attribute of an element within a document */

	icf_xcap_res_et	res_type;


/* duration of time for which ICF will wait for response on connection */

	icf_duration_t	timer_duration;


/* value for if_match header in outgoing HTTP request */

	icf_stretchable_hdr_st	if_match;

/* value for if_none_match header in outgoing HTTP request */

	icf_stretchable_hdr_st	if_none_match;

} icf_fetch_res_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_syncml_send_req_st
 *
 * Description: This API is used by application to trigger ICF to send a HTTP 
 *				POST request with application supplied SyncMl message body 
 *				to Device Management server. 
 *
 *---------------------------------------------------------------------------*/

typedef struct 
{

/* bitmask for indicating valid/invalid values for optional parameters */

#define ICF_HEADER_LIST_PRESENT 0x80

	icf_uint8_t		bitmask;

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

/* content type field for the outgoing message */

	icf_string_st	content_type;

/* value of cache-control header in outgoing HTTP request */

	icf_string_st	cache_control;

/* value of transfer_encoding header in outgoing HTTP request */

	icf_string_st	transfer_encoding;

/* value of user_agent header in outgoing HTTP request */

	icf_string_st	user_agent;

/* value of accept header in outgoing HTTP request */

	icf_string_st	accept;

/* value of accept_char header in outgoing HTTP request */

	icf_string_st	accept_char;

/* duration of time for which ICF will wait for response on connection */

	icf_duration_t	timer_duration;

/* list of additional headers that the application wants to send in the outgoing request */

	icf_header_list_st  header_list;

/* length of the payload  */

	icf_uint32_t		payload_length;

/* stretchable array for the XML payload. This payload will be 
	transparently attached to the outgoing HTTP message as message body. */

	icf_uint8_t 	*payload;

} icf_syncml_send_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_api_proc_failure_st
 *
 * Description: This API is used by ICF to inform the application about 
 *				the failure of processing of an earlier XCAP/SyncML 
 *				APIs. The exchange-id passed by the application is returned 
 *				so as to help application correlate the request and this API.  
 *
 *---------------------------------------------------------------------------*/
	
typedef struct 
{

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* A 32-bit handle to the connection context created by ICF */

	icf_uint32_t	conn_handle;

/* defines the api_id of the request that failed */

	icf_api_id_t	api_id;

/* defines the error cause */	

	icf_error_t	error_cause;

} icf_api_proc_failure_st;

	

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_mgmt_plane_nw_resp_st
 *
 * Description: This API is used by ICF to convey a received response from 
 *				server to application.  
 *
 *---------------------------------------------------------------------------*/
	
typedef struct
{

/* bitmask for indicating valid/invalid values for optional parameters */

#define ICF_ENTITY_TAG_PRESENT		            0x01
#define ICF_CONTENT_TYPE_PRESENT	            0x02
#define ICF_CONNECTION_STATUS_PRESENT           0x04

	icf_uint8_t       bitmask;

/* API ID given by application for which the HTTP request was dispatched */
	
	icf_api_id_t      req_api;

/* Unique exchange ID that will be returned by ICF to application when it 
	sends back the result of the API execution to Application */

	icf_uint16_t	exchange_id;

/* reference to the connection (connection handle) on which the 
	message was received */

	icf_uint32_t      conn_handle;

/* entity tag in the received message */
	
	icf_stretchable_hdr_st  entity_tag;

/* content type field in the received message */

	icf_string_st  content_type;

/* Below Values of connection header will be present */
#define ICF_CONN_STATUS_CLOSE_WITH_REATTEMPT       0x01
#define ICF_CONN_STATUS_CLOSE_WITH_NO_REATTEMPT    0x02

/* Specifies the value of connection header if present in the payload */

    icf_uint8_t       connection_status;

/* list of additional headers as per Syncml/XCAP HTTP binding */

	icf_header_list_st  header_list;

/* enumeration value for the response code in the received HTTP response */		

	icf_uint32_t   response_code;
		
/* length of the payload  */

	icf_uint32_t		payload_length;

/* received payload */

	icf_uint8_t		*payload;

} icf_mgmt_plane_nw_resp_st;


/*ICF_HANDLE_REINVITE_START*/


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_incoming_call_modify_ind_st
 *
 * Description: This API is used by ICF to send the Application an indication 
 * 				for the Media Change Request
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
		/* ICF REL 6.0: multiple m lines */
#define ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT		0x01
#define ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT		0x02
	icf_uint32_t 			bit_mask ;
		
    /* If this flag is TRUE then application needs to send confirmation for the
     call modify indication. If FALSE, no need to send any confirmation and if
     sent, it will be ignored at ICF */
    icf_boolean_t           confirm_required;

    /* List of additional headers that are of importance to the application. */
	icf_header_list_st 		header_list;
		
    /* List of additional message bodies that are of importance to the 
	 * application. */
	icf_msg_body_list_st 	body_list ;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_call_modify_media_data_st */
	icf_list_st				*p_active_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_stream_list[ICF_MAX_STREAM_PER_CALL];

    
} icf_incoming_call_modify_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_call_modify_cfm_st
 *
 * Description: This API is sent by application as the response to the media change request.  
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t			bit_mask;
#define ICF_CALL_MODIFY_RESPONSE_CODE_PRESENT      0x01
#define ICF_CALL_MODIFY_CFM_REJECTED_STREAM_PRESENT  0x02
#define ICF_CALL_MODIFY_RESPONSE_SIGNALING_MODE_PRESENT  0x04

	/* The Accept / Reject response including the SDP*/
	icf_result_t			result;

	/* If reject respose, cause of rejection */
	icf_error_t				cause;

      /* This is the nw_response_code application wants IPTK to send */
        icf_uint16_t            nw_response_code;
		
	/* List of additional headers that are of importance to the application. */
	icf_header_list_st		header_list;

	/* List of additional tags that the application wants to send in the API. */
	icf_tag_list_st			tag_list;

	/* List of additional message bodies that are of importance to the 
	 * application. */
	icf_msg_body_list_st	body_list ;

	/* this is a list of the type: icf_stream_capab_st. This field will be used
     * only to pick the stream level encoded attributes for a stream. Any other
     * field in this structure will be ignored within ICF */
	icf_list_st              *p_stream_list;

	/* this is a list of rejected/deleted streams. The information given by 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t	        count;
	icf_stream_list_st      deleted_stream_list[ICF_MAX_STREAM_PER_CALL];

      /* In this field, application can specify the mode to be sent to remove 
       * in 200 ok of REINVITE. Note that this will only be the signaling mode. 
       * IPTK will not change mediamode based on this mode. IPTK will change 
       * the local media mode based on the mode received in answer SDP in ACK */
       icf_media_mode_t	         signaling_mode;
       
} icf_call_modify_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_call_modify_req_st
 *
 * Description	 : This API is sent by application as the media change 
 * 				   request.  
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
  	icf_uint32_t			bit_mask;
	/* ICF REL 6.0: multiple m lines */
#define ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT     0x01
#define ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT		0x02

	/* List of additional headers that are of importance to the application. */
	icf_header_list_st		header_list;

	/* List of additional tags that the application wants to send in the API. */
	icf_tag_list_st			tag_list;

	/* List of additional message bodies that are of importance to the 
	 * application. */
	icf_msg_body_list_st	body_list ;
	
	/* ICF REL 6.0: multiple m lines */
	/* the information here will be list of streams that the application
	 * wishes to modify (only) during mid-call. the application can only 
	 * modify the codecs or the mode for a stream. Optionally it can also 
	 * modify the encoded attributes for the application stream. 
	 * This is a list of the type icf_call_modify_media_data_st. This 
	 * is an optional field in this structure. the application may decide not 
	 * to change the media infomration for the established call. 
	 */
	icf_list_st				*p_media_modify_list;
	
	/* the application may delete any stream mid-call. This is 
	 * optional. */
	icf_stream_id_t			count;
	icf_stream_list_st	    deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
} icf_call_modify_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_call_modify_resp_st
 *
 * Description   : This API is sent to application as the cfm to media change 
 * 				   request.  
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
#define  ICF_RESPONSE_CODE_PRESENT					0x01
	/* ICF REL 6.0: multiple m lines */
#define  ICF_CALL_MOD_ACCEPT_STREAM_LIST_PRESENT	0x02
#define  ICF_CALL_MOD_RESP_REJECT_STREAM_PRESENT	0x04
	icf_uint32_t			bit_mask;
	
	/* The Accept / Reject response */
	icf_result_t			result;

	/* If reject respose, cause of rejection */
	icf_error_t			error_cause ;

	/* The response code of the call modify request*/
	icf_uint32_t		response_code;

	/* List of additional headers that are of importance to the application. */
	icf_header_list_st		header_list;

	/* List of additional message bodies that are of importance to the 
	 * application. */
	icf_msg_body_list_st		body_list ;

	/* ICF REL 6.0: multiple m lines */
	/* information of the accepted/commited media streams for the received 
	 * INVITE. This is a list of the node type icf_call_modify_media_data_st */
	icf_list_st				*p_active_stream_list;

	/* this is a list of rejected/deleted streams. The information shared with 
	 * APP here is the stream id and the corresponding stream_type. This is 
	 * an optional field */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_stream_list[ICF_MAX_STREAM_PER_CALL];

        /* duration received in retry-after header*/
        icf_duration_t    retry_after_duration;
} icf_call_modify_resp_st;

/************************************************************************
 *
 * DESCRIPTION:  This function would initialize the entire ICF
 *                
 *
 ************************************************************************/

extern icf_return_t 
icf_init(
        	icf_init_param_st	*p_init_param,
			icf_void_t			**p_p_icf_glb_data);


/*ICF_HANDLE_REINVITE_END*/

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_res_unavail_ind_st
 *
 * Description: This API is sent by ICF to the aplication when ICF is being
 * 				depleted of resources and it is not in a position to handle new
 * 				requests. The retry_after parameter in the payload is an 
 * 				indication for the application to retry after some time.
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_api_id_t			req_api_id;
	icf_result_t			result;
	icf_error_t				cause;
	
	icf_uint8_t				retry_after;
} icf_res_unavail_ind_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_call_hold_req_st
 *
 * Description: This is payload of call hold request from application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t		bit_mask;
    icf_header_list_st	    header_list;
    icf_tag_list_st		    tag_list;
    icf_msg_body_list_st	body_list;
    icf_stream_id_list_st	    stream_list;

} icf_call_hold_req_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_call_resume_req_st
 *
 * Description: This is payload of call resume request from application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t		bit_mask;
    icf_header_list_st	    header_list;
    icf_tag_list_st		    tag_list;
    icf_msg_body_list_st	body_list;
    icf_stream_id_list_st	    stream_list;

} icf_call_resume_req_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_api_resp_st
 *
 * Description: This is payload to send common responses to application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t		    bit_mask;
    icf_result_t		    result;
    icf_error_t		        error_cause;
    icf_header_list_st	    header_list;
    icf_msg_body_list_st	body_list;
    icf_stream_id_list_st	    stream_list;

    /* duration received in retry-after header*/
    icf_duration_t    retry_after_duration;
} icf_api_resp_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_ctu_resp_st
 *
 * Description: This is payload to send CTU responses to application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    icf_uint32_t		bit_mask;
    icf_result_t		result;
    icf_error_t			error_cause;
    icf_header_list_st	header_list;
   
    /* duration received in retry-after header*/
    icf_duration_t    retry_after_duration; 

} icf_ctu_resp_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cta_resp_st
 *
 * Description: This is payload to send CTA responses to application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    icf_uint32_t		bit_mask;
    icf_result_t		result;
    icf_error_t			error_cause;
    icf_header_list_st	header_list;
   
    /* duration received in retry-after header*/
     icf_duration_t    retry_after_duration;
 
} icf_cta_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_api_ind_st
 *
 * Description: This is payload to send remote hold indication and resume 
 *              indication
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t		    bit_mask;
    icf_header_list_st	    header_list;
    icf_msg_body_list_st	body_list;
    icf_stream_id_list_st	    stream_list;
} icf_api_ind_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_party_replaced_ind_st
 *
 * Description: This is payload to send indication that remote party of the call
 *              has been replaced by the new party
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    icf_uint32_t		bit_mask ;
    
#define ICF_REPLACING_PARTY_ADDR_PRESENT    0x00000001
#define ICF_PARTY_REPLACED_NAME		        0x00000002
    
    icf_call_id_t		replacing_call_id;
    icf_address_st		replacing_party_addr;
    icf_name_st			replacing_party_name;
    icf_header_list_st	header_list;
    icf_msg_body_list_st    body_list;
    icf_stream_id_list_st	stream_list;
} icf_party_replaced_ind_st;


#ifdef ICF_FAX_SUPPORT
/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_start_fax_req_st
 *
 * Description: This is payload to start fax request from Application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
#define ICF_START_FAX_REQ_METHOD_PRESENT 0x01
#define ICF_START_FAX_REQ_PT_PARAM_PRESENT  0x02
   
      icf_uint8_t bitmask;
      icf_uint8_t         pt_param;
#define ICF_START_FAX_PT_PARAM_SILENCE_SUPP    0x01
#define ICF_START_FAX_PT_PARAM_VBD_GPMD        0x02  
#define ICF_START_FAX_PT_PARAM_ECHO_CANC       0x04

      /* Method to be used to send fax to remote party*/
      icf_fax_start_type_t  fax_type;
	/*Preffered codec for fax pass through */
      icf_fax_pt_codec_t  fax_pt_codec;

      icf_uint8_t fax_method;
}icf_start_fax_req_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_start_fax_resp_st
 *
 * Description: This is payload to send response to fax start req to Application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* bitmask added for retyr after handling */
    icf_uint32_t                 bit_mask;
 
    icf_result_t	     	result;

	/* Error cause is set if result is failure*/
	icf_error_t			   error_cause;

	/* This field will indicate if fax call is running
	 * using T38 or G711PT method */
	icf_fax_start_type_t   fax_type;
   
    /* duration received in retry-after header*/
    icf_duration_t    retry_after_duration;
}icf_start_fax_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_start_fax_ind_st
 *
 * Description: This is payload to send fax start indication to Application
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
	/* This field will indicate if fax call is running
	 * using T38 or G711PT method 
	 * NOTE: In this release fax_type will be only T38
	 */
	icf_fax_start_type_t   fax_type;

}icf_start_fax_ind_st;
#endif




/*NOTIFY_OUT_OF_DIALOG_START*/
/*-----------------------------------------------------------------------------
 *
 * Structure Name:	icf_notify_cfm_st
 *
 * Description: This API is used by the application to inform the framework.
 *                about the result of an the notify request received  by 
 *                the application.
 *---------------------------------------------------------------------------*/

typedef struct
{
#define ICF_NOTIFY_ERROR_CAUSE_PRESENT		0x10000
#define ICF_NOTIFY_RESP_CODE_PRESENT		0x20000

	icf_uint32_t bit_mask;

    /* Indicates the result of the notify (success/failure). 
		success : 1 and failure : 0 */
    icf_result_t result;

   	icf_error_t error_code ;
	    
	/* Response code to be sent for the NOTIFY request. */
    icf_uint32_t response_code ;


} icf_notify_cfm_st;

/*NOTIFY_OUT_OF_DIALOG_END*/

/*HANDLE_INFO_IND_START*/
/*-----------------------------------------------------------------------------
*
* NAME :		 icf_info_ind_st
*
* DESCRIPTION  : This data type is used to define a structure,
*                which specifies payload data of API
*                ICF_INFO_IND.
-----------------------------------------------------------------------------*/

typedef struct
{
    icf_uint32_t		bit_mask;
                
    icf_header_list_st	    header_list;
    icf_msg_body_list_st	body_list;

} icf_info_ind_st;

/*HANDLE_INFO_IND_END*/

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_info_resp_st
 *
 * Description: This is payload of API (ICF_INFO_RESP)  to send 
 *				INFO responses to application
 *              
 *
 *---------------------------------------------------------------------------*/
typedef struct 
{
    icf_uint32_t			bit_mask;
    icf_result_t			result;
    icf_error_t				error_cause;
	icf_uint32_t			response_code;
    icf_header_list_st		header_list;
	icf_msg_body_list_st    body_list;
    
} icf_info_resp_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_call_forwarded_ind_st
 *
 * Description: This is payload of API (ICF_CALL_FORWARDED_IND)  to send 
 *				call forwarded indication to application that an incoming call
 *              has been forwarded.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This is the address of remote party of the call which is forwarded*/
    icf_address_st remote_party_addr;   
    
    /* This field bitwise contains the presence information of optional fields 
     * in this structure.*/

#define ICF_REMOTE_PARTY_NAME    0x01

    icf_uint32_t bit_mask;
    
    icf_name_st                 remote_party_name;

#define ICF_CALL_FORWARD_TYPE_UNCOND        1
#define ICF_CALL_FORWARD_TYPE_NO_ANSWER     2
#define ICF_CALL_FORWARD_TYPE_BUSY_LINE     3
    icf_uint8_t                 call_forwarded_type;
} icf_call_forwarded_ind_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_options_req_st
 *
 * Description: This API is used by the application to send an OPTIONS request 
 *				to query the capabilities of peer.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Use common hash defines if header/msg-body/tag list present */
	icf_uint32_t bit_mask;

    /* This bitmask is set when Application mentions the list of headers
     * which need not to be send on network in OPTIONS request.
     */ 
#define  ICF_OPTIONS_REQ_REMOVE_HDR_LIST_PRESENT 0x1

	/*Line-id that has been used for outgoing request*/
	icf_line_id_t line_id;

    /* Indicates the SIP URL of the remote user to which OPTIONS request has
     * to be sent.
     */
    icf_address_st destination_info;

    /* Indicates the Preferred Identity, which shall be used at the P-CSCF for
     * asserted identity in trusted networks.
     */
    icf_address_st pref_identity;

    /* List of additional headers that the application wants to send in the
     * OPTIONS request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * OPTIONS request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * OPTIONS request.
     */
    icf_msg_body_list_st body_list;

    /* This field is a list of non-madatory headers that the application wants 
     * to remove form out of dialog OPTIONS sip message before sending on
     * network.
     */
    icf_header_name_list_st remove_hdr_list;
} icf_options_req_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_options_resp_st
 *
 * Description: This API is used by ICF to inform the application about the
 *              result of an earlier OPTIONS request issued by the
 *              application.
 *---------------------------------------------------------------------------*/
typedef struct 
{
	icf_uint32_t			bit_mask;
#define  ICF_OPTIONS_RESP_CODE_PRESENT		             0x01
#define  ICF_OPTIONS_ERROR_CODE_PRESENT					 0x02
#define  ICF_OPTIONS_REMOTE_STREAM_LIST_PRESENT	         0x04

	/* The Accept / Reject response */
	icf_result_t			result;

	/* If reject respose, cause of rejection */
	icf_error_t				error_code_for_options;

	/* The response code of the OPTIONS request*/
	icf_uint32_t			response_code_for_options;

	/* List of additional headers that are of importance to the application. */
	icf_header_list_st		header_list;
	/* List of additional message bodies that are of importance to the 
	 * application. */
	icf_msg_body_list_st	body_list;

	/*This is a list of the node type icf_stream_capab_st */
	/*This contains the media capabilities of the peer as received in response 
	 *to OPTIONS request
	 */
	icf_list_st				*p_remote_stream_list;

} icf_options_resp_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_inc_options_ind_st
 *
 * Description: This API is used by ICF to inform the application about the 
 *		incoming OPTIONS request.Moreover, it tells about the currently 
 *		supported capabilities of that application by Media Manager.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Use common hash defines if header/msg-body/tag list present */
	icf_uint32_t 		bit_mask;
#define ICF_INC_OPTIONS_IND_MEDIA_PRESENT		0x01

    /* List of additional headers that the application currently supports in its profile
     */
    icf_header_list_st 	header_list;

    /* List of additional tags that the application currently supports in its profile
     */
    icf_tag_list_st 	tag_list;

    /* List of message bodies that the application currently supports in its profile
    */
    icf_msg_body_list_st body_list;

	/* 
	 * This is a list of the node type icf_stream_capab_st */
	/* This structure contains capabilities present in the application profile.
	 * This contains the capabilities that are returned by the Media Manager for 
	 * that application.
	 */
	icf_list_st			*p_app_sdp_list;
} icf_inc_options_ind_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_options_cfm_st
 *
 * Description: This API is used by the application to confirm incoming OPTIONS 
 * 		request indication. This will tell the final response to be sent
 * 		on the network for incoming OPTIONS.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Use common hash defines if header/msg-body/tag list present */
	icf_uint32_t 			bit_mask;
#define ICF_OPTIONS_CFM_MEDIA_PRESENT		0x01
#define ICF_OPTIONS_CFM_DEL_MEDIA_PRESENT	0x02

	/* the application can decide to accept or reject a call following the
	 * OPTIONS request that has been received. If the application returns
	 * failure in the CFM, ICF will respond to the received OPTIONS with a 
	 * 486 */
	icf_result_t			result;

	/* List of additional headers that the application wants to send in the
     * OPTIONS request
     */
    icf_header_list_st 		header_list;

    /* List of additional tags that the application wants to send in the 
     * OPTIONS request.
     */
    icf_tag_list_st 		tag_list;

    /* List of message bodies that the application wants to send in the
     * OPTIONS request.
     */
    icf_msg_body_list_st 	body_list;

	/* This is a list of the node type icf_stream_capab_st */ 
	/* This structure contains capabilities modified by the application
	 * If Incoming OPTIONS contains Accept Header with value as application/sdp, 
	 * then this is set and contains SDP that is to be sent in 200ok.
	 * Application can modify the attributes of the streams and can add only 
	 * encoded attributes of the stream. However, it can add only application 
	 * based streams. 
     * The application can add a media stream to this list only if the media stream
     * is already configured by the application in the media profile in app_conf
     * during ICF_APP_ADD 
	 */
     icf_list_st			*p_app_sdp_list;

	/* the application can reject a complete media stream it it desires */
	icf_stream_id_t			count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
} icf_options_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_set_line_param_req_st
 *
 * Description: This API is used by the application to set  
 * sip timers duration (t1, t2,B and F), registration head start 
 * duration and subscription head start duration. on a line. 
 *-----------------------------------------------------------------------------*/


typedef struct 
{
	/* Mandatory Parameters */

	icf_line_id_t			line_id;
	icf_uint16_t			bitmask;

	/* This Bit mask will be set when sending subscribe
 	* head start duration */

#define ICF_SET_LINE_PRM_SUBS_HS_TIMER	    	 0x01

   	/* This Bit Mastk will be set when sending t1 timer 
 	* duration */

#define ICF_SET_LINE_PRM_T1_TIMER				0x02

	/* This Bit Mastk will be set when sending t2 timer 
	 * duration */

#define ICF_SET_LINE_PRM_T2_TIMER		     	0x04

       /* This Bit Mastk will be set when sending B timer 
 	* duration */

#define ICF_SET_LINE_PRM_INV_TIMER		     	0x08

       /* This Bit Mastk will be set when sending F timer 
 	* duration */

#define ICF_SET_LINE_PRM_NON_INV_TIMER	     	0x10

/*Start : Per Line Feature */
#define ICF_SET_LINE_SIP_DSCP                   0x20

#define ICF_SET_LINE_RTP_DSCP                   0x40

#ifdef ICF_SESSION_TIMER
#define ICF_SET_LINE_MIN_SE                     0x80
#define ICF_SET_LINE_SESSION_TIMER              0x0100
#define ICF_SET_LINE_SESSION_TIMER_REFRESHER    0x0200
#endif
#define ICF_SET_LINE_AUDIO_CODEC 				0x0400
#define ICF_SET_LINE_VIDEO_CODEC 				0x0800
#define ICF_SET_LINE_CONTACT_USER_INFO	    	0x1000
/* End: Per Line Feature */

/* This bitmask will be set when application configures remove_route_hdr */
#define ICF_SET_LINE_REMOVE_ROUTE_HDR           0x2000

	/* Optional Parameters */
	/* Number of seconds prior to expriation of 
 	* subscription */

	icf_duration_t			subscribe_head_start_timer;

	/* SIP T1 Timer duration value in milliseconds*/

	icf_duration_t			sip_timer_t1 ;

	/* SIP T2 Timer duration value in milliseconds*/

	icf_duration_t			sip_timer_t2 ;

	/* SIP B Timer duration value in milliseconds*/

	icf_duration_t			sip_timer_invite ;

	/* SIP F Timer duration value in milliseconds*/

	icf_duration_t			sip_timer_noninvite ;

    /*Start : Per Line Feature */
    /* The Differentiated Services Codepoint (DSCP) value that should populate the 
    DS field of SIP packets sent by the User Agent.*/
    icf_uint8_t             sip_dscp;
   
    /* The Differentiated Services Codepoint (DSCP) value that should populate the 
    DS field of RTP packets sent by the User Agent.*/
    icf_uint8_t             rtp_dscp;

    #ifdef ICF_SESSION_TIMER
    /* The minimum acceptable value, in seconds, 
    for the RFC session timer interval. */
    icf_duration_t          min_se;

    /* The maximum acceptable value, in seconds, 
    for the RFC session timer interval. */
    icf_duration_t          session_timer;

    /* A value of 'none,' 'uas,' or 'uac' specifying the refresher to be specified 
    by the UA when it has a choice. A value of 'none' specifies that the UA SHALL NOT 
    request session expiration; any other value specifies that the UA SHALL request 
    session expiration. The default value SHALL be 'uas'*/
    icf_refresher_type_t    session_timer_refresher; 
    #endif
    icf_list_st     *p_codec_attributes_audio;
    icf_list_st     *p_codec_attributes_video;
	icf_string_st	contact_user_info;
    /* End : Per Line Feature */

	/* This field indicates whether the Route header from outgoing requests 
	 * will be stripped or not
	 */
    icf_boolean_t   remove_route_hdr;

} icf_set_line_param_req_st;


/*----------------------------------------------------------------------------
 * NAME : icf_logs_ind_st
 *
 * Description: This structure is used to specify payload data of
 *               API ICF_LOGS_IND
 *
 *---------------------------------------------------------------------------*/
typedef struct {
    icf_long_string_st  	trace_string;
}icf_logs_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:	icf_info_cfm_st
 *
 * Description: This API is used by the application to inform the framework.
 *                about the result of an the info request received  by 
 *                the application.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Mandatory Parameters */
    icf_result_t		result;
    icf_uint8_t			bit_mask;

#define ICF_INFO_CFM_ERROR_CAUSE_PRESENT          0x01
#define ICF_INFO_CFM_RESPONSE_CODE_PRESENT        0x02
#define ICF_INFO_CFM_HEADER_LIST_PRESENT          0x04
#define ICF_INFO_CFM_TAG_LIST_PRESENT             0x08
#define ICF_INFO_CFM_BODY_LIST_PRESENT            0x10

    /* Optionsl Parameters */
    icf_uint32_t 		response_code;  
    icf_error_t			error_cause;
    icf_header_list_st		header_list;
    icf_tag_list_st		tag_list;
    icf_msg_body_list_st	body_list;

} icf_info_cfm_st;

typedef struct
{ 
    icf_uint8_t         dscp_bits;

} icf_set_dscp_bits_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_early_call_ind_st
 *
 * Description:	  This API is sent by IMS Module to inform Application about
 *                an early call indication, so that Application can look whether
 *                it has to accept the call or reject the call.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{

icf_uint8_t			bit_mask ;

#define ICF_EARLY_CALL_CLG_PTY_ADDR      0x01
#define ICF_EARLY_CALL_CLG_PTY_NAME		 0x02
#define ICF_EARLY_CALL_CALLED_PTY_ADDR	 0x04

    /* Denotes the address of the remote party calling the local user. */
    icf_address_st 				calling_party_addr;

    /* Denotes the name of the remote party calling the local user. */
    icf_name_st 				calling_party_name;

    /* Denotes the address of the local user. */
    icf_address_st 				called_party_addr;

    /* List of additional headers that are of importance to the application. */
    icf_header_list_st 			header_list;

} icf_early_incoming_call_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_early_call_cfm_st
 *
 * Description:	  This is the response of the early call indication API sent
 *				   to the application earlier. The line id must be valid in case
 *					response is Success.
 *                
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
	/* response for the early call indication*/
	icf_result_t			result;
	/*line id must be specified by the APP in case response is Success */
	icf_line_id_t			line_id;

}icf_early_incoming_call_cfm_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_unexpected_msg_ind_st
 *
 * Description: This API is invoked by ICF towards the Application
 *              to convey to application that the message which was 
 *              sent to ICF by Application,was an invalid(unexpected) message.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
   /* Denotes the error cause */
   icf_error_t          error_cause;   
   
   /* Denotes the API identifier for which the indication is sent. */
   icf_api_id_t         api_id;
}icf_unexpected_msg_ind_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_stop_fax_resp_st
 *
 * Description: This API is send by IPTK  to notify the application for stop
 * 		fax response.
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* bitmask added for retyr after handling */
    icf_uint32_t                 bit_mask;

    /* result of the request */
    icf_result_t result;

    /* in case of failure, the error cause */
    icf_error_t error_cause;
    
    /* duration received in retry-after header*/
    icf_duration_t    retry_after_duration;

}icf_stop_fax_resp_st; 


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_stop_fax_ind_st
 *
 * Description: This API is send by IPTK  to notify the application for stop
 * 		fax indication.
 *---------------------------------------------------------------------------*/
typedef struct
{
	/* This field will indicate which fax call is stopped
	 * by remote */
	icf_fax_start_type_t   fax_type;
}icf_stop_fax_ind_st; 


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_media_connect_req_st
 *
 * Description: This API is invoked by Application towards the IMS Module
 *              in response to the ICF_INCOMING_CALL_IND API to send 183 with.
 *              SDP on network.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
		/* icf REL 6.0: multiple m lines */
#define ICF_MEDIA_CONNECT_DELETED_STREAM_PRESENT	0x00000001
#define ICF_MEDIA_CONNECT_MEDIA_MODE_PRESENT		0x00000002

    icf_uint32_t bit_mask;
    /* Use common hash defines if privacy/header/msg-body/tag/stream 
     * list present in the API. 
     */

    /* Indicates whether privacy is required. */
    icf_boolean_t privacy_ind;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st header_list;

    /* List of additional tags that the application wants to send in the 
     * message request.
     */
    icf_tag_list_st tag_list;

    /* List of message bodies that the application wants to send in the
     * message request.
     */
    icf_msg_body_list_st body_list;

    /* List of encoded_attributes for a media stream that application wishes 
     * to set in the outgoing response to the remote offer. Node is of type
     * icf_stream_capab_st */
    icf_list_st			*p_stream_list;

	/* icf REL 6.0: multiple m lines */
	/* the application may delete any stream during the call setup. This is 
	 * optional. The application will only return the rejected/deleted stream 
	 * information. The application will not be allowed to change/modify any
	 * codecs or any other parameter within the stream. The application if 
	 * desires can reject a stream only */
	/* the application should not indicate those stream_id's that have been
	 * already deleted in the onging session */
	icf_stream_id_t				count;
	icf_stream_list_st		deleted_media_streams[ICF_MAX_STREAM_PER_CALL];	
    icf_media_mode_t        media_mode;

    /* Indicates whether the SIP message needs to be sent with or without SDP.
     * To send 183 without SDP, set send_without_sdp = ICF_TRUE
     * Else To send 183 with SDP, set send_without_sdp = ICF_FALSE
	 * If set to true, ICF will not process the deleted streams and media mode
	 * parameters in this api.
	 */
    icf_boolean_t			send_without_sdp;
} icf_media_connect_req_st;

typedef struct
{
#define ICF_CALL_TRANS_INIT_ERROR_CAUSE_PRESENT         0x40000000
#define ICF_CALL_TRANS_INIT_CFM_CODE_PRESENT            0x80000000


    icf_uint32_t bit_mask;

    /* Indicates the result of the call transfer req (success/failure).
success : 1 and failure : 0 */
    icf_result_t result;

    icf_error_t error_code ;

    /* Response code to be sent for the call transfer request. */
    /* This variable is for future use */
    icf_uint32_t response_code ;


} icf_remote_call_transfer_initiated_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_get_register_status_req_st
 *
 * Description: This is payload of API ICF_GET_REGISTER_STATUS_REQ.
 *              This API can be sent by application to request for user REGISTER
 *              status.
 *
 *---------------------------------------------------------------------------*/

typedef struct
{

        icf_line_id_t           line_id;

} icf_get_register_status_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_get_register_status_resp_st
 *
 * Description: This is payload of API ICF_GET_REGISTER_STATUS_RESP.
 *              This API is sent by IPTK to PA in response to API
 *              ICF_GET_REGISTER_STATUS_REQ.
 *
 *---------------------------------------------------------------------------*/


typedef struct
{

        /* This will be a list of icf_user_reg_status_st*/
        icf_list_st                     *p_user_reg_status;

        icf_transport_address_st        registrar_address;

        icf_duration_t                  expires_duration;

        icf_result_t                    result;

        icf_error_t                     error_cause;

}icf_get_register_status_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_call_modify_cancel_resp_st
 *
 * Description: This is payload of API ICF_CALL_MODIFY_CANCEL_RESP.
 *              This API is sent by IPTK to Application to indicate that the 
 *              call modify cancellation was successful or not.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* result of the request */
    icf_result_t             result;

    /* in case of failure, the error cause */
    icf_error_t              error_cause;

}icf_call_modify_cancel_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_progress_ind_st
 *
 * Description: This API is invoked by IMS Module towards Application when
 *		the SIP peer sends non-alerting provisional response 
 *		without sdp indicating that the call is in progress. It is 
 *		sent with the api id ICF_PROGRESS_IND.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
   	/* status code received in the progress message from peer */
    icf_uint16_t   					response_code;

} icf_progress_ind_st;

/***************************************************************************
*                   XCONF APIs
***************************************************************************/

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_create_xconf_req_st 
 *
 * Description: This is payload of API ICF_CREATE_XCONF_REQ.
 *              This API is sent by Application to IPTK to initiate external
 *              conference
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_uint32_t             bitmask;
    /* The bitmask ordering is important as it has mapping 
       with create_call_req API */
#define ICF_CREATE_XCONF_CONF_URI_PRESENT            0x0001
#define ICF_CREATE_XCONF_TOGGLE_PRES_IND_PRESENT     0x0002    
#define ICF_CREATE_XCONF_PREF_MEDIA_PROFILE_PRESENT  0x0004
#define ICF_CREATE_XCONF_EXIST_CALL_ID_PRESENT       0x0008
#define ICF_CREATE_XCONF_CALLING_PARTY_ADDR_PRESENT  0x0010
#define ICF_CREATE_XCONF_CALLING_PARTY_NAME_PRESENT  0x0020

    /* the line_id on which the xconf needs to be established */
    icf_line_id_t            line_id;

    /*Address of conference server*/
    icf_address_st           conference_uri;

    /*The call id of the ingoing call for transferring it on to the Conference*/
    icf_call_id_t            existing_call_id;

    /* List of additional headers that the application wants to send in the
     * message request
     */
    icf_header_list_st       header_list;

    /* List of additional tags that the application wants to send in the
     * message request.
     */
     icf_tag_list_st         tag_list;

     /* this will be a list of icf_stream_capab_st */
     icf_list_st             *p_pref_media_profile;
    
     icf_address_st          calling_party_address;
    
     icf_string_st           calling_party_name;

     /* Indicates whether the local user wants to toggle the presentation
     * indication status for the current call.
     */
     icf_boolean_t           toggle_presentation_indicator;
} icf_create_ext_xconf_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_add_xconf_party_req_st 
 *
 * Description: This is payload of API ICF_ADD_XCONF_PARTY_REQ
 *              This API is sent by Application to IPTK to add a party to 
 *              existing XCONF conference
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_uint32_t             bitmask;

    /* List of additional headers that the application wants to send in the
     * message request
     */
     icf_header_list_st      header_list;

    /* List of additional tags that the application wants to send in the
     * message request.
     */
     icf_tag_list_st         tag_list;

     /* call id of call between host party and new party to be added to conference */
     icf_call_id_t          add_party_id;

}icf_add_xconf_party_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_delete_xconf_party_req_st
 *
 * Description: This is payload of API ICF_DELETE_XCONF_PARTY_REQ
 *              This API is sent by Application to IPTK to delete a party from 
 *              existing XCONF conference
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_uint32_t             bitmask;

    /* List of additional headers that the application wants to send in the
     * message request
     */
     icf_header_list_st      header_list;

    /* List of additional tags that the application wants to send in the
     * message request.
     */
     icf_tag_list_st tag_list;

     /* URI of party to be deleted from the conference */
     icf_address_st          del_party_uri;

}icf_delete_xconf_party_req_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_release_xconf_req_st
 *
 * Description: This is payload of API ICF_RELEASE_XCONF_REQ
 *              This API is sent by Application to IPTK to release / delete
 *              existing XCONF conference
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_uint32_t             bitmask;

    /* List of additional headers that the application wants to send in the
     * message request
     */
     icf_header_list_st      header_list;

    /* List of additional tags that the application wants to send in the
     * message request.
     */
     icf_tag_list_st tag_list;

}icf_release_xconf_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_release_xconf_resp_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_RELEASE_XCONF_RESP.
-----------------------------------------------------------------------------*/
typedef struct
{
    /* Denotes the Failure status only. */
    icf_boolean_t result;

    /* Denotes the error cause if the status of the API is Failure. */
    icf_error_t   error_code;
}icf_release_xconf_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_create_xconf_resp_st
 *
 * Description: This is payload of API ICF_CREATE_EXT_XCONF_RESP
 *              This API is sent by IPTK to Application to indicate that the 
 *              conference is initiated successfully or not
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_uint8_t             bitmask;

     icf_result_t            result;

     /* Denotes the error cause if the status of the API is Failure. */
     icf_error_t             error_cause;

}icf_create_ext_xconf_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_add_xconf_party_resp_st
 *
 * Description: This is payload of API ICF_ADD_XCONF_PARTY_RESP
 *              This API is sent by IPTK to Application to indicate that the
 *              party is added successfully
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_result_t         result;

     /* Denotes the error cause if the status of the API is Failure. */
     icf_error_t          error_cause;

}icf_add_xconf_party_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_delete_xconf_party_resp_st
 *
 * Description: This is payload of API ICF_DELETE_XCONF_PARTY_RESP
 *              This API is sent by IPTK to Application to indicate that the
 *              party is deleted successfully
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
     icf_result_t         result;

     /* Denotes the error cause if the status of the API is Failure. */
     icf_error_t          error_code;

     /* URI of party to be deleted from the conference */
     icf_address_st       deleted_party_uri;
}icf_delete_xconf_party_resp_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_xconf_release_ind_st
 *
 * Description: This is payload of API ICF_RELEASE_XCONF_IND
 *              This API is sent by IPTK to Application to indicate that the
 *              external conference is released
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    icf_call_clear_reason_t call_clear_reason;
}icf_xconf_release_ind_st;

#ifdef __cplusplus

}

#endif /* __cplusplus */





#endif /* __ICF_API_H__ */


