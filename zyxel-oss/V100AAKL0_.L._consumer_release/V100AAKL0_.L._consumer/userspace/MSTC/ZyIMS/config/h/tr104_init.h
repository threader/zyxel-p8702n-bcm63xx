/*
   $Id: tr104Init.h 0.00.0.1 2008/11/12 14:51 ctpan Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 ZyXEL Communications, Corp.
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
 * $Log: tr104Init.h $
*/

#ifndef _TR104INIT_H_INCLUDE
#define	_TR104INIT_H_INCLUDE

#include "tr104_object.h"

/*____________________________________________________________________________*/
/*	structure                                                                                                 		   */
/*____________________________________________________________________________*/

#define	VOICESERVICEINIT  	1
#define	VOICECAPINIT		2
#define	VOICEPROFILEINIT	3
#define	VOICELINEINIT		4
#define	VOICECOMMONINIT	5
#define	VOICEPHYINTFMGNINIT 	6
#define	VOICEPSTNINIT			7
#define	VOICEPHOONBOOKINIT	8

#define 	DEFAULTINITSTEP	8

typedef struct {
    int    record[DEFAULTINITSTEP];
} tr104InitStepRec_t;


/*____________________________________________________________________________*/
/*	function                                                                                                 		   */
/*____________________________________________________________________________*/


int tr104ModuleInit(void);

int  tr104VoiceCapObjChildInit(void);
//Ssu-Ying, support Multiple Profile
int tr104VoiceProfObjChildInit( int NumOfProfile );
int tr104VoiceProfLineObjChildInit(int VpItr , int NumOfLine);
int tr104VoicePhyIntfChildInit(int phyIntfIdx);
int tr104VoiceFxoChildInit(fxoIdx);
int tr104VoiceSipAccountInit(SipAccountId);


VoiceProfButtonMapObj_t* tr104VoiceProfButtonMapObjInit(void);
VoiceProfRtpObj_t* tr104VoiceProfRtpObjInit(void);
VoiceProfNumPlanObj_t * tr104VoiceProfNumPlanObjInit(void);
VoiceProfSipObj_t * tr104VoiceProfSipObjInit(void);
VoiceProfToneObj_t* tr104VoiceProfToneObjInit(void);
VoiceProfLineSipObj_t* tr104VoiceProfLineSipObjInit( void);
VoiceProfLineCodecObj_t* tr104VoiceProfLineCodecObjInit(void);
VoiceProfLineRingerObj_t* tr104VoiceProfLineRingerObjInit(void);
VoiceProfLineCallingFeaturesObj_t* tr104VoiceProfLineCallingFeaturesObjInit(void);

int tr104VoiceServiceObjLoadConfig(void);
int tr104VoiceCommonLoadConfig(void);
int tr104VoiceCapObjLoadConfig(void);
int tr104VoiceCapCodecsTabLoadConfig(void);

//Ssu-Ying, support Multiple Profile
int tr104VoiceProfileObjLoadConfig(int VpItr);
int tr104VoiceProfSipObjLoadConfig(int VpItr,VoiceProfSipObj_t*  pProfSipObj);
int tr104VoiceProfSipSubTabLoadConfig(int VpItr);
int tr104VoiceProfSipRespMapTabLoadConfig(int VpItr);

int tr104VoiceProfRtpObjLoadConfig(int VpItr ,VoiceProfRtpObj_t*  pProfRtp);

int tr104VoiceProfNumPlanObjLoadConfig( int VpItr ,VoiceProfNumPlanObj_t* ProfNumPlan);
int tr104VoiceProfNumPlanPrefixTabLoadConfig(int VpItr);

int tr104VoiceProfToneObjLoadConfig( int VpItr  ,VoiceProfToneObj_t*	pProfTone);
int tr104VoiceProfToneEventTabLoadConfig(int VpItr);
int tr104VoiceProfToneDescriptionTabLoadConfig(int VpItr);
int tr104VoiceProfTonePatternTabLoadConfig(int VpItr);
int tr104VoiceProfLineObjLoadConfig(int VpItr , int LineItr);
int tr104VoiceLineSessionLstLoadConfig(int VpItr , int LineItr);
int tr104VoiceLineSipObjLoadConfig (int VpItr , int LineItr,VoiceProfLineSipObj_t*  obj);
int tr104VoiceLineSipEventSubTabLoadConfig(int VpItr , int LineItr);

int tr104VoiceLineRingerObjLoadConfig(int VpItr , int LineItr,VoiceProfLineRingerObj_t*  obj);
int tr104VoiceLineRingerEventTabLoadConfig(int VpItr , int LineItr);
int tr104VoiceLineRingerDescriptionTabLoadConfig(int VpItr , int LineItr);
int tr104VoiceLineRingerPatternTabLoadConfig(int VpItr , int LineItr);

int tr104VoiceLineCodecObjLoadConfig (int VpItr , int LineItr,VoiceProfLineCodecObj_t*  obj);
int tr104VoiceLineCodecsTabLoadConfig(int VpItr , int LineItr);

int tr104VoiceFxoLoadConfig(int fxoIdx);
int tr104VoicePhyIntfLoadConfig(int phyIntfIdx);
int tr104VoicePSTNLoadConfig(void);

void tr104ReloadAll(void);
void tr104DumpAll(void);


/******************************************************************************
**	Free Function of TR104
**
******************************************************************************/
void tr104ModuleFree(void);
static void tr104VoiceCapObjChildFree(void);
//Ssu-Ying, support Multiple Profile
static void tr104VoiceProfObjChildFree(int VpItr);
static void tr104VoiceProfButtonMapObjFree(int VpItr);
static void tr104VoiceProfRtpObjFree(int VpItr);
static void tr104VoiceProfNumPlanObjFree(int VpItr);
static void tr104VoiceProfSipObjFree(int VpItr);
static void tr104VoiceProfToneObjFree(int VpItr);

static void tr104VoiceProfLineObjChildFree(int VpItr, int LineItr);
static void tr104VoiceProfLineCodecObjFree(int VpItr, int LineItr);
static void tr104VoiceProfLineSipObjFree(int VpItr, int LineItr);
static void tr104VoiceProfLineRingerObjFree(int VpItr, int LineItr);
static void tr104VoiceProfLineCallingFeaturesObjFree(int VpItr, int LineItr);
static void tr104VoicePhyIntfChildFree(PhyIdx);
static void tr104VoiceFxoChildFree(FxoIdx);
static void tr104VoicePhyIntfMgnChildFree(void);

#endif

