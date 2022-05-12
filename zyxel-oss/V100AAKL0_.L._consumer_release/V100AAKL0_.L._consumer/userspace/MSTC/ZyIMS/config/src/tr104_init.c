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

#include "global.h"
#include "tr104_global.h"
#include "tr104.h"
#include "tr104_object.h"
#include "tr104_init.h"
#include "tr104_voicemdm.h"

#include "switch.h" 

#if 1 /*Jason ,[SPRID # 090622474]*/
#include "voiceApp_iptk_api.h"
#endif
#include "voiceApp_main.h"

#if HEARTBEAT_MECHANISM_ENABLE 	
#include "voiceApp_iptk_MsgHandler.h"
#endif

//Ssu-Ying, support Multiple Profile
VoiceServiceObj_t 	*tr104VoiceService = NULL;	/*TR104 Profile*/
ZyXEL_VoiceSipAccount_t *ZyXEL_VoiceSipAccount = NULL;
extern uint8* reg_error_cnt ;
extern int* reg_check_cnt ;
//extern tm_id_t *reg_status_timer;

#if HEARTBEAT_MECHANISM_ENABLE 
extern heartbeatRec_t* heartbeatRecord ;
#endif
int  Tr104InitComplete = 0; /*0: not complete , 1: complete*/

/*sipAccountNumber is a global variable in TR104 Module which is stored the total sip account number */
uint32 sipAccountNumber=0;

//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104ModuleInit
**
** Description  : Init TR104 data model step by step 
**                           
** Returns      : 
****************************************************************************/
int 
tr104ModuleInit(void)
{
	int VpItr=0,LineItr=0,PhyIdx=0,FxoIdx=0,SipAccountId=0,ret=0 , tr104Ret=0;
	uint32 NumOfLine=0 ,NumOfPhyIntf=0 ,NumOfFxo=0, NumOfProfile=0 ;


	do{			
		/*Step 0 : load Configs of the VoiceService object */
		if (  tr104VoiceService == NULL) {
			tr104VoiceService = (VoiceServiceObj_t*)zyMalloc(sizeof(VoiceServiceObj_t));
			if (  tr104VoiceService == NULL) {
				ZyPrintf("tr104VoiceServiceInit Error: No enough memory\n");
				ret = TR104_ERROR ;
				break;
			}else if((ret = tr104VoiceServiceObjLoadConfig())!=TR104_SUCCESS){
				ZyPrintf(" tr104VoiceServiceObjLoadConfig not success: cmsRet = %d \n",ret);
				break;
			}else{
				TR104_DBG(" tr104VoiceService Init Success ");
			}
		}
				
		/*Step 1 : Init voiceCapabilities and load config*/
		if (tr104VoiceService->voiceCapabilities == NULL) {
			  tr104VoiceService->voiceCapabilities = (VoiceCapObj_t *)zyMalloc(sizeof(VoiceCapObj_t));
			if (  tr104VoiceService->voiceCapabilities== NULL) {
				ZyPrintf("voiceCapabilityInit Error: No enough memory\n");
				ret = TR104_ERROR;
				break;
			}else{
				tr104VoiceCapObjChildInit();
				if((ret = tr104VoiceCapObjLoadConfig())!=TR104_SUCCESS){
					ZyPrintf(" tr104VoiceCapObjLoadConfig not success: ret = %d \n",ret);
					break;
				}else{
					TR104_DBG(" voiceCapabilities Init Success ");
				}
			}
		}

		TR104_DBG("before tr104GetNumOfPhyIntf");
		/*Step 4 : Init VoicePhyIntf and load config*/
		NumOfPhyIntf = tr104GetNumOfPhyIntf(); 
		TR104_DBG1("NumOfPhyIntf =",NumOfPhyIntf);
		if (tr104VoiceService->phyInterface == NULL) {	
			tr104VoiceService->phyInterface = (VoicePhyIntfObj_t*)zyMalloc(NumOfPhyIntf * sizeof(VoicePhyIntfObj_t));
			if (tr104VoiceService->phyInterface == NULL) {
				ZyPrintf("voicePhyInterface Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}else{
				for(PhyIdx=0;PhyIdx<NumOfPhyIntf;PhyIdx++){
					tr104VoicePhyIntfChildInit(PhyIdx);	
					if((ret = tr104VoicePhyIntfLoadConfig(PhyIdx))!=TR104_SUCCESS){
						ZyPrintf(" tr104VoicePhyIntfLoadConfig not success: ret = %d , PhyIdx = %d \n",ret,PhyIdx);
						break;
					}else{
						TR104_DBG(" voicePhyIntf Init Success ");		
					}
				}
			}			
		}

		/*Step 5 : Init ZyXEL_VoiceFXO and load config*/
		NumOfFxo = tr104GetNumOfFXO();
		TR104_DBG1("NumOfFxo =",NumOfFxo);
		if (tr104VoiceService->ZyXEL_VoiceFXO== NULL) {	
			tr104VoiceService->ZyXEL_VoiceFXO = (ZyXEL_VoiceFXO_t*)zyMalloc(NumOfFxo * sizeof(ZyXEL_VoiceFXO_t));
			if (tr104VoiceService->ZyXEL_VoiceFXO == NULL) {
				ZyPrintf("ZyXEL_VoiceFXO Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}else{
				for(FxoIdx=0;FxoIdx<NumOfFxo;FxoIdx++){
					tr104VoiceFxoChildInit(FxoIdx);	
					if((ret = tr104VoiceFxoLoadConfig(FxoIdx))!=TR104_SUCCESS){
						ZyPrintf(" tr104VoiceFxoLoadConfig not success: ret = %d , FxoIdx = %d \n",ret,FxoIdx);
						break;
					}else{
						TR104_DBG(" voiceFxo Init Success ");		
					}
				}
			}			
		}
		
		NumOfProfile = tr104GetNumProfile();
		TR104_DBG1("NumOfProfile=",NumOfProfile);

		/*Step 2 :Init voice profile and load config*/
		if (tr104VoiceService->voiceProfObjArray == NULL) {
			tr104VoiceService->voiceProfObjArray = (VoiceProfObj_t *)zyMalloc(NumOfProfile*sizeof(VoiceProfObj_t));
			if (tr104VoiceService->voiceProfObjArray  == NULL) {
				ZyPrintf("voiceProfObjArrayInit Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}else{
				tr104VoiceProfObjChildInit(NumOfProfile);
				for(VpItr=0;VpItr<NumOfProfile;VpItr++){					
					if((ret = tr104VoiceProfileObjLoadConfig(VpItr))!=TR104_SUCCESS){
						ZyPrintf(" tr104VoiceProfileObjLoadConfig not success: ret = %d , VpItr = %d \n",ret,VpItr);
						break;
					}else {
						TR104_DBG(" voiceProfObjArray Init Success ");
						VoiceProfObj_t* pProfObj = tr104VoiceProfObjPtrGet(VpItr);
						NumOfLine = tr104GetVPNumOfLine(VpItr);
						pProfObj->lineProfPtrLst = (VoiceProfLineObj_t *)zyMalloc(NumOfLine*sizeof(VoiceProfLineObj_t));
						if(pProfObj->lineProfPtrLst == NULL) {
							ZyPrintf("lineProfPtrLstInit Error: No enough memory\n");
							ret =TR104_ERROR;
							break;
						}else{		
							tr104VoiceProfLineObjChildInit(VpItr,NumOfLine);
							for(LineItr=0;LineItr<NumOfLine;LineItr++){									
								TR104_DBG1("NumOfLine =",NumOfLine);			
														
								if((ret = tr104VoiceProfLineObjLoadConfig(VpItr,LineItr)) != TR104_SUCCESS){
									ZyPrintf(" tr104VoiceProfLineObjLoadConfig not success: ret = %d , VpItr = %d , LineItr = %d \n",ret,VpItr,LineItr);
									break;
								}else{
									TR104_DBG(" lineProfPtrLst Init Success ");
								}						
							}
						}
					}
				}
			}
			
		}	
#if HEARTBEAT_MECHANISM_ENABLE 
		heartbeatRecord = (heartbeatRec_t*)zyMalloc(NumOfProfile*sizeof(heartbeatRec_t));
		for(VpItr=0;VpItr<NumOfProfile;VpItr++){
			heartbeatRecord[VpItr].hearbeatOngoing = FALSE ;
			heartbeatRecord[VpItr].timerID = 0;
		}
#endif	
			
		/*Step 3 : Init ZyXEL_voiceCommon and load config*/
		if (tr104VoiceService->ZyXEL_voiceCommon== NULL) {
			tr104VoiceService->ZyXEL_voiceCommon= (ZyXEL_VoiceCommon_t *)zyMalloc(sizeof(ZyXEL_VoiceCommon_t));
			if (tr104VoiceService->ZyXEL_voiceCommon == NULL) {
				ZyPrintf("ZyXEL_voiceCommonInit Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}
			if((ret = tr104VoiceCommonLoadConfig())!=TR104_SUCCESS){
				ZyPrintf(" tr104VoiceCommonLoadConfig not success: ret = %d \n",ret);
				break;
			}
			TR104_DBG(" ZyXEL_voiceCommon Init Success ");
		}		

		/*Step 6 : Init ZyXEL_VoiceSipAccount*/
		sipAccountNumber = 0;
		for(VpItr=0;VpItr<NumOfProfile;VpItr++){
			NumOfLine = tr104GetVPNumOfLine(VpItr);
			sipAccountNumber = sipAccountNumber + NumOfLine;
			TR104_DBG2("NumOfLine =",NumOfLine , "sipAccountNumber",sipAccountNumber);
		}
		
		if (ZyXEL_VoiceSipAccount == NULL) {	
			ZyXEL_VoiceSipAccount = (ZyXEL_VoiceSipAccount_t*)zyMalloc(sipAccountNumber * sizeof(ZyXEL_VoiceSipAccount_t));
			if (ZyXEL_VoiceSipAccount == NULL) {
				ZyPrintf("ZyXEL_VoiceSipAccount Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}else{
				for(SipAccountId=0;SipAccountId<sipAccountNumber;SipAccountId++){
					tr104VoiceSipAccountInit(SipAccountId);	
				}
				SipAccountId = 0;
				VoiceProfObj_t* Proftmp= NULL;
				VoiceProfLineObj_t* Linetmp= NULL;
				for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
					Proftmp = tr104VoiceProfObjPtrGet(VpItr);
					NumOfLine = tr104GetVPNumOfLine(VpItr);
					TR104_DBG1("VpItr =",VpItr);
					for(LineItr=0;LineItr<NumOfLine;LineItr++){
						Linetmp = tr104VoiceLineObjPtrGet(VpItr, LineItr);
						TR104_DBG1("LineItr =",LineItr);
						ZyXEL_VoiceSipAccount[SipAccountId].enable = (Linetmp->enable) & (Proftmp->enable);
						ZyXEL_VoiceSipAccount[SipAccountId].sipAccountId = Linetmp->ZyXEL_CMAcntNum;
						ZyXEL_VoiceSipAccount[SipAccountId].SPId = Proftmp->ZyXEL_SPNum;
						ZyXEL_VoiceSipAccount[SipAccountId].LineItr= LineItr;
						ZyXEL_VoiceSipAccount[SipAccountId].VpItr= VpItr;
						ZyXEL_VoiceSipAccount[SipAccountId].flag = 0;
						ZyXEL_VoiceSipAccount[SipAccountId].ptrProf = tr104VoiceProfObjPtrGet(VpItr);
						ZyXEL_VoiceSipAccount[SipAccountId].ptrLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
						SipAccountId++;
					}
					
				}
			}			
			TR104_DBG(" ZyXEL_VoiceSipAccount Init Success ");
		}
		/* voiceAPP_IPTK_MsgHandler : reg_error_cnt init */
		if(reg_error_cnt!=NULL){
			zyFree(reg_error_cnt);
			reg_error_cnt = NULL;
		}
		reg_error_cnt = (uint8 *)zyMalloc(sipAccountNumber*sizeof(uint8));		
		if(reg_check_cnt!=NULL){
			zyFree(reg_check_cnt);
			reg_check_cnt = NULL;
		}		
		reg_check_cnt = (int *)zyMalloc(sipAccountNumber*sizeof(int));

		if(reg_timer!=NULL){
			zyFree(reg_timer);
			reg_timer = NULL;
		}
		reg_timer = (voiceAppRegTimer_t *)zyMalloc(sipAccountNumber*sizeof(voiceAppRegTimer_t));	

		if(ICFLineInitDone != NULL){
			zyFree(ICFLineInitDone);
			ICFLineInitDone = NULL;
		}
		ICFLineInitDone = (uint8 *)zyMalloc(sipAccountNumber*sizeof(uint8));
		
		//reg_status_timer = (tm_id_t *)zyMalloc(sipAccountNumber*sizeof(tm_id_t));
		zyMemset((void *)reg_error_cnt, 0, sipAccountNumber*sizeof(uint8));
		zyMemset((void *)reg_check_cnt, 0, sipAccountNumber*sizeof(int));
		zyMemset((void *)reg_timer, 0, sipAccountNumber*sizeof(voiceAppRegTimer_t));	
		zyMemset((void *)ICFLineInitDone, 0, sipAccountNumber*sizeof(uint8));	
		//zyMemset((void *)reg_status_timer, 0, sipAccountNumber*sizeof(tm_id_t));

		#if 1 /*Jason ,[SPRID # 090622474]*/	
			oneShotEvent = (oneShotEventRec_t*)zyMalloc(sipAccountNumber*sizeof(struct oneShotEventRec_s)) ;
		#endif			

		
#if defined(FXO) || defined(FXO_DSPRELAY)
		/*Step 6 : Init VoicePSTN and load config*/
		/*need modify*/
		if (tr104VoiceService->ZyXEL_VoicePSTN== NULL) {
			tr104VoiceService->ZyXEL_VoicePSTN = (ZyXEL_VoicePSTN_t*)zyMalloc(TR104_VPSTN_CNT*sizeof(ZyXEL_VoicePSTN_t));
			if (tr104VoiceService->ZyXEL_VoicePSTN == NULL) {
				ZyPrintf("VoicePSTNInit Error: No enough memory\n");
				ret=TR104_ERROR;
				break;
			}
			if((ret = tr104VoicePSTNLoadConfig())!=TR104_SUCCESS){
				ZyPrintf(" tr104VoicePSTNLoadConfig not success: ret = %d \n",ret);
				break;
			}
			TR104_DBG(" VoicePSTN Init Success ");
		}
#endif
		/*Step 7 : Init VoicePhoneBook and load config*/
			
		if (tr104VoiceService->ZyXEL_VoicePhoneBook== NULL) {
			tr104VoiceService->ZyXEL_VoicePhoneBook = (ZyXEL_VoicePhoneBook_t*)zyMalloc(TR104_VPHONE_BOOK_CNT*sizeof(ZyXEL_VoicePhoneBook_t));
			if (tr104VoiceService->ZyXEL_VoicePhoneBook == NULL) {
				ZyPrintf("VoicePhoneBookInit Error: No enough memory\n");
				return TR104_ERROR;
			}
			if((ret = tr104VoicePhoneBookLoadConfig())!=TR104_SUCCESS){
				ZyPrintf(" tr104VoicePhoneBookLoadConfig not success: ret = %d \n",ret);
				break;
			}
			TR104_DBG(" tr104VoicePhoneBookLoadConfig Init Success ");

			
		}
		
	}while(0);
	
	if( ret == TR104_SUCCESS ){
		Tr104InitComplete = 1;
		if(Tr104Debug)	{	
			tr104DumpAll();
		}
	}else{
		Tr104InitComplete = 0;
		tr104ModuleFree(); 
		ZyPrintf("ERROR : ############ TR104 Module Init Fail ###########\n");
	}

	return ret; 
}

/***************************************************************************
** Function Name: tr104VoiceCapObjChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceCapObjChildInit(void)
{
	VoiceCapObj_t* voiceCapObj = (VoiceCapObj_t*) tr104VoiceCapObjPtrGet();
	voiceCapObj->CapOfSIP =  (VoiceCapSipObj_t *) zyMalloc(sizeof(VoiceCapSipObj_t));
	voiceCapObj->CapCodecsTable =tr104ObjLstNew(); 
	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfObjChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceProfObjChildInit( int NumOfProfile )
{
	int VpItr = 0 ;
	VoiceProfObj_t* voiceProfPtr = NULL;
	
	for(VpItr=0;VpItr<NumOfProfile;VpItr++){
		voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);	
		
		voiceProfPtr->serviceProviderInfo =  (VoiceProfServiceProviderInfoObj_t *) zyMalloc(sizeof(VoiceProfServiceProviderInfoObj_t));
		voiceProfPtr->SIPObj =  tr104VoiceProfSipObjInit();
		voiceProfPtr->RTP_Profile = tr104VoiceProfRtpObjInit();
		voiceProfPtr->numberingPlanObj = tr104VoiceProfNumPlanObjInit();
		voiceProfPtr->toneObj =  tr104VoiceProfToneObjInit();
		voiceProfPtr->buttonMapObj = tr104VoiceProfButtonMapObjInit() ;	
		voiceProfPtr->faxT38Obj =  (VoiceProfFaxT38Obj_t *) zyMalloc(sizeof(VoiceProfFaxT38Obj_t));
		/*we only need the pointer record for lines which use this profile*/
	}

	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoiceProfButtonMapObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/

VoiceProfButtonMapObj_t*
tr104VoiceProfButtonMapObjInit(void)
{
	VoiceProfButtonMapObj_t* this_= (VoiceProfButtonMapObj_t*)zyMalloc(sizeof(VoiceProfButtonMapObj_t));
	this_->numberOfButtons=0;
	this_->buttonTab = tr104ObjLstNew();
	
	return this_;
}
/***************************************************************************
** Function Name: tr104VoiceProfRtpObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/

VoiceProfRtpObj_t*
tr104VoiceProfRtpObjInit(void)
{
	VoiceProfRtpObj_t* this_ = (VoiceProfRtpObj_t*) zyMalloc(sizeof(VoiceProfRtpObj_t));
	this_->RTCP_Profile = (VoiceProfRtpRtcpObj_t*) zyMalloc(sizeof(VoiceProfRtpRtcpObj_t));
	this_->SRTP_Profile = (VoiceProfRtpSrtpObj_t*) zyMalloc(sizeof(VoiceProfRtpSrtpObj_t));
	this_->RTPRedundancy = (VoiceProfRtpRedundancyObj_t*) zyMalloc(sizeof(VoiceProfRtpRedundancyObj_t));

	return this_;
}
/***************************************************************************
** Function Name: tr104VoiceProfNumPlanObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfNumPlanObj_t * 
tr104VoiceProfNumPlanObjInit(void)
{
	VoiceProfNumPlanObj_t * this_ = (VoiceProfNumPlanObj_t *) zyMalloc(sizeof(VoiceProfNumPlanObj_t));
	this_->prefixInfoMaxEntries = 0;
	this_->prefixInfoNumberOfEntries = 0;
	this_->PrefixInfoTab = tr104ObjLstNew();
	return this_;
}
/***************************************************************************
** Function Name: tr104VoiceProfSipObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfSipObj_t *
tr104VoiceProfSipObjInit(void)
{
	VoiceProfSipObj_t * _this=(VoiceProfSipObj_t *) zyMalloc(sizeof(VoiceProfSipObj_t));
	_this->SIPEventSubscribeNumberOfElements = 0 ;
	_this->SIPResponseMapNumberOfElements = 0;
	_this->responseMapTab =  tr104ObjLstNew();		/*W*/
	_this->eventsubScribeTab =  tr104ObjLstNew();		/*W*/
	
	return _this;
}
/***************************************************************************
** Function Name: tr104VoiceProfToneObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfToneObj_t*
tr104VoiceProfToneObjInit(void)
{
	VoiceProfToneObj_t* this_= (VoiceProfToneObj_t*) zyMalloc(sizeof(VoiceProfToneObj_t));
	this_->eventNumberOfEntries=0;
	this_->descriptionNumberOfEntries=0;
	this_->patternNumberOfEntries=0;
	this_->toneEventTab= tr104ObjLstNew();		/*R*/
	this_->toneDescriptionTab =  tr104ObjLstNew();	/*W*/
	this_->tonePatternTab =  tr104ObjLstNew();		/*W*/

	return this_;
}
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104VoiceProfLineObjChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/

int 
tr104VoiceProfLineObjChildInit(int VpItr , int NumOfLine)
{
	VoiceProfLineObj_t* Object_= NULL;
	int LineItr = 0;

	TR104_DBG("tr104VoiceProfLineObjChildInit > ENTER ");

	for(LineItr=0;LineItr<NumOfLine;LineItr++ ){	
		Object_= (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);	
		Object_->line_sip = tr104VoiceProfLineSipObjInit();
		Object_->ringer = tr104VoiceProfLineRingerObjInit();
		Object_->callingFeatures=tr104VoiceProfLineCallingFeaturesObjInit() ;
		Object_->voiceProcessing=(VoiceProfLineVoiceProcessingObj_t*) zyMalloc(sizeof(VoiceProfLineVoiceProcessingObj_t));
		Object_->lineCodec= tr104VoiceProfLineCodecObjInit();
		Object_->sessionLst =  tr104ObjLstNew();
		Object_->lineStatistics = (VoiceProfLineStatsObj_t*)zyMalloc(sizeof(VoiceProfLineStatsObj_t));
		//Object_->status = TR104I_STATUS_QUIECCENT;
		Object_->status = TR104I_STATUS_IDLE;
	}
		
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfLineCodecObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfLineCodecObj_t*
tr104VoiceProfLineCodecObjInit(void)
{
	VoiceProfLineCodecObj_t* Object_=(VoiceProfLineCodecObj_t*)zyMalloc(sizeof(VoiceProfLineCodecObj_t));
	Object_->codecList = tr104ObjLstNew();
	return Object_;
}

/***************************************************************************
** Function Name: tr104VoiceProfLineSipObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfLineSipObj_t*
tr104VoiceProfLineSipObjInit(void)
{
	VoiceProfLineSipObj_t* this_ = (VoiceProfLineSipObj_t*)zyMalloc(sizeof(VoiceProfLineSipObj_t));
	this_->SIPEventSubscribeNumberOfElements=0;
	this_->sipEventSubTab= tr104ObjLstNew();
	return this_;
}
/***************************************************************************
** Function Name: tr104VoiceProfLineRingerObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfLineRingerObj_t*
tr104VoiceProfLineRingerObjInit(void)
{
	VoiceProfLineRingerObj_t* this_ = (VoiceProfLineRingerObj_t*)zyMalloc(sizeof(VoiceProfLineRingerObj_t));
	this_->eventNumberOfEntries=0	;
	this_->descriptionNumberOfEntries=0;
	this_->patternNumberOfEntries=0;

	this_->ringerEventTab= tr104ObjLstNew();		/*R*/
	this_->ringerDescriptionTab= tr104ObjLstNew();	/*W*/
	this_->ringerPatternTab= tr104ObjLstNew();	/*W*/
	return this_;
}

/***************************************************************************
** Function Name: tr104VoiceProfLineCallingFeaturesObjInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
VoiceProfLineCallingFeaturesObj_t*
tr104VoiceProfLineCallingFeaturesObjInit(void)
{
	VoiceProfLineCallingFeaturesObj_t* this_=(VoiceProfLineCallingFeaturesObj_t*) zyMalloc(sizeof(VoiceProfLineCallingFeaturesObj_t));
	/* this_->ZyXEL_VoiceCallFwdTab = (ZyXEL_VoiceCallFwd_t*)zyMalloc(sizeof(ZyXEL_VoiceCallFwd_t));*/
	return this_;
}
//Ssu-Ying, support Multiple Profile
#if 0
/***************************************************************************
** Function Name: tr104VoicePhyIntfMgnChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoicePhyIntfMgnChildInit(void)
{
	int i=0;
	int numOfFXS,numOfFXO,numOfPhyIntf; 
	ZyXEL_VoicePhyIntfMgn_t* pPhyIntfManager =  tr104VoiceService->voicePhyIntfMgn;
	VoicePhyIntfObj_t*	phyIntfObj = NULL;

	numOfFXS = pPhyIntfManager->numberOfFXS ;
	numOfFXO = pPhyIntfManager->numberOfFXO ;
	numOfPhyIntf = pPhyIntfManager->numberOfPhyIntf ;
	if(numOfPhyIntf==0){
		ZyPrintf(" numOfPhyIntf =0 \n");
	}else{
		pPhyIntfManager->phyIntfObjArray=(VoicePhyIntfObj_t *) zyMalloc(numOfPhyIntf * sizeof(VoicePhyIntfObj_t)) ;

		for(i=0;i<numOfPhyIntf;i++){
			phyIntfObj = ( VoicePhyIntfObj_t* )tr104VoicePhyIntfObjPtrGet(i);
			
			if(i<numOfFXS){
				phyIntfObj->ZyXEL_PhyIntf = (ZyXEL_VoiceFXS_t *) zyMalloc(sizeof(ZyXEL_VoiceFXS_t));
			}else if((i>=numOfFXS)&&(i<numOfPhyIntf)){
				phyIntfObj->ZyXEL_PhyIntf = (ZyXEL_VoiceFXO_t *) zyMalloc(sizeof(ZyXEL_VoiceFXO_t));		
			}
		}
	}
	return TR104_SUCCESS;
}
#endif

/***************************************************************************
** Function Name: tr104VoicePhyIntfChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoicePhyIntfChildInit(phyIntfIdx)
{
	VoicePhyIntfObj_t* phyInterface= NULL;

	phyInterface= (VoicePhyIntfObj_t*)tr104VoicePhyIntfObjPtrGet(phyIntfIdx);
	//phyInterface->PhyInterfaceTest = (VoicePhyIntfTests_t*) zyMalloc(sizeof(VoicePhyIntfTests_t));
	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceFxoChildInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceFxoChildInit(fxoIdx)
{
	ZyXEL_VoiceFXO_t* ZyXEL_VoiceFXO= NULL;

	ZyXEL_VoiceFXO= (ZyXEL_VoiceFXO_t*)tr104VoiceFXOPtrGet(fxoIdx);
	ZyXEL_VoiceFXO->fxoPort = (ZyXEL_VoiceFXOPort_t*) zyMalloc(sizeof(ZyXEL_VoiceFXOPort_t));
	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceVoiceSipAccountInit
**
** Description  : Init the child objects  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceSipAccountInit(SipAccountId)
{
	ZyXEL_VoiceSipAccount_t* ZyXEL_VoiceSipAccount= NULL;

	ZyXEL_VoiceSipAccount= (ZyXEL_VoiceSipAccount_t*)tr104VoiceSipAccountPtrGet(SipAccountId);

	return TR104_SUCCESS;
}
/**********************************************************/
/***************************************************************************
** Function Name: tr104VoiceServiceObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int
tr104VoiceServiceObjLoadConfig(void)
{
	CmsRet ret;
	TR104_DBG("call tr104VoiceServiceObjLoadConfig ");			
	if((ret= tr104LoadMdmVoiceCfg(tr104VoiceService))!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104VoiceServiceObjLoadConfig : %d \n",ret);
		return TR104_ERROR;
	}
	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoiceCommonLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceCommonLoadConfig(void)
{
	CmsRet ret;
	
	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceService->ZyXEL_voiceCommon;

	pCommon->parent_VoiceServiceObj = (void*) tr104VoiceServiceObjPtrGet();
	TR104_DBG("call tr104VoiceCommonLoadConfig ");		
	if((ret=tr104LoadMdmVoiceCommCfg(pCommon))!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104VoiceCommonLoadConfig : %d \n",ret);
		return ret;
	}
	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoiceCapObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int 
tr104VoiceCapObjLoadConfig(void)
{
	CmsRet ret;
	VoiceCapObj_t* pCapObj = tr104VoiceService->voiceCapabilities;
	VoiceCapSipObj_t*	pCapSipObj = pCapObj->CapOfSIP;
	
	TR104_DBG("call tr104VoiceCapObjLoadConfig ");	
	ret=tr104LoadMdmVoiceCapCfg(pCapObj);	/*load the config of the comm part */
	if(ret!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104LoadMdmVoiceCapCfg : %d \n",ret);	
		return ret;
	}
	ret=tr104LoadMdmVoiceCapSipCfg(pCapSipObj);
	if(ret!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104LoadMdmVoiceCapSipCfg : %d \n",ret);	
		return ret;
	}	
	
	tr104VoiceCapCodecsTabLoadConfig();

	pCapObj->parent_VoiceServiceObj = (void*) tr104VoiceServiceObjPtrGet();
	pCapSipObj->parent_VoiceCapObj = (void*) tr104VoiceCapObjPtrGet();
		
	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoiceCapCodecsTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceCapCodecsTabLoadConfig(void)
{
	CmsRet ret;
	int idx; 
	voiceParams_t  parms ;
	VoiceCapObj_t* pCapObj = tr104VoiceService->voiceCapabilities;
	VoiceCapCodecObj_t* pCapCodecObj = NULL; 	

	TR104_DBG("call tr104VoiceCapCodecsTabLoadConfig ");		
	/*clear the list before loading the config from mdm*/
	if(pCapObj->CapCodecsTable->size_!=0){
		TR104_DBG(" CapCodecsTable->size_!=0 ==> reload mode ");
		return TR104_SUCCESS ; 
	}

	ret=CMSRET_SUCCESS;
	parms.op[0]=0;
	TR104_DBG1(" CapCodecsTable->size_ =",pCapObj->CapCodecsTable->size_);
	
	for(idx=0;ret==CMSRET_SUCCESS;idx++){	
		parms.op[0]= idx+1;
		pCapCodecObj=(VoiceCapCodecObj_t*) zyMalloc(sizeof(VoiceCapCodecObj_t));
		ret = tr104LoadMdmVoiceCapCodecsCfg(&parms , pCapCodecObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(pCapCodecObj);
			pCapCodecObj = NULL;
				TR104_DBG1(" number of cap Codec =",pCapObj->CapCodecsTable->size_);				
		}else{
			pCapCodecObj->parent_VoiceCapObj=(void*) tr104VoiceCapObjPtrGet();
			tr104LstAddElmTail(pCapObj->CapCodecsTable , pCapCodecObj);
		}	
	}
	
	return TR104_SUCCESS;
}
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104VoiceProfileObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int
tr104VoiceProfileObjLoadConfig(int VpItr)
{	
	CmsRet ret;
	voiceParams_t  parms ;
	
	VoiceProfObj_t* pProfObj = (VoiceProfObj_t*) tr104VoiceProfObjPtrGet(VpItr);	
	VoiceProfServiceProviderInfoObj_t* pProvider = pProfObj->serviceProviderInfo;
	VoiceProfSipObj_t* pProfSipObj = pProfObj->SIPObj ;
	VoiceProfRtpObj_t*  pProfRtp=pProfObj->RTP_Profile;
	VoiceProfNumPlanObj_t* ProfNumPlan=pProfObj->numberingPlanObj;
	VoiceProfToneObj_t*	pProfTone=pProfObj->toneObj;
	VoiceProfFaxT38Obj_t * pProfFaxT38=pProfObj->faxT38Obj;
	TR104_DBG("call tr104VoiceProfileObjLoadConfig ");	
	
	parms.op[0]=VpItr+1;
	
	ret = tr104LoadMdmVoiceProfCfg(&parms, pProfObj);
	if(ret!=CMSRET_SUCCESS){
		ZyPrintf("  tr104LoadMdmVoiceProfCfg : ret =  \n",ret);
		return ret;
	}
	tr104LoadMdmServiceProviderInfoCfg(&parms,pProvider);
	tr104VoiceProfSipObjLoadConfig(VpItr,pProfSipObj);
	tr104VoiceProfRtpObjLoadConfig(VpItr,pProfRtp);
	tr104VoiceProfNumPlanObjLoadConfig(VpItr,ProfNumPlan);
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	tr104VoiceProfToneObjLoadConfig(VpItr,pProfTone);
	#endif
	tr104LoadMdmVoiceProfFaxT38Cfg(&parms,pProfFaxT38);
	
	
	pProfObj->parent_VoiceServiceObj = (void*) tr104VoiceServiceObjPtrGet();
	pProvider->parent_VoiceProfObj = (void*) pProfObj;
	pProfSipObj->parent_VoiceProfObj= (void*) pProfObj;
	pProfRtp->parent_VoiceProfObj=(void*) pProfObj;
	ProfNumPlan->parent_VoiceProfObj=(void*) pProfObj;
	pProfTone->parent_VoiceProfObj=(void*) pProfObj	;	
	pProfFaxT38->parent_VoiceProfObj=(void*)  pProfObj;	
	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoiceProfSipObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int  
tr104VoiceProfSipObjLoadConfig(int VpItr,VoiceProfSipObj_t*  pProfSipObj)
{
	voiceParams_t parms;

	TR104_DBG("call tr104VoiceProfSipObjLoadConfig ");	
	
	parms.op[0]= VpItr+1;
	tr104LoadMdmVoiceProfSipCfg(&parms,pProfSipObj);
	tr104VoiceProfSipSubTabLoadConfig(VpItr);
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	tr104VoiceProfSipRespMapTabLoadConfig(VpItr);
	#endif
	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfSipSubTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int 
tr104VoiceProfSipSubTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	int i, sipEventSubNum ;
	voiceParams_t parms;
	VoiceProfSipObj_t*  pProfSipObj = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfSipEventSubObj_t* pSipEventSubObj=NULL ;

	TR104_DBG("call tr104VoiceProfSipSubTabLoadConfig ");		
	
	if(pProfSipObj==NULL){
		ZyPrintf("Profile sip Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((pProfSipObj->eventsubScribeTab->size_)!=0){
		tr104LstAllElmClear(pProfSipObj->eventsubScribeTab);
	}
	
	sipEventSubNum=pProfSipObj->SIPEventSubscribeNumberOfElements;
	TR104_DBG1("sipEventSubNum = ",sipEventSubNum);
	parms.op[0]= VpItr+1;
	parms.op[1]= 0; 

	for(i=0;i<sipEventSubNum;i++){
		parms.op[1]= i+1; 
		pSipEventSubObj=(VoiceProfSipEventSubObj_t*) zyMalloc(sizeof(VoiceProfSipEventSubObj_t));	
		if((ret=tr104LoadMdmVoiceProfSipSubCfg(&parms,  pSipEventSubObj))!=CMSRET_SUCCESS){
			zyFree(pSipEventSubObj);
			pSipEventSubObj=NULL;
			ZyPrintf(" tr104LoadMdmVoiceProfSipSubCfg ERROR \n");
			return TR104_ERROR;
		}	
		pSipEventSubObj->parent_VoiceProfSipObj=pProfSipObj;
		tr104LstAddElmTail(pProfSipObj->eventsubScribeTab , pSipEventSubObj);
	}

	return TR104_SUCCESS;
}
#ifndef SUPPORT_412_VOICE /*Not Support*/
/***************************************************************************
** Function Name: tr104VoiceProfSipRespMapTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceProfSipRespMapTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	int i, sipRespMapNum ;
	voiceParams_t parms;
	VoiceProfSipObj_t*  pProfSipObj = tr104VoiceProfSipObjPtrGet(VpItr);
	VoiceProfSipRespMapObj_t*  pSipRespMapObj=NULL;

	TR104_DBG("call tr104VoiceProfSipRespMapTabLoadConfig ");			
	if(pProfSipObj==NULL){
		ZyPrintf("Profile sip Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((pProfSipObj->responseMapTab->size_)!=0){
		tr104LstAllElmClear(pProfSipObj->responseMapTab);
	}
	sipRespMapNum=pProfSipObj->SIPResponseMapNumberOfElements;
	TR104_DBG1("sipRespMapNum = ",sipRespMapNum);
	parms.op[0]= VpItr+1;
	parms.op[1]= 0; 

	for(i=0;i<sipRespMapNum;i++){
		parms.op[1]= i+1; 
		pSipRespMapObj=(VoiceProfSipRespMapObj_t*) zyMalloc(sizeof(VoiceProfSipRespMapObj_t));	
		if((ret=tr104LoadMdmVoiceProfSipRespCfg(&parms, pSipRespMapObj))!=CMSRET_SUCCESS){
			zyFree(pSipRespMapObj);
			pSipRespMapObj = NULL;
			ZyPrintf(" tr104VoiceProfSipRespMapTabLoadConfig ERROR \n");
			return TR104_ERROR;
		}	
		pSipRespMapObj->parent_VoiceProfSipObj=pProfSipObj;
		tr104LstAddElmTail(pProfSipObj->responseMapTab, pSipRespMapObj);
	}

	return TR104_SUCCESS;
}
#endif
/***************************************************************************
** Function Name: tr104VoiceProfRtpObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int  
tr104VoiceProfRtpObjLoadConfig(int VpItr ,VoiceProfRtpObj_t*  pProfRtp)
{
	voiceParams_t parms;

	VoiceProfRtpRtcpObj_t* pRtpRtcp=pProfRtp->RTCP_Profile;
	VoiceProfRtpSrtpObj_t* pRtpSrtp=pProfRtp->SRTP_Profile;
	VoiceProfRtpRedundancyObj_t*  pRtpRedundancy = pProfRtp->RTPRedundancy;
	TR104_DBG("call tr104VoiceProfRtpObjLoadConfig ");		
	parms.op[0]=VpItr+1;
	
	tr104LoadMdmVoiceProfRtpCfg( &parms, pProfRtp);
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	tr104LoadMdmVoiceProfRtpRtcpCfg( &parms, pRtpRtcp);	
	#endif
#ifdef MSTC_VOICE_SUPPORT_SRTP
	tr104LoadMdmVoiceProfRtpSrtpCfg( &parms , pRtpSrtp);
#endif
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	tr104LoadMdmVoiceProfRtpRedundancyCfg(&parms , pRtpRedundancy);
	#endif

	pRtpRtcp->parent_VoiceProfRtpObj=(void*) pProfRtp;
	pRtpSrtp->parent_VoiceProfRtpObj=(void*) pProfRtp;
	pRtpRedundancy->parent_VoiceProfRtpObj=(void*) pProfRtp;
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfNumPlanObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceProfNumPlanObjLoadConfig( int VpItr ,VoiceProfNumPlanObj_t* ProfNumPlan)
{
	voiceParams_t parms;

	TR104_DBG("call tr104VoiceProfNumPlanObjLoadConfig ");		
	parms.op[0]=VpItr+1;
	tr104LoadMdmVoiceProfNumberingPlanCfg(&parms, ProfNumPlan);
	tr104VoiceProfNumPlanPrefixTabLoadConfig(VpItr);
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfNumPlanPrefixTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceProfNumPlanPrefixTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i,prefixNum;
	VoiceProfNumPlanPrefixInfoObj_t* pPrefixInfoObj=NULL;
	VoiceProfNumPlanObj_t* ProfNumPlan = tr104VoiceProfNumPlanObjPtrGet(VpItr);	
	TR104_DBG("call tr104VoiceProfNumPlanPrefixTabLoadConfig ");	
	if(ProfNumPlan==NULL){
		ZyPrintf("ProfNumPlan Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((ProfNumPlan->PrefixInfoTab->size_)!=0){
		tr104LstAllElmClear(ProfNumPlan->PrefixInfoTab);
	}

	prefixNum = ProfNumPlan->prefixInfoNumberOfEntries;
	TR104_DBG1("prefixNum = ",prefixNum);
	parms.op[0]=VpItr+1;
	parms.op[1]= 0; 

	for(i=0;i<prefixNum;i++){
		parms.op[1]= i+1; 
		pPrefixInfoObj=(VoiceProfNumPlanPrefixInfoObj_t*) zyMalloc(sizeof(VoiceProfNumPlanPrefixInfoObj_t));
		ret=tr104LoadMdmVoiceProfNumPlanPrefixCfg(&parms, pPrefixInfoObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(pPrefixInfoObj);
			pPrefixInfoObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceProfSipRespCfg ERROR\n");
			return TR104_ERROR;
		}	
		pPrefixInfoObj->parent_VoiceProfNumPlanObj=ProfNumPlan;
		tr104LstAddElmTail(ProfNumPlan->PrefixInfoTab, pPrefixInfoObj);	
	}			

	return TR104_SUCCESS;
}

#ifndef SUPPORT_412_VOICE /*Not Support*/
/***************************************************************************
** Function Name: tr104VoiceProfToneObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int  
tr104VoiceProfToneObjLoadConfig( int VpItr  ,VoiceProfToneObj_t*	pProfTone)
{
	voiceParams_t parms;
	
	TR104_DBG("call tr104VoiceProfToneObjLoadConfig ");
	parms.op[0]=VpItr+1;
	tr104LoadMdmVoiceProfToneCfg(&parms, pProfTone);

	tr104VoiceProfToneEventTabLoadConfig(VpItr);
	tr104VoiceProfToneDescriptionTabLoadConfig(VpItr);
	tr104VoiceProfTonePatternTabLoadConfig(VpItr);
	return TR104_SUCCESS;
}



/***************************************************************************
** Function Name: tr104VoiceProfToneEventTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceProfToneEventTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i,eventNum;

	VoiceProfToneEventObj_t*	 toneEventObj;

	VoiceProfToneObj_t*	pProfTone=tr104VoiceProfToneObjPtrGet(VpItr);
	TR104_DBG("call tr104VoiceProfToneEventTabLoadConfig ");

	if(pProfTone==NULL){
		ZyPrintf("pProfTone Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((pProfTone->toneEventTab->size_)!=0){
		tr104LstAllElmClear(pProfTone->toneEventTab);
	}

	parms.op[0]=VpItr+1;

	eventNum=pProfTone->eventNumberOfEntries;
	TR104_DBG1("eventNum= ",eventNum);
	parms.op[1]= 0; 

	for(i=0;i<eventNum;i++){
		parms.op[1]= i+1; 
		toneEventObj=(VoiceProfToneEventObj_t*) zyMalloc(sizeof(VoiceProfToneEventObj_t));
		ret=tr104LoadMdmVoiceProfToneEventCfg(&parms, toneEventObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(toneEventObj);
			toneEventObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceProfToneEventCfg ERROR \n");
			return TR104_ERROR;
		}		
		toneEventObj->parent_VoiceProfToneObj=pProfTone;
		tr104LstAddElmTail(pProfTone->toneEventTab, toneEventObj);			
	}			
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceProfToneDescriptionTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceProfToneDescriptionTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i, descriptionNum;

	VoiceProfToneDescriptionObj_t*	toneDescriptionObj;
	
	VoiceProfToneObj_t*	pProfTone=tr104VoiceProfToneObjPtrGet(VpItr);

	TR104_DBG("call tr104VoiceProfToneDescriptionTabLoadConfig ");
	
	if(pProfTone==NULL){
		ZyPrintf("pProfTone Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((pProfTone->toneDescriptionTab->size_)!=0){
		tr104LstAllElmClear(pProfTone->toneDescriptionTab);
	}
	parms.op[0]=VpItr+1;

	descriptionNum=pProfTone->descriptionNumberOfEntries;
	TR104_DBG1("descriptionNum= ",descriptionNum);

	parms.op[1]= 0; 

	for(i=0;i<descriptionNum;i++){
		parms.op[1]= i+1; 
		toneDescriptionObj=(VoiceProfToneDescriptionObj_t*) zyMalloc(sizeof(VoiceProfToneDescriptionObj_t));
		ret=tr104LoadMdmVoiceProfToneDescriptionCfg(&parms, toneDescriptionObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(toneDescriptionObj);
			toneDescriptionObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceProfToneDescriptionCfg ERROR \n");
			return TR104_ERROR;
		}	
		toneDescriptionObj->parent_VoiceProfToneObj=pProfTone;
		tr104LstAddElmTail(pProfTone->toneDescriptionTab, toneDescriptionObj);	
	}	
	return TR104_SUCCESS;
}


/***************************************************************************
** Function Name: tr104VoiceProfTonePatternTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceProfTonePatternTabLoadConfig(int VpItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i, patternNum;

	VoiceProfTonePatternObj_t*	tonePatternObj;
	tr104ObjLst_t*	listTmp=NULL;
	
	VoiceProfToneObj_t*	pProfTone=tr104VoiceProfToneObjPtrGet(VpItr);

	TR104_DBG("call tr104VoiceProfTonePatternTabLoadConfig ");
	
	if(pProfTone==NULL){
		ZyPrintf("pProfTone Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((pProfTone->tonePatternTab->size_)!=0){
		tr104LstAllElmClear(pProfTone->tonePatternTab);
	}

	parms.op[0]=VpItr+1;

	patternNum=pProfTone->patternNumberOfEntries;
	TR104_DBG1("patternNum= ",patternNum);

	parms.op[1]= 0; 
	listTmp=pProfTone->tonePatternTab;
	for(i=0;i<patternNum;i++){
		parms.op[1]= i+1; 
		tonePatternObj=(VoiceProfTonePatternObj_t*) zyMalloc(sizeof(VoiceProfTonePatternObj_t));
		ret=tr104LoadMdmVoiceProfTonePatternCfg(&parms, tonePatternObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(tonePatternObj);
			tonePatternObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceProfTonePatternCfg ERROR \n");
			break;
		}	
		tonePatternObj->parent_VoiceProfToneObj=pProfTone;		
		tr104LstAddElmTail(listTmp, tonePatternObj);	
	}		
	
	return TR104_SUCCESS;
}

#endif



/***************************************************************************
** Function Name: tr104VoiceProfLineObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
/*
need modify when mutiple Voice Profile
*/
int 
tr104VoiceProfLineObjLoadConfig(int VpItr , int LineItr)
{
	voiceParams_t  parms ;
	VoiceProfLineObj_t*  Line = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t*  	lineSip = Line->line_sip;
	VoiceProfLineRingerObj_t*	ringer = Line->ringer;
	VoiceProfLineCallingFeaturesObj_t*	callingFeatures = Line->callingFeatures;
	VoiceProfLineVoiceProcessingObj_t*	voiceProcessing = Line->voiceProcessing;
	VoiceProfLineCodecObj_t*	lineCodec = Line->lineCodec;	
	VoiceProfLineStatsObj_t*	lineStats = Line->lineStatistics;
	
	TR104_DBG("call tr104VoiceProfLineObjLoadConfig ");
	
/*
NOTICE : BRCM		vpInst : 	voice profile index   ,  1 , 2 , 3 ,....
		TR104		vpSelect: voice profile index  ,  0 , 1 , 2 , ...
		BRCM		lineInst : Line index		,  1 , 2 , 3 ....			
		TR104		LineItr  : Line index  		,  0 , 1 , 2 , ...
*/		
	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;
		
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);
	
	tr104LoadMdmVoiceLineCfg(&parms, Line);
	
	tr104VoiceLineSipObjLoadConfig(VpItr,LineItr,lineSip);
	#ifndef SUPPORT_412_VOICE /*Not Support*/
	tr104VoiceLineRingerObjLoadConfig(VpItr,LineItr,ringer);
	#endif
	tr104VoiceLineCodecObjLoadConfig(VpItr,LineItr,lineCodec);
	tr104LoadMdmVoiceLineCallingFeaturesCfg(&parms,callingFeatures);
	tr104LoadMdmVoiceLineProcessingCfg(&parms,voiceProcessing);
	/*need modify*/

	tr104VoiceLineSessionLstLoadConfig(VpItr ,  LineItr);
	
	//tr104LineSetProfSelect(VpItr,LineItr);
	
	lineSip->parent_VoiceProfLineObj=(void*) Line;
	ringer->parent_VoiceProfLineObj=(void*) Line;
	lineCodec->parent_VoiceProfLineObj=(void*) Line;
	callingFeatures->parent_VoiceProfLineObj=(void*) Line;
	voiceProcessing->parent_VoiceProfLineObj=(void*) Line;
	lineStats->parent_VoiceProfLineObj=(void*) Line;

	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceLineSessionLstLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int 
tr104VoiceLineSessionLstLoadConfig(int VpItr , int LineItr)
{
	CmsRet	ret;
	int idx;
	voiceParams_t  parms ;

	VoiceProfLineSessionObj_t*	lineSessionObj=NULL;
	tr104ObjLst_t*	listTmp = NULL;	

	VoiceProfLineObj_t*  Line = tr104VoiceLineObjPtrGet(VpItr, LineItr);

	TR104_DBG("call tr104VoiceLineSessionLstLoadConfig ");
	
	if(Line == NULL){
		ZyPrintf("Line Object not ready\n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((Line->sessionLst->size_)!=0){
		tr104LstAllElmClear(Line->sessionLst);
	}

	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;
		
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);
	
	ret=CMSRET_SUCCESS;
	parms.op[2]= 0; 
	listTmp=Line->sessionLst;
	for(idx=0;ret==CMSRET_SUCCESS;idx++){	
		parms.op[2]= idx+1;
		lineSessionObj=(VoiceProfLineSessionObj_t*) zyMalloc(sizeof(VoiceProfLineSessionObj_t));
		ret = tr104LoadMdmVoiceLineSessionCfg(&parms , lineSessionObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(lineSessionObj);
			lineSessionObj = NULL;
		}else{
			tr104LstAddElmTail(listTmp , lineSessionObj);
		}
	}
		
	return TR104_SUCCESS;
}


/***************************************************************************
** Function Name: tr104VoiceLineSipObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceLineSipObjLoadConfig (int VpItr , int LineItr,VoiceProfLineSipObj_t*  obj)
{
	voiceParams_t parms;
	
	TR104_DBG("call tr104VoiceLineSipObjLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);
	parms.op[0]= VpItr+1;
	parms.op[1]= LineItr+1;
	
	tr104LoadMdmVoiceLineSipCfg(&parms,obj);
#ifndef SUPPORT_412_VOICE /*Not Support*/		
	tr104VoiceLineSipEventSubTabLoadConfig(VpItr , LineItr);
#endif

	return TR104_SUCCESS;
}

#ifndef SUPPORT_412_VOICE/*Not Support*/
/***************************************************************************
** Function Name: tr104VoiceLineSipEventSubTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int
tr104VoiceLineSipEventSubTabLoadConfig(int VpItr , int LineItr)
{
	CmsRet ret ;
	int i, sipEventSubNum ;
	voiceParams_t parms;
	VoiceProfLineSipEventSubObj_t*	pLineSipEventSuboObj=NULL;
	tr104ObjLst_t*	listTmp=NULL;

	
	VoiceProfLineSipObj_t*  obj=tr104VoiceLineSipObjPtrGet(VpItr, LineItr);

	TR104_DBG("call tr104VoiceLineSipEventSubTabLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);
	
	if(obj==NULL){
		ZyPrintf("VoiceProfLineSip Object not ready\n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((obj->sipEventSubTab->size_)!=0){
		tr104LstAllElmClear(obj->sipEventSubTab);
	}

	parms.op[0]= VpItr+1;
	parms.op[1]= LineItr+1;
	
	sipEventSubNum=obj->SIPEventSubscribeNumberOfElements;
	TR104_DBG1("sipEventSubNum=",sipEventSubNum);
	
	parms.op[2]= 0; 
	listTmp=obj->sipEventSubTab;
	for(i=0;i<sipEventSubNum;i++){
		parms.op[2]= i+1; 
		pLineSipEventSuboObj=(VoiceProfLineSipEventSubObj_t*) zyMalloc(sizeof(VoiceProfLineSipEventSubObj_t));	
		if((ret=tr104LoadMdmVoiceLineSipEventSubCfg(&parms,  pLineSipEventSuboObj))!=CMSRET_SUCCESS){
			zyFree(pLineSipEventSuboObj);
			pLineSipEventSuboObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceLineSipEventSubCfg ERROR \n");
			return TR104_ERROR;
		}
		pLineSipEventSuboObj->parent_VoiceProfLineSipObj=obj;
		tr104LstAddElmTail(listTmp , pLineSipEventSuboObj);
	}
		
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceLineRingerObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceLineRingerObjLoadConfig(int VpItr , int LineItr,VoiceProfLineRingerObj_t*  obj)
{
	voiceParams_t parms;
	
	TR104_DBG("call tr104VoiceLineRingerObjLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);	
	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;
	
	tr104LoadMdmVoiceLineRingerCfg(&parms, obj);
	
	tr104VoiceLineRingerEventTabLoadConfig( VpItr ,  LineItr);
	tr104VoiceLineRingerDescriptionTabLoadConfig( VpItr ,  LineItr);
	tr104VoiceLineRingerPatternTabLoadConfig( VpItr ,  LineItr);
	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoiceLineRingerEventTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceLineRingerEventTabLoadConfig(int VpItr , int LineItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i, eventNum;
	VoiceProfLineRingerEventObj_t*	 RingerEventObj=NULL;
	tr104ObjLst_t*	listTmp=NULL;	
	VoiceProfLineRingerObj_t*  obj=tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);

	TR104_DBG("call tr104VoiceLineRingerEventTabLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);

	if(obj==NULL){
		ZyPrintf("VoiceProfLineRinger Object not ready\n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if(obj->ringerEventTab->size_!=0){
		tr104LstAllElmClear(obj->ringerEventTab);
	}
	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;

	eventNum=obj->eventNumberOfEntries;
	TR104_DBG1("eventNum = ",eventNum);
	parms.op[2]= 0; 
	listTmp=obj->ringerEventTab;
	for(i=0;i<eventNum;i++){
		parms.op[2]= i+1; 
		RingerEventObj=(VoiceProfLineRingerEventObj_t*) zyMalloc(sizeof(VoiceProfLineRingerEventObj_t));
		ret=tr104LoadMdmVoiceLineRingerEventCfg(&parms, RingerEventObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(RingerEventObj);
			RingerEventObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceLineRingerEventCfg ERROR \n");
			return TR104_ERROR;
		}		
		RingerEventObj->parent_VoiceProfLineRingerObj=obj;
		tr104LstAddElmTail(listTmp, RingerEventObj);			
	}	


	return TR104_SUCCESS;
}


/***************************************************************************
** Function Name: tr104VoiceLineRingerDescriptionTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceLineRingerDescriptionTabLoadConfig(int VpItr , int LineItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i,eventNum,descriptionNum,patternNum;
	VoiceProfLineRingerDescriptionObj_t*	RingerDescriptionObj;
	tr104ObjLst_t*	listTmp=NULL;
	VoiceProfLineRingerObj_t*  obj=tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);

	TR104_DBG("call tr104VoiceLineRingerDescriptionTabLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);	
	
	if(obj==NULL){
		ZyPrintf("VoiceProfLineRinger Object not ready\n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((obj->ringerDescriptionTab->size_)!=0){
		tr104LstAllElmClear(obj->ringerDescriptionTab);
	}

	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;
	

	descriptionNum=obj->descriptionNumberOfEntries;


	TR104_DBG1("descriptionNum = ",descriptionNum);	
	parms.op[2]= 0;
	listTmp=obj->ringerDescriptionTab;
	for(i=0;i<descriptionNum;i++){
		parms.op[2]= i+1; 
		RingerDescriptionObj=(VoiceProfLineRingerDescriptionObj_t*) zyMalloc(sizeof(VoiceProfLineRingerDescriptionObj_t));
		ret=tr104LoadMdmVoiceLineRingerDescriptionCfg(&parms, RingerDescriptionObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(RingerDescriptionObj);
			RingerDescriptionObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceLineRingerDescriptionCfg ERROR \n");
			return TR104_ERROR;
		}	
		RingerDescriptionObj->parent_VoiceProfLineRingerObj=obj;
		tr104LstAddElmTail(listTmp, RingerDescriptionObj);	
	}	

	return TR104_SUCCESS;
}



/***************************************************************************
** Function Name: tr104VoiceLineRingerPatternTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceLineRingerPatternTabLoadConfig(int VpItr , int LineItr)
{
	CmsRet ret ;
	voiceParams_t parms;
	int i, patternNum;
	VoiceProfLineRingerPatternObj_t*	RingerPatternObj;
	tr104ObjLst_t*	listTmp=NULL;

	VoiceProfLineRingerObj_t*  obj=tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);
	TR104_DBG("call tr104VoiceLineRingerPatternTabLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);	
	
	if(obj==NULL){
		ZyPrintf("VoiceProfLineRinger Object not ready \n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((obj->ringerPatternTab->size_)!=0){
		tr104LstAllElmClear(obj->ringerPatternTab);
	}

	parms.op[0]=VpItr+1;
	parms.op[1]=LineItr+1;
	
	patternNum=obj->patternNumberOfEntries;

	TR104_DBG1("patternNum = ",patternNum);	
	parms.op[2]= 0;
	listTmp=obj->ringerPatternTab;
	for(i=0;i<patternNum;i++){
		parms.op[2]= i+1; 
		RingerPatternObj=(VoiceProfLineRingerPatternObj_t*) zyMalloc(sizeof(VoiceProfLineRingerPatternObj_t));
		ret=tr104LoadMdmVoiceLineRingerPatternCfg(&parms, RingerPatternObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(RingerPatternObj);
			RingerPatternObj = NULL;
			ZyPrintf(" tr104LoadMdmVoiceLineRingerPatternCfg ERROR \n");
			return TR104_ERROR;
		}			
		RingerPatternObj->parent_VoiceProfLineRingerObj=obj;
		tr104LstAddElmTail(listTmp, RingerPatternObj);	
	}

	return TR104_SUCCESS;
}
#endif
/***************************************************************************
** Function Name: tr104VoiceLineCodecObjLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/

int  
tr104VoiceLineCodecObjLoadConfig (int VpItr , int LineItr,VoiceProfLineCodecObj_t*  obj)
{
	voiceParams_t parms;
	
	TR104_DBG("call tr104VoiceLineCodecObjLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);	
	parms.op[0]= VpItr+1;
	parms.op[1]= LineItr+1;
	
	tr104LoadMdmVoiceLineCodecCfg(&parms,obj);

	tr104VoiceLineCodecsTabLoadConfig(VpItr , LineItr);
	
	return TR104_SUCCESS;
}


/***************************************************************************
** Function Name: tr104VoiceLineCodecsTabLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoiceLineCodecsTabLoadConfig(int VpItr , int LineItr)
{

	CmsRet ret ;
	int idx ;
	voiceParams_t parms;
	VoiceProfLineCodecListObj_t*	lineCodecLstObj=NULL;
	tr104ObjLst_t*	listTmp=NULL;

	
	VoiceProfLineCodecObj_t*  obj=tr104VoiceLineCodecObjPtrGet(VpItr, LineItr);

	TR104_DBG("call tr104VoiceLineCodecsTabLoadConfig ");
	TR104_DBG2("VpItr = ",VpItr,"LineItr =",LineItr);	
	
	if(obj==NULL){
		ZyPrintf("VoiceProfLineCodec Object not ready\n");
		return TR104_ERROR;
	}
	
	/*clear the list before loading the config from mdm*/
	if((obj->codecList->size_)!=0){
		tr104LstAllElmClear(obj->codecList);
	}

	parms.op[0]= VpItr+1;
	parms.op[1]= LineItr+1;
	
	ret=CMSRET_SUCCESS;
	parms.op[2]= 0; 
	listTmp=obj->codecList;
	for(idx=0;ret==CMSRET_SUCCESS;idx++){	
		parms.op[2]= idx+1;
		TR104_DBG1(" number of Line Sip Codec = ",idx);
		lineCodecLstObj=(VoiceProfLineCodecListObj_t*) zyMalloc(sizeof(VoiceProfLineCodecListObj_t));
		ret = tr104LoadMdmVoiceLineCodecListCfg(&parms , lineCodecLstObj);
		if(ret!=CMSRET_SUCCESS){
			zyFree(lineCodecLstObj);	
			lineCodecLstObj = NULL;			
		}else{
			lineCodecLstObj->parent_VoiceProfLineCodecObj=obj;
			tr104LstAddElmTail(listTmp , lineCodecLstObj);
		}				
	}
	return TR104_SUCCESS;
}
/***************************************************************************
** Function Name: tr104VoicePhyIntfLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoicePhyIntfLoadConfig(int phyIntfIdx)
{
	CmsRet ret;
	voiceParams_t  parms ;
	VoicePhyIntfObj_t* phyInterface = (VoicePhyIntfObj_t*) tr104VoicePhyIntfObjPtrGet(phyIntfIdx);
	//VoicePhyIntfTests_t*	PhyInterfaceTest = phyInterface->PhyInterfaceTest;
	
	TR104_DBG("call tr104VoicePhyIntfLoadConfig ");	
	parms.op[0]= phyIntfIdx+1;
	ret=tr104LoadMdmVoicePhyIntfCfg(&parms, phyInterface);
	if(ret!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104LoadMdmVoicePhyIntfCfg : %d \n",ret);	
		return ret;
	}
	

	return TR104_SUCCESS;
	
}


/***************************************************************************
** Function Name: tr104VoiceFxoLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoiceFxoLoadConfig(int fxoIdx)
{
	CmsRet ret;
	voiceParams_t  parms ;
	ZyXEL_VoiceFXO_t* ZyXEL_VoiceFXO = (ZyXEL_VoiceFXO_t*) tr104VoiceFXOPtrGet(fxoIdx);
	ZyXEL_VoiceFXOPort_t*	fxoPort = ZyXEL_VoiceFXO->fxoPort;
	
	TR104_DBG("call tr104VoiceFxoLoadConfig ");	
	parms.op[0]= fxoIdx+1;
	ret=tr104LoadMdmVoiceFXOCfg(&parms,ZyXEL_VoiceFXO);
	if(ret!=CMSRET_SUCCESS){
		ZyPrintf("\n tr104LoadMdmVoiceFXOCfg : %d \n",ret);	
		return ret;
	}

	tr104LoadMdmVoiceFXOPortCfg(&parms,fxoPort);

	ZyXEL_VoiceFXO->parent_VoiceServiceObj = (void*) tr104VoiceServiceObjPtrGet();
	
	return TR104_SUCCESS;
	
}

/***************************************************************************
** Function Name: tr104VoicePSTNLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int 
tr104VoicePSTNLoadConfig(void)
{
	CmsRet ret; 
	int pstnIndex=0;
	voiceParams_t  parms ;
	ZyXEL_VoicePSTN_t* PSTN =  (ZyXEL_VoicePSTN_t*)tr104VoicePSTNPtrGet(pstnIndex);
	TR104_DBG("call tr104VoicePSTNLoadConfig ");	
	parms.op[0]=pstnIndex+1;
	if((ret=tr104LoadMdmVoiceZYXELPstnCfg(&parms,PSTN))!=CMSRET_SUCCESS){
	ZyPrintf("\n tr104VoicePSTNLoadConfig : %d \n",ret);
	}	
	return TR104_SUCCESS;
}

/***************************************************************************
** Function Name: tr104VoicePhoneBookLoadConfig
**
** Description  : load config from MDM by calling the APIs in tr104_voicemdm.c  
**                           
** Returns      : 
****************************************************************************/
int
tr104VoicePhoneBookLoadConfig(
	void
){
	int i=0;
	CmsRet ret; 
	voiceParams_t  parms ;
	ZyXEL_VoicePhoneBook_t* PhoneBookObj=NULL ;

	for(i=0;i<TR104_VPHONE_BOOK_CNT;i++){
		
		PhoneBookObj = tr104VoicePhoneBookPtrGet(i);	
		parms.op[0]=i+1;		
		ret=tr104LoadMdmVoicePhoneBookCfg(&parms,PhoneBookObj);
		TR104_DBG2("\n tr104VoicePhoneBookLoadConfig ( ",parms.op[0]," ) : ",ret);	
	}
	return TR104_SUCCESS;
}
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104ReloadAll
**
** Description  : reload TR104 Voice Service configs 
**                           
** Returns      : 
****************************************************************************/
void 
tr104ReloadAll(void)
{
	int VpItr=0,LineItr=0,PhyIdx=0,FxoIdx=0,SipAccountId = 0,ret=0;
	uint32 NumOfProfile=0 , NumOfLine=0 ,NumOfPhyIntf=0 ,NumOfFxo=0;

	TR104_PRINTF("Reload TR104 Voice Service configs start \n");
	/*Step 0 : load Configs of the VoiceService object */
	/*type0 Object Reload*/
	if((ret=tr104VoiceServiceObjLoadConfig())!=TR104_SUCCESS){
		ZyPrintf(" tr104VoiceServiceObjLoadConfig not success: ret = %d \n",ret);
		return;
	}	
	/*Step 1 : Init voiceCapabilities and load config*/
	if((ret=tr104VoiceCapObjLoadConfig())!=TR104_SUCCESS){
		ZyPrintf(" tr104VoiceCapObjLoadConfig not success: ret = %d \n",ret);
		return;
	}
	
	/*Step 2 :*/
	/* Init voice profile and load config*/
	NumOfProfile = tr104GetNumProfile();
	for(VpItr=0;VpItr<NumOfProfile;VpItr++){
		if((ret=tr104VoiceProfileObjLoadConfig(VpItr))!=TR104_SUCCESS){
			ZyPrintf(" tr104VoiceProfileObjLoadConfig not success: ret = %d , VpItr = %d \n",ret,VpItr);
			return;
		}else{
			NumOfLine = tr104GetVPNumOfLine(VpItr);
			for(LineItr=0;LineItr<NumOfLine;LineItr++){
				if((ret=tr104VoiceProfLineObjLoadConfig(VpItr,LineItr))!=TR104_SUCCESS){
					ZyPrintf(" tr104VoiceProfLineObjLoadConfig not success: ret = %d ,  VpItr = %d , LineItr = %d\n",ret,VpItr,LineItr);
					return;
				}
			}
		}
	}	
	
	/*Step 3 : Init ZyXEL_voiceCommon and load config*/
	if((ret=tr104VoiceCommonLoadConfig())!=TR104_SUCCESS){
		ZyPrintf(" tr104VoiceCommonLoadConfig not success: ret = %d \n",ret);
		return;
	}

	/*Step 4 : Init ZyXEL_VoiceSipAccount and load config*/
	for(SipAccountId=0;SipAccountId<sipAccountNumber;SipAccountId++){
		VoiceProfObj_t* Proftmp= NULL;
		VoiceProfLineObj_t* Linetmp= NULL;
		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);
		Proftmp = tr104VoiceProfObjPtrGet(VpItr);
		Linetmp = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		ZyXEL_VoiceSipAccount[SipAccountId].enable = (Linetmp->enable) & (Proftmp->enable);
		ZyXEL_VoiceSipAccount[SipAccountId].sipAccountId = Linetmp->ZyXEL_CMAcntNum;
		ZyXEL_VoiceSipAccount[SipAccountId].SPId = Proftmp->ZyXEL_SPNum;
		ZyXEL_VoiceSipAccount[SipAccountId].LineItr= LineItr;
		ZyXEL_VoiceSipAccount[SipAccountId].VpItr= VpItr;
		ZyXEL_VoiceSipAccount[SipAccountId].flag = 0;
		ZyXEL_VoiceSipAccount[SipAccountId].ptrProf = tr104VoiceProfObjPtrGet(VpItr);
		ZyXEL_VoiceSipAccount[SipAccountId].ptrLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);	
	}

	/*Step 4 : Init VoicePhyIntf and load config*/
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	for(PhyIdx=0;PhyIdx<NumOfPhyIntf;PhyIdx++){
		if((ret = tr104VoicePhyIntfLoadConfig(PhyIdx))!=TR104_SUCCESS){
			ZyPrintf(" tr104VoicePhyIntfLoadConfig not success: ret = %d , PhyIdx = %d \n",ret,PhyIdx);
			break;
		}
	}

	/*Step 5 : Init ZyXEL_VoiceFXO and load config*/
	NumOfFxo = tr104GetNumOfFXO();
	for(FxoIdx=0;FxoIdx<NumOfFxo;FxoIdx++){
		if((ret = tr104VoiceFxoLoadConfig(FxoIdx))!=TR104_SUCCESS){
			ZyPrintf(" tr104VoicePhyIntfLoadConfig not success: ret = %d , FxoIdx = %d \n",ret,FxoIdx);
			break;
		}
	}

	
/*Step 6 : Init VoicePSTN ,VoicePhoneBook and load config*/
	tr104VoicePhoneBookLoadConfig();
#if defined(FXO) || defined(FXO_DSPRELAY)	
	/*need modify , and VoicePhoneBook not ready*/
	tr104VoicePSTNLoadConfig();
#endif

	
	if(Tr104Debug)	{	
		tr104DumpAll();
	}
	TR104_PRINTF("reload TR104 Voice Service configs end  \n");
}

/***************************************************************************
** Function Name: tr104DumpAll
**
** Description  : Dump All Tr104 Voice Objects  
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpAll(void)
{
	ZyPrintf("\n  ********* Dump All Tr104 Voice Objects start ************ \n ");
	int VpItr,LineItr,PhyIdx,FxoIdx=0,SipAccountId,PhoneBookIdx,PSTNIdx;
	uint32 NumOfProfile, NumOfLine, NumOfPhyIntf, NumOfFxo;
	
	NumOfProfile = tr104GetNumProfile();
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	NumOfFxo = tr104GetNumOfFXO();

	tr104DumpVoiceServiceObj();
/*Dump VoiceCommonObj and VoiceCapabilities*/	
	tr104DumpVoiceCommonObj();
	tr104DumpVoiceCapObj();
	tr104DumpVoiceCapSipObj();
	tr104DumpVoiceCapCodecTab();

/*Dump VoiceProfile and VoiceLine*/	
	for(VpItr=0;VpItr<NumOfProfile;VpItr++){
		tr104DumpVoiceProfileObj(VpItr);
		tr104DumpVoiceProfSipObj(VpItr);
		tr104DumpVoiceProfSipEventSubTab(VpItr);
		tr104DumpVoiceProfSipRespMapTab(VpItr);		
		tr104DumpVoiceProfRtpObj(VpItr);
		tr104DumpVoiceProfRtpRtcpObj(VpItr);
		tr104DumpVoiceProfRtpSrtpObj(VpItr);
		tr104DumpVoiceProfRtpRedundancyObj(VpItr);		
		tr104DumpVoiceProfFaxT38Obj(VpItr);
		tr104DumpVoiceProfNumPlanObj(VpItr);
		tr104DumpVoiceProfNumPlanPrefixinfoTab(VpItr);
		NumOfLine = tr104GetVPNumOfLine(VpItr);
		for(LineItr=0;LineItr<NumOfLine;LineItr++){
			tr104DumpVoiceLineObj(VpItr, LineItr);
			tr104DumpVoiceLineSipObj(VpItr, LineItr);
			tr104DumpVoiceLineSipEventSubTab(VpItr, LineItr);
			tr104DumpVoiceLineRingerObj(VpItr, LineItr);
			tr104DumpVoiceLineRingerEventTab(VpItr, LineItr);
			tr104DumpVoiceLineRingerDesriptionTab(VpItr, LineItr);
			tr104DumpVoiceLineRingerPatternTab(VpItr, LineItr);

			tr104DumpVoiceLineCallingFeaturesObj(VpItr, LineItr);
			tr104DumpVoiceLineProcessingObj(VpItr, LineItr);
			tr104DumpVoiceLineCodecObj(VpItr, LineItr);
			tr104DumpVoiceLineCodecList(VpItr, LineItr);
		}
	}

/*Dump VoicePhyIntf*/
	for(PhyIdx=0;PhyIdx<NumOfPhyIntf;PhyIdx++){
		tr104DumpVoicePhyIntfObj(PhyIdx);
	}

/*Dump VoiceFxo and VoiceFxoPort*/
		tr104DumpVoiceFxoObj(FxoIdx);
	for(FxoIdx=0;FxoIdx<NumOfFxo;FxoIdx++){
		ZyPrintf("==> Fxo : %d \n",FxoIdx);
		tr104DumpVoiceFxoPortObj(FxoIdx);
	}

/*Dump VoiceSipAccount*/
	for(SipAccountId=0;SipAccountId<sipAccountNumber;SipAccountId++){
		tr104DumpVoiceSipAccountObj(SipAccountId);
	}

/*Dump VoicePhoneBook*/		
	for ( PhoneBookIdx=0; PhoneBookIdx<TR104_VPHONE_BOOK_CNT; PhoneBookIdx++ ) {
		tr104DumpVoicePhoneBookObj(PhoneBookIdx);
	}


	for(PSTNIdx=0; PSTNIdx<TR104_VPSTN_CNT;PSTNIdx++) {
		tr104DumpVoicePSTNObj(PSTNIdx);
	}
		
	ZyPrintf("\n  ********* Dump All Tr104 Voice Objects end ************ \n ");
}

/******************************************************************************
**	Free Function of TR104
**
******************************************************************************/

/***************************************************************************
** Function Name: tr104ModuleFree
**
** Description  : Free the Tr104 data-Model in turn when Tr104 Data-Modle initial error .  
**                		  step1.Free tr104VoiceService->ZyXEL_voiceCommon and the child objects 
**				  step2.Free tr104VoiceService->ZyXEL_VoiceFXO and the child objects
**				  step3.Free tr104VoiceService->phyInterface and the child objects
**				  step4.Free tr104LineArray and the child objects
**				  step5.Free tr104VoiceService->voiceProfObjArray and the child objects
**				  step6.Free VoicePSTN and the child objects					
**				  step7.Free VoicePhoneBook and the child objects
**				  step8.Free tr104VoiceService->voiceCapabilities and the child objects  
**				  step9.Free tr104VoiceService
**				  step10.Free ZyXEL_VoiceSipAccount
** Returns      : 
****************************************************************************/
void
tr104ModuleFree(void)
{
	TR104_DBG("\n  ********* Free All Tr104 Voice Objects start ************ \n ");
	int VpItr=0,LineItr=0, PhyIdx = 0, FxoIdx=0;
	uint32 NumOfProfile=0 , NumOfLine=0 , NumOfPhyIntf = 0 , NumOfFxo = 0;
	
	if(NULL != tr104VoiceService){
		
		TR104_DBG(" tr104VoiceService isn't NULL ");
		TR104_DBG("Free tr104VoiceService and the child objects");
		
		if(NULL != tr104VoiceService->voiceCapabilities){	
			
			TR104_DBG(" tr104VoiceService->voiceCapabilities isn't NULL ");
						
			if(NULL!= tr104VoiceService->ZyXEL_voiceCommon){
				TR104_DBG("Free tr104VoiceService->ZyXEL_voiceCommon and the child objects");
				zyFree(tr104VoiceService->ZyXEL_voiceCommon);
				tr104VoiceService->ZyXEL_voiceCommon = NULL;
			}

			if(NULL!= tr104VoiceService->ZyXEL_VoiceFXO){
				NumOfFxo = tr104GetNumOfFXO();
				TR104_DBG("Free tr104VoiceService->ZyXEL_VoiceFXO and the child objects");
				for(FxoIdx=0;FxoIdx<NumOfFxo;FxoIdx++){
					tr104VoiceFxoChildFree(FxoIdx);
				}
				zyFree(tr104VoiceService->ZyXEL_VoiceFXO);
				tr104VoiceService->ZyXEL_VoiceFXO = NULL;
			}

			if(NULL!= tr104VoiceService->phyInterface){
				NumOfPhyIntf = tr104GetNumOfPhyIntf();
				TR104_DBG("Free tr104VoiceService->phyInterface and the child objects");
				for(PhyIdx=0;PhyIdx<NumOfPhyIntf;PhyIdx++){
					tr104VoicePhyIntfChildFree(PhyIdx);
				}
				zyFree(tr104VoiceService->phyInterface);
				tr104VoiceService->phyInterface = NULL;
			}
			
			if(NULL!= tr104VoiceService->voiceProfObjArray){
				NumOfProfile = tr104GetNumProfile();
				for(VpItr = 0 ; VpItr < NumOfProfile ; VpItr++){	
					NumOfLine = tr104GetVPNumOfLine(VpItr);
					TR104_DBG2("NumOfProfile=",NumOfProfile ,"NumOfLine =",NumOfLine);
					if(NULL != tr104VoiceService->voiceProfObjArray->lineProfPtrLst){
						TR104_DBG("Free tr104VoiceService->voiceProfObjArray->lineProfPtrLst and the child objects");
						for(LineItr = 0 ; LineItr < NumOfLine ; LineItr++){				
							tr104VoiceProfLineObjChildFree(VpItr , LineItr);
						}
						zyFree(tr104VoiceService->voiceProfObjArray->lineProfPtrLst);
						tr104VoiceService->voiceProfObjArray->lineProfPtrLst = NULL;
					}

					TR104_DBG("Free tr104VoiceService->voiceProfObjArray and the child objects");								
					tr104VoiceProfObjChildFree(VpItr);
				}					
				zyFree(tr104VoiceService->voiceProfObjArray);
				tr104VoiceService->voiceProfObjArray = NULL;
			}


			if(NULL != tr104VoiceService->ZyXEL_VoicePSTN){
				TR104_DBG("Free VoicePSTN and the child objects");
				zyFree(tr104VoiceService->ZyXEL_VoicePSTN);
				tr104VoiceService->ZyXEL_VoicePSTN = NULL;
			}
			if(NULL != tr104VoiceService->ZyXEL_VoicePhoneBook){
				TR104_DBG("Free VoicePhoneBook and the child objects");
				zyFree(tr104VoiceService->ZyXEL_VoicePhoneBook);
				tr104VoiceService->ZyXEL_VoicePhoneBook = NULL;
			}

		}
		TR104_DBG("Free tr104VoiceService->voiceCapabilities and the child objects");
		tr104VoiceCapObjChildFree();
		zyFree(tr104VoiceService->voiceCapabilities);
		tr104VoiceService->voiceCapabilities = NULL;
	}	
	zyFree(tr104VoiceService);
	TR104_DBG(" Free tr104VoiceService");
	tr104VoiceService = NULL;
	zyFree(ZyXEL_VoiceSipAccount);
	TR104_DBG(" Free ZyXEL_VoiceSipAccount");
	ZyXEL_VoiceSipAccount = NULL;
	TR104_DBG("\n  ********* Free All Tr104 Voice Objects end ************ \n ");		
}


/***************************************************************************
** Function Name: tr104VoiceCapObjChildFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceCapObjChildFree(void)
{
	VoiceCapObj_t* voiceCapObj = (VoiceCapObj_t*) tr104VoiceCapObjPtrGet();
	if(voiceCapObj != NULL){
		if(voiceCapObj->CapOfSIP!=NULL){
			zyFree(voiceCapObj->CapOfSIP);
			voiceCapObj->CapOfSIP = NULL;
		}
		if(voiceCapObj->CapCodecsTable!=NULL){
			tr104LstFree(voiceCapObj->CapCodecsTable);
			voiceCapObj->CapCodecsTable = NULL;
		}
	}	
}


/***************************************************************************
** Function Name: tr104VoiceProfObjChildFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfObjChildFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);

	if(voiceProfPtr != NULL){
		if(voiceProfPtr->serviceProviderInfo != NULL){
			zyFree(voiceProfPtr->serviceProviderInfo);
			voiceProfPtr->serviceProviderInfo = NULL;
		}
		
		tr104VoiceProfSipObjFree(VpItr);
		tr104VoiceProfRtpObjFree(VpItr);
		tr104VoiceProfNumPlanObjFree(VpItr);
		tr104VoiceProfToneObjFree(VpItr);
		tr104VoiceProfButtonMapObjFree(VpItr);
		
		if(voiceProfPtr->faxT38Obj!=NULL){
			zyFree(voiceProfPtr->faxT38Obj);
			voiceProfPtr->faxT38Obj = NULL;
		}

		if(voiceProfPtr->lineProfPtrLst != NULL){
			zyFree(voiceProfPtr->lineProfPtrLst);
			voiceProfPtr->lineProfPtrLst = NULL;	
		}
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfSipObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfSipObjFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfSipObj_t* this_ = voiceProfPtr->SIPObj;
	if(this_ != NULL){
		if(this_->responseMapTab!=NULL){
			tr104LstFree(this_->responseMapTab);
			this_->responseMapTab = NULL;
		}
		if(this_->eventsubScribeTab!=NULL){
			tr104LstFree(this_->eventsubScribeTab);
			this_->eventsubScribeTab = NULL;
		}
		zyFree(voiceProfPtr->SIPObj);
		voiceProfPtr->SIPObj = NULL;
	}	
}



/***************************************************************************
** Function Name: tr104VoiceProfButtonMapObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfButtonMapObjFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfButtonMapObj_t* obj = NULL;

	if(voiceProfPtr != NULL){ 
		obj = voiceProfPtr->buttonMapObj ;
		if(obj->buttonTab!=NULL){
			tr104LstFree(obj->buttonTab);
			obj->buttonTab = NULL;
		}
		zyFree(voiceProfPtr->buttonMapObj);
		voiceProfPtr->buttonMapObj=NULL;
	}	
}



/***************************************************************************
** Function Name: tr104VoiceProfRtpObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfRtpObjFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfRtpObj_t* this_ = voiceProfPtr->RTP_Profile ;
	if(this_ != NULL){
		if(this_->RTCP_Profile!=NULL){
			zyFree(this_->RTCP_Profile);
			this_->RTCP_Profile = NULL;
		}
		if(this_->SRTP_Profile!=NULL){
			zyFree(this_->SRTP_Profile);
			this_->SRTP_Profile = NULL;
		}
		if(this_->RTPRedundancy!=NULL){
			zyFree(this_->RTPRedundancy);
			this_->RTPRedundancy = NULL;
		}
		zyFree(voiceProfPtr->RTP_Profile);
		voiceProfPtr->RTP_Profile = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfNumPlanObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfNumPlanObjFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfNumPlanObj_t* this_ = voiceProfPtr->numberingPlanObj;
	if(this_ != NULL){
		if(this_->PrefixInfoTab!=NULL){
			tr104LstFree(this_->PrefixInfoTab);
			this_->PrefixInfoTab = NULL;
		}
		zyFree(voiceProfPtr->numberingPlanObj);
		voiceProfPtr->numberingPlanObj = NULL;
	}	
}


/***************************************************************************
** Function Name: tr104VoiceProfToneObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfToneObjFree(int VpItr)
{
	VoiceProfObj_t* voiceProfPtr = (VoiceProfObj_t*)tr104VoiceProfObjPtrGet(VpItr);
	VoiceProfToneObj_t* this_ = voiceProfPtr->toneObj;
	if(this_ != NULL){
		if(this_->toneEventTab!=NULL){
			tr104LstFree(this_->toneEventTab);
			this_->toneEventTab = NULL;
		}
		if(this_->toneDescriptionTab!=NULL){
			tr104LstFree(this_->toneDescriptionTab);
			this_->toneDescriptionTab = NULL;
		}
		if(this_->tonePatternTab!=NULL){
			tr104LstFree(this_->tonePatternTab);
			this_->tonePatternTab = NULL;
		}		
		zyFree(voiceProfPtr->toneObj);
		voiceProfPtr->toneObj = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfLineObjChildFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfLineObjChildFree(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* lineObj = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);

	if(lineObj != NULL){

		tr104VoiceProfLineSipObjFree(VpItr , LineItr);
		tr104VoiceProfLineRingerObjFree(VpItr , LineItr);
		tr104VoiceProfLineCallingFeaturesObjFree(VpItr , LineItr);
		
		if( NULL != lineObj->voiceProcessing ){
			zyFree(lineObj->voiceProcessing);
			lineObj->voiceProcessing = NULL;
		}
	
		tr104VoiceProfLineCodecObjFree(VpItr , LineItr);
		
		if(lineObj->sessionLst!=NULL){
			tr104LstFree(lineObj->sessionLst);
			lineObj->sessionLst = NULL;
		}
		if( NULL != lineObj->lineStatistics ){
			zyFree(lineObj->lineStatistics);
			lineObj->lineStatistics = NULL;
		}		
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfLineCodecObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfLineCodecObjFree(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* lineObj = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineCodecObj_t* this_ = lineObj->lineCodec;
	
	if(this_ != NULL){
		if(this_->codecList!=NULL){
			tr104LstFree(this_->codecList);
			this_->codecList = NULL;
		}
	
		zyFree(lineObj->lineCodec);
		lineObj->lineCodec = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfLineSipObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfLineSipObjFree(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* lineObj = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineSipObj_t* this_ = lineObj->line_sip;
	
	if(this_ != NULL){
		if(this_->sipEventSubTab!=NULL){
			tr104LstFree(this_->sipEventSubTab);
			this_->sipEventSubTab = NULL;
		}
		zyFree(lineObj->line_sip);
		lineObj->line_sip = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfLineRingerObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfLineRingerObjFree(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* lineObj = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineRingerObj_t* this_ = lineObj->ringer;
	
	if(this_ != NULL){
		if(this_->ringerEventTab!=NULL){
			tr104LstFree(this_->ringerEventTab);
			this_->ringerEventTab = NULL;
		}
		if(this_->ringerDescriptionTab!=NULL){
			tr104LstFree(this_->ringerDescriptionTab);
			this_->ringerDescriptionTab = NULL;
		}
		if(this_->ringerPatternTab!=NULL){
			tr104LstFree(this_->ringerPatternTab);
			this_->ringerPatternTab = NULL;
		}		
		zyFree(lineObj->ringer);
		lineObj->ringer = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoiceProfLineCallingFeaturesObjFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceProfLineCallingFeaturesObjFree(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* lineObj = (VoiceProfLineObj_t*)tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VoiceProfLineCallingFeaturesObj_t* this_ = lineObj->callingFeatures;
	
	if(this_ != NULL){		
		zyFree(lineObj->callingFeatures);
		lineObj->callingFeatures = NULL;
	}	
}

/***************************************************************************
** Function Name: tr104VoicePhyIntfChildFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoicePhyIntfChildFree(PhyIdx)
{
	VoicePhyIntfObj_t* phyInterface = (VoicePhyIntfObj_t*) tr104VoicePhyIntfObjPtrGet(PhyIdx);
	if(phyInterface != NULL){
		if(phyInterface->PhyInterfaceTest!=NULL){
			zyFree(phyInterface->PhyInterfaceTest);
			phyInterface->PhyInterfaceTest = NULL;
		}
	}
}

/***************************************************************************
** Function Name: tr104VoiceFxoChildFree
**
** Description  : Free the Tr104 data-Model when Tr104 Data-Modle initial error . 
**				  Need check parent object are still exist .
**                           
** Returns      : 
****************************************************************************/
static void
tr104VoiceFxoChildFree(FxoIdx)
{	
	ZyXEL_VoiceFXO_t* ZyXEL_VoiceFXO = (ZyXEL_VoiceFXO_t*) tr104VoiceFXOPtrGet(FxoIdx);
	if(ZyXEL_VoiceFXO != NULL){
		if(ZyXEL_VoiceFXO->fxoPort!=NULL){
			zyFree(ZyXEL_VoiceFXO->fxoPort);
			ZyXEL_VoiceFXO->fxoPort = NULL;
		}
	}
}

