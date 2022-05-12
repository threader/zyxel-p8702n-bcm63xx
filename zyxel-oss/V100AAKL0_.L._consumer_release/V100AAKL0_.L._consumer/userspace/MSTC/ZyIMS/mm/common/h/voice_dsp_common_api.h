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
#ifndef __VOICE_DSP_COMMON_API_H__
#define __VOICE_DSP_COMMON_API_H__


#define VOICE_DSP_API_START 					5000
#define VOICE_DSP_INIT_REQ		 				VOICE_DSP_API_START+1
#define VOICE_DSP_INIT_RSP		 				VOICE_DSP_API_START+2
#define VOICE_DSP_CHAN_OPEN_REQ 				VOICE_DSP_API_START+3 /*vDspChanOpen*/
#define VOICE_DSP_CHAN_OPEN_RSP 				VOICE_DSP_API_START+4
#define VOICE_DSP_CHAN_CLOSE_REQ 				VOICE_DSP_API_START+5 /*vDspChanClose*/
#define VOICE_DSP_CHAN_CLOSE_RSP 				VOICE_DSP_API_START+6 /*vDspChanClose*/
#define VOICE_DSP_CREATE_MEETING_ROOM_REQ 				VOICE_DSP_API_START+7 /*vDspCreateMeetingRoom*/
#define VOICE_DSP_CREATE_MEETING_ROOM_RSP 				VOICE_DSP_API_START+8 /*vDspCreateMeetingRoom*/
#define VOICE_DSP_DESTROY_MEETING_ROOM_REQ 				VOICE_DSP_API_START+9 /*vDspDestroyMeetingRoom*/
#define VOICE_DSP_DESTROY_MEETING_ROOM_RSP 				VOICE_DSP_API_START+10 /*vDspDestroyMeetingRoom*/
#define VOICE_DSP_CREATE_LOCALPEER_MEETING_ROOM_REQ 	VOICE_DSP_API_START+11 /*vDspCreateLocalPeerMeetingRoom*/
#define VOICE_DSP_CREATE_LOCALPEER_MEETING_ROOM_RSP 	VOICE_DSP_API_START+12 /*vDspCreateLocalPeerMeetingRoom*/
#define VOICE_DSP_DESTROY_LOCALPEER_MEETING_ROOM_REQ 	VOICE_DSP_API_START+13 /*vDspDestroyLocalPeerMeetingRoom*/
#define VOICE_DSP_DESTROY_LOCALPEER_MEETING_ROOM_RSP 	VOICE_DSP_API_START+14 /*vDspDestroyLocalPeerMeetingRoom*/
#define VOICE_DSP_CREATE_INTERNAL_CALL_REQ				VOICE_DSP_API_START+15 /*vDspCreateInternalCall*/
#define VOICE_DSP_CREATE_INTERNAL_CALL_RSP				VOICE_DSP_API_START+16 /*vDspCreateInternalCall*/
#define VOICE_DSP_SET_FAX_PARAMETER_REQ					VOICE_DSP_API_START+17 /*vDspSetFaxParameter*/
#define VOICE_DSP_SET_FAX_PARAMETER_RSP					VOICE_DSP_API_START+18 /*vDspSetFaxParameter*/
#define VOICE_DSP_START_HOWLER_TONE_REQ					VOICE_DSP_API_START+19 /*vDspStartHowlerTone*/
#define VOICE_DSP_START_HOWLER_TONE_RSP					VOICE_DSP_API_START+20 
#define VOICE_DSP_STOP_HOWLER_TONE_REQ					VOICE_DSP_API_START+21 /*vDspStopHowlerTone*/
#define VOICE_DSP_STOP_HOWLER_TONE_RSP					VOICE_DSP_API_START+22 
#define VOICE_DSP_IVR_FUNC_REQ							VOICE_DSP_API_START+23 /*vDspIVRFunc*/
#define VOICE_DSP_IVR_FUNC_RSP							VOICE_DSP_API_START+24 /*vDspIVRFunc*/
#define VOICE_DSP_START_EARLY_MEDIA_REQ					VOICE_DSP_API_START+25 /*vDspStartEarlyMedia*/
#define VOICE_DSP_START_EARLY_MEDIA_RSP					VOICE_DSP_API_START+26 /*vDspStartEarlyMedia*/
#define VOICE_DSP_STOP_EARLY_MEDIA_REQ					VOICE_DSP_API_START+27 /*vDspStopEarlyMedia*/
#define VOICE_DSP_STOP_EARLY_MEDIA_RSP					VOICE_DSP_API_START+28 /*vDspStopEarlyMedia*/
#if 1	/* aircheng add for MOH*/
#define VOICE_DSP_START_MUSIC_ONHOLD_REQ				VOICE_DSP_API_START+29 /*vDspStartMusicOnHold*/
#define VOICE_DSP_START_MUSIC_ONHOLD_RSP				VOICE_DSP_API_START+30 /*vDspStartMusicOnHold*/
#define VOICE_DSP_STOP_MUSIC_ONHOLD_REQ					VOICE_DSP_API_START+31 /*vDspStopMusicOnHold*/
#define VOICE_DSP_STOP_MUSIC_ONHOLD_RSP					VOICE_DSP_API_START+32 /*vDspStopMusicOnHold*/
#endif

#define VOICE_MM_SET_DEBUG_REQ							VOICE_DSP_API_START+33
#define VOICE_MM_SET_DEBUG_RSP							VOICE_DSP_API_START+34
#define VOICE_MM_DEBUG_STATUS_REQ						VOICE_DSP_API_START+35
#define VOICE_MM_DEBUG_STATUS_RSP						VOICE_DSP_API_START+36
#define VOICE_MM_KSOCKET_SET_REQ							VOICE_DSP_API_START+37
#define VOICE_MM_KSOCKET_SET_RSP							VOICE_DSP_API_START+38
#if 1/*Jason , syslog*/
#define VOICE_MM_GET_STREAM_LOG_REQ						VOICE_DSP_API_START+39 /* */
#define VOICE_MM_GET_STREAM_LOG_RSP						VOICE_DSP_API_START+40 /* */
#endif

#define VOICE_DSP_API_END									VOICE_DSP_API_START+41

typedef struct voiceDspInitReq_s{	
	uint16 country;
}voiceDspInitReq_t;


typedef struct voiceDspChanOpenReq_s{
	uint16 	logicalId;		/* phone logicalId */
	uint16 	physicalId; 	/* phone physicalId */
	uint8 	portType; 		/* FXS, FXO, DECT... */
	uint16 	call_id;		/* iptk call_id*/
	uint8 	dtmfMethod; 	/* Inband, SIPInfo, rfc2833 */
	uint8 	dtmfMethodG711; /* Inband, SIPInfo, rfc2833 
							    Follow TR-104 if the current codec is G.711 */	
	uint8 	silenceSupp; 	/* silence suppression on/off */
	uint8 	echoCancel; 	/* echo cancellation on/off */
	uint8	mode;			/* send/recv mode */
}voiceDspChanOpenReq_t;

typedef struct voiceDspChanCloseReq_s{
	uint16 	logicalId;		/*phone logicalId*/
}voiceDspChanCloseReq_t;

typedef struct voiceDspCreateMeetingRoomReq_s{
	uint16 	bLogicId;		/*peer b phone logicalId*/
}voiceDspCreateMeetingRoomReq_t;

typedef struct voiceDspDestroyMeetingRoomReq_s{
	uint16 	bLogicId;		/*peer b phone logicalId*/
}voiceDspDestroyMeetingRoomReq_t;

typedef struct voiceDspCreateInternalCallReq_s{
	uint16 logicalId; 		/*phone logicalId*/
}voiceDspCreateInternalCallReq_t;

typedef struct voiceDspSetFaxParameterReq_s{
	uint16 logicalId; /*phone logicalId*/
}voiceDspSetFaxParameterReq_t;

typedef struct voiceDspIvrFuncReq_s{
	uint16 call_id;
	uint8 priFunc;
	uint8 secFunc;
	uint8 action;
	uint8 itemNum;
}voiceDspIvrFuncReq_t;

typedef struct voiceDspCommonReq_s{
	uint16 logicalId;
}voiceDspCommonReq_t;


typedef struct voiceDspEarlyMediaReq_s{
	uint16 call_id;
	uint8 ivrPlayIndex;	/* aircheng add */
}voiceDspEarlyMediaReq_t;

typedef struct voiceDspMusicOnHoldReq_s{
	uint16 call_id;
	uint16 physicalId;	/* aircheng add */
	uint8 ivrPlayIndex;	/* aircheng add */
}voiceDspMusicOnHoldReq_t;


typedef struct voiceDspInitRsp_t{
	uint8 result;
	uint16 initDataLength;
	uint8 * initData;	
}voiceDspInitRsp_t;

typedef struct voiceDspChanOpenRsp_s{	
	uint8 	result;			/* MM_SUCCESS or MM_FALIURE */
}voiceDspChanOpenRsp_t;

typedef struct voiceDspChanCloseRsp_s{
	uint8 	result;			/* MM_SUCCESS or MM_FALIURE */
}voiceDspChanCloseRsp_t;

typedef struct voiceDspCreateMeetingRoomRsp_s{
	uint8 	result;
}voiceDspCreateMeetingRoomRsp_t;

typedef struct voiceDspDestroyMeetingRoomRsp_s{
	uint8 result;	
}voiceDspDestroyMeetingRoomRsp_t;

typedef struct voiceDspCreateInternalCallRsp_s{
	uint8 result;
}voiceDspCreateInternalCallRsp_t;

typedef struct voiceDspSetFaxParameterRsp_s{
	uint8 result;
}voiceDspSetFaxParameterRsp_t;

typedef struct voiceDspCommonRsp_s{
	uint8 result;
}voiceDspCommonRsp_t;


#if 1 /*Jason , syslog*/
/*BL2348_Config*/
typedef struct voiceDspMmStatsReq_s{
	uint32 	line_id;       /* Line ID */
	uint8	phy_port;
}voiceDspMmStatsReq_t;

typedef struct voiceDspMmStatsRsp_s{
	uint8 result;
	char txCodec[65];
	char rxCodec[65];
	uint32 txBitRate;
	uint32 rxBitRate;
	uint8 txSilenSupp;
	uint8 rxSilenSupp;
	uint32 txPktizPeriod;
	uint32 SessionDuration;
	uint32 txPktCnt;	/* RTP Packets sent */
	uint32 rxPktCnt;	/* RTP Packets received */
	uint32 txOctetCnt;	/* RTP payload bytes sent */
	uint32 rxOctetCnt;	/* RTP payload bytes received */
	uint32 lostPktCnt;	/* RTP Packets lost */
	uint32 totalCallTime; /* total call session duration */
	uint8 rcvLostRate;    /* receive packet lost rate */
	uint8 farLostRate;    /* far end packet lost rate */
	uint32 rtt;           /* round trip delay */
	uint32 rcvJitter;     /* receive interarrival jitter */
	uint32 farJitter;     /* far end receive interarrival jitter */
	uint32 avgRcvJitter;
	uint32 avgFarJitter;
	uint32 avgrtt;
}voiceDspMmStatsRsp_t;
/*BL2348_Config*/
#endif


/* Debug Command*/
typedef struct{
	uint32 bit_mask;
#define MM_SET_IPTK_DEBUG	0x01
#define MM_SET_RTP_DEBUG	0x02    
#define MM_SET_DSP_DEBUG	0x04
#define MM_SET_IVR_DEBUG	0x08

	uint8 iptk_debug;
	uint8 rtp_debug;
	uint8 dsp_debug;
	uint8 ivr_debug;
	uint8 dest;	
}mm_set_debug_req_st;

typedef struct{
	uint8 status_rm;
	uint8 status_rtp;
	uint8 status_dsp;
	uint8 status_ivr;
	uint8 reserved;
}mm_debug_status_req_st;

typedef struct{	
	uint8 enable;
}mm_ksocket_set_req_st;


typedef struct{
	uint8 result;
}mm_common_rsp_st;

/* Debug Command End*/

#define VOICE_DSP_MAX_API_LENGTH	512

typedef enum _VOICE_ERROR {
    VOICE_ERR_UNKNOWN,
    VOICE_ERR_NULL_POINTER,
    VOICE_ERR_MEMGET_FAILED,
    VOICE_ERR_MEMFREE_FAILED,
    VOICE_ERR_ENCODE_FAILED,
    VOICE_ERR_DECODE_FAILED,
    VOICE_ERR_INVALID_LENGTH
} _VOICE_ERROR;

/****************Voice data structures*************************************/

typedef struct voiceApiHeader_s                       
{

    /* a unique identifier for representing API received */
  	uint16          api_id;
    /* Total length of payload + API header   */
    uint16          api_length;
} voiceApiHeader_t;



typedef struct voiceMsg_s
{

      /* API Message header */
      voiceApiHeader_t   hdr;

      /* This is first byte of the payload. This is a strechable array. 
       * this field can be cast to the API specific structure 
       */ 
      uint8         payload[1];
} voiceMsg_t;
/***********************macros*********************************************/
#define VOICE_MSG_GET_PAYLOAD(p_msg )\
		((uint8*)(p_msg) + sizeof(voiceApiHeader_t))

#endif

