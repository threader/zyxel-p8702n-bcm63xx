/***********************************************************************
** FUNCTION:
**			This  file contains the parser for IM URL
************************************************************************
**
** FILENAME:
** microsip_imurl.c
**
** DESCRIPTION:
**		This file contains functions for parsing SIP header elements.
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/08/2002	Mahesh Govind		-			Initial					
**
**
**	COPYRIGHT , Aricent, 2006
***********************************************************************/

#if 0
		====================================================
		Syntax of IM url
		====================================================

		
		IM-URL         = "im:" [ to ] [ headers ]
        to             =  mailbox
        headers        =  "?" header *( "&" header )
        header         =  hname "=" hvalue
        hname          =  *urlc
        hvalue         =  *urlc

		mailbox     =  addr-spec                    ; simple address
				   /phrase route-addr            ; name & addr-spec
		route-addr  =  "<" [route] addr-spec ">"

		
		route       =  1#("@" domain) ":"           ; path-relative
		addr-spec   =  local-part "@" domain        ; global address
		local-part  =  word *("." word)             ; uninterpreted
		domain      =  sub-domain *("." sub-domain)
		sub-domain  =  domain-ref / domain-literal
		domain-literal =  "[" *(dtext / quoted-pair) "]"
		domain-ref  =  atom                         
		====================================================
		Structure of IM url
		====================================================
		typedef struct 
		{
			SIP_S8bit*		pDispName; 	/* phrase go here */
			SipList			slRoute;   	/* route go here */
			SIP_S8bit* 		pUser;		/* local-part go here */
			SIP_S8bit*		pHost;   	/* domain go here */
			SipList			slParams;   /* header params go here*/
			SIP_RefCount    dRefCount; 
		} ImUrl;
#endif
/***********************************************************************
** FUNCTION: sip_parseImUrl
** DESCRIPTION: This function parses IM URL 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pImurl(IN/OUT)	- The Im Url structure
**				pErr(IN)		- SipError
**			
** NOTE:Please note that this function will be called from related API 
************************************************************************/
#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_imurl.h"
#include "microsip_imurlfree.h"
#include "microsip_imurlinit.h"
#include "microsip_statistics.h"
#include "microsip_encodeinternal.h"
#include "microsip_hdrencoders.h"
#include "microsip_telparser.h"


SipBool sip_parseImUrl(SIP_S8bit *pStart ,ImUrl *pImurl ,\
	SipError *pErr)
{
	SIP_S8bit *pTmp			= SIP_NULL;
	SIP_S8bit *pEnd			= SIP_NULL;
	SIP_S8bit *pPtr			= SIP_NULL;
	SIP_S8bit *pCurrentPos	= SIP_NULL;
	SIP_S8bit *pCommaPos	= SIP_NULL;
	SIP_S8bit *pImAddrBegin	= SIP_NULL;
	SIP_S8bit *pImAddrEnd	= SIP_NULL;
	/*
	 * First remove unwanted spaces
	 */
	pEnd = pStart+ (sip_strlen(pStart) - 1);
	pStart = sip_stripLeadingLWS(pStart , pEnd);
	pEnd = sip_stripTrailingLWS(pStart , pEnd);
	
	/*First separate headers .
	 *If headers are present they will be after ?
	 *So first tokenize for ? and separate them.
	 * 
	 */
	 pCommaPos = sip_tokenize(pStart , pEnd , '?');
	 /*
	  * In imurl ? headers are stored as list of name=value ,
	  * not as in SipUrl.
	  */ 
	 if(pCommaPos !=SIP_NULL)
	 {
		/*
		 * Call the parameter parser to parse the  headers
		 * with dlimiter as &
		 */ 
		if (SipFail == sip_parseSipParam(&(pImurl->slParams) ,\
			pCommaPos+1 ,pEnd, (en_SipBoolean)1 , '&' ,pErr ))
		{
			*pErr = E_PARSER_ERROR;
			return SipFail;	
		}	
		/*
		 * Since we have 
		 * parsed until ? now the end pont is one char before ?
		 */ 
		pEnd = pCommaPos-1;
	 }
	  
		
	/*
	 * First search for escaped equivalent of "<" 
	 * if it is there then the grammar corresponds to
	 *		mailbox =phrase route-addr
	 *		mail box = "A.bell" <@hss.com,@hns.com:user@host>
	 * else
	 *		it is just an addrspec
	 *		mail box = user@host
	 */
	pImAddrBegin = pStart;
	pImAddrEnd = pEnd;
	if ((pCurrentPos = sip_strstr(pStart ,(SIP_S8bit*)"%3C")) == SIP_NULL )
	{
		if ((pCurrentPos = sip_strstr(pStart ,(SIP_S8bit*)"%3c")) == SIP_NULL )
			pCurrentPos = SIP_NULL;
	}
	if ((pCurrentPos == SIP_NULL))
	{
		/*
		 * only addrspec is there in the  im: url
		 * mail box = user@host
		 * store the user and host
		 */ 
		if (SipFail == sip_parseImAddr(pStart , pEnd ,&(pImurl->pUser) ,\
			&(pImurl->pHost) , pErr))
		{
			return SipFail;
		}	
		*pErr = E_PARSER_ERROR;
		return SipSuccess;
	}
	/*
	 * Now if you come here means you have found mailbox:phrase routeaddr
	 * LANGLE ([%][3][C|c])
	 * RANGLE ([%][3][E|e])
	 */
	pTmp = pCurrentPos-1;
	pImAddrBegin = pCurrentPos+3;
	
	if ((pCurrentPos = sip_strstr(pStart ,(SIP_S8bit*)"%3E")) == SIP_NULL )
	{
		if ((pCurrentPos = sip_strstr(pStart ,(SIP_S8bit*)"%3e")) == SIP_NULL )
			pCurrentPos = SIP_NULL;
	}
	if (pCurrentPos == SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}	
	pImAddrEnd = pCurrentPos-1;
	/*
	 * We need to store the display name/Phrase
	 */ 
	pImurl->pDispName = sip_nStrdup(DECODE_MEM_ID ,pStart ,pTmp-pStart+1 ,pErr);
	if (pImurl->pDispName ==SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;	
		return SipFail;
	}
	
	/*
	 * First tokenize for :
	 * if it is null this means [route] is absent
	 */
	pPtr = sip_tokenize(pImAddrBegin ,pImAddrEnd ,':');
	if (pPtr == SIP_NULL)
	{
		/*
		 * Store the adrspec
		 */
		if (SipFail == sip_parseImAddr(pImAddrBegin , pImAddrEnd ,\
			&(pImurl->pUser) ,&(pImurl->pHost) , pErr))
		{
			return SipFail;
		}	
	}
	else 
	{
	
		/*
		 * store the [route] 
		 *and 
		 *then store addrspec
		 */
		/*
		 * First store addrspec 
		 */
		if (SipFail == sip_parseImAddr(pPtr+1 , pImAddrEnd ,&(pImurl->pUser) \
			,&(pImurl->pHost) , pErr))
		{
			return SipFail;
		}	


		while (pImAddrBegin < pPtr)
		{
			/*
			 * route       =  1#("@" domain) ":"        
			 * Now  store route
			 * 1)tokenize for comma
			 * 2)remove @
			 * 3)append the remaining string to list of routeStrings
			 * 
			 */
			pCommaPos = sip_tokenize(pImAddrBegin ,pPtr ,',');
			/*
			 * if pCommaPos is Null this means there is only one 
			 * route string.
			 */ 
			if (pCommaPos == SIP_NULL)
			{	
			
				if (SipFail == sip_parseImRoutelist(pImAddrBegin ,pPtr-1 ,\
				&(pImurl->slRoute) ,pErr))
				{
					return SipFail;
				}
				return SipSuccess;
			}/*if pCommaPos ==SIP_NULL */
			/*
			 * If you have multiple route you have to handle it here
			 */
			if (SipFail == sip_parseImRoutelist(pImAddrBegin ,pCommaPos-1 ,\
			&(pImurl->slRoute) ,pErr))
			{
				return SipFail;
			}
			pImAddrBegin = pCommaPos+1;	
		
		}/*End of While loop*/
	}
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/***********************************************************************
** FUNCTION: sip_parseImRouteList
** DESCRIPTION: This function parses and stores a list of route in IM URL 
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

SipBool sip_parseImRoutelist(SIP_S8bit *pStart ,\
	SIP_S8bit *pEnd , SipList *pList ,SipError *pErr)
{
	SIP_S8bit *pCommaPos	= SIP_NULL;
	SIP_S8bit *pString		= SIP_NULL;
	SIP_S8bit *pPtr			= SIP_NULL;
	/*
	 * route       =  1#("@" domain) ":"        
	 * Now  store route
	 * 1)tokenize for comma
	 * 2)remove @
	 * 3)append the remaining string to list of routeStrings
	 * 
	 */
	pCommaPos = sip_tokenize(pStart ,pEnd ,'@');
	if (pCommaPos ==SIP_NULL)
	{
		/*
		 * if pCommaPos is null this means @ is absent which is
		 * mandartory by the grammar
		 */ 
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	pCommaPos++;/* move afterwards @*/
	pCommaPos = sip_stripLeadingLWS(pCommaPos ,pEnd);
	pPtr = sip_stripTrailingLWS(pCommaPos ,pEnd);
	/*
	 * Store the value to string list of
	 * pImurl->slRoute
	 */ 
	pString = sip_nStrdup(DECODE_MEM_ID , pCommaPos ,pPtr-pCommaPos+1 ,pErr);
	if (pString == SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
	/*
	 * Append to list of routes
	 */
	if (SipFail == sip_listAppend(pList, \
			(SIP_Pvoid)pString , pErr))
	{	
		(void)fast_memfree(DECODE_MEM_ID ,pString ,pErr);
		return SipFail;
	}		
	*pErr = E_NO_ERROR;			
	return SipSuccess;
}
/***********************************************************************
** FUNCTION: sip_parseImAddr
** DESCRIPTION: This function parses and stores  IM URL addrspec
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pUser			- The User to be returned by the parser
**				pHost			- The host to be returned by the parser
**
** NOTE: 
************************************************************************/
SipBool sip_parseImAddr(SIP_S8bit *pStart ,SIP_S8bit *pEnd ,\
	SIP_S8bit **pUser ,SIP_S8bit **pHost ,SipError *pErr)

{
	/*
	 * only addrspec is there in the  im: url
	 * mail box = user@host
	 * store the user and host
	 */ 
	SIP_S8bit *pCurrentPos	=	SIP_NULL;	
	SIP_S8bit *pBegin		=	SIP_NULL;	
	SIP_S8bit *pEn			=	SIP_NULL;	

	pCurrentPos = sip_tokenize(pStart ,pEnd ,'@');
	if (pCurrentPos ==SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	pBegin = sip_stripLeadingLWS(pStart, pCurrentPos-1);
	pEn = sip_stripTrailingLWS(pStart, pCurrentPos-1);
	*pUser = sip_nStrdup(DECODE_MEM_ID ,pBegin ,pEn-pBegin+1 ,pErr);	
	if (*pUser == SIP_NULL)
		return SipFail;

	pBegin = sip_stripLeadingLWS(pCurrentPos+1, pEnd);
	pEn = sip_stripTrailingLWS(pCurrentPos+1, pEnd);
	if (pEn < pBegin)
		*pHost = SIP_NULL;
	else
		*pHost = sip_nStrdup(DECODE_MEM_ID ,pBegin, pEn-pBegin+1, pErr);
	
	if (*pHost == SIP_NULL)
	{
		(void)fast_memfree(DECODE_MEM_ID ,*pUser, pErr);
		*pUser = SIP_NULL;
		return SipFail;
	}	
	*pErr = E_NO_ERROR;	
	return SipSuccess;
}/*End of function */

/***********************************************************************
** FUNCTION: sip_getImUrlFromAddrSpec
**
** DESCRIPTION: gets the ImUrl from the SipAddrSpec structure
**
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- retrieved ImUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_getImUrlFromAddrSpec (\
	SipAddrSpec *pAddrSpec, ImUrl **ppImUrl, SipError *pErr)
{
	SIP_S8bit *pCurrentPos;
	INC_API_COUNT

	SIPDEBUGFN ("Entering sip_getImUrlFromAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#ifndef SIP_BY_REFERENCE
	if (pImUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#else
	if (ppImUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (pAddrSpec->u.pUri == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	if ( sip_isImUrl(pAddrSpec, pErr) == SipFail)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#endif
	/*
	 * Trying to find the end of  im:
	 */ 
	pCurrentPos = sip_tokenize(pAddrSpec->u.pUri ,((pAddrSpec->u.pUri)+\
					sip_strlen(pAddrSpec->u.pUri)) ,':');
	if (pCurrentPos == SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	if (SipFail == sip_initImUrl(ppImUrl , pErr))
	{
		return SipFail;
	}
	/*
	 * Starting from the character after im:user@host
	 *                                      ^
	 *                                      |
	 */                                      
	if (SipFail == sip_parseImUrl(pCurrentPos +1, *ppImUrl ,pErr))
	{
		sip_freeImUrl( *ppImUrl);
		return SipFail;
	}

	SIPDEBUGFN ("Exiting sip_getImUrlFromAddrSpec\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}



#ifdef SIP_PRES

/***********************************************************************
** FUNCTION: sip_getPresUrlFromAddrSpec
**
** DESCRIPTION: gets the PresUrl from the SipAddrSpec structure
**
** PARAMETERS:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- retrieved ImUrl
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/


SipBool sip_getPresUrlFromAddrSpec (\
	SipAddrSpec *pAddrSpec, ImUrl **ppImUrl, SipError *pErr)
{
	SIP_S8bit *pCurrentPos;
	INC_API_COUNT

	SIPDEBUGFN ("Entering sip_getImUrlFromAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#ifndef SIP_BY_REFERENCE
	if (ppImUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

#else
	if (ppImUrl == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (pAddrSpec->u.pUri == SIP_NULL)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
	if ( sip_isPresUrl(pAddrSpec, pErr) == SipFail)
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}
#endif
	/*
	 * Trying to find the end of  pres:
	 */ 
	pCurrentPos = sip_tokenize(pAddrSpec->u.pUri ,((pAddrSpec->u.pUri)+\
					sip_strlen(pAddrSpec->u.pUri)) ,':');
	if (pCurrentPos == SIP_NULL)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	if (SipFail == sip_initImUrl(ppImUrl , pErr))
	{
		return SipFail;
	}
	/*
	 * Starting from the character after im:user@host
	 *                                      ^
	 *                                      |
	 */                                      
	if (SipFail == sip_parseImUrl(pCurrentPos +1, *ppImUrl ,pErr))
	{
		sip_freeImUrl( *ppImUrl);
		return SipFail;
	}

	SIPDEBUGFN ("Exiting sip_getPresUrlFromAddrSpec\n");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

#endif




/***********************************************************************
** Function: sip_isImUrl
** Description: Checks if the Addrspec has a im-url
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_isImUrl
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, SipError *pErr)
#else
	(pAddrSpec,pErr)
	SipAddrSpec *pAddrSpec;
	SipError *pErr;
#endif
{
	en_AddrType pAddrtype;
	SIP_S8bit *pTempUri=SIP_NULL;
	SIP_U32bit i=0;

	SIPDEBUGFN("Entering sip_isImUrl\n");
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
		if (( pTempUri[i] == 'I' || pTempUri[i] == 'i') &&
			( pTempUri[i+1] == 'M' || pTempUri[i+1] == 'm'))
		{
			for (i=2;((pTempUri[i] == ' ') || (pTempUri[i] == '\t'))&& \
					(pTempUri[i] != '\0');i++);

			if (pTempUri[i] == ':')
			{
				*pErr = E_NO_ERROR;
				SIPDEBUGFN("Exiting sip_isImUrl\n");
				return SipSuccess;
			}
		}
	}
	SIPDEBUGFN("Exiting sip_isImUrl\n");
	*pErr = E_INV_PARAM;
	return SipFail;

}

/***********************************************************************
** Function: sip_isPresUrl
** Description: Checks if the Addrspec has a Pres-url
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
#ifdef SIP_PRES

SipBool sip_isPresUrl
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, SipError *pErr)
#else
	(pAddrSpec,pErr)
	SipAddrSpec *pAddrSpec;
	SipError *pErr;
#endif
{
	en_AddrType pAddrtype;
	SIP_S8bit *pTempUri=SIP_NULL;
	SIP_U32bit i=0;
	SIPDEBUGFN("Entering sip_isPresUrl\n");
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
		if (( pTempUri[i] == 'P' || pTempUri[i] == 'p') &&
			( pTempUri[i+1] == 'R' || pTempUri[i+1] == 'r')&&
			( pTempUri[i+2] == 'E' || pTempUri[i+2] == 'e')&&
			( pTempUri[i+3] == 'S' || pTempUri[i+3] == 's'))
		{
			for (i=4;((pTempUri[i] == ' ') || (pTempUri[i] == '\t'))&& \
					(pTempUri[i] != '\0');i++);

			if (pTempUri[i] == ':')
			{
				*pErr = E_NO_ERROR;
				SIPDEBUGFN("Exiting sip_isPresUrl\n");
				return SipSuccess;
			}
		}
	}
	SIPDEBUGFN("Exiting sip_isPresUrl\n");
	*pErr = E_INV_PARAM;
	return SipFail;

}
#endif


/***********************************************************************
** Function: sip_setImUrlInAddrSpec
** Description: sets the ImUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- ImUrl to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
SipBool sip_setImUrlInAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, ImUrl *pImUrl, SipError *pErr)
#else
	(pAddrSpec, pImUrl, pErr)
	SipAddrSpec *pAddrSpec;
	ImUrl *pImUrl;
	SipError *pErr;
#endif
{
	SIP_S8bit *pOut=SIP_NULL;
	SIP_U32bit listSize=0;

	SIPDEBUGFN("Entering sip_setImUrlInAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pAddrSpec->dType != SipAddrReqUri )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if ( pAddrSpec->u.pUri != SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pAddrSpec->u.pUri),\
			pErr) ==SipFail)
			return SipFail;
	}

	if (pImUrl==SIP_NULL)
	{
		pAddrSpec->u.pUri=SIP_NULL;
		*pErr=E_NO_ERROR;
	}
	else
	{
		pOut=(SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID,MAX_IM_URL_SIZE, pErr);
		if (pOut == SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy(pOut,(SIP_S8bit*)"im:");

		if(pImUrl->pDispName != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pDispName);
			(void)STR_CAT(pOut,(SIP_S8bit*)"%3c");
		}

		(void)sip_formSipList(pOut, &(pImUrl->slRoute), (SIP_S8bit *) ",",1, pErr);

		if(pImUrl->pUser != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pUser);
			(void)STR_CAT(pOut,(SIP_S8bit*)"@");
		}

		if(pImUrl->pHost != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pHost);
		}

		if(pImUrl->pDispName != SIP_NULL)
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"%3e");
		}

		/* Now Form Header */
		(void)sip_listSizeOf(&(pImUrl->slParams), &listSize, pErr);
		if(listSize!=0)
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"?");
		}

		/* Called with leadingsep value of 2 to avoid putting leading space or
		   leading separator */
 
		(void)sip_formSipParamList(/* CSR_1-6613911 Change Start */ SIP_NULL, /* CSR_1-6613911 Change End */pOut, &(pImUrl->slParams), (SIP_S8bit *) "&",\
			0, pErr);

		pAddrSpec->u.pUri=pOut;
	}

	SIPDEBUGFN("Exitting sip_setImUrlInAddrSpec\n");
	return SipSuccess;
}


/***********************************************************************
** Function: sip_setPresUrlInAddrSpec
** Description: sets the PresUrl in the SipAddrSpec structure
** Parameters:
**		pAddrSpec (IN)	- SipAddrSpec
**		pImUrl (OUT)	- ImUrl to be set
**		pErr (OUT)		- Possible Error value (see API ref doc)
************************************************************************/
#ifdef SIP_PRES

SipBool sip_setPresUrlInAddrSpec
#ifdef ANSI_PROTO
	(SipAddrSpec *pAddrSpec, ImUrl *pImUrl, SipError *pErr)
#else
	(pAddrSpec, pImUrl, pErr)
	SipAddrSpec *pAddrSpec;
	ImUrl *pImUrl;
	SipError *pErr;
#endif
{
	SIP_S8bit *pOut=SIP_NULL;
	SIP_U32bit listSize=0;

	SIPDEBUGFN("Entering sip_setImUrlInAddrSpec\n");
#ifndef SIP_NO_CHECK
	if (pErr == SIP_NULL)
		return SipFail;
	if (pAddrSpec == SIP_NULL)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
	if( pAddrSpec->dType != SipAddrReqUri )
	{
		*pErr = E_INV_TYPE;
		return SipFail;
	}
#endif
	if ( pAddrSpec->u.pUri != SIP_NULL)
	{
		if(sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid*)&(pAddrSpec->u.pUri),\
			pErr) ==SipFail)
			return SipFail;
	}

	if (pImUrl==SIP_NULL)
	{
		pAddrSpec->u.pUri=SIP_NULL;
		*pErr=E_NO_ERROR;
	}
	else
	{
		pOut=(SIP_S8bit *)fast_memget(ACCESSOR_MEM_ID,MAX_IM_URL_SIZE, pErr);
		if (pOut == SIP_NULL)
		{
			*pErr = E_NO_MEM;
			return SipFail;
		}
		(void)sip_strcpy(pOut,(SIP_S8bit*)"pres:");

		if(pImUrl->pDispName != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pDispName);
			(void)STR_CAT(pOut,(SIP_S8bit*)"%3c");
		}

		(void)sip_formSipList(pOut, &(pImUrl->slRoute), (SIP_S8bit *) ",",1, pErr);

		if(pImUrl->pUser != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pUser);
			(void)STR_CAT(pOut,(SIP_S8bit*)"@");
		}

		if(pImUrl->pHost != SIP_NULL)
		{
			(void)STR_CAT(pOut,pImUrl->pHost);
		}

		if(pImUrl->pDispName != SIP_NULL)
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"%3e");
		}

		/* Now Form Header */
		(void)sip_listSizeOf(&(pImUrl->slParams), &listSize, pErr);
		if(listSize!=0)
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"?");
		}

		/* Called with leadingsep value of 2 to avoid putting leading space or
		   leading separator */
 
		(void)sip_formSipParamList(/* CSR_1-6613911 Change Start */ SIP_NULL, /* CSR_1-6613911 Change End */pOut, &(pImUrl->slParams), (SIP_S8bit *) "&",\
			0, pErr);

		pAddrSpec->u.pUri=pOut;
	}

	SIPDEBUGFN("Exitting sip_setImUrlInAddrSpec\n");
	return SipSuccess;
}

#endif


/*****************************************************************
** FUNCTION: sip_formSipList
** DESCRIPTION: Converts a list of String to text
** Parameters:
** 		pOut(OUT)    - output buffer
**		pList(IN)       -  the string list to be converted
** 		pSeperator(IN)	- each element to be sepearated by
**		dLeadingsep(IN)	- leading seperator
**		pErr (OUT)		- Possible Error value (see API ref doc)
*****************************************************************/

SipBool sip_formSipList
#ifdef ANSI_PROTO
	(SIP_S8bit 	*pOut, 
	SipList 	*pList, 
	SIP_S8bit 	*pSeparator,
	SIP_U8bit	dLeadingsep,
	SipError 	*pErr)
#else
	(pOut, pList, pSeparator, dLeadingsep, pErr)
	SIP_S8bit *pOut;
	SipList *pList;
	SIP_S8bit *pSeparator;
	SIP_U8bit dLeadingsep;
	SipError *pErr;
#endif
{
		SIP_U32bit listSize,listIter;
		(void)sip_listSizeOf( pList, &listSize, pErr);
		listIter = 0;
		while (listIter < listSize)
		{
			SIP_S8bit *pString;
			(void)sip_listGetAt (pList, listIter, (SIP_Pvoid *) &pString, pErr);
			if((listIter!=0)&&(dLeadingsep!=0))
				(void)STR_CAT ((SIP_S8bit *)pOut, pSeparator);
			
			(void)STR_CAT(pOut,(SIP_S8bit*)"@");
			(void)STR_CAT(pOut,pString);
			listIter++;
		} /* while */
		if(listSize!=0)
			(void)STR_CAT(pOut,(SIP_S8bit*)":");
			
		return SipSuccess;
}


