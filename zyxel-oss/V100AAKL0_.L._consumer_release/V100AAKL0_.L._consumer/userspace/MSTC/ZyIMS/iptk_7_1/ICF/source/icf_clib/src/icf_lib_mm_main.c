/*******************************************************************************
*										
* FILE NAME    :icf_lib_mm_main.c
*										
* DESCRIPTION  :The file contains the encoder and decoder routines of the 
*		library for media manager as well as the related free routines
*
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


#include "icf_clib_mm.h"
#include "icf_utils.h"
#include "icf_mm_api.h"
#include "icf_clib.h"
#include "icf_utils_mm.h"
#include "icf_pack_mm_api.h"
#include "icf_unpack_mm_api.h"

/* for warning removal */
extern icf_return_t icf_decode_mm_msg_payload(icf_msg_st*  p_msg, /* Output*/
                            icf_uint8_t*  p_encoded_buf,
                            icf_uint32_t  p_encoded_buf_length,
                            icf_api_id_t  api_id,
                            icf_error_t*  p_ecode);

extern icf_return_t icf_encode_mm_api_payload(icf_msg_st*   p_msg, /* Input*/
                            icf_uint8_t** p_p_encoded_buf, /* Output*/
                            icf_uint32_t* p_encoded_buf_length,
                            icf_error_t*  p_ecode);
/* end of warning removal code */


icf_return_t icf_encode_mm_msg(icf_msg_st*   p_msg, /* Input*/
							icf_uint8_t** p_p_encoded_buf, /* Output*/
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode)

{

	icf_uint32_t  length    = ICF_NULL;
	icf_uint8_t*  p_src     = ICF_NULL;
	icf_uint8_t*  p_dest    = ICF_NULL;
	icf_uint8_t*  p_buff    = ICF_NULL;
	icf_uint32_t api_length = ICF_NULL;
    icf_return_t ret_val    = ICF_SUCCESS;
	icf_uint32_t hdr_length = 0;
	icf_uint32_t api_length_index = 0;
	icf_uint16_t api_length_value = 0;


	if (!p_msg) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Argument #1 p_msg is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n<======================== ENCODE START ======================>\n"));


	api_length = p_msg->hdr.api_length;

    if (api_length <= 0 || api_length >= ICF_CLIB_MAX_API_LENGTH) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Invalid API Length (%d)\n", api_length));
        *p_ecode = ICF_ERR_INVALID_LENGTH;
        return ICF_FAILURE;
    }


	/* Allocate a temporary buffer with the API length of the */
	/* Input Message */

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_hdr\n"));
	p_buff   = (icf_uint8_t*)ICF_CLIB_MEMGET(api_length + ICF_CLIB_MAX_API_LENGTH);

	if (!p_buff) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure!! (p_buff)\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	p_src    = (icf_uint8_t*)p_msg;

	/* Pack the header first*/
	ret_val = (icf_uint8_t)icf_pack_icf_api_header_st(p_buff, p_src, &length, &api_length_index);

	hdr_length = length;
		
    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - ENCODING Failed While Packing API Header!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	/* Pack the rest of the payload*/
	p_src = (icf_uint8_t*)p_msg->payload;

	/* Make sure the destination points beyond the API header*/
	p_dest = p_buff + ICF_API_HEADER_LENGTH;


	/* Pack the Payload according to API Types*/

	switch (p_msg->hdr.api_id) 
	{
	

	/***************************************
	*	   Resource Management APIs
	****************************************/

	case ICF_GET_CODEC_LIST_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_get_codec_list_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_CODEC_LIST_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_get_codec_list_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_codec_commit_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_codec_commit_resp_st(p_dest, p_src, &length);
		break;

	case ICF_REL_RES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_rel_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_open_media_channel_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_open_media_channel_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_close_media_channel_req_st(p_dest, p_src, &length);
		break;

	/***************************************
	*	   Media Control APIs
	****************************************/
	
	case ICF_CREATE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_create_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CREATE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_create_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_modify_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_modify_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_delete_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_DELETE_MEDIA_SESSION_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_delete_media_session_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_merge_media_session_req_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_media_capabilities_req_st(p_dest, p_src, &length);
		break;
/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_media_capabilities_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_UDP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_udp_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_RECV_UDP_DATA_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_recv_udp_data_ind_st(p_dest, p_src, &length);
		break;

        case ICF_MM_UNEXPECTED_MSG_IND:
                ret_val= (icf_uint8_t)icf_pack_icf_mm_unexpected_msg_ind_st(p_dest, p_src, &length);
                break;


/***************************************
*	   Common Application APIs
****************************************/
	
	case ICF_START_RINGING_REQ:
	case ICF_START_RINGING_RESP:
	case ICF_VIBRATION_REQ:
	case ICF_VIBRATION_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_REQ:
	case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_ECHO_CANC_REQ:
	case ICF_ECHO_CANC_RESP:
	case ICF_VAD_SS_CNG_REQ:
	case ICF_VAD_SS_CNG_RESP:
	case ICF_GET_STATS_REQ:
	case ICF_GET_STATS_RESP:
	case ICF_PLAY_MEDIA_REQ:
	case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_REQ:
	case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_STOP_MEDIA_PLAY_IND:
	case ICF_STOP_RINGING_REQ:
	case ICF_STOP_RINGING_RESP:
	case ICF_STOP_VIBRATION_REQ:
	case ICF_STOP_VIBRATION_RESP:
/*Case missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_STOP_RINGING_IND:

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
	case ICF_MUTE_MEDIA_REQ:
	case ICF_MUTE_MEDIA_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_MEDIA_RECORD_REQ:
	case ICF_MEDIA_RECORD_RESP:
	case ICF_SEND_DGTS_REQ:
	case ICF_SEND_DGTS_RESP:
	case ICF_SET_EARP_VOLUME_REQ:
	case ICF_SET_EARP_VOLUME_RESP:
	case ICF_SET_SPEAKER_VOLUME_REQ:
	case ICF_SET_SPEAKER_VOLUME_RESP:
	case ICF_STOP_MEDIA_RECORD_REQ:
	case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_SET_MIC_GAIN_REQ:
	case ICF_SET_MIC_GAIN_RESP:
	case ICF_PAUSE_MEDIA_REQ:
	case ICF_PAUSE_MEDIA_RESP:
/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
	case ICF_STOP_STATS_RESP:
	case ICF_RCV_APP_DATA_IND:
	case ICF_SEND_APP_DATA_REQ:
	case ICF_SEND_APP_DATA_RESP:
	case ICF_SEND_STATS_REQ:
	case ICF_SEND_STATS_RESP:

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_RESP:
	case ICF_CFG_RINGER_TONES_REQ:
	case ICF_CFG_RINGER_TONES_RESP:
	case ICF_CFG_MEDIA_REQ:
	case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_DIGIT_TRANS_REQ:
	case ICF_CFG_SET_DIGIT_TRANS_RESP:
	case ICF_CFG_JBM_MGMT_REQ:
	case ICF_CFG_JBM_MGMT_RESP:
	case ICF_CFG_MAX_EARP_VOL_REQ:
	case ICF_CFG_MAX_EARP_VOL_RESP:
	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
	case ICF_CFG_MAX_MIC_GAIN_REQ:
	case ICF_CFG_MAX_MIC_GAIN_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
/*Cases missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
	case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_CFG_SET_SELF_IP_REQ:
	case ICF_CFG_SET_SELF_IP_RESP:
	case ICF_CFG_SET_PORT_RANGE_REQ:
	case ICF_CFG_SET_PORT_RANGE_RESP:
	
	case ICF_MM_INIT_COMPLETE:
	case ICF_MM_SET_NW_STATUS:


#ifdef ICF_PLTFM_TEST_ENABLE
/***************************************
*	       PLATFORM APIs
****************************************/
#ifdef ICF_NW_ACTIVATE
	case ICF_GPRS_SEC_PDP_ACTIVATE_REQ:
	case ICF_GPRS_PDP_DEACTIVATE_REQ:
#endif
	case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
	case ICF_PORT_PLTFM_PDP_MODIFY_IND:
	case ICF_PORT_PLTFM_PDP_DEACT_IND:
	case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:
	
#endif

	length = icf_get_api_length(p_msg->hdr.api_id);
	length = length - ICF_API_HEADER_LENGTH;

	api_length_value = (icf_uint16_t)(length + hdr_length);

	icf_pack_U16(p_buff + api_length_index, (icf_uint8_t*)&api_length_value, &hdr_length);

	length += ICF_API_HEADER_LENGTH;

	*p_encoded_buf_length = length;
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying API Header to encoded buffer\n"));
	/* Copy the encoded API header first*/
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_buff, ICF_API_HEADER_LENGTH)

	/* Copy the rest of the payload, move past the */
	/* encoded API Header*/
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf + ICF_API_HEADER_LENGTH, p_src, length - ICF_API_HEADER_LENGTH)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));

	return ret_val;
	break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	/***************************************
	*	   Common Application APIs
	****************************************/
	
	case ICF_START_RINGING_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_start_ringing_req_st(p_dest, p_src, &length);
		break;

	case ICF_START_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_vibration_req_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_connect_media_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_echo_canc_req_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_vad_ss_cng_req_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_get_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_get_stats_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_play_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_play_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_play_ind_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_RINGING_REQ:
		break;

	case ICF_STOP_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_VIBRATION_REQ:
		break;

	case ICF_STOP_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_dgts_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_mute_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_mute_media_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_media_playback_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_dgts_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_earp_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_speaker_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_pause_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_RCV_APP_DATA_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_rcv_app_data_ind_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_app_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

#endif

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_codec_info_req_st(p_dest, p_src, &length);
		break;

	
#ifdef ICF_MM_CFG_FUTURE_RELEASE

	case ICF_CFG_CODEC_INFO_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_ringer_tones_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_set_digit_trans_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_jbm_mgmt_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_earp_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_speaker_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MM_INIT_COMPLETE:
		break;
#endif

	default:				
        if (ICF_API_HEADER_LENGTH < api_length)
        {
        length = api_length - ICF_API_HEADER_LENGTH;
        ret_val = (icf_uint8_t)icf_pack_unknown_st(p_dest, p_src, &length);
        }
        else
        {
            /* donot get into negative subtraction */
            length = 0;
        }
        break;

	}


	 if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - ENCODING Failed While Packing!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	/* The length of the encoded buffer includes*/
	/* the length of API header */
	api_length_value = (icf_uint16_t)(length + hdr_length);

	icf_pack_U16(p_buff + api_length_index, (icf_uint8_t*)&api_length_value, &hdr_length);

	length += ICF_API_HEADER_LENGTH;

	*p_encoded_buf_length = length;
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying API Header to encoded buffer\n"));
	/* Copy the encoded API header first*/
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_buff, ICF_API_HEADER_LENGTH)

	/* Copy the rest of the encoded payload, move past the */
	/* encoded API Header*/
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf + ICF_API_HEADER_LENGTH, p_dest, length - ICF_API_HEADER_LENGTH)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));
    ICF_CLIB_TRACE(("\n<========================= ENCODE END =======================>\n"));
	
	return ret_val;
}	



icf_return_t icf_decode_mm_msg(icf_msg_st**  p_p_msg, /* Output*/
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  p_encoded_buf_length,
							icf_error_t*  p_ecode)
{

	icf_uint32_t length      = ICF_NULL;
	icf_uint8_t* p_src       = ICF_NULL;
	icf_uint8_t* p_dest      = ICF_NULL;
	icf_uint32_t api_length  = ICF_NULL;
	icf_uint16_t api_id      = ICF_NULL;
	icf_api_header_st* p_hdr = ICF_NULL;
        icf_return_t ret_val     = ICF_SUCCESS;
        /* To remove warning */
        p_encoded_buf_length=p_encoded_buf_length;

	if (!p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Argument #2 p_encoded_buf is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n<======================== DECODE START ======================>\n"));

	p_hdr = (icf_api_header_st*)ICF_CLIB_MEMGET(ICF_API_HEADER_LENGTH);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Getting Memory for p_hdr\n"));
	if (!p_hdr) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure()!\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	/* Unpack the header first*/
	ret_val = (icf_uint8_t)icf_unpack_icf_api_header_st((icf_uint8_t*)p_hdr, p_encoded_buf, &length);

    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - DECODING Failed While Unpacking API Header!!!\n"));
	    ICF_CLIB_MEMFREE(p_hdr)
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

	api_id     = p_hdr->api_id;

	if ((api_id < ICF_EXTERNAL_API_START) && (api_id >ICF_EXTERNAL_API_END))
	{
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - API id (%d) & API Length (%d) Mismatch\n", api_id, api_length));
		ICF_CLIB_MEMFREE(p_hdr)
        *p_ecode = ICF_ERR_DECODE_FAILED;
        return ICF_FAILURE;
    }

	api_length = icf_get_api_length(api_id);

	if(0 == api_length)
	{
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - API id (%d) & API Length (%d) Mismatch\n", api_id, api_length));
		ICF_CLIB_MEMFREE(p_hdr)
        *p_ecode = ICF_ERR_INVALID_LENGTH;
        return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Getting Memory for *p_p_msg\n"));
	*p_p_msg = (icf_msg_st*)ICF_CLIB_MEMGET(api_length);

	if (!*p_p_msg) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure()!\n"));
                ICF_CLIB_MEMFREE(p_hdr)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}

	p_dest = (*p_p_msg)->payload;

	/* Move past the API header into the buffer*/
	p_src  = p_encoded_buf + ICF_API_HEADER_LENGTH;


	/* Unpack the Payload according to API Types*/
	switch (api_id) 
	{

	/***************************************
	*	   Resource Management APIs
	****************************************/

	case ICF_GET_CODEC_LIST_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_get_codec_list_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_CODEC_LIST_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_codec_list_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_codec_commit_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_codec_commit_resp_st(p_dest, p_src, &length);
		break;

	case ICF_REL_RES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_rel_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_open_media_channel_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_open_media_channel_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_close_media_channel_req_st(p_dest, p_src, &length);
		break;

	/***************************************
	*	   Media Control APIs
	****************************************/
	
	case ICF_CREATE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_create_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CREATE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_create_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_modify_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_modify_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_delete_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_DELETE_MEDIA_SESSION_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_delete_media_session_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_merge_media_session_req_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_capabilities_req_st(p_dest, p_src, &length);
		break;
/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_capabilities_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_UDP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_udp_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_RECV_UDP_DATA_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_recv_udp_data_ind_st(p_dest, p_src, &length);
		break;

        case ICF_MM_UNEXPECTED_MSG_IND:
                ret_val= (icf_uint8_t)icf_unpack_icf_mm_unexpected_msg_ind_st(p_dest, p_src, &length);
                break;

/***************************************
*	   Common Application APIs
****************************************/
	
	case ICF_START_RINGING_REQ:
	case ICF_START_RINGING_RESP:
	case ICF_VIBRATION_REQ:
	case ICF_VIBRATION_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_REQ:
	case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_ECHO_CANC_REQ:
	case ICF_ECHO_CANC_RESP:
	case ICF_VAD_SS_CNG_REQ:
	case ICF_VAD_SS_CNG_RESP:
	case ICF_GET_STATS_REQ:
	case ICF_GET_STATS_RESP:
	case ICF_PLAY_MEDIA_REQ:
	case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_REQ:
	case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_STOP_MEDIA_PLAY_IND:
	case ICF_STOP_RINGING_REQ:
	case ICF_STOP_RINGING_RESP:
	case ICF_STOP_VIBRATION_REQ:
	case ICF_STOP_VIBRATION_RESP:
/*Case missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_STOP_RINGING_IND:

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
	case ICF_MUTE_MEDIA_REQ:
	case ICF_MUTE_MEDIA_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_MEDIA_RECORD_REQ:
	case ICF_MEDIA_RECORD_RESP:
	case ICF_SEND_DGTS_REQ:
	case ICF_SEND_DGTS_RESP:
	case ICF_SET_EARP_VOLUME_REQ:
	case ICF_SET_EARP_VOLUME_RESP:
	case ICF_SET_SPEAKER_VOLUME_REQ:
	case ICF_SET_SPEAKER_VOLUME_RESP:
	case ICF_STOP_MEDIA_RECORD_REQ:
	case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_SET_MIC_GAIN_REQ:
	case ICF_SET_MIC_GAIN_RESP:
	case ICF_PAUSE_MEDIA_REQ:
	case ICF_PAUSE_MEDIA_RESP:
/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
	case ICF_STOP_STATS_RESP:
	case ICF_RCV_APP_DATA_IND:
	case ICF_SEND_APP_DATA_REQ:
	case ICF_SEND_APP_DATA_RESP:
	case ICF_SEND_STATS_REQ:
	case ICF_SEND_STATS_RESP:

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_RESP:
	case ICF_CFG_RINGER_TONES_REQ:
	case ICF_CFG_RINGER_TONES_RESP:
	case ICF_CFG_MEDIA_REQ:
	case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_DIGIT_TRANS_REQ:
	case ICF_CFG_SET_DIGIT_TRANS_RESP:
	case ICF_CFG_JBM_MGMT_REQ:
	case ICF_CFG_JBM_MGMT_RESP:
	case ICF_CFG_MAX_EARP_VOL_REQ:
	case ICF_CFG_MAX_EARP_VOL_RESP:
	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
	case ICF_CFG_MAX_MIC_GAIN_REQ:
	case ICF_CFG_MAX_MIC_GAIN_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
/*Cases missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
	case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_CFG_SET_SELF_IP_REQ:
	case ICF_CFG_SET_SELF_IP_RESP:
	case ICF_CFG_SET_PORT_RANGE_REQ:
	case ICF_CFG_SET_PORT_RANGE_RESP:

	case ICF_MM_INIT_COMPLETE:
	case ICF_MM_SET_NW_STATUS:


#ifdef ICF_PLTFM_TEST_ENABLE
/***************************************
*	       PLATFORM APIs
****************************************/
#ifdef ICF_NW_ACTIVATE
	case ICF_GPRS_SEC_PDP_ACTIVATE_REQ:
	case ICF_GPRS_PDP_DEACTIVATE_REQ:
#endif
	case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
	case ICF_PORT_PLTFM_PDP_MODIFY_IND:
	case ICF_PORT_PLTFM_PDP_DEACT_IND:
	case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:

#endif

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - MEMCopying decoded Header to ICF Message\n"));
	ICF_CLIB_MEMCPY(&((*p_p_msg)->hdr), p_hdr, ICF_API_HEADER_LENGTH);
	ICF_CLIB_MEMCPY(&((*p_p_msg)->payload), p_src, api_length - ICF_API_HEADER_LENGTH);

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Freeing Header\n"));
	ICF_CLIB_MEMFREE(p_hdr)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Decoded Buffer Length (%d) Bytes\n", length));
	return ret_val;
	break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	/***************************************
	*	   Common Application APIs
	****************************************/
	
	case ICF_START_RINGING_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_start_ringing_req_st(p_dest, p_src, &length);
		break;

	case ICF_START_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_vibration_req_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_connect_media_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_echo_canc_req_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_vad_ss_cng_req_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_stats_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_play_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_play_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_play_ind_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_RINGING_REQ:
		break;

	case ICF_STOP_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_VIBRATION_REQ:
		break;

	case ICF_STOP_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_dgts_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_mute_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_mute_media_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_media_playback_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_dgts_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_earp_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_speaker_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_pause_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_RCV_APP_DATA_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_rcv_app_data_ind_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_app_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

#endif

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_codec_info_req_st(p_dest, p_src, &length);
		break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	case ICF_CFG_CODEC_INFO_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_ringer_tones_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_set_digit_trans_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_jbm_mgmt_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_earp_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_speaker_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MM_INIT_COMPLETE:
		break;
#endif

	default:				
        length = api_length - ICF_API_HEADER_LENGTH;
        ret_val = (icf_uint8_t)icf_unpack_unknown_st(p_dest, p_src, &length);
        break;
	}

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - MEMCopying decoded Header to ICF Message\n"));
	ICF_CLIB_MEMCPY(&((*p_p_msg)->hdr), p_hdr, ICF_API_HEADER_LENGTH);

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Freeing Header\n"));
	ICF_CLIB_MEMFREE(p_hdr)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Decoded Buffer Length (%d) Bytes\n", length));
    ICF_CLIB_TRACE(("\n<========================= DECODE END =======================>\n"));
	return ret_val;
}


icf_return_t icf_free_decoded_mm_msg(icf_msg_st* p_msg, icf_error_t* p_ecode)
{

    icf_return_t ret_val = ICF_SUCCESS;
    /*To remove warning */
    p_ecode=p_ecode;

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_free_decoded_mm_msg - FREEing decoded Buffer\n"));

	switch (p_msg->hdr.api_id) 
	{
	
    case ICF_GET_CODEC_LIST_REQ:			case ICF_GET_CODEC_LIST_RESP:
	case ICF_CODEC_COMMIT_REQ:			case ICF_CODEC_COMMIT_RESP:
	case ICF_CREATE_MEDIA_SESSION_REQ:		case ICF_CREATE_MEDIA_SESSION_RESP:
	case ICF_MODIFY_MEDIA_SESSION_REQ:		case ICF_MODIFY_MEDIA_SESSION_RESP:
	case ICF_MEDIA_CAPABILITIES_RESP:		case ICF_OPEN_MEDIA_CHANNEL_REQ:
	case ICF_OPEN_MEDIA_CHANNEL_RESP:		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
	case ICF_CFG_CODEC_INFO_REQ:  			case ICF_MEDIA_CAPABILITIES_REQ:
        case ICF_SEND_UDP_DATA_REQ:                     case ICF_RECV_UDP_DATA_IND:
        case ICF_MM_UNEXPECTED_MSG_IND:                          
		ret_val = icf_free_mm_msg(p_msg);
		break;

  
    case ICF_REL_RES_REQ:
		
	case ICF_DELETE_MEDIA_SESSION_REQ:	  		case ICF_DELETE_MEDIA_SESSION_RESP:
	case ICF_DELETE_MEDIA_SESSION_IND:	  	
	case ICF_MERGE_MEDIA_SESSION_REQ:	  		case ICF_MERGE_MEDIA_SESSION_RESP:
				

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	case ICF_START_RINGING_REQ:	  				case ICF_START_RINGING_RESP:
	case ICF_VIBRATION_REQ:	  					case ICF_VIBRATION_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_REQ:			case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_ECHO_CANC_REQ:	  					case ICF_ECHO_CANC_RESP:
	case ICF_VAD_SS_CNG_REQ:	  				case ICF_VAD_SS_CNG_RESP:
	case ICF_GET_STATS_REQ:	  					case ICF_GET_STATS_RESP:
	case ICF_PLAY_MEDIA_REQ:	  				case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_REQ:	  			case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_STOP_MEDIA_PLAY_IND:	  		
	case ICF_STOP_RINGING_REQ:	  				case ICF_STOP_RINGING_RESP:
	case ICF_STOP_VIBRATION_REQ:	  			case ICF_STOP_VIBRATION_RESP:
	case ICF_STOP_RINGING_IND:

	case ICF_DGTS_IND:	  				
	case ICF_MUTE_MEDIA_REQ:	  				case ICF_MUTE_MEDIA_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:		case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_MEDIA_RECORD_REQ:	  				case ICF_MEDIA_RECORD_RESP:
	case ICF_SEND_DGTS_REQ:	  					case ICF_SEND_DGTS_RESP:
	case ICF_SET_EARP_VOLUME_REQ:	  			case ICF_SET_EARP_VOLUME_RESP:
	case ICF_SET_SPEAKER_VOLUME_REQ:			case ICF_SET_SPEAKER_VOLUME_RESP:
	case ICF_STOP_MEDIA_RECORD_REQ:				case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_SET_MIC_GAIN_REQ:	  				case ICF_SET_MIC_GAIN_RESP:
	case ICF_PAUSE_MEDIA_REQ:	  				case ICF_PAUSE_MEDIA_RESP:

	case ICF_STOP_STATS_REQ:	  				case ICF_STOP_STATS_RESP:
	case ICF_RCV_APP_DATA_IND:	 
	case ICF_SEND_APP_DATA_REQ:	  				case ICF_SEND_APP_DATA_RESP:
	case ICF_SEND_STATS_REQ:	  				case ICF_SEND_STATS_RESP:

	case ICF_CFG_CODEC_INFO_RESP:
	case ICF_CFG_RINGER_TONES_REQ:	  			case ICF_CFG_RINGER_TONES_RESP:
	case ICF_CFG_MEDIA_REQ:	  					case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_DIGIT_TRANS_REQ:			case ICF_CFG_SET_DIGIT_TRANS_RESP:
	case ICF_CFG_JBM_MGMT_REQ:	  				case ICF_CFG_JBM_MGMT_RESP:
	case ICF_CFG_MAX_EARP_VOL_REQ:	  			case ICF_CFG_MAX_EARP_VOL_RESP:
	case ICF_CFG_MAX_SPEAKER_VOL_REQ:			case ICF_CFG_MAX_SPEAKER_VOL_RESP:
	case ICF_CFG_MAX_MIC_GAIN_REQ:	  			case ICF_CFG_MAX_MIC_GAIN_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ: 	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:			case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_CFG_SET_SELF_IP_REQ: 				case ICF_CFG_SET_SELF_IP_RESP:
	case ICF_CFG_SET_PORT_RANGE_REQ:			case ICF_CFG_SET_PORT_RANGE_RESP:

	case ICF_MM_INIT_COMPLETE:
#endif	  				
		ICF_CLIB_MEMFREE(p_msg)
		break;

    default:
		ICF_CLIB_MEMFREE(p_msg);
	    break;
	}

	return ret_val;
}


icf_return_t icf_decode_mm_msg_payload(icf_msg_st*  p_msg, 
							icf_uint8_t*  p_encoded_buf,
							icf_uint32_t  p_encoded_buf_length,
							icf_api_id_t  api_id,
							icf_error_t*  p_ecode)
{

	icf_uint32_t length      = ICF_NULL;
	icf_uint8_t* p_src       = ICF_NULL;
	icf_uint8_t* p_dest      = ICF_NULL;
	icf_uint32_t api_length  = ICF_NULL;
        icf_return_t ret_val     = ICF_SUCCESS;
        /*for warning removal */
        p_encoded_buf_length=p_encoded_buf_length;
 
  api_length = icf_get_api_length(api_id);

	if(0 == api_length)
	{
		ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - API id (%d) & API Length (%d) Mismatch\n", api_id, api_length));
        *p_ecode = ICF_ERR_INVALID_LENGTH;
        return ICF_FAILURE;
	}

	if (!p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Argument #2 p_encoded_buf is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

    ICF_CLIB_TRACE(("\n<======================== DECODE START ======================>\n"));



        p_dest =(icf_uint8_t *) p_msg;

	/* Move past the API header into the buffer*/
	p_src  = p_encoded_buf; 

/* Unpack the Payload according to API Types*/
	switch (api_id) 
	{

	/***************************************
	*	   Resource Management APIs
	****************************************/

	case ICF_GET_CODEC_LIST_REQ:
		ret_val = (icf_uint8_t)icf_unpack_icf_get_codec_list_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_CODEC_LIST_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_codec_list_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_codec_commit_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_codec_commit_resp_st(p_dest, p_src, &length);
		break;

	case ICF_REL_RES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_rel_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_open_media_channel_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_open_media_channel_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_close_media_channel_req_st(p_dest, p_src, &length);
		break;

	/***************************************
	*	   Media Control APIs
	****************************************/
	
	case ICF_CREATE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_create_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CREATE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_create_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_modify_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_modify_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_delete_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_DELETE_MEDIA_SESSION_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_delete_media_session_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_merge_media_session_req_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_capabilities_req_st(p_dest, p_src, &length);
		break;
/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_capabilities_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_UDP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_udp_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_RECV_UDP_DATA_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_recv_udp_data_ind_st(p_dest, p_src, &length);
		break;
/***************************************
*	   Common Application APIs
****************************************/
	
	case ICF_START_RINGING_REQ:
	case ICF_START_RINGING_RESP:
	case ICF_VIBRATION_REQ:
	case ICF_VIBRATION_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_REQ:
	case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_ECHO_CANC_REQ:
	case ICF_ECHO_CANC_RESP:
	case ICF_VAD_SS_CNG_REQ:
	case ICF_VAD_SS_CNG_RESP:
	case ICF_GET_STATS_REQ:
	case ICF_GET_STATS_RESP:
	case ICF_PLAY_MEDIA_REQ:
	case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_REQ:
	case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_STOP_MEDIA_PLAY_IND:
	case ICF_STOP_RINGING_REQ:
	case ICF_STOP_RINGING_RESP:
	case ICF_STOP_VIBRATION_REQ:
	case ICF_STOP_VIBRATION_RESP:
/*Case missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_STOP_RINGING_IND:

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
	case ICF_MUTE_MEDIA_REQ:
	case ICF_MUTE_MEDIA_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_MEDIA_RECORD_REQ:
	case ICF_MEDIA_RECORD_RESP:
	case ICF_SEND_DGTS_REQ:
	case ICF_SEND_DGTS_RESP:
	case ICF_SET_EARP_VOLUME_REQ:
	case ICF_SET_EARP_VOLUME_RESP:
	case ICF_SET_SPEAKER_VOLUME_REQ:
	case ICF_SET_SPEAKER_VOLUME_RESP:
	case ICF_STOP_MEDIA_RECORD_REQ:
	case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_SET_MIC_GAIN_REQ:
	case ICF_SET_MIC_GAIN_RESP:
	case ICF_PAUSE_MEDIA_REQ:
	case ICF_PAUSE_MEDIA_RESP:
/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
	case ICF_STOP_STATS_RESP:
	case ICF_RCV_APP_DATA_IND:
	case ICF_SEND_APP_DATA_REQ:
	case ICF_SEND_APP_DATA_RESP:
	case ICF_SEND_STATS_REQ:
	case ICF_SEND_STATS_RESP:

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_RESP:
	case ICF_CFG_RINGER_TONES_REQ:
	case ICF_CFG_RINGER_TONES_RESP:
	case ICF_CFG_MEDIA_REQ:
	case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_DIGIT_TRANS_REQ:
	case ICF_CFG_SET_DIGIT_TRANS_RESP:
	case ICF_CFG_JBM_MGMT_REQ:
	case ICF_CFG_JBM_MGMT_RESP:
	case ICF_CFG_MAX_EARP_VOL_REQ:
	case ICF_CFG_MAX_EARP_VOL_RESP:
	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
	case ICF_CFG_MAX_MIC_GAIN_REQ:
	case ICF_CFG_MAX_MIC_GAIN_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
/*Cases missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
	case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_CFG_SET_SELF_IP_REQ:
	case ICF_CFG_SET_SELF_IP_RESP:
	case ICF_CFG_SET_PORT_RANGE_REQ:
	case ICF_CFG_SET_PORT_RANGE_RESP:

	case ICF_MM_INIT_COMPLETE:
	case ICF_MM_SET_NW_STATUS:


#ifdef ICF_PLTFM_TEST_ENABLE
/***************************************
*	       PLATFORM APIs
****************************************/
	case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
	case ICF_PORT_PLTFM_PDP_MODIFY_IND:
	case ICF_PORT_PLTFM_PDP_DEACT_IND:
	case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:

#endif
#if 0
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - MEMCopying decoded Header to ICF Message\n"));
	ICF_CLIB_MEMCPY(&((*p_p_msg)->hdr), p_hdr, ICF_API_HEADER_LENGTH);
	ICF_CLIB_MEMCPY(&((*p_p_msg)->payload), p_src, api_length - ICF_API_HEADER_LENGTH);

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Freeing Header\n"));
	ICF_CLIB_MEMFREE(p_hdr)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Decoded Buffer Length (%d) Bytes\n", length));
	return ret_val;
#endif
	break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	/***************************************
	*	   Common Application APIs
	****************************************/
	
	case ICF_START_RINGING_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_start_ringing_req_st(p_dest, p_src, &length);
		break;

	case ICF_START_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_vibration_req_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_connect_media_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_echo_canc_req_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_vad_ss_cng_req_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_get_stats_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_play_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_play_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_play_ind_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_RINGING_REQ:
		break;

	case ICF_STOP_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_VIBRATION_REQ:
		break;

	case ICF_STOP_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_dgts_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_mute_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_mute_media_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_media_playback_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_dgts_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_earp_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_speaker_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_set_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_pause_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_stop_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_RCV_APP_DATA_IND:
		ret_val= (icf_uint8_t)icf_unpack_icf_rcv_app_data_ind_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_app_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_send_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

#endif

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_codec_info_req_st(p_dest, p_src, &length);
		break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	case ICF_CFG_CODEC_INFO_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_ringer_tones_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_set_digit_trans_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_jbm_mgmt_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_earp_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_speaker_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_cfg_max_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_unpack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_unpack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MM_INIT_COMPLETE:
		break;
#endif

	default:				
        length = api_length - ICF_API_HEADER_LENGTH;
        ret_val = (icf_uint8_t)icf_unpack_unknown_st(p_dest, p_src, &length);
        break;
	}
	

    if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - DECODING Failed While Unpacking!!!\n"));
		*p_ecode = ICF_ERR_DECODE_FAILED;
        return ret_val;
    }

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_decode_mm_msg - Decoded Buffer Length (%d) Bytes\n", length));
    ICF_CLIB_TRACE(("\n<========================= DECODE END =======================>\n"));
	return ret_val;
}


icf_return_t icf_encode_mm_api_payload(icf_msg_st*   p_msg, /* Input*/
							icf_uint8_t** p_p_encoded_buf, /* Output*/
							icf_uint32_t* p_encoded_buf_length,
							icf_error_t*  p_ecode)

{

	icf_uint32_t  length    = ICF_NULL;
	icf_uint8_t*  p_src     = ICF_NULL;
	icf_uint8_t*  p_dest    = ICF_NULL;
	icf_uint8_t*  p_buff    = ICF_NULL;
	icf_uint32_t api_length  = ICF_NULL;
	icf_uint32_t api_length_value  = ICF_NULL;
	icf_return_t ret_val    = ICF_SUCCESS;

        /* For Warning removal */
        api_length=api_length;
        api_length_value=api_length_value;


	if (!p_msg) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_msg - Argument #1 p_msg is NULL\n"));
		*p_ecode = ICF_ERR_NULL_POINTER;
		return ICF_FAILURE;
	}

	ICF_CLIB_TRACE(("\n<======================== ENCODE START ======================>\n"));


	/* Allocate a temporary buffer with the API length of the 
	   Input Message */

	ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_hdr\n"));
	p_buff   = (icf_uint8_t*)ICF_CLIB_MEMGET(ICF_CLIB_MAX_API_LENGTH);

	if (!p_buff) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure!! (p_buff)\n"));
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


         api_length = p_msg->hdr.api_length;
	/* Pack the rest of the payload*/
	p_src = (icf_uint8_t*)p_msg->payload;

	/* Make sure the destination points beyond the API header*/
	p_dest = p_buff; 


switch (p_msg->hdr.api_id) 
	{
	

	/***************************************
	*	   Resource Management APIs
	****************************************/

	case ICF_GET_CODEC_LIST_REQ:
		ret_val = (icf_uint8_t)icf_pack_icf_get_codec_list_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_GET_CODEC_LIST_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_get_codec_list_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_codec_commit_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CODEC_COMMIT_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_codec_commit_resp_st(p_dest, p_src, &length);
		break;

	case ICF_REL_RES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_rel_res_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_open_media_channel_req_st(p_dest, p_src, &length);
		break;

	case ICF_OPEN_MEDIA_CHANNEL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_open_media_channel_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_close_media_channel_req_st(p_dest, p_src, &length);
		break;

	/***************************************
	*	   Media Control APIs
	****************************************/
	
	case ICF_CREATE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_create_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_CREATE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_create_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_modify_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_MODIFY_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_modify_media_session_resp_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_delete_media_session_req_st(p_dest, p_src, &length);
		break;
	
	case ICF_DELETE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_DELETE_MEDIA_SESSION_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_delete_media_session_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_merge_media_session_req_st(p_dest, p_src, &length);
		break;

	case ICF_MERGE_MEDIA_SESSION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_media_capabilities_req_st(p_dest, p_src, &length);
		break;
/* ICF_6_1_OPTIONS_SUPPORT start */

	case ICF_MEDIA_CAPABILITIES_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_media_capabilities_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_UDP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_udp_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_RECV_UDP_DATA_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_recv_udp_data_ind_st(p_dest, p_src, &length);
		break;


/***************************************
*	   Common Application APIs
****************************************/
	
	case ICF_START_RINGING_REQ:
	case ICF_START_RINGING_RESP:
	case ICF_VIBRATION_REQ:
	case ICF_VIBRATION_RESP:
	case ICF_CONNECT_MEDIA_DEVICE_REQ:
	case ICF_CONNECT_MEDIA_DEVICE_RESP:
	case ICF_ECHO_CANC_REQ:
	case ICF_ECHO_CANC_RESP:
	case ICF_VAD_SS_CNG_REQ:
	case ICF_VAD_SS_CNG_RESP:
	case ICF_GET_STATS_REQ:
	case ICF_GET_STATS_RESP:
	case ICF_PLAY_MEDIA_REQ:
	case ICF_PLAY_MEDIA_RESP:
	case ICF_STOP_MEDIA_PLAY_REQ:
	case ICF_STOP_MEDIA_PLAY_RESP:
	case ICF_STOP_MEDIA_PLAY_IND:
	case ICF_STOP_RINGING_REQ:
	case ICF_STOP_RINGING_RESP:
	case ICF_STOP_VIBRATION_REQ:
	case ICF_STOP_VIBRATION_RESP:
/*Case missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_STOP_RINGING_IND:

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
	case ICF_MUTE_MEDIA_REQ:
	case ICF_MUTE_MEDIA_RESP:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
	case ICF_MEDIA_RECORD_REQ:
	case ICF_MEDIA_RECORD_RESP:
	case ICF_SEND_DGTS_REQ:
	case ICF_SEND_DGTS_RESP:
	case ICF_SET_EARP_VOLUME_REQ:
	case ICF_SET_EARP_VOLUME_RESP:
	case ICF_SET_SPEAKER_VOLUME_REQ:
	case ICF_SET_SPEAKER_VOLUME_RESP:
	case ICF_STOP_MEDIA_RECORD_REQ:
	case ICF_STOP_MEDIA_RECORD_RESP:
	case ICF_SET_MIC_GAIN_REQ:
	case ICF_SET_MIC_GAIN_RESP:
	case ICF_PAUSE_MEDIA_REQ:
	case ICF_PAUSE_MEDIA_RESP:
/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
	case ICF_STOP_STATS_RESP:
	case ICF_RCV_APP_DATA_IND:
	case ICF_SEND_APP_DATA_REQ:
	case ICF_SEND_APP_DATA_RESP:
	case ICF_SEND_STATS_REQ:
	case ICF_SEND_STATS_RESP:

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_RESP:
	case ICF_CFG_RINGER_TONES_REQ:
	case ICF_CFG_RINGER_TONES_RESP:
	case ICF_CFG_MEDIA_REQ:
	case ICF_CFG_MEDIA_RESP:
	case ICF_CFG_SET_DIGIT_TRANS_REQ:
	case ICF_CFG_SET_DIGIT_TRANS_RESP:
	case ICF_CFG_JBM_MGMT_REQ:
	case ICF_CFG_JBM_MGMT_RESP:
	case ICF_CFG_MAX_EARP_VOL_REQ:
	case ICF_CFG_MAX_EARP_VOL_RESP:
	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
	case ICF_CFG_MAX_MIC_GAIN_REQ:
	case ICF_CFG_MAX_MIC_GAIN_RESP:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
/*Cases missing in  ICF_MM_CFG_FUTURE_RELEASE flag*/
	case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
	case ICF_CFG_SET_VIDEO_ATTRIB_RESP:
	case ICF_CFG_SET_SELF_IP_REQ:
	case ICF_CFG_SET_SELF_IP_RESP:
	case ICF_CFG_SET_PORT_RANGE_REQ:
	case ICF_CFG_SET_PORT_RANGE_RESP:
	
	case ICF_MM_INIT_COMPLETE:
	case ICF_MM_SET_NW_STATUS:


#ifdef ICF_PLTFM_TEST_ENABLE
/***************************************
*	       PLATFORM APIs
****************************************/
	case ICF_PORT_PLTFM_SEC_PDP_ACT_RSP:
	case ICF_PORT_PLTFM_PDP_MODIFY_IND:
	case ICF_PORT_PLTFM_PDP_DEACT_IND:
	case ICF_PORT_PLTFM_PLATFORM_EVENT_IND:
	
#endif
#if 0
	length = icf_get_api_length(p_msg->hdr.api_id);
	length = length - ICF_API_HEADER_LENGTH;

	api_length_value = length + hdr_length;

	icf_pack_U16(p_buff + api_length_index, (icf_uint8_t*)&api_length_value, &hdr_length);

	length += ICF_API_HEADER_LENGTH;

	*p_encoded_buf_length = length;
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying API Header to encoded buffer\n"));
	/* Copy the encoded API header first*/
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_buff, ICF_API_HEADER_LENGTH)

	/* Copy the rest of the payload, move past the */
	/* encoded API Header*/
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf + ICF_API_HEADER_LENGTH, p_src, length - ICF_API_HEADER_LENGTH)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));

	return ret_val;
#endif
	break;

#ifdef ICF_MM_CFG_FUTURE_RELEASE

	/***************************************
	*	   Common Application APIs
	****************************************/
	
	case ICF_START_RINGING_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_start_ringing_req_st(p_dest, p_src, &length);
		break;

	case ICF_START_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_vibration_req_st(p_dest, p_src, &length);
		break;

	case ICF_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_connect_media_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_echo_canc_req_st(p_dest, p_src, &length);
		break;

	case ICF_ECHO_CANC_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_vad_ss_cng_req_st(p_dest, p_src, &length);
		break;

	case ICF_VAD_SS_CNG_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_get_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_GET_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_get_stats_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_play_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PLAY_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_play_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_PLAY_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_play_ind_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_RINGING_REQ:
		break;

	case ICF_STOP_RINGING_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_VIBRATION_REQ:
		break;

	case ICF_STOP_VIBRATION_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Video/Voice APIs
****************************************/

	case ICF_DGTS_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_dgts_ind_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_mute_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_MUTE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_mute_media_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_media_playback_device_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_dgts_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_DGTS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_earp_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_EARP_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_speaker_volume_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_SPEAKER_VOLUME_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_media_record_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_MEDIA_RECORD_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_set_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_SET_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_pause_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_PAUSE_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

/***************************************
*	       Push To Talk APIs
****************************************/

	case ICF_STOP_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_stop_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_STOP_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_RCV_APP_DATA_IND:
		ret_val= (icf_uint8_t)icf_pack_icf_rcv_app_data_ind_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_app_data_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_APP_DATA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_send_stats_req_st(p_dest, p_src, &length);
		break;

	case ICF_SEND_STATS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

#endif

/***************************************
*	       Configuration APIs
****************************************/

	case ICF_CFG_CODEC_INFO_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_codec_info_req_st(p_dest, p_src, &length);
		break;

	
#ifdef ICF_MM_CFG_FUTURE_RELEASE

	case ICF_CFG_CODEC_INFO_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_ringer_tones_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_RINGER_TONES_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_media_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MEDIA_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_set_digit_trans_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_DIGIT_TRANS_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_jbm_mgmt_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_JBM_MGMT_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_earp_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_EARP_VOL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_speaker_vol_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_SPEAKER_VOL_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_cfg_max_mic_gain_req_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_MAX_MIC_GAIN_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
		ret_val= (icf_uint8_t)icf_pack_icf_video_display_st(p_dest, p_src, &length);
		break;

	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP:
		ret_val= (icf_uint8_t)icf_pack_icf_msg_resp_st(p_dest, p_src, &length);
		break;

	case ICF_MM_INIT_COMPLETE:
		break;
#endif

	default:				
        if (ICF_API_HEADER_LENGTH < api_length)
        {
        length = api_length - ICF_API_HEADER_LENGTH;
        ret_val = (icf_uint8_t)icf_pack_unknown_st(p_dest, p_src, &length);
        }
        else
        {
            /* donot get into negative subtraction */
            length = 0;
        }
        break;

	}


	 if (ret_val == ICF_FAILURE) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - ENCODING Failed While Packing!!!\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_ENCODE_FAILED;
        return ret_val;
    }

	*p_encoded_buf_length = length;
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Getting Memory for p_p_encoded_buf\n"));
	*p_p_encoded_buf = (icf_uint8_t*)ICF_CLIB_MEMGET(length);

	if (!*p_p_encoded_buf) {
        ICF_CLIB_TRACE(("\n[ICF_CLIB]: ICF_CLIB_MEMGET Failure! (*p_p_encoded_buf)\n"));
	    ICF_CLIB_MEMFREE(p_buff)
		*p_ecode = ICF_ERR_MEMGET_FAILED;
		return ICF_FAILURE;
	}


	/* Copy the rest of the encoded payload, move past the 
	   encoded API Header */
    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - MEMCopying Payload to encoded buffer\n"));
	ICF_CLIB_MEMCPY(*p_p_encoded_buf, p_dest, length)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - FREEing p_buff\n"));
	ICF_CLIB_MEMFREE(p_buff)

    ICF_CLIB_TRACE(("\n[ICF_CLIB]: icf_encode_mm_msg - Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length));
    ICF_CLIB_TRACE(("\n<========================= ENCODE END =======================>\n"));
	
	return ret_val;
}
