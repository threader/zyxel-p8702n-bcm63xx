/**************************************************************
** FUNCTION:
**			Contains prototypes for Timer Related Functions
***************************************************************
** FILENAME:
** siptxntimer.h
**
** DESCRIPTION: This file contains the prototypes for the call backs 
** 		related to transaction layer .The user is is free to have his 
**		own implementation for these interfaces.
**
** DATE			NAME			REFERENCE		REASON
** ----			----			---------		------
** 07/02/2002	Mahesh		    -			Initial Creation
**
** COPYRIGHT , Aricent, 2006
**************************************************************/
/* Ensure Names are not Mangled by C++ Compilers */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TXN_TIMER_H__
#define __TXN_TIMER_H__
#include "microsip_common.h"
#include "microsip_txnstruct.h"


#define GRANULARITY 100
#define MAX_TIMEOUT 32000
#define TXN_CREAT	1
#define TXN_REMOVE	2


/***************************************************************************
* FUNCTION: fast_startTxnTimer
* Parameters
*
* dDuration(IN)		:	Duration of the Timer
* timeOutFuncPtr(IN):	TimeOut function to be invoked on Timer Expiry
* pData(IN)			:	Buffer to be stored
* ppHandle(OUT)		:	Handle to the timer, to be used with
*							fast_stopTxnTimer
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool fast_startTxnTimer(SIP_U32bit dDuration,\
	TimeoutFuncPtr timeOutFuncPtr, SIP_Pvoid pData, SIP_Pvoid* ppHandle,\
	SipError *pErr);
/***************************************************************************
* FUNCTION: fast_stopTxnTimer
* Parameters
*
* pInkey(IN)		:	Handle to the Timer that is to be stoped
* ppBuff(OUT)		:	Buffer that was stored during the fast_startTxnTimer
*							will be returned
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool fast_stopTxnTimer(SIP_Pvoid pInkey,SIP_Pvoid* ppBuff,\
	SipError* pErr);
/***************************************************************************
* FUNCTION: sip_timerThread
* Parameters
*
* pTWheel(IN)		:	Pointer to the timer wheel
****************************************************************************/
#if !defined(SIP_VXWORKS) && !defined(SIP_WINDOWS)  && !defined(SIP_WINCE)
SIP_Pvoid sip_timerThread(SIP_Pvoid pTWheel);
#endif

/***************************************************************************
* FUNCTION: __sip_sleep
* Parameters
*
* dMicrosecs(IN)	:	Sleep duration in MicroSeconds
****************************************************************************/
void __sip_sleep(SIP_U32bit dMicrosecs);
/***************************************************************************
* FUNCTION: __sip_flushTimer
* Parameters
*
* None
****************************************************************************/
void __sip_flushTimer(void);
/***************************************************************************
* FUNCTION: __sip_flushHash
* Parameters
*
* None
****************************************************************************/
void __sip_flushHash(void);
/***************************************************************************
* FUNCTION: __sip_getHandle
* Parameters
*
* pHandle(OUT)		:	Handle will be returned with this variable
****************************************************************************/
SIP_S32bit __sip_getHandle(SIP_U32bit* pHandle);
/***************************************************************************
* FUNCTION: sip_freeTimerWheel
* Parameters
*
* None
****************************************************************************/
extern void sip_freeTimerWheel(void);
/***************************************************************************
* FUNCTION: sip_freeHashTable
* Parameters
*
* None
****************************************************************************/
extern void sip_freeHashTable(void);

/***************************************************************************
* FUNCTION: sip_cbk_fetchTxn
* Parameters
*
* ppTxnBuffer(OUT)	:	Txn Buffer pointer will be returned with
*						this variable
* pTxnKey(IN)		:	Txn Key for fetching the Txn Element
* pContext(IN)		:	Extra value being stored
* dOpt(IN)			:	When set to O_CREAT, new Txn Element will be
* 						created if it can't find one
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool sip_cbk_fetchTxn(SIP_Pvoid* ppTxnBuffer,SIP_Pvoid pTxnKey, \
			SIP_Pvoid pContext,SIP_S8bit dOpt,SipError *pErr);

/***************************************************************************
* FUNCTION: sip_cbk_releaseTxn
* Parameters
*
* pTxnKey(IN)		:	Key to the Element
* ppTxnKey(OUT)		:	Key of the Element being released
* ppTxnBuffer(OUT)	:	Buffer of the Txn Element being released
* dOpt(IN)			:	When set to O_REMOVE, Txn Element will be deleted
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool sip_cbk_releaseTxn(SIP_Pvoid pTxnKey, SIP_Pvoid* ppTxnKey,\
			SIP_Pvoid *ppTxnBuffer, SIP_Pvoid pContextData, SIP_S8bit dOpt,\
			SipError *pErr);

/***************************************************************************
* FUNCTION: sip_freeTimerHandle
* Parameters
*
* pTimerHandle(OUT)			:	SIP_Pvoid timer handle
****************************************************************************/
extern void sip_freeTimerHandle(SIP_Pvoid pTimerHandle);

/***************************************************************************
* FUNCTION: __sip_hashCalc
* Parameters
*
* pKey(IN)			:	Key used for calculating the Hash Value
****************************************************************************/
SIP_U32bit __sip_hashCalc(SIP_Pvoid pKey);
/***************************************************************************
* FUNCTION: sip_getAllTxnElem
* Parameters
*
* ppTxnKeys(OUT)	:	Keys of all the Txn Elements will be returned
* pCount(OUT)		:	Count of Txn Elements
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool sip_getAllKeysOfTxnElem(SipTxnKey** ppTxnKeys,\
		SIP_U32bit* pCount,SipError* pErr);

/***************************************************************************
* FUNCTION: sip_getBranchFromViaHdr
* Parameters
*
* pViaHeader(IN)	:	Via Header from which branch is extracted
* ppBranch(OUT)		:	Branch from Via Hdr
* pErr(OUT)			:	Error variable
****************************************************************************/
extern SipBool sip_getBranchFromViaHdr _ARGS_((SipViaHeader *pViaHeader, \
	SIP_S8bit **ppBranch, SipError *pErr));



#endif

#ifdef __cplusplus
}
#endif
