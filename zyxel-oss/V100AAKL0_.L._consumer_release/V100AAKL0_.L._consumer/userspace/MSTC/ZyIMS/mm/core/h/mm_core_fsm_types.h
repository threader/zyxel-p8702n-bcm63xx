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


#ifndef __MM_CORE_FSM_TYPES_H__
#define __MM_CORE_FSM_TYPES_H__

/***************************************************************************
* Include files area 
**************************************************************************/
#include "global.h"
#include "switch.h"
#include "icf_types.h"
#include "mm_cfg_types.h"
#include "mm_core_rtp.h"
#include "mm_core_fsm_defs.h"
#include "mm_core_ivr.h"
/************************************************************************
*All the typedefs should go here
************************************************************************/


/* the enum defines the type of call that FSM is handling*/
typedef enum
{
	MM_OUTGOING_CALL,
	MM_INCOMING_CALL
}mm_core_calldir_et;

/* Following enum defines the events handled in CORE call FSM */
typedef enum
{
	RM_CORE_GET_GCL_MMI,
	RM_CORE_CODEC_COMMIT_MMI,
	RM_CORE_REL_RES_MMI,
	RM_CORE_LAST_EVENT
}icf_rm_core_event_et;

/* Following enum defines the events handled in CORE call FSM */
typedef enum
{
	RTP_CORE_OPEN_CHANNEL_MMI,
	RTP_CORE_CREATE_SESSION_MMI,
	RTP_CORE_MODIFY_SESSION_MMI,
	RTP_CORE_DELETE_SESSION_MMI,
	RTP_CORE_CLOSE_SESSION_MMI,
	RTP_CORE_MERGE_SESSION_MMI,
	RTP_CORE_CAPABILITIES_MMI,
	RTP_CORE_LAST_EVENT
}icf_rtp_core_event_et;

typedef enum
{
	DSP_CORE_INIT,
	DSP_CORE_CHAN_OPEN, /*vDspChanOpen*/
	DSP_CORE_CHAN_CLOSE, /*vDspChanClose*/	
	DSP_CORE_CREATE_MEETING_ROOM, /*vDspCreateMeetingRoom*/
	DSP_CORE_DESTROY_MEETING_ROOM, /*vDspDestroyMeetingRoom*/	
	DSP_CORE_CREATE_INTERNAL_CALL,
	DSP_CORE_SET_FAX_PARAMETER,
	#ifdef HOWLER_TONE
	DSP_CORE_START_HOWLER_TONE,
	DSP_CORE_STOP_HOWLER_TONE,
	#endif
	DSP_CORE_IVR_FUNC,
	DSP_CORE_START_EARLY_MEDIA,
	DSP_CORE_STOP_EARLY_MEDIA,
#if 1	/* aircheng add for MOH*/
	DSP_CORE_START_MUSIC_ONHOLD,
	DSP_CORE_STOP_MUSIC_ONHOLD,
#endif
#ifdef VOIP_SYSLOG /*Jason , syslog*/
	DSP_MM_STREAMLOG,  /* voiceDspLogStatsFunc,get one stream rtp status */
#endif
	DSP_CORE_LAST_EVENT 
}voiceDspCoreEvent_et;

typedef enum
{
	DSP_IDLE,
	DSP_LAST_STATE
}voiceDspCoreState_et;

typedef enum
{
	CHAN_IDLE,
	CHAN_RESERVED,
	CHAN_COMMITTED,
	CHAN_OPENED,
	CHAN_CHANGE_TO_RESERVED,
	CHAN_WAIT_FOR_CLOSE,
	CHAN_LAST_STATE
}dspChanState_et;

/************************************************************************
*All the typedefs should go here
************************************************************************/
typedef void (*DSPCALLBACK)(icf_call_id_t call_id, icf_stream_id_t	stream_id, icf_media_type_t	stream_type, uint16 logicalId, icf_uint32_t pload_type, icf_uint8_t *packet, icf_uint16_t len, icf_uint32_t timestamp, void* handle);

typedef struct dspChan_s 
{
	uint16				dspId;
	uint16				chanId;					/*dsp pool channel id*/
	uint16				phLogicalId;			/* phone logical channel Id */
	uint16				phPhysicalId;			/* phone physical Id*/
	uint8				phPortType;
	icf_call_id_t		call_id;
	icf_stream_id_t		stream_id;
	icf_media_type_t	stream_type;			/* Video or Audio */		
	#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, codec_type is predefined codec number ZYNOS_CODEC_XXX and codec_num is negociated number used for this media session*/
	icf_uint8_t			codec_type;				/*Eason, dyncodec, pre-define codec number*/
	#endif
	icf_uint8_t			codec_num;				/*Eason, dyncodec, used for dynamic codec number negociation*/
	#if 1 /*Eason, SPRID # 090826582, dynamic codec number selection, negociated telephone-event (rfc2833) payload number*/
	icf_uint8_t			dtmfCodecNum;			/*Eason, dyncodec, pre-define codec number*/
	#endif
	uint16	     		ptime;					/* pref_ptime */
	uint16				silenceSupp;			/* silence_suppression */
	uint16				echoCancel;
	uint8				dtmfMethod;				/*If the parameter dtmfMethodG711 is non-empty, then this parameter applies only when the current codec is not G.711.*/
	uint8				dtmfMethodG711;			/*Method by which DTMF digits MUST be passed if the current codec is G.711*/
	uint16				timeslot;				/*DSP channel timeslot*/
	uint16				timeslotUseType;		/*DSP channel timeslot UseType*/	
	dspChanState_et		chanState;
	IvrPeriodPlay_t		per;	
	DSPCALLBACK 		voipDspCallBack;
	DSPCALLBACK 		voipDspRtcpCallBack;
} dspChan_t;

typedef struct dspConf_s {
	icf_uint16_t		dspId;
	icf_uint16_t		confId;					/* conference call index */
	icf_uint16_t		bLogicalId;				/* conference call peer B phone logical Id */
	icf_uint16_t		cLogicalId;				/* conference call peer C phone logical Id */
	icf_call_id_t		call_id;
	icf_call_id_t		merged_call_id;
	#if 0
	dspChan_t			*bDspChan_p;			/* conference call peer B phone dsp chan */
	dspChan_t			*cDspChan_p;			/* conference call peer C phone dsp chan */
	#endif
} dspConf_t;

typedef struct voiceDsp_s
{
	icf_uint16_t 			dspId;
	dspChan_t*		dspChanList[MAX_DSP_CHAN_PER_DSP];
	dspConf_t* 		dspConfList[MAX_CONF_PER_DSP];	
}voiceDsp_t;

typedef struct dspPool_s
{
	icf_uint8_t				dspCount;
	voiceDsp_t			voiceDsp[MAX_DSP];
	icf_uint8_t				pcmTimeslotUsed[MAX_TIMESLOT_NUM];
}dspPool_t;

/************************************************************************
*All the typedefs should go here
************************************************************************/


typedef struct mmCtxStream_s
{
	icf_stream_id_t	stream_id;
	icf_media_type_t	stream_type;
	icf_sdp_stream_st		local_sdp;
	icf_sdp_stream_st	remote_sdp;
	icf_media_mode_t	media_mode;	/* INVALID:0/INACTIVE:1/SEND_ONLY:2/RECV_ONLY:3/SEND_RECV:4 */
	icf_qos_mark_t	qos_mark;
	icf_boolean_t	rtp_detection_reqd;
	icf_boolean_t	echo_cancellation;
}mmCtxStream_t;


typedef struct mmCoreCallCtx_s
{
	icf_call_id_t					call_id;  /* key to identify the FSM */
	mm_core_calldir_et			call_direction; /* type of call incoming or outgoing*/
	icf_line_id_t					line_id;
	icf_uint8_t					stream_count;	
	icf_list_st						*p_stream_list;	/*mmCtxStream_t*/
	rtpTable_t					*p_rtp_session;
	mmCfgDb_t 					*p_mm_cfg_db;
}mmCoreCallCtx_t;

/* Link list to keep the call context */

typedef struct mmCoreCallCtxList_s
{
	mmCoreCallCtx_t			call_context;
	struct mmCoreCallCtxList_s		*next;
}mmCoreCallCtxList_t;


/************************************************************************

************************************************************************/

typedef icf_return_t (*mmCoreStateEventFprtTy)
						(icf_void_t *input_param,
                         mmCoreCallCtxList_t	*p_call_context_node,
                         icf_error_t	*err);

#endif
