/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
** FUNCTION:
**          Tel Url Parsing facility
************************************************************************
**
** FILENAME:
** microsip_telparser.c
**
** DESCRIPTION:
**		This file contains functions for parsing tel: URL
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 23/08/2002	Mahesh Govind		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#include "microsip_telstruct.h"
#include "microsip_telinit.h"
#include "microsip_telfree.h"
#include "microsip_telparser.h"
#include "microsip_subparsers.h"
#include "microsip_statistics.h"
#include "microsip_trace.h"
#define MAX_TEL_URL_SIZE 1024
/***********************************************************************
** FUNCTION: sip_parseTelUrl
** DESCRIPTION: This function does the parsing of telurl 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**              pEnd(IN)        - The end point of the input buffer
**				TelUrl(IN/Out)	- TelUrl structure returned
** NOTE: 
************************************************************************/

SipBool sip_parseTelUrl (SIP_S8bit	*pStart,	TelUrl	\
	*pTelUrl,SipError	*pErr)
	
{

	TelLocalNum		*pLocalNum	=	SIP_NULL;
	TelGlobalNum	*pGlobalNum	=	SIP_NULL;
	SIP_S8bit *pCurrentPos		=	SIP_NULL;
	SIP_S8bit *pNumEnd			=	SIP_NULL;
	SIP_S8bit *pNum				=	SIP_NULL;
	en_SipBoolean	numberOnly	=	SipFalse;
	SIP_S8bit *pEnd				=	SIP_NULL;

	/*
	* tel:+12345
	* The higher level function getTelUrlFrom AddrSpec will always check
	* whether the given URI is a tel URL.So here we dont need to  check the
	* same again.The higer level function getTelUrlFroAddrSpec will give the
	* input pStart as +12345.This will reduce the amount of processing.
	* So we need to check only whether the given number is a TelLocalNum or a
	* TelGlobalNumber.
	*		 telephone-uri       =  "tel:" subscriber *param                
	*       subscriber          =  global-number / local-number  
	*       global-number       =  global-number-part [isdn-subaddress]
	*       global-number-part  =  "+" 1*phonedigit
	*       local-number        =  local-number-part [isdn-subaddress]
	*                              [context]                                    
	*/
	pEnd = pStart + (sip_strlen(pStart)-1);	
	pCurrentPos	= sip_stripLeadingLWS(pStart ,pEnd);
	pEnd		= sip_stripTrailingLWS(pStart ,pEnd);
	
	/*
	* Now we have stripped the leading white space.We have the telephone number.
	* if the number starts with "+" it is a telephone global number else 
	* it is a telelphone local number.
	*/
	if (*pCurrentPos == '+')
	{
		/*
		 * Bypassing +
		 */
		pCurrentPos = sip_stripLeadingLWS(pCurrentPos+1 ,pEnd);
		
		/*
		 * telGlobal number and do parsing
		 */
		/*
		 * Find the end of the  number
		 */
		
		pNumEnd=sip_tokenize(pCurrentPos , pEnd , ';');
		/*
		 * If there is no ; this means only the number is there 
		 * no other params.
		 */
		if (pNumEnd == SIP_NULL)
		{
			pNumEnd		= pEnd;
			numberOnly	= SipTrue;
			
		}else
		{
			pNumEnd--;
		}
		
		/*
		 * Validate the phone number to see whether any illegal characters
		 * are there
		 */ 
		pNum = sip_stripTrailingLWS(pCurrentPos ,pNumEnd);
		if (sip_validatePhoneNumber(pCurrentPos ,pNum, pErr) == SipFail)	
		{
			return SipFail;
		
		}
		if (sip_initTelGlobalNum(&pGlobalNum , pErr) == SipFail )
		{
			return SipFail;
		}
		pTelUrl->pGlobal = pGlobalNum;
	
		pGlobalNum->pBaseNo = sip_nStrdup(DECODE_MEM_ID ,pCurrentPos ,\
				(pNum - pCurrentPos)+1 ,pErr);
		/*
		 * If we have only numbers and no parameters we can stop 
		 * parsing and at this pont we can return a success
		 */
		if (numberOnly)	
		{
			*pErr = E_NO_ERROR;
			return SipSuccess;
		}	
		/*
		 * Try to get the parameters
		 * First we are checking for isub=
		 * If we come here this means there is params after number.
		 */ 
		pNumEnd = sip_stripLeadingLWS( pNumEnd+2 , pEnd);
		pCurrentPos = sip_tokenize(pNumEnd ,pEnd , ';');
		if (pCurrentPos == SIP_NULL)
		{
			pCurrentPos = pEnd;
			/*
			 * Following variable is set true if there is no 
			 * params after isub
			 */ 
			numberOnly = SipTrue;
		}
		else
		{
			pCurrentPos--;
		}
		/*
		 * Now tokenize for = to get isub
		 */
		
		pStart = sip_tokenize(pNumEnd ,pCurrentPos , '=');
		if ( pStart != SIP_NULL)
		{
			/*
			 * If = is there and it is an ISUB then store the same
			 */
			if (sip_strncasecmp((SIP_S8bit*)"isub" , pNumEnd , 4) == 0)
			{
				pGlobalNum->pIsdnSubAddr = sip_nStrdup(DECODE_MEM_ID ,pStart+1 \
					, pCurrentPos- pStart , pErr);	
					if (pGlobalNum->pIsdnSubAddr == SIP_NULL)
					{
						return SipFail;
					}
				/*
				 * After parsing isub=  start parsing  SipParam
				 */
				/*
				 * If you have nothing after isub no need to proceed
				 * parsing after that
				 */ 
				if (!numberOnly)
				{
					pCurrentPos = sip_stripLeadingLWS(pCurrentPos+2 , pEnd);
					if (*pCurrentPos != '\0')
					{
						if (SipFail == sip_parseSipParam(&(pGlobalNum->\
							slParams), pCurrentPos ,pEnd , SipFalse ,';', \
										pErr))
						{		
							/*sip_freeTelGlobalNum(pGlobalNum);*/
							return SipFail;
						}	
					 }	
				 }

			
				/*
				 * If ISUB is parsed succesfully
				 */
			}/* End of ISUB */
			else/*If it is not isub .....*/
			{
				/*
				 * Call the parameter parser to parse it
				 */
					if (SipFail == sip_parseSipParam(&(pGlobalNum->slParams), \
						pNumEnd ,pEnd , SipFalse,';', pErr))
					{	
/*						sip_freeTelGlobalNum(pGlobalNum);*/
						return SipFail;
					}	
			}
		}
		else
		{
			if (SipFail == sip_parseSipParam(&(pGlobalNum->slParams), \
				pNumEnd ,pCurrentPos , SipFalse,';', pErr))
			{	
/*				sip_freeTelGlobalNum(pGlobalNum);*/
				return SipFail;
			}	
		}
				
	}/*End of Tel Global Number*/
	else
	{
		/**
		 ** initialize telLocal number
		 **/
		/*
		 * Find the end of the  number
		 */
		pNumEnd=sip_tokenize(pCurrentPos , pEnd , ';');
		/*
		 * If there is no ; this means only the number is there 
		 * no other params.
		 */
		if (pNumEnd == SIP_NULL)
		{
			pNumEnd		= pEnd;
			numberOnly	= SipTrue;
			
		}else
		{
			pNumEnd--;
		}
		
		/*
		 * Validate the phone number to see whether any illegal characters
		 * are there
		 */ 
		pNum = sip_stripTrailingLWS(pCurrentPos ,pNumEnd);
		if (sip_validatePhoneNumber(pCurrentPos ,pNum ,pErr) == SipFail)	
		{
			*pErr = E_PARSER_ERROR;
			return SipFail;
		
		}
		/*
		 * We need to initialize the local number only if 
		 * the initial validation of telephone number 
		 * succeeds
		 */ 
		if (sip_initTelLocalNum(&pLocalNum , pErr) == SipFail )
		{
			return SipFail;
		}
		pTelUrl->pLocal = pLocalNum;
		
		pLocalNum->pLocalPhoneDigit = sip_nStrdup(DECODE_MEM_ID ,pCurrentPos ,\
				(pNum - pCurrentPos)+1 ,pErr);
		/*
		 * If we have only numbers and no parameters we can stop 
		 * parsing and at this pont we can return a success
		 */
		if (numberOnly)	
		{
			*pErr = E_NO_ERROR;
			return SipSuccess;
		}	
		/*
		 * Try to get the parameters
		 * First we are checking for isub=
		 * If we come here this means there is params after number.
		 */ 
		pNumEnd = sip_stripLeadingLWS( pNumEnd+2 , pEnd);
		pCurrentPos = sip_tokenize(pNumEnd ,pEnd , ';');
		if (pCurrentPos == SIP_NULL)
		{
			pCurrentPos = pEnd;
			/*
			 * Following variable is set true if there is no 
			 * params after isub
			 */ 
			numberOnly = SipTrue;
		}
		else
		{
			pCurrentPos--;
		}
		/*
		 * Now tokenize for = to get isub
		 */
		pStart = sip_tokenize(pNumEnd ,pCurrentPos , '=');
		if ( pStart != SIP_NULL)
		{
			/*
			 * If = is there and it is an ISUB then store the same
			 */
			if (sip_strncasecmp((SIP_S8bit*)"isub" , pNumEnd , 4) == 0)
			{
				pLocalNum->pIsdnSubAddr = sip_nStrdup(DECODE_MEM_ID ,pStart+1 ,\
					pCurrentPos	- pStart , pErr);	
					if (pLocalNum->pIsdnSubAddr == SIP_NULL)
					{
						return SipFail;
					}
				/*
				 * After parsing isub=  Check for SipParam
				 */
				/*
				 * If you have nothing after isub no need to proceed
				 * parsing after that
				 */ 
				if (!numberOnly)
				{
					pCurrentPos = sip_stripLeadingLWS(pCurrentPos+2 , pEnd);
					if (*pCurrentPos != '\0')
					{
						if (SipFail == sip_parseSipParam(&(pLocalNum->slParams)\
							, pCurrentPos ,pEnd , SipFalse ,';', pErr))
						{	
							/*sip_freeTelLocalNum(pLocalNum);*/
							return SipFail;
						}	
					 }	
				 }
				/*
				 * If ISUB is parsed succesfully
				 */
			}/* End of ISUB */
			else/*If it is not isub .....*/
			{
				/*
				 * Call the parameter parser to parse it
				 */
					if (SipFail == sip_parseSipParam(&(pLocalNum->slParams), \
						pNumEnd ,pEnd , SipFalse,';', pErr))
					{	
						/*sip_freeTelLocalNum(pLocalNum);*/
						return SipFail;
					}	
			}
		}
		else
		{
			if (SipFail == sip_parseSipParam(&(pLocalNum->slParams), \
				pNumEnd ,pCurrentPos , SipFalse,';', pErr))
			{	
				/*sip_freeTelLocalNum(pLocalNum);*/
				return SipFail;
			}	
		
		
		}
	}
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** FUNCTION: sip_validatePhoneNumber
**
** DESCRIPTION: This function does the validation of a phone number 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- Number of characters .The end point of 
**								- input buffer
** NOTE: 
************************************************************************/

SipBool sip_validatePhoneNumber(SIP_S8bit *pStart , SIP_S8bit *pEnd ,\
					SipError *pErr)
{
/*
 * Following are the character set for phone digit
 * phonedigit          =  HEXDIG / visual-separator
 * visual-separator    =  "-" / "." / "(" / ")"                          
 */ 
	pStart = sip_stripLeadingLWS(pStart ,pEnd);
	pEnd = sip_stripTrailingLWS(pStart ,pEnd);
	
 while (pStart  <= pEnd) 
 {
	if ((*pStart >=  '0' && *pStart <='9' )  ||\
		((*pStart >='a' && *pStart <= 'f')||\
		(*pStart >='A' && *pStart <= 'F')) ||\
		(*pStart == '-' )||( *pStart == '.' )|| (*pStart == '(')|| 
		( *pStart == ')') || (*pStart == '*') || (*pStart == '#'))
			pStart++;
	else 
	{
		*pErr = E_PARSER_ERROR;
		sip_error(SIP_Minor,"Validate  pHone Number Failed\n");
		return SipFail ;
	}
 }/*End of while loop*/
 return SipSuccess;
}					


/**********************************************************************
** FUNCTION: sip_getTelUrlFromAddrSpec
**
** DESCRIPTION: gets the TelUrl from the SipAddrSpec structure
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pTelUrl (OUT)	- retrieved TelUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getTelUrlFromAddrSpec (
	SipAddrSpec *pAddrSpec, TelUrl **ppTelUrl, SipError *pErr)
{
	SIP_S8bit *pStart =SIP_NULL;

	INC_API_COUNT

	SIPDEBUGFN ("Entering sip_getTelUrlFromAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if ( ppTelUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if (pAddrSpec->u.pUri == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#endif
	if ( sip_isTelUrl(pAddrSpec, pErr) == SipFail)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	pStart = sip_tokenize(pAddrSpec->u.pUri ,(pAddrSpec->u.pUri)+sip_strlen(pAddrSpec->\
		u.pUri), ':');
	if (pStart == SIP_NULL)	
	{	*pErr = E_PARSER_ERROR;
		return SipFail;
	}	
		
	if (SipFail == sip_initTelUrl(ppTelUrl ,pErr))
	{
		*pErr = E_NO_MEM;
		return SipFail;
	
	}
	if (SipFail == sip_parseTelUrl(pStart+1 , *ppTelUrl ,pErr) )
	{

		sip_freeTelUrl(*ppTelUrl);
		return SipFail;
	}
	SIPDEBUGFN ("Exiting sip_getTelUrlFromAddrSpec\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** FUNCTION: sip_isTelUrl
**
** DESCRIPTION: Checks if the Addrspec has a tel-url
**
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_isTelUrl
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, SipError *pErr)
#else
	(pAddrSpec,pErr)
	SipAddrSpec *pAddrSpec;
	SipError *pErr;
#endif
{
	SIP_S8bit *pTempUri;

	SipUrl *pTempUrl;
	SIP_U32bit count =0;
	SIP_U32bit icount =0;
	SipParam* pTempParam;
	SIP_S8bit* pTempValue;
	en_AddrType pAddrtype;

	SIP_U32bit i;
	SIPDEBUGFN("Entering sip_isTelUrl\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	pAddrtype = pAddrSpec->dType;
	if ( pAddrtype == SipAddrReqUri )
	{
		pTempUri = pAddrSpec->u.pUri;

		i=0;
		if (( pTempUri[i] == 'T' || pTempUri[i] == 't') &&
			( pTempUri[i+1] == 'E' || pTempUri[i+1] == 'e')&&
			( pTempUri[i+2] == 'L' || pTempUri[i+2] == 'l'))
		{
			for (i=3;((pTempUri[i] == ' ') || (pTempUri[i] == '\t'))&& \
					(pTempUri[i] != '\0');i++);

			if (pTempUri[i] == ':')
			{
				*pErr = E_NO_ERROR;
				SIPDEBUGFN("Exiting sip_isTelUrl\n");
				return SipSuccess;
			}
		}
		SIPDEBUGFN("Exiting sip_isTelUrl\n");
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	if((pAddrtype != SipAddrSipUri) && (pAddrtype != SipAddrSipSUri))
	{
		SIPDEBUGFN("Exiting sip_isTelUrl\n");
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	pTempUrl = pAddrSpec->u.pSipUrl;

	/* get the count of URL param */
	if (sip_listSizeOf(&(pTempUrl->slParam), &count , pErr) == SipFail )
	{
		return SipFail;
	}

	while ( count > 0 )
	{
		/* get Url Param from the Url at index */
		if( sip_listGetAt(&(pTempUrl->slParam),count-1,\
				(SIP_Pvoid*)&pTempParam, pErr) == SipFail)
			return SipFail;

		if ( sip_strcasecmp(pTempParam->pName,(SIP_S8bit *)"user") == 0 )
		{
			(void)sip_listSizeOf(&(pTempParam->slValue),&icount,pErr);
			if ( icount == 1 )
			{
				if ( sip_listGetAt(&(pTempParam->slValue),0,\
					(SIP_Pvoid*)&pTempValue,pErr) != SipSuccess )
				{
					sip_freeSipParam(pTempParam);
					*pErr = E_INV_PARAM;
					SIPDEBUGFN("Exiting sip_isTelUrl\n");
					return SipFail;
				}
				if ( sip_strcasecmp(pTempValue, (SIP_S8bit *)"phone") == 0 )
				{
					sip_freeSipParam(pTempParam);
					*pErr = E_NO_ERROR;
					SIPDEBUGFN("Exiting sip_isTelUrl\n");
					return SipSuccess;
				}
			}
		}
		count--;
	}

	SIPDEBUGFN("Exiting sip_isTelUrl\n");
	*pErr = E_INV_PARAM;
	return SipFail;
}
/***********************************************************************
** Function: sip_setTelUrlInAddrSpec
** Description: sets the TelUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pTelUrl (OUT)	- TelUrl to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setTelUrlInAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, TelUrl *pTelUrl, SipError *pErr)
#else
	(pAddrSpec, pTelUrl, pErr)
	SipAddrSpec *pAddrSpec;
	TelUrl *pTelUrl;
	SipError *pErr;
#endif
{
	SIP_S8bit *out;
	en_AddrType pAddrtype;

	SIPDEBUGFN("Entering sip_setTelUrlInAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	pAddrtype = pAddrSpec->dType;
	if ( (pAddrtype == SipAddrSipUri) || (pAddrtype == SipAddrSipSUri) )
	{
		if ( pAddrSpec->u.pSipUrl != SIP_NULL )
		{
			sip_freeSipUrl((pAddrSpec->u).pSipUrl);
		}
	}

	if ( pAddrtype == SipAddrReqUri )
	{
		if ( pAddrSpec->u.pUri != SIP_NULL)
		{
			if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pAddrSpec->u.pUri),\
				pErr) ==SipFail)
				return SipFail;
		}
		if (pTelUrl == SIP_NULL)
		{
			pAddrSpec->u.pUri = SIP_NULL;
			*pErr = E_NO_ERROR;
			return SipSuccess;
		}
	}

	pAddrSpec->dType = SipAddrReqUri;
	out=(SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID,MAX_TEL_URL_SIZE, pErr);
	if (out == SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
	(void)sip_strcpy(out, (SIP_S8bit *)"Tel:");
	if (pTelUrl->pGlobal != SIP_NULL)
		(void)sip_formTelGlobalNum(pTelUrl->pGlobal,(SIP_S8bit *)out,pErr);
	else if (pTelUrl->pLocal != SIP_NULL)
		(void)sip_formTelLocalNum(pTelUrl->pLocal,(SIP_S8bit *)out,pErr);
	else {
		(void)fast_memfree(ACCESSOR_MEM_ID, out, pErr);
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	pAddrSpec->u.pUri=out;

	SIPDEBUGFN("Exitting sip_setTelUrlInAddrSpec\n");
	return SipSuccess;
}

/*****************************************************************
** FUNCTION: sip_formTelGlobalNum
** DESCRIPTION: Converts TelGlobalNum to text
** Parameters:
** 		pGlobal(IN)    - The TelGlobalNum to be used
**		out(OUT)       - The output buffer
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formTelGlobalNum(TelGlobalNum *pGlobal,SIP_S8bit* out,SipError *pErr)
{
	(void)STR_CAT(out,(SIP_S8bit*)"+");
	(void)STR_CAT(out,pGlobal->pBaseNo);
	if(pGlobal->pIsdnSubAddr != SIP_NULL)
		(void)STR_CAT(out,(SIP_S8bit*)";isub=");
	(void)STR_CAT(out,pGlobal->pIsdnSubAddr);

	if(pGlobal->pPostDial !=SIP_NULL)
	{
		(void)STR_CAT(out,(SIP_S8bit*)";postd=");

		/* escape here for post dial */
		(void)STR_CAT(out,pGlobal->pPostDial);
	}

	/* extract each element of pGlobal->slAreaSpecifier and escape */
	(void)sip_formSipStringList(out, &(pGlobal->slAreaSpecifier), (SIP_S8bit *) \
		";",1, pErr);
	/* extract each element of pGlobal->slParams and escape */
	(void)sip_formEscapedSipParamList(out, &(pGlobal->slParams), (SIP_S8bit *) ";",1, pErr);
	return SipSuccess;

}

/*****************************************************************
** FUNCTION: sip_formTelLocalNum
** DESCRIPTION: Converts a list of TelLocalNum to text
** Parameters:
** 		pLocal(IN)    - The TelLocalNum to be used
**		out(OUT)       - The output buffer
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formTelLocalNum(TelLocalNum *pLocal,SIP_S8bit* out,SipError *pErr)
{
	(void)STR_CAT(out,pLocal->pLocalPhoneDigit);
	if(pLocal->pIsdnSubAddr != SIP_NULL)
		(void)STR_CAT(out,(SIP_S8bit*)";isub=");
	(void)STR_CAT(out,pLocal->pIsdnSubAddr);
	/* escape here for post dial */
	if(pLocal->pPostDial !=SIP_NULL)
	{
		(void)STR_CAT(out,(SIP_S8bit*)";postd=");
		(void)STR_CAT(out,pLocal->pPostDial);

	}
	/* extract each element of pGlobal->slAreaSpecifier and escape */
	(void)sip_formSipStringList(out, &(pLocal->slAreaSpecifier), (SIP_S8bit *) \
		";",1, pErr);
	/* extract each element of pGlobal->slParams and escape */
	(void)sip_formEscapedSipParamList(out, &(pLocal->slParams), (SIP_S8bit *) ";",1, pErr);
	return SipSuccess;
}

/*****************************************************************
** FUNCTION: sip_formEscapedSipParamList
** DESCRIPTION: Converts a list of Param to text with escaping certain
**                  charaters
** Parameters:
** 		out(OUT)    - output buffer
**		list(IN)       -  the param list to be converted
** 		seperator(IN)	- each element to be sepearated by
**		leadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
**
** DESCRIPTION: Converts a list of SipParams to text
*****************************************************************/

SipBool sip_formEscapedSipParamList
#ifdef ANSI_PROTO
	(SIP_S8bit 	*out,
	SipList 	*list,
	SIP_S8bit 	*separator,
	SIP_U8bit	leadingsep,
	SipError 	*err)
#else
	(out, list, separator, leadingsep, err)
	SIP_S8bit *out;
	SipList *list;
	SIP_S8bit *separator;
	SIP_U8bit leadingsep;
	SipError *err;
#endif
{
		SIP_U32bit listSize,listIter;
		(void)sip_listSizeOf( list, &listSize, err);
		listIter = 0;
		while (listIter < listSize)
		{
			SipParam *pParam;
			SIP_U32bit valueSize;

			(void)sip_listGetAt (list, listIter, (SIP_Pvoid *) &pParam, err);
			if((listIter!=0)||(leadingsep!=0))
				(void)STR_CAT ( (SIP_S8bit *)out, separator);
			(void)STR_CAT(out,pParam->pName);
			(void)sip_listSizeOf( &(pParam->slValue), &valueSize, err);
			if ( valueSize>=1)
			{
				SIP_S8bit *value;
				SIP_U32bit valueIter=0;
				(void)STR_CAT((SIP_S8bit *) out,(SIP_S8bit*)"=");
				while(valueIter < valueSize)
				{
					if(valueIter>0)
						(void)STR_CAT((SIP_S8bit *) out,(SIP_S8bit*)",");
					(void)sip_listGetAt (&(pParam->slValue), valueIter, \
						(SIP_Pvoid *) &value, err);
					if(!((sip_strcasecmp(pParam->pName,(SIP_S8bit*)"tsp")==0)&&\
						(value[0]=='[')&&(value[sip_strlen(value)-1]==']')))
					{
						(void)STR_CAT(out,value);
					}
					else
					{
						(void)STR_CAT(out,value);
					}


					valueIter++;
				}
			}
			listIter++;
		} /* while */
		return SipSuccess;
}
/*****************************************************************
** FUNCTION: sip_formSipStringList
** DESCRIPTION: Converts a list of String to text
** Parameters:
** 		out(OUT)    - output buffer
**		list(IN)       -  the string list to be converted
** 		seperator(IN)	- each element to be sepearated by
**		leadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formSipStringList
#ifdef ANSI_PROTO
	(SIP_S8bit 	*out,
	SipList 	*list,
	SIP_S8bit 	*separator,
	SIP_U8bit	leadingsep,
	SipError 	*pErr)
#else
	(out, list, separator, leadingsep, pErr)
	SIP_S8bit *out;
	SipList *list;
	SIP_S8bit *separator;
	SIP_U8bit leadingsep;
	SipError *pErr;
#endif
{
		SIP_U32bit listSize,listIter;
		(void)sip_listSizeOf( list, &listSize, pErr);
		listIter = 0;
		while (listIter < listSize)
		{
			SIP_S8bit *pString;
			(void)sip_listGetAt (list, listIter, (SIP_Pvoid *) &pString, pErr);
			if((listIter!=0)||(leadingsep!=0))
				(void)STR_CAT ((SIP_S8bit *)out, separator);
			(void)STR_CAT((SIP_S8bit *) out,(SIP_S8bit*)"phone-context=");
			(void)STR_CAT(out,pString);
			listIter++;
		} /* while */
		return SipSuccess;
}



/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

