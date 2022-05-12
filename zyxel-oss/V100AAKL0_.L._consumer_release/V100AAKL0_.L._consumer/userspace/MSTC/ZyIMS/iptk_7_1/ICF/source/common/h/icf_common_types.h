/*------------------------------------------------------------------------------
 *
 * File name        : icf_common_types.h
 *
 * Purpose          : This file contains all the common structures
 *                    or typedefs that are being used across the whole ICF.
 *
 * Revision History :
 *
 *   Date        Name           Ref#      Comments
 * --------      ------------   ----      ---------------
 * 03-Nov-2004   Mayank Rastogi           File created and initial version
 * 18-Nov-2004   Rohit Aggarwal           Added fields in call context
 * 24-Nov-2004   Rohit Aggarwal           Added timer structure
 * 16-Dec-2004   Sumit Gupta              Added fields in global data
 * 20-Dec-2004   Rohit Aggarwal           Updated fields in call context
 * 21-Dec-2004   Rohit Aggarwal           Added a #define for invalid timer id
 * 29-Dec-2004   Aashish Suchdev          Added the pointer for porting info in
 *                                          global data
 * 14-Jan-2005   Rohit Aggarwal           Removed Logiscope warning
 * 25-Jan-2005   Amit Sharma              Addd call_status in icf_call_ctx_st
 * 31-Jan-2005   Jalaj Negi               Added retry duration field in config
 *                                        data structure
 * 15-Feb-2005   Navneet Gupta            Changed Service Subscription flags
 *                                        from uint8 to uint16
 * 22-Mar-2005   Amit Sharma              Added rnw_timer in config_data and 
 *                                        removed proxy_used_flag from line_data
 * 27-Apr-2005   Umang Singh              Added a new structure for SSA-RNW 
 *                                          interface for Replaces header
 * 24-May-2005   Sumit Gupta   SPR 7610   Added a new field rem_committed_sdp                                             in 

 * 01-Jun-2005   Jalaj Negi   IMS CLIENT  Added fields for IMS CLIENT
 *                                        compliance.
 * 09-Jun-2005   Jalaj Negi   IMS CLIENT  Added Flag in call context structure
 *                                        to indicate that 200 OK for PRACK
 *                                        is receive
 * 10-Jun-2005   Umang Singh              Added a new #define in common bitmask
 *                                        for privacy header handling
 * 13-Jun-2005   Amit Sharma   SPR 7662   Added call_status flag and 
 *                                        p_conf_other_call_ctx in call context 
 * 23-Jun-2005   Jalaj Negi   IMS CLIENT  Added structure for bandwidth
 *                                        support
 * 04-Aug-2005    Rohit Aggarwal    ICF          Changes for ICF compilation
 * 09-Aug-2005  Saurabh Gupta   ICF       Changes for ICF datastructures
 * 10-Aug-2005    Rohit Aggarwal    ICF          Added new members to call ctx structure
 * 12-Aug-2005  Ashutos Rath    ICF          sigcomp_support_by_proxy is added in
 *                                        the line_data_st
 * 17-Aug-2005  Rohit Aggarwal  ICF          Added p_called_party_id in call ctx
 * 07-Dec-2005  Rohit Aggarwal  ICF CDD   Code changes for IPSec support
 * 07-Dec-2005	  Jyoti Narula		ICF 4.0		New member in init_param_st for Auto-Register
 * 08-Dec-2005    Navneet Gupta   ICF 4.0 Added internal filter list structures
 *                                        for Multiple App filtering support
 * 18-Jan-2006	Aman Arora	    SPR 9591	for adding TBCP stream in the otg 
 * 02-Feb-2006  Amit Sharma               CFG merged with ICF 2.0
 * 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
 * 22-Feb-2006	Amit Sharma               Added code for auth support
 * 30-Mar-2006  Anuradha Gupta	ICF 5.0    Call Modify Feature changes
 *       
 * 27-Nov-2006  Deepti Goyal    ICF Rel6.2  NAT "rport" Feature Support 
 *
 * 12-Dec-2006  Deepti Goyal    SPR 14030   Store the initial registration
 *                                          duration in the rport config data
 * 28-Dec-2006  Deepti Goyal    SPR 14173   Store the initial registration boolean
 * 28-Nov-2006  Umang Singh     SPR 13862  
 * 03-Jan-2007  Amit Mahajan                INFO confirmation changes. 
 * 04-Jan-2007   Priyanka Dua   ICF 6.3     Added bitmask in call_context/field in 
 *											icf_config_data_st for Forking and 
 *											Multiple answer
 * 23/02/2007    Umang Singh    Solving the race condition for timer expiries
 * 26-Mar-2007  Deepti Goyal    ICF 6.4     Changes for media connect API from PA.
 *
 * 12-MAr-2007  Deepti Goyal    SPR 14682   Contact User Info in line data	
 * 04-Apr-2007  Deepti Goyal    ICF 6.3     Changes for media connect API from PA.
 *
 * 11-Apr-2007  Deepti Goyal    ICF 6.3     Added new bitmask for local media 
 *                                          connect codec change.
 * 17-Apr-2007  Abhishek Dutta  ICF 6.3     Added the changes for 
 *                                          TWC-UPDATE Handling
 * 09-Jul-2007  Amit Sharma     ICF 7.0     Added changes for 7.0 enhancements
 * 09-Jul-2007  Neha Choraria   ICF 7.0     Changes for Forking & Multiple 
 *                                          Answer
 * 11-Jul-2007  Neha Choraria   ICF 7.0     Added ICF_CC_491_RESP_PROCESS_ONGOING
 *                                          for changes in Retry-After header
 * 13-Jul-2007  Abhishek Dhammawat Rel7.0   Modifications for XCONF feature.
 * 31-Jul-2007  Amit Mahajan    SPR 15478
 * 7-Sep-2007   Deepti Goyal    SPR 16019   Changes for TCP blocking connect.
 * 25-Jun-2008  Rajiv Kumar     SPR - 18722 IPTK not compilable with gcc
 *                                          version 4.2.3 
 * 07-Nov-2008  Anurag Khare    SPR 19168   new field add in icf_config_data_st
 *                                          port_to_be_sent_in_from_header
 *
 * 07-Jan-2009  Vinay Jindal    SPR 19250   Added 'conf_fail_call_clear_not_reqd'
 *                                          in icf_config_data_st
 * 05-Feb-2009   Rohit Aggarwal  SPR 19316   Changes for INVITE with Replaces
 * 29-Jun-2009	Aman Aggarwal	SPR 19792	Added handling of mode in 200OK in case of 
 *          							    P-Early-media in initial INVITE	
 * 27-Aug-2009 Anurag Khare     SPR 19989   Changes done for NOTIFY challenge
 *                                          Enhancement
 * 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_COMMON_TYPES_H__
#define __ICF_COMMON_TYPES_H__

#include "icf_types.h"
#include "icf_port_types.h"
#include "icf_api.h"
#include "icf_port_pltfm_types.h"
#include "icf_port_pltfm_defs.h"
#include "icf_common_defs.h"
#include "icf_common_interface_defs.h"
#include "icf_trace_error_types.h"
#include "icf_trace_id.h"
#include "icf_mm_types.h"
#include "icf_stats_types.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_map_extif_feature_flags.h"
#endif

#include "icf_trace_id.h"
#ifdef ICF_TRACE_ENABLE
#include "icf_trace_error_types.h"
#endif

#include "icf_error_id.h"
#ifdef ICF_ERROR_ENABLE
#endif

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

#ifdef ICF_PORT_SYMBIAN
typedef icf_uint32_t         icf_timer_id_t;
typedef icf_uint8_t  icf_pool_id_t;
/*typedef icf_uint8_t  icf_hash_table_id_t;*/
typedef icf_uint32_t  icf_time_t;
#endif


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cc_timer_list_st
 * Description :  Timer list maintained by CC
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /*
     * timer_id array will store the id of currently running timer. For all
     * other timers that are not running currently, value would be 0. We could
     * have had a single value rather than an array, but array is only for
     * future support in case we wish to run more than one timers simultaneously
     */
#define ICF_TIMER_ID_INVALID                 0
#define ICF_CC_NUMBER_OF_TIMERS              4
    icf_timer_id_t             timer_id[ICF_CC_NUMBER_OF_TIMERS];

} icf_cc_timer_list_st;

/* -----------------------------------------------------------------------
 * Type Name    : icf_int_module_id_t
 * Description  : To define the type for internal module ids that form
 *                SIP Packet Phone Signaling Toolkit.
 * ----------------------------------------------------------------------*/
typedef icf_uint8_t   icf_int_module_id_t;

/* --------------------------------------------------------------------
 * Type Name   :  icf_hash_table_id_t
 * Description :  A typedef for hash table id used for hash table indexing
 *                initialization and deletion.
 *  
 * ------------------------------------------------------------------------*/
typedef icf_uint16_t	icf_hash_table_id_t;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_timer_type_t
 *
 *  Description :    Would contain different timer types e.g. call hold timer
 *
 *---------------------------------------------------------------------------*/
typedef icf_uint8_t			icf_timer_type_t;


typedef icf_uint8_t         icf_cc_fsm_state_t;

typedef icf_uint8_t         icf_mmi_state_t;

/*Fix done for SPR-18722 : CSR-1-6068965*/
typedef icf_uint16_t         icf_mmi_event_t;

/*typedef icf_uint8_t         icf_media_mode_t;*/

typedef icf_return_t	(*icf_cmn_compare_identifier_fn_ptr_t)
(		
	INOUT	icf_void_t          	*p_tmp_node,
	INOUT	icf_void_t				*p_identifier
);

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_rport_config_data_st 
 * Description :  All configuration data for rport support.
 *
 *----------------------------------------------------------------------------*/

#ifdef ICF_NAT_RPORT_SUPPORT
typedef struct{
    
   /* Define a contact address which will be contain the Public Address
    *
    */
   icf_transport_address_st    contact_ip_addr;
   icf_transport_address_st    temp_contact_ip_addr;
   icf_boolean_t               is_rport_reqd;
   icf_uint8_t                 reg_id; 
   icf_duration_t              binding_refresh_timer;
   /* BINDING Refresh timer id */
    icf_timer_id_t               binding_refresh_timer_id;
    /* BINDING Refresh timer data */
    /* icf_timer_data_st           *p_binding_refresh_timer;*/
    icf_void_t           *p_binding_refresh_timer;
    /* This will store the IP address to be filled in the "rport" and 
     * "received" parameters in the Via Header of a response, if "rport"
     * received in the request.
     */
    icf_transport_address_st    received_addr;
    icf_boolean_t               nat_binding_chng;
    icf_duration_t              init_reg_duration;
    icf_line_id_t               line_id;
    icf_address_st              user_address;
    icf_boolean_t               is_initial_reg;
    icf_boolean_t               is_rport_completed;
    /* this flag is to be set / reset one time when seld address
      configuration is per line */
    icf_boolean_t       is_nat_resolved_sys;
}icf_rport_config_data_st;
#endif

#if defined (ICF_TRACE_ENABLE) || defined (ICF_ERROR_ENABLE)
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_print_info_st
 * Description :  Contains the trace and error logging parameters
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* enable or disable trace/error */
    icf_status_t          	status;

    /* level of the traces/errors to be reported */
    icf_uint8_t     		level;

    /* type of traces/errors to be reported */
    icf_uint32_t      		type;

} icf_print_info_st;
#endif


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_config_data_st
 * Description :  All configuration data
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
   /*------------- Scalable data -------------*/
    /* max number of lines supported by phone */
    icf_line_id_t                  max_lines;

ICF_CRM_START
    /* max number of applications supported by phone */
    icf_uint8_t         max_applications;
#ifdef ICF_NW_ACTIVATE
    /* max number of networks supported by phone */
    icf_uint8_t         max_networks;

    /* retry timer duration for network activate timer */
	icf_duration_t		net_act_retry_timer_duration;

	/* maximum number of retry attempts for network activation */
	icf_uint8_t		net_act_retry_timer_attempts;
	
	/* current attempt # */
	icf_uint8_t		  attempt_id;
#endif
	/* sigcomp init time option. 0=off 1=on */
    icf_uint8_t         sigcomp_required;

	/* auth_type: 0 for ICF_IMS_AUTH_OPTION, 
				1 for ICF_NONIMS_AUTH_OPTION */
	icf_uint8_t		   auth_type; 
	icf_string_st       auth_password;
	/* Client nonce being read from icf.cfg */
    	icf_string_st       client_nonce;
	
/*	icf_uint16_t       recv_from_app_port;  ICF shall listen on this port for msgs from applications 
	icf_uint16_t       recv_from_network_port;  ICF shall listen on this port for msgs from network */

	icf_uint16_t       send_to_network_port; /* ICF shall send message to network on this port */

	
ICF_CRM_END

ICF_4_0_CRM_START
	/* Status of AUTO REGISTER: ON=1 or OFF=0 */
	icf_uint8_t		auto_register;
	icf_uint8_t		auto_scale;
#ifdef ICF_NW_ACTIVATE
	icf_uint8_t		auto_activate;
#endif
	icf_uint32_t	opt_parms_bitmask;

ICF_4_0_CRM_END

    /* max number of simultaneous audio calls handled by the phone */
    icf_uint16_t                 max_audio_calls;

    /* max number of simultaneous video calls handled by the phone */
    icf_uint16_t                 max_video_calls;

    /*
     * max number of simultaneous service calls (audio as well as video)
     * handled by the phone
     */
    icf_uint16_t                 max_service_calls;
    icf_uint16_t                 max_num_of_calls;

    /*Max number of simultaneous registration calls done by phone*/
    icf_uint16_t                  max_registration_req;

    /* max number of users that can be configured per line */
    icf_uint16_t                 max_users_per_line;

    /*----------- Transport data ----------------*/
    /* self IP address - default value is obtained using system call */
    icf_transport_address_st     self_ip_address;

    icf_transport_mode_t         self_mode;

    /*------------- Call data -------------------*/
    /*
     * max number of calls to be stored in call log history, value 0 means no
     * logging - default value is 4
     */
    icf_uint8_t                  num_of_calls_to_log;

/*
#define ICF_CFG_CC_PRIVACY_HEADER_PRESENT   default is TRUE 
#define ICF_CFG_CC_PRACK_REQD               default is FALSE 
#define ICF_CFG_CC_CALL_THRU_SIP_PROXY    
*/
    icf_uint8_t                  default_settings;

    icf_qos_mark_t               qos_mark;    

    /* setup timer - default value is 120000ms */
    icf_duration_t               setup_timer;

    /* alert timer - default value is 30000ms */
    icf_duration_t               alerting_timer;

    /* release timer - default value is 60000ms */
    icf_duration_t               release_timer;

    /* modify media timer - default value is 500ms */
    icf_duration_t               modify_media_timer;


    /*
     * registration response timer, this is the time for which phone should wait
     * for response from registrar for a registration request - default value
     * is 30000ms.  This runs over and above the UATK timer mechanism for 
     * retx of REG cmd.
     */
    icf_duration_t               registration_resp_timer;

    /*Duration for which REGM will wait before sending new registration
     * in case of failure response from SSA.
     */
    icf_duration_t               registration_retry_timer;

	icf_duration_t               subs_retry_timer;

	icf_duration_t               subs_resp_timer;

	 /* This timer will be started when INFO request will be sent on nw
     * and stopped when response is recd. The default value is 120000 ms. 
     */
    icf_duration_t               info_timer;
    
    icf_duration_t               options_timer;

    /* User-Agent Parameter to be used in SIp URI */
    icf_string_st                *p_phone_info;

    /* Application specific parameter to handle UPDATE */
    icf_uint8_t                  update_reqd;

    icf_duration_t               dns_max_retrans;
     
    icf_duration_t               dns_min_retrans; 
 
    /*------------------- Service data ------------------------*/
    icf_hold_option_t            hold_option; 

 /* This bitmask specifies whether the service subscribed is for whole 
     * system
     */
/*#define ICF_SERVICE_GLB_SUBS_MESSAGE_WAITING_IND          0x0001 */
    icf_uint8_t      global_service_subscription_flag;

    /* This is a bitmask,which specifies the service activated for whole 
     * system 
     */

/*#define ICF_SERVICE_GLB_ACT_MESSAGE_WAITING_IND           0x0001 */
    icf_uint8_t                   global_service_activation_flag;


    /* Maximum number of hops for which a call can be forwarded.
     */

    icf_uint8_t                    max_num_of_call_forwarding_hops;

    /* Maximum number of calls in call waiting simultaneously.
     * After this threshold Call Forwarding with Busy Line
     * will be invoked.
     */
    
    icf_uint8_t                   max_num_of_calls_in_call_waiting;

    
    /* call hold resume timer - default value is 60000ms */
    icf_duration_t               call_hold_resume_timer;

    /* re-invite race timer - default value is 5000ms */
    icf_duration_t               reinvite_race_timer;

    /* TWC timer - default value is 60000ms */
    icf_duration_t               twc_timer;

    /*Duration of notification timer */
    icf_duration_t                ctu_timer;

    /*Duration of cta timer */
    icf_duration_t                cta_timer;

    /*Duration of cta timer */
    icf_duration_t                rnw_timer;

    /* Duration of cw alert timer */
    icf_duration_t                cw_alert_timer;

    /* Duration of cnd alert timer */
    icf_duration_t                cnd_alert_timer;

    /* Duration of cw and cnd alert timer */
    icf_duration_t                cw_n_cnd_alert_timer;
#if defined (ICF_TRACE_ENABLE) || defined (ICF_ERROR_ENABLE)
	/* Print info for Traces */
	icf_print_info_st	 trace_info;
#endif



     /*-------------------------VMS Data-------------------------------*/

     /* Group VMS Address*/
     icf_address_st                 *p_group_vms_addr;

     /* Group VMS Duration*/
     icf_duration_t                  vms_subscribe_duration;

     /* Bitmask to indicate which type of VMS is set. If the subscription is
      * for group, the MSB, ICF_CFG_VMS_GRP_SUBSCRIBED will be set. If the
      * subscription is for line, then MSB will not be set.The value in the 
      * other 7 bits of the bitmask will be equal to the number of lines on 
      * which VMS address has been set.
      */
#define ICF_CFG_VMS_GRP_SUBSCRIBED   0x80
#define ICF_CFG_VMS_LINE_SUBSCRIBED  0x40
     icf_uint8_t                     vms_subscription_status;

     /* Max Subm Context */
     icf_uint8_t                     max_num_of_subm_ctx;


   /*
    * flag that is set when all the mandatory parameters have been recd once -
    * init value is ICF_FALSE
    */
   icf_boolean_t                 config_data_init_complete;

   /* Flag which should be set to ICF_TRUE when network resource 
    * reservation is supported by IMS CLIENT and is sent in Qos precondition
    * otherwise it should be set to ICF_FALSE.
    */
   icf_boolean_t                 nw_res_reservation_supported;

#ifdef IMS_CLIENT
   /* This is an array which specifies total bandwidth requirement by the
    * application for a particular codec.
    */
   icf_codec_bw_map_list_st      codec_bw_map_list;
#endif

#ifdef ICF_SESSION_TIMER
   icf_duration_t      session_timer;
   icf_duration_t      min_se;
  /* A new Boolean named √¢?¨session_timer_enable√¢?¨‚Ñ¢ will be added in this structure that   
    * will be indicate whether session timer needs to be enabled or not. If it is 1 then session  
    * timer feature should be enabled, if it 0 then session timer needs to be disabled.
    * By Default this boolean will be TRUE i.e. session timer feature will be enabled.*/      
   icf_boolean_t               session_timer_enable;

   /* Following boolean will be used to specify by application that whether in Outgoing 
    * INVITEs refresher needs to be sent in Session-Expires header or not. Session  
    * refresher need not to be stored here in config data, because as set_call_params API 
    * will be fired and if application wants  to configure session refresher, we will update  
    * it on every line.
    * By default, this boolean will be TRUE i.e. session_refresher  will be sent out on nw.*/
   icf_boolean_t               session_refresher_reqd;  

   /* This boolean will specify that whether timer logic required is ASB specific or not.   
    *  By default this boolean will be FALSE i.e. existing handling of running timer will 
    * continue.*/
   icf_boolean_t               session_timer_logic; 
#endif
ICF_CONF_STATUS_API_START
    
   icf_void_st*                  p_generic_routing_info;

ICF_CONF_STATUS_API_END

#ifdef ICF_IPSEC_ENABLED
	/* flag indicating whether IPsec support is currently enabled or not */
    icf_boolean_t    	        ipsec_enabled;

    /* This pointer will be not null only if ipsec_enabled flag is true
	 * IPSec init-time parameters */
    icf_ipsec_config_params_st	*p_ipsec_config_data;
#endif /* ICF_IPSEC_ENABLED */
    icf_uint8_t sip_compact_hdrs;
    
   /*
    * flag that is set when all the mandatory parameters have been recd once -
    * init value is ICF_FALSE
    */
   icf_boolean_t                 scale_param_recd;

   icf_boolean_t                 nw_trace_enabled;
    /*
    * flag that is set to TRUE when ICF to behave IMS CLIENT FRAMEWORK
    * Set To FALSE, When ICF to behanve IpPhonce Client Framework
    */
   icf_boolean_t                 ims_oprn_flag;
   icf_uint8_t			         access_net_type;
   icf_string_st				 apn;


/* ICF_HANDLE_REINVITE_START*/

	/* call modify request method:   0 for INVITE, 
					   	   1 for UPDATE */
	icf_uint8_t			call_modify_req_method;
	
/* ICF_HANDLE_REINVITE_END*/

#ifdef ICF_NW_ACTIVATE
	 /* This flag will be set to TRUE when
    * the SUSPEND indication is received from NM for GPRS. */
   icf_boolean_t		gprs_suspended;

#endif

/*NOTIFY_OUT_OF_DIALOG_START*/

   /* out of dialog message handling enable. 0=off 1=on */

    icf_uint8_t         non_sub_notify_enable;

/*NOTIFY_OUT_OF_DIALOG_END*/


   icf_boolean_t		        is_blocking_connect;
   icf_tls_init_params_st      *p_tls_init_data;/* contains the TLS parameters info */
#ifdef ICF_NAT_RPORT_SUPPORT
   icf_rport_config_data_st     rport_config_data;
  #endif

/*This flag will be set TRUE if set_trans_req_st has self_addr_per_line 
    *bit set*/
#define ICF_CFG_SELF_ADDR_INV_TYPE       0x00
#define ICF_CFG_SELF_ADDR_SYS_WIDE       0x01
#define ICF_CFG_SELF_ADDR_PER_LINE       0x02
   icf_uint8_t   self_addr_per_line;
  
   icf_uint16_t     server_index[ICF_INTF_INFO_PROTOCOL_TLS + 1];

  /* value ICF_TRUE= multiple answer ON, ICF_FALSE = multiple answer OFF */	
   icf_boolean_t        multiple_answer_enable;
#ifdef ICF_DNS_LOOKUP_ENABLED
   /* parameters added for DNS Buffer */
   icf_uint8_t			sip_req_retry;
   icf_uint32_t			dns_purge_timer;	
#endif
   /* end of added parameters for DNS Buffer */

    /*Max SIP call objects*/
    icf_uint16_t                 max_sip_call_objs;

    /*Max number of timer blocks*/
    icf_uint16_t                 max_num_of_timers;

    /* value ICF_TRUE = early_inc_call_ind for all users ON, 
     * ICF_FALSE = early_inc_call_ind for all users OFF */ 
    icf_boolean_t                 early_inc_ind_for_all_users;
    icf_boolean_t                 conf_reqd_at_transferee;
    icf_boolean_t                 conf_cfm_reqd_at_transferee;
    icf_boolean_t                 xconf_subs_reqd;
    icf_duration_t                xconf_subs_duration;
    /* SPR 16019: Blocking Connect boolean: this will be set whenever any
     * Register transaction is forcefully cleared and will be reset when
     * transaction sent on network.
     */
    icf_boolean_t		          is_tcp_blocking_connect;
    /* In cfg data we will set this bitmask dns_configuration with the following 
    * bitmask value, if application has specified dns port for making dns query. 
    */
   icf_uint8_t                   dns_configuration;
#define                ICF_DNS_PORT_CONFIGURED_BY_APP      0x01

   /* Following variable will be containing dns port that application wants to configure. This port 
    * will be system wide.
    */
   icf_uint16_t                  dns_source_port;
   /*  This boolean indicates that the port sould be sent in
    *  From header or not 
    */
   icf_boolean_t                 port_to_be_sent_in_from_header;

   /* This parameter indicates if calls need to be cleared when 
    * conference request fails due to merge media failure or 
    * twc timer expiry
    */
   icf_boolean_t                 conf_fail_call_clear_not_reqd;

    /* This parameter indicates whether IPTK should directly send 200 OK for
     * INVITE with Replaces and create the media session, or present INVITE with
     * Replaces as a normal incoming call and wait for APP request to connect 
     * the call.
     * -  ICF_TRUE means call will be connected on getting ICF_CONNECT_REQ.
     * -  ICF_FALSE means call will be connected by IPTK.
     * Default value for this boolean is ICF_FALSE.
     */
    icf_boolean_t           app_connect_reqd_for_replaces;

} icf_config_data_st;



/* ---------------------------------------------------------------------------
 * Type Name   :  icf_internal_list_st
 * 
 * Description :  This is a generic list used to store data of any type
 * 				  within ICF
 * -------------------------------------------------------------------------*/
typedef struct icf_internal_list_s
{
	/* saves the pointer to the data to be stored in the list */	
	icf_void_t		*p_payload;
	/* saves the identifier that is to be used as an index into the list */
  	icf_void_t		*p_identifier;
	/* points to the function that implements the comparison function 
	 * for the list. This will be unique to the calling module that implements
	 * the list */
  	icf_cmn_compare_identifier_fn_ptr_t	func_ptr;
	/* points to the next node of the list */
  	struct icf_internal_list_s	*p_next;
} icf_internal_list_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_diversion_data_st
 * Decription  : Typedef to specify the parameters for diversion data.
 * --------------------------------------------------------------------------*/
typedef struct icf_diversion_data_s
{

     /* This bit mask decide id diverted_name_to field is valid or not*/
#define ICF_DIVERTERD_NAME_PRESENT     0x01
    icf_uint8_t           bitmask;

    icf_address_st        diverted_frm_addr;
    icf_name_st           diverted_frm_name;
    /* If more than one diversion header is present, then it is sent as linked
     * list. */
    struct icf_diversion_data_s  *p_next;
} icf_diversion_data_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_diversion_info_st
 * Decription  : Typedef to specify the parameters for diversion info. This
 *               is a set of diversion data parameters.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t   no_of_diversion_data;
    /* Specifies the no. of hops remaining. */
    icf_uint8_t            no_of_hops;
    icf_address_st         *p_contact_addr;
    icf_name_st            *p_contact_name;
    icf_diversion_data_st  *p_diversion_data;
} icf_diversion_info_st;

/* ----------------------------------------------------------------------------
 * Type Name   : icf_nw_call_id_t
 * Decription  : Typedef to network call id.
 * --------------------------------------------------------------------------*/
typedef struct
{
    icf_uint8_t  length;
    icf_uint8_t  nw_call_id[ICF_MAX_NW_CALL_ID_DGTS];
} icf_nw_call_id_t;


/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_replaces_hdr_st
 *
 *  Description :    Would define the structure which would contain the
 *                   the replaces call id and the from tag and to tag
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* Replaces call id */
    icf_nw_call_id_t      replace_nw_call_id;

    /* from tag present in Replaces hdr */
    icf_string_st         from_tag;

    /* to tag present in Replaces hdr */
    icf_string_st         to_tag;
} icf_replaces_hdr_st;

/* ICF rel 6.0 multiple m lines */
typedef struct 
{
	icf_stream_id_t			stream_id;
	icf_media_type_t		stream_type;
	
	icf_uint8_t				codec_count;
	
	/* This structure defines the per codec information in a stream */
	icf_codec_attrib_st		codec_info[ICF_MAX_CODECS];

}icf_codec_for_commit_st;

#ifdef IMS_CLIENT
/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_codec_bw_map_st
 *
 *  Description :    This defines a mapping of codec to total bandwidth. It 
 *                   indicates a bandwidth which is to be used if application 
 *                   uses a particular codec.It can be changed by application 
 *                   if it wishes to use another bandwidth value for a 
 *                   particular codec. 
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* IANA identifier of the codec */
    icf_codec_t              codec_id;

    /* Number of bandwidth's supported by this codec. Currently we are 
     * supporting one one bandwidth per codec. */
    icf_uint8_t              num_of_bw_supported;

    /* This specifies the total bandwidth for a particular codec in kilobits
     * per second.
     */
    icf_uint16_t             as_bandwidth[ICF_MAX_NUM_OF_BW_PER_CODEC];
} icf_codec_bw_map_st;

/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_codec_bw_map_list_st
 *
 *  Description :    This defines a list each element of which specifies a 
 *                   codec and its corresponding bandwidth.
 *
 *---------------------------------------------------------------------------*/
typedef struct
{
    /* This indicates number of valid elements of the mapping array */
    icf_uint8_t              num_of_codecs;

    /* This is an array which contains codec to bandwidth mappings */
    icf_codec_bw_map_st      codec_bw_map[ICF_MAX_CODEC_IN_LIST];
} icf_codec_bw_map_list_st;
#endif

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/*-----------------------------------------------------------------------------
 * typedef Name: icf_nat_stream_info_st
 * Description:  This structure will be used to list the deleted streams to
 * 				 MM. MM neednot know the stream_type, since it has all the 
 * 				 information that is mapped on the stream_id as the index.
 *----------------------------------------------------------------------------*/
typedef struct
{
	icf_uint8_t			        count;	
    icf_boolean_t               is_nat_complete;
	icf_stream_id_t		        stream_id;
    icf_transport_address_st    local_addr;
}icf_nat_stream_info_st;

/*-----------------------------------------------------------------------------
 * typedef Name: icf_nat_context_info_st
 * Description:  This structure will be used to store the stream id and 
 *               NAT context id mapping.
 *----------------------------------------------------------------------------*/
typedef struct
{
    icf_stream_id_t     stream_id;
    icf_uint32_t        context_id;
}icf_nat_context_info_st;

#endif


typedef struct
{
     /* The bits of the following bitmask indicate various bit flags */
     /*
      * This bit indicates that there was a failure during codec change and
      * MMI is currently trying to restore the earlier media parameters
      */
#define ICF_RESTORATION_IN_PROGRESS			0x01
     /*
      * This bit indicates that a media session has been established and
      * hence on receiving a DMS from CC,a Delete Media Session Request
      * needs to be sent to MM.This filed is set on getting a successful CMS
      * from MM and is unset on sending a DMS_REQ for all streams
      */
#define ICF_MEDIA_ESTABLISHED				0x02
     /*
      * This bit indiactes that the media session has been deleted and the
      * resources have been de-allocated.When this bit is set,and MMI
      * receives a DMS request from CC,it will simply send a successful
      * resp and not send any message on the RM/MM interface
      */
#define ICF_DELETE_COMPLETE					0x04
	 /* This bit indicates whether we have attempted 
	  * SECONDARY PDP ACTIVATION or not.
	  */
#define ICF_QOS_RESERVATION_ATTEMPTED		0x08

	 /* This bitmask is set when GCL is successful for at least one stream
	    during call setup. This bitmask is reset when REL_RES_REQ is being
		sent for streams during DMS_ONGOING or CMS_ONGOING */
#define ICF_MMI_RES_RESERVED				0x10

	 /* This bitmask is set when OPEN_CHANNEL is successful. This is reset
	  * when close_channel is sent for all streams */
#define ICF_MMI_CHANNEL_OPENED				0x20
	/* this bit is set in the MMI context to identify if the restoration is
     * ongoing for commit failure. this bit is used to identify if we need 
     * to send MMS request or not for the restored streams */
#define ICF_COMMIT_RESTORATION_IN_PROGRESS  0x40

	 /*	ICF_REL6.0:	for multiple m- lines*/
	  icf_uint8_t						mmi_bitmask;
	 /* the field defines the ongoing eents within MMI. Incase of MMS, this
	  * will indicate the ongoing action */
#define ICF_MMI_CMS_ONGOING				1
#define	ICF_MMI_MMS_DEL_ONGOING			2
#define ICF_MMI_MMS_MOD_ONGOING			3
#define ICF_MMI_MMS_ADD_ONGOING			4
#define ICF_MMI_DMS_ONGOING				5
#define ICF_MMI_ONGOING_NONE			6
	  icf_uint8_t						ongoing_event;

     /* Call ID of the second call which is sent in the merge media request */
      icf_call_id_t						merge_call_id;

	 /* We need to save a list of codecs, that we successfully commit in a call
	  * This structure is a list of commited codecs per stream */
	  icf_stream_id_t					stream_count;
	  icf_codec_for_commit_st			codec_for_commit[ICF_MAX_STREAM_PER_CALL];

     /* This will store the current state of the MMI */
      icf_mmi_state_t					mmi_state;

     /*This stores the current ongoing event */
      icf_mmi_event_t					mmi_event;


	/* ICF rel 6.0: multiple m line */
	/* This node will contain information about the remote SDP. This is a list 
	 * of the type icf_inc_media_sdp_info_st */
	icf_internal_list_st				*p_remote_recvd_sdp;

	/* ICF rel 6.0: multiple m line */
	/* MMI neeeds to store the local address for RTP/app. This information needs
	 * to be stored on a per stream basis. This is a list of the node
	 * icf_local_stream_addr_st */
	icf_list_st						*p_local_stream_addr;
	
	/* MMI needs to buffer the MMS request from CC, as it needs to execute a
	 * sequence of events when the MMS is received from CC typecast as 
	 * icf_mmi_mms_request_st */
	icf_void_t						*p_mms_req;

	/* MMI needs to buffer the CMS and the MMS response towards CC, as they
	 * appear to be atomic to CC, but within MMI, they break into a sequence
	 * of steps. CC awaits the response after the sequence is completed within
	 * MMI and for all the streams. This will either hold (towards CC)
	 * icf_mmi_internal_cms_resp_st or icf_mmi_internal_mms_resp_st */
	icf_void_t						*p_mmi_resp;
	
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    /* This is used to store the no. of nat contexts and the mapping 
     * of each context id with the stream id.
     */
    icf_uint8_t                     no_of_nat_ctxt;
    icf_nat_context_info_st         context_info[ICF_MAX_STREAM_PER_CALL];
    /* This is used to store the local address corresponding to each 
     * stream. This will be used to send the local address to MM.
     */
    icf_uint8_t                     nat_stream_count;
    icf_uint8_t                     restore_count;
    icf_nat_stream_info_st	    nat_streams[ICF_MAX_STREAM_PER_CALL];

#endif
	/* This count maintains the no of streams that are being sent in an API to MM
	 * if the response from MM does not contain a total no of accepted+rejcted 
	 * streams equal to the no of streams that were sent in the request then
	 * MMI send call clear ind to CC */
	icf_stream_id_t					api_stream_count;
	icf_stream_id_t					api_stream[ICF_MAX_STREAM_PER_CALL];


#ifdef ICF_QOS_SUPPORT
	icf_stream_id_t					pdp_count;
	icf_stream_id_t					pdp_stream_id[ICF_MAX_STREAM_PER_CALL];
#endif
	/* Buffer for storing ICF_MEDIA_SESSION_REQ received when mmi is waiting for
	 * MMS response from the Media Manager. after receiving MMS response MMI 
	 * will ICF_MEDIA_SESSION_REQ to MM. this is temporary fix.
	 */
	icf_msg_st						*p_media_session_req;
}icf_mmi_context_st;


ICF_SESSION_CONTROL_START
#ifdef ICF_REL_OLD
/* this structure is not required any longer, as the TBCP stream information
 * will be included along with the other media streams */
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_tbcp_info_st
 * Description :  TBCP stream details
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* id for TBCP stream for this call */
    icf_stream_id_t            stream_id;

    /* local port of TBCP stream for this call */
    icf_uint16_t                local_port;

    /* remote ip/port of TBCP stream for this call */
    icf_transport_address_st remote_addr;

} icf_tbcp_info_st;
#endif


/* ICF rel 6.0 multiple m lines */
typedef struct
{
	/* defines the stream id for the stream. This is the strm
	 * identifier */
	icf_stream_id_t				stream_id;

	/* stores the local address for the stream. This information needs to be
	 * shared with MM. This will not be shared with SSA for forming the SDP
	 */
	icf_transport_address_st	stream_addr;	
}icf_local_stream_addr_st;



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_internal_sdp_stream_st
 *
 * Description: This data type is used to define a structure, which contains the
 *              media session parameters (SDP).
 *
 *---------------------------------------------------------------------------*/

typedef struct
{ 
  /* The field defines the mode of the media stream */
    icf_media_mode_t	media_mode;    
    
  /* The structure stores the information for a media stream within the SDP */
    icf_sdp_stream_st	sdp_stream;    
    
  /* QOS params shall not be shared with MM */	
#ifdef ICF_QOS_SUPPORT
    /* Structures for Qos Support in IMS CLIENT */
    
    /* It contains Qos parameters for Local Send direction */
    icf_qos_st 			local_send;

    /* It contains Qos parameters for Local RECV direction */
    icf_qos_st 			local_recv;

    /* It contains Qos parameters for Remote Send direction */
    icf_qos_st 			remote_send;

    /* It contains Qos parameters for Remote Recv direction */
    icf_qos_st 			remote_recv;
#endif
}icf_internal_sdp_stream_st; 


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_inc_media_sdp_info_st
 *
 * Description: This data type is used to define a structure, which contains a 
 *              list of media session parameters (SDP) for multiple streams.
 *
 *---------------------------------------------------------------------------*/

typedef struct
{
	/* this enum defines the stream status for the streams recvd in the 
	 * incoming SDP */
	/* if an additional stream (besides T.38 is received from the N/w), SSA
	 * needs to se the stream as rejected and hence not populate the SDP
	 * parameters for the stream. If the message in which this additional 
	 * stream was received is an offer, then CC needs to send this back on the
	 * n/w as a rejected stream, but if the additional stream is recvd in the
	 * answer, then CC needs to ignore that stream completely and not use the
	 * stream in subsequent offer/answer */
    /* when the stream is set to active the stream_active boolean is set to
	 * TRUE */

	/* this bit will be set when the received stream in the SDP comes in with
	 * port 0. In this case SDP parameters for the stream will not be populated
	 */
    /* when the stream is set to active the stream_active boolean is set to
	 * FALSE */
	icf_boolean_t				stream_active;
	
    /* denotes the id and type of the stream */
	icf_stream_id_t				stream_id;
	icf_media_type_t			stream_type;

    /* contains parameters for individual stream */
	icf_internal_sdp_stream_st	*p_int_sdp_stream;
}icf_inc_media_sdp_info_st;

ICF_SESSION_CONTROL_END

#ifdef ICF_NW_ACTIVATE
typedef icf_port_pltfm_qos_profile_st					icf_nm_qos_st;

typedef icf_port_pltfm_pkt_filter_list_st			icf_nm_pkt_filter_list_st;

#ifdef IMS_CLIENT
typedef icf_port_pltfm_auth_token_st					icf_nm_auth_token_st;
#endif

typedef icf_port_pltfm_flow_id_st					icf_nm_flow_id_st;			

/*typedef icf_port_pltfm_params_filter_ut				icf_nm_params_filter_ut;*/

/*typedef	icf_port_pltfm_params_filter_list_st			icf_nm_params_filter_list_st;*/

typedef icf_port_pltfm_tft_operation_code			icf_nm_tft_operation_code;

typedef	icf_port_pltfm_tft_st						icf_nm_tft_st;

typedef icf_port_pltfm_pdp_secondary_cntxt_info_st	icf_nm_pdp_secondary_cntxt_info_st;

typedef icf_port_pltfm_pdp_primary_cntxt_info_st		icf_nm_pdp_primary_cntxt_info_st;

typedef icf_port_pltfm_pdp_cntxt_info_st				icf_pdp_cntxt_info_st;

typedef icf_port_pltfm_pdp_prot_config_options_st	icf_pdp_prot_config_options_st;

typedef icf_port_pltfm_pdp_def_req_st				icf_pdp_def_req_st;

typedef	icf_port_pltfm_pdp_def_resp_st				icf_pdp_def_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_pdp_context_st
 * Description      : PDP context(both primay and secondary)for indexing purpose
 *						
 *----------------------------------------------------------------------------*/
typedef struct{
	/*PDP context id*/
	icf_uint32_t		cntxt_id;
	/*If primary PDP context then 0, oterwise denotes the id of primary PDP
	  context, from which this secondary PDP context is derived */
	icf_uint32_t		primay_pdp_id;

	/*Quality of service parameters negotiated with the network*/
	icf_nm_qos_st	qos;
	/*TFT filter*/
	icf_nm_tft_st	tft;

} icf_nm_pdp_context_st;

#endif/*#ifdef ICF_NW_ACTIVATE*/

#ifdef ICF_SESSION_TIMER
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_session_timer_data_st
 * Description :  Contains Session timer related paramters that are relevant 
 *                for a call.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   /* All the following variable will be initialized from line data (if not particularly  
    * specified) if session_timer_enable boolean is TRUE in call ctx. */

   /* session interval value for this call.Value for this */
   icf_duration_t         session_timer;

   /* Minimum session interval for this call */
   icf_duration_t         min_se;

   /* Preffered Session refresher method for call. */
   icf_uint8_t            session_refresher_method;

   /* session timer refresher i.e. whether uac or uas.*/
   icf_refresher_type_t   session_timer_refresher;

   /* This variable will be used for checking whether session refresher needs to be sent 
    * out or not. Value for this boolean will be copied from config data.*/
   icf_boolean_t          session_refresher_reqd;  

   /* This boolean will specify that whether timer logic required is ASB specific or not.    
    * Value for this boolean will be taken from config data.*/
   icf_boolean_t          session_timer_logic;

} icf_session_timer_data_st; /* typedef struct */
#endif


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_call_ctx_st
 * Description :  call context
 *
 *----------------------------------------------------------------------------*/

typedef struct icf_call_ctx_st
{

    /* call identifier */
    icf_call_id_t            call_id;

    /* line identifier on which call is running */
    icf_line_id_t            line_id;

	/* line identifier on which the service needs to be presented In rel 2.0 
     * this would be used for CFW service only */
    icf_line_id_t            service_line_id;

    /* cc fsm state */
    icf_cc_fsm_state_t      call_state;

    /* next state of cc fsm */
    icf_cc_fsm_state_t      next_state;

    /* previous state of cc fsm from where current state is achieved */
    icf_cc_fsm_state_t      prev_state;

    /* valid call id indicate that invite with replace has been received.
     * In this case IP phone will not send indication to PA for call */
    icf_call_id_t           replace_initiated_call_id;

	/* ICF REL 6.0: the SDP structure has been changed to sdp_stream_st in 
	 * the current release */
	/* a list of local SDP, remote SDP, remote commited SDP and last commited
	 * SDP for each stream along with the stream id and stream type will define
	 * a new structure. A list of such nodes will define the complete SDP per 
	 * stream in the call This is a list of the type icf_stream_sdp_info_st 
	 * This will be deleted on getting DMS response in state 7 */
	icf_internal_list_st	*p_stream_sdp_info;

    /* This is filled by SSA in case INVITE has refer by header
     * This is used by CC to fill transferring party name in INCOMING_CALL
     * indication to PA. This will be freed by CC  */
    /* In case of outgoing call created due to reception of REFER from remote
     * party REFER SLP will fill this field with address of party sending
     * refer so that SSA can use it to fill reffered by header in outgoing
     * invite. This will be freed by CC*/

    icf_address_st           *p_referred_party_addr;

    icf_name_st              *p_referred_party_name;

    /* Replaces header received in refer allocated by SSA and freed
     * by CC
     */
    icf_replaces_hdr_st      *p_replaces_header;

    /* Diversion info Allocated by SSA and freed by CC*/
    icf_diversion_info_st    *p_diversion_header_list;
#ifdef ICF_REL_OLD
    /* current media mode Used by SSA to fill the mode in outgoing sdp */
    icf_media_mode_t        media_mode;

    /* This is set by SSA in call ctx in case it sets remote sdp */
    icf_media_mode_t        rem_media_mode;

    /* remote mode which was committed*/
    icf_media_mode_t        rem_committed_mode;
	
	/* local mode which was committed*/
    icf_media_mode_t        local_applied_mode;

/* ICF_HANDLE_REINVITE_START*/

	/* the last committed media mode, to be populated when there 
		is an outgoing call modify request */
    icf_media_mode_t        last_committed_mode;
#endif
/* ICF_HANDLE_REINVITE_END*/

    /* This value is filled by dbm at time of call ctx creattion from
     * configuration data. PA can overwrite this in case of outgoing call */
    icf_qos_mark_t          qos_mark;

    /* remote pary address */
    icf_address_st          remote_party_addr;

    /* remote party name */
    icf_name_st             remote_party_name;

    /* local address. Required only for error and debugging */
    icf_address_st          local_address;

    /* local name. Required only for error and debugging */
    icf_name_st             local_name;


    /* common_bitmask is combination of flag values which are stored in the
       specific bits */
    /* First bit is for toggle presentation indicator - true or false */
    #define ICF_TOGGLE_PI              0x01
    /* Second bit stores call direction - incoming (1) or outgoing (0) */
    #define ICF_CALL_DIR               0x02
    /* Third bit stores media setup mode - fast start (1) or nonfast start(0) */
    #define ICF_CALL_MEDIA_SETUP_TYPE  0x04
    /* This flag is set when 200 OK from Network is received or 200 OK has
     been sent to network */
    #define ICF_CALL_CONNECT_DONE      0x08

    /* This flag is set when remote party name is present */
    #define ICF_REMOTE_NAME_PRESENT    0x10

    /* This flag is set when local party name is present */
    #define ICF_LOCAL_NAME_PRESENT    0x20

    /* This bit is set when privacy header is present in incoming INVITE */
    #define ICF_PRIVACY_HDR_PRESENT   0x40

	/* This bit is set by MMI when RM api succeeds but MM api fails */
	#define ICF_RM_SUCC_MM_FAIL		0x80

    /* This bit is set by SSA when 401/407 with auth param is received from 
     * peer entity. CC will check configured auth param and ask CC to send 
     * SETUP again */ 
    #define ICF_CALL_AUTHENTICATION_REQ 0x0100
    
    /* This bit is set by CC to indicate to SSA that ACK for 401/407 should
     * not result in ssa context clearing */
    #define ICF_CONTINUE_CALL 0x0200

	/* This bit would tell whether remote has indicated support of prack in 
     * invite or not
     */ 
    #define ICF_REL_PROV_RESP_SUPP_BY_PEER 0x0400

/* ICF_HANDLE_REINVITE_START*/
	/* this bit is set for identifying the call modify request direction */
	#define ICF_CALL_MODIFY_DIR		0x0800
	
	/* this bit is set if the call modify result fails */
	#define ICF_CALL_MODIFY_RESP_FAIL  0x1000
/* ICF_HANDLE_REINVITE_END*/

	/* this bit indicates that the incoming call connect contained SDP */
	#define ICF_CALL_CONNECT_WITH_SDP	0x2000

    /* In case Privacy header is sent in outgoing INVITE, this bit indicates 
     * that it should be sent in all BYE, REFER, RE-INVITE requests also */
    #define ICF_PRIVACY_HDR_ALL_REQ     0x4000

	/* This bit indicates whether calling party requires NW 
	 * resource reservation or not
	 */
	#define ICF_NW_RES_RESERV_REQD      0x8000
    
    /* this bit is set if the fax processing results into failure */
    #define ICF_FAX_RESP_FAIL			0x10000
  
    #define ICF_CC_FINAL_SDP_SENT       0x20000

 	/* this bit is set to indicate forked resp */
  	#define ICF_FORKED_RESP			0x40000
  	
	/* this bit is set to indicate GCL request to contain both local and remote codecs */
   	#define ICF_OFFER_CODECS_IN_GCL		0x80000
    #define ICF_IN_DLG_OPTIONS_SEND     0x100000
    #define ICF_IN_DLG_OPTIONS_RECV     0x200000
#define ICF_PRACK_FOR_MEDIA_CONNECT     0x400000
#define ICF_ECHO_CANC_PRESENT                   0x800000    

#define ICF_FAX_CALL_ESTABLISHED		0x01000000

#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
    /* This bit defines that the incoming req-uri contains special parameters
     * like phone-context & local user search should be performed accordingly.
     */
#define ICF_ALTERNATE_ADDR_FORMAT_POSSIBLE 0x2000000
#endif

    /*twc_update handling*/
    /*this bit is set when the call modify request is sent without SDP*/
#define ICF_CALL_MOD_REQ_WO_SDP             0x4000000 

    /*this bitmask shall be present in common_bitmask field if in incoming message, 
     *?úalias??is present Via header */
#define ICF_ALIAS_HEADER_PRESENT            0x8000000 

/* this bit is set if the call modify cancel  processing results into failure */
#define ICF_CALL_MOD_CANCEL_RESP_FAIL       0x10000000

#define ICF_NO_LOCAL_CODEC_IN_GCL           0x20000000
#define ICF_INC_CALL_MOD_WO_SDP             0x40000000
#define ICF_MEDIA_MODE_APPLIED_FROM_APP     0x80000000
    icf_uint32_t             common_bitmask;

/* A bit mask for checking that it is a conference call or not */
#define ICF_CC_EXTERNAL_CONFERENCE_CALL          0x00000001

/* this bimask is used for identifying various flavors of request and 
 * responses in call modify from applicaiton or remote */
#define ICF_CALL_MOD_CFM_PARTIAL_OFFER_REJECT    0x00000002

/*This bit indicates that Cancel is received from application for call modify req*/
#define ICF_CALL_MOD_CANCEL_REQ                  0x00000004

/*This bit indicates that Cancel is received for incoming ReInvite */
#define ICF_CALL_MOD_CANCEL_REINV_REQ            0x00000008

#define ICF_INCOMING_CALL_MOD_CANCEL_REQ         0x00000010
  
/* This bit is set when 491 response is received */
#define  ICF_CC_491_RESP_PROCESS_ONGOING  0x00000020

#define ICF_CC_INCOMING_CONFERENCE_REQ           0x00000040    
/*the bit specifies the presence of P-Early-Media header in the outgoing/incoming 18x response.*/ 
#define  ICF_EARLY_MEDIA_HDR_SUPPORTED           0x00000080

/*the bit specifies the presence of P-Preferred-Identity header in outgoing INVITE*/
#define ICF_PREFERRED_ID_HDR_REQD                0x0000100
#define ICF_NW_INC_CONNECT_ISFOCUS_FOUND         0x0000200
#define ICF_INC_MEDIA_CHANGE_ISFOCUS_FOUND       0x0000400
#define ICF_CC_CLEAR_OPTIONS_CTXT                0x0000800
/* This bit will be set in galre handling for stop_fax_req,
 * when stop_fax_resp is already given to the app. 
 */
#define ICF_STOP_FAX_RESP_ALREADY_GIVEN          0x0001000
#define ICF_NEGOTIATE_PT_PARAM_PRESENT           0x00002000 
#define ICF_CC_BUFFERED_INC_MEDIA_UPDATE         0x00004000
#define ICF_CC_TERM_CALL_ALREADY_SEND_TO_APP     0x00008000

/*  This bit will be set when NOTIFY will be challanged. IT is introduced to
 *  avoid the NOTIFY<-->401/407 Loop.
 */
#define ICF_NOTIFY_AUTHENTICATION_REQ            0X00010000

   icf_uint32_t             common_bitmask_2;
    
    /* this bitmask is used for identifying the reINVITE sent over netwrok
	for which call modify methos. This is a combination of flag values 
       which are stored in the specific bits */
#define ICF_METHOD_CALL_MODIFY_REQ	0x01
#define ICF_METHOD_START_FAX_REQ	0x02
#define ICF_METHOD_STOP_FAX_REQ		0x04
    icf_uint8_t        	     call_modify_method_bitmask;   

/*This bitmask is corresponding to the negotiation of VBD parameter
     *in START_FAX_REQ Api in PT call only
     */
#define ICF_NEGOTIATE_PARAM_SILEN_SUPP    0x01
#define ICF_NEGOTIATE_PARAM_VBD           0x02  
#define ICF_NEGOTIATE_PARAM_ECHO_CANC     0x04
    icf_uint8_t              pt_param;


    /* list of timers currently running. can be - setup timer, alert timer,
       media timer (started by calling module), release timer  */
    icf_cc_timer_list_st        timer_list;

	icf_timer_id_t              info_timer_id;

	icf_timer_id_t              info_cfm_timer_id;         

	icf_timer_id_t              retry_after_timer_id;        

    /* specify interface which initiated release - PA, Network, media, service,
       internal or timer expiry */
    icf_uint8_t                 release_type;

    /* cause for call release */
#define ICF_REL_CAUSE_INVALID                0
#define ICF_REL_CAUSE_SERV_INT_ERR           1  /* corresponds to SIP 500 */
#define ICF_REL_CAUSE_USER_BUSY              2  /* corresponde to SIP 486 */
#define ICF_REL_CAUSE_REQUEST_TERMINATED     3  /* corresponds to SIP 487 */
#define ICF_CC_REL_CAUSE_MM_ERROR_IND        4
#define ICF_REL_CAUSE_USER_NOT_FOUND         5  /* corresponds to SIP 404 */
    icf_uint8_t             release_cause;

    /* PA initiated call clear response code */
    icf_uint16_t            response_code;

   /* Remote initiated call clear response code */
    icf_uint16_t            inc_response_code;

     /*Remote initiated call clear string*/
    icf_uint8_t 	    *p_response_str; 

    /* bitmask for call release status. There is a bit for PA release and
       network release */
    /* first bit is for PA interface status */
#define ICF_CALL_CLEAR_STATUS_PA_CLEARED      0x1
    /* second bit is for network interface status */
#define ICF_CALL_CLEAR_STATUS_NW_CLEARED      0x2
    /* second bit is for MM interface status */
#define ICF_CALL_CLEAR_STATUS_MM_CLEARED      0x4
    /* third bit is for call context cleared. This would mend we have cleared up
     * the data in the call context structure */
#define ICF_CALL_CLEAR_STATUS_CC_CLEARED      0x8

 

    icf_uint8_t             call_clear_status;

	/* 
     * This flag would mark the status for the provisional responses received
     */ 
#define ICF_ALERT_RECEIVED					0x1
#define ICF_MEDIA_PARAM_RECEIVED			0x2
#define ICF_CALL_MODIFY_PROV_RESP_RECVD		0x4
    icf_uint8_t             prov_resp_status;

    /* flag set when media create/modify/delete is ongoing */
    icf_uint8_t             media_ongoing;

    /* flag set when a sip transaction is ongoing */
    icf_uint8_t             signalling_ongoing;

    /* bitmask for events which are buffered waiting to be processed */
    /* first bit is for alert with sdp */
#define ICF_PENDING_EVENT_ALERT_PROV_RESP_WITH_SDP        0x0001
    /* second bit is for media commit */
#define ICF_PENDING_EVENT_MEDIA_PROV_RESP_WITH_SDP        0x0002
/* for forking/multiple_answer */
#define ICF_PENDING_EVENT_FMA_SSA_MEDIA_PARAM			  0x0004
/* for forking/multiple_answer */
#define ICF_PENDING_EVENT_FMA_SSA_ALERT_WITH_MEDIA        0x0008
    /* sixth bit is for setup reject */
#define ICF_PENDING_EVENT_SETUP_REJ                       0x0010
     /* seventh bit is for connect from SSA */
#define ICF_PENDING_EVENT_SSA_CONNECT                     0x0020
    /* eighth bit is for waiting for start process signaling event for sending
       BYE on network */
#define ICF_PENDING_EVENT_WAIT_PROC_SIGNALING_FOR_BYE      0x0040
    /* ninth bit is for waiting for start process media event for sending modify
       media request to MM */
#define ICF_PENDING_EVENT_WAIT_PROC_MEDIA_FOR_MODIFY_MEDIA 0x0080
    /* tenth bit is for local user answered */
#define ICF_PENDING_EVENT_PA_CALL_CONNECT                  0x0100
    /* eleventh bit is for media error indication */
#define ICF_PENDING_EVENT_MM_ERR_IND                       0x0200
    /* twelvth bit is for call terminate request from PA */
#define ICF_PENDING_EVENT_PA_CALL_TERM_REQ                 0x0400
    /* thirteenth bit is for offer change request from peer */
#define ICF_PENDING_EVENT_OFFER_CHANGE_REQ                 0x0800
    /* fourteenth bit is for internal error */
#define ICF_PENDING_EVENT_INT_ERR                          0x1000

    /* fifteenth bit is for SDP in CONNECT ACK  */
#define ICF_PENDING_EVENT_SSA_CALL_CONNECT_ACK             0x2000

/* ICF_HANDLE_REINVITE_START*/
   /* the next bit is for call modify request from local user */
#define ICF_PENDING_EVENT_PA_CALL_MODIFY_REQ               0x4000

/* ICF_HANDLE_REINVITE_END*/
#define ICF_PENDING_EVENT_SSA_FAX_REQ					   0x8000
/* for forking/multiple_answer */
#define ICF_PENDING_EVENT_FMA_SSA_CONNECT                  0x10000
/*unused*/
#define ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ                        0x20000
#define ICF_PENDING_EVENT_PA_ALERT_WO_MEDIA_REQ                      0x40000
#define ICF_PENDING_EVENT_PA_ALERT_WITH_MEDIA_REQ                    0x80000
#define ICF_PENDING_EVENT_PA_STOP_FAX_REQ                    0x01000000
#define ICF_PENDING_EVENT_SSA_STOP_FAX_REQ                   0x02000000
/* Handling REINVITE/UPDATE without SDP*/
#define ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ 	   0x04000000

/*This pending event bitmask is set when already any event is ongoing 
 *(e.g. call modify request) and call modify cancel request comes from 
 *the phone application*/
#define ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ       0x08000000
/*This pending event bitmask is set when already any event is ongoing 
 *(e.g. incoming call modify indication) and call modify cancel request 
 *comes from the network*/
#define ICF_PENDING_EVENT_SSA_CALL_MOD_CANCEL_REQ       0x10000000
#define ICF_PENDING_EVENT_PA_MEDIA_CONNECT_REQ_WO_SDP   0x20000000

    icf_uint32_t            pending_event;

    /* specifies event which is being processed */
    icf_uint8_t             ongoing_event;

    /* pointer to sip stack call context */
    icf_void_t              *p_sip_ctx;

    /* Following pointer will point to ssa_ctxt that will be created while Outgoing call in ssa, 
     * it will be used only while sending CANCEL req to the nw. */
    icf_void_t              *p_initial_sip_ctx;

    /* pointer to timer data in call context */
    icf_void_t              *p_timer_data;
	
	 /* pointer to info timer data */
    icf_void_t              *p_info_timer_data;   

    /* This pointer will be maintaining timer data corresponding to the timer that 
     * will be started when info_ind is given to application and will be stopped 
     * when info_cfm from application will be received.
     */

    icf_void_t              *p_info_cfm_timer_data;

    /* This pointer will be maintaining timer data corresponding to the timer that
     * will be started when retry_after(response code 491) is received.
     */
 
    icf_void_t              *p_retry_after_timer_data;

    /* pointer to service context*/
    icf_void_t              *p_service_ctx; /* TODO: Change to service con */

    /* flag that specifies whether media has been updated or not -true/false */
    icf_uint8_t             media_updated;

    /*Flag that specifies whether media has been committed or not -true/false*/
    icf_boolean_t           media_committed;

    /* bitmask that contains bits for current media state */
    /* media state is active when bitmask is 0 */
#define ICF_MEDIA_STATE_ACTIVE       0x00
    /* first bit is for call mute/unmute */
#define ICF_MEDIA_STATE_CALL_MUTE    0x01
    /* second bit is for local hold */
#define ICF_MEDIA_STATE_LOCAL_HELD   0x02
    /* third bit is for remote hold */
#define ICF_MEDIA_STATE_REMOTE_HELD  0x04

    icf_uint8_t             media_state;

    /*call_state indicates whether call is currently
      locally held or active*/
#define ICF_CALL_STATUS_ACTIVE        0x01
#define ICF_CALL_STATUS_LOCAL_HELD    0x02
#define ICF_CALL_STATUS_CONFERENCE    0x04

    icf_uint8_t            call_status;

    icf_uint8_t            allowed_value;
/*This indicates when UPDATE is present in allow header*/
#define ICF_UPDATE_PRESENT_IN_ALLOW_HDR  1

#ifdef ICF_REL_OLD
    /* contains the media mode that needs to be applied to MMI */
	/* this is no longer required here, as the mode will now be 
	 * set on a per stream basis */
    /*icf_media_mode_t         media_mode_to_be_applied;*/
#endif
    /* THis flag is req to identify if remote hold/resume ind need to be
     * send to PA on MMS response */
/*    icf_boolean_t            remote_mode_change_ind;*/

    /* flag that specifies whether inband-info present or not - true or false */
    icf_uint8_t             inband_info_present;
   
    /* the originator (calling party) identity is asserted/verified by proxy
     * This flag will be set by SSA in case p_assert was present in invite
     * else this will be set to 0*/
    icf_boolean_t           remote_aserted_address;

    /* THis flag is set by CC in case it receives call_wait as internal event*/
    icf_boolean_t           call_waiting_call;

    /* The mmi context for the call is stored in the following field.
     * It is not made a pointer variable, as its size is not big
     * enough. */
    icf_mmi_context_st      mmi_context;

    /* This flag would tell if there was a pending info response */
    icf_boolean_t           pending_info_resp;

    /* This flag would tell if info confirmation from application is pending.*/
    icf_boolean_t           pending_info_cfm;

    /* This will be containing info_cfm failure reason, whether it is due to 
     * timer expiry or it is because some info_cfm is already pending.*/ 
    icf_error_t             info_failure_cause;

    /* 
     * This field would be filled in by SSA on receiving 200 OK resonse for 
     * invite. This would contain the contact address received in 200 OK 
     * response
     */
    icf_address_st         *p_contact_address;

#ifdef ICF_QOS_SUPPORT
    /* This is used to indicate the percentage of total bandwidth allocated
     * to active data senders. This value is in percent.*/
    icf_uint8_t             percent_alloc_to_rs;
    
    /* This is used to indicate the percentage of total bandwidth allocated
     * to data receivers. This value is in percent.*/
    icf_uint8_t             percent_alloc_to_rr;

	
	/* This flag is used for Qos related processing.
     * This flag is SET to ICF_TRUE on reception of successful response
     * to PRACK. In case INVITE transaction is not completed before sending
     * any UPDATE request this flag needs to be checked and if it is TRUE
     * only then an UPDATE request can be initiated
     */
    icf_boolean_t           received_successful_prack_resp;

	/* This flag is set when resource reservation is complete on both 
     * local and remote network.
     */
    icf_boolean_t           resource_reservation_completed;
#endif

ICF_SESSION_CONTROL_START
    /* unique application identifier that is sent/received in ext msg header */
    icf_app_id_t                app_id;

	/* ICF rel 6.0 Multiple m-lines this is bitmap */ 
    /* indicates the call type used by the application for this call */
    icf_call_type_t            call_type;

    /* flag indicating whether privacy header needs to be sent in sip msg */
    icf_boolean_t            send_privacy_hdr;

    /* preferred identity of the local user */
    icf_address_st            *p_preferred_identity;

    /* asserted identity of the local user */
    icf_address_st            *p_asserted_identity;

    /* called party address */
    icf_address_st            *p_called_party_id;
#ifdef ICF_REL_OLD
    /* TBCP stream details for this call */
	/* this information will be set in the stream list along with other streams
	 * in the media params list.This information will not be set individually */
    /*icf_tbcp_info_st            tbcp_info;*/
#endif
    /* list of additional message headers */
    icf_header_list_st        header_list;
    
    /* list of additional message tags */
    icf_tag_list_st            tag_list;

    /* list of additional message bodies */
    icf_msg_body_list_st        body_list;

	/* list of additional headers - this will be used as of now for Re-Invite 
     * or UPDATE sent for Session Refresh */
    icf_header_list_st        persistent_header_list;

#ifdef ICF_REL_OLD
	/* this information will be stored on a per stream basis within ICF */
    /* list of additional streams - In first release of ICF, it is assumed that
     * this list will have only one element containing the TBCP stream */
    icf_stream_list_st        stream_list;

	/* list of additional streams - In first release of ICF, it is assumed that
     * this list will have only one element containing the TBCP stream */
	/* this will be used as of now for Re-Invite sent for Session Refresh */
    icf_stream_list_st        persistent_stream_list;
#endif
	
#ifdef ICF_QOS_SUPPORT
#ifdef IMS_CLIENT
#ifdef ICF_NW_ACTIVATE
    icf_nm_auth_token_st     *p_auth_token;
#endif
#endif
#endif	

ICF_SESSION_CONTROL_END

    icf_boolean_t            inband_dtmf_supported;
#ifdef ICF_FAX_SUPPORT
    /* SCM to check this flag if fax type is set, this flag is
     * set, then no service API should be handled from REMOTE or APPlication
     * CC should initalize this flag to 0 */
    icf_fax_start_type_t     fax_type;
   
	/* This variable will store the pref_fax_codec */
	icf_fax_pt_codec_t       pref_codec;

#endif
	/* This variable will be initalized at creation time 
	 * CC will copy this from create_call request and 
	 * this will be freed at time of call clear */
	icf_list_st               *p_pref_media_profile;

	/* Count of rejected streams received in Connect Req from App */
	icf_uint8_t               rejected_stream_count;

	icf_stream_list_st        rejected_stream_list[ICF_MAX_STREAM_PER_CALL];                      

 /* This will be initalized to 0 at create call time.
     * When analysing MMS_RESP, on basis of ongoing event this
     * will be incremeneted
     */
    
    icf_boolean_t              sdp_changed; 	
    /*Pointer of other call context involved in conference*/
    struct icf_call_ctx_st  *p_conf_other_call_ctx;

    /*This flag is set if UPDATE neds to be sent to remote*/
    icf_boolean_t           update_reqd;

    /* This would tell what error reason is to be send to SSA for update error
     * in icf_nw_otg_media_change_resp_st
     */
    icf_error_t                 offer_failure_reason;

        /* This would tell what error reason is to be send to PA
     */
    icf_error_t                 app_failure_cause;

    /* Parameter is set if forcefull call clear is already received for
   	 * this call in connected state */
   	icf_boolean_t     force_clr_rcvd;

	/* This parameter indicates that a forked response has come without SDP,
	In this case though media_committed may be true, still no answer has been
	received from desired destination, call should be cleared if no SDP is 
	received until final response */
   	icf_boolean_t     no_remote_sdp;

#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
    icf_void_t    *p_load_timer_data;
    icf_timer_id_t  load_call_dur_timer_id;  
#endif

     /*This will give the value of Echo cancellation*/
         icf_boolean_t    echo_canc;

	/* this boolean is whether stop fax response is received */
	icf_boolean_t     stop_fax_resp_recvd;
#ifdef ICF_REQ_URI_CALLED_PARTY_ADDR_PARSING
    /* This parameter contains the called party address for an incoming call,
     * that has been reformed based on certain parameters in received
     * req-uri. Local User search should be performed for this address.
     */
    icf_address_st      alternate_local_address;
#endif

#ifdef ICF_TRACE_ENABLE
    icf_trace_criteria_data_st    *p_trace_criteria_data;

    /* following is the linked list of type icf_stream_data_st which
        * has information on per stream basis */
        icf_list_st                             *p_stream_data;
#endif

/*#ifdef ICF_LOAD_FORCEFUL_CALL_CLEAR
    icf_void_t    *p_load_timer_data;
    icf_timer_id_t  load_call_dur_timer_id;
#endif*/

    /* new variable to maintain counts for forked responses */
    icf_uint8_t forked_resp_counter;
    /* External conference URI  received in Contact with isfocus paramter, filled by SSA*/
    icf_address_st  conference_uri;

    /*This is call-ID for the dialog present in Join header populated by SSA*/
    icf_call_id_t  call_to_be_joined;

    /*This field will be populated by SSA if retry-after header is received from 
     *remote in any failure response and module which is sending failure response 
     *to application will mark it NULL after populating this duration in corresponding 
     *API payload*/
    icf_duration_t   retry_after_duration;
    
    /*
     * What is the method being used for FAX calls.
     * Was the fax call started by modiffying the existing stream or by
     * adding a new stream. 
     */
     icf_uint8_t fax_method;
#ifdef ICF_SESSION_TIMER

   /* This variable will be used  for getting session timer status for this call. */ 
   icf_boolean_t                  session_timer_enable;

   /* Following pointer will contain session  timer data values for this call.
    * This pointer will be assigned memory only if session_timer_enable boolean is 
    * TRUE. Fields in this structure will be filled during call setups whether it is  
    * outgoing or incoming.*/
   icf_session_timer_data_st      *p_session_timer_data;

#endif
    /* This mode is used to take the backup of the mode received in initial
     * INIVITE. IPTK shall send the mode in 200 OK response based on this 
     * mode
     */	
    icf_media_mode_t         recvd_media_attrib_backup;
	
} icf_call_ctx_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_call_ctx_list_st
 * Description :  List of call context present on one line
 *
 *----------------------------------------------------------------------------*/
typedef struct icf_call_ctx_list_s
{
    icf_call_ctx_st              *p_call_ctx;

    struct icf_call_ctx_list_s   *p_call_ctx_next;
} icf_call_ctx_list_st; /* typedef struct */


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_stream_sdp_info_st
 * Description :  This structure defines the set of possible media sets for a 
 *				  session on a per stream basis. all the fields in this struct
 *				  are mandatory. Incase of otg/non-FS call, without any 
 *				  media_profile or pref_media_profile, CC will populate the 
 *				  stream_id =1 and stream_type = AUDIO. In this case, after 
 *				  getting the GCL response, MMI will allocate memory for the 
 *				  local_sdp. Incase of FS incoming, MMI shall populate the 
 *				  local_sdp adn allocate memory. Incase of otg, with either
 *				  media_profile or pref_media_profile, CC will allocate mem.
 *				  for local_sdp before sending CMS to MMI.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	/* If this bit is set, there is media stream existing
	 * for this stream */
#define ICF_MEDIA_STREAM_ACTIVE		0x01
	/* SSA needs to check this bit to see if it should fill this stream
	 * related information in sdp in outgoing SIP message	 */
#define ICF_MEDIA_STREAM_NW_ALLOW	0x02
	/* This bit is filled to check if this stream needs to be
	 * indicated to APPLICATION */
#define ICF_MEDIA_STREAM_PRIVATE	0x04
        
	/* This bit is filled by CC to indicate to SSA that stream is active
	 * but in answer SDP we need to fill the port as 0 to indicate that
	 * mid call offer for this stream has been rejected
	 * SSA needs to reset this bit after sending answer 
	 * CC needs to set this bit only if answer is 200 OK and not 4xx */
#define ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT 0x08
	
	/* This bit is filled by CC to indicate to Application that stream
	 * is active but CALL_MOD_REQ for this stream has failed
	 * CC needs to set this bit at two places:
	 * 1. On getting MMS failure for local change
	 * 2. On getting port as 0 in remote Answer 
	 * This bit will be reset after sending response to PA 
	 */
#define ICF_MEDIA_STREAM_APP_MID_CALL_REQ_REJECT 0x10

    /* To be used for sending reINVITE or receiving the reinvite answer
     * For sending reinvite, if it is set, then send c-line as 0 for the 
     * paritcular stream.
     * For response, if audio stream has c-line 0 then set in the
     * stream status in SSA 
     */

#define ICF_MEDIA_STREAM_CLINE_ZERO              0x20

#define ICF_MEDIA_STREAM_REMOTE_HOLD             0x40

	icf_uint8_t	         		stream_status;
	
	icf_stream_id_t				stream_id;
	icf_media_type_t			stream_type;

	/* local session descriptor parameters */
    /* SSA will use mode from this to form outgoing SDP */
    icf_internal_sdp_stream_st	*p_local_sdp;

    /* remote session descriptor parameters which are committed */
    /* This is the SDP currently committed to MM */
    icf_internal_sdp_stream_st	*p_applied_sdp;

    /* remote session descriptor parameters which are received but not committed
     * yet */
    /* SSA will fill mode in this from incoming SDP */
    icf_internal_sdp_stream_st	*p_remote_received_sdp;

    /* this will be a list of type icf_forked_resp_data_st which has information
      of SDP and identifier for the UAS */
     icf_list_st        *p_forked_resp_list;

	/* the last committed SDP, to be populated when there is an 
	 * incoming call modify request */
    /* This is the SDP last committed to MM */
    icf_internal_sdp_stream_st  *p_last_commited_sdp;

	/* Addition corresponding to spr 14476 fix.*/
    /* Following pointer will contain deep copy of the offer, which 
     * was sent in the initial INVITE. Basically, this is being added
     * here to access the list of codecs which was sent as offer in 
     * INVITE corresponding to this stream. This will be deep copied      
     * while sendinf INVITE and this memory will be freed as soon as 
     * call connect is done.
     */
    icf_internal_sdp_stream_st    *p_offered_sdp;

	/* This is mode sdecified by application.
	 * Assumption:
	 * PA_CONNECT: application_mode = SEND_RECV
     * PA_CREATE_CALL: APPLICATION_MODE = RECV_ONLY
     * SSA_CALL_SETUP:   APPLICATION_MODE = SENDRECV;
     * CALL_HOLD:      APPLICATION_MODE = INACTIVE;
     * CALL_RESUME:    APPLICATION_MODE = SENDRECV;
	 */
   
	icf_media_mode_t           application_mode;

	/* SPR 13875 : variable to take the backup of any mode
	  right now added for local mode */
	icf_media_mode_t	   backup_mode;

/*#define 	ICF_MODE_CHANGE					0x001*/
	/* (OTG/INC Re-INVITE for Mode Change)+ (Answer for OTG INVITE)+
 	 * (INC INVITE call-setup) */
/*#define 	ICF_REMOTE_SDP_MEDIA_CHANGE			0x002*/
 	/* (INC Re-INVITE/UPDATE for SDP change) */
/*#define 	ICF_LOCAL_SDP_MEDIA_CHANGE			0x004*/
	/* (OTG Re-INVITE for SDP change)*/
/*#define 	ICF_REMOTE_SDP_CODEC_CHANGE			0x008*/
	/* (INC Re-INVITE for CODEC change) */
/*#define 	ICF_LOCAL_SDP_CODEC_CHANGE			0x010*/
	/* (OTG Re-INVITE for CODEC change) */
/*#define 	ICF_REMOTE_ANSWER_CODEC_CHANGE		0x020*/
	/* "(Answer for OTG INVITE)+ 
	 * (Answer for OTG Re-INVITE. Offer ACC by peer)" */
/*#define 	ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE	0x040*/
	/* (Answer for OTG Re-INVITE. Offer REJ by peer) */
/*#define 	ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE	0x080*/
	/* (Answer for OTG re-INVITE with only sdp changed in reINVITE) */
/*#define 	ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE	0x100*/
	/* Application return reject for call modify ind due to 
	 * ICF_REMOTE_SDP_MEDIA_CHANGE */
/*#define 	ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE	0x200*/

	/* MMS is required to add this stream */
#define     ICF_STREAM_ADD                         0x400
    /* Processing is being done to delete this stream */
#define     ICF_STREAM_DELETE                      0x800
/* Processing is being done to mark the delete in case of multiple 
       answer is ON */
#define     ICF_STREAM_PENDING_DELETE                      0x1000 
#define     ICF_MEDIA_REVERT_NULL                          0x2000 

	/* This bitmask indicates if add, mod(and type of modify) or 
	 * del is being done for this stream*/
	icf_uint16_t                       media_modify_type;	

	/* this value is the mode applied with MM. This is
	 * not the same mode as set in the SDP sent on the n/w */
    icf_media_mode_t			mm_applied_mode; 
}icf_stream_sdp_info_st;


/*----------------------------------------------------------------------------
 *
 *  icf_transport_mode_et These values specifies the mode of
 *   transport as TCP or UDP or both
 *
 *--------------------------------------------------------------------------*/
typedef icf_uint32_t icf_transport_mode_et;


#ifdef ICF_IPSEC_ENABLED
/*------------------------------------------------------------------------------
 * Structures needed for IPSec Support in ICF Release 4.0
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * Type Name   :  ippstk_ipsec_config_params_st
 * Description :  IPSec configuration parameters 
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
    /* limit up to which successive transaction timeouts are acceptable */
    icf_uint8_t    	            transaction_threshold;
    
    /* lower limit of available range of protected ports */
    icf_uint16_t    	            start_port_val;
    
    /* upper limit of available range of protected ports */
    icf_uint16_t    	            end_port_val;
    
    /* duration of SA Create Timer */
    icf_duration_t		        sa_create_timer;
    
    /* duration of SA Modify Timer */
    icf_duration_t		        sa_modify_timer;
    
    /* duration of SA Delete Timer */
    icf_duration_t		        sa_delete_timer;

} icf_ipsec_config_params_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_ipsec_security_params_st
 * Description :  Key security association parameters that are generated locally
 *                at UE and P-CSCF
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* protected server port */
    icf_uint16_t                 port_s;
    
    /* protected client port */
    icf_uint16_t                 port_c;
    
    /* spi of inbound SA at protected server port */
    icf_uint32_t                 spi_s;
    
    /* spi of inbound SA at protected client port */
    icf_uint32_t                 spi_c;
    
} icf_ipsec_security_params_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_ipsec_sa_context_st
 * Description :  Context of each security association in ICF
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* integrity algorithm returned by P-CSCF and used by the SA */
    icf_uint8_t                  *p_integrity_algo;
    
    /* encryption algorithm returned by P-CSCF and used by the SA */
    icf_uint8_t                  *p_encryption_algo;
    
    /* security mechanism preference */
    icf_uint8_t                  *p_q_value;
    
    /* pointer to the SA parameters generated locally by the UE */
    icf_ipsec_security_params_st *p_ue_security_params;
    
    /* pointer to the SA parameters received from the P-CSCF */
    icf_ipsec_security_params_st *p_proxy_security_params;
    
	/* duration for which the SA is active - 
	 * If SA Lifetime Modification is supported, this value is in milliseconds,
	 * else this value is (2**32 - 1) seconds */
    icf_duration_t               sa_lifetime;
    
    /* number of transactions currently active on this SA */
    icf_uint8_t                  num_active_trans;
    
    /* flag indicating whether the SA is currently active  or not - this will be
     * initialized to FALSE, set as TRUE on receiving success response for 
     * register, set as FALSE on sending deletion request for this SA */
    icf_boolean_t	            is_active;
    
    /* IP address of the P-CSCF copied from line data */
    icf_transport_addr_st        proxy_ip_addr;
    
} icf_ipsec_sa_context_st;


/* ------------------ End: Structures for IP Security Support --------------- */
#endif /* ICF_IPSEC_ENABLED */

/*-----------------------------------------------------------------------------
*
* NAME : icf_user_db_node_st
*
* DESCRIPTION  : This structure stores a node of the user database
-----------------------------------------------------------------------------*/

typedef struct icf_user_db_node_st
{
    icf_address_st public_user;
    icf_address_list_node_st *p_assoc_uri_list;
    icf_address_list_node_st *p_contact_uri_list;
#define ICF_USER_DB_PUBLIC_NODE_BARRED      0x01
#define ICF_USER_DB_PUBLIC_NODE_TEMPORARY   0x02
#define ICF_USER_DB_AUTH_KEY_RECD           0x04
#define ICF_USER_DB_AUTH_PASSWD_RECD        0x08
#define ICF_USER_DB_AUTH_PRMS_PER_USER      0x10
    icf_uint32_t                flags;
    /* authentication key for this user */
    icf_string_st               authentication_key;

    /* authentication password for this user */
    icf_string_st               authentication_password;
 
    struct icf_user_db_node_st *p_next;
}icf_user_db_node_st;



/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_line_data_st
 * Description :  Line related persistant and dynamic data.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /*------------- General line data ---------------*/
    /* line identifier */
    icf_line_id_t                line_id;

    /* line status whether free or busy - init value is free */
    icf_uint8_t                  line_status;

    /* number of active or held calls on the line - init value is 0 */
    icf_uint8_t                  num_of_calls;

    /* Number of calls in call waiting on this line
     * After it reach threshold Call Forwarding with Busy Line
     * will be invoked.
     */
    
    icf_uint8_t                   num_call_waiting_calls;

#ifdef ICF_SESSION_TIMER
	/* session interval for this line */
	icf_duration_t session_timer;
	/*minimum session interval for this line */
	icf_duration_t min_se;
#endif

    /* list of call context pointers for active calls in the line */
    icf_call_ctx_list_st         *p_call_ctx_list;


    /*----------- Transport data ----------------*/
    /* proxy configured for this line */
    icf_transport_address_st     proxy_addr;

    /* message trasfer mode for this line, UDP or TCP - default mode is both */
    icf_transport_mode_t         transport_mode;
#ifdef ICF_SIGCOMP_SUPPORT
    /* Indicates whether the Proxy (P-CSCF) supports Sigcomp or not*/
    icf_uint8_t                  sigcomp_support_by_proxy;
    /*Values for sigcomp_support  */
#define ICF_PEER_DOES_NOT_SUPPORT_SIGCOMP           0
#define ICF_PEER_SUPPORTS_SIGCOMP                   1
#define ICF_PEER_SUPPORTS_SIGCOMP_IS_UNKNOWN        2
    
#endif

    /*----------- Self identifier data ----------------*/
    /* local user name configured for this line */
    icf_name_st                  user_name;

	/* Pointer to the user database for this line */
    icf_user_db_node_st          *p_user_db;

    /*------------------- Registration data -------------------*/
    /* registrar configured for this line */
    icf_transport_address_st     registrar_addr;

    /* Route/next hop configured for this line */
    icf_transport_address_st     reg_route_addr;

    /* transport mode for registration request */
    icf_transport_mode_t            reg_mode;

    /*
     * registration duration for this line, value 0xffff indicates infinite
     * duration for registration - default value is 600000ms
     */
    icf_duration_t               registration_duration;

    /* authentication key for this line */
    icf_string_st                authentication_key;

    /* authentication password for this line */
    icf_string_st                authentication_password;

	/* realm name for this line */
    icf_string_st                configured_realm;

 /* ------------------Services related data--------------------*/

    icf_uint16_t                  service_subscription_flag;

    icf_uint16_t                  service_activation_flag;

    icf_address_st               addr_for_cfu;
    
    icf_address_st               addr_for_cfb;
    
    icf_address_st               addr_for_cfd;

#if 1 /*ZyXEL , Jason , [RD Internal #231]*/
    /*BL2348_iptk add for noans forward by per-line*/
    icf_duration_t                cnd_alert_timer;
#endif
    
   /*
    * bitmask that identifies whether all the mandatory parameters have been
    * recd once or not - init value is 0x0
    */
#define ICF_CFG_LINE_DATA_PROXY_RECD                     0x1
#define ICF_CFG_LINE_DATA_SELF_ADDR_RECD                 0x2
#define ICF_CFG_LINE_DATA_REGISTRAR_RECD                 0x4
#define ICF_CFG_LINE_DATA_AUTH_KEY_RECD                  0x8
#define ICF_CFG_LINE_DATA_AUTH_PASSWD_RECD               0x10
#define ICF_CFG_LINE_DATA_REALM_RECD                     0x20
#define ICF_CFG_LINE_DATA_REG_ROUTE_ADDR_RECD            0x40
#define ICF_CFG_LINE_DATA_SELF_TRANSPORT_RCVD            0x80 
	icf_uint8_t                   line_data_received;

   /* Following stores the default settings passed in self user configuration
    * API for line specific setup.
    * Currently only ICF_CFG_CC_USER_PRESENTATION_IND and
    * ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO are defined.
    * if ICF_SUPPORT_OUT_OF_DIALOG_NOTIFY_IGNORE_DIALOG_INFO is set then Notify 
    * will be given to application without matching its dialog.  
    * is defined */
   icf_uint8_t                   default_settings;

#ifdef IMS_CLIENT
    /* The following parameters are configured to support AKA algorithm,
       Shall be removed once Algorithm itself is in place */

    /*
     * bitmask that identifies whether all the auth parameters have been
     * recd once or not - init value is 0x0
     */
#define ICF_CFG_LINE_DATA_AUTH_REALM_RECD                     0x01
#define ICF_CFG_LINE_DATA_AUTH_NONCE_RECD                     0x02
#define ICF_CFG_LINE_DATA_AUTH_NC_RECD                         0x04
#define ICF_CFG_LINE_DATA_AUTH_CNONCE_RECD                      0x08
#define ICF_CFG_LINE_DATA_AUTH_RESPONSE_RECD					 0x10
#define ICF_CFG_LINE_DATA_AUTH_OPAQUE_RECD                      0x20
#define ICF_CFG_LINE_DATA_AUTH_AUTS_RECD                          0x40
#define ICF_CFG_LINE_DATA_AUTH_URI_RECD							0x80
    icf_uint16_t                  auth_data_received;


    /* Contains the realm */
    icf_string_st                 auth_realm;

    /* Contains the nonce */
    icf_string_st                 auth_nonce;

    /* Contains the nc */
    icf_string_st                 auth_nc;

    /* Contains the cnonce */
    icf_string_st                 auth_cnonce;

    /* Contains the response */
    icf_string_st                 auth_response;

    /* Contains the opaque */
    icf_string_st                 auth_opaque;

    /* Contains the AUTS parameter */
    icf_string_st                 auth_auts;

	/* Contains the URI parameter */
	icf_string_st				auth_uri;
#endif
   /*----------- Self Transport data ----------------*/
    /* Self Transprot Address configured for this line */
    icf_transport_address_st     self_addr;

    /* message trasfer mode for this line, UDP or TCP - default mode is both */
    icf_transport_mode_t         self_mode;

#define ICF_WIN_UDP_SIP_SERVER   0
#define ICF_WIN_TCP_SIP_SERVER   1
#define ICF_WIN_TLS_SIP_SERVER   2

    icf_uint16_t     server_index[ICF_WIN_TLS_SIP_SERVER + 1];

    icf_boolean_t    line_data_init_complete;

	/* Number of seconds prior to expiration of
	 * subscription */
	icf_duration_t				subs_hs_duration;
	
	/* Number of seconds prior to expriation of 
	 * registration */
	icf_duration_t				reg_hs_duration;

	/* SIP T1 Timer duration value in milliseconds*/
	icf_duration_t				sip_timer_t1;

	/* SIP T2 Timer duration value in milliseconds*/
	icf_duration_t				sip_timer_t2;

	/* SIP B Timer duration value in milliseconds*/
	icf_duration_t				sip_timer_invite;

	/* SIP F Timer duration value in milliseconds*/
	icf_duration_t				sip_timer_non_invite; 
	
   /* The Differentiated Services Codepoint (DSCP) value 
   that should populate the DS field of SIP packets sent 
   by the User Agent.*/
    icf_uint8_t                 sip_dscp;

    /* The Differentiated Services Codepoint (DSCP) value 
    that should populate the DS field of RTP packets sent 
    by the User Agent.*/
    icf_uint8_t                 rtp_dscp;

#ifdef ICF_SESSION_TIMER
    /* A value of 'none,' 'uas,' or 'uac' specifying the refresher to be specified by the UA when it has a choice. 
     * A value of 'none' specifies that the UA SHALL NOT request session expiration; any other value specifies that
     * the UA SHALL request session expiration. The default value SHALL be 'uas'*/
     icf_refresher_type_t    session_timer_refresher;

    /* Following variable will be used for keeping session refresher method i.e. 
     * INVITE/UPDATE, if specified 
     * by application otherwise it will be having value as in icf_defs.h.*/
    icf_uint8_t             session_refresher_method;
#endif
    icf_hold_option_t       hold_option; 
    icf_list_st     *p_codec_attributes_audio;
    icf_list_st     *p_codec_attributes_video; 
   
    /* The userid to be set in outgoing Contact headers. If this is empty, then
     * the userid configured through set_self_id will be set in Contact
     */
    icf_string_st	contact_user_info;

    /*  The URI of the Conference Server */
    icf_address_st conf_server_uri;

#ifdef ICF_NAT_RPORT_SUPPORT
   icf_rport_config_data_st     *p_rport_config_data;
#endif

	/* Fix for CSR 1-8047728: SPR 20246 */
	icf_boolean_t               remove_route_hdr;

} icf_line_data_st;


#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_trace_internal_data_st
 * Description :  Trace data stored in global data.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* string in english to be used for printing the traces from packetphone */
    icf_uint8_t         *p_trace_string;

    /* trace level like brief and detailed */
    icf_trace_level_t   trace_level;

    /* trace category like system, interface */
    icf_trace_type_t    trace_type;

    /* to get time stamp */

} icf_trace_internal_data_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_app_trace_data_st
 * Description :  App specific Trace data stored in global data.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* string in english to be used for printing the traces from packetphone */
    icf_uint8_t         *p_trace_string;

    /* trace category like system, interface */
    icf_trace_type_t    trace_type;

} icf_app_trace_data_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_app_trace_info_st
 * Description :  App specific Trace information for call id and app id
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
   /* application id */
    icf_app_id_t        app_id;

    /* call identifier */
      icf_call_id_t            call_id;

} icf_app_trace_info_st;

#endif /* ICF_TRACE_ENABLE */

#ifdef ICF_ERROR_ENABLE
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_error_internal_data_st
 * Description :  Error data stored in global data.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* string in english to be used for printing the error from packetphone */
    icf_uint8_t        *p_error_string;

    /* error level like brief and detailed */
    icf_error_level_t   error_level;

    /* error category like system, interface */
    icf_error_type_t    error_type;

} icf_error_internal_data_st;
#endif /* ICF_ERROR_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_dns_info_st
 * Description :  Contains the timer and the pointer to the DNS list
 *
 *--------------------------------------------------------------------------*/
typedef struct
{

     /* This will hold the list of DNS buffer   
     */
    icf_list_st  		*p_dns_buffer_list;

    /*
     * This will hold the timer id of DNS buffer timer  
     */
    icf_timer_id_t  		dns_buffer_timer_id;

    /*
     * This will hold the timer data of DNS buffer timer  
     */
    icf_void_t			*p_dns_timer_data;
}icf_dns_info_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_glb_data_st
 * Description :  Structure containing all global data which is of importance to *                all modules.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
#ifdef ICF_UNIT_TEST
    /* all configuration data */
    icf_config_data_st  config_data;
#endif

    /* This would be a runtime allocated array of pointers which would hold
     * address of line configurations block for each line availble */
    icf_line_data_st    **p_p_line_data;

#ifdef ICF_TRACE_ENABLE
    icf_trace_internal_data_st   trace_data[ICF_MAX_NUM_OF_TRACE_IDS];
    icf_app_trace_data_st       app_trace_data[ICF_MAX_APP_TRACE_IDS];
    icf_app_trace_info_st       app_trace_info;
#endif

#ifdef ICF_ERROR_ENABLE
    icf_error_internal_data_st  error_data[ICF_MAX_NUM_OF_ERROR_IDS];
#endif

    /*Points to a block which would contains various stats data*/
    icf_void_t *p_stats_data;

    /*Would contain data related to module for all relevant modules*/
    icf_void_t *p_module_data[ICF_INT_MAX_MODULE];

    /*
    * flag that is set when all mandatory parameters have been recd once -
    * init value is ICF_FALSE
    */
    icf_boolean_t                global_data_init_complete;

    /* CHR module global data*/
    icf_void_t *p_p_chr_data;

    /*
     * The memory for this would be allocated by the port_init in port
     */
    icf_void_t  	*p_glb_port_info;
    icf_dns_info_st	*p_dns_info;
    /* added to check whether to send responses to application or not
     * in case when restart or reinitialize is received from 
     * application
     * ICF_FALSE means responses should be sent 
     * ICF_TRUE means responses should not be sent as restart or reinit has been
     * received*/
     icf_boolean_t      recvd_restart_reinit_event;


} icf_glb_data_st;


#ifdef ICF_LOAD_STAT
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_load_stat_data_st
 * Description :  
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    icf_uint32_t    otg_call_attempts;
    icf_uint32_t    otg_call_connects;
    icf_uint32_t    inc_call_attempts;
    icf_uint32_t    inc_early_call_attempts;
    icf_uint32_t    inc_call_connects;
    icf_uint32_t   otg_call_term_req;
    icf_uint32_t    term_call_ind_bye;
    icf_uint32_t    term_call_ind_404;
    icf_uint32_t    term_call_ind_486;
    icf_uint32_t    term_call_ind_default;
    icf_uint32_t    call_hold_attempts;
    icf_uint32_t    call_hold_success;
    icf_uint32_t    call_resume_attempts;
    icf_uint32_t    call_resume_success;
    icf_uint32_t    options_attempts;
    icf_uint32_t    options_success;
    icf_uint32_t    otg_options_attempts;
    icf_uint32_t    inc_options_attempts;
    icf_uint32_t    otg_options_success;
    icf_uint32_t    otg_options_failure;
    icf_uint32_t    inc_options_failure_wo_app;
    icf_uint32_t    inc_options_success;
    icf_uint32_t    inc_options_failure;
    icf_uint32_t    options_rcvd_from_app;
    icf_uint32_t    info_attempts;
    icf_uint32_t    info_success;
    icf_uint32_t    time_new;
    icf_uint32_t    time_old;
    icf_uint32_t    init_time;
    icf_uint32_t    register_req;
    icf_uint32_t    register_refresh;
    icf_uint32_t    register_cfm;
    icf_uint32_t    register_failure;
    icf_uint32_t    subscribe_req;
    icf_uint32_t    subscribe_cfm;
    icf_uint32_t    subscribe_fail;
    icf_uint32_t    subscribe_refresh;
    icf_uint32_t    load_force_cleared_calls;
} icf_load_stat_data_st;


#endif

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_glb_pdb_st
 * Description :  global process data block
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* call context pointer */
    icf_call_ctx_st  *p_call_ctx;

    /* pointer to message received from external interface */
    icf_msg_st           *p_recv_msg;

    /* pointer to global data */
    icf_glb_data_st      *p_glb_data;

    /* This keeps tracks of the list of modules travered in single
     * flow. It stores the stack history of the modules. */
    /* This Array is being filled from 1 to ICF_MAX_MODULE_TVSL
     * so Array sixe taken as ICF_MAX_MODULE_TVSL+1
     */
    icf_int_module_id_t  module_hist[ICF_MAX_MODULE_TVSL+1];

    /* The current index of the module histrory which is active. */
    icf_uint8_t          curr_module_index;

    /* When porting returns more than one messages, then it is
     * appended to this list. This is part of glb pdb, so that
     * when timer is started this can be passed and later at expiry
     * the message can be stored at this location. */
    icf_msg_list_st      *p_msg_lst;

    /* System resource limit configuration */
    icf_uint8_t          sys_resource_limit;

#ifdef ICF_PORT_WINCE
	void			*connection_handle;

	/* ICF log redirection parameter */
    icf_uint8_t         logs_indication;

#endif

#ifdef ICF_TRACE_ENABLE
	/* Print info for Traces */
	icf_print_info_st	 trace_info;
        icf_uint8_t      *p_interface_trace_info;
        icf_app_id_t                app_id;
#endif

#ifdef ICF_ERROR_ENABLE
	/* Print info for Errors */
	icf_print_info_st	 error_info;
#endif

#ifdef ICF_LOAD_STAT
    icf_load_stat_data_st    load_data;
#endif

#ifdef ICF_STAT_ENABLE
        icf_stats_st         *p_stats_info;
        icf_time_stamp_st     last_reset_time;
#endif
        icf_uint32_t   no_of_call_obj;

} icf_glb_pdb_st;


#if defined (ICF_TRACE_ENABLE) || defined (ICF_ERROR_ENABLE)
extern icf_glb_pdb_st *p_persistent_glb_pdb;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_internal_msg_hdr_st
 * Description :  Header for internal message
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* internal message id */
    icf_uint16_t            msg_id;

    /* length of p_msg_data in internal message. This may be deleted if felt
       lateron that it is not being used */
    icf_uint16_t            payload_length;

    /* global pdb pointer */
    icf_glb_pdb_st          *p_glb_pdb;

    /* return error code */
    icf_error_t             ecode;

    /* return status */
    icf_uint32_t            status;

    /* return status_2 is defined as all the bitmasks for status is already in use */
    icf_uint32_t            status_2;

} icf_internal_msg_hdr_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_internal_msg_st
 * Description :  Msg from any internal module to another internal module
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

    /* header of internal message */
    icf_internal_msg_hdr_st    msg_hdr;

    /* internal message data which will be cast as per msg id */
    icf_uint8_t               *p_msg_data;

} icf_internal_msg_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_cc_ssa_msg_ctx_st
 * Description :  Msg from CC module to SSA module (typecast to p_msg_data)
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    /* common bitmask is combination of flag values which are stored in the
       specific bits */
    /* First bit determines whether provisional response is required for an
       outgoing message or not - true if it is required or false otherwise */
    #define ICF_PRACK_REQ              0x1
    /* Second bit determines whether media info needs to be sent  in an
       outgoing message or not - true if SDP needs to be sent or false
       otherwise */
    #define ICF_MEDIA_INFO_PRESENT     0x2
    icf_uint8_t             common_bitmask;
} icf_cc_ssa_msg_ctx_st;


/*-----------------------------------------------------------------------------
 *
 *  Structure Name : icf_timer_data_st
 *
 *  Description :    Would contain the information that would be passed to other
 *                   module in case of timer expiry
 *
 *---------------------------------------------------------------------------*/
typedef struct
{

    /* This would contain the information regarding the type of timer */
     icf_timer_type_t   timer_type;

    /*This would contain the information regarding the timer id started*/
     icf_timer_id_t     timer_id;

    /* This would contain the id of the module which has raised the error */
    icf_int_module_id_t module_id;

    /* This buffer would contain module specific information, which the module
     * would need to find the call context, registration context etc */
    icf_void_t          *p_timer_buffer;

  /*Pointer to global packet data block*/
    icf_glb_pdb_st      *p_glb_pdb;
} icf_timer_data_st;




#ifdef IMS_CLIENT
/* --------------------------------------------------------------------
 * Type Name   :  icf_subs_ntfy_evnt_et
 * Description :  Values for Notify Indication (notify_ind) Event 
 *               
 * ------------------------------------------------------------------------*/
    typedef enum
    {
    ICF_SUBS_NTFY_EVNT_REJECTED = 0,
    ICF_SUBS_NTFY_EVNT_DEACTIVATED
    }icf_subs_ntfy_evnt_et;

#endif


ICF_INIT_START
/* --------------------------------------------------------------------
 * Type Name   :  icf_conf_line_st
 * Description :  structure for storing line of confifuration file
 *               
 * ------------------------------------------------------------------------*/
typedef struct 
{
  char data[ICF_CONF_MAX_LINE_SIZE];
} icf_conf_line_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_conf_file_st
 * Description :  structure for storing for configuration file & number of lines in it
 *               
 * ------------------------------------------------------------------------*/
typedef struct 
{
  icf_conf_line_st  *line;
  int          count;
} icf_conf_file_st;


ICF_INIT_END

/* --------------------------------------------------------------------
 * Type Name   :  icf_string_node_st
 * Description :  This datatype is used to define a node in a 
 *                linked list of strings
 *               
 * ------------------------------------------------------------------------*/

typedef struct icf_string_node_st
{
  icf_uint8_t 		*p_str;
  struct icf_string_node_st	*p_next;
}icf_string_node_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_filter_expr_st
 * Description :  This datatype is used to define a structure that contains 
 *                the parsed expression, which is specified by the 
 *                application as the expression string in a filter.
 *               
 * ------------------------------------------------------------------------*/

typedef struct
{
  icf_boolean_t		 match_all;
  icf_uint32_t		 count;
  icf_string_node_st *p_match_str_list;
}icf_filter_expr_st;

/* --------------------------------------------------------------------
 * Type Name   :  icf_int_filter_st
 * Description :  This datatype is used to define a structure that 
 *                contains single filter specification in the   
 *                internal filter list structure.
 * ------------------------------------------------------------------------*/

typedef struct icf_int_filter_st
{

  /* Specifies the filter type */
  icf_filter_type_t	filter_type;

  /* Specifies the method to be used while matching the filter */
  icf_match_type_t	match_type;
  /* Name of sip header on which the filter criteria is to be applied */
  icf_string_st		target_header;
  
  /* Specifies the expression, which has to be matched according to the 
     filter criteria */
  icf_filter_expr_st	filter_expr;
  /* Pointer to the next filter node in the list */
  struct icf_int_filter_st       *p_next;
}icf_int_filter_st;


/* --------------------------------------------------------------------
 * Type Name   :  icf_int_filter_list_st
 * Description :  Internal Structure for storing application filter list
 *               
 * ------------------------------------------------------------------------*/

typedef struct 
{
  /* Specfies whether all or any one of the filtering criteria has 
	to be matched */
  icf_boolean_t	match_all;
  /* Number of nodes in the list */
  icf_uint32_t 	count;
  /* Pointer to the List */
  icf_int_filter_st 	*p_filter_list;
} icf_int_filter_list_st;

#if 0
/*-----------------------------------------------------------------------------
 *
 * typedef Name : icf_time_n_date_st
 * 
 *  Description  : This data type is used to define a structure which specifies
 *                 the data and time information.
 *   
 -----------------------------------------------------------------------------*/

typedef struct
{

    icf_uint8_t     seconds;  /* seconds after the minute - [0,61] */
    icf_uint8_t     minutes;  /* minutes after hour - [0,59] */
    icf_uint8_t     hour;     /* hour since midnight - [0,23] */
    icf_uint8_t     date;     /* day of the month - [1,31] */
    icf_uint8_t     month;    /* months after January - [0,11] */
    icf_uint32_t    year;     /* year like 2005 */


} icf_time_n_date_st;
#endif


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
					New Mapping for icf_cfg_api.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/




/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_cfg_cc_set_service_params_req_st 
 *
 * Description: This structure specifies payload data of 
 *              API ICF_SET_SERVICE_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/
#define ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_FLAG  ICF_SET_SER_PRM_SUBSCRIPTION_FLAG
#define ICF_CFG_CC_SET_SER_PRM_ACTIVATION_FLAG    ICF_SET_SER_PRM_ACTIVATION_FLAG
#define ICF_CFG_CC_SET_SER_PRM_GLB_SUBS_FLAG      ICF_SET_SER_PRM_GLB_SUBS_FLAG
#define ICF_CFG_CC_SET_SER_PRM_GLB_ACT_FLAG       ICF_SET_SER_PRM_GLB_ACT_FLAG
#define ICF_CFG_CC_SET_SER_PRM_CALL_HOLD_OPTION   ICF_SET_SER_PRM_CALL_HOLD_OPTION
#define ICF_CFG_CC_SET_SER_PRM_MAX_NUM_OF_HOPS    ICF_SET_SER_PRM_MAX_NUM_OF_HOPS
#define ICF_CFG_CC_SET_SER_PRM_MAX_CALL_WAITING   ICF_SET_SER_PRM_MAX_CALL_WAITING
#define ICF_CFG_CC_SET_SER_PRM_CFU_ADDR           ICF_SET_SER_PRM_CFU_ADDR
#define ICF_CFG_CC_SET_SER_PRM_CFB_ADDR           ICF_SET_SER_PRM_CFB_ADDR
#define ICF_CFG_CC_SET_SER_PRM_CFD_ADDR           ICF_SET_SER_PRM_CFD_ADDR
#define ICF_CFG_CC_SET_SER_PRM_CHR_TIMER          ICF_SET_SER_PRM_CHR_TIMER
#define ICF_CFG_CC_SET_SER_PRM_REINVITE_RACE_TIMER  ICF_SET_SER_PRM_REINVITE_RACE_TIMER
#define ICF_CFG_CC_SET_SER_PRM_TWC_TIMER          ICF_SET_SER_PRM_TWC_TIMER
#define ICF_CFG_CC_SET_SER_PRM_CTU_TIMER          ICF_SET_SER_PRM_CTU_TIMER
#define ICF_CFG_CC_SET_SER_PRM_CTA_TIMER          ICF_SET_SER_PRM_CTA_TIMER
#define ICF_CFG_CC_SET_SER_PRM_RNW_TIMER          ICF_SET_SER_PRM_RNW_TIMER
#define ICF_CFG_CC_SET_SER_PRM_CW_ALERT_TIMER     ICF_SET_SER_PRM_CW_ALERT_TIMER
#define ICF_CFG_CC_SET_SER_PRM_CND_ALERT_TIMER    ICF_SET_SER_PRM_CND_ALERT_TIMER
#define ICF_CFG_CC_SET_SER_PRM_CW_N_CND_ALERT_TIMER  ICF_SET_SER_PRM_CW_N_CND_ALERT_TIMER
#define ICF_CFG_CC_SET_SER_PRM_SUBS_RESP_TIMER    ICF_SET_SER_PRM_SUBS_RESP_TIMER
#define ICF_CFG_CC_SET_SER_PRM_SUBS_RETRY_TIMER   ICF_SET_SER_PRM_SUBS_RETRY_TIMER
#define ICF_CFG_CC_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF ICF_SET_SER_PRM_SUBSCRIPTION_REQD_IN_EXT_CONF
#define ICF_CFG_CC_SET_SER_PRM_CONFERENCE_URI_PRESENT ICF_SET_SER_PRM_CONFERENCE_URI_PRESENT


typedef icf_set_service_params_req_st	icf_cfg_cc_set_service_params_req_st;
/*----------------------------------------------------------------------------
 * NAME : icf_cfg_cc_set_scale_params_req_st
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_CFG_CC_SET_SCALE_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/
typedef icf_set_scale_params_req_st icf_cfg_cc_set_scale_params_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_cfg_cc_set_transport_params_req_st 
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_CFG_CC_SET_TRANSPORT_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/

#define ICF_CFG_CC_SET_TRP_PRM_PROXY_ADDR_LIST         ICF_SET_TRP_REQ_PROXY_ADDR
#define ICF_CFG_CC_SET_TRP_PRM_SLF_TRANSPORT_ADDRESS   ICF_SET_TRP_REQ_SLF_TRANSPORT_ADDRESS
#define ICF_CFG_CC_SET_TRP_PRM_SLF_MODE                ICF_SET_TRP_REQ_SLF_MODE
#define ICF_CFG_CC_SET_TRP_PRM_SLF_PORT                ICF_SET_TRP_REQ_SLF_PORT

typedef icf_set_transport_req_st icf_cfg_cc_set_transport_params_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_cfg_cc_set_call_params_req_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_CFG_CC_SET_CALL_PARAMS_REQ 
 *
 *---------------------------------------------------------------------------*/



#define  ICF_CFG_CC_SET_CALL_PRM_DEFAULT_SETTINGS			ICF_SET_CALL_PRM_DEFAULT_SETTINGS
#define  ICF_CFG_CC_SET_CALL_PRM_QOS_MARK					ICF_SET_CALL_PRM_QOS_MARK
#define  ICF_CFG_CC_SET_CALL_PRM_SETUP_TIMER				ICF_SET_CALL_PRM_SETUP_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_ALERTING_TIMER				ICF_SET_CALL_PRM_ALERTING_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_RELEASE_TIMER				ICF_SET_CALL_PRM_RELEASE_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_MODIFY_MEDIA_TIMER			ICF_SET_CALL_PRM_MODIFY_MEDIA_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_RG_RSP_TIMER				ICF_SET_CALL_PRM_REGISTRATION_RESP_DURATION
#define  ICF_CFG_CC_SET_CALL_PRM_RG_RETRY_TIMER				ICF_SET_CALL_PRM_REGISTRATION_RETRY_DURATION
#define  ICF_CFG_CC_SET_CALL_PRM_INFO_TIMER					ICF_SET_CALL_PRM_INFO_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_PHONE_INFO					ICF_SET_CALL_PRM_PHONE_INFO
#define  ICF_CFG_CC_SET_CALL_PRM_OPTIONS_TIMER				ICF_SET_CALL_PRM_OPTIONS_TIMER
#ifdef ICF_SESSION_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL		ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL
#define  ICF_CFG_CC_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL  ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL
#endif


/*ICF_HANDLE_REINVITE_START*/
#define ICF_CFG_CC_SET_CALL_PRM_CALL_MOD_REQ_METHOD		ICF_SET_CALL_PRM_CALL_MOD_REQ_METHOD
/*ICF_HANDLE_REINVITE_END*/

#define ICF_CFG_CC_CONFIG_CALL_THRU_SIP_PROXY		   ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY
#define ICF_CFG_CC_CONFIG_PRIVACY_HEADER_PRESENT		   ICF_CALL_CONFIG_PRIVACY_HEADER_PRESENT
#define ICF_CFG_CC_CONFIG_PRACK_REQD					   ICF_CALL_CONFIG_PRACK_REQD
#ifdef ICF_NAT_RPORT_SUPPORT
#define  ICF_CFG_CC_SET_CALL_PRM_BINDING_REFRESH_TIMER  	ICF_SET_BINDING_REFRESH_TIMER
#endif

#define ICF_CFG_CC_SET_CALL_PRM_DNS_MIN_TIMER     ICF_SET_CALL_PRM_DNS_MIN_TIMER
#define ICF_CFG_CC_SET_CALL_PRM_DNS_MAX_TIMER     ICF_SET_CALL_PRM_DNS_MAX_TIMER



typedef icf_set_call_params_req_st icf_cfg_cc_set_call_params_req_st;



/*-----------------------------------------------------------------------------
 * Structure Name: icf_cfg_cc_set_self_id_params_req_st
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ
 *
 *---------------------------------------------------------------------------*/
/* Configuration merge related New Type defs */
#define  ICF_CFG_CC_SET_SLF_ID_PRM_ADDR_ACTION      ICF_SET_SLF_ID_PRM_ADDR_ACTION
#define  ICF_CFG_CC_SET_SLF_ID_PRM_ADDR             ICF_SET_SLF_ID_PRM_ADDR
#define  ICF_CFG_CC_SET_SLF_ID_PRM_PRIORITY         ICF_SET_SLF_ID_PRM_PRIORITY
#define  ICF_CFG_CC_SET_SLF_ID_PRM_NAME             ICF_SET_SLF_ID_PRM_NAME
#define  ICF_CFG_CC_SET_SLF_ID_PRM_DEFAULT_SETTINGS ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS
#define  ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_KEY         ICF_SET_SLF_ID_PRM_AUTH_KEY
#define  ICF_CFG_CC_SET_SLF_ID_PRM_AUTH_PASSWORD    ICF_SET_SLF_ID_PRM_AUTH_PASSWORD
#define  ICF_CFG_CC_SET_SLF_ID_PRM_REALM            ICF_SET_SLF_ID_PRM_REALM

#define  ICF_CFG_CC_SET_SLF_ID_PRM_ADDR_LIST             ICF_SET_SLF_ID_PRM_ADDR_LIST
#define  ICF_CFG_CC_SET_SLF_ID_PRM_NAME_LIST             ICF_SET_SLF_ID_PRM_NAME_LIST
#define  ICF_CFG_CC_SET_SLF_ID_PRM_DEFAULT_SETTINGS_LIST ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS_LIST

typedef icf_set_self_id_req_st icf_cfg_cc_set_self_id_params_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_cfg_cc_set_registration_params_req_st
 *
 * Description: This structure specifies payload data of
 *              API ICF_CFG_CC_SET_REGISTRATION_PARAMS_REQ
 *
 *---------------------------------------------------------------------------*/
#define ICF_CFG_CC_SET_REG_REQ_MODE                     ICF_SET_REG_REQ_MODE
#define ICF_CFG_CC_SET_REG_REQ_DURATION                 ICF_SET_REG_REQ_DURATION
#define ICF_CFG_CC_SET_REG_REQ_ROUTE_ADDR           	ICF_SET_REG_REQ_ROUTE_ADDR
#define ICF_CFG_CC_SET_REG_HS_DURATION			        ICF_SET_REG_HS_DURATION

typedef icf_set_registration_req_st icf_cfg_cc_set_registration_params_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_cc_cfg_set_config_param_resp_st
 *
 * Description: This structure is used as payload of API 
 *              ICF_CC_CFG_SET_CONFIG_PARAM_RRESP.
 *
 *---------------------------------------------------------------------------*/

typedef icf_configure_cfm_st icf_cc_cfg_set_config_params_resp_st;


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
						New Mapping for icf_pa_api.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
*
* NAME : icf_cc_pa_incoming_call_ind_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API  ICF_CC_PA_INCOMING_CALL_IND.
-----------------------------------------------------------------------------*/

#define ICF_CC_PA_INC_CALL_CLG_PTY_ADDR          ICF_CALLING_PARTY_ADD_PRESENT
#define ICF_CC_PA_INC_CALL_CLG_PTY_NAME          ICF_CALLING_PARTY_NAME_PRESENT
#define ICF_CC_PA_INC_CALL_TFR_PTY_ADDR          ICF_TRANSF_PARTY_ADD_PRESENT
#define ICF_CC_PA_INC_CALL_TFR_PTY_NAME          ICF_TRANSF_PARTY_NAME_PRESENT

typedef icf_incoming_call_ind_st icf_cc_pa_incoming_call_ind_st;


/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_info_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_PA_CC_INFO_REQ.
-------------------------------------------------------------------------------*/

#define ICF_PA_CC_INFO_REQ_INFO_PAYLOAD	ICF_INFO_REQ_INFO_PAYLOAD    

typedef icf_info_req_st				icf_pa_cc_info_req_st;


/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_info_cfm_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_PA_CC_INFO_CFM.
-------------------------------------------------------------------------------*/
typedef icf_info_cfm_st				icf_pa_cc_info_cfm_st;



/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_create_call_req_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API ICF_PA_CC_CREATE_CALL_REQ. 
-----------------------------------------------------------------------------*/

#define ICF_PA_CC_CREATE_CALL_REQ_CALLED_PARTY_NAME             ICF_CALLED_PARTY_NAME_PRESENT
#define ICF_PA_CC_CREATE_CALL_REQ_TOGGLE_PRESENTATION_INDICATOR ICF_TOGGLE_PRES_IND_PRESENT
#define ICF_PA_CC_CREATE_CALL_REQ_QOS_MARK                      ICF_QOS_MARK_PRESENT
#define ICF_PA_CC_CREATE_CALL_REQ_RES_RESERV_REQD				ICF_RESOURCE_RESERV_FLAG_PRESENT

typedef icf_create_call_req_st icf_pa_cc_create_call_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_create_ext_xconf_req_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API ICF_CREATE_XCONF_REQ. 
-----------------------------------------------------------------------------*/
typedef icf_create_ext_xconf_req_st icf_pa_cc_create_ext_xconf_req_st;

/*ICF_HANDLE_REINVITE_START*/
/*-----------------------------------------------------------------------------
*
* NAME : icf_cc_pa_incoming_call_modify_ind_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API  ICF_CC_PA_INCOMING_CALL_MODIFY_IND.
-----------------------------------------------------------------------------*/

typedef icf_incoming_call_modify_ind_st icf_cc_pa_incoming_call_modify_ind_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_call_modify_cfm_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API  ICF_PA_CC_CALL_MODIFY_CFM.
-----------------------------------------------------------------------------*/

typedef icf_call_modify_cfm_st icf_pa_cc_call_modify_cfm_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_call_modify_req_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API  ICF_PA_CC_CALL_MODIFY_REQ.
-----------------------------------------------------------------------------*/

typedef icf_call_modify_req_st icf_pa_cc_call_modify_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_cc_pa_call_modify_resp_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API  ICF_CC_PA_CALL_MODIFY_RESP.
-----------------------------------------------------------------------------*/

typedef icf_call_modify_resp_st icf_cc_pa_call_modify_resp_st;

/*ICF_HANDLE_REINVITE_END*/

/*-----------------------------------------------------------------------------
 *
 * NAME : icf_cc_pa_alert_user_ind_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *               payload data of API ICF_CC_PA_ALERT_USER_IND. 
-----------------------------------------------------------------------------*/
 
typedef icf_remote_user_alerted_ind_st icf_cc_pa_alert_user_ind_st;



/*-----------------------------------------------------------------------------
 *
 * NAME : icf_pa_cc_user_alerted_req_st
 * 
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                 payload data of API ICF_PA_CC_USER_ALERTED_REQ. 
 *                 
-----------------------------------------------------------------------------*/

typedef icf_local_user_alerted_req_st icf_pa_cc_user_alerted_req_st;


/*-----------------------------------------------------------------------------
 *
 * NAME : icf_pa_cc_media_connect_req_st
 * 
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                 payload data of API ICF_PA_CC_MEDIA_CONNECT_REQ. 
 *                 
-----------------------------------------------------------------------------*/

typedef icf_media_connect_req_st icf_pa_cc_media_connect_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_cc_pa_terminate_call_ind_st
*
* DESCRIPTION  :  This data type is used to define a structure, 
*                 which specifies payload data of API 
*                 ICF_CC_PA_TERMINATE_CALL_IND.
*----------------------------------------------------------------------------*/

#define ICF_CC_PA_TERMINATE_CALL_IND_ADDNL_INFO        ICF_ADDNL_INFO_PRESENT

typedef icf_terminate_call_ind_st icf_cc_pa_terminate_call_ind_st;


/*-----------------------------------------------------------------------------
 *
 * NAME : icf_pa_cc_terminate_call_req_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                payload data of API ICF_CC_PA_TERMINATE_CALL_REQ. 
 *                
 ----------------------------------------------------------------------------*/

#define ICF_PA_CC_TERMINATE_CALL_REQ_LINE_ID        ICF_LINE_ID_PRESENT

typedef icf_terminate_call_req_st icf_pa_cc_terminate_call_req_st;



/*-----------------------------------------------------------------------------
 *
 * NAME : icf_pa_cc_connect_req_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                payload data of API ICF_PA_CC_CONNECT_REQ. 
 *                
 ----------------------------------------------------------------------------*/

typedef icf_connect_req_st icf_pa_cc_connect_req_st;

/*-----------------------------------------------------------------------------
 *
 * NAME : icf_connect_ind_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                payload data of API ICF_CC_PA_CONNECT_IND.
 *                
 ----------------------------------------------------------------------------*/

#define ICF_CC_PA_CONNECT_IND_CONTACT_HEADER		ICF_CONNECT_IND_CONTACT_HEADER




/*-----------------------------------------------------------------------------

 *

 * Structure Name:  icf_msg_resp_st

 *

 * Description: This is a common structure used to return back the response of 

 *              _req APIs.

 *

 *---------------------------------------------------------------------------*/

typedef icf_msg_resp_st	icf_mm_msg_resp_st;


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
					New Mapping for misc api st
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_sm_cc_set_trace_level_req_st
 *
 * Description: This is payload of API ICF_SM_CC_SET_TRACE_LEVEL_REQ.
 *              This API is sent by system administration module to enable or  
 *              disable the reporting of traces of a specific level or type,
 *              during system runtime.
 *
 *---------------------------------------------------------------------------*/

typedef icf_set_trace_level_req_st icf_sm_cc_set_trace_level_req_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_sm_report_trace_ind_st
 *
 * Description: This is payload of API ICF_CC_SM_REPORT_TRACE_IND. 
 *              This API is sent by call control to report a trace of given
 *              level and type. The API can report upto 4 integer and 2 string  
 *              values.
 *
 *---------------------------------------------------------------------------*/

typedef icf_report_trace_ind_st icf_cc_sm_report_trace_ind_st;
#endif

/*-----------------------------------------------------------------------------
*
* NAME : icf_pa_cc_call_transfer_attended_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_PA_CC_CALL_TRANSFER_ATTENDED_REQ.
-----------------------------------------------------------------------------*/
typedef icf_call_transfer_attended_req_st icf_pa_cc_call_transfer_attended_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_call_transfer_unattended_req_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ.
-----------------------------------------------------------------------------*/
#define ICF_PA_CC_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR \
        ICF_CALL_TRANSFER_UNATTENDED_REQ_TOGGLE_PRESENTATION_INDICATOR

typedef icf_call_transfer_unattended_req_st icf_pa_cc_call_transfer_unattended_req_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_remote_call_transfer_initiated_ind_st
*
* DESCRIPTION  : This data type is used to define a structure,
*               which specifies payload data of API
*               ICF_REMOTE_CALL_TRANSFER_INITIATED_IND.
-----------------------------------------------------------------------------*/
#define ICF_CC_PA_REFER_INITIATED_REFERRED_PARTY_NAME ICF_REFER_INITIATED_REFERRED_PARTY_NAME
typedef icf_remote_call_transfer_initiated_ind_st icf_cc_pa_remote_call_transfer_initiated_ind_st;

/*-----------------------------------------------------------------------------
*
* NAME : icf_call_merged_ind_st
*
* DESCRIPTION  : This data type is used to define a structure,
*                which specifies payload data of API
*                ICF_CALL_MERGED_IND.
-----------------------------------------------------------------------------*/
typedef icf_call_merged_ind_st icf_cc_pa_call_merged_ind_st;


typedef icf_conference_req_st  icf_pa_cc_conference_req_st;



/*-----------------------------------------------------------------------------
 *
 * NAME : icf_display_ind_st
 *
 * DESCRIPTION  :  This data type is used to define a structure,which specifies
 *                 payload data of API ICF_CC_PA_DISPLAY_IND. 
 *                 
-----------------------------------------------------------------------------*/

  /* This field bitwise contains the presence information of optional fields 
     * in this structure.
     */

#define ICF_CC_PA_DISPLAY_IND_DISPLAY_DATA    ICF_DISPLAY_IND_DISPLAY_DATA
#define ICF_CC_PA_DISPLAY_IND_TONE            ICF_DISPLAY_IND_TONE

/*typedef icf_tone_id_t icf_tone_id_t;*/
    
typedef icf_display_ind_st icf_cc_pa_display_ind_st;

/*------------------------------------------------------------------------------
*
*NAME : icf_cc_pa_call_redirection_ind_st
*
*DESCRIPTION : This data type is used to define a structure,
*               which specifies payload data of 
*               API ICF_CC_PA_CALL_REDIRECTION_IND.
*
------------------------------------------------------------------------------*/
#define ICF_CC_PA_CALL_RDR_IND_PTY_ADDR   ICF_CALL_RDR_IND_PTY_ADDR
#define ICF_CC_PA_CALL_RDR_IND_PTY_NAME   ICF_CALL_RDR_IND_PTY_NAME
typedef icf_call_redirection_ind_st    icf_cc_pa_call_redirection_ind_st;


#ifdef ICF_ERROR_ENABLE
typedef icf_set_error_report_level_req_st icf_sm_cc_set_error_report_level_req_st;
#endif

/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_cc_sm_report_error_ind_st
 *
 * Description: This is payload of API ICF_CC_SM_REPORT_ERROR_IND. 
 *              This API is sent by call control to report a error of given
 *              level and type. The API can report upto 4 integer and 2 string  
 *              values.
 *
 *---------------------------------------------------------------------------*/

typedef icf_report_error_ind_st icf_cc_sm_report_error_ind_st;

typedef enum
{
	/* used in case of RTCP flow */
	ICF_FLOW_TYPE_NONE=0,
	ICF_FLOW_TYPE_AUDIO,
	ICF_FLOW_TYPE_VIDEO,
}icf_flow_type_et;

typedef enum
{
	ICF_FLOW_PROTOCOL_RTP=0,
	ICF_FLOW_PROTOCOL_RTCP
}icf_flow_protocol_et;

typedef enum
{
	ICF_FLOW_DIR_UPLINK=0,
	ICF_FLOW_DIR_DOWNLINK
}icf_flow_direction_et;

typedef struct 
{
	icf_uint16_t	 media_component_id;
	icf_uint16_t	 ip_flow_id;
}icf_flow_id_st;

typedef struct 
{	/* destination IP and port */
	icf_transport_address_st			destination_address;
	icf_flow_direction_et	direction;
}icf_flow_direction_info_st;

typedef struct
{
	icf_flow_id_st			flow_id;
	icf_flow_type_et		flow_type;
	icf_flow_protocol_et	flow_protocol;
	/* used in case of RTCP flows to denote the linked RTP flow */
	icf_uint8_t				linked_flow_index;
	icf_uint8_t				num_flow_directions;
	icf_uint8_t				max_traffic_class;
	icf_uint16_t				max_bit_rate_ul;
	icf_uint16_t				max_bit_rate_dl;
	icf_flow_direction_info_st flow_direction_info[ICF_MAX_FLOW_DIRECTIONS];
}icf_flow_info_st;


typedef struct 
{
	icf_call_id_t		call_id;
	icf_uint8_t			num_flow_ids;
	icf_flow_info_st	flow_info[ICF_MAX_MEDIA_FLOWS];
}icf_call_flow_map_st;

typedef struct 
{
	icf_uint8_t			stream_id;
	icf_uint8_t			from_flow_idx;
	icf_uint8_t			to_flow_idx;
}icf_stream_flow_map_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_stream_status_st
 * Description :  This structure is used by MMI to indicate the status of each
 * 				  stream in the CMS response. 
 *
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_stream_id_t		stream_id;
	/* this boolean determines if the stream processing was successful or not
	 */
	icf_boolean_t		status_success;
}icf_stream_status_st;

/*-----------------------------------------------------------------------------
* Structure Name: icf_mms_modify_media_info_st
* Description: 	  The bits are set by CC, to indicate to MMI, the sequence of
* 				  steps to be executed during MMS
*---------------------------------------------------------------------------*/
typedef struct
{
 	icf_uint16_t		bitmask;
#define 	ICF_MODE_CHANGE					0x001
	/* (OTG/INC Re-INVITE for Mode Change)+ (Answer for OTG INVITE)+
 	 * (INC INVITE call-setup) */
#define 	ICF_REMOTE_SDP_MEDIA_CHANGE			0x002
 	/* (INC Re-INVITE/UPDATE for SDP change) */
#define 	ICF_LOCAL_SDP_MEDIA_CHANGE			0x004
	/* (OTG Re-INVITE for SDP change)*/
#define 	ICF_REMOTE_SDP_CODEC_CHANGE			0x008
	/* (INC Re-INVITE for CODEC change) */
#define 	ICF_LOCAL_SDP_CODEC_CHANGE			0x010
	/* (OTG Re-INVITE for CODEC change) */
#define 	ICF_REMOTE_ANSWER_CODEC_CHANGE		0x020
	/* "(Answer for OTG INVITE)+ 
	 * (Answer for OTG Re-INVITE. Offer ACC by peer)" */
#define 	ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE	0x040
	/* (Answer for OTG Re-INVITE. Offer REJ by peer) */
#define 	ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE	0x080
	/* (Answer for OTG re-INVITE with only sdp changed in reINVITE) */
#define 	ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE	0x100
	/* Application return reject for call modify ind due to 
	 * ICF_REMOTE_SDP_MEDIA_CHANGE */
#define 	ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE	0x200
	/* Application return reject for call modify ind due to 
	 * MMS is required to add this stream */
/* The below mentioned commented #defines are also defined somewhere else
 * Thus add the new bitmasks after this value.
 */
    /*#define     ICF_STREAM_ADD                         0x400*/
    /* Processing is being done to delete this stream */
    /*#define     ICF_STREAM_DELETE                      0x800*/
   /* ICF_REMOTE_SDP_CODEC_CHANGE */
#define ICF_LOCAL_MEDIA_CONNECT_CODEC_CHANGE           0x1000
#define ICF_LOCAL_MEDIA_CONNECT_MODE_CHANGE            0x2000

  icf_stream_id_t		stream_id;
  icf_media_type_t 	stream_type;
}icf_mms_modify_media_info_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_forked_resp_data_st
 * Description :  This structures stores the sdp info along with its identifier
 *                to uniquely indentify each sdp and retrieve it when accordingly
 *                when a final response to a forked response is received
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
    icf_uint8_t                   forked_resp_id;
    icf_internal_sdp_stream_st      *p_sdp_info;
} icf_forked_resp_data_st;





#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_COMMON_TYPES_H__ */
