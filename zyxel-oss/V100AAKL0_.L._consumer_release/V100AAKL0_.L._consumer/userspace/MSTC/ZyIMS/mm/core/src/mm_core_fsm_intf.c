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

#include "icf_api.h"
#include "mm_core_fsm_types.h"
#include "mm_macros.h"
#include "mm_core_prototypes.h"
#include "mm_glb.h"
#include "voice_dsp_core_fsm_types.h"
#include "global.h"
#include "switch.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"

mmCoreCallCtxList_t *p_call_context_list = MM_NULL;
/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRmMsgHandler
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
mmCoreRmMsgHandler(icf_msg_st *p_msg,icf_error_t *p_err)
{
	icf_api_id_t					    	api_id;
	icf_call_id_t						    call_id = 0;
	icf_uint8_t						        event_from_icf;
	mmCoreCallCtxList_t 	*p_call_context_node = MM_NULL;
	
	if(ICF_NULL == p_msg)
	{
		return ICF_FAILURE;
	}
	api_id  = MM_MSG_GET_API(p_msg);
	call_id = MM_MSG_GET_CALL_ID(p_msg);

	if(api_id == ICF_GET_CODEC_LIST_REQ)
	{
		if(mmRmCoreStateEvent[RM_CORE_GET_GCL_MMI](p_msg,ICF_NULL,p_err)==ICF_FAILURE)
		{
			/* raise the alarm */
			return ICF_FAILURE;
		}
		return ICF_SUCCESS;
	}

	/* get the event to be send to the FSM */
	if(mmConvertApiToMmEvent(api_id,&event_from_icf) == ICF_FAILURE)
	{
		/* some undefined event is received from the ICF */
		/* raise the alarm */
		return ICF_SUCCESS;
	}

	p_call_context_node = mmCoreGetCallContext(p_call_context_list, call_id);
	if(ICF_NULL == p_call_context_node)
	{
		mmCoreCallIdErrorMmi(call_id, api_id, p_err);
		return ICF_FAILURE;
	}


	/* call the function of va call state event table */
	if(ICF_FAILURE == 
		mmRmCoreStateEvent[event_from_icf](p_msg,p_call_context_node, p_err))
	{
		return ICF_FAILURE;
	}
	return ICF_SUCCESS;

}



/*************************************************************************
*
*  FUNCTION NAME  : mmCoreRtpMsgHandler
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
mmCoreRtpMsgHandler(icf_msg_st *p_msg,icf_error_t *p_err)
{
	icf_api_id_t					    	api_id;
	icf_call_id_t						    call_id = 0;
	icf_uint8_t						        event_from_icf;
	mmCoreCallCtxList_t 	*p_call_context_node = MM_NULL;
	
	if(ICF_NULL == p_msg)
	{
		return ICF_FAILURE;
	}
	api_id  = MM_MSG_GET_API(p_msg);
	call_id = MM_MSG_GET_CALL_ID(p_msg);

	/* get the event to be send to the FSM */
	if(mmConvertApiToRtpEvent(api_id,&event_from_icf) == ICF_FAILURE)
	{
		/* some undefined event is received from the ICF */
		/* raise the alarm */
		return ICF_SUCCESS;
	}

	p_call_context_node = mmCoreGetCallContext(p_call_context_list, call_id);
	if(ICF_NULL == p_call_context_node)
	{
		mmCoreCallIdErrorMmi(call_id, api_id, p_err);
		return ICF_FAILURE;
	}

	/* call the function of va call state event table */
	if(ICF_FAILURE == 
		mmRtpCoreStateEvent[event_from_icf](p_msg,p_call_context_node, p_err))
	{

		return ICF_FAILURE;
	}

	return ICF_SUCCESS;
}



/*************************************************************************
*
*  FUNCTION NAME  : voiceDspCoreMsgHandler
*
*  DESCRIPTION    : This function is called by the CORE main and, it receives
*					the message from the CC and will call the CALL FSM 
*					function of the CORE
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
icf_return_t
voiceDspCoreMsgHandler(voiceMsg_t *p_msg,icf_error_t *p_err)
{
	icf_api_id_t					    	api_id;
	icf_uint8_t						        event_from_icf;

	if(ICF_NULL == p_msg)
	{
		return ICF_FAILURE;
	}
	api_id  = p_msg->hdr.api_id;
	
	/* get the event to be send to the FSM */
	if(voiceDspConvertApiToEvent(api_id,&event_from_icf) == ICF_FAILURE)
	{
		/* some undefined event is received from the ICF */
		/* raise the alarm */
		return ICF_FAILURE;
	}

	/* call the function of va call state event table */
	if(voiceDspCoreStateEvent[0][event_from_icf](p_msg, &p_err) == MM_FAILURE)
	{
		return ICF_FAILURE;
	}
	
	return ICF_SUCCESS;

}


/*************************************************************************
*
*  FUNCTION NAME  : mmConvertApiToMmEvent
*
*  DESCRIPTION    : 
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/


icf_return_t
mmConvertApiToMmEvent(
		icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf)
{	
	switch(api_id)
	{
		case ICF_GET_CODEC_LIST_REQ:
			
			*p_event_from_icf = RM_CORE_GET_GCL_MMI;
			break;
			
		case ICF_CODEC_COMMIT_REQ:
			
			*p_event_from_icf = RM_CORE_CODEC_COMMIT_MMI;
			break;
			
		case ICF_REL_RES_REQ:
			
			*p_event_from_icf = RM_CORE_REL_RES_MMI;
			break;
				
		default :
			MM_PRINTF("MM: %s : mm_event not found\n",__FUNCTION__);
			return ICF_FAILURE;
			/* error condition raise the error*/
			
	}/* end of switch case statement */

	return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : mmConvertApiToRtpEvent
*
*  DESCRIPTION    : 
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/


icf_return_t
mmConvertApiToRtpEvent(
		icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf)
{	
	switch(api_id)
	{
		case ICF_OPEN_MEDIA_CHANNEL_REQ:
			
			*p_event_from_icf = RTP_CORE_OPEN_CHANNEL_MMI;
			break;

		case ICF_CREATE_MEDIA_SESSION_REQ :

			*p_event_from_icf = RTP_CORE_CREATE_SESSION_MMI;
			 break;


		case ICF_MODIFY_MEDIA_SESSION_REQ :
			
			*p_event_from_icf = RTP_CORE_MODIFY_SESSION_MMI;
	    	 	 break;

		case ICF_DELETE_MEDIA_SESSION_REQ :
			
			*p_event_from_icf = RTP_CORE_DELETE_SESSION_MMI;
			break;

		case ICF_CLOSE_MEDIA_CHANNEL_REQ :
		
			*p_event_from_icf = RTP_CORE_CLOSE_SESSION_MMI;
			break;
			
		case ICF_MERGE_MEDIA_SESSION_REQ :
		
			*p_event_from_icf = RTP_CORE_MERGE_SESSION_MMI;
			break;
			
		case ICF_MEDIA_CAPABILITIES_REQ:
			
			*p_event_from_icf = RTP_CORE_CAPABILITIES_MMI;
			break;
		
		default :
			MM_PRINTF("MM: %s : rtp_event not found\n",__FUNCTION__);
			return ICF_FAILURE;
			/* error condition raise the error*/
			
		}/* end of switch case statement */
		return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  : voiceDspConvertApiToEvent
*
*  DESCRIPTION    : 
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/


icf_return_t
voiceDspConvertApiToEvent(
		icf_api_id_t		api_id, 
		icf_uint8_t	*p_event_from_icf)
{	

	switch(api_id)
	{
		case VOICE_DSP_INIT_REQ:
			*p_event_from_icf = DSP_CORE_INIT;
			break;
		case VOICE_DSP_CHAN_OPEN_REQ: 			
			*p_event_from_icf = DSP_CORE_CHAN_OPEN;
			break;
		case VOICE_DSP_CHAN_CLOSE_REQ: 			
			*p_event_from_icf = DSP_CORE_CHAN_CLOSE;
			break;
		case VOICE_DSP_CREATE_MEETING_ROOM_REQ: 			
			*p_event_from_icf = DSP_CORE_CREATE_MEETING_ROOM;
			break;
		case VOICE_DSP_DESTROY_MEETING_ROOM_REQ: 			
			*p_event_from_icf = DSP_CORE_DESTROY_MEETING_ROOM;
			break;		
		case VOICE_DSP_CREATE_INTERNAL_CALL_REQ: 			
			*p_event_from_icf = DSP_CORE_CREATE_INTERNAL_CALL;
			break;
		case VOICE_DSP_SET_FAX_PARAMETER_REQ: 			
			*p_event_from_icf = DSP_CORE_SET_FAX_PARAMETER;
			break;
		#ifdef HOWLER_TONE
		case VOICE_DSP_START_HOWLER_TONE_REQ:
			*p_event_from_icf = DSP_CORE_START_HOWLER_TONE;
			break;
		case VOICE_DSP_STOP_HOWLER_TONE_REQ:
			*p_event_from_icf = DSP_CORE_STOP_HOWLER_TONE;
			break;
		#endif
		case VOICE_DSP_IVR_FUNC_REQ:
			*p_event_from_icf = DSP_CORE_IVR_FUNC;
			break;
		case VOICE_DSP_START_EARLY_MEDIA_REQ:
			*p_event_from_icf = DSP_CORE_START_EARLY_MEDIA;
			break;
		case VOICE_DSP_STOP_EARLY_MEDIA_REQ:
			*p_event_from_icf = DSP_CORE_STOP_EARLY_MEDIA;
			break;
#if 1	/* aircheng add for MOH */
		case VOICE_DSP_START_MUSIC_ONHOLD_REQ:
			*p_event_from_icf = DSP_CORE_START_MUSIC_ONHOLD;
			break;
		case VOICE_DSP_STOP_MUSIC_ONHOLD_REQ:
			*p_event_from_icf = DSP_CORE_STOP_MUSIC_ONHOLD;
			break;
#endif
#ifdef VOIP_SYSLOG /*Jason , syslog*/ 
		case VOICE_MM_GET_STREAM_LOG_REQ:
			*p_event_from_icf = DSP_MM_STREAMLOG;	/*get stream log */
			break;			
#endif			
		default :
			MM_PRINTF("MM: %s : event not found\n",__FUNCTION__);
			return ICF_FAILURE;
			/* error condition raise the error*/
			
	}/* end of switch case statement */
	
	return ICF_SUCCESS;
}

