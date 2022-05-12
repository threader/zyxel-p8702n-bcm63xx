/*
   $Id: TR104Init.c 0.00.0.1 2008/11/12 14:51 ctpan Exp $
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
 * $Log: TR104Init.c $
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cms.h>
#include <cms_util.h>
#include <cms_core.h>
#include <cms_obj.h>
#include <cms_mdm.h>
#include <cms_lck.h>
#include <cms_log.h>


#include "global.h"
#include "tr104_global.h"
#include "tr104.h"
#include "tr104_object.h"
#include "tr104_convert.h"
#include "tr104_init.h"
#include "tr104_voicemdm.h"
#include "switch.h"

#ifdef TERMINAL_PORTABILITY			
extern int isTerminalPortabilitySupport;
#endif
#if 1 /*Eason, Reinit dsp if country code changed*/
extern int dspReinitNeeded;
#endif

/***************************************************************************
** Function Name: getMdmObj
** Description  : Gets a specific instance of an object lacated at a specific
**                level in the TR-104 hierarchy.
**
** Parameters   : info (IN)      - Array that identifies objects and their instances
**                iidStack (OUT) - Instance ID stack
**                obj (OUT)      - The fetched object
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
CmsRet 
getMdmObj (levelInfo_t info[], InstanceIdStack *iidStack, void **obj)
{
	CmsRet ret = CMSRET_INTERNAL_ERROR;
	int i;
	unsigned int lineitr = 1,vpitr = 1;	
	/* Traverse until we reach the final level or run out of levels */
	for(i = 0; i < _VOICE_MAXLEVELS && info[i].id > 0; i++) {
		INIT_INSTANCE_ID_STACK(&info[i].iidStack);

		/* Traverse object instances */
//Ssu-Ying, support Multiple Profile
			while( (ret = (i == 0) ? cmsObj_getNext(info[i].id, &info[i].iidStack, obj) :cmsObj_getNextInSubTree(info[i].id, &info[i-1].iidStack, &info[i].iidStack, obj)) == CMSRET_SUCCESS ) {
			/* Found the specified instance or the first instance is instId = -1 */
			if(info[i].id == MDMOID_VOICE_PROF){
			      if (vpitr == info[i].instId)
			      {
			      		*iidStack = info[i].iidStack;
					break;
				}else{
					vpitr++;
				}
			}else if(info[i].id == MDMOID_VOICE_LINE){
			      if (lineitr == info[i].instId)
			      {
			      		*iidStack = info[i].iidStack;
					break;
				}else{
					lineitr++;
				}
			}else{
				if ( info[i].instId == -1 || (_PEEK_INSTANCE_ID(&info[i].iidStack)) == info[i].instId ) {
					*iidStack = info[i].iidStack;
					break;
				}
			}

			/* Free the object if it not the final object the we are interested in */
			cmsObj_free(obj);
		}

		/* No need to continue if we could not fetch an object from the current level */
		if ( ret != CMSRET_SUCCESS ) {
			return ret;
		}

		/* Free the object if it not the final object the we are interested in */
		if ( i != _VOICE_MAXLEVELS-1 && info[i+1].id != 0 ) {
			cmsObj_free(obj);
		}
	}

	return ret;
}
/***************************************************************************
** Function Name: displayIIDStack
**
** Description  : display the IIDStack
**                           
** Returns      : 
****************************************************************************/

void 
displayIIDStack(InstanceIdStack *iidStack)
{
	int i=0 ;
	
	if((Tr104Debug == 1 )&&(iidStack!=NULL)){
		ZyPrintf("\n ######################### \n");
		ZyPrintf("\n iidStack.currentDepth = %d \n",iidStack->currentDepth);
		for(i=0;i<iidStack->currentDepth;i++)
		{
			ZyPrintf("\n iidStack.instance[%d] = %d \n",i,iidStack->instance[i]);
		}
		ZyPrintf("\n ######################### \n");	
	}
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

/*Following functions are used to read CFG data from MDM*/
CmsRet 
tr104LoadMdmVoiceCfg( VoiceServiceObj_t * VoiceServiceObj)
{
	CmsRet      ret;
	_VoiceObject*   mdm_VoiceServiceObj = NULL;
	InstanceIdStack   iidStack = EMPTY_INSTANCE_ID_STACK;

	/* Get InternetGatewayDevice.Services.VoiceService.{1}. */
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS){
		cmsLog_error( "Can't get lock \n" );
		return (ret);	
	} else {
		if (((ret = cmsObj_getNext(MDMOID_VOICE, &iidStack, (void **) &mdm_VoiceServiceObj)) != CMSRET_SUCCESS)||(VoiceServiceObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}

		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE");
		displayIIDStack(&iidStack);
		VoiceServiceObj->voiceProfileNumberOfEntries = mdm_VoiceServiceObj->voiceProfileNumberOfEntries ;
				
		 /* Free object */
		cmsObj_free((void **) &mdm_VoiceServiceObj);
			  
	}
	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceCommCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceCommCfg( ZyXEL_VoiceCommon_t * tr104VoiceComm)
{
	CmsRet              ret = CMSRET_OBJECT_NOT_FOUND;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	InstanceIdStack     iidStack1   = EMPTY_INSTANCE_ID_STACK;
	InstanceIdStack     iidStack2   = EMPTY_INSTANCE_ID_STACK;
	InstanceIdStack     iidStack3   = EMPTY_INSTANCE_ID_STACK;
	_VoiceObject*   mdm_VoiceServiceObj = NULL;
	_VoiceCommonObject    *mdmObj    = NULL;
   	_IGDDeviceInfoObject *deviceInfoObj = NULL;
	_VoiceProfObject * mdm_VoiceProfObj = NULL;

	if(tr104VoiceComm == NULL) {
		cmsLog_error( "tr104VoiceComm NULL!! \n" );
		return (ret);
	}

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLck_releaseLock();
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	}else{

   		if (cmsObj_get(MDMOID_IGD_DEVICE_INFO, &iidStack2, 0, (void **) &deviceInfoObj) != CMSRET_SUCCESS) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}

		cmsLck_releaseLock();
		
		if(deviceInfoObj->softwareVersion == NULL){
			TR104_DBG(" deviceInfoObj->softwareVersion NULL");
			zyStrcpy( tr104VoiceComm->softwareVersion, "" );
		}else{
			zyStrcpy(tr104VoiceComm->softwareVersion, deviceInfoObj->softwareVersion);
		}	

		cmsObj_free((void **) &deviceInfoObj);
	}
		
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLck_releaseLock();
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	}else{
	//Ssu-Ying, support Multiple Profile
		if ((ret = cmsObj_getNext(MDMOID_VOICE, &iidStack1, (void **) &mdm_VoiceServiceObj)) != CMSRET_SUCCESS) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}

	//Ssu-Ying, support Multiple Profile
		while ((ret = cmsObj_getNext(MDMOID_VOICE_PROF, &iidStack3, (void **) &mdm_VoiceProfObj)) == CMSRET_SUCCESS) {			
			cmsLck_releaseLock();
#ifdef SUPPORT_NEW_OUI
			if(mdm_VoiceProfObj->X_5067F0_ActiveBoundIfName == NULL){
				TR104_DBG(" mdm_VoiceProfObj->X_5067F0_ActiveBoundIfName NULL");
				zyStrcpy( tr104VoiceComm->ifName, "" );
			}else{
				zyStrcpy(tr104VoiceComm->ifName, mdm_VoiceProfObj->X_5067F0_ActiveBoundIfName);
			}
#else
			if(mdm_VoiceProfObj->X_ZYXEL_COM_ActiveBoundIfName == NULL){
				TR104_DBG(" mdm_VoiceProfObj->X_ZYXEL_COM_ActiveBoundIfName NULL");
				zyStrcpy( tr104VoiceComm->ifName, "" );
			}else{
				zyStrcpy(tr104VoiceComm->ifName, mdm_VoiceProfObj->X_ZYXEL_COM_ActiveBoundIfName);
			}
#endif	/* SUPPORT_NEW_OUI end */
			cmsObj_free((void **) &mdm_VoiceProfObj);
		}

#ifdef SUPPORT_NEW_OUI
		tr104VoiceComm->loggingLevel=tr104ConvertLoggingLevel(mdm_VoiceServiceObj->X_5067F0_LoggingLevel);
#else
		tr104VoiceComm->loggingLevel=tr104ConvertLoggingLevel(mdm_VoiceServiceObj->X_BROADCOM_COM_LoggingLevel);
#endif	/* SUPPORT_NEW_OUI end */
		cmsObj_free((void **) &mdm_VoiceServiceObj);
	}

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS)
	{
		cmsLck_releaseLock();
		cmsLog_error( "Can't get lock \n" );
		return (ret);	
	}else{

		if (((ret = cmsObj_getNext(MDMOID_VOICE_COMMON, &iidStack, (void **) &mdmObj)) != CMSRET_SUCCESS)||(tr104VoiceComm==NULL))
		{
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}
	
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_COMMON");
		displayIIDStack(&iidStack);
		tr104VoiceComm->voipPort = mdmObj->voipPort;
		tr104VoiceComm->ivrsyspermit = mdmObj->ivrsyspermit;
		tr104VoiceComm->ivrLanguage = mdmObj->ivrLanguage;	
		tr104VoiceComm->ivrcodec = mdmObj->ivrcodec;	
		
		tr104VoiceComm->specialFlag = mdmObj->specialFlag;	
		tr104VoiceComm->callFallBack = mdmObj->callFallBack;	
//Ssu-Ying, support Multiple Profile
#if 0
		tr104VoiceComm->activedialplan= mdmObj->activedialplan;
	
		if(mdmObj->dialPlan == NULL){
			TR104_DBG(" mdmObj->dialPlan is NULL");
			zyStrcpy( tr104VoiceComm->dial_plan,"");
		}else{
			/*256*/
			 tr104ConvertDialPlan(tr104VoiceComm->dial_plan , mdmObj->dialPlan ) ; 
		}
#endif	

		tr104VoiceComm->dialMethod = tr104ConvertDialMethod(mdmObj->dialMethod);	

#ifdef SUPPORT_NEW_OUI
		tr104VoiceComm->ZyXEL_pound = mdmObj->X_5067F0_Pound;
#else
		tr104VoiceComm->ZyXEL_pound = mdmObj->X_ZyXEL_COM_Pound;
#endif	/* SUPPORT_NEW_OUI end */
		tr104VoiceComm->voipIOPFlags = mdmObj->voipIOPFlags;

		cmsObj_free((void **) &mdmObj);	 
	}
	
	 return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceCapCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

/*Following functions are used to read CFG data from MDM*/
CmsRet
tr104LoadMdmVoiceCapCfg( VoiceCapObj_t * tr104VoCapObj)
{
	CmsRet      ret;
	_VoiceCapObject*   mdm_obj = NULL;
	InstanceIdStack   iidStack = EMPTY_INSTANCE_ID_STACK;

	/* Get InternetGatewayDevice.Services.VoiceService.{1}.Capabilities. */
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );
		return (ret);	
	} else {
		if (((ret = cmsObj_getNext(MDMOID_VOICE_CAP, &iidStack, (void **) &mdm_obj)) != CMSRET_SUCCESS)||(tr104VoCapObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_CAP");
		displayIIDStack(&iidStack);

		tr104VoCapObj->maxProfileCount = mdm_obj->maxProfileCount;
		tr104VoCapObj->maxLineCount = mdm_obj->maxLineCount;
		tr104VoCapObj->maxSessionsPerLine = mdm_obj->maxSessionsPerLine;
		tr104VoCapObj->maxSessionCount = mdm_obj->maxSessionCount;

		if(mdm_obj->signalingProtocols == NULL){
			TR104_DBG(" mdm_obj->signalingProtocols is NULL");
			zyStrcpy(tr104VoCapObj->signalingProtocols, "");
		} else {
			zyStrncpy(tr104VoCapObj->signalingProtocols, mdm_obj->signalingProtocols, min(257, zyStrlen(mdm_obj->signalingProtocols)+1)); //256+1 , 256
		}

		if(mdm_obj->regions == NULL){
			TR104_DBG(" mdm_obj->regions is NULL");
			zyStrcpy(tr104VoCapObj->regions, "" ); /*256+1 , 256*/
		} else {
			zyStrncpy(tr104VoCapObj->regions, mdm_obj->regions, min(257, zyStrlen(mdm_obj->regions)+1));  /*256+1 , 256*/
		}

		tr104VoCapObj->RTCP = mdm_obj->RTCP;
		tr104VoCapObj->SRTP = mdm_obj->SRTP;	

		if(mdm_obj->SRTPKeyingMethods == NULL){
			TR104_DBG(" mdm_obj->SRTPKeyingMethods is NULL");
			zyStrcpy(tr104VoCapObj->SRTPKeyingMethods,"");
		} else {
			zyStrncpy(tr104VoCapObj->SRTPKeyingMethods, mdm_obj->SRTPKeyingMethods, min(257, zyStrlen(mdm_obj->SRTPKeyingMethods)+1));
		}
		if(mdm_obj->SRTPEncryptionKeySizes == NULL){
			TR104_DBG(" mdm_obj->SRTPEncryptionKeySizes is NULL");
			zyStrcpy( tr104VoCapObj->SRTPEncryptionKeySizes,"");
		} else {
			zyStrncpy(tr104VoCapObj->SRTPEncryptionKeySizes, mdm_obj->SRTPEncryptionKeySizes, min(257, zyStrlen(mdm_obj->SRTPEncryptionKeySizes)+1)); 
		}

		tr104VoCapObj->RTPRedundancy = mdm_obj->RTPRedundancy;
		tr104VoCapObj->DSCPCoupled = mdm_obj->DSCPCoupled;
		tr104VoCapObj->ethernetTaggingCoupled = mdm_obj->ethernetTaggingCoupled;
		tr104VoCapObj->PSTNSoftSwitchOver=mdm_obj->PSTNSoftSwitchOver;
		tr104VoCapObj->faxT38 = mdm_obj->faxT38;
		tr104VoCapObj->faxPassThrough = mdm_obj->faxPassThrough;
		tr104VoCapObj->modemPassThrough = mdm_obj->modemPassThrough;
		tr104VoCapObj->toneGeneration = mdm_obj->toneGeneration;
#ifndef SUPPORT_412_VOICE /*Not Support*/
		tr104VoCapObj->toneDescriptionsEditable= mdm_obj->toneDescriptionsEditable;
		tr104VoCapObj->patternBasedToneGeneration= mdm_obj->patternBasedToneGeneration;
		tr104VoCapObj->fileBasedToneGeneration= mdm_obj->fileBasedToneGeneration;

		if(mdm_obj->toneFileFormats == NULL){
			TR104_DBG(" mdm_obj->toneFileFormats NULL");
			zyStrcpy( tr104VoCapObj->toneFileFormats,"");
		} else {
			zyStrncpy( tr104VoCapObj->toneFileFormats, mdm_obj->toneFileFormats, min(257, zyStrlen(mdm_obj->toneFileFormats)+1));
		}

		tr104VoCapObj->ringGeneration = mdm_obj->ringGeneration;
		tr104VoCapObj->ringDescriptionsEditable= mdm_obj->ringDescriptionsEditable;
		tr104VoCapObj->patternBasedRingGeneration= mdm_obj->patternBasedRingGeneration;
		tr104VoCapObj->ringPatternEditable= mdm_obj->ringPatternEditable;
		tr104VoCapObj->fileBasedRingGeneration= mdm_obj->fileBasedRingGeneration;

		if(mdm_obj->ringFileFormats == NULL){
			TR104_DBG(" mdm_obj->ringFileFormats NULL");
			zyStrcpy(tr104VoCapObj->ringFileFormats,"");
		} else {
			zyStrncpy(tr104VoCapObj->ringFileFormats, mdm_obj->ringFileFormats, min(257, strlen(mdm_obj->ringFileFormats)+1)); 
		}
#endif
		tr104VoCapObj->digitMap = mdm_obj->digitMap;
		tr104VoCapObj->numberingPlan = mdm_obj->numberingPlan;
		tr104VoCapObj->buttonMap = mdm_obj->buttonMap;
		tr104VoCapObj->voicePortTests = mdm_obj->voicePortTests;
#ifdef SUPPORT_NEW_OUI
		tr104VoCapObj->ZyXEL_pound=mdm_obj->X_5067F0_Pound;
#else
		tr104VoCapObj->ZyXEL_pound=mdm_obj->X_ZyXEL_COM_Pound;
#endif	/* SUPPORT_NEW_OUI end */

		/* Free object */
		cmsObj_free((void **) &mdm_obj);
	}
	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceCapSipCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet
tr104LoadMdmVoiceCapSipCfg(VoiceCapSipObj_t * tr104VoCapSipObj)
{
	CmsRet      ret;
	_VoiceCapSipObject*   mdm_obj = NULL;
	InstanceIdStack   iidStack = EMPTY_INSTANCE_ID_STACK;

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	} else {
		/* Get  VoiceCapSip Object */
		if (( (ret = cmsObj_getNext(MDMOID_VOICE_CAP_SIP, &iidStack, (void **) &mdm_obj)) != CMSRET_SUCCESS)||( tr104VoCapSipObj == NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );
			return (ret);
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_CAP_SIP");
		displayIIDStack(&iidStack);

		if(mdm_obj->role == NULL){
			TR104_DBG("mdm_obj->role NULL");
			zyStrcpy(tr104VoCapSipObj->role ,"") ;
		} else {
			zyStrncpy(tr104VoCapSipObj->role, mdm_obj->role, min(257, zyStrlen(mdm_obj->role)+1));
		}
		if(mdm_obj->extensions == NULL){
			TR104_DBG(" mdm_obj->extensions NULL");
			zyStrcpy(tr104VoCapSipObj->extensions ,"") ;
		} else {
			zyStrncpy(tr104VoCapSipObj->extensions, mdm_obj->extensions, min(257, zyStrlen(mdm_obj->extensions)+1)); 
		}
		if(mdm_obj->transports == NULL){
			TR104_DBG(" mdm_obj->transports NULL");
			zyStrcpy(tr104VoCapSipObj->transports ,"") ;
		} else {
			zyStrncpy(tr104VoCapSipObj->transports, mdm_obj->transports, min(257, zyStrlen(mdm_obj->transports)+1));
		}
		if(mdm_obj->URISchemes == NULL){
			TR104_DBG(" mdm_obj->URISchemes NULL");
			zyStrcpy(tr104VoCapSipObj->URISchemes ,"") ;
		} else {
			zyStrncpy( tr104VoCapSipObj->URISchemes, mdm_obj->URISchemes, min(257, zyStrlen(mdm_obj->URISchemes)+1));
		}

		tr104VoCapSipObj->eventSubscription = mdm_obj->eventSubscription;
		tr104VoCapSipObj->responseMap = mdm_obj->responseMap;
#ifndef SUPPORT_412_VOICE /*Not Support*/
		if(mdm_obj->TLSAuthenticationProtocols == NULL){
			TR104_DBG(" mdm_obj->TLSAuthenticationProtocols NULL");
			zyStrcpy(tr104VoCapSipObj->TLSAuthenticationProtocols ,"");
		} else {
			zyStrncpy(tr104VoCapSipObj->TLSAuthenticationProtocols, mdm_obj->TLSAuthenticationProtocols, min(257, zyStrlen(mdm_obj->TLSAuthenticationProtocols)+1));
		}

		if(mdm_obj->TLSAuthenticationKeySizes == NULL){
			TR104_DBG(" mdm_obj->TLSAuthenticationKeySizes NULL");
			zyStrcpy(tr104VoCapSipObj->TLSAuthenticationKeySizes ,"");
		} else {
			zyStrncpy(tr104VoCapSipObj->TLSAuthenticationKeySizes, mdm_obj->TLSAuthenticationKeySizes, min(257, zyStrlen(mdm_obj->TLSAuthenticationKeySizes)+1));
		}

		if(mdm_obj->TLSEncryptionProtocols == NULL){
			TR104_DBG(" mdm_obj->TLSEncryptionProtocols NULL");
			zyStrcpy(tr104VoCapSipObj->TLSEncryptionProtocols ,"");
		} else {
			zyStrncpy(tr104VoCapSipObj->TLSEncryptionProtocols, mdm_obj->TLSEncryptionProtocols, min(257, zyStrlen(mdm_obj->TLSEncryptionProtocols)+1));
		}

		if(mdm_obj->TLSEncryptionKeySizes == NULL){
			TR104_DBG(" mdm_obj->TLSEncryptionKeySizes NULL");
			zyStrcpy(tr104VoCapSipObj->TLSEncryptionKeySizes ,"");
		} else {
			zyStrncpy(tr104VoCapSipObj->TLSEncryptionKeySizes, mdm_obj->TLSEncryptionKeySizes, min(257, zyStrlen(mdm_obj->TLSEncryptionKeySizes)+1));
		}
		if(mdm_obj->TLSKeyExchangeProtocols == NULL){
			TR104_DBG(" mdm_obj->TLSKeyExchangeProtocols NULL");
			zyStrcpy(tr104VoCapSipObj->TLSKeyExchangeProtocols ,"");
		} else {
			zyStrncpy(tr104VoCapSipObj->TLSKeyExchangeProtocols, mdm_obj->TLSKeyExchangeProtocols, min(257, zyStrlen(mdm_obj->TLSKeyExchangeProtocols)+1));
		}
#endif
		/* Free object */
		cmsObj_free((void **) &mdm_obj);
	}
	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceCapCodecsCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

/*! \brief InternetGatewayDevice.Services.VoiceService.{i}.Capabilities.Codecs.{i}. */
CmsRet 
tr104LoadMdmVoiceCapCodecsCfg( voiceParams_t *parms ,VoiceCapCodecObj_t * tr104VoCapCodec)
{
	CmsRet      ret;
	unsigned int    codecInst   = parms->op[0];
	_VoiceCapCodecsObject*   mdm_CapCodecObj = NULL;
	InstanceIdStack   iidStack = EMPTY_INSTANCE_ID_STACK;
	levelInfo_t   info[_VOICE_MAXLEVELS]    = {  {MDMOID_VOICE_CAP_CODECS, codecInst, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	} else {
		if ((( ret = getMdmObj(info, &iidStack, (void**)&mdm_CapCodecObj)) != CMSRET_SUCCESS)||( tr104VoCapCodec == NULL)) {
			cmsLck_releaseLock();
			/*cmsLog_error( "Can't retrieve object\n" );*/
			return (ret);
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_CAP_CODECS");
		displayIIDStack(&iidStack);
		tr104VoCapCodec->entryID =tr104ConvertCodecEntryID(mdm_CapCodecObj->entryID);

		tr104VoCapCodec->bitRate =mdm_CapCodecObj->bitRate;
		tr104VoCapCodec->silenceSuppression =mdm_CapCodecObj->silenceSuppression;

		if(mdm_CapCodecObj->packetizationPeriod == NULL){
			TR104_DBG(" mdm_CapCodecObj->packetizationPeriod  is NULL");
			zyStrcpy(tr104VoCapCodec->packetizationPeriod,"");
		} else {
			zyStrncpy(tr104VoCapCodec->packetizationPeriod ,mdm_CapCodecObj->packetizationPeriod,  min(65, zyStrlen(mdm_CapCodecObj->packetizationPeriod)+1));
		}
							
		if(mdm_CapCodecObj->codec == NULL){
			TR104_DBG(" mdm_CapCodecObj->codec  is NULL");
			zyStrcpy(tr104VoCapCodec->codecName,"");
		} else {
			zyStrncpy(tr104VoCapCodec->codecName ,mdm_CapCodecObj->codec,  min(65, zyStrlen(mdm_CapCodecObj->codec)+1));
		}

		/* Free object */
		cmsObj_free((void **) &mdm_CapCodecObj);
	}	
	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfCfg( voiceParams_t *parms ,VoiceProfObj_t * tr104VoProf)
{
//Ssu-Ying, support Multiple Profile
unsigned int    VpItr   = parms->op[0] ;
	CmsRet              ret;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfObject    *mdmProfObj    = NULL;
	levelInfo_t   info[_VOICE_MAXLEVELS]    = {{MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };
		
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	} else {
		/* Get voiceProfile object */ 
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdmProfObj)) != CMSRET_SUCCESS )||(tr104VoProf == NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );
			return (ret);
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_PROF");
		displayIIDStack(&iidStack);

#ifdef SUPPORT_NEW_OUI
		tr104VoProf->ZyXEL_ProfObjID = mdmProfObj->X_5067F0_ProfObjID ; 
#else
		tr104VoProf->ZyXEL_ProfObjID = mdmProfObj->X_ZyXEL_COM_ProfObjID ; 
#endif	/* SUPPORT_NEW_OUI end */

	   	if(mdmProfObj->enable == NULL){
			TR104_DBG(" mdmProfObj->enable  is NULL");
		} else {
			tr104VoProf->enable = tr104ConvertEnable(mdmProfObj->enable);
		}

		tr104VoProf->reset = mdmProfObj->reset;
		tr104VoProf->numberOfLines =mdmProfObj->numberOfLines;
			
	   	if(mdmProfObj->name == NULL){
			TR104_DBG(" mdmProfObj->name  is NULL");
			zyStrcpy(tr104VoProf->name,"");
		} else {
			zyStrncpy(tr104VoProf->name, mdmProfObj->name, min(65, zyStrlen(mdmProfObj->name)+1));
		}

	   	if(mdmProfObj->signalingProtocol == NULL){
			TR104_DBG(" mdmProfObj->signalingProtocol  is NULL");
			zyStrcpy(tr104VoProf->signalingProtocol ,"");
		} else {
			zyStrncpy(tr104VoProf->signalingProtocol, mdmProfObj->signalingProtocol, min(65, zyStrlen(mdmProfObj->signalingProtocol)+1));
		}

		tr104VoProf->maxSessions = mdmProfObj->maxSessions;
		tr104VoProf->DTMFMethod = tr104ConvertDTMFMethod(mdmProfObj->DTMFMethod);
		tr104VoProf->DTMFMethodG711 = tr104ConvertDTMFMethod(mdmProfObj->DTMFMethodG711);

	   	if(mdmProfObj->region == NULL){
			TR104_DBG(" mdmProfObj->region  is NULL");
			zyStrcpy(tr104VoProf->region, "");
		} else {
#ifdef SUPPORT_NEW_OUI
			if(mdmProfObj->X_5067F0_SPNum == 0) {
#else
			if(mdmProfObj->X_BROADCOM_COM_SPNum == 0) {
#endif	/* SUPPORT_NEW_OUI end */
			#if 1 /*Eason, deinit dsp if country code changed*/
				if((NULL != tr104VoProf->region) && (NULL != mdmProfObj->region) && zyStrcmp(tr104VoProf->region,mdmProfObj->region)!=0){
					ZyPrintf("dspReinitNeeded, %s > %s\n", tr104VoProf->region, mdmProfObj->region);
					dspReinitNeeded = 1;
				}			
			#endif
			}
			zyStrncpy(tr104VoProf->region, mdmProfObj->region, min(3, zyStrlen(mdmProfObj->region)+1));
			#ifdef TERMINAL_PORTABILITY			
			if(!strncmp(tr104VoProf->region, "DK", 2)){ /*DENMARK*/
				isTerminalPortabilitySupport = 1;
				TR104_DBG("TerminalPortability Supportted\n");
			}
			else{
				isTerminalPortabilitySupport = 0;
			}
			#endif
		}

#ifdef SUPPORT_NEW_OUI
		tr104VoProf->ZyXEL_countrycode = mdmProfObj->X_5067F0_countrycode;
#else
		tr104VoProf->ZyXEL_countrycode = mdmProfObj->X_ZyXEL_COM_countrycode;
#endif	/* SUPPORT_NEW_OUI end */

		if(mdmProfObj->digitMap == NULL){
			TR104_DBG(" mdmProfObj->digitMap  is NULL");
			zyStrcpy(tr104VoProf->digitMap, "");
		} else {
			zyStrncpy(tr104VoProf->digitMap, mdmProfObj->digitMap, min(257, zyStrlen(mdmProfObj->digitMap)+1));
		}

		tr104VoProf->digitMapEnable=mdmProfObj->digitMapEnable;
		tr104VoProf->STUNEnable=mdmProfObj->STUNEnable;

		if(mdmProfObj->STUNServer == NULL){
			TR104_DBG(" mdmProfObj->STUNServer  is NULL");
			zyStrcpy(tr104VoProf->STUNServer, "");
		} else {
			zyStrncpy(tr104VoProf->STUNServer, mdmProfObj->STUNServer, min(257, zyStrlen(mdmProfObj->STUNServer)+1));
		}

#ifdef SUPPORT_NEW_OUI
		tr104VoProf->ZyXEL_SPNum = mdmProfObj->X_5067F0_SPNum;
		tr104VoProf->ZyXEL_stunServPort = mdmProfObj->X_5067F0_STUNServerPort ;
#else
		tr104VoProf->ZyXEL_SPNum = mdmProfObj->X_BROADCOM_COM_SPNum;
		tr104VoProf->ZyXEL_stunServPort = mdmProfObj->X_BROADCOM_COM_STUNServerPort ;
#endif	/* SUPPORT_NEW_OUI end */
#ifndef SUPPORT_412_VOICE /*Not Support*/
		tr104VoProf->nonVoiceBandwidthReservedUpstream=mdmProfObj->nonVoiceBandwidthReservedUpstream;
		tr104VoProf->nonVoiceBandwidthReservedDownstream=mdmProfObj->nonVoiceBandwidthReservedDownstream;
		tr104VoProf->PSTNFailOver=mdmProfObj->PSTNFailOver; 		
#endif		
		tr104VoProf->faxPassThrough=tr104ConvertPassthrough(mdmProfObj->faxPassThrough);
#ifdef SUPPORT_NEW_OUI
		tr104VoProf->ZyXEL_faxPassThroughPCMA = mdmProfObj->X_5067F0_FaxPassthroughPCMA;
#else
		tr104VoProf->ZyXEL_faxPassThroughPCMA = mdmProfObj->X_ZYXEL_COM_FaxPassthroughPCMA;
#endif	/* SUPPORT_NEW_OUI end */
		tr104VoProf->modemPassThrough=tr104ConvertPassthrough(mdmProfObj->modemPassThrough);

#ifdef BUILD_IGNORE_DIRECT_IP
		tr104VoProf->ZyXEL_ignoreDirectIP=mdmProfObj->X_5067F0_IgnoreDirectIP;
#endif

#ifdef SUPPORT_NEW_OUI
		if(mdmProfObj->X_5067F0_ActiveBoundIfName == NULL){
			TR104_DBG(" mdmProfObj->X_5067F0_ActiveBoundIfName  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_ActiveBoundIfName,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_ActiveBoundIfName, mdmProfObj->X_5067F0_ActiveBoundIfName, min(33, zyStrlen(mdmProfObj->X_5067F0_ActiveBoundIfName)+1));
		}
		if(mdmProfObj->X_5067F0_BoundIfList == NULL){
			TR104_DBG(" mdmProfObj->X_5067F0_BoundIfList  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIfList,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIfList, mdmProfObj->X_5067F0_BoundIfList, min(33, zyStrlen(mdmProfObj->X_5067F0_BoundIfList)+1));
		}
		if(mdmProfObj->X_5067F0_BoundIfName == NULL){
			TR104_DBG(" mdmProfObj->X_5067F0_BoundIfName  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIfName,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIfName, mdmProfObj->X_5067F0_BoundIfName, min(33, zyStrlen(mdmProfObj->X_5067F0_BoundIfName)+1));
		}
		if(mdmProfObj->X_5067F0_BoundIpAddr == NULL){
			TR104_DBG(" mdmProfObj->X_5067F0_BoundIpAddr  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIpAddr,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIpAddr, mdmProfObj->X_5067F0_BoundIpAddr, min(33, zyStrlen(mdmProfObj->X_5067F0_BoundIpAddr)+1));
		}
#else
		if(mdmProfObj->X_ZYXEL_COM_ActiveBoundIfName == NULL){
			TR104_DBG(" mdmProfObj->X_ZYXEL_COM_ActiveBoundIfName  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_ActiveBoundIfName,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_ActiveBoundIfName, mdmProfObj->X_ZYXEL_COM_ActiveBoundIfName, min(33, zyStrlen(mdmProfObj->X_ZYXEL_COM_ActiveBoundIfName)+1));
		}
		if(mdmProfObj->X_ZYXEL_COM_BoundIfList == NULL){
			TR104_DBG(" mdmProfObj->X_ZYXEL_COM_BoundIfList  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIfList,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIfList, mdmProfObj->X_ZYXEL_COM_BoundIfList, min(33, zyStrlen(mdmProfObj->X_ZYXEL_COM_BoundIfList)+1));
		}
		if(mdmProfObj->X_ZYXEL_COM_BoundIfName == NULL){
			TR104_DBG(" mdmProfObj->X_ZYXEL_COM_BoundIfName  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIfName,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIfName, mdmProfObj->X_ZYXEL_COM_BoundIfName, min(33, zyStrlen(mdmProfObj->X_ZYXEL_COM_BoundIfName)+1));
		}
		if(mdmProfObj->X_ZYXEL_COM_BoundIpAddr == NULL){
			TR104_DBG(" mdmProfObj->X_ZYXEL_COM_BoundIpAddr  is NULL");
			zyStrcpy(tr104VoProf->ZyXEL_BoundIpAddr,"");
		} else {
			zyStrncpy(tr104VoProf->ZyXEL_BoundIpAddr, mdmProfObj->X_ZYXEL_COM_BoundIpAddr, min(33, zyStrlen(mdmProfObj->X_ZYXEL_COM_BoundIpAddr)+1));
		}
#endif	/* SUPPORT_NEW_OUI end */

		cmsObj_free((void **) &mdmProfObj);
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmServiceProviderInfoCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmServiceProviderInfoCfg(voiceParams_t *parms , VoiceProfServiceProviderInfoObj_t * ServiceProvider)
{
//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];
	CmsRet ret;
	InstanceIdStack iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfProviderObject *mdm_ProviderObj = NULL;

	levelInfo_t info[_VOICE_MAXLEVELS] = {  {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
		{MDMOID_VOICE_PROF_PROVIDER, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
		{0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the ServiceProviderInfo object */
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdm_ProviderObj)) != CMSRET_SUCCESS )||(ServiceProvider==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_PROVIDER");
		displayIIDStack(&iidStack);
		if(mdm_ProviderObj->name == NULL){
			TR104_DBG(" mdm_ProviderObj->name is NULL");
			zyStrcpy(ServiceProvider->name,"" );
		} else {
			zyStrncpy(ServiceProvider->name, mdm_ProviderObj->name, min(257, zyStrlen(mdm_ProviderObj->name)+1));
		}
		#ifndef SUPPORT_412_VOICE /*Not Support*/
	   	if(mdm_ProviderObj->URL == NULL){
			TR104_DBG(" mdm_ProviderObj->URL is NULL");
			zyStrcpy(ServiceProvider->URL , "");
		} else {
			zyStrncpy(ServiceProvider->URL, mdm_ProviderObj->URL, min(257, zyStrlen(mdm_ProviderObj->URL)+1)); 
		}		
		
		if(mdm_ProviderObj->contactPhoneNumber == NULL){
			TR104_DBG(" mdm_ProviderObj->contactPhoneNumber is NULL");
			zyStrcpy(ServiceProvider->contactPhoneNumber, "");
		} else {
			zyStrncpy(ServiceProvider->contactPhoneNumber, mdm_ProviderObj->contactPhoneNumber, min(33, zyStrlen(mdm_ProviderObj->contactPhoneNumber)+1));
		}
		
		if(mdm_ProviderObj->emailAddress == NULL){
			TR104_DBG(" mdm_ProviderObj->emailAddress is NULL");
			zyStrcpy(ServiceProvider->emailAddress, "");
		} else {
			zyStrncpy(ServiceProvider->emailAddress, mdm_ProviderObj->emailAddress, min(257, zyStrlen(mdm_ProviderObj->emailAddress)+1)); 
		}
		#endif

		/* Free object */
		cmsObj_free((void **) &mdm_ProviderObj);
	}
	return ( ret );
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfSipCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/


CmsRet 
tr104LoadMdmVoiceProfSipCfg(voiceParams_t *parms , VoiceProfSipObj_t * voiceProfSip)
{
//Ssu-Ying, support Multiple Profile
	unsigned int    VpItr   = parms->op[0];
	CmsRet              ret;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfSipObject  *mdm_sipObj    = NULL;

	levelInfo_t   info[_VOICE_MAXLEVELS]    = {  {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_SIP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS){
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	} else {
		/*  Get the MDMOID_VOICE_PROF_SIP object */
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdm_sipObj)) != CMSRET_SUCCESS )||(voiceProfSip==NULL)){
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_SIP");
		displayIIDStack(&iidStack);	
	   	if(mdm_sipObj->proxyServer == NULL){
			TR104_DBG(" mdm_sipObj->proxyServer is NULL");
			zyStrcpy(voiceProfSip->proxyServer, "0.0.0.0");
		}else{
			zyStrncpy(voiceProfSip->proxyServer, mdm_sipObj->proxyServer, min(SIP_SERVER_ADDR_LEN+1, zyStrlen(mdm_sipObj->proxyServer)+1));
		}

		voiceProfSip->proxyServerPort = mdm_sipObj->proxyServerPort ; 
		
		voiceProfSip->proxyServerTransport = tr104ConvertTransport(mdm_sipObj->proxyServerTransport);

	   	if(mdm_sipObj->registrarServer == NULL){
			TR104_DBG(" mdm_sipObj->registrarServer is NULL");
			zyStrcpy(voiceProfSip->registrarServer, "0.0.0.0");
		}else{
			zyStrncpy(voiceProfSip->registrarServer, mdm_sipObj->registrarServer, min(SIP_REGISTER_ADDR_LEN+1, zyStrlen(mdm_sipObj->registrarServer)+1));
		}

		voiceProfSip->registrarServerPort = mdm_sipObj->registrarServerPort ; 
		
		voiceProfSip->registrarServerTransport =  tr104ConvertTransport(mdm_sipObj->registrarServerTransport);

	   	if(mdm_sipObj->userAgentDomain == NULL){
			TR104_DBG(" mdm_sipObj->userAgentDomain is NULL");
			zyStrcpy(voiceProfSip->userAgentDomain, "0.0.0.0");
		}else{
			zyStrncpy(voiceProfSip->userAgentDomain, mdm_sipObj->userAgentDomain, min(SIP_DOMAIN_NAME_LEN+1, zyStrlen(mdm_sipObj->userAgentDomain)+1));
		}
	
		if(0 == mdm_sipObj->userAgentPort){
			voiceProfSip->userAgentPort = USERAGENTPORT_BASIC_VALUE;
		}else{
			voiceProfSip->userAgentPort = mdm_sipObj->userAgentPort ; 
		}		
		voiceProfSip->userAgentTransport = tr104ConvertTransport(mdm_sipObj->userAgentTransport);

	   	if(mdm_sipObj->outboundProxy == NULL){
			TR104_DBG(" mdm_sipObj->outboundProxy is NULL");
			zyStrcpy(voiceProfSip->outboundProxy, "0.0.0.0");
		} else {
			zyStrncpy(voiceProfSip->outboundProxy, mdm_sipObj->outboundProxy, min(SIP_SERVER_ADDR_LEN+1, zyStrlen(mdm_sipObj->outboundProxy)+1));
		}
		voiceProfSip->outboundProxyPort = mdm_sipObj->outboundProxyPort ; 
#ifdef SUPPORT_NEW_OUI
		/* outbound proxy keep alive interval */
		voiceProfSip->ZyXEL_outboundKAIntvl= mdm_sipObj->X_5067F0_outboundKAIntvl;  	/*outboundKAIntvl*/
		/*keep alive target*/
		voiceProfSip->ZyXEL_KAServer = mdm_sipObj->X_5067F0_KAServer;			/*KAServer*/

		if(mdm_sipObj->X_5067F0_fakeSipIP == NULL){
			TR104_DBG(" mdm_sipObj->X_5067F0_fakeSipIP is NULL");
			zyStrcpy(voiceProfSip->ZyXEL_fakeSipIP,"");
		} else {
			zyStrncpy(voiceProfSip->ZyXEL_fakeSipIP, mdm_sipObj->X_5067F0_fakeSipIP, min(SIP_FAKE_NAME_LEN+1, zyStrlen(mdm_sipObj->X_5067F0_fakeSipIP)+1)); //VoiceServiceSignal.fakeSipIP
		}
		voiceProfSip->ZyXEL_fakeSipPort= mdm_sipObj->X_5067F0_fakeSipPort;	
#else
		/* outbound proxy keep alive interval */
		voiceProfSip->ZyXEL_outboundKAIntvl= mdm_sipObj->X_ZyXEL_COM_outboundKAIntvl;  	/*outboundKAIntvl*/
		/*keep alive target*/
		voiceProfSip->ZyXEL_KAServer = mdm_sipObj->X_ZyXEL_COM_KAServer;			/*KAServer*/

		if(mdm_sipObj->X_ZyXEL_COM_fakeSipIP == NULL){
			TR104_DBG(" mdm_sipObj->X_ZyXEL_COM_fakeSipIP is NULL");
			zyStrcpy(voiceProfSip->ZyXEL_fakeSipIP,"");
		} else {
			zyStrncpy(voiceProfSip->ZyXEL_fakeSipIP, mdm_sipObj->X_ZyXEL_COM_fakeSipIP, min(SIP_FAKE_NAME_LEN+1, zyStrlen(mdm_sipObj->X_ZyXEL_COM_fakeSipIP)+1)); //VoiceServiceSignal.fakeSipIP
		}
		voiceProfSip->ZyXEL_fakeSipPort= mdm_sipObj->X_ZyXEL_COM_fakeSipPort;	
#endif	/* SUPPORT_NEW_OUI end */


		voiceProfSip->registrationPeriod = mdm_sipObj->registrationPeriod ; 
		voiceProfSip->registerExpires = mdm_sipObj->registerExpires ;
		voiceProfSip->registerRetryInterval = mdm_sipObj->registerRetryInterval ;
		voiceProfSip->DSCPMark = mdm_sipObj->DSCPMark ; 
		
		if(mdm_sipObj->organization == NULL){
			TR104_DBG(" mdm_sipObj->organization is NULL");
			zyStrcpy(voiceProfSip->organization, "");
		} else {
			/*VoiceServiceSignal.fakeSipIP*/
			zyStrncpy(voiceProfSip->organization, mdm_sipObj->organization, min(257, zyStrlen(mdm_sipObj->organization)+1)); 
		}
#ifndef SUPPORT_412_VOICE /*Not Support*/	
		voiceProfSip->TimerT1=mdm_sipObj->timerT1;
		voiceProfSip->TimerT2=mdm_sipObj->timerT2;
		voiceProfSip->TimerT4=mdm_sipObj->timerT4;
		voiceProfSip->TimerA=mdm_sipObj->timerA;
		voiceProfSip->TimerB=mdm_sipObj->timerB;
		voiceProfSip->TimerC=mdm_sipObj->timerC;
		voiceProfSip->TimerD=mdm_sipObj->timerD;
		voiceProfSip->TimerE=mdm_sipObj->timerE;
		voiceProfSip->TimerF=mdm_sipObj->timerF;
		voiceProfSip->TimerG=mdm_sipObj->timerG;
		voiceProfSip->TimerH=mdm_sipObj->timerH;
		voiceProfSip->TimerI=mdm_sipObj->timerI;
		voiceProfSip->TimerJ=mdm_sipObj->timerJ;
		voiceProfSip->TimerK=mdm_sipObj->timerK;
#endif		
		/*Invite request Expires header value, in seconds.*/
		if(INVITEEXPIRES_MIN_VALUE > mdm_sipObj->inviteExpires){
			ZyPrintf("\r\n TR104 WARNING : invalid inviteExpires value \r\n");
			voiceProfSip->inviteExpires = INVITEEXPIRES_BASIC_VALUE;
		}else{		
			voiceProfSip->inviteExpires=mdm_sipObj->inviteExpires;		/*sessionExpireTime*/
		}
		
		voiceProfSip->reInviteExpires=mdm_sipObj->reInviteExpires;	
		
#ifdef SUPPORT_NEW_OUI
		/*Invite request Expires header value, in seconds.*/
		if(ZyXEL_MINSE_MIN_VALUE > mdm_sipObj->X_5067F0_minSE){
			ZyPrintf("\r\n TR104 WARNING : invalid X_5067F0_minSE value \r\n");
			voiceProfSip->ZyXEL_minSE = ZyXEL_MINSE_BASIC_VALUE;
		}else{		
			voiceProfSip->ZyXEL_minSE = mdm_sipObj->X_5067F0_minSE;	
		}		
		/*Register request Min-Expires header value, in seconds.*/
		voiceProfSip->ZyXEL_mwiExpireTime = mdm_sipObj->X_5067F0_mwiExpireTime;		/*registrarExpireTime*/
#else
		/*Invite request Expires header value, in seconds.*/
		if(ZyXEL_MINSE_MIN_VALUE > mdm_sipObj->X_ZyXEL_COM_minSE){
			ZyPrintf("\r\n TR104 WARNING : invalid X_ZyXEL_COM_minSE value \r\n");
			voiceProfSip->ZyXEL_minSE = ZyXEL_MINSE_BASIC_VALUE;
		}else{		
			voiceProfSip->ZyXEL_minSE = mdm_sipObj->X_ZyXEL_COM_minSE;	
		}		
		/*Register request Min-Expires header value, in seconds.*/
		voiceProfSip->ZyXEL_mwiExpireTime = mdm_sipObj->X_ZyXEL_COM_mwiExpireTime;		/*registrarExpireTime*/
#endif	/* SUPPORT_NEW_OUI end */

		voiceProfSip->inboundAuth=tr104ConvertInBoundAuth(mdm_sipObj->inboundAuth);  /* Default "None"*/

		if(mdm_sipObj->inboundAuthUsername == NULL){
			TR104_DBG(" mdm_sipObj->inboundAuthUsername is NULL");
			zyStrcpy(voiceProfSip->inboundAuthUsername, "");
		} else {
			zyStrncpy(voiceProfSip->inboundAuthUsername, mdm_sipObj->inboundAuthUsername, min(SIP_IBAUTH_USERNAME_LEN+1, zyStrlen(mdm_sipObj->inboundAuthUsername)+1)); //VoiceServiceSignal.fakeSipIP
		}

		if(mdm_sipObj->inboundAuthPassword == NULL){
			TR104_DBG(" mdm_sipObj->inboundAuthPassword is NULL");
			zyStrcpy(voiceProfSip->inboundAuthPassword, "");
		} else {
			zyStrncpy(voiceProfSip->inboundAuthPassword , mdm_sipObj->inboundAuthPassword, min(SIP_IBAUTH_PW_LEN+1, zyStrlen(mdm_sipObj->inboundAuthPassword)+1)); //VoiceServiceSignal.fakeSipIP
		}
#ifndef SUPPORT_412_VOICE /*Not Support*/
		voiceProfSip->useCodecPriorityInSDPResponse=mdm_sipObj->useCodecPriorityInSDPResponse;
#endif
		voiceProfSip->VLANIDMark=mdm_sipObj->VLANIDMark; /*[-1:] , Default = -1*/
		voiceProfSip->ethernetPriorityMark=mdm_sipObj->ethernetPriorityMark;  /*[-1:] , Default = -1*/
#ifndef SUPPORT_412_VOICE /*Not Support*/
		voiceProfSip->SIPEventSubscribeNumberOfElements=mdm_sipObj->SIPEventSubscribeNumberOfElements;
		voiceProfSip->SIPResponseMapNumberOfElements=mdm_sipObj->SIPResponseMapNumberOfElements;
#endif

#ifdef SUPPORT_NEW_OUI
		voiceProfSip->ZyXEL_sigVLanTagTPID=mdm_sipObj->X_5067F0_sigVLanTagTPID;
		voiceProfSip->ZyXEL_sigVLanTagTCI=mdm_sipObj->X_5067F0_sigVLanTagTCI;
		voiceProfSip->ZyXEL_rfc3325support=mdm_sipObj->X_5067F0_rfc3325support;
		voiceProfSip->ZyXEL_rfc3262support=mdm_sipObj->X_5067F0_rfc3262support;

#if HEARTBEAT_MECHANISM_ENABLE
		voiceProfSip->ZyXEL_HeartbeatEnable = mdm_sipObj->X_5067F0_HeartbeatEnable ; 
		voiceProfSip->ZyXEL_RateOfHeartbeat = mdm_sipObj->X_5067F0_RateOfHeartbeat ;
#endif
		voiceProfSip->ZyXEL_rfc3263support = mdm_sipObj->X_5067F0_rfc3263support;
#else
		voiceProfSip->ZyXEL_sigVLanTagTPID=mdm_sipObj->X_ZyXEL_COM_sigVLanTagTPID;
		voiceProfSip->ZyXEL_sigVLanTagTCI=mdm_sipObj->X_ZyXEL_COM_sigVLanTagTCI;
		voiceProfSip->ZyXEL_rfc3325support=mdm_sipObj->X_ZyXEL_COM_rfc3325support;
		voiceProfSip->ZyXEL_rfc3262support=mdm_sipObj->X_ZyXEL_COM_rfc3262support;

#if HEARTBEAT_MECHANISM_ENABLE
		voiceProfSip->ZyXEL_HeartbeatEnable = mdm_sipObj->X_ZyXEL_COM_HeartbeatEnable ; 
		voiceProfSip->ZyXEL_RateOfHeartbeat = mdm_sipObj->X_ZyXEL_COM_RateOfHeartbeat ;
#endif
		voiceProfSip->ZyXEL_rfc3263support = mdm_sipObj->X_ZyXEL_COM_rfc3263support;
#endif	/* SUPPORT_NEW_OUI end */
		/* Free object */
		cmsObj_free((void **) &mdm_sipObj);
	}
	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfSipSubCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfSipSubCfg(voiceParams_t *parms , VoiceProfSipEventSubObj_t * voiceProfSipSub)
{
//Ssu-Ying, support Multiple Profile
	unsigned int    VpItr   = parms->op[0] ;
	unsigned int	    vpSipSubInst = parms->op[1];
	CmsRet              ret;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfSipSubscribeObject  *mdm_sipSubObj    = NULL;

	levelInfo_t   info[_VOICE_MAXLEVELS]    = {{MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_SIP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_SIP_SUBSCRIBE,vpSipSubInst , EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK},};

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfSipSub object */
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdm_sipSubObj)) != CMSRET_SUCCESS )||(voiceProfSipSub==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		
		TR104_DBG("MDMOID_VOICE_PROF_SIP_SUBSCRIBE");
		displayIIDStack(&iidStack);

		if(mdm_sipSubObj->event == NULL){
			TR104_DBG(" mdm_sipSubObj->event  is NULL");
			zyStrcpy(voiceProfSipSub->event, "");
		} else {
			zyStrncpy(voiceProfSipSub->event, mdm_sipSubObj->event, min(33, zyStrlen(mdm_sipSubObj->event)+1)); 
		}	
		
		if(mdm_sipSubObj->notifier == NULL){
			TR104_DBG(" mdm_sipSubObj->notifier  is NULL");
			zyStrcpy(voiceProfSipSub->notifier, "");
		} else {
			zyStrncpy(voiceProfSipSub->notifier, mdm_sipSubObj->notifier, min(257, zyStrlen(mdm_sipSubObj->notifier)+1)); 
		}
		
		voiceProfSipSub->notifierPort=mdm_sipSubObj->notifierPort;
		voiceProfSipSub->notifierTransport=tr104ConvertTransport(mdm_sipSubObj->notifierTransport);
		#ifndef SUPPORT_412_VOICE /*Not Support*/
		voiceProfSipSub->expireTime=mdm_sipSubObj->expireTime;
		#endif
			
		/* Free object */
		cmsObj_free((void **) &mdm_sipSubObj);
	}
	return (ret);
}
#ifndef SUPPORT_412_VOICE /*Not Support*/
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfSipRespCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/


CmsRet 
tr104LoadMdmVoiceProfSipRespCfg(voiceParams_t *parms , VoiceProfSipRespMapObj_t * voiceProfSipResp)
{
//Ssu-Ying, support Multiple Profile
	unsigned int    VpItr   = parms->op[0] ;
	unsigned int	    vpSipRespInst = parms->op[1];
	CmsRet              ret;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfSipResponseObject  *mdm_sipRespObj    = NULL;

	levelInfo_t   info[_VOICE_MAXLEVELS]    = {  {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_SIP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_SIP_RESPONSE,vpSipRespInst , EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfSipResp object */
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdm_sipRespObj)) != CMSRET_SUCCESS )||(voiceProfSipResp==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfSipResp object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		
		TR104_DBG("MDMOID_VOICE_PROF_SIP_RESPONSE");
		displayIIDStack(&iidStack);
		
		if(mdm_sipRespObj->textMessage == NULL){
			TR104_DBG(" mdm_sipRespObj->textMessage  is NULL");
			zyStrcpy(voiceProfSipResp->textMessage ,"" ) ; 
		} else {
			zyStrncpy(voiceProfSipResp->textMessage, mdm_sipRespObj->textMessage, min(65, zyStrlen(mdm_sipRespObj->textMessage)+1)); //VoiceServiceSignal.fakeSipIP
		}
		voiceProfSipResp->SIPResponseNumber=mdm_sipRespObj->SIPResponseNumber ;
		voiceProfSipResp->tone=mdm_sipRespObj->tone ;
		
		/* Free object */
		cmsObj_free((void **) &mdm_sipRespObj);
	}
	return (ret);
}
#endif
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfRtpCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/


CmsRet 
tr104LoadMdmVoiceProfRtpCfg( voiceParams_t *parms , VoiceProfRtpObj_t * RTPProf)
{
	CmsRet              ret;
   InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
   _VoiceProfRtpObject *mdmVoiceProfRtpObj = NULL;

//Ssu-Ying, support Multiple Profile
   unsigned int VpItr = parms->op[0];

   levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS)
	{	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	}
	else
	{
	   /*  Get the VoiceProfRtp object */
	   if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmVoiceProfRtpObj)) != CMSRET_SUCCESS)||(RTPProf==NULL))
	   {
	   	cmsLck_releaseLock();
	      cmsLog_error( "Can't retrieve VoiceProfRtp object \n" );
	      return ( ret );
	   }
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_RTP");
		displayIIDStack(&iidStack);	
		if(TR104_ERROR == tr104ValidationRtpPortRange(mdmVoiceProfRtpObj->localPortMin , mdmVoiceProfRtpObj->localPortMax)){
			ZyPrintf("\r\n TR104 WARNING : invalid localPortMin %d, localPortMax value %d\r\n", mdmVoiceProfRtpObj->localPortMin, mdmVoiceProfRtpObj->localPortMax);
			RTPProf->localPortMin = RTP_LOCALPORTMIN_BASIC_VALUE;
			RTPProf->localPortMax = RTP_LOCALPORTMAX_BASIC_VALUE;
		}else{
			RTPProf->localPortMin=mdmVoiceProfRtpObj->localPortMin;
			RTPProf->localPortMax=mdmVoiceProfRtpObj->localPortMax;
		}
		RTPProf->DSCPMark=mdmVoiceProfRtpObj->DSCPMark;
		RTPProf->VLANIDMark=mdmVoiceProfRtpObj->VLANIDMark;
		RTPProf->ethernetPriorityMark=mdmVoiceProfRtpObj->ethernetPriorityMark;		
		RTPProf->telephoneEventPayloadType=mdmVoiceProfRtpObj->telephoneEventPayloadType;

		/* Free object */
		cmsObj_free((void **) &mdmVoiceProfRtpObj);
	}	
	return (ret);
}
#ifndef SUPPORT_412_VOICE /*Not Support*/
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfRtpRtcpCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfRtpRtcpCfg( voiceParams_t *parms , VoiceProfRtpRtcpObj_t * RTCPProf)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfRtpRtcpObject *mdmProfRtpRtcpObj = NULL;

//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP_RTCP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );
		return (ret);
	} else {
		/*  Get the VoiceProfRtpRtcp object */
		if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmProfRtpRtcpObj)) != CMSRET_SUCCESS)||(RTCPProf==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfRtpRtcp object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_RTP_RTCP");
		displayIIDStack(&iidStack);

		RTCPProf->enable=mdmProfRtpRtcpObj->enable;

		RTCPProf->txRepeatInterval=mdmProfRtpRtcpObj->txRepeatInterval;

		if(mdmProfRtpRtcpObj->localCName == NULL){
			TR104_DBG(" mdmProfRtpRtcpObj->localCName is NULL");
			zyStrcpy(RTCPProf->localCName, "");
		} else {
			zyStrncpy(RTCPProf->localCName, mdmProfRtpRtcpObj->localCName, min(65, zyStrlen(mdmProfRtpRtcpObj->localCName)+1));
		}

		/* Free object */
		cmsObj_free((void **) &mdmProfRtpRtcpObj);
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfRtpSrtpCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/
#endif
#ifdef MSTC_VOICE_SUPPORT_SRTP
CmsRet 
tr104LoadMdmVoiceProfRtpSrtpCfg( voiceParams_t *parms , VoiceProfRtpSrtpObj_t * SRTPProf)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfRtpSrtpObject *mdmProfRtpSrtpObj = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP_SRTP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n");
		return (ret);
	} else {
		/*  Get the VoiceProfRtpSrtp object */
		if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmProfRtpSrtpObj)) != CMSRET_SUCCESS)||(SRTPProf==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfRtpSrtp object \n");
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_RTP_SRTP");
		displayIIDStack(&iidStack);

		SRTPProf->enable = mdmProfRtpSrtpObj->enable;

		if(mdmProfRtpSrtpObj->keyingMethods== NULL){
			TR104_DBG(" mdmProfRtpSrtpObj->keyingMethods is NULL");
			zyStrcpy(SRTPProf->keyingMethods, "");
		} else {
			zyStrncpy(SRTPProf->keyingMethods, mdmProfRtpSrtpObj->keyingMethods, min(257, zyStrlen(mdmProfRtpSrtpObj->keyingMethods)+1));
		}

		if(mdmProfRtpSrtpObj->encryptionKeySizes== NULL){
			TR104_DBG(" mdmProfRtpSrtpObj->encryptionKeySizes is NULL");
			zyStrcpy(SRTPProf->encryptionKeySizes, "");
		} else {
			zyStrncpy(SRTPProf->encryptionKeySizes, mdmProfRtpSrtpObj->encryptionKeySizes, min(257, zyStrlen(mdmProfRtpSrtpObj->encryptionKeySizes)+1));
		}

#ifdef SUPPORT_NEW_OUI
		if(mdmProfRtpSrtpObj->X_5067F0_Crypto_Suite_Name ==  NULL){
			TR104_DBG(" mdmProfRtpSrtpObj->X_5067F0_Crypto_Suite_Name is NULL");
			zyStrcpy(SRTPProf->ZyXEL_CryptoSuiteName, "");
		}
		else{
			zyStrncpy(SRTPProf->ZyXEL_CryptoSuiteName, mdmProfRtpSrtpObj->X_5067F0_Crypto_Suite_Name, min(65, zyStrlen(mdmProfRtpSrtpObj->X_5067F0_Crypto_Suite_Name)+1));
		}
#else
		if(mdmProfRtpSrtpObj->X_ZyXEL_COM_Crypto_Suite_Name ==  NULL){
			TR104_DBG(" mdmProfRtpSrtpObj->X_ZyXEL_COM_Crypto_Suite_Name is NULL");
			zyStrcpy(SRTPProf->ZyXEL_CryptoSuiteName, "");
		}
		else{
			zyStrncpy(SRTPProf->ZyXEL_CryptoSuiteName, mdmProfRtpSrtpObj->X_ZyXEL_COM_Crypto_Suite_Name, min(65, zyStrlen(mdmProfRtpSrtpObj->X_ZyXEL_COM_Crypto_Suite_Name)+1));
		}
#endif	/* SUPPORT_NEW_OUI end */

		/* Free object */
		cmsObj_free((void **) &mdmProfRtpSrtpObj);
	}
	return (ret);
}
#endif
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfRtpRedundancyCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

#ifndef SUPPORT_412_VOICE /*Not Support*/
CmsRet 
tr104LoadMdmVoiceProfRtpRedundancyCfg( voiceParams_t *parms , VoiceProfRtpRedundancyObj_t * ProfRtpRedundancyObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfRtpRedundancyObject *mdmProfRtpRedundancyObj = NULL;

//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_RTP_REDUNDANCY, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n");
		return (ret);
	} else {
		/*  Get the VoiceProfRtpRedundancy object */
		if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmProfRtpRedundancyObj)) != CMSRET_SUCCESS)||(ProfRtpRedundancyObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfRtpRedundancy object \n");
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_RTP_REDUNDANCY");
		displayIIDStack(&iidStack);

		ProfRtpRedundancyObj->enable = mdmProfRtpRedundancyObj->enable;
		ProfRtpRedundancyObj->payloadType = mdmProfRtpRedundancyObj->payloadType;
		ProfRtpRedundancyObj->blockPayloadType = mdmProfRtpRedundancyObj->blockPayloadType;
		ProfRtpRedundancyObj->faxAndModemRedundancy = mdmProfRtpRedundancyObj->faxAndModemRedundancy;
		ProfRtpRedundancyObj->modemRedundancy = mdmProfRtpRedundancyObj->modemRedundancy;
		ProfRtpRedundancyObj->DTMFRedundancy = mdmProfRtpRedundancyObj->DTMFRedundancy;
		ProfRtpRedundancyObj->voiceRedundancy = mdmProfRtpRedundancyObj->voiceRedundancy;
		ProfRtpRedundancyObj->maxSessionsUsingRedundancy = mdmProfRtpRedundancyObj->maxSessionsUsingRedundancy;

		/* Free object */
		cmsObj_free((void **) &mdmProfRtpRedundancyObj);
	}
	return (ret);
}
#endif

/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfNumberingPlanCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfNumberingPlanCfg( voiceParams_t *parms , VoiceProfNumPlanObj_t * NumberingPlanObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfNumberingPlanObject *mdmVoiceProfNumPlanObj = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_NUMBERING_PLAN , _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfNumberingPlan object */
		if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmVoiceProfNumPlanObj)) != CMSRET_SUCCESS)||(NumberingPlanObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfNumberingPlan object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		
		TR104_DBG("MDMOID_VOICE_PROF_LINE_NUMBERING_PLAN");
		displayIIDStack(&iidStack);
		
		NumberingPlanObj->minimumNumberOfDigits = mdmVoiceProfNumPlanObj->minimumNumberOfDigits ;
		NumberingPlanObj->maximumNumberOfDigits = mdmVoiceProfNumPlanObj->maximumNumberOfDigits ;
		if((INTERDIGITTIMERSTD_MIN_VALUE > mdmVoiceProfNumPlanObj->interDigitTimerStd)
			||(INTERDIGITTIMERSTD_MAX_VALUE < mdmVoiceProfNumPlanObj->interDigitTimerStd))
		{
			ZyPrintf("\r\n TR104 WARNING : invalid interDigitTimerStd value \r\n");
			NumberingPlanObj->interDigitTimerStd = INTERDIGITTIMERSTD_BASIC_VALUE;
		}else{
			NumberingPlanObj->interDigitTimerStd = mdmVoiceProfNumPlanObj->interDigitTimerStd ;
		}
		NumberingPlanObj->interDigitTimerOpen = mdmVoiceProfNumPlanObj->interDigitTimerOpen ;
	
#ifdef SUPPORT_NEW_OUI
		if(0 == (mdmVoiceProfNumPlanObj->X_5067F0_dialingLongInterval)){
			ZyPrintf("\r\n TR104 WARNING : invalid X_5067F0_dialingLongInterval value \r\n");
			NumberingPlanObj->ZyXEL_dialingLongInterval = ZyXEL_DIALINGLONGINTERVAL_BASIC_VALUE;
		}else{
			NumberingPlanObj->ZyXEL_dialingLongInterval = mdmVoiceProfNumPlanObj->X_5067F0_dialingLongInterval ;		
		}

#else
		if(0 == (mdmVoiceProfNumPlanObj->X_ZyXEL_COM_dialingLongInterval)){
			ZyPrintf("\r\n TR104 WARNING : invalid X_ZyXEL_COM_dialingLongInterval value \r\n");
			NumberingPlanObj->ZyXEL_dialingLongInterval = ZyXEL_DIALINGLONGINTERVAL_BASIC_VALUE;
		}else{
			NumberingPlanObj->ZyXEL_dialingLongInterval = mdmVoiceProfNumPlanObj->X_ZyXEL_COM_dialingLongInterval ;		
		}
#endif	/* SUPPORT_NEW_OUI end */
		
		NumberingPlanObj->invalidNumberTone = mdmVoiceProfNumPlanObj->invalidNumberTone ;
		NumberingPlanObj->prefixInfoMaxEntries = mdmVoiceProfNumPlanObj->prefixInfoMaxEntries ;
		NumberingPlanObj->prefixInfoNumberOfEntries = mdmVoiceProfNumPlanObj->prefixInfoNumberOfEntries ;

		/* Free object */
		cmsObj_free((void **) &mdmVoiceProfNumPlanObj);
	}	
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfNumPlanPrefixCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfNumPlanPrefixCfg( voiceParams_t *parms , VoiceProfNumPlanPrefixInfoObj_t * prefixInfoObj )
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfNumberingPlanPrefixInfoObject *mdmPrefixInfoObj = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int VpItr = parms->op[0];
	unsigned int prefixInst = parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_NUMBERING_PLAN , _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_PROF_NUMBERING_PLAN_PREFIX_INFO , prefixInst, EMPTY_INSTANCE_ID_STACK},
   					   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS){	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfNumPlanPrefix object */
		if ( ((ret = getMdmObj(info, &iidStack, (void**)&mdmPrefixInfoObj)) != CMSRET_SUCCESS)||(prefixInfoObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfNumPlanPrefix object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_NUMBERING_PLAN_PREFIX_INFO");
		displayIIDStack(&iidStack);		
		if(mdmPrefixInfoObj->prefixRange== NULL){
			TR104_DBG(" mdmPrefixInfoObj->prefixRange is NULL");
			zyStrcpy(prefixInfoObj->prefixRange , "");
		} else {
			zyStrncpy(prefixInfoObj->prefixRange, mdmPrefixInfoObj->prefixRange, min(43, zyStrlen(mdmPrefixInfoObj->prefixRange)+1));
		}

		if(mdmPrefixInfoObj->facilityAction== NULL){
			TR104_DBG(" mdmPrefixInfoObj->facilityAction is NULL");
			zyStrcpy(prefixInfoObj->facilityAction , "");
		} else {
			zyStrncpy(prefixInfoObj->facilityAction, mdmPrefixInfoObj->facilityAction, min(65, zyStrlen(mdmPrefixInfoObj->facilityAction)+1));
		}

		if(mdmPrefixInfoObj->facilityActionArgument== NULL){
			TR104_DBG(" mdmPrefixInfoObj->facilityActionArgument is NULL");
			zyStrcpy(prefixInfoObj->facilityActionArgument , "");
		} else {
			zyStrncpy(prefixInfoObj->facilityActionArgument, mdmPrefixInfoObj->facilityActionArgument, min(257, zyStrlen(mdmPrefixInfoObj->facilityActionArgument)+1));
		}
		prefixInfoObj->prefixMinNumberOfDigits = mdmPrefixInfoObj->prefixMinNumberOfDigits;
		prefixInfoObj->prefixMaxNumberOfDigits = mdmPrefixInfoObj->prefixMaxNumberOfDigits;
		prefixInfoObj->numberOfDigitsToRemove = mdmPrefixInfoObj->numberOfDigitsToRemove;
		prefixInfoObj->posOfDigitsToRemove = mdmPrefixInfoObj->posOfDigitsToRemove;
		prefixInfoObj->dialTone = mdmPrefixInfoObj->dialTone;


		/* Free object */
		cmsObj_free((void **) &mdmPrefixInfoObj);
	}	
	return (ret);
}
#ifndef SUPPORT_412_VOICE
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfToneCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfToneCfg( voiceParams_t *parms ,VoiceProfToneObj_t * profToneObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfToneObject *mdmToneObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS]     = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS){	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get theVoiceProfTone object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmToneObj)) != CMSRET_SUCCESS)||(profToneObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfTone object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		profToneObj->eventNumberOfEntries=mdmToneObj->eventNumberOfEntries;
		profToneObj->descriptionNumberOfEntries=mdmToneObj->descriptionNumberOfEntries;
		profToneObj->patternNumberOfEntries=mdmToneObj->patternNumberOfEntries;

		/* Free object */
		cmsObj_free((void **) &mdmToneObj);	
	}
	return (ret);
}


/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfToneEventCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfToneEventCfg( voiceParams_t *parms ,VoiceProfToneEventObj_t * toneEventObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfToneEventObject *mdmToneEventObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  toneEventInst   = parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS]     = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE_EVENT, toneEventInst, EMPTY_INSTANCE_ID_STACK},
   						{0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfToneEvent object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmToneEventObj)) != CMSRET_SUCCESS)||(toneEventObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfToneEvent object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   	TR104_DBG("MDMOID_VOICE_PROF_TONE_EVENT");
		displayIIDStack(&iidStack);
#if 0
		if(mdmToneEventObj->function== NULL){
			TR104_DBG(" mdmToneEventObj->Function is NULL");
			zyStrcpy(toneEventObj->function , "");
		}else{   		
			zyStrcpy(toneEventObj->function , mdmToneEventObj->function);
		}
#endif
		toneEventObj->function=tr104ConvertToneEventFunction(mdmToneEventObj->function);
		toneEventObj->toneID=mdmToneEventObj->toneID;
   
		/* Free object */
		cmsObj_free((void **) &mdmToneEventObj);	
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfToneDescriptionCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet
tr104LoadMdmVoiceProfToneDescriptionCfg( voiceParams_t *parms ,VoiceProfToneDescriptionObj_t * toneDescriptionObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfToneDescriptionObject *mdmTonetDescriptionObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  toneDiscriptionInst   = parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS]     = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE_DESCRIPTION, toneDiscriptionInst, EMPTY_INSTANCE_ID_STACK},
   						{0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceProfToneDescription object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmTonetDescriptionObj)) != CMSRET_SUCCESS)||(toneDescriptionObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfToneDescription object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_TONE_DESCRIPTION");
		displayIIDStack(&iidStack);   
		if(mdmTonetDescriptionObj->toneName== NULL){
			TR104_DBG(" mdmTonetDescriptionObj->toneName is NULL");
			zyStrcpy(toneDescriptionObj->toneName , "");
		} else {
			zyStrncpy(toneDescriptionObj->toneName, mdmTonetDescriptionObj->toneName, min(65, zyStrlen(mdmTonetDescriptionObj->toneName)+1));
		}
		if(mdmTonetDescriptionObj->toneFile== NULL){
			TR104_DBG(" mdmTonetDescriptionObj->toneFile is NULL");
			zyStrcpy(toneDescriptionObj->toneFile , "");
		} else {
			zyStrncpy(toneDescriptionObj->toneFile , mdmTonetDescriptionObj->toneFile, min(257, zyStrlen(mdmTonetDescriptionObj->toneFile)+1));
		}
		if(mdmTonetDescriptionObj->toneText== NULL){
			TR104_DBG(" mdmTonetDescriptionObj->toneText is NULL");
			zyStrcpy(toneDescriptionObj->toneText , "");
		} else {
			zyStrncpy(toneDescriptionObj->toneText , mdmTonetDescriptionObj->toneText, min(65, zyStrlen(mdmTonetDescriptionObj->toneText)+1));
		}		
		toneDescriptionObj->entryID=mdmTonetDescriptionObj->entryID;
		toneDescriptionObj->toneEnable=mdmTonetDescriptionObj->toneEnable;
		toneDescriptionObj->tonePattern=mdmTonetDescriptionObj->tonePattern;
		toneDescriptionObj->toneRepetitions=mdmTonetDescriptionObj->toneRepetitions;

		/* Free object */
		cmsObj_free((void **) &mdmTonetDescriptionObj);	
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfTonePatternCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceProfTonePatternCfg( voiceParams_t *parms ,VoiceProfTonePatternObj_t * tonePatternObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfTonePatternObject *mdmTonePatternObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  tonePatternInst   = parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS]     = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_TONE_PATTERN, tonePatternInst, EMPTY_INSTANCE_ID_STACK},
   						{0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
 	if(ret!=CMSRET_SUCCESS) {
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	}
	else {
		/*  Get the VoiceProfTonePattern object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmTonePatternObj)) != CMSRET_SUCCESS)||(tonePatternObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceProfTonePattern object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_PROF_TONE_PATTERN");
		displayIIDStack(&iidStack);   	   
	
		tonePatternObj->entryID=mdmTonePatternObj->entryID;
		tonePatternObj->toneOn=mdmTonePatternObj->toneOn;
		tonePatternObj->frequency1=mdmTonePatternObj->frequency1;
		tonePatternObj->power1=mdmTonePatternObj->power1;
		
		tonePatternObj->frequency2=mdmTonePatternObj->frequency2;
		tonePatternObj->power2=mdmTonePatternObj->power2;
		
		tonePatternObj->frequency3=mdmTonePatternObj->frequency3;
		tonePatternObj->power3=mdmTonePatternObj->power3;
		
		tonePatternObj->frequency4=mdmTonePatternObj->frequency4;
		tonePatternObj->power4=mdmTonePatternObj->power4;
		
		tonePatternObj->modulationFrequency=mdmTonePatternObj->modulationFrequency;
		tonePatternObj->modulationPower=mdmTonePatternObj->modulationPower;
		tonePatternObj->duration=mdmTonePatternObj->duration;
		tonePatternObj->nextEntryID=mdmTonePatternObj->nextEntryID;

		/* Free object */
		cmsObj_free((void **) &mdmTonePatternObj);	
	}
	return (ret);
}
#endif
/***************************************************************************
** Function Name: tr104LoadMdmVoiceProfFaxT38Cfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet
tr104LoadMdmVoiceProfFaxT38Cfg( voiceParams_t *parms ,VoiceProfFaxT38Obj_t * FaxT38Prof)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceProfFaxT38Object *mdmFaxT38obj      = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS]     = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                       {MDMOID_VOICE_PROF_FAX_T38, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the Voice Profile object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmFaxT38obj)) != CMSRET_SUCCESS)||(FaxT38Prof == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve fax T38 object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

	   	TR104_DBG("MDMOID_VOICE_PROF_FAX_T38");
		displayIIDStack(&iidStack);
			
		FaxT38Prof->enable = mdmFaxT38obj->enable ;
		FaxT38Prof->bitRate = mdmFaxT38obj->bitRate ;/*uint16 : uint32*/
		FaxT38Prof->highSpeedPacketRate = mdmFaxT38obj->highSpeedPacketRate ;/*uint8 : uint32*/
		
		FaxT38Prof->highSpeedRedundancy;
		FaxT38Prof->lowSpeedRedundancy;
		FaxT38Prof->TCFMethod = tr104ConvertTCFMethod(mdmFaxT38obj->TCFMethod) ; 

		/* Free object */
		cmsObj_free((void **) &mdmFaxT38obj);	
	}
	return (ret);
}


/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet
tr104LoadMdmVoiceLineCfg( voiceParams_t *parms  ,VoiceProfLineObj_t * LineProf)
{
//Ssu-Ying, support Multiple Profile
	unsigned int        VpItr     = parms->op[0];
	unsigned int        LineItr   = parms->op[1];

	CmsRet   ret = CMSRET_SUCCESS;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineObject    *mdmLineObj    = NULL;

	levelInfo_t   info[_VOICE_MAXLEVELS]    = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                      {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                      {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	TR104_DBG("enter tr104LoadMdmVoiceProfLineCfg ");
	/*  check  */
	if ( LineProf==NULL){
		cmsLog_error( " NULL pointer \n" );
		ret = CMSRET_INTERNAL_ERROR;
		return (ret);
	}
	
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLine object */
		if  ((ret = getMdmObj(info, &iidStack, (void**)&mdmLineObj)) != CMSRET_SUCCESS ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLine object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   	TR104_DBG("MDMOID_VOICE_LINE");
		displayIIDStack(&iidStack);

	   	if(mdmLineObj->enable == NULL){
			TR104_DBG(" mdmLineObj->enable is NULL");
		} else {
	   		LineProf->enable = tr104ConvertEnable(mdmLineObj->enable)  ;
		}

#ifdef SUPPORT_NEW_OUI
		LineProf->ZyXEL_lineId = mdmLineObj->X_5067F0_lineID;
#else
		LineProf->ZyXEL_lineId = mdmLineObj->X_ZyXEL_COM_lineID;
#endif	/* SUPPORT_NEW_OUI end */
		
	   	if(mdmLineObj->directoryNumber == NULL){
			TR104_DBG(" mdmLineObj->directoryNumber is NULL");
			zyStrcpy(LineProf->directoryNumber , "" );
		} else {
			zyStrncpy(LineProf->directoryNumber, mdmLineObj->directoryNumber, min(33, zyStrlen(mdmLineObj->directoryNumber)+1));
		}		
		
	   	if(mdmLineObj->phyReferenceList == NULL){
			TR104_DBG(" mdmLineObj->phyReferenceList is NULL");
			zyStrcpy(LineProf->phyReferenceList , "" );
		} else {
			zyStrncpy(LineProf->phyReferenceList, mdmLineObj->phyReferenceList, min(33, zyStrlen(mdmLineObj->phyReferenceList)+1));
		}	
#ifdef SUPPORT_NEW_OUI
	   	if(mdmLineObj->X_5067F0_AutoDialNumber == NULL){
			TR104_DBG(" mdmLineObj->X_5067F0_AutoDialNumber is NULL");
			zyStrcpy(LineProf->ZyXEL_AutoDialNumber , "" );
		} else {
			zyStrncpy(LineProf->ZyXEL_AutoDialNumber, mdmLineObj->X_5067F0_AutoDialNumber, min(PSTN_PHONE_NUM_LEN+1, zyStrlen(mdmLineObj->X_5067F0_AutoDialNumber)+1));
		}			
#else
	   	if(mdmLineObj->X_ZyXEL_COM_AutoDialNumber == NULL){
			TR104_DBG(" mdmLineObj->X_ZyXEL_COM_AutoDialNumber is NULL");
			zyStrcpy(LineProf->ZyXEL_AutoDialNumber , "" );
		} else {
			zyStrncpy(LineProf->ZyXEL_AutoDialNumber, mdmLineObj->X_ZyXEL_COM_AutoDialNumber, min(PSTN_PHONE_NUM_LEN+1, zyStrlen(mdmLineObj->X_ZyXEL_COM_AutoDialNumber)+1));
		}			
#endif	/* SUPPORT_NEW_OUI end */
		if(!LineProf->enable){			
		    LineProf->status = TR104I_STATUS_DISABLED;			
			voipLedOnOff( tr104GetNumOfPhyIntf() );    //Ssu-Ying, support Multiple Profile
		}

		TR104_PRINTF("LineProf->status %d\n", LineProf->status);
		
		LineProf->callState = tr104ConvertCallState(mdmLineObj->callState); /* def = IDLE , R*/

#ifdef SUPPORT_NEW_OUI
		/* bit map for SIP_FXS mapping */ /* bit 0 mapping to phone id 0, ... */
		if(0==mdmLineObj->X_5067F0_phyReferenceFXSList){
			LineProf->ZyXEL_phyReferenceFXSList = ZyXEL_PHYREFERENCEFXSLIST_BASIC_VALUE;		
		}else{
			LineProf->ZyXEL_phyReferenceFXSList = mdmLineObj->X_5067F0_phyReferenceFXSList;		/*VoiceServiceSignal.phoneSelect*/
		}
		LineProf->ZyXEL_AutoDialEnable=mdmLineObj->X_5067F0_AutoDialEnable;			/* Auto Dial Enable/Disable */		
#ifndef SUPPORT_412_VOICE /*Not Support*/
		LineProf->ringMuteStatus=mdmLineObj->ringMuteStatus;		/*R*/
		LineProf->ringVolumeStatus=mdmLineObj->ringVolumeStatus;/*[0:100],R*/
#endif
		LineProf->ZyXEL_CMAcntNum = mdmLineObj->X_5067F0_CMAcntNum;
#else
		/* bit map for SIP_FXS mapping */ /* bit 0 mapping to phone id 0, ... */
		if(0==mdmLineObj->X_ZyXEL_COM_phyReferenceFXSList){
			LineProf->ZyXEL_phyReferenceFXSList = ZyXEL_PHYREFERENCEFXSLIST_BASIC_VALUE;		
		}else{
			LineProf->ZyXEL_phyReferenceFXSList = mdmLineObj->X_ZyXEL_COM_phyReferenceFXSList;		/*VoiceServiceSignal.phoneSelect*/
		}
		LineProf->ZyXEL_AutoDialEnable=mdmLineObj->X_ZyXEL_COM_AutoDialEnable;			/* Auto Dial Enable/Disable */		

		LineProf->ringMuteStatus=mdmLineObj->ringMuteStatus;		/*R*/
		LineProf->ringVolumeStatus=mdmLineObj->ringVolumeStatus;/*[0:100],R*/
		LineProf->ZyXEL_CMAcntNum = mdmLineObj->X_BROADCOM_COM_CMAcntNum;
#endif	/* SUPPORT_NEW_OUI end */

		cmsObj_free((void **) &mdmLineObj);

	}
	return ( ret );
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineSipCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineSipCfg( voiceParams_t *parms  ,VoiceProfLineSipObj_t * LineSip)
{
	CmsRet    ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineSipObject  *mdmSipObj = NULL;
//Ssu-Ying, support Multiple Profile
	/* parse Service Provider Number as unsigned int */
	unsigned int VpItr = parms->op[0];
	/* parse Account Number as unsigned int */
	unsigned int LineItr= parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_SIP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);
	} else {
		/*  Get the VoiceLineSip object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmSipObj)) != CMSRET_SUCCESS)||(LineSip==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLineSip object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_LINE_SIP");
		displayIIDStack(&iidStack);   

	   	if(mdmSipObj->authUserName == NULL){
			TR104_DBG(" mdmSipObj->authUserName is NULL");
			zyStrcpy(LineSip->authUserName, "");
		} else {
			zyStrncpy(LineSip->authUserName, mdmSipObj->authUserName, min(129, zyStrlen(mdmSipObj->authUserName)+1));
		}
		
	   	if(mdmSipObj->authPassword == NULL){
			TR104_DBG(" mdmSipObj->authPassword is NULL");
			zyStrcpy(LineSip->authPassword, "");
		} else {
			zyStrncpy(LineSip->authPassword, mdmSipObj->authPassword, min(129, zyStrlen(mdmSipObj->authPassword)+1));
		}	

	   	if(mdmSipObj->URI == NULL){
			TR104_DBG(" mdmSipObj->URI is NULL");
			zyStrcpy(LineSip->URI, "");
		} else {
			zyStrncpy(LineSip->URI, mdmSipObj->URI, min(390, zyStrlen(mdmSipObj->URI)+1));
		}		
		
#ifdef SUPPORT_NEW_OUI
		LineSip->ZyXEL_registrarType = tr104ConvertRegType(mdmSipObj->X_5067F0_registrarType) ;
		LineSip->ZyXEL_url_type=tr104ConvertURLType(mdmSipObj->X_5067F0_url_type) ;
		LineSip->ZyXEL_mixerMode=tr104ConvertMixMode(mdmSipObj->X_5067F0_mixerMode) ;
		//LineSip->ZyXEL_remoteRingbackTone=tr104ConvertRemoteRingback(mdmSipObj->X_5067F0_remoteRingbackTone) ;	/* aircheng delete - move to calling feature */
		
		LineSip->ZyXEL_callFwdTableIndex=mdmSipObj->X_5067F0_callFwdTableIndex ;
		LineSip->SIPEventSubscribeNumberOfElements=mdmSipObj->SIPEventSubscribeNumberOfElements ;
#else
		LineSip->ZyXEL_registrarType = tr104ConvertRegType(mdmSipObj->X_ZyXEL_COM_registrarType) ;
		LineSip->ZyXEL_url_type=tr104ConvertURLType(mdmSipObj->X_ZyXEL_COM_url_type) ;
		LineSip->ZyXEL_mixerMode=tr104ConvertMixMode(mdmSipObj->X_ZyXEL_COM_mixerMode) ;
		//LineSip->ZyXEL_remoteRingbackTone=tr104ConvertRemoteRingback(mdmSipObj->X_ZyXEL_COM_remoteRingbackTone) ;	/* aircheng delete - move to calling feature */
		
		LineSip->ZyXEL_callFwdTableIndex=mdmSipObj->X_ZyXEL_COM_callFwdTableIndex ;
		LineSip->SIPEventSubscribeNumberOfElements=mdmSipObj->SIPEventSubscribeNumberOfElements ;
#endif	/* SUPPORT_NEW_OUI end */

		cmsObj_free((void **) &mdmSipObj);
		
	}
	return ( ret );
}
#ifndef SUPPORT_412_VOICE
/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineSipEventSubCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/


/*! \brief InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.EventSubscribe.{i}. */

CmsRet
tr104LoadMdmVoiceLineSipEventSubCfg( voiceParams_t *parms ,VoiceProfLineSipEventSubObj_t * lineSipEventSubObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineSipEventSubscribeObject *mdmLineSipSubObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  LineItr   = parms->op[1];
	unsigned int  sipEventSubInst   = parms->op[2];
   
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_SIP, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_SIP_EVENT_SUBSCRIBE, sipEventSubInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLineSipEventSub object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineSipSubObj)) != CMSRET_SUCCESS)||(lineSipEventSubObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLineSipEventSub object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   	TR104_DBG("MDMOID_VOICE_LINE_SIP_EVENT_SUBSCRIBE");
		displayIIDStack(&iidStack);
	   	if(mdmLineSipSubObj->event == NULL){
			TR104_DBG(" mdmLineSipSubObj->event is NULL");
			zyStrcpy(lineSipEventSubObj->event, "");
		} else {
			zyStrncpy(lineSipEventSubObj->event, mdmLineSipSubObj->event, min(33, zyStrlen(mdmLineSipSubObj->event)+1));
		}	

	   	if(mdmLineSipSubObj->authUserName == NULL){
			TR104_DBG(" mdmLineSipSubObj->authUserName is NULL");
			zyStrcpy(lineSipEventSubObj->authUserName, "");
		} else {
			zyStrncpy(lineSipEventSubObj->authUserName, mdmLineSipSubObj->authUserName, min(129, zyStrlen(mdmLineSipSubObj->authUserName)+1));
		}	

	   	if(mdmLineSipSubObj->authPassword == NULL){
			TR104_DBG(" mdmLineSipSubObj->authPassword is NULL");
			zyStrcpy(lineSipEventSubObj->authPassword, "");
		} else {
			zyStrncpy(lineSipEventSubObj->authPassword, mdmLineSipSubObj->authPassword, min(129, zyStrlen(mdmLineSipSubObj->authPassword)+1));
		}			

		/* Free object */
		cmsObj_free((void **) &mdmLineSipSubObj);	
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineRingerCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineRingerCfg( voiceParams_t *parms ,VoiceProfLineRingerObj_t * lineRingerObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineRingerObject *mdmLineRingerObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  LineItr   = parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLineRinger object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineRingerObj)) != CMSRET_SUCCESS)||(lineRingerObj == NULL) ) 	   {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLineRinger object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
		TR104_DBG("MDMOID_VOICE_LINE_RINGER");
		displayIIDStack(&iidStack);   
		lineRingerObj->eventNumberOfEntries = mdmLineRingerObj->eventNumberOfEntries;
		lineRingerObj->descriptionNumberOfEntries = mdmLineRingerObj->descriptionNumberOfEntries;
		lineRingerObj->patternNumberOfEntries = mdmLineRingerObj->patternNumberOfEntries;

		/* Free object */
		cmsObj_free((void **) &mdmLineRingerObj);	
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineRingerEventCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineRingerEventCfg( voiceParams_t *parms ,VoiceProfLineRingerEventObj_t *ringerEventObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineRingerEventObject *mdmRingerEventObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  LineItr   = parms->op[1];
	unsigned int  ringerEventInst  = parms->op[2];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},                                   
                                   {MDMOID_VOICE_LINE_RINGER_EVENT, ringerEventInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };


	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLineRingerEvent object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmRingerEventObj)) != CMSRET_SUCCESS)||(ringerEventObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLineRingerEvent object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_RINGER_EVENT");
		displayIIDStack(&iidStack);

		ringerEventObj->function = tr104ConvertRingerEventFunction(mdmRingerEventObj->function);
		ringerEventObj->ringID = mdmRingerEventObj->ringID;

		/* Free object */
		cmsObj_free((void **) &mdmRingerEventObj);	
	}
	return (ret);
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineRingerDescriptionCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineRingerDescriptionCfg( voiceParams_t *parms ,VoiceProfLineRingerDescriptionObj_t * ringerDescriptionObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineRingerDescriptionObject *mdmRingerDesciptionObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  LineItr   = parms->op[1];
	unsigned int  descriptionInst  = parms->op[2];

	 levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER_DESCRIPTION, descriptionInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get RingerDesciptionObj  */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmRingerDesciptionObj)) != CMSRET_SUCCESS)||(ringerDescriptionObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve RingerDesciption object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   
		TR104_DBG("MDMOID_VOICE_LINE_RINGER_DESCRIPTION");
		displayIIDStack(&iidStack);
	   
		ringerDescriptionObj->entryID = mdmRingerDesciptionObj->entryID;
		ringerDescriptionObj->ringEnable = mdmRingerDesciptionObj->ringEnable;
		ringerDescriptionObj->ringPattern = mdmRingerDesciptionObj->ringPattern;
		
	   	if(mdmRingerDesciptionObj->ringName == NULL){
			TR104_DBG(" mdmRingerDesciptionObj->ringName is NULL");
			zyStrcpy(ringerDescriptionObj->ringName, "");
		} else {
			zyStrncpy(ringerDescriptionObj->ringName, mdmRingerDesciptionObj->ringName, min(65, zyStrlen(mdmRingerDesciptionObj->ringName)+1));
		}	

		if(mdmRingerDesciptionObj->ringFile == NULL){
			TR104_DBG(" mdmRingerEventObj->ringFile is NULL");
			zyStrcpy(ringerDescriptionObj->ringFile, "");
		} else {
			zyStrncpy(ringerDescriptionObj->ringFile, mdmRingerDesciptionObj->ringFile, min(257, zyStrlen(mdmRingerDesciptionObj->ringFile)+1));
		}	

		/* Free object */
		cmsObj_free((void **) &mdmRingerDesciptionObj);	
	}
	return (ret);
	
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineRingerPatternCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineRingerPatternCfg( voiceParams_t *parms ,VoiceProfLineRingerPatternObj_t * ringerPatternObj)
{
	CmsRet                 ret;
	InstanceIdStack        iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineRingerPatternObject *mdmRingerPatternObj     = NULL;
//Ssu-Ying, support Multiple Profile
	unsigned int  VpItr   = parms->op[0];
	unsigned int  LineItr   = parms->op[1];
	unsigned int  ringerPatternInst  = parms->op[2];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_RINGER_PATTERN, ringerPatternInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLineRingerPattern object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmRingerPatternObj)) != CMSRET_SUCCESS)||(ringerPatternObj == NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve RingerPattern object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   
		TR104_DBG("MDMOID_VOICE_LINE_RINGER_PATTERN");
		displayIIDStack(&iidStack);  

		ringerPatternObj->entryID = mdmRingerPatternObj->entryID;
		ringerPatternObj->ringerOn = mdmRingerPatternObj->ringerOn;
		ringerPatternObj->duration = mdmRingerPatternObj->duration;
		ringerPatternObj->nextEntryID = mdmRingerPatternObj->nextEntryID;
	
		/* Free object */
		cmsObj_free((void **) &mdmRingerPatternObj);	
	}
	return (ret);
}
#endif

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineCallingFeaturesCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineCallingFeaturesCfg( voiceParams_t *parms  ,VoiceProfLineCallingFeaturesObj_t * lineCallingFeaturesObj)
{
//Ssu-Ying, support Multiple Profile
	unsigned int       VpItr   = parms->op[0];
	unsigned int       LineItr = parms->op[1];
	CmsRet             ret;
	InstanceIdStack    iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineCallingFeaturesObject *mdmVoiceLineCallingFeaturesObj = NULL;
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_CALLING_FEATURES, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };
	VoiceProfLineObj_t * lineObj = tr104VoiceLineObjPtrGet(VpItr-1, LineItr-1);

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmVoiceLineCallingFeaturesObj)) != CMSRET_SUCCESS) || (lineCallingFeaturesObj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve calling features object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_CALLING_FEATURES");
		displayIIDStack(&iidStack);
		
		lineCallingFeaturesObj->callerIDEnable =mdmVoiceLineCallingFeaturesObj->callerIDEnable ;
		
	   	if(mdmVoiceLineCallingFeaturesObj->callerIDName == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callerIDName is NULL");
			if(NULL!=lineObj && zyStrlen(lineObj->directoryNumber)){
				zyStrcpy(lineCallingFeaturesObj->callerIDName ,lineObj->directoryNumber);
			}
			else{
				zyStrcpy(lineCallingFeaturesObj->callerIDName ,"CHANGEME");
			}
			
		} else {
			zyStrncpy(lineCallingFeaturesObj->callerIDName ,mdmVoiceLineCallingFeaturesObj->callerIDName, min(257, zyStrlen(mdmVoiceLineCallingFeaturesObj->callerIDName)+1));
		}			
		
		lineCallingFeaturesObj->callerIDNameEnable= mdmVoiceLineCallingFeaturesObj->callerIDNameEnable ;
		

		lineCallingFeaturesObj->callWaitingEnable = mdmVoiceLineCallingFeaturesObj->callWaitingEnable ;
		
		lineCallingFeaturesObj->callWaitingStatus = tr104ConvertCWStatus(mdmVoiceLineCallingFeaturesObj->callWaitingStatus);
		lineCallingFeaturesObj->maxSessions = mdmVoiceLineCallingFeaturesObj->maxSessions ;
		if(lineCallingFeaturesObj->maxSessions <=0){
			lineCallingFeaturesObj->maxSessions = tr104GetMaxSessionsPerLine();
		}
	
		lineCallingFeaturesObj->conferenceCallingStatus= tr104ConvertConfCallStatus(mdmVoiceLineCallingFeaturesObj->conferenceCallingStatus)  ;
		lineCallingFeaturesObj->conferenceCallingSessionCount = mdmVoiceLineCallingFeaturesObj->conferenceCallingSessionCount;

		lineCallingFeaturesObj->callForwardUnconditionalEnable = mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalEnable ;
		lineCallingFeaturesObj->callForwardOnBusyEnable = mdmVoiceLineCallingFeaturesObj->callForwardOnBusyEnable ;
		lineCallingFeaturesObj->callForwardOnNoAnswerEnable = mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerEnable ;
		lineCallingFeaturesObj->callForwardOnNoAnswerRingCount=mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerRingCount ;

	   	if(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber is NULL");
			zyStrcpy(lineCallingFeaturesObj->callForwardUnconditionalNumber ,"");
		} else {
			zyStrncpy(lineCallingFeaturesObj->callForwardUnconditionalNumber ,mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber, min(33, zyStrlen(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber)+1));
		}	
	   	if(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber is NULL");
			zyStrcpy(lineCallingFeaturesObj->callForwardOnBusyNumber ,"");
		} else {
			zyStrncpy(lineCallingFeaturesObj->callForwardOnBusyNumber ,mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber, min(33, zyStrlen(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber)+1));
		}	

	   	if(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber is NULL");
			zyStrcpy(lineCallingFeaturesObj->callForwardOnNoAnswerNumber ,"");
		} else {
			zyStrncpy(lineCallingFeaturesObj->callForwardOnNoAnswerNumber ,mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber, min(33, zyStrlen(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber)+1));
		}			

#ifdef SUPPORT_NEW_OUI
		lineCallingFeaturesObj->callTransferEnable = mdmVoiceLineCallingFeaturesObj->callTransferEnable ;
		lineCallingFeaturesObj->ZyXEL_callTransferAfterConference = mdmVoiceLineCallingFeaturesObj->X_5067F0_CallTransferAfterConference;	//callTransferAfterConference
		lineCallingFeaturesObj->ZyXEL_warmLineEnable = mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineEnable;	//warmLineEnable
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_warmLineNum, "");	//warmLineNum
		}
		else{
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_warmLineNum, mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber, 32+1);	//warmLineNumber
		}
		lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine = mdmVoiceLineCallingFeaturesObj->X_5067F0_TimerFirstDigitWarmLine;
		lineCallingFeaturesObj->ZyXEL_hotLineEnable = mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineEnable;	//hotLineEnable
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->ZyXEL_hotLineNum is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_hotLineNum, "");	//hotLineNum
		}
		else{
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_hotLineNum, mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum, 32+1);	//hotLineNumber
		}

		lineCallingFeaturesObj->ZyXEL_missedCallEmailEnable = mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailEnable;
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailServerAddr){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailServerAddr, mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailServerAddr);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailToAddr){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailAddr, mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailToAddr);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailTitle){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailTitle, mdmVoiceLineCallingFeaturesObj->X_5067F0_MissedCallEmailTitle);
		}

		lineCallingFeaturesObj->ZyXEL_CCBSEnable = mdmVoiceLineCallingFeaturesObj->X_5067F0_CCBSEnable;
		
#if 1	/* aircheng add */
		lineCallingFeaturesObj->ZyXEL_remoteRingbackTone = mdmVoiceLineCallingFeaturesObj->X_5067F0_remoteRingbackTone ;
		lineCallingFeaturesObj->ZyXEL_remoteRingbackToneIndex = mdmVoiceLineCallingFeaturesObj->X_5067F0_remoteRingbackToneIndex;
		lineCallingFeaturesObj->ZyXEL_musicOnHoldTone = mdmVoiceLineCallingFeaturesObj->X_5067F0_musicOnHoldTone ;
		lineCallingFeaturesObj->ZyXEL_musicOnHoldToneIndex = mdmVoiceLineCallingFeaturesObj->X_5067F0_musicOnHoldToneIndex;
#endif
		lineCallingFeaturesObj->MWIEnable = mdmVoiceLineCallingFeaturesObj->MWIEnable ;
		lineCallingFeaturesObj->ZyXEL_mwiExpireTime = mdmVoiceLineCallingFeaturesObj->X_5067F0_mwiExpireTime ;
#else
		lineCallingFeaturesObj->callTransferEnable = mdmVoiceLineCallingFeaturesObj->callTransferEnable ;
		lineCallingFeaturesObj->ZyXEL_callTransferAfterConference = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallTransferAfterConference;	//callTransferAfterConference
		lineCallingFeaturesObj->ZyXEL_warmLineEnable = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineEnable;	//warmLineEnable
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_warmLineNum, "");	//warmLineNum
		}
		else{
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_warmLineNum, mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum, 32+1);	//warmLineNumber
		}
		lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_TimerFirstDigitWarmLine;
		lineCallingFeaturesObj->ZyXEL_hotLineEnable = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineEnable;	//hotLineEnable
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->ZyXEL_hotLineNum is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_hotLineNum, "");	//hotLineNum
		}
		else{
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_hotLineNum, mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum, 32+1);	//hotLineNumber
		}

		lineCallingFeaturesObj->ZyXEL_missedCallEmailEnable = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailEnable;
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailServerAddr){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailServerAddr, mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailServerAddr);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailToAddr){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailAddr, mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailToAddr);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailTitle){
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_missedCallEmailTitle, mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_MissedCallEmailTitle);
		}

		lineCallingFeaturesObj->ZyXEL_CCBSEnable = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CCBSEnable;
		
#if 1	/* aircheng add */
		lineCallingFeaturesObj->ZyXEL_remoteRingbackTone = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_remoteRingbackTone ;
		lineCallingFeaturesObj->ZyXEL_remoteRingbackToneIndex = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_remoteRingbackToneIndex;
		lineCallingFeaturesObj->ZyXEL_musicOnHoldTone = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_musicOnHoldTone ;
		lineCallingFeaturesObj->ZyXEL_musicOnHoldToneIndex = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_musicOnHoldToneIndex;
#endif
		lineCallingFeaturesObj->MWIEnable = mdmVoiceLineCallingFeaturesObj->MWIEnable ;
		lineCallingFeaturesObj->ZyXEL_mwiExpireTime = mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_mwiExpireTime ;
#endif	/* SUPPORT_NEW_OUI end */
		#if 0 /*SPR#090909671, readonly*/
		lineCallingFeaturesObj->messageWaiting=mdmVoiceLineCallingFeaturesObj->messageWaiting;
		#endif
		lineCallingFeaturesObj->anonymousCallBlockEnable = mdmVoiceLineCallingFeaturesObj->anonymousCallBlockEnable ;
		lineCallingFeaturesObj->anonymousCallEnable= mdmVoiceLineCallingFeaturesObj->anonymousCallEnable ;
		lineCallingFeaturesObj->doNotDisturbEnable = mdmVoiceLineCallingFeaturesObj->doNotDisturbEnable ;
		lineCallingFeaturesObj->callReturnEnable = mdmVoiceLineCallingFeaturesObj->callReturnEnable ;
		lineCallingFeaturesObj->repeatDialEnable = mdmVoiceLineCallingFeaturesObj->repeatDialEnable ;

		

#ifdef SUPPORT_NEW_OUI
		lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time =mdmVoiceLineCallingFeaturesObj->X_5067F0_CallWaitingRejectTime  ;

	   	if(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->ZyXEL_conferenceURI is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_conferenceURI ,"");
		} else {
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_conferenceURI ,mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI, min(49, zyStrlen(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI)+1));
		}			
#else
		lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time =mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallWaitingRejectTime  ;

	   	if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->ZyXEL_conferenceURI is NULL");
			zyStrcpy(lineCallingFeaturesObj->ZyXEL_conferenceURI ,"");
		} else {
			zyStrncpy(lineCallingFeaturesObj->ZyXEL_conferenceURI ,mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI, min(49, zyStrlen(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI)+1));
		}			
#endif	/* SUPPORT_NEW_OUI end */

	 	cmsObj_free((void **) &mdmVoiceLineCallingFeaturesObj);
		
	}
	return ( ret );
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineProcessingCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineProcessingCfg( voiceParams_t *parms  ,VoiceProfLineVoiceProcessingObj_t * voiceProcessingObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineProcessingObject *mdmVoiceLineProcessingObj = NULL;
//Ssu-Ying, support Multiple Profile
	/* parse Service Provider Number as unsigned int */
	uint32 VpItr = parms->op[0];
	/* parse Account Number as unsigned int */
	uint32 LineItr= parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_PROCESSING, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };


	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoiceLineProcessing object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmVoiceLineProcessingObj)) != CMSRET_SUCCESS)||(voiceProcessingObj==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceLineProcessing object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_PROCESSING");
		displayIIDStack(&iidStack);  	

		voiceProcessingObj->transmitGain =mdmVoiceLineProcessingObj->transmitGain;	/*< TransmitGain */
		voiceProcessingObj->receiveGain =mdmVoiceLineProcessingObj->receiveGain;	/*< ReceiveGain */
		voiceProcessingObj->echoCancellationEnable=mdmVoiceLineProcessingObj->echoCancellationEnable ;	/*< echoCancellationEnable */
		voiceProcessingObj->echoCancellationInUse=mdmVoiceLineProcessingObj->echoCancellationInUse;	/*< echoCancellationInUse */
		voiceProcessingObj->echoCancellationTail =mdmVoiceLineProcessingObj->echoCancellationTail;	/*< echoCancellationTail */

		cmsObj_free((void **) &mdmVoiceLineProcessingObj);		
	}
	return ( ret );
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineCodecCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineCodecCfg( voiceParams_t *parms  ,VoiceProfLineCodecObj_t * lineCodecObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineCodecObject *mdmLineCodecObj = NULL;
//Ssu-Ying, support Multiple Profile
	/* parse Service Provider Number as unsigned int */
	uint32 VpItr = parms->op[0];
	/* parse Account Number as unsigned int */
	uint32 LineItr= parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_CODEC, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);
	} else {
		/*  Get the LineCodec object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineCodecObj)) != CMSRET_SUCCESS)||(lineCodecObj==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve LineCodec object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();
	   	
		TR104_DBG("MDMOID_VOICE_LINE_CODEC");
		displayIIDStack(&iidStack);  

		if(mdmLineCodecObj->transmitCodec == NULL){
			TR104_DBG(" mdmLineCodecObj->transmitCodec is NULL");
			zyStrcpy(lineCodecObj->transmitCodec ,"");
		} else {
			zyStrncpy(lineCodecObj->transmitCodec, mdmLineCodecObj->transmitCodec, min(64, zyStrlen(mdmLineCodecObj->transmitCodec)+1));
		}	
		if(mdmLineCodecObj->receiveCodec == NULL){
			TR104_DBG(" mdmLineCodecObj->receiveCodec is NULL");
			zyStrcpy(lineCodecObj->receiveCodec ,"");
		} else {
			zyStrncpy(lineCodecObj->receiveCodec, mdmLineCodecObj->receiveCodec, min(64, zyStrlen(mdmLineCodecObj->receiveCodec)+1));
		}	

		lineCodecObj->transmitBitRate = mdmLineCodecObj->transmitBitRate;
		lineCodecObj->receiveBitRate = mdmLineCodecObj->receiveBitRate;
		/*Whether or not silence suppression is in use for the outgoing voice stream.*/
		lineCodecObj->transmitSilenceSuppression = mdmLineCodecObj->transmitSilenceSuppression;
		/*Whether or not silence suppression is in use for the incoming voice stream.*/
		lineCodecObj->receiveSilenceSuppression = mdmLineCodecObj->receiveSilenceSuppression;

		lineCodecObj->transmitPacketizationPeriod = mdmLineCodecObj->transmitPacketizationPeriod;
		
		cmsObj_free((void **) &mdmLineCodecObj);
		
	}
	return ( ret );
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineCodecListCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineCodecListCfg( voiceParams_t *parms  ,VoiceProfLineCodecListObj_t * lineCodecListObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineCodecListObject *mdmLineCodecListObj = NULL;
//Ssu-Ying, support Multiple Profile
	/* parse Service Provider Number as unsigned int */
	uint32 VpItr = parms->op[0];
	/* parse Account Number as unsigned int */
	uint32 LineItr= parms->op[1];
	uint32 lineCodecInst= parms->op[2];
   
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_CODEC, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_CODEC_LIST, lineCodecInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };


	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the LineCodecList object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineCodecListObj)) != CMSRET_SUCCESS)||(lineCodecListObj==NULL) ) {
			cmsLck_releaseLock();
		/*   cmsLog_error( "Can't retrieve LineCodecList object \n" );*/
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_CODEC_LIST");
		displayIIDStack(&iidStack);  

		lineCodecListObj->entryID = tr104ConvertCodecEntryID(mdmLineCodecListObj->entryID) ;
		
		if(mdmLineCodecListObj->codec == NULL){
			TR104_DBG(" mdmLineCodecListObj->codec is NULL");
			zyStrcpy(lineCodecListObj->codec ,"");
		} else {
			zyStrncpy(lineCodecListObj->codec, mdmLineCodecListObj->codec, min(65, zyStrlen(mdmLineCodecListObj->codec)+1));
		}
		lineCodecListObj->bitRate = mdmLineCodecListObj->bitRate ;
		
		if(mdmLineCodecListObj->packetizationPeriod == NULL){
			TR104_DBG(" lineCodecListObj->packetizationPeriod is NULL");
			zyStrcpy(lineCodecListObj->packetizationPeriod ,"");
		} else {
			zyStrncpy(lineCodecListObj->packetizationPeriod, mdmLineCodecListObj->packetizationPeriod, min(65, zyStrlen(mdmLineCodecListObj->packetizationPeriod)+1));
		}	
		
		lineCodecListObj->silenceSuppression = mdmLineCodecListObj->silenceSuppression ;
		lineCodecListObj->enable = mdmLineCodecListObj->enable ;
		lineCodecListObj->priority = mdmLineCodecListObj->priority ;

		cmsObj_free((void **) &mdmLineCodecListObj);
		
	}
	return ( ret );
}
/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineSessionCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineSessionCfg( voiceParams_t *parms  ,VoiceProfLineSessionObj_t * lineSessionObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineSessionObject *mdmLineSessionObj = NULL;
//Ssu-Ying, support Multiple Profile
	/* parse Service Provider Number as unsigned int */
	uint32 VpItr = parms->op[0];
	/* parse Account Number as unsigned int */
	uint32 LineItr= parms->op[1];
	uint32 sessionInst= parms->op[2];
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_SESSION,sessionInst , EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the LineSession object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineSessionObj)) != CMSRET_SUCCESS)||(lineSessionObj==NULL) ) {
			cmsLck_releaseLock();
			/*cmsLog_error( "Can't retrieve LineSession object \n");*/
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_SESSION");
		displayIIDStack(&iidStack);  

		lineSessionObj->sessionStartTime=mdmLineSessionObj->sessionStartTime ;
		lineSessionObj->sessionDuration=mdmLineSessionObj->sessionDuration ;
		lineSessionObj->farEndUDPPort=mdmLineSessionObj->farEndUDPPort ;
		lineSessionObj->localUDPPort=mdmLineSessionObj->localUDPPort ;

	   	if(mdmLineSessionObj->farEndIPAddress == NULL){
			TR104_DBG(" mdmVoiceLineCallingFeaturesObj->ZyXEL_conferenceURI is NULL");
			zyStrcpy(lineSessionObj->farEndIPAddress, "");
		} else {
			zyStrncpy(lineSessionObj->farEndIPAddress, mdmLineSessionObj->farEndIPAddress, min(65, zyStrlen(mdmLineSessionObj->farEndIPAddress)+1));
		}

		cmsObj_free((void **) &mdmLineSessionObj);
	}
	return ( ret );
}
#if 0
/***************************************************************************
** Function Name: tr104LoadMdmVoiceLineStatsCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceLineStatsCfg( voiceParams_t *parms  ,VoiceProfLineStatsObj_t * lineStatsObj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceLineStatsObject *mdmLineStatsObj = NULL;

	/* parse Service Provider Number as unsigned int */
	uint32 vpInst = parms->op[0];
	/* parse Account Number as unsigned int */
	uint32 lineInst= parms->op[1];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, vpInst, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE, lineInst, EMPTY_INSTANCE_ID_STACK},
                                   {MDMOID_VOICE_LINE_STATS, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };


	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the LineStats object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmLineStatsObj)) != CMSRET_SUCCESS)||(lineStatsObj==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve LineStats object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_LINE_STATS");
		displayIIDStack(&iidStack);  

		cmsObj_free((void **) &mdmLineStatsObj);
	}
	return ( ret );
}
#endif
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104LoadMdmVoicePhyIntfCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoicePhyIntfCfg( voiceParams_t *parms  ,VoicePhyIntfObj_t * tr104Obj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_PhyIntfObject *mdmObj = NULL;

	/* parse Service Provider Number as unsigned int */
	uint32 phyIntfInst = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_PHY_INTF, phyIntfInst, EMPTY_INSTANCE_ID_STACK},
                                   {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		/*  Get the VoicePhyIntf object */
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmObj)) != CMSRET_SUCCESS)||(tr104Obj==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve  VoicePhyIntf object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_PHY_INTF");
		displayIIDStack(&iidStack);  

		if(mdmObj->phyPort == NULL){
			TR104_DBG(" mdmObj->phyPort is NULL");
			zyStrcpy(tr104Obj->phyPort, "");
		} else {
			zyStrncpy(tr104Obj->phyPort, mdmObj->phyPort, min(3, zyStrlen(mdmObj->phyPort)+1));
		}
		tr104Obj->interfaceID=mdmObj->interfaceID ;		
	   	if(mdmObj->description == NULL){
			TR104_DBG(" mdmObj->description  is NULL");
			zyStrcpy(tr104Obj->description, "");
		} else {
			zyStrncpy(tr104Obj->description, mdmObj->description, min(33, zyStrlen(mdmObj->description)+1));
		}		
		
#ifdef SUPPORT_NEW_OUI
		/* ZyXEL FXS Setting START*/
		tr104Obj->ZyXEL_PotsType = tr104ConvertPortsType(mdmObj->X_5067F0_PotsType);
		tr104Obj->ZyXEL_DialingType = tr104ConvertDialingType(mdmObj->X_5067F0_DialingType);
		tr104Obj->ZyXEL_DialToneType = tr104ConvertDialToneType(mdmObj->X_5067F0_DialToneType);
		tr104Obj->ZyXEL_ReceiveToneType = tr104ConvertReceiveToneType(mdmObj->X_5067F0_ReceiveToneType);
		if( (mdmObj->X_5067F0_FlashMinInterval > 0) &&			
			(mdmObj->X_5067F0_FlashMaxInterval > 0) &&
			((tr104Obj->ZyXEL_FlashMinInterval != mdmObj->X_5067F0_FlashMinInterval) \
			|| (tr104Obj->ZyXEL_FlashMaxInterval != mdmObj->X_5067F0_FlashMaxInterval)) &&
			(mdmObj->X_5067F0_FlashMaxInterval > mdmObj->X_5067F0_FlashMinInterval))
		{
			ZyPrintf("flash interval changed %d-%d to %d-%d, dsp reinit needed\n", 
				tr104Obj->ZyXEL_FlashMinInterval,
				tr104Obj->ZyXEL_FlashMaxInterval,
				mdmObj->X_5067F0_FlashMinInterval,
				mdmObj->X_5067F0_FlashMaxInterval
				);
			dspReinitNeeded = 1;
		}
		tr104Obj->ZyXEL_FlashMaxInterval = mdmObj->X_5067F0_FlashMaxInterval;
		tr104Obj->ZyXEL_FlashMinInterval = mdmObj->X_5067F0_FlashMinInterval;
		tr104Obj->ZyXEL_JitterBufSize = mdmObj->X_5067F0_JitterBufSize;
		tr104Obj->ZyXEL_CallerIDType = tr104ConvertCallerIDType(mdmObj->X_5067F0_CallerIDType);
		tr104Obj->ZyXEL_CallerIDPayload = tr104ConvertCallerIDPayloadType(mdmObj->X_5067F0_CallerIDPayload);
		tr104Obj->ZyXEL_TasTimeoutInterval = mdmObj->X_5067F0_TasTimeoutInterval;
		tr104Obj->ZyXEL_TasTimeoutInterval_2 = mdmObj->X_5067F0_TasTimeoutInterval_2;
		tr104Obj->ZyXEL_CallerIDRingTimeout = mdmObj->X_5067F0_CallerIDRingTimeout;
		tr104Obj->ZyXEL_FirstRingTimoutInterval = mdmObj->X_5067F0_FirstRingTimoutInterval;
		tr104Obj->ZyXEL_FirstTASType = tr104ConvertTASType(mdmObj->X_5067F0_FirstTASType);
		tr104Obj->ZyXEL_SecondTASType = tr104ConvertTASType(mdmObj->X_5067F0_SecondTASType);
		tr104Obj->ZyXEL_CallerID_DTAS_ACK_Timeout = mdmObj->X_5067F0_CallerID_DTAS_ACK_Timeout;
		tr104Obj->ZyXEL_LineSelect = mdmObj->X_5067F0_LineSelect;		
		tr104Obj->ZyXEL_MsnIncomingSelect = mdmObj->X_5067F0_MsnIncomingSelect;
		tr104Obj->ZyXEL_MsnOutgoingSelect = mdmObj->X_5067F0_MsnOutgoingSelect;

		if(0 == mdmObj->X_5067F0_HowlerToneTimeout){
			tr104Obj->ZyXEL_HowlerToneTimeout = HOWLERTONETIMEOUT_BASIC_VALUE ;
		}else{
			tr104Obj->ZyXEL_HowlerToneTimeout = mdmObj->X_5067F0_HowlerToneTimeout;
		}		
		/* ZyXEL FXS Setting END*/
#else
		/* ZyXEL FXS Setting START*/
		tr104Obj->ZyXEL_PotsType = tr104ConvertPortsType(mdmObj->X_ZyXEL_COM_PotsType);
		tr104Obj->ZyXEL_DialingType = tr104ConvertDialingType(mdmObj->X_ZyXEL_COM_DialingType);
		tr104Obj->ZyXEL_DialToneType = tr104ConvertDialToneType(mdmObj->X_ZyXEL_COM_DialToneType);
		tr104Obj->ZyXEL_ReceiveToneType = tr104ConvertReceiveToneType(mdmObj->X_ZyXEL_COM_ReceiveToneType);
		if( (mdmObj->X_ZyXEL_COM_FlashMinInterval > 0) &&			
			(mdmObj->X_ZyXEL_COM_FlashMaxInterval > 0) &&
			((tr104Obj->ZyXEL_FlashMinInterval != mdmObj->X_ZyXEL_COM_FlashMinInterval) \
			|| (tr104Obj->ZyXEL_FlashMaxInterval != mdmObj->X_ZyXEL_COM_FlashMaxInterval)) &&
			(mdmObj->X_ZyXEL_COM_FlashMaxInterval > mdmObj->X_ZyXEL_COM_FlashMinInterval))
		{
			ZyPrintf("flash interval changed %d-%d to %d-%d, dsp reinit needed\n", 
				tr104Obj->ZyXEL_FlashMinInterval,
				tr104Obj->ZyXEL_FlashMaxInterval,
				mdmObj->X_ZyXEL_COM_FlashMinInterval,
				mdmObj->X_ZyXEL_COM_FlashMaxInterval
				);
			dspReinitNeeded = 1;
		}
		tr104Obj->ZyXEL_FlashMaxInterval = mdmObj->X_ZyXEL_COM_FlashMaxInterval;
		tr104Obj->ZyXEL_FlashMinInterval = mdmObj->X_ZyXEL_COM_FlashMinInterval;
		tr104Obj->ZyXEL_JitterBufSize = mdmObj->X_ZyXEL_COM_JitterBufSize;
		tr104Obj->ZyXEL_CallerIDType = tr104ConvertCallerIDType(mdmObj->X_ZyXEL_COM_CallerIDType);
		tr104Obj->ZyXEL_CallerIDPayload = tr104ConvertCallerIDPayloadType(mdmObj->X_ZyXEL_COM_CallerIDPayload);
		tr104Obj->ZyXEL_TasTimeoutInterval = mdmObj->X_ZyXEL_COM_TasTimeoutInterval;
		tr104Obj->ZyXEL_TasTimeoutInterval_2 = mdmObj->X_ZyXEL_COM_TasTimeoutInterval_2;
		tr104Obj->ZyXEL_CallerIDRingTimeout = mdmObj->X_ZyXEL_COM_CallerIDRingTimeout;
		tr104Obj->ZyXEL_FirstRingTimoutInterval = mdmObj->X_ZyXEL_COM_FirstRingTimoutInterval;
		tr104Obj->ZyXEL_FirstTASType = tr104ConvertTASType(mdmObj->X_ZyXEL_COM_FirstTASType);
		tr104Obj->ZyXEL_SecondTASType = tr104ConvertTASType(mdmObj->X_ZyXEL_COM_SecondTASType);
		tr104Obj->ZyXEL_CallerID_DTAS_ACK_Timeout = mdmObj->X_ZyXEL_COM_CallerID_DTAS_ACK_Timeout;
		tr104Obj->ZyXEL_LineSelect = mdmObj->X_ZyXEL_COM_LineSelect;		
		tr104Obj->ZyXEL_MsnIncomingSelect = mdmObj->X_ZyXEL_COM_MsnIncomingSelect;
		tr104Obj->ZyXEL_MsnOutgoingSelect = mdmObj->X_ZyXEL_COM_MsnOutgoingSelect;

		if(0 == mdmObj->X_ZyXEL_COM_HowlerToneTimeout){
			tr104Obj->ZyXEL_HowlerToneTimeout = HOWLERTONETIMEOUT_BASIC_VALUE ;
		}else{
			tr104Obj->ZyXEL_HowlerToneTimeout = mdmObj->X_ZyXEL_COM_HowlerToneTimeout;
		}		
		/* ZyXEL FXS Setting END*/
#endif	/* SUPPORT_NEW_OUI end */


		cmsObj_free((void **) &mdmObj);
	}
	return ( ret );
}


/***************************************************************************
** Function Name: tr104LoadMdmVoiceFXOCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceFXOCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXO_t * tr104Obj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceFxoObject *mdmObj = NULL;

	/* parse Service Provider Number as unsigned int */
	uint32 fxoInst = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS]    = { {MDMOID_VOICE_FXO, fxoInst, EMPTY_INSTANCE_ID_STACK},
                                      {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	/*! \brief InternetGatewayDevice.Services.VoiceService.{i}.X_ZyXEL_COM_FXO. */

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		if (( (ret = getMdmObj(info, &iidStack, (void**)&mdmObj)) != CMSRET_SUCCESS)||(tr104Obj==NULL) ) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve VoiceFXO object \n" );
			return ( ret );
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_FXO");
		displayIIDStack(&iidStack);

		tr104Obj->numberOfPort = mdmObj->numberOfPort;

		cmsObj_free((void **) &mdmObj);
	}
	return ( ret );
}


/***************************************************************************
** Function Name: tr104LoadMdmVoicePhyIntfFXOCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceFXOPortCfg( voiceParams_t *parms  ,ZyXEL_VoiceFXOPort_t * tr104Obj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack = EMPTY_INSTANCE_ID_STACK;
	_VoiceFxoPortObject *mdmObj = NULL;

	/* parse Service Provider Number as unsigned int */
	uint32 fxoPortInst = parms->op[0];

	levelInfo_t   info[_VOICE_MAXLEVELS]    = { {MDMOID_VOICE_FXO_PORT, fxoPortInst, EMPTY_INSTANCE_ID_STACK},
                                      {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	/*! \brief InternetGatewayDevice.Services.VoiceService.{i}.X_ZyXEL_COM_FXO.Port.{i}. */

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);	
	} else {
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdmObj)) != CMSRET_SUCCESS) ||(tr104Obj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_FXO");
		displayIIDStack(&iidStack);

		tr104Obj->fxoDialLongInterval = mdmObj->fxoDialLongInterval;

		if(0 == mdmObj->fxoPhoneSelect){
			ZyPrintf("\r\n TR104 WARNING : invalid fxoPhoneSelect value \r\n");		
			tr104Obj->fxoPhoneSelect = FXOPHONESELECT_BASIC_VALUE;/* default select the first fxs*/
		}else{	
			tr104Obj->fxoPhoneSelect = mdmObj->fxoPhoneSelect;
		}
		
		tr104Obj->fxoDTMFPauseDuration = mdmObj->fxoDTMFPauseDuration;
		tr104Obj->fxoFlashMinInterval = mdmObj->fxoFlashMinInterval;
		tr104Obj->fxoFlashMaxInterval = mdmObj->fxoFlashMaxInterval;
		tr104Obj->fxoLifeLineStableTime = mdmObj->fxoLifeLineStableTime;
		tr104Obj->fxoDTMFDigitDuration = mdmObj->fxoDTMFDigitDuration;
		tr104Obj->reversed2 = mdmObj->reversed2;
		tr104Obj->pollingPollarityTime = mdmObj->pollingPollarityTime;
		tr104Obj->detectCIDInterval = mdmObj->detectCIDInterval;

		cmsObj_free((void **) &mdmObj);
	}
	return ( ret );
}

/***************************************************************************
** Function Name: tr104LoadMdmVoiceZYXELPstnCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

CmsRet 
tr104LoadMdmVoiceZYXELPstnCfg( voiceParams_t *parms  ,ZyXEL_VoicePSTN_t * tr104Obj)
{
	CmsRet              ret;
	InstanceIdStack     iidStack   = EMPTY_INSTANCE_ID_STACK;
#ifdef SUPPORT_NEW_OUI
	_VoicePstnObject    *mdmObj    = NULL;

	unsigned int        pstnInst     = parms->op[0];
	levelInfo_t   info[_VOICE_MAXLEVELS]    = { {MDMOID_VOICE_PSTN, pstnInst, EMPTY_INSTANCE_ID_STACK},
                                      {0, 0, EMPTY_INSTANCE_ID_STACK}, };
#else
	_VoiceZYXELPstnObject    *mdmObj    = NULL;

	unsigned int        pstnInst     = parms->op[0];
	levelInfo_t   info[_VOICE_MAXLEVELS]    = { {MDMOID_ZYXEL_PSTN, pstnInst, EMPTY_INSTANCE_ID_STACK},
                                      {0, 0, EMPTY_INSTANCE_ID_STACK}, };
#endif	/* SUPPORT_NEW_OUI end */

		
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {
		cmsLck_releaseLock();
		cmsLog_error( "Can't get lock \n" );
		return (ret);	
	} else {
		if (((ret = getMdmObj(info, &iidStack, (void**)&mdmObj)) != CMSRET_SUCCESS) ||(tr104Obj==NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );	
			return (ret);
		}

		cmsLck_releaseLock();

		TR104_DBG("MDMOID_ZYXEL_PSTN");
		displayIIDStack(&iidStack);
		
	   	if(mdmObj->forcePSTNTab == NULL){
			TR104_DBG(" mdmObj->description  is NULL");
			zyStrcpy(tr104Obj->forcePSTNTab, "");
		} else {
			zyStrncpy(tr104Obj->forcePSTNTab, mdmObj->forcePSTNTab, min(PSTN_PHONE_NUM_LEN, zyStrlen(mdmObj->forcePSTNTab)+1));
		}	

	   	if(mdmObj->reversed == NULL){
			TR104_DBG(" mdmObj->description  is NULL");
			zyStrcpy(tr104Obj->reversed, "");
		} else {
			zyStrncpy(tr104Obj->reversed, mdmObj->reversed, min(2, zyStrlen(mdmObj->reversed)+1));
		}

		tr104Obj->flags =mdmObj->flags ;

		cmsObj_free((void **) &mdmObj);
	}

	return (ret);
}
/***************************************************************************
** Function Name: tr104LoadMdmVoicePhoneBookCfg
**
** Description  : load config from the corresponding object in MDM
**                           
** Returns      : 
****************************************************************************/

/*! \brief InternetGatewayDevice.Services.VoiceService.{i}.X_ZYXEL_COM_VoicePhoneBook.{i}. */
CmsRet 
tr104LoadMdmVoicePhoneBookCfg( voiceParams_t *parms ,ZyXEL_VoicePhoneBook_t * tr104Obj)
{
	CmsRet      ret;
	unsigned int    PBInst   = parms->op[0] ;
	_VoicePhoneBookObject*   mdmObj = NULL;
	InstanceIdStack   iidStack = EMPTY_INSTANCE_ID_STACK;
	levelInfo_t   info[_VOICE_MAXLEVELS]    = {  {MDMOID_VOICE_PHONE_BOOK, PBInst, EMPTY_INSTANCE_ID_STACK},
                                       {0, 0, EMPTY_INSTANCE_ID_STACK}, };

	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock \n" );		
		return (ret);
	} else {
		if ((( ret = getMdmObj(info, &iidStack, (void**)&mdmObj)) != CMSRET_SUCCESS)||( tr104Obj == NULL)) {
			cmsLck_releaseLock();
			cmsLog_error( "Can't retrieve object\n" );
			return (ret);
		}
		cmsLck_releaseLock();

		TR104_DBG("MDMOID_VOICE_PHONE_BOOK");
		displayIIDStack(&iidStack);     
		
	   	if(mdmObj->origNumber == NULL){
			TR104_DBG(" mdmObj->origNumber  is NULL");
			zyStrcpy(tr104Obj->origNumber, "");
		} else {
			zyStrncpy(tr104Obj->origNumber, mdmObj->origNumber, min(POTS_PORT_NAME_LEN, zyStrlen(mdmObj->origNumber)+1));
		}	

	   	if(mdmObj->forceSipURI == NULL){
			TR104_DBG(" mdmObj->forceSipURI  is NULL");
			zyStrcpy(tr104Obj->forceSipURI, "");
		} else {
			zyStrncpy(tr104Obj->forceSipURI, mdmObj->forceSipURI, min(SIP_DOMAIN_NAME_LEN, zyStrlen(mdmObj->forceSipURI)+1));
		}	

	   	if(mdmObj->speedNumber == NULL){
			TR104_DBG(" mdmObj->speedNumber  is NULL");
			zyStrcpy(tr104Obj->speedNumber, "");
		} else {
			zyStrncpy(tr104Obj->speedNumber, mdmObj->speedNumber, min(POTS_PORT_NAME_LEN, zyStrlen(mdmObj->speedNumber)+1));
		}	

	   	if(mdmObj->name == NULL){
			TR104_DBG(" mdmObj->name  is NULL");
			zyStrcpy(tr104Obj->name, "");
		} else {
			zyStrncpy(tr104Obj->name, mdmObj->name, min(SPEED_DIAL_NAME_LEN, zyStrlen(mdmObj->name)+1));
		}
		tr104Obj->flags = tr104ConvertPhoneBookFlag(mdmObj->flags);
		tr104Obj->type = tr104ConvertPhoneBookType(mdmObj->type);

		/* Free object */
		cmsObj_free((void **) &mdmObj);
	}
	return (ret);
}


//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104ResetVoiceLineStats
**
** Description  : reset Voice Line Statistic
**                           
** Returns      : 
****************************************************************************/
int
tr104ResetVoiceLineStats(voiceParams_t *params ){

	int LineItr = 0 , VpItr = 0 ,SipAccountId = 0, lockRet = 0 ,ret = 0 ; 
	VoiceProfLineStatsObj_t *tr104lineStatsObj = NULL; 
	SipAccountId = params->op[1];

	lockRet = tr104AcquireStatsLock();
	
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	tr104lineStatsObj = (VoiceProfLineStatsObj_t *)tr104VoiceLineStatsObjPtrGet(VpItr, LineItr);	
	if(NULL != tr104lineStatsObj){
		zyMemset( tr104lineStatsObj, 0, sizeof(tr104lineStatsObj));
		ret = TR104_SUCCESS;
	}else{
		ZyPrintf("tr104ResetVoiceLineStats :ERROR : LineStstsObj is NULL \n");
		ret = TR104_ERROR;
	}
	tr104ReleaseStatsLock(lockRet);

	return ret ;
}


/***************************************************************************
** Function Name: tr104SetVoiceLineStats
**
** Description  : collect the statistic infomation of the line and set the Line stats to MDM .
**                           
** Returns      : 
****************************************************************************/

int 
tr104SetMdmVoiceLineStats(voiceParams_t *params  ,_VoiceLineStatsObject * mdmLineStatsObj)
{
//Ssu-Ying, support Multiple Profile
	int ret = TR104_ERROR, VpItr=0, LineItr=0 , SipAccountId = 0;
	VoiceProfLineStatsObj_t* lineStatsObj = NULL ; 

	SipAccountId = params->op[1];
	TR104_DBG1("tr104SetMdmVoiceLineStats : SipAccountId = ", SipAccountId);
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	TR104_DBG2("tr104SetMdmVoiceLineStats : VpItr =", VpItr , " LineItr = ", LineItr);	                      
	lineStatsObj = tr104VoiceLineStatsObjPtrGet(VpItr , LineItr);
	
	if(lineStatsObj!= NULL)
	{
	mdmLineStatsObj->packetsSent = lineStatsObj->packetsSent;
	mdmLineStatsObj->packetsReceived= lineStatsObj->packetsReceived;
	mdmLineStatsObj->bytesSent= lineStatsObj->bytesSent ;
	mdmLineStatsObj->bytesReceived= lineStatsObj->bytesReceived ;
	mdmLineStatsObj->bytesReceived= lineStatsObj->bytesReceived ;
	mdmLineStatsObj->packetsLost= lineStatsObj->packetsLost ;
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	mdmLineStatsObj->overruns= lineStatsObj->overruns ;
	mdmLineStatsObj->underruns= lineStatsObj->underruns ;
	#endif

	mdmLineStatsObj->incomingCallsReceived = lineStatsObj->incomingCallsReceived;
	mdmLineStatsObj->incomingCallsAnswered = lineStatsObj->incomingCallsAnswered;
	mdmLineStatsObj->incomingCallsConnected = lineStatsObj->incomingCallsConnected ;
	mdmLineStatsObj->incomingCallsFailed= lineStatsObj->incomingCallsFailed ;
	mdmLineStatsObj->outgoingCallsAttempted = lineStatsObj->outgoingCallsAttempted;	
	mdmLineStatsObj->outgoingCallsAnswered = lineStatsObj->outgoingCallsAnswered;
	mdmLineStatsObj->outgoingCallsConnected = lineStatsObj->outgoingCallsConnected;
	mdmLineStatsObj->outgoingCallsFailed = lineStatsObj->outgoingCallsFailed;
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	mdmLineStatsObj->callsDropped= lineStatsObj->callsDropped;	
	#endif
	mdmLineStatsObj->totalCallTime= lineStatsObj->totalCallTime;
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	mdmLineStatsObj->serverDownTime= lineStatsObj->serverDownTime;
	#endif
	
	mdmLineStatsObj->receivePacketLossRate= lineStatsObj->receivePacketLossRate;
	mdmLineStatsObj->farEndPacketLossRate= lineStatsObj->farEndPacketLossRate;
	
	mdmLineStatsObj->receiveInterarrivalJitter= lineStatsObj->receiveInterarrivalJitter;
	mdmLineStatsObj->farEndInterarrivalJitter= lineStatsObj->farEndInterarrivalJitter;
	mdmLineStatsObj->roundTripDelay= lineStatsObj->roundTripDelay;
	
	mdmLineStatsObj->averageReceiveInterarrivalJitter= lineStatsObj->averageReceiveInterarrivalJitter;
	mdmLineStatsObj->averageFarEndInterarrivalJitter= lineStatsObj->averageFarEndInterarrivalJitter;
	mdmLineStatsObj->averageRoundTripDelay= lineStatsObj->averageRoundTripDelay;
	}
	return ret;
}


/********************************************************************
**  
**
**  API Class : Validation APIs 
**  descriptions:Area of the Validation APIs of TR104 Module
**
**
*********************************************************************/

int 
tr104ValidationRtpPortRange(uint16 portMin ,uint16 portMax)
{
	int ret = TR104_ERROR;
	if((portMin <= portMax)&&((0!=portMin)||(0!=portMax)))
	{
		ret = TR104_SUCCESS;
	}
	return ret ;
}

//Ssu-Ying, support Multiple Profile
int tr104StoreMdmVoiceLineCallingFeaturesCfg(int VpItr, int LineItr, VoiceProfLineCallingFeaturesObj_t *lineCallingFeaturesObj){
	char bConfChange = 0;
	CmsRet			   ret;
	InstanceIdStack    iidStack = EMPTY_INSTANCE_ID_STACK;
	VoiceLineCallingFeaturesObject *mdmVoiceLineCallingFeaturesObj = NULL;
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_VOICE_PROF, VpItr, EMPTY_INSTANCE_ID_STACK},
								   {MDMOID_VOICE_LINE, LineItr, EMPTY_INSTANCE_ID_STACK},
								   {MDMOID_VOICE_LINE_CALLING_FEATURES, _VOICE_ANY_INSTANCE, EMPTY_INSTANCE_ID_STACK},
								   {0, 0, EMPTY_INSTANCE_ID_STACK}, };	
	TR104_PRINTF("%s, VpItr = %d, LineItr = %d\n",__FUNCTION__, VpItr, LineItr);
	
   
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock, ret %d\n", ret );		
		return (ret);	
	} else {	
		if(NULL == lineCallingFeaturesObj){
			TR104_PRINTF("lineCallingFeaturesObj NULL\n");
			return TR104_ERROR;
		}
		if ( (ret = getMdmObj(info, &iidStack, (void**)&mdmVoiceLineCallingFeaturesObj)) != CMSRET_SUCCESS )
	    {
	      cmsLck_releaseLock();
	      cmsLog_error( "Can't retrieve calling features object, ret %d \n", ret );
	      return ( TR104_ERROR );
	    }
			

		TR104_DBG("tr104VoiceLineCallingFeatureObjPtrSet");

		/*1. callerIDEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callerIDEnable != lineCallingFeaturesObj->callerIDEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callerIDEnable = lineCallingFeaturesObj->callerIDEnable;

		/*2. callerIDName*/
		#if 0 /*don't set CallerIDName, we use directoryNumber to replace this field if CallerIDName is NULL when loading config*/
		if(mdmVoiceLineCallingFeaturesObj->callerIDName != NULL ){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->callerIDName);
		}
		mdmVoiceLineCallingFeaturesObj->callerIDName =  cmsMem_strdup (lineCallingFeaturesObj->callerIDName);
		#endif
		
		/*3. callerIDNameEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callerIDNameEnable != lineCallingFeaturesObj->callerIDNameEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callerIDNameEnable = lineCallingFeaturesObj->callerIDNameEnable;
		
		/*4. callWaitingEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callWaitingEnable != lineCallingFeaturesObj->callWaitingEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callWaitingEnable = lineCallingFeaturesObj->callWaitingEnable;
		
		/*5. callWaitingStatus - R*/
		
#ifdef SUPPORT_NEW_OUI
		/*6. ZyXEL_Call_Waiting_Reject_Time*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_CallWaitingRejectTime != lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_CallWaitingRejectTime = lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time;
#else
		/*6. ZyXEL_Call_Waiting_Reject_Time*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallWaitingRejectTime != lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallWaitingRejectTime = lineCallingFeaturesObj->ZyXEL_Call_Waiting_Reject_Time;
#endif	/* SUPPORT_NEW_OUI end */

		/*7. maxSessions*/
		if(mdmVoiceLineCallingFeaturesObj->maxSessions != lineCallingFeaturesObj->maxSessions) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->maxSessions = lineCallingFeaturesObj->maxSessions;
		
		/*8. conferenceCallingStatus - R*/
		
#ifdef SUPPORT_NEW_OUI
		/*9. ZyXEL_conferenceURI*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI, lineCallingFeaturesObj->ZyXEL_conferenceURI) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_ConferenceURI = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_conferenceURI);
#else
		/*9. ZyXEL_conferenceURI*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI, lineCallingFeaturesObj->ZyXEL_conferenceURI) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_ConferenceURI = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_conferenceURI);
#endif	/* SUPPORT_NEW_OUI end */
			
		/*10. conferenceCallingSessionCount - R*/
		
		/*11. callForwardingUnconditionalEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalEnable != lineCallingFeaturesObj->callForwardUnconditionalEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalEnable = lineCallingFeaturesObj->callForwardUnconditionalEnable;

		/*12. callForwardOnBusyEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyEnable != lineCallingFeaturesObj->callForwardOnBusyEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callForwardOnBusyEnable = lineCallingFeaturesObj->callForwardOnBusyEnable;
		
		/*13. callForwardOnNoAnswerEnable */
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerEnable != lineCallingFeaturesObj->callForwardOnNoAnswerEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerEnable = lineCallingFeaturesObj->callForwardOnNoAnswerEnable;

		/*14. callForwardOnNoAnswerRingCount*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerRingCount != lineCallingFeaturesObj->callForwardOnNoAnswerRingCount) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerRingCount = lineCallingFeaturesObj->callForwardOnNoAnswerRingCount;

		/*15. callForwardUnconditionalNumber*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber);
		}
		if(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber, lineCallingFeaturesObj->callForwardUnconditionalNumber) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->callForwardUnconditionalNumber = cmsMem_strdup(lineCallingFeaturesObj->callForwardUnconditionalNumber);

		/*16. callForwardOnBusyNumber*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber);
		}
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber, lineCallingFeaturesObj->callForwardOnBusyNumber) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->callForwardOnBusyNumber = cmsMem_strdup(lineCallingFeaturesObj->callForwardOnBusyNumber);

		/*17. callForwardOnNoAnswerNumber*/
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber);
		}
		if(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber, lineCallingFeaturesObj->callForwardOnNoAnswerNumber) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->callForwardOnNoAnswerNumber = cmsMem_strdup(lineCallingFeaturesObj->callForwardOnNoAnswerNumber);

		/*18. callTransferEnable*/
		if(mdmVoiceLineCallingFeaturesObj->callTransferEnable != lineCallingFeaturesObj->callTransferEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callTransferEnable = lineCallingFeaturesObj->callTransferEnable;

#ifdef SUPPORT_NEW_OUI
		/*19. ZyXEL_callTransferAfterConference*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_CallTransferAfterConference != lineCallingFeaturesObj->ZyXEL_callTransferAfterConference) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_CallTransferAfterConference = lineCallingFeaturesObj->ZyXEL_callTransferAfterConference; 

		/*20. ZyXEL_warmLineEnable*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineEnable != lineCallingFeaturesObj->ZyXEL_warmLineEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineEnable = lineCallingFeaturesObj->ZyXEL_warmLineEnable; 		

		/*21. ZyXEL_warmLineNum*/		
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber, lineCallingFeaturesObj->ZyXEL_warmLineNum) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_WarmLineNumber = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_warmLineNum);		

		/*22. ZyXEL_timerFirstDigitWarmLine*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_TimerFirstDigitWarmLine != lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_TimerFirstDigitWarmLine = lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine;

		/*23. ZyXEL_hotLineEnable*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineEnable != lineCallingFeaturesObj->ZyXEL_hotLineEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineEnable = lineCallingFeaturesObj->ZyXEL_hotLineEnable;

		/*24. ZyXEL_hotLineNum*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum, lineCallingFeaturesObj->ZyXEL_hotLineNum) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_HotLineNum = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_hotLineNum);

		/*25. MWIEnable*/
		if(mdmVoiceLineCallingFeaturesObj->MWIEnable != lineCallingFeaturesObj->MWIEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->MWIEnable = lineCallingFeaturesObj->MWIEnable;

		/*26. ZyXEL_mwiExpireTime*/
		if(mdmVoiceLineCallingFeaturesObj->X_5067F0_mwiExpireTime != lineCallingFeaturesObj->ZyXEL_mwiExpireTime) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_5067F0_mwiExpireTime = lineCallingFeaturesObj->ZyXEL_mwiExpireTime;		
#else
		/*19. ZyXEL_callTransferAfterConference*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallTransferAfterConference != lineCallingFeaturesObj->X_ZyXEL_COM_CallTransferAfterConference) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_CallTransferAfterConference = lineCallingFeaturesObj->ZyXEL_callTransferAfterConference; 

		/*20. ZyXEL_warmLineEnable*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineEnable != lineCallingFeaturesObj->ZyXEL_warmLineEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineEnable = lineCallingFeaturesObj->ZyXEL_warmLineEnable; 

		/*21. ZyXEL_warmLineNum*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum, lineCallingFeaturesObj->ZyXEL_warmLineNum) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_WarmLineNum = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_warmLineNum);

		/*22. ZyXEL_timerFirstDigitWarmLine*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_TimerFirstDigitWarmLine != lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_TimerFirstDigitWarmLine = lineCallingFeaturesObj->ZyXEL_timerFirstDigitWarmLine;

		/*23. ZyXEL_hotLineEnable*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineEnable != lineCallingFeaturesObj->ZyXEL_hotLineEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineEnable = lineCallingFeaturesObj->ZyXEL_hotLineEnable;

		/*24. ZyXEL_hotLineNum*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum != NULL){
			cmsMem_free(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum);
		}
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum == NULL) {
			bConfChange = 1;
		} else {
			if(zyStrcmp(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum, lineCallingFeaturesObj->ZyXEL_hotLineNum) != 0) {
				bConfChange = 1;
			}
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_HotLineNum = cmsMem_strdup(lineCallingFeaturesObj->ZyXEL_hotLineNum);

		/*25. MWIEnable*/
		if(mdmVoiceLineCallingFeaturesObj->MWIEnable != lineCallingFeaturesObj->MWIEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->MWIEnable = lineCallingFeaturesObj->MWIEnable;

		/*26. ZyXEL_mwiExpireTime*/
		if(mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_mwiExpireTime != lineCallingFeaturesObj->ZyXEL_mwiExpireTime) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->X_ZyXEL_COM_mwiExpireTime = lineCallingFeaturesObj->ZyXEL_mwiExpireTime;		
#endif	/* SUPPORT_NEW_OUI end */
		#if 0 /*SPR#090909671, readonly*/
		/*27. messageWaiting*/		
		mdmVoiceLineCallingFeaturesObj->messageWaiting = lineCallingFeaturesObj->messageWaiting;
		#endif

		/*28. anonymousCallBlockEnable*/
		if(mdmVoiceLineCallingFeaturesObj->anonymousCallBlockEnable != lineCallingFeaturesObj->anonymousCallBlockEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->anonymousCallBlockEnable = lineCallingFeaturesObj->anonymousCallBlockEnable;

		/*29. anonymousCallEnable*/
		if(mdmVoiceLineCallingFeaturesObj->anonymousCallEnable != lineCallingFeaturesObj->anonymousCallEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->anonymousCallEnable = lineCallingFeaturesObj->anonymousCallEnable;		

		/*30. doNotDisturbEnable;*/
		if(mdmVoiceLineCallingFeaturesObj->doNotDisturbEnable != lineCallingFeaturesObj->doNotDisturbEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->doNotDisturbEnable = lineCallingFeaturesObj->doNotDisturbEnable;		

		/*31. callReturnEnable;*/
		if(mdmVoiceLineCallingFeaturesObj->callReturnEnable != lineCallingFeaturesObj->callReturnEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->callReturnEnable = lineCallingFeaturesObj->callReturnEnable;		

	    /*32. repeatDialEnable;*/
		if(mdmVoiceLineCallingFeaturesObj->repeatDialEnable != lineCallingFeaturesObj->repeatDialEnable) {
			bConfChange = 1;
		}
		mdmVoiceLineCallingFeaturesObj->repeatDialEnable = lineCallingFeaturesObj->repeatDialEnable;

	   if ( ( ret = cmsObj_set( mdmVoiceLineCallingFeaturesObj, &iidStack)) != CMSRET_SUCCESS )
	   {
	      cmsLog_error( "Can't set Calling feature Obj ret = %d\n", ret);
	   }
		cmsObj_free((void **) &mdmVoiceLineCallingFeaturesObj);
		
	}	

	if(bConfChange) {
		sendConfigChangeToZyIMS();
	}

	if(cmsMgm_saveConfigToFlash()!=CMSRET_SUCCESS){
		cmsLck_releaseLock();
		return TR104_ERROR;
	}

#if 1	/* send event to ZyIMS */
	sendConfigWrittenToZyIMS();
#endif

	cmsLck_releaseLock();

	

	return TR104_SUCCESS;
}

int tr104StoreMdmVoicePhyIntfObj(int interfaceInst, VoicePhyIntfObj_t* phyIntfObj){
	
	CmsRet			   ret;
	InstanceIdStack    iidStack = EMPTY_INSTANCE_ID_STACK;
	PhyIntfObject *mdmVoicePhyIntfObj = NULL;
	levelInfo_t   info[_VOICE_MAXLEVELS] = { {MDMOID_PHY_INTF, interfaceInst, EMPTY_INSTANCE_ID_STACK},
								   {0, 0, EMPTY_INSTANCE_ID_STACK}, };	
	TR104_PRINTF("%s, InterfaceInst = %d\n",__FUNCTION__, interfaceInst);
	
   
	ret=cmsLck_acquireLock();
	if(ret!=CMSRET_SUCCESS) {	
		cmsLog_error( "Can't get lock, ret %d\n", ret );		
		return (ret);	
	} else {	
		if(NULL == phyIntfObj){
			TR104_PRINTF("phyIntfObj NULL\n");
			return TR104_ERROR;
		}
		if ( (ret = getMdmObj(info, &iidStack, (void**)&mdmVoicePhyIntfObj)) != CMSRET_SUCCESS )
	    {
	      cmsLck_releaseLock();
	      cmsLog_error( "Can't retrieve phyIntf object, ret %d \n", ret );
	      return ( TR104_ERROR );
	    }
			

		TR104_DBG("tr104StoreMdmVoicePhyIntfObj");


#ifdef SUPPORT_NEW_OUI
		mdmVoicePhyIntfObj->X_5067F0_FlashMaxInterval = phyIntfObj->ZyXEL_FlashMaxInterval;
		mdmVoicePhyIntfObj->X_5067F0_FlashMinInterval = phyIntfObj->ZyXEL_FlashMinInterval;
#else
		mdmVoicePhyIntfObj->X_ZyXEL_COM_FlashMaxInterval = phyIntfObj->ZyXEL_FlashMaxInterval;
		mdmVoicePhyIntfObj->X_ZyXEL_COM_FlashMinInterval = phyIntfObj->ZyXEL_FlashMinInterval;
#endif	/* SUPPORT_NEW_OUI end */
		
	   if ( ( ret = cmsObj_set( mdmVoicePhyIntfObj, &iidStack)) != CMSRET_SUCCESS )
	   {
	      cmsLog_error( "Can't set Calling feature Obj ret = %d\n", ret);
	   }
		cmsObj_free((void **) &mdmVoicePhyIntfObj);
		
	}	

	if(cmsMgm_saveConfigToFlash()!=CMSRET_SUCCESS){
		cmsLck_releaseLock();
		return TR104_ERROR;
	}
	
	sendConfigWrittenToZyIMS();

	cmsLck_releaseLock();

	

	return TR104_SUCCESS;
}

