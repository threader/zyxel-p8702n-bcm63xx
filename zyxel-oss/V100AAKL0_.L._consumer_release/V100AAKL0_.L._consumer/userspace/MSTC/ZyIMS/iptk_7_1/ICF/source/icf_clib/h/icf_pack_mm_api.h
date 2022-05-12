/*******************************************************************************
*										
* FILE NAME    : icf_pack_mm_api.h
*										
* DESCRIPTION  :The file contains declarations for the buffer packing code.
*               This is an automatically generated file.
*										
* Revision History :				
*				
*   DATE             NAME               REFERENCE              REASON
* -----------    ---------------      -------------      ------------------
* 05-JUL-2005     Raveen Sharma	       ICF REL 6.0		       Initial 
*   										
* Copyright 2006, Aricent.   					
*										
*******************************************************************************/



#ifndef __ICF_PACK_MM_API__H__
#define __ICF_PACK_MM_API__H__



/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */



/***************************************************

			RESOURCE MANAGEMENT APIs

****************************************************/
icf_uint32_t
icf_pack_icf_encoded_attributes_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_encoded_attributes_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_amr_params_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_audio_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_video_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_size_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_config_video_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_config_t38_attrib_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_codec_attrib_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t   stream_type
);

icf_uint32_t
icf_pack_icf_codec_attrib_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_codec_attrib_list_st,
    icf_uint32_t   *p_length,
	icf_uint8_t	stream_type
);

icf_uint32_t
icf_pack_icf_codec_info_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_audio_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_video_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_t_38_media_profile_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);
	
icf_uint32_t
icf_pack_icf_stream_capab_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_stream_capab_list_st
(	
	icf_uint8_t	   *p_dest,
    icf_list_st    *p_icf_stream_capab_list_st,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_get_codec_list_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);


icf_uint32_t
icf_pack_icf_config_audio_codec_param_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_get_codec_list_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_codec_commit_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_codec_commit_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_codec_commit_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_codec_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stream_commit_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stream_commit_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_stream_commit_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_codec_commit_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_codec_commit_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_rel_res_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_channel_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_channel_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_channel_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_open_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_channel_addr_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_channel_addr_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_channel_addr_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_open_media_channel_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);



/***************************************************

			     MEDIA CONTROL APIs

****************************************************/

icf_uint32_t
icf_pack_icf_sdp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length,
	icf_uint8_t   stream_type
);

icf_uint32_t
icf_pack_icf_req_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_req_stream_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_req_stream_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_create_media_session_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_resp_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_resp_stream_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_resp_stream_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_create_media_session_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_modify_stream_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_modify_stream_list_st
(
	icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_modify_stream_list_st,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_modify_media_session_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_modify_media_session_resp_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_delete_media_session_req_st
(	
	icf_uint8_t	*p_dest,
	icf_uint8_t	*p_src,
	icf_uint32_t	*p_length
);

icf_uint32_t
icf_pack_icf_msg_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_close_media_channel_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_delete_media_session_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_merge_stream_id_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_merge_media_session_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_media_capabilities_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_send_udp_data_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_recv_udp_data_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_media_capabilities_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);


#ifdef ICF_MM_CFG_FUTURE_RELEASE

/***************************************************

			   COMMOM APPLICATION APIs

****************************************************/
icf_uint32_t
icf_pack_icf_start_ringing_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_vibration_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_connect_media_device_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_echo_canc_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_vad_ss_cng_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_get_stats_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_rtcp_report_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_get_stats_resp_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_file_name_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t    *p_length
);

icf_uint32_t
icf_pack_icf_play_media_req_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stop_media_play_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stop_media_play_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

/***************************************************

			VIDEO/VOICE APPLICATION APIs

****************************************************/

icf_uint32_t
icf_pack_icf_dgts_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_dgts_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_mute_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_mute_media_resp_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_set_media_playback_device_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_media_record_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_send_dgts_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_set_earp_volume_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_set_speaker_volume_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_stop_media_record_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_set_mic_gain_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_pause_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

/***************************************************

				PUSH TO TALK APIs

****************************************************/
icf_uint32_t
icf_pack_icf_stop_stats_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_app_data_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_rcv_app_data_ind_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_send_app_data_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_send_stats_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

#endif

/***************************************************

			     CONFIGURATION APIs

****************************************************/
icf_uint32_t
icf_pack_icf_config_codec_attrib_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_codec_info_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

#ifdef ICF_MM_CFG_FUTURE_RELEASE

icf_uint32_t
icf_pack_icf_ringer_id_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_ringer_tones_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_media_id_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_media_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_set_digit_trans_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_jbm_mgmt_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_max_earp_vol_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_max_speaker_vol_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_cfg_max_mic_gain_req_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

icf_uint32_t
icf_pack_icf_video_display_st
(
	icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);

#endif /* MM_FUTURE RELEASE */

icf_uint32_t
icf_pack_icf_encoded_params_list_st
(
    icf_uint8_t    *p_dest,
    icf_list_st    *p_icf_encoded_params_list_st,
    icf_uint32_t    *p_length
);


icf_uint32_t
icf_pack_icf_mm_unexpected_msg_ind_st
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t   *p_length
);





#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /*__PACK_MM_H__*/
