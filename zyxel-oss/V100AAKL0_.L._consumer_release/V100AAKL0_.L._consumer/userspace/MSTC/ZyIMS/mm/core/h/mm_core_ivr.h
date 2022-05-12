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
#ifndef _MM_CORE_IVR_H
#define _MM_CORE_IVR_H

#include "ivr_common.h"

#ifdef DEFAULT_EARLYMEDIA_RINGTONE
typedef struct {
	chann_sig_t sig;		/* channel signal parameters	*/
} ivrCb_t;
#endif

#ifdef DEFAULT_EARLYMEDIA_RINGTONE
extern ivrCb_t ivrCb;
#endif

typedef struct {
	uint32 	index;
	char 	country[4];
} voiceCountIndex_t;

typedef struct {
	uint32	periodTimerId;
	uint32	ReplayTimerId;
	uint32	elementStartAddr;
	uint8*	assemPakcet;			/* used in ivrPlayMultiStart. please several voices continuously */
	uint8*	packet;
	uint16	numOfPackets;
	uint16	seqOfPacket;
	uint32	Interval;				/* 0: means one time play,  !0 means delay interval then repeat play */
	uint8   Repeat;
	uint16	packetsize;
	uint16	packettime;
	RTPCALLBACK voipIvrCallBack;
} IvrPeriodPlay_t;

#if 0
/*
 * RTP data header
 */ 
 #ifdef __ghs__
#pragma pack(1)
#endif
typedef struct {
	unsigned int version:2;		/* protocol version */
	unsigned int p:1;			/* padding flag */
	unsigned int x:1;			/* header extension flag */
	unsigned int cc:4;			/* CSRC count */
	unsigned int m:1;			/* marker bit */
	unsigned int pt:7;			/* payload type */
	unsigned int seq:16;		/* sequence number */
	uint32 ts;				/* timestamp */
	uint32 ssrc;				/* synchronization source */
} rtp_header_t;
#ifdef __ghs__
#pragma pack()
#endif
#endif

#ifdef __ghs__
#pragma pack(1)
#endif
typedef struct VoiceElement_s{
	uint32		element_length;		/*The length of element, start from element_length field*/
	rtp_header_t	rtp_header;
} VoiceElement_t;
#ifdef __ghs__
#pragma pack()
#endif


#ifdef __ghs__
#pragma pack(1)
#endif
typedef struct VoiceLebel_s{
	uint8	used;					/*0: not used, 1:used*/
	uint8	type;					/*0:TYPE_IVR_SYS_DEFAULT, 1:TYPE_IVR_USR_DEFINED*/
	uint8	reserved[2];
	uint32	offset_to_element;		/*the offset from the start of header to the start of the corresponding element*/
} VoiceLabel_t;
#ifdef __ghs__
#pragma pack()
#endif


#ifdef __ghs__
#pragma pack(1)
#endif
typedef struct VoiceHeader_s {
	uint32	crc;
	VoiceLabel_t	label[Max_Voice_Elements];
	uint32	total_elements_length;	/*The total voice elements length. Start from the first element head to the last element tail*/
} VoiceHeader_t;
#ifdef __ghs__
#pragma pack()
#endif

extern uint32  ivrsys_startaddr;
extern uint32	 ivrsys_length;
#ifndef N0_IVRUSR_PATH
extern uint32	 ivrusr_startaddr;
extern uint32	 ivrusr_length;
#endif

void	ivrRecordHalt(int ivrIndex);
int ivrRecordStop(int ivrIndex);
#endif

