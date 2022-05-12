/*
  $Id: Phcmd.h 1.14 2006/04/25 11:03:07 Eric_Chen Exp $
*/
/******************************************************************************/
/*                                                    
 *  Copyright (C) 2004 ZyXEL Communications, Inc.
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

/*
  $Log: Phcmd.h $
  Revision 1.14  2006/04/25 11:03:07  Eric_Chen
  Phone & DSP common APIs Support
  Revision 1.13  2006/04/19 06:45:33  Eric_Chen
  Replace FSM and hardware depend related files to phase 2
  Revision 1.18  2006/04/18 13:45:32  Eric_Chen
  Add DSP Pool with PMC platform
  Revision 1.17  2006/02/19 09:58:18  Eric_Chen
  Remove PMC4200_PORTING_TEMP flag & Change FXO flag to FXO_DSPRELAY or remove it
  Revision 1.16  2006/01/12 10:44:20  ohsieh
  Add FXO Support.
  Revision 1.15  2006/01/05 08:42:18  cjalee
  Support Country_Code feature and remove Caller_ID compiler flag.
  Revision 1.14  2006/01/02 07:25:05  cjalee
  Add SUPPORT_CALLER_ID compiler flag
  Revision 1.13  2005/12/26 06:39:18  kycheng
  Add PMC4200_PORTING_TEMP
  Revision 1.12  2005/10/31 02:54:52  khung
  FXO_DSPRELAY for DSP relay
  Revision 1.11  2005/08/31 07:35:48  ErinLo
  Add Distinctive Ring Feature
  Revision 1.10  2005/08/23 08:02:16  kehuang
  Add DO_NOT_DISTURB of CALL_FILTER
  Revision 1.9  2005/06/30 02:33:23Z  ohsieh
  Add G723 Support
  Revision 1.8  2005/05/26 12:07:33  hchsu
  Polarity reverse support
  Revision 1.7  2005/05/24 08:30:46  KevinYeh
  OffHook CallerId support during call waiting(DTAS tone).
  Revision 1.6  2005/03/29 05:58:24  cwchen
  Support onHook CallerID by ETSI 659-1.
  Revision 1.5  2005/02/01 12:02:00  cwchen
  Remove some warning message during executing the compiler.
  Revision 1.4  2004/12/15 12:06:48  cwchen
  Support the Call-Waiting Tone.
  Revision 1.3  2004/12/03 06:58:10  cwchen
  Fix the non-voice when the callee picks up the phone at the first ring.
  Revision 1.2  2004/11/15 11:57:29  cwchen
  Fix the caller-ID could not be shown at the phone-2.
  Revision 1.10  2004/07/28 13:39:55  yjlee
  Rename phPlayDtmfTone() as phPlayTone().
  Revision 1.9  2004/07/07 03:12:59  ohsieh
  Revision 1.8  2004/06/28 10:19:20  yjlee
  Add play DTMF tone function.
  Revision 1.7  2004/05/26 05:55:00  ohsieh
  Add Caller-ID Support
  Revision 1.6  2004/05/25 05:51:44  cwchen
  Support DTMF Relay.
  Revision 1.5  2004/05/19 03:45:13  cwchen
  Support fxsHookSt() at SI3210.
  Revision 1.4  2004/05/11 06:39:11  ohsieh
  Add Send CID Function
  Revision 1.3  2004/05/07 04:09:50  cwchen
  Support "FXO_LIFELINE".
  Revision 1.2  2004/04/26 02:34:20  bchung
  Add phone status API.
  Revision 1.1  2004/02/21 08:30:11  cwchen
  Initial revision
  Revision 1.1  2004/01/09 15:00:00  cwchen
  Initial Version
*/
#ifndef _PHCMD_H_INCLUDE
#define _PHCMD_H_INCLUDE

#include "phfsm.h"

/*____________________________________________________________________________*/
/*	constant                                                                                                                        */
/*____________________________________________________________________________*/
/* The definition for the call id */
#define CALL_ID_SUBSCRIBE	1000
#define CALL_ID_UPBOUND	32768	/* 0x8000 */

/*____________________________________________________________________________*/
/*	macro                                                                                                                        */
/*____________________________________________________________________________*/
/* DTMF tone */
#define VOIP_DTMF_0		0
#define VOIP_DTMF_1		1
#define VOIP_DTMF_2		2
#define VOIP_DTMF_3		3
#define VOIP_DTMF_4		4
#define VOIP_DTMF_5		5
#define VOIP_DTMF_6		6
#define VOIP_DTMF_7		7
#define VOIP_DTMF_8		8
#define VOIP_DTMF_9		9
#define VOIP_DTMF_A		10
#define VOIP_DTMF_B		11
#define VOIP_DTMF_C		12
#define VOIP_DTMF_D		13
#define VOIP_DTMF_ASTR	14
#define VOIP_DTMF_PUMP	15
/* Call Progress Tone */
#define VOIP_Dial_Tone	16
#define VOIP_Busy_Tone	17
#define VOIP_Ring_Back	18
#define VOIP_CALL_WAITING	21

#define VOIP_PCM_COMPANDING_DEFAULT   0
#define VOIP_PCM_COMPANDING_A_LAW     1
#define VOIP_PCM_COMPANDING_MU_LAW    2
#define VOIP_PCM_COMPANDING_LINEAR    3

/* SLIC LINE_STATE */
typedef enum  {
	OPEN,						// 0
	FORWARD_ACTIVE,			// 1
	ON_HOOK_TRANSMISSION,	// 2
	TIP_OPEN,					// 3
	RING,						// 4
	REVERSE_ACTIVE,			// 5
	REVERSE_ON_HOOK_TRANSMISSION,	//6
	RING_OPEN					//7
};

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef struct {
	int possibleFlash;
	uint16 slicChId;
	tm_id_t onHookTimer;
	uint32 flags;
	bool		FirstRing;
} SLICCheck_struct;

/*____________________________________________________________________________*/
/*	function                                                                                                                      */
/*____________________________________________________________________________*/
int phStopTone(phoneCh_t *phoneCB);
int phStartDialTone(phoneCh_t *phoneCB);
int phStartMWITone(phoneCh_t *phoneCB);
int phStartBusyTone(phoneCh_t *phoneCB);
int phStartRingBackTone(phoneCh_t *phoneCB);
int phStartReorderTone(	phoneCh_t *phoneCB);
int phStartCongestTone(phoneCh_t *phoneCB);
int phStartBeepTone(phoneCh_t *phoneCB);
int phStartCallWaitingTone(phoneCh_t *phoneCB);
#ifdef DO_NOT_DISTURB
int phStartSpecialDialTone(phoneCh_t *phoneCB);
#endif
int phEnableDtmfDetect(phoneCh_t *phoneCB);
int phDisableDtmfDetect(phoneCh_t *phoneCB);
int phPolarityReverse(phoneCh_t *phoneCB);
int phStartRing(phoneCh_t *phoneCB);
int phStopRing(phoneCh_t *phoneCB);
void phSendCID(phoneCh_t *phoneCB);
void phSetSLICStatus(uint16 chId, uint8 slicSt);
int phHookStatus(phoneCh_t *phoneCB);
static int phCallWaittingToneCallBack( tm_id_t tmId, void*param);
int phStartCptTone(phoneCh_t*phoneCB, uint8 toneType);
int phStartRingingPulseAlertingSignal(phoneCh_t*phoneCB);
int phPlayDTAS(tm_id_t timer_id,void *arg);
int phWaitPhoneACKTimeout(tm_id_t,void*);

#if (defined FXO_DSPRELAY) || (defined FXO_LIFELINE)
void fxoOffHook(phoneCh_t*phoneCB);
void fxoOnHook(phoneCh_t*phoneCB);
void fxoGenDtmfTone(uint16 chId, char dialDigit);
void fxoLifeLineRelay(uint16 chId, int	enable);
void fxoOffHookDetect(phoneCh_t*phoneCB);
void fxoOnHookDetect(phoneCh_t*phoneCB);
void fxoDisableHookDetect(phoneCh_t*phoneCB);
void fxoCheckLinkStatus(phoneCh_t*phoneCB);
void swapTimeSlot(uint16 chId1, uint16 chId2, uint16 type);
#endif
#ifdef FXO_DSPRELAY
void fxoLifeLineRelay(uint16 chId, int	enable);
#endif
#endif

uint16 map_chId_to_timeslot(uint16 chId);
int phStartDtmfTone(uint16 chId, char dialDigit, uint16 onTime);
int phStopDtmfTone(uint16 chId);

#ifdef LIFELINE_AUTODETECT
int lifelineExistCheck(void);
int iflifelineExist(void);
#endif
