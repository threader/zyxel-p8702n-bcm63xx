
/******************************************************************************
 ** FUNCTION:   Has the functions for handling the transaction layer 
 **				aggregation and other APIs for txn layer integration.
 **
 ******************************************************************************
 **
 ** FILENAME: 		sdf_txn.h
 **
 ** DESCRIPTION:	This file contains declarations for functions used 
 **					to handle the SIP transaction layer integration.
 **
 **  DATE			NAME            REFERENCE             REASON
 **  ----			----            ---------            --------
 ** 19-July-02		Siddharth						     Creation
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_TXN_H__
#define __SDF_TXN_H__

#include "sdf_common.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
#ifdef SDF_TXN

/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaAbortCall
 ** 
 ** DESCRIPTION:	Aborts a call that was started/recd by the application. 
 **					If the application is retransmitting any message for 
 **					any transactions of this call-leg, the retransmissions
 **					are stopped at this stage without getting any response.
 **
 ** NOTE:			The application must ensure that the call object is 
 **					locked before the invocation of this API.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAbortCall _ARGS_((Sdf_st_callObject *pCallObj, \
	Sdf_ty_pvoid pContext, Sdf_st_error *pError));
	

/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaAbortTransaction
 ** 
 ** DESCRIPTION:	Aborts a transaction that was sent out/recd by the 
 **					application. If the application is retransmitting any
 **					message for this transaction, the retransmissions are
 **					stopped at this stage without getting any response.
 **
 ** NOTE:			The application must ensure that the call object is 
 **					locked before the invocation of this API.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAbortTransaction _ARGS_((Sdf_st_callObject *pCallObj,\
	 Sdf_ty_txnIdT dTxnId, Sdf_ty_pvoid pContext, Sdf_st_error *pError));

/* =========================================================================
 *				UA-T TXN LAYER RELATED INTERNAL APIs FOLLOW
 * ========================================================================= */

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaAbortTransaction
 ** 
 ** DESCRIPTION:	Internal API to invoke the stack and abort the SIP
 **					transaction.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAbortTransaction _ARGS_((Sdf_st_callObject *pCallObj, \
	 SipTxnKey *pSipTxnKey, Sdf_ty_pvoid pContext, Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaStartNoAnswerTimer
 **
 ** DESCRIPTION	:	Function that the UA Toolkit invokes within 
 **					sdf_ivk_uaUpdateCallDetails to start a timer upon 
 **					receipt of a provisional response.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStartNoAnswerTimer _ARGS_((\
	Sdf_st_callObject *pCallObj, Sdf_st_eventContext *pEventContext,
	Sdf_ty_bool dReset, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaStopNoAnswerTimer
 **
 ** DESCRIPTION	:	Function that the UA Toolkit invokes within 
 **					sdf_ivk_uaUpdateCallDetails to stop the timer that was
 **					started upon receipt of the last provisional response.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStopNoAnswerTimer _ARGS_((\
	Sdf_st_callObject *pCallObj, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaHandleNoAnswerTimerExpiry
 **
 ** DESCRIPTION	:	Function that the application invokes so that the UATK
 **					can inform the application of a "call-setup" timer 
 **					getting expired.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleNoAnswerTimerExpiry _ARGS_((\
	Sdf_ty_timerType dTimerType, Sdf_ty_pvoid pContextInfo, \
	Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION	:	sdf_ivk_uaHandleTxnLayerTimerExpiry
 **
 ** DESCRIPTION	:	Function that the application invokes so that the UATK
 **					can perform local retransmission of a SIP message. The
 **					UATK simply invokes the corresponding function of the
 **					SIP Stack. 
 **
 ** NOTE		:	The main need for this function arises since the handler
 **					routine passed as input to sdf_cbk_uaStartTimer() is 
 **					of a different protoytpe compared to the handler routine
 **					passed as input to fast_startTxnTimer().
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHandleTxnLayerTimerExpiry _ARGS_(\
	(Sdf_ty_timerType dTimerType, Sdf_ty_pvoid pContextInfo, \
	 Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION:		sdf_ntfy_uaTxnTimeout 
 **
 ** DESCRIPTION:	Invoked if a message that was sent through UDP with 
 **					retransmissions did not receive a matching response
 **					after the maximum retransmissions have been sent.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ntfy_uaTxnTimeout _ARGS_(\
	(Sdf_st_callObject *pObject, SipMessage *pSipMsg, \
	 en_SipTimerType dTimerType, Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:		sdf_ntfy_uaSendingMessageFailed
 **
 ** DESCRIPTION:	Notification function invoked by the application if
 **                 sending the SIP message on the network FAILS.
 **
 *****************************************************************************/
Sdf_ty_retVal  sdf_ntfy_uaSendingMessageFailed _ARGS_(\
	(Sdf_st_callObject *pObject, SipMessage *pSipMsg, Sdf_st_error *pErr));



/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaFetchSipTransaction
 **
 ** DESCRIPTION	:	Internal API to fetch a transaction structure from the
 **					SIP Stack transaction hash aggregation maintained within
 **					the UA Toolkit.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFetchSipTransaction _ARGS_((\
	Sdf_st_callObject *pCallObj, SipTxnKey *pSipTxnKey, \
	Sdf_ty_pvoid *ppTxnBuffer, Sdf_ty_s8bit dOpt, \
	Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaReleaseSipTransaction
 **
 ** DESCRIPTION	:	Internal API to release a transaction structure within 
 **					the SIP Stack transaction hash aggregation maintained by
 **					the UA Toolkit.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaReleaseSipTransaction _ARGS_((\
	Sdf_st_callObject *pCallObj, SipTxnKey *pInSipTxnKey, \
	Sdf_ty_pvoid *ppOutSipTxnKey, Sdf_ty_pvoid *ppTxnBuffer, \
	Sdf_ty_s8bit dOpt, Sdf_ty_pvoid pApplicationContext, Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaTxnHashCalc
 ** 
 ** DESCRIPTION:	Calculate the hash value for the hash table containing
 **					the SIP Stack transaction structures.
 **
 *****************************************************************************/
Sdf_ty_u32bit sdf_fn_uaTxnHashCalc _ARGS_((Sdf_ty_pvoid pData));


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaTxnKeyCompare
 ** 
 ** DESCRIPTION:	Compare the keys of different transaction structures to
 **					determine if they belong to the same call-leg.
 **
 *****************************************************************************/
Sdf_ty_u8bit  sdf_fn_uaTxnKeyCompare _ARGS_((Sdf_ty_pvoid pStoredKey,\
	 Sdf_ty_pvoid pGeneratedKey));

/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaUpdateTxnSockFd
 **
 ** DESCRIPTION:	This function updates the socket Fd of stack transaction and
 **					also UATK transaction 
 **		
 **	PARAMETERS:
 **	pTransaction(IN/OUT)		: Transaction Object to which the Socket info
 ** 								needs to be updated
 **	pOverlapTransInfo(IN/OUT)	: OverlaptransactionObject to which 
 **									to which the Socket info needs to be updated
 **	pSocket(IN)					: Socket with which the transactions has to be
 **								  updated. 
 **	pErr(OUT)					: Error structure in case the function fails			 
 **					
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateTxnSockFd _ARGS_((\
	Sdf_st_transaction *pTransaction,\
	Sdf_st_overlapTransInfo *pOverlapTransInfo,Sdf_st_socket *pSocket,Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION:		sdf_ivk_uaProcessTimeout 
 **
 ** DESCRIPTION:	This ivk will be invoked by the application in timeout
 **				    scenarios before removing the callObject from the aggregation.
 **					This ivk should take care of any extra handling required - 
 **					for example, posting of state change in case of DEP etc.
 **					This will currently handle processing only for DEP.
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaProcessTimeout(
 Sdf_st_callObject *pObject,
 Sdf_st_error *pError);


#endif /* End - ifdef SDF_TXN */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
#endif /* Prevent multiple inclusions */
