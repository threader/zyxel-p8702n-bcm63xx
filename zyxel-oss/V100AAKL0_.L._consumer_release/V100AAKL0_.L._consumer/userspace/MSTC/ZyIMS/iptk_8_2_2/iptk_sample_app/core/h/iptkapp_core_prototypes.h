/*------------------------------------------------------------------------------
*
* File name        : iptkapps_core_fsm_defs.h 
*
* Purpose          : File contains #defines for fsm usage
* 
* Revision History :
*  DATE            NAME      REFERENCE       REASON
* --------      ------------   ----------   ---------------
* 5-Dec-2007   Vidyut Gupta                 Created Origional
* 10-Dec-2008  Vipul Varshney   SPR 17945   update for publish and message
*
*   Copyright © 2006 Aricent Inc . All Rights Reserved.
*
*******************************************************************************/

#ifndef __IPTKAPP_CORE_PROTOTYPES_H__
#define __IPTKAPP_CORE_PROTOTYPES_H__

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

icf_return_t
iptkapp_core_remove_call_context(
							icf_call_id_t 	call_id);
icf_return_t
iptkapp_core_remove_type_from_addr(icf_uint8_t *p_addr);

icf_void_t iptkapp_core_handle_display_menu(icf_void_t);

icf_int16_t iptkapp_publish_display_menu(icf_void_t);

icf_void_t iptkapp_core_display_menu(icf_void_t);
/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_add_type_to_addr
*
*  DESCRIPTION    : The function will take sip address whithout any sip: or
*					tel: and then depending on type that is present in the 
*					address it will prepend the sip: or tel:
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

icf_return_t
iptkapp_core_add_type_to_addr(icf_address_st *sip_addr);


/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_create_new_call_context
*
*  DESCRIPTION    : The function creates the new call context and initialize it.
*					Also this fucntion makes the entry of this new context in the
*					existing call context list
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

icf_return_t
iptkapp_core_create_new_call_context(
	iptkapp_core_call_context_list_st	**pp_new_call_context);

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_fill_hdr_and_send_cc
*
*  DESCRIPTION    : This function will fill the API header and send it to 
*					to the IPTK
*
*  RETURNS		  : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
*************************************************************************/
icf_return_t 
iptkapp_core_fill_hdr_and_send_cc(
	  icf_msg_st			*p_msg, 
	  icf_api_id_t			api_id,
	  icf_call_id_t		call_id,
	  icf_error_t			*p_err);


/***********************************************************************
*
*  FUNCTION NAME  : iptkapp_core_call_entry
*
*  DESCRIPTION    : This function will be called by the UIM to initiate 
*					a new call
*
*  RETURNS		  : Either DMP_SUCCESS or DMP_FAILURE
*************************************************************************/
icf_return_t
iptkapp_core_call_entry(iptkapp_core_call_data_st    *p_call_data,
                       iptkapp_core_event_et 	    event_from_cli,
				       icf_error_t					*p_err );

/*************************************************************************
*
*  FUNCTION NAME  : iptkapp_core_msg_handler_cc
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
iptkapp_core_msg_handler_cc(icf_msg_st *p_msg,icf_error_t *p_err);


icf_return_t
iptkapp_core_get_event
		(icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf);

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_create_call_api
 *
 *  DESCRIPTION    : This function fills the create call API towards IPTK 
 *                   for initiating outgoing calls.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_create_call_api(
        icf_void_t   *p_input_param,
        icf_uint16_t call_id,
        icf_error_t    *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_local_user_alerted_req_api
 *
 *  DESCRIPTION    : This function fills local user alerted req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_local_user_alerted_req_api(
        icf_uint16_t call_id,
        icf_error_t    *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_terminate_call_ind_api
 *
 *  DESCRIPTION    : This function fills local user alerted req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_terminate_call_ind_api(
        icf_uint16_t call_id,
        icf_error_t  *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_terminate_call_req_api
 *
 *  DESCRIPTION    : This function fills terminate call req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_terminate_call_req_api(
        icf_uint16_t            call_id,
        icf_call_clear_reason_t call_clear_reason,
        icf_error_t             *p_err);

/*************************************************************************
 *
 *  FUNCTION NAME  : iptkapp_core_fill_connect_call_req_api
 *
 *  DESCRIPTION    : This function fills connect call req API 
 *                    towards IPTK.
 *
 *  RETURNS:       : Either IPTKAPP_SUCCESS or IPTKAPP_FAILURE
 *************************************************************************/
iptkapp_return_t iptkapp_core_fill_connect_call_req_api(
        icf_uint16_t call_id,
        icf_error_t    *p_err);


/************************************************************************
 * NAME:          iptkapp_validate_addr 
 * DESCRIPTION:   Function to validate characters in address
 ***********************************************************************/

iptkapp_return_t iptkapp_validate_addr(icf_int8_t *addr);


/******************************************************************************
 *
 * FUNCTION:        iptkapp_publish_display_menu
 *
 * DESCRIPTION:     This function displays the menu and take input from CLI 
 *                  for furthur options under PUBLISH request
 *
 *                  IP Phone Toolkit Demo Application
 *
 *                  1) INITIAL 
 *                  2) REFRESH
 *                  3) MODIFY
 *                  4) REMOVE
 *
 *****************************************************************************/
icf_int16_t iptkapp_publish_display_menu(void);








/* All memory related macros will go here */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif 


