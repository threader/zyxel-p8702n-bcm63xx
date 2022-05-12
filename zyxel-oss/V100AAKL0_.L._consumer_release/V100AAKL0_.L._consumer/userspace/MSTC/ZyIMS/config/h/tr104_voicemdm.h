/*
   $Id: tr104_voicemdm.h 0.00.0.1 2008/11/12 14:51 ctpan Exp $
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
 * $Log: tr104_voicemdm.h $
*/

#ifndef _TR104VOICEMDM_H_INCLUDE
#define	_TR104VOICEMDM_H_INCLUDE

#if 1 /* Broadcom CMS */
#include <cms_obj.h>
#endif

/*___________________________________________________________
	value validation
___________________________________________________________*/

/*VoiceProfile.{i}.SIP*/
#define  INVITEEXPIRES_MIN_VALUE  1 /* in second*/
#define  ZyXEL_MINSE_MIN_VALUE 1 /* in second*/
/*VoiceProfile.{i}.RTP*/
#define  RTP_LOCALPORTMIN_MIN_VALUE 0
#define  RTP_LOCALPORTMIN_MAX_VALUE 65535
/*VoiceProfile.{i}.NumberingPlan*/
#define	INTERDIGITTIMERSTD_MIN_VALUE 1
#define	INTERDIGITTIMERSTD_MAX_VALUE 50000 /*in ms*/
/*___________________________________________________________
	Basic Default value
___________________________________________________________*/
/*VoiceProfile.{i}*/
#define USERAGENTPORT_BASIC_VALUE 5060 

/*VoiceProfile.{i}.SIP*/
#define  INVITEEXPIRES_BASIC_VALUE 	180  /* in second*/
#define  ZyXEL_MINSE_BASIC_VALUE 120 /* in second*/

/*VoiceProfile.{i}.RTP*/
#define RTP_LOCALPORTMIN_BASIC_VALUE	50000
#define RTP_LOCALPORTMAX_BASIC_VALUE	65535

/*VoiceProfile.{i}.NumberingPlan*/
#define	INTERDIGITTIMERSTD_BASIC_VALUE 3000 /*in ms*/
#define ZyXEL_DIALINGLONGINTERVAL_BASIC_VALUE 10  /*In second*/

/*VoiceProfile.{i}.Line.{i}*/
#define ZyXEL_PHYREFERENCEFXSLIST_BASIC_VALUE   0x01 /*default the 1st FXS -- 1st line*/
	
/*VoiceProfile.{i}.X_ZYXEL_COM_PhyIntfMgn.PhyInterface.{i}.X_ZYXEL_COM_FXS.{i}*/
#define FXS_LINESELECT_BASIC_VALUE 0x01 /* default select the first Line*/
#define HOWLERTONETIMEOUT_BASIC_VALUE  30  /*in second*/

/*VoiceProfile.{i}.X_ZYXEL_COM_PhyIntfMgn.PhyInterface.{i}.X_ZYXEL_COM_FXO.{i}*/
#define FXOPHONESELECT_BASIC_VALUE 0x01


/*___________________________________________________________
	Defines and Data structures
___________________________________________________________*/

#define _VOICE_MAX_VOIP_ARGS 6             /* Maximum number of voip arguments */
#define _VOICE_MAXLEVELS 10	             /* Maximum levels allowed in TR-104 tree */
#define _VOICE_ANY_INSTANCE             (-1)


typedef struct
{
   MdmObjectId id;                   /* MDM Object ID */
   int instId;                       /* Instance ID */
   InstanceIdStack iidStack;         /* Instance ID Stack */
}levelInfo_t; 

/* Argument block to be passed to DAL functions from provisioning interface */
typedef struct {
    int    op[_VOICE_MAX_VOIP_ARGS];
} voiceParams_t;


/** Return the topmost instance id from the stack, but does not modify stack. */
#define _PEEK_INSTANCE_ID(s) \
   ((s)->currentDepth > 0) ? (s)->instance[(s)->currentDepth - 1] : 0

/* Common helper functions */
CmsRet getMdmObj (levelInfo_t info[], InstanceIdStack *iidStack, void **obj);
void displayIIDStack(InstanceIdStack *iidStack);
/*__________________________________________________________________________*/
/*	function                                                                                                 		*/
/*__________________________________________________________________________*/

CmsRet tr104LoadMdmVoiceCfg( VoiceServiceObj_t * VoiceServiceObj);

CmsRet tr104LoadMdmVoiceCommCfg( ZyXEL_VoiceCommon_t * tr104VoiceComm);

CmsRet tr104LoadMdmVoiceCapCfg(VoiceCapObj_t * tr104VoCapObj);

CmsRet tr104LoadMdmVoiceCapSipCfg(VoiceCapSipObj_t * tr104VoCapSipObj);

CmsRet tr104LoadMdmVoiceCapCodecsCfg(voiceParams_t *parms  , VoiceCapCodecObj_t * tr104VoCapCodecTab);

CmsRet tr104LoadMdmVoiceProfCfg(voiceParams_t *parms  , VoiceProfObj_t * tr104VoProf);

CmsRet tr104LoadMdmServiceProviderInfoCfg(voiceParams_t *parms  , VoiceProfServiceProviderInfoObj_t * ServiceProvider);

CmsRet tr104LoadMdmVoiceProfSipCfg(voiceParams_t *parms  , VoiceProfSipObj_t * voiceProfSip);

CmsRet tr104LoadMdmVoiceProfSipSubCfg(voiceParams_t *parms , VoiceProfSipEventSubObj_t * voiceProfSipSub);

CmsRet tr104LoadMdmVoiceProfSipRespCfg(voiceParams_t *parms , VoiceProfSipRespMapObj_t * voiceProfSipResp);

CmsRet tr104LoadMdmVoiceProfRtpCfg(voiceParams_t *parms  , VoiceProfRtpObj_t * RTPProf);

CmsRet tr104LoadMdmVoiceProfRtpRtcpCfg( voiceParams_t *parms , VoiceProfRtpRtcpObj_t * RTCPProf);

CmsRet tr104LoadMdmVoiceProfRtpSrtpCfg( voiceParams_t *parms , VoiceProfRtpSrtpObj_t * SRTPProf);

CmsRet tr104LoadMdmVoiceProfRtpRedundancyCfg( voiceParams_t *parms , VoiceProfRtpRedundancyObj_t * ProfRtpRedundancyObj);

CmsRet tr104LoadMdmVoiceProfNumberingPlanCfg( voiceParams_t *parms , VoiceProfNumPlanObj_t * NumberingPlanObj);

CmsRet tr104LoadMdmVoiceProfNumPlanPrefixCfg( voiceParams_t *parms , VoiceProfNumPlanPrefixInfoObj_t * prefixInfoObj );

CmsRet tr104LoadMdmVoiceProfToneCfg( voiceParams_t *parms ,VoiceProfToneObj_t * profToneObj);

CmsRet tr104LoadMdmVoiceProfToneEventCfg( voiceParams_t *parms ,VoiceProfToneEventObj_t * toneEventObj);

CmsRet tr104LoadMdmVoiceProfToneDescriptionCfg( voiceParams_t *parms ,VoiceProfToneDescriptionObj_t * toneDescriptionObj);

CmsRet tr104LoadMdmVoiceProfTonePatternCfg( voiceParams_t *parms ,VoiceProfTonePatternObj_t * tonePatternObj);

CmsRet tr104LoadMdmVoiceProfFaxT38Cfg(voiceParams_t *parms  , VoiceProfFaxT38Obj_t * FaxT38Prof);

CmsRet tr104LoadMdmVoiceLineCfg( voiceParams_t *parms  ,VoiceProfLineObj_t * LineProf);

CmsRet tr104LoadMdmVoiceLineSipCfg( voiceParams_t *parms  ,VoiceProfLineSipObj_t * LineProf);

CmsRet tr104LoadMdmVoiceLineRingerCfg( voiceParams_t *parms ,VoiceProfLineRingerObj_t * lineRingerObj);

CmsRet tr104LoadMdmVoiceLineRingerEventCfg( voiceParams_t *parms ,VoiceProfLineRingerEventObj_t *ringerEventObj);

CmsRet tr104LoadMdmVoiceLineRingerDescriptionCfg( voiceParams_t *parms ,VoiceProfLineRingerDescriptionObj_t * ringerDescriptionObj);

CmsRet tr104LoadMdmVoiceLineRingerPatternCfg( voiceParams_t *parms ,VoiceProfLineRingerPatternObj_t * ringerPatternObj);
	
CmsRet tr104LoadMdmVoiceLineCallingFeaturesCfg( voiceParams_t *parms  ,VoiceProfLineCallingFeaturesObj_t * lineCallingFeaturesObj);

CmsRet tr104LoadMdmVoiceLineProcessingCfg( voiceParams_t *parms  ,VoiceProfLineVoiceProcessingObj_t * voiceProcessingObj);

CmsRet tr104LoadMdmVoiceLineCodecCfg( voiceParams_t *parms  ,VoiceProfLineCodecObj_t * lineCodecObj);

CmsRet tr104LoadMdmVoiceLineCodecListCfg( voiceParams_t *parms  ,VoiceProfLineCodecListObj_t * lineCodecListObj);

CmsRet tr104LoadMdmVoiceLineSessionCfg( voiceParams_t *parms  ,VoiceProfLineSessionObj_t * lineSessionObj);
#if 0
CmsRet tr104LoadMdmVoiceLineStatsCfg( voiceParams_t *parms  ,VoiceProfLineStatsObj_t * lineStatsObj);
#endif

//Ssu-Ying, support Multiple Profile
#if 0
CmsRet tr104LoadMdmVoicePhyIntfMgnCfg( ZyXEL_VoicePhyIntfMgn_t * tr104Obj);

CmsRet tr104LoadMdmVoicePhyIntfCfg( voiceParams_t *parms  ,VoicePhyIntfObj_t * tr104Obj);

CmsRet tr104LoadMdmVoicePhyIntfFXSCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXS_t * tr104Obj);

CmsRet tr104LoadMdmVoicePhyIntfFXOCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXO_t * tr104Obj);
#endif

CmsRet tr104LoadMdmVoicePhyIntfCfg( voiceParams_t *parms  ,VoicePhyIntfObj_t * tr104Obj);

CmsRet tr104LoadMdmVoiceFXOCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXO_t * tr104Obj);

CmsRet tr104LoadMdmVoiceFXOPortCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXOPort_t * tr104Obj) ; 

CmsRet tr104LoadMdmVoiceZYXELPstnCfg(voiceParams_t *parms  , ZyXEL_VoicePSTN_t * tr104Obj);

CmsRet tr104LoadMdmVoicePhoneBookCfg( voiceParams_t *parms ,ZyXEL_VoicePhoneBook_t * tr104Obj);

int tr104ResetVoiceLineStats( voiceParams_t *params );

int tr104SetMdmVoiceLineStats(voiceParams_t *params  ,_VoiceLineStatsObject * mdmLineStatsObj);


/********************************************************************
**  
**
**  API Class : Validation APIs 
**  descriptions:Area of the Validation APIs of TR104 Module
**
**
*********************************************************************/

int tr104ValidationRtpPortRange(uint16 portMin ,uint16 portMax);

#endif

