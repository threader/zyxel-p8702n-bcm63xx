/*------------------------------------------------------------------------------
 *
 * File name        : icf_nm_prototypes.h
 *
 * Purpose          : This file contains the prototypes of all functions 
 *                    used by Network Manager(NM).
 *                    
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 01-Aug-2005	Irfan Khan		LLD					Initial
 * 10-Dec-2005  Rohit Aggarwal  ICF 4.0 CDD         Code changes for handling
 *                                                  platform req/resp
 * 
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_NM_PROTOTYPES_H__ 
#define __ICF_NM_PROTOTYPES_H__ 

#include "icf_nm_types.h"


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

/*******************************************************************************
 *
 * FUNCTION	   :     icf_nm_get_icf_messg_for_pltfm
 *
 * DESCRIPTION :     This function will allocate mem for icf_msg_st and assign 
 *					  values to hdrs which need to be sent to Pltfm
 *					
 *
 * RETURN	   :  	 ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/
icf_return_t icf_nm_get_icf_messg_for_pltfm(INOUT   icf_glb_pdb_st       *p_glb_pdb,
												  OUT icf_msg_st ** p_p_icf_msg,
												  IN icf_int32_t payload_size);

/*******************************************************************************
 *
 * FUNCTION	   :     icf_nm_free_icf_messg_for_pltfm
 *
 * DESCRIPTION :     This function will free memory allocated for icf_msg_st
 *					
 *
 * RETURN	   :  	 ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/

icf_return_t icf_nm_free_icf_messg_for_pltfm(INOUT   icf_glb_pdb_st       *p_glb_pdb,
												  IN icf_msg_st * p_icf_msg);
   
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_process_mesg
 *
 * DESCRIPTION:     This is the only entry point of Network Manager. It will be
 *					used for all the requests/messages to NM. It will call 
 *					internal functions as per the request parameters and  may 
 *					send messages to other modules to notify about platform/
 *					network events.
 *					
 *					Return:ICF_SUCCESS_WO_RES/ICF_SUCCESS/ICF_FAILURE.
 *

 ******************************************************************************/

icf_return_t icf_nm_process_mesg(INOUT icf_internal_msg_st	*p_msg);

#ifdef ICF_NW_ACTIVATE
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_pcscf_info
 *
 * DESCRIPTION:     It will return the address of the pcscf
 *					
 * RETURN:			ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_pcscf_info(
		INOUT icf_nm_nw_info_req_st		*p_nw_info,
		INOUT icf_internal_msg_hdr_st	*p_msg_hdr);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_nw_info
 *
 * DESCRIPTION:     It will return the pointer to icf_nm_nw_info structure 
 *					which is stored localy in the NM.NM gets this information in
 *					NM initialization and maitains it.
 *					
 *					Return:ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_get_nw_info(INOUT icf_nm_nw_info_req_st *p_nw_info,
									  icf_internal_msg_hdr_st *p_msg_hdr);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_mbl_info
 *
 * DESCRIPTION:     It will return the pointer to device_parameters structure 
 *					which is stored localy in the NM.
 *					
 *					Return:ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_mbl_info(INOUT icf_nm_mbl_info_st	*p_m_info);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_usim_info
 *
 * DESCRIPTION:     It will calculate the information from IMSI no. in case of 
 *					USIM application
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_get_usim_info(
			INOUT   icf_glb_pdb_st					*p_glb_pdb,
			IN		icf_port_pltfm_device_params_resp_st	*p_plat_info, 
			INOUT   icf_nm_global_st				**p_p_nm_glb,
			INOUT   icf_error_t					    *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_acs_info
 *
 * DESCRIPTION:     It will return the network access information which is stored
 *				   locally in the NM. If required information is not present
 *				   then return failure
 *					
 * RETURN:			ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_acs_info(INOUT icf_nm_acs_info_st	*p_na_info,
									   INOUT icf_internal_msg_hdr_st *p_msg_hdr);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_inttostr
 *
 * DESCRIPTION:     Converts integer to string
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_int32_t icf_nm_inttostr(icf_int32_t p_int,icf_uint8_t *p_string);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_pacs_info
 *
 * DESCRIPTION:     It will provide the location information like mobile country
 *					code(mcc), mobile network code(mnc),location area code and
 *					cell id. 
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_pacs_info(icf_nm_pnwacs_info_st	*p_info,
									icf_internal_msg_hdr_st *p_msg_hdr);

#ifdef ICF_NW_ACTIVATE
#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        		icf_nm_populate_default_qos
 *
 * DESCRIPTION:    		Populate Default Qos for POC
 *
 *					
 * RETURN:				ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_populate_default_qos (INOUT icf_nm_qos_st *p_qos);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_activate_sec
 *
 * DESCRIPTION:     This will activate the secondary PDP context of a given 
 *					primary PDP context. Qos and TFT parameters may be requested
 *					for secondary PDP context.
 *					1)p_act->prim_id = id of primary PDP context from which 
 *					secondary has to derive, for this function.
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t  icf_nm_activate_sec(icf_nm_global_st	*p_glb_nm, 
										icf_nm_pdp_context_st *p_pdp_ctxt);

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_activate_multiple_sec_pdps
 *
 * DESCRIPTION	:     It will activate Multiple Secondary PDP contexts 
 *										
 * RETURN:			  ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
 
icf_return_t  icf_nm_activate_multiple_sec_pdps(
	INOUT	icf_nm_multiple_sec_pdp_activate_st		*p_mult_sec_act,
	INOUT	icf_internal_msg_hdr_st 				*p_msg_hdr);

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_deactivate_multiple_sec_pdps
 *
 * DESCRIPTION	:     It will deactivate All Secondary PDP contexts associated 
 *					  with a particular Call ID
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/


icf_return_t icf_nm_deactivate_multiple_sec_pdps( 
							IN icf_nm_multiple_sec_pdp_deactivate_st *p_mult_sec_deact,
							INOUT icf_internal_msg_hdr_st *p_msg_hdr);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_check_all_sec_deactivated
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  check if all sec pdps for a call id are deactivated 
 *					or not 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_check_all_sec_deactivated(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_call_id_t		callid);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_check_all_sec_activated
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  check if all sec pdps for a call id are activated 
 *					or not 
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_check_all_sec_activated(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_call_id_t		callid);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_deactivate_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the deactivate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_deactivate_resp(
		    INOUT icf_nm_pdb_st		*p_nm_pdb,
			IN    icf_call_id_t		callid,
			IN    icf_result_t		result);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_deactivate_ind
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the unsolicited deactivate indication with cause
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_deactivate_ind(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN 	     icf_call_id_t   callid,
		IN    icf_result_t		result);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_sec_activate_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the activate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_sec_activate_resp(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN	     icf_call_id_t		callid,
		IN    icf_result_t		result);

/******************************************************************************
 *
 * FUNCTION:            icf_nm_send_sec_modify_ind
 *
 * DESCRIPTION:      This function is invoked when ever any function needs to 
 *                  		    send the unsolicited modify indication with cause
 *                   
 * RETURNS:              ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		     case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_send_sec_modify_ind(
        INOUT icf_nm_pdb_st	*p_nm_pdb,
        IN	     icf_call_id_t		callid,
		IN    icf_result_t		result);

/******************************************************************************
 *
 * FUNCTION:         icf_nm_get_callid_index
 *
 * DESCRIPTION:      
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_callid_index(icf_nm_prim_sec_pdp_map_st				*p_pdp_ctxt_info,
									 icf_nm_multiple_sec_pdp_activate_st	*p_mult_sec_act,
									 icf_uint8_t							*idx);

/******************************************************************************
 *
 * FUNCTION:         icf_nm_get_vacant_index
 *
 * DESCRIPTION:      
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_vacant_index(icf_nm_prim_sec_pdp_map_st				*p_pdp_ctxt_info,
									 icf_uint8_t							*idx);

#endif /* #ifdef ICF_QOS_SUPPORT */

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_get_nw_info
 *
 * DESCRIPTION:     It will return the pointer to icf_nm_nw_info structure 
 *					which is stored localy in the NM.NM gets this information in
 *					NM initialization and maitains it.
 *					
 * RETURN:			ICF_SUCCESS/ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_get_nw_info(INOUT icf_nm_nw_info_req_st *p_nw_info,
									  INOUT icf_internal_msg_hdr_st *p_msg_hdr);

 /*******************************************************************************
 *
 * FUNCTION		:    icf_nm_activate_primary_pdp
 *
 * DESCRIPTION	:     This will send the activate PDP Req to nw for a given APN, Qos and
 *					TFT parameters.
 *					1)p_act->prim_id =0, for this function.
 *					2)In phase-1 only one primary PDP context will be allowed,
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

 icf_return_t icf_nm_activate_primary_pdp(icf_nm_pdp_st	 *p_act,
												icf_uint16_t	     prim_pdp_id,
												icf_port_pltfm_pdp_def_resp_st *p_pdp_def_rsp );

/*******************************************************************************
 *
 * FUNCTION		:    icf_nm_activate
 *
 * DESCRIPTION	:   This will activate the PDP context for a given APN, Qos and
 *					TFT parameters.
 *					1)p_act->prim_id =0, for this function.
 *					2)In phase-1 only one primary PDP context will be allowed,
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t  icf_nm_activate(icf_nm_pdp_st	*p_act, 
									icf_internal_msg_hdr_st *p_msg_hdr);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_deactivate
 *
 * DESCRIPTION:     It will deactivate the given PDP context 
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_deactivate(INOUT icf_nm_deactivate_st	*p_deact,
									 icf_internal_msg_hdr_st *p_msg_hdr);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_send_platform_even_to_cfg
 *
 * DESCRIPTION:     It will notify CFG module about network events.
 *										
 * RETURN			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_send_platform_event_to_cfg(
				INOUT icf_nm_pdb_st		*p_nm_pdb);

/******************************************************************************
 *
 * FUNCTION:        icf_nm_get_cntxt_from_cntxt_id
 *
 * DESCRIPTION:     This function is invoked to find the call index and sec 
 *					pdp index
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_get_call_id_from_cntxt_id(
         INOUT   icf_nm_global_st	*p_glb_nm,
         IN	     icf_uint16_t		 cntxt_id,
		 INOUT	     icf_boolean_t		*primary,
		 OUT	 icf_uint16_t	*call_index,
		 OUT	 icf_uint16_t	*sec_pdp_index);

/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_modify_ind
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   unsolicited modify indication is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_modify_ind(
        INOUT icf_nm_pdb_st		*p_nm_pdb);


/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_deactivate_ind
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   unsolicited deactivate indication is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_deactivate_ind(
        INOUT icf_nm_pdb_st		*p_nm_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_deactivate_rsp
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   deactivate response is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_deactivate_rsp(
        INOUT icf_nm_pdb_st		*p_nm_pdb);
#endif

/******************************************************************************
 *
 * FUNCTION:         icf_nm_process_pdp_activate_rsp
 *
 * DESCRIPTION:      This function is invoked when ever any 
 *                   activate response is received
 *                   
 * RETURNS:          ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                   case of any failure.       
 *
 ******************************************************************************/

icf_return_t icf_nm_process_pdp_activate_rsp(
        INOUT icf_nm_pdb_st		*p_nm_pdb);

#endif/*#ifdef ICF_NW_ACTIVATE*/

#ifdef ICF_QOS_SUPPORT
/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_qos_res_for_non_ims
 *
 * DESCRIPTION	:     It will activate Multiple Secondary PDP contexts 
 *										
 * RETURN:			  ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
 
icf_return_t  icf_nm_qos_res_for_non_ims(
			IN		icf_nm_non_ims_qos_reserve_st		*p_non_ims_qos_reserve,
			INOUT	icf_internal_msg_hdr_st 			*p_msg_hdr,
			INOUT	icf_nm_pdb_st						*p_nm_pdb);

/*******************************************************************************
 *
 * FUNCTION		:     icf_nm_qos_rel_for_non_ims
 *
 * DESCRIPTION	:     It will deactivate All Secondary PDP contexts associated 
 *					  with a particular Call ID
 *										
 * RETURN:			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/

icf_return_t icf_nm_qos_rel_for_non_ims(
			IN		icf_nm_non_ims_qos_release_st	*p_non_ims_qos_release,
			INOUT	icf_internal_msg_hdr_st			*p_msg_hdr,
			INOUT	icf_nm_pdb_st					*p_nm_pdb);

#if 0
/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_qos_release_non_ims_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the deactivate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_qos_release_non_ims_resp(
		    INOUT icf_nm_pdb_st		*p_nm_pdb);
#endif

/******************************************************************************
 *
 * FUNCTION:        icf_nm_send_qos_reserve_non_ims_resp
 *
 * DESCRIPTION:     This function is invoked when ever any function needs to 
 *                  		  send the activate success or failure.
 *                   
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
 *                  		case of any failure.       
 *
 ******************************************************************************/
icf_return_t icf_nm_send_qos_reserve_non_ims_resp(
        INOUT icf_nm_pdb_st		*p_nm_pdb);
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_auth
 *
 * DESCRIPTION:     This will be used to do authentication operation and return
 *					the result parameters along with the status.
 *																				
 * RETURN			ICF_SUCCESS/ ICF_FAILURE.
 *
 ******************************************************************************/
icf_return_t icf_nm_auth(icf_nm_aka_info_st  *p_aka, 
							   icf_internal_msg_hdr_st *p_msg_hdr );

/******************************************************************************
*
* FUNCTION:        icf_nm_set_call_ctxt_in_glb_db
*
* DESCRIPTION:     This function sets the local call context in 
*					the global database
*
*                   
* RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE in 
*                  		case of any failure.       
*
******************************************************************************/
icf_return_t icf_nm_set_call_ctxt_in_glb_db(
		    INOUT icf_nm_global_st	*p_glb_nm,
			IN    icf_call_id_t		callid);

/*******************************************************************************
 *
 * FUNCTION:        icf_nm_process_scalable_params_req
 *
 * DESCRIPTION:     This function will process scalable parameter req of nm
 *					
 *
 * RETURN			:ICF_SUCCESS/ICF_FAILURE
 *
 ******************************************************************************/

icf_return_t 	icf_nm_process_scalable_params_req
					(INOUT icf_nm_pdb_st		*p_nm_pdb);

#ifdef ICF_IPSEC_ENABLED
/* -------------------------------------------------------------------------- */
/* **** NM IPSec Functions **** */
/* -------------------------------------------------------------------------- */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_fetch_algo
 *
 * DESCRIPTION:     This is a utility function that returns the list of locally
 *                  supported security algorithms in internal message payload.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_fetch_algo(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_create_req
 *
 * DESCRIPTION:     This is a utility function that handles SA creation request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_create_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_create_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA creation response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_create_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_modify_req
 *
 * DESCRIPTION:     This is a utility function that handles SA modify request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_modify_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);
/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_modify_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA modify response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_modify_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_delete_req
 *
 * DESCRIPTION:     This is a utility function that handles SA deletion request
 *                  from REGM.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_delete_req(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_delete_resp
 *
 * DESCRIPTION:     This is a utility function that handles SA deletion response
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_delete_resp(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);


/*******************************************************************************
 *
 * FUNCTION:        icf_nm_sa_expiry_ind
 *
 * DESCRIPTION:     This is a utility function that handles SA expiry indication
 *                  from Wrapper.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_nm_sa_expiry_ind(
        INOUT   icf_nm_pdb_st        *p_nm_pdb);
#endif




#ifdef __cplusplus

}

#endif /* __cplusplus */



#endif /* __ICF_NM_PROTOTYPES_H__ */

