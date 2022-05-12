#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "icf_types.h"
#include "icf_defs.h"
#include "global.h"
#include "voiceApp_main.h"

#if defined (IPTK_REL_8_2_2)	
char *iptk_api_id2str(icf_api_id_t api_id)
{
	char *api_str=NULL;

    switch(api_id)
    {
   // case ICF_CFG_CC_SET_SCALE_PARAMS_REQ         :api_str="ICF_CFG_CC_SET_SCALE_PARAMS_REQ";break;

    case ICF_CALL_TRANSFER_UNATTENDED_REQ  :api_str="ICF_CALL_TRANSFER_UNATTENDED_REQ";break;

    case  ICF_CALL_TRANSFER_ATTENDED_REQ         :api_str="ICF_CALL_TRANSFER_ATTENDED_REQ";break;

    case  ICF_REMOTE_CALL_TRANSFER_INITIATED_IND   :api_str="ICF_REMOTE_CALL_TRANSFER_INITIATED_IND";break;

    case ICF_CALL_MERGED_IND                    :api_str="ICF_CALL_MERGED_IND";break;

    case  ICF_CONFERENCE_REQ                    :api_str="ICF_CONFERENCE_REQ";break;

    case ICF_CONFERENCE_RESP					:api_str="ICF_CONFERENCE_RESP";break;

    case ICF_SET_SERVICE_PARAMS_REQ             :api_str="ICF_SET_SERVICE_PARAMS_REQ";break;

    case ICF_DISPLAY_IND                        :api_str="ICF_DISPLAY_IND";break;

    case ICF_CALL_REDIRECTION_IND               :api_str="ICF_CALL_REDIRECTION_IND";break;

    case ICF_SET_MGMT_PLANE_AUTH_PARAM          :api_str="ICF_SET_MGMT_PLANE_AUTH_PARAM"; break;

    case ICF_CREATE_SECURE_CONN_REQ             :api_str="ICF_CREATE_SECURE_CONN_REQ"; break;

    case ICF_CREATE_SECURE_CONN_RESP            :api_str="ICF_CREATE_SECURE_CONN_RESP"; break;

    case ICF_CLOSE_SECURE_CONN_REQ              :api_str="ICF_CLOSE_SECURE_CONN_REQ"; break;

    case ICF_CLOSE_SECURE_CONN_RESP             :api_str="ICF_CLOSE_SECURE_CONN_RESP"; break;

    case ICF_SECURE_CONN_STATUS_IND             :api_str="ICF_SECURE_CONN_REQ"; break;

    case ICF_XCAP_CREATE_REPLACE_RES            :api_str="ICF_XCAP_CREATE_REPLACE_RES"; break;

    case ICF_XCAP_DELETE_RES                   :api_str="ICF_XCAP_DELETE_RES"; break;

    case ICF_XCAP_FETCH_RES                    :api_str="ICF_XCAP_FETCH_RES"; break;

    case ICF_SYNCML_SEND_REQ                   :api_str="ICF_SYNCML_SEND_REQ"; break;

   case ICF_MGMT_PLANE_API_PROC_FAILURE        :api_str="ICF_MGMT_PLANE_API_PROC_FAILURE"; break;

    case ICF_MGMT_PLANE_NW_RESP                :api_str="ICF_MGMT_PLANE_NW_RESP";break;

    case ICF_RES_UNAVAIL_IND                   :api_str="ICF_RES_UNAVAIL_IND";break;

    case ICF_SET_STATS_REQ                     :api_str="ICF_SET_STATS_REQ";break;

    case ICF_CALL_TRANSFER_UNATTENDED_RESP     :api_str="ICF_CALL_TRANSFER_UNATTENDED_RESP"; break;

    case ICF_CALL_TRANSFER_ATTENDED_RESP       :api_str="ICF_CALL_TRANSFER_ATTENDED_RESP"; break;

    case ICF_CALL_FORWARDED_IND                :api_str="ICF_CALL_FORWARDED_IND"; break;

    case ICF_LOGS_IND                          :api_str="ICF_LOGS_IND";break;

    case ICF_CFG_SET_VIDEO_ATTRIB_REQ          :api_str="ICF_CFG_SET_VIDEO_ATTRIB_REQ";break;

    case ICF_MM_SET_NW_STATUS                  :api_str="ICF_MM_SET_NW_STATUS";break;

    case ICF_SET_LINE_PARAM_REQ                :api_str="ICF_SET_LINE_PARAM_REQ"; break;

    case ICF_API_ID_INVALID                    :api_str="ICF_API_ID_INVALID";break;

#ifdef ICF_NW_ACTIVATE

    case ICF_EVENT_INDICATION                  :api_str="ICF_EVENT_INDICATION";break;

#endif

    case ICF_APP_ADD_REQ                       :api_str="ICF_APP_ADD_REQ";break;

    case ICF_APP_ADD_CFM                       :api_str="ICF_APP_ADD_CFM";break;

    case ICF_APP_REMOVE_REQ                    :api_str="ICF_APP_REMOVE_REQ";break;

    case ICF_APP_REMOVE_CFM                    :api_str="ICF_APP_REMOVE_CFM";break;

#ifdef ICF_NW_ACTIVATE

    case ICF_NETWORK_ACTIVATE_REQ              :api_str="ICF_NETWORK_ACTIVATE_REQ";break;

    case ICF_NETWORK_ACTIVATE_CFM              :api_str="ICF_NETWORK_ACTIVATE_CFM";break;

    case ICF_NETWORK_DEACTIVATE_REQ            :api_str="ICF_NETWORK_DEACTIVATE_REQ";break;

    case ICF_NETWORK_DEACTIVATE_CFM            :api_str="ICF_NETWORK_DEACTIVATE_CFM";break;

#endif

    case ICF_SET_TRANSPORT_REQ                 :api_str="ICF_SET_TRANSPORT_REQ";break;

    case ICF_SET_CALL_PARAM_REQ                :api_str="ICF_SET_CALL_PARAM_REQ";break;

    case ICF_SET_SELF_ID_REQ                   :api_str="ICF_SET_SELF_ID_REQ";break;

    case ICF_SET_REGISTRATION_REQ              :api_str="ICF_SET_REGISTRATION_REQ";break;

    case ICF_CONFIGURE_CFM                     :api_str="ICF_CONFIGURE_CFM";break;

	case ICF_GET_SYSTEM_SETTINGS_REQ           :api_str="ICF_GET_SYSTEM_SETTINGS_REQ";break;  

	case ICF_GET_LINE_SETTINGS_REQ             :api_str="ICF_GET_LINE_SETTINGS_REQ";break;   

	case ICF_GET_SYSTEM_SETTINGS_RESP          :api_str="ICF_GET_SYSTEM_SETTINGS_RESP";break; 

	case ICF_GET_LINE_SETTINGS_RESP            :api_str="ICF_GET_LINE_SETTINGS_RESP";break; 

    case ICF_REGISTER_REQ                      :api_str="ICF_REGISTER_REQ";break;

    case ICF_REGISTER_CFM                      :api_str="ICF_REGISTER_CFM";break;

    case ICF_REGISTER_STATUS_IND               :api_str="ICF_REGISTER_STATUS_IND";break;

    case ICF_CREATE_CALL_REQ                   :api_str="ICF_CREATE_CALL_REQ";break;

    case ICF_LOCAL_USER_ALERTED_REQ            :api_str="ICF_LOCAL_USER_ALERTED_REQ";break;
    
    case ICF_MEDIA_CONNECT_REQ                 :api_str="ICF_MEDIA_CONNECT_REQ";break;
 
    case ICF_TERMINATE_CALL_REQ                :api_str="ICF_TERMINATE_CALL_REQ";break;

    case ICF_LOCAL_USER_TERMINATE_CALL_RESP    :api_str="ICF_LOCAL_USER_TERMINATE_CALL_RESP";break;

    case ICF_CONNECT_REQ                       :api_str="ICF_CONNECT_REQ";break;

    case ICF_SUBSCRIBE_REQ                     :api_str="ICF_SUBSCRIBE_REQ";break;

    case ICF_PUBLISH_REQ                       :api_str="ICF_PUBLISH_REQ";break;

    case ICF_MESSAGE_REQ                       :api_str="ICF_MESSAGE_REQ";break;

    case ICF_REFER_REQ                         :api_str="ICF_REFER_REQ";break;

    case ICF_CALL_HOLD_REQ                     :api_str="ICF_CALL_HOLD_REQ";break;

    case ICF_CALL_HOLD_RESP                    :api_str="ICF_CALL_HOLD_RESP";break;

    case ICF_CALL_RESUME_REQ                   :api_str="ICF_CALL_RESUME_REQ";break;

    case ICF_CALL_RESUME_RESP                  :api_str="ICF_CALL_RESUME_RESP";break;

    case ICF_INCOMING_CALL_IND                 :api_str="ICF_INCOMING_CALL_IND";break;

	case ICF_NOTIFY_CFM                         :api_str="ICF_NOTIFY_CFM";break;

	case ICF_INFO_REQ                          :api_str="ICF_INFO_REQ";break;

	case ICF_INFO_RESP                          :api_str="ICF_INFO_RESP";break;

	case ICF_INFO_IND	                        :api_str="ICF_INFO_IND";break;
	case ICF_OPTIONS_REQ	                    :api_str="ICF_OPTIONS_REQ";break;
    case ICF_OPTIONS_RESP	                    :api_str="ICF_OPTIONS_RESP";break;
    case ICF_INC_OPTIONS_IND                    :api_str="ICF_INC_OPTIONS_IND";break;
    case ICF_OPTIONS_CFM                        :api_str="ICF_OPTIONS_CFM";break;
    case ICF_INFO_CFM                           :api_str="ICF_INFO_CFM";break;
    case ICF_RESTART_REQ                           :api_str="ICF_RESTART_REQ";break;
    case ICF_RESTART_IND                           :api_str="ICF_RESTART_IND";break;
    case ICF_RESTART_RESP                           :api_str="ICF_RESTART_RESP";break;
    case ICF_REINITIALISE_REQ                           :api_str="ICF_REINITIALISE_REQ";break;
    case ICF_REINIT_IND                           :api_str="ICF_REINIT_IND";break;
    case ICF_REINIT_RESP                           :api_str="ICF_REINIT_RESP";break;

//ICF_HANDLE_REINVITE_START

	case ICF_CALL_MODIFY_REQ	                :api_str="ICF_CALL_MODIFY_REQ";break;

	case ICF_CALL_MODIFY_RESP	                :api_str="ICF_CALL_MODIFY_RESP";break;

	case ICF_INCOMING_CALL_MODIFY_IND           :api_str="ICF_INCOMING_CALL_MODIFY_IND";break;

	case ICF_INCOMING_CALL_MODIFY_CANCEL_IND    :api_str="ICF_INCOMING_CALL_MODIFY_CANCEL_IND";break;

	case ICF_CALL_MODIFY_CFM		            :api_str="ICF_CALL_MODIFY_CFM";break;

//ICF_HANDLE_REINVITE_END



    case ICF_REMOTE_USER_ALERTED_IND           :api_str="ICF_REMOTE_USER_ALERTED_IND";break;

    case ICF_TERMINATE_CALL_IND                :api_str="ICF_TERMINATE_CALL_IND";break;

    case ICF_REMOTE_USER_TERMINATE_CALL_RESP   :api_str="ICF_REMOTE_USER_TERMINATE_CALL_RESP";break;

    case ICF_MEDIA_CONNECTED_IND               :api_str="ICF_MEDIA_CONNECTED_IND";break;

    case ICF_CONNECT_IND                       :api_str="ICF_CONNECT_IND";break;

	case ICF_PARTY_REPLACED_IND            	:api_str="ICF_PARTY_REPLACED_IND";break;

    case ICF_SUBSCRIPTION_STATUS_IND           :api_str="ICF_SUBSCRIPTION_STATUS_IND";break;

    case ICF_NOTIFY_IND                        :api_str="ICF_NOTIFY_IND";break;

    case ICF_PUBLISH_RESP                      :api_str="ICF_PUBLISH_RESP";break;

    case ICF_MESSAGE_RESP                      :api_str="ICF_MESSAGE_RESP";break;

    case ICF_MESSAGE_IND                       :api_str="ICF_MESSAGE_IND";break;

    case ICF_REFER_RESP                        :api_str="ICF_REFER_RESP";break;

	
	case ICF_CONNECT_RESP                        :api_str="ICF_CONNECT_RESP";break;
    
	case ICF_START_FAX_REQ                        :api_str="ICF_START_FAX_REQ";break;
	
	case ICF_START_FAX_RESP                        :api_str="ICF_START_FAX_RESP";break;

	case ICF_START_FAX_IND                        :api_str="ICF_START_FAX_IND";break;


	case ICF_CFG_CODEC_INFO_REQ                :api_str="ICF_CFG_CODEC_INFO_REQ";break;

    case ICF_CFG_CODEC_INFO_RESP               :api_str="ICF_CFG_CODEC_INFO_RESP";break;

    case ICF_CFG_RINGER_TONES_REQ              :api_str="ICF_CFG_RINGER_TONES_REQ";break;

    case ICF_CFG_RINGER_TONES_RESP             :api_str="ICF_CFG_RINGER_TONES_RESP";break;

    case ICF_CFG_JBM_MGMT_REQ                  :api_str="ICF_CFG_JBM_MGMT_REQ";break;

    case ICF_CFG_JBM_MGMT_RESP                 :api_str="ICF_CFG_JBM_MGMT_RESP";break;

    case ICF_CFG_MAX_EARP_VOL_REQ              :api_str="ICF_CFG_MAX_EARP_VOL_REQ";break;

    case ICF_CFG_MAX_EARP_VOL_RESP             :api_str="ICF_CFG_MAX_EARP_VOL_RESP";break;

    case ICF_CFG_MAX_SPEAKER_VOL_REQ           :api_str="ICF_CFG_MAX_SPEAKER_VOL_REQ";break;

    case ICF_CFG_MAX_SPEAKER_VOL_RESP          :api_str="ICF_CFG_MAX_SPEAKER_VOL_RESP";break;

    case ICF_CFG_SET_PORT_RANGE_REQ                :api_str="ICF_CFG_PORT_RANGE_REQ";break;

    case ICF_CFG_SET_PORT_RANGE_RESP               :api_str="ICF_CFG_PORT_RANGE_RESP";break;

    case ICF_START_RINGING_REQ                 :api_str="ICF_START_RINGING_REQ";break;

    case ICF_START_RINGING_RESP                :api_str="ICF_START_RINGING_RESP";break;

    case ICF_MEDIA_SESSION_REQ                 :api_str="ICF_MEDIA_SESSION_REQ";break;

    case ICF_MEDIA_SESSION_RESP                :api_str="ICF_MEDIA_SESSION_RESP";break;

    case ICF_RTP_START_IND                :api_str="ICF_RTP_START_IND";break;

    case ICF_GET_STATS_REQ                     :api_str="ICF_GET_STATS_REQ";break;

    case ICF_GET_STATS_RESP                   :api_str="ICF_GET_STATS_RESP";break;

    case ICF_STOP_RINGING_REQ                 :api_str="ICF_STOP_RINGING_REQ";break;

    case ICF_STOP_RINGING_RESP                :api_str="ICF_STOP_RINGING_RESP";break;

    case ICF_STOP_RINGING_IND                 :api_str="ICF_STOP_RINGING_IND";break;

	
	case ICF_MUTE_MEDIA_REQ                   :api_str="ICF_MUTE_MEDIA_REQ";break;


    case ICF_MUTE_MEDIA_RESP                  :api_str="ICF_MUTE_MEDIA_RESP";break;

	
	case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ    :api_str="ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ";break;

    case ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP   :api_str="ICF_SET_MEDIA_PLAYBACK_DEVICE_RESP";break;
    
	
	case ICF_PLAY_MEDIA_REQ					  :api_str="ICF_PLAY_MEDIA_REQ";break;
  
    case ICF_PLAY_MEDIA_RESP			      :api_str="ICF_PLAY_MEDIA_RESP";break;


    case ICF_SET_EARP_VOLUME_REQ              :api_str="ICF_SET_EARP_VOLUME_REQ";break;

    case ICF_SET_EARP_VOLUME_RESP             :api_str="ICF_SET_EARP_VOLUME_RESP";break;

    case ICF_SET_SPEAKER_VOLUME_REQ           :api_str="ICF_SET_SPEAKER_VOLUME_REQ";break;

    case ICF_SET_SPEAKER_VOLUME_RESP          :api_str="ICF_SET_SPEAKER_VOLUME_RESP";break;

    case ICF_SET_MIC_GAIN_REQ				   :api_str="ICF_SET_MIC_GAIN_REQ";break;

   	case ICF_SET_MIC_GAIN_RESP			       :api_str="ICF_SET_MIC_GAIN_RESP";break;

    case ICF_CFG_MAX_MIC_GAIN_REQ			   :api_str="ICF_CFG_MAX_MIC_GAIN_REQ";break;

	case ICF_CFG_MAX_MIC_GAIN_RESP			   :api_str="ICF_CFG_MAX_MIC_GAIN_RESP";break;

    case ICF_SEND_APP_DATA_REQ                 :api_str="ICF_SEND_APP_DATA_REQ";break;

    case ICF_SEND_APP_DATA_RESP                :api_str="ICF_SEND_APP_DATA_RESP";break;

    case ICF_RCV_APP_DATA_IND                  :api_str="ICF_RCV_APP_DATA_IND";break;

    case ICF_SEND_STATS_REQ                    :api_str="ICF_SEND_STATS_REQ";break;

    case ICF_SEND_STATS_RESP                   :api_str="ICF_SEND_STATS_RESP";break;

    case ICF_STOP_STATS_REQ                    :api_str="ICF_STOP_STATS_REQ";break;

    case ICF_STOP_STATS_RESP                   :api_str="ICF_STOP_STATS_RESP";break;

	case ICF_MSG_FRM_PLTFM		       :api_str="ICF_MSG_FRM_PLTFM";break;	

    case ICF_NW_API_MSG_FROM_PEER            :api_str="ICF_NW_API_MSG_FROM_PEER";break;

    case ICF_NW_API_OPEN_CONN_IND            :api_str="ICF_NW_API_OPEN_CONN_IND";break;

    case ICF_NW_API_CONNECT_SUCCESS          :api_str="ICF_NW_API_CONNECT_SUCCESS";break;

    case ICF_NW_API_CONNECT_FAILURE          :api_str="ICF_NW_API_CONNECT_FAILURE";break;

    case ICF_NW_API_CLOSE_CONN_IND            :api_str="ICF_NW_API_CLOSE_CONN_IND";break;

    case ICF_TIMER_MSG                        :api_str="ICF_TIMER_MSG";break;


#ifdef ICF_DEBUG_MEMPOOL

	case ICF_MEM_POOL_STATS_PRINT            :api_str="ICF_MEM_POOL_STATS_PRINT";break;

#endif        

    case ICF_VIBRATION_REQ					:api_str="ICF_VIBRATION_REQ";break;

    case ICF_VIBRATION_RESP					:api_str="ICF_VIBRATION_RESP";break;

    case ICF_ECHO_CANC_REQ					:api_str="ICF_ECHO_CANC_REQ";break;

    case ICF_ECHO_CANC_RESP					:api_str="ICF_ECHO_CANC_RESP";break;

    case ICF_VAD_SS_CNG_REQ					:api_str="ICF_VAD_SS_CNG_REQ";break;

    case ICF_VAD_SS_CNG_RESP				:api_str="ICF_VAD_SS_CNG_RESP";break;
 
    case ICF_STOP_MEDIA_PLAY_REQ			:api_str="ICF_STOP_MEDIA_PLAY_REQ";break;

    case ICF_STOP_MEDIA_PLAY_RESP			:api_str="ICF_STOP_MEDIA_PLAY_RESP";break;

    case ICF_STOP_MEDIA_PLAY_IND			:api_str="ICF_STOP_MEDIA_PLAY_IND";break;

    case ICF_STOP_VIBRATION_REQ				:api_str="ICF_STOP_VIBRATION_REQ";break;

    case ICF_STOP_VIBRATION_RESP			:api_str="ICF_STOP_VIBRATION_RESP";break;

    case ICF_CFG_MEDIA_REQ					:api_str="ICF_CFG_MEDIA_REQ";break;

    case ICF_CFG_MEDIA_RESP					:api_str="ICF_CFG_MEDIA_RESP";break;

    case ICF_CFG_SET_DIGIT_TRANS_REQ		:api_str="ICF_CFG_SET_DIGIT_TRANS_REQ";break;

    case ICF_CFG_SET_DIGIT_TRANS_RESP		:api_str="ICF_CFG_SET_DIGIT_TRANS_RESP";break;

    case ICF_CFG_SET_SELF_IP_REQ			:api_str="ICF_CFG_SET_SELF_IP_REQ";break;

    case ICF_CFG_SET_SELF_IP_RESP			:api_str="ICF_CFG_SET_SELF_IP_RESP";break;


    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ	:api_str="ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ";break;

    case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP	:api_str="ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_RESP";break;
	
	case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ	:api_str="ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ";break;

    case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP	:api_str="ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_RESP";break;

	
	case ICF_DGTS_IND				:api_str="ICF_DGTS_IND";break;  

    case ICF_CONNECT_MEDIA_DEVICE_REQ		:api_str="ICF_CONNECT_MEDIA_DEVICE_REQ";break;

	case ICF_CONNECT_MEDIA_DEVICE_RESP		:api_str="ICF_CONNECT_MEDIA_DEVICE_RESP";break;

    case ICF_MEDIA_RECORD_REQ				:api_str="ICF_MEDIA_RECORD_REQ";break;

	case ICF_MEDIA_RECORD_RESP				:api_str="ICF_MEDIA_RECORD_RESP";break;

	case ICF_PAUSE_MEDIA_REQ				:api_str="ICF_PAUSE_MEDIA_REQ";break;

	case ICF_PAUSE_MEDIA_RESP				:api_str="ICF_PAUSE_MEDIA_RESP";break;


    case ICF_SEND_DGTS_REQ					:api_str="ICF_SEND_DGTS_REQ";break;

    case ICF_SEND_DGTS_RESP					:api_str="ICF_SEND_DGTS_RESP";break;

    case ICF_STOP_MEDIA_RECORD_REQ			:api_str="ICF_STOP_MEDIA_RECORD_REQ";break;

	case ICF_STOP_MEDIA_RECORD_RESP			:api_str="ICF_STOP_MEDIA_RECORD_RESP";break;


    case ICF_CREATE_MEDIA_SESSION_REQ    :api_str="ICF_CREATE_MEDIA_SESSION_REQ";break;     

    case ICF_CREATE_MEDIA_SESSION_RESP   :api_str="ICF_CREATE_MEDIA_SESSION_RESP";break;  

    case ICF_MODIFY_MEDIA_SESSION_REQ    :api_str="ICF_MODIFY_MEDIA_SESSION_REQ";break;     

    case ICF_MODIFY_MEDIA_SESSION_RESP   :api_str="ICF_MODIFY_MEDIA_SESSION_RESP";break;  

    case ICF_DELETE_MEDIA_SESSION_REQ    :api_str="ICF_DELETE_MEDIA_SESSION_REQ";break;   

    case ICF_DELETE_MEDIA_SESSION_RESP  :api_str="ICF_DELETE_MEDIA_SESSION_RESP";break;  

    case ICF_DELETE_MEDIA_SESSION_IND    :api_str="ICF_DELETE_MEDIA_SESSION_IND";break;     

    case ICF_MERGE_MEDIA_SESSION_REQ    :api_str="ICF_MERGE_MEDIA_SESSION_REQ";break;    

    case ICF_MERGE_MEDIA_SESSION_RESP    :api_str="ICF_MERGE_MEDIA_SESSION_RESP";break;    

    case ICF_GET_CODEC_LIST_REQ         :api_str="ICF_GET_CODEC_LIST_REQ";break;   

    case ICF_GET_CODEC_LIST_RESP        :api_str="ICF_GET_CODEC_LIST_RESP";break;    

    case ICF_CODEC_COMMIT_REQ            :api_str="ICF_CODEC_COMMIT_REQ";break;     

    case ICF_CODEC_COMMIT_RESP           :api_str="ICF_CODEC_COMMIT_RESP";break;  

    case ICF_REL_RES_REQ                 :api_str="ICF_REL_RES_REQ";break;    
	
	case ICF_MEDIA_CAPABILITIES_REQ      :api_str="ICF_MEDIA_CAPABILITIES_REQ";break;     

    case ICF_MEDIA_CAPABILITIES_RESP     :api_str="ICF_MEDIA_CAPABILITIES_RESP";break;  

	case ICF_OPEN_MEDIA_CHANNEL_REQ			 :api_str="ICF_OPEN_CHANNEL_REQ";break;     

    case ICF_OPEN_MEDIA_CHANNEL_RESP     :api_str="ICF_OPEN_MEDIA_CHANNEL_RESP";break;  

	case ICF_CLOSE_MEDIA_CHANNEL_REQ     :api_str="ICF_CLOSE_MEDIA_CHANNEL_REQ";break;     


    case ICF_SEND_UDP_DATA_REQ			 :api_str="ICF_SEND_UDP_DATA_REQ";break;  

	case ICF_RECV_UDP_DATA_IND			 :api_str="ICF_RECV_UDP_DATA_IND";break;

	case ICF_PORT_PLTFM_SA_CREATE_REQ         :api_str="ICF_WRPR_SA_CREATE_REQ";break;

    case ICF_PORT_PLTFM_SA_CREATE_RESP        :api_str="ICF_WRPR_SA_CREATE_RESP";break;

    case ICF_PORT_PLTFM_SA_MODIFY_REQ         :api_str="ICF_WRPR_SA_MODIFY_REQ";break;

    case ICF_PORT_PLTFM_SA_MODIFY_RESP        :api_str="ICF_WRPR_SA_MODIFY_RESP";break;

    case ICF_PORT_PLTFM_SA_DELETE_REQ         :api_str="ICF_WRPR_SA_DELETE_REQ";break;

    case ICF_PORT_PLTFM_SA_DELETE_RESP        :api_str="ICF_WRPR_SA_DELETE_RESP";break;

    case ICF_PORT_PLTFM_SA_EXPIRY_IND         :api_str="ICF_WRPR_SA_EXPIRY_IND";break;

    case ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_REQ   :api_str="ICF_WRPR_SA_FETCH_ENC_ALGO";break;

    case ICF_PORT_PLTFM_SA_FETCH_AUTH_ALGO_RESP :api_str="ICF_WRPR_SA_FETCH_AUTH_ALGO";break;

	case ICF_MM_INIT_COMPLETE :api_str="ICF_MM_INIT_COMPLETE";break;

	case ICF_STOP_FAX_REQ            :api_str="ICF_STOP_FAX_REQ";break;
	case ICF_STOP_FAX_RESP            :api_str="ICF_STOP_FAX_RESP";break;
	case ICF_STOP_FAX_IND            :api_str="ICF_STOP_FAX_IND";break;
	case ICF_SET_TRACE_REQ            :api_str="ICF_SET_TRACE_REQ";break;
	case ICF_REPORT_TRACE_IND         :api_str="ICF_REPORT_TRACE_IND";break;
        case ICF_CALL_MODIFY_CANCEL_RESP      :api_str="ICF_CALL_MODIFY_CANCEL_RESP";break;
    case ICF_PROGRESS_IND                 :api_str="ICF_PROGRESS_IND";break;
    case ICF_SET_ERROR_REPORT_LEVEL_REQ   :api_str="ICF_SET_ERROR_REPORT_LEVEL_REQ";break;
    case ICF_STATS_REQ                    :api_str="ICF_STATS_REQ";break;
    case ICF_REPORT_ERROR_IND             :api_str="ICF_REPORT_ERROR_IND";break;
    case ICF_STATS_RESP                   :api_str="ICF_STATS_RESP";break;

/******* Traces Add Start for SPR 17213 *******/

        case ICF_CREATE_XCONF_REQ:
             api_str="ICF_CREATE_XCONF_REQ";
             break;
        case ICF_ADD_XCONF_PARTY_REQ:
             api_str="ICF_ADD_XCONF_PARTY_REQ";
             break;
        case ICF_DELETE_XCONF_PARTY_REQ:
             api_str="ICF_DELETE_XCONF_PARTY_REQ";
             break;
        case ICF_RELEASE_XCONF_REQ:
             api_str="ICF_RELEASE_XCONF_REQ";
             break;
        case ICF_RELEASE_XCONF_RESP:
             api_str="ICF_RELEASE_XCONF_RESP";
             break;
        case ICF_CREATE_XCONF_RESP:
             api_str="ICF_CREATE_XCONF_RESP";
             break;
        case ICF_ADD_XCONF_PARTY_RESP:
             api_str="ICF_ADD_XCONF_PARTY_RESP";
             break;
        case ICF_DELETE_XCONF_PARTY_RESP:
             api_str="ICF_DELETE_XCONF_PARTY_RESP";
             break;
        case ICF_RELEASE_XCONF_IND:
             api_str="ICF_RELEASE_XCONF_IND";
             break;
        case ICF_EARLY_INC_CALL_IND:
             api_str="ICF_EARLY_INC_CALL_IND";
             break;
        case ICF_EARLY_INC_CALL_CFM:
             api_str="ICF_EARLY_INC_CALL_CFM";
             break;
        case ICF_UNEXPECTED_MSG_IND:
             api_str="ICF_UNEXPECTED_MSG_IND";
             break;
        case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
             api_str="ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM";
             break;
        case ICF_GET_REGISTER_STATUS_REQ:
             api_str="ICF_GET_REGISTER_STATUS_REQ";
             break;
        case ICF_GET_REGISTER_STATUS_RESP:
             api_str="ICF_GET_REGISTER_STATUS_RESP";
             break;
        case ICF_AKA_AUTH_RESP:
             api_str="ICF_AKA_AUTH_RESP";
             break;
        case ICF_MM_UNEXPECTED_MSG_IND:
             api_str="ICF_MM_UNEXPECTED_MSG_IND";
             break;
 
        case ICF_SESSION_TIMER_TOGGLE_REQ:
             api_str="ICF_SESSION_TIMER_TOGGLE_REQ";
             break; 

         case ICF_SESSION_TIMER_TOGGLE_RESP:
             api_str="ICF_SESSION_TIMER_TOGGLE_RESP";
             break;

	 case ICF_GENERIC_MSG_REQ:
	     api_str="ICF_GENERIC_MSG_REQ";
	     break; 


	 case ICF_GENERIC_MSG_RESP:
	     api_str="ICF_GENERIC_MSG_RESP";
	     break;	

	 case ICF_GENERIC_MSG_IND:
	     api_str="ICF_GENERIC_MSG_IND";
	     break;	

	 case ICF_GENERIC_MSG_CFM:
	     api_str="ICF_GENERIC_MSG_CFM";
	     break;
     /*Start Added for SPR 19473 */
	 case ICF_PRACK_REQ_RECV_IND:
	     api_str="ICF_PRACK_REQ_RECV_IND";
	     break;
	 case ICF_PRACK_RESP_SENT_IND:
	     api_str="ICF_PRACK_RESP_SENT_IND";
	     break;
	 case ICF_PRACK_REQ_SENT_IND:
	     api_str="ICF_PRACK_REQ_SENT_IND";
	     break;
	 case ICF_PRACK_RESP_RECV_IND:
	     api_str="ICF_PRACK_RESP_RECV_IND";
	     break;
     /*End SPR 19473 */
     case ICF_MESSAGE_CFM:
         api_str="ICF_MESSAGE_CFM";
         break;
	 
	 case ICF_TRACE_NW_BUFF_IND:
	 	api_str="ICF_TRACE_NW_BUFF_IND";
	 	 break;	

/******* Traces Add End for SPR 17213 *******/


    default:api_str="UNKNOWN_API_ID";break ;    

    }
	return api_str;

}
#endif

