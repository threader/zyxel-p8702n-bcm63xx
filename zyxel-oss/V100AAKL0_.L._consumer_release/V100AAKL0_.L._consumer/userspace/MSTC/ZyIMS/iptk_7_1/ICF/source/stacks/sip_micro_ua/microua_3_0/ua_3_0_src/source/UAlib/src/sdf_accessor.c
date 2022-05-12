/******************************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various structures in the UA toolkit
 ******************************************************************************
 **
 ** FILENAME		: sdf_accessor.c
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various structures in the UA toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 06/12/00	Subhash						Creation
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 12/05/03     Subbarao	UATK 2.1 Design      RFC 3428 feature added.
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#include "ALheader.h"
#include "sdf_portlayer.h"
#include "sdf.h"
#include "sdf_accessor.h"
#include "sdf_init.h"
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_configs.h"
#include "sdf_trace.h"
#include "sdf_callbacks.h"
#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif
#include "sdf_sdpaccessor.h"
#include "sdf_sdpinit.h"
#include "sdf_sdp.h"

#ifdef SDF_SUBSCRIPTION_LAYER
#include "sdf_subslayerapi.h"
#include "sdf_subslayerutils.h"
#include "sdf_subslayerinitfree.h"
#endif /* End of SDF_SUBSCRIPTION_LAYER */


/******************************************************************************
** FUNCTION: sdf_ivk_uaGetFromInfoFromCommonInfo
**
** DESCRIPTION: This function returns the From information from the
**				CommonInfo structure
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromCommonInfo
#ifdef ANSI_PROTO
	(Sdf_st_commonInfo *pCommonInfo, 
	Sdf_st_nameInfo **ppFrom,
	Sdf_st_error *pError)
#else	
	(pCommonInfo, ppFrom, pError)
	Sdf_st_commonInfo *pCommonInfo;
	Sdf_st_nameInfo **ppFrom;
	Sdf_st_error *pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	SipUrl *pSipUrl =Sdf_co_null;
	Sdf_ty_u32bit userHostLen=0, urlParamCount=0;
	Sdf_ty_u32bit extParamCount=0, tagCount=0, iterator=0;
	Sdf_ty_s8bit *pUserName = Sdf_co_null, *pHost = Sdf_co_null;
	Sdf_ty_s8bit *pDispName = Sdf_co_null;
	Sdf_ty_s8bit *pUri=Sdf_co_null,*pScheme=Sdf_co_null,*pAddress=Sdf_co_null;
	Sdf_ty_s8bit dTempUri[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_bool	 dIsTelUrl = Sdf_co_false;
	Sdf_ty_bool	 dIsImUrl  = Sdf_co_false;	
	Sdf_ty_bool	 dIsPresUrl  = Sdf_co_false;	

	en_AddrType addrType;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetFromInfoFromCommonInfo");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"CommonInfo Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppFrom == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Name info pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	*ppFrom=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if(pCommonInfo->pFrom == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"CommonInfo Param does not have a from header ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(sip_getAddrSpecFromFromHdr(pCommonInfo->pFrom, &pAddrSpec,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Failed to extract addrspec from From header",pError);
#endif
		pError->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}
	if(sip_getAddrTypeFromAddrSpec(pAddrSpec, &addrType, \
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Failed to extract type from from header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		sip_freeSipAddrSpec(pAddrSpec);
		return Sdf_co_fail;
	}
	/* 
	 * Now start filling in stuff into name-info
	 */

	/*
	 * Initialize a From NameInfo structure to return the information
	 */
	if(sdf_ivk_uaInitNameInfo(ppFrom, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Name info initialization failed",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#ifdef SDF_TLS
	if((addrType != SipAddrSipUri) && (addrType != SipAddrSipSUri))
#else
	if(addrType != SipAddrSipUri)
#endif
	{
#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
			   	==SipSuccess)
			dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
		if( dIsTelUrl != Sdf_co_true )
		{
			if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
				   	==SipSuccess)
				dIsImUrl = Sdf_co_true;
		}
#endif		
#ifdef SDF_PRES
        if((dIsTelUrl != Sdf_co_true)&&(dIsImUrl != Sdf_co_true))
        {
			if(sip_isPresUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
				   	==SipSuccess)
				dIsPresUrl = Sdf_co_true;
        }    
#endif        
		if((dIsTelUrl == Sdf_co_false) && (dIsImUrl == Sdf_co_false) &&
            (dIsPresUrl == Sdf_co_false))
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"From Header Contains Unsupported URL scheme",pError);
#endif
			pError->errCode=Sdf_en_notImplemented;
			return Sdf_co_fail;
		}
		if(sip_getUriFromAddrSpec(pAddrSpec,&pUri,\
			(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Failed to Get Uri From AddrSpec",pError);
#endif
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		userHostLen += Sdf_mc_strlen(pUri);

		(*ppFrom)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
		userHostLen+2,pError);
		if((*ppFrom)->pAddress == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		Sdf_mc_strcpy(dTempUri,pUri);
		pScheme = Sdf_mc_strtokr(dTempUri,":",&pAddress);
		Sdf_mc_strcpy((*ppFrom)->pAddress,pAddress);
	}
	else
	{
		if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl,\
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Could not extract SIP URL from From header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		userHostLen = 0;

/*w534*/(void)sip_getUserFromUrl(pSipUrl, &pUserName, \
					(SipError *)&(pError->stkErrCode));

		if(pUserName != Sdf_co_null)
			userHostLen += Sdf_mc_strlen(pUserName);

/*w534*/(void)sip_getHostFromUrl(pSipUrl, &pHost, (SipError *)&(pError->stkErrCode));

		userHostLen += Sdf_mc_strlen(pHost);

		(*ppFrom)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
			userHostLen+2,pError);

		if((*ppFrom)->pAddress == Sdf_co_null)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		/*
		 * Copy the user@host portion into ppFrom->pAddress
		 */
		Sdf_mc_strcpy((*ppFrom)->pAddress,"");
		if ((pUserName!=Sdf_co_null) && (Sdf_mc_strcmp(pUserName, "") != 0))
		{
			Sdf_mc_strcat((*ppFrom)->pAddress, pUserName);
			Sdf_mc_strcat((*ppFrom)->pAddress, "@");
		}
		Sdf_mc_strcat((*ppFrom)->pAddress, pHost);

		if(sip_getPortFromUrl(pSipUrl, &((*ppFrom)->dPort), \
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_TLS
			if (addrType == SipAddrSipSUri)
			    (*ppFrom)->dPort = Sdf_mc_defaultSipSPort;
			else
#endif
			    (*ppFrom)->dPort = Sdf_mc_defaultSipPort;
		}
	
		/* 
		 * Username, host and port extracted. Get URL params now
		 */
		if (sip_getUrlParamCountFromUrl(pSipUrl, &urlParamCount, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Could not get count of url parameters from From header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		for(iterator = 0; iterator < urlParamCount; iterator++)
		{
			SipParam *pParam;
			if (sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, iterator, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
					"Could not get url parameter at index from From header", \
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			if (sdf_listAppend(&((*ppFrom)->slUrlParams), pParam,  \
				pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
					"Failed to append Url parameter to nameInfo structure", \
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
		}
		sip_freeSipUrl(pSipUrl);
	}

	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);

	/* 
	 * All url related information extracted. Now get the header parameters
	 */

	/* 
	 * For the tag we get the first tag from the list in the header
	 */
/*w534*/(void)sip_getTagCountFromFromHdr(pCommonInfo->pFrom, &tagCount, \
		(SipError *)&(pError->stkErrCode));

	if(tagCount != 0)
	{
		Sdf_ty_s8bit *pTag;
/*w534*/(void)sip_getTagAtIndexFromFromHdr(pCommonInfo->pFrom, &pTag, 0, \
			(SipError *)&(pError->stkErrCode));
		Sdf_mc_strdup((*ppFrom)->pTag,pTag);
	}
	
	/*
	 * Extract display name
	 */
	if(sip_getDispNameFromFromHdr(pCommonInfo->pFrom, &pDispName, \
		(SipError *)&(pError->stkErrCode)) ==SipFail)
		(*ppFrom)->pName = Sdf_co_null;
	else
	{
		Sdf_mc_strdup((*ppFrom)->pName,pDispName);
	}

	/*Fill the scheme*/
	if(addrType==SipAddrSipUri)
		Sdf_mc_strdup((*ppFrom)->pScheme,"sip");
#ifdef SDF_TLS
	else if(addrType==SipAddrSipSUri)
		Sdf_mc_strdup((*ppFrom)->pScheme,"sips");
#endif
	else
		Sdf_mc_strdup((*ppFrom)->pScheme,pScheme);

	if (sip_getExtensionParamCountFromFromHdr(pCommonInfo->pFrom, \
		&extParamCount, (SipError *)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
			"Could not get count of extension parameters from From header", \
			pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	for(iterator = 0; iterator < extParamCount; iterator++)
	{
		SipParam *pParam;
		if (sip_getExtensionParamAtIndexFromFromHdr(pCommonInfo->pFrom, \
			&pParam, iterator, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Could not get extension parameter at index from From header",\
				pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sdf_listAppend(&((*ppFrom)->slHeaderParams), pParam, pError) \
			== Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCommonInfo(): "
				"Could not append extn param to nameInfo structure",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetFromInfoFromCommonInfo");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}		


/******************************************************************************
** FUNCTION: 	sdf_ivk_uaGetToInfoFromCommonInfo
**
** DESCRIPTION: This function returns the To information from the
**				CommonInfo structure
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetToInfoFromCommonInfo
#ifdef ANSI_PROTO
	(Sdf_st_commonInfo *pCommonInfo, 
	Sdf_st_nameInfo **ppTo,
	Sdf_st_error *pError)
#else	
	(pCommonInfo, ppTo, pError)
	Sdf_st_commonInfo *pCommonInfo;
	Sdf_st_nameInfo **ppTo;
	Sdf_st_error *pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	SipUrl *pSipUrl = Sdf_co_null;
	Sdf_ty_u32bit urlParamCount=0, extParamCount=0, tagCount=0, iterator=0;
	Sdf_ty_s8bit *pUserName = Sdf_co_null, *pHost = Sdf_co_null;
	Sdf_ty_s8bit *pDispName = Sdf_co_null;
	Sdf_ty_s8bit	*pScheme = Sdf_co_null;
	Sdf_ty_s8bit	*pUri = Sdf_co_null;
	Sdf_ty_s8bit	*pAddress = Sdf_co_null;
	Sdf_ty_s8bit dTempUri[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_bool	 dIsTelUrl = Sdf_co_false;
	Sdf_ty_bool	 dIsImUrl  = Sdf_co_false;	
	Sdf_ty_bool	 dIsPresUrl  = Sdf_co_false;	

	en_AddrType addrType;
	Sdf_ty_u32bit userHostLen=0;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetToInfoFromCommonInfo");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"CommonInfo Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(ppTo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Name info pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	*ppTo=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if(pCommonInfo->pTo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"CommonInfo Param does not have a To header ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(sip_getAddrSpecFromToHdr(pCommonInfo->pTo, &pAddrSpec,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Failed to extract addrspec from To header",pError);
#endif
		pError->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}
	if(sip_getAddrTypeFromAddrSpec(pAddrSpec, &addrType, \
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Failed to extract type from To header",pError);
#endif
		sip_freeSipAddrSpec(pAddrSpec);
		return Sdf_co_fail;
	}
	/* 
	 * Now start filling in stuff into name-info
	 */

	/*
	 * Initialize a To NameInfo structure to return the information
	 */
	if(sdf_ivk_uaInitNameInfo(ppTo, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Name info initialization failed",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		sip_freeSipUrl(pSipUrl);
		sip_freeSipAddrSpec(pAddrSpec);
		return Sdf_co_fail;
	}

#ifdef SDF_TLS
	if((addrType != SipAddrSipUri) && (addrType != SipAddrSipSUri))
#else
	if(addrType != SipAddrSipUri)
#endif
	{
#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode))==SipSuccess)
			dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
		if( dIsTelUrl == Sdf_co_false )
		{
			if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsImUrl = Sdf_co_true;
		}
#endif
#ifdef SDF_PRES
		if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true))
        {
			if(sip_isPresUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsPresUrl = Sdf_co_true;
        }    
#endif        
		if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true) &&
           (dIsPresUrl != Sdf_co_true))
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
			sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"From Header Contains Unsupported URI Scheme",pError);
#endif
			pError->errCode=Sdf_en_notImplemented;
			return Sdf_co_fail;
		}

		if(sip_getUriFromAddrSpec(pAddrSpec,&pUri,\
				(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Failed to Get Uri From AddrSpec",pError);
#endif
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		userHostLen += Sdf_mc_strlen(pUri);

		(*ppTo)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
				userHostLen+2,pError);
		if((*ppTo)->pAddress == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		/*Copy Uri into the Address.Before Cpy remove the tel: from it.*/
		Sdf_mc_strcpy(dTempUri,pUri);
		pScheme = Sdf_mc_strtokr(dTempUri,":",&pAddress);
		Sdf_mc_strcpy((*ppTo)->pAddress,pAddress);
	}
	else
	{
		if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl,\
			pError) == Sdf_co_fail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Could not extract SIP URL from To header",pError);
#endif
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

/*w534*/(void)sip_getUserFromUrl(pSipUrl, &pUserName, \
				(SipError *)&(pError->stkErrCode));

		if(pUserName != Sdf_co_null)
			userHostLen += Sdf_mc_strlen(pUserName);

/*w534*/(void)sip_getHostFromUrl(pSipUrl, &pHost, (SipError *)&(pError->stkErrCode));

		userHostLen += Sdf_mc_strlen(pHost);

		(*ppTo)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
			userHostLen+2,pError);

		if((*ppTo)->pAddress == Sdf_co_null)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		/*
		 * Copy the user@host portion into ppFrom->pAddress
		 */
		Sdf_mc_strcpy((*ppTo)->pAddress,"");
		if ((pUserName!=Sdf_co_null) && (Sdf_mc_strcmp(pUserName, "") != 0))
		{
			Sdf_mc_strcat((*ppTo)->pAddress, pUserName);
			Sdf_mc_strcat((*ppTo)->pAddress, "@");
		}
		Sdf_mc_strcat((*ppTo)->pAddress, pHost);

		if(sip_getPortFromUrl(pSipUrl, &((*ppTo)->dPort), \
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_TLS
			if (addrType == SipAddrSipSUri)
			    (*ppTo)->dPort = Sdf_mc_defaultSipSPort;
			else
#endif
			    (*ppTo)->dPort = Sdf_mc_defaultSipPort;
		}

		/* 
		 * Username, host and port extracted. Get URL params now
		 */
		if (sip_getUrlParamCountFromUrl(pSipUrl, &urlParamCount, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Could not get count of url parameters from To header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		for(iterator = 0; iterator < urlParamCount; iterator++)
		{
			SipParam *pParam;
			if (sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, iterator, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
					"Could not get url parameter at index from To header", \
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			if (sdf_listAppend(&((*ppTo)->slUrlParams), pParam, pError) \
					== Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
					"Failed to append Url parameter to nameInfo structure", \
					pError);
#endif	
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
		}
		sip_freeSipUrl(pSipUrl);
	}

	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);

	/* 
	 * All url related information extracted. Now get the header parameters
	 */
	
	/* 
	 * For the tag we get the first tag from the list in the header
	 */
/*w534*/(void)sip_getTagCountFromToHdr(pCommonInfo->pTo, &tagCount, \
		(SipError *)&(pError->stkErrCode));

	if(tagCount != 0)
	{
		Sdf_ty_s8bit *pTag;
/*w534*/(void)sip_getTagAtIndexFromToHdr(pCommonInfo->pTo, &pTag, 0, \
			(SipError *)&(pError->stkErrCode));
		Sdf_mc_strdup((*ppTo)->pTag,pTag);
	}
	
	/*
	 * Extract display name
	 */
	if(sip_getDispNameFromToHdr(pCommonInfo->pTo, &pDispName, \
		(SipError *)&(pError->stkErrCode)) ==SipFail)
		(*ppTo)->pName = Sdf_co_null;
	else
	{
		Sdf_mc_strdup((*ppTo)->pName,pDispName);
	}

	/*Fill the scheme*/
	if (addrType == SipAddrSipUri)
		Sdf_mc_strdup((*ppTo)->pScheme,"sip");
#ifdef SDF_TLS
	else if(addrType==SipAddrSipSUri)
		Sdf_mc_strdup((*ppTo)->pScheme,"sips");
#endif
	else
		Sdf_mc_strdup((*ppTo)->pScheme,pScheme);
			
	if (sip_getExtensionParamCountFromToHdr(pCommonInfo->pTo, &extParamCount,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
			"Could not get count of extension parameters from To header", \
			pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	for(iterator = 0; iterator < extParamCount; iterator++)
	{
		SipParam *pParam;
		if (sip_getExtensionParamAtIndexFromToHdr(pCommonInfo->pTo, \
			&pParam, iterator, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Could not get extension parameter at index from To header",\
				pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sdf_listAppend(&((*ppTo)->slHeaderParams), pParam, pError) \
			== Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppTo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCommonInfo(): "
				"Could not append extn param to nameInfo structure",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		
	}

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetToInfoFromCommonInfo");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetDestInfoFromTransaction
 **
 ** DESCRIPTION: This function returns the Destination Info from the
 **				 Transaction structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetDestInfoFromTransaction
#ifdef ANSI_PROTO
	(Sdf_st_transaction  *pTransaction, 
	Sdf_st_overlapTransInfo  *pOverlapTransInfo, 
	Sdf_st_transportInfo **ppDestInfo,
	Sdf_st_error *pError)
#else	
	(pTransaction,pOverlapTransInfo,ppDestInfo, pError)
	Sdf_st_transaction  *pTransaction; 
	Sdf_st_overlapTransInfo  *pOverlapTransInfo; 
	Sdf_st_transportInfo **ppDestInfo;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetDestInfoFromTransaction");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetDestInfoFromTransaction(): Error Param Passed to "
			"function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if ((pTransaction == Sdf_co_null) && (pOverlapTransInfo==Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestInfoFromTransaction():  "
			"Transaction object is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (ppDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestInfoFromTransaction():  "
			"ppDestInfo param is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	if(pOverlapTransInfo==Sdf_co_null)
	{
#ifdef SDF_LINT
		/* This is a dummy check to remove the lint warning.
		 */	 
		if(pTransaction == Sdf_co_null)
				return Sdf_co_fail;
#endif					
		if (pTransaction->pDestInfo == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestInfoFromTransaction():  "
				"pDestInfo in pTransaction is invalid ",pError);
#endif
			pError->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		HSS_LOCKEDINCREF(pTransaction->pDestInfo->dRefCount);
		*ppDestInfo = pTransaction->pDestInfo;
	}
	else
	{
#ifdef SDF_LINT
		/* This is a dummy check to remove the lint warning.
		 */	 
		if(pOverlapTransInfo == Sdf_co_null)
				return Sdf_co_fail;
#endif
		if (pOverlapTransInfo->pDestInfo == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetDestInfoFromTransaction():  "
				"pDestInfo in pOverlapTransInfo is invalid ",pError);
#endif
			pError->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}

		HSS_LOCKEDINCREF(pOverlapTransInfo->pDestInfo->dRefCount);
		*ppDestInfo = pOverlapTransInfo->pDestInfo;


	}
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetDestInfoFromTransaction");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetCommonInfoFromCallObject
 **
 ** DESCRIPTION: This function returns the Common Info from the
 **				 call object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetCommonInfoFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_commonInfo **ppCommonInfo,
	Sdf_st_error *pError)
#else	
	(pObject, ppCommonInfo, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_commonInfo **ppCommonInfo;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetCommonInfoFromCallObject");

	/* ================ Parameter validations begin ================== */
#ifdef SDF_PARAMVALIDATION

	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetCommonInfoFromCallObject(): Error Param Passed to "
			"function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCommonInfoFromCallObject():  "
			"Call Object Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (ppCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCommonInfoFromCallObject():  "
			"ppCommonInfo  Param  is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (pObject->pCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCommonInfoFromCallObject():  "
			"Common Info in Call Object  is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	HSS_LOCKEDINCREF(pObject->pCommonInfo->dRefCount);
	*ppCommonInfo = pObject->pCommonInfo;

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetCommonInfoFromCallObject");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetFromInfoFromCallObject
 **
 ** DESCRIPTION: A higher level API that directly retrieves 
 **				 the From Info from the call object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_nameInfo **ppFrom,
	Sdf_st_error *pError)
#else	
	(pObject, ppFrom, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_nameInfo **ppFrom;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetFromInfoFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetFromInfoFromCallObject(): Error Param Passed to "
			"function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCallObject():  "
			"Call Object Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */
	
	*ppFrom=Sdf_co_null;
	/* ================ Parameter validations end ================== */

	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCallObject():  "
			"Getting Common Info From Call Object failed ",pError);
#endif
		return Sdf_co_fail;
	}

	if (sdf_ivk_uaGetFromInfoFromCommonInfo(pCommonInfo, ppFrom, pError) \
		== Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromCallObject():  "
			"Getting From Info From Common Info failed ",pError);
#endif
		return Sdf_co_fail;
	}	
		
	/* Free the local reference */
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetFromInfoFromCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}	


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetToInfoFromCallObject
 **
 ** DESCRIPTION: A higher level API that directly retrieves the To Info 
 **				 from the call object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetToInfoFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_nameInfo **ppTo,
	Sdf_st_error *pError)
#else	
	(pObject, ppTo, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_nameInfo **ppTo;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_commonInfo *pCommonInfo = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetToInfoFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetToInfoFromCallObject(): Error Param Passed to "
			"function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCallObject():  "
			"Call Object Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	*ppTo=Sdf_co_null;

	/* ================ Parameter validations end ================== */
	
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pObject, &pCommonInfo, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCallObject():  "
			"Getting Common Info From Call Object Failed ",pError);
#endif
		return Sdf_co_fail;
	}

	if (sdf_ivk_uaGetToInfoFromCommonInfo(pCommonInfo, ppTo, pError) \
		== Sdf_co_fail)
	{
		(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetToInfoFromCallObject():  "
			"Getting To Info From Common Info Failed ",pError);
#endif
		return Sdf_co_fail;
	}	
		
	/* Free the local reference */
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetToInfoFromCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetCallInfoFromCallObject
 **
 ** DESCRIPTION: This function retrieves the CallInfo structure from the 
 **				 call object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetCallInfoFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_callInfo **ppCallInfo,
	Sdf_st_error *pError)
#else	
	(pObject, ppCallInfo, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_msgInfo **ppCallInfo;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaGetCallInfoFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetCallInfoFromCallObject(): Error Param Passed"
			" is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallInfoFromCallObject():" 
			"Call Object param passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppCallInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallInfoFromCallObject(): "
			"Call Info param passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	*ppCallInfo=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if (pObject->pCallInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallInfoFromCallObject(): "
			"Call info in Call Object does not exist ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	HSS_LOCKEDINCREF(pObject->pCallInfo->dRefCount);
	*ppCallInfo = pObject->pCallInfo;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaGetCallInfoFromCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetAppDataFromCallObject
 **
 ** DESCRIPTION: This function returns the application context data 
 **				 structure (AppData) from the Call Object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetAppDataFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_st_appData **ppAppData,
	Sdf_st_error *pError)
#else	
	(pObject, ppAppData, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_appData **ppAppData;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetAppDataFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetAppDataFromCallObject(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetAppDataFromCallObject():  "
			"Call Object Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (ppAppData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetAppDataFromCallObject():  "
			"AppData param passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */


	*ppAppData=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if (pObject->pAppData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetAppDataFromCallObject():  "
			"AppData of the Call Object does not exist ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	HSS_LOCKEDINCREF(pObject->pAppData->dRefCount);
	*ppAppData = pObject->pAppData;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetAppDataFromCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetTransportInfoFromInitData
 **
 ** DESCRIPTION: This function returns the Transport Info from the
 **	 			 initData structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTransportInfoFromInitData
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData,
	Sdf_st_transportInfo **ppTransport,
	Sdf_st_error *pError)
#else	
	(pInitData, ppTransport,pError)
	Sdf_st_initData *pInitData;
	Sdf_st_transportInfo **ppTransport;
	Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "	
		"Entering sdf_ivk_uaGetTransportInfoFromInitData");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetTransportInfoFromInitData(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransportInfoFromInitData():  "
			"InitData param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppTransport == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransportInfoFromInitData():  "
			"Transport param passed is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	*ppTransport=Sdf_co_null;
	
	/* ================ Parameter validations end ================== */

	if (pInitData->pTransport == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransportInfoFromInitData():  "
			"Transport in InitData does nto exist",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	HSS_LOCKEDINCREF(pInitData->pTransport->dRefCount);
	*ppTransport = pInitData->pTransport;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetTransportInfoFromInitData");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneNameInfo
 **
 ** DESCRIPTION: This function copies the Name info structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneNameInfo
#ifdef ANSI_PROTO 
(Sdf_st_nameInfo *pDestInfo,
Sdf_st_nameInfo *pSrcInfo,
Sdf_st_error *pErr)
#else
(pDestInfo,pSrcInfo, pErr )
 Sdf_st_nameInfo *pDestInfo;
 Sdf_st_nameInfo *pSrcInfo;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneNameInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneNameInfo(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneNameInfo():  "
			"pDestInfo param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneNameInfo():  "
			"pSrcInfo Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pName),pErr);
	if(pSrcInfo->pName == Sdf_co_null)
		pDestInfo->pName = Sdf_co_null;
	else
		pDestInfo->pName = Sdf_mc_strdupAccessor(pSrcInfo->pName);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pAddress),pErr);
	if(pSrcInfo->pAddress == Sdf_co_null)
		pDestInfo->pAddress = Sdf_co_null;
	else
		pDestInfo->pAddress = Sdf_mc_strdupAccessor( pSrcInfo->pAddress);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)\
		&(pDestInfo->pTag),pErr);
	if(pSrcInfo->pTag == Sdf_co_null)
		pDestInfo->pTag = Sdf_co_null;
	else
		pDestInfo->pTag = Sdf_mc_strdupAccessor(pSrcInfo->pTag);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pScheme),pErr);
	if(pSrcInfo->pScheme == Sdf_co_null)
		pDestInfo->pScheme = Sdf_mc_strdupAccessor("sip");
	else
		pDestInfo->pScheme = Sdf_mc_strdupAccessor(pSrcInfo->pScheme);
	
	pDestInfo->dPort = pSrcInfo->dPort;
	/*
	 * Clone the URL parameters
	 */
	(void)sdf_listDeleteAll(&(pDestInfo->slUrlParams), pErr);
	if ((__sip_cloneSipParamList((SipList *)&(pDestInfo->slUrlParams),\
		(SipList *)&(pSrcInfo->slUrlParams),\
		(SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneNameInfo( ): "
			"Failed to clone URL params from source to destination",pErr);
#endif
			return Sdf_co_fail;
	}

	/*
	 * Clone the Header parameters
	 */
	(void)sdf_listDeleteAll(&(pDestInfo->slHeaderParams), \
		pErr);
	if ((__sip_cloneSipParamList((SipList *)&(pDestInfo->slHeaderParams), \
		(SipList *)&(pSrcInfo->slHeaderParams), \
		(SipError *)&(pErr->stkErrCode))) == SipFail)
	{
		pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneNameInfo( ): "
			"Failed to clone header params from source to destination",pErr);
#endif
			return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneNameInfo");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneTransportInfo
 **
 ** DESCRIPTION: This function copies the transport info structure 
 **				 from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneTransportInfo
#ifdef ANSI_PROTO 
(Sdf_st_transportInfo *pDestInfo,
Sdf_st_transportInfo *pSrcInfo,
Sdf_st_error *pErr)
#else
(pDestInfo,pSrcInfo, pErr )
 Sdf_st_transportInfo *pDestInfo;
 Sdf_st_transportInfo *pSrcInfo;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneTransportInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneTransportInfo(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransportInfo():  "
			"DestInfo Param is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransportInfo():  "
			"SrcInfo Param is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&(pDestInfo->pIp),pErr);
	pDestInfo->pIp = Sdf_mc_strdupAccessor(pSrcInfo->pIp);

	pDestInfo->dPort = pSrcInfo->dPort;

    pDestInfo->dScheme = pSrcInfo->dScheme;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneTransportInfo");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneCallObjectKey
 **
 ** DESCRIPTION: This function copies the call-object key structure 
 **				 from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneCallObjectKey
#ifdef ANSI_PROTO 
	(Sdf_st_callObjectKey *pDestInfo,
	Sdf_st_callObjectKey *pSrcInfo,
	Sdf_st_error *pErr)
#else
	(pDestInfo,pSrcInfo, pErr )
	 Sdf_st_callObjectKey *pDestInfo;
	 Sdf_st_callObjectKey *pSrcInfo;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaCloneCallObjectKey");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneCallObjectKey(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObjectKey():  "
			"pDestInfo param passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObjectKey():  "
			"pSrcInfo param passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if( pSrcInfo->pLocalAddr->dType == SipHdrTypeFrom )
	{
		(void)sip_initSipFromHeader( \
			(SipFromHeader **)&(pDestInfo->pLocalAddr->pHeader), \
			(SipError *) &(pErr->stkErrCode));
	}
	else
	{
		(void)sip_initSipToHeader( \
			(SipToHeader **)&(pDestInfo->pLocalAddr->pHeader), \
			(SipError *) &(pErr->stkErrCode));
	}
		
	if(sip_cloneSipHeader(pDestInfo->pLocalAddr, pSrcInfo->pLocalAddr,\
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObjectKey(): "
			"From header cloning failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if( pSrcInfo->pRemoteAddr->dType == SipHdrTypeFrom )
	{
		(void)sip_initSipFromHeader( \
			(SipFromHeader **)&(pDestInfo->pRemoteAddr->pHeader), \
			(SipError *) &(pErr->stkErrCode));
	}
	else
	{
		(void)sip_initSipToHeader( \
			(SipToHeader **)&(pDestInfo->pRemoteAddr->pHeader), \
			(SipError *) &(pErr->stkErrCode));
	}

	if(sip_cloneSipHeader(pDestInfo->pRemoteAddr, pSrcInfo->pRemoteAddr,\
		(SipError *) &(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObjectKey(): "
			"To header cloining failed",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pCallId),pErr);

	pDestInfo->pCallId = Sdf_mc_strdupAccessor(pSrcInfo->pCallId);	

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pLocalViaBranch),pErr);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pRemoteViaBranch),pErr);

	pDestInfo->pLocalViaBranch = \
				Sdf_mc_strdupAccessor(pSrcInfo->pLocalViaBranch);	

	pDestInfo->pRemoteViaBranch = \
				Sdf_mc_strdupAccessor(pSrcInfo->pRemoteViaBranch);	
	
	pDestInfo->dCaller = pSrcInfo->dCaller;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCloneCallObjectKey");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneCommonInfo
 **
 ** DESCRIPTION: This function copies the CommonInfo structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneCommonInfo
#ifdef ANSI_PROTO 
	(Sdf_st_commonInfo *pDestInfo,
	Sdf_st_commonInfo *pSrcInfo,
	Sdf_st_error *pErr)
#else
	(pDestInfo,pSrcInfo, pErr )
	 Sdf_st_commonInfo *pDestInfo;
	 Sdf_st_commonInfo *pSrcInfo;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneCommonInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneCommonInfo(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"pDestInfo is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"pSrcInfo is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_ivk_uaCloneCallObjectKey(pDestInfo->pKey, pSrcInfo->pKey,\
		pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to clone call object key Structure ",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	if (sip_cloneSipHeader(pDestInfo->pFrom, pSrcInfo->pFrom,\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,  Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to clone From header Structure ",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}
	
	if (sip_cloneSipHeader(pDestInfo->pTo, pSrcInfo->pTo,\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to clone To header Structure ",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Delete contents of the destination common info's contact header list
	 * before adding the new cloned contact headers to it
	 */
	if (sdf_listDeleteAll(&(pDestInfo->slContact), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to delete entries in the destination common info",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	if (sdf_fn_uaCloneSipHeadersList((Sdf_ty_slist *)&(pDestInfo->slContact), \
		(Sdf_ty_slist *)&(pSrcInfo->slContact), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to clone list of Contact headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Delete contents of the destination common info's Route header list
	 * before adding the new cloned contact headers to it
	 */
	if (sdf_listDeleteAll(&(pDestInfo->slInvRoute), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to delete entries in the destination common info",pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	if (sdf_fn_uaCloneSipHeadersList((Sdf_ty_slist *)&(pDestInfo->slInvRoute), \
		(Sdf_ty_slist *)&(pSrcInfo->slInvRoute), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCommonInfo():  "
			"Failed to clone list of Route headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}
	if( pDestInfo->pChallangePasswd != Sdf_co_null )
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid *)(pDestInfo->pChallangePasswd), pErr);

	pDestInfo->pChallangePasswd =	\
		Sdf_mc_strdupAccessor(pSrcInfo->pChallangePasswd);

	pDestInfo->dPreloadedSet = pSrcInfo->dPreloadedSet;
	pDestInfo->dSdpInInvite = pSrcInfo->dSdpInInvite;
	pDestInfo->dRemoteCseq = pSrcInfo->dRemoteCseq;
	pDestInfo->dLocalCseq = pSrcInfo->dLocalCseq;

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pCallid),pErr);
	pDestInfo->pCallid = Sdf_mc_strdupAccessor(pSrcInfo->pCallid);

    pDestInfo->dScheme = pSrcInfo->dScheme;

	pDestInfo->dCallerCancelStatus = pSrcInfo->dCallerCancelStatus;

	pDestInfo->dLocalRseq = pSrcInfo->dLocalRseq;
	pDestInfo->dRemoteRseq = pSrcInfo->dRemoteRseq;
#ifdef SIP_RPR
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestInfo->pRPRMethod),pErr);
	pDestInfo->pRPRMethod = Sdf_mc_strdupAccessor(pSrcInfo->pRPRMethod);	
#endif /* SIP_RPR */

	pDestInfo->dLocalRegCseq = pSrcInfo->dLocalRegCseq;
	pDestInfo->dRemoteRegCseq = pSrcInfo->dRemoteRegCseq;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneCommonInfo");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneOverlapTransInfo
 **
 ** DESCRIPTION: This function copies the 
 **				 overlap transport info structure from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneOverlapTransInfo
#ifdef ANSI_PROTO 
(Sdf_st_overlapTransInfo *pDestObj,
Sdf_st_overlapTransInfo *pSrcObj,
Sdf_st_error *pErr)
#else
(pDestObj,pSrcObj, pErr )
 Sdf_st_overlapTransInfo *pDestObj;
 Sdf_st_overlapTransInfo *pSrcObj;
 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneOverlapTransInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneOverlapTransInfo (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo():  "
			"pSrcObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	pDestObj->dState = pSrcObj->dState;

	/* 
	 * Clone list of processed headers
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slProcessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slProcessedHeaders), \
		&dListIterator, pErr);
	
	/* 
	 * Clone TransportInfo structure
	 */
	if(pDestObj->pDestInfo == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pDestObj->pDestInfo), pErr) == \
			Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing transport info in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pDestObj->pDestInfo, \
		pSrcObj->pDestInfo, pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
			"Cloning transport info failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Clone the dSockfd
	 */
	pDestObj->dSocket.dSockfd=pSrcObj->dSocket.dSockfd;


	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList = (Sdf_st_headerList *)\
			(dListIterator.pCurrentElement->pData);
		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing HeaderList structure failed",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning Processed Headers list failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slProcessedHeaders), (Sdf_ty_pvoid)\
	 		pClonedHdrList,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_overlapTransInfoAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Appending HeaderList structure to ProcessedHeaders list of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_overlapTransInfoAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
#ifdef SDF_SUBSCRIPTION_LAYER 
    /* Clone the SubsTxnData
     */
  if(pSrcObj->pSubsTxnData != Sdf_co_null)  
  {    
    if(pDestObj->pSubsTxnData == Sdf_co_null)
    {
      if(sdf_fn_uaSLInitSubsTxnData(&(pDestObj->pSubsTxnData),
        pErr) == Sdf_co_fail)  
      {
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing Subs-Txn Data failed ",pErr);
#endif
            return Sdf_co_fail;
      }    
      if(sdf_fn_uaSLCloneSubsTxnData(pDestObj->pSubsTxnData,
        pSrcObj->pSubsTxnData,pErr) == Sdf_co_fail)
      {    
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning Subs-Txn Data failed ",pErr);
#endif
			return Sdf_co_fail;
      }
    }
    else
    {    
        if(sdf_fn_uaSLCloneSubsTxnData(pDestObj->pSubsTxnData,
        pSrcObj->pSubsTxnData,pErr) == Sdf_co_fail)
        {    
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning Subs-Txn Data failed ",pErr);
#endif
			return Sdf_co_fail;
        }    
    }    
  }
  else
      pDestObj->pSubsTxnData = Sdf_co_null;
#endif    

	/* 
	 * Clone list of Unprocessed headers
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slUnprocessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slUnprocessedHeaders), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;
		
		pHeaderList = (Sdf_st_headerList *)\
			(dListIterator.pCurrentElement->pData);
		
		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing HeaderList structure failed",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning UnprocessedHeaders list failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slUnprocessedHeaders), (Sdf_ty_pvoid)\
	 		pClonedHdrList,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_overlapTransInfoAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Appending HeaderList structure to UnprocessedHeaders list of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_overlapTransInfoAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	pDestObj->dRseq = pSrcObj->dRseq;
	pDestObj->dLocalCseq = pSrcObj->dLocalCseq;
	pDestObj->dRemoteCseq = pSrcObj->dRemoteCseq;
	
	/* 
	 * Clone list of message bodies
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slMsgBodyList), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipMsgBody *pSipMsgBody;
		SipMsgBody *pClonedMsgBody;

		pSipMsgBody = (SipMsgBody*)(dListIterator.pCurrentElement->pData);

		if(sip_initSipMsgBody(&pClonedMsgBody, SipBodyAny,\
		 	(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing SipMsgBody failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sip_cloneSipMsgBody(pClonedMsgBody, pSipMsgBody,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning message body failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slMsgBodyList), (Sdf_ty_pvoid)\
	 		pClonedMsgBody,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_overlapTransInfoAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Appending MsgBody to slMsgBodyList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_overlapTransInfoAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	/* Clone Sip Message */
	if (pDestObj->pSipMsg != Sdf_co_null)
		sip_freeSipMessage(pDestObj->pSipMsg);

	if (pSrcObj->pSipMsg == Sdf_co_null)
		pDestObj->pSipMsg = Sdf_co_null;
	else
	{	
		SipMessage *pClonedMessage;
		if(sip_initSipMessage(&pClonedMessage, \
		 	pSrcObj->pSipMsg->dType, (SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Initializing SipMessage failed ",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if(sip_cloneSipMessage(pClonedMessage, pSrcObj->pSipMsg,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
				"Cloning SipMessage failed ",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		pDestObj->pSipMsg = pClonedMessage;
	}

#ifdef SDF_TXN
	if( pSrcObj->pRprTxnKey != Sdf_co_null )
	{
		if( pDestObj->pRprTxnKey == Sdf_co_null )
/*w534*/(void)sip_initSipTxnKey( &(pDestObj->pRprTxnKey), \
						(SipError *)&(pErr->stkErrCode));

		if (sip_cloneSipTxnKey(pDestObj->pRprTxnKey, pSrcObj->pRprTxnKey, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transactionAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
					"Cloning the SIP Transaction key structure failed.", pErr);
#endif
				pErr->errCode=Sdf_en_overlapTransInfoAccessError;
				return Sdf_co_fail;
		}
	}
	
	if( pSrcObj->pOLTxnKey != Sdf_co_null )
	{
		if( pDestObj->pOLTxnKey == Sdf_co_null )
/*w534*/(void)sip_initSipTxnKey( &(pDestObj->pOLTxnKey), \
						(SipError *)&(pErr->stkErrCode));

		if (sip_cloneSipTxnKey(pDestObj->pOLTxnKey, pSrcObj->pOLTxnKey, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transactionAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneOverlapTransInfo(): " 
					"Cloning the SIP Transaction key structure failed.", pErr);
#endif
				pErr->errCode=Sdf_en_overlapTransInfoAccessError;
				return Sdf_co_fail;
		}
	}
#endif

	pDestObj->dTxnId = pSrcObj->dTxnId;
	pDestObj->pOverlapViaBranch = 	\
			Sdf_mc_strdupAccessor( pSrcObj->pOverlapViaBranch );

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneOverlapTransInfo");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneTxnToOverLapTxn
 **
 ** DESCRIPTION: This function copies the 
 **				 UAS/UAC Transaction structure to Overlap Transaction 
 **					Structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneTxnToOverLapTxn
#ifdef ANSI_PROTO
	(Sdf_st_overlapTransInfo *pDestTransaction,
	Sdf_st_transaction *pSrcTransaction,
	Sdf_st_error *pErr)
#else
	(pDestTransaction,pSrcTransaction, pErr )
	 Sdf_st_overlapTransInfo *pDestTransaction;
	 Sdf_st_transaction *pSrcTransaction;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneTxnToOverLapTxn");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneTxnToOverLapTxn():Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn():  "
			"pDestTransaction Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn():  "
			"pSrcTransaction Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestTransaction->dState = pSrcTransaction->dTransactionState;

	if (pSrcTransaction->pSipMsg != Sdf_co_null)
	{
		if (sip_initSipMessage(&(pDestTransaction->pSipMsg), \
			pSrcTransaction->pSipMsg->dType, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): "
				"Initializing SIP message in transaction object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		if (sip_cloneSipMessage(pDestTransaction->pSipMsg, \
			pSrcTransaction->pSipMsg, \
			(SipError*)&(pErr->stkErrCode))== SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): "
				"Cloning SIP message from source to destination failed ",\
				pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
	}

	/* 
	 * Clone list of Processed headers
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slProcessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slProcessedHeaders), \
		&dListIterator, pErr);

	/* 
	 * Clone TransportInfo structure
	 */
	if(pDestTransaction->pDestInfo == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pDestTransaction->\
			pDestInfo),pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Initializing transport info in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pDestTransaction->pDestInfo, \
		pSrcTransaction->pDestInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
			"Cloning transport info failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Clone the dSockfd
	 */
	pDestTransaction->dSocket.dSockfd=pSrcTransaction->dSocket.dSockfd;


	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Initializing HeaderList for Processed Headers failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Cloning Processed Headers List failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slProcessedHeaders), \
			(Sdf_ty_pvoid) pClonedHdrList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Appending ProcessedHeader to slProcessedHeaders of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	
	/* 
	 * Clone list of Unprocessed headers
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slUnprocessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slUnprocessedHeaders), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Initializing HeaderList for Unprocessed headers failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Cloning Unprocessed Headers List failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slUnprocessedHeaders), \
			(Sdf_ty_pvoid) pClonedHdrList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Appending UnprocessedHeader to slUnprocessedHeaders of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	/* 
	 * Clone list of Message Bodies 
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slMsgBodyList), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipMsgBody *pSipMsgBody;
		SipMsgBody *pClonedMsgBody;

		pSipMsgBody = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		if(sip_initSipMsgBody(&pClonedMsgBody, SipBodyAny,\
		 	(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Initializing message body for slMsgBodyList failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sip_cloneSipMsgBody(pClonedMsgBody, pSipMsgBody,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Cloning message body for slMsgBodyList failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slMsgBodyList), (Sdf_ty_pvoid)\
	 		pClonedMsgBody,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
				"Appending MessageBody to slMsgBodyList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}


#ifdef SDF_TXN
	if( pSrcTransaction->pTxnKey != Sdf_co_null )
	{
		if( pDestTransaction->pRprTxnKey == Sdf_co_null )
/*w534*/(void)sip_initSipTxnKey( &(pDestTransaction->pRprTxnKey), \
						(SipError *)&(pErr->stkErrCode));

		if (sip_cloneSipTxnKey(pDestTransaction->pRprTxnKey, \
			 pSrcTransaction->pTxnKey, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transactionAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTxnToOverLapTxn(): " 
					"Cloning the SIP Transaction key structure failed.", pErr);
#endif
				pErr->errCode=Sdf_en_transactionAccessError;
				return Sdf_co_fail;
		}
	}
	pDestTransaction->pOLTxnKey = Sdf_co_null;
#endif
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneTxnToOverLapTxn");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMsgBody
 **
 ** DESCRIPTION: This function clones the Sdf_st_msgBody structure
 **				 from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMsgBody
#ifdef ANSI_PROTO 
(Sdf_st_msgBody *pDestObj,
Sdf_st_msgBody *pSrcObj,
Sdf_st_error *pErr)
#else
(pDestObj,pSrcObj, pErr )
 Sdf_st_msgBody *pDestObj;
 Sdf_st_msgBody *pSrcObj;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaCloneMsgBody");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneMsgBody (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBody():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBody():  "
			"pSrcObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (pDestObj->pMsgBody == Sdf_co_null)
	{
		if(sip_initSipMsgBody(&(pDestObj->pMsgBody), SipBodyAny,\
		 	(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBody(): " 
				"Initializing SipMsgBody failed ",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}
	if(sip_cloneSipMsgBody(pDestObj->pMsgBody, pSrcObj->pMsgBody,\
		(SipError *)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
			"Cloning message body list failed ",pErr);
#endif
		return Sdf_co_fail;
	}

	if (pDestObj->pCorrelation != Sdf_co_null)
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, \
			(Sdf_ty_pvoid*)&(pDestObj->pCorrelation),pErr);
	
	pDestObj->pCorrelation = Sdf_mc_strdupAccessor(pSrcObj->pCorrelation);
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaCloneMsgBody");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneConfigExtraHeaders
 **
 ** DESCRIPTION: This function copies the config extra headers structure 
 **				 from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneConfigExtraHeaders
#ifdef ANSI_PROTO 
(Sdf_st_configExtraHeaders *pDestObj,
Sdf_st_configExtraHeaders *pSrcObj,
Sdf_st_error *pErr)
#else
(pDestObj,pSrcObj, pErr )
 Sdf_st_configExtraHeaders *pDestObj;
 Sdf_st_configExtraHeaders *pSrcObj;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneConfigExtraHeaders");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneConfigExtraHeaders (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneConfigExtraHeaders():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneConfigExtraHeaders():  "
			"pSrcObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(pDestObj->pParam == Sdf_co_null)
	{
		if(sip_initSipParam(&(pDestObj->pParam), (SipError *)\
			&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_extraHeaderAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneConfigExtraHeaders():  "
				"Initializing SipParam failed ",pErr);
#endif
			pErr->errCode=Sdf_en_extraHeaderAccessError;
			return Sdf_co_fail;
		}
	}
	if(sip_cloneSipParam(pDestObj->pParam, pSrcObj->pParam, (SipError *)\
		&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_extraHeaderAccessError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneConfigExtraHeaders():  "
			"Cloning SipParam failed ",pErr);
#endif
		pErr->errCode=Sdf_en_extraHeaderAccessError;
		return Sdf_co_fail;
	}

	Sdf_mc_strdup(pDestObj->pData,(Sdf_ty_s8bit *)pSrcObj->pData);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneConfigExtraHeaders");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneInitData
 **
 ** DESCRIPTION: This function copies the init data structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneInitData
#ifdef ANSI_PROTO 
	(Sdf_st_initData *pDestObj,
	Sdf_st_initData *pSrcObj,
	Sdf_st_error *pErr)
#else
	(pDestObj,pSrcObj, pErr )
	 Sdf_st_initData *pDestObj;
	 Sdf_st_initData *pSrcObj;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneInitData");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneInitData (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Clone From Header
	 */
	if(pDestObj->pFrom == Sdf_co_null)
	{
		if(sip_initSipHeader(&(pDestObj->pFrom), SipHdrTypeFrom, (SipError *)\
			&(pErr->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing From header in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sip_cloneSipHeader(pDestObj->pFrom, pSrcObj->pFrom, (SipError *)\
		&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
			"Cloning From header failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Clone the tag
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)&(pDestObj->pTag),pErr);
	if(pSrcObj->pTag == Sdf_co_null)
		pDestObj->pTag = Sdf_co_null;
	else
		pDestObj->pTag = Sdf_mc_strdupAccessor(pSrcObj->pTag);

	/* 
	 * Clone Registrar transport info
	 */
	if(pDestObj->pRegistrar == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pDestObj->pRegistrar),\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing Registrar transport info failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pDestObj->pRegistrar, pSrcObj->pRegistrar,\
	 	pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
			"Cloning Registrar transport info failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * Clone TransportInfo structure
	 */
	if(pDestObj->pTransport == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pDestObj->pTransport),pErr) == \
				Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing transport info in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pDestObj->pTransport, pSrcObj->pTransport,\
	 	pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
			"Cloning transport info failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}

	/* Clone slContactTransport  list present in Init Data */
	(void)sdf_listDeleteAll(&(pDestObj->slContactTransport), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slContactTransport), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_transportInfo *pSrcTransInfo = Sdf_co_null;
		Sdf_st_transportInfo *pDestTransInfo = Sdf_co_null;

		pSrcTransInfo = (Sdf_st_transportInfo*)\
							(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitTransportInfo(&pDestTransInfo,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing transport info in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
		
		if(sdf_ivk_uaCloneTransportInfo(pDestTransInfo, pSrcTransInfo, \
			 	pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Cloning transport info failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}

		if(sdf_listAppend(&(pDestObj->slContactTransport), (Sdf_ty_pvoid)\
	 		pDestTransInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Appending the Dest Transport Info failed",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	
	/* 
	 * Clone Contact header list
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slContact), pErr);

	if (sdf_fn_uaCloneSipHeadersList(&(pDestObj->slContact), \
		&(pSrcObj->slContact), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData():  "
			"Failed to clone list of Contact headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
	}

	/* 
	 * Clone list of message bodies
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slMsgBodyList), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipMsgBody *pSipMsgBody;
		SipMsgBody *pClonedMsgBody;

		pSipMsgBody = (SipMsgBody*)(dListIterator.pCurrentElement->pData);

		if(sip_initSipMsgBody(&pClonedMsgBody, SipBodyAny,\
		 	(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing MessageBody failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sip_cloneSipMsgBody(pClonedMsgBody, pSipMsgBody,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Cloning message body list failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slMsgBodyList), (Sdf_ty_pvoid)\
	 		pClonedMsgBody,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Appending MessageBody to slMsgBodyList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/*Clone Session Param*/
	if(sdf_ivk_uaCloneSessionParam(pDestObj->pSessionParam, \
		pSrcObj->pSessionParam, 	pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
			"Cloning SessionParam failed ",pErr);
#endif
		return Sdf_co_fail;
	}

	/* 
	 * Clone list of extra headers
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slExtraHeadersList), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slExtraHeadersList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pExtraHdrs;
		Sdf_st_configExtraHeaders *pClonedExtraHdrs;

		pExtraHdrs =\
			(Sdf_st_configExtraHeaders*)(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitConfigExtraHeaders(&pClonedExtraHdrs,\
		 	pErr) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing ExtraHeaders failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneConfigExtraHeaders(pClonedExtraHdrs, pExtraHdrs,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Cloning ExtraHeader failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slExtraHeadersList), (Sdf_ty_pvoid)\
	 		pClonedExtraHdrs,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Appending ExtraHeader to slExtraHeadersList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

    #ifdef SDF_IMS  
  (void)sdf_listDeleteAll(&(pDestObj->slRespHeadersList), pErr);

	(void)sdf_listInitIterator(&(pSrcObj->slRespHeadersList), \
		&dListIterator, pErr);

  while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_configExtraHeaders *pRespHdrs;
		Sdf_st_configExtraHeaders *pClonedRespHdrs;

		pRespHdrs =\
			(Sdf_st_configExtraHeaders*)(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitConfigExtraHeaders(&pClonedRespHdrs,\
		 	pErr) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Initializing ExtraHeaders failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneConfigExtraHeaders(pClonedRespHdrs, pRespHdrs,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Cloning ExtraHeader failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slRespHeadersList), (Sdf_ty_pvoid)\
	 		pClonedRespHdrs,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData(): " 
				"Appending ExtraHeader to slExtraHeadersList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
#endif
	/* 
	 * Clone Preloaded route  header list
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slPreLoadedRoute), pErr);

	if (sdf_fn_uaCloneSipHeadersList(&(pDestObj->slPreLoadedRoute), \
		&(pSrcObj->slPreLoadedRoute), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneInitData():  "
			"Failed to clone list of Route  headers from source to"
			"destination", pErr);	
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneInitData");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************
** Function:  sdf_ivk_uaCloneEventContext
** Description: This function Makes a deep copy of the EventContext
**				structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneEventContext
#ifdef ANSI_PROTO
(Sdf_st_eventContext	*pDestObj, 
Sdf_st_eventContext 	*pSrcObj, 
Sdf_st_error 			*pErr)
#else
	(pDestObj, pSrcObj, pErr)
	Sdf_st_eventContext *pDestObj;
	Sdf_st_eventContext  *pSrcObj;
	Sdf_st_error 		 *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-"
		"Entering sdf_ivk_uaCloneEventContext");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneEventContext (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneEventContext():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneEventContext():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	pDestObj->dOptions = pSrcObj->dOptions;
    pDestObj->pDirectBuffer = pSrcObj->pDirectBuffer;

	/* TODO Clone header type list, DirectBuffer and Transport Addr structure */

	pDestObj->dRemainingLength = pSrcObj->dRemainingLength;
	pDestObj->dNextMessageLength = pSrcObj->dNextMessageLength;

#ifndef SDF_TXN	
	pDestObj->dRetransT1 				= pSrcObj->dRetransT1;
	pDestObj->dRetransT2 				= pSrcObj->dRetransT2;
	pDestObj->dMaxRetransCount 			= pSrcObj->dMaxRetransCount;
	pDestObj->dMaxInviteRetransCount 	= pSrcObj->dMaxInviteRetransCount;
#endif
	
#ifdef SDF_SESSION_TIMER
	pDestObj->dReinviteTimeout 	= pSrcObj->dReinviteTimeout;
#endif
	
	pDestObj->dProtocol 			= pSrcObj->dProtocol;
	pDestObj->dPreallocMsgBufIndex 	= pSrcObj->dPreallocMsgBufIndex;
	pDestObj->dSdfDisabledChecks 	= pSrcObj->dSdfDisabledChecks;

#ifdef SDF_TXN
	pDestObj->dTimeoutValues.dT1 		= pSrcObj->dTimeoutValues.dT1;
	pDestObj->dTimeoutValues.dT2 		= pSrcObj->dTimeoutValues.dT2;
	pDestObj->dTimeoutValues.dTimerB	= pSrcObj->dTimeoutValues.dTimerB;
	pDestObj->dTimeoutValues.dTimerC	= pSrcObj->dTimeoutValues.dTimerC;
	pDestObj->dTimeoutValues.dTimerD_T3 = pSrcObj->dTimeoutValues.dTimerD_T3;
	pDestObj->dTimeoutValues.dTimerF_T3 = pSrcObj->dTimeoutValues.dTimerF_T3;
	pDestObj->dTimeoutValues.dTimerH 	= pSrcObj->dTimeoutValues.dTimerH;
	pDestObj->dTimeoutValues.dTimerI_T4 = pSrcObj->dTimeoutValues.dTimerI_T4;
	pDestObj->dTimeoutValues.dTimerJ_T3 = pSrcObj->dTimeoutValues.dTimerJ_T3;
	pDestObj->dTimeoutValues.dTimerK_T4 = pSrcObj->dTimeoutValues.dTimerK_T4;
	pDestObj->dTimerOption				= pSrcObj->dTimerOption;
	pDestObj->dTimeoutCbkOption			= pSrcObj->dTimeoutCbkOption;
	pDestObj->dNoAnswerTimerDuration	= pSrcObj->dNoAnswerTimerDuration;
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
  if(pSrcObj->pSubsObj != Sdf_co_null)
  {
    pDestObj->pSubsObj = pSrcObj->pSubsObj;
    HSS_LOCKEDINCREF(pDestObj->pSubsObj->dRefCount);
  }
  else
    pDestObj->pSubsObj= Sdf_co_null;

  pDestObj->dIsRestartTmr = pSrcObj->dIsRestartTmr; 
#endif

	if( pSrcObj->pData != Sdf_co_null)
	{
		if(sdf_cbk_uaCloneEventContextpData(pDestObj, pSrcObj, pErr) \
				== Sdf_co_fail)
			pDestObj->pData = Sdf_co_null;
	}
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitEventContext");	
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetTransactionFromCallObject
 **
 ** DESCRIPTION: This function returns a pointer to the appropriate
 **					transaction sub-structure of the call object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTransactionFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, Sdf_ty_transactionType dType, \
	Sdf_st_transaction **ppTransaction, Sdf_st_error *pErr)
#else
	(pObject, dType,ppTransaction, pErr)
	Sdf_st_callObject *pObject;
    Sdf_ty_transactionType dType;
	Sdf_st_transaction **ppTransaction;
	Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaGetTransactionFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetTransactionFromCallObject() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}

	if(ppTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransactionFromCallObject() : "
			"Invalid transaction pointer passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransactionFromCallObject() : "
			"Invalid call-object parameter passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	*ppTransaction=Sdf_co_null;

	if (dType == Sdf_en_uasTransaction)
	{
		if(pObject->pUasTransaction == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransactionFromCallObject() : "
				"No UAS transaction in call-object",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
		*ppTransaction = pObject->pUasTransaction;
		HSS_LOCKEDINCREF(pObject->pUasTransaction->dRefCount);
	}
	else
	{
		if(pObject->pUacTransaction == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetTransactionFromCallObject() : "
				"No UAC transaction in call-object",pErr);
#endif
			pErr->errCode=Sdf_en_invalidParamError;
			return Sdf_co_fail;
		}
		*ppTransaction = pObject->pUacTransaction;
		HSS_LOCKEDINCREF(pObject->pUacTransaction->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaGetTransactionFromCallObject");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaSetTransactionInCallObject
 **
 ** DESCRIPTION: This function sets the transaction structure into the
 **				 appropriate (UAC/UAS) transaction of the Call Object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetTransactionInCallObject 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, 
	Sdf_ty_transactionType dType, 
	Sdf_st_transaction *pTransaction,
	Sdf_st_error *pError)
#else
	(pObject, dType, pTransaction, pError)
	 Sdf_st_callObject *pObject; 
	 Sdf_ty_transactionType dType;
	 Sdf_st_transaction *pTransaction;
	 Sdf_st_error *pError;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetTransactionInCallObject");


	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetTransactionInCallObject():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTransactionInCallObject():  "
			"Transaction Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetTransactionInCallObject():  "
			"Call Object param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Free the existing transaction in the call object
	 * before pointing it to the new data
	 */

	if (dType == Sdf_en_uacTransaction)
	{
		if (pObject->pUacTransaction != Sdf_co_null)
			(void)sdf_ivk_uaFreeTransaction(pObject->pUacTransaction);
		pObject->pUacTransaction = pTransaction;
	}
	else
	{
		if (pObject->pUasTransaction != Sdf_co_null)
			(void)sdf_ivk_uaFreeTransaction(pObject->pUasTransaction);
		pObject->pUasTransaction = pTransaction;
	}
	HSS_LOCKEDINCREF(pTransaction->dRefCount);


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetTransactionInCallObject");
	pError->errCode = Sdf_en_noUaError;
 
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneCallObject
 **
 ** DESCRIPTION: This function copies the 
 **				 CallObject structure from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneCallObject
#ifdef ANSI_PROTO 
	(Sdf_st_callObject *pDestObj,
	Sdf_st_callObject *pSrcObj,
	Sdf_st_error *pErr)
#else
	(pDestObj,pSrcObj, pErr )
	 Sdf_st_callObject *pDestObj;
	 Sdf_st_callObject *pSrcObj;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;
	Sdf_st_error error;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneCallObject (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject():  "
			"pDestObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject():  "
			"pSrcObj Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (Sdf_co_null == (pDestObj->pCommonInfo))
	{
		if (sdf_ivk_uaInitCommonInfo(&(pDestObj->pCommonInfo), pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject():  "
				"Initializing CommonInfo in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}

#ifdef SDF_SUBSCRIPTION_LAYER
  pDestObj->dDialogUsageCount = pSrcObj->dDialogUsageCount;

	/* 
	 * Clone list of Subscription Objects
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slSubsObjList), &error);

	(void)sdf_listInitIterator(&(pSrcObj->slSubsObjList), &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_uaSLSubscriptionObject *pSubsObj       = Sdf_co_null;
		Sdf_st_uaSLSubscriptionObject *pClonedSubsObj = Sdf_co_null;

		pSubsObj = (Sdf_st_uaSLSubscriptionObject*)\
                                     (dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaSLInitSubsObject(&pClonedSubsObj, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing SubsObject failed.",pErr);
#endif
			return Sdf_co_fail;
		}

		if (sdf_ivk_uaSLCloneSubsObject(pClonedSubsObj, pSubsObj,\
                                    pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Cloning SubsObj failed ", pErr);
#endif
      sdf_ivk_uaSLFreeSubsObject(pClonedSubsObj);
			return Sdf_co_fail;
		}

		if(sdf_listAppend(&(pDestObj->slSubsObjList), (Sdf_ty_pvoid)\
                      pClonedSubsObj, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Appending SubsObj to slSubsObjList of destination failed ", pErr);
#endif
      sdf_ivk_uaSLFreeSubsObject(pClonedSubsObj);
			pErr->errCode = Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		(void)sdf_listNext(&dListIterator, pErr);
	} /* End of while.. */
#endif /* End of SDF_SUBSCRIPTION_LAYER */

	if (sdf_ivk_uaCloneCommonInfo(pDestObj->pCommonInfo, \
		pSrcObj->pCommonInfo, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject():  "
			"Cloning CommonInfo from source to destination failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

#ifdef SDF_SESSION_TIMER
	if (sdf_ivk_uaCloneSessionTimerInfo(pDestObj->pSessionTimer, \
		pSrcObj->pSessionTimer, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject():  "
			"Cloning SessionTimerInfo from source to destination failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
#endif


	if(Sdf_co_null == (pDestObj->pUacTransaction))
	{
		if (sdf_ivk_uaInitTransaction(&(pDestObj->pUacTransaction),pErr)\
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing UAC Transaction in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	if (sdf_ivk_uaCloneTransaction(pDestObj->pUacTransaction, \
		pSrcObj->pUacTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Cloning UAC Transaction in call object failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
	
	if(Sdf_co_null == (pDestObj->pUasTransaction))
	{
		if (sdf_ivk_uaInitTransaction(&(pDestObj->pUasTransaction),pErr)\
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing UAS Transaction in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	if (sdf_ivk_uaCloneTransaction(pDestObj->pUasTransaction, \
		pSrcObj->pUasTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Cloning UAC Transaction in call object failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	if(Sdf_co_null == (pDestObj->pCallInfo))
	{
		if (sdf_ivk_uaInitCallInfo(&(pDestObj->pCallInfo),pErr)\
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing CallInfo in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	pDestObj->pCallInfo->dState = pSrcObj->pCallInfo->dState;
	pDestObj->pCallInfo->dInviteInProg = pSrcObj->pCallInfo->dInviteInProg;

	if(Sdf_co_null == (pDestObj->pAppData))
	{
		if (sdf_ivk_uaInitAppData(&(pDestObj->pAppData), pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing Appdata in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces, \
		(Sdf_ty_s8bit*)"Issuing callback sdf_cbk_uaCloneAppData", 0, pErr);
#endif
	/*
	 * Issue the sdf_cbk_uaCloneAppData to the application in order to clone 
	 * the pData part of the pAppData. This is because it is a void pointer that
	 * is set by the application..and hence only the application has knowledge
	 * of the data structure and how to clone it.
	 */
	if (sdf_cbk_uaCloneAppData(pDestObj->pAppData, \
		pSrcObj->pAppData, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Cloning Appdata from source to destination Failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Clone the user profile
	 */
	if(Sdf_co_null == pDestObj->pInitData)
	{
		if(sdf_ivk_uaInitInitData(&(pDestObj->pInitData), pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing user profile in call object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
	}
	
	if(sdf_ivk_uaCloneInitData(pDestObj->pInitData, pSrcObj->pInitData, pErr)\
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Cloning user profile from source to destination failed ",pErr);
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/* 
	 * Now clone the lists. Each element in the list must be cloned
	 */
	/* 
	 * Clone list of overlap transactions
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slOverlapTransInfo), &error);

	(void)sdf_listInitIterator(&(pSrcObj->slOverlapTransInfo), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo *pOverlapTransInfo;
		Sdf_st_overlapTransInfo *pClonedTransInfo;

		pOverlapTransInfo = (Sdf_st_overlapTransInfo*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitOverlapTransInfo(&pClonedTransInfo, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing OverlapTransInfo failed.",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneOverlapTransInfo(pClonedTransInfo, pOverlapTransInfo,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Cloning OverlapTransInfo failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slOverlapTransInfo), (Sdf_ty_pvoid)\
	 		pClonedTransInfo, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Appending OverlapTransInfo to slOverlapTransInfo of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	/* 
	 * Clone list of Persistent headers
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slPersistentHeaders), &error);

	(void)sdf_listInitIterator(&(pSrcObj->slPersistentHeaders), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList =(Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing HeaderList for Persistent Headers failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Cloning Persistent Headers List failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slPersistentHeaders), (Sdf_ty_pvoid)\
	 		pClonedHdrList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Appending PersistentHeader to slPersistentHeaders of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	/* 
	 * Clone list of persistent message bodies 
	 */
	(void)sdf_listDeleteAll(&(pDestObj->slPersistentMsgBodyList), &error);

	(void)sdf_listInitIterator(&(pSrcObj->slPersistentMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_msgBody *pMsgBody;
		Sdf_st_msgBody *pClonedMsgBody;

		pMsgBody = (Sdf_st_msgBody*)(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitMsgBody(&pClonedMsgBody, pMsgBody->pCorrelation,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Initializing message body for PersistentMsgBody list failed",\
				pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneMsgBody(pClonedMsgBody, pMsgBody, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Cloning message body for PersistentMsgBody list failed ",\
				pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestObj->slPersistentMsgBodyList), (Sdf_ty_pvoid)\
	 		pClonedMsgBody, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
				"Appending message body to slPersistentMsgBodyList of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/*Clone Media Handling Structure*/
	if(sdf_ivk_uaInitMediaHandling(&(pDestObj->pMediaInfo),pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Initializing Media Handling Structure failed",\
			pErr);
#endif
		return Sdf_co_fail;
	}
	if(sdf_ivk_uaCloneMediaHandling(pDestObj->pMediaInfo,pSrcObj->pMediaInfo,\
						pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCallObject(): " 
			"Cloning Media Handling Structure failed ",\
			pErr);
#endif
			return Sdf_co_fail;
	}
    pDestObj->dCallObjectType = pSrcObj->dCallObjectType;
	pDestObj->dCallObjId = pSrcObj->dCallObjId;

#ifdef SDF_THREAD_SAFE
	/*
	 * Initialize the mutexes in the same state (lock/unlock) as they are in 
	 * the source
	 */
	sdf_fn_uaInitMutex(&(pDestObj->dCallObjectMutex), Sdf_co_true);
	sdf_fn_uaInitMutex(&(pDestObj->dCallStateMutex), Sdf_co_false);
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneCallObject");

	pErr->errCode = Sdf_en_noUaError;
 
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneHeaderList
 **
 ** DESCRIPTION: This function copies the HeaderList structure from src 
 **				 to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneHeaderList
#ifdef ANSI_PROTO 
	(Sdf_st_headerList *pDestInfo,
	Sdf_st_headerList *pSrcInfo,
	Sdf_st_error *pErr)
#else
	(pDestInfo,pSrcInfo, pErr )
	 Sdf_st_headerList *pDestInfo;
	 Sdf_st_headerList *pSrcInfo;
	 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneHeaderList");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneHeaderList (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneHeaderList():  "
			"pDestInfo Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneHeaderList():  "
			"pSrcInfo Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Iterate through the list of headers in the source and clone them into 
	 * the destination headerList
	 */
	(void)sdf_listDeleteAll(&(pDestInfo->slHeaders), pErr);

	if (sdf_fn_uaCloneSipHeadersList(&(pDestInfo->slHeaders), \
		&(pSrcInfo->slHeaders), pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_commonInfoAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneHeaderList():  "
			"Failed to clone list of headers from source to destination", \
			pErr);
#endif
		pErr->errCode=Sdf_en_commonInfoAccessError;
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneHeaderList");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetInitDataFromCallObject
 **
 ** DESCRIPTION: This function returns the InitData Structure from the
 **				 Call Object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetInitDataFromCallObject 
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject, Sdf_st_initData **ppInitData, Sdf_st_error *pError)
#else
(pObject, ppInitData, pError)
 Sdf_st_callObject *pObject; 
 Sdf_st_initData **ppInitData;
 Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetInitDataFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetInitDataFromCallObject(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (ppInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetInitDataFromCallObject():  "
			"ppInitData Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetInitDataFromCallObject():  "
			"Call Object param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	*ppInitData=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if (pObject->pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetInitDataFromCallObject():  "
			"Call Object does not have InitData",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	
	HSS_LOCKEDINCREF(pObject->pInitData->dRefCount);
	*ppInitData = pObject->pInitData;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetInitDataFromCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaSetInitDataInCallObject
 **
 ** DESCRIPTION: This function sets the InitData Structure into the
 **				 Call Object
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetInitDataInCallObject 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, Sdf_st_initData *pInitData, \
	Sdf_st_error *pError)
#else
	(pObject, pInitData, pError)
	 Sdf_st_callObject *pObject; 
	 Sdf_st_initData *pInitData;
	 Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetInitDataInCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetInitDataInCallObject():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetInitDataInCallObject():  "
			"InitData Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetInitDataInCallObject():  "
			"Call Object param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Free the existing user profile in the call object
	 * before pointing it to the new data
	 */
	if (pObject->pInitData != Sdf_co_null)
		(void)sdf_ivk_uaFreeInitData(pObject->pInitData);

	(pObject->pInitData) = pInitData;
	HSS_LOCKEDINCREF(pInitData->dRefCount);

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetInitDataInCallObject");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetFromInfoFromInitData
 **
 ** DESCRIPTION: This function returns the From Info from the initData 
 **				 structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetFromInfoFromInitData 
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	Sdf_st_nameInfo **ppFrom, 
	Sdf_st_error *pError)
#else
	(pInitData, ppFrom, pError)
	 Sdf_st_initData *pInitData;
	 Sdf_st_nameInfo **ppFrom; 
	 Sdf_st_error *pError;
#endif
{
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	SipUrl *pSipUrl = Sdf_co_null;
	Sdf_ty_u32bit urlParamCount=0, extParamCount=0, tagCount=0, iterator=0;
	Sdf_ty_s8bit *pUserName=Sdf_co_null,*pHost= Sdf_co_null;
	Sdf_ty_s8bit *pDispName = Sdf_co_null;
	Sdf_ty_s8bit *pScheme=Sdf_co_null;
	Sdf_ty_s8bit *pUri=Sdf_co_null,*pAddress=Sdf_co_null;
	Sdf_ty_s8bit dTempUri[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_bool	 dIsTelUrl = Sdf_co_false;
	Sdf_ty_bool	 dIsImUrl   = Sdf_co_false;
	Sdf_ty_bool	 dIsPresUrl   = Sdf_co_false;
	en_AddrType addrType;
	Sdf_ty_u32bit userHostLen = 0;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetFromInfoFromInitData");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetFromInfoFromInitData(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"InitData Param passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppFrom == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"ppFrom NameInfo param passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	*ppFrom=Sdf_co_null;
	/* ================ Parameter validations end ================== */

	if(pInitData->pFrom == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"InitData Param does not have a from header ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize a From NameInfo structure to return the information
	 */
	if(sdf_ivk_uaInitNameInfo(ppFrom, pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"Initialization of NameInfo failed",pError);
#endif
		return Sdf_co_fail;
	}

	if(sip_getAddrSpecFromFromHdr(pInitData->pFrom, &pAddrSpec,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"Failed to extract addrspec from From header",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
		return Sdf_co_fail;
	}

	if(sip_getAddrTypeFromAddrSpec(pAddrSpec, &addrType, \
		(SipError *)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"Failed to extract type from AddrSpec",pError);
#endif
		(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
		sip_freeSipAddrSpec(pAddrSpec);
		return Sdf_co_fail;
	}
	/* 
	 * Now start filling in stuff into name-info
	 */

	/*handle sip: and tel: seperately*/
#ifdef SDF_TLS
	if((addrType != SipAddrSipUri) && (addrType != SipAddrSipSUri))
#else
	if(addrType != SipAddrSipUri)
#endif
	{
#ifdef SDF_TEL
		if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode))==SipSuccess)
			dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
		if( dIsTelUrl == Sdf_co_false )
		{
			if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsImUrl = Sdf_co_true;
		}
#endif
#ifdef SDF_PRES
		if((dIsTelUrl == Sdf_co_false)&&
            (dIsImUrl == Sdf_co_false))
		{
			if(sip_isPresUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
					==SipSuccess)
				dIsPresUrl = Sdf_co_true;
		}
#endif        
		if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true) &&
           (dIsPresUrl != Sdf_co_true))
		{
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"From Header Contains Unsupported URL Scheme",pError);
#endif
			pError->errCode=Sdf_en_notImplemented;
			return Sdf_co_fail;
		}
		if(sip_getUriFromAddrSpec(pAddrSpec,&pUri,\
				(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to Get Uri From AddrSpec",pError);
#endif
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		userHostLen += Sdf_mc_strlen(pUri);

		(*ppFrom)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
		userHostLen+2,pError);
		if((*ppFrom)->pAddress == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}
		/*Copy Uri into the Address.Before Cpy remove the tel: from it.*/
		Sdf_mc_strcpy(dTempUri,pUri);
		pScheme = Sdf_mc_strtokr(dTempUri,":",&pAddress);
		Sdf_mc_strcpy((*ppFrom)->pAddress,pAddress);
	}
	else
	{
		if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl,\
			pError) == Sdf_co_fail)
		{
			pError->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Could not extract SIP URL from From header",pError);
#endif
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

/*w534*/(void)sip_getUserFromUrl(pSipUrl,&pUserName,(SipError*)&(pError->stkErrCode));

		if(pUserName != Sdf_co_null)
			userHostLen += Sdf_mc_strlen(pUserName);

/*w534*/(void)sip_getHostFromUrl(pSipUrl, &pHost, (SipError *)&(pError->stkErrCode));

		userHostLen += Sdf_mc_strlen(pHost);
	
		(*ppFrom)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
			userHostLen+2,pError);

		if((*ppFrom)->pAddress == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
			pError->errCode=Sdf_en_noMemoryError;
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		/*
		 * Copy the user@host portion into ppFrom->pAddress
		 */
		Sdf_mc_strcpy((*ppFrom)->pAddress,"");
		if ((pUserName!=Sdf_co_null) && (Sdf_mc_strcmp(pUserName, "") != 0))
		{
			Sdf_mc_strcat((*ppFrom)->pAddress, pUserName);
			Sdf_mc_strcat((*ppFrom)->pAddress, "@");
		}
		Sdf_mc_strcat((*ppFrom)->pAddress, pHost);

		if(sip_getPortFromUrl(pSipUrl, &((*ppFrom)->dPort), \
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_TLS
			if (addrType == SipAddrSipSUri)
			    (*ppFrom)->dPort = Sdf_mc_defaultSipSPort;
			else
#endif
			    (*ppFrom)->dPort = Sdf_mc_defaultSipPort;
		}
	
		/* 
		 * Username, host and port extracted. Get URL params now 
		 */
		if (sip_getUrlParamCountFromUrl(pSipUrl, &urlParamCount, \
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to get count of Url params from URL",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}


		for(iterator = 0; iterator < urlParamCount; iterator++)
		{
			SipParam *pParam;
			if (sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, iterator, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
					"Failed to get URL param at index from URL",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			if (sdf_listAppend(&((*ppFrom)->slUrlParams), pParam, \
					pError)== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
					"Failed to append URL param to destination",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				(void)sdf_ivk_uaFreeNameInfo(*ppFrom);
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
		}
		sip_freeSipUrl(pSipUrl);
	}

	/* Free the local reference */
	sip_freeSipAddrSpec(pAddrSpec);
	/* 
	 * All URL related information extracted. Now get the header parameters
	 */

	/* 
	 * For the tag we get the first tag from the list in the header
	 */
/*w534*/(void)sip_getTagCountFromFromHdr(pInitData->pFrom, &tagCount, \
		(SipError *)&(pError->stkErrCode));

	if(tagCount != 0)
	{
		Sdf_ty_s8bit *pTag;
/*w534*/(void)sip_getTagAtIndexFromFromHdr(pInitData->pFrom, &pTag, 0, \
			(SipError *)&(pError->stkErrCode));
		Sdf_mc_strdup((*ppFrom)->pTag,pTag);
	}
	
	/*
	 * Extract the display name
	 */
	if(sip_getDispNameFromFromHdr(pInitData->pFrom, &pDispName, \
		(SipError *)&(pError->stkErrCode)) == SipFail)
		(*ppFrom)->pName = Sdf_co_null;
	else
	{
		Sdf_mc_strdup((*ppFrom)->pName,pDispName);
	}
	
	/*Fill the scheme*/
	if(addrType == SipAddrSipUri)
		Sdf_mc_strdup((*ppFrom)->pScheme,"sip");
#ifdef SDF_TLS
	else if(addrType==SipAddrSipSUri)
		Sdf_mc_strdup((*ppFrom)->pScheme,"sips");
#endif
	else
		Sdf_mc_strdup((*ppFrom)->pScheme,pScheme);

	/*Fill Extension Params*/
	if (sip_getExtensionParamCountFromFromHdr(pInitData->pFrom, &extParamCount,\
		(SipError *)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
			"Failed to get count of Extension Params from From header",pError);
#endif
		pError->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	for(iterator = 0; iterator < extParamCount; iterator++)
	{
		SipParam *pParam;
		if (sip_getExtensionParamAtIndexFromFromHdr(pInitData->pFrom, \
			&pParam, iterator, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to get Extension Param at index from From header",\
				pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
		if (sdf_listAppend(&((*ppFrom)->slHeaderParams), pParam, pError) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetFromInfoFromInitData(): "
				"Failed to append Extension param to destination", pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetFromInfoFromInitData");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}		


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetRegistrarInfoFromInitData
 **
 ** DESCRIPTION: This function returns the Registrar Info from the
 **				 initData structure
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetRegistrarInfoFromInitData 
#ifdef ANSI_PROTO
(Sdf_st_initData *pInitData, 
Sdf_st_transportInfo **ppRegistrar,
Sdf_st_error *pError)
#else
(pInitData, ppRegistrar, pError)
Sdf_st_initData *pInitData; 
Sdf_st_transportInfo **ppRegistrar; 
Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetRegistrarInfoFromInitData");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetRegistrarInfoFromInitData(): Error Param Passed "
			"is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRegistrarInfoFromInitData():  "
			"InitData Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppRegistrar == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRegistrarInfoFromInitData():  "
			"Registrar Info param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */


	*ppRegistrar=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if (pInitData->pRegistrar == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRegistrarInfoFromInitData():  "
			"Init Data does not have valid Registrar Info",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	
	HSS_LOCKEDINCREF(pInitData->pRegistrar->dRefCount);
	*ppRegistrar = pInitData->pRegistrar;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetRegistrarInfoFromInitData");

	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetHeaderListByType
 **
 ** DESCRIPTION: This function returns a list of Sdf_st_headerList structure
 **				 corresponding to the given SipHeader type.
 **
 **				 Note:
 **				 The 'pslMatchedHeaderList' variable is initialized inside 
 **				 the API.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetHeaderListByType
#ifdef ANSI_PROTO
	(Sdf_ty_slist slHeaderList, en_HeaderType dType,\
	Sdf_ty_slist *pslMatchedHeaderList, Sdf_st_error *pErr)
#else
	(slHeaderList, dType, pslMatchedHeaderList, pErr)
	Sdf_ty_slist slHeaderList; 
	en_HeaderType dType;
	Sdf_ty_slist *pslMatchedHeaderList;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_headerList *pHeaderList = Sdf_co_null;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetHeaderListByType");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetHeaderListByType(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_listInit(pslMatchedHeaderList, __sdf_fn_uaFreeHeaderList, \
		Sdf_co_false,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetHeaderListByType():  "
			"Failed to  initialize list of MatchedHeaders",pErr);	
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}	
	
	/*
	 * Iterate through the slHeaderlist searching for the dType. If a 
	 * headerlist of this type is found, append it to the pslMatchedHeaderList
	 */
	(void)sdf_listInitIterator(&(slHeaderList), &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pHeaderList =(Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if (pHeaderList->dType == dType)
		{
			/*
			 * The type of this HeaderList matches. Append it to
			 * pslMatchedHeaderList 
			 */
			if (sdf_listAppend(pslMatchedHeaderList, pHeaderList, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetHeaderListByType():  "
					"Failed to append HeaderList to pslMatchedHeaderList",pErr);	
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			/*
			 * Increment the reference count, since sdf_listAppend does not do
			 * the same.
			 */
			HSS_LOCKEDINCREF(pHeaderList->dRefCount);
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaGetHeaderListByType");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	  sdf_ivk_uaGetHeaderListByCorrelation
 **
 ** DESCRIPTION:  This function returns a list of Sdf_st_headerList structure
 **				  corresponding to the given correlation string
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetHeaderListByCorrelation
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslHeaderList, 
Sdf_ty_s8bit *pCorrelation,
Sdf_ty_slist *pslMatchedHeaderList, 
Sdf_st_error *pErr)
#else
(pslHeaderList, pCorrelation, pslMatchedHeaderList, pErr)
Sdf_ty_slist *pslHeaderList; 
Sdf_ty_s8bit *pCorrelation;
Sdf_ty_slist *pslMatchedHeaderList;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_headerList *pHeaderList = Sdf_co_null;
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetHeaderListByCorrelation");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetHeaderListByCorrelation(): Error Param passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetHeaderListByCorrelation():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_listInit(pslMatchedHeaderList, __sdf_fn_uaFreeHeaderList, \
		Sdf_co_false,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetHeaderListByCorrelation():  "
			"Failed to  initialize list of matched headers",pErr);	
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}	
	
	/*
	 * Iterate through the slHeaderlist searching for pCorrelation. If a 
	 * headerlist with this correlation is found, append it to the 
	 * pslMatchedHeaderList
	 */
	(void)sdf_listInitIterator(pslHeaderList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pHeaderList =(Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if (pHeaderList->pCorrelation == Sdf_co_null)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		if (Sdf_mc_strcmp(pHeaderList->pCorrelation,pCorrelation) == 0)
		{
			/*
			 * The correlation of this HeaderList matches. Append it to
			 * pslMatchedHeaderList 
			 */
			if (sdf_listAppend(pslMatchedHeaderList, pHeaderList, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetHeaderListByCorrelation(): "
					"Failed to  append Header to pslMatchedHeaderList",pErr);	
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			/*
			 * Increment the reference count, since sdf_listAppend does not do
			 * the same.
			 */
			HSS_LOCKEDINCREF(pHeaderList->dRefCount);
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaGetHeaderListByCorrelation");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCopyMsgBodyToPersistent
 **
 ** DESCRIPTION: This function copies a SipMsgBody structure from the
 **				 source list to the destination list (PersistentMsgBodyList)
 **				 The correlation string to be set is passed as a parameter
 **				 to the function.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCopyMsgBodyToPersistent 
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcMsgBodyList, Sdf_ty_slist *pslDestMsgBodyList, \
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr)
#else
	(pslSrcMsgBodyList, pslDestMsgBodyList, pCorrelation, *pErr)
	Sdf_ty_slist *pslSrcMsgBodyList;
	Sdf_ty_slist *pslDestMsgBodyList; 
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaCopyMsgBodyToPersistent");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCopyMsgBodyToPersistent(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCopyMsgBodyToPersistent():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Iterate through the slSrcMsgBodyList and append entries with 
	 * correlation string as pCorrelation into pslDestMsgBodyList
	 */
	(void)sdf_listInitIterator(pslSrcMsgBodyList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_msgBody *pSdfMsgBody;

		pMsgBody = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		if (sdf_ivk_uaInitMsgBody(&pSdfMsgBody,pCorrelation,pErr) ==\
				Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCopyMsgBodyToPersistent():  "
				"Failed to initialize Sdf_st_msgBody structure",pErr);	
#endif
			return Sdf_co_fail;
		}

		/*
		 * Increment reference count of pMsgBody in order to store it in the
		 * Sdf_st_msgBody structure in the destination list
		 */
		HSS_LOCKEDINCREF(pMsgBody->dRefCount);
		pSdfMsgBody->pMsgBody = pMsgBody;

		if (sdf_listAppend(pslDestMsgBodyList, pSdfMsgBody, \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCopyMsgBodyToPersistent():  "
				"Failed to append Sdf_st_msgBody message body to destination"
				"list",pErr);	
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCopyMsgBodyToPersistent");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCopyMsgBodyFromPersistent
 **
 ** DESCRIPTION: This function copies message bodies from persistent list
 **				 by the correlation string to the  destination list
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCopyMsgBodyFromPersistent 
#ifdef ANSI_PROTO
 	(Sdf_ty_slist *pslSrcMsgBodyList,
	Sdf_ty_slist *pslDestMsgBodyList,
	Sdf_ty_s8bit *pCorrelation,
	Sdf_st_error *pErr)
#else
	(pslSrcMsgBodyList, pslDestMsgBodyList, pCorrelation, *pErr)
	Sdf_ty_slist *pslSrcMsgBodyList;
	Sdf_ty_slist *pslDestMsgBodyList; 
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCopyMsgBodyFromPersistent");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCopyMsgBodyFromPersistent(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCopyMsgBodyFromPersistent():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Iterate through the Persistent MsgBody list searching for pCorrelation 
	 * and append matching entries to pslDestMsgBodyList
	 */
	(void)sdf_listInitIterator(pslSrcMsgBodyList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_msgBody *pSdfMsgBody;

		pSdfMsgBody = (Sdf_st_msgBody*)\
			(dListIterator.pCurrentElement->pData);

		if (pSdfMsgBody->pCorrelation == Sdf_co_null)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		if (Sdf_mc_strcmp(pSdfMsgBody->pCorrelation,pCorrelation) == 0)
		{
			/*
			 * Correlation matches. Append this to pslDestMsgBodyList
			 */

			pMsgBody = pSdfMsgBody->pMsgBody ;
			/*
			 * Increment reference count of pMsgBody as sdf_listAppend does not
			 * do this automatically
			 */
			HSS_LOCKEDINCREF(pMsgBody->dRefCount);

			if (sdf_listAppend(pslDestMsgBodyList, pMsgBody, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCopyMsgBodyFromPersistent():  "
					"Failed to append message body to List",pErr);	
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCopyMsgBodyFromPersistent");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMsgBodyFromPersistent
 **
 ** DESCRIPTION: This function copies message bodies from persistent list
 **				 by the correlation string to the  destination list
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMsgBodyFromPersistent 
#ifdef ANSI_PROTO
 	(Sdf_ty_slist *pslSrcMsgBodyList,
	Sdf_ty_slist *pslDestMsgBodyList,
	Sdf_ty_s8bit *pCorrelation,
	Sdf_st_error *pErr)
#else
	(pslSrcMsgBodyList, pslDestMsgBodyList, pCorrelation, *pErr)
	Sdf_ty_slist *pslSrcMsgBodyList;
	Sdf_ty_slist *pslDestMsgBodyList; 
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMsgBodyFromPersistent");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneMsgBodyFromPersistent(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBodyFromPersistent():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Iterate through the Persistent MsgBody list searching for pCorrelation 
	 * and append matching entries to pslDestMsgBodyList
	 */
	(void)sdf_listInitIterator(pslSrcMsgBodyList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_msgBody *pSdfMsgBody;

		pSdfMsgBody = (Sdf_st_msgBody*)\
			(dListIterator.pCurrentElement->pData);

		if (pSdfMsgBody->pCorrelation == Sdf_co_null)
		{
			(void)sdf_listNext(&dListIterator, pErr);
			continue;
		}

		if (Sdf_mc_strcmp(pSdfMsgBody->pCorrelation,pCorrelation) == 0)
		{
			/*
			 * Correlation matches. Append this to pslDestMsgBodyList
			 */

			if(sip_initSipMsgBody(&pMsgBody,SipSdpBody,(SipError*)\
				&(pErr->stkErrCode))== SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBodyFromPersistent():  "
					"Failed to initialise the Message Body",pErr);	
#endif
				return Sdf_co_fail;
			}
			if ( __sip_cloneSipMsgBody(pMsgBody,pSdfMsgBody->pMsgBody,\
				(SipError*)&(pErr->stkErrCode)) == SipFail )
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBodyFromPersistent():  "
					"Failed to Clone the Message Body",pErr);	
#endif
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pMsgBody,Sdf_co_null);
				return Sdf_co_fail;
			}
			if (sdf_listAppend(pslDestMsgBodyList, pMsgBody, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMsgBodyFromPersistent():  "
					"Failed to append message body to List",pErr);	
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pMsgBody,Sdf_co_null);
				return Sdf_co_fail;
			}
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCloneMsgBodyFromPersistent");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}




/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaMoveMsgBodyToPersistent
 **
 ** DESCRIPTION: This function moves a Sdf_st_msgBody structure identified
 **				 by the correlation string from the source list to the 
 **				 destination list. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMoveMsgBodyToPersistent 
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcMsgBodyList, Sdf_ty_slist *pslDestMsgBodyList, \
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr)
#else
	(pslSrcMsgBodyList, pslDestMsgBodyList, pCorrelation, *pErr)
	Sdf_ty_slist *pslSrcMsgBodyList;
	Sdf_ty_slist *pslDestMsgBodyList; 
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_st_listIterator dListIterator;
	
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaMoveMsgBodyToPersistent");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMoveMsgBodyToPersistent(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyToPersistent():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*
	 * Iterate through slSrcMsgBodyList and move elements to the 
	 * PersistentMsgBody list.
	 */
	(void)sdf_listInitIterator(pslSrcMsgBodyList, &dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_msgBody *pSdfMsgBody;

		pMsgBody = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		if (sdf_ivk_uaInitMsgBody(&pSdfMsgBody,pCorrelation,pErr) ==\
			Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyToPersistent():  "
				"Failed to initialize Sdf_st_msgBody structure ",pErr);	
#endif
			return Sdf_co_fail;
		}

		/*
		 * Increment the reference count and insert the message body into the
		 * Sdf_st_msgBody structure
		 */
		HSS_LOCKEDINCREF(pMsgBody->dRefCount);
		pSdfMsgBody->pMsgBody = pMsgBody;

		if (sdf_listAppend(pslDestMsgBodyList, pSdfMsgBody, pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyToPersistent():  "
				"Failed to append Sdf_st_msgBody message body to "
				"PersistentMsgBody List",pErr);	
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/*
	 * Now delete all elements from the source list
	 */
	(void)sdf_listDeleteAll (pslSrcMsgBodyList,pErr);	

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaMoveMsgBodyToPersistent");

	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaMoveMsgBodyFromPersistent
 **
 ** DESCRIPTION: This function copies message bodies from persistent list
 **				 by the correlation string to the  destination list
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMoveMsgBodyFromPersistent 
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcMsgBodyList, 
	Sdf_ty_slist *pslDestMsgBodyList,
	Sdf_ty_s8bit *pCorrelation,
	Sdf_st_error *pErr)
#else
	(pslSrcMsgBodyList, pslDestMsgBodyList, pCorrelation, *pErr)
	Sdf_ty_slist *pslSrcMsgBodyList;
	Sdf_ty_slist *pslDestMsgBodyList; 
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	SipMsgBody *pMsgBody = Sdf_co_null;
	Sdf_ty_u32bit dSize=0;
	Sdf_ty_u32bit dIndex=0;
	
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaMoveMsgBodyFromPersistent");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMoveMsgBodyFromPersistent(): Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyFromPersistent():  "
			"pCorrelation param passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_listSizeOf(pslSrcMsgBodyList, &dSize, pErr) == Sdf_co_fail)
			return Sdf_co_fail;

	/*
	 * Iterate through slSrcMsgBodyList and move elements to the 
	 * pslDestMsgBodyList list.
	 */
	for (dIndex=0; dIndex<dSize; dIndex++)
	{
		Sdf_st_msgBody *pSdfMsgBody;

		if (sdf_listGetAt(pslSrcMsgBodyList, dIndex, (Sdf_ty_pvoid *)\
			&pSdfMsgBody, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyFromPersistent():  "
				"Failed to get message body at index from "
				"PersistentMsgBodyList",pErr);	
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}

		if (pSdfMsgBody->pCorrelation == Sdf_co_null)
			continue;

		if (Sdf_mc_strcmp(pSdfMsgBody->pCorrelation,pCorrelation) == 0)
		{
			/*
			 * Increment the reference count and extract the message body 
			 * from the Sdf_st_msgBody structure
			 */
			pMsgBody = pSdfMsgBody->pMsgBody ;
			HSS_LOCKEDINCREF(pMsgBody->dRefCount);

			if (sdf_listAppend(pslDestMsgBodyList, pMsgBody, \
				pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyFromPersistent():  "
					"Failed to append message body to slDestMsgBodyList",pErr);	
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			/*
			 * Delete the element from the source MsgBody list
			 */
			if (sdf_listDeleteAt(pslSrcMsgBodyList, dIndex, pErr) == \
					Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaMoveMsgBodyFromPersistent():  "
					"Failed to  delete message body from source list",pErr);	
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
			dIndex--;
			dSize--;
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaMoveMsgBodyFromPersistent");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaMoveHeaderListByType
 **
 ** DESCRIPTION: This function moves a headerList structure identified
 **				 by the type from the source list to the destination list. 
 **				 The structure is removed from the source list.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMoveHeaderListByType 
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, 
	Sdf_ty_slist  *pslDestHeaderList,
	en_HeaderType dType, 
	Sdf_st_error *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, dType, pErr)
	Sdf_ty_slist *pslSrcHeaderList;
	Sdf_ty_slist *pslDestHeaderList;
	en_HeaderType dType;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_bool dDeleteAfterCopy = Sdf_co_true;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaMoveHeaderListByType");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMoveHeaderListByType() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	
	if (sdf_fn_uaPerformHeaderListCopyByType(pslSrcHeaderList, \
		pslDestHeaderList, dType, dDeleteAfterCopy, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMoveHeaderListByType():  "
			"Failed to move the headerlist from source to destination", pErr);	
#endif		
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaMoveHeaderListByType");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaMoveHeaderListByCorrelation
 **
 ** DESCRIPTION: This function moves a headerList structure identified
 **				 by the correlation string from the source list to the 
 **				 destination list. The structure is removed from the 
 **				 source list.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMoveHeaderListByCorrelation
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList,\
	Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, pCorrelation, pErr)
	Sdf_ty_slist *pslSrcHeaderList;
	Sdf_ty_slist *pslDestHeaderList;
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_bool dDeleteAfterCopy = Sdf_co_true;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaMoveHeaderListByCorrelation");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaMoveHeaderListByCorrelation() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMoveHeaderListByCorrelation() : "
			"Invalid correlation string passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (sdf_fn_uaPerformHeaderListCopyByCorrelation(pslSrcHeaderList, \
		pslDestHeaderList, pCorrelation, dDeleteAfterCopy, pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaMoveHeaderListByCorrelation() : "
			"Failed to move the headerlist from source to destination",pErr);
#endif		
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaMoveHeaderListByCorrelation");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCopyHeaderListByType
 **
 ** DESCRIPTION: This function copies a headerList structure identified
 **				 by the type from the source list to the destination list.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCopyHeaderListByType 
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList,\
	en_HeaderType dType, Sdf_st_error *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, dType, pErr)
	Sdf_ty_slist pslSrcHeaderList;
	Sdf_ty_slist pslDestHeaderList;
	en_HeaderType dType;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_bool dDeleteAfterCopy = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaCopyHeaderListByType");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCopyHeaderListByType() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	
	if (sdf_fn_uaPerformHeaderListCopyByType(pslSrcHeaderList, \
		pslDestHeaderList, dType, dDeleteAfterCopy, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCopyHeaderListByType():  "
			"Failed to copy the headerlist from source to destination", pErr);	
#endif		
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCopyHeaderListByType");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCopyHeaderListByCorrelation
 **
 ** DESCRIPTION: This function copies a headerList structure identified
 **				 by the correlation string from the source list to the 
 **				 destination list.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCopyHeaderListByCorrelation
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslSrcHeaderList, 
	Sdf_ty_slist  *pslDestHeaderList, 
	Sdf_ty_s8bit  *pCorrelation, 
	Sdf_st_error  *pErr)
#else
	(pslSrcHeaderList, pslDestHeaderList, pCorrelation, pErr)
	Sdf_ty_slist *pslSrcHeaderList;
	Sdf_ty_slist *pslDestHeaderList;
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_bool dDeleteAfterCopy = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaCopyHeaderListByCorrelation");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCopyHeaderListByCorrelation() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCopyHeaderListByCorrelation() : "
			"Invalid correlation string passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (sdf_fn_uaPerformHeaderListCopyByCorrelation(pslSrcHeaderList, \
		pslDestHeaderList, pCorrelation, dDeleteAfterCopy, pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCopyHeaderListByCorrelation() : "
			"Failed to copy the headerlist from source to destination",pErr);
#endif		
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaCopyHeaderListByCorrelation");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetCallObjectKeyFromCallObject
 **
 ** DESCRIPTION: This function returns a pointer to the key structure 
 **				 associated with the callObject
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_ivk_uaGetCallObjectKeyFromCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, Sdf_st_callObjectKey **ppCallObjectKey, \
	Sdf_st_error *pErr)
#else
	(pObject, ppCallObjectKey, pErr)
	Sdf_st_callObject *pObject;
	Sdf_st_callObjectKey **ppCallObjectKey;
	Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaGetCallObjectKeyFromCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetCallObjectKeyFromCallObject() : Error param passed is"
			" invalid ");
#endif
		return Sdf_co_fail;
	}
	if(ppCallObjectKey == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallObjectKeyFromCallObject() : "
			"Invalid call object key pointer passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallObjectKeyFromCallObject() : "
			"Invalid call-object parameter passed",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */


	*ppCallObjectKey=Sdf_co_null;

	/* ================ Parameter validations end ================== */

	if(pObject->pCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallObjectKeyFromCallObject() : "
			"CommonInfo pointer in call-object is NULL",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if(pObject->pCommonInfo->pKey == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCallObjectKeyFromCallObject() : "
			"CallObjectKey in call-object is NULL",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	*ppCallObjectKey = pObject->pCommonInfo->pKey;
	HSS_LOCKEDINCREF(pObject->pCommonInfo->pKey->dRefCount);

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaGetCallObjectKeyFromCallObject");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaDeleteMsgBodyByCorrelation
 **
 ** DESCRIPTION: This function deletes Sdf_st_msgBody structure in
 **				 the list where an entry corresponding to the given
 **				 correlation string is located
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteMsgBodyByCorrelation
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pslMsgBodyList, Sdf_ty_s8bit *pCorrelation, \
	Sdf_st_error *pErr)
#else
	(pslMsgBodyList, pCorrelation, pErr)
	Sdf_ty_slist pslMsgBodyList;
	Sdf_ty_s8bit *pCorrelation;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit 	count=0, i=0;
	Sdf_ty_bool		dMessageBodyDeleted = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Entering sdf_ivk_uaDeleteMsgBodyByCorrelation");
		
	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
			"Error param passed is  invalid ");
#endif
		return Sdf_co_fail;
	}	
 	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
			"Correlation string passed is NULL",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;		
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_listSizeOf(pslMsgBodyList, &count, pErr) == Sdf_co_fail)
			return Sdf_co_fail;

	/*
	 * Iterate through the message body list searching for pCorrelation. Delete
	 * the matching message body
	 */
	for (i=0; i<count; i++)
	{
		Sdf_st_msgBody *pSdfMsgBody = Sdf_co_null;

		if (sdf_listGetAt(pslMsgBodyList, i, (Sdf_ty_pvoid *)&pSdfMsgBody, \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
				"Could not get message body at an index from Msgbody list",\
				pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		if (Sdf_mc_strcmp(pCorrelation, pSdfMsgBody->pCorrelation) == 0)
		{
			/*
			 * Correlation strings matched. Delete this element.
			 */
			if (sdf_listDeleteAt(pslMsgBodyList, i, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
					"Could not delete message body at index from MsgBody list",\
					pErr);
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
			dMessageBodyDeleted = Sdf_co_true;
			i--;
			count--;
		}
	}

	if (dMessageBodyDeleted != Sdf_co_true)
	{
		/*
		 * No message body matching pCorrelation found. Return 
		 * Sdf_en_noExistError
		 */
		pErr->errCode = Sdf_en_noExistError;
		sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaDeleteMsgBodyByCorrelation");
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaDeleteMsgBodyByCorrelation");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

}

/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetStateMaintenance
**
** DESCRIPTION: This API returns the type of state maintenance (call/
**				transaction statefulness) set in the toolkit.
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetStateMaintenance 
#ifdef ANSI_PROTO 
(Sdf_ty_stateMaintenance *pStateMaintenance, 
Sdf_st_error *pErr)
#else
(pStateMaintenance, pErr)
Sdf_ty_stateMaintenance *pStateMaintenance; 
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetStateMaintenance");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetStateMaintenance( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}

	if(pGlbToolkitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaGetStateMaintenance( ): "
		 	"Toolkit not initialized",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	*pStateMaintenance = pGlbToolkitData->dStateMaintenance;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetStateMaintenance");	

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetStateMaintenance
**
** DESCRIPTION: This API sets whether call/transaction stateful behaviour
**				is to be applied if no matching call object is found for
**				the call. If this API is not invoked, call stateful is
**				assumed. In the transaction stateful mode, the UA toolkit
**				will not automatically respond to bad requests. Instead it
**				will issue the sdf_cbk_uaTransactionReceived callback to the
**				application.
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetStateMaintenance 
#ifdef ANSI_PROTO 
(Sdf_ty_stateMaintenance dStateMaintenance, 
Sdf_st_error *pErr)
#else
(dStateMaintenance, pErr)
Sdf_ty_stateMaintenance dStateMaintenance; 
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetStateMaintenance");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetStateMaintenance( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}

	if(pGlbToolkitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaSetStateMaintenance( ): "
		 	"Toolkit not initialized",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	pGlbToolkitData->dStateMaintenance = dStateMaintenance;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetStateMaintenance");	

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;

}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneTransaction
 **
 ** DESCRIPTION: This function copies the 
 **				 Transaction structure from src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneTransaction
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pDestTransaction,
	Sdf_st_transaction *pSrcTransaction,
	Sdf_st_error *pErr)
#else
	(pDestTransaction,pSrcTransaction, pErr )
	 Sdf_st_transaction *pDestTransaction;
	 Sdf_st_transaction *pSrcTransaction;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator dListIterator;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
		"sdf_ivk_uaCloneTransaction");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaCloneTransaction (): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction():  "
			"pDestTransaction Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pSrcTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction():  "
			"pSrcTransaction Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestTransaction->dTransactionState = pSrcTransaction->dTransactionState;

	if (pSrcTransaction->pSipMsg != Sdf_co_null)
	{
		if (sip_initSipMessage(&(pDestTransaction->pSipMsg), \
			pSrcTransaction->pSipMsg->dType, \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): "
				"Initializing SIP message in transaction object failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		if (sip_cloneSipMessage(pDestTransaction->pSipMsg, \
			pSrcTransaction->pSipMsg, \
			(SipError*)&(pErr->stkErrCode))== SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): "
				"Cloning SIP message from source to destination failed ",\
				pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
	}

	/* 
	 * Clone list of Processed headers
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slProcessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slProcessedHeaders), \
		&dListIterator, pErr);

	/* 
	 * Clone TransportInfo structure
	 */
	if(pDestTransaction->pDestInfo == Sdf_co_null)
	{
		if(sdf_ivk_uaInitTransportInfo(&(pDestTransaction->\
			pDestInfo),pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Initializing transport info in destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	}
	if(sdf_ivk_uaCloneTransportInfo(pDestTransaction->pDestInfo, \
		pSrcTransaction->pDestInfo, pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
			"Cloning transport info failed ",pErr);
#endif
		pErr->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	/*
	 * Clone the dSockfd
	 */
	pDestTransaction->dSocket.dSockfd=pSrcTransaction->dSocket.dSockfd;


	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Initializing HeaderList for Processed Headers failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Cloning Processed Headers List failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slProcessedHeaders), \
			(Sdf_ty_pvoid) pClonedHdrList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Appending ProcessedHeader to slProcessedHeaders of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/* 
	 * Clone list of Unprocessed headers
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slUnprocessedHeaders), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slUnprocessedHeaders), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		Sdf_st_headerList *pClonedHdrList;

		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_ivk_uaInitHeaderList(&pClonedHdrList, pHeaderList->dType,\
			pHeaderList->pCorrelation, pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Initializing HeaderList for Unprocessed headers failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_ivk_uaCloneHeaderList(pClonedHdrList, pHeaderList,\
			pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Cloning Unprocessed Headers List failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slUnprocessedHeaders), \
			(Sdf_ty_pvoid) pClonedHdrList, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Appending UnprocessedHeader to slUnprocessedHeaders of "
				"destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}


	/* 
	 * Clone list of Message Bodies 
	 */
	(void)sdf_listDeleteAll(&(pDestTransaction->slMsgBodyList), pErr);

	(void)sdf_listInitIterator(&(pSrcTransaction->slMsgBodyList), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		SipMsgBody *pSipMsgBody;
		SipMsgBody *pClonedMsgBody;

		pSipMsgBody = (SipMsgBody*)\
			(dListIterator.pCurrentElement->pData);

		if(sip_initSipMsgBody(&pClonedMsgBody, SipBodyAny,\
		 	(SipError *)&(pErr->stkErrCode)) ==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Initializing message body for slMsgBodyList failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sip_cloneSipMsgBody(pClonedMsgBody, pSipMsgBody,\
			(SipError *)&(pErr->stkErrCode))==SipFail)
		{
			pErr->errCode = Sdf_en_noMemoryError;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
				"Cloning message body for slMsgBodyList failed ",pErr);
#endif
			return Sdf_co_fail;
		}
		if(sdf_listAppend(&(pDestTransaction->slMsgBodyList), (Sdf_ty_pvoid)\
	 		pClonedMsgBody,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): Appending"
			   	"MessageBody to slMsgBodyList of destination failed ",pErr);
#endif
			pErr->errCode=Sdf_en_transactionAccessError;
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}

#ifdef SDF_TXN
	if(pSrcTransaction->pTxnKey != Sdf_co_null)
	{
		if( pDestTransaction->pTxnKey == Sdf_co_null )
/*w534*/(void)sip_initSipTxnKey( &(pDestTransaction->pTxnKey), \
					(SipError *)&(pErr->stkErrCode));

		if (sip_cloneSipTxnKey(pDestTransaction->pTxnKey, \
			 pSrcTransaction->pTxnKey, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transactionAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
					"Cloning the SIP Transaction key structure failed.", pErr);
#endif
				pErr->errCode=Sdf_en_transactionAccessError;
				return Sdf_co_fail;
		}
	}
	if(pSrcTransaction->pInvTxnKey != Sdf_co_null)
	{
		if( pDestTransaction->pInvTxnKey == Sdf_co_null )
/*w534*/(void)sip_initSipTxnKey( &(pDestTransaction->pInvTxnKey), \
					(SipError *)&(pErr->stkErrCode));

		if (sip_cloneSipTxnKey(pDestTransaction->pInvTxnKey, \
			 pSrcTransaction->pInvTxnKey, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_transactionAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaCloneTransaction(): " 
					"Cloning the SIP Transaction key structure failed.", pErr);
#endif
				pErr->errCode=Sdf_en_transactionAccessError;
				return Sdf_co_fail;
		}
	}

#endif
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaCloneTransaction");

	pErr->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_ivk_uaGetContactInfoFromInitData
**
** DESCRIPTION: This function returns the Conatct information from the
**				InitData structure
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetContactInfoFromInitData
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	Sdf_ty_slist *pContactList,
	Sdf_st_error *pError)
#else	
	(pInitData, pContactList, pError)
	Sdf_st_initData *pInitData;
	Sdf_ty_slist *pContactList;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_nameInfo	*pNameInfo = Sdf_co_null;
	Sdf_ty_u32bit	dSize=0;
	SipHeader 		*pContactHeader = Sdf_co_null;
	SipAddrSpec *pAddrSpec = Sdf_co_null;
	SipUrl *pSipUrl = Sdf_co_null;
	Sdf_ty_u32bit userHostLen=0, urlParamCount=0;
	Sdf_ty_u32bit contactParamCount=0, iterator=0;
	Sdf_ty_u32bit	iterator1=0;
	Sdf_ty_s8bit *pUserName = Sdf_co_null,*pHost = Sdf_co_null;
	Sdf_ty_s8bit *pDispName = Sdf_co_null;
	en_AddrType addrType;
	Sdf_ty_s8bit *pUri=Sdf_co_null,*pAddress=Sdf_co_null;
	Sdf_ty_s8bit dTempUri[Sdf_mc_maxConfigFileLen]="";
	Sdf_ty_bool	dIsImUrl = Sdf_co_false;
	Sdf_ty_bool	dIsTelUrl = Sdf_co_false;
	Sdf_ty_s8bit *pScheme=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetContactInfoFromInitData");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetContactInfoFromInitData(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
			"pInitData Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pContactList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
			"SipList  pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	
	/*Get the Size of Conatact Header List*/
	if(sdf_listSizeOf(&(pInitData->slContact),&dSize,pError)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
			"Failed to Get Size of Contact Header List",pError);
#endif
		pError->errCode=Sdf_en_initDataAccessError;
		return Sdf_co_fail;
	}
	if(dSize==0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
			"InitData Param does not have a Contact header ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	for(iterator1=0; iterator1<dSize; iterator1++)
	{
		if(sdf_listGetAt(&(pInitData->slContact),iterator1,\
			(Sdf_ty_pvoid *)&pContactHeader,pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Failed to Get Contact Header from List",pError);
#endif
			pError->errCode=Sdf_en_initDataAccessError;
			return Sdf_co_fail;
		}
	
		/*
		 * Initialize a Contact NameInfo structure 
		 */
		if (sdf_ivk_uaInitNameInfo(&pNameInfo,pError)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Name info initialization failed", pError);
#endif
			sip_freeSipUrl(pSipUrl);
			sip_freeSipAddrSpec(pAddrSpec);
			return Sdf_co_fail;
		}

		if(sip_getAddrSpecFromContactHdr(pContactHeader,&pAddrSpec,\
			(SipError *)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Failed to extract addrspec from Contact header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
			return Sdf_co_fail;
		}
		if(sip_getAddrTypeFromAddrSpec(pAddrSpec, &addrType, \
			(SipError *)&(pError->stkErrCode))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Failed to extract type from Contact header",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			sip_freeSipAddrSpec(pAddrSpec);
			(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
			return Sdf_co_fail;
		}
		/* 
		 * Now start filling in stuff into name-info
		 */

#ifdef SDF_TLS
	    if((addrType != SipAddrSipUri) && (addrType != SipAddrSipSUri))
#else
	    if(addrType != SipAddrSipUri)
#endif
		{
#ifdef SDF_TEL
			if(sip_isTelUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
				==SipSuccess)
				dIsTelUrl = Sdf_co_true;
#endif
#ifdef SDF_IM
			if( dIsTelUrl == Sdf_co_false )
			{
				if(sip_isImUrl(pAddrSpec,(SipError*)&(pError->stkErrCode)) \
						==SipSuccess)
					dIsImUrl = Sdf_co_true;
			}
#endif
			if((dIsTelUrl != Sdf_co_true) && (dIsImUrl != Sdf_co_true))
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
				sip_freeSipAddrSpec(pAddrSpec);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_notImplemented, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"From Header Contains Unsupported URL Scheme", pError);
#endif
				pError->errCode=Sdf_en_notImplemented;
				return Sdf_co_fail;
			}

			if(sip_getUriFromAddrSpec(pAddrSpec,&pUri,\
					(SipError*)&(pError->stkErrCode))==SipFail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,pError->errCode, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Failed to Get Uri From AddrSpec",pError);
#endif
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			userHostLen += Sdf_mc_strlen(pUri);

			(pNameInfo)->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
			userHostLen+2,pError);
			if((pNameInfo)->pAddress == Sdf_co_null)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Failed to allocate memory for the pAddress member",pError);
#endif
				pError->errCode=Sdf_en_noMemoryError;
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			/*Copy Uri into the Address.Before Cpy remove the tel: from it.*/
			Sdf_mc_strcpy(dTempUri,pUri);
			pScheme = Sdf_mc_strtokr(dTempUri,":",&pAddress);
			Sdf_mc_strcpy((pNameInfo)->pAddress,pAddress);
		}
		else
		{
			if(sdf_ivk_uaGetUrlFromAddrSpec(pAddrSpec, &pSipUrl,\
				pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Could not extract SIP URL from Contact header",pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}

/*w534*/(void)sip_getUserFromUrl(pSipUrl, &pUserName,\
			(SipError *)&(pError->stkErrCode));
	
			if(pUserName != Sdf_co_null)
				userHostLen += Sdf_mc_strlen(pUserName);
	
/*w534*/(void)sip_getHostFromUrl(pSipUrl, &pHost, \
					(SipError *)&(pError->stkErrCode));

			userHostLen += Sdf_mc_strlen(pHost);
	
			pNameInfo->pAddress = (Sdf_ty_s8bit *)sdf_memget(ACCESSOR_MEM_ID,\
				userHostLen+2,pError);

			if(pNameInfo->pAddress == Sdf_co_null)
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Failed to allocate memory for the pAddress member",pError);
#endif
				pError->errCode=Sdf_en_noMemoryError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			/*
			 * Copy the user@host portion into pNameInfo->pAddress
			 */
			Sdf_mc_strcpy(pNameInfo->pAddress,"");
			if ((pUserName!=Sdf_co_null) && (Sdf_mc_strcmp(pUserName, "") != 0))
			{
				Sdf_mc_strcat(pNameInfo->pAddress, pUserName);
				Sdf_mc_strcat(pNameInfo->pAddress, "@");
			}
			Sdf_mc_strcat(pNameInfo->pAddress, pHost);
	
			if(sip_getPortFromUrl(pSipUrl, &(pNameInfo->dPort), \
				(SipError *)&(pError->stkErrCode))==SipFail)
			{
#ifdef SDF_TLS
				if (addrType == SipAddrSipSUri)
					pNameInfo->dPort = Sdf_mc_defaultSipSPort;
				else
#endif
					pNameInfo->dPort = Sdf_mc_defaultSipPort;
			}
		
			/* 
			 * Username, host and port extracted. Get URL params now
			 */
			if (sip_getUrlParamCountFromUrl(pSipUrl, &urlParamCount, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Could not get count of url params from Contact header",\
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pAddrSpec);
				return Sdf_co_fail;
			}
			for(iterator = 0; iterator < urlParamCount; iterator++)
			{
				SipParam *pParam = Sdf_co_null;

				if (sip_getUrlParamAtIndexFromUrl(pSipUrl, &pParam, iterator, \
					(SipError *)&(pError->stkErrCode)) == SipFail)
				{
					(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData()"
						"Could not get url param at index from Contact header",\
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					sip_freeSipUrl(pSipUrl);
					sip_freeSipAddrSpec(pAddrSpec);
					return Sdf_co_fail;
				}
				if (sdf_listAppend(&(pNameInfo->slUrlParams), pParam, pError) \
					== Sdf_co_fail)
				{
					(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, \
						Sdf_en_headerManipulationError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData:"
						"Failed to append Url parameter to nameInfo structure",\
						pError);
#endif
					pError->errCode=Sdf_en_headerManipulationError;
					sip_freeSipUrl(pSipUrl);
					sip_freeSipAddrSpec(pAddrSpec);
					return Sdf_co_fail;
				}	
			}
			sip_freeSipUrl(pSipUrl);
		}
		sip_freeSipAddrSpec(pAddrSpec);

		/* 
		 * All url related information extracted. Now get the header parameters
		 */

		/*
		 * Extract display name
		 */
		if(sip_getDispNameFromContactHdr(pContactHeader, &pDispName, \
			(SipError *)&(pError->stkErrCode)) ==SipFail)
			pNameInfo->pName = Sdf_co_null;
		else
		{
			Sdf_mc_strdup(pNameInfo->pName,pDispName);
		}
		
		/*Fill the Scheme*/
		if(addrType==SipAddrSipUri)			
			Sdf_mc_strdup(pNameInfo->pScheme,"sip");
#ifdef SDF_TLS
	    else if(addrType==SipAddrSipSUri)
		    Sdf_mc_strdup(pNameInfo->pScheme,"sips");
#endif
		else
			Sdf_mc_strdup(pNameInfo->pScheme,pScheme);

		if (sip_getContactParamsCountFromContactHdr(pContactHeader, \
			&contactParamCount, (SipError *)&(pError->stkErrCode)) == SipFail)
		{
			(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Could not get count of Contact parameters from Contact "
				"header", pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		for(iterator = 0; iterator < contactParamCount; iterator++)
		{
			SipContactParam *pContactParam = Sdf_co_null;

			if (sip_getContactParamAtIndexFromContactHdr(pContactHeader, \
				&pContactParam, iterator, (SipError *)&(pError->stkErrCode)) \
				== SipFail)
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Could not get Contact parameter at index from "
					"Contact header", pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if (sdf_listAppend(&((pNameInfo)->slHeaderParams), pContactParam, \
				pError) == Sdf_co_fail)
			{
				(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_headerManipulationError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
					"Could not append Contact param to nameInfo structure", \
					pError);
#endif
				pError->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		if(sdf_listAppend(pContactList,(Sdf_ty_pvoid)pNameInfo,pError) == \
				Sdf_co_fail)
		{
			(void)sdf_ivk_uaFreeNameInfo(pNameInfo);
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetContactInfoFromInitData(): "
				"Could not append nameInfo structure into the List",pError);
#endif
			pError->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetContactInfoFromInitData()");

	pError->errCode = Sdf_en_noUaError;
	
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaAddheader
 **
 ** DESCRIPTION: This function will form a header from the string passed
 **				 to this API and will insert in the header list
 **
 ******************************************************************************/

Sdf_ty_retVal  sdf_ivk_uaAddheader
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pList,
	Sdf_ty_s8bit  *pHeader,
	Sdf_st_error  *pErr)
#else
	(pList,pHeader,pErr)
	Sdf_ty_slist *pList;
	Sdf_ty_s8bit  *pHeader;
	Sdf_st_error  *pErr;
#endif	
{
	SipHeader *pHdr=Sdf_co_null;
	Sdf_ty_s8bit *pName,*pTemp, *pFinal;
	en_HeaderType dHeaderType;
    SIP_U32bit dSize;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaAddheader");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaAddheader():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddheader():  "
			"pList param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pHeader == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddheader():  "
			"pHeader param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* Get the type of the header by taking the first part of the string
	 * i.e from begining to :
	 */

	(void)Sdf_mc_strtok(pHeader, "\n");
	Sdf_mc_strdup(pTemp,pHeader);
	if (Sdf_mc_strstr(pTemp, ":") != Sdf_co_null)
	{
		Sdf_ty_s8bit *pTempName=Sdf_co_null,*addr=Sdf_co_null;
		pTempName=Sdf_mc_strtokr(pTemp,":",&addr);
		Sdf_mc_strdup(pName,pTempName);
		(void)Sdf_mc_strtok(pName, " ");
        dSize = Sdf_mc_strlen(pName);
    	pFinal =(char *) fast_memget(Sdf_mc_initMemId, (dSize+2), SIP_NULL);
		Sdf_mc_strcpy(pFinal,pName);
		Sdf_mc_strcat(pFinal,":");
		pTempName=Sdf_co_null;
		addr=Sdf_co_null;
	}
	else	
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddheader():  "
			"pHeader param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	
	sip_getTypeFromName(pFinal, &dHeaderType, (SipError *)&\
		(pErr->stkErrCode));
		
	/* Free The Local References */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
		&pTemp, Sdf_co_null);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
		&pName, Sdf_co_null);
    (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
		&pFinal, Sdf_co_null);
	/* Allocate memory for the SipHeader structure*/
	pHdr = (SipHeader *) sdf_memget\
				(Sdf_mc_initMemId,sizeof(SipHeader), Sdf_co_null);

	/* Form the header from the string */			
	if(sip_parseSingleHeader(pHeader, dHeaderType, pHdr,(SipError *)\
		&(pErr->stkErrCode))==SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddheader():  "
			"parsing pHeader failed ",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
       /* klock tool error fix */
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(0,(Sdf_ty_pvoid *)&pHdr, pErr);

		return Sdf_co_fail;
	}
	if (sdf_listAppend(pList,(Sdf_ty_pvoid )pHdr, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerListAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddheader(): "
			"Failed to add SipHeader into the pList ",pErr);
#endif
		pErr->errCode=Sdf_en_headerListAccessError;
		sip_freeSipHeader(pHdr);
		return Sdf_co_fail;
	}
	(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
		&pHeader, Sdf_co_null);
	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaAddheader");
   	return Sdf_co_success;

}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaExtractHeaderList
 **
 ** DESCRIPTION: This function Extracts a header from the slProcessed header list
 **
 ******************************************************************************/

Sdf_ty_retVal  sdf_ivk_uaExtractHeaderList
#ifdef ANSI_PROTO
	(Sdf_ty_slist *pList,
	en_HeaderType  dHdrType,
	Sdf_st_headerList **ppExtractedHdrList,
	Sdf_st_error  *pErr)
#else
	(pList,dHdrType,ppExtractedHdrList,pErr)
	Sdf_ty_slist *pList;
	en_HeaderType  dHdrType;
	Sdf_st_headerList **ppExtractedHdrlist;
	Sdf_st_error  *pErr;
#endif	
{
	Sdf_st_listIterator dListIterator;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaExtractheader");
	
	/* ================ Parameter validations begin ================== */
	
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaExtractheader():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaExtractheader():  "
			"pList param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */
	
	/*
	 * Operate on elements of the selected "processed headers" list
	 */
	(void)sdf_listInitIterator(pList, &dListIterator, pErr);
	
	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_headerList *pHeaderList;
		
		pHeaderList = (Sdf_st_headerList*)\
			(dListIterator.pCurrentElement->pData);
		
		if (pHeaderList->dType == dHdrType)
		{
			/* Increment reference count */
			HSS_LOCKEDINCREF(pHeaderList->dRefCount);
			*ppExtractedHdrList = pHeaderList;
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		
		(void)sdf_listNext(&dListIterator, pErr);
	} /* End of outer for loop looping through siplists of headers */
	
	*ppExtractedHdrList = Sdf_co_null;
	pErr->errCode = Sdf_en_noExistError;
	return Sdf_co_fail;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaGetCseqInCallObject
**
** DESCRIPTION: This function returns the current local Cseq number from the
**				Call Object structure
**
** PARAMETERS:
**		pCallObject(IN)		: The Call object structure 
**		pCseq(OUT) 			: The local Cseq that has been retrieved
**		pError(OUT)			: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetCseqInCallObject 
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObject, Sdf_ty_u32bit *pCseq, \
	Sdf_st_error *pError)
#else
	(pCallObject, pCseq, pError)
	 Sdf_st_callObject *pCallObject; 
	 Sdf_ty_u32bit *pCseq;
	 Sdf_st_error *pError;
#endif
{
	Sdf_st_commonInfo *pCommonInfo;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetCseqInCallObject");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetCseqInCallObject():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCseq == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCseqInCallObject():  "
			"pCseq Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCseqInCallObject():  "
			"Call Object param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * get the Cseq Value from the call object 
	 */
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pCallObject, &pCommonInfo, \
			pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetCommonInfoFromCallObject(): "
			"Unable to get Common Info from Call Object",pError);
#endif
		return Sdf_co_fail;
	}
	
	*pCseq = pCommonInfo->dLocalCseq;
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetCseqInCallObject");

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaSetCseqInCallObject
**
** DESCRIPTION: This function sets the local Cseq number in Call Object 
**				to the specified value.
**
** PARAMETERS:
**		pCallObject(IN)		: The Call object structure 
**		dCseq(IN) 			: The Cseq number
**		pError(OUT)			: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetCseqInCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObject, Sdf_ty_u32bit dCseq, \
	Sdf_st_error *pError)
#else
	(pCallObject, dCseq, pError)
	 Sdf_st_callObject *pCallObject; 
	 Sdf_ty_u32bit dCseq;
	 Sdf_st_error *pError;
#endif
{
	Sdf_st_commonInfo *pCommonInfo;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetCseqInCallObject");

	/* ================ Parameter validations begin ================== */
#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetCseqInCallObject():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetCseqInCallObject():  "
			"Call Object param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	/* 
	 * get the Cseq Value from the call object 
	 */
	if (sdf_ivk_uaGetCommonInfoFromCallObject(pCallObject, &pCommonInfo, \
			pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetCommonInfoFromCallObject(): "
			"Unable to get Common Info from Call Object",pError);
#endif
		return Sdf_co_fail;
	}
	
	pCommonInfo->dLocalCseq = dCseq;
	(void)sdf_ivk_uaFreeCommonInfo(pCommonInfo);
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetCseqInCallObject");

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}
/******************************************************************************
 ** FUNCTION:  	  sdf_ivk_uaDeleteHeaderListByCorrelation
 **
 ** DESCRIPTION:  This function returns a list of Sdf_st_headerList structure
 **				  corresponding to the given correlation string
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDeleteHeaderListByCorrelation
#ifdef ANSI_PROTO
(Sdf_ty_slist *pslHeaderList, 
const Sdf_ty_s8bit *pCorrelation,
Sdf_st_error *pErr)
#else
(pslHeaderList, pCorrelation, pErr)
Sdf_ty_slist *pslHeaderList; 
const Sdf_ty_s8bit *pCorrelation;
Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit 	count=0, i=0;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaDeleteHeaderListByCorrelation");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaDeleteHeaderListByCorrelation(): Error Param passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteHeaderListByCorrelation():  "
			"pCorrelation Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */
	
	if(sdf_listSizeOf(pslHeaderList, &count, pErr) == Sdf_co_fail)
			return Sdf_co_fail;

	/*
	 * Iterate through the HeaderList searching for pCorrelation. Delete
	 * the matching Struture
	 */
	for (i=0; i<count; i++)
	{
		Sdf_st_headerList *pHeaderList = Sdf_co_null;

		if (sdf_listGetAt(pslHeaderList, i, (Sdf_ty_pvoid *)&pHeaderList, \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
				"Could not get HeaderList at an index from HeaderList list",\
				pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			return Sdf_co_fail;
		}
		if (Sdf_mc_strcmp(pCorrelation, pHeaderList->pCorrelation) == 0)
		{
			/*
			 * Correlation strings matched. Delete this element.
			 */
			if (sdf_listDeleteAt(pslHeaderList, i, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_callObjectAccessError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaDeleteMsgBodyByCorrelation() : "
					"Could not delete HeaderList structure at index from MsgBody list",\
					pErr);
#endif
				pErr->errCode=Sdf_en_callObjectAccessError;
				return Sdf_co_fail;
			}
			i--;
			count--;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaDeleteMsgBodyByCorrelation");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMsgBody
 **
 ** DESCRIPTION: This function returns a list of Sdf_st_msgBody structure
 **				 corresponding to the given correlation string
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMsgBody 
#ifdef ANSI_PROTO
(Sdf_st_callObject *pObject, Sdf_ty_s8bit *pCorrelation, \
Sdf_ty_slist *pslMatchedMsgBodyList, Sdf_st_error *pErr)
#else
(pObject, pCorrelation, pslMatchedMsgBodyList, pErr)
Sdf_st_callObject *pObject;
Sdf_ty_s8bit *pCorrelation;
Sdf_ty_slist *pslMatchedMsgBodyList;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_mediaHandling *pMediaHandling = Sdf_co_null;
	Sdf_ty_sessionParamType dSessionParamType;
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	Sdf_st_msgBody	*pSdfMsgBody=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaGetMsgBody");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetMsgBody(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"pObject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pCorrelation == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"pCorrelation param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if (Sdf_mc_strcmp(pCorrelation, "uat-newsdp-recd") == 0)
    {
		dSessionParamType = Sdf_en_lastOfferRecvd;
    }
	else if (Sdf_mc_strcmp(pCorrelation, "uat-newsdp-proposed") == 0)
    {
		dSessionParamType = Sdf_en_lastOfferMade;
    }
	else if ((Sdf_mc_strcmp(pCorrelation, "uat-negotiatedsdp") == 0) ||
			 (Sdf_mc_strcmp(pCorrelation, "uat-session-sdp") == 0))
    {
		dSessionParamType = Sdf_en_currentSession;
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"pCorrelation string passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaGetMediaInfo(pObject, &pMediaHandling,\
				pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"Failed to get MediaHandling from CallObject.",pErr);	
#endif
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaGetSessionParamsFromMediaInfo(pMediaHandling,\
				&pSessionParams, dSessionParamType, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_mediaHandlingAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"Failed to get SessionParams from MediaHandling.",pErr);	
#endif
	(void)sdf_ivk_uaFreeMediaHandling(pMediaHandling);
		pErr->errCode=Sdf_en_mediaHandlingAccessError;
		return Sdf_co_fail;
    }

	/* Free the local reference. */
	(void)sdf_ivk_uaFreeMediaHandling(pMediaHandling);

	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pErr->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetMsgBody( ): "
			"Failed to allocate memory for the SDP message", pErr);
#endif		
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

 	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams, \
			Sdf_en_convertLocal,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetMsgBody( ): "
			"Failed to clone the sdp message structure", pErr);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}
	/* Free the local reference. */

	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pErr->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetMsgBody( ): "
			"Failed to allocate memory for the SIP message body", pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   ((SipError*)&(pErr->stkErrCode))) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetMsgBody( ): "
			"Failed to set the SDP message in the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }
	sip_freeSdpMessage(pSdpMessage);

	if (sdf_listInit(pslMatchedMsgBodyList, __sdf_fn_uaFreeMsgBody, \
		Sdf_co_false,pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"Failed to initialize list of matched MessageBodies list",pErr);	
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	/*Form Sdf_st_msgBody*/
	if(sdf_ivk_uaInitMsgBody(&pSdfMsgBody,pCorrelation,\
					pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"Failed to Init Sdf_st_msgBody", pErr);	
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_listFree(pslMatchedMsgBodyList, pErr);
		return Sdf_co_fail;
	}
	/*Set the SipMsgBody inside Sdf_st_msgBody*/
	pSdfMsgBody->pMsgBody = pSipMsgBody;
	if (sdf_listAppend(pslMatchedMsgBodyList,pSdfMsgBody,pErr) ==  Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMsgBody():  "
			"Failed to append message body to pslMatchedMsgBodyList", pErr);	
#endif
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_listFree(pslMatchedMsgBodyList, pErr);
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaGetMsgBody");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRetrieveOverlapTransInfoByCseq
 **
 ** DESCRIPTION: This API retrieves an OverlapTransInfo element within the 
 ** 			 call object that corresponds to the Cseq passed.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaRetrieveOverlapTransInfoByCseq
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit		dCseqNum,
	Sdf_ty_msgMode		dMsgMode,
	en_SipMessageType 	dType,
	Sdf_st_callObject 	*pCallObj,
	Sdf_st_overlapTransInfo **ppTransInfo,
	Sdf_st_error *pError)
#else	
	(dCseqNum,dMsgMode,dType,pCallObj, ppTransInfo, pError)
	Sdf_ty_u32bit		dCseqNum;
	Sdf_ty_msgMode		dMsgMode;
	en_SipMessageType 	dType;
	Sdf_st_callObject 	*pCallObj;
	Sdf_st_overlapTransInfo **ppTransInfo;
	Sdf_st_error 		*pError;
#endif
{
	Sdf_st_listIterator dListIterator;


	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaRetrieveOverlapTransInfoByCseq");

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRetrieveOverlapTransInfoByCseq(): pError param passed "
			"is invalid");
#endif
		return Sdf_co_fail;
	}

	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveOverlapTransInfoByCseq( ): "
			"Invalid Call Object passed as parameter",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppTransInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRetrieveOverlapTransInfoByCseq( ): "
			"Invalid OverlapTransInfo passed as parameter",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

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
		/*
		 * Check against remote cseq if:
		 * 1. It is a request that has been recd. 		OR
		 * 2. It is a response that is being sent.
		 */
		if (((dMsgMode == Sdf_en_msgModeRecd) && \
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
		(void)sdf_listNext(&dListIterator, pError);
	}

	pError->errCode = Sdf_en_noExistError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaRetrieveOverlapTransInfoByCseq");
	return Sdf_co_fail;
}


#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormCallObjectKeyFromReplacesHeader
**
** DESCRIPTION: This API forms a call-Object-Key structure with the
**		information present in the Replaces header. It fills the
**		Call-Id, From-Tag & To-Tags of the CallObjectKey structure. The 
**		other fields of key are just initialised & returned to application.
**
** PARAMETERS:
**		pSipMsg(IN)		: The SipMessage that contains Replaces header
**		ppCallObjectKey(OUT): Call-object key 
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormCallObjectKeyFromReplacesHeader
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg,
	 Sdf_st_callObjectKey **ppCallObjectKey,
	 Sdf_st_error *pErr)
#else
	( pSipMsg, ppNewCallObject, pErr )
	 SipMessage *pSipMsg;
	 Sdf_st_callObject **ppNewCallObject;
	 Sdf_st_error *pErr;
#endif
{
	SipHeader 				dSipHdrReplaces;
	Sdf_ty_retVal   		dRetVal = Sdf_co_fail;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader");

#ifdef SDF_PARAMVALIDATION
#ifdef SDF_LINT
	if((ppCallObjectKey == Sdf_co_null) || (pSipMsg == Sdf_co_null))
#else    
	if((pErr == Sdf_co_null) || (ppCallObjectKey == Sdf_co_null) || \
		(pSipMsg == Sdf_co_null))
#endif        
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader( ):"
			"Invalid pError parameter passed to the function");
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
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
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to get Replaces header from Sip Message", pErr);
#endif
		return Sdf_co_fail;
	}

	if (Sdf_co_null != dSipHdrReplaces.pHeader)
	{
		SipHeader	*pFromHdr = Sdf_co_null;
		SipHeader	*pToHdr = Sdf_co_null;
		Sdf_ty_s8bit *pFromTag = Sdf_co_null;
		Sdf_ty_s8bit *pCfromTag = Sdf_co_null;
		Sdf_ty_s8bit *pToTag = Sdf_co_null;
		Sdf_ty_s8bit *pCtoTag = Sdf_co_null;
		Sdf_ty_s8bit *pCallId = Sdf_co_null;

		/* Get Call-Id, From-Tag, To-Tag from Replaces header */
		if (SipFail == sip_getFromTagFromReplacesHdr(&dSipHdrReplaces, \
					&pFromTag, (SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&dSipHdrReplaces);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		if (SipFail == sip_getToTagFromReplacesHdr(&dSipHdrReplaces, \
					&pToTag, (SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&dSipHdrReplaces);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		if (SipFail == sip_getCallidFromReplacesHdr(&dSipHdrReplaces, \
					&pCallId, (SipError *)&(pErr->stkErrCode)))
		{
			pErr->errCode = pErr->stkErrCode;
			sip_freeSipHeader(&dSipHdrReplaces);
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		sip_freeSipHeader(&dSipHdrReplaces);
		if (Sdf_co_fail == sdf_ivk_uaInitCallObjectKey(ppCallObjectKey, pErr))
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Set From-tag/To-tag/Call-id into C-O-Key */
		(*ppCallObjectKey)->pCallId = Sdf_mc_strdupCallHandler(pCallId);
		/* Init FROM header to set the tag */
		if (SipFail == sip_initSipHeader(&pFromHdr, SipHdrTypeFrom, 
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(*ppCallObjectKey);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to init FROM header ", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Init TO header to set the tag */
		if (SipFail == sip_initSipHeader(&pToHdr, SipHdrTypeTo, 
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(*ppCallObjectKey);
			sip_freeSipHeader(pFromHdr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to init TO header ", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Set from-tag in FROM header */
	 	pCfromTag = pFromTag;
		if (SipFail == sip_insertTagAtIndexInFromHdr(pFromHdr, pCfromTag, 0,
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(*ppCallObjectKey);
			sip_freeSipHeader(pFromHdr);
			sip_freeSipHeader(pToHdr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to set tag at index in From header ", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Set to-tag in TO header */
	 	pCtoTag = pToTag;
		if (SipFail == sip_insertTagAtIndexInToHdr(pToHdr, pCtoTag, 0,
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(*ppCallObjectKey);
			sip_freeSipHeader(pFromHdr);
			sip_freeSipHeader(pToHdr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader:"
				"Failed to set tag at index in TO header ", pErr);
#endif
			return Sdf_co_fail;
		}
		/* Set the from & to headers into Call-object-key */
		/* As Per replaces draft:
		 * "The to-tag parameter is compared to the local tag, and
		 * the from-tag parameter is compared to the remote tag."
		 */
        (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pCtoTag, pErr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pCfromTag, pErr);
		sip_freeSipHeader((*ppCallObjectKey)->pLocalAddr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)\
			&((*ppCallObjectKey)->pLocalAddr), pErr);
		sip_freeSipHeader((*ppCallObjectKey)->pRemoteAddr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)\
			&((*ppCallObjectKey)->pRemoteAddr), pErr);

		(*ppCallObjectKey)->pLocalAddr = pToHdr;
		(*ppCallObjectKey)->pRemoteAddr = pFromHdr;
		
		dRetVal = Sdf_co_success;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaFormCallObjectKeyFromReplacesHeader");
	return dRetVal;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormAndInsertReplaces
**
** DESCRIPTION: This API constructs a Replaces header. The Call-Id, From-tag
**		To-tag are taken from Call-Object, if they are not provided as
**		arguments.
**
** PARAMETERS:
**		pCallObj(IN)	: call object from which Call-id, from-tag, to-tag
**							are retrieved, if not provided as argumetns
**		pCallId(IN)		: Call-Id of call to be replaced
**		pToTag(IN)		: To-Tag of call to be replaced
**		pFromTag(IN)	: From-Tag of call to be replaced
**		pSipMsg(OUT)	: The SipMessage in which Replaces header is to be set
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormReplacesHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 Sdf_ty_s8bit *pCallId,
	 Sdf_ty_s8bit *pToTag,
	 Sdf_ty_s8bit *pFromTag,
	 SipHeader **ppReplacesHdr,
	 Sdf_st_error *pErr)
#else
	( pCallObj, pCallId, pToTag, pFromTag, ppReplacesHdr, pErr )
	 Sdf_st_callObject *pCallObj;
	 Sdf_ty_s8bit *pCallId;
	 Sdf_ty_s8bit *pToTag;
	 Sdf_ty_s8bit *pFromTag;
	 SipHeader **ppReplacesHdr;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_callObjectKey *pCallObjectKey = Sdf_co_null;
	Sdf_ty_s8bit *pNewCallId=Sdf_co_null;
	Sdf_ty_s8bit *pLocalTag = Sdf_co_null, *pRemoteTag = Sdf_co_null;
	Sdf_ty_s8bit *pClocalTag = Sdf_co_null, *pCremoteTag = Sdf_co_null;
	Sdf_st_error	dLocalErr;
	Sdf_ty_u32bit dCount=0;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
			"sdf_ivk_uaFormAndInsertReplaces");

#ifdef SDF_PARAMVALIDATION
	if((pErr == Sdf_co_null))
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaFormAndInsertReplaces( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	if (SipFail == sip_initSipHeader(ppReplacesHdr, SipHdrTypeReplaces, \
				(SipError *)&(pErr->stkErrCode)))
	{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				" age", pErr);
#endif
			return Sdf_co_fail;
	}
	
	/*
	 * If the call object is not null then take the param from callobject key
	 * else set it from the individual input param passed in this function
	 */
	if (Sdf_co_null != pCallObj)
	{
		if(sdf_ivk_uaGetCallObjectKeyFromCallObject(pCallObj, &pCallObjectKey,
				pErr) == Sdf_co_fail)
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode =  pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
			
	 	pNewCallId = Sdf_mc_strdupCallHandler(pCallObjectKey->pCallId);
		
		if (SipFail == sip_setCallidInReplacesHdr(*ppReplacesHdr,pNewCallId,  
				(SipError *)&(pErr->stkErrCode)))
		{
			(void)sdf_ivk_uaFreeCallObjectKey(pCallObjectKey);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pNewCallId, \
					&dLocalErr);
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode =  pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}

		/*==========Set From-tag in "Replaces" header===========*/
		if(pCallObjectKey->pLocalAddr->dType == SipHdrTypeFrom)
		{
				(void)sip_getTagCountFromFromHdr(pCallObjectKey->pLocalAddr, &dCount, 
				(SipError *)&(pErr->stkErrCode));
			if (dCount > 0)
				(void)sip_getTagAtIndexFromFromHdr(pCallObjectKey->pLocalAddr,
					&pLocalTag, 0, (SipError *)&(pErr->stkErrCode));
		}
		else if(pCallObjectKey->pLocalAddr->dType == SipHdrTypeTo)
		{
				(void)sip_getTagCountFromToHdr(pCallObjectKey->pLocalAddr, &dCount,
				(SipError *)&(pErr->stkErrCode));
			if (dCount > 0)
				(void)sip_getTagAtIndexFromToHdr(pCallObjectKey->pLocalAddr,
					&pLocalTag, 0, (SipError *)&(pErr->stkErrCode));
		}
	 	pClocalTag = pLocalTag;
		if (SipFail == sip_setFromTagInReplacesHdr(*ppReplacesHdr, pClocalTag, \
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(pCallObjectKey);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pNewCallId, \
					&dLocalErr);
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}

		/*==========Set To-tag in "Replaces" header===========*/
		
		if(pCallObjectKey->pRemoteAddr->dType == SipHdrTypeTo)
		{
/*w534*/(void)sip_getTagCountFromToHdr(pCallObjectKey->pRemoteAddr, &dCount,
				(SipError *)&(pErr->stkErrCode));
			if (dCount > 0)
				(void)sip_getTagAtIndexFromToHdr(pCallObjectKey->pRemoteAddr,
					&pRemoteTag, 0, (SipError *)&(pErr->stkErrCode));
		}
		if(pCallObjectKey->pRemoteAddr->dType == SipHdrTypeFrom)
		{
/*w534*/(void)sip_getTagCountFromFromHdr(pCallObjectKey->pRemoteAddr, &dCount, 
				(SipError *)&(pErr->stkErrCode));
			if (dCount > 0)
				(void)sip_getTagAtIndexFromFromHdr(pCallObjectKey->pRemoteAddr,
					&pRemoteTag, 0, (SipError *)&(pErr->stkErrCode));
		}
	 	pCremoteTag = pRemoteTag;
		if (SipFail == sip_setToTagInReplacesHdr(*ppReplacesHdr, pCremoteTag, \
					(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			/* Free Local Reference */
			(void)sdf_ivk_uaFreeCallObjectKey(pCallObjectKey);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
					&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		(void)sdf_ivk_uaFreeCallObjectKey(pCallObjectKey);
	}
	else
	{
		if((pCallId == Sdf_co_null) || (pFromTag == Sdf_co_null) || \
				(pToTag == Sdf_co_null))
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode = Sdf_en_invalidParamError; 
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		if (SipFail == sip_setCallidInReplacesHdr(*ppReplacesHdr, pCallId, \
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		
		/*==========Set From-tag in "Replaces" header===========*/
		if (SipFail == sip_setFromTagInReplacesHdr(*ppReplacesHdr, pFromTag, \
				(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
				&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}

		/*==========Set To-tag in "Replaces" header===========*/
		if (SipFail == sip_setToTagInReplacesHdr(*ppReplacesHdr, pToTag, \
					(SipError *)&(pErr->stkErrCode)))
		{
			/* Free Replaces Header */
			sip_freeSipHeader(*ppReplacesHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)ppReplacesHdr, \
					&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaFormAndInsertReplaces:"
				"Failed to get event header from Sip Message", pErr);
#endif
			return Sdf_co_fail;
		}
		/*==========Setting To-tag done===========*/
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pFromTag, pErr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pToTag, pErr);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaFormAndInsertReplaces");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetEarlyonlyInReplacesHdr
 **
 ** DESCRIPTION	: Inserts the "early-only" paramater in Replaces hedaer
 **				    
 **     PARAMETERS:
 **		pSipHeader(IN)	: Replaces header to which the param need to be added.
 **
 ** 	pError(OUT)		 : Error structure in case the function fails
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetEarlyonlyInReplacesHdr 
#ifdef ANSI_PROTO
	(SipHeader 			*pReplacesHeader, 
	 Sdf_st_error 		*pError)
#else
	(pSipHdr, pError)
	SipHeader 			*pReplacesHeader; 
	Sdf_st_error 		*pError;
#endif
{
    Sdf_ty_u32bit		dcount=0;
	SipParam *pParam = Sdf_co_null;

#ifdef SDF_PARAMVALIDATION
    if(pError == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
                "sdf_ivk_uaSetEarlyonlyInReplacesHdr():"
                "Error Param Passed is invalid ");
#endif
        return Sdf_co_fail;
    }

    if(pReplacesHeader == Sdf_co_null)
    {
#ifdef SDF_ERROR		
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit*)"sdf_ivk_uaSetEarlyonlyInReplacesHdr( ): "
                "Sip header passed is null", pError);
#endif		
        pError->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
#endif	/* End of param validation ifdef */	

    /* get the param count in replaces header */
    if (SipFail == sip_getParamCountFromReplacesHdr(pReplacesHeader,\
                &dcount ,(SipError *)&(pError->stkErrCode)))
    {
        pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaSetEarlyonlyInReplacesHdr(): "
                "Failed to get the param count in the replaces header", pError);
#endif
        return Sdf_co_fail;
    }
	(void)sip_initSipParam(&pParam,(SipError *)&(pError->stkErrCode));
	pParam->pName = Sdf_mc_strdupConfig("early-only");
	(void)sip_listInsertAt(&(((SipReplacesHeader *)(pReplacesHeader->pHeader))->slParam),dcount,pParam,(SipError *)&(pError->stkErrCode));
	
    sdf_fn_debug((Sdf_ty_s8bit *)\
            "SDF_DEBUG - Exiting sdf_ivk_uaSetEarlyOnlyInReplacesHdr");
    pError->errCode = Sdf_en_noUaError;

    return Sdf_co_success;	
}
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormURLStringFromReplacesHeader
**
** DESCRIPTION: This API converts Replaces header into a string. Core stack
**		API's are invoked for converting the header to string. Note that
**		special characters are escaped in the returned string.
**
** PARAMETERS:
**		pReplacesHdr(IN): Replaces header which has to be converted to string
**		ppUrlString(OUT): Escaped string equivalent of Replaces header
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormURLStringFromReplacesHeader
#ifdef ANSI_PROTO
	(SipHeader *pReplacesHdr,
	 Sdf_ty_s8bit **ppUrlString,
	 Sdf_st_error *pErr)
#else
	( pReplacesHdr, ppUrlString, pErr )
	 SipReplacesHeader *pReplacesHdr;
	 Sdf_ty_s8bit **ppUrlString;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal 	dRetVal = Sdf_co_fail;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Entering sdf_ivk_uaFormURLStringFromReplacesHeader");

#ifdef SDF_PARAMVALIDATION
	if (Sdf_co_null == pErr)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaFormURLStringFromReplacesHeader( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((Sdf_co_null == pReplacesHdr) || (Sdf_co_null == ppUrlString))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormURLStringFromReplacesHeader():"
			"Invalid pReplacesHdr/ppUrlString passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	*ppUrlString = Sdf_co_null;
	if (Sdf_co_null != pReplacesHdr)
	{
		Sdf_ty_s8bit 	*pReplacesStr = Sdf_co_null;
		SipReqHeader 	*pRequestHdr = Sdf_co_null;
		Sdf_st_error	dLocalErr;
#ifdef SDF_LINT
		Sdf_ty_u32bit dLen = 0;
#endif		
		
		pReplacesStr = (Sdf_ty_s8bit *)sdf_memget(0, \
				Sdf_co_largeBufferSize, pErr);
		
		Sdf_mc_strcpy(pReplacesStr, "");
		/* Stack doesnot have API to convert 'SipReplacesHeader' to
		 * string. So we first create a 'SipReqHeader' and set the
		 * "Replaces" header and then call the conversion API */
		pRequestHdr = (SipReqHeader *)sdf_memget(0, sizeof(SipReqHeader),
				pErr);
		pRequestHdr->pReplacesHdr = (SipReplacesHeader *)pReplacesHdr->pHeader;

		STR_CAT(pReplacesStr,(SIP_S8bit*) "Replaces:");
		STR_CAT(pReplacesStr, pRequestHdr->pReplacesHdr->pStr1);
		if(SipFail == sip_formSipParamList(pReplacesStr,&(pRequestHdr \
			->pReplacesHdr->slParam), (SIP_S8bit *) ";",1, \
			(SipError *)&pErr->stkErrCode))
		{
			pErr->errCode = pErr->stkErrCode;
			(void)sdf_memfree(0, (Sdf_ty_pvoid*)&pReplacesStr, &dLocalErr);
			(void)sdf_memfree(0, (Sdf_ty_pvoid*)&pRequestHdr, &dLocalErr);
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormURLStringFromReplacesHeader:"
			"Failed to convert 'Replaces' header into String", pErr);
#endif			
			return Sdf_co_fail;
		}
		(void)sdf_memfree(0, (Sdf_ty_pvoid*)&pRequestHdr, &dLocalErr);
		
		if (Sdf_co_null != pReplacesStr)
		{
			/* Remove the "Replaces:" from the converted string */
			Sdf_ty_s8bit pTemp[Sdf_co_largeBufferSize]="";
			Sdf_mc_strcpy(pTemp, pReplacesStr);
#ifdef SDF_LINT
            dLen = Sdf_mc_strlen("Replaces:");
            if(pReplacesStr != Sdf_co_null)            
    			Sdf_mc_strcpy(pReplacesStr, (pTemp+dLen));
#else            
			Sdf_mc_strcpy(pReplacesStr, pTemp+Sdf_mc_strlen("Replaces:"));
#endif            

			/* Escape the characters present in 'pReplacesStr' */
			*ppUrlString = sdf_fn_uaEscapeCharacters(pReplacesStr, pErr);
			(void)sdf_memfree(0, (Sdf_ty_pvoid*)&pReplacesStr, &dLocalErr);
			dRetVal = Sdf_co_success;
			
		}
	}
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaFormURLStringFromReplacesHeader");
	return dRetVal;
}



/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetReplacesHeaderFromReferTo
**
** DESCRIPTION: This API will extract any Replaces Header, which is 
**		embedded as URL parameter in the Refer-To header and will 
**		return a SipReplacesHeader structure.
**
** PARAMETERS:
**		pReferToHdr(IN)	: Refer-To header which contains escaped Replaces hdr
**		pReplacesHdr(OUT):Replaces header constructed out of Refer-To
**							header param
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetReplacesHeaderFromReferTo
#ifdef ANSI_PROTO
	(SipReferToHeader *pReferToHdr,
	 SipReplacesHeader **ppReplacesHdr,
	 Sdf_st_error *pErr)
#else
	( pReferToHdr, ppReplacesHdr, pErr )
	 SipReferToHeader *pReferToHdr;
	 SipReplacesHeader **ppReplacesHdr;
	 Sdf_st_error *pErr;
#endif
{
	SipHeader 		dSipHeaderReferTo;
	SipHeader 		*pSipHeaderReplaces = Sdf_co_null;
	SipAddrSpec 	*pReferToAddrSpec = Sdf_co_null;
	SipUrl		    *pSipUrl = Sdf_co_null;
	Sdf_ty_s8bit 	*pReplacesHdrStr = Sdf_co_null,*pTempStr = Sdf_co_null;
	Sdf_ty_s8bit	*pStringHdr=Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
						"sdf_ivk_uaGetReplacesHeaderFromReferTo");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaGetReplacesHeaderFromReferTo( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
	if ((Sdf_co_null == pReferToHdr) || (Sdf_co_null == ppReplacesHdr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetReplacesHeaderFromReferTo():"
			"Invalid Refer-To/replaces header passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	*ppReplacesHdr = Sdf_co_null;
	dSipHeaderReferTo.dType = SipHdrTypeReferTo;
	dSipHeaderReferTo.pHeader = pReferToHdr;

	if (sip_getAddrSpecFromReferToHdr(&dSipHeaderReferTo, &pReferToAddrSpec, \
								(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit *)"sdf_ivk_uaGetReplacesHeaderFromReferTo( ): "
								"Failed to get addrspec from Refer-To hdr", pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	if(sdf_ivk_uaGetUrlFromAddrSpec(pReferToAddrSpec,&pSipUrl,\
								pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
								(Sdf_ty_s8bit *)"sdf_ivk_uaGetReplacesHeaderFromReferTo( ): "
								"Failed to Url from from AddressSpec", pErr);
#endif
			sip_freeSipAddrSpec(pReferToAddrSpec);
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
	}

	if( pSipUrl != Sdf_co_null )
	{
		if(SipFail == sip_getHeaderFromUrl(pSipUrl,&pTempStr,(SipError
												*)&(pErr->stkErrCode)))
		{
#ifdef SDF_LINT
			if(*((SipError*)(pErr->stkErrCode)) != E_NO_EXIST)
#else				
			if(pErr->stkErrCode != E_NO_EXIST)
#endif					
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
												Sdf_en_headerManipulationError,\
												(Sdf_ty_s8bit *)"sdf_fn_uaCTgetReplacesHdrFromReferToParams( ): "
												"Failed to get Header from Url",pErr);
#endif
				pErr->errCode=Sdf_en_headerManipulationError;
				sip_freeSipAddrSpec(pReferToAddrSpec);
				sip_freeSipUrl(pSipUrl);
				return Sdf_co_fail;
			}
			pReplacesHdrStr = SIP_NULL;
		}
		else 
		{
			pReplacesHdrStr = Sdf_mc_strstr(pTempStr,"Replaces");
		}

		/*
		 * We have extracted the Replaces header value as string.
		 * 
		 * 1.	Now, construct a proper Replaces header and 
		 * 		construct a SipHeader out of the string. 
		 * 2.	Add this SipHeader to SipMessage.
		 * 3.	Delete the Url header from Refer-To header (coz, this
		 * 		shouldnt get added in ReqUri & To header, while
		 * 		constructing INVITE to transfer target)
		 */

		if(pReplacesHdrStr != SIP_NULL)
		{

			pStringHdr = (Sdf_ty_s8bit *)sdf_memget(0, \
										Sdf_mc_strlen(pReplacesHdrStr)+Sdf_mc_strlen("Replaces:")+1, pErr);
			if (Sdf_co_null == pStringHdr)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
												Sdf_en_headerManipulationError,\
												(Sdf_ty_s8bit *)"sdf_fn_uaCTgetReplacesHdrFromReferToParams( ): "
												"Failed to get memory",pErr);
#endif
				pErr->errCode = Sdf_en_noMemoryError;
				sip_freeSipAddrSpec(pReferToAddrSpec);
				sip_freeSipUrl(pSipUrl);
				return Sdf_co_fail;
			}

			Sdf_mc_strcpy(pStringHdr, "Replaces: ");
			Sdf_mc_strcat(pStringHdr, pReplacesHdrStr+strlen("Replaces="));

			/* Un-Escape the characters in Replaces header */
			(void)sdf_fn_uaUnescapeCharacters(pStringHdr, pErr);

			if (SipFail == sip_initSipHeader(&pSipHeaderReplaces, SipHdrTypeAny,\
												(SipError*)&(pErr->stkErrCode)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
												Sdf_en_headerManipulationError,\
												(Sdf_ty_s8bit *)"sdf_fn_uaCTgetReplacesHdrFromReferToParams( ): "
												"Failed to get memory",pErr);
#endif
				pErr->errCode = Sdf_en_headerManipulationError;
				(void)sdf_memfree(0, (Sdf_ty_pvoid *)&pStringHdr, pErr);
				sip_freeSipAddrSpec(pReferToAddrSpec);
				sip_freeSipUrl(pSipUrl);
				return Sdf_co_fail;
			}

			/* Convert the String to SipHeader */
			if (SipFail == sip_parseSingleHeader(pStringHdr, \
												SipHdrTypeReplaces,  pSipHeaderReplaces, \
												(SipError*)&(pErr->stkErrCode)))
			{
				(void)sdf_memfree(0, (Sdf_ty_pvoid *)&pStringHdr, pErr);
				sip_freeSipHeader(pSipHeaderReplaces);
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
				Sdf_en_headerManipulationError,\
												(Sdf_ty_s8bit *)"sdf_fn_uaCTgetReplacesHdrFromReferToParams( ): "
												"Failed to get memory",pErr);
#endif
				pErr->errCode = Sdf_en_headerManipulationError;
				sip_freeSipAddrSpec(pReferToAddrSpec);
				sip_freeSipUrl(pSipUrl);
				return Sdf_co_fail;
			}

			(void)sdf_memfree(0, (Sdf_ty_pvoid *)&pStringHdr, pErr);

			/* Delete nd remove the pHeader in SipUrl for Replaces */
			if(SipFail ==
										sip_setHeaderInUrl(pSipUrl,SIP_NULL,(SipError*)&pErr->stkErrCode))
			{
				sip_freeSipUrl(pSipUrl);
				sip_freeSipAddrSpec(pReferToAddrSpec);
				return Sdf_co_fail;
			}

			/* Store the "Replaces" Header in the return value */
			*ppReplacesHdr = (SipReplacesHeader *)pSipHeaderReplaces->pHeader;
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pSipHeaderReplaces, \
										pErr);
		} /* pReplacesHdrStr != SIP_NULL */
		sip_freeSipUrl(pSipUrl);
	}/* if pSipUrl is NonNull */
	sip_freeSipAddrSpec(pReferToAddrSpec);

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaGetReplacesHeaderFromReferTo");
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertReferredByHeader
**
** DESCRIPTION: This API will check the pReferredBy header parameter, 
**		if this is NULL, then It will form a default referredby header 
**		from the From Information of the InitData.
**
** PARAMETERS:
**		pCallObj(IN)		: call object from which ReferredBy header will
**								be constructed (out of FROM info), if it is
**								not specified in pReferredBy input argument.
**		pSipMsg(OUT)		: The SipMessage in which the header has to be set
**		pReferredByHdr(IN)	: Referred-BY header to be set in SipMessage
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInsertReferredByHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 SipMessage *pSipMsg,
	 SipHeader *pReferredByHdr,
	 Sdf_st_error *pErr)
#else
	( pCallObj, pSipMsg, pReferredByHdr, pErr )
	 Sdf_st_callObject *pCallObj;
	 SipMessage *pSipMsg;
	 SipHeader *pReferredByHdr;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_st_error	dLocalErr;
	Sdf_ty_bool		dIsReferredByCreated = Sdf_co_false;
		
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaInsertReferredByHeader");

#ifdef SDF_PARAMVALIDATION
	if (Sdf_co_null == pErr)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaInsertReferredByHeader( ):"
			"Invalid pError parameter passed to the function");
#endif
		return Sdf_co_fail;
	}

	if ((Sdf_co_null == pCallObj) && (Sdf_co_null == pReferredByHdr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInsertReferredByHeader():"
			"Invalid pCallObj/pReferredByHdr passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (Sdf_co_null == pSipMsg)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInsertReferredByHeader():"
			"Invalid SipMsg passed to the function",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* end-of-param-validation */
	
	if (Sdf_co_null == pReferredByHdr)
	{
		SipAddrSpec 	*pAddrSpec = Sdf_co_null;
		
		dIsReferredByCreated = Sdf_co_true;
		if (SipFail == sip_initSipHeader(&pReferredByHdr, SipHdrTypeReferredBy,\
			(SipError *)&pErr->stkErrCode))
		{
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertReferredByHeader:"
				"Failed to init 'Referred-By' header ", pErr);
#endif			
			return Sdf_co_fail;
		}

#ifdef SDF_LINT
    if (pCallObj == Sdf_co_null)
		{
			sip_freeSipHeader(pReferredByHdr);
      return Sdf_co_fail;
		}
#endif

		if (SipFail == sip_getAddrSpecFromFromHdr(pCallObj->pInitData->pFrom,\
					&pAddrSpec, (SipError *)&pErr->stkErrCode))
		{
			sip_freeSipHeader(pReferredByHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReferredByHdr, \
					&dLocalErr);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertReferredByHeader:"
				"Failed to get address spec from From header ", pErr);
#endif			
			return Sdf_co_fail;
		}
		if (SipFail == sip_setReferrerInReferredByHdr(pReferredByHdr, pAddrSpec, \
				(SipError *)&pErr->stkErrCode))
		{
			sip_freeSipHeader(pReferredByHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReferredByHdr, \
					&dLocalErr);
			sip_freeSipAddrSpec(pAddrSpec);
			pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertReferredByHeader:"
				"Failed to set Referrer in referred-by header ", pErr);
#endif			
			return Sdf_co_fail;
		}
		sip_freeSipAddrSpec(pAddrSpec);
	}
	
	if (SipFail == sip_insertHeaderAtIndex(pSipMsg, pReferredByHdr, 0, \
		(SipError *)&(pErr->stkErrCode)))
	{
		pErr->errCode = pErr->stkErrCode;
		if (dIsReferredByCreated == Sdf_co_true)
		{
			sip_freeSipHeader(pReferredByHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReferredByHdr, \
					&dLocalErr);
		}
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaInsertReferredByHeader:"
			"Failed to insert 'Referred-by' header", pErr);
#endif			
		return Sdf_co_fail;
	}
	/* Free local references */
	if (dIsReferredByCreated == Sdf_co_true)
	{
		sip_freeSipHeader(pReferredByHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pReferredByHdr, \
				&dLocalErr);
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaInsertReferredByHeader");
	return Sdf_co_success;
}
#endif /* SDF_REFER */

/******************************************************************************
** FUNCTION: sdf_ivk_uaGetUrlFromAddrSpec
**
** DESCRIPTION: This function retrieves the URL from a SIP AddrSpec.
**              It first checks if the URL scheme and calls the corresponding
**              stack API based on the scheme.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetUrlFromAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec  *pAddrSpec, 
	 SipUrl       **ppUrl, 
	 Sdf_st_error *pError)
#else	
	(pAddrSpec, ppUrl, pError)
	 SipAddrSpec  *pAddrSpec; 
	 SipUrl       **ppUrl; 
	 Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaGetUrlFromAddrSpec");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetUrlFromAddrSpec(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pAddrSpec == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetUrlFromAddrSpec(): "
			"AddrSpec Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppUrl == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetUrlFromAddrSpec(): "
			"URL pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

#ifdef SDF_TLS
	if (pAddrSpec->dType == SipAddrSipSUri)
	{
		if(sip_getSUrlFromAddrSpec(pAddrSpec, ppUrl,\
						(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->stkErrCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaGetUrlFromAddrSpec( ): "
				"Failed to get SUrl from AddrSpec.", pError);
#endif		
			return Sdf_co_fail;
		}
	}
	else
#endif /* SDF_TLS */
	if(sip_getUrlFromAddrSpec(pAddrSpec, ppUrl,\
				(SipError*)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->stkErrCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaGetUrlFromAddrSpec( ): "
			"Failed to get Url from AddrSpec.", pError);
#endif		
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaGetUrlFromAddrSpec");
	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_ivk_uaSetUrlInAddrSpec
**
** DESCRIPTION: This function sets the URL field in SIP AddrSpec.
**              It first checks if the URL scheme and calls the corresponding
**              stack API based on the scheme.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetUrlInAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec  *pAddrSpec, 
	 SipUrl       *pUrl, 
	 Sdf_st_error *pError)
#else	
	(pAddrSpec, pUrl, pError)
	 SipAddrSpec  *pAddrSpec; 
	 SipUrl       *pUrl; 
	 Sdf_st_error *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG - Entering sdf_ivk_uaSetUrlInAddrSpec");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetUrlInAddrSpec(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pAddrSpec == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetUrlInAddrSpec(): "
			"AddrSpec Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pUrl == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetUrlInAddrSpec(): "
			"URL pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

#ifdef SDF_TLS
	if (pAddrSpec->dType == SipAddrSipSUri)
	{
		if(sip_setSUrlInAddrSpec(pAddrSpec, pUrl,\
						(SipError*)&(pError->stkErrCode))==SipFail)
		{
			pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, pError->stkErrCode, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaSetUrlInAddrSpec( ): "
				"Failed to set SUrl in AddrSpec.", pError);
#endif		
			return Sdf_co_fail;
		}
	}
	else
#endif /* SDF_TLS */
	if(sip_setUrlInAddrSpec(pAddrSpec, pUrl,\
				(SipError*)&(pError->stkErrCode))==SipFail)
	{
		pError->errCode = pError->stkErrCode;
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, pError->stkErrCode, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetUrlInAddrSpec( ): "
			"Failed to set Url in AddrSpec.", pError);
#endif		
		return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *)\
		"SDF_DEBUG - Exiting sdf_ivk_uaSetUrlInAddrSpec");
	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

#ifdef SDF_TLS

/******************************************************************************
** FUNCTION: sdf_ivk_uaGetSslDataFromInitData
**
** DESCRIPTION: This function returns the SSL data from the InitData.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSslDataFromInitData
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	 Sdf_st_sslData  **ppSslData, 
	 Sdf_st_error    *pError)
#else	
	(pInitData, ppSslData, pError)
	 Sdf_st_initData *pInitData;
	 Sdf_st_sslData  **ppSslData;
	 Sdf_st_error    *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaGetSslDataFromInitData");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetSslDataFromInitData(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSslDataFromInitData(): "
			"InitData Param Passed to function is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(ppSslData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSslDataFromInitData(): "
			"SSL data pointer passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (pInitData->pSslData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSslDataFromInitData(): "
			"InitData data does not have a SSL data pointer.",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	else
	{
		*ppSslData = pInitData->pSslData;
        HSS_LOCKEDINCREF(pInitData->pSslData->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetSslDataFromInitData");
	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}


/******************************************************************************
** FUNCTION: sdf_ivk_uaSetSslDataInInitData
**
** DESCRIPTION: This function sets the SSL data in the InitData.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSslDataInInitData
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, 
	 Sdf_st_sslData  *pSslData, 
	 Sdf_st_error    *pError)
#else	
	(pInitData, pSslData, pError)
	 Sdf_st_initData *pInitData;
	 Sdf_st_sslData  *pSslData;
	 Sdf_st_error    *pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaSetSslDataInInitData");

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetSslDataInInitData():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pSslData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSslDataInInitData():  "
			"SslData Param Passed is invalid ",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSslDataInInitData():  "
			"InitData param is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	if (pInitData->pSslData != Sdf_co_null)
	{
		(void)sdf_ivk_uaFreeSslData(pInitData->pSslData);
	}

	pInitData->pSslData = pSslData;
	HSS_LOCKEDINCREF(pInitData->pSslData->dRefCount);

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaSetSslDataInInitData");
	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}

#endif /* SDF_TLS */


/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDestTransport
**
** DESCRIPTION: Sets the destination transport information (IP/port) for
**	the call. This may change during the call due to presence of Contact
**	headers and Record-Routes.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDestTransport
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObj,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u32bit dPort,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pErr)
#else
	( pCallObj,pAddr,dPort, pScheme, pErr )
	 Sdf_st_callObject *pCallObj;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u32bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal	retVal;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDestTransport");

	retVal = sdf_ivk_uaSetDestTransportInTransaction (pCallObj->pUacTransaction,
				Sdf_co_null, pAddr, dPort, pScheme, pErr);


	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDestTransport");
	
	return retVal;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetDestTransportInTransaction
**
** DESCRIPTION: Sets the destination transport information (IP/port) for
**	the call. This may change during the call due to presence of Contact
**	headers and Record-Routes.
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetDestTransportInTransaction
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
     Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 Sdf_ty_s8bit *pAddr,
	 Sdf_ty_u32bit dPort,
	 Sdf_ty_s8bit *pScheme,
	 Sdf_st_error *pErr)
#else
	( pTransaction,pOverlapTransInfo,pAddr,dPort, pScheme, pErr )
	 Sdf_st_transaction *pTransaction;
	 Sdf_st_overlapTransInfo *pOverlapTransInfo;
	 Sdf_ty_s8bit *pAddr;
	 Sdf_ty_u32bit dPort;
	 Sdf_ty_s8bit *pScheme;
	 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal	dRetVal;
	(void)pAddr;
	(void)dPort;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaSetDestTransportInTransaction");
	
	dRetVal = sdf_ivk_uaSetTransportSchemeInTransaction(pTransaction, \
				pOverlapTransInfo, pScheme, pErr);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetDestTransportInTransaction");

	return dRetVal;
}



#ifdef SDF_CONGEST
/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertRsrcPriorityHeader
**
** DESCRIPTION: This API takes Resource-Priority header in the form of a
**              string and inserts it into the passed SipMessage.
**              The header formed will be inserted into the persistent
**              header list of the call object.
** PARAMETERS:
**      pCallObj    (IN): Call object to which the Resource Priority hdr
**                         has to be added in persistent headerlist.
**		pSipMsg     (IN) :sipMessage to which the Header to added
**		pRsrcPriorityStr (IN) :resource priority string which is used to 
**                              form Resource Priority hdr
**		pErr                   (OUT):Error structure in case the function fail
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertRsrcPriorityHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
     SipMessage *pSipMsg,
	 Sdf_ty_s8bit *pRsrcPriorityStr,
	 Sdf_st_error *pErr)
#else
	(pCallObj,pSipMsg, pRsrcPriorityStr,pErr )
     Sdf_st_callObject *pCallObj,
	 SipMessage *pSipMsg,
	 Sdf_ty_s8bit *pRsrcPriorityStr,
	 Sdf_st_error *pErr;
#endif
{
	SipHeader *pHeader = Sdf_co_null;
    
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaInsertRsrcPriorityHeader");

#ifdef SDF_PARAMVALIDATION
    if((pErr == Sdf_co_null))
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaInsertRsrcPriorityHeader( ):"
                "Invalid pError parameter passed to the function");
#endif
        return Sdf_co_fail;
    }
    if (Sdf_co_null == pSipMsg)
    {
    #ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaInsertRsrcPriorityHeader():"
                "Invalid SipMsg passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif	/* end-of-param-validation */

   	(void)Sdf_mc_strtok(pRsrcPriorityStr, "\n");
	/* Allocate memory for the SipHeader structure*/
	pHeader = (SipHeader *) sdf_memget(Sdf_mc_initMemId, \
            sizeof(SipHeader), Sdf_co_null);
    
    if ( SipFail == sip_parseSingleHeader(pRsrcPriorityStr,SipHdrTypeRsrcPriority,pHeader,(SipError*)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertRsrcPriorityHeader:"
                "parsesingleheader failed for Resource-Priority Hdr", pErr);
#endif
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);        
        return Sdf_co_fail;
    }
    
    if (SipFail == sip_insertHeaderAtIndex(pSipMsg,pHeader,0,(SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertRsrcPriorityHeader:"
                "Failed to insert Resource-Priority Hdr", pErr);
#endif
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);        
        return Sdf_co_fail;
        
    }
    
    if ( Sdf_co_fail == sdf_fn_uaAddHeaderInPersistentHdrList(pCallObj,pHeader,SipHdrTypeRsrcPriority,"uatk-Rsrcpriority",pErr))
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
                (Sdf_ty_s8bit *)"sdf_ivk_uaInsertRsrcPriorityHeader(): "
                "sdf_fn_uaAddHeaderInPersistentHdrList ",pErr);
#endif
        pErr->errCode=Sdf_en_headerManipulationError;
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);        
        return Sdf_co_fail;        

    }
    
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaInsertRsrcPriorityHeader");
    return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertAcceptRsrcPriorityHeader
**
** DESCRIPTION: This API takes Accept-Resource-Priority header in the form of a
**              string and inserts it into the passed SipMessage.
** PARAMETERS:
**		pSipMsg                (IN) :sipMessage to which the Header to added
**		pAcceptRsrcPriorityStr (IN) :Accept resource priority string to be
**                                   used to form Accept Resource Priority hdr
**		pErr                   (OUT):Error structure in case the function fail
**
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertAcceptRsrcPriorityHeader
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg,
	 Sdf_ty_s8bit *pAcceptRsrcPriorityStr,
	 Sdf_st_error *pErr)
#else
	(pSipMsg, pAcceptRsrcPriorityStr,pErr )
	 SipMessage *pSipMsg,
	 Sdf_ty_s8bit *pAcceptRsrcPriorityStr,
	 Sdf_st_error *pErr;
#endif
{
		SipHeader *pHeader = Sdf_co_null;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
						"sdf_ivk_uaInsertAcceptRsrcPriorityHeader");

#ifdef SDF_PARAMVALIDATION
		if((pErr == Sdf_co_null))
		{
#ifdef SDF_ERROR
				sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
								"sdf_ivk_uaInsertAcceptRsrcPriorityHeader( ):"
								"Invalid pError parameter passed to the function");
#endif
				return Sdf_co_fail;
		}
		if (Sdf_co_null == pSipMsg)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
								(Sdf_ty_s8bit *)"sdf_ivk_uaInsertAcceptRsrcPriorityHeader():"
								"Invalid SipMsg passed to the function",pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
		}

#endif	/* end-of-param-validation */

		(void)Sdf_mc_strtok(pAcceptRsrcPriorityStr, "\n");
		/* Allocate memory for the SipHeader structure*/
		pHeader = (SipHeader *) sdf_memget(Sdf_mc_initMemId, \
						sizeof(SipHeader), Sdf_co_null);

		if ( SipFail == sip_parseSingleHeader(pAcceptRsrcPriorityStr, \
								SipHdrTypeAcceptRsrcPriority,pHeader,(SipError*)&(pErr->stkErrCode)))
		{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
								(Sdf_ty_s8bit*)"sdf_ivk_uaInsertAcceptRsrcPriorityHeader:"
								"parsesingleheader failed for Resource-Priority Hdr", pErr);
#endif
				(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);        
				return Sdf_co_fail;
		}

		if (SipFail == sip_insertHeaderAtIndex(pSipMsg,pHeader,0, \
								(SipError *)&(pErr->stkErrCode)))
		{
				pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
								(Sdf_ty_s8bit*)"sdf_ivk_uaInsertAcceptRsrcPriorityHeader:"
								"Failed to insert Resource-Priority Hdr", pErr);
#endif
				(void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHeader,pErr);        
				return Sdf_co_fail;

		}

		pErr->errCode = Sdf_en_noUaError;
		sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
						"sdf_ivk_uaInsertAcceptRsrcPriorityHeader");
		return Sdf_co_success;
}

#endif

#ifdef SDF_3GPP
/*******************************************************************
** FUNCTION: sdf_ivk_uaInsertHistoryInfoHeader
**
** DESCRIPTION: This API takes callobject,Addrspec and History index as input
**              parameters and calls the function sdf_fn_uaAddHistoryInfoHeader
**              which forms History info header and inserts it into 
**              persistent hdrs list of call object.This ivk will be invoked
**              only if there is no historyinfo header in the message.
**
** PARAMETERS:
**      pCallObj    (IN) :Callobject to which this header be added in the
**                        persistent header list
**		pSipMsg     (IN) :sipMessage to which the Header to added
**		pAddrSpec   (IN) :AddressSpec to be used to form History-Info hdr
**      pIndex      (IN) :Index to be used to form the History-Info hdr
**		pErr        (OUT):Error structure in case the function fail
**
**
*******************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertHistoryInfoHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
	 Sdf_st_error *pErr)
#else
	(pCallObj,pSipMsg,pAddrSpec,pIndex,pErr )
     Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
	 Sdf_st_error *pErr;
#endif
{
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaInsertHistoryInfoHeader");

    if(sdf_fn_uaAddHistoryInfoHeader(pCallObj,pSipMsg,pAddrSpec,pHistoryIndex,0,pErr) == Sdf_co_fail )
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertHistoryInfoHeader:"
                "Failed to Add HistoryInfo ", pErr);
#endif
        return Sdf_co_fail;             
    }

    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaInsertHistoryInfoHeader");
    return Sdf_co_success;
    
}
/*******************************************************************
** FUNCTION: sdf_ivk_uaAddHistoryInfo
**
** DESCRIPTION: This API takes callobject,Addrspec and History index as input
**              parameters and calls the function sdf_fn_uaAddHistoryInfoHeader
**              which forms History info header and inserts it into 
**              persistent hdrs list of call object.This ivk will be invoked
**              only if there is atleast one historyinfo header in the message.
**              So, this ivk will be used to add a history info value to
**              the header.
**
** PARAMETERS:
**      pCallObj    (IN) :Callobject to which this header be added in the
**                        persistent header list
**		pSipMsg     (IN) :sipMessage to which the Header to added
**		pAddrSpec   (IN) :AddressSpec to be used to form History-Info hdr
**      pIndex      (IN) :Index to be used to form the History-Info hdr
**		pErr        (OUT):Error structure in case the function fail
**
**
*******************************************************************/

Sdf_ty_retVal sdf_ivk_uaAddHistoryInfo
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
	 Sdf_st_error *pErr)
#else
	(pCallObj,pSipMsg,pAddrSpec,pIndex,pErr )
     Sdf_st_callObject *pCallObj,
	 SipMessage  *pSipMsg,
	 SipAddrSpec *pAddrSpec,
	 Sdf_ty_s8bit *pHistoryIndex,
	 Sdf_st_error *pErr;
#endif
{
    Sdf_ty_u32bit      dListSize=0;

    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaAddHistoryInfo");
#ifdef SDF_PARAMVALIDATION
    if((pErr == Sdf_co_null))
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaAddHistoryInfo( ):"
                "Invalid pError parameter passed to the function");
#endif
        return Sdf_co_fail;
    }
    if (Sdf_co_null == pCallObj)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaAddHistoryInfo():"
                "Invalid CallObject passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
    
    if (Sdf_co_null == pSipMsg)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaAddHistoryInfo():"
                "Invalid SipMsg passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
    if (Sdf_co_null == pAddrSpec)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaAddHistoryInfo():"
                "Invalid AddressSpec passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }
    if (Sdf_co_null == pHistoryIndex)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaAddHistoryInfo():"
                "Invalid History-Index passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif	/* end-of-param-validation */
	(void)sip_listSizeOf(&(pSipMsg->pGeneralHdr->slHistoryInfoHdr),&dListSize,\
                         (SipError *)&(pErr->stkErrCode));
    if (dListSize == 0)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHistoryInfo:"
                "History-Info header not present ", pErr);

#endif
        return Sdf_co_fail;        
    }
    if(sdf_fn_uaAddHistoryInfoHeader(pCallObj,pSipMsg,pAddrSpec,pHistoryIndex,dListSize,pErr) == Sdf_co_fail )
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHistoryInfo:"
                "Failed to Add HistoryInfo ", pErr);
#endif
        return Sdf_co_fail;             
    }
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaAddHistoryInfo");
    return Sdf_co_success;
    
}
/*******************************************************************
** FUNCTION: sdf_ivk_uaAddHeaderParamToLastHistoryInfo
**
** DESCRIPTION: When the INVITE gets redirected, the new INVITE can have 
**              header params in it's last history info header.This IVK 
**              will take headerparams as input and add it to the last 
**              History-Info header.
**		pSipMsg     (IN) :sipMessage to which the Header to added
**      pHeaderParam(IN) :header param to be inserted in the History-Info hdr
**		pErr        (OUT):Error structure in case the function fail
**
*******************************************************************/


Sdf_ty_retVal sdf_ivk_uaAddHeaderParamToLastHistoryInfo
    (SipMessage  *pSipMsg,
	 Sdf_ty_s8bit *pHeaderParam,
	 Sdf_st_error *pErr)
{
    Sdf_ty_u32bit      dListSize=0;
    SipHeader          *pHistHeader = Sdf_co_null;
    SipAddrSpec        *pAddrSpec = Sdf_co_null;
    SipUrl             *pSipUrl  =  Sdf_co_null;    

        sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaAddHeaderParamToLastHistoryInfo");
#ifdef SDF_PARAMVALIDATION
    if((pErr == Sdf_co_null))
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
                "sdf_ivk_uaAddHeaderParamToLastHistoryInfo( ):"
                "Invalid pError parameter passed to the function");
#endif
        return Sdf_co_fail;
    }
    if (Sdf_co_null == pSipMsg)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo():"
                "Invalid SipMsg passed to the function",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif	/* end-of-param-validation */
	(void)sip_listSizeOf(&(pSipMsg->pGeneralHdr->slHistoryInfoHdr),&dListSize,\
                         (SipError *)&(pErr->stkErrCode));
    if (dListSize == 0)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to Add HistoryInfo ", pErr);

#endif
        return Sdf_co_fail;        
    }
    if (SipFail == sip_initSipHeader(&pHistHeader, SipHdrTypeAny,\
                (SipError *)&pErr->stkErrCode))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to init 'History-Info' header ", pErr);
#endif			
        return Sdf_co_fail;
    }
	if ( sip_getHeaderAtIndex(pSipMsg,SipHdrTypeHistoryInfo,pHistHeader, \
                dListSize-1,(SipError *)&(pErr->stkErrCode)) == SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to get 'History-Info' header ", pErr);
#endif
/*        sip_freeSipHeader(pHistHeader);				*/
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistHeader,pErr);

        return Sdf_co_fail;
    }
	if (sip_getAddrSpecFromHistoryInfoHdr(pHistHeader,&pAddrSpec, \
                (SipError *)&(pErr->stkErrCode)) == SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to get addrspec from 'History-Info' header ", pErr);
#endif			
        sip_freeSipHeader(pHistHeader);				
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistHeader,pErr);        
        return Sdf_co_fail;
    }
        
    if (sip_getUrlFromAddrSpec(pAddrSpec,&pSipUrl,(SipError
                    *)&(pErr->stkErrCode)) == SipFail )
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to get Url from addrspec ", pErr);
#endif			
        sip_freeSipAddrSpec(pAddrSpec);
        sip_freeSipHeader(pHistHeader);				
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistHeader,pErr);
        return Sdf_co_fail;
        
    }
        
    if ( sip_setHeaderInUrl(pSipUrl,pHeaderParam,(SipError
                    *)&(pErr->stkErrCode)) == SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaAddHeaderParamToLastHistoryInfo:"
                "Failed to set header param in url ", pErr);
#endif
        sip_freeSipUrl(pSipUrl);
        sip_freeSipAddrSpec(pAddrSpec);
        sip_freeSipHeader(pHistHeader);				
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistHeader,pErr);        
        return Sdf_co_fail;
    }
        
    sip_freeSipUrl(pSipUrl);
    sip_freeSipAddrSpec(pAddrSpec);
    
    sip_freeSipHeader(pHistHeader);				
    (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pHistHeader,pErr);
    
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaAddHeaderParamToLastHistoryInfo:");
    return Sdf_co_success;
}

#endif
#ifdef SDF_PEMEDIA
/********************************************************************
** FUNCTION: sdf_ivk_uaInsertPEarlyMediaHeader
**
** DESCRIPTION: This API constructs a p-early-Media  header.The direction 
** 				is used to form the header.
**              The header formed will be inserted into the persistent
**              header list of the call object.
**
** PARAMETERS:
**      pCallObj (IN) : Call object to which this header is added in the
**                      persistent header list
**      pSipMsg   (IN) :sipMessage to which the Header to added
**		pDirection (IN): direction to be used to form the P-Early-Media hdr
**		pErr(OUT)		: Error structure in case the function fails
**
********************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertPEarlyMediaHeader
#ifdef ANSI_PROTO
  ( Sdf_st_callObject *pCallObj,
    SipMessage  *pSipMsg,
	Sdf_ty_s8bit *pDirection,
	Sdf_st_error *pErr)
#else
	(pCallObj,pSipMsg,pDirection,pErr )
    Sdf_st_callObject *pCallObj;
	SipMessage *pSipMsg;
    Sdf_ty_s8bit *pDirection;
	Sdf_st_error *pErr;
#endif
{
	SipHeader       *pPEarlyMediaHdr = Sdf_co_null;
	en_SipMessageType dType;    
    Sdf_st_error	dLocalErr;
    
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering "
            "sdf_ivk_uaInsertpEarlyMediaHeader");

#ifdef SDF_PARAMVALIDATION
    if((pErr == Sdf_co_null))
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -\
                sdf_ivk_uaInsertpEarlyMediaHeader( ): Invalid pError parameter passed \
                to the function");
#endif
        return Sdf_co_fail;
    }

    if(pSipMsg== Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -\
                sdf_ivk_uaInsertpEarlyMediaHeader( ): Invalid pSipMsg parameter \
                passed	to the function");
#endif
        return Sdf_co_fail;
    }

    if(pDirection == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR -\
                sdf_ivk_uaInsertpEarlyMediaHeader( ): Invalid pDirection parameter \
                passed	to the function");
#endif
        return Sdf_co_fail;
    }
#endif	/* end-of-param-validation */


    if (SipFail == sip_initSipHeader(&pPEarlyMediaHdr,
                SipHdrTypePEarlyMedia,\
                (SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertpEarlyMediaHeader:"
                " age", pErr);
#endif
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pDirection,pErr);
        return Sdf_co_fail;
    }

/*Set the direction in p-Early-Media Header*/
    if(sip_setEarlyInPEarlyMediaHdr(pPEarlyMediaHdr,pDirection, \
                (SipError *)&(pErr->stkErrCode)) == SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR			
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertpEarlyMediaHeader:"
                "Failed to set direction in pEarlyMedia Header", pErr);
#endif
        /* Free P-early-Media Header */
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pDirection,pErr);
        sip_freeSipHeader(pPEarlyMediaHdr);	
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pPEarlyMediaHdr,&dLocalErr);
        return Sdf_co_fail;
    }

    /* insert the formed P-early-Media Header header into the message passed */
    if(sip_insertHeaderAtIndex(pSipMsg,pPEarlyMediaHdr,0, \
                (SipError*)&(pErr->stkErrCode))==SipFail)
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertpEarlyMediaHeader:"
                "Failed to insert pEarlyMedia Header", pErr);
#endif
        sip_freeSipHeader(pPEarlyMediaHdr);	
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pPEarlyMediaHdr,&dLocalErr);
        return Sdf_co_fail;
    }
    if ( SipFail == sip_getMessageType(pSipMsg,&dType,(SipError*)&(pErr->stkErrCode)) )
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertpEarlyMediaHeader:"
                "Failed to get the message type", pErr);
#endif
        sip_freeSipHeader(pPEarlyMediaHdr);	
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pPEarlyMediaHdr,&dLocalErr);
        return Sdf_co_fail;
      
    }
    
    if (dType == SipMessageRequest)
    {    
        if ( Sdf_co_fail == sdf_fn_uaAddHeaderInPersistentHdrList(pCallObj, \
                    pPEarlyMediaHdr,SipHdrTypePEarlyMedia,"uatk-Pearlymedia",pErr))
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
                    (Sdf_ty_s8bit *)"sdf_ivk_uaInsertpEarlyMediaHeader(): "
                    "sdf_fn_uaAddHeaderInPersistentHdrList ",pErr);
#endif
            sip_freeSipHeader(pPEarlyMediaHdr);	
            (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pPEarlyMediaHdr,&dLocalErr);
            pErr->errCode=Sdf_en_headerManipulationError;
            return Sdf_co_fail;        

        }
    }
    else
    {
        sip_freeSipHeader(pPEarlyMediaHdr);	
        (void)sdf_memfree(0,(Sdf_ty_pvoid*)&pPEarlyMediaHdr,&dLocalErr);
    }
    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaInsertpEarlyMediaHeader:");
    return Sdf_co_success;
}
#endif /* SDF_PEMEDIA */
#ifdef SDF_CONF  /*SDF_CONF */
/*****************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetIsfocusInContactHeader
 **
 ** DESCRIPTION	: Inserts the "isfocus" paramater in Contact header
 **				    
 **     PARAMETERS:
 **		pContactHeader(IN): Contact header to which the param need to be added.
 ** 	pError(OUT)		 : Error structure in case the function fails
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetIsfocusInContactHeader 
#ifdef ANSI_PROTO
	(SipHeader 			*pContactHeader, 
	 Sdf_st_error 		*pError)
#else
	(pSipHdr, pError)
	SipHeader 			*pContactHeader; 
	Sdf_st_error 		*pError;
#endif
{
	Sdf_ty_u32bit  dcount=0;
	SipParam *pTempParam = Sdf_co_null;
	Sdf_ty_s8bit *pTmpValue = Sdf_co_null;	
	
#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaSetIsfocusInContactHeader():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	
	if(pContactHeader == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetIsfocusInContactHeader (): "
			"Sip header passed is null", pError);
#endif		
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */	

	/* get the param count in contact header */
	if (SipFail == sip_getParamCountFromContactHdr(pContactHeader,\
			&dcount ,(SipError *)&(pError->stkErrCode)))
	{
		pError->errCode = pError->stkErrCode;
/*		sip_freeSipHeader(pContactHeader);*/
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetIsfocusInContactHeader (): Failed to \
			get the param count in the Contact header", pError);
#endif
		return Sdf_co_fail;
	}
	
	/* set the isfocus parameter in sipContactParam structure */
	pTmpValue = Sdf_mc_strdupCallHandler("isfocus");
    if (sip_initSipParam(&pTempParam, \
                (SipError *)&(pError->stkErrCode)) == SipFail)
    {
        pError->stkErrCode = pError->stkErrCode;
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pTmpValue,pError);
#ifdef SDF_ERROR					
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
		(Sdf_ty_s8bit *)"sdf_ivk_uaSetIsfocusInContactHeader: "
		"Failed to init a SipParam structure.",pError);
#endif	
		return Sdf_co_fail;        
    }
	if (sip_setNameInSipParam(pTempParam, pTmpValue, \
				(SipError *)&(pError->stkErrCode)) == SipFail)
	{
		pError->errCode = pError->stkErrCode ;
#ifdef SDF_ERROR					
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
		(Sdf_ty_s8bit *)"sdf_ivk_uaSetIsFocusInContactHeader(): "
		"Failed to set isfocus name in a contact param structure.",pError);
#endif
        (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pTmpValue,pError);
		sip_freeSipParam(pTempParam);
		return Sdf_co_fail;
	}
	
	if (SipFail == sip_insertParamAtIndexInContactHdr(pContactHeader,
			pTempParam, dcount, (SipError *)&(pError->stkErrCode)))
	{
		pError->errCode = pError->stkErrCode;
/*		sip_freeSipHeader(pContactHeader);*/
   		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid*)&pTmpValue,pError);
		sip_freeSipParam(pTempParam);
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, pError->errCode,\
			(Sdf_ty_s8bit*)"sdf_ivk_uaSetIsfocusInContactHeader (): Failed \
			to set isfocus parameter into the Contact header ", pError);
#endif
		return Sdf_co_fail ;
	}
    sip_freeSipParam(pTempParam);
	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting \
									sdf_ivk_uaSetIsfocusInContactHeader:");
	return Sdf_co_success;	
}

#endif /* SDF_CONF */

#ifdef SDF_PRIVACY
/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertPrivacyHeader
**
** DESCRIPTION: This API constructs a Privacy header. It takes the inputs 
** from the application and sets it into the Sip Message
**
** PARAMETERS:
**      pCallObject(IN): Callobject to store the Privacy hdr into persistent
**                        header list. 
**		pSipMsg(IN)	    : The SipMessage in which Privacy header is to be set
**      pPrivacyHdrStr(IN)	: The header value for privacy header.
**		pErr(OUT)		: Error structure in case the function fails

*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertPrivacyHeader
 ( Sdf_st_callObject	*pCallObject,
   SipMessage  	*pSipMsg,
   Sdf_ty_s8bit	*pPrivacyHdrStr,
   Sdf_st_error  *pErr)
{
	SipHeader *pHdr = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *) \
			"SDF_DEBUG - Entering sdf_ivk_uaInsertPrivacyHeader");
	/*  Parameter validations begin */
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
				"sdf_ivk_uaInsertPrivacyHeader():  Error Param Passed is "
				"invalid ");
#endif
		return Sdf_co_fail;
	}

	if (pSipMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInsertPrivacyHeader():  "
				"SipMessage passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

	if (pPrivacyHdrStr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInsertPrivacyHeader():  "
				"Privacy header string passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End of param validations */

	(void)Sdf_mc_strtok(pPrivacyHdrStr, "\n");
	if (SipFail == sip_initSipHeader(&pHdr, SipHdrTypeAny,\
				(SipError *)&pErr->stkErrCode))
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertPrivacyHeader:"
				"Failed to init 'Privacy' header ", pErr);
#endif
		return Sdf_co_fail;
	}

	/* Form the header from the string */			
	if(sip_parseSingleHeader(pPrivacyHdrStr,SipHdrTypePrivacy,\
				pHdr,(SipError *)&(pErr->stkErrCode))==SipFail)
	{
		pErr->errCode=Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInsertprivacyheader():  "
				"parsing Privacy Header failed ",pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
		return Sdf_co_fail;
	}

	if (SipFail == sip_insertHeaderAtIndex(pSipMsg, pHdr, 0, \
				(SipError *)&(pErr->stkErrCode)))
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertPrivacyHeader:"
				"Failed to insert 'Privacy' header", pErr);
#endif		
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);

		return Sdf_co_fail;
	}
	/*
	 * Store the Privacy Header as part of Persistent Header List.
	 * If this INVITE got chanllenged, we can extract the Privacy
	 * header and resend the INVITE
	 */

	if(sdf_fn_uaAddHeaderInPersistentHdrList(pCallObject,\
				pHdr,SipHdrTypePrivacy, "uat-Privacy",pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertPrivacyHeader:" \
				"Failed to insert 'Privacy' header into " \
				"persistent header list", pErr);
#endif			
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
		return Sdf_co_fail;
	} 
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
			"sdf_ivk_uaInsertPrivacyheader");
	return Sdf_co_success;
}
  

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInsertPPreferredIdHdr
**
** DESCRIPTION:This API constructs a P-Preferred-Id header. It takes the 
**  Display name and address spec from the application and sets it 
**  into the Sip Message
**
** PARAMETERS:
**	  pCallObject(IN): Callobject to store the PPId hdr into persistent
**                        header list. 
**    pSipMsg(IN)	 : The SipMessage in which PPId header is to be set
**    pDispName(IN)	 : The Display name for PPId header.
**    pAddrSpec(IN)	 : The Address Spec for  PPId header.
**	  pErr(OUT)		 : Error structure in case the function fails

*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInsertPPreferredIdHdr
( Sdf_st_callObject	*pCallObject,
  SipMessage  	    *pSipMsg,
  Sdf_ty_s8bit	    *pDispName,
  SipAddrSpec 	    *pAddrSpec,
  Sdf_st_error      *pErr)
{
    SipHeader *pHdr = Sdf_co_null;

    sdf_fn_debug((Sdf_ty_s8bit *) \
            "SDF_DEBUG - Entering sdf_ivk_uaAddPrivacyHeader");


    /* ================ Parameter validations begin ================== */
#ifdef SDF_PARAMVALIDATION
    if (pCallObject == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
                "sdf_ivk_uaInsertPPreferredIdHdr():  pCallObject Passed is "
                "invalid ");
#endif
        return Sdf_co_fail;
    }

    if (pErr == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
                "sdf_ivk_uaInsertPPreferredIdHdr():  Error Param Passed is "
                "invalid ");
#endif
        return Sdf_co_fail;
    }


    if (pSipMsg == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaInsertPPreferredIdHdr:  "
                "SipMessage passed is invalid ",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

    if (pAddrSpec == Sdf_co_null)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
                (Sdf_ty_s8bit *)"sdf_ivk_uaInsertPPreferredIdHdr():  "
                "Privacy header string passed is invalid ",pErr);
#endif
        pErr->errCode=Sdf_en_invalidParamError;
        return Sdf_co_fail;
    }

#endif 
    /* ================ End of Parameter validations ================== */

    if (SipFail == sip_initSipHeader(&pHdr, SipHdrTypePPreferredId,\
                (SipError *)&pErr->stkErrCode))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertPPreferredIdHeader:"
                "Failed to init 'PPreferred-ID' header ", pErr);
#endif			
        return Sdf_co_fail;
    }

    if(pDispName != Sdf_co_null)
    {	
		if(SipFail == sip_setDispNameInPPreferredIdHdr( pHdr, \
                    pDispName, (SipError *)&(pErr->stkErrCode)))
        {
            pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                    (Sdf_ty_s8bit*)"sdf_ivk_uaInsertPPreferredIdHdr:"
                    "Failed to get value count from 'Privacy' header", pErr);
#endif		
			sip_freeSipHeader(pHdr);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
            return Sdf_co_fail;
        }
    }

    if(SipFail == sip_setAddrSpecInPPreferredIdHdr ( pHdr, \
                pAddrSpec, (SipError *)&(pErr->stkErrCode)))
	{
		pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit*)"sdf_ivk_uaInsertPPreferredIdHdr:"
				"Failed to get value count from 'Privacy' header", pErr);
#endif
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
		return Sdf_co_fail;
	}

    if (SipFail == sip_insertHeaderAtIndex(pSipMsg, pHdr, 0, \
                (SipError *)&(pErr->stkErrCode)))
    {
        pErr->errCode = pErr->stkErrCode;
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertPPreferredIdHdr:"
                "Failed to insert 'PPID' header", pErr);
#endif	
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
        return Sdf_co_fail;
    }

    /*
     * Store the P-Preffered-Id Header as part of Persistent Header List.
     * If this INVITE got chanllenged, we can extract the P-Preffered-Id
     * header and resend the INVITE
     */

    if(sdf_fn_uaAddHeaderInPersistentHdrList(pCallObject,\
                pHdr,SipHdrTypePPreferredId, \
                "uat-Preffered",pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
                (Sdf_ty_s8bit*)"sdf_ivk_uaInsertPPreferredIdHdr:" \
                "Failed to insert 'PPrefferedId'header into "\
                "persistent header list", pErr);
#endif		
		sip_freeSipHeader(pHdr);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
						  &pHdr,pErr);
        return Sdf_co_fail;
    } 

    pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
            "sdf_ivk_uaInsertPPreferredIdHdr");
    return Sdf_co_success;
}

#endif
#ifdef SDF_RPORT


/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/

Sdf_ty_matchMessageType sdf_ivk_uaAddViaRportParam
	( SipMessage  	*pMsg,
	  Sdf_st_error *pErr)
{
	SipHeader dViaHeader;
	SipParam * pRportParam = Sdf_co_null;
	Sdf_ty_s8bit *pTmpValue = Sdf_co_null;


	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaAddViaRportParam");


#ifdef SDF_PARAMVALIDATION

	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaAddViaRportParam():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_en_fail;
	}


	if (pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddViaRportParam():  "
			"SipMessage passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}

#endif /* End param validations ifdef */






	if(sip_getHeaderAtIndex(pMsg, SipHdrTypeVia, \
		&dViaHeader, (Sdf_ty_u32bit)0, (SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaAddViaRportParam(): "
			"Failed to get topmost Via header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}



	if (sip_initSipParam(&pRportParam, (SipError *)\
		&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
		(Sdf_ty_s8bit *)"sdf_ivk_uaAddViaRportParam(): "
		"Failed to init a SipParam structure.",pErr);
#endif	
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}

	pTmpValue = Sdf_mc_strdupCallHandler("rport");

	if (sip_setNameInSipParam(pRportParam, pTmpValue, \
				(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
		(Sdf_ty_s8bit *)"sdf_ivk_uaAddViaRecdParam(): "
		"Failed to set name in a SipParam structure.",pErr);
#endif		
		sip_freeSipHeader(&dViaHeader);
		sip_freeSipParam(pRportParam);
		return Sdf_en_fail;
	}

	if (sip_insertViaParamAtIndexInViaHdr(&dViaHeader, pRportParam,\
		(Sdf_ty_u32bit)0, (SipError *)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = pErr->stkErrCode ;
#ifdef SDF_ERROR					
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaAddViaRecdParam(): "
			"Failed to insert value in a SipParam structure.", \
			pErr);
#endif			
		sip_freeSipHeader(&dViaHeader);
		sip_freeSipParam(pRportParam);
		return Sdf_en_fail;
	}

	sip_freeSipParam(pRportParam);



	/* Free the local reference */
	sip_freeSipHeader(&dViaHeader);

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaAddViaRportParam");



	return Sdf_en_success;
	
}






/*****************************************************************************
** FUNCTION: sdf_ivk_uaRemoveViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/


Sdf_ty_matchMessageType sdf_ivk_uaRemoveViaRportParam
	( SipMessage  	*pMsg,
	  Sdf_st_error *pErr)
{
	SipHeader dViaHeader;
/*	SipParam * pRportParam = Sdf_co_null;
	Sdf_ty_s8bit *pTmpValue = Sdf_co_null;		*/
	Sdf_ty_u32bit dViaParamCount,dIndex;
	Sdf_ty_s32bit dRportFoundFlag;
	SipParam *pViaParam;
	Sdf_ty_s8bit * pParamName;


	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Entering sdf_ivk_uaRemoveViaRportParam");



#ifdef SDF_PARAMVALIDATION

	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaRemoveViaRportParam():  Error Param Passed is "
			"invalid ");
#endif
		return Sdf_en_fail;
	}

	if (pMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaRemoveViaRportParam():  "
			"SipMessage passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_en_fail;
	}

#endif /* End param validations ifdef */




	if(sip_getHeaderAtIndex(pMsg, SipHdrTypeVia, \
		&dViaHeader, (Sdf_ty_u32bit)0, (SipError*)&(pErr->stkErrCode)) \
		== SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaRemoveViaRportParam(): "
			"Failed to get topmost Via header from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_en_fail;
	}


	if(sip_getViaParamCountFromViaHdr(&dViaHeader, &dViaParamCount, \
		(SipError *)&(pErr->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaRemoveViaRportParam(): "
			"Failed to get via param count from Via header of SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		sip_freeSipHeader(&dViaHeader);
		return Sdf_en_fail;
	}


	dRportFoundFlag = 0;

	for(dIndex=0; dIndex<dViaParamCount; dIndex++)
	{
		if(sip_getViaParamAtIndexFromViaHdr(&dViaHeader, &pViaParam, \
			dIndex, (SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaRemoveViaRportParam(): "
				"Failed to get via param from Via header of SIP message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}

		if(sip_getNameFromSipParam(pViaParam, &pParamName, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaRemoveViaRportParam(): "
				"Failed to get name from via param",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipParam(pViaParam);
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}
		if(strcmp(pParamName, "rport")==0)
		{
			dRportFoundFlag = 1;
			break;
		}
		sip_freeSipParam(pViaParam);
	}
	if(dRportFoundFlag)
	{
		sip_freeSipParam(pViaParam);
		if(sip_deleteViaParamAtIndexInViaHdr(&dViaHeader, dIndex, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaRemoveViaRportParam(): "
				"Failed to delete via param in Via header of SIP message",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			sip_freeSipParam(pViaParam);
			sip_freeSipHeader(&dViaHeader);
			return Sdf_en_fail;

		}
	}
	



	/* Free the local reference */
	sip_freeSipHeader(&dViaHeader);

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaRemoveViaRportParam");


	return Sdf_en_success;
	
}

#endif


