/***********************************************************************
** FUNCTION:
**			Header Accessor API functionality
************************************************************************
**
** FILENAME:
** microsip_header.c
**
** DESCRIPTION:
**		This file contains the accessor functions for the various SIP
**		headers supported by the microSIP stack.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 9/09/2002	Aparna Kuppachi		-			Initial					
** 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
**
**
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_init.h"
#include "microsip_clone.h"
#include "microsip_accessors.h"
#include "microsip_header.h"
#include "microsip_subparsers.h"
#include "microsip_encodeinternal.h"
#include "microsip_decodeinternal.h"
#include "microsip_hdrencoders.h"
#include "microsip_subapi.h"
#include "microsip_decode.h"
#include "microsip_encode.h"
#include "microsip_msgbodyparser.h"

/****************************************************************************
** FUNCTION: sip_getdIntVar1FromCommonHdr
** DESCRIPTION: This function gets the first U32bit value of common header 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pValue(OUT)	    - The retrieved U32bit value
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getdIntVar1FromCommonHdr
	( SipHeader     *pHdr,      
	  SIP_U32bit    *pValue,
	  SipError      *pErr)
{
	SIPDEBUGFN ( "Entering sip_getdIntVar1FromCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pValue == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif	
	*pValue = ((SipCommonHeader *) (pHdr->pHeader))->dIntVar1; 
	
	SIPDEBUGFN ( "Exiting sip_getdIntVar1FromCommonHdr");
	
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setdIntVar1InCommonHdr
** DESCRIPTION: This function sets the first U32bit Value in Common Header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Content-Length SipHeader
**				dIntVar(IN)	    - The length field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setdIntVar1InCommonHdr
	( SipHeader  *pHdr,    /* Content Length pHeader */
	  SIP_U32bit value, 
	  SipError   *pErr)
{
	SIPDEBUGFN ( "Entering sip_setdIntVar1InCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif	
	((SipCommonHeader *)(pHdr->pHeader))->dIntVar1 = value;

	SIPDEBUGFN ( "Exiting sip_setdIntVar1InCommonHdr");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_getStr1FromCommonHdr
** DESCRIPTION: This function gets the first string type field from Common Header. 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pStr(OUT)       - The retrieved string field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getStr1FromCommonHdr
	( SipHeader *pHdr,	
      SIP_S8bit **ppStr,
	  SipError  *pErr)
{

	SIPDEBUGFN ( "Entering sip_getStr1FromCommonHdr"); 
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (ppStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif	
	*ppStr = ((SipCommonHeader *) (pHdr->pHeader))->pStr1; 
	
	if (*ppStr == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
	SIPDEBUGFN ( "Exiting sip_getStr1FromCommonHdr");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setStr1InCommonHdr
** DESCRIPTION: This function sets the first string type field in Common Header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pStr(IN)		- The string to be set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setStr1InCommonHdr
	( SipHeader	*pHdr,	
	  SIP_S8bit *pStr,
	  SipError 	*pErr)
{
	SipCommonHeader		*pCommonHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_setStr1InCommonHdr"); 
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	
	pCommonHdr = (SipCommonHeader *) (pHdr->pHeader);

	if (pCommonHdr->pStr1 != SIP_NULL)
	{
		if (fast_memfree(ACCESSOR_MEM_ID, pCommonHdr->pStr1, \
			pErr)== SipFail)
			return SipFail;
	}

	pCommonHdr->pStr1=pStr;
	SIPDEBUGFN ( "Exiting sip_setStr1InCommonHdr"); 

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_getParamCountFromCommonHdr
** DESCRIPTION: This function gets the number of parameters present 
**				in the Common Hdr. 
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pLength(OUT)	- The retrieved count
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getParamCountFromCommonHdr
	( SipHeader     *pHdr,
	  SIP_U32bit    *pCount,
	  SipError      *pErr  )
{
	SipCommonHeader *pCommonHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_getParamCountFromCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pCount == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	pCommonHdr = (SipCommonHeader*) (pHdr->pHeader);

	if (sip_listSizeOf(&(pCommonHdr->slParam), pCount , pErr) == SipFail)
	{
		return SipFail;
	}
	
	SIPDEBUGFN ("Exitting sip_getParamCountFromCommonHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;	
}

/****************************************************************************
** FUNCTION: sip_getParamAtIndexFromCommonHdr
** DESCRIPTION: This function gets the Param at a specified index in the CommonHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				ppParam(OUT)	- The retrieved Param 
**				dIndex(IN)		- The index to retrieve from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getParamAtIndexFromCommonHdr
	( SipHeader	*pHdr, SipParam **ppParam, SIP_U32bit dIndex, SipError *pErr)
{
	SIP_Pvoid 	    pElementFromList = SIP_NULL;
	SipCommonHeader *pCommonHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_getParamAtIndexFromCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (ppParam == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	pCommonHdr = (SipCommonHeader*)(pHdr->pHeader);

	if (sip_listGetAt(&(pCommonHdr->slParam), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;
	
	if (pElementFromList == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF(((SipParam *)(pElementFromList))->dRefCount);
	*ppParam=(SipParam *)pElementFromList;

	SIPDEBUGFN("Exitting sip_getParamAtIndexFromCommonHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_setParamAtIndexInCommonHdr
** DESCRIPTION: This function sets the SipParam structure at a specified index
**				in the CommonHdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pParam(In)		- The SipParam to be set 
**				dIndex(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setParamAtIndexInCommonHdr
	( SipHeader     *pHdr, 
	  SipParam	    *pParam,
	  SIP_U32bit    dIndex,
	  SipError 	    *pErr )
{

	SIPDEBUGFN ( "Entering sip_setParamAtIndexInCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ((pHdr == SIP_NULL)||(pParam == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	HSS_LOCKEDINCREF(pParam->dRefCount);
	if (sip_listSetAt(&(((SipCommonHeader*)(pHdr->pHeader))->slParam),\
		 dIndex, (SIP_Pvoid) pParam, pErr) == SipFail)
	{
		if (pParam != SIP_NULL)
        {	  
            HSS_LOCKEDDECREF(pParam->dRefCount);
            sip_freeSipParam(pParam);
        }
		return SipFail;
	}
	SIPDEBUGFN ( "Exitting sip_setParamAtIndexInCommonHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_insertParamAtIndexInCommonHdr
** DESCRIPTION: This function inserts the SipParam structure at a specified 
**				index in the Common Hdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pParam(IN)		- The SipParam to be retrieved
**				dIndex(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_insertParamAtIndexInCommonHdr
	( SipHeader     *pHdr,
	  SipParam	    *pParam,
	  SIP_U32bit    dIndex,
	  SipError 	    *pErr )
{
	SipCommonHeader *pCommonHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_insertParamAtIndexInCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if ((pHdr->pHeader == SIP_NULL)||(pParam == SIP_NULL))
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	
	pCommonHdr = (SipCommonHeader *) (pHdr->pHeader);
	HSS_LOCKEDINCREF(pParam->dRefCount);
	if (sip_listInsertAt(&(pCommonHdr->slParam), dIndex, pParam, \
		pErr) == SipFail)
	{
		if (pParam != SIP_NULL)
        { 	
            HSS_LOCKEDDECREF(pParam->dRefCount);
            sip_freeSipParam(pParam);
        }
        return SipFail;
	}

	SIPDEBUGFN ("Exitting sip_insertParamAtIndexInCommonHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_deleteParamAtIndexInCommonHdr 
** DESCRIPTION: This function deletes a SipParam structure at a specified index 
**				in the Common Hdr.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				index(In)		- The index from which to delete
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_deleteParamAtIndexInCommonHdr
	( SipHeader     *pHdr,
	  SIP_U32bit    dIndex,
	  SipError      *pErr )
{
	SipCommonHeader *pCommonHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_deleteParamAtIndexInCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	pCommonHdr = (SipCommonHeader *)(pHdr->pHeader);
	
	if (sip_listDeleteAt(&(pCommonHdr->slParam), dIndex, pErr) == SipFail)
		return SipFail;
	
	SIPDEBUGFN ( "Exitting sip_deleteParamAtIndexInCommonHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_getAddrSpecFromCommonHdr
** DESCRIPTION: This function retrieves the Addrspec field from the
**				SIP Common header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pLength(OUT)	- The retrieved addr-spec
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getAddrSpecFromCommonHdr
	(SipHeader   *pHdr, 
     SipAddrSpec **ppAddrSpec, 
     SipError    *pErr)
{
 	SipAddrSpec *pTempAddrSpec = SIP_NULL;
	SIPDEBUGFN("Entering function sip_getAddrSpecFromCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (ppAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

 	pTempAddrSpec = ((SipCommonHeader *) (pHdr ->pHeader))->pAddrSpec;
 	if (pTempAddrSpec == SIP_NULL)
 	{
 		*pErr = E_NO_EXIST;
 		return SipFail;
 	}
	HSS_LOCKEDINCREF(pTempAddrSpec->dRefCount);
	*ppAddrSpec = pTempAddrSpec;
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getAddrSpecFromCommonHdr");
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setAddrSpecInCommonHdr
** DESCRIPTION: This function sets the Addr-spec field in SIP
**				Common header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pLength(OUT)	- The addr-spec to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setAddrSpecInCommonHdr
	(SipHeader   *pHdr, 
     SipAddrSpec *pAddrSpec, 
     SipError    *pErr)
{
	SIPDEBUGFN("Entering function sip_setAddrSpecInCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
 	if (pAddrSpec == SIP_NULL)
	{
		sip_freeSipAddrSpec(((SipCommonHeader *)(pHdr ->pHeader))->pAddrSpec);
 		((SipCommonHeader *) (pHdr ->pHeader))->pAddrSpec = SIP_NULL;
	}
	else
	{
		HSS_LOCKEDINCREF(pAddrSpec->dRefCount);
		sip_freeSipAddrSpec(((SipCommonHeader *)(pHdr ->pHeader))->pAddrSpec);
		((SipCommonHeader *)(pHdr->pHeader))->pAddrSpec = pAddrSpec;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setAddrSpecInCommonHdr");
	return SipSuccess;
}



/****************************************************************************
** FUNCTION: sip_getStr2FromCommonHdr
** DESCRIPTION: This function retrieves the second string field from a
**				Common header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				ppValue(OUT)	- The retrieved string field
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getStr2FromCommonHdr
	(SipHeader  *pHdr, 
     SIP_S8bit  **ppValue, 
     SipError   *pErr)
{
	SIPDEBUGFN("Entering function sip_getStr2FromCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (ppValue == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
 	*ppValue = ((SipCommonHeader *)(pHdr ->pHeader))->pStr2;
 	if (*ppValue == SIP_NULL)
 	{
 		*pErr = E_NO_EXIST;
 		return SipFail;
 	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getStr2FromCommonHdr");
 	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setStr2InCommonHdr
** DESCRIPTION: This function sets the second string field in the Common header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pValue(IN)	    - The string value field to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setStr2InCommonHdr
	(SipHeader *pHdr, 
     SIP_S8bit *pValue, 
     SipError  *pErr)
{

    SIP_S8bit *pTempValue;
	
	SIPDEBUGFN("Entering function sip_setStr2InCommonHdr");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	pTempValue = ((SipCommonHeader *)(pHdr->pHeader))->pStr2;
	if (pTempValue != SIP_NULL)
	{
		if (sip_memfree(ACCESSOR_MEM_ID, (void**)(&pTempValue), pErr) == SipFail)
		{
			return SipFail;
		}
	}
	((SipCommonHeader *)(pHdr->pHeader))->pStr2 = pValue;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setStr2InCommonHdr");
	return SipSuccess;
}
#ifdef SIP_CORE_STACK_COMPATIBLE
/****************************************************************************
** FUNCTION: sip_getNameValuePairAtIndexFromCommonHdr
** DESCRIPTION: This function retrieves NameValuePair at specified index
**				from Common Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				ppNameValue(IN)	- The retreived name-value pair
**				dIndex(IN		- The index to retrieve from
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getNameValuePairAtIndexFromCommonHdr
	( SipHeader         *pHdr, 
      SipNameValuePair  **ppNameValue,
	  SIP_U32bit        dIndex, 
      SipError          *pErr )
{
	SipParam	*pTempParam = SIP_NULL;
	SipNameValuePair *pNameValuePair = SIP_NULL;
	SIP_S8bit	*pValue = SIP_NULL;

	SIPDEBUGFN("Entering sip_getNameValuePairAtIndexFromCommonHdr\n");

#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
	if (ppNameValue == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (SipFail == sip_getParamAtIndexFromCommonHdr(pHdr, &pTempParam, 
		dIndex, pErr))
		return SipFail;

	if (SipFail == sip_initSipNameValuePair(&pNameValuePair, pErr))
    {
        sip_freeSipParam(pTempParam);
		return SipFail;
    }

	pNameValuePair->pName = STRDUPACCESSOR(pTempParam->pName);
	if (SIP_NULL == pNameValuePair->pName)
	{
		*pErr = E_NO_MEM;
        sip_freeSipParam(pTempParam);
		return SipFail;
	}

	if (sip_listGetAt(&(pTempParam->slValue),
		0, (void **) &pValue, pErr) == SipFail)
	{
		if (E_NO_EXIST == *pErr)
		{
			/*
			 * This is not an error; it just means that 
			 * this param is of type name only.
			 */
			pValue = SIP_NULL;
		}
		else
        {
            sip_freeSipParam(pTempParam);
			return SipFail;
        }
	}

	if (SIP_NULL != pValue)
	{
		pNameValuePair->pValue = STRDUPACCESSOR(pValue);
		if (SIP_NULL == pNameValuePair->pValue)
		{
			*pErr = E_NO_MEM;
            sip_freeSipParam(pTempParam);
			return SipFail;
		}
	}

	*ppNameValue = pNameValuePair;

	SIPDEBUGFN("Exitting sip_getNameValuePairAtIndexFromCommonHdr\n");
	*pErr = E_NO_ERROR;
    sip_freeSipParam(pTempParam);
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_setNameValuePairAtIndexInCommonHdr
** DESCRIPTION: This function sets the NameValuePair at specified index
**				in Common header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pNameValue(IN)	- The name value pair to set
**				dIndex(IN)		- The index at which to set
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_setNameValuePairAtIndexInCommonHdr
	( SipHeader         *pHdr, 
	  SipNameValuePair  *pNameValue,
	  SIP_U32bit        dIndex, 
      SipError          *pErr )
{
	SipParam	*pNewParam = SIP_NULL;
	SIP_S8bit	*pValue = SIP_NULL;

	SIPDEBUGFN("Entering sip_setNameValuePairAtIndexInCommonHdr\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	/*
	 * First create a new SipParam structure and copy the info 
	 * from the SipNameValuePair (pNameValue) to this. Then set this 
	 * new SipParam into the header. The user must free the pNameValue
	 * structure after returning from this function since the data is
	 * copied by value.
	 */

	if (SIP_NULL == pNameValue)
		pNewParam = SIP_NULL;
	else
	{
		if (SipFail == sip_initSipParam(&pNewParam, pErr))
			return SipFail;

		if (SIP_NULL == pNameValue->pName)
			pNewParam->pName = SIP_NULL;
		else
		{
			pNewParam->pName = STRDUPACCESSOR(pNameValue->pName);
			if (SIP_NULL == pNewParam->pName)
			{
				*pErr = E_NO_MEM;
                sip_freeSipParam(pNewParam);
				return SipFail;
			}
		}

		if (SIP_NULL == pNameValue->pValue)
			pValue = SIP_NULL;
		else
		{
			pValue = STRDUPACCESSOR(pNameValue->pValue);
			if (SIP_NULL == pValue)
			{
				*pErr = E_NO_MEM;
                sip_freeSipParam(pNewParam);
				return SipFail;
			}
		}

		if (SipFail == sip_listAppend(&(pNewParam->slValue), (SIP_Pvoid)pValue, pErr))
        {
            sip_freeSipParam(pNewParam);
			return SipFail;
        }
	}

	if (SipFail == sip_setParamAtIndexInCommonHdr(pHdr, pNewParam, dIndex, pErr))
    {
        sip_freeSipParam(pNewParam);
		return SipFail;
    }

	SIPDEBUGFN("Exiting sip_setNameValuePairAtIndexInCommonHdr\n");

	*pErr = E_NO_ERROR;
    sip_freeSipParam(pNewParam);
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_insertNameValuePairAtIndexInCommonHdr
** DESCRIPTION: This function inserts NameValuePair at specified index
**				in Common Header
** PARAMETERS:
**				pHdr(IN)		- Pointer to the SipHeader
**				pNameValue(IN)	- The name value pair to insert
**				dIndex(IN)		- The index at which to insert
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_insertNameValuePairAtIndexInCommonHdr
	( SipHeader         *pHdr, 
      SipNameValuePair  *pNameValue,
      SIP_U32bit        dIndex, 
      SipError          *pErr )
{
	SipParam	*pNewParam = SIP_NULL;
	SIP_S8bit	*pValue = SIP_NULL;

	SIPDEBUGFN("Entering sip_insertNameValuePairAtIndexInCommonHdr\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;

	if (pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	/*
	 * First create a new SipParam structure and copy the info 
	 * from the SipNameValuePair (pNameValue) to this. Then set this 
	 * new SipParam into the header. The user must free the pNameValue
	 * structure after returning from this function since the data is
	 * copied by value.
	 */

	if (SIP_NULL == pNameValue)
		pNewParam = SIP_NULL;
	else
	{
		if (SipFail == sip_initSipParam(&pNewParam, pErr))
			return SipFail;

		if (SIP_NULL == pNameValue->pName)
			pNewParam->pName = SIP_NULL;
		else
		{
			pNewParam->pName = STRDUPACCESSOR(pNameValue->pName);
			if (SIP_NULL == pNewParam->pName)
			{
				*pErr = E_NO_MEM;
                sip_freeSipParam(pNewParam);
				return SipFail;
			}
		}

		if (SIP_NULL == pNameValue->pValue)
			pValue = SIP_NULL;
		else
		{
			pValue = STRDUPACCESSOR(pNameValue->pValue);
			if (SIP_NULL == pValue)
			{
				*pErr = E_NO_MEM;
                sip_freeSipParam(pNewParam);
				return SipFail;
			}
		}

		if (SipFail == sip_listAppend(&(pNewParam->slValue), (SIP_Pvoid)pValue, pErr))
        {
            sip_freeSipParam(pNewParam);
			return SipFail;
        }

	}

	if (SipFail == sip_insertParamAtIndexInCommonHdr(pHdr, pNewParam, dIndex, \
		pErr))
    {
        sip_freeSipParam(pNewParam);
		return SipFail;
    }

	SIPDEBUGFN("Exiting sip_insertNameValuePairAtIndexInCommonHdr\n");

	*pErr = E_NO_ERROR;
    sip_freeSipParam(pNewParam);
	return SipSuccess;
}
#endif

/* SS: Strdup used. These funtions will be removed after clarification. */
#ifdef SIP_REPLACES
/****************************************************************************
** FUNCTION: sip_getFromTagFromReplacesHdr
** DESCRIPTION: This function retrieves the from-tag field from a Replaces 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				ppFromTag(OUT)	- The retrieved from-tag
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getFromTagFromReplacesHdr 
(	SipHeader *pHdr, 
 	SIP_S8bit **ppFromTag, 
 	SipError  *pErr)
{
	SIP_S8bit		*pStringData = SIP_NULL;

	SIPDEBUGFN("Entering function sip_getFromTagFromReplacesHdr");

    if(SipFail ==
            sip_getParamForNameFromCommonHdr(pHdr,"from-tag",&pStringData,pErr))
    {
        return SipFail;
    }

	/*
	 * Now, make a copy of the from-tag found and return to the user in the field
	 * provided (ppFromTag). The user must do an explicit setFromTagInReplacesHdr
	 * if he wishes to make changes to this field.
	 */

	*ppFromTag = STRDUPACCESSOR(pStringData);
	if (SIP_NULL == *ppFromTag)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getFromTagFromReplacesHdr");
 	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_getToTagFromReplacesHdr
** DESCRIPTION: This function retrieves the to-tag field from a Replaces 
**				header.
** PARAMETERS:
**				pHdr(IN)		- Pointer to the Replaces SipHeader
**				ppToTag(OUT)	- The retrieved to-tag
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_getToTagFromReplacesHdr
	(SipHeader *pHdr, SIP_S8bit **ppToTag, SipError *pErr)
{
	SIP_S8bit       *pStringData = SIP_NULL;

	SIPDEBUGFN("Entering function sip_getToTagFromReplacesHdr");
    if(SipFail ==
            sip_getParamForNameFromCommonHdr(pHdr,"to-tag",&pStringData,pErr))
    {
        return SipFail;
    }

	/*
	 * Now, make a copy of the to-tag found and return to the user in the field
	 * provided (ppToTag). The user must do an explicit setToTagInReplacesHdr
	 * if he wishes to make changes to this field.
	 */

	*ppToTag = STRDUPACCESSOR(pStringData);
	if (SIP_NULL == *ppToTag)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getToTagFromReplacesHdr");
 	return SipSuccess;
}
#endif /* SIP_REPLACES */

#ifdef SIP_DATE
/***********************************************************************
** Function: sip_getDateFromCommonHdr
** Description: get date field from Common Header
** Parameters:
**				pHdr(IN)		- SipHeader
**				ppDate(OUT) 	- date to retrieve
**				pErr(OUT)       - error value 
************************************************************************/

SipBool sip_getDateFromCommonHdr
	( SipHeader     *pHdr,
      SipDateStruct **ppDate,
      SipError      *pErr)
{
	SIPDEBUGFN ( "Entering sip_getDateFromCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}

	if(ppDate == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

	if ((( (SipCommonHeader*) (pHdr->pHeader) )->u.pDate) == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	HSS_LOCKEDINCREF((( (SipCommonHeader*) (pHdr->pHeader) )->u.pDate)->\
		dRefCount);
	*ppDate = ( (SipCommonHeader*) (pHdr->pHeader) )->u.pDate;
	SIPDEBUGFN ( "Exiting sip_getDateFromCommonHdr");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setDateInCommonHdr
** Description: set date in Common header
** Parameters:
**				pHdr(IN/OUT)- SipHeader
**				date(IN) 	- date to set
**				err(OUT)    - error value 
************************************************************************/

SipBool sip_setDateInCommonHdr
	( SipHeader     *pHdr, 
      SipDateStruct *dDate, 
      SipError      *pErr)
{
	SipRetryAfterHeader *pRetryAfterHdr = SIP_NULL;

	SIPDEBUGFN ( "Entering sip_setDateInCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}

#endif

	pRetryAfterHdr = (SipRetryAfterHeader*)(pHdr->pHeader);

    if ((pRetryAfterHdr->u.pDate) != SIP_NULL)
        sip_freeSipDateStruct(pRetryAfterHdr->u.pDate);

	if (dDate == SIP_NULL)
	{
		pRetryAfterHdr->u.pDate=SIP_NULL;
	}
	else
	{
		HSS_LOCKEDINCREF(dDate->dRefCount);
		pRetryAfterHdr->u.pDate = dDate;
	}
	SIPDEBUGFN ( "Exiting sip_setDateInCommonHdr");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}
#endif /* SIP_DATE */


/***********************************************************************
** Function: sip_setDeltaSecondsInRetryAfterHdr
** Description: set delta seconds in RetryAfter header
** Parameters:
**				hdr(IN/OUT)	- Sip RetryAfter Header
**				dseconds(IN)	- delta seconds to set
**				err(OUT)    - Possible error value (See API ref doc)
************************************************************************/

SipBool sip_setDeltaSecondsInRetryAfterHdr
#ifdef ANSI_PROTO
	( SipHeader *pHdr, SIP_U32bit dseconds, SipError *pErr)
#else
	( hdr, dseconds, err )
	  SipHeader *hdr;
	  SIP_U32bit dseconds;
	  SipError *err;
#endif
{
	SIPDEBUGFN ( "Entering setDeltaSecondsInRetryAfterHeader");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if (pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif

#ifdef SIP_DATE
	((SipCommonHeader *)(pHdr->pHeader))->u.dSec  = dseconds;
#else
	((SipCommonHeader *)(pHdr->pHeader))->dIntVar1  = dseconds;
#endif

	SIPDEBUGFN ( "Exiting setDeltaSecondsInRetryAfterHdr");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/****************************************************************
**
** FUNCTION: sip_getdIntVar2FromCommonHdr
**
** DESCRIPTION: This function retrives the second integer field from the
** Common Header. 
**
*****************************************************************/
SipBool sip_getdIntVar2FromCommonHdr
	(SipHeader  *pHdr, 
     SIP_U32bit *pValue, 
     SipError   *pErr)
{
	SIPDEBUGFN("Entering function sip_getdIntVar2FromCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pValue == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif	
	/* setting the pValue to be returned */
	*pValue = ( (SipCommonHeader *)(pHdr->pHeader) )->dIntVar2; 
	
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getdIntVar2FromCommonHdr");
	return SipSuccess;
}

/****************************************************************
**
** FUNCTION: sip_setdIntVar2InCommonHdr
**
** DESCRIPTION: This function sets the second integer field in 
**              Common header
**
*****************************************************************/
SipBool sip_setdIntVar2InCommonHdr
	(SipHeader  *pHdr, 
     SIP_U32bit value, 
     SipError   *pErr)
{
	SIPDEBUGFN("Entering function sip_setdIntVar2InCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_HEADER;
		return SipFail;
	}
#endif
	/* setting the Value in the structure */
	((SipCommonHeader *)(pHdr->pHeader))->dIntVar2 = value; 
	
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setdIntVar2InCommonHdr");
	return SipSuccess;
}
#if 0
#ifdef SIP_DATE
#endif /* SIP_DATE */


#endif
#ifdef SIP_CORE_STACK_COMPATIBLE
/*******************************************************************************
** FUNCTION: sip_getGenericCredintialsFromAuthorizationHeader
**
**  DESCRIPTION: This function changes Returns a generic Credential 
** 				Structure from SipAuthorization Structure
** PARAMETERS:
**		pSipCommonHeader(IN): The SipCommonHeader
**		pCredintial(OUT)				: The SipGenericCredential
**		pError(OUT)						: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool sip_getCredentialsFromAuthorizationHdr\
	(SipHeader *pHdr, SipGenericCredential **pCredintial,\
	 SipError *pError)
{

	SipParam *pParam=SIP_NULL;
	SipCommonHeader *pAuth = SIP_NULL;
	SIPDEBUGFN( "Entering getCredentialsFromAuthorizationHdr");
#ifndef SIP_NO_CHECK
	if(pError == SIP_NULL)
		return SipFail;


	if ( pHdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if ( pCredintial == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	if ((pHdr->pHeader == SIP_NULL))
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
	if ((pHdr->dType != SipHdrTypeAuthorization) &&\
		(pHdr->dType != SipHdrTypeProxyauthorization))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}

#endif
	/*
	 * First check whether the cred is basic
	 */
	pAuth= ((SipCommonHeader*)(pHdr->pHeader));
	if ((SIP_NULL == (((SipCommonHeader*)(pHdr->pHeader))->pStr1)))
	{
		*pError = E_NO_EXIST;
		return SipFail;
	}	
		
	if(sip_strcasecmp(pAuth->pStr1,(SIP_S8bit*)"Basic") == 0)
	{
		SIP_S8bit *pString = SIP_NULL;
		SIP_U32bit tmpSize=0;

		/*
		 * if Cred is basic type proceed according to
		 */
		if (SipFail == sip_initSipGenericCredential(pCredintial,\
					SipCredBasic, pError))
			return SipFail;
		/*
		 * Copy the basic related part
		 */ 	
		/*
		 * Now if the  Cred Type is basic get the name fro  SipParam
		 */ 
		/*if (sip_initSipParam(&pParam, pError)== SipFail)
		{
			return SipFail	;
		
		}*/	
		if (SipFail == sip_listGetAt(&(pAuth->slParam),0 ,\
					(SIP_Pvoid*)&pParam, pError ))	
		{	
			sip_freeSipGenericCredential(*pCredintial);
			return SipFail;	
		}	
		if (SIP_NULL!= pParam->pName)
			tmpSize = sip_strlen(pParam->pName);

		if (SipFail == sip_listGetAt(&(pParam->slValue),0 ,\
			(SIP_Pvoid*)&pString, pError))	
		{
			/*if (*pError != E_INV_INDEX)
			{
				sip_freeSipGenericCredential(*pCredintial);
				return SipFail;	
			}*/
		}

		if (pString != SIP_NULL)
			tmpSize += sip_strlen(pString)+2; /* for '=' and '\0' */
		else
			tmpSize += 1; /* for '\0' */

		(*pCredintial)->u.pBasic= (SIP_S8bit *)fast_memget(\
			ACCESSOR_MEM_ID, tmpSize, pError);
		if (SIP_NULL == (*pCredintial)->u.pBasic)
		{
			sip_freeSipGenericCredential(*pCredintial);
			return SipFail;	
		}
        (void)sip_strcpy((*pCredintial)->u.pBasic, pParam->pName);
		if (pString != SIP_NULL)
		{
			(void)sip_strcat((*pCredintial)->u.pBasic,(SIP_S8bit *) "=");
			(void)sip_strcat((*pCredintial)->u.pBasic, pString);
		}
	}	
	else
	{
		/*
		 * If not basic process according to
		 */
		if (SipFail == sip_initSipGenericCredential(pCredintial, SipCredAuth, \
			pError))
			return SipFail;
		/*
		 * Init generic challenge
		 */ 
		if (SipFail == sip_initSipGenericChallenge((&(*pCredintial)->\
						u.pChallenge),	pError))
		{
			sip_freeSipGenericCredential(*pCredintial);
			return SipFail;
		}
		/*
		 * Copy the scheme
		 */ 
        (*pCredintial)->u.pChallenge->pScheme =\
            sip_strdup(pAuth->pStr1, ACCESSOR_MEM_ID);
		/*
		 * Copy the sipList
		 */ 
		if (SipFail == __sipParser_cloneSipParamList(&((*pCredintial)->\
			u.pChallenge->slParam), &(pAuth->slParam), pError))
		{
			sip_freeSipGenericCredential(*pCredintial);
			*pError = E_NO_EXIST;
			return SipFail;
		}	
	}	
	*pError = E_NO_ERROR;
	return SipSuccess;	
}

/***********************************************************************
** Function: sip_setCredentialsInAuthorizationHdr 
** Description: sets the Credential type in the Sip Authorisation Header
** Parameters:
**			pHdr(IN/OUT)	- Sip Authorization Header
**			pHtype(IN)		- The header type set
**			pErr(OUT)		- Possible Error value (see API ref doc)
**
************************************************************************/
 SipBool sip_setCredentialsInAuthorizationHdr _ARGS_((SipHeader *hdr, \
		SipGenericCredential *pGcr, SipError *pError))
{
	SipCommonHeader *pSipCommonHeader= SIP_NULL;

#ifndef SIP_NO_CHECK
	if(pError == SIP_NULL)
		return SipFail;
	if ( hdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if ((hdr->dType != SipHdrTypeAuthorization) &&\
		(hdr->dType != SipHdrTypeProxyauthorization))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}
	if ((hdr->pHeader == SIP_NULL))
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
#endif

	pSipCommonHeader = (SipCommonHeader*)(hdr->pHeader);
	if (SIP_NULL == pGcr)
	{
		if (pSipCommonHeader->pStr1 != SIP_NULL)
		{
			(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
				pStr1,pError );
			pSipCommonHeader->pStr1= SIP_NULL;
		}	
		(void)sip_listDeleteAll(&(pSipCommonHeader->slParam), pError);
	}
	else
	{
		if (pGcr->dType == SipCredBasic)
		{
			SipParam *pTmp =SIP_NULL;
			/*
			 * Store  scheme
			 */ 
			if (pSipCommonHeader->pStr1 != SIP_NULL)
			{
					(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
							pStr1	,pError );

					pSipCommonHeader->pStr1= SIP_NULL;
			}	
			pSipCommonHeader->pStr1 = sip_nStrdup(ACCESSOR_MEM_ID, \
				(SIP_S8bit*) "Basic", sip_strlen((SIP_S8bit*)"Basic"), pError);
			if (pSipCommonHeader->pStr1 == SIP_NULL)
				return SipFail;
			if (SipFail ==sip_initSipParam(&pTmp ,pError) )
			{	
				if (pSipCommonHeader->pStr1 == SIP_NULL)
				{	
					(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
							pStr1	,pError );
				}
				return SipFail;
			}
			
			/*
			 * Store value
			 */
			if (pGcr->u.pBasic != SIP_NULL)
			 pTmp->pName = sip_strdup(pGcr->u.pBasic, \
				ACCESSOR_MEM_ID);	
			if (SIP_NULL ==pTmp->pName)
			{
				sip_freeSipParam(pTmp);
				(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
					pStr1	,pError );
				return SipFail;
			}
			/*
			 * Now insert the sipparam to slparam
			 */ 
			(void)sip_listDeleteAll(&(pSipCommonHeader->slParam), pError);
            (void)sip_listInit(&(pSipCommonHeader->slParam), \
					__sip_freeSipParam ,pError);
			if (SipFail == sip_listInsertAt(&(pSipCommonHeader->slParam), \
						0, (SIP_Pvoid)pTmp, pError))
			{
			
				sip_freeSipParam(pTmp);
				(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
					pStr1	,pError );
				return SipFail;
			}	
			
		}
		if (pGcr->dType == SipCredAuth)
		{
			if (SIP_NULL !=(pGcr->u.pChallenge->pScheme) )
			{
                if (pSipCommonHeader->pStr1 != SIP_NULL)
        			{
		    			(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->\
			    				pStr1	,pError );
                    }	
				pSipCommonHeader->pStr1 = sip_nStrdup(ACCESSOR_MEM_ID, \
				pGcr->u.pChallenge->pScheme, sip_strlen(pGcr->u.pChallenge->\
					pScheme), pError);
			}	
			if (SipFail ==__sipParser_cloneSipParamList(&(pSipCommonHeader->\
							slParam), &(pGcr->u.pChallenge->slParam), pError))
			{
				sip_freeSipGenericCredential(pGcr);
				return SipFail;
			}	
		}	
		if ((pGcr->dType) == SipCredAny)
			return SipFail;		
	}

	*pError = E_NO_ERROR;
	return SipSuccess;
}		
/*******************************************************************************
** FUNCTION: sip_getChallengeFromWwwAuthenticateHeader
**
**  DESCRIPTION: This function changes Returns a generic Challenge
**				 from SipCommonHeader Header	
** PARAMETERS:
**		pSipCommonHeader(IN)	: The SipCommonHeader
**		pChallenge(OUT)					: The SipGenericChallenge
**		pError(OUT)						: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getChallengeFromWwwAuthenticateHdr(SipHeader *hdr , \
		SipGenericChallenge **pChallenge,  SipError *pError)


{
	SipGenericChallenge *pTmp = SIP_NULL;
	
#ifndef SIP_NO_CHECK
	if( pError == SIP_NULL )
	{
		return SipFail;
	}

	if (( hdr == SIP_NULL)||(pChallenge==SIP_NULL))
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	if ((hdr->dType != SipHdrTypeWwwAuthenticate) &&\
		(hdr->dType != SipHdrTypeProxyAuthenticate))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}

	if (hdr->pHeader == SIP_NULL)
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
#endif
	if (SipFail == sip_initSipGenericChallenge(&pTmp, pError))
	{
		return SipFail;
	}
	if (SIP_NULL != ((((SipCommonHeader*)(hdr->pHeader))->pStr1)))
		pTmp->pScheme =sip_strdup(((SipCommonHeader*)(hdr->pHeader))->\
				pStr1, ACCESSOR_MEM_ID);

	else
	{
		/* 
		 * If either scheme OR slParams is NULL, we treat it as a WwwAuth hdr 
		 * which has a NULL challenge field, and hence return E_NO_EXIST from
		 * this function. This is because our setChallengeInWwwAuthHdr makes
		 * both scheme and slParams NULL when asked to set a NULL Challenge
		 * into the hdr.
		 */
		*pError = E_NO_EXIST;
		sip_freeSipGenericChallenge(pTmp);
		return SipFail;
	}
	
	if (SipFail == __sipParser_cloneSipParamList(&((pTmp)-> \
		slParam), &(((SipCommonHeader*)(hdr->pHeader))->slParam),pError))
	{
		sip_freeSipGenericChallenge(pTmp);
		return SipFail;
	}
	*pChallenge = pTmp;
	*pError = E_NO_ERROR;
	return SipSuccess;	

}			

/***********************************************************************
** Function: sip_setChallengeInWwwAuthenticateHdr
** Description: set Challenge field in WWWAuthenticate Header
** Parameters:
**				hdr(IN/OUT)		- Sip WWWAuthenticate Header
**				challenge(IN) 	- Challenge to set
**				err(OUT)    	- Possible error value (See API ref doc)
** Return Value:
**				  SipBool	
************************************************************************/
SipBool sip_setChallengeInWwwAuthenticateHdr _ARGS_(( SipHeader *hdr, \
		SipGenericChallenge *pChallenge, SipError *pError))
{
	
	SipCommonHeader *pSipCommonHeader=SIP_NULL;
#ifndef SIP_NO_CHECK
	if( pError == SIP_NULL )
		return SipFail;

	if ( hdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	if ((hdr->dType != SipHdrTypeWwwAuthenticate) &&\
		(hdr->dType != SipHdrTypeProxyAuthenticate))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}

	if (hdr->pHeader == SIP_NULL)
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
#endif
	
	
	pSipCommonHeader=(SipCommonHeader*)hdr->pHeader;
	if (pChallenge !=SIP_NULL)
	{
		if (pChallenge->pScheme != SIP_NULL)
		{
			if (pSipCommonHeader->pStr1 != SIP_NULL)
				(void)fast_memfree(ACCESSOR_MEM_ID ,\
					   	pSipCommonHeader->pStr1, pError);
			pSipCommonHeader->pStr1 = sip_nStrdup(ACCESSOR_MEM_ID, \
					pChallenge->pScheme, sip_strlen(pChallenge->pScheme), pError);
								
		}
		if (SipFail == __sipParser_cloneSipParamList\
				(&(pSipCommonHeader->slParam),	&(pChallenge->slParam),\
				 pError))
		{
			sip_freeSipGenericChallenge(pChallenge);
			return SipFail;
		}
	}	
	else
	{
		if (pSipCommonHeader->pStr1 != SIP_NULL)
		{
			(void)fast_memfree(ACCESSOR_MEM_ID, pSipCommonHeader->pStr1, \
				pError);
			pSipCommonHeader->pStr1 = SIP_NULL;
		}
		(void)sip_listDeleteAll(&(pSipCommonHeader->slParam), pError);
	}

	*pError = E_NO_ERROR;
	return SipSuccess;
}

#endif
/******************************************************************************
** FUNCTION: sip_getContactParamAtIndexFromContactHdr
** DESCRIPTION:gets the Contact Parameters at an index_1 from the SipCommonHeader
** PARAMETERS:
**			hdr(IN) 			- SipCommonHeader
**			conatct_param(OUT)	- ContactParameter to retrieve
**			index_1(IN)			- Index at which the Contact Param is retrieved
**			err(OUT)			- Possible error value(see API ref doc)
**
*******************************************************************************/


/*SipBool sip_checkContactFeatureParam(SIP_S8bit*); */

#ifdef SIP_3GPP

SipBool sip_checkNameParam(SIP_S8bit ch,SIP_S8bit* pname)
{
	SipBool flag=SipFail;
	switch(ch)
	{
	case 'a':
		if ( sip_strcmp(pname,(SIP_S8bit*)"audio")==0 || sip_strcmp(pname,(SIP_S8bit*)"automata")==0 || 												sip_strcmp(pname,(SIP_S8bit*)"application")==0 )
					flag = SipSuccess;
        	break; 
	case 'c':
		if  (sip_strcmp(pname,(SIP_S8bit*)"class")==0 || sip_strcmp(pname,(SIP_S8bit*)"control")==0)	
        	     flag=SipSuccess;
	        break;
	case 'd':
		if (sip_strcmp(pname,(SIP_S8bit*)"duplex")==0 || sip_strcmp(pname,(SIP_S8bit*)"data")==0 || 												sip_strcmp(pname,(SIP_S8bit*)"description")==0 )
			flag = SipSuccess;
        	break;

	case 'e':
		if ( sip_strcmp(pname,(SIP_S8bit*)"events")==0 )
			flag = SipSuccess;
	        break;
	case 'i':
		if ( sip_strcmp(pname,(SIP_S8bit*)"isfocus")==0 )
			flag = SipSuccess;
        	break;
	case 'l':
		if ( sip_strcmp(pname,(SIP_S8bit*)"language")==0 )
			flag = SipSuccess;
	        break;
	case 'm':
		if ( sip_strcmp(pname,(SIP_S8bit*)"mobility")==0 || sip_strcmp(pname,(SIP_S8bit*)"methods")==0 || 													sip_strcmp(pname,(SIP_S8bit*)"msgserver")==0 )
			flag = SipSuccess;
	        break;
	case 'p':
		if ( sip_strcmp(pname,(SIP_S8bit*)"priority")==0 )
			flag = SipSuccess;
	        break;	
	case 's':
		if ( sip_strcmp(pname,(SIP_S8bit*)"schemes")==0 )
			flag = SipSuccess;
	        break;

	case 't':
		if ( sip_strcmp(pname,(SIP_S8bit*)"type")==0 )
			flag = SipSuccess;
	        break;
	case 'u':
		if  (sip_strcmp(pname,(SIP_S8bit*)"uri-user")==0 || sip_strcmp(pname,(SIP_S8bit*)"uri-domain")==0)	
        	     flag=SipSuccess;
	        break;
	case 'v':
		if ( sip_strcmp(pname,(SIP_S8bit*)"video")==0 )
			flag = SipSuccess;
	        break;

	}
 	return(flag);

}

SipBool sip_checkContactFeatureParam(SIP_S8bit* pname)
{
      SipBool flag;

      if (pname[0] != '+' )
  	 flag=sip_checkNameParam(pname[0],pname);
      else
      {	
	flag=sip_checkNameParam(pname[1],pname+1);	
	if ( flag )
 		flag=SipFail;
	else
		flag=SipSuccess;
      } 	
      return(flag);
}

#endif




SipBool sip_getContactParamAtIndexFromContactHdr _ARGS_ ((SipHeader *hdr, \
	SipContactParam **contact_param, SIP_U32bit index_1, SipError *pError))

{	
		SIP_U32bit dateLength				= 0;
		SIP_U32bit length1 					= 0;
		SIP_U32bit length2 					= 0;
		SIP_S8bit  *ptr    					= SIP_NULL;
		SIP_S8bit *pTmpString 				= SIP_NULL;
		SipParam *pSipParam 				= SIP_NULL;
		SipContactParam *pSipContactParam 	= SIP_NULL;
#ifdef SIP_DATE
		SipDateStruct *pDate				= SIP_NULL;
#endif
		SipCommonHeader *pSipCommonHeader = SIP_NULL;
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pError == SIP_NULL )
		return SipFail;

	if ( hdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if ( contact_param == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if ( (hdr == SIP_NULL)  )
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	if ( (hdr->pHeader == SIP_NULL))
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}

#endif
	 pSipCommonHeader = (SipCommonHeader*)hdr->pHeader;
	 if(( hdr->dType != SipHdrTypeContactNormal)&&(hdr->dType != \
					 SipHdrTypeContactWildCard))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}

	if( SipFail == sip_listGetAt\
		(&(pSipCommonHeader->slParam), index_1,(SIP_Pvoid*)&pSipParam, pError))
	{
		return SipFail;
	}
	else if (sip_strcasecmp(pSipParam->pName, (SIP_S8bit *)"expires") == 0)
	{
		SipExpiresStruct *pExp;
		if (SipFail == sip_initSipContactParam (&pSipContactParam, \
			SipCParamExpires , pError))
		{
			return SipFail;
		}

#ifdef SIP_DATE
		if (SipFail == sip_initSipExpiresStruct (&pExp, SipExpSeconds , pError))
		{
			return SipFail;
		}
#else
		if (SipFail == sip_initSipExpiresStruct (&pExp, pError))
		{
			return SipFail;
		}
#endif /* SIP_DATE */

		if (SipFail == sip_listGetAt(&(pSipParam->slValue), 0, \
			(SIP_Pvoid*)&pTmpString, pError))
		{
			sip_freeSipExpiresStruct(pExp);
			sip_freeSipContactParam(pSipContactParam);
			return SipFail;
		}
		dateLength = sip_strlen(pTmpString);
#ifdef SIP_DATE
		if (SipFail==sip_atoi(pTmpString,pTmpString+ (dateLength-1), \
			&(pExp->u.dSec)))
#else
		if (SipFail==sip_atoi(pTmpString,pTmpString+ (dateLength-1), \
			&(pExp->dIntVar1)))
#endif
		{
#ifdef SIP_DATE
			if (SipFail ==sip_parseSipDate(pTmpString+1,(pTmpString+\
							(dateLength-2)), &pDate,	pError))
			{	
				sip_freeSipExpiresStruct(pExp);
				sip_freeSipContactParam(pSipContactParam);
				return SipFail;
			}
			else
			{	pExp->dIntVar1 = SipExpDate;
				pExp->u.pDate = pDate;
				pSipContactParam->u.pExpire = pExp;
			
			}	
#else
			sip_freeSipExpiresStruct(pExp);
			sip_freeSipContactParam(pSipContactParam);
			return SipFail;
#endif /* SIP_DATE */
		}
		else
		{
			pSipContactParam->u.pExpire = pExp;
		
		}	
	}
	else if (sip_strcasecmp(pSipParam->pName, (SIP_S8bit *)"q") == 0)	
	{
		if (SipFail == sip_initSipContactParam (&pSipContactParam, \
			SipCParamQvalue , pError))
		{
			return SipFail;
		}	
		if (SipFail == sip_listGetAt(&(pSipParam->slValue), 0, \
			(SIP_Pvoid*)&pTmpString, pError))
		{
			sip_freeSipParam(pSipParam);
			sip_freeSipContactParam(pSipContactParam);
			return SipFail;
		}
		pSipContactParam->u.pQValue = sip_strdup(pTmpString, ACCESSOR_MEM_ID);
	}
	else
	{
#ifdef SIP_3GPP

	if (SipFail == sip_listGetAt(&(pSipParam->slValue), 0, \
			(SIP_Pvoid*)&pTmpString, pError))
		{
			sip_freeSipParam(pSipParam);
			return SipFail;
		}
 		if (pTmpString[0] == '\"')
                {
			if(!sip_checkContactFeatureParam(pSipParam->pName))
		                 return SipFail;

			length1 = sip_strlen(pSipParam->pName);
			if (SipFail == sip_listGetAt(&(pSipParam->slValue), 0, \
				(SIP_Pvoid*)&pTmpString, pError))
			{
                                /* Fix from UATK 2.2 Start*/
				if(pSipParam->pName != SIP_NULL)
				{
					if (SipFail == sip_initSipContactParam (&pSipContactParam, \
								SipCParamFeature , pError))
					{
						sip_freeSipParam(pSipParam);
						return SipFail;
					}
					if(!sip_checkContactFeatureParam(pSipParam->pName))
					{
						sip_freeSipParam(pSipParam);
						return SipFail;
					}
					pSipContactParam->u.pFeatureAttr = sip_strdup(pSipParam->pName, ACCESSOR_MEM_ID);
					*contact_param = pSipContactParam;
					return SipSuccess;
				}
				else 
				{
					sip_freeSipParam(pSipParam);
					sip_freeSipContactParam(pSipContactParam);
					return SipFail;
				}
				/* Fix from UATK 2.2 End*/
			
			}

			 length2 = sip_strlen(pTmpString);
			 ptr = (SIP_S8bit*)fast_memget(ACCESSOR_MEM_ID, \
			     (length2+length1+2), pError);
			 (void)sip_strcpy(ptr, pSipParam->pName);
			 (void)sip_strcat(ptr, (SIP_S8bit*)"=");
			 (void)sip_strcat(ptr, pTmpString);
			if (SipFail == sip_initSipContactParam (&pSipContactParam, \
				SipCParamFeature , pError))
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}
		 	pSipContactParam->u.pFeatureAttr = sip_strdup(ptr, ACCESSOR_MEM_ID);
		 	(void)fast_memfree(ACCESSOR_MEM_ID, ptr, pError);

		}

		else
#endif /* 3gpp end */
		{
			if (SipFail == sip_initSipContactParam (&pSipContactParam, \
				SipCParamExtension , pError))
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}
			
			length1 = sip_strlen(pSipParam->pName);
			if (SipFail == sip_listGetAt(&(pSipParam->slValue), 0, \
				(SIP_Pvoid*)&pTmpString, pError))
			{
				sip_freeSipParam(pSipParam);
				sip_freeSipContactParam(pSipContactParam);
				return SipFail;
			}

			 length2 = sip_strlen(pTmpString);
			 ptr = (SIP_S8bit*)fast_memget(ACCESSOR_MEM_ID, \
			(length2+length1+2), pError);
			 (void)sip_strcpy(ptr, pSipParam->pName);
			 (void)sip_strcat(ptr, (SIP_S8bit*)"=");
			 (void)sip_strcat(ptr, pTmpString);

		 	pSipContactParam->u.pExtensionAttr = sip_strdup(ptr, ACCESSOR_MEM_ID);
		 	(void)fast_memfree(ACCESSOR_MEM_ID, ptr, pError);
		}

	
	}
	*contact_param = pSipContactParam;
	/*HSS_LOCKEDINCREF(pSipContactParam->dRefCount);*/
	return SipSuccess;
}

/*******************************************************************************
**
** FUNCTION:  validateSipContactParamsType
**
** DESCRIPTION:  This fucntion returns SipSuccess if "dType"
** 				is one among the defined en_ContactParamsType's; 
**				else it returns SipFail.
** PARAMETERS:
**			dType(IN)  - the contactType
**			err(OUT)   - The error Value
** ReturnValue:
**			SipBool
**
*******************************************************************************/


SipBool validateSipContactParamsType
#ifdef ANSI_PROTO
	(en_ContactParamsType dType, SipError *err)
#else
	(dType, err)
	en_ContactParamsType dType;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function validateSipContactParamsType");
	switch(dType)
	{
		case	SipCParamQvalue		:
		case	SipCParamExpires	:
		case	SipCParamAction		:
		case	SipCParamExtension	:
#ifdef SIP_3GPP		
		case    SipCParamFeature	:
#endif		
		case	SipCParamAny		:
						 break;
		default				:*err = E_INV_TYPE;
						 return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function validateSipContactParamsType");
	return SipSuccess;
}

/******************************************************************************
** FUNCTION: sip_setContactParamAtIndexInContactHdr
** DESCRIPTION:set the Contact Parameters at an index_1 in the Sip Contact Header
** PARAMETERS:
**			hdr(IN/OUT) 			- Sip Contact Header
**			conatct_param(IN)		- ContactParameter to set
**			index_1(IN)				- Index at which the Contact Parameteris set
**			err(OUT)				- Possible error value(see API ref doc)
**
******************************************************************************/
SipBool sip_setContactParamAtIndexInContactHdr _ARGS_ ((SipHeader *hdr, \
	SipContactParam *contact_param, SIP_U32bit index_1, SipError *pError))
{

					
	SipParam *pSipParam 				= SIP_NULL;
	SIP_S8bit* pValue					= SIP_NULL;
	SIP_U32bit slen						= 0;
#ifndef SIP_NO_CHECK
	if( pError == SIP_NULL)
		return SipFail;
	if( hdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if( hdr->pHeader == SIP_NULL)
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
	if ((hdr->dType != SipHdrTypeContactNormal)&& \
		(hdr->dType != SipHdrTypeContactWildCard))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}
#endif

	if (SIP_NULL == contact_param)
	{
		pSipParam = SIP_NULL;
		if (sip_listSetAt(&(((SipCommonHeader *)(hdr->pHeader))->slParam),
			index_1, (SIP_Pvoid)pSipParam, pError) == SipFail)
		{
			return SipFail;
		}
		return SipSuccess;
	}

	if (SipFail == sip_initSipParam(&pSipParam, pError))
		return SipFail;

	if (validateSipContactParamsType(contact_param->dType, pError) == \
		SipFail)
		return SipFail;

	switch (contact_param->dType)
	{
		case SipCParamAction:
		{
			pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
				(SIP_S8bit*) "action", 6, pError);
			slen = sip_strlen(contact_param->u.pAction);
			/*
			 * Getting the string  and setting it at SipList of value
			 */
			pValue = sip_nStrdup(ACCESSOR_MEM_ID, contact_param->u.pAction, \
					slen, pError);
			if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValue, pError) == SipFail)
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}

			break;
		}
		case SipCParamExpires:
		{
#ifdef SIP_DATE
			if (contact_param->u.pExpire->dIntVar1 == SipExpSeconds)
			{
				SIP_S8bit arr[20];
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
				(void)sip_snprintf(arr, 20, "%u", \
						contact_param->u.pExpire->u.dSec);
				pValue = sip_strdup(arr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			}
			if (contact_param->u.pExpire->dIntVar1 == SipExpDate)
			{
				SIP_S8bit *pArr = SIP_NULL;
				SIP_U32bit i;
				pArr = (SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID, 50 , pError);
				for (i=0; i < 50; i++)
					pArr[i] = ' ';
				pArr[0]= '\0';
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
				(void)STR_CAT(pArr, (char*)"\"");
				if (SipFail == sip_formDateStruct(&pArr,contact_param->\
							u.pExpire->u.pDate, pError))
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				
				}	
				(void) STR_CAT(pArr,(SIP_S8bit *) "\"");
				pValue = sip_strdup(pArr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			
			}	
#else
			{
				SIP_S8bit arr[20];
				
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
				(void)sip_snprintf(arr, 20, "%u", \
					contact_param->u.pExpire->dIntVar1);
				pValue = sip_strdup(arr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			}
#endif /* SIP_DATE */
			break;
		}
		case SipCParamQvalue:
		{
			pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, (SIP_S8bit*)"q", 1,\
				   	pError);
			slen = sip_strlen(contact_param->u.pQValue);
			/*
			 * Getting the string  and setting it at SipList of value
			 */
			pValue = sip_nStrdup(ACCESSOR_MEM_ID, contact_param->u.pQValue, \
					slen, pError);
			if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValue, pError) == SipFail)
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}

			break;
		}
		case SipCParamExtension:
		{
			SIP_S8bit *pTmpExt = SIP_NULL;
			SIP_S8bit *pNameSt = SIP_NULL;
			SIP_S8bit *pValueSt = SIP_NULL;
			SIP_U32bit len = 0;
			
				pTmpExt = contact_param->u.pExtensionAttr;
				len = sip_strlen(contact_param->u.pExtensionAttr);
				pNameSt = sip_tokenize(pTmpExt,pTmpExt+len , '=' );
				if (pNameSt == SIP_NULL)
					return SipFail;
				
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 pNameSt - pTmpExt, pError);
				pValueSt = sip_nStrdup(ACCESSOR_MEM_ID, pNameSt+1,
					  (pTmpExt+len) - (pNameSt), pError);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValueSt, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}

			break;
		}
#ifdef SIP_3GPP
		case SipCParamFeature:
		{
			SIP_S8bit *pTmpExt = SIP_NULL;
			SIP_S8bit *pNameSt = SIP_NULL;
			SIP_S8bit *pValueSt = SIP_NULL;
			SIP_U32bit len = 0;
			
				pTmpExt = contact_param->u.pFeatureAttr;
				len = sip_strlen(contact_param->u.pFeatureAttr);
				pNameSt = sip_tokenize(pTmpExt,pTmpExt+len , '=' );
				if (pNameSt == SIP_NULL)
                                /*Fix from UATK 2.2 Start*/
				{
					pNameSt = pTmpExt;
					pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 len, pError);
					break;
				}
				/*Fix from UATK 2.2 End*/
				
				
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 pNameSt - pTmpExt, pError);
				pValueSt = sip_nStrdup(ACCESSOR_MEM_ID, pNameSt+1,
					  (pTmpExt+len) - (pNameSt), pError);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValueSt, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}

			break;
		}
#endif


		default:;
	}
	
	if (sip_listSetAt( &(((SipCommonHeader *)(hdr->pHeader))->slParam), 
		index_1, (SIP_Pvoid)pSipParam, pError) == SipFail)
	{
		sip_freeSipParam(pSipParam);
		return SipFail;
	}
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_getAuthorizationParamAtIndexInChallenge
** 
** DESCRIPTION: his function gets an Authorization Param
** from a given index_1 in SipGenericChallenge.
** PARAMETERS:
**			pSch(IN)				- SipGenericCredential
**			dIndex(IN)				- The index_1 at 
**			ppParam(IN)				- The outpuut SipParam
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/

#ifdef SIP_AUTHENTICATE
SipBool sip_getAuthorizationParamAtIndexFromChallenge
#ifdef ANSI_PROTO
#ifdef SIP_BY_REFERENCE
	( SipGenericChallenge 	*pSch, 
	  SipParam **ppParam, 
	  SIP_U32bit 	dIndex, 
	  SipError 	*pErr )
#else
	( SipGenericChallenge 	*pSch, 
	  SipParam *pParam, 
	  SIP_U32bit 	dIndex, 
	  SipError 	*pErr )
#endif
#else 
#ifdef SIP_BY_REFERENCE
	( pSch, ppParam, dIndex, pErr)
	  SipGenericChallenge 	*pSch;
	  SipParam **ppParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#else
	( pSch, pParam, dIndex, pErr)
	  SipGenericChallenge 	*pSch;
	  SipParam *pParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
#endif
{
	SIP_Pvoid 	pElement_from_list;

	SIPDEBUGFN( "Entering getAuthorizationParamAtIndexFromChallenge");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

#ifdef SIP_BY_REFERENCE
	if ( (pSch == SIP_NULL) || (ppParam == SIP_NULL) )
#else
	if ( (pSch == SIP_NULL) || (pParam == SIP_NULL) )
#endif
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	if ( ppParam == SIP_NULL)
#else
	if ( pParam == SIP_NULL)
#endif
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	
	if( sip_listGetAt(&(pSch->slParam), dIndex, &pElement_from_list, pErr)\
		== SipFail)
		return SipFail;
	
	if ( pElement_from_list == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	HSS_LOCKEDINCREF(((SipParam *)pElement_from_list)->dRefCount);
	*ppParam = (SipParam *) pElement_from_list;

#else
	if ( __sip_cloneSipParam(pParam, (SipParam *)pElement_from_list, pErr) == \
		SipFail)
	{
		sip_freeSipParam((SipParam *) pElement_from_list);
		return SipFail;
	}
#endif

	SIPDEBUGFN( "Exitting getAuthorizationParamAtIndexFromChallenge");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: sip_setAuthorizationParamAtIndexInChallenge
** 
** DESCRIPTION: his function sets an Authorization Param
** at a given index_1 in SipGenericChallenge.
** PARAMETERS:
**			pSch(IN/OUT)			- SipGenericCredential
**			index_1(IN)				- The index_1 at which which 
**						authorization param is  to be set .
**			pParam(IN)				- The input SipParam
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/

SipBool sip_setAuthorizationParamAtIndexInChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge 	*pSch, 
	  SipParam	*pParam, 
	  SIP_U32bit dIndex, 
	  SipError 	*pErr )
#else 
	( pSch, pParam, dIndex, pErr)
	  SipGenericChallenge 	*pSch;
	  SipParam	*pParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
{
	SipParam 	*pElement_in_list;

	SIPDEBUGFN( "Entering setAuthorizationParamAtIndexInChallenge");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;
	
	if (pSch == SIP_NULL) 
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
        (void)sip_initSipParam(&pElement_in_list, pErr);
		if ( __sip_cloneSipParam(pElement_in_list, pParam, pErr) == SipFail)		
		{
			sip_freeSipParam(pElement_in_list);
			return SipFail;
		}
#endif
	}

	if( sip_listSetAt(&(pSch->slParam), dIndex, (SIP_Pvoid) pElement_in_list, \
		pErr) == SipFail)
	{
		if (pElement_in_list != SIP_NULL )
#ifdef SIP_BY_REFERENCE
                HSS_LOCKEDDECREF(pParam->dRefCount);
#else
			sip_freeSipParam(pElement_in_list);
#endif
		return SipFail;
	}
	SIPDEBUGFN( "Exitting setAuthorizationParamAtIndexInChallenge");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_getAuthorizationParamCountFromChallenge
** 
** DESCRIPTION: This function retrieves the number of 
**		authorization-pParam in a SIP Challenge
** PARAMETERS:
**			pSch(IN)			   : The SipGenericChallenge
**			pIndex(OUT)			   : pParam Count	
**			pErr(OUT)			   : The error Value	
**
*******************************************************************************/

SipBool sip_getAuthorizationParamCountFromChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge	*pSch, 
	  SIP_U32bit	*pIndex, 
	  SipError	*pErr  )
#else 
	( pSch, pIndex, pErr)
	  SipGenericChallenge 	*pSch;
	  SIP_U32bit 	*pIndex;
	  SipError 	*pErr;
#endif
{
	SIPDEBUGFN( "Entering getAuthorizationParamCountFromChallenge");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if ( pErr == SIP_NULL ) 
		return SipFail;
	
	if ( (pSch == SIP_NULL) || ( pIndex == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	if (sip_listSizeOf(&(pSch->slParam), pIndex , pErr) == SipFail )
	{
		return SipFail;
	}
	
	SIPDEBUGFN( "Exitting getAuthorizationParamCountFromChallenge");
	*pErr = E_NO_ERROR;
	return SipSuccess;	
}
/*******************************************************************************
** FUNCTION: sip_deleteAuthorizationParamAtIndexInChallenge
** 
** DESCRIPTION: his function deletes an Authorization Param
** at a given index_1 from SipGenericChallenge.
** PARAMETERS:
**			pSch(IN)				- SipGenericCredential
**			index_1(IN)				- The index_1 from which 
**						authorization param has to be deleted .
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_deleteAuthorizationParamAtIndexInChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge 	*pSch, 
	  SIP_U32bit 	dIndex, 
	  SipError 	*pErr )
#else 
	( pSch, dIndex, pErr)
	  SipGenericChallenge 	*pSch;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
{
	SIPDEBUGFN( "Entering deleteAuthorizationParamAtIndexInChallenge");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;
		
	if ( pSch == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pSch->slParam), dIndex, pErr) == SipFail)
		return SipFail;
	

	SIPDEBUGFN( "Exitting deleteAuthorizationParamAtIndexInChallenge");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: sip_getCredentialTypeFromCredential
** 
** DESCRIPTION: his function retrieves the dType of a SIP credential
** PARAMETERS:
**			pCr(IN)					- SipGenericCredential
**			pType(OUT)				- The  Credential Type.
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_getCredentialTypeFromCredential
#ifdef ANSI_PROTO
	(SipGenericCredential *pCr, 
	en_CredentialType *pType, 
	SipError *pErr)
#else
	( pCr, pType , pErr )
	  SipGenericCredential *pCr;
	  en_CredentialType *pType; 
	  SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering getCredentialTypeFromCredential");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if(pErr == SIP_NULL)
		return SipFail;

	if ( pCr == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	

	*pType = pCr->dType; 	
	SIPDEBUGFN( "Exitting getCredentialTypeFromCredential");

	*pErr=E_NO_ERROR;
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: sip_getBasicInCredential
** 
** DESCRIPTION: This function gets the pBasic field from SIP Credential
** PARAMETERS:
**			pCr(IN)						- SipGenericCredential
**			ppBasic(OUT)				- The  pBasic Field to be set.
**  		pErr(OUT)					- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_getBasicFromCredential 
#ifdef ANSI_PROTO
	(SipGenericCredential *pCr, 
	SIP_S8bit **ppBasic, 
	SipError *pErr)
#else
	( pCr, ppBasic , pErr )
	SipGenericCredential *pCr;
	SIP_S8bit **ppBasic; 
	SipError *pErr;	
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U16bit dLength;
#endif
	SIP_S8bit * pTemp_basic;
	SIPDEBUGFN( "Entering getBasicFromCredential");
#ifndef SIP_NO_CHECK	
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCr == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppBasic == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	
	if( pCr->dType != SipCredBasic  )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}	
#endif
	pTemp_basic = pCr->u.pBasic; 
	
	
	if( pTemp_basic == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
#ifdef SIP_BY_REFERENCE
	*ppBasic = pTemp_basic;
#else
	dLength = sip_strlen(pTemp_basic );
	*ppBasic = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1, pErr);
	if ( *ppBasic == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppBasic, pTemp_basic );
#endif
	SIPDEBUGFN( "Exitting getBasicFromCredential");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_setBasicInCredential
** 
** DESCRIPTION: This function sets the pBasic field in a SIP Credential
** PARAMETERS:
**			pCr(IN)					- SipGenericCredential
**			pBasic(IN)				- The  pBasic Field to be set.
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_setBasicInCredential 
#ifdef ANSI_PROTO
	(SipGenericCredential *pCr, 
	SIP_S8bit *pBasic, 
	SipError *pErr)
#else
	( pCr, pBasic , pErr )
	SipGenericCredential *pCr;
	SIP_S8bit *pBasic; 
	SipError *pErr;	 
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_basic;
#endif

	SipGenericCredential *pTemp_cr;
	SIPDEBUGFN( "Entering setBasicInCredential");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCr == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pCr->dType != SipCredBasic  )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif	
	pTemp_cr=pCr;
	
	if(pTemp_cr->u.pBasic !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pTemp_cr->u.pBasic), \
			pErr) ==SipFail)
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	pTemp_cr->u.pBasic = pBasic;
#else
	if( pBasic == SIP_NULL)
		pTemp_basic = SIP_NULL;
	else
	{
		dLength = sip_strlen( pBasic );
		pTemp_basic = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1, \
			pErr);
		if ( pTemp_basic == SIP_NULL )
			return SipFail;
		
		(void)sip_strcpy( pTemp_basic, pBasic );
	}
	
	pTemp_cr->u.pBasic = pTemp_basic; 
#endif
	SIPDEBUGFN( "Exitting setBasicInCredential");

	*pErr = E_NO_ERROR;
	
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_getChallengeFromCredential
** 
** DESCRIPTION: This function gets a SIP Challenge From SipCredential
** PARAMETERS:
**			ppCh(IN)				- SipGenericChallenge
**			pCr(IN)					- The  SipGenericCredential.
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_getChallengeFromCredential
#ifdef ANSI_PROTO
#ifdef SIP_BY_REFERENCE
	(SipGenericCredential *pCr, 
	SipGenericChallenge **ppCh, 
	SipError *pErr)
#else
	(SipGenericCredential *pCr, 
	SipGenericChallenge *pCh, 
	SipError *pErr)
#endif
#else
#ifdef SIP_BY_REFERENCE
	( pCr, ppCh, pErr )
	  SipGenericCredential *pCr;
	  SipGenericChallenge **ppCh;
	  SipError *pErr;
#else
	( pCr, pCh, pErr )
	  SipGenericCredential *pCr;
	  SipGenericChallenge *pCh;
	  SipError *pErr;
#endif
#endif
{
	SipGenericChallenge *pFrom;
	SIPDEBUGFN( "Entering getChallengeFromCredential");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if(pErr == SIP_NULL)
		return SipFail;
#ifdef SIP_BY_REFERENCE	
	if ( pCr == SIP_NULL||ppCh==SIP_NULL) 
#else
	if ( pCr == SIP_NULL||pCh==SIP_NULL) 
#endif
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pCr->dType != SipCredAuth )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}	
#endif
	pFrom=pCr->u.pChallenge;
	if (pFrom == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
	
#ifdef SIP_BY_REFERENCE
	HSS_LOCKEDINCREF(pFrom->dRefCount);
	*ppCh =	pFrom;
#else
	if(__sip_cloneChallenge(pCh, pFrom, pErr)==SipFail)
	{
		return SipFail;
	}
#endif
	SIPDEBUGFN( "Exitting getChallengeFromCredential ");
	
	*pErr=E_NO_ERROR;
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: sip_setChallengeInCredential
** 
** DESCRIPTION: This function sets a SIP Challenge in SipCredential
** PARAMETERS:
**			pCh(IN)				- SipGenericChallenge
**			pCr(OUT)			- The  SipGenericCredential.
**  		pErr(OUT)			- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/

SipBool sip_setChallengeInCredential 
#ifdef ANSI_PROTO
	(SipGenericCredential *pCr, 
	SipGenericChallenge *pCh, 
	SipError *pErr)
#else
	( pCr, pCh, pErr )
	SipGenericCredential *pCr;
	SipGenericChallenge *pCh;	
	SipError *pErr;
#endif
{
	SipGenericChallenge *pTo;
	SIPDEBUGFN( "Entering setChallengeInCredential");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if(pErr == SIP_NULL)
		return SipFail;

	if (pCr==SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pCr->dType != SipCredAuth )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}	
#endif
	pTo=pCr->u.pChallenge;		
	if (pCh==SIP_NULL)
	{
		if (pTo != SIP_NULL)
			sip_freeSipGenericChallenge(pTo);
		pCr->u.pChallenge= SIP_NULL;
	}
	else
	{
		if (pTo != SIP_NULL)
			sip_freeSipGenericChallenge(pTo);
		HSS_LOCKEDINCREF(pCh->dRefCount);
		pCr->u.pChallenge= pCh;
	}

	*pErr=E_NO_ERROR;
	SIPDEBUGFN( "Exitting setChallengeInCredential");
	return SipSuccess;

}
/*******************************************************************************
** FUNCTION: sip_getSchemeFromChallenge
** 
** DESCRIPTION: This function gets the Scheme  from  a  
**	SipGenericChallenge structure
** PARAMETERS:
**			pCh(IN)				- SipGenericChallenge
**			ppSch(OUT)			- The  Scheme.
**  		pErr(OUT)			- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/
SipBool sip_getSchemeFromChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge *pCh, SIP_S8bit **ppSch, SipError *pErr)
#else
	( pCh, ppSch, pErr )
	  SipGenericChallenge *pCh;
	  SIP_S8bit **ppSch;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
#endif
	SIP_S8bit * pTemp_sch;
	SIPDEBUGFN( "Entering getSchemeFromChallenge");
#ifndef SIP_NO_CHECK	
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;
	
	if ( pCh == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppSch == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	pTemp_sch = ((SipGenericChallenge *) pCh)->pScheme; 
	
	if( pTemp_sch == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
#ifdef SIP_BY_REFERENCE
	*ppSch = pTemp_sch;
#else
	dLength = sip_strlen(pTemp_sch);
	*ppSch = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, dLength+1, pErr);
	if ( *ppSch == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppSch , pTemp_sch );
#endif
    SIPDEBUGFN( "Exitting getSchemeFromChallenge");	

	*pErr = E_NO_ERROR;
	return SipSuccess;

}
/*******************************************************************************
** FUNCTION: sip_setSchemeInChallenge
** 
** DESCRIPTION: This function sets the Scheme  in a  SipGenericChallenge
**				structure
** PARAMETERS:
**			pCh(IN/OUT)				- SipContactparamStructure
**			pSch(OUT)				- The  Scheme.
**  		pErr(OUT)				- SipError
** ReturnValue :
**				SipBool
*******************************************************************************/

SipBool sip_setSchemeInChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge *pCh, SIP_S8bit *pSch, SipError *pErr)
#else
	( pCh, pSch, pErr )
	  SipGenericChallenge *pCh;
	  SIP_S8bit *pSch;
	  SipError *pErr;
#endif
{

#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_sch;
#endif
	SipGenericChallenge * pTemp_ch_hdr;
	SIPDEBUGFN( "Entering setSchemeInChallenge");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCh == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_ch_hdr= (SipGenericChallenge *)(pCh);
	
	if(pTemp_ch_hdr->pScheme !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pTemp_ch_hdr->pScheme), \
			pErr) ==SipFail)
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	((SipGenericChallenge *)(pCh))->pScheme = pSch;
#else
	if( pSch == SIP_NULL)
		pTemp_sch = SIP_NULL;
	else
	{
		dLength = sip_strlen( pSch );
		pTemp_sch = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, \
			dLength+1, pErr);
		if ( pTemp_sch== SIP_NULL )
			return SipFail;
		
		(void)sip_strcpy( pTemp_sch, pSch );

	}
	
	pTemp_ch_hdr->pScheme = pTemp_sch; 
#endif  
	SIPDEBUGFN( "Exitting setSchemeInChallenge");
		
	*pErr = E_NO_ERROR;
	return SipSuccess;

}
#endif


/***************************************************************
** FUNCTION: sip_getQvalueFromContactParam
**
** DESCRIPTION: This function retrieves the pQValue-pValue field from a
**		SIP contact-param
**
***************************************************************/

SipBool sip_getQvalueFromContactParam
#ifdef ANSI_PROTO
	(SipContactParam *pCp,
	SIP_S8bit **ppQValue,
	SipError *pErr)
#else
	( pCp, ppQValue , pErr )
	  SipContactParam *pCp;  /* ContactParam pHeader */
	  SIP_S8bit **ppQValue;
	  SipError * pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U16bit dLength;
#endif
	SIP_S8bit * pTemp_qvalue;
	SIPDEBUGFN( "Entering getQValueFromContactParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCp == SIP_NULL || ppQValue == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pCp->dType != SipCParamQvalue )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif

	pTemp_qvalue = pCp->u.pQValue;


	if( pTemp_qvalue == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	*ppQValue = pTemp_qvalue;
#else
	dLength = sip_strlen(pTemp_qvalue );
	*ppQValue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
	if ( *ppQValue == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppQValue, pTemp_qvalue );
#endif
	SIPDEBUGFN( "Exitting getQValueFromContactParam");
	*pErr = E_NO_ERROR;


	return SipSuccess;
}
/***************************************************************
** FUNCTION: sip_setQvalueInContactParam
**
** DESCRIPTION: This function sets the pQValue-pValue field in a SIP
**		contact-param
**
***************************************************************/

SipBool sip_setQvalueInContactParam
#ifdef ANSI_PROTO
	(SipContactParam *pCp,
	SIP_S8bit *pQValue,
	SipError *pErr)
#else
	( pCp, pQValue , pErr )
	  SipContactParam *pCp; /* ContactParam pHeader */
	  SIP_S8bit *pQValue;
	  SipError * pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_qvalue;
#endif
	SIPDEBUGFN( "Entering setQValueInContactParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCp == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if( pCp->dType != SipCParamQvalue )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if(pCp->u.pQValue !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pCp->u.pQValue),pErr) ==SipFail)
		return SipFail;
	}

#ifdef SIP_BY_REFERENCE
	pCp->u.pQValue = pQValue;
#else
	if( pQValue == SIP_NULL)
		pTemp_qvalue = SIP_NULL;
	else
	{
		dLength = sip_strlen( pQValue );
		pTemp_qvalue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);
		if ( pTemp_qvalue == SIP_NULL )
			return SipFail;

		(void)sip_strcpy( pTemp_qvalue, pQValue );
	}
	pCp->u.pQValue = pTemp_qvalue;
#endif

	SIPDEBUGFN( "Exitting setQValueInContactParam");
	*pErr = E_NO_ERROR;

	return SipSuccess;
}

#ifdef SIP_3GPP

/***************************************************************
** FUNCTION: sip_getFeatureAttrFromContactParam
**
** DESCRIPTION: This function retrieves the pFeatureAttribute-pValue field from a
**		SIP contact-param
**
***************************************************************/


SipBool sip_getFeatureAttrFromContactParam
#ifdef ANSI_PROTO
	(SipContactParam *pCp, 
	SIP_S8bit **ppFeatureAttribute, 
	SipError *pErr)
#else
	( pCp, ppFeatureAttribute , pErr )
	  SipContactParam *pCp;  
	  SIP_S8bit **ppFeatureAttribute;
	  SipError * pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U16bit dLength;
#endif
	SIP_S8bit * pTemp_FeatureAttr;
	SIPDEBUGFN( "Entering getFeatureAttrFromContactParam");
#ifndef SIP_NO_CHECK	
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCp == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppFeatureAttribute == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	
	if( pCp->dType != SipCParamFeature )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}	
#endif
	pTemp_FeatureAttr = pCp->u.pFeatureAttr; 
	
	
	if( pTemp_FeatureAttr == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
#ifdef SIP_BY_REFERENCE
	*ppFeatureAttribute = pTemp_FeatureAttr;
#else
	dLength = sip_strlen(pTemp_FeatureAttr );
	*ppFeatureAttribute = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, \
		dLength+1, pErr);
	if ( *ppFeatureAttribute == SIP_NULL )
		return SipFail;

	(void)sip_strcpy( *ppFeatureAttribute, pTemp_FeatureAttr );
#endif
	SIPDEBUGFN( "Exitting getFeatureAttrFromContactParam");
	*pErr = E_NO_ERROR;
	
	return SipSuccess;
}


/***************************************************************
** FUNCTION: sip_setFeatureAttrInContactParam
**
** DESCRIPTION: This function sets the pFeatureAttribute-pValue field in a SIP
**		contact-param
**
***************************************************************/

/*sip_setFeatureAttrInContactParam:- */

SipBool sip_setFeatureAttrInContactParam
#ifdef ANSI_PROTO
	(SipContactParam *pCp, 
	SIP_S8bit *pFeatureAttribute, 
	SipError *pErr)
#else
	( pCp, pFeatureAttribute , pErr )
	  SipContactParam *pCp; 
	  SIP_S8bit *pFeatureAttribute;
	  SipError * pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_U32bit dLength;
	SIP_S8bit * pTemp_FeatureAttr;
#endif
	SIPDEBUGFN( "Entering setFeatureAttributeInContactParam");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pCp == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pCp->dType != SipCParamFeature )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}	
#endif
	
	if(pCp->u.pFeatureAttr !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pCp->u.pFeatureAttr), \
			pErr) ==SipFail)
		return SipFail;
	}
#ifdef SIP_BY_REFERENCE
	pCp->u.pFeatureAttr = pFeatureAttribute;
#else
	if( pFeatureAttribute == SIP_NULL)
		pTemp_FeatureAttr = SIP_NULL;
	else
	{
		dLength = sip_strlen( pFeatureAttribute );
		pTemp_FeatureAttr = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, \
			dLength+1, pErr);
		if ( pTemp_FeatureAttr == SIP_NULL )
			return SipFail;
		
		(void)sip_strcpy( pTemp_FeatureAttr, pFeatureAttribute );
	}


	pCp->u.pFeatureAttr = pTemp_FeatureAttr; 
#endif
	SIPDEBUGFN( "Exitting setFeatureAttributeInContactParam");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#endif



/*****************************************************************
**
** FUNCTION:  sip_insertContactParamAtIndexInContactHdr
**
** DESCRIPTION: This function inserts a contact param at a spec.
**		index_1 in a SIP Contact pHeader
**
***************************************************************/
SipBool sip_insertContactParamAtIndexInContactHdr(SipHeader *hdr, \
		SipContactParam *contact_param, SIP_U32bit index_1, SipError *pError)
{
	SipParam *pSipParam 				= SIP_NULL;
	SIP_S8bit* pValue					= SIP_NULL;
	SIP_U32bit slen						= 0;
#ifndef SIP_NO_CHECK
	if( pError == SIP_NULL)
		return SipFail;
	if( hdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if( hdr->pHeader == SIP_NULL)
	{
		*pError = E_INV_HEADER;
		return SipFail;
	}
	if(( hdr->dType != SipHdrTypeContactNormal)&&(hdr->dType != \
				SipHdrTypeContactWildCard))
	{
		*pError = E_INV_TYPE;
		return SipFail;
	}
#endif
	if (SIP_NULL == contact_param)
	{
		pSipParam = SIP_NULL;
		if (sip_listInsertAt(&(((SipCommonHeader *)(hdr->pHeader))->slParam),
			index_1, (SIP_Pvoid)pSipParam, pError) == SipFail)
		{
			return SipFail;
		}
		return SipSuccess;
	}

	if(SipFail == sip_initSipParam(&pSipParam, pError))
		return SipFail;
	if (validateSipContactParamsType(contact_param->dType, pError) == \
		SipFail)
		return SipFail;
	switch(contact_param->dType)
	{
		case SipCParamAction:
		{
			pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
				(SIP_S8bit*) "action", 6, pError);
			slen = sip_strlen(contact_param->u.pAction);
			/*
			 * Getting the string  and setting it at SipList of value
			 */
			pValue = sip_nStrdup(ACCESSOR_MEM_ID, contact_param->u.pAction, \
					slen, pError);
			if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValue, pError) == SipFail)
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}

			break;
		}
		case SipCParamExpires:
		{
#ifdef SIP_DATE
			if (contact_param->u.pExpire->dIntVar1 == SipExpSeconds)
			{
				SIP_S8bit arr[20];
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
				(void)sip_snprintf(arr, 20, "%u", \
						contact_param->u.pExpire->u.dSec);
				pValue = sip_strdup(arr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			}
			if (contact_param->u.pExpire->dIntVar1 == SipExpDate)
			{
				SIP_S8bit *pArr = SIP_NULL;
				SIP_U32bit i;
				pArr = (SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID, 50 , pError);
				for (i=0; i < 50; i++)
					pArr[i] = ' ';
				pArr[0]= '\0';
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
                (void)STR_CAT(pArr, (SIP_S8bit *)"\"");
				if (SipFail == sip_formDateStruct(&pArr,contact_param->\
							u.pExpire->u.pDate, pError))
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				
				}	
                (void)STR_CAT(pArr, (SIP_S8bit *)"\"");
				pValue = sip_strdup(pArr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			
			}
#else
			{
				SIP_S8bit arr[20];

				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, \
					(SIP_S8bit*)"expires", 7, pError);
				(void)sip_snprintf(arr, 20, "%u", \
						contact_param->u.pExpire->dIntVar1);
				pValue = sip_strdup(arr, ACCESSOR_MEM_ID);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
					(SIP_Pvoid)pValue, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}
			}
#endif /* SIP_DATE */
			break;
		}
		case SipCParamQvalue:
		{
			pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, (SIP_S8bit*)"q", \
					1, pError);
			slen = sip_strlen(contact_param->u.pQValue);
			/*
			 * Getting the string  and setting it at SipList of value
			 */
			pValue = sip_nStrdup(ACCESSOR_MEM_ID, contact_param->u.pQValue, \
					slen, pError);
			if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValue, pError) == SipFail)
			{
				sip_freeSipParam(pSipParam);
				return SipFail;
			}

			break;
		}
		case SipCParamExtension:
		{
			SIP_S8bit *pTmpExt = SIP_NULL;
			SIP_S8bit *pNameSt = SIP_NULL;
			SIP_S8bit *pValueSt = SIP_NULL;
			SIP_U32bit len = 0;
			
				pTmpExt = contact_param->u.pExtensionAttr;
				len = sip_strlen(contact_param->u.pExtensionAttr);
				pNameSt = sip_tokenize(pTmpExt,pTmpExt+len , '=' );
				if (pNameSt == SIP_NULL)
					return SipFail;
				
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 pNameSt- pTmpExt, pError);
				pValueSt = sip_nStrdup(ACCESSOR_MEM_ID, pNameSt+1,
					(pTmpExt+len) - (pNameSt), pError);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValueSt, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}



			break;
		}

#ifdef SIP_3GPP
		case SipCParamFeature:
		{
			SIP_S8bit *pTmpExt = SIP_NULL;
			SIP_S8bit *pNameSt = SIP_NULL;
			SIP_S8bit *pValueSt = SIP_NULL;
			SIP_U32bit len = 0;
			
				pTmpExt = contact_param->u.pFeatureAttr;
				len = sip_strlen(contact_param->u.pFeatureAttr);
				pNameSt = sip_tokenize(pTmpExt,pTmpExt+len , '=' );
				if (pNameSt == SIP_NULL)
                                /*Fix from UATK 2.2 Start*/
				{
					pNameSt = pTmpExt;
					pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 len, pError);
					break;
				}
				/*Fix from UATK 2.2 End*/
				
				
				pSipParam->pName = sip_nStrdup(ACCESSOR_MEM_ID, pTmpExt,
					 pNameSt- pTmpExt, pError);
				pValueSt = sip_nStrdup(ACCESSOR_MEM_ID, pNameSt+1,
					(pTmpExt+len) - (pNameSt), pError);
				if (sip_listInsertAt(&(pSipParam->slValue),0, \
				(SIP_Pvoid)pValueSt, pError) == SipFail)
				{
					sip_freeSipParam(pSipParam);
					return SipFail;
				}



			break;
		}

#endif



		default:;
	}
	
	if (sip_listInsertAt( &(((SipCommonHeader *)(hdr->pHeader))->slParam), 
		index_1, (SIP_Pvoid)pSipParam, pError) == SipFail)
	{
		sip_freeSipParam(pSipParam);
		return SipFail;
	}
	return SipSuccess;

}
/***************************************************************
** FUNCTION: sip_insertAuthorizationParamAtIndexInChallenge
**
** DESCRIPTION: This function inserts an authorization-param at a
**		specified index_1 in a SIp Challenge
**
***************************************************************/
#ifdef SIP_AUTHENTICATE
SipBool sip_insertAuthorizationParamAtIndexInChallenge
#ifdef ANSI_PROTO
	( SipGenericChallenge 	*pSch,
	  SipParam	*pParam,
	  SIP_U32bit 	dIndex,
	  SipError 	*pErr )
#else
	( pSch,pParam,dIndex,pErr)
	  SipGenericChallenge 	*pSch;
	  SipParam	*pParam;
	  SIP_U32bit 	dIndex;
	  SipError 	*pErr;
#endif
{
	SipParam 	*pElement_in_list;

	SIPDEBUGFN( "Entering InsertAuthorizationParamAtIndexInChallenge");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pSch == SIP_NULL )
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
        (void)sip_initSipParam(&pElement_in_list,pErr);
		if ( __sip_cloneSipParam(pElement_in_list, pParam,pErr) == SipFail)
		{
			 sip_freeSipParam(pElement_in_list);
			return SipFail;
		}
#endif
	}

	if( sip_listInsertAt(&(pSch->slParam), dIndex, pElement_in_list, pErr)\
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

	SIPDEBUGFN( "Exitting InsertAuthorizationParamAtIndexInChallenge");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
#endif
#endif /* SIP_CORE_STACK_COMPATIBLE */

/*****************************************************************
**
** FUNCTION:  sip_setSecondsInExpires
**
** DESCRIPTION: This function sets the seconds field in a SIP Expires
**		Struct
**
***************************************************************/
SipBool sip_setSecondsInExpires
#ifdef ANSI_PROTO
	(SipExpiresStruct *hdr, SIP_U32bit dSec, SipError *err)
#else
	(hdr, dSec, err)
	SipExpiresStruct *hdr;
	SIP_U32bit dSec;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_setSecondsInExpires");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#ifdef SIP_DATE
	if( hdr->dIntVar1 != SipExpSeconds)
	{
		*err = E_INV_TYPE;
		return SipFail;
	}
#endif /* SIP_DATE */
#endif

#ifdef SIP_DATE
	hdr->u.dSec = dSec;
#else
	hdr->dIntVar1 = dSec;
#endif

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setSecondsInExpires");
	return SipSuccess;
}

#ifdef STRCAT
/*****************************************************************
**
** FUNCTION:  sip_getSecondsFromExpires
**
** DESCRIPTION: This function retrieves the seconds field from a SIP
**		Expires Struct
**
***************************************************************/
SipBool sip_getSecondsFromExpires
#ifdef ANSI_PROTO
	(SipExpiresStruct *hdr, SIP_U32bit *dSec, SipError *err)
#else
	(hdr, dSec, err)
	SipExpiresStruct *hdr;
	SIP_U32bit *dSec;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_getSecondsFromExpires");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( dSec == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#ifdef SIP_DATE
	if( hdr->dIntVar1 != SipExpSeconds)
	{
		*err = E_INV_TYPE;
		return SipFail;
	}
#endif /* SIP_DATE */
#endif

#ifdef SIP_DATE
	*dSec = hdr->u.dSec;
#else
	*dSec = hdr->dIntVar1;
#endif

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getSecondsFromExpires");
	return SipSuccess;
}
#endif


#ifdef SIP_DATE
/* Will be replaced. Why SIP_BY_REFERENCE is used here */
/*****************************************************************
**
** FUNCTION:  sip_getDateStructFromExpires
**
** DESCRIPTION: This function retrieves the Date from a SIP Expires
**
**
***************************************************************/
SipBool sip_getDateStructFromExpires
	(SipExpiresStruct *pHeader, SipDateStruct **ppDateStruct, SipError *pErr)
{
	SipHeader Hdr;
	SIPDEBUGFN("Entering function sip_getDateStructFromExpires");

	Hdr.dType = SipHdrTypeExpiresDate;
	Hdr.pHeader = pHeader;

	if(SipFail == sip_getDateFromCommonHdr(&Hdr,ppDateStruct,pErr))
	{
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getDateStructFromExpires");
	return SipSuccess;
}

/*****************************************************************
**
** FUNCTION:  sip_setDateStructInExpires
**
** DESCRIPTION: This function sets the dDate in a SIP Expires Struct
**
***************************************************************/
SipBool sip_setDateStructInExpires
	(SipExpiresStruct 	*pHeader, 
	 SipDateStruct 		*pDateStruct, 
	 SipError 			*pErr)
{
	SipHeader Hdr;
	SIPDEBUGFN("Entering function sip_setDateStructInExpires");
	Hdr.dType = SipHdrTypeExpiresDate;
	Hdr.pHeader = pHeader;
    
	if(SipFail == sip_setDateInCommonHdr(&Hdr,pDateStruct,pErr))
	{
		return SipFail;
	}
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setDateStructInExpires");
	return SipSuccess;
}
#endif /* SIP_DATE */


/*****************************************************************
**
** FUNCTION:  sip_setSecondsInExpiresHdr
**
** DESCRIPTION: This function sets the seconds field in a SIP Expires
**		pHeader
**
***************************************************************/
SipBool sip_setSecondsInExpiresHdr
#ifdef ANSI_PROTO
	(SipHeader *hdr, SIP_U32bit dSec, SipError *err)
#else
	(hdr, dSec, err)
	SipHeader *hdr;
	SIP_U32bit dSec;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_setSecondsInExpiresHdr");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#ifdef SIP_DATE
	if ((hdr->dType != SipHdrTypeExpiresDate)&& \
		(hdr->dType != SipHdrTypeExpiresSec))
#else
	if (hdr->dType != SipHdrTypeExpires)
#endif /* SIP_DATE */
	{
		*err = E_INV_TYPE;
		return SipFail;
	}
	if( hdr->pHeader == SIP_NULL)
	{
		*err = E_INV_HEADER;
		return SipFail;
	}
#endif

#ifdef SIP_DATE
	(((SipCommonHeader *)(hdr->pHeader))->u).dSec = dSec;
#else
	((SipCommonHeader *)(hdr->pHeader))->dIntVar1  = dSec;
#endif /* SIP_DATE */

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setSecondsInExpiresHdr");
	return SipSuccess;
}

/*#ifdef STK_RELEASE*/
/*****************************************************************
**
** FUNCTION:  sip_getSecondsFromExpiresHdr
**
** DESCRIPTION: This function retrieves the seconds field from a SIP
**		Encryption pHeader
**
***************************************************************/
SipBool sip_getSecondsFromExpiresHdr
	(SipHeader *hdr, SIP_U32bit *dSec, SipError *err)
{
	SIPDEBUGFN("Entering function sip_getSecondsFromExpiresHdr");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( dSec == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#ifdef SIP_DATE
	if(( hdr->dType != SipHdrTypeExpiresDate)&& \
		(hdr->dType != SipHdrTypeExpiresSec))
#else
	if (hdr->dType != SipHdrTypeExpires)
#endif
	{
		*err = E_INV_TYPE;
		return SipFail;
	}
	if( hdr->pHeader == SIP_NULL)
	{
		*err = E_INV_HEADER;
		return SipFail;
	}
#endif

#ifdef SIP_DATE
	*dSec = (((SipCommonHeader *)(hdr->pHeader))->u).dSec;
#else
	*dSec = ((SipCommonHeader *)(hdr->pHeader))->dIntVar1;
#endif

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getSecondsFromExpiresHdr");
	return SipSuccess;
}
/*#endif*/

#ifdef SIP_CORE_STACK_COMPATIBLE
#ifdef SIP_REFER
/* SS: Strdup is used here. */
/*********************************************************
** FUNCTION:sip_getMsgIdFromReferredByHdr
**
** DESCRIPTION: This function retrieves the MsgId field
**		from a SIP ReferredBy pHeader
**
**********************************************************/
SipBool sip_getMsgIdFromReferredByHdr
	(SipHeader *pHdr,
	SIP_S8bit **ppMsgId,
	SipError *pErr)
{
	SIP_S8bit *pStringData = SIP_NULL;
	
	SIPDEBUGFN( "Entering getMsgIdFromReferredByHdr");

    if(SipFail == sip_getParamForNameFromCommonHdr(pHdr,"cid",&pStringData,pErr))
    {
        return SipFail;
    }
	if (pStringData == SIP_NULL)
 	{
 		*pErr = E_NO_EXIST;
 		return SipFail;
 	}

	/*
	 * Now, make a copy of the cid param found and return to the user in 
	 * the field provided (ppMsgId). The user must do an explicit setMsgId
	 * if he wishes to make changes to this field.
	 */

	*ppMsgId = STRDUPACCESSOR(pStringData);
	if (SIP_NULL == *ppMsgId)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	SIPDEBUGFN( "Exitting getMsgIdFromReferredByHdr");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#endif /* SIP_REFER */
#endif /* SIP_CORE_STACK_COMPATIBLE */


/*****************************************************************
**
** FUNCTION:  sip_getParamCountForNameFromCommonHdr
**
** DESCRIPTION: This FUNCTION gets the number of params with given "Name"
**		        In SIP Common pHeader
** PARAMETERS:
**			pHdr(IN) 			- Pointer to SipHeader
**          pName(IN)           - The "Name" for which count is to be retrieved
**			pCount(OUT)			- The Count to retrieve
**			pErr(OUT)			- Error if any 
**
**************************************************************/
SipBool sip_getParamCountForNameFromCommonHdr
	(SipHeader  *pHdr, 
     SIP_S8bit  *pName,
     SIP_U32bit *pCount, 
     SipError   *pErr)
{
	SIP_U32bit size = 0;
	SIP_U32bit dIndex= 0;
	SipParam   *pSipParam = SIP_NULL;

	SIPDEBUGFN("Entering FUNCTION sip_getParamCountForNameFromCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pCount == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf( &(((SipCommonHeader *)(pHdr->pHeader))->slParam), &size , 
                pErr) == SipFail )
		return SipFail;
	*pCount = 0;
	while (dIndex<size)
	 {
		if (SipFail == sip_listGetAt(&(((SipCommonHeader *)(pHdr->pHeader))->slParam) ,
                    dIndex, (void**)&pSipParam, pErr))
			return SipFail;
		/*Checking if pParam->pName == pName */
		if ( 0 == sip_strcasecmp( pSipParam->pName, pName ))
		{
			(*pCount)++;
		}
		dIndex++;	
	 } /* End of While LOOP */
	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting FUNCTION sip_getParamCountForNameFromCommonHdr");
	return SipSuccess;
}

/***********************************************************************
** FUNCTION: sip_getParamForNameFromCommonHdr
** DESCRIPTION:gets the param at an index from the Sip Common Header
** PARAMETERS:
**			pHdr(IN) 			- Sip Common Header
**			pName(IN)			- The name for which param value to be retrieved
**			ppValue(OUT)		- The param value for param name
**			pErr(OUT)			- error value if any
**
************************************************************************/
SipBool sip_getParamForNameFromCommonHdr 
    (SipHeader  *pHdr,
     SIP_S8bit  *pName,
     SIP_S8bit  **ppValue,
     SipError   *pErr)
{
    SIP_U32bit size = 0;
    SIP_U32bit dIndex= 0;
    SipCommonHeader *pCommonHdr = SIP_NULL;
    SipParam    *pSipParam = SIP_NULL;

#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pName == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

    pCommonHdr = (SipCommonHeader *)(pHdr->pHeader);
	/*Finding the size of slParam*/
	if( SipFail == sip_listSizeOf(&(pCommonHdr->slParam), 
                &size, 
                pErr) )
		return SipFail;

	 while (dIndex<size)
	 {
		if (SipFail == sip_listGetAt( &(pCommonHdr->slParam), dIndex,
			(void **)&pSipParam, pErr ) )
			return SipFail;

		/*Checking if pParam->pName == pName */
		if ( 0 == sip_strcasecmp( pSipParam->pName, pName ))
		{
			if (SipFail ==sip_listGetAt( &(pSipParam->slValue),
				0, (void **)ppValue, pErr ))
			{
				*pErr = E_NO_EXIST;
				return SipFail;
			}

			if(*ppValue == SIP_NULL)
			{
				*pErr = E_NO_EXIST;
				return SipFail;
			}	
			break;
		}
		dIndex++;	
	 } /* End of While LOOP */

	return SipSuccess;
}
	
/***********************************************************************
** FUNCTION: sip_setParamForNameInCommonHdr
** DESCRIPTION:sets a value for a param name in the Sip Common Header
** PARAMETERS:
**			pHdr(IN/OUT)		- Sip From Header
**			tag(IN)				- The Tag to set
**			index(IN)			- The index at which the tag is set
**			err(OUT)			- Possible error value(see API ref doc)
**
************************************************************************/
SipBool sip_setParamForNameInCommonHdr
	(SipHeader *pHdr, 
     SIP_S8bit *pName,
     SIP_S8bit *pValue,
     SipError  *pErr)
{
	en_SipBoolean	bool_found = SipFalse;
	SipList			*pTmpList;	
	SipListElement  *pElement;
	SIP_S8bit		*pTempValue;

	SIPDEBUGFN("Entering FUNCTION sip_setParamForNameInCommonHdr");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pValue == SIP_NULL)
		pTempValue = SIP_NULL;
	else
	{
		pTempValue = STRDUPACCESSOR(pValue);
	}

	pTmpList = &(((SipCommonHeader *)(pHdr->pHeader))->slParam);

	 if (pTmpList == SIP_NULL)
	 {
		sip_freeString(pTempValue);
		*pErr = E_NO_EXIST;
		return SipFail;
	 }

	/* First search for tag param */
	 for (pElement=pTmpList->head; pElement!=SIP_NULL; pElement=pElement->next)
	 {
		SipParam *pParam = (SipParam *)(pElement->pData);

		if (0 == sip_strcasecmp(pParam->pName, pName))
		{
			/* 
			 * param name already exists; free the previous value
			 * of this param and set the new value by value. The user
			 * is expected to free the value field passed to this function
			 * after return.
			 */

			bool_found = SipTrue;
			if (SipFail == sip_listSetAt(&(pParam->slValue), 
                        0, (void *)pTempValue, pErr))
			{
				sip_freeString(pTempValue);
				return SipFail;
			}
			break;
		}
	 }

	 if (SipFalse == bool_found)
	 {
		/* 
		 * Create a SipParam and append it to the list of params
		 */
		
		SipParam	*pNewParam;

		if (SipFail == sip_initSipParam(&pNewParam, pErr))
		{
			sip_freeString(pTempValue);
			return SipFail;
		}

		pNewParam->pName = (SIP_S8bit *)STRDUPACCESSOR(pName);
		if (SIP_NULL == pNewParam->pName)
		{
			sip_freeSipParam(pNewParam);
			sip_freeString(pTempValue);
			return SipFail;
		}

		if (SipFail == sip_listAppend(&(pNewParam->slValue), 
			(SIP_Pvoid)pTempValue, pErr))
		{
			sip_freeSipParam(pNewParam);
			sip_freeString(pTempValue);
			return SipFail;
		}

		if (SipFail == sip_listAppend(pTmpList, (SIP_Pvoid)pNewParam, pErr))
		{	
			sip_freeSipParam(pNewParam);
			return SipFail ;
		}
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting FUNCTION sip_setParamForNameInCommonHdr");
	return SipSuccess;
}


/***********************************************************************
** FUNCTION: sip_deleteParamForNameInCommonHdr
** DESCRIPTION:deletes the param with given name in the Sip Common Header
** PARAMETERS:
**			pHdr(IN/OUT) 		- Sip Common Header
**          pName(IN)           - The "Name" for which the param is to be
**                                deleted
**			pErr(OUT)			- error code in case of error
**
************************************************************************/
SipBool sip_deleteParamForNameInCommonHdr 
        (SipHeader *pHdr,
         SIP_S8bit *pName,
         SipError  *pErr)
{
	SIP_U32bit	    lcl_index;
	en_SipBoolean	bool_found = SipFalse;
	SipList			*pTmpList;	
	SipListElement  *pElement;

	SIPDEBUGFN("Entering FUNCTION sip_deleteParamForNameInCommonHdr");

#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL)
		return SipFail;
	if( pHdr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pHdr->dType > SipHdrTypeAny)
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
	if( pHdr->pHeader == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTmpList = &(((SipCommonHeader *)(pHdr->pHeader))->slParam);

	 if (pTmpList == SIP_NULL)
	 {
		*pErr = E_NO_EXIST;
		return SipFail;
	 }

	/* First search for the param with "pName" */
	 lcl_index=0;
	 for (pElement=pTmpList->head; pElement!=SIP_NULL; pElement=pElement->next)
	 {
		SipParam *pParam = (SipParam *)(pElement->pData);

		if (0 == sip_strcasecmp(pParam->pName, pName))
		{
			bool_found = SipTrue;
			if (SipFail == sip_listDeleteAt(pTmpList, lcl_index, pErr))
				return SipFail;
			break;
		}
		else
			lcl_index++;
	 }

	 if (SipFalse == bool_found)
	 {
		 *pErr = E_NO_EXIST;
		 return SipFail;
	 }

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting FUNCTION sip_deleteParamForNameInCommonHdr");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sip_deleteAllHeaderType
**
** DESCRIPTION: This function deletes all headers of en_HeaderType 
** "dType" from a SipMessage "msg".
**
*********************************************************************/
SipBool sip_deleteAllHeaderType
	(SipMessage *msg, en_HeaderType dType, SipError *err)
{
	SIP_U32bit rep, count;
	SipHeaderOrderInfo	*temp_order;

	SIPDEBUGFN("Entering function sip_deleteAllHeaderType");

#ifndef SIP_NO_CHECK
	if (err == SIP_NULL)
		return SipFail;

	if (msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	if (validateSipHeaderType (dType, err) == SipFail)
		return SipFail;

	switch (msg->dType)
	{
		case	SipMessageRequest	:
			 if (isSipGeneralHeader(dType) == SipTrue)
			 {
				if (msg->pGeneralHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}	
				else
				{
					if (deleteAllGeneralHdr(msg->pGeneralHdr, dType, err) == SipFail)
						return SipFail;
				}
			 }
			 else if (isSipReqHeader(dType) == SipTrue)
			 {
				if ((msg->u).pRequest == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				 }
				if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}	
				else
				{
					if (deleteAllRequestHdr(((msg->u).pRequest)->pRequestHdr, \
						dType, err) == SipFail)
						return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

		case	SipMessageResponse	:
			 if (isSipGeneralHeader(dType) == SipTrue)
			 {
				if (msg->pGeneralHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				else
				{
					if (deleteAllGeneralHdr(msg->pGeneralHdr, dType, err) == SipFail)
						return SipFail;
				}
			 }
			 else if (isSipRespHeader(dType) == SipTrue)
			 {
				if ((msg->u).pResponse == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				if (((msg->u).pResponse)->pResponseHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				else
				{
					if (deleteAllResponseHdr(((msg->u).pResponse)->pResponseHdr, \
						dType, err) == SipFail)
						return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

		default	: 
			*err = E_INV_TYPE;
			 return SipFail;
	}

	/* updating order table */
	if (sip_getHeaderLineCount( msg, &count, err) == SipFail)
		return SipFail;

	for (rep = 0; rep < count; rep++)
	{
		if (sip_listGetAt( &(msg->slOrderInfo), rep, (SIP_Pvoid*)(&temp_order), \
			err) == SipFail)
			return SipFail;

		if (temp_order->dType == dType)
		{
			if (sip_listDeleteAt(&(msg->slOrderInfo), rep,err) == SipFail)
				return SipFail;

			rep--;
			count--;
		}
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_deleteAllHeaderType");
	return SipSuccess;
}


/******************************************************************
**
** FUNCTION:  deleteAllGeneralHdr
**
** DESCRIPTION: This function deletes all general headers of
**	en_HeaderType "dType".
**
******************************************************************/
SipBool deleteAllGeneralHdr
	(SipGeneralHeader *hdr, en_HeaderType dType, SipError *err)
{
	SIP_Pvoid contact;
	SIP_U32bit count, dIndex;
	SipList	*pList = SIP_NULL;
	SipCommonHeader **ppHeader = SIP_NULL;
	SIPDEBUGFN("Entering function deleteAllGeneralHdr");

	switch (dType)
	{
		case SipHdrTypeAllow:
			pList = &(hdr->slAllowHdr);
			break;
		case SipHdrTypeContentDisposition:
			pList = &(hdr->slContentDispositionHdr);
			break;
		case SipHdrTypeCallId:
			ppHeader = &(hdr->pCallidHdr);
			break;
		case SipHdrTypeCseq:
			ppHeader = &(hdr->pCseqHdr);
			break;
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
			pList = &(hdr->slPEarlyMediaHdr);
			break;
#endif
#ifdef SIP_DATE
		case SipHdrTypeExpiresSec:
			ppHeader = &(hdr->pExpiresHdr);
			 break;
		case SipHdrTypeExpiresDate:
			if (hdr->pExpiresHdr != SIP_NULL)
			{
				if ( ((hdr->pExpiresHdr)->dIntVar1) == SipExpDate )
				{
					sip_freeSipCommonHeaderWithDate(hdr->pExpiresHdr);
					hdr->pExpiresHdr = SIP_NULL;
				}
			}
			break;
		case SipHdrTypeExpiresAny:
			ppHeader = &(hdr->pExpiresHdr);
			break;
#else
		case SipHdrTypeExpires:
			ppHeader = &(hdr->pExpiresHdr);
			 break;
#endif /* SIP_DATE */

#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
			 ppHeader = &(hdr->pMinSEHdr);
			break;

		case SipHdrTypeSessionExpires:
			 ppHeader = &(hdr->pSessionExpiresHdr);
			break;
#endif

		case SipHdrTypeFrom:
			 ppHeader = &(hdr->pFromHeader);
			break;
		case SipHdrTypeRecordRoute:
			pList = &(hdr->slRecordRouteHdr);
			break;
#ifdef SIP_3GPP
		case SipHdrTypePath:
			pList = &(hdr->slPathHdr);
			break;
		case SipHdrTypePanInfo:
			ppHeader = &(hdr->pPanInfoHdr);
			break;
		case SipHdrTypePcfAddr:
			ppHeader = &(hdr->pPcfAddrHdr);
			break;
		case SipHdrTypePcVector:
			ppHeader = &(hdr->pPcVectorHdr);
			break;
		case SipHdrTypeHistoryInfo:
			pList = &(hdr->slHistoryInfoHdr);
			break;

#endif
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
			ppHeader = &(hdr->pTimeStampHdr);
			break;
#endif			
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
			ppHeader = &(hdr->pPrivacyHdr);
			break;
		case SipHdrTypePPreferredId:
			pList = &(hdr->slPPreferredIdHdr);
			break;
		case SipHdrTypePAssertId: 
			pList = &(hdr->slPAssertIdHdr);
			break;
#endif
		case SipHdrTypeTo:
			ppHeader = &(hdr->pToHdr);
			break;

		case SipHdrTypeVia:
			pList = &(hdr->slViaHdr);
			 break;

		case SipHdrTypeContentEncoding:
			pList = &(hdr->slContentEncodingHdr);
			break;

		case SipHdrTypeSupported:
			pList = &(hdr->slSupportedHdr);
			break;

		case SipHdrTypeContentLength:
			ppHeader = &(hdr->pContentLengthHdr);
			break;

		case SipHdrTypeContentType:
			ppHeader = &(hdr->pContentTypeHdr);
			break;

		case SipHdrTypeMimeVersion:
			ppHeader = &(hdr->pMimeVersionHdr);
			break;

		case SipHdrTypeContactWildCard:
		case SipHdrTypeContactNormal:
			if (sip_listSizeOf(&(hdr->slContactHdr), &count, err) == SipFail)
				return SipFail;
			for (dIndex=0; dIndex<count; dIndex++)
			{
				if (sip_listGetAt(&(hdr->slContactHdr), dIndex, &contact, err)\
					== SipFail)
					return SipFail;
				if (((dType == SipHdrTypeContactWildCard) && 
							( ((SipCommonHeader *)contact)->dIntVar1 == \
					SipContactWildCard)) ||
				   (((dType == SipHdrTypeContactNormal) && 
							( ((SipCommonHeader *)contact)->dIntVar1 == \
					SipContactNormal))))
				{
					if (sip_listDeleteAt(&(hdr->slContactHdr), dIndex, err)\
						== SipFail)
				    {
						contact = SIP_NULL;
						return SipFail;
					}
				}

			}
			break;

		case SipHdrTypeContactAny:
			pList = &(hdr->slContactHdr);
			break;

		case SipHdrTypeRequire:
			pList = &(hdr->slRequireHdr);
			break;

#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate:
			if (hdr->pRetryAfterHdr != SIP_NULL)
			{
				if ((hdr->pRetryAfterHdr)->dIntVar1 == SipExpDate)
				{
					sip_freeSipCommonHeaderWithDate(hdr->pRetryAfterHdr);
					hdr->pRetryAfterHdr = SIP_NULL;
				}
			}
			break;

		case SipHdrTypeRetryAfterSec:
		case SipHdrTypeRetryAfterAny:
#else
		case SipHdrTypeRetryAfter:
			ppHeader = &(hdr->pRetryAfterHdr);
			break;
#endif /* SIP_DATE */

		case SipHdrTypeUnknown:
			pList = &(hdr->slUnknownHdr);
			break;

#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
			pList = &(hdr->slAllowEventsHdr);
			break;
#endif

		case SipHdrTypeAccept:
			pList = &(hdr->slAcceptHdr);
			break;

		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	if((ppHeader != SIP_NULL) && (*ppHeader != SIP_NULL))
	{
		sip_freeSipCommonHeader (*ppHeader);
		*ppHeader = SIP_NULL;
	}
	else if(pList != SIP_NULL)
	{
		if (sip_listDeleteAll(pList, err) == SipFail)
		return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteAllGeneralHdr");
	return SipSuccess;
}
/******************************************************************
**
** FUNCTION:  deleteAllRequestHdr
**
** DESCRIPTION:  This function deletes all request headers of
**	en_HeaderType "dType".
**
******************************************************************/
SipBool deleteAllRequestHdr
	(SipReqHeader *hdr, en_HeaderType dType, SipError *err)
{

    SipCommonHeader **ppHeader = SIP_NULL;
	SIPDEBUGFN("Entering function deleteAllRequestHdr");
	switch (dType)
	{
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeAuthorization:
			if (sip_listDeleteAll(&(hdr->slAuthorizationHdr),err)==SipFail)
				return SipFail;
			break;
#endif
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
            ppHeader = &(hdr->pReferToHdr);
			break;
		case SipHdrTypeReferredBy:
            ppHeader = &(hdr->pReferredByHdr);
			break;	/* call-transfer */
#endif

#ifdef SIP_IMPP
		case SipHdrTypeEvent:
            ppHeader = &(hdr->pEventHdr);
			break;

		case SipHdrTypeSubscriptionState:
            ppHeader = &(hdr->pSubscriptionStateHdr);
			break;
#endif

#ifdef SIP_REPLACES
		case SipHdrTypeReplaces:
            ppHeader = &(hdr->pReplacesHdr);
			break;
#endif

		case SipHdrTypeMaxforwards:
            ppHeader = &(hdr->pMaxForwardsHdr);
			break;
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeProxyauthorization:
			if (sip_listDeleteAll(&(hdr->slProxyAuthorizationHdr),err)==SipFail)
				return SipFail;
			break;
#endif
		case SipHdrTypeRoute:
			if (sip_listDeleteAll(&(hdr->slRouteHdr), err) == SipFail)
				return SipFail;
			break;
#ifdef SIP_RPR
		case SipHdrTypeRAck:
            ppHeader = &(hdr->pRackHdr);
			break;
#endif			
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
			if (hdr->pPCalledPartyIdHdr != SIP_NULL)
			{
				sip_freeSipPCalledPartyIdHeader(hdr->pPCalledPartyIdHdr);
				hdr->pPCalledPartyIdHdr = SIP_NULL;
			}
			break;
		case SipHdrTypeJoin:
			if (hdr->pJoinHdr != SIP_NULL)
			{
				sip_freeSipJoinHeader(hdr->pJoinHdr);
				hdr->pJoinHdr = SIP_NULL;
			}
			break;
		case SipHdrTypeIfMatch:
			if (hdr->pIfMatchHdr != SIP_NULL)
			{
				sip_freeSipIfMatchHeader(hdr->pIfMatchHdr);
				hdr->pIfMatchHdr = SIP_NULL;
			}
			break;

		case SipHdrTypePVisitedNetworkId:
			if (sip_listDeleteAll(&(hdr->slPVisitedNetworkIdHdr),err)==SipFail)
				return SipFail;
			break;
		case SipHdrTypeAcceptContact:
			if (sip_listDeleteAll(&(hdr->slAcceptContactHdr),err)==SipFail)
				return SipFail;
			break;
		case SipHdrTypeRejectContact:
			if (sip_listDeleteAll(&(hdr->slRejectContactHdr),err)==SipFail)
				return SipFail;
			break;
		case SipHdrTypeRequestDisposition:
			if (sip_listDeleteAll(&(hdr->slRequestDispositionHdr),err)==SipFail)
				return SipFail;
			break;

#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityClient:
			if (sip_listDeleteAll(&(hdr->slSecurityClientHdr),err)==SipFail)
				return SipFail;
			break;
        case SipHdrTypeSecurityVerify:
			if (sip_listDeleteAll(&(hdr->slSecurityVerifyHdr),err)==SipFail)
				return SipFail;
			break;            
#endif
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
			if (sip_listDeleteAll(&(hdr->slRsrcPriorityHdr),err)==SipFail)
				return SipFail;
			break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

    if(ppHeader != SIP_NULL)
    {
		if(*ppHeader != SIP_NULL)
		{
			sip_freeSipCommonHeader(*ppHeader);
			*ppHeader = SIP_NULL;
		}
    }

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteAllRequestHdr");
	return SipSuccess;
}


/******************************************************************
**
** FUNCTION:  deleteAllResponseHdr
**
** DESCRIPTION: This function deletes all response headers of en_HeaderType "dType".
**
******************************************************************/
SipBool deleteAllResponseHdr
	(SipRespHeader *hdr, en_HeaderType dType, SipError *err)
{
	SIPDEBUGFN("Entering function deleteAllResponseHdr");
	switch(dType)
	{
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeProxyAuthenticate:
			if (sip_listDeleteAll(&(hdr->slProxyAuthenticateHdr), err)\
				== SipFail)
				return SipFail;
			break;
#endif
#ifdef SIP_RPR
		case SipHdrTypeRSeq:
			if (hdr->pRSeqHdr != SIP_NULL)
			{
					sip_rpr_freeSipRSeqHeader(hdr->pRSeqHdr);
					hdr->pRSeqHdr = SIP_NULL;
			}
			break;
#endif
		case SipHdrTypeUnsupported:
			if (sip_listDeleteAll(&(hdr->slUnsupportedHdr), err) == SipFail)
				return SipFail;
			break;
#ifdef SIP_WARNING
		case SipHdrTypeWarning:
			if (sip_listDeleteAll(&(hdr->slWarningHeader), err) == SipFail)
				return SipFail;
			break;
#endif			
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeWwwAuthenticate:
			if (sip_listDeleteAll(&(hdr->slWwwAuthenticateHdr), err) == SipFail)
				return SipFail;
			break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
			if (sip_listDeleteAll(&(hdr->slPAssociatedUriHdr), err) == SipFail)
				return SipFail;
			break;

		case SipHdrTypeServiceRoute:
			if (sip_listDeleteAll(&(hdr->slServiceRouteHdr), err) == SipFail)
				return SipFail;
			break;
		case SipHdrTypeETag:
			if (hdr->pETagHdr != SIP_NULL)
			{
					sip_freeSipETagHeader(hdr->pETagHdr);
					hdr->pETagHdr = SIP_NULL;
			}
			break;
#endif

#ifdef SIP_SECURITY
       	case SipHdrTypeSecurityServer:
			if (sip_listDeleteAll(&(hdr->slSecurityServerHdr), err) == SipFail)
				return SipFail;
			break;     
#endif
		case SipHdrTypeMinExpires:
			if (hdr->pMinExpiresHdr != SIP_NULL)
			{
					sip_freeSipMinExpiresHeader(hdr->pMinExpiresHdr);
					hdr->pMinExpiresHdr = SIP_NULL;
			}
			break;
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
			if (sip_listDeleteAll(&(hdr->slAcceptRsrcPriorityHdr), err) == SipFail)
				return SipFail;
			break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteAllResponseHdr");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sip_parseSingleHeader
**
** DESCRIPTION: This function parses a header string to form a SipHeader
**		structure containing the parsed header. This function can only
**		be used to parse known headers.
** 
** PARAMETERS:
**	pHeaderStr(IN) - The string containing the header to be parsed.
**	dType(IN) - The type of of the header string that is to be parsed.
**		For types like Contact or Expires, the Any type should be used.
**		The function will return fail if this type does not match with 
**		the type of the header given in the string.
**	pHeader(OUT) - The header structure which will contain the parsed
**		header on return. This structure should be initialized using
**		the sip_initSipHeader function with header type SipHdrTypeAny.
**	pErr(OUT) - Error indication in case of failure.
**
*********************************************************************/

SipBool sip_parseSingleHeader
(	SIP_S8bit *pHeaderStr, en_HeaderType dType, SipHeader *pHeader, \
	SipError *pErr)
{

	SIP_S8bit	*pTempHeaderStr = SIP_NULL;

	if (pErr == SIP_NULL)
		return SipFail;

	if (sip_validateHeaderString(pHeaderStr, dType, pErr)!=SipSuccess)
		return SipFail;

    /* Lint Fix: Return type of this function */
	(void)sip_changeTypeAny(&dType, pErr);

	/* Check if header type is unknown. If so dont use the parser */
/*	if(dType == SipHdrTypeUnknown)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
*/
	/* 
	 * pHeader would have been inited with type SipHdrTypeAny. Thus we need
	 * to allocate memory for pHeader->pHeader.
	 */

	pHeader->dType = dType;
	/*
	 * Now invoke the respective header parser.
	 * The string that is passed to us in pHeaderStr
	 * contains the header name as well. We need to isolate
	 * just the hdr body and pass that to parseHeaderBody.
	 */

    if(dType == SipHdrTypeContactNormal)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &pHeader->pHeader, 
                SipContactNormal, pErr);
    }
    else if(dType == SipHdrTypeContactWildCard)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &pHeader->pHeader, 
                SipContactWildCard, pErr);
    }
    else if(dType == SipHdrTypeContactAny)
    {
        (void)sip_initSipContactHeader((SipContactHeader **) &pHeader->pHeader, 
                SipContactAny, pErr);
    }
    
#ifdef SIP_DATE
    else if((dType == SipHdrTypeExpiresDate)||
            (dType == SipHdrTypeRetryAfterDate))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &pHeader->pHeader, SipExpDate, pErr);
    }else if((dType == SipHdrTypeExpiresAny)||
            (dType == SipHdrTypeRetryAfterAny))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &pHeader->pHeader, SipExpAny, pErr);
    }else if((dType == SipHdrTypeExpiresSec)||
            (dType == SipHdrTypeRetryAfterSec))
    {
        (void)sip_initSipCommonHeaderWithDate((SipCommonHeader **) 
                &pHeader->pHeader, SipExpSeconds, pErr);
    }
#endif
    else if(dType != SipHdrTypeAny)
    {
        (void)sip_initSipCommonHeader((SipCommonHeader **)&pHeader->pHeader,pErr);
    }

	pTempHeaderStr = sip_tokenize(pHeaderStr, pHeaderStr + sip_strlen(pHeaderStr) -1,\
		   	':');
	if (pTempHeaderStr == SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	if(dType == SipHdrTypeUnknown)
	{
		((SipCommonHeader *)(pHeader->pHeader))->pStr1 = sip_nStrdup( \
			DECODE_MEM_ID, pHeaderStr,(SIP_U16bit)(pTempHeaderStr-pHeaderStr),\
		   	pErr);	
		if(((SipCommonHeader *)(pHeader->pHeader))->pStr1 == SIP_NULL)
		{
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}
	pTempHeaderStr += 1;	

	if (SipFail == sip_parseHeaderBody(pTempHeaderStr, \
		pTempHeaderStr + sip_strlen(pTempHeaderStr) -1, dType, pHeader, pErr))
	{
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	return SipSuccess;

}


/*****************************************************************
**
** FUNCTION:  sip_getBodyFromBadHdr
**
** DESCRIPTION: This function retrieves the body from an Unknown SIP
**		header
**
***************************************************************/
#ifdef  SIP_BADMESSAGE_PARSING
SipBool sip_getBodyFromBadHdr
	(SipBadHeader *hdr, SIP_S8bit **pBody, SipError *err)
{
 	SIP_S8bit *temp_body;
	SIPDEBUGFN("Entering function sip_getBodyFromBadHdr");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pBody == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
 	temp_body = hdr->pBody;
 	if (temp_body == SIP_NULL)
 	{
 		*err = E_NO_EXIST;
 		return SipFail;
 	}
	*pBody = temp_body;

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getBodyFromBadHdr");
 	return SipSuccess;
}

/*****************************************************************
**
** FUNCTION:  sip_getNameFromBadHdr
**
** DESCRIPTION: This function retrieves the name from an Unknown
**		SIP header
**
***************************************************************/
SipBool sip_getNameFromBadHdr
	(SipBadHeader *hdr, SIP_S8bit **pName, SipError *err)
{
 	SIP_S8bit *temp_name;
	SIPDEBUGFN("Entering function sip_getNameFromBadHdr");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pName == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
 	temp_name = hdr ->pName;
 	if (temp_name == SIP_NULL)
 	{
 		*err = E_NO_EXIST;
 		return SipFail;
 	}
	*pName = temp_name;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getNameFromBadHdr");
 	return SipSuccess;
}

/*****************************************************************
**
** FUNCTION:  sip_getTypeFromBadHdr
**
** DESCRIPTION: This function retrieves the header type from the bad
**		header. This is the type corresponding to the name of the
**		header.
**
***************************************************************/
SipBool sip_getTypeFromBadHdr
	(SipBadHeader *hdr, en_HeaderType *pType, SipError *err)
{
	SIPDEBUGFN("Entering function sip_getTypeFromBadHdr");
#ifndef SIP_NO_CHECK
	if( err == SIP_NULL)
		return SipFail;
	if( pType == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if( hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
 	*pType = hdr ->dType;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getTypeFromBadHdr");
 	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sip_getBadHeaderCount
**
** DESCRIPTION: This function returns the number of bad headers in the 
**	SIP message. Messages with bad headers are returned when a syntactically
**	incorrect message is passed to sip_decodeMessage with the SIP_OPT_BADMESSAGE
**	option.
** PARAMETERS:
**	pMessage (IN) - The SIP message that is to be examined for the presence of
**		bad headers
**	pCount (OUT) - Returns the number of bad headers in the message.
**	pError (OUT) - Returns the error code in case of failure.
**
*********************************************************************/
SipBool sip_getBadHeaderCount
(SipMessage *pMessage, SIP_U32bit *pCount, SipError *pError)
{
#ifndef SIP_NO_CHECK
	if(pError == SIP_NULL)
		return SipFail;
	if(pMessage == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if(pCount == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
#endif
	return sip_listSizeOf(&(pMessage->pGeneralHdr->slBadHdr), pCount, pError);
}


/********************************************************************
**
** FUNCTION:  sip_getBadHeaderAtIndex
**
** DESCRIPTION: This function returns a bad header from a SIP message
**	Messages with bad headers are returned when a syntactically
**	incorrect message is passed to sip_decodeMessage with the 
**	SIP_OPT_BADMESSAGE option.
** PARAMETERS:
**	pMessage (IN) - The SIP message that is to be examined for the presence of
**		bad headers
**	ppBadHdr (OUT) - Returns the bad header from the message.
**	index (IN) - This index of the bad header to be returned.
**	pError (OUT) - Returns the error code in case of failure.
**
*********************************************************************/
SipBool sip_getBadHeaderAtIndex
(SipMessage *pMessage, SipBadHeader **ppBadHdr, SIP_U32bit dIndex, SipError *pError)
{
#ifndef SIP_NO_CHECK
	if(pError == SIP_NULL)
		return SipFail;
	if(pMessage == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	if(ppBadHdr == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(sip_listGetAt(&(pMessage->pGeneralHdr->slBadHdr), dIndex, (SIP_Pvoid *)\
		ppBadHdr, pError)==SipFail)
		return SipFail;
	HSS_LOCKEDINCREF((*ppBadHdr)->dRefCount);
	*pError = E_NO_ERROR;
	return SipSuccess;
}
#endif
#ifdef SIP_DATE
#endif


/* MICROUA additions begin */
void sip_getTypeFromName
(SIP_S8bit *pName, en_HeaderType *pType, SipError *pError)
{
	en_HeaderForm 	dForm;
	SIP_S8bit	  	*pTempStr;
	SIP_U32bit		dSize;

	if (pName == SIP_NULL)
	{
		*pType = SipHdrTypeUnknown;
		return;
	}

	dSize = sip_strlen(pName);
	pTempStr =(char *) fast_memget(ACCESSOR_MEM_ID, dSize, SIP_NULL);
	if (pTempStr == SIP_NULL)
	{
		*pType = SipHdrTypeUnknown;
		return;
	}
	(void)sip_strncpy(pTempStr, pName, dSize);
	pTempStr[dSize-1]='\0';

	*pType = sip_getHeaderTypeFromString(pTempStr, \
			pTempStr + (sip_strlen(pTempStr)) - 1, &dForm);
	
	(void)fast_memfree(0, pTempStr , SIP_NULL);
	*pError = E_NO_ERROR;

}

#ifdef STK_RELEASE
#else
/********************************************************************
**
** FUNCTION:  sip_getHeaderAsStringAtIndex
**
** DESCRIPTION:  This function retrieves a SipHeader of en_HeaderType 
** "dType" in exactly the same way as sip_getHeaderAtIndex; 
** the output pHeader here is in the form of a string "hdr".

**
*********************************************************************/
SipBool sip_getHeaderAsStringAtIndex
(SipMessage *s, en_HeaderType dType,  SIP_S8bit **hdr, SIP_U32bit dIndex, SipError *err)
{
	SipError pTempErr;
	if(validateSipHeaderType(dType,err)==SipFail)
	{
		return SipFail;
	}
	*hdr = (SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID,SIP_MAX_HDR_SIZE,err);
	(void)sip_memset(*hdr, 0, SIP_MAX_HDR_SIZE);
	if (*hdr == SIP_NULL)
	{
		return SipFail;
	}
	/* Fix for IPTK CSR 1-8047728: SPR 20246 */
	if (sip_formSingleHeader(dType, NULL, dIndex, SipModeNew, s, \
		0, *hdr, err) == SipFail)
	{
		(void)fast_memfree(ACCESSOR_MEM_ID, *hdr, &pTempErr);
		*hdr=SIP_NULL;
		return SipFail;
	}
	*err = E_NO_ERROR;
	return SipSuccess;
}
#endif


#ifdef SIP_SESSIONTIMER
/* SS: Strdup is used here */
/*****************************************************************
**
** FUNCTION:  sip_getRefresherFromSessionExpiresHdr
**
** DESCRIPTION: This function retrieves the refresher field from a SIP
**		SessionExpires pHeader
**
***************************************************************/
SipBool sip_getRefresherFromSessionExpiresHdr
	(SipHeader *pHdr,SIP_S8bit **ppRefresher, SipError *pErr)
{
	SIP_S8bit	*pStringData = SIP_NULL;
	
	SIPDEBUGFN ( "Entering getRefresherFromSessionExpiresHeader");

    if(SipFail ==
            sip_getParamForNameFromCommonHdr(pHdr,"refresher",&pStringData,pErr))
    {
        return SipFail;
    }

	if (pStringData == SIP_NULL)
 	{
 		*pErr = E_NO_EXIST;
 		return SipFail;
 	}

	/*
	 * Now, make a copy of the refresher param found and return to the user 
	 * in the field provided (ppRefresher). 
	 */

	*ppRefresher = STRDUPACCESSOR(pStringData);
	if (SIP_NULL == *ppRefresher)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}

	SIPDEBUGFN ( "Exiting getRefresherFromSessionExpiresHeader");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


#endif /* SIP_SESSIONTIMER */


/********************************************************************
**
** FUNCTION:  sdp_setSdpConnectionFromString
**
** DESCRIPTION: This function sets the SESSION LEVEL "c=" line of the 
**				SDP message (taken as input) from the string provided.
**
*********************************************************************/
SipBool sdp_setSdpConnectionFromString
#ifdef ANSI_PROTO
(SdpMessage *pSdpMsg, SIP_S8bit *pStrConn, SipError *pErr)
#else
	( pSdpMsg,pStrConn,pErr)
	SdpMessage *pSdpMsg;
	SIP_S8bit *pStrConn;
	SipError *pErr;
#endif
{
	SdpConnection *pTempConn;
	SIP_S8bit	  *pDummyPos = SIP_NULL, *pDupStr;
	SIP_U32bit		dLen;

	SIPDEBUGFN("Entering function sdp_setSdpConnectionFromString");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStrConn == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	dLen = sip_strlen(pStrConn);
	pDupStr = sip_nStrdup(ACCESSOR_MEM_ID, pStrConn, dLen+1, pErr);
	
	/* 
	 * Insert an LF at the end of the string (just before the NULL), since
	 * the parser expects this.
	 */
	pDupStr[dLen] = '\n';

	if (SipFail == sip_parseSdpConnectionInfo(pDupStr+2, pDupStr+dLen, \
		&pDummyPos, &pTempConn, pErr))
	{
		(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);
		return SipFail;
	}
	(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);

	if(pSdpMsg->slConnection != SIP_NULL)
		sip_freeSdpConnection(pSdpMsg->slConnection);

	pSdpMsg->slConnection = pTempConn;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN ("Exitting function sdp_setSdpConnectionFromString");
	return SipSuccess;

}


/********************************************************************
**
** FUNCTION:  sdp_setSdpOriginFromString
**
** DESCRIPTION: This function sets the "o=" line of the 
**				SDP message (taken as input) from the string provided.
**
*********************************************************************/
SipBool sdp_setSdpOriginFromString
#ifdef ANSI_PROTO
(SdpMessage *pSdpMsg, SIP_S8bit *pStr, SipError *pErr)
#else
	( pSdpMsg,pStr,pErr)
	SdpMessage *pSdpMsg;
	SIP_S8bit *pStr;
	SipError *pErr;
#endif
{
	SdpOrigin *pTempOrigin;
	SIP_S8bit *pDummyPos, *pDupStr;
	SIP_U32bit	dLen;

	SIPDEBUGFN("Entering function sdp_setSdpOriginFromString");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	dLen = sip_strlen(pStr);
	pDupStr = sip_nStrdup(ACCESSOR_MEM_ID, pStr, dLen+1, pErr);
	
	/* 
	 * Insert an LF at the end of the string (just before the NULL), since
	 * the parser expects this.
	 */
	pDupStr[dLen] = '\n';

	pTempOrigin = pSdpMsg->pOrigin;
	if (SipFail == sip_parseSdpOrigin(pDupStr+2, pDupStr+dLen, &pDummyPos, \
		pSdpMsg, pErr))
	{
		(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);
		return SipFail;
	}
	(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);
	
	if (pTempOrigin != SIP_NULL)
		sip_freeSdpOrigin(pTempOrigin);
	
	*pErr = E_NO_ERROR;
	return SipSuccess;

}


/***********************/


/********************************************************************
**
** FUNCTION:  sip_insertHeaderFromStringAtIndex
**
** DESCRIPTION:  This function inserts a SipHeader of en_HeaderType "dType" in exactly the same way as sip_insertHeaderAtIndex; the input pHeader here is in the form of a string "hdr".
**
*********************************************************************/
SipBool sip_insertHeaderFromStringAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, SIP_S8bit *hdr, SIP_U32bit dIndex, SipError *err)
#else
	(msg, dType, hdr, dIndex, err)
	SipMessage *msg;
	en_HeaderType dType;
	SIP_S8bit *hdr;
	SIP_U32bit dIndex;
	SipError *err;
#endif
{
	SipHeader				*pParsedHeader;

#ifndef SIP_NO_CHECK
	if(err == SIP_NULL)
		return SipFail;

	if(msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if(hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	(void)sip_changeTypeAny(&dType, err);

	if(sip_validateHeaderString(hdr, dType, err)!=SipSuccess)
		return SipFail;
	
	pParsedHeader = (SipHeader *)fast_memget(INIT_MEM_ID,sizeof(SipHeader),err);
	if (pParsedHeader == SIP_NULL)
		return SipFail;

	if (SipFail == sip_parseSingleHeader(hdr, dType, pParsedHeader, err))
		return SipFail;

	if(sip_insertHeaderAtIndex(msg, pParsedHeader, dIndex, err)\
		!=SipSuccess)
	{
		sip_freeSipHeader(pParsedHeader);
		(void)fast_memfree(INIT_MEM_ID, pParsedHeader,err);
		return SipFail;
	}
	sip_freeSipHeader(pParsedHeader);
	(void)fast_memfree(INIT_MEM_ID, pParsedHeader,err);

	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sdp_insertSdpAttrAtIndexFromString
**
** DESCRIPTION: This function inserts the "a=" line at a given index in 
**				the	SDP message (taken as input) from the string provided.
**
*********************************************************************/
SipBool sdp_insertSdpAttrAtIndexFromString
#ifdef ANSI_PROTO
(SdpMessage *pSdpMsg, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr)
#else
	( pSdpMsg,pStr,dIndex,pErr)
	SdpMessage *pSdpMsg;
	SIP_S8bit *pStr;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	/*SdpTime *pTempTime;*/
	SIP_S8bit			*pTemp, *pDupStr;
	SipList				tmpList;
	SipListElement		*pTmpListElem;
	SdpAttr				*pTmpAttr;
	SIP_U32bit			dLen;

	SIPDEBUGFN("Entering function sdp_insertSdpAttrAtIndexFromString");
#ifndef SIP_NO_CHECK 
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	dLen = sip_strlen(pStr);
	pDupStr = sip_nStrdup(ACCESSOR_MEM_ID, pStr, dLen+1, pErr);
	
	/* 
	 * Insert an LF at the end of the string (just before the NULL), since
	 * the parser expects this.
	 */
	pDupStr[dLen] = '\n';

	if (SipFail == sip_listInit(&tmpList, __sip_freeSdpAttr, pErr))
		return SipFail;

	if (SipFail == sip_parseSdpAttr(pDupStr+2, pDupStr+dLen, \
		&pTemp, &(tmpList), pErr))
	{
		(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);
		return SipFail;
	}
	(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&pDupStr), pErr);
	
	pTmpListElem = (SipListElement *)(tmpList.head);
	pTmpAttr	= (SdpAttr *)(pTmpListElem->pData);
	if ((sip_listInsertAt(&(pSdpMsg->slAttr), dIndex, \
		(void *)pTmpAttr, pErr))==SipFail)
		return SipFail;
	pTmpListElem->pData = SIP_NULL;
	(void)sip_listDeleteAll(&tmpList, pErr);


	*pErr = E_NO_ERROR;
	SIPDEBUGFN ("Exitting function sdp_insertSdpAttrAtIndexFromString");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sdp_insertAttrFromStringAtIndexInMedia
**
** DESCRIPTION: This function inserts the "a=" line at a given index in 
**				the	SDP message (taken as input) from the string provided.
**
*********************************************************************/
SipBool sdp_insertAttrFromStringAtIndexInMedia
#ifdef ANSI_PROTO
(SdpMedia *pSdpMedia, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr)
#else
	( pSdpMedia,pStr,dIndex,pErr)
	SdpMedia *pSdpMedia;
	SIP_S8bit *pStr;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	/*SdpTime *pTempTime;*/
	SIP_S8bit *pTemp, *pEnd;
	SipList				tmpList;
	SipListElement		*pTmpListElem;
	SdpAttr				*pTmpAttr;

	SIPDEBUGFN("Entering function sdp_insertAttrFromStringAtIndexInMedia");
#ifndef SIP_NO_CHECK 
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMedia == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pEnd = pStr + sip_strlen((char *)pStr);
	*pEnd = '\n';

	if (SipFail == sip_listInit(&tmpList, __sip_freeSdpAttr, pErr))
		return SipFail;

	if (SipFail == sip_parseSdpAttr(pStr+2, pEnd, \
		&pTemp, &(tmpList), pErr))
		return SipFail;
	
	pTmpListElem = (SipListElement *)(tmpList.head);
	pTmpAttr	= (SdpAttr *)(pTmpListElem->pData);

	if ((sip_listInsertAt(&(pSdpMedia->slAttr), dIndex, \
		(void *)pTmpAttr, pErr))==SipFail)
		return SipFail;

	pTmpListElem->pData = SIP_NULL;
	(void)sip_listDeleteAll(&tmpList, pErr);

	*pErr = E_NO_ERROR;
	SIPDEBUGFN ("Exiting function sdp_insertAttrFromStringAtIndexInMedia");
	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sdp_insertConnectionFromStringAtIndexInMedia
**
** DESCRIPTION: This function 'inserts' the MEDIA LEVEL "c=" line  
**				(at index: dIndex) from the string provided.
**
*********************************************************************/
SipBool sdp_insertConnectionFromStringAtIndexInMedia
#ifdef ANSI_PROTO
(SdpMedia *pSdpMedia, SIP_S8bit *pStrConn, SIP_U32bit dIndex, SipError *pErr)
#else
	( pSdpMedia,pStrConn,dIndex,pErr)
	SdpMedia *pSdpMedia;
	SIP_S8bit *pStrConn;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SdpConnection *pTempConn;
	SIP_S8bit	  *pTemp, *pEnd;

	SIPDEBUGFN ("Entering function sdp_insertConnectionFromStringAtIndexInMedia");	
#ifndef SIP_NO_CHECK	
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMedia == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStrConn == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif		
/*
	if (dIndex == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}		
*/
	pEnd =  pStrConn + sip_strlen((char *)pStrConn);
	*pEnd = '\n'; /* Temp. FIX */
	if (SipFail == sip_parseSdpConnectionInfo(pStrConn+2, \
		pEnd, &pTemp, &pTempConn, pErr))
		return SipFail;

	if ((sip_listInsertAt(&(pSdpMedia->slConnection), dIndex, \
						(void *)pTempConn, pErr))==SipFail)
		return SipFail;
	
	*pErr = E_NO_ERROR;
	SIPDEBUGFN ("Exitting function sdp_insertConnectionFromStringAtIndexInMedia");
	return SipSuccess;	
}


/********************************************************************
**
** FUNCTION:  sip_setHeaderFromStringAtIndex
**
** DESCRIPTION:  This function inserts a SipHeader of en_HeaderType "dType" in exactly the same way as sip_insertHeaderAtIndex; the input pHeader here is in the form of a string "hdr".
**
*********************************************************************/
SipBool sip_setHeaderFromStringAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, SIP_S8bit *hdr, SIP_U32bit dIndex, SipError *err)
#else
	(msg, dType, hdr, dIndex, err)
	SipMessage *msg;
	en_HeaderType dType;
	SIP_S8bit *hdr;
	SIP_U32bit dIndex;
	SipError *err;
#endif
{
	/*SIP_U32bit				count;
	SipError				tempError;
	SIP_S8bit				*temp;*/
	SipHeader				*pParsedHeader=SIP_NULL;

#ifndef SIP_NO_CHECK
	if(err == SIP_NULL)
		return SipFail;

	if(msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if(hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	(void)sip_changeTypeAny(&dType, err);

	if(sip_validateHeaderString(hdr, dType, err)!=SipSuccess)
		return SipFail;
	
	pParsedHeader = (SipHeader *)fast_memget(INIT_MEM_ID,sizeof(SipHeader),err);
	if (pParsedHeader == SIP_NULL)
		return SipFail;
	
	if (SipFail == sip_parseSingleHeader(hdr, dType, pParsedHeader, err))
		return SipFail;

	if(sip_setHeaderAtIndex(msg, pParsedHeader, dIndex, err)\
		!=SipSuccess)
	{
		sip_freeSipHeader(pParsedHeader);
		(void)fast_memfree(INIT_MEM_ID, pParsedHeader,err);
		return SipFail;
	}
	sip_freeSipHeader(pParsedHeader);
	(void)fast_memfree(INIT_MEM_ID, pParsedHeader,err);

	return SipSuccess;
}


/********************************************************************
**
** FUNCTION:  sdp_insertSdpTimeAtIndexFromString
**
** DESCRIPTION: This function inserts the "t=" line at a given index in 
**				the	SDP message (taken as input) from the string provided.
**
*********************************************************************/
SipBool sdp_insertSdpTimeAtIndexFromString
#ifdef ANSI_PROTO
(SdpMessage *pSdpMsg, SIP_S8bit *pStr, SIP_U32bit dIndex, SipError *pErr)
#else
	( pSdpMsg,pStr,dIndex,pErr)
	SdpMessage *pSdpMsg;
	SIP_S8bit *pStr;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	/*SdpTime *pTempTime;*/
	SIP_S8bit *pTemp, *pEnd;
	(void)dIndex;
	SIPDEBUGFN("Entering function sdp_insertSdpTimeAtIndexFromString");
#ifndef SIP_NO_CHECK 
	if (pErr == SIP_NULL)
		return SipFail;
	if (pSdpMsg == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pEnd = pStr + sip_strlen((char *)pStr);
	*pEnd = '\n';
	if (SipFail == sip_parseSdpTime(pStr+2, pEnd, \
		&pTemp, &(pSdpMsg->slTime), pErr))
		return SipFail;

	/*
	 * UA currently uses this API only for appending, so insertAtIndex is
	 * not reqd. since parseSdpTime itself appends it to the end of the
	 * list.
	 */

	*pErr = E_NO_ERROR;
	SIPDEBUGFN ("Exitting function sdp_insertSdpTimeAtIndexFromString");
	return SipSuccess;
}

/********************************************************************
**
** FUNCTION:  sip_setAddrSpecFromString
**
** DESCRIPTION: This function sets the string parameter taken as input
**				as a AddrSpec structure.
**
** NOTE: This function in turn calls sip_makeToHeader to parse the
**		 sipurl. Once that is done; the AddrSpec structure is cloned
**		 into the input AddrSpec pointer
*********************************************************************/
SipBool sip_setAddrSpecFromString
(SipAddrSpec **ppAddrSpec, SIP_S8bit *pStr, SipError *pErr)
{
	SIP_U32bit dLength;
	SIP_S8bit	*pStart, *pEnd;

	SIPDEBUG("Entering sip_setAddrSpecFromString");
	if (pErr == SIP_NULL)
	{
		return SipFail;
	}
	if (ppAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pStr == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	dLength = sip_strlen((char *)pStr);
	pStart = pStr;
	pEnd = pStr + dLength - 1;

	if (SipFail == sip_parseAddrSpec(pStart, pEnd, \
		ppAddrSpec, pErr)) /*might require FIX*/
		return SipFail;	

	SIPDEBUG("Exitting sip_setAddrSpecFromString");
	return SipSuccess;
}
/* MICROUA additions end */











