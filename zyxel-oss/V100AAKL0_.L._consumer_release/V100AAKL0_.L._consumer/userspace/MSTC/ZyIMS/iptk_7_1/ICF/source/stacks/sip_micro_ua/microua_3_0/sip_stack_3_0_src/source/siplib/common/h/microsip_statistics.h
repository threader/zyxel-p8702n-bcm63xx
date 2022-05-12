/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 ** FUNCTION:
 **	 This file contains the protypes of the functions used to manipulate
 **  Statistics APIs
 ******************************************************************************
 **
 ** FILENAME:
 ** microsip_statistics.h
 **
 ** DESCRIPTION:
 **	All modules using SipLists must include this file for the structure definitions
 **	and the function prototypes.
 **
 **
 ** DATE		NAME		REFERENCE	 REASON
 ** ----		----		--------	 ------
 **23-03-2002 	Mahesh		Core Stack			
 **
 ** COPYRIGHT , Aricent, 2006
 *****************************************************************************/


#ifndef __SIPSTAT_H_
#define __SIPSTAT_H_
#include "microsip_common.h"
#include "microsip_struct.h"
extern SIP_U32bit glbSipParserMemErr;
extern SIP_U32bit glbSipParserNetErr;
extern SIP_U32bit glbSipParserProtoErr;
extern SIP_U32bit glbSipParserApiCount;
extern SIP_U32bit glbSipParserReqParsed;
extern SIP_U32bit glbSipParserRespParsed;
extern SIP_U32bit glbSipParserReqSent;
extern SIP_U32bit glbSipParserRespSent;
#ifdef SIP_THREAD_SAFE
extern synch_id_t glbLockStatisticsMutex;
#endif

/* Types of statistics */
#define SIP_STAT_TYPE_API		0
#define SIP_STAT_TYPE_ERROR		1
#define SIP_STAT_TYPE_INTERNAL	2
/* Counters recording API statistics */
#define SIP_STAT_API_ALL			0
#define SIP_STAT_API_COUNT			1
#define SIP_STAT_API_REQ_PARSED 	2
#define SIP_STAT_API_RESP_PARSED	3
#define SIP_STAT_API_REQ_SENT		4
#define SIP_STAT_API_RESP_SENT		5
/* Counters recording SIP_ERROR statistics */
#define SIP_STAT_ERROR_ALL			0
#define SIP_STAT_ERROR_MEM			1
#define SIP_STAT_ERROR_NETWORK		2
#define SIP_STAT_ERROR_PROTOCOL 	3
/* Macros for incerementing and decrmenting counters */

/*
 * if thread safe is defined
 * the global statistics varialbes are locked before doning any manipulation
 * on them
 */
#ifdef SIP_STATISTICS
#ifdef SIP_THREAD_SAFE

#define INC_ERROR_MEM		{\
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
							glbSipParserMemErr++;\
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}

#define DEC_ERROR_MEM		{\
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
							glbSipParserMemErr--; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_ERROR_NETWORK	{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserNetErr++; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_ERROR_NETWORK	{	\
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserNetErr--; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_ERROR_PROTOCOL	{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserProtoErr++; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_ERROR_PROTOCOL	{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserProtoErr--; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_API_COUNT		{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserApiCount++; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_API_COUNT		{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserApiCount--; \
							fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_API_REQ_PARSED	{ \
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserReqParsed++; \
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_API_REQ_PARSED	{ \
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserReqParsed--; \
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_API_RESP_PARSED	{ \
							fast_lock_synch(0,&glbLockStatisticsMutex, \
								FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserRespParsed++; \
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_API_RESP_PARSED	{ \
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserRespParsed--; \
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_API_REQ_SENT	{ \
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserReqSent++;\
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define DEC_API_REQ_SENT	{	\
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserReqSent--;\
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#define INC_API_RESP_SENT 	{	\
								fast_lock_synch(0,&glbLockStatisticsMutex, \
									FAST_MXLOCK_EXCLUSIVE ); \
								glbSipParserRespSent++;\
								fast_unlock_synch(0,&glbLockStatisticsMutex );\
							}
#else

#define INC_ERROR_MEM		glbSipParserMemErr++;
#define DEC_ERROR_MEM		glbSipParserMemErr--;
#define INC_ERROR_NETWORK	glbSipParserNetErr++;
#define DEC_ERROR_NETWORK	glbSipParserNetErr--;
#define INC_ERROR_PROTOCOL	glbSipParserProtoErr++;
#define DEC_ERROR_PROTOCOL	glbSipParserProtoErr--;
#define INC_API_COUNT		glbSipParserApiCount++;
#define DEC_API_COUNT		glbSipParserApiCount--;
#define INC_API_REQ_PARSED	glbSipParserReqParsed++;
#define DEC_API_REQ_PARSED	glbSipParserReqParsed--;
#define INC_API_RESP_PARSED	glbSipParserRespParsed++;
#define DEC_API_RESP_PARSED	glbSipParserRespParsed--;
#define INC_API_REQ_SENT	glbSipParserReqSent++;
#define DEC_API_REQ_SENT	glbSipParserReqSent--;
#define INC_API_RESP_SENT	glbSipParserRespSent++;
#endif

#else
/*
 * If SIP_STATISTICS are not used these
 * macros for incrementing /decrementing statistics are not used
 */
#define INC_ERROR_MEM		;
#define DEC_ERROR_MEM		;
#define INC_ERROR_NETWORK	;
#define DEC_ERROR_NETWORK	;
#define INC_ERROR_PROTOCOL	;
#define DEC_ERROR_PROTOCOL	;
#define INC_API_COUNT		;
#define DEC_API_COUNT		;
#define INC_API_REQ_PARSED	;
#define DEC_API_REQ_PARSED	;
#define INC_API_RESP_PARSED	;
#define DEC_API_RESP_PARSED	;
#define INC_API_REQ_SENT	{};
#define DEC_API_REQ_SENT	;
#define INC_API_RESP_SENT	;
#endif
/* Used in the getStatistics call to reset the counter */
#define SIP_STAT_RESET		0
#define SIP_STAT_NO_RESET	1

/*******************************************************************************
** FUNCTION: sip_initStatistics
** DESCRIPTION: inits Statistics
** 
** PARAMETERS:
**      	      dType(IN)			:The error Level
**				 scope(IN)			:The scope from  which statistics has 
**										to be collected
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern SipBool sip_initStatistics _ARGS_((SIP_U8bit type,SIP_U8bit scope, \
	SipError *err));
/*******************************************************************************
** FUNCTION: sip_getStatistics
** DESCRIPTION: Retrieves particular statistics
** 
** PARAMETERS:
**      	      	dType(IN)			:The error Level
**				 	scope(IN)			:The scope from  which statistics has 
**					reset(IN)			:Whether to reset the statistics.
**				  	err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern SipBool sip_getStatistics _ARGS_((SIP_U8bit type,SIP_U8bit scope, \
	SIP_U8bit reset, SIP_Pvoid stats, SipError *err));

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

