/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/


#include "mm_macros.h"
#include "icf_clib.h"
#include "mm_cfg_types.h"
#include "mm_port_prototypes.h"
#include "icf_error_id.h"
#include "mm_glb.h"

/* This is universal application id*/

#if KSOCKET_RTP_SUPPORT
extern uint8 mmKSocketEnable;
#endif


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgCodecInfo
 *
 *  DESCRIPTION    :  ICF_CFG_CODEC_INFO_REQ
 *  This API is used by configuration module to configure Codec parameters in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgCodecInfo(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_codec_info_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_uint8_t	count = 0;
	icf_list_st	*p_encoded_params_list= MM_NULL;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	icf_string_st	*p_encoded_params_tr = MM_NULL;

MM_PRINTF("\n\tMM >Enter mmCfgCodecInfo \n");

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_codec_info_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));
	
	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	line_id = p_input_payload->line_id;
	for(count = 0; count < p_input_payload->count; count++)
	{
		p_mm_cfg_db->codec_info[line_id].line_id = line_id;
		p_mm_cfg_db->codec_info[line_id].codec_supp[count].bitmask = 0;
		p_mm_cfg_db->codec_info[line_id].codec_supp[count].p_encoded_params = MM_NULL;

		switch(p_input_payload->codec_supp[count].codec_type){
			case ICF_CODEC_TYPE_AUDIO:
				zyMemcpy(&(p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_param.audio_param), &(p_input_payload->codec_supp[count].codec_param.audio_param), sizeof(icf_config_audio_codec_param_st));
				break;
			case ICF_CODEC_TYPE_VIDEO:
				zyMemcpy(&(p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_param.video_param), &(p_input_payload->codec_supp[count].codec_param.video_param), sizeof(icf_config_video_codec_param_st));
				break;
			case ICF_CODEC_TYPE_IMAGE:
				zyMemcpy(&(p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_param.t38_param), &(p_input_payload->codec_supp[count].codec_param.t38_param), sizeof(icf_config_t38_attrib_param_st));
				break;
		}

		zyStrcpy(p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_name.str, 
			p_input_payload->codec_supp[count].codec_name.str);

		p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_name.str_len = 
			p_input_payload->codec_supp[count].codec_name.str_len;

		p_mm_cfg_db->codec_info[line_id].codec_supp[count].enable = p_input_payload->codec_supp[count].enable;
		p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_type = p_input_payload->codec_supp[count].codec_type;
		
		if(p_input_payload->codec_supp[count].bitmask & ICF_CODEC_NUM_PRESENT)
		{
			p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_num = p_input_payload->codec_supp[count].codec_num;
			p_mm_cfg_db->codec_info[line_id].codec_supp[count].bitmask |= ICF_CODEC_NUM_PRESENT;
		}

		if(p_input_payload->codec_supp[count].bitmask & ICF_CODEC_PARAM_PRESENT)
		{
			zyMemcpy(&p_mm_cfg_db->codec_info[line_id].codec_supp[count].codec_param, 
				&p_input_payload->codec_supp[count].codec_param, sizeof(union icf_codec_params_ut));

			p_mm_cfg_db->codec_info[line_id].codec_supp[count].bitmask |= ICF_CODEC_PARAM_PRESENT;
		}

		if(p_input_payload->codec_supp[count].bitmask & ICF_ENCODED_PARAM_PRESENT)
		{
			p_encoded_params_list = p_input_payload->codec_supp[count].p_encoded_params;
			while(MM_NULL != p_encoded_params_list)
			{
				p_encoded_params_tr = zyMalloc(sizeof(icf_string_st));
				zyMemcpy(p_encoded_params_tr, p_encoded_params_list->p_data, sizeof(icf_string_st));
				mmAddNodeToList(&p_mm_cfg_db->codec_info[line_id].codec_supp[count].p_encoded_params, p_encoded_params_tr);

				p_mm_cfg_db->codec_info[line_id].codec_supp[count].bitmask |= ICF_ENCODED_PARAM_PRESENT;
				p_encoded_params_list = p_encoded_params_list->p_next;
			}
		}
	}
	p_mm_cfg_db->codec_info[line_id].count = p_input_payload->count;

	/* Set and check input parameters here */
	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_CODEC_INFO_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}
	
MM_PRINTF("\n\tMM >Exist mmCfgCodecInfo ret_val=%d\n", ret_val);
	return ret_val;
}

//Ssu-Ying, support Multiple Profile
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgSetPortRange
 *
 *  DESCRIPTION    :  ICF_CFG_SET_PORT_RANGE_REQ
 *  This API is used by configuration module to configure media port ranges in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgSetPortRange(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_set_port_range_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t 	SipAccountId = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	int i=0;
MM_PRINTF("\n\tMM >Enter mmCfgSetPortRange \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_set_port_range_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;
	#if 1 /*Eason, per-line port range*/
	if(p_input_payload->SipAccountId >= MAX_SIP_LINE_COUNT){
		MM_PRINTF("\n\tMM> SipAccountId out of range %d\n", p_input_payload->SipAccountId);
		return MM_FAILURE;
	}
	else{
		#if 0 /*temporary set all line config to the same, wait for multi-profile implementation*/
		for(i=0;i<MAX_SIP_LINE_COUNT;i++){
			zyMemcpy(&p_mm_cfg_db->port_range[i], p_input_payload, sizeof(icf_cfg_set_port_range_req_st));
		}
		p_mm_cfg_db->port_range[i].line_id = i;
		#else		
		zyMemcpy(&p_mm_cfg_db->port_range[p_input_payload->SipAccountId], p_input_payload, sizeof(icf_cfg_set_port_range_req_st));
		#endif
		MM_PRINTF("MM > mmCfgSetPortRange\n");
		MM_PRINTF("SipAccountId %d\n", p_input_payload->SipAccountId);
		MM_PRINTF("srtp_enable %d\n", p_input_payload->srtp_enable);
		MM_PRINTF("srtp_encryption_key_size %d\n", p_input_payload->srtp_encryption_key_size);
#if KSOCKET_RTP_SUPPORT
		if(p_input_payload->srtp_enable){			
			mmKSocketEnable = 0;
			MM_PRINTF("srtp enabled, turn off RTP kernel socket mode\n");
		}
#endif
	}
	#else
	zyMemcpy(&p_mm_cfg_db->port_range, p_input_payload, sizeof(icf_cfg_set_port_range_req_st));
	#endif

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_SET_PORT_RANGE_RESP,
			app_id,
			call_id,
			SipAccountId,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgSetPortRange ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgRingerTones
 *
 *  DESCRIPTION    :  ICF_CFG_RINGER_TONES_REQ
 *  This API is used by configuration module to configure ringer tones in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgRingerTones(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_ringer_tones_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgRingerTones \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_ringer_tones_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;
	
	zyMemcpy(&p_mm_cfg_db->ringer_tones, p_input_payload, sizeof(icf_cfg_ringer_tones_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_RINGER_TONES_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgRingerTones ret_val=%d\n", ret_val);
	return ret_val;
}

//Ssu-Ying, support Multiple Profile
/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgSelfIp
 *
 *  DESCRIPTION    :  ICF_CFG_SET_SELF_IP_REQ
 *  This API is used by configuration module to configure self IP address in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgSelfIp(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_set_self_ip_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t 	SipAccountId = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgSelfIp \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_set_self_ip_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	#if 1 /*Eason, per-line port range*/
	if(p_input_payload->SipAccountId >= MAX_SIP_LINE_COUNT){
		MM_PRINTF("\n\tMM> SipAccountId out of range %d\n", p_input_payload->SipAccountId);
		return MM_FAILURE;
	}
	else{
		#if 0 /*temporary set all line config to the same, wait for multi-profile implementation*/
		for(i=0;i<MAX_SIP_LINE_COUNT;i++){
			zyMemcpy(&p_mm_cfg_db->port_range[i], p_input_payload, sizeof(icf_cfg_set_port_range_req_st));
		}
		p_mm_cfg_db->port_range[i].line_id = i;
		#else		
		zyMemcpy(&p_mm_cfg_db->self_ip[p_input_payload->SipAccountId], p_input_payload, sizeof(icf_cfg_set_self_ip_req_st));
		#endif
		MM_PRINTF("MM > mmCfgSelfIp\n");
		MM_PRINTF("SipAccountId %d\n", p_input_payload->SipAccountId);
	}
	#else
	zyMemcpy(&p_mm_cfg_db->self_ip, p_input_payload, sizeof(icf_cfg_set_self_ip_req_st));
	#endif	

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_SET_SELF_IP_RESP,
			app_id,
			call_id,
			p_input_payload->SipAccountId,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgSelfIp ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgMedia
 *
 *  DESCRIPTION    :  ICF_CFG_MEDIA_REQ
 *  This API is being enhanced to configure any media file with the Media Manager. 
 *  The application shall configure a media file with a unique ID within MM. 
 *  This ID will be applicable system wide and is not application specific. 
 *  The application can use this ID to play media at any time subsequently                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgMedia(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_media_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgMedia \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_media_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->media, p_input_payload, sizeof(icf_cfg_media_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgMedia ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgSetDigitTrans
 *
 *  DESCRIPTION    :  ICF_CFG_SET_DIGIT_TRANS_REQ
 *  This API is used by configuration module to configure inband transport method in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgSetDigitTrans(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_set_digit_trans_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgSetDigitTrans \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_set_digit_trans_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->digit_trans, p_input_payload, sizeof(icf_cfg_set_digit_trans_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgSetDigitTrans ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgJbmMgmt
 *
 *  DESCRIPTION    :  ICF_CFG_JBM_MGMT_REQ
 *  This API is used by configuration module to configure jitter management paramers 
 *  in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgJbmMgmt(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_jbm_mgmt_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgJbmMgmt \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_jbm_mgmt_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->jbm_mgmt, p_input_payload, sizeof(icf_cfg_jbm_mgmt_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgJbmMgmt ret_val=%d\n", ret_val);
	return ret_val;
}


/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgMaxEarpVol
 *
 *  DESCRIPTION    :  ICF_CFG_MAX_EARP_VOL_REQ
 *  This API is used by configuration module to configure maximum ringer volume parameters
 *  in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgMaxEarpVol(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_max_earp_vol_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgMaxEarpVol \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_max_earp_vol_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->max_earp_vol, p_input_payload, sizeof(icf_cfg_max_earp_vol_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgMaxEarpVol ret_val=%d\n", ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgMaxSpeakerVol
 *
 *  DESCRIPTION    :  ICF_CFG_MAX_SPEAKER_VOL_REQ
 *  This API is used by configuration module to configure maximum speaker volume parameters 
 *  in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgMaxSpeakerVol(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	icf_msg_resp_st                      *p_payload = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_max_speaker_vol_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgMaxSpeakerVol \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_max_speaker_vol_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->max_speaker_vol, p_input_payload, sizeof(icf_cfg_max_speaker_vol_req_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgMaxSpeakerVol ret_val=%d\n", ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgSetRemoteVideoDisplay
 *
 *  DESCRIPTION    :  ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ
 *  This API is used by application to configure remote video display size and coordinates 
 *  in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgSetRemoteVideoDisplay(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_set_remote_video_display_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgSetRemoteVideoDisplay \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_set_remote_video_display_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->remote_video_display, p_input_payload, sizeof(icf_cfg_set_remote_video_display_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgSetRemoteVideoDisplay ret_val=%d\n", ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgSetPreviewVideoDisplay
 *
 *  DESCRIPTION    :  ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ
 *  This API is used by configuration module to configure preview video display size and 
 *  coordinates in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgSetPreviewVideoDisplay(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_set_preview_video_display_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgSetRemoteVideoDisplay \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_set_preview_video_display_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	zyMemcpy(&p_mm_cfg_db->preview_video_display, p_input_payload, sizeof(icf_cfg_set_preview_video_display_st));

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgSetPreviewVideoDisplay ret_val=%d\n", ret_val);
	return ret_val;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : mmCfgMaxMicGain
 *
 *  DESCRIPTION    :  ICF_CFG_MAX_MIC_GAIN_REQ
 *  This API is used by configuration module to configure the maximum microphone gain level 
 *  in Media manager                 
 *
 *  RETURNS:       : Either MM_SUCCESS or MM_FAILURE
 *************************************************************************/
mm_return_t mmCfgMaxMicGain(
	icf_msg_st *p_msg,
	icf_error_t    *p_err)
{
	mmCfgDb_t *p_mm_cfg_db = MM_NULL;
	mm_return_t                            ret_val = MM_SUCCESS;
	icf_cfg_max_mic_gain_req_st	*p_input_payload = MM_NULL;
	icf_line_id_t line_id = 0;
	icf_call_id_t	call_id = 0;
	icf_app_id_t         app_id = ICF_MODULE_APP;	
	
MM_PRINTF("\n\tMM >Enter mmCfgMaxMicGain \n");	

	if(MM_NULL == p_msg )
	{
		return MM_FAILURE;
	}
	
	p_input_payload = (icf_cfg_max_mic_gain_req_st*)
							(MM_MSG_GET_PAYLOAD(p_msg));

	call_id = MM_MSG_GET_CALL_ID(p_msg);
	app_id = MM_MSG_GET_APP_ID(p_msg);

	p_mm_cfg_db = &gMmCfgDb_t;

	/* Set and check input parameters here */

	if(MM_FAILURE == 
		mmCoreFillMsgRespApi(
			ICF_CFG_MEDIA_RESP,
			app_id,
			call_id,
			line_id,
			p_err))
	{
		ret_val = MM_FAILURE;
	}

MM_PRINTF("\n\tMM >Exist mmCfgMaxMicGain ret_val=%d\n", ret_val);
	return ret_val;
}

