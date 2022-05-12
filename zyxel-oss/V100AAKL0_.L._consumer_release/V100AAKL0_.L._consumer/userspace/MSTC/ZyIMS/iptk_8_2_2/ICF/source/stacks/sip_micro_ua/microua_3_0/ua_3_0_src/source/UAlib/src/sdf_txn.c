/*****************************************************************************
 ** FUNCTION:
 **			This file handles wrapping of the transaction layer and
 **			other stack callbacks with sdf_cbk_uaXXX APIs. It also
 **			provides other APIs to handle the stack transaction structures.
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_txn.c
 **
 ** DESCRIPTION		: This file has implementation of the stack callbacks as 
 **					  well as other APIs related to handling of the transaction
 **					  layer	functionality.
 **  	
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			---------		------
 ** 21-May-02		Siddharth						Creation
 **					Toshniwal
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"


#include "sdf_common.h"
#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_struct.h"
#include "sdf_list.h"
#endif
#include "sdf.h"
#include "sdf_callbacks.h"
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_hash.h"
#include "sdf_txn.h"
#include "sdf_init.h"
#include "sdf_sessTimer.h"

#ifdef SDF_SUBSCRIPTION_LAYER
#include "sdf_subslayerstruct.h"
#include "sdf_subslayerapi.h"
#include "sdf_subslayertimer.h"
#include "sdf_subslayerinitfree.h"
#endif

#ifdef SDF_DEP
#include "sdf_deputil.h"
#endif
#ifdef SDF_TXN
/* ==========================================================================
 *						APIs to act on Stack txn structures
 * ========================================================================== */

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
Sdf_ty_retVal sdf_ivk_uaAbortCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 Sdf_ty_pvoid		pContext,
	 Sdf_st_error		*pError)
#else
	(pCallObj, pContext, pError)
	 Sdf_st_callObject	*pCallObj;
	 Sdf_ty_pvoid		pContext;
	 Sdf_st_error		*pError;
#endif
{
	Sdf_st_listIterator 	 dListIterator;
	SipTxnKey				 *pSipTxnKey	= Sdf_co_null;
	SipTxnInformn			dTxnInfo;
	en_SipTxnClass			dTxnClass;
	SipError 		    	error;

#ifdef SDF_SUBSCRIPTION_LAYER
  Sdf_st_uaSLSubscriptionObject *pTempSubsObj = Sdf_co_null;
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaAbortCall");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAbortCall(): Error variable passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAbortCall(): "
			"Call object passed is invalid", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Stop the no-answer timer if it is running still.
	 */
	if (sdf_fn_uaStopNoAnswerTimer(pCallObj, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAbortCall(): "
			"Failed to stop the \"No-answer\" timer.", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#ifdef SDF_SESSION_TIMER
	/*
	 * Stop the session timer if it is still running.
	 */
	if (pCallObj->pSessionTimer->pTimerId != Sdf_co_null)
	{
		if (sdf_fn_uaStopSessionTimer(\
			(Sdf_ty_pvoid *)&(pCallObj->pSessionTimer->pTimerId), \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAbortCall(): "
				"Failed to stop the session timer", pError);
#endif
			return Sdf_co_fail;
		}
	}
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
  /* Now remove the Subscription object from the CallObj */
  (void)sdf_listInitIterator(&(pCallObj->slSubsObjList), &dListIterator, 
                             pError);

  while (dListIterator.pCurrentElement != Sdf_co_null)
  {
    pTempSubsObj = (Sdf_st_uaSLSubscriptionObject *)\
                         (dListIterator.pCurrentElement->pData);

    /* Call AbortSubscription for each SubsObj */
    if (sdf_ivk_uaSLAbortSubscription(pCallObj, pTempSubsObj, pContext, 
                                      pError) == Sdf_co_fail)
      return Sdf_co_fail;

  } /* End of while */
#endif /* End of SDF_SUBSCRIPTION_LAYER */

	pSipTxnKey = pCallObj->pUacTransaction->pTxnKey;

	if ( pSipTxnKey == Sdf_co_null && 
			pCallObj->pUacTransaction->pLastSipMsg != Sdf_co_null )
	{
		if(sip_captureTxnInformn(pCallObj->pUacTransaction->pLastSipMsg,
				SIP_NULL, 0,SIP_NULL,SIP_NULL,0,&dTxnInfo) == SipFail)
           return Sdf_co_fail;
        
		if(sip_getTxnClass(&dTxnInfo,SipTxnMesgSent,&dTxnClass) == SipFail)
				return Sdf_co_fail;

		/*Here pTxnKey is the internal SipTxnKey variable*/	
		if(sip_txn_createTxnKey\
			(pCallObj->pUacTransaction->pLastSipMsg,dTxnClass,
			 &pSipTxnKey,&error) == SipFail)
				return Sdf_co_fail;
		pCallObj->pUacTransaction->pTxnKey = pSipTxnKey;
	}

	/*
	 * Abort the UAC Transaction.
	 */
	if (pCallObj->pUacTransaction->pTxnKey != Sdf_co_null)
	{
		/*
		 * If the UAC key contains method as "CANCEL", we must abort 
		 * the INVITE transaction separately here.
		 */
		if ( (Sdf_mc_strcmp(pCallObj->pUacTransaction->pTxnKey->pMethod, \
			"CANCEL") == 0) ||
		   (Sdf_mc_strcmp(pCallObj->pUacTransaction->pTxnKey->pMethod, \
			"BYE") == 0) )
		{
			/*
			 * First abort the CANCEL transaction.
			 */
			(void)sdf_fn_uaAbortTransaction(pCallObj, \
				pCallObj->pUacTransaction->pTxnKey, pContext, pError);
			/*
			 * Now abort the INVITE transaction.
			 */

			(void)sdf_fn_uaAbortTransaction(pCallObj, \
				pCallObj->pUacTransaction->pInvTxnKey, pContext, pError); 
		}
		else
		{
			(void)sdf_ivk_uaAbortTransaction(pCallObj, \
				pCallObj->pUacTransaction->dTxnId, pContext, pError); 
		}
	}

	/*
	 * Abort the UAS Transaction.
	 */
	if (pCallObj->pUasTransaction->pTxnKey != Sdf_co_null)
	{
		if ( (Sdf_mc_strcmp(pCallObj->pUasTransaction->pTxnKey->pMethod, \
				"CANCEL") == 0) ||
		   	(Sdf_mc_strcmp(pCallObj->pUasTransaction->pTxnKey->pMethod, \
				"BYE") == 0) )
		{
			
			/*
			 * First abort the CANCEL transaction.
			 */
			(void)sdf_fn_uaAbortTransaction(pCallObj, \
				pCallObj->pUasTransaction->pTxnKey, pContext, pError);

			/*
			 * Now abort the INVITE transaction.
			 */
			(void)sdf_fn_uaAbortTransaction(pCallObj, \
				pCallObj->pUasTransaction->pInvTxnKey, pContext, pError);
		}
		else
		{
			(void)sdf_ivk_uaAbortTransaction(pCallObj, \
				pCallObj->pUasTransaction->dTxnId, pContext, pError); 
		}

	}

	/*
	 * Abort all overlapping transactions. RPRs will be aborted
	 * when the INVITE transaction is aborted. Here, we have to
	 * remove PRACK/COMET and other regular transactions.
	 */
	(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo	 *pTemp = Sdf_co_null;

		/*
		 * Store the current element in a local variable.
		 */
		pTemp = (Sdf_st_overlapTransInfo *)\
			(dListIterator.pCurrentElement->pData);
		
		/*
		 * Get the next one here in case the current element is 
		 * going to get deleted.
		 */
		(void)sdf_listNext(&dListIterator, pError);

		if (pTemp->pOLTxnKey != Sdf_co_null)
		{
			(void)sdf_ivk_uaAbortTransaction(pCallObj, pTemp->dTxnId, pContext, \
									pError); 
		}
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exitting sdf_ivk_uaAbortCall");
	return Sdf_co_success;
}


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
Sdf_ty_retVal sdf_ivk_uaAbortTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 Sdf_ty_txnIdT		 dTxnId,
	 Sdf_ty_pvoid		pContext,
	 Sdf_st_error		*pError)
#else
	(pCallObj, dTxnId, pContext, pError)
	 Sdf_st_callObject	*pCallObj;
	 Sdf_ty_txnIdT		 dTxnId;
	 Sdf_ty_pvoid		pContext;
	 Sdf_st_error		*pError;
#endif
{
	SipTxnKey				 *pSipTxnKey	= Sdf_co_null;
	Sdf_st_overlapTransInfo	 *pTemp 		= Sdf_co_null;
	Sdf_ty_u32bit			 dIndex 		= 0;
	Sdf_ty_bool				 dIsUasTransaction	= Sdf_co_false;
	Sdf_ty_bool				 dIsUacTransaction	= Sdf_co_false;
	Sdf_ty_bool				 dIsOverlapTransaction = Sdf_co_false;
   	SipTxnInformn			dTxnInfo;
	en_SipTxnClass			dTxnClass;
	SipError 		    	error;

	
	(void)pContext;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_ivk_uaAbortTransaction");


	if (pCallObj->pUasTransaction->dTxnId == dTxnId)
	{
		pSipTxnKey = pCallObj->pUasTransaction->pTxnKey;
		dIsUasTransaction = Sdf_co_true;
	}
	else if (pCallObj->pUacTransaction->dTxnId == dTxnId)
	{
		pSipTxnKey = pCallObj->pUacTransaction->pTxnKey;
	      			
		if ( pSipTxnKey == Sdf_co_null && 
			pCallObj->pUacTransaction->pLastSipMsg != Sdf_co_null )
		{
			if(sip_captureTxnInformn(pCallObj->pUacTransaction->pLastSipMsg,
					SIP_NULL, 0,SIP_NULL,SIP_NULL,0,&dTxnInfo) == SipFail)
					return Sdf_co_fail;

			if(sip_getTxnClass(&dTxnInfo,SipTxnMesgSent,&dTxnClass) ==SipFail)
					return Sdf_co_fail;

			/*Here pTxnKey is the internal SipTxnKey variable*/	
			if(sip_txn_createTxnKey\
				(pCallObj->pUacTransaction->pLastSipMsg,dTxnClass,
				&pSipTxnKey,&error) == SipFail)
					return Sdf_co_fail;
			pCallObj->pUacTransaction->pTxnKey = pSipTxnKey;
		}
	     
		dIsUacTransaction = Sdf_co_true;
	}

	if ((dIsUasTransaction != Sdf_co_true) && (dIsUacTransaction != Sdf_co_true))
	{
		Sdf_st_listIterator 	 dListIterator;

		/*
		 * Not the UAS/UAC transaction. Look into overlap transactions.
		 */
		(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
			&dListIterator, pError);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			pTemp = (Sdf_st_overlapTransInfo *)\
				(dListIterator.pCurrentElement->pData);
			
			if (pTemp->dTxnId == dTxnId)
			{
				/*
				 * Found a matching regular/overlap transaction.
				 */
				if (pTemp->pOLTxnKey != Sdf_co_null)
					pSipTxnKey = pTemp->pOLTxnKey;
				else
					pSipTxnKey = pTemp->pRprTxnKey;
			
			
				if ( pSipTxnKey == Sdf_co_null && 
					pTemp->pLastSipMsg != Sdf_co_null )
				{
					if(sip_captureTxnInformn(pTemp->pLastSipMsg,
							SIP_NULL, 0,SIP_NULL,SIP_NULL,0,&dTxnInfo) == SipFail)
							return Sdf_co_fail;
					if(sip_getTxnClass(&dTxnInfo,SipTxnMesgSent,&dTxnClass) ==
									SipFail)
							return Sdf_co_fail;

					/*Here pTxnKey is the internal SipTxnKey variable*/	
					if(sip_txn_createTxnKey\
						(pTemp->pLastSipMsg,dTxnClass,
						 &pSipTxnKey,&error) == SipFail)
							return Sdf_co_fail;

					if (pTemp->pOLTxnKey != Sdf_co_null)
						pTemp->pOLTxnKey = pSipTxnKey;
					else
						pTemp->pRprTxnKey = pSipTxnKey;

				}
			
				dIsOverlapTransaction = Sdf_co_true;
				break;
			}
			dIndex++ ;
			(void)sdf_listNext(&dListIterator, pError);
		}

		if (dIsOverlapTransaction == Sdf_co_false)
		{
			/*
		 	* Neither UAC, UAS or overlapping transaction.
		 	*/
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noExistError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAbortTransaction(): Failed to "
				"find the transaction with the given txn id to abort.", pError);
#endif
			pError->errCode = Sdf_en_noExistError;
			return Sdf_co_fail;
		}
	}

	if ( (dIsUasTransaction == Sdf_co_true) ||
		 (dIsUacTransaction == Sdf_co_true) )
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0, &(pCallObj->dCallStateMutex), 0);
#endif

		/*
		 * Check if the UAC/UAS transaction has already 
		 * completed or is in the process of being cancelled.
		 * In such a case aborting the INVITE transaction is not 
		 * possible.
		 */
		if ( (pCallObj->pCallInfo->dState == Sdf_en_idle) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_callEstablished) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_inviteCancelled) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteCancelled) )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noExistError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAbortTransaction(): Transaction "
				"reached a stable state or cancellation in progress.", pError);
#endif
			pError->errCode = Sdf_en_noExistError;
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObj->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}
		
		if (pCallObj->pCommonInfo->dCallerCancelStatus == Sdf_en_cancelSent)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAbortTransaction(): Transaction "
				"cancellation in progress.", pError);
#endif
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(pCallObj->dCallStateMutex));
#endif
			return Sdf_co_fail;
		}

		if (pCallObj->pCallInfo->dState == Sdf_en_reInviteprovRespReceived)
		{
			/*
			 * Stop the no-answer timer.
			 */
			if (sdf_fn_uaStopNoAnswerTimer(pCallObj, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaAbortTransaction(): "
					"Failed to stop the \"No-answer\" timer.", pError);
#endif
#ifdef SDF_THREAD_SAFE
				sdf_fn_uaUnlockMutex(0, &(pCallObj->dCallStateMutex));
#endif
				pError->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
			}
		}
		
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pCallObj->dCallStateMutex));
#endif
	}

	/*
	 * Invoke the stack API to abort the transaction.
	 */
	if (sdf_fn_uaAbortTransaction(pCallObj, pSipTxnKey, pContext, \
				pError) == Sdf_co_fail)
	{
		if( pError->errCode != Sdf_en_txnNoExist)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaAbortTransaction(): "
				"Failed to abort the transaction.", pError);
#endif
			return Sdf_co_fail;
		}
	}

	/*
	 * Have aborted the transaction. Restore state back now.
	 */
	if ( (dIsUacTransaction == Sdf_co_true) ||
		 (dIsUasTransaction == Sdf_co_true) )
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaLockMutex(0, &(pCallObj->dCallStateMutex), 0);
#endif

		/*
		 * Check if its INVITE or BYE has to be aborted.
		 */
		if ( (pCallObj->pCallInfo->dState == Sdf_en_reInviteSent) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteReceived)||
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteprovRespSent) || 
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteprovRespReceived) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteFinalResponseSent) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_reInviteFinalResponseReceived) )
		{
			pCallObj->pCallInfo->dState = Sdf_en_callEstablished;
			pCallObj->pMediaInfo->dMediaState = Sdf_en_establishedMedia;

		}
		else if ( (pCallObj->pCallInfo->dState == Sdf_en_inviteSent) ||
	 		(pCallObj->pCallInfo->dState == Sdf_en_inviteReceived) ||
		 	(pCallObj->pCallInfo->dState == Sdf_en_provisionalRespReceived) ||
			(pCallObj->pCallInfo->dState == Sdf_en_provisionalRespSent) ||
			(pCallObj->pCallInfo->dState == Sdf_en_finalSuccessSent) ||
			(pCallObj->pCallInfo->dState == Sdf_en_finalSuccessReceived) ||
			(pCallObj->pCallInfo->dState == Sdf_en_finalFailureSent) ||
			(pCallObj->pCallInfo->dState == Sdf_en_finalFailureReceived) )
		{
			pCallObj->pCallInfo->dState = Sdf_en_idle;
			pCallObj->pMediaInfo->dMediaState = Sdf_en_idleMedia;

		}
		else if ( (pCallObj->pCallInfo->dState == Sdf_en_byeSent) ||
			 (pCallObj->pCallInfo->dState == Sdf_en_byeReceived) )
		{
			pCallObj->pCallInfo->dState = Sdf_en_callEstablished;
			pCallObj->pMediaInfo->dMediaState = Sdf_en_establishedMedia;

		}

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(pCallObj->dCallStateMutex));
#endif
	}
	else
	{
		/*Media State Should be restored*/
		if((pCallObj->pMediaInfo->dMediaState == Sdf_en_offerSent)||\
			(pCallObj->pMediaInfo->dMediaState == Sdf_en_offerRecv))
		{
			pCallObj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
		}
		else if((pCallObj->pMediaInfo->dMediaState == Sdf_en_offerChangeRecv)||\
			(pCallObj->pMediaInfo->dMediaState == Sdf_en_offerChangeSent))
		{
			pCallObj->pMediaInfo->dMediaState = Sdf_en_establishedMedia;
		}

		/*
		 * Regular transaction getting aborted. Destroy the overlap
		 * transaction structure here. No state to be reverted.
		 */
		(void)sdf_listDeleteAt(&(pCallObj->slOverlapTransInfo), dIndex, pError);
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaAbortTransaction");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaAbortTransaction
 ** 
 ** DESCRIPTION:	Internal API to invoke the stack and abort the SIP
 **					transaction.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAbortTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 SipTxnKey			*pSipTxnKey,
	 Sdf_ty_pvoid		pContext,
	 Sdf_st_error		*pError)
#else
	(pCallObj, pSipTxnKey, pContext, pError)
	 Sdf_st_callObject	*pCallObj;
	 SipTxnKey			*pSipTxnKey,
	 Sdf_ty_pvoid		pContext;
	 Sdf_st_error		*pError;
#endif
{
	Sdf_st_txnContextData	 *pTxnContextData	= Sdf_co_null;
	SipTxnContext			 *pTxnContext	= Sdf_co_null;
	Sdf_st_eventContext		 *pEventContext	= Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaAbortTransaction");

	/*
	 * Create the transaction context to give to the abort
	 * transaction API.
	 */
	if (sip_txn_initSipTxnContext(&pTxnContext, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAbortTransaction(): "
			"Failed to initialize the transaction context "
			"structure.",pError);
#endif
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	pTxnContext->dTxnType = SipUATypeTxn;

	if (sdf_ivk_uaInitTxnContextData(&pTxnContextData, \
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAbortTransaction(): "
			"Failed to initialize the transaction context "
			"data.", pError);
#endif
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pTxnContext, pError);
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	HSS_LOCKEDINCREF(pCallObj->dRefCount);
	pTxnContextData->pRefCallObject		= pCallObj;
	pTxnContextData->pApplicationData	= pContext;

	if (sdf_ivk_uaInitEventContext(&pEventContext, \
		pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAbortTransaction(): "
			"Failed to initialize the transaction context "
			"data.", pError);
#endif
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pTxnContext, pError);
		(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);
		pError->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	
	pTxnContext->pEventContext	= (SipEventContext *)pEventContext;
	pEventContext->pData		= (Sdf_ty_pvoid) pTxnContextData;

	/*
	 * Release the stack transaction structure & stop retransmissions.
	 */
	if (sip_txn_abortTxn(pSipTxnKey, pTxnContext, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		/*
		 * Release context in failure case.
		 */
		pEventContext->pData = Sdf_co_null;
		(void)sdf_ivk_uaFreeEventContext(pEventContext);
		(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);

		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)&pTxnContext, pError);

		pError->errCode = Sdf_en_txnNoExist;
		return Sdf_co_fail;
	}
	
	/*
	 * Release context data structures now.
	 */
	pEventContext->pData = Sdf_co_null;
	(void)sdf_ivk_uaFreeEventContext(pEventContext);
	(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&pTxnContext, pError);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaAbortTransaction");
	return Sdf_co_success;
}


/* ==========================================================================
 *						Stack callback implementations
 * ========================================================================== */

/*****************************************************************************
 ** FUNCTION	:	sip_cbk_fetchTxn
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
SipBool sip_cbk_fetchTxn
#ifdef ANSI_PROTO
	(SIP_Pvoid* ppTxnBuffer,
	 SIP_Pvoid pTxnKey,
	 SIP_Pvoid pContext,
	 SIP_S8bit dOpt,
	 SipError *pErr)
#else
	(ppTxnBuffer, pTxnKey, pContext, dOpt, pErr)
	SIP_Pvoid* ppTxnBuffer;
	SIP_Pvoid pTxnKey;
	SIP_Pvoid pContext;
	SIP_S8bit dOpt;
	SipError *pErr;
#endif
{
	Sdf_st_txnContextData	*pContextData	= (Sdf_st_txnContextData *)pContext;
	Sdf_st_callObject		*pCallObj		= pContextData->pRefCallObject;
	SipTxnKey				*pSipTxnKey		= (SipTxnKey *) pTxnKey;
	Sdf_st_error			 dError;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sip_cbk_fetchTxn");
	
	/*
	 * If the application wants to over-ride the default implementation
	 * for the SIP Stack transaction aggregation, it can do so here.
	 */
	if (pGlbToolkitData->pTxnFetchFunc != Sdf_co_null)
	{
		/*
		 * TODO : Callback traces.
		 */
		if (pGlbToolkitData->pTxnFetchFunc(pCallObj, pSipTxnKey, \
			ppTxnBuffer, dOpt, pContextData->pApplicationData, \
			&dError) == Sdf_co_fail)
		{
			*pErr = (SipError) dError.errCode;
			return SipFail;
		}
	}
	else
	{
		/*
		 * Go ahead with the UA Toolkit maintaining the stack data
		 * structures.
		 */
		if (sdf_fn_uaFetchSipTransaction(pCallObj, pSipTxnKey, ppTxnBuffer, \
			dOpt, pContextData->pApplicationData, &dError) == Sdf_co_fail)
		{
			*pErr = (SipError) dError.errCode;
			return SipFail;
		}
	}
	
	*pErr = E_NO_ERROR;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sip_cbk_fetchTxn");

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION	:	sip_cbk_releaseTxn
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
SipBool sip_cbk_releaseTxn
#ifdef ANSI_PROTO
	(SIP_Pvoid 	 pInTxnKey, 
	 SIP_Pvoid  *ppOutSipTxnKey,
	 SIP_Pvoid  *ppTxnBuffer, 
	 SIP_Pvoid	 pContextData,
	 SIP_S8bit   dOpt, 
	 SipError	*pErr)
#else
	(pInTxnKey, ppOutTxnKey, ppTxnBuffer, pContextData, dOpt, pErr)
	 SIP_Pvoid	 pInTxnKey; 
	 SIP_Pvoid	*ppOutTxnKey;
	 SIP_Pvoid	*ppTxnBuffer; 
	 SIP_Pvoid	 pContextData;
	 SIP_S8bit	 dOpt; 
	 SipError	*pErr;
#endif
{
	Sdf_st_txnContextData *pTxnContextData	= \
		(Sdf_st_txnContextData *)pContextData;

	Sdf_st_callObject	  *pCallObj			= pTxnContextData->pRefCallObject;
	SipTxnKey			  *pInSipTxnKey		= (SipTxnKey *) pInTxnKey;
	Sdf_st_error		   dError;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sip_cbk_releaseTxn");

	if (pGlbToolkitData->pTxnReleaseFunc != Sdf_co_null)
	{
		if (pGlbToolkitData->pTxnReleaseFunc(pCallObj, pInSipTxnKey, \
			ppOutSipTxnKey, ppTxnBuffer, dOpt, \
			pTxnContextData->pApplicationData, &dError) == Sdf_co_fail)
		{
			*pErr = (SipError) dError.errCode;
			return SipFail;
		}
	}
	else
	{
		if (sdf_fn_uaReleaseSipTransaction(pCallObj, pInSipTxnKey, \
			ppOutSipTxnKey, ppTxnBuffer, dOpt, \
			pTxnContextData->pApplicationData, &dError) == Sdf_co_fail)
		{
			*pErr = (SipError) dError.errCode;
			return SipFail;
		}
	}

	*pErr = E_NO_ERROR;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sip_cbk_releaseTxn");

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION	:	fast_startTxnTimer
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
SipBool fast_startTxnTimer
#ifdef ANSI_PROTO
	(SIP_U32bit		 dDuration,
	 TimeoutFuncPtr	 timeOutFuncPtr, 
	 SIP_Pvoid		 pData, 
	 SIP_Pvoid*		 ppHandle,
	 SipError		*pErr)
#else
	(dDuration, timeOutFuncPtr, pData, ppHandle, pErr)
	 SIP_U32bit		 dDuration;
	 TimeoutFuncPtr  timeOutFuncPtr;
	 SIP_Pvoid		 pData; 
	 SIP_Pvoid		*ppHandle;
	 SipError		*pErr;
#endif
{
	Sdf_st_error			 dError;
	Sdf_st_txnContextData	*pTxnContextData = (Sdf_st_txnContextData *) \
		( (SipTxnTimeoutData *)pData )->pContextData;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering fast_startTxnTimer");
	
	/*
	 * Store the function passed by the stack to process the timeout.
	 */
	pTxnContextData->pTimeOutFuncPtr = timeOutFuncPtr;
	
#ifdef SDF_LINT
    if (sdf_cbk_uaStartTimer(dDuration, Sdf_en_txnLayerTimer, \
		                     pData, pTxnContextData->pApplicationData, \
		                     sdf_ivk_uaHandleTxnLayerTimerExpiry, ppHandle, \
		                     &dError) == Sdf_co_fail)
#else    
    if (sdf_cbk_uaStartTimer(dDuration, Sdf_en_txnLayerTimer, \
		pData, pTxnContextData->pApplicationData, \
		&sdf_ivk_uaHandleTxnLayerTimerExpiry, ppHandle, \
		&dError) == Sdf_co_fail)
#endif        
	{
		/* 
		 * TODO : Check if this is errCode only or should this be appCode ? 
		 */

		*pErr = (SipError) dError.errCode;
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting fast_startTxnTimer");

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION	:	fast_stopTxnTimer
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
SipBool fast_stopTxnTimer
#ifdef ANSI_PROTO
	(SIP_Pvoid	 pInkey,
	 SIP_Pvoid	*ppBuff,
	 SipError	*pErr)
#else
	(pInkey, ppBuff, pErr)
	 SIP_Pvoid	 pInkey;
	 SIP_Pvoid	*ppBuff;
	 SipError	*pErr;
#endif
{
	Sdf_st_error	dError;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering fast_stopTxnTimer");

	if (sdf_cbk_uaStopTimer(pInkey, ppBuff, &dError) == Sdf_co_fail)
	{
		*pErr = (SipError) dError.errCode;
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting fast_stopTxnTimer");

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION	:	sip_freeTimerHandle
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
void sip_freeTimerHandle
#ifdef ANSI_PROTO
	(SIP_Pvoid 	pTimerHandle)
#else
	(pTimerHandle)
	 SIP_Pvoid	pTimerHandle;
#endif
{
	sdf_cbk_uaFreeTimerHandle(pTimerHandle);
	return;
}


/* =========================================================================
 *			MISC UA-T TXN LAYER RELATED INTERNAL APIs
 * ========================================================================= */

/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaFetchSipTransaction
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFetchSipTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 SipTxnKey			*pSipTxnKey,
	 Sdf_ty_pvoid		*ppTxnBuffer, 
	 Sdf_ty_s8bit		dOpt, 
	 Sdf_ty_pvoid		pApplicationContext,
	 Sdf_st_error		*pErr)
#else
	(pCallObj, pSipTxnKey, ppTxnBuffer, dOpt, pApplicationContext, pErr)
	 Sdf_st_callObject  *pCallObj;
	 SipTxnKey			*pSipTxnKey;
	 Sdf_ty_pvoid		*ppTxnBuffer; 
	 Sdf_ty_s8bit		dOpt; 
	 Sdf_ty_pvoid		pApplicationContext;
	 Sdf_st_error		*pErr;
#endif
{
	(void)pCallObj;
	(void)pApplicationContext;

	if (sdf_ivk_uaHashExclFetch(&pGlbToolkitData->dSipTxnHash, pSipTxnKey, \
		ppTxnBuffer, dOpt) == Sdf_co_fail)
	{
		pErr->errCode = Sdf_en_noExistError;
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaReleaseSipTransaction
 **
 ** DESCRIPTION	:	
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaReleaseSipTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject  *pCallObj,
	 SipTxnKey			*pInSipTxnKey, 
	 Sdf_ty_pvoid		*ppOutSipTxnKey, 
	 Sdf_ty_pvoid		*ppTxnBuffer, 
	 Sdf_ty_s8bit		dOpt, 
	 Sdf_ty_pvoid		pApplicationContext,
	 Sdf_st_error		*pErr)
#else
	(pCallObj, pInSipTxnKey, ppOutSipTxnKey, ppTxnBuffer, 
	 dOpt, pApplicationContext, pError)
	 Sdf_st_callObject	*pCallObj;
	 SipTxnKey			*pInSipTxnKey; 
	 Sdf_ty_pvoid		*ppOutSipTxnKey; 
	 Sdf_ty_pvoid		*ppTxnBuffer; 
	 Sdf_ty_s8bit		dOpt; 
	 Sdf_ty_pvoid		pApplicationContext;
	 Sdf_st_error		*pErr;
#endif
{
	(void)pCallObj;
	(void)pApplicationContext;

	sdf_ivk_uaHashExclRelease(&(pGlbToolkitData->dSipTxnHash), \
		pInSipTxnKey, dOpt, ppOutSipTxnKey, ppTxnBuffer);

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaTxnHashCalc
 ** 
 ** DESCRIPTION:	Calculate the hash value for the hash table containing
 **					the SIP Stack transaction structures.
 **
 *****************************************************************************/
Sdf_ty_u32bit sdf_fn_uaTxnHashCalc 
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	pData)
#else
	(pData)
	Sdf_ty_pvoid	pData;
#endif
{
	SipTxnKey		*pTxnKey = (SipTxnKey *) pData;
	Sdf_ty_u32bit	 dHash	 = 0;

	dHash = sdf_ivk_uaElfHash( (Sdf_ty_pvoid)(pTxnKey->pCallid) );

	return dHash;
}


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaTxnKeyCompare
 ** 
 ** DESCRIPTION:	Compare the keys of different transaction structures to
 **					determine if they belong to the same call-leg.
 **
 *****************************************************************************/
Sdf_ty_u8bit  sdf_fn_uaTxnKeyCompare
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	pStoredKey,
	 Sdf_ty_pvoid	pGeneratedKey)
#else
	(pStoredKey, pGeneratedKey)
	 Sdf_ty_pvoid	pStoredKey;
	 Sdf_ty_pvoid	pGeneratedKey;
#endif
{
	Sdf_st_error	dError;
	SipBool			dResult;
	
	dResult = sip_txn_compareTxnKeys((SipTxnKey *)pStoredKey, \
		(SipTxnKey *)pGeneratedKey, (SipError *)&(dError.stkErrCode));

	return ((dResult == SipSuccess) ? (Sdf_ty_u32bit)0 : (Sdf_ty_u32bit)1);
}


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
Sdf_ty_retVal sdf_ivk_uaHandleTxnLayerTimerExpiry
#ifdef ANSI_PROTO
	(Sdf_ty_timerType	 dTimerType,
	 Sdf_ty_pvoid		 pContextInfo,
	 Sdf_st_error		*pErr)
#else
	(dTimerType, pContextInfo, pErr)
	 Sdf_ty_timerType	dTimerType;
	 Sdf_ty_pvoid		pContextInfo;
	 Sdf_st_error		*pErr;
#endif
{
	Sdf_st_txnContextData	*pTxnContextData = Sdf_co_null;
	TimeoutFuncPtr			 pTempFunc;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_ivk_uaHandleTxnLayerTimerExpiry");

	if (dTimerType != Sdf_en_txnLayerTimer)
	{
		/* TODO : New error code needed ? */
		return Sdf_co_fail;
	}
	
	/*
	 * Fetch the handler function of the stack that will process this
	 * timer's expiry.
	 */
	pTxnContextData = (Sdf_st_txnContextData *) \
		( (SipTxnTimeoutData *)pContextInfo )->pContextData;
	
	/*
	 * Invoke the stack to handle the timer expiry event.
	 */
	pTempFunc = pTxnContextData->pTimeOutFuncPtr;
	if(Sdf_co_null != pTempFunc)
	{
        (void)pTempFunc(pContextInfo);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaHandleTxnLayerTimerExpiry");

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaHandleNoAnswerTimerExpiry
 **
 ** DESCRIPTION	:	Function that the application invokes so that the UATK
 **					can inform the application of a "call-setup" timer 
 **					getting expired.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleNoAnswerTimerExpiry
#ifdef ANSI_PROTO
	(Sdf_ty_timerType	 dTimerType,
	 Sdf_ty_pvoid		 pContextInfo,
	 Sdf_st_error		*pErr)
#else
	(dTimerType, pContextInfo, pErr)
	 Sdf_ty_timerType	dTimerType;
	 Sdf_ty_pvoid		pContextInfo;
	 Sdf_st_error		*pErr;
#endif
{
	Sdf_st_noAnswerTimerContext	*pTimerContext = \
		(Sdf_st_noAnswerTimerContext *)pContextInfo;
	Sdf_ty_state	dCallState;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaHandleNoAnswerTimerExpiry");


#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaHandleNoAnswerTimerExpiry(): "
			"Error variable passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if (pContextInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError (Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaHandleNoAnswerTimerExpiry(): "
			"Context information passed is invalid", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	
	if (dTimerType != Sdf_en_noAnswerTimer)
		return Sdf_co_fail;

	if (pContextInfo == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	/*
	 * Free the timer handle now.
	 */
	sdf_cbk_uaFreeTimerHandle(\
		pTimerContext->pRefCallObject->pCommonInfo->pNoAnswerTimerHandle);
	pTimerContext->pRefCallObject->pCommonInfo->pNoAnswerTimerHandle \
		= Sdf_co_null;
	
	/*
	 * Retrieve the state of the call first.
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &(pTimerContext->pRefCallObject->dCallStateMutex), 0);
#endif
	dCallState = pTimerContext->pRefCallObject->pCallInfo->dState;	
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pTimerContext->pRefCallObject->dCallStateMutex));
#endif

	if (dCallState == Sdf_en_callTerminated)
	{
		/*
		 * The caller cancelled the INVITE with a BYE after getting the
		 * first provisional response. This call needs to be destroyed now.
		 */
#ifdef SDF_THREAD_SAFE
		/*w534*/(void)sdf_ivk_uaLockCallObject(pTimerContext->pRefCallObject, pErr);
#endif
		/*w534*/(void)sdf_ivk_uaAbortCall(pTimerContext->pRefCallObject,	\
				(Sdf_ty_pvoid)(pTimerContext->pEventContext->pData), pErr);
#ifdef SDF_THREAD_SAFE
		/*w534*/(void)sdf_ivk_uaUnlockCallObject(pTimerContext->pRefCallObject, pErr);
#endif

		/*
		 * Release resources. Call object will get freed as a part of 
		 * freeing the timer context structure.
		 */
		(void)sdf_fn_uaFreeNoAnswerTimerContext(pTimerContext);
	}
	else
	{
		pTimerContext->pRefCallObject->pCommonInfo->dIsTimeout = Sdf_co_true;
		
		/*w534*/(void)sdf_cbk_uaIndicateTxnTimeOut(pTimerContext->pRefCallObject, \
			Sdf_co_null, Sdf_en_timerNoAnswer, \
			(Sdf_ty_pvoid)(pTimerContext->pEventContext->pData), pErr);

		(void)sdf_fn_uaFreeNoAnswerTimerContext(pTimerContext);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaHandleNoAnswerTimerExpiry");

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaStartNoAnswerTimer
 **
 ** DESCRIPTION	:	Function that the UA Toolkit invokes within 
 **					sdf_ivk_uaUpdateCallDetails to start a timer upon 
 **					receipt of a provisional response.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStartNoAnswerTimer
#ifdef ANSI_PROTO
	(Sdf_st_callObject		*pCallObj,
	 Sdf_st_eventContext	*pEventContext,
	 Sdf_ty_bool			 dReset,
	 Sdf_st_error			*pErr)
#else
	(pCallObj, pEventContext, dReset, pErr)
	 Sdf_st_callObject		*pCallObj;
	 Sdf_st_eventContext	*pEventContext;
	 Sdf_ty_bool			 dReset;
	 Sdf_st_error			*pErr;
#endif
{
	Sdf_st_noAnswerTimerContext	*pNoAnswerTimerContext = Sdf_co_null;
	Sdf_ty_u32bit	dDuration = 0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaStartNoAnswerTimer");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaStartNoAnswerTimer(): "
			"Error variable passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): "
			"Call object passed is invalid", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): "
			"Event context passed is invalid", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	
	/*
	 * Stop the previously running timer if 'reset' is set to true.
	 */
	if (dReset == Sdf_co_true)
	{
		if (sdf_cbk_uaStopTimer(pCallObj->pCommonInfo->pNoAnswerTimerHandle, \
			(Sdf_ty_pvoid *)&pNoAnswerTimerContext, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_timerDoesNotExist, \
				(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): "
				"Failed to stop the existing \"No-answer\" timer", pErr);
#endif
			pErr->errCode = Sdf_en_timerDoesNotExist;
			return Sdf_co_fail;
		}

		if (pNoAnswerTimerContext == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): "
				"Event context returned by the application is NULL.", pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		/*
		 * Release the earlier event-context and the earlier timer handle.
		 */
		(void)sdf_ivk_uaFreeEventContext(pNoAnswerTimerContext->pEventContext);
		sdf_cbk_uaFreeTimerHandle(pCallObj->pCommonInfo->pNoAnswerTimerHandle);
		pCallObj->pCommonInfo->pNoAnswerTimerHandle = Sdf_co_null;
	}
	else
	{
		/*
		 * Starting a new timer. So init a new timer context.
		 */
		if (sdf_fn_uaInitNoAnswerTimerContext(&pNoAnswerTimerContext, \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): Failed"
				"to init context structure for \"No-answer\" timer", pErr);
#endif
			pErr->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		pNoAnswerTimerContext->pRefCallObject = pCallObj;
		HSS_LOCKEDINCREF(pCallObj->dRefCount);
	}

	/*
	 * Replace the event context (if it earlier contained one) in 
	 * the timer context now.
	 */
	pNoAnswerTimerContext->pEventContext = pEventContext;
	HSS_LOCKEDINCREF(pEventContext->dRefCount);

	/*
	 * Set the duration appropriately.
	 */
	if (pEventContext->dNoAnswerTimerDuration != 0)
		dDuration = pEventContext->dNoAnswerTimerDuration;
	else
		dDuration = pGlbToolkitData->dNoAnswerTimerDuration;
	
	/*
	 * Start the new timer now.
	 */
#ifdef SDF_LINT
	if (sdf_cbk_uaStartTimer(dDuration, Sdf_en_noAnswerTimer, \
		                     (Sdf_ty_pvoid)pNoAnswerTimerContext,pEventContext->pData,\
		                     sdf_fn_uaHandleNoAnswerTimerExpiry, \
		                     &(pCallObj->pCommonInfo->pNoAnswerTimerHandle), \
		                     pErr) == Sdf_co_fail)
#else    
	if (sdf_cbk_uaStartTimer(dDuration, Sdf_en_noAnswerTimer, \
		(Sdf_ty_pvoid)pNoAnswerTimerContext, pEventContext->pData, \
		&sdf_fn_uaHandleNoAnswerTimerExpiry, \
		&(pCallObj->pCommonInfo->pNoAnswerTimerHandle), \
		pErr) == Sdf_co_fail)
#endif        
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_fn_uaStartNoAnswerTimer(): Failed"
				"to start the \"No-answer\" timer", pErr);
#endif
		pErr->errCode = Sdf_en_noMemoryError;
		(void)sdf_fn_uaFreeNoAnswerTimerContext(pNoAnswerTimerContext);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaStartNoAnswerTimer");

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION	:	sdf_fn_uaStopNoAnswerTimer
 **
 ** DESCRIPTION	:	Function that the UA Toolkit invokes within 
 **					sdf_ivk_uaUpdateCallDetails to stop the timer that was
 **					started upon receipt of the last provisional response.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaStopNoAnswerTimer
#ifdef ANSI_PROTO
	(Sdf_st_callObject		*pCallObj,
	 Sdf_st_error			*pErr)
#else
	(pCallObj, pErr)
	 Sdf_st_callObject		*pCallObj;
	 Sdf_st_error			*pErr;
#endif
{
	Sdf_st_noAnswerTimerContext	*pNoAnswerTimerContext = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaStopNoAnswerTimer");

	if (pCallObj->pCommonInfo->pNoAnswerTimerHandle == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_fn_uaStopNoAnswerTimer");
		return Sdf_co_success;
	}
	
	/*
	 * Stop the timer by invoking the application.
	 */
	if (sdf_cbk_uaStopTimer(pCallObj->pCommonInfo->pNoAnswerTimerHandle, \
		(Sdf_ty_pvoid *)&pNoAnswerTimerContext, pErr) == Sdf_co_fail)
	{
		return Sdf_co_fail;
	}

	/*
	 * Free resources that had been stored in the 'context' information
	 * for the timer.
	 */
	if (pNoAnswerTimerContext == Sdf_co_null)
	{
		return Sdf_co_fail;
	}
	else
	{
		(void)sdf_fn_uaFreeNoAnswerTimerContext(pNoAnswerTimerContext);
		sdf_cbk_uaFreeTimerHandle(pCallObj->pCommonInfo->pNoAnswerTimerHandle);
		pCallObj->pCommonInfo->pNoAnswerTimerHandle = Sdf_co_null;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_fn_uaStopNoAnswerTimer");

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	 sip_sendToNetwork
 **
 ** DESCRIPTION: A callback invoked by the stack when a message is to be 
 **				 sent to the network. A call to sip_sendMessage() results 
 **				 in this callback if the message is encoded successfully.
 **
 *****************************************************************************/
SipBool sip_sendToNetwork
#ifdef ANSI_PROTO
	(SIP_S8bit		*pBuffer,
	SIP_U32bit		 dBuflen,
	SipTranspAddr	*pTranspaddr,
	SIP_S8bit		 dTransptype,
	SipError		*pErr) 
#else
	(pBuffer, dBuflen, pTranspaddr, dTransptype, pErr)
	SIP_S8bit		*pBuffer;
	SIP_U32bit		 dBuflen;
	SipTranspAddr	*pTranspaddr;
	SIP_S8bit		 dTransptype;
	SipError		*pErr;
#endif
{
	Sdf_st_error		 dError;
	Sdf_st_callObject	*pCallObj	= Sdf_co_null;
	Sdf_ty_protocol		 dProtocol 	= Sdf_en_protoUdp;
	Sdf_st_txnContextData	*pTxnContext = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sip_sendToNetwork");

	if (pTranspaddr->pData != Sdf_co_null)
	{
		pTxnContext = (Sdf_st_txnContextData *)pTranspaddr->pData;

		if (pTxnContext->pRefCallObject != Sdf_co_null)
		{
			pCallObj = pTxnContext->pRefCallObject;
		}
		else
			return SipFail;
	}
	else
		return SipFail;

	if ((dTransptype & SIP_TCP) == SIP_TCP)
		dProtocol = Sdf_en_protoTcp;
#ifdef SDF_TLS
	else if ((dTransptype & SIP_TLS) == SIP_TLS)
		dProtocol = Sdf_en_protoTls;
#endif
	else
		dProtocol = Sdf_en_protoUdp;
	
	if (sdf_cbk_uaSendMsgOnNetwork(pCallObj, pBuffer, dBuflen, \
		pTranspaddr, dProtocol, &dError) == Sdf_co_fail)
	{
		/* As Stack tries to free the memory based on returned error code */ 
		*pErr = E_TXN_NETWORK_SEND_ERR;
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sip_sendToNetwork");

	return SipSuccess;
}


/*****************************************************************************
 ** FUNCTION:		sip_indicateTimeout 
 **
 ** DESCRIPTION:	Callback function to be implemented by the application.
 **					Invoked if a message that was sent through UDP with 
 **					retransmissions did not receive a matching response
 **					after the maximum retransmissions have been sent.
 **
 *****************************************************************************/
void sip_indicateTimeOut 
#ifdef ANSI_PROTO
	(SipEventContext *context, en_SipTimerType dTimerType)
#else
	(context, dTimerType)
	SipEventContext *context;
	en_SipTimerType dTimerType;
#endif
{
	Sdf_st_txnContextData	*pContextData	= \
		(Sdf_st_txnContextData *)context->pData;
	Sdf_st_callObject		*pCallObj		= pContextData->pRefCallObject;
	Sdf_st_overlapTransInfo	*pOverlapTxn	= Sdf_co_null;
	Sdf_st_error			 dError;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sip_indicateTimeOut");

	if ( (dTimerType == SipTimerD) || (dTimerType == SipTimerI) ||
		 (dTimerType == SipTimerJ) || (dTimerType == SipTimerK) )
	{
		/*
		 * CSM moving from "Completed" to "Terminated". This is normal.
		 * Ignore these cases.
		 */
		sip_freeEventContext(context);
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sip_indicateTimeOut");
		return;
	}
	
	/* Set the Timeout Flag in Transaction to True */
	pCallObj->pCommonInfo->dIsTimeout = Sdf_co_true;

	/* 
	 * Return the overlapping transaction structure to the 
	 * application if a regular/overlapping transaction times
	 * out.
	 */
	if ( (pContextData->dType == Sdf_en_overlapTransaction) ||
		 (pContextData->dType == Sdf_en_RegularUasTransaction) ||
		 (pContextData->dType == Sdf_en_RegularUacTransaction) )
	{
		pOverlapTxn = (Sdf_st_overlapTransInfo *)pContextData->pTransaction;
	}

#ifdef SDF_SUBSCRIPTION_LAYER
  /* If it's a message for the SubsLayer (check for SubsObj in CtxtData) then
   * throw cbk for SubsLayer otherwise, throw the general callback.
   */
  if ((Sdf_st_uaSLSubscriptionObject *)pContextData->pSubsObj != Sdf_co_null)
  {
    (void)sdf_fn_uaSLHandleSubsNtfyTxnTimer(pCallObj, pOverlapTxn, 
          (Sdf_st_uaSLSubscriptionObject *)pContextData->pSubsObj, 
          (Sdf_ty_txnTimerType)dTimerType, 
          (Sdf_ty_pvoid)(pContextData->pApplicationData), &dError);
  }
  else
  {
	  (void)sdf_cbk_uaIndicateTxnTimeOut (pCallObj, pOverlapTxn, \
	      	(Sdf_ty_txnTimerType)dTimerType, \
      		(Sdf_ty_pvoid)(pContextData->pApplicationData), &dError);
  }
#else
	(void)sdf_cbk_uaIndicateTxnTimeOut (pCallObj, pOverlapTxn, \
		(Sdf_ty_txnTimerType)dTimerType, \
		(Sdf_ty_pvoid)(pContextData->pApplicationData), &dError);
#endif

         /*CSR_1-4747136_FIX Starts */
	/*if(pCallObj->pCommonInfo->dIsTimeout == Sdf_co_true)
		pCallObj->pCommonInfo->dIsTimeout = Sdf_co_false;*/

        /*CSR_1-4747136_FIX Ends*/

#ifndef SDF_LOOKUP
	if( pOverlapTxn != Sdf_co_null )
		(void)sdf_ivk_uaRemoveOverlapInfo(pCallObj, pOverlapTxn);
#endif

	/*
	 * Free the event context now.
	 */
	sip_freeEventContext(context);
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sip_indicateTimeOut");
	return;
}


/*****************************************************************************
 ** FUNCTION:		sdf_ntfy_uaSendingMessageFailed
 **
 ** DESCRIPTION:	Notification function invoked by the application if
 **                 sending the SIP message on the network FAILS.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ntfy_uaSendingMessageFailed
#ifdef ANSI_PROTO
	(Sdf_st_callObject       *pObject,
	 SipMessage              *pMessage, 
	 Sdf_st_error            *pError)
#else
	(pObject,pMessage,pError)
	 Sdf_st_callObject       *pObject;
	 SipMessage              *pMessage; 
	 Sdf_st_error            *pError;
#endif
{
	Sdf_st_overlapTransInfo *pOverlapTransaction = Sdf_co_null;
	Sdf_st_transaction      *pTransaction = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "\
			           "sdf_ntfy_uaSendingMessageFailed");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ntfy_uaSendingMessageFailed( ): Invalid Error param");
#endif
		return Sdf_co_fail;
	}	
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ntfy_uaSendingMessageFailed( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ntfy_uaSendingMessageFailed( ): "
			" SipMessage parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/*
     * This API is invoked to fetch the Overlap Transaction.
	 */
	if (sdf_ivk_uaGetTransactionForSipMsg(pObject, pMessage, &pTransaction,\
				&pOverlapTransaction, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
		    (Sdf_ty_s8bit *)"sdf_ntfy_uaSendingMessageFailed(): "
		    "Failed to get the transaction from SipMessage.", pError);
#endif
	    return Sdf_co_fail;		
    }

	if( pOverlapTransaction != Sdf_co_null )
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
{
   (void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransaction);
#endif		
        /*w534*/(void)sdf_ivk_uaRemoveOverlapInfo(pObject, pOverlapTransaction);
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
}
#endif	
    else
		(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		               "sdf_ntfy_uaSendingMessageFailed");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:		sdf_ntfy_uaTxnTimeout 
 **
 ** DESCRIPTION:	Invoked if a message that was sent through UDP with 
 **					retransmissions did not receive a matching response
 **					after the maximum retransmissions have been sent.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ntfy_uaTxnTimeout
#ifdef ANSI_PROTO
	(Sdf_st_callObject       *pObject,
	 SipMessage              *pMessage,
	 en_SipTimerType         dTimerType,
	 Sdf_st_error            *pError)
#else
	(pObject,pMessage,dTimerType,pError)
	 Sdf_st_callObject       *pObject;
	 SipMessage              *pMessage;
	 en_SipTimerType        dTimerType; 
	 Sdf_st_error            *pError;
#endif
{
	Sdf_st_overlapTransInfo *pOverlapTransaction = Sdf_co_null;
	Sdf_st_transaction      *pTransaction = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "\
			           "sdf_ntfy_uaTxnTimeout");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ntfy_uaTxnTimeout( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ntfy_uaTxnTimeout( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ntfy_uaTxnTimeout( ): "
			" SipMessage parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/* Set the Timeout Flag in Transaction to True */
	if( (dTimerType == SipTimerB) || (dTimerType == SipTimerH))
		pObject->pCommonInfo->dIsTimeout = Sdf_co_true;

	/*
     * This API is invoked to fetch the Overlap Transaction.
	 */
	if (sdf_ivk_uaGetTransactionForSipMsg(pObject, pMessage, &pTransaction,\
				&pOverlapTransaction, pError) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
		    (Sdf_ty_s8bit *)"sdf_ntfy_uaTxnTimeout(): "
		    "Failed to get the transaction from SipMessage.", pError);
#endif
	    return Sdf_co_fail;		
    }

	if( pOverlapTransaction != Sdf_co_null )
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
{
		(void)sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransaction);
#endif
	/*w534*/(void)sdf_ivk_uaRemoveOverlapInfo(pObject, pOverlapTransaction);

#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
}
#endif
	else
		(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		               "sdf_ntfy_uaTxnTimeout");
	return Sdf_co_success;
}
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
Sdf_ty_retVal sdf_ivk_uaUpdateTxnSockFd
	( Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_socket 			 *pSocket,
	 Sdf_st_error	         *pError)
	
{
	SIP_S8bit			      dCreate=0;
	SipTxnKey				 *pTxnKey = Sdf_co_null;
	SipTxnBuffer 			 *pTxnBuffer = Sdf_co_null;
 	SipTranspAddr			 *pTranspaddr = Sdf_co_null;

	SIPDEBUGFN("Entering into sdf_ivk_uaUpdateTxnSockFd");

	/*Check for the validity of the i/p parameters*/
#ifdef SDF_PARAMVALIDATION
	if(pSocket == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateTxnSockFd ():  "
			"pSocket Param Passed is invalid ",pError);
#endif
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateTxnSockFd ():  "
			"pError Param Passed is invalid ",pError);
#endif
		return Sdf_co_fail;
	}
#endif
	if(pOverlapTransInfo == Sdf_co_null)
	{	
		pTransaction->dSocket = *pSocket;
		pTxnKey = pTransaction->pTxnKey;
	}
	else
	{
		pOverlapTransInfo->dSocket = *pSocket;

		if(pOverlapTransInfo->pOLTxnKey != Sdf_co_null)
			pTxnKey = pOverlapTransInfo->pOLTxnKey;
		else
			pTxnKey = pOverlapTransInfo->pRprTxnKey;
	}
	/*
	 *	Fetch the Stack Transaction from the hash using the txn key. 
	 */
	if (sdf_ivk_uaHashExclFetch(&(pGlbToolkitData->dSipTxnHash), pTxnKey, \
		(SIP_Pvoid *)(&pTxnBuffer), dCreate) == Sdf_co_fail)
	{
		pError->errCode = Sdf_en_noExistError;
		return Sdf_co_fail;
	}
	if (Sdf_co_null != pTxnBuffer->pAddr)
	{
		pTranspaddr = pTxnBuffer->pAddr;
	}
	else
	{
		sdf_ivk_uaHashExclRelease(&(pGlbToolkitData->dSipTxnHash), \
		pTxnKey, dCreate,Sdf_co_null,(SIP_Pvoid *)(& pTxnBuffer));
		return Sdf_co_fail;
	}

	pTranspaddr->dSockFd=\
				pSocket->dSockfd;
	/*
	 *  Release the Hash 
	 */
	sdf_ivk_uaHashExclRelease(&(pGlbToolkitData->dSipTxnHash), \
		pTxnKey, dCreate,Sdf_co_null,(SIP_Pvoid *)(& pTxnBuffer));
	
	SIPDEBUGFN("Exiting from sdf_ivk_uaUpdateTxnSockFd");

	return Sdf_co_success;
		

}

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
 Sdf_st_error *pError)
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaProcessTimeout");

    (void) pError; /* to avoid compilation warning */
	/* Input Param Validation */
#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
		return Sdf_co_fail;

	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaProcessTimeout( ): "
				"Invalid param value: Call Object",pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

    /*  If the call-state is in Sdf_en_inviteSent state, move it
     *  idle state so that out state machine handles it correctly.
     */
    if(pObject->pCallInfo->dState == Sdf_en_inviteSent)
        pObject->pCallInfo->dState = Sdf_en_idle;
       
	/* The handling is currently only in case of DEP.Here we call
	 * the event posting function */
#ifdef SDF_DEP
	/* Call the function to post trigger for timeout scenarios */
	if(sdf_fn_uaDepPostOnTimeout(pObject,pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,	pError->errCode,
				(Sdf_ty_s8bit *)"sdf_ivk_uaProcessTimeout( ):"
				"Posting on event manager failed",pError);
#endif
		return Sdf_co_fail;	
	}
#endif
	return Sdf_co_success;
}
	
#endif /* End of ifdef SDF_TXN */
