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
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>

#include "icf_types.h"
#include "icf_clib.h"
#include "global.h"
#include "mm_macros.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#include "switch.h"

uint32
voiceMsgUnpackU8
(
    icf_uint8_t    *p_dest,
    icf_uint8_t    *p_src,
    icf_uint32_t *p_length
);

uint32
voiceMsgUnpackU16
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32 *p_length
);


uint32
voiceMsgUnpackU32
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32 *p_length
);


uint32
voiceMsgPackU8
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgPackU16
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgPackU32
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


#define VOICE_IPC_PRINTF
uint8 voiceEventIpcSend( int sock_fd,
								const struct sockaddr *p_addr, 
								int sockaddr_len,
								voiceMsg_t *p_msg)
{
	uint32		payload_length = 0,packet_length,buf_len = 0;
	uint32		bytes_written = 0;
	uint8 		*p_tmp_buf = NULL, *p_buf = NULL, p_err = 0;
	uint8		ret_val = 0;
	int sendSock = 0;

	ret_val = voiceMsgEncode((voiceMsg_t*)p_msg, &p_buf, &buf_len, &p_err);
	if(NULL == p_msg)
	{
		if(NULL != p_buf){
			zyFree(p_buf);
		}
		return MM_FAILURE;
	}

	sendSock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sendSock == -1){
		if(NULL != p_buf){
			zyFree(p_buf);
		}
		return MM_FAILURE;
	}

	p_tmp_buf=p_buf;

	packet_length = payload_length = buf_len;
	VOICE_IPC_PRINTF("packet_length %d bytes\n",packet_length);		
	VOICE_IPC_PRINTF("socklen %d\n",sockaddr_len);
	while( packet_length ){
		if( packet_length > MAX_SEND_SIZE ) {
			packet_length = MAX_SEND_SIZE;
		}

		bytes_written = sendto(sendSock, (void *)p_tmp_buf,packet_length,0 ,
									(struct sockaddr*)p_addr,
									sockaddr_len);

		VOICE_IPC_PRINTF("IPC sendto %d bytes\n",bytes_written);
		if ( packet_length != bytes_written) {			
			zyFree(p_buf);
			return (MM_FAILURE);
		}
		
		p_tmp_buf +=packet_length;

		payload_length -= packet_length;

		packet_length = payload_length;	

	}

	close(sendSock);	
	zyFree(p_buf);
	return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgEncode
 *
 * Description: 
 *	Utility function, encode the voice message to one buffer
 * Parameters:
 * 	Input
 *    uint8*  p_encoded_buf - encoded buffer, raw data
 *	uint32  encoded_buf_length - encoded buffer length
 *	Output
 *	voiceMsg_t**  p_p_msg - the voice message decoded from the buffer
 *	uint8*  p_ecode - error code
 * Return Value: uint8
 *    MM_SUCCESS
 *****************************************************************************/
uint8 voiceMsgEncode(voiceMsg_t*   p_msg, /* Input*/
							uint8** p_p_encoded_buf, /* Output*/
							uint32* p_encoded_buf_length,
							uint8*  p_ecode)
{

	uint32  length    = 0;
	uint8*  p_src     = NULL;
	uint8*  p_dest    = NULL;
	uint8*  p_buff    = NULL;
	uint32 api_length = 0;
    uint8 ret_val    = MM_SUCCESS;
	uint32 hdr_length = 0;
	uint32 api_length_index = 0;
	uint16 api_length_value = 0;


	if (!p_msg) {
        VOICE_IPC_PRINTF("\n[voiceMsgEncode]  - Argument #1 p_msg is NULL\n");
		*p_ecode = VOICE_ERR_NULL_POINTER;
		return MM_FAILURE;
	}
    VOICE_IPC_PRINTF("\n<======================== ENCODE START ======================>\n");

	api_length = p_msg->hdr.api_length;
	VOICE_IPC_PRINTF("[voiceMsgEncode] msg->hdr.api_length:%d\n",api_length);

	/*1. check api_length*/
	VOICE_IPC_PRINTF("\n[voiceMsgEncode]1. check api_length\n");
    if (api_length <= 0 || api_length >= VOICE_DSP_MAX_API_LENGTH) {
        VOICE_IPC_PRINTF("\n[voiceMsgEncode] Invalid API Length (%d)\n", api_length);
        *p_ecode = VOICE_ERR_INVALID_LENGTH;
        return MM_FAILURE;
    }

	/* Allocate a temporary buffer with the API length of the */
	/* Input Message */
	/*2. alloc a temp buffer for encode*/
    VOICE_IPC_PRINTF("\n[voiceMsgEncode] 2. Alloc a temp buffer for encode\n");	
	p_buff   = (uint8 *)zyMalloc(api_length);
	if (!p_buff) {
        VOICE_IPC_PRINTF("\n[voiceMsgEncode]: ICF_CLIB_MEMGET Failure!! (p_buff)\n");
		*p_ecode = VOICE_ERR_MEMGET_FAILED;
		return MM_FAILURE;
	}

	p_src    = (uint8*)p_msg;

	/*3. pack voice msg header*/
	VOICE_IPC_PRINTF("\n[voiceMsgEncode] 3. pack voice msg header\n");
	ret_val = (icf_uint8_t)voiceMsgPackVoiceApiHeader(p_buff, p_src, &length, &api_length_index);

	hdr_length = length;
		
    if (ret_val == MM_FAILURE) {
        VOICE_IPC_PRINTF("\n[voiceMsgEncode]: icf_encode_msg - ENCODING Failed While Packing API Header!!!\n");
	    zyFree(p_buff);
		*p_ecode = VOICE_ERR_ENCODE_FAILED;
        return ret_val;
    }

	/*4. pack voice msg payload*/
	VOICE_IPC_PRINTF("\n[voiceMsgEncode] 4. Pack voice msg payload\n");
	/* Pack the rest of the payload*/
	p_src = (icf_uint8_t*)p_msg->payload;

	/* Make sure the destination points beyond the API header*/
	p_dest = p_buff + sizeof(voiceApiHeader_t);
	ret_val = voiceMsgPackApi(p_dest,p_src,&length,p_msg->hdr.api_id);

    /*5. pack api_length( = api header + api payload length) to header */
	VOICE_IPC_PRINTF("\n[voiceMsgEncode] 5. pack api_length( = api header + api payload length) to header \n");
	/* The length of the encoded buffer includes
	   the length of API header */
	api_length_value = (uint16)(length + hdr_length);

	voiceMsgPackU16(p_buff + api_length_index, (icf_uint8_t*)&api_length_value, &hdr_length);
	VOICE_IPC_PRINTF("\n[voiceMsgEncode] pack api_length %d bytes\n",api_length_value);

	/*6. memcpy the temp buffer to output encoded msg*/
	*p_encoded_buf_length = api_length_value;
    VOICE_IPC_PRINTF("\n[voiceMsgEncode] 6. memcpy the temp buffer to output encoded msg, Alloc encoded msg buffer %d bytes\n",*p_encoded_buf_length);
	*p_p_encoded_buf = (icf_uint8_t*)zyMalloc(*p_encoded_buf_length);

	if (!*p_p_encoded_buf) {
        VOICE_IPC_PRINTF("\n[voiceMsgEncode]: Allocate output encoded mem failed\n");
	    zyFree(p_buff);
		*p_ecode = VOICE_ERR_MEMGET_FAILED;
		return MM_FAILURE;
	}
    VOICE_IPC_PRINTF("\n[voiceMsgEncode] Copy temp buffer to encoded msg\n");
	zyMemcpy(*p_p_encoded_buf, p_buff, api_length_value);	

	/*7. free temp buffer*/
    VOICE_IPC_PRINTF("\n[voiceMsgEncode]7. Free temp buffer\n");
	zyFree(p_buff);

    VOICE_IPC_PRINTF("\n[voiceMsgEncode] Encoded Buffer Length (%d) Bytes\n", *p_encoded_buf_length);
    VOICE_IPC_PRINTF("\n<========================= ENCODE END =======================>\n");
	
	return ret_val;
}


/******************************************************************************
 * voiceMsgDecode
 *
 * Description: 
 *	Utility function, decode the encoded buffer to voice message
 * Parameters:
 * 	Input
 *    uint8*  p_encoded_buf - encoded buffer, raw data
 *	uint32  encoded_buf_length - encoded buffer length
 *	Output
 *	voiceMsg_t**  p_p_msg - the voice message decoded from the buffer
 *	uint8*  p_ecode - error code
 * Return Value: uint8
 *    MM_SUCCESS
 *****************************************************************************/
uint8 voiceMsgDecode(voiceMsg_t**  p_p_msg, /* Output*/
							uint8*  p_encoded_buf,
							uint32  encoded_buf_length,
							uint8*  p_ecode)
{

	uint32 length      = 0;
	uint8* p_src       = NULL;
	uint8* p_dest      = NULL;
	uint32 api_length  = NULL;
	uint16 api_id      = NULL;
	voiceApiHeader_t* p_hdr = NULL;
	uint8 ret_val     = MM_SUCCESS;


	if (!p_encoded_buf) {
        	VOICE_IPC_PRINTF("\n[voiceMsgDecode] - Argument #2 p_encoded_buf is NULL\n");
		*p_ecode = VOICE_ERR_NULL_POINTER;
		return MM_FAILURE;
	}
	VOICE_IPC_PRINTF("\n<======================== DECODE START ======================>\n");

	/*1. alloc voice msg header*/
	VOICE_IPC_PRINTF("\n[voiceMsgDecode] 1. Alloc voice msg header\n");
	p_hdr = (voiceApiHeader_t*)zyMalloc(sizeof(voiceApiHeader_t));    
	if (!p_hdr) {
        	VOICE_IPC_PRINTF("[voiceMsgDecode] Alloc voice msg header failed!\n");
		*p_ecode = VOICE_ERR_MEMGET_FAILED;
		return MM_FAILURE;
	}

	/*2. unpack voice msg header*/
	/* Unpack the header first*/
	VOICE_IPC_PRINTF("[voiceMsgDecode] 2. Unpack voice msg header\n");
	ret_val = (icf_uint8_t)voiceMsgUnpackVoiceApiHeader((uint8*)p_hdr, p_encoded_buf, &length);
	VOICE_IPC_PRINTF("hdr.api_id %d\n",p_hdr->api_id);
	VOICE_IPC_PRINTF("hdr.api_length %d\n",p_hdr->api_length);
	if (ret_val == MM_FAILURE) {
		VOICE_IPC_PRINTF("\n[voiceMsgDecode] Unpack voice msg header failed\n");
		zyFree(p_hdr);
		*p_ecode = VOICE_ERR_DECODE_FAILED;
		return ret_val;
	}

	/*3. check api_id*/
	api_id     = p_hdr->api_id;
	VOICE_IPC_PRINTF("[voiceMsgDecode] 3. check api_id : %d\n",api_id);
		
	if ((api_id < VOICE_DSP_API_START) || (api_id >VOICE_DSP_API_END)) {
		VOICE_IPC_PRINTF("\n[voiceMsgDecode] - wrong API id (%d) \n", api_id);
		zyFree(p_hdr);
		*p_ecode = VOICE_ERR_DECODE_FAILED;
		return MM_FAILURE;
	}

	/*4. alloc output buffer : api_length*/
	api_length = voiceMsgGetApiLength(api_id);
	VOICE_IPC_PRINTF("[voiceMsgDecode] 4. Alloc output buffer : api_length %d\n",api_length);
	if(0 == api_length) {
		VOICE_IPC_PRINTF("\n[voiceMsgDecode] - API id (%d) & API Length Mismatch\n", api_id);
		zyFree(p_hdr);
		*p_ecode = VOICE_ERR_INVALID_LENGTH;
		return MM_FAILURE;
	}	
	*p_p_msg = (voiceMsg_t*)zyMalloc(api_length);

	if (!*p_p_msg) {
		VOICE_IPC_PRINTF("\n[voiceMsgDecode]: Alloc output buffer failed!\n");
		zyFree(p_hdr);
		*p_ecode = VOICE_ERR_MEMGET_FAILED;
		return MM_FAILURE;
	}


	/*5. unpack voice msg payload*/
	p_dest = (*p_p_msg)->payload;

	/* Move past the API header into the buffer*/
	p_src  = p_encoded_buf + sizeof(voiceApiHeader_t);

	VOICE_IPC_PRINTF("\n[voiceMsgDecode] 5. Unpack voice msg payload \n");
	ret_val = voiceMsgUnpackApi(p_dest, p_src, &length, api_id);

	if (ret_val == MM_FAILURE) {
		VOICE_IPC_PRINTF("\n[voiceMsgDecode] -Unpack voice msg payload failed!\n");
		zyFree(*p_p_msg);
		zyFree(p_hdr);
		*p_ecode = VOICE_ERR_DECODE_FAILED;
		return ret_val;
	}

	/*6. Memcpy temp voice msg header to output buffer*/
	VOICE_IPC_PRINTF("\n[voiceMsgDecode] 6.Memcpy temp header to output buffer\n");
	zyMemcpy(&((*p_p_msg)->hdr), p_hdr, sizeof(voiceApiHeader_t));

	/*7. Free temp voice msg header*/
	VOICE_IPC_PRINTF(("\n[voiceMsgDecode] 7. Free temp voice msg header\n"));
	zyFree(p_hdr);

	VOICE_IPC_PRINTF("\n[voiceMsgDecode] - Decoded Buffer Length (%d) Bytes\n", length);
	VOICE_IPC_PRINTF("\n<========================= DECODE END =======================>\n");
	return ret_val;
}


/******************************************************************************
 * voiceMsgPackApi
 *
 * Description: 
 *	Utility function, used to pack data from voice message depending on api id
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	uint16    api_id - voice message api id
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint8
 *    MM_SUCCESS
 *****************************************************************************/
uint8 voiceMsgPackApi(uint8 *p_dest, uint8 *p_src, uint32 *p_length, uint16 api_id)
{
    uint8    ret_val = MM_SUCCESS;
    uint32    length = 0;

    switch (api_id) 
    {    				
		case VOICE_DSP_INIT_RSP:			
			ret_val = (uint8)voiceMsgPackDspInitRsp(p_dest, p_src, &length);
			break;			
	    case VOICE_DSP_CHAN_OPEN_REQ:			
			ret_val = (uint8)voiceMsgPackDspChanOpenReq(p_dest, p_src, &length);
	        break;			
		case VOICE_DSP_IVR_FUNC_REQ:			
			ret_val = (uint8)voiceMsgPackDspIvrFuncReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CHAN_CLOSE_REQ:
		case VOICE_DSP_DESTROY_MEETING_ROOM_REQ:		
		case VOICE_DSP_CREATE_INTERNAL_CALL_REQ:
		case VOICE_DSP_SET_FAX_PARAMETER_REQ:
		case VOICE_DSP_START_HOWLER_TONE_REQ:	
		case VOICE_DSP_STOP_HOWLER_TONE_REQ:
			ret_val = (uint8)voiceMsgPackDspCommonReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_START_EARLY_MEDIA_REQ:			
		case VOICE_DSP_STOP_EARLY_MEDIA_REQ:			
			ret_val = (uint8)voiceMsgPackDspEarlyMediaReq(p_dest, p_src, &length);
			break;			
#if 1	/* aircheng add for MOH */
		case VOICE_DSP_START_MUSIC_ONHOLD_REQ:
		case VOICE_DSP_STOP_MUSIC_ONHOLD_REQ:
			ret_val = (uint8)voiceMsgPackDspMusicOnHoldReq(p_dest, p_src, &length);
			break;			
#endif
		case VOICE_DSP_CHAN_OPEN_RSP:
		case VOICE_DSP_CHAN_CLOSE_RSP:
		case VOICE_DSP_DESTROY_MEETING_ROOM_RSP:			
		case VOICE_DSP_CREATE_INTERNAL_CALL_RSP:
		case VOICE_DSP_SET_FAX_PARAMETER_RSP:
		case VOICE_DSP_START_HOWLER_TONE_RSP:
		case VOICE_DSP_STOP_HOWLER_TONE_RSP:
		case VOICE_DSP_IVR_FUNC_RSP:			
		case VOICE_DSP_START_EARLY_MEDIA_RSP:
		case VOICE_DSP_STOP_EARLY_MEDIA_RSP:
		case VOICE_DSP_START_MUSIC_ONHOLD_RSP:	/* aircheng add for MOH*/
		case VOICE_DSP_STOP_MUSIC_ONHOLD_RSP:		/* aircheng add for MOH*/
			ret_val = (uint8)voiceMsgPackDspCommonRsp(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CREATE_MEETING_ROOM_REQ:			
			ret_val = (uint8)voiceMsgPackDspCreateMeetingRoomReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CREATE_MEETING_ROOM_RSP:			
			ret_val = (uint8)voiceMsgPackDspCreateMeetingRoomRsp(p_dest, p_src, &length);
			break;		
#ifdef VOIP_SYSLOG /*Jason , syslog*/
		case VOICE_MM_GET_STREAM_LOG_REQ:
			ret_val = (uint8)voiceMsgPackDspMmStatsReq(p_dest, p_src, &length);
		    	break;		    		
		case VOICE_MM_GET_STREAM_LOG_RSP:
			ret_val = (uint8)voiceMsgPackDspMmStatsRsp(p_dest, p_src, &length);
			break;	
#endif			
		case VOICE_MM_SET_DEBUG_REQ:
		case VOICE_MM_SET_DEBUG_RSP:
		case VOICE_MM_DEBUG_STATUS_REQ:
		case VOICE_MM_DEBUG_STATUS_RSP:
		case VOICE_DSP_INIT_REQ:
		default:
			length = voiceMsgGetApiLength(api_id) - sizeof(voiceApiHeader_t);
			zyMemcpy(p_dest, p_src, length);
			ret_val = MM_SUCCESS;
	       	break;
	}

    *p_length=length;
    return ret_val;
}


/******************************************************************************
 * voiceMsgUnpackApi
 *
 * Description: 
 *	Utility function, used to unpack data from voice message depending on api id
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	uint16    api_id - voice message api id
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint8
 *    MM_SUCCESS if success
 *	MM_FAILURE if failure
 *****************************************************************************/
uint8 voiceMsgUnpackApi(uint8 *p_dest, uint8 *p_src, uint32 *p_length,uint16 api_id)
{
    uint8    ret_val = MM_SUCCESS;
    uint32    length = 0;

    switch (api_id) 
    {    				
		case VOICE_DSP_INIT_RSP:			
			ret_val = (uint8)voiceMsgUnpackDspInitRsp(p_dest, p_src, &length);			
			break;			
		case VOICE_DSP_CHAN_OPEN_REQ:			
			ret_val = (uint8)voiceMsgUnpackDspChanOpenReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_IVR_FUNC_REQ:			
			ret_val = (uint8)voiceMsgUnpackDspIvrFuncReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CHAN_CLOSE_REQ:
		case VOICE_DSP_DESTROY_MEETING_ROOM_REQ:				
		case VOICE_DSP_CREATE_INTERNAL_CALL_REQ:
		case VOICE_DSP_SET_FAX_PARAMETER_REQ:
		case VOICE_DSP_START_HOWLER_TONE_REQ:			
		case VOICE_DSP_STOP_HOWLER_TONE_REQ:
			ret_val = (uint8)voiceMsgUnpackDspCommonReq(p_dest, p_src, &length);
			break;				
		case VOICE_DSP_START_EARLY_MEDIA_REQ:			
		case VOICE_DSP_STOP_EARLY_MEDIA_REQ:			
			ret_val = (uint8)voiceMsgUnpackDspEarlyMediaReq(p_dest, p_src, &length);
			break;			
#if 1	/* aircheng add for MOH */
		case VOICE_DSP_START_MUSIC_ONHOLD_REQ:
		case VOICE_DSP_STOP_MUSIC_ONHOLD_REQ:
			ret_val = (uint8)voiceMsgUnpackDspMusicOnHoldReq(p_dest, p_src, &length);
			break;	
#endif
		case VOICE_DSP_CHAN_OPEN_RSP:
		case VOICE_DSP_CHAN_CLOSE_RSP:
		case VOICE_DSP_DESTROY_MEETING_ROOM_RSP:
		case VOICE_DSP_CREATE_INTERNAL_CALL_RSP:
		case VOICE_DSP_SET_FAX_PARAMETER_RSP:
		case VOICE_DSP_START_HOWLER_TONE_RSP:
		case VOICE_DSP_STOP_HOWLER_TONE_RSP:
		case VOICE_DSP_IVR_FUNC_RSP:			
		case VOICE_DSP_START_EARLY_MEDIA_RSP:
		case VOICE_DSP_STOP_EARLY_MEDIA_RSP:
		case VOICE_DSP_START_MUSIC_ONHOLD_RSP:	/* aircheng add for MOH*/
		case VOICE_DSP_STOP_MUSIC_ONHOLD_RSP:		/* aircheng add for MOH*/
			ret_val = (uint8)voiceMsgUnpackDspCommonRsp(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CREATE_MEETING_ROOM_REQ:						
			ret_val = (uint8)voiceMsgUnpackDspCreateMeetingRoomReq(p_dest, p_src, &length);
			break;			
		case VOICE_DSP_CREATE_MEETING_ROOM_RSP:					
			ret_val = (uint8)voiceMsgUnpackDspCreateMeetingRoomRsp(p_dest, p_src, &length);
			break;			
#ifdef VOIP_SYSLOG /*Jason , syslog*/	
		case VOICE_MM_GET_STREAM_LOG_REQ:
			ret_val = (uint8)voiceMsgUnpackDspMmStatsReq(p_dest, p_src, &length);
			break;			
		case VOICE_MM_GET_STREAM_LOG_RSP:
			ret_val = (uint8)voiceMsgUnpackDspMmStatsRsp(p_dest, p_src, &length);
			break;	
#endif			
		case VOICE_MM_SET_DEBUG_REQ:
		case VOICE_MM_SET_DEBUG_RSP:
		case VOICE_MM_DEBUG_STATUS_REQ:
		case VOICE_MM_DEBUG_STATUS_RSP:
		case VOICE_DSP_INIT_REQ:
		case VOICE_MM_KSOCKET_SET_REQ:
		case VOICE_MM_KSOCKET_SET_RSP:
		default:
			length = voiceMsgGetApiLength(api_id) - sizeof(voiceApiHeader_t);
			zyMemcpy(p_dest, p_src, length);
			ret_val = MM_SUCCESS;
	       	break;
	}

    *p_length = length;
	VOICE_IPC_PRINTF("voiceMsgUnpackApi length:%d\n",length);
    return ret_val;
}

/******************************************************************************
 * voiceMsgGetApiLength
 *
 * Description: 
 *	Utility function, used to get voice message length of specified api id
 * Parameters:
 * 	Input
 *    uint16 api_id -voice message api id
 * Return Value: uint32
 *    api length = voice message length
 *****************************************************************************/
uint32 voiceMsgGetApiLength(uint16 api_id)
{
	uint32 api_length = 0;
	uint8 is_valid_api = MM_TRUE;
	switch (api_id) {
		/* requests */
		case VOICE_DSP_INIT_REQ:
			api_length= sizeof(voiceDspInitReq_t);
			break;
		case VOICE_DSP_CHAN_OPEN_REQ:
			api_length = sizeof(voiceDspChanOpenReq_t);
			break;
		case VOICE_DSP_CHAN_CLOSE_REQ:
			api_length = sizeof(voiceDspChanCloseReq_t);
			break;			
		case VOICE_DSP_CREATE_MEETING_ROOM_REQ:
			api_length = sizeof(voiceDspCreateMeetingRoomReq_t);
			break;
		case VOICE_DSP_DESTROY_MEETING_ROOM_REQ:
			api_length = sizeof(voiceDspDestroyMeetingRoomReq_t);
			break;
		case VOICE_DSP_CREATE_INTERNAL_CALL_REQ:
			api_length = sizeof(voiceDspCreateInternalCallReq_t);
			break;
		case VOICE_DSP_SET_FAX_PARAMETER_REQ:
			api_length = sizeof(voiceDspSetFaxParameterReq_t);
			break;
		case VOICE_DSP_START_HOWLER_TONE_REQ:
		case VOICE_DSP_STOP_HOWLER_TONE_REQ:
			api_length = sizeof(voiceDspCommonReq_t);
			break;
		case VOICE_DSP_IVR_FUNC_REQ:
			api_length = sizeof(voiceDspIvrFuncReq_t);
			break;
		case VOICE_DSP_START_EARLY_MEDIA_REQ:
		case VOICE_DSP_STOP_EARLY_MEDIA_REQ:
			api_length = sizeof(voiceDspEarlyMediaReq_t);
			break;
#if 1	/* aircheng add for MOH */
		case VOICE_DSP_START_MUSIC_ONHOLD_REQ:
		case VOICE_DSP_STOP_MUSIC_ONHOLD_REQ:
			api_length = sizeof(voiceDspMusicOnHoldReq_t);
			break;
#endif
		/*responses*/
		case VOICE_DSP_INIT_RSP:
			api_length = sizeof(voiceDspInitRsp_t);
			break;
		case VOICE_DSP_CHAN_OPEN_RSP:
			api_length = sizeof(voiceDspChanOpenRsp_t);
			break;
		case VOICE_DSP_CHAN_CLOSE_RSP:
			api_length = sizeof(voiceDspChanCloseRsp_t);
			break;
		case VOICE_DSP_CREATE_MEETING_ROOM_RSP:
			api_length = sizeof(voiceDspCreateMeetingRoomRsp_t);
			break;
		case VOICE_DSP_DESTROY_MEETING_ROOM_RSP:
			api_length = sizeof(voiceDspDestroyMeetingRoomRsp_t);
			break;		
		
		case VOICE_DSP_CREATE_INTERNAL_CALL_RSP:
			api_length = sizeof(voiceDspCreateInternalCallRsp_t);
			break;
		case VOICE_DSP_SET_FAX_PARAMETER_RSP:
			api_length = sizeof(voiceDspSetFaxParameterRsp_t);
			break;
		case VOICE_DSP_START_HOWLER_TONE_RSP:
		case VOICE_DSP_STOP_HOWLER_TONE_RSP:
		case VOICE_DSP_IVR_FUNC_RSP:
		case VOICE_DSP_START_EARLY_MEDIA_RSP:
		case VOICE_DSP_STOP_EARLY_MEDIA_RSP:
		case VOICE_DSP_START_MUSIC_ONHOLD_RSP:	/* aircheng add for MOH */
		case VOICE_DSP_STOP_MUSIC_ONHOLD_RSP:		/* aircheng add for MOH */
			api_length = sizeof(voiceDspCommonRsp_t);
			break;
		case VOICE_MM_SET_DEBUG_REQ:
			api_length = sizeof(mm_set_debug_req_st);
			break;
		case VOICE_MM_DEBUG_STATUS_REQ:
			api_length = sizeof(mm_debug_status_req_st);
			break;
		case VOICE_MM_SET_DEBUG_RSP:
		case VOICE_MM_DEBUG_STATUS_RSP:
		case VOICE_MM_KSOCKET_SET_RSP:
			api_length = sizeof(mm_common_rsp_st);
			break;
		case VOICE_MM_KSOCKET_SET_REQ:
			api_length = sizeof(mm_ksocket_set_req_st);
			break;
#ifdef VOIP_SYSLOG /*Jason , syslog*/
		case VOICE_MM_GET_STREAM_LOG_REQ:
			api_length = sizeof(voiceDspMmStatsReq_t);
			break;
		case VOICE_MM_GET_STREAM_LOG_RSP:
			api_length = sizeof(voiceDspMmStatsRsp_t);
			break;
#endif
		default:
			is_valid_api=MM_FALSE;
			break;
	}
	
	if(MM_TRUE == is_valid_api)
	{
		api_length += sizeof(voiceApiHeader_t);		
	}

	/* else api_length is 0 already initialised above */

	return api_length;
}

/******************************************************************************
 * voiceMsgPackDspInitRsp
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_INIT_RSP
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspInitRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length) {
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspChanOpenRsp\n");
    voiceDspInitRsp_t    *p_voice_dsp_init_rsp_st = NULL;
    
    p_voice_dsp_init_rsp_st   = (voiceDspInitRsp_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_init_rsp_st->result), &offset);
    
    *p_length    += offset;

	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_init_rsp_st->initDataLength), &offset);
    
    *p_length    += offset;

	zyMemcpy(p_dest +(*p_length), p_voice_dsp_init_rsp_st->initData, p_voice_dsp_init_rsp_st->initDataLength);
	   
    *p_length    += p_voice_dsp_init_rsp_st->initDataLength;
    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgPackDspChanOpenReq
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspChanOpenReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspChanOpenReq\n");
    voiceDspChanOpenReq_t    *p_voice_dsp_chan_open_req_st = NULL;
    
    p_voice_dsp_chan_open_req_st   = (voiceDspChanOpenReq_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->logicalId), &offset);
    
    *p_length    += offset;

	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->physicalId), &offset);
    
    *p_length    += offset;
    
	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->portType), &offset);
    
    *p_length    += offset;

	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->call_id), &offset);
    
    *p_length    += offset;	

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->dtmfMethod), &offset);
    
    *p_length    += offset;	
   
	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->dtmfMethodG711), &offset);
    
    *p_length    += offset;	

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->silenceSupp), &offset);
    
    *p_length    += offset;	

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->echoCancel), &offset);
    
    *p_length    += offset;	

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_chan_open_req_st->mode), &offset);
    
    *p_length    += offset;	
	
    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgPackDspCreateMeetingRoomReq
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_CREATE_MEETING_ROOM_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspCreateMeetingRoomReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspCreateMeetingRoomReq\n");
    voiceDspCreateMeetingRoomReq_t    *p_voice_dsp_create_meeting_room_req_st = ICF_NULL;
    
    p_voice_dsp_create_meeting_room_req_st   = (voiceDspCreateMeetingRoomReq_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_create_meeting_room_req_st->bLogicId), &offset);
    
    *p_length    += offset;

    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackDspCreateMeetingRoomRsp
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_CREATE_MEETING_ROOM_RSP
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspCreateMeetingRoomRsp(uint8 *p_dest,uint8 *p_src,uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspCreateMeetingRoomRsp\n");
    voiceDspCreateMeetingRoomRsp_t    *p_voice_dsp_create_meeting_room_rsp_st = ICF_NULL;
    
    p_voice_dsp_create_meeting_room_rsp_st   = (voiceDspCreateMeetingRoomRsp_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_create_meeting_room_rsp_st->result), &offset);
    
    *p_length    += offset;	
	
    return MM_SUCCESS;
}

/******************************************************************************
 * voice_pack_voice_dsp_ivr_func_rsp_st
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_IVR_FUNC_RSP
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspIvrFuncReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspIvrFuncReq\n");
    voiceDspIvrFuncReq_t    *p_voice_dsp_ivr_func_req_st = ICF_NULL;
    
    p_voice_dsp_ivr_func_req_st   = (voiceDspIvrFuncReq_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_ivr_func_req_st->call_id), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_ivr_func_req_st->priFunc), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_ivr_func_req_st->secFunc), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_ivr_func_req_st->action), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_ivr_func_req_st->itemNum), &offset);
    
    *p_length    += offset;
	
    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgPackDspCommonReq
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api common request(logicalId only)
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspCommonReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspCommonReq\n");
    voiceDspCommonReq_t    *p_voice_dsp_common_req_st = NULL;
    
    p_voice_dsp_common_req_st   = (voiceDspCommonReq_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_common_req_st->logicalId), &offset);
    
    *p_length    += offset;

    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackDspEarlyMediaReq - aircheng modify
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_START_EARLY_MEDIA_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspEarlyMediaReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspEarlyMediaReq\n");
    voiceDspEarlyMediaReq_t    *p_voice_dsp_early_media_req_st = NULL;
    
    p_voice_dsp_early_media_req_st   = (voiceDspEarlyMediaReq_t *)p_src;

    /* Initialize length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_early_media_req_st->call_id), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_early_media_req_st->ivrPlayIndex), &offset);
    
    *p_length    += offset;

    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackDspMusicOnHoldReq - aircheng add
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_START_MUSIC_ONHOLD_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspMusicOnHoldReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspMusicOnHoldReq\n");
    voiceDspMusicOnHoldReq_t    *p_voice_dsp_music_onhold_req_st = NULL;
    
    p_voice_dsp_music_onhold_req_st   = (voiceDspMusicOnHoldReq_t *)p_src;

    /* Initialize length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_music_onhold_req_st->call_id), &offset);
    
    *p_length    += offset;

	voiceMsgPackU16(p_dest + (*p_length), (uint8*)&(p_voice_dsp_music_onhold_req_st->physicalId), &offset);
    
    *p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_music_onhold_req_st->ivrPlayIndex), &offset);
    
    *p_length    += offset;

    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgPackDspCommonRsp
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api common response(result only)
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspCommonRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspCommonRsp\n");
    voiceDspCommonRsp_t    *p_voice_dsp_common_rsp_st = NULL;
    
    p_voice_dsp_common_rsp_st   = (voiceDspCommonRsp_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;
    
	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_common_rsp_st->result), &offset);
    
    *p_length    += offset;
	
    return MM_SUCCESS;
}
#ifdef VOIP_SYSLOG /*Jason , syslog*/
/* BL2348_Config */
/******************************************************************************
 * voiceMsgPackDspMmStatsReq
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspMmStatsReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
	uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspMmStatsReq\n");
	voiceDspMmStatsReq_t    *p_voice_dsp_mm_stats_req_st = NULL;

	p_voice_dsp_mm_stats_req_st   = (voiceDspMmStatsReq_t *)p_src;

ZyPrintf("pack line id:%d, phyPort:%d\n",
	p_voice_dsp_mm_stats_req_st->line_id,
	p_voice_dsp_mm_stats_req_st->phy_port
	);
	/* Initialise length to 0 */
	*p_length    = 0;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_req_st->line_id), &offset);

	*p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_req_st->phy_port), &offset);

	*p_length    += offset;

	return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackDspMmStatsRsp
 *
 * Description: 
 *	Utility function, used to pack data from voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgPackDspMmStatsRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
	uint32        offset = 0;
	VOICE_IPC_PRINTF("voiceMsgPackDspMmStatsRsp\n");
	voiceDspMmStatsRsp_t    *p_voice_dsp_mm_stats_rsp_st = NULL;

	p_voice_dsp_mm_stats_rsp_st   = (voiceDspMmStatsRsp_t *)p_src;

	/* Initialise length to 0 */
	*p_length    = 0;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->result), &offset);

	*p_length    += offset;

	offset = zyStrlen(p_voice_dsp_mm_stats_rsp_st->txCodec);
	zyStrcpy(p_dest + (*p_length), p_voice_dsp_mm_stats_rsp_st->txCodec);
	*(p_dest + (*p_length) + offset) = '\0';

	*p_length    += (offset + 1);

	offset = zyStrlen(p_voice_dsp_mm_stats_rsp_st->rxCodec);
	zyStrcpy(p_dest + (*p_length), p_voice_dsp_mm_stats_rsp_st->rxCodec);
	*(p_dest + (*p_length) + offset) = '\0';

	*p_length    += (offset + 1);

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->txBitRate), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rxBitRate), &offset);

	*p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->txSilenSupp), &offset);

	*p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rxSilenSupp), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->txPktizPeriod), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->SessionDuration), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->txPktCnt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rxPktCnt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->txOctetCnt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rxOctetCnt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->lostPktCnt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->totalCallTime), &offset);

	*p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rcvLostRate), &offset);

	*p_length    += offset;

	voiceMsgPackU8(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->farLostRate), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rtt), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->rcvJitter), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->farJitter), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->avgRcvJitter), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->avgFarJitter), &offset);

	*p_length    += offset;

	voiceMsgPackU32(p_dest + (*p_length), (uint8*)&(p_voice_dsp_mm_stats_rsp_st->avgrtt), &offset);

	*p_length    += offset;

	return MM_SUCCESS;
}
/* BL2348_Config */
#endif

/******************************************************************************
 * voiceMsgUnpackDspInitRsp
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_INIT_RSP
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspInitRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;

    voiceDspInitRsp_t    *p_voice_dsp_init_rsp_st = NULL;
    
    p_voice_dsp_init_rsp_st    = (voiceDspInitRsp_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
   
	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_init_rsp_st->result), p_src + (*p_length), &offset);

	*p_length    += offset;   
   
	voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_init_rsp_st->initDataLength), p_src + (*p_length), &offset);

	*p_length    += offset;

	p_voice_dsp_init_rsp_st->initData=zyMalloc(p_voice_dsp_init_rsp_st->initDataLength);
	
	zyMemcpy(p_voice_dsp_init_rsp_st->initData, p_src +(*p_length), p_voice_dsp_init_rsp_st->initDataLength);
	
	*p_length += p_voice_dsp_init_rsp_st->initDataLength;

return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgUnpackDspChanOpenReq
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspChanOpenReq(uint8 *p_dest,uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspChanOpenReq_t    *p_voice_dsp_chan_open_req_st = NULL;
    
    p_voice_dsp_chan_open_req_st    = (voiceDspChanOpenReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_chan_open_req_st->logicalId), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_chan_open_req_st->physicalId), p_src + (*p_length), &offset);

    *p_length    += offset;
	
    voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->portType), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_chan_open_req_st->call_id), p_src + (*p_length), &offset);

    *p_length    += offset;	

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->dtmfMethod), p_src + (*p_length), &offset);

    *p_length    += offset;	

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->dtmfMethodG711), p_src + (*p_length), &offset);

    *p_length    += offset;	

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->silenceSupp), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->echoCancel), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_chan_open_req_st->mode), p_src + (*p_length), &offset);

    *p_length    += offset;
	
return MM_SUCCESS;
}



/******************************************************************************
 * voiceMsgUnpackDspCreateMeetingRoomReq
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_CREATE_MEETING_ROOM_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspCreateMeetingRoomReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspCreateMeetingRoomReq_t    *p_voice_dsp_create_meeting_room_req_st = NULL;
    
    p_voice_dsp_create_meeting_room_req_st    = (voiceDspCreateMeetingRoomReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_create_meeting_room_req_st->bLogicId), p_src + (*p_length), &offset);

    *p_length    += offset;
   
	return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgUnpackDspCreateMeetingRoomRsp
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_CREATE_MEETING_ROOM_RSP
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspCreateMeetingRoomRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspCreateMeetingRoomRsp_t    *p_voice_dsp_create_meeting_room_rsp_st = NULL;
    
    p_voice_dsp_create_meeting_room_rsp_st    = (voiceDspCreateMeetingRoomRsp_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
    
	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_create_meeting_room_rsp_st->result), p_src + (*p_length), &offset);

    *p_length    += offset;	
	
return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgUnpackDspIvrFuncReq
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_IVR_FUNC_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspIvrFuncReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspIvrFuncReq_t    *p_voice_dsp_ivr_func_req_st = NULL;
    
    p_voice_dsp_ivr_func_req_st    = (voiceDspIvrFuncReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_ivr_func_req_st->call_id), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_ivr_func_req_st->priFunc), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_ivr_func_req_st->secFunc), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_ivr_func_req_st->action), p_src + (*p_length), &offset);

    *p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_ivr_func_req_st->itemNum), p_src + (*p_length), &offset);

    *p_length    += offset;
   
	return MM_SUCCESS;
}



/******************************************************************************
 * voiceMsgUnpackDspCommonReq
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api common request(logicalId only)
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspCommonReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspCommonReq_t    *p_voice_dsp_common_req_st = NULL;
    
    p_voice_dsp_common_req_st    = (voiceDspCommonReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_common_req_st->logicalId), p_src + (*p_length), &offset);

    *p_length    += offset;	

return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgUnpackDspEarlyMediaReq - aircheng modify
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_START_EARLY_MEDIA_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspEarlyMediaReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspEarlyMediaReq_t    *p_voice_dsp_early_media_req_st = NULL;
    
    p_voice_dsp_early_media_req_st    = (voiceDspEarlyMediaReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_early_media_req_st->call_id), p_src + (*p_length), &offset);

    *p_length    += offset;	

    voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_early_media_req_st->ivrPlayIndex), p_src + (*p_length), &offset);

    *p_length    += offset;	

return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgUnpackDspMusicOnHoldReq - aircheng add
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_START_MUSIC_ONHOLD_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspMusicOnHoldReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;
    voiceDspMusicOnHoldReq_t    *p_voice_dsp_music_onhold_req_st = NULL;
    
    p_voice_dsp_music_onhold_req_st    = (voiceDspMusicOnHoldReq_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;    
        
    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_music_onhold_req_st->call_id), p_src + (*p_length), &offset);

    *p_length    += offset;	

    voiceMsgUnpackU16((uint8 *)&(p_voice_dsp_music_onhold_req_st->physicalId), p_src + (*p_length), &offset);

    *p_length    += offset;	

    voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_music_onhold_req_st->ivrPlayIndex), p_src + (*p_length), &offset);

    *p_length    += offset;	

return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgUnpackDspCommonRsp
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api common response( result only)
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspCommonRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;

    voiceDspCommonRsp_t    *p_voice_dsp_common_rsp_st = NULL;
    
    p_voice_dsp_common_rsp_st    = (voiceDspCommonRsp_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;          

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_common_rsp_st->result), p_src + (*p_length), &offset);

    *p_length    += offset;
	
return MM_SUCCESS;
}

#ifdef VOIP_SYSLOG /*Jason , syslog*/
/* BL2348_Config */
/******************************************************************************
 * voiceMsgUnpackDspMmStatsReq
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspMmStatsReq(uint8 *p_dest,uint8 *p_src, uint32 *p_length)
{
	uint32        offset = 0;
	voiceDspMmStatsReq_t    *p_voice_dsp_mm_stats_req_st = NULL;

	p_voice_dsp_mm_stats_req_st    = (voiceDspMmStatsReq_t *)p_dest;

	/* Initialise length to 0 */
	*p_length    = 0;    

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_req_st->line_id), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_req_st->phy_port), p_src + (*p_length), &offset);

	*p_length    += offset;

ZyPrintf("unpack line id:%d, phyPort:%d\n",
	p_voice_dsp_mm_stats_req_st->line_id,
	p_voice_dsp_mm_stats_req_st->phy_port
	);
	return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgUnpackDspMmStatsRsp
 *
 * Description: 
 *	Utility function, used to unpack data to voice message of api VOICE_DSP_CHAN_OPEN_REQ
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS
 *****************************************************************************/
uint32 voiceMsgUnpackDspMmStatsRsp(uint8 *p_dest,uint8 *p_src, uint32 *p_length)
{
	uint32        offset = 0;
	voiceDspMmStatsRsp_t    *p_voice_dsp_mm_stats_rsp_st = NULL;

	p_voice_dsp_mm_stats_rsp_st    = (voiceDspMmStatsRsp_t *)p_dest;

	/* Initialise length to 0 */
	*p_length    = 0;    

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->result), p_src + (*p_length), &offset);

	*p_length    += offset;

	offset = zyStrlen(p_src + (*p_length));
	zyStrcpy(p_voice_dsp_mm_stats_rsp_st->txCodec, p_src + (*p_length));

	*p_length    += (offset + 1);

	offset = zyStrlen(p_src + (*p_length));
	zyStrcpy(p_voice_dsp_mm_stats_rsp_st->rxCodec, p_src + (*p_length));

	*p_length	 += (offset + 1);

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->txBitRate), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rxBitRate), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->txSilenSupp), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rxSilenSupp), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->txPktizPeriod), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->SessionDuration), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->txPktCnt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rxPktCnt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->txOctetCnt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rxOctetCnt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->lostPktCnt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->totalCallTime), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rcvLostRate), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU8((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->farLostRate), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rtt), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->rcvJitter), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->farJitter), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->avgRcvJitter), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->avgFarJitter), p_src + (*p_length), &offset);

	*p_length    += offset;

	voiceMsgUnpackU32((uint8 *)&(p_voice_dsp_mm_stats_rsp_st->avgrtt), p_src + (*p_length), &offset);

	*p_length    += offset;

	return MM_SUCCESS;
}
/* BL2348_Config */
#endif
/******************************************************************************
 * voiceMsgAlloc
 *
 * Description: 
 *	Utility function, used to allocate buffer for voice message
 * Parameters:
 * 	Input
 *	uint16 api_id - api id for this voice message
 *    uint16 payload_length -voice message payload length need for this api
 * Return Value: voiceMsg_t*
 *    allocated 	voice message 
 *****************************************************************************/
voiceMsg_t* voiceMsgAlloc(uint16 api_id, uint16 payload_length){
	voiceMsg_t *p_msg;
	uint16 msg_length=sizeof(voiceApiHeader_t)+payload_length;
	VOICE_IPC_PRINTF("[voiceMsgAlloc] api:%d msg_length:(%d+%d)%d\n",api_id,sizeof(voiceApiHeader_t),payload_length,msg_length);
	p_msg=zyMalloc(msg_length);
	memset((void *)p_msg,0,msg_length);
	p_msg->hdr.api_id=api_id;
	VOICE_IPC_PRINTF("[voiceMsgAlloc] p_msg->hdr.api_id=%d\n",api_id);
	p_msg->hdr.api_length=msg_length;
	VOICE_IPC_PRINTF("[voiceMsgAlloc] p_msg->hdr.api_length=%d\n",msg_length);
	return p_msg;
}
/******************************************************************************
 * voiceMsgFree
 *
 * Description: 
 *	Utility function, used to free voice message
 * Parameters:
 * 	Input
 *    voiceMsg_t *p_msg -voice message
 * Return Value: 
 *    
 *****************************************************************************/
void voiceMsgFree(voiceMsg_t *p_msg){
	VOICE_IPC_PRINTF("[voiceMsgFree]\n");
	/* free memory allcated in the payload pointer*/
	switch(p_msg->hdr.api_id){
		case VOICE_DSP_INIT_RSP:
			{
				voiceDspInitRsp_t *p_payload = NULL;
				p_payload = (voiceDspInitRsp_t *)p_msg->payload;
				if((NULL != p_payload) && (NULL != p_payload->initData)){
					zyFree(p_payload->initData);
				}
			}
			break;
	}
	/*free memory for header and payload*/
	zyFree((void *)p_msg);

}

/******************************************************************************
 * voiceMsgPackVoiceApiHeader
 *
 * Description: 
 *	Utility function, used to pack header to voice message
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgPackVoiceApiHeader(uint8 *p_dest, uint8 *p_src, uint32 *p_length, uint32 *p_api_length_index)
{
    uint32        offset = 0;

    voiceApiHeader_t    *p_voice_api_header_st = NULL;
    
    p_voice_api_header_st    = (voiceApiHeader_t *)p_src;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function packs icf_api_header_st */
      voiceMsgPackU16(p_dest + (*p_length), (icf_uint8_t *)&(p_voice_api_header_st->api_id), &offset);
    
    *p_length    += offset;

	*p_api_length_index = *p_length;

    voiceMsgPackU16(p_dest + (*p_length), (icf_uint8_t *)&(p_voice_api_header_st->api_length), &offset);
    
    *p_length    += offset;
   
    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgUnpackVoiceApiHeader
 *
 * Description: 
 *	Utility function, used to unpack header from voice message
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgUnpackVoiceApiHeader(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    uint32        offset = 0;

    voiceApiHeader_t    *p_voice_api_header_st = NULL;
    
    p_voice_api_header_st    = (voiceApiHeader_t *)p_dest;

    /* Initialise length to 0 */
    *p_length    = 0;

    /* This function is for unpacking icf_api_header_st */
    voiceMsgUnpackU16((icf_uint8_t *)&(p_voice_api_header_st->api_id), p_src + (*p_length), &offset);
    
    *p_length    += offset;    
   
     voiceMsgUnpackU16((icf_uint8_t *)&(p_voice_api_header_st->api_length), p_src + (*p_length), &offset);
    
    *p_length    += offset;
    
    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgUnpackU8
 *
 * Description: 
 *	Utility function, used to unpack 8-bit data from voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgUnpackU8(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    *p_length    = sizeof(icf_uint8_t);

    *p_dest = *p_src;
    
    return MM_SUCCESS;

}

/******************************************************************************
 * voiceMsgUnpackU16
 *
 * Description: 
 *	Utility function, used to unpack 16-bit data from voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgUnpackU16(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    /* Unpacking U16 */
     
    *(uint16*)p_dest    = 0;
    
    *(uint16*)p_dest    |= (uint16)p_src[0] << 8;
    *(uint16*)p_dest    |= (uint16)p_src[1];
    
    *p_length    = sizeof(uint16);

    return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgUnpackU32
 *
 * Description: 
 *	Utility function, used to unpack 32-bit data from voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgUnpackU32(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    /* Unpacking icf_uint32_t */
     
    *(uint32*)p_dest    = 0;

    *(uint32*)p_dest    |= (uint32)p_src[0] << 24;
    *(uint32*)p_dest    |= (uint32)p_src[1] << 16;
    *(uint32*)p_dest    |= (uint32)p_src[2] << 8;
    *(uint32*)p_dest    |= (uint32)p_src[3];
    
    *p_length    = sizeof(uint32);

     return MM_SUCCESS;
}


/******************************************************************************
 * voiceMsgPackU8
 *
 * Description: 
 *	Utility function, used to pack 8-bit data to voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgPackU8(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    *p_dest = *p_src;
    
    *p_length    = sizeof(uint8);
    
    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackU16
 *
 * Description: 
 *	Utility function, used to pack 16-bit data to voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgPackU16(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    /* Packing U16 */

    p_dest[0]    = (uint8)((*(uint16*)p_src) >> 8);
    p_dest[1]    = (uint8)((*(uint16*)p_src));
    
    *p_length    = sizeof(uint16);
    
    return MM_SUCCESS;
}

/******************************************************************************
 * voiceMsgPackU32
 *
 * Description: 
 *	Utility function, used to pack 32-bit data to voice message.
 * Parameters:
 * 	Input
 *    uint8    *p_dest -destination data pointer
 *	uint8    *p_src - source data pointer
 *	Output
 *	uint32 *p_length -data length
 * Return Value: uint32
 *    MM_SUCCESS = success
 *    MM_FAILURE = failure
 *****************************************************************************/
uint32 voiceMsgPackU32(uint8 *p_dest, uint8 *p_src, uint32 *p_length)
{
    /* Packing icf_uint32_t */
    
    p_dest[0]    = (uint8)((*(uint32*)p_src) >> 24);
    p_dest[1]    = (uint8)((*(uint32*)p_src) >> 16);
    p_dest[2]    = (uint8)((*(uint32*)p_src) >> 8);
    p_dest[3]    = (uint8)((*(uint32*)p_src));
    
    *p_length    = sizeof(uint32);

    return MM_SUCCESS;
}

