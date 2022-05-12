/*------------------------------------------------------------------------------
 *
 * File name        : icf_port_pltfm_types.h
 *
 * Purpose          : This file contains the datatypes used at Wrapper interface
 *
 * Revision History :
 *
 *   Date       Name			Ref#		Comments
 * --------     ------------	----		---------------
 * 12-Dec-2005	Abhishek/Rohit	ICF 4.0 CDD	Initial
 * 12-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD Code changes for IPSec support
 * 13-Jan-2006  Aman Arora		ICF 4.1 	removed IPSEC releated feature flags
 * 
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_PORT_PLTFM_TYPES_H__
#define __ICF_PORT_PLTFM_TYPES_H__


#include "icf_types.h"
#include "icf_port_pltfm_defs.h"

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

typedef icf_uint32_t	icf_port_pltfm_sim_type_t;

typedef icf_uint32_t	icf_port_pltfm_device_params_req_st;
typedef icf_uint32_t	icf_port_pltfm_access_network_req_st;
typedef icf_uint32_t	icf_port_pltfm_fetch_enc_algo_req_st;
typedef icf_uint32_t	icf_port_pltfm_fetch_auth_algo_req_st;
typedef icf_string_st	icf_port_pltfm_host_to_ip_addr_req_st;
typedef icf_string_st	icf_port_pltfm_host_to_ip_addr_resp_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_device_params_resp_st
 *
 * Description :  
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
	icf_port_pltfm_sim_type_t	sim_type;
	icf_uint8_t					mncLen;
	icf_string_st				imsi;
	icf_uint32_t				num_of_apns; 
	icf_string_st				apn_list[ICF_PORT_PLTFM_MAX_APNS];
	/* Optional fields*/
	/*Home network domain name*/
	icf_string_st				home_nw_domain;
	/*Private user id*/
	icf_string_st				private_user_id; 
	/*Number of public URIs*/
	icf_uint32_t				num_public_uris; 
	/*List of public URIs*/
	icf_string_st				public_user_id[ICF_PORT_PLTFM_MAX_PUBLIC_USER_IDS];
}icf_port_pltfm_device_params_resp_st;


typedef struct 
{
	icf_uint8_t					nw_type; 
	/*Mobile Country Code*/
	icf_uint16_t				mcc;
	/*Mobile Network Code*/
	icf_uint16_t				mnc;
	/*Location Area Code*/
	icf_uint16_t				lac;
	/*Cell Id*/
	icf_string_st				cell_id;
} icf_port_pltfm_access_network_resp_st;

typedef struct
{
	/*Random number came from NW*/
	icf_string_st		rand;
	/*Parameter used by SIM to authenticate the NW*/
	icf_string_st		autn;

} icf_port_pltfm_aka_challenge_req_st;



typedef icf_uint16_t  icf_port_pltfm_aka_result_t;

typedef struct 
{
	icf_port_pltfm_aka_result_t 	    status;

	/*Response value calculated by SIM*/
	icf_string_st						res;

	/*IMS Ciphering Key for use in message encryption*/
	icf_string_st						imsCk;

	/*IMS Integrity Key for use in message authentication*/
	icf_string_st						imsIk;

	/*Present only if authentication is successful but the authentication 
	 sequence number is out of range*/
	icf_string_st						auts;

} icf_port_pltfm_aka_response_st;

typedef icf_uint16_t icf_port_pltfm_qos_traffic_class_t;
typedef icf_uint16_t icf_port_pltfm_qos_delivery_order_t;
typedef icf_uint16_t icf_port_pltfm_qos_delivery_of_erroneous_sdu_t;
typedef icf_uint16_t icf_port_pltfm_qos_max_sdu_size_t;
typedef icf_uint16_t icf_port_pltfm_qos_residual_ber_t;
typedef icf_uint16_t icf_port_pltfm_qos_sdu_error_ratio_t;
typedef icf_uint16_t icf_port_pltfm_qos_transfer_delay_t;
typedef icf_uint16_t icf_port_pltfm_qos_traffic_handling_priority_t;
typedef icf_uint16_t icf_port_pltfm_qos_guaranteed_bit_rate_ul_t;
typedef icf_uint16_t icf_port_pltfm_qos_guaranteed_bit_rate_dl_t;
typedef icf_uint16_t icf_port_pltfm_qos_signaling_indication_t;
typedef icf_uint16_t icf_port_pltfm_qos_source_statistics_descriptor_t;
typedef icf_uint16_t icf_port_pltfm_qos_maximum_bit_rate_ul_t;
typedef icf_uint16_t icf_port_pltfm_qos_maximum_bit_rate_dl_t;

typedef struct	
{
	icf_port_pltfm_qos_traffic_class_t						traffic_class;
	icf_port_pltfm_qos_delivery_order_t						delivery_order ;
	icf_port_pltfm_qos_delivery_of_erroneous_sdu_t			delivery_of_erroneous_sdu;
	icf_port_pltfm_qos_max_sdu_size_t						max_sdu_size;
	icf_port_pltfm_qos_residual_ber_t						residual_ber;
	icf_port_pltfm_qos_sdu_error_ratio_t					sdu_error_ratio;
	icf_port_pltfm_qos_transfer_delay_t  					transfer_delay;
	icf_port_pltfm_qos_traffic_handling_priority_t  		traffic_handling_priority;
	icf_port_pltfm_qos_guaranteed_bit_rate_ul_t  			guaranteed_bit_rate_ul;
	icf_port_pltfm_qos_guaranteed_bit_rate_dl_t  			guaranteed_bit_rate_dl;
	icf_port_pltfm_qos_signaling_indication_t  				signaling_indication;
	icf_port_pltfm_qos_source_statistics_descriptor_t  		source_statistics_descriptor;
	icf_port_pltfm_qos_maximum_bit_rate_ul_t  				maximum_bit_rate_ul;
	icf_port_pltfm_qos_maximum_bit_rate_dl_t 				maximum_bit_rate_dl;
}icf_port_pltfm_qos_profile_st;

/* component type as in TFT packet filter contents */
typedef icf_uint8_t icf_port_pltfm_pkt_filter_component_type_t; 

typedef struct 
{
	icf_port_pltfm_pkt_filter_component_type_t	pkt_filter_component_type;
	icf_uint16_t	pkt_filter_component_value;
}icf_port_pltfm_pkt_filter_contents_st;

typedef struct 
{
	icf_uint8_t	pkt_filter_id;
	icf_uint8_t	pkt_filter_eval_prec;
	icf_port_pltfm_pkt_filter_contents_st	pkt_filter_contents[ICF_PORT_PLTFM_MAX_TFT_PKT_FILTER_CONTENTS];
}icf_port_pltfm_pkt_filter_list_st;

typedef struct 
{
	icf_uint8_t	num_of_octets;
	icf_uint8_t	octets[ICF_PORT_PLTFM_MAX_AUTH_TOKEN_OCTET];
}icf_port_pltfm_auth_token_st;

typedef struct 
{
	icf_uint16_t	 media_component_id;
	icf_uint16_t	 ip_flow_id;
}icf_port_pltfm_flow_id_st;

typedef union
{
		icf_port_pltfm_auth_token_st	    auth_token; /* Authorization Token */
		icf_port_pltfm_flow_id_st  			flow_id;
}icf_port_pltfm_params_ut;

typedef struct 
{
	icf_uint8_t							parameter_identifier;
	icf_port_pltfm_params_ut			parameter;
}icf_port_pltfm_params_list_st;

typedef icf_uint16_t  icf_port_pltfm_tft_operation_code;

typedef struct
{
	icf_port_pltfm_tft_operation_code				tft_operation_code;
	icf_uint8_t										e_bit;
	icf_uint8_t										num_pkt_filters;
	icf_port_pltfm_pkt_filter_list_st				pkt_filter_list[ICF_PORT_PLTFM_MAX_TFT_PKT_FILTER_LIST];
	icf_uint8_t										num_params;
	icf_port_pltfm_params_list_st			params_list[ICF_PORT_PLTFM_MAX_TFT_PARAMS_LIST];
}icf_port_pltfm_tft_st;

typedef icf_uint16_t  icf_port_pltfm_pdp_cntxt_type_t;
typedef icf_uint16_t  icf_port_pltfm_d_comp_t;
typedef icf_uint16_t  icf_port_pltfm_h_comp_t;
typedef icf_uint16_t  icf_port_pltfm_pdp_pdp_type_t;
typedef icf_uint16_t  icf_port_pltfm_protocol_config_opt_type_t;
typedef icf_uint16_t  icf_port_pltfm_gprs_error_t;

typedef struct 
{
	/*Context Id*/
	icf_uint32_t			context_id;

	/*Id of Primary PDP context of this sec. PDP cotnext*/
	icf_uint32_t			primary_id;

	/*List of TFT filtees*/
	icf_port_pltfm_tft_st			tft;
} icf_port_pltfm_pdp_secondary_cntxt_info_st;

typedef struct
{
	/*Context Id*/
	icf_uint32_t					context_id;

	/*PDP Type*/
	icf_port_pltfm_pdp_pdp_type_t	pdp_type;

	/*APN for the PDP Context*/
	icf_string_st					apn;

	/*Address associated with PDP context*/
	icf_string_st					pdp_addr; 

} icf_port_pltfm_pdp_primary_cntxt_info_st;

typedef union
{
	icf_port_pltfm_pdp_primary_cntxt_info_st		primary;
	icf_port_pltfm_pdp_secondary_cntxt_info_st	    secondary;
}icf_port_pltfm_pdp_cntxt_info_st;

typedef struct 
{
	icf_port_pltfm_protocol_config_opt_type_t	type;
	icf_string_st								value;
}icf_port_pltfm_pdp_prot_config_options_st;

typedef struct
{
	icf_port_pltfm_pdp_cntxt_type_t		 		 pdp_context_type;
	icf_port_pltfm_pdp_cntxt_info_st			 pdp_context_info;	
	/* Optional Parameters*/
	icf_boolean_t								 dcomp_present;
	icf_port_pltfm_d_comp_t						 dcomp;
	icf_boolean_t								 hcomp_present;
	icf_port_pltfm_h_comp_t						 hcomp;
	icf_boolean_t								 qos_profile_present;
	icf_port_pltfm_qos_profile_st				 qos_profile; 
	icf_boolean_t								 min_qos_present;
	icf_port_pltfm_qos_profile_st				 min_qos_profile; 
	icf_uint32_t							     num_pdp_var_params;
	icf_port_pltfm_pdp_prot_config_options_st    pdp_var_params[ICF_PORT_PLTFM_MAX_PDP_VAR_PARAMS];
}icf_port_pltfm_pdp_def_req_st;

typedef struct
{
	/* mandatory parameters*/
	icf_port_pltfm_gprs_error_t 				status;
	/*Context Id*/
	icf_uint32_t								context_id;
	/* Optional Parameters */
	icf_boolean_t								pdp_addr_present;
	icf_string_st								pdp_addr; 
	icf_boolean_t								qos_profile_present;
	icf_port_pltfm_qos_profile_st				qos_profile_rsp; 
	icf_boolean_t								pcscf_addr_present;

#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
	icf_boolean_t								dhcp_addr_present;
	icf_boolean_t								dns_addr_present;
#endif
	icf_string_st								pcscf_addr; 

#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
	icf_string_st								dhcp_addr; 
	icf_string_st								dns_addr; 
#endif
}icf_port_pltfm_pdp_def_resp_st;

typedef struct
{
	icf_uint32_t	id;
	void*			payload;
	
}icf_port_pltfm_qos_res_non_ims_req_st;

typedef struct
{
	icf_uint32_t					id;
	icf_boolean_t 				status;

}icf_port_pltfm_qos_res_non_ims_resp_st;


typedef struct
{
	icf_uint32_t							 context_id;
}icf_port_pltfm_pdp_deact_req_st;


typedef struct
{
	/* mandatory parameters*/
	icf_port_pltfm_gprs_error_t 			 status;
	icf_uint32_t							 context_id;
}icf_port_pltfm_pdp_deact_rsp_st;

typedef struct
{
	icf_uint32_t							 id;
}icf_port_pltfm_qos_rel_non_ims_req_st;


typedef struct
{
	icf_uint32_t						id;
	icf_boolean_t 						status;

}icf_port_pltfm_qos_rel_non_ims_rsp_st;

typedef icf_port_pltfm_pdp_deact_rsp_st icf_port_pltfm_pdp_deact_ind_st;

typedef icf_port_pltfm_pdp_deact_rsp_st icf_port_pltfm_pdp_mod_ind_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_port_pltfm_network_event_t
 * Description      : Possible network events.
 *						
 *----------------------------------------------------------------------------*/
typedef icf_uint32_t			icf_port_pltfm_network_event_t;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_port_pltfm_event_ind_st
 * Description      : 
 *						
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_port_pltfm_network_event_t	event;
	void								*event_data;
}icf_port_pltfm_event_ind_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_ret_val_t
 *
 * Description :  Return value - may be Success/Failure/Pending
 *
 *----------------------------------------------------------------------------*/

typedef icf_uint8_t icf_port_pltfm_ret_val_t;


/*------------------------------------------------------------------------------
 * Structures needed for IPSec Support
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_fetch_algo_resp_st
 * Description :  List of encryption and integrity algorithms
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* number of locally supported integrity algorithms */
    icf_uint8_t                         num_auth_algo;

    /* list of locally supported integrity algorithms */
    icf_string_st                       integrity_algo[ICF_MAX_SUPP_I_ALGO];
    
    /* number of locally supported encryption algorithms */
    icf_uint8_t                         num_enc_algo;

    /* list of locally supported encryption algorithms */
    icf_string_st                       encryption_algo[ICF_MAX_SUPP_E_ALGO];
    
} icf_port_pltfm_fetch_algo_resp_st;

typedef icf_uint8_t	icf_port_pltfm_ipsec_protocol_t;
typedef icf_uint8_t	icf_port_pltfm_ipsec_protocol_mode_t;
typedef icf_uint8_t	icf_port_pltfm_ipsec_enc_mode_t;
typedef icf_uint8_t	icf_port_pltfm_ipsec_direction_t;
	

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sa_create_req_st
 * Description :  Structure for SA create request from ICF to Wrapper
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* UE's IP address and protected port */
    icf_transport_address_st            ue_ip_port;

    /* P-CSCF's IP address and protected port */
    icf_transport_address_st            proxy_ip_port;

    /* UE generated spi value identifying the SA */
    icf_uint32_t                        spi;

    /* lifetime for the new SA */
    icf_duration_t                      lifetime;

    icf_port_pltfm_ipsec_direction_t    direction;

    /* encryption algorithm to be used */
    icf_uint8_t                         enc_algo[ICF_MAX_STR_LEN];

    /* encryption key and length */
    icf_string_st                       enc_key;

    /* integrity algorithm to be used */
    icf_uint8_t                         auth_algo[ICF_MAX_STR_LEN];

    /* integrity password and length */
    icf_string_st                       auth_passwd;
        
    /* protocol - AH or ESP; ESP to be used */
    icf_port_pltfm_ipsec_protocol_t     protocol;
    
  
    icf_port_pltfm_ipsec_protocol_mode_t protocol_mode;
    
    icf_port_pltfm_ipsec_enc_mode_t    enc_mode;
    
    /* duration of key validity in seconds */
    icf_duration_t                      key_life;
    
    /* number of mask bits if using Tunnel mode */
    icf_uint32_t                        mask_bits;

} icf_port_pltfm_sa_create_req_st;

/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sa_modify_req_st
 * Description :  Structure for SA modify request from ICF to Wrapper
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has to be modified */
    icf_uint32_t                        spi;

    /* new duration for the SA */
    icf_duration_t                      lifetime;
    
} icf_port_pltfm_sa_modify_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sa_delete_req_st
 * Description :  Structure for SA delete request from ICF to Wrapper
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has to be deleted */
    icf_uint32_t                        spi;
    
} icf_port_pltfm_sa_delete_req_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sa_cmd_resp_st
 * Description :  Structure for SA create/modify/delete req from Wrapper to ICF
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA for which the response is received */
    icf_uint32_t                        spi;
    
    /* response for the SA specific operation - can be Success or Failure */
    icf_return_t	                    response;

} icf_port_pltfm_sa_cmd_resp_st;


/*------------------------------------------------------------------------------
 *
 * Type Name   :  icf_port_pltfm_sa_expiry_ind_st
 * Description :  Structure for SA expiry indication from Wrapper to ICF
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* spi of SA that has expired */
    icf_uint32_t                        spi;

} icf_port_pltfm_sa_expiry_ind_st;



#ifdef __cplusplus
}

#endif /* __cplusplus */
#endif /* __ICF_PORT_WRP_TYPES_H__ */

