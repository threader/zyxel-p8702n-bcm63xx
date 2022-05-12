#ifdef SDF_AUTHENTICATE
/***********************************************************************
 ** FUNCTION:
 **			This file has all the API's related to authentication at the
 **			UAC/UAS end
 *********************************************************************
 **
 ** FILENAME		: sdf_authorization.h
 **
 ** DESCRIPTION		: This file has all the API's related to 
 **						authentication at the UAC/UAS end
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 15/01/01	Subhash						Creation
 ** 16/02/01    Seshashayi					UAS part of authentication
 ** 27/02/09    Alok Tiwari   IPTK Rel8.1   Wrapper functions added 
 **                                         for aDigest APIs
 ** 18-Jul-2009 Rajiv Kumar   IPTK Rel 8.2  Vxwork 5.4 porting
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2009
 *********************************************************************/

#ifndef __SDF_AUTHORIZATION_H_
#define __SDF_AUTHORIZATION_H_

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/*****************************************************************************
** FUNCTION: sdf_fn_uaFormAuthorizationCredential
**
** DESCRIPTION: Forms the credential to be sent to authorize
** 				a user.
** PARAMETERS:
**		pSipMsg(IN)					: The SipMessage that has been formed.
**		pMethod(IN)					: The method in the Request-URI
**    	ppGenericCredential(OUT) 	: The generic credential struture which will
**										be returned
**		pAuthenticationParams(IN) 	: The params for the Authorization header
**		pUacAuthInfo(IN) 			: UAC params for the Authorization header
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormAuthorizationCredential _ARGS_\
	((SipMessage *pSipMsg, Sdf_ty_s8bit *pMethod, \
	SipGenericCredential **ppGenericCredential, \
	Sdf_st_authenticationParams *pAuthenticationParams, \
	Sdf_st_uacAuthInfo *pUacAuthInfo, Sdf_st_error *pError));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaAuthorize 
**
** DESCRIPTION: This function is used to check whether
**				the incoming request has to be authorized. It issues the
**				authentication related callbacks to the application
** PARAMETERS:
**		ppCallObj(IN)			: The call object corresponding to the call
**		pOverlapTransInfo(IN)	: In case of PRACK/COMET, the overlapTransInfo
**									structure from which the authorization 
**									parameters are to be retreived
**		ppContext(IN/OUT)		: Used to pass any application specific data
**									across the authentication callbacks
**		pErr(OUT)	: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_authResult sdf_ivk_uaAuthorize _ARGS_((Sdf_st_callObject  \
	**ppCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_ty_pvoid *ppContext,Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_fn_uaMatchEncode
**
** DESCRIPTION: This function runs the Digest algorithm to match the 
**				H(A1) value	returned by the application against the
**				credentials provided by the peer entity.
** PARAMETERS:
**		pSipMsg(IN)		: The SipMessage that is to be validated
**		pAuthParams(IN)	: The authentication parameters 
**		pUasAuthInfo(IN): The UAS Auth Info parameters
**		pUri(IN)		: The digest-uri to be used
**		pNonceCount(IN)	: The nonce-count to be used
**		pResponse(IN)	: The response contained in the header
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaMatchEncode _ARGS_((SipMessage \
	*pSipMessage, Sdf_st_authenticationParams *pAuthParams,	\
	Sdf_st_uasAuthInfo *pUasAuthInfo, Sdf_ty_s8bit *pUri, \
	Sdf_ty_s8bit *pNonceCount, Sdf_ty_s8bit *pResponse, Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_fn_uaFormAuthenticateChallenge
**
** DESCRIPTION: This function is used to form a WWW-Authenticate header
**				to challenge the peer entity.
**
** PARAMETERS:
**		pCallObj(IN)				: The call object corresponding to the call
**		pAuthParams(IN) 			: The parameters for authorization
**		ppAuthenticateHeader(OUT) 	: The WWW-Authenticate header returned to
**										the application
**		pErr(OUT)					: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormAuthenticateChallenge _ARGS_(( \
	Sdf_st_callObject *pCallObj, Sdf_st_authenticationParams \
	*pAuhParams, SipHeader **ppAuthenticateHeader,\
	Sdf_st_error *pErr));

/***************************************************************************
** FUNCTION: sdf_fn_uaUnq
**
** DESCRIPTION: This function  returns the string without double quotes.
**
** PARAMETERS:
**			pInput(IN) : The string for which double quotes are to be removed
**			pOutput(IN) : Input string without double quotes. This should be
**						preallocated and passed to the function.
**
***************************************************************************/
 Sdf_ty_retVal sdf_fn_uaUnq _ARGS_((Sdf_ty_s8bit* pInput, \
 	Sdf_ty_s8bit *pOutput));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetAuthParams
**
** DESCRIPTION: This fills up the authParams structure from the 
**				WWW-Authenticate/Proxy-Authenticate header
**
**	PARAMETERS:
**		pCallObj(IN) 				: The call object corresponding to the call
**		pOverlapTransInfo(IN)		: In case of PRACK/COMET, the 
**										overlapTransInfo structure from which 
**										the authorization parameters are to 
**										be retreived
**		pslAuthenticationParams(OUT): List of authentication parameters 
**										retrieved
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetAuthParams _ARGS_((Sdf_st_callObject \
	*pCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_ty_slist *pslAuthenticationParams, Sdf_st_error *pError));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormRequestWithAuthorization
**
** DESCRIPTION: This forms the request with the 
**				Authorization/Proxy-Authorization header
**
**	PARAMETERS:
**		pObject(IN/OUT)				: The call object corresponding to the call
**		pOverlapTransInfo(IN)		: In case of PRACK/COMET, the 
**										overlapTransInfo structure for which 
**										the request is to be formed
**		pMethod(IN) 				: The method  of the SIP request to form
**		dIncludeMsgBody(IN) 		: Boolean indicating whether the MsgBody 
**										sent in the initial request should 
**										be resent
**		pslAuthenticationParams(IN) : List of authentication params to form 
**										the Auth/Proxy-Authorization header
**		pslUacAuthInfo(IN) 			: List of UAC params to form the 
**										Auth/Proxy-Authorization Header
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormRequestWithAuthorization _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_ty_s8bit *pMethod, Sdf_ty_bool dIncludeMsgBody, \
	Sdf_ty_slist *pslAuthenticationParams, Sdf_ty_slist *pslUacAuthInfo, \
	Sdf_st_error *pError));

/*****************************************************************************
** FUNCTION: sdf_fn_uaInsertAuthenticationParamInChallenge
**
** DESCRIPTION: Inserts an authentication parameter in the
**				SipGenericChallenge which is passed as a parameter
**				If the parameter should be a quoted string, the
**				quotes are also inserted in this API.
**
** PARAMETERS:
**		pGenericChallenge(IN/OUT)	: The SipGenericChallenge structure in 
**										which the authentication parameter 
**										is to be inserted
**		pParamName(IN)				: The name of the parameter
**		pParamValue(IN)				: The value of the parameter
**		dUseQuotes(IN)				: Boolean indicating whether the parameter
**										value should be surrounded by quotes
**		pError(OUT)					: Error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaInsertAuthenticationParamInChallenge _ARGS_\
	((SipGenericChallenge *pGenericChallenge, Sdf_ty_s8bit *pParamName, \
	Sdf_ty_s8bit *pParamValue, Sdf_ty_bool dUseQuotes, Sdf_st_error *pError));

/*****************************************************************************
** FUNCTION: sdf_fn_uaDigestCalcHA1
**
** DESCRIPTION: This is the wrapper function to use the aDigest API 
**              aDigest_calc_HA1.It creates the input parameters for API 
**              aDigest_calc_HA1 by using it's own functional parameters
**              and invokes the API aDigest_calc_HA1 to calculate the HA1 value.
** PARAMETERS:
**              pszAlg(IN)           : Algoritm Name
**              pszUserName(IN)      : User Name
**              pszRealm(IN)         : Realm
**              pszPassword(IN)      : Password
**              pszNonce(IN)         : Nonce value
**              pszCNonce(IN)        : CNonce value
**              SessionKey(OUT)      : It will contain the calculated output
**                                     i.e. H(A1).            
**
******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaDigestCalcHA1 _ARGS_\
	((Sdf_ty_s8bit *pszAlg, 
      Sdf_ty_s8bit *pszUserName,
      Sdf_ty_s8bit *pszRealm,
      Sdf_ty_s8bit *pszPassword,
      Sdf_ty_s8bit *pszNonce,
      Sdf_ty_s8bit *pszCNonce,
      Sdf_ty_s8bit *SessionKey)); 

/*****************************************************************************
** FUNCTION: sdf_fn_uaDigestCalcHEntity
**
** DESCRIPTION: This is the wrapper function to use the aDigest API 
**              aDigest_calc_HEntity.It creates the input parameters for API 
**              aDigest_calc_HEntity by using it's own functional parameters
**              and invokes the API aDigest_calc_HEntity to calculate
**              the HEntity value.
** PARAMETERS:
**              pszEntityBody(IN) : Entitybody value
**              dLength(IN)       : Length of entity body
**              HEntity(OUT)      : It will contain the calculated output
**                                  i.e. H(pszEntityBody).
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDigestCalcHEntity _ARGS_\
   ((Sdf_ty_s8bit *pszEntityBody,
     Sdf_ty_s16bit dLength,
     Sdf_ty_s8bit *HEntity)); 

/*****************************************************************************
** FUNCTION: sdf_fn_uaDigestCalcResponse
**
** DESCRIPTION: This is the wrapper function to use the aDigest API 
**              aDigest_calc_response.It creates the input parameters for API 
**              aDigest_calc_response by using it's own functional parameters
**              and invokes the API aDigest_calc_response to calculate
**              the Digest response.
** PARAMETERS:
**              HA1(IN)              : Session key 
**              pszNonce(IN)         : Nonce value
**              pszNonceCount(IN)    : Nonce Count
**              pszCNonce(IN)        : CNonce value 
**              pszQop(IN)           : qop value
**              pszMethod(IN)        : Method Name
**              pszDigestUri(IN)     : Digest request URI
**              HEntity(IN)          : Hash value of entity-body 
**              Response(OUT)        : Calculated Digest Response    
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDigestCalcResponse _ARGS_\
   ((Sdf_ty_s8bit *HA1,
     Sdf_ty_s8bit *pszNonce,
     Sdf_ty_s8bit *pszNonceCount,
     Sdf_ty_s8bit *pszCNonce,
     Sdf_ty_s8bit *pszQop,
     Sdf_ty_s8bit *pszMethod,
     Sdf_ty_s8bit *pszDigestUri,
     Sdf_ty_s8bit *HEntity,
     Sdf_ty_s8bit *Response));

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

#endif
#endif /* SDF_AUTHENTICATE */

