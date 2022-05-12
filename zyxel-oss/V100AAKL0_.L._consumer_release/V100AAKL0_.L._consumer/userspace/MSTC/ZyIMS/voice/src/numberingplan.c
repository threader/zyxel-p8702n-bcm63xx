/*
  $Id: DialPlan.c 1.2.1.1 2007/07/26 02:37:16 Allen.Yang Exp $
*/

/******************************************************************************/
/*
 *	Copyright (C) 1994 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 */
/******************************************************************************/

/*
  $Log: DialPlan.c $
*/


#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "switch.h"
#include "global.h"
#include "sys_msg.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "tr104_object.h"
#include "tr104.h"
#include "numberingplan.h"
/*Jason , syslog*/
#ifdef VOIP_SYSLOG
#include "syslog.h"
#include "voip_syslog.h"
#endif


/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Variable Declaration
%%____________________________________________________________________________*/



/*______________________________________________________________________________
**  checkNumberingPlanPhoneFeature
**
**  descriptions: NumberingPlan callback function
**  parameters:  
**  local:
**  global:
**  return: 
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

void
checkNumberingPlanPhoneFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag) {

	char *tmpStr = NULL;
	int featureLen = 0;
	int ret = CALL_FEATURE_NUMBERS;
	int i=0;
	int remove_featurekey=1;
	uint16 phonePhyId;
	phoneCh_t *tmpPhoneCB = NULL;
	tr104ObjLst_t* prefixInfoTab=NULL;
	tr104LstElm_t* iter=NULL;
	VoiceProfNumPlanPrefixInfoObj_t* prefixInfoObj=NULL;
	int SipAccountId = 0, VpItr = 0;

#ifdef VOIP_SYSLOG /*Jason , syslog*/	
	VoiceProfLineObj_t *pLine =NULL;
#endif
	CCM_PRINTF("checkNumberingPlanPhoneFeature \n");

	/*TBD: read VoiceService.{i}.Capabilities.NumberingPlan   (enable / disable numberingplan )*/
	
	tmpStr = (char *)zyMalloc(MAX_DIAL_STRING_LENGTH);
	zyMemset(tmpStr, 0, MAX_DIAL_STRING_LENGTH);
	zyMemcpy(tmpStr, &(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), ccmConn->dialInform.dialDigitNumber.current_length);
	tmpPhoneCB = find_phoneCB(ccmConn->origPortId, 0);
#ifdef VOIP_SYSLOG /*Jason , syslog*/	
	#if 1 /*Eason, get registered line obj then enabled line obj*/
	pLine = getAssociatedRegLineObj(tmpPhoneCB);	
	#else
	pLine = getAssociatedLineObj(tmpPhoneCB);	
	#endif
#endif
	SipAccountId = getAssociateRegedLineId(tmpPhoneCB);
	VpItr = mapSipAccountIdToVpItr(SipAccountId);	
	prefixInfoTab= tr104VoiceProfNumPlanPrefixInfoTabPtrGet(VpItr);
	iter=(prefixInfoTab)?prefixInfoTab->head_:NULL;
	*retCode=ret;
	CCM_PRINTF("%s, dail digits = %s, len = %d\n",__FUNCTION__, tmpStr, zyStrlen(tmpStr));
	/* if no prefixinfo table, return */	
	if(prefixInfoTab==NULL){
		CCM_PRINTF("Prefix Table is NULL\n");
	//	return ret;
	return;
	}
	/*check prefixinfo table */
	for(i=0;i<prefixInfoTab->size_;i++){	
		if(iter->value_==NULL){
			CCM_PRINTF("value_ is NULL\n");
			return ;
		}
		prefixInfoObj=iter->value_;		
#ifdef DBG_NUMBERLING_PLAN		
		CCM_PRINTF("\r\n ---- item %d --- ",i);
		CCM_PRINTF("\n  prefixRange = %s \n ", prefixInfoObj->prefixRange);
		CCM_PRINTF("\n  facilityAction = %s \n ", prefixInfoObj->facilityAction);
		CCM_PRINTF("\n prefixRange length = %d\n",zyStrlen(prefixInfoObj->prefixRange));
#endif		
		if ((zyStrlen(prefixInfoObj->prefixRange)!=0)&&(zyStrlen(tmpStr) >= zyStrlen(prefixInfoObj->prefixRange))){
			if ( strncmp(tmpStr, prefixInfoObj->prefixRange, zyStrlen(prefixInfoObj->prefixRange) ) == 0 ) {
				ret=convertFacilityActiontoPhonefeature(prefixInfoObj->facilityAction);

				CCM_PRINTF("\n match , Phonefeature id is %d\n",ret);

/*Jason , syslog*/
#ifdef VOIP_SYSLOG
				#if 1 /*Eason, get registered line obj then enabled line obj*/
				if(NULL != pLine){
					voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,tmpPhoneCB->physicalId+1,prefixInfoObj->facilityAction);
				}
				#else
				voipSyslog(LOG_PHONE_EVENT,LOG_INFO,pLine->directoryNumber,tmpPhoneCB->physicalId+1,prefixInfoObj->facilityAction);
				#endif
#endif	
				featureLen=zyStrlen(prefixInfoObj->prefixRange);
				//return ret;		
				break;
			}
		}
	
		iter = iter->next_;
	}

	/*update callfeature id to retCode */
	if(ret!=-1){
		*retCode=ret;
	}
	
	/* modify the dial digits */
	/* for example *31*12345 , must remove *31* */

	switch(ret){
		case CALL_FEATURE_ONE_SHOT_CID_HIDDEN:
			ccmConn->dialInform.dialDigitNumber.dialDigitFlags|=DIAL_DIGITAL_ONE_SHOT_CID_HIDDEN;
			CCM_PRINTF("\n set dialDigitFlags is DIAL_DIGITAL_ONE_SHOT_CID_HIDDEN ");
			CCM_PRINTF("\n dialDigitFlags is %d ",ccmConn->dialInform.dialDigitNumber.dialDigitFlags);
		
			break;
		case CALL_FEATURE_ONE_SHOT_CID_DISPLAY:
			ccmConn->dialInform.dialDigitNumber.dialDigitFlags|=DIAL_DIGITAL_ONE_SHOT_CID_DISPLAY;
			CCM_PRINTF("\n test set dialDigitFlags is DIAL_DIGITAL_ONE_SHOT_CID_DISPLAY ");
			CCM_PRINTF("\n dialDigitFlags is %d ",ccmConn->dialInform.dialDigitNumber.dialDigitFlags);

			break;

		case CALL_FEATURE_ONE_SHOT_CALL_WAITING_ENABLE:
			ccmConn->dialInform.dialDigitNumber.dialDigitFlags|=DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_ENABLE;
			break;
		case CALL_FEATURE_ONE_SHOT_CALL_WAITING_DISABLE:
			ccmConn->dialInform.dialDigitNumber.dialDigitFlags|=DIAL_DIGITAL_ONE_SHOT_CALL_WAITING_DISABLE;		
			break;				
	#ifdef CALL_RETURN_SUPPORT
		case CALL_FEATURE_RETURN:
			phonePhyId=find_phonePhyId(ccmConn->origPortId);
			if(phonePhyId==0)
				break ;
			else
				phonePhyId=phonePhyId-1;
				zyMemset(&(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), 0, MAX_DIAL_STRING_LENGTH);
				zyMemcpy(&(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), callHistoryLastCall[phonePhyId].callHistoryLastCallDigit, callHistoryLastCall[phonePhyId].current_length);
				ccmConn->dialInform.dialDigitNumber.current_length=callHistoryLastCall[phonePhyId].current_length;
				remove_featurekey=0;
			break;
	#endif
#if 1	/* aircheng add for outgoing sip */
		case CALL_FEATURE_OUTGOING_SIP:
			*retCode = ret;
			remove_featurekey = 1;
			break;
#endif
#ifdef NORWAY_VOIP_CUSTOMIZATION
		case CALL_FEATURE_UNCONDITIONAL_FORWARD_ENABLE:
		case CALL_FEATURE_UNCONDITIONAL_FORWARD_DISABLE:
		case CALL_FEATURE_NOANSWER_FORWARD_ENABLE:
		case CALL_FEATURE_NOANSWER_FORWARD_DISABLE:
		case CALL_FEATURE_BUSY_FORWARD_ENABLE:
		case CALL_FEATURE_BUSY_FORWARD_DISABLE:
			remove_featurekey=0;
			break;
#endif
		default:
			ccmConn->callFeatures=ret;
			
			break;
	}


	if(remove_featurekey){
		if(*retCode != CALL_FEATURE_NUMBERS) {
			zyMemset( &(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), 0, MAX_DIAL_STRING_LENGTH);
			zyMemcpy( &(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), (tmpStr+featureLen), ccmConn->dialInform.dialDigitNumber.current_length-featureLen );
			ccmConn->dialInform.dialDigitNumber.current_length -= featureLen;
		}
	}

	zyMemset(tmpStr, 0, MAX_DIAL_STRING_LENGTH);
	zyMemcpy(tmpStr, &(ccmConn->dialInform.dialDigitNumber.dial_digits[0]), ccmConn->dialInform.dialDigitNumber.current_length);
	CCM_PRINTF("after checkPhoneFeature dial number :%s\r\n", tmpStr);
	zyFree(tmpStr);
	
	
}

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  getNumberingPlanPhoneFeatureIdBydialdigits
**
**  descriptions: get CCM phonefeature ID  by dialdigits 
**  parameters: profile id , dialDigits string
**  local:
**  global:
**  return: CCM Phonefeature ID 
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int
getNumberingPlanPhoneFeatureIdBydialdigits(int VpItr,char *dialDigits) {

	int ret = 0;
	int i=0;
	tr104ObjLst_t* prefixInfoTab= tr104VoiceProfNumPlanPrefixInfoTabPtrGet(VpItr);
	tr104LstElm_t* iter=(prefixInfoTab)?prefixInfoTab->head_:NULL;
	VoiceProfNumPlanPrefixInfoObj_t* prefixInfoObj=NULL;

	/*TBD: read VoiceService.{i}.Capabilities.NumberingPlan   (enable / disable numberingplan )*/
	

	
	if(prefixInfoTab==NULL){
		ZyPrintf("Table is NULL\n");
		return ret;
	}


	for(i=0;i<prefixInfoTab->size_;i++){	
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return ret;
		}
		prefixInfoObj=iter->value_;		
	//	ZyPrintf("\r\n ---- item %d --- ",i);
	//	ZyPrintf("\n  prefixRange = %s \n ", prefixInfoObj->prefixRange);
	//	ZyPrintf("\n  facilityAction = %s \n ", prefixInfoObj->facilityAction);
	//	ZyPrintf("\n prefixRange length = %d\n",zyStrlen(prefixInfoObj->prefixRange));
		
		if (zyStrlen(dialDigits) >= zyStrlen(prefixInfoObj->prefixRange)){
			if ( strncmp(dialDigits, prefixInfoObj->prefixRange, zyStrlen(prefixInfoObj->prefixRange) ) == 0 ) {

				ret=convertFacilityActiontoPhonefeature(prefixInfoObj->facilityAction);
			
				return ret;
		
				
			}
		}
	
		iter = iter->next_;
	}
	return 0;
	
}


/*______________________________________________________________________________
**  convertFacilityActiontoPhonefeature
**
**  descriptions: convert TR-104 FacilityAction to CCM Phonefeature ID ( define in call_mgnt.h) ex: CALL_FEATURE_INTERNAL_CALL
**  parameters: FacilityAction string
**  local:
**  global:
**  return: CCM Phonefeature ID 
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

int convertFacilityActiontoPhonefeature(char *facilityAction){

	int i=0;
	int ret = -1;

	for (i = 0; i < CALL_FEATURE_NUMBERS; i++){
#ifdef DBG_NUMBERLING_PLAN	
		PH_FSM_PRINTF("\n  item %d prefixRange = %s \n ", i,PhoneFeatureTable[i].tr104FacilityAction);
		PH_FSM_PRINTF("\n  phonefeatureid = %d \n ", PhoneFeatureTable[i].phonefeatureid);
#endif
		if(zyStrlen(PhoneFeatureTable[i].tr104FacilityAction)>0){
			if ( strncmp(facilityAction, PhoneFeatureTable[i].tr104FacilityAction, zyStrlen(PhoneFeatureTable[i].tr104FacilityAction) ) == 0 ) {
				ret = PhoneFeatureTable[i].phonefeatureid;
				return ret;
			}
		}
	}

	/* if no match , return -1 , this case is the FacilityAction not find in PhoneFeatureTable */ 
	return ret;

}

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  getCallFeaturekeyLengthByFeatureId
**
**  descriptions: get the PrefixInfoKey length by FeatureID
**  parameters: profile ID,  featureID ( define in call_mgnt.h) ex: CALL_FEATURE_INTERNAL_CALL
**  local:
**  global:
**  return: the TR-104  prefixRange (feature key) length
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/


int getCallFeaturekeyLengthByFeatureId(int VpItr, int callfeatureid) {

	char *tmpStr = NULL;
	int ret = 0;
	int i=0;

	tr104ObjLst_t* prefixInfoTab=NULL;
	tr104LstElm_t* iter=NULL;
	VoiceProfNumPlanPrefixInfoObj_t* prefixInfoObj=NULL;

	
	tmpStr = (char *)zyMalloc(64+1);
	zyMemset(tmpStr, 0, 64+1);
	

	prefixInfoTab= tr104VoiceProfNumPlanPrefixInfoTabPtrGet(VpItr);
	iter=(prefixInfoTab)?prefixInfoTab->head_:NULL;


	/* if no prefixinfo table, return */	
	if(prefixInfoTab==NULL){
		ZyPrintf("********** Prefix Table is NULL\n");
        zyFree(tmpStr);
		return ret;

	}
	//ZyPrintf("getCallFeaturekeyLengthByFeatureId  input is %d", callfeatureid);

	for (i = 0; i < CALL_FEATURE_NUMBERS; i++){
		if(PhoneFeatureTable[i].phonefeatureid==callfeatureid){
			zyMemcpy( tmpStr, PhoneFeatureTable[i].tr104FacilityAction, zyStrlen(PhoneFeatureTable[i].tr104FacilityAction) );
			break;						
		}
	}


	/*check prefixinfo table */
	for(i=0;i<prefixInfoTab->size_;i++){	
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
            zyFree(tmpStr);
			return -1;
		}
		prefixInfoObj=iter->value_;		
#ifdef DBG_NUMBERLING_PLAN			
		ZyPrintf("\r\n ---- item %d --- ",i);
		ZyPrintf("\n  prefixRange = %s \n ", prefixInfoObj->prefixRange);
		ZyPrintf("\n  facilityAction = %s \n ", prefixInfoObj->facilityAction);
		ZyPrintf("\n prefixRange length = %d\n",zyStrlen(prefixInfoObj->prefixRange));
#endif		
		if (zyStrlen(tmpStr) == zyStrlen(prefixInfoObj->facilityAction)){
			if ( strncmp(tmpStr, prefixInfoObj->facilityAction, zyStrlen(prefixInfoObj->facilityAction) ) == 0 ) {
				ret=zyStrlen(prefixInfoObj->prefixRange);
				break;			
			}
		}
	
		iter = iter->next_;
	}

	zyFree(tmpStr);


	return ret;
	
}

/*______________________________________________________________________________
**  checkDialDigitsNumberingPlan
**
**  descriptions: check the new digitail is in Numbering plan(phoneconfig) and the rule is match
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
int
checkDialDigitsNumberingPlan(
	int VpItr,
	char *dialDigits,
	uint8 *length
) {
	int ret = 0;
	int i=0;
	tr104ObjLst_t* prefixInfoTab= tr104VoiceProfNumPlanPrefixInfoTabPtrGet(VpItr);
	tr104LstElm_t* iter=(prefixInfoTab)?prefixInfoTab->head_:NULL;
	VoiceProfNumPlanPrefixInfoObj_t* prefixInfoObj=NULL;


/*TBD: read VoiceService.{i}.Capabilities.NumberingPlan   (enable / disable numberingplan )*/

	int	dialdigitslength=zyStrlen(dialDigits);
	if(prefixInfoTab==NULL){
		ZyPrintf("Table is NULL\n");
		return ret;
	}
	//ZyPrintf("\n  *********checkDialDigitsNumberingPlan************ \n ");
	//ZyPrintf("\n dialdigits length is %d ",zyStrlen(dialDigits));
	for(i=0;i<prefixInfoTab->size_;i++){	
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return ret;
		}
		prefixInfoObj=iter->value_;		
			
		if (zyStrlen(prefixInfoObj->prefixRange)!=0) {
			
			if(dialdigitslength< zyStrlen(prefixInfoObj->prefixRange)){
				if (strncmp(dialDigits, prefixInfoObj->prefixRange, dialdigitslength) == 0 ) {
					if(dialdigitslength >= prefixInfoObj->prefixMinNumberOfDigits){
						/* if total dial digits > prefixMinNumberOfDigits lengt, change interDigitsTimer */ 	
						ret = 2;
						return ret;
					}
				}
			}

			
			if (dialdigitslength >= zyStrlen(prefixInfoObj->prefixRange)){
				if ( strncmp(dialDigits, prefixInfoObj->prefixRange, zyStrlen(prefixInfoObj->prefixRange) ) == 0 ) {
					/* check the dial digits length  maximum
					TR-104:Once the number of digits received reaches this value an outgoing request should be initiated.

				 	case 1: full match , must  dial Immediately , 
				 	        ex: the prefix key is #43# , if user dial #43# and prefixMaxNumberOfDigits = 4 ,
			 		           we dial Immediately.
					*/			
					if(dialdigitslength >= prefixInfoObj->prefixMaxNumberOfDigits){
						/* if total dial digits >= prefixMaxNumberOfDigits lengt, dial Immediately */ 	
						ret = 1;
						return ret;

					}

				
					/* if the minimum number of digits has been reached, use InterDigitTimerOpen replace InterDigitTRimerStd */
					if(dialdigitslength >= prefixInfoObj->prefixMinNumberOfDigits){
	
						ret = 2;
						return ret;

					}


				
					/* TBD case 2: check last digit is # key .
					   prefix is *31*  , prefixMaxNumberOfDigits is 40 (its mean length not care)
						ex: *31*123456# 
						    *31#112233#
						     if last digits is '#' , must dial Immediately
					*/
	
				
				}
			}
	
			
		}
		iter = iter->next_;
	}


	return ret;
}

#if 1	/* aircheng add for outgoing sip */

int outgoingSipPhoneFeature(ccmConn_t *ccmConn)
{
	int ret = -1;
	dialDigitNum_t *dialDigit_p = NULL;
	char sipAccount[MAX_DIAL_STRING_LENGTH];
	char *digits = NULL;
	uint32 maxLineCount = tr104GetMaxLine();

	if(ccmConn == NULL) {
		CCM_PRINTF("%s: Error!! ccmConn is NULL\n", __FUNCTION__);
		goto exit;
	}

	ccmConn->outgoingSipAccountId = -1;
	dialDigit_p = &ccmConn->dialInform.dialDigitNumber;
	zyStrncpy(sipAccount, dialDigit_p->dial_digits, MAX_DIAL_STRING_LENGTH);

	digits = strchr(sipAccount, '#');
	if(digits == NULL) {
		CCM_PRINTF("%s: Error!! digits is NULL\n", __FUNCTION__);
		bzero(dialDigit_p->dial_digits, MAX_DIAL_STRING_LENGTH);
		dialDigit_p->current_length = 0;
		goto exit;
	}

	*digits = '\0';
	digits++;

	bzero(dialDigit_p->dial_digits, MAX_DIAL_STRING_LENGTH);
	strcpy(dialDigit_p->dial_digits, digits);
	dialDigit_p->current_length = strlen(dialDigit_p->dial_digits);

	ccmConn->outgoingSipAccountId = atoi(sipAccount);
	if((ccmConn->outgoingSipAccountId > maxLineCount) || (ccmConn->outgoingSipAccountId <= 0)) {
		ccmConn->outgoingSipAccountId = -1;
	} else {
		ccmConn->outgoingSipAccountId--;		/* ex. #1201#+number -> outgoingSipAccountId = 0 */
	}

#if 1
	CCM_PRINTF("%s: dialDigit_p->dial_digits = %s\n", __FUNCTION__, dialDigit_p->dial_digits);
	CCM_PRINTF("%s: ccmConn->outgoingSipAccountId = %d\n", __FUNCTION__, ccmConn->outgoingSipAccountId);
#endif

	ret = 0;

exit:
	return ret;
}

#endif

