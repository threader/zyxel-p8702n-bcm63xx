/**********************************************************************
 ** FUNCTION:
 **            Handles internal txn-layer functionality
 *********************************************************************
 ** FILENAME:
 ** txndecodeintrnl.c
 **
 ** DESCRIPTION: This file contains the txn layer related internal functions
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 **
 ** 19/03/2002  mahesh 	                               Initial Version
 ** 05-Jun-2009   Anuradha Gupta         SPR 19737       Klocworks Warning Removal
 ** 25-Oct-2009   Pankaj Negi            SPR 21076       Klocworks Warning Removal
 **
 **      Copyright (C) 2009 Aricent Inc . All Rights Reserved
 *********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <ctype.h>
#include "microsip_common.h"
#include "microsip_list.h"
#include "microsip_statistics.h"
#include "microsip_txndecode.h"
#include "microsip_txndecodeintrnl.h"
#include "microsip_txntimer.h"
#include "microsip_timer.h"
#include "microsip_txnclone.h"
#include "microsip_txninit.h"
#include "microsip_txnfree.h"
#include "microsip_trace.h"
#include "microsip_accessors.h"
#include "microsip_header.h"

#ifdef __cplusplus
}
#endif


#define MIN(a,b) (a < b) ? a : b

#ifdef __cplusplus
extern "C"
{
#endif
#define FETCH_TXN(a,b,c,d,e) sip_cbk_fetchTxn(a,b,c,d,e)
#define RELEASE_TXN(a,b,c,d,e,f) sip_cbk_releaseTxn(a,b,c,d,e,f)
#define START_TIMER(a,b,c,d,e) fast_startTxnTimer(a,b,c,d,e)
#define STOP_TIMER(a,b,c) fast_stopTxnTimer(a,b,c)
#define INDICATE_TIMEOUT(a,b) sip_indicateTimeOut(a,b)
#define SEND_TO_NETWORK(a,b,c,d,e)	sip_sendToNetwork(a,b,c,d,e)
#define FREE_EVENT_CONTEXT(a)	sip_freeEventContext(a)
#define FREE_TIMER_HANDLE(a)	sip_freeTimerHandle(a)


/*
 * It must be possible for applications to set the timer values once and
 * for these values to become applicable for all of the transactions. For
 * this reason there is a set of global variable provided. In case the
 * application decides to modify the values contained in these then these
 * values are then used for all transactions. However in case for any
 * particular transaction the application decides to modify the timer
 * values by passing these in TxnContext, then the values in the TxnContext
 * override the values in the global variables.
 */
SIP_U32bit		glbT1=SIP_DEFAULT_T1;
SIP_U32bit		glbT2=SIP_DEFAULT_T2;
SIP_U32bit		glbTimerB=SIP_DEFAULT_B;
SIP_U32bit		glbTimerC=SIP_DEFAULT_C;
SIP_U32bit		glbTimerCr=SIP_DEFAULT_Cr;
SIP_U32bit		glbTimerD_T3=SIP_DEFAULT_D_T3;
SIP_U32bit		glbTimerF_T3=SIP_DEFAULT_F_T3;
SIP_U32bit		glbTimerH=SIP_DEFAULT_H;
SIP_U32bit		glbTimerI_T4=SIP_DEFAULT_I_T4;
SIP_U32bit		glbTimerJ_T3=SIP_DEFAULT_J_T3;
SIP_U32bit		glbTimerK_T4=SIP_DEFAULT_K_T4;


/*
 * This is an array of function pointers indexed on the various states
 * that a Txn can be in, when the state of the txn changes these functions
 * are invoked , all timer related functionality gets (de)activated thru
 * these functions alone.
 */
SipBool (*glbSipTxnStateChange[TXNSTATES])(SipTxnBuffer *,SipTxnKey *, \
							SIP_Pvoid pContextData,SipTxnInformn *,SipError *) =
{
	sip_txn_InvClCallingState,
	sip_txn_InvClProceedingState,
	sip_txn_InvClFinalResponseRecvState,
   	sip_txn_InvClCompletedState,

	sip_txn_InvSrvProceedingState,
	sip_txn_InvSrv1xxSentState,
	sip_txn_InvSrv2xxForwardedState,
	sip_txn_InvSrvCompletedState,
	sip_txn_InvSrvConfirmedState,

	sip_txn_NonInvClTryingState,
	sip_txn_NonInvClProceedingState,
	sip_txn_NonInvClCompletedState,

	sip_txn_NonInvSrvTryingState,
	sip_txn_NonInvSrvProceedingState,
	sip_txn_NonInvSrvCompletedState,

	sip_txn_NonInvClientRPRRecvngState,
	sip_txn_NonInvServerRPRSendingState,

	sip_txn_NonInvClientRPRCompletedState,
	sip_txn_NonInvServerRPRCompletedState,

	sip_txn_TerminatedState,
	sip_txn_SrvBypassState

};

/***************************************************************************
**** FUNCTION:sip_isTxnAnRPR
****
****
**** DESCRIPTION:Depending on the messsage that was sent or recvd, this
****			function determines whether the Txn Class is of the RPR type.
******************************************************************************/
SipBool sip_isTxnAnRPR
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo
)
#else
	(pTxnInfo)
	SipTxnInformn *pTxnInfo;
#endif
{
	SIP_U32bit dCodeNum=0;
	
	SIPDEBUGFN("Entering function sip_isTxnAnRPR");
	
	/*In case it is an RPR we can return immediately*/
	if  (pTxnInfo->dMesgType==SipMessageResponse)
	{
		/*Check for the presence of an RSeq hdr*/
		dCodeNum = pTxnInfo->pSipMessage->u.pResponse->pStatusLine->dIntVar1;

#ifdef SIP_RPR
        if ((pTxnInfo->pSipMessage->u.pResponse->pResponseHdr->pRSeqHdr \
			!=SIP_NULL)&&(dCodeNum > 100)&&(dCodeNum < 200))
#endif
		{
			return SipSuccess;
		}
	}
	
	SIPDEBUGFN("Exiting function sip_isTxnAnRPR");
	return SipFail;
}

/*****************************************************************************
 ** FUNCTION: sip_checkMandatoryTxnHeaders
 *****************************************************************************
 ** Checks to see if message has mandatory some mandatory headers
 ** Currently checks for Call-Id, CSeq, From ant To headers so that the timer
 ** module that uses these doesnt throw up. Does not look for all the
 ** mandatory headers which differ depending on the message dType.
 ****************************************************************************/
SipBool sip_checkMandatoryTxnHeaders
#ifdef ANSI_PROTO
        (SipMessage *s, en_SipTxnType dTxnType)
#else
        (s,dTxnType)
        SipMessage *s;
		en_SipTxnType	dTxnType;
#endif
{
	SIP_U32bit MissingHdrCount =0;

	if (s->dType == SipMessageRequest)
	{
		if((s->pGeneralHdr->pCallidHdr == SIP_NULL))
			MissingHdrCount++;
		if((s->pGeneralHdr->pCseqHdr == SIP_NULL))
			MissingHdrCount++;
		if((s->pGeneralHdr->pFromHeader == SIP_NULL))
			MissingHdrCount++;
		if((s->pGeneralHdr->pToHdr == SIP_NULL))
			MissingHdrCount++;
#ifdef SIP_RPR
		if(sip_strcmp(s->u.pRequest->pRequestLine->pStr1,"PRACK")==0)
		{
			/*
			 * If the method is a PRACK then we need to have 
			 * a RAck header in the message. But in case of a Proxy
			 * type of a txn we do not need to put in this check
			 * since for a proxy we do not do PRACK related
			 * handling
			 */
			if (dTxnType==SipUATypeTxn)
			{
				if (s->u.pRequest->pRequestHdr->pRackHdr == SIP_NULL)
					MissingHdrCount++;
			}
		}
#endif

		if ( MissingHdrCount != 0 )
		{
			/*s->dIncorrectHdrsCount += MissingHdrCount;
			 *SipMessage of microsip doesnt have this structure .
			 */
			sip_error(SIP_Minor,(SIP_S8bit *)\
							"Message does not contain Mandatory Headers");
			return SipFail;
		}
	}
	else
	{
		if((s->pGeneralHdr->pCallidHdr == SIP_NULL))
			MissingHdrCount++;
		if((s->pGeneralHdr->pCseqHdr == SIP_NULL))
            MissingHdrCount++;
		if((s->pGeneralHdr->pFromHeader == SIP_NULL))
			MissingHdrCount++;
		if((s->pGeneralHdr->pToHdr == SIP_NULL))
			MissingHdrCount++;
		if ( MissingHdrCount != 0 )
		{
			/*s->dIncorrectHdrsCount += MissingHdrCount;
			 *SipMessage of microsip doesnt have this structure .
			 */
			sip_error(SIP_Minor,(SIP_S8bit *)\
							"Message does not contain Mandatory Headers");
			return SipFail;
		}
	}
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_getTxnClass
****
****
**** DESCRIPTION:Depending on whether a messsage was sent or recvd, this
****			function determines the Txn Class Inv-Server,In-Client etc
****			en_SipTxnClass is the OUT param in which this info is filled
******************************************************************************/
SipBool sip_getTxnClass
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnAPICalled dAPICalled,
	en_SipTxnClass *pTxnClass
)
#else
	(pTxnInfo,dAPICalled,pTxnClass)
	SipTxnInformn *pTxnInfo;
	en_SipTxnAPICalled dAPICalled;
	en_SipTxnClass *pTxnClass;
#endif
{
	SIPDEBUGFN("Entering function sip_getTxnClass");

	switch(dAPICalled)
	{
		case SipTxnMesgSent:
		{
			switch (pTxnInfo->dMesgType)
			{
				case SipMessageRequest:
				{
					/*Has to be Client Txn , question is Invite or NonInvite*/
					if ((pTxnInfo->dMethodType==SipTxnMethodInvite) \
						|| (pTxnInfo->dMethodType==SipTxnMethodAck))
					{
						*pTxnClass=SipInviteClient;
					}
					else
						*pTxnClass=SipNonInviteClient;
					break;
				}

				case SipMessageResponse:
				{
					/*Has to be Server Txn, Invite or NonInvite*/
					if ((pTxnInfo->dMethodType==SipTxnMethodInvite) \
						|| (pTxnInfo->dMethodType==SipTxnMethodAck))
					{
						*pTxnClass=SipInviteServer;
					}
					else
						*pTxnClass=SipNonInviteServer;
					break;
				}

				default:return SipFail;
			}
			break;
		}
		case SipTxnMesgRecvd:
		{
			switch (pTxnInfo->dMesgType)
			{
				case SipMessageRequest:
				{
					/*Has to be Server Txn , question is Invite or NonInvite*/
					if ((pTxnInfo->dMethodType==SipTxnMethodInvite) \
						|| (pTxnInfo->dMethodType==SipTxnMethodAck))
					{
						*pTxnClass=SipInviteServer;
					}
					else
						*pTxnClass=SipNonInviteServer;
					break;
				}

				case SipMessageResponse:
				{
					/*Has to be Client Txn, Invite or NonInvite*/
					if ((pTxnInfo->dMethodType==SipTxnMethodInvite) \
						|| (pTxnInfo->dMethodType==SipTxnMethodAck))
					{
						*pTxnClass=SipInviteClient;
					}
					else
						*pTxnClass=SipNonInviteClient;
					break;
				}
				case SipMessageAny:
					break;
			}
		}
	}
	SIPDEBUGFN("Exitting function sip_getTxnClass");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_captureTxnInformn
****
****
**** DESCRIPTION:This API obtains all necessary information abt the
	sent/decoded message and fills up in a structure. This structure is passed
	around , thus removing the necesity of passing different bits of informn
	as separate parameters to functions.
******************************************************************************/
SipBool sip_captureTxnInformn
#ifdef ANSI_PROTO
(
	SipMessage *pMessage,
	SIP_S8bit *pMsgBuffer,
	SIP_U32bit dBufLen,
	SipTxnContext *pContext,
	SipTranspAddr *pAddr,
	SIP_S8bit	dTranspType,
	SipTxnInformn *pTxnInfo
)
#else
	(pMessage,pMsgBuffer,dBufLen,pContext,pAddr,dTranspType,pTxnInfo)
	SipMessage *pMessage;
	SIP_S8bit *pMsgBuffer;
	SIP_U32bit dBufLen;
	SipTxnContext *pContext;
	SipTranspAddr *pAddr;
	SIP_S8bit	dTranspType;
	SipTxnInformn *pTxnInfo;
#endif
{
	SIP_U16bit dCodenum=0;
	SIP_S8bit *pMethod=SIP_NULL;


	SIPDEBUGFN("Entering function sip_captureTxnInformn");

	/*Is it a request or a response*/
	pTxnInfo->dMesgType = pMessage->dType;

	/*Set the reference to SipMessage*/
	pTxnInfo->pSipMessage=pMessage;

	/*Set the reference to message buffer*/
	pTxnInfo->pMsgBuffer=pMsgBuffer;

	/*Set the buf length*/
	pTxnInfo->dMessageLen=dBufLen;

	/*Set the transport type*/
	pTxnInfo->dTranspType=dTranspType;

	/*Set the Txn Context*/
	pTxnInfo->pTxnContext=pContext;

	/*Set the TranspAddr*/
	pTxnInfo->pAddr=pAddr;

	/*Get the actual method*/
	if(pMessage->dType == SipMessageRequest)
    {
		pMethod = pMessage->u.pRequest->pRequestLine->pStr1;
		if ( sip_strcmp(pMethod,pMessage->pGeneralHdr->pCseqHdr->pStr1) != 0 )
		{
			SIPDEBUG("Invalid message Cseq Method doesn't match with the" \
						" actual method");
			return SipFail;
		}
	}
	else
	{
		pMethod = pMessage->pGeneralHdr->pCseqHdr->pStr1;
	}

	if (pMethod==SIP_NULL)
	{
		return SipFail;
	}

	/*Copy the method of the message into an internal array*/
	if(sip_strlen(pMethod) <= SIP_TXN_MAX_METHOD_SIZE)
	{
		(void)sip_strcpy(pTxnInfo->methodName,pMethod);
	}
	else
	{
		(void)sip_strcpy(pTxnInfo->methodName,(SIP_S8bit*)"UNKNOWN");
	}


	/*Get the method type*/
	if (sip_strcmp(pMethod,(SIP_S8bit*)"INVITE")==0)
	{
		pTxnInfo->dMethodType=SipTxnMethodInvite;
	}else if (sip_strcmp(pMethod,(SIP_S8bit*)"PRACK")==0)
	{
		pTxnInfo->dMethodType=SipTxnMethodPrack;
	}else if (sip_strcmp(pMethod,(SIP_S8bit*)"ACK")==0)
	{
		pTxnInfo->dMethodType=SipTxnMethodAck;
	}else
	pTxnInfo->dMethodType=SipTxnMethodNonInvite;


	/*If it is a response then get the response class*/
	if(pMessage->dType == SipMessageResponse)
	{
		dCodenum=pMessage->u.pResponse->pStatusLine->dIntVar1;

		if(dCodenum<100)
		{
			pTxnInfo->dRespClass=SipResponseInvalid;
		}
		else if ((dCodenum>=100)&&(dCodenum<200))
       	{
			pTxnInfo->dRespClass=SipResponse1xx;
		}
		else if ((dCodenum>=200)&&(dCodenum<300))
		{
			pTxnInfo->dRespClass=SipResponseFinal2xx;
		} 
		else /* any response >=300 */
		{
			pTxnInfo->dRespClass=SipResponseFinalNon2xx;
		}
	}

	pTxnInfo->dRespCode=dCodenum;

	SIPDEBUGFN("Exitting function sip_captureTxnInformn");

	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:__sip_txn_getBranchFromViaHdr
****
****
**** DESCRIPTION:A purely internal function which does what the fn name says.
******************************************************************************/
SipBool __sip_txn_getBranchFromViaHdr
#ifdef ANSI_PROTO
	(SipViaHeader *pViaHeader,\
	 SIP_S8bit **ppBranch,\
	 SipError *pErr)
#else
	(pViaHeader,ppBranch,pErr)
	SipViaHeader *pViaHeader;
	SIP_S8bit **ppBranch;
	SipError *pErr;
#endif
{
	SIP_U32bit dIndex, size;

	*pErr = E_NO_ERROR;

	if(sip_listSizeOf(&(pViaHeader->slParam), &size, pErr) == SipFail)
		return SipFail;
	dIndex = 0;
	while (dIndex < size)
	{
		SipParam *pParam;

		if(sip_listGetAt(&(pViaHeader->slParam), dIndex, ((SIP_Pvoid *) &pParam)\
			, pErr) == SipFail)
			return SipFail;
		if(pParam->pName != SIP_NULL)
		{
			/* changes for CSR_1-5436128 Starts */
			if(sip_strcasecmp(pParam->pName,(SIP_S8bit*)"branch")==0)
			/* changes for CSR_1-5436128 Ends */            
			{
				SIP_S8bit *pBranch;

				if(sip_listGetAt(&(pParam->slValue),0,((SIP_Pvoid *) &pBranch),\
					pErr) != SipFail)
				{
					*ppBranch = pBranch;
					return SipSuccess;
				}
			}
		}
		dIndex++;
	}
	return SipFail;
}

/***************************************************************************
**** FUNCTION:sip_txn_sendBuffer
****
****
**** DESCRIPTION:This function is used by the Txn-Layer to actually
****			send the buffer across without making use of
****			sip_sendMessage, it needs to increment the statistics
****			however.
******************************************************************************/
SipBool sip_txn_sendBuffer
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipBool	dResult=SipFail;
	SIP_S8bit *pBuffer=SIP_NULL;
	SIP_U32bit dLength=0;
	SipTranspAddr *pAddr=SIP_NULL;
	SIP_S8bit dTranspType=0;

	SIPDEBUGFN("Entering function sip_txn_sendBuffer");

	/*Extract information from pTxnInfo*/
	pBuffer=pTxnInfo->pMsgBuffer;
	dLength=pTxnInfo->dMessageLen;
	pAddr=pTxnInfo->pAddr;
    
	/* CSR 1-6466704 Change Start */
	if(pTxnInfo->pAddr!=SIP_NULL)
	{
		HSS_LOCKEDINCREF(pTxnInfo->pAddr->dRefCount);
		pAddr->pData = SIP_NULL;
	}
	/* CSR 1-6466704 Change End */    
    /* klocworks : in case pAddr is NULL then do not proceed
     * as it is accessed across multiple places and will 
     * result into erroneous condition if not available.
     */ 
	else
		return SipFail;
    
	dTranspType=pTxnInfo->dTranspType;
	
	if (pTxnInfo->pTxnContext != SIP_NULL)
	{
        if(pTxnInfo->pTxnContext->pEventContext != SIP_NULL)
		{
			pAddr->pData = pTxnInfo->pTxnContext->pEventContext->pData;
		}
	}
	/*Call sendToNetwork to actually send the message*/

	dResult=SEND_TO_NETWORK(pBuffer, dLength,\
			 pAddr, dTranspType, pError);

	if (dResult==SipFail)
	{
        /* CSR 1-6466704 Change Start */
        (void)sip_freeSipTranspAddr(pTxnInfo->pAddr);
        /* CSR 1-6466704 Change End */
		return SipFail;
	}
    
    /* CSR 1-6466704 Change Start */
    (void)sip_freeSipTranspAddr(pTxnInfo->pAddr);
    /* CSR 1-6466704 Change End */

	/*Message is sent, now increment the stats*/
	if(pTxnInfo->dMesgType == SipMessageResponse)
	{
		INC_API_RESP_SENT
	}
	else if(pTxnInfo->dMesgType == SipMessageRequest)
	{
		INC_API_REQ_SENT
	}

#ifdef SIP_TRACE
	(void)sip_trace(SIP_Brief,SIP_Outgoing,(SIP_S8bit *) "Message sent out.");
	(void)sip_trace(SIP_Detailed,SIP_Outgoing,pBuffer);
#endif

	SIPDEBUGFN("Exiting from function sip_txn_sendBuffer");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_changeTxnStateAndRelease
****
****
**** DESCRIPTION:This function takes a txn buffer, invokes a state change on
**** that and releases the TxnBuffer.After that it checks if the State of the
**** buffer has changed to SipTxnTerminatedState, in which case it calls the
**** sip_releaseTxn with the TXN_REMOVE flag.
******************************************************************************/
SipBool sip_changeTxnStateAndRelease
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnKey 	*pTxnKey,
	en_SipTxnState dTxnState,
	SipTxnInformn *pTxnInfo,
	SIP_Pvoid	pContextData,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnKey,dTxnState,pTxnInfo,pContextData,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnKey *pTxnKey;
	en_SipTxnState dTxnState;
	SipTxnInformn *pTxnInfo;
	SIP_Pvoid	pContextData;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipTxnKey *pTempKey=SIP_NULL;
	SIP_S8bit	dRemove=0;
	SipBool	dRemoveRPRs=SipFail;
	

	SIPDEBUGFN("Entering function sip_changeTxnStateAndRelease");

    /* klocwork: In case pContext Data is not available then
     * do not proceed further as the absence of Context information
     * call object is not available which is updated in case of
     * change in transaction state or removing the transaction
     */ 
    if(SIP_NULL == pContextData)
    {
        return SipFail;
    }

	/*Some specific processing is necessarry for the client txns*/
	if ((dTxnState==SipTxnInvClientFinalResponseRecvState) ||
			(dTxnState == SipTxnInvServerCompletedState ))
	{
		if (pTxnInfo!=SIP_NULL)
		{
			/*
			 * Add the actual response code that caused this change into 
			 * the TxnBuffer for later use
			 */
			pTxnBuffer->dRespCode=pTxnInfo->dRespCode;
		}
	}

	if(pTxnInfo != SIP_NULL)
		pTxnInfo->pMsgBuffer=pTxnBuffer->pBuffer;

	/*
	 * In case of the invoked API having had been sip_txn_sendMessage()
	 * we have not yet sent the message even for the first time, that 
	 * needs to be done now since all the confirmn checks have been done
	 */
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled==SipTxnMesgSent)
		{
			pTxnInfo->pMsgBuffer=pTxnBuffer->pBuffer;
			dResult=sip_txn_sendBuffer(pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/* 
				 * sendBuffer Failed ,remove the Txn created for this
				 * and free the key. If the reference is passed to the
				 * application decrement the ref count. Make the BUffer
				 * Reference to NULL we will free in sip_txn_sendMessage
				 * anyway
				 */
				SipError tempError; 
				tempError = E_NO_ERROR;
				pTxnBuffer->pBuffer = SIP_NULL;
				if(sip_removeTxnNFreeKey\
					(pTxnBuffer, pTxnKey, pTxnInfo, &tempError) == SipFail)
					return SipFail;
				return SipFail;
			}
            /* CSR_1-7760113 START 
			 IPTK CSR 1-7757056. Clear the txn key for CANCEL request. When any new CANCEL request comes,
			 then it should be treated as a new transaction*/      
      		else
	  		{
			  if ((sip_strcmp(pTxnBuffer->pTxnKey->pMethod,"CANCEL") == 0)&& (pTxnInfo->dRespClass == SipResponseFinalNon2xx))
			  {
					  SipError tempError; 
					  tempError = E_NO_ERROR;
						
					  if(sip_removeTxnNFreeKey\
									  (pTxnBuffer, pTxnKey, pTxnInfo, &tempError) == SipFail)
							  return SipFail;

					  pTxnInfo->dMesgSentAtLeastOnce=SipSuccess;
 					  
					  pTxnInfo->pMsgBuffer=SIP_NULL;
	                  FREE_EVENT_CONTEXT(pTxnInfo->pTxnContext->pEventContext);

				  return SipSuccess;
			  } 
	  		}
            /* CSR_1-7760113 END */
			pTxnInfo->dMesgSentAtLeastOnce=SipSuccess;
			pTxnInfo->pMsgBuffer=SIP_NULL;
		}
	}
	
	/*
	 * This information is critical for us so that we may remove the RPRs
	 * later on. The state maintained within the Vbuffer itself cannot be
	 * used since that will get overwritten by the glbSipTxnStateChange fn
	 */
	if (dTxnState==SipTxnInvClientCompletedState) dRemoveRPRs=SipSuccess;

	/*
	 * In case the txn is being pushed to the terminated state by
	 * abortTxn , even in that case we need to remove the RPRs
	 */
	if (dTxnState==SipTxnTerminatedState) dRemoveRPRs=SipSuccess;
	

	dResult=glbSipTxnStateChange[dTxnState]\
					(pTxnBuffer,pTxnKey,pContextData,pTxnInfo,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Check the state of the txn, if it has moved to the terminated state*/
	if (pTxnBuffer->dTxnState==SipTxnTerminatedState)
	{

        dRemove=TXN_REMOVE;
		/*Now release the txn as well --with TXN_REMOVE*/
		dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
					(SIP_Pvoid *)(&pTxnBuffer),(SIP_Pvoid)pContextData,dRemove,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
		
		if (dRemoveRPRs)
		{	
			/* 
			 * Change the State of all RPR Txns to Terminated
			 * State
			 */
			if ((pTxnBuffer->dTxnClass==SipInviteClient)\
					&&\
				(pTxnBuffer->pslRPRTxnKeyQ!=SIP_NULL))
			{	
				if(sip_updateRPRTxnState(pTxnBuffer->pslRPRTxnKeyQ,\
						SipTxnTerminatedState,pContextData,pError) == SipFail)
				return SipFail;
			}	
		}
		
		/*First free the EventContext in the TxnBuffer*/
		if (pTxnBuffer->pEventContext!=SIP_NULL)
			FREE_EVENT_CONTEXT(pTxnBuffer->pEventContext);

		/*Set the EventContext in the TxnBuffer to NULL*/
		pTxnBuffer->pEventContext=SIP_NULL;
		

		/*Now free up the stored Txn and the Key also*/
		sip_freeSipTxnBuffer(pTxnBuffer);
		sip_freeSipTxnKey(pTempKey);
	}
	else
	{
		/*SipBool retVal= SipSuccess;*/
		do{
	    dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
					(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError);

		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}

		if (dRemoveRPRs)
		{	
			/* 
			 * Change the State of all RPR Txns to Terminated
			 * State
			 */
			if ((pTxnBuffer->dTxnClass==SipInviteClient)\
					&&\
				(pTxnBuffer->pslRPRTxnKeyQ!=SIP_NULL))
			{	
				if(sip_updateRPRTxnState(pTxnBuffer->pslRPRTxnKeyQ,\
						SipTxnTerminatedState,pContextData,pError) == SipFail)
					{
					/*	retVal = SipFail;	*/
						break;
#if 0					
				return SipFail;
#endif					
					}
			}	
		}
		
		if(pTxnBuffer->dTxnState == SipTxnInvClientFinalResponseRecvState )
		{
			/*
			 * At this point the context data that we will give will be that
			 * of the INVITE txn that had been maintained in the TxnBuffer
			 * itself
			 */
			SIP_Pvoid	pTempContextData=SIP_NULL;
			if (pTxnBuffer->pEventContext!=SIP_NULL)
			{	
				pTempContextData=pTxnBuffer->pEventContext->pData;
			}
			else
			{
				pTempContextData=pContextData;
			}
			
			if (pTxnBuffer->pslRPRTxnKeyQ != SIP_NULL)
			{
				/*
				 * Change the State of all RPR Txns to Completed
				 * State basically in short ...KILL ALL RPRs
				 */
				if(sip_updateRPRTxnState(pTxnBuffer->pslRPRTxnKeyQ,\
					SipTxnNonInvClientRPRCompletedState,pTempContextData,\
						pError) == \
							SipFail)
					{
						/*retVal = SipFail;	*/
						break;
#if 0					
					return SipFail;
#endif					
					}
			}
		}
		if(pTxnBuffer->dTxnState == SipTxnInvServerCompletedState)
		{
			/*
				 * At this point context data that we will give will again be
				 * that of the INVITE txn that had been maintained in TxnBuffer
			 * itself. It was given when the 2xx had been sent
			 */
			SIP_Pvoid	pTempContextData=SIP_NULL;
			
			/*
			 * Extract the SIP_Pvoid in the eventcontext of the
			 * passed TxnInformn
			 */
			sip_getContextDatafromTxnInfo(pTxnInfo,&pTempContextData);
			
			if(pTxnBuffer->pslRPRTxnKeyQ != SIP_NULL)
			{
				/* 
				 * Change the State of all RPR Txns to Completed
				 * State
				 */
				if(sip_updateRPRTxnState(pTxnBuffer->pslRPRTxnKeyQ,\
					SipTxnNonInvServerRPRCompletedState,pContextData,\
					pError) == \
						SipFail)
					{
						/*retVal = SipFail;	*/
						break;
#if 0					
					return SipFail;
#endif					
					}
			}
		}
		}while(0);
	}

	sip_freeSipTxnKey(pTxnKey);

	SIPDEBUGFN("Exitting function sip_changeTxnStateAndRelease");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_reassignbufferInTxnBuffer
****
****
**** DESCRIPTION:This function takes a txn buffer, frees the mesage buffer
****	contained within and reasigns it to the one supplied in TxnInformn
******************************************************************************/
SipBool sip_reassignbufferInTxnBuffer
#ifdef ANSI_PROTO
(
	SipTxnBuffer 	*pTxnBuffer,
	SipTxnInformn 	*pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnInformn 	*pTxnInfo;
	SipError *pError;
#endif
{
	SIP_S8bit	*pBuffer=SIP_NULL;
	SipBool		dResult=SipFail;

	SIPDEBUGFN("Entering function sip_reassignbufferInTxnBuffer");

	pBuffer=pTxnBuffer->pBuffer;
	if (pBuffer==SIP_NULL)
	{
		/*We can set the TxnBuffer's buffer to the one present in TxnInformn*/
		pTxnBuffer->pBuffer=pTxnInfo->pMsgBuffer;
		pTxnBuffer->dDirectOption=pTxnInfo->dDirectOption;
	}
	else
	{
		if (!pTxnInfo->dDirectOption)
		{
			dResult=fast_memfree(TXN_MEM_ID,pBuffer,pError);
			if (dResult==SipFail)
			{
				return SipFail;
			}
		}
		pTxnBuffer->pBuffer=pTxnInfo->pMsgBuffer;
		pTxnBuffer->dDirectOption=pTxnInfo->dDirectOption;
	}
	pTxnBuffer->dBufferLength = pTxnInfo->dMessageLen;

	/*
	 * Very important, now somebody else owns the buffer in TxnInfo,
	 * need to mark this fact
	 */
	pTxnInfo->pMsgBuffer=SIP_NULL;

	SIPDEBUGFN("Exitting function sip_reassignbufferInTxnBuffer");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_releaseTxnNFreeKey
****
****
**** DESCRIPTION:This function takes a txn buffer, invokes a sip_cbk_releaseTxn
****			on it and frees the incoming key.
******************************************************************************/
SipBool sip_releaseTxnNFreeKey
#ifdef ANSI_PROTO
(
	SipTxnBuffer 	*pTxnBuffer,
	SipTxnKey 		*pTxnKey,
	SipTxnInformn	*pTxnInfo,
	SipError 		*pError
)
#else
	(pTxnBuffer,pTxnKey,pTxnInfo,pError)
	SipTxnBuffer 	*pTxnBuffer;
	SipTxnKey 		*pTxnKey;
	SipTxnInformn	*pTxnInfo;
	SipError 		*pError;
#endif
{
	SipBool dResult;
	SipTxnKey *pTempKey=SIP_NULL;
	SIP_S8bit dRemove=0;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_releaseTxnNFreeKey");


	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	if (pTxnBuffer!=SIP_NULL)
	{
		/*Now release the txn as well --without TXN_REMOVE*/
#ifdef SIP_LINT
        dRemove=0;
#else
        dRemove=!TXN_REMOVE;
#endif

        /* klocworks: In case Context Data is not avaialable that means
         * call object information is not available , whiel releasing
         * the transaction, the pointer will be dereferenced
         */ 
		if(SIP_NULL != pContextData)
		{
		    dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
						(SIP_Pvoid *)(&pTxnBuffer),pContextData,dRemove,pError);
		}
		else
			dResult = SipFail;

		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	/*Now free up the Key */
	sip_freeSipTxnKey(pTxnKey);

	SIPDEBUGFN("Exitting function sip_releaseTxnNFreeKey");
	return SipSuccess;
}
/***************************************************************************
**** FUNCTION:sip_removeTxnNFreeKey
****
****
**** DESCRIPTION:This function takes a txn buffer, invokes a sip_cbk_releaseTxn
****			on it to remove the Txn and frees the incoming key.
******************************************************************************/
SipBool sip_removeTxnNFreeKey
#ifdef ANSI_PROTO
(
	SipTxnBuffer 	*pTxnBuffer,
	SipTxnKey 		*pTxnKey,
	SipTxnInformn	*pTxnInfo,
	SipError 		*pError
)
#else
	(pTxnBuffer,pTxnKey,pTxnInfo,pError)
	SipTxnBuffer 	*pTxnBuffer;
	SipTxnKey 		*pTxnKey;
	SipTxnInformn	*pTxnInfo;
	SipError 		*pError;
#endif
{
	SipBool dResult;
	SipTxnKey *pTempKey=SIP_NULL;
	SIP_Pvoid pContextData=SIP_NULL;
	SIPDEBUGFN("Entering function sip_removeTxnNFreeKey");
	
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	if (pTxnBuffer!=SIP_NULL)
	{
		/*Now release the txn as well --with TXN_REMOVE*/
		if(SIP_NULL != pContextData)
		{
		    dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
						(SIP_Pvoid *)(&pTxnBuffer),pContextData,TXN_REMOVE,\
						pError);
		}
		else
			dResult = SipFail;
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}
	if(pTxnInfo->pKeyOfCreatedTxn)
	{
		sip_freeSipTxnKey(pTempKey);
		pTxnInfo->pKeyOfCreatedTxn = SIP_NULL;
	}
	sip_freeSipTxnBuffer(pTxnBuffer);
	sip_freeSipTxnKey(pTempKey);

	/*Now free up the Key */
	sip_freeSipTxnKey(pTxnKey);

	SIPDEBUGFN("Exitting function sip_removeTxnNFreeKey");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_fetchTxnUsingMessage
****
****
**** DESCRIPTION:This function takes a SipMessage and creates a SipTxnKey
****	from it. It later invokes the sip_cbk_fetchTxn and fetches the txnBuffer
****	corresponding to the key.
******************************************************************************/
SipBool sip_fetchTxnUsingMessage
#ifdef ANSI_PROTO
(
	SipMessage *pMsg,
	SIP_S8bit dOpt,
	SipTxnInformn *pTxnInfo,
	SipTxnKey **ppTxnKey,
	SipTxnBuffer **ppTxnBuffer,
	SipError *pError
)
#else
	(pMsg,dOpt,pTxnInfo,ppTxnKey,ppTxnBuffer,pError)
	SipMessage *pMsg;
	SIP_S8bit dOpt;
	SipTxnInformn *pTxnInfo;
	SipTxnKey **ppTxnKey;
	SipTxnBuffer **ppTxnBuffer;
	SipError *pError;
#endif
{
	SipBool dResult;
	SIP_S8bit dCreate=0;
	SIP_Pvoid pContextData=SIP_NULL;
	en_SipTxnClass		dTxnClass;
	
	SIPDEBUGFN("Entering function sip_fetchTxnUsingMessage");

	/*First check if the key points to something,
		if so free it first*/
	if (*ppTxnKey!=SIP_NULL)
	{
		sip_freeSipTxnKey(*ppTxnKey);
	}
	
	dTxnClass=pTxnInfo->dTxnClass;
	/*Create the Txn-Key for this message*/
	dResult=sip_txn_createTxnKey(pMsg,dTxnClass,ppTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*We want to do a To-Tag check on the basis of the received param*/
	(*ppTxnKey)->dTagCheck=(SipBool)dOpt;

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Now call a fetch txn using this key, without TXN_CREAT*/
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif

if(SIP_NULL != pContextData)
	{
	    dResult= FETCH_TXN((SIP_Pvoid *)ppTxnBuffer,(SIP_Pvoid)(*ppTxnKey),\
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;

	if (dResult!=SipSuccess)
	{
		/*A txn corresponding to this key was not found*/
		*ppTxnBuffer=SIP_NULL;
	}

	/*Now ppTxnBuffer points to a member in the aggregation*/

	SIPDEBUGFN("Exitting function sip_fetchTxnUsingMessage");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_getContextDatafromTxnInfo
****
****
**** DESCRIPTION:This function obtains the void data present in the
****			eventcontext of the TxnInformation.
******************************************************************************/
void  sip_getContextDatafromTxnInfo
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SIP_Pvoid *ppData
)
#else
	(pTxnInfo,ppData)
	SipTxnInformn *pTxnInfo;
	SIP_Pvoid *ppData;
#endif
{
	*ppData=SIP_NULL;

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->pTxnContext!=SIP_NULL)
			if (pTxnInfo->pTxnContext->pEventContext!=SIP_NULL)
				*ppData=pTxnInfo->pTxnContext->pEventContext->pData;
	}
}

/***************************************************************************
**** FUNCTION:sip_createTxnUsingKey
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_createTxnUsingKey
#ifdef ANSI_PROTO
(
	SipTxnBuffer **ppTxnBuffer,
	SipTxnKey **ppTxnKey,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(ppTxnBuffer,ppTxnKey,pTxnInfo,pError)
	SipTxnBuffer **ppTxnBuffer;
	SipTxnKey	**ppTxnKey;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipTxnKey *pTempKey=SIP_NULL;
	SipTxnBuffer *pTempBuffer=SIP_NULL;
	SIP_S8bit	dCreate;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_createTxnUsingKey ");

	/*First init the Txn Buffer*/
	dResult=sip_initSipTxnBuffer(ppTxnBuffer,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		sip_freeSipTxnBuffer(*ppTxnBuffer);
		return SipFail;
	}

	/*Save the buffer in case we need to free it later*/
	pTempBuffer=*ppTxnBuffer;

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Now we need to add this buffer into the aggregation of TxnBuffers*/
	/*We set the TXN_CREAT flag*/
	dCreate=TXN_CREAT;

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while fetching 
     * the transaction, the pointer will be dereferenced
     */
    if(SIP_NULL != pContextData)
	{
	    dResult= FETCH_TXN((SIP_Pvoid *)ppTxnBuffer,(SIP_Pvoid)(*ppTxnKey),\
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;

	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		sip_freeSipTxnBuffer(*ppTxnBuffer);
		return SipFail;
	}

	/*
	 * The above fetch would either return with a newly created txn or 
	 * else will change the ppTxnBuffer we have passed to an already
	 * existing txn within the internal aggregation
	 */

	/*Check if a new txn got created or not*/
	if ((*ppTxnBuffer)->dTxnState==SipTxnDefaultState)
	{
		/*A new txn was created*/

		/*Set the reference of the key in the TxnBuffer*/
		(*ppTxnBuffer)->pTxnKey=*ppTxnKey;

		/*
		 * Add a reference to the TxnInfo as well after incrementing 
		 * the refcount
		 */
		HSS_LOCKEDINCREF((*ppTxnKey)->dRefCount);
		pTxnInfo->pKeyOfCreatedTxn=*ppTxnKey;

		pTempKey=*ppTxnKey;

		/*
		 * If a txn got created then we need to clone the key
		 * that this function had received since the key recvd
		 * has been added into the aggregn
		 */
		if(sip_initSipTxnKey(ppTxnKey,pError) == SipFail)
			return SipFail;

		if(sip_cloneSipTxnKey(*ppTxnKey,pTempKey,pError) == SipFail)
			return SipFail;

		/*Now the caller of this function can continue to use the Key*/
	}
	else
	{
		/*
		 * A new txn did not get created, this means the initial init on 
		 * the TxnBuffer has not served any purpose since this memory 
		 * has not been utilised
		 */
		sip_freeSipTxnBuffer(pTempBuffer);
	}

	SIPDEBUGFN("Exitting function sip_createTxnUsingKey ");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_remoteRetransmitBuffer
****
****
**** DESCRIPTION:This function remote-retransmits the buffer contained
****	within the Txn Buffer to the destination specified in the Txn Buffer
******************************************************************************/
SipBool sip_remoteRetransmitBuffer
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{

	SIP_S8bit *pBuffer=SIP_NULL;
	SIP_U32bit	dBufLength;
	SipBool	dResult;
#ifdef SIP_TRACE
	SIP_S8bit traceBuf[SIP_TRACE_BUF_SIZE];
	SIP_S8bit mesg[SIP_FIRST_20_BYTES];
#endif
  /* Changes for CSR1-5349648 */
  (void)pTxnInfo;
  /* Changes for CSR1-5349648 */

	SIPDEBUGFN("Entering function sip_remoteRetransmitBuffer");
	/*
	 * Use the buffer that is contained in the Txn Buffer and
	 * send it to the destination as specified in the TxnBuffer 
	 * contents
	 */
	pBuffer=pTxnBuffer->pBuffer;
	dBufLength=pTxnBuffer->dBufferLength;
	/*Call sendToNetwork callback to remote retransmit the buffer*/

	dResult=SEND_TO_NETWORK((SIP_S8bit *)(pBuffer), \
			dBufLength, pTxnBuffer->pAddr,pTxnBuffer->dTranspType, pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}
#ifdef SIP_TRACE
	(void)sip_strncpy (mesg, (SIP_S8bit *)pBuffer, 20);
	mesg[20]='\0';
	(void)HSS_SPRINTF (traceBuf,(SIP_S8bit*)"Message Remote Retranmsmitted \n" \
	 					"(1st 20 bytes) : \"%s\" ",mesg);
	(void)sip_trace(SIP_Brief, SIP_Outgoing, traceBuf);
#endif

	SIPDEBUGFN("Exiting from function sip_remoteRetransmitBuffer");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_populateTimeOutValues
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the initial data
****			Updates the TimeOut values for this txn.
****
******************************************************************************/
SipBool sip_populateTimeOutValues
#ifdef ANSI_PROTO
(
	SipTimeOutValues *pTimeoutValues,
	SipTxnInformn *pTxnInfo
)
#else
	(pTimeoutValues,pTxnInfo)
	SipTimeOutValues *pTimeoutValues;
	SipTxnInformn *pTxnInfo;
#endif
{
	SIP_U32bit dTimerOption=0;
	SipTimeOutValues *pUserProvided=SIP_NULL;

	SIPDEBUGFN("Entering function sip_populateTimeOutValues");

	/*Check validity of options first*/
	if (pTxnInfo==SIP_NULL)
	{
		return SipFail;
	}

    /* klcoworks: this piece of code leg has been moved up so that
     * the subsequent code is within the NULL check
     */ 
	dTimerOption=pTxnInfo->pTxnContext->txnOption.dTimerOption;
	pUserProvided=&(pTxnInfo->pTxnContext->timeoutValues);

     /*
	 * Now check for the bit mask that has been set in the Txn Info 
	 * and on that basis pick up the values from the TxnInfo
	 */
	/*First populate the timeOut values as the default values only*/
	if(pTimeoutValues!=SIP_NULL)
	{
		pTimeoutValues->dT1=glbT1;
		pTimeoutValues->dT2=glbT2;
		pTimeoutValues->dTimerB=glbTimerB;
		pTimeoutValues->dTimerC=glbTimerC;
		pTimeoutValues->dTimerCr=glbTimerCr;
		pTimeoutValues->dTimerD_T3=glbTimerD_T3;
		pTimeoutValues->dTimerF_T3=glbTimerF_T3;
		pTimeoutValues->dTimerH=glbTimerH;
		pTimeoutValues->dTimerI_T4=glbTimerI_T4;
		pTimeoutValues->dTimerJ_T3=glbTimerJ_T3;
		pTimeoutValues->dTimerK_T4=glbTimerK_T4;


	if (dTimerOption&SIP_OPT_TIMER_T1)
		if ( pUserProvided->dT1 )
		pTimeoutValues->dT1=pUserProvided->dT1;

	if ((dTimerOption&SIP_OPT_TIMER_T2)&&(pUserProvided->dT2))
		pTimeoutValues->dT2=pUserProvided->dT2;

	if ((dTimerOption&SIP_OPT_TIMER_B)&&(pUserProvided->dTimerB))
		pTimeoutValues->dTimerB=pUserProvided->dTimerB;

	if ((dTimerOption&SIP_OPT_TIMER_C)&&(pUserProvided->dTimerC))
		pTimeoutValues->dTimerC=pUserProvided->dTimerC;

	if ((dTimerOption&SIP_OPT_TIMER_Cr)&&(pUserProvided->dTimerCr))
		pTimeoutValues->dTimerCr=pUserProvided->dTimerCr;
		
	if ((dTimerOption&SIP_OPT_TIMER_D)&&(pUserProvided->dTimerD_T3))
		pTimeoutValues->dTimerD_T3=pUserProvided->dTimerD_T3;

	if ((dTimerOption&SIP_OPT_TIMER_F)&&(pUserProvided->dTimerF_T3))
		pTimeoutValues->dTimerF_T3=pUserProvided->dTimerF_T3;

	if ((dTimerOption&SIP_OPT_TIMER_H)&&(pUserProvided->dTimerH))
		pTimeoutValues->dTimerH=pUserProvided->dTimerH;

	if ((dTimerOption&SIP_OPT_TIMER_I)&&(pUserProvided->dTimerI_T4))
		pTimeoutValues->dTimerI_T4=pUserProvided->dTimerI_T4;

	if ((dTimerOption&SIP_OPT_TIMER_J)&&(pUserProvided->dTimerJ_T3))
		pTimeoutValues->dTimerJ_T3=pUserProvided->dTimerJ_T3;

	if ((dTimerOption&SIP_OPT_TIMER_K)&&(pUserProvided->dTimerK_T4))
		pTimeoutValues->dTimerK_T4=pUserProvided->dTimerK_T4;
	}


	SIPDEBUGFN("Exiting from function sip_populateTimeOutValues");

	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateTxnBuffer
****
****
**** DESCRIPTION:This function updates  the Txn Buffer with the transport
****			 related data
****
******************************************************************************/
SipBool sip_updateTxnBuffer
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipBool	dResult;

	SIPDEBUGFN("Entering function sip_updateTxnBuffer");

	/*Check for validity of the parameters*/

	if (pError==SIP_NULL)
	{
		return SipFail;
	}

	if ((pTxnInfo==SIP_NULL)||(pTxnBuffer==SIP_NULL))
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}

	/*Set the TranspAddr fields*/
	pTxnBuffer->dTranspType=pTxnInfo->dTranspType;
	
	/*Set the bitmask for the timer TimeOut callbacks*/
	if (pTxnInfo->pTxnContext!=SIP_NULL)
	{
		pTxnBuffer->dTimeoutCbkBitMask=pTxnInfo->pTxnContext->txnOption.\
			dTimeoutCbkOption;
	}
	else
		pTxnBuffer->dTimeoutCbkBitMask=SIP_ISSUE_CBKFOR_ALLTIMERS;

	
	/*Set the timeout values*/
	dResult=sip_populateTimeOutValues(&(pTxnBuffer->timeoutValues), \
				pTxnInfo);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*
	 * Check for NULLness of eventcontext if it is not NULL then we need
	 * to free what was earlier present
	 */
    /* klcoworks : NULL check has been added so that there is no dereferencing
     * of TxnContext in the code within the if blocks
     */ 
    if(SIP_NULL != pTxnInfo->pTxnContext)
    {
        if (pTxnBuffer->pEventContext!=SIP_NULL)
        {
            /*
             * Free only if the new reference is different from the old
             * reference in which case the application can reuse this 
             * context if we don't free this.
             */
            if(pTxnBuffer->pEventContext != pTxnInfo->pTxnContext->pEventContext)
            {
                FREE_EVENT_CONTEXT(pTxnBuffer->pEventContext);
                /*Set the EventContext in the TxnBuffer to NULL*/
                pTxnBuffer->pEventContext=SIP_NULL;
            }	
        }

        if (pTxnInfo->dAPICalled==SipTxnMesgSent) 
        {
            pTxnBuffer->pEventContext=pTxnInfo->pTxnContext->pEventContext;
            pTxnBuffer->dDirectOption=pTxnInfo->dDirectOption;
        }
    }
	

	/*Check if the Buffer already had a address, in case of CPP we need
	to free this address*/
    
    /* CSR 1-6466704 Change Start */
	if (pTxnBuffer->pAddr!=SIP_NULL)
	    sip_freeSipTranspAddr(pTxnBuffer->pAddr);
    /*{
		if (pTxnBuffer->pAddr->pHost!=SIP_NULL)
			(void)fast_memfree(0,pTxnBuffer->pAddr->pHost,SIP_NULL);
		(void)fast_memfree(0,pTxnBuffer->pAddr,SIP_NULL);
	}*/

    if(pTxnInfo->pAddr!=SIP_NULL)
      HSS_LOCKEDINCREF(pTxnInfo->pAddr->dRefCount);

    /* CSR 1-6466704 Change End */       

	pTxnBuffer->pAddr=pTxnInfo->pAddr;
	if ((pTxnInfo->pTxnContext != SIP_NULL)&&(pTxnInfo->pAddr!=SIP_NULL))
	{
		if (pTxnInfo->pTxnContext->pEventContext != SIP_NULL)
		{
			pTxnBuffer->pAddr->pData = \
				pTxnInfo->pTxnContext->pEventContext->pData;
		}
	}		
	pTxnBuffer->dEnableRetransCallback = SipFail;

#ifdef SIP_RETRANSCALLBACK
    /* klcoworks : NULL check has been added so that there is no dereferencing
     * of TxnContext in the code within the if blocks
     */
	if((pTxnInfo->pTxnContext != SIP_NULL) && 
            (pTxnInfo->pTxnContext->txnOption.dOption.dOption
		& SIP_OPT_RETRANSCALLBACK) == SIP_OPT_RETRANSCALLBACK)
		pTxnBuffer->dEnableRetransCallback = SipSuccess;
	else
		pTxnBuffer->dEnableRetransCallback = SipFail;
#endif

	*pError = E_NO_ERROR;


	SIPDEBUGFN("Exiting function sip_updateTxnBuffer");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_populateTxnBuffer
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the initial data
****			Used only after calling sip_cbk_fetchTxn(TXN_CREAT)
****			1>Updates the TimeOut values for this txn.
****			2>Sets the transport address fields
****			3>Sets the Event context.
****			4>Sets the dEnableRetransCallback value.
****			5>Sets the txn type(Proxy/UA)
****
******************************************************************************/
SipBool sip_populateTxnBuffer
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipEventContext *pEventContext=SIP_NULL;
	SipBool	dResult;

	SIPDEBUGFN("Entering function sip_populateTxnBuffer");

    /* Klockworks: Failure has been returned to avoid
     * dereferencing of TxnContext when NULL
     */ 
    if (pTxnInfo->pTxnContext == SIP_NULL)
        return SipFail;

	pEventContext=pTxnInfo->pTxnContext->pEventContext;

	/* 
	 * Set the reference to the user provided Event Context.
	 * This is to be done only if a message was sent
	 */
	if (pTxnInfo->dAPICalled==SipTxnMesgSent)
	{
		pTxnBuffer->pEventContext=pEventContext;
	}

	/*Set the Proxy/UA type field*/
	pTxnBuffer->dType=pTxnInfo->pTxnContext->dTxnType;

	/*Set the Txn Class*/
	pTxnBuffer->dTxnClass=pTxnInfo->dTxnClass;

	/*Set the TranspAddr fields*/
	pTxnBuffer->dTranspType=pTxnInfo->dTranspType;
	pTxnBuffer->pAddr=pTxnInfo->pAddr;

    /* CSR 1-6466704 Change Start */
    if(pTxnInfo->pAddr!=SIP_NULL)
        HSS_LOCKEDINCREF(pTxnBuffer->pAddr->dRefCount);
    /* CSR 1-6466704 Change End */       
	
	/*Set the bitmask for the timer TimeOut callbacks*/
	if (pTxnInfo->pTxnContext!=SIP_NULL)
	{
		pTxnBuffer->dTimeoutCbkBitMask=pTxnInfo->pTxnContext->txnOption.\
			dTimeoutCbkOption;
	}
	else
		pTxnBuffer->dTimeoutCbkBitMask=SIP_ISSUE_CBKFOR_ALLTIMERS;
		
	/*Set the timeout values*/
	dResult=sip_populateTimeOutValues(&(pTxnBuffer->timeoutValues), \
				pTxnInfo);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	pTxnBuffer->dEnableRetransCallback = SipFail;
	pTxnBuffer->dDirectOption=pTxnInfo->dDirectOption;

#ifdef SIP_RETRANSCALLBACK
    if((pTxnInfo->pTxnContext->txnOption.dOption.dOption
		& SIP_OPT_RETRANSCALLBACK) == SIP_OPT_RETRANSCALLBACK)
		pTxnBuffer->dEnableRetransCallback = SipSuccess;
	else
		pTxnBuffer->dEnableRetransCallback = SipFail;
#endif

	*pError = E_NO_ERROR;

	SIPDEBUGFN("Exiting from function sip_populateTxnBuffer");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_dropQueuedUpForProxy
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the retransmitted
****			requests
******************************************************************************/
SipBool sip_dropQueuedUpForProxy
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipError *pError
)
#else
	(pTxnBuffer,pError)
	SipTxnBuffer *pTxnBuffer;
	SipError *pError;
#endif
{
	SipList *pList=SIP_NULL;
	SipBool dResult;

	SIPDEBUGFN("Entering function sip_dropQueuedUpForProxy");

	/*Get the list from which the messages have to be dropped*/
	pList=pTxnBuffer->pslMesgQ;

	/*
	 * Iterate thru the contents of this list and free 
	 * up all the SipMessage's contained therein
	 */
	dResult=sip_listDeleteAll(pList,pError);
	if (dResult==SipFail)
	{
		return SipFail;
	}

	SIPDEBUGFN("Exiting from  function sip_dropQueuedUpForProxy");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_queueUpRetransxForProxy
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the retransmitted
****			requests
******************************************************************************/
SipBool sip_queueUpRetransxForProxy
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	/*
	 * This function populates the TxnBuffer's internal list
	 * of messages with a retransmitted message.
	 */

	SipBool dResult;
	SipMessage *pMesg=SIP_NULL;
	SipMessage *pCopiedMesg=SIP_NULL;
	SipList *pList=SIP_NULL;


	SIPDEBUGFN("Entering function sip_queueUpRetransxForProxy");

	/*First get a reference to the message that shud get queued up*/
	pMesg=pTxnInfo->pSipMessage;

	/*Now increment the refcount of this*/
	HSS_LOCKEDINCREF(((SipMessage *) (pMesg))->dRefCount);

	pCopiedMesg=pMesg;

	/*Get the list into which this has to be inserted*/
	pList=pTxnBuffer->pslMesgQ;

	/*
	 * This copied message now needs to be added to the collection 
	 * inside the SipTxnBuffer
	 */
	dResult=sip_listAppend(pList,(SIP_Pvoid)pCopiedMesg	,pError);
	if (dResult!=SipSuccess)
	{
		sip_freeSipMessage(pCopiedMesg);
		return SipFail;
	}

	SIPDEBUGFN("Exiting from function sip_queueUpRetransxForProxy");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_updateInvClTxnAfterMesgDecoding
****
****
**** DESCRIPTION:The Invite Client txn gets updated after rcvng a response
******************************************************************************/
SipBool sip_updateInvClTxnAfterMesgDecoding
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnDecodeResult *pDecodeResult,
	SipError *pError
)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	SipMessage *pMesg=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SIP_Pvoid	pContextData=SIP_NULL;
	SipBool		dRetVal=SipSuccess;

	SIPDEBUGFN("Entering function sip_updateInvClTxnAfterMesgDecoding");

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/* Do a check for the type of response message that has been recvd*/
	switch(pTxnInfo->dRespClass)
	{
		case SipResponse1xx:
			{
				SIP_S8bit dToTagChk=0;

				/* 
				 * This fn fetches the txn corresponding to the 1xx that
				 * has been received 
				 */
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo, \
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					dRetVal=SipFail;
					break;
				}

				/*Check if a txn was fetched*/
				if (pTxnBuffer==SIP_NULL)
				{
					/*Meaning it is a stray response*/
					*pDecodeResult=SipTxnStrayMessage;
					break;
				}
				else
				{
					/*
					 * There was a txn corresponding to this response.
					 * Now in case of proxies or UA we call the state 
					 * change fn anyway even if we get the provisional 
					 * response a second time. This is different from 
					 * Bis05 where only the first provisional response 
					 * caused any internal timer changes.
					 */
					if ((pTxnBuffer->dTxnState==SipTxnInvClientCallingState)
						||\
						(pTxnBuffer->dTxnState==SipTxnInvClientProceedingState))
					{
						/*In case the TimerCr was rnning then NO STATE CHANGES*/
						if (pTxnBuffer->dTimerStarted!=SipTimerCr)
						{	
							/*Only if the state is in Calling does 
								1xx have effect*/
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
								pTxnKey,SipTxnInvClientProceedingState, \
								pTxnInfo,pContextData,pError);
							if (dResult!=SipSuccess)
							{
								/*Handle failure here*/
								dRetVal=SipFail;
								break;
							}
							dStateChanged=SipSuccess;
							*pDecodeResult=SipTxnNonIgnorable;
						}
						else
						{
							/*A 1xx was received when Timer Cr was running*/
							*pDecodeResult=SipTxnIgnorable;
						}
					}
					else
					{
						/*
						 * Catch cases such as recvng a 1xx after having had
						 * already recvd a final or recvng one after an ACK
						 * is sent out.
						 */
						*pDecodeResult=SipTxnIgnorable;
					}
				}
				break;
			}
		case SipResponseFinal2xx:
			{
				/*
				 * A 2xx has been received. The below function invocn
				 * handles the case for case for both proxy and UA 
				 * kind of txns
				 */
				dResult=sip_updateInvClTxnFor2xxRecvd(pTxnInfo, \
								pDecodeResult,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					dRetVal=SipFail;
					break;
				}
				dStateChanged=SipSuccess;
				break;
			}
		case SipResponseFinalNon2xx:
			{
				SIP_S8bit dToTagChk=1;
				SIP_S8bit *pTempStr=SIP_NULL;
				SIP_S8bit	*pToBeFreed=SIP_NULL;

				/* 
				 * This fn fetches the txn corresponding to the final 
				 * response that has been received 
				 */
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo, \
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					dRetVal=SipFail;
					break;
				}

				/*Check if a txn was indeed fetched*/
				if (pTxnBuffer==SIP_NULL)
				{
					/*There was no txn corresponding to this response*/
					*pDecodeResult=SipTxnStrayMessage;
				}
				else
				{
					if ((pTxnBuffer->dTxnState==SipTxnInvClientCallingState) \
					||(pTxnBuffer->dTxnState==SipTxnInvClientProceedingState))
					{
						/*A final response has an effect only in these states*/
						/*Basically stop timers A_B if in Calling,
							Timer C if in Proceeding*/
						*pDecodeResult=SipTxnNonIgnorable;

						/*Obtain the To Tag and set it in the TxnKey*/
						/*Get the To Tag from the message*/
						dResult = sip_getTagfromFromHeader\
							(pMesg->pGeneralHdr->pToHdr,&pTempStr,pError);		
						if (dResult==SipSuccess)
						{
							/*
							 * In case the final response is of a non-2xx type
							 * even though there is no To Tag we need to be
							 * able to continue since more than one response
							 * from two branches will not be received
							 */
							pToBeFreed=pTxnBuffer->pTxnKey->pToTag;
							if (pToBeFreed!=SIP_NULL)
							{
								/*
								 * In case the initial key already had a To 
								 * Tag then we need to free that.
								 */
								SipError	tempError;
								(void)fast_memfree(TXN_MEM_ID,pToBeFreed,&tempError);
							}
						
							pTxnBuffer->pTxnKey->pToTag = (SIP_S8bit *)\
									STRDUPNETWORK ((SIP_S8bit *)pTempStr);
							pTxnBuffer->pTxnKey->dTagCheck=SipSuccess;
						}
						
						dResult=sip_changeTxnStateAndRelease(pTxnBuffer,\
							pTxnKey,SipTxnInvClientFinalResponseRecvState,
							pTxnInfo,pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							dRetVal=SipFail;
							break;
						}
						dStateChanged=SipSuccess;
					}
					else
					{
						if (pTxnBuffer->dTxnState==\
										SipTxnInvClientCompletedState)
						{
							/*
							 * Remote retransmit only when the respnse recvd 
							 * is the same as the one that had originally 
							 * caused the state change.
							 */
								dResult=sip_remoteRetransmitBuffer(pTxnBuffer, \
												pTxnInfo,pError);
								if (dResult!=SipSuccess)
								{
									/*Handle failure here*/
									dRetVal=SipFail;
									break;
								}
						}
						*pDecodeResult=SipTxnIgnorable;
					}
				}
				break;
			}
		case SipResponseInvalid:
			{
				*pDecodeResult=SipTxnStrayMessage;
				break;
			}
	}
	
	if ((!dStateChanged)||(dRetVal==SipFail))
	{
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}

		if (dRetVal==SipFail)
		{
			return SipFail;
		}
	}
	SIPDEBUGFN("Exiting from function sip_updateInvClTxnAfterMesgDecoding");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateNonInvClTxnForMesgSent
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateNonInvClTxnForMesgSent
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	SipBool dTxnExists=SipFail;
	en_SipTxnState dTxnState;
	en_SipTxnClass		dTxnClass;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateNonInvClTxnForMesgSent");

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/*
	 * Since this is a Non-Invite client txn we take similar action 
	 * for all methods
	 */
	
	dTxnClass=pTxnInfo->dTxnClass;
	/*Create the Txn-Key for this message*/
	if(sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError) == SipFail)
		return SipFail;

	if((pTxnInfo->dMethodType == SipTxnMethodPrack)&&\
					(pTxnInfo->pTxnContext->dTxnType == SipUATypeTxn))
	{
		if(sip_getInvTxnStateFromPRACKTxnKey(pTxnKey,pTxnInfo,&dTxnState, \
					pError) == SipFail)
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			/*Nothing can be done if the above call fails!!*/
			*pError = E_TXN_NO_EXIST;
			return SipFail;
		}

		/*
		 * A PRACK can be sent only if a final response has not yet been
		 * received
		 */
		if((dTxnState != SipTxnInvClientCallingState ) &&
			(dTxnState != SipTxnInvClientProceedingState))
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			/*Nothing can be done if the above call fails!!*/
			*pError = E_TXN_INV_STATE;
			return SipFail;
		}
	}
	/*We do not set the To-Tag check in the generated key*/

	/*Now a new txn needs to be created*/
	if(sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,pError) == SipFail)
		return SipFail;

	/*In case a txn already exists then we do not need to do anything*/
    if (pTxnBuffer->dTxnState==SipTxnDefaultState)
	{
		/*
		 * Copy the buffer that has been sent into the TxnBuffer
		 * for handling future retransX
		 */
		dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			/*Nothing more can be done if the above call fails!!*/
			return SipFail;
		}

		/*Populate the TxnBuffer with the values*/
		dResult=sip_populateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			/*Nothing more can be done if the above call fails!!*/
			return SipFail;
		}

		dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
				SipTxnNonInvClientTryingState,pTxnInfo,pContextData,pError);
		if (dResult!=SipSuccess)
		{
			if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
			{
				dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			}
			/*Nothing more can be done if the above call fails!!*/
			return SipFail;
		}
		dStateChanged=SipSuccess;
	}
	else
	{
		dTxnExists=SipSuccess;
	}

	if (!dStateChanged)
	{
		/*The txn needs to be released and also the Txn Key shud be freed*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	if (dTxnExists)
	{
		*pError=E_TXN_EXISTS;
		return SipFail;
	}

	SIPDEBUGFN("Exiting from function sip_updateNonInvClTxnForMesgSent");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_updateNonInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:
****************************************************************************/
SipBool sip_updateNonInvSrvTxnForMesgSent
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SIP_S8bit dToTagChk=0;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	SipBool dTxnWrongStateChange=SipFail;
	SIP_Pvoid	pContextData=SIP_NULL;
	
	SIPDEBUGFN("Entering function sip_updateNonInvSrvTxnForMesgSent");

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Fetch the txn without using To Tags*/
	dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo,&pTxnKey, \
				&pTxnBuffer,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Check if a txn of a similar class was indeed fetched*/
	if (pTxnBuffer==SIP_NULL)
	{
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}

		/*A response was sent for a non-existent txn*/
		*pError=E_TXN_NO_EXIST;
		return SipFail;
	}
	else
	{
		/*
		 * Initially a txn is created in the UA mode, but in case 
		 * an application decides to change the mode it shud be possible 
		 * to do so
		 */
		if (pTxnInfo->pTxnContext!=SIP_NULL)
		{	
			en_SipTxnType	dTxnType;
			dTxnType = pTxnInfo->pTxnContext->dTxnType;
			if (pTxnBuffer->dType!=dTxnType)
			{
				pTxnBuffer->dType=dTxnType;
			}
		}

		/*
		 * In case the txn state is in ByPass mode then we just need
		 * to pass on the message and no txn level handling is further
		 * necessary... This is strictly for a proxy alone
		 */
		if (pTxnBuffer->dTxnState==SipTxnServerBypassState)
		{
			dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
					pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				dResult=sip_releaseTxnNFreeKey\
					(pTxnBuffer,pTxnKey,pTxnInfo,pError);
				/*Handle failure here*/
				return SipFail;
			}

			dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
			if (dResult==SipFail)
			{
				dResult=sip_releaseTxnNFreeKey\
					(pTxnBuffer,pTxnKey,pTxnInfo,pError);
				return SipFail;
			}

			/*
			 * State change was not caused hence release of Txn, freeing of
			 * Txn Key necessary
			 */
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}

			return SipSuccess;
		}

		/*
		 * The txn now moves to the confirmed state for a proxy.
		 * If this is the first response being sent out by the 
		 * proxy then the txn is confirmed!! The queued up messages
		 * can be dropped
		 */
		if (pTxnBuffer->dTxnState==SipTxnNonInvServerTryingState)
		{
			if (pTxnBuffer->dType==SipProxyTypeTxn)
			{
				dResult=sip_dropQueuedUpForProxy(pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					return SipFail;
				}
			}
		}

		/*Change the state on basis of response sent*/
		switch(pTxnInfo->dRespClass)
		{
			case SipResponseInvalid:
			{
				break;
			}

			case SipResponse1xx:
			{
				if ((pTxnBuffer->dTxnState==SipTxnNonInvServerTryingState)|| \
					(pTxnBuffer->dTxnState==SipTxnNonInvServerProceedingState))
				{
					/*
					 * The transport information within the buffer will
					 * need to be updated
					 */
					dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
					if (dResult==SipFail)
					{
						dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					dResult=sip_reassignbufferInTxnBuffer \
									(pTxnBuffer,pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
				}

				if (pTxnBuffer->dTxnState==SipTxnNonInvServerTryingState)
				{
					/*
					 * Only in this case will a state change occur.
					 * Change the state to SipTxnNonInvServerProceedingState
					 */
					dResult=sip_changeTxnStateAndRelease \
							(pTxnBuffer,pTxnKey, \
							SipTxnNonInvServerProceedingState, pTxnInfo,\
							pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				else
				{
					if (pTxnBuffer->dTxnState==SipTxnNonInvServerCompletedState)
					{
						dTxnWrongStateChange=SipSuccess;
					}
				}
				break;
			}

			default:
			{
				/*
				 * This case is for a final response 2xx-6xx.We shud
				 * allow the application to send a message for any 
				 * state the txn is in. However if the txn is already
				 * in the SipTxnNonInvServerCompletedState then we need 
				 * to check the transport address and only if the addresses
				 * are different we shud allow it to be sent
				 */

				if (pTxnBuffer->dTxnState==SipTxnNonInvServerCompletedState)
				{
					SipBool	dResultTranspCheck;
					/*Check the two transports*/

					/*
					 * We allow sending a response for a second time
					 * only if the transport address is different
					 */
					dResultTranspCheck=sip_compareTranspAddr\
						(pTxnBuffer->pAddr,pTxnInfo->pAddr);
					if (dResultTranspCheck==SipSuccess)
					{
						dTxnWrongStateChange=SipSuccess;
					}	
				}	

				if (dTxnWrongStateChange==SipFail)
				{
					/*
					 * The transport information within the buffer will 
					 * need to be updated
					 */
					dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
					if (dResult==SipFail)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					dResult=sip_reassignbufferInTxnBuffer \
												(pTxnBuffer,pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
							SipTxnNonInvServerCompletedState, pTxnInfo,\
							pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				break;
			}
		}
	}

	if(!dStateChanged)
	{
		/*
		 * State change was not caused hence release of Txn, hence
		 * freeing of Txn Key necessary
		 */
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	if (dTxnWrongStateChange)
	{
		/*An error needs to be returned in this case*/
		*pError=E_TXN_INV_STATE;
		return SipFail;
	}

	SIPDEBUGFN("Exiting from function sip_updateNonInvSrvTxnForMesgSent");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateNonInvSrvTxnAfterMesgDecoding
****
****
**** DESCRIPTION:
****************************************************************************/
SipBool sip_updateNonInvSrvTxnAfterMesgDecoding
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnDecodeResult *pDecodeResult,
	SipError *pError
)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	en_SipTxnClass	dTxnClass;
	SIP_Pvoid pContextData=SIP_NULL;


	SIPDEBUGFN("Entering function sip_updateNonInvSrvTxnAfterMesgDecoding");

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Create the Txn-Key for this message*/
	dTxnClass=pTxnInfo->dTxnClass;
	dResult=sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

    /*
	 * Now a new txn needs to be created in case this is
	 * the first instance of a NonInvite being received.
	 */
	dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*
	 * Check the state of the txn to check if a new txn indeed
	 * got created if the above fetch resulted in a new txn
	 * then it is not retransX.
	 */
    if (pTxnBuffer->dTxnState!=SipTxnDefaultState)
	{
		if (pTxnBuffer->dTxnClass!=pTxnInfo->dTxnClass)
		{
			/*Freak case*/
			*pDecodeResult=SipTxnStrayMessage;
		}
		else
		{
			*pDecodeResult=SipTxnIgnorable;

			/*
			 * In case it is a proxy and the state of the txn is in
			 * SipTxnInvServerProceedingState then queue up the req.
			 */
			if (pTxnBuffer->dType==SipProxyTypeTxn)
			{
				/*RetransX case*/
				/*Proxy related handling*/
				if (pTxnBuffer->dTxnState== \
					SipTxnServerBypassState)
				{
					/*For a bypass mode silently pass along*/
					*pDecodeResult=SipTxnRetrans;
				}
				else
		 		if (pTxnBuffer->dTxnState== \
					 SipTxnNonInvServerTryingState)
				{
					/*Queue up the messages*/
					dResult=sip_queueUpRetransxForProxy (pTxnBuffer,\
								pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
					*pDecodeResult=SipTxnQueued;
				}
				else
				{
					/*Remote retransmit the buffer*/
					dResult=sip_remoteRetransmitBuffer(pTxnBuffer,\
							pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
				}
				/*Proxy related handling ends*/
			}
			else
			{
				/*UA related handling starts*/

				/*RetransX case*/
				/*Check if a r-retransX is needed*/
				if ((pTxnBuffer->dTxnState==SipTxnNonInvServerProceedingState)
					||(pTxnBuffer->dTxnState==SipTxnNonInvServerCompletedState))
				{
					/*Remote retransmit the buffer*/
					dResult=sip_remoteRetransmitBuffer(pTxnBuffer, pTxnInfo, \
														pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
				}
			}
		}
	}
	else
	{
		/*A new txn has got created*/
		*pDecodeResult=SipTxnNonIgnorable;

		/*Set the Txn Class*/
		pTxnBuffer->dTxnClass=pTxnInfo->dTxnClass;

		/*We need to populate the Txn Buffer*/
		dResult=sip_populateTxnBuffer(pTxnBuffer, \
				pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey\
					(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			return SipFail;
		}

		/*
		 * In case of a proxy we need to inform the proxy that
		 * a confirmn of this txn is needed.
		 */
		if (pTxnBuffer->dType!=SipProxyTypeTxn)
			*pDecodeResult=SipTxnNonIgnorable;
		else
			*pDecodeResult=SipTxnConfirmnNeeded;
		/*
		 * Change the state to SipTxnInvServerProceedingState.
		 * Increment REF Count in case of PRACK as we need the
		 * reference of the key to remove RPR Txn
		 */
		if ((pTxnInfo->dMethodType==SipTxnMethodPrack)&&\
			(pTxnBuffer->dType == SipUATypeTxn))
				HSS_LOCKEDINCREF((pTxnKey)->dRefCount);
		dResult=sip_changeTxnStateAndRelease \
			(pTxnBuffer,pTxnKey,SipTxnNonInvServerTryingState, \
		 	pTxnInfo,pContextData,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
			{
				dResult=sip_releaseTxnNFreeKey\
					(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			}
			return SipFail;
		}
		
		if((pTxnInfo->dMethodType == SipTxnMethodPrack)&&\
						(pTxnBuffer->dType == SipUATypeTxn))
		{
			/* Remove RPR Txn Corresponding to this PRACK */
			if(sip_removeRPRTxn(pTxnKey,pTxnInfo,pDecodeResult,\
					pError) == SipFail)
			{
				/*
				 * This is a stray PRACK , meaning a PRACK has been
				 * received on the UAS side for which there is no RPR
				 * existing. The application needs to respond to this PRACK
				 * with a 408. So a transaction HAS to get created over
				 * which the response can be sent.
				 */
				*pDecodeResult = SipTxnStrayPrack;
			}
			
			sip_freeSipTxnKey(pTxnKey);
		}
		dStateChanged=SipSuccess;
	}

	/*
	 * In case the state has not changed then the memory
	 * needs to be freed up.
	 */

	if (!dStateChanged)
	{
		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
										pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting from function sip_updateNonInvSrvTxnAfterMesgDecoding");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_cloneNCreateNewTxn
****
****
**** DESCRIPTION:
****************************************************************************/
SipBool sip_cloneNCreateNewTxn
#ifdef ANSI_PROTO
(
	SipTxnBuffer *pTxnBuffer,
	SipTxnKey *pTxnKey,
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnBuffer,pTxnKey,pTxnInfo,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnKey *pTxnKey;
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipTxnKey *pNewKey=SIP_NULL;
	SipTxnBuffer *pNewTxnBuffer=SIP_NULL;
	SipTxnKey *pTempKey=SIP_NULL;
	SipTxnBuffer *pTempBuffer=SIP_NULL;
	SIP_S8bit dRemove=0,dCreate=0;
	SIP_Pvoid pContextData=SIP_NULL;

	SipBool dResult=SipFail;
	SIPDEBUGFN("Entering function sip_cloneNCreateNewTxn");

	dResult=sip_initSipTxnKey(&pNewKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Ensure To Tag check is set*/
	pTxnKey->dTagCheck=SipSuccess;

	dResult=sip_cloneSipTxnKey(pNewKey,pTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	dResult=sip_initSipTxnBuffer(&pNewTxnBuffer,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		sip_freeSipTxnKey(pNewKey);
		return SipFail;
	}

	/*Now copy all values into the newly created buffer*/
	pNewTxnBuffer->dType=pTxnBuffer->dType;
	pNewTxnBuffer->dTxnState=SipTxnInvClientFinalResponseRecvState;
	pNewTxnBuffer->dEnableRetransCallback = \
		pTxnBuffer->dEnableRetransCallback;
	pNewTxnBuffer->dTxnClass=pTxnBuffer->dTxnClass;
	pNewTxnBuffer->dTimeoutCbkBitMask=pTxnBuffer->dTimeoutCbkBitMask;

	/*HALT!!!! The respcode shud be copied fromthe TxnInfo instead*/
	pNewTxnBuffer->dRespCode=pTxnInfo->dRespCode;

	/*Add the reference to the newly created key as well*/
	pNewTxnBuffer->pTxnKey=pNewKey;
	pNewTxnBuffer->timeoutValues.dTimerD_T3= \
		pTxnBuffer->timeoutValues.dTimerD_T3;

	/*
	 * Now release the earlier fetched txn since otherwise
	 * we might get locked
	 */
#ifdef SIP_LINT
        dRemove=0;
#else
        dRemove=!TXN_REMOVE;
#endif
	/*Now release the txn as well --without TXN_REMOVE*/
	pTxnKey->dTagCheck=SipFail;

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while releasing 
     * the transaction, the pointer will be dereferenced
     */
	if(SIP_NULL != pContextData)
	{
	    dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTempBuffer),pContextData,dRemove,pError);
	}
	else
		dResult = SipFail;

	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Now we need to add this buffer into the aggregation of TxnBuffers*/
	/*We set the TXN_CREAT flag*/
	dCreate=TXN_CREAT;
	dResult= FETCH_TXN((SIP_Pvoid *)&pNewTxnBuffer, \
			(SIP_Pvoid)(pNewKey), pContextData,dCreate,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		sip_freeSipTxnBuffer(pNewTxnBuffer);
		sip_freeSipTxnKey(pNewKey);
		return SipFail;
	}

	/*
	 * Since a reference to the newly created key part of (key,buffer)
	 * pair is needed by the invoker of the higher level API, we
	 * increment the refcount and add a reference into the TxnInfo 
	 * variable
	 */
	HSS_LOCKEDINCREF((pNewKey)->dRefCount);
	pTxnInfo->pKeyOfCreatedTxn=pNewKey;

	/*Release the txn now*/
#ifdef SIP_LINT
        dRemove=0;
#else
        dRemove=!TXN_REMOVE;
#endif
	/*Now release the txn as well --without TXN_REMOVE*/
	dResult= RELEASE_TXN((SIP_Pvoid)pNewKey,(SIP_Pvoid *)&pTempKey, \
				(SIP_Pvoid *)(&pTempBuffer),pContextData,dRemove,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	SIPDEBUGFN("Exiting function sip_cloneNCreateNewTxn");

	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateInvClTxnFor2xxRecvd
****
****
**** DESCRIPTION:
****************************************************************************/
SipBool sip_updateInvClTxnFor2xxRecvd
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnDecodeResult *pDecodeResult,
	SipError *pError
)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	SIP_S8bit dToTagChk=1;
	SipMessage *pMesg=SIP_NULL;
	SipTxnBuffer	*pTxnBuffer=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SIP_Pvoid	pContextData=SIP_NULL;
	SIPDEBUGFN("Entering function sip_updateInvClTxnFor2xxRecvd");

	/*Get the reference to the message*/
	pMesg=pTxnInfo->pSipMessage;
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/*Do a fetch with the to tag initially*/
	dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo,&pTxnKey, \
			&pTxnBuffer,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*Check if the txn fetched was of the proper class or not*/
	if (pTxnBuffer!=SIP_NULL)
	{
		if (pTxnBuffer->dTxnClass!=pTxnInfo->dTxnClass)
		{
			/*Freak case... return stray*/
			*pDecodeResult=SipTxnStrayMessage;

			/*Release the txn and also free the key*/
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
			return SipSuccess;
		}
	}

	/*Check if a txn was fetched*/
	if (pTxnBuffer==SIP_NULL)
	{
		/*
		 * Meaning it is a stray response
		 *	 OR
		 * it is a 2xx being recvd from a different branch after a 
		 * final resp has already been recvd on a diff branch.
		 * These two cases need to be differentiated further,
		 * needs to be noted that for a proxy response from
		 * a diff branch is not possible for this we do a without
		 * To Tag fetch.
		 */

		dToTagChk=0;
		dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo,&pTxnKey, \
				&pTxnBuffer,pError);
		if (dResult!=SipSuccess)
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			/*Handle failure here*/
			return SipFail;
		}

		if (pTxnBuffer==SIP_NULL)
		{
			/*It is most definitely a stray*/
			*pDecodeResult=SipTxnStrayMessage;
		}
		else
		{
			/*
			 * Needs to be noted that if control has reached here
			 * then for a proxy receipt of a 2xx cannot cause the 
			 * txn to move to 2xxforwarded state since the txn is
			 * already involved in ACKING a final response
			 */

			*pDecodeResult=SipTxnNonIgnorable;

			/*
			 * A new txn needs to be created for handling this final
			 * response from a new branch, only if it is a non-proxy.
			 */
			if (pTxnBuffer->dType!=SipProxyTypeTxn)
			{
				dResult=sip_cloneNCreateNewTxn(pTxnBuffer, pTxnKey, \
														pTxnInfo,pError);
				if (dResult!=SipSuccess)
				{
					dResult=sip_releaseTxnNFreeKey\
						(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					return SipFail;
				}
				pTxnInfo->pKeyOfCreatedTxn->dViaBranchCheck = SipFail;
				pTxnBuffer=SIP_NULL;
			}
		}
	}
	else
	{
		/*
		 * Coming here means that the response recvd is either
		 * a first time response being recvd or a retransX of
		 * a response from a branch, these cases need to be 
		 * further differentiated .Needs to be noted that for
		 * a proxy only the former is possible
		 */

		/*
		 * A check of the state of the txn will tell us if this 
		 * is a first time a 2xx is being received
		 */

		if ((pTxnBuffer->dTxnState==SipTxnInvClientCallingState)
			||(pTxnBuffer->dTxnState==SipTxnInvClientProceedingState))
		{
			*pDecodeResult=SipTxnNonIgnorable;

			/*For a proxy the txn needs to be removed*/
	 		if (pTxnBuffer->dType==SipProxyTypeTxn)
			{
				/*Move to terminated state*/
				/*Change the state to SipTxnTerminatedState*/
				dResult=sip_changeTxnStateAndRelease \
					(pTxnBuffer,pTxnKey,SipTxnTerminatedState, \
			 			pTxnInfo,pContextData,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
					{
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					}
					return SipFail;
				}
				dStateChanged=SipSuccess;
			}
			else
			{
				SIP_S8bit *pTempStr=SIP_NULL;
				SIP_S8bit *pToBeFreed=SIP_NULL;

				/*For a UA move to the FinalResponseRecvd state*/

				/*
				 * Change the To Tag of the pTxnBuffer to reflect the 
				 * To Tag present in the recently recvd response
				 */
				
				/*
				 * Get the To Tag from the message, in case a TO Tag is not
				 * present then it actually means a violation of the
				 * protocol, however since we might need to maintain
				 * compatibility with older clients, the To Tags absence
				 * will be tolerated
				 */
				dResult = sip_getTagfromFromHeader\
							(pMesg->pGeneralHdr->pToHdr,&pTempStr,pError);		
				if ((dResult==SipSuccess) && (SIP_NULL != pTempStr))
				{
					pToBeFreed=pTxnBuffer->pTxnKey->pToTag;
					if (pToBeFreed!=SIP_NULL)
					{
						SipError	tempError;
						/*
						 * In case the initial key already had a To Tag
						 * then we need to free that.
						 */
						(void)fast_memfree(TXN_MEM_ID,pToBeFreed,&tempError);
					}

					pTxnBuffer->pTxnKey->pToTag = (SIP_S8bit *)STRDUPNETWORK\
										((SIP_S8bit *)pTempStr);
					pTxnBuffer->pTxnKey->dTagCheck=SipSuccess;
				}

				/*
				 * Set the via branch check to false. This is because when
				 * an ACK is sent to this 2xx response, the topmost Via
				 * branch will be different from what is present in the
				 * topmost Via of the INVITE. This flag will be checked in
				 * sip_txn_compareTxnKeys, and if FALSE, the via branch
				 * will not be checked. 
				 * In case of non-2xx response, the flag is left untouched 
				 * (SipSuccess set in constructor of the SipTxnKey) and 
				 * therefore, a strict check will be done for the topmost 
				 * Via branch.
				 */
				pTxnBuffer->pTxnKey->dViaBranchCheck = SipFail;

				dResult=sip_changeTxnStateAndRelease (pTxnBuffer,\
						pTxnKey, SipTxnInvClientFinalResponseRecvState,\
						pTxnInfo,pContextData,pError);

				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
					{
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					}
					return SipFail;
				}
				dStateChanged=SipSuccess;
			}
		}
		else
		{
			if (pTxnBuffer->dTxnState==SipTxnInvClientCompletedState)
			{
				/*
				 * Remote retransmit only when the respnse recvd is
				 * the same as the one that had originally caused
				 * the state change.
				 */
				if (pTxnBuffer->dRespCode==pTxnInfo->dRespCode)
				{
					/*Remote retransmit an ACK if that has been sent already*/
					dResult=sip_remoteRetransmitBuffer(pTxnBuffer, \
													pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
				}
			}
			*pDecodeResult=SipTxnIgnorable;
		}
	}

	if (!dStateChanged)
	{
		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting from function sip_updateInvClTxnFor2xxRecvd");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_updateInvSrvTxnAfterMesgDecoding
****
****
**** DESCRIPTION:
****************************************************************************/
SipBool sip_updateInvSrvTxnAfterMesgDecoding
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnDecodeResult *pDecodeResult,
	SipError *pError
)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	en_SipTxnClass	dTxnClass;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateInvSrvTxnAfterMesgDecoding");

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Obtain the TxnClass from TxnInfo*/
	dTxnClass=pTxnInfo->dTxnClass;
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/* Do a check for the type of message that had been sent*/
	switch(pTxnInfo->dMethodType)
	{
		case SipTxnMethodInvite:
			{
				/*Create the Txn-Key for this message*/
				dResult=sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

			    /*
				 * Now a new txn needs to be created in case this is
				 * the first instance of an Invite being received.
				 */
				dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey, \
											pTxnInfo, pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					return SipFail;
				}

				/*
				 * Check the state of the txn to check if a new txn 
				 * indeed got created .If the above fetch resulted
				 * in a new txn then it is not retransX.
				 */
			    if (pTxnBuffer->dTxnState!=SipTxnDefaultState)
				{
					/*
					 * RetransX case::In case it is a proxy and the state 
					 * of the txn is in SipTxnInvServerProceedingState 
					 * then queue up the req
					 */
					if (pTxnBuffer->dType==SipProxyTypeTxn)
					{
						/*RetransX case*/
						/*Proxy related handling*/
					 	if (pTxnBuffer->dTxnState== \
							 SipTxnInvServerProceedingState)
						{
							dResult=sip_queueUpRetransxForProxy (pTxnBuffer,\
											pTxnInfo,pError);
							if (dResult!=SipSuccess)
							{
								/*Handle failure here*/
								dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
								return SipFail;
							}
							*pDecodeResult=SipTxnQueued;
						}
						else
						{
							if (pTxnBuffer->dTxnState \
									==SipTxnInvServer1xxSentState)
							{
								/*Remote retransmit the buffer*/
								dResult=sip_remoteRetransmitBuffer(pTxnBuffer,\
										pTxnInfo,pError);
								if (dResult!=SipSuccess)
								{
									/*Handle failure here*/
									dResult=sip_releaseTxnNFreeKey\
										(pTxnBuffer,pTxnKey,pTxnInfo,pError);
									return SipFail;
								}
							}

							*pDecodeResult=SipTxnIgnorable;

							/*
							 * Check for the bypass state and silently
							 * pass on in that case
							 */
							if (pTxnBuffer->dTxnState==SipTxnServerBypassState)
							{
								*pDecodeResult=SipTxnRetrans;
							}
						}
					}
					else
					{
						/*RetransX case*/
						en_SipTxnState	dTempTxnState;
						dTempTxnState=pTxnBuffer->dTxnState;
						
						/*Check if a r-retransX is needed*/
						if ((dTempTxnState==SipTxnInvServer1xxSentState)\
								||
							(dTempTxnState==SipTxnInvServerCompletedState))	
						{
							/*Remote retransmit the buffer*/
							dResult=sip_remoteRetransmitBuffer(pTxnBuffer, \
														pTxnInfo,pError);
							if (dResult!=SipSuccess)
							{
								/*Handle failure here*/
								dResult=sip_releaseTxnNFreeKey\
										(pTxnBuffer,pTxnKey,pTxnInfo,pError);
								return SipFail;
							}
						}
						*pDecodeResult=SipTxnIgnorable;
					}
					/*RetransX handling ends*/
				}
				else
				{
					/*A new txn has got created*/
					*pDecodeResult=SipTxnNonIgnorable;

					/*Set the Txn Class*/
					pTxnBuffer->dTxnClass=pTxnInfo->dTxnClass;

					/*We need to populate the Txn Buffer*/
					dResult=sip_populateTxnBuffer(pTxnBuffer, \
											pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					/*
					 * In case of a proxy we need to inform the proxy that
					 * a confirmn of this txn is needed
					 */
					if (pTxnBuffer->dType!=SipProxyTypeTxn)
						*pDecodeResult=SipTxnNonIgnorable;
					else
						*pDecodeResult=SipTxnConfirmnNeeded;

					/*Change the state to SipTxnInvServerProceedingState*/
					dResult=sip_changeTxnStateAndRelease \
						(pTxnBuffer,pTxnKey,SipTxnInvServerProceedingState, \
					 	pTxnInfo,pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				break;
			}
		case SipTxnMethodAck:
			{
				SIP_S8bit dToTagChk=1;
				pTxnKey=SIP_NULL;

				/*Fetch the txn using To Tags*/
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo, \
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*Check if a txn was indeed fetched*/
				if (pTxnBuffer==SIP_NULL)
				{
					*pDecodeResult=SipTxnStrayMessage;
				}
				else
				{
					/*Now check if a state change is actually needed*/

					if (pTxnBuffer->dTxnState==SipTxnInvServerCompletedState)
					{
						/* we need to check the via branch if the response is
						 * 400 and there is no need to check for via branch if
						 * the response is 200
                         */
						if(pTxnBuffer->dRespCode/100 != 2)
						{
							if ( (pTxnBuffer->pTxnKey->pViaBranch != \
								SIP_NULL) && (pTxnKey->pViaBranch != \
								SIP_NULL) )
							{
								if(sip_strcmp(pTxnBuffer->pTxnKey->pViaBranch,\
										pTxnKey->pViaBranch)!=0)
									*pDecodeResult = SipTxnStrayMessage;
							}
							else
							{
								if ( pTxnBuffer->pTxnKey->pViaBranch != \
										pTxnKey->pViaBranch )
									*pDecodeResult = SipTxnStrayMessage;
							}
						}
						
						*pDecodeResult=SipTxnNonIgnorable;
						/*Change state to SipTxnInvServerCompletedState */
						dResult=sip_changeTxnStateAndRelease \
						(pTxnBuffer,pTxnKey,SipTxnInvServerConfirmedState, \
				 			pTxnInfo,pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
								dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;
					}
					else
					{
						/*
						 * In case of the state being in 2xxforwarded
						 * then any ACK needs to be treated as a stray
						 */
						if (pTxnBuffer->dTxnState== \
										SipTxnInvServer2xxForwardedState)
						{
							*pDecodeResult=SipTxnStrayMessage;
						}
						else
						if (pTxnBuffer->dTxnState== \
										SipTxnServerBypassState)
						{
							/*
							 * In case it is a proxy and in the bypass mode
							 * pass on anything that is recvd.
							 */
							*pDecodeResult=SipTxnRetrans;
						}
						else
						*pDecodeResult=SipTxnIgnorable;
					}
				}
				break;
			}
		default:
				break;
	}

	/*
	 * In case the state has not changed then the memory
	 * needs to be freed up
	 */
	if (!dStateChanged)
	{
		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
						pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting from function sip_updateInvSrvTxnAfterMesgDecoding");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateProxyInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateProxyInvSrvTxnForMesgSent
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipTxnBuffer *pTxnBuffer,
	SipTxnKey *pTxnKey,
	SipError *pError
)
#else
	(pTxnInfo,pTxnBuffer,pTxnKey,pError)
	SipTxnInformn *pTxnInfo;
	SipTxnBuffer *pTxnBuffer;
	SipTxnKey *pTxnKey;
	SipError *pError;
#endif
{

	SipBool dStateChanged=SipFail;
	SipBool	dTxnWrongStateChange=SipFail;
	SipBool dResult;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateProxyInvSrvTxnForMesgSent");
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/*
	 * In case the txn is in the bypass mode then we do not need to
	 * do any state changes, we only need to pass on the message.
	 */
	if (pTxnBuffer->dTxnState==SipTxnServerBypassState)
	{
		SIP_S8bit 			*pTempMsgBuffer=0;

		dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}

		dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
		if (dResult==SipFail)
		{
			return SipFail;
		}

		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo, \
					pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}


		pTempMsgBuffer=pTxnInfo->pMsgBuffer;
		pTxnInfo->pMsgBuffer=pTxnBuffer->pBuffer;
		dResult=sip_txn_sendBuffer(pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/* 
			 * sendBuffer Failed ,release the Txn created for this
			 * and free the key. If the reference is passed to the
			 * application decrement the ref count. Make the BUffer
			 * Reference to NULL we will free in sip_txn_sendMessage
			 * anyway
			 */
			SipError tempError; 
			tempError = E_NO_ERROR;
			pTxnBuffer->pBuffer = SIP_NULL;
			(void)sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
											   pTxnInfo,&tempError);
			return SipFail;
		}
		pTxnInfo->dMesgSentAtLeastOnce=SipSuccess;
		pTxnInfo->pMsgBuffer=pTempMsgBuffer;
		return SipSuccess;
	}

	/*
	 * If this is the first response being sent out by the proxy then
	 * the txn is confirmed!! and the queued up messaged can be dropped
	 */
	
	if (pTxnBuffer->dTxnState==SipTxnInvServerProceedingState)
	{
		dResult=sip_dropQueuedUpForProxy(pTxnBuffer,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo, \
					pError);
			return SipFail;
		}
	}

	/*
	 * If a 2xx has already been forwarded in this txn, then just 
	 * forward whatever has been sent without doing any state changes
	 */

	if (pTxnBuffer->dTxnState!=SipTxnInvServer2xxForwardedState)
	{
		switch(pTxnInfo->dRespClass)
		{
			case SipResponseInvalid:
			{
				break;
			}

			case SipResponseFinal2xx:
			{
				/*
				 * A 2xx is being forwarded on this txn, this txn needs
				 * to be maintained for a period for which retransX of 
				 * the INVITE will continue, so that these retransX
				 * can be eaten up by this txn ,later this txn
				 * will be removed.
				 */

				dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
						pTxnInfo,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					dResult=sip_releaseTxnNFreeKey\
						(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					return SipFail;
				}

				dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
				if (dResult==SipFail)
				{
					/*Handle failure here*/
					dResult=sip_releaseTxnNFreeKey\
						(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					return SipFail;
				}

				/*Change the state to SipTxnInvServer2xxForwardedState*/
				dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
						SipTxnInvServer2xxForwardedState,pTxnInfo,\
						pContextData,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
					{
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
					}
					return SipFail;
				}
				dStateChanged=SipSuccess;
				break;
			}

			case SipResponse1xx:
			{
				/*
				 * If a final response has not yet been sent on this txn
				 * Irrespective of the 1xx sent update the txn buffer
				 * with the same for r-retransX.
				 */

				if ((pTxnBuffer->dTxnState==SipTxnInvServerProceedingState)
					||(pTxnBuffer->dTxnState==SipTxnInvServer1xxSentState))
				{
					dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
							pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
					
					/*Update event context and stuff*/
					dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
					if (dResult==SipFail)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}
				}

				/*
				 * A sent 1xx has an effect on the txn state only if it
				 * had been in the SipTxnInvServerProceedingState state.
				 */
				if (pTxnBuffer->dTxnState==SipTxnInvServerProceedingState)
				{
					/*Change the state to SipTxnInvServer1xxSentState*/
					dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
							SipTxnInvServer1xxSentState, pTxnInfo,\
							pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				else if (pTxnBuffer->dTxnState==SipTxnInvServer1xxSentState)
				{
					SIP_S8bit 			*pTempMsgBuffer=0;

					pTempMsgBuffer=pTxnInfo->pMsgBuffer;
					pTxnInfo->pMsgBuffer=pTxnBuffer->pBuffer;
					dResult=sip_txn_sendBuffer(pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/* 
						 * sendBuffer Failed ,release the Txn created for this
						 * and free the key. If the reference is passed to the
						 * application decrement the ref count. Make the BUffer
						 * Reference to NULL we will free in sip_txn_sendMessage
						 * anyway
						 */
						SipError tempError; 
						tempError = E_NO_ERROR;
						pTxnBuffer->pBuffer = SIP_NULL;
						(void)sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
														   pTxnInfo,&tempError);
						return SipFail;
					}
					pTxnInfo->dMesgSentAtLeastOnce=SipSuccess;
					pTxnInfo->pMsgBuffer=pTempMsgBuffer;
				}
				break;
			}

			default:
			{
				/*
				 * Any other final response causes it to move to the 
				 * completed state.
				 */
				if (pTxnBuffer->dTxnState==SipTxnInvServerCompletedState)
				{
					/*
					 * If already a response has been dispatched on this txn,
					 * we need to check the transport and allow the state
					 * change only if the new transport is different from 
					 * the earlier one.
					 */
					SipBool	dResultTranspCheck;

					/*
					 * We allow sending a response for a second time
					 * only if the transport address is different.
					 */
					dResultTranspCheck=sip_compareTranspAddr\
							(pTxnBuffer->pAddr,pTxnInfo->pAddr);
					if (dResultTranspCheck==SipSuccess)
					{
						dTxnWrongStateChange=SipSuccess;
						
						/*
						 * Actually state has not changed. But we dont want
						 * the releaseTxnNFreeKey to be invoked. That is
						 * already getting done when this fn returns with
						 * failure
						 */
						dStateChanged=SipSuccess;
						break;
					}	
				}
				
				if (dTxnWrongStateChange==SipFail)
				{
					dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
							pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					/*
					 * The transport information within the buffer will 
					 * need to be updated.
					 */
					dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
					if (dResult==SipFail)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
						pTxnKey, SipTxnInvServerCompletedState,pTxnInfo,\
						pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				break;
			}
		}
	}
	else
	{

		/*Just update the transport information*/
		dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
		if (dResult==SipFail)
		{
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
				pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
			return SipFail;
		}
	}

	/*In case the state has not changed then the memory needs to be freed up*/
	if (!dStateChanged)
	{
		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}
	
	if (dTxnWrongStateChange==SipSuccess)
	{
		/*An error needs to be returned in this case*/
		*pError=E_TXN_INV_STATE;
		return SipFail;
	}
	
	SIPDEBUGFN("Exiting from function sip_updateProxyInvSrvTxnForMesgSent");
	return SipSuccess;
}

/****************************************************************************
**** FUNCTION:sip_updateInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateInvSrvTxnForMesgSent
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SIP_S8bit dToTagChk=0;
	SipBool dStateChanged=SipFail;
	SipBool dTxnWrongStateChange=SipFail;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateInvSrvTxnForMesgSent");

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/* 
	 * This fn fetches the txn corresponding to the msg that
	 * had been sent , also pTxnKey points to the key for this
	 * txn, do a without To Tag fetch.
	 */
	dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo,&pTxnKey, \
					&pTxnBuffer,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*
	 * Check if a txn in the similar state for this key was
	 * fetched or not if a txn was not fetched then nothing
	 * more needs to be done.
	 */
	if (pTxnBuffer==SIP_NULL) 
	{
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
		*pError=E_TXN_NO_EXIST;
		return SipFail;
	}

	/*
	 * Initially a txn is created in the UA mode, but in case an 
	 * application decides to change the mode it hud be possible
	 * to do so
	 */
	if (pTxnInfo->pTxnContext!=SIP_NULL)
	{	
		en_SipTxnType	dTxnType;
		dTxnType = pTxnInfo->pTxnContext->dTxnType;
		if (pTxnBuffer->dType!=dTxnType)
		{
			pTxnBuffer->dType=dTxnType;
		}
	}
	
	/*
	 * Now that the txn is fetched we can go ahead and check
	 * if the txn is a proxy type or a UA type.
	 */
	if (pTxnBuffer->dType==SipProxyTypeTxn)
	{

		/*A proxy requires special handling*/

		/*
		 * Basically if a 2xx has been sent then the state needs to be 
		 * set to SipTxnInvServer2xxForwardedState that blocks out 
		 * retransX of the INVITE request for which this 2xx has 
		 * actually been sent out
		 */

		dResult=sip_updateProxyInvSrvTxnForMesgSent(pTxnInfo,pTxnBuffer, \
					pTxnKey,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
					pTxnInfo,pError);
			return SipFail;
		}
		dStateChanged=SipSuccess;
	}
	else
	{
		/*The Txn is of an end-point UA type*/

		/*Check the kind of response that has been sent*/
		switch(pTxnInfo->dRespClass)
		{
			case SipResponseInvalid:
			{
				break;
			}

			case SipResponse1xx:
				{
					/*
					 * If a final response has not yet been sent on this 
					 * txn Irrespective of the 1xx sent update the txn
					 * buffer with the same for r-retransX.
					 */

					if ((pTxnBuffer->dTxnState==SipTxnInvServerProceedingState)
						||(pTxnBuffer->dTxnState==SipTxnInvServer1xxSentState))
					{
						dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
								pTxnInfo,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
								pTxnInfo,pError);
							return SipFail;
						}
					}

					/*
					 * The buffer needs to be updated only in case of
					 * a 1xx being sent in ProceedingState or 1xxSentState.
					 */
					if ((pTxnBuffer->dTxnState==
							SipTxnInvServerProceedingState)|| \
						(pTxnBuffer->dTxnState==SipTxnInvServer1xxSentState))
					{
						/*
						 * The transport information within the buffer will
						 * need to be updated.
						 */
						dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
						if (dResult==SipFail)
						{
							/*Handle failure here*/
							dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
								pTxnInfo,pError);
							return SipFail;
						}
					}

					/*
					 * A sent 1xx has an effect on the txn state only if it
					 * had been in the SipTxnInvServerProceedingState
					 * state.
					 */
					if (pTxnBuffer->dTxnState==SipTxnInvServerProceedingState)
					{
						/*Change the state to SipTxnInvServer1xxSentState*/
						dResult=sip_changeTxnStateAndRelease \
							(pTxnBuffer,pTxnKey,SipTxnInvServer1xxSentState, \
							 pTxnInfo,pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
								dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
															   pTxnInfo,pError);
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;
					}
					else if (pTxnBuffer->dTxnState==SipTxnInvServer1xxSentState)
					{
						SIP_S8bit 			*pTempMsgBuffer=0;

						pTempMsgBuffer=pTxnInfo->pMsgBuffer;
						pTxnInfo->pMsgBuffer=pTxnBuffer->pBuffer;
						dResult=sip_txn_sendBuffer(pTxnInfo,pError);
						if (dResult!=SipSuccess)
						{
							/* 
							 * sendBuffer Failed ,release the Txn created for this
							 * and free the key. If the reference is passed to the
							 * application decrement the ref count. Make the BUffer
							 * Reference to NULL we will free in sip_txn_sendMessage
							 * anyway
							 */
							SipError tempError; 
							tempError = E_NO_ERROR;
							pTxnBuffer->pBuffer = SIP_NULL;
                            /* CSR_1-5943513 START */
							(void)sip_removeTxnNFreeKey(pTxnBuffer,pTxnKey, \
															   pTxnInfo,&tempError);
                            /* CSR_1-5943513 END */
							return SipFail;
						}
						pTxnInfo->dMesgSentAtLeastOnce=SipSuccess;
						pTxnInfo->pMsgBuffer=pTempMsgBuffer;
					}
					else
					{
						/*
						 * Catch the cases when an attempt is made to send
						 * a 1xx after sending a finalr response or after
						 * having had received an ACK.
						 */
						if ((pTxnBuffer->dTxnState== \
							SipTxnInvServerCompletedState)||\
							(pTxnBuffer->dTxnState== \
							SipTxnInvServerConfirmedState))
						dTxnWrongStateChange=SipSuccess;
					}
					break;
				}
			default:
				{
					/*
					 * Any other response causes it to move to the completed
					 * state only if it was not there already, also if a 
					 * message is sent when an ACK has already been receive 
					 * it is a similar error.
					 */

					/*
					 * For an INVITE server txn there is no difference 
					 * between a 2xx and 3xx-6xx reponse
					 */
					if (pTxnBuffer->dTxnState!=SipTxnInvServerConfirmedState)
					{
						SIP_S8bit	*pTempStr=SIP_NULL;
						SIP_S8bit	*pToBeFreed=SIP_NULL;

						if (pTxnBuffer->dTxnState==\
								SipTxnInvServerCompletedState)
						{
							SipBool	dResultTranspCheck;

							/*
							 * We allow sending a response for a second 
							 * time only if the transport address is
							 * different
							 */
							dResultTranspCheck=sip_compareTranspAddr\
								(pTxnBuffer->pAddr,pTxnInfo->pAddr);
							if (dResultTranspCheck==SipSuccess)
							{
								dTxnWrongStateChange=SipSuccess;
								break;
							}
						}
						
						dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
								pTxnInfo,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
								pTxnInfo,pError);
							return SipFail;
						}

						/*
						 * Change the To Tag of the pTxnBuffer to reflect the 
						 * To Tag present in the recently recvd response
						 */
						
						/*
						 * Get the ToTag from the message, if a TO Tag is not
						 * present then it actually means a violation of the
						 * protocol, however since we might need to maintain
						 * compatibility with older clients, the To Tags absence
						 * will be tolerated
						 */
						dResult = sip_getTagfromFromHeader\
							(pMesg->pGeneralHdr->pToHdr,&pTempStr,pError);		
						if ((dResult==SipSuccess) && (SIP_NULL != pTempStr))
						{
							pToBeFreed=pTxnBuffer->pTxnKey->pToTag;
							if (pToBeFreed!=SIP_NULL)
							{
								SipError	tempError;
								/*
								 * In case the initial key already had a To Tag
								 * then we need to free that.
								 */
								(void)fast_memfree(TXN_MEM_ID,pToBeFreed,&tempError);
							}

							pTxnBuffer->pTxnKey->pToTag = (SIP_S8bit *)\
									STRDUP((SIP_S8bit *)pTempStr);
							pTxnBuffer->pTxnKey->dTagCheck=SipSuccess;
						}
						/*
						 * The transport information within the buffer will 
						 * need to be updated.
						 */
						dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
						if (dResult==SipFail)
						{
							/*Handle failure here*/
							dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
								pTxnInfo,pError);
							return SipFail;
						}

						dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
							pTxnKey, SipTxnInvServerCompletedState,pTxnInfo,\
							pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
							    /*  1-7769493 - Start */
								/*dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
															   pTxnInfo,pError);*/
                                SipBool dResult; 
                                SipTxnKey *pTempKey=SIP_NULL; 
                                SIP_S8bit dRemove=0; 
                                SIP_Pvoid        pContextData=SIP_NULL; 
                                sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData); 
                                if (pTxnBuffer!=SIP_NULL) 
                                { 
#ifdef SIP_LINT 
                                    dRemove=2; 
#else 
                                    dRemove=TXN_REMOVE; 
#endif 
                                    /* klocworks warning removal */ 
                                    if(SIP_NULL != pContextData)
                                    {
                                           dResult= RELEASE_TXN((SIP_Pvoid)pTxnKey,(SIP_Pvoid *)&pTempKey, \
                                           (SIP_Pvoid *)(&pTxnBuffer), pContextData,dRemove,pError); 
                                    }
                                    else 
                                    {
                                          dResult = SipFail;
                                    }
  
                                    if (dResult!=SipSuccess) 
                                    { 
                                                return SipFail; 
                                    }
                                    /* This is set to NULL because the calling 
                                     * function frees this Buffer and if it will
                                     * be freed here then there are FUM observed
                                     */
                                    pTxnBuffer->pBuffer = SIP_NULL;
                                    sip_freeSipTxnBuffer(pTxnBuffer);
                                    /* This is the transaction key that is 
                                     * present in the node because the macro 
                                     * RELEASE_TXN removes the node but did 
                                     * not free the txn key present in node.
                                     */
                                    sip_freeSipTxnKey(pTempKey); 
                                } 
                                sip_freeSipTxnKey(pTxnKey); 
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;

						if(pTxnBuffer->dRespCode/100 == 2)
						{
							/*
							 * Set the via branch check to false. This is because when
							 * an ACK is rcvd to this 2xx response, the topmost Via
							 * branch will be different from what is present in the
							 * topmost Via of the INVITE. This flag will be checked in
							 * sip_txn_compareTxnKeys, and if FALSE, the via branch
							 * will not be checked. 
							 * In case of non-2xx response, the flag is left untouched 
							 * (SipSuccess set in constructor of the SipTxnKey) and 
							 * therefore, a strict check will be done for the topmost 
							 * Via branch.
							 */
							
							pTxnBuffer->pTxnKey->dViaBranchCheck = SipFail;
						}
					}
					else
					{
						/*
						 * This catches attempts to send a final response
						 * after having had already sent out one to the same 
						 * transport already. The case wherein we send a
						 * response for the second time after having had
						 * sent a response to a different transport is
						 * however allowd. Also sending a final response
						 * after having had recvd an ACK is trapped.
						 */
						dTxnWrongStateChange=SipSuccess;
					}
					break;
				}
		}
	}

	/*In case the state has not changed then the memory needs to be freed up*/
	if (!dStateChanged)
	{
		/*Release the txn and also free the key*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	if (dTxnWrongStateChange)
	{
		/*An error needs to be returned in this case*/
		*pError=E_TXN_INV_STATE;
		return SipFail;
	}

	SIPDEBUGFN("Exiting from function sip_updateInvSrvTxnForMesgSent");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateInvClTxnForMesgSent
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateInvClTxnForMesgSent
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	SipError *pError
)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	en_SipTxnClass	dTxnClass;
	SipBool dTxnExists=SipFail;
	SipBool dTxnInvState=SipFail;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateInvClTxnForMesgSent");

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Obtain the txn class from the TxnInformation*/
	dTxnClass=pTxnInfo->dTxnClass;

	/* Do a check for the type of message that had been sent*/
	switch(pTxnInfo->dMethodType)
	{
		case SipTxnMethodInvite:
			{
				/*Create the Txn-Key for this message*/
				dResult=sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*We do not set the To-Tag check in the generated key*/

				/*Now a new txn needs to be created*/
				dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,\
							pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*
				 * In case a txn already exists then we do not need 
				 * to do anything.
				 */
    			if (pTxnBuffer->dTxnState==SipTxnDefaultState)
				{
					/*
					 * Copy the buffer that has been sent into the 
					 * TxnBuffer for handling future retransX.
					 */
					dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
										pTxnInfo,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					/*
					 * Populate the TxnBuffer with the values, since
					 * this buffer is newly created.
					 */
					dResult=sip_populateTxnBuffer(pTxnBuffer,pTxnInfo, \
													pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						dResult=sip_releaseTxnNFreeKey\
							(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						return SipFail;
					}

					dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
								SipTxnInvClientCallingState,pTxnInfo,\
								pContextData,pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
						{
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
						}
						return SipFail;
					}
					dStateChanged=SipSuccess;
				}
				else
				{
					dTxnExists=SipSuccess;
				}

				if (!dStateChanged)
				{
					/*
					 * The txn needs to be released and also the Txn
					 * Key shud be freed.
					 */
					dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,\
							pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						return SipFail;
					}
				}
				break;
			}

		case SipTxnMethodAck:
			{
				SIP_S8bit dToTagChk=1;
				pTxnKey=SIP_NULL;

				/* 
				 * This fn fetches the txn corresponding to the ACK that
				 * had been received , also pTxnKey points to the key
				 * for this txn.
				 */
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo, \
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*
				 * Check if a txn was fetched and the txn is of the 
				 * same type as well.
				 */
				if (pTxnBuffer==SIP_NULL)
				{
					/*
					 * The txn needs to be released and also the Txn Key
					 * shud be freed
					 */
					dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,\
							pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						return SipFail;
					}
					*pError=E_TXN_NO_EXIST;
					return SipFail;
				}
				else
				{
					/*
					 * Now we have the txn correponding to the received 
					 * ACK. Action needs to be taken only if the txn is 
					 * in a SipTxnInvClientFinalResponseRecvState
					 */
					if (pTxnBuffer->dTxnState== \
										SipTxnInvClientFinalResponseRecvState)
					{
						/*
						 * Copy the buffer that has been sent into the 
						 * TxnBuffer so that any future remote retransX 
						 * can be handled.
						 */
						dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
											pTxnInfo,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							dResult=sip_releaseTxnNFreeKey\
								(pTxnBuffer,pTxnKey,pTxnInfo,pError);
							return SipFail;
						}

						/*
						 * The transport information within the buffer will
						 * need to be updated.
						 */
						dResult=sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
						if (dResult==SipFail)
						{
							/*
							 * The txn needs to be released and also the Txn 
							 * Key shud be freed.
							 */
							dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,\
															pTxnInfo,pError);
							return SipFail;
						}

						/*
						 * Change the state of the TxnBuffer and release
						 * the txnBuffer.
						 */
						dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
							pTxnKey, SipTxnInvClientCompletedState,pTxnInfo,\
							pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
								dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,\
															   pTxnInfo,pError);
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;
					}
					else
					{
						dTxnInvState=SipSuccess;
					}
				}

				if (!dStateChanged)
				{
					/*
					 * The txn needs to be released and also the Txn 
					 * Key shud be freed.
					 */
					dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,\
													pError);
					if (dResult!=SipSuccess)
					{
						/*Handle failure here*/
						return SipFail;
					}
				}
				break;
			}
		default:
				break;
	}

	/*
	 * This flag will be set for cases when an attempt is made to 
	 * resend an INVITE over an existing txn.
	 */
	if (dTxnExists)
	{
		*pError=E_TXN_EXISTS;
		return SipFail;
	}

	/*
	 * This flag will get set for cases when an attempt is made 
	 * to send an ACK before having had received a final response
	 */
	if (dTxnInvState)
	{
		*pError=E_TXN_INV_STATE;
		return SipFail;
	}

	SIPDEBUGFN("Exiting from function sip_updateInvClTxnForMesgSent");
	return SipSuccess;
}


/***************************************************************************
**** FUNCTION:sip_updateNonInvClTxnAfterMesgDecoding
****
****
**** DESCRIPTION:The Invite Client txn gets updated after receiving a response
******************************************************************************/
SipBool sip_updateNonInvClTxnAfterMesgDecoding
#ifdef ANSI_PROTO
(
	SipTxnInformn *pTxnInfo,
	en_SipTxnDecodeResult *pDecodeResult,
	SipError *pError
)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipMessage *pMesg=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dStateChanged=SipFail;
	SipTxnKey *pTxnKey=SIP_NULL;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateNonInvClTxnAfterMesgDecoding");
	
	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);
	
	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;


	/* Do a check for the type of message that had been sent*/
	switch(pTxnInfo->dRespClass)
	{
		case SipResponse1xx:
			{
				SIP_S8bit dToTagChk=0;

				/* 
				 * This fn fetches the txn corresponding to the 1xx that
				 * has been received .
				 */
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo, \
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*Check for a possible stray response*/
				if (pTxnBuffer==SIP_NULL)
				{
					*pDecodeResult=SipTxnStrayMessage;
				}
				else
				{
					if (pTxnBuffer->dTxnState==SipTxnNonInvClientTryingState)
					{
						/*Only in this state   1xx has an effect*/
						*pDecodeResult=SipTxnNonIgnorable;

						/*The peakoff of the retransX interval occurs here*/
						dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
							pTxnKey,SipTxnNonInvClientProceedingState, \
							pTxnInfo,pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
								dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;
					}
					else
					{
						/*
						 * If a 1xx has been already recvd then any 1xx
						 * needs to be passed on meaning it cannot be
						 * ignored
						 */
						if (pTxnBuffer->dTxnState==\
								SipTxnNonInvClientProceedingState)
							*pDecodeResult=SipTxnNonIgnorable;
						else
							*pDecodeResult=SipTxnIgnorable;
					}
				}
				break;
			}

		/*Same treatment for all final responses here*/
		case SipResponseFinal2xx:
		case SipResponseFinalNon2xx:
			{
				SIP_S8bit dToTagChk=0;

				/* 
				 * This fn fetches the txn corresponding to the final
				 * response that has been received .
				 */
				dResult=sip_fetchTxnUsingMessage(pMesg,dToTagChk,pTxnInfo,
					&pTxnKey,&pTxnBuffer,pError);
				if (dResult!=SipSuccess)
				{
					/*Handle failure here*/
					return SipFail;
				}

				/*Eliminate stray case*/
				if (pTxnBuffer==SIP_NULL)
				{
					/*This is a stray response*/
					*pDecodeResult=SipTxnStrayMessage;
				}
				else
				{
					if ((pTxnBuffer->dTxnState==SipTxnNonInvClientTryingState) \
										|| \
					(pTxnBuffer->dTxnState==SipTxnNonInvClientProceedingState))
					{
						SipTxnKey *pTempTxnKey=SIP_NULL;
						en_SipTxnType	dTxnType;
						
						/*A final response has an effect only in these states*/
						*pDecodeResult=SipTxnNonIgnorable;

						/*
						 * Now grab a reference to the TxnKey since if the 
						 * changeTxnStateAndRelease fn actually removes
						 * the txn, then we would not be able to get a
						 * reference to the key
						 */
						pTempTxnKey=pTxnBuffer->pTxnKey;
						HSS_LOCKEDINCREF(pTempTxnKey->dRefCount);
						dTxnType=pTxnBuffer->dType;
						
						/* 
						 * Increment Ref Count ,We need the reference
						 * to remove RPR Txn
						 */
						dResult=sip_changeTxnStateAndRelease(pTxnBuffer,
									pTxnKey, SipTxnNonInvClientCompletedState, \
									pTxnInfo,pContextData,pError);
						if (dResult!=SipSuccess)
						{
							/*Handle failure here*/
							if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
							{
								dResult=sip_releaseTxnNFreeKey\
									(pTxnBuffer,pTxnKey,pTxnInfo,pError);
							}
							return SipFail;
						}
						dStateChanged=SipSuccess;
						if (pTxnInfo->dMethodType==SipTxnMethodPrack)
						{
							/*
							 * In the Case Of PRACK Remove RPR Txn that 
							 * absorbs RPR Retransmissions, Only if it is 
							 * a UA Type Txn.
							 */

							if(dTxnType == SipUATypeTxn)
							{
								/*
								 * Remove the RPR Transaction If Final
								 * Response is Received. No need of checking
								 * for Stray Message. Since We have PRACK Txn
								 * Existing 
								 */
								dResult = sip_removeRPRTxn(pTempTxnKey,\
											pTxnInfo,SIP_NULL, pError);
								/*
								 * We dont need to check for return status here
								 * since if a removal is not succesful it means
								 * that a RPR txn did not xist here
								 */
							}
						}
						/*
						 * Free off the reference to the key that we had
						 * extracted from the TxnBuffer, since it is no 
						 * longer needed
						 */
						sip_freeSipTxnKey(pTempTxnKey);
					}
					else
					{
						/*Already in SipTxnNonInvClientCompletedState state*/
						*pDecodeResult=SipTxnIgnorable;
					}
				}
				break;
			}
		case SipResponseInvalid:
			{
				*pDecodeResult=SipTxnStrayMessage;
				break;
			}
	}


	if (!dStateChanged)
	{
		/*The txn needs to be released and also the Txn Key shud be freed*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}


	SIPDEBUGFN("Exiting function sip_updateNonInvClTxnAfterMesgDecoding");
	return SipSuccess;
}
/***************************************************************************
**** FUNCTION:sip_updateRPRTxnState
****
****
**** DESCRIPTION:This Function  Updates the states of all RPR Txns in the list
****************************************************************************/
SipBool sip_updateRPRTxnState
#ifdef ANSI_PROTO
	(SipList *pslRPRTxnKeyQ,en_SipTxnState dTxnState, \
	SIP_Pvoid pContextData, SipError *pError)
#else
	(pslRPRTxnKeyQ,dTxnState,pContextData,pError)
	SipList *pslRPRTxnKeyQ;
	en_SipTxnState dTxnState;
	SIP_Pvoid	pContextData;
	SipError *pError;
#endif
{
	SIP_U32bit count,dIndex=0;
	SipTxnBuffer* pTxnBuffer=SIP_NULL;
	SipTxnKey* pTxnKey=SIP_NULL;
	SIP_S8bit dCreate=0;
	SipBool dResult=SipFail;

	SIPDEBUGFN("Entering function sip_updateRPRTxnState");

	if(sip_listSizeOf(pslRPRTxnKeyQ, &count, pError) !=SipFail)
	{
		while(dIndex < count )
		{
			if(sip_listGetAt(pslRPRTxnKeyQ, dIndex, \
						(SIP_Pvoid *) &pTxnKey , pError) == SipFail)
				return SipFail;
#ifdef SIP_LINT
            dCreate=0;
#else
            dCreate=!TXN_CREAT;
#endif
			dResult = FETCH_TXN((SIP_Pvoid *)&pTxnBuffer,pTxnKey,\
					pContextData,dCreate,pError);
			if(dResult != SipFail)
			{
				HSS_LOCKEDINCREF(pTxnKey->dRefCount);
				
				/*
				 * This is needed since the context data that had been
				 * stored inside the TxnBuffer was that of the initial 
				 * INVITE txn and cannot be freed as such, since the 
				 * INVITE txn is very much alive
				 */
				if (dTxnState==SipTxnTerminatedState)
				{	
					pTxnBuffer->pEventContext=SIP_NULL;
				}
					
				/*
				 * The below function internally calls a free on the TxnKey
				 * passed to it and so we need to increment the refcount of 
				 * the key present in the Invite txn
				 */
				if(sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
							dTxnState,SIP_NULL,pContextData,pError)==SipFail)
					return SipFail;
			}
			dIndex++;
		}
	}

	SIPDEBUGFN("Exiting function sip_updateRPRTxnState");
	return SipSuccess;
}
/***************************************************************************
**** FUNCTION:sip_searchRPRTxnKeyListAndAdd
****
****
**** DESCRIPTION:This Function searches the specified key in the list
****			of RPR Txn Keys and if not found adds to the List.
****************************************************************************/
SipBool sip_searchRPRTxnKeyListAndAdd
#ifdef ANSI_PROTO
	(SipTxnBuffer *pTxnBuffer,SipTxnKey *pTxnKey,SipBool dCheckSeq,\
	SipError *pError)
#else
	(pTxnBuffer,pTxnKey,dCheckSeq,pError)
	SipTxnBuffer *pTxnBuffer;
	SipTxnKey *pTxnKey;
	SipBool dCheckSeq;
	SipError *pError;
#endif
{
	SIP_U32bit count,dIndex=0;
	SipTxnKey* pListKey;
	SIP_U32bit dTempRseq=0;
    /* CSR 1-7175208 Change Start*/
    /*SipBool dToTagMatch = SipSucces;*/
    SipBool dToTagMatch = SipFail;
    /* CSR 1-7175208 Change End*/
	SIPDEBUGFN("Entering function sip_searchRPRTxnKeyListAndAdd");
	/* Add this Key to the list of RPR Txns */
	if(pTxnBuffer->pslRPRTxnKeyQ == SIP_NULL)
	{
		pTxnBuffer->pslRPRTxnKeyQ = ( SipList * ) fast_memget(TXN_MEM_ID, \
				sizeof(SipList), pError);
		(void)sip_listInit(((pTxnBuffer)->pslRPRTxnKeyQ ),__sip_freeSipTxnKey,pError);
		if(sip_listAppend(pTxnBuffer->pslRPRTxnKeyQ, \
			(SIP_Pvoid)pTxnKey,pError) == SipFail)
			return SipFail;
		/*
		 * Now that the Key has been added into the Invite TxnBuffer's
		 * internal list we need to increment the refCount.
		 */
		HSS_LOCKEDINCREF(pTxnKey->dRefCount);
	}
	else
	{
		if(sip_listSizeOf(pTxnBuffer->pslRPRTxnKeyQ, &count, pError)!=SipFail)
		{
			while(dIndex < count )
			{
				if(sip_listGetAt(pTxnBuffer->pslRPRTxnKeyQ, dIndex, \
							(SIP_Pvoid *) &pListKey , pError) == SipFail)
					return SipFail;
                /* CSR 1-7175208 Change Start*/
                /*
				if(pListKey->dRseq == pTxnKey->dRseq)
				{
					if(strcmp(pListKey->pToTag,\
								pTxnKey->pToTag)==0)
					{
						return SipFail;
					}
					else
					{
						dToTagMatch = SipFail;
					}
				}
				dIndex++;
				dTempRseq =  pListKey->dRseq;
                */
                if((pListKey->pToTag != SIP_NULL) && (pTxnKey->pToTag != SIP_NULL)) 
                { 
                    if(strcmp(pListKey->pToTag,pTxnKey->pToTag)==0) 
                    { 
                        dTempRseq = pListKey->dRseq; 
                        dToTagMatch = SipSuccess; 
                        dIndex++; 
                        continue; 
                    } 
                } 
                dIndex++; 
                /* CSR 1-7175208 Change End*/ 
			}
			if(dCheckSeq && dToTagMatch == SipSuccess)
			{
				/* check the Sequence Number of RPR */
				/* Compare only when To Tag Matches */
				if(dTempRseq != pTxnKey->dRseq-1)
					return SipFail;
			}
			if(sip_listAppend(pTxnBuffer->pslRPRTxnKeyQ,(SIP_Pvoid)pTxnKey, \
				pError) == SipFail)
				return SipFail;
			/*
			 * Now that the Key has been added into the Invite TxnBuffer's
			 * internal list we need to increment the refCount.
			 */
			HSS_LOCKEDINCREF(pTxnKey->dRefCount);
		}
	}
	SIPDEBUGFN("Exiting function sip_searchRPRTxnKeyListAndAdd");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_getInvTxnStateFromPRACKTxnKey
****
****
**** DESCRIPTION:This Function Returns the Invite Txn State Corresponding\
***	 to the PRACK Txn
****************************************************************************/
SipBool sip_getInvTxnStateFromPRACKTxnKey
#ifdef ANSI_PROTO
(SipTxnKey *pTxnKey,SipTxnInformn *pTxnInfo, en_SipTxnState *pTxnState, \
	SipError *pError)
#else
	(pTxnKey,pTxnInfo,pTxnState,pError)
	SipTxnKey *pTxnKey;
	SipTxnInformn *pTxnInfo;
	en_SipTxnState *pTxnState;
	SipError *pError;
#endif
{
	SipTxnKey* pClonedKey=SIP_NULL;
	SipTxnBuffer* pTxnBuffer=SIP_NULL;
	SIP_S8bit dCreate;
	SipBool dResult=SipFail;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_getInvTxnStateFromPRACKTxnKey");


	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/* get RPRTxn Key From PRACK Txn */
	if(sip_getRPRTxnKeyFromPRACKTxnKey(pTxnKey,&pClonedKey,pError) == SipFail)
		return SipFail;
		
	/*
	 * Set the txn class to NonInviteSrver since we want to fetch 
	 * the RPR on the Server side corresponding to this PRACK.
	 */

	pClonedKey->dTxnClass=SipInviteClient;	

	/* for INVITE Txn There should not be RSeq */
	pClonedKey->dRseq = 0;

	/*
	 * Also since the Via Branches might not agree we need to do a lax
	 * Via-Branch check
	 */
	pClonedKey->dViaBranchCheck = SipFail;
	
	/* With out TXN_CREAT */
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while fetching 
     * the transaction, the pointer will be dereferenced
     */
	if(SIP_NULL != pContextData)
	{
	   dResult = FETCH_TXN((SIP_Pvoid *)&pTxnBuffer,pClonedKey,\
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;

	if(dResult != SipSuccess)
	{
		(void)sip_releaseTxnNFreeKey(pTxnBuffer,pClonedKey,pTxnInfo,pError);
		/*Nothing can be done in case the above call fails!!*/
		return SipFail;
	}
	else
	{
		/* return the state of Invite Txn */
		*pTxnState = pTxnBuffer->dTxnState;
	}

	if(sip_releaseTxnNFreeKey(pTxnBuffer,pClonedKey,pTxnInfo,pError) == SipFail)
			return SipFail;
	SIPDEBUGFN("Exiting function sip_getInvTxnStateFromPRACKTxnKey");
	return dResult;
}

/***************************************************************************
**** FUNCTION:sip_getRPRTxnKeyFromPRACKTxnKey
****
****
**** DESCRIPTION:This Function forms RPR Txn Key from PRACK Txn Key by cloning
****************************************************************************/
SipBool sip_getRPRTxnKeyFromPRACKTxnKey
#ifdef ANSI_PROTO
	(SipTxnKey *pTxnKey,SipTxnKey **ppClonedKey,\
	SipError *pError)
#else
	(pTxnKey, ppClonedKey,pError)
	SipTxnKey *pTxnKey;
	SipTxnKey **ppClonedKey;
	SipError *pError;
#endif
{
	SIP_S8bit* pTempMethod=SIP_NULL;
	SipTxnKey* pClonedKey = SIP_NULL;

	SIPDEBUGFN("Entering function sip_getRPRTxnKeyFromPRACKTxnKey");
	/* Make a Clone of this key as this is Modified */
	(void)sip_initSipTxnKey(&pClonedKey,pError);

    /* klcoworks: In case Init Key fails that means no more memory
     * is left in the system, therefore return failure
     */ 
     if(SIP_NULL == pClonedKey)
             return SipFail;
 
	(void)sip_cloneSipTxnKey(pClonedKey,pTxnKey,pError);

	/* With dTagCheck */
	pClonedKey->dTagCheck = SipSuccess;

   	/* Also it should match with the RPR sent */
	
	/* To match the PRACK with already sent RPR we need to change the key */
	pClonedKey->dCSeq = pClonedKey->pRackHdr->dIntVar2;
	pTempMethod = pClonedKey->pMethod;
	pClonedKey->pMethod = pClonedKey->pRackHdr->pStr1;
	pClonedKey->pRackHdr->pStr1 = pTempMethod;
	pClonedKey->dTxnClass=SipInviteClient;
	
	/*
	 * NOTE:The swapping of the methods ensures that the sip_freeSipRackHeader
	 * does not free up the method name that we have just added.
	 */

	pClonedKey->dRseq = pClonedKey->pRackHdr->dIntVar1;
	sip_txn_freeSipRAckHeader(pClonedKey->pRackHdr);
	pClonedKey->pRackHdr = SIP_NULL;
	*ppClonedKey = pClonedKey;
	SIPDEBUGFN("Exiting function sip_getRPRTxnKeyFromPRACKTxnKey");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_removeRPRTxn
****
****
**** DESCRIPTION:This Function Removes the RPR Txn Corresponding to this PRACK
****************************************************************************/
SipBool sip_removeRPRTxn
#ifdef ANSI_PROTO
	(SipTxnKey *pTxnKey,SipTxnInformn *pTxnInfo, \
	en_SipTxnDecodeResult *pDecodeResult, SipError *pError)
#else
	(pTxnKey, pTxnInfo,pDecodeResult,pError)
	SipTxnKey *pTxnKey;
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipTxnKey* pClonedKey=SIP_NULL;
	SipTxnBuffer* pTxnBuffer=SIP_NULL;
	SipBool dResult=SipFail;
	SIP_S8bit	dCreate;
	SIP_Pvoid pContextData=SIP_NULL;
    (void)pDecodeResult;

	SIPDEBUGFN("Entering function sip_removeRPRTxn");
	
    /*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/* get RPRTxn Key From PRACK Txn */
	if(sip_getRPRTxnKeyFromPRACKTxnKey(pTxnKey,&pClonedKey,pError) == SipFail)
		return SipFail;
	
	/*
	 * Set the txn class to NonInviteSrver since we want to fetch 
	 * the RPR on the Server side corresponding to this PRACK.
	 */
	pClonedKey->dTxnClass=SipInviteServer;
	pClonedKey->dViaBranchCheck=SipFail;

	/* With out TXN_CREAT */
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while fetching 
     * the transaction, the pointer will be dereferenced
     */
	if(SIP_NULL != pContextData)
	{
	    dResult= FETCH_TXN((SIP_Pvoid *)&pTxnBuffer,pClonedKey,
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;

	if (dResult!=SipSuccess)
	{
		/* This Means already deleted or Stray*/
		/* Continue.... */
		if(sip_releaseTxnNFreeKey(pTxnBuffer,pClonedKey,pTxnInfo,pError) \
				== SipFail)
			return SipFail;
		return SipFail;
	}
	else
	{
		/* Change the Txn State to Terminated State */
		if(sip_changeTxnStateAndRelease(pTxnBuffer,pClonedKey, \
			SipTxnTerminatedState,pTxnInfo,pContextData,pError)==SipFail)
			return SipFail;
	}

	SIPDEBUGFN("Exiting function sip_removeRPRTxn");
	return SipSuccess;

}

/***************************************************************************
**** FUNCTION:sip_updateRPRTxnForMesgSent
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateRPRTxnForMesgSent
#ifdef ANSI_PROTO
	(SipTxnInformn *pTxnInfo, SipError *pError)
#else
	(pTxnInfo,pError)
	SipTxnInformn *pTxnInfo;
	SipError *pError;
#endif
{
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey *pTxnKey=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipBool dResult;
	SipBool dStateChanged=SipFail;
	SIP_U32bit dTempRSeq=0;
	SIP_S8bit	dCreate;
	SIP_S8bit	dRemove;
	en_SipTxnClass	dTxnClass;
	SipBool dCheckSeq;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateRPRTxnForMesgSent");

	/* If Proxy type Txn dont create new Txn for RPR */
	if(pTxnInfo->pTxnContext->dTxnType == SipProxyTypeTxn)
	{
			return SipSuccess;
	}

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Extract the TxnClass from the TxnInformation*/
	dTxnClass=pTxnInfo->dTxnClass;
	
	/*Obtain the SipMessage from the TxnInformn*/
	pMesg=pTxnInfo->pSipMessage;

	/*Create the Txn-Key for this message*/
	dResult=sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/* 
	 * First Check For any Transaction Existing With This
	 * check With out Rack Header With To Tag Check.
	 */
	dTempRSeq = pTxnKey->dRseq;
	pTxnKey->dRseq = 0;
	pTxnKey->dTagCheck = SipSuccess;
	/* With Out TXN_CREAT */
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while fetching 
     * the transaction, the pointer will be dereferenced
     */
    if(SIP_NULL != pContextData)
	{
	    dResult= FETCH_TXN((SIP_Pvoid *)&pTxnBuffer,pTxnKey, \
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;
	
	if (dResult==SipFail)
	{
		/* If There is no Transaction return Failure */
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		*pError = E_TXN_NO_EXIST;
		return SipFail;
	}
	else
	{
		/* RPR Should not be sent once the Final Response is sent */
		if((pTxnBuffer->dTxnState == SipTxnInvServerCompletedState) \
			|| (pTxnBuffer->dTxnState == SipTxnInvServerConfirmedState) || \
			(pTxnBuffer->dTxnClass != SipInviteServer))
		{
			(void)sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			*pError = E_TXN_INV_STATE;
			return SipFail;
		}

		/* OtherWise Proceeed */
#ifdef SIP_LINT
           dRemove=0;
#else
           dRemove = !TXN_REMOVE;
#endif
		/* Sequence Number Check for RPR's need not be done */
		dCheckSeq = SipFail;
		/* copy back the dRseq We need RSeq in comparing*/
		pTxnKey->dRseq = dTempRSeq;
		/* search the list of RPR Txns if its present already */
		if( sip_searchRPRTxnKeyListAndAdd(pTxnBuffer,\
						pTxnKey,dCheckSeq,pError) == SipFail)
		{
			/* RPR Txn Already Present */
			/* Release the Txn */
			(void)sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			*pError = E_TXN_EXISTS;
			return SipFail;
		}
	}

	/*
	 * For releasing the Txn ,Rseq should not be present
	 * as we have fetched with out RSeq.
	 */
	pTxnKey->dRseq = 0;
#ifdef SIP_LINT
    dRemove=0;
#else
    dRemove=!TXN_REMOVE;
#endif
	(void)RELEASE_TXN(pTxnKey,SIP_NULL,\
				SIP_NULL,pContextData,dRemove,pError);
	/* copy back the dRseq ,We have to create RPR Txn*/
	pTxnKey->dRseq = dTempRSeq;
	
	/*
	 * Set the To-Tag check in the generated key.
	 * Now a new txn needs to be created
	 */
	pTxnKey->dTagCheck = SipSuccess;
	dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}

	/*
	 * In case a txn already exists then we do not need to do anything.
	 * Actually this case will not occur since any RPR Txn that would
	 * have got created would have an entry in the TxnBuffer fetched
	 * during the initial part of the function.
	 */
    if (pTxnBuffer->dTxnState==SipTxnDefaultState)
	{

		/*
		 * Copy the buffer that has been sent into the TxnBuffer
		 * for handling future retransX.
		 */
		dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
					pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey\
				(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			return SipFail;
		}

		/*Set the Txn Class*/
		pTxnBuffer->dTxnClass=pTxnInfo->dTxnClass;

		/*Populate the TxnBuffer with the values*/
		dResult=sip_populateTxnBuffer(pTxnBuffer,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey\
				(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			return SipFail;
		}
		dResult=sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
				SipTxnNonInvServerRPRSendingState,pTxnInfo,pContextData,\
				pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			if ( (*pError) != E_TXN_NETWORK_SEND_ERR )
			{
				dResult=sip_releaseTxnNFreeKey\
					(pTxnBuffer,pTxnKey,pTxnInfo,pError);
			}
			return SipFail;
		}
		dStateChanged=SipSuccess;
    }

	if (!dStateChanged)
	{
		/*The txn needs to be released and also the Txn Key shud be freed*/
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting function sip_updateRPRTxnForMesgSent");
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_updateRPRTxnAfterMesgDecoding
****
****
**** DESCRIPTION:
******************************************************************************/
SipBool sip_updateRPRTxnAfterMesgDecoding
#ifdef ANSI_PROTO
	(SipTxnInformn *pTxnInfo, en_SipTxnDecodeResult *pDecodeResult, \
		SipError *pError)
#else
	(pTxnInfo,pDecodeResult,pError)
	SipTxnInformn *pTxnInfo;
	en_SipTxnDecodeResult *pDecodeResult;
	SipError *pError;
#endif
{
	SipBool dResult;
	SipMessage *pMesg=SIP_NULL;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipTxnKey	*pTxnKey=SIP_NULL;
	SIP_U32bit dTempRSeq;
	SipBool dStateChanged=SipFail;
	SIP_S8bit	dCreate;
	en_SipTxnClass	dTxnClass;
	SipBool dCheckSeq;
	SIP_Pvoid pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_updateRPRTxnAfterMesgDecoding");
	/* If Proxy type Txn dont create new Txn for RPR */
	if(pTxnInfo->pTxnContext->dTxnType == SipProxyTypeTxn)
	{
			*pDecodeResult = SipTxnNonIgnorable;
			return SipSuccess;
	}

	/*Set the reference to the message*/
	pMesg=pTxnInfo->pSipMessage;

	/*Extract the SIP_Pvoid in the eventcontext of the passed TxnInformn*/
	sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

	/*Obtain the TxnClass from the TxnInformation*/
	dTxnClass=pTxnInfo->dTxnClass;
	
	/*Create the Txn-Key for this message*/
	dResult=sip_txn_createTxnKey(pMesg,dTxnClass,&pTxnKey,pError);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}
	/* Fetch With out To Tag Check and with out Rseq Header */
	pTxnKey->dTagCheck = SipFail;
	dTempRSeq = pTxnKey->dRseq;
	pTxnKey->dRseq = 0;

	/* With out TXN_CREAT */
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif

    /* klocworks: In case Context Data is not avaialable that means
     * call object information is not available , while fetching 
     * the transaction, the pointer will be dereferenced
     */
	if(SIP_NULL != pContextData)
	{
	    dResult= FETCH_TXN((SIP_Pvoid *)&pTxnBuffer,pTxnKey, \
				pContextData,dCreate,pError);
	}
	else
		dResult = SipFail;

	/*Check for a stray*/
	if ((dResult==SipSuccess)&&(pTxnBuffer->dTxnClass==SipInviteClient))
	{
		/*
		 * Copy back RSeq We need this info while searching 
		 * the list of RPR Txns.
		 */
		pTxnKey->dRseq = dTempRSeq;
		/* We need to check the Sequece NUmber of RPR's */
		dCheckSeq = SipSuccess;
		/* search the list of RPR Txns if its present already */
		if( sip_searchRPRTxnKeyListAndAdd(pTxnBuffer,\
						pTxnKey,dCheckSeq,pError) == SipFail)
		{
			/*
			 * This RPR Txn is already Present in the list or the Rseq
			 * may be out of sequence.In which case  Release the Txn.
			 */
			*pDecodeResult = SipTxnIgnorable;

			/* remove RSeq Since we fetched as Inv Txn */
			pTxnKey->dRseq = 0;

			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,\
				pError);
			return SipSuccess;
		}

		/* remove RSeq Since we fetched as Inv Txn */
		pTxnKey->dRseq = 0;
		
		/*
		 * An RPR is nothing but a diff lkind of a provisional response
		 * if one is recvd then the retransX of the INVITE txn must
		 * also get stopped
		 */
		if((pTxnBuffer->dTxnState == SipTxnInvClientCallingState )
			||(pTxnBuffer->dTxnState == SipTxnInvClientProceedingState))
		{
			dResult=glbSipTxnStateChange[SipTxnInvClientProceedingState]\
					(pTxnBuffer,pTxnKey,pContextData,pTxnInfo,pError);
		}
		
		/*
		 * Now we do not have any furthe use of the fetched Invite
		 * Client txn hence release the same.
		 */
		(void)RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pContextData,0,pError);

		/* RPR Txn needs to be created,copy back RSeq */
		pTxnKey->dRseq = dTempRSeq;
		/*Now create a new txn for RPR*/

		pTxnKey->dTagCheck = SipSuccess;

		dResult = sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo, \
					pError);
		if (dResult!=SipSuccess)
		{
			/*Handle failure here*/
			dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,\
				pError);
			return SipFail;
		}

		if (pTxnBuffer->dTxnState==SipTxnDefaultState)
		{
			*pDecodeResult = SipTxnNonIgnorable;

			/*Set the Txn Class*/
			pTxnBuffer->dTxnClass=pTxnInfo->dTxnClass;

			/*
			 * The transport information within the buffer will 
			 * need to be updated.
			 */
			if(sip_updateTxnBuffer(pTxnBuffer,pTxnInfo,pError) == SipFail)
				 return SipFail;

			if(sip_changeTxnStateAndRelease(pTxnBuffer,pTxnKey, \
				SipTxnNonInvClientRPRRecvngState,pTxnInfo,pContextData,\
				pError) == SipFail)
				return SipFail;
			dStateChanged=SipSuccess;
		}
	}
	else
	{
		*pDecodeResult =  SipTxnStrayMessage;
	}

	if (!dStateChanged)
	{
		dResult=sip_releaseTxnNFreeKey(pTxnBuffer,pTxnKey,pTxnInfo,pError);
		if (dResult!=SipSuccess)
		{
				/*Handle failure here*/
				return SipFail;
		}
	}
	SIPDEBUGFN("Exiting function sip_updateRPRTxnAfterMesgDecoding");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txnUpdateStats
**
**DESCRIPTION:This Function will be called whenever the statistics
**	          need to be updated
**
***********************************************************************/
void sip_txnUpdateStats
#ifdef ANSI_PROTO
	(SipTxnKey *pTxnKey)
#else
	(pTxnKey)
	SipTxnKey *pTxnKey;
#endif
{

	if(pTxnKey->dType == SipMessageResponse)
	{
		INC_API_RESP_SENT
	}
	else if(pTxnKey->dType == SipMessageRequest)
	{
		INC_API_REQ_SENT
	}
}

/***************************************************************************
**** FUNCTION:sip_freeSipTxnContext
****
****
**** DESCRIPTION:This API will be used to free down to the last level of
****			 TxnContext instances.
******************************************************************************/
SipBool	sip_freeSipTxnContext
#ifdef ANSI_PROTO
	(SipTxnContext *pTarget,SipError *pErr)
#else
	(pTarget,pErr)
	SipTxnContext *pTarget;
	SipError *pErr;
#endif
{
	SipEventContext *pTempContext=SIP_NULL;
#ifdef SIP_INCREMENTAL_PARSING	

	SipHdrTypeList *pHdrList=SIP_NULL;
#endif	
	SipTranspAddr *pTranspAddr=SIP_NULL;
	SIP_S8bit	*pHost=SIP_NULL;
	SipBool	dResult;

	SIPDEBUGFN("Entering into sip_freeSipTxnContext Function");

	/*Get the handle to everything that needs to be freed*/
	if (pTarget!=SIP_NULL)
		pTempContext=pTarget->pEventContext;
		else
		return SipSuccess;

#ifdef SIP_INCREMENTAL_PARSING
	if (pTempContext!=SIP_NULL) pHdrList=pTempContext->pList;
#endif	
	if (pTempContext!=SIP_NULL) pTranspAddr=pTempContext->pTranspAddr;
	if (pTranspAddr!=SIP_NULL) pHost=pTranspAddr->pHost;

	/*Start freeing*/

	if (pTempContext!=SIP_NULL)
	{
		dResult=fast_memfree(TXN_MEM_ID,pTempContext,pErr);
		if (dResult==SipFail)
		{
			return SipFail;
		}
	}

#ifdef SIP_INCREMENTAL_PARSING
	if (pHdrList!=SIP_NULL)
	{
		dResult=fast_memfree(TXN_MEM_ID,pHdrList,pErr);
		if (dResult==SipFail)
		{
			return SipFail;
		}
	}
#endif
	if (pTranspAddr!=SIP_NULL)
	{
		dResult=fast_memfree(TXN_MEM_ID,pTranspAddr,pErr);
		if (dResult==SipFail)
		{
			return SipFail;
		}
	}

	if (pHost!=SIP_NULL)
	{
		dResult=fast_memfree(TXN_MEM_ID,pHost,pErr);
		if (dResult==SipFail)
		{
			return SipFail;
		}
	}

	/*Set the mother of all problems i.e SipEventContext to SIP_NULL*/
	pTarget->pEventContext=SIP_NULL;

	SIPDEBUGFN("Exiting from sip_freeSipTxnContext Function");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************************
**** FUNCTION:sip_cloneSipTxnContext
****
****
**** DESCRIPTION:This API will be used internall to clone a SipTxnContext.
******************************************************************************/
SipBool	sip_cloneSipTxnContext
#ifdef ANSI_PROTO
	(SipTxnContext *pSrc, SipTxnContext *pDest,SipError *pErr)
#else
	(pSrc,pDest,pErr)
	SipTxnContext *pSrc;
	SipTxnContext *pDest;
	SipError *pErr;
#endif
{
	SipEventContext *pTempContext=SIP_NULL;

	SIPDEBUGFN("Entering into sip_cloneSipTxnContext Function");

	*pErr=E_NO_ERROR;

	if ((pSrc==SIP_NULL)||(pDest==SIP_NULL))
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	/*Copy the rest of the values*/
	pDest->timeoutValues=pSrc->timeoutValues;
	pDest->dTxnType=pSrc->dTxnType;
	pDest->txnOption=pSrc->txnOption;

	if (pSrc->pEventContext!=SIP_NULL)
	{
		/*
		 * Prepare the skeleton of the target i.e allocate memory 
		 * for the internal member EventContext.
		 */
		pTempContext=(SipEventContext *)fast_memget(TXN_MEM_ID,\
			sizeof(SipEventContext),pErr);
		if (pTempContext==SIP_NULL)
		{
			return SipFail;
		}
		*pTempContext=*(pSrc->pEventContext);
	}
	else
		return SipSuccess;


#ifdef SIP_INCREMENTAL_PARSING
	if (pSrc->pEventContext->pList!=SIP_NULL)
	{
		/*Copy the contents of the HdrType List*/
		pTempContext->pList=SIP_NULL;
		pTempContext->pList=(SipHdrTypeList *)fast_memget(TXN_MEM_ID,\
			sizeof(SipHdrTypeList),pErr);
		if (pTempContext->pList==SIP_NULL)
		{
			return SipFail;
		}
		*(pTempContext->pList)=*(pSrc->pEventContext->pList);
	}

#endif

	if (pSrc->pEventContext->pTranspAddr!=SIP_NULL)
	{
		/*Copy the contents of the Transport Address*/
		pTempContext->pTranspAddr=SIP_NULL;
		pTempContext->pTranspAddr=(SipTranspAddr *)fast_memget(TXN_MEM_ID,\
			sizeof(SipTranspAddr),pErr);
		if (pTempContext->pTranspAddr==SIP_NULL)
		{
			return SipFail;
		}
		*(pTempContext->pTranspAddr)=*(pSrc->pEventContext->pTranspAddr);

		/*User data cannot be cloned hence setting to SIP_NULL*/
		pTempContext->pTranspAddr->pData=SIP_NULL;

		/*The pHost value can be deep copied*/
		pTempContext->pTranspAddr->pHost=SIP_NULL;

		pTempContext->pTranspAddr->pHost=\
			sip_strdup(pSrc->pEventContext->pTranspAddr->pHost,TXN_MEM_ID);
		if (pTempContext->pTranspAddr->pHost==SIP_NULL)
		{
			*pErr=E_NO_MEM;
			return SipFail;
		}
	}

	/*The direct Buffer needs to be set to SIP_NULL since it is user data*/
	pTempContext->pDirectBuffer=SIP_NULL;
	pTempContext->pData=SIP_NULL;

	/*Copy stuf from the eventContext of src to this*/
	pDest->pEventContext=pTempContext;

	SIPDEBUGFN("Exiting from sip_cloneSipTxnContext Function");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_compareTranspAddr
**
**DESCRIPTION:This Function compares two transport adrresses
**				and returns SipSuccess if they are the same.
**
***********************************************************************/
SipBool sip_compareTranspAddr
#ifdef ANSI_PROTO
	(SipTranspAddr	*pTransp1,SipTranspAddr	*pTransp2)
#else
	(pTransp1,pTransp2)
	SipTranspAddr	*pTransp1;
	SipTranspAddr	*pTransp2;
#endif
{
	int iCount;
	
	SIPDEBUGFN("Entering into sip_compareTranspAddr Function");
	
	/*The value 16 is the size of a standard IP Address*/
	for(iCount=0;iCount<SIP_IPADDR_SIZE;iCount++)
	{
		if (pTransp1->dIpv4[iCount]!=pTransp2->dIpv4[iCount])
			return SipFail;
	}

	if (pTransp2->dPort!=pTransp1->dPort)
		return SipFail;
	
	if ((pTransp1->pHost!=SIP_NULL)&&(pTransp2->pHost!=SIP_NULL))
	{
		if (sip_strcmp(pTransp2->pHost,pTransp1->pHost))
			return SipFail;
	}

	SIPDEBUGFN("Exiting into sip_compareTranspAddr Function");
	return SipSuccess;
}	

/***********************************************************************
**FUNCTION: sip_startTimerCr
**
**DESCRIPTION:This function starts the Timer Cr on the Txn buffer that
**			  is passed. It also takes care to send the indicate Timeout
**			  callback to the application, that is interested in receipt of 
**			  a callback upon the expiry of this timer
***********************************************************************/
SipBool	sip_startTimerCr
#ifdef ANSI_PROTO
	(SipTxnBuffer *pTxnBuffer,SipTxnKey *pTxnKey,SipEventContext *pContext,\
	SipTxnTimeoutData	*pTimeOutData, SipError *pErr)
#else
	(pTxnBuffer,pTxnKey,pContext,pTimeOutData,pErr)
	SipTxnBuffer	*pTxnBuffer;
	SipTxnKey		*pTxnKey;
	SipEventContext	*pContext;
	SipTxnTimeoutData	*pTimeOutData;
	SipError	*pErr;
#endif
{
	SIP_U32bit	dDuration;
	
	SIPDEBUGFN("Entering into sip_startTimerCr Function");
	dDuration=pTxnBuffer->timeoutValues.dTimerCr;
	
	/*
	 * Call the indicateTimeout,also needs to be noted that the appln 
	 * shudnt free the Event Context in this case since we will need
	 * it for later handling in the txn.
	 */
	if (sip_throwTimeOutCbk(SipTimerC,pTxnBuffer->dTimeoutCbkBitMask))
	{	
		/*
		 * The application has indicated interest in the 
		 * receipt of this cbk.
		 */
		INDICATE_TIMEOUT(pContext,SipTimerC);
	}	
	
	pTimeOutData->dTimerStarted = SipTimerCr; 
	
	if( START_TIMER(dDuration, (TimeoutFuncPtr)timerTimeOut,\
			(SIP_Pvoid)pTimeOutData,(SIP_Pvoid*)&(pTxnBuffer->pTimerHandle),\
			pErr) == SipFail )
	{	
#ifdef SIP_LINT
			(void)RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pTimeOutData->pContextData,0,pErr);
#else        
			RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pTimeOutData->pContextData,!TXN_REMOVE,pErr);
#endif            
			return SipFail;
	}	

	pTxnBuffer->dTimerStarted = SipTimerCr;

	SIPDEBUGFN("Exiting from  sip_startTimerCr Function");
	return SipSuccess;
}	


/***********************************************************************
**FUNCTION: sip_getCulpritTimer
**
**DESCRIPTION:We have a lot of timers that are started in tandem. For ex
**			  (Timer A)+(Timer B),(Timer E)+(Timer F). Now the basic txn
**			  layer architecture does not lend itself to simultaneous start	
**			  of multiple timers for a single txn. To get arnd this we 
**			  start the MIN(dur(x),dur(y)). In this function we determine
**			  which timer is the one that had fired.
***********************************************************************/
en_SipTimerType sip_getCulpritTimer
#ifdef ANSI_PROTO
	(SipTxnBuffer	*pTxnBuffer)
#else
	(pTxnBuffer)
	SipTxnBuffer	*pTxnBuffer;
#endif
{
	en_SipTimerType	dCulpritTimer;
	
	SIPDEBUGFN("Entering into sip_getCulpritTimer Function");
	/*Now which timer do we give in the indicate timeout??*/
	switch(pTxnBuffer->dTimerStarted)
	{
		case SipTimerA_B:
		case SipTimerB:
			/*
			 * A slightly shady way of determining if TimerC had
			 * fired or TimerB had fired, but it works!!!
			 */
			if (pTxnBuffer->dType==SipProxyTypeTxn)
			{
				if ((pTxnBuffer->timeoutValues.dTimerB)\
					<(pTxnBuffer->timeoutValues.dTimerC))
					dCulpritTimer=SipTimerB;
				else
				{
					dCulpritTimer=SipTimerC;
				}
			}
			else
				dCulpritTimer=SipTimerB;
			break;

		case SipTimerE_F:
			dCulpritTimer=SipTimerF;
			break;

		case SipTimerG_H:
			dCulpritTimer=SipTimerH;
			break;
			

		default:
			dCulpritTimer=pTxnBuffer->dTimerStarted;
			break;
	}
	
	SIPDEBUGFN("Exiting from sip_getCulpritTimer Function");
	return dCulpritTimer;
}

/***********************************************************************
**FUNCTION: sip_throwTimeOutCbk
**
**DESCRIPTION:This Function decides on the basis of the bitmask stored
**			  within SipTxnBuffer whether the application is interested 
**			  in receiving the timeOut callback or not
**
***********************************************************************/
SipBool sip_throwTimeOutCbk
#ifdef ANSI_PROTO
	(en_SipTimerType	dCulpritTimer,SIP_U32bit	dBitMask)
#else
	(dCulpritTimer,dBitMask)
	en_SipTimerType	dCulpritTimer;
	SIP_U32bit	dBitMask;
#endif
{
	SIP_U32bit	dWhichTimer=0;
	
	SIPDEBUGFN("Entering into sip_throwTimeOutCbk Function");
	switch(dCulpritTimer)
	{
		case SipTimerB:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_B;
			break;
			
		case SipTimerC:
		case SipTimerCr:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_C;
			break;
			
		case SipTimerD:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_D;
			break;
			
		case SipTimerF:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_F;
			break;
			
		case SipTimerH:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_H;
			break;
			
		case SipTimerI:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_I;
			break;
			
		case SipTimerJ:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_J;
			break;
			
		case SipTimerK:
			dWhichTimer=SIP_ISSUE_CBKFOR_TIMER_K;
			break;
			
		default:
			dWhichTimer=0;
	}
	
	SIPDEBUGFN("Exiting from sip_throwTimeOutCbk Function");
	
	if (!dWhichTimer)
		return SipFail;
		
	if (dBitMask&dWhichTimer)
		return SipSuccess;
		else
		return SipFail;	
	
}

/***********************************************************************
**FUNCTION: timerTimeOut
**
**DESCRIPTION:This Function will be called by timer Module when any
	      of the timerstarted by fast_start_timer is timedOut.
**
***********************************************************************/
SipBool timerTimeOut
#ifdef ANSI_PROTO
	(SIP_Pvoid pData)
#else
	(pData)
	SIP_Pvoid pData;
#endif
{
	SIP_U32bit tempDur;
	SipTxnKey *pTxnOutKey=SIP_NULL;
	SIP_U32bit nextTimerDuration;
	SipTxnBuffer* pTxnBuffer=SIP_NULL;
    SIP_S8bit dTranspType;
	SipError err;
	SipTxnTimeoutData *pTxnTimeoutData=SIP_NULL;
	SipTxnKey* pTxnKey=SIP_NULL;
	SIP_U32bit dDur;
	SipBool dRemoveTxn = SipFail;
	SipBool	dDeleteRPRList = SipFail;
	SipBool dGiveTimeoutIndicn=SipFail;
	SIP_S8bit dCreate=0;
	SIP_Pvoid pContextData=SIP_NULL;

#ifdef SIP_RETRANSCALLBACK
	SIP_U32bit dOldDur;
#endif


#ifdef SIP_TRACE
	SIP_S8bit tracebuf[SIP_TRACE_BUF_SIZE];
	char msg[SIP_FIRST_20_BYTES];
#endif

	SIPDEBUGFN("Entering timerTimeOut Function");
	/* Key is Stored inside the Wrapper */
	pTxnTimeoutData = (SipTxnTimeoutData*) pData;
	pTxnKey = (SipTxnKey*)pTxnTimeoutData->pKey ;

	pContextData=pTxnTimeoutData->pContextData;	
	/* Fetch Transactiopn into pBuffer With Out TXN_CREAT*/
#ifdef SIP_LINT
    dCreate=0;
#else
    dCreate=!TXN_CREAT;
#endif
	if(FETCH_TXN((SIP_Pvoid*)&pTxnBuffer,pTxnKey,pContextData,
		dCreate,&err) == SipFail)
	{
		/* Transaction deleted Seems to be Some Problem?*/
		/* Free the Key*/
		sip_freeSipTxnTimeoutData(pTxnTimeoutData);
		return SipFail;
	}

	/* 
	 * If there is any descrepency inside the timer Type
	 * Stored in the Txn Buffer and timerType Stored in
	 * the Wrapper,This Means some body stopped the timer
	 * and Started another timer.Just return in this case.
	 */

	if( pTxnTimeoutData->dTimerStarted != pTxnBuffer->dTimerStarted)
	{	
#ifdef SIP_LINT
        (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pContextData,0,&err);
#else
        RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pContextData,!TXN_REMOVE,&err);
#endif
		return SipSuccess;
	}	
	
	/*
	 * We had been given a handle by the application during the time we had
	 * requested for a timer to be started. At this point however since the
	 * timer has expired the handle also needs to be freed. The
	 * sip_freeTimerHandle callback can be used by the stack to free the
	 * timer handle. This is a callback that is provided by the application
	 * using which the stack can free the timer handle when it no longer
	 * needs it. Other places where this callback is invoked are the points
	 * when the stackinvokes the fast_stopTxnTimer callbacks. At this point
	 * also the TimerHandle is nolonger necessary and can be freed. It
	 * needs to be noted here that the timer handle can also be freed by
	 * the application rather than having the stack issuing a callback. In
	 * C++ this can be achieved by not registering the freeTimerHandle()
	 * cbk with the singleton SipWSTack class, which would result in the
	 * callback not getting invoked. In "C" the application would have to
	 * implement the callback as a plain entry exit fn, with no code. The
	 * cbk would get invoked , however the handle wuld not get freed. The
	 * application in the above two cases must itself take the
	 * responsibility of freeing the time handle however
	 */
	FREE_TIMER_HANDLE(pTxnBuffer->pTimerHandle);
	pTxnBuffer->pTimerHandle =  SIP_NULL;

	switch(pTxnBuffer->dTimerStarted)
	{
		case SipTimerA_B:
		case SipTimerE_F:
		case SipTimerG_H:
			/* store the old Duration */
#ifdef SIP_RETRANSCALLBACK
			dOldDur = pTxnBuffer->dDuration;
#endif
			/* If reaches Maximum duaration Remove Transaction */
			if(pTxnBuffer->dCumulativeDuration == pTxnBuffer->dMaxDuration)
			{
				/* Remove the Txn */
				dRemoveTxn = SipSuccess;
				dGiveTimeoutIndicn=SipSuccess;
				break;
			}
			if(pTxnBuffer->dTimerStarted == SipTimerA_B)
				/* for timer A next value is 2*T1 */
				nextTimerDuration = pTxnBuffer->dDuration << 1;
			else
			{
				/* for timerE_F and timerG_H we have to take min of (2*T1,T2)*/
				tempDur = pTxnBuffer->dDuration << 1;
				dDur = pTxnBuffer->timeoutValues.dT2;
				nextTimerDuration = MIN(tempDur, dDur);
			}

			/* Caluculate the next Cumulative Duaration */
			tempDur = nextTimerDuration + pTxnBuffer->dCumulativeDuration;

			/* reteransmit the message in the BUffer */
        	if(pTxnBuffer->pEventContext != SIP_NULL)
            	   pTxnBuffer->pAddr->pData = pTxnBuffer->pEventContext->pData;

	    	dTranspType = pTxnBuffer->dTranspType;

        	if(SEND_TO_NETWORK(pTxnBuffer->pBuffer, \
				pTxnBuffer->dBufferLength,pTxnBuffer->pAddr, dTranspType, \
				&err) == SipFail)
			{
				sip_error (SIP_Major, "SIP_ERROR: Cannot send to network");
				INC_ERROR_NETWORK
/* Txn key with txn buffer should be freed */
#if 0 
#ifdef SIP_LINT
                (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
				SIP_NULL,pContextData,0,&err);
#else
                RELEASE_TXN(pTxnKey,SIP_NULL,\
				SIP_NULL,pContextData,!TXN_REMOVE,&err);
#endif
#endif
                                /* UATK CSR 1-6056530 fix */
                                (void)RELEASE_TXN(pTxnKey,(SIP_Pvoid*)&pTxnOutKey,\
                                (SIP_Pvoid*)&pTxnBuffer,pContextData,TXN_REMOVE,&err);
                
                                sip_freeEventContext(pTxnBuffer->pEventContext);
                                /*Mark this fact in the TxnBuffer that the event context is not owned
                                by the TxnBuffer any more*/
                                pTxnBuffer->pEventContext = SIP_NULL;

                                sip_freeSipTxnBuffer(pTxnBuffer);
                                sip_freeSipTxnKey(pTxnOutKey);
                                sip_freeSipTxnTimeoutData(pTxnTimeoutData); 
                                return SipFail;
			}
			/* Increment the Retransmission Count */
			pTxnBuffer->dRetransCount++;
#ifdef SIP_RETRANSCALLBACK
			if((pTxnBuffer->dEnableRetransCallback) && (pTxnBuffer->pEventContext != SIP_NULL))
			{
				sip_indicateMessageRetransmission\
					(pTxnBuffer->pEventContext, pTxnKey, pTxnBuffer->pBuffer,\
					 pTxnBuffer->dBufferLength,pTxnBuffer->pAddr,\
					 pTxnBuffer->dRetransCount, dOldDur);
			}
#endif
#ifdef SIP_TRACE
			(void)sip_strncpy (msg, (SIP_S8bit *)pTxnBuffer->pBuffer, 20);
			msg[20]='\0';
			(void)sip_trace(SIP_Brief,SIP_Outgoing,(SIP_S8bit *) "Message sent out.");
			(void)sip_trace(SIP_Detailed,SIP_Outgoing,(SIP_S8bit *)\
			                                    pTxnBuffer->pBuffer);
			(void)HSS_SPRINTF (tracebuf,"Timer Value: %d\n"\
							"Total Duration: %d\nMsg"\
							"(1st 20 bytes) : \"%s\" ",pTxnBuffer->dDuration,\
							pTxnBuffer->dMaxDuration,msg);
			(void)sip_trace(SIP_Brief, SIP_Outgoing, tracebuf);
#endif

			/*The statistics need to be updated also for retransmissions*/
			sip_txnUpdateStats(pTxnKey);


			/* If Cumulative less than the Maximum call start timer one
							more time */
			if(tempDur <= pTxnBuffer->dMaxDuration)
			{
				pTxnBuffer->dDuration = nextTimerDuration;
				pTxnBuffer->dCumulativeDuration = tempDur;
				if( START_TIMER(pTxnBuffer->dDuration, \
					(TimeoutFuncPtr)timerTimeOut, \
					(SIP_Pvoid)pTxnTimeoutData, \
					(SIP_Pvoid*)&(pTxnBuffer->pTimerHandle), \
					&err) == SipFail )
				{
#ifdef SIP_LINT
                     (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
						SIP_NULL,pContextData,0,&err);
#else
                    RELEASE_TXN(pTxnKey,SIP_NULL,\
						SIP_NULL,pContextData,!TXN_REMOVE,&err);
#endif                    
					return SipFail;
				}	
			}
			else
			{
				/* get the remaining time Duration */
				tempDur = (pTxnBuffer->dMaxDuration) - \
								(pTxnBuffer->dCumulativeDuration);
				pTxnBuffer->dCumulativeDuration = pTxnBuffer->dMaxDuration;
				if( START_TIMER(tempDur, (TimeoutFuncPtr)timerTimeOut, \
					(SIP_Pvoid)pTxnTimeoutData, \
					(SIP_Pvoid*)&(pTxnBuffer->pTimerHandle), \
					&err) == SipFail )
				{	
#ifdef SIP_LINT					
                    (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
						SIP_NULL,pContextData,0,&err);
#else
                    RELEASE_TXN(pTxnKey,SIP_NULL,\
						SIP_NULL,pContextData,!TXN_REMOVE,&err);
#endif                    
					return SipFail;
				}	
			}
#ifdef SIP_LINT			
            (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pContextData,0,&err);
#else
            RELEASE_TXN(pTxnKey,SIP_NULL,\
					SIP_NULL,pContextData,!TXN_REMOVE,&err);  
#endif            
			dGiveTimeoutIndicn=SipSuccess;
			break;
			
		case SipTimerCr:
			{
				dGiveTimeoutIndicn=SipSuccess;
				dRemoveTxn = SipSuccess;
				break;
			}
			
		case SipTimerC:
			{
				dDeleteRPRList = SipSuccess;
				dGiveTimeoutIndicn=SipSuccess;
				dRemoveTxn = SipSuccess;
				break;
			}
		case SipTimerH:
			{
				dGiveTimeoutIndicn=SipSuccess;
				dRemoveTxn = SipSuccess;
				break;
			}
		case SipTimerB:
			{
				dGiveTimeoutIndicn=SipSuccess;
				dRemoveTxn = SipSuccess;
				break;
			}
		case SipTimerForProxyUAS2xxFwded:
			{
				dRemoveTxn = SipSuccess;
				break;
			}
		case SipTimerF:
			{
				dGiveTimeoutIndicn=SipSuccess;
				dRemoveTxn = SipSuccess;
				break;
			}
		case SipTimerI:
			{
				if (pTxnBuffer->dTxnState==SipTxnNonInvClientRPRCompletedState)
				{
					/*
					 * In this case we have to time out the RPR client
					 * txns on the UAC side, these were maintained in the
					 * first place to eat up retransX of the RPRs,
					 * receipt of a final response caused their downfall,
					 * however we need not throw a callback in this case
					 * since this shud be a transparent state maintenance,
					 * they were not removed immediately and were maintained
					 * for Timer I period
					 */
					dGiveTimeoutIndicn=SipFail;
					dRemoveTxn = SipSuccess;
					break;
				}
#ifdef SIP_LINT 
        else
				{       
          dRemoveTxn = SipSuccess;
          dGiveTimeoutIndicn=SipSuccess;
          break;
        }
#endif                

			}
		case SipTimerD:
		case SipTimerJ:
		case SipTimerK:
			dRemoveTxn = SipSuccess;
			dGiveTimeoutIndicn=SipSuccess;
			break;
		case SipTimerForCancelledState:	
			dRemoveTxn = SipSuccess;
			dGiveTimeoutIndicn=SipFail;/*No need of callback*/
			break;
			/* For All Other Timers do the same thing */
		default:
			dRemoveTxn = SipSuccess;
			dGiveTimeoutIndicn=SipFail;
			break;
	}
	
	if (dRemoveTxn)
	{ /*
		Here we remove the txn that has fired, the timer cud be one time
		fire like timer K or the guys who keep firing for a fixed duration.
		We need to trap the firing of timer C here. For Timer C there is a 
		different treatment. When Timer C fires this information needs to 
		be given to the application via a callback sip_indicateTimeOut.
		However the default operation dictates that after the callback is 
		givn the txn is removed(the event context is freed by the application
		in the cbk).For Timer C though we need to give some more time to the
		application since it would send a CANCEL on the initial INVITE. This
		addnl time comes thru Timer Cr. This timer is set for one time firing
		, it is NOT reset like Timer C when provisional responses come in.
		When Timer C fires there is an important assumption that the 
		application does **NOT** free the event context
		*/
		
		en_SipTimerType	dWhichTimer;
		SipEventContext *pTempEventContext=SIP_NULL;

		/*Get the event context that wil be given to the indicateTimeOut 
			callbacks*/
		pTempEventContext=pTxnBuffer->pEventContext;
		
		/*Mark this fact in the TxnBuffer that the event context is not owned
			by the TxnBuffer any more*/
		pTxnBuffer->pEventContext=SIP_NULL;
			
		/*We need to determine if the Timer C had fired,*/
		dWhichTimer=sip_getCulpritTimer(pTxnBuffer);
		
		if ((dWhichTimer==SipTimerC)&&(pTxnBuffer->dType==SipProxyTypeTxn))
		{
			/*SipBool	dRetVal=SipFail;*/
			
			dDeleteRPRList=SipSuccess;
			
			/*
			 * In case the timeOut is happening for the TimerC then
			 * if there are any RPR handling txns created they too
			 * should go, if pslRPRTxnKeyQ is not NULL change the 
			 * RPR Txns to Terminated State 
			 */
			if((dDeleteRPRList == SipSuccess)&& \
				(pTxnBuffer->pslRPRTxnKeyQ!=SIP_NULL))
			{
				/* Change the State of all RPR Txns to Terminated State */
				if(sip_updateRPRTxnState(pTxnBuffer->pslRPRTxnKeyQ,\
						SipTxnTerminatedState,pContextData,&err) == SipFail)
					return SipFail;
			}
			
			/*Start Timer Cr on this txn and give the timeout indication*/
			(void)sip_startTimerCr(pTxnBuffer,pTxnKey,pTempEventContext,\
									pTxnTimeoutData,&err);

			/*
			 * Now we need to set the pEventContext back into the TxnBuffer
			 * since the TimerC indicateTimeOut cbk would not have freed
			 * the eventcontext since the application is forbidden from
			 * doing so.
			 */
			pTxnBuffer->pEventContext=pTempEventContext;

			/*No need to remove this Txn, we only need to release the txn*/
#ifdef SIP_LINT
             (void)RELEASE_TXN(pTxnKey,SIP_NULL,\
				SIP_NULL,pContextData,0,&err);
#else
            RELEASE_TXN(pTxnKey,SIP_NULL,\
				SIP_NULL,pContextData,!TXN_REMOVE,&err);
#endif
		}
		else
		{
			(void)RELEASE_TXN(pTxnKey,(SIP_Pvoid*)&pTxnOutKey,\
				(SIP_Pvoid*)&pTxnBuffer,pContextData,TXN_REMOVE,&err);
			
			/* Call Indicate Time Out */
			if (dGiveTimeoutIndicn)
			{
				if (sip_throwTimeOutCbk(dWhichTimer,\
					pTxnBuffer->dTimeoutCbkBitMask))
				{	
					/*
					 * The application has indicated interest in the 
					 * receipt of this cbk.
					 */
					INDICATE_TIMEOUT(pTempEventContext,dWhichTimer);
					pTempEventContext=SIP_NULL;
				}	
			}
			
			sip_freeSipTxnBuffer(pTxnBuffer);
			sip_freeSipTxnKey(pTxnOutKey);

			/*
			 * In case the indicateTimeOut has not been called
			 * then we ourselves need to free the eventContext.
			 */
			if (pTempEventContext!=SIP_NULL)
			{
				FREE_EVENT_CONTEXT(pTempEventContext);
			}
			sip_freeSipTxnTimeoutData(pTxnTimeoutData);
		}
	}	
	
	SIPDEBUGFN("Exiting from timerTimeOut Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_startTimersWithTranspCheck
**
**DESCRIPTION:This starts two timers in case UDP and one timer in case of TCP
**	 	In the case of UDP starting two timers is implemented by only
**		one timer,by setting maxdutaion value as second timer value and
**		first timer Value to the duration.In the case of TCP only one
**		timer is started with the duration of second timer value.
**
**
***********************************************************************/

SipBool sip_txn_startTimersWithTranspCheck
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer, SipTxnKey* pTxnKey, \
	SIP_Pvoid pContextData,en_SipTimerType dTimer1, en_SipTimerType dTimer2, \
	SIP_U32bit dDuration1,SIP_U32bit dDuration2,\
	SipBool dTranspCheck, SipError* pErr)
#else
	(pBuffer,pTxnKey,pContextData,dTimer1,dTimer2,dDuration1,\
	dDuration2,dTranspCheck,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pTxnKey;
	SIP_Pvoid	pContextData;
	en_SipTimerType dTimer1;
	en_SipTimerType dTimer2;
	SIP_U32bit dDuration1;
	SIP_U32bit dDuration2;
	SipBool dTranspCheck;
	SipError* pErr;
#endif
{
	SIP_U32bit  tempDur;
	SipTxnTimeoutData *pTxnTimeoutData=SIP_NULL;
	SipTxnKey  *pClonedKey=SIP_NULL;
	SIPDEBUGFN("Entering  sip_txn_startTimersWithTranspCheck Function");
	
	(void)sip_initSipTxnKey(&pClonedKey,pErr);
	(void)sip_cloneSipTxnKey(pClonedKey,pTxnKey,pErr);
	/* Stop Timers If any of them are started */
	if(pBuffer->pTimerHandle!=SIP_NULL)
	{
		if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pTxnTimeoutData,\
					pErr) == SipFail)
			return SipFail;
		sip_freeSipTxnTimeoutData(pTxnTimeoutData);
		FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
		pBuffer->pTimerHandle = SIP_NULL;
	}

	if(sip_initSipTxnTimeoutData(&pTxnTimeoutData,pClonedKey,pErr) == SipFail)
		return SipFail;
	
	pTxnTimeoutData->pContextData=pContextData;
	
	/*
	 * Now.. we have two timeout durations. durn1,durn2. The timer shud be
	 * made to fire in such a way that it keeps firing for fixed durations
	 * starting at durn1. Th next durn will be twice this( but this again
	 * is controlled by the T2 value in some cases). We need to keep doing
	 * this for durn2 . But what if durn2 is smaller than durn1?  In that
	 * case we start the timer for durn2 itself. This however needs to be
	 * checked at this point 
	 */
	if (dDuration2<dDuration1)
	{
		dDuration1=dDuration2;
	}

	if((pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS)))&&(dTranspCheck))
	{
		/* start only one timer in the case of TCP */
		pTxnTimeoutData->dTimerStarted = dTimer2;
		tempDur =  dDuration2;
		if( START_TIMER(tempDur, (TimeoutFuncPtr)timerTimeOut, \
			(SIP_Pvoid)pTxnTimeoutData, (SIP_Pvoid*)&(pBuffer->pTimerHandle), \
			pErr) == SipFail )
        {
            sip_freeSipTxnTimeoutData(pTxnTimeoutData);
			return SipFail;
        }    
		pBuffer->dTimerStarted = dTimer2;
	}
	else
	{
		/*If UDP start two timers by setting max duration to dDuration2 */
		pTxnTimeoutData->dTimerStarted = dTimer1;
		/* Get the Duaration of Timer1  */
		pBuffer->dDuration = dDuration1;
		/* intialize the cumulative duration */
		pBuffer->dCumulativeDuration = pBuffer->dDuration;
		/* set the max duration,to Timer2 Duration */
		pBuffer->dMaxDuration = dDuration2;
		if( START_TIMER(pBuffer->dDuration, \
			(TimeoutFuncPtr)timerTimeOut, \
			(SIP_Pvoid)pTxnTimeoutData, (SIP_Pvoid*)&(pBuffer->pTimerHandle), \
			pErr) == SipFail )
        {
            sip_freeSipTxnTimeoutData(pTxnTimeoutData);
			return SipFail;
        }    
		pBuffer->dTimerStarted = dTimer1;
	}
	SIPDEBUGFN("Exiting from sip_txn_startTimersWithTranspCheck Function");
	return SipSuccess;
}
/***********************************************************************
**FUNCTION: sip_txn_startTimer
**
**DESCRIPTION:This Function Start the Specified timer Type With Specified duration
**
***********************************************************************/

SipBool sip_txn_startTimer
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer, SipTxnKey* pTxnKey, en_SipTimerType dTimer,\
	 SIP_U32bit dDuration,SipTxnInformn *pTxnInfo,SIP_Pvoid pContextData,\
	 SipError* pErr)
#else
	(pBuffer,pTxnKey,dTimer,dDuration,pTxnInfo,pContextData,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pTxnKey;
	en_SipTimerType dTimer;
	SIP_U32bit dDuration;
	SipTxnInformn *pTxnInfo;
	SIP_Pvoid pContextData;
	SipError* pErr;
#endif
{
	SipTxnTimeoutData *pTxnTimeoutData=SIP_NULL;
	SipTxnKey  *pClonedKey=SIP_NULL;
	SipBool	dItsUA=SipFail;
	SipBool dTimerCToBeStarted=SipSuccess;
	SipBool dTimerCRestart=SipSuccess;

	SIPDEBUGFN("Entering sip_txn_startTimer Function");

	/*
	 * In case pTxnInfo is NULL it means this is a RPR specific 
	 * case that is free from the limitaions of the TimerC.
	 */
	if ((dTimer==SipTimerC)&&(pTxnInfo!=SIP_NULL))
	{	
		if (pBuffer->dType==SipUATypeTxn)
			dItsUA=SipSuccess;

		/*The Timer C shud not be restarted if the foll conditions
			hold
		1>It is a proxy.
		2>The txn was already in the Proceeeding state
		3>Th response received is 100 Trying
			*/

		if ((!dItsUA)&&(pTxnInfo->dRespCode==100)&&\
			(pBuffer->dTxnState==SipTxnInvClientProceedingState))
			dTimerCRestart=SipFail;


		/*In case of a UA the timerC should not be started*/
		if ((dItsUA)&&(dTimer==SipTimerC))
			dTimerCToBeStarted=SipFail;

		/* Stop Timers If any of them are started already for this Txn*/
		if((dTimerCRestart)&&(pBuffer->pTimerHandle!=SIP_NULL))
		{
			if( STOP_TIMER(pBuffer->pTimerHandle,\
				(SIP_Pvoid *) &pTxnTimeoutData, pErr) == SipFail)
			return SipFail;
			sip_freeSipTxnTimeoutData(pTxnTimeoutData);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}

		if ((dTimerCRestart)&&(dTimerCToBeStarted))
		{
			(void)sip_initSipTxnKey(&pClonedKey,pErr);
			(void)sip_cloneSipTxnKey(pClonedKey,pTxnKey,pErr);

			if(sip_initSipTxnTimeoutData(&pTxnTimeoutData,pClonedKey,pErr)\
					== SipFail)
			return SipFail;
	
			pTxnTimeoutData->dTimerStarted = dTimer;
			pTxnTimeoutData->pContextData = pContextData;
			
			if( START_TIMER(dDuration, (TimeoutFuncPtr)timerTimeOut, \
				(SIP_Pvoid)pTxnTimeoutData,\
				(SIP_Pvoid*)&(pBuffer->pTimerHandle), pErr) == SipFail )
			return SipFail;

			pBuffer->dTimerStarted = dTimer ;
		}
	}	
	else
	{
		/* Stop Timers If any of them are started */
		if(pBuffer->pTimerHandle!=SIP_NULL)
		{
			if( STOP_TIMER(pBuffer->pTimerHandle,\
				(SIP_Pvoid *) &pTxnTimeoutData, pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pTxnTimeoutData);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}

		(void)sip_initSipTxnKey(&pClonedKey,pErr);
		(void)sip_cloneSipTxnKey(pClonedKey,pTxnKey,pErr);

		if(sip_initSipTxnTimeoutData\
				(&pTxnTimeoutData,pClonedKey,pErr) == SipFail)
			return SipFail;

		pTxnTimeoutData->dTimerStarted = dTimer;
		pTxnTimeoutData->pContextData = pContextData;
		
		if( START_TIMER(dDuration, (TimeoutFuncPtr)timerTimeOut, \
			(SIP_Pvoid)pTxnTimeoutData, (SIP_Pvoid*)&(pBuffer->pTimerHandle), \
			pErr) == SipFail )
			return SipFail;

		pBuffer->dTimerStarted = dTimer ;
	}
	SIPDEBUGFN("Exiting from sip_txn_startTimer Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvClCallingState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvClCallingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipBool dTranspCheck = SipSuccess;

	SIPDEBUGFN("Entering sip_txn_InvClCallingState Function");
	
	(void)pContextData;
	(void)pTxnInfo;
	
	if(pBuffer->dTxnState == SipTxnInvClientCallingState)
		return SipFail;

	/* set the state to the Calling State */
	pBuffer->dTxnState = SipTxnInvClientCallingState;

	/*Now for some interesting code:
		Bis09 mandates that sending an INVITE from a proxy UAC should start
		3 timers. Timer A that controls retransmission, Timer B that handles
		for how long TimerA keeps firing and retransXin, also TimerC for
		some reason, so we check which timeOut value is smaller and start
		that timer alone. The choice being between TimerB and TimerC*/

	/*Our architecture allows for more than one timers by the cumulative
		technique, so we check which timer amongst C,B is smaller and start
		that one*/
	if (pTxnInfo!=SIP_NULL)
	{	
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	
	if (pBuffer->dType==SipProxyTypeTxn)
	{
		if (pBuffer->timeoutValues.dTimerB < pBuffer->timeoutValues.dTimerC)
		{
			if(sip_txn_startTimersWithTranspCheck(pBuffer,pKey,pContextData,\
				SipTimerA_B,SipTimerB, pBuffer->timeoutValues.dT1,\
				pBuffer->timeoutValues.dTimerB,dTranspCheck,pErr) == SipFail)
			return SipFail;
		}
		else
		{
			if(sip_txn_startTimersWithTranspCheck(pBuffer,pKey,pContextData,\
				SipTimerA_B,SipTimerC, pBuffer->timeoutValues.dT1,\
				pBuffer->timeoutValues.dTimerC,dTranspCheck,pErr) == SipFail)
			return SipFail;
		}
	}
	else
	{
		if(sip_txn_startTimersWithTranspCheck(pBuffer,pKey,pContextData,\
			SipTimerA_B, SipTimerB, pBuffer->timeoutValues.dT1,
			pBuffer->timeoutValues.dTimerB,dTranspCheck,pErr) == SipFail)
		return SipFail;
	}

	SIPDEBUGFN("Exiting from sip_txn_InvClCallingState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvClProceedingState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvClProceedingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIP_U32bit tempDur;

	SIPDEBUGFN("Entering sip_txn_InvClProceedingState Function");

	/* get the timerC duration  */
	tempDur = pBuffer->timeoutValues.dTimerC;
	if (pTxnInfo!=SIP_NULL)
	{	
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}

	/*This fn takes care of the sitn wherein Timer C is already running
	and it needs to be restarted(stopped,started with new timeOut)*/
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerC, \
		tempDur,pTxnInfo,pContextData,pErr) == SipFail)
		return SipFail;

	/* set the state  */
	pBuffer->dTxnState = SipTxnInvClientProceedingState;

	SIPDEBUGFN("Exiting from sip_txn_InvClProceedingState Function");
	return SipSuccess;
}


/***********************************************************************
**FUNCTION: sip_txn_InvClFinalResponseRecvdState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvClFinalResponseRecvState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData* pTxnTimeoutData=SIP_NULL;
	(void)pKey;
	(void)pContextData;
	(void)pTxnInfo;

	SIPDEBUGFN("Entering sip_txn_InvClFinalResponseRecvState Function");

	/* set the state  */
	if(pBuffer->dTxnState == SipTxnInvClientFinalResponseRecvState)
		return SipFail;
	pBuffer->dTxnState = SipTxnInvClientFinalResponseRecvState;
	
	if (pBuffer->pTimerHandle!=SIP_NULL)
	{	
		if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pTxnTimeoutData,\
					pErr) == SipFail)
			return SipFail;
		sip_freeSipTxnTimeoutData(pTxnTimeoutData);
		FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
		pBuffer->pTimerHandle=SIP_NULL;
	}	

	SIPDEBUGFN("Exiting from sip_txn_InvClFinalResponseRecvState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvClCompletedState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvClCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData *pWKey=SIP_NULL;
	SIPDEBUGFN("Entering sip_txn_InvClCompletedState Function");

	/* set the state */
	if(pBuffer->dTxnState == SipTxnInvClientCompletedState)
		return SipFail;
	if((pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS))))
	{
		if (pBuffer->pTimerHandle!=SIP_NULL)
		{	
			if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
					pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pWKey);	
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}
		
		/* In case of TCP  Remove the Transaction */
		pBuffer->dTxnState = SipTxnTerminatedState;
		return SipSuccess;
	}
	pBuffer->dTxnState = SipTxnInvClientCompletedState;
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	/* start Timer D */
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerD, \
			pBuffer->timeoutValues.dTimerD_T3,pTxnInfo,pContextData,pErr)\
			== SipFail)
		return SipFail;

	SIPDEBUGFN("Exiting from sip_txn_InvClCompletedState Function");
	return SipSuccess;
}


/***********************************************************************
**FUNCTION: sip_txn_NonInvClTryingState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_NonInvClTryingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{

	SipBool dTranspCheck = SipSuccess;
	SIPDEBUGFN("Entering sip_txn_NonInvClTryingState Function");
	
	(void)pContextData;
	(void)pTxnInfo;

	if(pBuffer->dTxnState == SipTxnNonInvClientTryingState)
		return SipFail;
	/* set the state to the Calling State */
	if(pBuffer->dTxnState == SipTxnNonInvClientTryingState)
		return SipFail;
	pBuffer->dTxnState = SipTxnNonInvClientTryingState;
	
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}

	if(sip_txn_startTimersWithTranspCheck(pBuffer,pKey,pContextData,\
			SipTimerE_F,\
			SipTimerF, pBuffer->timeoutValues.dT1, \
			pBuffer->timeoutValues.dTimerF_T3,dTranspCheck, pErr) == SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_NonInvClTryingState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_NonInvClProceedingState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_NonInvClProceedingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvClProceedingState Function");
#if 0

	(void)pTxnInfo;
	(void)pContextData;
    (void)pKey; 
#endif

    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvClientProceedingState)
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvClientProceedingState;

	/*For removing compiln warning*/
	/*pKey=SIP_NULL;*/

	/* set the peak off, From now on if timerE fires
	it has to retransmit every T2 duration ,so set the transaction
	duration as T2 */
	pBuffer->dDuration = pBuffer->timeoutValues.dT2;

	/* CSR 1-7754726 Fix Start */ 
	if(!(pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS))))
	{
			if (pTxnInfo!=SIP_NULL)
			{
					if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
					{
							/*
							 * In case the mesage had been decoded instead of being sent 
							 * the context data needs to be pulled from the EventContext 
							 * in the TxnBuffer instead
							 */
							if (pBuffer->pEventContext!=SIP_NULL)
							{
									pContextData=pBuffer->pEventContext->pData;
							}
							else
									pContextData=SIP_NULL;
					}
			}
			sip_txn_startTimer(pBuffer,pKey,SipTimerE_F,pBuffer->timeoutValues.dT2,pTxnInfo,pContextData,pErr);
	}           
	/* CSR 1-7754726 Fix End */

	SIPDEBUGFN("Exiting from sip_txn_NonInvClProceedingState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_NonInvClCompletedState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_NonInvClCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData *pWKey=SIP_NULL;
	SIPDEBUGFN("Entering sip_txn_NonInvClCompletedState Function");

	if(pBuffer->dTxnState == SipTxnNonInvClientCompletedState)
		return SipFail;
	pBuffer->dTxnState = SipTxnNonInvClientCompletedState;
	if((pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS))))
	{
		if (pBuffer->pTimerHandle!=SIP_NULL)
		{	
			if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
				 pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pWKey);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}	
		/* In case of TCP  Remove the Transaction */
		pBuffer->dTxnState = SipTxnTerminatedState;
		return SipSuccess;
	}

	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	
	/* start timer K in the case of UDP */
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerK,\
		pBuffer->timeoutValues.dTimerK_T4,pTxnInfo,\
		pContextData,pErr) == SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_NonInvClCompletedState Function");
	return SipSuccess;
}


/***********************************************************************
**FUNCTION: sip_txn_InvSrvProceedingState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvSrvProceedingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_InvSrvProceedingState Function");

	(void)pTxnInfo;
	(void)pContextData;
    (void)pKey;	
    /* set the state  */
	if(pBuffer->dTxnState == SipTxnInvServerProceedingState)
		return SipFail;
    pBuffer->dTxnState = SipTxnInvServerProceedingState;
	/* start Default Timer */
	SIPDEBUGFN("Exiting from sip_txn_InvSrvProceedingState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvSrv1xxSentState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvSrv1xxSentState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_InvSrv1xxSentState Function");

	(void)pTxnInfo;
	(void)pContextData;
	(void)pKey;	
    /* set the state  */
	if(pBuffer->dTxnState == SipTxnInvServer1xxSentState )
		return SipFail;
    pBuffer->dTxnState = SipTxnInvServer1xxSentState;
	SIPDEBUGFN("Exiting from sip_txn_InvSrv1xxSentState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvSrv2xxForwardedState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvSrv2xxForwardedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIP_U32bit tempDur;
	SIPDEBUGFN("Entering sip_txn_InvSrv2xxForwardedState Function");

    /* set the state  */
	if(pBuffer->dTxnState == SipTxnInvServer2xxForwardedState)
		return SipFail;
    pBuffer->dTxnState = SipTxnInvServer2xxForwardedState;
	/* start timer SipTimerForProxyUAS2xxFwded for eating up
		retransmissions of the INVITEs, the duration however is the same
		as that of Timer B itself*/
	tempDur = pBuffer->timeoutValues.dTimerB;

	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerForProxyUAS2xxFwded, \
		tempDur,pTxnInfo,pContextData,pErr) == SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_InvSrv2xxForwardedState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvSrvCompletedState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvSrvCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipBool dTranspCheck = SipSuccess;
	SipTxnTimeoutData *pWKey=SIP_NULL;
	(void) pTxnInfo;

	SIPDEBUGFN("Entering sip_txn_InvSrvCompletedState Function");

	/*This function can be reached when the application decides that
		it has been sending a response to a wrong transport address
		and decides to change the same, it then retries to a diff transport
		address in which case this fn is hit when the state was already
		InvSrvCompletedState*/
		
	/*=====================================*/	
	/*Responses	for 404,407 shud NOT be retransmitted, this check has been
		relaxed for SDF*/
	/*=====================================*/	
		
	if (pBuffer->pTimerHandle!=SIP_NULL)
	{	
		if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
					pErr) == SipFail)
			return SipFail;
		sip_freeSipTxnTimeoutData(pWKey);
		FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
		pBuffer->pTimerHandle=SIP_NULL;
	}	

    pBuffer->dTxnState = SipTxnInvServerCompletedState;
	
	/* IN the case of 2xx ,Retransmissions Should
	   be happened even in the case of
	   Reliable Transport */
	if( pBuffer->dRespCode / 100 == 2) dTranspCheck = SipFail;
	
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}

	if(sip_txn_startTimersWithTranspCheck\
		(pBuffer,pKey,pContextData,SipTimerG_H,SipTimerH, \
		pBuffer->timeoutValues.dT1, pBuffer->timeoutValues.dTimerH,\
		dTranspCheck, pErr) == SipFail)
	return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_InvSrvCompletedState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_InvSrvConfirmedState
**
**DESCRIPTION:
**
***********************************************************************/

SipBool sip_txn_InvSrvConfirmedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData *pWKey=SIP_NULL;
	SIPDEBUGFN("Entering sip_txn_InvSrvConfirmedState Function");

    /* set the state  */
	if(pBuffer->dTxnState == SipTxnInvServerConfirmedState )
		return SipFail;
    pBuffer->dTxnState = SipTxnInvServerConfirmedState;
	if((pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS))))
	{
		if (pBuffer->pTimerHandle!=SIP_NULL)
		{	
			/* stop timerG_H */
			if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
						pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pWKey);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}	
		/* In case of TCP  Remove the Transaction */
		pBuffer->dTxnState = SipTxnTerminatedState;
		return SipSuccess;
	}

	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	
	/* start timer In case of UDP */
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerI, \
			pBuffer->timeoutValues.dTimerI_T4,pTxnInfo,pContextData,pErr)\
			== SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_InvSrvConfirmedState Function");
	return SipSuccess;

}

/***********************************************************************
**FUNCTION: sip_txn_SrvBypassState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_SrvBypassState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_SrvBypassState Function");

    pBuffer->dTxnState = SipTxnServerBypassState;
	
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
	
	/* start timer for waiting for a period of Invite RetransX period
		+ response retransX period*/
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerForCancelledState, \
			pBuffer->timeoutValues.dTimerB+pBuffer->timeoutValues.dTimerH,\
			pTxnInfo,pContextData,pErr) == SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_InvSrvConfirmedState Function");
	return SipSuccess;

}

/***********************************************************************
**FUNCTION: sip_txn_NonInvSrvTryingState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvSrvTryingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvSrvTryingState Function");

	(void)pTxnInfo;
	(void)pContextData;
    (void)pKey;	

    /* set the state  */
	if(pBuffer->dTxnState ==SipTxnNonInvServerTryingState )
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvServerTryingState;
	SIPDEBUGFN("Exiting from sip_txn_NonInvSrvTryingState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_NonInvSrvProceedingState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvSrvProceedingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvSrvProceedingState Function");

	(void)pTxnInfo;
	(void)pContextData;
    (void)pKey;	

    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvServerProceedingState )
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvServerProceedingState;
	SIPDEBUGFN("Exiting from sip_txn_NonInvSrvProceedingState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



/***********************************************************************
**FUNCTION: sip_txn_NonInvSrvCompletedState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvSrvCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData* pWKey=SIP_NULL;
	SIPDEBUGFN("Entering sip_txn_NonInvSrvCompletedState Function");
	(void)pTxnInfo;

	/*In case we come into this call for a UA that is trying to send a response
		a second time after having had tried a different network address 
		earlier, we need to stop the earlier timer, but this is valid only for 
		UDP cases*/
	if((pBuffer->dTranspType & SIP_UDP))
	{	
		if (pBuffer->pTimerHandle!=SIP_NULL)
		{	
			if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
						pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pWKey);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;
		}	
	}	
		
    pBuffer->dTxnState = SipTxnNonInvServerCompletedState;
	
	if((pBuffer->dTranspType & ((SIP_TCP)|(SIP_TLS))))
	{
		if (pBuffer->pTimerHandle!=SIP_NULL)
		{	
			if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
						pErr) == SipFail)
				return SipFail;
			sip_freeSipTxnTimeoutData(pWKey);
			FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
			pBuffer->pTimerHandle=SIP_NULL;	
		}	
		/* In case of TCP  Remove the Transaction */
		pBuffer->dTxnState = SipTxnTerminatedState;
		return SipSuccess;
	}
	
	if (pTxnInfo!=SIP_NULL)
	{
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}
/* Changes for CSR1-5349648 */
#ifdef SIP_RPR	
   if( strcmp(pKey->pMethod, "PRACK")==0)
    {	

		if(pBuffer->pTxnKey->pRackHdr)
		{
			if(sip_initSipRackHeader(&((pKey)->pRackHdr),\
                                 pErr)==SipFail)
                		return SipFail;

			/* clone RAck Header */
			if(sip_txn_rprcloneSipRAckHeader(pKey->pRackHdr, \
			pBuffer->pTxnKey->pRackHdr, pErr) == SipFail)
			{
			  /* free Rack */
			  if(pKey->pRackHdr)
			   sip_freeSipRackHeader(pKey->pRackHdr);

			   return SipFail;
			}
		}

	if(sip_txn_startTimer(pBuffer,pKey,SipTimerJ, \
			pBuffer->timeoutValues.dTimerJ_T3,pTxnInfo,pContextData,pErr)\
			== SipFail)
		return SipFail;
    }
else
{
#endif
/* Changes for CSR1-5349648 */

	/* Start Timer J for Unreliable Transmission only */
	if(sip_txn_startTimer(pBuffer,pKey,SipTimerJ, \
			pBuffer->timeoutValues.dTimerJ_T3,pTxnInfo,pContextData,pErr)\
			== SipFail)
		return SipFail;
/* Changes for CSR1-5349648 */
#ifdef SIP_RPR
}
#endif
/* Changes for CSR1-5349648 */
	
	SIPDEBUGFN("Exiting from sip_txn_NonInvSrvCompletedState Function");
	return SipSuccess;
}



/***********************************************************************
**FUNCTION: sip_txn_NonInvClientRPRRecvngState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvClientRPRRecvngState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvClientRPRRecvngState Function");
	
	/*For ensuring compiln*/
	(void)pTxnInfo;
	(void)pContextData;
    (void)pKey;	

    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvClientRPRRecvngState)
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvClientRPRRecvngState;

	SIPDEBUGFN("Exiting from sip_txn_NonInvClientRPRRecvngState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_NonInvClientRPRCompletedState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvClientRPRCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvClientRPRCompletedState Function");
	(void)pKey;
    (void)pContextData;
    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvClientRPRCompletedState)
		return SipFail;
    pBuffer->dTxnState =SipTxnNonInvClientRPRCompletedState ;
	if (pTxnInfo!=SIP_NULL)
	{	
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
#ifndef SIP_LINT
                pContextData=pBuffer->pEventContext->pData;
#endif
			}
			else
            {
#ifndef SIP_LINT
                pContextData=SIP_NULL;
#endif       
            }
		}
	}
	
	/*
	 * We were as per design supposed to start a timer here that decided
	 * how long a transaction stays to eat up retransX of RPRs. But 
	 * starting a timer caused a few other problems like what contextdata
	 * do we pass to this timer. Since this timer has its birthpoint as a 
	 * txndecodeMessage the contextData given in that cannot be used for
	 * the timer, since the contextdata is temporary. We instead maintain 
	 * the txn seemingly infinitely. Removing it finally when the INVITE
	 * txn itself gets removed
	 */
	
	/*****COMMENTED CODE ***********************************/
 	/********if(sip_txn_startTimer(pBuffer,pKey,SipTimerI, \
			pBuffer->timeoutValues.dTimerI_T4,pTxnInfo,pContextData,pErr)\
			== SipFail)
		return SipFail;*/
	/*****COMMENTED CODE ENDS ***********************************/

	SIPDEBUGFN("Exiting from sip_txn_NonInvClientRPRCompletedState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/***********************************************************************
**FUNCTION: sip_txn_NonInvServerRPRCompletedState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvServerRPRCompletedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_txn_NonInvServerRPRCompletedState Function");
	(void)pTxnInfo;
	
    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvServerRPRCompletedState)
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvServerRPRCompletedState;
	if (pTxnInfo!=SIP_NULL)
	{	
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}

	if(sip_txn_startTimer(pBuffer,pKey,SipTimerI,\
		pBuffer->timeoutValues.dTimerI_T4,pTxnInfo,pContextData,\
		pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN("Exiting from sip_txn_NonInvServerRPRCompletedState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***********************************************************************
**FUNCTION: sip_txn_NonInvServerRPRSendingState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_NonInvServerRPRSendingState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipBool dTranspCheck = SipFail;
	SIPDEBUGFN("Entering sip_txn_NonInvServerRPRSendingState Function");
	(void)pTxnInfo;


    /* set the state  */
	if(pBuffer->dTxnState == SipTxnNonInvServerRPRSendingState)
		return SipFail;
    pBuffer->dTxnState = SipTxnNonInvServerRPRSendingState;
	if (pTxnInfo!=SIP_NULL)
	{	
		if (pTxnInfo->dAPICalled!=SipTxnMesgSent)
		{
			/*
			 * In case the mesage had been decoded instead of being sent the
			 * context data needs to be pulled from the EventContext in the
			 * TxnBuffer instead
			 */
			if (pBuffer->pEventContext!=SIP_NULL)
			{
				pContextData=pBuffer->pEventContext->pData;
			}
			else
				pContextData=SIP_NULL;
		}
	}

	if(sip_txn_startTimersWithTranspCheck(pBuffer,pKey,pContextData,\
			SipTimerA_B,SipTimerB, pBuffer->timeoutValues.dT1, \
			pBuffer->timeoutValues.dTimerB,\
			dTranspCheck, pErr) == SipFail)
		return SipFail;
	SIPDEBUGFN("Exiting from sip_txn_NonInvServerRPRSendingState Function");
	return SipSuccess;
}

/***********************************************************************
**FUNCTION: sip_txn_TerminatedState
**
**DESCRIPTION:
**
***********************************************************************/
SipBool sip_txn_TerminatedState
#ifdef ANSI_PROTO
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo, SipError *pErr)
#else
	(pBuffer,pKey,pContextData,pTxnInfo,pErr)
	SipTxnBuffer* pBuffer;
	SipTxnKey* pKey;
	SIP_Pvoid pContextData;
	SipTxnInformn *pTxnInfo;
	SipError *pErr;
#endif
{
	SipTxnTimeoutData *pWKey=SIP_NULL;
	SIPDEBUGFN("Entering sip_txn_TerminatedState Function");
  (void)pKey; 
	(void)pTxnInfo;
	(void)pContextData;
	
	if(pBuffer->dTxnState == SipTxnTerminatedState)
		return SipFail;
    pBuffer->dTxnState = SipTxnTerminatedState;
	if(pBuffer->pTimerHandle!=SIP_NULL)
	{
		/* Stop Any Timers If Started */
		if( STOP_TIMER(pBuffer->pTimerHandle, (SIP_Pvoid *) &pWKey,\
					pErr) == SipFail)
			return SipFail;
		sip_freeSipTxnTimeoutData(pWKey);
		FREE_TIMER_HANDLE(pBuffer->pTimerHandle);	
		pBuffer->pTimerHandle=SIP_NULL;	
	}
	SIPDEBUGFN("Exiting from sip_txn_TerminatedState Function");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#ifdef __cplusplus
}
#endif
