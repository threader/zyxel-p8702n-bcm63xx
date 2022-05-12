/*
   $Id: $
   */
/************************************************************************
 *
 *  Copyright (C) 2007 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 *
 ************************************************************************/
/*
   $Log: $

*/

#include "global.h"
#include "switch.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "webDial_fsm.h"
#include "webDial_mtn.h"
#include "tr104.h"	
/*_____ initial  variables _______________________________________________*/
char   WebPhoneDialVision[10]= "1.0.0.0";

/*______________________________________________________________________*/
/*      Commands Declaration
 **______________________________________________________________________*/
#if 0
static const cmds_t WebDialCmds[] =
{
	{ "dial",				doWebPhoneDial,			CAT1,	3,	"<dialNumber> <WebDialPhoneType> <physicalID>" },
	{ "redial",			doWebPhoneReDial,			CAT1,	0,	NULL },
	{ "stopRedial",		doWebPhoneStopRedial,		CAT1,	0,	NULL },
	{ "status",			doWebPhoneDialStatus,		CAT1,	0,    NULL},
	{ "vision",			doWebPhoneDialVision,	CAT1,	0,    NULL},
	NULL,
};
#endif

/*______________________________________________________________________________
 **	doWebPhone
 **
 **	descriptions:
 **	parameters:
 **	local:
 **	global:
 **	return:
 **	called by:
 **	call:
 **	revision:
 **____________________________________________________________________________*/
int
doWebPhone(
		int argc,
		char *argv[],
		void *p
	  ){
	//return subcmd(WebDialCmds, argc, argv, p);
	return 0;
} /* doWebPhone */


int  doWebPhoneDial(int argc, char * argv [ ], void * p){

	//<dialNumber> <WebDialPhoneType> <physicalID>
	int WebDialPhoneType, physicalID;
	char *DialNumber_p;
	//phoneCh_t tmpWebDialPhoneCB;
	ccmConn_t *newCcmConn_p = NULL, *tmpWebDialCcm_p=NULL;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	DialNumber_p = argv[1];
	if (strlen(DialNumber_p) >= POTS_PORT_NAME_LEN) {
		printf("The length of phone number must be less than %d.\n\r", POTS_PORT_NAME_LEN);
		return 0;
	}
	WebDialPhoneType = atoi(argv[2]);
	physicalID = atoi(argv[3]);

	// Now just support FXS type//   /*0:FXS, 1:FXO ,ISDN ...etc*/
	if  ( WebDialPhoneType !=0)  {
		printf("wrong WebDialPhoneType[%d], Now we just support FXS type!!\n\r", WebDialPhoneType);
		return 0;
	}

	if (( physicalID < 0 ) || ( physicalID >= NumOfPhyIntf) ) {
		printf("wrong physicalID[%d], PH_FXS_NUM is=0~%d \n\r", physicalID ,NumOfPhyIntf-1);
		return 0;
	}

	//dbg_pline_1("\dialnumber :");
	//dbg_pline_1(DialNumber_p);
	//dbg_pline_1("\r\n");

	if (WebPhoneDialint(WebDialPhoneType, physicalID,DialNumber_p)==-1){
		printf("WebPhoneDial init fail!\n\r");
	}
	return 0;


}/* doWebPhoneDial */


int  doWebPhoneReDial(int argc, char * argv [ ], void * p){

	int RedialInretval, RedialTime, i, phyId;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();

	if(argc<=3){
		printf("Usage: <phone phyId> <dialInterval: 10~60 sec.> <redialTime: 1~10>\r\n");
		printf("\r\n");
		
		
		if(argc==1){
			for(i=0;i<NumOfPhyIntf;i++){
				printf("Phone %d\n", i);
				printf("Repeat Dial Flag is %d\r\n",webRepeatDial[i].webRepeatDialFlags);
				printf("Repeat Dial Interval is %d sec.\r\n",webRepeatDial[i].webRepeatDialInterval);
				printf("Repeat Dial Time is %d\r\n",webRepeatDial[i].webRepeatDialTimes);
				printf("Repeat Dial Current Time is %d\r\n",webRepeatDial[i].webRepeatDialCurrentTimes);
			}
		}
		return 0;
	}
	
	phyId = atoi(argv[1]);

	if(phyId > NumOfPhyIntf){
		printf("phone phyId must between 0 to %d\n", NumOfPhyIntf);
		return 0;
	}
	
	RedialInretval = atoi(argv[2]);
	if ( ( RedialInretval < RedialInretval_NUM_MIN ) || ( RedialInretval > RedialInretval_NUM_MAX  ) ) {
		printf("RedialInretval must between =10~60 sec\n\r");
		return 0;
	}

	RedialTime=atoi(argv[3]);
	if ( ( RedialTime < RedialTime_NUM_MIN ) || ( RedialTime > RedialTime_NUM_MAX ) ) {
		printf("RedialTime must between =1~10\n\r");
		return 0;
	}

	webRepeatDial[phyId].webRepeatDialFlags |= WEB_REPEAT_DIAL;
	webRepeatDial[phyId].webRepeatDialInterval=RedialInretval;
	webRepeatDial[phyId].webRepeatDialTimes=RedialTime;
	webRepeatDial[phyId].webRepeatDialCurrentTimes=0;
	printf("webRepeatDial[%d].webRepeatDialFlags=%d\n\r", phyId, webRepeatDial[phyId].webRepeatDialFlags); //Doris Debug
	printf("webRepeatDial[%d].webRepeatDialInterval=%d\n\r", phyId, webRepeatDial[phyId].webRepeatDialInterval); //Doris Debug
	printf("webRepeatDial[%d].webRepeatDialTimes=%d\n\r", phyId, webRepeatDial[phyId].webRepeatDialTimes); //Doris Debug
	printf("webRepeatDial[%d].webRepeatDialCurrentTimes=%d\n\r", phyId, webRepeatDial[phyId].webRepeatDialCurrentTimes); //Doris Debug

	return 0;
}/* doWebPhoneReDial */

int  doWebPhoneStopRedial(int argc, char * argv [ ], void * p){
	/* Reset the webRepeatDial initial value */
	int i;
	int NumOfPhyIntf = 0;
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	if( argc !=1){
		printf("error parameters\n\r");
	}
	else{
		printf("Stop repeat dial!\n\r");
		//printf("Doris-> webRepeatDial.dialDigitNumber1=%s\n\r", webRepeatDial.webRepeatDialDigitNumber.dial_digits); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialFlags1=%d\n\r", webRepeatDial.webRepeatDialFlags); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialInterval1=%d\n\r", webRepeatDial.webRepeatDialInterval); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialTimes1=%d\n\r", webRepeatDial.webRepeatDialTimes); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialCurrentTimes1=%d\n\r", webRepeatDial.webRepeatDialCurrentTimes); //Doris Debug
		for(i=0;i<NumOfPhyIntf;i++){
			webRepeatDial[i].webRepeatDialFlags &= ~WEB_REPEAT_DIAL;
			webRepeatDial[i].webRepeatDialInterval=0;
			webRepeatDial[i].webRepeatDialPhonePhyID=0;
			webRepeatDial[i].webRepeatDialPhoneType=0;
			webRepeatDial[i].webRepeatDialTimes=0;
			webRepeatDial[i].webRepeatDialCurrentTimes=0;
			memset(&(webRepeatDial[i].webRepeatDialDigitNumber), 0, sizeof(dialDigitNum_t));
		}
		//printf("Doris-> webRepeatDial.dialDigitNumber=%s\n\r", webRepeatDial.webRepeatDialDigitNumber.dial_digits); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialFlags=%d\n\r", webRepeatDial.webRepeatDialFlags); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialInterval=%d\n\r", webRepeatDial.webRepeatDialInterval); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialTimes=%d\n\r", webRepeatDial.webRepeatDialTimes); //Doris Debug
		//printf("Doris-> webRepeatDial.webRepeatDialCurrentTimes=%d\n\r", webRepeatDial.webRepeatDialCurrentTimes); //Doris Debug
	}
	return 0;
}/* doWebPhoneStopRedial */

int  doWebPhoneDialStatus(int argc, char * argv [ ], void * p){

	int enableStatus=0;
	char tmpsbuffer[1024];
	va_list argptr;
	int len_insert=0, len_current=0, len_sum=0;

	if(( argc > 2 ) ||( argc < 2 )){
		printf("Web phone Status = %d\n\r", WebphoneStatus);
		return 0;
	}

	enableStatus=atoi(argv[1]);

	if (enableStatus < 0 || enableStatus>1) {
		printf("Usage:  <disable:0 enable:1>\r\n");
		return 0;
	}

	WebphoneStatus = enableStatus;
	printf("Web phone Status = %d\n\r", WebphoneStatus);

	if (WebphoneStatus==WEBDIAL_STATE_START){
		/*alloc Dial States buffer-rpWebDialStatusBuf_p*/
		if (rpWebDialStatusBuf_p != NULL){
			free(rpWebDialStatusBuf_p);
			rpWebDialStatusBuf_p = NULL;
		}

		if (rpWebDialStatusBuf_p == NULL){
			if ( ( rpWebDialStatusBuf_p = (char *)malloc(1024*sizeof(char))) == NULL ) {
				printf("alloc rpWebDialStatusBuf_p error\n\r");
				return -1;
			}
		}
		memset(rpWebDialStatusBuf_p, 0, sizeof(rpWebDialStatusBuf_p));

	}
	return 0;
}/* doWebPhoneDialStatus */


int  doWebPhoneDialVision(int argc, char * argv [ ], void * p){

	printf("Web Phone Dial Vision: %s\n", WebPhoneDialVision);
	return 0;

}/* doWebPhoneDialVision */




