/************************************************************************
 ** FUNCTION:
 **             This file has all the SIP Txn Related init Functions

 ************************************************************************
 **
 ** FILENAME:
 ** txninit.c
 **
 ** DESCRIPTION:
 **
 ** DATE                NAME                    REFERENCE               REASON
 ** ----                ----                    --------                ------
 **	7-Feb-2002			Sasidhar PVK
 **     COPYRIGHT , Aricent, 2006
 *************************************************************************/

#include "microsip_txnstruct.h"
#include "microsip_txninit.h"
#include "microsip_free.h"
#include "microsip_init.h"

/******************************************************************
**** FUNCTION:sip_initSipTxnKey
****
****
**** DESCRIPTION:This function does an init on the i/p parameter o
****				SipTxnKey type.
******************************************************************/
SipBool sip_initSipTxnKey
#ifdef ANSI_PROTO
	(SipTxnKey **ppKey, SipError *pErr)
#else
	(ppKey,pErr)
	SipTxnKey **ppKey;
	SipError *pErr;
#endif
{
	/*Check the validity of the i/p params*/
	if(pErr==SIP_NULL)
	{
		return SipFail;
	}

	if (ppKey==SIP_NULL)
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	*ppKey= ( SipTxnKey * ) fast_memget(TXN_MEM_ID, sizeof(SipTxnKey), pErr);
	if(*ppKey==SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	INIT((*ppKey)->pMethod);
	INIT((*ppKey)->pToTag);
	INIT((*ppKey)->pFromTag);
	INIT((*ppKey)->pViaBranch);
	INIT((*ppKey)->pCallid);
#ifdef SIP_RPR
	INIT((*ppKey)->pRackHdr);
#endif
	(*ppKey)->dCSeq	= 0;
	(*ppKey)->dRseq	= 0;
	(*ppKey)->dRespCodeNum= 0;
/*Changes for CSR 1-5930636 starts*/
	/*(*ppKey)->dViaHdrCount= 0;*/
/*Changes for CSR 1-5930636 end */
	(*ppKey)->dType= SipMessageAny;
	(*ppKey)->dTagCheck = SipFail;
	(*ppKey)->dViaBranchCheck = SipSuccess;

	 HSS_INITREF((*ppKey)->dRefCount);

	return SipSuccess;
}

/*************************************************************************
**** FUNCTION:sip_initSipTxnBuffer
****
****
**** DESCRIPTION:This function does an init on the SipTxnBuffer
*************************************************************************/
SipBool sip_initSipTxnBuffer
#ifdef ANSI_PROTO
	(SipTxnBuffer **ppBuffer, SipError *pErr)
#else
	(ppBuffer,pErr)
	SipTxnBuffer **ppBuffer;
	SipError *pErr;
#endif
{
	/*Do a validity check on the i/p params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if (ppBuffer==SIP_NULL)
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	*ppBuffer= ( SipTxnBuffer * ) \
		fast_memget(TXN_MEM_ID, sizeof(SipTxnBuffer), pErr);
	if(*ppBuffer==SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	INIT((*ppBuffer)->pBuffer);
	INIT((*ppBuffer)->pEventContext);
	INIT((*ppBuffer)->pTxnKey);
	INIT((*ppBuffer)->pTimerHandle);
	INIT((*ppBuffer)->pslRPRTxnKeyQ);
	INIT((*ppBuffer)->pAddr);

	
	(*ppBuffer)->dRetransCount = 0;
	(*ppBuffer)->dDirectOption = SipFail;
	(*ppBuffer)->dTxnState = SipTxnDefaultState;
	(*ppBuffer)->dTimeoutCbkBitMask = SIP_ISSUE_CBKFOR_ALLTIMERS;
	(*ppBuffer)->pslMesgQ= ( SipList * ) \
			fast_memget(TXN_MEM_ID, sizeof(SipList), pErr);
	if((*ppBuffer)->pslMesgQ == SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
	(void)sip_listInit(((*ppBuffer)->pslMesgQ ),__sip_freeSipMessage,pErr);

	HSS_INITREF((*ppBuffer)->dRefCount);

	return SipSuccess;
}

/****************************************************************************
**** FUNCTION:sip_initSipTxnTimeoutData
****
****
**** DESCRIPTION:Thid does an init on the wrapper Txn Key
****************************************************************************/
SipBool sip_initSipTxnTimeoutData
#ifdef ANSI_PROTO
        (SipTxnTimeoutData **ppTxnTimeoutData,SipTxnKey *pTxnKey,SipError *pErr)
#else
        (ppTxnTimeoutData,pTxnKey, pErr)
	SipTxnTimeoutData **ppTxnTimeoutData;
	SipTxnKey *pTxnKey;
	SipError *pErr;
#endif
{
	/*Check for the validity of the params*/
	if (pErr==SIP_NULL)
	{
		return SipFail;
	}

	if ((pTxnKey==SIP_NULL)||(ppTxnTimeoutData==SIP_NULL))
	{
		*pErr=E_INV_PARAM;
		return SipFail;
	}

	*ppTxnTimeoutData= ( SipTxnTimeoutData * ) \
					fast_memget(TXN_MEM_ID, sizeof(SipTxnTimeoutData), pErr);
	if(*ppTxnTimeoutData==SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
	(*ppTxnTimeoutData)->pKey = pTxnKey;
	return SipSuccess;
}
