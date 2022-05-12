/*****************************************************************************
 ** FUNCTION: 		Header level encoding APIs
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_hdrencoders.c
 **
 ** DESCRIPTION:	This file contains code to convert from structures 
 **					to SIP Text buffer.
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.c		Initial
 **					Toshniwal		of SIP Stack
 ** 03-Mar-2202		Aparna				-				Added Auth, From
 **					Kuppachi							& String Family
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "microsip_struct.h"
#include "microsip_hdrencoders.h"
#include "microsip_encodeinternal.h"
#include "microsip_init.h"


/****************************************************************************
** FUNCTION: sip_formSipParamList
** DESCRIPTION: This function converts a list of SipParams to text. 
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				list(IN)		- The list of SipParams to be converted 
**				separator(IN)	- The separator to be used between the params in the
**								  output text message
**				leadingsep(IN)	- The separator character to be used at the
**								  beginning of the encoded list
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formSipParamList
#ifdef ANSI_PROTO
	(SIP_S8bit	*pOut,
	SipList 	*list,
	SIP_S8bit	*separator,
	SIP_U8bit	leadingsep,
	SipError	*pError)
#else
	(pOut, list, separator, leadingsep, pError)
	SIP_S8bit *pOut;
	SipList *list;
	SIP_S8bit *separator;
	SIP_U8bit leadingsep;
	SipError *pError;
#endif
{
	SIP_U32bit	listSize,listIter;

	(void)sip_listSizeOf(list, &listSize, pError);
	listIter = 0;
	while (listIter < listSize)
	{
		SipParam		*pParam;
		SIP_U32bit		valueSize;
		SIP_S8bit		*value;
		SIP_U32bit		valueIter=0;

		/* 
		 * For each element in the list of params, do the following:
		 * If not the first element, first output the separator character.
		 * Then print the param name, followed by a "=" and then the list of
		 * param values.
		 */

		(void)sip_listGetAt(list, listIter, (SIP_Pvoid *) &pParam, pError);
		if ((listIter!=0)||(leadingsep!=0))
			(void)STR_CAT ( (SIP_S8bit *)pOut, separator);
			
		(void)STR_CAT((SIP_S8bit *) pOut,pParam->pName);
		(void)sip_listSizeOf( &(pParam->slValue), &valueSize, pError);
		
		if (valueSize >= 1)
		{
			/* 
			 * Output "=" only if a NON_NULL element is present.
			 */

			(void)sip_listGetAt(&(pParam->slValue), valueIter, \
				(SIP_Pvoid *) &value, pError);
			if (value != SIP_NULL)
			{
				(void)STR_CAT((SIP_S8bit *) pOut, (SIP_S8bit*)"=");
				while (valueIter < valueSize)
				{
					if (valueIter > 0)
						(void)STR_CAT((SIP_S8bit *) pOut, (SIP_S8bit*)",");
					(void)sip_listGetAt(&(pParam->slValue), valueIter, \
						(SIP_Pvoid *) &value, pError);
					(void)STR_CAT((SIP_S8bit *) pOut,value);
					valueIter++;
				}
			}
		}
		listIter++;
	} /* while */

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formAddrSpec
** DESCRIPTION: This function converts a SipAddrSpec structure to text 
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				pAddrSpec(IN)	- The addr spec structure to be converted 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formAddrSpec 
#ifdef ANSI_PROTO
	(
	SIP_S8bit *pDummyBuf,
	SIP_S8bit	*pOut, 
	SipAddrSpec	*pAddrSpec, 
	SipError	*pError)
#else
	(pOut, pAddrSpec, pError)
	SIP_S8bit 	*pOut;
	SipAddrSpec *pAddrSpec;
	SipError 	*pError;
#endif
{
	(void) pDummyBuf;

	if ((pAddrSpec->dType == SipAddrSipUri) || \
		(pAddrSpec->dType == SipAddrSipSUri))
	/* Address is a SIP Uri, so more dissection */
	{
		SipUrl *pSipUrl = pAddrSpec->u.pSipUrl;
		
		if (pAddrSpec->dType == SipAddrSipUri)
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)"sip:");
		else
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)"sips:");	
		(void)STR_CAT ( (SIP_S8bit *)pOut, pSipUrl->pUser);

		if (pSipUrl->pPassword != SIP_NULL)
		{
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)":");
			(void)STR_CAT ( (SIP_S8bit *)pOut, pSipUrl->pPassword);
		}

		/* if pUser was there, add an @ after pUser:[passwd] */
		if (pSipUrl->pUser !=SIP_NULL) 
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)"@");

		(void)STR_CAT ( (SIP_S8bit *)pOut, pSipUrl->pHost);

		/* see if its pHost or pHost:dPort */
		if (pSipUrl->dPort != SIP_NULL)
		{
			SIP_S8bit porttext[6]; /* max val of dPort = 65535 */
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)":");
			(void)sip_snprintf((SIP_S8bit *)porttext, 6, "%u", *(pSipUrl->dPort) );
			porttext[5]='\0';
			(void)STR_CAT ( (SIP_S8bit *)pOut, porttext);
		}
		
		(void)sip_formSipParamList(pOut, &(pSipUrl->slParam), \
			(SIP_S8bit *)";",1, pError);

		/* Now parse Header */
		if (pSipUrl->pHeader !=SIP_NULL)
		{
			(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)"?");
			(void)STR_CAT ( (SIP_S8bit *)pOut, pSipUrl->pHeader);
		}
	} /* End of if (ch->pAddrSpec->dType == SipAddrSipUri/SipAddrSipSUri) */
	else
		/* Address is  a pUri */
		(void)STR_CAT ( (SIP_S8bit *)pOut, pAddrSpec->u.pUri);

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formFromFamily
** DESCRIPTION: This function forms textual strings for the following header 
**				types: 
**					1. From			4. Record-Route
**			  		2. To			5. Reffered-By
**					3. Route
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				pSipMsg(IN)		- The SipMessage structure
**				dIndex(IN)		- The index of the header to be encoded 
**				dMode(IN)		- The mode of the header to be encoded
**				dForm(IN)		- The form of the header to be encoded
**				dType(IN)		- The header type 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formFromFamily
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SipMessage *pSipMsg, SIP_U32bit dIndex, \
	 en_AdditionMode dMode, en_HeaderForm dForm, en_HeaderType dType, \
	 SipError *pError)
#else
	(pOut, pSipMsg, dIndex, dMode, dForm, dType, pError)
	SIP_S8bit 		*pOut;
	SipMessage 		*pSipMsg;
	SIP_U32bit 		 dIndex;
	en_AdditionMode  dMode; 
	en_HeaderForm 	 dForm;
	en_HeaderType	 dType;
	SipError 		*pError;
#endif
{
	SipFromHeader 	*pHeader = SIP_NULL;

	/*
	 * Form the header name based on header type
	 */

	switch (dType)
	{
		case SipHdrTypeFrom:
			pHeader = pSipMsg->pGeneralHdr->pFromHeader;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"From: ",\
				(SIP_S8bit*)"f: ");
			break;
			
		case SipHdrTypeTo:
			pHeader = pSipMsg->pGeneralHdr->pToHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"To: ", \
				(SIP_S8bit*)"t: ");
			break;

		case SipHdrTypeRoute:

			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slRouteHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Route: ", 
				(SIP_S8bit*)"Route: ");
			break;

		case SipHdrTypeRecordRoute:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slRecordRouteHdr), dIndex, (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Record-Route: ", \
				(SIP_S8bit*)"Record-Route: ");
			break;
#ifdef SIP_3GPP			
	case SipHdrTypePath:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slPathHdr), dIndex, (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Path: ", \
				(SIP_S8bit*)"Path: ");
			break;
	case SipHdrTypePAssociatedUri:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pResponse->\
				pResponseHdr->slPAssociatedUriHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Associated-URI: ", \
				(SIP_S8bit*)"P-Associated-URI: ");
			break;
	case SipHdrTypePCalledPartyId:
			if (pSipMsg->dType != SipMessageRequest)
				return SipFail;
			pHeader = pSipMsg->u.pRequest->pRequestHdr->pPCalledPartyIdHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Called-Party-ID: ", \
				(SIP_S8bit*)"P-Called-Party-ID: ");
			break;

	case SipHdrTypeServiceRoute:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pResponse->\
							pResponseHdr->slServiceRouteHdr), dIndex, \
						    (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Service-Route: ", \
				(SIP_S8bit*)"Service-Route: ");
		break;
	case SipHdrTypeHistoryInfo:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slHistoryInfoHdr), dIndex, (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"History-Info: ", \
				(SIP_S8bit*)"History-Info: ");
			break;

#endif
#ifdef SIP_PRIVACY
		case SipHdrTypePPreferredId:
			if (SipFail == sip_listGetAt(&(pSipMsg-> \
							pGeneralHdr->slPPreferredIdHdr), dIndex, \
						    (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, \
                    (SIP_S8bit*)"P-Preferred-Identity: ",  (SIP_S8bit*)"P-Preferred-Identity: ");
			break;

		case SipHdrTypePAssertId:
				if (SipFail == sip_listGetAt(&(pSipMsg-> \
							pGeneralHdr->slPAssertIdHdr), dIndex, \
						    (SIP_Pvoid *) &pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, \
				(SIP_S8bit*)"P-Asserted-Identity: ",  (SIP_S8bit*)"P-Asserted-Identity: ");
			break;
#endif

#ifdef SIP_REFER
		case SipHdrTypeReferredBy:
			if (pSipMsg->dType != SipMessageRequest)
				return SipFail;
			pHeader = pSipMsg->u.pRequest->pRequestHdr->pReferredByHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, \
				(SIP_S8bit*)"Referred-By: ",  (SIP_S8bit*)"b: ");
			break;
		
		case SipHdrTypeReferTo:
			if (pSipMsg->dType != SipMessageRequest)
				return SipFail;
			pHeader = pSipMsg->u.pRequest->pRequestHdr->pReferToHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, \
				(SIP_S8bit*)"Refer-To: ",  (SIP_S8bit*)"r: ");
			break;
	
#endif
			
		default:
			break; /* Should not reach here */
	} /* end of switch on hdr type */

	/*
	 * Now encode header body
	 */

#ifdef SIP_INCREMENTAL_PARSING			
			if (pHeader->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pBuffer);
			else
			{
#endif

				/*
				 * Encode display name
				 */

				if (pHeader->pStr1 != SIP_NULL)
				{
					(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pStr1);
				}

				/*
				 * Encode addr spec 
				 */

				if (pHeader->pAddrSpec != SIP_NULL)
				{
					(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)"<");
					(void)sip_formAddrSpec (SIP_NULL, pOut, pHeader->pAddrSpec, pError);
					(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)">");
				}

				/* Now get extension Param */
				(void)sip_formSipParamList(pOut,&(pHeader->slParam),(SIP_S8bit *)";",1,pError);

				/* CRLF will be inserted at invocation place */
#ifdef SIP_INCREMENTAL_PARSING	
			}
#endif
	
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formAuthFamily
** DESCRIPTION: This function forms textual strings for the following
**				  	header types:
**					1. Www-Authenticate			4. Proxy-Authorization
**			  		2. Proxy-Authenticate	
**					3. Authorization
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				pSipMsg(IN)		- The SipMessage structure
**				dIndex(IN)		- The index of the header to be encoded 
**				dMode(IN)		- The mode of the header to be encoded
**				dForm(IN)		- The form of the header to be encoded
**				dType(IN)		- The header type 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool sip_formAuthFamily
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SipMessage *pSipMsg, SIP_U32bit dIndex, \
	 en_AdditionMode dMode, en_HeaderForm dForm, en_HeaderType dType, \
	 SipError *pError)
#else
	(pOut, pSipMsg, dIndex, dMode, dForm, dType, pError)
	SIP_S8bit 		*pOut;
	SipMessage 		*pSipMsg;
	SIP_U32bit 		 dIndex;
	en_AdditionMode  dMode; 
	en_HeaderForm 	 dForm;
	en_HeaderType	 dType;
	SipError 		*pError;
#endif
{
	SipAuthorizationHeader 	*pHeader = SIP_NULL;

	/*
	 * Form the header name based on the header type
	 */

	if(dType > SipHdrTypeAny)
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}
	switch (dType)
	{
		case SipHdrTypeAuthorization:
		{
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slAuthorizationHdr), dIndex,(SIP_Pvoid* )&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Authorization: ", \
				(SIP_S8bit*)"Authorization: ");
			break;
		}
			
		case SipHdrTypeProxyauthorization:
		{
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->pRequestHdr->\
				slProxyAuthorizationHdr), dIndex,(SIP_Pvoid* )&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			} 
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Proxy-Authorization: ", \
				(SIP_S8bit*)"Proxy-Authorization: ");
			break;
		}

		case SipHdrTypeProxyAuthenticate:
		{
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pResponse->\
				pResponseHdr->slProxyAuthenticateHdr), dIndex, \
				(SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Proxy-Authenticate: ", \
				(SIP_S8bit*)"Proxy-Authenticate: ");
			break;
		}

		case SipHdrTypeWwwAuthenticate:
		{
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pResponse->\
				pResponseHdr->slWwwAuthenticateHdr), dIndex, \
				(SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Www-Authenticate: ", \
				(SIP_S8bit*)"Www-Authenticate: ");
			break;
		}
		default:
			break; /* Should not reach here */

	} /* end of switch on hdr type */

			/*
			 * Encode the header body if parsed
			 */

#ifdef SIP_INCREMENTAL_PARSING			
			if (pHeader->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pBuffer);
			else
			{
#endif
				/* Encode Scheme */
				if (pHeader->pStr1 != SIP_NULL)
				{
					(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pStr1);
					if (sip_strcmp(pHeader->pStr1,(SIP_S8bit*)""))
					(void)STR_CAT ( (SIP_S8bit *)pOut, (SIP_S8bit*)" ");
				}
	
				/* Now encode the params if any */
				(void)sip_formSipParamList(pOut,&(pHeader->slParam),(SIP_S8bit *)",",\
							0,pError);
		
#ifdef SIP_INCREMENTAL_PARSING			
			}
#endif 		
		
	/* CRLF will be inserted at invocation place */
	return SipSuccess;

}

#endif
/****************************************************************************
** FUNCTION: sip_formStringFamily
** DESCRIPTION: This function forms textual strings for the following
**				  	header types:
**					Require, Allow, Allow-Events, Supported,
**					Unsupported, Call-ID, Content-Encoding 
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				pSipMsg(IN)		- The SipMessage structure
**				dIndex(IN)		- The index of the header to be encoded 
**				dMode(IN)		- The mode of the header to be encoded
**				dForm(IN)		- The form of the header to be encoded
**				dType(IN)		- The header type 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formStringFamily
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SipMessage *pSipMsg, SIP_U32bit dIndex, \
	 en_AdditionMode dMode, en_HeaderForm dForm, en_HeaderType dType, \
	 SipError *pError)
#else
	(pOut, pSipMsg, dIndex, dMode, dForm, dType, pError)
	SIP_S8bit 		*pOut;
	SipMessage 		*pSipMsg;
	SIP_U32bit 		 dIndex;
	en_AdditionMode  dMode; 
	en_HeaderForm 	 dForm;
	en_HeaderType	 dType;
	SipError 		*pError;
#endif
{
	SipUnsupportedHeader 	*pHeader = SIP_NULL;

	/* Form header name based on header type */

	switch (dType)
	{
		case SipHdrTypeUnsupported:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pResponse->\
				pResponseHdr->slUnsupportedHdr), dIndex,\
				(SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Unsupported: ", \
				(SIP_S8bit*)"Unsupported: ");
			break;
			
		case SipHdrTypeSupported:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slSupportedHdr), dIndex,(SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			} 
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Supported: ", \
				(SIP_S8bit*)"k: ");
			break;

		case SipHdrTypeAllow:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slAllowHdr), dIndex, (SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Allow: ", \
				(SIP_S8bit*)"Allow: ");
			break;

#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slAllowEventsHdr), dIndex, (SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Allow-Events: ", \
				(SIP_S8bit*)"u: ");
			break;
#endif

		case SipHdrTypeRequire:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slRequireHdr), dIndex, (SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Require: ", \
				(SIP_S8bit*)"Require: ");
			break;

		case SipHdrTypeCallId:
			pHeader = pSipMsg->pGeneralHdr->pCallidHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Call-ID: ", \
				(SIP_S8bit*)"i: ");
			break;

		case SipHdrTypeContentEncoding:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slContentEncodingHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Content-Encoding: ", \
				(SIP_S8bit*)"e: ");
			break;
#ifdef SIP_PEMEDIA
           case SipHdrTypePEarlyMedia:
			if (SipFail == sip_listGetAt(&(pSipMsg->pGeneralHdr->\
				slPEarlyMediaHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Early-Media: ", (SIP_S8bit*)" P-Early-Media: ");
			break;
#endif
#ifdef SIP_3GPP
			case SipHdrTypeRequestDisposition:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slRequestDispositionHdr), dIndex,\
				(SIP_Pvoid *)&pHeader, pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Request-Disposition: ", \
				(SIP_S8bit*)"Request-Disposition: ");
			break;

			case SipHdrTypeIfMatch:
			{
			if (pSipMsg->dType != SipMessageRequest)
				return SipFail;

			pHeader = pSipMsg->u.pRequest->pRequestHdr->pIfMatchHdr;

			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"SIP-If-Match: ", \
				(SIP_S8bit*)"SIP-If-Match: ");
			break;
			}

			case SipHdrTypeETag:
			{
			if (pSipMsg->dType != SipMessageResponse)
				return SipFail;

			pHeader = pSipMsg->u.pResponse->pResponseHdr->pETagHdr;

			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"SIP-ETag: ", \
				(SIP_S8bit*)"SIP-ETag: ");
			break;
			}

#endif

		default:
			break; /* Should not reach here */
	} /* end of switch on hdr type */

	/* Now form the header body */

#ifdef SIP_INCREMENTAL_PARSING			
	if (pHeader->pBuffer != SIP_NULL)
		(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pBuffer);
	else
	{
#endif
		(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pStr1);
#ifdef SIP_INCREMENTAL_PARSING
	}
#endif

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formSdpConnection
** DESCRIPTION: This function forms textual string for the SDP c= line.
** 
** PARAMETERS:
**				pOut(OUT)			- The output c= line 
**				pSdpConnection(IN)	- The SdpConnection structure to be encoded 
**				pError(OUT)			- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formSdpConnection
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SdpConnection *pSdpConnection, SipError *pError)
#else
	(pOut, pSdpConnection, pError)
	SIP_S8bit 		*pOut;
	SdpConnection	*pSdpConnection;
	SipError 		*pError;
#endif
{
	/* 
	 * Form a c= line from the input SdpConnection structure.
	 */

	(void)STR_CAT (pOut, (SIP_S8bit*)"c=");
	STRCAT_AND_SPACE (pOut, pSdpConnection->pNetType);
	STRCAT_AND_SPACE (pOut, pSdpConnection->pAddrType);
	(void)STR_CAT (pOut, pSdpConnection->pAddr);
	(void)STR_CAT (pOut, (SIP_S8bit*)CRLF);

	*pError = E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formListOfBandwidthVal
** DESCRIPTION: This function forms textual string for the list of
**					SDP b= lines.
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer with the encoded
**								  b= lines
**				pslBandwidth(IN)- The bandwidth list to be encoded 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formListOfBandwidthVal
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SipList *pslBandwidth, SipError *pError)
#else
	(pOut, pslBandwidth, pError)
	SIP_S8bit 		*pOut;
	SipList			*pslBandwidth;
	SipError 		*pError;
#endif
{
	SIP_U32bit	dSize = 0;
	SIP_U32bit	dIndex = 0;

	(void)sip_listSizeOf(pslBandwidth, &dSize, pError);

	/* 
	 * Form a b= line for each element in the list.
	 */

	while (dIndex< dSize)
	{
		SIP_S8bit *pBandwidth;

		(void)sip_listGetAt(pslBandwidth, dIndex, (SIP_Pvoid *) &pBandwidth, pError);
		(void)STR_CAT(pOut, (SIP_S8bit*)"b=");
		(void)STR_CAT (pOut, pBandwidth);
		(void)STR_CAT (pOut,(SIP_S8bit*) CRLF);
		dIndex++;
	}

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formListOfAttributes
** DESCRIPTION: This function forms textual string for the list of
**					SDP a= lines. 
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer with the encoded
**								  a= lines
**				pslAttribList(IN)- The SDP attributes list to be encoded 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formListOfAttributes
#ifdef ANSI_PROTO
	(SIP_S8bit *pOut, SipList *pslAttribList, SipError *pError)
#else
	(pOut, pslAttribList, pError)
	SIP_S8bit 	*pOut;
	SipList		*pslAttribList;
	SipError 	*pError;
#endif
{
	SIP_U32bit	dSize = 0;
	SIP_U32bit	dIndex = 0;

	(void)sip_listSizeOf(pslAttribList, &dSize, pError);

	/* 
	 * Form a a= line for each element in the list.
	 */

	while (dIndex < dSize)
	{
		SdpAttr *pAttr;
		(void)sip_listGetAt (pslAttribList, dIndex, (SIP_Pvoid *)&pAttr, pError);
		(void)STR_CAT (pOut, (SIP_S8bit*)"a=");
		(void)STR_CAT (pOut, pAttr->pName);
		if(pAttr->pValue != SIP_NULL)
		{
			(void)STR_CAT (pOut, (SIP_S8bit*)":");
			(void)STR_CAT (pOut, pAttr->pValue);
		}
		(void)STR_CAT (pOut, (SIP_S8bit*)CRLF);
		dIndex++;
	} /* while Attr */

	return SipSuccess;

}


#ifdef SIP_DATE
/****************************************************************************
** FUNCTION: sip_formDateStruct
** DESCRIPTION: This function converts a SipDateStruct to text.
** 
** PARAMETERS:
**				ppOut(OUT)		- The output text buffer with the encoded
**								 	date struct 
**				dt(IN)			- The SipDate struct to be encoded 
**				err(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formDateStruct
#ifdef ANSI_PROTO
	(
	SIP_S8bit **ppOut,
	SipDateStruct *dt,
	SipError *err)
#else
	(ppOut, dt, err)
	SIP_S8bit **ppOut;
	SipDateStruct *dt;
	SipError *err;
#endif
{
	SIP_S8bit days[7][4]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	SIP_S8bit dMonth[12][4]={"Jan", "Feb", "Mar", "Apr", "May", \
							"Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	SIP_S8bit temp[20];
	SipDateFormat *df;
	SipTimeFormat *tf;
	SIP_S8bit* out;
  (void)err;	
	SIPDEBUGFN("Entering into  sip_formDateStruct");
	
	out = *ppOut;
	if (dt == SIP_NULL) 
		return SipSuccess;
	if (dt->dDow != SipDayNone)
	{
		(void)STR_CAT ( out, days[(int)(dt->dDow)]);
		(void)STR_CAT ( out,(SIP_S8bit *)", ");
	}
	df = dt->pDate;
	(void)sip_snprintf((SIP_S8bit *)temp, 20, "%02d", df->dDay);
	temp[20-1]='\0';
	(void)STR_CAT ( out, temp);
	(void)STR_CAT ( out, (SIP_S8bit *)" ");
	(void)STR_CAT ( out, dMonth[(int)(df->dMonth)]);
	(void)STR_CAT ( out,(SIP_S8bit *)" ");
	(void)sip_snprintf((SIP_S8bit *)temp, 20, "%04d", df->dYear);
	temp[ 20-1]='\0';
	(void)STR_CAT ( out,temp);
	(void)STR_CAT ( out, (SIP_S8bit *)" ");
	/* Now get slTime */
	tf = dt->pTime;
	(void)sip_snprintf((SIP_S8bit *)temp, 20, "%02d:%02d:%02d ", \
		tf->dHour, tf->dMin, tf->dSec );
	temp[ 20-1]='\0';
	(void)STR_CAT ( out, temp);
	(void)STR_CAT ( out,(SIP_S8bit *)"GMT");
	*ppOut = out;

	SIPDEBUGFN("Exiting from sip_formDateStruct");
	return SipSuccess;
}
#endif /* SIP_DATE */

#ifdef SIP_3GPP

/****************************************************************************
** FUNCTION: sip_form3GPPHeader
** DESCRIPTION: This function forms textual strings for the following header 
**				types: 
**					1. PVisitedNetworkInfo	5.Accept-Contact
**			  		2. PanInfo				6.Reject-Contact
**					3. PcfAddr				7. Join
**					4. PcVector
** 
** PARAMETERS:
**				pOut(OUT)		- The output text buffer 
**				pSipMsg(IN)		- The SipMessage structure
**				dIndex(IN)		- The index of the header to be encoded 
**				dMode(IN)		- The mode of the header to be encoded
**				dForm(IN)		- The form of the header to be encoded
**				dType(IN)		- The header type 
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_form3GPPHeader
	(SIP_S8bit *pOut, SipMessage *pSipMsg, SIP_U32bit dIndex, \
	 en_AdditionMode dMode, en_HeaderForm dForm, en_HeaderType dType, \
	 SipError *pError)
{
	SipCommonHeader 	*pHeader = SIP_NULL;

	/*
	 * Form the header name based on header type
	 */

	switch (dType)
	{
		
	case SipHdrTypePVisitedNetworkId:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slPVisitedNetworkIdHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Visited-Network-ID: ", \
				(SIP_S8bit*)"P-Visted-Network-ID: ");
			break;

		
  case SipHdrTypePcfAddr:
			pHeader = pSipMsg->pGeneralHdr->pPcfAddrHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Charging-Function-Addresses: ",\
				(SIP_S8bit*)"P-Charging-Function-Addresses: ");
			break;	
		
		
	case SipHdrTypeAcceptContact:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slAcceptContactHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Accept-Contact: ", \
				(SIP_S8bit*)"Accept-Contact: ");
			break;
	case SipHdrTypeRejectContact:
			if (SipFail == sip_listGetAt(&(pSipMsg->u.pRequest->\
				pRequestHdr->slRejectContactHdr), dIndex, (SIP_Pvoid *)&pHeader, \
				pError))
				return SipFail;
			if (SIP_NULL == pHeader)	
			{
				*pError = E_INV_PARAM;
				return SipFail;
			}
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Reject-Contact: ", \
				(SIP_S8bit*)"Reject-Contact: ");
			break;

		case SipHdrTypePanInfo:
		{
			pHeader = pSipMsg->pGeneralHdr->pPanInfoHdr;
			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Access-Network-Info: ",\
				(SIP_S8bit*)"P-Access-Network-Info: ");
			break;
		}
		case SipHdrTypeJoin:
		{
			if (pSipMsg->dType != SipMessageRequest)
				return SipFail;

			pHeader = pSipMsg->u.pRequest->pRequestHdr->pJoinHdr;

			if (SipFail == VALIDATE_HEADER(pHeader, dIndex, pError))
				return SipFail;
			
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"Join: ", \
				(SIP_S8bit*)"Join: ");
			break;
		}

		case SipHdrTypePcVector:  
		{
			pHeader = pSipMsg->pGeneralHdr->pPcVectorHdr;

			if (VALIDATE_HEADER(pHeader, dIndex, pError) == SipFail)
				return SipFail;
				
			sip_formHeaderName(dMode, dForm, pOut, (SIP_S8bit*)"P-Charging-Vector: ", \
				(SIP_S8bit*)"P-Charging-Vector: ");

			break;
		} /* end of PcVector Header */

		default:
			break; /* Should not reach here */
	} /* end of switch on hdr type */

	/*
	 * Now encode header body
	 */

#ifdef SIP_INCREMENTAL_PARSING			
			if (pHeader->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pBuffer);
			else
			{
#endif

				/*
				 * Encode display name
				 */

				if (pHeader->pStr1 != SIP_NULL)
				{
					(void)STR_CAT ( (SIP_S8bit *)pOut, pHeader->pStr1);
				}

				/* Now get extension Param */
				(void)sip_formSipParamList(pOut,&(pHeader->slParam),(SIP_S8bit *)";",1,pError);

				/* CRLF will be inserted at invocation place */
#ifdef SIP_INCREMENTAL_PARSING	
			}
#endif
	
	return SipSuccess;
}

#endif
