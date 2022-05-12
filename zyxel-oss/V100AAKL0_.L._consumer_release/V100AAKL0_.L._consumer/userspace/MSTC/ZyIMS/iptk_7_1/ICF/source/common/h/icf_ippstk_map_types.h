/*------------------------------------------------------------------------------
 *
 * File name        : icf_icf_map_types.h
 *
 * Purpose          : This file contains the mapping of ICF and ICF types.
 *
 * Revision History :
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 03-Aug-2005  Rohit Aggarwal                  Initial creation
 * 02-Feb-2006  Amit Sharma                     CFG merged with ICF 2.0
 * 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
 * 13-Jan-2006  Aman Arora		IPSEC			added self IP and port for
 * 												binding during IPSEC
 * 07-Dec-2005  Anuradha Gupta					Added new typedefs for Re-invite
 * Copyright (c) 2006, Aricent .
 *----------------------------------------------------------------------------*/



#ifndef __ICF_ICF_MAP_TYPES_H__
#define __ICF_ICF_MAP_TYPES_H__

#include "icf_map_extif_feature_flags.h"
#include "icf_types.h"
#include "icf_api.h"
#include "icf_mm_types.h"
#include "icf_icf_map_defs.h"
#include "icf_mm_api.h"


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

#if 0
/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_types.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_int8_t
 * 
 * Description: This data type represents a 1 byte signed integer
 *
 *---------------------------------------------------------------------------*/

typedef icf_int8_t icf_int8_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_int16_t
 *
 * Description: This data type represents a 2 byte-signed integer.
 * 
 *---------------------------------------------------------------------------*/

typedef icf_int16_t icf_int16_t; 


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_int32_t
 *
 * Description: This data type represents a 4 byte signed long integer
 *
 *---------------------------------------------------------------------------*/

typedef icf_int32_t icf_int32_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_uint8_t
 *
 * Description: This data type represents a 1 byte unsigned integer.
 *
 *---------------------------------------------------------------------------*/

typedef icf_uint8_t icf_uint8_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_uint16_t
 * 
 * Description: This data type represents a 2 byte unsigned integer. 
 * 
 *---------------------------------------------------------------------------*/

typedef icf_uint16_t icf_uint16_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_uint32_t
 *
 * Description: This data type represents a 4 byte unsigned long integer.
 *  
 *---------------------------------------------------------------------------*/

typedef icf_uint32_t icf_uint32_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_real_t
 *
 * Description: This data type represents a double data type 
 * 
 ---------------------------------------------------------------------------*/

/*typedef icf_real_t icf_real_t; */


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_boolean_t
 * 
 * Description: Represent boolean vales
 *
 *---------------------------------------------------------------------------*/

typedef icf_boolean_t icf_boolean_t;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_string_st
 * 
 * Description: This data type represents a structure, which consists of a 
 *              character array of specified length
 *
 *---------------------------------------------------------------------------*/

typedef icf_string_st icf_string_st;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_return_t
 * 
 * Description: Represent the  execution status values return by a function.
 *              This type is used in function interface and not in message based
 *              interface.
 *
 *---------------------------------------------------------------------------*/

typedef icf_return_t icf_return_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_void_t
 * 
 * Description: Represent the type void.
 *
 *---------------------------------------------------------------------------*/

typedef icf_void_t icf_void_t;




/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_common_api_types.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_api_id_t
 *
 * Description: Used to define unique identifiers to represent APIs on various 
 *              interfaces
 * 
 *---------------------------------------------------------------------------*/

typedef icf_api_id_t icf_api_id_t;			


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_call_id_t
 *
 * Description: Used to define unique identifiers to represent call id
 * 
 *---------------------------------------------------------------------------*/

typedef icf_call_id_t icf_call_id_t;


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_line_id_t
 *
 * Description: Used to define unique identifiers to represent line id
 * 
 *----------------------------------------------------------------------------*/

typedef icf_line_id_t icf_line_id_t;


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_duration_t
 *
 * Description: 
 * 
 *----------------------------------------------------------------------------*/

typedef icf_duration_t icf_duration_t;


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_error_t
 *
 * Description: Defines the error cause when the response of an API is failure.
 * 
 *----------------------------------------------------------------------------*/

typedef icf_error_t icf_error_t;


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_inband_info_status_t
 *
 * Description: Used as a flag to state who will play the ring back tone
 * 
 *----------------------------------------------------------------------------*/

typedef icf_inband_info_status_t icf_inband_info_status_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_mode_t
 *
 * Description: Used to state the mode of line 
 * 
 *---------------------------------------------------------------------------*/

typedef icf_mode_t icf_mode_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_module_id_t
 *
 * Description: Used to define unique identifiers to represent various modules
 * 
 *---------------------------------------------------------------------------*/

typedef icf_module_id_t icf_module_id_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_presentation_indicator_t
 *
 * Description: Used to state if the name of calling party will be displayed
 * 
 *---------------------------------------------------------------------------*/

typedef icf_presentation_indicator_t icf_presentation_indicator_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_result_t
 *
 * Description: Used to show the success or failure
 * 
 *---------------------------------------------------------------------------*/

typedef icf_result_t icf_result_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_service_type_t
 *
 * Description: Shows the type of service in case of call hold 
 * 
 *---------------------------------------------------------------------------*/

typedef icf_service_type_t icf_service_type_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_status_t
 *
 * Description: Value indicates the enable/disable status of a flag.
 * 
 *---------------------------------------------------------------------------*/

typedef icf_status_t icf_status_t;


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_stream_id_t
 *
 * Description: defines the stream id within a call.
 * 
 *----------------------------------------------------------------------------*/

typedef icf_stream_id_t icf_stream_id_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_app_id_t
 *
 * Description: Identifies the application invoking/receiving the API.
 *
 *---------------------------------------------------------------------------*/

typedef icf_app_id_t icf_app_id_t;            


/*------------------------------------------------------------------------------
 * 
 * typedef Name: icf_call_type_t
 *
 * Description: defines the call type for an incoming/outgoing call.
 * 
 *----------------------------------------------------------------------------*/

typedef icf_call_type_t icf_call_type_t;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_header_st
 *
 * Description: This datatype represents a structure that contains SIP headers.
 *              
 *---------------------------------------------------------------------------*/

typedef icf_header_st icf_header_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_header_list_st
 *
 * Description: This datatype represents a structure that contains a list
 *                of SIP headers.
 *
 *---------------------------------------------------------------------------*/

typedef icf_header_list_st icf_header_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_msg_body_st
 *
 * Description: This datatype represents a structure that contains message bodies.
 *
 *---------------------------------------------------------------------------*/

typedef icf_msg_body_st icf_msg_body_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_msg_body_list_st
 *
 * Description: This datatype represents a structure that contains a list
 *                of message bodies.
 *
 *---------------------------------------------------------------------------*/

typedef icf_msg_body_list_st icf_msg_body_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_stream_st
 *
 * Description: This datatype represents a structure that contains stream
 *              information. Presently only used for specifying a TBCP stream 
 *
 *---------------------------------------------------------------------------*/

typedef icf_stream_st icf_stream_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_stream_list_st
 *
 * Description: This datatype represents a structure that contains
 *                a list of stream information.
 *
 *---------------------------------------------------------------------------*/

typedef icf_stream_list_st icf_stream_list_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_tag_st
 *
 * Description: This datatype represents a structure that contains SIP
 *                specific tags or parameters.
 *
 *---------------------------------------------------------------------------*/

typedef icf_tag_st icf_tag_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_tag_list_st
 *
 * Description: This datatype represents a structure that contains a list
 *                of SIP specific tags or parameters.
 *
 *---------------------------------------------------------------------------*/

typedef icf_tag_list_st icf_tag_list_st;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_tone_id_t
 *
 * Description: 
 * 
 *---------------------------------------------------------------------------*/

/*typedef icf_tone_id_t icf_tone_id_t;*/


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_transport_mode_t
 *
 * Description: 
 * 
 *---------------------------------------------------------------------------*/

typedef icf_transport_mode_t icf_transport_mode_t;


/*-----------------------------------------------------------------------------
 * 
 * typedef Name: icf_address_type_t
 *
 * Description: This data type is used to define type of address format for the
 *              party address information present in the icf_address_st 
 *              structure
 * 
 *---------------------------------------------------------------------------*/

typedef icf_address_type_t icf_address_type_t;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_api_header_st
 *
 * Description: This is a generic API header used on all interfaces
 *
 *---------------------------------------------------------------------------*/

typedef icf_api_header_st icf_api_header_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_name_st
 * 
 * Description: This data type is used to define a data value of pp_string_st 
 *              type, which contains the name of the calling or called party.
 *
 *---------------------------------------------------------------------------*/

typedef icf_name_st icf_name_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_ipv4_int_address_st
 *
 * Description: This data type is used to define a union tagged by 
 *              icf_ip_address_type_t, which specifies the IP address
 *
 *---------------------------------------------------------------------------*/

typedef icf_ipv4_int_address_st icf_ipv4_int_address_st;

#ifdef ICF_IPV6_ENABLED
typedef icf_ipv6_int_address_st icf_ipv6_int_address_st;
#endif

/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_transport_addr_type_t
 *
 * Description: This data type is used to define which transport
 *              address type is used
 *
 *---------------------------------------------------------------------------*/
#define  ICF_TRANSPORT_ADDRESS_INVALID		 ICF_TRANSPORT_ADDRESS_INVALID
#define  ICF_TRANSPORT_ADDRESS_DOMAIN_NAME    ICF_TRANSPORT_ADDRESS_DOMAIN_NAME
#define  ICF_TRANSPORT_ADDRESS_IPV4_ADDR      ICF_TRANSPORT_ADDRESS_IPV4_ADDR
#define  ICF_TRANSPORT_ADDRESS_IPV6_ADDR      ICF_TRANSPORT_ADDRESS_IPV6_ADDR

typedef icf_transport_addr_type_t icf_transport_addr_type_t;



/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_transport_addr_st
 *
 * Description: This data type is used to define a union tagged by 
 *              icf_ip_address_type_t, which specifies the IP address
 *
 *---------------------------------------------------------------------------*/

typedef icf_transport_addr_st icf_transport_addr_st;

 	
 
/*-----------------------------------------------------------------------------
 * 
 * Structure Name:icf_transport_address_st   
 *
 * Description: This data type is used to define a structure, which contains the
 *              IP address and the port number.
 *
 *---------------------------------------------------------------------------*/

typedef icf_transport_address_st icf_transport_address_st;



typedef icf_address_st icf_address_st;


/*-----------------------------------------------------------------------------
*
* NAME : icf_msg_st
*
* DESCRIPTION  :messages excange at external interface 
*
-----------------------------------------------------------------------------*/

typedef icf_msg_st icf_msg_st;
 



/*-----------------------------------------------------------------------
 * NAME : icf_qos_mark_t
 *-----------------------------------------------------------------------*/

typedef icf_qos_mark_t icf_qos_mark_t;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_msg_resp_st
 *
 * Description: This is a common structure used to return back the response of 
 *              _req APIs.
 *
 *---------------------------------------------------------------------------*/

typedef icf_app_remove_cfm_st icf_msg_resp_st;



/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_cfg_types.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
 *
 * values of type icf_mode_t 
 *
 *
 *---------------------------------------------------------------------------*/

/* Invite with c=0 */
#define ICF_CALL_HOLD_OPT_INV_WTH_C_0                ICF_CALL_HOLD_OPT_INV_WTH_C_0

/* Invite with mode as send only */
#define ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY     ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY

/* Invite with port as 0 */
#define ICF_CALL_HOLD_OPT_INV_WTH_PORT_0             ICF_CALL_HOLD_OPT_INV_WTH_PORT_0

/* Update with c=0 */
#define ICF_CALL_HOLD_OPT_UPDT_WTH_C_0               ICF_CALL_HOLD_OPT_UPDT_WTH_C_0

/* Update with mode as send only */
#define ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY    ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY

/* Update with port as 0 */
#define ICF_CALL_HOLD_OPT_UPDT_WTH_PORT_0            ICF_CALL_HOLD_OPT_UPDT_WTH_PORT_0

/* INVITE with mode=send only and c=0 */
#define ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY_N_C_0   ICF_CALL_HOLD_OPT_INV_WTH_MODE_SEND_ONLY_N_C_0 

/* UPDATE with mode=send only and c=0 */
#define ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY_N_C_0  ICF_CALL_HOLD_OPT_UPDT_WTH_MODE_SEND_ONLY_N_C_0


/*-----------------------------------------------------------------------------
 *
 * Name: icf_hold_option_t 
 *
 * Description: This data type is used to define a data value of icf_uint8_t
 *              type, which identifies the option used by CC to send call hold 
 *              indication to remote party.
 *
 *---------------------------------------------------------------------------*/


/* typedef icf_hold_option_t     icf_hold_option_t;*/


/*-----------------------------------------------------------------------
 * NAME : icf_config_action_t
 *-----------------------------------------------------------------------*/

typedef icf_config_action_t icf_config_action_t;


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_config_transport_addr_st 
 *
 * Description: This data type is used to define a structure, which contains
 *              the transport addresses supported by phone.
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_transport_address_st icf_config_transport_address_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name: icf_config_transport_addr_list_st 
 *
 * Description: This data type is used to define a structure, which contains
 *              the list of transport addresses supported by phone.
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_transport_address_list_st icf_config_transport_address_list_st;



/*-----------------------------------------------------------------------
 * NAME : Values for icf_config_action_t
 *-----------------------------------------------------------------------*/

#define ICF_CONFIG_ACTION_ADD        ICF_CONFIG_ACTION_ADD
#define ICF_CONFIG_ACTION_MODIFY     ICF_CONFIG_ACTION_MODIFY
#define ICF_CONFIG_ACTION_DELETE     ICF_CONFIG_ACTION_DELETE


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_self_addr_st 
 *
 * Description: This structure contains contains the information of the
 *               self address to be modified 
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_self_addr_st icf_config_self_addr_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_self_name_st 
 *
 * Description: This structure contains contains the information of local user
 *               name to be modified 
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_self_name_st icf_config_self_name_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_self_name_list_st 
 *
 * Description: This structure contains contains the list of local user
 *               name supported by the phone 
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_self_name_list_st icf_config_self_name_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_self_addr_list_st 
 *
 * Description: This structure contains contains the list of local user
 *               addresses supported by the phone 
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_self_addr_list_st icf_config_self_addr_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_address_st 
 *
 *  Description: This data type is used to define a structure, which 
 *  contains the party address for a  line.
 *
 *---------------------------------------------------------------------------*/
typedef icf_config_address_st icf_config_address_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_address_list_st 
 *
 *  Description: This data type is used to define a structure, which 
 *               contains the list of party address.
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_address_list_st icf_config_address_list_st;


#define ICF_CONFIG_RG_PRM_RG_ADDR                    ICF_CONFIG_RG_PRM_RG_ADDR
#define ICF_CONFIG_RG_PRM_RG_MODE                    ICF_CONFIG_RG_PRM_RG_MODE
#define ICF_CONFIG_RG_PRM_RG_DURATION                ICF_CONFIG_RG_PRM_RG_DURATION
#define ICF_CONFIG_RG_PRM_AUTH_KEY                   ICF_CONFIG_RG_PRM_AUTH_KEY
#define ICF_CONFIG_RG_PRM_AUTH_PASSWORD              ICF_CONFIG_RG_PRM_AUTH_PASSWORD
#ifdef IMS_CLIENT
#define ICF_CONFIG_RG_PRM_AUTH_REALM               	ICF_CONFIG_RG_PRM_AUTH_REALM
#define ICF_CONFIG_RG_PRM_AUTH_NONCE              	ICF_CONFIG_RG_PRM_AUTH_NONCE
#define ICF_CONFIG_RG_PRM_AUTH_NC                    ICF_CONFIG_RG_PRM_AUTH_NC
#define ICF_CONFIG_RG_PRM_AUTH_CNONCE               	ICF_CONFIG_RG_PRM_AUTH_CNONCE
#define ICF_CONFIG_RG_PRM_AUTH_RESPONSE              ICF_CONFIG_RG_PRM_AUTH_RESPONSE
#define ICF_CONFIG_RG_PRM_AUTH_OPAQUE               	ICF_CONFIG_RG_PRM_AUTH_OPAQUE
#endif


typedef icf_config_registration_params_st icf_config_registration_params_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_default_settings_st 
 *
 *  Description: This data type is used to define a structure, which 
 *  contains the default settings for a  line.
 *
 *---------------------------------------------------------------------------*/
typedef icf_config_default_settings_st icf_config_default_settings_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_default_settings_list_st 
 *
 *  Description: This data type is used to define a structure, which 
 *               contains the list of default settings supported by phone.
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_default_settings_list_st icf_config_default_settings_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_status_st 
 *
 *  Description: This structure contains contains the bitmask per line. 
 *               like service subscr ption, service activation etc 
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_status_st icf_config_status_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_config_status_list_st 
 *
 * Description: THis structure contains the list of status values for lines
 *
 *---------------------------------------------------------------------------*/

typedef icf_config_status_list_st icf_config_status_list_st;




/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_cfg_api.h
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

#define ICF_SERVICE_SUBS_CALL_HOLD_RESUME  ICF_SERVICE_SUBS_CALL_HOLD_RESUME
#define ICF_SERVICE_SUBS_THREE_WAY_CALL    ICF_SERVICE_SUBS_THREE_WAY_CALL
#define ICF_SERVICE_SUBS_CALL_TRF_ATTENDED ICF_SERVICE_SUBS_CALL_TRF_ATTENDED
#define ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED  ICF_SERVICE_SUBS_CALL_TRF_UNATTENDED
#define ICF_SERVICE_SUBS_INVITE_WITH_REPLACE  ICF_SERVICE_SUBS_INVITE_WITH_REPLACE
#define ICF_SERVICE_SUBS_CALL_TRF_FROM_NW     ICF_SERVICE_SUBS_CALL_TRF_FROM_NW
#define ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL  ICF_SERVICE_SUBS_CALL_FWD_UNCONDITIONAL
#define ICF_SERVICE_SUBS_CALL_FWD_BUSY           ICF_SERVICE_SUBS_CALL_FWD_BUSY
#define ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER      ICF_SERVICE_SUBS_CALL_FWD_NO_ANSWER
#define ICF_SERVICE_SUBS_CALL_WAITING            ICF_SERVICE_SUBS_CALL_WAITING
#define ICF_SERVICE_SUBS_MESSAGE_WAITING_IND     ICF_SERVICE_SUBS_MESSAGE_WAITING_IND
#define ICF_SERVICE_SUBS_EXTERNAL_CONFERENCE  ICF_SERVICE_SUBS_EXTERNAL_CONFERENCE

    /* This is part of a bitmask,which specifies the service activated by user
      *  Following are the values which can be filled in status field
      *  in icf_config_status_st  */
#define ICF_SERVICE_ACT_CALL_HOLD_RESUME     ICF_SERVICE_ACT_CALL_HOLD_RESUME
#define ICF_SERVICE_ACT_THREE_WAY_CALL       ICF_SERVICE_ACT_THREE_WAY_CALL
#define ICF_SERVICE_ACT_CALL_TRF_ATTENDED    ICF_SERVICE_ACT_CALL_TRF_ATTENDED
#define ICF_SERVICE_ACT_CALL_TRF_UNATTENDED  ICF_SERVICE_ACT_CALL_TRF_UNATTENDED
#define ICF_SERVICE_ACT_INVITE_WITH_REPLACE  ICF_SERVICE_ACT_INVITE_WITH_REPLACE
#define ICF_SERVICE_ACT_CALL_TRF_FROM_NW     ICF_SERVICE_ACT_CALL_TRF_FROM_NW
#define ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL   ICF_SERVICE_ACT_CALL_FWD_UNCONDITIONAL
#define ICF_SERVICE_ACT_CALL_FWD_BUSY            ICF_SERVICE_ACT_CALL_FWD_BUSY
#define ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER   ICF_SERVICE_ACT_CALL_FORWARDING_NO_ANSWER
#define ICF_SERVICE_ACT_CALL_WAITING                ICF_SERVICE_ACT_CALL_WAITING
#define ICF_SERVICE_ACT_EXTERNAL_CONFERENCE         ICF_SERVICE_ACT_EXTERNAL_CONFERENCE
typedef icf_hold_option_t  icf_hold_option_t;

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
#define  ICF_CFG_CC_SET_CALL_PRM_SETUP_TIMER					ICF_SET_CALL_PRM_SETUP_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_ALERTING_TIMER				ICF_SET_CALL_PRM_ALERTING_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_RELEASE_TIMER				ICF_SET_CALL_PRM_RELEASE_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_MODIFY_MEDIA_TIMER			ICF_SET_CALL_PRM_MODIFY_MEDIA_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_RG_RSP_TIMER				ICF_SET_CALL_PRM_REGISTRATION_RESP_DURATION
#define  ICF_CFG_CC_SET_CALL_PRM_RG_RETRY_TIMER				ICF_SET_CALL_PRM_REGISTRATION_RETRY_DURATION
#ifdef ICF_SESSION_TIMER
#define  ICF_CFG_CC_SET_CALL_PRM_SESSION_TIMER_INTERVAL		ICF_SET_CALL_PRM_SESSION_TIMER_INTERVAL
#define  ICF_CFG_CC_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL  ICF_SET_CALL_PRM_MIN_SESSION_TIMER_INTERVAL
#endif


#define ICF_CFG_CC_CONFIG_CALL_THRU_SIP_PROXY		   ICF_CALL_CONFIG_CALL_THRU_SIP_PROXY
#define ICF_CFG_CC_CONFIG_PRIVACY_HEADER_PRESENT		   ICF_CALL_CONFIG_PRIVACY_HEADER_PRESENT
#define ICF_CFG_CC_CONFIG_PRACK_REQD					   ICF_CALL_CONFIG_PRACK_REQD


typedef icf_set_call_params_req_st icf_cfg_cc_set_call_params_req_st;

/*ICF_CRM_START*/
/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_network_activate_req_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_NETWORK_ACTIVATE_REQ 
 *
 *---------------------------------------------------------------------------*/


typedef icf_route_info_st               icf_route_info_st;

typedef icf_void_st                     icf_void_st;

typedef icf_network_activate_req_st     icf_network_activate_req_st;

/* ############################################################ */
/* ICF_CONF_STATUS_API_START */

typedef icf_get_system_settings_req_st  icf_get_system_settings_req_st;

typedef icf_get_system_settings_resp_st icf_get_system_settings_resp_st;

typedef icf_get_line_settings_req_st    icf_get_line_settings_req_st;

typedef icf_get_line_settings_resp_st   icf_get_line_settings_resp_st;

typedef icf_apn_list_st                 icf_apn_list_st;

/* ICF_CONF_STATUS_API_END */
/* ############################################################ */


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_network_info_st
 *
 * Description: This structure is used to return network info in  
 *               API ICF_NETWORK_ACTIVATE_CFM 
 *
 *---------------------------------------------------------------------------*/

#define ICF_NET_INFO_CTXT_ID_PRESENT         ICF_NET_INFO_CTXT_ID_PRESENT
#define ICF_NET_INFO_SELF_IP_PRESENT         ICF_NET_INFO_SELF_IP_PRESENT
#define ICF_NET_INFO_ACCESS_TYPE_PRESENT     ICF_NET_INFO_ACCESS_TYPE_PRESENT
#define ICF_NET_INFO_ACCESS_INFO_PRESENT     ICF_NET_INFO_ACCESS_INFO_PRESENT
#define ICF_NET_INFO_ASSO_URI_PRESENT        ICF_NET_INFO_ASSO_URI_PRESENT
#define ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT  ICF_NET_INFO_OUT_PROXY_ADDR_PRESENT 

    /* Denotes the known outbound proxy IP addresses. */
typedef icf_transport_list_st       icf_transport_list_st;

typedef icf_network_info_st         icf_network_info_st; 



/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_network_activate_cfm_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_NETWORK_ACTIVATE_CFM 
 *
 *---------------------------------------------------------------------------*/

#define ICF_NET_ACT_CFM_ERR_CAUSE_PRESENT  ICF_NET_ACT_CFM_ERR_CAUSE_PRESENT 
#define ICF_NET_ACT_CFM_NUM_NW_PRESENT     ICF_NET_ACT_CFM_NUM_NW_PRESENT 
#define ICF_NET_ACT_CFM_NW_INFO_PRESENT    ICF_NET_ACT_CFM_NW_INFO_PRESENT

typedef icf_network_activate_cfm_st     icf_network_activate_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_network_deactivate_req_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_NETWORK_DEACTIVATE_REQ 
 *
 *---------------------------------------------------------------------------*/

typedef icf_network_deactivate_req_st   icf_network_deactivate_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_network_deactivate_cfm_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_NETWORK_DEACTIVATE_CFM 
 *
 *---------------------------------------------------------------------------*/

#define ICF_NET_DEACT_CFM_ERR_CAUSE_PRESENT  ICF_NET_DEACT_CFM_ERR_CAUSE_PRESENT
#define ICF_NET_DEACT_CFM_NUM_NW_PRESENT     ICF_NET_DEACT_CFM_NUM_NW_PRESENT
#define ICF_NET_DEACT_CFM_NW_INFO_PRESENT    ICF_NET_DEACT_CFM_NW_INFO_PRESENT

typedef icf_network_deactivate_cfm_st   icf_network_deactivate_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_app_add_req_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_APP_ADD_REQ 
 *
 *---------------------------------------------------------------------------*/

typedef icf_app_add_req_st              icf_app_add_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_app_add_cfm_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_APP_ADD_CFM 
 *
 *---------------------------------------------------------------------------*/

#define ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT  ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT
#define ICF_APP_ADD_CFM_NUM_NW_PRESENT       ICF_APP_ADD_CFM_NUM_NW_PRESENT
#define ICF_APP_ADD_CFM_NET_INFO_PRESENT     ICF_APP_ADD_CFM_NET_INFO_PRESENT

typedef icf_app_add_cfm_st              icf_app_add_cfm_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:ippstk_res_unavail_ind_st
 *
 * Description: This structure is used to throttle messages from application
 * 		when ICF resources have reached nearly the limit. This is a 
 * 		mapping for API IPPSTK_RES_UNAVAIL_IND 
 *
 *---------------------------------------------------------------------------*/

typedef icf_res_unavail_ind_st              ippstk_res_unavail_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_app_remove_req_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_APP_REMOVE_REQ 
 *
 *---------------------------------------------------------------------------*/

typedef icf_app_remove_req_st           icf_app_remove_req_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_app_remove_cfm_st
 *
 * Description: This structure is used to specifies payload data of 
 *               API ICF_APP_REM_CFM 
 *
 *---------------------------------------------------------------------------*/

typedef icf_app_remove_cfm_st           icf_app_remove_cfm_st;


/*-----------------------------------------------------------------------------
 * Structure Name: icf_event_indication_st
 *
 * Description: This structure is used to indicate the ICF system/Network events 
 *               to the application
 *
 *---------------------------------------------------------------------------*/

typedef icf_event_indication_st         icf_event_indication_st;
/*ICF_CRM_END*/


/*-----------------------------------------------------------------------------
 * Structure Name: icf_cfg_cc_set_self_id_params_req_st
 *
 * Description: This structure is used to specifies payload data of
 *               API ICF_CFG_CC_SET_SELF_ID_PARAMS_REQ
 *
 *---------------------------------------------------------------------------*/

#define  ICF_CFG_CC_SET_SLF_ID_PRM_ADDR_LIST             ICF_SET_SLF_ID_PRM_ADDR_LIST
#define  ICF_CFG_CC_SET_SLF_ID_PRM_NAME_LIST             ICF_SET_SLF_ID_PRM_NAME_LIST
#define  ICF_CFG_CC_SET_SLF_ID_PRM_DEFAULT_SETTINGS_LIST ICF_SET_SLF_ID_PRM_DEFAULT_SETTINGS_LIST

#define ICF_SETTINGS_CONFIG_PRESENTATION_IND             ICF_SETTINGS_CONFIG_PRESENTATION_IND

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

#define ICF_MGMT_PLANE_AUTH_DATA_PRESENT	ICF_MGMT_PLANE_AUTH_DATA_PRESENT

typedef icf_set_mgmt_plane_auth_param_req_st icf_set_mgmt_plane_auth_param_req_st; 



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_create_secure_conn_req_st
 *
 * Description: This API is used by Application to request ICF to set up a 
 *				secure TCP connection to peer.  This connection can be used 
 *				either for XCAP or for SyncML messages. 
 *
 *---------------------------------------------------------------------------*/

#define ICF_XCAP_USAGE			ICF_XCAP_USAGE	
#define ICF_SYNCML_USAGE			ICF_SYNCML_USAGE

typedef icf_create_secure_conn_req_st		icf_create_secure_conn_req_st;
				
				

/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_create_secure_conn_resp_st
 *
 * Description: This API is used by ICF to send a secure connection set-up 
 *				response.
 *
 *---------------------------------------------------------------------------*/
			
typedef icf_create_secure_conn_resp_st		icf_create_secure_conn_resp_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_close_secure_conn_req_st
 *
 * Description: This API is used by Application to close an established secure 
 *				connection.
 *
 *---------------------------------------------------------------------------*/

typedef icf_close_secure_conn_req_st		icf_close_secure_conn_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_close_secure_conn_resp_st
 *
 * Description: This API is used by ICF to update the application about the 
 *				status of a previously issued Close Connection Request.
 *
 *---------------------------------------------------------------------------*/

typedef icf_close_secure_conn_resp_st		icf_close_secure_conn_resp_st;



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

#define ICF_SECURE_CONN_CLOSED_BY_PEER		ICF_SECURE_CONN_CLOSED_BY_PEER
#define ICF_SECURE_CONN_TIME_OUT				ICF_SECURE_CONN_TIME_OUT
#define ICF_REATTEMPTED_SECURE_CONNECTION	ICF_REATTEMPTED_SECURE_CONNECTION

typedef icf_secure_conn_status_ind_st		icf_secure_conn_status_ind_st;



/*----------------------------------------------------------------------------
 *
 *  icf_xcap_res_et - This data type is used to define value of type 
 *					  icf_uint8_t that indicates the resource type of 
 *					  MPH request.
 *
 *--------------------------------------------------------------------------*/

#define ICF_XCAP_RES_DOC			ICF_XCAP_RES_DOC
#define ICF_XCAP_RES_ELEM		ICF_XCAP_RES_ELEM
#define ICF_XCAP_RES_ATTR		ICF_XCAP_RES_ATTR

typedef icf_xcap_res_et			icf_xcap_res_et;



/*----------------------------------------------------------------------------
 *
 *  icf_stretchable_hdr_st - This data type is used to define a stretchable 
 *							 header for which length is indicated.
 *
 *--------------------------------------------------------------------------*/

typedef icf_stretchable_hdr_st		icf_stretchable_hdr_st;


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

#define ICF_NO_RESP_DURATION_PRESENT			ICF_NO_RESP_DURATION_PRESENT
#define ICF_ACCEPT_PRESENT					ICF_ACCEPT_PRESENT
#define ICF_IF_MATCH_PRESENT					ICF_IF_MATCH_PRESENT
#define ICF_IF_NONE_MATCH_PRESENT			ICF_IF_NONE_MATCH_PRESENT
#define ICF_ACCEPT_ENCODING_PRESENT			ICF_ACCEPT_ENCODING_PRESENT	
#define ICF_CONTENT_ENCODING_PRESENT			ICF_CONTENT_ENCODING_PRESENT
#define ICF_HIGH_PRIORITY_REQ				ICF_HIGH_PRIORITY_REQ
#define ICF_HEADER_LIST_PRESENT				ICF_HEADER_LIST_PRESENT
#define ICF_ENTITY_TAG_PRESENT				ICF_ENTITY_TAG_PRESENT
#define ICF_CONTENT_TYPE_PRESENT				ICF_CONTENT_TYPE_PRESENT

typedef  icf_create_replace_res_req_st		icf_create_replace_res_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_delete_res_req_st
 *
 * Description: This API is used by the application to delete an XCAP resource 
 *				on the document server. 
 *
 *---------------------------------------------------------------------------*/

typedef  icf_delete_res_req_st		icf_delete_res_req_st;
	


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_fetch_res_req_st
 *
 * Description: This API is used by the application to fetch an XCAP resource 
 *				on the document server. 
 *
 *---------------------------------------------------------------------------*/
	
typedef  icf_fetch_res_req_st		icf_fetch_res_req_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_syncml_send_req_st
 *
 * Description: This API is used by application to trigger ICF to send a HTTP 
 *				POST request with application supplied SyncMl message body 
 *				to Device Management server. 
 *
 *---------------------------------------------------------------------------*/

typedef  icf_syncml_send_req_st			icf_syncml_send_req_st;



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
	
typedef  icf_api_proc_failure_st		icf_api_proc_failure_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_mgmt_plane_nw_resp_st
 *
 * Description: This API is used by ICF to convey a received response from 
 *				server to application.  
 *
 *---------------------------------------------------------------------------*/
	
typedef  icf_mgmt_plane_nw_resp_st		icf_mgmt_plane_nw_resp_st;


/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_pa_types.h
 ******************************************************************************
 ******************************************************************************
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * typedef Name : icf_call_clear_reason_t
 * 
 *  Description  : This data type is used to define a data ipppstk_uint8_t type,
 *                 which identifies reason for clearing the call.
 *   
 -----------------------------------------------------------------------------*/

typedef icf_call_clear_reason_t icf_call_clear_reason_t;    


/*-----------------------------------------------------------------------------
 *
 * typedef Name : icf_display_id_t
 * 
 *  Description  : This data type is used to define a value of type 
 *                 icf_uint16_t that identifies the display information, 
 *                 which needs to be displayed on phone.  
 *   
-----------------------------------------------------------------------------*/

typedef icf_display_id_t icf_display_id_t;


/*------------------------------------------------------------------------------
 * Description: Values for data type  icf_display_id_t
 *----------------------------------------------------------------------------*/

#define ICF_DISPLAY_UNDEFINED                       ICF_DISPLAY_UNDEFINED

/* Indicates a re-registration is done for a previously unregistered user. 
 * display_data will contain one integer and one string. String value will 
 * contain user id for which registration is done and integer value will contain 
 * the line id for which the user is configured.*/
#define ICF_DISPLAY_USER_REGISTERED                 ICF_DISPLAY_USER_REGISTERED

/* Indicates a re-registration is done for a previously unregistered user. 
 * display_data will contain one integer and one string. String value will 
 * contain user id for which registration is done and integer value will 
 * contain the line id for which the user is configured.*/
#define ICF_DISPLAY_USER_NOT_REGISTERED             ICF_DISPLAY_USER_NOT_REGISTERED

/* Indicates that deregistration is successful */
#define ICF_DISP_USER_DEREGISTERED				   ICF_DISP_USER_DEREGISTERED					

/* This will be sent by CC when a privously requested call mute request is 
 * failed */
#define ICF_DISP_CALL_MUTE_FAILURE				   ICF_DISP_CALL_MUTE_FAILURE

/* This will be sent by CC when a privously requested call unmute request is 
 * failed */
#define ICF_DISP_CALL_UNMUTE_FAILURE				   ICF_DISP_CALL_UNMUTE_FAILURE



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



/*------------------------------------------------------------------------------
*
* Name : icf_display_data_st
*
* Description  : This data type is used to define a structure,
*               which contains display data to be displayed in the API 
*               ICF_CC_PA_DISPLAY_REQ.             
-----------------------------------------------------------------------------*/

typedef icf_display_data_st icf_display_data_st;

 
/*-----------------------------------------------------------------------------
 *
 * NAME : icf_call_info_st
 *
 * DESCRIPTION  : This structure
 *              specifies the party address and the time when the call is done
 *
 ----------------------------------------------------------------------------*/
/*typedef icf_call_info_st icf_call_info_st;*/
    




/*-----------------------------------------------------------------------------
 ******************************************************************************
 ******************************************************************************
 Mapping for icf_pa_api.h
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

#define ICF_TYPE_OF_CALL_IND_PRESENT				ICF_TYPE_OF_CALL_IND_PRESENT
#define ICF_CALL_WAITING_IND_PRESENT				ICF_CALL_WAITING_IND_PRESENT
#define	ICF_CALLING_ADD_ASRTD_IND_PRESENT		ICF_CALLING_ADD_ASRTD_IND_PRESENT
#define ICF_CC_PA_INC_CALL_CLG_PTY_ADDR          ICF_CALLING_PARTY_ADD_PRESENT
#define ICF_CC_PA_INC_CALL_CLG_PTY_NAME          ICF_CALLING_PARTY_NAME_PRESENT
#define ICF_CALLED_PARTY_ADD_PRESENT				ICF_CALLED_PARTY_ADD_PRESENT
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
* NAME : icf_pa_cc_create_call_req_st
*
* DESCRIPTION  : This data type is used to define a structure, which specifies 
*                payload data of API ICF_PA_CC_CREATE_CALL_REQ. 
-----------------------------------------------------------------------------*/

#define ICF_PA_CC_CREATE_CALL_REQ_CALLED_PARTY_NAME             ICF_CALLED_PARTY_NAME_PRESENT
#define ICF_PA_CC_CREATE_CALL_REQ_TOGGLE_PRESENTATION_INDICATOR ICF_TOGGLE_PRES_IND_PRESENT
#define ICF_PA_CC_CREATE_CALL_REQ_QOS_MARK                      ICF_QOS_MARK_PRESENT

typedef icf_create_call_req_st icf_pa_cc_create_call_req_st;

//ICF_HANDLE_REINVITE_START
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
*                payload data of API  ICF_CC_PA_CALL_MODIFY_CFM.
-----------------------------------------------------------------------------*/

typedef icf_call_modify_cfm_st icf_pa_cc_call_modify_cfm_st;

//ICF_HANDLE_REINVITE_END

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
 * NAME : icf_media_connected_ind_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                payload data of API ICF_CC_PA_MEDIA_CONNECTED_IND. 
 *                
 ----------------------------------------------------------------------------*/

typedef icf_media_connected_ind_st icf_media_connected_ind_st;


/*-----------------------------------------------------------------------------
 *
 * NAME : icf_connect_ind_st
 *
 * DESCRIPTION  : This data type is used to define a structure, which specifies
 *                payload data of API ICF_CC_PA_CONNECT_IND.
 *                
 ----------------------------------------------------------------------------*/

#define ICF_ASSERTED_ID_PRESENT					ICF_ASSERTED_ID_PRESENT
#define ICF_CC_PA_CONNECT_IND_CONTACT_HEADER		ICF_CONNECT_IND_CONTACT_HEADER

typedef icf_connect_ind_st icf_connect_ind_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:  icf_uri_list_st 
 *
 * Description: This datatype is used to contain list of URIs
 *
 *---------------------------------------------------------------------------*/
typedef icf_uri_list_st icf_uri_list_st;



/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_header_name_list_st
 *
 * Description: This datatype represents a structure that contains a 
 *                list of SIP headers.
 *
 *---------------------------------------------------------------------------*/
typedef icf_header_name_list_st icf_header_name_list_st;


/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_header_name_st
 *
 * Description: This datatype represents a structure that contains a 
 *                list of SIP headers.
 *
 *---------------------------------------------------------------------------*/
typedef icf_header_name_st icf_header_name_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_string_list_st
 *
 * Description: This datatype represents a structure that contains a 
 *                list of strings of type icf_string_st.
 *
 *---------------------------------------------------------------------------*/
typedef icf_string_list_st icf_string_list_st;

/*-----------------------------------------------------------------------------
 *
 * Structure Name:icf_string_list_node_st
 *
 * Description: This datatype represents a structure that contains a 
 *                list of strings of type icf_string_st.
 *
 *---------------------------------------------------------------------------*/
typedef icf_string_list_node_st icf_string_list_node_st;

/*----------------------------------------------------------------------------
 *
 *  icf_transport_mode_et These values specifies the mode of
 *   transport as TCP or UDP or both
 *
 *--------------------------------------------------------------------------*/
typedef enum{  
/* Specifies transport mode is UDP. */
ICF_TRANSPORT_TYPE_UDP = ICF_TRANSPORT_TYPE_UDP,

/* Specifies transport mode is TCP. */
ICF_TRANSPORT_TYPE_TCP = ICF_TRANSPORT_TYPE_TCP,

/* Specifies that transport could be either TCP or UDP. */
ICF_TRANSPORT_MODE_BOTH	 = ICF_TRANSPORT_MODE_BOTH
}icf_transport_mode_et;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name: icf_codec_t
 * 
 * Description:  This is used to define the identifier for a codec.
 *                                         
 *---------------------------------------------------------------------------*/

typedef icf_codec_t icf_codec_t;

typedef icf_trace_nw_buff_ind_st    icf_trace_nw_buff_ind_st;

#define ICF_DUMMY_APP_ID	     ICF_DUMMY_APP_ID

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

/*Presence flag of icf_address_st*/
#define ICF_ADDRESS_MODE              ICF_ADDRESS_MODE
#define ICF_ADDRESS_PHONE_CONTEXT     ICF_ADDRESS_PHONE_CONTEXT  
#define ICF_ADDRESS_USER_PARAM        ICF_ADDRESS_USER_PARAM     
#define ICF_ADDRESS_TRANSPORT         ICF_ADDRESS_TRANSPORT      
#define ICF_ADDRESS_UNKNOWN_PARAMS    ICF_ADDRESS_UNKNOWN_PARAMS 

typedef icf_conference_req_st  icf_pa_cc_conference_req_st;
#endif



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

typedef icf_tone_id_t icf_tone_id_t;
    
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


/*-----------------------------------------------------------------------------
 *
 * NAME : icf_media_session_req_st
 *
 * DESCRIPTION  :  This data type is used to define a structure,which specifies
 *                 payload data of API ICF_MEDIA_SESSION_REQ. 
 *                 
-----------------------------------------------------------------------------*/

typedef icf_media_session_req_st ippstk_media_session_req_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__ICF_ICF_MAP_TYPES_H__*/
 
