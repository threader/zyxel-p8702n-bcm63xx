/*
  $Id: webCallHistory.c 1.2 2007/05/03 02:34:04 eric.chung Exp $
*/
/******************************************************************************/
/*                                                    
 *	Copyright (C) 1994 ZyXEL Communications, Corp.
 *  All Rights Reserved.
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
** $Log: webCallHistory.c $
** Revision 1.2  2007/05/03 02:34:04  eric.chung
** add isdn support
** add file header
** Revision 1.1  1997/07/15 16:42:22  eric.chung
*/
#include <time.h>
#include "global.h"
#include "webCallHistory.h"
#include "call_mgnt.h"

webCallHistoryBuffer_t *webCallHistory_pool = NULL;

/*______________________________________________________________________________
**	webCallHistoryInit
**	descriptions: Call by VoIP initialization process
**	parameters:
**	local:
**	global:
**	return: 0
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int 
webCallHistoryInit(
	void
){
	uint32 ulValue = 0;
	uint8 *ptr;
	
	webCallHistory_pool = malloc(sizeof(webCallHistoryBuffer_t));	
	zyMemset((uint8 *)webCallHistory_pool, 0, sizeof(webCallHistoryBuffer_t));
	zyMemset(&(webCallHistory_pool->webCallHistoryEntryInCalls), 0 ,sizeof(spWebCallHistoryEntry_t)*MAX_CALL_HISTORY_ENTRY_INCALLS); 
	zyMemset(&(webCallHistory_pool->webCallHistoryEntryOutCalls), 0 ,sizeof(spWebCallHistoryEntry_t)*MAX_CALL_HISTORY_ENTRY_OUTCALLS); 
	zyMemset(&(webCallHistory_pool->dayCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*MAX_CALL_HISTORY_SUMMARY_DAY); 
	zyMemset(&(webCallHistory_pool->weekCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*2); 
	zyMemset(&(webCallHistory_pool->monthCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*2); 
	webCallHistory_pool->currentDaySummaryIndex = 0;

	return(WEB_CALLHISTORY_RETURN_OK);

}/* webCallHistoryInit */



//dennis_CallHistory
char
*webCallHistoryGetTime(
	uint32 *time
)
{
	static char timeStr[CALLHISTORY_MAX_TIME_LENGTH];
	struct tm *tmptr;

	zyMemset(timeStr, 0, CALLHISTORY_MAX_TIME_LENGTH);

   	tmptr = localtime((const long *)time);
    snprintf(	timeStr,
				CALLHISTORY_MAX_TIME_LENGTH,
    			"%02d/%02d/%d %.2d:%.2d:%.2d",
            	tmptr->tm_mon + 1,
              	tmptr->tm_mday,
             	tmptr->tm_year + 1900,
             	tmptr->tm_hour,
             	tmptr->tm_min,
             	tmptr->tm_sec
     		);

	return timeStr;
}

/* FUNCTION NAME : webCallHistoryDisplayEntry
 * PURPOSE:
 *      Show the completed call data to screen.
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */
int
webCallHistorySummaryAddEntry(spWebCallHistoryEntry_t *entry){
	int i = 0;
	int daySummaryIndex = 0;;
	struct tm* sTime = NULL, *lastDayTime = NULL, *lastWeekTime = NULL, *lastMonthTime = NULL;
	int entryYear = 0, entryMon = 0, entryWDay = 0, entryDay = 0, entryHour = 0, entryMin = 0;

	sTime = localtime(&entry->callHistoryStartTime);
	entryYear = sTime->tm_year;
	entryMon = sTime->tm_mon;
	entryDay = sTime->tm_mday;
	entryWDay = sTime->tm_wday;
	entryHour = sTime->tm_hour;
	entryMin = sTime->tm_min;
		
	if(webCallHistory_pool == NULL){
		ZyPrintf("webCallHistory not initialized, exiting..\n");
		return WEB_CALLHISTORY_RETURN_FAIL;
	}

	/********************************
	********Prepare Day Summary*****
	********************************/
	daySummaryIndex = webCallHistory_pool->currentDaySummaryIndex;
	if(webCallHistory_pool->dayCallHistory[daySummaryIndex].startTime > 0){
		lastDayTime = localtime(&webCallHistory_pool->dayCallHistory[daySummaryIndex].startTime);

		/*a new day call history summary record need to be added*/
		if( (entryYear != lastDayTime->tm_year) ||
			(entryMon != lastDayTime->tm_mon) ||
			(entryDay != lastDayTime->tm_mday)){									
			(webCallHistory_pool->currentDaySummaryIndex)++;
			if(webCallHistory_pool->currentDaySummaryIndex >= MAX_CALL_HISTORY_SUMMARY_DAY){
				webCallHistory_pool->currentDaySummaryIndex = 0;
			}
			daySummaryIndex = webCallHistory_pool->currentDaySummaryIndex;
			zyMemset(&(webCallHistory_pool->dayCallHistory[daySummaryIndex]), 0, sizeof(spCallHistorySummaryEntry_t));
			webCallHistory_pool->dayCallHistory[daySummaryIndex].startTime = entry->callHistoryStartTime;
			CCM_PRINTF("Call History, New Day Summary Added, index %d\n", daySummaryIndex);
		}
	}
	else{
		/*first time day call history summary*/
		webCallHistory_pool->dayCallHistory[daySummaryIndex].startTime = entry->callHistoryStartTime;
		CCM_PRINTF("Call History, First Day Summary Added, index %d\n", daySummaryIndex);		
	}
			
	
	/********************************
	********Prepare Week Summary*****
	********************************/
	if(webCallHistory_pool->weekCallHistory[1].startTime > 0){ /*weekCallHistory[1] - this week, weekCallHistory[0] - last week*/
		lastWeekTime = localtime(&webCallHistory_pool->weekCallHistory[1].startTime);	
		/*a new day call history summary record need to be added*/
		if( entry->callHistoryStartTime > (webCallHistory_pool->weekCallHistory[1].startTime + 7*86400)){
			
			zyMemcpy(&(webCallHistory_pool->weekCallHistory[0]), &(webCallHistory_pool->weekCallHistory[1]), sizeof(spCallHistorySummaryEntry_t));
		
			zyMemset(&(webCallHistory_pool->weekCallHistory[1]), 0, sizeof(spCallHistorySummaryEntry_t));
			webCallHistory_pool->weekCallHistory[1].startTime = entry->callHistoryStartTime - (entryWDay * 86400); /*set to first day of the week*/
			CCM_PRINTF("Call History, New Week Summary Added, week day %d\n", entryWDay);
		}
		
	}
	else{
		/*first time week call history summary*/
		webCallHistory_pool->weekCallHistory[1].startTime = entry->callHistoryStartTime - (entryWDay * 86400); /*set to first day of the week*/
		CCM_PRINTF("Call History, First Week Summary Added, week day %d\n", entryWDay);
	}

	/********************************
	********Prepare Month Summary*****
	********************************/
	if(webCallHistory_pool->monthCallHistory[1].startTime > 0){ /*monthCallHistory[1] - this month, monthCallHistory[0] - last month*/
		lastMonthTime = localtime(&webCallHistory_pool->monthCallHistory[1].startTime);

		/*a new day call history summary record need to be added*/
		if( (entryYear != lastWeekTime->tm_year) ||
			(entryMon != lastWeekTime->tm_mon)){
			
			zyMemcpy(&(webCallHistory_pool->monthCallHistory[0]), &(webCallHistory_pool->monthCallHistory[1]), sizeof(spCallHistorySummaryEntry_t));
		
			zyMemset(&(webCallHistory_pool->monthCallHistory[1]), 0, sizeof(spCallHistorySummaryEntry_t));
			webCallHistory_pool->monthCallHistory[1].startTime = entry->callHistoryStartTime;
			CCM_PRINTF("Call History, New Month Summary Added\n");			
		}
		
	}
	else{
		/*first time week call history summary*/
		webCallHistory_pool->monthCallHistory[1].startTime = entry->callHistoryStartTime;
		CCM_PRINTF("Call History, First Month Summary Added\n");
	}

	
	/********************************
	********Update  Summary*****
	********************************/
	switch(entry->catalogClass){
		case WEB_CALL_HISTORY_CATALOG_MISSED:
			(webCallHistory_pool->dayCallHistory[daySummaryIndex].missingCalls)++; 
			(webCallHistory_pool->weekCallHistory[1].missingCalls)++; 
			(webCallHistory_pool->monthCallHistory[1].missingCalls)++; 
			#ifdef MSTC_EMAIL_NOTIFICATION
			sendMissedCallEmailNotification(entry->SipAccountId, entry->portType, entry->callHistoryLocalNumber, entry->callHistoryPeerNumber, entry->callHistoryStartTime);						
			#endif
			break;
		case WEB_CALL_HISTORY_CATALOG_DIALED:
			(webCallHistory_pool->dayCallHistory[daySummaryIndex].outgoingCalls)++;
			webCallHistory_pool->dayCallHistory[daySummaryIndex].duration += entry->callHistoryTalkDuration;
			(webCallHistory_pool->weekCallHistory[1].outgoingCalls)++;
			webCallHistory_pool->weekCallHistory[1].duration += entry->callHistoryTalkDuration;
			(webCallHistory_pool->monthCallHistory[1].outgoingCalls)++;
			webCallHistory_pool->monthCallHistory[1].duration += entry->callHistoryTalkDuration;
			break;
		case WEB_CALL_HISTORY_CATALOG_RECEIVED:			
			(webCallHistory_pool->dayCallHistory[daySummaryIndex].incomingCalls)++;
			webCallHistory_pool->dayCallHistory[daySummaryIndex].duration += entry->callHistoryTalkDuration;
			(webCallHistory_pool->weekCallHistory[1].incomingCalls)++;
			webCallHistory_pool->weekCallHistory[1].duration += entry->callHistoryTalkDuration;
			(webCallHistory_pool->monthCallHistory[1].incomingCalls)++;
			webCallHistory_pool->monthCallHistory[1].duration += entry->callHistoryTalkDuration;
			break;
	} /*switch*/

	CCM_PRINTF("******************Call Day History Summary******************************\r\n");
	CCM_PRINTF("startTime  outgoinCalls incomingCalls missingCalls duration\r\n");
	CCM_PRINTF("%10d %5d %5d %5d %5d\r\n", 
		webCallHistory_pool->dayCallHistory[daySummaryIndex].startTime,
		webCallHistory_pool->dayCallHistory[daySummaryIndex].outgoingCalls,
		webCallHistory_pool->dayCallHistory[daySummaryIndex].incomingCalls,
		webCallHistory_pool->dayCallHistory[daySummaryIndex].missingCalls,
		webCallHistory_pool->dayCallHistory[daySummaryIndex].duration
		);

	CCM_PRINTF("******************Call Week History Summary******************************\r\n");
	CCM_PRINTF("startTime  outgoinCalls incomingCalls missingCalls duration\r\n");
	CCM_PRINTF("%10d %5d %5d %5d %5d\r\n", 
		webCallHistory_pool->weekCallHistory[1].startTime,
		webCallHistory_pool->weekCallHistory[1].outgoingCalls,
		webCallHistory_pool->weekCallHistory[1].incomingCalls,
		webCallHistory_pool->weekCallHistory[1].missingCalls,
		webCallHistory_pool->weekCallHistory[1].duration
		);

	CCM_PRINTF("******************Call Month History Summary******************************\r\n");
	CCM_PRINTF("startTime  outgoinCalls incomingCalls missingCalls duration\r\n");
	CCM_PRINTF("%10d %5d %5d %5d %5d\r\n", 
		webCallHistory_pool->monthCallHistory[1].startTime,
		webCallHistory_pool->monthCallHistory[1].outgoingCalls,
		webCallHistory_pool->monthCallHistory[1].incomingCalls,
		webCallHistory_pool->monthCallHistory[1].missingCalls,
		webCallHistory_pool->monthCallHistory[1].duration
		);
	return WEB_CALLHISTORY_RETURN_OK;
}


/* FUNCTION NAME : webCallHistoryDisplayEntry
 * PURPOSE:
 *      Show the completed call data to screen.
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */
int
webCallHistoryDisplayEntry(void)
{
	int i, num = 0, index = 0;
	char timeStr[CALLHISTORY_MAX_TIME_LENGTH];
	
	if(webCallHistory_pool == NULL){
		ZyPrintf("webCallHistory not initialized, exiting..\n");
		return WEB_CALLHISTORY_RETURN_FAIL;
	}

	
	if(webCallHistory_pool->webCallHistoryEntryInCalls[webCallHistory_pool->inCallIndex].callHistoryStartTime==0){
		num = webCallHistory_pool->inCallIndex;
		ZyPrintf("\r\n Number of Incoming Call History: %d\r\n", webCallHistory_pool->inCallIndex);
	}
	else{
		num = MAX_CALL_HISTORY_ENTRY_INCALLS;
		ZyPrintf("\r\n Number of Incoming Call History: %d\r\n", MAX_CALL_HISTORY_ENTRY_INCALLS);
	}
	if(num > 0){
		ZyPrintf("Date/Time            LocalNum  PeerNum  Duration PortType PortNum\r\n");
		ZyPrintf("--------------------------------------------------------------------\r\n");
		for(i = webCallHistory_pool->inCallIndex; i < MAX_CALL_HISTORY_ENTRY_INCALLS; i++){
			if(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime != 0){				
				zyStrncpy(timeStr, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime)), CALLHISTORY_MAX_TIME_LENGTH);
				ZyPrintf("%s  %s     %s        %d      %d   %d\r\n",  
					timeStr,				 
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryLocalNumber, 
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryPeerNumber,
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryTalkDuration,					
					webCallHistory_pool->webCallHistoryEntryInCalls[i].portType,
					webCallHistory_pool->webCallHistoryEntryInCalls[i].portNum
					 );
			}
		}
		
		for(i=0; i< webCallHistory_pool->inCallIndex; i++){
			if(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime != 0){				
				zyStrncpy(timeStr, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime)), CALLHISTORY_MAX_TIME_LENGTH);
				ZyPrintf("%s  %s     %s        %d      %d   %d\r\n",  
					timeStr,				 
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryLocalNumber, 
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryPeerNumber,
					webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryTalkDuration,					
					webCallHistory_pool->webCallHistoryEntryInCalls[i].portType,
					webCallHistory_pool->webCallHistoryEntryInCalls[i].portNum
					 );
			}
		}
		ZyPrintf("-------------------------------------------------------------------\r\n");
	}	

	if(webCallHistory_pool->webCallHistoryEntryOutCalls[webCallHistory_pool->outCallIndex].callHistoryStartTime==0){
		ZyPrintf("\r\n Number of Outgoing Call History: %d\r\n", webCallHistory_pool->outCallIndex);
		num = webCallHistory_pool->outCallIndex;
	}
	else{
		ZyPrintf("\r\n Number of Outgoing Call History: %d\r\n", MAX_CALL_HISTORY_ENTRY_OUTCALLS);
		num = MAX_CALL_HISTORY_ENTRY_OUTCALLS;
	}	
	if(num > 0){
		ZyPrintf("Date/Time            LocalNum  PeerNum  Duration PortType PortNum\r\n");
		ZyPrintf("--------------------------------------------------------------------\r\n");
		for(i = webCallHistory_pool->outCallIndex; i < MAX_CALL_HISTORY_ENTRY_OUTCALLS; i++){
			if(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime != 0){
				zyStrncpy(timeStr, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime)), CALLHISTORY_MAX_TIME_LENGTH);
				ZyPrintf("%s  %s     %s        %d      %d   %d\r\n", 
					timeStr, 				
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryLocalNumber, 
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryPeerNumber,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryTalkDuration,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].portType,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].portNum
					 );
			}
		}
		
		for(i=0; i< webCallHistory_pool->outCallIndex; i++){
			if(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime != 0){
				zyStrncpy(timeStr, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime)), CALLHISTORY_MAX_TIME_LENGTH);
				ZyPrintf("%s  %s     %s        %d      %d   %d\r\n", 
					timeStr, 				
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryLocalNumber, 
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryPeerNumber,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryTalkDuration,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].portType,
					webCallHistory_pool->webCallHistoryEntryOutCalls[i].portNum
					 );
			}
		}
		ZyPrintf("-------------------------------------------------------------------\r\n");
	}	
	return WEB_CALLHISTORY_RETURN_OK;
}/* webCallHistoryDisplayEntry */


/* FUNCTION NAME : webCallHistoryDisplaySummary
 * PURPOSE:
 *      Show the completed call data to screen.
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */
int
webCallHistoryDisplaySummary(void)
{
	int i = 0;
	char timeStr[CALLHISTORY_MAX_TIME_LENGTH];
	struct tm *tmptm= NULL;
		
	if(webCallHistory_pool == NULL){
		ZyPrintf("webCallHistory not initialized, exiting..\n");
		return WEB_CALLHISTORY_RETURN_FAIL;
	}

	ZyPrintf("*******************Call Day History Summary***************************\r\n");
	ZyPrintf("Date        totalCalls  outCalls  inCalls  missCalls   Duration(s)\r\n");

	for(i=webCallHistory_pool->currentDaySummaryIndex+1; i<MAX_CALL_HISTORY_SUMMARY_DAY; i++){		
		if(0 != webCallHistory_pool->dayCallHistory[i].startTime){
			tmptm = localtime(&(webCallHistory_pool->dayCallHistory[i].startTime));
			snprintf(timeStr, CALLHISTORY_MAX_TIME_LENGTH, "%2d/%2d/%d", tmptm->tm_mon+1, tmptm->tm_mday, tmptm->tm_year+1900);
			ZyPrintf("%s %5d     %5d     %5d     %5d       %5d\r\n", 
				timeStr,
				(webCallHistory_pool->dayCallHistory[i].outgoingCalls+
				webCallHistory_pool->dayCallHistory[i].incomingCalls+
				webCallHistory_pool->dayCallHistory[i].missingCalls),
				webCallHistory_pool->dayCallHistory[i].outgoingCalls,
				webCallHistory_pool->dayCallHistory[i].incomingCalls,
				webCallHistory_pool->dayCallHistory[i].missingCalls,
				webCallHistory_pool->dayCallHistory[i].duration
				);
		}
		else{
			break;
		}
	}

	for(i=0; i<=webCallHistory_pool->currentDaySummaryIndex; i++){
		if(0 != webCallHistory_pool->dayCallHistory[i].startTime){			
			tmptm = localtime(&(webCallHistory_pool->dayCallHistory[i].startTime));
			snprintf(timeStr, CALLHISTORY_MAX_TIME_LENGTH, "%2d/%2d/%d", tmptm->tm_mon+1, tmptm->tm_mday, tmptm->tm_year+1900);
			ZyPrintf("%s %5d     %5d     %5d     %5d       %5d\r\n",  
				timeStr,
				(webCallHistory_pool->dayCallHistory[i].outgoingCalls+
				webCallHistory_pool->dayCallHistory[i].incomingCalls+
				webCallHistory_pool->dayCallHistory[i].missingCalls),
				webCallHistory_pool->dayCallHistory[i].outgoingCalls,
				webCallHistory_pool->dayCallHistory[i].incomingCalls,
				webCallHistory_pool->dayCallHistory[i].missingCalls,
				webCallHistory_pool->dayCallHistory[i].duration
				);
		}
	}	
	
	
	tmptm = localtime(&(webCallHistory_pool->weekCallHistory[1].startTime));
	snprintf(timeStr, CALLHISTORY_MAX_TIME_LENGTH, "%2d/%2d/%d", tmptm->tm_mon+1, tmptm->tm_mday, tmptm->tm_year+1900);
	ZyPrintf("******************Call Week History Summary******************************\r\n");
	ZyPrintf("Date        totalCalls  outCalls  inCalls  missCalls   Duration(s)\r\n");
	ZyPrintf("%s %5d     %5d     %5d     %5d       %5d\r\n", 
		timeStr,
		(webCallHistory_pool->weekCallHistory[1].outgoingCalls+
		webCallHistory_pool->weekCallHistory[1].incomingCalls+
		webCallHistory_pool->weekCallHistory[1].missingCalls),
		webCallHistory_pool->weekCallHistory[1].outgoingCalls,
		webCallHistory_pool->weekCallHistory[1].incomingCalls,
		webCallHistory_pool->weekCallHistory[1].missingCalls,
		webCallHistory_pool->weekCallHistory[1].duration
		);


	tmptm = localtime(&(webCallHistory_pool->monthCallHistory[1].startTime));
	snprintf(timeStr, CALLHISTORY_MAX_TIME_LENGTH, "%2d/%2d/%d", tmptm->tm_mon+1, tmptm->tm_mday, tmptm->tm_year+1900);
	ZyPrintf("******************Call Month History Summary******************************\r\n");
	ZyPrintf("Date        totalCalls  outCalls  inCalls  missCalls   Duration(s)\r\n");
	ZyPrintf("%s %5d     %5d     %5d     %5d       %5d\r\n", 
		timeStr,
		(webCallHistory_pool->monthCallHistory[1].outgoingCalls+
		webCallHistory_pool->monthCallHistory[1].incomingCalls+
		webCallHistory_pool->monthCallHistory[1].missingCalls),
		webCallHistory_pool->monthCallHistory[1].outgoingCalls,
		webCallHistory_pool->monthCallHistory[1].incomingCalls,
		webCallHistory_pool->monthCallHistory[1].missingCalls,
		webCallHistory_pool->monthCallHistory[1].duration
		);
	return WEB_CALLHISTORY_RETURN_OK;
}/* webCallHistory_DisplayEntry */

/* FUNCTION NAME : webCallHistoryClearOutCalls
 * PURPOSE:
 *      clear outgoing call history entry
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */

int
webCallHistoryClearOutCalls(void)
{
	zyMemset(&(webCallHistory_pool->webCallHistoryEntryOutCalls), 0 ,sizeof(spWebCallHistoryEntry_t)*MAX_CALL_HISTORY_ENTRY_OUTCALLS); 	
	webCallHistory_pool->outCallIndex=0;
	return(WEB_CALLHISTORY_RETURN_OK);
}

/* FUNCTION NAME : webCallHistoryClearInCalls
 * PURPOSE:
 *      clear incoming call history entry
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */

int
webCallHistoryClearInCalls(void)
{
	zyMemset(&(webCallHistory_pool->webCallHistoryEntryInCalls), 0 ,sizeof(spWebCallHistoryEntry_t)*MAX_CALL_HISTORY_ENTRY_INCALLS); 
	webCallHistory_pool->inCallIndex=0;
	return(WEB_CALLHISTORY_RETURN_OK);
}

/* FUNCTION NAME : webCallHistoryClearInCalls
 * PURPOSE:
 *      clear alll history summary entry
 * INPUT:
 *      None.
 *
 * OUTPUT:
 *      None.
 *
 * RETURN:
 *      WEB_CALLHISTORY_RETURN_OK = 0,
 *      WEB_CALLHISTORY_RETURN_FAIL.
 *
 * NOTES:
 *
 */

int
webCallHistorySummaryClearAll(void)
{
	zyMemset(&(webCallHistory_pool->dayCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*MAX_CALL_HISTORY_SUMMARY_DAY); 
	zyMemset(&(webCallHistory_pool->weekCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*2); 
	zyMemset(&(webCallHistory_pool->monthCallHistory), 0 ,sizeof(spCallHistorySummaryEntry_t)*2); 
	webCallHistory_pool->currentDaySummaryIndex = 0;
	return(WEB_CALLHISTORY_RETURN_OK);
} 





