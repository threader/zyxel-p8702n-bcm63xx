/*------------------------------------------------------------------------------
 *     
 * File name        : icf_nm_types.h
 *
 * Purpose          : This file contains the datatypes used by Network Manager(NM).
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 01-Aug-2005	Irfan Khan		LLD					Initial
 * 13-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 * 06-Jan-2006	Jyoti Narula	SPR 8063/ 9043		new members in icf_nm_aka_info_st to fetch Ik/Ck
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/
#ifndef __ICF_NM_TYPES_H__
#define __ICF_NM_TYPES_H__

/*#include "icf_common_interface_types.h"
#include "icf_common_defs.h"*/

#include "icf_nm_defs.h"
#include "icf_common_types.h"

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */

typedef enum 
{
	ICF_2GSIM,
	ICF_25GSIM,
	ICF_3GUSIM,
	ICF_3GISIM
}icf_nm_sim_type_et;

#ifdef ICF_NW_ACTIVATE
typedef 	icf_uint16_t 	icf_sec_pdp_state_st;

typedef struct __icf_sec_pdp_list_st
{
	icf_call_id_t					call_id ;
	icf_uint16_t					sec_pdps_count;
	icf_uint32_t					sec_pdp_ids[ICF_NM_MAX_SECONDARY_PDP_CONTEXTS];
	icf_sec_pdp_state_st			sec_pdp_states[ICF_NM_MAX_SECONDARY_PDP_CONTEXTS];
}icf_callid_sec_pdp_ctxts_map_st;

typedef struct __icf_nm_prim_sec_pdp_map_st
{
	icf_uint16_t					prim_pdp_id;
	icf_uint16_t					sec_pdp_id_counter;
	icf_uint16_t					number_of_calls;
	icf_callid_sec_pdp_ctxts_map_st   	* p_callid_sec_pdp_ctxts_map;
}icf_nm_prim_sec_pdp_map_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_nw_info_st
 * Description      : Information, corresponding to active primary PDP contexts
 *
 *----------------------------------------------------------------------------*/
typedef struct __icf_nm_nw_info_st
{
	/*PDP context id*/
	/* Currently Only One PDP Context is supported ..So we have only one here */
	icf_nm_prim_sec_pdp_map_st       pdp_cntxt_info;

	/*self ip address*/
	icf_transport_addr_st			self_ip;  
	
	/*Access Type e.g GPRS, WLAN*/
	icf_uint8_t						access_type;
	/*APN*/
	icf_string_st					nm_access_info;
	/*P-cscf addresses*/
	icf_transport_list_st			outbound_proxies;
#ifdef ICF_P_CSCF_DISCOVERY_ENABLED
	/*DNS addresses*/
	icf_transport_list_st			dns_addr;
	/*DHCP addresses*/
	icf_transport_list_st			dhcp_addr;
#endif
	
}icf_nm_nw_info_st;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_private_id_st
 * Description      : Private user identification 
 *
 *----------------------------------------------------------------------------*/
typedef struct{
	/*Type of id temperory/stored*/
	icf_uint8_t			type;
	/*value of private user id*/
	icf_string_st		value;
}icf_private_id_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_access_info_st
 * Description      : Network Access information 
 *
 *----------------------------------------------------------------------------*/
typedef struct __icf_nm_access_info_st
{
#ifdef ICF_NW_ACTIVATE
	/*Access Type e.g. GPRS, WLAN*/	
	icf_uint8_t				access_type;
#endif
	/*Home network domain name*/
	icf_string_st			home_nw_dmn;
#ifdef ICF_NW_ACTIVATE
	/*Number of APNS for above mentioned access type*/
	icf_uint32_t				nofapns;
	/*APN list*/
	icf_string_st			*p_apn_list;
#endif
	/*Number of public user ids*/
	icf_uint32_t				nofpbuids;
	/*Array of public user ids*/
	icf_string_st			*p_public_usr_id;
	/*Number of private user ids*/
	icf_uint32_t				nofprids;
	/*List of private user ids*/
	icf_private_id_st		*p_private_usr_id;
}icf_nm_access_info_st;

#ifdef ICF_IPSEC_ENABLED
typedef icf_port_pltfm_fetch_algo_resp_st icf_ipsec_self_algo_st;
#endif

#ifdef ICF_QOS_SUPPORT
typedef struct
{
	icf_uint16_t	id;
	icf_call_id_t	call_id;
}icf_nm_non_ims_qos_map_st;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_global_st
 * Description      : Global structure of Network Manager,created and destroyed 
 *						NM only.
 *
 *----------------------------------------------------------------------------*/
typedef struct __icf_nm_global_st
{
	/*Check for the validity of icf_nm_global_st pointer*/
	icf_uint32_t					chk;
	/*Pointer to global structure of ICF which has pointer to icf_nm_global_st*/
	icf_glb_pdb_st				*p_icf_glb_ptr;
#ifdef ICF_NW_ACTIVATE
	/*Pointer to the list of Network Informations	*/
	icf_nm_nw_info_st			*p_n_info_list;
#endif
	/*Pointer to list having all the information related to Network Access*/
	icf_nm_access_info_st		*p_acc_info;
	/*SIM type */
	icf_nm_sim_type_et			simType;
	/*IMSI*/
	icf_string_st				imsi;

#ifdef ICF_IPSEC_ENABLED
    /* locally supported security algorithms */
    icf_ipsec_self_algo_st       *p_ue_algo_list;
#endif /* ICF_IPSEC_ENABLED */

#ifdef ICF_QOS_SUPPORT
	icf_nm_non_ims_qos_map_st	 *p_non_ims_qos_map;
#endif
}icf_nm_global_st;

#ifdef ICF_NW_ACTIVATE
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_nw_info_st
 * Description      : Contains parameters to process network information request
 *						
 *----------------------------------------------------------------------------*/
typedef struct
{
	/*NM's global pointer*/
	icf_nm_global_st	*p_nm_ptr;
	icf_string_st    apn;
	/*Pointer of network information, return by NM to user*/
	icf_nm_nw_info_st *p_ninfo;
}icf_nm_nw_info_req_st;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_mbl_info_st
 * Description      : Contains parameters to process platform information request
 *						
 *----------------------------------------------------------------------------*/
typedef struct
{
	/*NM's global pointer*/
	icf_nm_global_st	*p_nm_ptr;
	/*pointer to device parameters, returned by NM to user*/
	/*deviceparameters_st *p_dp  //need to modify Irfan*/
}icf_nm_mbl_info_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_acs_info_st
 * Description      : Contains parameters to process Network Access information
 *						request
 *						
 *----------------------------------------------------------------------------*/
typedef struct
{
	/*NM's global pointer*/
	icf_nm_global_st			*p_nm_ptr;

	/*Network access information, pointer is returned from NM to user*/
	icf_nm_access_info_st 	*p_ainfo;

}icf_nm_acs_info_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_pdp_st
 * Description      : Contains parmeters to process PDP context activation
 *						/modification request.. 
 *						
 *----------------------------------------------------------------------------*/
typedef struct{
	/*NM's global pointer*/
	icf_nm_global_st			*p_nm_ptr;
#ifdef ICF_NW_ACTIVATE
	/* This can be filled to an application dependent value if the application is 
	     maintaining a map of these ids 
	     In most the cases the application should fill this as 
	     ICF_NM_AUTOMATIC_PDP_CTXT_ID_GENERATION*/
	icf_uint32_t				prim_id;
	
	/*Name of APN to which has to connect*/
	icf_string_st 			apn;
	/*Quality of Service requested*/
	icf_nm_qos_st			qos; 
	/*TFT Filter*/
	icf_nm_tft_st 			tft;  
#endif
	/*pointer to state data  */
	void 						*p_caller_data;

}icf_nm_pdp_st;


#ifdef ICF_NW_ACTIVATE
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_multiple_sec_pdp_activate_st
 * Description      : Contains parmeters to process Multiple Secondary PDP context activation
 *				requests 
 *						
 *----------------------------------------------------------------------------*/
typedef struct{
	/*NM's global pointer*/
	icf_nm_global_st				  *p_nm_ptr;

	icf_uint32_t					  no_of_cntxts;
	
	icf_call_id_t				  call_id;

	icf_nm_pdp_context_st          *p_pdp_contexts;	
		
	/*pointer to state data  */
	void 							   *p_caller_data;
	
}icf_nm_multiple_sec_pdp_activate_st;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_multiple_sec_pdp_deactivate_st
 * Description      : Contains parmeters to process Multiple Secondary PDP context deactivation
 *				requests related to a Call Id
 *						
 *----------------------------------------------------------------------------*/
 
 typedef struct{
	/*NM's global pointer*/
	icf_nm_global_st			*p_nm_ptr;
	icf_call_id_t				 call_id;
	/*pointer to state data  */
	void 						*p_caller_data;	
}icf_nm_multiple_sec_pdp_deactivate_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_deactivate_st
 * Description      : Contains parmeters to process PDP context activation
 *						/modification request. 
 *						
 *----------------------------------------------------------------------------*/
typedef struct{
	/*NM's global pointer*/
	icf_nm_global_st			*p_nm_ptr;
	/*Id of pdpContext to be de activated*/
	icf_uint32_t				cid;
    /*If true then primary and all its secondary contexts will be deactivated*/
    icf_boolean_t				tree;
    /*pointer to caller data  */
	void 						*p_caller_data;
}icf_nm_deactivate_st;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_mnc_st
 * Description      : Mobile network code
 *						
 *----------------------------------------------------------------------------*/
typedef struct{
	/*Lenth of mnc 2 or 3*/
	icf_uint8_t		len;
	/*Mobile network code*/
	icf_uint8_t		p_mnc[3];
}icf_mnc_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_pnwacs_info_st
 * Description      : Contains parameters to process P-network- access- 
 *						information request.
 *						
 *----------------------------------------------------------------------------*/

typedef struct{
	/*Global Pointer of NM*/
	icf_nm_global_st  	*p_nm_ptr; 
	/*Network type*/
	icf_uint8_t			access_type;
	/*Mobile country code*/
	icf_uint8_t			p_mcc[3]; 
	/*Mobile network code*/
	icf_mnc_st			mnc;	
	/*Location area code*/
	icf_uint16_t			lac; 
	/*Cell id  for accesstype = ICF_3GPP_GERAN */
	icf_uint16_t			ci; 
    /*Cell id for access_type = ICF_3GPP_UTRAN_FDD, 
	ICF_3GPP_UTRAN_TDD, ICF_3GPP_CDMA2000 */
	icf_uint32_t			umts_c;
}icf_nm_pnwacs_info_st;


/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_aka_info_st
 * Description      : Contains request-response parameters to process
 *						authentication.
 *						
 *----------------------------------------------------------------------------*/
typedef struct{

	/* NM global pointer*/
	icf_nm_global_st		*p_nm_ptr;
	
	/*Parameter contains the random value sent as a challenge from the network*/
	icf_string_st		autn;

	/*Parameter contains a parameter using with SIM authenticates the network,
	as well as verifies whether network database and SIM agree on the sequence
	number of current authentication challenge*/
	icf_string_st		rand;

	/*present only if sequence number is out of range, It indicates to the 
	network how much it should adjust its authentication sequence to come back 
	into synchronization. */
	icf_string_st		auts;

	/*Identifies the challenge-response value calculated by SIM*/
	icf_string_st		res;

	icf_string_st		imsCk;
	icf_string_st		imsIk;


} icf_nm_aka_info_st ;


/*------------------------------------------------------------------------------
 *
 * Type Name        : NM_private_uid_type
 * Description      : private user identity is temperary/stored
 *						
 *----------------------------------------------------------------------------*/
typedef enum __NM_private_uid_type
{
	ICF_PUID_STORED,
	ICF_PUID_TEMPORARY,
	ICF_PUID_CALCULATED
}icf_nm_private_uid_type_rt;

#ifdef ICF_NW_ACTIVATE
/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_network_event_t
 * Description      : NW event and data used to send to upper layers
 *						
 *----------------------------------------------------------------------------*/

typedef icf_uint32_t icf_nm_network_event_t;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_nw_event_ind_st
 * Description      : NW event and data used to send to upper layers
 *						
 *----------------------------------------------------------------------------*/
typedef struct 
{
	icf_nm_network_event_t 	event;
	void					*event_data;
}icf_nm_nw_event_ind_st;
#endif

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_aka_status_et
 * Description      : Status of AKA operation
 *						
 *----------------------------------------------------------------------------*/
typedef enum __icf_nm_aka_status
{
    ICF_NM_EMIMAKASUCCESSFUL,
	ICF_NM_EMIMAKASEQRANGEERR,
	ICF_NM_EMIMAKAUNAUTHNWERR,
	ICF_NM_EMIMAKASYSTEMERR,
	ICF_NM_EMIMAKANULL
}icf_nm_aka_status;

/*------------------------------------------------------------------------------
 *
 * Type Name        : icf_nm_pdb_st
 * Description      : NM module pdb
 *						
 *----------------------------------------------------------------------------*/
typedef struct
{
    /* pointer to global pdb */
    icf_glb_pdb_st		            *p_glb_pdb;
    
    /* pointer to internal message received */
    icf_internal_msg_st              *p_internal_msg;

    /* pointer to NM global data */
    icf_nm_global_st                 *p_nm_glb_data;

    /* pointer to error code */
    icf_error_t                      *p_ecode;
    
} icf_nm_pdb_st;

typedef struct
{
	icf_result_t		result;

}icf_nm_deactivate_ind_st;

#ifdef ICF_QOS_SUPPORT
typedef struct
{
	icf_uint16_t	id;
	icf_call_id_t	call_id;
}icf_nm_non_ims_qos_reserve_st;

typedef icf_nm_non_ims_qos_reserve_st	icf_nm_non_ims_qos_release_st;
#endif

typedef icf_nm_deactivate_ind_st	icf_nm_deactivate_rsp_st;
typedef icf_nm_deactivate_ind_st	icf_nm_activate_rsp_st;

typedef icf_nm_deactivate_ind_st	icf_nm_qos_release_rsp_st;
typedef icf_nm_deactivate_ind_st	icf_nm_qos_reserve_rsp_st;

typedef icf_nm_deactivate_ind_st icf_nm_modify_ind_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_NM_TYPES_H__ */

