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

#include "mm_port_prototypes.h"
#include "mm_macros.h"
#include "icf_types.h"
#include "icf_api.h"
#include "mm_port_wrapper.h"
#include "mm_glb.h"
#include "mm_core_prototypes.h"
#include "mm_core_rtp.h"
#include "icf_clib.h"

#include "global.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#if 1 /*ZyXEL, rt_priority_Eason*/
#include <linux/sched.h>
#endif

/****************************************************************************/
/******************************************************************************
* This is the main starting function of Sample Program. 
******************************************************************************/
uint8 voiceDspProcessMessage(voiceMsg_t *p_msg);

int main(int argc, char *argv[])
{
	icf_boolean_t timeout_flag;
	icf_uint32_t usec = MM_SELECT_INFINITE;
	icf_return_t  ret_val;

	icf_uint8_t *p_buf = MM_NULL;
	mm_error_t ecode = 0;
	#if 1/*ZyXEL, rt_priority_Eason*/
	struct sched_param param = { .sched_priority = 99 };   
	ZyPrintf("%s, getpid = %d\n", __FUNCTION__, getpid());
	if(sched_setscheduler(getpid(), SCHED_RR, &param) != 0){
		ZyPrintf("%s, sched_setscheduler failed\n",__FUNCTION__);
	}
    #endif
   
    

	/*This Function initialises the socket level functionality
	*/	
	if (MM_SUCCESS != mmPortUdpInit())
	{
		MM_PRINTF("\n\tMM > mmPortUdpInit failed \n");
		return MM_FAILURE;
	}
	
	/*bos service init, (bosTask, bosEvent, bosMsg...)*/
	vendorOsInit();  // replace BRCM_SUPPORT bosInit();

	UITimer_Init();


	if (MM_SUCCESS != rtpInit())
	{
		MM_PRINTF("\n\tMM > rtpInit failed \n");
		return MM_FAILURE;
	}
	
	dspPoolInit();
	/*sndInit(); //used for gen howler tone*/
	ivrInit();
	
#if 0	/* Use TR014 replcase it */
	if (MM_SUCCESS != mm_cfg_init())
	{
		MM_PRINTF("\n\tMM > mm_cfg_init failed \n");
		return MM_FAILURE;
	}
#endif

	nice(-10); /*increase mm msg handler priority*/

	/* infinite select loop polls between keyboard input and messages from IPTK 
	*/
	for(;;)
	{

		ret_val = mmPortSelect(usec,&timeout_flag);

		if (MM_SUCCESS == ret_val)
		{

			if(FD_ISSET(gMmFd,&mmFdset))
			{			
				if (MM_SUCCESS != mmPortUdpRecv(&p_buf))
				{
					return MM_FAILURE;
				}

				if(MM_SUCCESS == mmProcessMessage((icf_msg_st *)p_buf)){
					icf_free_decoded_msg((icf_msg_st *)p_buf,&ecode);
				}
				else{
					voiceDspProcessMessage((voiceMsg_t *)p_buf);
					voiceMsgFree((voiceMsg_t *)p_buf);
					
				}

				
			}
		}
	}

	return MM_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION NAME : mmProcessMessage
 *
 * DESCRIPTION   : This function recieves and process the messages recieved
 *				   from IPTK 
 *
 * RETURNS       : mm_return_t
 *
 ******************************************************************************/

mm_return_t mmProcessMessage(icf_msg_st *p_msg)
{
	icf_api_id_t	api_id;
    	mm_error_t ecode = 0;
    	mm_return_t ret_val = MM_SUCCESS;
	icf_module_id_t source_id;

	if(p_msg == MM_NULL) {
		return(MM_FAILURE);
	}

	source_id = MM_MSG_GET_SRC(p_msg);
	api_id = MM_MSG_GET_API(p_msg);
	
	MM_PRINTF("\n######################################################\n");
	MM_PRINTF("IPTK ->  MM : API id %d", api_id);
	MM_PRINTF("\n######################################################\n");

	switch(api_id)
	{
		/* Call Control APIs */
		case ICF_GET_CODEC_LIST_REQ:
		case ICF_CODEC_COMMIT_REQ:
		case ICF_REL_RES_REQ:
            mmCoreRmMsgHandler(p_msg,&ecode);
			break;
		case ICF_OPEN_MEDIA_CHANNEL_REQ:
		case ICF_CREATE_MEDIA_SESSION_REQ:
		case ICF_MODIFY_MEDIA_SESSION_REQ:
		case ICF_DELETE_MEDIA_SESSION_REQ:
		case ICF_CLOSE_MEDIA_CHANNEL_REQ:
		case ICF_MERGE_MEDIA_SESSION_REQ:
		case ICF_MEDIA_CAPABILITIES_REQ:
			mmCoreRtpMsgHandler(p_msg,&ecode);
			break;
		case ICF_SEND_UDP_DATA_REQ:
		case ICF_RECV_UDP_DATA_IND:
			break;
		case ICF_MM_UNEXPECTED_MSG_IND:
		mmCoreUnexpectedMsg(p_msg, &ecode);
			break;
		/* Common Application APIs */
		case ICF_START_RINGING_REQ:
		case ICF_VIBRATION_REQ:
		case ICF_CONNECT_MEDIA_DEVICE_REQ:
		case ICF_ECHO_CANC_REQ:
		case ICF_VAD_SS_CNG_REQ:
		case ICF_GET_STATS_REQ:
		case ICF_PLAY_MEDIA_REQ:
		case ICF_STOP_MEDIA_PLAY_REQ:
		case ICF_STOP_MEDIA_PLAY_IND:
		case ICF_STOP_RINGING_REQ:
		case ICF_STOP_VIBRATION_REQ:
		case ICF_STOP_RINGING_IND:		
		/* Video/Voice Application */
		case ICF_DGTS_IND:
		case ICF_MUTE_MEDIA_REQ:
		case ICF_SET_MEDIA_PLAYBACK_DEVICE_REQ:
		case ICF_MEDIA_RECORD_REQ:
		case ICF_SEND_DGTS_REQ:
		case ICF_SET_EARP_VOLUME_REQ:
		case ICF_SET_SPEAKER_VOLUME_REQ:
		case ICF_STOP_MEDIA_RECORD_REQ:
		case ICF_SET_MIC_GAIN_REQ:
		case ICF_PAUSE_MEDIA_REQ:
		/* Push To Talk */	
		/*case ICF_RCV_DATA_IND:*/
		case ICF_SEND_APP_DATA_REQ:
		case ICF_SEND_STATS_REQ:
		case ICF_STOP_STATS_REQ:
			break;	
		/* Configuration */		
		case ICF_CFG_CODEC_INFO_REQ:
			mmCfgCodecInfo(p_msg, &ecode);
			break;
		case ICF_CFG_RINGER_TONES_REQ:
			mmCfgRingerTones(p_msg, &ecode);
			break;
		case ICF_CFG_MEDIA_REQ:
			mmCfgMedia(p_msg, &ecode);
			break;
		case ICF_CFG_JBM_MGMT_REQ:	
			mmCfgJbmMgmt(p_msg, &ecode);
			break;
		case ICF_CFG_MAX_EARP_VOL_REQ:
			mmCfgMaxEarpVol(p_msg, &ecode);
			break;
		case ICF_CFG_MAX_SPEAKER_VOL_REQ:
			mmCfgMaxSpeakerVol(p_msg, &ecode);
			break;
		case ICF_CFG_SET_PORT_RANGE_REQ:
			mmCfgSetPortRange(p_msg, &ecode);
			break;
		case ICF_CFG_SET_SELF_IP_REQ:
			mmCfgSelfIp(p_msg, &ecode);
			break;
		case ICF_CFG_SET_DIGIT_TRANS_REQ:
			mmCfgSetDigitTrans(p_msg, &ecode);
			break;
		case ICF_CFG_SET_REMOTE_VIDEO_DISPLAY_REQ:
			mmCfgSetRemoteVideoDisplay(p_msg, &ecode);
			break;
		case ICF_CFG_SET_PREVIEW_VIDEO_DISPLAY_REQ:
			mmCfgSetPreviewVideoDisplay(p_msg, &ecode);
			break;
		case ICF_CFG_MAX_MIC_GAIN_REQ:
			mmCfgMaxMicGain(p_msg, &ecode);
			break;
		case ICF_CFG_SET_VIDEO_ATTRIB_REQ:
			/* not implement yet */
			break;
		default:
			ret_val = MM_FAILURE;
			break;
	}

	return ret_val;
}

uint8 voiceDspProcessMessage(voiceMsg_t *p_msg)
{
	mm_error_t ecode;
	uint16 api_id = 0;

	if(p_msg == MM_NULL) {
		return(MM_FAILURE);
	}
	api_id = p_msg->hdr.api_id;
	MM_PRINTF("\n######################################################\n");
	MM_PRINTF("Phfsm -> MM : API id %d", api_id);
	MM_PRINTF("\n######################################################\n");

	if(VOICE_MM_SET_DEBUG_REQ == api_id)
	{
		mmCfgDebug(p_msg,&ecode);
	}
	else if (VOICE_MM_DEBUG_STATUS_REQ == api_id){
		mmCfgDebugStatus(p_msg,&ecode);
	}
	#if KSOCKET_RTP_SUPPORT
	else if(VOICE_MM_KSOCKET_SET_REQ == api_id){
		mmKSocketSet(p_msg, &ecode);
	}
	#endif
	else
	{
		voiceDspCoreMsgHandler(p_msg,&ecode);
	}

	return MM_SUCCESS;
}

