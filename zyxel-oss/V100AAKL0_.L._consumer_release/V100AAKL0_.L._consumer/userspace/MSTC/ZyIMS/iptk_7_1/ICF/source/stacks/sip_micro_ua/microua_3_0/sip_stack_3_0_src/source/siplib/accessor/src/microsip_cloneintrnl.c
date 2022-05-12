/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ** FUNCTION:
 **             Some Clone Functions
 **
 *********************************************************************
 **
 ** FILENAME:
 ** microsip_clone.c
 **
 ** DESCRIPTION:
 ** This file contains some essential clone functions. 
 **
 ** DATE        NAME                    REFERENCE       REASON
 ** ----        ----                    ---------       --------
 ** 15 02 2002  Mahesh Govind	           -		 Initial Creation
 **
 ** COPYRIGHT , Aricent, 2006
 *********************************************************************/ 

#include "microsip_common.h"
#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_decode.h"
#include "microsip_cloneintrnl.h"


/*******************************************************************************
** FUNCTION: __sipParser_cloneSipParam
** DESCRIPTION: This function copies the contents of a SIP param
**		structure into another SIP param structure
** 
** PARAMETERS:
**      	      pDest(OUT)			:The destination sipparam
**				  pSource(IN)			:the Sipparam to be cloned
**				  pError(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool __sipParser_cloneSipParam
#ifdef ANSI_PROTO
	(SipParam *pDest, SipParam *pSource, SipError *pErr)
#else
	(pDest, pSource, pErr)
	SipParam *pDest;
	SipParam *pSource;
	SipError *pErr;
#endif
{	
	SIP_U32bit index_1, count;
	SIP_S8bit *pValue, *pDupValue;
	SipError	tempErr;

	SIPDEBUGFN("Entering function sip_cloneSipParam");
	/* duplicating siplist of slParams */

	/* deleting siplist of slParams */
	if ( sip_listDeleteAll(&( pDest->slValue ), pErr ) == SipFail )
		return SipFail;

	/* copying */
	if ( sip_listInit(& (pDest->slValue),(pSource->slValue).freefunc,pErr)==\
		SipFail)
		return SipFail;
	if ( sip_listSizeOf(&( pSource->slValue), &count, pErr) == SipFail )
		return SipFail;

	for ( index_1 = 0; index_1 < count ; index_1++)
	{
		if( sip_listGetAt(&( pSource->slValue),index_1, (SIP_Pvoid * ) (&pValue),\
			pErr) == SipFail )
			return SipFail;

		if ( pValue == SIP_NULL )
			pDupValue = SIP_NULL;
		else
		{
			pDupValue = (SIP_S8bit *)STRDUP(pValue);
			if ( pDupValue == SIP_NULL)
			{
				*pErr = E_NO_MEM;
				return SipFail;
			}
		}

		if ( sip_listAppend(&( pDest->slValue ), pDupValue, pErr) == SipFail )
		{
			if ( pDupValue != SIP_NULL )
				(void)sip_memfree(0,(SIP_Pvoid*)&(pDupValue), &tempErr);
			return SipFail;
		}
	}

	/* duplicating Name */

	/* freeing pName  in destination if something is present in destination.*/
	if ( pDest->pName != SIP_NULL)
	{
		(void)sip_memfree(0, (SIP_Pvoid *)(&pDest->pName), pErr);
		pDest->pName = SIP_NULL;
	}

	if (pSource->pName == SIP_NULL)
		pDest->pName = SIP_NULL;
	else
	{
		pDest->pName = (SIP_S8bit*)STRDUP(pSource->pName);
		if ( pDest->pName == SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_cloneSipParam");
	return SipSuccess;	
}


/*******************************************************************************
** FUNCTION: __validateSipAddrSpecType
** DESCRIPTION: This function returns SipSuccess if "Type" is 
** among the defined en_AddType's ; else it returns SipFail;
** 
** PARAMETERS:
**      	      dType(IN)				:The adderspec to be validated
**				  pError(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool __validateSipAddrSpecType
#ifdef ANSI_PROTO
	(en_AddrType dType, SipError *err)
#else
	(dType, err)
	en_AddrType dType;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function validateSipAddrSpecType");
	switch (dType)
	{
		case	SipAddrSipUri	:
		case	SipAddrSipSUri	:
		case	SipAddrReqUri	:
		case	SipAddrAny	:
			/* 
			* if any of the above types, then break and return success
			*/
			break;
		default			:
			*err = E_INV_TYPE;
			 return SipFail; 
	}
	
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipAddrSpecType");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_cloneSipUrl
** DESCRIPTION:  This function makes a deep copy of the 
** fileds from the Url structures "source" to "dest".
** 
** PARAMETERS:
**      	      to(OUT)			:The destination sipurl
**				  from(IN)			:The source sipusrl
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool __sipParser_cloneSipUrl
#ifdef ANSI_PROTO
	(SipUrl *to,
	SipUrl *from,
	SipError *err)
#else
	( to,from, err )
	SipUrl *to;
	SipUrl *from;
	SipError *err;
#endif
{
	SIP_U32bit dLength;
	SipError   local_err;

	SIPDEBUGFN( "Entering sip_cloneSipUrl");
	
	if ((from==SIP_NULL)||(to==SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if(to->pUser!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pUser),err)==SipFail)
			return SipFail;
	if(to->pPassword!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pPassword),err)==SipFail)
			return SipFail;
	if(to->pHost!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pHost),err)==SipFail)
			return SipFail;
	if(to->pHeader!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->pHeader),err)==SipFail)
			return SipFail;
	if(to->dPort!=SIP_NULL)
		if(sip_memfree(0,(SIP_Pvoid*)&(to->dPort),err)==SipFail)
			return SipFail;
	if(sip_listDeleteAll(&(to->slParam),err)==SipFail)
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
			goto error;
		}
		(void)sip_strcpy( to->pUser, from->pUser );
	}
	/*
	 * Copying password
	 */
	if(from->pPassword== SIP_NULL)
	{
		to->pPassword=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pPassword );

		to->pPassword=( SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID, dLength+1, err);
		if ( to->pPassword == SIP_NULL )
		{
			goto error;
		}
		(void)sip_strcpy( to->pPassword, from->pPassword );
	}
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
			goto error;
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

		to->dPort=( SIP_U16bit *)fast_memget(ACCESSOR_MEM_ID, dLength, err);
		if ( to->dPort == SIP_NULL )
		{
			goto error;
		}
		*to->dPort=*from->dPort;
	}

	/* copying siplist of SipParam */
	if(__sipParser_cloneSipParamList(&(to->slParam),&(from->slParam),err)==\
		SipFail)
		goto error;
	/*
	 * Copying header
	 */

	if (from->pHeader== SIP_NULL)
	{
		to->pHeader=SIP_NULL;
	}
	else
	{
		dLength = sip_strlen(from->pHeader );

		to->pHeader=( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1, err);
		if (to->pHeader == SIP_NULL)
		{
			goto error;
		}
		(void)sip_strcpy( to->pHeader, from->pHeader );
	}
	SIPDEBUGFN( "Exitting sip_cloneSipUrl");	
	
	*err=E_NO_ERROR;
	return SipSuccess;

error: 
	if(to->pUser != SIP_NULL)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID,(void **)&(to->pUser),&local_err);
	}

	if(to->pPassword != SIP_NULL)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID,(void **)&(to->pPassword),&local_err);
	}

	if(to->pHost != SIP_NULL)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID,(void **)&(to->pHost),&local_err);
	}

	if(to->dPort != SIP_NULL)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID,(void **)&(to->dPort),&local_err);
	}
	
	if(to->pHeader != SIP_NULL)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID,(void **)&(to->pHeader),&local_err);
	}

	return SipFail;

}

/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipAddrSpec
** DESCRIPTION:   This function makes a deep copy of the fileds 
** from the AddrSpec structures "source" to "dest".
** 
** PARAMETERS:
**      	      dest(OUT)			:The destination SipAddrSpec
**				  source(IN)		:The source SipAddrSpec
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool __sipParser_cloneSipAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *dest, SipAddrSpec *source, SipError *err)
#else
	(dest, source, err)
	SipAddrSpec *dest;
	SipAddrSpec *source;
	SipError *err;
#endif
{
	SIP_S8bit *temp;
	SIPDEBUGFN("Entering function sip_cloneSipAddrSpec");
	/*
	 * Validating the source  addrspec type
	 */
	if (__validateSipAddrSpecType(source->dType, err) == SipFail)
		return SipFail;
	/*
	 * Cleaning up destination 
	 */
	switch (dest->dType)
	{
		case	SipAddrReqUri:
					if ((dest->u).pUri != SIP_NULL)
						sip_freeString((dest->u).pUri);
					 break;
		case	SipAddrSipUri:
		case	SipAddrSipSUri:
					if ((dest->u).pSipUrl != SIP_NULL)
				 		sip_freeSipUrl((dest->u).pSipUrl);
					 break;
		case	SipAddrAny:
					break;
		default	:
					*err = E_INV_TYPE;
					 return SipFail;
	}

	/*
	 * Now copying dType
	 */
	dest->dType = source->dType;
	
	/*
	 * Now copy rest of the members according to dType
	 */
	switch (source->dType)
	{
		case	SipAddrReqUri	:
					if ((source->u).pUri == SIP_NULL)
						temp = SIP_NULL;
					 else
					 {
						temp = (SIP_S8bit *)STRDUP((source->u).pUri);
						if (temp == SIP_NULL)
						{
							*err = E_NO_MEM;
							return SipFail;
						}
					 }
					 (dest->u).pUri = temp;
					 break;

		case	SipAddrSipUri	:
		case	SipAddrSipSUri:
					if ((source->u).pSipUrl == SIP_NULL)
						(dest->u).pSipUrl = SIP_NULL;
					 else
					 {
						if (sip_initSipUrl(&((dest->u).pSipUrl), err) == SipFail)
							return SipFail;
						if (__sipParser_cloneSipUrl((dest->u).pSipUrl, \
							(source->u).pSipUrl, err) == SipFail)
						{
							sip_freeSipUrl((dest->u).pSipUrl);
							return SipFail;
						}
					 }
					 break;

		case	SipAddrAny	:*err = E_INV_PARAM;
					 return SipFail;

		default			:*err = E_INV_TYPE;
					 return SipFail;

	}
	
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_cloneSipAddrSpec");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipParamList
** DESCRIPTION:  This function makes a deep copy of a 
** SipList of SipParam from the "source" to "dest".
**
** PARAMETERS:
**      	      dest(OUT)			:The destination SipList
**				  source(IN)		:The source SipList
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool __sipParser_cloneSipParamList
#ifdef ANSI_PROTO
	(SipList *dest, SipList *source, SipError *err)
#else
	(dest, source, err)
	SipList *dest;
	SipList *source;
	SipError *err;
#endif
{
	SipParam *temp_param, *clone_param;
	SIP_U32bit count, index_1;

	SIPDEBUGFN("Entering function sip_cloneSipParamList");
	/* copying siplist of SipParam */
	if ( sip_listDeleteAll(dest , err ) == SipFail )
		return SipFail;
	
	/* Getting the size of the  siplist */
	if ( sip_listSizeOf(source, &count, err) == SipFail )
	{
		return SipFail;
	}

	/* Now Cloning each element */
	for ( index_1 = 0; index_1 < count ; index_1++)
	{
		if( sip_listGetAt(source,index_1, (SIP_Pvoid * ) (&temp_param), err) == \
			SipFail )
		{
			return SipFail;
		}
		if ( temp_param == SIP_NULL )
			clone_param = SIP_NULL;
		else
		{
			if(sip_initSipParam(&clone_param,err)==SipFail)
			{
				return SipFail;
			}
			
			if ( clone_param == SIP_NULL )
				return SipFail;

			if ( __sipParser_cloneSipParam(  clone_param, temp_param,err) ==\
				SipFail )
			{
				(void)sip_memfree(0, (SIP_Pvoid*)&(clone_param),err);
				return SipFail;
			}
		}
		
		/* Appending each of the  nodes to the destination */
		if ( sip_listAppend(dest, clone_param, err) == SipFail )
		{
			if ( clone_param != SIP_NULL )
				(void)sip_memfree(0,(SIP_Pvoid*)&(clone_param), err);
			return SipFail;
		}
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_cloneSipParamList");
	return SipSuccess;	
}


/*******************************************************************************
** FUNCTION:  __sipParser_cloneSipCommonHeader
**
** DESCRIPTION: This function makes a deep copy of the fileds 
** from the CommonHeader structures "source" to "dest".
**
** PARAMETERS:
**      	      dest(OUT)			:The destination SipCommonHeader
**				  source(IN)		:The source SipCommonHeader
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool __sipParser_cloneSipCommonHeader
	(SipCommonHeader *pDest, SipCommonHeader *pSource, SipError *pErr)
{
	SIP_S8bit *pTempStr = SIP_NULL;
	SIPDEBUGFN("Entering function __sipParser_cloneSipCommonHeader");

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
        if (__sipParser_cloneSipAddrSpec(pDest->pAddrSpec, pSource->pAddrSpec, pErr) 
                == SipFail)
        {
            sip_freeSipAddrSpec(pDest->pAddrSpec);
            return SipFail;
        }
    }
	if(__sipParser_cloneSipParamList(&(pDest->slParam),&(pSource->slParam),pErr)
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
	SIPDEBUGFN("Exitting function __sipParser_cloneSipCommonHeader");
	return SipSuccess;
}

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

