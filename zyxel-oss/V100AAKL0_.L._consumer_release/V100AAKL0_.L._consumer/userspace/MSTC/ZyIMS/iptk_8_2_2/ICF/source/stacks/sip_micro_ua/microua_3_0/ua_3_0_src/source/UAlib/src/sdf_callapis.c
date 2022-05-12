/******************************************************************************
 ** FUNCTION:	
 **	 	This file contains all the high-level APIs required at the UAC to
 **		initiate, terminate and modify a call.
 **
 ******************************************************************************
 **
 ** FILENAME		: sdf_callapis.c
 **
 ** DESCRIPTION		: This file contains all the high-level APIs required 
 **						at the UAC to initiate, terminate and modify a call.
 **
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 ** 27/10/00		Prashant, 					Creation
 **					Subhash				
 ** 07/12/00		Subhash						Implemented use of 
 **											accessor fns 
 ** 22/04/03        Anil Kumar     ---          Changes for TLS feature.
 ** 12/05/03       Subbarao		UATK 2.1 Design      RFC 3428 feature added.
 ** 08/06/09       Anuradha Gupta  SPR 19737    Klocworks fix
 **
 ******************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf_portlayer.h"
#include "sdf_callapis.h"
#include "sdf_debug.h"
#include "sdf_accessor.h"
#include "sdf_sdp.h"
#include "sdf_init.h"
#include "sdf_trace.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_sdpaccessor.h"
#include "sdf_sdpfree.h"
#include "sdf_sdpinit.h"
#include "sdf_reqresp.h"
#include "sdf.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_list.h"
#endif

/* klocworks : NULL checks before pointer access, failure return in case of
 * memeory allocation failures, in case of failrue return, freeing the dynamically
 * allocated memory through strdup within the funcion
 */ 

/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeCall
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form an INVITE message to be sent to the peer entity to
**				start a call.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_error *pErr)
#else
	( pObject, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeCall");

	/*
	 * Invoke the sdf_ivk_uaMakeTransaction API to form the INVITE request
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INVITE",\
		pObject, Sdf_co_null, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeCall( ):  "
			"Failed to make INVITE transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeCall");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaTerminateCall
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form an BYE message to be sent to the peer entity to
**				terminate a call.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaTerminateCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_error *pErr)
#else
	( pObject, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaTerminateCall");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaTerminateCall(): pError Param passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaTerminateCall( ):  "
			"Invalid parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/* 
	 * Increment the CSeq
	 */
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_commonInfoAccessError, \
		(Sdf_ty_s8bit *)"sdf_ivk_uaTerminateCall( ):  \
		Errorgetting the common info from the call object",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * Set dLocalCseq to Max(dLocalCseq, dLocalRegCseq)+1
	 */
	if (pCommonInfo->dLocalRegCseq > pCommonInfo->dLocalCseq)
		pCommonInfo->dLocalCseq = pCommonInfo->dLocalRegCseq + 1;
	else	
		pCommonInfo->dLocalCseq++ ;

	/* Free the local reference */
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

	/*
	 * Form a BYE request by invoking sdf_ivk_uaFormRequest
	 */
	if (sdf_ivk_uaFormRequest((Sdf_ty_s8bit*)"BYE", pObject, \
		Sdf_co_null, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaTerminateCall( ):  "
			"Forming BYE failed.",pErr);
#endif
		return Sdf_co_fail;
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaTerminateCall");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeRegister
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a REGISTER message to be sent to the registrar
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeRegister
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit *pExpiry,
	 Sdf_ty_slist *pslContactList,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr)
#else
	( pObject, pExpiry, pslContactList, pOverlapTransInfo, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit *pExpiry;
	 Sdf_ty_slist *pslContactList;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_st_error *pErr;
#endif
{
	SipMessage			*pSipMsg = Sdf_co_null;
	Sdf_st_transportInfo *pRegistrarInfo;
#ifdef SDF_TLS
	Sdf_ty_bool         dCorrectContactFound = Sdf_co_false;
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
		
		en_AddrType         dAddrType;
#else
    en_AddrType         dAddrType, dContactAddrType = SipAddrSipUri;
#endif
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeRegister");

	/* Check if Registrar address is configured or not */
	if (sdf_ivk_uaGetRegistrarInfoFromInitData(pObject->pInitData,\
			&pRegistrarInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
			"Failed to get Registrar Information",pErr);
#endif
		return Sdf_co_fail;
	}
	if(pRegistrarInfo->pIp == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
			"Failed to get Registrar Information",pErr);
#endif
		(void)sdf_ivk_uaFreeTransportInfo(pRegistrarInfo);
		return Sdf_co_fail;
	}
	(void)sdf_ivk_uaFreeTransportInfo(pRegistrarInfo);

	/*
	 * Form the REGISTER by invoking sdf_ivk_uaMakeTransaction
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"REGISTER",\
		pObject, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
			"Failed to make REGISTER transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Add an Expires header with the value passed to this function
	 */
	if ( (pExpiry != Sdf_co_null) && (Sdf_mc_strcmp(pExpiry, "") != 0) )
	{
		Sdf_st_transaction *pTransaction = Sdf_co_null;
		Sdf_ty_s8bit dExpiresHeader[Sdf_co_characterBufferSize] = "Expires:";

		/*
		 * If the expiry is in seconds, we do a parameter validation check
		 * to see if the user has entered has passed a negative value. If
		 * this check is omitted, then the sip_setHeaderFromStringAtIndex
		 * wraps the negative value around the MAX_LENGTH defined in the
		 * stack.
		 */
		if (Sdf_mc_strstr(pExpiry, "-") != Sdf_co_null)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit*)" sdf_ivk_uaMakeRegister(): "
				"Negative value in Expires value passed.", pErr);
#endif			
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
	
		Sdf_mc_strcat(dExpiresHeader, pExpiry);

		if( pOverlapTransInfo == Sdf_co_null )
		{
			if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
				Sdf_en_uacTransaction, &pTransaction,pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit*)" sdf_ivk_uaMakeRegister(): "
					"Failed to get UAC transaction from call object", pErr);
#endif			
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
			pSipMsg = pTransaction->pSipMsg;
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
		}
		else
			pSipMsg = pOverlapTransInfo->pSipMsg;


#if defined(MICROUA) && !defined(SIP_DATE)
		if(sip_setHeaderFromStringAtIndex(pSipMsg, \
			SipHdrTypeExpires, dExpiresHeader, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
#else
		if(sip_setHeaderFromStringAtIndex(pSipMsg, \
			SipHdrTypeExpiresAny, dExpiresHeader, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
#endif
		{
			pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
				"Forming Expires Header failed",pErr);
#endif
			return Sdf_co_fail;
		}
		/* Free the local reference */
	}

	/*
	 * The SIP REGISTER message is ready at this stage. If, however, a list 
	 * of contacts is provided as an input parameter to this API, we should
	 * replace the Contacts in the REGISTER message with this list. This is
	 * used, for example, for third-party registration, where the list of
	 * Contacts of the party to be registered is passed as a parameter to 
	 * this API.
	 */

	if (pslContactList != Sdf_co_null)
	{
		Sdf_st_transaction *pTransaction = Sdf_co_null;
		Sdf_ty_u32bit i = 0, dSize = 0;
		Sdf_st_listIterator dListIterator;

		(void)sdf_listSizeOf(pslContactList, &dSize, pErr);

		if (dSize > 0)
		{
			if( pOverlapTransInfo == Sdf_co_null )
			{
				if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
					Sdf_en_uacTransaction, &pTransaction,pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_callObjectAccessError, \
						(Sdf_ty_s8bit*)" sdf_ivk_uaMakeRegister(): "
						"Failed to get UAC transaction from call object", pErr);
#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
					return Sdf_co_fail;
				}
				pSipMsg = pTransaction->pSipMsg;
		
				(void)sdf_ivk_uaFreeTransaction(pTransaction);
			}
			else
				pSipMsg = pOverlapTransInfo->pSipMsg;

#ifdef SDF_TLS
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
 if (Sdf_co_fail == sdf_fn_uaGetContactAddrType(pSipMsg, pObject, \
				&(pObject->pCallInfo->dContactAddrType), SipMessageRequest, \
				pErr))
#else
			if (Sdf_co_fail == sdf_fn_uaGetContactAddrType(pSipMsg, pObject, \
				&dContactAddrType, SipMessageRequest, pErr))
#endif                
			{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_callObjectAccessError, \
						(Sdf_ty_s8bit*)" sdf_ivk_uaMakeRegister(): "
						"Failed to get Contact address type from SipMsg.", pErr);
#endif			
					pErr->errCode=Sdf_en_callObjectAccessError;
					return Sdf_co_fail;
			}
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
            
			pObject->pCallInfo->dContactAddrType = dContactAddrType;
#endif /*End :  changes done by subbaroa*/            
#endif /* SDF_TLS */

			if(sip_deleteAllHeaderType(pSipMsg, SipHdrTypeContactAny, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
					"Failed to delete existing contact headers",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			(void)sdf_listInitIterator(pslContactList, &dListIterator, pErr);

			while (dListIterator.pCurrentElement != Sdf_co_null)
			{
				SipHeader *pContactHdr = Sdf_co_null;

				pContactHdr = (SipHeader*)\
					(dListIterator.pCurrentElement->pData);

				if (pContactHdr == Sdf_co_null)
				{
					(void)sdf_listNext(&dListIterator, pErr);
					i++;
					continue;
				}
				/*  Allow * value only when expiry is 0*/
#ifdef SDF_PARAMVALIDATION
				if (pContactHdr->dType == SipHdrTypeContactWildCard)
				{
					if (Sdf_mc_strcmp(pExpiry, "0") != 0)
					{
						/* 
						 * wild-card '*' is allowed in contact list only 
						 * if expires header has been specified as '0'
						 */
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_invalidParamError, \
							(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
							"Wild-card \'*\' present in contact list but "
							"expires header is not specified as 0",pErr);
#endif
						pErr->errCode=Sdf_en_invalidParamError;
						return Sdf_co_fail;
					}
				}
#endif

#ifdef SDF_TLS
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
if (pObject->pCallInfo->dContactAddrType.dProtocolScheme == \
						SipAddrSipSUri)

#else
				if (dContactAddrType == SipAddrSipSUri)
#endif                
				{
					dAddrType = ((SipContactHeader *)\
				            (pContactHdr->pHeader))->pAddrSpec->dType;
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
if (dAddrType != \
						pObject->pCallInfo->dContactAddrType.dProtocolScheme)

#else
					if (dAddrType != dContactAddrType)
#endif                    
					{
						(void)sdf_listNext(&dListIterator, pErr);
						continue;
					}
					else
						dCorrectContactFound = Sdf_co_true;
				}
#endif /* SDF_TLS */

				if (sip_insertHeaderAtIndex(pSipMsg, pContactHdr, \
					(Sdf_ty_u32bit)i, (SipError *)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRegister( ):  "
						"Failed to insert a contact header in the SipMessage", \
						pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				(void)sdf_listNext(&dListIterator, pErr);
				i++;
			}
		}
	}

#ifdef SDF_TLS
	/* Check if the correct Contact header has been found and added. */
#ifdef SDF_UASSF_ONLY
/*If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
	if (pObject->pCallInfo->dContactAddrType.dProtocolScheme == SipAddrSipSUri)
#else
	if (dContactAddrType == SipAddrSipSUri)
#endif    
	{
		if (!dCorrectContactFound)
		{
			pErr->errCode = Sdf_en_noSipSUriInContact;
			return Sdf_co_fail;
		}
	}
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeRegister");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

#ifdef SDF_IM
/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeMessage
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a MESSAGE Request.  If application provides Expires
**				and/or Date values, it forms Expire and/or Date headers and
**				sets them to the formed Request.
**				The date should be of <Sat, 13 Nov 2001 23:29:00 GMT> format.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeMessage
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_ty_s8bit *pExpiry,
	 Sdf_ty_s8bit *pDate,
	 Sdf_st_error *pErr)
#else
	( pObject, pOverlapTransInfo, pExpiry, pDate, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_ty_s8bit *pExpiry;
	 Sdf_ty_s8bit *pDate;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeMessage");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMakeMessage(): pError Param passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeMessage( ):  "
			"Invalid parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pOverlapTransInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeMessage( ):  "
			"Invalid parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Form the MESSAGE by invoking sdf_ivk_uaMakeTransaction
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"MESSAGE",\
		pObject, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeMessage(): " \
			"Failed to make Message transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Add an Expires header with the value passed to this function
	 */
	if((pExpiry != Sdf_co_null) && (Sdf_mc_strcmp(pExpiry, "") != 0))
	{
		Sdf_ty_s8bit dExpiresHeader[Sdf_co_characterBufferSize] = "Expires:";

		/*
		 * If the expiry is in seconds, we do a parameter validation check
		 * to see if the user has entered has passed a negative value. If
		 * this check is omitted, then the sip_setHeaderFromStringAtIndex
		 * wraps the negative value around the MAX_LENGTH defined in the
		 * stack.
		 */
		if (Sdf_mc_strstr(pExpiry, "-") != Sdf_co_null)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit*)" sdf_ivk_uaMakeMessage(): "
				"Negative value in Expires value passed.", pErr);
#endif			
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
		Sdf_mc_strcat(dExpiresHeader, pExpiry);
#if defined(MICROUA) && !defined(SIP_DATE)
		if(sip_setHeaderFromStringAtIndex(pOverlapTransInfo->pSipMsg, \
			SipHdrTypeExpires, dExpiresHeader, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
#else
		if(sip_setHeaderFromStringAtIndex(pOverlapTransInfo->pSipMsg, \
			SipHdrTypeExpiresAny, dExpiresHeader, 0, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
#endif
		{
			pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeMessage( ):  "
				"Forming Expires Header failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}

	/*
	 * Add an Date header with the value passed to this function
	 */
	if ( (pDate != Sdf_co_null) && (Sdf_mc_strcmp(pDate, "") != 0) )
	{
		Sdf_ty_s8bit dDateHeader[Sdf_co_characterBufferSize] = "Date:";

		Sdf_mc_strcat(dDateHeader, pDate);

		if(sip_insertHeaderFromStringAtIndex(pOverlapTransInfo->pSipMsg, \
					SipHdrTypeUnknown, dDateHeader, 0, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeMessage( ):  "
					"Forming Date Header failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeMessage");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
#endif

/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeOptions
**
** DESCRIPTION: Invokes core stack APIs to form a OPTIONS message to be 
**				sent to the peer entity
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeOptions
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr)
#else
	( pObject, pOverlapTransInfo, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeOptions");

	/*
	 * Form an OPTIONS request using sdf_ivk_uaMakeTransaction
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"OPTIONS", \
		pObject, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeOptions( ):  "
			"Failed to make OPTIONS transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeOptions");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeInfo
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a INFO message to be sent to the peer entity
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeInfo
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr)
#else
	( pObject, pOverlapTransInfo, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeInfo");

	/*
	 * Form an INFO by invoking sdf_ivk_uaMakeTransaction
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INFO",\
		pObject, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeInfo( ):  "
			"Failed to make INFO transaction",pErr);
#endif
		return Sdf_co_fail;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeInfo");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

#ifdef SDF_PRIVACY
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormAnonymousFromHdrForPrivacy
**
** DESCRIPTION:This API constructs a Anonymous from header on request of user
**  It takes the Display name and address spec from the application and sets it 
**  into the Sip Message
**
** PARAMETERS:
**	  pSipMsg(IN)	: The SipMessage in which Anonymous FROM header is to be set
**    pFromHeaderStr(IN): The Address String Anonymous FROM  header taken 
**                           from the application .
**	  pErr(OUT)		: Error structure in case the function fails
** 
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFormAnonymousFromHdrForPrivacy 
(Sdf_st_callObject  *pObject, 
 Sdf_ty_s8bit	    *pFromHeaderStr,
 Sdf_st_error       *pErr)
{

    SipHeader           *pHdr = Sdf_co_null;
    Sdf_st_commonInfo 	*pCommonInfo = Sdf_co_null;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaFormAnonymousFromHdrForPrivacy");
#ifdef SDF_PARAMVALIDATION  
    if(pObject == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaFormAnonymousFromHdrForPrivacy( ): pObject Param " \
                "Passed is Invalid");
#endif
        return Sdf_co_fail;
    }

    if(pFromHeaderStr == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaFormAnonymousFromHdrForPrivacy( ):" \
                "pFromHeaderStr Param Passed is Invalid");
#endif
        return Sdf_co_fail;
    }
    if(pErr == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaFormAnonymousFromHdrForPrivacy( ):" \
                "pErr Param Passed is Invalid");
#endif
        return Sdf_co_fail;
    }

#endif

    if (SipFail == sip_initSipHeader(&pHdr, SipHdrTypeAny,\
                (SipError *)&pErr->stkErrCode))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertFormAnonymousFromHeader:"
                "Failed to init 'From' header ", pErr);
#endif			
        return Sdf_co_fail;
    }

    if(sip_parseSingleHeader(pFromHeaderStr,SipHdrTypeFrom, pHdr,\
                (SipError *) &(pErr->stkErrCode))==SipFail)
    {
        pErr->errCode=Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaFormAnonymousFromHdrForPrivacy():  "
                "parsing From header failed ",pErr);
#endif
        sip_freeSipHeader(pHdr);
        (void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
        return Sdf_co_fail;
    }
    /* Copy the newly formed Anonymous From header into the 
     * CommonInfo structure.
     */
    if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, pErr)\
            == Sdf_co_fail)
    {
        pErr->errCode=Sdf_en_callObjectAccessError;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaformAnonymousFrom header:  "
                "Error getting CommonInfo from call object.",pErr);
#endif
        sip_freeSipHeader(pHdr);
        (void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
        (void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
        return Sdf_co_fail;
    }

    /*
     * Free the Original (Profile) From header of commonInfo and
     * Copy the Newly formed FROM header into the Form header of
     * commonInfo. The request will have this from header only.
     */

    if(pCommonInfo->pFrom != Sdf_co_null)
    {   
        sip_freeSipHeader(pCommonInfo->pFrom);
        (void)sdf_memfree(Sdf_mc_freeMemId, \
                          (Sdf_ty_pvoid *)&(pCommonInfo->pFrom), \
                          (Sdf_st_error*)Sdf_co_null);
    }

    pCommonInfo->pFrom = pHdr;

    (void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaFormAnonymousFromHdrForPrivacy:");

    return Sdf_co_success;
}

#endif

/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeTransaction
**
** DESCRIPTION: This function prepares a SIP transaction message.
**				Methods that are supported are REGISTER, OPTIONS, 
**				INFO, PRACK, COMET, SUBSCRIBE and NOTIFY. The API does any 
**				internal handling that is required before calling 
**				sdf_ivk_uaFormRequest API. 
**				The prepared SIP message is inserted into the pOutgoingMsg
**				structure of the input call object.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeTransaction
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit *pMethod,
	Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_st_error *pErr)
#else
	( pMethod, pObject, pOverlapTransInfo, pErr )
	 Sdf_ty_s8bit *pMethod;
	 Sdf_st_callObject *pObject;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_st_appData 	*pAppData = Sdf_co_null;
	SipMessage		*pSipMessage = Sdf_co_null;
	Sdf_st_commonInfo 	*pCommonInfo = Sdf_co_null;
	Sdf_ty_transactionType	dTransactionType= Sdf_en_InvalidTransaction;
	Sdf_ty_messageType 		dMsgType		= Sdf_en_unknownRequest;
	Sdf_st_msgInfo		dMessageInfo;
#ifdef SDF_TLS
	SipAddrSpec *pRequestAddrSpec=Sdf_co_null;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG-Entering sdf_ivk_uaMakeTransaction");
	
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaMakeTransaction( ): pError Param Passed is Invalid");
#endif
		return Sdf_co_fail;
	}

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#if 0
#ifdef SDF_LINT
	if(pOverlapTransInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
			"Invalid overlap transaction info parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
#endif	
#endif

/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pErr);

	dMessageInfo.dMsgType 	= dMsgType;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dResponse	= 0;
	dMessageInfo.pMethod = pMethod;

	dTransactionType = sdf_fn_getTransactionType( Sdf_co_null, dMessageInfo, \
						   		Sdf_co_false, pErr);
	/* 
	 * First check if a call exists for this callObject. 
	 * If so, the CallId is reused. Else a new CallId is generated
	 */
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, pErr)\
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
			"Error getting CommonInfo from call object.",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

    if (dMsgType == Sdf_en_invite)
    {
        /*
         * We are starting a new INVITE transaction, so reset the CommonInfo's
         * last received Rseq to 0. This is done so that the peer can reuse 
         * the same Rseq no. (in RPRs) for a new transaction inside the call.
         */
        pCommonInfo->dRemoteRseq.dRseq = 0;
    }

	if (pObject->dCallObjectType == Sdf_en_NewCallObject)
	{
		/* 
		 * As the dCallObjectType is Sdf_en_NewCallObject, we need to create
		 * the Call-id, initilaze the pCommonInfo->pFrom header and generate
		 * the call leg key.
		 */
        if ( (dTransactionType == Sdf_en_RegularUacTransaction) && 
							(pOverlapTransInfo != Sdf_co_null))
			pOverlapTransInfo->dLocalCseq = pCommonInfo->dLocalCseq;
        
		/*
		 * Generate the Call-ID by passing the user profile and application
		 * data of this call object to the call-id generator function
		 */
#ifdef SDF_UASSF_ONLY        
        /*SPR-4632 :SSF wants its own callid generation method. So they want relaxation that
         * when callid is null then only do the UATK level call id generation function.
         * Otherwise use the SSF given call-id.*/
/* by JK*/
		if( pCommonInfo->pCallid == Sdf_co_null )
		{
#endif
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
		if (sdf_ivk_uaGetAppDataFromCallObject(pObject, &pAppData, pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Error accessing application data from the call object",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			return Sdf_co_fail;
		}
#endif /*End : changes done by subbaroa */		        
#ifdef SDF_UASSF_ONLY  
        } /*SPR-4632 : by JK*/
#endif
		if ((sdf_ivk_uaGetInitDataFromCallObject(pObject,&pInitData, \
			pErr)) == Sdf_co_fail)
		{
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/        
			(void)sdf_ivk_uaFreeAppData(pAppData);
#endif /*End : changes done by subbaroa */            
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Error accessing user profile from the call object",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
#ifdef SDF_UASSF_ONLY
        /*SPR-4632 : SSF wants its own callid generation method. So they want relaxation that
         * when callid is null then only do the UATK level call id generation function.
         * Otherwise use the SSF given call-id.*/
    /* by jk*/
		if( pCommonInfo->pCallid == Sdf_co_null )
		{
#endif
#ifndef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

		pCommonInfo->pCallid = sdf_ivk_uaGenerateCallid(pInitData, pAppData);

		if( pCommonInfo->pCallid == Sdf_co_null )
		{
			(void)sdf_ivk_uaFreeAppData(pAppData);
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			pErr->errCode = Sdf_en_callBackError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Error in Generating the Call ID",pErr);
#endif
			return Sdf_co_fail;
		}
		
		/* Freeing the local reference */
		(void)sdf_ivk_uaFreeAppData(pAppData);
#endif /*End : changes done by subbaroa */	        
#ifdef SDF_UASSF_ONLY 
    } /* SPR-4632 :by jk*/
#endif
#ifdef SDF_PRIVACY 
     /*
     * Check the Privacy flag in the Callobject is set or not, 
     * if it is False then copy From header of pInitData into From hdr
     * of CommonInfo. (Also means that this is not a privacy call).
     * If the flag is true then we conclude that user needs privacy 
     * service for this call and had given the anonymous From header,
     * and also the Anonymous From header is already copied into From 
     * header of CommonInfo in the API sdf_ivk_uaFormAnonymousFromHeader
     */
 
    if(pObject->bIsPrivacy == Sdf_co_false)
    {
#endif
		/* 
		 * Make a deep copy of the From Information From the user profile 
		 * into the call object.
		 */
		if (sip_cloneSipHeader(pCommonInfo->pFrom, pInitData->pFrom, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Error in cloning the From information from user profile",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
#ifdef SDF_PRIVACY
    }
#endif
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/

if( pCommonInfo->pCallid == Sdf_co_null )
		{
			/*
			 * Generate the Call-ID by passing the user profile and application
			 * data of this call object to the call-id generator function
			 */
			if (sdf_ivk_uaGetAppDataFromCallObject(pObject, &pAppData, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
					"Error accessing application data from the call object",\
					pErr);
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				return Sdf_co_fail;
			}

			pCommonInfo->pCallid = \
					sdf_ivk_uaGenerateCallid(pInitData, pAppData);

			if( pCommonInfo->pCallid == Sdf_co_null )
			{
				(void)sdf_ivk_uaFreeAppData(pAppData);
				(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
				(void)sdf_ivk_uaFreeInitData(pInitData);
				pErr->errCode = Sdf_en_callBackError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
					"Error in Generating the Call ID",pErr);
#endif
				return Sdf_co_fail;
			}
			(void)sdf_ivk_uaFreeAppData(pAppData);
		}

#endif /*End : changes done by subbaroa */
		/* Free the local reference */
		(void)sdf_ivk_uaFreeInitData(pInitData);
	}
	else
	{
		/* We first check for the correctness of CSeq value*/
		if (pCommonInfo->dLocalCseq > Sdf_mc_maxCseqValue)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_cseqValueExceeded, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Cseq Error - Cseq value exceeded (2**31-1).",pErr);
#endif
			pErr->errCode=Sdf_en_cseqValueExceeded;
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
			return Sdf_co_fail;
		}

		/* 
		 * For PRACK/COMET/Regular Transactions:
		 * Set dLocalRegCseq to Max(dLocalCseq, dLocalRegCseq) + 1.
		 * Update the corresponding Sdf_st_overlapTransInfo structure's dCseq
		 * Use dLocalRegCseq as the Cseq
		 *
		 * For other requests, use the dLocalCseq. The dLocalCseq is already
		 * updated at this stage with the Cseq to be sent out in the request
		 *
		 */
#ifdef SIP_RPR
		if ( (dMsgType == Sdf_en_prack) || \
			 (dMsgType == Sdf_en_comet) || \
			 (dTransactionType == Sdf_en_RegularUacTransaction))
#else
		if ( (dMsgType == Sdf_en_comet) || \
			 (dTransactionType == Sdf_en_RegularUacTransaction))
#endif /* SIP_RPR */
		{
			if (pCommonInfo->dLocalRegCseq > pCommonInfo->dLocalCseq)
				pCommonInfo->dLocalRegCseq += 1;
			else
				pCommonInfo->dLocalRegCseq = pCommonInfo->dLocalCseq + 1;

			if(pOverlapTransInfo != Sdf_co_null)
				pOverlapTransInfo->dLocalCseq = pCommonInfo->dLocalRegCseq;
        
		}
		else
		{
			/* 
			 * For other requests: 
			 * Set dLocalCseq to Max(dLocalCseq, dLocalRegCseq)+1 
			 */
			if ( dMsgType != Sdf_en_ack )
			{
				if (pCommonInfo->dLocalRegCseq > pCommonInfo->dLocalCseq)
					pCommonInfo->dLocalCseq = pCommonInfo->dLocalRegCseq + 1;
				else	
					pCommonInfo->dLocalCseq++ ;
			}
		}
	}

	/*
	 * Add any Extra headers that need to be added to the request (This uses
	 * the information set in the user profile). Thereafter, form the request
	 * message.
	 */
	if(sdf_ivk_uaAddExtraHeaders(pObject, pOverlapTransInfo, pMethod, \
		pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
			"Adding extra headers failed\n",pErr);
#endif
		return Sdf_co_fail;
	}
	
	if(sdf_ivk_uaFormRequest(pMethod, pObject, pOverlapTransInfo, pErr) \
		== Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
			"Forming request failed",pErr);
#endif
		return Sdf_co_fail;
	}

	if (pObject->dCallObjectType == Sdf_en_NewCallObject)
	{
		/*
		 *  Enter here only if the request being made is for a fresh call
		 *  or an OPTIONS/REGISTER etc outside of a call.
		 *
		 *	Each call-leg is identified using a Key composed of 
		 *	From, To, Callid. Generate it here.
		 */

#ifdef SDF_TLS
		SipReqLine          *pRequest = Sdf_co_null;
#endif

      
		if( (dTransactionType == Sdf_en_RegularUacTransaction) && \
						(pOverlapTransInfo != Sdf_co_null))
			pSipMessage = pOverlapTransInfo->pSipMsg;
		else
			pSipMessage = pObject->pUacTransaction->pSipMsg;
        
#ifdef SDF_TLS
		if(sip_getReqLineFromSipReqMsg(pSipMessage, \
			&pRequest, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaMakeTransaction( ):"
				"Failed to get Request Line from Sip Message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
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

		if (pRequestAddrSpec->dType == SipAddrSipSUri)
		{
			/* Set the secure flag. */
			pObject->pCallInfo->dSecureCall = Sdf_co_true;
		}

		/* Free the local reference here. */
		 sip_freeSipAddrSpec(pRequestAddrSpec); 
		sip_freeSipReqLine(pRequest);
#endif /* SDF_TLS */

		if ((sdf_fn_uaGenerateCallLegKey( pSipMessage, \
				&(pCommonInfo->pKey), pErr)) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMakeTransaction( ):  "
				"Generation of the call-leg key failed",pErr);
#endif
			return Sdf_co_fail;
		}

		/* Set the dCaller to Sdf_en_caller */
		pCommonInfo->pKey->dCaller = Sdf_en_caller;

		/* Set the Call object flag to Permanent */
		pObject->dCallObjectType = Sdf_en_PermanentCallObject;
	}

	/* Free the local reference */
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_ivk_uaMakeTransaction");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaCancelCall
**
** DESCRIPTION: Invokes core stack APIs to form a CANCEL message and
** 				inserts it into the input call object
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaCancelCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_error *pErr)
#else
	( pObject, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaCancelCall");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaCancelCall( ): Invalid pError parameter passed to "
			"the function");
#endif
		return Sdf_co_fail;
	}

	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCancelCall( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Form a CANCEL request by invoking sdf_ivk_uaFormRequest
	 */
	if(sdf_ivk_uaFormRequest((Sdf_ty_s8bit*)"CANCEL", pObject, \
		Sdf_co_null, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCancelCall( ):  "
			"Forming the CANCEL request failed",pErr);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaCancelCall");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaReinvite
 **
 ** DESCRIPTION: This function updates the call object and prepares an 
 **				 INVITE message with new media parameters which are passed
 **				 to the API as a parameter.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaReinvite
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	Sdf_ty_slist slSdpMediaList,
	 Sdf_st_error *pErr)
#else
	( pObject, slSdpMediaList, pErr )
	Sdf_st_callObject *pObject;
	Sdf_ty_slist slSdpMediaList;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit dSize=0, i=0, dNumStreams=0;
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	SdpMedia   *pSdpMedia = Sdf_co_null;
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaReinvite");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -  sdf_ivk_uaReinvite():"
			" Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Initiating Offer for sending ReInvite Failed.",pErr);
#endif
		return Sdf_co_fail;
	}

	/* First free the existing MediaStreams */
    dNumStreams = pSessionParams->dNumMediaStreams;
	
	for (i = 0; i < dNumStreams; i++)
	{
			(void)sdf_ivk_uaFreeMediaStream(pSessionParams->pMediaStreams[i]);
	}

	(void)sdf_memfree(Sdf_mc_sdpMemId, (Sdf_ty_pvoid*)&pSessionParams->pMediaStreams, \
			    				pErr);

	/* Now allocate memory for new MediaStreams */
	(void)sdf_listSizeOf(&(slSdpMediaList), &dSize, pErr);

	pSessionParams->pMediaStreams = (Sdf_st_mediaStream**)sdf_memget(\
			Sdf_mc_sdpMemId, sizeof(Sdf_st_mediaStream*)*dSize, pErr);

	if(Sdf_co_null == pSessionParams->pMediaStreams)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
				"memory allocation failure.",pErr);
#endif

		pErr->errCode = Sdf_en_noMemoryError;
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}

	/* Allocate Memory for Individual Pointers and form the MediaStream */
	for(i=0; i<dSize; i++)
	{
		/*Init MediaStreams*/
		if(sdf_ivk_uaInitMediaStream(&(pSessionParams->pMediaStreams[i]),\
				pErr)==Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite : "
				"Failed to Init MediaStream",pErr);
#endif	
			return Sdf_co_fail;
		}

        if (sdf_listGetAt(&(slSdpMediaList), i,\
							(Sdf_ty_pvoid *)&pSdpMedia, pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			pErr->errCode = Sdf_en_sdpAccessorError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite : "
				"Failed to get SdpMedia at index from slSdpMediaList",pErr);
#endif	
			return Sdf_co_fail;
		}

		/* Fill the MediaStream Structure with the SdpMedia Info */
		if(sdf_ivk_uaFormMediaStreamFromSdpMedia(pSessionParams->pMediaStreams[i],
					pSdpMedia, pErr)==Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
			pErr->errCode = Sdf_en_sdpError;
#ifdef SDF_ERROR			
		    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			    (Sdf_ty_s8bit*)"sdf_ivk_uaReinvite : "
			    "Failed to Form MediaStream Structure",pErr);
#endif	
		    return Sdf_co_fail;
		}
	}

	pSessionParams->dNumMediaStreams = dSize;

	/* Make INVITE Transaction */
	if( sdf_ivk_uaMakeTransaction( (Sdf_ty_s8bit *)"INVITE", pObject, \
			Sdf_co_null, pErr) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite( ):  "
			"Failed to Make Invite Transaction.",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pErr->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite ( ): "
			"Failed to allocate memory for the SDP message", pErr);
#endif		
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Failed to make SdpMessage from SessionParams",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }
	
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams,pErr) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Failed to Set Last Offer Made",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}
	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	/*
	 * Form the SipMsgBody with the SdpMessage just constructed.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pErr->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite ( ): "
			"Failed to allocate memory for the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   ((SipError*)&pErr->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite ( ): "
			"Failed to set the SDP message in the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	sip_freeSdpMessage(pSdpMessage);
	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
			Sdf_en_uacTransaction,&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaReinvite ( ): "
			"Failed to get UAC transaction from call object", pErr);
#endif			
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pErr) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Failed to delete the current message bodies.",pErr);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Failed to append message body to slMsgBodyList",pErr);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pErr) == \
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaReinvite ( ):  "
			"Failed to set sdp into the SipMessage being made.",pErr);
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaReinvite");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaAddExtraHeaders
 **
 ** DESCRIPTION: This function adds any extra headers that are set in the user
 **				 profile, into the call object's Processed and Unprocessed 
 **				 header list before calling the sdf_ivk_uaFormRequest API.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddExtraHeaders
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_ty_s8bit *pMethod,
	Sdf_st_error *pErr)
#else
	(pObject, pOverlapTransInfo, pMethod, pErr)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_s8bit *pMethod;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;
	Sdf_ty_slist *pUnprocessedHeaderList=Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	Sdf_st_headerList *pAllowHdrList=Sdf_co_null;
	Sdf_ty_bool       dReplacesCheck=Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddExtraHeaders");
	
	(void)pMethod;
	
#ifdef SDF_PARAMVALIDATION
	if (pErr==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddExtraHeaders(): pError passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if((pObject==Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
			"Call object parameter passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	if (Sdf_mc_strcmp(pMethod, "ACK") == 0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders():  "
			"Extra headers should not be added to ACK request",pErr);
#endif
		pErr->errCode=Sdf_en_noUaError;
		return Sdf_co_success;
	}
	if ((sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
			pErr)) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
			"Error accessing the initData from the call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Set the pProcessedHeaderList and pUnprocessedHeaderList to the correct
	 * list depending on whether pOverlapTransInfo is Sdf_co_null
	 */
	if(pOverlapTransInfo == Sdf_co_null)
	{
		pProcessedHeaderList = &(pObject->pUacTransaction->slProcessedHeaders);
		pUnprocessedHeaderList = \
			&(pObject->pUacTransaction->slUnprocessedHeaders);
	}
	else
	{
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
		pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
	}
	if (sdf_fn_uaGetAllowHeaderList(pObject, pInitData, &pAllowHdrList,\
			pErr) == Sdf_co_fail)
	{
		pErr->errCode=Sdf_en_headerListAccessError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
			"Error accessing the initData from the call object",pErr);
#endif
		(void)sdf_ivk_uaFreeInitData(pInitData);
		return Sdf_co_fail;
	}
	if (pAllowHdrList != Sdf_co_null)
	{
		/*
		 * The header should be inserted into every request.
		 * Hence add this HeaderList structure into the
		 * ProcessedHeaders list of the call object
		 */
		if(sdf_listAppend(pProcessedHeaderList, pAllowHdrList, \
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to append Allow HeaderList to "
				"slProcessedHeaders List ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			(void)sdf_ivk_uaFreeHeaderList(pAllowHdrList);
			(void)sdf_ivk_uaFreeInitData(pInitData);
			return Sdf_co_fail;
		}
	}
	/*
	 * Iterate through the list of ExtraHeaders and append them to the
	 * appropriate list depending on whether the addalways attribute has been
	 * set
	 */
	(void)sdf_listInitIterator(&(pInitData->slExtraHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pTemp;
		en_HeaderType pType;
		Sdf_ty_s8bit dTempType[Sdf_co_characterBufferSize]="";

		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		Sdf_mc_strncpy(dTempType,pTemp->pParam->pName,Sdf_co_characterBufferSize);

		if(Sdf_co_characterBufferSize > Sdf_mc_strlen(dTempType))
		    Sdf_mc_strcat(dTempType,":");

		/*
		 * Invoke stack API to convert the Header name into the stacks
		 * en_HeaderType enumeration
		 */
		if(sip_getTypeFromString(dTempType,&pType, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
				"Failed to get header type from string ",pErr);
#endif
			return Sdf_co_fail;
		}
		if (pType == SipHdrTypeAllow)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		/*
		 * Find out if it is a request header type.
		 */
		if (!(isSipRespHeader(pType)))
		{
			Sdf_ty_u32bit size,j;
			Sdf_st_headerList *pTempElement;

			(void)sip_listSizeOf(&(pTemp->pParam->slValue), &size, \
 			  (SipError*)&(pErr->stkErrCode));

			/* ---------
			 * IMPORTANT
			 * ---------
			 * Before proceeding, we change the pType to unknown here if it
			 * is not of the type supported or require. If more headers
			 * need to be handled in this block later; we shall need to
			 * expand this check.
			 */
			if ( (pType != SipHdrTypeSupported) && 
					(pType != SipHdrTypeRequire) && 
					(pType != SipHdrTypeAllow) )
				pType = SipHdrTypeUnknown;

			/*
			 * Initialize a HeaderList structure to hold the 
			 * headers.
			 */
			if (sdf_ivk_uaInitHeaderList(&pTempElement, \
            	pType, Sdf_co_null, pErr) ==Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
					"Failed to Initialize HeaderList ",pErr);
#endif
				return Sdf_co_fail;
			}
					
			/*
			 * Iterate through the list of headers in the
			 * ExtraHeaders and set it into the HeaderList structure
			 */
			for (j = 0; j < size; j++)
			{
			  	Sdf_ty_s8bit *pTempVal, *pOptions;
			  	SipHeader *pHdr;

			  	if (sip_listGetAt(&(pTemp->pParam->slValue),  \
					(Sdf_ty_u32bit) j,(Sdf_ty_pvoid *)&pTempVal,  \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
			  	{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_extraHeaderAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to get value at index from header list",pErr);
#endif
					pErr->errCode=Sdf_en_extraHeaderAccessError;
					(void)sdf_ivk_uaFreeInitData(pInitData);
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					return Sdf_co_fail;
			  	}
				/* HSS-FIX*/
				{
				  	if (sip_initSipHeader(&pHdr, pType, \
						(SipError*)&(pErr->stkErrCode))==SipFail)
				  	{
						pErr->errCode=Sdf_en_headerManipulationError;
#ifdef SDF_ERROR	
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
							"Failed to Initialize SipHeader",pErr);
#endif
						(void)sdf_ivk_uaFreeInitData(pInitData);
		 				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
						return Sdf_co_fail;
				  	}

				}
				/* HSS-FIX*/
				switch (pType)
				{
					case (SipHdrTypeSupported):
					{
						/*
                         * If the tag is 100rel, then we should add it to the
                         * Supported header only if the method is either
                         * INVITE or OPTIONS.
                         */
                        if((Sdf_mc_strcmp(pTempVal,"100rel")==0) &&
                           ((Sdf_mc_strcmp(pMethod,"INVITE")!=0)&&
                            (Sdf_mc_strcmp(pMethod, "OPTIONS") != 0)))
                        {
						  	sip_freeSipHeader(pHdr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
						  		(Sdf_ty_pvoid*)&pHdr,pErr);
                            continue;
                        }
						/* CSR - 1-2613099 */
						if(Sdf_mc_strcmp(pTempVal,"replaces")==0)
								dReplacesCheck = Sdf_co_true;

						/* CSR - 1-2613099 */
					
						/*
						 * Set the options in supported header
						 */
						Sdf_mc_strdup(pOptions,pTempVal);
					  	if(sip_setOptionInSupportedHdr(pHdr,pOptions, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
					  	{
							(void)sdf_ivk_uaFreeInitData(pInitData);
						  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
						  	sip_freeSipHeader(pHdr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
						  		(Sdf_ty_pvoid*)&pHdr,pErr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
						  		(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,
								Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
								"Failed to set options in Supported Header",\
								pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
						  	return Sdf_co_fail;
					   	}
	
						break;
					} /* End of case "supported" */
	
					case SipHdrTypeRequire:
					{
						/*
                         * If the tag is 100rel, then we should add it to the
                         * Require header only if the method is INVITE 
                         */
                        if((Sdf_mc_strcmp(pTempVal,"100rel")==0) &&
                           (Sdf_mc_strcmp(pMethod,"INVITE")!=0))
                        {
						  	sip_freeSipHeader(pHdr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,\
						  		(Sdf_ty_pvoid*)&pHdr,pErr);
                            continue;
                        }

					  	/*
						 * Set the options in Require header
						 */
						Sdf_mc_strdup(pOptions,pTempVal);
					  	if(sip_setTokenInRequireHdr(pHdr,pOptions, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
					  	{
						  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
						  	sip_freeSipHeader(pHdr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(\
						  		Sdf_ty_pvoid*)&pHdr,pErr);
						  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(\
						  		Sdf_ty_pvoid*)&pOptions,pErr);
							(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
							"Failed to  set option in Require header",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
					  		return Sdf_co_fail;
					  	}
					   break;
					} /* End of SipHdrTypeRequire */
					default:
					{
					  	Sdf_ty_s8bit *pUnk, *pBody;

					  	/*
						 * Set the name in unknown header
						 */
					  	Sdf_mc_strdup(pUnk,pTemp->pParam->pName);

					  	if(sip_setNameInUnknownHdr(pHdr,pUnk, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
					  	{
						  	sip_freeSipHeader(pHdr);
					  	  	(void)sdf_memfree(Sdf_mc_freeMemId,(\
						  		Sdf_ty_pvoid*)&pHdr,pErr);
					  	  	(void)sdf_memfree(Sdf_mc_freeMemId,(\
						  		Sdf_ty_pvoid*)&pUnk,pErr);
						  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
							(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
								"Failed to set name in Unknown header ",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
						  	return Sdf_co_fail;
					   	}
    
					  	/*
						 * Set body in unknown header
						 */
						Sdf_mc_strdup(pBody,pTempVal);

					  	if(sip_setBodyInUnknownHdr(pHdr,pBody, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
					  	{
					  	  	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
									&pBody,pErr);
						  	sip_freeSipHeader(pHdr);
					  	  	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
									&pHdr,pErr);
						  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
							(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
								"Failed to set body in Unknown header",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
						  	return Sdf_co_fail;
					  	}
					} /* End of 'default' case */
		 		} /* End of switch-case */

				/*
				 * Add this header to the HeaderList
				 */
			  	if(sdf_listAppend(&(pTempElement->slHeaders), \
				 	(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
			   	{
					(void)sdf_ivk_uaFreeInitData(pInitData);
				  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				  	sip_freeSipHeader(pHdr);
				  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, \
							pErr);
				  	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_headerListAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported header to "
						"HeaderList structure",pErr);
#endif
					pErr->errCode=Sdf_en_headerListAccessError;
			   	  	return Sdf_co_fail;
			   	}
			} /* 
			   * End of for loop making a header list structure. Use the
			   * header list formed in the loop now
			   */ 	
			
			if(pTemp->pData==Sdf_co_null)
			{
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				(void)sdf_listNext(&dListIterator, pErr);
				continue;
			}

			if(Sdf_mc_strcasecmp("yes",(Sdf_ty_s8bit *)pTemp->pData) \
				== 0)
			{
				/*
				 * The header should be inserted into every request.
				 * Hence add this HeaderList structure into the
				 * ProcessedHeaders list of the call object
				 */
		    	if(sdf_listAppend(pProcessedHeaderList, pTempElement, \
					pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported HeaderList to "
						"slProcessedHeaders List ",pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					(void)sdf_ivk_uaFreeInitData(pInitData);
					return Sdf_co_fail;
				}
			}
			else
			{
				/*
				 * The header should not be inserted into every request.
				 * Hence add this HeaderList structure into the
				 * UnProcessedHeaders list of the call object
				 */
				if(sdf_listAppend(pUnprocessedHeaderList,pTempElement, \
					pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported HeaderList to "
						"slUnprocessedHeaders List ",pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					(void)sdf_ivk_uaFreeInitData(pInitData);
					return Sdf_co_fail;
				}
			}
	  	} /* End of if (not Response Header) */
		(void)sdf_listNext(&dListIterator, pErr);
	} /* End of for loop */

	/* Free the local reference */
	(void)sdf_ivk_uaFreeInitData(pInitData);
	
	/* CSR - 1-2613099 */

	if(dReplacesCheck !=Sdf_co_true)
	
	/* CSR - 1-2613099 */
	{

	/* Send "Supported: replaces" in all the requests */
#ifdef SDF_SERVICE
	{
	  	Sdf_ty_s8bit *pOptions=Sdf_co_null;
	  	SipHeader *pHdr=Sdf_co_null;
		Sdf_st_headerList *pTempElement=Sdf_co_null;
		
		if (sip_initSipHeader(&pHdr, SipHdrTypeSupported, \
				(SipError*)&(pErr->stkErrCode)) == SipFail)
	  	{
			pErr->errCode=Sdf_en_headerManipulationError;
#ifdef SDF_ERROR	
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to Initialize SipHeader (Supported:replaces)",pErr);
#endif
			return Sdf_co_fail;
		}
		/*
		 * Set the options in supported header
		 */
		Sdf_mc_strdup(pOptions,"replaces");
		if(sip_setOptionInSupportedHdr(pHdr,pOptions, \
			 	(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to set -replaces- option in Supported Header", pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		/*
		* Initialize a HeaderList structure to hold the 
		* headers.
		*/
		if (sdf_ivk_uaInitHeaderList(&pTempElement, \
            	SipHdrTypeSupported, Sdf_co_null, pErr) ==Sdf_co_fail)
		{
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
					"Failed to Initialize Supported HeaderList",pErr);
#endif
			return Sdf_co_fail;
		}

		/*
		 * Add this header to the HeaderList
		 */
		if(sdf_listAppend(&(pTempElement->slHeaders), \
				 	(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,
				Sdf_en_headerListAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to append Supported header to "
				"HeaderList structure",pErr);
#endif
			pErr->errCode=Sdf_en_headerListAccessError;
			return Sdf_co_fail;
		}
		/*
		 * The header should be inserted into every request.
		 * Hence add this HeaderList structure into the
		 * ProcessedHeaders list of the call object
		 */
		if(sdf_listAppend(pProcessedHeaderList, pTempElement, \
					pErr)==Sdf_co_fail)
		{
			(void)sdf_listDeleteAll(&(pTempElement->slHeaders),pErr);
			(void)sdf_ivk_uaFreeHeaderList(pTempElement);
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, pErr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,
				Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
				"Failed to append Supported HeaderList to "
				"slProcessedHeaders List ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		} /* End of if(dReplacesCheck !=Sdf_co_true)*/
#endif	/* end of SDF_SERVICE */
	}
  	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddExtraHeaders()\n");
	return Sdf_co_success;	
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaDeleteRegistration
 **
 ** DESCRIPTION: Invoke the sdf_ivk_uaMakeRegister and insert the Contact
 ** 			 list to the call object for which delete Registration 
 **				 will be sent
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteRegistration
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_slist *pContactList,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr)
#else
	( pObject, pContactList, pOverlapTransInfo, pErr )
	Sdf_st_callObject *pObject;
	Sdf_ty_slist *pContactList;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	SipMessage *pSipMsg = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaDeleteRegistration");

#ifdef SDF_PARAMVALIDATION
	if (pErr==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDeleteRegistration(): pError passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if((pObject==Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
			"Call object parameter passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
	 * Form the REGISTER by invoking sdf_ivk_uaMakeRegister
	 */
	if (sdf_ivk_uaMakeRegister(pObject,(Sdf_ty_s8bit *)"0", Sdf_co_null, \
		pOverlapTransInfo, pErr)== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
			"Failed to Make Register",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Delete all the Contact header from the pOutgoingMsg SipMessage formed
	 */
	if( pOverlapTransInfo == Sdf_co_null )
	{	   
		if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
			Sdf_en_uacTransaction, &pTransaction, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaDeleteRegistration( ): "
				"Failed to get UAC transaction from call object", pErr);
#endif			
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		pSipMsg = pTransaction->pSipMsg;

		/* Free the local reference */
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
	}
	else
		pSipMsg = pOverlapTransInfo->pSipMsg;

	if(sip_deleteAllHeaderType(pSipMsg, SipHdrTypeContactAny, \
		(SipError *)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
			"Failed to delete existing contact headers",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
	 * If the pContactList is Null the Form a Contact header of type WildCard
	 */
	if(pContactList==Sdf_co_null)
	{
		Sdf_ty_s8bit dContact[] = "Contact: *";

		if(sip_insertHeaderFromStringAtIndex(pSipMsg,\
			SipHdrTypeContactWildCard,dContact,(Sdf_ty_u32bit)0,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
			"Failed to Insert contact headers",pErr);
#endif
			return Sdf_co_fail;
		}	
	}
	/*
	 * Otherwise insert the contact header from the input list to callobject
	 */
	else
	{
		Sdf_ty_u32bit size, dIndex = 0;
		SipHeader *pTempData;
		Sdf_st_listIterator dListIterator;

		(void)sdf_listSizeOf(pContactList, &size, pErr);

		if (size == 0)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
				"Size of list of Contact headers passed is zero.",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		(void)sdf_listInitIterator(pContactList, &dListIterator, pErr);

		while (dListIterator.pCurrentElement != Sdf_co_null)
		{
			pTempData = (SipHeader*)\
				(dListIterator.pCurrentElement->pData);

			if(sip_insertHeaderAtIndex(pSipMsg,pTempData,\
				dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
				pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteRegistration( ):  "
					"Failed to insert Contact header in SIP message",pErr);
#endif
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator, pErr);
			dIndex++;
		}
	}	
 
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaDeleteRegistration");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMakeUpdate
 **
 ** DESCRIPTION: 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeUpdate
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_st_error *pErr)
#else
	( pCallObj, pOverlapTransInfo, pErr )
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_state 	dCallState;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaMakeUpdate");
	
	/* ================ Parameter validations begin ================== */
#ifdef SDF_PARAMVALIDATION
	if ((pCallObj == Sdf_co_null) ||
			(pCallObj->pCallInfo == Sdf_co_null) ||
			(pErr == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeUpdate(): "
			"Invalid parameters passed\n",pErr);
#endif
        if(pErr != Sdf_co_null)
           pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	/* ================ Parameter validations end ================== */
	
	/* 
	 * Get the call state
	 */
	dCallState = pCallObj->pCallInfo->dState;
	
	if ((dCallState == Sdf_en_invalidStateTransition) ||\
			(dCallState == Sdf_en_idle) ||\
			(dCallState == Sdf_en_callTerminated) ||\
			(dCallState == Sdf_en_inviteCancelled))
	{
#ifdef SDF_ERROR
		sdf_fn_setError( Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeUpdate(): "
			"Cannot Make an UPDATE request in this state.\n",pErr);
#endif
		pErr->errCode = Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
	
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"UPDATE", pCallObj,\
		pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError( Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeUpdate(): "
			"Could not Make an UPDATE transaction.\n",pErr);
#endif
		return Sdf_co_fail;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaMakeUpdate");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMuteCall
 **
 ** DESCRIPTION: This function prepares an INVITE message to Mute the call by
 **              either setting the c= line to 0.0.0.0 or changing the 
 **              direction attribute.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMuteCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit      *pCodecsToMute,
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
	 Sdf_st_error      *pErr)
#else
	( pObject, pCodecsToMute, pslMediaIDs, dType, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit *pCodecsToMute;
	 Sdf_ty_slist *pslMediaIDs,
	 Sdf_ty_muteType dType,
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMuteCall");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaMuteCall( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMuteCall( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
     * This function will Mute the SDP based on dType and also form
	 * the INVITE transaction.
	 */
	if(sdf_ivk_uaFormMuteSdp( pObject, pCodecsToMute,\
       pslMediaIDs, dType, (Sdf_ty_s8bit *)"INVITE", pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMuteCall( ):  "
			"Muting the SDP for MuteCall failed",pErr);
#endif
		return Sdf_co_fail;
    }
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaMuteCall");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFormMuteSdp
 **
 ** DESCRIPTION: This function initiates an offer and mutes the SDP in the call 
 **              object. If pCodecsToMute is not null then matching MediaStreams
 **              are found with the help of codecs present in pCodecsToMute and
 **              are muted. If pslMediaIDs is not null the all the MediaStreams
 **              whose ID match with those present in the pslMediaIDs are muted.
 **              If both these parameters are null then the whole session is
 **              muted.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormMuteSdp
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit      *pCodecsToMute,
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
  	 Sdf_ty_s8bit      *pMethod,
	 Sdf_st_error      *pErr)
#else
	( pObject, pCodecsToMute, pslMediaIDs, dType, pMethod, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit      *pCodecsToMute;
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
  	 Sdf_ty_s8bit      *pMethod,
	 Sdf_st_error      *pErr;
#endif
{
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_transaction *pTransaction = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaFormMuteSdp");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaFormMuteSdp( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pMethod == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Invalid method parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Initiating Offer for Muting Failed.",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Call this API to do the actual Muting work.
     */	 
	if (sdf_ivk_uaMuteMediaStreams(pSessionParams, pCodecsToMute, pslMediaIDs,\
		                 dType, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Muting Media Streams Failed.",pErr);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pErr->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMutesdp( ): "
			"Failed to allocate memory for the SDP message", pErr);
#endif		
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Failed to make SdpMessage from SessionParams",pErr);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams,pErr) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Failed to Set Last Offer Made",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	/*
     * Form the INVITE/UPDATE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction(pMethod, pObject, Sdf_co_null, pErr)\
		   	== Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Failed to make the transaction",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }

	/*
	 * Form the SipMsgBody with the SdpMessage just constructed.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pErr->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMutesdp( ): "
			"Failed to allocate memory for the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   ((SipError*)&pErr->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMutesdp( ): "
			"Failed to set the SDP message in the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	/*Free the SdpMessage*/
	sip_freeSdpMessage(pSdpMessage);
	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
			Sdf_en_uacTransaction,&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormMutesdp( ): "
			"Failed to get UAC transaction from call object", pErr);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMutesdp( ):  "
			"Failed to delete the current message bodies.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}
	
	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMutesdp( ):  "
			"Failed to append message body to slMsgBodyList",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormMuteSdp( ):  "
			"Failed to set sdp into the SipMessage being made.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local references here. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaFormMuteSdp");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMuteMediaStreams
 **
 ** DESCRIPTION: This function mutes the MediaStreams present in the given 
 **              Session Param. If pCodecsToMute is not null then matching 
 **              MediaStreams are found with the help of codecs present in 
 **              pCodecsToMute and are muted. If pslMediaIDs is not null the 
 **              all the MediaStreams whose ID match with those present in 
 **              the pslMediaIDs are muted.  If both these parameters are null 
 **              then the whole session is muted.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMuteMediaStreams
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	 Sdf_ty_s8bit        *pCodecsToMute,
	 Sdf_ty_slist        *pslMediaIDs,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr)
#else
	( pSessionParams, pCodecsToMute, pslMediaIDs, dType, pErr )
	 Sdf_st_sessionParam *pSessionParams;
	 Sdf_ty_s8bit        *pCodecsToMute;
	 Sdf_ty_slist        *pslMediaIDs,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Entering sdf_ivk_uaMuteMediaStreams");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaMuteMediaStreams( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
			"Invalid Session Param parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* Param Validation ends here. */

	/*
     * If the application wishes to mute the media streams with some 
	 * particular codecs.
	 */
	if (pCodecsToMute != Sdf_co_null)
    {
		Sdf_ty_s8bit *pCodecStr = Sdf_co_null;
		Sdf_ty_s8bit *payload = Sdf_co_null;
        Sdf_ty_s8bit dTempCodecList[Sdf_co_characterBufferSize];
		Sdf_ty_s8bit *pTempCodec = Sdf_co_null;

        Sdf_mc_strcpy(dTempCodecList, pCodecsToMute);

        pCodecStr = Sdf_mc_strtokr(dTempCodecList," ",&pTempCodec);

		if(Sdf_co_null == pCodecStr)
		{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
			"Invalid Session Param parameter passed to the function",pErr);
#endif			
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		do
        {
			Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
			Sdf_ty_u8bit       dIndex = 0;
		    Sdf_ty_u32bit      codecIndex = 0;

			/*if (!sdf_fn_uaIsNumber(pCodecStr))
				payload = sdf_fn_uaMapPayloadFromCodec(pCodecStr, pErr);
			else
				payload = pCodecStr;*/

			payload = pCodecStr;


			for (dIndex = 0; dIndex < (pSessionParams->dNumMediaStreams); dIndex++)
            {
				Sdf_ty_u32bit dCodecListSize = 0;
				Sdf_st_codec *pCodec = Sdf_co_null;
				Sdf_ty_codecName dCodecName;
				Sdf_ty_bool dStreamToBeMuted = Sdf_co_false;

				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
							&pMediaStream, dIndex, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pErr);
#endif
					pErr->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				(void)sdf_listSizeOf(&(pMediaStream->slCodecs), &dCodecListSize,\
					           pErr);

				for (codecIndex = 0; codecIndex < dCodecListSize; codecIndex++)
                {
					if (sdf_listGetAt(&(pMediaStream->slCodecs), codecIndex,\
							(Sdf_ty_pvoid *)&pCodec, pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
		                sdf_fn_setError(Sdf_en_majorErrors,\
				           	Sdf_en_sdpAccessorError,\
      				       	(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
	      				    "Failed to get Codec at index from MediaStream",
			        		pErr);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
                        return Sdf_co_fail;
                    }

					/* Get the codec as name (enumeration). */
					dCodecName = \
						sdf_fn_uaMapCodecNumberToName(Sdf_mc_atoi(payload),\
								pErr);

					if (dCodecName != Sdf_en_otherCodec)
                    {
                        if (dCodecName == pCodec->dCodecName)
                        {
							dStreamToBeMuted = Sdf_co_true;
							break;
                        }
                    }
					else
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamToBeMuted = Sdf_co_true;
							break;
                        }
                    }
                } /* End of for loop of dCodecListSize */

				if (dStreamToBeMuted == Sdf_co_true)
                {
					if (sdf_fn_uaMuteMediaStream(pMediaStream, dType,\
								pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
				        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
        					(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
	        				"Error Muting MediaStream in SDP",pErr);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			        	return Sdf_co_fail;
                    }
                }

				/* Free the local reference */
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */

			/* 
			 * Pick up the next payload from the input list
			 */
			pCodecStr = Sdf_mc_strtokr(pTempCodec, " ",&pTempCodec);

        } while (pCodecStr != Sdf_co_null);
    }
	/*
     * If the application wants to mute media streams with given IDs
	 */
	else if (pslMediaIDs != Sdf_co_null)
    {
		Sdf_ty_u32bit dMediaIDListSize = 0;
		Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
		Sdf_ty_u8bit        dMediaIDIndex = 0, dMediaStreamIndex = 0;
		Sdf_ty_u8bit        dNumMediaStreams = 0;
        Sdf_ty_s8bit        *pMediaStreamId = Sdf_co_null;

		(void)sdf_listSizeOf(pslMediaIDs, &dMediaIDListSize, pErr);

		for (dMediaIDIndex = 0; dMediaIDIndex < dMediaIDListSize;\
			   	            dMediaIDIndex++)
        {
			if (sdf_listGetAt(pslMediaIDs, dMediaIDIndex,\
					    (Sdf_ty_pvoid *)&pMediaStreamId, pErr) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
		        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
      					(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
	      				"Failed to get MediastreamId at index from slMediaID",
						pErr);
#endif
	        	return Sdf_co_fail;
            }

			dNumMediaStreams = pSessionParams->dNumMediaStreams;

			for (dMediaStreamIndex = 0; dMediaStreamIndex < dNumMediaStreams;\
				   dMediaStreamIndex++)
            {
				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
						&pMediaStream, dMediaStreamIndex, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pErr);
#endif
					pErr->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				/* Check if the MediaStreamIds match. */
				if (Sdf_mc_strcmp(pMediaStream->pMediaStreamId, \
							pMediaStreamId) == 0)
                {
					if (sdf_fn_uaMuteMediaStream(pMediaStream, dType,\
								pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
				        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
        					(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
	        				"Error Muting MediaStream in SDP",pErr);
#endif
				        (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			        	return Sdf_co_fail;
                    }

					/* Free the local reference */
			        (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
					break;
                }

                /* Free the local reference */
			    (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */
        } /* End of for loop of dMediaIDListSize */
    }
	/*
     * If the application wishes to mute the whole call.
	 */
	else
    {
		if (sdf_fn_uaMuteMediaSession(pSessionParams, dType,\
					pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaMuteMediaStreams( ):  "
				"Error Muting Media Session",pErr);
#endif
			return Sdf_co_fail;
		}
    }

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_ivk_uaMuteMediaStreams");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaUnMuteCall
 **
 ** DESCRIPTION: This function prepares an INVITE message to UnMute the call by
 **              either restoring the c= line to prev IP addr or by restoring 
 **              the direction attribute, depending on how the media stream
 **              was muted.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUnMuteCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit      *pCodecsToUnMute,
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
	 Sdf_st_error      *pErr)
#else
	( pObject, pCodecsToUnMute, pslMediaIDs, dType, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit      *pCodecsToUnMute;
	 Sdf_ty_slist      *pslMediaIDs;
	 Sdf_ty_muteType   dType;
	 Sdf_st_error      *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaUnMuteCall");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaUnMuteCall( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteCall( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/*
     * This function will Mute the SDP based on dType and also form
	 * the INVITE transaction.
	 */
	if(sdf_ivk_uaFormUnMuteSdp( pObject, pCodecsToUnMute,\
		pslMediaIDs, dType, (Sdf_ty_s8bit *)"INVITE", pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteCall( ):  "
			"Muting the SDP for UnMuteCall failed",pErr);
#endif
		return Sdf_co_fail;
    }
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaUnMuteCall");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFormUnMuteSdp
 **
 ** DESCRIPTION: This function prepares an INVITE message to Mute the call by
 **              either setting the c= line to 0.0.0.0 or changing the 
 **              direction attribute.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormUnMuteSdp
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_ty_s8bit      *pCodecsToUnMute,
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
	 Sdf_ty_s8bit      *pMethod,
	 Sdf_st_error      *pErr)
#else
	( pObject, pCodecsToUnMute, pslMediaIDs, dType, pMethod, pErr )
	 Sdf_st_callObject *pObject;
	 Sdf_ty_s8bit      *pCodecsToUnMute;
	 Sdf_ty_slist      *pslMediaIDs,
	 Sdf_ty_muteType   dType,
	 Sdf_ty_s8bit      *pMethod;
	 Sdf_st_error      *pErr;
#endif
{
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_transaction *pTransaction = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaFormUnMuteSdp");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaFormUnMuteSdp( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Invalid call object parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pMethod == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Invalid method parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/*
     * Invoke this API to check media state and verify whether an offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pObject, Sdf_en_offerChange,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Initiating Offer for Muting Failed.",pErr);
#endif
		return Sdf_co_fail;
	}

	/*
     * Invoke this API to do the actual UnMute work.
	 */
	if (sdf_ivk_uaUnMuteMediaStreams(pSessionParams, pCodecsToUnMute,\
		pslMediaIDs, dType, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"UnMuting MediaStreams Failed.",pErr);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}

	/*
     * Convert the SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pErr->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormUnMuteSdp( ): "
			"Failed to allocate memory for the SDP message", pErr);
#endif		
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to make SdpMessage from SessionParams",pErr);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }
	
	/*Set LastOfferMade*/
	if(sdf_ivk_uaSetLastOfferMade(pObject->pMediaInfo,pSessionParams,pErr) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to Set Last Offer Made",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
	}

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	/*
     * Form the INVITE/UPDATE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction(pMethod, pObject, Sdf_co_null, pErr)\
		   	== Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to make the transaction",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }

	/*
	 * Form the SipMsgBody with the SdpMessage just constructed.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pErr->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormUnMuteSdp( ): "
			"Failed to allocate memory for the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   ((SipError*)&pErr->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormUnMuteSdp( ): "
			"Failed to set the SDP message in the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }

	/* Free the SdpMessage */
	sip_freeSdpMessage(pSdpMessage);

	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_ivk_uaGetTransactionFromCallObject(pObject,\
			Sdf_en_uacTransaction,&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormUnMuteSdp( ): "
			"Failed to get UAC transaction from call object", pErr);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to delete the current message bodies.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}
	
	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to append message body to slMsgBodyList",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormUnMuteSdp( ):  "
			"Failed to set sdp into the SipMessage being made.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local references here. */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaFormUnMuteSdp");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaUnMuteMediaStreams
 **
 ** DESCRIPTION: This function UnMutes the SessionParam by
 **              either restoring the c= line to prev IP addr or by restoring 
 **              the direction attribute, depending on dType.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUnMuteMediaStreams
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParams,
	 Sdf_ty_s8bit        *pCodecsToUnMute,
	 Sdf_ty_slist        *pslMediaIDs,
	 Sdf_ty_muteType     dType,
	 Sdf_st_error        *pErr)
#else
	( pSessionParams, pCodecsToUnMute, pslMediaIDs, dType, pErr )
	 Sdf_st_sessionParam *pSessionParams;
	 Sdf_ty_s8bit        *pCodecsToUnMute;
	 Sdf_ty_slist        *pslMediaIDs,
	 Sdf_ty_muteType     dType;
	 Sdf_st_error        *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "\
			"sdf_ivk_uaUnMuteMediaStreams");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaUnMuteMediaStreams( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
			"Invalid Session Param parameter passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif  /* End of Param Validation. */

	/*
     * If the application wishes to unmute the media streams with some 
	 * particular codecs.
	 */
	if (pCodecsToUnMute != Sdf_co_null)
    {
		Sdf_ty_s8bit *pCodecStr = Sdf_co_null;
		Sdf_ty_s8bit *payload = Sdf_co_null;
        Sdf_ty_s8bit dTempCodecList[Sdf_co_characterBufferSize];
		Sdf_ty_s8bit *pTempCodec = Sdf_co_null;

        Sdf_mc_strcpy(dTempCodecList, pCodecsToUnMute);

        pCodecStr = Sdf_mc_strtokr(dTempCodecList," ",&pTempCodec);

		if(Sdf_co_null == pCodecStr)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
					"Invalid Session Param parameter passed to the function",pErr);
#endif			
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		do
        {
			Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
			Sdf_ty_u8bit       dIndex = 0, codecIndex = 0;

			/*if (!sdf_fn_uaIsNumber(pCodecStr))
				payload = sdf_fn_uaMapPayloadFromCodec(pCodecStr, pErr);
			else
				payload = pCodecStr;*/

			payload = pCodecStr;

			for (dIndex = 0; dIndex < (pSessionParams->dNumMediaStreams); dIndex++)
            {
				Sdf_ty_u32bit dCodecListSize = 0;
				Sdf_st_codec *pCodec = Sdf_co_null;
				Sdf_ty_codecName dCodecName;
				Sdf_ty_bool dStreamToBeUnMuted = Sdf_co_false;

				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
							&pMediaStream, dIndex, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pErr);
#endif
				    pErr->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				(void)sdf_listSizeOf(&(pMediaStream->slCodecs), &dCodecListSize,\
					           pErr);

				for (codecIndex = 0; codecIndex < dCodecListSize; codecIndex++)
                {
					if (sdf_listGetAt(&(pMediaStream->slCodecs), codecIndex,\
							(Sdf_ty_pvoid *)&pCodec, pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
		                sdf_fn_setError(Sdf_en_majorErrors,\
				           	Sdf_en_sdpAccessorError,\
      				       	(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
	      				    "Failed to get Codec at index from MediaStream",
			        		pErr);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
                        return Sdf_co_fail;
                    }

					/* Get the codec as name (enumeration). */
					dCodecName = \
						sdf_fn_uaMapCodecNumberToName(Sdf_mc_atoi(payload),\
								pErr);

					if (dCodecName != Sdf_en_otherCodec)
                    {
                        if (dCodecName == pCodec->dCodecName)
                        {
							dStreamToBeUnMuted = Sdf_co_true;
							break;
                        }
                    }
					else
                    {
						if (Sdf_mc_strcmp(payload,pCodec->pOtherCodecName) == 0)
                        {
							dStreamToBeUnMuted = Sdf_co_true;
							break;
                        }
                    }
                } /* End of for loop of dCodecListSize */

				if (dStreamToBeUnMuted == Sdf_co_true)
                {
					if (sdf_fn_uaUnMuteMediaStream(pMediaStream, dType,\
								pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
				        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
        					(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
	        				"Error Muting MediaStream in SDP",pErr);
#endif
						(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			        	return Sdf_co_fail;
                    }
                }

				/* Free the local reference */
				(void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */

			/* 
			 * Pick up the next payload from the input list
			 */
			pCodecStr = Sdf_mc_strtokr(pTempCodec, " ",&pTempCodec);

        } while (pCodecStr != Sdf_co_null);
    }
	/*
     * If the application wants to unmute media streams with given IDs
	 */
	else if (pslMediaIDs != Sdf_co_null)
    {
		Sdf_ty_u32bit dMediaIDListSize = 0;
		Sdf_st_mediaStream *pMediaStream = Sdf_co_null;
		Sdf_ty_u8bit        dMediaIDIndex = 0, dMediaStreamIndex = 0;
		Sdf_ty_u8bit        dNumMediaStreams = 0;
        Sdf_ty_s8bit        *pMediaStreamId = Sdf_co_null;

		(void)sdf_listSizeOf(pslMediaIDs, &dMediaIDListSize, pErr);

		for (dMediaIDIndex = 0; dMediaIDIndex < dMediaIDListSize;\
			   	            dMediaIDIndex++)
        {
			if (sdf_listGetAt(pslMediaIDs, dMediaIDIndex,\
					    (Sdf_ty_pvoid *)&pMediaStreamId, pErr) == Sdf_co_fail)
            {
#ifdef SDF_ERROR
		        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
      					(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
	      				"Failed to get MediastreamId at index from slMediaID",
						pErr);
#endif
	        	return Sdf_co_fail;
            }

			dNumMediaStreams = pSessionParams->dNumMediaStreams;

			for (dMediaStreamIndex = 0; dMediaStreamIndex < dNumMediaStreams;\
				   dMediaStreamIndex++)
            {
				if (sdf_ivk_uaGetMediaStreamAtIndex(pSessionParams,\
						&pMediaStream, dMediaStreamIndex, pErr) == Sdf_co_fail)
                {
#ifdef SDF_ERROR
	                sdf_fn_setError(Sdf_en_majorErrors,\
			           	Sdf_en_sessionParamAccessError,\
   				       	(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
      				    "Failed to get MediaStream at index from SessionParam",\
					   	pErr);
#endif
				    pErr->errCode=Sdf_en_sessionParamAccessError;
                    return Sdf_co_fail;
                }

				/* Check if the MediaStreamIds match. */
				if (Sdf_mc_strcmp(pMediaStream->pMediaStreamId, \
							pMediaStreamId) == 0)
                {
					if (sdf_fn_uaUnMuteMediaStream(pMediaStream, dType,\
								pErr) == Sdf_co_fail)
                    {
#ifdef SDF_ERROR
				        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
        					(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
	        				"Error Muting MediaStream in SDP",pErr);
#endif
				        (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
			        	return Sdf_co_fail;
                    }

					/* Free the local reference */
			        (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
					break;
                }

                /* Free the local reference */
			    (void)sdf_ivk_uaFreeMediaStream(pMediaStream);
            } /* End of for loop of dNumMediaStreams */
        } /* End of for loop of dMediaIDListSize */
    }
	/*
     * If the application wishes to UnMute the whole call.
	 */
	else
	{
		if (sdf_fn_uaUnMuteMediaSession(pSessionParams, dType,\
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaUnMuteMediaStreams( ):  "
				"Error Un Muting Media Session",pErr);
#endif
			return Sdf_co_fail;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "\
			"sdf_ivk_uaUnMuteMediaStreams");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeRefer
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a REFER request to be sent to the peer entity.
**				This function also adds a "Refer-To" header. If a "Replaces"
**				header is also passed, this function converts it to string
**				(escapes any characters, if required) and adds it to
**				"Refer-To" header as a URL param.
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store REFER txn
**		pReferToHdr(IN)		: Refer-To header to go in REFER request
**		pReplacesHdr(IN)	: Replaces header to go in REFER request as header
**								param in Refer-To header.
**		dIsSendRfrdBy(IN)	: Boolean to indicate whether to send Referred-By
**								header in the REFER request or not
**		pReferredBy(IN)		: If `dIsSendRfrdBy` is TRUE, this header will be
**								set in SipMessage
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeRefer
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	SipHeader *pReferToHdr,
	SipHeader *pReplacesHdr,
	Sdf_ty_bool			dIsSendRfrdBy,
	SipHeader	*pReferredBy,
	Sdf_st_error *pErr)
#else
	( pCallObj, pOverlapTransInfo, pReferToHdr, pReplacesHdr, \
	dIsSendRfrdBy, pReferredBy, pErr )
	 Sdf_st_callObject *pCallObj;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 SipHeader *pReferToHdr;
	 SipHeader *pReplacesHdr;
     Sdf_ty_bool			dIsSendRfrdBy;
	 SipHeader	*pReferredBy;
	 Sdf_st_error *pErr;
#endif
{
	SipMessage		*pSipMsg = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeRefer");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaMakeRefer( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((pCallObj == Sdf_co_null) || (Sdf_co_null == pReferToHdr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRefer( ):  "
			"Invalid CallObject/ReferToHdr passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	/*
	 * Form REFER request using sdf_ivk_uaMakeTransaction()
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"REFER", \
		pCallObj, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRefer( ):  "
			"Failed to make REFER transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	pSipMsg = pOverlapTransInfo->pSipMsg;
	/*
	 * Add "Refer-To" header
	 */
	if (Sdf_co_null != pReferToHdr)
	{
		Sdf_st_error	dLocalErr;
		
		/*
		 * Check if "Replaces" header is specified. If yes, convert the replaces
		 * header into a string (escape the characters if necessary) and add it to
		 * "Refer-To" header.
		 */
		if (Sdf_co_null != pReplacesHdr)
		{
			Sdf_ty_s8bit	*pEscdRepStr = Sdf_co_null;
			
			if (Sdf_co_fail !=	sdf_ivk_uaFormURLStringFromReplacesHeader(\
						pReplacesHdr, &pEscdRepStr, pErr))
			{
				if (Sdf_co_null != pEscdRepStr)
				{
					SipAddrSpec		*pSipReferToAddrSpec=Sdf_co_null;
					Sdf_ty_s8bit 	*pReplace =Sdf_co_null;
					Sdf_ty_u32bit 	dLength = 0;
					SipUrl			*pSipUrl=Sdf_co_null;

					dLength = Sdf_mc_strlen(pEscdRepStr);	
					pReplace = (Sdf_ty_s8bit
							*)sdf_memget(1,dLength+Sdf_mc_strlen("Replaces=")+1,Sdf_co_null);

					if(Sdf_co_null == pReplace)
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
								(Sdf_ty_s8bit *)"sdf_ivk_uaMakeRefer( ):  "
								"pReplace - memeory allocation failure",pErr);
#endif
						pErr->errCode = Sdf_en_noMemoryError;
						return Sdf_co_fail;
					}

					Sdf_mc_strcpy(pReplace,"Replaces=");
					Sdf_mc_strcat(pReplace,pEscdRepStr);

					/* Free the replace header string */
					(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pEscdRepStr, &dLocalErr);
					if (SipFail == sip_getAddrSpecFromReferToHdr(pReferToHdr,\
						&pSipReferToAddrSpec, (SipError*)&pErr->stkErrCode))
					{
						pErr->errCode = pErr->stkErrCode;
						(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pReplace, &dLocalErr);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
							(Sdf_ty_s8bit*)"sdf_ivk_uaMakeRefer:"
							"Failed to get SipAddrSpec in Refer-To Header", pErr);
#endif			
						return Sdf_co_fail;
					}
					if(SipFail == sip_getUrlFromAddrSpec(pSipReferToAddrSpec,&pSipUrl,
					(SipError*)&pErr->stkErrCode))
					{
						pErr->errCode = pErr->stkErrCode;
						(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pReplace, &dLocalErr);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
							(Sdf_ty_s8bit*)"sdf_ivk_uaMakeRefer:"
							"Failed to get SipAddrSpec in Refer-To Header", pErr);
#endif			
						sip_freeSipAddrSpec(pSipReferToAddrSpec);
						return Sdf_co_fail;
					}
					if(SipFail ==
							sip_setHeaderInUrl(pSipUrl,pReplace,(SipError*)&pErr->stkErrCode))
					{
						pErr->errCode = pErr->stkErrCode;
						(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pReplace, &dLocalErr);
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
							(Sdf_ty_s8bit*)"sdf_ivk_uaMakeRefer:"
							"Failed to get SipAddrSpec in Refer-To Header", pErr);
#endif			
						sip_freeSipAddrSpec(pSipReferToAddrSpec);
						sip_freeSipUrl(pSipUrl);
						return Sdf_co_fail;
					}
					
					/* Free local reference */
					sip_freeSipUrl(pSipUrl);
					sip_freeSipAddrSpec(pSipReferToAddrSpec);
				}
			}
		}
		
		/* Add "Refer-To" header to "SipMessage" */
		if(SipFail == sip_insertHeaderAtIndex(pSipMsg, pReferToHdr, 0, \
			(SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaMakeRefer:"
				"Failed to insert 'Refer-To' header into SipMsg", pErr);
#endif			
			return Sdf_co_fail;
		}

	}
	if (Sdf_co_true == dIsSendRfrdBy)
	{
		if (Sdf_co_fail == sdf_ivk_uaInsertReferredByHeader(pCallObj,
					pSipMsg, pReferredBy, pErr))
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaMakeRefer:"
				"Failed to insert 'Referred-By' header into SipMsg", pErr);
#endif			
			return Sdf_co_fail;
		}
	}


	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeRefer");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeNotify
**
** DESCRIPTION: Invokes core stack APIs to form a NOTIFY message to be 
**				sent to the peer entity. This API will also create and
**				add an EVENT header, depending on the function arguments.
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store NOTIFY txn
**		pEvent(IN)		: Event header to go in NOTIFY request
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeNotify
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_ty_s8bit *pEvent,
	 Sdf_st_error *pErr)
#else
	( pCallObj, pOverlapTransInfo, pEvent, pErr )
	 Sdf_st_callObject *pCallObj;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_ty_s8bit *pEvent;
	 Sdf_st_error *pErr;
#endif
{
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaMakeNotify");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaMakeNotify( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((Sdf_co_null == pCallObj) || (Sdf_co_null == pOverlapTransInfo))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeNotify( ):  "
			"Invalid CallObject/OverlaTxnInfo passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	/*
	 * Form a NOTIFY request using sdf_ivk_uaMakeTransaction
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"NOTIFY", \
		pCallObj, pOverlapTransInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMakeNotify( ):  "
			"Failed to make NOTIFY transaction",pErr);
#endif
		return Sdf_co_fail;
	}

	/* Set "Event" header, if present */
	if (Sdf_co_null != pEvent)
	{
		SipMessage		*pSipMsg = Sdf_co_null;
		SipHeader		*pEventHdr = Sdf_co_null;
		Sdf_st_error	dLocalErr;
		
		pSipMsg = pOverlapTransInfo->pSipMsg;
	
		/* Initialise Event Header */
		if(SipFail == sip_initSipHeader(&pEventHdr, SipHdrTypeEvent, \
						(SipError*)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaMakeNotify:"
				"Failed to init Sip Header", pErr);
#endif
			return Sdf_co_fail;
		}

		/* Set Value In Event Header */
		if( SipFail == sip_impp_setEventTypeInEventHdr(pEventHdr, \
			Sdf_mc_strdupCallHandler(pEvent), \
			(SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(pEventHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pEventHdr, \
					&dLocalErr);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaMakeNotify:"
				"Failed to Set the Token into Sip Event Header", pErr);
#endif			
			return Sdf_co_fail;
		}
		
		/* Add Event Header To NOTIFY Request */
		if(SipFail == sip_insertHeaderAtIndex(pSipMsg, pEventHdr, 0, \
			(SipError *)&(pErr->stkErrCode)))
		{
			sip_freeSipHeader(pEventHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pEventHdr, \
					&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaMakeNotify:"
				"Failed to insert event header into Sip Message", pErr);
#endif			
			return Sdf_co_fail;
		}
		/* Free Local Reference */
		sip_freeSipHeader(pEventHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pEventHdr, \
					&dLocalErr);
	}
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaMakeNotify");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormNotify
**
** DESCRIPTION: This API forms the Notify, sets the event header and adds
**				the subscription state  header based on the input response
**				message passed. It also sets the response message as the 
**				appsip message body in this notify
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store NOTIFY txn
**		pEvent(IN)		: Event header to go in NOTIFY request
**		pMessage(IN)	: The Sipmessage which is gonna be set as the msg body
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormNotify (Sdf_st_callObject *pCallObject, \
		Sdf_st_overlapTransInfo *pOverlapTxn, \
		Sdf_ty_s8bit *pEvent, SipMessage *pSipMsg, Sdf_st_error *pErr)
{
	Sdf_ty_s8bit *pSubState = Sdf_co_null;
	SipParam *pParam = Sdf_co_null;
	Sdf_ty_u16bit dResponse = 0;
	SipHeader *pHdr = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)
		"SDF_DEBUG - Entering sdf_ivk_uaFormNotify");

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pErr == Sdf_co_null) ||(pCallObject == Sdf_co_null) ||\
		(pSipMsg == Sdf_co_null) || (pOverlapTxn == Sdf_co_null)||\
		(pEvent == Sdf_co_null))
	{
		if (pErr != Sdf_co_null)
		{
			pErr->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormNotify( ): "
			"Invalid Param passed to function", pErr);
#endif			
		}
		else
		{
#ifdef SDF_ERROR
			sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_ivk_uaFormNotify(): pErr param passed is Invalid");
#endif
		}
		return Sdf_co_fail; 
	}
#endif	/* End of param validation ifdef */

	/* Make the Notify Request */
	if (sdf_ivk_uaMakeNotify(pCallObject,\
			   	pOverlapTxn, pEvent, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sdf_ivk_uaMakeTransaction failed",pErr);
#endif
		return Sdf_co_fail;
    }

	/*
	 * Add a Subscription-State header based on the response code 
	 */
  
	 /*
	  * Prepare the header
	  */
	 if ((sip_initSipHeader(&pHdr, SipHdrTypeSubscriptionState, \
	    	(SipError*)&(pErr->stkErrCode)))==SipFail)
	 {
	 	pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sip_initSipHeader failed",pErr);
#endif
	    return Sdf_co_fail;
	 }
	 if(sip_initSipParam(&pParam,(SipError*)&(pErr->stkErrCode)) 
		 == SipFail)
	 {
	 	pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sip_initSipParam failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		return Sdf_co_fail;
	 }

	 /*
	  * Get the response code from the Sip message
	  */
	 if (sdf_ivk_uaGetRespCodeFromSipMessage(pSipMsg, &dResponse, pErr)
		 == Sdf_co_fail) 
	 { 
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sdf_ivk_uaGetRespCodeFromSipMessage failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		sip_freeSipParam(pParam);
		return Sdf_co_fail;
	 }
	 
	 if(dResponse/100 == 1) /* Provisional */
	 {
		Sdf_mc_strdup(pSubState, "active");
		if( sip_setNameInSipParam(pParam,Sdf_mc_strdupCT("expires"),\
		 	(SipError*)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"Failed to set Name in SipParam",pErr);
#endif
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pSubState,pErr); 
			sip_freeSipParam(pParam);
			pErr->errCode = Sdf_en_headerManipulationError;
			return Sdf_co_fail;

		}
		if( sip_insertValueAtIndexInSipParam(pParam,Sdf_mc_strdupCT("60"),0,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
		    sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"Failed to set Value in SipParam",pErr);
#endif
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
            (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pSubState,pErr);
			sip_freeSipParam(pParam);
			pErr->errCode = Sdf_en_headerManipulationError;
			return Sdf_co_fail;

		}

	 }
	 else /* any other final response */
	 {
		 Sdf_mc_strdup(pSubState, "terminated");
		 if( sip_setNameInSipParam(pParam,Sdf_mc_strdupCT("reason"),\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		 {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"Failed to set Name in SipParam",pErr);
#endif
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pSubState,pErr);
			sip_freeSipParam(pParam);
			pErr->errCode = Sdf_en_headerManipulationError;
			return Sdf_co_fail;

		 }
		 if( sip_insertValueAtIndexInSipParam(pParam,
			 Sdf_mc_strdupCT("noresource"),0,\
			(SipError*)&(pErr->stkErrCode))==SipFail)
		 {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"Failed to set Value in SipParam",pErr);
#endif
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
			(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pSubState,pErr);
			sip_freeSipParam(pParam);
			pErr->errCode = Sdf_en_headerManipulationError;
			return Sdf_co_fail;

		 }
	
	 }
	/*
	 * Now set the state in the subscription state header */
	 if(sip_impp_setSubStateInSubscriptionStateHdr(pHdr, pSubState,
		 (SipError*)&(pErr->stkErrCode)) == SipFail)
	 {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sip_impp_setSubStateInSubscriptionStateHdr failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		sip_freeSipParam(pParam);
		pErr->errCode = Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	 }
	 /*
	  * Set the header param i.e. "active;expires=60" or 
	  * "terminated;reason=noresource" based on the response
	  */
	 if(sip_impp_insertParamAtIndexInSubscriptionStateHdr(pHdr, pParam, 0 ,
		  (SipError*)&(pErr->stkErrCode)) == SipFail)
	 { 
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sip_impp_setParamAtIndexInSubscriptionStateHdr failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		sip_freeSipParam(pParam);
		pErr->errCode = Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	 }

 	/* 
	 * Set the subscription state header in the sip message 
	 */
	if (sip_setHeader(pOverlapTxn->pSipMsg, pHdr, 
		(SipError*)&(pErr->stkErrCode))==SipFail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sip_setHeader in SipMsg failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		sip_freeSipParam(pParam);
		pErr->errCode = Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/*
	 * Add a sipfrag message body and the content type as 
	 * message/sipfrag in the SipMessage from the SipMessage 
	 */
	if(sdf_ivk_uaFormAndAddSipfragBodyToSipMessage(pSipMsg, 
			&(pOverlapTxn->pSipMsg), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaFormNotify( ): "
		 	"sdf_ivk_uaFormAndAddSipfragBodyToSipMessage failed",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);
		sip_freeSipParam(pParam);
		pErr->errCode=Sdf_en_msgBodyError;
		return Sdf_co_fail;
	}
	sip_freeSipParam(pParam);
	sip_freeSipHeader(pHdr);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr,pErr);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)
		"SDF_DEBUG - Exiting sdf_ivk_uaFormNotify");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaHandleReplacement
**
** DESCRIPTION: This API indicates what action needs to be done when a
**		Replaces header matches an existing (early/confirmed) dialog.this
**      function checks for the presence of "early-only" parameter in the 
**		Replaces header.If it exists then only early dialogs are replaced.
**		If the dialog is in confirmed state the "INVITE" request containing
**      Replaces header will be given " 486 busy here" Response.
**
**
** PARAMETERS:
**		pSipMsg                 : sipMessage with Replaces header
**      pMatchedCallObj(IN)		: Call object which matched the Replaces hdr
**		pRespForNewCall(OUT)	: Response to be sent out on the new call
**		ppReqForMatchedCall(OUT): Request to be sent out on the matched call
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHandleReplacement
	(
	 SipMessage *pSipMsg,
	 Sdf_st_callObject *pMatchedCallObj,
	 Sdf_ty_u32bit *pRespForNewCall,
	 Sdf_ty_s8bit **ppReqForMatchedCall,
	 Sdf_st_error *pErr)
{
    Sdf_ty_state			dCallState = Sdf_en_idle;
    SipHeader 				dSipHdrReplaces;
    Sdf_ty_bool 			isearlyonly = Sdf_co_false;
    Sdf_ty_u32bit     dcount=0;
	SipParam   			*pParam = Sdf_co_null;

    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaHandleReplacement");

#ifdef SDF_PARAMVALIDATION
    if(pErr == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaHandleReplacement( ):"
                "Invalid pError parameter passed to the function");
#endif
        return Sdf_co_fail;
    }
    if (pMatchedCallObj == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaHandleReplacement():"
                "Invalid Call Object passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
    if (pSipMsg == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaHandleReplacement():"
                "Invalid SIP message passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif	/* end-of-param-validation */

    *pRespForNewCall = 0;
    *ppReqForMatchedCall = Sdf_co_null;

    /* Get the Call State Mutex of the Matched Dialog */
#ifdef SDF_THREAD_SAFE
    sdf_fn_uaLockMutex(0,&(pMatchedCallObj->dCallStateMutex),0);
#endif
    dCallState = pMatchedCallObj->pCallInfo->dState;

    if (Sdf_en_idle == dCallState)
    {
        /* This dialog was not established with INVITE */
        *pRespForNewCall = 481;
    }
    else if((Sdf_en_callTerminated == dCallState) || \
            (Sdf_en_byeSent == dCallState) || \
            (Sdf_en_byeReceived == dCallState))
    {
        /* This dialog was Terminated */
        *pRespForNewCall = 603;
    }

    /* check for the presence of early-only parameter in the Replaces
     * header in the sipmessage with Replaces header that is passed as 
     * a parameter to this function */

    dSipHdrReplaces.dType = SipHdrTypeUnknown;
    dSipHdrReplaces.pHeader = Sdf_co_null;

    /* Check if "Replaces" header is present */
    if (SipFail == sip_getHeader(pSipMsg, SipHdrTypeReplaces, \
                &dSipHdrReplaces, (SipError *)&(pErr->stkErrCode)))
    {
        if((SipError)pErr->stkErrCode == E_NO_EXIST)
        {
            pErr->errCode = Sdf_en_noExistError;
            return Sdf_co_fail;
        }
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaHandleReplacement():"
                "Failed to get Replaces header from Sip Message", pErr);
#endif
        return Sdf_co_fail;
    }

    /* get the param count in "Replaces" header */	
    if (SipFail == sip_getParamCountFromReplacesHdr(&dSipHdrReplaces, \
                &dcount,(SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
        sip_freeSipHeader(&dSipHdrReplaces);
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaHandleReplacement():"
                "Failed to get param count in Replaces header ", pErr);
#endif
        return Sdf_co_fail;
    }
    /* check whether early-only is present in the Replaces Header */
		while(dcount != 0)
    {
        /* Get  param */
        if (SipFail == sip_getParamAtIndexFromReplacesHdr (&dSipHdrReplaces,&pParam, 
                    dcount-1, (SipError *)&(pErr->stkErrCode)))
        {
            pErr->errCode = pErr->stkErrCode;
            sip_freeSipHeader(&dSipHdrReplaces);
#ifdef SDF_ERROR			
            sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                    (Sdf_ty_s8bit*)"sdf_ivk_uaIsEarlyonlyPresentInReplaces:"
                    "Failed to get param fron sip message ", pErr);
#endif
            return Sdf_co_fail;
        }

        if(Sdf_mc_strcmp(pParam->pName,"early-only")==0) 
        {	
            isearlyonly = Sdf_co_true;
            break;
        }
        dcount--;
    }
    /*free the local variables*/
	sip_freeSipParam(pParam);
	sip_freeSipHeader(&dSipHdrReplaces);

    if(isearlyonly == Sdf_co_true)
    {
        if (Sdf_en_callEstablished == dCallState)
        {
            /* established dialog */
            *pRespForNewCall = 486;
        }
    }
    else
    {	
        if (Sdf_en_callEstablished == dCallState)
        {
            /* established dialog */
            *pRespForNewCall = 200;
            *ppReqForMatchedCall = Sdf_mc_strdupCallHandler("BYE");
        }
    }
    if ((Sdf_en_rprReceived == dCallState) || \
            (Sdf_en_overlapTransactionReceived == dCallState) || \
            (Sdf_en_provisionalRespReceived == dCallState) || \
            (Sdf_en_reInviteprovRespReceived == dCallState))
    {
        /* This dialog was initialised by this UA */
        *pRespForNewCall = 200;
        *ppReqForMatchedCall = Sdf_mc_strdupCallHandler("CANCEL");
    }
    else if ((Sdf_en_rprSent == dCallState) || \
            (Sdf_en_overlapTransactionSent == dCallState) || \
            (Sdf_en_provisionalRespSent == dCallState) || \
            (Sdf_en_reInviteprovRespSent == dCallState))
    {
        /* This dialog was not initialised by this UA(peer initiated) */
        *pRespForNewCall = 481;
    }

#ifdef SDF_THREAD_SAFE
    sdf_fn_uaUnlockMutex(0, &(pMatchedCallObj->dCallStateMutex));
#endif
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaHandleReplacement");
    return Sdf_co_success;
}

#endif /* SDF_REFER */

#ifdef SDF_IMS
/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaAddExtraRespHeaders
 **
 ** DESCRIPTION: This function adds any extra headers that are set in the user
 **				 profile, into the call object's Processed and Unprocessed 
 **				 header list before calling the sdf_ivk_uaFormRequest API.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddExtraRespHeaders
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_ty_s8bit *pMethod,
	Sdf_st_error *pErr)
#else
	(pObject, pOverlapTransInfo, pMethod, pErr)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_s8bit *pMethod;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_initData *pInitData = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;
	Sdf_ty_slist *pUnprocessedHeaderList=Sdf_co_null;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddExtraHeaders");
	
	(void)pMethod;
	
#ifdef SDF_PARAMVALIDATION
	if (pErr==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddExtraHeaders(): pError passed is Invalid");
#endif
		return Sdf_co_fail;
	}
	if((pObject==Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
			"Call object parameter passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	if ((sdf_ivk_uaGetInitDataFromCallObject(pObject, &pInitData, \
			pErr)) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
			"Error accessing the initData from the call object",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Set the pProcessedHeaderList and pUnprocessedHeaderList to the correct
	 * list depending on whether pOverlapTransInfo is Sdf_co_null
	 */
	if(pOverlapTransInfo == Sdf_co_null)
	{
		pProcessedHeaderList = &(pObject->pUasTransaction->slProcessedHeaders);
		pUnprocessedHeaderList = \
			&(pObject->pUasTransaction->slUnprocessedHeaders);
	}
	else
	{
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
		pUnprocessedHeaderList = &(pOverlapTransInfo->slUnprocessedHeaders);
	}

	/*
	 * Iterate through the list of ExtraHeaders and append them to the
	 * appropriate list depending on whether the addalways attribute has been
	 * set
	 */
	(void)sdf_listInitIterator(&(pInitData->slRespHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pTemp;
		en_HeaderType pType;
		Sdf_ty_s8bit dTempType[Sdf_co_characterBufferSize]="";
    Sdf_ty_u32bit size,j;
		Sdf_st_headerList *pTempElement;
    Sdf_ty_s8bit *pUnk, *pBody;

		pTemp = (Sdf_st_configExtraHeaders*)\
			(dListIterator.pCurrentElement->pData);

		Sdf_mc_strcpy(dTempType,pTemp->pParam->pName);
		Sdf_mc_strcat(dTempType,":");

		/*
		 * Invoke stack API to convert the Header name into the stacks
		 * en_HeaderType enumeration
		 */
		if(sip_getTypeFromString(dTempType,&pType, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
			pErr->errCode = Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
				"Failed to get header type from string ",pErr);
#endif
			return Sdf_co_fail;
		}

		(void)sip_listSizeOf(&(pTemp->pParam->slValue), &size, \
 			  (SipError*)&(pErr->stkErrCode));

			if ( (pType != SipHdrTypeSupported) && 
					(pType !=SipHdrTypeProxyRequire)&&
					(pType != SipHdrTypeAccept) && 
					(pType != SipHdrTypeRequire) && 
					(pType != SipHdrTypeAllow) )
				pType = SipHdrTypeUnknown;

			/*
			 * Initialize a HeaderList structure to hold the 
			 * headers.
			 */
			if (sdf_ivk_uaInitHeaderList(&pTempElement, \
            	pType, Sdf_co_null, pErr) ==Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders( ):  "
					"Failed to Initialize HeaderList ",pErr);
#endif
				return Sdf_co_fail;
			}
					
			/*
			 * Iterate through the list of headers in the
			 * ExtraHeaders and set it into the HeaderList structure
			 */
			for (j = 0; j < size; j++)
			{
			  	Sdf_ty_s8bit *pTempVal, *pOptions;
			  	SipHeader *pHdr;

			  	if (sip_listGetAt(&(pTemp->pParam->slValue),  \
					(Sdf_ty_u32bit) j,(Sdf_ty_pvoid *)&pTempVal,  \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
			  	{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_extraHeaderAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to get value at index from header list",pErr);
#endif
					pErr->errCode=Sdf_en_extraHeaderAccessError;
					(void)sdf_ivk_uaFreeInitData(pInitData);
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					return Sdf_co_fail;
			  	}

				 	if (sip_initSipHeader(&pHdr, pType, \
						(SipError*)&(pErr->stkErrCode))==SipFail)
				 	{
						pErr->errCode=Sdf_en_headerManipulationError;
#ifdef SDF_ERROR	
						sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
							"Failed to Initialize SipHeader",pErr);
#endif
						(void)sdf_ivk_uaFreeInitData(pInitData);
		 				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
						return Sdf_co_fail;
				 	}

					/*
				  * Set the name in unknown header
				  */
					Sdf_mc_strdup(pUnk,pTemp->pParam->pName);

					if(sip_setNameInUnknownHdr(pHdr,pUnk, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
					{
					  	sip_freeSipHeader(pHdr);
					  	(void)sdf_memfree(Sdf_mc_freeMemId,(\
					 		Sdf_ty_pvoid*)&pHdr,pErr);
					  	(void)sdf_memfree(Sdf_mc_freeMemId,(\
					 		Sdf_ty_pvoid*)&pUnk,pErr);
					  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
							(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
								"Failed to set name in Unknown header ",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
						 	return Sdf_co_fail;
				 	}
    
					/*
					* Set body in unknown header
					*/
					Sdf_mc_strdup(pBody,pTempVal);

					if(sip_setBodyInUnknownHdr(pHdr,pBody, \
					 	(SipError *)&(pErr->stkErrCode))==SipFail)
					{
						  	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
								&pBody,pErr);
						  	sip_freeSipHeader(pHdr);
					 	  	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
								&pHdr,pErr);
						  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
							(void)sdf_ivk_uaFreeInitData(pInitData);
#ifdef SDF_ERROR
							sdf_fn_setError(Sdf_en_majorErrors,\
								Sdf_en_headerManipulationError,\
								(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
								"Failed to set body in Unknown header",pErr);
#endif
							pErr->errCode=Sdf_en_headerManipulationError;
					  	return Sdf_co_fail;
					}
				/*
				 * Add this header to the HeaderList
				 */
			  	if(sdf_listAppend(&(pTempElement->slHeaders), \
				 	(Sdf_ty_pvoid)pHdr,pErr) == Sdf_co_fail)
			   	{
					(void)sdf_ivk_uaFreeInitData(pInitData);
				  	(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				  	sip_freeSipHeader(pHdr);
				  	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pHdr, \
							pErr);
				  	(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
							(Sdf_ty_pvoid*)&pOptions,pErr);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_headerListAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported header to "
						"HeaderList structure",pErr);
#endif
					pErr->errCode=Sdf_en_headerListAccessError;
			   	  	return Sdf_co_fail;
			   	}
			} /* 
			   * End of for loop making a header list structure. Use the
			   * header list formed in the loop now
			   */ 	
			
			if(pTemp->pData==Sdf_co_null)
			{
				(void)sdf_ivk_uaFreeHeaderList(pTempElement);
				(void)sdf_listNext(&dListIterator, pErr);
				continue;
			}

			if(Sdf_mc_strcasecmp("yes",(Sdf_ty_s8bit *)pTemp->pData) \
				== 0)
			{
				/*
				 * The header should be inserted into every request.
				 * Hence add this HeaderList structure into the
				 * ProcessedHeaders list of the call object
				 */
		    	if(sdf_listAppend(pProcessedHeaderList, pTempElement, \
					pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported HeaderList to "
						"slProcessedHeaders List ",pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					(void)sdf_ivk_uaFreeInitData(pInitData);
					return Sdf_co_fail;
				}
			}
			else
			{
				/*
				 * The header should not be inserted into every request.
				 * Hence add this HeaderList structure into the
				 * UnProcessedHeaders list of the call object
				 */
				if(sdf_listAppend(pUnprocessedHeaderList,pTempElement, \
					pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAddExtraHeaders(): "
						"Failed to append Supported HeaderList to "
						"slUnprocessedHeaders List ",pErr);
#endif
					pErr->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_ivk_uaFreeHeaderList(pTempElement);
					(void)sdf_ivk_uaFreeInitData(pInitData);
					return Sdf_co_fail;
				}
			}
		(void)sdf_listNext(&dListIterator, pErr);
	} /* End of for loop */

	/* Free the local reference */
	(void)sdf_ivk_uaFreeInitData(pInitData);
	
  	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaAddExtraHeaders()\n");
	return Sdf_co_success;	
}
#endif


