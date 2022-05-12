#ifdef SDF_AUTHENTICATE

/******************************************************************************
 ** FUNCTION:	
 **			This file has all the API's related to authentication at the
 **			UAC/UAS end
 ** 	
 ******************************************************************************
 **
 ** FILENAME		: sdf_authorization.c
 **
 ** DESCRIPTION		: This file has all the API's related to 
 **					  authentication at the UAC/UAS end
 **
 ** DATE			NAME		REFERENCE		REASON
 ** ----			----		---------		------
 ** 15/01/01		Subhash						Creation
 ** 16/02/01		Seshashayi					UAS part of authentication
 ** 07/08/01		Subhash						Revamped authentication to
 **												issue new callbacks 
 ** 22/04/03        Anil Kumar     ---          Changes for TLS feature.
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"
#include "basic.h"
#include "digcalc.h"

#include "sdf_callapis.h"
#include "sdf_authorization.h"
#include "sdf_init.h"
#include "sdf_network.h"
#include "sdf_sdp.h"
#include "sdf_trace.h"

#include "sdf_accessor.h"
#include "sdf.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_callbacks.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_portlayer.h"
#include "sdf_trace.h"
#include "sdf_list.h"
#endif

/*********************************************************
** FUNCTION: sdf_fn_uaFormAuthorizationCredential
**
** DESCRIPTION: Forms the credential to be sent to authorize
** a user.
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaFormAuthorizationCredential
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg,
	Sdf_ty_s8bit *pMethod,
	SipGenericCredential **ppGenericCredential,
	Sdf_st_authenticationParams *pAuthenticationParams,
	Sdf_st_uacAuthInfo *pUacAuthInfo,
	Sdf_st_error *pError)
#else	
	(pSipMsg, pMethod, ppGenericCredential, pAuthenticationParams, 
		pUacAuthInfo, pError)
	SipMessage *pSipMsg;
	Sdf_ty_s8bit *pMethod;
	SipGenericCredential **ppGenericCredential;
	Sdf_st_authenticationParams *pAuthenticationParams;
	Sdf_st_uacAuthInfo *pUacAuthInfo;
	Sdf_st_error *pError;
#endif
{
	HASHHEX HA1;
	HASHHEX HEntity="";
	SipReqLine *pReqLine = Sdf_co_null;
	SipAddrSpec *pTempSpec = Sdf_co_null;
	Sdf_ty_s8bit *pResponse=Sdf_co_null;
	Sdf_ty_s8bit dResponse[HASHHEXLEN+1];
	Sdf_ty_s8bit *pUri = Sdf_co_null;
	Sdf_ty_s8bit tempUri[Sdf_co_smallBufferSize] = "";
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;


	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_fn_uaFormAuthorizationCredential");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaFormAuthorizationCredential():  Invalid error parameter");
#endif		
		return Sdf_co_fail;
	}

	if(pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
			"SipMessage parameter passed is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif	/* End of param validation ifdef */
	*ppGenericCredential=Sdf_co_null;


	/* 
	 * Get the URI from the RequestLine to be used in the uri parameter
	 * of the response in case of Digest scheme
	 */
	if (Sdf_mc_strcasecmp(pAuthenticationParams->pScheme, "Digest") == 0)
	{
		if (sip_getReqLine(pSipMsg, &pReqLine, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
				"Failed to get the reqest line from message",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		
		if (sip_getAddrSpecFromReqLine(pReqLine, &pTempSpec, \
			 (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
				"Failed to get the address spec from Request line",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipReqLine(pReqLine);
			return Sdf_co_fail;
		}

		if(pTempSpec->dType == SipAddrReqUri)
		{
			if (sip_getUriFromAddrSpec(pTempSpec, &pUri,  \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
					"Failed to get the Uri from Addr Spec",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipAddrSpec(pTempSpec);
				sip_freeSipReqLine(pReqLine);
				return Sdf_co_fail;
			}
			/*
			 * pUri will now hold the "uri" parameter to be sent in the
			 * Authorization/Proxy-Authorization header
			 */
		}
		else 
		{
			SipUrl *pUrl;
			Sdf_ty_s8bit *name,*host;
			Sdf_ty_u16bit dPort = 0;

			if(sdf_ivk_uaGetUrlFromAddrSpec(pTempSpec, &pUrl,\
					pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
					"Failed to get the Url from Addr Spec",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipAddrSpec(pTempSpec);
				sip_freeSipReqLine(pReqLine);
				return Sdf_co_fail;
			}
	
			if ((sip_getUserFromUrl(pUrl, &name,(SipError*)\
				&(pError->stkErrCode))) == SipFail)
				name = Sdf_co_null;
	
			if ((sip_getHostFromUrl(pUrl, &host,(SipError*)\
					&(pError->stkErrCode)))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential(): "
					"Failed to get the host from Url",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pUrl);
				sip_freeSipAddrSpec(pTempSpec);
				sip_freeSipReqLine(pReqLine);
				return Sdf_co_fail;
			}
	
			(void)sip_getPortFromUrl(pUrl, &dPort,(SipError*)&(pError->stkErrCode));
			
			if (name != Sdf_co_null)
			{
				if (dPort != 0)
					(void)sdf_fn_uaSprintf(tempUri,"sip:%s@%s:%d", name, host, dPort);
				else
					(void)sdf_fn_uaSprintf(tempUri, "sip:%s@%s", name, host);
			}
			else
			{
				if (dPort != 0)
					(void)sdf_fn_uaSprintf(tempUri, "sip:%s:%d", host, dPort);
				else
					(void)sdf_fn_uaSprintf(tempUri, "sip:%s", host);
			}
	
			/*
			 * pUri will now hold the "uri" parameter to be sent in the
			 * Authorization/Proxy-Authorization header
			 */
			pUri = tempUri;
	
			/* Free the local reference */
			sip_freeSipUrl(pUrl);
		}
		/* Free the local reference */
		sip_freeSipAddrSpec(pTempSpec);
		sip_freeSipReqLine(pReqLine);
	} /* End of "if Digest pick up Uri" */

	if (Sdf_mc_strcasecmp(pAuthenticationParams->pScheme, "Basic") == 0)
	{
		/* 
		 * Server has requested Basic Authentication scheme
		 */
		Sdf_ty_s8bit dBasicEncodeInput[Sdf_co_smallBufferSize]="";
		Sdf_ty_s8bit dBasicResponse[Sdf_co_smallBufferSize]="";

		if((pUacAuthInfo->pUserName!=Sdf_co_null)&& \
			(pUacAuthInfo->pPassword!=Sdf_co_null))
		{
			(void)sdf_fn_uaSprintf(dBasicEncodeInput, "%s:%s", \
				pUacAuthInfo->pUserName, pUacAuthInfo->pPassword);
		}
		/*
		 * Encode the "username:password" string using the 
		 * encodeBase64 algorithm
		 */
		encodeBase64(dBasicEncodeInput, dBasicResponse);
		pResponse = Sdf_mc_strdupAuth(dBasicResponse);
	}
	else
	{
		/* 
		 * Server has requested Digest Authentication scheme
		 */
#ifdef ICF_PORT_SYMBIAN
		Sdf_ty_s8bit *dEntityBody=Sdf_co_null;
#else
		Sdf_ty_s8bit dEntityBody[Sdf_mc_maxMsgSize];
#endif /*ICF_PORT_SYMBIAN*/		
		Sdf_ty_u32bit dLength=0;
		Sdf_ty_u32bit dMsgBodyCount=0;
		Sdf_ty_s8bit dAuthRealm[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit dAuthNonce[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit dAuthCnonce[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit dAuthQop[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit *pTempArray = Sdf_co_null;
#ifdef ICF_PORT_SYMBIAN
		dEntityBody = (Sdf_ty_s8bit*) sdf_memget(0,Sdf_mc_maxMsgSize , pError );
#endif /*ICF_PORT_SYMBIAN*/

		/*
		 * Allocate memory and initialize the dEntityBody buffer. This will 
		 * be filled with the entire entity body and then passed to
		 * DigestCalcHEntity to calculate the hash of the entity body.
		 */
		Sdf_mc_bzero(dEntityBody,Sdf_mc_maxMsgSize*sizeof(Sdf_ty_s8bit));

		/*
		 * First we do a MD5 of "" whether or not the MsgBody is
		 * present. This is because, if a msgBody is not present
		 * in the SipMessage, we should do a hash of "" and use
		 * it to compute the final Digest response. If a messageBody
		 * is present, the HEntity is anyway overwritten below
		 */ 
		DigestCalcHEntity((Sdf_ty_s8bit *)"",0,HEntity);

		/* 
		 * Calculate H(Entity-Body) required for the response
		 */
		if (sip_getMsgBodyCount(pSipMsg, &dMsgBodyCount, \
			(SipError *)&(pError->stkErrCode)) != SipFail)
		{
			if (dMsgBodyCount > 0)
			{
				SipHeader dHeader;

				if (sip_getHeader(pSipMsg, SipHdrTypeContentType,\
					&dHeader,(SipError *) &(pError->stkErrCode)) == SipFail)
				{
					pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pError->errCode, \
						(Sdf_ty_s8bit *) \
						"sdf_fn_uaFormAuthorizationCredential() :" \
						"Error getting Content Type Header from Sip Message",\
						pError);
#endif
					return Sdf_co_fail;
				}

				/* 
				 * This forms a buffer from the MsgBodies of the SipMessage
				 */
				pTempArray = dEntityBody;
				if (sip_formMimeBody(pTempArray+Sdf_mc_maxMsgSize,\
							pSipMsg->slMessageBody, \
					(SipContentTypeHeader *)dHeader.pHeader,\
					pTempArray,&dLength,\
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{ 
					pError->errCode = Sdf_en_headerManipulationError;
					sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
						(Sdf_ty_s8bit *)\
						"sdf_fn_uaFormAuthorizationCredential():"	\
						"Error forming a buffer from the message bodies in" \
						"Sip Message", pError);
#endif
					return Sdf_co_fail;
				}
				/********Fix for the patch done in stack******/
				sip_freeSipHeader(&dHeader);

				/* 
				 * sip_formMimeBody returns the initial CRLF (\r\n)
				 * in dEntityBody. But this initial CRLF should
				 * not be used in computing the Hash(Entity).
				 */
				DigestCalcHEntity(dEntityBody+2,dLength-2,HEntity);
			}	
		}
		/*
		 * Unquote the realm, nonce, Cnonce and Qop before calculating the
		 * response digest.
		 */
		(void)sdf_fn_uaUnq(pAuthenticationParams->pRealm, dAuthRealm);
		(void)sdf_fn_uaUnq(pAuthenticationParams->pNonce, dAuthNonce);
		(void)sdf_fn_uaUnq(pUacAuthInfo->pCnonce, dAuthCnonce);
		(void)sdf_fn_uaUnq(pUacAuthInfo->pQop, dAuthQop);

		/* 
		 * Calculate the H(A1)
		 */
		DigestCalcHA1(pAuthenticationParams->pAlgorithm, \
			pUacAuthInfo->pUserName, dAuthRealm, pUacAuthInfo->pPassword, \
			dAuthNonce, dAuthCnonce, HA1);


		/* 
		 * Calculate the response digest
		 */
		DigestCalcResponse(HA1, dAuthNonce, pUacAuthInfo->pNonceCount,\
			dAuthCnonce, dAuthQop, pMethod, pUri, HEntity, \
			dResponse);
#ifdef ICF_PORT_SYMBIAN
		sdf_memfree( 0, (Sdf_ty_pvoid*)&dEntityBody, pError);
#endif/*ICF_PORT_SYMBIAN*/
		
	}


	/* 
	 * Create the credential for the header
	 */
	if (Sdf_mc_strcasecmp(pAuthenticationParams->pScheme, "Basic") == 0)
	{
		if (sip_initSipGenericCredential(ppGenericCredential, SipCredBasic, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error initializing Credential structure",pError);
#endif
			return Sdf_co_fail;
		}

		/*
		 * Set the response in the header
		 */
		if (sip_setBasicInCredential(*ppGenericCredential, pResponse, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error setting Basic Scheme in Credential",pError);
#endif
			sip_freeSipGenericCredential(*ppGenericCredential);
			return Sdf_co_fail;
		}
	}
	else if (Sdf_mc_strcasecmp(pAuthenticationParams->pScheme, "Digest") == 0)
	{
		SipGenericChallenge *pGenericChallenge;

		/*
		 * Initialize a SipGenericCredential structure, and insert the
		 * authentication parameters into that structure.
		 */
		if (sip_initSipGenericCredential(ppGenericCredential, \
			SipCredAuth,(SipError *) &(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error initialising Credential structure",pError);
#endif
			return Sdf_co_fail;
		}

		if (sip_initSipGenericChallenge(&pGenericChallenge, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error initialising Challenge structure",pError);
#endif
			sip_freeSipGenericChallenge(pGenericChallenge);
			sip_freeSipGenericCredential(*ppGenericCredential);
			return Sdf_co_fail;
		}

		pTmpVal = Sdf_mc_strdupAuth("Digest");
		
		if(sip_setSchemeInChallenge(pGenericChallenge, pTmpVal, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error setting Digest Scheme in Credential",pError);
#endif
			sip_freeSipGenericChallenge(pGenericChallenge);
			sip_freeSipGenericCredential(*ppGenericCredential);
			return Sdf_co_fail;
		}	
			
		/*
		 * Set the UserName in the generic challenge
		 */
		if (pUacAuthInfo->pUserName != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				,(Sdf_ty_s8bit *)"username", pUacAuthInfo->pUserName, \
				Sdf_co_true, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Username in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Realm in the generic challenge
		 */
		if (pAuthenticationParams->pRealm != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"realm", pAuthenticationParams->pRealm,\
				Sdf_co_false, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Realm in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}	

		/*
		 * Set the Nonce in the generic challenge
		 */
		if (pAuthenticationParams->pNonce != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"nonce", pAuthenticationParams->pNonce,\
				Sdf_co_false, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Nonce in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}	

		/*
		 * Set the Uri in the generic challenge
		 */
		if (pUri != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"uri", pUri, Sdf_co_true, pError)\
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting URI in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Response digest in the generic challenge
		 */
		if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
			, (Sdf_ty_s8bit *)"response", dResponse,\
			Sdf_co_true, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error setting response digest in generic challenge", \
				pError);
#endif
			sip_freeSipGenericChallenge(pGenericChallenge);
			sip_freeSipGenericCredential(*ppGenericCredential);
		}

		/*
		 * Set the Algorithm in the generic challenge
		 */
		if (pAuthenticationParams->pAlgorithm != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"algorithm", pAuthenticationParams->\
				pAlgorithm, Sdf_co_false, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Algorithm in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Cnonce in the generic challenge
		 */
		if (pUacAuthInfo->pCnonce != Sdf_co_null)
		{
            Sdf_ty_bool dFlag = Sdf_co_true;
			
			if( pUacAuthInfo->pCnonce[0] == '"' )
				dFlag = Sdf_co_false;
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"cnonce", pUacAuthInfo->pCnonce,\
				dFlag, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Cnonce in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Opaque value in the generic challenge
		 */
		if (pAuthenticationParams->pOpaque != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"opaque", pAuthenticationParams->pOpaque, \
				Sdf_co_false, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Opaque in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Qop in the generic challenge
		 */
		if (pUacAuthInfo->pQop != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"qop", pUacAuthInfo->pQop, Sdf_co_false,\
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Qop in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		/*
		 * Set the Nonce count in the generic challenge
		 */
		if (pUacAuthInfo->pNonceCount != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"nc", pUacAuthInfo->pNonceCount,\
				Sdf_co_false, pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
					"Error setting Nonce count in generic challenge",pError);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
				sip_freeSipGenericCredential(*ppGenericCredential);
			}
		}

		if (sip_setChallengeInCredential(*ppGenericCredential, \
			pGenericChallenge,(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthorizationCredential( ) : "
				"Error setting Challenge in Credential",pError);
#endif
			sip_freeSipGenericChallenge(pGenericChallenge);
			sip_freeSipGenericCredential(*ppGenericCredential);
			return Sdf_co_fail;
		}
		
		/* Free the local reference */
		sip_freeSipGenericChallenge(pGenericChallenge);
	}

	sdf_fn_debug((Sdf_ty_s8bit*)\
			"SDF_DEBUG--Exiting sdf_fn_uaFormAuthorizationCredential");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaAuthorize 
**
** DESCRIPTION: This function is used to check whether
**				the incoming request has to be authorized. It issues the
**				authentication related callbacks to the application
**
******************************************************************************/
Sdf_ty_authResult sdf_ivk_uaAuthorize
#ifdef ANSI_PROTO
	(Sdf_st_callObject **ppCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_ty_pvoid *ppContext,
	Sdf_st_error *pErr)
#else
	(ppCallObj, pOverlapTransInfo, ppContext, pErr)
	Sdf_st_callObject **ppCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_pvoid *ppContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit size = 0;
	Sdf_st_headerList *pHeaderList = Sdf_co_null;
	Sdf_st_authenticationParams *pAuthParams = Sdf_co_null;
	SipHeader *pAuthenticateHeader = Sdf_co_null;
	Sdf_ty_slist slMatchedHeaderList;
	Sdf_ty_authResult dAuthResult;
	Sdf_ty_s8bit *pMethod = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;	
	SipMessage *pSipMsg=Sdf_co_null;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_smallBufferSize];
	Sdf_ty_s8bit *pMethodTrace = Sdf_co_null;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*) \
			"SDF_DEBUG--Entering sdf_ivk_uaAuthorize");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{	
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAuthorize():  Invalid error parameter");
#endif		
		return Sdf_en_error;
	}
	if((*ppCallObj) == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
			"Invalid callobject parameter",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_error;
	}
#endif	/* End of param validation ifdef */

	/*
	 * Set pProcessedHeaderList to the correct list of processed headers
	 * depending on whether the pOverlapTransInfo is NULL or not.
	 */
	if(pOverlapTransInfo == Sdf_co_null)
	{
		Sdf_st_transaction *pTransaction;

		if (sdf_ivk_uaGetTransactionFromCallObject((*ppCallObj), \
			Sdf_en_uasTransaction, &pTransaction, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize(): "
				"Failed to get UAS transaction from call object",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
		}
		pProcessedHeaderList = &(pTransaction->slProcessedHeaders);
		pSipMsg = pTransaction->pSipMsg;

		(void)sdf_ivk_uaFreeTransaction(pTransaction);
	}
	else
	{
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);
		pSipMsg = pOverlapTransInfo->pSipMsg;
	}

	/*
	 * Retrieve the Authorization header from the list of processed headers
	 */
	if (sdf_ivk_uaGetHeaderListByType(*pProcessedHeaderList, \
		SipHdrTypeAuthorization, &slMatchedHeaderList, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
			"Failed to retrieve Authorization HeaderList from list of "
			"processed headers",pErr);
#endif
		return Sdf_en_error;
	}

	(void)sdf_listSizeOf(&slMatchedHeaderList, &size, pErr);

	if (size != 0)
	{
		/* 
		 * Authorization header is present in the request
		 */
		SipGenericCredential *pCredential = Sdf_co_null;
		SipHeader *pAuthHeader;
		en_CredentialType credType;
		Sdf_st_uasAuthInfo *pUasAuthInfo;
		Sdf_ty_s8bit *pBasic, *pTemp;
		Sdf_ty_s8bit dTempPassword[Sdf_co_characterBufferSize]="",
		dResponse[Sdf_co_smallBufferSize]="";
		
		/*
		 * Retrieve the HeaderList at index 0 from slMatchedHeaderList
		 */
		if (sdf_listGetAt(&slMatchedHeaderList, 0, \
				(Sdf_ty_pvoid *)&pHeaderList,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting Header at index 0 from retrieved list of "
				"Authorization headers.",pErr);
#endif
			(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
			return Sdf_en_error;
		}

		/*
		 * Retrieve the header at index 0 from the Authorization HeaderList
		 */
		if (sdf_listGetAt(&(pHeaderList->slHeaders), 0, \
			(Sdf_ty_pvoid *)&pAuthHeader,pErr)== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting Authorization from AdditionalHdr list",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
			return Sdf_en_error;
		}

		if(pAuthHeader == Sdf_co_null)
		{
			(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
			return Sdf_en_error;
		}

		/* 
		 * Check for scheme and proceed accordingly
		 */
  		if (SipFail == sip_getCredentialsFromAuthorizationHdr(pAuthHeader, \
			&pCredential,(SipError*) &(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting Credential from Authorization Header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
			return Sdf_en_error;
		}
		

		if (SipFail == sip_getCredentialTypeFromCredential( \
			pCredential, &credType,(SipError*)&(pErr->stkErrCode)))
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting Credentialtype from Authorization Header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipGenericCredential(pCredential);
			(void)sdf_listDeleteAll(&slMatchedHeaderList, \
				pErr);
			return Sdf_en_error;
		}

		if(SipCredBasic == credType)
		{
			/* 
			 * Basic Authentication scheme 
			 */
			Sdf_ty_s8bit dTempBasic[Sdf_co_smallBufferSize]="";
			SipHeader fromHeader;
			SipAddrSpec *pAddrSpec;
			Sdf_ty_s8bit *pTrack = ICF_NULL;

#ifdef SDF_TRACE	
			if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
				&pMethodTrace, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Method from SIP Message",pErr);
#endif
				sip_freeSipGenericCredential(pCredential);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call: %s - Received with Method as "
				"%s: Authorization Header with Scheme as Basic \n", \
				(*ppCallObj)->pCommonInfo->pKey->pCallId ,pMethodTrace);

			sdf_fn_trace(Sdf_en_detailedTraces,Sdf_en_authTraces,trace,0,pErr);
#endif	

			if (SipFail == sip_getBasicFromCredential(pCredential, \
				&pBasic,(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Basic Credential from Authorization Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipGenericCredential(pCredential);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			/* Free the local reference */
			sip_freeSipGenericCredential(pCredential);

			if(pBasic!=Sdf_co_null)
				(void)sdf_fn_uaSprintf(dTempBasic,"%s",pBasic);

			/*
			 * Decode the response received into the cleartext
			 * "username:password" format.
			 */
			decodeBase64(dTempBasic, dResponse);	

			/* 
			 * Fill up the Sdf_st_uasAuthInfo structure and issue the
			 * sdf_cbk_uaAuthenticateUser callback
			 */
			if (sdf_ivk_uaInitUasAuthInfo(&pUasAuthInfo, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error initialising Sdf_st_uasAuthInfo structure",pErr);
#endif
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			/*
			 * Set the scheme to "Basic"
			 */
			pUasAuthInfo->pScheme = Sdf_mc_strdupAuth("Basic"); 

			if (Sdf_co_null == (pTemp = Sdf_mc_strtokr(dResponse,":", &pTrack)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_authError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting username from Basic Authorization Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_authError;
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			/*
			 * Set the username as returned by the decodeBase64 algorithm
			 */
			pUasAuthInfo->pUserName = Sdf_mc_strdupAuth(pTemp);

			if (Sdf_co_null == (pTemp = Sdf_mc_strtokr(pTrack,":",&pTrack)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_authError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize() : "
					"Error getting passwd from Basic Authorization Header",\
					pErr);
#endif
				pErr->errCode=Sdf_en_authError;
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}
			/*
			 * Store the password as returned by decodeBase64 into 
			 * dTempPassword
			 */
			(void)sdf_fn_uaSprintf(dTempPassword,"%s",pTemp);

			/*
			 * Extract the "From" addrspec to provide a reference to the
			 * application as to the user that is being authorized.
			 */
			if (SipFail == sip_getHeader(pSipMsg, \
				SipHdrTypeFrom,&fromHeader,(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidHeaderError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting From Header from SipMessage",pErr);
#endif
				pErr->errCode=Sdf_en_invalidHeaderError;
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			if(SipFail == sip_getAddrSpecFromFromHdr(&fromHeader,\
			 	&pAddrSpec,(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting From Addr Spec from From header",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				sip_freeSipHeader(&fromHeader);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}
				
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s -  Request received; "
				"Issuing sdf_cbk_uaAuthenticateUser to application", \
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,trace,0,pErr);
#endif
			/*
			 * Provide the sdf_cbk_uaAuthenticateUser callback to the user 
			 * to check the username provided in the authorization header
			 * against its database and return the password
			 */
			dAuthResult = sdf_cbk_uaAuthenticateUser(ppCallObj, pAddrSpec, \
					pUasAuthInfo, ppContext, pErr);

			if(pErr->errCode != Sdf_en_noUaError)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error returned from sdf_cbk_uaAuthenticateUser() callback",pErr);
#endif
				pErr->errCode=Sdf_en_callBackError;
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				sip_freeSipHeader(&fromHeader);
				sip_freeSipAddrSpec(pAddrSpec);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}

			if (dAuthResult == Sdf_en_authReqd)
			{
				/*
				 * The application has indicated that the credentials are to be
				 * verified. The pSecret member of the pUasAuthInfo structure
				 * should be set by the application to the password for this
				 * user (as present in it's database). Compare the password 
				 * returned against the password computed by decodeBase64
				 */
				if ( (pUasAuthInfo->pSecret != Sdf_co_null) && \
					(Sdf_mc_strcmp(pUasAuthInfo->pSecret, dTempPassword) == 0) )
				{
					/*
					 * Passwords match. Return Sdf_en_authNotReqd
					 */
					sdf_fn_debug((Sdf_ty_s8bit*)\
						"SDF_DEBUG--Exiting sdf_ivk_uaAuthorize");
					pErr->errCode = Sdf_en_noUaError;
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
						"Call : %s -  The Request "
						"received has matched the authorization credentials "
						"for Basic Scheme", \
						(*ppCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_briefTraces, Sdf_en_authTraces, trace,\
						0, pErr);
#endif
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipHeader(&fromHeader);
					sip_freeSipAddrSpec(pAddrSpec);
					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_authNotReqd;
				}
				else
				{
					/*
					 * Credentials returned by application do not match with
					 * that present in the SIP Message. Issue the
					 * sdf_cbk_uaChallengeUser so that the application can
					 * return the parameters for challenging the user
					 */
					if (Sdf_co_fail == sdf_ivk_uaInitAuthenticationParams(\
						&pAuthParams, pErr))
					{	
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
							"Error initialising Authenticate Params",pErr);
#endif
						(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
						return Sdf_en_error;
					}
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
						"Call : %s -  Authorization "
						"required; Issuing sdf_cbk_uaChallengeUser to user "
						"to authorize call", \
						(*ppCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_briefTraces, Sdf_en_cbksTraces, trace,\
						0, pErr);
#endif
					/*
					 * Provide the sdf_cbk_uaChallengeUser callback to the user 
					 * where the application returns the parameters for issuing 
					 * the challenge to the remote user
					 */
					dAuthResult = sdf_cbk_uaChallengeUser(ppCallObj,pAddrSpec, \
						pAuthParams, ppContext, pErr);

					if((pErr->errCode != Sdf_en_noUaError))
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_callBackError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : Error "
							"returned from sdf_cbk_uaChallengeUser() callback",\
							pErr);
#endif
						pErr->errCode=Sdf_en_callBackError;
						(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
						(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
						sip_freeSipHeader(&fromHeader);
						sip_freeSipAddrSpec(pAddrSpec);
						(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
						return Sdf_en_error;
					}
				}
			}
			/* Free the local reference */
			sip_freeSipHeader(&fromHeader);
			sip_freeSipAddrSpec(pAddrSpec);
			(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
		}
		else
		{
			/*
			 * Digest Authentication scheme
			 */
			Sdf_ty_s8bit *pScheme = Sdf_co_null, *pUri = Sdf_co_null;
			Sdf_ty_s8bit *pNonceCount = Sdf_co_null, *pResponse = Sdf_co_null;
			SipGenericChallenge *pChallenge;
			Sdf_ty_u32bit dCount,dIndex;
			SipHeader fromHeader;
			SipAddrSpec *pAddrSpec;

			if (sip_getChallengeFromCredential(pCredential, \
				&pChallenge,(SipError*)&(pErr->stkErrCode)) == SipFail)
			{	
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Challenge from Credential",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipGenericCredential(pCredential);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}
			/* Free the local reference */
			sip_freeSipGenericCredential(pCredential);

			if (sip_getSchemeFromChallenge(pChallenge,&pScheme, \
				(SipError*)&(pErr->stkErrCode))	== SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Scheme from Authorization Header",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

#ifdef SDF_TRACE			
			if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, &pMethodTrace, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Method from Sip Message",pErr);
#endif
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}
#endif
						
			if(Sdf_mc_strcasecmp(pScheme,"Digest") != 0)
			{
				/*
				 * Scheme other than Digest. Not supported by the UA
				 * Toolkit.
				 */
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_notImplemented,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Authorization header with Scheme other than Basic/Digest."
					"Not implemented in the UA toolkit.",pErr);
#endif
				pErr->errCode=Sdf_en_notImplemented;
#ifdef SDF_TRACE
				(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
					"Call: %s - Recieved with Method "
					"as %s: Authorization Header with Scheme as %s which is "
					"not supported\n", \
					(*ppCallObj)->pCommonInfo->pKey->pCallId,\
				pMethodTrace,pScheme);
				sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_authTraces, trace, \
					0, pErr);
#endif	
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call: %s - Recieved with Method as %s: "
				"Authorization Header with Scheme as Digest \n", \
				(*ppCallObj)->pCommonInfo->pKey->pCallId, pMethodTrace);
			sdf_fn_trace(Sdf_en_detailedTraces,Sdf_en_authTraces,trace,0,pErr);
#endif

			if (SipFail == sip_getAuthorizationParamCountFromChallenge \
				(pChallenge, &dCount,(SipError*)&(pErr->stkErrCode)))
			{
				pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting Authorization ParamCount",pErr);
#endif
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;		
			}

			/* 
			 * Fill up the Sdf_st_uasAuthInfo and Sdf_st_authenticationParams 
			 * structures and issue the sdf_cbk_uaAuthenticateUser callback
			 */
			if (sdf_ivk_uaInitUasAuthInfo(&pUasAuthInfo, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error initialising Sdf_st_uasAuthInfo structure",pErr);
#endif
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			if (Sdf_co_fail == sdf_ivk_uaInitAuthenticationParams(&pAuthParams,\
				pErr))
			{	
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error initialising Authenticate Params",pErr);
#endif	
				sip_freeSipGenericChallenge(pChallenge);
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}

			/*
			 * Set the Scheme to Digest
			 */
			pUasAuthInfo->pScheme = Sdf_mc_strdupAuth("Digest");
		
			/*
			 * Iterate through the parameters in the Authorization header
			 * and extract the relevant parameters into pUasAuthInfo and
			 * pAuthParams
			 */
	  		for (dIndex = 0; dIndex < dCount; dIndex++)
			{
				SipParam *pParam;
				Sdf_ty_s8bit *pName, *pValue;

				if (sip_getAuthorizationParamAtIndexFromChallenge(pChallenge, \
					&pParam, dIndex, (SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,\
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
						"Error getting Param from Authorization Header",pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					sip_freeSipGenericChallenge(pChallenge);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_error;
				}
				if (sip_getNameFromSipParam(pParam, &pName, \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
						"Error getting Param  name from Authorization Header",\
						pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					sip_freeSipGenericChallenge(pChallenge);
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipParam(pParam);
					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_error;
				}
				if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0, \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
						"Error getting Param value from Authorization Header",\
						pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					sip_freeSipGenericChallenge(pChallenge);
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipParam(pParam);
					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_error;
				}
				if(pValue == Sdf_co_null)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
						"No value for SipParam in Authorization Header",pErr);
#endif
					pErr->errCode=Sdf_en_headerManipulationError;
					sip_freeSipGenericChallenge(pChallenge);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipParam(pParam);
					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_error;
				}

				if (Sdf_mc_strcasecmp(pName, "realm") == 0)
				{
					pUasAuthInfo->pRealm = (Sdf_ty_s8bit *)sdf_memget(\
						Sdf_mc_authMemId, Sdf_mc_strlen(pValue)+1, pErr);
					(void)sdf_fn_uaUnq(pValue, pUasAuthInfo->pRealm);
				}
				else if (Sdf_mc_strcasecmp(pName, "username") == 0)
				{
					pUasAuthInfo->pUserName = (Sdf_ty_s8bit *)sdf_memget(\
						Sdf_mc_authMemId, Sdf_mc_strlen(pValue)+1, pErr);
					(void)sdf_fn_uaUnq(pValue, pUasAuthInfo->pUserName);
				}
				else if (Sdf_mc_strcasecmp(pName, "algorithm") == 0)
				{
					pUasAuthInfo->pAlgorithm = (Sdf_ty_s8bit *)sdf_memget(\
						Sdf_mc_authMemId, Sdf_mc_strlen(pValue)+1, pErr);
					(void)sdf_fn_uaUnq(pValue, pUasAuthInfo->pAlgorithm);
				}
				else if (Sdf_mc_strcasecmp(pName, "nonce") == 0)
				{
					pAuthParams->pNonce = Sdf_mc_strdupAuth(pValue);
					pUasAuthInfo->pNonce = (Sdf_ty_s8bit *)sdf_memget(\
						Sdf_mc_authMemId, Sdf_mc_strlen(pValue)+1, pErr);
					(void)sdf_fn_uaUnq(pValue, pUasAuthInfo->pNonce);
				}
				else if (Sdf_mc_strcasecmp(pName, "qop") == 0)
					pAuthParams->pQop = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "uri") == 0)
					pUri = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "cnonce") == 0)
				{
					pUasAuthInfo->pCnonce = (Sdf_ty_s8bit *)sdf_memget(\
						Sdf_mc_authMemId, Sdf_mc_strlen(pValue)+1, pErr);
					(void)sdf_fn_uaUnq(pValue, pUasAuthInfo->pCnonce);
				}
				else if (Sdf_mc_strcasecmp(pName, "nc") == 0)
					pNonceCount = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "response") == 0)
					pResponse = Sdf_mc_strdupAuth(pValue);

				/* Free the local reference */
				sip_freeSipParam(pParam);
			}
			/* Free the local reference */
			sip_freeSipGenericChallenge(pChallenge);

			/*
			 * Extract the "From" addrspec to provide a reference to the
			 * application as to the user that is being authorized.
			 */
			if(SipFail == sip_getHeader(pSipMsg, \
				SipHdrTypeFrom,&fromHeader,(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error getting From Header from SipMessage",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}

			if(SipFail == sip_getAddrSpecFromFromHdr(&fromHeader,\
				&pAddrSpec,(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : \
					Error getting Addr Spec from SipMessage",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
				return Sdf_en_error;
			}

#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s -  Request received; "
				"Issuing sdf_cbk_uaAuthenticateUser to user",\
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,trace,0,pErr);
#endif
			/*
			 * Provide the sdf_cbk_uaAuthenticateUser callback to the user 
			 * to check the username against its database and return the 
			 * H(A1).
			 */
			dAuthResult = sdf_cbk_uaAuthenticateUser(ppCallObj, pAddrSpec, \
					pUasAuthInfo, ppContext, pErr);

			if((pErr->errCode != Sdf_en_noUaError))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : Error returned "
					"from sdf_cbk_uaAuthenticateUser() callback",pErr);
#endif
				pErr->errCode=Sdf_en_callBackError;
				sip_freeSipHeader(&fromHeader);
				sip_freeSipAddrSpec(pAddrSpec);
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
				(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
				(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
				return Sdf_en_error;
			}

			if(dAuthResult == Sdf_en_authReqd)
			{
				/*
				 * The application has indicated that the credentials are to be
				 * verified. The pSecret member of the pUasAuthInfo structure
				 * should be set by the application to the H(A1) for this
				 * user (as present in it's database). Compute the response
				 * digest using this H(A1) and compare it against that present
				 * in the Authorization header
				 */

				/*
				 * The authorization may have failed because either the 
				 * username or realm did not match the valid credentials 
				 * for the user. In such a case, the pUasAuthInfo->pSecret 
				 * member will not be set. In such a case, we can issue
				 * the sdf_cbk_uaChallengeUser callback directly without
				 * going for the sdf_fn_uaMatchEncode check.
				 */
				if (sdf_fn_uaMatchEncode(pSipMsg, pAuthParams, \
					pUasAuthInfo, pUri, pNonceCount, pResponse, pErr) \
					== Sdf_co_success)
				{	
					/*
					 * Response digest matches perfectly. Return
					 * Sdf_en_authNotReqd.
					 */
					sdf_fn_debug((Sdf_ty_s8bit*)\
						"SDF_DEBUG--Exiting sdf_ivk_uaAuthorize");
					pErr->errCode = Sdf_en_noUaError;
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
						"Call : %s - The Request received "
						"has matched the authorization credentials for "
						"Digest Scheme", \
						(*ppCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_briefTraces, Sdf_en_authTraces, trace,\
						0, pErr);
#endif
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipHeader(&fromHeader);
					sip_freeSipAddrSpec(pAddrSpec);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pUri, pErr);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pNonceCount, pErr);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pResponse, pErr);

					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_authNotReqd;
			   }
			   else if(pErr->errCode != Sdf_en_noUaError)
			   {
					/* 
					 * Error occured
					 */
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
						"Error while matching application's database "
						"credentials against the response digest",pErr);
#endif
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
					(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
					sip_freeSipHeader(&fromHeader);
					sip_freeSipAddrSpec(pAddrSpec);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pUri, pErr);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pNonceCount, pErr);
					(void)sdf_memfree(Sdf_mc_authMemId, \
						(Sdf_ty_pvoid*)&pResponse, pErr);

					(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
					return Sdf_en_error;
				}
				else
				{
					/*
					 * Credentials returned by application do not match with
					 * that present in the SIP Message. Issue the
					 * sdf_cbk_uaChallengeUser so that the application can
					 * return the parameters for challenging the user
					 */
#ifdef SDF_TRACE
					(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
						"Call : %s - Authorization "
						"required; Issuing sdf_cbk_uaChallengeUser to user "
						"to authorize call", \
						(*ppCallObj)->pCommonInfo->pKey->pCallId);
					sdf_fn_trace(Sdf_en_briefTraces, Sdf_en_cbksTraces, trace,\
						0, pErr);
#endif
					/*
					 * Provide the sdf_cbk_uaChallengeUser callback to the user 
					 * where the application returns the parameters for issuing 
					 * the challenge to the remote user
					 */
					dAuthResult = sdf_cbk_uaChallengeUser(ppCallObj,pAddrSpec, \
						pAuthParams, ppContext, pErr);

					if((pErr->errCode != Sdf_en_noUaError))
					{
#ifdef SDF_ERROR
						sdf_fn_setError(Sdf_en_majorErrors, \
							Sdf_en_callBackError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : Error "
							"returned from sdf_cbk_uaChallengeUser() callback",\
							pErr);
#endif
						pErr->errCode=Sdf_en_callBackError;
						(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
						(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
						sip_freeSipHeader(&fromHeader);
						sip_freeSipAddrSpec(pAddrSpec);
						(void)sdf_memfree(Sdf_mc_authMemId, \
							(Sdf_ty_pvoid*)&pUri, pErr);
						(void)sdf_memfree(Sdf_mc_authMemId, \
							(Sdf_ty_pvoid*)&pNonceCount, pErr);
						(void)sdf_memfree(Sdf_mc_authMemId, \
							(Sdf_ty_pvoid*)&pResponse, pErr);

						(void)sdf_listDeleteAll(&slMatchedHeaderList,pErr);
						return Sdf_en_error;
					}
				}
			}
			/* Free the local reference */
			sip_freeSipHeader(&fromHeader);
			sip_freeSipAddrSpec(pAddrSpec);
			(void)sdf_memfree(Sdf_mc_authMemId, (Sdf_ty_pvoid*)&pUri, pErr);
			(void)sdf_memfree(Sdf_mc_authMemId, (Sdf_ty_pvoid*)&pNonceCount, pErr);
			(void)sdf_memfree(Sdf_mc_authMemId, (Sdf_ty_pvoid*)&pResponse, pErr);
			(void)sdf_ivk_uaFreeUasAuthInfo(pUasAuthInfo);
		}
		/* Free the elements of the list now */
		(void)sdf_listDeleteAll(&slMatchedHeaderList, pErr);
	}
	else
	{
		/* 
		 * There is no Authorization header in the request. Issue a 
		 * callback to the application to fill up the authparam structure 
		 */
		SipHeader fromHeader;
		SipAddrSpec *pAddrSpec;

		if (Sdf_co_fail == sdf_ivk_uaInitAuthenticationParams(&pAuthParams, \
			pErr))
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error initialising Authenticate Params",pErr);
#endif
			return Sdf_en_error;
		}

		/*
		 * Extract the "From" addrspec to provide a reference to the
		 * application as to the user that is being authorized.
		 */
		if(SipFail == sip_getHeader(pSipMsg, \
			SipHdrTypeFrom,&fromHeader,(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting From Header from Sip message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
			return Sdf_en_error;
		}

		if(SipFail == sip_getAddrSpecFromFromHdr(&fromHeader,\
		 	&pAddrSpec,(SipError*)&(pErr->stkErrCode)))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error getting From Addr Spec from Sip message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
			return Sdf_en_error;
		}
	
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
			"Call : %s -  Request received; "
			"Issuing sdf_cbk_uaChallengeUser to user to authorize call",\
			(*ppCallObj)->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,trace,0,pErr);
#endif
		/*
		 * Provide the sdf_cbk_uaChallengeUser callback to the user where
		 * the application returns the parameters for issuing the challenge
		 * to the remote user
		 */
		dAuthResult = sdf_cbk_uaChallengeUser(ppCallObj,pAddrSpec, \
			pAuthParams, ppContext, pErr);

		/* Free the local reference */
		sip_freeSipHeader(&fromHeader);
		sip_freeSipAddrSpec(pAddrSpec);

		if((pErr->errCode != Sdf_en_noUaError))
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error returned from sdf_cbk_uaChallengeUser() callback",pErr);
#endif
			pErr->errCode=Sdf_en_callBackError;
			return Sdf_en_error;
		}
	} /* End of else block handling requests without Authorization header */

	if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
		&pMethod, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
			"Error getting Method from SipMessage",pErr);
#endif
		return Sdf_en_error;
	}

	switch(dAuthResult)
	{
		case Sdf_en_authReqd:
		/*
		 * Authorization required. Form a 401 response
		 */
		{
			Sdf_st_headerList *pWwwAuthenticateHeaderList;

			/* 
			 * Form the authentication challenge using the authparams structure
			 */
			if (Sdf_co_fail == sdf_fn_uaFormAuthenticateChallenge((*ppCallObj),\
				pAuthParams,&pAuthenticateHeader,pErr))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_authError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error forming challenge for www-Authenticate Header",pErr);
#endif
				pErr->errCode=Sdf_en_authError;
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
				return Sdf_en_error;
			}

			/* 
			 * Insert the formed challenge into a WWW-Authenticate header and
			 * insert the same into the ProcessedHeaders list of the call 
			 * object
			 */
			if (sdf_ivk_uaInitHeaderList(&pWwwAuthenticateHeaderList, \
				SipHdrTypeWwwAuthenticate, Sdf_co_null, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize(): "
					"Failed to initialize the HeaderList structure for "
					"WWW-Authenticate header",pErr);
#endif
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);		
				return Sdf_en_error;
			}

			if (sdf_listAppend(&(pWwwAuthenticateHeaderList->slHeaders), \
				pAuthenticateHeader,pErr) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pWwwAuthenticateHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize(): "
					"Failed to insert WWW-Authenticate header into the header "
					"list structure", pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);		
				return Sdf_en_error;
			}

			if (sdf_listAppend(pProcessedHeaderList, \
				pWwwAuthenticateHeaderList, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize(): "
					"Failed to add the header list of Www-Authenticate into "
					"the list of Processedheaders in the call object", pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeHeaderList(pWwwAuthenticateHeaderList);
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);		
				return Sdf_en_error;
			}	

			/*
			 * Now form a 401 response using the WWW-Authenticate header created
			 * above.
			 */
			if(sdf_ivk_uaFormResponse(401, pMethod, *ppCallObj, \
				pOverlapTransInfo, Sdf_co_false, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error forming 401 response",pErr);
#endif
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);		
				return Sdf_en_error;
			}

#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s -  401 Unauthorized Response "
				"formed for Authentication", \
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pErr);
#endif
			/* Free the local reference */
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);

			pErr->errCode = Sdf_en_noUaError;
			sdf_fn_debug((Sdf_ty_s8bit*)\
				"SDF_DEBUG-- Exiting sdf_ivk_uaAuthorize");
			return Sdf_en_authReqd;
		}
		case Sdf_en_deny:
		/*
		 * The remote user is to be denied
		 */
		{
			/* 
			 * Form a 403 Forbidden response 
			 */
			if(sdf_ivk_uaFormResponse(403, pMethod, (*ppCallObj),\
				pOverlapTransInfo, Sdf_co_false, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
					"Error forming 403 Response",pErr);
#endif
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
				return Sdf_en_error;
			}
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s -  Request Failed in "
				"Authorization: 403 Forbidden Response formed", \
				(*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pErr);
#endif
			sdf_fn_debug((Sdf_ty_s8bit*)\
				"SDF_DEBUG--Exiting sdf_ivk_uaAuthorize");

			/* Free the local reference */
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);

			pErr->errCode = Sdf_en_noUaError;

			return Sdf_en_deny;
		}
		case Sdf_en_authNotReqd:
		/*
		 * Either this user does not need to authenticate himself OR
		 * the authentication was a success.
		 */
		{
			sdf_fn_debug((Sdf_ty_s8bit*)\
				"SDF_DEBUG--Exiting sdf_ivk_uaAuthorize");

			/* Free the local reference */
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);

			pErr->errCode = Sdf_en_noUaError;

#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s -  The Request need not be "
				"Authorized", (*ppCallObj)->pCommonInfo->pKey->pCallId);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pErr);
#endif
			return Sdf_en_authNotReqd;
		}
		case Sdf_en_appInfo:
		/*
		 * Application specific handling
		 */
		{	
			sdf_fn_debug((Sdf_ty_s8bit*)\
				"SDF_DEBUG--Exiting sdf_ivk_uaAuthorize");
			pErr->errCode = Sdf_en_noUaError;

			/* Free the local reference */
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
			return Sdf_en_appInfo;
		}
		case Sdf_en_error:
		/*
		 * Error in callback
		 */
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callBackError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaAuthorize( ) : "
				"Error returned from sdf_cbk_uaChallengeUser() callback", pErr);
#endif
			pErr->errCode=Sdf_en_callBackError;
			/* Free the local reference */
			(void)sdf_ivk_uaFreeAuthenticationParams(pAuthParams);
			return Sdf_en_error;
		}
	}
	return Sdf_en_error;
}


/*****************************************************************************
** FUNCTION: sdf_fn_uaMatchEncode
**
** DESCRIPTION: This function runs the Digest algorithm to match the 
**				H(A1) value	returned by the application against the
**				credentials provided by the peer entity.
**
******************************************************************************/

Sdf_ty_retVal sdf_fn_uaMatchEncode
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg,
	Sdf_st_authenticationParams *pAuthParams,
	Sdf_st_uasAuthInfo *pUasAuthInfo,
	Sdf_ty_s8bit *pUri,
	Sdf_ty_s8bit *pNonceCount,
	Sdf_ty_s8bit *pResponse,
	Sdf_st_error *pErr)
#else
	(pSipMsg,pAuthParams,pUasAuthInfo,pUri,pNonceCount,pResponse,pErr)
	SipMessage *pSipMsg;
	Sdf_st_authenticationParams *pAuthParams;
	Sdf_st_uasAuthInfo *pUasAuthInfo;
	Sdf_ty_s8bit *pUri;
	Sdf_ty_s8bit *pNonceCount;
	Sdf_ty_s8bit *pResponse;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s8bit *pMethod = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG--Entering sdf_fn_uaMatchEncode");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaMatchEncode():  Invalid error parameter");
#endif		
		return Sdf_co_fail;
	}	
	if(pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode(): "
			"SipMessage param passed is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pAuthParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode(): "
			"AuthParams param passed is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pUasAuthInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode(): "
			"UasAuthInfo param passed is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(Sdf_mc_strcasecmp(pUasAuthInfo->pScheme,"Digest") == 0)
	{
		Sdf_ty_s8bit dResponse[HASHHEXLEN+1];
#ifdef ICF_PORT_SYMBIAN
		Sdf_ty_s8bit *dEntityBody=Sdf_co_null;
#else
		Sdf_ty_s8bit dEntityBody[Sdf_mc_maxMsgSize];
#endif /*ICF_PORT_SYMBIAN*/
		Sdf_ty_s8bit dCalculatedResponse[HASHHEXLEN+1];
		Sdf_ty_s8bit dReceivedResponse[HASHHEXLEN+1];
		Sdf_ty_s8bit dAuthNonce[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit dAuthQop[Sdf_co_smallBufferSize];
		Sdf_ty_s8bit dAuthUri[Sdf_co_smallBufferSize];
		Sdf_ty_u32bit dLength=0;
		Sdf_ty_u32bit dMsgBodyCount=0;
		HASHHEX HEntity="";
		Sdf_ty_s8bit *pTempArray = Sdf_co_null;
#ifdef ICF_PORT_SYMBIAN
		dEntityBody = (Sdf_ty_s8bit*) sdf_memget(0,Sdf_mc_maxMsgSize , pErr );
#endif /*ICF_PORT_SYMBIAN*/

		/*
		 * If the pSecret member has not been filled, return
		 * Sdf_co_fail from here without setting an error code.
		 */
		if (pUasAuthInfo->pSecret == Sdf_co_null)
			return Sdf_co_fail;

		Sdf_mc_bzero(dEntityBody,Sdf_mc_maxMsgSize*sizeof(Sdf_ty_s8bit));

		/*
		 * First we do a MD5 of "" whether or not the MsgBody is
		 * present. This is because, if a msgBody is not present
		 * in the SipMessage, we should do a hash of "" and use
		 * it to compute the final Digest response. If a messageBody
		 * is present, the HEntity is anyway overwritten below
		 */ 
		DigestCalcHEntity((Sdf_ty_s8bit *)"",0,HEntity);

		/*	
		 * (Method + SIP + Version) + SdpBody
		 */
		if (sdf_ivk_uaGetMethodFromSipMessage(pSipMsg, \
			&pMethod, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode( ) : "
				"Error getting Method from SIP Message",pErr);
#endif
			return Sdf_co_fail;
		}
			
		/* 
		 * Calculate H(Entity-Body) required for the response
		 */
		if (sip_getMsgBodyCount(pSipMsg, &dMsgBodyCount, \
			(SipError*)&(pErr->stkErrCode)) != SipFail)
		{
			if (dMsgBodyCount > 0)
			{
				SipHeader dHeader;

				if (sip_getHeader(pSipMsg, SipHdrTypeContentType, &dHeader, \
					(SipError*)&(pErr->stkErrCode)) == SipFail)
				{
					pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode(): "
						"Error getting Content Type Header",pErr);
#endif
					return Sdf_co_fail;
				}

				/* 
				 * This forms a buffer from the MsgBodies of the SipMessage
				 */
				
				pTempArray = dEntityBody;
				if (SipFail == sip_formMimeBody(pTempArray+Sdf_mc_maxMsgSize,\
					pSipMsg->slMessageBody, \
					(SipContentTypeHeader *)dHeader.pHeader,\
					pTempArray,&dLength, (SipError*)&(pErr->stkErrCode)))
				{
					pErr->errCode = Sdf_en_headerManipulationError;
					sip_freeSipHeader(&dHeader);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaMatchEncode(): "
						"Error forming a buffer of Mime Body from Sip "
						"Message",pErr);
#endif
					return Sdf_co_fail;
				}
				/********Fix for the patch done in stack******/

				sip_freeSipHeader(&dHeader);

				/* 
				 * Calculate H(entity-body) which is required for calculating 
				 * the final response
				 */
				/* 
				 * sip_formMimeBody returns the initial CRLF (\r\n)
				 * in dEntityBody. But this initial CRLF should
				 * not be used in computing the Hash(Entity).
				 */
				DigestCalcHEntity(dEntityBody+2,dLength-2,HEntity);

			}	
		}

		(void)sdf_fn_uaUnq(pAuthParams->pNonce, dAuthNonce);
		(void)sdf_fn_uaUnq(pAuthParams->pQop, dAuthQop);
		(void)sdf_fn_uaUnq(pUri, dAuthUri);

		/* 
		 * Calculate the response digest got using the secret that is
		 * returned by the application. Note that the secret is the 
		 * H(A1) for the particular user.
		 */
		DigestCalcResponse(pUasAuthInfo->pSecret, dAuthNonce, pNonceCount, \
			pUasAuthInfo->pCnonce, dAuthQop, pMethod, dAuthUri,HEntity, dResponse);

		/* 
		 * Compare the response digests
		 */
		(void)sdf_fn_uaUnq(dResponse, dCalculatedResponse);
		(void)sdf_fn_uaUnq(pResponse, dReceivedResponse);

		if(Sdf_mc_strcmp(dCalculatedResponse, dReceivedResponse) == 0)
		{
#ifdef ICF_PORT_SYMBIAN
			sdf_memfree( 0, (Sdf_ty_pvoid*)&dEntityBody, pErr);
#endif /*ICF_PORT_SYMBIAN*/			
			sdf_fn_debug((Sdf_ty_s8bit*)\
				"SDF_DEBUG--Exiting sdf_fn_uaMatchEncode");

			pErr->errCode = Sdf_en_noUaError;

			return Sdf_co_success;
		}
#ifdef ICF_PORT_SYMBIAN
		sdf_memfree( 0, (Sdf_ty_pvoid*)&dEntityBody, pErr);
#endif /*ICF_PORT_SYMBIAN*/		
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG--Exiting sdf_fn_uaMatchEncode");
	return Sdf_co_fail;
}

/*********************************************************
** FUNCTION: sdf_fn_uaFormAuthenticateChallenge
**
** DESCRIPTION: This function is used to  form a 
** WWW-Authenticate header to challenge the peer entity
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaFormAuthenticateChallenge
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_authenticationParams *pAuthParams,
	SipHeader **ppAuthenticateHeader,
	Sdf_st_error *pErr)
#else
	(pCallObj,pAuthParams,ppAuthenticateHeader,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_authenticationParams *pAuthParams;
	SipHeader **ppAuthenticateHeader;
	Sdf_st_error *pErr;
#endif
{
	SipGenericChallenge *pGenericChallenge = Sdf_co_null;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_smallBufferSize];
#endif
#ifndef SDF_LINT    
   	Sdf_st_callObject *pDummyCallObj = Sdf_co_null;
	pDummyCallObj = pCallObj;
#endif
	sdf_fn_debug((Sdf_ty_s8bit*)\
		"SDF_DEBUG--Entering sdf_fn_uaFormAuthenticateChallenge");

	(Sdf_ty_void)pCallObj;

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaFormAuthenticateChallenge():  Invalid error parameter");
#endif		
		return Sdf_co_fail;
	}	

	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"CallObject param passed is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pAuthParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"AuthParams param passed is invalid",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	*ppAuthenticateHeader=Sdf_co_null;

	/*
	 * Initialize a SipGenericChallenge structure and set the parameters
	 * for authentication in this structure.
	 */
	if (sip_initSipGenericChallenge(&pGenericChallenge, \
		(SipError*)&(pErr->stkErrCode)) == SipFail)
	{	
		pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"Error initializing Challenge",pErr);
#endif
		return Sdf_co_fail;
	}
	pTmpVal = Sdf_mc_strdupAuth(pAuthParams->pScheme);

	
	if (SipFail == sip_setSchemeInChallenge(pGenericChallenge, pTmpVal, \
		(SipError*)&(pErr->stkErrCode)))
	{
		pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"Error setting scheme in generic challenge",pErr);
#endif
		sip_freeSipGenericChallenge(pGenericChallenge);
		return Sdf_co_fail;	
	}	

	if (Sdf_mc_strcasecmp(pAuthParams->pScheme,"Basic") == 0)
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
			"Call : %s - Forming Www-Authenticate "
			"Header with Scheme as Basic", \
			pCallObj->pCommonInfo->pKey->pCallId);
		sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pErr);
#endif

		/*
		 * Set the realm in the generic challenge
		 */
		if(pAuthParams->pRealm != Sdf_co_null)
		{
			/*
			 * If the realm is in quotes, strip off the quotes before 
			 * invoking sdf_fn_uaInsertAuthenticationParamInChallenge.
			 */
			Sdf_ty_s8bit dTempRealm[Sdf_co_smallBufferSize];

			(void)sdf_fn_uaUnq(pAuthParams->pRealm, dTempRealm);

			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				,(Sdf_ty_s8bit *)"realm", dTempRealm, \
				Sdf_co_true, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting realm in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}
	}
	else
	{
#ifdef SDF_TRACE
		(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
			"Call : %s -  Forming Www-Authenticate "
			"Header with Scheme as %s", pCallObj->pCommonInfo->pKey->pCallId,\
			pAuthParams->pScheme);
		sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pErr);
#endif

		/*
		 * Set the realm in the generic challenge
		 */
		if (pAuthParams->pRealm != Sdf_co_null)
		{
			/*
			 * If the realm is in quotes, strip off the quotes before 
			 * invoking sdf_fn_uaInsertAuthenticationParamInChallenge.
			 */
			Sdf_ty_s8bit dTempRealm[Sdf_co_smallBufferSize];

			(void)sdf_fn_uaUnq(pAuthParams->pRealm, dTempRealm);

			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"realm", dTempRealm, \
				Sdf_co_true, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting realm in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}

		/*
		 * Set the domain in the generic challenge
		 */
		if (pAuthParams->pDomain != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"domain", pAuthParams->pDomain,\
				Sdf_co_true, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting domain in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}

		/*
		 * Set the nonce in the generic challenge
		 */
		if (pAuthParams->pNonce != Sdf_co_null)
		{
			/*
			 * If the nonce is in quotes, strip off the quotes before 
			 * invoking sdf_fn_uaInsertAuthenticationParamInChallenge.
			 */
			Sdf_ty_s8bit dTempNonce[Sdf_co_smallBufferSize];

			(void)sdf_fn_uaUnq(pAuthParams->pNonce, dTempNonce);

			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"nonce", dTempNonce, Sdf_co_true, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting nonce in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}	

		/*
		 * Set the stale in the generic challenge
		 */
		if (pAuthParams->pStale != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"stale", pAuthParams->pStale, \
				Sdf_co_false, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting stale in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}

		/*
		 * Set the algorithm in the generic challenge
		 */
		if (pAuthParams->pAlgorithm != Sdf_co_null)
		{
			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"algorithm", pAuthParams->pAlgorithm,\
				Sdf_co_false, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting algorithm in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}

		/*
		 * Set the opaque in the generic challenge
		 */
		if (pAuthParams->pOpaque != Sdf_co_null)
		{
			/*
			 * If the opaque is in quotes, strip off the quotes before 
			 * invoking sdf_fn_uaInsertAuthenticationParamInChallenge.
			 */
			Sdf_ty_s8bit dTempOpaque[Sdf_co_smallBufferSize];

			(void)sdf_fn_uaUnq(pAuthParams->pOpaque, dTempOpaque);

			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"opaque", dTempOpaque, Sdf_co_true, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting opaque in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}

		/*
		 * Set the qop in the generic challenge
		 */
		if (pAuthParams->pQop != Sdf_co_null)
		{
			/*
			 * If the qop is in quotes, strip off the quotes before 
			 * invoking sdf_fn_uaInsertAuthenticationParamInChallenge.
			 */
			Sdf_ty_s8bit dTempQop[Sdf_co_smallBufferSize];

			(void)sdf_fn_uaUnq(pAuthParams->pQop, dTempQop);

			if (sdf_fn_uaInsertAuthenticationParamInChallenge(pGenericChallenge\
				, (Sdf_ty_s8bit *)"qop", dTempQop, Sdf_co_true, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
					"Error setting qop in generic challenge",pErr);
#endif
				sip_freeSipGenericChallenge(pGenericChallenge);
			}
		}
	} /* End of else block handling Digest authentication scheme */

	if(SipFail == sip_initSipHeader(ppAuthenticateHeader,\
		SipHdrTypeWwwAuthenticate,(SipError*)&(pErr->stkErrCode)))
	{
		pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"Error initializing WWW-Authenticate header",pErr);
#endif
		sip_freeSipGenericChallenge(pGenericChallenge);
		return Sdf_co_fail;
	}

	if (SipFail == sip_setChallengeInWwwAuthenticateHdr (*ppAuthenticateHeader,\
		pGenericChallenge, (SipError*)&(pErr->stkErrCode)))
	{
		pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaFormAuthenticateChallenge(): "
			"Error setting the Challenge in WWW-Authenticate Header",pErr);
#endif
		sip_freeSipGenericChallenge(pGenericChallenge);
		sip_freeSipHeader(*ppAuthenticateHeader);
		return Sdf_co_fail;
	}
	/* Free the local reference */
	sip_freeSipGenericChallenge(pGenericChallenge);

	sdf_fn_debug((Sdf_ty_s8bit*)\
		"SDF_DEBUG--Exiting sdf_fn_uaFormAuthenticateChallenge");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/*********************************************************
** FUNCTION: sdf_fn_uaUnq
**
** DESCRIPTION: This function  returns the string without
**  double quotes
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaUnq
#ifdef ANSI_PROTO
	(Sdf_ty_s8bit* pInput, Sdf_ty_s8bit *pOutput)
#else
	(pInput, pOutput)
	Sdf_ty_s8bit* pInput;
	Sdf_ty_s8bit* pOutput;
#endif
{
	Sdf_ty_s8bit *pTemp = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG--Entering sdf_fn_uaUnq");

	pOutput[0] = '\0';

	if(pInput == Sdf_co_null)
		return Sdf_co_success;

	if((Sdf_mc_strchr(pInput,'\"')) == Sdf_co_null)
	{
		Sdf_mc_strcpy(pOutput, pInput);
		return Sdf_co_success;
	}

	pTemp = pInput + 1;
	Sdf_mc_strcpy(pOutput, pTemp);
	pOutput[Sdf_mc_strlen(pOutput)-1] = '\0';

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG--Exiting sdf_fn_uaUnq");
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaGetAuthParams
**
** DESCRIPTION: This fills up the authParams structure from the 
**				WWW-Authenticate/Proxy-Authenticate header
**
**********************************************************/

Sdf_ty_retVal sdf_ivk_uaGetAuthParams
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_ty_slist *pslAuthenticationParamsList,
	Sdf_st_error *pError)
#else
	(pCallObj, pOverlapTransInfo, pslAuthenticationParamsList, pError)
	Sdf_st_callObject *pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_slist *pslAuthenticationParamsList;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_listIterator dListIterator1, dListIterator2;
	Sdf_ty_s8bit *pTemp = Sdf_co_null;
	Sdf_ty_u32bit dIndex = 0, dCount = 0;
	SipGenericChallenge *pChallenge = Sdf_co_null;
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG--Entering sdf_ivk_uaGetAuthParams");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetAuthParams():  Invalid error parameter");
#endif		
		return Sdf_co_fail;
	}	

	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
			"Call Object param passed is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	/*
	 * Set pProcessedHeaderList to the correct processed headers list in the
	 * call object.
	 */
	if(pOverlapTransInfo == Sdf_co_null)
		pProcessedHeaderList = &(pCallObj->pUacTransaction->slProcessedHeaders);
	else
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);

	/* 
	 * Pick up the Www-authenticate and Proxy-authenticate headers from the
	 * Processedheaders list if it exists. If it is present, the scheme 
	 * used in the authentication is one of Basic/Digest. Else it is an 
	 * Unsupported scheme 
	 */
	(void)sdf_listInitIterator(pProcessedHeaderList, \
		&dListIterator1, pError);

	while (dListIterator1.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator1.pCurrentElement->pData);

		/*
		 * If the headerlist is neither of type SipHdrTypeWwwAuthenticate
		 * or SipHdrTypeProxyAuthenticate, then pick up the next element
		 */
		if ( (pHeaderList->dType != SipHdrTypeWwwAuthenticate) &&
			(pHeaderList->dType != SipHdrTypeProxyAuthenticate) )
		{
			(void)sdf_listNext(&dListIterator1, pError);
			continue;
		}

		(void)sdf_listInitIterator(&(pHeaderList->slHeaders), \
			&dListIterator2, pError);

		while (dListIterator2.pCurrentElement != Sdf_co_null)
		{
			SipHeader *pHeader = Sdf_co_null;
			Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;

			if (sdf_ivk_uaInitAuthenticationParams(&pAuthenticationParams, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
					"Failed to initialize Sdf_st_authenticationParams "
					"structure",pError);
#endif
				return Sdf_co_fail;
			}

			/*
			 * Retrieve the header from pHeaderList. 
			 */
			pHeader = (SipHeader*)\
				(dListIterator2.pCurrentElement->pData);

			if (pHeaderList->dType == SipHdrTypeWwwAuthenticate)
			{
				/* 
				 * Extract paramters from Www-authenticate header
				 */
				if (sip_getChallengeFromWwwAuthenticateHdr(pHeader, \
					&pChallenge, (SipError *)&(pError->stkErrCode)) == SipFail)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
						"Error Getting challenge from Www-Authenticate Hdr", \
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				/*
				 * Set the dHdrType to SipHdrTypeWwwAuthenticate. This is
				 * to facilitate sending a Www-Authorization/Proxy-Authorization
				 * header for this challenge
				 */
				pAuthenticationParams->dHdrType = SipHdrTypeWwwAuthenticate;
			}
			else
			{
				/* 
				 * Extract parameters from Proxy-authenticate header
				 */
				if (sip_getChallengeFromProxyAuthenticateHdr(pHeader, \
					&pChallenge, (SipError *)&(pError->stkErrCode)) == SipFail)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): Error "
						"getting challenge from Proxy-Authenticate Header", \
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				/*
				 * Set the dHdrType to SipHdrTypeProxyAuthenticate. This is
				 * to facilitate sending a Www-Authorization/Proxy-Authorization
				 * header for this challenge
				 */
				pAuthenticationParams->dHdrType = SipHdrTypeProxyAuthenticate;
			}

			/* 
			 * Get the scheme for authentication ("Basic"/"Digest")
			 */
			if (sip_getSchemeFromChallenge(pChallenge, &pTemp, \
				(SipError *)&(pError->stkErrCode))	== SipFail)
			{
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
				sip_freeSipGenericChallenge(pChallenge);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
					"Error getting Scheme from Authenticate Challenge",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			
			if(pTemp == Sdf_co_null)
			{
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
				sip_freeSipGenericChallenge(pChallenge);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
					"Error getting Scheme from Authenticate Challenge",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			pAuthenticationParams->pScheme = Sdf_mc_strdupAuth(pTemp);	
			if (Sdf_mc_strcasecmp(pTemp,"Basic") == 0)
			{

#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
				  Sdf_en_basicSchemeNotImplemented,\
				  (Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
				  "Failed to get Authorizationticate because scheme was Basic",\
					pError);
#endif
				pError->errCode=Sdf_en_basicSchemeNotImplemented;
				return Sdf_co_fail;
			}
			
			if (sip_getAuthorizationParamCountFromChallenge(pChallenge, \
				&dCount, (SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
				sip_freeSipGenericChallenge(pChallenge);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
					"Failed to get AuthorizationParam count from challenge", \
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			
			/* 
			 * Iterate through the parameter list of the WWW-Authenticate /
			 * Proxy-Authenticate header and retrieve the parameters into the 
			 * authparams structure.
			 */
			for (dIndex = 0; dIndex < dCount; dIndex++)
			{
				SipParam *pParam;
				Sdf_ty_s8bit *pName, *pValue;
				Sdf_ty_s8bit pTempValue[Sdf_co_smallBufferSize];

				pParam=Sdf_co_null;
				
				if (sip_getAuthorizationParamAtIndexFromChallenge(pChallenge, \
					&pParam, dIndex,(SipError *) &(pError->stkErrCode)) == \
					SipFail)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
					sip_freeSipGenericChallenge(pChallenge);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
						"Error Getting the param from Authenticate Challenge",\
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				if (sip_getNameFromSipParam(pParam, &pName, \
					 (SipError *)&(pError->stkErrCode)) == SipFail)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
					sip_freeSipGenericChallenge(pChallenge);
					sip_freeSipParam(pParam);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
						"Error getting param name from Authenticate Challenge",\
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				if (sip_getValueAtIndexFromSipParam(pParam, &pValue, 0,\
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
					sip_freeSipGenericChallenge(pChallenge);
					sip_freeSipParam(pParam);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
						"Error Getting param from Authenticate Challenge", \
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}

				/* Free the local reference */
				sip_freeSipParam(pParam);
				
				if(pValue == Sdf_co_null)
				{
					(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
					sip_freeSipGenericChallenge(pChallenge);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
						"Authenticate param has no value", pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}

				if (Sdf_mc_strcasecmp(pName, "realm") == 0)
					pAuthenticationParams->pRealm = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "domain") == 0)
					pAuthenticationParams->pDomain = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "nonce") == 0)
					pAuthenticationParams->pNonce = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "opaque") == 0)
					pAuthenticationParams->pOpaque = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "stale") == 0)
					pAuthenticationParams->pStale = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "algorithm") == 0)
					pAuthenticationParams->pAlgorithm = Sdf_mc_strdupAuth(pValue);
				else if (Sdf_mc_strcasecmp(pName, "qop") == 0)
				{
                                    Sdf_ty_s8bit *pQopValue = Sdf_co_null;
                                    Sdf_ty_s8bit *pTempQopValue = Sdf_co_null;
                   
                                    (void)sdf_fn_uaUnq(pValue, pTempValue);
                                    pQopValue = Sdf_mc_strtokr(pTempValue,",",&pTempQopValue);
                                    if ((Sdf_co_null != pTempQopValue) &&
                    	                (0 == Sdf_mc_strcasecmp(pTempQopValue, "auth-int")))
                                    {
                    	                 Sdf_mc_strcpy(pQopValue, pTempQopValue);
                                    }
                    
                                    pAuthenticationParams->pQop = Sdf_mc_strdupAuth(pQopValue);
				}
			}
			/* 
			 * If algorithm has not been specified, it is assumed to be MD-5 
			 * according to RFC-2617
			 */
			if (pAuthenticationParams->pAlgorithm == Sdf_co_null)
			{
				Sdf_mc_strdup(pAuthenticationParams->pAlgorithm, "md5");
			}

			/* Freeing the local reference */
			sip_freeSipGenericChallenge(pChallenge);

			/*
			 * Append this pAuthenticationParams structure to the
			 * pslAuthenticationParamsList 
			 */
			if (sdf_listAppend(pslAuthenticationParamsList, \
				(Sdf_ty_pvoid)pAuthenticationParams, pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeAuthenticationParams(pAuthenticationParams);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetAuthParams(): "
					"Failed to append AuthenticationParams structure to "
					"list of authentication structures.",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			(void)sdf_listNext(&dListIterator2, pError);
		} /* End of for loop iterating thru list of headers in the headerlist*/
		(void)sdf_listNext(&dListIterator1, pError);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG--Exiting sdf_ivk_uaGetAuthParams");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFormRequestWithAuthorization
**
** DESCRIPTION: This forms the request with the 
**				Authorization/Proxy-Authorization header
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFormRequestWithAuthorization
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_ty_s8bit *pMethod, 
	Sdf_ty_bool dIncludeMsgBody,
	Sdf_ty_slist *pslAuthenticationParams,
	Sdf_ty_slist *pslUacAuthInfo,
	Sdf_st_error *pError)
#else
	(pObject,pMethod,pOverlapTransInfo,dIncludeMsgBody,\
		pslAuthenticationParams, pslUacAuthInfo, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_ty_s8bit *pMethod;
	Sdf_ty_bool dIncludeMsgBody;
	Sdf_ty_slist *pslAuthenticationParams;
	Sdf_ty_slist *pslUacAuthInfo;
	Sdf_st_error *pError;
#endif	
{
	SipGenericCredential *pGenericCredential = Sdf_co_null;
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;
	Sdf_st_headerList *pAuthorizationHeaderList = Sdf_co_null;
	Sdf_st_headerList *pProxyAuthorizationHeaderList = Sdf_co_null;
	Sdf_ty_s8bit dTempMethod[Sdf_co_characterBufferSize] = "";
	Sdf_ty_slist *pProcessedHeaderList=Sdf_co_null;
	Sdf_ty_u32bit i = 0; 
	Sdf_ty_u32bit dListSizeAuthenticationParams = 0, dListSizeUacAuthInfo = 0;
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	SipMessage *pSipMsg = Sdf_co_null;
	Sdf_ty_messageType	dMsgType;
	Sdf_st_msgInfo			dMessageInfo;
#ifdef SDF_TRACE
	Sdf_ty_s8bit trace[Sdf_co_smallBufferSize];
#endif
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_ty_u32bit 		dCount = 0;
	Sdf_ty_u32bit   	dIndex = 0;

	/* 
	 * SPR - 2102 FIX.
	 * Reset the MediaState so that MediaState transitions are done
	 * properly when the new request is sent with credentials. 
	 */
	if (pObject->pCallInfo->dState == Sdf_en_idle)
		pObject->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	else if (pObject->pCallInfo->dState == Sdf_en_callEstablished)
		pObject->pMediaInfo->dMediaState = Sdf_en_establishedMedia;

	if(sdf_listSizeOf(pslAuthenticationParams, &dListSizeAuthenticationParams,
		pError) == Sdf_co_fail)
			return Sdf_co_fail;
	if(sdf_listSizeOf(pslUacAuthInfo, &dListSizeUacAuthInfo, pError) \
					== Sdf_co_fail)
			return Sdf_co_fail;

	if (dListSizeAuthenticationParams != dListSizeUacAuthInfo)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Number of authenticationParam structures passed not equal to "
			"number of UacAuthInfo structures passed.",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
	}

	Sdf_mc_strcpy(dTempMethod, pMethod);

	(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pError);

	dMessageInfo.dMsgType 	= dMsgType;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dResponse	= 0;
	dMessageInfo.pMethod = pMethod;
	
	dTransactionType = sdf_fn_getTransactionType(Sdf_co_null, dMessageInfo, \
						Sdf_co_false, pError);

	/* 
	 * Increment the CSeq for all requests other than CANCEL
	 */
	if (dMsgType != Sdf_en_cancel)
	{
		if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Error Getting the param from Authenticate Challenge",pError);
#endif
			pError->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		 /* 
		  * For PRACK/COMET:
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
			(dMsgType == Sdf_en_comet) ||
			(dTransactionType == Sdf_en_RegularUacTransaction) )
#else
		if ((dMsgType == Sdf_en_comet) ||
			(dTransactionType == Sdf_en_RegularUacTransaction) )
#endif /* SIP_RPR */
		{
			if (pCommonInfo->dLocalRegCseq > pCommonInfo->dLocalCseq)
				pCommonInfo->dLocalRegCseq += 1;
			else
				pCommonInfo->dLocalRegCseq = pCommonInfo->dLocalCseq + 1;
		
			pOverlapTransInfo->dLocalCseq = pCommonInfo->dLocalRegCseq;
		}
		else
		{
			/* 
			 * For other requests: 
			 * Set dLocalCseq to Max(dLocalCseq, dLocalRegCseq)+1 
			 */
			if (pCommonInfo->dLocalRegCseq > pCommonInfo->dLocalCseq)
				pCommonInfo->dLocalCseq = pCommonInfo->dLocalRegCseq + 1;
			else	
				pCommonInfo->dLocalCseq++ ;
		}

		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	}
	else
	{
		/* 
		 * For CANCEL, pick up the Cseq from the 401/407 in the CallObject
		 */
		SipHeader dTempHeader;
		SipMessage *pIncomingMsg=Sdf_co_null;

		if (pOverlapTransInfo == Sdf_co_null)
			pIncomingMsg = pObject->pUacTransaction->pSipMsg;
		else
			pIncomingMsg = pOverlapTransInfo->pSipMsg;

		if (sip_getHeader(pIncomingMsg, SipHdrTypeCseq, &dTempHeader, \
			(SipError*)&(pError->stkErrCode)) != SipFail)
		{
			Sdf_ty_u32bit dCseq;
			if (sip_getSeqNumFromCseqHdr(&dTempHeader,&dCseq,\
				(SipError *) &(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization():"\
					"Error getting sequence number from Cseq header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, \
				pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Error getting CommonInfo from Call Object",pError);
#endif
				return Sdf_co_fail;
			}
			pCommonInfo->dLocalCseq = dCseq;

			sip_freeSipHeader(&dTempHeader);
			(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
		}		
	}

	/* 
	 * If CANCEL is being resent, it is being sent on a temporary call object. 
	 * Since the pDestInfo->pScheme is not known at this point of time, it is 
	 * picked up from the 401/407 message
	 */
	if (dMsgType == Sdf_en_cancel)
	{	
		SipHeader dTempHeader;
		Sdf_ty_s8bit *pSentProto;
		Sdf_st_transportInfo *pDestInfo;
		SipMessage *pIncomingMsg=Sdf_co_null;

		if (pOverlapTransInfo == Sdf_co_null)
			pIncomingMsg = pObject->pUacTransaction->pSipMsg;
		else
			pIncomingMsg = pOverlapTransInfo->pSipMsg;

		if (sip_getHeaderAtIndex(pIncomingMsg, SipHdrTypeVia, \
			&dTempHeader, 0, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Error Accessing Via Header From Sip Message",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sip_getSentProtocolFromViaHdr(&dTempHeader, &pSentProto, \
			(SipError*)&(pError->stkErrCode)) == SipFail)
		{	
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to get sent protocol from Via header of the request", \
				pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}	
		sip_freeSipHeader(&dTempHeader);

		if (sdf_ivk_uaGetDestInfoFromTransaction(pObject->pUacTransaction,\
			pOverlapTransInfo, &pDestInfo, pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to get destination Info from the Call Object",pError);
#endif
			return Sdf_co_fail;
		}
	
    
        if (Sdf_mc_strstr(pSentProto, "UDP") != Sdf_co_null)
            pDestInfo->dScheme = Sdf_en_protoUdp;
#ifdef SDF_TLS
	    else if (Sdf_mc_strstr(pSentProto, "TLS") != Sdf_co_null)
            pDestInfo->dScheme = Sdf_en_protoTls;
#endif
	    else	
            pDestInfo->dScheme = Sdf_en_protoTcp;
       
        (void)sdf_ivk_uaFreeTransportInfo(pDestInfo);	
     }
	/*Before Calling FormRequest delete previous entries of authorization and 
	 * proxy-authorization headers from the persistent list.
	 * Previous entries might be there if we were challenged more than once.
	 */
/*w534*/(void)sdf_ivk_uaDeleteHeaderListByCorrelation(&(pObject->slPersistentHeaders),\
					"uat-authenticate",pError);
/*w534*/(void)sdf_ivk_uaDeleteHeaderListByCorrelation(&(pObject->slPersistentHeaders),\
					"uat-proxyauth",pError);
	
	/*Srikanth: To Tag Fix moved from FormRequest to Here.
	 * When a Request is re-formed using this API after getting challenged
	 * from the peer,we need to remove the To tag in this Request*/

 	/* Rohit Aggarwal : Fix for missing To tag in refresh SUBSCRIBE 
         * request with credentials:
         * Issue was that when a refresh SUBSCRIBE request is challenged, 
         * the next SUBSCRIBE message with credentials was not carrying the 
         * To tag, which was creating a new dialog. This was causing problems 
         * with the subsequent NOTIFYs sent by the server
         * The fix is that the To tag should not be deleted here in case of 
         * overlap transactions.
         */
	if ((pObject->pCallInfo->dState == Sdf_en_idle) &&
        (pOverlapTransInfo == Sdf_co_null))
        {
		if(sip_getTagCountFromToHdr(pObject->pCommonInfo->pTo,&dCount,\
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequestWithAuthorization(): "\
				"Failed to get tag count from to header", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}

		if (dCount != 0)
		{
			for(dIndex=0;dIndex<dCount;dIndex++)
			{
				if(sip_deleteTagAtIndexInToHdr(pObject->pCommonInfo->pTo, \
					dIndex, (SipError *)&(pError->stkErrCode))==SipFail)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
						,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequest(): "\
						"Failed to init get tag count from to header", pError);
#endif		
					pError->errCode=Sdf_en_formRequestError;
					return Sdf_co_fail;
				}
			}
		}
	}


	/* 
	 * Form the new Request
	 */
	if (sdf_ivk_uaFormRequest(pMethod, pObject, pOverlapTransInfo, pError) \
		==  Sdf_co_fail)
	{	
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to Form Request Message",pError);
#endif
		return Sdf_co_fail;
	}	

	/* Form register Request, we need to set the Expires header and Contact
	 * header as we have sent in the original Request.
	 */ 

	if((dMsgType == Sdf_en_register) && 
#ifdef SDF_LINT
		 (Sdf_co_null != pOverlapTransInfo) && 
#endif        
	   (Sdf_co_null != pOverlapTransInfo->pLastSipMsg))
	{
		SipHeader 		TempHeader;
		Sdf_ty_u32bit	dContactSize = 0;

#if defined(MICROUA) && !defined(SIP_DATE)
		if(sip_getHeaderCount(pOverlapTransInfo->pLastSipMsg, 
					SipHdrTypeExpires, &dCount, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
#else
		if(sip_getHeaderCount(pOverlapTransInfo->pLastSipMsg, 
					SipHdrTypeExpiresAny, &dCount, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
#endif
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
				,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequestWithAuthorization(): "\
				"Failed to get Expire Header count", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}
		
		/* Set the Expire header only if it is set in Last REGISTER Req */
		if(0 != dCount)
		{
			/* Get the Expiers header for the Register Request sent in the
		   	 * pLastSipMsg. 
		 	 */
#if defined(MICROUA) && !defined(SIP_DATE)
			if(sip_getHeaderAtIndex(pOverlapTransInfo->pLastSipMsg, \
					SipHdrTypeExpires,&TempHeader, 0, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
#else
			if(sip_getHeaderAtIndex(pOverlapTransInfo->pLastSipMsg, \
					SipHdrTypeExpiresAny,&TempHeader, 0, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
#endif
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
					,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequestWithAuthorization():"\
					"Failed to get Expire Header", pError);
#endif		
				pError->errCode=Sdf_en_formRequestError;
				return Sdf_co_fail;
			}
			/* Add the Expiers header for the Register Request sent in the
		   	 * pLastSipMsg(last Request Msg). 
		 	 */
			if(sip_setHeaderAtIndex(pOverlapTransInfo->pSipMsg, \
					&TempHeader, 0, (SipError *)&(pError->stkErrCode)) \
			   		!= SipSuccess)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
					,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequestWithAuthorization():"\
					"Failed to set Expire Header", pError);
#endif		
				pError->errCode=Sdf_en_formRequestError;
				return Sdf_co_fail;
			}
			/* Free the temp Expire header. */
			sip_freeSipHeader(&TempHeader);
		}
		
		/* Remove the Existing Conatct Header from the incoming Message. */
/*w534*/(void)sip_deleteAllHeaderType(pOverlapTransInfo->pSipMsg,\
				SipHdrTypeContactAny,(SipError *)&(pError->stkErrCode));
		
		/* find out the number of Contact header sent in Last Sip Request. */
		if(sip_getHeaderCount(pOverlapTransInfo->pLastSipMsg, \
					SipHdrTypeContactAny, &dContactSize, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
				,(Sdf_ty_s8bit*)"sdf_ivk_uaFormRequestWithAuthorization():"\
				"Failed to get Contact Header count", pError);
#endif		
			pError->errCode=Sdf_en_formRequestError;
			return Sdf_co_fail;
		}
		
		if(0 != dContactSize)
		{
			/* Set all Contact header sent in Last Sip Request to SipMsg. */
			for(dIndex=0; dIndex < dContactSize; dIndex++)
			{
				SipHeader ContactHeader;

				if(sip_getHeaderAtIndex(pOverlapTransInfo->pLastSipMsg, \
					SipHdrTypeContactAny, &ContactHeader, dIndex, \
					(SipError *)&(pError->stkErrCode)) != SipSuccess)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
						,(Sdf_ty_s8bit*) \
						"sdf_ivk_uaFormRequestWithAuthorization(): "\
						"Failed to get Contact Header", pError);
#endif		
					pError->errCode=Sdf_en_formRequestError;
					return Sdf_co_fail;
				}
				
				/* Add the Contact header for the Register Request sent in the
		   	 	 * pLastSipMsg(last Request Msg). 
		 	 	 */
				if(sip_insertHeaderAtIndex(pOverlapTransInfo->pSipMsg, \
					&ContactHeader, dIndex, (SipError *)&(pError->stkErrCode)) \
			   		!= SipSuccess)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_formRequestError\
						,(Sdf_ty_s8bit*) \
						"sdf_ivk_uaFormRequestWithAuthorization(): "\
						"Failed to set Contact Header", pError);
#endif		
					pError->errCode=Sdf_en_formRequestError;
					return Sdf_co_fail;
				}
				/* Free the temp Contact header. */
				sip_freeSipHeader(&ContactHeader);
			}
		}
		else
		{
			Sdf_ty_s8bit dContact[] = "Contact: *";

/*w534*/(void)sip_insertHeaderFromStringAtIndex(pOverlapTransInfo->pSipMsg,\
					SipHdrTypeContactWildCard, dContact,(Sdf_ty_u32bit)0,\
					(SipError *)&(pError->stkErrCode));
		}
	}

	if (sdf_ivk_uaGetTransactionFromCallObject(pObject, \
				Sdf_en_uacTransaction, &pTransaction, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to get UAC transaction from call object",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
	}

	/*
	 * Make pSipMsg point to the formed SipMessage
	 */
	if (pOverlapTransInfo == Sdf_co_null)
		pSipMsg = pTransaction->pSipMsg;
	else
		pSipMsg = pOverlapTransInfo->pSipMsg;

	if (dIncludeMsgBody == Sdf_co_true)
	{
		if (sdf_ivk_uaSetAllMsgBody(pTransaction, pOverlapTransInfo, pError)\
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to set Message body from Call Object into outgoing"
				"SIP message",pError);
#endif
			(void)sdf_ivk_uaFreeTransaction(pTransaction);
			return Sdf_co_fail;
		}
	}

	/* Free the local reference */
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	/*
	 * Initialize a header list each for Authorization and Proxy-Authorization
	 * headers
	 */
	if (sdf_ivk_uaInitHeaderList(&pAuthorizationHeaderList, \
		SipHdrTypeAuthorization, (Sdf_ty_s8bit *)"uat-authenticate", pError) \
					== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to init header list of type Authorization",pError);
#endif
		return Sdf_co_fail;
	}

	if (sdf_ivk_uaInitHeaderList(&pProxyAuthorizationHeaderList, \
		SipHdrTypeProxyauthorization, (Sdf_ty_s8bit *)"uat-proxyauth", pError)\
			== Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to init header list of type Proxy-Authorization", \
			pError);
#endif
		return Sdf_co_fail;
	}

	/*
	 * Iterate through the list of authentication params and UacAuthInfo 
	 * structures and form the appropriate headers
	 */
	for (i = 0; i < dListSizeAuthenticationParams; i++)
	{
		Sdf_st_authenticationParams *pAuthenticationParams = Sdf_co_null;
		Sdf_st_uacAuthInfo *pUacAuthInfo = Sdf_co_null;
		SipHeader *pHeader = Sdf_co_null;

		if (sdf_listGetAt(pslAuthenticationParams, i, \
			(Sdf_ty_pvoid *)&pAuthenticationParams, \
			pError) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
			(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to get authenticationParam at index from list of"
				"authentication params passed.",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if (sdf_listGetAt(pslUacAuthInfo, i, (Sdf_ty_pvoid *)&pUacAuthInfo, \
			pError) == Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
			(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to get UacAuthInfo at index from list of"
				"UacAuthInfo structures passed.",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if(pUacAuthInfo->pPassword !=Sdf_co_null)
		{
			if(pObject->pCommonInfo->pChallangePasswd != Sdf_co_null)
				(void)sdf_memfree(Sdf_mc_authMemId, (Sdf_ty_pvoid*)\
					&(pObject->pCommonInfo->pChallangePasswd), pError);
				
			pObject->pCommonInfo->pChallangePasswd = \
						Sdf_mc_strdupAuth(pUacAuthInfo->pPassword);
		}

		/* 
		 * Form the appropriate request header (Authorization in case of 
		 * WWW-Authenticate and Proxy-Authorization in case of 
		 * Proxy-Authenticate) 
		 */
		if (sdf_fn_uaFormAuthorizationCredential(pSipMsg, dTempMethod, \
			&pGenericCredential, pAuthenticationParams, pUacAuthInfo, pError) \
			== Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
			(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_authError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to form the Authorization Credential",pError);
#endif
			pError->errCode=Sdf_en_authError;
			return Sdf_co_fail;
		}

		if (pAuthenticationParams->dHdrType == SipHdrTypeWwwAuthenticate)
		{
			/*
			 * Authorization header is to be formed and sent
			 */
			if (sip_initSipHeader(&pHeader, SipHdrTypeAuthorization, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to initialize the Authorization Header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if (sip_setCredentialsInAuthorizationHdr(pHeader, \
				pGenericCredential,(SipError *) &(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to set credentials in Authorization Header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pHeader);
				return Sdf_co_fail;
			}
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s - Formed Authorization "
				"Header for %s Method", \
				pObject->pCommonInfo->pKey->pCallId, pMethod);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pError);
#endif
			/* Free the local reference */
			sip_freeSipGenericCredential(pGenericCredential);
		}
		else
		{
			/*
			 * Proxy-authorization header is to be formed and sent
			 */
			if (sip_initSipHeader(&pHeader, SipHdrTypeProxyauthorization, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to initialize Proxy-Authorization Header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if (sip_setCredentialsInProxyAuthorizationHdr(pHeader, \
				pGenericCredential,(SipError *) &(pError->stkErrCode)) == \
				SipFail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to set credentials in Proxy-Authorization Header",\
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipHeader(pHeader);
				return Sdf_co_fail;
			}
#ifdef SDF_TRACE
			(void)sdf_fn_uaSnprintf(trace,Sdf_co_smallBufferSize,\
				"Call : %s - Formed Proxy-Authorization "
				"Header for %s Method", \
				pObject->pCommonInfo->pKey->pCallId, pMethod);
			sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_authTraces,trace,0,pError);
#endif
			/* Free the local reference */
			sip_freeSipGenericCredential(pGenericCredential);
		}

		/* 
		 * Add the header directly into the SIP message. This is because
		 * sdf_ivk_uaFormRequest has already been invoked and the message bodies
		 * have also been attached. This is necessary as we cannot form the 
		 * response digest for authorization (in case of "Digest" 
		 * authentication scheme) without knowing hash of the entity body.
		 */
		if (sip_insertHeaderAtIndex(pSipMsg, pHeader, \
			0, (SipError *) &(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
			(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
				"Failed to set Authorization/Proxy-Authorization header to "
				"the SIP Message",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}


		if (pAuthenticationParams->dHdrType == SipHdrTypeWwwAuthenticate)
		{
			if (sdf_listAppend(&(pAuthorizationHeaderList->slHeaders), \
				pHeader,pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to append the Authorization header"
					"to the header list", pError);
#endif
				return Sdf_co_fail;
			}

		}
		else
		{
			if (sdf_listAppend(&(pProxyAuthorizationHeaderList->slHeaders), \
				pHeader,pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
				(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
					"Failed to append the Proxy-Authorization header"
					"to the header list", pError);
#endif
				return Sdf_co_fail;
			}
		}
	} /* End of for loop creating Authorization/Proxy-Authorization headers */


	/* 
	 * Append the pAuthorizationHeaderList and pProxyAuthorizationHeaderList
	 * to the appropriate header list structures in the call object
	 */
	if(pOverlapTransInfo == Sdf_co_null)
		pProcessedHeaderList = &(pObject->pUacTransaction->slProcessedHeaders);
	else
		pProcessedHeaderList = &(pOverlapTransInfo->slProcessedHeaders);

	if (sdf_listAppend(pProcessedHeaderList, pAuthorizationHeaderList, \
		pError) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
		(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to add the header list of Authorization headers to the "
			"ProcessedHeader list", pError);
#endif
		return Sdf_co_fail;
	}	

	/* Incrementing the Ref count for  pAuthorizationHeaderList */
	HSS_LOCKEDINCREF(pAuthorizationHeaderList->dRefCount);

	if (sdf_listAppend(pProcessedHeaderList, pProxyAuthorizationHeaderList, \
		pError) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
		(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to add the header list of Proxy-Authorization headers to "
			"the ProcessedHeader list", pError);
#endif
		return Sdf_co_fail;
	}	

	/* Incrementing the Ref count for  pProxyAuthorizationHeaderList */
	HSS_LOCKEDINCREF(pProxyAuthorizationHeaderList->dRefCount);

	if ((sdf_listAppend(&(pObject->slPersistentHeaders), \
		(Sdf_ty_pvoid)pAuthorizationHeaderList, pError)) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
		(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to add the header list of Authorization headers to "
			"the PersistentHeaders list of callObject", pError);
#endif
		return Sdf_co_fail;
	}
	if ((sdf_listAppend(&(pObject->slPersistentHeaders), \
		(Sdf_ty_pvoid)pProxyAuthorizationHeaderList, pError)) == Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeHeaderList(pAuthorizationHeaderList);
		(void)sdf_ivk_uaFreeHeaderList(pProxyAuthorizationHeaderList);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRequestWithAuthorization(): "
			"Failed to add the header list of Authorization headers to "
			"the PersistentHeaders list of callObject", pError);
#endif
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG--Exiting sdf_ivk_uaFormRequestWithAuthorization");
	
	pError->errCode = Sdf_en_noUaError;
	
	return Sdf_co_success;
}
/*****************************************************************************
** FUNCTION: sdf_fn_uaInsertAuthenticationParamInChallenge
**
** DESCRIPTION: Inserts an authentication parameter in the
**				SipGenericChallenge which is passed as a parameter
**				If the parameter should be a quoted string, the
**				quotes are also inserted in this API.
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInsertAuthenticationParamInChallenge
#ifdef ANSI_PROTO
(SipGenericChallenge *pGenericChallenge, 
 Sdf_ty_s8bit *pParamName, 
 Sdf_ty_s8bit *pParamValue, 
 Sdf_ty_bool dUseQuotes, 
 Sdf_st_error *pError)
#else
(pGenericChallenge, pParamName, pParamValue, dUseQuotes, pError)
SipGenericChallenge *pGenericChallenge;
Sdf_ty_s8bit *pParamName;
Sdf_ty_s8bit *pParamValue;
Sdf_ty_bool dUseQuotes;
Sdf_st_error *pError;
#endif
{
	SipParam *pSipParam = Sdf_co_null;
	Sdf_ty_s8bit dActualValue[Sdf_co_mediumBufferSize] = "";
	Sdf_ty_u32bit dParamCount = 0;
	Sdf_ty_s8bit	*pTmpVal = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaInsertAuthenticationParamInChallenge");
	

	/*
	 * If the param value should be a quoted string (acc. to RFC), then
	 * insert the quotes.
	 */
	if (dUseQuotes == Sdf_co_true)
		(void)sdf_fn_uaSprintf(dActualValue, "\"%s\"", pParamValue);
	else
		(void)sdf_fn_uaSprintf(dActualValue, "%s", pParamValue);

	if (sip_initSipParam(&pSipParam,(SipError *)&(pError->stkErrCode)) \
		== SipFail)
	{ 
		pError->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertAuthenticationParamInChallenge() : "
			"Error initializing SipParam",pError);
#endif
		return Sdf_co_fail;
	}	

	pTmpVal = Sdf_mc_strdupAuth(pParamName);
	if (sip_setNameInSipParam(pSipParam, pTmpVal, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertAuthenticationParamInChallenge(): "
			"Error setting name in SipParam",pError);
#endif
		sip_freeSipParam(pSipParam);
		return Sdf_co_fail;
	}	
	pTmpVal = Sdf_mc_strdupAuth(dActualValue);

	if (sip_insertValueAtIndexInSipParam(pSipParam, pTmpVal, \
		0, (SipError *) &(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertAuthenticationParamInChallenge(): "
			"Error inserting value in SipParam",pError);
#endif
		sip_freeSipParam(pSipParam);
		return Sdf_co_fail;
	}	

	if (sip_getAuthorizationParamCountFromChallenge(pGenericChallenge, \
		&dParamCount, (SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertAuthenticationParamInChallenge(): "
			"Failed to get count of authorization params from generic "
			"challenge",pError);
#endif
		sip_freeSipParam(pSipParam);
		return Sdf_co_fail;
	}

	if (sip_insertAuthorizationParamAtIndexInChallenge(pGenericChallenge, \
		pSipParam, dParamCount, (SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInsertAuthenticationParamInChallenge(): "
			"Error inserting SipParam in Authorization Header",pError);
#endif
		sip_freeSipParam(pSipParam);
		return Sdf_co_fail;
	}	

	/* Free the local reference */
	sip_freeSipParam(pSipParam);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_fn_uaInsertAuthenticationParamInChallenge");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}
#endif /* SDF_AUTHENTICATE */
