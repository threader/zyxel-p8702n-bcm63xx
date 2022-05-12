/***********************************************************************
** FUNCTION:
**			Header  sub elements parsing functionality
************************************************************************
**
** FILENAME:
** microsip_subparsers.c
**
** DESCRIPTION:
**		This file contains functions for parsing SIP header elements.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/02/2002	Mahesh Govind		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_subparsers.h"
#include "microsip_msgbodyparser.h"


/***********************************************************************
** FUNCTION: sip_tokenize
** DESCRIPTION: This function tokenizes a given string 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				dLimiter(IN)	- The dLimiter 
**
** NOTE: 
************************************************************************/

SIP_S8bit*  sip_tokenize( SIP_S8bit *pStart, SIP_S8bit *pEnd,  \
			SIP_S8bit dLimiter)
{
	SIP_U32bit quoteFound=0;
	while (pStart  <=  pEnd) 
	{
		if ((*pStart == '"' )&& (quoteFound== 0))
		{
			pStart++;
			quoteFound++;
			continue;
			
		}
		if ((*pStart == '"' )&& (quoteFound != 0))
		{
			pStart++;
			quoteFound--;
			continue;
			
		}

		if ((*pStart == dLimiter) && (quoteFound == 0))
			return pStart;
		pStart++;
	}
	return SIP_NULL;	
} 

/***********************************************************************
** FUNCTION: sip_tokenize
** DESCRIPTION: This function tokenizes a given string 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				dLimiter(IN)	- The dLimiter 
**
** NOTE: 
************************************************************************/

SIP_S8bit*  sip_tokenize_sdp_attr( SIP_S8bit *pStart, SIP_S8bit *pEnd,  \
			SIP_S8bit dLimiter)
{
	while (pStart  <=  pEnd) 
	{
		if (*pStart == dLimiter)
			return pStart;
		pStart++;
	}
	return SIP_NULL;	
} 

/***********************************************************************
** FUNCTION: sip_nStrdup
** DESCRIPTION: This function does strdup for n characters 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				n(IN)			- Number of characters
** NOTE: 
************************************************************************/

SIP_S8bit * sip_nStrdup( SIP_U32bit id ,SIP_S8bit *pStart, SIP_S32bit n, \
		SipError *pErr)
{
	SIP_S8bit *pRetVal = SIP_NULL;

	if (0 == n) 
	{
		*pErr = E_PARSER_ERROR;
		return SIP_NULL;
	}
    pRetVal = (SIP_S8bit *) fast_memget(id,  n+1,  pErr);
    if (SIP_NULL == pRetVal)
	{
		sip_error(SIP_Minor,"sip_nStrDup Failed\n");
		*pErr = E_NO_MEM;
        return SIP_NULL;
	}		
#ifdef SIP_LINT
    if(pStart != SIP_NULL)
#endif
    memcpy(&pRetVal[0],  pStart, n);
    pRetVal[n] = '\0';
	*pErr = E_NO_ERROR;
    return pRetVal;
} 

/****************************************************************************
** FUNCTION: sip_validateIPv6Addr
** DESCRIPTION: This function Checks whether the given address is
**				a valid IPV6 address or not    
** 
** PARAMETERS:
**				ppStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pErr(OUT		- Error value if any 
**
** NOTE: Currently this function does only "valid-character" checking.
****************************************************************************/
SipBool sip_validateIPv6Addr(SIP_S8bit *pStart, SIP_S8bit *pEnd, SipError *pErr)
{

	/* We need to do  only the range checking here
	 *	Examples of ipV6address are given below.
	 *
	 *	1)ae12
	 *	2)ae12:bc21
	 *	3)ae12:dc34::3344:89aa
	 *	4)::aa22
	 *	5)bb33::
	 *	6)ae12:dc34::3344:89aa:139.85.229.21
	 *
	 */
	pStart++;
	while (pStart <pEnd)
	{
		if ( (*pStart >=  '0' && *pStart <='9' )  ||\
				((*pStart >='a' && *pStart <= 'f')||\
				(*pStart >='A' && *pStart <= 'F')) ||\
				(*pStart == ':') || (*pStart == '.') )
			pStart++;
		else 
		{
			*pErr = E_PARSER_ERROR;
			sip_error(SIP_Minor,"Validate IPV6 Failed\n");
			return SipFail ;
		}
	}
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_parseEachParam
** DESCRIPTION: This function parses and fills a SipParam structure   
** 
** PARAMETERS:
**				ppStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				Flag			- Flag telling whether name=value is a must
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** RETURN VALUE: Pointer to a SipParam structure
****************************************************************************/
SipParam * sip_parseEachParam(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
								en_SipBoolean valueReqd, SipError *err)

{
	SipParam 	*tmpSipParam	=SIP_NULL;
	SIP_S8bit 	*tmp			=SIP_NULL;
	SIP_S8bit 	*pPtr			=SIP_NULL;

	/* Strip leading and trailing LWS */
	pStart = sip_stripLeadingLWS(pStart, pEnd);
	pEnd   = sip_stripTrailingLWS(pStart, pEnd);

	if	(SipFail == sip_initSipParam ( &tmpSipParam, err ) )
	{
		sip_error(SIP_Minor,"Error in initing SipParam\n");
		return SIP_NULL;
	}	

	/* Look for param name */
	tmp = sip_tokenize (pStart,  pEnd,  '='  );
	if (tmp!=SIP_NULL)
	{
		pPtr = sip_stripLeadingLWS(tmp+1,pEnd);
		if ((*pPtr == '\n') ||(*pPtr == '\r')||(pPtr>pEnd))
		  goto error;
	}
	
	if (tmp == SIP_NULL)
	{
		/*
		 *params of type abc i.e. name only
		 * In this case check whether such params are allowed.
		 *if allowed then store the name 
		 */
		
		if (valueReqd == SipFalse)
		{
			/*If name alone is allowed store  name
			 */
			tmpSipParam->pName = sip_nStrdup(DECODE_MEM_ID, pStart, \
						(pEnd-(pStart)+1), err);	
#ifndef  SIP_NO_CHECK						
			if (SIP_NULL == tmpSipParam->pName)
			{
				sip_freeSipParam(tmpSipParam);
				return SIP_NULL;	
			}
#endif			
			pStart = pEnd;
		}	
		else
		{
			/*
			if name alone is not allowed return error.
			*/
			sip_freeSipParam(tmpSipParam);
			sip_error(SIP_Minor,\
			"Error in parsing SipParam , name alone is not allowed\n");
			return SIP_NULL;
		}	
		
	}		
	else
	/*Params of type  abc=def
	If got an equal sign 	store name and value
	*/
	{
		SIP_S8bit 	*valueString = SIP_NULL ;
		SIP_S8bit 	*valueBegin = SIP_NULL ;
		SIP_S8bit 	*valueEnd = SIP_NULL ;
		SIP_S8bit 	*nameEnd = SIP_NULL ;
		nameEnd = sip_stripTrailingLWS(pStart,tmp-1);
		tmpSipParam->pName 		 = sip_nStrdup(DECODE_MEM_ID, pStart,\
								nameEnd-pStart+1,err);	
								
#ifndef  SIP_NO_CHECK						
		if (tmpSipParam->pName == SIP_NULL)
		{
			sip_freeSipParam(tmpSipParam);
			return SIP_NULL;
		}	
#endif		
		valueBegin = sip_stripLeadingLWS(tmp+1, pEnd);
		valueEnd   = sip_stripTrailingLWS(tmp+1,pEnd);
		valueString	 = sip_nStrdup(DECODE_MEM_ID, valueBegin,\
								valueEnd - valueBegin+1, err);
								
#ifndef  SIP_NO_CHECK						
		if (valueString == SIP_NULL)
		{
			sip_freeSipParam(tmpSipParam);
			return SIP_NULL;
		}	
		if ((SIP_NULL == tmpSipParam->pName) || (SIP_NULL == valueString))
		{
			if (SIP_NULL!= valueString)
				(void)fast_memfree(DECODE_MEM_ID, valueString, err);
			sip_freeSipParam(tmpSipParam);
			return SIP_NULL;	
		
		}
#endif	
		if (sip_listAppend( &(tmpSipParam->slValue), \
			(SIP_Pvoid)valueString, err)!=SipSuccess)
		{
			goto error;
		}
	}
	return tmpSipParam;

error:
			
			sip_error(SIP_Major,"Parsing of parseEachParam  Failed\n");
			sip_freeSipParam(tmpSipParam);
			return SIP_NULL ;

}



/****************************************************************************
** FUNCTION: sip_parseSipParam
** DESCRIPTION: This function parses a SipParam and Returns a list of slParam  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				slParam(OUT)	- The sipList which is filled and returned
**				Flag			- Flag telling whether name=value is a must
**								  headers filled. 
**				dLimiter		- The dLimiter by which params are separated
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseSipParam (SipList	*slParam,  SIP_S8bit	*pStart,  SIP_S8bit	*pEnd,   en_SipBoolean	valueReqd, SIP_S8bit 	dLimiter,  SipError	*err)
{
	SipList	 	pTmpList;
	SipParam 	*pTmpParam =SIP_NULL;
	SIP_S8bit	*pTmp 	= SIP_NULL;
	SIP_S8bit	*pPtr	=	SIP_NULL;  
	
	if (*pEnd == dLimiter)
		return SipFail;
	if (SipFail == sip_listInit(&pTmpList, __sip_freeSipParam, err) )
		return SipFail;

	while (pStart <= pEnd) /*Continue until the start < End*/
	{
/*		while ((*pStart == ' ')|| (*pStart == '\t'))
			pStart++;remove the leading white spaces*/
		pStart =sip_stripLeadingLWS(pStart , pEnd);
		if (pStart > pEnd)
		{
			*err = E_PARSER_ERROR;
			sip_error(SIP_Minor, "Error in parsing Sip params");
			goto error;
		}

		/* First extract each parameter by looking for the delimiter
		 * and then parse each param individually.
		 */

		pTmp = sip_tokenize (pStart,  pEnd,  dLimiter );
		if (pTmp!=SIP_NULL)
		{
			pPtr = sip_stripTrailingLWS(pTmp+1,pEnd);
			if (*pPtr == dLimiter)
				goto error;
		}	
		
		if (pTmp == SIP_NULL)	
			pTmp = pEnd+1;

		pTmpParam = sip_parseEachParam(pStart, pTmp-1, valueReqd, err);
		if (pTmpParam != SIP_NULL)
		{
			if (sip_listAppend( &pTmpList, (SIP_Pvoid)pTmpParam, err)!=\
						SipSuccess)
				goto error;
		}	
		else
		{
			goto error;
		}
		pStart = pTmp+1;
	}/* END of While Loop */	
	*slParam = pTmpList;	
	return SipSuccess;
	
error:
				sip_freeSipParam (pTmpParam);
				(void)sip_listDeleteAll ( &pTmpList, err);
				sip_error(SIP_Major,"Parsing of SipParam  Failed\n");
				return SipFail;
}

/****************************************************************************
** FUNCTION: sip_parseHostPort
** DESCRIPTION: This function parses the Host and Port fields
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppHost(OUT)		- The parsed Host address
**				ppPort(OUT)		- The parsed port no
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseHostPort(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
		SIP_S8bit **ppHost, SIP_U16bit **ppPort, SipError *pErr)
{
	SIP_S8bit *Host = SIP_NULL;
	SIP_U16bit *pPort = SIP_NULL;
	SIP_U32bit	tmpPort = 0;
	SIP_S8bit *pCurrentPos = SIP_NULL;
	SIP_S8bit *tmp = SIP_NULL;
	
	pPort = (SIP_U16bit *) fast_memget(DECODE_MEM_ID, sizeof(SIP_U32bit), pErr);
	if (SIP_NULL == pPort)
		return SipFail;

	/*first Check whether it is an IPV6 address
	 * 
	 */
	pStart = sip_stripLeadingLWS(pStart, pEnd);
	pEnd  = sip_stripTrailingLWS(pStart, pEnd);
	if (*pStart == '[')
	{
		pCurrentPos = sip_tokenize(pStart, pEnd,  ']')	;
		if (pCurrentPos == SIP_NULL)
		{
			/*If Current Position is NULL tis means an invalid IPV6Address
			 *is present
			 */
			sip_error(SIP_Minor,"Error in Parsing IPV6\n");
			goto error;
		}/*End Of CHECKING for IPV6 Address*/
		else
		{
			/*Call validate IPV6 address
			 */

			if (SipFail == sip_validateIPv6Addr(pStart, pCurrentPos, pErr))
			{
				sip_error(SIP_Minor,"Error in Validating IPV6\n");
				goto error;
			}/*End of Validation*/
			else
			{
				/*If it is a valid IPV6 Now Store IPV6Address and Port
				 */
				tmp = sip_tokenize(pCurrentPos+1, pEnd, ':');
				if (tmp == SIP_NULL)
				{
					/*This means only Host part is present
				 	*/
					Host = sip_nStrdup(DECODE_MEM_ID, pStart, pEnd-pStart+1, pErr);
					(void)fast_memfree(DECODE_MEM_ID,pPort,pErr);
					pPort = SIP_NULL;
					if (SIP_NULL == Host)
					{
						return SipFail;
					}
				}	
				else
				{
					/* Both host and port present. */

					SIP_S8bit* pHostEnd = SIP_NULL;
					SIP_S8bit* pPortBegin = SIP_NULL;

					pHostEnd = sip_stripTrailingLWS(pStart, tmp-1);
					Host = sip_nStrdup(DECODE_MEM_ID, pStart,\
						   	pHostEnd-pStart+1, pErr);
					if (SIP_NULL == Host)
					{
						return SipFail;
					}
					pPortBegin = sip_stripLeadingLWS(tmp+1,pEnd);
					if ((SipFail == sip_atoi(pPortBegin, pEnd, &tmpPort)) ||\
						(tmpPort > 65535))
						goto error;
					*pPort = (SIP_U16bit)tmpPort;
				}	
				
			}/*END of Storing IPV6 address and PORT*/	
		}			

	}/*End of CHECKING for IPV6 address		*/
	else
	{
		/*Try to extract Host and Port if it is a normal address.
		*/
		tmp = sip_tokenize(pStart, pEnd, ':');
		if (tmp == SIP_NULL)
		{
			/*This means only Host part is present
			 */
			(void)fast_memfree(DECODE_MEM_ID,pPort,pErr);
			pPort = SIP_NULL;
			Host = sip_nStrdup(DECODE_MEM_ID, pStart, pEnd-pStart+1, pErr);
			if(Host == SIP_NULL)
			{
				sip_error(SIP_Minor,"Host absent\n");
				goto error;
			}	
		}	
		else
		{
			SIP_S8bit* pHostEnd = SIP_NULL;
			SIP_S8bit* pPortBegin = SIP_NULL;

			pHostEnd = sip_stripTrailingLWS(pStart, tmp-1);
			Host = sip_nStrdup(DECODE_MEM_ID, pStart, pHostEnd-pStart+1, pErr);
			if (SIP_NULL ==Host)
				goto error;
			
			pPortBegin = sip_stripLeadingLWS(tmp+1,pEnd);
			if ((SipFail == sip_atoi(pPortBegin, pEnd, &tmpPort)) ||\
				(tmpPort > 65535))
				goto error;	
			*pPort = (SIP_U16bit)tmpPort;
		}	
		
	}		

	*ppHost = Host;
	*ppPort = pPort;
	return SipSuccess;
				
error:
		(void)fast_memfree(DECODE_MEM_ID,pPort,pErr);
		(void)fast_memfree(DECODE_MEM_ID,Host,pErr);
		sip_error(SIP_Major ,"Host Port Parsing Failed\n");
		return SipFail;
		
}




/****************************************************************************
** FUNCTION: sip_parseUserAndHostPort
** DESCRIPTION: This function parses the username, password, host and port 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**				ppHost(OUT)		- The Host address
**				ppPort(OUT)		- The port no
**				ppUser(OUT)		- The user name
**				ppPassword(OUT)	- The password	
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseUserAndHostPort(SIP_S8bit *pStart,  \
	SIP_S8bit *pEnd, SIP_S8bit **ppUser, SIP_S8bit **ppPassword,  \
	SIP_S8bit **ppHost, SIP_U16bit **ppPort, SipError * pErr)

{
	SIP_S8bit *pCurrentPos	 = SIP_NULL;
	SIP_S8bit *tmp			 = SIP_NULL;
	SIP_S8bit *User			 = SIP_NULL;
	SIP_S8bit *Password		 = SIP_NULL;
	SIP_S8bit *Host			 = SIP_NULL;
	SIP_U16bit *Port		 = SIP_NULL;
	SIP_S8bit *pPasswordEnd	 = SIP_NULL;

	/*Remove LWS*/
	pStart = sip_stripLeadingLWS(pStart, pEnd);	
	pEnd  = sip_stripTrailingLWS(pStart,pEnd);
	if (*pStart == '@')		
	{
		/*username:password@host:port cannot begin	with @*/
		*pErr = E_PARSER_ERROR ;
		return SipFail;
		
	}

	/*Now tokenize for @ to get user and Password*/
	 
	pCurrentPos = sip_tokenize(pStart, pEnd,  '@')	;
	if(pCurrentPos != SIP_NULL)
		pPasswordEnd = sip_stripTrailingLWS(pStart, pCurrentPos-1);
	if (pCurrentPos != SIP_NULL)
	{
		/*This means user:password@hss.hns.com:5060 is  there*/
		tmp = sip_tokenize(pStart, pCurrentPos,  ':');
		if (tmp != SIP_NULL)
		{
			SIP_S8bit *pUserEnd = SIP_NULL;	
			SIP_S8bit *pPasswordBegin = SIP_NULL;	
			/*This means user:password is  there
			 *Now tokenize to get password and User name
		 	 */
			pUserEnd = sip_stripTrailingLWS(pStart, tmp-1);
			User = sip_nStrdup(DECODE_MEM_ID, pStart, pUserEnd - pStart+1,pErr);
			pPasswordBegin = sip_stripLeadingLWS(tmp+1,pPasswordEnd);
			Password = sip_nStrdup(DECODE_MEM_ID, pPasswordBegin, \
					pPasswordEnd-pPasswordBegin +1, pErr);
			if ((SIP_NULL == User) || (SIP_NULL == Password))
			{
					return SipFail;
			}

			if (SipFail == sip_parseHostPort(pCurrentPos+1, pEnd, \
				&Host, &Port, pErr))
			{
				goto error;
			}
		}/*End of USER and PAssword both are present*/
		else
		{
			SIP_S8bit *pUserEnd = SIP_NULL;	
			/*if Colon is absent 
			* Password is not there
			*/
			pUserEnd = sip_stripTrailingLWS(pStart, pCurrentPos-1);
			User= sip_nStrdup(DECODE_MEM_ID, pStart, pUserEnd-pStart+1, pErr);
			/* if for Storing  User and  password*/				
			if (SIP_NULL == User)
			{
				return SipFail;
			}
			
			if (SipFail == sip_parseHostPort(pCurrentPos+1, pEnd, \
							&Host, &Port, pErr))
				goto error;
		}/*End of ONLY USER present*/
	}	/*End of Message of Form sip:user:password@host:port*/
	else
	{   
		/*This Case User:password is not there
		* This is of the form sip:hss.hns.com:5060
		* So get the host and port
		*/
	
		if (SipFail == sip_parseHostPort(pStart, pEnd, &Host, &Port, pErr))
		{
			*pErr = E_PARSER_ERROR;
			return SipFail ;
		}
		
	}/*End of USER:Password  not present */
	*ppUser = User;
	*ppPassword = Password;
	*ppHost = Host;
	*ppPort = Port;
	return SipSuccess;

error:
			*pErr = E_PARSER_ERROR;
			(void)fast_memfree(DECODE_MEM_ID, User,pErr);
			(void)fast_memfree(DECODE_MEM_ID, Password ,pErr);
			sip_error(SIP_Major,"Error in parsing UserHost and Port\n");
			return SipFail ;
}

/****************************************************************************
** FUNCTION: sip_parseSipUrl
** DESCRIPTION: This function parses the SipURL 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppOutUrl(OUT)	- The SipUrl structure with relevan values
**								  filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseSipUrl(SipUrl **ppOutUrl, SIP_S8bit *pStart,  \
	SIP_S8bit *pEnd,  SipError *pErr)

{
	SipUrl 			*tmpUrl;
	SIP_S8bit		*tmp;
	SIP_S8bit		*pCurrentPos = SIP_NULL;
	en_SipBoolean 	Flag = SipFalse;
	
	pStart = sip_stripLeadingLWS(pStart, pEnd);
	pEnd   = sip_stripTrailingLWS(pStart, pEnd);
	if (SipFail == sip_initSipUrl (&tmpUrl, pErr))
		return SipFail ;
		
	/*
	 * First Check for  headers 
	 */
		
	tmp = sip_tokenize(pStart, pEnd, '?');
	
	if (tmp == SIP_NULL)
	{
		/*	This Case Headers are absent
		 *This means headers are absent
		 */
		tmpUrl->pHeader = SIP_NULL ;
		
		/*Now look for parameters and fill the host and port part
		 */
		pCurrentPos = sip_tokenize(pStart, pEnd, ';');
		if (pCurrentPos == SIP_NULL)
		{

			/*Call Host port Parsercwith pStart and pEnd
			 */
			if (SipFail == sip_parseUserAndHostPort(pStart,  \
				pEnd, &(tmpUrl->pUser), &(tmpUrl->pPassword),  \
				&(tmpUrl->pHost), (SIP_U16bit**)&(tmpUrl->dPort), pErr))
				goto error;
		}
		else
		{

			/*Call Host Port Parser with pStart and pCurrentPos
			 *Call sipparam parser with pCurrentPos(START) and pEnd(END)
			 */
			 
			 if (SipFail == sip_parseUserAndHostPort(pStart, \
				pCurrentPos-1,&(tmpUrl->pUser), &(tmpUrl->pPassword),  \
				&(tmpUrl->pHost), (SIP_U16bit **)&(tmpUrl->dPort),\
				pErr))
				goto error;

			if (SipFail == sip_parseSipParam(&(tmpUrl->slParam),\
				pCurrentPos+1, pEnd,  Flag, ';',  pErr))
				goto  error;
		}	
	}
	else
	{
	/*This Case Headers are present
	 *if headers are present fill headers
	 */
		SIP_S8bit	*pHdrStart;

		pHdrStart = sip_stripLeadingLWS(tmp+1, pEnd);
		tmpUrl->pHeader = sip_nStrdup(DECODE_MEM_ID, pHdrStart, pEnd - \
				pHdrStart + 1, pErr);
		if (SIP_NULL == tmpUrl->pHeader)
		{
		
			return SipFail;
		}
		/*Now look for parameters and fill the host and port part
		*/
		pCurrentPos = sip_tokenize(pStart, tmp-1, ';');
		if (pCurrentPos == SIP_NULL)
		{
			/*Call Host port Parser with pStart and pEnd
			 */
			if (SipFail == sip_parseUserAndHostPort(pStart,  \
			   tmp-1, &(tmpUrl->pUser), &(tmpUrl->pPassword),  \
			   &(tmpUrl->pHost), (SIP_U16bit **)&(tmpUrl->dPort), pErr))
			    goto error;
		}
		else
		{
			/*Call Host Port Parser with pStart and pCurrentPos
			 *Call sipparam parser with pCurrentPos(START) and pEnd(END)
			 */

			if (SipFail == sip_parseUserAndHostPort(pStart,\
				pCurrentPos-1,&(tmpUrl->pUser), &(tmpUrl->pPassword),\
				&(tmpUrl->pHost), (SIP_U16bit **)&(tmpUrl->dPort),pErr))
				goto error;
			/*
			 * parse the Url parameters
			 */
			if (SipFail == sip_parseSipParam(&(tmpUrl->slParam),\
				pCurrentPos+1, tmp-1,  Flag, ';',  pErr))
				goto error;
		}	
	}
	*ppOutUrl= tmpUrl;
	return SipSuccess;

error:
	
				sip_freeSipUrl(tmpUrl);
				*pErr = E_PARSER_ERROR;
				sip_error(SIP_Major,"Error in parsing SipURL\n");
				return SipFail;
}


/****************************************************************************
** FUNCTION: sip_parseAddrSpec
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppOutAddr(OUT)	- The SipAddrSpec structure with the relevant
**								  headers filled. 
**				pErr(OUT		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_parseAddrSpec(SIP_S8bit *pStart, \
		SIP_S8bit * pEnd, SipAddrSpec **ppOutAddr, SipError *pErr)
{
	SipAddrSpec *tmpAddr;

	if (SipFail == sip_initSipAddrSpec (&tmpAddr, SipAddrAny, pErr ))
		return SipFail;

	pStart = sip_stripLeadingLWS(pStart, pEnd);
	pEnd = sip_stripTrailingLWS(pStart, pEnd);
	
	/*
	 * Checking to find the input is a SipUrl or not
	 */
	if (sip_strncasecmp(pStart, (SIP_S8bit*)"sip:",4) == 0)
	{

		pStart = pStart+ 4;
		if (pStart > pEnd) 
			goto error;
			
		/*
		 * Now Call SipURL Parser
		 */
		if (SipFail == sip_parseSipUrl(&(tmpAddr->u.pSipUrl), pStart,\
						pEnd, pErr))
			goto error;
		else
		{
			tmpAddr->dType = SipAddrSipUri ;
		}
	}
	else if (sip_strncasecmp(pStart, (SIP_S8bit*)"sips:",5) == 0)
	{

		pStart = pStart+ 5;
		if (pStart > pEnd) 
			goto error;
			
		/*
		 * Now Call SipURL Parser
		 */
		if (SipFail == sip_parseSipUrl(&(tmpAddr->u.pSipUrl), pStart,\
						pEnd, pErr))
			goto error;
		else
		{
			tmpAddr->dType = SipAddrSipSUri ;
		}
	}
	else
	{
	/*
	 * Comming here means input is a uri and storethe uri
	 */
		tmpAddr->dType = SipAddrReqUri;
		tmpAddr->u.pUri = sip_nStrdup(DECODE_MEM_ID, pStart, pEnd-pStart+1, \
				pErr);
#ifndef SIP_NO_CHECK	
		if (SIP_NULL == tmpAddr->u.pUri)
		{
			sip_freeSipAddrSpec(tmpAddr);
			*pErr = E_NO_MEM;
			return SipFail;	
		}
#endif		
	}	

	*ppOutAddr = tmpAddr;
	return SipSuccess;

error:

			sip_error(SIP_Minor, "Incomplete addr-spec found");
			sip_freeSipAddrSpec(tmpAddr);
			*pErr = E_PARSER_ERROR ;
			return SipFail;
}		

		
/****************************************************************************
** FUNCTION: sip_stripLeadingLWS
** DESCRIPTION: This function removes leading White spaces  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/

SIP_S8bit *sip_stripLeadingLWS(SIP_S8bit *pStart,  SIP_S8bit *pEnd)
{

    while ((isspace((unsigned)(int)*pStart)) && (pStart <= pEnd)) 
        pStart++;

    return pStart;
}

/****************************************************************************
** FUNCTION: stripTrailingLWS
** DESCRIPTION: This function Removes Trailing White spaces  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/

SIP_S8bit *sip_stripTrailingLWS(SIP_S8bit *pStart,  SIP_S8bit *pEnd)
{

    while ((isspace((unsigned)(int)*pEnd)) && (pEnd >= pStart))
        pEnd--;

    return pEnd;
}

/****************************************************************************
** FUNCTION: sip_atoi
** DESCRIPTION: This function parses the Addrspec  
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				
** NOTE: 
****************************************************************************/
SipBool	sip_atoi(SIP_S8bit *pStart,  SIP_S8bit *pEnd,  SIP_U32bit	*pNum)
{
	SIP_S8bit	*pTemp = pStart;
	SipError	error;
	SIP_S8bit	*pStr;

	*pNum = 0;
	if (pStart > pEnd) 
		return SipFail;

	while ((pTemp <= pEnd) && (isdigit((unsigned)(int)*pTemp))) 	
		pTemp++;
	
	if (pTemp <= pEnd)
	{
		/* String is not a numeric string */
		sip_error(SIP_Minor,"Error in sip_atoi:not a numeric string\n");
		return SipFail;
	}

	/*
	 * Calculate value of the numeric string 
	 */
	/* begin SPRabc */
	pStr = sip_nStrdup(DECODE_MEM_ID ,pStart, pEnd - pStart + 1, &error);
	if (SIP_NULL == pStr)
	{
		
		sip_error(SIP_Minor,"Failed in Str_nDup\n");
		return SipFail;
	}
	*pNum = STRTOU32CAP(pStr, &error);
/*
	if (E_INV_RANGE == error)
	{
		sip_error(SIP_Minor, "Range overshoot\n");
		fast_memfree(DECODE_MEM_ID, pStr, &error);
		return SipFail;
	}
*/
	(void)fast_memfree(DECODE_MEM_ID, pStr, &error);
	/* end SPRabc */

	return SipSuccess;

}


#ifdef SIP_DATE
/****************************************************************************
** FUNCTION: sip_parseSipDate
** DESCRIPTION: This function parses a SIP-Date string as given in the RFC
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppDateStruct(OUT)- The output parsed date structure.
**				pErr(OUT)		- Error if any.
**				
** NOTE: 
****************************************************************************/
SipBool	sip_parseSipDate(SIP_S8bit *pStart,  SIP_S8bit *pEnd,  \
		SipDateStruct **ppDateStruct, SipError *pErr)
{
	SIP_S8bit 		*pCurrentPos = pStart;
	SIP_S8bit 		*pToken;
	en_SipBoolean	isErrorCase = SipFalse;
	SIP_U32bit		dTemp, loopIndex;

	SIPDEBUGFN("Entering into sip_parseSipDate");

	/*
	 * Allocate and initialise the SipDateStruct
	 */

	if (SipFail == sip_initSipDateStruct(ppDateStruct, pErr))
		return SipFail;

	/* 
	 * Skip leading and trailing LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	if ((pCurrentPos + 28) > pEnd)
	{
		/*
		 * All Date fields not present (SIP-date must be 29 chars long).
		 */
		goto error;
	}
		
	/*
	 * Look for the week-day field
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ',');
	if (SIP_NULL == pToken)
	{
		/*
		 * COMMA is mandatory; raise parser error
		 */
		goto error;
	}

	(*ppDateStruct)->dDow = SipDayNone; 
	switch (*pCurrentPos)
	{
		case 'M':
				if (0 == sip_strncmp((SIP_S8bit*)"Mon", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDayMon;
				break;
		case 'T':
				if (0 == sip_strncmp((SIP_S8bit*)"Tue", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDayTue;
				else if (0 == sip_strncmp((SIP_S8bit*)"Thu", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDayThu;
				break;
		case 'W':
				if (0 == sip_strncmp((SIP_S8bit*)"Wed", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDayWed;
				break;
		case 'F':
				if (0 == sip_strncmp((SIP_S8bit*)"Fri", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDayFri;
				break;
		case 'S':
				if (0 == sip_strncmp((SIP_S8bit*)"Sat", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDaySat;
				else if (0 == sip_strncmp((SIP_S8bit*)"Sun", pCurrentPos, 3))
					(*ppDateStruct)->dDow = SipDaySun;
				break;
		default:
				break;
	} /* end of switch */

	if ((SipDayNone == (*ppDateStruct)->dDow) || ((pToken - pCurrentPos) != 3) ||\
		(*(pToken + 1) != ' '))
		goto error;

	/*
	 * Now parse date1 field; first init the pDate field.
	 */

	if (SipFail == sip_initSipDateFormat(&((*ppDateStruct)->pDate), pErr))
		goto error;

	/*
	 * Validate the day field.
	 */

	pCurrentPos = pToken + 2; /* advance beyond SPACE */
	if ((SipFail == sip_atoi(pCurrentPos, pCurrentPos + 1, \
		&dTemp)) || (dTemp > 255) || (*(pCurrentPos + 2) != ' '))
		goto error;
	(*ppDateStruct)->pDate->dDay = (SIP_S8bit) dTemp;

	/*
	 * Validate and store the month field.
	 */

	pCurrentPos += 3; /* advance beyond SPACE */
	switch (*pCurrentPos)
	{
		case 'J':
				if (0 == sip_strncmp((SIP_S8bit*)"Jan", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthJan;
				else if (0 == sip_strncmp((SIP_S8bit*)"Jun", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthJun;
				else if (0 == sip_strncmp((SIP_S8bit*)"Jul", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthJul;
				else
					isErrorCase = SipTrue;
				break;
		case 'F':
				if (0 == sip_strncmp((SIP_S8bit*)"Feb", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthFeb;
				else
					isErrorCase = SipTrue;
				break;
		case 'M':
				if (0 == sip_strncmp((SIP_S8bit*)"Mar", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthMar;
				else if (0 == sip_strncmp((SIP_S8bit*)"May", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthMay;
				else
					isErrorCase = SipTrue;
				break;
		case 'A':
				if (0 == sip_strncmp((SIP_S8bit*)"Apr", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthApr;
				else if (0 == sip_strncmp((SIP_S8bit*)"Aug", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthAug;
				else
					isErrorCase = SipTrue;
				break;
		case 'S':
				if (0 == sip_strncmp((SIP_S8bit*)"Sep", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthSep;
				else
					isErrorCase = SipTrue;
				break;
		case 'O':
				if (0 == sip_strncmp((SIP_S8bit*)"Oct", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthOct;
				else
					isErrorCase = SipTrue;
				break;
		case 'N':
				if (0 == sip_strncmp((SIP_S8bit*)"Nov", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthNov;
				else
					isErrorCase = SipTrue;
				break;
		case 'D':
				if (0 == sip_strncmp((SIP_S8bit*)"Dec", pCurrentPos, 3))
					(*ppDateStruct)->pDate->dMonth = SipMonthDec;
				else
					isErrorCase = SipTrue;
				break;
		default:
				isErrorCase = SipTrue;
				break;
	} /* end of switch */

	if ((SipTrue == isErrorCase) || (*(pCurrentPos + 3) != ' ')) 
		goto error;

	/*
	 * Validate and store year field.
	 */

	pCurrentPos += 4;
	if ((SipFail == sip_atoi(pCurrentPos, pCurrentPos + 3, \
		&dTemp)) || (*(pCurrentPos + 4) != ' '))
		goto error;

	(*ppDateStruct)->pDate->dYear = (SIP_U16bit)dTemp;

	/*
	 * Now validate and store the pTime field.
	 */

	pCurrentPos+=5; /* advance beyond SPACE */

	/*
	 * Allocate memory for time field.
	 */

	if (SipFail == sip_initSipTimeFormat(&((*ppDateStruct)->pTime), pErr))
		goto error;

	for (loopIndex = 1; loopIndex < 4; loopIndex++)
	{
		if ((SipFail == sip_atoi(pCurrentPos, pCurrentPos + 1, \
			&dTemp)) || (dTemp > 255) || \
			(*(pCurrentPos + 2) != ((loopIndex==3)?' ' : ':')))
			goto error;

		if (1 == loopIndex)
			(*ppDateStruct)->pTime->dHour = (SIP_S8bit) dTemp;
		else if (2 == loopIndex)
			(*ppDateStruct)->pTime->dMin = (SIP_S8bit) dTemp;
		else if (3 == loopIndex)
			(*ppDateStruct)->pTime->dSec = (SIP_S8bit) dTemp;
		pCurrentPos+=3;
	}

	/*
	 * Look for the string "GMT" at the end.
	 */

	if (0 != sip_strncmp((SIP_S8bit*)"GMT", pCurrentPos, 3))
		goto error;

	SIPDEBUGFN("Exiting from sip_parseSipDate");
	return SipSuccess;

error:

		sip_freeSipDateStruct(*ppDateStruct);
		*pErr = E_PARSER_ERROR;
		sip_error(SIP_Major,"Parsing of SipDate  Failed\n");
		return SipFail;
}
#endif /* SIP_DATE */





