/***********************************************************************
** FUNCTION:
**			Message Body parsing functionality
************************************************************************
**
** FILENAME:
** microsip_msgbodyparser.c
**
** DESCRIPTION:
**		This file contains functions for parsing the message body
**		part of a SIP message. Message body types currently recognized
**		are SDP, MIME, SIP and ISUP. Anything else is stored as an 
**		unknown message.
**
** DATE	 		NAME		 	REFERENCE	 	REASON
** ----	 		----		 	---------	 	--------
** 26/02/2002	Aparna Kuppachi	    -			Initial
** 25-Apr-2002	Aparna				-			Fix for SPR 1_0_4.
** 28-Feb-2003	Mahesh				-			Added gotos and flags
** 20-Mar-2003	Aparna				-			Corrected flags, put back comments
**												and error traces removed while 
**												placing gotos.
** 12-Feb-2009  Rajiv Kumar     Rel 8.1         Changes done for handling
**                                              icf_tunnelled_sdp_info_st in
**                                              tunnelled mode.
** 27-May-2009   Kamal Ashraf    SPR 19672      CSR_1_7192585 Merged 
** 05-Jun-2009   Anuradha Gupta  SPR 19737      Klocworks Warning Removal     
** 20-Oct-2009   Anurag Khare    SPR 20154      Fix for SPR 20154
** 30-Oct-2009   Rajiv Kumar     SPR 20154      Fix for SPR 20154
**
**	COPYRIGHT , Aricent, 2009
***********************************************************************/

#include "microsip_struct.h"
#include "microsip_msgbodyparser.h"
#include "microsip_decode.h"
#include "microsip_decodeinternal.h"
#include "microsip_accessors.h"
#include "microsip_subapi.h"

/****************************************************************************
** FUNCTION: sip_parseMessageBody
** DESCRIPTION: This function is the top-level function for parsing the
**				message body part of the Sip Message. Individual parsers for
**				parsing the different types of message bodies are invoked
**				internally by this function.
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  start of the message body as given by the
**								  caller function sip_parseSipBuffer()
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				pSipMesg(IN/OUT)- The pointer to the SipMessage being parsed. 
**								  Upon successful parsing, the message body gets
**								  filled into this structure.
**				pContext(IN)	- Pointer to Event Context
**				pOpt(IN)		- The Options bit_mask	
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseMessageBody(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SipMessage *pSipMesg, SipEventContext *pContext, SipOptions *pOpt,\
	SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SipMsgBody		*pMsgBody = SIP_NULL; 
	SIP_S8bit		*pMediaType;
	SipBool			retVal = SipSuccess;
	en_SipBoolean	freeMsgBody = SipTrue;
	(void)pContext;
	SIPDEBUGFN("Entering into sip_parseMessageBody");

#ifndef SIP_NO_CHECK
	if ((SIP_NULL == pStart) || (SIP_NULL == pEnd))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	} 
#endif

	/*
	 * Update the global current parser position to the start of the
	 * message body as determined by sip_decode(...)
	 */

	/*sip_setGlbCurrentPosValue(pStart);*/

	/*
	 * Check for Content-Type hdr having been parsed in the SipMessage
	 * (Content-Type hdr MUST be present if the message body is not empty).
	 */

	if (SIP_NULL == pSipMesg->pGeneralHdr->pContentTypeHdr)
	{
		/* 
		 * No Content-Type hdr present; store the whole of msg body
		 * as unknown msg body. 
		 */
		/*begin SPR 1_0_4*/
#ifdef SIP_BADMESSAGE_PARSING		
		if ((pOpt->dOption & SIP_OPT_BADMESSAGE)== SIP_OPT_BADMESSAGE)
		{
		/*end SPR 1_0_4*/
			sip_error(SIP_Minor, "MessageBody present but no Content-Type header;\
				storing the message body as unknown body");

			if (SipFail == sip_initSipMsgBody(&pMsgBody, SipUnknownBody, \
				pErr))
			{
				return SipFail;
			}	
			if (SipFail == sip_initSipUnknownMessage(&(pMsgBody->u.\
				pUnknownMessage), pErr))
				goto error;
			pMsgBody->u.pUnknownMessage->dLength = pEnd - pStart + 1;
			pMsgBody->u.pUnknownMessage->pBuffer = (SIP_S8bit *) fast_memget(\
				DECODE_MEM_ID, pMsgBody->u.pUnknownMessage->dLength, pErr);
#ifndef SIP_NO_CHECK
			if (SIP_NULL == pMsgBody->u.pUnknownMessage->pBuffer)
				goto error;
#endif
			memcpy(pMsgBody->u.pUnknownMessage->pBuffer, pStart, \
				pMsgBody->u.pUnknownMessage->dLength);

			/*
			* Fill Message Body into the SipMessage
			*/

			if (SipSuccess != (sip_listAppend(&(pSipMesg->slMessageBody), \
				(SIP_Pvoid)pMsgBody, pErr)))
				goto error;
			*pErr = E_PARSER_ERROR;
			return SipSuccess;
		}
		else
#endif
		{
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}	

	pMediaType = pSipMesg->pGeneralHdr->pContentTypeHdr->pStr1;
#ifdef SIP_INCREMENTAL_PARSING	
	if (pMediaType ==SIP_NULL)
	{
		SipHeader *pCType = SIP_NULL;
/*		if(SipFail == sip_initSipHeader\
			(&pCType, SipHdrTypeContentType, pErr))
		{
			sip_freeSipMsgBody(pMsgBody);
			return SipFail;
		}	
	*/
		pCType = (SipHeader *)fast_memget(DECODE_MEM_ID, \
			sizeof(SipHeader), pErr);
		pCType->dType = SipHdrTypeContentType;	
		/*pCType->pHeader = pSipMesg->pGeneralHdr->pContentTypeHdr;*/
		if(SipFail == sip_getHeaderAtIndex(pSipMesg, \
			SipHdrTypeContentType,pCType, 0, pErr))
			goto error;
			
		/*pMediaType =
		 * ((SipContentTypeHeader*)(pCType->pHeader))->pStr1;*/
		pMediaType = pSipMesg->pGeneralHdr->pContentTypeHdr->pStr1;
		HSS_LOCKEDDECREF(((SipContentTypeHeader *) \
			(pCType->pHeader))->dRefCount);
		(void)fast_memfree(DECODE_MEM_ID,pCType,pErr);		
	}
#endif	
         if ((0 == sip_strcasecmp(pMediaType, (SIP_S8bit *) "application/sdp")) &&      
		(sip_strlen((SIP_S8bit *) "application/sdp") == sip_strlen(pMediaType)))
	{
		/*
		 * Invoke SDP parser
		 */

		if (SipFail == sip_initSipMsgBody(&pMsgBody, SipSdpBody, pErr))
		{
			return SipFail;
		}
		if (SipFail == sip_initSdpMessage(&(pMsgBody->u.pSdpMessage), pErr))
			goto error;
		retVal = sip_parseSdpMessage(pCurrentPos, pEnd, \
			pMsgBody->u.pSdpMessage, pOpt, pErr);	
	}
#ifdef SIP_MIME_PARSING	
        else if 
         (
         (
            (0 == sip_strcasecmp(pMediaType, (SIP_S8bit *) "multipart/mixed"               )) && \

		  (sip_strlen((SIP_S8bit *) "multipart/mixed") == sip_strlen(pMediaType)) 
		 ) ||
		 ( 
		  (0 == sip_strcasecmp(pMediaType, (SIP_S8bit *) "multipart/related" )) && 
		  (sip_strlen((SIP_S8bit *) "multipart/related") == sip_strlen(pMediaType)) 
		 )
		)
	{
		/*
		 * Invoke MIME parser
		 */
		
		retVal = sip_parseMimeEntity(0, pCurrentPos, pEnd, \
			pSipMesg, SIP_NULL , pContext, pOpt, SIP_NULL, pErr);	
		return retVal;
	}
#endif
#ifdef SIP_SIPMSGBODY
        else if ((0 == sip_strcasecmp(pMediaType, (SIP_S8bit *) "message/sipfrag"        )) && \
		(sip_strlen((SIP_S8bit *) "message/sipfrag") == sip_strlen(pMediaType)))
	{
		SIP_S8bit *pNextMsg = SIP_NULL;
		SipMessage *pDecSipMesg = SIP_NULL;
		/*
		 * Message body contains a full/partial SIP Message;
		 * invoke sip_parseSipBuffer again
		 */
		
		if (SipFail == sip_initSipMsgBody(&pMsgBody, SipAppSipBody, pErr))
		{
			return SipFail;
		}
		/*
		 * Use the option SIP_OPT_PARTIAL as a sip mesage in a message body
		 * can be a single status/Request line without any header
		 */ 
		pOpt->dOption |= SIP_OPT_PARTIAL | SIP_OPT_NOTIMER;
#ifdef SIP_MIME_PARSING
		retVal = sip_parseSipBuffer(pStart, &pDecSipMesg, pOpt, \
			(pEnd - pCurrentPos) +1, &pNextMsg, pContext, SIP_NULL, pErr);
#else		
		retVal = sip_parseSipBuffer(pStart, &pDecSipMesg, pOpt, \
			(pEnd - pCurrentPos) +1, &pNextMsg, pContext,  pErr);
#endif		
		pMsgBody->u.pAppSipMessage = pDecSipMesg;
	}
#endif	
#ifdef SIP_MWI
    else if ((0 == sip_strncmp(pMediaType, (SIP_S8bit *)
                    "application/simple-message-summary", \
		sip_strlen((SIP_S8bit *) "application/simple-message-summary"))) && \
		(sip_strlen((SIP_S8bit *) "application/simple-message-summary") \
         == sip_strlen(pMediaType)))
		{
				/*
				 * Invoke MWI parser
				 */

				if (SipFail == sip_initSipMsgBody(&pMsgBody,  \
										SipMessageSummaryBody, pErr))
				{
						return SipFail;
				}
				if (SipFail == sip_mwi_initMesgSummaryMessage( \
										&(pMsgBody->u.pSummaryMessage), pErr))
				goto error;

				retVal = sip_parseMwiMessage(pCurrentPos, pEnd, \
								pMsgBody->u.pSummaryMessage, pOpt, pErr);	
		}
#ifdef SIP_MIME_PARSING	
	else if ((0 == sip_strncmp(pMediaType, (SIP_S8bit *) "multipart/mixed", \
		sip_strlen((SIP_S8bit *) "multipart/mixed"))) && \
		(sip_strlen((SIP_S8bit *) "multipart/mixed") == sip_strlen(pMediaType)))

	{
		/*
		 * Invoke MIME parser
		 */
		
		retVal = sip_parseMimeEntity(0, pCurrentPos, pEnd, \
			pSipMesg, SIP_NULL , pContext, pOpt, SIP_NULL, pErr);	
		return retVal;
	}
#endif
#endif
#ifdef SIP_ISUP	
        else if ((0 == sip_strcasecmp(pMediaType, (SIP_S8bit *) "application/isup        ")) && 
		(sip_strlen((SIP_S8bit *) "application/isup") == sip_strlen(pMediaType)))
	{
		/*
		 * ISUP messages are stored as sequence of bytes
		 */

		if (SipFail == sip_initSipMsgBody(&pMsgBody, SipIsupBody, pErr))
		{
			return SipFail;
		}	
		if (SipFail == sip_initIsupMessage(&(pMsgBody->u.pIsupMessage), pErr))
			goto error;
		
		pMsgBody->u.pIsupMessage->dLength = pEnd - pStart + 1;
		pMsgBody->u.pIsupMessage->pBody = (SIP_S8bit *) fast_memget(\
			DECODE_MEM_ID, pMsgBody->u.pIsupMessage->dLength, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pMsgBody->u.pIsupMessage->pBody)
			goto error;
#endif
		memcpy(pMsgBody->u.pIsupMessage->pBody, pStart, \
			pMsgBody->u.pIsupMessage->dLength);
	}
#endif	
	else
	{
		/*
		 * Unrecognized message body
		 */
			
		if (SipFail == sip_initSipMsgBody(&pMsgBody, SipUnknownBody, \
			pErr))
		{
			return SipFail;
		}
		if (SipFail == sip_initSipUnknownMessage(&(pMsgBody->u.\
			pUnknownMessage), pErr))
			goto error;
		
		pMsgBody->u.pUnknownMessage->dLength = pEnd - pStart + 1;
		pMsgBody->u.pUnknownMessage->pBuffer = (SIP_S8bit *) fast_memget(\
			DECODE_MEM_ID, pMsgBody->u.pUnknownMessage->dLength, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pMsgBody->u.pUnknownMessage->pBuffer)
			goto error;
#endif
		memcpy(pMsgBody->u.pUnknownMessage->pBuffer, pStart, \
			pMsgBody->u.pUnknownMessage->dLength);
	}

	
	if (SipFail == retVal)
	{
		/*
		 * If BADMESSAGE parsing is enabled, then increment Entity
		 * error count, and return SUCCESS. The msg body does not
		 * get stored into the SIP mesg in this case.
		 */
#ifdef SIP_BADMESSAGE_PARSING		
		if ((pOpt->dOption & SIP_OPT_BADMESSAGE)== SIP_OPT_BADMESSAGE)
		{
			sip_freeSipMsgBody(pMsgBody);
		}
		else
		{
#endif		
			if (SipTrue == freeMsgBody)
				sip_freeSipMsgBody(pMsgBody);
#ifdef SIP_BADMESSAGE_PARSING		
		}
#endif		
		return SipFail;
	}

	/*
	 * Fill Message Body into the SipMessage
	 */

	if (SipTrue == freeMsgBody)
	{
		if (SipSuccess != (sip_listAppend(&(pSipMesg->slMessageBody), \
			(SIP_Pvoid)pMsgBody, pErr)))
			goto error;
	}

	SIPDEBUGFN("Exiting from  sip_parseMessageBody");
	return SipSuccess;
error:
		sip_freeSipMsgBody(pMsgBody);
		sip_error(SIP_Major ,"sip_parseMessageBody Failed \n");
		return SipFail;
   
} /* end of function sip_parseMessageBody */
		
		

/****************************************************************************
** FUNCTION: sip_parseSdpMessage
** DESCRIPTION: This function parses an SDP message. It internally invokes
**				various sub-parsers for parsing the different SDP lines. It
**				is assumed that only one SDP message is contained per 
**				SipMessage.
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				pSdpMsg(OUT) 	- The pointer to the output SDP message 
**				pOpt(IN)		- The Options bit_mask	
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
		
SipBool	sip_parseSdpMessage(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SdpMessage *pSdpMsg, SipOptions *pOpt, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;

	/*
	 * pNewCurrentPos variable is set by all the sub-parsers 
	 * before returning, to indicate to the parent function the 
	 * position at which they have stopped parsing.
	 */
	SIP_S8bit		*pNewCurrentPos = SIP_NULL;

	en_SipBoolean	connInfoAtSessionLevel = SipFalse;

	SIPDEBUGFN("Entering into sip_parseSdpMessage");
	(void)pOpt;
	/*
	 * Parse Version line 
	 */
     /*Change for IPTK CSR1-5734896*/
     if(pCurrentPos > pEnd)
     {
       sip_error(SIP_Minor, "Incorrect Content Length");
       *pErr = E_PARSER_ERROR;
       return SipFail;
     }
	if ((*pCurrentPos != 'v') || (*(pCurrentPos+1) != '='))
	{
		sip_error(SIP_Minor, "Syntax error in SDP message: v= line absent");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pCurrentPos+=2;
    if(pCurrentPos > pEnd)
    {
      sip_error(SIP_Minor, "Incorrect Content Length");
      *pErr = E_PARSER_ERROR;
      return SipFail;
    }
	if (SipFail == sip_parseSdpVersion(pCurrentPos, pEnd, &pNewCurrentPos, \
		pSdpMsg, pErr))
	{
		sip_error(SIP_Minor, "Syntax error in SDP message: error in v= line"); 
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pCurrentPos = pNewCurrentPos;

	/* 	
	 * Parse Origin line
	 */
    if(pCurrentPos > pEnd)
     {
        sip_error(SIP_Minor, "Incorrect Content Length");
        *pErr = E_PARSER_ERROR;
        return SipFail;
     }
	if ((*pCurrentPos != 'o') || (*(pCurrentPos+1) != '='))
	{
		sip_error(SIP_Minor, "Syntax error in SDP message: o= line absent");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pCurrentPos+=2;
    if(pCurrentPos > pEnd)
    {
       sip_error(SIP_Minor, "Incorrect Content Length");
       *pErr = E_PARSER_ERROR;
       return SipFail;
    }
	if (SipFail == sip_parseSdpOrigin(pCurrentPos, pEnd, &pNewCurrentPos, \
		pSdpMsg, pErr))
	{
		sip_error(SIP_Minor, "Syntax error in SDP message: error in o= line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pCurrentPos = pNewCurrentPos;

	/* 
	 * Parse session name 
	 */

	/*
	 * The RFC mandates a session name field. However, we are not
	 * mandating this in order to parse the SIPIt test messages most
	 * of which do not contain this SDP line.
	 */
/*CSR 1-4735601*/
	if ((*pCurrentPos == 's') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpSessionNameOrSessionInfo(pCurrentPos, pEnd, \
			&pNewCurrentPos, &(pSdpMsg->pSession), SipTrue, pErr))
		{
			sip_error(SIP_Minor, \
				"Syntax error in SDP message: error in s= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pCurrentPos = pNewCurrentPos;
	}

	/* 
	 * Parse session info if present
	 */
/*CSR 1-4735601*/

	if ((*pCurrentPos == 'i') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpSessionNameOrSessionInfo(pCurrentPos, pEnd, \
			&pNewCurrentPos, &(pSdpMsg->pInformation), SipFalse, pErr))
		{
			sip_error(SIP_Minor, "Syntax error in SDP message: error \
				in i= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pCurrentPos = pNewCurrentPos;
	}

	/* 
	 * Parse Uri if present
	 */
/*CSR 1-4735601*/

	if ((*pCurrentPos == 'u') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpSessionNameOrSessionInfo(pCurrentPos, pEnd, \
			&pNewCurrentPos, &(pSdpMsg->pUri), SipFalse, pErr))
		{
			sip_error(SIP_Minor, "Syntax error in SDP message: error in \
				parsing u= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pCurrentPos = pNewCurrentPos;
	}

	/*
	 * Parse any email fields if present 
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'e') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpEmailAddrOrPhoneNum(pCurrentPos, pEnd, \
				&pNewCurrentPos, &(pSdpMsg->slEmail), pErr))
			{
				return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else 
			break;
	}

	/*
	 * Parse any phone fields if present 
	 */

	for(;;)
    {
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'p') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpEmailAddrOrPhoneNum(pCurrentPos, pEnd, \
				&pNewCurrentPos, &(pSdpMsg->slPhone), pErr))
			{
				return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else 
			break;
	}

	/*
	 * Parse connection field if any present at session-level
	 */
/*CSR 1-4735601*/
	if ((*pCurrentPos == 'c') && (*(pCurrentPos + 1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		/*
		 * connection field found at session-level;
		 * set a flag to indicate this 
		 */

		connInfoAtSessionLevel = SipTrue;
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpConnectionInfo(pCurrentPos, pEnd, \
			&pNewCurrentPos, &(pSdpMsg->slConnection), pErr))
		{
			return SipFail;
		}
		pCurrentPos = pNewCurrentPos;
	}

	/*
	 * Parse bandwidth fields if any present
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'b') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpBandwidthInfo(pCurrentPos, pEnd, \
				&pNewCurrentPos, &(pSdpMsg->pBandwidth), pErr))
			{
				return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else 
			break;
	}		
	/* 
	 * Parse time fields 
	 */

	/*
	 * The RFC mandates at least 1 time field. However, we are not
	 * mandating this in order to interop with certain external softphones
	 * which do not send this line by default.
	 */

	for(;;)
    {
/*CSR 1-4735601*/
		if ((*pCurrentPos == 't') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpTime(pCurrentPos, pEnd, &pNewCurrentPos, \
				&(pSdpMsg->slTime), pErr))
			{
					return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else
			break;
	}
		

	/* 
	 * Parse encryption key 
	 */
/*CSR 1-4735601*/
	if ((*pCurrentPos == 'k') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpKey(pCurrentPos, pEnd, &pNewCurrentPos, \
			&(pSdpMsg->pKey), pErr))
		{
			return SipFail;
		}
		pCurrentPos = pNewCurrentPos;
	}

	/* 
	 * Parse attributes if any
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'a') && (*(pCurrentPos + 1) == '=') &&\
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpAttr(pCurrentPos, pEnd, \
				&pNewCurrentPos, &(pSdpMsg->slAttr), pErr))
			{
				return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else 
			break;
	}

	/* 
	 * Parse media descriptors if any
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'm') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpMediaDescription(pCurrentPos, pEnd, \
				connInfoAtSessionLevel, &pNewCurrentPos, &(pSdpMsg->slMedia), \
				pErr))
			{
				return SipFail;
			}
			pCurrentPos = pNewCurrentPos;
		}
		else 
			break;
	} 

            
	
	/*
	 * At this point we have parsed all the SDP lines we expect to
	 * always find in an SDP message. Any other lines present in the 
	 * input buffer beyond this are ignored.
	 */

	SIPDEBUGFN("Exiting from sip_parseSdpMessage");
	return SipSuccess;

} /* end of function sip_parseSdpMessage */


/****************************************************************************
** FUNCTION: sip_parseSdpVersion
** DESCRIPTION: This function parses the Version field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pSdpMsg(OUT) 	- The pointer to the output SDP message into 
**								  the Version field is filled in. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpVersion(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpMessage *pSdpMsg, SipError *pErr)
{
	SIP_S8bit	*pCurrentPos = pStart, *pToken;
	SIP_U32bit	temp;
	SIP_U32bit	tempnewline=0;
	SIP_U32bit	increment = 0;

	SIPDEBUGFN("Entering into sip_parseSdpVersion");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
			not allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
/* Changes for CSR_1-5833665 Starts */
	pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
	if (SIP_NULL == pToken)
	{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */

			sip_error(SIP_Minor, "Neither CR nor LF present at end of v= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
		increment = 1;

		if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif	
/* Changes for CSR_1-5833665 Ends */	
	
	/*
	 * Validate version field if present
	 */

	if ((pCurrentPos == pToken) || \
		(SipFail == sip_atoi(pCurrentPos, (pToken - tempnewline) -1 , &temp))) /* Changes for CSR_1-5833665 */
	{
		/* 
		 * Non-numeric SDP version field found;
		 * raise parser error
		 */

		sip_error(SIP_Minor, "Null/non-numeric SDP version field");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Store Version
	 */
		
	pSdpMsg->pVersion = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		(pToken - tempnewline) - pCurrentPos, pErr);
	
	/* 
	 * Advance current parser position to beginning of next line.
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseSdpVersion");
	return SipSuccess;

} /* end of function sip_parseSdpVersion */


		
/****************************************************************************
** FUNCTION: sip_parseSdpOrigin
** DESCRIPTION: This function parses the Origin field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pSdpMsg(OUT) 	- The pointer to the output SDP message into 
**								  the Origin field is filled in. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpOrigin(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpMessage *pSdpMsg, SipError *pErr)
{
	SIP_S8bit	*pCurrentPos = pStart, *pToken;
	SIP_U32bit	temp;
	SIP_U32bit	increment = 0;
	SdpOrigin	*pOrigin;
	SIP_U32bit	tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpOrigin");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */
		
		sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
			not allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Initialize Origin field 
	 */
	if (SipFail == sip_initSdpOrigin(&pOrigin, pErr))
		return SipFail;	

	/*
	 * Look for username
	 */
	
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	
	
	pOrigin->pUser = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;


	/*
	 * Look for session-id
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		 goto error;

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))	
		 goto error;
	pOrigin->pSessionid = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;

	/*
	 * Look for session-version
	 */
	
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		 goto error;
	
	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))
		 goto error;
	pOrigin->pVersion = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;
	

	/*
	 * Look for network-type
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		 goto error;

	if (2 != (pToken - pCurrentPos))
	 goto error;

	if (0 != sip_strncmp((SIP_S8bit *) "IN", pCurrentPos, 2))
		 goto error;
	pOrigin->pNetType = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;

	/*
	 * Look for addr-type 
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;
	if (3 != (pToken - pCurrentPos))
		 goto error;
	
	if (0 != sip_strncmp((SIP_S8bit *) "IP4", pCurrentPos, 3))
		if (0 != sip_strncmp((SIP_S8bit *) "IP6", pCurrentPos, 3))
		 goto error;
	pOrigin->pAddrType = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;

	/*
	 * Look for address
	 */
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
	if (SIP_NULL == pToken)
	{
		/* 
		 * Neither CR nor LF present at the end of line.
		 */
		sip_error(SIP_Minor, "Neither CR nor LF present at end of o= line");
		*pErr = E_PARSER_ERROR;
                goto error;
	}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
		increment = 1;
	        if ('\r' == *(pToken - 1))
	     	    tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */

	
	if (SipFail == sip_parseSdpAddress())
		goto error;
	
	pOrigin->pAddr = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		(pToken - tempnewline) - pCurrentPos, pErr);

	/*
	 * Advance current parser position beyond LF 
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
		
	pSdpMsg->pOrigin = pOrigin;

	SIPDEBUGFN("Exiting from sip_parseSdpOrigin");
	return SipSuccess;
error:	
		sip_freeSdpOrigin(pOrigin);
		sip_error(SIP_Minor ,"sip_parseSdpOrigin Failed \n");
		*pErr = E_PARSER_ERROR;
		return SipFail;

} /* end of function sip_parseSdpOrigin */
		

	
/****************************************************************************
** FUNCTION: sip_parseSdpSessionNameOrSessionInfo
** DESCRIPTION: This function parses the Session-name/session-info fields of
**				an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				ppSessionName(OUT) 	- The pointer to the session-name/session-
**									  info fields of the SDP message.
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
	
SipBool	sip_parseSdpSessionNameOrSessionInfo(SIP_S8bit *pStart, \
	SIP_S8bit *pEnd, SIP_S8bit **ppNewCurrentPos, SIP_S8bit **ppSessionName, \
	en_SipBoolean	isSessionName, SipError *pErr)
{
	SIP_S8bit	*pCurrentPos = pStart;
	SIP_S8bit	*pToken;
	SIP_U32bit	increment = 0;
	SIP_U32bit	tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpSessionNameOrSessionInfo");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		if (SipFalse == isSessionName)
		{
			/* 
			 * white space is not allowed around the '='
			 */

			sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
				not allowed arnd '='");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}

/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
                        sip_error(SIP_Minor, "Neither CR nor LF present at end of one of \
				the foll. lines: s=/i=/u=");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
	if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */
	  
	*ppSessionName = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		(pToken - tempnewline) - pCurrentPos, pErr);
	if (SIP_NULL == *ppSessionName)
	{
		return SipFail;
	}

	/*
	 * Advance current parser position to beginning of next line. 
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
	
	SIPDEBUGFN("Exiting from sip_parseSdpSessionNameOrSessionInfo");
	return SipSuccess;	

} /* end of function sip_parseSdpSessionNameOrSessionInfo */

	
/****************************************************************************
** FUNCTION: sip_parseSdpConnectionInfo
** DESCRIPTION: This function parses the connection field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pConnInfo(OUT) 	- The pointer to the output connection info 
**								  structure.
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpConnectionInfo(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SdpConnection **ppConnInfo, SipError *pErr)
{
	SIP_S8bit	*pCurrentPos = pStart, *pToken;
	SIP_U32bit	increment = 0;
	SIP_U32bit	tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpConnectionInfo");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
			not allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Allocate memory for Connection field 
	 */
	
	if (SipFail == sip_initSdpConnection(ppConnInfo, pErr))
		return SipFail;

	/* 
	 * Look for network-type
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		sip_error(SIP_Minor, "network-type fld absent in c= line");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	
	if (2 != (pToken - pCurrentPos))
		  goto error;
	if (0 != sip_strncmp((SIP_S8bit *) "IN", pCurrentPos, 2))
		goto error;
	(*ppConnInfo)->pNetType = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;
	
	/*
	 * Look for addr-type 
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;
	if (3 != (pToken - pCurrentPos))
	  goto error;
	if (0 != sip_strncmp((SIP_S8bit *) "IP4", pCurrentPos, 3))
		if (0 != sip_strncmp((SIP_S8bit *) "IP6", pCurrentPos, 3))
		  goto error;

	(*ppConnInfo)->pAddrType = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken + 1;

	/*
	 * Look for connection-address
	 */
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
                        sip_error(SIP_Minor, "Neither CR nor LF present at end of c= line");
			goto error;
	}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
	if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */

	if (SipFail == sip_parseSdpConnectionAddress())
		goto error;
		
	(*ppConnInfo)->pAddr = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		(pToken-tempnewline)- pCurrentPos,pErr);

	/*
	 * Advance current parser position to the bginning of next line.
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
		
	SIPDEBUGFN("Exiting from sip_parseSdpConnectionInfo");
	return SipSuccess;
error:	
	sip_freeSdpConnection(*ppConnInfo);
	*ppConnInfo = SIP_NULL;
	sip_error(SIP_Major ,"sip_parseSdpConnectionInfo Failed \n");
	*pErr = E_PARSER_ERROR;
	return SipFail;

} /* end of function sip_parseSdpConnectionInfo */



/****************************************************************************
** FUNCTION: sip_parseSdpBandwidthInfo
** DESCRIPTION: This function parses the bandwidth field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pBwList(OUT) 	- The pointer to the bandwidth list of the SDP 
**								  message into which the parsed bandwidth field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpBandwidthInfo(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pBwList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pToken;
	SIP_S8bit		*pBwInfo;
	SIP_U32bit		increment = 0;
	SIP_U32bit		tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpBandwidthInfo");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		/*sip_listDeleteAll(pBwList, pErr);*/
		sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
			not allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Neither CR nor LF present at end of b= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
	if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */

	pBwInfo = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, (pToken-tempnewline) - pCurrentPos, pErr);
#ifndef SIP_NO_CHECK	
	if (SIP_NULL == pBwInfo)
	{
		return SipFail;
	}
#endif	
	if (SipSuccess != (sip_listAppend(pBwList,(SIP_Pvoid)pBwInfo, pErr)))
	{	
		(void)fast_memfree(0, pBwInfo, pErr);
		return SipFail;
	}

	/*
	 * Advance current parser position to the beginning of next line. 
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
		
	SIPDEBUGFN("Exiting from sip_parseSdpBandwidthInfo");
	return SipSuccess;		

} /* end of function sip_parseSdpBandwidthInfo */


		
/****************************************************************************
** FUNCTION: sip_parseSdpTime
** DESCRIPTION: This function parses the time field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pTimeList(OUT) 	- The pointer to the time list of the SDP 
**								  message into which the parsed time field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpTime(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pTimeList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken; 
	SIP_U32bit		temp;
	SIP_U32bit		tempnewline=0;
	SdpTime			*pTime = SIP_NULL;
	SIP_U32bit		increment = 0;

	SIPDEBUGFN("Entering into sip_parseSdpTime");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
			not allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Init time field
	 */

	if (SipFail == sip_initSdpTime(&pTime, pErr))
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
		
	/*
	 * Look for start-time
	 */
	
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	if ((1 == (pToken - pCurrentPos)) && (*pCurrentPos == '0'))
	{
		pTime->pStart = sip_nStrdup(DECODE_MEM_ID, (SIP_S8bit *) "0", 1, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStart)
			goto error;
#endif
	}
	else
	{
		if ((*pCurrentPos < '1') || (*pCurrentPos > '9'))
		goto error;
		if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))	
			goto error;
		if (100000000 > temp)
			goto error;
		pTime->pStart = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pToken - pCurrentPos, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStart)
			goto error;
#endif
	}
	pCurrentPos = pToken + 1;

	/*
	 * Look for stop-time
	 */
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_freeSdpTime(pTime);
			sip_error(SIP_Minor, "Neither CR nor LF present at end of t= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
		increment = 1;

	if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */
	
	if ((1 == ((pToken - tempnewline) - pCurrentPos)) && (*pCurrentPos == '0'))
	{
		pTime->pStop = sip_nStrdup(DECODE_MEM_ID, (SIP_S8bit *) "0", 1, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStop)
			goto error;
#endif
	}
	else
	{
		if ((*pCurrentPos < '1') || (*pCurrentPos > '9'))
			goto error;
		if (SipFail == sip_atoi(pCurrentPos, (pToken - tempnewline) - 1, &temp))	
			goto error;
		if (100000000 > temp)
			goto error;
		pTime->pStop = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			(pToken - tempnewline )- pCurrentPos, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStop)
			goto error;
#endif
	}
	pCurrentPos = pToken + increment;
    /* CSR_1-7813405 START*/  
	*ppNewCurrentPos = pCurrentPos;
    /* CSR_1-7813405 END*/
	/* 
	 * If repeat-field is present, parse it
	 */

	for(;;)
	{
		if (('r' == *pCurrentPos) && ('=' == *(pCurrentPos + 1)))
		{	
			pCurrentPos+=2;

			if (SipFail == sip_parseSdpRepeatField(pCurrentPos, pEnd, \
				ppNewCurrentPos, &(pTime->slRepeat), pErr))
            {
                goto error;
            }
            /* CSR_1-7909949 Change Start */
            else
            {
                pCurrentPos = *ppNewCurrentPos;
            }
            /* CSR_1-7909949 Change End */
        }
        else
			break;
    } 
    
    /* Fix for UATK CSR1-7909949: SPR 20154 Start */
    if(('z' == *pCurrentPos) && ('=' == *(pCurrentPos + 1)))
    {
        pCurrentPos+=2;
        if (SipFail == sip_parseSdpZone(pCurrentPos, pEnd, \
                    ppNewCurrentPos, &(pTime->pZone), pErr))
            goto error;
    }
    for(;;)
    {
        if(('z' == **ppNewCurrentPos) && ('=' == *(*ppNewCurrentPos+1)))
            goto error;
        else 
            break; 
    }
    /* Fix for UATK CSR 1-7909949 End */
/* CSR_1-7813405 START */  
#if 0
	pCurrentPos = *ppNewCurrentPos;
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Error in t= line");
                        goto error;
	}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */
#endif
/* CSR_1-7813405 END */

	if (SipSuccess != (sip_listAppend(pTimeList, (SIP_Pvoid)pTime, pErr)))
			goto error;
/* CSR_1-7813405 START */  
#if 0
	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
#endif
/* CSR_1-7813405 END */

	SIPDEBUGFN("Exiting from sip_parseSdpTime");
	return SipSuccess;
error:
		sip_freeSdpTime(pTime);
		sip_error(SIP_Minor ,"sip_parseSdpTime\n");
		*pErr = E_PARSER_ERROR;
		return SipFail;
} /* end of function sip_parseSdpTime */


/* Fix for UATK CSR1-7909949: SPR 20154 Start */
/****************************************************************************
** FUNCTION: sip_parseSdpZone
** DESCRIPTION: This function parses the repeat field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pRepList(OUT) 	- The pointer to the repeat list of the SDP 
**								  message into which the parsed repeat field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpZone(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SIP_S8bit **pZoneLine, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken;
	SIP_S8bit		*pZon;
	SIP_U32bit	increment = 0;
  SIP_U32bit		tempnewline=0;

	SIPDEBUGFN("Entering into sip_parseSdpZone");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP line: whitespace is not \
			allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */

			sip_error(SIP_Minor, "Neither CR nor LF present at end of z= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
    increment = 1;

    if ('\r' == *(pToken - 1))
      tempnewline = 1;
	}
#endif

pZon = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, (pToken - tempnewline) - pCurrentPos, pErr);
	if (SIP_NULL == pZon)
	{
	  return SipFail;
  }
    (*pZoneLine) = sip_strdup(pZon, 0);
    if (SIP_NULL == (*pZoneLine))
    {
        (void)fast_memfree(0, pZon, pErr);
	   	return SipFail;
    }
    (void)fast_memfree(0, pZon, pErr);   
	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseSdpZone");
	return SipSuccess;

} /* end of function sip_parseSdpZone */
/* Fix for UATK CSR 1-7909949 End */


/****************************************************************************
** FUNCTION: sip_parseSdpRepeatField
** DESCRIPTION: This function parses the repeat field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pRepList(OUT) 	- The pointer to the repeat list of the SDP 
**								  message into which the parsed repeat field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpRepeatField(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pRepList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken;
	SIP_S8bit		*pRep;
	SIP_U32bit	increment = 0;
	SIP_U32bit		tempnewline=0;

	SIPDEBUGFN("Entering into sip_parseSdpRepeatField");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP line: whitespace is not \
			allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */

			sip_error(SIP_Minor, "Neither CR nor LF present at end of r= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
    increment = 1;

    if ('\r' == *(pToken - 1))
      tempnewline = 1;
	}
#endif
/* Changes for CSR_1-5833665 Ends */

	pRep = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, (pToken - tempnewline) - pCurrentPos, pErr);
	if (SIP_NULL == pRep)
	{
		return SipFail;
	}
		
	if (SipSuccess != (sip_listAppend(pRepList, (SIP_Pvoid)pRep, pErr)))
	{	
		(void)fast_memfree(0, pRep, pErr);
		return SipFail;
	}

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseSdpRepeatField");
	return SipSuccess;

} /* end of function sip_parseSdpRepeatField */




/****************************************************************************
** FUNCTION: sip_parseSdpAttr
** DESCRIPTION: This function parses the attribute field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pAttrList(OUT) 	- The pointer to the attribute list of the SDP 
**								  message into which the parsed attribute field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpAttr(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pAttrList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken, *pTmpToken;
	SdpAttr			*pAttr;
	SIP_U32bit	increment = 0;
	SIP_U32bit	tempnewline = 0;

	SIPDEBUGFN("Entering into sip_parseSdpAttr");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP line: whitespace is not \
			allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Init attr field
	 */

	if (SipFail == sip_initSdpAttr(&pAttr, pErr))
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
		
	/*
	 * Look for attr-name
	 */
/* Changes for CSR_1-5833665 Starts */
		pToken = sip_tokenize_sdp_attr(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Neither CR nor LF present at end of a= line");
			*pErr = E_PARSER_ERROR;
                        goto error;
	}
	else
	{
		/*
		 * CR may or may not be followed by LF.
		 */
			increment = 1;
		if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif

	pTmpToken = sip_tokenize_sdp_attr(pCurrentPos, (pToken-tempnewline)-1, ':'); /* Changes for CSR_1-5833665 */

	if (SIP_NULL == pTmpToken)
	{
		/*
		 * e.g. a=orient
		 */

		pAttr->pName = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pToken - pCurrentPos - tempnewline, pErr);/* Changes for CSR_1-5833665 */
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pAttr->pName)
			goto error;
#endif
		pCurrentPos = pToken + increment;
	}
	else
	{
		/*
		 * e.g. a=orient:portrait
		 */

		pAttr->pName = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pTmpToken - pCurrentPos, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pAttr->pName)
			goto error;
#endif
		pCurrentPos = pTmpToken + 1;

			pToken = sip_tokenize_sdp_attr(pCurrentPos, pEnd, '\n');
#if 1	
			if (SIP_NULL == pToken)
			{
				/* 
				 * Neither CR nor LF present at the end of line.
				 */
                                sip_freeSdpAttr(pAttr);
				sip_error(SIP_Minor, \
					"Neither CR nor LF present at end of a= line");
				*pErr = E_PARSER_ERROR;
				return SipFail;
			}
		else
		{
			/*
	 		* CR may or may not be followed by LF.
	 		*/
		increment = 1;
		
		if ('\r' == *(pToken - 1))
		tempnewline = 1;

		}
#endif

		pAttr->pValue = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pToken - pCurrentPos -tempnewline , pErr);/* Changes for CSR_1-5833665 */
/* Changes for CSR_1-5833665 End */

#ifndef SIP_NO_CHECK
		if (SIP_NULL == pAttr->pValue)
			goto error;
#endif
		pCurrentPos = pToken + increment;
	}
	
	if (SipSuccess != (sip_listAppend(pAttrList, (SIP_Pvoid)pAttr, pErr)))
		goto error;
	
	*ppNewCurrentPos = pCurrentPos;
	SIPDEBUGFN("Exiting from sip_parseSdpAttr");
	return SipSuccess;
error:	
		sip_freeSdpAttr(pAttr);
		return SipFail;
}	/* end of function sip_parseSdpAttr */

		

/****************************************************************************
** FUNCTION: sip_parseSdpKey
** DESCRIPTION: This function parses the key field of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				ppKey(OUT) 		- The pointer to the output parsed key field 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpKey(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SIP_S8bit **ppKey, SipError *pErr)
{
	SIP_S8bit	*pCurrentPos = pStart, *pToken;
	SIP_U32bit	increment = 0;
	SIP_U32bit	tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpKey");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in SDP line: whitespace is not \
			allowed arnd '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
/* Changes for CSR_1-5833665 Starts*/
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Neither CR nor LF present at end of k= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
		increment = 1;

		if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif

	/*
	 * Look for a key-type of uri. If found, parse it 
	 * but still store as a string.
	 */

	if (0 == sip_strncmp(pCurrentPos, (SIP_S8bit *) "uri:", 4))
	{
		SipAddrSpec	*pTmpAddrSpec;

		pCurrentPos += 4;
		if (SipFail == sip_parseAddrSpec(pCurrentPos, (pToken - tempnewline)- 1, 
			&pTmpAddrSpec, pErr))/* Changes for CSR_1-5833665 */
		{
			sip_error(SIP_Minor, "Parsing addr-spec fld of k= line failed");
			return SipFail;
		}
		/*begin SPR1_0_16*/
		sip_freeSipAddrSpec(pTmpAddrSpec);
		/*end SPR1_0_16*/
	}

	*ppKey = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, (pToken- tempnewline) - pCurrentPos, pErr);/* Changes for CSR_1-5833665 */
/* Changes for CSR_1-5833665 ends*/
	/*
	 * Advance current parser position to the beginning of the next line. 
	 */

	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;
	
	SIPDEBUGFN("Exiting from sip_parseSdpKey");
	return SipSuccess;	

} /* end of function sip_parseSdpKey */



/****************************************************************************
** FUNCTION: sip_parseSdpMediaDescription
** DESCRIPTION: This function parses a media descriptor of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pMediaList(OUT) 	- The pointer to the media list of the SDP 
**								  message into which the parsed media descriptor
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseSdpMediaDescription(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	en_SipBoolean connInfoAtSessionLevel, SIP_S8bit **ppNewCurrentPos, \
	SipList *pMediaList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken, *pTemp;
	SIP_S8bit		*pNextToken;
	SIP_U32bit		temp, tmpPort;
	SdpMedia		*pMedia;
	en_SipBoolean	connInfoAtMediaLevel;
	SIP_U32bit		increment = 0;
	SIP_U32bit		tempnewline = 0;

	SIPDEBUGFN("Entering into sip_parseSdpMediaDescription");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		sip_error(SIP_Minor, "Syntax error in parsing SDP line: whitespace is \
			not allowed arnd the '='");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/* 
	 * Init media field
	 */

	if (SipFail == sip_initSdpMedia(&pMedia, pErr))
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
		
	/*
	 * Look for media-field.
	 */
	
	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	pMedia->pMediaValue = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos, pErr);
	pCurrentPos = pToken+1;

	/*
	 * Look for port-fld.
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
		goto error;

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	/*
	 * Check if port-num is present.
	 */

	pNextToken = sip_tokenize(pCurrentPos, pToken - 1, '/');
	if (SIP_NULL != pNextToken)
	{
		/*
		 * port-num present.
		 */

		if (SipFail == sip_atoi(pNextToken + 1, pToken - 1, &temp))	
		{
			sip_error(SIP_Minor, "Non-numeric port-num fld in m= line");
			goto error;
		}
		pMedia->pPortNum = (SIP_U32bit *) fast_memget(DECODE_MEM_ID, \
			sizeof(SIP_U32bit), pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pMedia->pPortNum)
			goto error;
#endif
		*pMedia->pPortNum = temp;
	}

	/*
	 * Validate and store port-fld.
	 */

	if (SipFail == sip_atoi(pCurrentPos, (SIP_NULL == pNextToken)?(pToken - 1):\
		(pNextToken - 1), (SIP_U32bit *)&tmpPort))	
	{
		sip_error(SIP_Minor, "Non-numeric port fld in m= line");
		goto error;
	}
	pMedia->dPort = (SIP_U16bit)tmpPort;
	pCurrentPos = pToken+1;
		
	/*
	 * Store protocol-field
	 */

	pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
	if (SIP_NULL == pToken)
	{
		/*
		 * At least one format field must be present
		 */
		
		sip_error(SIP_Minor, "At least one format fld must be present in m= \
			line");
		goto error;
	}

	/* CSR 1-7008527 changes start */	
	if ( *(pToken+1) == ' ')
         goto error;
	/* CSR 1-7008527 changes end */	

	pMedia->pProtocol = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pToken - pCurrentPos,pErr);
	pCurrentPos = pToken + 1;

	/*
	 * Store format field
	 */
/* Changes for CSR_1-5833665 Starts*/
	pToken = sip_tokenize_sdp_attr(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
                        sip_freeSdpMedia(pMedia);
			sip_error(SIP_Minor, "Neither CR nor LF present at end of m= line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
		if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif


	pCurrentPos = sip_stripLeadingLWS(pCurrentPos, (pToken - tempnewline)- 1);/* Changes for CSR_1-5833665 */
	pTemp = sip_stripTrailingLWS(pCurrentPos, (pToken - tempnewline)- 1);/* Changes for CSR_1-5833665 */
/* Changes for CSR_1-5833665 Ends*/
	if (pCurrentPos > pTemp)
	{
		/*
		 * No format field present;
		 * raise error
		 */
		sip_error(SIP_Minor, "Format fld not present in media descriptor");
		goto error;
	}
	pMedia->pFormat = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
		pTemp - pCurrentPos + 1,pErr);
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pMedia->pFormat)
	{
		*pErr = E_NO_MEM;
		goto error;
	}
#endif
	pCurrentPos = pToken + increment;

	/* 
	 * parse info field if present
	 */
/*CSR 1-4735601*/
	if ((*pCurrentPos == 'i') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpSessionNameOrSessionInfo(pCurrentPos, pEnd, \
			ppNewCurrentPos, &(pMedia->pInformation), SipFalse, pErr))
		{
			sip_error(SIP_Minor, "Error in parsing info line in media \
				descriptor");
			goto error;
		}
		pCurrentPos = *ppNewCurrentPos;
	}


	/*
	 * parse connection lines if present
	 */

	connInfoAtMediaLevel = SipFalse;
	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'c') && (*(pCurrentPos+1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			SdpConnection *pConnInfo;

			pCurrentPos+=2;
			if (SipFail == sip_parseSdpConnectionInfo(pCurrentPos, pEnd, \
				ppNewCurrentPos, &pConnInfo, pErr))
			{
				sip_error(SIP_Minor, "Error in c= line in media descriptor");
				goto error;
			}
			if (SipSuccess != (sip_listAppend(&(pMedia->slConnection), \
				(SIP_Pvoid)pConnInfo, pErr)))
				goto error;

			connInfoAtMediaLevel = SipTrue;
			pCurrentPos = *ppNewCurrentPos;
		}
		else
			break;
	}

	if ((SipFalse == connInfoAtMediaLevel) && \
		(SipFalse == connInfoAtSessionLevel))
	{
		/* 
		 * No connection info at either session-level or media-level 
		 * for this media descriptor; raise parser error
		 */

		sip_error(SIP_Minor, "No connection info at either session-level \
			or media-level for this media descriptor");
		goto error;
	}

	/* 
	 * Parse bandwidth fields if any present
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'b') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpBandwidthInfo(pCurrentPos, pEnd, \
				ppNewCurrentPos, &(pMedia->slBandwidth), pErr))
				goto error;
			pCurrentPos = *ppNewCurrentPos;
		}
		else 
			break;
	} 

	/* 
	 * parse key field if present
	 */
/*CSR 1-4735601*/
	if ((*pCurrentPos == 'k') && (*(pCurrentPos+1) == '=') && \
			(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
	{
		pCurrentPos+=2;
		if (SipFail == sip_parseSdpKey(pCurrentPos, pEnd, \
			ppNewCurrentPos, &(pMedia->pKey), pErr))
		{
			sip_error(SIP_Minor, "Error in parsing k= line");
			goto error;
		}
		pCurrentPos = *ppNewCurrentPos;
	}

	/* 
	 * Parse attributes if any
	 */

	for(;;)
	{
/*CSR 1-4735601*/
		if ((*pCurrentPos == 'a') && (*(pCurrentPos + 1) == '=') && \
				(pCurrentPos<=pEnd))
/*CSR 1-4735601*/
		{
			pCurrentPos+=2;
			if (SipFail == sip_parseSdpAttr(pCurrentPos, pEnd, \
				ppNewCurrentPos, &(pMedia->slAttr), pErr))
				goto error;
			pCurrentPos = *ppNewCurrentPos;
		}
		else 
			break;
	} 

	/*
	 * Store this media descriptor into the SipMessage
	 */

	if (SipSuccess != (sip_listAppend(pMediaList,(SIP_Pvoid)pMedia, pErr)))
			goto error;

	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseSdpMediaDescription");
	return SipSuccess;
error:

		sip_freeSdpMedia(pMedia);
		sip_error(SIP_Minor,"Error in sip_parseSdpMediaDescription\n");
		*pErr = E_PARSER_ERROR;
		return SipFail;
} /* end of function sip_parseSdpMediaDescription */			


#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_parseMimeEntity
** DESCRIPTION: This function parses a MIME message. Only multipart/mixed are
**				supported. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				pMimeMsg(OUT) 	- The pointer to the output Mime message 
**				pContext(IN)	- The pointer to the Event Context; required
**								  while parsing MIME hdrs if any.
**				pOpt(IN)		- The Options bit_mask	
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseMimeEntity(SIP_U16bit dLevel, SIP_S8bit *pStart, \
	SIP_S8bit *pEnd, SipMessage *pSipMesg, MimeMessage *pMimeMsg, \
	SipEventContext *pContext, SipOptions *pOpt, SipMimeHeader *pMHdr, SipError *pErr)	
{
	SIP_S8bit			*pCurrentPos = pStart;
	SIP_S8bit			*pBoundary = SIP_NULL;
	SIP_S8bit			*pTmpBoundary = SIP_NULL;
	SIP_S8bit			*pToken;
	en_SipMsgBodyType	contentType = SipUnknownBody;
	SIP_U32bit			boundaryValSize = 0, i;
	en_SipBoolean		boundaryValFound;
	SIP_S8bit			*pStartOfBody;
	SIP_S8bit			*pEndOfBody;
	SipListElement 		*pListElem;
	SipList				*pTmpList = SIP_NULL;
	SipMimeHeader		*pTmpMimeHdr = SIP_NULL;
	SipContentTypeHeader *pPtr=SIP_NULL;

	SIPDEBUGFN("Entering into sip_parseMimeEntity");
	/*
	 * Retrieve the boundary value parameter contained in the
	 * Content-Type header.
	 */

	if (SIP_NULL == pMHdr)
		pPtr = pSipMesg->pGeneralHdr->pContentTypeHdr;
	else
		pPtr = pMHdr->pContentType;

	if ((SIP_NULL == pPtr) || \
		(SIP_NULL == pPtr->slParam.head))
	{
		sip_error(SIP_Minor, "Unable to retrieve boundary value parameter");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	pTmpList = &(pPtr->slParam);
	for (pListElem = pTmpList->head; pListElem != SIP_NULL; \
		pListElem = pListElem->next)
	{
		if (0 == sip_strncmp(((SipParam *)(pListElem->pData))->pName, \
			(SIP_S8bit *) "boundary", sip_strlen((SIP_S8bit *) "boundary")))
		{
			/*
			 * Boundary param found; extract its value.
			 */
	
			if (SipFail == sip_listGetAt(&(((SipParam *)(pListElem->pData))->\
				slValue), 0, (SIP_Pvoid *)&pTmpBoundary, pErr))
			{
				sip_error(SIP_Minor, \
					"Unable to retrieve boundary value parameter");
				return SipFail;
			}
			break;
		}
	}

	if (SIP_NULL == pTmpBoundary)
	{
		/*
		 * Boundary Value parameter is necessary for parsing MIME messages.
		 * Raise parser error.
		 */

		sip_error(SIP_Minor, "Unable to retrieve boundary value parameter");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}	
pBoundary = (SIP_S8bit *)fast_memget\
		(DECODE_MEM_ID,sizeof(SIP_S8bit)*strlen(pTmpBoundary)+1,pErr);
	if(pBoundary==SIP_NULL)
		return SipFail;
	if(pTmpBoundary[0]=='\"')
	{
		strcpy(pBoundary,&(pTmpBoundary[1]));
		pBoundary[strlen(pTmpBoundary) - 2] = '\0';
	}
	else
		strcpy(pBoundary,pTmpBoundary);
	boundaryValSize = sip_strlen(pBoundary);

	/*
	 * Iterate through the buffer looking for "--" followed
	 * by boundary value. Anything before the boundary val is
	 * preamble text; ignore it.
	 */
		
	boundaryValFound=SipFalse;
	while ((pCurrentPos+boundaryValSize+1) <= pEnd)
	{
		if ((*pCurrentPos == '-') && (*(pCurrentPos+1) == '-'))
		{
			SIP_S8bit	*pTempPtr = pCurrentPos + 2;

			boundaryValFound=SipTrue;
			for (i=0; i < boundaryValSize; i++)	
			{
				if (*pTempPtr != pBoundary[i])
				{
					boundaryValFound = SipFalse;
					break;
				}
				pTempPtr++;
			}
			if (SipTrue == boundaryValFound)
				break;
			else
				pCurrentPos++;
		}
		else
			pCurrentPos++;
	} /* end of while-loop */
	
	if (SipFalse == boundaryValFound)
	{
                fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);		
		sip_error(SIP_Minor, "Error in parsing MIME body: no boundary value \
			found");
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
	pCurrentPos += (boundaryValSize + 1); 
			
	/*
	 * pCurrentPos will now be pointing to the last char of 
	 * the dash-boundary.
	 */

	while (1)
	{
		pToken = sip_tokenize(pCurrentPos, pEnd, '\r');
		if ((SIP_NULL == pToken) || ((pToken+1) > pEnd) || \
			(*(pToken+1) != '\n'))
		{
                        fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);	
			sip_error(SIP_Minor, "Error in parsing MIME body: No CRLF at the end \
				of boundary line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}

		/*
		 * pCurrentPos was pointing to last char of dash-boundary;
		 * now update it to point to first char after boundary line.
		 */

		pCurrentPos = pToken+2;
		if (pCurrentPos > pEnd)
		{
			fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);
			sip_error(SIP_Minor, "Incomplete MIME body");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	
		if (*pCurrentPos != '\r')
		{
           SIP_S8bit *pStartOfEntityBody = SIP_NULL;

			/* 
			 * Body headers are present.
			 */

			if (SipFail == sip_initSipMimeHeader(&pTmpMimeHdr, pErr))
            {
                /* klocworks : free the previously allocated dynamic memories */
                fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);
                sip_error(SIP_Minor, "Error in Init MIME Header");
                *pErr = E_PARSER_ERROR;
                return SipFail;
            }

			if (SipFail == sip_parseSipBuffer(pCurrentPos, &pSipMesg, pOpt, \
				(pEnd - pCurrentPos + 1), &pStartOfEntityBody, pContext, \
				pTmpMimeHdr, pErr))
			{
				sip_error(SIP_Minor, "Error in parsing MIME headers");
				goto error;
			}	  

			/*
			 * sip_parseMimeHeaders will return the start of the body
			 * part of the entity. Update pCurrentPos to point to this.
			 */

			pCurrentPos = pStartOfEntityBody;

			/* 
			 * Retrieve Content-Type if any parsed for the body-part 
			 * to which these headers pertain.
			 */

			if (SIP_NULL != pTmpMimeHdr->pContentType)
			{
                                 if ((0 == sip_strcasecmp(pTmpMimeHdr->pContentType->pStr1, \
                                        (SIP_S8bit *) "application/sdp")) && \

					(sip_strlen((SIP_S8bit *) "application/sdp") == \
					sip_strlen(pTmpMimeHdr->pContentType->pStr1)))

					contentType = SipSdpBody;
                                 else if ((0 == sip_strcasecmp(pTmpMimeHdr->pContentType->pStr1, \
                                         (SIP_S8bit *) "message/sipfrag"))         && \
                			(sip_strlen((SIP_S8bit *) "message/sipfrag") == \
					sip_strlen(pTmpMimeHdr->pContentType->pStr1)))
				
					contentType = SipAppSipBody;
                                 else if ((0 == sip_strcasecmp(pTmpMimeHdr->pContentType->pStr1, \
                                       (SIP_S8bit *) "application/isup")        ) && \
					(sip_strlen((SIP_S8bit *) "application/isup") == \
					sip_strlen(pTmpMimeHdr->pContentType->pStr1)))

					contentType = SipIsupBody;
#ifdef SIP_MWI
                                 else if ((0 == sip_strncmp(pTmpMimeHdr->pContentType->pStr1, \
                                                 (SIP_S8bit *) "application/simple-message-summary", \
                                                 sip_strlen((SIP_S8bit *) \
                                                     "application/simple-message-summary"))) && \
                                         (sip_strlen((SIP_S8bit *) \
                                                     "application/simple-message-summary") == \
                                          sip_strlen(pTmpMimeHdr->pContentType->pStr1)))

                                     contentType = SipMessageSummaryBody;
#endif
                                 else if ((0 == sip_strcasecmp(pTmpMimeHdr->pContentType->pStr1, \
                                        (SIP_S8bit *) "multipart/mixed"))         && \
					(sip_strlen((SIP_S8bit *) "multipart/mixed") == \
					sip_strlen(pTmpMimeHdr->pContentType->pStr1)))

					contentType = SipMultipartMimeBody;
/* FSS_ICF_CHANGES_START*/
				else if ((0 == sip_strcasecmp(pTmpMimeHdr->pContentType->pStr1, \
						(SIP_S8bit *) "multipart/related")) && \
						(sip_strlen((SIP_S8bit *) "multipart/related") == \
						sip_strlen(pTmpMimeHdr->pContentType->pStr1)))
					contentType = SipMultipartMimeBody;
				/* FSS_ICF_CHANGES_END*/
				else
					contentType = SipUnknownBody;
			}
		}
		else
		{
			/*
			 * No body headers; move to the beginning of the body-part.
			 */

			pCurrentPos+=2;
		}

		/*
		 * pCurrentPos now points to the start of the body-part
		 */

		pStartOfBody = pCurrentPos;
	
		/*
		 * Iterate thru' the remaining buffer looking for a linebreak
		 * followed by a dash-boundary.
		 */
	
		boundaryValFound = SipFalse;
		while ((pCurrentPos+boundaryValSize+1) <= pEnd)
		{
			if ((SIP_NULL != pCurrentPos) && (*pCurrentPos == '-') && (*(pCurrentPos+1) == '-'))
			{
				SIP_S8bit	*pTempPtr = pCurrentPos + 2;
				
				boundaryValFound = SipTrue;
				for (i=0; i < boundaryValSize; i++)	
				{
					if (*pTempPtr != pBoundary[i])
					{
						boundaryValFound = SipFalse;
						break;
					}
					pTempPtr++;
				}
				if (SipTrue == boundaryValFound)
					break;
				else
					pCurrentPos++;
			}
			else
				pCurrentPos++;
		} /* end of while-loop */
	
		if (SipFalse == boundaryValFound)
		{
			sip_error(SIP_Minor, "Incomplete MIME body: no close delimiter"
				"found");
			goto error;
		}	

		if ((*(pCurrentPos - 1) != '\n') || (*(pCurrentPos - 2) != '\r') ||\
	   		((pCurrentPos+boundaryValSize+3) > pEnd)) /* 3 or 4 ?? */
		{
			sip_error(SIP_Minor, "Error in parsing MIME body: boundary "
				"delimiter 	does not occur at the beginning of a line, or"
				"incomplete delimiter line");
			goto error;
		}
		pCurrentPos += (boundaryValSize + 1);
		if ((*(pCurrentPos+1) == '-') && (*(pCurrentPos+2) == '-'))
		{
			/*
			 * close-delimiter found; this is the last body part. 
			 * The CRLF at the beginning of the delimiter line is
			 * not conceptually a part of the body part itself
			 * (refer RFC 2046), so calculate body size accordingly.
			 */

			pEndOfBody = pCurrentPos - (boundaryValSize + 4);
			if (SipFail == sip_parseSingleMimeEntity(dLevel, contentType, \
				pStartOfBody, pEndOfBody, pSipMesg, pMimeMsg, pContext, pOpt, \
				pTmpMimeHdr, pErr))
			goto error;
			/*
			 * Reset pTmpMimeHdr and contentType.
			 */

			pTmpMimeHdr = SIP_NULL;
			contentType = SipUnknownBody;
			break; /* break out of while-loop */	
		}
		else
		{
			/*
			 * not a close-delimiter, another body-part present;
			 * repeat above processing.
			 */
				
			pEndOfBody = pCurrentPos - boundaryValSize - 4; 
			if (SipFail == sip_parseSingleMimeEntity(dLevel, contentType, \
				pStartOfBody, pEndOfBody, pSipMesg, pMimeMsg, pContext, pOpt, \
				pTmpMimeHdr, pErr))
			goto error;
			pCurrentPos --;
			/*
			 * Reset pTmpMimeHdr and contentType.
			 */

			pTmpMimeHdr = SIP_NULL;
			contentType = SipUnknownBody;
			continue;
		} /* end of else  if not a close delimiter */
	} /* end of while EOB not reached */

	fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);
	SIPDEBUGFN("Exiting from sip_parseMimeEntity");
	return SipSuccess;
error:	
                        fast_memfree(DECODE_MEM_ID,pBoundary,SIP_NULL);
			sip_bcpt_freeSipMimeHeader(pTmpMimeHdr);
			*pErr = E_PARSER_ERROR;
			sip_error(SIP_Minor ,"sip_parseMimeEntity \n");
			return SipFail;
} /* end of function sip_parseMimeEntity */
#endif
		
/****************************************************************************
** FUNCTION: sip_parseSdpAddress
** DESCRIPTION: This function parses the address field of an o= line. 
** 
** PARAMETERS: None.
**
** NOTE: Currently, this field is not parsed i.e. any non-NULL str is allowed. 
****************************************************************************/

SipBool	sip_parseSdpAddress(void)
{
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_parseSdpConnectionAddress
** DESCRIPTION: This function parses the connection-address field of an 
**				SDP c= line.	
** 
** PARAMETERS: None.
**
** NOTE: Currently, this function will always return success. 
****************************************************************************/

SipBool sip_parseSdpConnectionAddress(void)
{
	return SipSuccess;
}
				

/****************************************************************************
** FUNCTION: sip_parseSdpEmailAddrOrPhoneNum
** DESCRIPTION: This function parses the e= and p= lines of an SDP message. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse 
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pEmailList(OUT) 	- The pointer to the output email/phonenum list
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_parseSdpEmailAddrOrPhoneNum(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pEmailList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart, *pToken;
	SIP_S8bit		*pEmail;
	SIP_U32bit		increment = 0;
	SIP_U32bit		tempnewline = 0;
	SIPDEBUGFN("Entering into sip_parseSdpEmailAddrOrPhoneNum");
	if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
		(*pCurrentPos != '\n'))
	{
		/* 
		 * white space is not allowed around the '='
		 */

		*pErr = E_PARSER_ERROR;
		return SipFail;
	}

	/*
	 * No explicit parsing done for email
	 * addresses and phone numbers. Just 
	 * store the line as a string.
	 */
/* Changes for CSR_1-5833665 Starts*/
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
#if 1	
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Neither CR nor LF present at end of line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	else
	{
		/*
	 	* CR may or may not be followed by LF.
	 	*/
			increment = 1;
		if ('\r' == *(pToken - 1))
		tempnewline = 1;
	}
#endif
	pEmail = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, (pToken - tempnewline)- pCurrentPos, pErr);/* Changes for CSR_1-5833665 */
	
/* Changes for CSR_1-5833665 Ends */
#ifndef SIP_NO_CHECK
	if (SIP_NULL == pEmail)
	{
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
#endif
	
	if (SipSuccess != (sip_listAppend(pEmailList, (SIP_Pvoid)pEmail, pErr)))
	{	
		(void)fast_memfree(0, pEmail, pErr);
		*pErr = E_PARSER_ERROR;
		return SipFail;
	}
		
	pCurrentPos = pToken + increment;
	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseSdpEmailAddrOrPhoneNum");
	return SipSuccess;

} /* end of function sip_parseSdpEmailAddrOrPhoneNum */
#ifdef SIP_MWI
/****************************************************************************
** FUNCTION: sip_parseMwiMessage
** DESCRIPTION: This function parses an MWI message. It internally invokes
**				various sub-parsers for parsing the different MWI Msg body Headers. 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				pSummaryMessage(OUT)- The pointer to the output MWI message 
**				pOpt(IN)		- The Options bit_mask	
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
		
SipBool	sip_parseMwiMessage(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	MesgSummaryMessage *pSummaryMessage, SipOptions *pOpt, SipError *pErr)
{
	SIP_S8bit	        *pCurrentPos = pStart;
	SIP_S8bit	       	*pToken = SIP_NULL;
	SipNameValuePair    *slNameValue = SIP_NULL;
	SIP_U32bit          dIndex = 0;
	SIP_U32bit          dSummaryIndex = 0;
	/*
	 * pNewCurrentPos variable is set by all the sub-parsers 
	 * before returning, to indicate to the parent function the 
	 * position at which they have stopped parsing.
	 */
	SIP_S8bit			*pNewCurrentPos = SIP_NULL;

	SIPDEBUGFN("Entering into sip_parseMwiMessage");
	(void)pOpt;

	/*
	 * Parse Status Line 
	 */
	if (0 == sip_strncasecmp(pStart, (SIP_S8bit *) "Messages-Waiting:",17))
	{
		pCurrentPos += 17;
		SIP_SKIPSPACE(pCurrentPos);	
		if (0 == sip_strncasecmp(pCurrentPos, (SIP_S8bit *) "Yes",3)) 
		{
			pSummaryMessage->dStatus = SipMsgWaitingYes;
			pCurrentPos += 3;
		}
		else if(0 == sip_strncasecmp(pCurrentPos, (SIP_S8bit *) "No",2))
		{
			pSummaryMessage->dStatus = SipMsgWaitingNo;
			pCurrentPos += 2; 
		}
		else
		{
			sip_error(SIP_Minor, "Syntax error in Mwi message:" \
					" Parsing Status line fld failed");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pCurrentPos++;
		/* To Check if any new line is before Message-Account header */
		pToken = sip_tokenize(pCurrentPos, pCurrentPos+1, '\n');
		if(pToken != SIP_NULL)
			pCurrentPos++;

	}
	else
	{
		sip_error(SIP_Minor, "Syntax error in Mwi message:" \
				" Parsing Status line fld failed");
		*pErr = E_PARSER_ERROR;
		return SipFail;

	}

	/* 	
	 * Parse Message Account
	 */
	if(pCurrentPos != pEnd)
	{
		if (0 == sip_strncasecmp(pCurrentPos, (SIP_S8bit *) "Message-Account:", 16))
		{
			SipAddrSpec	*pTmpAddrSpec;

			pCurrentPos += 16;
			SIP_SKIPSPACE(pCurrentPos);
			pToken = sip_tokenize(pCurrentPos, pEnd, '\r');
			if (SIP_NULL == pToken)
			{
				pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
				if (SIP_NULL == pToken)
				{
					/* 
					 * Neither CR nor LF present at the end of line.
					 */
					sip_error(SIP_Minor, "Neither CR nor LF present at end ");
					*pErr = E_PARSER_ERROR;
					return SipFail;
				}

				if (SipFail == sip_parseAddrSpec(pCurrentPos, pToken - 1, 
							&pTmpAddrSpec, pErr))
				{
					sip_error(SIP_Minor, "Parsing addr-spec fld failed");
					return SipFail;
				}
				pSummaryMessage->pAddrSpec = pTmpAddrSpec;
			}
			pCurrentPos = pToken+1; 
			pToken++;
		}
	}

	/* Parse Summary Line details*/
	if(pCurrentPos != pEnd)
	{
			if(*pCurrentPos == '\n')
							pCurrentPos++;

			for(;;)
			{
					if(*pCurrentPos == '\n')
							pCurrentPos++;

					pToken = sip_tokenize(pCurrentPos, pEnd, '/');
					if(pToken != SIP_NULL)
					{
							if(sip_parseMwiSummaryLine(pCurrentPos, pEnd, &pNewCurrentPos, \
													pSummaryMessage, dSummaryIndex, pErr) == SipFail)
							{
									sip_error(SIP_Minor, "Syntax error in Mwi message: " 
													"error in SummaryLine"); 
									*pErr = E_PARSER_ERROR;
									return SipFail;
							}
							pCurrentPos = pNewCurrentPos;
							dSummaryIndex++;

					}
					else 
							break;
			}

	}
	/* Store the name value pair*/

	if(pCurrentPos != pEnd)
	{
		while((pToken = sip_tokenize(pCurrentPos, pEnd, ':')) != SIP_NULL)	
		{
			/* Init the name value pair structure */

			if(sip_initSipNameValuePair(&slNameValue, \
						(SipError*)pErr) == SipFail) 
			{
				return SipFail;
			}
			pToken = sip_tokenize(pCurrentPos, pEnd, ':');
			if(pToken == SIP_NULL)	
			{
				sip_error(SIP_Minor, "name value fld failed");
				return SipFail;
			}
			SIP_SKIPSPACE(pCurrentPos);
			if(*pCurrentPos == '\n')
			pCurrentPos++;
			slNameValue->pName = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
					pToken-pCurrentPos, pErr);

			pCurrentPos = pToken + 1;

			pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
			if(pToken == SIP_NULL)
			{
				sip_error(SIP_Minor, "name value fld failed");
				return SipFail;
			}
			SIP_SKIPSPACE(pCurrentPos);
			slNameValue->pValue = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
					pToken-pCurrentPos, pErr);

			if( sip_listInsertAt( &(pSummaryMessage->slNameValue), dIndex,   \
						(SIP_Pvoid) slNameValue, pErr) == SipFail)
			{
				return SipFail;
			}
			dIndex++;

			pCurrentPos = pToken + 1;
		}
	}
	SIPDEBUGFN("Exiting from sip_parseMwiMessage");
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_parseMwiSummaryLine
** DESCRIPTION: This function parses the summary line field of an MWI message.
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pSummaryMessage - The message summary structure
**				dSummaryIndex - The Sumamry line Index
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseMwiSummaryLine(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, MesgSummaryMessage *pSummaryMessage, \
  SIP_U32bit dSummaryIndex, SipError *pErr)

{
	SIP_S8bit	        *pCurrentPos = pStart;
	SIP_S8bit			*pToken = SIP_NULL;
	SIP_S8bit			*pTemp = SIP_NULL;
	SummaryLine         *pSummaryLine;
	SIP_U32bit	        dtemp = 0;

	SIPDEBUGFN("Entering into sip_parseMwiSummaryLine");
	if(sip_mwi_initSummaryLine(&pSummaryLine,pErr) == SipFail)
	{
		return SipFail;
	}
	pToken = sip_tokenize(pCurrentPos, pEnd, '\r');
	if (SIP_NULL == pToken)
	{
		pToken = sip_tokenize(pCurrentPos, pEnd, '\n');
		if (SIP_NULL == pToken)
		{
			/* 
			 * Neither CR nor LF present at the end of line.
			 */
			sip_error(SIP_Minor, "Neither CR nor LF present at end ");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
	}

	/* Parse and store the media in summary line  */
	pToken = sip_tokenize(pCurrentPos, pEnd, ':');
	if(pToken == SIP_NULL)
	{
		__sip_mwi_freeSummaryLine(pSummaryLine);
		sip_error(SIP_Minor, "Parsing Media in Summaryline fld failed");
		return SipFail;
	}
	else
	{
		pSummaryLine->pMedia = sip_nStrdup(DECODE_MEM_ID,  \
				pCurrentPos, pToken - pCurrentPos, pErr);
	}
	pCurrentPos = pToken+2; 

	/* Remove any LWS present in the summaryline*/
	SIP_SKIPSPACE(pCurrentPos);
	if(*pCurrentPos == '\n')
			pCurrentPos++;

	/* Parse and store the New Messages Count in summary line */
	pToken = sip_tokenize(pCurrentPos, pEnd, '/');
	if(pToken == SIP_NULL)
		
	{
		__sip_mwi_freeSummaryLine(pSummaryLine);
		sip_error(SIP_Minor, "Parsing new msgs in Summaryline " \
				"fld failed");
		return SipFail;
	}
	else
	{
		if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &dtemp))	
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor, "Non-numeric count for new msgs " \
					"in summary line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		} 
		pSummaryLine->newMessages = dtemp;
	}
	pCurrentPos= pToken+1; 

	/* 
	 * Check the urgent messages count is available or not.
	 * If it is not available store the Old Messages Count, and
	 * proceed to next line, else parse the urgent messages count and
	 * proceed.
	 */
	SIP_SKIPSPACE(pCurrentPos);

	pToken = sip_tokenize(pCurrentPos, pEnd, '(');
	if(pToken != SIP_NULL)
		
	{
		pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
		if (SipFail == sip_atoi(pCurrentPos,pTemp,&dtemp))	
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor,"Non-numeric count for old msgs" \
					"in summary line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pSummaryLine->oldMessages = dtemp;
		pCurrentPos = pToken+1; 

		pToken = sip_tokenize(pCurrentPos, pEnd, '/');
	  if(pToken == SIP_NULL)		
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor, "Parsing old msgs in " \
					"Summaryline fld failed");
			return SipFail;
		}      
		else
		{
			/*  Parse and store the New Urgent Messages Count " \
				"in summary line.*/
			pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
			if (SipFail == sip_atoi(pCurrentPos, pTemp, &dtemp))	
			{
				__sip_mwi_freeSummaryLine(pSummaryLine);
				sip_error(SIP_Minor, "Non-numeric count for new" \
						"urgent msgs in summary line");
				*pErr = E_PARSER_ERROR;
				return SipFail;
			}
			pSummaryLine->newUrgentMessages = dtemp;
		}
		pCurrentPos= pToken+1; 
		pToken = sip_tokenize(pCurrentPos, pEnd, ')');
	  if(pToken == SIP_NULL)			
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor, "Parsing new urgent msgs in " \
					"Summaryline fld failed");
			return SipFail;
		}
		/*Parse and store the Old Urgent Messages Count in summaryline*/
		if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &dtemp))	
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor, "Non-numeric count for old " \
					"urgent msgs in summary line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}
		pSummaryLine->oldUrgentMessages = dtemp;
		pCurrentPos= pToken+1; 

	}
	else if((pToken = sip_tokenize(pCurrentPos, pEnd, '\n')) == SIP_NULL)
	{
		__sip_mwi_freeSummaryLine(pSummaryLine);
		sip_error(SIP_Minor, "Parsing old urgent msgs in " \
				"Summaryline fld failed");
		return SipFail;
	}
	else
	{
		/* Parse and store the Old Messages Count in summary line */
		pTemp = sip_stripTrailingLWS(pCurrentPos, pToken - 1);
		if (SipFail == sip_atoi(pCurrentPos, pTemp, &dtemp))	
		{
			__sip_mwi_freeSummaryLine(pSummaryLine);
			sip_error(SIP_Minor, "Non-numeric count for old" \
					"msgs in summary line");
			*pErr = E_PARSER_ERROR;
			return SipFail;
		}

		pSummaryLine->oldMessages = dtemp;
	}
	if( sip_listInsertAt( &(pSummaryMessage->slSummaryLine), dSummaryIndex,\
				(SIP_Pvoid) pSummaryLine, pErr) == SipFail)
	{
		__sip_mwi_freeSummaryLine(pSummaryLine);
		return SipFail;
	}
	pCurrentPos= pToken+1;

	if(pCurrentPos != pEnd) 
		if(*pCurrentPos == '\n')
			pCurrentPos++;

	*ppNewCurrentPos = pCurrentPos;

	SIPDEBUGFN("Exiting from sip_parseMwiSummaryLine");
	return SipSuccess;
}

#endif

#ifdef SIP_MIME_PARSING
/****************************************************************************
** FUNCTION: sip_parseSingleMimeEntity
** DESCRIPTION: This function parses one body-part of a MIME message. Invoked
**				from within sip_parseMimeEntity.
** 
** PARAMETERS:
**				dLevel(IN)		- A number (1/0) indicating whether or not
**								  this is the first call to this function. 
**								  The first level of a MIME message is broken
**								  up into its constituent parts and stored rather
**								  than being stored in the pMimeMsg fld of the
**								  SipMessage. Nested entities if any are however
**								  stored in the pMimeMsg fld.
**				contentType(IN) - The contentType for this body-part.
**				pStartOfBody(IN)- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEndOfBody(IN)	- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  body-part).
**				pSipMesg(IN/OUT)- The pointer to the Sip Message
**				pMimeMsg(OUT) 	- The pointer to the output Mime message 
**				pOpt(IN)		- The Options bit_mask	
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/
SipBool sip_parseSingleMimeEntity(SIP_U16bit dLevel, \
	en_SipMsgBodyType contentType, SIP_S8bit *pStartOfBody, \
	SIP_S8bit *pEndOfBody, SipMessage *pSipMesg, MimeMessage *pMimeMsg, \
	SipEventContext *pContext, SipOptions *pOpt, SipMimeHeader *pMimeHdr, \
	SipError *pErr)	
{
	SipBool	retVal;

	SIPDEBUGFN("Entering into sip_parseSingleMimeEntity");
	if (0 == dLevel)
	{
		SipMsgBody	*pMsgBody = SIP_NULL; 

		switch (contentType)
		{
			case SipUnknownBody:
			case SipAppSipBody:
#ifdef SIP_ISUP
			case SipIsupBody:
#endif
			{
				en_SipBoolean	flag = SipFalse;

				if (SipUnknownBody == contentType)
					flag = SipTrue;
#ifdef SIP_ISUP
				if (SipFail == sip_initSipMsgBody(&pMsgBody, \
					(flag?SipUnknownBody:SipIsupBody), pErr))
#else
				if (SipFail == sip_initSipMsgBody(&pMsgBody, \
					SipUnknownBody, pErr))
#endif
				{
					return SipFail;
				}
#ifdef SIP_ISUP
				if (flag)
#endif
				{
					if (SipFail == sip_initSipUnknownMessage(&(pMsgBody->u.\
						pUnknownMessage), pErr))
						goto error;
/*SPR- 19314 fixed*/
/* CSR_1-6887463 Workaround Start */
					if(pEndOfBody > pStartOfBody )
					{
/* CSR_1-6887463 Workaround End */
					pMsgBody->u.pUnknownMessage->dLength = pEndOfBody	\
						- pStartOfBody + 1;
					pMsgBody->u.pUnknownMessage->pBuffer = \
						(SIP_S8bit *) fast_memget(DECODE_MEM_ID, \
							pMsgBody->u.pUnknownMessage->dLength, pErr);
                    if(SIP_NULL == pMsgBody->u.pUnknownMessage->pBuffer)
                        goto error;

					memcpy(pMsgBody->u.pUnknownMessage->pBuffer, pStartOfBody, \
						pMsgBody->u.pUnknownMessage->dLength);
/* CSR_1-6887463 Workaround Start */
				}
				else
						pMsgBody->u.pUnknownMessage->dLength = 0;
/* CSR_1-6887463 Workaround End */
				}
#ifdef SIP_ISUP
				else
				{
					if (SipFail == sip_initIsupMessage(&(pMsgBody->u.\
						pIsupMessage), pErr))
						goto error;
                    /* CSR_1_7192585 Merged SPR 19672 Start */
           	        /* CSR_1-7194850 Change Start */
					if(pEndOfBody > pStartOfBody)
					{
                   /* CSR_1-7194850 Change End */
                   /* CSR_1_7192585 Merged SPR 19672 End */
					    pMsgBody->u.pIsupMessage->dLength = pEndOfBody	\
						    - pStartOfBody + 1;
				    	pMsgBody->u.pIsupMessage->pBody = \
						    (SIP_S8bit *) fast_memget(DECODE_MEM_ID, \
							 pMsgBody->u.pIsupMessage->dLength, \
							 pErr);
                        if(SIP_NULL == pMsgBody->u.pIsupMessage->pBody)
                            goto error;

                        memcpy(pMsgBody->u.pIsupMessage->pBody, pStartOfBody, \
                                pMsgBody->u.pIsupMessage->dLength);
                    /* CSR_1_7192585 Merged SPR 19672 Start */
                    /* CSR_1-7194850 Change Start */
                    }
                    else
                        pMsgBody->u.pIsupMessage->dLength = 0;
                    /* CSR_1-7194850 Change End */
                    /* CSR_1_7192585 Merged SPR 19672 End */

                }

#endif

				pMsgBody->pMimeHeader = pMimeHdr;
				if (SipSuccess != (sip_listAppend(&(pSipMesg->\
					slMessageBody), (SIP_Pvoid)pMsgBody, pErr)))
						goto error;
			}
			break;

			case SipSdpBody:
				if (SipFail == sip_initSipMsgBody(&pMsgBody, \
					SipSdpBody, pErr))
				{
					return SipFail;
				}
				if (SipFail == sip_initSdpMessage(&(pMsgBody->u.\
					pSdpMessage), pErr))
						goto error;
				retVal = sip_parseSdpMessage(pStartOfBody, pEndOfBody, \
					pMsgBody->u.pSdpMessage, pOpt, pErr);
				if (SipSuccess != retVal)
						goto error;
				pMsgBody->pMimeHeader = pMimeHdr;
				if (SipSuccess != (sip_listAppend(&(pSipMesg->\
					slMessageBody), (SIP_Pvoid)pMsgBody, pErr)))
						goto error;
				break;
#ifdef SIP_MWI

            case SipMessageSummaryBody:
				if (SipFail == sip_initSipMsgBody(&pMsgBody, \
					SipMessageSummaryBody, pErr))
				{
					return SipFail;
				}
				if (SipFail == sip_mwi_initMesgSummaryMessage(&(pMsgBody->u.\
					pSummaryMessage), pErr))
						goto error;
				retVal = sip_parseMwiMessage(pStartOfBody,\
                         /* CSR_1_7192585 Merged SPR 19672 Start */
                         /* CSR_1-6887463 Change Start */
						 pEndOfBody+2, 
                         /* CSR_1-6887463 Change End */
                         /* CSR_1_7192585 Merged SPR 19672 End */ \
					pMsgBody->u.pSummaryMessage, pOpt, pErr);
				if (SipSuccess != retVal)
						goto error;
				pMsgBody->pMimeHeader = pMimeHdr;
				if (SipSuccess != (sip_listAppend(&(pSipMesg->\
					slMessageBody), (SIP_Pvoid)pMsgBody, pErr)))
						goto error;
				break;

#endif
#ifdef SIP_MIME_PARSING
			case SipMultipartMimeBody:
				/* 
				 * When parseMimeEntity is called with 1, 
				 * we initSipMsg before calling it.
				 */

				if (SipFail == sip_initSipMsgBody(&pMsgBody, \
					SipMultipartMimeBody, pErr))
				{
					return SipFail;
				}
				if (SipFail == sip_initMimeMessage(&(pMsgBody->u.\
					pMimeMessage), pErr))
						goto error;
				pMsgBody->pMimeHeader = pMimeHdr;
				retVal = sip_parseMimeEntity(1, pStartOfBody, pEndOfBody, \
					pSipMesg, pMsgBody->u.pMimeMessage, pContext, pOpt,
					pMimeHdr, pErr);	
				if (SipFail == retVal)
				{
					pMsgBody->pMimeHeader = SIP_NULL; 
					goto error;
				}		
				if (SipSuccess != (sip_listAppend(&(pSipMesg->\
					slMessageBody), (SIP_Pvoid)pMsgBody, pErr)))
				{	
					pMsgBody->pMimeHeader = SIP_NULL; 
					goto error;
				}		
				break;
#endif
			default:
				break;
		} /* end of switch */

	SIPDEBUGFN("Exiting from sip_parseSingleMimeEntity");
	return SipSuccess;
error:
			sip_freeSipMsgBody(pMsgBody);
			sip_error(SIP_Major ," sip_parseSingleMimeEntity Failed\n");
			return SipFail;
	} /* end of if dLevel is 0 */ 
	else
	{
		/*
		 * dLevel is 1; this is a nested MIME entity
		 * and will be filled into the MIME message field
		 * of the SipMsg.
		 */

		SipMsgBody	*pMsgBody2 = SIP_NULL;

		if (SIP_NULL == pMimeMsg)
		{
			*pErr = E_INV_PARAM;
			return SipFail;
		}

		switch (contentType)
		{
            /* klocworks: separate switch cases have been done for 
             * SipUnknownBody and SipIsupBody , so as to avoid any
             * confusion on when to release Unknown Body and when
             * Isup body
             */ 
			case SipUnknownBody:
                if (SipFail == sip_initSipMsgBody(&pMsgBody2, \
                            SipUnknownBody, pErr))
                {
                    return SipFail;
                }

                if (SipFail == sip_initSipUnknownMessage(&(pMsgBody2->u.\
                                pUnknownMessage), pErr))
                    /* CSR_1_7192585 Merged CSR 19672 Start */
                    goto error2;
                /* CSR_1-7194850  Change Start */
                if(pEndOfBody > pStartOfBody )
                {
                    /* CSR_1-7194850  Change End */
                    /* CSR_1_7192585 Merged CSR 19672 End */
                    pMsgBody2->u.pUnknownMessage->dLength = pEndOfBody \
                                                            - pStartOfBody + 1;
                    pMsgBody2->u.pUnknownMessage->pBuffer = \
                                          (SIP_S8bit *) fast_memget(DECODE_MEM_ID, \
                                          pMsgBody2->u.pUnknownMessage->dLength, \
                                          pErr);
                    if(SIP_NULL == pMsgBody2->u.pUnknownMessage->pBuffer)
                        goto error2;

                    memcpy(pMsgBody2->u.pUnknownMessage->pBuffer, \
                            pStartOfBody, pMsgBody2->u.pUnknownMessage->dLength);
                    /* CSR_1_7192585 Merged CSR 19672 Start */
                    /* CSR_1-7194850  Change Start */
                }
                else
                    pMsgBody2->u.pUnknownMessage->dLength = 0;
                /* CSR_1-7194850  Change End */
                /* CSR_1_7192585 Merged CSR 19672 End */ 

                pMsgBody2->pMimeHeader = pMimeHdr;
                if (SipSuccess != (sip_listAppend(&(pMimeMsg->\
                                    slRecmimeBody), (SIP_Pvoid)pMsgBody2, \
                                pErr)))
                    goto error2;
                retVal = SipSuccess;
                break;

#ifdef SIP_ISUP
			case SipIsupBody:
                if (SipFail == sip_initSipMsgBody(&pMsgBody2, \
                            SipIsupBody, pErr))
                {
                    return SipFail;
                }

                if (SipFail == sip_initIsupMessage(&(pMsgBody2->u.\
                                pIsupMessage), pErr))
                    goto error2;
                /* CSR_1_7192585 Merged CSR 19672 Start */
                /* CSR_1-6887463 Change Start */
                if(pEndOfBody > pStartOfBody )
                {
                    /* CSR_1-6887463 Change End */
                    /* CSR_1_7192585 Merged CSR 19672 End */
                    pMsgBody2->u.pIsupMessage->dLength = pEndOfBody	\
                                                         - pStartOfBody + 1;
                    pMsgBody2->u.pIsupMessage->pBody = \
                              (SIP_S8bit *) fast_memget(DECODE_MEM_ID, \
                              pMsgBody2->u.pIsupMessage->dLength, \
                              pErr);

                    if(SIP_NULL == pMsgBody2->u.pIsupMessage->pBody)
                        goto error2;

                    memcpy(pMsgBody2->u.pIsupMessage->pBody, pStartOfBody, \
                            pMsgBody2->u.pIsupMessage->dLength);
                    /* CSR_1_7192585 Merged CSR 19672 Start */
                    /* CSR_1-6887463 Change Start */
                }
                else
                    pMsgBody2->u.pIsupMessage->dLength = 0;
                /* CSR_1-6887463 Change End */
                /* CSR_1_7192585 Merged CSR 19672 End */
                pMsgBody2->pMimeHeader = pMimeHdr;
                if (SipSuccess != (sip_listAppend(&(pMimeMsg->\
                                    slRecmimeBody), (SIP_Pvoid)pMsgBody2, \
                                pErr)))
                    goto error2;
                retVal = SipSuccess;
                break;
#endif

			case SipSdpBody:
				if (SipFail == sip_initSipMsgBody(&pMsgBody2, \
					SipSdpBody, pErr))
                /* CSR_1_7192585 Merged CSR 19672 Start */ 
                /* CSR_1-6887463 Change Start */
                {
                    return SipFail;
                }	
                /* CSR_1-6887463 Change End */	
                /* CSR_1_7192585 Merged CSR 19672 End */			
				if (SipFail == sip_initSdpMessage(&(pMsgBody2->u.\
					pSdpMessage), pErr))
                /* CSR_1_7192585 Merged CSR 19672 Start */
                /* CSR_1-6887463 Change Start */
				    goto error2;
                /* CSR_1-6887463 Change End */
                /* CSR_1_7192585 Merged CSR 19672 Start */
				retVal = sip_parseSdpMessage(pStartOfBody, pEndOfBody, \
					pMsgBody2->u.pSdpMessage, pOpt, pErr);
				if (SipFail == retVal)
				goto error2;
				pMsgBody2->pMimeHeader = pMimeHdr;
				if (SipSuccess != (sip_listAppend(&(pMimeMsg->\
					slRecmimeBody), (SIP_Pvoid)pMsgBody2, \
					pErr)))
				goto error2;
				break;
#ifdef SIP_MWI
            case SipMessageSummaryBody:
				if (SipFail == sip_initSipMsgBody(&pMsgBody2, \
					SipMessageSummaryBody, pErr))
                /* CSR_1_7192585 Merged CSR 19672 Start */ 
                /* CSR_1-6887463 Change Start */
                {
                    return SipFail;
                }	
                /* CSR_1-6887463 Change End */	
                /* CSR_1_7192585 Merged CSR 19672 End */
				if (SipFail == sip_mwi_initMesgSummaryMessage(&(pMsgBody2->u.\
					pSummaryMessage), pErr))
                /* CSR_1_7192585 Merged CSR 19672 Start */
                /* CSR_1-6887463 Change Start */
				    goto error2;
                /* CSR_1-6887463 Change End */
                /* CSR_1_7192585 Merged CSR 19672 Start */
				retVal = sip_parseMwiMessage(pStartOfBody, pEndOfBody, \
					pMsgBody2->u.pSummaryMessage, pOpt, pErr);
				if (SipFail == retVal)
				goto error2;
				pMsgBody2->pMimeHeader = pMimeHdr;
				if (SipSuccess != (sip_listAppend(&(pMimeMsg->\
					slRecmimeBody), (SIP_Pvoid)pMsgBody2, \
					pErr)))
				goto error2;
				break;
#endif
			case SipMultipartMimeBody:
				if (SipFail == sip_initSipMsgBody(&pMsgBody2, \
					SipMultipartMimeBody, pErr))
				{
					return SipFail;
				}
				if (SipFail == sip_initMimeMessage(&(pMsgBody2->u.\
					pMimeMessage), pErr))
				goto error2;
				pMsgBody2->pMimeHeader = pMimeHdr;
				retVal = sip_parseMimeEntity(1, pStartOfBody, pEndOfBody, \
					pSipMesg, pMsgBody2->u.pMimeMessage, pContext, pOpt,
					pMimeHdr, pErr);	
				if (SipFail == retVal)
				{
						pMsgBody2->pMimeHeader = SIP_NULL; 
						goto error2;
				}		
				if (SipSuccess != (sip_listAppend(&(pMimeMsg->\
					slRecmimeBody), (SIP_Pvoid)pMsgBody2, \
					pErr)))
				{
					pMsgBody2->pMimeHeader = SIP_NULL; 
					goto error2;
				}	
				break;

			default:
				break;
		} /* end of switch */
	SIPDEBUGFN("Exiting from sip_parseSingleMimeEntity");
	return SipSuccess;
error2:
			sip_freeSipMsgBody(pMsgBody2);
			sip_error(SIP_Major, "sip_parseSingleMimeEntity Failed \n");
			return SipFail;
	} /* end of else */

} /* end of function sip_parseSingleMimeEntity */
#endif

/****************************************************************************
** FUNCTION: sip_parseTunnelledSdpTime
** DESCRIPTION: This function parses the time field of an SDP message.
**              The functionality of this function is similar to 
**              sip_parseSdpTime.The only difference is that repeat field ie
**              r= in sdp is also concatenated in the input string with comma
**              seperated value between repeat field and time field.
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse (the end of the
**								  message body as detected by sip_parseBuffer).
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pTimeList(OUT) 	- The pointer to the time list of the SDP 
**								  message into which the parsed time field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
****************************************************************************/
SipBool	sip_parseTunnelledSdpTime(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pTimeList, SipError *pErr)
{
    SIP_S8bit		*pCurrentPos = pStart;
    SIP_U32bit      temp = 0;
    SIP_S8bit       *pToken = SIP_NULL; 
    SdpTime			*pTime = SIP_NULL;
    SIP_U32bit		increment = 0;
    SipBool         dRepeatFieldOrZonePresent = SipFail;
    SipBool         dRepeatFieldPresent = SipFail;
    SipBool         dZoneFieldPresent = SipFail;

    SIPDEBUGFN("Entering into sip_parseTunnelledSdpTime");
    if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
            (*pCurrentPos != '\n'))
    {
        /*White space is not allowed around the '=' */
        sip_error(SIP_Minor, "Syntax error in SDP message: whitespace is \
                not allowed arnd '='");
        *pErr = E_PARSER_ERROR;
        return SipFail;
    }

    /*Init time field*/

    if (SipFail == sip_initSdpTime(&pTime, pErr))
    {
        *pErr = E_NO_MEM;
        return SipFail;
    }
		
    /* Look for start-time*/
    /* The two set of possible values which are to be parsed by this function
     * are:
     * Case1: t=0 0 (instead of 0 other value(1-9) can be possible.)
     * Case2: t=0 0,r=1234 (instead of 0 other value(1-9) can be possible.)
     */

    /*Get the position of space between 0 0*/
    pToken = sip_tokenize(pCurrentPos, pEnd, ' ');
    
	if (SIP_NULL == pToken)
		goto error;
    /*If the start value is 0*/
    if ((1 == (pToken - pCurrentPos)) && (*pCurrentPos == '0'))
	{
        pTime->pStart = sip_nStrdup(DECODE_MEM_ID, (SIP_S8bit *) "0", 1, pErr);
#ifndef SIP_NO_CHECK
        if (SIP_NULL == pTime->pStart)
            goto error;
#endif
	}
    /*If the start value is between 1 to 9*/
    else
    {
        if ((*pCurrentPos < '1') || (*pCurrentPos > '9'))
            goto error;
        if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))	
            goto error;
        if (100000000 > temp)
            goto error;
        pTime->pStart = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
                pToken - pCurrentPos, pErr);
#ifndef SIP_NO_CHECK
        if (SIP_NULL == pTime->pStart)
            goto error;
#endif
    }
    pCurrentPos = pToken + 1;

    /*Look for repeat field in t line */
    pToken = sip_tokenize(pCurrentPos, pEnd, ',');
    
    /* Value of pToken as NULL specifies that neither repeat field nor zone
     * filed is not present in t line.*/
    if (SIP_NULL == pToken)
    {
        /* Update the value of pToken field.This is done to handle the case1
         * scenario.*/
        pToken = pEnd;
    }
	else
	{
		increment = 1;
        /*Fix For SPR:20154*/
        /* Repeat-Field or Zone field is present */
        dRepeatFieldOrZonePresent = SipSuccess;
        /* If after "," ie pToken+1 if r field is present set the boolean
         * dRepeatFieldPresent to Success*/
        if('r' == *(pToken + 1))
        {
            dRepeatFieldPresent = SipSuccess;
        }
        /* Else If after "," ie pToken+1 if z field is present set the boolean
         * dRepeatFieldPresent to Success*/
        else if('z' == *(pToken + 1))
        {
            dZoneFieldPresent = SipSuccess;
        }   
    }
	/*If the stop value is 0*/
	if ((1 == (pToken - pCurrentPos)) && (*pCurrentPos == '0'))
	{
		pTime->pStop = sip_nStrdup(DECODE_MEM_ID, (SIP_S8bit *) "0", 1, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStop)
			goto error;
#endif
	}
    /*If the stop value is between 1 to 9*/
	else
	{
		if ((*pCurrentPos < '1') || (*pCurrentPos > '9'))
			goto error;
		if (SipFail == sip_atoi(pCurrentPos, pToken - 1, &temp))	
			goto error;
		if (100000000 > temp)
			goto error;
		pTime->pStop = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, \
			pToken - pCurrentPos, pErr);
#ifndef SIP_NO_CHECK
		if (SIP_NULL == pTime->pStop)
			goto error;
#endif
	}
	/* If repeat-field or Zone field is present, parse it*/
    if(SipSuccess == dRepeatFieldOrZonePresent)
    {
        /*Update the Current position*/
        pCurrentPos = pToken + increment;

        /*If the repeat filed is present parse the same and save it.*/
        if(SipSuccess == dRepeatFieldPresent)
        {
            for(;;)
            {
                /*Get the position of space between the next "," if present.*/
                pToken = sip_tokenize(pCurrentPos, pEnd, ',');
                if (('r' == *pCurrentPos) && ('=' == *(pCurrentPos + 1)))
                {	
                    pCurrentPos+=2;
                    /* Parse the repeat(r=) field.Use fuction
                     * sip_parseTunnelledSdpRepeatField as in this case 
                     * some string is still pending which need to be
                     * parsed.*/
                    if(SIP_NULL != pToken)
                    {
                        if (SipFail == sip_parseTunnelledSdpRepeatField(pCurrentPos,
                                    pToken - 1,ppNewCurrentPos, &(pTime->slRepeat),
                                    pErr))
                        {
                            goto error;
                        }
                        else
                        {
                            /* If the repeat field is successfully set in pTime
                             * variable update the */
                            pCurrentPos = pToken + 1;
                        }
                    }
                    /* If token field is NULL it means we have reached the last
                     * block of comma seperated values and thus in this case
                     * use function sip_parseSdpRepeatField to parse the
                     * repeat field.*/
                    else
                    {
                        if (SipFail == sip_parseSdpRepeatField(pCurrentPos,
                                    pEnd,ppNewCurrentPos, &(pTime->slRepeat),
                                    pErr))
                        {
                            goto error;
                        }
                        else
                        {
                            /*If this is the last block of comma seperated
                             * values it means application has not provided
                             * the zone filed. Set the value of
                             * dZoneFieldPresent to fail.*/
                            dZoneFieldPresent = SipFail;
                        }
                        break;
                    }
                }
                else
                {
                    /*If next block is of zone field  update the
                     * dZoneFieldPresent vaiable to Success. */
                    if('z' == *pCurrentPos)
                    {
                        dZoneFieldPresent = SipSuccess;
                    }
                    break;
                }
            } 
        }
        /*If the variable dZoneFieldPresent is set to SipSuccess parse the
         * zone field and set in pTime variable.*/
        if(SipSuccess == dZoneFieldPresent)
        {
            pToken = sip_tokenize(pCurrentPos, pEnd, ',');
            /* If comma is present it means there are still some block prsesent
             * which need to be parsed. This is incorrect as Z should be the
             * last parameter*/
            if(SIP_NULL != pToken)
                goto error;

            if(('z' == *pCurrentPos) && ('=' == *(pCurrentPos + 1)))
            {
                pCurrentPos+=2;
                if (SipFail == sip_parseSdpZone(pCurrentPos, pEnd, \
                    ppNewCurrentPos, &(pTime->pZone), pErr))
                    goto error;
            }            
        }
    }

    /*Append the value in TimeList*/
    if (SipSuccess != (sip_listAppend(pTimeList, (SIP_Pvoid)pTime, pErr)))
			goto error;

    SIPDEBUGFN("Exiting from sip_parseTunnelledSdpTime");
    return SipSuccess;
error:
    sip_freeSdpTime(pTime);
    sip_error(SIP_Minor ,"sip_parseTunnelledSdpTime\n");
    *pErr = E_PARSER_ERROR;
    return SipFail;
}

/* Fix For SPR:20154*/
/****************************************************************************
** FUNCTION: sip_parseTunnelledSdpRepeatField
** DESCRIPTION: This function parses the repeat field of an SDP message
**              Tunnelled mode. 
** 
** PARAMETERS:
**				pStart(IN)		- A pointer into the input buffer indicating the
**								  position from where this parser must start
**								  parsing. 
**				pEnd(IN)		- The position in the input buffer till 
**								  which this parser should parse .
**				ppNewCurrentPos(OUT) - The point at which this parser stops 
**									  taking input from the buffer
**				pRepList(OUT) 	- The pointer to the repeat list of the SDP 
**								  message into which the parsed repeat field
**								  will be appended. 
**				pErr(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool	sip_parseTunnelledSdpRepeatField(SIP_S8bit *pStart, SIP_S8bit *pEnd, \
	SIP_S8bit **ppNewCurrentPos, SipList *pRepList, SipError *pErr)
{
	SIP_S8bit		*pCurrentPos = pStart;
	SIP_S8bit		*pRep = SIP_NULL;

	SIPDEBUGFN("Entering into sip_parseTunnelledSdpRepeatField");

    if ((isspace((unsigned)(int)(*pCurrentPos))) && (*pCurrentPos != '\r') && \
            (*pCurrentPos != '\n'))
    {
        /* White space is not allowed around the '='*/
        sip_error(SIP_Minor, "Syntax error in SDP line: whitespace is not \
                allowed arnd '='");
        *pErr = E_PARSER_ERROR;
        return SipFail;
    }

    pRep = sip_nStrdup(DECODE_MEM_ID, pCurrentPos, pEnd - pCurrentPos, pErr);
    
    if (SIP_NULL == pRep)
    {
        return SipFail;
    }

	if (SipSuccess != (sip_listAppend(pRepList, (SIP_Pvoid)pRep, pErr)))
    {	
		(void)fast_memfree(0, pRep, pErr);
        return SipFail;
    }

	*ppNewCurrentPos = pEnd;

	SIPDEBUGFN("Exiting from sip_parseTunnelledSdpRepeatField");
	return SipSuccess;

} /* end of function sip_parseTunnelledSdpRepeatField*/


