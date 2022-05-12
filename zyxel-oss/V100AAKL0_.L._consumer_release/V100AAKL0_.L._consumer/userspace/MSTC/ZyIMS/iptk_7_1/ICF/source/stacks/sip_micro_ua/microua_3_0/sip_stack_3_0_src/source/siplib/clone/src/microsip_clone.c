/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ** FUNCTION:
 **              Clone Functions
 **
 *********************************************************************
 **
 ** FILENAME:
 ** microsip_clone.c
 **
 ** DESCRIPTION:
 ** This file contains  clone functions. 
 **
 ** DATE        NAME                    REFERENCE       REASON
 ** ----        ----                    ---------       --------
 ** 15 02 2002  Mahesh Govind	           -		 Initial Creation
 **
 ** COPYRIGHT , Aricent, 2006
 *********************************************************************/ 

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_decode.h"
#include "microsip_cloneintrnl.h"
#include "microsip_clone.h"
#include "microsip_accessors.h"
#ifdef SIP_TEL
    /*
#include "microsip_telstruct.h"
#include "microsip_telinit.h"*/
#include "microsip_telfree.h"
#endif
#ifdef SIP_IMURL
    /*
#include "microsip_imurlstruct.h"
#include "microsip_imurlinit.h"*/
#include "microsip_imurlfree.h"
#endif
#include "microsip_header.h"
#include "microsip_subapi.h"


/*******************************************************************************
** FUNCTION:sip_cloneSipCommonHeader
** DESCRIPTION:makes a deep copy of the source SipCommonHeader to the destination 
** SipCommonHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCommonHeader 
**			pSource(IN)		- Source SipCommonHeader 
**			pErr(OUT)		- error value 
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipCommonHeader
	(SipCommonHeader *pDest, 
	 SipCommonHeader *pSource, 
	 SipError        *pErr)
{
	SIP_S8bit *pTempStr = SIP_NULL;
	SIPDEBUGFN("Entering function sip_cloneSipCommonHeader");

	if((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pDest->pStr1 != SIP_NULL)
		sip_freeString(pDest->pStr1);
	if (pDest->pStr2 != SIP_NULL)
		sip_freeString(pDest->pStr2);
    if (pDest->pAddrSpec != SIP_NULL)
        sip_freeSipAddrSpec(pDest->pAddrSpec);
	if(sip_listDeleteAll(&(pDest->slParam), pErr) == SipFail)
		return SipFail;
#ifdef SIP_INCREMENTAL_PARSING	
	if (pDest->pBuffer != SIP_NULL)
		sip_freeString(pDest->pBuffer);
#endif		

    /* Copying the value from Source */
	pDest->dIntVar1 = pSource->dIntVar1;
	pDest->dIntVar2 = pSource->dIntVar2;

	if (pSource->pStr1 == SIP_NULL)
		pTempStr = SIP_NULL;
	else
	{
		if ( (pTempStr = (SIP_S8bit *)STRDUPACCESSOR (pSource->pStr1)) == SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
	}
	pDest->pStr1 = pTempStr;

	if (pSource->pStr2 == SIP_NULL)
		pTempStr = SIP_NULL;
	else
	{
		if ( (pTempStr = (SIP_S8bit *)STRDUPACCESSOR (pSource->pStr2)) == SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
	}
	pDest->pStr2 = pTempStr;

    if(pSource->pAddrSpec != SIP_NULL)
    {
        /* duplicating pAddrSpec */
        if (validateSipAddrSpecType((pSource->pAddrSpec)->dType, pErr) == SipFail)
            return SipFail;
        if (sip_initSipAddrSpec(&(pDest->pAddrSpec), (pSource->pAddrSpec)->dType,
                    pErr) == SipFail)
            return SipFail;
        if (__sip_cloneSipAddrSpec(pDest->pAddrSpec, pSource->pAddrSpec, pErr) 
                == SipFail)
        {
            sip_freeSipAddrSpec(pDest->pAddrSpec);
            return SipFail;
        }
    }
	if(__sip_cloneSipParamList(&(pDest->slParam),&(pSource->slParam),pErr)
		==SipFail)
	{
		return SipFail;
	}	
#ifdef SIP_INCREMENTAL_PARSING	
	if (pSource->pBuffer == SIP_NULL)
		pDest->pBuffer = SIP_NULL;
	else
	{
		if ( (pDest->pBuffer = (SIP_S8bit *)STRDUPACCESSOR (pSource->pBuffer))
					== SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
	}
#endif

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipCommonHeader");
	return SipSuccess;
}

#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:sip_cloneSipCommonHeaderWithDate
** DESCRIPTION:makes a deep copy of the source SipCommonHeader to the destination 
** SipCommonHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipCommonHeader 
**			pSource(IN)		- Source SipCommonHeader 
**			pErr(OUT)		- error value 
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipCommonHeaderWithDate
	(SipCommonHeader *pDest, 
	 SipCommonHeader *pSource, 
	 SipError        *pErr)
{
	SIPDEBUGFN("Entering function sip_cloneSipCommonHeaderWithDate");

	if(pSource->dIntVar1 == SipExpAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if(pDest->dIntVar1 == SipExpDate)
	{
		if(pDest->u.pDate != SIP_NULL)
			sip_freeSipDateStruct(pDest->u.pDate);
	}
    if(SipFail == sip_cloneSipCommonHeader(pDest,pSource,pErr))
    {
        return SipFail;
    }
	if(pSource->dIntVar1 == SipExpDate)
	{
		if(pSource->u.pDate == SIP_NULL)
			pDest->u.pDate = SIP_NULL;
		else
		{
			if(sip_initSipDateStruct(&(pDest->u.pDate), pErr) == SipFail)
				return SipFail;
			if(__sip_cloneSipDateStruct(pDest->u.pDate,pSource->u.pDate,pErr)\
				==SipFail)
			{
				sip_freeSipDateStruct(pDest->u.pDate);
				return SipFail;
			}
		}
	}
	else if(pSource->dIntVar1 == SipExpSeconds)
	{
		pDest->u.dSec = pSource->u.dSec;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipCommonHeaderWithDate");
	return SipSuccess;
}
#endif

/*******************************************************************************
** FUNCTION: sip_cloneSipCommonHeaderList
** DESCRIPTION:makes a deep copy of the source SipList of Common header 
**             to the destination  SipList of Common Header List
** PARAMETERS:
**          pDest(OUT)      - Destination SipList
**          pSource(IN)     - Source SipList
**          pErr(OUT)       - error value 
**
** Return Values:
**          SipFail
**          SipSuccess
*******************************************************************************/


SipBool sip_cloneSipCommonHeaderList
    (SipList *pDest, SipList *pSource, SipError *pErr)
{
    SipCommonHeader *pTempCommon = SIP_NULL, *pCloneCommon = SIP_NULL;
    SIP_U32bit count, dIndex;

    SIPDEBUGFN("Entering function sip_cloneSipCommonHeaderList");
    if ( sip_listDeleteAll(pDest, pErr ) == SipFail )
        return SipFail;
    if ( sip_listSizeOf(pSource, &count, pErr) == SipFail )
    {
        return SipFail;
    }
    for ( dIndex = 0; dIndex < count ; dIndex++)
    {
        if( sip_listGetAt(pSource,dIndex, (void **) (&pTempCommon), pErr)\
            == SipFail )
        {
            return SipFail;
        }
        if ( pTempCommon == SIP_NULL )
            pCloneCommon = SIP_NULL;
        else
        {
			if(sip_initSipCommonHeader(&pCloneCommon, pErr)==SipFail)
			{
				return SipFail;
			}
			
            if ( pCloneCommon == SIP_NULL )
                return SipFail;

            if ( sip_cloneSipCommonHeader(pCloneCommon, pTempCommon,
                        pErr) == SipFail )
            {
                (void)sip_memfree(ACCESSOR_MEM_ID, (void**)&(pCloneCommon), \
                    SIP_NULL);
                return SipFail;
            }
        }

        if ( sip_listAppend(pDest, pCloneCommon, pErr) == SipFail )
        {
            if ( pCloneCommon != SIP_NULL )
                (void)sip_memfree(ACCESSOR_MEM_ID,(void**)&(pCloneCommon), \
                    SIP_NULL);
            return SipFail;
        }
    }

    *pErr = E_NO_ERROR;
    SIPDEBUGFN("Exiting function sip_cloneSipCommonHeaderList");
    return SipSuccess;
}


#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION:__sip_cloneSipContactParam
** DESCRIPTION:makes a deep copy of the source SipContactParam to the 
** destination SipContactParam 
** PARAMETERS:
**			pDest(OUT)		- Destination SipContactParam 
**			pSource(IN)		- Source SipContactParam
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipContactParam
#ifdef ANSI_PROTO
	(SipContactParam *dest, SipContactParam *source, SipError *err)
#else
	(dest, source, err)
	SipContactParam *dest;
	SipContactParam *source;
	SipError *err;
#endif
{
	SIP_S8bit *temp;
	SIPDEBUGFN("Entering function __sip_cloneSipContactParam");

 	if ( source == SIP_NULL || dest == SIP_NULL )
    {
 		*err = E_INV_PARAM;
 		return SipFail;
 	}	
	if (validateSipContactParamsType(source->dType, err) == SipFail)
		return SipFail;

	/* clear destination parameter */
	switch(dest->dType)
	{
		case SipCParamQvalue:
			if ((dest->u).pQValue != SIP_NULL)
				sip_freeString((dest->u).pQValue);
		 	break;
		case SipCParamExpires:
			if ((dest->u).pExpire != SIP_NULL)
				sip_freeSipExpiresStruct((dest->u).pExpire);
		 	break;
		case SipCParamExtension:
			if ((dest->u).pExtensionAttr != SIP_NULL)
				sip_freeString((dest->u).pExtensionAttr);
		 	break;
#ifdef SIP_3GPP                
		case SipCParamFeature:
			if ((dest->u).pFeatureAttr != SIP_NULL)
				sip_freeString((dest->u).pFeatureAttr);
		 	break;
#endif

		case SipCParamAny:
			break;
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	dest->dType = source->dType;
	switch (source->dType)
	{
		case SipCParamQvalue:
			if ((source->u).pQValue == SIP_NULL)
				temp = SIP_NULL;
			else
			{
				temp = (SIP_S8bit *)STRDUPACCESSOR((source->u).pQValue);
				if (temp == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
			}
			(dest->u).pQValue = temp;
			break;
		case SipCParamExpires:
#ifdef SIP_DATE
			if (sip_initSipExpiresStruct(&((dest->u).pExpire), \
				((source->u).pExpire)->dIntVar1, err) == SipFail)
#else
				if (sip_initSipExpiresStruct(&((dest->u).pExpire), \
					err) == SipFail)
#endif
				return SipFail;
			if (__sip_cloneSipExpiresStruct( (dest->u).pExpire, \
				(source->u).pExpire, err ) == SipFail)
			{
				sip_freeSipExpiresStruct((dest->u).pExpire);
				return SipFail;
			}
			break;
		case SipCParamExtension:
			if ((source->u).pExtensionAttr == SIP_NULL)
				temp = SIP_NULL;
			else
			{
				temp = (SIP_S8bit *)STRDUPACCESSOR((source->u).pExtensionAttr);
				if (temp == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
			}
			(dest->u).pExtensionAttr = temp;
			break;
#ifdef SIP_3GPP
		case SipCParamFeature:
			if ((source->u).pFeatureAttr == SIP_NULL)
				temp = SIP_NULL;
			else
			{
				temp = (SIP_S8bit*)STRDUPACCESSOR((source->u).pFeatureAttr);
				if (temp == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
			}
			(dest->u).pFeatureAttr = temp;
			break;
#endif



		case SipCParamAny:
			*err = E_INV_PARAM;
			break;
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipContactParam");
	return SipSuccess;
}
#endif

#ifdef SIP_DATE
/********************************************************************
**
** FUNCTION:  validateSipExpiresType
**
** DESCRIPTION:  This function validates the pValue of "dType"
**
********************************************************************/

SipBool validateSipExpiresType
#ifdef ANSI_PROTO
	(en_ExpiresType dType, SipError *err)
#else
	(dType, err)
	en_ExpiresType dType;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function validateSipExpiresType");
	switch (dType)
	{
		case	SipExpDate		:
		case	SipExpSeconds	:
		case	SipExpAny		: break;
		default					:*err = E_INV_TYPE;
					 			return SipFail;

	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function validateSipExpiresType");
	return SipSuccess;
}
#endif /* SIP_DATE */


/*******************************************************************************
** FUNCTION: __sip_cloneSipStringList
** DESCRIPTION:makes a deep copy of the source SipListof Stringt o the 
** destination SipList of String
** PARAMETERS:
**			pDest(OUT)		- Destination SipList (of strings)
**			pSource(IN)		- Source SipList (of strings) 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipStringList
#ifdef ANSI_PROTO
	(SipList *dest, SipList *source, SipError *err)
#else
	(dest, source, err)
	SipList *dest;
	SipList *source;
	SipError *err;
#endif
{
	SIP_S8bit *temp_string, *clone_string;
	SIP_Pvoid temp;
	SIP_U32bit count, index_1;

	SIPDEBUGFN("Entering function __sip_cloneStringList");
	if (sip_listDeleteAll(dest, err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf (source, &count, err) == SipFail )
			return SipFail;

	for (index_1 = 0; index_1 < count; index_1 ++)
	{
		if (sip_listGetAt(source, index_1, &temp, err) == SipFail)
			return SipFail;
		temp_string = (SIP_S8bit *)temp;
		if (temp_string == SIP_NULL)
			clone_string = SIP_NULL;
		else
		{
			clone_string = (SIP_S8bit *)STRDUPACCESSOR(temp_string);
			if (clone_string == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
		}/* end of else */
		if (sip_listAppend(dest, (SIP_Pvoid)clone_string, err) == SipFail)
		{
			sip_freeString(clone_string);
			return SipFail;
		}
	} /* end of for */

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneStringList");
	return SipSuccess;
}


#ifdef SIP_CORE_STACK_COMPATIBLE

/*******************************************************************************
** FUNCTION:__sip_cloneCredential
** DESCRIPTION:makes a deep copy of the source SipGenericCredential to the 
** destination  SipGenericCredential 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGenericCredential 
**			pSource(IN)		- Source SipGenericCredential 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool __sip_cloneCredential
#ifdef ANSI_PROTO
	(SipGenericCredential *to,
	SipGenericCredential *from,
	SipError *err)
#else
	( to,from, err )
	SipGenericCredential *to;
	SipGenericCredential *from;
	SipError *err;
#endif
{
	SIP_U16bit dLength;

	SIPDEBUGFN( "Entering __sip_cloneCredential");

	if((from==SIP_NULL)||(to==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	/* Cleaning up to */
	if(to->dType == SipCredBasic)
	{
		if(to->u.pBasic!=SIP_NULL)
		{
			if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid *)&(to->u.pBasic),err) ==\
				SipFail)
				return SipFail;
		}
	}
	else if(to->dType == SipCredAuth)
	{
		if(to->u.pChallenge != SIP_NULL)
			sip_freeSipGenericChallenge(to->u.pChallenge);
	}
	/* Cleaned up to */
	to->dType=from->dType;
	if(from->dType==SipCredBasic)
	{
		if(from->u.pBasic== SIP_NULL)
			to->u.pBasic=SIP_NULL;
		else
		{
			dLength = sip_strlen(from->u.pBasic );
			to->u.pBasic=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,\
				err);
			if ( to->u.pBasic == SIP_NULL )
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			(void)sip_strcpy( to->u.pBasic, from->u.pBasic );
		}
	}
	else if(from->dType==SipCredAuth)
		{
			if(from->u.pChallenge == SIP_NULL)
				to->u.pChallenge = SIP_NULL;
			else
			{
				if(sip_initSipGenericChallenge(&(to->u.pChallenge),\
					err)==SipFail)
					return SipFail;
				if(__sip_cloneChallenge(to->u.pChallenge,from->u.pChallenge,\
					err) ==SipFail)
				{
					sip_freeSipGenericChallenge(to->u.pChallenge);
					return SipFail;
				}
			}
		}
	else
	{
		*err = E_INV_TYPE;
		return SipFail;
	}
	SIPDEBUGFN( "Exiting __sip_cloneCredential");

	*err=E_NO_ERROR;
	return SipSuccess;

}
#endif
#endif

/*******************************************************************************
** FUNCTION:__sip_cloneSipChallenge
** DESCRIPTION:makes a deep copy of the source SipGenericChallenge to the 
** destination SipGenericChallenge 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGenericChallenge 
**			pSource(IN)		- Source SipGenericChallenge 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_AUTHENTICATE
_STATIC_ SipBool __sip_cloneSipChallenge
#ifdef ANSI_PROTO
	(SipGenericChallenge * dest, SipGenericChallenge *src,SipError * err)
#else
	( dest,src,err )
	  SipGenericChallenge 	*dest;
	  SipGenericChallenge 	*src;
	  SipError 		*err;
#endif
{
	SIP_U32bit dLength;

	SIPDEBUGFN ( "Entering __sip_cloneSipChallenge");

	/* freeing the fields of the dest structure */
	if ( dest->pScheme != SIP_NULL )
	{
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pScheme)), err)\
			== SipFail )
			return SipFail;
	}

	if ( src->pScheme == SIP_NULL)
		dest->pScheme = src->pScheme;
	else
	{
		dLength = sip_strlen(src->pScheme);
		dest->pScheme = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1,\
			err);
		if ( dest->pScheme == SIP_NULL )
			return SipFail;
		(void)sip_strcpy(dest->pScheme,src->pScheme);
	}

	/* freeing siplist of auth pParam */
	if ( sip_listDeleteAll(&( dest->slParam ), err ) == SipFail )
		return SipFail;
	if(__sip_cloneSipParamList(&(dest->slParam), &(src->slParam), err)==SipFail)
		return SipFail;

	SIPDEBUGFN ( "Exiting __sip_cloneSipChallenge");
	*err = E_NO_ERROR;
	return SipSuccess;
}
#endif
#endif


#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION: __sip_cloneDateFormat
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the destination 
** SipDateFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneDateFormat
#ifdef ANSI_PROTO
	(SipDateFormat *dest, SipDateFormat *src,SipError *err)
#else
	(dest,src,err )
	  SipDateFormat *dest;
	  SipDateFormat *src;
	  SipError *err;
#endif
{
	SipError *dummy;
	dummy=err;
    (void)dummy;
	SIPDEBUGFN( "Entering __sip_cloneDateFormat");
	dest->dDay = src->dDay;
	dest->dMonth = src->dMonth;
	dest->dYear = src->dYear;
	SIPDEBUGFN( "Exiting __sip_cloneDateFormat");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneTimeFormat
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination 
** SiptimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SiptimeFormat 
**			pSource(IN)		- Source SipTimeFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneTimeFormat
#ifdef ANSI_PROTO
	(SipTimeFormat * dest, SipTimeFormat *src,SipError *err)
#else
	( dest,src,err )
	  SipTimeFormat *dest;
	  SipTimeFormat *src;
	  SipError *err;
#endif
{
	SipError *dummy;
	dummy=err;
    (void)dummy;
	SIPDEBUGFN( "Entering __sip_cloneTimeFormat");
	dest->dHour = src->dHour;
	dest->dMin = src->dMin;
	dest->dSec = src->dSec;
	SIPDEBUGFN( "Exiting __sip_cloneTimeFormat");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneDateStruct
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination 
** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct
**			pSource(IN)		- Source SipDateStruct
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneDateStruct
#ifdef ANSI_PROTO
	(SipDateStruct * dest, SipDateStruct *src,SipError *err)
#else
	( dest,src,err )
	  SipDateStruct *dest;
	  SipDateStruct *src;
	  SipError *err;
#endif
{
	SIPDEBUGFN( "Entering __sip_cloneDateStruct");
	dest->dDow = src->dDow;
	(void)__sip_cloneDateFormat( dest->pDate,src->pDate, err );
	(void)__sip_cloneTimeFormat( dest->pTime,src->pTime, err );
	SIPDEBUGFN( "Exiting __sip_cloneDateStruct");
	return SipSuccess;
}
#endif

/*******************************************************************************
** FUNCTION:__sip_cloneSdpTime
** DESCRIPTION:makes a deep copy of the source SdpTime to the destination 
** SdpTime 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpTime 
**			pSource(IN)		- Source SdpTime 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSdpTime
#ifdef ANSI_PROTO
	( SdpTime 	*dest,
	  SdpTime 	*src,
	  SipError 		*err)
#else
	( dest, src, err)
	  SdpTime 	*dest;
	  SdpTime 	*src;
	  SipError 		*err;
#endif
{
	SIP_U32bit dLength,size,index_1;
	SIP_S8bit * element_in_list, * dup_element_in_list;
	SipError temp_err;
	SIP_U8bit mem_flag = 0;


	SIPDEBUGFN ( "Entering __sip_cloneTime");
	do
	{


	if ( dest->pStart != SIP_NULL )
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pStart)), \
			err ) == SipFail )
			return SipFail;
	if ( src->pStart == SIP_NULL )
		dest->pStart = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pStart);

		dest->pStart = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1,\
			err);
		if ( dest->pStart == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pStart, src->pStart);
	}

	if ( dest->pStop != SIP_NULL )
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pStop)), err ) \
			== SipFail )
			return SipFail;
	if ( src->pStop == SIP_NULL )
		dest->pStop = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pStop);

		dest->pStop = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, \
			err);
		if ( dest->pStop == SIP_NULL )
			{mem_flag = 1;break;}

		(void)sip_strcpy(dest->pStop, src->pStop);
	}
	if ( dest->pZone != SIP_NULL )
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pZone)), err ) \
			== SipFail )
			return SipFail;
	if ( src->pZone == SIP_NULL )
		dest->pZone = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pZone);
		dest->pZone = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, \
				err);
		if ( dest->pZone == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pZone, src->pZone);
	}
	/* cloning slRepeat here */
	if( sip_listDeleteAll(&(dest->slRepeat), err) == SipFail)
		return SipFail;
	if( sip_listSizeOf(&(src->slRepeat),&size, err) == SipFail)
		return SipFail;
	for ( index_1 = 0; index_1 < size ; index_1++ )
	{
		if ( sip_listGetAt(&(src->slRepeat), index_1,\
			(SIP_Pvoid *)(&element_in_list), err) == SipFail)
			return SipFail;
		if ( element_in_list == SIP_NULL)
			dup_element_in_list = SIP_NULL;
		else
		{
			dLength = sip_strlen(element_in_list);
			dup_element_in_list = ( SIP_S8bit * ) fast_memget(ACCESSOR_MEM_ID,\
				dLength+1,err);
			if (dup_element_in_list == SIP_NULL )
				{mem_flag = 1;break;}
			(void)sip_strcpy(dup_element_in_list, element_in_list);
		}
		if( sip_listAppend( &(dest->slRepeat),(SIP_Pvoid)dup_element_in_list,\
			err) == SipFail)
		{
			if ( dup_element_in_list != SIP_NULL)
				(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&dup_element_in_list)\
					, &temp_err);
			return SipFail;
		}
	}
	if( mem_flag ) break;
	} while(0);
	if( mem_flag )
	{
		sip_freeSdpTime( dest );
        	return SipFail;
	}


	SIPDEBUGFN ( "Exiting __sip_cloneTime");
	*err = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSdpMedia
** DESCRIPTION:makes a deep copy of the source SdpMedia to the destination 
** SdpMedia
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMedia 
**			pSource(IN)		- Source SdpMedia 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSdpMedia
#ifdef ANSI_PROTO
	( SdpMedia 	*dest,
	  SdpMedia 	*src,
	  SipError 	*err)
#else
	( dest, src, err)
	  SdpMedia 	*dest;
	  SdpMedia 	*src;
	  SipError 	*err;
#endif
{
	SIP_U32bit dLength,size,index_1;
	SdpConnection *element_from_src, *clone_connection;
	SdpAttr *element_from_src_attr, *clone_attr;

	SIP_U8bit mem_flag = 0;

	SIPDEBUGFN ( "Entering __sip_cloneMedia");
	do
	{


	/* cloning pInformation */
	if ( dest->pInformation != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid *)(&(dest->pInformation)),\
			err) == SipFail)
			return SipFail;

	if ( src->pInformation == SIP_NULL )
		dest->pInformation = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pInformation);
		dest->pInformation = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,\
				dLength + 1, err);
		if ( dest->pInformation == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pInformation, src->pInformation);
	}

	/* cloning pBandwidth */
	if( sip_listDeleteAll(&(dest->slBandwidth),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->slBandwidth), &(src->slBandwidth),\
		err) == SipFail)
		return SipFail;

	/* cloning pKey */
	if ( dest->pKey != SIP_NULL)
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pKey)), err ) \
			== SipFail)
			return SipFail;
	if ( src->pKey == SIP_NULL )
		dest->pKey = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pKey);
		dest->pKey = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, \
			err);
		if ( dest->pKey == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pKey, src->pKey);
	}
	/* cloning pFormat */
	if ( dest->pFormat != SIP_NULL)
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pFormat)),\
			err ) == SipFail)
			return SipFail;
	if ( src->pFormat == SIP_NULL )
		dest->pFormat = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pFormat);
		dest->pFormat = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1,\
			err);
		if ( dest->pFormat == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pFormat, src->pFormat);
	}
	/* cloning pProtocol */
	if ( dest->pProtocol != SIP_NULL)
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pProtocol)), \
			err ) == SipFail)
			return SipFail;
	if ( src->pProtocol == SIP_NULL )
		dest->pProtocol = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pProtocol);
		dest->pProtocol = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pProtocol == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pProtocol, src->pProtocol);
	}
	/* cloning pMediaValue */
	if ( dest->pMediaValue != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid *)(&(dest->pMediaValue)),\
			err) == SipFail)
			return SipFail;

	if ( src->pMediaValue == SIP_NULL )
		dest->pMediaValue = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pMediaValue);

		dest->pMediaValue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength \
			+ 1, err);
		if ( dest->pMediaValue == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pMediaValue, src->pMediaValue);
	}
	/* cloning dPort */
	dest->dPort = src->dPort;
	/* cloning pPortNum */
	if ( dest->pPortNum != SIP_NULL)
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pPortNum)), \
			err ) == SipFail)
			return SipFail;
	if ( src->pPortNum == SIP_NULL )
		dest->pPortNum = SIP_NULL;
	else
	{
		dest->pPortNum = ( SIP_U32bit * )fast_memget(ACCESSOR_MEM_ID,sizeof\
			(SIP_U32bit), err);
		if ( dest->pPortNum == SIP_NULL )
			{mem_flag = 1;break;}
		*(dest->pPortNum) = *(src->pPortNum);
	}
	/* cloning slConnection */

	if ( sip_listDeleteAll(&(dest->slConnection), err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf(&(src->slConnection), &size, err) == SipFail)
		return SipFail;
	for ( index_1 = 0; index_1 < size ; index_1++ )
	{
		if( sip_listGetAt(&(src->slConnection),index_1, (SIP_Pvoid * )\
			(&element_from_src), err) == SipFail )
		{
			return SipFail;
		}
		if (element_from_src == SIP_NULL)
			clone_connection = SIP_NULL;
		else
		{
			if ( sip_initSdpConnection(&clone_connection, err) == SipFail)
				return SipFail;
			if (__sip_cloneSdpConnection ( clone_connection, element_from_src,\
				err) == SipFail)
			{
				sip_freeSdpConnection(clone_connection);
				return SipFail;
			}
		}
		if (sip_listAppend (&(dest->slConnection), (SIP_Pvoid *)\
			clone_connection, err) == SipFail)
		{
			if (clone_connection != SIP_NULL)
				sip_freeSdpConnection (clone_connection);
			return SipFail;
		}

	}

	if ( sip_listDeleteAll(&(dest->slAttr), err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf(&(src->slAttr), &size, err) == SipFail)
		return SipFail;
	for ( index_1 = 0; index_1 < size ; index_1++ )
	{
		if( sip_listGetAt(&(src->slAttr),index_1, (SIP_Pvoid * )\
			(&element_from_src_attr), err) == SipFail )
		{
			return SipFail;
		}
		if (element_from_src_attr == SIP_NULL)
			clone_attr = SIP_NULL;
		else
		{
			if ( sip_initSdpAttr (&clone_attr, err) == SipFail)
				return SipFail;
			if (__sip_cloneSdpAttr (clone_attr, element_from_src_attr, err) \
				== SipFail)
			{
				sip_freeSdpAttr(clone_attr);
				return SipFail;
			}
		}
		if (sip_listAppend (&(dest->slAttr), (SIP_Pvoid *)clone_attr, err) == \
			SipFail)
		{
			if (clone_attr != SIP_NULL)
				sip_freeSdpAttr (clone_attr);
			return SipFail;
		}

	}
	if( sip_listDeleteAll(&(dest->slIncorrectLines),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->slIncorrectLines), \
		&(src->slIncorrectLines), err) == SipFail)
		return SipFail;

	} while(0);
	if( mem_flag )
	{
		sip_freeSdpMedia (dest);
        	return SipFail;
	}


	SIPDEBUGFN ( "Exiting __sip_cloneMedia");
	*err = E_NO_ERROR;
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION:__sip_cloneSdpConnection
** DESCRIPTION:makes a deep copy of the source SdpConnection to the destination 
** SdpConnection 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpConnection 
**			pSource(IN)		- Source SdpConnection 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
 SipBool __sip_cloneSdpConnection
#ifdef ANSI_PROTO
	( SdpConnection 	*dest,
	  SdpConnection 	*src,
	  SipError 		*err)
#else
	( dest, src, err)
	  SdpConnection 	*dest;
	  SdpConnection 	*src;
	  SipError 		*err;
#endif
{
	SIP_U32bit dLength;
	SIP_U8bit mem_flag = 0;


	SIPDEBUGFN ( "Entering __sip_cloneSdpConnection");
	do
	{

	/* freeing fileds of destination */
	if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pNetType)), err )\
		== SipFail )
		return SipFail;

	if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pAddrType)), err ) \
		== SipFail )
		return SipFail;

	if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pAddr)), err ) == \
		SipFail )
		return SipFail;

	if ( src->pNetType == SIP_NULL )
		dest->pNetType = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pNetType);
		dest->pNetType = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pNetType == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pNetType, src->pNetType);
	}

	if ( src->pAddrType == SIP_NULL )
		dest->pAddrType = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pAddrType);

		dest->pAddrType = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pAddrType == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pAddrType, src->pAddrType);
	}

	if ( src->pAddr == SIP_NULL )
		dest->pAddr = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pAddr);
		dest->pAddr = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, \
			err);
		if ( dest->pAddr == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pAddr, src->pAddr);
	}
	} while(0);
	if( mem_flag )
	{
		sip_freeSdpConnection(dest);
        	return SipFail;
	}

	SIPDEBUGFN ( "Exiting __sip_cloneSdpConnection");

	*err = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSdpAttr
** DESCRIPTION:makes a deep copy of the source SdpAttr to the destination 
** SdpAttr
** PARAMETERS:
**			pDest(OUT)		- Destination SdpAttr 
**			pSource(IN)		- Source SdpAttr
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSdpAttr
#ifdef ANSI_PROTO
	( SdpAttr 	*dest,
	  SdpAttr 	*src,
	  SipError 	*err)
#else
	( dest, src, err)
	  SdpAttr 	*dest;
	  SdpAttr 	*src;
	  SipError 	*err;
#endif
{
	SIP_U32bit dLength;
	SIP_U8bit mem_flag = 0;

	SIPDEBUGFN ( "Entering __sip_cloneSdpAttr");
	do
	{

		/* freeing the fieds of destination */
		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pName)), err ) \
			== SipFail)
			return SipFail;

		if ( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(dest->pValue)), err )\
			== SipFail)
			return SipFail;

		if ( src->pName == SIP_NULL )
			dest->pName = SIP_NULL;
		else
		{
			dLength = sip_strlen(src->pName);
			dest->pName = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,\
			err);
			if ( dest->pName == SIP_NULL )
				{mem_flag = 1;break;}
			(void)sip_strcpy(dest->pName, src->pName);
		}
		if ( src->pValue == SIP_NULL )
			dest->pValue = SIP_NULL;
		else
		{
			dLength = sip_strlen(src->pValue);
			dest->pValue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1\
					,err);
			if ( dest->pValue == SIP_NULL )
				{mem_flag = 1;break;}
			(void)sip_strcpy(dest->pValue, src->pValue);
		}
	} while(0);
	if( mem_flag )
	{
		sip_freeSdpAttr(dest);
        	return SipFail;
	}

	SIPDEBUGFN ( "Exiting __sip_cloneSdpAttr");

	*err = E_NO_ERROR;
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:__sip_cloneSdpMessage
** DESCRIPTION:makes a deep copy of the source SdpMessage to the destination 
** SdpMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMessage 
**			pSource(IN)		- Source SdpMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSdpMessage
#ifdef ANSI_PROTO
	(SdpMessage * dest, SdpMessage *src, SipError *err)
#else
	( dest,src,err )
		SdpMessage *dest;
		SdpMessage *src;
		SipError *err;

#endif
{
	SIP_U32bit i,dLength,count;
	SdpTime *timeout, *clone_timeout;
	SdpAttr *attrout, *clone_attrout;
	SdpMedia *mediaout, *clone_mediaout;
	SIP_U8bit mem_flag = 0;

	SIPDEBUGFN("Entering __sip_cloneSdpMessage\n");
	do
	{
	if((src == SIP_NULL)||(dest==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	/* Copying pVersion */
	if (dest->pVersion !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pVersion), \
			err)==SipFail)
			return SipFail;
	if ( src->pVersion == SIP_NULL )
		dest->pVersion = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pVersion);
		dest->pVersion = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pVersion == SIP_NULL )
		{
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pVersion, src->pVersion);
	}
	/* Copied pVersion */
	/* Copying pSession */
	if (dest->pSession !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pSession), \
			err)==SipFail)
			return SipFail;
	if ( src->pSession == SIP_NULL )
		dest->pSession = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pSession);
		dest->pSession = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pSession == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pSession, src->pSession);
	}
	/* Copied pSession */
	/* Copying pInformation */
	if (dest->pInformation !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pInformation), \
			err)==SipFail)
			return SipFail;
	if ( src->pInformation == SIP_NULL )
		dest->pInformation = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pInformation);
		dest->pInformation = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,\
			dLength + 1, err);
		if ( dest->pInformation == SIP_NULL )
		{
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pInformation, src->pInformation);
	}
	/* Copied pInformation */
	/* Copying pUri */
	if (dest->pUri !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)&(dest->pUri), err)\
			==SipFail)
			return SipFail;
	if ( src->pUri == SIP_NULL )
		dest->pUri = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pUri);
		dest->pUri = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, \
			err);
		if ( dest->pUri == SIP_NULL )
		{
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pUri, src->pUri);
	}
	/* Copied pUri */
	/* Copying pKey */
	if (dest->pKey !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)&(dest->pKey), err)\
			==SipFail)
			return SipFail;
	if ( src->pKey == SIP_NULL )
		dest->pKey = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pKey);
		if ( dest->pKey != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pKey, err ) == SipFail )
				return SipFail;
		}

		dest->pKey = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1,\
			err);
		if ( dest->pKey == SIP_NULL )
			{mem_flag = 1;break;}
		(void)sip_strcpy(dest->pKey, src->pKey);
	}
	/* Copied pKey */

	/* Copy pOrigin */
	if (dest->pOrigin !=SIP_NULL)
		sip_freeSdpOrigin(dest->pOrigin);
	if(src->pOrigin==SIP_NULL)
		dest->pOrigin = SIP_NULL;
	else
	{
		if(sip_initSdpOrigin(&(dest->pOrigin),err)==SipFail)
			return SipFail;
		if(__sip_cloneSdpOrigin(dest->pOrigin,src->pOrigin,err)==SipFail)
		{
			sip_freeSdpOrigin(dest->pOrigin);
			return SipFail;
		}
	}

	/* Copy slConnection */
	if (dest->slConnection !=SIP_NULL)
		sip_freeSdpConnection(dest->slConnection);
	if(src->slConnection==SIP_NULL)
	{
		dest->slConnection = SIP_NULL;
	}
	else
	{
		if(sip_initSdpConnection(&(dest->slConnection),err)==SipFail)
			return SipFail;
		if(__sip_cloneSdpConnection(dest->slConnection,src->slConnection,err)\
			==SipFail)
			return SipFail;
	}
	/* Copy all siplists */
	/* Copy slEmail list */
	if( sip_listDeleteAll(&(dest->slEmail),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->slEmail), &(src->slEmail), err) \
		== SipFail)
		return SipFail;


	/* Copy slPhone list */

	if( sip_listDeleteAll(&(dest->slPhone),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->slPhone), &(src->slPhone), err) \
		== SipFail)
		return SipFail;

	/* Copy BandWidth list */

	if( sip_listDeleteAll(&(dest->pBandwidth),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->pBandwidth), &(src->pBandwidth),\
		err) == SipFail)
		return SipFail;

	/* Copy Time list */

	if ( sip_listDeleteAll(&(dest->slTime), err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf(&(src->slTime), &count, err) == SipFail)
		return SipFail;
	for ( i = 0; i < count ; i++ )
	{
		if( sip_listGetAt(&(src->slTime),i, (SIP_Pvoid *) (&timeout), err) == \
			SipFail )
		{
			return SipFail;
		}
		if (timeout == SIP_NULL)
			clone_timeout = SIP_NULL;
		else
		{
			if ( sip_initSdpTime (&clone_timeout, err) == SipFail)
				return SipFail;
			if (__sip_cloneSdpTime (clone_timeout, timeout, err) == SipFail)
			{
				sip_freeSdpTime (clone_timeout);
				return SipFail;
			}
		}
		if (sip_listAppend (&(dest->slTime), (SIP_Pvoid *)clone_timeout, err) \
			== SipFail)
		{
			if (clone_timeout != SIP_NULL)
				sip_freeSdpTime (clone_timeout);
			return SipFail;
		}

	}
	if ( sip_listDeleteAll(&(dest->slAttr), err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf(&(src->slAttr), &count, err) == SipFail)
		return SipFail;
	for ( i = 0; i < count ; i++ )
	{
		if( sip_listGetAt(&(src->slAttr),i, (SIP_Pvoid *) (&attrout), err) \
			== SipFail )
		{
			return SipFail;
		}
		if (attrout == SIP_NULL)
			clone_attrout = SIP_NULL;
		else
		{
			if ( sip_initSdpAttr (&clone_attrout, err) == SipFail)
				return SipFail;
			if (__sip_cloneSdpAttr (clone_attrout, attrout, err) == SipFail)
			{
				sip_freeSdpAttr (clone_attrout);
				return SipFail;
			}
		}
		if (sip_listAppend (&(dest->slAttr), (SIP_Pvoid *)clone_attrout, err) \
			== SipFail)
		{
			if (clone_attrout != SIP_NULL)
				sip_freeSdpAttr (clone_attrout);
			return SipFail;
		}

	}
	/* Copy Media list */
	if ( sip_listDeleteAll(&(dest->slMedia), err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf(&(src->slMedia), &count, err) == SipFail)
		return SipFail;
	for ( i = 0; i < count ; i++ )
	{
		if( sip_listGetAt(&(src->slMedia),i, (SIP_Pvoid *) (&mediaout), err) \
			== SipFail )
		{
			return SipFail;
		}
		if (mediaout == SIP_NULL)
			clone_mediaout = SIP_NULL;
		else
		{
			if ( sip_initSdpMedia (&clone_mediaout, err) == SipFail)
				return SipFail;
			if (__sip_cloneSdpMedia (clone_mediaout, mediaout, err) == SipFail)
			{
				sip_freeSdpMedia (clone_mediaout);
				return SipFail;
			}
		}
		if (sip_listAppend (&(dest->slMedia), (SIP_Pvoid *)clone_mediaout,\
			err) == SipFail)
		{
			if (clone_mediaout != SIP_NULL)
				sip_freeSdpMedia (clone_mediaout);
			return SipFail;
		}

	}
	if( sip_listDeleteAll(&(dest->slIncorrectLines),err) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (&(dest->slIncorrectLines), \
		&(src->slIncorrectLines), err) == SipFail)
		return SipFail;

	} while(0);
	if( mem_flag )
	{
		sip_freeSdpMessage(dest);
        	return SipFail;
	}

	SIPDEBUGFN("Leaving __sip_cloneSdpMessage. Succeeded.\n");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION:__sip_cloneSdpOrigin
** DESCRIPTION:makes a deep copy of the source SdpOrigin to the destination 
** SdpOrigin
** PARAMETERS:
**			pDest(OUT)		- Destination SdpOrigin 
**			pSource(IN)		- Source SdpOrigin
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSdpOrigin
#ifdef ANSI_PROTO
	(SdpOrigin * dest, SdpOrigin *src, SipError *err)
#else
	( dest,src, err )
	  	SdpOrigin *dest;
		SdpOrigin *src;
		SipError *err;
#endif
{
	SIP_U32bit dLength;
	SIP_U8bit mem_flag = 0;

	SIPDEBUGFN("Entering __sip_cloneSdpOrigin\n");
	do
	{

	if((dest==SIP_NULL)||(src == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (dest->pUser !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)&(dest->pUser), err)\
			==SipFail)
			return SipFail;
	if ( src->pUser == SIP_NULL )
		dest->pUser = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pUser);
		if ( dest->pUser != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pUser, err ) == SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}

		dest->pUser = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1,\
			err);
		if ( dest->pUser == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pUser, src->pUser);
	}

	if (dest->pSessionid != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)&(dest->pSessionid), err)\
			==SipFail)
			return SipFail;
	if ( src->pSessionid == SIP_NULL )
		dest->pSessionid = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pSessionid);
		if ( dest->pSessionid != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pSessionid, err ) == \
				SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}

		dest->pSessionid = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength +\
			1, err);
		if ( dest->pSessionid == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pSessionid, src->pSessionid);
	}

	if (dest->pVersion !=SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pVersion), err)\
			==SipFail)
			return SipFail;
	if ( src->pVersion == SIP_NULL )
		dest->pVersion = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pVersion);
		if ( dest->pVersion != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pVersion, err )\
				== SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}

		dest->pVersion = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength \
			+ 1, err);
		if ( dest->pVersion == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pVersion, src->pVersion);
	}

	if (dest->pNetType != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pNetType), \
			err)==SipFail)
			return SipFail;
	if ( src->pNetType == SIP_NULL )
		dest->pNetType = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pNetType);
		if ( dest->pNetType != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pNetType, err ) == SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}
		dest->pNetType = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + \
			1, err);
		if ( dest->pNetType == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pNetType, src->pNetType);
	}
	if (dest->pAddrType != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pAddrType), \
			err)==SipFail)
			return SipFail;
	if ( src->pAddrType == SIP_NULL )
		dest->pAddrType = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pAddrType);
		if ( dest->pAddrType != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pAddrType, err ) == \
				SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}

		dest->pAddrType = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,\
			dLength + 1, err);
		if ( dest->pAddrType == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pAddrType, src->pAddrType);
	}

	if (dest->pAddr != SIP_NULL)
		if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pAddr), err)==\
			SipFail)
			return SipFail;
	if ( src->pAddr == SIP_NULL )
		dest->pAddr = SIP_NULL;
	else
	{
		dLength = sip_strlen(src->pAddr);
		if ( dest->pAddr != SIP_NULL )
		{
			if ( fast_memfree(ACCESSOR_MEM_ID, dest->pAddr, err ) == SipFail )
			{
				SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
				return SipFail;
			}
		}
		dest->pAddr = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1,\
			err);
		if ( dest->pAddr == SIP_NULL )
		{
			SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Failed.\n");
			{mem_flag = 1;break;}
		}
		(void)sip_strcpy(dest->pAddr, src->pAddr);
	}
	} while(0);
	if( mem_flag )
	{
		sip_freeSdpOrigin(dest);
        	return SipFail;
	}


	SIPDEBUGFN("Leaving __sip_cloneSdpOrigin. Succeeded.\n");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipUnknownMessage
** DESCRIPTION:makes a deep copy of the source SipUnknownMessage to the 
** destination  SipUnknownMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnknownMessage 
**			pSource(IN)		- Source SipUnknownMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipUnknownMessage
#ifdef ANSI_PROTO
(SipUnknownMessage *dest, SipUnknownMessage *src, SipError *err)
#else
	(dest, src, err)
	SipUnknownMessage *dest;
	SipUnknownMessage *src;
	SipError *err;
#endif
{
	/* No validation to be done, since function is internal */
	/* free up destination structure */
	if(dest->pBuffer != SIP_NULL)
		sip_freeString(dest->pBuffer);
	dest->dLength = src->dLength;
	dest->pBuffer = (SIP_S8bit *)\
		fast_memget(ACCESSOR_MEM_ID,src->dLength,err);
	if(dest->pBuffer == SIP_NULL)
		return SipFail;
	memcpy(dest->pBuffer,src->pBuffer,src->dLength);

	*err = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipMsgBody
** DESCRIPTION:makes a deep copy of the source SipMsgBody to the destination 
** SipMsgBody 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMsgBody 
**			pSource(IN)		- Source SipMsgBody 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipMsgBody
#ifdef ANSI_PROTO
(SipMsgBody *dest, SipMsgBody *src, SipError *err)
#else
	(dest, src, err)
	SipMsgBody *dest;
	SipMsgBody *src;
	SipError *err;
#endif
{
	/* No validation to be done, since function is internal */
	/* free up destination structure */
#ifdef SIP_MIME_PARSING	
	if (dest->pMimeHeader != SIP_NULL)
		sip_bcpt_freeSipMimeHeader(dest->pMimeHeader);
#endif		
	switch(dest->dType)
	{
		case SipSdpBody:
			sip_freeSdpMessage(dest->u.pSdpMessage);
			break;
		case SipUnknownBody:
			sip_freeSipUnknownMessage(dest->u.pUnknownMessage);
			break;
#ifdef SIP_ISUP			
		case SipIsupBody:  /* bcpt ext */
			sip_bcpt_freeIsupMessage(dest->u.pIsupMessage);
			break;
#endif			
#ifdef SIP_MIME_PARSING			
		case SipMultipartMimeBody:/* bcpt ext */
			sip_bcpt_freeMimeMessage(dest->u.pMimeMessage);
			break;
#endif			
#ifdef SIP_MWI
        case SipMessageSummaryBody:
			sip_mwi_freeMesgSummaryMessage(dest->u.pSummaryMessage);
            break;
#endif
#ifdef SIP_SIPMSGBODY
		case SipAppSipBody:
			sip_freeSipMessage(dest->u.pAppSipMessage);
			break;
#endif			
		case SipBodyAny:
			break;
		default:
			*err = E_INV_PARAM;
			return SipFail;
	}
	/* set destination's dType to source dType */
	dest->dType = src->dType;
	/* copy src to destination */
	switch(src->dType)
	{
		case SipSdpBody:
			if(sip_initSdpMessage(&(dest->u.pSdpMessage),err)==SipFail)
				return SipFail;
			if(__sip_cloneSdpMessage(dest->u.pSdpMessage, src->u.pSdpMessage,\
				err)==SipFail)
			{
				sip_freeSdpMessage(dest->u.pSdpMessage);
				return SipFail;
			}
			break;
		case SipUnknownBody:
			if(sip_initSipUnknownMessage(&(dest->u.pUnknownMessage),err)\
				==SipFail)
				return SipFail;
			if(__sip_cloneSipUnknownMessage(dest->u.pUnknownMessage,\
				src->u.pUnknownMessage, err)==SipFail)
			{
				sip_freeSipUnknownMessage (dest->u.pUnknownMessage);
				return SipFail;
			}
			break;
#ifdef SIP_ISUP			
		case SipIsupBody:
			if(sip_initIsupMessage(&(dest->u.pIsupMessage),err)==SipFail)
				return SipFail;
			if(sip_bcpt_cloneIsupMessage(dest->u.pIsupMessage,\
				src->u.pIsupMessage, err)==SipFail)
			{
				return SipFail;
			}
			break;
#endif			
#ifdef SIP_MWI

            case SipMessageSummaryBody:
			if(sip_mwi_initMesgSummaryMessage(&(dest->u.pSummaryMessage), \
                        err)==SipFail)
				return SipFail;
			if(sip_mwi_cloneMesgSummaryMessage(dest->u.pSummaryMessage,\
                            src->u.pSummaryMessage,err)==SipFail)
			{
				sip_mwi_freeMesgSummaryMessage(dest->u.pSummaryMessage);
				return SipFail;
			}
			break;

#endif
#ifdef SIP_MIME_PARSING			
		case SipMultipartMimeBody:
			if(sip_initMimeMessage(&(dest->u.pMimeMessage),err)==SipFail)
				return SipFail;
			if(sip_bcpt_cloneMimeMessage(dest->u.pMimeMessage,\
				src->u.pMimeMessage, err)==SipFail)
			{
				return SipFail;
			}
			break;
#endif		
#ifdef SIP_SIPMSGBODY
		case SipAppSipBody:
			if(sip_initSipMessage(&(dest->u.pAppSipMessage),\
				src->u.pAppSipMessage->dType,err)==SipFail)
				return SipFail;
			if(__sip_cloneSipMessage(dest->u.pAppSipMessage,\
				src->u.pAppSipMessage, err)==SipFail)
				return SipFail;
			break;
#endif			
		default:
			*err = E_INV_PARAM;
			return SipFail;
	}
/* bcpt ext */
#ifdef SIP_MIME_PARSING
	if ( src->pMimeHeader == SIP_NULL)
		dest->pMimeHeader = SIP_NULL;
	else
	{
		if (sip_initSipMimeHeader(&(dest->pMimeHeader), err)\
				== SipFail)
				return SipFail;
		if ( sip_bcpt_cloneSipMimeHeader(dest->pMimeHeader,src->pMimeHeader,\
			err) == SipFail)
		{
			sip_bcpt_freeSipMimeHeader (dest->pMimeHeader);
			return SipFail;
		}
	}/* end of else */
/* bcpt ext ends */
#endif
	*err = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipAddrSpec 
** DESCRIPTION:makes a deep copy of the source AddrSpec to the destination
**  AddrSpec
** PARAMETERS:
**			pDest(OUT)		- Destination AddrSpec
**			pSource(IN)		- Source AddrSpec
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool sip_cloneSipAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *pDest, SipAddrSpec *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipAddrSpec *pDest;
	SipAddrSpec *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipAddrSpec");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipAddrSpec(ppDest, SipAddrAny, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipAddrSpec (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipAddrSpec");
	return SipSuccess;
}

#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION:sip_cloneSipContactParam 
** DESCRIPTION:makes a deep copy of the source Contact Parameter to the 
**	destination Contact Parameter
** PARAMETERS:
**			pDest(OUT)		- Destination Contact Parameter Line
**			pSource(IN)		- Source Contact Parameter Line
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipContactParam
#ifdef ANSI_PROTO
	(SipContactParam *pDest, SipContactParam *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipContactParam *pDest;
	SipContactParam *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipContactParam");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipContactParam(pDest, SipCParamAny, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipContactParam (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipContactParam");
	return SipSuccess;
}

#endif
/*******************************************************************************
** FUNCTION:sip_cloneSipStringList 
** DESCRIPTION:makes a deep copy of the source String LIst to the destination 
** String List
** PARAMETERS:
**			pDest(OUT)		- Destination String List
**			pSource(IN)		- Source String List
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipStringList
#ifdef ANSI_PROTO
	(SipList *pDest, SipList *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipList *pDest;
	SipList *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipStringList");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_listInit(*ppDest, __sip_freeString, pErr) == SipFail)
		return SipFail; */
	if (sip_listDeleteAll (pDest, pErr) == SipFail)
		return SipFail;
	if  (__sip_cloneSipStringList (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipStringList");
	return SipSuccess;
}

#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************                               
** FUNCTION:sip_cloneSipGenericChallenge 
** DESCRIPTION:makes a deep copy of the source Generic Challenge to the 
** destination Generic Challenge
** PARAMETERS:
**			pDest(OUT)		- Destination Generic Challenge
**			pSource(IN)		- Source Generic Challenge
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool sip_cloneSipGenericChallenge
#ifdef ANSI_PROTO
	(SipGenericChallenge *pDest, SipGenericChallenge *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipGenericChallenge *pDest;
	SipGenericChallenge *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipGenericChallenge");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipGenericChallenge(ppDest, pErr) == SipFail)
		return SipFail; */

	if  (__sip_cloneSipChallenge (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipGenericChallenge");
	return SipSuccess;
}
/*******************************************************************************                               
** FUNCTION:sip_cloneSipGenericCredential 
** DESCRIPTION:makes a deep copy of the source GenericCredential to the 
** destination GenericCredential
** PARAMETERS:
**			pDest(OUT)		- Destination GenericCredential
**			pSource(IN)		- Source GenericCredential
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool sip_cloneSipGenericCredential
#ifdef ANSI_PROTO
	(SipGenericCredential *pDest, SipGenericCredential *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipGenericCredential *pDest;
	SipGenericCredential *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipGenericCredential");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sip_cloneCredential (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipGenericCredential");
	return SipSuccess;
}
#endif
#endif
/*******************************************************************************
** FUNCTION:sip_cloneSipUrl 
** DESCRIPTION:makes a deep copy of the source SipUrl to the destination SipUrl
** PARAMETERS:
**			pDest(OUT)		- Destination SipUrl 
**			pSource(IN)		- Source SipUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipUrl
#ifdef ANSI_PROTO
	(SipUrl *pDest, SipUrl *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipUrl *pDest;
	SipUrl *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipUrl");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipUrl(pDest, pErr) == SipFail)
		return SipFail;*/
	if  (__sip_cloneSipUrl (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipUrl");
	return SipSuccess;
}
#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:sip_cloneSipDateFormat 
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the 
**	destination SipDateFormat 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipDateFormat
#ifdef ANSI_PROTO
	(SipDateFormat *pDest, SipDateFormat *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipDateFormat *pDest;
	SipDateFormat *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipDateFormat");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipDateFormat(ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipDateFormat (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipDateFormat");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipTimeFormat 
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination SipTimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipTimeFormat 
**			pSource(IN)		- Source SipTimeFormat 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipTimeFormat
#ifdef ANSI_PROTO
	(SipTimeFormat *pDest, SipTimeFormat *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipTimeFormat *pDest;
	SipTimeFormat *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipTimeFormat");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipTimeFormat(ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipTimeFormat (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipTimeFormat");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipDateStruct 
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination ** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct 
**			pSource(IN)		- Source SipDateStruct 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipDateStruct
#ifdef ANSI_PROTO
	(SipDateStruct *pDest, SipDateStruct *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipDateStruct *pDest;
	SipDateStruct *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipDateStruct");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipDateStruct (pDest, pErr) == SipFail)
		return SipFail;*/
	if  (__sip_cloneSipDateStruct (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipDateStruct");
	return SipSuccess;
}
#endif /* SIP_DATE */

/*******************************************************************************
** FUNCTION:sdp_cloneSdpTime 
** DESCRIPTION:makes a deep copy of the source SdpTime to the destination  
**	SdpTime 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpTime 
**			pSource(IN)		- Source SdpTime 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sdp_cloneSdpTime
#ifdef ANSI_PROTO
	(SdpTime *pDest, SdpTime *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpTime *pDest;
	SdpTime *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_cloneSdpTime");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSdpTime (ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSdpTime (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_cloneSdpTime");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sdp_cloneSdpMedia 
** DESCRIPTION:makes a deep copy of the source SdpMedia to the destination 
**	SdpMedia 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMedia 
**			pSource(IN)		- Source SdpMedia 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sdp_cloneSdpMedia
#ifdef ANSI_PROTO
	(SdpMedia *pDest, SdpMedia *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpMedia *pDest;
	SdpMedia *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_cloneSdpMedia");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSdpMedia (ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSdpMedia (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_cloneSdpMedia");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sdp_cloneSdpConnection 
** DESCRIPTION:makes a deep copy of the source SdpConnection to the destination
**  SdpConnection 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpConnection 
**			pSource(IN)		- Source SdpConnection 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sdp_cloneSdpConnection
#ifdef ANSI_PROTO
	(SdpConnection *pDest, SdpConnection *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpConnection *pDest;
	SdpConnection *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_cloneSdpConnection");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/*	if (sip_initSdpConnection (pDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSdpConnection (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_cloneSdpConnection");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:sdp_cloneSdpAttr 
** DESCRIPTION:makes a deep copy of the source SdpAttributes to the 
** destination SdpAttributes 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpAttributes 
**			pSource(IN)		- Source SdpAttributes 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool sdp_cloneSdpAttr
#ifdef ANSI_PROTO
	(SdpAttr *pDest,SdpAttr *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpAttr *pDest;
	SdpAttr *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_clonesdp_cloneSdpAttr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sip_cloneSdpAttr (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_clonesdp_cloneSdpAttr");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sdp_cloneSdpMessage 
** DESCRIPTION:makes a deep copy of the source SdpMessage to the destination SdpMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpMessage 
**			pSource(IN)		- Source SdpMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sdp_cloneSdpMessage
#ifdef ANSI_PROTO
	(SdpMessage *pDest, SdpMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpMessage *pDest;
	SdpMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_cloneSdpMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSdpMessage (ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSdpMessage (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_cloneSdpMessage");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:sdp_cloneSdpOrigin 
** DESCRIPTION:makes a deep copy of the source SdpOrigin to the destination 
** SdpOrigin 
** PARAMETERS:
**			pDest(OUT)		- Destination SdpOrigin 
**			pSource(IN)		- Source SdpOrigin 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sdp_cloneSdpOrigin
#ifdef ANSI_PROTO
	(SdpOrigin *pDest, SdpOrigin *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SdpOrigin *pDest;
	SdpOrigin *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sdp_cloneSdpOrigin");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sip_cloneSdpOrigin (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sdp_cloneSdpOrigin");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipUnknownMessage 
** DESCRIPTION:makes a deep copy of the source SipUnknownMessage to the 
** destination SipUnknownMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipUnknownMessage 
**			pSource(IN)		- Source SipUnknownMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipUnknownMessage
#ifdef ANSI_PROTO
	(SipUnknownMessage *pDest, SipUnknownMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipUnknownMessage *pDest;
	SipUnknownMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipUnknownMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/*	if (sip_initSipUnknownMessage (ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipUnknownMessage (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipUnknownMessage");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipMsgBody 
** DESCRIPTION:makes a deep copy of the source SipMessageBody to the 
** destination SipMessageBody 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessageBody 
**			pSource(IN)		- Source SipMessageBody 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipMsgBody
#ifdef ANSI_PROTO
	(SipMsgBody *pDest, SipMsgBody *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipMsgBody *pDest;
	SipMsgBody *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipMsgBody");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sip_cloneSipMsgBody (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipMsgBody");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:sip_cloneSipParam 
** DESCRIPTION:makes a deep copy of the source SipParam to the destination 
** SipParam 
** PARAMETERS:
**			pDest(OUT)		- Destination SipParam 
**			pSource(IN)		- Source SipParam 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool sip_cloneSipParam
#ifdef ANSI_PROTO
	(SipParam *pDest, SipParam *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipParam *pDest;
	SipParam *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipParam");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
/* 	if (sip_initSipParam (ppDest, pErr) == SipFail)
		return SipFail; */
	if  (__sip_cloneSipParam (pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipParam");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:sip_cloneSipHeader 
** DESCRIPTION:makes a deep copy of the source SipHeader to the destination 
** SipHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipHeader 
**			pSource(IN)		- Source SipHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool sip_cloneSipHeader
#ifdef ANSI_PROTO
	(SipHeader *pDest, SipHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipHeader *pDest;
	SipHeader *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipHeader");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pSource->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	if(pSource->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}

#ifdef SIP_DATE
	if((pSource->dType != SipHdrTypeExpiresSec) && 
		(pSource->dType != SipHdrTypeExpiresDate) &&
		(pSource->dType != SipHdrTypeRetryAfterSec) &&
		(pSource->dType != SipHdrTypeRetryAfterDate)) 
	{
#endif

	if (sip_cloneSipCommonHeader((SipCommonHeader *)\
				(pDest->pHeader), (SipCommonHeader *)(pSource->pHeader),\
				pErr)== SipFail)
		return SipFail;
#ifdef SIP_DATE
	}
	else
	{
		if (sip_cloneSipCommonHeaderWithDate((SipCommonHeader *)\
					(pDest->pHeader), (SipCommonHeader *)(pSource->pHeader),\
					pErr)== SipFail)
			return SipFail;
	}
#endif

	pDest->dType = pSource->dType;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipHeader");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_cloneSipMessage 
** DESCRIPTION:makes a deep copy of the source SipMessage to the destination 
** SipMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessage 
**			pSource(IN)		- Source SipMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_cloneSipMessage
#ifdef ANSI_PROTO
	(SipMessage *pDest, SipMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipMessage *pDest;
	SipMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneSipMessage");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pSource == SIP_NULL)||(pDest == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if (__sip_cloneSipMessage(pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneSipMessage");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipOrderInfoList
** DESCRIPTION:makes a deep copy of the source SipList of Header order Info to 
** the destination destination list of header order info 
** PARAMETERS:
**			pDest(OUT)		- Destination SipList
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool __sip_cloneSipOrderInfoList
#ifdef ANSI_PROTO
	(SipList *pDest, SipList *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipList *pDest;
	SipList *pSource;
	SipError *pErr;
#endif
{
	SipHeaderOrderInfo *temp_order, *clone_order;
	SIP_U32bit count, index_1;

	SIPDEBUGFN("Entering function __sip_cloneSipOrderInfoList");
	/* copying siplist of SipHeaderOrderInfo */
	if ( sip_listDeleteAll(pDest, pErr ) == SipFail )
		return SipFail;
	if ( sip_listSizeOf(pSource, &count, pErr) == SipFail )
	{
		return SipFail;
	}
	for ( index_1 = 0; index_1 < count ; index_1++)
	{
		if( sip_listGetAt(pSource,index_1, (SIP_Pvoid * ) (&temp_order), pErr)\
			== SipFail )
		{
			return SipFail;
		}
		if ( temp_order == SIP_NULL )
			clone_order = SIP_NULL;
		else
		{
			if(sip_initSipHeaderOrderInfo(&clone_order,pErr)==SipFail)
			{
				return SipFail;
			}

			if ( clone_order == SIP_NULL )
				return SipFail;

			if (__sip_cloneSipHeaderOrderInfo( clone_order, temp_order, pErr)\
				== SipFail )
			{
				(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(clone_order),\
					SIP_NULL);
				return SipFail;
			}
		}

		if ( sip_listAppend(pDest, clone_order, pErr) == SipFail )
		{
			if ( clone_order != SIP_NULL )
			{
				(void)sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(clone_order),\
					SIP_NULL);
			}
			return SipFail;
		}
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipOrderInfoList");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneMessageBodyList
** DESCRIPTION:makes a deep copy of the source SipList of Message Body to the 
** destination SipList of message body
** PARAMETERS:
**			pDest(OUT)		- Destination SipList 
**			pSource(IN)		- Source SipList
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool __sip_cloneSipMessageBodyList
#ifdef ANSI_PROTO
	(SipList *pDest, SipList *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipList *pDest;
	SipList *pSource;
	SipError *pErr;
#endif
{
	SipMsgBody *temp_msgbody, *clone_msgbody;
	SIP_U32bit count, index_1;

	SIPDEBUGFN("Entering function __sip_cloneSipMessageBodyList");
	/* copying siplist of SipMsgBody */
	if ( sip_listDeleteAll(pDest, pErr ) == SipFail )
		return SipFail;
	if ( sip_listSizeOf(pSource, &count, pErr) == SipFail )
	{
		return SipFail;
	}
	for ( index_1 = 0; index_1 < count ; index_1++)
	{
		if( sip_listGetAt(pSource,index_1, (SIP_Pvoid * ) (&temp_msgbody),\
			pErr) == SipFail )
		{
			return SipFail;
		}
		if ( temp_msgbody == SIP_NULL )
			clone_msgbody = SIP_NULL;
		else
		{
			if(sip_initSipMsgBody(&clone_msgbody, SipBodyAny, pErr)==SipFail)
			{
				return SipFail;
			}

			if ( clone_msgbody == SIP_NULL )
				return SipFail;

			if ( __sip_cloneSipMsgBody(clone_msgbody, temp_msgbody, pErr)\
				== SipFail )
			{
				(void)sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(clone_msgbody),\
					SIP_NULL);
				return SipFail;
			}
		}

		if ( sip_listAppend(pDest, clone_msgbody, pErr) == SipFail )
		{
			if ( clone_msgbody != SIP_NULL )
			{
				(void)sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(clone_msgbody),\
					SIP_NULL);
			}
			return SipFail;
		}
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipMessageBodyList");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipGeneralHeader
** DESCRIPTION:makes a deep copy of the source SipGeneralHeader to the 
** destination SipGeneralHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipGeneralHeader 
**			pSource(IN)		- Source SipGeneralHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipGeneralHeader
#ifdef ANSI_PROTO
	(SipGeneralHeader *pDest, SipGeneralHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipGeneralHeader *pDest;
	SipGeneralHeader *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipGeneralHeader");
	/* clearing up the destination parameter */
	if (sip_listDeleteAll(&(pDest->slAllowHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slContactHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slRecordRouteHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slAcceptHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_PRIVACY
  sip_freeSipPrivacyHeader(pDest->pPrivacyHdr); 
  if (sip_listDeleteAll(&(pDest->slPAssertIdHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slPPreferredIdHdr), pErr) == SipFail)
		return SipFail;
#endif	
#ifdef SIP_PEMEDIA
if (sip_listDeleteAll(&(pDest->slPEarlyMediaHdr), pErr) == SipFail)
		return SipFail;
if (__sip_cloneSipPEarlyMediaHeaderList (&(pDest->slPEarlyMediaHdr)\
				, &(pSource->slPEarlyMediaHdr), pErr) == SipFail)
		return SipFail;
#endif
#ifdef SIP_3GPP
	if (sip_listDeleteAll(&(pDest->slPathHdr), pErr) == SipFail)
		return SipFail;
	sip_freeSipPcfAddrHeader (pDest->pPcfAddrHdr);
	sip_freeSipPcVectorHeader (pDest->pPcVectorHdr);
	sip_freeSipPanInfoHeader (pDest->pPanInfoHdr);
	if (sip_listDeleteAll(&(pDest->slHistoryInfoHdr), pErr) == SipFail)
		return SipFail;
#endif
	if (sip_listDeleteAll(&(pDest->slViaHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slContentEncodingHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slUnknownHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slSupportedHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slContentDispositionHdr), pErr) == SipFail)
		return SipFail;
 	if (sip_listDeleteAll(&(pDest->slRequireHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_IMPP
	if (sip_listDeleteAll(&(pDest->slAllowEventsHdr), pErr) == SipFail)
		return SipFail;
#endif
	sip_freeSipCallIdHeader (pDest->pCallidHdr);
	sip_freeSipCseqHeader (pDest->pCseqHdr);
	sip_freeSipExpiresHeader (pDest->pExpiresHdr);
#ifdef SIP_TIMESTAMP			
	sip_freeSipTimeStampHeader (pDest->pTimeStampHdr);
#endif	
	sip_freeSipToHeader (pDest->pToHdr);
	sip_freeSipContentLengthHeader (pDest->pContentLengthHdr);
	sip_freeSipContentTypeHeader (pDest->pContentTypeHdr);
	sip_bcpt_freeSipMimeVersionHeader (pDest->pMimeVersionHdr);
	sip_freeSipRetryAfterHeader (pDest->pRetryAfterHdr);
#ifdef SIP_SESSIONTIMER
	sip_freeSipMinSEHeader (pDest->pMinSEHdr);
	sip_freeSipSessionExpiresHeader (pDest->pSessionExpiresHdr);
#endif

	/* copying source fields */
	if (__sip_cloneSipAllowHeaderList (&(pDest->slAllowHdr), \
				&(pSource->slAllowHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipContactHeaderList (&(pDest->slContactHdr), \
				&(pSource->slContactHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipAcceptHeaderList (&(pDest->slAcceptHdr), \
				&(pSource->slAcceptHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipRecordRouteHeaderList (&(pDest->slRecordRouteHdr), \
				&(pSource->slRecordRouteHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_PRIVACY
	if (pSource->pPrivacyHdr != SIP_NULL)
	{
		if ( sip_initSipPrivacyHeader(&(pDest->pPrivacyHdr), pErr) == SipFail )
			return SipFail;
		if (__sip_cloneSipPrivacyHeader(pDest->pPrivacyHdr, pSource->pPrivacyHdr, \
			pErr) == SipFail)
			return SipFail;
	}
    if (__sip_cloneSipPAssertIdHeaderList (&(pDest->slPAssertIdHdr), \
				&(pSource->slPAssertIdHdr), pErr) == SipFail)
		return SipFail;
  if (__sip_cloneSipPPreferredIdHeaderList (&(pDest->slPPreferredIdHdr), \
				&(pSource->slPPreferredIdHdr), pErr) == SipFail)
		return SipFail;
#endif
	
#ifdef SIP_3GPP
		if (__sip_cloneSipPathHeaderList (&(pDest->slPathHdr), \
				&(pSource->slPathHdr), pErr) == SipFail)
		return SipFail;
		if(pSource->pPcfAddrHdr != SIP_NULL)
		{
			if (sip_initSipPcfAddrHeader (&(pDest->pPcfAddrHdr), pErr) == SipFail)
				return SipFail;
			if (__sip_cloneSipPcfAddrHeader (pDest->pPcfAddrHdr, pSource->pPcfAddrHdr, pErr)\
			   	== 	SipFail)
				return SipFail;
		}
		if(pSource->pPanInfoHdr != SIP_NULL)
		{
			if (sip_initSipPanInfoHeader (&(pDest->pPanInfoHdr), pErr) == SipFail)
				return SipFail;
			if (__sip_cloneSipPanInfoHeader (pDest->pPanInfoHdr, pSource->pPanInfoHdr, pErr)\
			   	== 	SipFail)
				return SipFail;
		}
		if(pSource->pPcVectorHdr != SIP_NULL)
		{
			if (sip_initSipPcVectorHeader (&(pDest->pPcVectorHdr), pErr) == SipFail)
				return SipFail;
			if (__sip_cloneSipPcVectorHeader (pDest->pPcVectorHdr, pSource->pPcVectorHdr, pErr)\
			   	== 	SipFail)
				return SipFail;
		}
		if (__sip_cloneSipHistoryInfoHeaderList (&(pDest->slHistoryInfoHdr), \
				&(pSource->slHistoryInfoHdr), pErr) == SipFail)
		return SipFail;
#endif
	if (__sip_cloneSipViaHeaderList (&(pDest->slViaHdr), \
				&(pSource->slViaHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipContentEncodingHeaderList (&(pDest->slContentEncodingHdr)\
				, &(pSource->slContentEncodingHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipUnknownHeaderList (&(pDest->slUnknownHdr), \
				&(pSource->slUnknownHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipSupportedHeaderList (&(pDest->slSupportedHdr), \
				&(pSource->slSupportedHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipRequireHeaderList (&(pDest->slRequireHdr), \
				&(pSource->slRequireHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipContentDispositionHeaderList \
			(&(pDest->slContentDispositionHdr), \
			 &(pSource->slContentDispositionHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_IMPP
	if (__sip_cloneSipAllowEventsHeaderList (&(pDest->slAllowEventsHdr), \
				&(pSource->slAllowEventsHdr), pErr) == SipFail)
		return SipFail;
#endif
	if(pSource->pCallidHdr != SIP_NULL)
	{
		if (sip_initSipCallIdHeader (&(pDest->pCallidHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipCallIdHeader(pDest->pCallidHdr, pSource->pCallidHdr, \
					pErr) == SipFail)
			return SipFail;
	}

	if(pSource->pCseqHdr != SIP_NULL)
	{
		if (sip_initSipCseqHeader (&(pDest->pCseqHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipCseqHeader (pDest->pCseqHdr, pSource->pCseqHdr, pErr)\
			   	== 	SipFail)
			return SipFail;
	}

	if(pSource->pExpiresHdr != SIP_NULL)
	{
#ifdef SIP_DATE
		if (sip_initSipExpiresHeader (&(pDest->pExpiresHdr), pSource->\
			pExpiresHdr->dIntVar1,pErr) == SipFail)
#else
		if (sip_initSipExpiresHeader (&(pDest->pExpiresHdr),
			pErr) == SipFail)
#endif
			return SipFail;
		if (__sip_cloneSipExpiresHeader (pDest->pExpiresHdr, \
					pSource->pExpiresHdr, pErr) == SipFail)
			return SipFail;
	}

	if(pSource->pFromHeader != SIP_NULL)
	{
		if (sip_initSipFromHeader (&(pDest->pFromHeader), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipFromHeader (pDest->pFromHeader, pSource->pFromHeader,\
					pErr) == SipFail)
			return SipFail;
	}

#ifdef SIP_TIMESTAMP			
	if(pSource->pTimeStampHdr != SIP_NULL)
	{
		if (sip_initSipTimeStampHeader(&(pDest->pTimeStampHdr), pErr)== SipFail)
			return SipFail;
		if (__sip_cloneSipTimeStampHeader (pDest->pTimeStampHdr, pSource->\
					pTimeStampHdr, pErr) == SipFail)
			return SipFail;
	}
#endif	

	if(pSource->pToHdr != SIP_NULL)
	{
		if (sip_initSipToHeader (&(pDest->pToHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipToHeader (pDest->pToHdr, pSource->pToHdr, pErr) == \
				SipFail)
			return SipFail;
	}

	if(pSource->pContentLengthHdr != SIP_NULL)
	{
		if (sip_initSipContentLengthHeader (&(pDest->pContentLengthHdr), \
					pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipContentLengthHeader (pDest->pContentLengthHdr, \
					pSource->pContentLengthHdr, pErr) == SipFail)
			return SipFail;
	}

	if(pSource->pContentTypeHdr != SIP_NULL)
	{
		if (sip_initSipContentTypeHeader (&(pDest->pContentTypeHdr), pErr) == \
				SipFail)
			return SipFail;
		if (__sip_cloneSipContentTypeHeader (pDest->pContentTypeHdr, pSource->\
					pContentTypeHdr, pErr) == SipFail)
			return SipFail;
	}

	if(pSource->pMimeVersionHdr != SIP_NULL)
	{
		if (sip_initSipMimeVersionHeader (&(pDest->pMimeVersionHdr), pErr) == \
				SipFail)
			return SipFail;
		if (__sip_cloneSipMimeVersionHeader (pDest->pMimeVersionHdr, pSource->\
					pMimeVersionHdr, pErr) == SipFail)
			return SipFail;
	}

	if(pSource->pRetryAfterHdr != SIP_NULL)
	{
#ifdef SIP_DATE
		if (sip_initSipRetryAfterHeader (&(pDest->pRetryAfterHdr),SipExpAny, \
					pErr) == SipFail)
#else
		if (sip_initSipRetryAfterHeader (&(pDest->pRetryAfterHdr), \
					pErr) == SipFail)
#endif
			return SipFail;
		if (__sip_cloneSipRetryAfterHeader (pDest->pRetryAfterHdr, \
					pSource->pRetryAfterHdr, pErr) == SipFail)
			return SipFail;
	}

#ifdef SIP_SESSIONTIMER
	if (pSource->pMinSEHdr != SIP_NULL)
	{
		if ( sip_initSipMinSEHeader(&(pDest->pMinSEHdr), pErr) == SipFail )
			return SipFail;
		if (__sip_cloneSipMinSEHeader(pDest->pMinSEHdr, pSource->pMinSEHdr, \
			pErr) == SipFail)
			return SipFail;
	}

	if (pSource->pSessionExpiresHdr != SIP_NULL)
	{
	if (sip_initSipSessionExpiresHeader(&(pDest->pSessionExpiresHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipSessionExpiresHeader(pDest->pSessionExpiresHdr, \
		pSource->pSessionExpiresHdr, pErr) == SipFail)
		return SipFail;
	}
#endif


	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Entering function __sip_cloneSipGeneralHeader");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipReqheader
** DESCRIPTION:makes a deep copy of the source SipReqHeader to the destination 
** SipReqHeader
** PARAMETERS:
**			pDest(OUT)		- Destination SipReqHeader 
**			pSource(IN)		- Source SipReqHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipReqHeader
#ifdef ANSI_PROTO
	(SipReqHeader *pDest, SipReqHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipReqHeader *pDest;
	SipReqHeader *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipReqHeader");
	/* Clearing destination parameter */
	if (sip_listDeleteAll(&(pDest->slProxyAuthorizationHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slRouteHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slAuthorizationHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_CONGEST
	if (sip_listDeleteAll(&(pDest->slRsrcPriorityHdr), pErr) == SipFail)
		return SipFail;
#endif
#ifdef SIP_IMPP
	sip_freeSipEventHeader (pDest->pEventHdr);
	if(pSource->pEventHdr != SIP_NULL)
	{
	if (sip_initSipEventHeader (&(pDest->pEventHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipEventHeader (pDest->pEventHdr, pSource->pEventHdr, pErr) == SipFail)
		return SipFail;
	}

	sip_impp_freeSipSubscriptionStateHeader(pDest->pSubscriptionStateHdr);
	if(pSource->pSubscriptionStateHdr != SIP_NULL)
	{
	if (sip_initSipSubscriptionStateHeader(&(pDest->pSubscriptionStateHdr), \
		pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipSubscriptionStateHeader(pDest->pSubscriptionStateHdr,\
		pSource->pSubscriptionStateHdr, pErr) == SipFail)
		return SipFail;
	}
#endif
#ifdef SIP_REPLACES
	sip_freeSipReplacesHeader(pDest->pReplacesHdr);
	if(pSource->pReplacesHdr != SIP_NULL)
	{
	if (sip_initSipReplacesHeader(&(pDest->pReplacesHdr), \
		pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipReplacesHeader(pDest->pReplacesHdr,\
		pSource->pReplacesHdr, pErr) == SipFail)
		return SipFail;
	}
#endif
#ifdef SIP_RPR
	sip_freeSipRackHeader (pDest->pRackHdr);
#endif	
#ifdef SIP_REFER	
	sip_freeSipReferToHeader(pDest->pReferToHdr); /* call-transfer */
	sip_freeSipReferredByHeader (pDest->pReferredByHdr); /* call-tansfer */
#endif	
	/* now copying the source fields */
	if (__sip_cloneSipRouteHeaderList (&(pDest->slRouteHdr), \
				&(pSource->slRouteHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_AUTHENTICATE		
	if (__sip_cloneSipAuthorizationHeaderList (&(pDest->slAuthorizationHdr), \
				&(pSource->slAuthorizationHdr), pErr) == SipFail)
		return SipFail;

	if (__sip_cloneSipProxyAuthorizationHeaderList \
			(&(pDest->slProxyAuthorizationHdr), \
			 &(pSource->slProxyAuthorizationHdr), pErr) == SipFail)
		return SipFail;
#endif	
#ifdef SIP_RPR
	if(pSource->pRackHdr != SIP_NULL)
	{
		if (sip_initSipRackHeader (&(pDest->pRackHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_rpr_cloneSipRAckHeader (pDest->pRackHdr, pSource->pRackHdr,\
				   	pErr) == SipFail)
			return SipFail;
	}
#endif	

/* call-transfer */
#ifdef SIP_REFER
	if(pSource->pReferToHdr != SIP_NULL)
	{
		if (sip_initSipReferToHeader (&(pDest->pReferToHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipReferToHeader (pDest->pReferToHdr, pSource->\
					pReferToHdr, pErr) == SipFail)
			return SipFail;
	}
/* call-transfer */
	if(pSource->pReferredByHdr != SIP_NULL)
	{
		if (sip_initSipReferredByHeader (&(pDest->pReferredByHdr),pErr) == \
				SipFail)
			return SipFail;
		if (__sip_cloneSipReferredByHeader (pDest->pReferredByHdr, \
					pSource->pReferredByHdr, pErr) == SipFail)
			return SipFail;
	}
#endif	
	sip_freeSipMaxForwardsHeader(pDest->pMaxForwardsHdr);
	if(pSource->pMaxForwardsHdr != SIP_NULL)
	{
	if (sip_initSipMaxForwardsHeader(&(pDest->pMaxForwardsHdr), \
		pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipMaxForwardsHeader(pDest->pMaxForwardsHdr,\
		pSource->pMaxForwardsHdr, pErr) == SipFail)
		return SipFail;
	}
	#ifdef SIP_3GPP

	if(pSource->pPCalledPartyIdHdr != SIP_NULL)
	{
		if (sip_initSipPCalledPartyIdHeader (&(pDest->pPCalledPartyIdHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipPCalledPartyIdHeader (pDest->pPCalledPartyIdHdr, pSource->\
					pPCalledPartyIdHdr, pErr) == SipFail)
			return SipFail;
	}
	if(pSource->pJoinHdr != SIP_NULL)
	{
		if (sip_initSipJoinHeader (&(pDest->pJoinHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipJoinHeader (pDest->pJoinHdr, pSource->\
					pJoinHdr, pErr) == SipFail)
			return SipFail;
	}
	if(pSource->pIfMatchHdr != SIP_NULL)
	{
		if (sip_initSipIfMatchHeader (&(pDest->pIfMatchHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipIfMatchHeader (pDest->pIfMatchHdr, pSource->\
					pIfMatchHdr, pErr) == SipFail)
			return SipFail;
	}
		
	if (__sip_cloneSipPVisitedNetworkIdHeaderList (&(pDest->slPVisitedNetworkIdHdr), \
				&(pSource->slPVisitedNetworkIdHdr), pErr) == SipFail)
		return SipFail;

	if (__sip_cloneSipRequestDispositionHeaderList (&(pDest->slRequestDispositionHdr), \
				&(pSource->slRequestDispositionHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipAcceptContactHeaderList (&(pDest->slAcceptContactHdr), \
				&(pSource->slAcceptContactHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipRejectContactHeaderList (&(pDest->slRejectContactHdr), \
				&(pSource->slRejectContactHdr), pErr) == SipFail)
		return SipFail;

#endif
#ifdef SIP_SECURITY
    if (__sip_cloneSipSecurityClientHeaderList (&(pDest->slSecurityClientHdr), \
				&(pSource->slSecurityClientHdr), pErr) == SipFail)
		return SipFail;
    if (__sip_cloneSipSecurityVerifyHeaderList (&(pDest->slSecurityVerifyHdr), \
				&(pSource->slSecurityVerifyHdr), pErr) == SipFail)
		return SipFail;
#endif    
#ifdef SIP_CONGEST
	if (__sip_cloneSipRsrcPriorityHeaderList(&(pDest->slRsrcPriorityHdr), \
				&(pSource->slRsrcPriorityHdr), pErr) == SipFail)
		return SipFail;
#endif
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Entering function __sip_cloneSipReqHeader");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipRespHeader
** DESCRIPTION:makes a deep copy of the source SipRespHeader to the destination 
** SipRespHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRespHeader 
**			pSource(IN)		- Source SipRespHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipRespHeader
#ifdef ANSI_PROTO
	(SipRespHeader *pDest, SipRespHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipRespHeader *pDest;
	SipRespHeader *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipRespHeader");
	/* clear the destination parameter */
	if (sip_listDeleteAll(&(pDest->slProxyAuthenticateHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slUnsupportedHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_WARNING		
	if (sip_listDeleteAll(&(pDest->slWarningHeader), pErr) == SipFail)
		return SipFail;
#endif		
	if (sip_listDeleteAll(&(pDest->slWwwAuthenticateHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_RPR		
	sip_freeSipRseqHeader (pDest->pRSeqHdr);
#endif	
#ifdef SIP_3GPP
	if (sip_listDeleteAll(&(pDest->slPAssociatedUriHdr), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slServiceRouteHdr), pErr) == SipFail)
		return SipFail;

#endif
#ifdef SIP_CONGEST
	if (sip_listDeleteAll(&(pDest->slAcceptRsrcPriorityHdr), pErr) == SipFail)
		return SipFail;
#endif
	sip_freeSipExpiresHeader (pDest->pMinExpiresHdr);

	/* copy source parameter */
#ifdef SIP_AUTHENTICATE	
	if (__sip_cloneSipProxyAuthenticateHeaderList \
			(&(pDest->slProxyAuthenticateHdr), \
			 &(pSource->slProxyAuthenticateHdr), pErr) == SipFail)
		return SipFail;
#endif		
	if (__sip_cloneSipUnsupportedHeaderList (&(pDest->slUnsupportedHdr), \
				&(pSource->slUnsupportedHdr), pErr) == SipFail)
		return SipFail;
#ifdef SIP_WARNING		
	if (__sip_cloneSipWarningHeaderList (&(pDest->slWarningHeader), \
				&(pSource->slWarningHeader), pErr) == SipFail)
		return SipFail;
#endif		
#ifdef SIP_AUTHENTICATE		
	if (__sip_cloneSipWwwAuthenticateHeaderList \
			(&(pDest->slWwwAuthenticateHdr), &(pSource->slWwwAuthenticateHdr), \
			 pErr) == SipFail)
		return SipFail;
#endif	
#ifdef SIP_RPR
	if(pSource->pRSeqHdr != SIP_NULL)
	{
		if (sip_initSipRseqHeader (&(pDest->pRSeqHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_rpr_cloneSipRSeqHeader (pDest->pRSeqHdr, pSource->pRSeqHdr, \
					pErr) == SipFail)
			return SipFail;
	}
#endif
	if(pSource->pMinExpiresHdr != SIP_NULL)
	{
		if (sip_initSipMinExpiresHeader (&(pDest->pMinExpiresHdr),
			pErr) == SipFail)
			return SipFail;

		if (__sip_cloneSipMinExpiresHeader (pDest->pMinExpiresHdr, \
					pSource->pMinExpiresHdr, pErr) == SipFail)
			return SipFail;
	}

#ifdef SIP_3GPP
		if(__sip_cloneSipPAssociatedUriHeaderList \
			(&(pDest->slPAssociatedUriHdr), &(pSource->slPAssociatedUriHdr), \
			 pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipServiceRouteHeaderList (&(pDest->slServiceRouteHdr), \
				&(pSource->slServiceRouteHdr), pErr) == SipFail)
		return SipFail;

		if(pSource->pETagHdr != SIP_NULL)
		{
		if (sip_initSipETagHeader (&(pDest->pETagHdr), pErr) == SipFail)
			return SipFail;
		if (__sip_cloneSipETagHeader (pDest->pETagHdr, pSource->pETagHdr, \
					pErr) == SipFail)
			return SipFail;
		}

#endif
#ifdef SIP_SECURITY
	if(__sip_cloneSipSecurityServerHeaderList \
			(&(pDest->slSecurityServerHdr), &(pSource->slSecurityServerHdr), \
			 pErr) == SipFail)
              return SipFail;
#endif

#ifdef SIP_CONGEST
	if (__sip_cloneSipAcceptRsrcPriorityHeaderList(&(pDest->slAcceptRsrcPriorityHdr), \
				&(pSource->slAcceptRsrcPriorityHdr), pErr) == SipFail)
		return SipFail;
#endif
	
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Entering function __sip_cloneSipRespHeader");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipReqMessage
** DESCRIPTION:makes a deep copy of the source SipReqMessage to the destination 
** SipReqMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SipReqMessage
**			pSource(IN)		- Source SipReqMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipReqMessage
#ifdef ANSI_PROTO
	(SipReqMessage *pDest, SipReqMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipReqMessage *pDest;
	SipReqMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipReqMessage");
	/* clear destination parameter */
	if (pDest->pRequestLine != SIP_NULL)
		sip_freeSipReqLine(pDest->pRequestLine);
	if (pDest->pRequestHdr != SIP_NULL)
		sip_freeSipReqHeader (pDest->pRequestHdr);
	/* Now copying source */
	if (sip_initSipReqLine (&(pDest->pRequestLine), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipReqLine(pDest->pRequestLine, pSource->pRequestLine, \
				pErr) == SipFail)
		return SipFail;
	if (sip_initSipReqHeader(&(pDest->pRequestHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipReqHeader(pDest->pRequestHdr, pSource->pRequestHdr, \
				pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipReqMessage");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: __sip_cloneSipHeaderOrderInfo
** DESCRIPTION:makes a deep copy of the source SipHeaderOrderInfo to the 
** destination  SipHeaderOrderInfo 
** PARAMETERS:
**			pDest(OUT)		- Destination SipHeaderOrderInfo 
**			pSource(IN)		- Source SipHeaderOrderInfo 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipHeaderOrderInfo
#ifdef ANSI_PROTO
	(SipHeaderOrderInfo *pDest, SipHeaderOrderInfo *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipHeaderOrderInfo *pDest;
	SipHeaderOrderInfo *pSource;
	SipError *pErr;
#endif
{
 	SIPDEBUGFN("Entering function __sip_cloneSipHeaderOrderInfo");
	/* no pointers in dest so nothing to be cleared. copying source */
	pDest->dType = pSource->dType;
	pDest->dTextType = pSource->dTextType;
	pDest->dNum = pSource->dNum;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipHeaderOrderInfo");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION:__sip_cloneSipRespMessage
** DESCRIPTION:makes a deep copy of the source SipRespMessage to the 
** destination SipRespMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipRespMessage
**			pSource(IN)		- Source SipRespMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipRespMessage
#ifdef ANSI_PROTO
	(SipRespMessage *pDest, SipRespMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipRespMessage *pDest;
	SipRespMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipRespMessage");
	/* clear destination parameter */
	if (pDest->pStatusLine != SIP_NULL)
		sip_freeSipStatusLine(pDest->pStatusLine);
	if (pDest->pResponseHdr != SIP_NULL)
		sip_freeSipRespHeader (pDest->pResponseHdr);
	/* Now copying source */
	if (sip_initSipStatusLine (&(pDest->pStatusLine), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipStatusLine(pDest->pStatusLine, pSource->pStatusLine, \
				pErr) == SipFail)
		return SipFail;
	if (sip_initSipRespHeader(&(pDest->pResponseHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipRespHeader(pDest->pResponseHdr, pSource->pResponseHdr, \
				pErr) == SipFail)
		return SipFail;
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipRespMessage");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION:__sip_cloneSipMessage
** DESCRIPTION:makes a deep copy of the source SipMessage to the destination 
** SipMessage
** PARAMETERS:
**			pDest(OUT)		- Destination SipMessage 
**			pSource(IN)		- Source SipMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipMessage
#ifdef ANSI_PROTO
	(SipMessage *pDest, SipMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipMessage *pDest;
	SipMessage *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function __sip_cloneSipMessage");
	/* clear destination Message */
	if (sip_listDeleteAll(&(pDest->slOrderInfo), pErr) == SipFail)
		return SipFail;
	if (sip_listDeleteAll(&(pDest->slMessageBody), pErr) == SipFail)
		return SipFail;
	sip_freeSipGeneralHeader (pDest->pGeneralHdr);
	switch (pDest->dType)
	{
		case SipMessageRequest:
			sip_freeSipReqMessage ((pDest->u).pRequest);
			break;
		case SipMessageResponse:
			sip_freeSipRespMessage ((pDest->u).pResponse);
			break;
		case SipMessageAny:
			break;
		default:
			*pErr = E_INV_PARAM;
			return SipFail;
	}
	/* copy Message Type */
	pDest->dType = pSource->dType;
	/* copy Order Info */
	if (__sip_cloneSipOrderInfoList(&(pDest->slOrderInfo),\
		&(pSource->slOrderInfo), pErr) == SipFail)
		return SipFail;
	/* copy Message Body list */
	if (__sip_cloneSipMessageBodyList(&(pDest->slMessageBody),\
		&(pSource->slMessageBody), pErr) == SipFail)
		return SipFail;
	/* copy General Headsers */
	if (sip_initSipGeneralHeader (&(pDest->pGeneralHdr), pErr) == SipFail)
		return SipFail;
	if (__sip_cloneSipGeneralHeader(pDest->pGeneralHdr, pSource->pGeneralHdr,\
		pErr) == SipFail)
		return SipFail;
	/* Copy REQUEST/RESPONSE headers */
	switch(pSource->dType)
	{
		case SipMessageRequest:
			if (sip_initSipReqMessage(&(pDest->u.pRequest), pErr) == SipFail)
				return SipFail;
			if (__sip_cloneSipReqMessage(pDest->u.pRequest,\
				pSource->u.pRequest, pErr) == SipFail)
			{
				sip_freeSipReqMessage (pDest->u.pRequest);
				return SipFail;
			}
			break;
		case SipMessageResponse:
			if (sip_initSipRespMessage(&(pDest->u.pResponse), pErr) == SipFail)
				return SipFail;
			if (__sip_cloneSipRespMessage (pDest->u.pResponse,\
				pSource->u.pResponse, pErr) == SipFail)
			{
				sip_freeSipRespMessage (pDest->u.pResponse);
				return SipFail;
			}
			break;
		case SipMessageAny:
			break;
		default:
			*pErr = E_INV_PARAM;
			return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneSipMessage");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:__sip_cloneSipBadHeader 
** DESCRIPTION:makes a deep copy of the source SipBadHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipBadHeader
**			pSource(IN)		- Source SipBadHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef  SIP_BADMESSAGE_PARSING
SipBool __sip_cloneSipBadHeader
#ifdef ANSI_PROTO
	(SipBadHeader *dest, SipBadHeader *source, SipError *err)
#else
	(dest, source, err)
	SipBadHeader *dest;
	SipBadHeader *source;
	SipError *err;
#endif
{
	SIP_S8bit *temp;
	SIPDEBUGFN("Entering function ___sip_cloneSipBadHeader");
	if (dest->pName != SIP_NULL)
		sip_freeString(dest->pName);
	if (dest->pBody != SIP_NULL)
		sip_freeString(dest->pBody);

	/* Duplicating pDispName */
	if (source->pName == SIP_NULL)
		temp = SIP_NULL;
	else
	{
		temp = (SIP_S8bit *)STRDUPACCESSOR(source->pName);
		if (temp == SIP_NULL)
		{
			*err = E_NO_MEM;
			return SipFail;
		}
	}
	dest->pName = temp;

	if (source->pBody == SIP_NULL)
		temp = SIP_NULL;
	else
	{
		temp = (SIP_S8bit *)STRDUPACCESSOR(source->pName);
		if (temp == SIP_NULL)
		{
			*err = E_NO_MEM;
			return SipFail;
		}
	}
	dest->pBody = temp;

    dest->dType =source->dType;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function ___sip_cloneSipBadHeader");
	return SipSuccess;
}
#endif

#ifdef SIP_ISUP
/*******************************************************************************
** FUNCTION:sip_bcpt_cloneIsupMessage
** DESCRIPTION:makes a deep copy of the source IsupMessage to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination IsupMessage 
**			pSource(IN)		- Source IsupMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_bcpt_cloneIsupMessage
#ifdef ANSI_PROTO
	(IsupMessage *pDest, IsupMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	IsupMessage *pDest;
	IsupMessage *pSource;
	SipError *pErr;
#endif
{	
	SIPDEBUGFN("Entering function sip_bcpt_cloneIsupMessage");
	
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
	{
		return SipFail;
	}
	if ( pDest == SIP_NULL || pSource == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	/* cleaning up the destination structure */
	if ( pDest->pBody != SIP_NULL )
		(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) (&pDest->pBody), pErr);

	/* duplicating */
	pDest->dLength = (pSource->dLength);
	if ( pSource->pBody == SIP_NULL )
		pDest->pBody = SIP_NULL;
	else
	{
		pDest->pBody = \
			(SIP_S8bit *) fast_memget(ACCESSOR_MEM_ID,pSource->dLength,pErr);
		if(pDest->pBody == SIP_NULL)
			return SipFail;
		memcpy(pDest->pBody,pSource->pBody,pSource->dLength);	
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_bcpt_cloneIsupMessage");
	return SipSuccess;	
}
#endif

/*******************************************************************************
** FUNCTION:validateSipContactType
** DESCRIPTION:   This fucntion returns SipSuccess if "dType" is
** one among the defined en_ContactType's; else it returns SipFail.
** PARAMETERS:
**			dType(IN)		- Contact Type to be validated
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool validateSipContactType
#ifdef ANSI_PROTO
	(en_ContactType dType, SipError *err)
#else
	(dType, err)
	en_ContactType dType;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function validateSipContactType");
	switch (dType)
	{
		case 	SipContactWildCard	:
		case	SipContactNormal	:
		case	SipContactAny		: break;
		default				:*err = E_INV_TYPE;
						 return SipFail;

	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function validateSipContactType");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION:sip_bcpt_cloneMimeMessage
** DESCRIPTION:makes a deep copy of the source MimeMessage to the destination 
** MimeMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination MimeMessage 
**			pSource(IN)		- Source MimeMessage 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
#ifdef SIP_MIME_PARSING
SipBool sip_bcpt_cloneMimeMessage
#ifdef ANSI_PROTO
	(MimeMessage *pDest, MimeMessage *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	MimeMessage *pDest;
	MimeMessage *pSource;
	SipError *pErr;
#endif
{	
	SIP_U32bit index_1, count;
	SipMsgBody	*pMsgB, *pDupMsgB;

	SIPDEBUGFN("Entering function sip_bcpt_cloneMimeMessage");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
	{
		return SipFail;
	}
	if ( pDest == SIP_NULL || pSource == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying siplist of Msg Body */
	if ( sip_listDeleteAll(&( pDest->slRecmimeBody ), pErr ) == SipFail )
		return SipFail;
	if ( sip_listInit(& (pDest->slRecmimeBody ),\
				(pSource->slRecmimeBody).freefunc,pErr)==SipFail)
		return SipFail;
	if ( sip_listSizeOf(&( pSource->slRecmimeBody ), &count, pErr) == SipFail )
		return SipFail;

	for ( index_1 = 0; index_1 < count ; index_1++)
	{
		if( sip_listGetAt(&( pSource->slRecmimeBody ),index_1,\
								(SIP_Pvoid * ) (&pMsgB), pErr) == SipFail )
			return SipFail;

		if ( pMsgB == SIP_NULL )
			pDupMsgB = SIP_NULL;
		else
		{
			(void)sip_initSipMsgBody(&pDupMsgB,pMsgB->dType,pErr);

			if ( pDupMsgB == SIP_NULL )
				return SipFail;

			if ( __sip_cloneSipMsgBody( pDupMsgB, pMsgB, pErr) == SipFail )
			{
				sip_freeSipMsgBody(pDupMsgB);
				return SipFail;
			}
		}

		if ( sip_listAppend(&( pDest->slRecmimeBody ), pDupMsgB, pErr)\
																== SipFail )
		{
			if ( pDupMsgB != SIP_NULL )
				sip_freeSipMsgBody(pDupMsgB);
			return SipFail;
		}
	}
	
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_bcpt_cloneSipMimeMessage");
	return SipSuccess;	
}
/*******************************************************************************
** FUNCTION:__sip_bcpt_cloneSipMimeHeader
** DESCRIPTION:makes a deep copy of the source SipMimeHeader to the destination 
** IsupMessage 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeHeader 
**			pSource(IN)		- Source SipMimeHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_bcpt_cloneSipMimeHeader
#ifdef ANSI_PROTO
	(SipMimeHeader *pDest, SipMimeHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipMimeHeader *pDest;
	SipMimeHeader *pSource;
	SipError *pErr;
#endif
{	
	SIP_S8bit 	*pMime, *pDupMime;
	SIP_U32bit 	index_1, count;
	SipError 	tempErr;
	SIP_S8bit 	mem_flag=0;

	SIPDEBUGFN("Entering function __sip_bcpt_cloneSipMimeHeader");
	
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	
	do{

		/* cleaning up the destination structure */

		/* content tranfer pEncoding */
		if ( pDest->pContentTransEncoding != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID,\
							(SIP_Pvoid *) (&pDest->pContentTransEncoding),pErr);
		/* content id */
		if ( pDest->pContentId != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) (&pDest->pContentId),\
				   	pErr);

		/* content description */
		if ( pDest->pContentDescription != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID,\
					(SIP_Pvoid *) (&pDest->pContentDescription), pErr);

		/* content dType */
		if ( pDest->pContentType != SIP_NULL )
		{
			sip_freeSipContentTypeHeader(pDest->pContentType);
			pDest->pContentType = SIP_NULL;
		}
		/* content Disposition */
		if ( pDest->pContentType != SIP_NULL )
		{
			sip_freeSipContentDispositionHeader(pDest->pContentDisposition);
			pDest->pContentDisposition = SIP_NULL;
		}


		/* Additional MimeHeaders */
		if ( sip_listDeleteAll( &(pDest->slAdditionalMimeHeaders), pErr) == \
				SipFail)
			return SipFail;


		/* duplication begins here  */

		/* contetn transfer pEncoding */
		if ( pSource->pContentTransEncoding == SIP_NULL )
			pDest->pContentTransEncoding = SIP_NULL;
		else
		{
			pDest->pContentTransEncoding = \
						(SIP_S8bit *)STRDUPACCESSOR(pSource->\
													pContentTransEncoding);
			if ( pDest->pContentTransEncoding == SIP_NULL)
			{
				*pErr = E_NO_MEM;
				{mem_flag=1;break;}
			}		
		}

		/* content id */
		if ( pSource->pContentId == SIP_NULL )
			pDest->pContentId = SIP_NULL;
		else
		{
			pDest->pContentId = (SIP_S8bit *)STRDUPACCESSOR\
				(pSource->pContentId);
			if ( pDest->pContentId == SIP_NULL)
			{
				*pErr = E_NO_MEM;
				{mem_flag=1;break;}
			}		
		}

		/* content description */
		if ( pSource->pContentDescription == SIP_NULL )
			pDest->pContentDescription = SIP_NULL;
		else
		{
			pDest->pContentDescription = \
					(SIP_S8bit *)STRDUPACCESSOR(pSource->pContentDescription);
			if ( pDest->pContentDescription == SIP_NULL)
			{
				*pErr = E_NO_MEM;
				{mem_flag=1;break;}
			}		
		}

		/* sontent tyep */
		if ( pSource->pContentType == SIP_NULL )
			pDest->pContentType = SIP_NULL;
		else
		{
			if ( sip_initSipContentTypeHeader\
							(&(pDest->pContentType), pErr)==SipFail)
				return SipFail;

			if ( __sip_cloneSipContentTypeHeader\
				(pDest->pContentType, pSource->pContentType, pErr) == SipFail)
			{
				sip_freeSipContentTypeHeader(pDest->pContentType);
				return SipFail;
			}
		}
		/* Content Disposition */
			if ( pSource->pContentDisposition == SIP_NULL )
					pDest->pContentDisposition = SIP_NULL;
			else
			{
					if ( sip_initSipContentDispositionHeader\
							(&(pDest->pContentDisposition), pErr)==SipFail)
							return SipFail;

					if ( __sip_cloneSipContentDispositionHeader\
					(pDest->pContentDisposition, pSource->pContentDisposition,\
							pErr) == SipFail)
					{
							sip_freeSipContentDispositionHeader\
												(pDest->pContentDisposition);
							return SipFail;
					}
			}


		/* copying siplist of additional mime headers */
		if ( sip_listInit(& (pDest->slAdditionalMimeHeaders ), \
			(pSource->slAdditionalMimeHeaders).freefunc,pErr)==SipFail)
			return SipFail;
		if ( sip_listSizeOf(&( pSource->slAdditionalMimeHeaders ),&count, pErr)\
				== SipFail )
			return SipFail;

		for ( index_1 = 0; index_1 < count ; index_1++)
		{
			if( sip_listGetAt(&( pSource->slAdditionalMimeHeaders ),index_1,  \
				(SIP_Pvoid * ) (&pMime), pErr) == SipFail )
				return SipFail;

			if ( pMime == SIP_NULL )
				pDupMime = SIP_NULL;
			else
			{
				pDupMime = (SIP_S8bit*)STRDUPACCESSOR(pMime);
				if ( pDupMime == SIP_NULL )
				{
					*pErr = E_NO_MEM;
					{mem_flag=1;break;}
				}
				
			}

			if ( sip_listAppend(&( pDest->slAdditionalMimeHeaders ),\
													pDupMime, pErr) == SipFail )
			{
				if ( pDupMime != SIP_NULL )
				  (void)sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pDupMime),&tempErr);
				return SipFail;
			}
		}
		if( mem_flag ) break;
	}while(0);	
	
	if( mem_flag ) 
	{
		sip_bcpt_freeSipMimeHeader(pDest);
		return SipFail;
	}


	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_bcpt_cloneSipMimeHeader");
	return SipSuccess;	
}

/*******************************************************************************
** FUNCTION:sip_bcpt_cloneSipMimeHeader
** DESCRIPTION:makes a deep copy of the source SipMimeHeader to the destination 
** SipMimeHeader 
** PARAMETERS:
**			pDest(OUT)		- Destination SipMimeHeader 
**			pSource(IN)		- Source SipMimeHeader 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool sip_bcpt_cloneSipMimeHeader
#ifdef ANSI_PROTO
	(SipMimeHeader *pDest, SipMimeHeader *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipMimeHeader *pDest;
	SipMimeHeader *pSource;
	SipError *pErr;
#endif
{	
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
	{
		return SipFail;
	}
	if ( pDest == SIP_NULL || pSource == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(__sip_bcpt_cloneSipMimeHeader(pDest,pSource,pErr) == SipFail)
		return SipFail;
	return SipSuccess;	
}
#endif

#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:__sip_cloneSipDateStruct
** DESCRIPTION:makes a deep copy of the source SipDateStruct to the destination 
** SipDateStruct 
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateStruct 
**			pSource(IN)		- Source SipDateStruct 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipDateStruct
#ifdef ANSI_PROTO
	(SipDateStruct * dest, SipDateStruct *src, SipError * err)
#else
	( dest,src , err)
	  SipDateStruct *dest;
	  SipDateStruct *src;
	  SipError 	*err;
#endif
{
	SIPDEBUGFN ( "Entering __sip_cloneSipDateStuct");
	if (src == SIP_NULL)
	{	
		*err = E_INV_PARAM;	
		return SipFail;
	}	
	if (dest == SIP_NULL)
	{	
		*err = E_INV_PARAM;	
		return SipFail;
	}	
	if(dest->pDate != SIP_NULL)
		sip_freeSipDateFormat(dest->pDate);
	if(dest->pTime != SIP_NULL)
		sip_freeSipTimeFormat(dest->pTime);
	dest->dDow = src->dDow;
	if(sip_initSipDateFormat( &(dest->pDate), err) == SipFail)
		return SipFail;
	if(__sip_cloneSipDateFormat( dest->pDate, src->pDate ,err)==SipFail)
	{
		sip_freeSipDateFormat(dest->pDate);
		return SipFail;
	}
	if(sip_initSipTimeFormat( &(dest->pTime), err) == SipFail)
	{
		sip_freeSipDateFormat(dest->pDate);
		return SipFail;
	}
	if(__sip_cloneSipTimeFormat( dest->pTime, src->pTime ,err)==SipFail)
	{
		sip_freeSipTimeFormat(dest->pTime);
		sip_freeSipDateFormat(dest->pDate);
		return SipFail;
	}
	SIPDEBUGFN ( "Exiting __sip_cloneSipDateStruct");
	return SipSuccess;
}
#endif

#ifdef SIP_TEL
/*********************************************************
** FUNCTION:__sip_cloneTelLocalNum
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  TelLocalNum structures "from" to "to".
**
**********************************************************/
SipBool __sip_cloneTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *to,
	TelLocalNum *from,
	SipError *pErr)
#else
	( to,from, pErr )
	TelLocalNum *to;
	TelLocalNum *from;
	SipError *pErr;
#endif
{
	SIP_U32bit dLength;
	SIPDEBUGFN( "Entering __sip_cloneTelLocalNum");

	if ((from==SIP_NULL)||(to==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	/* clean up of to */

	if(to->pLocalPhoneDigit  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pLocalPhoneDigit)),\
			pErr) == SipFail)
			return SipFail;
	}

	if(to->pIsdnSubAddr  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pIsdnSubAddr)), \
			pErr) == SipFail)
			return SipFail;
	}

	if(to-> pPostDial != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pPostDial)), pErr) \
			== SipFail)
			return SipFail;
	}

	if (sip_listDeleteAll(&(to->slParams), pErr) == SipFail)
		return SipFail;

	if (sip_listDeleteAll(&(to->slAreaSpecifier), pErr) == SipFail)
		return SipFail;


	/* clean up over */

	if(from->pLocalPhoneDigit== SIP_NULL)
	{
		to->pLocalPhoneDigit=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pLocalPhoneDigit );

		to->pLocalPhoneDigit=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,\
			dLength+1,pErr);
		if ( to->pLocalPhoneDigit == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pLocalPhoneDigit, from->pLocalPhoneDigit );
	}

	if(from->pIsdnSubAddr== SIP_NULL)
	{
		to->pIsdnSubAddr=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pIsdnSubAddr );

		to->pIsdnSubAddr=( SIP_S8bit * )fast_memget(\
			ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( to->pIsdnSubAddr == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pIsdnSubAddr, from->pIsdnSubAddr );
	}

	if(from->pPostDial== SIP_NULL)
	{
		to->pPostDial=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pPostDial );

		to->pPostDial=( SIP_S8bit * )fast_memget(\
			ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( to->pPostDial == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pPostDial, from->pPostDial );
	}

	if (__sipParser_cloneSipStringList(&(to->slAreaSpecifier), &(\
		from->slAreaSpecifier), pErr) == SipFail)
		return SipFail;

	if (__sipParser_cloneSipParamList (&(to->slParams), &(from->slParams), \
		pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN( "Exiting __sip_cloneTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*********************************************************
** FUNCTION:__sip_cloneTelGlobalNum
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  TelGlobalNum structures "from" to "to".
**
**********************************************************/
SipBool __sip_cloneTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *to,
	TelGlobalNum *from,
	SipError *pErr)
#else
	( to,from, pErr )
	TelGlobalNum *to;
	TelGlobalNum *from;
	SipError *pErr;
#endif
{
	SIP_U32bit dLength;
	SIPDEBUGFN( "Entering __sip_cloneTelGlobalNum");

	if ((from==SIP_NULL)||(to==SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	/* clean up of to */

	if(to->pBaseNo  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pBaseNo)), pErr) \
			== SipFail)
			return SipFail;
	}

	if(to->pIsdnSubAddr  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pIsdnSubAddr)), \
			pErr) == SipFail)
			return SipFail;
	}

	if(to-> pPostDial != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(to->pPostDial)), pErr) \
			== SipFail)
			return SipFail;
	}

	if (sip_listDeleteAll(&(to->slParams), pErr) == SipFail)
		return SipFail;

	if (sip_listDeleteAll(&(to->slAreaSpecifier), pErr) == SipFail)
		return SipFail;


	/* clean up over */

	if(from->pBaseNo== SIP_NULL)
	{
		to->pBaseNo=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pBaseNo );

		to->pBaseNo=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( to->pBaseNo == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pBaseNo, from->pBaseNo );
	}

	if(from->pIsdnSubAddr== SIP_NULL)
	{
		to->pIsdnSubAddr=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pIsdnSubAddr );

		to->pIsdnSubAddr=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,\
			pErr);
		if ( to->pIsdnSubAddr == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pIsdnSubAddr, from->pIsdnSubAddr );
	}

	if(from->pPostDial== SIP_NULL)
	{
		to->pPostDial=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pPostDial );

		to->pPostDial=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,\
			pErr);
		if ( to->pPostDial == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pPostDial, from->pPostDial );
	}

	if (__sipParser_cloneSipStringList(&(to->slAreaSpecifier), \
		&(from->slAreaSpecifier), pErr) == SipFail)
		return SipFail;

	if (__sipParser_cloneSipParamList (&(to->slParams), &(from->slParams), \
		pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN( "Exiting __sip_cloneTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*********************************************************
** FUNCTION:__sip_cloneTelUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  TelUrl structures "from" to "to".
** Parameters:
**	to (OUT)		- TelUrl
**	from (IN)		- TelUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**
**********************************************************/
SipBool __sip_cloneTelUrl
#ifdef ANSI_PROTO
	(TelUrl *to,
	TelUrl *from,
	SipError *pErr)
#else
	( to,from, pErr )
	TelUrl *to;
	TelUrl *from;
	SipError *pErr;
#endif
{

	SIPDEBUGFN("Entering function __sip_cloneTelUrl");
	if (from->pGlobal == SIP_NULL)
		to->pGlobal = SIP_NULL;
	else
	{
		if (sip_initTelGlobalNum(&(to->pGlobal),pErr) == SipFail)
			return SipFail;
		if (__sip_cloneTelGlobalNum(to->pGlobal,from->pGlobal,pErr) == SipFail)
			return SipFail;
	}
	if (from->pLocal == SIP_NULL)
		to->pLocal = SIP_NULL;
	else
	{
		if (sip_initTelLocalNum(&(to->pLocal),pErr) == SipFail)
			return SipFail;
		if (__sip_cloneTelLocalNum(to->pLocal,from->pLocal,pErr) == SipFail)
			return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function __sip_cloneTelUrl");
	return SipSuccess;
}

/*********************************************************
** FUNCTION:sip_cloneTelUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  TelUrl structures "from" to "to".
** Parameters:
**	to (OUT)		- TelUrl
**	from (IN)		- TelUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**********************************************************/

SipBool sip_cloneTelUrl
#ifdef ANSI_PROTO
	(TelUrl *to,
	TelUrl *from,
	SipError *pErr)
#else
	( to,from, pErr )
	TelUrl *to;
	TelUrl *from;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneTelUrl");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((to == SIP_NULL)||(from  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sip_cloneTelUrl(to,from, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneTelUrl");
	return SipSuccess;
}

#endif
#ifdef SIP_IMURL
/*********************************************************
** FUNCTION:sip_cloneImUrl
**
** DESCRIPTION:  This function makes a deep copy of the fileds from
**	the  ImUrl structures "from" to "to".
** Parameters:
**	pTo (OUT)		- ImUrl
**	pFrom (IN)		- ImUrl which has to be cloned
**	pErr (OUT)		- Possible Error value (see API ref doc)
**
**********************************************************/

SipBool sip_cloneImUrl
#ifdef ANSI_PROTO
	(ImUrl *pTo,
	ImUrl *pFrom,
	SipError *pErr)
#else
	( pTo,pFrom, pErr )
	ImUrl *pTo;
	ImUrl *pFrom;
	SipError *pErr;
#endif
{
	SIP_U32bit dLength=0;

	SIPDEBUGFN("Entering function sip_cloneImUrl");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pTo == SIP_NULL)||(pFrom  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pTo->pDispName  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(pTo->pDispName)), pErr) \
		== SipFail)
		return SipFail;
	}
	if(pTo->pUser  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(pTo->pUser)), pErr) \
		== SipFail)
		return SipFail;
	}
	if(pTo->pHost  != SIP_NULL )
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)(&(pTo->pHost)), pErr) \
		== SipFail)
		return SipFail;
	}
	if (sip_listDeleteAll(&(pTo->slParams), pErr) == SipFail)
		return SipFail;

	if (sip_listDeleteAll(&(pTo->slRoute), pErr) == SipFail)
		return SipFail;

	/* cleaning of to over */

	if(pFrom->pDispName== SIP_NULL)
	{
		pTo->pDispName=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(pFrom->pDispName );

		pTo->pDispName=(SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTo->pDispName == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( pTo->pDispName, pFrom->pDispName );
	}
	if(pFrom->pUser== SIP_NULL)
	{
		pTo->pUser=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(pFrom->pUser );

		pTo->pUser=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTo->pUser == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( pTo->pUser, pFrom->pUser );
	}
	if(pFrom->pHost== SIP_NULL)
	{
		pTo->pHost=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(pFrom->pHost );

		pTo->pHost=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTo->pHost == SIP_NULL )
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( pTo->pHost, pFrom->pHost);
	}
	if (__sip_cloneSipStringList(&(pTo->slRoute), \
		&(pFrom->slRoute), pErr) == SipFail)
		return SipFail;

	if (__sip_cloneSipParamList (&(pTo->slParams), &(pFrom->slParams), \
		pErr) == SipFail)
		return SipFail;
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneImUrl");
	return SipSuccess;
}

#endif


/******************************************************************
**
** FUNCTION:  __sipParser_cloneSipStringList
**
** DESCRIPTION:  This function makes a deep copy of a 
** SipList of SIP_S8bit *  from the "source" to "dest".
**
******************************************************************/
SipBool __sipParser_cloneSipStringList
#ifdef ANSI_PROTO
	(SipList *dest, SipList *source, SipError *err)
#else
	(dest, source, err)
	SipList *dest;
	SipList *source;
	SipError *err;
#endif
{
	SIP_S8bit *temp_string, *clone_string;
	SIP_Pvoid temp;
	SIP_U32bit count, dIndex;

	SIPDEBUGFN("Entering function sip_cloneStringList");
	if (sip_listDeleteAll(dest, err) == SipFail)
		return SipFail;
	if ( sip_listSizeOf (source, &count, err) == SipFail )
			return SipFail;

	for (dIndex = 0; dIndex < count; dIndex ++)
	{
		if (sip_listGetAt(source, dIndex, &temp, err) == SipFail)
			return SipFail;
		temp_string = (SIP_S8bit *)temp;
		if (temp_string == SIP_NULL)
			clone_string = SIP_NULL;
		else
		{
			clone_string = (SIP_S8bit *)sip_strdup(temp_string, \
				ACCESSOR_MEM_ID);
			if (clone_string == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}	
		}/* end of else */
		if (sip_listAppend(dest, (SIP_Pvoid)clone_string, err) == SipFail)
		{
			sip_freeString(clone_string);	
			return SipFail;
		}
	} /* end of for */

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneStringList");
	return SipSuccess;	
}

#ifdef SIP_CORE_STACK_COMPATIBLE
/*****************************************************************************
** FUNCTION:__sip_cloneSipNameValuePair
**
** DESCRIPTION:
**
**
******************************************************************************/
SipBool __sip_cloneSipNameValuePair
#ifdef ANSI_PROTO
        (SipNameValuePair * dest, SipNameValuePair *src, SipError *err)
#else
        ( dest,src,err )
                SipNameValuePair *dest;
                SipNameValuePair *src;
                SipError *err;

#endif
{
    SIP_U32bit dLength;
	SIP_U8bit mem_flag = 0;
	SIPDEBUGFN("Entering __sip_cloneSipNameValuePair");
	do
	{

        /* Copying pName */
        if (dest->pName !=SIP_NULL)
                if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pName), err)==SipFail)
                        return SipFail;
        if ( src->pName == SIP_NULL )
                dest->pName = SIP_NULL;
        else
        {
                dLength = sip_strlen(src->pName);
                dest->pName = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, err);
                if ( dest->pName == SIP_NULL )
                {
			{mem_flag = 1;break;}
                }
                (void)sip_strcpy(dest->pName, src->pName);
        }

	/* Copying pValue */
        if (dest->pValue !=SIP_NULL)
                if (sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *) &(dest->pValue), err)==SipFail)
                        return SipFail;
        if ( src->pValue == SIP_NULL )
                dest->pValue = SIP_NULL;
        else
        {
                dLength = sip_strlen(src->pValue);
                dest->pValue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength + 1, err);
                if ( dest->pValue == SIP_NULL )
                {
			{mem_flag = 1;break;}
                }
                (void)sip_strcpy(dest->pValue, src->pValue);
        }
	} while(0);
	if( mem_flag )
	{
		sip_freeSipNameValuePair( dest );
        	return SipFail;
	}
	SIPDEBUGFN("Exiting __sip_cloneSipNameValuePair");
	*err = E_NO_ERROR;
	return SipSuccess;

}


SipBool sip_cloneSipNameValuePair
#ifdef ANSI_PROTO
        (SipNameValuePair *pDest, SipNameValuePair *pSource, SipError *pErr)
#else
        (pDest, pSource, pErr)
        SipNameValuePair *pDest;
        SipNameValuePair *pSource;
        SipError *pErr;
#endif
{
        SIPDEBUGFN("Entering function sip_cloneSipNameValuePair");
#ifndef SIP_NO_CHECK
        if (pErr == SIP_NULL)
                return SipFail;
        if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
        {
                *pErr = E_INV_PARAM;
                return SipFail;
        }
#endif
        if  (__sip_cloneSipNameValuePair(pDest, pSource, pErr) == SipFail)
                return SipFail;

        *pErr = E_NO_ERROR;
        SIPDEBUGFN("Exiting function sip_cloneSipNameValuePair");
        return SipSuccess;
}
#endif /* SIP_CORE_STACK_COMPATIBLE */

#ifdef SIP_MWI
/*****************************************************************************
** FUNCTION:__sip_mwi_cloneMesgSummaryMessage
**
** DESCRIPTION:
**
**
******************************************************************************/
SipBool __sip_mwi_cloneMesgSummaryMessage
# ifdef ANSI_PROTO
        (MesgSummaryMessage * dest, MesgSummaryMessage *src, SipError *err)
# else
        ( dest,src,err )
                MesgSummaryMessage *dest;
                MesgSummaryMessage *src;
                SipError *err;

# endif
{
    SIP_U32bit size,dIndex;
	SIP_Pvoid element_from_src;
	SummaryLine *clone_summaryline;
	SipNameValuePair *clone_namevalue;
	SIPDEBUGFN("Entering __sip_mwi_cloneMesgSummaryMessage");

	if((src == SIP_NULL)||(dest==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
    /* copy status */

	dest->dStatus = src->dStatus;

    /* cloning Message-Account field */
    if(src->pAddrSpec != SIP_NULL)
    {
        
   	if (dest->pAddrSpec != SIP_NULL)
    {
        sip_freeSipAddrSpec(dest->pAddrSpec);
    }

	if (sip_initSipAddrSpec(&(dest->pAddrSpec), (src->pAddrSpec)->dType,\
		 err) == SipFail)
    {
		return SipFail;
    }

	if (__sip_cloneSipAddrSpec(dest->pAddrSpec, src->pAddrSpec, err)\
		 == SipFail)
	{
		sip_freeSipAddrSpec(dest->pAddrSpec);
		return SipFail;
	}
    }
    
    /* cloning slSummaryLine */

 	if ( sip_listDeleteAll(&(dest->slSummaryLine), err) == SipFail)
                return SipFail;
        if ( sip_listSizeOf(&(src->slSummaryLine), &size, err) == SipFail)
                return SipFail;
        for ( dIndex = 0; dIndex < size ; dIndex++ )
        {
			element_from_src = SIP_NULL;
                if( sip_listGetAt(&(src->slSummaryLine),dIndex, (SIP_Pvoid * )\
			 (&element_from_src), err) == SipFail )
                {
                        return SipFail;
                }
                if (element_from_src == SIP_NULL)
                        clone_summaryline = SIP_NULL;
                else
                {
                        if ( sip_mwi_initSummaryLine(&clone_summaryline, err) == SipFail)
                                return SipFail;
                        if ( __sip_mwi_cloneSummaryLine( clone_summaryline, \
				 (SummaryLine *)element_from_src, err) == SipFail)
                        {
                                sip_mwi_freeSummaryLine(clone_summaryline);
                                return SipFail;
                        }
                }
                if (sip_listAppend (&(dest->slSummaryLine), \
			 (SIP_Pvoid *)clone_summaryline, err) == SipFail)
                {
                        if (clone_summaryline != SIP_NULL)
                                sip_mwi_freeSummaryLine (clone_summaryline);
                        return SipFail;
                }

        }
/* cloning slNameValue */

	element_from_src = SIP_NULL;

	if ( sip_listDeleteAll(&(dest->slNameValue), err) == SipFail)
                return SipFail;
        if ( sip_listSizeOf(&(src->slNameValue), &size, err) == SipFail)
                return SipFail;
        for ( dIndex = 0; dIndex < size ; dIndex++ )
        {
			element_from_src = SIP_NULL;
                if( sip_listGetAt(&(src->slNameValue),dIndex, (SIP_Pvoid * )\
                         (&element_from_src), err) == SipFail )
                {
                        return SipFail;
                }
                if (element_from_src == SIP_NULL)
                        clone_namevalue = SIP_NULL;
                else
                {
                        if ( sip_initSipNameValuePair(&clone_namevalue, err) == SipFail)
                                return SipFail;
                        if ( __sip_cloneSipNameValuePair( clone_namevalue, \
                                 (SipNameValuePair *)element_from_src, err) == SipFail)
                        {
                                sip_freeSipNameValuePair(clone_namevalue);
                                return SipFail;
                        }
                }
                if (sip_listAppend (&(dest->slNameValue), \
                         (SIP_Pvoid *)clone_namevalue, err) == SipFail)
                {
                        if (clone_namevalue != SIP_NULL)
                                sip_freeSipNameValuePair(clone_namevalue);
                        return SipFail;
                }

        }
	SIPDEBUGFN("Exiting __sip_mwi_cloneMesgSummaryMessage");
	*err = E_NO_ERROR;
	return SipSuccess;

}
SipBool sip_mwi_cloneMesgSummaryMessage
#ifdef ANSI_PROTO
        (MesgSummaryMessage *pDest, MesgSummaryMessage *pSource, SipError *pErr)
#else
        (pDest, pSource, pErr)
        MesgSummaryMessage *pDest;
        MesgSummaryMessage *pSource;
        SipError *pErr;
#endif
{
        SIPDEBUGFN("Entering function sip_mwi_cloneMesgSummaryMessage");
#ifndef SIP_NO_CHECK
        if (pErr == SIP_NULL)
                return SipFail;
        if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
        {
                *pErr = E_INV_PARAM;
                return SipFail;
        }
#endif
        if  (__sip_mwi_cloneMesgSummaryMessage(pDest, pSource, pErr) == SipFail)
                return SipFail;

        *pErr = E_NO_ERROR;
        SIPDEBUGFN("Exiting function sip_mwi_cloneMesgSummaryMessage");
        return SipSuccess;
}

/*****************************************************************************
** FUNCTION:__sip_mwi_cloneSummaryLine
**
** DESCRIPTION:
**
**
******************************************************************************/
SipBool __sip_mwi_cloneSummaryLine
# ifdef ANSI_PROTO
        (SummaryLine * dest, SummaryLine *src, SipError *err)
# else
        ( dest,src,err )
                SummaryLine *dest;
                SummaryLine *src;
                SipError *err;

# endif
{
	SIP_U32bit dLength;
	SIPDEBUGFN("Entering __sip_mwi_cloneSummaryLine");

	/* Copying pMedia */
        if (dest->pMedia !=SIP_NULL)
                if (sip_memfree(ACCESSOR_MEM_ID,  \
                            (SIP_Pvoid *) &(dest->pMedia), err)==SipFail)
                        return SipFail;
        if ( src->pMedia == SIP_NULL )
                dest->pMedia = SIP_NULL;
        else
        {
                dLength = strlen(src->pMedia);
                dest->pMedia = ( SIP_S8bit * )fast_memget \
                               (ACCESSOR_MEM_ID,dLength + 1, err);
                if ( dest->pMedia == SIP_NULL )
                {
                        return SipFail;
                }
                strcpy(dest->pMedia, src->pMedia);
        }

	/* copy other values */
	dest->newMessages = src->newMessages;
	dest->oldMessages = src->oldMessages;
	dest->newUrgentMessages = src->newUrgentMessages;
	dest->oldUrgentMessages = src->oldUrgentMessages;

	SIPDEBUGFN("Exiting __sip_mwi_cloneSummaryLine");
	*err = E_NO_ERROR;
	return SipSuccess;

}

SipBool sip_mwi_cloneSummaryLine
#ifdef ANSI_PROTO
        (SummaryLine *pDest, SummaryLine *pSource, SipError *pErr)
#else
        (pDest, pSource, pErr)
        SummaryLine *pDest;
        SummaryLine *pSource;
        SipError *pErr;
#endif
{
        SIPDEBUGFN("Entering function sip_mwi_cloneSummaryLine");
#ifndef SIP_NO_CHECK
        if (pErr == SIP_NULL)
                return SipFail;
        if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
        {
                *pErr = E_INV_PARAM;
                return SipFail;
        }
#endif
        if  (__sip_mwi_cloneSummaryLine(pDest, pSource, pErr) == SipFail)
                return SipFail;

        *pErr = E_NO_ERROR;
        SIPDEBUGFN("Exiting function sip_mwi_cloneSummaryLine");
        return SipSuccess;
}
#endif
#ifdef SIP_DATE
/*******************************************************************************
** FUNCTION:__sip_cloneSipDateFormat
** DESCRIPTION:makes a deep copy of the source SipDateFormat to the destination 
** SipDateFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipDateFormat 
**			pSource(IN)		- Source SipDateFormat 
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipDateFormat
#ifdef ANSI_PROTO
	(SipDateFormat * dest, SipDateFormat *src, SipError * err)
#else
	( dest,src , err)
	  SipDateFormat *dest;
	  SipDateFormat *src;
	  SipError	*err;
#endif
{
	SipError *dummy=err;
	dummy=err;
    (void)dummy;
	SIPDEBUGFN ( "Entering __sip_cloneSipDateFormat");
	dest->dDay = src->dDay;
	dest->dMonth = src->dMonth;
	dest->dYear = src->dYear;
	SIPDEBUGFN ( "Exiting __sip_cloneSipDateFormat");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION:__sip_cloneSipTimeFormat 
** DESCRIPTION:makes a deep copy of the source SipTimeFormat to the destination 
** SipTimeFormat
** PARAMETERS:
**			pDest(OUT)		- Destination SipTimeFormat 
**			pSource(IN)		- Source SipTimeFormat
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/
SipBool __sip_cloneSipTimeFormat
#ifdef ANSI_PROTO
	(SipTimeFormat * dest, SipTimeFormat *src, SipError * err)
#else
	( dest,src ,err)
	  SipTimeFormat *dest;
	  SipTimeFormat *src;
	  SipError	*err;
#endif
{
	SipError *dummy=err;
	dummy=err;
    (void)dummy;
	SIPDEBUGFN ( "Entering __sip_cloneSipTimeFormat");
	dest->dHour = src->dHour;
	dest->dMin = src->dMin;
	dest->dSec = src->dSec;
	SIPDEBUGFN ( "Exiting __sip_cloneSipTimeFormat");
	return SipSuccess;
}
#endif /* SIP_DATE */

#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_3GPP

/*******************************************************************************
** FUNCTION:__sipParser_cloneMsrpUrl
** DESCRIPTION:makes a deep copy of the source MsrpUrl to the destination 
** 
** PARAMETERS:
**			pDest(OUT)		- Destination MsrpUrl 
**			pSource(IN)		- Source MsrpUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/

SipBool __sipParser_cloneMsrpUrl
#ifdef ANSI_PROTO
	(MsrpUrl *to,
	MsrpUrl *from,
	SipError *err)
#else
	( to,from, err )
	MsrpUrl *to;
	MsrpUrl *from;
	SipError *err;
#endif
{
	SIP_U32bit dLength;

	SIPDEBUGFN( "Entering sip_cloneMsrpUrl");
	
	if ((from==SIP_NULL)||(to==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if(to->pUser!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pUser),err)==SipFail)
			return SipFail;
	if(to->pHost!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pHost),err)==SipFail)
			return SipFail;
	if(to->dPort!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->dPort),err)==SipFail)
			return SipFail;
	if(to->pResource!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pResource),err)==SipFail)
			return SipFail;
	/*
	 * Copying User
	 */
	if(from->pUser== SIP_NULL)
	{
		to->pUser=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pUser );

		to->pUser=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1, err);
		if ( to->pUser == SIP_NULL )
		{
			*err = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pUser, from->pUser );
	}
	
	/*
	 * Copying Host
	 */

	if (from->pHost== SIP_NULL)
	{
		to->pHost=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pHost );

		to->pHost=( SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID, dLength+1, err);
		if ( to->pHost == SIP_NULL )
		{
			*err = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy( to->pHost, from->pHost );
	}
	/*
	 * Copying port
	 */
	if (from->dPort== SIP_NULL)
	{
		to->dPort=SIP_NULL;
	}
	else
	{
		dLength = sizeof(SIP_U16bit);

		to->dPort=from->dPort;
	}

	/*
	 * Copying port
	 */
	if (from->pResource== SIP_NULL)
	{
		to->pResource=SIP_NULL;
	}
	else
	{
		dLength = sizeof(SIP_U16bit);

		to->pResource=( SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID, dLength, err);
		if ( to->pResource == SIP_NULL )
		{
			*err = E_NO_MEM;
			return SipFail;
		}
		*to->pResource=*from->pResource;
	}

	SIPDEBUGFN( "Exiting sip_cloneMsrpUrl");	
	*err=E_NO_ERROR;
	return SipSuccess;

}



/*******************************************************************************
** FUNCTION: sip_cloneMsrpUrl
** DESCRIPTION:makes a deep copy of the source MsrpUrl to the destination 
** 
** PARAMETERS:
**			pDest(OUT)		- Destination MsrpUrl 
**			pSource(IN)		- Source MsrpUrl
**			pErr(OUT)		- possible error value (see API ref doc)
**
** Return Values:
**			SipFail
**			SipSuccess
*******************************************************************************/


SipBool sip_cloneMsrpUrl
#ifdef ANSI_PROTO
	(MsrpUrl *pDest, MsrpUrl *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	MsrpUrl *pDest;
	MsrpUrl *pSource;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_cloneMsrpUrl");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pDest == SIP_NULL)||(pSource == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if  (__sipParser_cloneMsrpUrl
(pDest, pSource, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exiting function sip_cloneMsrpUrl");
	return SipSuccess;
}

#endif
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

