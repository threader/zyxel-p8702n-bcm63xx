/*******************************************************************************
*										
* FILE NAME    : icf_pack_api.h
*										
* DESCRIPTION  :The file contains declarations for the buffer packing code.
*               This is an automatically generated file.
*										
* Revision History :				
*				
*    DATE            NAME         REFERENCE       REASON
* --------    ------------        ----         ----------
* 15-JUL-2005   Ram Narayan        None         Initial
* 02-Feb-2006  Amit Sharma                      CFG merged with ICF 2.0
* 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF
* 07-Mar-2006	Anuradha Gupta		ICF			Added new functions for Re-invite handling
* 30-Mar-2006  Anuradha Gupta	ICF 5.0             Call Modify Feature changes 
* 23-Mar-2007   Deepti Goyal    ICF 6.4         Added new functions for media_
*                                               connect_req API.
* Copyright 2007, Aricent.   					
*										
*******************************************************************************/



#ifndef __ICF_PACK_API__H__
#define __ICF_PACK_API__H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */






icf_uint32_t
icf_pack_icf_set_call_params_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);



icf_uint32_t
icf_pack_icf_string_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
icf_uint32_t
icf_pack_icf_large_string_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);
icf_uint32_t
icf_pack_icf_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_self_addr_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_self_addr_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_self_name_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_self_name_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_default_settings_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_default_settings_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


#ifdef ICF_SERVICE_ENABLE

icf_uint32_t
icf_pack_icf_set_service_params_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif


icf_uint32_t
icf_pack_icf_call_redirection_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_conference_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_call_transfer_unattended_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_cta_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

/*  XCONF API DEclarations */
icf_uint32_t
icf_pack_icf_create_xconf_req_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_add_xconf_party_req_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_delete_xconf_party_req_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_release_xconf_req_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_create_xconf_resp_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_add_xconf_party_resp_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_delete_xconf_party_resp_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_release_xconf_ind_st
(
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_release_xconf_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);
/* XCONF API end */

icf_uint32_t
icf_pack_icf_call_hold_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_call_resume_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_remote_call_transfer_initiated_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_call_merged_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_party_replaced_ind
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_config_status_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_status_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_address_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_ipv4_int_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_transport_addr_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_transport_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_network_activate_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif

icf_uint32_t
icf_pack_icf_void_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_event_indication_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_network_activate_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif

icf_uint32_t
icf_pack_icf_uri_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_transport_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

#ifdef ICF_NW_ACTIVATE
icf_uint32_t
icf_pack_icf_network_info_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_network_deactivate_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_network_deactivate_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif

icf_uint32_t
icf_pack_icf_app_add_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_get_sys_settings_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_get_sys_settings_resp_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_get_line_settings_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_get_line_settings_resp_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_header_name_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_header_name_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_app_add_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_app_remove_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_app_remove_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_set_self_id_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_set_transport_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_transport_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_transport_address_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_set_registration_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_config_registration_params_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_configure_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_register_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_header_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_header_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_tag_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_tag_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_msg_body_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_msg_body_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_create_call_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

#ifdef ICF_REL_OLD
icf_uint32_t
icf_pack_icf_stream_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_stream_list_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif

icf_uint32_t
icf_pack_icf_local_user_alerted_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_call_forwarded_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_terminate_call_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_connect_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_subscribe_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_publish_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_message_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_refer_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_register_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_register_status_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_incoming_call_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_remote_user_alerted_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_terminate_call_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_connect_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_subscribe_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_notify_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_notify_cfm_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_publish_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_message_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_message_ind_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_refer_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_api_header_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length,
	icf_uint32_t	*p_api_length_index
);
icf_uint32_t
icf_pack_unknown_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#if 0
icf_uint32_t
icf_pack_icf_set_scale_params_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
#endif
icf_uint32_t
icf_pack_icf_nw_buff_ind_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
/*--------------------------------------------------------------------*/






icf_uint32_t
icf_pack_U8
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_U16
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_U32
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

 

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Starts here *************/
/*****************************************************************************/


icf_uint32_t
icf_pack_icf_stretchable_hdr_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_set_mgmt_plane_auth_param_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_create_secure_conn_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_create_secure_conn_resp_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_close_secure_conn_req_st
(
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);



icf_uint32_t
icf_pack_icf_close_secure_conn_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_secure_conn_status_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);



icf_uint32_t
icf_pack_icf_create_replace_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);



icf_uint32_t
icf_pack_icf_delete_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);



icf_uint32_t
icf_pack_icf_fetch_res_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_req_uri_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_syncml_send_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_api_proc_failure_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_mgmt_plane_nw_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

/*****************************************************************************/
/*************** Management Plane Handler (MPH) APIs Ends here ***************/
/*****************************************************************************/

/*ICF_HANDLE_REINVITE_START*/

icf_uint32_t
icf_pack_icf_call_modify_cfm_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_incoming_call_modify_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_incoming_call_modify_cancel_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_call_modify_req_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_call_modify_resp_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

/*ICF_HANDLE_REINVITE_START*/
#ifdef ICF_SERVICE_ENABLE
icf_uint32_t
icf_pack_icf_ctu_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);
#endif


icf_uint32_t
icf_pack_icf_address_list_node_st
(
    icf_uint8_t    *p_dest,
    icf_address_list_node_st    *p_icf_address_list_node_st,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_pub_uri_list_st
(
    icf_uint8_t    *p_dest,
    icf_pub_uri_list_node_st    *p_icf_uri_list_st,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_res_unavail_ind_st
(
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_info_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_info_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_info_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_api_mesg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_ctu_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_cta_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_api_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);


icf_uint32_t
icf_pack_icf_redirection_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_route_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

/****************************************
********    ICF REL 6.0    *************
*****************************************/

icf_uint32_t
icf_pack_icf_short_string_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_long_string_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_stream_id_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_call_modify_media_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_call_modify_media_data_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_call_modify_media_change_req_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_media_connected_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_connect_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

/*Functions for fax support*/
#ifdef ICF_FAX_SUPPORT

icf_uint32_t
icf_pack_icf_start_fax_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_start_fax_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_start_fax_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

#endif

/* OPTIONS method support - start */
icf_uint32_t
icf_pack_icf_options_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_options_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_inc_option_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_icf_options_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

/* OPTIONS method support - end*/
icf_uint32_t
icf_pack_icf_set_scale_params_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_apn_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t icf_pack_icf_filter_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_filter_list_st
(
    icf_uint8_t     *p_dest,
    icf_uint8_t     *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_string_list_st
(   
        icf_uint8_t     *p_dest,
        icf_uint8_t     *p_src,
        icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_stream_list_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_info_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);
icf_uint32_t
icf_pack_icf_set_line_param_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_config_proxy_transport_address_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);
icf_uint32_t
icf_pack_icf_hold_option_st( icf_uint8_t *p_dest,
                             icf_uint8_t *p_src,
                             icf_uint32_t *p_length ); 

icf_uint32_t 
icf_pack_icf_line_id_t
( 
    icf_uint8_t *p_dest,
    icf_uint8_t *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_early_incoming_call_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_early_incoming_call_cfm_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);


#ifdef ICF_TRACE_ENABLE
icf_uint32_t icf_pack_icf_trace_level_req_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length);
#endif

#ifdef ICF_ERROR_ENABLE 
icf_uint32_t icf_pack_icf_error_report_level_req_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length);
icf_uint32_t icf_pack_icf_report_error_ind_st( icf_uint8_t *p_dest,
                                          icf_uint8_t *p_src,
                                          icf_uint32_t *p_length );
#endif

icf_uint32_t
icf_pack_icf_media_connect_req_st
(	
	icf_uint8_t	    *p_dest,
	icf_uint8_t	    *p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t icf_pack_icf_stop_fax_req_st( icf_uint8_t *p_dest,
                                                     icf_uint8_t *p_src,
                                                     icf_uint32_t *p_length );

icf_uint32_t icf_pack_icf_stop_fax_resp_st( icf_uint8_t *p_dest,
                                                      icf_uint8_t *p_src,
                                                      icf_uint32_t *p_length );

icf_uint32_t icf_pack_icf_stop_fax_ind_st( icf_uint8_t *p_dest,
                                                      icf_uint8_t *p_src,
                                                      icf_uint32_t *p_length );

icf_uint32_t icf_pack_icf_unexpected_msg_ind_st( icf_uint8_t    *p_dest,                                                                         icf_uint8_t    *p_src,
                                                 icf_uint32_t    *p_length
                                               );

icf_uint32_t icf_pack_icf_remote_call_transfer_initiated_cfm_st( icf_uint8_t *p_dest,
                                                      icf_uint8_t *p_src,
                                                      icf_uint32_t *p_length );




icf_uint32_t
icf_pack_icf_time_stamp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

#ifdef ICF_TRACE_ENABLE
icf_uint32_t
icf_pack_icf_trace_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_report_trace_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_set_trace_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

icf_uint32_t
icf_pack_icf_call_connect_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_stream_data_list_st
(
        icf_uint8_t    *p_dest,
        icf_list_st    *p_icf_stream_data_list_st,
        icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stream_data_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);


#endif




#ifdef ICF_STAT_ENABLE

icf_uint32_t
icf_pack_icf_user_reg_status_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_user_reg_status_st,
    icf_uint32_t    *p_length
);


icf_uint32_t
icf_pack_icf_stats_req_st
(
           icf_uint8_t      *p_dest,
           icf_uint8_t      *p_src,
           icf_uint32_t     *p_length
);


icf_uint32_t
icf_pack_icf_stats_resp_st
(
           icf_uint8_t      *p_dest,
           icf_uint8_t      *p_src,
           icf_uint32_t     *p_length
);

#endif

icf_uint32_t
icf_pack_icf_get_register_status_req_st
(
           icf_uint8_t      *p_dest,
           icf_uint8_t      *p_src,
           icf_uint32_t     *p_length
);


icf_uint32_t
icf_pack_icf_get_register_status_resp_st
(
           icf_uint8_t      *p_dest,
           icf_uint8_t      *p_src,
           icf_uint32_t     *p_length
);

icf_uint32_t
icf_pack_icf_user_reg_status_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_user_reg_status_list_st,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_user_reg_status_st
(
  icf_uint8_t    *p_dest,
  icf_list_st    *p_src,
  icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_progress_ind_st
(	
	icf_uint8_t		*p_dest,
	icf_uint8_t		*p_src,
	icf_uint32_t	*p_length
);
/*call_mod_cancel*/
icf_uint32_t icf_pack_icf_call_mod_cancel_resp_st( icf_uint8_t *p_dest,
                                            icf_uint8_t *p_src,
                                            icf_uint32_t *p_length );


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__PACK_H__*/
