
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
 extern "C" {
#endif
/**************************************************************
 ** FUNCTION:
 **	 	This file has the definitions for the tel-url
 **		APIs
 *************************************************************
 **
 ** FILENAME:
 ** telapi.h
 **
 ** DESCRIPTION:
 **
 **
 ** DATE			NAME			REFERENCE		REASON
 ** ----			----			--------		------
 ** 2000-Aug-15		Mahesh		    Core Stack		Initial
****************************************************************/

#ifndef __TEL_API_C_
#define __TEL_API_C_
#include "microsip_telapi.h"
#include "microsip_telinit.h"
#include "microsip_telfree.h"

/******************************************************************************
** FUNCTION: sip_compareTelUri
**
** DESCRIPTION: This function is used to compare two Tel Address Specs 
*******************************************************************************/
SipBool  sip_compareTelUri
#ifdef ANSI_PROTO	
(TelUrl		 *pTel1, 
 TelUrl		 *pTel2,
 SipError *pError)
#else
(pTel1, pTel2, pError)
 TelUrl *pTel1;
 TelUrl *pTel2;
 SipError *pError;
#endif
{
		
	(void)pError; 
	if ( pTel1 == SIP_NULL || pTel2 == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}	
	/* Compare the Global field of the Tel-URL
	 */

	/* If the Global filed is present one of the tel-url structure and the
	 * other tel-url structure do not have Global filed 
	 * then return SipFail(failure)
	 * Otherwies if the Global field is presents in both the tel-url structure
	 * then Compare each filead of the Global Tel Url Structure
	 */
	if( pTel1->pGlobal != SIP_NULL )
	{
		SIP_U32bit dGlobalAreaSpecifier1=0,dGlobalAreaSpecifier2=0,i=0,j=0;
		
		if(pTel2->pGlobal == SIP_NULL )
			return SipFail;

		/* Compare the Base numerber of the Global phone number
		 */

		if( pTel1->pGlobal->pBaseNo == SIP_NULL )
		{
			if( pTel2->pGlobal->pBaseNo != SIP_NULL )
				return SipFail;
		}
		else
		{
			if( pTel2->pGlobal->pBaseNo == SIP_NULL )
				return SipFail;
		}
		if( ( pTel1->pGlobal->pBaseNo != SIP_NULL ) &&
			( pTel2->pGlobal->pBaseNo != SIP_NULL ))
		{

			if( sip_strcmp( pTel1->pGlobal->pBaseNo, \
								pTel2->pGlobal->pBaseNo) != 0)
				return SipFail;
		}

		/*	Compare the Isdn part of the Global Phone number
		 */

		if( pTel1->pGlobal->pIsdnSubAddr == SIP_NULL )
		{
			if( pTel2->pGlobal->pIsdnSubAddr != SIP_NULL )
				return SipFail;
		}
		else
		{
			if( pTel2->pGlobal->pIsdnSubAddr == SIP_NULL )
				return SipFail;
		}

		if( ( pTel1->pGlobal->pIsdnSubAddr != SIP_NULL ) &&
			( pTel2->pGlobal->pIsdnSubAddr != SIP_NULL ))
		{
	
			if( sip_strcmp( pTel1->pGlobal->pIsdnSubAddr, \
								pTel2->pGlobal->pIsdnSubAddr) != 0)
				return SipFail;
		}

		/* Compare the Post dial number of the Global tel-url structure
		 */

		if( pTel1->pGlobal->pPostDial == SIP_NULL )
		{
			if( pTel2->pGlobal->pPostDial != SIP_NULL )
				return SipFail;
		}
		else
		{
			if( pTel2->pGlobal->pPostDial == SIP_NULL )
				return SipFail;
		}

		if( ( pTel1->pGlobal->pPostDial != SIP_NULL ) &&
			( pTel2->pGlobal->pPostDial != SIP_NULL ))
		{
	
			if( sip_strcmp( pTel1->pGlobal->pPostDial, \
								pTel2->pGlobal->pPostDial) != 0)
				return SipFail;
		}
		/* Compare the Area Specifier
		 */
		if(sip_getAreaSpecifierCountFromTelGlobalNum(pTel1->pGlobal,\
				&dGlobalAreaSpecifier1,pError) == SipFail)
		{
			return SipFail;
		}
		if(sip_getAreaSpecifierCountFromTelGlobalNum(pTel2->pGlobal,\
				&dGlobalAreaSpecifier2,pError) == SipFail)
		{
			return SipFail;
		}
		if(dGlobalAreaSpecifier2 != dGlobalAreaSpecifier1)
			return SipFail;
		
		/*Compare each of the area specifer
		 */

		for(i=0; i<dGlobalAreaSpecifier1;i++)
		{
			SIP_S8bit *pGloblaSpecifier1=SIP_NULL;
			SipBool	dAreaSpecifierMatch=SipFail;
			if(sip_getAreaSpecifierAtIndexFromTelGlobalNum(pTel1->pGlobal,\
				&pGloblaSpecifier1,i,pError)==SipFail)
			{
				return SipFail;
			}

			for(j=0; j<dGlobalAreaSpecifier2;j++)
			{
				SIP_S8bit *pGloblaSpecifier2=SIP_NULL;
				if(sip_getAreaSpecifierAtIndexFromTelGlobalNum(pTel2->pGlobal,\
					&pGloblaSpecifier2,j,pError)==SipFail)
				{
					return SipFail;
				}
				if(sip_strcasecmp(pGloblaSpecifier2,pGloblaSpecifier1) ==0)
				{
					dAreaSpecifierMatch=SipSuccess;
					break;	
				}
			}
			if(dAreaSpecifierMatch !=SipSuccess)
				return SipFail;
		}

	}
	else 
	{	
		if( pTel2->pGlobal != SIP_NULL )
			return SipFail;
	}

	/* Compare the Local phone number part of the Tel-Url Structure
	 */

	/* If the Local filed is present one of the tel-url structure and the
	 * other tel-url structure do not have Local filed 
	 * then return SipFail(failure)
	 * Otherwies if the Local field is presents in both the tel-url structure
	 * then Compare each filead of the Global Tel Url Structure
	 */


	if( pTel1->pLocal != SIP_NULL )
	{
		SIP_U32bit dLocalAreaSpecifier1=0,dLocalAreaSpecifier2=0,i=0,j=0;

		if(pTel2->pLocal == SIP_NULL )
			return SipFail;

		/* Compare the Local Phone digit
		 */

		if( pTel1->pLocal->pLocalPhoneDigit == SIP_NULL )
		{
			if( pTel2->pLocal->pLocalPhoneDigit != SIP_NULL )
				return SipFail;
		}
		if( ( pTel1->pLocal->pLocalPhoneDigit != SIP_NULL ) &&
			( pTel2->pLocal->pLocalPhoneDigit != SIP_NULL ))
		{

			if( sip_strcmp( pTel1->pLocal->pLocalPhoneDigit, \
								pTel2->pLocal->pLocalPhoneDigit) != 0)
				return SipFail;
		}

		/*	Compare the Isdn part of the Global Phone number
		 */

		if( pTel1->pLocal->pIsdnSubAddr == SIP_NULL )
		{
			if( pTel2->pLocal->pIsdnSubAddr != SIP_NULL )
				return SipFail;
		}

		if( ( pTel1->pLocal->pIsdnSubAddr != SIP_NULL ) &&
			( pTel2->pLocal->pIsdnSubAddr != SIP_NULL ))
		{
	
			if( sip_strcmp( pTel1->pLocal->pIsdnSubAddr, \
								pTel2->pLocal->pIsdnSubAddr) != 0)
				return SipFail;
		}
		
		/* Compare the Post dial number of the Global tel-url structure
		 */

		if( pTel1->pLocal->pPostDial == SIP_NULL )
		{
			if( pTel2->pLocal->pPostDial != SIP_NULL )
				return SipFail;
		}

		if( ( pTel1->pLocal->pPostDial != SIP_NULL ) &&
			( pTel2->pLocal->pPostDial != SIP_NULL ))
		{
	
			if( sip_strcmp( pTel1->pLocal->pPostDial, \
								pTel2->pLocal->pPostDial) != 0)
				return SipFail;
		}


		/* Compare the Area Specifier
		 */
		if(sip_getAreaSpecifierCountFromTelLocalNum(pTel1->pLocal,\
				&dLocalAreaSpecifier1,pError) == SipFail)
		{
			return SipFail;
		}
		if(sip_getAreaSpecifierCountFromTelLocalNum(pTel2->pLocal,\
				&dLocalAreaSpecifier2,pError) == SipFail)
		{
			return SipFail;
		}
		if(dLocalAreaSpecifier2 != dLocalAreaSpecifier1)
			return SipFail;
		
		/*Compare each of the area specifer
		 */

		for(i=0; i<dLocalAreaSpecifier1;i++)
		{
			SIP_S8bit *pGloblaSpecifier1=SIP_NULL;
			SipBool	dAreaSpecifierMatch=SipFail;
			if(sip_getAreaSpecifierAtIndexFromTelLocalNum(pTel1->pLocal,\
				&pGloblaSpecifier1,i,pError)==SipFail)
			{
				return SipFail;
			}

			for(j=0; j<dLocalAreaSpecifier2;j++)
			{
				SIP_S8bit *pGloblaSpecifier2=SIP_NULL;
				if(sip_getAreaSpecifierAtIndexFromTelLocalNum(pTel2->pLocal,\
					&pGloblaSpecifier2,j,pError)==SipFail)
				{
					return SipFail;
				}
				if(sip_strcasecmp(pGloblaSpecifier2,pGloblaSpecifier1) ==0)
				{
					dAreaSpecifierMatch=SipSuccess;
					break;	
				}
			}
			if(dAreaSpecifierMatch !=SipSuccess)
				return SipFail;
		}

	}
	else 
	{	
		if( pTel2->pLocal != SIP_NULL )
			return SipFail;
	}
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getGlobalNumFromTelUrl
** Description: gets the global number from the TelUrl structure
** Parameters:
**		pUrl (IN)		- TelUrl
**		ppLocal (OUT)	- retrieved global number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getGlobalNumFromTelUrl
	(TelUrl *pUrl,
	TelGlobalNum **ppGlobal,
	SipError *pErr)
{
	SIPDEBUGFN("Entering sip_getGlobalNumFromTelUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppGlobal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif

	if (pUrl->pGlobal == SIP_NULL)
	{
		*pErr=E_NO_EXIST;
		return SipFail;
	}
	HSS_LOCKEDINCREF(pUrl->pGlobal->dRefCount);
	*ppGlobal = pUrl->pGlobal;
	SIPDEBUGFN("Exiting sip_getGlobalNumFromTelUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setGlobalNumInTelUrl
** Description: sets the global number in the TelUrl structure
** Parameters:
**		pUrl (IN/OUT)	- TelUrl
**		pGlobal (OUT)	- Global number to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setGlobalNumInTelUrl
#ifdef ANSI_PROTO
	(TelUrl *pUrl,
	TelGlobalNum *pGlobal,
	SipError *pErr)
#else
	(pUrl,pGlobal,pErr)
	TelUrl *pUrl;
	TelGlobalNum *pGlobal;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_setGlobalNumInTelUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pUrl->pGlobal != SIP_NULL)
		sip_freeTelGlobalNum(pUrl->pGlobal);
	if (pGlobal==SIP_NULL)
	{
		pUrl->pGlobal=SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pGlobal->dRefCount);
		pUrl->pGlobal=pGlobal;
	}	
	SIPDEBUGFN("Exitting sip_setGlobalNumInTelUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getLocalNumFromTelUrl
** Description: gets the local url from the TelUrl structure
** Parameters:
**		pUrl (IN)		- TelUrl
**		ppLocal (OUT)	- retrieved local number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getLocalNumFromTelUrl
	(TelUrl *pUrl,
	TelLocalNum **ppLocal,
	SipError *pErr)
{
	SIPDEBUGFN("Entering sip_getLocalNumFromTelUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppLocal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pUrl->pLocal == SIP_NULL)
	{
		*pErr=E_NO_EXIST;
		return SipFail;
	}

	HSS_LOCKEDINCREF(pUrl->pLocal->dRefCount);
	*ppLocal = pUrl->pLocal;
	SIPDEBUGFN("Exiting sip_getLocalNumFromTelUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***********************************************************************
** Function: sip_setLocalNumInTelUrl
** Description: sets the local url from the TelUrl structure
** Parameters:
**		pUrl (IN/OUT)	- TelUrl
**		pLocal (OUT)	- local number to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setLocalNumInTelUrl
#ifdef ANSI_PROTO
	(TelUrl *pUrl,
	TelLocalNum *pLocal,
	SipError *pErr)
#else
	(pUrl,pLocal,pErr)
	TelUrl *pUrl;
	TelLocalNum *pLocal;
	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering sip_setLocalNumInTelUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if ( pUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pUrl->pLocal != SIP_NULL)
		sip_freeTelLocalNum(pUrl->pLocal);
	if (pLocal==SIP_NULL)
	{
		pUrl->pLocal=SIP_NULL;
	}
	else
	{	
		HSS_LOCKEDINCREF(pLocal->dRefCount);
		pUrl->pLocal=pLocal;
	}	
	SIPDEBUGFN("Exitting sip_setLocalNumInTelUrl\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/**********************************/
/*apis for TelLocalNum*/
/*******************************/
/***********************************************************************
** Function: sip_getLocalPhoneDigitFromTelLocalNum
** Description: gets the LocalPhoneDigits from the TelLocalNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum
**		ppPhone (OUT)	- retrieved local phone digit
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getLocalPhoneDigitFromTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	SIP_S8bit **ppPhone,
	SipError *pErr)
#else
	(pLocal, ppPhone, pErr)
	TelLocalNum *pLocal;
	SIP_S8bit **ppPhone;
	SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_PhoneDigit;
	SIPDEBUGFN( "Entering getLocalPhoneDigitFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pLocal == SIP_NULL) || ( ppPhone == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pTemp_PhoneDigit = pLocal->pLocalPhoneDigit;
	if( pTemp_PhoneDigit == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppPhone = pTemp_PhoneDigit;
	SIPDEBUGFN( "Exiting getLocalPhoneDigitFromTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



/***********************************************************************
** Function: sip_setLocalPhoneDigitInTelLocalNum
** Description: sets the LocalPhoneDigits in the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum
**		pPhoneNum (IN)		- LocalPhoneDigit to be set
**		pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setLocalPhoneDigitInTelLocalNum
#ifdef ANSI_PROTO
	( TelLocalNum *pLocal,
	SIP_S8bit *pPhoneNum,
	SipError *pErr  )
#else
	( pLocal,pPhoneNum,pErr )
	TelLocalNum *pLocal;
	SIP_S8bit *pPhoneNum;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering setLocalPhoneDigitInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( pLocal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pLocal->pLocalPhoneDigit !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(\
			pLocal->pLocalPhoneDigit),pErr) ==SipFail)
			return SipFail;
	}
	pLocal->pLocalPhoneDigit = pPhoneNum;
	SIPDEBUGFN( "Exitting setLocalPhoneDigitInTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getIsdnNoFromTelLocalNum
** Description: gets the isdn sub address from the TelLocalNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum
**		ppIsdnno (OUT)	- retrieved isdn sub address
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getIsdnNoFromTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	SIP_S8bit **ppIsdnno,
	SipError *pErr)
#else
	(pLocal, ppIsdnno, pErr)
	TelLocalNum *pLocal;
	SIP_S8bit **ppIsdnno;
	SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_IsdnNo;
	SIPDEBUGFN( "Entering getIsdnNoFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if (( pLocal == SIP_NULL) || (ppIsdnno  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pTemp_IsdnNo = pLocal->pIsdnSubAddr;
	if( pTemp_IsdnNo == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppIsdnno =pTemp_IsdnNo ;
	SIPDEBUGFN( "Exiting getLocalPhoneDigitFromTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



/***********************************************************************
** Function: sip_setIsdnNoInTelLocalNum
** Description: sets the isdn no in the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum
**		pIsdnno (IN)	- isdn no to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setIsdnNoInTelLocalNum
#ifdef ANSI_PROTO
	( TelLocalNum *pLocal,
	SIP_S8bit *pIsdnno,
	SipError *pErr  )
#else
	( pLocal,pIsdnno,pErr )
	TelLocalNum *pLocal;
	SIP_S8bit *pIsdnno;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering setIsdnNoInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( pLocal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pLocal->pIsdnSubAddr !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pLocal->pIsdnSubAddr),\
			pErr) ==SipFail)
			return SipFail;
	}
	pLocal->pIsdnSubAddr = pIsdnno;
	SIPDEBUGFN( "Exitting setIsdnNoInTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***********************************************************************
** Function: sip_getPostdialFromTelLocalNum
** Description: gets the post dial from the TelLocaLNum structure
** Parameters:
**		pLocal (IN)		- TelLocalNum
**		ppPostd (OUT)	- retreived post dial field
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getPostdialFromTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	SIP_S8bit **ppPostd,
	SipError *pErr)
#else
	(pLocal, ppPostd, pErr)
	TelLocalNum *pLocal;
	SIP_S8bit **ppPostd;
	SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_Postd;
	SIPDEBUGFN( "Entering getIsdnNoFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if (( pLocal == SIP_NULL) || (ppPostd == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pTemp_Postd = pLocal->pPostDial;
	if( pTemp_Postd == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppPostd =pTemp_Postd ;
	SIPDEBUGFN( "Exiting getIsdnNoFromTelLocalNum ");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}


/***********************************************************************
** Function: sip_setPostdialInTelLocalNum
** Description: sets the post dial in   the TelLocalNum structure
** Parameters:
**		pLocal (IN/OUT)		- TelLocalNum
**		ppPostd (OUT)	- retreived post dial field
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setPostdialInTelLocalNum
#ifdef ANSI_PROTO
	( TelLocalNum *pLocal,
	SIP_S8bit *pPostd,
	SipError *pErr )
#else
	( pLocal,pPostd,pErr )
	TelLocalNum *pLocal;
	SIP_S8bit *pPostd;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering sip_setPostdialInTelLocalNum ");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( pLocal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pLocal->pPostDial !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pLocal->pPostDial),pErr) \
			==SipFail)
			return SipFail;
	}
	pLocal->pPostDial = pPostd;
	SIPDEBUGFN( "Exitting sip_setPostdialInTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getParamCountFromTelLocalNum
** Description: gets the number of parameters in TelLocalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pLocal (IN)	- TelLocalNum
**		pCount (OUT)	- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getParamCountFromTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,SIP_U32bit *pCount,SipError *pErr)
#else
 	(pLocal,pCount,pErr)
	TelLocalNum *pLocal;
 	SIP_U32bit *pCount;
 	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_getParamCountFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pCount == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf( &(pLocal)->slParams, pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamCountFromTelLocalNum");
	return SipSuccess;
}




/*****************************************************************************
** Function: sip_getParamAtIndexInTelLocalNum
** Description: gets the Param at the specified index in TelLocalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pLocal (IN)	- TelLocalNum
**	ppParam(OUT)	- retreived Parameter
**	dIndex (IN)		- index at which param is to be retieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
SipBool sip_getParamAtIndexFromTelLocalNum
	(TelLocalNum *pLocal,SipParam **ppParam, SIP_U32bit dIndex,\
		SipError *pErr)
{
	SIP_Pvoid element_from_list;
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_getParamAtIndexFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
	if ( ppParam == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (ppParam == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (sip_listGetAt( &(pLocal->slParams), dIndex,  \
		&element_from_list, pErr) == SipFail)
		return SipFail;

	if (element_from_list == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	temp_param = (SipParam *)element_from_list;
	HSS_LOCKEDINCREF(temp_param->dRefCount);
	*ppParam = temp_param;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamAtIndexFromTelLocalNum");
	return SipSuccess;
}


/*****************************************************************************
** Function: sip_setParamAtIndexInTelLocalNum
** Description: sets the Param at the specified index in TelLocalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pLocal (IN/OUT)	- TelLocalNum
**	pParam(IN)			- Param to be set
**	dIndex (IN)			- index at which param is set in Tel Local number
**	pErr (OUT)			- Possible Error value (see API ref doc)
******************************************************************************/
SipBool sip_setParamAtIndexInTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
#else
	(pLocal,pParam, dIndex, pErr)
	TelLocalNum *pLocal;
	SipParam *pParam;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_setParamAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pParam == SIP_NULL )
		temp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		temp_param = pParam;
	}

	if( sip_listSetAt( &(pLocal->slParams),  \
		dIndex, (SIP_Pvoid)(temp_param), pErr) == SipFail)
	{
		if (temp_param != SIP_NULL)
            HSS_LOCKEDDECREF(pParam->dRefCount);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setParamAtIndexInTelLocalNum");
	return SipSuccess;
}




/******************************************************************************
** Function: sip_insertParamAtIndexInTelLocalNum
** Description: inserts the Param at the specified index in TelLocalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pLocal (IN/OUT)- TelLocalNum
**	pParam(IN)		- Param to be inserted
**	dIndex (IN)		- index at which param is inserted in Tel Local number
**	pErr (OUT)		- Possible Error value (see API ref doc)
*******************************************************************************/
SipBool sip_insertParamAtIndexInTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal, SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
#else
	(pLocal, pParam, dIndex, pErr)
	TelLocalNum *pLocal;
	SipParam *pParam;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_insertParamAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if ( pParam == SIP_NULL )
		temp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		temp_param = pParam;
	}

	if( sip_listInsertAt( &(pLocal->slParams),  \
		dIndex, (SIP_Pvoid)(temp_param), pErr) == SipFail)
	{
		if (temp_param != SIP_NULL)
            HSS_LOCKEDDECREF(pParam->dRefCount);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_insertParamAtIndexInTelLocalNum");
	return SipSuccess;
}



/***********************************************************************
** Function: sip_deleteParamAtIndexInTelLocalNum
** Description: deletes the param at the specified index in TelLocalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pLocal (IN)	- TelLocalNum
**	dIndex (IN)		- index at which param is deleted in Tel Local number
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_deleteParamAtIndexInTelLocalNum
#ifdef ANSI_PROTO
(TelLocalNum *pLocal, SIP_U32bit dIndex, SipError *pErr)
#else
(pLocal, dIndex, pErr)
TelLocalNum *pLocal;
SIP_U32bit dIndex;
SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_deleteParamAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if( sip_listDeleteAt( &(pLocal->slParams), dIndex, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_deleteParamAtIndexInTelLocalNum");
	return SipSuccess;
}


/***************************************************************
** Function: sip_getAreaSpecifierCountFromTelLocalNum
** Description: gets the scheme from the TelUrl structure
** Parameters:
**	pLocal (IN)	- TelLocalNum
**	pCount (OUT)	- number of Tel AreaSpecifiers
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getAreaSpecifierCountFromTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	SIP_U32bit *pCount,
	SipError *pErr)
#else
	(pLocal, pCount, pErr)
	TelLocalNum *pLocal;
	SIP_U32bit *pCount;
	SipError *pErr;
#endif
{
	SIPDEBUGFN ( "Entering getAreaSpecifierCountFromTelLocalNum");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if ( (pLocal == SIP_NULL) || ( pCount == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pLocal->slAreaSpecifier), pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting getAreaSpecifierCountFromTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getAreaSpecifierAtIndexFromTelLocalNum
** Description: gets the AreaSpecifier field at the index from TelLocalNum
** Parameters:
**	pLocal (IN)			- TelLocalNum
**	ppAreaSpecifier(OUT)- retreived AreaSpecifier
**	dIndex (IN)			- index at which AreaSpecifier is to be retrieved
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getAreaSpecifierAtIndexFromTelLocalNum
	(TelLocalNum *pLocal,
	 SIP_S8bit **ppAreaSpecifier,
	 SIP_U32bit 	dIndex,
	 SipError 	*pErr )
{
	SIP_Pvoid pElementFromList;

	SIPDEBUGFN ( "Entering sip_getAreaSpecifierAtIndexFromTelLocalNum ");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( (pLocal == SIP_NULL) || (ppAreaSpecifier == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pLocal->slAreaSpecifier), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;

	if ( pElementFromList == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	*ppAreaSpecifier = (SIP_S8bit*)pElementFromList;
	SIPDEBUGFN ( "Exitting sip_getAreaSpecifierAtIndexFromTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setAreaSpecifierAtIndexFromTelLocalNum
** Description: sets the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be set
**	dIndex (IN)			- index at which AreaSpecifier is to be set
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool  sip_setAreaSpecifierAtIndexInTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	 SIP_S8bit *pAreaSpecifier,
	 SIP_U32bit 	dIndex,
	 SipError 	*pErr )
#else
	(pLocal,pAreaSpecifier,dIndex,pErr)
	 TelLocalNum *pLocal;
	 SIP_S8bit *pAreaSpecifier;
  	 SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList;
	SipError tempErr;		/* used in freeing memory after an pError has happened */

	SIPDEBUGFN ( "Entering sip_setAreaSpecifierAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pLocal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pAreaSpecifier  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pAreaSpecifier;
	}

	if( sip_listSetAt(&(pLocal->slAreaSpecifier), dIndex, pElementFromList, \
		pErr) == SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)), \
				&tempErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_setAreaSpecifierAtIndexInTelLocalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_insertAreaSpecifierAtIndexInTelLocalNum
** Description: inserts the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be inserted
**	dIndex (IN)			- index at which AreaSpecifier is to be inserted
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_insertAreaSpecifierAtIndexInTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	 SIP_S8bit *pAreaSpecifier,
	 SIP_U32bit 	dIndex,
	 SipError 	*pErr )
#else
	(pLocal,pAreaSpecifier,dIndex,pErr)
	 TelLocalNum *pLocal;
	 SIP_S8bit *pAreaSpecifier;
    	 SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList;
	SipError tempErr;		/* used in freeing memory after an pError has happened */


	SIPDEBUGFN ( "Entering sip_insertAreaSpecifierAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pLocal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying the Value Headers structure/char*  */
	if (pAreaSpecifier  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pAreaSpecifier	;
	}

	if( sip_listInsertAt(&(pLocal->slAreaSpecifier), dIndex, pElementFromList, \
		pErr) == SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)), \
				&tempErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_insertAreaSpecifierAtIndexInTelLocalNum");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_deleteAreaSpecifierAtIndexInTelLocalNum
** Description: deletes the AreaSpecifier field at the index in TelLocalNum
** Parameters:
**	pLocal (IN/OUT)		- TelLocalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be deleted
**	dIndex (IN)			- index at which AreaSpecifier is to be deleted
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_deleteAreaSpecifierAtIndexInTelLocalNum
#ifdef ANSI_PROTO
	(TelLocalNum *pLocal,
	  SIP_U32bit 	dIndex,
	  SipError 	*pErr )
#else
	(pLocal,dIndex,pErr)
   	 TelLocalNum *pLocal;
	SIP_U32bit 	dIndex;
	SipError 	*pErr;
#endif
{

	SIPDEBUGFN ( "Entering sip_deleteAreaSpecifierAtIndexInTelLocalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pLocal  == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pLocal->slAreaSpecifier), dIndex, pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN ( "Exitting sip_deleteAreaSpecifierAtIndexInTelLocalNum");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}




/**********************************/
/*apis for TelGlobalNum*/
/*******************************/
/***********************************************************************
** Function: sip_getBaseNoFromTelGlobalNum
** Description: gets the  base number from TelGlobalNum structure
** Parameters:
**		pGlobal	(IN)- TelGlobalNum
**		ppBase (IN)		- retreived Base number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getBaseNoFromTelGlobalNum
#ifdef ANSI_PROTO
(TelGlobalNum *pGlobal,SIP_S8bit **ppBase, SipError *pErr )
#else
(pGlobal,ppBase, pErr )
TelGlobalNum *pGlobal;
SIP_S8bit **ppBase;
SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_base;
	SIPDEBUGFN( "Entering getBaseNoFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pGlobal == SIP_NULL) || ( ppBase == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_base = pGlobal->pBaseNo;


	if( pTemp_base == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppBase = pTemp_base;

	SIPDEBUGFN( "Exitting getBaseNoFromTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/***********************************************************************
** Function: sip_setBaseNoInTelGlobalNum
** Description: sets the base number in TelGlobalNum structure
** Parameters:
**		pGlobal	(IN/OUT)- TelGlobalNum
**		pBase (IN)		- Base number to set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setBaseNoInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_S8bit *pBase, SipError *pErr)
#else
	(pGlobal,pBase, pErr)
	TelGlobalNum *pGlobal;
	SIP_S8bit *pBase;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering setBaseNoInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pGlobal->pBaseNo  !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pGlobal->pBaseNo),pErr) \
			==SipFail)
		return SipFail;
	}
	pGlobal->pBaseNo = pBase;
	SIPDEBUGFN( "Exitting setBaseNoInTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getIsdnNoFromTelGlobalNum
** Description: gets the isdn number from TelGlobalNum
** Parameters:
**		pGlobal	(IN)	- TelGlobalNum
**		ppIsdnno (IN)	- retreived isdn number
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getIsdnNoFromTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_S8bit **ppIsdnno, SipError *pErr)
#else
	(pGlobal,ppIsdnno,pErr)
	TelGlobalNum *pGlobal;
	SIP_S8bit **ppIsdnno;
	SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_isdnno;
	SIPDEBUGFN( "Entering getIsdnNoFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pGlobal == SIP_NULL) || ( ppIsdnno  == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_isdnno = pGlobal->pIsdnSubAddr;


	if( pTemp_isdnno == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppIsdnno = pTemp_isdnno;

	SIPDEBUGFN( "Exitting getIsdnNoFromTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}



/***********************************************************************
** Function: sip_setIsdnNoInTelGlobalNum
** Description: sets the isdn number in TelGlobalNum
** Parameters:
**		pGlobal (IN/OUT)- TelGlobalNum
**		pIsdnno(IN)		- Isdn number to set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setIsdnNoInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_S8bit *pIsdnno, SipError *pErr)
#else
	(pGlobal,pIsdnno, pErr)
	TelGlobalNum *pGlobal;
	SIP_S8bit *pIsdnno;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering setIsdnNoInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pGlobal->pIsdnSubAddr  !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pGlobal->pIsdnSubAddr),\
			pErr) ==SipFail)
			return SipFail;
	}
	pGlobal->pIsdnSubAddr = pIsdnno;
	SIPDEBUGFN( "Exitting setIsdnNoInTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



/***********************************************************************
** Function: sip_getPostdialFromTelGlobalNum
** Description: gets the PostDial field from TelGlobalNum
** Parameters:
**		pGlobal (IN)	- TelGlobalNum
**		ppPostd(OUT)	- retreived PostDial fiels
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getPostdialFromTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_S8bit **ppPostd, SipError *pErr)
#else
	(pGlobal,ppPostd,pErr)
	TelGlobalNum *pGlobal;
	SIP_S8bit **ppPostd;
	SipError *pErr;
#endif
{
	SIP_S8bit * pTemp_postd;
	SIPDEBUGFN( "Entering getPostdialFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (( pGlobal == SIP_NULL) || (ppPostd == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	pTemp_postd = pGlobal->pPostDial;


	if( pTemp_postd == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	*ppPostd= pTemp_postd;

	SIPDEBUGFN( "Exitting getPostDialFromTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;

}


/***********************************************************************
** Function: sip_setPostdialInTelGlobalNum
** Description: sets the PostDial field in TelGlobalNum
** Parameters:
**		pGlobal (IN/OUT)- TelGlobalNum
**		pPostd(IN)		- PostDial to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setPostdialInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_S8bit *pPostd,SipError *pErr )
#else
	(pGlobal,pPostd,pErr )
	TelGlobalNum *pGlobal;
	SIP_S8bit *pPostd;
	SipError *pErr;
#endif
{
	SIPDEBUGFN( "Entering setPostDialInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if(pGlobal->pPostDial  !=SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID,(SIP_Pvoid*)&(pGlobal->pPostDial),\
			pErr) ==SipFail)
			return SipFail;
	}
	pGlobal->pPostDial = pPostd;
	SIPDEBUGFN( "Exitting setpostdialInTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



/***********************************************************************
** Function: sip_getParamCountFromTelGlobalNum
** Description: gets the number of parameters in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**		pGlobal (IN)	- TelGlobalNum
**		pCount (OUT)	- number of parameters
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
 SipBool sip_getParamCountFromTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,SIP_U32bit *pCount, SipError *pErr)
#else
 	(pGlobal,pCount,pErr)
	TelGlobalNum *pGlobal;
 	SIP_U32bit *pCount;
 	SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_getParamCountFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (pCount == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf( &(pGlobal)->slParams, pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamCountFromTelGlobalNum");
	return SipSuccess;
}




/*****************************************************************************
** Function: sip_getParamAtIndexInTelGlobalNum
** Description: gets the Param at the specified index in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN)	- TelGlobalNum
**	pParam(OUT)		- retreived Parameter
**	dIndex (IN)		- index at which param is to be retieved
**	pErr (OUT)		- Possible Error value (see API ref doc)
******************************************************************************/
SipBool sip_getParamAtIndexFromTelGlobalNum
	(TelGlobalNum *pGlobal,SipParam **ppParam, SIP_U32bit dIndex,\
		SipError *pErr)
{
	SIP_Pvoid element_from_list;
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_getParamAtIndexFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if (ppParam == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (sip_listGetAt( &(pGlobal->slParams), dIndex,  \
		&element_from_list, pErr) == SipFail)
		return SipFail;

	if (element_from_list == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	temp_param = (SipParam *)element_from_list;
	HSS_LOCKEDINCREF(temp_param->dRefCount);
	*ppParam = temp_param;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getParamAtIndexFromTelGlobalNum");
	return SipSuccess;
}


/*****************************************************************************
** Function: sip_setParamAtIndexInTelGlobalNum
** Description: sets the Param at the specified index in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN/OUT)	- TelGlobalNum
**	pParam(IN)			- Param to be set
**	dIndex (IN)			- index at which param is set in Tel Global number
**	pErr (OUT)			- Possible Error value (see API ref doc)
******************************************************************************/
SipBool sip_setParamAtIndexInTelGlobalNum
	(TelGlobalNum *pGlobal,SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
{
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_setParamAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pParam == SIP_NULL )
		temp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		temp_param = pParam;
	}

	if( sip_listSetAt( &(pGlobal->slParams),  \
		dIndex, (SIP_Pvoid)(temp_param), pErr) == SipFail)
	{
		if (temp_param != SIP_NULL)
            HSS_LOCKEDDECREF(pParam->dRefCount);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setParamAtIndexInTelGlobalNum");
	return SipSuccess;
}




/******************************************************************************
** Function: sip_insertParamAtIndexInTelGlobalNum
** Description: inserts the Param at the specified index in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN/OUT)- TelGlobalNum
**	pParam(IN)		- Param to be inserted
**	dIndex (IN)		- index at which param is inserted in Tel Global number
**	pErr (OUT)		- Possible Error value (see API ref doc)
*******************************************************************************/
 SipBool sip_insertParamAtIndexInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal, SipParam *pParam, SIP_U32bit dIndex, \
		SipError *pErr)
#else
	(pGlobal, pParam, dIndex, pErr)
	TelGlobalNum *pGlobal;
	SipParam *pParam;
	SIP_U32bit dIndex;
	SipError *pErr;
#endif
{
	SipParam *temp_param;
	SIPDEBUGFN("Entering function sip_insertParamAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if ( pParam == SIP_NULL )
		temp_param = SIP_NULL;
	else
	{
		HSS_LOCKEDINCREF(pParam->dRefCount);
		temp_param = pParam;
	}

	if( sip_listInsertAt( &(pGlobal->slParams),  \
		dIndex, (SIP_Pvoid)(temp_param), pErr) == SipFail)
	{
		if (temp_param != SIP_NULL)
            HSS_LOCKEDDECREF(pParam->dRefCount);
		return SipFail;
	}

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_insertParamAtIndexInTelGlobalNum");
	return SipSuccess;
}



/***********************************************************************
** Function: sip_deleteParamAtIndexInTelGlobalNum
** Description: deletes the param at the specified index in TelGlobalNum
**             (service provider and future extension are treated as parameters)
** Parameters:
**	pGlobal (IN)	- TelGlobalNum
**	dIndex (IN)		- index at which param is deleted in Tel Global number
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
 SipBool sip_deleteParamAtIndexInTelGlobalNum
#ifdef ANSI_PROTO
(TelGlobalNum *pGlobal, SIP_U32bit dIndex, SipError *pErr)
#else
(pGlobal, dIndex, pErr)
TelGlobalNum *pGlobal;
SIP_U32bit dIndex;
SipError *pErr;
#endif
{
	SIPDEBUGFN("Entering function sip_deleteParamAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#endif
	if( sip_listDeleteAt( &(pGlobal->slParams), dIndex, pErr) == SipFail)
		return SipFail;

	*pErr = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_deleteParamAtIndexInTelGlobalNum");
	return SipSuccess;
}




/***************************************************************
** Function: sip_getAreaSpecifierCountFromTelGlobalNum
** Description: gets the scheme from the TelUrl structure
** Parameters:
**	pGlobal (IN)	- TelGlobalNum
**	pCount (OUT)	- number of Tel AreaSpecifiers
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getAreaSpecifierCountFromTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,
	SIP_U32bit *pCount,
	SipError *pErr)
#else
	(pGlobal, pCount, pErr)
	TelGlobalNum *pGlobal;
	SIP_U32bit *pCount;
	SipError *pErr;
#endif
{
	SIPDEBUGFN ( "Entering getAreaSpecifierCountFromTelGlobalNum");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL )
		return SipFail;

	if ( (pGlobal == SIP_NULL) || ( pCount == SIP_NULL ))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pGlobal->slAreaSpecifier), pCount , pErr) == SipFail )
	{
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting getAreaSpecifierCountFromTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_getAreaSpecifierAtIndexFromTelGlobalNum
** Description: gets the AreaSpecifier field at the index from TelGlobalNum
** Parameters:
**	pGlobal (IN)			- TelGlobalNum
**	ppAreaSpecifier(OUT)- retreived AreaSpecifier
**	dIndex (IN)			- index at which AreaSpecifier is to be retrieved
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_getAreaSpecifierAtIndexFromTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,
	 SIP_S8bit **ppAreaSpecifier,
	 SIP_U32bit dIndex,
	 SipError 	*pErr )
#else
	(pGlobal,ppAreaSpecifier,dIndex,pErr)
	 TelGlobalNum *pGlobal;
	 SIP_S8bit **ppAreaSpecifier;
    	 SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{
	SIP_Pvoid pElementFromList;

	SIPDEBUGFN ( "Entering sip_getAreaSpecifierAtIndexFromTelGlobalNum ");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( (pGlobal == SIP_NULL) || (ppAreaSpecifier == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pGlobal->slAreaSpecifier), dIndex, &pElementFromList, \
		pErr) == SipFail)
		return SipFail;

	if ( pElementFromList == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}

	*ppAreaSpecifier = (SIP_S8bit*)pElementFromList;
	SIPDEBUGFN ( "Exitting sip_getAreaSpecifierAtIndexFromTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_setAreaSpecifierAtIndexFromTelGlobalNum
** Description: sets the AreaSpecifier field at the index in TelGlobalNum
** Parameters:
**	pGlobal (IN/OUT)		- TelGlobalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be set
**	dIndex (IN)			- index at which AreaSpecifier is to be set
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool  sip_setAreaSpecifierAtIndexInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,
	 SIP_S8bit *pAreaSpecifier,
	 SIP_U32bit 	dIndex,
	 SipError 	*pErr )
#else
	(pGlobal,pAreaSpecifier,dIndex,pErr)
	 TelGlobalNum *pGlobal;
	 SIP_S8bit *pAreaSpecifier;
    	 SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList;
	SipError tempErr;	/* used in freeing memory after an pError has happened */

	SIPDEBUGFN ( "Entering sip_setAreaSpecifierAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pGlobal == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (pAreaSpecifier  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pAreaSpecifier;
	}

	if( sip_listSetAt(&(pGlobal->slAreaSpecifier), dIndex, pElementFromList, \
		pErr) == SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)), \
				&tempErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_setAreaSpecifierAtIndexInTelGlobalNum");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_insertAreaSpecifierAtIndexInTelGlobalNum
** Description: inserts the AreaSpecifier field at the index in TelGlobalNum
** Parameters:
**	pGlobal (IN/OUT)		- TelGlobalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be inserted
**	dIndex (IN)			- index at which AreaSpecifier is to be inserted
**	pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_insertAreaSpecifierAtIndexInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,
	 SIP_S8bit *pAreaSpecifier,
	 SIP_U32bit 	dIndex,
	 SipError 	*pErr )
#else
	(pGlobal,pAreaSpecifier,dIndex,pErr)
	 TelGlobalNum *pGlobal;
	 SIP_S8bit *pAreaSpecifier;
     SIP_U32bit dIndex;
	 SipError *pErr;
#endif
{

	SIP_S8bit * pElementFromList;
	SipError tempErr;		/* used in freeing memory after an pError has happened */


	SIPDEBUGFN ( "Entering sip_insertAreaSpecifierAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pGlobal == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying the Value Headers structure/char*  */
	if (pAreaSpecifier  == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
		pElementFromList = pAreaSpecifier	;
	}

	if( sip_listInsertAt(&(pGlobal->slAreaSpecifier), dIndex, pElementFromList,\
		pErr) == SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)), \
				&tempErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting sip_insertAreaSpecifierAtIndexInTelGlobalNum");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** Function: sip_deleteAreaSpecifierAtIndexInTelGlobalNum
** Description: deletes the AreaSpecifier field at the index in TelGlobalNum
** Parameters:
**	pGlobal (IN/OUT)		- TelGlobalNum
**	pAreaSpecifier(IN)	- AreaSpecifier to be deleted
**	dIndex (IN)			- index at which AreaSpecifier is to be deleted
**	pErr (OUT)			- Possible Error value (see API ref doc)
************************************************************************/

SipBool sip_deleteAreaSpecifierAtIndexInTelGlobalNum
#ifdef ANSI_PROTO
	(TelGlobalNum *pGlobal,
	  SIP_U32bit 	dIndex,
	  SipError 	*pErr )
#else
	(pGlobal,dIndex,pErr)
  	TelGlobalNum *pGlobal;
	SIP_U32bit 	dIndex;
	SipError 	*pErr;
#endif
{

	SIPDEBUGFN ( "Entering sip_deleteAreaSpecifierAtIndexInTelGlobalNum");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pGlobal  == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pGlobal->slAreaSpecifier), dIndex, pErr) == SipFail)
		return SipFail;

	SIPDEBUGFN ( "Exitting sip_deleteAreaSpecifierAtIndexInTelGlobalNum");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
