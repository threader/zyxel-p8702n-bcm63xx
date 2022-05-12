/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_pltfm_defs.h
 *
 * Purpose          : This file contains the constants used at Wrapper interface
 *
 * Revision History :
 *
 *   Date       Name			Ref#		Comments
 * --------     ------------	----		---------------
 * 12-Dec-2005	Abhishek/Rohit	ICF 4.0 CDD	Initial
 * 
 *                                        
 * Copyright (c) 2006, Aricent .
 *----------------------------------------------------------------------------*/


#ifndef __ICF_PORT_PLTFM_DEFS_H__
#define __ICF_PORT_PLTFM_DEFS_H__

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


/*------------------------------------------------------------------------------
 * Type Name   :  icf_port_pltfm_ret_val_t
 *----------------------------------------------------------------------------*/
#define ICF_PORT_PLTFM_FAILURE					0
#define ICF_PORT_PLTFM_SUCCESS					1
#define ICF_PORT_PLTFM_PENDING					2


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sim_type_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

#define ICF_PORT_PLTFM_EMIM2GSIM				0 
#define ICF_PORT_PLTFM_EMIM25GSIM				1
#define ICF_PORT_PLTFM_EMIM3GUSIM				2
#define ICF_PORT_PLTFM_EMIM3GISIM				3



#define ICF_PORT_PLTFM_MAX_APNS					4
#define ICF_PORT_PLTFM_MAX_PUBLIC_USER_IDS		5
#define ICF_PORT_PLTFM_MAX_PCSCF_ADDRS			2

#define	ICF_PORT_PLTFM_AKA_SUCCESSFUL			0
#define	ICF_PORT_PLTFM_AKA_SEQRANGEERR			1
#define ICF_PORT_PLTFM_AKA_UNAUTHNWERR			2
#define ICF_PORT_PLTFM_AKA_AKASYSTEMERR			3
#define ICF_PORT_PLTFM_AKA_AKANULL				4

#define ICF_PORT_PLTFM_PRIMARY_PDP_CNTXT		0 
#define ICF_PORT_PLTFM_SECONDARY_PDP_CNTXT		1 

#define ICF_PORT_PLTFM_PDP_TYPE_IPV4			0 
#define ICF_PORT_PLTFM_PDP_TYPE_IPV6			1 
#define ICF_PORT_PLTFM_PDP_TYPE_DEFAULT			2

#define  ICF_PORT_PLTFM_PDP_CREATE_NEW_TFT		   			1
#define  ICF_PORT_PLTFM_PDP_DELETE_EXISTING_TFT				2
#define  ICF_PORT_PLTFM_PDP_ADD_PKT_FILTERS_TFT				3
#define  ICF_PORT_PLTFM_PDP_REPLACE_PKT_FILTERS_TFT 		4
#define  ICF_PORT_PLTFM_PDP_DELETE_PKT_FILTERS_TFT 			5
#define  ICF_PORT_PLTFM_PDP_NO_TFT_OPERATION 				6

#define ICF_PORT_PLTFM_MAX_TFT_PKT_FILTER_LIST			    2
#define ICF_PORT_PLTFM_MAX_TFT_PARAMS_LIST				    3

#define ICF_PORT_PLTFM_MAX_AUTH_TOKEN_OCTET			    50

#define ICF_PORT_PLTFM_PDP_PARAM_IDENT_AUTH_TOKEN	 	    1
#define ICF_PORT_PLTFM_PDP_PARAM_IDENT_FLOW_ID 			    2
#define ICF_PORT_PLTFM_MAX_TFT_FLOW_IDS					    4

#define ICF_PORT_PLTFM_QOS_CONVERSATIONAL_TRAFFIC_CLASS		1
#define ICF_PORT_PLTFM_QOS_STREAMING_TRAFFIC_CLASS			2
#define ICF_PORT_PLTFM_QOS_INTERACTIVE_TRAFFIC_CLASS		3
#define ICF_PORT_PLTFM_QOS_BACKGROUND_TRAFFIC_CLASS			4
#define ICF_PORT_PLTFM_MAX_PDP_VAR_PARAMS					4

	
/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_qos_delivery_order_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

#define ICF_PORT_PLTFM_QOS_DELIVERY_ORDER_NO			0
#define ICF_PORT_PLTFM_QOS_DELIVERY_ORDER_YES			1
#define ICF_PORT_PLTFM_QOS_DELIVERY_ORDER_DETECT		2	
#define ICF_PORT_PLTFM_QOS_DELIVERY_ORDER_SUBSCRIBED	3
#define ICF_PORT_PLTFM_QOS_DELIVERY_ORDER_DEFAULT		4
	

#define ICF_PORT_PLTFM_DCOMP_OFF							0
#define ICF_PORT_PLTFM_DCOMP_ON								1
#define ICF_PORT_PLTFM_DCOMP_V42BIS							2
#define ICF_PORT_PLTFM_DCOMP_V44BIS							3
#define ICF_PORT_PLTFM_DCOMP_DEFAULT						4

#define ICF_PORT_PLTFM_HCOMP_OFF							0
#define ICF_PORT_PLTFM_HCOMP_ON								1
#define ICF_PORT_PLTFM_HCOMP_RFC1144						2
#define ICF_PORT_PLTFM_HCOMP_RFC2507						3
#define ICF_PORT_PLTFM_HCOMP_RFC3095						4
#define ICF_PORT_PLTFM_HCOMP_DEFAULT						5

#define ICF_PORT_PLTFM_PCSCF_ADDR							0
#define ICF_PORT_PLTFM_DNS_ADDR								1
#define ICF_PORT_PLTFM_IM_CN_SIG_FLAG						2

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_network_event_up_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

#define ICF_PORT_PLTFM_NW_FAILURE			0
#define ICF_PORT_PLTFM_NW_UP				1
#define ICF_PORT_PLTFM_NW_RECOVER			2

#define ICF_PORT_PLTFM_NW_NOT_AVAILABLE		3
#define ICF_PORT_PLTFM_NW_UNATTACHED		4
#define ICF_PORT_PLTFM_NW_ACTIVE			5
#define ICF_PORT_PLTFM_NW_SUSPENDED			6
#define ICF_PORT_PLTFM_NW_REG_NOT_AVAILABLE	7
#define ICF_PORT_PLTFM_NW_REG_UNKNOWN		8
#define ICF_PORT_PLTFM_NW_REG_NO_SERVICE	9
#define ICF_PORT_PLTFM_NW_REG_EMERGENCY_ONLY	10
#define ICF_PORT_PLTFM_NW_REG_SEARCHING		11
#define ICF_PORT_PLTFM_NW_REG_BUSY			12
#define ICF_PORT_PLTFM_NW_REG_DENIED		13

#define	ICF_PORT_PLTFM_SIM_INSERTED			14
#define ICF_PORT_PLTFM_SIM_REMOVED			15
#define ICF_PORT_PLTFM_GPRS_FAILURE			16	
#define ICF_PORT_PLTFM_GPRS_ON_HOLD			17
#define ICF_PORT_PLTFM_GPRS_RECOVED			18
#define ICF_PORT_PLTFM_NW_AC_INFO_CHANGE	19
#define ICF_PORT_PLTFM_SYS_POW_RES			20
#define ICF_PORT_PLTFM_SYS_MEM_RES			21	
#define ICF_PORT_PLTFM_NW_CHNGE				22


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_gprs_error_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

#define ICF_PORT_PLTFM_GPRS_NOERR							0
#define ICF_PORT_PLTFM_GPRS_SYTEM_ERR						1
#define ICF_PORT_PLTFM_GPRS_NW_CHNGE							2
#define ICF_PORT_PLTFM_GPRS_ILLEGAL_MS						3
#define ICF_PORT_PLTFM_GPRS_ILLEGAL_ME						4
#define ICF_PORT_PLTFM_GPRS_SERVICE_NOT_ALLOWED				5
#define ICF_PORT_PLTFM_GPRS_PLMN_NOT_ALLOWED					6
#define ICF_PORT_PLTFM_GPRS_LOCATION_NOT_ALLOWED				7
#define ICF_PORT_PLTFM_GPRS_ROAMING_NOT_ALLOWED				8
#define ICF_PORT_PLTFM_GPRS_OPTION_NOT_ALLOWED				9
#define ICF_PORT_PLTFM_GPRS_OPTION_NOT_SUBSCRIBED			10
#define ICF_PORT_PLTFM_GPRS_OPTION_TEMP_OUT_OF_ORDER			11
#define ICF_PORT_PLTFM_GPRS_AUTH_FAILURE						12
#define ICF_PORT_PLTFM_GPRS_INVALID_MOBILE_CLASS				13
#define ICF_PORT_PLTFM_GPRS_UNSPECIFIED_GPRS_ERR				14


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_ipsec_direction_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/
    /* direction - inbound or outbound */
#define ICF_PORT_PLTFM_IPSEC_SA_DIR_IN             0x01
#define ICF_PORT_PLTFM_IPSEC_SA_DIR_OUT            0x02

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_ipsec_protocol_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

#define ICF_PORT_PLTFM_IPSEC_PROTO_AH              0x01
#define ICF_PORT_PLTFM_IPSEC_PROTO_ESP             0x02

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_ipsec_enc_mode_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/
/* encryption mode - ECBC or EECB; ECBC to be used */
#define ICF_PORT_PLTFM_IPSEC_ENC_MODE_ECBC         0x01
#define ICF_PORT_PLTFM_IPSEC_ENC_MODE_EECB         0x02

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_ipsec_protocol_mode_t
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

/* protocol mode - Transport or Tunnel; Tunnel mode to be used */
#define ICF_PORT_PLTFM_IPSEC_PROT_MODE_TRANSPORT        0x01
#define ICF_PORT_PLTFM_IPSEC_PROT_MODE_TUNNEL           0x02

#define ICF_PORT_PLTFM_TFT_COMPONENT_IPV4_SOURCE_ADDRESS_TYPE			16
#define ICF_PORT_PLTFM_TFT_COMPONENT_IPV6_SOURCE_ADDRESS_TYPE			32
#define ICF_PORT_PLTFM_TFT_COMPONENT_PROTOCOL_IDENTIFIER				48
#define ICF_PORT_PLTFM_TFT_COMPONENT_SINGLE_DESTINATION_PORT_TYPE		64
#define ICF_PORT_PLTFM_TFT_COMPONENT_DESTINATION_PORT_RANGE_TYPE		65
#define ICF_PORT_PLTFM_TFT_COMPONENT_SINGLE_SOURCE_PORT_TYPE			84
#define ICF_PORT_PLTFM_TFT_COMPONENT_SINGLE_PORT_RANGE_TYPE				85
#define ICF_PORT_PLTFM_TFT_COMPONENT_SECURITY_PARAMETER_INDEX_TYPE		96
#define ICF_PORT_PLTFM_TFT_COMPONENT_TRAFFIC_CLASS_TYPE					112
#define ICF_PORT_PLTFM_TFT_COMPONENT_FLOW_LABEL_TYPE					128
#define ICF_PORT_PLTFM_MAX_TFT_PKT_FILTER_CONTENTS						2

#ifdef __cplusplus
}

#endif /* __cplusplus */
#endif /* __ICF_PORT_PLTFM_DEFS_H__ */

