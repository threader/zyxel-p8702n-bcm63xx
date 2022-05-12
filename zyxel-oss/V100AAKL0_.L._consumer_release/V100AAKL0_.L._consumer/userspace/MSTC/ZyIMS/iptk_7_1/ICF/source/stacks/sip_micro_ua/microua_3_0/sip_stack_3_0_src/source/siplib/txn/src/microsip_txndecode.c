/************************************************************************
 ** FUNCTION:
 **            Parses SIP Messages
 *********************************************************************
 **
 ** FILENAME:
 ** microsip_txndecode.c
 **
 ** DESCRIPTION:
 **             This file contains the logic for parsing SIP messages.
 **     It invokes a number of bison generated parsers. It also contains
 **     the logic for indicate invocation.
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 **
 ** 19/03/2002	Mahesh									Initial Version	
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


#include "microsip_txndecode.h"	
#include "microsip_txndecodeintrnl.h"
#include "microsip_txntimer.h"
#include "microsip_timer.h"
#include "microsip_txninit.h"
#include "microsip_txnclone.h"
#include "microsip_txnfree.h"
#include "microsip_accessors.h" 
#include "microsip_header.h" 
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#define FETCH_TXN(a,b,c,d,e) sip_cbk_fetchTxn(a,b,c,d,e)
#define RELEASE_TXN(a,b,c,d,e,f) sip_cbk_releaseTxn(a,b,c,d,e,f)
#define FREE_EVENT_CONTEXT(a) sip_freeEventContext(a)
#define START_TIMER(a,b,c,d,e) fast_startTxnTimer(a,b,c,d,e) 
#define STOP_TIMER(a,b,c) fast_stopTxnTimer(a,b,c)
#define FREE_TIMER_HANDLE(a)	sip_freeTimerHandle(a)


/***************************************************************************
**** FUNCTION:sip_txn_abortTxn
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_txn_abortTxn
#ifdef ANSI_PROTO
(
	SipTxnKey *pTxnKey,
	SipTxnContext	*pTxnContext,
	SipError *pError
)
#else
	(pTxnKey,pTxnContext,pError)
	SipTxnKey *pTxnKey;
	SipTxnContext	*pTxnContext;
	SipError *pError;
#endif
{
	SipBool dResult;
	SIP_S8bit dCreate=0;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipEventContext	*pEventContext=SIP_NULL;
	SipTxnKey *pTempKey=SIP_NULL;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering into sip_txn_abortTxn");

	/*Check for the validity of the i/p parameters*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if (pTxnKey==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	if (pTxnContext==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}
	
	/*
	 * Get a reference to the event context in the txnContext
	 * from which the context data will be then extracted
	 */
	pEventContext=pTxnContext->pEventContext;

	if (pEventContext!=SIP_NULL)
	{
		pContextData=pEventContext->pData;	
	}
	
	/*First fetch the txn*/
	/*Call a fetch txn using this key, without O_CREATE*/
	dResult=FETCH_TXN((SIP_Pvoid *)(&pTxnBuffer),pTxnKey, \
				pContextData,dCreate,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*This function internally frees up the passed key, so we need
	to pass a reference to a different key*/

	if (sip_initSipTxnKey(&pTempKey,pError)==SipFail)
		return SipFail;

	if(sip_cloneSipTxnKey(pTempKey,pTxnKey,pError) == SipFail)
		return SipFail;

	dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTempKey, \
			SipTxnTerminatedState,SIP_NULL,pContextData,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	SIPDEBUGFN("Exiting from sip_txn_abortTxn");

	/*No need of freeing any memory since the above state
		change does all that*/
	*pError=E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
**** FUNCTION:sip_txn_cancelTxn
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_txn_cancelTxn
#ifdef ANSI_PROTO
(
	SipTxnKey *pTxnKey,
	SipList **ppList,
	SipError *pError
)
#else
	(pTxnKey,ppList,pError)
	SipTxnKey *pTxnKey;
	SipList **ppList;
	SipError *pError;
#endif
{
	SipBool dResult;
	SIP_S8bit dCreate=0;
	SIP_S8bit dRemove=0;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipTxnKey *pTempKey=SIP_NULL;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering into sip_txn_cancelTxn");

	/*Check for validity of the i/p params*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if ((pTxnKey==SIP_NULL)||(ppList==SIP_NULL))
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*This API will be valid only in the case of proxies*/

	/*First fetch the TxnBuffer corresponding to the passed key*/
	/*Now call a fetch txn using this key, without O_CREATE*/
	dResult=FETCH_TXN((SIP_Pvoid *)(&pTxnBuffer),pTxnKey, \
				pContextData,dCreate,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}
	
	/*In case the application has not initialised the txn with the Proxy type
		then the cancellation is proof enough that the Txn is of Proxy type*/
	if(pTxnBuffer->dType!=SipProxyTypeTxn)
		pTxnBuffer->dType=SipProxyTypeTxn;
		
	/*Txn is fetched obtain  the contained queue*/
	*ppList=pTxnBuffer->pslMesgQ;

	/*Set the reference of the Txn Buffer to Null*/
	pTxnBuffer->pslMesgQ=SIP_NULL;

	/*There are no timers to be stopped in this state
	hence directly change the state of the txn*/
	pTxnBuffer->dTxnState=SipTxnServerBypassState;

	/*Once the state is set to ByPassState the only way this txn
		is ever going to get removed is by calling an abortTxn*/
	
	dResult=glbSipTxnStateChange[SipTxnServerBypassState]\
					(pTxnBuffer,pTxnKey,pContextData,SIP_NULL,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		SipError	tempError;
		
		/*Release the txn as well*/
		dResult=RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,&tempError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
		return SipFail;
	}
	
	/*Now release the txn as well --withut O_REMOVE*/
	dResult=RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	SIPDEBUGFN("Exiting from sip_txn_cancelTxn");

	/*No need to free the key, since it will be done by the
	applicn which called this fn*/
	*pError=E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_txn_sendMessage
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_txn_sendMessage
#ifdef ANSI_PROTO
(
	SipMessage *pMessage,
	SipTranspAddr *pAddr,
	SIP_S8bit dTranspType,
	SipTxnContext *pTxnContext,
	en_SipTxnBypass dBypassEnable,
	SipTxnKey **ppTxnKey,
	SipError *pError
)
#else
	(pMessage,pAddr,dTranspType,pTxnContext,dBypassEnable,ppTxnKey,pError)
	SipMessage *pMessage;
	SipTranspAddr *pAddr;
	SIP_S8bit dTranspType;
	SipTxnContext *pTxnContext;
	en_SipTxnBypass dBypassEnable;
	SipTxnKey **ppTxnKey;
	SipError *pError;
#endif
{
	en_SipTxnClass dTxnClass;
	SIP_S8bit *pBuffer=SIP_NULL;
	SIP_S8bit *pTempBuffer=SIP_NULL;
	SipTxnInformn txnInformn;
	SipBool dResult;
	SipEventContext *pContext=SIP_NULL;
	SipOptions *pSipOptions=SIP_NULL;
	SIP_U32bit dLength;
	SipBool dDirectOption=SipFail;

	SIPDEBUGFN("Entering into sip_txn_sendMessage");

	/*Check for validity of the i/p params*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if ((pMessage==SIP_NULL)||(pAddr==SIP_NULL)||(pTxnContext==SIP_NULL))
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	if (ppTxnKey==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Obtain the event context from the Txn Event cntext which needs to
		be passed to sip_send_Message*/
	pContext=pTxnContext->pEventContext;
	/*if (pContext==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}*/

	/*Obtain the Options that will be sent to sendMessage*/
	pSipOptions=&(pTxnContext->txnOption.dOption);

	/*Check if the direct buffer option has been set or not*/
    if ((pSipOptions->dOption & SIP_OPT_DIRECTBUFFER) == SIP_OPT_DIRECTBUFFER)
		dDirectOption=SipSuccess;
		else
		dDirectOption=SipFail;

	if (dDirectOption==SipFail)
	{
		/*Form the message buffer*/
		/*Call the form_message to obtain the actual buffer corresponding
			to the message*/
		pTempBuffer=(SIP_S8bit *) fast_memget(TXN_MEM_ID,SIP_MAX_MSG_SIZE,pError);
		if (pTempBuffer==SIP_NULL)
		{
			/*Handle failure here*/
			return SipFail;
		}
		
		dLength = SIP_MAX_MSG_SIZE;	

		if (sip_formMessage(pMessage,pSipOptions,pTempBuffer,&dLength,pError) \
				==	SipFail)
		{
			(void)fast_memfree(TXN_MEM_ID,pTempBuffer,NULL);
            /*SPR-5471. LEAK. The transport details were not freed if formMessage fails*/
			(void)fast_memfree(TXN_MEM_ID,pAddr->pHost,NULL);
			(void)fast_memfree(TXN_MEM_ID,pAddr,NULL);

			return SipFail;
		}
		pBuffer	= (SIP_S8bit *) fast_memget(TXN_MEM_ID,dLength,pError);	
		if (pBuffer==SIP_NULL)
		{
			/*Handle failure here*/
			(void)fast_memfree(TXN_MEM_ID,pTempBuffer,NULL);
			return SipFail;
		}
		memcpy(pBuffer,pTempBuffer,dLength);
		(void)fast_memfree(TXN_MEM_ID,pTempBuffer,NULL);

	}
	else
	{
		if(pContext->pDirectBuffer == SIP_NULL)
		{
			*pError=E_INV_PARAM;
        	/*SPR-5471. LEAK. The transport details were not freed if formMessage fails*/
			(void)fast_memfree(TXN_MEM_ID,pAddr->pHost,NULL);
			(void)fast_memfree(TXN_MEM_ID,pAddr,NULL);

			return SipFail;
		}

		pBuffer = (SIP_S8bit*) pContext->pDirectBuffer;
		dLength = sip_strlen(pBuffer);
	}

	/*The message has been sent now start the txn level handling on this*/
	dResult=sip_captureTxnInformn(pMessage,pBuffer,dLength,pTxnContext, \
								pAddr,dTranspType,&txnInformn);
	if (dResult!=SipSuccess)
	{
		/*Handle failure  here*/
		return SipFail;
	}

	txnInformn.dAPICalled=SipTxnMesgSent;
	txnInformn.dDirectOption=dDirectOption;

	if (dBypassEnable==SipTxnByPass)
	{
		/*Return immediately after sending the message*/
		/*Send the buffer that had been formed*/
		dResult=sip_txn_sendBuffer(&txnInformn,pError);
		if (dResult!=SipSuccess)
		{
			/*Free the buffer in TxnInformn*/
			if (!dDirectOption)
			(void)fast_memfree(TXN_MEM_ID,pBuffer,pError);
			return SipFail;
		}

		/*Free the buffer in TxnInformn*/
		if (!dDirectOption)
		(void)fast_memfree(TXN_MEM_ID,pBuffer,pError);
		
		/*Free up the eventContext*/
		if (pTxnContext->pEventContext != SIP_NULL)
		{
			FREE_EVENT_CONTEXT(pTxnContext->pEventContext);
			pTxnContext->pEventContext = SIP_NULL;
		}
		return SipSuccess;
	}

	/*Ensure that all the mandatory headers are present*/
	dResult=sip_checkMandatoryTxnHeaders(pMessage,pTxnContext->dTxnType);
    if (dResult!=SipSuccess)
	{
		sip_freeSipMessage(pMessage);
    	*pError = E_TXN_INV_MSG;
    	return SipFail;
    }

	/*Obtain the class of the txn whether inv-client,non-inv-client...*/
	dResult=sip_getTxnClass(&txnInformn,SipTxnMesgSent,&dTxnClass);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Also add the clas informn into the TxnInformn*/
	txnInformn.dTxnClass=dTxnClass;

	/*Set the pKeyOfCreatedTxn component of TxnInformn to SIP_NULL*/
	txnInformn.pKeyOfCreatedTxn=SIP_NULL;

	/*The message needs to be sent at least once for all success cases*/
	txnInformn.dMesgSentAtLeastOnce=SipFail;

	/*On the basis of the kind of txn that will get affected call the
		respective update function*/
	if (sip_isTxnAnRPR(&txnInformn)==SipSuccess)
	{
		dResult=sip_updateRPRTxnForMesgSent(&txnInformn,pError);
		if (dResult!=SipSuccess)
		{
			SipError intError;

			/*Handle failure here*/
			/*Free the buffer in TxnInformn*/
			if (!dDirectOption)
				(void)fast_memfree(TXN_MEM_ID,pBuffer,&intError);
			return SipFail;
		}
	}		
	else
	{	
		switch(dTxnClass)
		{
			case SipInviteClient:
			{
				dResult=sip_updateInvClTxnForMesgSent(\
				&txnInformn,pError);
				if (dResult!=SipSuccess)
				{
					SipError intError;

					/*Handle failure here*/
					/*Free the buffer in TxnInformn*/
					if (!dDirectOption)
					(void)fast_memfree(TXN_MEM_ID,pBuffer,&intError);
					return SipFail;
				}
				break;
			}

			case SipNonInviteClient:
			{
				dResult=sip_updateNonInvClTxnForMesgSent(\
				&txnInformn,pError);
				if (dResult!=SipSuccess)
				{
					SipError intError;

					/*Handle failure here*/
					/*Free the buffer in TxnInformn*/
					if (!dDirectOption)
					(void)fast_memfree(TXN_MEM_ID,pBuffer,&intError);
					return SipFail;
				}
				break;
			}

			case SipInviteServer:
			{
				dResult=sip_updateInvSrvTxnForMesgSent(\
				&txnInformn,pError);
				if (dResult!=SipSuccess)
				{
					SipError intError;
					/*Handle failure here*/
					/*Free the buffer in TxnInformn*/
					if (!dDirectOption)
					(void)fast_memfree(TXN_MEM_ID,pBuffer,&intError);
					return SipFail;
				}
				break;
			}

			case SipNonInviteServer:
			{
				dResult=sip_updateNonInvSrvTxnForMesgSent(\
				&txnInformn,pError);
				if (dResult!=SipSuccess)
				{
					SipError intError;

					/*Handle failure here*/
					/*Free the buffer in TxnInformn*/
					if (!dDirectOption)
					(void)fast_memfree(TXN_MEM_ID,pBuffer,&intError);
					return SipFail;
				}
				break;
			}

		default:
			{
				/*Handle error condition*/
				return SipFail;
			}
		}/*Switch ends*/
	}/*else block ends for isItAnRPR*/	

	/*You come here only for succesful return from the above switch-case
	check for two things
	1>If message has not got sent even once it needs to be sent.
	2>If the buffer inside txnInformn is non-NULL means nobody owns
		it apart from TxnInformn and it needs to be freed*/

	if (txnInformn.dMesgSentAtLeastOnce==SipFail)
	{
		SIP_S8bit *pTempVariable=SIP_NULL;

		pTempVariable=txnInformn.pMsgBuffer;

		txnInformn.pMsgBuffer=pBuffer;
		dResult=sip_txn_sendBuffer(&txnInformn,pError);
		if (dResult!=SipSuccess)
		{
			if (txnInformn.pMsgBuffer!=SIP_NULL)
			{
				/*Free the buffer in TxnInformn*/
				if (!dDirectOption)
				(void)fast_memfree(TXN_MEM_ID,txnInformn.pMsgBuffer,pError);
			}
			return SipFail;
		}
		txnInformn.pMsgBuffer=pTempVariable;
	}

	if (txnInformn.pMsgBuffer!=SIP_NULL)
	{
		/*Free the buffer in TxnInformn*/
		if (!dDirectOption)
		(void)fast_memfree(TXN_MEM_ID,txnInformn.pMsgBuffer,pError);
	}

	/*Now check if a txn got created, this will be indicated by the
	 presence of a Non-Null value in the pKeyOfCreatedTxn of TxnInformn*/
	if (txnInformn.pKeyOfCreatedTxn!=SIP_NULL)
	{
		*ppTxnKey=txnInformn.pKeyOfCreatedTxn;
	}
	else
	{
		*ppTxnKey=SIP_NULL;
	}


	SIPDEBUGFN("Exiting from sip_txn_sendMessage");

	*pError=E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_txn_handleTxn
****
****
**** DESCRIPTION:This API will be called when an application has invoked the
**** core SIP API sip_decodeMessage to decode the message and later wants to
**** handle the message from within a transaction.
******************************************************************************/
SipBool sip_txn_handleTxn
#ifdef ANSI_PROTO
	(SipMessage *pMessage , \
	SipTxnContext *pTxnContext, \
	en_SipTxnDecodeResult *pDecodeResult,\
	SipTxnKey **ppTxnKey,\
	SipError *pError)
#else
	(pMessage, pTxnContext,pDecodeResult, ppTxnKey,pError)
	SipMessage *pMessage;
	SipTxnContext *pTxnContext;
	en_SipTxnDecodeResult *pDecodeResult;
	SipTxnKey **ppTxnKey;
	SipError *pError;
#endif
{
	en_SipTxnClass dTxnClass;
	SIP_S8bit *pBuffer=SIP_NULL;
	SipTxnInformn txnInformn;
	SipBool dResult;
	SIP_S8bit dTranspType=0;
	SIP_U32bit	dLength=0;

	SIPDEBUGFN("Entering function sip_txn_handleTxn");

	/*Check the validity of the i/p params*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if ((pTxnContext==SIP_NULL)||(pMessage==SIP_NULL))
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Check for mandatorily necessary headers*/
	/*Ensure that all the mandatory headers are present*/
	dResult=sip_checkMandatoryTxnHeaders(pMessage,pTxnContext->dTxnType);
    if (dResult!=SipSuccess)
	{
    	*pError = E_TXN_INV_MSG;
    	return SipFail;
    }
	
	/*Start the txn level handling on this*/
	dResult=sip_captureTxnInformn(pMessage,pBuffer,dLength,pTxnContext, \
								SIP_NULL,dTranspType,&txnInformn);
	if (dResult!=SipSuccess)
	{
		/*Handle failure  here*/
                /* set the error to invalid message on failure */
		*pError = E_TXN_INV_MSG;
		return SipFail;
	}

	/*Set the pKeyOfCreatedTxn component of TxnInformn to SIP_NULL*/
	txnInformn.pKeyOfCreatedTxn=SIP_NULL;

	txnInformn.dAPICalled=SipTxnMesgRecvd;

	/*
	 * The dDirectOption has to be by default set to SipFail in this case
	 * since the question of using a preallocated buffer does not arise 
	 * in case of a decode!!
	 */
	txnInformn.dDirectOption=SipFail;

	/*Obtain the class of the txn whether inv-client,non-inv-client...*/
	dResult=sip_getTxnClass(&txnInformn,SipTxnMesgRecvd,&dTxnClass);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Also add the clas informn into the TxnInformn*/
	txnInformn.dTxnClass=dTxnClass;

	/*In case the message is an RPR we need to have different 
		handling for it*/
	if (sip_isTxnAnRPR(&txnInformn)==SipSuccess)
	{
		dResult=sip_updateRPRTxnAfterMesgDecoding(\
				&txnInformn,pDecodeResult,pError);
	}
	else
	{	
		/*On the basis of the kind of txn that will get affected call the
			respective update function*/
		switch(dTxnClass)
		{
			case SipInviteClient:
			{
					dResult=sip_updateInvClTxnAfterMesgDecoding(\
					&txnInformn,pDecodeResult,pError);
				break;
			}

			case SipNonInviteClient:
			{
				dResult=sip_updateNonInvClTxnAfterMesgDecoding(\
					&txnInformn,pDecodeResult,pError);
				break;
			}

			case SipInviteServer:
			{
				dResult=sip_updateInvSrvTxnAfterMesgDecoding(\
					&txnInformn,pDecodeResult,pError);
				break;
			}

			case SipNonInviteServer:
			{
				dResult=sip_updateNonInvSrvTxnAfterMesgDecoding(\
					&txnInformn,pDecodeResult,pError);
				break;
			}
		default:
			{
				dResult=SipFail;
			}
		}
	}	

	/*After the switch case/RPR is through we can check the result of the
		update functions*/
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Now check if a txn got created, this will be indicated by the
	 presence of a Non-Null value in the pKeyOfCreatedTxn of TxnInformn*/
	if (txnInformn.pKeyOfCreatedTxn!=SIP_NULL)
	{
		*ppTxnKey=txnInformn.pKeyOfCreatedTxn;
	}
	else
	{
		*ppTxnKey=SIP_NULL;
	}

	SIPDEBUGFN("Exiting function sip_txn_handleTxn");

	*pError=E_NO_ERROR;
	return SipSuccess;
}



/***************************************************************************
**** FUNCTION:sip_txn_decodeMessage
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_txn_decodeMessage
#ifdef ANSI_PROTO
	(SIP_S8bit *pMsgBuffer, SipMessage **ppDecodedMessage , \
	SipTxnContext *pTxnContext, SIP_U32bit dMessageLength, \
	SIP_S8bit **ppNextMessage, en_SipTxnDecodeResult *pDecodeResult,\
	SipTxnKey **ppTxnKey,SipError *pError)
#else
	(pMsgBuffer, ppDecodedMessage, pTxnContext, dMessageLength,\
	ppNextMessage,pDecodeResult, ppTxnKey,pError)
	SIP_S8bit *pMsgBuffer;
	SipMessage **ppDecodedMessage;
	SipTxnContext *pTxnContext;
	SIP_U32bit dMessageLength;
	SIP_S8bit **ppNextMessage;
	en_SipTxnDecodeResult *pDecodeResult;
	SipTxnKey **ppTxnKey;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipOptions *pSipOptions=SIP_NULL;
	SipEventContext *pEventContext=SIP_NULL;

	SIPDEBUGFN("Entering function sip_txn_decodeMessage");

	/*Check for the validity of the i/p params*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if ((pMsgBuffer==SIP_NULL)||(ppDecodedMessage==SIP_NULL)|| \
		(pTxnContext==SIP_NULL)||(pDecodeResult==SIP_NULL))
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Obtain the event context from the Txn Event cntext which needs to
		be passed to sip_send_Message*/
	pEventContext=pTxnContext->pEventContext;
	if (pEventContext==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Obtain the Options that will be sent to sendMessage*/
	pSipOptions=&(pTxnContext->txnOption.dOption);

	/*Check if proper options have been passed*/
	if (pSipOptions==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Ensure that the flag SIP_OPT_NOTIMER option is set by default
	 so that the call to sip_decodeMessage does not result in any attempts
	 to stop timers*/
	pSipOptions->dOption |= SIP_OPT_NOTIMER;

	dResult=sip_decodeMessage(pMsgBuffer,ppDecodedMessage, \
				pSipOptions,dMessageLength,ppNextMessage,pEventContext,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Now call handle_txn since you have the SipMessage*/
	/*The message has been sent now start the txn level handling on this*/
	dResult=sip_txn_handleTxn(*ppDecodedMessage,pTxnContext, \
								pDecodeResult,ppTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure  here*/
		return SipFail;
	}


	SIPDEBUGFN("Exiting function sip_txn_decodeMessage");

	*pError=E_NO_ERROR;
	return SipSuccess;
}


/*******************************************************************************
**** FUNCTION:sip_txn_compareTxnKeys
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_txn_compareTxnKeys
#ifdef ANSI_PROTO
	(SipTxnKey *pStoredKey,	SipTxnKey *pGeneratedKey, SipError *pErr)
#else
	(pStoredKey, pGeneratedKey, pErr)
	SipTxnKey *pStoredKey;
	SipTxnKey *pGeneratedKey;
	SipError *pErr;
 
#endif
{
	SipBool	dIsItAnAck=SipSuccess;

	SIPDEBUGFN("Entering into sip_txn_compareTxnKeys");

	/*Check for the validity of the i/p params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if ((pStoredKey==SIP_NULL)||(pGeneratedKey==SIP_NULL))
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	*pErr =  E_NO_ERROR;

	/* Check the To Tag only if the dTagCheck Flag is enabled */
	if((pGeneratedKey->dTagCheck) && (pStoredKey->dTagCheck))
	{
		/* do validity of To Tag */
		if((pGeneratedKey->pToTag)&&(pStoredKey->pToTag))
		{
			if(sip_strcasecmp(pGeneratedKey->pToTag,pStoredKey->pToTag) != 0)
			return SipFail;
		}
		else
		{
		/* if one is NULL and one is NOT NULL return Failure */
		if(pGeneratedKey->pToTag != pStoredKey->pToTag)
			return SipFail;
		}
	}

	/* do validity of From Tag */
	if((pGeneratedKey->pFromTag)&&(pStoredKey->pFromTag))
	{
		if(sip_strcasecmp(pGeneratedKey->pFromTag,pStoredKey->pFromTag) != 0)
		return SipFail;
	}
	else
	{
		/* if one is NULL and one is NOT NULL return Failure */
		if(pGeneratedKey->pFromTag != pStoredKey->pFromTag)
			return SipFail;
	}
		
	/* check the callId and cseq */
	if(pGeneratedKey->dCSeq != pStoredKey->dCSeq)
			return SipFail;

	/*Check for the class of the Txn as well*/
	if(pGeneratedKey->dTxnClass != pStoredKey->dTxnClass)
			return SipFail;
	
	/*Confirm validity of key--Method*/
	if ((pGeneratedKey->pMethod==SIP_NULL) ||
		(pStoredKey->pMethod==SIP_NULL))
		return SipFail;

	/*Confirm validity of key--Callid*/
	if ((pGeneratedKey->pCallid==SIP_NULL) ||
		(pStoredKey->pCallid==SIP_NULL))
		return SipFail;

	/*Go ahead with comparing callid,method*/
	/* Compare only if Method is not ACK */
	if(sip_strcmp(pGeneratedKey->pMethod,(SIP_S8bit*)"ACK") != 0)
	{
		dIsItAnAck=SipFail;
		if(sip_strcmp(pGeneratedKey->pMethod,pStoredKey->pMethod) != 0)
			return SipFail;
	}
	else
	{
		/* 
		 * If the generated key is for an ACK message, make sure that the txn
		 	we match it to is an INVITE
		 */
        if(pGeneratedKey->dType == SipMessageRequest)
        {
            if (sip_strcmp(pStoredKey->pMethod,(SIP_S8bit*)"INVITE") != 0)
                return SipFail;
        }
        else
        {
            return SipFail;
        }
	}
	
	if(sip_strcmp(pGeneratedKey->pCallid,pStoredKey->pCallid) != 0)
		return SipFail;



	if (dIsItAnAck == SipFail)
	{

		/* Check for Via branch is made if the message is not 
		 * an ACK.Check for via branch in case of ACk message is made
	     * in sip_updateInvSrvTxnAfterMesgDecoding function
         * in txndecodeintrnl.c file
         */	
		if (pGeneratedKey->dViaBranchCheck==SipSuccess)
		{	
			/*Check if Via branch is available*/
			if ((pGeneratedKey->pViaBranch!=SIP_NULL) &&
				(pStoredKey->pViaBranch!=SIP_NULL))
			{
				if (sip_strcasecmp(pGeneratedKey->pViaBranch,\
						pStoredKey->pViaBranch) != 0)
					return SipFail;
			}
			else
			{
				/*It is possible both are NULL*/
				if ((pGeneratedKey->pViaBranch!=pStoredKey->pViaBranch))
					return SipFail;
			}

			/*As per loop detection mechanism we need to ensure that the
			count of the Via headers in the two messages is the same as well*/
			if (pGeneratedKey->dViaHdrCount!=pStoredKey->dViaHdrCount)
				return SipFail;
		}
	}

	/*
	 * If an ACK is being sent out and the dViaBranchCheck in the stored
	 * key is set to TRUE, do the via branch check. The dViaBranchCheck in
	 * the stored key is always TRUE. It is set to FALSE only when a 2xx
	 * response is received to the INVITE. This is because the ACK to the
	 * 2xx will have a different Via branch...and hence the check must not
	 * be done.
	 */
	if ( (dIsItAnAck == SipSuccess) && \
		(pStoredKey->dViaBranchCheck == SipSuccess) )
	{
		/*Check if Via branch is available*/
		if ((pGeneratedKey->pViaBranch!=SIP_NULL) &&
				(pStoredKey->pViaBranch!=SIP_NULL))
		{
			if (sip_strcasecmp(pGeneratedKey->pViaBranch,\
						pStoredKey->pViaBranch) != 0)
				return SipFail;
		}
		else
		{
			/*It is possible both are NULL*/
			if ((pGeneratedKey->pViaBranch!=pStoredKey->pViaBranch))
				return SipFail;
		}
	}

	if((pGeneratedKey->dRseq != pStoredKey->dRseq))
			return SipFail;

#ifdef SIP_RPR
	/* compare the RackHdr Structure if present in both keys
	If RackHeader is not present in both return SipSuccess.
	*/
	if(( pGeneratedKey->pRackHdr )&& (pStoredKey->pRackHdr))
	{
		if(sip_strcmp(pGeneratedKey->pRackHdr->pStr1, \
			pStoredKey->pRackHdr->pStr1) != 0)
			return SipFail;

		if((pGeneratedKey->pRackHdr->dIntVar1 !=  \
			pStoredKey->pRackHdr->dIntVar1) ||    \
			(pGeneratedKey->pRackHdr->dIntVar2 != \
			pStoredKey->pRackHdr->dIntVar2) )
			return SipFail;
	}
#endif
	SIPDEBUGFN("Exiting from sip_txn_compareTxnKeys");
	return SipSuccess;
}

/*************************************************************************************
**** FUNCTION:sip_txn_createTxnKey
****
****
**** DESCRIPTION:
*************************************************************************************/
SipBool sip_txn_createTxnKey
#ifdef ANSI_PROTO
	(SipMessage *pMessage, en_SipTxnClass		dTxnClass,\
		SipTxnKey **ppTxnKey, SipError *pErr)
#else
	(pMessage, dTxnClass, ppTxnKey, pErr)
	SipMessage *pMessage;
	en_SipTxnClass	dTxnClass;
	SipTxnKey **ppTxnKey;
	SipError *pErr;

#endif
{
	SipGeneralHeader *pGenHdr=SIP_NULL;
	SIP_U32bit count=0;
	SipReqHeader *pReqHdr=SIP_NULL;
	SipRespHeader *pRespHdr=SIP_NULL;
	en_SipMessageType dType;
	SipTxnKey *pKey=SIP_NULL;
	SIP_S8bit *pTempStr=SIP_NULL;

	SIPDEBUGFN("Entering into sip_txn_createTxnKey");

	/*Check for the validity of the i/p params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if ((pMessage==SIP_NULL)||(ppTxnKey==SIP_NULL))
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	dType = pMessage->dType;

	pGenHdr = pMessage->pGeneralHdr;

	if( dType == SipMessageRequest )
		pReqHdr =  pMessage->u.pRequest->pRequestHdr;
	else
		pRespHdr= pMessage->u.pResponse->pResponseHdr;

	/* Init the Key */
	if(sip_initSipTxnKey(ppTxnKey,pErr) == SipFail)
		return SipFail;

	pKey = *ppTxnKey;
	pKey->dType = dType;
	pKey->dTxnClass=dTxnClass;
	/* Timer *ppKey stuff - Callid, Method, Cseq */
	/*pKey->dTagCheck = 0;*/
	pKey->dTagCheck = SipFail;
	pKey->pCallid= (SIP_S8bit *)STRDUPNETWORK(pGenHdr->pCallidHdr->pStr1);
	if(pKey->pCallid == SIP_NULL)
	{
		 *pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
                 sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/
		 
		 return SipFail;
	}

	/* If Request Message take from Request line other wise take from
		CSeq Header */
	if( dType == SipMessageRequest )
	{
		pKey->pMethod = (SIP_S8bit *)STRDUPNETWORK(pMessage->\
							u.pRequest->pRequestLine->pStr1);
		if(pKey->pMethod == SIP_NULL)
		{
			*pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
                        sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/
		 	return SipFail;
		}
	}
	else
	{
		pKey->pMethod = (SIP_S8bit *)STRDUPNETWORK(pGenHdr->pCseqHdr->pStr1);
		if(pKey->pMethod == SIP_NULL)
		{
			*pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
                        sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/			
		 	return SipFail;
		}

		pKey->dRespCodeNum = (SIP_U16bit)pMessage->u.pResponse->pStatusLine->dIntVar1;
	}

	pKey->dCSeq = pGenHdr->pCseqHdr->dIntVar1;

	/* Only if it is a Response Message */
	if( dType == SipMessageResponse )
	{
		/* Only If Rseq Header is Not NULL */
		if(pRespHdr->pRSeqHdr)
		{
			SIP_U32bit dCodeNum;
			dCodeNum = pMessage->u.pResponse->pStatusLine->dIntVar1;
			if((dCodeNum < 200)&&(dCodeNum > 100))
				pKey->dRseq = pRespHdr->pRSeqHdr->dIntVar1;
		}
	}

	/*Get the From Tag*/
	if (sip_getTagfromFromHeader(pGenHdr->pFromHeader,\
	 &pTempStr, pErr) != SipFail)
	{
/*CSR_1-4868194 starts*/		
        if(pTempStr!=SIP_NULL)
		{
/*CSR_1-4868194 ends*/	
          pKey->pFromTag = (SIP_S8bit *)STRDUPNETWORK((SIP_S8bit *)pTempStr);
		  if(pKey->pFromTag == SIP_NULL)
		  {
		 	*pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
             sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/  		 	
	 		return SipFail;
		  }
/*CSR_1-4868194 starts*/
       }
/*CSR_1-4868194 ends*/
	}


	/*Get the To Tag*/
	if (sip_getTagfromFromHeader(pGenHdr->pToHdr,\
	 &pTempStr, pErr) != SipFail)
	{
/*CSR_1-4868194 starts*/                
		if(pTempStr!=SIP_NULL)
		{	
/*CSR_1-4868194 ends*/            
          pKey->pToTag = (SIP_S8bit *)STRDUPNETWORK((SIP_S8bit *)pTempStr);
		  if(pKey->pToTag == SIP_NULL)
		  {
			 *pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
             sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/  			 
	 		return SipFail;
		   }
/*CSR_1-4868194 starts*/		   
         }
/*CSR_1-4868194 ends*/
	}

	/* Add branch from first Via Header */
	pKey->pViaBranch = SIP_NULL;
	/* Check if Via header present */
	if(sip_listSizeOf(&(pGenHdr->slViaHdr), &count, pErr)\
					!=SipFail)
	{
		if(count!=0)
		{
			SipViaHeader *pViaHeader;
			
			/*Set the Via Header count*/
			pKey->dViaHdrCount=count;
			
			/* Get first via header from the list */
			if(sip_listGetAt(&(pGenHdr->slViaHdr), 0, \
					(SIP_Pvoid *) &pViaHeader , pErr) != SipFail)
			{
				SIP_S8bit *pViaBranch;
				/* Get Branch parameter */
				if(__sip_txn_getBranchFromViaHdr(pViaHeader, &pViaBranch,\
								pErr) !=SipFail)
				{
					pKey->pViaBranch = (SIP_S8bit *)STRDUPNETWORK(pViaBranch);
					if(pKey->pViaBranch == SIP_NULL)
					{
						*pErr = E_NO_MEM;
/*CSR_1-4868194 starts*/		 
                                                sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/
						return SipFail;
					}
				}
			}
		}
	}

	/*PRACK related stuff*/
	/* Check Only in the case of Request Message */
#ifdef SIP_RPR
	if( dType == SipMessageRequest )
	{
		SipRackHeader *pRackHdr;
		/* If Rack Header is present copy the values */
		pRackHdr = pReqHdr->pRackHdr;
		if(pRackHdr!=SIP_NULL)
		{
			if(sip_initSipRackHeader(&((pKey)->pRackHdr),\
				 pErr)==SipFail)
/*CSR_1-4868194 starts*/		 
                        {
                             sip_freeSipTxnKey(pKey);
/*CSR_1-4868194 ends*/  
                             return SipFail;
                        }
			pKey->pRackHdr->dIntVar1 = pRackHdr->dIntVar1;
			pKey->pRackHdr->dIntVar2 = pRackHdr->dIntVar2;
			pKey->pRackHdr->pStr1 = (SIP_S8bit *)STRDUPNETWORK(pRackHdr->\
							pStr1);
		}
		else
		{
				pKey->pRackHdr = SIP_NULL;
		}
	}
#endif

	SIPDEBUGFN("Exiting from sip_txn_createTxnKey");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}

/*************************************************************************************
**** FUNCTION:sip_txn_freeTxnKey
****
****
**** DESCRIPTION:
*************************************************************************************/
SipBool sip_txn_freeTxnKey
#ifdef ANSI_PROTO
	(SipTxnKey *pTxnKey, SipError *pErr)
#else
	(pTxnKey, pErr)
	SipTxnKey *pTxnKey;
	SipError *pErr;

#endif
{

	SIPDEBUGFN("Entering into sip_txn_freeTxnKey");

	/*Check the validity of the params*/
	/*Check for the validity of the i/p params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if (pTxnKey==SIP_NULL)
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	/*This does not take any params*/
	sip_freeSipTxnKey(pTxnKey);

	SIPDEBUGFN("Exiting from sip_txn_freeTxnKey");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
**** FUNCTION:sip_txn_initSipTxnContext
****
****
**** DESCRIPTION:
*******************************************************************************/
SipBool sip_txn_initSipTxnContext
#ifdef ANSI_PROTO
	(SipTxnContext **ppTxnContext, SipError *pErr)
#else
	(ppTxnContext, pErr)
	SipTxnContext **ppTxnContext;
	SipError *pErr;
#endif
{
	SipTimeOutValues *pTimeOutValues=SIP_NULL;

	SIPDEBUGFN("Entering into sip_txn_initSipTxnContext");

	/*Do a validity check on the i/p params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if (ppTxnContext==SIP_NULL)
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	/*Allocate memory for the TxnContext variable*/
	*ppTxnContext= ( SipTxnContext * ) \
					fast_memget(TXN_MEM_ID, sizeof(SipTxnContext), pErr);

	if(*ppTxnContext==SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	/*Initialise the components*/
	(*ppTxnContext)->pEventContext=SIP_NULL;

	(*ppTxnContext)->dTxnType=SipUATypeTxn;

	(*ppTxnContext)->txnOption.dTimerOption=0;
	(*ppTxnContext)->txnOption.dTimeoutCbkOption=SIP_ISSUE_CBKFOR_ALLTIMERS;
	(*ppTxnContext)->txnOption.dOption.dOption=0;

	pTimeOutValues=&((*ppTxnContext)->timeoutValues);

	pTimeOutValues->dT1=0;
	pTimeOutValues->dT2=0;
	pTimeOutValues->dTimerB=0;
	pTimeOutValues->dTimerC=0;
	pTimeOutValues->dTimerCr=0;
	pTimeOutValues->dTimerD_T3=0;
	pTimeOutValues->dTimerF_T3=0;
	pTimeOutValues->dTimerH=0;
	pTimeOutValues->dTimerI_T4=0;
	pTimeOutValues->dTimerJ_T3=0;
	pTimeOutValues->dTimerK_T4=0;

	SIPDEBUGFN("Exiting from sip_txn_initSipTxnContext");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}
/***************************************************************************
**** FUNCTION:sip_txn_resetTimerInTxn
****
****
**** DESCRIPTION:
**** This function will reset the timer for the transaction whose key is
**** passed.  This fn doesnt care what timer is there in the txn, it just resets 
**** the timer if there was one.
**** It informs what timer was running to the application.
**** Parameters :-
****	1. Txn Key :- Txn Key of the Txn whose timer has to be restarted
****    2. Txn Context :- Txn Context of Txn whose timer has to be restarted
****    3. New Timer Value :- Value of timer which is to be reset
****    4. Timer Type which will be given back :- timer which was stopped
****    5. Error :- error if any will be given back by this parameter
**** Return Value :-
****   SipSuccess :- if the timer is reset successfully
****   SipFail    :- if the timer can not be reset properly and error is
****                 returned in pError parameter
******************************************************************************/
SipBool sip_txn_resetTimerInTxn
#ifdef ANSI_PROTO
(
	SipTxnKey *pTxnKey,
	SipTxnContext	*pTxnContext,
	SIP_U32bit newTimeoutVal,
	en_SipTimerType *pTimerType,
	SipError *pError
)
#else
	(pTxnKey,pTxnContext,newTimeoutVal,pTimerType,pError)
	SipTxnKey *pTxnKey;
	SipTxnContext	*pTxnContext;
	SIP_U32bit newTimeoutVal;
	en_SipTimerType *pTimerType;
	SipError *pError;
#endif
{
	SipBool dResult;
	SIP_S8bit dCreate=0,dRemove=0;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipEventContext	*pEventContext=SIP_NULL;
	SIP_Pvoid pContextData=SIP_NULL;
	SipTxnTimeoutData * pTxnTimeoutData = SIP_NULL ; 
	SipTxnKey *pTempKey=SIP_NULL;

	SIPDEBUGFN("Entering sip_txn_resetTimerInTxn ") ;
	/*Check for the validity of the i/p parameters*/
	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if (pTxnKey==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	if (pTxnContext==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}
	
	/*
	 * Get a reference to the event context in the txnContext
	 * from which the context data will be then extracted
	 */
	pEventContext=pTxnContext->pEventContext;

	if (pEventContext!=SIP_NULL)
	{
		pContextData=pEventContext->pData;	
	}
	
	/*First fetch the txn*/
	/*Call a fetch txn using this key, without O_CREATE*/
	dResult=FETCH_TXN((SIP_Pvoid *)(&pTxnBuffer),pTxnKey, \
				pContextData,dCreate,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}
	/* Give back the timer which was running */
	*pTimerType = pTxnBuffer->dTimerStarted ;
	/* Stop the timer  */
	if (STOP_TIMER(pTxnBuffer->pTimerHandle,
			(SIP_Pvoid*)&pTxnTimeoutData,pError) == SipFail)
	{
		dResult=RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError);
			 return SipFail ;
	}
	FREE_TIMER_HANDLE(pTxnBuffer->pTimerHandle) ;
	pTxnBuffer->pTimerHandle = SIP_NULL ;

	dResult = START_TIMER(newTimeoutVal,(TimeoutFuncPtr)timerTimeOut,
			(SIP_Pvoid)pTxnTimeoutData,
			(SIP_Pvoid*)&(pTxnBuffer->pTimerHandle),pError) ;
	
	if (dResult	== SipFail)
	{
		dResult=RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError);
		return SipFail ;
	}

  
	dResult = RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError) ;
	
	if (dResult	== SipFail)
			return SipFail ;

	SIPDEBUGFN("Exiting from sip_txn_resetTimerInTxn ") ;
 return SipSuccess ;	
}


#ifdef __cplusplus
}
#endif
