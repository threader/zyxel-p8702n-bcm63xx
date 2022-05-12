#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 ** FUNCTION:		Top level and mid-level parser functionality
 **
 ****************************************************************************
 **
 ** FILENAME:		microsip_decode.c
 **
 ** DESCRIPTION:	This file contains the entry point into the stack for
 **					the application to parse a SIP message. The mid-level
 **					parser and body parsers are invoked from this function
 **					and are also coded in this file.
 **
 ** DATE	 		NAME			 REFERENCE	 		REASON
 ** ----	 		----			 ---------	 		--------
 ** 24-Feb-2002		Siddharth		 * SipLite			Initial					
 **					Toshniwal		   decodeMessage()
 **					Mahesh Govind	 * SIP Stack
 **									   decodeMessage()
 
 **
 ****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ****************************************************************************/

 
#include "microsip_portlayer.h"
#include "microsip_hdrparsers.h"
#include "microsip_timer.h"
#include "microsip_decode.h"
#include "microsip_decodeinternal.h"
#include "microsip_callbacks.h"
#include "microsip_accessors.h"
#include  "microsip_statistics.h"
#include "microsip_msgbodyparser.h"
#define UNKNOWNSIZE 1024
#define PRESENT 1	
#define ABSENT 0	

#ifdef SIP_THREAD_SAFE
extern synch_id_t	glbLockStatisticsMutex;
#endif

/* The following is the function pointer table of parsers */


SipBool (*glbSipParser[])(SIP_S8bit *pStart,  SIP_S8bit *pEnd, SipHeader \
	*pHeader, en_HeaderType dHdrType, SipError *pError) =
{
			sip_parseStringHeader, /* Allow Header parser*/
#ifdef SIP_IMPP			
			sip_parseStringHeader, /* Allow Events Header parser */
#endif		
#ifdef SIP_AUTHENTICATE
			sip_parseAuthHeaders, /* Authorization Header parser */
#endif			
			sip_parseStringHeader, /*  Call id Header Parser */
			sip_parseContactHeader, /* Contact HeaderParser */
			sip_parseContactHeader, /* Contact HeaderParser */ 
			sip_parseContactHeader, /* Contact HeaderParser */ 
			sip_parseContentDispositionHeader, /* ContentDisposition */
			sip_parseStringHeader, /* Content Encoding Parser */
			sip_parseContentLengthHeader, /* Content Length Parser */
			sip_parseContentTypeHeader, /* Content Type Parser */
			sip_parseCSeqHeader, /* Cseq Header Parser */
#ifdef SIP_IMPP			
			sip_parseContentDispositionHeader, /* Event Header Parser */
#endif			
#ifdef SIP_DATE
			sip_parseExpiresHeader, /* Expires Header Parser */
			sip_parseExpiresHeader, /* Expires Header Parser */
			sip_parseExpiresHeader, /* Expires Header Parser */
#else
			sip_parseExpiresHeader, /* Expires Header Parser */
#endif /* SIP_DATE */
			sip_parseAcceptHeader,	/* Accept Header Parser */ 
			sip_parseMinExpiresHeader,	/* MinExpires Header Parser */
			sip_parseFromHeader, /* From Header */
			sip_parseContentLengthHeader, /* Max-Forwards Parser */
			sip_parseMimeVersionHeader,/*Mime Version Header */
#ifdef SIP_PRIVACY
  	  sip_parsePrivacyHeader,					/*Privacy header*/	
		  sip_parsePPreferredIdHeader,  		  /* of P-Preferred-Identity Header */
			sip_parsePAssertIdHeader,     	/* of P-Asserted-Identity Header*/  
#endif				
#ifdef SIP_SESSIONTIMER
			sip_parseContentLengthHeader,	/* MinSE Parser */
#endif
#ifdef SIP_3GPP
			sip_parsePathHeader, /* Path Header */
			sip_parsePAssociatedUriHeader, /* P-Associated-URI header */
			sip_parsePCalledPartyIdHeader, /* P-Called-Party_ID header */
			sip_parsePVisitedNetworkIdHeader, /* P-Visited-Network-ID header */		
			sip_parsePcfAddrHeader, /* P-Charging-Function-Addresses */
		  sip_parsePanInfoHeaders, /* P-Access-Network-Info */
			sip_parsePcVectorHeader,  /* P-Charging-Vector */ 
			sip_parseServiceRouteHeader, /* Service-Route Header */
			sip_parseHistoryInfoHeader, /* HistoryInfo Header */
			sip_parseRequestDispositionHeader, /* Request Disposition Header */
			sip_parseAcceptContactHeader, /* AcceptContact Header */
			sip_parseRejectContactHeader, /* RejectContact Header */
			sip_parseJoinHeader, /* Join Header */
			sip_parseIfMatchHeader,  /* SIP-If-Match Header */
			sip_parseETagHeader, /* SIP-ETag Header */

#endif
#ifdef SIP_AUTHENTICATE
			sip_parseAuthHeaders, /* Proxy AuthenticateHeader Parser */
			sip_parseAuthHeaders, /* Proxy AuthorizationHeaderParser */
#endif			
#ifdef SIP_RPR
			sip_parseRAckHeader, /* Rack Header Parser */
#endif			
			sip_parseFromHeader, /* RecordRoute Header Parser */
#ifdef SIP_REFER			
			sip_parseFromHeader, /* Referred By header Parser */
			sip_parseFromHeader, /* ReferTo header Parser */
#endif			
#ifdef SIP_REPLACES
			sip_parseContentDispositionHeader, /* Replaces parser */
#endif
			sip_parseStringHeader, /* Require header Parser */
			sip_parseFromHeader, /* Route header Parser */
#ifdef SIP_RPR
			sip_parseContentLengthHeader, /* Rseq Header Parser */
#endif
#ifdef SIP_SECURITY
            sip_parseSecurityClientHeader, /*SecurityClient Header*/
            sip_parseSecurityVerifyHeader, /*SecurityVerify Header*/
            sip_parseSecurityServerHeader, /*SecurityServer Header*/
#endif             
#ifdef SIP_SESSIONTIMER
			sip_parseContentLengthHeader, /* Session-Expires Parser */
#endif /* SIP_SESSIONTIMER */
#ifdef SIP_IMPP
			sip_parseContentDispositionHeader,	/* Subscription-State Parser */
#endif /* SIP_IMPP */
			sip_parseStringHeader, /* Supported Header Parser */
#ifdef SIP_TIMESTAMP			
			sip_parseTimeStampHeader, /* time stamp Header Parser */
#endif			
			sip_parseFromHeader, /* To Header Parser */
			sip_parseStringHeader, /* Unsupported Header Parser */
			sip_parseViaHeader, /* Via Header Parser */
#ifdef SIP_WARNING			
			sip_parseWarningHeader,/* Warning Header Parser */
#endif			
#ifdef SIP_AUTHENTICATE			
			sip_parseAuthHeaders,/* Www-Authenticate Header Parser */
#endif			
			sip_parseUnknownHeader, /* Unknown Header Parser */
#ifdef SIP_DATE
			sip_parseRetryAfterHeader, /* RetryAfter Header Parser */
			sip_parseRetryAfterHeader, /* RetryAfter Header Parser */
			sip_parseRetryAfterHeader, /* RetryAfter Header Parser */
#else
			sip_parseRetryAfterHeader, /* RetryAfter Header Parser */
#endif /* SIP_DATE */
#ifdef SIP_PEMEDIA
			sip_parseStringHeader, /* P-Early-Media Parser */
#endif			
#ifdef SIP_CONGEST
			sip_parseResourcePriorityHeader, /* Resource-Priority Header parser */
			sip_parseResourcePriorityHeader /* Accept-resource-Priority Header parser */
#endif
};




/****************************************************************************
 ** FUNCTION :		sip_parseHeaderBody
 **
 ** DESCRIPTION:	This function parses the textual SIP header passed to
 **					it. The SipMessage data structure is filled up with
 **					the appropriate header as a part of this API. 
 **					sip_parseSipBuffer() internally invokes this API to 
 **					perform parsing.
 ** PARAMETERS:
 **	 		pStart(IN):		The beginning of the sipMessage to be parsed.
 **			pEnd(IN):   	The end of the SipMessage to be parsed
 **			dHdrType(IN):	The header Type to be parsed
 **			ppHeader(IN):	The SipHeader Structure
 **			pError(IN): 	The Error Variable
 **
 ****************************************************************************/
SipBool sip_parseHeaderBody( SIP_S8bit *pStart,  SIP_S8bit *pEnd,\
	en_HeaderType dHdrType, SipHeader *pHeader,  SipError *pError)
{
	/*  - Function entry exit debug */
#ifdef SIP_STATISTICS	
	INC_API_COUNT
#endif	

	/* Calling the appropriate function pointer */

	 if (SipFail == glbSipParser[(SIP_U32bit)dHdrType](pStart,pEnd,\
			 pHeader,  dHdrType, pError) )
		 
	 {
		sip_freeSipHeader(pHeader);
		(void)fast_memfree(DECODE_MEM_ID, pHeader, pError);
		return SipFail;
	 }
	return SipSuccess;
}/* End of function sip_parseHeaderBody */



/****************************************************************************
** FUNCTION :		sip_parseSipBuffer
**
** DESCRIPTION:	This function parses the textual SIP buffer passed to
**					it till a failure is reached or till the next message
**					is encountered. The SipMessage data structure filling
**					is done as a part of this API. sip_decodeMessage()
**					internally invokes this API to perform parsing.
**
**   PARAMETERS:
**			pMessage 		(IN)	:The buffer to be parsed
**			ppDecodedMsg	(OUT)	:The filled sipmessage structure
** 			pOpt			(IN)	:The runtime options for decoding
**			dMessageLength	(IN)	:The length of message buffer
** 			ppNextMsg		(OUT)	:The next message in the buffer
** 			pContext		(IN)	:The event Context
** 			SipMimeHeaders	(IN)	:to indicate whether this function is 
**									  called from message body parser 
** 			pErr			(IN/OUT): SipError
 
 ****************************************************************************/
#ifdef SIP_MIME_PARSING
SipBool sip_parseSipBuffer
	(  SIP_S8bit *pMessage, SipMessage **ppDecodedMsg, SipOptions *pOpt, \
	 SIP_U32bit dMessageLength, SIP_S8bit **ppNextMsg, \
	 SipEventContext *pContext, SipMimeHeader *pMimeHdrs, SipError *pErr)
#else
SipBool sip_parseSipBuffer
	(  SIP_S8bit *pMessage, SipMessage **ppDecodedMsg, SipOptions *pOpt, \
	 SIP_U32bit dMessageLength, SIP_S8bit **ppNextMsg, \
	 SipEventContext *pContext, SipError *pErr)
#endif
{


	SIP_S8bit *pCurrentPos 					= SIP_NULL ;
/*	SIP_S8bit *pTmpString 					= SIP_NULL ;*/
	SIP_S8bit *pBeginIndex 					= SIP_NULL ;
	SIP_S8bit *pEndIndex					= SIP_NULL ;
	SIP_S8bit *pBodyBeginIndex 				= SIP_NULL ;
	SIP_S8bit *pBodyEndIndex				= SIP_NULL ;
	SIP_S8bit *pMessageBodyBegin			= SIP_NULL ;
	SIP_S8bit *pMessageBodyEnd				= SIP_NULL ;
	SIP_S8bit *pNextLineBeginIndex			= SIP_NULL ;
	SIP_S8bit *pHeaderNameBeginIndex		= SIP_NULL;
	SIP_S8bit *pHeaderNameEndIndex			= SIP_NULL;
	SIP_S8bit *pName						= SIP_NULL;
	SIP_S8bit *pTrailingComma				= SIP_NULL;
	SIP_S8bit singleLineMessage 			= 0;
	SIP_U32bit lengthOfHeader				= 0;
	SipMessage *pSipMessage					= SIP_NULL ;
	en_StateofHeaderParsing headerParsingState;
	en_HeaderType 	dType 					= SipHdrTypeAny;
	en_SipBoolean isAuth					= SipFalse;
	en_SipBoolean exitCondition 		 	= SipFalse;
	en_SipBoolean ParseBody 			 	= SipTrue;
	SipHeaderOrderInfo *pSipHeaderOrderInfo = SIP_NULL;
	SIP_S8bit *pEnd 						= pMessage + dMessageLength - 1;
	SIP_U32bit dContentLength               = 0;
	en_SipBoolean hdrChkFlag 				= SipTrue;
	en_SipBoolean mimeHdrParsing			= SipFalse;
	SIP_U32bit quotesFound					= 0;
	SIP_U32bit angleFound					= 0;
	SIP_U32bit bracketFound					= 0;
	SIP_U32bit bracketCount					= 0;
	SIP_U32bit angleCount					= 0;
	SIP_U32bit trailingComma				= 0;
	en_SipBoolean wrongClass				= SipFalse;
	en_SipBoolean toBeappended				= SipTrue;
	en_SipBoolean orderAppended				= SipFalse;
        en_SipBoolean bDateFlag					= SipFalse;
	

#ifdef SIP_TRACE
	SIP_S8bit *tracetemp;
	SipError traceError;
#endif
#ifdef SIP_STATISTICS
	INC_API_COUNT
#endif	
#ifdef SIP_TRACE
   (void)sip_trace(SIP_Brief,SIP_Incoming,(SIP_S8bit *)\
               "Message received for decoding");
   tracetemp = (SIP_S8bit *) fast_memget(DECODE_MEM_ID,\
               (dMessageLength+2)*sizeof(SIP_S8bit),&traceError);
   if (tracetemp==SIP_NULL)
   {
          *(pErr)=E_NO_ERROR;
           return SipFail;
   }
   (void)sip_strncpy(tracetemp,pMessage,dMessageLength);
   tracetemp[dMessageLength] = '\0';
   (void)sip_trace(SIP_Brief,SIP_Incoming,tracetemp);
   (void)fast_memfree(DECODE_MEM_ID,tracetemp,SIP_NULL);
#endif
	/* Store the value of The begining of The SipMessage */
	pCurrentPos = pMessage;
	SIPDEBUGFN("Entering sip_parseSipBuffer function \n");
	/* Now ignore. initial CRLFs */
	if (pCurrentPos != SIP_NULL)
	{
		SIPDEBUGFN("Ignoring the leading CR or LF \n");
		while ((*pCurrentPos == '\r') || (*pCurrentPos == '\n') ||(*pCurrentPos
					==' ')||(*pCurrentPos =='\t'))
		{
			pCurrentPos++;
			if (pCurrentPos >= (pMessage+dMessageLength))
			{
                /*CSR_1-5644960 Change Start*/
				/* *pErr =  E_MAYBE_INCOMPLETE; */

                 *pErr = E_PARSER_ERROR;

                /*CSR_1-5644960 Change End*/
				return SipFail;
			}
		}	
	}
   /*
   	* Now Find the end of the first line and see 
	* whether any Line folds are there in the Start
	* Line.If there is a Line fold Raise Error
	*
	*/
   if (SIP_NULL == *ppDecodedMsg)
   {
		SIPDEBUGFN("Trying to find the end of first Non EmptyLine\n");
		
		/* pBeginIndex is the Begining of Request/StatusLine */
		pBeginIndex = pCurrentPos ;
		while ((*pCurrentPos != '\r') && (*(pCurrentPos) != '\n'))
		{
			
			pCurrentPos++;
			if (pCurrentPos >= (pMessage+dMessageLength))
			{
				*pErr =  E_MAYBE_INCOMPLETE;
				return SipFail;
			}
			
			/*
			 * Removal of Line folding in request /Status Line starts
			 * from here
			 */
			
			if (*pCurrentPos == '\r')
			{
				if ((*(pCurrentPos+1) == '\n') && 
					((*(pCurrentPos + 2) == ' ' )|| (*(pCurrentPos+2) == '\t')))
				{	
					*pCurrentPos = 	' ';
					*(pCurrentPos+1) = 	' ';
				}
				else if ((*(pCurrentPos+1)==' ') || (*(pCurrentPos+1) == '\t'))
				{
					*pCurrentPos = 	' ';
				}	
			}	
			if (*pCurrentPos == '\n')
			{
				if ((*(pCurrentPos+1)==' ') || (*(pCurrentPos+1) == '\t'))
				{
					*pCurrentPos = 	' ';
				}	
			}	
		}
		/* pEndIndex is the End of Request/StatusLine.
		 * and it can be \n or \r. */
		pEndIndex = pCurrentPos ;
		/* First remove the trailing CRLF after the Status/Request line(ifany)*/
		while(pCurrentPos < (pMessage+dMessageLength))
		{
			if((*pCurrentPos == '\n')||(*pCurrentPos == '\r'))
				if((*(pCurrentPos+1) != '\n') && (*(pCurrentPos+1) != '\r') &&
				   (*(pCurrentPos+1) != '\t') && (*(pCurrentPos+1) != ' '))
					   	break;
			pCurrentPos++;
		}
	   
		if((pCurrentPos == (pMessage+dMessageLength-1)) && 
		   ((*pCurrentPos == '\n')||(*pCurrentPos == '\r')))
		{
	
                        /*CSR_1-5512901 Change Start*/
                        /*	
                	if((pOpt->dOption & SIP_OPT_PARTIAL) != SIP_OPT_PARTIAL)
			{
				*pErr =  E_MAYBE_INCOMPLETE;
				return SipFail;
			}
			else
			{*/
                        /*CSR_1-5512901 Change End*/
				singleLineMessage = 1;
                        /*CSR_1-5512901 Change Start*/
			/*}*/
                        /*CSR_1-5512901 Change End*/
		}

		/* Here Finding the Exact beginning of next header 
		 * : The End Index/pCurrentPos  has to be incremented to the
		 *  beginnig of the next header. Make Both Same
		 *  INVITE sip:xyz@hss.hns.com SIP/2.0\r\n
		 *  It may be ended by a CR only
		 *  INVITE sip:xyz@hss.hns.com SIP/2.0\r
		 *  It may be ended by a LF only
		 *  INVITE sip:xyz@hss.hns.com SIP/2.0\n
		 *	So Increase the pointer accordingly  to bypass the CRLF\CR\LF
		 */
		
        if ((pCurrentPos+1) > (pMessage+dMessageLength))
		{
			*pErr =  E_MAYBE_INCOMPLETE;
			return SipFail;
		}
		/*
		 * Now pCurrentPos is pointing to the beginning of First header
		 * Now We have bypassed \r\n (OR \r OR \n)  of SIP/2.0\r\n 
		 * in the above example 
		 */
		/* Check for Line Folding.If Line Folding is there*
		 * Raise Error 
		 */
    
		SIPDEBUGFN("CHECKING for linefolding in Request/StatusLine\n");
		if ((*(pCurrentPos+1) == '\t') || (*(pCurrentPos+1) == ' '))
			return SipFail;

   }
	/*
	 * Check for *ppDecodedMsg being NULL before allocating memory for
	 * SipMessage, since this function is also called as part of MIME
	 * headers parsing, in which case SipMessage will already have been
	 * allocated.
	 */
	if (SIP_NULL == *ppDecodedMsg)
	{
		SIPDEBUGFN("CHECKING for SIP Request/Response \n");
		if (sip_strncasecmp(pBeginIndex,(SIP_S8bit*)"sip",3) == 0)
		/* Now Initialize response Type SipMessage  Structure */
		{
			if (SipFail == sip_initSipMessage (&pSipMessage , \
				SipMessageResponse,pErr))
				return SipFail;
			/* Now Call the StatusLine  Line parser */
			if (SipFail == sip_parseStatusLine(pBeginIndex,\
				pEndIndex-1, pSipMessage->u.pResponse->pStatusLine,\
					pErr)) 
				{	sip_freeSipMessage(pSipMessage);
					sip_error(SIP_Minor, "Parsing of StatusLine Failed");
					return SipFail ;
				}	
		}/* End of if Status line */
		else/* Else for Request Line */
		{
			/* Initialize a Request Type SipMessage */
			if (SipFail == sip_initSipMessage (&pSipMessage , \
				SipMessageRequest,pErr))
				return SipFail;
					/* Now Call Request LineParser */
            if (SipFail == sip_parseRequestLine(pBeginIndex,\
			pEndIndex-1,pSipMessage->u.pRequest->pRequestLine, pErr)) 
			{
				
				sip_freeSipMessage(pSipMessage);
				sip_error(SIP_Minor, "Parsing of RequestLine Failed");
				return SipFail ;
			}	
		}
	}
	else
	{
		mimeHdrParsing = SipTrue;
		/* Initialize a Request Type SipMessage */
		if (SipFail == sip_initSipMessage (&pSipMessage , \
			SipMessageRequest,pErr))
			return SipFail;
	}
	/*
	 * Both pCurrentPos and pEndIndex points to the begining of next
	 * header:refer line 222
	 */
	pEndIndex = pCurrentPos;	

	/* Message initializations done */
	/* If partial parsing is enabled and singleLineMessage is true
	   return success here */
	if(singleLineMessage == 1)
	{
                /*CSR_1-5512901 Change Start*/
		if((pOpt->dOption & SIP_OPT_PARTIAL) != SIP_OPT_PARTIAL)
		{
			sip_freeSipMessage(pSipMessage);
			*pErr =  E_MAYBE_INCOMPLETE;
			return SipFail;
		}
		else
		{
		/*CSR_1-5512901 Change End*/
	
                	*ppDecodedMsg = pSipMessage ;
    	        	return SipSuccess;

              	/*CSR_1-5512901 Change Start*/		
		}
		/*CSR_1-5512901 Change End*/
	}

	    /* 
	  * At this point pCurrentPos is pointing at the beginning
	  * of First header.Here we are assigning pCurrentPos 
	  * to pEndIndex
	  */
			/* Now Start Parsing headers.This is in a  do While Loop */
	headerParsingState = Sip_en_headerNameExpected ;
	do
	{
	 	SIPDEBUGFN("Stating to parse Headers\n");
		if (headerParsingState == Sip_en_headerNameExpected )
		{   
			/*resetting the variables */
			hdrChkFlag = SipTrue;
			pNextLineBeginIndex = SIP_NULL;
			pHeaderNameBeginIndex = pCurrentPos;
			
			/*Here trying to find the end of the header name */
            while (pCurrentPos <= (pMessage + dMessageLength))
			{
				/*
				 * If you find a ':' ,
				 * this means the end of the header name.
				 * so break
				 */ 
				if (*pCurrentPos == ':')
					break;
				/*
				 * If you find a line fold handle properly
				 */
				if (*pCurrentPos == '\r')
				{
					if ((*(pCurrentPos+1) == '\n') && 
						((*(pCurrentPos + 2) == ' ' )|| (*(pCurrentPos+2) == \
														 	'\t')))
					{	
						/*
						 * replace line folds with white spaces
						 */
						*pCurrentPos = 	' ';
						*(pCurrentPos+1) = 	' ';
					}
					else if ((*(pCurrentPos+1)==' ') || (*(pCurrentPos+1) == \
									'\t'))
					{
						/*
						 * replace line folds with white spaces
						 */
						*pCurrentPos = 	' ';
					}	
				}	
				if (*pCurrentPos == '\n')
				{
					if ((*(pCurrentPos+1)==' ') || (*(pCurrentPos+1) == '\t'))
					{
						/*
						 * replace line folds with white spaces
						 */
						*pCurrentPos = 	' ';
					}	
				}

				pCurrentPos++;	
			}/* End of While Loop*/	
			SIPDEBUGFN("INITIng Header Order Info \n");
			/* initialize the header order info */
			if (SipFail == sip_initSipHeaderOrderInfo \
					(&pSipHeaderOrderInfo,pErr))
			{
				sip_freeSipMessage(pSipMessage);
				return SipFail ;
			}	
            pHeaderNameEndIndex = pCurrentPos-1;
			pHeaderNameBeginIndex = sip_stripLeadingLWS\
				(pHeaderNameBeginIndex, pHeaderNameEndIndex);
			pHeaderNameEndIndex = sip_stripTrailingLWS\
				(pHeaderNameBeginIndex, pHeaderNameEndIndex);
			lengthOfHeader =\
				pHeaderNameEndIndex-pHeaderNameBeginIndex+1;
/*Fix from UATK 2.1 Start*/			
			pName = sip_nStrdup\
				(DECODE_MEM_ID,pHeaderNameBeginIndex, (SIP_S32bit)lengthOfHeader, pErr);
/*Fix from UATK 2.1 End*/
			if (SIP_NULL == pName)
			{
				sip_freeSipMessage(pSipMessage);
				__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
				return SipFail;
			}
#ifdef SIP_TRACE 				
        	(void)sip_trace(SIP_Brief,SIP_Incoming,\
				(SIP_S8bit*)"The name of the Header is");
        	(void)sip_trace(SIP_Brief,SIP_Incoming,pName);
#endif				
			/*	get the header Name and HeaderType  */
			SIPDEBUGFN("Getting Header Type\n");
			pSipHeaderOrderInfo->dTextType = SipFormFull;
			dType = sip_getHeaderTypeFromString(pName,\
					pName+lengthOfHeader+1,\
						&(pSipHeaderOrderInfo->dTextType));
               if((strcmp(pName,"Date") == 0))
                   bDateFlag = SipTrue;
			
			quotesFound = 0;
			bracketFound= 0;
			bracketCount= 0;
			angleCount	= 0;
			angleFound	= 0;
			toBeappended = SipTrue;
			orderAppended = SipFalse;
			wrongClass	= SipFalse;
			(void)fast_memfree(DECODE_MEM_ID,pName,pErr);
#ifdef SIP_AUTHENTICATE			
			if (dType ==SipHdrTypeProxyAuthenticate  ||
				dType == SipHdrTypeProxyauthorization ||
				dType == SipHdrTypeWwwAuthenticate ||
				dType == SipHdrTypeAuthorization)
			{
				isAuth = SipTrue ;
			}	
			else
#endif			
			{
				isAuth = SipFalse ;
			}	
			/* Assigning the dType to the LocalHeaderOrderInfo */				 
			pSipHeaderOrderInfo->dType 		= dType ;
			pSipHeaderOrderInfo->dNum		= 0 ;
			/*
			 * Now Changing the state to parsing  the header body
			 */		
			headerParsingState	= Sip_en_headerBodyExpected ;
			SIPDEBUGFN("CHANGED state to Header body parsing\n");
			/*
			 * if  Header Type is SipUnknown then we need
			 * to pass the name and value to the parser
			 */
			/* Bypassing COLON */
            pCurrentPos++ ; 
			pBodyBeginIndex 	= pCurrentPos;
             /* Second WorkAround Start */
            if(((*pBodyBeginIndex == ' ') && (*(pBodyBeginIndex+1) == '\0')) ||
               (*pBodyBeginIndex == '\0'))
			{
				if ((pOpt->dOption&(SIP_OPT_CLEN))!=(SIP_OPT_CLEN))
				{
					*ppDecodedMsg = pSipMessage ;	
					*pErr = E_INCOMPLETE;
                    if (orderAppended == SipFalse)
                    {
	                    if (toBeappended == SipTrue)
                        __sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
                    } 
				    return SipFail;
				}
            }
            /* Second WorkAround End */
			pBodyEndIndex	 	= pCurrentPos;
		}/* Actual Header Body  Parsing Starts here */
	 	if (headerParsingState == Sip_en_headerBodyExpected )
		{  
			if (hdrChkFlag)
			{
				if (SipFail == sip_checkEachHdrType(pSipMessage,dType,pErr))
				{	
				/*
				 * If checkEach HeaderType fails, we have to ignore this header
				 * this is not like the earlier case where we stop parsing of
				 * the SipMessage.For achieving this we can assume that this
				 * header will always succeed and  no need to parse this header
				 */	
					wrongClass = SipTrue;
					__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
				}	
				hdrChkFlag = SipFalse;	
			}	
            if (pBodyEndIndex >= (pMessage+dMessageLength))
			{
/*Fix from UATK 2.1 Start*/
/*				(void)fast_memfree(DECODE_MEM_ID, pTmpString,pErr);*/
/*				pTmpString = SIP_NULL ;*/
/*Fix from UATK 2.1 End*/
				sip_freeSipMessage(pSipMessage);
			 	__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
				return SipFail;
			}
            while (*pBodyEndIndex != ',' &&
					*pBodyEndIndex != '\r' &&
					*pBodyEndIndex != '\n' &&
					*pBodyEndIndex != '"' &&
					*pBodyEndIndex != '<' &&
					*pBodyEndIndex != '>' &&
					*pBodyEndIndex != '(' &&
					*pBodyEndIndex != ')' )
			{
				 pBodyEndIndex++;
				if (pBodyEndIndex >= (pMessage+dMessageLength))
				{
/*Fix from UATK 2.1 Start*/
/*					fast_memfree(DECODE_MEM_ID, pTmpString,pErr);*/
/*					pTmpString = SIP_NULL ;*/
/*Fix from UATK 2.1 End*/
					sip_freeSipMessage(pSipMessage);
					if (orderAppended == SipFalse)
				 		__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
					return SipFail;
		
				}
			}
			/*
			 * Given below is the checking for buffer overflow.
			 * This single checking is done to reduce code size.
			 * This is because if *pBodyEndIndex is " or ) or (
			 * we need to increment pBodyEndIndex by one to continue thte
			 * parsing.So we can do this checking at one palce instead of *
			 * doing
			 * at each place where we increment pBodyEndIndex for checking for 
			 * comments and qupoted string.
			 */
            if ( *pBodyEndIndex == '"' ||
				*pBodyEndIndex == '<' ||
				*pBodyEndIndex == '>' ||
				*pBodyEndIndex == ')' ||
				*pBodyEndIndex == '(' )
			{	
				/*
				 * Given below is the checking.
				 * If buffer over flow is there 
				 * retun fail by freeing the resources.
				 */
				if ((pBodyEndIndex+1) >= (pMessage+dMessageLength))
				{
/*Fix from UATK 2.1 Start*/
/*					fast_memfree(DECODE_MEM_ID, pTmpString,pErr);*/
/*					pTmpString = SIP_NULL ;*/
/*Fix from UATK 2.1 End*/
					sip_freeSipMessage(pSipMessage);
					if (orderAppended == SipFalse)
				 		__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
					return SipFail;
				}
			}
			if ( *pBodyEndIndex == ',' )
			{
				/*
				 * If youn have already found a quote or 
				 * bracket, if you are getting a , this is not a comma
				 * for separating headers, but a comma inside a comment or a
				 * quoted string.
				 * so continue.
				 */
				if ((quotesFound == 1) || (bracketFound == 1) || angleFound ==1)	
				{
					pBodyEndIndex++;
					continue;
				}	                
			}

            if ( *pBodyEndIndex == '"' )
			{
				if (bracketFound == 0)
				{
					/*
					 * If you are if the state of Bracket found = 1
					 * this means that  u are tyring to 
					 * find the  closing paranthesis.So ( """,")
					 * If bracketFound is one and it comes here
					 * it may be a case like above.
					 * so we nned to continue with out any processinf
					 */

				   /*
				 	* If you have already found a quote
				 	* and if again you are finding one again 
				 	* toggle the state
				 	*/
					if (quotesFound)
					{
						quotesFound = 0;
						pBodyEndIndex++;
						continue;
					}	
					/*
				 	 * If you find a quote then continue until next quote
				 	 */
					quotesFound = 1;
					pBodyEndIndex++;
					continue;
				}	
				/*
				 * If you are in the state of bracket found 
				 * continue
				 */
				pBodyEndIndex++;
				continue;
			}
            if ( *pBodyEndIndex == '(' )
			{
				if (quotesFound == 0)
				{
						
						/*
						 * Now you are in the state of bracketFound.
						 * Now we need to find the end of the comment.
						 * We will be 
						 * in the mission to find  the end of the comment.
						 * So we will be checking for a ).this is done by 
						 * makeing bracket count =0.At the moment 
						 * when we find that bracket count is zero this means
						 * the comment has ended.then we will give the 
						 * buffer to the Header parser.
						 */

						/*
						 * Now we set the state to bracket found
						 */
						bracketFound = 1;
						/*
						 * Increment the bracket count
						 */
						bracketCount++;
						/*
						 * now continue to  find the end of the comment
						 */
						  
						pBodyEndIndex++;
						continue;
				}
				/*
				 * If you are in quotes found state
				 * And getting a ) dont consider it.
				 * You need to ignore it can continue until the quotesFound
				 * becomes zero
				 * 
				 */
				pBodyEndIndex++;
				continue;
			}

            if (*pBodyEndIndex == ')')
			{
				if (quotesFound == 0)
				{
					/*
					 * If you find a ) decrement the bracket count
					 * If bracket count is zero.
					 * reset the state bracketFound to 0.This means
					 * we have reached
					 * the end of the comment
					 */
					bracketCount--;
					if (!bracketCount)
					{
						/*
						 * if baracket count is zero reset the state
						 */
						bracketFound = 0;
					}
				
					/*
					 * Any case you continue
					 */
					pBodyEndIndex++;
					continue;
				}
				/*
				 * If you are in quotes found state
				 * And getting a ( dont consider it.
				 * You need to ignore it can continue until the quotesFound
				 * becomes zero
				 * 
				 */
				pBodyEndIndex++;
				continue;
			}
			/*
			 * Here adding dupport for checking comma between < and > 
			 */
            if ( *pBodyEndIndex == '<' )
			{
				if (angleFound == 0)
				{
						
						/*
						 * Now you are in the state of angleFound.
						 * Now we need to find the end of the uri.
						 * We will be 
						 * in the mission to find  the end of URI.
						 * So we will be checking for a >.this is done by 
						 * makeing angle count =0.At the moment 
						 * when we find that angle count is zero this means
						 * the comment has ended.then we will give the 
						 * buffer to the Header parser.
						 */

						/*
						 * Now we set the state to bracket found
						 */
						angleFound = 1;
						/*
						 * Increment the bracket count
						 */
						angleCount++;
						/*
						 * now continue to  find the end of the URI
						 */
						  
						pBodyEndIndex++;
						continue;
				}
				/*
				 * If you are in quotes found state
				 * And getting a > dont consider it.
				 * You need to ignore it can continue until the quotesFound
				 * becomes zero
				 * 
				 */
				pBodyEndIndex++;
				continue;
			}

            if (*pBodyEndIndex == '>')
			{
				if (quotesFound == 0)
				{
					/*
					 * If you find a > decrement the angle count
					 * If angle count is zero.
					 * reset the state angleFound to 0.This means
					 * we have reached
					 * the end of the comment
					 */
					angleCount--;
					if (!angleCount)
					{
						/*
						 * if angle count is zero reset the state
						 */
						angleFound = 0;
					}
				
					/*
					 * Any case you continue
					 */
					pBodyEndIndex++;
					continue;
				}
				/*
				 * If you are in quotes found state
				 * And getting a > dont consider it.
				 * You need to ignore it can continue until the quotesFound
				 * becomes zero
				 * 
				 */
				pBodyEndIndex++;
				continue;
			}


			/*
			 *Support for <    ,   >  ends here.
			 *
			 */
			
			/*
			 * Here pCurrentPos is the position after the first
			 * occurance of CR\LF .To start the next\header
			 * line we need to increment  pCurrentPos.
			 */
			pCurrentPos=pBodyEndIndex ;
			if (pNextLineBeginIndex != SIP_NULL)		
			{
				pBodyBeginIndex = pNextLineBeginIndex ;
				
			}
#ifdef SIP_DATE
			/*
			 * If a retry after in Date format is there ignore the 
			 * comma and find the end of the header in Date format
			 */
            if ((dType == SipHdrTypeRetryAfterAny) && ((*pBodyEndIndex) == ','))
			{
				pBodyEndIndex++;	
				dType = SipHdrTypeRetryAfterDate;
				continue ;
			}
            if ((dType == SipHdrTypeExpiresAny) && ((*pBodyEndIndex) == ','))
			{
				pBodyEndIndex++;	
				dType = SipHdrTypeExpiresDate;
				continue ;
			}
#endif /* SIP_DATE */
            if ((bDateFlag ==  SipTrue)&& ((*pBodyEndIndex) == ','))
            {
                pBodyEndIndex++;
                continue ;
            }
			/*
			 * The following means the header is not an auth header
			 * but a comma separated one
			 */
            if ((isAuth == SipFalse)  && (*pBodyEndIndex) == ',')	
			{
				/* 
				 * Here First Comma separated header is parsed
				 * We must not forgot that a header will 
				 * always end a CR or LF.So the last Header in
				 * the comma separated series will be handled at
				 * the end
				 */
				 /*
				  *    header ,\r\n
				  */  
				pTrailingComma = pBodyEndIndex;
				 /*
				  * Checking various combinations in which trailing 
				  * commas can occur
				  *  (1) header ,,,,\r\n
				  *  (2) header,\r\n
				  *       	,,,\r\n
				  *  (3) header,,,,,header\r\n   	
				  */ 
				 if ((*(pTrailingComma+1) == '\r') &&( *(pTrailingComma+2)== \
							 '\n'))
				 {
					if ( (*(pTrailingComma+3) != '\t')&&\
						(*(pTrailingComma+3) != ' ') )
					{
						*pBodyEndIndex = ' ';
						*(pBodyEndIndex+1) = ' ';
						*(pBodyEndIndex+2) = ' ';
						pBodyEndIndex= pBodyEndIndex+2;
						headerParsingState = Sip_en_headerNameExpected;
						if (toBeappended == SipTrue)
						{	
							if (SipFail == 	sip_listAppend\
							(&(pSipMessage->slOrderInfo),(SIP_Pvoid)\
								pSipHeaderOrderInfo, pErr))
							{
								sip_freeSipMessage(pSipMessage);
								if (orderAppended == SipFalse)
									__sip_freeSipHeaderOrderInfo\
										(pSipHeaderOrderInfo);
								return SipFail ;
							}
							toBeappended = SipFalse;
							orderAppended = SipTrue;
						}
					
					}
				 }
				if (( *(pTrailingComma+1)==  '\n') || \
						( *(pTrailingComma+1)==  '\r'))
				 {
					if ( (*(pTrailingComma+2) != '\t')&&\
						(*(pTrailingComma+2) != ' ') )
					{
						*pBodyEndIndex = ' ';
						*(pBodyEndIndex+1) = ' ';
						pBodyEndIndex= pBodyEndIndex+1;
						headerParsingState = Sip_en_headerNameExpected;
						if (toBeappended == SipTrue)
						{	
							if (SipFail == 	sip_listAppend\
							(&(pSipMessage->slOrderInfo),(SIP_Pvoid)\
								pSipHeaderOrderInfo, pErr))
							{
								sip_freeSipMessage(pSipMessage);
								if (orderAppended == SipFalse)
								__sip_freeSipHeaderOrderInfo\
									(pSipHeaderOrderInfo);
								return SipFail ;
							}
							toBeappended = SipFalse;
							orderAppended = SipTrue;
						}
					
					}		
				 }	 
				 if(*(pTrailingComma+1) == ',')
				 {
					/*
					 * Setting the state to trailing comma present
					 */ 
					 
				 	trailingComma = PRESENT;
					if (trailingComma)
					{	
						/*
						 * If trailing comma is present remove the trailing 
						 * comma.Replace trailing comma with white space.
						 * This part of the code will solve problems like
						 * (1)header,,,,header\r\n
						 * (2)header,,,,\r\n
						 */ 
						while(*(pTrailingComma) == ',')
						{
							*pTrailingComma = ' ' ;
							pTrailingComma++;
						}
						pBodyEndIndex = pTrailingComma-1;
						while ((*pTrailingComma == ' ' )||\
							   (*pTrailingComma == '\t'))
							pTrailingComma++;
						if ((*pTrailingComma == '\r') ||\
						   	(*pTrailingComma == '\n'))
						{
							continue;
						}	

					}	
				 }/*End of 	if(*(pTrailingComma+1) == ',') */
					
						
				if (wrongClass)
				{
					pBodyEndIndex++;	
					pBodyBeginIndex = pBodyEndIndex;
					continue;
				}
				else
				{
#ifdef SIP_MIME_PARSING
					if (SipFail == sip_handleIncParser \
						 (pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
							pContext, dType, mimeHdrParsing, pMimeHdrs,\
							pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#else
					if (SipFail == sip_handleIncParser \
						 (pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
							pContext, dType,mimeHdrParsing, \
							pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#endif
					{
#ifdef SIP_BADMESSAGE_PARSING					
					   if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == \
						SIP_OPT_BADMESSAGE)
					   {
							if (SipFail == sip_fillBadHeader\
								(pHeaderNameBeginIndex,	pHeaderNameEndIndex,\
								pBodyBeginIndex, pBodyEndIndex\
								-1, pSipMessage,dType,pErr))
							{
								sip_freeSipMessage(pSipMessage);
								if (orderAppended == SipFalse)
								{
									if (toBeappended == SipTrue)
										__sip_freeSipHeaderOrderInfo\
										(pSipHeaderOrderInfo);
								}
								return SipFail;
							}
							pBodyEndIndex++;
							pBodyBeginIndex = pBodyEndIndex;
							continue;
						}
						else
						{
#endif						
							*pErr = E_PARSER_ERROR;
							sip_freeSipMessage(pSipMessage);
							if (orderAppended == SipFalse)
							{
								if (toBeappended == SipTrue)
								__sip_freeSipHeaderOrderInfo\
										(pSipHeaderOrderInfo);
							}
							return SipFail ;
#ifdef SIP_BADMESSAGE_PARSING								
						}
#endif							
					}
					else
					{
						/*
						 * if parsing Succeeds we have to increment
						 * the  number of headers in that line
						 */
                        pSipHeaderOrderInfo->dNum++;
						/*
						 * we need to by pass the comma
						 * 
						 */
						pBodyEndIndex++;
						/*
						 * Here we are ading the header order info to the
						 * list of header order info.
						 * Example Case:
						 * Via: SIP/2.0/TCP abc.com,another via
						 */ 
						if (toBeappended == SipTrue)
						{	
							if (SipFail == 	sip_listAppend\
							(&(pSipMessage->slOrderInfo),(SIP_Pvoid)\
								pSipHeaderOrderInfo, pErr))
							{
								sip_freeSipMessage(pSipMessage);
								if (orderAppended == SipFalse)
								__sip_freeSipHeaderOrderInfo\
									(pSipHeaderOrderInfo);
								return SipFail ;
							}
							toBeappended = SipFalse;
							orderAppended = SipTrue;
						}
						/*
						 * Assign pBodyBeginIndex to pBodyEndIndex
						 * to the beginning of next header.
						 */

						pBodyBeginIndex = pBodyEndIndex;
						continue;
					}
				}	/*End of else of wrong  class header*/
			}/*End of handling Comma separation */
            if (isAuth == SipTrue  && (*pBodyEndIndex) == ',')	
			{
				pBodyEndIndex++;	
				continue ;
			}	
			/*
			 * From here checking of Line fold starts 
			 * Check of CR with /without Line folding
			 */
            if (*pBodyEndIndex == '\r' )
			{
				/*
				 *Check for CRLF followed by SP/TAB. TBD 
				 *SP TAB whether to check strictly. 
				 */
				
				if ((*(pBodyEndIndex+1) == '\n') &&
					(((*(pBodyEndIndex+2) == ' ') || \
					  ( *(pBodyEndIndex+2) == '\t')) ) )
				{
					/*
					 * Replace Line folding with white space
					 */
					*pBodyEndIndex = ' ';
					*(pBodyEndIndex+1) = ' ';
					/*
					 * Incrementing pBodyEndIndex to 
					 * cross CR +LF
					 */
					pBodyEndIndex = pBodyEndIndex+2;
					continue;
				}
				/*
				 * Check whether CR followed by LF  followed by SPACE or tab
				 */
				if ( (*(pBodyEndIndex+1) == ' ') ||\
					( *(pBodyEndIndex+1) == '\t') ) 
				{
					/*
					 * Replace Line folding with white space
					 */
					*pBodyEndIndex = ' ';
					/*
					 * Incrementing pBodyEndIndex to 
					 * cross CR 
					 */
					pBodyEndIndex = pBodyEndIndex+1;
					continue;
				}
				
				/*
				 * If you are comming here means you need to parse the 
				 * header here.No Line folding is here
				 * Call handle Incremental parsing here
				 */
				if (wrongClass)
				{
					if ((*pBodyEndIndex == '\r') && \
						(*(pBodyEndIndex+1) == '\n'))
					{
						/*
					 	 *if CR followed by LF 
						 *increment pBodyEndIndex by 2
						 */
						pBodyEndIndex = pBodyEndIndex + 2;
						
						/*
						 * Now Header parsed we need to assign the begining of
						 * next header
						 */
						pCurrentPos = pBodyEndIndex;
					}
					else
					{
					
						/*
						 * Comming here means ended only with CR
						 * Increment by one.
						 */
						pBodyEndIndex++;
						/*
						 * Now Header parsed we need to assign the begining of
						 * next header
						 */
						pCurrentPos = pBodyEndIndex;
					}	

				}
				else/*End of handling Wrong Class here*/
				{	
#ifdef SIP_MIME_PARSING
					if (SipFail == sip_handleIncParser \
						(pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
						pContext, dType, mimeHdrParsing, pMimeHdrs, \
						pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#else
					if (SipFail == sip_handleIncParser \
						(pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
						pContext, dType, mimeHdrParsing, \
						pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#endif
					{
#ifdef SIP_BADMESSAGE_PARSING					
					   if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == \
						SIP_OPT_BADMESSAGE)
					   {
                            /* CSR_1-5203885 Change Start */
                            if (orderAppended == SipFalse)
                            {   
                                    if (toBeappended == SipTrue)
                                        __sip_freeSipHeaderOrderInfo\
                                        (pSipHeaderOrderInfo);  
                            }
                            /* CSR_1-5203885 Change End */
							if (SipFail == sip_fillBadHeader\
								(pHeaderNameBeginIndex,	pHeaderNameEndIndex,\
								pBodyBeginIndex, pBodyEndIndex-1, pSipMessage,\
								dType,pErr))
							{
								sip_freeSipMessage(pSipMessage);
                                /* CSR_1-5203885 Change Start */
							  /*if (orderAppended == SipFalse)
								{	
									if (toBeappended == SipTrue)
										__sip_freeSipHeaderOrderInfo\
										(pSipHeaderOrderInfo);
								}*/	
                                /* CSR_1-5203885 Change End */
								return SipFail;
							}
/*START: CSR 3776004*/

							/*pBodyEndIndex++;*/
							/*continue;*/

							
							if ((*pBodyEndIndex == '\r') && \
							(*(pBodyEndIndex+1) == '\n'))
						{
							/*
							 *if CR followed by LF 
							 *increment pBodyEndIndex by 2
							 */
							pBodyEndIndex = pBodyEndIndex + 2;
							
							/*
							 * Now Header parsed we need to assign the 
							 * begining of next header
							 */
							pCurrentPos = pBodyEndIndex;
						}
					if (((*pBodyEndIndex == '\r')&&( *(pBodyEndIndex+1) =='\n') )||\
								(*pBodyEndIndex == '\n')||\
								(*pBodyEndIndex == '\0')||\
								(*pBodyEndIndex == '\r') )
										{
										exitCondition = SipTrue ;
								  	    }
                    /*pBodyEndIndex++;
                      continue;*/
                    if ((*pBodyEndIndex == '\r') && \
                            (*(pBodyEndIndex+1) == '\n'))
                    {
                        pBodyEndIndex = pBodyEndIndex + 2;
                        pCurrentPos = pBodyEndIndex;
                    }
                    if (((*pBodyEndIndex == '\r')&&( *(pBodyEndIndex+1) =='\n') )||\
                            (*pBodyEndIndex == '\n')||\
                            (*pBodyEndIndex == '\r') )
                    {
                        exitCondition = SipTrue ;
                    }
                    headerParsingState = Sip_en_headerNameExpected;
                    continue;
                    /*END: CSR 3776004*/
                    /*CSR_1-4837503 ends*/
                       }
                       else
						{
#endif						
							*pErr = E_PARSER_ERROR;
							sip_freeSipMessage(pSipMessage);
							if (orderAppended == SipFalse)
								__sip_freeSipHeaderOrderInfo(pSipHeaderOrderInfo);
							return SipFail ;
#ifdef SIP_BADMESSAGE_PARSING								
						}
#endif							
					}
					else
					{
						/*
						 * if parsing Succeeds we have to increment
						 * the  number of headers in that line.
						 * Then insert SipHeaderOrderInfo to SipMessage.
						 */
                        pSipHeaderOrderInfo->dNum++;
						{	
							/*
							 * If once we have appended SipHeaderOrderInfo 
							 * then no need to append it again
							 */
							if (toBeappended == SipTrue)
							{	
								if (SipFail == 	sip_listAppend\
								(&(pSipMessage->slOrderInfo),(SIP_Pvoid)\
									pSipHeaderOrderInfo, pErr))
								{
									sip_freeSipMessage(pSipMessage);
									if (orderAppended == SipFalse)
									__sip_freeSipHeaderOrderInfo\
										(pSipHeaderOrderInfo);
									return SipFail ;
								}
								toBeappended = SipFalse;
							}
						}

						if ((*pBodyEndIndex == '\r') && \
							(*(pBodyEndIndex+1) == '\n'))
						{
							/*
							 *if CR followed by LF 
							 *increment pBodyEndIndex by 2
							 */
							pBodyEndIndex = pBodyEndIndex + 2;
							
							/*
							 * Now Header parsed we need to assign the 
							 * begining of next header
							 */
							pCurrentPos = pBodyEndIndex;
						}
						else
						{
						
							/*
							 * Comming here means ended only with CR
							 * Increment by one.
							 */
							pBodyEndIndex++;
							/*
							 * Now Header parsed we need to assign the
							 * begining of next header
							 */
							pCurrentPos = pBodyEndIndex;
						}	
					}/*If parsing succeeds*/
				}/*End of else of wrong class headers*/
			}/*End of CR */
			
			else if (*pBodyEndIndex == '\n')/*Else Check for LF*/
			{
				if ((*(pBodyEndIndex+1) == ' ') ||\
					*(pBodyEndIndex+1) == '\t')	
				{
					/*
					 * Replace Line folding with white space
					 */
					*pBodyEndIndex = ' ';
					/*
					 * Incrementing pBodyEndIndex to 
					 * cross LF
					 */
					pBodyEndIndex = pBodyEndIndex+1;
					continue;
				}
			
				/*
				 * If you are comming here means you need to parse the 
				 * header here.No Line folding is here
				 * Call handle Incremental parsing here.
				 */
				if (wrongClass)
				{
					/*
					 * Comming here means we need to increment pBodyEndIndex by
					 * one.
					 */
					pBodyEndIndex++;
					/*
					 * Starting point of next header is
					 * assigned to pCurrentPos
					 */
					pCurrentPos = pBodyEndIndex;
				
				}	
				else
				{
#ifdef SIP_MIME_PARSING
					if (SipFail == sip_handleIncParser \
						 (pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
							pContext,dType, mimeHdrParsing, pMimeHdrs, \
							pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#else
					if (SipFail == sip_handleIncParser \
						 (pBodyBeginIndex ,pBodyEndIndex-1,	pSipMessage,\
							pContext,dType, mimeHdrParsing,\
							pHeaderNameBeginIndex, pHeaderNameEndIndex, pErr) )
#endif
					{
#ifdef SIP_BADMESSAGE_PARSING					
					   if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == \
						SIP_OPT_BADMESSAGE)
					   {
                            /* CSR_1-5203885 Change Start */
                                if (orderAppended == SipFalse)
                                {   
                                    if (toBeappended == SipTrue)
                                    __sip_freeSipHeaderOrderInfo\
                                    (pSipHeaderOrderInfo);
                                }
                            /* CSR_1-5203885 Change End */  
							if (SipFail == sip_fillBadHeader\
								(pHeaderNameBeginIndex,	pHeaderNameEndIndex,\
									pBodyBeginIndex, pBodyEndIndex-1, \
									pSipMessage,dType,pErr))
							{
								sip_freeSipMessage(pSipMessage);
                                /* CSR_1-5203885 Change Start */
								/*if (orderAppended == SipFalse)
								{	
									if (toBeappended == SipTrue)
									__sip_freeSipHeaderOrderInfo\
									(pSipHeaderOrderInfo);
								}	*/
                                /* CSR_1-5203885 Change End */
								return SipFail;
							}
							/*
							 * After storing the bad header
							 * increment pBodyEndIndex by one.This is to
							 * bypass the \n.After that assign pCurrentPos =
							 * pBodyEndIndex.This is the starting of next header
							 */
#if 0
THis is the duplicate code this code is alredy written few lines above */
                            /*CSR_1-4837503 start*/
                            if (orderAppended == SipFalse)	
                            {
                                if (toBeappended == SipTrue)
                                    __sip_freeSipHeaderOrderInfo\
                                        (pSipHeaderOrderInfo);
                            }
#endif
                            /*CSR_1-4837503 ends*/
                            pBodyEndIndex++;
							pCurrentPos = pBodyEndIndex;
						}
						else
						{
#endif						
							*pErr = E_PARSER_ERROR;
							sip_freeSipMessage(pSipMessage);
							if (orderAppended == SipFalse)
							{	
								if (toBeappended == SipTrue)
									__sip_freeSipHeaderOrderInfo\
											(pSipHeaderOrderInfo);
							}	
							return SipFail ;
#ifdef SIP_BADMESSAGE_PARSING								
						}
#endif							
					}
					else
					{
					
						/*
						 * if parsing Succeeds we have to increment
						 * the  number of headers in that line.
						 * Then insert SipHeaderOrderInfo to SipMessage.
						 */
                        pSipHeaderOrderInfo->dNum++;
						{
							/*
							 * If once we have appended SipHeaderOrderInfo 
							 * then no need to append it again
							 */
							if (toBeappended == SipTrue)
							{	
								if (SipFail == 	sip_listAppend\
									(&(pSipMessage->slOrderInfo),(SIP_Pvoid)\
									pSipHeaderOrderInfo, pErr))
								{
									sip_freeSipMessage(pSipMessage);
									if (orderAppended == SipFalse)
									__sip_freeSipHeaderOrderInfo\
											(pSipHeaderOrderInfo);
									return SipFail ;
								}
								toBeappended = SipFalse;
							}	
						}
						/*
						 * Comming here means we need to increment 
						 * pBodyEndIndex by one.
						 */
						pBodyEndIndex++;
						/*
						 * Starting point of next header is
						 * assigned to pCurrentPos
						 */
						pCurrentPos = pBodyEndIndex;
					}
				}/*End of else of wrong class headers*/	
			}/*End of LF */
			
				/* Here Check for Exit Condition */	
            if (((*pBodyEndIndex == '\r')&&( *(pBodyEndIndex+1) =='\n') )||\
				(*pBodyEndIndex == '\n')||\
				(*pBodyEndIndex == '\0') || \
                (*pBodyEndIndex == '\r'))
			{
				exitCondition = SipTrue ;
			}
            /* CSR_1-5139236 WorkAround Start 
			else if(*pBodyEndIndex == '\0')
			{
            if ((pOpt->dOption&(SIP_OPT_CLEN))!=(SIP_OPT_CLEN))
            {
             *ppDecodedMsg = pSipMessage ;	
             *pErr = E_INCOMPLETE;
             return SipFail;
             }
             else
             {
             exitCondition = SipTrue ;
             }
             }
             CSR_1-5139236 WorkAround End */
            /*
			 * Now change the state to header name expected
			 */	
			headerParsingState = Sip_en_headerNameExpected;
		}/* End of Header body  parsing state */	
	}while (!exitCondition);

    if (((*pCurrentPos == '\r') && *(pCurrentPos+ 1) == '\n'))
	{
		pCurrentPos = pCurrentPos+2;	
	}		
	else if (*pCurrentPos == '\n')
	{
		pCurrentPos = pCurrentPos + 1;	
	}else if (*pCurrentPos == '\r')	
	{
		pCurrentPos = pCurrentPos + 1;	
	}	
	pMessageBodyBegin = pCurrentPos;	


    while((*pMessageBodyBegin == ' ' )||
		(*pMessageBodyBegin == '\t'))	
		/* Actual message body begining */
		pMessageBodyBegin++;
		/*
		 * Now Call The message Body parser
		 * Before Calling the message body parser
		 * check for the content Length .And Check for the
		 * incorrect Contentlength allowed Flag
		 */
		
	if (pSipMessage->pGeneralHdr->pContentLengthHdr == SIP_NULL)
	{
		/*
		 *If the message has no Content-Length header the message
		 *body is assumed to end at the end of the transport packet.
		 */
        if (pEnd < pMessageBodyBegin)	
		{
			/*begin SPR 11*/
			if (SIP_NULL == pSipMessage->pGeneralHdr->pContentTypeHdr)
			{
				ParseBody = SipFalse;
			}
			else
			{	
				sip_error(SIP_Minor, "Content-Type header is present but no"\
					" message body found");
				*pErr = E_MAYBE_INCOMPLETE;
				sip_freeSipMessage(pSipMessage);
				return SipFail;
			}
			/*end SPR 11*/
		}
		else if(pMessageBodyBegin < pEnd)
		{
			if (SIP_NULL == pSipMessage->pGeneralHdr->pContentTypeHdr)
			{
				ParseBody = SipFalse;
				pMessageBodyEnd = pMessageBodyBegin;	
	            if (SipTrue != mimeHdrParsing)
                {   
                *ppNextMsg = (SIP_S8bit *) fast_memget(DECODE_MEM_ID,\
     				pEnd - pMessageBodyEnd + 2, pErr);

				if(*ppNextMsg == SIP_NULL)
				return SipFail;
                
				strncpy(*ppNextMsg, pMessageBodyEnd,\
           				pEnd - pMessageBodyEnd +1);
				(*ppNextMsg)[pEnd - pMessageBodyEnd + 1] = '\0';
                }
				if(pContext != SIP_NULL)
    				pContext->dNextMessageLength = pEnd-pMessageBodyEnd + 1;
			}
			else
			{
				pMessageBodyEnd = pEnd;
			}
		}
		else
		{
			pMessageBodyEnd = pEnd;
		}	
	}
	else
	{
		 dContentLength = \
		 		pSipMessage->pGeneralHdr->pContentLengthHdr->dIntVar1;
		if (dContentLength == 0)		
		{
			ParseBody = SipFalse;		
#ifdef SIP_TRACE
			(void)sip_trace(SIP_Brief, SIP_Incoming ,\
				(SIP_S8bit *)"Length of message body specified by the Content"\
				"-Length hdr is 0; so message body" \
				"if any will not be parsed.");
#endif
            if(pMessageBodyBegin <  pEnd)
			{
				pMessageBodyEnd = pMessageBodyBegin;	
				*ppNextMsg = (SIP_S8bit *) fast_memget(DECODE_MEM_ID,\
						pEnd - pMessageBodyEnd + 2, pErr);
				if(*ppNextMsg == SIP_NULL)
					return SipFail;
				strncpy(*ppNextMsg, pMessageBodyEnd,\
						pEnd - pMessageBodyEnd + 1);
				(*ppNextMsg)[pEnd - pMessageBodyEnd + 1] = '\0';
				if(pContext != SIP_NULL)
					pContext->dNextMessageLength = pEnd-pMessageBodyEnd + 1;
			}
		}
	
		/*Please Refer Bis-05 section 19.3 Framing
		 * If the transport packet ends before the end 
		 * of the message body, this is considered an error.
		 */
		
		if (ParseBody)
		{
            if ((pMessageBodyBegin +  dContentLength - 1) > pEnd)
			{
			
				if ((pOpt->dOption&(SIP_OPT_CLEN))!=(SIP_OPT_CLEN))
				{
					/*
					 * If the transport packet ends before the end 
					 * of the message body, this is considered an error.
					 */
					ParseBody = SipFalse;
					sip_error(SIP_Minor, "Message buffer ends before the end"\
						"of message body as specified by Content-Length hdr");
					*pErr = E_INCOMPLETE;
/*START: CSR 3776004*/
                    /*ARICENT- below line added for clen>sdp*/
                    *ppDecodedMsg = pSipMessage;
                   	/*ARICENT-below line commented for clen>sdp*/

                    /*sip_freeSipMessage(pSipMessage);*/
/*END: CSR 3776004*/
					return SipFail;
				}	
				else
				{
					pMessageBodyEnd = pEnd;
					(void)sip_trace(SIP_Brief,SIP_Incoming,\
					(SIP_S8bit*)\
					"Content length is wrong and end of buffer is taken");
				}
			}
			else
			{	
				/*
				 * If there are additional bytes in the transport
				 * packet below the end of the body, they MUST be discarded.
				 */
		
				if  ((pMessageBodyBegin + dContentLength - 1) <=  pEnd)
				{
					pMessageBodyEnd = pMessageBodyBegin + dContentLength - 1;
					if (SipFalse == mimeHdrParsing)
					{
						if(pMessageBodyEnd == pEnd)
						{
							*ppNextMsg = NULL;
						}else
						{
							/*START: CSR 3776004*/
                            SipHeader    dTempViaHeader;
                            SIP_S8bit    *pTempSentProto =SIP_NULL;
                            if(sip_getHeaderAtIndex(pSipMessage, SipHdrTypeVia, \
			                		&dTempViaHeader, 0, pErr)==SipFail)
                            {
                                return SipFail;
                            }

                            pTempSentProto = (SIP_S8bit*)((SipCommonHeader *) \
                                             (dTempViaHeader.pHeader))->pStr1;
                            
                            /*CSR 1-5148417:added*/
                            sip_freeSipHeader(&dTempViaHeader);							

                            if (pTempSentProto == SIP_NULL)
                            {
                                *pErr = E_NO_EXIST;
                                sip_freeSipMessage(pSipMessage);
                                /*CSR 1-5148417:commented*/
                                /*sip_freeSipHeader(&dTempViaHeader);*/
                                return SipFail;
                            }

/*change for vxWork Compilation*/

                            if(sip_strcasecmp((SIP_S8bit *)pTempSentProto, (SIP_S8bit *)"SIP/2.0/UDP") == 0)
                            {
                                *ppNextMsg = NULL;						
/*CSR 1-4735601*/
								/*Below code is commented so as not to send 4xx in case clen< sdp which was part of CSR 3776004 fix*/	
								/*ParseBody = SipFalse;
								sip_error(SIP_Minor, "Message buffer more than"\
								"as specified by Content-Length hdr");
								*pErr = E_INCOMPLETE;
					
								*ppDecodedMsg = pSipMessage;   */
								/*sip_freeSipHeader(&dTempViaHeader);*/
 	
						        /*return SipFail;*/
/*CSR 1-4735601*/
                            }
							else
                            {

							    *ppNextMsg = (SIP_S8bit *) fast_memget(DECODE_MEM_ID,\
                         	    	pEnd - pMessageBodyEnd + 2, pErr);
              					if(*ppNextMsg == SIP_NULL)
                       					return SipFail;
              					strncpy(*ppNextMsg, pMessageBodyEnd +1,\
                           			pEnd - pMessageBodyEnd + 1);
          	    				(*ppNextMsg)[pEnd - pMessageBodyEnd + 1] = '\0';
          	    				if(pContext != SIP_NULL)
			    					pContext->dNextMessageLength =
			    						pEnd-pMessageBodyEnd + 1;
                            }
						}
					}	
				}	
		 	}
		} 
	}
	if (SipTrue == mimeHdrParsing)
	{
		ParseBody = SipFalse;
		*ppNextMsg = pMessageBodyBegin;
	}

	if (ParseBody)
	{
		/*TBD */
        if ((*pMessageBodyBegin == '\0' )&&\
			( pSipMessage->pGeneralHdr->pContentTypeHdr !=SIP_NULL))
		{
			sip_error(SIP_Minor,\
			"Content Type header present, But no message body part\n");
			sip_freeSipMessage(pSipMessage);
			return SipFail;
		}
		
		if ((pOpt->dOption & SIP_OPT_NOPARSEBODY) != SIP_OPT_NOPARSEBODY)
        {
			SIPDEBUGFN("Entering function : sip_parseMessageBody\n");
			if (SipFail == sip_parseMessageBody\
				(pMessageBodyBegin, pMessageBodyEnd, \
				pSipMessage, pContext, pOpt, pErr))
			{
#ifdef SIP_BADMESSAGE_PARSING		
				if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == SIP_OPT_BADMESSAGE)
				{
					*ppDecodedMsg = pSipMessage;
#ifdef SIP_STATISTICS			 
               /* INC_ERROR_PROTOCOL*/
#endif				
				}
				else
				{
#endif			
					sip_freeSipMessage(pSipMessage);
#ifdef SIP_BADMESSAGE_PARSING		
				}
#endif			
				return SipFail;
			}/*End of Invoking Message body parser */
		}
		else
		{
			SipMsgBody	*pMsgBody;

			if (SipFail == sip_initSipMsgBody(&pMsgBody, SipUnknownBody, \
				pErr))
			{
				return SipFail;
			}	
			if (SipFail == sip_initSipUnknownMessage(&(pMsgBody->u.\
				pUnknownMessage), pErr))
			{
				sip_freeSipMsgBody(pMsgBody);
				return SipFail;
			}
            pMsgBody->u.pUnknownMessage->dLength = pMessageBodyEnd - \
				pMessageBodyBegin + 1;
			pMsgBody->u.pUnknownMessage->pBuffer = (SIP_S8bit *) fast_memget(\
				DECODE_MEM_ID, pMsgBody->u.pUnknownMessage->dLength, pErr);
			if (SIP_NULL == pMsgBody->u.pUnknownMessage->pBuffer)
			{
				sip_freeSipMsgBody(pMsgBody);
				return SipFail;
			}
			memcpy(pMsgBody->u.pUnknownMessage->pBuffer, pMessageBodyBegin, \
				pMsgBody->u.pUnknownMessage->dLength);

			/*
			* Fill Message Body into the SipMessage
			*/

			if (SipSuccess != (sip_listAppend(&(pSipMessage->slMessageBody), \
				(SIP_Pvoid)pMsgBody, pErr)))
			{
				sip_freeSipMsgBody(pMsgBody);
				return SipFail;
			}	
		}
	}/*End of if (ParseBody)*/
	SIPDEBUGFN("Exiting from function : sip_parseSipBuffer\n");

	if (SipFalse == mimeHdrParsing)
		*ppDecodedMsg = pSipMessage ;
	else
	{
		sip_freeSipMessage(pSipMessage);
		
	}	
	return SipSuccess;
}


/****************************************************************************
 ** FUNCTION :		sip_decodeMessage
 **
 ** DESCRIPTION:	This function takes an input buffer and fills the sipmessage ** 				structure
 ** PARAMETERS:
 **	 		pMessaget(IN):		The beginning of the sipMessage to be parsed.
 **			ppDecodedMsg(OUT):	The filled SipMessage Strucutre
 **			pOpt(IN):   	Various options passed to SipMessage
 **			dMessageLength(IN):	The length of input Buffer
 **			pContext(IN):	The SipEventContext Structure
 **			pErr(IN): 	The Error Variable
 **
 ** Return Value:SipFail/SipSuccess
 ****************************************************************************/
#ifdef SIP_NO_CALLBACK
SipBool sip_decodeMessage (SIP_S8bit *pMessage, \
					SipMessage **ppDecodedMsg, SipOptions *pOpt, \
					SIP_U32bit dMessageLength, SIP_S8bit **ppNextMsg, \
					SipEventContext *pContext, SipError *pErr) 
#else
SipBool sip_decodeMessage (SIP_S8bit *pMessage, \
					SipOptions *pOpt, SIP_U32bit dMessageLength, \
					SIP_S8bit **ppNextMsg, SipEventContext *pContext, \
					SipError *pErr)
#endif

{
	SipMessage *pTempMessage = SIP_NULL;

	/*  - Function entry exit debug */

	/* 
	 * Error checks.
	 */
#ifdef SIP_STATISTICS	
	INC_API_COUNT
#endif	
	if (SIP_NULL == pErr)
	{
		return SipFail;
	}	

	if (SIP_NULL == pMessage)
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}

	*ppNextMsg = SIP_NULL;
	
	/*  - Increment statistics for number of messages recd for decoding */

	/*  - Trace the SIP message recd for decoding */

	/* 
	 * Invoke the function to parse and return one SipMessage.
	 */
#ifdef SIP_MIME_PARSING	
	if (sip_parseSipBuffer(pMessage, &pTempMessage, pOpt, \
		dMessageLength, ppNextMsg, pContext, SIP_NULL, pErr) == SipFail)
#else
	if (sip_parseSipBuffer(pMessage, &pTempMessage, pOpt, \
		dMessageLength, ppNextMsg, pContext, pErr) == SipFail)
#endif
	{
		/* 
		 *  - 	Handle bad message parsing and paranoid message length
		 * 			checks here
		 */
    	switch (*pErr)
        {
		   case E_INV_HEADER:
#ifdef SIP_STATISTICS
				INC_ERROR_PROTOCOL
#endif
                sip_freeSipMessage(pTempMessage);
				return SipFail;
           case E_INCOMPLETE:
#ifdef SIP_BADMESSAGE_PARSING
		   if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == SIP_OPT_BADMESSAGE)
                   {      
                 	*ppDecodedMsg = pTempMessage;
                 	break;
                   }
#endif
           case E_MAYBE_INCOMPLETE:
           case E_PARSER_ERROR:
			   /*ARICENT - below line added for clen>sdp*/
               *ppDecodedMsg = pTempMessage;
#ifdef SIP_BADMESSAGE_PARSING
		   if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == SIP_OPT_BADMESSAGE)
                   {      
                 	*ppDecodedMsg = pTempMessage;
                   }
		   
#endif
			   {
#ifdef SIP_STATISTICS
                      INC_ERROR_PROTOCOL
#endif
				}
            	break;
            case E_NO_MEM:
              INC_ERROR_MEM
             	break;
             default:;
         }
          return SipFail;
	}
	
	if (*ppNextMsg !=SIP_NULL)
	{
		SIP_U32bit length = 0;
		SIP_U32bit whitespaces = 0;
		SIP_U32bit dLength = 0;

		dLength = pContext->dNextMessageLength;
		while (length < dLength )
		{
			if ((isspace((unsigned)(int)((*ppNextMsg)[length])))||
					(*ppNextMsg)[length] == '\0') 
			{
				whitespaces++;
			}
			length++;
		}
		/* release *nextmesg if its only LWS */
		if (length == whitespaces)
		{
			/* Free the NextMsg buffer */
			(void)fast_memfree(DECODE_MEM_ID,*ppNextMsg,pErr);
			*ppNextMsg = SIP_NULL;
		}
	}
	
#ifndef SIP_TXN_LAYER
        if ((pOpt->dOption & SIP_OPT_NOTIMER) != SIP_OPT_NOTIMER)
        {
                if (sip_decodeInvokeTimerCallbacks(pTempMessage, pContext,pErr)\
					==SipFail)
                {
                        /* Mandatory header check failed for message */
#ifdef SIP_BADMESSAGE_PARSING
                        if ((pOpt->dOption & SIP_OPT_BADMESSAGE) == \
							SIP_OPT_BADMESSAGE)
                        {
                                /* If BADMESSAGE option is set we ignore this error */
                        }
                        else
#endif
                        {
                                sip_freeSipMessage(pTempMessage);
                                return SipFail;
                        }
                }
        }
#endif

        if (pTempMessage->dType == SipMessageResponse)
        {
#ifdef SIP_STATISTICS		
                INC_API_RESP_PARSED
#endif				
        }
        else
        {
#ifdef SIP_STATISTICS		
                INC_API_REQ_PARSED
#endif
        }

	/* 
	 * Invoke callbacks if in callback mode. 
	 *
	 * 
	 */

	/*  - Increment statistics  before issuing any callback */
#ifdef SIP_NO_CALLBACK
	*ppDecodedMsg = pTempMessage;
#else
	if (pTempMessage->dType == SipMessageRequest)
    {
		SIP_S8bit 	*pMethod = pTempMessage->u.pRequest->pRequestLine->pStr1;
		en_SipBoolean	dCallBackInvoked = SipFalse;
		
		switch (pMethod[0])
		{
			case 'I':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"INVITE")==0)
						{
							/* Call INVITE indicate */
							sip_indicateInvite(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						else if (sip_strcmp(pMethod,(SIP_S8bit*)"INFO")==0)
						{
							/* Call INFO indicate */
							sip_indicateInfo(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'A':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"ACK")==0)
						{
							/* Call ACK indicate */
							sip_indicateAck(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'B':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"BYE")==0)
						{
							/* Call BYE indicate */
							sip_indicateBye(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'R':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"REGISTER")==0)
						{
							/* Call REFER indicate */
							sip_indicateRegister(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						else if (sip_strcmp(pMethod,(SIP_S8bit*)"REFER")==0)
						{
							/* Call REGISTER indicate */
							sip_indicateRefer(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'O':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"OPTIONS")==0)
						{
							/* Call OPTIONS indicate */
							sip_indicateOptions(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'C':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"CANCEL")==0)
						{
							/* Call CANCEL indicate */
							sip_indicateCancel(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						/*
						else if (sip_strcmp(pMethod,(SIP_S8bit*)"COMET")==0)
						{
							sip_indicateComet(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						*/
						break;
			case 'P':
#ifdef SIP_RPR
						if (sip_strcmp(pMethod,(SIP_S8bit*)"PRACK")==0)
						{
							/* Call PRACK indicate */
							sip_indicatePrack(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
#endif
#ifdef SIP_3GPP
						else if (sip_strcmp(pMethod,(SIP_S8bit*)"PUBLISH")==0)
						{
							/* Call PUBLISH indicate */
							sip_indicatePublish(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
#endif
						break;
#ifdef SIP_IMPP
			case 'S':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"SUBSCRIBE")==0)
						{
							/* Call SUBSCRIBE indicate */
							sip_indicateSubscribe(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'N':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"NOTIFY")==0)
						{
							sip_indicateNotify(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
			case 'M':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"MESSAGE")==0)
						{
							/* Call MESSAGE indicate */
							sip_indicateMessage(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
#endif
			case 'U':
						if (sip_strcmp(pMethod,(SIP_S8bit*)"UPDATE")==0)
						{
							/* Call UPDATE indicate */
							sip_indicateUpdate(pTempMessage, pContext);
							dCallBackInvoked = SipTrue;
						}
						break;
		}	/* End of switch-case */

		if (dCallBackInvoked != SipTrue)
		{
			sip_indicateUnknownRequest(pTempMessage, pContext);
		}
	}
	else
#endif	
	if (pTempMessage->dType == SipMessageResponse)
	{
		SIP_U16bit codenum = pTempMessage->u.pResponse->pStatusLine->dIntVar1;

		if ((codenum>=100)&&(codenum<200))
		{
		{
#ifdef SIP_NO_CALLBACK
                        *ppDecodedMsg = pTempMessage;
#else
                        sip_indicateInformational(pTempMessage, pContext);
#endif
                }

		}
		else
		{
#ifdef SIP_NO_CALLBACK
                        *ppDecodedMsg = pTempMessage;
#else
                        sip_indicateFinalResponse(pTempMessage, pContext);
#endif	
		}
	}

	*pErr = E_NO_ERROR;
	return SipSuccess;
}




/****************************************************************************
** FUNCTION: sip_handleIncParser
** DESCRIPTION: This function calls the function sip_parseHeaderBody and
** 				also handles incremental parsing 
** 
** PARAMETERS:
**				ppSipMessage(OUT)- The SipMessage Structure
**				ppOutHdr(OUT)	- The SipHeader Structure
**				pStart(In)		- The  begin Index
**				pEnd(IN)		- The  end index
**				pContext(OUT)	- The Event Context Structure ,Which contains
** 								   the header List
**				en_HeaderType	- SipHeaderType	
**				mimeHdrParsing	- Flag indicating whether this function is being 
**								  for parsing MIME Headers.
**				pMimeHdrs		- The poiner to the MimeHeader structure.
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
#ifdef SIP_MIME_PARSING
SipBool sip_handleIncParser(SIP_S8bit *pStart,SIP_S8bit *pEnd, \
								SipMessage *pSipMessage,\
								SipEventContext *pContext,  \
								en_HeaderType dHdrType, \
								en_SipBoolean mimeHdrParsing, \
								SipMimeHeader *pMimeHdrs,
								SIP_S8bit* pHNameBegin,
								SIP_S8bit* pHNameEnd,
								SipError *pErr )
#else
SipBool sip_handleIncParser(SIP_S8bit *pStart,SIP_S8bit *pEnd, \
								SipMessage *pSipMessage,\
								SipEventContext *pContext,  \
								en_HeaderType dHdrType, \
								en_SipBoolean mimeHdrParsing, \
								SIP_S8bit* pHNameBegin,
								SIP_S8bit* pHNameEnd,
								SipError *pErr )
#endif


{
#ifdef SIP_TRACE
        SIP_S8bit *tracetemp = SIP_NULL;
#endif
	SipHeader *pHeader	=	SIP_NULL;
#ifdef SIP_STATISTICS	
		INC_API_COUNT
#endif		
		(void)mimeHdrParsing;

#ifdef SIP_TRACE
        (void)sip_trace(SIP_Brief,SIP_Incoming,(SIP_S8bit *)\
                "Header  received for decoding");
        tracetemp = sip_nStrdup(DECODE_MEM_ID,pStart,pEnd-pStart+1, pErr);
        if ((SipHdrTypeUnknown != dHdrType) && (pStart <= pEnd))
		{
			if (tracetemp==SIP_NULL)
			{
					*(pErr)=E_NO_ERROR;
					return SipFail;
			}
			(void)sip_trace(SIP_Brief,SIP_Incoming,tracetemp);
		}
		if (SIP_NULL != tracetemp)
			(void)fast_memfree(DECODE_MEM_ID,tracetemp,SIP_NULL);
#endif
	if (SipFail ==sip_initSipHeader(&pHeader,dHdrType,pErr))
		return SipFail ;

/* For Enabling incremental parsing, Please read the following arefully.
 * The SipEventContext structure is being passed to this function by 
 * application.The SipEventContext structure is having a member 
 * SipHdrTypeList *pList.For parsing a particular header at first instance
 * if Incremental parsing is enabled and the values corresponding header type is 
 *  set to SipSuccess , that header will be parsed completely at the first
 * instance.
 */
	
#ifndef SIP_INCREMENTAL_PARSING
	(void)pContext;
#endif
	switch(dHdrType)
	{
		/*General Headers*/
			case SipHdrTypeCallId:
			{
		
				if (pSipMessage->pGeneralHdr->pCallidHdr != SIP_NULL)
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one CALLID Header is present");
					goto error;
				}		
					/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart, pEnd, \
						dHdrType, pHeader, pErr))
				{
					(pSipMessage)->pGeneralHdr->pCallidHdr = \
						(SipCallIdHeader*)(pHeader->pHeader);
				}		
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of CallId Header Failed");
					return SipFail;	
				}	
				break;
		}
			case SipHdrTypeContactAny:			
			case SipHdrTypeContactWildCard:			
			case SipHdrTypeContactNormal:			
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
					/*Parse the Contact header and Append to the list*/
					if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slContactHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
					sip_error\
					(SIP_Minor, "parsing of COntact header Header Failed");
					 return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipContactHeader *pLocalContact = SIP_NULL;
				if (SipFail == sip_initSipContactHeader\
					(&pLocalContact,SipContactAny,pErr))
					return SipFail;
				pLocalContact->pBuffer = \
                sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1,pErr);
				if (pLocalContact->pBuffer == SIP_NULL)
				{
					return SipFail;
				}	
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slContactHdr,\
						(SIP_Pvoid)pLocalContact,pErr))
					return SipFail;
			}
#endif		
					break;
			}
			case SipHdrTypeCseq:
			{
				if (pSipMessage->pGeneralHdr->pCseqHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one CSeq Header is present");
					goto error;
					
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType,pHeader,pErr))
					(pSipMessage)->pGeneralHdr->pCseqHdr = \
					(SipCseqHeader*)(pHeader)->pHeader;
				else 
				{
				
					sip_error\
					(SIP_Minor, "parsing of CSeq  Header Failed");
					return SipFail;	
				}
					
					break ;
			}		
			case SipHdrTypeFrom:
			{
				if (pSipMessage->pGeneralHdr->pFromHeader != SIP_NULL)
				{
					
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one FROM Header is present");
					goto error;
				}	
				if (SipFail != sip_parseHeaderBody(pStart,pEnd, \
					dHdrType, pHeader,pErr))
					(pSipMessage)->pGeneralHdr->pFromHeader = \
					(SipFromHeader*)(pHeader)->pHeader;
				else 
				{
				sip_error\
				(SIP_Minor, "parsing of FROM Header Failed");
					return SipFail;	
				}	
			
        			break;
			}		
			case SipHdrTypeTo:
			{
				if (pSipMessage->pGeneralHdr->pToHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one TO Header is present");
					goto error;
				}	

				if (SipFail != sip_parseHeaderBody(pStart,\
					pEnd,dHdrType,pHeader,pErr))
					(pSipMessage)->pGeneralHdr->pToHdr = \
						(SipFromHeader*)(pHeader->pHeader);
		
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of TO Header Failed");
					 return SipFail;	
				 }

    			break;
			}				
			case SipHdrTypeRecordRoute:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
				/*get the size of the Via list, if size is greater than one that
				 * means Then if incremental par*/
				(void)sip_listSizeOf(&(pSipMessage)->pGeneralHdr->slRecordRouteHdr,\
					&size,pErr);
				if (size <=0)
				{
					/*This means this is the first Via Header and not to be
					 * parsed incrementally
					 */
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
										pHeader,pErr))
					{
						/*Parse the Via header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
				 				pGeneralHdr->slRecordRouteHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
					}
					else
				 		return SipFail;

			
				}
				else if (SipSuccess ==pContext->pList->enable[dHdrType])
				{
					{
						/*Parse this header completely;*/
						if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType, pHeader,pErr))
						{
							/*Parse the Viaheader and Append to the list*/
							if (SipFail == sip_listAppend(&(pSipMessage)->\
								pGeneralHdr->slRecordRouteHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
						}
						else 
						{
							return SipFail;	
						}	
					}
				}	
				else
				{
					SipRecordRouteHeader *pLocalHdr = SIP_NULL;
				
					if (SipFail == sip_initSipRecordRouteHeader(&pLocalHdr,\
						pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						pGeneralHdr->slRecordRouteHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
				/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
				/*Parse the Via header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slRecordRouteHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
				
					sip_error\
					(SIP_Minor, "parsing of Record-Route Header Failed");
					return SipFail;
				 	
				}
 /*End of Record Route Header parsing*/
#endif		
				break ;
		}
#ifdef SIP_3GPP
	case SipHdrTypePath:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
				/*get the size of the Path list, if size is greater than one that
				 * means Then if incremental par*/
				(void)sip_listSizeOf(&(pSipMessage)->pGeneralHdr->slPathHdr,\
					&size,pErr);
				if (size <=0)
				{
					/*This means this is the first Path Header and not to be
					 * parsed incrementally
					 */
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
										pHeader,pErr))
					{
						/*Parse the Path header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
				 				pGeneralHdr->slPathHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
					}
					else
				 		return SipFail;

			
				}
				else if (SipSuccess ==pContext->pList->enable[dHdrType])
				{
					{
						/*Parse this header completely;*/
						if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType, pHeader,pErr))
						{
							/*Parse the Path header and Append to the list*/
							if (SipFail == sip_listAppend(&(pSipMessage)->\
								pGeneralHdr->slPathHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
						}
						else 
						{
							return SipFail;	
						}	
					}
				}	
				else
				{
					SipPathHeader *pLocalHdr = SIP_NULL;
				
					if (SipFail == sip_initSipPathHeader(&pLocalHdr,\
						pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						pGeneralHdr->slPathHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
				/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
				/*Parse the Path header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slPathHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
				
					sip_error\
					(SIP_Minor, "parsing of PATH Header Failed");
					return SipFail;
				 	
				}
 /*End of path Header parsing*/
#endif		
				break ;
		}
		case SipHdrTypePAssociatedUri:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the PAssociatedUri header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slPAssociatedUriHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else
				{		
				sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of PAssociatedUri Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipPAssociatedUriHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipPAssociatedUriHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slPAssociatedUriHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
				break ;
		}

		case SipHdrTypePCalledPartyId:
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pPCalledPartyIdHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one P-Called-Party-ID Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pPCalledPartyIdHdr = \
				(SipPCalledPartyIdHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of P-Called-Party-ID Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipPCalledPartyIdHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
						pPCalledPartyIdHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pPCalledPartyIdHdr->\
				pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
			
#endif	
        			break;
			}		
		case SipHdrTypePVisitedNetworkId:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the PVisitedNetworkId header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slPVisitedNetworkIdHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of P-Visited-Network-ID Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipPVisitedNetworkIdHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipPVisitedNetworkIdHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slPVisitedNetworkIdHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
				break ;
		}
		
		case SipHdrTypePanInfo:
		{
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pPanInfoHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one P-Access-Network-Info Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pPanInfoHdr = \
					(SipPanInfoHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of PanInfo Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipPanInfoHeader\
					(&(pSipMessage)->pGeneralHdr->pPanInfoHdr, pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pPanInfoHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}
		case SipHdrTypePcfAddr:
		{
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pPcfAddrHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one P-Charging-Function-Addresses Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pPcfAddrHdr = \
					(SipPcfAddrHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of PcfAddr Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipPcfAddrHeader\
					(&(pSipMessage)->pGeneralHdr->pPcfAddrHdr, pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pPcfAddrHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}
		case SipHdrTypePcVector:
		{
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pPcVectorHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one P-Charging-Vector Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pPcVectorHdr = \
					(SipPcVectorHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of PcVector Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipPanInfoHeader\
					(&(pSipMessage)->pGeneralHdr->pPcVectorHdr, pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pPcVectorHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}
	
		case SipHdrTypeServiceRoute:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
				/*get the size of the ServiceRoute list, if size is greater than one that
				 * means Then if incremental part*/
				(void)sip_listSizeOf(&(pSipMessage)->u.pResponse->pResponseHdr->slServiceRouteHdr,\
					&size,pErr);                       
				if (size <=0)
				{
					/*This means this is the first ServiceRoute Header and not to be
					 * parsed incrementally
					 */
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
										pHeader,pErr))
					{
						/*Parse the ServiceRoute header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
				 					u.pResponse->pResponseHdr->slServiceRouteHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
					}
					else
				 		return SipFail;

			
				}
				else if (SipSuccess ==pContext->pList->enable[dHdrType])
				{
					{
						/*Parse this header completely;*/
						if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType, pHeader,pErr))
						{
							/*Parse the ServiceRouteheader and Append to the list*/
							if (SipFail == sip_listAppend(&(pSipMessage)->\
								u.pResponse->pResponseHdr->slServiceRouteHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
						}
						else 
						{
							return SipFail;	
						}	
					}
				}	
				else
				{
					SipServiceRouteHeader *pLocalHdr = SIP_NULL;
				
					if (SipFail == sip_initSipServiceRouteHeader(&pLocalHdr,\
						pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						u.pResponse->pResponseHdr->slServiceRouteHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
				/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
				/*Parse the ServiceRoute header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slServiceRouteHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
				
					sip_error\
					(SIP_Minor, "parsing of ServiceRoute Header Failed");
					return SipFail;
				 	
				}
 /*End of ServiceRoute Header parsing*/
#endif		
				break ;
		}
		case SipHdrTypeHistoryInfo:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
				/*get the size of the HistoryInfo list, if size is greater than one that
				 * means Then if incremental part*/
				(void)sip_listSizeOf(&(pSipMessage)->pGeneralHdr->slHistoryInfoHdr,\
					&size,pErr);
				if (size <=0)
				{
					/*This means this is the first HistoryInfo Header and not to be
					 * parsed incrementally
					 */
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
										pHeader,pErr))
					{
						/*Parse the HistoryInfo header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
				 				pGeneralHdr->slHistoryInfoHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
					}
					else
				 		return SipFail;

			
				}
				else if (SipSuccess ==pContext->pList->enable[dHdrType])
				{
					{
						/*Parse this header completely;*/
						if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType, pHeader,pErr))
						{
							/*Parse the HistoryInfo Header and Append to the list*/
							if (SipFail == sip_listAppend(&(pSipMessage)->\
								pGeneralHdr->slHistoryInfoHdr,pHeader->\
									pHeader,pErr))
							return SipFail;
						}
						else 
						{
							return SipFail;	
						}	
					}
				}	
				else
				{
					SipHistoryInfoHeader *pLocalHdr = SIP_NULL;
				
					if (SipFail == sip_initSipHistoryInfoHeader(&pLocalHdr,\
						pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						pGeneralHdr->slHistoryInfoHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
				/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
				/*Parse the HistoryInfo header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slHistoryInfoHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
				
					sip_error\
					(SIP_Minor, "parsing of HistoryInfo Header Failed");
					return SipFail;
				 	
				}
 /*End of HistoryInfo Header parsing*/
#endif		
				break ;
		}

		case SipHdrTypeRequestDisposition:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the RequestDisposition header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRequestDispositionHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of RequestDisposition Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipRequestDispositionHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipRequestDispositionHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRequestDispositionHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
				break ;
		}
		case SipHdrTypeAcceptContact:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the AcceptContact header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slAcceptContactHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of AcceptContact Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipAcceptContactHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipAcceptContactHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slAcceptContactHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
				break ;
		}

		case SipHdrTypeRejectContact:
			{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the RejectContact header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRejectContactHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of RejectContact Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipRejectContactHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipRejectContactHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRejectContactHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
				break ;
		}

		case SipHdrTypeJoin:
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pJoinHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Join Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pJoinHdr = \
				(SipJoinHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Join Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipJoinHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
						pJoinHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pJoinHdr->\
				pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
			
#endif	
        			break;
			}		

		case SipHdrTypeIfMatch:
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pIfMatchHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one SIP-If-Match Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pIfMatchHdr = \
				(SipIfMatchHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of SIP-If-Match Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipIfMatchHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
						pIfMatchHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pIfMatchHdr->\
				pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
			
#endif	
        			break;
			}		

		case SipHdrTypeETag:
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pResponse->pResponseHdr->pETagHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one SIP-ETag Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pResponse->pResponseHdr->pETagHdr = \
				(SipETagHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of SIP-ETag Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipETagHeader\
					(&(pSipMessage)->u.pResponse->pResponseHdr->\
						pETagHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pResponse->pResponseHdr->pETagHdr->\
				pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
			
#endif	
        			break;
			}		

#endif
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pTimeStampHdr != SIP_NULL)
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one TimeStamp Header is present");
					goto error;
				}		

					/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody\
					(pStart, pEnd, dHdrType, pHeader, pErr))
				(pSipMessage)->pGeneralHdr->pTimeStampHdr = \
					(SipTimeStampHeader*)(pHeader)->pHeader;
				else 
				{

					sip_error\
					(SIP_Minor, "parsing of TimeStampHeader Failed");
					  return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
			if (SipFail == sip_initSipTimeStampHeader\
					(&(pSipMessage)->pGeneralHdr->pTimeStampHdr, pErr))
						return SipFail;

			(pSipMessage)->pGeneralHdr->pTimeStampHdr->pBuffer = \
				sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif
        			break;
		}
#endif		
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterSec:
		case SipHdrTypeRetryAfterAny:
#else
		case SipHdrTypeRetryAfter:
#endif
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif	
				if (pSipMessage->pGeneralHdr->pRetryAfterHdr != SIP_NULL)
			{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one RetryAfter Header is present");
					goto error;
				}		

					/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody\
					(pStart, pEnd, dHdrType, pHeader, pErr))
				(pSipMessage)->pGeneralHdr->pRetryAfterHdr = \
					(SipRetryAfterHeader*)(pHeader)->pHeader;
				else 
				{

					sip_error\
					(SIP_Minor, "parsing of RetryAfter Failed");
					  return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
#ifdef SIP_DATE
				en_ExpiresType dExpType=SipExpAny;

				if (dHdrType == SipHdrTypeRetryAfterAny)
					dExpType = SipExpAny;
				else if (dHdrType == SipHdrTypeRetryAfterSec)
					dExpType = SipExpSeconds;
				else if (dHdrType == SipHdrTypeRetryAfterDate)
					dExpType = SipExpDate;

				if (SipFail == sip_initSipRetryAfterHeader\
					(&(pSipMessage)->pGeneralHdr->pRetryAfterHdr, dExpType, pErr))
					return SipFail;
#else
				if (SipFail == sip_initSipRetryAfterHeader\
					(&(pSipMessage)->pGeneralHdr->pRetryAfterHdr, pErr))
					return SipFail;
#endif /* SIP_DATE */

				(pSipMessage)->pGeneralHdr->pRetryAfterHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);
			}	
#endif
       		break;
		}	


		case SipHdrTypeVia:
		{
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
/*get the size of the Via list, if size is greater than one that means Then if incremental par*/
			(void)sip_listSizeOf(&(pSipMessage)->pGeneralHdr->slViaHdr,&size,pErr); 
			if (size <=0)
			{
			/*This means this is the first Via Header and not to be
			 * parsed incrementally
			 */
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				{
				/*Parse the Via header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slViaHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
					return SipFail;

			
			}
			else if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
				{
					/*Parse this header completely;*/
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
						dHdrType, pHeader,pErr))
					{
						/*Parse the Viaheader and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
							pGeneralHdr->slViaHdr,pHeader->pHeader,pErr))
						return SipFail;
					}
			
					else 
					{
						return SipFail;	
					}	
				}
				
			}	
			else
			{
				SipViaHeader *pLocalHdr = SIP_NULL;
				
				if (SipFail == sip_initSipViaHeader(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slViaHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
		/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
				/*Parse the Via header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slViaHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
				
					sip_error\
					(SIP_Minor, "parsing of Via Header Failed");
					return SipFail;
				 	
				}
 /*End of Via Header parsing*/
#endif		
				break ;
		}

		case SipHdrTypeContentLength:
		{
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pContentLengthHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one ContentLength Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pContentLengthHdr = \
				(SipContentLengthHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of Contentlength Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
				
			else
			{
				if (SipFail == sip_initSipContentLengthHeader\
					(&(pSipMessage)->pGeneralHdr->pContentLengthHdr\
						,pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pContentLengthHdr->\
					pBuffer = sip_nStrdup\
						(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
								 break;
		}

		case SipHdrTypeMaxforwards:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->u.pRequest->pRequestHdr->pMaxForwardsHdr \
					!= SIP_NULL)
				{
					sip_error (SIP_Major, \
						"SIP_ERROR: More than one Max-Forwards Header is present");
					goto error;
				}

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pMaxForwardsHdr = \
					(SipMaxForwardsHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of Max-Forwards Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipMaxForwardsHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->pMaxForwardsHdr, pErr))
					return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pMaxForwardsHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}

#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
		{
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pMinSEHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one MinSE Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pMinSEHdr = \
					(SipMinSEHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of MinSE Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipMinSEHeader\
					(&(pSipMessage)->pGeneralHdr->pMinSEHdr, pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pMinSEHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}

		case SipHdrTypeSessionExpires:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->pGeneralHdr->pSessionExpiresHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Session-Expires Header is present");
					goto error;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->pGeneralHdr->pSessionExpiresHdr = \
					(SipSessionExpiresHeader *)(pHeader)->pHeader;
				else 
				{
					sip_error\
						(SIP_Minor, "parsing of Session-Expires Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipSessionExpiresHeader\
					(&(pSipMessage)->pGeneralHdr->pSessionExpiresHdr, pErr))
						return SipFail;
				(pSipMessage)->pGeneralHdr->pSessionExpiresHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}

#endif /* SIP_SESSIONTIMER */

#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresDate:
#else
		case SipHdrTypeExpires:
#endif
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif	
				if (pSipMessage->pGeneralHdr->pExpiresHdr != SIP_NULL)
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one Expires Header is present");
					goto error;
				}		
			
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody\
					(pStart, pEnd, dHdrType, pHeader, pErr))
				(pSipMessage)->pGeneralHdr->pExpiresHdr = \
					(SipExpiresHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of Expires Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
#ifdef SIP_DATE
				en_ExpiresType dExpType=SipExpAny;

				if (dHdrType == SipHdrTypeExpiresAny)
					dExpType = SipExpAny;
				else if (dHdrType == SipHdrTypeExpiresSec)
					dExpType = SipExpSeconds;
				else if (dHdrType == SipHdrTypeExpiresDate)
					dExpType = SipExpDate;

				if (SipFail == sip_initSipExpiresHeader\
					(&(pSipMessage)->pGeneralHdr->pExpiresHdr, dExpType, pErr))
						return SipFail;
#else
				if (SipFail == sip_initSipExpiresHeader\
					(&(pSipMessage)->pGeneralHdr->pExpiresHdr, pErr))
						return SipFail;
#endif /* SIP_DATE */

				(pSipMessage)->pGeneralHdr->pExpiresHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}		
#endif	
								 break;
		}					 

		case SipHdrTypeMinExpires:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif	
				if (pSipMessage->u.pResponse->pResponseHdr->pMinExpiresHdr != SIP_NULL)
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one Min-Expires Header is present");
					goto error;
				}		
			
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody\
					(pStart, pEnd, dHdrType, pHeader, pErr))
				(pSipMessage)->u.pResponse->pResponseHdr->pMinExpiresHdr = \
					(SipMinExpiresHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of Min-Expires Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipMinExpiresHeader\
					(&(pSipMessage)->u.pResponse->pResponseHdr->pMinExpiresHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pResponse->pResponseHdr->pMinExpiresHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}		
#endif	
				break;
		}
		
		case SipHdrTypeContentEncoding:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				{
					/*Parse the ContentEncoding  header and Append to the list*/
					if (SipFail == sip_listAppend(&(pSipMessage)->\
							pGeneralHdr->slContentEncodingHdr,\
						(SIP_Pvoid)pHeader->pHeader,pErr))
						return SipFail;
					}
			
					else 
					{
						sip_error\
							(SIP_Minor, "parsing of ContentEncoding Header Failed");
						 return SipFail;	
					}	 
#ifdef SIP_INCREMENTAL_PARSING					 
				}
				
				else
				{
					SipContentEncodingHeader *pLocalHdr = SIP_NULL;
					if (SipFail == sip_initSipContentEncodingHeader\
						(&pLocalHdr,pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						pGeneralHdr->slContentEncodingHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
	
#endif	
        	break;
		}
        #ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				{
					/*Parse the PEarlyMedia  header and Append to the list*/
					if (SipFail == sip_listAppend(&(pSipMessage)->\
							pGeneralHdr->slPEarlyMediaHdr,\
						(SIP_Pvoid)pHeader->pHeader,pErr))
						return SipFail;
					}
			
					else 
					{
						sip_error\
							(SIP_Minor, "parsing of PEarlyMedia Header Failed");
						 return SipFail;	
					}	 
#ifdef SIP_INCREMENTAL_PARSING					 
				}
				
				else
				{
					SipPEarlyMediaHeader *pLocalHdr = SIP_NULL;
					if (SipFail == sip_initSipPEarlyMediaHeader\
						(&pLocalHdr,pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
					sip_freeSipHeader(pHeader);

					if (SipFail == sip_listAppend(&(pSipMessage)->\
						pGeneralHdr->slPEarlyMediaHdr,(SIP_Pvoid)pLocalHdr,\
							pErr))
					return SipFail;
				}
	
#endif	
        	break;
		}

#endif

		case SipHdrTypeContentType:
		{
#ifdef SIP_INCREMENTAL_PARSING
#ifdef SIP_MIME_PARSING
	if (SipTrue == mimeHdrParsing)
		pContext->pList->enable[dHdrType] = SipSuccess;
#endif		
	if (SipSuccess ==pContext->pList->enable[dHdrType])
		{
#endif		
		/*Parse this header completely;*/
#ifdef SIP_MIME_PARSING		
			if (((mimeHdrParsing == SipFalse) ? \
				((SIP_Pvoid)(pSipMessage->pGeneralHdr->pContentTypeHdr)):\
					((SIP_Pvoid)(pMimeHdrs->pContentType))) \
				!= SIP_NULL)
#else
			if ((SIP_Pvoid)(pSipMessage->pGeneralHdr->pContentTypeHdr)!= SIP_NULL)
#endif
			{
				sip_error (SIP_Major,\
				"SIP_ERROR: More than one ContentType header is present");
					goto error;
			}		
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType, pHeader,\
				pErr))
			{
#ifdef SIP_MIME_PARSING			
				if (SipFalse == mimeHdrParsing)
#endif				
					(pSipMessage)->pGeneralHdr->pContentTypeHdr = \
						(SipContentTypeHeader*)(pHeader)->pHeader;
#ifdef SIP_MIME_PARSING			
				else
					pMimeHdrs->pContentType = \
						(SipContentTypeHeader*)(pHeader)->pHeader;
#endif						
			}
			else 
			{
				sip_error (SIP_Major,\
				"SIP_ERROR: Parsing of ContentType Header Failed");
				 return SipFail;	
			}	 
#ifdef SIP_INCREMENTAL_PARSING				 
		}
		else if (SipTrue != mimeHdrParsing)
		{
			if (SipFail == sip_initSipContentTypeHeader\
					(&(pSipMessage)->pGeneralHdr->pContentTypeHdr\
						,pErr))
			return SipFail;			

			pSipMessage->pGeneralHdr->pContentTypeHdr->pBuffer = \
				sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
			sip_freeSipHeader(pHeader);
		}		
#endif	
	        			break;
		}			

			case SipHdrTypeAccept:			
			{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
				{
					/*Parse the Accept header and Append to the list*/
					if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slAcceptHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
					sip_error\
					(SIP_Minor, "parsing of Accept header Header Failed");
					 return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipAcceptHeader *pLocalAccept = SIP_NULL;
				if (SipFail == sip_initSipAcceptHeader\
					(&pLocalAccept,pErr))
					return SipFail;
				pLocalAccept->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1,pErr);
				if (pLocalAccept->pBuffer == SIP_NULL)
				{
					return SipFail;
				}	
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slAcceptHdr,\
						(SIP_Pvoid)pLocalAccept,pErr))
					return SipFail;
			}
#endif		
					break;
			}
			
		case SipHdrTypeUnknown:
		{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipTrue == mimeHdrParsing)
				pContext->pList->enable[dHdrType] = SipSuccess;
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
										pHeader,pErr))
				{
					((SipUnknownHeader*)(pHeader->pHeader))->pStr1=\
						sip_nStrdup(DECODE_MEM_ID, pHNameBegin, \
						pHNameEnd-pHNameBegin+1, pErr);
				
#ifdef SIP_MIME_PARSING				
					if (SipFalse == mimeHdrParsing)
#endif					
					{
						/*Parse the unknown header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
							pGeneralHdr->slUnknownHdr,pHeader->pHeader,pErr))
							{
								return SipFail;
							}	
					}
#ifdef SIP_MIME_PARSING				
					else
					{
						SipUnknownHeader *pTmpUnknownHdr = \
							(SipUnknownHeader *)(pHeader->pHeader);

						if (0 == sip_strncmp(pTmpUnknownHdr->pStr1,\
							(SIP_S8bit*) "Content-Transfer-Encoding", \
							sip_strlen((SIP_S8bit*)"Content-Transfer-Encoding")))
							pMimeHdrs->pContentTransEncoding = \
								((SipUnknownHeader *)(pHeader->pHeader))->\
									pStr2;
						else if (0 == sip_strncmp(pTmpUnknownHdr->pStr1,\
							(SIP_S8bit*) "Content-Id", \
							sip_strlen((SIP_S8bit*)"Content-Id")))
							pMimeHdrs->pContentId = \
								((SipUnknownHeader *)(pHeader->pHeader))->\
									pStr2;
						else if (0 == sip_strncmp(pTmpUnknownHdr->pStr1,\
							(SIP_S8bit*)"Content-Description", \
							sip_strlen((SIP_S8bit*)"Content-Description")))
							pMimeHdrs->pContentDescription = \
								((SipUnknownHeader *)(pHeader->pHeader))->pStr2;
						else
							(void)fast_memfree(DECODE_MEM_ID, \
								((SipUnknownHeader *)(pHeader->pHeader))->\
								pStr2, pErr);

						    (void)fast_memfree(DECODE_MEM_ID, \
								((SipUnknownHeader *)(pHeader->pHeader))->\
									pStr1, pErr);
						    (void)fast_memfree(DECODE_MEM_ID, \
							    (SipUnknownHeader *)(pHeader->pHeader), pErr);
					}
#endif					
				}
				else 
				{
					sip_error (SIP_Major,\
						"SIP_ERROR: Parsing of Unknown Header Failed");
								return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else if (SipTrue != mimeHdrParsing)
			{
				SipUnknownHeader *pLocalHdr = SIP_NULL;
				SIP_S8bit *pUheader= SIP_NULL;
				SIP_U32bit length = 0;

				length = (pHNameEnd-pHNameBegin)+(pEnd-pStart)+4;
				pUheader =(SIP_S8bit*) fast_memget(DECODE_MEM_ID, length, \
					pErr);
				if (pUheader ==SIP_NULL)
				{
					sip_freeSipHeader(pHeader);
					return SipFail;
				}
			
#ifdef SIP_LINT
                if( pHNameBegin !=SIP_NULL)
#endif
                memcpy(&pUheader[0],pHNameBegin, pHNameEnd-pHNameBegin+1 );
				memcpy((pUheader+((pHNameEnd-pHNameBegin)+1)), ":",1);
#ifdef SIP_LINT
                if( pStart !=SIP_NULL)
#endif                
				memcpy((pUheader+((pHNameEnd-pHNameBegin)+2)), pStart,\
						(pEnd-pStart+1));
				pUheader[length-1] = '\0'; 
				
				if (SipFail == sip_initSipUnknownHeader(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = pUheader;
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slUnknownHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif
			break;
		}

		case SipHdrTypeRequire:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
									pHeader,pErr))
				{
				/*Parse the Require header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slRequireHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Require Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipRequireHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipRequireHeader(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
			sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slRequireHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
	
#endif	
        			break;
		}
#ifdef SIP_RPR
		case SipHdrTypeRSeq:
		{	
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pResponse->pResponseHdr->pRSeqHdr != \
					SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Rseq Header is present");
					goto error;
				}		
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pResponse->pResponseHdr->pRSeqHdr = \
				(SipRseqHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Rseq Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
				
			else
			{
				if (SipFail == sip_initSipRseqHeader\
					(&(pSipMessage)->u.pResponse->pResponseHdr->\
						pRSeqHdr, pErr))
					return SipFail;
						

				(pSipMessage)->u.pResponse->pResponseHdr->pRSeqHdr->pBuffer= \
				sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}	
#endif		
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif			
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart, pEnd, \
					dHdrType, pHeader,pErr))
				{
				/*
				 * Parse the AuthorizationHeader header and Append to the list
				 */
				if (SipFail == sip_listAppend(&(pSipMessage->\
					u.pRequest->pRequestHdr->slAuthorizationHdr),\
						(SIP_Pvoid)(pHeader->pHeader),pErr))

					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Authorization Header Failed");
					return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING						
			}
			else
			{
				SipAuthorizationHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipAuthorizationHeader\
								(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart, \
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slAuthorizationHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
#endif			
			break;
		}			

		case SipHdrTypeWwwAuthenticate:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the wwwAuthenticate header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slWwwAuthenticateHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of WwwAuthenticate Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipWwwAuthenticateHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipWwwAuthenticateHeader(&pLocalHdr,\
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slWwwAuthenticateHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
			
#endif			
        			break;
		}
#endif		
		case SipHdrTypeSupported:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
									pHeader,pErr))
				{
				/*Parse the Supported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slSupportedHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Supported Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipSupportedHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipSupportedHeader(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
						slSupportedHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
#endif		
        			break;
		}		

#ifdef SIP_IMPP			
		case SipHdrTypeEvent:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif			
				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pEventHdr \
					!= SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Event Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
							pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pEventHdr = \
				(SipEventHeader*)(pHeader)->pHeader;
				else 
				{

					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Event Header Failed");
					return SipFail;
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				if (SipFail == sip_initSipEventHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
					pEventHdr, pErr))

				(pSipMessage)->u.pRequest->pRequestHdr->pEventHdr->\
					pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart, \
						pEnd-pStart+1, pErr);
			sip_freeSipHeader(pHeader);
			}	
#endif	
					break;
			/*End of General Headers*/
			/*Request Headers*/
		}

		case SipHdrTypeSubscriptionState:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif			
				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pSubscriptionStateHdr \
					!= SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Subscription-State Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
							pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pSubscriptionStateHdr = \
					(SipSubscriptionStateHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
						"SIP_ERROR: Parsing of Subscription-State Header Failed");
					return SipFail;
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				if (SipFail == sip_initSipSubscriptionStateHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
					pSubscriptionStateHdr, pErr))

				(pSipMessage)->u.pRequest->pRequestHdr->pSubscriptionStateHdr->\
					pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart, \
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
					break;
		}

#endif /* SIP_IMPP */

		case SipHdrTypeRoute:
		{	
#ifdef SIP_INCREMENTAL_PARSING	
				SIP_U32bit size= 0;
				/* get the size of the Via list, if size is greater than one that
				 * means Then if incremental par*/
				(void)sip_listSizeOf(&(pSipMessage)->u.pRequest->pRequestHdr->\
					slRouteHdr,&size,pErr);
				if (size <=0)
				{
					/*
					 * This means this is the first Via Header and not to be
					 * parsed incrementally
					 */
					if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
					{
						/*Parse the Via header and Append to the list*/
					if (SipFail == sip_listAppend(&(pSipMessage)->\
						u.pRequest->pRequestHdr->slRouteHdr,pHeader->pHeader,\
							pErr))
						return SipFail;
					}
					else
						return SipFail;
				}
				else if (SipSuccess ==pContext->pList->enable[dHdrType])
				{
					{
						/*Parse this header completely;*/
						if (SipFail != sip_parseHeaderBody(pStart,pEnd,\
							dHdrType, pHeader,pErr))
						{
							/*Parse the Viaheader and Append to the list*/
							if (SipFail == sip_listAppend(&(pSipMessage)->\
								u.pRequest->pRequestHdr->slRouteHdr,\
								pHeader->pHeader,pErr))
							return SipFail;
						}
						else 
						{
							return SipFail;	
						}	
					}
				
				}	
				else
				{
					SipRouteHeader *pLocalHdr = SIP_NULL;
				
					if (SipFail == sip_initSipRouteHeader(&pLocalHdr,pErr))
						return SipFail;
					pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1,pErr);
					sip_freeSipHeader(pHeader);
					if (SipFail == sip_listAppend(&(pSipMessage)->\
						u.pRequest->pRequestHdr->slRouteHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
						return SipFail;
				}
				/*End of Incremental parsing*/
#endif			
#ifndef SIP_INCREMENTAL_PARSING
			if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					pHeader,pErr))
			{
				/*Parse the Via header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRouteHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
			}
			else
			{
				
				sip_error\
					(SIP_Minor, "parsing of Route Header Failed");
				return SipFail;
				 	
			}
 /*End of RouteHeader  Header parsing*/
#endif		
				break ;
		}

#ifdef  SIP_REFER
		case SipHdrTypeReferTo:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif		
				if (pSipMessage->u.pRequest->pRequestHdr->pReferToHdr != \
					SIP_NULL)
				{
					sip_error (SIP_Major,\
						"SIP_ERROR: More than one Refer-To Header is present");
					sip_freeSipHeader(pHeader);
					(void)fast_memfree(DECODE_MEM_ID, pHeader, pErr);
						return SipFail;
				}		

				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{	
					(pSipMessage)->u.pRequest->pRequestHdr->pReferToHdr = \
					(SipReferToHeader*)(pHeader->pHeader);
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of ReferTo Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				if (SipFail == sip_initSipReferToHeader\
					(&(pSipMessage->u.pRequest->pRequestHdr->\
							pReferToHdr),pErr))
				return SipFail;
				(pSipMessage)->u.pRequest->pRequestHdr->pReferToHdr->pBuffer \
					= sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}
			
#endif	
				break;
		}
		case SipHdrTypeReferredBy:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pReferredByHdr \
					!= SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than  one ReferBy Header is present");
					sip_freeSipHeader(pHeader);
					(void)fast_memfree(DECODE_MEM_ID, pHeader, pErr);
						return SipFail;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
					(pSipMessage)->u.pRequest->pRequestHdr->pReferredByHdr = \
						(SipReferredByHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of ReferredBy Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipReferredByHeader\
					(&(pSipMessage->u.pRequest->pRequestHdr->\
							pReferredByHdr),pErr))
				return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pReferredByHdr->\
				   pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1,\
						   pErr);
				sip_freeSipHeader(pHeader);
			}
			
#endif	
			break;
		}	
#endif		
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pRackHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one RackHeader Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pRackHdr = \
				(SipRackHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Rack Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				if (SipFail == sip_initSipRackHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
						pRackHdr, pErr))
						return SipFail;

				(pSipMessage)->u.pRequest->pRequestHdr->pRackHdr->\
				pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
			
#endif	
			break;
		}	
#endif		
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyauthorization:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				{
				/*Parse the ProxyAuthorization  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slProxyAuthorizationHdr,\
						pHeader->pHeader,pErr))
					return SipFail;
				}
				else
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Proxy Authorization Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipProxyAuthorizationHeader *pLocalHdr = SIP_NULL;

				if (SipFail == sip_initSipProxyAuthorizationHeader\
					(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slProxyAuthorizationHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
#endif			
			break;				
		}
#endif		
		case SipHdrTypeUnsupported:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Unsupported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->u.pResponse->\
					pResponseHdr->slUnsupportedHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Unsupported Header Failed");
					 return SipFail;	
				}	 
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
			   SipUnsupportedHeader *pLocalHdr = SIP_NULL;
			   if (SipFail == sip_initSipUnsupportedHeader(&pLocalHdr, \
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slUnsupportedHdr,\
						(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;
			}
#endif			
			break;
		}
		case SipHdrTypeAllow:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Unsupported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
					slAllowHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Allow Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipAllowHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipAllowHeader(&pLocalHdr, \
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slAllowHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}
#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Unsupported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
					slAllowEventsHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{

					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of AllowEventsHeader Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
				
			else
			{
				SipAllowEventsHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipAllowEventsHeader(&pLocalHdr, \
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slAllowEventsHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}
#endif

#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		{

#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif	
				if (pSipMessage->pGeneralHdr->pPrivacyHdr != SIP_NULL)
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: More than one Privacy Header is present");
					goto error;
				}		
			
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody\
					(pStart, pEnd, dHdrType, pHeader, pErr))
				(pSipMessage)->pGeneralHdr->pPrivacyHdr = \
					(SipPrivacyHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error\
					(SIP_Minor, "parsing of Privacy Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	
			}
			else
			{
				if (SipFail == sip_initSipPrivacyHeader\
					(&(pSipMessage)->pGeneralHdr->pPrivacyHdr, pErr))
						return SipFail;

				(pSipMessage)->pGeneralHdr->pPrivacyHdr->pBuffer = \
					sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}		
#endif	
			break;
		}
case SipHdrTypePAssertId:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Unsupported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
						slPAssertIdHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of PAssertId Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipPAssertIdHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipPAssertIdHeader(&pLocalHdr, \
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slPAssertIdHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}
case SipHdrTypePPreferredId:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Unsupported  header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
						slPPreferredIdHdr,pHeader->pHeader,pErr))
					return SipFail;
				}
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of PPreferredId Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				SipPPreferredIdHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipPPreferredIdHeader(&pLocalHdr, \
					pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					pGeneralHdr->slPPreferredIdHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}
		
#endif
		
#ifdef SIP_WARNING
		case SipHdrTypeWarning:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
				 pHeader,pErr))
				{
				/*Parse the Warning header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slWarningHeader, \
						pHeader->pHeader, pErr))
					return SipFail;
				}
			
				else
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Warning  Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	 				 
			}
				
			else
			{
				SipWarningHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipWarningHeader(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
						pEnd-pStart+1, pErr);
			sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->u.pResponse->\
					pResponseHdr->slWarningHeader,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}
#endif		
		case SipHdrTypeContentDisposition:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipTrue == mimeHdrParsing)
				pContext->pList->enable[dHdrType] = SipSuccess;
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
				 pHeader,pErr))
				{
#ifdef SIP_MIME_PARSING				
					if (SipFalse == mimeHdrParsing)
#endif					
					{
						/*Parse the Warning header and Append to the list*/
						if (SipFail == sip_listAppend(&(pSipMessage)->\
							pGeneralHdr->slContentDispositionHdr, \
							pHeader->pHeader, pErr))
							return SipFail;
					}
#ifdef SIP_MIME_PARSING				
					else
						pMimeHdrs->pContentDisposition = \
							(SipContentDispositionHeader *)(pHeader)->pHeader;
#endif							
				}
			
				else
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of ContentDisposition Header Failed");
					return SipFail;	
				}	
#ifdef SIP_INCREMENTAL_PARSING	 				 
			}
			else if (SipTrue != mimeHdrParsing)
			{
				SipContentDispositionHeader *pLocalHdr = SIP_NULL;

				if (SipFail == sip_initSipContentDispositionHeader\
					(&pLocalHdr,pErr))
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1,pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->pGeneralHdr->\
					slContentDispositionHdr,(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
			break;
		}

#ifdef SIP_REPLACES
		case SipHdrTypeReplaces:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif			
				
				/*Parse this header completely;*/
				if (pSipMessage->u.pRequest->pRequestHdr->pReplacesHdr \
					!= SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Replaces Header is present");
					goto error;
				}		

				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
							pHeader,pErr))
				(pSipMessage)->u.pRequest->pRequestHdr->pReplacesHdr = \
					(SipReplacesHeader*)(pHeader)->pHeader;
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Replaces Header Failed");
					return SipFail;
				}	
#ifdef SIP_INCREMENTAL_PARSING					 
			}
			else
			{
				if (SipFail == sip_initSipReplacesHeader\
					(&(pSipMessage)->u.pRequest->pRequestHdr->\
					pReplacesHdr, pErr))

				(pSipMessage)->u.pRequest->pRequestHdr->pReplacesHdr->\
					pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart, \
						pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
			break;
		}
#endif /* SIP_REPLACES */
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyAuthenticate:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the ProxyAutheticate header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slProxyAuthenticateHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing ofProxy Authenticate Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipProxyAuthenticateHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipProxyAuthenticateHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slProxyAuthenticateHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		}
		break;
#endif		
#ifdef SIP_SECURITY
		case SipHdrTypeSecurityServer:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the SecurityServer header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slSecurityServerHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of SecurityServer Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipSecurityServerHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipSecurityServerHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slSecurityServerHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		}
		break;
	
        case SipHdrTypeSecurityClient:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the SecurityClient header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slSecurityClientHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of SecurityClient Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipSecurityClientHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipSecurityClientHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slSecurityClientHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		}
		break;
        case SipHdrTypeSecurityVerify:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the SecurityVerify header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slSecurityVerifyHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of SecurityVerify Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipSecurityVerifyHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipSecurityVerifyHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slSecurityVerifyHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		}
		break;
#endif        
		case SipHdrTypeMimeVersion:
		{	
			
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (pSipMessage->pGeneralHdr->pMimeVersionHdr != SIP_NULL)
				{
					sip_error (SIP_Major, \
					"SIP_ERROR: More than one Mime-version Header is present");
						
					goto error;
				}
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
								pHeader,pErr))
				{				
					(pSipMessage)->pGeneralHdr->pMimeVersionHdr = \
					(SipMimeVersionHeader*)(pHeader)->pHeader;
				}	
				else
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parse Error for Mime version ");
					return SipFail;	
				}
#ifdef SIP_INCREMENTAL_PARSING	
			}
				
			else
			{
				if (SipFail == sip_initSipMimeVersionHeader\
					(&(pSipMessage)->pGeneralHdr->pMimeVersionHdr\
						,pErr))
						return SipFail;

				(pSipMessage)->pGeneralHdr->pMimeVersionHdr->pBuffer \
				= sip_nStrdup(DECODE_MEM_ID, pStart,pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);
			}	
#endif	
		 break;
		}					 
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Resource Priority header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRsrcPriorityHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Resource Priority Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipRsrcPriorityHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipRsrcPriorityHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pRequest->pRequestHdr->slRsrcPriorityHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		break;
		}	

		case SipHdrTypeAcceptRsrcPriority:
		{
#ifdef SIP_INCREMENTAL_PARSING	
			if (SipSuccess ==pContext->pList->enable[dHdrType])
			{
#endif				
				/*Parse this header completely;*/
				if (SipFail != sip_parseHeaderBody(pStart,pEnd,dHdrType,\
					 pHeader,pErr))
				{
				/*Parse the Accept Resource Priority header and Append to the list*/
				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slAcceptRsrcPriorityHdr,\
					pHeader->pHeader,pErr))
					return SipFail;
				}
			
				else 
				{
					sip_error (SIP_Major,\
					"SIP_ERROR: Parsing of Accept Resource Priority Header Failed");
				  return SipFail;	
				 } 
#ifdef SIP_INCREMENTAL_PARSING						
			}
				
			else
			{
				SipAcceptRsrcPriorityHeader *pLocalHdr = SIP_NULL;
				if (SipFail == sip_initSipAcceptRsrcPriorityHeader(&pLocalHdr, \
				pErr))
				
					return SipFail;
				pLocalHdr->pBuffer = sip_nStrdup(DECODE_MEM_ID, pStart,\
					pEnd-pStart+1, pErr);
				sip_freeSipHeader(pHeader);

				if (SipFail == sip_listAppend(&(pSipMessage)->\
					u.pResponse->pResponseHdr->slAcceptRsrcPriorityHdr,\
					(SIP_Pvoid)pLocalHdr,pErr))
					return SipFail;

			}
#endif			
		}
		break;
#endif	
		default:;
	}/* End of Switch Case .*/
	/*
	 * For Failure cases.pHeader would have already been freed in side parse
	 * header body
	 */
	(void)fast_memfree(DECODE_MEM_ID, pHeader, pErr);
	
	return SipSuccess;
error:
			sip_error(SIP_Major ,"sip_handleIncParser() Failed\n");
			sip_freeSipHeader(pHeader);
			(void)fast_memfree(DECODE_MEM_ID, pHeader, pErr);
			return SipFail;
}/*End of function sip_handleIncParser*/
#ifdef SIP_BADMESSAGE_PARSING

/****************************************************************************
** FUNCTION: sip_fillBadHeader
** DESCRIPTION: This function calls the function fills the list of unknown header  when a bad message parsing is enabled 
** 
** PARAMETERS:
**      	      pHBodyEnd(IN)		:Beginnning of Header body
**				  pHBodyStart(IN)	:End of header body
**				  pHNameStart(IN)	:Beginning of header name
**			      pHNameEnd(IN)		:End of Header name
**				  pSipMessage(OUT)	:The filled sipmessage structure
**
** NOTE:  This function will be invoked when bad message parsing will be
**					enabled
****************************************************************************/


SipBool sip_fillBadHeader(SIP_S8bit *pHNameStart,SIP_S8bit *pHNameEnd,\
		SIP_S8bit *pHBodyStart,SIP_S8bit* pHBodyEnd,\
			SipMessage *pSipMessage,en_HeaderType dType,SipError *pError)
{
	SipBadHeader *pBadHdr = SIP_NULL;
#ifdef SIP_STATISTICS	
	/*INC_API_COUNT*/
#endif	


	if (SipFail == sip_initSipBadHeader(&pBadHdr,dType,pError))
		return SipFail;
	pBadHdr->pName = sip_nStrdup(DECODE_MEM_ID, pHNameStart,\
		pHNameEnd-pHNameStart +1, pError);
	pBadHdr->pBody = sip_nStrdup(DECODE_MEM_ID, pHBodyStart,\
		pHBodyEnd-pHBodyStart +1, pError);

	if (SipFail == sip_listAppend(&(pSipMessage->pGeneralHdr->slBadHdr),
			(SIP_Pvoid)pBadHdr,pError))
		{
		
			sip_freeSipBadHeader(pBadHdr);
			return SipFail;
		
		}	
return SipSuccess;
}	
#endif
/****************************************************************************
** FUNCTION: sip_decodeInvokeTimerCallBacks
** DESCRIPTION: This function calls the function invokes all the timer related
**		Call backs.
** 
** PARAMETERS:
**      	      tempMessage(IN):The decoded SipMessage
**				  pContext(IN)	:The SipEventContext Structure
**					err(IN/OUT) :SipError
**Return Value:
**			SipBool
**
****************************************************************************/

#ifndef SIP_TXN_LAYER

SipBool sip_decodeInvokeTimerCallbacks(SipMessage *tempMessage, \
SipEventContext *pContext,SipError *err)
{
		/*
		 * Checking for mandatory headers
		 */
        if (sip_checkMandatory(tempMessage) == SipFail)
        {
                *err = E_PARSER_ERROR;
#ifdef SIP_STATISTICS				
#ifndef SIP_BADMESSAGE_PARSING
				   INC_ERROR_PROTOCOL
#endif				   
#endif				
                return SipFail;
        }
        if (tempMessage->dType == SipMessageRequest)
        {
                SIP_S8bit *pMethod = \
					tempMessage->u.pRequest->pRequestLine->pStr1;

                if (sip_strcmp(pMethod,(SIP_S8bit*)"ACK")==0)
                {
					/*
					 * if method is an ACK call ing stop timer to stop
					 * retransmissions
					 */
				
                        (void)sip_stopTimer(tempMessage,pContext, err);
                }
                else if (sip_strcmp(pMethod,(SIP_S8bit*)"PRACK")==0)
                {
                        (void)sip_stopTimer(tempMessage,pContext, err);
                }
        }
        else if (tempMessage->dType == SipMessageResponse)
        {
                SIP_U16bit codenum=\
					tempMessage->u.pResponse->pStatusLine->dIntVar1;

                if ((codenum>=100)&&(codenum<200))
                {
                        /* 
						 *
						 * Form timerkey and restart timer with informational
						 */
                        /* If INVITE, informational stops transmissions */
                        if (sip_strcmp(tempMessage->pGeneralHdr->pCseqHdr->\
							pStr1,(SIP_S8bit*)"INVITE"))
                           (void)sip_updateTimer(tempMessage,SipSuccess,\
										pContext,err);
                        else 
						(void)sip_stopTimer(tempMessage,pContext,err);
                }
                else
                {
                        /* Form timer pKey and pStop timer */
                        /* Call Final indicate */
                        (void)sip_stopTimer(tempMessage,pContext,err);
         }
        }
        return SipSuccess;
}
#endif


#ifdef SIP_3GPP



/****************************************************************************
** FUNCTION: Sip_parseMsrpUrl
** DESCRIPTION: Application will parse MsrpUrl using this Method
** 
** PARAMETERS:
**				  pBuf (IN )    : Input Msrp String to be Parsed
**      	      		  pMsrpUrl(OUT) : The decodedMsrpUrl
**				  pErr(OUT)	: Error Information
**
****************************************************************************/


/*	msrp_url = "msrp" ":" "//" [userinfo] hostport ["/' resource]

	resource = 1*unreserved

	userinfo      = *( unreserved | escaped |
                         ";" | ":" | "&" | "=" | "+" | "$" | "," )

	hostport      = host [ ":" port ]
	port          = *digit   */
/*SipBool Sip_parseMsrpUrl(SIP_S8bit  *pBuf, MsrpUrl *pMsrpUrl, SipError *pErr);*/

SipBool Sip_parseMsrpUrl(SIP_S8bit  *pBuf, MsrpUrl *pMsrpUrl, SipError *pErr)
{
	SIP_S8bit		*pEnd;
	SIP_S8bit		*pCurrentPos = pBuf, *pToken;
	
        SIPDEBUGFN("Entering into sip_parseMsrpUrl");
#ifndef SIP_LINT
	if ((pBuf==SIP_NULL) ||(pErr==SIP_NULL) || (pMsrpUrl == SIP_NULL))
#else
	if ((pBuf==SIP_NULL) ||(pMsrpUrl == SIP_NULL))
#endif
	{
        *pErr = E_INV_PARAM;
		return SipFail;
	}
	
	
	
	pEnd = pCurrentPos + (sip_strlen(pBuf) - 1);
	
	/*
	 * First remove unwanted spaces
	 */
	
	/* Skip leading LWS */
	pCurrentPos = sip_stripLeadingLWS(pCurrentPos , pEnd);
	
	/* Skip trailing LWS */
	pEnd = sip_stripTrailingLWS(pCurrentPos , pEnd);
	

	/* Application will strip off "msrp://" from MSRP_URL and giving remaining part of 
the MSRP_URL to us 
a=session:<MSRP_URL>    */

	/* Now we are parsing [userinfo] */

	pToken = sip_tokenize(pCurrentPos, pEnd, '@');
        
	if ( pToken == NULL )
		pMsrpUrl->pUser = NULL;
	else
        {
         pMsrpUrl->pUser = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, pToken - 
                                pCurrentPos, pErr);
         pCurrentPos = pToken + 1;
        }
            
	pToken = sip_tokenize(pCurrentPos, pEnd, '/');
        if ( pToken == NULL)
	{
	      /* Now we are parsing Hostport */

	
               if (!sip_parseHostPortInMsrp(pCurrentPos,pEnd,&(pMsrpUrl->pHost),&(pMsrpUrl->dPort),pErr))
	      	{	
	      		
				sip_freeMsrpUrl(pMsrpUrl);
				*pErr = E_PARSER_ERROR;
				sip_error(SIP_Minor,"Error in parsing MsrpUri URL\n");
				return SipFail ;			
		}
              pMsrpUrl->pResource=NULL;
	 }		

	else
	{
	      /* Now we are parsing HostPort and resource */
              if (!sip_parseHostPortInMsrp(pCurrentPos,pToken-1,&(pMsrpUrl->pHost),&(pMsrpUrl->dPort),pErr))
              {
			sip_freeMsrpUrl(pMsrpUrl);
			*pErr = E_PARSER_ERROR;
			sip_error(SIP_Minor,"Error in parsing MsrpUri URL\n");
			return SipFail ;			
	      }
              pCurrentPos = pToken + 1;
              pMsrpUrl->pResource=sip_nStrdup(DECODE_MEM_ID, pCurrentPos, pEnd-pCurrentPos+1, pErr);	 	
	      
       }
	return SipSuccess;
	
} 

/****************************************************************************
** FUNCTION: sip_parseHostPortInMsrp
** DESCRIPTION: Application will parse MsrpUrl's Host and Port using this Method
** 
** PARAMETERS:
**				  pStart (IN )  : Start of the Input Msrp String to be Parsed
**				  pEnd   (IN )  : End of the Input Msrp String to be Parsed
**      	      		  ppHost (OUT)  : Host name 
**				  ppPort (OUT)  : Port number
**				  pErr(OUT)	: Error Information
**
****************************************************************************/


SipBool sip_parseHostPortInMsrp(SIP_S8bit *pStart,SIP_S8bit *pEnd,SIP_S8bit 
**ppHost,SIP_U16bit **ppPort,SipError *pErr)
{
	
	SIP_S8bit		*pToken = SIP_NULL;
	SIP_S8bit       *pHost = SIP_NULL;
	SIP_U32bit		dPort=0;
        

	*ppPort = (SIP_U16bit *)fast_memget(DECODE_MEM_ID, sizeof(SIP_U16bit), \
					pErr);
	if(*ppPort == SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
	pToken = sip_tokenize(pStart, pEnd, ':');
	if (pToken == SIP_NULL)
	{
		/*This means only Host part is present
		 */
		*ppPort = SIP_NULL;
		pHost = sip_nStrdup(DECODE_MEM_ID, pStart, pEnd-pStart+1, pErr);
		if(pHost == SIP_NULL)
		{
			(void)fast_memfree(DECODE_MEM_ID,*ppPort,pErr);
			sip_error(SIP_Minor,"Host absent\n");
			return SipFail;
		}	
	}
	else
	{
		SIP_S8bit* pHostEnd = SIP_NULL;
		SIP_S8bit* pPortBegin = SIP_NULL;
		pHostEnd = sip_stripTrailingLWS(pStart, pToken-1);
		pHost = sip_nStrdup(DECODE_MEM_ID, pStart, pHostEnd-pStart+1, pErr);
		if (SIP_NULL ==pHost)
		{
			(void)fast_memfree(DECODE_MEM_ID,*ppPort,pErr);
			return SipFail;
		}
		pPortBegin = sip_stripLeadingLWS(pToken+1,pEnd);
		if ((SipFail == sip_atoi(pPortBegin, pEnd, &dPort)) ||\
			(dPort > 65535))
		{
			(void)fast_memfree(DECODE_MEM_ID,pHost,pErr);
			return SipFail;
		}
		**ppPort = (SIP_U16bit)dPort;
	
	}	
	
	*ppHost = pHost;

	return SipSuccess;
}

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

