/*
  $Id: DialPlan.h 1.2.1.1 2007/07/26 02:37:16 Allen.Yang Exp $
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
  $Log: DialPlan.h $
*/

#ifndef _NUMBERINGPLAN_H_INCLUDE
#define _NUMBERINGPLAN_H_INCLUDE

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Extern Variable Declaration
%%____________________________________________________________________________*/



/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      External Function Declaration
%%____________________________________________________________________________*/

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Variable Declaration
%%____________________________________________________________________________*/

typedef struct PhoneFeaturePrefixInfo_s
{
   unsigned int   phonefeatureid;	/* callfeature id */
   char           tr104FacilityAction[64];	/* TR104,Facility Actions */
#if 1 /*Jason , syslog*/
   char           featurename[64];	/* Feature Name, for syslog */
#endif
} PhoneFeaturePrefixInfo_t;


extern PhoneFeaturePrefixInfo_t PhoneFeatureTable[CALL_FEATURE_NUMBERS];

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Function Declaration
%%____________________________________________________________________________*/



void checkNumberingPlanPhoneFeature(ccmConn_t *ccmConn, int *retCode, int *exitFlag);
int convertFacilityActiontoPhonefeature(char *facilityAction);
int getCallFeaturekeyLengthByFeatureId(int profileId, int callfeatureid);
int getNumberingPlanPhoneFeatureIdBydialdigits(int vpIndex,char *dialDigits);
int checkDialDigitsNumberingPlan(int vpIndex,char *dialDigits,uint8 *length);
#if 1	/* aircheng add for outgoing sip */
int outgoingSipPhoneFeature(ccmConn_t *ccmConn);
#endif

#endif

