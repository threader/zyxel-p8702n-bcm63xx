/***********************************************************************
** FUNCTION:
**			Header parsing functionality
************************************************************************
**
** FILENAME:
** microsip_hdrparser.c
**
** DESCRIPTION:
**		This file contains functions for parsing SIP headers. It uses
**		functions defined in microsip_subparsers.c for parsing sub-fields. 
**
** DATE	 		NAME			 REFERENCE	 	REASON
** ----	 		----			 ---------	 	--------
** 15/02/2002	Aparna Kuppachi		-			Initial					
** 23/02/2002	Mahesh Govind		-			Added Via and Contact
**												parsers.
**	25-Apr-2002	Aparna				-			Fix for SPR 1_0_3.
**
** 31/03/2009   Kamal Ashraf     SPR 19280      Fix For SPR 19280
** 23-Apr-2009  Rajiv Kumar      IPTK Rel 8.2   Fix For SPR 19464
** 05-Jun-2009   Anuradha Gupta  SPR 19737       Klocworks Warning Removal 
**
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#include "microsip_hdrparsers.h"
#include "microsip_subparsers.h"
#include "microsip_bcptfree.h"



#ifdef SIP_TIMESTAMP			
/****************************************************************************
** FUNCTION: sip_parseTimeStampHeader
** DESCRIPTION: This function parses the TimeStamp Header 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseTimeStampHeader
#ifdef ANSI_PROTO
(SIP_S8bit *pStart, SIP_S8bit *pEnd, SipHeader *pOutHdr,\
			en_HeaderType dType, SipError *pErr)
#else
(pStart, pEnd, hdr, pOutHdr, pErr)
SIP_S8bit *pStart;
SIP_S8bit *pEnd;
SipHeader *pOutHdr;
SipError *pErr;
#endif
{
	SIP_S8bit 			*pCurrentPos=pStart;
	SIP_S8bit			*pToken, *pTemp;
	SipTimeStampHeader	*pTimeStampHdr;
	SIP_U32bit			size, i;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseTimeStampHeader");
	pTimeStampHdr = (SipTimeStampHeader *)(pOutHdr->pHeader);
	
	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* begin fix for SPR 1_0_14 */
	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	/* end fix for SPR 1_0_14 */

	/* Now pCurrentPos points to a non-LWS character */

	/* Tokenize for SP */
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/* No Delay field present */
		pTimeStampHdr->pStr2 = SIP_NULL;
	
		/* Skip trailing LWS and store time field */
		pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

		size = pEnd - pCurrentPos + 1; /* size of time field in hdr */

		/* begin fix for SPR1_0_14 */
		for (i=0; i < size; i++)
		{
			if ((pCurrentPos[i] < 48) || (pCurrentPos[i] > 57))
			{
				if (pCurrentPos[i] != '.')
				{
					/* 
					 * The time field contains a char which is neither 
					 * a digit nor a '.'; raise parser error.
					 */

					sip_error(SIP_Minor, "A char other than digit or . present in time fld");
					*pErr = E_PARSER_ERROR;
					return SipFail;
				}
			}
		}
		/* end fix for SPR1_0_14 */

		pTimeStampHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, size, \
			pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTimeStampHdr->pStr1)
		{
			return SipFail;
		}
#endif		
	} /* end of if no SP found */ 
	else
	{
		/* 
		 * Both time and delay fields are present. pToken now points
		 * to the SP character found. So, the time field is contained
		 * in pCurrentPos to (pToken - 1).
		 */
		size = pToken - pCurrentPos; /* size of time field in hdr */

		/* begin fix for SPR1_0_14 */
		for (i=0; i < size; i++)
		{
			if ((pCurrentPos[i] < 48) || (pCurrentPos[i] > 57))
			{
				if (pCurrentPos[i] != '.')
				{
					/* 
					 * The time field contains a char which is neither 
					 * a digit nor a '.'; raise parser error.
					 */

					sip_error(SIP_Minor, "A char other than digit or . present in time fld");
					*pErr = E_PARSER_ERROR;
					return SipFail;
				}
			}
		}
		/* end fix for SPR1_0_14 */

		pTimeStampHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, size,\
			pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL ==  pTimeStampHdr->pStr1)
		{
			return SipFail;
		}
#endif		
		
		/* Now skip trailing LWS and store delay field */
		pEnd = sip_stripTrailingLWS(pToken + 1/*pCurrentPos*/, pEnd);

		size = pEnd - (pToken +1) + 1; /* size of delay field in hdr */

		/* begin fix for SPR1_0_14 */
		pTemp = pToken + 1;
		for (i=0; i < size; i++)
		{
			if ((pTemp[i] < 48) || (pTemp[i] > 57))
			{
				if (pTemp[i] != '.')
				{
					/* 
					 * The delay field contains a char which is neither 
					 * a digit nor a '.'; raise parser error.
					 */

					sip_error(SIP_Minor, \
						"A char other than digit or . present in delay fld");
					*pErr = E_PARSER_ERROR;
					return SipFail;
				}
			}
		}
		/* end fix for SPR1_0_14 */

		pTimeStampHdr->pStr2 = sip_nStrdup(DECODE_MEM_ID, pToken + 1, size,\
			pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTimeStampHdr->pStr2)
		{
			return SipFail;
		}
#endif		
	} /* end of else if both time and delay present */

	SIPDEBUGFN("Exiting from sip_parseTimeStampHeader");
	return SipSuccess;

} /* end of sip_parseTimeStampHeader */
#endif



/****************************************************************************
** FUNCTION: sip_parseContentTypeHeader
** DESCRIPTION: This function parses the Content Type Header 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseContentTypeHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SipContentTypeHeader	*pContentTypeHdr;
	SIP_S8bit				*pCurrentPos = pStart;
	SIP_S8bit				*pToken;
	SIP_S8bit				*pPrevToken;
	SIP_U32bit				size;
	
	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseContentTypeHeader");
	pContentTypeHdr = (SipContentTypeHeader *)(pOutHdr->pHeader);
	
	/* 
	 * Skip leading LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	if( dType == SipHdrTypeAccept )
	{
		pToken =  sip_strstr(pCurrentPos,";q=");
		if( pToken != SIP_NULL)
		{
			size = pEnd - pToken+ 1;

			/* check for no q value */
			if(size <= 3)
					return SipFail;

			pContentTypeHdr->pStr2 = sip_nStrdup(DECODE_MEM_ID, pToken, \
				size, pErr);
#ifndef SIP_NO_CHECK
			if (SIP_NULL == pContentTypeHdr->pStr2)
			{
				return SipFail;
			}
#endif	
			/* Check for multiple q lines */
			if ( sip_strstr((pContentTypeHdr->pStr2+3),";q=") != SIP_NULL )
			{
				(void)fast_memfree(0,pContentTypeHdr->pStr2,SIP_NULL);
				pContentTypeHdr->pStr2=SIP_NULL;
				return SipFail;
			}
		}	
	}	/* end of if  dType == SipHdrTypeAccept*/
	else
	{	
	/* 
	 * Tokenize for ; 
	 */

		pToken = sip_tokenize(pCurrentPos, pEnd, ';');
		if (SIP_NULL != pToken)
		{
				/*
				* Paramters present; call param parser.
				*/

				if (SipFail == sip_parseSipParam(&(pContentTypeHdr->slParam), \
				pToken + 1, pEnd, SipTrue, ';', pErr)) 
				{
						return SipFail;
				}
		} /* end of if params found */
	}
	pPrevToken = pToken; /* Store the position where params(if any) start */	

	/* Tokenize for / */
	pToken = sip_tokenize(pCurrentPos, 
		(SIP_NULL == pPrevToken) ? pEnd:(pPrevToken - 1), '/');
	if (SIP_NULL == pToken)
	{
		/* 
		 * No media subtype present; raise parser error.
		 */
       	/* Fix for CSR 1-7181055 */
		/* work-around change start */ 
   		if(((dType == SipHdrTypeAccept) && 
			((*pCurrentPos == '\n') ||(*pCurrentPos == '\r')))) 
        {    
      		return SipSuccess; 
        }    
   		else 
    	{ 
    	/* work-around change end */

		   sip_error(SIP_Minor, \
			"Syntax error while parsing ContentType hdr: no media subtype present");
		   *pErr = E_PARSER_ERROR;
		   return SipFail;
        /* work-around change start */ 
    	} 
    	/* work-around change end */
	}		
	else 
	{
		SIP_S8bit	*pTemp, *pTemp1, *pTemp2, *pTemp3;

		/*
		 * Store media type and subtype 
		 */

		if (SIP_NULL != pPrevToken)
			pPrevToken = sip_stripTrailingLWS(pCurrentPos, pPrevToken - 1);

		size = ((SIP_NULL == pPrevToken) ? pEnd:pPrevToken) - \
			pCurrentPos + 1;

		pContentTypeHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			size, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pContentTypeHdr->pStr1)
		{
			return SipFail;
		}
#endif		
		/* begin SPR1_0_8 */
		pTemp = pContentTypeHdr->pStr1;
		pTemp1 = sip_stripLeadingLWS(pTemp, pTemp + size - 1);
		pTemp2 = sip_stripTrailingLWS(pTemp, pTemp1 + size - 1);
		pTemp3 = sip_tokenize(pTemp1, pTemp2, '/');
		if ((0 >= (pTemp3 - pTemp1)) || (0 >= (pTemp2 - pTemp3)))
		{
			sip_error(SIP_Minor, \
				"Syntax error while parsing ContentType hdr: Incomplete header");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		/* end SPR1_0_8 */

	} /* end of else media subtype found */
	
	SIPDEBUGFN("Exiting from sip_parseContentTypeHeader");
	return SipSuccess;

} /* end of function sip_parseContentTypeHeader */


/****************************************************************************
** FUNCTION: sip_parseCSeqHeader
** DESCRIPTION: This function parses the CSeq Header 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseCSeqHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_S8bit		*pTemp;
	SipCseqHeader	*pCSeqHdr;
	SIP_U32bit		size;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseCSeqHeader");
	pCSeqHdr = (SipCseqHeader *) (pOutHdr->pHeader);
	
	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* begin SPR1_0_9 */
	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	/* end SPR1_0_9 */

	/* Tokenize for SP */
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		sip_error(SIP_Minor, "Syntax error in CSeq hdr: incomplete hdr"); 
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Validate and store the Seq Number part 
	 */
	pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1); 
	if (SipFail == sip_atoi(pCurrentPos, pTemp, &pCSeqHdr->dIntVar1))	
	{
		/*
		 * Non-numeric Seq Num found; raise parser error
		 */

		sip_error(SIP_Minor, "Syntax error in parsing CSeq hdr: non-numeric \
			seq num found");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Store the Method after stripping leading LWS 
	 */

	pTemp = sip_stripLeadingLWS(pToken + 1, pEnd); 

	size = pEnd - pTemp + 1;
	if (0 >= size)
	{
		/*
		 * No method present
		 */

		*pErr = E_NO_MEM;
		return SipFail;
	}
	
	pCSeqHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pTemp, size, pErr);	
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pCSeqHdr->pStr1)
	{	
		return SipFail;
	}
#endif	

	SIPDEBUGFN("Exiting from sip_parseCSeqHeader");
	return SipSuccess;

} /* end of function sip_parseCSeqHeader */



/****************************************************************************
** FUNCTION: sip_parseContentDispositionHeader
** DESCRIPTION: This function parses the following headers:
**				Event, Content-Disposition, Replaces, Subscription-State.
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseContentDispositionHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SipContentDispositionHeader	*pEventHdr;
	SIP_U32bit		size;
	SIP_S8bit		*pTemp;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseContentDispositionHeader");
	pEventHdr = (SipContentDispositionHeader *)(pOutHdr->pHeader);

	/*
	 * Skip leading LWS 
	 */

	 pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	pToken = sip_tokenize(pCurrentPos, pEnd, ';');
	/*
	 * Store the Event-type if it has non-zero 
	 * length.
	 */

	pTemp = sip_stripTrailingLWS(pCurrentPos, \
		(SIP_NULL == pToken)?pEnd:(pToken - 1));
	size = pTemp - pCurrentPos + 1; /* size of the Event-type fd */

	if (0 >= size)
	{
		/* 
		 * No event-type present; raise error.
		 */

#ifdef SIP_ERROR
		if (dType == SipHdrTypeContentDisposition)
			sip_error(SIP_Minor, "Syntax error in Content-Disposition hdr: no event-type "
			"present");
#ifdef SIP_IMPP
		else if (dType == SipHdrTypeEvent)
			sip_error(SIP_Minor, "Syntax error in Event hdr: no event-type "
			"present");
#endif
#ifdef SIP_REPLACES
		else if (dType == SipHdrTypeReplaces)
			sip_error(SIP_Minor, "Syntax error in Replaces hdr: no event-type "
			"present");
#endif
#ifdef SIP_IMPP
		else if (dType == SipHdrTypeSubscriptionState)
			sip_error(SIP_Minor, "Syntax error in Subscription-State hdr: no event-type "
			"present");
#endif
#ifdef SIP_SECURITY
		else if (dType == SipHdrTypeSecurityClient)
			sip_error(SIP_Minor, "Syntax error in SecurityClient hdr: no mechanism-name "
			"present");
        else if (dType == SipHdrTypeSecurityVerify)
			sip_error(SIP_Minor, "Syntax error in SecurityVerify hdr: no mechanism-name "
			"present");
        else if (dType == SipHdrTypeSecurityServer)
			sip_error(SIP_Minor, "Syntax error in SecurityServer hdr: no mechanism-name "
			"present");
#endif        
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	pEventHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, size, pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pEventHdr->pStr1)
	{
		return SipFail;
	}
#endif	

	if (SIP_NULL != pToken)
	{
		/*
		 * Params present
		 */

		if (SipFail == sip_parseSipParam(&(pEventHdr->slParam), pToken + 1, \
			pEnd, SipFalse, ';', pErr))
		{
			return SipFail;
		}

	} /* end of if params present */
	
	SIPDEBUGFN("Exiting from sip_parseContentDispositionHeader");
	return SipSuccess;

} /* end of function sip_parseContentDispositionHeader */

#ifdef SIP_PRIVACY
/****************************************************************************
** FUNCTION: sip_parsePrivacyHeader
** DESCRIPTION: This function parses the following headers:
**				      Privacy.
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parsePrivacyHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SipPrivacyHeader	*pPrivacyHdr;
	SIP_U32bit		size;
    /*SIP_S8bit		*pTemp; */
	(void)	dType;
    
	SIPDEBUGFN("Entering into sip_parsePrivacyHeader");
	pPrivacyHdr = (SipPrivacyHeader *)(pOutHdr->pHeader);

	/*
	 * Skip leading LWS 
	 */

	 pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/*pTemp = sip_stripTrailingLWS(pCurrentPos, pEnd);*/
	size = pEnd - pCurrentPos + 1; /* size of the Event-type fd */

	if (0 >= size)
	{
#ifdef SIP_ERROR
			sip_error(SIP_Minor, "Syntax error in Privacy hdr: no privacy value "
			"present");
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	if (SIP_NULL != pCurrentPos)
	{
		if (SipFail == sip_parseSipParam(&(pPrivacyHdr->slParam), pCurrentPos, \
			pEnd, SipFalse, ';', pErr))
		{
			return SipFail;
		}

	} 	
	SIPDEBUGFN("Exiting from sip_parsePrivacyHeader");
	return SipSuccess;

} /* end of function sip_parsePrivacyHeader */

#endif

/****************************************************************************
** FUNCTION: sip_parseFromHeader
** DESCRIPTION: This function parses the From, To, Referred-By, Route
**				and Record-Route Headers. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseFromHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_S8bit		*pAddrSpecEnd = SIP_NULL;
	SipFromHeader	*pFromHdr;
	SIP_U32bit		size;
	SipAddrSpec		*pAddrSpec;
	SIP_S8bit		*pTemp2 = SIP_NULL;
	SIP_S8bit		*pExtraCharsBegin, *pExtraCharsEnd;

	/*
	 * Variable indicating whether right angular quote is expected or not
	 */
	en_SipBoolean	raquotExpected = SipFalse; 

	(void) dType;

	SIPDEBUGFN("Entering into sip_parseFromHeader");
	pFromHdr = (SipFromHeader *) (pOutHdr->pHeader);

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	/* Now pCurrentPos points to a non-LWS character */

	/*
	 * we need to handle the Display name with a quoted string specifically
	 * Other wise in the current approach if the header (with display namen)
	 * starts with a quoted string and not ending with  quoted string 
	 * returns a null if one tokenzie for  a < . 
	 * So if the first character is a " and tokenize for < returns null, one
	 * can return a fail.This means that the display name is not ended .In
	 * the initial releases this was not like this.this case came because
	 * in sipfunction sip_tokenize(), was made strict for checking quoted 
	 * string.Else  what happens is the entire uri will be
	 * treated as a URI. Since From To , Route record Route , referTo
	 * ,Referred by and path headers are parsed by the same parser it will
	 * be good to  doi this check.This is done here not to disturb the top
	 * level check for the quotes .Actually it has to return fail from the
	 * top level , strictly speaking  .Since it is not wright to disturb
	 * the code in top level parsing , it is better to do this here.
	* Tokenize for < */
	
	pToken = sip_tokenize(pCurrentPos, pEnd, '<');
	if (SIP_NULL == pToken)
	{
		/*
		 * This failure is to handle the above explained case.
		 */
		if('"'== *pCurrentPos )
		{
			sip_error(SIP_Minor, "Syntax error in From Family hdr: dangling Quote\" ");
			return SipFail;
		}	
		/*
		 * Display name is not present 
		 */
		
		pFromHdr->pStr1 = SIP_NULL;
	}
	else
	{
		/*
		 * Display name may or may not be present. If display name 
		 * is of non-zero length, store it. 
		 */

		SIP_S8bit	*pTemp;

        if (pCurrentPos <= (pToken - 1))
        {
	      pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
	      size = pTemp - pCurrentPos + 1; /* size of display name fd */
        }
        else
        {
          size = 0;
        }

		if (size > 0)
		{
			/*
			 * Display name is present 
			 */

			pFromHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, size,\
				pErr);
#ifndef SIP_NO_CHECK
			if (SIP_NULL == pFromHdr->pStr1)
			{
				return SipFail;
			}
#endif			
		}
		else
			pFromHdr->pStr1 = SIP_NULL;
		/*
		 * Advance pCurrentPos
		 */

		pCurrentPos = pToken + 1;

		/* 
		 * LAQUOT found, now look for RAQUOT
		 */

		raquotExpected = SipTrue;

	} /* end of else if disp-name present */
	
	/*
	 * Now pCurrentPos points to start of addr-spec.			
	 * Next, determine end of addr-spec 
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, '>');
	if (SIP_NULL != pToken)
	{
		if (SipFalse == raquotExpected)
		{
			/*
			 * RAQUOT found whereas LAQUOT was never encountered.
			 * Raise parser error.
			 */

#ifdef SIP_ERROR
			if (dType == SipHdrTypeFrom)
				sip_error(SIP_Minor, "Syntax error in From hdr: dangling right "
				"angular quote found");
			else if (dType == SipHdrTypeTo)
				sip_error(SIP_Minor, "Syntax error in To hdr: dangling right "
				"angular quote found");
#ifdef SIP_REFER
			else if (dType == SipHdrTypeReferredBy)
				sip_error(SIP_Minor, "Syntax error in Referred-By hdr: dangling right "
				"angular quote found");
#endif
			else if (dType == SipHdrTypeRoute)
				sip_error(SIP_Minor, "Syntax error in Route hdr: dangling right "
				"angular quote found");
			else if (dType == SipHdrTypeRecordRoute)
				sip_error(SIP_Minor, "Syntax error in Record-Route hdr: dangling right "
				"angular quote found");
#ifdef SIP_3GPP				
			else if (dType == SipHdrTypePath)
				sip_error(SIP_Minor, "Syntax error in Path hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypePAssociatedUri)
				sip_error(SIP_Minor, "Syntax error in PAssociatedUri hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypePCalledPartyId)
				sip_error(SIP_Minor, "Syntax error in PCalledPartyId hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypeHistoryInfo)
				sip_error(SIP_Minor, "Syntax error in HistoryInfo hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypeServiceRoute)
				sip_error(SIP_Minor, "Syntax error in Service-Route hdr: dangling right "
				"angular quote found");
#endif
#ifdef SIP_PRIVACY
				else if (dType == SipHdrTypePPreferredId) 
				sip_error(SIP_Minor, "Syntax error in PPreferredI hdr: dangling right "
				"angular quote found");
				
				else if (dType == SipHdrTypePAssertId)
				sip_error(SIP_Minor, "Syntax error in PAssertId hdr: dangling right "
				"angular quote found");
#endif
#endif
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}

		pAddrSpecEnd = pToken - 1;

		/*begin SPR 9*/
		/*
		 * Store the position where ">" is found. Required later for
		 * checking for presence of dangling characters after the end of
		 * addr-spec.
		 */

		pTemp2 = pToken;
		/*end SPR 9*/
	}
	else
	{
		if (SipTrue == raquotExpected)
		{
			/* 
			 * RAQUOT expected but not found.
			 * Raise parser error.
			 */

#ifdef SIP_ERROR
			if (dType == SipHdrTypeFrom)
				sip_error(SIP_Minor, "Syntax error in From hdr: dangling left"
				"angular quote found");
			else if (dType == SipHdrTypeTo)
				sip_error(SIP_Minor, "Syntax error in To hdr: dangling left"
				"angular quote found");
#ifdef SIP_REFER
			else if (dType == SipHdrTypeReferredBy)
				sip_error(SIP_Minor, "Syntax error in Referred-By hdr: dangling left"
				"angular quote found");
#endif
			else if (dType == SipHdrTypeRoute)
				sip_error(SIP_Minor, "Syntax error in Route hdr: dangling left"
				"angular quote found");
			else if (dType == SipHdrTypeRecordRoute)
				sip_error(SIP_Minor, "Syntax error in Record-Route hdr: dangling left"
				"angular quote found");
#ifdef SIP_3GPP
		  else if (dType == SipHdrTypeServiceRoute)
				sip_error(SIP_Minor, "Syntax error in Service-Route hdr: dangling left"
				"angular quote found");
				
		  else if (dType == SipHdrTypePath)
				sip_error(SIP_Minor, "Syntax error in Path hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypePAssociatedUri)
				sip_error(SIP_Minor, "Syntax error in PAssociatedUri hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypePCalledPartyId)
				sip_error(SIP_Minor, "Syntax error in PCalledPartyId hdr: dangling right "
				"angular quote found");
				
			else if (dType == SipHdrTypeHistoryInfo)
				sip_error(SIP_Minor, "Syntax error in HistoryInfo hdr: dangling right "
				"angular quote found");

#endif				
#ifdef SIP_PRIVACY
		 else if (dType == SipHdrTypePPreferredId) 
			 sip_error(SIP_Minor, "Syntax error in PPreferredI hdr: dangling right "
			 "angular quote found");
				
		 else if (dType == SipHdrTypePAssertId)
			sip_error(SIP_Minor, "Syntax error in PAssertId hdr: dangling right "
			"angular quote found");
#endif

#endif
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}

		/*
		 * In this case end of addr-spec is as yet undetermined.
		 */

	}

	/* 
	 * Search for parameters
	 */
	
	pToken = sip_tokenize((SIP_NULL == pToken)?pCurrentPos : pToken + 1, 
				pEnd, ';');
	if (SIP_NULL == pToken)
	{
		/* 
		 * No from-params present
		 */

		if (SIP_NULL == pAddrSpecEnd)
			pAddrSpecEnd = pEnd;

	}
	else
	{
		if (SIP_NULL == pAddrSpecEnd)
			pAddrSpecEnd = pToken - 1;
	}

	/*begin SPR 9*/

	/* 
	 * Fix for PQT reported SPR-06. If any dangling characters are present
	 * after the address spec that are not params, then throw a parser
	 * error.
	 */

	if ((SIP_NULL != pTemp2) && (pTemp2 < pEnd))
	{
		/* 
		 * Determine beginning and end of such dangling chars.
		 */

		pExtraCharsBegin = pTemp2 + 1;
		pExtraCharsEnd = (SIP_NULL == pToken)?pEnd:pToken - 1;
		pExtraCharsBegin = sip_stripLeadingLWS(pExtraCharsBegin, pExtraCharsEnd);
		pExtraCharsEnd = sip_stripTrailingLWS(pExtraCharsBegin, pExtraCharsEnd);

		/*
		 * If non-white space dangling chars found, throw an error.
		 */

		if (0 < (pExtraCharsEnd - pExtraCharsBegin + 1))
		{
#ifdef SIP_ERROR
			if (dType == SipHdrTypeFrom)
				sip_error(SIP_Minor, "Syntax error in From hdr: dangling characters"
				" after address spec");
			else if (dType == SipHdrTypeTo)
				sip_error(SIP_Minor, "Syntax error in To hdr: dangling characters"
				" after address spec");
#ifdef SIP_REFER
			else if (dType == SipHdrTypeReferredBy)
				sip_error(SIP_Minor, "Syntax error in Referred-By hdr: dangling characters"
				" after address spec");
#endif
			else if (dType == SipHdrTypeRoute)
				sip_error(SIP_Minor, "Syntax error in Route hdr: dangling characters"
				" after address spec");
			else if (dType == SipHdrTypeRecordRoute)
				sip_error(SIP_Minor, "Syntax error in Record-Route hdr: dangling characters"
				" after address spec");
#ifdef SIP_3GPP
		 else if (dType == SipHdrTypeServiceRoute)
			sip_error(SIP_Minor, "Syntax error in Service-Route hdr: dangling characters"
				" after address spec");
		else if (dType == SipHdrTypePath)
				sip_error(SIP_Minor, "Syntax error in Path hdr: dangling right "
				"angular quote found");
				
		else if (dType == SipHdrTypePAssociatedUri)
				sip_error(SIP_Minor, "Syntax error in PAssociatedUri hdr: dangling right "
				"angular quote found");
				
		else if (dType == SipHdrTypePCalledPartyId)
				sip_error(SIP_Minor, "Syntax error in PCalledPartyId hdr: dangling right "
				"angular quote found");
				
		else if (dType == SipHdrTypeHistoryInfo)
				sip_error(SIP_Minor, "Syntax error in HistoryInfo hdr: dangling right "
				"angular quote found");

#endif
#ifdef SIP_PRIVACY
	  else if (dType == SipHdrTypePPreferredId) 
				sip_error(SIP_Minor, "Syntax error in PPreferredI hdr: dangling right "
				"angular quote found");
				
	  else if (dType == SipHdrTypePAssertId)
				sip_error(SIP_Minor, "Syntax error in PAssertId hdr: dangling right "
				"angular quote found");
#endif

#endif
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}
	/*end SPR 9*/

	/*
	 * Invoke address spec parser
	 */

	if (SipFail == sip_parseAddrSpec(pCurrentPos, pAddrSpecEnd, \
		&pAddrSpec, pErr)) 
	{
                if(SIP_NULL != pFromHdr->pStr1)
		   (void)fast_memfree(0,pFromHdr->pStr1,SIP_NULL);
		pFromHdr->pStr1=SIP_NULL;
		return SipFail;
	}
	pFromHdr->pAddrSpec = pAddrSpec;
#ifdef SIP_PRIVACY
	if((dType != SipHdrTypePPreferredId) || (dType != SipHdrTypePAssertId))
  {
#endif
		if (SIP_NULL != pToken)
		{
				/*
				 * Params expected; call param parser.
				 */

				pCurrentPos = pToken + 1; 
				if (SipFail == sip_parseSipParam(&(pFromHdr->slParam), pCurrentPos,\
					pEnd, SipFalse, ';', pErr))
				{
					return SipFail;
				}	
		}
#ifdef SIP_PRIVACY
	}
#endif
				
	SIPDEBUGFN("Exiting from sip_parseFromHeader");
	return SipSuccess;	

} /* end of function sip_parseFromHeader */

		
			
/****************************************************************************
** FUNCTION: sip_parseStringHeader
** DESCRIPTION: This function parses the Allow, Supported, Unsupported, Allow-
**				Events, Require, Call-Id and Content-Encoding Headers. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseStringHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit 				*pCurrentPos = pStart;
	SipUnsupportedHeader	*pHdr;
	
	SIPDEBUGFN("Entering into sip_parseStringHeader");
	/* 
	 * Skip leading LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* 
	 * Skip trailing LWS 
	 */

	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	pHdr = (SipUnsupportedHeader *) (pOutHdr->pHeader);

	/* 
	 * If the header is "Supported" or "Allow", allow empty header body.
	 */

	if (((0 == (pEnd - pCurrentPos + 1)) && (SipHdrTypeSupported == dType)) ||
            		((0 == (pEnd - pCurrentPos + 1)) && (SipHdrTypeAllow == dType)) 
#ifdef SIP_PEMEDIA
 || ((0 == (pEnd - pCurrentPos + 1)) && (SipHdrTypePEarlyMedia == dType)) 
#endif
	)
	{
		pHdr->pStr1 = SIP_NULL;
	}
	else
	{
		pHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pEnd - pCurrentPos + 1, pErr);
		if (SIP_NULL == pHdr->pStr1)
		{
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting from sip_parseStringHeader");
	return SipSuccess;

} /* end of function sip_parseStringHeader */


		

/****************************************************************************
** FUNCTION: sip_parseWarningHeader
** DESCRIPTION: This function parses the Warning Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_WARNING
SipBool	sip_parseWarningHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit			*pCurrentPos = pStart;
	SipWarningHeader	*pWarningHdr;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseWarningHeader");
	pWarningHdr = (SipWarningHeader *) (pOutHdr->pHeader);

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Now pCurrentPos points to a non-LWS character */

	/* begin fix for SPR 1_0_10 */
	if (SipFail == sip_parseWarningValue(pCurrentPos, 
		pEnd, &pWarningHdr, pErr))
	{
		return SipFail;
	}
	/* end fix for SPR 1_0_10 */

	SIPDEBUGFN("Exiting from sip_parseWarningHeader");
	return SipSuccess;

} /* end of function sip_parseWarningHeader */
 



SipBool	sip_parseWarningValue(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipWarningHeader **ppWarningHdr, SipError *pErr)
{
	SIP_S8bit 	*pCurrentPos = pStart;

	SIPDEBUGFN("Entering into sip_parseWarningValue");

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

    /* If warning header is received having no value give decode failure for
     * the same*/
	if (0 == (pEnd - pCurrentPos + 1)) 
	{
		return SipFail;
	}
	else
	{
        /*Populate the warning header value in pStr1 of common header*/
		(*ppWarningHdr)->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pEnd - pCurrentPos + 1, pErr);
		if (SIP_NULL == (*ppWarningHdr)->pStr1)
		{
			return SipFail;
		}
	}

	SIPDEBUGFN("Exiting from sip_parseStringHeader");
	return SipSuccess;

} /* end of function sip_parseWarningValue */	
		
#endif

#ifdef SIP_RPR
/****************************************************************************
** FUNCTION: sip_parseRAckHeader
** DESCRIPTION: This function parses the Rack Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseRAckHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit			*pCurrentPos = pStart;
	SIP_S8bit			*pToken;
	SipRackHeader		*pRAckHdr;
	SIP_S8bit			*pTemp;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseRAckHeader");
	pRAckHdr = (SipRackHeader *)(pOutHdr->pHeader);

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Now pCurrentPos points to a non-LWS character */

	/* 
	 * Look for resp-num by tokenizing for SP 
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/* 
		 * Resp-num absent; raise parser error
		 */

		sip_error(SIP_Minor, "Syntax error in RAck hdr: resp num absent");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Validate and store the Resp Number part 
	 */

	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &pRAckHdr->dIntVar1))	
	{
		/*
		 * Non-numeric Resp Num found; raise parser error
		 */

		sip_error(SIP_Minor, "Syntax error in RAck hdr: non-numeric resp-num");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * Advance pCurrentPos
	 */
	
	pCurrentPos = pToken + 1;

	/* 
	 * Look for CSeq-num by tokenizing for SP.
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd); 
	
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/*
		 * CSeq-num not found; raise parser error.
		 */

		sip_error(SIP_Minor, "Syntax error in RAck hdr: Cseq num absent");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Validate and store the CSeq Number part 
	 */

	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &pRAckHdr->dIntVar2))	
	{
		/*
		 * Non-numeric CSeq Num found; raise parser error
		 */

		sip_error(SIP_Minor, "Syntax error in RAck hdr: non-numeric cseq num \
			found");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * Advance pCurrentPos
	 */

	pCurrentPos = pToken + 1;

	/*
	 * Look for Method
	 */
	
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pTemp = sip_stripTrailingLWS(pCurrentPos, pEnd);

	if (pCurrentPos > pTemp)
	{
		/*
		 * No Method present;
		 * Raise parser error.
		 */

		sip_error(SIP_Minor, "Syntax error in RAck hdr: method absent");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Method found. Store into header.
	 */

	pRAckHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pTemp - pCurrentPos + 1, pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pRAckHdr->pStr1)
	{
		return SipFail;
	}
#endif	
		
	SIPDEBUGFN("Exiting from sip_parseRAckHeader");
	return SipSuccess;

} /* end of function sip_parseRAckHeader */
#endif



/****************************************************************************
** FUNCTION: sip_parseAuthHeaders
** DESCRIPTION: This function parses the Authorization, Proxy-Authorization,
**				Authenticate and WWW-Authenticate Headers. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#if defined(SIP_AUTHENTICATE) || defined(SIP_3GPP)
SipBool	sip_parseAuthHeaders(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr,  en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit					*pCurrentPos = pStart;
	SIP_S8bit					*pToken;
	SipAuthorizationHeader		*pAuthHdr;
	SIP_S8bit					*pPrevToken;
	SIP_S8bit					*pTemp, *pSchemeEnd;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseAuthHeaders");
	pAuthHdr = (SipAuthorizationHeader *)(pOutHdr->pHeader);

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
#ifdef	SIP_CORE_STACK_COMPATIBLE
	/*
	 * If scheme is "Basic", store all the params in the 
	 * pName field of the first SipParam itself. This has been done 
	 * for compatibility with the way the core stack handles Basic 
	 * scheme.
	 */

	if ((0 == sip_strncmp(pCurrentPos, (SIP_S8bit *)"Basic", \
		sip_strlen((SIP_S8bit *)"Basic"))) && (*(pCurrentPos+5) == ' '))
	{
		SipParam 	*pTmpSipParam = SIP_NULL;

		pAuthHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, (SIP_S8bit *)"Basic", 5,\
			pErr);
		if (SIP_NULL == pAuthHdr->pStr1)
		{
			return SipFail;
		}
                /* Fix for SPR 19280 Starts */ 
                if(0 <= (pEnd - pCurrentPos - 5)) 
                { 
                /* Fix for SPR 19280 Ends */
                    if (SipFail == sip_initSipParam(&pTmpSipParam, pErr))
                        return SipFail;

                    pTmpSipParam->pName = sip_nStrdup(DECODE_MEM_ID, pCurrentPos + 6, \
                                          (pEnd - pCurrentPos - 5), pErr);
                    if (SIP_NULL == pTmpSipParam->pName)
                    {
                        sip_freeSipParam(pTmpSipParam);
                        return SipFail;
                    }

                    if (sip_listAppend(&(pAuthHdr->slParam), (SIP_Pvoid)pTmpSipParam, \
                        pErr) != SipSuccess)
                    {	
                        sip_freeSipParam (pTmpSipParam);
                        return SipFail ;
                    }

                    SIPDEBUGFN("Exiting from sip_parseAuthHeaders");
                    return SipSuccess;
                /* Fix for SPR 19280 Starts */ 
                } 
                else 
                    return SipFail;   
                /* Fix for SPR 19280 Ends */      
	}
#endif /* SIP_CORE_STACK_COMPATIBLE */

	/* Now pCurrentPos points to a non-LWS character */

	pPrevToken = sip_tokenize(pCurrentPos, pEnd, ',');
	
	pTemp = sip_tokenize(pCurrentPos, 
		(SIP_NULL == pPrevToken)?pEnd:pPrevToken - 1, '=');
	if (SIP_NULL == pTemp)
	{
		/*
		 * '=' is mandatory
		 */

#ifdef SIP_ERROR
#ifdef SIP_AUTHENTICATE		
		if (dType == SipHdrTypeAuthorization)
			sip_error(SIP_Minor, "Syntax error in Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyauthorization)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authenticate hdr: '='"
			" absent");
		else if (dType == SipHdrTypeWwwAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Www-Authenticate hdr: '='"
			" absent");

#endif
#ifdef SIP_3GPP
		if (dType == SipHdrTypePanInfo)
			sip_error(SIP_Minor, "Syntax error in P-Access-Network-Info hdr: '='"
			" absent");
		else if (dType == SipHdrTypeJoin)
			sip_error(SIP_Minor, "Syntax error in Join hdr: '='"
			" absent");

#endif
#endif

			
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/*
	 * Strip trailing LWS
	 */

	pTemp = sip_stripTrailingLWS(pCurrentPos, pTemp - 1);

	/*
	 * Tokenize for SP
	 */

	pToken = sip_tokenize(pCurrentPos, pTemp, ' ');
	if (SIP_NULL == pToken)
	{
		/*
		 * Scheme is not present;
		 * Raise parser error.
		 */

#ifdef SIP_ERROR
#ifdef SIP_AUTHENTICATE		
		if (dType == SipHdrTypeAuthorization)
			sip_error(SIP_Minor, "Syntax error in Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyauthorization)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authenticate hdr: '='"
			" absent");
		else if (dType == SipHdrTypeWwwAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Www-Authenticate hdr: '='"
			" absent");

#endif
#ifdef SIP_3GPP
		if (dType == SipHdrTypePanInfo)
			sip_error(SIP_Minor, "Syntax error in P-Access-Network-Info hdr: '='"
			" absent");
		else if (dType == SipHdrTypeJoin)
			sip_error(SIP_Minor, "Syntax error in Join hdr: '='"
			" absent");

#endif
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Store scheme after stripping trailing LWS.
	 */

	pSchemeEnd = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
	
	pAuthHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pSchemeEnd - pCurrentPos + 1, pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pAuthHdr->pStr1)
	{
		return SipFail;
	}
#endif	
	
	/*
	 * Invoke param parser
	 */

	if (SipFail == sip_parseSipParam(&(pAuthHdr->slParam), pToken + 1, pEnd, \
		SipTrue, ',', pErr))
	{
#ifdef SIP_ERROR
#ifdef SIP_AUTHENTICATE		
		if (dType == SipHdrTypeAuthorization)
			sip_error(SIP_Minor, "Syntax error in Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyauthorization)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authorization hdr: '='"
			" absent");
		else if (dType == SipHdrTypeProxyAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Proxy-Authenticate hdr: '='"
			" absent");
		else if (dType == SipHdrTypeWwwAuthenticate)
			sip_error(SIP_Minor, "Syntax error in Www-Authenticate hdr: '='"
			" absent");

#endif
#ifdef SIP_3GPP
		if (dType == SipHdrTypePanInfo)
			sip_error(SIP_Minor, "Syntax error in P-Access-Network-Info hdr: '='"
			" absent");
		else if (dType == SipHdrTypeJoin)
			sip_error(SIP_Minor, "Syntax error in Join hdr: '='"
			" absent");

#endif
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	SIPDEBUGFN("Exiting from sip_parseAuthHeaders");
	return SipSuccess;

} /* end of function sip_parseAuthHeaders */
			
		

#endif
/****************************************************************************
** FUNCTION: sip_parseRequestLine
** DESCRIPTION: This function parses the Request Line.
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppReqLine(OUT)	- A SipReqLine structure with the request
**								  line filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
		
SipBool	sip_parseRequestLine(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipReqLine *pReqLine, SipError *pErr)
{	
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_S8bit		*pTemp;

	SIPDEBUGFN("Entering into sip_parseRequestLine");

	/* 
	 * Skip leading LWS 
	 */

	/*pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);*/

	/* 
	 * Now pCurrentPos points to a non-LWS character 
	 */

	/*
	 * Look for Method
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/*
		 * Incomplete request line
		 */

		sip_error(SIP_Minor, "Incomplete Request Line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * Skip trailing LWS and store Method
	 */

	pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
	pReqLine->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pTemp - pCurrentPos + 1, pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pReqLine->pStr1)
	{
		return SipFail;
	}
#endif	

	pCurrentPos = pToken + 1;
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos,pEnd);

	/* 
	 * Look for Request-URI
	 */

	pTemp = sip_tokenize(pCurrentPos, pEnd, ' ');
	
	if (SIP_NULL == pTemp)
	{
		/*
		 * Incomplete request line
		 */

		sip_error(SIP_Minor, "Incomplete Request Line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
				
	/*
	 * Request-URI found; invoke addr-spec parser
	 */	
     /*
   * RFC 3261, Section 7.1 prohibits enclosing AddrSpec of ReqLine in AngBrkts.
   */
    if (*pCurrentPos == '<' && *(pTemp-1) == '>')
	{
	    sip_error(SIP_Minor, "Request Line enclosed in Angular Brkts!");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	if (SipFail == sip_parseAddrSpec(pCurrentPos, pTemp - 1, 
		&(pReqLine->pAddrSpec), pErr))
	{
		return SipFail;
	}
	pCurrentPos = pToken + 1;

	/* 	
	 * Process SIP Version field
	 */

	if (SipFail == sip_parseSipVersion(pTemp+1, pEnd, 
		&(pReqLine->pStr2), pErr))
	{
		/*
		 * Validation of SIP Version failed;
		 * raise parser error.
		 */

		return SipFail;
	}

	SIPDEBUGFN("Exiting from sip_parseRequestLine");
	return SipSuccess;

} /* end of function sip_parseRequestLine */



/****************************************************************************
** FUNCTION: sip_parseSipVersion
** DESCRIPTION: This function parses the SIP Version field.
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				ppVersion(OUT)	- The parsed SIP Version field.
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
		
SipBool	sip_parseSipVersion(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppVersion, SipError *pErr)
{

	SIPDEBUGFN("Entering into sip_parseSipVersion");

	/* Remove leading and trailing LWS before validating
	 * the version.
	 */

	pStart = sip_stripLeadingLWS(pStart, pEnd);
	pEnd = sip_stripTrailingLWS(pStart, pEnd);
	
	if ((*pEnd == '/') ||	\
		(0 != sip_strncasecmp(pStart, (SIP_S8bit *) "SIP/", \
		sip_strlen((SIP_S8bit *)"SIP/"))))
	{
		/*
		 * Unsupported/Unknown version
		 */

		sip_error(SIP_Minor, "Not a SIP version/Incomplete version string");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Store Version field
	 */

	*ppVersion = sip_nStrdup(DECODE_MEM_ID, pStart,
		(pEnd - pStart + 1), pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == *ppVersion)
	{
		return SipFail;
	}
#endif	
	
	SIPDEBUGFN("Exiting from sip_parseSipVersion");
	return SipSuccess;

} /* end of function sip_parseSipVersion */


	

/****************************************************************************
** FUNCTION: sip_parseStatusLine
** DESCRIPTION: This function parses the Status Line.
** 
** PARAMETERS:
**				pStart(IN)			- The starting point in the input buffer
**								  	  from where this parser can start 
**									  parsing. 
**				pEnd(IN)			- The position in the input buffer till 
**								  	  which this parser should parse.
**				ppStatusLine(OUT)	- A SipStatusLine structure with the 
**									  status line filled. 
**				pErr(OUT)			- Error value if any 
**
** NOTE: 
****************************************************************************/
		
SipBool	sip_parseStatusLine(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipStatusLine *pStatusLine, SipError *pErr)
{	
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_U32bit		tempCodeNum;

	SIPDEBUGFN("Entering into sip_parseStatusLine");

	/* 
	 * Skip leading LWS 
	 */

	/*pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);*/

	/* 
	 * Now pCurrentPos points to a non-LWS character 
	 */

	/*
	 * Look for Version
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/*
		 * Incomplete status line
		 */

		sip_error(SIP_Minor, "Incomplete status line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Validate version
	 */

	if (SipFail == sip_parseSipVersion(pCurrentPos, pToken - 1, \
		&(pStatusLine->pStr1), pErr))
	{
		return SipFail;
	}

	pCurrentPos = pToken + 1;

	/*
	 * Look for status-code
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		sip_error(SIP_Minor, "Status-code absent in Status Line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * Validate status-code
	 */
	
	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, \
		(SIP_U32bit *) &tempCodeNum))
	{
		sip_error(SIP_Minor, "non-numeric status code in status line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pStatusLine->dIntVar1 = (SIP_U16bit)tempCodeNum;

	if ((pStatusLine->dIntVar1 < 100) || (pStatusLine->dIntVar1 > 999))
	{
		/*
		 * status-code must be 3-DIGIT
		 */

		sip_error(SIP_Minor, "status code in status line must be 3-digit");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	pCurrentPos = pToken + 1;
	/*
	 * Store reason phrase, if present
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	if (pCurrentPos <= pEnd)
	{
		/*
		 * Reason-phrase present
		 */

		pStatusLine->pStr2 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pEnd - pCurrentPos + 1, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pStatusLine->pStr2)
		{
			return SipFail;
		}
#endif		
	}

	SIPDEBUGFN("Exiting from sip_parseStatusLine");
	return SipSuccess;

} /* end of function sip_parseStatusLine */


				

/****************************************************************************
** FUNCTION: sip_parseContentLengthHeader
** DESCRIPTION: This function parses the Content-Length, RSeq, Max-Forwards,
**				Min-SE and Session-Expires headers. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseContentLengthHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit			*pCurrentPos = pStart;
	SipContentLengthHeader	*pContentLenHdr;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseContentLengthHeader");
	pContentLenHdr = (SipContentLengthHeader *)(pOutHdr->pHeader);

	/* 
	 * Skip leading and trailing LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

#ifdef SIP_SESSIONTIMER
	/*
	 * This parser is used to parse Content-Length, RSeq, Max-Forwards,
	 * Min-SE and Session-Expires headers. Of these, the Min-SE and
	 * Session-Expires headers can have parameters in the header 
	 * body. Thus, we need to look for params for these two types of 
	 * headers.
	 */

	if ((dType == SipHdrTypeMinSE) || (dType == SipHdrTypeSessionExpires))
	{
		SIP_S8bit	*pToken = SIP_NULL;

		pToken = sip_tokenize(pCurrentPos, pEnd, ';');
		if (SIP_NULL != pToken)
		{
			if (SipFail == sip_parseSipParam(&(pContentLenHdr->slParam), \
				pToken + 1, pEnd, SipFalse, ';', pErr))
				return SipFail;
			pEnd = pToken - 1;
			pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
		}
	}
#endif /* SIP_SESSIONTIMER */

	if (pCurrentPos > pEnd)
	{
		/*
		 * Length field not present
		 */

#ifdef SIP_ERROR
		if (dType == SipHdrTypeContentLength)
			sip_error(SIP_Minor, "Syntax error in Content-Length hdr: "
			"length field not present");
#ifdef SIP_RPR
		else if (dType == SipHdrTypeRSeq)
			sip_error(SIP_Minor, "Syntax error in RSeq hdr: "
			"length field not present");
#endif
		else if (dType == SipHdrTypeMaxforwards)
			sip_error(SIP_Minor, "Syntax error in Max-Forwards hdr: "
			"length field not present");
#ifdef SIP_SESSIONTIMER
		else if (dType == SipHdrTypeMinSE)
			sip_error(SIP_Minor, "Syntax error in Min-SE hdr: "
			"length field not present");
		else if (dType == SipHdrTypeSessionExpires)
			sip_error(SIP_Minor, "Syntax error in Session-Expires hdr: "
			"length field not present");
#endif
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
		

	/* 
	 * Validate and store the length/resp-num field 
	 */

	if (SipFail == sip_atoi(pCurrentPos, pEnd, &pContentLenHdr->dIntVar1))	
	{
		/*
		 * Non-numeric Length field found; raise parser error
		 */

#ifdef SIP_ERROR
		if (dType == SipHdrTypeContentLength)
			sip_error(SIP_Minor, "Syntax error in Content-Length hdr: "
			"non-numeric length field");
#ifdef SIP_RPR
		else if (dType == SipHdrTypeRSeq)
			sip_error(SIP_Minor, "Syntax error in RSeq hdr: "
			"non-numeric length field");
#endif
		else if (dType == SipHdrTypeMaxforwards)
			sip_error(SIP_Minor, "Syntax error in Max-Forwards hdr: "
			"non-numeric length field");
#ifdef SIP_SESSIONTIMER
		else if (dType == SipHdrTypeMinSE)
			sip_error(SIP_Minor, "Syntax error in Min-SE hdr: "
			"non-numeric length field");
		else if (dType == SipHdrTypeSessionExpires)
			sip_error(SIP_Minor, "Syntax error in Session-Expires hdr: "
			"non-numeric length field");
#endif
#endif
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
		
	SIPDEBUGFN("Exiting from sip_parseContentLengthHeader");
	return SipSuccess;

} /* end of function sip_parseContentLengthHeader */



/****************************************************************************
** FUNCTION: sip_parseMimeVersionHeader
** DESCRIPTION: This function parses the Mime-Version header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseMimeVersionHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit				*pCurrentPos = pStart;
	SIP_S8bit				*pToken;
	SipMimeVersionHeader	*pMimeVersionHdr;
	SIP_U32bit				temp;
	SIP_S8bit				*pStartOfHdr;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseMimeVersionHeader");
	pMimeVersionHdr = (SipMimeVersionHeader *)(pOutHdr->pHeader);

	/* 
	 * Skip leading and trailing LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	if (pCurrentPos > pEnd)
	{
		/*
		 * Incomplete header 
		 */

		sip_error(SIP_Minor, "Incomplete MIME Version hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * This header is of the form "numeric-token.numeric-token"
	 * We validate the tokens but store the whole hdr as one
	 * string only.
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, '.');
	if (SIP_NULL == pToken)
	{
		sip_error(SIP_Minor, "Syntax error in MIME Version hdr: '.' not found");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}	
	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))
	{
		/*
		 * Non-numeric token in Version; raise error
		 */
		
		sip_error(SIP_Minor, "Non-numeric version field in MIME Version hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Store pCurrentPos
	 */
	pStartOfHdr = pCurrentPos;

	pCurrentPos = pToken + 1;
	if (pCurrentPos > pEnd)
	{
		/*
		 * Incomplete header
		 */

		sip_error(SIP_Minor, "Incomplete MIME Version hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	if (SipFail == sip_atoi(pCurrentPos, pEnd, &temp))
	{
		/*
		 * Non-numeric token in Version; raise error
		 */
		
		sip_error(SIP_Minor, "Non-numeric version field in MIME Version hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * Store the Mime Version header
	 */

	pMimeVersionHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pStartOfHdr, \
		pEnd - pStartOfHdr + 1, pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pMimeVersionHdr->pStr1)
	{
		return SipFail;
	}
#endif	

	SIPDEBUGFN("Exiting from sip_parseMimeVersionHeader");
	return SipSuccess;

} /* end of function sip_parseMimeVersionHeader */



/****************************************************************************
** FUNCTION: sip_parseViaHeader
** DESCRIPTION: This function parses the Via Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseViaHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos 			= pStart;
	SIP_S8bit		*pEndOfSendProtocol 	= SIP_NULL;
	SIP_S8bit		*pTmp				 	= SIP_NULL;
#if 0
    SIP_S8bit		*pTmpEnd				= SIP_NULL;
#endif    
	SIP_S8bit		*pSentProto			 	= SIP_NULL;
	SipViaHeader	*pViaHdr				= SIP_NULL;
	SIP_S8bit		*pSlash 				= SIP_NULL;
	SIP_S8bit		*pSlash2 				= SIP_NULL;
	
	(void)	dType;


	SIPDEBUGFN("Entering into sip_parseViaHeader");
	pViaHdr = (SipViaHeader *)(pOutHdr->pHeader);

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
#if 0
	pTmpEnd = pEnd;
	/*
	 * For handling via comments we can have a strategy like this.
	 * If Via Comments are there pEnd must be ) .
	 * So start parsing from the end.decrement the  pointer untill we 
	 * get bracketCount = 0.This decrement has to be done untill pCurrentPos
	 * because this will be the beginnig of Via Header.If you are not getting 
	 * bracketCount = 0 , by the time u reached pCurrentPos , this means
	 * an error in comments due to unbalance  ( and )
	 */
	if ( *pTmpEnd == ')')
	{
		/*
		 * Comming here means Via Comments are there.
		 * Now check for the validity of comments
		 */
		while (pTmpEnd >= pCurrentPos)
		{
			if (*pTmpEnd == ')')
			{
				/*
				 * Increment the bracket count
				 */
				bracketCount++;
			}
			else if (*pTmpEnd == '(')
			{
				/*
				 * decrement the bracket count
				 */
				bracketCount--;
				if(bracketCount == 0)
				{
					/*
					 * if bracket count is 0 
					 * we can exit from the  loop and store the comment
					 */
					pViaHdr->pComment = sip_nStrdup(DECODE_MEM_ID, pTmpEnd, \
					 		pEnd-pTmpEnd+1, pErr);
					if (pViaHdr->pComment == SIP_NULL)		
					{
						return SipFail;	
					}
					/*
					 * after storing  Via Comments  break fro the loop
					 */
					pTmp = pTmpEnd-1;
					break;
				
				}/* End of if BracketCount= 0 */
			}/* End of (*pTmpEnd == '(' )    Part */
			pTmpEnd--;
		}/*End of While loop */
	}/*End of if for pTmpEnd  = ) */
	else
	{
		pTmp = pEnd;
	}
#endif
	pTmp = pEnd;
    pSentProto = sip_tokenize(pCurrentPos, pTmp, ' ');
	if (SIP_NULL == pSentProto)
	{
 		*pErr = E_PARSER_ERROR;
		return SipFail ;
	}	
    
	/*begin SPR 10*/

	/*
	 * Validate the sent-protocol field. 
	 * Currently the only validation being done is checking that the
	 * sent-proto field has indeed 3 components as required.
	 */

	pSlash = sip_tokenize(pCurrentPos, pSentProto - 1, '/');
	if ((SIP_NULL == pSlash) || ((pSentProto-1) == pSlash) || \
		(*pCurrentPos == '/'))
	{
		sip_error(SIP_Minor, "Error in Via hdr: Incomplete sent-protocol");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pSlash2 = sip_tokenize(pSlash+1, pSentProto - 1, '/');
	if ((SIP_NULL == pSlash2) || ((pSentProto-1) == pSlash2))
	{
		sip_error(SIP_Minor, "Error in Via hdr: Incomplete sent-protocol");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	/*end SPR 10*/
    
	pEndOfSendProtocol = pSentProto + 1;		
	pViaHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pSentProto-pCurrentPos, pErr);
	/* begin SPR1_0_7 */
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pViaHdr->pStr1)
	{
		return SipFail;
	}
#endif	
	/* end SPR1_0_7 */
	pCurrentPos = sip_tokenize(pEndOfSendProtocol, pTmp-1, ';');
	if (SIP_NULL == pCurrentPos)
	{
		
		pEndOfSendProtocol = sip_stripLeadingLWS(pEndOfSendProtocol, pTmp);
		pTmp	= sip_stripTrailingLWS(pEndOfSendProtocol, pTmp);
		pViaHdr->pStr2 = sip_nStrdup(DECODE_MEM_ID, pEndOfSendProtocol, \
			(pTmp + 1 - pEndOfSendProtocol), pErr);
		/* begin SPR1_0_7 */
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pViaHdr->pStr2)
		{
			return SipFail ;
		}
#endif		
		/* end SPR1_0_7 */
	}
 	else
	{
		/*
		 * Store Send by.
		 * Check whether the next character after ';' is not a 
		 * space cr or lf. Store  the via params.
		 */

		/* Moving one char backward not to include ; */
		pCurrentPos--;
		pSentProto = pCurrentPos; 
		pCurrentPos = sip_stripTrailingLWS(pEndOfSendProtocol, pCurrentPos);
		pEndOfSendProtocol = sip_stripLeadingLWS(pEndOfSendProtocol, pCurrentPos);
		pViaHdr->pStr2 = sip_nStrdup(DECODE_MEM_ID, pEndOfSendProtocol, \
			pCurrentPos-pEndOfSendProtocol + 1, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pViaHdr->pStr2)
		{
			return SipFail ;
		}
#endif			
		pCurrentPos = pSentProto;
		pCurrentPos = pCurrentPos+2;	
		pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pTmp);
		if ((*pCurrentPos == '\r') || (*pCurrentPos == '\n'))
		{
			return SipFail ;
		}
	}
	if (SIP_NULL != pCurrentPos)
	{	
		if (SipFail == sip_parseSipParam(&(pViaHdr->slParam), \
			pCurrentPos, pTmp, SipFalse, ';', pErr))
		{
			return SipFail ;
		}
	}
	SIPDEBUGFN("Exiting from sip_parseViaHeader");
	return SipSuccess ;
} /* end of function sip_parseViaHeader */


/****************************************************************************
** FUNCTION: sip_parseContactHeader
** DESCRIPTION: This function parses the Contact  Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseContactHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr,  en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit *pCurrentPos				= SIP_NULL ;
	SipHeader *pSipHeader				= SIP_NULL ;
	SipContactHeader *pContactHeader 	= SIP_NULL ;
	SipBool retVal						= SipSuccess;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseContactHeader");
	pContactHeader = (SipContactHeader *)(pOutHdr->pHeader);
	pCurrentPos = sip_stripLeadingLWS(pStart, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	do
	{
		if (SipFail == sip_initSipHeader(&pSipHeader,SipHdrTypeFrom,pErr))
				return SipFail;
		if ((*pCurrentPos == '*') && (pCurrentPos == pEnd)) 
		{
			pContactHeader->dIntVar1 = SipContactWildCard;
			/*
			 * Set the dType pf the SipHeader structure to match that
			 * within the SipContactHeader structure just parsed.
			 */
			pOutHdr->dType = SipHdrTypeContactWildCard;
		}
		else
		{    
			retVal = sip_parseFromHeader(pCurrentPos, pEnd, 
				pSipHeader, SipHdrTypeContactNormal, pErr);
			if (retVal ==SipFail)	
			{
				break;
			}
			else
			{
				pContactHeader->pStr1 = \
					(((SipFromHeader*)(pSipHeader->pHeader))->pStr1);
				pContactHeader->pAddrSpec = ((SipFromHeader*)\
					(pSipHeader->pHeader))->pAddrSpec;
				pContactHeader->slParam = ((SipFromHeader*)(pSipHeader->pHeader))->slParam;
				pContactHeader->dIntVar1 = SipContactNormal;

				/*
				 * Set the dType pf the SipHeader structure to match that
				 * within the SipContactHeader structure just parsed.
				 */
				pOutHdr->dType = SipHdrTypeContactNormal;
			}
		} /* End of Else not Contact Wild */
	} while (0);

	if (!retVal)
	{
		sip_freeSipFromHeader((SipFromHeader *)(pSipHeader->pHeader));
		(void)fast_memfree(DECODE_MEM_ID, pSipHeader, pErr);
		return SipFail;
	}

	HSS_DELETEREF(((SipFromHeader *)(pSipHeader->pHeader))->dRefCount);
	(void)fast_memfree(DECODE_MEM_ID, pSipHeader->pHeader, pErr);
	(void)fast_memfree(DECODE_MEM_ID, pSipHeader, pErr);

	SIPDEBUGFN("Exiting from sip_parseContactHeader");
	return SipSuccess;

} /* end of function sip_parseContactHeader */


#ifdef SIP_REFER
/****************************************************************************
** FUNCTION: sip_parseReferToHeader
** DESCRIPTION: This function parses the Refer-To Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#if 0
SipBool	sip_parseReferToHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit			*pCurrentPos	= pStart;
	SipAddrSpec			*pAddrSpec	 	= SIP_NULL;
	SipReferToHeader	*pReferToHdr 	= SIP_NULL;

	SIPDEBUGFN("Entering into sip_parseReferToHeader");
	pReferToHdr = (SipReferToHeader *)(pOutHdr->pHeader);

	(void)	dType;

	/* 
	 * Strip leading and trailing LWS.
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	/*
	 * Refer-To header is nothing but an addr-spec;
	 * so just invoke the addr-spec parser here.
	 */
	
	if (SipFail == sip_parseAddrSpec(pCurrentPos, pEnd, &pAddrSpec, pErr))
		return SipFail;

	pReferToHdr->pAddrSpec = pAddrSpec;
	
	SIPDEBUGFN("Exiting from sip_parseReferToHeader");
	return SipSuccess;

} /* end of function sip_parseReferToHeader */
#endif
#endif




/****************************************************************************
** FUNCTION: sip_parseUnknownHeader
** DESCRIPTION: This function parses the Unknown Header. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseUnknownHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr,  en_HeaderType dType, SipError *pErr)
{
	SipUnknownHeader 	*pSipUnknownHeader 	= SIP_NULL ;
	SIP_S8bit			*pCurrentPos		= pStart;
	/*SIP_S8bit			*pToken				= SIP_NULL;*/

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseUnknownHeader");
	pSipUnknownHeader = (SipUnknownHeader *)(pOutHdr->pHeader);

	/*
	 * Skip leading and trailing LWS.
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	if (pCurrentPos >pEnd)
	{
			pSipUnknownHeader->pStr2 = SIP_NULL;
	
	}
	else
	{
		pSipUnknownHeader->pStr2 = \
			sip_nStrdup(DECODE_MEM_ID, pCurrentPos, 	pEnd - pCurrentPos+1,\
				pErr); 
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pSipUnknownHeader->pStr2)
		{
			return SipFail;
		}
#endif		

		SIPDEBUGFN("Exiting from sip_parseUnknownHeader");
	}	
	return SipSuccess;
} /* end of function sip_parseUnknownHeader */


/****************************************************************************
** FUNCTION: sip_parseRetryAfterHeader
** DESCRIPTION: This function parses the RetryAfterHeader. 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseRetryAfterHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
			SipHeader *pOutHdr,  en_HeaderType dType, SipError *pErr)
{

	SIP_S8bit		*pCurrentPos 			= pStart;
	SIP_S8bit		*pSecEnd			 	= SIP_NULL;
	SIP_S8bit		*pSecBegin			 	= SIP_NULL;
	SIP_S8bit		*pParamBegin		 	= SIP_NULL;
	SIP_S8bit		*pParamEnd			 	= SIP_NULL;
	SIP_S8bit		*pCommentToken		 	= SIP_NULL;
	SIP_S8bit		*pTmpComment		 	= SIP_NULL;
	SipRetryAfterHeader	*pRetryAfterHdr		= SIP_NULL;
	SIP_U32bit 		bracketCount			= 0;

	(void)	dType;


	SIPDEBUGFN("Entering into sip_RetryAfterHeader");
	pRetryAfterHdr = (SipRetryAfterHeader *)(pOutHdr->pHeader);

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	/*
	 * Beginning of retry After Seconds
	 */
	pSecBegin = pCurrentPos;

	/*
	 * The parsing of comments for retry After is different form
	 * Parsing Via Comments.The grammar for RetryAfter is given below.
	 * Retry-After = Retry-After HCOLON ( SIP-date | delta-seconds )
	 *    			   [ comment ] *( SEMI retry-param )
	 * So Comment if at all present will be present before parameters and
	 * there will be only  one comment.If parameters are present they will 
	 * found after the comment (if comment is there).So it will be better to 
	 * extract the comment first.Then parse the parameter.
	 */    			   
	pCommentToken = sip_tokenize(pCurrentPos, pEnd, '(');
	pTmpComment = pCommentToken;
	if (SIP_NULL != pCommentToken)
	{
		/*
		 * Coming here means comments are present
		 * Check for the validity of comments and parse them
		 */
        pRetryAfterHdr->pStr1 = SIP_NULL;
		while ( pCommentToken <= pEnd )
		{
			if ( *pCommentToken == '(' )
				bracketCount++;
			else if ( *pCommentToken == ')' )
				bracketCount--;
			if ((*pCommentToken == ')') && (bracketCount == 0))
			{
				/*
				 * Coming here means comment ended properly
				 * So exit from the loop.Now  we can store the comment.
				 */
                /* Klocworks: in case Retry After Header string is already populated 
                 * then free the previous allocation before assigning the new value
                 * this precautional code is requried as it is in while loop
                 */ 
                if(SIP_NULL != pRetryAfterHdr->pStr1)
                {
                    (void)fast_memfree(DECODE_MEM_ID,pRetryAfterHdr->pStr1,SIP_NULL);
                }

				pRetryAfterHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, \
						pTmpComment, pCommentToken - pTmpComment +1, pErr);
#ifndef SIP_NO_CHECK
				if (pRetryAfterHdr->pStr1 == SIP_NULL)		
				{
					return SipFail;	
				}
#endif				
				/*
				 * If a proper comment is present  the seconds/date will end just
				 * before  the beginning of comment
				 */
				pSecEnd = pTmpComment-1;

				/*
				 * Now parameters begin after 
				 * comment. To assign the start point of params we  must 
				 * check the range.
				 */
				if ((pCommentToken +1) <= pEnd)
				{	
					pParamBegin = sip_tokenize(pCommentToken+1, pEnd, ';');
					{
						if (pParamBegin != SIP_NULL)
						{
							pParamBegin++;
							pParamEnd = pEnd;
						}	
					}
					break;
				}
			}
			
			pCommentToken++;
		}/* End of while loop */

		if (bracketCount  != 0)
		{
			/* 
			 * Comming here means there is an error in 
			 * parsing comments.
			 * 
			 */
			sip_error(SIP_Minor,"Error in parsing comment\n");                  
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}	
	}	/*End of if pCommentToken != SIP_NULL */
	else
	{
		/*
		 * Now look for params.
		 */

		pSecEnd	  = sip_tokenize(pCurrentPos, pEnd, ';');
		if (pSecEnd == SIP_NULL)
		{
			pSecEnd = pEnd;
			/*
			 * If rety After Params are not present then make the pointers of
			 * params  null
			 */
			pParamEnd = pParamBegin = SIP_NULL;
		}
		else
		{
			/*
			 * if params are present assign the start and end 
			 */
			pParamBegin = pSecEnd + 1;	
			pParamEnd	=	pEnd;
			pSecEnd = pSecEnd - 1;
		}	
	}
	
	/*
	 * Strip trailing spaces for seconds
	 */ 

        if(SIP_NULL != pSecEnd)
	   pSecEnd = sip_stripTrailingLWS(pSecBegin, pSecEnd);

	/*
	 * Parse Date/delta-seconds field.
	 */

	if (pSecBegin > pSecEnd)
	{
		/*
		 * Empty header body not allowed.
		 */

		sip_error(SIP_Minor, "Syntax error in RetryAfter hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

#ifdef SIP_DATE
	/*
	 * decodeMessage(..) will have determined whether this is the Date or the
	 * delta-seconds format of the hdr. So check the dType and parse
	 * accordingly.
	 */

	if (SipHdrTypeRetryAfterDate == dType)
	{
		/* 
		 * first char is not a digit
		 */
		SipDateStruct	*pDateStruct = SIP_NULL;

		pRetryAfterHdr->dIntVar1 = SipExpDate;
		if ((SIP_NULL == pSecEnd) || 
				(SipFail == sip_parseSipDate(pSecBegin, pSecEnd, \
											 &pDateStruct, pErr)))
		{
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pRetryAfterHdr->u.pDate = pDateStruct;
	}
	else
	{
		/* 
		 * Validate and store the delta-seconds field 
		 */

		pRetryAfterHdr->dIntVar1 = SipExpSeconds;
		if (SipFail == sip_atoi(pSecBegin, pSecEnd, &(pRetryAfterHdr->u.dSec)))
		{
			/*
			 * Non-numeric Length field found; raise parser error.
			 */

			sip_error(SIP_Minor, \
				"Syntax error in RetryAfter Header: non-numeric delta-sec field");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}
#else
	if (SipFail == sip_atoi(pSecBegin, pSecEnd, &(pRetryAfterHdr->dIntVar1)))
	{
		/*
		 * Non-numeric Length field found; raise parser error.
		 */

		sip_error(SIP_Minor, \
			"Syntax error in RetryAfter Header: non-numeric delta-sec field");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
#endif /* SIP_DATE */

	/*
	 * Now parse params if previously found.
	 */

	if ((pParamEnd != SIP_NULL) &&(pParamBegin != SIP_NULL ))	
	{
	
		if (SipFail == sip_parseSipParam(&(pRetryAfterHdr->slParam), \
			pParamBegin, pParamEnd, SipFalse, ';', pErr))
			return SipFail;
	
	}	
	SIPDEBUGFN("Exiting From sip_RetryAfterHeader");
	return SipSuccess;

}/*End of Retry After Header */



/****************************************************************************
** FUNCTION: sip_parseExpiresHeader
** DESCRIPTION: This function parses the Expires header 
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseExpiresHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit			*pCurrentPos = pStart;
	SipExpiresHeader	*pExpiresHdr;
#ifdef SIP_DATE
	SipDateStruct		*pDateStruct = SIP_NULL;
#endif /* SIP_DATE */

	(void)	dType;
	SIPDEBUGFN("Entering into sip_parseExpiresHeader");
	pExpiresHdr = (SipExpiresHeader *)(pOutHdr->pHeader);

	/* 
	 * Skip leading and trailing LWS 
	 */

	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	if (pCurrentPos > pEnd)
	{
		/*
		 * Empty header body not allowed.
		 */

		sip_error(SIP_Minor, "Syntax error in Expires hdr");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	if	(SipHdrTypeMinExpires == dType)
	{
		if (SipFail == sip_atoi(pCurrentPos, pEnd, &(pExpiresHdr->dIntVar1)))
		{
			/*
			 * Non-numeric Length field found; raise parser error.
			 */

			sip_error(SIP_Minor, \
				"Syntax error in Expires Header: non-numeric delta-sec field");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}
	else
	{	
#ifdef SIP_DATE
	/*
	 * decodeMessage(..) will have determined whether this is the Date or the
	 * delta-seconds format of the hdr. So check the dType and parse
	 * accordingly.
	 */
		if ((SipHdrTypeExpiresAny == dType) || (SipHdrTypeExpiresSec == dType))
		{
				/*
				* Validate and store the delta-seconds field
				*/

				if (SipFail == sip_atoi(pCurrentPos, pEnd, &(pExpiresHdr->u.dSec)))
				{
				/*
				* Non-numeric Length field found; try for date format now.
				*/
				dType = SipHdrTypeExpiresDate;
				}
				else
				{
						pExpiresHdr->dIntVar1 = SipExpSeconds;
						dType = SipHdrTypeExpiresSec;
				}
		}
	

		if (SipHdrTypeExpiresDate == dType)
		{
				/* 
				* first char is not a digit
				*/

				pExpiresHdr->dIntVar1 = SipExpDate;
				if (SipFail == sip_parseSipDate(pCurrentPos, pEnd, \
				&pDateStruct, pErr))
				{
						*pErr = E_PARSER_ERROR;
						return SipFail;
				}
				pExpiresHdr->u.pDate = pDateStruct;
		}
#else
		if (SipFail == sip_atoi(pCurrentPos, pEnd, &(pExpiresHdr->dIntVar1)))
		{
			/*
			 * Non-numeric Length field found; raise parser error.
			 */

			sip_error(SIP_Minor, \
				"Syntax error in Expires Header: non-numeric delta-sec field");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
#endif /* SIP_DATE */
	}	/* end of if SipHdrTypeMinExpires	*/
	SIPDEBUGFN("Exiting from sip_parseExpiresHeader");
	return SipSuccess;

} /* end of function sip_parseExpiresHeader */
#ifdef SIP_3GPP
/****************************************************************************
** FUNCTION: sip_parse3GPPHeader
** DESCRIPTION: This function parses 3GPP Headers the PVisistedNetworkId, PanInfo, 
**              PcfAddr,PcVector,AcceptContact,RejectContact and Join. 
**				
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool	sip_parse3GPPHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SipCommonHeader	*p3GPPHdr;
	SIP_U32bit		size;
	en_SipBoolean	isContact=SipFalse;
	(void) dType;
	
	SIPDEBUGFN("Entering into sip_parse3GPPHeader");
	p3GPPHdr = (SipCommonHeader *) (pOutHdr->pHeader);

	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);

	/* Now pCurrentPos points to a non-LWS character */

	if(dType == SipHdrTypeAcceptContact || dType == SipHdrTypeRejectContact) 
	{
		if(*pCurrentPos == '*')
		{
			isContact = SipTrue ;
		}
		else
		{
			sip_error(SIP_Minor, "Syntax error in 3GPP hdr");
			return SipFail;
		}
	}	
	pToken = sip_tokenize(pCurrentPos, pEnd, ';');
	
	if (SIP_NULL == pToken)
	{
		if (pEnd > pCurrentPos)
		{
			if(isContact)
			{
				sip_error(SIP_Minor, "Syntax error in 3GPP hdr");
				return SipFail;
			}
			if((*pCurrentPos == '"')&&(*pEnd != '"'))
			{
				sip_error(SIP_Minor, "Syntax error in 3GPP hdr: \
						dangling Quote\" ");
				return SipFail;
			}
			p3GPPHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
					pEnd - pCurrentPos + 1, pErr);
			if (SIP_NULL == p3GPPHdr->pStr1)
			{
				return SipFail;
			}		
			pCurrentPos = pEnd;
		}
		else if (pCurrentPos == pEnd)
		{
			p3GPPHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
					pEnd - pCurrentPos + 1, pErr);
			pCurrentPos++;
		}
	}
	else
	{
		SIP_S8bit	*pTemp;
		if((isContact) && (*(pCurrentPos+1) != ';'))
		{
			sip_error(SIP_Minor, "Syntax error in 3GPP hdr");
			return SipFail;
		}
		pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
		size = pTemp - pCurrentPos + 1; 
		if (size > 0)
		{
			p3GPPHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, size,\
					pErr);
#ifndef SIP_NO_CHECK
			if (SIP_NULL == p3GPPHdr->pStr1)
			{
				return SipFail;
			}
#endif			
		}
		else
			p3GPPHdr->pStr1 = SIP_NULL;

		/*
		 * Advance pCurrentPos
		 */

		pCurrentPos=pToken + 1;	
	}

	if (SIP_NULL != pToken)
	{
		/*
		 * Params expected; call param parser.
		 */

		if (SipFail == sip_parseSipParam(&(p3GPPHdr->slParam), pCurrentPos,\
					pEnd, SipFalse, ';', pErr))
		{
			return SipFail;
		}
	}	

	SIPDEBUGFN("Exiting from sip_parse3GPPHeader");
	return SipSuccess;	
}
#endif
#ifdef SIP_CONGEST
/*******************************************************************************
** FUNCTION: sip_parseResourcePriorityHeader
** DESCRIPTION: This function parses Communications Resource Priority Headers -
**				Resource-Priority & Accept-Resource-Priority. 
**				
** 
** PARAMETERS:
**				pStart(IN)		- The starting point in the input buffer
**								  from where this parser can start parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse.
**				hdr(IN)			- The particular header for which this 
**								  parser is being invoked (required since
**								  multiple similar headers could be parsed
**								  by the same function).
**				pOutHdr(OUT)	- A SipHeader structure with the relevant
**								  headers filled. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool	sip_parseResourcePriorityHeader(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipHeader *pOutHdr, en_HeaderType dType, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_S8bit		*pTemp;
	SipRsrcPriorityHeader	*pResPriorityHdr;

	(void)	dType;

	SIPDEBUGFN("Entering into sip_parseResourcePriorityHeader");
	pResPriorityHdr = (SipRsrcPriorityHeader *) (pOutHdr->pHeader);
	
	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, pEnd);

	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos, pEnd);
	
	/* Tokenize for '.' */
	pToken = sip_tokenize(pCurrentPos, pEnd, '.');
	if (SIP_NULL == pToken)
	{
		sip_error(SIP_Minor, "Syntax error in Resource Priority headerdr: incomplete hdr"); 
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	pTemp = pToken - 1;
	
	pResPriorityHdr->pStr1 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pTemp - pCurrentPos + 1, pErr);
	
	
	pCurrentPos = pToken + 1;
	
	pTemp = sip_stripTrailingLWS(pCurrentPos, pEnd);
	
	if (pCurrentPos > pTemp)
	{
		/*
		 * No Priority value present;
		 * Raise parser error.
		 */
	
		sip_error(SIP_Minor, "Priority value not present");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	/* 
	 * Store priority value into header.
	 */
	
	pResPriorityHdr->pStr2 = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
	pTemp - pCurrentPos + 1, pErr);
	
	SIPDEBUGFN("Exiting from sip_parseResourcePriorityHeader");
		return SipSuccess;
	
} /* end of function sip_parseResourcePriorityHeader */
#endif


