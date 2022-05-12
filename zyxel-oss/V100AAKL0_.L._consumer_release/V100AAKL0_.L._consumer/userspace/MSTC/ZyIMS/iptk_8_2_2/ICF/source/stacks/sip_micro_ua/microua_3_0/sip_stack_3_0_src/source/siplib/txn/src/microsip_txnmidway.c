/************************************************************************
 ** FUNCTION:
 **            Parses SIP Messages
 *********************************************************************
 **
 ** FILENAME:
 ** txnmidway.c
 **
 ** DESCRIPTION:
 **             
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 **
 ** 9/2/2002	P.R.Kamath									Initial Version
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


#include "microsip_txnmidway.h"
#include "microsip_txndecode.h"
#include "microsip_txndecodeintrnl.h"
#include "microsip_timer.h"
#include "microsip_txntimer.h"
#include "microsip_txninit.h"
#include "microsip_txnfree.h"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#define RELEASE_TXN(a,b,c,d,e,f) sip_cbk_releaseTxn(a,b,c,d,e,f)

/***************************************************************************
**** FUNCTION:sip_txn_createMidwayTxn
**** Description:The application will use this API to create a Txn by giving
****             the SipMessage.This API does not  perform the check of whether
****            dependent Txn is present or not.This will immediately Create
****             New Txn If there is Nothing.
******************************************************************************/
SipBool sip_txn_createMidwayTxn
#ifdef ANSI_PROTO
	(SipMessage *pMessage,
	SipTranspAddr *pAddr,
	SIP_S8bit dTranspType,
	SipTxnContext *pTxnContext,
	en_SipTxnAPICalled dAPICalled,
	SipTxnKey **ppTxnKey,
	SipError *pError)
#else
	(pMessage,pAddr,dTranspType,pTxnContext,dAPICalled,ppTxnKey,pError)
	SipMessage *pMessage;
	SipTranspAddr *pAddr;
	SIP_S8bit dTranspType;
	SipTxnContext *pTxnContext;
	en_SipTxnAPICalled dAPICalled;
	SipTxnKey **ppTxnKey;
	SipError *pError;
#endif
{
	en_SipTxnClass dTxnClass;
	SIP_S8bit *pBuffer=SIP_NULL;
	SipBool dResult=SipFail;
	SipOptions *pSipOptions=SIP_NULL;
	SIP_U32bit dLength=0;
	SipTxnBuffer *pTxnBuffer=SIP_NULL;
	SipTxnInformn txnInformn;
	SipTxnInformn *pTxnInfo=SIP_NULL;
	SipMessage *pMesg=SIP_NULL;
	SipTxnKey	*pTxnKey=SIP_NULL;
	SIP_Pvoid	pContextData=SIP_NULL;

	SIPDEBUGFN("Entering function sip_txn_createMidwayTxn");

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
    
	txnInformn.pKeyOfCreatedTxn=SIP_NULL;
	txnInformn.pMsgBuffer=SIP_NULL;
	txnInformn.pTxnContext=SIP_NULL;
	txnInformn.pAddr=SIP_NULL;
	txnInformn.pSipMessage=SIP_NULL;
	txnInformn.dDirectOption=SipFail;
	pTxnInfo = &txnInformn;

	if (pTxnContext==SIP_NULL)
	{
		*pError=E_INV_PARAM;
		return SipFail;
	}


	/*Obtain the Options that will be sent to sendMessage*/
	pSipOptions=&(pTxnContext->txnOption.dOption);

	/*if( dAPICalled == SipTxnMesgSent )*/
	{

		/*The message has been sent now start the txn level handling on this*/
		dResult=sip_captureTxnInformn(pMessage,pBuffer,dLength,pTxnContext, \
				pAddr,dTranspType,pTxnInfo);
		if (dResult!=SipSuccess)
		{
			/*Handle failure  here*/
			return SipFail;
		}
	}
	txnInformn.dAPICalled=dAPICalled;

	/*Obtain the class of the txn whether inv-client,non-inv-client...*/
	dResult=sip_getTxnClass(pTxnInfo,dAPICalled,&dTxnClass);
	if (dResult!=SipSuccess)
	{
		/*Handle failure here*/
		return SipFail;
	}
	pTxnInfo->dTxnClass = dTxnClass;

	if( dAPICalled == SipTxnMesgSent )
	{

		/*Form the message buffer*/

		/*Call the form_message to obtain the actual buffer corresponding
		  to the message*/
		pBuffer=(SIP_S8bit *) fast_memget(TXN_MEM_ID,SIP_MAX_MSG_SIZE,pError);
		if (pBuffer==SIP_NULL)
		{
			/*Handle failure here*/
			return SipFail;
		}

		if (sip_formMessage(pMessage,pSipOptions,pBuffer,&dLength,pError) \
				==	SipFail)
		{
			(void)fast_memfree(TXN_MEM_ID,pBuffer,NULL);
			return SipFail;
		}
		/*The message has been sent now start the txn level handling on this*/
		dResult=sip_captureTxnInformn(pMessage,pBuffer,dLength,pTxnContext, \
				pAddr,dTranspType,pTxnInfo);
		if (dResult!=SipSuccess)
		{
			/*Handle failure  here*/
			return SipFail;
		}


		/*On the basis of the kind of txn that will get affected call the
		  respective update function*/

		if(( dTxnClass == SipInviteClient)&&\
				(pTxnInfo->dMethodType == SipTxnMethodInvite))
		{
			dResult=sip_updateInvClTxnForMesgSent(\
					pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Free the buffer in TxnInformn*/
				(void)fast_memfree(TXN_MEM_ID,pBuffer,SIP_NULL);
				return SipFail;
			}
		}
		else if( dTxnClass == SipNonInviteClient)
		{
			dResult=sip_updateNonInvClTxnForMesgSent(\
					pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Free the buffer in TxnInformn*/
				(void)fast_memfree(TXN_MEM_ID,pBuffer,SIP_NULL);
				return SipFail;
			}
		}
		else if (sip_isTxnAnRPR(&txnInformn)==SipSuccess)
		{
			dResult=sip_updateRPRTxnForMesgSent(pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Free the buffer in TxnInformn*/
				(void)fast_memfree(TXN_MEM_ID,pBuffer,SIP_NULL);
				return SipFail;
			}
		}
		else
		{	
			en_SipTxnClass	dTempTxnClass;

			/*First get a reference to the message that shud get queued up*/
			pMesg=pTxnInfo->pSipMessage;
			/* Create Txn Key */

			/*Get the TxnClass from the TxnInfo*/
			dTempTxnClass=pTxnInfo->dTxnClass;

			dResult=sip_txn_createTxnKey(pMesg,dTempTxnClass,&pTxnKey,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
			dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,pError);
			pTxnBuffer->dTxnClass = dTempTxnClass;
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
			/*Extract the SIP_Pvoid in the eventcontext of the passed
			  TxnInformn*/
			sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

			if (pTxnBuffer->dTxnState!=SipTxnDefaultState)
			{
				/* Already Transaction is there */
				(void)RELEASE_TXN(pTxnKey,SIP_NULL, SIP_NULL,pContextData,0,pError);
				sip_freeSipTxnKey(pTxnKey);
				return SipFail;
			}

			/*Copy the buffer that has been sent into the TxnBuffer
			  for handling future retransX */
			dResult=sip_reassignbufferInTxnBuffer(pTxnBuffer, \
					pTxnInfo,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
			/*Populate the TxnBuffer with the values, since
			  this buffer is newly created*/
			dResult=sip_populateTxnBuffer(pTxnBuffer,pTxnInfo, \
					pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}

			if (sip_isTxnAnRPR(&txnInformn)==SipSuccess)
			{
				dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
						pTxnKey, SipTxnNonInvServerRPRSendingState,\
						pTxnInfo,pContextData,pError);
			}
			else
			{	
				switch(dTxnClass)
				{
					case SipInviteClient:
						{
							if(pTxnInfo->dMethodType == SipTxnMethodAck)
							{
								dResult=sip_changeTxnStateAndRelease(\
										pTxnBuffer,pTxnKey, \
										SipTxnInvClientCompletedState,\
										pTxnInfo, pContextData,pError);

							}
							break;
						}	
					case SipInviteServer:
						{
							if(pTxnInfo->dRespClass ==SipResponse1xx)
							{
								dResult=sip_changeTxnStateAndRelease(\
										pTxnBuffer, pTxnKey, \
										SipTxnInvServer1xxSentState,\
										pTxnInfo,pContextData,pError);
							}
							else
							{
								dResult=sip_changeTxnStateAndRelease(\
										pTxnBuffer,  pTxnKey, \
										SipTxnInvServerCompletedState,\
										pTxnInfo,pContextData,pError);
							}
							break;
						}	

					case SipNonInviteServer:
						{
							if(pTxnInfo->dRespClass ==SipResponse1xx)
							{
								dResult=sip_changeTxnStateAndRelease(\
										pTxnBuffer,  pTxnKey, \
										SipTxnNonInvServerProceedingState,\
										pTxnInfo,pContextData,pError);
							}
							else
							{
								dResult=sip_changeTxnStateAndRelease(\
										pTxnBuffer,  pTxnKey, \
										SipTxnNonInvServerCompletedState,\
										pTxnInfo,pContextData,pError);
							}
							break;
						}	

					default:
						{
							/*Handle error condition*/
							return SipFail;
						}
				}	/*Switch block ends*/
			}	/*end of if-then-else for isTxnAnRPR*/

			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
		}
	}
	else
	{
		/*
		 * This section handles the functionality in case a message had
		 * been received instead of having had been sent
		 */

		if (sip_isTxnAnRPR(&txnInformn)==SipSuccess)
		{
			en_SipTxnDecodeResult dDecodeResult;

			dResult=sip_updateRPRTxnAfterMesgDecoding(pTxnInfo,&dDecodeResult,\
					pError);
			if (dResult!=SipSuccess)
			{
				/*Free the buffer in TxnInformn*/
				(void)fast_memfree(TXN_MEM_ID,pBuffer,SIP_NULL);
				return SipFail;
			}
		}
		else
		{	
			en_SipTxnClass	dTempTxnClass;

			pMesg=pTxnInfo->pSipMessage;

			/*Obtain the txn class*/
			dTempTxnClass=pTxnInfo->dTxnClass;

			/* Create Txn Key */
			dResult=sip_txn_createTxnKey(pMesg,dTempTxnClass,&pTxnKey,pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}

			dResult=sip_createTxnUsingKey(&pTxnBuffer,&pTxnKey,pTxnInfo,pError);
			pTxnBuffer->dTxnClass = dTxnClass;
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}

			/*
			 * Extract the SIP_Pvoid in the eventcontext of the passed 
			 * TxnInformn
			 */ 
			sip_getContextDatafromTxnInfo(pTxnInfo,&pContextData);

			if (pTxnBuffer->dTxnState!=SipTxnDefaultState)
			{
				/* Already Transaction is there */
				(void)RELEASE_TXN(pTxnKey,SIP_NULL,\
						SIP_NULL,pContextData,0,pError);
				sip_freeSipTxnKey(pTxnKey);
				return SipFail;
			}

			/*
			 * Populate the TxnBuffer with the values, since
			 * this buffer is newly created
			 */
			dResult=sip_populateTxnBuffer(pTxnBuffer,pTxnInfo, \
					pError);
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}

			pTxnBuffer->pEventContext=pTxnContext->pEventContext;

			/*Now call handle_txn since you have the SipMessage*/
			/*
			 * The message has been sent now start the txn level
			 * handling on this
			 */
			switch(dTxnClass)
			{
				case SipInviteClient:
					{
						if(pTxnInfo->dRespClass ==SipResponse1xx)
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, SipTxnInvClientProceedingState,\
									pTxnInfo, pContextData,pError);
						}
						else
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, \
									SipTxnInvClientFinalResponseRecvState,\
									pTxnInfo,pContextData,pError);
						}
						break;
					}	

				case SipNonInviteClient:
					{
						if(pTxnInfo->dRespClass ==SipResponse1xx)
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, SipTxnNonInvClientProceedingState,\
									pTxnInfo,pContextData,pError);
						}
						else
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, SipTxnNonInvClientCompletedState,\
									pTxnInfo,pContextData,pError);
						}
						break;
					}	

				case SipInviteServer:
					{
						if(pTxnInfo->dMethodType == SipTxnMethodAck)
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, SipTxnInvServerConfirmedState,\
									pTxnInfo,pContextData,pError);
						}
						else
						{
							dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
									pTxnKey, SipTxnInvServerProceedingState,\
									pTxnInfo,pContextData,pError);
						}
						break;
					}	

				case SipNonInviteServer:
					{
						dResult=sip_changeTxnStateAndRelease(pTxnBuffer, \
								pTxnKey, SipTxnNonInvServerTryingState,\
								pTxnInfo,pContextData,pError);
						break;
					}	

				default:
					/*Handle error condition*/
					return SipFail;
			}/*Switch case block ends*/
			if (dResult!=SipSuccess)
			{
				/*Handle failure here*/
				return SipFail;
			}
		}
	}	
	SIPDEBUGFN("Exiting function sip_txn_createMidwayTxn");
	/*set the Key */
	*ppTxnKey = pTxnInfo->pKeyOfCreatedTxn;
	*pError=E_NO_ERROR;
	return SipSuccess;
}
#ifdef __cplusplus
}
#endif

