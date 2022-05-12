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

#ifndef _WEB_DIAL_FSM_H
#define _WEB_DIAL_FSM_H

/*____________________________________________________________________________*/
/*	macro                                                                                                                         */
/*____________________________________________________________________________*/

/* The definition for the webdial_p->webdialStatus*/
#define WEB_DIAL_ST_IDLE						0
#define WEB_DIAL_ST_1stZYPHONE_CREATE		1
#define WEB_DIAL_ST_2ndZYPHONE_CREATE		2
#define WEB_DIAL_ST_ZYPHONE_REMOVE			3

/* The definition for the webRepeatDial.webRepeatDialFlags */
#define WEB_REPEAT_DIAL						(1<<0)     

#define RedialInretval_NUM_MAX	60
#define RedialInretval_NUM_MIN	10
#define RedialTime_NUM_MAX	10
#define RedialTime_NUM_MIN	1

/*____________________________________________________________________________*/
/*	Data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*copy WebphoneStatus*/
#define WEBDIAL_STATE_IDLE					0
#define WEBDIAL_STATE_START				1

/*copy WebphoneGUIStatus*/
#define GUI_WEBDIAL_STATE_IDLE				0
#define GUI_WEBDIAL_STATE_START				1
#define GUI_WEBDIAL_STATE_END					2
#define GUI_WEBDIAL_STATE_REDAILSTART		3

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/

typedef struct webdial_s
{
	uint8		webdialStatus;
	uint8		webdialType;				/* 0: RX(First ZyXELPhone make outgoing call),  1: TX(Second ZyXELPhone make local phone ring)*/ 
	uint16		PhonePhyID;
  	ccmConn_t*	webdialPeerZyPhoneCCM;		/*Peer ZyPhone CCM*/		 
}webdial_t;

typedef struct webRepeatDial_s
{
	uint8			webRepeatDialFlags;                        	 
	uint8			webRepeatDialInterval;
	uint8			webRepeatDialTimes; 			/* The repeat dial times*/
	uint8 			webRepeatDialCurrentTimes;		/* The repeat dial current times*/
	dialDigitNum_t 	webRepeatDialDigitNumber;
	uint8			webRepeatDialPhoneType;					/* 0:FXS, 1:FXO ,ISDN ...etc*/
	uint16			webRepeatDialPhonePhyID;
	uint8 			resverse[1];
}webRepeatDial_t;


/*____________________________________________________________________________*/
/*	Data declaration                                                                                                          */
/*____________________________________________________________________________*/

extern int WebphoneStatus;
extern int WebphoneGUIStatus;

/*_____ global variables _______________________________________________*/
extern webRepeatDial_t *webRepeatDial;
extern char *rpWebDialStatusBuf_p;
extern int FXO_Busy_Here;

/*____________________________________________________________________________*/
/*	function definition                                                                                                        */
/*____________________________________________________________________________*/
void copyWebDialLog(char *fmt, ...);
void setWebDialStates_Start(void);
void setWebDialStates_End(void);
int getWebDialStatus(ccmConn_t *ccmConn);
int WebDialAssociate(ccmConn_t* ccmConn_p);
int createWebDialCB(ccmConn_t *ccmConn_p, int physicalID);
void removeWebDialCB(webdial_t *);
void WebDialFsmProc(ccmConn_t *, uint16, void*, uint16);
void WebDialFsm1stZyPhoneCreate(ccmConn_t *, uint16, void*, uint16);
void WebDialFsm2ndZyPhoneCreate(ccmConn_t *, uint16, void*, uint16);
void WebDialFsmZyPhoneRemove(ccmConn_t *, uint16, void*, uint16);
int WebRedialTimeout (tm_id_t timer_id, void *arg);
int WebDialTonedetetTimeout( tm_id_t tmId, void *param);
uint8 getWebRedialInterval(uint16 phyId);
uint8 getCallMgntWebDailFlag();
int WebDialAssociate(ccmConn_t* ccmConn_p);
int  WebPhoneDialint(int, int , char *);
void  WebPhoneDialStop(int physicalID);
int webPhoneDialStart(int WebDialPhoneType, int physicalID, char *DialNumber_p);


/*return reasone phase*/
const char* getFXOStatusCodeToString(int FXOStatusCode);

#endif
