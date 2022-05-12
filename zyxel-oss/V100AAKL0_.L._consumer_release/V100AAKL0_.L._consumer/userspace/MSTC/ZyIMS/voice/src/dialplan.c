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
#include "tr104_object.h"
#include "tr104.h"
#include "call_mgnt.h"
#include "dialplan.h"

#ifdef DIAL_PLAN

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Variable Declaration
%%____________________________________________________________________________*/

static char dial_plan[256];/* store area for user defined dial plan */

//Ssu-Ying, support Multiple Profile
static bool *activeDialPlan;
bool instantDial;
bool DialPlanDebug;

static dial_plan_rule_t **rule_head; /* head rule of dial plan link list */

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**	dodialPlanLoad
**	descriptions: load dialPlan from " tr104 : VoiceCommonObj " and set dial plan rules (must be enclosed in "()")
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int 
doDialPlanLoad(void)
{
	int ret=0;
	uint32 NumOfProfile = 0;
	int VpItr=0;
	VoiceProfObj_t*	voiceProf_p = NULL;	
	NumOfProfile = tr104GetNumProfile();
	for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
		voiceProf_p = (VoiceProfObj_t*) tr104VoiceProfObjPtrGet(VpItr);	
		if(voiceProf_p == NULL){
			printf("error : voiceProf_p is NULL \n");
			return -1;
		}

		activeDialPlan[VpItr] = voiceProf_p->digitMapEnable;	
		strncpy(dial_plan, voiceProf_p->digitMap,256);


		if(zyStrlen(dial_plan) != 0) {/*if zero ,  no any dial plan set in flash */
			
			if(DialPlanDebug) {
				printf("\n dial_plan string = %s \n",dial_plan);
			}

			if( rule_head[VpItr] != NULL ) {
				freeDialPlan(&rule_head[VpItr]);
			}
			
			rule_head[VpItr] = dialplanParser(dial_plan);

			if(rule_head[VpItr] != NULL){
				if(DialPlanDebug) {
					doDialPlanShow(VpItr);
				}
			} else {
				if(DialPlanDebug) {
					printf("Parser error : rule_head is NULL , VpItr = %d\n" , VpItr);
				}
				ret= -1;
			}
			
		}else{
			if(DialPlanDebug) {
				printf("dial_plan string is empty :do nothing \n");
			}
		}		
	}
	return ret ;
}
/*______________________________________________________________________________
**	doDialplanReload
**
**	descriptions:clear the dial_plan and rule_head
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int 
dialplanReload(void)
{

	doDialplanClear();
	doDialPlanLoad();

	return 0;
} /* doDialplanReload */	
//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**	doDialplanShow
**
**	descriptions:show dial plan in detail
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int 
doDialPlanShow(int VpItr)
{
	dial_plan_rule_t *pRule = NULL;
	int index, rule_counter;

	pRule = rule_head[VpItr];
	if( pRule == NULL ) {
		printf("no dial plan had been set.\n\r");
		return 0;
	}

	rule_counter = 0;
	printf("Dial Plan :%s\n\r", dial_plan);
	printf("--------------------------------------------------------------------------------\n\r");
	printf("VpItr:%02i\n\r", VpItr);
	while( pRule != NULL ) {
		rule_counter++;
		printf("                        rule:%02i\n\r", rule_counter);
		printf("                     blocked:%s\n\r", pRule->blocked == TRUE ? "True" : "False");
		printf("                     instant:%s\n\r", pRule->instant == TRUE ? "True" : "False");
		printf("          total valid digits:%i\n\r", pRule->digit_number);
		printf("                      repeat:0x%x\n\r", pRule->repeat);
		printf("                     gateway:%i\n\r", pRule->gateway);
		printf("prefix dailing digits number:%i\n\r", pRule->replace.origin_digit_number);
		printf("   prefix transmitted digits:%s\n\r", pRule->replace.digit_transmitted);
		printf("         digit parsing table:");
		for( index = 0; index < pRule->digit_number; index++ ) {
			printf("0x%x ", pRule->digit_table[index]);
		}
		printf("\n\r");
		printf("--------------------------------------------------------------------------------\n\r");
		pRule = pRule->next;
	}

	return 0;
} /* doDialplanShow */
/*______________________________________________________________________________
**	doDialplanClear
**
**	descriptions:clear the dial_plan and rule_head
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int 
doDialplanClear(void)
{
//Ssu-Ying, support Multiple Profile
	uint32 NumOfProfile = 0;
	int VpItr=0;
	NumOfProfile = tr104GetNumProfile();
	for(VpItr=0;VpItr<NumOfProfile;VpItr++) {
		if( rule_head[VpItr] != NULL )
			freeDialPlan(&rule_head[VpItr]);
		rule_head[VpItr]=NULL;
	}
	dial_plan[0] = '\0'; /* reset */

	return 0;
} /* doDialplanClear */	

/*______________________________________________________________________________
**	dialplanParser
**
**	descriptions:parse dial plan rules to defined data structure
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static dial_plan_rule_t* 
dialplanParser(char *rules)
{
	dial_plan_rule_t *pRule = NULL, *pFirstRule = NULL;
	int rule_counter = 0;
	char *head, *tail, *index, *workBuffer = NULL;

	if(zyStrlen(rules) == 0) {
		goto end; /* Dial Plan is NULL */
	}

	workBuffer = (char *)zyMalloc(zyStrlen(rules) + 1);
	if( workBuffer == NULL ) {
		printf("no enough memory for Dial Plan!\n\r");
		pFirstRule = NULL;
		goto end;
	} else {
		zyStrcpy(workBuffer, rules);
	}
	trimSpace(workBuffer);/* trim all space in dial plan */

	if( (head = strchr(workBuffer, '(')) == NULL || (tail = strchr(workBuffer, ')')) == NULL ) {
		printf("Can't find complete parentheses in dial plan.\n\r");
		printf("The dial plan syntax is invalid!\n\r");
		pFirstRule = NULL;
		goto end;
	} else {
		head++; /* shift head to first rule start */
		pFirstRule = pRule = NULL;
		for( ;; ) {
			if( (index = strchr(head, '|')) == NULL ) { /* last rule*/
				*tail = '\0'; /* replace ')' with '\0' */                
				if( zyStrlen(head) != 0 ) {/* Not empty rule */
					rule_counter++;
#ifdef DBG_DIAL_PLAN
					printf("rule %02i:%s\n\r", rule_counter, head);
#endif
					if( pFirstRule == NULL ) {/* first rule */
						pRule = ruleParser(head);
						if( pRule != NULL ) {
							pFirstRule = pRule;
							pFirstRule->next = NULL;
						}
					} else {
						pRule->next = ruleParser(head);
						if( pRule->next != NULL ) {
							pRule = pRule->next;
							pRule->next = NULL;
						}
					}
				}
				break; /* jump out the loop */
			} else {
				*index = '\0';/* replace '|' with '\0' */
				index++; /* index points to next rule start */
				if( zyStrlen(head) != 0 ) {/* Not empty rule */
					rule_counter++;
#ifdef DBG_DIAL_PLAN
					printf("rule %02i:%s\n\r", rule_counter, head);
#endif
					if( pFirstRule == NULL ) {/* first rule */
						pRule = ruleParser(head);
						if( pRule != NULL ) {
							pFirstRule = pRule;
							pFirstRule->next = NULL;
						}
					} else {
						pRule->next = ruleParser(head);
						if( pRule->next != NULL ) {
							pRule = pRule->next;
							pRule->next = NULL;
						}
					}

				}
			}
			head = index; /* head points to next rule start */
		}
	}

end:
	if( workBuffer != NULL ) {
		zyFree(workBuffer);
		workBuffer=NULL;
	}
	return pFirstRule;
} /* dialplanParser */

/*______________________________________________________________________________
**	ruleParser
**
**	descriptions:parse each rule in dial plan
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static dial_plan_rule_t*
ruleParser(char *rule)
{
	dial_plan_rule_t* pRule = NULL;
	int digit_counter, *pDigitTable, i, temp_counter;
	char *head, *tail, *index, repeatDigit, *index1;

	pRule = (dial_plan_rule_t* )zyMalloc(sizeof(dial_plan_rule_t));
	if( pRule == NULL ) {
		printf("No enough memory for Dial Plan!\n\r");
		goto failed;
	}
	zyMemset(pRule, 0, sizeof(dial_plan_rule_t));

	digit_counter = 0;
	temp_counter = 0;
	index = rule;/* index points to rule start */

	if( head = strchr(index, '@') ) {/* there is instant transformation syntax in this rule */
		*head = '\0';
		pRule->instant = 1;
	}

	if( head = strchr(index, '=') ) {/* there is gateway syntax in this rule */
		index = head + 1;
		if( (tail = strchr(index, '=')) == NULL ) {
			printf("There is invalid syntax in this rule. Bad Dial Plan!\n\r");
			goto failed;
		} else {
			*tail = '\0';

			/* need to modify in the future */
			if(zyStrcmp(index, "gw0") == 0){/*DSP_FXS_PORT*/
				pRule->gateway = 0;
			}else if(zyStrcmp(index, "gw1") == 0){/*DSP_FXO_PORT*/
				pRule->gateway = 1;
			}else if(zyStrcmp(index, "gw2") == 0){/*DSP_FXO_LIFELINE*/
				pRule->gateway = 2;
			}else if(zyStrcmp(index, "gw3") == 0){/*SIP_PORT*/
				pRule->gateway = 3;
			}else if(zyStrcmp(index, "gw4") == 0){/*ZYXEL_PHONE_PORT*/
				pRule->gateway = 4;
			}else if(zyStrcmp(index, "gw5") == 0){/*ISDN_NT_PORT*/
				pRule->gateway = 5;
			}else if(zyStrcmp(index, "gw6") == 0){/*ISDN_TE_PORT*/
				pRule->gateway = 6;
			}else if(zyStrcmp(index, "gw7") == 0){/*DSP_DECT_PORT*/
				pRule->gateway = 7;
			}else{
				pRule->gateway = 3; /* gw3, default VoIP gateway */
			}

			*head = '\0';
			zyStrcpy(head, (char *)(tail + 1));/* cover '=gwx=' with following digits */
		}
	}else{
		/*if no gateway syntax in this rule , we set the gateway to default : SIP_PORT.*/
		pRule->gateway = 3;  /* gw3, default VoIP gateway */
	}

	index = rule;/* index reset */

	if( head = strchr(index, '.') ) {/* there is repeat syntax in this rule */
		repeatDigit = *(head - 1); /* repeat digit is the digit before '.' */
		*(head - 1) = '\0';
		trimValidDigits((char *)(head + 1));/* ignore all valid digits after '.' */
		zyStrcat(index, (char *)(head + 1));

		if( repeatDigit >= '0' && repeatDigit <= '9' ) {
			pRule->repeat = (int)(1 << (repeatDigit - '0'));
		} else if( repeatDigit == 'x' || repeatDigit == 'X' ) {
			pRule->repeat = 0xFFF; /* 0000111111111111 */
		} else if( repeatDigit == '*' ) {
			pRule->repeat = 0x400; /* 0000010000000000 */
		} else if( repeatDigit == '#' ) {
			pRule->repeat = 0x800; /* 0000100000000000 */
		}
	}

	index = rule;/* index reset */

	if( head = strchr(index, '<') ) {/* there is replace syntax in this rule */
		head ++;
		if( (tail = strchr(head, '>')) == NULL ) {
			printf("There is invalid syntax in this rule. Bad Dial Plan!\n\r");
			goto failed;
		} else {
			if( (index = strchr(head, ':')) == NULL ) {
				printf("There is invalid syntax in this rule. Bad Dial Plan!\n\r");
				goto failed;
			} else {
				*index = '\0';
				index++;
				pRule->replace.origin_digit_number = zyStrlen(head);
				digit_counter += pRule->replace.origin_digit_number;
				*tail = '\0';
				pRule->replace.digit_transmitted = (char *)zyMalloc(zyStrlen(index) + 1);
				if( pRule->replace.digit_transmitted == NULL ) {
					printf("No enough memory for Dial Plan!\n\r");
					goto failed;
				} else {
					zyStrcpy(pRule->replace.digit_transmitted, index);
				}
				index = (char *)(tail + 1);
			}
		}
	}

	index1 = rule;
	if( (head = strchr(index1, '<')) != NULL ){
		while( *index1 != '<' ) {
			if( (*index1 >= '0' && *index1 <= '9') || (*index1 == 'x') ||
				(*index1 == 'X') ||(*index1 == '*') || (*index1 == '#') ) {
				digit_counter++;
				temp_counter ++;
			} else if( *index1 == '[' ) {
				if( (index = strchr(index1, ']')) == NULL ) {
					printf("There is invalid syntax in this rule. Bad Dial Plan!");
					goto failed;
				}
				digit_counter++;
				temp_counter ++;
			} else if( *index1 == '!' ) {/* blocked syntax */
				pRule->blocked = TRUE;
			}
			index1++;
		}
		pRule ->prefix = temp_counter;
	}

	while( *index != '\0' ) {
		if( (*index >= '0' && *index <= '9') || (*index == 'x') ||
			(*index == 'X') ||(*index == '*') ||(*index == '#') ) {
			digit_counter++;
		} else if( *index == '[' ) {
			if( (index = strchr(index, ']')) == NULL ) {
				printf("There is invalid syntax in this rule. Bad Dial Plan!");
				goto failed;
			}
			digit_counter++;
		} else if( *index == '!' ) {/* blocked syntax */
			pRule->blocked = TRUE;
		}
		index++;
	}

#ifdef DBG_DIAL_PLAN
	printf("total valid digits of the rule:%i\n\r", digit_counter);
#endif
	pRule->digit_number = digit_counter;
	if(pRule->digit_number != 0){
		pRule->digit_table = (int *)zyMalloc(digit_counter * sizeof(int));
		if(pRule->digit_table == NULL){
			printf("No enough memory for Dial Plan!\n\r");
			goto failed;
		}
	}else{
		pRule->digit_table = NULL;
		return pRule; /* NULL digit_table, skip following steps */
	}
	pDigitTable = pRule->digit_table;
	index = rule; /* reset */
	index1 = rule;
	if(( head = strchr(index1, '<')) != NULL ){
		while( *index1 != '<' ) {/* ':' is already replaced by '\0' */
			if( *index1 >= '0' && *index1 <= '9' ) {
				*pDigitTable = (int)(1 << (*index1 - '0'));
				pDigitTable++;
			} else if( *index1 == 'x' || *index1 == 'X' ) {
				*pDigitTable = 0xFFF; /* 0000111111111111 */
				pDigitTable++;
			} else if( *index1 == '*' ) {
				*pDigitTable = 0x400; /* 0000010000000000 */
				pDigitTable++;
			} else if( *index1 == '#' ) {
				*pDigitTable = 0x800; /* 0000100000000000 */
				pDigitTable++;
			}
			index1++;
		}
	}
	if( head = strchr(index, '<') ) {
		index = head + 1;
		while( *index != '\0' ) {/* ':' is already replaced by '\0' */
			if( *index >= '0' && *index <= '9' ) {
				*pDigitTable = (int)(1 << (*index - '0'));
				pDigitTable++;
			} else if( *index == 'x' || *index == 'X' ) {
				*pDigitTable = 0xFFF; /* 0000111111111111 */
				pDigitTable++;
			} else if( *index == '*' ) {
				*pDigitTable = 0x400; /* 0000010000000000 */
				pDigitTable++;
			} else if( *index == '#' ) {
				*pDigitTable = 0x800; /* 0000100000000000 */
				pDigitTable++;
			}
			index++;
		}
		index++;
		index += zyStrlen(index) + 1; /* move index after '>' */
	}

	while( *index != '\0' ) {
		if( *index >= '0' && *index <= '9' ) {
			*pDigitTable = (1 << (*index - '0'));
			pDigitTable++;
		} else if( *index == 'x' || *index == 'X' ) {
			*pDigitTable = 0xFFF; /* 0000111111111111 */
			pDigitTable++;
		} else if( *index == '*' ) {
			*pDigitTable = 0x400; /* 0000010000000000 */
			pDigitTable++;
		} else if( *index == '#' ) {
			*pDigitTable = 0x800; /* 0000100000000000 */
			pDigitTable++;
		} else if( *index == '[' ) {
			index++;
			*pDigitTable = 0; /* reset value to 0 */
			while( *index != ']' ) {
				if( *index >= '0' && *index <= '9' ) {
					*pDigitTable |= (1 << (*index - '0'));
				} else if( *index == '*' ) {
					*pDigitTable |= 0x400; /* 0000010000000000 */
				} else if( *index == '#' ) {
					*pDigitTable |= 0x800; /* 0000100000000000 */
				} else if( *index == '-' ) {
					for( i = *(index - 1);i <= *(index + 1); i++ ) {
						*pDigitTable |= (1 << (i - '0'));
					}
				}
				index++;
			}
			pDigitTable++;
		}
		index++;
	}

	return pRule;

failed:
	if( pRule != NULL ) {
		if( pRule->replace.digit_transmitted != NULL ) {
			zyFree(pRule->replace.digit_transmitted);
			pRule->replace.digit_transmitted=NULL;
		}
		if( pRule->digit_table != NULL ) {
			zyFree(pRule->digit_table);
			pRule->digit_table = NULL;
		}
		zyFree(pRule);
		pRule = NULL;
	}
	return NULL;
} /* ruleParser */
/*______________________________________________________________________________
**	trimSpace
**
**	descriptions:trim all space in the string
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static void
trimSpace(char *stringInput)
{
	char *strPtr1, *strPtr2;
	int index, strLength = 0;

	strLength = zyStrlen(stringInput);
	if( strLength == 0 ) {
		return;   /* NULL string */
	}

	strPtr1 = strPtr2 = stringInput;

	for( index = 0; index < strLength; index++ ) {
		if( *strPtr1 != ' ' ) {
			*strPtr2 = *strPtr1;
			strPtr1++;
			strPtr2++;
		} else {
			strPtr1++;
		}
	}
	*strPtr2 = '\0';

	return;
} /* trimSpace */

/*______________________________________________________________________________
**	trimValidDigits
**
**	descriptions:trim all valid dailing digits in the string
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static void
trimValidDigits(char *stringInput)
{
	char *strPtr1, *strPtr2;
	int index, strLength = 0;

	strLength = zyStrlen(stringInput);
	if( strLength == 0 ) {
		return;   /* NULL string */
	}

	strPtr1 = strPtr2 = stringInput;

	for( index = 0; index < strLength; index++ ) {
		if( *strPtr1 >= '0' && *strPtr1 <= '9' ||*strPtr1 == 'x' || *strPtr1 == 'X' || *strPtr1 == '*' || *strPtr1 == '#' ) {
			strPtr1++;
		} else {
			*strPtr2 = *strPtr1;
			strPtr1++;
			strPtr2++;
		}
	}
	*strPtr2 = '\0';

	return;
} /* trimSpace */

/*______________________________________________________________________________
**	freeDialPlan
**
**	descriptions:free memory of dial plan structure
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static int
freeDialPlan(dial_plan_rule_t** head)
{
	dial_plan_rule_t *ptrRule, *nextRule;

	ptrRule = *head;
	while( ptrRule != NULL ) {
		nextRule = ptrRule->next;
		if( ptrRule->replace.digit_transmitted != NULL )
			zyFree(ptrRule->replace.digit_transmitted);
		if( ptrRule->digit_table != NULL )
			zyFree(ptrRule->digit_table);
		zyFree(ptrRule);
		ptrRule = nextRule;
	}
	*head = NULL;
	return 0;
} /* freeDialPlan */

/*______________________________________________________________________________
**	transferByDialplan
**
**	descriptions:parse dialing digits with dial plan
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
transferByDialplan(int VpItr , uint8   *input, uint8   *length, int     *gateway)
{
	dial_plan_rule_t *pRule;
	uint8 *pDigit, input_len, *output = NULL;
	int index, dailing_key, total_digits, strLen, rtn;
	bool repeatSyntax;

	input_len = *length;/* record original input length for later use */
	pRule = rule_head[VpItr];
	if( pRule == NULL ) { /* no dial plan set */
		/* do nothing */
		rtn=-2;
		goto end;
	} else {
		total_digits = *length;
		while( pRule != NULL ) {
			pDigit = input; /* reset pDigit */
			if(instantDial == TRUE){/* if coming dailing number is the instant number */
				while( pRule != NULL ){/* fast searching */
					if(pRule->instant == TRUE) {
						break; /* got it */
					}
					pRule = pRule->next;
				}
				if(pRule == NULL){ /* prevent abnormal case from cashing the device */
					rtn = -1;
					goto end;
				}
			}
			if( total_digits >= pRule->digit_number ) {/* total dialing digits must be equal to or larger than total digits in the rule */
				/* ready to compare digits between dailing and rule */
				for( index = 0; index <= pRule->digit_number; index++ ) {
					if( *pDigit >= '0' && *pDigit <= '9' ) {
						dailing_key = (int)(1 << (*pDigit - '0'));
					} else if( *pDigit == '*' ) {
						dailing_key = 0x400; /* 0000010000000000 */
					} else if( *pDigit == '#' ) {
						dailing_key = 0x800; /* 0000100000000000 */
					} else {
						dailing_key = 0; /* other digits, never come here normally */
					}
					if(pRule->digit_number != 0){/* not case 'X.' */
						if( dailing_key & pRule->digit_table[index] ) {
							pDigit++;
						} else {
							break;/* try next rule */
						}
					}
					if(index == pRule->digit_number - 1 /* match the last rule digit */
						|| (pRule->repeat != 0 && pRule->digit_number == 0)) { /* case 'X.' */
						if(total_digits - pRule->digit_number > 0){/* check '.' syntax in the rule */
							repeatSyntax = FALSE;
							if(pRule->repeat != 0){
								for(index = pRule->digit_number; index < total_digits; index++){
									if( *pDigit >= '0' && *pDigit <= '9' ) {
										dailing_key = (int)(1 << (*pDigit - '0'));
									} else if( *pDigit == '*' ) {
										dailing_key = 0x400; /* 0000010000000000 */
									} else if( *pDigit == '#' ) {
										dailing_key = 0x800; /* 0000100000000000 */
									} else {
										dailing_key = 0; /* other digits, never come here normally */
									}
									if( dailing_key & pRule->repeat ){
										pDigit++;
									}else{
										break;
									}

									if(index == total_digits - 1){
										repeatSyntax = TRUE;
									}
								}
							}
							if(repeatSyntax == FALSE){
								break; /* try next rule */
							}
						}

						if( pRule->replace.digit_transmitted != NULL ) {
							strLen = zyStrlen(pRule->replace.digit_transmitted);
						} else {
							strLen = 0;
						}

						*length = total_digits + strLen - pRule->replace.origin_digit_number;
						output = (uint8 *)zyMalloc(*length);
						zyMemset(output, 0, sizeof(output));

						if( strLen != 0 && pRule ->prefix == 0){
							zyMemcpy(output, pRule->replace.digit_transmitted, strLen);
							zyMemcpy((output + strLen), (input + pRule->replace.origin_digit_number), 
								total_digits - pRule->replace.origin_digit_number); /* remove prefix digits and copy others*/
						} else if ( pRule ->prefix != 0 ){
							strncat(output, (char *)(input ), pRule->prefix); /* append prefix digits */
							zyStrcat(output, pRule->replace.digit_transmitted); /* append prefix digits */
							zyStrcat(output, (char *)(input + pRule->prefix + pRule->replace.origin_digit_number));
						} else {
							zyMemcpy((output + strLen), (input + pRule->replace.origin_digit_number), 
								total_digits - pRule->replace.origin_digit_number); /* remove prefix digits and copy others*/
						}
						if(pRule->blocked == TRUE){
							rtn = -1;
						}else{
							rtn = 0;
						}
						*gateway = pRule->gateway; /* which gateway wants to pass through */
						zyMemset(input, 0, input_len);
						zyMemcpy(input, output, *length);
						goto end;
					}
				}
			}
			pRule = pRule->next;
		}
		#if 1 /*Jason , no match rule, the dialing number will be blocked*/
		/*printf("no match rule, the dialing number will be blocked!\n\r");*/
        rtn = -1;
		#else
		/* no match rule, but we don't need to block the dialing number! */
		rtn = -2;
		#endif
	}

end:
	if(output != NULL){
		zyFree(output); 
		output = NULL;
	}
		return rtn;
} /* transferByDialplan */

/*______________________________________________________________________________
**	instantDialOutCheck
**
**	descriptions:check the dialing digits whether instant number or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
bool
instantDialOutCheck(int VpItr, uint8   *input, uint8   *length)
{
	dial_plan_rule_t *pRule;
	uint8 *pDigit = NULL;
	int index, dailing_key, total_digits;

	pRule = rule_head[VpItr];
	if( pRule == NULL ) { /* no dial plan set */
		/* do nothing */
		return FALSE;
	} else {
		total_digits = *length;
		while( pRule != NULL ) {
			pDigit = input; /* reset pDigit */
			if(pRule->instant == TRUE){
				if( total_digits == pRule->digit_number ) {/* total dialing digits must equal to total digits in the rule */
					/* ready to compare digits between dailing and rule */
					for( index = 0; index < total_digits; index++ ) {
						if( *pDigit >= '0' && *pDigit <= '9' ) {
							dailing_key = (int)(1 << (*pDigit - '0'));
						} else if( *pDigit == '*' ) {
							dailing_key = 0x400; /* 0000010000000000 */
						} else if( *pDigit == '#' ) {
							dailing_key = 0x800; /* 0000100000000000 */
						} else {
							dailing_key = 0; /* other digits, never come here normally */
						}
						if( dailing_key & pRule->digit_table[index] ) {
							pDigit++;
						} else {
							break;/* next rule */
						}
						if( index == pRule->digit_number - 1 ) {/* match the last digit */
							return TRUE; /* match rule */
						}
					}
				}
			}
			pRule = pRule->next;
		}
	}
	return FALSE;
} /* instantDialOutCheck */

/*______________________________________________________________________________
**	DialOutPartialCheck
**
**	descriptions:check the dialing digits whether instant number or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
DialOutPartialCheck(int VpItr, uint8   *input, uint8   *length)
{
	int instantPartialCheck = -1;
	int noninstantPartialCheck = -1;

	instantDial = FALSE;
	instantPartialCheck = instantDialOutPartialCheck(VpItr , input, length);
	noninstantPartialCheck = noninstantDialOutPartialCheck(VpItr , input, length);
	if(DialPlanDebug){
		printf("dialDigisAdj : Before DIAL_PLAN check:instantPartialCheck=%d ,noninstantPartialCheck=%d \n",instantPartialCheck,noninstantPartialCheck);
	}
	if ((noninstantPartialCheck==0) && (instantPartialCheck==-1)) {
		if(DialPlanDebug){
			printf(" Match the open dialplan but didn't match the close dialplan. Inter-Digit timer will be 5 seconds\n ");
		}
		return 4;/*Run 5 seconds timer*/
	}
	if ((noninstantPartialCheck==-1)&&(instantPartialCheck==0)) {
		if(DialPlanDebug){
			printf(" Match the close dialplan but didn't match the open dialplan. Inter-Digit timer will be 15 seconds\n ");
		}
		return 5;/*Run 15 seconds timer*/
	}
	if ((noninstantPartialCheck==0)&&(instantPartialCheck==0)) {
		if(DialPlanDebug){
		printf(" Match the open dialplan and match the close dialplan. Inter-Digit timer will be default timer\n ");
		}
		return 0;/*Run default timer*/
	}
	if (instantPartialCheck == 1) { /*matched*/
		if(DialPlanDebug){
		printf(" dialDigisAdj  : DIAL_PLAN : whole match\n");
		}
		instantDial = TRUE;
		return 1;
	}
	if(DialPlanDebug){
		printf(" instantDial=%d\n",instantDial);
	}
	return 0;
}

/*______________________________________________________________________________
**	instantDialOutPartialCheck
**
**	descriptions:check the dialing digits whether instant number or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
instantDialOutPartialCheck(int VpItr , uint8   *input, uint8   *length)
{
	dial_plan_rule_t *pRule;
	uint8 *pDigit = NULL;
	int index, dailing_key, total_digits;
	if(VpItr<0){
		return -1;
	}
	pRule = rule_head[VpItr];  
	if( (pRule == NULL )||(FALSE==checkDialPlanActive(VpItr))) { /* no dial plan set */
		/* do nothing */
		return -1; /*no match rule*/
	} else {
		total_digits = *length;
		while( pRule != NULL ) {
			pDigit = input; /* reset pDigit */
			if(pRule->instant == TRUE){
				if( total_digits <= pRule->digit_number ) {/* total dialing digits must equal to total digits in the rule */
					/* ready to compare digits between dailing and rule */
					for( index = 0; index < total_digits; index++ ) {
						if( *pDigit >= '0' && *pDigit <= '9' ) {
							dailing_key = (int)(1 << (*pDigit - '0'));
						} else if( *pDigit == '*' ) {
							dailing_key = 0x400; /* 0000 0100 0000 0000 */
						} else if( *pDigit == '#' ) {
							dailing_key = 0x800; /* 0000100000000000 */
						} else {
							dailing_key = 0; /* other digits, never come here normally */
						}
						if( dailing_key & pRule->digit_table[index] ) {
							pDigit++;
						} else {
							break;/* next rule */
						}
						if( index == total_digits - 1 ) {/* match the last digit */
							if(total_digits == pRule->digit_number){
								return 1; /* match rule */
							} else{
								return 0; /*partial match rule*/
							}
						}
					}
				}
			}
			pRule = pRule->next;
		}
	}
	return -1; /*no match rule*/
} /* instantDialOutCheck */


/*______________________________________________________________________________
**	noninstantDialOutPartialCheck
**
**	descriptions:check the dialing digits whether instant number or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
noninstantDialOutPartialCheck(int VpItr, uint8   *input, uint8   *length)
{
	dial_plan_rule_t *pRule;
	uint8 *pDigit = NULL;
	int index, dailing_key, total_digits,i,num;
  	bool repeatSyntax;
	pRule = rule_head[VpItr];    
	if( (pRule == NULL )||(FALSE==checkDialPlanActive(VpItr))) { /* no dial plan set */
		/* do nothing */
		return -1; /*no match rule*/
	} else {
		total_digits = *length;
		while( pRule != NULL ) {
			pDigit = input; /* reset pDigit */
			if((pRule->instant == FALSE)&&(pRule->repeat==0xfff)){
				if(pRule->digit_number==0)/*For case "(x.)", if we found this rule,ignore it.*/
					break;/* try next rule */
				if(pRule->digit_number>total_digits)
					num=total_digits;
				else
					num=pRule->digit_number;
		                /* ready to compare digits between dailing and rule */
		               for( index = 0; index <= num; index++ ) {                    
		                    if( *pDigit >= '0' && *pDigit <= '9' ) {
		                        dailing_key = (int)(1 << (*pDigit - '0'));
		                    } else if( *pDigit == '*' ) {
		                        dailing_key = 0x400; /* 0000010000000000 */
		                    } else if( *pDigit == '#' ) {
		                        dailing_key = 0x800; /* 0000100000000000 */
		                    } else {
		                        dailing_key = 0; /* other digits, never come here normally */
		                    }
		                    if( dailing_key & pRule->digit_table[index] ) {
		                       pDigit++;
		                    } else {
		                       break;/* try next rule */
		                    }                                            
		                    if(index == num - 1) {
					  printf(" noninstantDialOutPartialCheck,partial match\n");
					  return 0; /*partial match rule*/
		                    }
		                }
	            }
		pRule = pRule->next;
		}
	}
	return -1; /*no match rule*/
} /* instantDialOutCheck */

/*______________________________________________________________________________
**	initDialPlan
**
**	descriptions:load dial plan from flash for initialization
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
initDialPlan(void)
{
	int NumOfProfile = 0 , VpItr = 0 , ret = 0;
	*dial_plan = '\0'; /* clear dial_plan */
//Ssu-Ying, support Multiple Profile
	NumOfProfile = tr104GetNumProfile();
	if (rule_head != NULL) {
		zyFree(rule_head);	
	}

	if (activeDialPlan != NULL) {
		zyFree(activeDialPlan);
	} 
	
	rule_head = (dial_plan_rule_t **)zyMalloc(NumOfProfile*sizeof(dial_plan_rule_t*));
	activeDialPlan = (bool *)zyMalloc(NumOfProfile*sizeof(bool));
	zyMemset(rule_head , NULL , (NumOfProfile*sizeof(dial_plan_rule_t*)));
	zyMemset(activeDialPlan , FALSE , (NumOfProfile*sizeof(bool)));	
		
	instantDial = FALSE;
	DialPlanDebug = FALSE;
	
	return doDialPlanLoad();

} /* initDialPlan */

/*______________________________________________________________________________
**	checkDialPlanActive
**
**	descriptions:check dial plan active or not
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
bool
checkDialPlanActive(int VpItr)
{   
    return activeDialPlan[VpItr];
} /* checkDialPlanActive */

/*______________________________________________________________________________
**  checkDialPlan
**
**  descriptions: check if the dial digits match the DialPlan.
**  parameters:
**  local:
**  global:
**  return:
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
int
checkDialPlan(int VpItr , dialDigitNum_t *dialDigitNum_p)
{
    int rtnValue = 0, gateway;
    uint8* ptr;
    int i;

	if(checkDialPlanActive(VpItr)){

		 if(DialPlanDebug == TRUE){
		 /* print dialing digits */
			 ptr = dialDigitNum_p->dial_digits;
			printf("Before Dial Plan: ");
			for(i = 0; i < dialDigitNum_p->current_length; i++){
				printf("%c ", *ptr);
				ptr++;
			}
			printf("\n\r");
		}
		rtnValue = transferByDialplan(VpItr , 
		                               &(dialDigitNum_p->dial_digits[0]),
	                                      &(dialDigitNum_p->current_length),
	                                      &gateway);
		
	
		if(rtnValue == -1){/* the dial number is blocked  or error occur */
			zyMemset( dialDigitNum_p, 0, sizeof(dialDigitNum_t) );
			dialDigitNum_p->current_length = 0;
		}else if(rtnValue == -2){/* donothing */
			 if(DialPlanDebug == TRUE){
				printf("no match rule  or no dial plan rule\n ");
			 }
		}else{
		        if(DialPlanDebug == TRUE){
		            ptr = dialDigitNum_p->dial_digits;
		            printf("After Dial Plan: ");
		            for(i = 0; i < dialDigitNum_p->current_length; i++){
		                printf("%c ", *ptr);
		                ptr++;
		            }
		            printf("\n\r");
		        }
				
			if(rtnValue == 0){   	
				return gateway;
			}
		}
	}else{
		rtnValue=-2;
		 if(DialPlanDebug == TRUE)
			printf(" activeDialPlan = FALSE ");
	}

    return rtnValue;
}


#endif /* DIAL_PLAN */

