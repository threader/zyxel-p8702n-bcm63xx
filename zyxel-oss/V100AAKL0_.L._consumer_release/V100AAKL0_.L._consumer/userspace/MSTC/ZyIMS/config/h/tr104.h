/*
   $Id: tr104.h 0.00.0.1 2008/10/21 11:21 ctpan Exp $
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
 * $Log: tr104.h $
*/

#ifndef _TR104_H_INCLUDE
#define _TR104_H_INCLUDE

#include "tr104_object.h"


extern uint8 Tr104Debug ;

/*____________________________________________________________________________*/
/*	Debug Print                                                                                                                 */
/*____________________________________________________________________________*/

#define TR104_DBG(fs)					if (Tr104Debug & 1)	{	ZyPrintf(fs);			\
																	ZyPrintf("\n\r"); }

#define TR104_DBG1(fs, v1)				if (Tr104Debug & 1)	{	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf("\n\r"); }

#define TR104_DBG2(fs, v1, f2, v2)		if (Tr104Debug & 1) {	ZyPrintf(fs);	\
																ZyPrintf("0x%x", v1); \
																ZyPrintf(f2); \
																ZyPrintf("0x%x", v2); \
																ZyPrintf("\n\r"); }

#define TR104_DBGS1(fs, v1)				if (Tr104Debug & 1)	{	ZyPrintf(fs);	\
																ZyPrintf("%s", v1); \
																ZyPrintf("\n\r"); }

#define TR104_PRINTF	if (Tr104Debug & 1)	ZyPrintf

/*Index check Function*/
//Ssu-Ying, support Multiple Profile
int tr104ProfileIndexCheck(int VpItr);
int tr104LineIndexCheck(int VpItr, int LineItr);

/*Lock Function for Protecting the line statistic object*/

int tr104AcquireStatsLock(void);
void tr104ReleaseStatsLock(int x);

/* the Object list system of TR104 , only support the objects which have no child object */

tr104LstElm_t*	tr104LstElmNew(void);
void tr104LstElmFree(tr104LstElm_t* e);
tr104ObjLst_t* tr104ObjLstNew(void);
void tr104LstAllElmClear(tr104ObjLst_t* _this );
void tr104LstFree(tr104ObjLst_t* _this);
int tr104LstAddElmTail(tr104ObjLst_t* _this , void* value);
int tr104LstAddElmHead(tr104ObjLst_t* _this , void* value);
int   tr104LstInsert(tr104ObjLst_t* _this, void* value, int pos);
void*   tr104LstPeek(tr104ObjLst_t* _this , int pos);
int   tr104ObjLstGetSize(tr104ObjLst_t* _this);


VoiceServiceObj_t* tr104VoiceServiceObjPtrGet(void);
//Ssu-Ying, support Multiple Profile
ZyXEL_VoicePSTN_t* tr104VoicePSTNPtrGet(int PSTNIdx) ;
ZyXEL_VoicePhoneBook_t* tr104VoicePhoneBookPtrGet(int PhoneBookIdx) ;
ZyXEL_VoiceCommon_t* tr104VoiceCommonObjPtrGet(void);

/*capabilities*/
VoiceCapObj_t* tr104VoiceCapObjPtrGet(void) ;
VoiceCapSipObj_t* tr104VoiceCapSipObjPtrGet(void);
tr104ObjLst_t* tr104VoiceCapCodecsTabPtrGet(void);
VoiceCapCodecObj_t* tr104GetCapCodecByEntryID(uint32 entryId);

/*Profile*/
//Ssu-Ying, support Multiple Profile
VoiceProfObj_t* tr104VoiceProfObjPtrGet(int VpItr) ;
VoiceProfFaxT38Obj_t* tr104VoiceProfFaxT38ObjPtrGet(int VpItr);
VoiceProfSipObj_t* tr104VoiceProfSipObjPtrGet(int VpItr);
tr104ObjLst_t* tr104VoiceProfSipEventSubTabPtrGet(int VpItr);
VoiceProfSipEventSubObj_t* tr104GetProfSipEventSubObjByEventName(int VpItr , char* eventName) ;
tr104ObjLst_t* tr104VoiceProfSipRespMapTabPtrGet(int VpItr);
VoiceProfSipRespMapObj_t* tr104GetProfSipRespMapObjByRespNum(int VpItr ,  uint32 respNum);
VoiceProfToneObj_t* tr104VoiceProfToneObjPtrGet(int VpItr);
tr104ObjLst_t*  tr104VoiceProfToneEventTabPtrGet(int VpItr) ;
VoiceProfToneEventObj_t* tr104GetProfToneEventObjByFuncName( int VpItr , uint8 FuncName) ;
tr104ObjLst_t*  tr104VoiceProfToneDescriptionTabPtrGet(int VpItr) ;
tr104ObjLst_t*  tr104VoiceProfTonePatternTabPtrGet(int VpItr) ;
VoiceProfNumPlanObj_t* tr104VoiceProfNumPlanObjPtrGet(int VpItr);
tr104ObjLst_t* tr104VoiceProfNumPlanPrefixInfoTabPtrGet(int VpItr);

VoiceProfRtpObj_t *tr104VoiceProfRtpObjPtrGet (int VpItr);
VoiceProfRtpRtcpObj_t *tr104VoiceProfRtpRtcpObjPtrGet (int VpItr);
VoiceProfRtpSrtpObj_t *tr104VoiceProfRtpSrtpObjPtrGet (int VpItr);
VoiceProfRtpRedundancyObj_t *tr104VoiceProfRtpRedundancyObjPtrGet (int VpItr);
/*Line*/
VoiceProfLineObj_t* tr104VoiceLineObjPtrGet(int VpItr, int LineItr) ;
VoiceProfLineSipObj_t* tr104VoiceLineSipObjPtrGet(int VpItr, int LineItr) ;
tr104ObjLst_t*  tr104VoiceLineSipEventSubTabPtrGet(int VpItr, int LineItr);
VoiceProfLineVoiceProcessingObj_t* tr104VoiceLineProcessingObjPtrGet(int VpItr, int LineItr);
VoiceProfLineStatsObj_t* tr104VoiceLineStatsObjPtrGet(int VpItr, int LineItr) ;
VoiceProfLineCallingFeaturesObj_t* tr104VoiceLineCallingFeatureObjPtrGet(int VpItr, int LineItr) ;
int tr104VoiceLineCallingFeatureObjPtrSet(int VpItr, int LineItr ,	VoiceProfLineCallingFeaturesObj_t *pFeature );

VoiceProfLineRingerObj_t* tr104VoiceLineRingerObjPtrGet(int VpItr, int LineItr) ;
tr104ObjLst_t* tr104VoiceLineRingerEventTabPtrGet(int VpItr, int LineItr) ;
tr104ObjLst_t* tr104VoiceLineRingerDescriptionTabPtrGet(int VpItr, int LineItr) ;
tr104ObjLst_t* tr104VoiceLineRingerPatternTabPtrGet(int VpItr, int LineItr) ;
VoiceProfLineCodecObj_t* tr104VoiceLineCodecObjPtrGet(int VpItr, int LineItr) ;
tr104ObjLst_t* tr104VoiceLineCodecTabPtrGet(int VpItr, int LineItr) ;
VoiceProfLineCodecListObj_t* tr104GetLineCodecByEntryID(int VpItr, int LineItr , uint32 entryId) ;
VoiceProfLineCodecListObj_t* tr104GetLineCodecByCodecName(int VpItr, int LineItr , uint8* codec) ;
uint8 tr104GetLineCodecSilenceSuppression(int VpItr, int LineItr) ;

/*Phyicial Interface Manager*/
VoicePhyIntfObj_t* tr104VoicePhyIntfObjPtrGet(int phyIntfIdx) ;
ZyXEL_VoiceFXO_t* tr104VoiceFXOPtrGet(int fxoIndex); /*index of FXO*/
ZyXEL_VoiceFXOPort_t* tr104VoiceFXOPortPtrGet(int fxoIndex); /*index of FXO*/
ZyXEL_VoiceSipAccount_t* tr104VoiceSipAccountPtrGet(int SipAccountId);	



/********************************************************************
**  
**
**  APIs class: Parameter Getting APIs 
**  descriptions: the APIs to get the Parameter
**
**
*********************************************************************/

uint32 tr104GetNumProfile(void);
uint32 tr104GetMaxProfile(void);
uint32 tr104GetVPNumOfLine(int vpidx);
uint32 tr104GetMaxLine(void);
uint32 tr104GetMaxSessionsPerLine(void);
//uint8 tr104GetNumOfFXS(void);
uint8 tr104GetNumOfFXO(void);
uint32 tr104GetNumOfPhyIntf(void);

//Ssu-Ying, support Multiple Profile
int tr104LineSetProfSelect(int VpItr,int LineItr);


/********************************************************************
**  
**
**  API Class : Dump APIs 
**  descriptions:Area of the dump APIs of TR104 Module
**
**
*********************************************************************/


void tr104DumpVoiceServiceObj(void);
void tr104DumpVoiceCommonObj(void);
void tr104DumpVoiceCapObj(void);
void tr104DumpVoiceCapSipObj(void);
void tr104DumpVoiceCapCodecTab(void);

//Ssu-Ying, support Multiple Profile
void tr104DumpVoiceProfileObj(int VpItr);
void tr104DumpVoiceProfSipObj(int VpItr);

void tr104DumpVoiceProfNumPlanObj(int VpItr);
void tr104DumpVoiceProfNumPlanPrefixinfoTab(int VpItr);

void tr104DumpVoiceProfRTPObj(int VpItr);
void tr104DumpVoiceProfRtpRtcpObj(int VpItr);
void tr104DumpVoiceProfRtpSrtpObj(int VpItr);
void tr104DumpVoiceProfRtpRedundancyObj(int VpItr);

void tr104DumpVoiceProfFaxT38Obj(int VpItr);

void tr104DumpVoiceLineObj(int VpItr, int LineItr);
void tr104DumpVoiceLineSipObj(int VpItr, int LineItr);
void tr104DumpVoiceLineSipEventSubTab(int VpItr, int LineItr);
void tr104DumpVoiceLineRingerObj(int VpItr, int LineItr);
void tr104DumpVoiceLineRingerEventTab(int VpItr, int LineItr);
void tr104DumpVoiceLineRingerDesriptionTab(int VpItr, int LineItr);
void tr104DumpVoiceLineRingerPatternTab(int VpItr, int LineItr);

void tr104DumpVoiceLineCallingFeaturesObj(int VpItr, int LineItr);
void tr104DumpVoiceLineCodecObj(int VpItr, int LineItr);
void tr104DumpVoiceLineCodecList(int VpItr, int LineItr);
void tr104DumpVoiceLineProcessingObj(int VpItr, int LineItr);

void tr104DumpVoicePhyIntfObj(int phyIntfIdx);
void tr104DumpVoiceFxoObj(int fxoIdx);
void tr104DumpVoiceFxoPortObj(int fxoIdx);
void tr104DumpVoiceSipAccountObj(int SipAccountId);
void tr104DumpVoicePSTNObj(int PSTNIdx);
void tr104DumpVoicePhoneBookObj(int PhoneBookIdx);

int tr104FindLineIdFromRegisterUrl(char *regUrl, int typeUrl);

int mapSipAccountIdToVpItr(int SipAccountId);
int mapSipAccountIdToLineItr(int SipAccountId);
int mapVpItrLineItrToSipAccountId(int VpItr, int LineItr);

#endif

