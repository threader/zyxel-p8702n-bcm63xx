/*
   $Id: Dsp_itf.h 1.6 2007/04/18 12:58:12 eric.chung Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2003-2005 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 *
 *************************************************************************/
 /*
  $Log: Dsp_itf.h $
  Revision 1.6  2007/04/18 12:58:12  eric.chung
  add DSP play tone function for ISDN
  Revision 1.5  2006/10/02 01:37:29  BlueLan
  add multiple_dsp feature
  Revision 1.4  2006/09/29 02:46:56  tommyhsu
  Fix send off-hook caller ID can't find DSP channel
  Revision 1.3  2006/09/22 07:31:45  Weicheng
  Support GNU compiler and Danube platform
  Revision 1.2  2006/04/28 08:19:15  Eric_Chen
  Add dsp pool with TI 5402 driver
  Revision 1.1  2006/04/26 14:47:56  Eric_Chen
  Initial revision
  */

#include "switch.h"

#ifdef VOIP_SYSLOG  /*Jason , syslog*/
#include "config_voip_stats.h"
#endif   
/*____________________________________________________________________________*/
/*	Macro                                                                                                             */
/*____________________________________________________________________________*/
#define Voice_Data		1
#define FAX_Data			2
#define NoTonenRelay		3

/*____________________________________________________________________________*/
/*	function prototypes                                                                                                             */
/*____________________________________________________________________________*/
typedef enum {
	SENDONLY,
	RECVONLY, 
	SENDRECV,
	CONFERENCE,
	INACTIVE,
	LOOPBACK,
	TEST,
	REPLICATE,
	#if 1 /*Eason, Added for 404L01 voice driver*/
	TRANSCODE
	#endif
}chan_mode_t;

/*____________________________________________________________________________*/
/*	Function                                                                                                           */
/*____________________________________________________________________________*/
void DSP_Init(void);
#ifdef MULTIPLE_DSP
int vDspChanOpen(phoneCh_t *phoneCB,int dsp);
#else
int vDspChanOpen(phoneCh_t *phoneCB);
int vDspChanOpenRecvonly(phoneCh_t *phoneCB);
#endif
int vDspChanClose(phoneCh_t *phoneCB);
int vDSPStartCIDFSK(phoneCh_t *phoneCB);
int vDSPStartOffHookCIDFSK(phoneCh_t *phoneCB);
int vDSPStopCIDFSK(phoneCh_t *phoneCB);
int vDSPStartCIDDTMF(phoneCh_t *phoneCB);
int vDSPStopCIDDTMF(phoneCh_t *phoneCB);
int vDSPDetectCIDFSK(phoneCh_t *phoneCB);
int vDspStartDtmfTone(phoneCh_t *phoneCB, uint16 onTime);
int vDspStopDtmfTone(phoneCh_t *phoneCB);
int vDSPToneOn(phoneCh_t *phoneCB, uint8 Type, uint16 onTime, uint16 offTime, uint16 repNum);
int vDSPToneOff(phoneCh_t *phoneCB);
void vDspEnableToneDetection(phoneCh_t *phoneCB);
void vDspDisableToneDetection(phoneCh_t *phoneCB);
int vDspSetFAXParameter(phoneCh_t *phoneCB);
int vDspSetFAXT38Mode(phoneCh_t *phoneCB, T38Info_t *T38Info_p);
int vDspEnableVAD(phoneCh_t *phoneCB);
int vDspDisableVAD(phoneCh_t *phoneCB);

#ifdef VOIP_PHASEII
int dspSendToDsp(uint16 chId, uint32 pload_type, uint8 *packet, uint16 len, uint32 timestamp, void* handle);
#else
int dspSendToDsp(uint16 chId, uint8 *packet, uint16 len, uint32 timestamp);
#endif

uint32 DSPGetAvailCodecList(void);
uint32 DSPCheckCodecAvail(uint8);

#ifdef CALL_CONFERENCE
int vDspCreateMeetingRoom(phoneCh_t* b_phoneCB);
int vDspDestroyMeetingRoom(uint16 baseLogicalId);
int vDspCreateLocalPeerMeetingRoom(phoneCh_t* b_phoneCB);
int vDspDestroyLocalPeerMeetingRoom(uint16 baseLogicalId);
#endif
#ifdef VOIP_SYSLOG /*Jason , syslog*/
int vDspStreamStatsReq(	uint32 lineID,	uint8  phy_port, Config_VoIP_Stats_t *stats);
#endif

