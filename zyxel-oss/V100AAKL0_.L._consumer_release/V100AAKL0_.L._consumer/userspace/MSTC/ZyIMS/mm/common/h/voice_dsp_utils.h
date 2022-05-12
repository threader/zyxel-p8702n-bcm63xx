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
#ifndef __VOICE_DSP_UTILS_H__
#define __VOICE_DSP_UTILS_H__

#include "switch.h" 


/***********************Voice message encode/decode functions***********************/
uint8 voiceMsgEncode(voiceMsg_t*   p_msg, /* Input*/
							uint8** p_p_encoded_buf, /* Output*/
							uint32* p_encoded_buf_length,
							uint8*  p_ecode);

uint8 voiceMsgDecode(voiceMsg_t**  p_p_msg, /* Output*/
							uint8*  p_encoded_buf,
							uint32  encoded_buf_length,
							uint8*  p_ecode);


uint8 voiceMsgPackApi
(
    uint8     *p_dest,
    uint8     *p_src,
    uint32    *p_length,
    uint16    api_id   
);
uint8 voiceMsgUnpackApi
(
    uint8     *p_dest,
    uint8     *p_src,
    uint32    *p_length,
    uint16    api_id
);


uint32 voiceMsgGetApiLength(icf_uint16_t api_id);



/*****************Voice message pack functions********************************/
uint32
voiceMsgPackDspInitRsp

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspChanOpenReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspChanOpenRsp

(
    uint8    *p_dest,
    uint8    *p_src,
    uint32   *p_length
);

uint32
voiceMsgPackDspCreateMeetingRoomReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspCreateMeetingRoomRsp

(
    uint8    *p_dest,
    uint8    *p_src,
    uint32   *p_length
);

uint32
voiceMsgPackDspIvrFuncReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspCreateInternalCallReq

(
    uint8    *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspCommonReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspEarlyMediaReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspMusicOnHoldReq

(
    uint8  *p_dest,
    uint8    *p_src,
    uint32   *p_length
);


uint32
voiceMsgPackDspCommonRsp

(
    uint8    *p_dest,
    uint8    *p_src,
    uint32   *p_length
);



/*****************Voice message unpack functions*******************************/
uint32
voiceMsgUnpackDspInitRsp
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgUnpackDspChanOpenReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);
uint32
voiceMsgUnpackDspChanOpenRsp
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);

uint32
voiceMsgUnpackDspCreateMeetingRoomReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);
uint32
voiceMsgUnpackDspCreateMeetingRoomRsp
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);
uint32
voiceMsgUnpackDspIvrFuncReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgUnpackDspCreateInternalCallReq

(
    uint8    *p_dest,
    uint8    *p_src,
    uint32   *p_length
);

uint32
voiceMsgUnpackDspCommonReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);

uint32
voiceMsgUnpackDspEarlyMediaReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgUnpackDspMusicOnHoldReq
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


uint32
voiceMsgUnpackDspCommonRsp
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length
);


#ifdef VOIP_SYSLOG /*Jason , syslog*/
/* BL2348_Config */
uint32 voiceMsgPackDspMmStatsReq(uint8 *p_dest, uint8 *p_src, uint32 *p_length);
uint32 voiceMsgPackDspMmStatsRsp(uint8 *p_dest, uint8 *p_src, uint32 *p_length);
uint32 voiceMsgUnpackDspMmStatsReq(uint8 *p_dest,uint8 *p_src, uint32 *p_length);
uint32 voiceMsgUnpackDspMmStatsRsp(uint8 *p_dest,uint8 *p_src, uint32 *p_length);
/* BL2348_Config */
#endif


/**********************utils*****************************/
voiceMsg_t* voiceMsgAlloc(uint16 api_id, uint16 payload_length);
void voiceMsgFree(voiceMsg_t *p_msg);
uint32
voiceMsgPackVoiceApiHeader
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32    *p_length,
	uint32	*p_api_length_index
);
uint32
voiceMsgUnpackVoiceApiHeader
(
    uint8    *p_dest,
    uint8    *p_src,
    uint32 *p_length
);
uint8 voiceEventIpcSend( int sock_fd,
								const struct sockaddr *p_addr, 
								int sockaddr_len,
								voiceMsg_t *p_msg);





#define MAX_SEND_SIZE 2000

#endif
