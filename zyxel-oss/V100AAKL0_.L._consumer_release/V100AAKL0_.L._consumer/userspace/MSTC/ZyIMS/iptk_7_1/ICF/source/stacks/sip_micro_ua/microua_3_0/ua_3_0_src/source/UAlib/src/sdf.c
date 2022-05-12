/*****************************************************************************
 ** FUNCTION: 		  This file has all the call related invocation APIs
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf.c
 **
 ** DESCRIPTION		: This file has all the call related invocation APIs
 **  	
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 27/10/00	Anitha						Creation
 ** 07/11/00	Prashant					Updated
 ** 29/08/01	Subhash						Code cleanup, comments,
 **											and correcting error codes
 ** 19/09/01	Subhash						Added support for SUBSCRIBE
 **											and NOTIFY
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 12/05/03     Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 ** 12/01/09    Vinay Jindal	SPR 19254   sdf_ivk_uaIPTKCheckMessageLength
 **                                         corrected
 ** 09/04/10    Alok Tiwari     CSR 1-8223807  UATK CSR 1-8290042 Merged
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2009
 *****************************************************************************/

#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_portlayer.h"
#include "sdf_list.h"
#endif


#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf.h"
#include "sdf_init.h"
#include "sdf_hash.h"
#include "sdf_accessor.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_network.h"
#include "sdf_callapis.h"

#ifdef SDF_AUTHENTICATE
#include "sdf_authorization.h"
#endif /* SDF_AUTHENTICATE */

#include "sdf_callbacks.h"
#include "sdf_sdp.h"
#include "sdf_debug.h"
#include "sdf_reqresp.h"
#include "sdf_statistics.h"
#include "sdf_trace.h"
#include "sdf_sdpaccessor.h"
#ifdef SDF_TXN
#include "sdf_txn.h"
#endif

#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
#include <sdf_subslayerstruct.h>
#include <sdf_subslayercbk.h>
#include <sdf_subslayerutils.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
#ifdef SDF_DEP
 Sdf_ty_retVal sdf_fn_uaDepPostStateChange (
    Sdf_st_callObject               *pCallObj,
    SipMessage                      *pMessage,
	Sdf_ty_msgMode                  dMode,
    Sdf_st_error                    *pErr);
#endif
 #ifdef __cplusplus
}
#endif

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaSetTo
 **
 ** DESCRIPTION: Sets the destination ("To") address for the call in the 
 **				 call object
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetTo 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit *pName,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u32bit dPort,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pError)
#else
	( pObject,pName, pAddr,dPort,pScheme, pError )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit *pName;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u32bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	Sdf_ty_s8bit	*pSipName = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaSetTo");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetTo(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo(): "
			"pObject param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo():  pAddr passed is Invalid",\
			pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Make a SipAddrSpec structure from the values passed to this API.
	 */
	if (sdf_ivk_uaMakeAddrSpecFromConstituents(&pAddrSpec, pAddr, \
		dPort, pScheme, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo():  Failed to make an addr spec"
			" from the parameters passed.", pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/* 
	 * Free the existing header if present 
	 */
	sip_freeSipHeader(pObject->pCommonInfo->pTo);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
		(Sdf_ty_pvoid *)&(pObject->pCommonInfo->pTo), pError);

	/* 
	 * Initialize a To header and set the Addrspec in it
	 */
	if (sip_initSipHeader(&pObject->pCommonInfo->pTo, SipHdrTypeTo, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo(): "
			"Failed to initialize SipHeader To",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	if (sip_setAddrSpecInToHdr(pObject->pCommonInfo->pTo, pAddrSpec, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
		sip_freeSipAddrSpec(pAddrSpec);
		sip_freeSipHeader(pObject->pCommonInfo->pTo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo(): "
			"Failed to set addrspec in To header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/* 
	 * Set the display name 
	 */
	if (pName != Sdf_co_null)
	{
		pSipName = Sdf_mc_strdupCallHandler(pName);
		
		if (sip_setDispNameInToHdr(pObject->pCommonInfo->pTo, \
			pSipName, (SipError*)&(pError->stkErrCode)) \
			== SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetTo(): "
				"Failed to set DisplayName in To header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}
	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaSetTo");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetTransportSchemeInTransaction
**
** DESCRIPTION: Sets the Transport Scheme to be used for this call. 
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetTransportSchemeInTransaction
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
     Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pErr)
#else
	( pTransaction,pOverlapTransInfo,pScheme, pErr )
	 Sdf_st_transaction *pTransaction;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_transportInfo *pDestInfo = Sdf_co_null;
    Sdf_ty_protocol dTempScheme = Sdf_en_protoNone;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetTransportSchemeInTransaction");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetTransportSchemeInTransaction:pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if((pOverlapTransInfo == Sdf_co_null) && (pTransaction == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTransportSchemeInTransaction(): "
			"pObject param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pScheme == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTransportSchemeInTransaction(): "
			"pScheme param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif
	
	if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,\
		pOverlapTransInfo,&pDestInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTransportSchemeInTransaction(): "
			"Error getting destination info structure from call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

    if(Sdf_mc_strcasecmp(pScheme,"TCP") == 0)
        dTempScheme=Sdf_en_protoTcp;
#ifdef SDF_TLS
    else if(Sdf_mc_strcasecmp(pScheme,"TLS") == 0)
        dTempScheme=Sdf_en_protoTls;
#endif
    else if(Sdf_mc_strcasecmp(pScheme,"UDP") == 0)
        dTempScheme=Sdf_en_protoUdp;

    if((pDestInfo->dScheme == Sdf_en_protoNone) || \
       (dTempScheme != pDestInfo->dScheme))
       pDestInfo->dScheme = dTempScheme;

	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetTransportSchemeInTransaction");
	
	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaDecodeMessage
 **
 ** DESCRIPTION: Decodes the received SIP message buffer and sets it in the
 **				 call object passed as a parameter. It also checks if it is 
 **				 a remote retransmission, and if so, resends the same response 
 **				 that was sent earlier.
 **
 *****************************************************************************/
Sdf_ty_matchMessageType sdf_ivk_uaDecodeMessage
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_s8bit *pBuffer,
	SipOptions *pOptions,
	Sdf_ty_u32bit dLength,
	Sdf_ty_s8bit **ppNextBuffer,
	Sdf_st_eventContext *pEventContext,
	Sdf_st_error *pErr)
#else
	(pCallObj,pBuffer, pOptions, dLength,ppNextBuffer,pEventContext,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_s8bit *pBuffer;
	SipOptions *pOptions;
	Sdf_ty_u32bit dLength;
	Sdf_ty_s8bit **ppNextBuffer;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error *pErr;
#endif
{
	SipMessage 				*pMsg	= Sdf_co_null;
	Sdf_ty_bool 			dSdpLineError	= Sdf_co_false;
	Sdf_ty_matchMessageType	returnVal 	= Sdf_en_success;
#ifdef SIP_BADMESSAGE_PARSING
	Sdf_ty_slist			slNonMandatoryBadHeaders;
#endif /* SIP_BADMESSAGE_PARSING */
    Sdf_st_error dErr1 = {Sdf_en_LastUatkError,Sdf_en_LastCoreStackErrors,0,""};

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG-Entering sdf_ivk_uaDecodeMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDecodeMessage(): pErr passed is Invalid");
#endif
		return Sdf_en_fail;
	}
	if(pBuffer == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"pBuffer param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}	
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"pCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}	
	if(pEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"pEventContext passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}
	if (pEventContext->pTranspAddr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"Invalid transpaddr passed.",pErr);
#endif			
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}
#endif
    /* Decode the Received Buffer and form the Sip Message */
	returnVal = sdf_fn_uaSipDecode(pCallObj, pBuffer, &pMsg, pOptions,  \
		dLength, ppNextBuffer, pEventContext, pErr);
        
  /* Enhancement Change Start */
    if( ((pErr->stkErrCode == Sdf_en_incomplete) || 
         (Sdf_en_sipIncomplete == pErr->stkErrCode))&&
         (pEventContext->dProtocol == Sdf_en_protoUdp)
         )
	{
         if(pMsg != Sdf_co_null)
         {    
              returnVal = Sdf_en_success;
                /*Enhancement Change Start*/
              if(Sdf_en_incomplete == pErr->stkErrCode)
                /*Enhancement Change End*/
              dErr1.stkErrCode = Sdf_en_incomplete;
                /*Enhancement Change Start*/
          else
              dErr1.stkErrCode = Sdf_en_sipIncomplete;
               /*Enhancement Change End*/    
          }
    }    
  /*Enhancement Change End*/
	if( returnVal != Sdf_en_success )
	{
		if( pErr->errCode != Sdf_en_noUaError)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_parserError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
				"Decoding the SIP message failed",pErr);
#endif
		}
		return returnVal;
	}

#ifdef SIP_BADMESSAGE_PARSING 
	/*Initialize the List*/
	if(sdf_listInit(&slNonMandatoryBadHeaders,__sip_freeSipBadHeader,\
			Sdf_co_false,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"Failed to Init Bad Headers List",pErr);
#endif
		return Sdf_en_fail;
	}

	/* If any error occurs in non-mandatory headers
	 * then ignore the error headers and proceed further without fail */
	if( (pOptions->dOption & SIP_OPT_BADMESSAGE) == SIP_OPT_BADMESSAGE )
	{
		returnVal = sdf_fn_uaHandleBadHeaders( pMsg,&slNonMandatoryBadHeaders, \
						&dSdpLineError, pErr);
		if( returnVal != Sdf_en_success )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
				"Failed due to Bad headers",pErr);
#endif
			(void)sdf_listDeleteAll(&slNonMandatoryBadHeaders,pErr);
			return returnVal;
		}
	}
#endif /* SIP_BADMESSAGE_PARSING */

	/*
     * The message got decoded successfully, now do all the UATK processing 
	 * with the decoded SipMessage.
	 */
	returnVal = sdf_ivk_uaProcessDecodedMessage(pCallObj, pMsg, pEventContext,\
		                           pErr);
	/*This fix is for spr-2755.(added in uatk2.2 but in uatk3.0 it is not
	 * present so to fix the spr-3929 raised by ssf again sip free is added ) */
#ifdef SDF_UASSF
    	sip_freeSipMessage(pMsg);
#endif
	if( returnVal != Sdf_en_success )
	{
#ifdef SDF_ERROR
		if (returnVal != Sdf_en_remoteRetransmission)	
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
				"Failed to process decoded message.",pErr);
#endif
#ifdef SIP_BADMESSAGE_PARSING 
		(void)sdf_listDeleteAll(&slNonMandatoryBadHeaders,pErr);
#endif /* SIP_BADMESSAGE_PARSING */
		return returnVal;
	}

#ifdef SIP_BADMESSAGE_PARSING 
	if(slNonMandatoryBadHeaders.size != 0)
	{
		if(sdf_fn_uaGetSpecificNonMandatoryBadHeaderError(\
				&slNonMandatoryBadHeaders,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"Error while manipulating non-Mandatory badheaders",pErr);
#endif
			(void)sdf_listDeleteAll(&slNonMandatoryBadHeaders,pErr);
			return Sdf_en_fail;
		}
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"Error in non-Mandatory header",pErr);
#endif
		(void)sdf_listDeleteAll(&slNonMandatoryBadHeaders,pErr);
		sdf_fn_debug( \
			(Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaDecodeMessage");
		return Sdf_en_fail;
	}	
	(void)sdf_listDeleteAll(&slNonMandatoryBadHeaders,pErr);
#endif /* SIP_BADMESSAGE_PARSING */

	if(dSdpLineError==Sdf_co_true)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpLineError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDecodeMessage(): "
			"Error in Sdp Line",pErr);
#endif
		pErr->errCode=Sdf_en_sdpLineError;
		sdf_fn_debug( \
			(Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaDecodeMessage");
		return Sdf_en_fail;
	}
	/*CSR 3776004	*/
        if(Sdf_en_incomplete == dErr1.stkErrCode)
	{
		pErr->stkErrCode = Sdf_en_incomplete;
		return Sdf_en_fail;
	}
	/*CSR 3776004*/
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaDecodeMessage");
	return Sdf_en_success;
}


/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaProcessDecodedMessage
 **
 ** DESCRIPTION: Processes the decoded SipMessage and fills in the relevant
 **              information in the CallObject, validates the message and
 **              handles remote retransmission if txn layer support is not
 **              present.
 **
 *****************************************************************************/
Sdf_ty_matchMessageType  sdf_ivk_uaProcessDecodedMessage
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	SipMessage *pMsg,
	Sdf_st_eventContext *pEventContext,
	Sdf_st_error *pErr)
#else
	(pCallObj,pMsg,pEventContext,pErr)
	Sdf_st_callObject *pCallObj;
	SipMessage *pMsg;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_msgInfo			dMessageInfo;
	en_SipMessageType 		dReqRespType;
	Sdf_st_transportInfo 	*pDestInfo	= Sdf_co_null;
	Sdf_ty_s8bit 			*pMethod	= Sdf_co_null;
#ifdef SDF_LINT
#if defined (SDF_TRACE) || !defined (SDF_TXN)
	Sdf_ty_u16bit 			dRespCode;
#endif	
#else
	Sdf_ty_u16bit 			dRespCode;
#endif /* SDF_TRACE */	
	Sdf_ty_messageType 		dMsgType	= Sdf_en_unknownRequest;
	Sdf_ty_matchMessageType	returnVal 	= Sdf_en_success;
	Sdf_st_transaction	*pTransaction = Sdf_co_null;
	Sdf_ty_transactionType transType;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
#endif

/* CSR_1-5250732 Change Start */	
	Sdf_ty_bool dIsContactMandatory = Sdf_co_false;
/* CSR_1-5250732 Change End */
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG -" \
						"Entering sdf_ivk_uaProcessDecodedMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaProcessDecodedMessage(): pErr passed is Invalid");
#endif
		return Sdf_en_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"pCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}	
	if(pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"SipMessage passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}
	if(pEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"pEventContext passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}
#endif  /* End of ParamValidation */


	if( Sdf_co_success != sdf_fn_uaGetMessageInfo( pMsg, &dMessageInfo, pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"Failed to get message details from  SIP Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}

	dMsgType     = dMessageInfo.dMsgType;		
	dReqRespType = dMessageInfo.dReqRespType;
#ifdef SDF_LINT
#if defined (SDF_TRACE) || !defined (SDF_TXN)
	dRespCode    = dMessageInfo.dResponse;
#endif
#else
	dRespCode    = dMessageInfo.dResponse;
#endif /* SDF_TRACE */	
	pMethod	     = dMessageInfo.pMethod;


#ifndef SDF_TXN
	/* 
	 * Check for the received message against the entries in the remote 
	 * retransmission list. If it is a remote retransmission, the response
	 * that was sent earlier is resent and Sdf_en_remoteRetransmission is
	 * returned from this API.
	 * This check can be skipped for ACK requests and 1xx responses.
	 */
	if ( ((dReqRespType == SipMessageRequest) && (dMsgType == Sdf_en_ack)) || \
		((dReqRespType == SipMessageResponse) && (dRespCode < 200)) )
		returnVal = Sdf_en_success;
	else
		returnVal = sdf_fn_uaCheckRetransmission(pMsg, pEventContext, pErr);
	
	if (returnVal == Sdf_en_remoteRetransmission)
	{
		pErr->errCode = Sdf_en_noUaError;
#ifdef SDF_TRACE
			sdf_fn_trace(Sdf_en_detailedTraces,Sdf_en_remoteRetransTraces,\
				(Sdf_ty_s8bit*)"Retransmitted Msg received while decoding",\0, pErr);
#endif
		sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG - Exiting sdf_ivk_uaProcessDecodedMessage");
		return returnVal;
	}
#endif

	returnVal = sdf_fn_uaExtractKeyInfo( pCallObj, pMsg, dReqRespType, pErr);

	if( returnVal != Sdf_en_success )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"Failed to Extract Key Information From Message.",pErr);
#endif
		return returnVal;
	}	

#ifdef SDF_TRACE	
	/*
	 * Print a trace describing message that was just received
	 */
	if(dReqRespType == SipMessageResponse)
	{
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
			"Call: %s - %d to %s received.\n", \
			pCallObj->pCommonInfo->pKey->pCallId, dRespCode, pMethod);

		sdf_fn_trace(Sdf_en_allTraceLevels, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace, 0, pErr);
	}
	else
	{
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
			"Call: %s - %s received.\n", \
			pCallObj->pCommonInfo->pKey->pCallId, pMethod);

		sdf_fn_trace(Sdf_en_allTraceLevels, \
			Sdf_en_mesgTraces, (Sdf_ty_s8bit *)trace, 0, pErr);
	}
#endif	

	if (dReqRespType == SipMessageRequest)
		transType = Sdf_en_uasTransaction;
	else
		transType = Sdf_en_uacTransaction;


	if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, transType, \
		&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSipDecode(): "
			"Failed to get UAS structure from call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_fail;
	}
	pTransaction->pSipMsg = pMsg;
	HSS_LOCKEDINCREF(pMsg->dRefCount);
	(void)sdf_ivk_uaFreeTransaction(pTransaction);


	/*
	 * Update the member of the dest info structure of call object to contain
	 * the protocol (UDP/TCP) on which the message was received.
	 */
	if(dReqRespType==SipMessageResponse)
		pDestInfo = pCallObj->pUacTransaction->pDestInfo;
	else
		pDestInfo = pCallObj->pUasTransaction->pDestInfo;
	
     pDestInfo->dScheme = pEventContext->dProtocol;

    if(pDestInfo->dScheme == Sdf_en_protoTcp
       /* Fix for spr 17941 AND  CSR_1-5512919*/
#ifdef SDF_TLS
        || (pDestInfo->dScheme == Sdf_en_protoTls)
#endif
      )
/* Fix for spr 17191 end */
    {
        SipHeader dContentLengthHdr;
        if(SipFail==sip_getHeaderAtIndex(pMsg,SipHdrTypeContentLength,\
                    &dContentLengthHdr,0,(SipError *)&(pErr->stkErrCode)))
        {
            pErr->errCode = Sdf_en_mandatoryHdrMissing;
            sip_freeSipMessage(pMsg);
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                    (Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
                    "Failed to get the content-length header",pErr);
#endif
            return Sdf_en_fail;

        }
        sip_freeSipHeader(&dContentLengthHdr);
    }
/* CSR_1-5929710 Change Start */
#ifdef SIP_IMPP
    if(Sdf_en_notify != dMsgType)
    {
#endif
/* CSR_1-5929710 Change End */
/* CSR_1-5250732 Change Start */
	dIsContactMandatory = sdf_fn_uaIsContactHdrMandatory(dReqRespType,dMsgType, dMessageInfo.dResponse);
	if(dIsContactMandatory == Sdf_co_true)
	{
		if(pMsg->pGeneralHdr->slContactHdr.size == 0)
		{	
			pErr->errCode = Sdf_en_mandatoryHdrMissing;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
				"Failed to get the Contact header",pErr);
#endif
			/* Fix for CSR 1-6792719 referred from IPTK Rel 7.1.2 */	
            		/*CERT Fix: Free SipMessage in case of failure
             		*as this is not available in IPTK scope,
	                *so this can not be freed later*/
#ifndef SDF_UASSF
            sip_freeSipMessage(pMsg);
#endif  
			return Sdf_en_fail;
		}
	}
/* CSR_1-5250732 Change End */
/* CSR_1-5929710 Change Start */
#ifdef SIP_IMPP
    }
#endif
    /* CSR_1-5929710 Change End */

	if(dReqRespType == SipMessageRequest)
	{
		returnVal = sdf_fn_uaCorrectCseq( pMethod, pMsg, pErr);
		
		if( returnVal != Sdf_en_success )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
				"Failed to Correct the Cseq header",pErr);
#endif
		}
	} /* End of "if message is a request" */


	/*
	 * Check if the incoming message has a non sip: and tel: URL in the 
	 * From or Contact header. In such a case, return an error code of
	 * Sdf_en_notImplemented. The application can now return an appropriate 
	 * error response code (501).
	 * 
	 * From URI is now checked only for requests.
	 */
	returnVal = sdf_fn_uaValidateMessage(pMsg, dReqRespType, dMsgType, pErr);
	if( returnVal != Sdf_en_success )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"Header validations in Message Failed",pErr);
#endif
	/*This fix is not present before (spr-2755) added in between uatk3.0
 	* To fix the spr-3929 raised by ssf the sip free fn is commented. We are
 	* retaining old code */
#ifndef SDF_UASSF
      sip_freeSipMessage(pMsg);
#endif
		return returnVal;
	}
#if 0/* FIX FOR SPR 3976 */
	/*
	 * On linux Port if Sdf_IPV6 is defined, then we should not Add received
	 * parameter in the response.
	 */ 
#ifdef SDF_RPORT
{
	Sdf_ty_u32bit Isrportpresent=0;	
	if(sdf_fn_uaPopulateViaRportParam(pMsg, pEventContext, &Isrportpresent, pErr) != Sdf_en_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"Via header validation failed",pErr);
#endif
		return Sdf_en_fail;



	}
	if(Isrportpresent==0)
	{
		
#endif


	returnVal = sdf_fn_uaAddViaRecdParam(pMsg, pEventContext, pErr);

#endif /*if 0 SPR 3976*/
if(dReqRespType == SipMessageRequest)
{
#ifdef SDF_RPORT
{
	Sdf_ty_u32bit Isrportpresent=0;	
	if(sdf_fn_uaPopulateViaRportParam(pMsg, pEventContext, &Isrportpresent, pErr) != Sdf_en_success)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			"Via header validation failed",pErr);
#endif
		return Sdf_en_fail;
	}
	if(Isrportpresent==0)
	{
#endif
	  returnVal = sdf_fn_uaAddViaRecdParam(pMsg, pEventContext, pErr);

	  if( returnVal != Sdf_en_success )
	  {
#ifdef SDF_ERROR
		  sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			  (Sdf_ty_s8bit *)"sdf_ivk_uaProcessDecodedMessage(): "
			  "Via header validation failed",pErr);
#endif
    /*This fix is not present before (spr-2755) added in between uatk3.0. 
	 * To fix the spr-3929 raised by ssf the sip free fn is commented. We are
	 * retaining old code */
	
#ifndef SDF_UASSF
    	  sip_freeSipMessage(pMsg); 
#endif
		  return Sdf_en_fail;
	  }
#ifdef SDF_RPORT
	}
}
#endif
}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			              "sdf_ivk_uaProcessDecodedMessage");
    /*This fix is not present before (spr-2755) added in between uatk3.0. 
	 * To fix the spr-3929 raised by ssf the sip free fn is commented. We are
	 * retaining old code */

#ifndef SDF_UASSF
    sip_freeSipMessage(pMsg); 
#endif
	return Sdf_en_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaHandleCall
**
** DESCRIPTION: This function should be invoked on receipt of a new 
**	message that does not map into an existing call. It updates the
**	call object with the details of the received message and issues
**	the appropriate callback to the application.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHandleCall
#ifdef ANSI_PROTO
(Sdf_st_callObject **ppCallObj,
 Sdf_st_eventContext *pEventContext,
 Sdf_st_error *pErr)
#else
 (ppCallObj,pEventContext,pErr)
 Sdf_st_callObject **ppCallObj;
 Sdf_st_eventContext *pEventContext;
 Sdf_st_error *pErr;
#endif
{	
	Sdf_ty_retVal 			dCbkRetVal = Sdf_co_success;
	Sdf_ty_messageType 		dMethodType = Sdf_en_unknownRequest;
#ifndef SDF_LINT	
    Sdf_ty_s8bit			*pMethod = Sdf_co_null;
   	Sdf_ty_u16bit 			dRespCode;
#endif    
	Sdf_ty_bool	 	 		dIsToTagPresent = Sdf_co_false;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	SipMessage				*pIncMessage = Sdf_co_null;
	SipMessage				**ppMessage;
	Sdf_ty_slist			*pslMessageBody;
    en_SipMessageType		dMsgType;
	Sdf_st_msgInfo			dMessageInfo;
	SipAddrSpec *pRequestAddrSpec=Sdf_co_null;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId 		dCallingThread = 0;
	Sdf_st_error			dLocalError;
#endif
#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif

	sdf_fn_debug( (Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaHandleCall");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaHandleCall() : pErr passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if (ppCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"ppCallObj param passed is Invlaid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

#ifdef SDF_THREAD_SAFE
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
	(void)sdf_ivk_uaLockCallObject(*ppCallObj, pErr);
#endif

	/* 
	 * Set the CallObjectType to Sdf_en_PermanentCallObject as call-id, key, 
	 * etc need not be generated for a received call, it will set from the 
	 * received INVITE message.
	 */
	(*ppCallObj)->dCallObjectType = Sdf_en_PermanentCallObject;

	pIncMessage = (*ppCallObj)->pUasTransaction->pSipMsg;
	
	if(pIncMessage == Sdf_co_null)
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSprintf(Trace,\
			"sdf_ivk_uaHandleCall received response. Dropped the message");
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_uaAllTraces,Trace,0,pErr);
#endif


#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
		return Sdf_co_success;
	}

	if( Sdf_co_success != sdf_fn_uaGetMessageInfo(pIncMessage,  \
		 &dMessageInfo, pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Failed to get message Information From Message",pErr);
#endif
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
		return Sdf_co_fail;
	}
	dMsgType    = dMessageInfo.dReqRespType;
	dMethodType = dMessageInfo.dMsgType;
#ifndef SDF_LINT
    dRespCode   = dMessageInfo.dResponse;
    pMethod	    = dMessageInfo.pMethod;
#endif    

	dTransactionType = sdf_fn_getTransactionType( pIncMessage, dMessageInfo, \
						Sdf_co_true,pErr);

	if(dMsgType == SipMessageRequest)
	{
        SipReqLine    *pRequest = Sdf_co_null;

		if(sip_getReqLineFromSipReqMsg(pIncMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,(Sdf_ty_s8bit *)\
			   	"sdf_ivk_uaHandleCall(): Failed to get Request Line" \
			   	"from incoming Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		/* 
		 * pEventContext->dProtocol will be set by the application before 
		 * invoking sdf_ivk_uaDecodeMessage.
		 */
		if ( sip_getAddrSpecFromReqLine(pRequest,&pRequestAddrSpec, \
					(SipError *)&(pErr->stkErrCode)) == SipFail ) 
        { 
#ifdef SDF_ERROR 
            sdf_fn_setError(Sdf_en_majorErrors, \
                    Sdf_en_headerManipulationError,\
                    (Sdf_ty_s8bit *) "sdf_fn_uaGetContactAddrType( ):"\
                    "Failed to getAddrSpec from  Request Line",pErr); 
#endif 
            pErr->errCode=Sdf_en_headerManipulationError; 
            /* Free already accessed ReqLine */ 
            sip_freeSipReqLine(pRequest) ; 
            return Sdf_co_fail; 
        } 

#ifdef SDF_TLS
		if ((dMethodType == Sdf_en_invite) && \
                (pRequestAddrSpec->dType == SipAddrSipSUri) && \
                pEventContext->dProtocol == Sdf_en_protoTls)
		{
			/* Set the secure flag here. */
			(*ppCallObj)->pCallInfo->dSecureCall = Sdf_co_true;
		}
#endif /* SDF_TLS */

        /* Changes for CSR: 1-5210208 */
		/* Set the Request-URI in the CommonInfo of the Call Object */
		if( (*ppCallObj)->pCommonInfo->pNewRequestUri != Sdf_co_null )
			sip_freeSipAddrSpec((*ppCallObj)->pCommonInfo->pNewRequestUri);
     
		(*ppCallObj)->pCommonInfo->pNewRequestUri = pRequestAddrSpec;
		HSS_LOCKEDINCREF(pRequestAddrSpec->dRefCount);
        /* Changes for CSR: 1-5210208 */
        
		/* Free the local reference here. */
		 sip_freeSipAddrSpec(pRequestAddrSpec);
		sip_freeSipReqLine(pRequest);

#ifdef SDF_TLS
		/*
		 * Store the Contact address type in the CallInfo structure so that
		 * correct type of Contact headers can be inserted in the response
		 * messages for this request.
		 */
		if (Sdf_co_fail == sdf_fn_uaGetContactAddrType(\
				(*ppCallObj)->pUasTransaction->pSipMsg, \
				(*ppCallObj), &((*ppCallObj)->pCallInfo->dContactAddrType), \
				SipMessageResponse, pErr))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaHandleCall( ):"
				"Failed to get Contact header URI type.",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
#endif /* SDF_TLS */
	}

#ifdef SDF_REFER
	if( (dTransactionType == Sdf_en_InvalidTransaction) || 
    	(dTransactionType == Sdf_en_uacTransaction) ||
		((dTransactionType == Sdf_en_RegularUacTransaction)&&\
		 (dMethodType != Sdf_en_refer)))
#else
	if( (dTransactionType == Sdf_en_InvalidTransaction) || 
    	(dTransactionType == Sdf_en_uacTransaction) ||
		(dTransactionType == Sdf_en_RegularUacTransaction))
#endif
	{
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaHandleCall() : Stray "
			"SIP response message received.", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	pslMessageBody = &((*ppCallObj)->pUasTransaction->slMsgBodyList);
	ppMessage = &((*ppCallObj)->pUasTransaction->pSipMsg);

	if( (dTransactionType == Sdf_en_RegularUasTransaction) ||
		(dTransactionType == Sdf_en_overlapTransaction))
	{
		if (sdf_ivk_uaInitOverlapTransInfo(&pOverlapTransInfo, pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"Failed to initialize an OverlapTransInfo element", pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
			return Sdf_co_fail;
		}
		pOverlapTransInfo->dMsgType = dMethodType;
		/* Set the Overlap Branch paramater also */
		pOverlapTransInfo->pOverlapViaBranch = Sdf_mc_strdupAccessor( \
				(*ppCallObj)->pCommonInfo->pKey->pRemoteViaBranch);

		pOverlapTransInfo->dSocket.dSockfd = \
			(*ppCallObj)->pUasTransaction->dSocket.dSockfd;

		pOverlapTransInfo->dSocket.dProtocol = \
			(*ppCallObj)->pUasTransaction->dSocket.dProtocol;

		(*ppCallObj)->pUasTransaction->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
		(*ppCallObj)->pUasTransaction->dSocket.dProtocol = Sdf_en_protoNone;
		pOverlapTransInfo->pOLTxnKey = (*ppCallObj)->pUasTransaction->pTxnKey;
		(*ppCallObj)->pUasTransaction->pTxnKey = Sdf_co_null;

		if (sdf_listAppend(&((*ppCallObj)->slOverlapTransInfo), \
							pOverlapTransInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall():"
				"Failed to add OverlapTransInfo element into"
				"call object",pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		pOverlapTransInfo->pSipMsg = pIncMessage;

		ppMessage = &(pOverlapTransInfo->pSipMsg);
		pslMessageBody = &(pOverlapTransInfo->slMsgBodyList);
		HSS_LOCKEDINCREF((*ppMessage)->dRefCount);
	}
	if( Sdf_co_success != sdf_fn_uaUpdateToTag(*ppCallObj, *ppMessage, 
							&dIsToTagPresent, pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"To Tag insertion failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
		return Sdf_co_fail;
	}

	if( Sdf_co_success != sdf_fn_uaValidateRequireAndSupported( *ppCallObj, \
			dTransactionType, pEventContext, pOverlapTransInfo, pErr))
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"Validation of Require and Supported headers Failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
			return Sdf_co_fail;
	}


#ifdef SDF_THREAD_SAFE
	(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
#endif

	/*
	 * Check if the SIP message is present in the UAC or UAS structure.
	 * If it is present in the UAC structure, it is a stray response
	 * and can be dropped.
	 */
	if ((*ppCallObj)->pUacTransaction->pSipMsg != Sdf_co_null)
	{	
		sdf_fn_uaUpdateCallFlow (*ppMessage, Sdf_co_true);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaHandleCall() : Stray "
			"SIP response message received.", pErr);
#endif
		return Sdf_co_fail;
	}	
	
	/* Update the scheme of the Commoninfo strcuture */
    (*ppCallObj)->pCommonInfo->dScheme = pEventContext->dProtocol;

	sdf_fn_uaUpdateCallFlow ( *ppMessage, Sdf_co_true);
		
#ifdef SDF_THREAD_SAFE
	(void)sdf_ivk_uaLockCallObject(*ppCallObj, pErr);
#endif

		
	/* Update the contents of the common info w.r.t. the From, To, call-id,
	 * cseq values.  */
	if (sdf_fn_uaUpdateCallConstants((*ppCallObj), *ppMessage, \
			dMessageInfo, Sdf_co_true, pErr) == 	Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Failed to set constants for the call", pErr);
#endif			
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
		return Sdf_co_fail;
	}

	/* Update the call object with the details of the incoming SIP message. */
	if (sdf_fn_uaUpdateCallObject((*ppCallObj), pOverlapTransInfo, *ppMessage, \
			dMessageInfo, pErr) == Sdf_co_fail)
	{	
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Updation of CallObject details failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/* Set dCaller to Sdf_en_callee.  */
	(*ppCallObj)->pCommonInfo->pKey->dCaller = Sdf_en_callee;
	

	/* if the state maintainence is txn stateful mode, issue
	 * sdf_cbk_uaTransactionReceived() callback.  */
	
	if (pGlbToolkitData->dStateMaintenance == Sdf_en_transactionStateful)
	{
		dCbkRetVal = sdf_fn_uaIssueTransactionCallback(ppCallObj, \
						pEventContext, pOverlapTransInfo, pErr);
#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&((*ppCallObj)->dCallObjectMutex), \
			dCallingThread) == Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
		}
#endif
		if (dCbkRetVal == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): Failed in "
				"TransactionReceived callback/SubsLayer Handler", pErr);
#endif
			return Sdf_co_fail;
		}
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_ivk_uaHandleCall");
		return Sdf_co_success;
	}


	/* 
	 * A stray request has been received. Send a 481  
	 * (Call Leg/Transaction does not exist) response 
	 * for all requests other than ACK. Stray ACK requests 
	 * are dropped according to the RFC. This also handles 
	 * the specific case of INFO, where this INFO has come 
	 * outside of an existing call. 
	 */
	if( (dMethodType == Sdf_en_bye)  || (dMethodType == Sdf_en_info) || 
#ifdef SIP_RPR
		(dMethodType == Sdf_en_prack)|| 
#endif /* SIP_RPR */
		(dMethodType == Sdf_en_comet)|| 
		(dMethodType == Sdf_en_cancel) || (dMethodType == Sdf_en_update))
	{
		if( sdf_fn_uaFormAndSendResponse(*ppCallObj, ppMessage, 481, \
				pEventContext, pErr) == Sdf_co_fail )
        {   
#ifdef SDF_THREAD_SAFE
	      	(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
#endif
				return Sdf_co_fail;				
        }        

#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
#endif
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_ivk_uaHandleCall");
		pErr->errCode = Sdf_en_noCallExists;
		return Sdf_co_fail;
	}
	else if(dMethodType == Sdf_en_ack)
	{
		pErr->errCode = Sdf_en_noUaError;
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
#endif
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_ivk_uaHandleCall");
		return Sdf_co_success;
	}
	
	if(Sdf_co_success != sdf_fn_uaValidateCseqValue( *ppMessage, pErr))
	{
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Failed to get CSeq header from the Message",pErr);
#endif
		return Sdf_co_fail;
	}
	

	/* 
	 * Check if the message contains a To tag. if it does, then issue
	 * sdf_cbk_uaUnexpectedRequestReceived() callback. The application is
	 * expected to send a 481 from inside this callback or reconstruct the
	 * call object and proceed with the call.
	 */

	if (dIsToTagPresent == Sdf_co_true)
	{
		dCbkRetVal = sdf_cbk_uaUnexpectedRequestReceived(ppCallObj, \
			pEventContext, pOverlapTransInfo, pErr);
#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&((*ppCallObj)->dCallObjectMutex), \
			dCallingThread) == Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
		}
#endif
		if (dCbkRetVal == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"Failed to issue the TransactionReceived callback", pErr);
#endif
			return Sdf_co_fail;
		}
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_ivk_uaHandleCall");
		return Sdf_co_success;
	}


	/*
	 * On receiving any Transaction known to the UATK issue the 
	 * sdf_cbk_uaTransactionReceived callback
	 */
	if ( (dMethodType == Sdf_en_options) ||
#ifdef SDF_REFER
		(dMethodType == Sdf_en_refer) ||
#endif
#ifdef SIP_IMPP
		(dMethodType == Sdf_en_notify) ||
		(dMethodType == Sdf_en_subscribe) ||
#endif
#ifdef SDF_IM
		(dMethodType == Sdf_en_message) ||
#endif
		(dMethodType == Sdf_en_register))
	{
		dCbkRetVal = sdf_fn_uaIssueTransactionCallback(ppCallObj, \
				pEventContext, pOverlapTransInfo, pErr);
#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&((*ppCallObj)->dCallObjectMutex), \
			dCallingThread) == Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
		}
#endif
		if(dCbkRetVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"TransactionReceived callback/SubsLayer Handler", pErr);
#endif
			return Sdf_co_fail;
		}
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_ivk_uaHandleCall");
		return Sdf_co_success;
	}

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0, &((*ppCallObj)->dCallStateMutex), 0);
#endif

	if (sdf_ivk_uaChangeCallState((*ppCallObj), pOverlapTransInfo, \
		*ppMessage, Sdf_en_msgModeRecd, pErr) == Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
		sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Changing call state failed", pErr);
#endif
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}
#ifndef SDF_UASSF	
	/*Change Media State*/
	if(sdf_fn_uaChangeMediaState(*ppCallObj, \
				&((*ppCallObj)->pMediaInfo->dMediaStateChange), dMessageInfo, \
				*ppMessage,Sdf_en_msgModeRecd,pErr)==Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
		sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Changing Media state failed", pErr);
#endif
		pErr->errCode=Sdf_en_mediaStateTransitionError;
		return Sdf_co_fail;
	}
#endif
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif

	/* Call "Session Timer Update" here */
#ifdef SDF_SESSION_TIMER
    /* Update the SessionTimerInfo structure w.r.t incoming INVITE message */
    if (sdf_fn_uaUpdateSessionInfoFromMsg(*ppCallObj, pOverlapTransInfo, \
		*ppMessage, dMessageInfo, Sdf_en_idle, pEventContext, \
		pErr)==Sdf_co_fail)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
            (Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
            "Updation of SessionTimerInfo details failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
        (void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
		sdf_fn_uaUnlockMutex(0, &((*ppCallObj)->dCallStateMutex));
#endif
        return Sdf_co_fail;
    }
#endif

	if (sdf_fn_uaStoreAllMsgBodies( *ppMessage, pslMessageBody,\
		 pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
			"Storing all message bodies in call object failed",pErr);
#endif
		(*ppCallObj)->pUasTransaction->dTransactionState = Sdf_en_idle;
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
#endif
		return Sdf_co_fail;
	}

	if (dMethodType == Sdf_en_invite)
	{	
		dCbkRetVal = sdf_fn_uaHandleInvite(ppCallObj, *ppMessage, \
						pEventContext, Sdf_en_idle, pErr);

#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&((*ppCallObj)->dCallObjectMutex), dCallingThread)
			== Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, &dLocalError);
		}
#endif
		if (dCbkRetVal == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"Callback sdf_cbk_uaNewCallReceived failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_fn_uaRestoreOldState(&((*ppCallObj)->pCallInfo->dState), \
					&((*ppCallObj)->dCallStateMutex), Sdf_en_idle);
#else
			(void)sdf_fn_uaRestoreOldState(&((*ppCallObj)->pCallInfo->dState), \
					Sdf_en_idle);
#endif
			return Sdf_co_fail;
		}
	}
	else
	{
		/*
		 * At this point, any request other than ACK, is not
		 * understood by the toolkit. Hence issue the Unknown Request
		 * callback to the application
		 */
	

#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s -  Unknown Method Request "
			"received;Issuing sdf_cbk_uaUnknownMethodReceived to "
			"application",(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif

		/* Issue the UnknownMethod Received callback to the application */
		dCbkRetVal = sdf_cbk_uaUnknownMethodReceived(ppCallObj, \
			pEventContext, pOverlapTransInfo, pErr);

#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&((*ppCallObj)->dCallObjectMutex), dCallingThread)
			== Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(*ppCallObj, pErr);
		}
#endif
		
		if (dCbkRetVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleCall(): "
				"Callback sdf_cbk_uaUnknownMethodReceived failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting sdf_ivk_uaHandleCall");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaUpdateCallDetails
 **
 ** DESCRIPTION: Updates the callobject with the details of the incoming 
 **				 SipMessage. Also issues the appropriate callback to the 
 **				 application.
 **
 ******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaUpdateCallDetails
#ifdef ANSI_PROTO
	(Sdf_st_callObject 	**ppOriginalCallObj,
	Sdf_st_callObject 	*pTempCallObj,
	Sdf_st_eventContext *pEventContext,
	Sdf_st_error *pErr)
#else
	(ppOriginalCallObj,pTempCallObj,pEventContext,pErr)
	Sdf_st_callObject 	**ppOriginalCallObj;
	Sdf_st_callObject 	*pTempCallObj;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error 		*pErr;
#endif
{
#ifndef SDF_LINT    
	Sdf_ty_s8bit 	*pMethod = Sdf_co_null;
#endif    
	Sdf_ty_state 	dOldCallState, dCurrentCallState;
	Sdf_ty_mediaState	dOldMediaState = Sdf_en_idleMedia;
	Sdf_ty_state 	dOldUacTransactionState, dOldUasTransactionState;
	Sdf_ty_state 	dOldOverlapTransInfoState=Sdf_en_idle;
	Sdf_ty_state 	dRestorationState = Sdf_en_idle;
	Sdf_ty_state 	*pRestorationState;
	Sdf_ty_retVal 	dCbkRetVal=Sdf_co_fail;
	Sdf_ty_u16bit 	dStatusCode;
	Sdf_ty_slist	*pslOperatingList = Sdf_co_null;
	SipMessage		*pIncomingMsg, **ppMessage;
	Sdf_ty_s8bit	*pViaBranch = Sdf_co_null;
	Sdf_ty_messageType 		 dMethodType = Sdf_en_unknownRequest;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	en_SipMessageType 		 dType;
	Sdf_ty_messageValidity	 dValidity;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
 	Sdf_ty_msgMode 			dMode = Sdf_en_msgModeRecd;
  Sdf_st_msgInfo dMessageInfo    = {Sdf_co_null, SipMessageAny, 
                                    Sdf_en_RegularTxn, 0, 0};
	Sdf_ty_bool				dInviteInProg = Sdf_co_false;
#ifdef SDF_SUBSCRIPTION_LAYER
  Sdf_ty_bool dSubsFlag       = Sdf_co_false;
  Sdf_ty_bool dIsErrorHandled = Sdf_co_false;
#endif
#ifndef SDF_TXN
	Sdf_ty_matchMessageType dRetransmit = Sdf_en_success;
#endif
#ifdef SDF_TXN
	SipTxnKey		**ppSipTxnKey		= Sdf_co_null;
	SipTxnKey		 *pIncomingTxnKey	= Sdf_co_null;
#endif
#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif
#ifdef SDF_THREAD_SAFE
	Sdf_ty_mutexIdT		*pMutex = Sdf_co_null;
	Sdf_ty_threadId 	dCallingThread = 0;
	Sdf_st_error		dLocalError;
#endif
	Sdf_st_socket		*pIncSocket = Sdf_co_null;
	Sdf_st_socket		*pOrgSocket = Sdf_co_null;
    Sdf_ty_bool         dRequestTagMismatch = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - " 
				"Entering sdf_ivk_uaUpdateCallDetails");

#ifdef SDF_THREAD_SAFE
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaUpdateCallDetails(): pErr passed is Invalid");
#endif
		return Sdf_en_failed;
	}

	if ((*ppOriginalCallObj) == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"ppOriginalCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}	

	if (pTempCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"pTempCallObj passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}	
#endif

	/*
	 * Figure out whether the incoming SipMessage is in the UAS or the UAC
	 * depending upon which is not NULL
	 */
	if (pTempCallObj->pUacTransaction->pSipMsg != Sdf_co_null)
		pIncomingMsg = pTempCallObj->pUacTransaction->pSipMsg;
	else
		pIncomingMsg = pTempCallObj->pUasTransaction->pSipMsg;
		

	if( Sdf_co_success != sdf_fn_uaGetMessageInfo(pIncomingMsg, &dMessageInfo, \
		 pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Getting Message Info from SipMessage failed",pErr);
#endif
		return Sdf_en_failed;
	}

	dMethodType = dMessageInfo.dMsgType;		
	dType 		= dMessageInfo.dReqRespType;
	dStatusCode = dMessageInfo.dResponse;
#ifndef SDF_LINT
    pMethod	    = dMessageInfo.pMethod;
#endif

	dTransactionType = sdf_fn_getTransactionType( pIncomingMsg, \
								dMessageInfo, Sdf_co_true, pErr);

	if( dTransactionType == Sdf_en_InvalidTransaction )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Invalid Transaction received",pErr);
#endif
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}
	
#ifndef SDF_TXN
	if((dMethodType != Sdf_en_ack) && \
		(dTransactionType == Sdf_en_RegularUasTransaction))
		dRetransmit	 = sdf_fn_uaCheckRetransmission(pIncomingMsg, \
							pEventContext, pErr);

	if(dRetransmit == Sdf_en_remoteRetransmission)
	{	
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - " \
			"Exiting sdf_ivk_uaUpdateCallDetails" \
			"Request Message is retransmitted one");
		pErr->errCode = Sdf_en_noUaError;
		return Sdf_en_discarded;
	}
#endif


#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(0,&((*ppOriginalCallObj)->dCallStateMutex),0);
#endif

	/* ----------------------------------------------------
	 *  Now check if the message is valid or not. If not,
	 *  stop processing straightaway.
	 * ----------------------------------------------------
	 */
	/* If incoming request is not having To tag and existing call object has
	 * To tag then reject the request with 481 response. */
	if(Sdf_co_success != sdf_fn_uaCheckToTagValidity( (*ppOriginalCallObj), \
					pTempCallObj, dMessageInfo, pEventContext, pErr))
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
#endif
		if( pErr->errCode == Sdf_en_forkedRespError)
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSprintf(Trace,\
				"Forked Response Received in sdf_ivk_uaUpdateCallDetails");
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_mesgTraces,Trace,0,pErr);
#endif
			return Sdf_en_valid;
		}
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateCallDetails()"
			"Failed while checking To tag validity",pErr);
#endif
		return Sdf_en_failed;
	}
	
	dValidity = sdf_fn_uaCheckMessageValidity((*ppOriginalCallObj), \
		pTempCallObj, dMessageInfo, pIncomingMsg, dTransactionType, \
		pEventContext, pErr);
	
	if (dValidity != Sdf_en_valid)
	{
#ifdef SDF_ERROR
		if(dValidity !=Sdf_en_discarded)
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
				"Message validity check failed.",pErr);
#endif
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
#endif
		return dValidity;
	}
    if ( dRequestTagMismatch == Sdf_co_true )
    {
        SipHeader		        dToHeader;
        Sdf_ty_s8bit		    *pExistTag = Sdf_co_null;

        /* Here there is a To-tag mismatch in the incoming request, and the
         * mereged request/loop detetction handling has already been done
         * in the above function i.e. check message validity. So here,if it's
         * a non- Invite or an Invite that comes with a To tag then reject it
         * with a 481 response */

        if( pOverlapTransInfo != Sdf_co_null )
            (void)sip_getHeader(pOverlapTransInfo->pSipMsg, SipHdrTypeTo, &dToHeader,\
                    (SipError*)	&(pErr->stkErrCode));
        else
            (void)sip_getHeader(pTempCallObj->pUasTransaction->pSipMsg, SipHdrTypeTo, &dToHeader,\
                    (SipError*)	&(pErr->stkErrCode));

        (void)sip_getTagAtIndexFromToHdr(&dToHeader, &pExistTag, 0, \
                (SipError *)&(pErr->stkErrCode));
        sip_freeSipHeader(&dToHeader);


        if( ((dMessageInfo.dMsgType == Sdf_en_invite ) && (pExistTag != Sdf_co_null)) || (dMessageInfo.dMsgType != Sdf_en_invite ))
        {
            SipMessage	            *pTempMessage = Sdf_co_null;
            Sdf_st_overlapTransInfo	*pOvlapTransInfo = Sdf_co_null;
            SipOptions 	         	dOptions;
#ifndef SDF_LINT
            Sdf_st_socket	        *pSockfd = Sdf_co_null;
#endif
            SipHeader		        dHeader;
            Sdf_ty_s8bit	        *pTmpTagValue = Sdf_co_null;
            SipHeader			    *pExistLocalHdr = Sdf_co_null;
            Sdf_ty_s8bit		    *pExistLocalTag = Sdf_co_null;

            if (sdf_ivk_uaRejectRequest(pTempCallObj, \
                        (*ppOriginalCallObj)->pInitData, 481, \
                        &pOvlapTransInfo, pErr) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
                        (Sdf_ty_s8bit*)"sdf_ivk_uaUpdateCallDetails()"
                        ": Failed to form 481 response", pErr);
#endif
                return Sdf_en_failed;
            }

            dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

            if( pOvlapTransInfo != Sdf_co_null )
                pTempMessage = pOvlapTransInfo->pSipMsg;
            else
            {
                pTempMessage = pTempCallObj->pUasTransaction->pSipMsg;
#ifndef SDF_LINT
                pSockfd = &(pTempCallObj->pUasTransaction->dSocket);
#endif
            }

            /* If Local tag is not matching, then set the currect local tag
             * to this failed response */

            (void)sip_getHeader(pTempMessage, SipHdrTypeTo, &dHeader,\
                    (SipError*)	&(pErr->stkErrCode));

            pExistLocalHdr  = (*ppOriginalCallObj)->pCommonInfo->pKey->pLocalAddr;

            if( pExistLocalHdr->dType == SipHdrTypeTo)
                (void)sip_getTagAtIndexFromToHdr(pExistLocalHdr, &pExistLocalTag, 0, \
                        (SipError *)&(pErr->stkErrCode));
            else
                (void)sip_getTagAtIndexFromFromHdr(pExistLocalHdr, &pExistLocalTag, 0, \
                        (SipError *)&(pErr->stkErrCode));

            pTmpTagValue = Sdf_mc_strdupCallHandler(pExistLocalTag);

            (void)sip_setTagAtIndexInToHdr(&dHeader, pTmpTagValue, 0, \
                    (SipError *)&(pErr->stkErrCode));

            sip_freeSipHeader(&dHeader);

            /* CSR_1-8290042 changes start */
            if(sdf_ivk_uaSendCallToPeer(pTempCallObj, pTempMessage, \
                        dOptions, 0, pEventContext, Sdf_co_null, pErr) == Sdf_co_fail)
            /* CSR_1-8290042 changes end */
            {
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
                        (Sdf_ty_s8bit*)"sdf_ivk_uaUpdateCallDetails() :" \
                        "Sending response to peer entity failed",pErr);
#endif
                return Sdf_en_failed;
            }
            return Sdf_en_failed;
        }
    }

	/* check if the received CANCEL matches the INVITE */
	if ((dType == SipMessageRequest) && (dMethodType == Sdf_en_cancel) && \
		(Sdf_co_false == sdf_fn_uaIsCancelMatching(\
		pTempCallObj, *ppOriginalCallObj, pErr)))
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
#endif

		if (sdf_ivk_uaRejectRequest(pTempCallObj, \
				 (*ppOriginalCallObj)->pInitData, 481, \
				 &pOverlapTransInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateCallDetails()"
				": Failed to form 481 response",pErr);
#endif
			return Sdf_en_failed;
		}

		pErr->errCode = Sdf_en_cancelTxnMismatch;
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_ivk_uaUpdateCallDetails");
		return Sdf_en_failed;
	}
	/* If received request is an Invite, 
	 *  Set the flag in call object saying that invite is in process */
	if((dMethodType == Sdf_en_invite) && (dType == SipMessageRequest))
	{
		(*ppOriginalCallObj)->pCallInfo->dInviteInProg = Sdf_co_true;
		dInviteInProg = Sdf_co_true;
	}
	
#ifdef SDF_THREAD_SAFE
	/* Before grabbing the CallObject Mutex 
	 * Unlock the Call State Mutex
	 * and then again grab the Call State Mutex
	 */
	sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);

	pMutex = &((*ppOriginalCallObj)->dCallObjectMutex);

	(void)sdf_ivk_uaLockCallObject(*ppOriginalCallObj, pErr);
	sdf_fn_uaLockMutex(0, &((*ppOriginalCallObj)->dCallStateMutex),0);
#endif	
	/* -------------------------------------------------------
	 * Before proceeding, get the overlapping transaction
	 * (or initialize a new one and add to overlaptransaction
	 * list) if required. 
	 * -------------------------------------------------------
	 */

	/* Reset the invite progress variable, because from this point,
	 * call Object got locked. No other StartTransaction can be done
	 * over this call object */
	if( dInviteInProg == Sdf_co_true )
		(*ppOriginalCallObj)->pCallInfo->dInviteInProg = Sdf_co_false;


	if(Sdf_co_success != sdf_fn_uaGetOverlapTransInfo( *ppOriginalCallObj,  \
		pIncomingMsg, &pOverlapTransInfo, dStatusCode, dTransactionType, \
		dType, dMethodType, pErr))
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
#endif
		if( pErr->errCode != Sdf_en_noUaError )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaUpdateCallDetails()"
				"Failed To get the Overlap TransInfo", pErr);
#endif
			return Sdf_en_failed;
		}
		return Sdf_en_discarded;
	}

	/* 
	 * Save the old states so that we can restore in case an API fails
	 */
	dOldCallState = (*ppOriginalCallObj)->pCallInfo->dState;
	dOldUacTransactionState = \
		(*ppOriginalCallObj)->pUacTransaction->dTransactionState;
	dOldUasTransactionState = \
		(*ppOriginalCallObj)->pUasTransaction->dTransactionState;

	if (pOverlapTransInfo != Sdf_co_null)
		dOldOverlapTransInfoState = pOverlapTransInfo->dState;

	/*Save the Old Media State*/

	dOldMediaState = (*ppOriginalCallObj)->pMediaInfo->dMediaState;

	/* -----------------------------------------------------------
	 * If ACK's to final responses are retransmitted, the callback 
	 * should be issued only once.
	 * -----------------------------------------------------------
	 */
	if(dMethodType == Sdf_en_ack)
	{
		if ( (dOldCallState == Sdf_en_callEstablished) || \
			(dOldCallState == Sdf_en_idle) || \
			(dOldCallState == Sdf_en_byeSent) || \
			(dOldCallState == Sdf_en_byeReceived) )
		{
#ifdef SDF_THREAD_SAFE
			sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
			(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif
			pErr->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
				"Exiting sdf_ivk_uaUpdateCallDetails");
			return Sdf_en_valid;
		}
	}

	/* -------------------------------------------------------
	 * 		Begin the code block to change call state.
	 * -------------------------------------------------------
	 * 
	 * 1. If CANCEL or BYE, Change call state using
	 * 		Sdf_en_msgModeCancelRecd or Sdf_en_msgModeByeRecd
	 * 		
	 * 2. For any other message, change call state normally
	 * -------------------------------------------------------
	 */
	if (dType == SipMessageRequest)
	{
		if (dMethodType == Sdf_en_cancel)
			dMode = Sdf_en_msgModeCancelRecd;
		else if (dMethodType == Sdf_en_bye) 
			dMode = Sdf_en_msgModeByeRecd;
	}


	if (sdf_ivk_uaChangeCallState(*ppOriginalCallObj, pOverlapTransInfo, \
		pIncomingMsg, dMode, pErr) == Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Changing call state failed",pErr);
#endif
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_en_failed;
	}	
#ifndef SDF_UASSF	
	if(sdf_fn_uaChangeMediaState(*ppOriginalCallObj, \
			&((*ppOriginalCallObj)->pMediaInfo->dMediaStateChange), \
			dMessageInfo, pIncomingMsg,Sdf_en_msgModeRecd,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_mediaStateTransitionError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Changing Media state failed",pErr);
#endif
		pErr->errCode=Sdf_en_mediaStateTransitionError;
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaUnlockMutex(0,	&(*ppOriginalCallObj)->dCallStateMutex);
		sdf_fn_uaUnlockMutex(dCallingThread, pMutex);
#endif
		return Sdf_en_failed;
	}
#endif
	dCurrentCallState = (*ppOriginalCallObj)->pCallInfo->dState;


	/* ----------------------------------------------
	 *  Ending code block to change the state here
	 * ----------------------------------------------
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(*ppOriginalCallObj)->dCallStateMutex);
#endif

	/* 
	 * ===========================================================
	 * Have changed the state now. Proceed with rest of processing
	 * ===========================================================
	 */
	/* -----------------------------------------------------------
	 *  First and foremost: CANCEL or BYE requests meant for call
	 *  cancellation. Issue sdf_cbk_uaCancelReceived callback here.
	 * -----------------------------------------------------------
	 */
	if ( (dType == SipMessageRequest) && \
         ( (dMethodType == Sdf_en_cancel) || \
           ( (dMethodType == Sdf_en_bye)  && \
             ( (dOldCallState == Sdf_en_inviteReceived) || \
			   (dOldCallState == Sdf_en_provisionalRespSent) ) ) ) )
	{
		if(Sdf_en_valid != sdf_fn_uaUpdateTempCallObject(pTempCallObj, \
			*ppOriginalCallObj, pIncomingMsg, dMessageInfo, pErr))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
				"Failed to Update the TempCallObject",pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState), \
				&((*ppOriginalCallObj)->dCallStateMutex), dOldCallState);
#else
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState), dOldCallState);
#endif
			(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
				
			return Sdf_en_failed;
		}
		
#ifdef SDF_THREAD_SAFE
		/* Unlock the original call object before issueing 
		 * sdf_cbk_uaCancelReceived() as it is not passed as an argument
		 * to this callback and the application may try to fetch and use the
		 * original call object. Since the original call object is not an
		 * argument to this callback, this case should be treated as identical
		 * to manipulating the call object outside the callbacks, where the
		 * application is expected to lock it.
		 */
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);

		/* 
		 * Lock the temporary call object and Issue the 
		 * sdf_cbk_uaCancelReceived callback to the application. 
		 * Note that locking is not strictly necessary here as only the 
		 * temporary call object is passed to the callback, but it is done just 
		 * ensure consistency with the other callbacks.
		 */
		(void)sdf_ivk_uaLockCallObject(pTempCallObj, pErr);
#endif


#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
			"Call : %s -  Call Cancellation Request received; "
			"Issuing sdf_cbk_uaCancelReceived to application",\
			(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif
	
		dCbkRetVal = sdf_cbk_uaCancelReceived(&pTempCallObj, \
							pEventContext, pErr);

#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(&(pTempCallObj->dCallObjectMutex), \
				dCallingThread) == Sdf_co_success)
		{
			(void)sdf_ivk_uaUnlockCallObject(pTempCallObj, pErr);
		}
#endif
		if (dCbkRetVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
				"Callback sdf_cbk_uaCancelReceived failed",pErr);
#endif
			return Sdf_en_failed;
		}
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_ivk_uaUpdateCallDetails");
		return Sdf_en_valid;
	}

	/* 
	 * ===========================================================
	 * For all other cases, updating the permanent call object 
	 * happens here. Updations include updating SipMessage of the
	 * transaction, headerlists and message body list.
	 * ===========================================================
	 */

	/*
	 * Choose the correct elements to work upon.
	 */
	if( (dTransactionType == Sdf_en_overlapTransaction) ||
	 	(dTransactionType == Sdf_en_RegularUasTransaction) ||
	 	(dTransactionType == Sdf_en_RegularUacTransaction) )
	{
#ifdef SDF_LINT
        if(pOverlapTransInfo == Sdf_co_null)
        	return Sdf_en_failed;
        else
        {
#endif            
        ppMessage			= &(pOverlapTransInfo->pSipMsg);
		pslOperatingList	= &(pOverlapTransInfo->slMsgBodyList);
		pRestorationState	= &(pOverlapTransInfo->dState);
		dRestorationState	= dOldOverlapTransInfoState;
#ifdef SDF_TXN
		ppSipTxnKey			= &(pOverlapTransInfo->pOLTxnKey);
#endif
#ifdef SDF_LINT
        }        
#endif
	}
	else if ( dType == SipMessageRequest )
	{
		ppMessage = &((*ppOriginalCallObj)->pUasTransaction->pSipMsg);
		pslOperatingList = \
			&((*ppOriginalCallObj)->pUasTransaction->slMsgBodyList);
		pRestorationState = \
			&((*ppOriginalCallObj)->pUasTransaction->dTransactionState);
		dRestorationState = dOldUasTransactionState;	
#ifdef SDF_TXN
		ppSipTxnKey = &((*ppOriginalCallObj)->pUasTransaction->pTxnKey);
#endif
	}
	else
	{
		ppMessage = &((*ppOriginalCallObj)->pUacTransaction->pSipMsg);
		pslOperatingList = \
			&((*ppOriginalCallObj)->pUacTransaction->slMsgBodyList);
		pRestorationState = \
			&((*ppOriginalCallObj)->pUacTransaction->dTransactionState);
		dRestorationState = dOldUacTransactionState;	
#ifdef SDF_TXN
		ppSipTxnKey = &((*ppOriginalCallObj)->pUacTransaction->pTxnKey);
#endif
	}

	/* Free existing contents */
	if (*ppMessage != Sdf_co_null)
	{
		sip_freeSipMessage(*ppMessage);
		*ppMessage = Sdf_co_null;
	}

	/* Point ppMessage to the new SipMessage */
	*ppMessage = pIncomingMsg;
	HSS_LOCKEDINCREF(pIncomingMsg->dRefCount);
	
#ifdef SDF_TXN
	if (pTempCallObj->pUacTransaction->pSipMsg != Sdf_co_null)
		pIncomingTxnKey	= pTempCallObj->pUacTransaction->pTxnKey;
	else
		pIncomingTxnKey	= pTempCallObj->pUasTransaction->pTxnKey;
	
	if ( ((*ppSipTxnKey) != pIncomingTxnKey) && \
		 (pIncomingTxnKey != Sdf_co_null) )
	{
		/* The transaction has changed. Update the keys now.  */
		sip_freeSipTxnKey(*ppSipTxnKey);
		*ppSipTxnKey = pIncomingTxnKey;
		HSS_LOCKEDINCREF(pIncomingTxnKey->dRefCount);
	}
#endif

	/* 
	 * Update the transaction's header lists and other call object details 
	 * with the values of the incoming SIP message.
	 */
	if (sdf_fn_uaUpdateCallObject(*ppOriginalCallObj, pOverlapTransInfo, \
		pIncomingMsg, dMessageInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
		(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, dRestorationState);
		if (dCurrentCallState != dOldCallState)
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState), \
				&((*ppOriginalCallObj)->dCallStateMutex), dOldCallState);
#else
		(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
		if (dCurrentCallState != dOldCallState)		
			(void)sdf_fn_uaRestoreOldState(  \
				&((*ppOriginalCallObj)->pCallInfo->dState), dOldCallState);
#endif
		(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Updating Call Object failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;

		return Sdf_en_failed;
	}

	/* Do not perform this check for ACK */
	
	if(dMethodType != Sdf_en_ack)
	{
		if(Sdf_co_success != sdf_fn_uaValidateRequireAndSupported( \
			*ppOriginalCallObj, dTransactionType, pEventContext, \
			pOverlapTransInfo, pErr))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
				"Require and Supported Headers validation failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
			(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
#endif
			return Sdf_en_failed;
		}
	}

#ifdef SDF_SESSION_TIMER
	if( Sdf_co_success != sdf_fn_uaUpdateSessionInfoFromMsg( \
		*ppOriginalCallObj, pOverlapTransInfo, pIncomingMsg, dMessageInfo, \
		dOldCallState, pEventContext, pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails( ): "
			"function UpdateSessionInfoFromMsg() Failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
		(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, dRestorationState);
		if (dCurrentCallState != dOldCallState)		
		(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->pCallInfo->dState), \
			&((*ppOriginalCallObj)->dCallStateMutex), dOldCallState);
#else
		(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
		if (dCurrentCallState != dOldCallState)		
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState),dOldCallState);
#endif
		(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
		return Sdf_en_failed;
	}
#endif	

	/* Delete existing contents */
	(void)sdf_listDeleteAll(pslOperatingList, pErr);
	
	/* Store new contents */
	if (sdf_fn_uaStoreAllMsgBodies(pIncomingMsg, pslOperatingList,  \
							pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
			"Storing all message bodies in call object failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE
		(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
		(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, dRestorationState);
		if (dCurrentCallState != dOldCallState)		
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState), \
				&((*ppOriginalCallObj)->dCallStateMutex), dOldCallState);
#else
		(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
		if (dCurrentCallState != dOldCallState)		
			(void)sdf_fn_uaRestoreOldState( \
				&((*ppOriginalCallObj)->pCallInfo->dState), dOldCallState);
#endif
		(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
		return Sdf_en_failed;
	}
	/* Update the Via branch present in original call object with
	 * the temp call object */

	/* For Regular uac transactions store the Viabranch into the
	 * overlap transaction structure and don't update the common info's
	 * key field  */

	pViaBranch = pTempCallObj->pCommonInfo->pKey->pRemoteViaBranch;

	/* If Method corresponding to Regular Transaction then 
	 * check the Request whether it is looped back one or not */
	
	if( (pViaBranch != Sdf_co_null) && (dMethodType != Sdf_en_ack) )
	{
		if( dTransactionType == Sdf_en_uasTransaction )
		{
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
				(*ppOriginalCallObj)->pCommonInfo->pKey->pRemoteViaBranch), \
				pErr);

			(*ppOriginalCallObj)->pCommonInfo->pKey->pRemoteViaBranch = \
				Sdf_mc_strdupAccessor(pViaBranch);
		}
		else if( (dTransactionType == Sdf_en_RegularUasTransaction) ||
				(( dTransactionType == Sdf_en_overlapTransaction) &&
  				(dType == SipMessageRequest)))
		{
#ifdef SDF_LINT
        if( pOverlapTransInfo == Sdf_co_null)
           return Sdf_en_failed;
        else
        {
#endif            
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&( \
				pOverlapTransInfo->pOverlapViaBranch), pErr);

			pOverlapTransInfo->pOverlapViaBranch = \
						Sdf_mc_strdupAccessor(pViaBranch);
#ifdef SDF_LINT
        }
#endif
        }
	}
	/* 
	 * Update the socket Information of the callObject in case the new 
	 * Request came on a different TCP channel.
	 */

	if(dType == SipMessageResponse)
	{
		pIncSocket = &(pTempCallObj->pUacTransaction->dSocket);
		if(pOverlapTransInfo == Sdf_co_null)
			pOrgSocket = &((*ppOriginalCallObj)->pUacTransaction->dSocket);
		else
			pOrgSocket = &(pOverlapTransInfo->dSocket);
	}
	else
	{
		pIncSocket = &(pTempCallObj->pUasTransaction->dSocket);
		if(pOverlapTransInfo == Sdf_co_null)
			pOrgSocket = &((*ppOriginalCallObj)->pUasTransaction->dSocket);
		else
			pOrgSocket = &(pOverlapTransInfo->dSocket);
	}

	if(pIncSocket->dSockfd != Sdf_co_InvalidSocketFd)
	{
		if(pOrgSocket->dSockfd != pIncSocket->dSockfd)
		{
			if(pOrgSocket->dProtocol != Sdf_en_protoUdp)	
				(void)sdf_fn_uaCloseSocket(pOrgSocket);

			pOrgSocket->dProtocol  =  pIncSocket->dProtocol;
			pOrgSocket->dSockfd    =  pIncSocket->dSockfd;

		}
	}

	/* Remove the sockinfo from  Temp Callobject.  This avoids
	 * the socket getting closed while freeing the Temp callobject*/
	pIncSocket->dProtocol = Sdf_en_protoNone;
	pIncSocket->dSockfd   = Sdf_co_InvalidSocketFd;


	/* ---------------------------------------------------
	 * 	Issuing of the callbacks other than CancelReceived
	 * 	is done here.
	 * ---------------------------------------------------
	 */
	if(dType == SipMessageRequest)
	{
		if(dMethodType == Sdf_en_invite)
		{
			dCbkRetVal = sdf_fn_uaHandleInvite(ppOriginalCallObj, \
							pIncomingMsg, pEventContext, dOldCallState, pErr);
#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
					(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj,&dLocalError);
#endif
			if((dCbkRetVal == Sdf_co_fail) && \
				(pErr->errCode != Sdf_en_callBackError))
			{
#ifdef SDF_THREAD_SAFE
				(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
							dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), &((*ppOriginalCallObj)->\
						dCallStateMutex), dOldCallState);
#else
				(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), dOldCallState);
#endif
			(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
			}
			if (dCbkRetVal == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback On receiving Invite failed",pErr);
#endif
				return Sdf_en_failed;
			}
		}
		else if (dMethodType == Sdf_en_ack)
		{
			dCbkRetVal = sdf_fn_uaHandleAck( ppOriginalCallObj, pIncomingMsg,\
							pEventContext, dOldCallState, pErr);
/*Umang- Aashish- temporary fix start*/
			/*(Sdf_ty_void)sdf_listDeleteAll(&((*ppOriginalCallObj)->pUasTransaction->slUnprocessedHeaders), \
			pErr);*/
/*Umang- Aashish- temporary fix end*/
			
#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
#endif
			if((dCbkRetVal == Sdf_co_fail) && \
				(pErr->errCode != Sdf_en_callBackError))
			{
#ifdef SDF_THREAD_SAFE
				(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
							dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), &((*ppOriginalCallObj)->\
						dCallStateMutex), dOldCallState);
#else
				(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), dOldCallState);
#endif
			(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,dOldMediaState);
			}
			
			if (dCbkRetVal == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback sdf_cbk_uaFailedCallAcknowledged failed", \
					pErr);
#endif
				return Sdf_en_failed;
			}
		}
		else if(dMethodType == Sdf_en_bye)
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s -  Call Termination request received;"
				"Issuing sdf_cbk_uaCallTerminateRequest to application",\
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif

			/* 
			 * Issue the sdf_cbk_uaCallTerminateRequest callback to the 
			 * application. 
			 */
			dCbkRetVal = sdf_cbk_uaCallTerminateRequest(ppOriginalCallObj, \
				pEventContext, pErr);

			if ((dOldCallState == Sdf_en_reInviteSent) || \
                (dOldCallState == Sdf_en_reInviteprovRespReceived )|| \
/*CSR 4566600 starts*/
				(dOldCallState == Sdf_en_finalSuccessSent) ||\
                dOldCallState == Sdf_en_reInviteprovRespReceived || \
		dOldCallState == Sdf_en_finalSuccessSent)
/*CSR 4566600 ends*/
			{
#ifdef SDF_TXN
				/*
				 * For any class of the final response, the "no-answer" timer
				 * has to be stopped. Do the same here.
			 	*/
				if (sdf_fn_uaStopNoAnswerTimer((*ppOriginalCallObj), \
				pErr) == Sdf_co_fail)
				{
					pErr->errCode=Sdf_en_callObjectAccessError;
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
						"Failed to stop the \"no-answer\" timer.", pErr);
#endif
					return Sdf_en_failed;
				}
#endif /* End - ifdef SDF_TXN */

#ifdef SDF_SESSION_TIMER
				/*
				 * Stop the session timer if it is still running.
				 */	
				if ((*ppOriginalCallObj)->pSessionTimer->pTimerId != Sdf_co_null)
				{
					if (sdf_fn_uaStopSessionTimer(\
						(Sdf_ty_pvoid *)&((*ppOriginalCallObj)->pSessionTimer->pTimerId), \
						pErr) == Sdf_co_fail)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
						"Failed to stop the session timer", pErr);
#endif	
						return Sdf_en_failed;
					}
				}
#endif
				if(sdf_ivk_uaAbortTransaction(*ppOriginalCallObj, \
					(*ppOriginalCallObj)->pUacTransaction->dTxnId, pEventContext, \
					pErr) ==	Sdf_co_fail)
						return Sdf_en_failed;
			}
			
#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif
			if (dCbkRetVal == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback sdf_cbk_uaCallTerminateRequest failed",pErr);
#endif
				return Sdf_en_failed;
			}
		}
		/*
		 * UPDATE(which is received before call is established), PRACK &
		 * COMET are causes sdf_cbk_uaOverlapTransactionReceived callback to
		 * be issued to application
		 */
#ifdef SIP_RPR
		else if((dMethodType == Sdf_en_prack) || \
				(dMethodType == Sdf_en_comet)) 
#else
		else if(dMethodType == Sdf_en_comet)	
#endif /* SIP_RPR */
		{
			/*
             * if the message is a PRACK/COMET request, issue the
             * sdf_ivk_uaOverlapTransactionRecieved callback to the application
             */
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s -  Overlap Transaction received;Issuing "
				"sdf_cbk_uaOverlapTransactionReceived to application",\
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaOverlapTransactionReceived callback to 
			 * the application 
			 */
			dCbkRetVal = sdf_cbk_uaOverlapTransactionReceived( \
				ppOriginalCallObj, pOverlapTransInfo, pEventContext, pErr);

#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif
			if (dCbkRetVal == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback sdf_cbk_uaOverlapTransactionReceived failed", \
					pErr);
#endif
				return Sdf_en_failed;
			}
		}
		else if ( (dMethodType == Sdf_en_info) || \
				(dMethodType == Sdf_en_register) || \
				(dMethodType == Sdf_en_options) ||
#ifdef SDF_REFER
				(dMethodType == Sdf_en_refer) ||
#endif
#ifdef SIP_IMPP
				(dMethodType == Sdf_en_subscribe) || \
				(dMethodType == Sdf_en_notify) ||
#endif
#ifdef SDF_IM
				(dMethodType == Sdf_en_message) ||
#endif
				(dMethodType == Sdf_en_update)) 
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s -  Transaction received;"
				"Issuing sdf_cbk_uaTransactionReceived to application",\
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces, \
				Trace,0,pErr);
#endif
			
#ifdef SDF_SUBSCRIPTION_LAYER
		  /* SubscriptionLayer handler MUST be called BEFORE issuing the 
       * TransactionReceived callback because we need to process all non-REFER 
	     * SUBSCRIBE and NOTIFY requests & responses in sdf_fn_uaSLHandler. If 
	     * the message is not handled by SubsLayer then it will be handled by
	     * TransactionReceived callback. First check if it is a non-REFER 
       * SUBSCRIBE/NOTIFY request/response before calling the SubsLayer.
       */

        /*
         * Currentcallstate is passed as parameter to fn
         * ismessageforsubslayer. 
         */

      if (sdf_fn_uaSLIsMessageForSubsLayer(&dMessageInfo, pOverlapTransInfo,
                                           &dSubsFlag, dCurrentCallState, pErr) \
              != Sdf_co_success)
      {
        /* If a mandatory header in SUBSCRIBE/NOTIFY is missing, thrown 
  			 * UnexpectedEvent callback so that Application can handle it 
				 * appropriately.
  			 */
        dIsErrorHandled = Sdf_co_true;
        dCbkRetVal      = sdf_cbk_uaSLUnexpectedEvent (ppOriginalCallObj, 
                                 Sdf_co_null, pOverlapTransInfo, pEventContext, 
                                 Sdf_en_uaSLErrorInMessage, pErr);
      }

      if (dSubsFlag == Sdf_co_true)
        dCbkRetVal = sdf_fn_uaSLHandler (ppOriginalCallObj, pOverlapTransInfo, 
                                          pEventContext, pErr);

			/* Above handler will set the dSubsFlag to true if it is			 
			 * handling the message. If the SubsLayer did not handle the message i.e.
			 * dSubsFlag is FALSE, then throw the existing UATK cbk
			 */
			if ((dSubsFlag == Sdf_co_false) && (dIsErrorHandled == Sdf_co_false))
			{
#endif
  		/* 
  		 * Issue the sdf_cbk_uaTransactionReceived callback to the 
  		 * application. 
  		 */
  		dCbkRetVal = sdf_cbk_uaTransactionReceived(ppOriginalCallObj, \
                        				pEventContext, pOverlapTransInfo, pErr);
#ifdef SDF_SUBSCRIPTION_LAYER
			}
#endif
#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif
			if (dCbkRetVal == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback failed",pErr);
#endif
				return Sdf_en_failed;
			}
		}
		/* 
		 * Unknown Method (Method not supported by UA Toolkit, but supported
		 * by application) received within a call 
		 */
		else
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Unknown method request received;"
				"Issuing sdf_cbk_uaUnknownMethodReceived to application",\
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,Trace,0,pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaUnknownMethodReceived callback to the 
			 * application. 
			 */
			dCbkRetVal = sdf_cbk_uaUnknownMethodReceived(ppOriginalCallObj,\
				pEventContext, pOverlapTransInfo, pErr);

#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif
			if (dCbkRetVal == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback sdf_cbk_uaUnknownMethodReceived failed",pErr);
#endif
				return Sdf_en_failed;
			}
		}
	} /* End of block handling all requests other than REGISTER */
	else
	{
		/* ---------------------------------------
		 *  Issue response related callbacks here.
		 * ---------------------------------------
		 */

		/* -------------------------------------------------
		 * First of all: InProgress / RPRReceived callbacks.
		 * -------------------------------------------------
		 */
		
		if( (dStatusCode >= 100) && (dStatusCode <= 199) )
		{

#ifdef SDF_TXN
			/*
			 * The message received was a provisional response. 
			 * If the method is INVITE, start a timer waiting for 
			 * a final response. If the timer already exists, reset 
			 * the timer now.
			 */
			if (dMethodType == Sdf_en_invite)
			{
				Sdf_ty_bool	dReset = Sdf_co_false;

				/*
				 * Reset the timer if we have already got a provisional
				 * response earlier.
				 */
				if ((*ppOriginalCallObj)->pCommonInfo->pNoAnswerTimerHandle != \
					Sdf_co_null)
					dReset = Sdf_co_true;

				if (sdf_fn_uaStartNoAnswerTimer(*ppOriginalCallObj, \
					pEventContext, dReset, pErr) == Sdf_co_fail)
				{
#ifdef SDF_THREAD_SAFE
					(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
					(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
						dRestorationState);
					if (dCurrentCallState != dOldCallState)		
						(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
							pCallInfo->dState), &((*ppOriginalCallObj)->\
							dCallStateMutex), dOldCallState);
#else
					(void)sdf_fn_uaRestoreOldState(pRestorationState, \
						dRestorationState);
					if (dCurrentCallState != dOldCallState)		
						(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
							pCallInfo->dState), dOldCallState);
#endif
					(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,\
							dOldMediaState);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_callObjectAccessError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
						"Failed to start the \"no-answer\" timer.", pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;

					return Sdf_en_failed;
				}
			}
#endif /* End - ifdef SDF_TXN */

#ifdef SDF_SUBSCRIPTION_LAYER
			/* The check below is needed because, otherwise, when a 1XX for INVITE is
			 * received, we would enter sdf_fn_uaSLIsMessageForSubsLayer and return
			 * failure from it (since pOverlapTransInfo would be NULL). As a result,
			 * the RPR & Progress received callbacks were not thrown.
			 * So sdf_fn_uaSLIsMessageForSubsLayer should be called only when 
			 * pOverlapTransInfo is not NULL.
			 */
			if ((dMethodType == Sdf_en_subscribe) || (dMethodType == Sdf_en_notify))
			{
		    /* SubscriptionLayer handler MUST be called BEFORE issuing the 
         * sdf_cbk_uaInProgress callback because we need to process all non-REFER
	       * SUBSCRIBE and NOTIFY responses in sdf_fn_uaSLHandler. If the message
	       * is not meant for the SubsLayer then it will be handled by
	       * sdf_cbk_uaInProgress callback.
         */
        /*
         * Currentcallstate is passed as parameter to fn
         * ismessageforsubslayer. 
         */

        if (sdf_fn_uaSLIsMessageForSubsLayer(&dMessageInfo, pOverlapTransInfo,
                                           &dSubsFlag, dCurrentCallState,pErr) \
                != Sdf_co_success)
        {
          /* If a mandatory header in SUBSCRIBE/NOTIFY is missing, thrown 
  			   * UnexpectedEvent callback so that Application can handle it 
				   * appropriately.
    			 */
          dIsErrorHandled = Sdf_co_true;
          dCbkRetVal      = sdf_cbk_uaSLUnexpectedEvent (ppOriginalCallObj, 
                                 Sdf_co_null, pOverlapTransInfo, pEventContext, 
                                 Sdf_en_uaSLErrorInMessage, pErr);
        }

        if (dSubsFlag == Sdf_co_true)
	  			dCbkRetVal = sdf_fn_uaSLHandler (ppOriginalCallObj, pOverlapTransInfo,
                                           pEventContext, pErr);

      } /* dMethodType == Sdf_en_subscribe || Sdf_en_notify */

			/* Above handler will set the dSubsFlag to true if it is			 
			 * handling the message*/
			if ((dSubsFlag == Sdf_co_false) && (dIsErrorHandled == Sdf_co_false))
			{
#endif
	
  			dCbkRetVal = sdf_fn_uaHandleProvResp(ppOriginalCallObj, \
    							pIncomingMsg, pOverlapTransInfo, pEventContext, pErr);

#ifdef SDF_SUBSCRIPTION_LAYER
      } /* End of dSubsFlag == false && dIsErrorHandled == false */
#endif

#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
#endif
			if((dCbkRetVal == Sdf_co_fail) && \
				(pErr->errCode != Sdf_en_callBackError))
			{
#ifdef SDF_THREAD_SAFE
				(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
							dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), &((*ppOriginalCallObj)->\
						dCallStateMutex), dOldCallState);
#else
				(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
				if (dCurrentCallState != dOldCallState)		
				(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), dOldCallState);
#endif
				(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,\
							dOldMediaState);
			}
			if (dCbkRetVal == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Callback sdf_cbk_uaInProgress/sdf_cbk_uaRPRReceived "
					"failed",pErr);
#endif
				return Sdf_en_failed;
			}
			sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
				"Exiting sdf_ivk_uaUpdateCallDetails");
			return Sdf_en_valid;
		}

		if ( dMethodType == Sdf_en_invite )
		{
#ifdef SDF_TXN
			/*
			 * For any class of the final response, the "no-answer" timer
			 * has to be stopped. Do the same here.
			 */
			if (sdf_fn_uaStopNoAnswerTimer((*ppOriginalCallObj), \
				pErr) == Sdf_co_fail)
			{
				pErr->errCode=Sdf_en_callObjectAccessError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Failed to stop the \"no-answer\" timer.", pErr);
#endif
#ifdef SDF_THREAD_SAFE
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
				(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
					dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), &((*ppOriginalCallObj)->\
						dCallStateMutex), dOldCallState);
#else
				(void)sdf_fn_uaRestoreOldState(pRestorationState, \
					dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), dOldCallState);
#endif
				(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,\
						dOldMediaState);
				return Sdf_en_failed;
			}
#endif /* End - ifdef SDF_TXN */

			dCbkRetVal = sdf_fn_uaHandleInvFinalResp( ppOriginalCallObj, \
				pIncomingMsg, dStatusCode, dOldCallState, pEventContext, pErr);
		
			if( (dCbkRetVal == Sdf_co_fail) && \
					(pErr->errCode != Sdf_en_callBackError))
			{
#ifdef SDF_THREAD_SAFE
				(void)sdf_fn_uaRestoreOldState(pRestorationState, pMutex, \
								dRestorationState);
				if (dCurrentCallState != dOldCallState)		
					(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
						pCallInfo->dState), &((*ppOriginalCallObj)->\
						dCallStateMutex), dOldCallState);

#else
				(void)sdf_fn_uaRestoreOldState(pRestorationState, dRestorationState);
				if (dCurrentCallState != dOldCallState)		
				(void)sdf_fn_uaRestoreOldState(&((*ppOriginalCallObj)->\
					pCallInfo->dState), dOldCallState);
#endif
				(void)sdf_fn_uaRestoreOldMediaState(*ppOriginalCallObj,\
							dOldMediaState);
			}

#ifdef SDF_THREAD_SAFE
			if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, &dLocalError);
#endif
			if (dCbkRetVal == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails(): "
					"Failed in processing the Invite Response",pErr);
#endif
				return Sdf_en_failed;
			}
			sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
				"Exiting sdf_ivk_uaUpdateCallDetails");
			return Sdf_en_valid;
		}	

		/* -------------------------------------------------------------
		 * Next come final responses to non-INVITE methods :
		 * -------------------------------------------------------------
		 * BYE - sdf_cbk_uaCallTerminated or sdf_cbk_uaCancelCompleted
		 * CANCEL - sdf_cbk_uaCancelCompleted
		 * INFO/OPTIONS/SUBSCRIBE/NOTIFY/Unknown - 
		 * 						  sdf_cbk_uaTransactionCompleted
		 * COMET/PRACK (>= 200) - sdf_cbk_uaOverlapTransactionCompleted
		 * -------------------------------------------------------------
		 */
		else if (dMethodType == Sdf_en_bye)
		{
			if (dOldCallState == Sdf_en_byeSent)
			{
				if( (dStatusCode >= 200) && (dStatusCode < 300))
				{
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
						"Call : %s - Call Terminated; "
						"Issuing sdf_cbk_uaCallTerminated  to application",\
						(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, \
						Trace,0,pErr);
#endif
					/* 
					 * Issue the sdf_cbk_uaCallTerminated callback to the 
					 * application. 
					 */
					/* Set all the overlap transactions to Transaction cancelled
					 * state. */
					(void)sdf_fn_uaSetOverlapInfoCancelled(*ppOriginalCallObj, pErr);

					dCbkRetVal = sdf_cbk_uaCallTerminated(ppOriginalCallObj, \
										pEventContext, pErr);

#ifdef SDF_ERROR
					if( dCbkRetVal == Sdf_co_fail)
					{
						sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
							": Callback sdf_cbk_uaCallTerminated failed",pErr);
					}
#endif
				}
				else
				{
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
						"Call : %s - Call Terminated; "
						"Issuing sdf_cbk_uaByeRejected  to application",\
						(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, \
						Trace,0,pErr);
#endif
					/* 
					 * Issue the sdf_cbk_uaByeRejected callback to the 
					 * application. 
					 */
					dCbkRetVal = sdf_cbk_uaByeRejected(ppOriginalCallObj, \
										pEventContext, pErr);

#ifdef SDF_ERROR
					if( dCbkRetVal == Sdf_co_fail)
					{
						sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
							": Callback sdf_cbk_uaByeRejected failed",pErr);
					}
#endif
				}
			}
			/* Treate this  as Cancel only */
			else 
			{
				/* 
				 * 2xx to BYE in any other state maps to
				 * CancelCompleted callback.
				 * Set the dCallerCancelStatus flag to 
				 * Sdf_en_cancelCompleted 
				 */
				(*ppOriginalCallObj)->pCommonInfo->dCallerCancelStatus =  \
										Sdf_en_cancelComplete;
				
				/* 
				 * Issue the sdf_cbk_uaCancelCompleted callback to the 
				 * application. 
				 */
				dCbkRetVal = sdf_cbk_uaCancelCompleted(ppOriginalCallObj, \
					pEventContext, pErr);
#ifdef SDF_ERROR
				if( dCbkRetVal == Sdf_co_fail)
				{
					sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
						": Callback sdf_cbk_uaCancelCompleted failed",pErr);
				}
#endif
			}
		}
#ifdef SIP_RPR
		else if ((dMethodType == Sdf_en_prack) || \
				(dMethodType == Sdf_en_comet))
#else
		else if (dMethodType == Sdf_en_comet)
#endif /* SIP_RPR */
		{
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Final response to PRACK/COMET recd;"
				"Issuing sdf_cbk_uaOverlapTransactionCompleted  to "
				"application", \
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);

			sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_cbksTraces, \
				Trace,	0, pErr);
#endif
			/* 
			 * Issue the sdf_cbk_uaOverlapTransactionCompleted 
			 * callback to the application. 
			 */
			dCbkRetVal = sdf_cbk_uaOverlapTransactionCompleted(\
				ppOriginalCallObj, pOverlapTransInfo, pEventContext,\
				pErr);
#ifdef SDF_ERROR
			if( dCbkRetVal == Sdf_co_fail)
			{
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
					":sdf_cbk_uaOverlapTransactionCompleted failed",pErr);
			}
#endif

			/* Delete the Overlap Info from the CallObject.
			 * On UAS side after sending the response in SendCallToPeer
			 * this removal is done */
			if( pOverlapTransInfo != Sdf_co_null )
			{
				if(sdf_ivk_uaRemoveOverlapInfo(*ppOriginalCallObj, \
						pOverlapTransInfo) == Sdf_co_fail)
								return Sdf_en_valid;
			}
		}
		else if (dMethodType == Sdf_en_cancel)
		{
			/*  Set the dCallerCancelStatus flag to Sdf_en_cancelCompleted  */
			(*ppOriginalCallObj)->pCommonInfo->dCallerCancelStatus = \
										Sdf_en_cancelComplete;
		
			/* 
			 * Issue the sdf_cbk_uaCancelCompleted callback to the 
			 * application. 
			 */
			dCbkRetVal = sdf_cbk_uaCancelCompleted(ppOriginalCallObj, \
				pEventContext, pErr);
#ifdef SDF_ERROR
			if( dCbkRetVal == Sdf_co_fail)
			{
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
					": Callback sdf_cbk_uaCancelCompleted failed",pErr);
			}
#endif
		}
		else
		{
			/* 
			* Final response to INFO/OPTIONS/SUBSCRIBE/NOTIFY/Unknown 
		 	* request */

#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
				"Call : %s - Transaction Completed; "
				"Issuing sdf_cbk_uaTransactionCompleted to application",\
				(*ppOriginalCallObj)->pCommonInfo->pKey->pCallId);
				sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces, \
				Trace,0,pErr);
#endif
#ifdef SDF_SUBSCRIPTION_LAYER
		  /* SubscriptionLayer handler MUST be called BEFORE issuing the 
       * TransactionCompleted callback because we need to process all non-REFER
	     * SUBSCRIBE and NOTIFY responses in sdf_fn_uaSLHandler. If the message
	     * is not meant for the SubsLayer then it will be handled by
	     * TransactionCompleted callback.
       */

        /*
         * Currentcallstate is passed as parameter to fn
         * ismessageforsubslayer. 
         */
        if (sdf_fn_uaSLIsMessageForSubsLayer(&dMessageInfo, pOverlapTransInfo,
                                           &dSubsFlag, dCurrentCallState, pErr)\
                != Sdf_co_success)
        {
          /* If a mandatory header in SUBSCRIBE/NOTIFY is missing, thrown 
  			   * UnexpectedEvent callback so that Application can handle it 
				   * appropriately.
    			 */
          dIsErrorHandled = Sdf_co_true;
          dCbkRetVal      = sdf_cbk_uaSLUnexpectedEvent (ppOriginalCallObj, 
                                 Sdf_co_null, pOverlapTransInfo, pEventContext, 
                                 Sdf_en_uaSLErrorInMessage, pErr);
        }

        if (dSubsFlag == Sdf_co_true)
				  dCbkRetVal = sdf_fn_uaSLHandler (ppOriginalCallObj, pOverlapTransInfo,
                                           pEventContext,pErr);
				/* Above handler will set the dSubsFlag to true if it is			 
				 * handling the message*/
				if ((dSubsFlag == Sdf_co_false) && (dIsErrorHandled == Sdf_co_false))
				{
#endif
		   	/* 
			   * Issue the sdf_cbk_uaTransactionCompleted callback to the 
			   * application. 
			   */
			    dCbkRetVal = sdf_cbk_uaTransactionCompleted(ppOriginalCallObj, \
					  	pEventContext, pOverlapTransInfo, pErr);
#ifdef SDF_SUBSCRIPTION_LAYER
        }
#endif

#ifdef SDF_ERROR
			if( dCbkRetVal == Sdf_co_fail)
			{
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaUpdateCallDetails()"
					": Callback sdf_cbk_uaTransactionCompleted failed",pErr);
			}
#endif
			/* Delete the Overlap Info from the CallObject.
			 * On UAS side after sending the response in SendCallToPeer
			 * this removal is done */
			if( pOverlapTransInfo != Sdf_co_null )
			{
				if (sdf_ivk_uaRemoveOverlapInfo(*ppOriginalCallObj, \
									pOverlapTransInfo) == Sdf_co_fail)
								return Sdf_en_valid;
			}
		}

		/* 
		 * End of all transaction related response callbacks
		 */
#ifdef SDF_THREAD_SAFE
		if (sdf_fn_uaTryLock(pMutex, dCallingThread) == Sdf_co_success)
				(void)sdf_ivk_uaUnlockCallObject(*ppOriginalCallObj, pErr);
#endif

		if (dCbkRetVal == Sdf_co_fail)
		{
			pErr->errCode=Sdf_en_callBackError;
			return Sdf_en_failed;
		}

		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_ivk_uaUpdateCallDetails");
		return Sdf_en_valid;
	} /* End of block handling SIP Response messages */

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaUpdateCallDetails");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_en_valid;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaCallIdGenerator
**
** DESCRIPTION: Provides a sample Call-Id generator function. If the
**	application does not wish to write its own Call-Id generator, this
**	function can be used. It generates Call-Id's of the form
**		"RandomNum-RandomNum"
**
******************************************************************************/
Sdf_ty_s8bit *sdf_ivk_uaCallIdGenerator
#ifdef ANSI_PROTO
	(Sdf_st_appData *pAppData)
#else
	(pAppData)
	Sdf_st_appData *pAppData;
#endif
{
	Sdf_ty_s8bit dCallId[Sdf_co_smallBufferSize];
	Sdf_ty_s8bit *ptr = Sdf_co_null;
	Sdf_ty_u32bit dMicroSecs = 0;
	Sdf_ty_u32bit dRand = 0 ;

	/* To remove warnings */
	(void)pAppData;
	
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaCallIdGenerator");

	if( sdf_fn_uaGetCurrentTimeInMicroSecs(&dMicroSecs) == Sdf_co_fail)
		return Sdf_co_null;

	(void)sdf_fn_uaGenerateRand(&dRand);
	(void)sdf_fn_uaSprintf(dCallId, "%lu-%d", dMicroSecs, (Sdf_ty_u32bit)dRand);

	ptr = Sdf_mc_strdupCallHandler(dCallId);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCallIdGenerator");
	return ptr;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaChangeCallState
**
** DESCRIPTION: Changes the call state of the call object 
**	based on the current state and the incoming/outgoing message
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaChangeCallState
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject,
 Sdf_st_overlapTransInfo *pOverlapTransInfo,
 SipMessage *pSipMsg,
 Sdf_ty_msgMode dMode,
 Sdf_st_error *pErr)
#else
 (pObject, pOverlapTransInfo, pSipMsg, dMode, pErr)
 Sdf_st_callObject *pObject = Sdf_co_null;
 Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
 SipMessage *pSipMsg = Sdf_co_null;
 Sdf_ty_msgMode dMode;
  Sdf_st_error *pErr = Sdf_co_null;
#endif
{ 
	en_SipMessageType dType=SipMessageAny;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaChangeCallState");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaChangeCallState(): pErr parameter passed is Invalid");
#endif
		return Sdf_co_fail;
	}	

	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
			"pObject parameter passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	/* 
	 * In case a CANCEL/BYE has been received in the inviteReceived state, 
	 * it is not updated into the CallObject. Hence, the dType is set to 
	 * SipMessageRequest 
	 */
	if ((dMode == Sdf_en_msgModeCancelRecd) || (dMode == Sdf_en_msgModeByeRecd))
	{
		dType = SipMessageRequest;
	}
	else
	{
		/*
		 * Get the message type of the message
		 */
		if (sip_getMessageType(pSipMsg, &dType, \
			(SipError*)&(pErr->stkErrCode)) ==  SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
				"Getting type from SIP message failed",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}

	switch (dType)
	{
		case SipMessageRequest:
		{
			if (sdf_fn_uaChangeCallStateRequest(pObject, pOverlapTransInfo, \
				pSipMsg, dMode, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
					"Failed to change call state on SIP Request message",pErr);
#endif
				pErr->errCode=Sdf_en_callStateError;
				return Sdf_co_fail;
			}
			break;
		}
		case SipMessageResponse:
		{
			Sdf_ty_s8bit *pMethod;

			if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethod, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
					"Failed to get method from SIP message",pErr);
#endif
				return Sdf_co_fail;
			}

			/*
			 * Responses to CANCEL should not modify the state of the call
			 * object. The final response to the INVITE causes the 
			 * appropriate state transition.  And also if Response to BYE 
			 * comes when the call state is not in BYE sent stage then it means
			 * the BYE is sent to Cancel the INVITE.  So don't update
			 * Call State
			 */
			if ( (Sdf_mc_strcmp(pMethod, "CANCEL") == 0) || \
			     ((Sdf_mc_strcmp(pMethod, "BYE") == 0) && \
				  (pObject->pCallInfo->dState != Sdf_en_byeSent) &&
				  (pObject->pCallInfo->dState != Sdf_en_byeReceived)))
				break;

			if (sdf_fn_uaChangeCallStateResponse(pObject, pOverlapTransInfo, \
				pSipMsg, dMode, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callStateError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
					"Failed to change call state on SIP Response message",pErr);
#endif
				pErr->errCode=Sdf_en_callStateError;
				return Sdf_co_fail;
			}
			break;
		}
		default:
			break;
	}
#ifdef SDF_DEP
     if(sdf_fn_uaDepPostStateChange(pObject,pSipMsg,dMode,pErr) == Sdf_co_fail)
	 {
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaChangeCallState(): "
					"Failed to Post State change for Dep",pErr);
#endif	   	 
	 }
#endif
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaChangeCallState");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaChangeRequestURI
**
** DESCRIPTION: This invocation is used by the application to set the 
**				Request URI in common info. 
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaChangeRequestURI
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject,
 SipAddrSpec	*pRequestUri,
 Sdf_st_error *pErr)
#else
 (pObject, pRequestUri, pErr)
 Sdf_st_callObject *pObject = Sdf_co_null;
 SipAddrSpec *pRequestUri = Sdf_co_null;
 Sdf_st_error *pErr = Sdf_co_null;
#endif
{ 
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null; 

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaChangeRequestURI");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaChangeRequestURI(): pErr parameter passed is Invalid");
#endif
		return Sdf_co_fail;
	}	

	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaChangeRequestURI(): "
			"pObject parameter passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, \
		&pCommonInfo, pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaChangeRequestURI(): "
			"Getting CommonInfo from CallObject failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if(pCommonInfo->pRequestUri != Sdf_co_null)
		sip_freeSipAddrSpec(pCommonInfo->pRequestUri);

	pCommonInfo->pRequestUri = pRequestUri;
	HSS_LOCKEDINCREF(pCommonInfo->pRequestUri->dRefCount);

	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaChangeRequestURI");
	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************
** FUNCTION: sdf_ivk_uaGetMethodFromSipMessage
**
** DESCRIPTION: This function retrieves the method from the SIP Message.
**	In case of SIP request messages, the method in the Request-Line is
**	returned. In case of SIP response messages, the method in the Cseq
**	header is returned indicating the request for which the response
**	was generated.
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMethodFromSipMessage
#ifdef ANSI_PROTO
	(SipMessage *pMsg, 
	Sdf_ty_s8bit **ppMethod, 
	Sdf_st_error *pErr)
#else	
	(pMsg, pMethod, pErr)
	SipMessage *pMsg;
	Sdf_ty_s8bit **ppMethod;
	Sdf_st_error *pErr;
#endif	
{
	en_SipMessageType dType;
	Sdf_ty_s8bit *pTemp = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetMethodFromSipMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetMethodFromSipMessage(): pErr param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
			"pMsg param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	*ppMethod=Sdf_co_null;

	if (sip_getMessageType(pMsg, &dType, (SipError*)&(pErr->stkErrCode)) \
		==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
			"Getting type from SIP message failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if (dType == SipMessageRequest)
	{
		/* 
		 * Extract the method from the request line
		 */
		SipReqLine *pReqLine;
		if (sip_getReqLineFromSipReqMsg(pMsg,&pReqLine,(SipError*)\
			&(pErr->stkErrCode))  == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
				"Getting Request Line from SipMessage failed",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (sip_getMethodFromReqLine(pReqLine,&pTemp,(SipError*)\
			&(pErr->stkErrCode))  == SipFail)
		{
			sip_freeSipReqLine(pReqLine);	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
				"Getting Request Method from Sip Request Message failed",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		sip_freeSipReqLine(pReqLine);	
	}	
	else
	{
		/* 
		 * Extract the method from the CSeq line
		 */
		SipHeader header;
		if (sip_getHeader(pMsg, SipHdrTypeCseq, &header, (SipError*)\
			&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
				"Getting Cseq header from SipMessage failed",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (sip_getMethodFromCseqHdr(&header, &pTemp, (SipError*)\
			&(pErr->stkErrCode)) ==  SipFail)
		{
			sip_freeSipHeader(&header);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetMethodFromSipMessage(): "
				"Getting Method from Cseq header of SipMessage failed",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		sip_freeSipHeader(&header);
	}

	*ppMethod = pTemp;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetMethodFromSipMessage");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************
** FUNCTION: sdf_ivk_uaGetRespCodeFromSipMessage
**
** DESCRIPTION:  This function retrives the status code from the
** Sip message
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetRespCodeFromSipMessage
#ifdef ANSI_PROTO	
	(SipMessage *pMsg,
	Sdf_ty_u16bit *pRespCode,
	Sdf_st_error *pErr)
#else	
	(pMsg, pRespCode, pErr)
	SipMessage *pMsg;
	Sdf_ty_u16bit *pRespCode;
	Sdf_st_error *pErr;
#endif	
{
	SipStatusLine *pLine = Sdf_co_null;
	en_SipMessageType dType;
	Sdf_ty_u32bit  CodeNum;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaGetRespCodeFromSipMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetRespCodeFromSipMessage(): pErr param passed is "
			"invalid");
#endif
		return Sdf_co_fail;
	}

	if(pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRespCodeFromSipMessage(): "
			"pMsg param passed is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	
	if (sip_getMessageType(pMsg, &dType, (SipError*)&(pErr->stkErrCode)) \
		==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRespCodeFromSipMessage(): "
			"Getting type from SIP message failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if (dType == SipMessageRequest)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRespCodeFromSipMessage(): "
			"Cannot retrieve response code from a SIP request message",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}


	/*
	 * Extract the status code from the Status line
	 */
	if (sip_getStatusLineFromSipRespMsg(pMsg,&pLine,(SipError*)\
		&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRespCodeFromSipMessage(): "
			"Getting statusline from SipMessage failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
        if (sip_getStatusCodeNumFromStatusLine(pLine,&CodeNum,(SipError*)\
		&(pErr->stkErrCode))  == SipFail)
	{
		sip_freeSipStatusLine(pLine);	
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRespCodeFromSipMessage(): "
			"Getting Response Code from status line failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	*pRespCode = (Sdf_ty_u16bit)CodeNum;
	sip_freeSipStatusLine(pLine);	
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaGetRespCodeFromSipMessage");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************
** FUNCTION: sdf_ivk_uaGenerateCallid
**
** DESCRIPTION:  This function generates a unique callid based on
**	the Callid generator function set in the toolkit for the particular
**	user profile. The application data is passed as a parameter to the
**	function to generate call-id's on a per call basis.
**
******************************************************************/
Sdf_ty_s8bit* sdf_ivk_uaGenerateCallid
#ifdef ANSI_PROTO	
	(Sdf_st_initData *pInitData,
	Sdf_st_appData *pAppData)
#else	
	(pInitData, pAppData)
	Sdf_st_initData *pInitData;
	Sdf_st_appData *pAppData;
#endif	
{
	/*
	 * Return the call-id generated by the call id generator set in the
	 * user profile
	 */
	return pInitData->sdf_fp_uaCallIdGen(pAppData);
}


/*******************************************************************
** FUNCTION: sdf_ivk_uaHandleRequire
**
** DESCRIPTION:	Checks if the Request/Response Has a Require Header. If
**				the Require header is present, it checks with 
**				the options in the SUPPORTED field of the 
**				profile. The convention is that if the 
**				Supported Header is absent then a 420 Bad 
**				Extensions is to be sent (for requests)
**
** 				If Supported is present in the user profile,
**				then it's values are compared with the require 
**				values. Options that are not supported are returned 
** 				as unsupported in a 420 response(for requests). If the 
**				SUPPORTED attribute in the user profile has ' * ' 
**				as one of it's values then it is assumed that 
**				all the options are supported.
**				The API only prepares a 420 response and places
**				it in pCallObj if the above negotiation fails (for
**				requests). For responses which come with a Require
**				header, it only returns Sdf_en_invalid indicating
**				mismatch.
**
********************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaHandleRequire
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 Sdf_st_initData *pInitData,
	 Sdf_st_error *pErr)
#else
	(pCallObj, pInitData, pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_initData *pInitData;
	Sdf_st_error *pErr;
#endif
{
    Sdf_ty_transactionType 	transType;
    Sdf_st_transaction 		*pTransaction = Sdf_co_null;
	Sdf_st_validateInArg 	dValidateInArg;
	Sdf_st_validateOutArg	dValidateOutArg;
	Sdf_ty_messageValidity	dRetVal;


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering sdf_ivk_uaHandleRequire");

    /*First check whether it is a request or a response message*/
    if (pCallObj->pUacTransaction->pSipMsg == Sdf_co_null) 
		transType = Sdf_en_uasTransaction;
	else
		transType = Sdf_en_uacTransaction;

	if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj, transType,\
		&pTransaction,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaHandleRequire( ): "
			"Failed to get transaction object from call object", pErr);
#endif			
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_failed;
	}

	dValidateInArg.pCallObj = pCallObj;
	dValidateInArg.pTransaction = pTransaction;
	dValidateInArg.pOverlapTransaction = Sdf_co_null;
	dValidateInArg.dTransactionType = transType;
	dValidateInArg.pInitData = pInitData;
	
	dRetVal=sdf_ivk_uaHandleRequireFromMsg(&dValidateInArg, \
			&dValidateOutArg, pErr);
	/* Free the local reference */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);
	return dRetVal;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMethod
**
** DESCRIPTION: Checks if the method in the received SIP request message
**				is supported by the user profile (ALLOW attribute of
**				the user profile). If not, it forms a SIP 405 error 
**				response message and places it in the call object.
**
******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMethod
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObj, 
Sdf_st_initData *pInitData,
Sdf_st_error *pErr)
#else
(pObj,pInitData,pErr)
Sdf_st_callObject *pObj;
Sdf_st_initData *pInitData;
Sdf_st_error *pErr;
#endif
{
    Sdf_ty_transactionType 	transType;
    Sdf_st_transaction 		*pTransaction = Sdf_co_null;
	Sdf_st_validateInArg 	dValidateInArg;
	Sdf_st_validateOutArg	dValidateOutArg;
	Sdf_ty_messageValidity	dRetVal;
	
   	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG-Entering "
		"sdf_ivk_uaIsSupportedMethod");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaIsSupportedMethod(): pErr param passed is invalid");
#endif
		return Sdf_en_failed;
	}
	
	if (pObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethod(): "
			"pObj param is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}

	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethod(): "
			"pInitData param is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}
#endif

	/*
	 * If a response has come in, avoid processing and return
	 * Sdf_en_valid
	 */
	if (pObj->pUasTransaction->pSipMsg == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaIsSupportedMethod");
		return Sdf_en_valid;
	}
	
	transType = Sdf_en_uasTransaction;

	if (sdf_ivk_uaGetTransactionFromCallObject(pObj, transType,\
		&pTransaction,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaIsSupportedMethod(): "
			"Failed to get transaction object from call object", pErr);
#endif			
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_en_failed;
	}

	dValidateInArg.pCallObj = pObj;
	dValidateInArg.pTransaction = pTransaction;
	dValidateInArg.pOverlapTransaction = Sdf_co_null;
	dValidateInArg.dTransactionType = transType;
	dValidateInArg.pInitData = pInitData;
	
	dRetVal = sdf_ivk_uaIsSupportedMethodFromMsg(&dValidateInArg, \
			&dValidateOutArg, pErr);

	/* Free the local reference */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaIsSupportedMethod");

	return dRetVal;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMethodFromMsg
**
** DESCRIPTION: Checks if the method in the received SIP request message
**				is supported by the user profile (ALLOW attribute of
**				the user profile). If not, it forms a SIP 405 error 
**				response message and places it in the call object.
**
******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMethodFromMsg
#ifdef ANSI_PROTO
(Sdf_st_validateInArg *pValidateInArg,
Sdf_st_validateOutArg *pValidateOutArg,
Sdf_st_error *pErr)
#else
(pValidateInArg,pValidateOutArg,pErr)
Sdf_st_validateInArg *pValidateInArg;
Sdf_st_validateOutArg *pValidateOutArg;
Sdf_st_error *pErr;
#endif
{
	/*
	 * List of methods supported by the UA Toolkit.
	 */
#if 0
	Sdf_ty_messageType dSupported[Sdf_co_noOfMethodsSupported] = {
		Sdf_en_invite,
		Sdf_en_bye,
		Sdf_en_cancel,
		Sdf_en_ack,
		Sdf_en_info,
		Sdf_en_register,
#ifdef SIP_RPR
		Sdf_en_prack,
#endif /* SIP_RPR */
		Sdf_en_comet,
#ifdef SIP_IMPP
		Sdf_en_subscribe,
		Sdf_en_notify,
#endif
#ifdef SDF_IM
		Sdf_en_message,
#endif		
#ifdef SDF_REFER
		Sdf_en_refer,
#endif
		Sdf_en_options,
		Sdf_en_update
	};
#endif
	Sdf_ty_u32bit  j;
	Sdf_ty_s8bit *pMethod = Sdf_co_null;
	Sdf_ty_messageType dMethodType = Sdf_en_unknownRequest;
	Sdf_ty_u32bit dSize1;
	Sdf_ty_bool dIsAllowPresent=Sdf_co_false;
	Sdf_st_configExtraHeaders *pTemp = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_transactionType 	transType;
	Sdf_st_callObject *pObj=Sdf_co_null;
	Sdf_st_initData *pInitData=Sdf_co_null;
	SipMessage *pSipMsg=Sdf_co_null;
	en_SipMessageType dMsgType;
	SipError 	sipError;

   	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG-Entering "
		"sdf_ivk_uaIsSupportedMethodFromMsg");	

	transType = pValidateInArg->dTransactionType;
	pInitData = pValidateInArg->pInitData;
	pObj = pValidateInArg->pCallObj;
	(void)*pValidateOutArg;
	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaIsSupportedMethodFromMsg(): pErr param passed is invalid");
#endif
		return Sdf_en_failed;
	}
	
	if (pObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
			"pObj param is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}

	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
			"pInitData param is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}
#endif

	if ( (transType == Sdf_en_overlapTransaction) ||
		 (transType == Sdf_en_RegularUasTransaction) ||
		 (transType == Sdf_en_RegularUacTransaction))
		pSipMsg = pValidateInArg->pOverlapTransaction->pSipMsg;
	else
		pSipMsg = pValidateInArg->pTransaction->pSipMsg;
	
	if (pSipMsg == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaIsSupportedMethodFromMsg");
		return Sdf_en_valid;
	}

	/*
	 * If a response has come in, avoid processing and return
	 * Sdf_en_valid
	 */
	if(sip_getMessageType(pSipMsg, &dMsgType, &sipError) == SipFail)
	{			
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *) "sdf_ivk_uaIsSupportedMethodFromMsg( ):"
			"Failed to get type of Sip Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_failed;
	}			
	
	if( dMsgType != SipMessageRequest)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaIsSupportedMethodFromMsg");
		return Sdf_en_valid;
	}


	
	if(sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
		&pMethod,  pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
			"Failed to get method from SIP Message",pErr);
#endif
		return Sdf_en_failed;
	}
	
	if(sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType,\
							pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
			"Failed to get enum value for a method",pErr);
#endif
		return Sdf_en_failed;
	}			

	/* Check if the method is one of INVITE, ACK or OPTIONS. Every
	 * user agent MUST support this. Even if the user profile 
	 * does not explicitly express support for these methods,
	 * These methods are treated as vaild and supported.
	 */

	if ( (dMethodType == Sdf_en_invite)|| 	\
		(dMethodType == Sdf_en_ack)    || 	\
		(dMethodType == Sdf_en_cancel) || 	\
		(dMethodType == Sdf_en_bye)    || 	\
		(dMethodType == Sdf_en_options) )
	{
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_ivk_uaIsSupportedMethod");	
		pErr->errCode = Sdf_en_noUaError;
		return Sdf_en_valid;
	}

	if(sdf_listSizeOf(&(pInitData->slExtraHeadersList), &dSize1, pErr) \
					== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
			"Failed to get size of list",pErr);
#endif
		return Sdf_en_failed;
	}			

	/*
	 * Iterate through the slExtraHeadersList of the user profile and
	 * retrieve the ALLOW list set in it.
	 */
	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTemp = (Sdf_st_configExtraHeaders*)\
					(dListIterator.pCurrentElement->pData);
		if(Sdf_mc_strcasecmp("ALLOW",pTemp->pParam->pName)==0)
		{
			dIsAllowPresent = Sdf_co_true;
			break;
		}

		(void)sdf_listNext(&dListIterator, pErr);
	}
	/*
	 * If there are no elements in the slExtraHeadersList of the user
	 * profile, or if an ALLOW attribute is anyway missing, send a 405
	 * response
	 */
	if (dIsAllowPresent == Sdf_co_false)
	{
        if(dMethodType==Sdf_en_unknownRequest)	
		{
			/* 
			 * Display incoming message in call flow (ctascii) 
			 */
			sdf_fn_uaUpdateCallFlow (pSipMsg, \
				Sdf_co_true);

			if (sdf_fn_uaRejectRequest(pObj,\
					pInitData,501,&pSipMsg,transType,pValidateInArg->\
					pOverlapTransaction, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
					"Failed to Form 501 Response Message",pErr);
#endif
				return Sdf_en_failed;
			}
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_notImplemented, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
				"Unknown Method received",pErr);
#endif
			if ( (transType == Sdf_en_overlapTransaction) ||
				 (transType == Sdf_en_RegularUasTransaction) ||
				 (transType == Sdf_en_RegularUacTransaction))
				pValidateInArg->pOverlapTransaction->pSipMsg = pSipMsg;
			else
				pValidateInArg->pTransaction->pSipMsg = pSipMsg;
            pErr->errCode = Sdf_en_notImplemented;
			return Sdf_en_invalid;
		}

		else
		{
			/*
			 * The method is supported.
			 */
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_ivk_uaIsSupportedMethodFromMsg");	
			return Sdf_en_valid;
		}
	}
	else
	{
		/*
		 * pTemp now points to the ALLOW attribute of the user profile.
		 * Check against the list of methods present there.
		 */
		Sdf_ty_u32bit size;
		Sdf_ty_s8bit *pTempVal;

        (void)sip_listSizeOf(&(pTemp->pParam->slValue), &size, \
			(SipError*)&(pErr->stkErrCode));
		for(j=0;j<size;j++)
		{
			if(sip_listGetAt(&(pTemp->pParam->slValue),  \
				(Sdf_ty_u32bit) j,(Sdf_ty_pvoid *)&pTempVal,  \
				(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_extraHeaderAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
					"Failed to get value from ALLOW list",pErr);
#endif
				pErr->errCode=Sdf_en_extraHeaderAccessError;
				return Sdf_en_failed;
			}
			/*
			 * Check if the method of the request is supported by
			 * the user profile.
			 */
			if(Sdf_mc_strcmp(pMethod,pTempVal)==0)
				break;
		}
		if(Sdf_mc_strcmp(pMethod,pTempVal)!=0)
        {
            sdf_fn_uaUpdateCallFlow (pSipMsg, \
                    Sdf_co_true);
            if(dMethodType==Sdf_en_unknownRequest)
            {
                if (sdf_fn_uaRejectRequest(pObj,\
                            pInitData,501,&pSipMsg,transType,pValidateInArg->\
                            pOverlapTransaction, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                            (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
                            "Failed to Form 501 Response Message",pErr);
#endif
                    return Sdf_en_failed;
                }
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors, \
                        Sdf_en_notImplemented, \
                        (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
                        "UnknownMethod  received",pErr);
#endif
                if ( (transType == Sdf_en_overlapTransaction) ||
                        (transType == Sdf_en_RegularUasTransaction) ||
                        (transType == Sdf_en_RegularUacTransaction))
                    pValidateInArg->pOverlapTransaction->pSipMsg = pSipMsg;
                else
                    pValidateInArg->pTransaction->pSipMsg = pSipMsg;

                pErr->errCode = Sdf_en_notImplemented;
                return Sdf_en_invalid;	
            }

            else
            {
                /*
                 * The method is understood but not present in ALLOW list so
                 * form 405 Method not Allowed here.
                 */

                if (sdf_fn_uaRejectRequest(pObj,\
                            pInitData,405,&pSipMsg,transType,pValidateInArg->\
                            pOverlapTransaction, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
                    sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
                            (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
                            "Failed to Form 405 Response Message",pErr);
#endif
                    return Sdf_en_failed;
                }
                pErr->errCode= Sdf_en_unknownMethodError;
#ifdef SDF_ERROR
                sdf_fn_setError(Sdf_en_majorErrors, \
                        Sdf_en_unknownMethodError, \
                        (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMethodFromMsg(): "
                        "Method not ALLOWED received",pErr);
#endif
                if ( (transType == Sdf_en_overlapTransaction) ||
                        (transType == Sdf_en_RegularUasTransaction) ||
                        (transType == Sdf_en_RegularUacTransaction))
                    pValidateInArg->pOverlapTransaction->pSipMsg = pSipMsg;
                else
                    pValidateInArg->pTransaction->pSipMsg = pSipMsg;

                return Sdf_en_invalid;

            }

        }
	}
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaIsSupportedMethodFromMsg");

	return Sdf_en_valid;
}

/****************************************************************************
** FUNCTION: sdf_ivk_uaValidateUri
**
** DESCRIPTION: This function takes in the Sip Message
** 				from the call object and checks if the scheme 
**				in the Request-URI is sip: and the version in use
**				is SIP/2.0. It also checks if the address in the
**				Request-URI match that set in the user profile 
**				(passed as a parameter to the function). In case any
**				of the above don't match, it forms an appropriate
**				response message and places it in pTempCallObj.
**
*****************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaValidateUri
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pTempCallObj,
	Sdf_st_initData *pInitData,
	Sdf_ty_u16bit	*pErrorResponseCode,
	Sdf_st_error *pErr)
#else
	(pTempCallObj, pInitData, pErrorResponseCode, pErr)
	Sdf_st_callObject *pTempCallObj;
	Sdf_st_initData *pInitData;
	Sdf_ty_u16bit	*pErrorResponseCode;
	Sdf_st_error *pErr;
#endif
{
	SipReqLine *pRequest = Sdf_co_null;
	SipAddrSpec *pReqUri;
	SipHeader *pHdr = Sdf_co_null;
	Sdf_ty_s8bit *pVersion = Sdf_co_null;
	en_SipMessageType dReqRespType;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_bool		dIsTelUri = Sdf_co_false;
	Sdf_ty_bool		dIsImUri  = Sdf_co_false;
	Sdf_ty_bool		dIsPresUri  = Sdf_co_false;
	Sdf_ty_bool		dIsMatchOccured = Sdf_co_false;


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaValidateUri");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
		return Sdf_en_failed;

	if (pTempCallObj == Sdf_co_null)
		return Sdf_en_failed;
		
	if (pInitData == Sdf_co_null)
		return Sdf_en_failed;
		
	if (pErrorResponseCode == Sdf_co_null)
		return Sdf_en_failed;
#endif

	if (pTempCallObj->pUasTransaction->pSipMsg == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaValidateUri");
		return Sdf_en_valid;
	}

	if (sip_getMessageType(pTempCallObj->pUasTransaction->pSipMsg, \
		&dReqRespType, (SipError*)&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *) "sdf_ivk_uaValidateUri( ):"
			"Failed to get type of Sip Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_failed;
	}

	if (dReqRespType == SipMessageRequest)
	{
#if 0
		SipHeader dTempHeader;
		Sdf_ty_s8bit *pRemoteIp, *pSentBy;
		Sdf_ty_s8bit *pHostAddress, *pHostPort=Sdf_co_null;
#ifdef SDF_LINT        
		Sdf_ty_s8bit dTempSentby[Sdf_co_characterBufferSize] = "";
#else
		Sdf_ty_s8bit dTempSentby[Sdf_co_characterBufferSize];
#endif        
		Sdf_st_transportInfo *pTransport;

		/* 
		 * If the topmost Via header of the incoming request has the IP/port 
		 * set in this profile, the message should be discarded. Else, this 
		 * user agent would end up sending a response to itself.
		 */
		if (sip_getHeaderAtIndex(pTempCallObj->pUasTransaction->pSipMsg, \
			SipHdrTypeVia, &dTempHeader, 0, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri(): "
				"Error Accessing Via Header From Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}

		if(sip_getViaParamCountFromViaHdr(&dTempHeader, &dViaParamCount, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
				return Sdf_en_failed;

		for (dIndex = 0; dIndex < dViaParamCount; dIndex++)
		{
			if(sip_getViaParamAtIndexFromViaHdr(&dTempHeader, &pParam, \
				dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail)
					return Sdf_en_failed;

			if(sip_getNameFromSipParam(pParam, &pName, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
					return Sdf_en_failed;

			if (Sdf_mc_strcmp(pName, "maddr") == 0)
			{
				if(sip_getValueAtIndexFromSipParam(pParam, &pValue, \
					0, (SipError *)&(pErr->stkErrCode)) == SipFail)
						return Sdf_en_failed;
				
				Sdf_mc_strcpy(dTempSentby, pValue);	
				sip_freeSipParam(pParam);
				break;
			}
			/* Free the local reference */
			sip_freeSipParam(pParam);
		}

		if(pValue == Sdf_co_null)
		{
		/* SS */

			if (sip_getSentByFromViaHdr(&dTempHeader, &pSentBy, (SipError*)\
						&(pErr->stkErrCode)) == SipFail)
			{		
				sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri(): "
						"Error accessing SentBy from Via Header in Sip Message",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}	

			Sdf_mc_strcpy(dTempSentby, pSentBy);	
		}

		/* If "[" is present in the address then it will be IPV6 address
		 * Otherwise it is IPv4 address as mentioned in the RFC-3261
		 */
		if(Sdf_mc_strstr(dTempSentby,"]") !=Sdf_co_null)
		{
			/* Strip the '[',']' before resolving the address. */
			pHostPort = Sdf_mc_strstr(dTempSentby, "]:");
			pHostAddress = dTempSentby+1;
			if (pHostPort != Sdf_co_null)
			{
			    pHostPort = pHostPort + 2; /* Get rid of "]:" */
			    pHostAddress[Sdf_mc_strlen(dTempSentby)-Sdf_mc_strlen(pHostPort)-3] = '\0';
			}
			else
			{
				pHostAddress[Sdf_mc_strlen(dTempSentby)-2] = '\0';
			}
		}
		else
		{
			pHostAddress = Sdf_mc_strtokr(dTempSentby, ":",&pHostPort);	
		} /* Endif of if ipv6 */

		if ((sdf_fn_uaGetHostIp(pHostAddress, &pRemoteIp, pErr)) ==\
				Sdf_co_fail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri(): "
				"Failed to resolve IP address of the host in Via header",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_en_failed;
		}	

		if (sdf_ivk_uaGetTransportInfoFromInitData(pInitData, \
			&pTransport, pErr) == Sdf_co_fail)
		{
			sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri(): "
				"Getting TransportInfo from InitData failed",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_en_failed;
		}

		/* Free the local reference */
		(void)sdf_ivk_uaFreeTransportInfo(pTransport);

		sip_freeSipHeader(&dTempHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&(pRemoteIp),pErr);
#endif
		/* 
		 * Now validate the Request URI
		 */
		if(sip_getReqLineFromSipReqMsg(pTempCallObj->pUasTransaction->pSipMsg, \
			&pRequest, (SipError *)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaValidateUri( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}

		/*
		 * Check if the version is SIP/2.0. Else form a 505 Version Not
		 * Implemented
		 */
		if (sip_getVersionFromReqLine(pRequest, &pVersion, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaValidateUri( ):"
				"Failed to get SIP Version from Request Line",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipReqLine(pRequest);
			return Sdf_en_failed;
		}

		if (Sdf_mc_strcasecmp(pVersion, "SIP/2.0") != 0)
		{
			/*
			 * Version is not SIP/2.0. Have to send a 505 
			 * "Version Not Implemented"
			 */
			sip_freeSipReqLine(pRequest);
			*pErrorResponseCode = 505;

#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri( ): "
				"SIP Version not supported",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_en_invalid;
		}
		
		/* Now check the URL scheme */	
		
/*w534*/(void)sip_getAddrSpecFromReqLine(pRequest,&pReqUri,\
				(SipError*)&(pErr->stkErrCode));
#ifdef SDF_TLS
		if((pReqUri->dType != SipAddrSipUri) &&
		   (pReqUri->dType != SipAddrSipSUri))
#else
		if(pReqUri->dType != SipAddrSipUri)
#endif
		{
#ifdef SDF_TEL
			if(sip_isTelUrl(pReqUri,\
						(SipError*)&(pErr->stkErrCode)) == SipSuccess)
				dIsTelUri = Sdf_co_true;
#endif
#ifdef SDF_IM
			if( dIsTelUri == Sdf_co_false )
			{
				if(sip_isImUrl(pReqUri,\
						(SipError*)&(pErr->stkErrCode)) == SipSuccess)
				dIsImUri = Sdf_co_true;
			}
#endif
#ifdef SDF_PRES
			if( (dIsTelUri == Sdf_co_false)&&(dIsImUri == Sdf_co_false))
			{
				if(sip_isPresUrl(pReqUri,\
						(SipError*)&(pErr->stkErrCode)) == SipSuccess)
				dIsPresUri = Sdf_co_true;
			}
#endif
			if((dIsTelUri != Sdf_co_true) && (dIsImUri != Sdf_co_true)
              &&(dIsPresUri != Sdf_co_true))	
			{
                        	sip_freeSipAddrSpec(pReqUri);
				sip_freeSipReqLine(pRequest);
				*pErrorResponseCode = 416;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri(): "
				"Url Scheme not supported",pErr);
#endif
				sip_freeSipHeader(pHdr);
				pErr->errCode=Sdf_en_invalidParamError;
				return Sdf_en_invalid;
			}
		}

#ifdef SDF_TLS
		/*
		 * If the Request line contains SIPS url and a transport parameter, 
		 * then its value should be some reliable (TCP) transport only. 
		 * This validation will be done in the following block.
		 */
		if (pReqUri->dType == SipAddrSipSUri)
		{
			SipParam *pLocalParam = Sdf_co_null;
			SipUrl	*pSipUrl = Sdf_co_null;
			Sdf_ty_s8bit *pLocalName = Sdf_co_null, *pLocalValue = Sdf_co_null;
			Sdf_ty_u32bit dLocalIndex = 0, dUrlParamListSize = 0;
				
			if (sip_getSUrlFromAddrSpec(pReqUri, \
				&pSipUrl, (SipError*)&(pErr->stkErrCode)) ==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *) "sdf_ivk_uaValidateUri( ):"
					"Failed to get URL from Request Line",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
                         	sip_freeSipAddrSpec(pReqUri);
				sip_freeSipReqLine(pRequest);
				return Sdf_en_failed;
			}

/*w534*/(void)sip_getUrlParamCountFromUrl(pSipUrl, &dUrlParamListSize, \
				(SipError *)&(pErr->stkErrCode));

			for (dLocalIndex = 0; dLocalIndex < dUrlParamListSize; dLocalIndex++)
			{
/*w534*/(void)sip_getUrlParamAtIndexFromUrl(pSipUrl, &pLocalParam, \
					dLocalIndex, (SipError *)&(pErr->stkErrCode));

/*w534*/(void)sip_getNameFromSipParam(pLocalParam, &pLocalName, \
					(SipError *)&(pErr->stkErrCode));

				if (Sdf_mc_strcmp(pLocalName, "transport") == 0)
				{
/*w534*/(void)sip_getValueAtIndexFromSipParam(pLocalParam, &pLocalValue, \
					    0, (SipError *)&(pErr->stkErrCode));

					if (Sdf_mc_strcasecmp(pLocalValue, "UDP") == 0)
					{
						*pErrorResponseCode = 400;
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_invalidSipSUrlParamError, (Sdf_ty_s8bit *)\
							"sdf_ivk_uaValidateUri( ): "
						    "Invalid transport parameter in Req URI.",pErr);
#endif
						sip_freeSipParam(pLocalParam);
						sip_freeSipUrl(pSipUrl);
            		                	sip_freeSipAddrSpec(pReqUri);
				                sip_freeSipReqLine(pRequest);
				 		pErr->errCode=Sdf_en_invalidSipSUrlParamError;
						return Sdf_en_invalid;
					}
					else
					{
			                        sip_freeSipParam(pLocalParam);
						break;
					}
				}
				/* Free the local reference */
			    sip_freeSipParam(pLocalParam);
			}
			/* Free the local reference */
			sip_freeSipUrl(pSipUrl);
		}
#endif /* SDF_TLS */

		/* 
		 * Check if the host and user name in the Request-URI of 
		 * the sip message are the same as the profile. For this,
		 * the URL in the Request-URI is compared against the From
		 * address in the user profile and the complete list of Contacts
		 * till a match is found.
		 */
		if (sdf_fn_uaCompareSipAddrSpec(pReqUri, \
			((SipFromHeader *)(pInitData->pFrom->pHeader))->pAddrSpec, \
			Sdf_co_false, Sdf_co_true, pErr) != 0)
		{
#ifdef SDF_TEL			
			/*Comparison may have failed if the profile had a tel url and the
			 * incoming Message had the tel url embedded in the user part of
			 * the sip url. Extract the user part and form a new Addrspec and
			 * call sdf_fn_uaCompareSipAddrSpec() again.
			 */
			if( dIsTelUri == Sdf_co_true )	
			{
				if (sdf_fn_uaCompareSipTelAddrSpec(pReqUri, \
					((SipFromHeader *)(pInitData->pFrom->pHeader))->pAddrSpec, \
					pErr) == 0)
					dIsMatchOccured = Sdf_co_true;
			}
#endif
			if ( dIsMatchOccured != Sdf_co_true )
			{
				/* 
				 * Normal comparision failed. Try comparision against each 
				 * Contact in turn. This is because, an intermediate proxy
				 * might have rewritten the Request-URI after a registrar
				 * lookup. In this case, the registered contact of the user
				 * will be present in the Request-URI, and so we must be
				 * prepared to receive such requests.
				 */
			
				Sdf_ty_u32bit dContactListSize = 0;
				Sdf_ty_bool dRequestUriValid = Sdf_co_false;
			
				if(sdf_listSizeOf(&(pInitData->slContact), &dContactListSize,pErr) \
								== Sdf_co_fail)
						return Sdf_en_failed;

				(void)sdf_listInitIterator(&(pInitData->slContact), \
					&dListIterator, pErr);

				while (dListIterator.pCurrentElement != Sdf_co_null)
				{
					SipHeader *pContact = Sdf_co_null;

					pContact = (SipHeader *) \
							(dListIterator.pCurrentElement->pData);

					if (sdf_fn_uaCompareSipAddrSpec(pReqUri, \
						((SipContactHeader *)(pContact->pHeader))->pAddrSpec, \
						Sdf_co_false, Sdf_co_true, pErr) == 0)
					{
						dRequestUriValid = Sdf_co_true;
						break;
					}
#ifdef SDF_TEL
					else
					{
						
						if( dIsTelUri == Sdf_co_true )	
						{
							if (sdf_fn_uaCompareSipTelAddrSpec(\
								pReqUri,\
								((SipContactHeader *)(pContact->pHeader))->\
								pAddrSpec,pErr) == 0)
							dIsMatchOccured = Sdf_co_true;
						}
						/*Same case for Contact. The Tel Url may be embedded
						 * in the sip url.
						 */
						if ( dIsMatchOccured == Sdf_co_true )
						{
							 dRequestUriValid = Sdf_co_true;
					         break;
						}
					}
#endif
					(void)sdf_listNext(&dListIterator, pErr);
				}

				if (dRequestUriValid == Sdf_co_false)
				{
					/*
					 * Set the error code to be returned for the response SIP
					 * message. This will be used in invocation to
					 * sdf_ivk_uaRejectCall
					 */
					*pErrorResponseCode = 404;
					sip_freeSipAddrSpec(pReqUri);
					sip_freeSipReqLine(pRequest);
					pErr->errCode = Sdf_en_userNotFoundError;
					return Sdf_en_invalid;
				}
			}
		}

#ifdef SDF_TLS
		/*
		 * If the request line contains a SIPS Uri then the SIP message 
		 * should have come over TLS. If not, then return the error.
		 */
		if (pReqUri->dType == SipAddrSipSUri)
		{
                   if(pTempCallObj->pUasTransaction->pDestInfo->dScheme != \
                                              Sdf_en_protoTls)
                   {
                        	sip_freeSipAddrSpec(pReqUri);
				sip_freeSipReqLine(pRequest);
				*pErrorResponseCode = 400;

#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transportSchemeMismatch,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaValidateUri( ): "
					"Mismatch between the URI scheme in Request line and "\
					"the transport protocol used." ,pErr);
#endif
				pErr->errCode=Sdf_en_transportSchemeMismatch;

				return Sdf_en_invalid;
			}
		}
#endif /* SDF_TLS */

		/* Free the local reference */
		sip_freeSipAddrSpec(pReqUri);
		sip_freeSipReqLine(pRequest);
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaValidateUri");
	return Sdf_en_valid;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaPurgeExpiredRemoteRetransEntries
**
** DESCRIPTION: This function removes entries from the remote
**				retransmission list that have been in existence for 
**				more than the configured expiry time.
**				If UATK is compiled with SDF_APP_CONTROLLED_REMOTE_RETX or if
**				SDF_USE_INTERNALTHREADS flag is not set, application has to 
**				invoke this function to purge the remote retransmission entries.
**				
**				If UATK is compiled with SDF_APP_CONTROLLED_REMOTE_RETX may 
**				choose to implement it's own purging logic. In that case, it 
**				need not use this function. But if it invokes this function,
**				then the pData parameter must indidate which remote retrans
**				hash table to purge.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaPurgeExpiredRemoteRetransEntries
#ifdef ANSI_PROTO
(Sdf_ty_timerType dTimerType,\
	Sdf_ty_pvoid pData,
	Sdf_st_error *pErr)
#else
(dTimerType,pData,pErr)
Sdf_ty_timerType dTimerType;
Sdf_ty_pvoid pData;
Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dCurrentTime = 0;
	Sdf_st_hash	*pRemoteRetransHash = Sdf_co_null;
	Sdf_st_remoteRetransBuffer *pRemoteRetransBuffer = Sdf_co_null;
	Sdf_st_hashIterator dIterator;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_traceBufferSize];
#endif	

	(void)pData;
	(void)pErr;
	(void)dTimerType;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaPurgeExpiredRemoteRetransEntries");
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks.
	 */
#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	if(pData == Sdf_co_null)
	{
		return Sdf_co_fail;
	}
#endif
#endif

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	pRemoteRetransHash = (Sdf_st_hash *) pData;
#else
	pRemoteRetransHash = &(pGlbToolkitData->dRemoteRetransHash);
#endif

	/* 
	 * Get the current time
	 */
	(void)sdf_fn_uaGetCurrentTimeInSecs(&dCurrentTime);

	/* 
	 * Go through the hash table looking for expired entries
	 */
	sdf_ivk_uaHashInitIterator(pRemoteRetransHash, &dIterator);

	while(dIterator.pCurrentElement != Sdf_co_null)
	{
		pRemoteRetransBuffer = (Sdf_st_remoteRetransBuffer *)\
			(dIterator.pCurrentElement->pElement);

		if ((dCurrentTime - pRemoteRetransBuffer->dTimeStamp) \
			> Sdf_mc_remoteRetransExpiryTime)
		{
#ifdef SDF_TRACE
			if (pRemoteRetransBuffer != Sdf_co_null)
			{
				if (pRemoteRetransBuffer->pKey != Sdf_co_null)
				{
					(void)sdf_fn_uaSnprintf(trace,Sdf_co_traceBufferSize,\
						"Call: %s - Deleted an expired "
						"entry from Remote Retransmission List.\n", \
						pRemoteRetransBuffer->pKey->pCallId);

					sdf_fn_trace(Sdf_en_allTraceLevels, \
						Sdf_en_remoteRetransTraces, trace, 0, pErr);
				}
			}
#endif
			/* 
			 * Remove the expired entry 
			 */
#ifdef SDF_LINT
            if(pRemoteRetransBuffer == Sdf_co_null)
                return Sdf_co_fail;
#endif                
			if(sdf_ivk_uaHashRemove(pRemoteRetransHash, \
				(Sdf_ty_pvoid) pRemoteRetransBuffer->pKey) == Sdf_co_fail)
						 return Sdf_co_fail;	
		}
		/* Get the next element. */
		sdf_ivk_uaHashNext(pRemoteRetransHash, &dIterator);
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaPurgeExpiredRemoteRetransEntries");

#ifndef SDF_USE_INTERNALTHREADS
	/*
	 * If the SDF_USE_INTERNALTHREADS has not been set, issue the
	 * sdf_cbk_uaStartTimer callback so that the application can start
	 * a timer for the next purge operation. Note that this callback has to
	 * be issued only if SDF_APP_CONTROLLED_REMOTE_RETX has not been set. In
	 * that case application is expected to purge the remote retrans entries 
	 * using its own logic.
	 */

#ifndef SDF_APP_CONTROLLED_REMOTE_RETX
#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,(Sdf_ty_s8bit*) \
		"Issuing callback sdf_cbk_uaStartTimer for purging expired remote"
		"remote retransmission entries.", 0, pErr);
#endif
/*w534*/(void)sdf_cbk_uaStartTimer(Sdf_mc_remoteRetransSpoolTime*1000, \
		Sdf_en_remoteRetransTimer, Sdf_co_null, Sdf_co_null, \
		sdf_ivk_uaPurgeExpiredRemoteRetransEntries, Sdf_co_null, pErr);
#endif
#endif

	return Sdf_co_success;
}


/***********************************************************************
** FUNCTION: sdf_ivk_uaCompareCallObjectKeys
**
** DESCRIPTION: This function compares two call object keys
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCompareCallObjectKeys
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pNewCallObject, 
	Sdf_st_callObject *pExistingCallObject, 
	SipMessage *pMsg,
	Sdf_ty_callLegMatch *pCallLegMatch,
	Sdf_st_error *pErr)
#else
	(pNewCallObject, pExistingCallObject, pMsg, pCallLegMatch, pErr)
	Sdf_st_callObject *pNewCallObject;
	Sdf_st_callObject *pExistingCallObject;
	SipMessage *pMsg;
	Sdf_ty_callLegMatch *pCallLegMatch;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_commonInfo *pNewCommonInfo = Sdf_co_null; 
	Sdf_st_commonInfo *pExistingCommonInfo = Sdf_co_null;
	Sdf_ty_bool		  dToTagLenient = Sdf_co_false;
#ifdef SDF_LINT
Sdf_st_msgInfo	  dMessageInfo = {Sdf_co_null,SipMessageAny, \
                                    Sdf_en_RegularTxn, 0, 0};

#else    
	Sdf_st_msgInfo	  dMessageInfo;
#endif    

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCompareCallObjectKeys");

#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if(pErr == Sdf_co_null)
	{
/*Fix from UATK 2.1 Start*/
#ifdef SDF_ERROR
		
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaCompareCallObjectKeys(): Invalid Error passed ");
#endif
/*Fix from UATK 2.1 Start*/
		
		return Sdf_co_fail;
	}
	
	if((pNewCallObject == Sdf_co_null) || (pExistingCallObject == Sdf_co_null))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareCallObjectKeys(): "
			"Invalid Call Object passed", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pCallLegMatch == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaCompareCallObjectKeys(): "
			"Invalid callleg matching argument passed", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */
	

	 if (sdf_ivk_uaGetCommonInfoFromCallObject(pNewCallObject, &pNewCommonInfo,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCompareCallObjectKeys(): "
			"Getting CommonInfo from CallObject failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_ivk_uaGetCommonInfoFromCallObject(pExistingCallObject, \
		&pExistingCommonInfo, pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pNewCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCompareCallObjectKeys(): "
			"Getting CommonInfo from CallObject failed",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * Check if the Call-Id's match
	 */
	if (Sdf_mc_strcmp(pNewCommonInfo->pKey->pCallId, \
		pExistingCommonInfo->pKey->pCallId) != 0)
	{
		/* 
		 * Call-id's do not match
		 */
		*pCallLegMatch = Sdf_en_notMatching;

		(void)sdf_ivk_uaFreeCommonInfo(pNewCommonInfo);
		(void)sdf_ivk_uaFreeCommonInfo(pExistingCommonInfo);

		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaCompareCallObjectKeys");

		pErr->errCode = Sdf_en_noUaError;

		return Sdf_co_success;
	}

	if( pMsg != Sdf_co_null )
	{
		if (sdf_fn_uaGetMessageInfo(pMsg, &dMessageInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCompareCallObjectKeys(): "
				"Failed to get Message Info from SIP message",pErr);
#endif
			pErr->errCode = Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeCommonInfo(pNewCommonInfo);
			(void)sdf_ivk_uaFreeCommonInfo(pExistingCommonInfo);
			return Sdf_co_fail;
		}
		/* For Response To Cancel and Non 2xx Response to Invite
			 * comparision should be lenient against the To Tags */
		
		/* Fix for CSR 1-1316815
		 * The To-tag leniency should be present for CANCEL request,
		 * response for CANCEL, and failure responses for INVITE.
		 */
		if(dMessageInfo.dMsgType == Sdf_en_cancel)
			dToTagLenient = Sdf_co_true;
		else if ((dMessageInfo.dReqRespType == SipMessageResponse) && \
				 (dMessageInfo.dMsgType == Sdf_en_invite) && \
				 (dMessageInfo.dResponse > 299))
			dToTagLenient = Sdf_co_true;
#ifdef SDF_SUBSCRIPTION_LAYER
    /* If the forking flag in enabled (i.e. Sdf_en_forkedSubscribe) and it's a
     * 1XX/2XX response to SUBSCRIBE, then set the dToTagLenient flag. Also,
     * reset pCallLegMatch since it is used only for detecting forked NOTIFYs.
     */
		else if ((*pCallLegMatch            == Sdf_en_forkedSubscribe) && \
             (dMessageInfo.dReqRespType == SipMessageResponse)     && \
             (dMessageInfo.dMsgType     == Sdf_en_subscribe)       && \
             (dMessageInfo.dResponse    >= 100)                    && \
             (dMessageInfo.dResponse    <  300))
     {
       dToTagLenient  = Sdf_co_true;
       *pCallLegMatch = Sdf_en_notMatching;
     }
#endif
	}

(void)sdf_fn_uaCompareSipHeaders(pNewCommonInfo->pKey, \
			pExistingCommonInfo->pKey, pCallLegMatch,dToTagLenient,pErr);
	/*
	 * If the SipMessage is not NULL, use the type of the SipMessage
	 * (SipMessageRequest/SipMessageResponse) and the dCaller variable
	 * in the existing call object, to decide whether to do
	 * (From:From,To:To) OR (From:To, To:From) [exact match]
	 */
	if ((pMsg != Sdf_co_null) && (*pCallLegMatch != Sdf_en_notMatching))
	{
		/*
		 * If the comparision returned Sdf_en_overlapDialing, we check
		 * whether this is an INVITE. If it is not an INVITE, we return
		 * Sdf_en_notMatching. Similarly, if extra digits were added in
		 * a response, we return Sdf_en_notMatching.
		 */
		if (*pCallLegMatch == Sdf_en_overlapDialing)
		{
			if (((dMessageInfo.dReqRespType == SipMessageRequest) && \
				(dMessageInfo.dMsgType != Sdf_en_invite)) || \
				(dMessageInfo.dReqRespType == SipMessageResponse))
				*pCallLegMatch = Sdf_en_notMatching;
		}
		
		if( *pCallLegMatch != Sdf_en_notMatching )
		{
			/* If request is CANCEL then compare the via branch keys
			 * also to have exact match of call object */
			if ((dMessageInfo.dReqRespType == SipMessageRequest) && \
				(dMessageInfo.dMsgType == Sdf_en_cancel ))	
			{
				SIP_S8bit *pViaBranch = Sdf_co_null;
				SIP_S8bit *pExistViaBranch = Sdf_co_null;

				pViaBranch = pNewCommonInfo->pKey->pRemoteViaBranch;
				pExistViaBranch = pExistingCommonInfo->pKey->pRemoteViaBranch;

				if( pViaBranch != Sdf_co_null )
				{
					if(pExistViaBranch != Sdf_co_null)
					{
						if( Sdf_mc_strcmp(pViaBranch, pExistViaBranch) != 0)
							*pCallLegMatch = Sdf_en_notMatching;
					}
					else
						*pCallLegMatch = Sdf_en_notMatching;
				}
				else
				{
					if(pExistViaBranch != Sdf_co_null)
						*pCallLegMatch = Sdf_en_notMatching;
				}
			}
		}
	}
	/* Free the local reference */
	(void)sdf_ivk_uaFreeCommonInfo(pNewCommonInfo);
	(void)sdf_ivk_uaFreeCommonInfo(pExistingCommonInfo);
	/*Fix from UATK 2.2*/
	if (Sdf_en_tagMismatchError != pErr->errCode)
	{
		pErr->errCode = Sdf_en_noUaError;
	}
	/*Fix from UATK 2.2*/
		
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaCompareCallObjectKeys");
	return Sdf_co_success;
}


/*******************************************************************
** FUNCTION: sdf_ivk_uaRedirectCall
**
** DESCRIPTION: Prepares a fresh INVITE to the new address
** 				(specified as a parameter)
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaRedirectCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pOrigCallObj,
	Sdf_st_callObject *pNewCallObj,
	SipHeader *pContact,
	Sdf_st_overlapTransInfo **ppOverlapTransInfo,
	Sdf_st_error *pError)
#else
	(pOrigCallObj, pNewCallObj, pContact, pOverlapTransInfo, pError)
	Sdf_st_callObject *pOrigCallObj;
	Sdf_st_callObject *pNewCallObj;
	SipHeader *pContact;
	Sdf_st_overlapTransInfo **ppOverlapTransInfo,
	Sdf_st_error *pError;

#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaRedirectCall");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRedirectCall(): pError param passed is invalid");
#endif
		return Sdf_co_fail;
	}

	if(pOrigCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectCall( ): "
			"Invalid Call Object - original call object",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(pNewCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectCall( ): "
			"Invalid Call Object - new call object",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	if(sdf_ivk_uaRedirectRequest(pOrigCallObj,pNewCallObj,pContact, \
		(Sdf_ty_s8bit *)"INVITE", ppOverlapTransInfo, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectCall( ): "
			"Failed to RedirectRequest",pError);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaRedirectCall");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/*******************************************************************
** FUNCTION: sdf_ivk_uaRedirectRequest
**
** DESCRIPTION: Prepares a fresh Request to the new address
** 				(specified as a parameter)
**
********************************************************************/
Sdf_ty_retVal sdf_ivk_uaRedirectRequest
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pOrigCallObj,
	Sdf_st_callObject *pNewCallObj,
	SipHeader *pContact,
	Sdf_ty_s8bit *pMethod,
	Sdf_st_overlapTransInfo **ppOverlapTransInfo,
	Sdf_st_error *pError)
#else
	(pOrigCallObj, pNewCallObj, pContact,pMethod, ppOverlapTransInfo, pError)
	Sdf_st_callObject *pOrigCallObj;
	Sdf_st_callObject *pNewCallObj;
	SipHeader *pContact;
	Sdf_ty_s8bit *pMethod;
	Sdf_st_overlapTransInfo **ppOverlapTransInfo;
	Sdf_st_error *pError;

#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null, *pContactAddrSpec = Sdf_co_null;
	SIP_S8bit	*pDispName = Sdf_co_null,*pName=Sdf_co_null;
	Sdf_ty_u32bit dCount=0,i=0;
	SipUrl	*pSipUrl=Sdf_co_null;
	SipParam *pParam=Sdf_co_null;
#ifdef SDF_LINT
	Sdf_st_listIterator dListIterator;
#else
	Sdf_st_listIterator dListIterator;
#endif    
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_ty_messageType dMethodType = Sdf_en_unknownRequest;
	SipMessage				**ppSipMessage;
	Sdf_ty_slist 			*pslMessageBody;
	Sdf_st_msgInfo			dMessageInfo;

	sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG - Entering sdf_ivk_uaRedirectRequest");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRedirectRequest(): pError param passed is invalid");
#endif
		return Sdf_co_fail;
	}

	if(pOrigCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Invalid Call Object - original call object",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(pNewCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Invalid Call Object - new call object",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Check if the address to be redirected to is a sip: or tel: address.
	 * Other redirections (mailto:, http:, etc) are not handled
	 * using a re INVITE
	 */
	if (sip_getAddrSpecFromContactHdr(pContact, &pContactAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to get addrspec from contact hdr",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}


	if(pContactAddrSpec->dType == SipAddrReqUri)
	{
#ifdef SDF_TEL
		/*
		 * Check if this is a tel Url,if yes then proceed, else return
		 * Sdf_en_notImplemented
		 */
		if(sip_isTelUrl(pContactAddrSpec,(SipError*)&(pError->stkErrCode))==\
				SipFail)
		{
			/* Free the local reference */
			sip_freeSipAddrSpec(pContactAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
				"Address to redirect to, is not a sip: or tel: address",pError);
#endif
			pError->errCode=Sdf_en_notImplemented;
			return Sdf_co_fail;
		}
#else
		/* Free the local reference */
		sip_freeSipAddrSpec(pContactAddrSpec);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Address to redirect to, is not a sip: address",pError);
#endif
			pError->errCode=Sdf_en_notImplemented;
			return Sdf_co_fail;
#endif

	}
#ifdef SDF_TEL
	if( SipFail == sip_isTelUrl(pContactAddrSpec, \
				(SipError*)&(pError->stkErrCode)) )
	{
#endif
		/* 
		 * Iterate thru the Contact params and find if "method" param
		 * exists. If present, honor it.
		 */
		if (sdf_ivk_uaGetUrlFromAddrSpec(pContactAddrSpec,&pSipUrl,\
			pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
				"Failed to get Url from AddrSpec",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipAddrSpec(pContactAddrSpec);
			return Sdf_co_fail;
		}

		if (SipFail == sip_getUrlParamCountFromUrl(pSipUrl,&dCount, \
				(SipError*)&(pError->stkErrCode)))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
				"Failed to get Url Count from Url",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipAddrSpec(pContactAddrSpec);
			sip_freeSipUrl(pSipUrl);
			return Sdf_co_fail;
		}

		for (i=0;i<dCount;i++)
		{
			if(sip_getUrlParamAtIndexFromUrl(pSipUrl,&pParam,i,\
				(SipError*)&(pError->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
					"Failed to get Param from Url",pError);
#endif
				sip_freeSipAddrSpec(pContactAddrSpec);
				sip_freeSipUrl(pSipUrl);
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if(sip_getNameFromSipParam(pParam,&pName,(SipError*)&(pError->\
				stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
					"Failed to get Param Name from SipParam",pError);
#endif
				sip_freeSipAddrSpec(pContactAddrSpec);
				sip_freeSipUrl(pSipUrl);
				sip_freeSipParam(pParam);
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if(Sdf_mc_strcmp(pName,"method")==0)
			{
				/*Overwrite pMethod*/
				if(sip_getValueAtIndexFromSipParam(pParam,&pMethod,0,\
					(SipError*)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
						"Failed to get Param Value from SipParam",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					sip_freeSipAddrSpec(pContactAddrSpec);
					sip_freeSipUrl(pSipUrl);
					sip_freeSipParam(pParam);
					return Sdf_co_fail;
				}
				sip_freeSipParam(pParam);
				break;
			}
			sip_freeSipParam(pParam);
		}
		sip_freeSipUrl(pSipUrl);
#ifdef SDF_TEL
	}
#endif
	sip_freeSipAddrSpec(pContactAddrSpec);
	
	if(sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Failed to get enum value for a method", pError);
#endif
		return Sdf_co_fail;
	}

	dMessageInfo.pMethod = pMethod;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dMsgType = dMethodType;
	dMessageInfo.dResponse = 0;

	/* If method corresponding to any Regular Transactions then New request
	 * is initiated by appending the info into the Overlap Info */
	dTransactionType = sdf_fn_getTransactionType( Sdf_co_null, \
							dMessageInfo, Sdf_co_false, pError);

	if (sdf_ivk_uaStartTransaction(pNewCallObj, ppOverlapTransInfo, \
				pMethod,  pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Failed to Start Transaction", pError);
#endif
		return Sdf_co_fail;
	}
	if( dTransactionType == Sdf_en_RegularUacTransaction)
	{
		/* Clone Uac transaction to the overlap transaction */
		if (sdf_ivk_uaCloneTxnToOverLapTxn( *ppOverlapTransInfo, \
			pOrigCallObj->pUacTransaction, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Cloning UAC Transaction in call object failed ",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	else
	{
		/*Clone Uac Transaction*/
		if (sdf_ivk_uaCloneTransaction(pNewCallObj->pUacTransaction, \
			pOrigCallObj->pUacTransaction, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Cloning UAC Transaction in call object failed ",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	/*Now Clone the necessary structures from the main callObject*/

	/*Clone From in the CommonInfo*/
	if (sip_cloneSipHeader(pNewCallObj->pCommonInfo->pFrom, pOrigCallObj->\
		pCommonInfo->pFrom,(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,  Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest():  "
			"Failed to clone From header Structure ",pError);
#endif
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
		pError->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	/* Delete any From Tag present in the cloned From Address */

	if(sip_getTagCountFromFromHdr( pNewCallObj->pCommonInfo->pFrom, &dCount,\
			(SipError *)&(pError->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,  Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest():  "
			"Failed to get Tag count from From header Structure ",pError);
#endif
		pError->errCode=Sdf_en_commonInfoAccessError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;

	}

	if (dCount != 0)
	{
		if(sip_deleteTagAtIndexInFromHdr(pNewCallObj->pCommonInfo->pFrom, \
			0, (SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest():  "
				"Failed to delete the From Tag ", pError);
#endif
			pError->errCode = Sdf_en_commonInfoAccessError;
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}
	}
	
#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces, \
		(Sdf_ty_s8bit*)"Issuing callback sdf_cbk_uaCloneAppData", 0, pError);
#endif
	/*
	 * Issue the sdf_cbk_uaCloneAppData to the application in order to clone
	 * the pData part of the pAppData. This is because it is a void pointer that
	 * is set by the application..and hence only the application has knowledge
	 * of the data structure and how to clone it.
	 */
	if (sdf_cbk_uaCloneAppData(pNewCallObj->pAppData, \
		pOrigCallObj->pAppData, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Cloning Appdata from source to destination Failed ",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	/*Clone user porfile*/
	if(Sdf_co_null == pNewCallObj->pInitData)
	{
		if(sdf_ivk_uaInitInitData(&(pNewCallObj->pInitData), pError)==\
			Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): "
				"Initializing user profile in call object failed ",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}
	}

	if(Sdf_co_fail == sdf_ivk_uaCloneInitData( pNewCallObj->pInitData, \
		pOrigCallObj->pInitData, pError))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Cloning user profile from source to destination failed ",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}
#ifdef SDF_TLS
	{
		Sdf_st_sslData   *pSslData = Sdf_co_null;
		/* 
		 * InitData is cloned. Now set the pSslData in this InitData. 
		 * This is required because SSL data is not cloned inside CloneInitData
		 * and this need to be done seperately.
		 */
/*w534*/(void)sdf_ivk_uaGetSslDataFromInitData(pOrigCallObj->pInitData, &pSslData,\
		   				pError);
/*w534*/(void)sdf_ivk_uaSetSslDataInInitData(pNewCallObj->pInitData,pSslData,pError);
		(void)sdf_ivk_uaFreeSslData(pSslData);
	}
#endif /* SDF_TLS */


	/*
	 * Clone list of Persistent headers
	 */
	(void)sdf_listInitIterator(&(pOrigCallObj->slPersistentHeaders), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pTempHeaderList = Sdf_co_null;
		Sdf_st_headerList *pClonedHdrList  = Sdf_co_null;

		pTempHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pTempHeaderList->dType,\
			pTempHeaderList->pCorrelation, pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Initializing HeaderList for Persistent Hdrs failed ",pError);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pTempHeaderList,\
			pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Cloning Persistent Headers List failed ",pError);
#endif
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}
		if(Sdf_co_fail == sdf_listAppend(&(pNewCallObj->slPersistentHeaders),\
			 (Sdf_ty_pvoid)pClonedHdrList, pError))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Appending PersistentHeader to slPersistentHeaders of "
				"destination failed ",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pError);
	}
		
#ifdef SDF_3GPP
        /*Copy History-Info From Persistent to Processed*/

        if(sdf_ivk_uaCopyHeaderListByCorrelation(\
                    &((pNewCallObj)->slPersistentHeaders),\
                    &((pNewCallObj)->pUacTransaction->slProcessedHeaders),\
                    (Sdf_ty_s8bit *)"uatk-Historyinfo",pError)==Sdf_co_fail)
        {
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
				"Failed to copy History-Info Headers from persistent"
				"to processed hdr list",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
        }
#endif
	/* Add list of Extra Headers */
		if(sdf_ivk_uaAddExtraHeaders((pNewCallObj), Sdf_co_null, \
			(Sdf_ty_s8bit *)pMethod,pError) ==Sdf_co_fail)
		{
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, pMethod, \
					Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}

	if(sdf_ivk_uaCloneMediaHandling(pNewCallObj->pMediaInfo, \
		pOrigCallObj->pMediaInfo, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Cloning MediaHandling from source to destination failed ",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	pNewCallObj->pMediaInfo->dMediaState=Sdf_en_idleMedia;

	/*All Cloning operation over.Now remove unwanted stuffs.*/

	/*
	 * SPR 2149 Fixed.
	 * Reset the socket descriptor to invalid since a new connection
	 * (incase of TCP/TLS) should be initiated to contact the new
	 * address.
	 */
	if( dTransactionType == Sdf_en_RegularUacTransaction)
	{
		(*ppOverlapTransInfo)->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
		(*ppOverlapTransInfo)->dSocket.dProtocol=Sdf_en_protoNone;
	}
	else
	{
		pNewCallObj->pUacTransaction->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
		pNewCallObj->pUacTransaction->dSocket.dProtocol=Sdf_en_protoNone;
	}

	/*
	 * If there are any Route headers in the response, these can be
	 * deleted from the call object as they should not be used in the
	 * fresh INVITE routing.
	 */
/*w534*/(void)sdf_ivk_uaDeleteHeaderListByCorrelation( \
				&(pNewCallObj->slPersistentHeaders), "uat-routeset", pError);
	/*
	 * Replace the To information in the new call Object with the new To
	 */
	if (pNewCallObj->pCommonInfo->pTo != Sdf_co_null)
	{
		sip_freeSipHeader(pNewCallObj->pCommonInfo->pTo);

		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid*)&(pNewCallObj->pCommonInfo->pTo), pError);
	}

	if (sip_initSipHeader(&(pNewCallObj->pCommonInfo->pTo), SipHdrTypeTo, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to initialize Sipheader of type To",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;

		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);

		return Sdf_co_fail;
	}

	if (sip_getAddrSpecFromContactHdr(pContact, &pContactAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to get addrspec from contact hdr",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	if (sip_initSipAddrSpec(&pAddrSpec, pContactAddrSpec->dType, (SipError *)\
		&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to initialize addrspec",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	if (sip_cloneSipAddrSpec(pAddrSpec, pContactAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to clone contact addrspec",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		sip_freeSipAddrSpec(pContactAddrSpec);
        /* klock tool error fix */
                sip_freeSipAddrSpec(pAddrSpec);
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}
	sip_freeSipAddrSpec(pContactAddrSpec);

	/* From Address Spec delete all params existing */
/*  Get the URL From Address Spec
	if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl,pError) != Sdf_co_fail)
    {
        sip_freeSipUrl(pSipUrl);
    }*/
	if (sip_setAddrSpecInToHdr(pNewCallObj->pCommonInfo->pTo, pAddrSpec, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to set addrspec in To header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		sip_freeSipAddrSpec(pAddrSpec);
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}
	/*get Display Name from Conatact Header */

/*w534*/(void)sip_getDispNameFromContactHdr(pContact,&pDispName,\
			(SipError*)&(pError->stkErrCode));

	if(pDispName!=Sdf_co_null)
	{
		Sdf_ty_s8bit	*pTmpDispName = Sdf_co_null;

		pTmpDispName = Sdf_mc_strdupCallHandler(pDispName);

		/*set DisplayName in To Header*/
		if(sip_setDispNameInToHdr(pNewCallObj->pCommonInfo->pTo, \
				pTmpDispName, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
				"Failed to set Display Name in To header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipAddrSpec(pAddrSpec);
			(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
				pMethod, Sdf_en_uacTransaction);
			return Sdf_co_fail;
		}
	}

	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);

	if( *ppOverlapTransInfo == Sdf_co_null )
		pslMessageBody = &(pNewCallObj->pUacTransaction->slMsgBodyList);
	else
		pslMessageBody = &((*ppOverlapTransInfo)->slMsgBodyList);
		
	/*Delete MsgBody Before Forming New Request*/
	if (sdf_listDeleteAll( pslMessageBody, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to delete Message Body from the List",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	/*
	 * Increment the current local Cseq Number for the fresh request
	 * being initiated
	 */
	pNewCallObj->pCommonInfo->dLocalCseq ++;
	if( *ppOverlapTransInfo != Sdf_co_null )
	 (*ppOverlapTransInfo)->dLocalCseq = pNewCallObj->pCommonInfo->dLocalCseq;

	/*
	 * Generate a new call-id for the call
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)\
		&(pNewCallObj->pCommonInfo->pCallid), pError);

	pNewCallObj->pCommonInfo->pCallid = sdf_ivk_uaGenerateCallid( \
						pNewCallObj->pInitData, pNewCallObj->pAppData);

	/*
	 * Now form a fresh Request to be sent to the redirected address.
	 */
	if (sdf_ivk_uaFormRequest(pMethod, pNewCallObj, \
		*ppOverlapTransInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to form INVITE request", pError);
#endif
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}

	if( *ppOverlapTransInfo == Sdf_co_null )
		ppSipMessage = &(pNewCallObj->pUacTransaction->pSipMsg);
	else
		ppSipMessage = &((*ppOverlapTransInfo)->pSipMsg);

	if( Sdf_co_fail == sdf_fn_uaHandleContactParams(*ppSipMessage, pContact, \
				pError))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest( ): "
			"Failed to handle contact params",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}
	/*
	 * Update the CallObject key with the new details
	 */
	if (sdf_fn_uaUpdateCallConstants(pNewCallObj, *ppSipMessage, \
			dMessageInfo, Sdf_co_false, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaRedirectRequest(): "
			"Failed to update constants for the call",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		(void)sdf_ivk_uaClearTransaction(pNewCallObj, *ppOverlapTransInfo, \
			pMethod, Sdf_en_uacTransaction);
		return Sdf_co_fail;
	}
	pNewCallObj->dCallObjectType = pOrigCallObj->dCallObjectType;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG-Exiting sdf_ivk_uaRedirectRequest");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRetrieveMatchingOverlapTransInfo
 **
 ** DESCRIPTION: This API retrieves an OverlapTransInfo element within the 
 ** 			 call object that corresponds to the SipMessage passed.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaRetrieveMatchingOverlapTransInfo
#ifdef ANSI_PROTO
	(SipMessage 		*pMessage,
	Sdf_st_callObject 	*pCallObj,
	Sdf_st_overlapTransInfo **ppTransInfo,
	Sdf_ty_msgMode		 dMsgMode,
	Sdf_st_error *pError)
#else	
	(pMessage,	pCallObj, ppTransInfo, pError)
	SipMessage *pMessage;
	Sdf_st_callObject 	*pCallObj;
	Sdf_st_overlapTransInfo **ppTransInfo;
	Sdf_ty_msgMode		 dMsgMode;
	Sdf_st_error 		*pError;
#endif
{
	en_SipMessageType dType;
#ifndef SDF_LINT
    Sdf_ty_s8bit *pMethod = Sdf_co_null;
#endif    
#ifdef SIP_RPR
    Sdf_ty_bool	dPrackRequest  = Sdf_co_false;
	Sdf_ty_bool	dPrackResponse = Sdf_co_false;
	Sdf_ty_bool dRPResponse	   = Sdf_co_false;
#endif
	Sdf_ty_messageType dMethodType = Sdf_en_unknownRequest;
	SipHeader	dHdr;
	Sdf_ty_u32bit dRespNum = 0, dCseqNum = 0, CheckCseq = 0;
	Sdf_ty_u16bit dRespCode = 0;
	Sdf_ty_bool	dCometRequest  = Sdf_co_false;
	Sdf_ty_bool	dCometResponse = Sdf_co_false;
	Sdf_ty_bool	dUpdateRequest  = Sdf_co_false;
	Sdf_ty_bool	dUpdateResponse = Sdf_co_false;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_ty_s8bit *pCseqMethod = Sdf_co_null;
  Sdf_st_msgInfo dMessageInfo    = {Sdf_co_null, SipMessageAny, 
                                    Sdf_en_RegularTxn, 0, 0};

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaRetrieveMatchingOverlapTransInfo");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): pError param passed "
			"is invalid");
#endif
		return Sdf_co_fail;
	}

	if (pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo( ): "
			"Invalid SIP message passed as parameter ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	
	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo( ): "
			"Invalid Call Object passed as parameter",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppTransInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo( ): "
			"Invalid OverlapTransInfo passed as parameter",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if ((dMsgMode != Sdf_en_msgModeRecd)&&(dMsgMode != Sdf_en_msgModeSend))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo( ): "
			"Invalid message mode passed as parameter",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	*ppTransInfo=Sdf_co_null;

	/*
	 * An OverlapTransInfo element exists only for the following cases:
     * PRACK, COMET.
	 *
	 * Rule used is as follows:
     * If the method is PRACK Request, then the Rseq num in the RAck header 
     * should be same as the rseq num stored in the OverlapTransInfo structure.
     * If the method is COMET request, then the Cseq number should be the same
     * as the Cseq number stored in the OverlapTransInfo structure.
     * If the Message is a reliable 1xx response, matching is based on rseq
     * number of Rseq header.
     * If the Message is a 2XX and above response, then matching is based on
     * Cseq number
	 */
	
	if( Sdf_co_success != sdf_fn_uaGetMessageInfo( pMessage, &dMessageInfo, \
				pError))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
			"Getting Message Info From SipMessage failed",pError);
#endif
		return Sdf_co_fail;
	}

#ifndef SDF_LINT
	pMethod = dMessageInfo.pMethod;
#endif    
	dMethodType = dMessageInfo.dMsgType;
	dType		= dMessageInfo.dReqRespType;
	dRespCode	= dMessageInfo.dResponse;

	dTransactionType = sdf_fn_getTransactionType(pMessage, dMessageInfo, \
					Sdf_co_true, pError);

	if( (dTransactionType == Sdf_en_RegularUacTransaction) || \
		(dTransactionType == Sdf_en_RegularUasTransaction)	)
	{
		/*
		 * If the message is either a Regular Req or Reqp
		 * matching is based on Cseq number. 
		 */
		if (sip_getHeader(pMessage,SipHdrTypeCseq, &dHdr,\
			(SipError*)	&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
				"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
				"Failed to get Cseq header from SIP message",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getSeqNumFromCseqHdr (&dHdr, &dCseqNum,\
			(SipError*)	&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
				"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): " \
				"Failed to get Cseq number from Cseq header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if(sip_getMethodFromCseqHdr(&dHdr, &pCseqMethod,(SipError*)\
				&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
				"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): " \
				"Failed to get Cseq Method from Cseq header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		CheckCseq = 1;
		sip_freeSipHeader(&dHdr);
	}
	else
	{
		if (dType == SipMessageRequest)
		{
			/*
			 * Return from the function if the method is neither 
			 * PRACK nor COMET.
			 */
			if (dMethodType == Sdf_en_comet)
				dCometRequest = Sdf_co_true;
#ifdef SIP_RPR
			else if (dMethodType == Sdf_en_prack)
				dPrackRequest = Sdf_co_true;
#endif /* SIP_RPR */
			else if (dMethodType == Sdf_en_update)
				dUpdateRequest = Sdf_co_true;
			else
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
				"OverlapTransInfo can only have methods as PRACK/COMET",pError);
#endif
				pError->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
			}
		}
		else
		{
			/*
			 * Message is a response. Only responses that are allowed in this
			 * API are responses to PRACK or COMET or RPRs. Any other response
			 * should be rejected.
			 */

			if (dMethodType == Sdf_en_comet)
				dCometResponse = Sdf_co_true;
#ifdef SIP_RPR
			else if (dMethodType == Sdf_en_prack)
				dPrackResponse = Sdf_co_true;
#endif /* SIP_RPR */
			else if (dMethodType == Sdf_en_update)
				dUpdateResponse = Sdf_co_true;
			else
			{
				/*
				 * Message needs to be a RPR if we are to continue processing
				 * in this API. Check that now.
				 */
#ifdef SIP_RPR
				if (sdf_fn_uaIsMessageRPR(pMessage, pError) != 0)
				{
#endif /* SIP_RPR */
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_invalidParamError, (Sdf_ty_s8bit *) \
							"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
							"Response neither a RPR nor a response for" \
						   	"PRACK/COMET.", pError);
#endif
					pError->errCode=Sdf_en_invalidParamError;
					return Sdf_co_fail;
#ifdef SIP_RPR
				}
				else
					dRPResponse = Sdf_co_true;
#endif /* SIP_RPR */
			}
		}

#ifndef SIP_RPR
		if (dCometRequest || dCometResponse || dUpdateRequest || \
				dUpdateResponse)
#else
		if (dPrackRequest || dPrackResponse || dCometRequest || dCometResponse\
				|| dUpdateRequest || dUpdateResponse)
#endif
		{
#ifdef SIP_RPR
			if (dPrackRequest)
			{
				/*
				 * If the message is a PRACK Request, matching is based 
				 * on Rseq num in rack header. So get the Rseqnum 
				 */
				if (sip_getHeader(pMessage,SipHdrTypeRAck, &dHdr,\
					(SipError*)	&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
						"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
						"Failed to get RACK header from SIP message",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				if (sip_rpr_getRespNumFromRAckHdr (&dHdr, &dRespNum,\
					(SipError*)	&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
						"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
						"Failed to get Rseq number from RACK header",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
			}
			else
#endif /* SIP_RPR */
			   	if (dCometRequest || dUpdateRequest || \
					((dType == SipMessageResponse)&&(dRespCode >= 200)))
			{
				/*
				 * If the message is either a COMET request OR
				 * a >=200 OK to PRACK/COMET, matching is based on
				 * Cseq number. 
				 */
				if (sip_getHeader(pMessage,SipHdrTypeCseq, &dHdr,\
					(SipError*)	&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
						"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
						"Failed to get Cseq header from SIP message",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				if (sip_getSeqNumFromCseqHdr (&dHdr, &dCseqNum,\
					(SipError*)	&(pError->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
						"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): " \
						"Failed to get Cseq number from Cseq header",pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				CheckCseq = 1;
			}
			sip_freeSipHeader(&dHdr);
		}
#ifdef SIP_RPR
		else if (dRPResponse)
		{
			/*
			 * The only other case is a RPR response, Here matching is based
			 * on Rseq number in Rseq header
			 */
			if (sip_getHeader(pMessage,SipHdrTypeRSeq, &dHdr,\
				(SipError*)	&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
					"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
					"Failed to get Rseq header from SIP message",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if (sip_rpr_getRespNumFromRSeqHdr(&dHdr, &dRespNum,\
					(SipError*)	&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, (Sdf_ty_s8bit *) \
					"sdf_ivk_uaRetrieveMatchingOverlapTransInfo(): "
					"Failed to get Rseq number from Rseq header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			sip_freeSipHeader(&dHdr);
		}
#endif /* SIP_RPR */
	}

	/*
	 * Now iterate through call objects's OverlapTransInfo elements
     * and get the relevant one
     */

	(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
			&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		*ppTransInfo = (Sdf_st_overlapTransInfo*)\
			(dListIterator.pCurrentElement->pData);

		if (CheckCseq)
		{
			/*
			 * Check against remote cseq if:
			 * 1. It is a request that has been recd. 		OR
			 * 2. It is a response that is being sent.
			 */
			if ( (	(dMsgMode == Sdf_en_msgModeRecd) && \
					(dType == SipMessageRequest)) ||	\
					 ((dMsgMode == Sdf_en_msgModeSend) && \
					  (dType == SipMessageResponse)) )
			{
				if ((*ppTransInfo)->dRemoteCseq == dCseqNum)
				{
					HSS_LOCKEDINCREF((*ppTransInfo)->dRefCount);
					pError->errCode = Sdf_en_noUaError;
					return Sdf_co_success;
				}
			}
			else
			{
				if ((*ppTransInfo)->dLocalCseq == dCseqNum)
				{
					HSS_LOCKEDINCREF((*ppTransInfo)->dRefCount);
					pError->errCode = Sdf_en_noUaError;
					return Sdf_co_success;
				}
			}
		}
		else
		{
			if ((*ppTransInfo)->dRseq == dRespNum)
			{
				HSS_LOCKEDINCREF((*ppTransInfo)->dRefCount);
				pError->errCode = Sdf_en_noUaError;
				return Sdf_co_success;
			}	
		}
		(void)sdf_listNext(&dListIterator, pError);
	}

	pError->errCode = Sdf_en_noExistError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaRetrieveMatchingOverlapTransInfo");
	return Sdf_co_fail;
}


/*****************************************************************************
 ** FUNCTION: 	 sip_freeEventContext
 **
 ** DESCRIPTION: Stack callback to free the SipEventContext structure. The
 **				UA Toolkit in turn invokes sdf_ivk_uaFreeEventContext and
 **				if the reference count is 0, the sdf_cbk_uaFreeEventContextData
 **				callback is issued to the application.
 **
 ******************************************************************************/
void sip_freeEventContext(SipEventContext *pContext)
{
	Sdf_st_eventContext *pEventContext = Sdf_co_null;
#ifdef SDF_TXN
	Sdf_st_txnContextData	*pTxnContextData = Sdf_co_null;
#endif

	if (pContext == Sdf_co_null)
		return;

	pEventContext = (Sdf_st_eventContext *)pContext;
#ifdef SDF_TXN
	/*
	 * Restore the void pointer stored in the event context data structure
	 * as the application's data.
	 */
	pTxnContextData		 = (Sdf_st_txnContextData *)pEventContext->pData;
	pEventContext->pData = pTxnContextData->pApplicationData;
	(void)sdf_ivk_uaFreeTxnContextData(pTxnContextData);
#endif
	(void)sdf_ivk_uaFreeEventContext(pEventContext);
}


/******************************************************************************
** FUNCTION: sdf_ivk_uaReturnRequiredList
**
** DESCRIPTION: This function returns the list of require headers as 
**	            a list of strings.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaReturnRequiredList 
#ifdef ANSI_PROTO 
	(SipMessage *pSipMsg,
	Sdf_ty_slist *pslRequiredList,
	Sdf_st_error *pErr)
#else
	(pSipMsg,pslRequiredList, pErr )
	SipMessage *pSipMsg;
	Sdf_ty_slist *pslRequiredList;
	Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaReturnRequiredList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaReturnRequiredList (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pslRequiredList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnRequiredList():  "
			"pRequiredList Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnRequiredList():  "
			"pSipMsg Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Retrieve list of Require headers as strings.
	 */
	if (sdf_fn_uaGetHeaderListAsStrings(pSipMsg, \
			SipHdrTypeRequire, pslRequiredList, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnRequiredList():  "
			"Failed to get list of Require headers as strings",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaReturnRequiredList");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_ivk_uaReturnSupportedList
**
** DESCRIPTION: This function returns the list of Supported headers as 
**	            a list of strings.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaReturnSupportedList 
#ifdef ANSI_PROTO 
	(SipMessage *pSipMsg,
	Sdf_ty_slist *pslSupportedList,
	Sdf_st_error *pErr)
#else
	(pSipMsg,pslSupportedList, pErr )
	SipMessage *pSipMsg;
	Sdf_ty_slist *pslSupportedList;
	Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaReturnSupportedList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaReturnSupportedList (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pslSupportedList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnSupportedList():  "
			"pslSupportedList Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnSupportedList():  "
			"pSipMsg Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	
	/* 
	 * Retrieve list of Supported headers as strings here
	 */
	if (sdf_fn_uaGetHeaderListAsStrings(pSipMsg, SipHdrTypeSupported, \
		pslSupportedList, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReturnSupportedList():  "
			"Failed to get list of Supported headers as strings",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaReturnSupportedList");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


#ifdef SDF_THREAD_SAFE

/******************************************************************************
** FUNCTION: sdf_ivk_uaLockCallObject
**
** DESCRIPTION: This function locks the call object for the exclusive use 
**				of the the calling thread.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaLockCallObject 
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pCallObj,
	Sdf_st_error *pErr)
#else
	(pCallObj, pErr )
	Sdf_st_callObject *pCallObj;
	Sdf_st_error *pErr;
#endif
{
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId dCallingThread = 0;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaLockCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaLockCallObject (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaLockCallObject ():  "
			"pCallObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

#ifdef SDF_THREAD_SAFE
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

	/*
	 * Lock the dCallObjectMutex
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaLockMutex(dCallingThread, &(pCallObj->dCallObjectMutex), 0);
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaLockCallObject");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/******************************************************************************
** FUNCTION: sdf_ivk_uaUnlockCallObject
**
** DESCRIPTION: This function unlocks the call object 
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUnlockCallObject 
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pCallObj,
	Sdf_st_error *pErr)
#else
	(pCallObj, pErr )
	Sdf_st_callObject *pCallObj;
	Sdf_st_error *pErr;
#endif
{
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId dCallingThread = 0;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaUnlockCallObject");


	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaUnlockCallObject (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUnlockCallObject ():  "
			"pCallObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#ifdef SDF_THREAD_SAFE
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

	/*
	 * Unlock the dCallObjectMutex
	 */
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(dCallingThread, &(pCallObj->dCallObjectMutex));
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaUnlockCallObject");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

#endif

/******************************************************************************
** FUNCTION: sdf_ivk_uaEndTransaction
**
** DESCRIPTION: This function unlocks the call object locked using 
**				Sdf_ivk_uaStartTransaction
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaEndTransaction 
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pCallObj,
	 Sdf_st_overlapTransInfo	*pOverlapTransaction,
	 const Sdf_ty_s8bit *pMethod,
	 Sdf_st_error *pErr)
#else
	(pCallObj, pOverlapTransaction, pMethod, pErr )
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo	*pOverlapTransaction;
	const Sdf_ty_s8bit *pMethod,
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_messageType dMsgType=Sdf_en_unknownRequest;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaEndTransaction");

#ifdef SDF_THREAD_SAFE
	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaEndTransaction (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaEndTransaction ():  "
			"pCallObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr) \
					== Sdf_co_fail)
			return Sdf_co_fail;

	if( (dMsgType == Sdf_en_invite) ||
		(dMsgType == Sdf_en_cancel) ||
		(dMsgType == Sdf_en_bye)	||
		(dMsgType == Sdf_en_ack))
	{
		dRetVal= sdf_ivk_uaUnlockCallObject(pCallObj, pErr);
	}
	else
	{
		dRetVal = sdf_ivk_uaEndRegularTransaction(pCallObj, \
			pOverlapTransaction, pMethod, pErr);
	}
	/* Unlock the call object */

#else
	/* In non-threadsafe mode, we need to do nothing */
	(void) pCallObj;
	(void)pOverlapTransaction;
	(void)pMethod;
	(void) pErr;
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaEndTransaction");
	return dRetVal;
}

/******************************************************************************
** FUNCTION: sdf_ivk_uaEndRegularTransaction
**
** DESCRIPTION: This function unlocks the call object locked using 
**				Sdf_ivk_uaStartTransaction
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaEndRegularTransaction 
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo	*pOverlapTransaction,
	const Sdf_ty_s8bit *pMethod,
	Sdf_st_error *pErr)
#else
	(pCallObj, pOverlapTransaction, pMethod, pErr )
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransaction;
	const Sdf_ty_s8bit *pMethod;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaEndRegularTransaction");

	(void)pOverlapTransaction;
	(void)pMethod;

#ifdef SDF_THREAD_SAFE
	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaEndRegularTransaction (): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaEndRegularTransaction ():  "
			"pCallObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* Unlock the call object */
	dRetVal= sdf_ivk_uaUnlockCallObject(pCallObj, pErr);

#else
	/* In non-threadsafe mode, we need to do nothing */
	(void) pCallObj;
	(void) pOverlapTransaction;
	(void) pMethod;
	(void) pErr;
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaEndRegularTransaction");
	return dRetVal;
}

/*********************************************************
 ** FUNCTION: sdf_ivk_uaComputeHashFromCallObjectKey
 ** 
 ** DESCRIPTION: this function takes a CallObject Key structure
 ** 				 and returns a hash value
 **
 *********************************************************/
Sdf_ty_u32bit sdf_ivk_uaComputeHashFromCallObjectKey 
#ifdef ANSI_PROTO 
	(Sdf_st_callObjectKey *pCallObjkey)
#else
	(pCallObjkey)
	Sdf_st_callObjectKey *pCallObjkey;
#endif
{
	return (sdf_ivk_uaElfHash((void*)(pCallObjkey->pCallId))); 
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRejectRequest
 **
 ** DESCRIPTION: This function has to be invoked to send a failure response to 
 **				 a recd request if any validation prior to 
 **				 sdf_ivk_uaUpdateCallDetails fails. The call object passed to 
 **				 this API is the temporary call object returned by 
 **				 sdf_ivk_uaDecodeMessage().
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaRejectRequest
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pTempCallObj, Sdf_st_initData *pInitData, \
	 Sdf_ty_u16bit dRespCode, Sdf_st_overlapTransInfo **ppOverlapTransInfo, \
	 Sdf_st_error *pError)
#else
	(pTempCallObj, pInitData, dRespCode, ppOverlapTransInfo, pError)
	Sdf_st_callObject *pTempCallObj;
	Sdf_st_initData *pInitData;
	Sdf_ty_u16bit dRespCode;
	Sdf_st_overlapTransInfo **ppOverlapTransInfo;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	SipMessage				**ppSipMsg;
	Sdf_ty_retVal			retVal = Sdf_co_fail;
	Sdf_ty_s8bit 			*pMethod = Sdf_co_null;
	Sdf_ty_messageType  	dMethodType = Sdf_en_unknownRequest;
	en_SipMessageType 		dType;
	Sdf_st_msgInfo			dMessageInfo;


	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaRejectRequest");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRejectRequest(): pError param passed is invalid");
#endif
		return Sdf_co_fail;
	}

	if (pTempCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRejectRequest( ): "
			"Invalid call object passed as parameter ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of parameter validations */

	/* If Transaction type is Regular Transaction then create Overlap
	 * Transaction and send the response over it */

/*w534*/(void)sdf_ivk_uaGetMethodFromSipMessage \
		((pTempCallObj)->pUasTransaction->pSipMsg,&pMethod, pError);
	if (sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMethodType, pError) \
					== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRejectRequest: "
			"Failed to get enum value for a method",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if(sip_getMessageType((pTempCallObj)->pUasTransaction->pSipMsg, &dType, \
						(SipError*)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRejectRequest: "
			"Failed to get type of SIP Message",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	dMessageInfo.dMsgType  = dMethodType;		
	dMessageInfo.dReqRespType = dType;
	dMessageInfo.dResponse = dRespCode;
	dMessageInfo.pMethod = Sdf_co_null;
	
	dTransactionType = sdf_fn_getTransactionType( \
							(pTempCallObj)->pUasTransaction->pSipMsg, \
							dMessageInfo, Sdf_co_true,pError);

	/* If the incoming message is an Response then this API should not be 
	 * called */
	
	if( dType != SipMessageRequest )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRejectRequest( ): "
			"Invalid message received. Message is not Request",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if( (dTransactionType == Sdf_en_RegularUasTransaction) ||
	 	(dTransactionType == Sdf_en_overlapTransaction) )
	{
		if( *ppOverlapTransInfo == Sdf_co_null )
		{
            Sdf_st_socket		*pIncSocket = Sdf_co_null;
	        Sdf_st_socket		*pOrgSocket = Sdf_co_null;

			if(sdf_ivk_uaInitOverlapTransInfo(ppOverlapTransInfo, pError) \
							== Sdf_co_fail)
					return Sdf_co_fail;
		
			(*ppOverlapTransInfo)->dMsgType = dMethodType;
			
			(void)sdf_listAppend(&(pTempCallObj->slOverlapTransInfo), \
							*ppOverlapTransInfo, pError);

            pOrgSocket = &(pTempCallObj->pUasTransaction->dSocket);
            pIncSocket = &((*ppOverlapTransInfo)->dSocket);

            if(pOrgSocket->dSockfd != Sdf_co_InvalidSocketFd)
        	{
		        if(pIncSocket->dSockfd != pOrgSocket->dSockfd)
		        {
        			if(pIncSocket->dProtocol != Sdf_en_protoUdp)	
								(void)sdf_fn_uaCloseSocket(pIncSocket);

			        pIncSocket->dProtocol  =  pOrgSocket->dProtocol;
        			pIncSocket->dSockfd    =  pOrgSocket->dSockfd;
    
	        	}
	        }

        	/* Remove the sockinfo from  Temp Callobject.  This avoids
        	 * the socket getting closed while freeing the Temp callobject*/
        	pOrgSocket->dProtocol = Sdf_en_protoNone;
        	pOrgSocket->dSockfd   = Sdf_co_InvalidSocketFd;
		}
		ppSipMsg = &((*ppOverlapTransInfo)->pSipMsg);

		if (*ppSipMsg != Sdf_co_null)
			sip_freeSipMessage (*ppSipMsg);

		*ppSipMsg = (pTempCallObj)->pUasTransaction->pSipMsg ;
		HSS_LOCKEDINCREF((*ppSipMsg)->dRefCount);

		retVal = sdf_fn_uaRejectRequest(pTempCallObj, pInitData, dRespCode,\
				&((*ppOverlapTransInfo)->pSipMsg), \
				dTransactionType, *ppOverlapTransInfo, pError);
	}
	else
	{
		retVal = sdf_fn_uaRejectRequest(pTempCallObj,pInitData,dRespCode,\
					&((pTempCallObj)->pUasTransaction->pSipMsg), \
					Sdf_en_uasTransaction, *ppOverlapTransInfo, pError);
	}

	if ( retVal == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRejectRequest( ): "
			"Failed to Form reject request",pError);
#endif					
		return Sdf_co_fail;
	}


	pError->errCode = Sdf_en_noUaError;
 
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaRejectRequest");
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaGetTransactionForSipMsg
**
** DESCRIPTION: This function returns transaction or
**			overlapped transaction structure that
**			contains the SipMessage which has to be
**			sent out using sendCallToPeer
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTransactionForSipMsg
	(Sdf_st_callObject *pCallObj, SipMessage *pSipMsg,\
	Sdf_st_transaction **ppTransaction,\
	Sdf_st_overlapTransInfo **ppOverlapTransInfo, Sdf_st_error *pErr)
{
	Sdf_ty_s8bit 			*pMethod = Sdf_co_null;
	Sdf_ty_s8bit 			*pCseqMethod = Sdf_co_null;
	SipHeader				dViaHeader;
	SipHeader				dCseqHeader;
	Sdf_ty_s8bit			*pViaBranch = Sdf_co_null;
	Sdf_ty_s8bit			*pIncViaBranch = Sdf_co_null;
	SipMessage				*pLocalMsg = Sdf_co_null;
	Sdf_st_overlapTransInfo	*pOverlapTxn = Sdf_co_null;
	Sdf_st_listIterator 	dListIterator;
	Sdf_ty_u32bit			dCseqNo = 0;
	Sdf_st_msgInfo			dMessageInfo;
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;

	if((Sdf_co_null == pSipMsg) || (Sdf_co_null == pCallObj) || \
		(Sdf_co_null == ppTransaction) || (Sdf_co_null == ppOverlapTransInfo))
		return Sdf_co_fail;

	if(sip_getHeader(pSipMsg, SipHdrTypeVia, &dViaHeader,\
		(SipError*)&(pErr->stkErrCode)) != SipFail)
	{
/*w534*/(void)sdf_fn_GetBranchFromViaHdr(&dViaHeader, &pViaBranch, pErr);

		if( pViaBranch != Sdf_co_null )
			pIncViaBranch = Sdf_mc_strdupAccessor(pViaBranch);

		sip_freeSipHeader(&dViaHeader);
		pViaBranch = Sdf_co_null;
	}
	
	if(sip_getHeader(pSipMsg, SipHdrTypeCseq, &dCseqHeader,\
		(SipError*)&(pErr->stkErrCode)) != SipFail)
	{
		(void)sip_getMethodFromCseqHdr(&dCseqHeader, &pMethod, \
			   (SipError*)&(pErr->stkErrCode));

		(void)sip_getSeqNumFromCseqHdr(&dCseqHeader, &dCseqNo, \
				(SipError*)&(pErr->stkErrCode));

		pCseqMethod = Sdf_mc_strdupAccessor(pMethod);
		pMethod = Sdf_co_null;

		sip_freeSipHeader(&dCseqHeader);
	}
	
	if(sdf_fn_uaGetMessageInfo(pSipMsg, &dMessageInfo, pErr) == \
					Sdf_co_fail)
	{
				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid*)&pIncViaBranch,\
						pErr);
				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *)&pCseqMethod, \
						pErr);
				return Sdf_co_fail;
	}
	
	dTransactionType = sdf_fn_getTransactionType( pSipMsg, dMessageInfo, \
					Sdf_co_true,pErr);
	
	if( (dTransactionType == Sdf_en_uasTransaction) || \
		(dTransactionType == Sdf_en_uacTransaction))
	{
		if(Sdf_co_null != pCallObj->pUasTransaction->pSipMsg)
		{
			pLocalMsg = pCallObj->pUasTransaction->pSipMsg;

			if( Sdf_co_true == sdf_fn_uaIsTransactionMatched(pLocalMsg, \
						pIncViaBranch, pCseqMethod, dCseqNo, pErr))
			{
				(void)sdf_ivk_uaGetTransactionFromCallObject(pCallObj,\
						Sdf_en_uasTransaction, ppTransaction, pErr);

				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid*)&pIncViaBranch,\
						pErr);
				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *)&pCseqMethod, \
						pErr);

				pErr->errCode = Sdf_en_noUaError;
				return Sdf_co_success;
			}
		}

		if(Sdf_co_null != pCallObj->pUacTransaction->pSipMsg)
		{
			pLocalMsg = pCallObj->pUacTransaction->pSipMsg;

			if( Sdf_co_true == sdf_fn_uaIsTransactionMatched(pLocalMsg, \
						pIncViaBranch, pCseqMethod, dCseqNo, pErr))
			{
				(void)sdf_ivk_uaGetTransactionFromCallObject(pCallObj,\
						Sdf_en_uacTransaction, ppTransaction, pErr);

				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid*)&pIncViaBranch,\
						pErr);
				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *)&pCseqMethod, \
						pErr);

				pErr->errCode = Sdf_en_noUaError;
				return Sdf_co_success;
			}
		}
	}
	else
	{
		/* Iterate through the entire Overlap Transactions List */
		(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
				&dListIterator, pErr);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			pOverlapTxn = (Sdf_st_overlapTransInfo*)\
				(dListIterator.pCurrentElement->pData);

			pLocalMsg = pOverlapTxn->pSipMsg;

			if( Sdf_co_true == sdf_fn_uaIsTransactionMatched(pLocalMsg, \
						pIncViaBranch, pCseqMethod, dCseqNo, pErr))
			{
				HSS_LOCKEDINCREF(pOverlapTxn->dRefCount);
				*ppOverlapTransInfo = pOverlapTxn;

				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *) \
						&pIncViaBranch,  pErr);
				(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *) \
						&pCseqMethod, pErr);

				pErr->errCode = Sdf_en_noUaError;
				return Sdf_co_success;
			}
			(void)sdf_listNext(&dListIterator, pErr);
		}
	}
	(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *)&pCseqMethod, pErr);
	(void)sdf_memfree(Sdf_mc_accessorMemId,(Sdf_ty_pvoid *) &pIncViaBranch,  pErr);

	pErr->errCode = Sdf_en_noExistError;
	return Sdf_co_fail;
}

/*******************************************************************
** FUNCTION: sdf_ivk_uaHandleRequireFromMsg
**
** DESCRIPTION:	Checks if the Request/Response Has a Require Header. If
**				the Require header is present, it checks with 
**				the options in the SUPPORTED field of the 
**				profile. The convention is that if the 
**				Supported Header is absent then a 420 Bad 
**				Extensions is to be sent (for requests)
**
** 				If Supported is present in the user profile,
**				then it's values are compared with the require 
**				values. Options that are not supported are returned 
** 				as unsupported in a 420 response(for requests). If the 
**				SUPPORTED attribute in the user profile has ' * ' 
**				as one of it's values then it is assumed that 
**				all the options are supported.
**				The API only prepares a 420 response and places
**				it in pCallObj if the above negotiation fails (for
**				requests). For responses which come with a Require
**				header, it only returns Sdf_en_invalid indicating
**				mismatch.
**
********************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaHandleRequireFromMsg
#ifdef ANSI_PROTO
	(Sdf_st_validateInArg *pValidateInArg,
	 Sdf_st_validateOutArg *pValidateOutArg,
	 Sdf_st_error *pErr)
#else
	(pValidateInArg, pValidateOutArg, pErr)
	Sdf_st_validateInArg *pValidateInArg;
	Sdf_st_validateOutArg *pValidateOutArg;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit size1=0,size2=0,i=0,IndicateSupported=0,unsupportedindex=0;
	Sdf_ty_u32bit ctr=0,sizeval=0;
	Sdf_ty_u32bit UnsuppSize=0;
	Sdf_st_configExtraHeaders *pTemp =Sdf_co_null;
    Sdf_ty_transactionType transType;
	SipMessage *pSipMsg = Sdf_co_null;
	Sdf_ty_slist slUnSupportedHeadersList;
	Sdf_st_listIterator dListIterator;
	Sdf_st_initData *pInitData;
	Sdf_st_callObject *pCallObj;
	en_SipMessageType dType;
#ifdef SDF_SESSION_TIMER	
	Sdf_ty_bool dReqTimer=Sdf_co_false;
#endif	

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Entering " \
			"sdf_ivk_uaHandleRequireFromMsg");

	transType = pValidateInArg->dTransactionType;
	pInitData = pValidateInArg->pInitData;
	pCallObj = pValidateInArg->pCallObj;
	(void)*pValidateOutArg;
	
	if ( (transType == Sdf_en_overlapTransaction) ||
		 (transType == Sdf_en_RegularUasTransaction) ||
		 (transType == Sdf_en_RegularUacTransaction))
		pSipMsg = pValidateInArg->pOverlapTransaction->pSipMsg;
	else
		pSipMsg = pValidateInArg->pTransaction->pSipMsg;

	/*- We need to send 420 if transaction type=UAS or
	 * transaction type is Overlapped & request=PRACK -*/
	if (sip_getMessageType(pSipMsg, &dType, \
				(SipError*)&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
			"Failed to get type of SIP Message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_failed;
	}
		
	/*
	 * Check if the SipMessage Has a Require Header
	 */
	if(sip_getHeaderCount(pSipMsg, SipHdrTypeRequire, \
		&size1, (SipError*)&(pErr->stkErrCode)) == SipFail)
			return Sdf_en_failed;

	if(size1==0)
	{
		/*size1 =0 means require is absent*/
		pErr->errCode=Sdf_en_noUaError; 

		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - " 
			"Exiting sdf_ivk_uaHandleRequireFromMsg");

		return Sdf_en_valid;
	}

	/* Find Require:timer in the message */

#ifdef SDF_SESSION_TIMER
	for(i=0; i<size1;i++)
	{
		SipHeader dHdr;
		Sdf_ty_s8bit *pUnsupp=Sdf_co_null;

		/*
		 * Extract the Require token present in the Require header.
		 */

		if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeRequire, &dHdr, \
			(Sdf_ty_u32bit)ctr, (SipError *)&(pErr->errCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Failed to get Require Header at index in SipMessage\n",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}

		if(sip_getTokenFromRequireHdr(&dHdr,&pUnsupp, \
			(SipError*)&(pErr->errCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Failed to get token from Require Header\n",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}
		sip_freeSipHeader(&dHdr);

		if(Sdf_mc_strcasecmp("timer",pUnsupp)==0)
		{
			dReqTimer=Sdf_co_true;
			break;
		}
	}

	if((dReqTimer == Sdf_co_true) && (size1 == 1))
	{
		if(sdf_fn_uaGetGlbIsSupportedTimer() == Sdf_co_true)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_ivk_uaHandleRequireFromMsg\n");
			pErr->errCode=Sdf_en_noUaError;
			return Sdf_en_valid;
		}
	}
#endif

	/*
	 * Check if SUPPORTED attribute had been set in the user profile.
	 */
	if(sdf_listSizeOf(&(pInitData->slExtraHeadersList), \
		&size2, pErr) == Sdf_co_fail)
			return Sdf_en_failed;

	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		if(pTemp->pParam->pName==Sdf_co_null)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		if((Sdf_mc_strcasecmp("SUPPORTED",pTemp->pParam->pName)==0)|| \
			(Sdf_mc_strcasecmp("K",pTemp->pParam->pName)==0))
		{
			IndicateSupported = 1;
			break;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	if((size2==0)||(IndicateSupported==0))
	{
		/*
		 * There is no SUPPORTED attribute set in the user profile.
		 * Send a 420 Bad Extensions response message.
		 */
		/* 
		 * Display incoming message in call flow (ctascii) 
		 */
		/*- We dont have to decide upon which transaction to use, as it
		 * has already been disided above -*/
        
		/* Form an Unsupported HeaderList by checking which extensions in the
	 	 * Require 
		 */

		/* size1 = size of require headers list */
		(void)sdf_listInit(&slUnSupportedHeadersList, __sdf_fn_uaFreeCompleteHeader, \
		Sdf_co_false, pErr);
        
	/*
	 * Iterate through the list of extensions that have arrived in the Require
	 * header of the SIP message and For every token form an
	 * Unsupported header. Use this list to send out the 420 Bad Extensions
	 * response message.
	 */
		/* 
		 * In case of a response we do not send a 420 Response, just 
		 * return Sdf_en_invalid instead.
		 */
		if (transType == Sdf_en_uacTransaction)
		{
#ifdef SDF_ERROR
			sdf_fn_setError( Sdf_en_majorErrors,Sdf_en_badExtensionsError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Bad Extensions in Require Header\n",pErr);
#endif
			pErr->errCode=Sdf_en_badExtensionsError;
			return Sdf_en_invalid;
		}

	   for(ctr=0;ctr<size1;ctr++)
       {
           SipHeader dHdr;
           SipHeader *pHdr;
           Sdf_ty_s8bit *pUnsupp, *pToken=Sdf_co_null;

           /*
            * Extract the Require token present in the Require header.
            */
           if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeRequire, &dHdr, \
                       (Sdf_ty_u32bit)ctr, (SipError *)&(pErr->stkErrCode)) == SipFail)
           {
#ifdef SDF_ERROR
               sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
                       (Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
                       "Failed to get Require Header at index in SipMessage\n",pErr);
#endif
               pErr->errCode=Sdf_en_headerManipulationError;
               return Sdf_en_failed;
           }

           if(sip_getTokenFromRequireHdr(&dHdr,&pUnsupp, \
                       (SipError*)&(pErr->stkErrCode))==SipFail)
           {
#ifdef SDF_ERROR
               sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
                       (Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
                       "Failed to get token from Require Header\n",pErr);
#endif
               pErr->errCode=Sdf_en_headerManipulationError;
               return Sdf_en_failed;
           }
           sip_freeSipHeader(&dHdr);
           /* 
            * make an Unsupported header and
            * add it to the list of Unsupported headers.
            */

           /* We have found a unsupported/unmatched header in the uas
            * transaction, so Continue adding Unsupported header into 
            * the UAS transaction -*/


           Sdf_mc_strdup(pToken,pUnsupp);
           if(sip_initSipHeader(&pHdr,SipHdrTypeUnsupported, \
                       (SipError*)&(pErr->stkErrCode))==SipFail)
           {
#ifdef SDF_ERROR
               sdf_fn_setError(Sdf_en_majorErrors, \
                       Sdf_en_headerManipulationError, \
                       (Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
                       "Failed to Initialize SipHeader of type Unsupported\n",pErr);
#endif
               pErr->errCode=Sdf_en_headerManipulationError;
               return Sdf_en_failed;
           }

           if(sip_setOptionInUnsupportedHdr(pHdr,pToken, \
                       (SipError*)&(pErr->stkErrCode))==SipFail)
           {
#ifdef SDF_ERROR
               sdf_fn_setError(Sdf_en_majorErrors, \
                       Sdf_en_headerManipulationError, \
                       (Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
                       "Failed to set Option in Unsupported Header\n",pErr);
#endif
               pErr->errCode=Sdf_en_headerManipulationError;
               return Sdf_en_failed;
           }

           if(sdf_listAppend(&slUnSupportedHeadersList, pHdr, \
                       pErr) == Sdf_co_fail)
           {
#ifdef SDF_ERROR
               sdf_fn_setError( Sdf_en_majorErrors, \
                       Sdf_en_headerListAccessError,\
                       (Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
                       "Failed to append Unsupported header to HeaderList\n",pErr);
#endif
               (void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
               pErr->errCode=Sdf_en_headerListAccessError;
               return Sdf_en_failed;
           }

       } /* End of for loop iterating through list of tokens in Require header */
	

		sdf_fn_uaUpdateCallFlow(pSipMsg, Sdf_co_true);
	        
		/* 
		 * Check in case it is a response in which case no need to 
		 * send a 420 response
		 */
		
		if ((transType == Sdf_en_uasTransaction) ||
			(transType == Sdf_en_RegularUasTransaction) ||
			((transType == Sdf_en_overlapTransaction) && \
			(dType == SipMessageRequest)))
		{
			if (sdf_fn_uaRejectRequest(pCallObj,\
				pInitData,420,&pSipMsg,transType,pValidateInArg->\
				pOverlapTransaction,pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to Form 420 Response Message",pErr);
#endif
				return Sdf_en_failed;
			}
		}
        		/*
		 * Add the Unsupported headers to the 420 response message
		 */
		(void)sdf_listInitIterator(&slUnSupportedHeadersList, \
			&dListIterator, pErr);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			SipHeader *pHdr;

			pHdr = (SipHeader*)\
				(dListIterator.pCurrentElement->pData);

			if(sip_insertHeaderAtIndex(pSipMsg,\
				pHdr, unsupportedindex,(SipError*)&(pErr->stkErrCode))==\
				SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg():"
					" Failed to insert Unsupported Header at index "
					"in SipMessage",pErr);
#endif
				(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			(void)sdf_listNext(&dListIterator, pErr);
			unsupportedindex++;
		}

#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_badExtensionsError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
			"Bad Extensions in Require Header\n",pErr);
#endif
		if ( (transType == Sdf_en_overlapTransaction) ||
			 (transType == Sdf_en_RegularUasTransaction) ||
			 (transType == Sdf_en_RegularUacTransaction))
			pValidateInArg->pOverlapTransaction->pSipMsg = pSipMsg;
		else
			pValidateInArg->pTransaction->pSipMsg = pSipMsg;
	
        (void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
		pErr->errCode=Sdf_en_badExtensionsError;
		return Sdf_en_invalid;
	}	


	/* 
	 * SUPPORTED attribute is present in the user profile and Require header is
	 * also present in the incoming request. Negotiation has to be done
	 */

	/*
	 * Step 1 -> search for a wild character ' * ' in SUPPORTED list
	 */
	if(sip_listSizeOf(&(pTemp->pParam->slValue),&sizeval, \
	   (SipError*)&(pErr->stkErrCode)) ==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
			"Failed to get size of list of supported extensions\n",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_en_failed;
	}

	for(ctr=0;ctr<sizeval;ctr++)
	{
		Sdf_ty_s8bit *pTempVal;

		if(sip_listGetAt(&(pTemp->pParam->slValue),  \
			(Sdf_ty_u32bit) ctr,(Sdf_ty_pvoid *)&pTempVal,  \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError( Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Failed to get supported extension at index in extra headers"
				"list\n",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_en_failed;
		}
		if(Sdf_mc_strcasecmp("*",pTempVal)==0)
		{
			sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
				"Exiting sdf_ivk_uaHandleRequireFromMsg\n");

			pErr->errCode=Sdf_en_noUaError;

			return Sdf_en_valid;
		}
	}

	/*
	 * Step 2 -> Wild char not present in list of accepted extns.
	 * Form an Unsupported HeaderList by checking which extensions in the
	 * Require are not supported in this profile.
	 */

	/* size1 = size of require headers list */
	if(sdf_listInit(&slUnSupportedHeadersList, __sdf_fn_uaFreeCompleteHeader, \
		Sdf_co_false, pErr) == Sdf_co_fail)
			return Sdf_en_failed;

	/*
	 * Iterate through the list of extensions that have arrived in the Require
	 * header of the SIP message and check if the extensions are supported by
	 * this user profile. For every token that is not supported, form an
	 * Unsupported header. Use this list to send out the 420 Bad Extensions
	 * response message.
	 */

	for(ctr=0;ctr<size1;ctr++)
	{
		SipHeader dHdr;
		Sdf_ty_s8bit *pUnsupp, *pToken=Sdf_co_null;
		Sdf_ty_u32bit matched=0;
	
		/*
		 * Extract the Require token present in the Require header.
		 */
		if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeRequire, &dHdr, \
			(Sdf_ty_u32bit)ctr, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Failed to get Require Header at index in SipMessage\n",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}

		if(sip_getTokenFromRequireHdr(&dHdr,&pUnsupp, \
			(SipError*)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Failed to get token from Require Header\n",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}
		sip_freeSipHeader(&dHdr);

		/*
		 * Check the token in the Require against the list of tokens set in the
		 * SUPPORTED attribute of the user profile.
		 */
		for(i=0;i<sizeval;i++)
		{
			Sdf_ty_s8bit *pTempVal;

			if(sip_listGetAt(&(pTemp->pParam->slValue),  \
	        	(Sdf_ty_u32bit) i,(Sdf_ty_pvoid *)&pTempVal,  \
   		  		(SipError*)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to get token at index in Supported extension list",\
					pErr);
#endif
				pErr->errCode=Sdf_en_initDataAccessError;
				return Sdf_en_failed;
			}
			if(Sdf_mc_strcasecmp(pUnsupp,pTempVal)==0)
			{
				matched=1;
			}
		}
		/* Compare Against the global Supported timer */

#ifdef SDF_SESSION_TIMER
		if((Sdf_mc_strcasecmp("timer",pUnsupp)==0) &&\
				(sdf_fn_uaGetGlbIsSupportedTimer() == Sdf_co_true))
			matched=1;
#endif


		/* 
		 * If no match was found for this token, make an Unsupported header and
		 * add it to the list of Unsupported headers.
		 */
		/* 
		 * In case of a response we do not send a 420 Response, just 
		 * return Sdf_en_invalid instead.
		 */
		if ((matched != 1) && (transType == Sdf_en_uacTransaction))
		{
#ifdef SDF_ERROR
			sdf_fn_setError( Sdf_en_majorErrors,Sdf_en_badExtensionsError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
				"Bad Extensions in Require Header\n",pErr);
#endif
			pErr->errCode=Sdf_en_badExtensionsError;
			return Sdf_en_invalid;
		}
 
		/* We have found a unsupported/unmatched header in the uas
		 * transaction, so Continue adding Unsupported header into 
		 * the UAS transaction -*/
		if (matched != 1)
		{
			SipHeader *pHdr;

			Sdf_mc_strdup(pToken,pUnsupp);
			if(sip_initSipHeader(&pHdr,SipHdrTypeUnsupported, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to Initialize SipHeader of type Unsupported\n",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			if(sip_setOptionInUnsupportedHdr(pHdr,pToken, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to set Option in Unsupported Header\n",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			if(sdf_listAppend(&slUnSupportedHeadersList, pHdr, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError( Sdf_en_majorErrors, \
					Sdf_en_headerListAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to append Unsupported header to HeaderList\n",pErr);
#endif
				(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
				pErr->errCode=Sdf_en_headerListAccessError;
				return Sdf_en_failed;
			}
		}
		matched=0;
   	} /* End of for loop iterating through list of tokens in Require header */

 	if(sdf_listSizeOf(&slUnSupportedHeadersList, &UnsuppSize, pErr) == \
					Sdf_co_fail)
			return Sdf_en_failed;

	if (UnsuppSize==0)
	{
		/*
		 * All extensions in the Require are supported by this profile. Return
		 * with Sdf_en_valid
		 */
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaHandleRequireFromMsg");
		pErr->errCode=Sdf_en_noUaError;
		(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
		return Sdf_en_valid;	
	}
	else
	{
     
		/*
		 * One or more extensions in the Require are not supported by this user
		 * profile. The list of Unsupported extensions are present in
		 * slUnSupportedHeadersList. Form a 420 response using the same 
		 * only in case of a initial request Call.
		 */

		/* 
		 * Display incoming message in call flow (ctascii) 
		 */
		sdf_fn_uaUpdateCallFlow(pSipMsg, Sdf_co_true);
 
		/*- We are done with making a list of unsupported
		 * headers. Now prepare a 420 (Bad Extension) response
		 * and store it in the call object -*/
		if ((transType == Sdf_en_uasTransaction) ||
			(transType == Sdf_en_RegularUasTransaction) ||
			((transType == Sdf_en_overlapTransaction) && \
			(dType == SipMessageRequest)))
		{
			if (sdf_fn_uaRejectRequest(pCallObj,\
				pInitData,420,&pSipMsg,transType,pValidateInArg->\
				pOverlapTransaction,pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
					"Failed to Form 420 Response Message",pErr);
#endif
				(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
				return Sdf_en_failed;
			}
		}

		/*
		 * Add the Unsupported headers to the 420 response message
		 */
		(void)sdf_listInitIterator(&slUnSupportedHeadersList, \
			&dListIterator, pErr);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			SipHeader *pHdr;

			pHdr = (SipHeader*)\
				(dListIterator.pCurrentElement->pData);

			if(sip_insertHeaderAtIndex(pSipMsg,\
				pHdr, unsupportedindex,(SipError*)&(pErr->stkErrCode))==\
				SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg():"
					" Failed to insert Unsupported Header at index "
					"in SipMessage",pErr);
#endif
				(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			(void)sdf_listNext(&dListIterator, pErr);
			unsupportedindex++;
		}

#ifdef SDF_ERROR
		sdf_fn_setError( Sdf_en_majorErrors,Sdf_en_badExtensionsError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaHandleRequireFromMsg(): "
			"Bad Extensions in Require Header\n",pErr);
#endif
		if ( (transType == Sdf_en_overlapTransaction) ||
			 (transType == Sdf_en_RegularUasTransaction) ||
			 (transType == Sdf_en_RegularUacTransaction))
			pValidateInArg->pOverlapTransaction->pSipMsg = pSipMsg;
		else
			pValidateInArg->pTransaction->pSipMsg = pSipMsg;
		
		(void)sdf_listDeleteAll(&slUnSupportedHeadersList, pErr);
		pErr->errCode=Sdf_en_badExtensionsError;
		
		return Sdf_en_invalid;
	}
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMsgBody
**
** DESCRIPTION: This function checks the following header in the receieved
**				message
**					1. If Content-Type header is present in the message then it 
**						Checks the Media type present in the Content-Type
**						header against the List of Accepted Media present in the
**						Accept attribute of the user profile. 
**						If no match found with the Accepted Media then
**						it rejects with 415. ( Default application/sdp)
**
**					2. If Content-Encoding header is present in the message then it 
**						Checks the Encoding present in the Content-Encoding
**						header against the List of Encdoing present in the
**						Accept-Encoding attribute of the user profile
**						If no match found with the Accepted Encoding then
**						it rejects with 415. 
**
**					2. If Content-Language header is present in the message then it 
**						Checks the Language present in the Content-Language
**						header against the List of Language present in the
**						Accept-Language attribute of the user profile
**						If no match found with the Accepted Language then
**						it rejects with 415. 
**
******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMsgBody
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObj, 
Sdf_st_initData *pInitData,
Sdf_st_error *pErr)
#else
(pObj,pInitData,pErr)
Sdf_st_callObject *pObj;
Sdf_st_initData *pInitData;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_configExtraHeaders 	*pTemp =Sdf_co_null;
	Sdf_st_configExtraHeaders	*pAccept=Sdf_co_null;
	Sdf_st_configExtraHeaders	*pAcceptEnc=Sdf_co_null;
    Sdf_st_configExtraHeaders	*pAcceptLang=Sdf_co_null;
	Sdf_ty_u32bit IndicateAccept=0,IndicateAcceptEnc=0; 
	SipMessage 					*pSipMsg=Sdf_co_null;
	Sdf_ty_u32bit size2=0,dConTypeSize=0,dConEncSize=0,dIndex;
	Sdf_st_listIterator dListIterator;
	Sdf_st_overlapTransInfo		*pOverlapTxn = Sdf_co_null;
    Sdf_ty_u32bit IndicateAcceptLang=0;

/*Fix from UATK 2.2 Start*/
	Sdf_ty_u32bit ctr;
	Sdf_ty_s8bit *pUnkwn=Sdf_co_null;
    	Sdf_ty_u32bit dUnkwnSize=0;
/*Fix from UATK 2.2 End*/
	
#ifdef SDF_SUBS_PACKAGE_LAYER
    Sdf_st_msgInfo               dMsgInfo;
    Sdf_ty_bool                  dIsMsgForSubs = Sdf_co_false;
#endif    

   	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG-Entering "
		"sdf_ivk_uaIsSupportedMsgBody");	
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaIsSupportedMsgBody(): pErr param passed is invalid");
#endif
		return Sdf_en_failed;
	}
	
	if (pObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
			"pObj param is Invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}

	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
			"pInitData param is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_failed;
	}
#endif

	/*
	 * If a response has come in, avoid processing and return
	 * Sdf_en_valid
	 */
	if (pObj->pUasTransaction->pSipMsg == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaIsSupportedMsgBody");
		return Sdf_en_valid;
	}
	else
		pSipMsg = pObj->pUasTransaction->pSipMsg;
	
	/*
	 * Check if ACCEPT ACCEPT-ENCODING and ACCEPT-LANGUAGE attribute 
	 * had been set in the user profile and take those headers from the profile.
	 */

	if(sdf_listSizeOf(&(pInitData->slExtraHeadersList), &size2, pErr) == \
					Sdf_co_fail)
			return Sdf_en_failed;

	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList),&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		if(pTemp->pParam->pName==Sdf_co_null)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		if(Sdf_mc_strcasecmp("ACCEPT",pTemp->pParam->pName)==0)
		{
			pAccept=pTemp;
			IndicateAccept = 1;
		}
		if(Sdf_mc_strcasecmp("ACCEPT-ENCODING",pTemp->pParam->pName)==0)
		{
			pAcceptEnc=pTemp;
			IndicateAcceptEnc = 1;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	
#ifdef SDF_SUBS_PACKAGE_LAYER
    if(sdf_fn_uaGetMessageInfo(pSipMsg,&dMsgInfo, pErr) 
                                 != Sdf_co_success)
    {
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, 
      (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
      "Getting Message Info from SipMessage failed", pErr);
#endif
      return Sdf_en_failed;
    }

    if((dMsgInfo.dMsgType == Sdf_en_subscribe) ||
       (dMsgInfo.dMsgType == Sdf_en_notify))
    {
      /* This function fails only if the Event-header is inaccessible or it
       * is not properly formed, don't return failure here since these cases
       * will be handled in the SubsLayer.
       */
      if(sdf_fn_uaSLAmIForSubsLayer(pSipMsg,&dIsMsgForSubs,pErr)==Sdf_co_fail) 
      {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, 
        (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
        "sdf_fn_uaSLAmIForSubsLayer failed", pErr);
#endif
      }    
    }    

    /* If the dIsMsgForSubs is true , dont do the validation for accept
     * header.This will be done at call-back level by the application
     * against the list of accept as per the package.
     */
   if(dIsMsgForSubs == Sdf_co_false) 
   {    
#endif    
	/*
	 * Check if the SipMessage Has a Content-Type Header
	 */
	if(sip_getHeaderCount(pSipMsg, SipHdrTypeContentType, \
		&dConTypeSize, (SipError*)&(pErr->stkErrCode)) == SipFail)
			return Sdf_en_failed;

	/* If  Content-Type Header is present then check for vaility against
	 * the ACCEPT attribute of the profile
	 */
	if(dConTypeSize !=0)
	{
		SipHeader dHdr;
		Sdf_ty_s8bit *pMediaType=Sdf_co_null;
		Sdf_ty_u32bit dFound=0;

		if (sip_getHeader(pSipMsg, SipHdrTypeContentType, &dHdr, \
			(SipError *)&(pErr->errCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
				"Failed to get Content-Type Header from SipMessage\n",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}
		if(sip_getMediaTypeFromContentTypeHdr(&dHdr,&pMediaType,\
			(SipError *)&(pErr->errCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
				"Failed to get Media-Type  from  Content-Type header\n",pErr);
#endif
			sip_freeSipHeader(&dHdr);
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_en_failed;
		}
		sip_freeSipHeader(&dHdr);

		/* If ACCEPT attribute not present in the profile the check for default
		 * media-types.
		 */
		/* Fix for SPR 2473
		 * text/plain should be supported by default if SDF_IM is enabled
		 * message/sipfrag should be supported by default if SDF_REFER
		 * is enabled.
		 */
		if(((IndicateAccept == 0) && (Sdf_mc_strcasecmp(pMediaType,"application/sdp")==0))
#ifdef SDF_REFER
			|| (Sdf_mc_strcasecmp(pMediaType,"message/sipfrag")==0)
#endif
#ifdef SDF_IM
			|| (Sdf_mc_strcasecmp(pMediaType,"text/plain")==0)
#endif
		  )
		{
			dFound=1;
		}
		else if(IndicateAccept != 0)
		{
			Sdf_ty_u32bit sizeval=0,ctr;

#ifdef SDF_LINT
            if( pAccept == Sdf_co_null)
            {
               	pErr->errCode=Sdf_en_initDataAccessError;
                return Sdf_en_failed;
            }
            else
            {
#endif                
			if(sip_listSizeOf(&(pAccept->pParam->slValue),&sizeval, \
			   (SipError*)&(pErr->stkErrCode)) ==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to get size of list of Accepted Media type\n",pErr);
#endif
				pErr->errCode=Sdf_en_initDataAccessError;
				return Sdf_en_failed;
			}
#ifdef SDF_LINT
            }
#endif            
			for(ctr=0;ctr<sizeval;ctr++)
			{
				Sdf_ty_s8bit *pTempVal;

				if(sip_listGetAt(&(pAccept->pParam->slValue),  \
					(Sdf_ty_u32bit) ctr,(Sdf_ty_pvoid *)&pTempVal,  \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError( Sdf_en_majorErrors, \
						Sdf_en_initDataAccessError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						"Failed to get supported Media type from extra headers"
						"list\n",pErr);
#endif
					pErr->errCode=Sdf_en_initDataAccessError;
					return Sdf_en_failed;
				}
				if(Sdf_mc_strcasecmp(pMediaType,pTempVal)==0)
				{
					dFound=1;
					break;
				}
			}
		}

		/* If no match found then form 415 response */
		if(dFound !=1)
		{
			if (sdf_ivk_uaRejectRequest(pObj,\
					pInitData,415, &pOverlapTxn, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to Form 415 Response Message",pErr);
#endif
				return Sdf_en_failed;
			}
			/* Change included to correct CORE dump w.r.t CERT
			 * Testing*/ 	
			if (pOverlapTxn != Sdf_co_null)
			{		

				if(pOverlapTxn != Sdf_co_null)
				{
					if( pObj->pUasTransaction->pSipMsg != Sdf_co_null )
						sip_freeSipMessage(pObj->pUasTransaction->pSipMsg);
					pObj->pUasTransaction->pSipMsg=pOverlapTxn->pSipMsg;
					HSS_LOCKEDINCREF(pObj->pUasTransaction->pSipMsg->dRefCount);
				}
			}	
			return Sdf_en_invalid;
		}
	}
#ifdef SDF_SUBS_PACKAGE_LAYER
   }
#endif   
	/*
	 * Check if the SipMessage Has a Content-Encoding Header
	 */
	if(sip_getHeaderCount(pSipMsg, SipHdrTypeContentEncoding, \
		&dConEncSize, (SipError*)&(pErr->stkErrCode)) == SipFail)
			return Sdf_en_failed;

	/* If  Content-Encoding Header is present then check for vailidity against
	 * the ACCEPT-ENCODING attribute of the profile
	 */
	if(dConEncSize !=0)
	{
		Sdf_ty_u32bit ctr;

		/* If no Accept-Encoding attribute present then form 415 */
		if(IndicateAcceptEnc !=1)
		{
			if (sdf_ivk_uaRejectRequest(pObj,\
					pInitData,415, &pOverlapTxn, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to Form 415 Response Message",pErr);
#endif
				return Sdf_en_failed;
			}
			if (pOverlapTxn != Sdf_co_null)
			{
				if(pOverlapTxn->pSipMsg != Sdf_co_null)
				{
					if( pObj->pUasTransaction->pSipMsg != Sdf_co_null )
						sip_freeSipMessage(pObj->pUasTransaction->pSipMsg);
					pObj->pUasTransaction->pSipMsg=pOverlapTxn->pSipMsg;
					HSS_LOCKEDINCREF(pObj->pUasTransaction->pSipMsg->dRefCount);
				}
			}	
			return Sdf_en_invalid;
		}

		for(ctr=0;ctr<dConEncSize;ctr++)
		{
			SipHeader dHdr;
			Sdf_ty_s8bit *pEncoding=Sdf_co_null;
			Sdf_ty_u32bit matched=0,sizeval=0;
		
			/*
			 * Extract the Coding value present in the CONTENT-ENCODING header.
			 */
			if (sip_getHeaderAtIndex(pSipMsg, SipHdrTypeContentEncoding, &dHdr,\
				(Sdf_ty_u32bit)ctr, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to get Accept-Encoding  From SipMessage\n",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}

			if(sip_getEncodingFromContentEncodingHdr(&dHdr,&pEncoding, \
				(SipError*)&(pErr->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to get Coding from Accept-Encoding Header\n",pErr);
#endif
				sip_freeSipHeader(&dHdr);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_en_failed;
			}
			sip_freeSipHeader(&dHdr);

			/*
			 * Check the EnCoding in the Content-Encoding against the list of
			 * Codings  set in the
			 * Accept-encoding attribute of the user profile.
			 */
#ifdef SDF_LINT
            if( pAcceptEnc == Sdf_co_null)
                return Sdf_en_failed;
            else
            {
#endif
            if(sip_listSizeOf(&(pAcceptEnc->pParam->slValue),&sizeval, \
			   (SipError*)&(pErr->stkErrCode)) ==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					"Failed to get size of list of Accept-Encoding\n",pErr);
#endif
				pErr->errCode=Sdf_en_initDataAccessError;
				return Sdf_en_failed;
			}
#ifdef SDF_LINT
            }
#endif            

			for(dIndex=0;dIndex<sizeval;dIndex++)
			{
				Sdf_ty_s8bit *pTempVal;

				if(sip_listGetAt(&(pAcceptEnc->pParam->slValue),  \
					(Sdf_ty_u32bit) dIndex,(Sdf_ty_pvoid *)&pTempVal,  \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_initDataAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						"Failed to get token at index in Accept-Encoding"
					   	"extension list", pErr);
#endif
					pErr->errCode=Sdf_en_initDataAccessError;
					return Sdf_en_failed;
				}
				if(Sdf_mc_strcasecmp(pEncoding,pTempVal)==0)
				{
					matched=1;
					break;
				}
			}
			if(matched !=1)
			{
				if (sdf_ivk_uaRejectRequest(pObj,\
					pInitData,415, &pOverlapTxn, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						"Failed to Form 415 Response Message",pErr);
#endif
					return Sdf_en_failed;
				}
				if(pOverlapTxn != Sdf_co_null)
				{
					if(pOverlapTxn->pSipMsg != Sdf_co_null)
					{
						if( pObj->pUasTransaction->pSipMsg != Sdf_co_null )
							sip_freeSipMessage(pObj->pUasTransaction->pSipMsg);

						pObj->pUasTransaction->pSipMsg=pOverlapTxn->pSipMsg;
						HSS_LOCKEDINCREF(pOverlapTxn->pSipMsg->dRefCount);
					}
				}
				return Sdf_en_invalid;
			}
			matched=0;
		} /* End of for loop iterating through list of Conent-encoding header */
	}

		/* * Check if the SipMessage Has a Content-Language Header*/

	if ((sip_getHeaderCount(pSipMsg, SipHdrTypeUnknown, \
				&dUnkwnSize, (SipError*)&(pErr->stkErrCode)))==SipFail)
                      return Sdf_en_failed;

   for (ctr=0;ctr<dUnkwnSize;ctr++)
   {
	   SipHeader dHdr;
	   Sdf_ty_u32bit sizeval=0,matched=0;


	   if (sip_getHeaderAtIndex(pSipMsg,SipHdrTypeUnknown, &dHdr,\
				   (Sdf_ty_u32bit)ctr, (SipError *)&(pErr->stkErrCode)) == SipFail)
	   {
#ifdef SDF_ERROR
		   sdf_fn_setError(Sdf_en_majorErrors, \
				   Sdf_en_headerManipulationError,\
				   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
				   "Failed to get Accept-Encoding  From SipMessage\n",pErr);
#endif
		   pErr->errCode=Sdf_en_headerManipulationError;
		   return Sdf_en_failed;
	   }

	   if(sip_getNameFromUnknownHdr(&dHdr,&pUnkwn, \
				   (SipError*)&(pErr->stkErrCode))==SipFail)
	   {
#ifdef SDF_ERROR
		   sdf_fn_setError(Sdf_en_majorErrors, \
				   Sdf_en_headerManipulationError,\
				   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
				   "Failed to get Coding from Accept-Encoding Header\n",pErr);
#endif
		   sip_freeSipHeader(&dHdr);
		   pErr->errCode=Sdf_en_headerManipulationError;
		   return Sdf_en_failed;
	   }

	   if(Sdf_mc_strcasecmp(pUnkwn,"CONTENT-LANGUAGE")==0)
	   {
		   if(sip_getBodyFromUnknownHdr(&dHdr,&pUnkwn, \
					   (SipError*)&(pErr->stkErrCode))==SipFail)
		   {
#ifdef SDF_ERROR
			   sdf_fn_setError(Sdf_en_majorErrors, \
					   Sdf_en_headerManipulationError,\
					   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					   "Failed to get Coding from Accept-Encoding Header\n",pErr);
#endif
			   sip_freeSipHeader(&dHdr);
			   pErr->errCode=Sdf_en_headerManipulationError;
			   return Sdf_en_failed;
		   }
		   sip_freeSipHeader(&dHdr);


		   /*    if the accept-language is not set in the profile	*/
		   if(IndicateAcceptLang !=1)
		   {
			   if (sdf_ivk_uaRejectRequest(pObj,\
						   pInitData,415, &pOverlapTxn, pErr) == Sdf_co_fail)
			   {
#ifdef SDF_ERROR
				   sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						   "Failed to Form 415 Response Message",pErr);
#endif
				   return Sdf_en_failed;
			   }
			   return Sdf_en_invalid;
		   }

		   if(sip_listSizeOf(&(pAcceptLang->pParam->slValue),&sizeval, \
					   (SipError*)&(pErr->stkErrCode)) ==SipFail)
		   {
#ifdef SDF_ERROR
			   sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError,\
					   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
					   "Failed to get size of list of Accept-Encoding\n",pErr);
#endif
			   pErr->errCode=Sdf_en_initDataAccessError;
			   return Sdf_en_failed;
		   }
		   for(dIndex=0;dIndex<sizeval;dIndex++)
		   {
			   Sdf_ty_s8bit *pTempVal;

			   if(sip_listGetAt(&(pAcceptLang->pParam->slValue),  \
						   (Sdf_ty_u32bit) dIndex,(Sdf_ty_pvoid *)&pTempVal,  \
						   (SipError*)&(pErr->stkErrCode)) == SipFail)
			   {
#ifdef SDF_ERROR
				   sdf_fn_setError(Sdf_en_majorErrors, \
						   Sdf_en_initDataAccessError,\
						   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						   "Failed to get token at index in Accept-Encoding"
						   "extension list", pErr);
#endif
				   pErr->errCode=Sdf_en_initDataAccessError;
				   return Sdf_en_failed;
			   }
			   if(Sdf_mc_strcasecmp(pUnkwn,pTempVal)==0)
			   {
				   matched=1;
				   break;
			   }
		   }
		   if(matched !=1)
		   {
			   if (sdf_ivk_uaRejectRequest(pObj,\
						   pInitData,415, &pOverlapTxn, pErr) == Sdf_co_fail)
			   {
#ifdef SDF_ERROR
				   sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
						   (Sdf_ty_s8bit *)"sdf_ivk_uaIsSupportedMsgBody(): "
						   "Failed to Form 415 Response Message",pErr);
#endif
				   return Sdf_en_failed;
			   }

			   return Sdf_en_invalid;
		   }
		   matched=0;

	   }
	   else 
	   {	
		   sip_freeSipHeader(&dHdr);
	   }	 
   }
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaIsSupportedMsgBody");
	return Sdf_en_valid;
}

/******************************************************************************
** FUNCTION: sdf_ivk_uaCheckMessageLength
**
** DESCRIPTION: This API checks if the length of any of the header 
**				(present in SIP message) is more than the specified length.
**
** PARAMETERS:
**		pString(IN)	: Incoming SIP Message (plain text)
**		dLen(IN)	: Boundary length. All the headers in 'pString' is checked
**					  to see if any of the headers exceeds this length.
**		dOption(IN)	: Specifies, which are the headers to be tested
**		pResult(OUT): Specifies whether or not this message contains a header of
**					  length more than 'dLen'
**		pError(OUT)	: Error structure in case the function fails
**
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCheckMessageLength(Sdf_ty_s8bit *pString, 
		Sdf_ty_u32bit dLen, Sdf_ty_u32bit dOption, 
		Sdf_ty_messageValidity *pResult, Sdf_st_error *pError)
{
	Sdf_ty_s8bit *pTempStr;
	Sdf_ty_s8bit *pTempStr1;
	Sdf_ty_s8bit *pTempStr2;

	(void)dOption;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaCheckMessageLength");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_ivk_uaCheckMessageLength(): pError param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if ((pString == Sdf_co_null) || (pResult == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCheckMessageLength(): "
				"pString/pResult param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	*pResult = Sdf_en_valid;
	Sdf_mc_strdup(pTempStr, pString);
#ifdef SDF_LINT
    if(pTempStr == Sdf_co_null)
        return Sdf_co_fail;
#endif    
	pTempStr2 = pTempStr;

	while(*pTempStr != '\0')
	{
		pTempStr1 = Sdf_mc_strchr(pTempStr, '\r');
		/* Break if source string is exhausted */
		if (pTempStr1 == Sdf_co_null)
		{
			pTempStr1 = Sdf_mc_strchr(pTempStr, '\n');
			if(pTempStr1 == Sdf_co_null)
			{
				pTempStr1 = Sdf_mc_strchr(pTempStr, '\0');
			}
		}
#ifdef SDF_LINT
        if(pTempStr1 == Sdf_co_null || pTempStr == Sdf_co_null)
            return Sdf_co_fail;
        else
        {          
#endif        
        if(dLen < ((Sdf_ty_u32bit)(pTempStr1-pTempStr)))
		{
			*pResult = Sdf_en_failed;
			Sdf_mc_strcpy(pError->ErrMsg, "Sip message arrived with length > dLen");
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pTempStr2), pError);
			return Sdf_co_fail;
		}
		if(*pTempStr1 == '\0')
			pTempStr = pTempStr1;
		else
		    pTempStr = pTempStr1+1;
#ifdef SDF_LINT        
        }
#endif        
	}

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pTempStr2), pError);
	return Sdf_co_success;
}
/* IPTK CML Start */
/******************************************************************************
** FUNCTION: sdf_ivk_uaIPTKCheckMessageLength
**
** DESCRIPTION: This API checks if the length of any of the header 
**				(present in SIP message) is more than the specified length.
**
** PARAMETERS:
**		pString(IN)	: Incoming SIP Message (plain text)
**		dLen(IN)	: Boundary length. All the headers in 'pString' is checked
**					  to see if any of the headers exceeds this length.
        dOption(IN)	: Specifies, which are the headers to be tested
**		pResult(OUT): Specifies whether or not this message contains a header of
**					  length more than 'dLen'
**		pError(OUT)	: Error structure in case the function fails
**
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIPTKCheckMessageLength(Sdf_ty_s8bit *pString, 
		Sdf_ty_u32bit dLen, Sdf_ty_u32bit dOption,
        Sdf_ty_u32bit *pResult, Sdf_st_error *pError)
{
	Sdf_ty_s8bit *pTempStr;
	Sdf_ty_s8bit *pTempStr1;
	Sdf_ty_s8bit *pTempStr2;
	Sdf_ty_s8bit *phdrName;
    Sdf_ty_bool dReqRespLine = 1;
    
    (void)dOption;
    sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaIPTKCheckMessageLength");

#ifdef SDF_PARAMVALIDATION
if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_ivk_uaCheckMessageLength(): pError param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
    if (pString == Sdf_co_null)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCheckMessageLength(): "
				"pString/pResult param passed is Invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
#endif

	if((dLen == 0) || (dOption != 0))
            return Sdf_co_fail;
    *pResult = Sdf_en_validlength;
	Sdf_mc_strdup(pTempStr, pString);
#ifdef SDF_LINT
    	if(pTempStr == Sdf_co_null)
        	return Sdf_co_fail;
#endif    
	pTempStr2 = pTempStr;

	while(*pTempStr != '\0')
	{
		pTempStr1 = Sdf_mc_strchr(pTempStr, '\r');
		/* Break if source string is exhausted */
		if (pTempStr1 == Sdf_co_null)
		{
			pTempStr1 = Sdf_mc_strchr(pTempStr, '\n');
			if(pTempStr1 == Sdf_co_null)
			{
				pTempStr1 = Sdf_mc_strchr(pTempStr, '\0');
			}
		}
#ifdef SDF_LINT
        	if(pTempStr1 == Sdf_co_null || pTempStr == Sdf_co_null)
            		return Sdf_co_fail;
        	else
        	{          
#endif        
        		if(dLen < ((Sdf_ty_u32bit)(pTempStr1-pTempStr)))
			    {
				 
                    if(dReqRespLine == 1)
                        *pResult = *pResult | Sdf_en_reqresplinelong;
                       /*  *pResult = *pResult | Sdf_en_mandatoryhdrlong; */
                    else
                    {
                        phdrName = Sdf_mc_strtok(pTempStr, ":");
                        if(phdrName == Sdf_co_null)
					        return Sdf_co_fail;
				        if((Sdf_mc_strcasecmp(phdrName, "From") == 0) ||
                        (Sdf_mc_strcmp(phdrName, "f") == 0) ||        
				        (Sdf_mc_strcasecmp(phdrName, "To") == 0) ||
                        (Sdf_mc_strcmp(phdrName, "t") == 0) ||
				        (Sdf_mc_strcasecmp(phdrName, "Via") == 0) ||
                        (Sdf_mc_strcmp(phdrName, "v") == 0) ||
				        (Sdf_mc_strcasecmp(phdrName, "CallId") == 0) ||
                        (Sdf_mc_strcmp(phdrName, "i") == 0) ||
				        (Sdf_mc_strcasecmp(phdrName, "CSeq") == 0) ||
                        (Sdf_mc_strcasecmp(phdrName, "Max-Forwards") == 0))
					        *pResult = *pResult | Sdf_en_mandatoryhdrlong;
				        
                        else if(Sdf_mc_strchr(phdrName, '=') == Sdf_co_null)                  
					        *pResult = *pResult | Sdf_en_optionalhdrlong;
                        else
					        *pResult = *pResult | Sdf_en_msgbodylinelong;
		            }
                    if((*pResult & Sdf_en_mandatoryhdrlong) == Sdf_en_mandatoryhdrlong ||
                            (*pResult & Sdf_en_reqresplinelong) == Sdf_en_reqresplinelong)
                    {    
                       (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&(pTempStr2),pError);
                        return Sdf_co_success;
                    }
			    }
                dReqRespLine = 0;
			    if(*pTempStr1 == '\0')
				    pTempStr = pTempStr1;
			    else
		    		pTempStr = pTempStr1+1;
#ifdef SDF_LINT        
        	}
#endif        
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid *)&(pTempStr2),
                      pError);
    return Sdf_co_success;
}
/* IPTK CML End */
#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaValidateNotifyWrtRefer
**
** DESCRIPTION: This function validates NOTIFY Request. It checks for 
**		1.   the presence of "event" header. 
**		2.   message body of type "message/sipfrag".
**		3.   Check if first line of message body is Response Status Line.
**	If any of the above validations fails, return Sdf_co_fail.
**
** PARAMETERS:
**		pSipMsg(IN)	: The SipMessage to be validated
**		pErr(OUT)	: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaValidateNotifyWrtRefer
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg,
	 Sdf_st_error *pErr)
#else
	( pSipMsg, pErr )
	 SipMessage *pSipMsg;
	 Sdf_st_error *pErr;
#endif
{
	SipHeader 		dTempHeader;
	SipMsgBody 		*pMsgBody = Sdf_co_null;
	Sdf_ty_bool 	dSipFragBodyfound = Sdf_co_false;
	Sdf_ty_retVal 	dRetVal = Sdf_co_fail;
	Sdf_ty_u32bit	dBodyCount=0, i=0;
	Sdf_ty_s8bit	*pEventType=Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_ivk_uaValidateNotifyWrtRefer");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaValidateNotifyWrtRefer( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if (pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaValidateNotifyWrtRefer( ):  "
			"Invalid SipMessage passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	/* Check for the presence of "event" header */
	/* "Event:" header is mandatory in NOTIFY & SUBSCRIBE requests */
	if (SipFail == sip_getHeader(pSipMsg, SipHdrTypeEvent, &dTempHeader, \
				(SipError *)&(pErr->stkErrCode)))
	{
		pErr->errCode = Sdf_en_basicHeaderError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaValidateNotifyWrtRefer:"
			"Failed to get event header from Sip Message", pErr);
#endif			
			return Sdf_co_fail;
	}
	/*Get Event Type*/
	if(sip_impp_getEventTypeFromEventHdr(&dTempHeader,&pEventType,\
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		sip_freeSipHeader(&dTempHeader);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,\
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaCTisNotifyForRefer():"
			"sip_impp_getEventTypeFromEventHdr",pErr);
#endif
		pErr->errCode = Sdf_en_basicHeaderError;
		return Sdf_co_fail;
	}
	sip_freeSipHeader(&dTempHeader);
	if(pEventType == Sdf_co_null)
	{
		pErr->errCode = Sdf_en_basicHeaderError;
		return Sdf_co_fail;
	}
	if(Sdf_mc_strcasecmp(pEventType,"refer") != 0)
	{
		pErr->errCode = Sdf_en_headerListAccessError;
		return Sdf_co_fail;
	}
	
	/* Check for message body of type "message/sipfrag" */
	/*
	 * First check how man message bodies are present in SipMessage.
	 * Browse through each Message Body & check if content type is 
	 * "message/sipfrag". Once such SipMsgBody is found, exit the loop
	 */
	if (SipFail == sip_getMsgBodyCount(pSipMsg, &dBodyCount, \
			((SipError *)&(pErr->stkErrCode))))
	{
		pErr->errCode = Sdf_en_basicHeaderError;
		return Sdf_co_fail;
	}
	for (i=0; ((i<dBodyCount) && (dSipFragBodyfound==Sdf_co_false));i++)
	{

		/* Check one Message Body at a time */
		if (SipFail == sip_getMsgBodyAtIndex(pSipMsg, &pMsgBody, i, \
				((SipError *)&(pErr->stkErrCode))))
		{
			pErr->errCode = Sdf_en_basicHeaderError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaValidateNotifyWrtRefer:"
				"Failed to get MessageBody from SipMessage", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Check if the MessageBodyType == SipAppSipBody 
		 * [equivalent to "message/sipfrag" content type */
		if (pMsgBody->dType == SipAppSipBody)
		{
			dSipFragBodyfound = Sdf_co_true;
			break;
		}
		else
			sip_freeSipMsgBody(pMsgBody);
	}
	
	/* Check if the first line is status line */
	if (dSipFragBodyfound == Sdf_co_true)
	{
		SipMessage *pSipfragSipMsg = Sdf_co_null;
		
		if (SipFail == sip_getSipMessageFromMsgBody(pMsgBody, \
					&pSipfragSipMsg, ((SipError *)&(pErr->stkErrCode))))
		{
			sip_freeSipMsgBody(pMsgBody);
			pErr->errCode = Sdf_en_basicHeaderError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaValidateNotifyWrtRefer:"
				"Failed to get event header from Sip Message", pErr);
#endif			
			return Sdf_co_fail;
		}
		sip_freeSipMsgBody(pMsgBody);
		
		/* Check if it is Response SipMessage, if yes, mark
		 * Sdf_co_success as the return value */
		if (SipMessageResponse == pSipfragSipMsg->dType)
			dRetVal = Sdf_co_success;
		
		sip_freeSipMessage(pSipfragSipMsg);
	}
	/* Set the error code, if sipfrag body is not found */
	if (Sdf_co_fail == dRetVal)
		pErr->errCode = Sdf_en_basicHeaderError;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaValidateNotifyWrtRefer");
	return dRetVal;
}
#endif /* SDF_REFER */

#ifdef SDF_SUBSCRIPTION_LAYER
/******************************************************************************
 ** FUNCTION: sdf_ivk_uaIncrementDialogUsageCount
 ** 
 ** DESCRIPTION: This API increments the UsageCount of the CallObject. This
 ** count is used in cases like multiple subscriptions in one dialog or a call
 ** and subscriptions in one dialog etc., where all the subscriptions use the
 ** same CallObject. Each time a subscription or call is initiated on this 
 ** CallObject, this count must be incremented by a value of one.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** Returns Sdf_co_success if it able to increment the UsageCount, else it
 ** returns Sdf_co_fail.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN) : The CallObj whose UsageCount is to be incremented
 **   pErr(OUT)    : Error structure used to return the type of error that 
 **                  occurred.
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIncrementDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_st_error          *pErr)
{
  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
        "sdf_ivk_uaIncrementDialogUsageCount");
  void (pErr);
#ifdef SDF_PARAMVALIDATION
  if (pErr == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
       "sdf_ivk_uaIncrementDialogUsageCount(): pErr passed is Invalid");
#endif
    return Sdf_co_fail;
  }

  if (pCallObj == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                    (Sdf_ty_s8bit *)"sdf_ivk_uaIncrementDialogUsageCount(): "
                    "pCallObj passed is Invalid", pErr);
#endif
    pErr->errCode = Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
#endif /* Param-validation */

  pCallObj->dDialogUsageCount++;

  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
       "sdf_ivk_uaIncrementDialogUsageCount");

  return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaDecrementDialogUsageCount
 ** 
 ** DESCRIPTION: This API decrements the UsageCount of the CallObject. This
 ** count is used in cases like multiple subscriptions in one dialog or a call
 ** and subscriptions in one dialog etc., where all the subscriptions use the
 ** same CallObject. 
 ** Ideally, the CallObject should be removed from the 
 ** aggregation only when this count becomes zero.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** Returns Sdf_co_success if it able to decrement the UsageCount, else it
 ** returns Sdf_co_fail.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN) : The CallObj whose UsageCount is to be decremented
 **   pErr(OUT)    : Error structure used to return the type of error that 
 **                  occurred.
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDecrementDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_st_error          *pErr)
{
  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
     "sdf_ivk_uaDecrementDialogUsageCount");
  void (pErr);

#ifdef SDF_PARAMVALIDATION
  if (pErr == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
       "sdf_ivk_uaDecrementDialogUsageCount(): pErr passed is Invalid");
#endif
    return Sdf_co_fail;
  }

  if (pCallObj == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                    (Sdf_ty_s8bit *)"sdf_ivk_uaDecrementDialogUsageCount(): "
                    "pCallObj passed is Invalid", pErr);
#endif
    pErr->errCode = Sdf_en_invalidParamError;
    return Sdf_co_fail;
  }
#endif /* Param-validation */

  pCallObj->dDialogUsageCount--;

  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
    "sdf_ivk_uaDecrementDialogUsageCount");

  return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaCheckDialogUsageCount
 ** 
 ** DESCRIPTION: This API returns the number of usages the Call Object.
 ** It returns the value of 'dDialogUsageCount' from the CallObject.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** It returns Sdf_en_dialogNotUsed if dDialogUsageCount is 0, 
 ** Sdf_en_dialogOneUse if dDialogUsageCount is 1 & Sdf_en_dialogMultipleUses
 ** if dDialogUsageCount > 1. The value of pDialogUsageCount is set accordingly.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN)           : Subscription from which the key is to be obtained
 **   pDialogUsageCount(OUT) : Usage count of the dialog
 **   pErr(OUT)              : Error structure used to return the type of 
 **                            error that occurred.
 ******************************************************************************/
Sdf_ty_dialogUsage sdf_ivk_uaCheckDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_ty_u32bit         *pDialogUsageCount,
   Sdf_st_error          *pErr)
{
  Sdf_ty_dialogUsage  dRetVal = Sdf_en_dialogNotUsed;
  *pDialogUsageCount          = 0;

  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
        "sdf_ivk_uaCheckDialogUsageCount");
  void (pErr);

#ifdef SDF_PARAMVALIDATION
  if (pErr == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
       "sdf_ivk_uaCheckDialogUsageCount(): pErr passed is Invalid");
#endif

    return Sdf_en_dialogErrorOccur; 
  }

  if (pCallObj == Sdf_co_null)
  {
#ifdef SDF_ERROR
    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                    (Sdf_ty_s8bit *)"sdf_ivk_uaCheckDialogUsageCount(): "
                    "pCallObj passed is Invalid", pErr);
#endif
    pErr->errCode = Sdf_en_invalidParamError;

    return Sdf_en_dialogErrorOccur;	
  }
#endif /* Param-validation */

  /* Since dretVal is initialized, if Usage is 0, no need to set anything */
  if (pCallObj->dDialogUsageCount == 1)
  {
    dRetVal            = Sdf_en_dialogOneUse;
    *pDialogUsageCount = 1;
  }
  else if (pCallObj->dDialogUsageCount > 1)
  {
    dRetVal            = Sdf_en_dialogMultipleUses;
    *pDialogUsageCount = pCallObj->dDialogUsageCount;
  }

  sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
        "sdf_ivk_uaCheckDialogUsageCount");
  return dRetVal;
}

#endif /* SDF_SUBSCRIPTION_LAYER */

