

#ifndef _WEBCALLHISTORY_H_
#define _WEBCALLHISTORY_H_

#include <time.h>

#define CALLHISTORY_MAX_TIME_LENGTH 20
#define CALLHISTORY_MAX_CALLNUMBER  32+1
#define MAX_CALL_HISTORY_ENTRY_INCALLS 300
#define MAX_CALL_HISTORY_ENTRY_OUTCALLS 300
#define MAX_CALL_HISTORY_SUMMARY_DAY 45
#if 1 //Gino
#define CLEAR_CALL_HISTORY_DAY_SUMMARY 1
#define CLEAR_CALL_HISTORY_ENTRY_OUTCALLS 2
#define CLEAR_CALL_HISTORY_ENTRY_INCALLS 3
#endif



typedef struct spWebCallHistoryEntry_s{
	uint8	catalogClass;
	uint8	callHistoryConnectType;
	uint8   portType; /*DSP_FXS_PORT, DSP_DECT_PORT, ...*/
	uint8   portNum;
#if 1 /* Jason , syslog*/	
	uint8	SipAccountId;
#endif
   	uint32 	callHistoryStartTime;
	uint32 	callHistoryTalkStartTime;
   	uint32 	callHistoryTalkDuration;
   	char	callHistoryLocalNumber[CALLHISTORY_MAX_CALLNUMBER+1];
	char	callHistoryPeerNumber[CALLHISTORY_MAX_CALLNUMBER+1];
}spWebCallHistoryEntry_t;

typedef struct spCallHistorySummaryEntry_s{
    time_t 	startTime;
	uint32 	duration;
	uint16  outgoingCalls;
	uint16  incomingCalls;
	uint16  missingCalls;	
}spCallHistorySummaryEntry_t;	


typedef struct webCallHistoryBuffer_s {
	uint16 flags;
	uint16 inCallIndex;
	uint16 outCallIndex;
	spWebCallHistoryEntry_t webCallHistoryEntryInCalls[MAX_CALL_HISTORY_ENTRY_INCALLS]; 
	spWebCallHistoryEntry_t webCallHistoryEntryOutCalls[MAX_CALL_HISTORY_ENTRY_OUTCALLS];
	uint16 currentDaySummaryIndex;
	spCallHistorySummaryEntry_t dayCallHistory[MAX_CALL_HISTORY_SUMMARY_DAY];
	spCallHistorySummaryEntry_t weekCallHistory[2]; /*last week, this week*/
	spCallHistorySummaryEntry_t monthCallHistory[2]; /*last month, this month*/	
}webCallHistoryBuffer_t;


typedef enum {
	WEB_CALLHISTORY_RETURN_OK = 0,
	WEB_CALLHISTORY_RETURN_FAIL
} webCallHistoryReturn_e;

typedef enum {
	WEB_CALL_HISTORY_CATALOG_MISSED = 0,
	WEB_CALL_HISTORY_CATALOG_DIALED,
	WEB_CALL_HISTORY_CATALOG_RECEIVED
} webCallHistoryCatalogClass_e;


int webCallHistoryInit(void);
int webCallHistory_SetEntry(char *webCallHistory_data_p); 
char * webCallHistoryGetTime(uint32 *time);
int webCallHistory_DisplayEntry(void);
int webCallHistoryClearInCalls(void);
int webCallHistoryClearOutCalls(void); 
int webCallHistorySummary_Clean(void); 

#endif
