/***********************************************************************
** FUNCTION:
**		Accessor API functionality for various sub-structures.
************************************************************************
**
** FILENAME:
** microsip_subapi.c
**
** DESCRIPTION:
**		This file contains the accessor functions for various 
**		sub-structures used by the microSIP stack, such as
**		SipParam, SipAddrSpec, SipUrl, etc.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			Initial	
** 05-Jun-2009   Anuradha Gupta         SPR 19735       Klocworks Warning Removal	
**
**
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_subapi.h"
/****************************************************************************
** FUNCTION: sip_getAddrTypeFromAddrSpec
** DESCRIPTION: This function retrieves the dType of a SIP dAddr-spec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the address-spec
**				pAddrtpe(OUT)	- The retrieved dType
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getAddrTypeFromAddrSpec
	(SipAddrSpec *pAddrspec,
	en_AddrType *pAddrtype,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering getAddrTypeFromAddrSpec");
#ifndef SIP_NO_CHECK
	if(pErr == SIP_NULL)
		return SipFail;
	if ( pAddrspec == SIP_NULL || pAddrtype == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif

	*pAddrtype = pAddrspec->dType;
	SIPDEBUGFN( "Exitting getAddrTypeFromAddrSpec");

	*pErr=E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_getUriFromAddrSpec
** DESCRIPTION: This function retrieves the Uri from a addr-spec.
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				ppUri(OUT)		- The retrieved URI
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getUriFromAddrSpec
	(SipAddrSpec *pAddrspec,
	SIP_S8bit **ppUri,
	SipError *pErr)
{
	SIP_S8bit * pTemp_uri;
	SIPDEBUGFN( "Entering getUriFromAddrSpec");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ((pAddrspec == SIP_NULL)||(ppUri==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pAddrspec->dType != SipAddrReqUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	pTemp_uri = pAddrspec->u.pUri;

	if (pTemp_uri == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppUri = pTemp_uri;

	SIPDEBUGFN( "Exitting getUriFromAddrSpec");
	*pErr = E_NO_ERROR;

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setUriInAddrSpec
** DESCRIPTION: This function sets the Uri in a SIP addr-spec.
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				pUri(In)		- The URI to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setUriInAddrSpec
	(SipAddrSpec *pAddrspec,
	SIP_S8bit *pUri,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering _setUriInAddrSpec");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pAddrspec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pAddrspec->dType != SipAddrReqUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif

	if (pAddrspec->u.pUri !=SIP_NULL)
	{
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pAddrspec->u.pUri),\
			pErr) ==SipFail)
		return SipFail;
	}
	pAddrspec->u.pUri = pUri;

	SIPDEBUGFN( "Exitting setUriInAddrSpec");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_getUrlFromAddrSpec
** DESCRIPTION: This function retrieves the url from a SIP addr-spec
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				ppUrl(IN)		- The retrieved URL
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getUrlFromAddrSpec
	(SipAddrSpec *pAddrspec,
	SipUrl **ppUrl,
	SipError *pErr)
{
	SipUrl *pFrom;

	SIPDEBUGFN( "Entering getUrlFromAddrSpec");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pAddrspec == SIP_NULL) || (ppUrl==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	/*SPR 4613 fix */
	if (pAddrspec->dType != SipAddrSipUri && pAddrspec->dType != SipAddrSipSUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	pFrom=pAddrspec->u.pSipUrl;
	if (pFrom == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	HSS_LOCKEDINCREF(pFrom->dRefCount);
    *ppUrl = pFrom;
	SIPDEBUGFN( "Exitting getUrlFromAddrSpec ");

	*pErr=E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setUrlInAddrSpec
** DESCRIPTION: This function sets the url field in a SipAddrSpec
** PARAMETERS:
**				pAddrSpec(IN)	- Pointer to the SipAddrSpec
**				pUrl(IN)		- The URL to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setUrlInAddrSpec
	(SipAddrSpec *pAddrspec,
	SipUrl *pUrl,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering setUrlInAddrSpec");

#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrspec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pAddrspec->dType != SipAddrSipUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	sip_freeSipUrl(pAddrspec->u.pSipUrl);
	if (pUrl==SIP_NULL)
	{
		pAddrspec->u.pSipUrl = SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pUrl->dRefCount);
		pAddrspec->u.pSipUrl = pUrl;
	}	
	*pErr=E_NO_ERROR;
	SIPDEBUGFN( "Exitting setUrlInAddrSpec");
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_getSUrlFromAddrSpec
** DESCRIPTION: This function retrieves the sips url from a SipAddrSpec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				ppUrl(OUT)		- The retrieved SIPS URL
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getSUrlFromAddrSpec
	(SipAddrSpec *pAddrspec,
	SipUrl **ppUrl,
	SipError *pErr)
{
	SipUrl *pFrom;
	SIPDEBUGFN( "Entering getSUrlFromAddrSpec");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pAddrspec == SIP_NULL) || (ppUrl==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pAddrspec->dType != SipAddrSipSUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	pFrom=pAddrspec->u.pSipUrl;
	if (pFrom == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	HSS_LOCKEDINCREF(pFrom->dRefCount);
    *ppUrl = pFrom;

	SIPDEBUGFN( "Exitting getSUrlFromAddrSpec ");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setSUrlInAddrSpec
** DESCRIPTION: This function sets the sips url in a SipAddrSpec
** PARAMETERS:
**				pAddrspec(IN)	- Pointer to the SipAddrSpec
**				pUrl(IN)		- The SIPS URL to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setSUrlInAddrSpec
	(SipAddrSpec *pAddrspec,
	SipUrl *pUrl,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering setSUrlInAddrSpec");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrspec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pAddrspec->dType != SipAddrSipSUri)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	sip_freeSipUrl(pAddrspec->u.pSipUrl);
	if (pUrl==SIP_NULL)
	{
		pAddrspec->u.pSipUrl = SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pUrl->dRefCount);
		pAddrspec->u.pSipUrl = pUrl;
	}	

	*pErr=E_NO_ERROR;
	SIPDEBUGFN( "Exitting setSUrlInAddrSpec");
	return SipSuccess;
}

/***************************************************************
** FUNCTION: sip_getUserFromUrl
**
** DESCRIPTION: this function retrieves the pUser from a SIP Url
**
***************************************************************/

SipBool sip_getUserFromUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit **ppUser, SipError *pErr)
#else
	( pUrl, ppUser, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit **ppUser;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
#endif
	SIP_S8bit * pTemp_user;
	SIPDEBUGFN( "Entering getUserFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL || ppUser == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_user = ((SipUrl *) pUrl)->pUser;

	if( pTemp_user == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	*ppUser = pTemp_user;
#else
	dLength = sip_strlen(pTemp_user);
	*ppUser = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
	if ( *ppUser == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppUser , pTemp_user );
#endif
    SIPDEBUGFN( "Exitting getUserFromUrl");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_setUserInUrl
**
** DESCRIPTION: this function sets the pUser in a SIP Url
**
***************************************************************/

SipBool sip_setUserInUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit *pUser, SipError *pErr)
#else
	( pUrl, pUser, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit *pUser;
	  SipError *pErr;
#endif
{

#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_user;
#endif
	SIPDEBUGFN( "Entering setUserInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if(pUrl->pUser !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pUrl->pUser),pErr)\
																	==SipFail)
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	pUrl->pUser = pUser;
#else
	if( pUser == SIP_NULL)
		pTemp_user = SIP_NULL;
	else
	{
		dLength = sip_strlen( pUser );
		pTemp_user = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTemp_user== SIP_NULL )
			return SipFail;

		(void)sip_strcpy( pTemp_user, pUser );

	}
	pUrl->pUser = pTemp_user;
#endif

	SIPDEBUGFN( "Exitting setUserInUrl");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}


/***************************************************************
** FUNCTION: sip_getPasswordFromUrl
**
** DESCRIPTION: This function retrieves the passworxd field from
**		a SIP Url
**
***************************************************************/

SipBool sip_getPasswordFromUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit **ppPassword, SipError *pErr)
#else
	( pUrl, ppPassword, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit **ppPassword;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
#endif
	SIP_S8bit * pTemp_password;
	SIPDEBUGFN( "Entering getPasswordFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL || ppPassword == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_password = ((SipUrl *) pUrl)->pPassword;

	if( pTemp_password == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

#ifdef SIP_BY_REFERENCE
	*ppPassword = pTemp_password;
#else
	dLength = sip_strlen(pTemp_password);
	*ppPassword = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
	if ( *ppPassword == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppPassword , pTemp_password );
#endif
	SIPDEBUGFN( "Exitting getPasswordFromUrl");


	*pErr = E_NO_ERROR;
	return SipSuccess;

}


/***************************************************************
** FUNCTION: sip_setPasswordInUrl
**
** DESCRIPTION: This function sets the pPassword field in a SIP Url
**
***************************************************************/

SipBool sip_setPasswordInUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit *pPassword, SipError *pErr)
#else
	( pUrl, pPassword, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit *pPassword;
	  SipError *pErr;
#endif
{

#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_password;
#endif
	SIPDEBUGFN( "Entering setPasswordInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pUrl->pPassword != SIP_NULL )
	{
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pUrl->pPassword),\
										pErr) == SipFail)
			return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	pUrl->pPassword = pPassword;
#else

	if( pPassword == SIP_NULL)
		pTemp_password = SIP_NULL;
	else
	{
		dLength = sip_strlen( pPassword );
		pTemp_password = \
					( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTemp_password== SIP_NULL )
			return SipFail;
		(void)sip_strcpy( pTemp_password, pPassword );
	}
	pUrl->pPassword = pTemp_password;
#endif
	SIPDEBUGFN( "Exitting setPasswordInUrl");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_getHostFromUrl
**
** DESCRIPTION: This function retrieves the pHost field from a SIP
**		Url
**
***************************************************************/

SipBool sip_getHostFromUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit **ppHost, SipError *pErr)
#else
	( pUrl, ppHost, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit **ppHost;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
#endif
	SIP_S8bit * pTemp_host;
	SIPDEBUGFN( "Entering getHostFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( pUrl == SIP_NULL || ppHost == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_host = ((SipUrl *) pUrl)->pHost;

	if( pTemp_host == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	 *ppHost = pTemp_host;
#else
	dLength = sip_strlen(pTemp_host);
	*ppHost = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
	if ( *ppHost == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppHost , pTemp_host );
#endif
	SIPDEBUGFN( "Exitting getHostFromUrl");


	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_setHostInUrl
**
** DESCRIPTION: this function sets the pHost field in a SIP Url
**
***************************************************************/

SipBool sip_setHostInUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit *pHost, SipError *pErr)
#else
	( pUrl, pHost, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit *pHost;
	  SipError *pErr;
#endif
{

#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_host;
#endif
	SIPDEBUGFN( "Entering setHostInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pUrl->pHost != SIP_NULL )
	{
		if ( sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&( pUrl->pHost), pErr)\
																	== SipFail)
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	pUrl->pHost = pHost;
#else
	if( pHost == SIP_NULL)
		pTemp_host = SIP_NULL;
	else
	{
		dLength = sip_strlen( pHost );
		pTemp_host = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTemp_host== SIP_NULL )
			return SipFail;

		(void)sip_strcpy( pTemp_host, pHost );

	}

	pUrl->pHost = pTemp_host;
#endif
	SIPDEBUGFN( "Exitting setHostInUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}
/***************************************************************
** FUNCTION: sip_getHeaderFromUrl
**
** DESCRIPTION: This function retrieves the pHeader field from a
**		SIP Url
**
***************************************************************/

SipBool sip_getHeaderFromUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit **ppHeader, SipError *pErr)
#else
	( pUrl, ppHeader, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit **ppHeader;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
#endif
	SIP_S8bit * pTemp_header;
	SIPDEBUGFN( "Entering getHostFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL || ppHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_header = ((SipUrl *) pUrl)->pHeader;

	if( pTemp_header == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	*ppHeader = pTemp_header;
#else
	dLength = sip_strlen(pTemp_header);
	*ppHeader = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
	if ( *ppHeader == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppHeader , pTemp_header );
#endif
	SIPDEBUGFN( "Exitting getHostFromUrl");


	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_setHeaderInUrl
**
** DESCRIPTION: This function sets the pHeader field in a SIP Url
**
***************************************************************/

SipBool sip_setHeaderInUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_S8bit *pHeader, SipError *pErr)
#else
	( pUrl, pHeader, pErr )
	  SipUrl *pUrl;
	  SIP_S8bit *pHeader;
	  SipError *pErr;
#endif
{

#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_header;
#endif
	SIPDEBUGFN( "Entering setHostInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pUrl->pHeader != SIP_NULL )
	{
		if ( sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&( pUrl->pHeader), pErr)\
																	== SipFail)
			return SipFail;
	}

#ifdef SIP_BY_REFERENCE
	pUrl->pHeader = pHeader;
#else
	if( pHeader == SIP_NULL)
		pTemp_header = SIP_NULL;
	else
	{
		dLength = sip_strlen( pHeader );
		pTemp_header = ( SIP_S8bit * )fast_memget\
											(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTemp_header== SIP_NULL )
			return SipFail;

		(void)sip_strcpy( pTemp_header, pHeader );

	}
	pUrl->pHeader = pTemp_header;
#endif
	SIPDEBUGFN( "Exitting setHostInUrl");


	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_getPortFromUrl
**
** DESCRIPTION: This function retrieves the dPort field from a SIp
**		Url
**
***************************************************************/

SipBool sip_getPortFromUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_U16bit *pPort, SipError *pErr)
#else
	( pUrl, pPort, pErr )
	  SipUrl *pUrl;
	  SIP_U16bit *pPort;
	  SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering getPortFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;

		return SipFail;
	}

	if ( pPort == SIP_NULL )
	{
		*pErr= E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pUrl->dPort==SIP_NULL)
	{
		 *pErr= E_NO_EXIST;
		 return SipFail;
	}
	*pPort= *(pUrl->dPort);

	SIPDEBUGFN( "Exitting getPortFromUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_setPortInUrl
**
** DESCRIPTION: This function sets the dPort field in a SIP Url
**
***************************************************************/

SipBool sip_setPortInUrl
#ifdef ANSI_PROTO
	( SipUrl *pUrl, SIP_U16bit dPort, SipError *pErr)
#else
	( pUrl, dPort, pErr )
	  SipUrl *pUrl;
	  SIP_U16bit dPort;
	  SipError *pErr;
#endif
{

	SIP_U16bit *temp_port;
	SIPDEBUGFN( "Entering setPortInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;


	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	temp_port = ( SIP_U16bit * )\
					fast_memget(ACCESSOR_MEM_ID,sizeof(SIP_U16bit),pErr);
        /* CSR_1-5210935 Change Start */
       /* *temp_port = dPort; */
        /* CSR_1-5210935 Change End */
	if ( temp_port== SIP_NULL )
		return SipFail;
        /* CSR_1-5210935 Change Start */
	*temp_port = dPort;
	/* CSR_1-5210935 Change End */ 

 	if ( pUrl->dPort != SIP_NULL )
	{
		if ( sip_memfree\
				(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pUrl->dPort), pErr) == SipFail)
			return SipFail;
	}
	pUrl->dPort = temp_port;

	SIPDEBUGFN( "Exitting setPortInUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***************************************************************
** FUNCTION: sip_getUrlParamCountFromUrl
**
** DESCRIPTION: This function retrieves the number of url-pParam
**		from a SIP Url
**
***************************************************************/

SipBool sip_getUrlParamCountFromUrl
#ifdef ANSI_PROTO
	( SipUrl	*pUrl,
	  SIP_U32bit	*pIndex,
	  SipError	*pErr  )
#else
	( pUrl,pIndex,pErr)
	  SipUrl 	*pUrl;
	  SIP_U32bit 	*pIndex;
	  SipError 	*pErr;
#endif
{
	SIPDEBUGFN( "Entering getUrlParamCountFromUrl");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if ( (pUrl == SIP_NULL) || ( pIndex == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pUrl->slParam), pIndex , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN( "Exitting getUrlParamCountFromUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************
** FUNCTION: sip_getUrlParamAtIndexFromUrl
**
** DESCRIPTION: This function retrieves a Url-param at a specified
**		index from a Sip Url
**
***************************************************************/

SipBool sip_getUrlParamAtIndexFromUrl
#ifdef ANSI_PROTO
#ifdef SIP_BY_REFERENCE
	( SipUrl 	*pUrl,
	  SipParam **ppParam,
	  SIP_U32bit   dIndex,
	  SipError     *pErr )
#else
	( SipUrl 	*pUrl,
	  SipParam *pParam,
	  SIP_U32bit   dIndex,
	  SipError     *pErr )
#endif
#else
#ifdef SIP_BY_REFERENCE
	( pUrl,ppParam,dIndex,pErr)
	  SipUrl 	 *pUrl;
	  SipParam  **ppParam;
	  SIP_U32bit 	dIndex;
	  SipError       *pErr;
#else
	( pUrl,pParam,dIndex,pErr)
	  SipUrl 	 *pUrl;
	  SipParam  *pParam;
	  SIP_U32bit 	dIndex;
	  SipError       *pErr;
#endif
#endif
{
	SIP_Pvoid 	pElement_from_list;
#ifdef SIP_BY_REFERENCE
    SipParam *  pUrl_param;
#endif

	SIPDEBUGFN( "Entering getUrlParamAtIndexFromUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
	{
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	if ( (pUrl == SIP_NULL) || (ppParam == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#else
	if ( (pUrl == SIP_NULL) || (pParam == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
#endif
	if( sip_listGetAt(&(pUrl->slParam), dIndex, &pElement_from_list, pErr)\
																	== SipFail)
		return SipFail;

	if ( pElement_from_list == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
    pUrl_param = (SipParam * )pElement_from_list;
	HSS_LOCKEDINCREF(pUrl_param->dRefCount);
	*ppParam = pUrl_param;
#else
	if ( __sip_cloneSipParam(pParam,(SipParam *)pElement_from_list,pErr)\
																	== SipFail)
	{
		sip_freeSipParam(pParam);
		return SipFail;
	}
#endif
	SIPDEBUGFN( "Exitting getUrlParamAtIndexFromUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/***************************************************************
** FUNCTION: sip_setUrlParamAtIndexInUrl
**
** DESCRIPTION: This function sets aurl-param at a specified index
**		in a Sip Url
**
***************************************************************/

SipBool sip_setUrlParamAtIndexInUrl
#ifdef ANSI_PROTO
	( SipUrl 	*pUrl,
	  SipParam	*pParam,
	  SIP_U32bit 	dIndex,
	  SipError 	*pErr )
#else
	( pUrl,pParam,dIndex,pErr)
	  SipUrl 	*pUrl;
	  SipParam	*pParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
{
	SipParam 	*pElement_in_list;

	SIPDEBUGFN( "Entering setUrlParamAtIndexInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if ( pParam == SIP_NULL )
		pElement_in_list = SIP_NULL;
	else
	{
#ifdef SIP_BY_REFERENCE
		HSS_LOCKEDINCREF(pParam->dRefCount);
		pElement_in_list = pParam;
#else
		(void)sip_initSipParam(&pElement_in_list,pErr);

		if ( __sip_cloneSipParam(pElement_in_list, pParam,pErr) == SipFail)
		{
			 sip_freeSipParam(pElement_in_list);
			return SipFail;
		}
#endif
	}


	if( sip_listSetAt(&(pUrl->slParam), dIndex, (SIP_Pvoid) pElement_in_list,\
														pErr) == SipFail)
	{
		if ( pElement_in_list != SIP_NULL )
		{
#ifdef SIP_BY_REFERENCE
                HSS_LOCKEDDECREF(pParam->dRefCount);
#else
			 sip_freeSipParam(pElement_in_list);
#endif
		}
		return SipFail;
	}
	SIPDEBUGFN( "Exitting setUrlParamAtIndexInUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***************************************************************
** FUNCTION: sip_insertUrlParamAtIndexInUrl
**
** DESCRIPTION: This function inserts a url-param at a specified
**		index in a Sip Url
**
***************************************************************/

SipBool sip_insertUrlParamAtIndexInUrl
#ifdef ANSI_PROTO
	( SipUrl 	*pUrl,
	  SipParam	*pParam,
	  SIP_U32bit 	dIndex,
	  SipError 	*pErr )
#else
	( pUrl,pParam,dIndex,pErr)
	  SipUrl 	*pUrl;
	  SipParam	*pParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
{
	SipParam 	*pElement_in_list;

	SIPDEBUGFN( "Entering InsertUrlParamAtIndexInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying the param structure */
	if ( pParam == SIP_NULL )
		pElement_in_list = SIP_NULL;
	else
	{
#ifdef SIP_BY_REFERENCE
		HSS_LOCKEDINCREF(pParam->dRefCount);
		pElement_in_list = pParam;
#else
		(void)sip_initSipParam(&pElement_in_list, pErr);

		if ( __sip_cloneSipParam(pElement_in_list, pParam,pErr) == SipFail)
		{
			 sip_freeSipParam(pElement_in_list);
			return SipFail;
		}
#endif
	}

	if( sip_listInsertAt(&(pUrl->slParam), dIndex, pElement_in_list, pErr)\
																	== SipFail)
	{
		if ( pElement_in_list != SIP_NULL )
#ifdef SIP_BY_REFERENCE
            HSS_LOCKEDDECREF(pParam->dRefCount);
#else
			 sip_freeSipParam(pElement_in_list);
#endif
		return SipFail;
	}

	SIPDEBUGFN( "Exitting InsertUrlParamAtIndexInUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***************************************************************
** FUNCTION: sip_deleteUrlParamAtIndexInUrl
**
** DESCRIPTION: This function deletes a url-param at a specified
**		index in a Sip Url
**
***************************************************************/

SipBool sip_deleteUrlParamAtIndexInUrl
#ifdef ANSI_PROTO
	( SipUrl 	*pUrl,
	  SIP_U32bit dIndex,
	  SipError 	*pErr )
#else
	( pUrl,dIndex,pErr)
	  SipUrl 	*pUrl;
	  SIP_U32bit dIndex;
	  SipError 	*pErr;
#endif
{
	SIPDEBUGFN( "Entering deleteUrlParamAtIndexInUrl");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pUrl == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pUrl->slParam), dIndex, pErr) == SipFail)
		return SipFail;


	SIPDEBUGFN( "Exitting deleteUrlParamAtIndexInUrl");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


#ifdef SIP_DATE
/*TBD*/

/***************************************************************
** FUNCTION: sip_getYearFromDateFormat
**
** DESCRIPTION: This function retrieves the dYear from a SIP dDate
**		structure
**
***************************************************************/

SipBool sip_getYearFromDateFormat
	(SipDateFormat *pHdr,
	SIP_U16bit *pYear,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering getYearFromDateFormat");
#ifndef SIP_NO_CHECK
	if(pErr == SIP_NULL)
		return SipFail;
	if ( pHdr == SIP_NULL|| pYear == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	*pYear=pHdr->dYear;
	SIPDEBUGFN( "Exitting getYearFromDateFormat");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}

/***************************************************************
** FUNCTION: sip_setYearInDateFormat
**
** DESCRIPTION: This function sets the dYear field in a SIP Date
**		structure
**
***************************************************************/

SipBool sip_setYearInDateFormat
	(SipDateFormat *pHdr,
	SIP_U16bit dYear,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering setYearInDateFormat");
#ifndef SIP_NO_CHECK
	if(pErr == SIP_NULL)
		return SipFail;

	if ( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if(dYear>9999)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pHdr->dYear=dYear;
	SIPDEBUGFN( "Exitting setYearInDateFormat");
	*pErr=E_NO_ERROR;
	return SipSuccess;
}


/***************************************************************
** FUNCTION: sip_setSecInTimeFormat
**
** DESCRIPTION: This function sets the seconds in a SIP Time structure
**
***************************************************************/

SipBool sip_setSecInTimeFormat
	(SipTimeFormat *pHdr,
	SIP_S8bit dSec,
	SipError *pErr)
{
	SIPDEBUGFN( "Entering setSecInTimeFormat");
#ifndef SIP_NO_CHECK
	if(pErr == SIP_NULL)
		return SipFail;

	if ( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if(((unsigned)(int)dSec)>59)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pHdr->dSec=dSec;
	SIPDEBUGFN( "Exitting setSecInTimeFormat");
	*pErr=E_NO_ERROR;


	return SipSuccess;
}




/*****************************************************************
**
** FUNCTION:  sip_setTimeFormatInDateStruct
**
** DESCRIPTION: This function sets the slTime in a SIP Date structure
**
***************************************************************/
SipBool sip_setTimeFormatInDateStruct
	(SipDateStruct *dstruct, SipTimeFormat *slTime, SipError *err)
{
	SipTimeFormat *time_format;
	SIPDEBUGFN("Entering function sip_setTimeFormatInDateStruct");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( dstruct == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( validateSipTimeFormat(slTime, err ) == SipFail)
		return SipFail;

 	time_format = dstruct->pTime;
	if (time_format != SIP_NULL)
		sip_freeSipTimeFormat(time_format);
		HSS_LOCKEDINCREF(slTime->dRefCount);
		dstruct->pTime = slTime;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setTimeFormatInDateStruct");
 	return SipSuccess;
}

/*********************************************************
** FUNCTION:validateMonth
**
** DESCRIPTION:  This fucntion returns SipSuccess if
** "htype" is one among the defined en_Month types else it
** returns SipFail.
**
**********************************************************/
SipBool validateMonth
	(en_Month *htype,
	SipError *err)
{
	switch(*htype)
	{
	case SipMonthJan:
	case SipMonthFeb:
	case SipMonthMar:
	case SipMonthApr:
	case SipMonthMay:
	case SipMonthJun:
	case SipMonthJul:
	case SipMonthAug:
	case SipMonthSep:
	case SipMonthOct:
	case SipMonthNov:
	case SipMonthDec:
			*err=E_NO_ERROR;
			return SipSuccess;
		default:
			*err = E_INV_PARAM;
			return SipFail;

	}
}


/********************************************************************
**
** FUNCTION:  validateSipDayOfWeek
**
** DESCRIPTION:   This fucntion returns SipSuccess if "dDay" is
** one among the defined en_DayOfWeek types; else it returns SipFail.
**
********************************************************************/

SipBool validateSipDayOfWeek
	(en_DayOfWeek dDay, SipError *err)
{
	SIPDEBUGFN("Entering function validateSipDayOfweek");
	switch (dDay)
	{
		case 	SipDaySun	:
		case	SipDayMon	:
		case	SipDayTue	:
		case	SipDayWed	:
		case	SipDayThu	:
		case	SipDayFri	:
		case	SipDaySat	:
		case	SipDayNone	:break;
		default			:*err = E_INV_PARAM;
					 return SipFail;

	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipDayOfweek");
	return SipSuccess;

}


/********************************************************************
**
** FUNCTION:  validateSipTimeFormat
**
**DESCRIPTION:  This function  validates the pValue of "time_format".
** It returns SipSuccess if dHour is less than 24, minute is less
** than 59 and second is lesss than 59; else it returns SipFail;
**
********************************************************************/

SipBool validateSipTimeFormat
	(SipTimeFormat *time_format, SipError *err)
{
	SIPDEBUGFN("Entering function validateSipTimeFormat");
	if (time_format->dHour >23)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (time_format->dMin >59)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (time_format->dSec >59)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipTimeFormat");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  validateSipDateFormat
**
** DESCRIPTION:  This function validates the values of the
** "date_format" structure fileds.
**
********************************************************************/

SipBool validateSipDateFormat
	(SipDateFormat *date_format, SipError *err)
{
	SIP_S8bit days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	SIPDEBUGFN("Entering function validateSipDateFormat");
	if (date_format->dYear > 9999)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if ( (date_format->dYear)%4 != 0)
		days_in_month[1] = 28;
	else if ( (date_format->dYear)%400 == 0)
		days_in_month[1] = 29;
	else if ( (date_format->dYear)%100 == 0)
		days_in_month[1] = 28;
	else
		days_in_month[1] = 29;

	switch(date_format->dMonth)
	{
		case	SipMonthJan	:
		case	SipMonthFeb	:
		case	SipMonthMar	:
		case 	SipMonthApr	:
		case	SipMonthMay	:
		case	SipMonthJun	:
		case	SipMonthJul	:
		case	SipMonthAug	:
		case	SipMonthSep	:
		case	SipMonthOct	:
		case	SipMonthNov	:
		case	SipMonthDec	:break;
		default			:*err = E_INV_PARAM;
					 return SipFail;

	}
	if (date_format->dDay > days_in_month[date_format->dMonth])
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipDateFormat");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  validateSipDateStruct
**
** DESCRIPTION:  This function validates the field values of 'date_struct"
** structure.
**
********************************************************************/

SipBool validateSipDateStruct
	(SipDateStruct *date_struct, SipError *err)
{
	SIPDEBUGFN("Entering function validateDateStruct");
	switch (date_struct->dDow)
	{
		case	SipDaySun	:
		case	SipDayMon	:
		case	SipDayTue	:
		case	SipDayWed	:
		case	SipDayThu	:
		case	SipDayFri	:
		case	SipDaySat	:
		case	SipDayNone	:break;
		default			:*err = E_INV_PARAM;
					 return SipFail;

	}
	if ( validateSipDateFormat((date_struct->pDate), err) == SipFail )
		return SipFail;
	if ( validateSipTimeFormat((date_struct->pTime), err) == SipFail )
		return SipFail;

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateDateStruct");
	return SipSuccess;
}

#endif /* SIP_DATE */


#ifdef SIP_CORE_STACK_COMPATIBLE
/*#ifdef STK_RELEASE*/
/********************************************************************
** FUNCTION:sip_getNameFromNameValuePair
**
** DESCRIPTION: This Function gets the Name from NamevaluePair
**
**
**********************************************************************/
SipBool sip_getNameFromNameValuePair
#ifdef ANSI_PROTO
	( SipNameValuePair *pNameValue, SIP_S8bit **pName, SipError *err)
#else
	( pNameValue, pName, err )
	  SipNameValuePair *pNameValue;
	  SIP_S8bit **pName;
	  SipError *err;
#endif
{
/*#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;*/
/*#endif*/
	SIP_S8bit *temp_name;
	SIPDEBUGFN("Entering sip_getNameFromNameValuePair\n");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pName == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( pNameValue == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	temp_name = pNameValue->pName;

	if( temp_name == SIP_NULL )
	{
		*err = E_NO_EXIST;
		return SipFail;
	}
	*pName = temp_name;

	SIPDEBUGFN("Exitting sip_getNameFromNameValuePair\n");

	*err = E_NO_ERROR;
	return SipSuccess;

}
/*#endif*/

/********************************************************************
** FUNCTION:sip_setNameInNameValuePair
**
** DESCRIPTION: This Function sets the Name in NamevaluePair
**
**
**********************************************************************/

SipBool sip_setNameInNameValuePair
#ifdef ANSI_PROTO
	( SipNameValuePair *pNameValue, SIP_S8bit *pName, SipError *err)
#else
	( pNameValue, pName, err )
	  SipNameValuePair *pNameValue;
	  SIP_S8bit *pName;
	  SipError *err;
#endif
{
	SIP_S8bit *temp_name;
	SIPDEBUGFN("Entering sip_setNameInNameValuePair\n");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pNameValue == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( pName == SIP_NULL)
		temp_name = SIP_NULL;
	else
	{
		temp_name = pName;

	}

	if ( pNameValue->pName != SIP_NULL )
	{
		if ( fast_memfree(ACCESSOR_MEM_ID, pNameValue->pName, err) == SipFail)
			return SipFail;
	}
        pNameValue->pName = temp_name;
	SIPDEBUGFN("Exiting sip_setNameInNameValuePair\n");

	*err = E_NO_ERROR;
	return SipSuccess;

}

/********************************************************************
** FUNCTION:sip_getValueFromNameValuePair
**
** DESCRIPTION: This Function gets the value from NamevaluePair
**
**
**********************************************************************/
SipBool sip_getValueFromNameValuePair
#ifdef ANSI_PROTO
	( SipNameValuePair *pNameValue, SIP_S8bit **pValue, SipError *err)
#else
	( pNameValue, pValue, err )
	  SipNameValuePair *pNameValue;
	  SIP_S8bit **pValue;
	  SipError *err;
#endif
{
	SIP_S8bit *temp_value;
	SIPDEBUGFN("Entering sip_getValueFromNameValuePair\n");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pValue == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( pNameValue == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	temp_value = pNameValue->pValue;

	if( temp_value == SIP_NULL )
	{
		*err = E_NO_EXIST;
		return SipFail;
	}
	*pValue = temp_value;

	SIPDEBUGFN("Exitting sip_getValueFromNameValuePair\n");

	*err = E_NO_ERROR;
	return SipSuccess;

}

/********************************************************************
** FUNCTION:sip_setValueInNameValuePair
**
** DESCRIPTION: This Function sets the value in NamevaluePair
**
**
**********************************************************************/

SipBool sip_setValueInNameValuePair
#ifdef ANSI_PROTO
	( SipNameValuePair *pNameValue, SIP_S8bit *pValue, SipError *err)
#else
	( pNameValue, pValue, err )
	  SipNameValuePair *pNameValue;
	  SIP_S8bit *pValue;
	  SipError *err;
#endif
{
	SIP_S8bit *temp_value;
	SIPDEBUGFN("Entering sip_setValueInNameValuePair\n");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pNameValue == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( pValue == SIP_NULL)
		temp_value = SIP_NULL;
	else
	{
		temp_value = pValue;

	}

	if ( pNameValue->pValue != SIP_NULL )
	{
		if ( fast_memfree(ACCESSOR_MEM_ID, pNameValue->pValue, err) == SipFail)
			return SipFail;
	}
        pNameValue->pValue = temp_value;
	SIPDEBUGFN("Exiting sip_setValueInNameValuePair\n");

	*err = E_NO_ERROR;
	return SipSuccess;

}
#endif /* SIP_CORE_STACK_COMPATIBLE */


/********************************************************************
**
** FUNCTION:  validateSipHeaderType
**
** DESCRIPTION:  This fucntion returns SipSuccess if "dType" is one
** among the defined en_HeaderType's else it returns SipFail.
**
********************************************************************/

SipBool validateSipHeaderType
	(en_HeaderType dType, SipError *err)
{
	SIPDEBUGFN("Entering function validateSipHeaderType");
	switch(dType)
	{
		case	SipHdrTypeCallId		:
		case	SipHdrTypeCseq			:
 #ifdef SIP_PEMEDIA		
		case	SipHdrTypePEarlyMedia		:
#endif
#ifdef SIP_DATE
		case	SipHdrTypeExpiresDate		:
		case	SipHdrTypeExpiresSec		:
		case	SipHdrTypeExpiresAny		:
#else
		case	SipHdrTypeExpires			:
#endif
		case	SipHdrTypeMinExpires		:
		case	SipHdrTypeAccept			:
		case	SipHdrTypeFrom			:
		case	SipHdrTypeRecordRoute		:
#ifdef SIP_TIMESTAMP			
		case	SipHdrTypeTimestamp		:
#endif		
		case	SipHdrTypeTo			:
		case	SipHdrTypeVia			:
		case	SipHdrTypeContentEncoding	:
		case	SipHdrTypeContentLength		:
		case	SipHdrTypeContentType		:
#ifdef SIP_AUTHENTICATE		
		case	SipHdrTypeAuthorization		:
#endif		
		case	SipHdrTypeContactNormal		:
		case	SipHdrTypeContactWildCard	:
		case	SipHdrTypeContactAny		:
		case	SipHdrTypeMaxforwards		:
#ifdef SIP_AUTHENTICATE		
		case	SipHdrTypeProxyauthorization		:
#endif		
		case	SipHdrTypeRoute			:
		case	SipHdrTypeRequire		:
		case	SipHdrTypeAllow			:
#ifdef SIP_AUTHENTICATE		
		case	SipHdrTypeProxyAuthenticate	:
#endif		
#ifdef SIP_DATE
		case	SipHdrTypeRetryAfterDate	:
		case	SipHdrTypeRetryAfterSec		:
		case	SipHdrTypeRetryAfterAny		:
#else
		case	SipHdrTypeRetryAfter		:
#endif
		case	SipHdrTypeUnsupported		:
#ifdef SIP_WARNING		
		case	SipHdrTypeWarning		:
#endif		
#ifdef SIP_AUTHENTICATE		
		case	SipHdrTypeWwwAuthenticate	:
#endif		
		case	SipHdrTypeUnknown		:
#ifdef SIP_RPR		
		case	SipHdrTypeRSeq			: /* Retrans */
		case	SipHdrTypeRAck			: /* Retrans */
#endif		
		case 	SipHdrTypeSupported		: /* RPR Retrans*/
		case 	SipHdrTypeMimeVersion		: /* bcpt ext */
		case	SipHdrTypeContentDisposition:
#ifdef SIP_REFER
		case 	SipHdrTypeReferTo			:
		case 	SipHdrTypeReferredBy		:
#endif
#ifdef SIP_REPLACES
		case 	SipHdrTypeReplaces		:
#endif
#ifdef SIP_3GPP
		case SipHdrTypePath:
		case SipHdrTypePAssociatedUri: 
    case SipHdrTypePCalledPartyId: 
    case SipHdrTypePVisitedNetworkId: 
    case SipHdrTypePcfAddr: 
 		case SipHdrTypePanInfo:   
    case SipHdrTypePcVector:  
		case SipHdrTypeServiceRoute:
		case SipHdrTypeHistoryInfo:
		case SipHdrTypeRequestDisposition: 
		case SipHdrTypeAcceptContact: 
		case SipHdrTypeRejectContact: 
		case SipHdrTypeJoin: 
		case SipHdrTypeIfMatch:
		case SipHdrTypeETag:
#endif
#ifdef SIP_IMPP
		case	SipHdrTypeEvent				:
		case	SipHdrTypeAllowEvents		:
		case	SipHdrTypeSubscriptionState	:
#endif
#ifdef SIP_SESSIONTIMER
		case 	SipHdrTypeMinSE			:
		case	SipHdrTypeSessionExpires:
#endif
#ifdef SIP_SECURITY
		case	SipHdrTypeSecurityClient	:
		case	SipHdrTypeSecurityVerify	:
		case	SipHdrTypeSecurityServer	:
#endif
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		case SipHdrTypePPreferredId:
		case SipHdrTypePAssertId:
#endif				
				break;
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
		case SipHdrTypeAcceptRsrcPriority:
#endif
		default		:
				*err = E_INV_TYPE;
				return SipFail;

	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting functoion validateSipHeaderType");
	return SipSuccess;
}


/******************************************************************
**
** FUNCTION:  sip_getTypeFromString
**
** DESCRIPTION: This function retrieves the pHeader dType from a pHeader
**		represented as string
**
******************************************************************/
SipBool sip_getTypeFromString
	 ( SIP_S8bit *str1,
	   en_HeaderType *dType,
	   SipError *err)
{
	SIP_S8bit * hdr_name, old;
	SIP_S8bit * temp_str, *str;
	SIP_U32bit dLength, i, j;
	SipBool		ret;
	/*SipError dErr;*/
    SIP_U32bit string_length;

	/*dErr = E_NO_ERROR;*/

	dLength = sip_strlen ( str1 );
	if ( dLength == 0 )
	{
		*err = E_INV_TYPE;
		return SipFail;
	}

	for ( i = 0; i < dLength ; i++ )
		if ( str1[i] == ':' )
			break;

	if ( i >= dLength )
	{
		*err = E_INV_TYPE;
		return SipFail;
	}

	str = (SIP_S8bit *) STRDUPACCESSOR( str1 );
	if ( str == SIP_NULL)
	{
		*err = E_NO_MEM;
		return SipFail;
	}
	old = str[i+1];
	str[i+1] = '\0';

	temp_str = ( SIP_S8bit *) STRDUPACCESSOR(str);
	if ( temp_str == SIP_NULL )
	{
		*err = E_NO_MEM;
                sip_freeString (str);
		return SipFail;
	}

	str[i+1] = old;

	hdr_name = ( SIP_S8bit *) fast_memget(ACCESSOR_MEM_ID,  i + 2, err );
	if ( hdr_name == SIP_NULL)
        {
                sip_freeString (str);
                sip_freeString (temp_str);
		return SipFail;
        }
        string_length = sip_strlen(temp_str);
	for(i=0,j=0;i<string_length;i++)
		if((temp_str[i]==' ')||(temp_str[i]=='\t')||(temp_str[i]=='\n')\
			||(temp_str[i]=='\r'))
	{
	}
	else
	{
		hdr_name[j] = temp_str[i];
		j++;
	}
	hdr_name[j]='\0';

	*err = E_NO_ERROR;
	ret = SipFail;

	/* comparision starts here to find dType */
	if ((sip_strcasecmp((SIP_S8bit *)"Call-id:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"i:",hdr_name)==0))
	{
		*dType =SipHdrTypeCallId;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"CSeq:",hdr_name)==0)
	{
		*dType =SipHdrTypeCseq;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Content-Disposition:", hdr_name)==0)
	{
		*dType =SipHdrTypeContentDisposition;
		ret = SipSuccess;
	}
#ifdef SIP_REPLACES
	else if(sip_strcasecmp((SIP_S8bit *)"Replaces:",hdr_name)==0)
	{
		*dType =SipHdrTypeReplaces;
		ret = SipSuccess;
	}
#endif
	else if(sip_strcasecmp((SIP_S8bit *)"Expires:",hdr_name)==0)
	{
#ifdef SIP_DATE
		*dType =SipHdrTypeExpiresAny;
#else
		*dType =SipHdrTypeExpires;
#endif
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Min-Expires:", hdr_name)==0)
	{
		*dType =SipHdrTypeMinExpires;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Accept:", hdr_name)==0)
	{
		*dType =SipHdrTypeAccept;
		ret = SipSuccess;
	}
#ifdef SIP_SESSIONTIMER
	else if ( sip_strcasecmp((SIP_S8bit *)"Min-SE:",hdr_name) == 0 )
	{
		*dType =SipHdrTypeMinSE;
		ret = SipSuccess;
	}
	else if((sip_strcasecmp((SIP_S8bit *)"Session-Expires:",hdr_name)==0) ||
	(sip_strcasecmp((SIP_S8bit *)"x:",hdr_name)==0))

	{
		*dType =SipHdrTypeSessionExpires;
		ret = SipSuccess;
	}
#endif
#ifdef SIP_PEMEDIA
	else if(sip_strcasecmp((SIP_S8bit *)"P-Early-Media:", hdr_name)==0)
	{
		*dType =SipHdrTypePEarlyMedia;
		ret = SipSuccess;
	}
#endif	
	else if((sip_strcasecmp((SIP_S8bit *)"From:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"f:",hdr_name)==0))
	{
		*dType =SipHdrTypeFrom;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Record-Route:",hdr_name)==0)
	{
		*dType =SipHdrTypeRecordRoute;
		ret = SipSuccess;
	}
#ifdef SIP_TIMESTAMP			
	else if(sip_strcasecmp((SIP_S8bit *)"Timestamp:",hdr_name)==0)
	{
		*dType =SipHdrTypeTimestamp;
		ret = SipSuccess;
	}
#endif	
	else if((sip_strcasecmp((SIP_S8bit *)"To:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"t:",hdr_name)==0))
	{
		*dType =SipHdrTypeTo;
		ret = SipSuccess;
	}
	else if((sip_strcasecmp((SIP_S8bit *)"Via:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"v:",hdr_name)==0))
	{
		*dType =SipHdrTypeVia;
		ret = SipSuccess;
	}
	else if((sip_strcasecmp((SIP_S8bit *)"Content-Encoding:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"e:",hdr_name)==0))
	{
		*dType =SipHdrTypeContentEncoding;
		ret = SipSuccess;
	}
	/* RPR */
	else if(sip_strcasecmp((SIP_S8bit *)"Supported:",hdr_name)==0)
	{
		*dType =SipHdrTypeSupported;
		ret = SipSuccess;
	}
 	else if((sip_strcasecmp((SIP_S8bit *)"Content-Length:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"l:",hdr_name)==0))
	{
		*dType =SipHdrTypeContentLength;
		ret = SipSuccess;
	}
	else if((sip_strcasecmp((SIP_S8bit *)"Content-Type:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"c:",hdr_name)==0))
	{
		*dType =SipHdrTypeContentType;
		ret = SipSuccess;
	}
	/* bcpt ext */
	else if(sip_strcasecmp((SIP_S8bit *)"Mime-Version:",hdr_name)==0)
	{
		*dType =SipHdrTypeMimeVersion;
		ret = SipSuccess;
	}
	/* bcpt ext ends */
	/* Retrans ext */
#ifdef SIP_RPR	
	else if(sip_strcasecmp((SIP_S8bit *)"RSeq:",hdr_name)==0)
	{
		*dType =SipHdrTypeRSeq;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"RAck:",hdr_name)==0)
	{
		*dType =SipHdrTypeRAck;
		ret = SipSuccess;
	}
#endif
#ifdef SIP_SECURITY
	else if(sip_strcasecmp((SIP_S8bit *)"Security-Client:",hdr_name)==0)
	{
		*dType =SipHdrTypeSecurityClient;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Security-Verify:",hdr_name)==0)
	{
		*dType =SipHdrTypeSecurityVerify;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Security-Server:",hdr_name)==0)
	{
		*dType =SipHdrTypeSecurityServer;
		ret = SipSuccess;
	}
#endif    
#ifdef SIP_3GPP
	else if(sip_strcasecmp((SIP_S8bit *)"Path:",hdr_name)==0)
	{
	
		*dType =SipHdrTypePath;
		ret = SipSuccess;
	}	
	else if(sip_strcasecmp((SIP_S8bit *)"P-Associated-URI:",hdr_name)==0)
	{		
		*dType =SipHdrTypePAssociatedUri;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Called-Party-ID:",hdr_name)==0)
	{		
		*dType =SipHdrTypePCalledPartyId;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Visited-Network-ID:",hdr_name)==0)
	{		
		*dType =SipHdrTypePVisitedNetworkId;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Charging-Function-Addresses:",hdr_name)==0)
	{		
		*dType =SipHdrTypePcfAddr;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Access-Network-Info:",hdr_name)==0)
	{		
		*dType =SipHdrTypePanInfo;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Charging-Vector:",hdr_name)==0)
	{		
		*dType =SipHdrTypePcVector;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Service-Route:",hdr_name)==0)
	{		
		*dType =SipHdrTypeServiceRoute;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"History-Info:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeHistoryInfo;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Request-Disposition:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeRequestDisposition;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Accept-Contact:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeAcceptContact;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Reject-Contact:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeRejectContact;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Join:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeJoin;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"SIP-If-Match:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeIfMatch;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"SIP-ETag:",hdr_name)==0)
	{
	
		*dType =SipHdrTypeETag;
		ret = SipSuccess;
	}
#endif
#ifdef SIP_AUTHENTICATE	
	else if(sip_strcasecmp((SIP_S8bit *)"Authorization:",hdr_name)==0)
	{
		*dType =SipHdrTypeAuthorization;
		ret = SipSuccess;
	}
#endif	
	else if((sip_strcasecmp((SIP_S8bit *)"Contact:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"m:",hdr_name)\
		==0))
	{
		*dType =SipHdrTypeContactAny;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Max-Forwards:",hdr_name)==0)
	{
		*dType =SipHdrTypeMaxforwards;
		ret = SipSuccess;
	}
#ifdef SIP_PRIVACY
	else if(sip_strcasecmp((SIP_S8bit *)"Privacy:",hdr_name)==0)
	{
		*dType =SipHdrTypePrivacy;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"P-Preferred-Identity:",hdr_name)==0)
	{
		*dType =SipHdrTypePPreferredId;
		ret = SipSuccess;
	}
	
  else if(sip_strcasecmp((SIP_S8bit *)"P-Asserted-Identity:",hdr_name)==0)
	{
		*dType =SipHdrTypePAssertId;
		ret = SipSuccess;
	}
	
#endif	
#ifdef SIP_AUTHENTICATE	
	else if(sip_strcasecmp((SIP_S8bit *)"Proxy-Authorization:",hdr_name)==0)
	{
		*dType =SipHdrTypeProxyauthorization;
		ret = SipSuccess;
	}
#endif	
	else if(sip_strcasecmp((SIP_S8bit *)"Route:",hdr_name)==0)
	{
		*dType =SipHdrTypeRoute;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Require:",hdr_name)==0)
	{
		*dType =SipHdrTypeRequire;
		ret = SipSuccess;
	}
#ifdef SIP_REFER
	else if((sip_strcasecmp((SIP_S8bit *)"Refer-To:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"r:",hdr_name)==0))
	{
		*dType =SipHdrTypeReferTo;
		ret = SipSuccess;
	}
	else if((sip_strcasecmp((SIP_S8bit *)"Referred-By:",hdr_name)==0)||\
		(sip_strcasecmp((SIP_S8bit *)"b:",hdr_name)==0))
	{
		*dType =SipHdrTypeReferredBy;
		ret = SipSuccess;
	}
#endif /* SIP_REFER */
	else if(sip_strcasecmp((SIP_S8bit *)"Allow:",hdr_name)==0)
	{
		*dType =SipHdrTypeAllow;
		ret = SipSuccess;
	}
#ifdef SIP_AUTHENTICATE	
	else if(sip_strcasecmp((SIP_S8bit *)"Proxy-Authenticate:",hdr_name)==0)
	{
		*dType =SipHdrTypeProxyAuthenticate;
		ret = SipSuccess;
	}
#endif	
	else if(sip_strcasecmp((SIP_S8bit *)"Retry-After:",hdr_name)==0)
	{
#ifdef SIP_DATE
		*dType =SipHdrTypeRetryAfterAny;
#else
		*dType =SipHdrTypeRetryAfter;
#endif /* SIP_DATE */
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Unsupported:",hdr_name)==0)
	{
		*dType =SipHdrTypeUnsupported;
		ret = SipSuccess;
	}
#ifdef SIP_WARNING	
	else if(sip_strcasecmp((SIP_S8bit *)"Warning:",hdr_name)==0)
	{
		*dType =SipHdrTypeWarning;
		ret = SipSuccess;
	}
#endif	
#ifdef SIP_AUTHENTICATE	
	else if(sip_strcasecmp((SIP_S8bit *)"WWW-Authenticate:",hdr_name)==0)
	{
		*dType =SipHdrTypeWwwAuthenticate;
		ret = SipSuccess;
	}
#endif	
#ifdef SIP_IMPP
	else if ( (sip_strcasecmp((SIP_S8bit *)"Event:",hdr_name)==0) || \
			(sip_strcasecmp((SIP_S8bit *)"o",hdr_name)==0))
	{
		*dType =SipHdrTypeEvent;
		ret = SipSuccess;
	}
	else if ((sip_strcasecmp((SIP_S8bit *)"Allow-Events:",hdr_name)==0) ||\
		      (sip_strcasecmp((SIP_S8bit *)"u",hdr_name)==0))
	{
		*dType =SipHdrTypeAllowEvents;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Subscription-State:",hdr_name)==0)
	{
		*dType =SipHdrTypeSubscriptionState;
		ret = SipSuccess;
	}
#endif
#ifdef SIP_CONGEST
	else if(sip_strcasecmp((SIP_S8bit *)"Resource-Priority:",hdr_name)==0)
	{
		*dType =SipHdrTypeRsrcPriority;
		ret = SipSuccess;
	}
	else if(sip_strcasecmp((SIP_S8bit *)"Accept-Resource-Priority:",hdr_name)==0)
	{
		*dType =SipHdrTypeAcceptRsrcPriority;
		ret = SipSuccess;
	}
#endif	
	if (ret == SipFail)
	{
		*dType =  SipHdrTypeUnknown ;
		ret = SipSuccess;
	}

	sip_freeString( str);
	sip_freeString( temp_str);
	sip_freeString( hdr_name );
	return ret;

}

/********************************************************************
**
** FUNCTION:  sip_getHeaderLineCount
**
** DESCRIPTION: This function returns the number of distinct pHeader 
**				lines in the SipMessage "msg".
**
*********************************************************************/
SipBool sip_getHeaderLineCount 
	(SipMessage *msg, SIP_U32bit *count, SipError *err)
{
	SIPDEBUGFN("Entering sip_getHeaderLineCount");
#ifndef SIP_NO_CHECK
	if (err == SIP_NULL)
		return SipFail;
	if ((msg == SIP_NULL)||(count==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	if (sip_listSizeOf (&(msg->slOrderInfo), count, err) == SipFail)
		return SipFail;
	
	*err = E_INV_PARAM;
	SIPDEBUGFN("Exitting sip_getHeaderLineCount");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sip_validateHeaderString
**
** DESCRIPTION: This function returns SipSuccess if the
** string "str1" is having a correct pFormat for any of the headers.
**
********************************************************************/

SipBool sip_validateHeaderString
	 ( SIP_S8bit *str1,
	   en_HeaderType dType,
	   SipError *err)
{
	SipBool	ret;
	SIP_S8bit *hdr_name, old;
	SIP_S8bit *temp_str, *str;
	SIP_U32bit dLength, i, j;
        SIP_U32bit string_length;

	dLength = sip_strlen ( str1 );
	if ( dLength == 0 )
	{
		*err = E_INV_TYPE;
		return SipFail;
	}

	/* traversing hte given string till ':' is encountered */
	for ( i = 0; i < dLength ; i++ )
		if ( str1[i] == ':' )
			break;

	/* No ':' is present in the string and therfore E_INV_TYPE */
	if ( i >= dLength )
	{
		*err = E_INV_TYPE;
		return SipFail;
	}

	/* duplicating the input string so that modifications made on it is not reflected outside */
	str = (SIP_S8bit *)STRDUPACCESSOR(str1);
	if ( str == SIP_NULL)
	{
		*err  = E_NO_MEM;
		return SipFail;
	}

	old = str[i+1];
	str[i+1] = '\0';

	temp_str = ( SIP_S8bit *) STRDUPACCESSOR(str);
	if ( temp_str == SIP_NULL )
	{
		*err = E_NO_MEM;
                sip_freeString ( str);
		return SipFail;
	}

	str[i+1] = old;

	hdr_name = ( SIP_S8bit *) fast_memget(ACCESSOR_MEM_ID,  i + 2, err );
	if ( hdr_name == SIP_NULL)
        {
            sip_freeString ( str);
            sip_freeString ( temp_str);
	    return SipFail;
        }
        
        string_length=sip_strlen(temp_str);
	for(i=0,j=0;i<string_length;i++)
		if((temp_str[i]==' ')||(temp_str[i]=='\t')||(temp_str[i]=='\n')||(temp_str[i]=='\r'))
	{
	}
	else
	{
		hdr_name[j] = temp_str[i];
		j++;
	}
	hdr_name[j]='\0';

	*err = E_NO_ERROR;


	switch ( dType )
	{
		case SipHdrTypeCallId :
				if((sip_strcasecmp((SIP_S8bit *)"Call-id:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"i:",hdr_name)==0))

					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
#ifdef SIP_PEMEDIA
        case SipHdrTypePEarlyMedia :
                if((sip_strcasecmp((SIP_S8bit *)"P-Early-Media:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"P-Early-Media:",hdr_name)==0))
                    ret = SipSuccess;
                else
                {
                    *err = E_INV_TYPE;
                    ret = SipFail;
                }

                break;
#endif
		case SipHdrTypeCseq :
				if(sip_strcasecmp((SIP_S8bit *)"CSeq:",hdr_name)==0)

					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_DATE
		case SipHdrTypeExpiresDate :
		case SipHdrTypeExpiresSec :
		case SipHdrTypeExpiresAny :
#else
		case SipHdrTypeExpires:
#endif
				if(sip_strcasecmp((SIP_S8bit *)"Expires:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeFrom :
				if((sip_strcasecmp((SIP_S8bit *)"From:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"f:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_REPLACES
		case SipHdrTypeReplaces:
				if((sip_strcasecmp((SIP_S8bit *)"Replaces:",hdr_name)==0))
									ret = SipSuccess;
							else
							{
									*err = E_INV_TYPE;
									ret = SipFail;
							}
							break;
#endif
		case SipHdrTypeMinExpires:
				if((sip_strcasecmp((SIP_S8bit *)"Min-Expires:",hdr_name)==0))
									ret = SipSuccess;
				else
				{
									*err = E_INV_TYPE;
									ret = SipFail;
				}
				break;
		case SipHdrTypeAccept:
				if((sip_strcasecmp((SIP_S8bit *)"Accept:",hdr_name)==0))
									ret = SipSuccess;
				else
				{
									*err = E_INV_TYPE;
									ret = SipFail;
				}
				break;
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
				if((sip_strcasecmp((SIP_S8bit *)"Privacy:",hdr_name)==0))
									ret = SipSuccess;
				else
				{
									*err = E_INV_TYPE;
									ret = SipFail;
				}
				break;
		case SipHdrTypePPreferredId:
				if((sip_strcasecmp((SIP_S8bit *)"P-Preferred-Identity:",hdr_name)==0))
									ret = SipSuccess;
				else
				{
									*err = E_INV_TYPE;
									ret = SipFail;
				}
				break;

		case SipHdrTypePAssertId:
				if((sip_strcasecmp((SIP_S8bit *)"P-Asserted-Identity:",hdr_name)==0))
									ret = SipSuccess;
				else
				{
									*err = E_INV_TYPE;
									ret = SipFail;
				}
				break;

#endif				
#ifdef SIP_REFER
		case SipHdrTypeReferTo :
				if((sip_strcasecmp((SIP_S8bit *)"Refer-To:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"r:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeReferredBy :
				if((sip_strcasecmp((SIP_S8bit *)"Referred-By:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"b:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif
		case SipHdrTypeRecordRoute :
				if(sip_strcasecmp((SIP_S8bit *)"Record-Route:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp :
				if(sip_strcasecmp((SIP_S8bit *)"Timestamp:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
		case SipHdrTypeTo :
				if((sip_strcasecmp((SIP_S8bit *)"To:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"t:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeVia :
				if((sip_strcasecmp((SIP_S8bit *)"Via:", hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"v:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeContentEncoding :
				if((sip_strcasecmp((SIP_S8bit *)"Content-Encoding:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"e:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		/* RPR */
		case SipHdrTypeSupported :
				if ( (sip_strcasecmp((SIP_S8bit *)"Supported:",hdr_name)==0) ||
				   (sip_strcasecmp((SIP_S8bit *)"k",hdr_name)==0) )
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;

		case SipHdrTypeContentLength :
				if((sip_strcasecmp((SIP_S8bit *)"Content-Length:", hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"l:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeContentType :
				if((sip_strcasecmp((SIP_S8bit *)"Content-Type:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"c:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		/* bcpt ext */
		case SipHdrTypeMimeVersion :
				if(sip_strcasecmp((SIP_S8bit *)"Mime-Version:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
				if(sip_strcasecmp((SIP_S8bit *)"Authorization:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
		case SipHdrTypeContactNormal :
		case SipHdrTypeContactWildCard :
		case SipHdrTypeContactAny :
				if((sip_strcasecmp((SIP_S8bit *)"Contact:",hdr_name)==0)||(sip_strcasecmp((SIP_S8bit *)"m:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeMaxforwards :
				if(sip_strcasecmp((SIP_S8bit *)"Max-Forwards:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
#ifdef SIP_AUTHENTICATE				
		case SipHdrTypeProxyauthorization :
				if(sip_strcasecmp((SIP_S8bit *)"Proxy-Authorization:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
		case SipHdrTypeRoute :
				 if(sip_strcasecmp((SIP_S8bit *)"Route:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeRequire :
				if(sip_strcasecmp((SIP_S8bit *)"Require:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		/* Retrans1 */
#ifdef SIP_RPR	
		case SipHdrTypeRSeq :
		if(sip_strcasecmp((SIP_S8bit *)"RSeq:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeRAck :
		if(sip_strcasecmp((SIP_S8bit *)"RAck:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
		/* Retrans1 ends */
		case SipHdrTypeAllow :
				if(sip_strcasecmp((SIP_S8bit *)"Allow:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_AUTHENTICATE				
		case SipHdrTypeProxyAuthenticate  :
				if(sip_strcasecmp((SIP_S8bit *)"Proxy-Authenticate:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate  :
		case SipHdrTypeRetryAfterSec   :
		case SipHdrTypeRetryAfterAny   :
#else
		case SipHdrTypeRetryAfter		:
#endif
				if(sip_strcasecmp((SIP_S8bit *)"Retry-After:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeUnsupported :
				if(sip_strcasecmp((SIP_S8bit *)"Unsupported:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#ifdef SIP_WARNING				
		case SipHdrTypeWarning :
				if(sip_strcasecmp((SIP_S8bit *)"Warning:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif	
#ifdef SIP_3GPP
		case SipHdrTypePath:
				if(sip_strcasecmp((SIP_S8bit *)"Path:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
		case SipHdrTypePAssociatedUri: 
				if(sip_strcasecmp((SIP_S8bit *)"P-Associated-URI:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
     case SipHdrTypePCalledPartyId: 
				if(sip_strcasecmp((SIP_S8bit *)"P-Called-Party-ID:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;

     case SipHdrTypePVisitedNetworkId: 
				if(sip_strcasecmp((SIP_S8bit *)"P-Visited-Network-ID:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;

    	case SipHdrTypePcfAddr: 
				if(sip_strcasecmp((SIP_S8bit *)"P-Charging-Function-Addresses:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;

 		case SipHdrTypePanInfo:  
				if(sip_strcasecmp((SIP_S8bit *)"P-Access-Network-Info:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
 
    	case SipHdrTypePcVector: 
				if(sip_strcasecmp((SIP_S8bit *)"P-Charging-Vector:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;

		case SipHdrTypeServiceRoute: 
				if(sip_strcasecmp((SIP_S8bit *)"Service-Route:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeHistoryInfo:
				if(sip_strcasecmp((SIP_S8bit *)"History-Info:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeAcceptContact:
				if(sip_strcasecmp((SIP_S8bit *)"Accept-Contact:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeRejectContact:
				if(sip_strcasecmp((SIP_S8bit *)"Reject-Contact:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeRequestDisposition:
				if(sip_strcasecmp((SIP_S8bit *)"Request-Disposition:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeJoin:
				if(sip_strcasecmp((SIP_S8bit *)"Join:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeIfMatch:
				if(sip_strcasecmp((SIP_S8bit *)"SIP-If-Match:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
		case SipHdrTypeETag:
				if(sip_strcasecmp((SIP_S8bit *)"SIP-ETag:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
#endif
#ifdef SIP_AUTHENTICATE				
		case SipHdrTypeWwwAuthenticate :
				if(sip_strcasecmp((SIP_S8bit *)"WWW-Authenticate:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif				
		case SipHdrTypeContentDisposition:
				if(sip_strcasecmp((SIP_S8bit *)"Content-Disposition:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break; 

#ifdef SIP_IMPP
		case SipHdrTypeEvent :
				if(sip_strcasecmp((SIP_S8bit *)"Event:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeAllowEvents :
				if(sip_strcasecmp((SIP_S8bit *)"Allow-Events:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
		case SipHdrTypeSubscriptionState :
				if(sip_strcasecmp((SIP_S8bit *)"Subscription-State:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityClient :
				if(sip_strcasecmp((SIP_S8bit *)"Security-Client:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
        case SipHdrTypeSecurityVerify :
				if(sip_strcasecmp((SIP_S8bit *)"Security-Verify:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;

        case SipHdrTypeSecurityServer :
				if(sip_strcasecmp((SIP_S8bit *)"Security-Server:",hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}

				break;
#endif                
		case SipHdrTypeUnknown :
					ret = SipSuccess;
				break;

#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE :
				if(sip_strcasecmp((SIP_S8bit *)"Min-SE:", hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
		case SipHdrTypeSessionExpires :
				if((sip_strcasecmp((SIP_S8bit *)"Session-Expires:",hdr_name)==0)|| \
				(sip_strcasecmp((SIP_S8bit *)"x:",hdr_name)==0))
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
#endif
#ifdef SIP_CONGEST
				case SipHdrTypeRsrcPriority:
				if(sip_strcasecmp((SIP_S8bit *)"Resource-Priority:", hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;
				case SipHdrTypeAcceptRsrcPriority:
				if(sip_strcasecmp((SIP_S8bit *)"Accept-Resource-Priority:", hdr_name)==0)
					ret = SipSuccess;
				else
				{
					*err = E_INV_TYPE;
					ret = SipFail;
				}
				break;	
#endif
		default			:
					*err = E_INV_TYPE;
					ret = SipFail;
				break;

	}

	sip_freeString ( str );
	sip_freeString ( temp_str);
	sip_freeString ( hdr_name );
	return ret;

}

