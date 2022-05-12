/*****************************************************************************
 ** FUNCTION:	
 **			 Print Api handler protoypes
 **                    
 **                
 **
 ******************************************************************************
 **
 ** FILENAME:		icf_es_intf_trace.h
 **
 ** DESCRIPTION:	This file contains API Handler prototypes
 **
 ** DATE			NAME		REFERENCE	   REASON
 ** ----			----		---------	   ------
 ** 11-APR-2005	   Ram Narayan	 None           Initial
 *****************************************************************************
 ** 			Copyrights 2006, Aricent.
 *****************************************************************************/


#ifndef __ICF__INTF__TRACE__H__
#define __ICF__INTF__TRACE__H__



/* Prohibit Name mangling by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "icf_defs.h"
#include "icf_types.h"
#include "icf_port.h"
#include "icf_port_defs.h"
#include "icf_common_defs.h"
#include "icf_mm_types.h"
#include "icf_mm_api.h"



#define ICF_MAX_BUFF_LEN (256)

#define ICF_MODULE_RTP (8)

#define NUM_API_MAP_ENTRIES (125)

icf_return_t icf_interface_trace(icf_msg_st* p_msg);

icf_return_t icf_ifx_interface_trace(icf_return_t src_mod, 
                                               icf_return_t dst_mod, 
                                               icf_return_t api_id, 
                                               void* ifx_msg,
                                               icf_uint16_t len);
#define ICF_INTERFACE_TRACE(p_msg)  \
icf_interface_trace((icf_msg_st*)p_msg);
char* icf_get_module_name_from_id(icf_module_id_t id);

char* icf_get_api_name(icf_api_id_t api_id, 
                          icf_module_id_t source_id, 
                          icf_module_id_t destination_id);

typedef icf_return_t (*icf_print_api_msg_handler)(icf_msg_st*);

icf_return_t icf_interface_trace(icf_msg_st* p_msg);

icf_return_t icf_ifx_interface_trace(icf_return_t src_mod, 
                                               icf_return_t dst_mod, 
                                               icf_return_t api_id, 
                                               void* ifx_msg,
                                               icf_uint16_t len);


#define ICF_IFX_INTERFACE_TRACE(src_mod, dst_mod, api_id, ifx_msg, len) \
icf_ifx_interface_trace(src_mod, dst_mod, api_id, ifx_msg, len);




#ifdef ICF_INTERFACE_TRACE_ENABLE
#define ICF_PRINT_MESSAGE(string)  printf string ;

#else
#define ICF_PRINT_MESSAGE(string)
#endif




char* icf_get_module_name_from_id(icf_module_id_t id);

char* icf_get_api_name(icf_api_id_t api_id, 
                          icf_module_id_t source_id, 
                          icf_module_id_t destination_id);


/*

	icf_print_message.c
	Prototypes of All API print handlers
*/	
	icf_return_t icf_print_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_common_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_no_payload_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_resp_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_api_header(icf_msg_st* p_msg);
	
        icf_return_t icf_print_codec_list(icf_codec_attrib_st* p_codec_list);

	icf_return_t icf_print_set_scale_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_trans_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_call_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_self_id_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_reg_params(icf_msg_st *p_msg);
	icf_return_t icf_print_set_config_params_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_set_service_params(icf_msg_st* p_msg);
	
	icf_return_t icf_print_incoming_call(icf_msg_st *p_msg);
	icf_return_t icf_print_create_call(icf_msg_st *p_msg);
	icf_return_t icf_print_alert_user(icf_msg_st *p_msg);
	icf_return_t icf_print_user_alerted(icf_msg_st *p_msg);
	
	icf_return_t icf_print_term_call_req(icf_msg_st *p_msg);
	icf_return_t icf_print_term_call_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_display_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_call_log_resp(icf_msg_st *p_msg);
	
	icf_return_t icf_print_call_redirection_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_conference_req(icf_msg_st *p_msg);
	icf_return_t icf_print_ctu_req(icf_msg_st *p_msg);
	icf_return_t icf_print_cta_req(icf_msg_st *p_msg);
	icf_return_t icf_print_string_list(icf_string_list_st *p_str_list,icf_uint8_t *p_list_name);
	
	
	icf_return_t icf_print_rem_call_transfer_init(icf_msg_st *p_msg);
	icf_return_t icf_print_call_merged_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_party_replaced_ind(icf_msg_st *p_msg);
	

	icf_return_t	icf_print_get_codec_list_resp(icf_msg_st *p_msg);
	icf_return_t	icf_print_gcl_resp(icf_msg_st *p_msg);

	icf_return_t icf_print_rtp_resp_msg(icf_msg_st *p_msg);
	icf_return_t icf_print_rtp_recv_req_msg(icf_msg_st *p_msg); 
   icf_return_t icf_print_rem_call_transfer_cfm(icf_msg_st* p_msg); 

	icf_return_t icf_print_unlock_codec_resp(icf_msg_st *p_msg);
	icf_return_t	icf_print_get_codec_list_req(icf_msg_st *p_msg);
        icf_return_t icf_print_codec_commit_req(icf_msg_st *p_msg);
        
    
        icf_return_t icf_print_cms_req(icf_msg_st *p_msg);
	icf_return_t icf_print_dms_ind(icf_msg_st *p_msg);  
        icf_return_t icf_print_merge_media_req_msg(icf_msg_st *p_msg);    


        icf_return_t icf_print_set_trace_level(icf_msg_st *p_msg);  
        icf_return_t icf_print_set_error_level(icf_msg_st *p_msg);  
        icf_return_t icf_print_get_stats(icf_msg_st *p_msg);  
        icf_return_t icf_print_set_stats(icf_msg_st *p_msg);  

	icf_return_t icf_print_lock_codec_resp(icf_msg_st* p_msg);  
	icf_return_t icf_print_gcl_req(icf_msg_st* p_msg);
	icf_return_t icf_print_lock_codec_req(icf_msg_st* p_msg);
	icf_return_t icf_print_unlock_codec_req(icf_msg_st* p_msg);  
	icf_return_t icf_print_cms_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_mms_req(icf_msg_st* p_msg);      
   
	icf_return_t icf_print_rtp_err_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_rm_err_resp(icf_msg_st* p_msg);      

        void icf_print_api_from_api_id(icf_api_id_t api_id,icf_msg_st* p_msg);

	icf_return_t icf_print_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_common_msg(icf_msg_st* p_msg);
        icf_return_t icf_print_msg_hexdump(icf_msg_st* p_msg);
        icf_return_t icf_log_common_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_no_payload_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_resp_msg(icf_msg_st* p_msg);
	icf_return_t icf_print_api_header(icf_msg_st* p_msg);


	icf_return_t icf_print_set_line_param_req(icf_msg_st* p_msg);
	icf_return_t icf_print_set_scale_params(icf_msg_st* p_msg);
	icf_return_t icf_print_set_trans_params(icf_msg_st* p_msg);
	icf_return_t icf_print_set_call_params(icf_msg_st* p_msg);
	icf_return_t icf_print_set_self_id_params(icf_msg_st* p_msg);
	icf_return_t icf_print_set_reg_params(icf_msg_st* p_msg);
	icf_return_t icf_print_set_config_params_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_set_service_params(icf_msg_st* p_msg);
	icf_return_t icf_print_config_params_resp(icf_msg_st* p_msg);
	
	icf_return_t icf_print_incoming_call(icf_msg_st* p_msg);
	icf_return_t icf_print_create_call(icf_msg_st* p_msg);
	icf_return_t icf_print_alert_user(icf_msg_st* p_msg);
	icf_return_t icf_print_user_alerted(icf_msg_st* p_msg);
	
	icf_return_t icf_print_term_call_req(icf_msg_st* p_msg);
	icf_return_t icf_print_term_call_ind(icf_msg_st* p_msg);
	icf_return_t icf_print_display_ind(icf_msg_st* p_msg);
	icf_return_t icf_print_call_log_resp(icf_msg_st* p_msg);

        icf_return_t icf_print_progress_ind(icf_msg_st* p_msg);
        icf_return_t icf_print_media_connect_req(icf_msg_st* p_msg);	
	icf_return_t icf_print_call_redirection_ind(icf_msg_st* p_msg);
	icf_return_t icf_print_conference_req(icf_msg_st* p_msg);
	icf_return_t icf_print_ctu_req(icf_msg_st* p_msg);
	icf_return_t icf_print_cta_req(icf_msg_st* p_msg);
	
	
	icf_return_t icf_print_rem_call_transfer_init(icf_msg_st* p_msg);
	icf_return_t icf_print_call_merged_ind(icf_msg_st* p_msg);
	icf_return_t icf_print_party_replaced_ind(icf_msg_st* p_msg);
	

	icf_return_t	icf_print_get_codec_list_resp(icf_msg_st* p_msg);
	icf_return_t	icf_print_gcl_resp(icf_msg_st* p_msg);

	icf_return_t icf_print_rtp_resp_msg(icf_msg_st* p_msg);

	icf_return_t icf_print_unlock_codec_resp(icf_msg_st* p_msg);
	icf_return_t	icf_print_get_codec_list_req(icf_msg_st* p_msg);
        icf_return_t icf_print_codec_commit_req(icf_msg_st* p_msg);
        
    
        icf_return_t icf_print_cms_req(icf_msg_st* p_msg);
	icf_return_t icf_print_dms_ind(icf_msg_st* p_msg);  
        icf_return_t icf_print_merge_media_req_msg(icf_msg_st* p_msg);    
        icf_return_t icf_print_media_capabilities_req(icf_msg_st* p_msg);


        icf_return_t icf_print_set_trace_level(icf_msg_st* p_msg);  
   	icf_return_t icf_print_set_error_level(icf_msg_st* p_msg);  
    	icf_return_t icf_print_get_stats(icf_msg_st* p_msg);  
    	icf_return_t icf_print_set_stats(icf_msg_st* p_msg);  


	icf_return_t icf_print_rtp_recv_req_msg(icf_msg_st* p_msg);  

	icf_return_t icf_print_lock_codec_resp(icf_msg_st* p_msg);  
	icf_return_t icf_print_gcl_req(icf_msg_st* p_msg);
	icf_return_t icf_print_lock_codec_req(icf_msg_st* p_msg);
	icf_return_t icf_print_unlock_codec_req(icf_msg_st* p_msg);  
	icf_return_t icf_print_cms_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_mms_req(icf_msg_st* p_msg);      
   
	icf_return_t icf_print_rtp_err_resp(icf_msg_st* p_msg);      
	icf_return_t icf_print_rm_err_resp(icf_msg_st* p_msg);      
        icf_return_t icf_print_error_ind(icf_msg_st* p_msg);
        icf_return_t icf_print_trace_ind(icf_msg_st* p_msg);
        icf_return_t icf_print_get_stats_resp(icf_msg_st* p_msg);




	void icf_print_api_from_api_id(icf_api_id_t api_id,icf_msg_st* p_msg);
	void  icf_print_error_details(int p); 

    	icf_return_t icf_print_enable_conf_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_disable_conf_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_enable_conf_resp(icf_msg_st* p_msg);
    	icf_return_t icf_print_disable_conf_resp(icf_msg_st* p_msg);

     	icf_void_t icf_print_pub_uri_list(icf_pub_uri_list_node_st *p_list);
 	icf_return_t icf_print_sec_pdp_activate_req(icf_msg_st *p_msg);
	icf_return_t icf_print_pdp_deactivate_req(icf_msg_st *p_msg);
 	icf_return_t icf_print_platform_event_ind(icf_msg_st *p_msg);

	icf_return_t icf_print_pdp_modify_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_pdp_deactivate_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_platform_event_ind(icf_msg_st *p_msg);
	icf_void_t icf_print_icf_address_st(icf_address_st *p_uri);
	icf_return_t icf_print_start_fax_req(icf_msg_st *p_msg);
	icf_return_t icf_print_start_fax_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_fax_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_start_fax_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_fax_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_remote_video_diaplay_req(icf_msg_st *p_msg);
	icf_return_t icf_print_preview_video_diaplay_req(icf_msg_st *p_msg);
	icf_return_t icf_print_connect_media_device_req(icf_msg_st* p_msg);
	icf_return_t icf_print_mms_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_codec_commit_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_cfg_media_req(icf_msg_st *p_msg);
	icf_return_t icf_print_play_media_req(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_media_play_req(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_media_play_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_vibration_req(icf_msg_st* p_msg);
	icf_return_t icf_print_echo_canc_req(icf_msg_st* p_msg);
	icf_return_t icf_print_vad_ss_cng_req(icf_msg_st* p_msg);
	icf_return_t icf_print_stop_vibration_req(icf_msg_st *p_msg);
	icf_return_t icf_print_sec_pdp_activate_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_pdp_deactivate_req(icf_msg_st *p_msg);
	icf_return_t icf_print_pdp_deactivate_resp(icf_msg_st *p_msg);


    	icf_return_t icf_print_transport_addr(icf_transport_addr_st* p_trans);

    	icf_return_t icf_print_header_list(icf_header_st* p_hdr, 
                                          icf_int32_t hdr_count);
    	icf_return_t icf_print_tag_list(icf_tag_st* p_tag, 
                                       icf_int32_t tag_count);
    	icf_return_t icf_print_body_list(icf_msg_body_st* p_body, 
                                        icf_int32_t body_count);
	#ifdef ICF_REL_OLD
    	icf_return_t icf_print_stream_list(icf_stream_sdp_info_st* p_stream,
                                          icf_uint8_t stream_count);
	#endif
    	icf_return_t icf_print_header_name_list(icf_header_name_st* p_hdr, 
                                               icf_uint8_t hdr_count);

    	icf_return_t icf_print_media_connected(icf_msg_st* p_msg);
        icf_return_t icf_print_media_connect_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_connect_req(icf_msg_st* p_msg);
	icf_return_t icf_print_connect_resp(icf_msg_st* p_msg);
    	icf_return_t icf_print_connect_ind(icf_msg_st* p_msg);


   	 icf_return_t icf_print_get_sys_settings(icf_msg_st* p_msg);
	icf_return_t icf_print_get_line_settings(icf_msg_st* p_msg);
	icf_return_t icf_print_get_line_settings_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_get_sys_settings_resp(icf_msg_st* p_msg);

	/*ICF REL 6.0 */
	icf_return_t icf_print_audio_codec_param(icf_uint8_t *p_audio_param);
	icf_return_t icf_print_video_codec_param(icf_uint8_t *p_video_param);
	icf_return_t icf_print_t38_attrib_param(icf_uint8_t *p_t38_param);
	icf_return_t icf_print_audio_profile(icf_uint8_t *p_audio_profile);
	icf_return_t icf_print_video_profile(icf_uint8_t *p_video_profile);
	icf_return_t icf_print_stream_capab(icf_stream_capab_st* p_stream_capab_st);
	icf_return_t icf_print_stream_sdp(icf_sdp_stream_st* p_sdp_stream_st,icf_media_type_t stream_type);
	icf_return_t icf_print_resp_stream(icf_resp_stream_st* p_resp_stream_st, icf_uint8_t i);
	icf_return_t icf_print_modify_media_change_req(icf_call_modify_media_data_st* p_modify_media_change_req);

    	icf_return_t icf_print_app_add_req(icf_msg_st* p_msg);
   	 icf_return_t icf_print_app_add_cfm(icf_msg_st* p_msg);
   	 icf_return_t icf_print_app_remove_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_app_remove_cfm(icf_msg_st* p_msg);
    
    	icf_return_t icf_print_network_activate_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_event_indication(icf_msg_st* p_msg);
    	icf_return_t icf_print_network_activate_cfm(icf_msg_st* p_msg);
    	icf_return_t icf_print_network_deactivate_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_network_deactivate_cfm(icf_msg_st* p_msg);

    	icf_return_t icf_print_register_req(icf_msg_st* p_msg);
    	icf_return_t icf_print_register_cfm(icf_msg_st* p_msg);
    	icf_return_t icf_print_register_status_ind(icf_msg_st* p_msg);

    	icf_return_t icf_print_subscribe_req(icf_msg_st* p_msg);
	icf_return_t icf_print_subscribe_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_notify_ind(icf_msg_st* p_msg);

	icf_return_t icf_print_publish_req(icf_msg_st* p_msg);
	icf_return_t icf_print_publish_resp(icf_msg_st* p_msg);

	icf_return_t icf_print_message_req(icf_msg_st* p_msg);
	icf_return_t icf_print_message_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_message_ind(icf_msg_st* p_msg);

	icf_return_t icf_print_codec_info_req(icf_msg_st *p_msg);
	icf_return_t icf_print_ringer_tones_req(icf_msg_st *p_msg);
	icf_return_t icf_print_jbm_mgmt_req(icf_msg_st *p_msg);
	icf_return_t icf_print_max_earp_vol_req(icf_msg_st *p_msg);
	icf_return_t icf_print_max_speaker_vol_req(icf_msg_st *p_msg);
	icf_return_t icf_print_port_range_req(icf_msg_st *p_msg);
	icf_return_t icf_print_start_ringing_req(icf_msg_st *p_msg);
#ifdef ICF_REL_OLD
	icf_return_t icf_print_media_session_req(icf_msg_st *p_msg);
#endif
	icf_return_t icf_print_get_stats_req(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_ringing_req(icf_msg_st *p_msg);
#ifdef ICF_REL_OLD
	icf_return_t icf_print_mute_req(icf_msg_st *p_msg);
	icf_return_t icf_print_play_media_spkr_req(icf_msg_st *p_msg);
#endif
	icf_return_t icf_print_set_earp_volume_req(icf_msg_st *p_msg);
	icf_return_t icf_print_set_speaker_volume_req(icf_msg_st *p_msg);
	icf_return_t icf_print_send_app_data_req(icf_msg_st *p_msg);
	icf_return_t icf_print_send_stats_req(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_stats_req(icf_msg_st *p_msg);
	
	icf_return_t icf_print_stop_ringing_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_rcv_app_data_ind(icf_msg_st *p_msg);
	
	icf_return_t icf_print_codec_info_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_ringer_tones_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_jbm_mgmt_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_max_earp_vol_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_max_speaker_vol_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_set_port_range_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_ringing_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_media_session_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_get_stats_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_ringing_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_mute_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_play_media_spkr_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_set_earp_volume_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_set_speaker_volume_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_send_app_data_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_send_stats_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_stop_stats_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_nw_buff_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_med_cap_resp(icf_msg_st *p_msg);
    	icf_return_t icf_print_rtp_start_ind(icf_msg_st *p_msg);

	icf_return_t icf_print_max_mic_gain_req(icf_msg_st *p_msg);
	icf_return_t icf_print_mic_gain_req(icf_msg_st *p_msg);
	icf_return_t icf_print_max_mic_gain_resp(icf_msg_st *p_msg);
	icf_return_t icf_print_mic_gain_resp(icf_msg_st *p_msg);

	icf_return_t icf_print_mm_msg_resp(icf_msg_st *p_msg);

    	icf_return_t icf_print_refer_req(icf_msg_st* p_msg);
	icf_return_t icf_print_refer_resp(icf_msg_st* p_msg);

	icf_return_t icf_print_options_req(icf_msg_st* p_msg);
	icf_return_t icf_print_options_resp(icf_msg_st* p_msg);
	icf_return_t icf_print_options_cfm(icf_msg_st* p_msg);
	icf_return_t icf_print_options_ind(icf_msg_st* p_msg);

/* ICF_HANDLE_REINVITE_START */ 
	icf_return_t icf_print_incoming_call_modify_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_call_modify_cfm(icf_msg_st *p_msg);
	icf_return_t icf_print_incoming_call_modify_cancel_ind(icf_msg_st *p_msg);
	icf_return_t icf_print_call_modify_req(icf_msg_st *p_msg);
	icf_return_t icf_print_call_modify_resp(icf_msg_st *p_msg);
/* ICF_HANDLE_REINVITE_END  */


/*************** Management Plane Handler (MPH) APIs Starts here *************/

icf_return_t icf_generate_set_mgmt_plane_auth_params(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_create_secure_conn_req(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_close_secure_conn_req(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_xcap_create_replace_res(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_xcap_delete_res(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_xcap_fetch_res(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_generate_syncml_send_req(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
icf_return_t icf_print_set_mgmt_plane_auth_params(icf_msg_st *p_msg);
icf_return_t icf_print_create_secure_conn_req(icf_msg_st *p_msg);
icf_return_t icf_print_create_secure_conn_resp(icf_msg_st *p_msg);
icf_return_t icf_print_close_secure_conn_req(icf_msg_st *p_msg);
icf_return_t icf_print_set_close_secure_conn_resp(icf_msg_st *p_msg);
icf_return_t icf_print_secure_conn_status_ind(icf_msg_st *p_msg);
icf_return_t icf_print_xcap_create_replace_res(icf_msg_st *p_msg);
icf_return_t icf_print_xcap_delete_res(icf_msg_st *p_msg);
icf_return_t icf_print_xcap_fetch_res(icf_msg_st *p_msg);
icf_return_t icf_print_syncml_send_req(icf_msg_st *p_msg);
icf_return_t icf_print_mgmt_plane_api_proc_failure(icf_msg_st *p_msg);
icf_return_t icf_print_set_mgmt_plane_nw_resp(icf_msg_st *p_msg);

icf_return_t icf_print_info_req(icf_msg_st *p_msg);
icf_return_t icf_print_info_resp(icf_msg_st *p_msg);
icf_return_t icf_print_chd_req(icf_msg_st *p_msg);
icf_return_t icf_print_crm_req(icf_msg_st *p_msg);

/*************** Management Plane Handler (MPH) APIs Ends here ***************/


/* ICF_HANDLE_REINVITE_START */

icf_return_t icf_generate_call_modify_cfm(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);

icf_return_t icf_generate_call_modify_req(
											  icf_uint8_t* p_msg_file, 
											  icf_uint8_t* p_record_number,
											  icf_msg_st** p_p_msg);
/* ICF_HANDLE_REINVITE_START */

    icf_return_t icf_generate_call_resume_req(icf_uint8_t* p_msg_file, 
                                           icf_uint8_t* p_record_number,
                                           icf_msg_st** p_p_msg);


icf_return_t icf_generate_max_mic_gain_req_resp(icf_uint8_t* p_msg_file,
										  icf_uint8_t* p_record_number,
										  icf_msg_st** p_p_msg);
#ifdef ICF_STAT_ENABLED
icf_return_t icf_generate_get_icf_stats_req(icf_uint8_t* p_msg_file,
										   icf_uint8_t* p_record_number,
										   icf_msg_st** p_p_msg);

icf_return_t icf_print_get_icf_stats_req(icf_msg_st *p_msg);
icf_return_t icf_print_get_icf_stats_resp(icf_msg_st *p_msg);
#endif
	
icf_return_t icf_generate_chd_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_generate_crm_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_generate_info_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_api_resp_msg(icf_msg_st* p_msg);
icf_return_t icf_print_ctu_resp(icf_msg_st* p_msg);
icf_return_t icf_print_cta_resp(icf_msg_st* p_msg);
icf_return_t icf_print_api_ind(icf_msg_st* p_msg);

icf_return_t icf_print_open_channel_req(icf_msg_st* p_msg);


icf_return_t icf_generate_open_media_channel_resp(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_open_media_channel_resp(icf_msg_st* p_msg);

icf_return_t icf_print_close_media_channel_req(icf_msg_st* p_msg);

icf_return_t icf_print_send_udp_data_req(icf_msg_st* p_msg);

icf_return_t icf_generate_recv_udp_data_ind(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_recv_udp_data_ind(icf_msg_st* p_msg);

icf_return_t icf_generate_media_record_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_media_record_req(icf_msg_st* p_msg);

icf_return_t icf_generate_pause_media_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_pause_media_req(icf_msg_st* p_msg);

icf_return_t icf_generate_set_media_playback_device_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_set_media_playback_device_req(icf_msg_st* p_msg);

icf_return_t icf_generate_mute_media_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_mute_media_req(icf_msg_st* p_msg);

icf_return_t icf_generate_stop_media_record_req(icf_uint8_t* p_msg_file, 
                                     icf_uint8_t* p_record_number,
                                     icf_msg_st** p_p_msg);

icf_return_t icf_print_stop_media_record_req(icf_msg_st* p_msg);

icf_return_t icf_print_dgts_ind(icf_msg_st* p_msg);

icf_return_t icf_generate_self_ip_req(icf_uint8_t* p_msg_file,
											icf_uint8_t* p_record_number,
											icf_msg_st** p_p_msg);

icf_return_t icf_generate_digit_trans_req(icf_uint8_t* p_msg_file,
											icf_uint8_t* p_record_number,
											icf_msg_st** p_p_msg);

icf_return_t icf_generate_send_dgts_req(icf_uint8_t* p_msg_file, 
                                   icf_uint8_t* p_record_number,
                                   icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_line_params(icf_uint8_t* p_msg_file, 
                                             icf_uint8_t* p_record_number,
                                             icf_msg_st** p_p_msg);


icf_return_t icf_print_self_ip_req(icf_msg_st *p_msg);
icf_return_t icf_print_digit_trans_req(icf_msg_st *p_msg);
icf_return_t icf_print_stop_vibration_resp(icf_msg_st *p_msg);
icf_return_t icf_print_send_dgts_req(icf_msg_st *p_msg);
icf_return_t icf_print_send_dgts_resp(icf_msg_st *p_msg);
icf_return_t icf_print_call_fwd_ind(icf_msg_st *p_msg);
icf_return_t icf_print_dms_req(icf_msg_st *p_msg);


icf_return_t icf_generate_notify_cfm(icf_uint8_t* p_msg_file, 
                                               icf_uint8_t* p_record_number,
                                               icf_msg_st** p_p_msg);

icf_return_t icf_generate_info_cfm(icf_uint8_t* p_msg_file, 
                                               icf_uint8_t* p_record_number,
                                               icf_msg_st** p_p_msg); 

icf_return_t icf_print_info_ind(icf_msg_st *p_msg);
icf_return_t icf_print_notify_cfm(icf_msg_st* p_msg);
icf_return_t icf_print_info_cfm(icf_msg_st* p_msg);
icf_return_t icf_print_rem_call_transfer_init_cfm(icf_msg_st* p_msg);

icf_return_t icf_generate_codec_info_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_cfg_media_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_self_ip_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_mute_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_media_record_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_connect_media_device_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_get_stats_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_send_app_data_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_ringer_tones_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);


icf_return_t icf_generate_digit_trans_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);


icf_return_t icf_generate_jbm_mgmt_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);


icf_return_t icf_generate_max_earp_vol_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);


icf_return_t icf_generate_max_speaker_vol_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_max_mic_gain_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_remote_video_display_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_preview_video_display_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_port_range_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_pause_media_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_start_ringing_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_stop_ringing_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);




icf_return_t icf_print_nw_status(icf_msg_st *p_msg);

icf_return_t icf_print_info_resp(icf_msg_st *p_msg);

icf_return_t icf_generate_play_media_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_stop_media_play_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_media_playback_device_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_earp_volume_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_speaker_volume_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_stop_media_record_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

icf_return_t icf_generate_set_mic_gain_resp(icf_uint8_t* p_msg_file, 
                                              icf_uint8_t* p_record_number,
                                              icf_msg_st** p_p_msg);

void icf_print_error_details(int error_no);

icf_return_t icf_print_unexpected_msg_ind(icf_msg_st* p_msg);

icf_return_t icf_print_mm_unexpected_msg_ind(icf_msg_st* p_msg);

icf_return_t icf_print_early_call_ind(icf_msg_st* p_msg);

icf_return_t icf_print_early_call_cfm(icf_msg_st* p_msg);

icf_return_t icf_print_get_reg_status_req(icf_msg_st* p_msg);

icf_return_t icf_print_get_reg_status_resp(icf_msg_st* p_msg);

icf_return_t icf_print_call_mod_cancel_resp(icf_msg_st *p_msg);

	/* Prohibit Name mangling by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif


