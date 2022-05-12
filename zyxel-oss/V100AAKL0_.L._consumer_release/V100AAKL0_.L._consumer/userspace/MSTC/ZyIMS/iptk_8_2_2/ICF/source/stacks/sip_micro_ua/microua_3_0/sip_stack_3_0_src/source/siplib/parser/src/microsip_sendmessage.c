/***********************************************************************
 ** FUNCTION:
 **	       Send module
 *********************************************************************
 **
 ** FILENAME:
 ** microsip_sendmessage.c
 **
 ** DESCRIPTION:
 **		This file defines the API for sending a SipMessage over 
 **		the network. The API internally calls the functions for 
 **		encoding the SIP message, setting the retransmission timers, etc.
 **		Finally it invokes a callback on the client, which actually sends
 **		the message over the network.
 **
 ** DATE		NAME			REFERENCE		REASON
 ** ----		----			---------	    --------
 **22-03-2002	Mahesh			CoreStack			-
 **
 **	COPYRIGHT , Aricent, 2006
 *********************************************************************/

#include "microsip_struct.h"
#include "microsip_sendmessage.h"
#include "microsip_timer.h"
#include "microsip_init.h"
#include "microsip_statistics.h"
#include "microsip_trace.h"
#include "microsip_decode.h"
#include "microsip_cloneintrnl.h"

/****************************************************************************
** FUNCTION: sip_sendMessage
** DESCRIPTION: This function forms a SIP text message from the message 
**				structure and sends the same to the network. 
** 
** PARAMETERS:
**				s(IN)			- The SipMessage structure containing
**								  the SipMessage to be sent over the network.
**				options(IN)		- The SIP options for this message.
**				dAddr(IN)		- The destination Host IP Address + port number (in
**								  case of UDP) or the socket fd (in case of TCP)
**								  where the message is to be sent.
**				dTranspType(IN)	- The transport type(TCP/UDP) to be used.
**				pContext(IN)	- Pointer to Event Context structure.
**				err(OUT)		- Error value if any. 
**
** NOTE: 
****************************************************************************/

SipBool sip_sendMessage
#ifdef ANSI_PROTO
(
	SipMessage			*s,
	SipOptions			*options,
	SipTranspAddr		*dAddr,
	SIP_S8bit			dTranspType,
	SipEventContext		*pContext,
	SipError			*err
)
#else
	(s, options, dAddr, dTranspType, pContext, err)
	SipMessage *s;
	SipOptions *options;
	SipTranspAddr *dAddr;
	SIP_S8bit dTranspType;
	SipEventContext *pContext;
	SipError *err;
#endif
{
	SIP_S8bit		*out;
	SIP_U32bit		dLength;
#ifndef SIP_TXN_LAYER	
	SIP_U32bit		dRetransT1, count;
#endif	
#ifndef SIP_NO_CHECK

	if (s == SIP_NULL)
	{
		sip_error(SIP_Major, "SIP_ERROR: SIP Message is empty");
		INC_ERROR_NETWORK
		return SipFail;
	}
#endif
	/* Update statistics */
	INC_API_COUNT

	if (pContext != SIP_NULL)
	{
		pContext->dOptions.dOption = options->dOption;
	}
	/*
	 * Added Support for Direct Buffer
	 */
	if ((options->dOption & SIP_OPT_DIRECTBUFFER) == SIP_OPT_DIRECTBUFFER)
	{
		/*
		 * Use the pre-allocated buffer given by the application in the
		 * event context, to send over the network.
		 */
#ifndef SIP_NO_CHECK
		if (pContext == SIP_NULL)
		{
			sip_error (SIP_Major, "SIP_ERROR: Event context is empty");
			INC_ERROR_NETWORK
			return SipFail;
		}

		if (pContext->pDirectBuffer == SIP_NULL)
		{
			sip_error (SIP_Major,"SIP_ERROR: Buffer in event context is empty");
			INC_ERROR_NETWORK
			return SipFail;
		}
#endif

		/*
		 * assign the preallocated buffer
		 */
		out = (SIP_S8bit*) pContext->pDirectBuffer;
		dLength = sip_strlen(out);
	}
	else
	{
		/* 
		 * Direct-buffer option not enabled; allocate memory for forming
		 * message to send.
		 */

		out = (SIP_S8bit *) fast_memget(NETWORK_MEM_ID,SIP_MAX_MSG_SIZE,err);
		if (out == SIP_NULL)
			return SipFail;

		(void)sip_memset(out, 0, SIP_MAX_MSG_SIZE);

		/* 
		 * Form the Text Message to be sent.
		 */

		if (sip_formMessage(s, options, out, &dLength, err) == SipFail)
		{
			(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
			DEC_API_COUNT
			return SipFail;
		}
		/* Sipformmessage called internally, so do not increment User API count */
		DEC_API_COUNT
	}
	if (pContext != SIP_NULL)
		dAddr->pData = pContext->pData;
	else
		dAddr->pData = SIP_NULL;
	/*
	 * Invoke callback on the application to actually send the data over
	 * the network. 
	 */

	if (sip_sendToNetwork(out, dLength, dAddr, dTranspType, err) == SipFail)
	{
		if ((options->dOption & SIP_OPT_DIRECTBUFFER) != SIP_OPT_DIRECTBUFFER)
		{
			(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
		}
		sip_error(SIP_Major, "SIP_ERROR: Could not send to network ");
		INC_ERROR_NETWORK
		if (pContext != SIP_NULL)
			sip_freeEventContext(pContext);
		return SipFail;
	}

	/* 
	 * Update response/request sent statistics.
	 */

	if (s->dType == SipMessageResponse)
	{
		INC_API_RESP_SENT
	}
	else if (s->dType == SipMessageRequest)
	{
		INC_API_REQ_SENT
	}

#ifdef SIP_TRACE
	(void)sip_trace(SIP_Brief,SIP_Outgoing,(SIP_S8bit *) "Message sent out.");
	(void)sip_trace(SIP_Detailed,SIP_Outgoing,out);
#endif
	
	/* 
	 * Retransmission timer logic 
	 */

#ifndef SIP_TXN_LAYER	
	if ((s->dType == SipMessageRequest)&&((dTranspType&SIP_UDP)==SIP_UDP)\
		&&((dTranspType&SIP_NORETRANS)!=SIP_NORETRANS))
	{
		/* Message is a request sent by UDP and NO_RETRANS Option not used */
	 	if (sip_strcmp(s->u.pRequest->pRequestLine->pStr1,\
			(SIP_S8bit *)"ACK")!=0)
		{
			/* All requests except ACK  are retransmitted */
			SipTimerBuffer *tbuf;
			SipTimerKey		*pKey;

			/* Timer Buffer which stores the message Text and other stuff */
			if (sip_initSipTimerBuffer(&tbuf, err)==SipFail)
			{
				return SipFail;
			}
			/* Timer Key that'll identify this message in the timer list*/
			if (sip_initSipTimerKey(&pKey, err)!=SipSuccess)
				return SipFail;

			/* initialise count */
			tbuf->dRetransCount = 0;
			tbuf->enableRetransCallback = SipFail;

#ifdef SIP_RETRANSCALLBACK
			if ((options->dOption & SIP_OPT_RETRANSCALLBACK) \
				== SIP_OPT_RETRANSCALLBACK)
			{
				tbuf->enableRetransCallback = SipSuccess;
			}
#endif
			/* initial retransmission interval */
			if (((options->dOption & SIP_OPT_PERMSGRETRANS) == \
				SIP_OPT_PERMSGRETRANS) && (pContext != SIP_NULL))
			{
				/* User has specified retransmission intervals to be used for
				 * this message. Use that for the initial interval and store
				 * these values in the timer buffer for calculating
				 * subsequent intervals. 
				 */

				tbuf->dRetransT2 = pContext->dRetransT2;
				tbuf->dDuration = pContext->dRetransT1;
				dRetransT1 = pContext->dRetransT1;
				if ((options->dOption & SIP_OPT_PERMSGRETRANSCOUNT) ==\
					SIP_OPT_PERMSGRETRANSCOUNT)
				{
					tbuf->dMaxRetransCount = pContext->dMaxRetransCount;
					tbuf->dMaxInviteRetransCount = pContext->\
													dMaxInviteRetransCount;
				}
				else
				{
					tbuf->dMaxRetransCount = SIP_MAXU16BIT;
					tbuf->dMaxInviteRetransCount = SIP_MAXU16BIT;
				}
			}
			else
			{
				/* User wants to use default timer values.
				 * Set initial duration to default value.
				 * Set intervals in timer-buffer to 0 so that default values
				 * are used for subsequent interval calculations. 
				 */

				dRetransT1 = SIP_T1;
				tbuf->dDuration = SIP_T1;
				tbuf->dRetransT2 = 0;
				tbuf->dMaxRetransCount = SIP_MAXU16BIT;
				tbuf->dMaxInviteRetransCount = SIP_MAXU16BIT;
			}

			/* Flag to be set when informational response is received
			 * Retrans caps off for some reqeusts after informational
			 * Response.
			 */
			tbuf->dInfoReceived = SipFail;

			/* Set address and transport Type */
			tbuf->dAddr = *dAddr;
			tbuf->dTranspType = dTranspType;

			/* Message Text and Context provided by User */
			tbuf->pBuffer = out;
			tbuf->dBufferLength = dLength;
			tbuf->pContext = pContext;

			/* Timer pKey stuff - Callid, Method, Cseq */
			pKey->dMatchFlag = TIMER_NONE;
			pKey->dCallid = STRDUPNETWORK(s->pGeneralHdr->pCallidHdr->pStr1);
			if (pKey->dCallid == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			pKey->pMethod = STRDUPNETWORK(s->u.pRequest->pRequestLine->pStr1);
			if (pKey->pMethod == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			pKey->dCseq = s->pGeneralHdr->pCseqHdr->dIntVar1;
			pKey->dMatchType = SipMessageRequest;
#ifdef SIP_RPR		
			pKey->pRackHdr = SIP_NULL;
#endif
			/* Call leg from and to addresses */
			if (sip_initSipFromHeader(&(pKey->pFromHdr),err)==SipFail)
				return SipFail;
			if (__sipParser_cloneSipFromHeader(pKey->pFromHdr,\
				s->pGeneralHdr->pFromHeader, err) == SipFail)
				return SipFail;
			if (sip_initSipToHeader(&(pKey->pToHdr),err)==SipFail)
				return SipFail;
			if (__sipParser_cloneSipToHeader(pKey->pToHdr,\
				s->pGeneralHdr->pToHdr,err) == SipFail)
				return SipFail;

			/* Add branch from first Via Header */
			pKey->pViaBranch = SIP_NULL;

			/* Check if Via header present */
			if (sip_listSizeOf(&(s->pGeneralHdr->slViaHdr), &count, err)\
				!=SipFail)
			{
				if (count != 0)
				{
					SipViaHeader *pViaHeader;

					/* Get first via header from the list */
					if (sip_listGetAt(&(s->pGeneralHdr->slViaHdr), 0, \
						(SIP_Pvoid *) &pViaHeader , err) != SipFail)
					{
						SIP_S8bit *pViaBranch;

						/* Get Branch parameter */
						if (sip_getBranchFromViaHdr(pViaHeader, &pViaBranch,\
							err) !=SipFail)
						{
							pKey->pViaBranch = STRDUPNETWORK(pViaBranch);
							if (pKey->pViaBranch == SIP_NULL)
							{
								*err = E_NO_MEM;
								return SipFail;
							}
						}
					}
				}
			}


			/* Start the timer now - implementation in User space
			 * sip_processTimeOut(siptimer.c) will handle
			 * further retransmissions */
			if (SipFail == fast_startTimer( dRetransT1, 0, sip_processTimeOut, \
				(SIP_Pvoid) tbuf, pKey, err))
			{
				sip_freeSipTimerBuffer(tbuf);
				sip_freeSipTimerKey(pKey);
			}
			
		}
		else
		{
			if (pContext != SIP_NULL)
			sip_freeEventContext(pContext);
			if ((options->dOption & SIP_OPT_DIRECTBUFFER) != SIP_OPT_DIRECTBUFFER)
			{
				(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
			}
		}
	}
	else if ((s->dType == SipMessageResponse) && ((dTranspType&SIP_NORETRANS)\
		!=SIP_NORETRANS) && (s->u.pResponse->pStatusLine->dIntVar1 != \
		401) && (s->u.pResponse->pStatusLine->dIntVar1 != 407))
	{
		/* Response - retransmitted over UDP and TCP */
#ifdef SIP_RPR		
	 	if (((sip_strcmp(s->pGeneralHdr->pCseqHdr->pStr1,(SIP_S8bit *)"INVITE")==0)\
			&&(s->u.pResponse->pStatusLine->dIntVar1 >= 200))\
			||((s->u.pResponse->pResponseHdr->pRSeqHdr!=SIP_NULL)\
			&&(s->u.pResponse->pStatusLine->dIntVar1>100)\
			&&(s->u.pResponse->pStatusLine->dIntVar1<200)))
#else
	if (((sip_strcmp(s->pGeneralHdr->pCseqHdr->pStr1,(SIP_S8bit *)"INVITE")==0)\
			&&(s->u.pResponse->pStatusLine->dIntVar1 >= 200)))


#endif
		{
			/* All responses to INVITE with CodeNum >=200 are retransmitted
			 * Responses with CodeNum between 100 and 200 containing RSeq
			 * Header are also retransmitted.
			 */

			SipTimerBuffer *tbuf;
			SipTimerKey *pKey;

			if (sip_initSipTimerBuffer(&tbuf, err)==SipFail)
				return SipFail;
			if (sip_initSipTimerKey(&pKey, err)!=SipSuccess)
				return SipFail;

			/* Buffer and Key formation */
			tbuf->dRetransCount = 0;
			tbuf->enableRetransCallback = SipFail;
#ifdef SIP_RETRANSCALLBACK
			if ((options->dOption & SIP_OPT_RETRANSCALLBACK) \
				== SIP_OPT_RETRANSCALLBACK)
			{
				tbuf->enableRetransCallback = SipSuccess;
			}
#endif
			if (((options->dOption & SIP_OPT_PERMSGRETRANS) == \
				SIP_OPT_PERMSGRETRANS) && (pContext != SIP_NULL))
			{
				/* User has specified retransmission intervals to be used for
				 * this message. Use that for the initial interval and store
				 * these values in the timer buffer for calculating
				 * subsequent intervals.
				 */

				tbuf->dRetransT2 = pContext->dRetransT2;
				tbuf->dDuration = pContext->dRetransT1;
				dRetransT1 = pContext->dRetransT1;
				if ((options->dOption & SIP_OPT_PERMSGRETRANSCOUNT) ==\
					SIP_OPT_PERMSGRETRANSCOUNT)
				{
					tbuf->dMaxRetransCount = pContext->dMaxRetransCount;
					tbuf->dMaxInviteRetransCount = pContext->\
													dMaxInviteRetransCount;
				}
				else
				{
					tbuf->dMaxRetransCount = SIP_MAXU16BIT;
					tbuf->dMaxInviteRetransCount = SIP_MAXU16BIT;
				}
			}
			else
			{
				/* User wants to use default timer values.
				 * Set initial duration to default value.
				 * Set intervals in timer-buffer to 0 so that default values
				 * are used for subsequent interval calculations.
				 */

				tbuf->dDuration = SIP_T1;
				tbuf->dRetransT2 = 0;
				dRetransT1 = SIP_T1;
				tbuf->dMaxRetransCount = SIP_MAXU16BIT;
				tbuf->dMaxInviteRetransCount = SIP_MAXU16BIT;

			}
			tbuf->dInfoReceived = SipFail;
			tbuf->dAddr = *dAddr;
			tbuf->dTranspType = dTranspType;
			tbuf->pBuffer = out;
			tbuf->dBufferLength = dLength;
			tbuf->pContext = pContext;
			pKey->dCallid = STRDUPNETWORK(s->pGeneralHdr->pCallidHdr->pStr1);
			if (pKey->dCallid == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			pKey->pMethod = STRDUPNETWORK(s->pGeneralHdr->pCseqHdr->pStr1);
			if (pKey->pMethod == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			pKey->dCseq = s->pGeneralHdr->pCseqHdr->dIntVar1;
			pKey->dMatchType = SipMessageResponse;
			pKey->dCodeNum = s->u.pResponse->pStatusLine->dIntVar1;
#ifdef SIP_RPR
			if (pKey->dCodeNum < 200)
				pKey->dRseq = s->u.pResponse->pResponseHdr->pRSeqHdr->dIntVar1;
			pKey->pRackHdr = SIP_NULL;
#endif			
			if (sip_initSipFromHeader(&(pKey->pFromHdr),err)==SipFail)
				return SipFail;
			if (__sipParser_cloneSipFromHeader(pKey->pFromHdr,\
				s->pGeneralHdr->pFromHeader,\
				err) == SipFail)
				return SipFail;
			if (sip_initSipToHeader(&(pKey->pToHdr),err)==SipFail)
				return SipFail;
			if (__sipParser_cloneSipToHeader(pKey->pToHdr,\
				s->pGeneralHdr->pToHdr,err)\
				== SipFail)
				return SipFail;

			/* Add branch from first Via Header */
			pKey->pViaBranch = SIP_NULL;

			/* Start the timer now - implementation in User space
			 * sip_processTimeOut(siptimer.c) will handle
			 * further retransmissions.
			 */
			if (SipFail == fast_startTimer( dRetransT1, 0, sip_processTimeOut, \
				(SIP_Pvoid) tbuf, pKey, err))
			{
				sip_freeSipTimerBuffer(tbuf);
				sip_freeSipTimerKey(pKey);
			}
		}
		else
		{
			if (pContext != SIP_NULL)
				sip_freeEventContext(pContext);
			
			if ((options->dOption & SIP_OPT_DIRECTBUFFER) != SIP_OPT_DIRECTBUFFER)
			{
				(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
			}
		}
	}
	else
	{
		if (pContext != SIP_NULL)
			sip_freeEventContext(pContext);
		
		if ((options->dOption & SIP_OPT_DIRECTBUFFER) != SIP_OPT_DIRECTBUFFER)
		{
			(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
		}
	}
#endif	/* SIP_TXN_LAYER */
/* CSR 1-7239006 Change Start*/
#ifdef SIP_TXN_LAYER
			if ((options->dOption & SIP_OPT_DIRECTBUFFER) != SIP_OPT_DIRECTBUFFER)
			{
				(void)fast_memfree(NETWORK_MEM_ID,out,NULL);
			}
#endif
/* CSR 1-7239006 Change End */
	*err = E_NO_ERROR;
	return SipSuccess;
}
