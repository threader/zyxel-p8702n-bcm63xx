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

#ifndef _DIALPLAN_H_INCLUDE
#define _DIALPLAN_H_INCLUDE

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Extern Variable Declaration
%%____________________________________________________________________________*/

extern bool DialPlanDebug;
extern bool instantDial;
/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      External Function Declaration
%%____________________________________________________________________________*/

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Variable Declaration
%%____________________________________________________________________________*/


typedef struct digit_replace_s {  /* struct for <dialed-subsequence:transmitted-subsequence> syntax */
	int origin_digit_number;
	char *digit_transmitted;
}digit_replace_t;

typedef struct dial_plan_rule_s { /* struct for each rule of dial plan*/
	bool blocked;
	bool instant;
	int digit_number;
	int repeat;
	int gateway;
	int prefix;
	digit_replace_t replace;
	int *digit_table;
	struct dial_plan_rule_s *next;
}dial_plan_rule_t;

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Function Declaration
%%____________________________________________________________________________*/


int transferByDialplan(int VpItr , uint8 *input, uint8 *length, int *gateway);
bool instantDialOutCheck(int VpItr , uint8 *input, uint8 *length);
int DialOutPartialCheck(int VpItr , uint8   *input, uint8   *length);
int instantDialOutPartialCheck(int VpItr , uint8   *input, uint8   *length);
int noninstantDialOutPartialCheck(int VpItr , uint8   *input, uint8   *length);

int initDialPlan(void);
bool checkDialPlanActive(int VpItr);
int dialplanReload(void);

static int doDialPlanLoad(void);
static int doDialPlanShow(int VpItr);
int doDialplanClear(void);
static dial_plan_rule_t* dialplanParser(char *);
static dial_plan_rule_t* ruleParser(char *);
static void trimSpace(char *);
static void trimValidDigits(char *);
static int  freeDialPlan(struct dial_plan_rule_s **);

int checkDialPlan(int VpItr , dialDigitNum_t *dialDigitNum_p);


#endif

