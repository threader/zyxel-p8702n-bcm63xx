/***********************************************************************
 ** FUNCTION:
 **	       Timer Logic Implementaion
 *********************************************************************
 **
 ** FILENAME:
 **		microsip_timer.c
 **
 ** DESCRIPTION:
 **	This file contains the timer logic implementation
 **
 ** DATE		NAME			REFERENCE		REASON
 ** ----		----			---------		--------
 **22-03-2002	mahesh			CoreStack		Initial
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
#include "microsip_clone.h"
#include "microsip_accessors.h"


/*
Retransmission Count
Start at T1, keep resending till T2 is reached. 
After that send in intervals of T2. Stop when count reaches 11
if a provisional response is received, restransmit in steps of T2
*/
/*******************************************************************************
** FUNCTION: sip_getCharString
** DESCRIPTION: Converts escaped values in  a string to unescaped ones
** 
** PARAMETERS:
**      	      	inputstr(IN)			:The input string
**				 	outstr(OUT)				:The outputstring 
**				  	err(IN)					:The error Value
** RETURN VALUE:
**				  SipBool	
*******************************************************************************/
SipBool sip_getCharString
#ifdef ANSI_PROTO
	( SIP_S8bit	*inputstr,
	  SIP_S8bit	**outstr,
	  SipError	*err )
#else
        ( inputstr, outstr, err)
	  SIP_S8bit     *inputstr;
	  SIP_S8bit     **outstr;
	  SipError      *err;
#endif
{
	SIP_S32bit	val;
	SIP_S8bit	*outputstr, *tempstr, *temp;
	SIP_U32bit	dLength;

	/*
	 * Validating input params
	 */
	if( inputstr == SIP_NULL )
	{
		*err = E_INV_PARAM;
		return SipFail;
     }
		
	dLength = sip_strlen(inputstr);

	temp = (SIP_S8bit *)fast_memget(TIMER_MEM_ID, 5, err); 
	if(temp == SIP_NULL)
		return SipFail;
	tempstr = (SIP_S8bit *)fast_memget(TIMER_MEM_ID, dLength+1, err);
	if(tempstr == SIP_NULL)
	{
		(void)fast_memfree(TIMER_MEM_ID, temp, err);
		return SipFail;
	}
	outputstr = tempstr;

	while (*inputstr != '\0')
	{
		if (*inputstr == '%')
		{
			(void)sip_strncpy (temp, ++inputstr, 2);
			*(temp+2) = '\0';
			val = (SIP_S32bit) strtol(temp, 0, 16);
			*tempstr++ = val;
            inputstr++;
		}
		else
			*tempstr++ = *inputstr;
		inputstr++;
	}

	*tempstr = '\0';
	if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp), err) == SipFail)
		return SipFail;

	*outstr = outputstr;
	return SipSuccess;
} 



/*******************************************************************************
** FUNCTION: sip_isEqualPort
** DESCRIPTION: Compares two ports for their equality.
** 
** PARAMETERS:
**      	      	port1(IN)				:The port1
**      	      	port2(IN)				:The port2
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_isEqualPort
#ifdef ANSI_PROTO
	( SIP_U16bit	*port1, 
	  SIP_U16bit	*port2 )
#else
        ( port1, port2 )
	  SIP_U16bit	*port1;
	  SIP_U16bit    *port2;
#endif
{
	if( port1 == SIP_NULL && port2 == SIP_NULL )
	/*
	 * If both ports are null return success
	 */
		return SipSuccess;
	else
	{
		if( port1 == SIP_NULL && port2 != SIP_NULL )
		{
			/*
			 * Set the default port
			 */
			if( *port2 == 5060 )
				return SipSuccess;
			else
				return SipFail;
		}

		if( port2 == SIP_NULL && port1 != SIP_NULL )
		{
			if( *port1 == 5060 )
				return SipSuccess;
			else
				return SipFail;
		}
	}

	if( *port1 == *port2 )
		return SipSuccess;
	else
		return SipFail;
}


/*******************************************************************************
** FUNCTION: sip_isEqualSipUrl
** DESCRIPTION: Compares two SipUrls  for their equality.
** 
** PARAMETERS:
**      	      	inputstr(IN)			:The input string
**				 	outstr(OUT)				:The outputstring 
**				  	err(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_isEqualSipUrl
#ifdef ANSI_PROTO
	( SipUrl	*url1,
	  SipUrl	*url2,
	  SipError      *err )
#else
	( url1, url2, err )
	  SipUrl        *url1;
	  SipUrl        *url2;
	  SipError      *err;
#endif
{
	SIP_S8bit	*temp1, *temp2;
	SIP_S32bit		result;
	
	if( url1 == SIP_NULL || url2 == SIP_NULL )
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	/*
	 * Compare the user
	 */
	if((url1->pUser == SIP_NULL) && (url2->pUser != SIP_NULL))
		return SipFail;
	if((url2->pUser == SIP_NULL) && (url1->pUser != SIP_NULL))
		return SipFail;
	if((url1->pUser != SIP_NULL) && (url2->pUser != SIP_NULL))
	{
		if(sip_getCharString(url1->pUser, &temp1, err) == SipFail)
			return SipFail;
		if(sip_getCharString(url2->pUser, &temp2, err) == SipFail)
			return SipFail;
		result = sip_strcasecmp(temp1, temp2);
		if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp1), err) == SipFail )
			return SipFail;
		if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp2), err) == SipFail )
			return SipFail;
		if( result !=0 )	
			return SipFail;	
	}

	/*
	 * Compare the password
	 */
	if((url1->pPassword == SIP_NULL) && (url2->pPassword != SIP_NULL))
		return SipFail;
	if((url2->pPassword == SIP_NULL) && (url1->pPassword != SIP_NULL))
		return SipFail;
	if((url1->pPassword != SIP_NULL) && (url2->pPassword != SIP_NULL))
	{
    	if(sip_getCharString(url1->pPassword, &temp1, err) == SipFail)
			return SipFail;
	    if(sip_getCharString(url2->pPassword, &temp2, err) == SipFail)
			return SipFail;
		result = sip_strcmp(temp1, temp2);
        if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp1), err) == SipFail )
                return SipFail;
        if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp2), err) == SipFail )
                return SipFail;
        if( result != 0 )
		return SipFail;
	}
	/*
	 * Compare host
	 */
	if( sip_strcasecmp( url1->pHost, url2->pHost ) )
		return SipFail;

	/*
	 * Compare port
	 */
	if( !sip_isEqualPort( url1->dPort, url2->dPort) )
		return SipFail; 

	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_isEqualReqUri
** DESCRIPTION: Compares two ReqUris  for their equality.
** 
** PARAMETERS:
**      	      	uri1(IN)				:First uri
**				 	uri2(OUT)				:Second URi 
**				  	err(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_isEqualReqUri
#ifdef ANSI_PROTO
( SIP_S8bit	*uri1,
  SIP_S8bit	*uri2,
  SipError	*err )
#else
( uri1, uri2, err )
	SIP_S8bit*    uri1;
	SIP_S8bit*    uri2;
	SipError      *err;
#endif
{
	SIP_S8bit *temp1,*temp2;
	SIP_S32bit result;
	
	if( uri1 == SIP_NULL || uri2 == SIP_NULL )
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	/*
	 * Unescape teh uris and compare it case insensitively
	 */
	if(sip_getCharString(uri1, &temp1, err)==SipFail)
		return SipFail;
	if(sip_getCharString(uri2, &temp2, err)==SipFail)
		return SipFail;
	result = sip_strcasecmp(temp1, temp2);
	if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp1), err) == SipFail )
		return SipFail;
	if( sip_memfree(TIMER_MEM_ID, (SIP_Pvoid *)(&temp2), err) == SipFail )
		return SipFail;

	if(result !=0 )
		return SipFail;
	else 
		return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_compareToHeaders
** DESCRIPTION: Compares two To headers  for their equality.
** 
** PARAMETERS:
**      	      	hdr1(IN)				:First to header
**				 	hdr2(IN)				:Second toheader
**				  	err(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_compareToHeaders
#ifdef ANSI_PROTO
	( SipToHeader	*hdr1,
	  SipToHeader	*hdr2,
	  SipError	*err )
#else
	( hdr1, hdr2, err )
	  SipToHeader *hdr1;
	  SipToHeader *hdr2;
	  SipError      *err;
#endif
{
	SIP_S8bit *str1 = SIP_NULL, *str2 = SIP_NULL;

	if( hdr1 == SIP_NULL || hdr2 == SIP_NULL )
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	/*
	 * Comparing addrspec for equality
	 */
	if(((hdr1->pAddrSpec->dType == SipAddrSipUri) && \
		(hdr2->pAddrSpec->dType == SipAddrSipUri )) || \
	   ((hdr1->pAddrSpec->dType == SipAddrSipSUri) && \
		(hdr2->pAddrSpec->dType == SipAddrSipSUri )))		
	{
		if(sip_isEqualSipUrl( (hdr1->pAddrSpec->u).pSipUrl,\
			(hdr2->pAddrSpec->u).pSipUrl,err )!=SipSuccess)
			return SipFail;
	}
	else if( hdr1->pAddrSpec->dType == SipAddrReqUri &&
		hdr2->pAddrSpec->dType == SipAddrReqUri )
	{
		if(sip_isEqualReqUri( (hdr1->pAddrSpec->u).pUri,\
			(hdr2->pAddrSpec->u).pUri,err ) != SipSuccess)
			return SipFail;
	}
	else
		return SipFail;
	/*
	 * comparing tags
	 */
	if (SipFail == 	sip_getTagfromFromHeader(hdr1,&str1,err))
	{
		if (E_NO_EXIST != *err)	
			return SipFail;
	}
	if (SipFail == 	sip_getTagfromFromHeader(hdr2,&str2,err))
	{
		if (E_NO_EXIST != *err)	
			return SipFail;
	}

	if ((SIP_NULL == str1) && (SIP_NULL == str2))
		return SipSuccess;
	if ((SIP_NULL == str1) && (SIP_NULL != str2))
		return SipSuccess;
	if ((SIP_NULL == str2) && (SIP_NULL != str1))
		return SipFail;
#ifdef SIP_LINT
 	if ((SIP_NULL != str1) && (SIP_NULL != str2))
#endif
	if (sip_strcmp(str1,str2)!=0)
		return SipFail;
	
	return SipSuccess;
	
}	


/*******************************************************************************
** FUNCTION: sip_compareFromHeaders
** DESCRIPTION: Compares two From headers  for their equality.
** 
** PARAMETERS:
**      	      	hdr1(IN)				:First to header
**				 	hdr2(IN)				:Second toheader
**				  	err(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_compareFromHeaders
#ifdef ANSI_PROTO
	( SipFromHeader	*hdr1,
	  SipFromHeader	*hdr2,
	  SipError	*err )
#else
	( hdr1, hdr2, err )
	  SipFromHeader *hdr1;
	  SipFromHeader *hdr2;
	  SipError      *err;
#endif
{
	SIP_S8bit *str1 = SIP_NULL, *str2 = SIP_NULL;

	if ((hdr1 == SIP_NULL) || (hdr2 == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}

	if(((hdr1->pAddrSpec->dType == SipAddrSipUri) && \
	    (hdr2->pAddrSpec->dType == SipAddrSipUri )) || \
	   ((hdr1->pAddrSpec->dType == SipAddrSipSUri) && \
	    (hdr2->pAddrSpec->dType == SipAddrSipSUri )))
	{
		if(sip_isEqualSipUrl( (hdr1->pAddrSpec->u).pSipUrl,\
			(hdr2->pAddrSpec->u).pSipUrl,err )!=SipSuccess)
			return SipFail;
	}
	else if( hdr1->pAddrSpec->dType == SipAddrReqUri &&\
		hdr2->pAddrSpec->dType == SipAddrReqUri )
	{
		if(sip_isEqualReqUri( (hdr1->pAddrSpec->u).pUri,\
			(hdr2->pAddrSpec->u).pUri,err ) != SipSuccess)
			return SipFail;
	}
	else
		return SipFail;

	if (SipFail == 	sip_getTagfromFromHeader(hdr1,&str1,err))
	{
		if (E_NO_EXIST != *err)	
			return SipFail;
	}

	if (SipFail == 	sip_getTagfromFromHeader(hdr2,&str2,err))
	{
		if (E_NO_EXIST != *err)	
			return SipFail;
	}

	if ((SIP_NULL == str1) && (SIP_NULL == str2))
		return SipSuccess;
	
	if ((SIP_NULL == str1) || (SIP_NULL == str2))
		return SipFail;

	if (sip_strcmp(str1,str2)!=0)
		return SipFail;

	return SipSuccess;

}	


/*******************************************************************************
** FUNCTION: sip_getBranchFromViaHdr
** DESCRIPTION: gets Branch parameter from ViaHeader.
** 
** PARAMETERS:
**      	      	pViaHeader(IN)				:Via header from which 
**												 branch has to be retrieved
**				 	ppBranch(Out)				:Branch parameter retrieved
**				  	pErr(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getBranchFromViaHdr
#ifdef ANSI_PROTO
	(SipViaHeader *pViaHeader,\
	 SIP_S8bit **ppBranch,\
	 SipError *pErr)
#else
	(pViaHeader,ppBranch,pErr)
	SipViaHeader *pViaHeader;
	SIP_S8bit **ppBranch;
	SipError *pErr;
#endif
{
	SIP_U32bit index_1, size;

	*pErr = E_NO_ERROR;
	
	if (sip_listSizeOf(&(pViaHeader->slParam), &size, pErr) == SipFail)
		return SipFail;
	index_1 = 0;
	while (index_1 < size)
	{
		SipParam *pParam;
		
		if (sip_listGetAt(&(pViaHeader->slParam), index_1, ((SIP_Pvoid *) &pParam)\
			, pErr) == SipFail)
			return SipFail;
		if (pParam->pName != SIP_NULL)
		{
			if (sip_strcmp(pParam->pName, (SIP_S8bit *)"branch")==0)
			{
				SIP_S8bit *pBranch;

				if (sip_listGetAt(&(pParam->slValue),0,((SIP_Pvoid *) &pBranch),\
					pErr) != SipFail)
				{
					*ppBranch = pBranch;
					return SipSuccess;
				}
			}
		}
		index_1++;
	}
	return SipFail;
}

/*******************************************************************************
** FUNCTION: sip_compareTimerKeys
** DESCRIPTION: Compares two timer key structures.
** 
** PARAMETERS:
**      	      	paramkey(IN)				:The key which has to
** 												be compared with the stored key 
**				 	storedkey(INt)				:The stored timer key
**				  	pErr(IN)					:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_compareTimerKeys
#ifdef ANSI_PROTO
(SipTimerKey *paramkey, SipTimerKey *storedkey, SipError *err)
#else
(paramkey, storedkey, err)
	SipTimerKey *paramkey; 
	SipTimerKey *storedkey; 
	SipError *err;
#endif
{
	if(paramkey->dMatchFlag == TIMER_NONE)
		return SipFail;

	/* Check if any of the keys has to be matched independent of message type */
	if((paramkey->dMatchType!=SipMessageAny) && \
		(storedkey->dMatchType!=SipMessageAny))
	{
		/* Match keys only if message type matches exactly */
		if(paramkey->dMatchType != storedkey->dMatchType)
			return SipFail;	
	}

	/* Compare Call-Id first since it less expensive compared to From 
	   and To header comparisons. The expensive comparisons may be 
	   carried out only if this check succeeds  */
	if(sip_strcmp(paramkey->dCallid,storedkey->dCallid)!=0)
		return SipFail;

	/* Now compare From and To headers */
	if (sip_compareFromHeaders(paramkey->pFromHdr,storedkey->pFromHdr,err)\
		==SipFail)
		return SipFail;
	if(sip_compareToHeaders(storedkey->pToHdr,paramkey->pToHdr,err)==SipFail)
		return SipFail;

	/* Compare Via branch parameters */
	/* Do not perform Via branch checking for PRACK request matching */
	if(paramkey->dMatchFlag != TIMER_RPR)
	{
		/* Do not perform Via branch checking for responses sent */
		if((storedkey->dMatchType != SipMessageResponse))
		{
			if(((storedkey->pViaBranch!=SIP_NULL)\
				&&(paramkey->pViaBranch==SIP_NULL))\
				||((storedkey->pViaBranch==SIP_NULL)\
				&&(paramkey->pViaBranch!=SIP_NULL)))
				return SipFail;
			if((storedkey->pViaBranch != SIP_NULL)&&(paramkey->pViaBranch\
				!= SIP_NULL))
			{
				if(sip_strcmp(storedkey->pViaBranch, paramkey->pViaBranch)!=0)
					return SipFail;
			}
		}
	}
	
	switch (paramkey->dMatchFlag)
	{
		case TIMER_TRANSACTION:
			/* Perform CSeq comparisons */
			if(storedkey->dMatchType == SipMessageResponse)
				if(storedkey->dCodeNum < 200)
					return SipFail;
			if(paramkey->dCseq==storedkey->dCseq)
			{
				/* For ACK, only CSeq number is to be matched */
				/* For other methods, the CSeq method must match exactly */
				if((sip_strcmp(paramkey->pMethod, (SIP_S8bit *)"ACK")==0)) 
					return SipSuccess;
				else if((sip_strcmp(paramkey->pMethod,storedkey->pMethod)==0)) 
					return SipSuccess;
			}
			return SipFail;
#ifdef SIP_RPR
		case TIMER_RPR:
			/* Matching for reliable response retransmissions */
			/* CSeq number must be greater that of message being acknowledged */
			if(paramkey->dCseq <= storedkey->dCseq)
				return SipFail;

			/* CSequence number in the RAck header must match the CSeq number
			   of the message being acknowledged */
			if(paramkey->pRackHdr->dIntVar1 != storedkey->dCseq)
				return SipFail;

			/* Methods in the RAck header must match the CSeq method of the 
			   message being acknowledged */
			if(sip_strcmp(paramkey->pRackHdr->pStr1,storedkey->pMethod)!=0)
				return SipFail;

			/* If all the conditions above are met, the message acknowledges
			   all responses with RSeq sequence less than or equal to the 
			   response number in the RACK header .
               Changed to perform an exact match as required by 03 draft*/
			if(paramkey->pRackHdr->dIntVar2 != storedkey->dRseq)
				return SipFail;

			return SipSuccess;
#endif			
	}

	return SipFail;
}


/*******************************************************************************
** FUNCTION: sip_stopTimer
** DESCRIPTION: This function stops the timer.
** 
** PARAMETERS:
**      	      	msg(IN)		:The SipMessage
**			       pContext(IN)	:The event Context structure
**				  	err(IN)		:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_stopTimer
#ifdef ANSI_PROTO
(SipMessage *msg, SipEventContext *pContext, SipError *err)
#else
(msg, pContext, err)
	SipMessage *msg; 
	SipEventContext *pContext;
	SipError *err;
#endif
{
	SipTimerKey tkey,*outkey=SIP_NULL;
	SipTimerBuffer *tbuf;
	SIP_Pvoid outbuf=SIP_NULL;
	SIP_U32bit count;
		
	switch(msg->dType)
	{
		case SipMessageResponse:
			/* Decoded message is a response. Will probably terminate matching
			   request retransmissions */
			/* Form the timer key to be used for the cancellation */
			tkey.dCallid = STRDUPTIMER((msg->pGeneralHdr->pCallidHdr->pStr1));
			if(tkey.dCallid == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			tkey.pMethod = STRDUPTIMER(msg->pGeneralHdr->pCseqHdr->pStr1);
			if(tkey.pMethod == SIP_NULL)
			{
				*err = E_NO_MEM;
				return SipFail;
			}
			tkey.dCseq = msg->pGeneralHdr->pCseqHdr->dIntVar1;
			/* This key should match retransmission timers for requests only */
			tkey.dMatchType = SipMessageRequest;
			/* Matching comparison should follow TRANSACTION matching rules */
			tkey.dMatchFlag = TIMER_TRANSACTION;
			/* Clone the From and To headers into the timer key */
			if(sip_initSipFromHeader(&(tkey.pFromHdr),err)==SipFail)
				return SipFail;
			if(__sipParser_cloneSipFromHeader(tkey.pFromHdr,msg->pGeneralHdr->\
				pFromHeader,err) == SipFail)
				return SipFail;
			if(sip_initSipToHeader(&(tkey.pToHdr),err)==SipFail)
				return SipFail;
			if(__sipParser_cloneSipToHeader(tkey.pToHdr,msg->pGeneralHdr->\
				pToHdr,err) == SipFail)
				return SipFail;
			/* Add branch from first Via Header */
			tkey.pViaBranch = SIP_NULL;
			/* Check if Via header present */
			if(sip_listSizeOf(&(msg->pGeneralHdr->slViaHdr), &count, err) !=\
				SipFail)
			{
				if(count!=0)
				{
					SipViaHeader *pViaHeader;
	
					/* Get first via header from the list */
					if(sip_listGetAt(&(msg->pGeneralHdr->slViaHdr), 0, \
						(SIP_Pvoid *) &pViaHeader , err) != SipFail)
					{
						SIP_S8bit *pViaBranch;

						/* Get Branch parameter */
						if(sip_getBranchFromViaHdr(pViaHeader, \
							&pViaBranch, err) !=SipFail)
						{
							tkey.pViaBranch = STRDUPTIMER(pViaBranch);
							if(tkey.pViaBranch == SIP_NULL)
							{
								*err = E_NO_MEM;
								return SipFail;
							}
						}
					}
				}
			}

			/* Ask application to stop timers that match the formed key */
			tkey.pContext = pContext;
				if(fast_stopTimer(&tkey,&outkey,&outbuf,err)==SipFail)
			
			{
				(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
				(void)fast_memfree(TIMER_MEM_ID,tkey.pMethod,SIP_NULL);
				if(tkey.pViaBranch != SIP_NULL)
					(void)fast_memfree(TIMER_MEM_ID, tkey.pViaBranch, SIP_NULL);
				sip_freeSipFromHeader(tkey.pFromHdr);
				sip_freeSipToHeader(tkey.pToHdr);
				return SipFail;
			}
			sip_freeSipFromHeader(tkey.pFromHdr);
			sip_freeSipToHeader(tkey.pToHdr);
			(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
			(void)fast_memfree(TIMER_MEM_ID,tkey.pMethod,SIP_NULL);
			if(tkey.pViaBranch != SIP_NULL)
				(void)fast_memfree(TIMER_MEM_ID, tkey.pViaBranch, SIP_NULL);
			break;

		case SipMessageRequest:
			/* Request message. May cause some response retransmissions to
			   cease */
			if(sip_strcmp((SIP_S8bit *)"ACK",msg->u.pRequest->pRequestLine->\
				pStr1)==0)
			{
				/* ACK message, will end INVITE response retransmissions */
				/* Form timer key */
				tkey.dCallid=STRDUPTIMER(msg->pGeneralHdr->pCallidHdr->pStr1);
				if(tkey.dCallid == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
				tkey.pMethod=STRDUPTIMER(msg->pGeneralHdr->pCseqHdr->pStr1);
				if(tkey.pMethod == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
				tkey.dCseq=msg->pGeneralHdr->pCseqHdr->dIntVar1;
				/* This key should match retransmission timers for responses 
				   only */
				tkey.dMatchType = SipMessageResponse;
				/* Matching comparison should follow TRANSACTION matching 
				   rules */
				tkey.dMatchFlag = TIMER_TRANSACTION;
				/* clone from and to headers into the timer Key fields */
				if(sip_initSipFromHeader(&(tkey.pFromHdr),err)==SipFail)
					return SipFail;
				if(__sipParser_cloneSipFromHeader(tkey.pFromHdr,msg->\
					pGeneralHdr->pFromHeader,err) == SipFail)
					return SipFail;
				if(sip_initSipToHeader(&(tkey.pToHdr),err)==SipFail)
					return SipFail;
				if(__sipParser_cloneSipToHeader(tkey.pToHdr,msg->pGeneralHdr->\
					pToHdr,err) == SipFail)
					return SipFail;
				/* Add branch from first Via Header */
				tkey.pViaBranch = SIP_NULL;
				/* Check if Via header present */
				if(sip_listSizeOf(&(msg->pGeneralHdr->slViaHdr), &count, err)\
					!=SipFail)
				{
					if(count!=0)
					{
						SipViaHeader *pViaHeader;
		
						/* Get first via header from the list */
						if(sip_listGetAt(&(msg->pGeneralHdr->slViaHdr), 0, \
							(SIP_Pvoid *) &pViaHeader , err) != SipFail)
						{
							SIP_S8bit *pViaBranch;
							/* Get Branch parameter */
							if(sip_getBranchFromViaHdr(pViaHeader, &pViaBranch,\
								err) !=SipFail)
							{
								tkey.pViaBranch = STRDUPTIMER(pViaBranch);
								if(tkey.pViaBranch == SIP_NULL)
								{
									*err = E_NO_MEM;
									return SipFail;
								}
							}
						}
					}
				}
	
				/* Ask application to stop timers that match the formed key */
				tkey.pContext = pContext;
					if(fast_stopTimer(&tkey,&outkey,&outbuf,err)==SipFail)
				{
					(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
					(void)fast_memfree(TIMER_MEM_ID,tkey.pMethod,SIP_NULL);
					if(tkey.pViaBranch != SIP_NULL)
						(void)fast_memfree(TIMER_MEM_ID,tkey.pViaBranch,SIP_NULL);
					sip_freeSipFromHeader(tkey.pFromHdr);
					sip_freeSipToHeader(tkey.pToHdr);
					return SipFail;
				}
				sip_freeSipFromHeader(tkey.pFromHdr);
				sip_freeSipToHeader(tkey.pToHdr);
				(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
				(void)fast_memfree(TIMER_MEM_ID,tkey.pMethod,SIP_NULL);
				if(tkey.pViaBranch != SIP_NULL)
					(void)fast_memfree(TIMER_MEM_ID,tkey.pViaBranch,SIP_NULL);
			}
#ifdef SIP_RPR			
			else if(sip_strcmp((SIP_S8bit *)"PRACK",msg->u.pRequest->\
				pRequestLine->pStr1)==0)
			{
#ifdef SIP_INCREMENTAL_PARSING	
				/*
				 * If the flag for incremental parsing is enabled 
				 * check whether the header is parsed.If not parsed call the
				 * accessor api , to parse it.
				 */
				SipHeader *pRack = SIP_NULL;
				/*pRack->pHeader =  msg->u.pRequest->pRequestHdr->pRackHdr;*/
				if (msg->u.pRequest->pRequestHdr->pRackHdr->pBuffer\
					!= SIP_NULL)
				{
					pRack = (SipHeader*)fast_memget(TIMER_MEM_ID,\
						 sizeof(SipHeader), err);
					pRack->dType = SipHdrTypeRAck;
					if(SipFail == sip_getHeaderAtIndex(msg, \
						SipHdrTypeRAck,pRack, 0, err))
					{	
						(void)fast_memfree(TIMER_MEM_ID, pRack, err);
						return SipFail;
					}
					sip_freeSipRackHeader\
						(msg->u.pRequest->pRequestHdr->pRackHdr);
					msg->u.pRequest->pRequestHdr->pRackHdr=\
						(SipRackHeader*)(pRack->pHeader)	;
					(void)fast_memfree(TIMER_MEM_ID, pRack, err);
				}	
			
#endif

				/* Message is a PRACK request used for acknowledgement of 
				   reliably sent provisional responses */
				/* Form timer key */
				tkey.dCallid=STRDUPTIMER(msg->pGeneralHdr->pCallidHdr->pStr1);
				if(tkey.dCallid == SIP_NULL)
				{
					*err = E_NO_MEM;
					return SipFail;
				}
				/* This key must be used for matching responses only */
				tkey.dMatchType = SipMessageResponse;
				tkey.dCseq = msg->pGeneralHdr->pCseqHdr->dIntVar1;
				if(sip_initSipRackHeader(&(tkey.pRackHdr),err)==SipFail)
					return SipFail;
				if (msg->u.pRequest->pRequestHdr->pRackHdr != SIP_NULL)	
				{
					tkey.pRackHdr->dIntVar2 = msg->u.pRequest->pRequestHdr->\
						pRackHdr->dIntVar2;
					tkey.pRackHdr->dIntVar1 = msg->u.pRequest->pRequestHdr->\
						pRackHdr->dIntVar1;
					tkey.pRackHdr->pStr1 = (SIP_S8bit *)STRDUPTIMER(msg->\
						u.pRequest->\
						pRequestHdr->pRackHdr->pStr1);
					if(tkey.pRackHdr->pStr1 == SIP_NULL)
					{
						*err = E_NO_MEM;
						return SipFail;
					}
					/*  Use different logic when comparing keys. Logic must take
						into account the RSeq and RACK header matching 
					*/
					tkey.dMatchFlag = TIMER_RPR;
				}
				/* clone from and to headers into the timer Key fields */
				if(sip_initSipFromHeader(&(tkey.pFromHdr),err)==SipFail)
					return SipFail;
				if(__sipParser_cloneSipFromHeader(tkey.pFromHdr,\
					msg->pGeneralHdr->pFromHeader,err) == SipFail)
					return SipFail;
				if(sip_initSipToHeader(&(tkey.pToHdr),err)==SipFail)
					return SipFail;
				if(__sipParser_cloneSipToHeader(tkey.pToHdr,\
					msg->pGeneralHdr->pToHdr,err) == SipFail)
					return SipFail;
				/* Add branch from first Via Header */
				tkey.pViaBranch = SIP_NULL;
				/* Check if Via header present */
				if(sip_listSizeOf(&(msg->pGeneralHdr->slViaHdr), &count, err)\
					!=SipFail)
				{
					if(count!=0)
					{
						SipViaHeader *pViaHeader;
		
						/* Get first via header from the list */
						if(sip_listGetAt(&(msg->pGeneralHdr->slViaHdr), 0, \
							(SIP_Pvoid *) &pViaHeader , err) != SipFail)
						{
							SIP_S8bit *pViaBranch;
							/* Get Branch parameter */
							if(sip_getBranchFromViaHdr(pViaHeader, &pViaBranch,\
								err) !=SipFail)
							{
								tkey.pViaBranch = STRDUPTIMER(pViaBranch);
								if(tkey.pViaBranch == SIP_NULL)
								{
									*err = E_NO_MEM;
									return SipFail;
								}
							}
						}
					}
				}


				/* Ask application to stop matching retransmissions */	
				tkey.pContext = pContext;
					if(fast_stopTimer(&tkey,&outkey,&outbuf,err)==SipFail)
				{
					(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
					sip_freeSipFromHeader(tkey.pFromHdr);
					sip_freeSipToHeader(tkey.pToHdr);
					sip_freeSipRackHeader(tkey.pRackHdr);
					if(tkey.pViaBranch != SIP_NULL)
						(void)fast_memfree(TIMER_MEM_ID,tkey.pViaBranch,SIP_NULL);
					return SipFail;
				}
				(void)fast_memfree(TIMER_MEM_ID,tkey.dCallid,SIP_NULL);
				sip_freeSipFromHeader(tkey.pFromHdr);
				sip_freeSipToHeader(tkey.pToHdr);
				sip_freeSipRackHeader(tkey.pRackHdr);
				if(tkey.pViaBranch != SIP_NULL)
					(void)fast_memfree(TIMER_MEM_ID,tkey.pViaBranch,SIP_NULL);
			}
#endif			 /* SIP_RPR */
			break;

		default:
			return SipSuccess; /* Aparna */
	}


	/* This section of code entered only if a fast_stopTimer returned 
	   successfully. Free all resources returned by the application on 
	   stopping the timer */
	tbuf = (SipTimerBuffer *) outbuf;
	sip_freeSipTimerKey(outkey);
	sip_freeSipTimerBuffer(tbuf);

	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_updateTimer
** DESCRIPTION: This function is invoked by the stack to change retrans
** count of a message. 
** 
** PARAMETERS:
**      	      	msg(IN)		:The SipMessage
**			       infoflag(IN)	:The infoflag.This is to indicate, 
**								 the receipt of provisional response.
**			       pContext(IN)	:The event Context structure
**				  	err(IN)		:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_updateTimer
#ifdef ANSI_PROTO
(SipMessage *msg, SipBool infoflag, SipEventContext *pContext, SipError *err)
#else
(msg, infoflag, pContext, err)
	SipMessage *msg; 
	SipBool infoflag; 
	SipEventContext *pContext;
	SipError *err;
#endif

{
	SipTimerKey *tkey, *outkey=SIP_NULL;
	SipTimerBuffer *tbuf;
	SIP_Pvoid outbuf;
	SIP_U16bit retrans;
	SipGeneralHeader *gheader;
	SIP_U32bit count;
	
	tkey = (SipTimerKey *) fast_memget(TIMER_MEM_ID, sizeof(SipTimerKey),\
		SIP_NULL);
	gheader = msg->pGeneralHdr;

	tkey->dCallid = STRDUPTIMER(gheader->pCallidHdr->pStr1);
	if(tkey->dCallid == SIP_NULL)
	{
		*err = E_NO_MEM;
		return SipFail;
	}
	tkey->pMethod = STRDUPTIMER(gheader->pCseqHdr->pStr1);
	if(tkey->pMethod == SIP_NULL)
	{
		*err = E_NO_MEM;
		return SipFail;
	}
	tkey->dCseq = gheader->pCseqHdr->dIntVar1;
	tkey->dMatchType = SipMessageAny;
	tkey->dMatchFlag = TIMER_TRANSACTION;
	/* clone From/To headers into timer Key */
	if(sip_initSipFromHeader(&(tkey->pFromHdr),err)==SipFail)
		return SipFail;
	if(__sipParser_cloneSipFromHeader(tkey->pFromHdr,gheader->pFromHeader,err)\
		== SipFail)
		return SipFail;
	if(sip_initSipToHeader(&(tkey->pToHdr),err)==SipFail)
		return SipFail;
	if(__sipParser_cloneSipToHeader(tkey->pToHdr,gheader->pToHdr,err) == \
		SipFail)
		return SipFail;
	/* Add branch from first Via Header */
	tkey->pViaBranch = SIP_NULL;
	/* Check if Via header present */
	if(sip_listSizeOf(&(msg->pGeneralHdr->slViaHdr), &count, err)\
		!=SipFail)
	{
		if(count!=0)
		{
			SipViaHeader *pViaHeader;
	
			/* Get first via header from the list */
			if(sip_listGetAt(&(msg->pGeneralHdr->slViaHdr), 0, \
				(SIP_Pvoid *) &pViaHeader , err) != SipFail)
			{
				SIP_S8bit *pViaBranch;
				/* Get Branch parameter */
				if(sip_getBranchFromViaHdr(pViaHeader, &pViaBranch,\
					err) !=SipFail)
				{
					tkey->pViaBranch = STRDUPTIMER(pViaBranch);
					if(tkey->pViaBranch == SIP_NULL)
					{
						*err = E_NO_MEM;
						return SipFail;
					}
				}
			}
		}
	}

	tkey->pContext = pContext;
	if(fast_stopTimer(tkey,&outkey,&outbuf,err)==SipFail) 
	
	{
		/* There is no timer entry for this response */
		(void)fast_memfree(TIMER_MEM_ID,tkey->dCallid,SIP_NULL);
		(void)fast_memfree(TIMER_MEM_ID,tkey->pMethod,SIP_NULL);
		if(tkey->pViaBranch != SIP_NULL)
			(void)fast_memfree(TIMER_MEM_ID,tkey->pViaBranch,SIP_NULL);
		sip_freeSipFromHeader(tkey->pFromHdr);
		sip_freeSipToHeader(tkey->pToHdr);
		(void)fast_memfree(TIMER_MEM_ID,tkey,SIP_NULL);
		return SipFail;
	}

	tbuf = (SipTimerBuffer *) outbuf;
	/* outkey now contains the TimerKey and outbuf, the sip message */

	(void)fast_memfree(TIMER_MEM_ID,tkey->dCallid,SIP_NULL);
	(void)fast_memfree(TIMER_MEM_ID,tkey->pMethod,SIP_NULL);
	if(tkey->pViaBranch != SIP_NULL)
		(void)fast_memfree(TIMER_MEM_ID,tkey->pViaBranch,SIP_NULL);
	sip_freeSipFromHeader(tkey->pFromHdr);
	sip_freeSipToHeader(tkey->pToHdr);
	(void)fast_memfree(TIMER_MEM_ID,tkey,SIP_NULL);
	
	/* Now change Duration according to Information Flag */
	tbuf->dDuration = tbuf->dDuration * 2;
	if ((tbuf->dInfoReceived == SipSuccess) || (infoflag ==SipSuccess) \
		|| (tbuf->dDuration>=SIP_T2))
		tbuf->dDuration = SIP_T2; 

	/* INVITE needs to timeout after seven not 11 */
	if (!sip_strncmp(outkey->pMethod,(SIP_S8bit *)"INVITE",6)) 
		retrans = SIP_MAXINVRETRANS;
	else 
		retrans = SIP_MAXRETRANS;

	if (tbuf->dRetransCount < retrans)
	{
		/*tbuf->dRetransCount++;*/
		SipBool res;
		res = fast_startTimer( tbuf->dDuration , 0, sip_processTimeOut,\
			(SIP_Pvoid) tbuf, outkey, err);
		if(res == SipFail)
		{
			sip_freeSipTimerBuffer(tbuf);
			sip_freeSipTimerKey(outkey);
		}
	}
	else
	{
		/* Maximum Retransmissions have been reached, 
		   so delete space returned by fast_stoptimer */
		sip_freeSipTimerBuffer(tbuf);
		sip_freeSipTimerKey(outkey);
	}
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_processTimeOut
** DESCRIPTION: This function is invoked by the timer manager
** when a Time out occurs. It is passed the TimerKey and the TimerBuffer
** of the timeout Context
** 
** PARAMETERS:
**      	      	pKey(IN)		:The SipMessage
**			       pBuffer(IN)	:The infoflag.This is to indicate, 
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_processTimeOut(SipTimerKey *pKey, SIP_Pvoid pBuffer)
{
	SipTimerBuffer *tbuf;
	SipError err;
	SIP_U16bit retrans;
	SIP_U32bit dRetransT2;
#ifdef SIP_TRACE
	SIP_S8bit tracebuf[200];
#endif
	

	/* Retrieve previous retransmission interval and double it */
	tbuf = (SipTimerBuffer *) pBuffer;
	tbuf->dDuration = tbuf->dDuration * 2;

	/* If user has specified own T2 value, use that for the comparisons that
	   follow */
	if(tbuf->dRetransT2 != 0)
		dRetransT2 = tbuf->dRetransT2;
	else
		dRetransT2 = SIP_T2;

	/* If INVITE request, cap retransmission interval to T2 if new retrans 
	   exceeds T2 */

	if (((tbuf->dInfoReceived == SipSuccess) || (tbuf->dDuration>=dRetransT2))\
		&& (pKey->dMatchType != SipMessageResponse) &&\
		(sip_strcmp(pKey->pMethod, (SIP_S8bit *)"INVITE")))
		tbuf->dDuration = dRetransT2; 

	/* Get the max retrans Value. Different for INVITE and responses */
	if(tbuf->dMaxInviteRetransCount == SIP_MAXU16BIT &&\
			tbuf->dMaxRetransCount == SIP_MAXU16BIT)
	{
		retrans = (sip_strncmp((SIP_S8bit *)tbuf->pBuffer,(SIP_S8bit *)"INVITE", 6)) ? \
			SIP_MAXRETRANS:SIP_MAXINVRETRANS;
		if(pKey->dMatchType == SipMessageResponse) 
			retrans = SIP_MAXINVRETRANS;
	}	
	else
	{
		retrans = (sip_strncmp((SIP_S8bit *)tbuf->pBuffer,(SIP_S8bit *)"INVITE", 6)) ? \
			tbuf->dMaxRetransCount:tbuf->dMaxInviteRetransCount;
		/*If u have retransmission probs  come here*/
	
		if(pKey->dMatchType == SipMessageResponse) 
			retrans = tbuf->dMaxInviteRetransCount;
	}
			
	if (tbuf->dRetransCount < retrans)
	{
		/* Retransmission count has not yet exceeded the limit */
#ifdef SIP_TRACE
		SIP_S8bit msg[21];
#endif
		if(tbuf->pContext != SIP_NULL)
			tbuf->dAddr.pData = tbuf->pContext->pData;
		else
			tbuf->dAddr.pData = SIP_NULL;

		if (sip_sendToNetwork((SIP_S8bit *)(tbuf->pBuffer), \
			tbuf->dBufferLength, &tbuf->dAddr, \
			tbuf->dTranspType, &err) == SipFail)
		{
			sip_error (SIP_Major, "SIP_ERROR: Cannot send to network");
			INC_ERROR_NETWORK
		}

		/* Update response/request sent statistics */
		if(pKey->dMatchType == SipMessageResponse)
		{
			INC_API_RESP_SENT
		}
		else if(pKey->dMatchType == SipMessageRequest)
		{
			INC_API_REQ_SENT
		}
	
#ifdef SIP_TRACE
		(void)sip_strncpy (msg, (SIP_S8bit *)tbuf->pBuffer, 20);
		msg[20]='\0';
		(void)sip_trace(SIP_Brief,SIP_Outgoing,(SIP_S8bit *) "Message sent out.");
		(void)sip_trace(SIP_Detailed,SIP_Outgoing,(SIP_S8bit *)tbuf->pBuffer);
		(void)HSS_SPRINTF (tracebuf,(SIP_S8bit *)"Timer Value: %d\nMax transmissions: %d\nMsg"\
			"(1st 20 bytes) : \"%s\" \n",tbuf->dDuration, retrans+1,msg);
		(void)sip_trace(SIP_Brief, SIP_Outgoing, tracebuf);
#endif
		tbuf->dRetransCount++;
#ifdef SIP_RETRANSCALLBACK
		if(tbuf->enableRetransCallback == SipSuccess)
		{
			sip_indicateMessageRetransmission\
				(tbuf->pContext, pKey, tbuf->pBuffer, tbuf->dBufferLength,
				&(tbuf->dAddr), tbuf->dRetransCount, tbuf->dDuration);
		}
#endif
		if(SipFail==fast_startTimer( tbuf->dDuration , 0, sip_processTimeOut, \
			(SIP_Pvoid) tbuf, pKey, &err))
		{
			sip_freeSipTimerBuffer(tbuf);
			sip_freeSipTimerKey(pKey);
		}
	}
	else
	{
		/* Maximum Retransmissions have been reached, 
		   so free space returned by fast_stoptimer */
		SipEventContext *tempcontext;
		tempcontext = tbuf->pContext;
		tbuf->pContext = SIP_NULL;
		if(tempcontext != SIP_NULL)
		{
			if((tempcontext->dOptions.dOption & SIP_OPT_DIRECTBUFFER) == \
					SIP_OPT_DIRECTBUFFER)
			{
				tbuf->pBuffer= SIP_NULL;
			}
		}
		
		sip_freeSipTimerBuffer(tbuf);
		sip_freeSipTimerKey(pKey);
		sip_indicateTimeOut(tempcontext);
	}
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_freeSipTimerBuffer
** DESCRIPTION: This function frees teh timer buffer structure
** of the timeout Context
** 
** PARAMETERS:
**      	      	b(IN)	:The SipMessage
** Return Value:
**				  void	
*******************************************************************************/
#if 0
void sip_freeSipTimerBuffer
#ifdef ANSI_PROTO
	(SipTimerBuffer *b)
#else
	(b)
	SipTimerBuffer *b;
#endif
{
	if (b==SIP_NULL) 
		return;
	HSS_LOCKREF(b->dRefCount);
	HSS_DECREF(b->dRefCount);
	if(HSS_CHECKREF(b->dRefCount))
	{
		if(b->pContext != SIP_NULL)
		{
			if( (b->pContext->dOptions.dOption & SIP_OPT_DIRECTBUFFER) != \
					SIP_OPT_DIRECTBUFFER)
			{
				if(b->pBuffer != SIP_NULL)
					FREE(b->pBuffer);
			}
		}			
		else
		{
			if(b->pBuffer != SIP_NULL)
				FREE(b->pBuffer);
		}	
		if(b->pContext != SIP_NULL)
			sip_freeEventContext(b->pContext);

		HSS_UNLOCKREF(b->dRefCount);
		HSS_DELETEREF(b->dRefCount);
		FREE(b);
	}
	else
	{
		HSS_UNLOCKREF(b->dRefCount);
	}
}
#endif





