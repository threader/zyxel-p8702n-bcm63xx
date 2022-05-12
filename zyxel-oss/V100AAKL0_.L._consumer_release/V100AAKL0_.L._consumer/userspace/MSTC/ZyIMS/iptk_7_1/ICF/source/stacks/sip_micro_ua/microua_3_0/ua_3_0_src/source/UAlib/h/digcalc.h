#ifdef SDF_AUTHENTICATE
/******************************************************************************
** FUNCTION:
**			This file has the implementation of the Digest authentication
**			scheme.
**
*******************************************************************************
**
** FILENAME			: digcalc.h
**
** DESCRIPTION		: This file has the implementation of the Digest
**						authentication scheme
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/

#ifndef __SDFDIGCALC_H__
#define __SDFDIGCALC_H__

#ifndef SIP_NO_FILESYS_DEP
#include <stdio.h>
#endif

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];
#ifndef IN
#define IN
#endif
#define OUT

#ifdef __cplusplus
extern "C" {
#endif 

/* calculate H(A1) as per HTTP Digest spec */
void DigestCalcHA1 _ARGS_((
    IN char * pszAlg,
    IN char * pszUserName,
    IN char * pszRealm,
    IN char * pszPassword,
    IN char * pszNonce,
    IN char * pszCNonce,
    OUT HASHHEX SessionKey
    ));

/* calculate H(entity-body) as per spec */
void DigestCalcHEntity _ARGS_((
	IN char * pszEntityBody,
	int dLength,
	OUT HASHHEX HEntity
	));

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse _ARGS_((
    IN HASHHEX HA1,           /* H(A1) */
    IN char * pszNonce,       /* nonce from server */
    IN char * pszNonceCount,  /* 8 hex digits */
    IN char * pszCNonce,      /* client nonce */
    IN char * pszQop,         /* qop-value: "", "auth", "auth-int" */
    IN char * pszMethod,      /* method from the request */
    IN char * pszDigestUri,   /* requested URL */
    IN HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
    OUT HASHHEX Response      /* request-digest or response-digest */
    ));
void CvtHex _ARGS_((
    IN HASH Bin,
    OUT HASHHEX Hex
    ));
#ifdef __cplusplus
}
#endif 

#endif
#endif /* SDF_AUTHENTICATE */
