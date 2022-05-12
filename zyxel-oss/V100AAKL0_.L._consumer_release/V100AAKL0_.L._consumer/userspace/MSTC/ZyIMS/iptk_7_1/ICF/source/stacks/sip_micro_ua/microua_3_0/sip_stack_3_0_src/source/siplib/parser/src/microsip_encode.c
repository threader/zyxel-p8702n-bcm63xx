/****************************************************************************
 ** FUNCTION: 		Forms a Text SIP Message from SIP Structure
 **
 ****************************************************************************
 **
 ** FILENAME: 		microsip_encode.c
 **
 ** DESCRIPTION:	This file contains code to convert from structures 
 **					to SIP Text buffer.
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.c		Initial
 **					Toshniwal		of SIP Stack
 ** 03-Mar-2002		Aparna				-				Added support for
 **														Auth, From & String
 **														Families
 **	25-Apr-2002		Aparna				-				Fix for SPRs 1_0_1
 **														and 1_0_2.
 **	29-Apr-2002		Aparna				-				Fix for SPR 1_0_5.
 ** 19-Dec-2009  Rohit Aggarwal   SPR 20246  Configuration to strip Route header
 **
 ***************************************************************************
 **				COPYRIGHT , Aricent, 2009
 ***************************************************************************/

#include "microsip_struct.h"
#include "microsip_encode.h"
#include "microsip_hdrencoders.h"
#include "microsip_encodeinternal.h"
#include "microsip_init.h"
#include "microsip_accessors.h"


/****************************************************************************
** FUNCTION: sip_formSdpBody
** DESCRIPTION: This function encodes an SDP body of a SIP message (i.e.
**				converts it to Text)
** 
** PARAMETERS:
**				pSdpMessage(IN)	- The decoded SDP message body
**				pOut(IN)		- The output buffer into which the encoded SDP lines
**								  will be written into.
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formSdpBody 
#ifdef ANSI_PROTO
	(SdpMessage *pSdpMsg, 
	 SIP_S8bit 	*pOut, 
	 SipError 	*pError)
#else
	(pSdpMsg, pOut, pError)
	SdpMessage 	*pSdpMsg;
	SIP_S8bit 	*pOut;
	SipError 	*pError;
#endif
{
	SIP_U32bit index_1, size;

	/* Form the version line */
	if (pSdpMsg->pVersion !=SIP_NULL)
	{
		(void)STR_CAT (pOut,(SIP_S8bit*) "v=");
		(void)STR_CAT (pOut, pSdpMsg->pVersion);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form origin line */
	if (pSdpMsg->pOrigin != SIP_NULL)
	{
		(void)STR_CAT(pOut,(SIP_S8bit*) "o=");
		STRCAT_AND_SPACE(pOut, pSdpMsg->pOrigin->pUser);
		STRCAT_AND_SPACE(pOut, pSdpMsg->pOrigin->pSessionid);
		STRCAT_AND_SPACE(pOut, pSdpMsg->pOrigin->pVersion);
		STRCAT_AND_SPACE(pOut, pSdpMsg->pOrigin->pNetType);
		STRCAT_AND_SPACE(pOut, pSdpMsg->pOrigin->pAddrType);
	    (void)STR_CAT(pOut, pSdpMsg->pOrigin->pAddr);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form session line */
	if (pSdpMsg->pSession != SIP_NULL)
	{
		(void)STR_CAT(pOut,(SIP_S8bit*) "s=");
		(void)STR_CAT(pOut, pSdpMsg->pSession);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form Information line */
	if (pSdpMsg->pInformation != SIP_NULL)
	{
		(void)STR_CAT(pOut,(SIP_S8bit*)"i=");
		(void)STR_CAT(pOut, pSdpMsg->pInformation);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form Uri */
	if (pSdpMsg->pUri != SIP_NULL)
	{
		(void)STR_CAT(pOut,(SIP_S8bit*)"u=");
		(void)STR_CAT(pOut, pSdpMsg->pUri);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form Email */
	(void)sip_listSizeOf(&(pSdpMsg->slEmail), &size, pError);
	index_1 = 0;
	while (index_1 < size)
	{
		SIP_S8bit *e;
		(void)sip_listGetAt(&(pSdpMsg->slEmail), index_1, (SIP_Pvoid *) &e, pError);
		(void)STR_CAT(pOut,(SIP_S8bit*)"e=");
		(void)STR_CAT(pOut, e);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		index_1++;
	}

	/* Form Phone lines */
	(void)sip_listSizeOf(&(pSdpMsg->slPhone), &size, pError);
	index_1 = 0;
	while (index_1 < size)
	{
		SIP_S8bit *p;
		(void)sip_listGetAt(&(pSdpMsg->slPhone), index_1, (SIP_Pvoid *) &p, pError);
		(void)STR_CAT(pOut,(SIP_S8bit*)"p=");
		(void)STR_CAT(pOut, p);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		index_1++;
	}

	/* Form connection line */
	if (pSdpMsg->slConnection != SIP_NULL)
	{
		(void)sip_formSdpConnection(pOut, pSdpMsg->slConnection, pError);
	}

	/* Form all the session level b= lines */
		(void)sip_formListOfBandwidthVal(pOut, &(pSdpMsg->pBandwidth), pError);

	/* Get SdpTime */
	(void)sip_listSizeOf(&(pSdpMsg->slTime), &size, pError);
	index_1 = 0;
	while (index_1 < size)
	{
		SdpTime		*st;
		SIP_U32bit	tsize, tindex;

		(void)sip_listGetAt(&(pSdpMsg->slTime), index_1, (SIP_Pvoid *) &st, pError);
		(void)STR_CAT(pOut,(SIP_S8bit*)"t=");
		(void)STRCAT_AND_SPACE(pOut, st->pStart);
		(void)STR_CAT(pOut, st->pStop);

		/* Now add Repeat r= */
		(void)sip_listSizeOf(&(st->slRepeat), &tsize, pError);
		tindex = 0;
		while (tindex < tsize)
		{
			SIP_S8bit	*r;

			(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		    (void)sip_listGetAt(&(st->slRepeat), tindex, (SIP_Pvoid *)&r, pError);
			(void)STR_CAT(pOut, (SIP_S8bit*)"r=");
			(void)STR_CAT(pOut,(SIP_S8bit*) r);
			tindex++;
		}
		if (st->pZone != SIP_NULL)
		{
			(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
			(void)STR_CAT(pOut,(SIP_S8bit*) "z=");
			(void)STR_CAT(pOut,(SIP_S8bit*) st->pZone);
		}
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		index_1++;
	}


	/* Form Key */
	if (pSdpMsg->pKey != SIP_NULL)
	{
		(void)STR_CAT(pOut,(SIP_S8bit*)"k=");
		(void)STR_CAT(pOut, pSdpMsg->pKey);
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}

	/* Form all the session level a= lines */
	(void)sip_formListOfAttributes(pOut, &(pSdpMsg->slAttr), pError);
		
	/* Get SdpMedia */
	(void)sip_listSizeOf(&(pSdpMsg->slMedia), &size, pError);
	index_1 = 0;
	while (index_1 < size)
	{
		SdpMedia	*m;
		SIP_S8bit	dPort[20];
		SIP_U32bit	tsize, tindex;

		(void)sip_listGetAt (&(pSdpMsg->slMedia), index_1, (SIP_Pvoid *) &m, pError);
		if (m->pMediaValue != SIP_NULL) /* is there an m= line ? */
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"m=");
			STRCAT_AND_SPACE(pOut, m->pMediaValue);
			(void)sip_snprintf((SIP_S8bit *)dPort, 20, "%u", m->dPort);
			dPort[20-1]='\0';
			(void)STR_CAT(pOut, dPort);
			if (m->pPortNum != SIP_NULL)
			{
				(void)STR_CAT(pOut, (SIP_S8bit*)"/");
				(void)sip_snprintf((SIP_S8bit *)dPort, 20, "%lu", *(m->pPortNum));
				dPort[ 20-1]='\0';
				(void)STR_CAT(pOut,dPort);
			}
			(void)STR_CAT(pOut, (SIP_S8bit*)" ");
			STRCAT_AND_SPACE(pOut, m->pProtocol);
			(void)STR_CAT(pOut, m->pFormat);
			(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		}
		if (m->pInformation != SIP_NULL) /* is there an i= ? */
		{
			(void)STR_CAT(pOut, (SIP_S8bit*)"i=");
			(void)STR_CAT(pOut, m->pInformation);
			(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		}
		/* Are there slConnection fields ? */
		(void)sip_listSizeOf(&(m->slConnection), &tsize, pError);
		tindex = 0;
		while (tindex < tsize)
		{
			SdpConnection *c;

			(void)sip_listGetAt(&(m->slConnection), tindex, \
				    (SIP_Pvoid *)&c, pError);
			(void)sip_formSdpConnection(pOut, c, pError);
			tindex++;
		}

		(void)sip_formListOfBandwidthVal(pOut, &(m->slBandwidth), pError);

		if (m->pKey != SIP_NULL) /* is there a k= ? */
		{
			(void)STR_CAT(pOut,(SIP_S8bit*)"k=");
			(void)STR_CAT(pOut, m->pKey);
			(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
		}

		/* For the a= lines for this media line */
		(void)sip_formListOfAttributes(pOut, &(m->slAttr), pError);

		index_1++;		
	}
	return SipSuccess;
} /* sip_formSdpBody */


/*****************************************************************************
** FUNCTION: 	 sip_formMessage
**
** DESCRIPTION: This forms the Text message with the order as given 
**				 in Order Table.
**				
** PARAMETERS:
**				 pSipMsg - SipMessage structure to be converted
**				 options - structure containing messages formation 
**						   options like - short/full form for header names
**						   comma separated/single line headers etc.
**				 pOut 	 - pre-allocated buffer into which the formed 
**						   text messsage is written into
**				 dLength - length of the formed message returned in this
**
** NOTE: 
**				
*****************************************************************************/

SipBool sip_formMessage 
#ifdef ANSI_PROTO
	(SipMessage *pSipMsg, SipOptions *options, SIP_S8bit *pOut, \
	 SIP_U32bit *dLength, SipError *pError)
#else
	(pSipMsg, options, pOut, dLength, pError)
	SipMessage 	*pSipMsg;
	SipOptions 	*options;
	SIP_S8bit 	*pOut;
	SIP_U32bit 	*dLength;
	SipError 	*pError;
#endif
{
	SipBool				retval = SipFail;
	SIP_U32bit			sipheaderlength;
	SIP_U32bit			i, index_1;
	SIP_U32bit			dSizeOfOrderInfoList = 0;
	SIP_U32bit			dCurrentOrderInfoIndex = 0;
	SIP_S32bit			siplistmap[HEADERTYPENUM];
	SIP_S8bit			*temp;
	SIP_S8bit			dCodeNum[4];
	SIP_U32bit			hdrcount,addcount,count;
	SIP_U32bit			bodylength;
	SipHeaderOrderInfo *pOrderInfoEntry, *ordertemp;
	SipContentTypeHeader *ctypehdr;

	if (pError == SIP_NULL) 
		return SipFail;
		
	if (pSipMsg == SIP_NULL) 
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	/* 
	 * Check if options has both FULL and SHORT Header options and
	 * return error.
	 */

	if ((options->dOption&(SIP_OPT_FULLFORM|SIP_OPT_SHORTFORM))==\
		(SIP_OPT_FULLFORM|SIP_OPT_SHORTFORM))
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	/*
	 * Check if options has both COMMA SEPARATED and SINGLE Header 
	 * options and return error
	 */

	if ((options->dOption&(SIP_OPT_COMMASEPARATED|SIP_OPT_SINGLE))==\
		(SIP_OPT_COMMASEPARATED|SIP_OPT_SINGLE))
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}

	(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), &dSizeOfOrderInfoList, \
                pError);
	while (dCurrentOrderInfoIndex < dSizeOfOrderInfoList)
	{
		en_HeaderType 	hdrtype;
		en_HeaderForm 	form;

		(void)sip_getHeaderTypeAtHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
			&hdrtype, pError);
		(void)sip_getHeaderCountFromHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
			&hdrcount, pError);
	/* Temp.	
		if (hdrtype == SipHdrTypeAuthorization)
			break;*/

		/* 
		 * Convert all headers to FULL or SHORT form till 
		 * Authorization Header.
		 */

		if ((options->dOption&(SIP_OPT_FULLFORM))==(SIP_OPT_FULLFORM))
		{
			(void)sip_setHeaderFormAtHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
				SipFormFull, pError);
			form = SipFormFull;
		}
		else if ((options->dOption&(SIP_OPT_SHORTFORM))==(SIP_OPT_SHORTFORM))
		{
			(void)sip_setHeaderFormAtHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
				SipFormShort, pError);
                form = SipFormShort;
		}
		else
		{
			if (sip_getHeaderFormFromHeaderLine(pSipMsg, \
				dCurrentOrderInfoIndex, &form, pError) == SipFail)
				return SipFail;
		}

		/*
		 * Convert all headers into SINGLE form 
		 * till Authorization Header
		 */

		if ((options->dOption&(SIP_OPT_SINGLE))==(SIP_OPT_SINGLE))
		{
			if (hdrcount > 1)
			{
				for (i=dCurrentOrderInfoIndex; \
					 i<(dCurrentOrderInfoIndex+hdrcount); \
					 i++)
				{
					if (sip_initSipHeaderOrderInfo(&pOrderInfoEntry, \
						pError) == SipFail)	
						return SipFail;

					pOrderInfoEntry->dType		= hdrtype;
					pOrderInfoEntry->dTextType	= form;
					pOrderInfoEntry->dNum		= 1;
					
					if (i == dCurrentOrderInfoIndex)
					{
						(void)sip_listSetAt(&(pSipMsg->slOrderInfo), i, \
							(SIP_Pvoid*)pOrderInfoEntry, pError);
					}
					else
					{
						(void)sip_listInsertAt(&(pSipMsg->slOrderInfo), i, \
							(SIP_Pvoid*)pOrderInfoEntry, pError);
					}
				}
				dCurrentOrderInfoIndex += hdrcount;
				(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), \
					&dSizeOfOrderInfoList, pError);
			}
			else
			{
				dCurrentOrderInfoIndex++;
			}
		} /* End of "if single" */
		else
		{
			dCurrentOrderInfoIndex++;
		}
	} /* end of iteration thru' order info table */

	/* Convert all headers to COMMA SEPARATED headers till Authorization */
	if ((options->dOption&(SIP_OPT_COMMASEPARATED))==(SIP_OPT_COMMASEPARATED))
	{
		en_HeaderType 	hdrtype;

		for (i=0;i<HEADERTYPENUM;i++)
			siplistmap[i]=-1;
		(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), &dSizeOfOrderInfoList, pError);
		dCurrentOrderInfoIndex=0;
		while (dCurrentOrderInfoIndex<dSizeOfOrderInfoList)
		{
			(void)sip_getHeaderTypeAtHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
				&hdrtype, pError);
#ifdef SIP_AUTHENTICATE				
			if (hdrtype == SipHdrTypeAuthorization)
				break;
#endif				

			if (siplistmap[hdrtype] != -1)
			{
				index_1=siplistmap[hdrtype];
				(void)sip_getHeaderCountFromHeaderLine(pSipMsg, \
					dCurrentOrderInfoIndex, &addcount, pError);
				(void)sip_getHeaderCountFromHeaderLine(pSipMsg, index_1, \
					&count, pError);
				(void)__sip_setHeaderCountInHeaderLine(pSipMsg, index_1, \
					(count+addcount), pError);
				(void)sip_listDeleteAt(&(pSipMsg->slOrderInfo), \
					dCurrentOrderInfoIndex, pError);
				(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), \
					&dSizeOfOrderInfoList,pError);
			}
			else
			{
				siplistmap[hdrtype]=dCurrentOrderInfoIndex;
				dCurrentOrderInfoIndex++;
			}
		}
	}
	
	/* Reorder the order table to place Hop by Hop headers at the end */
	if ((options->dOption&(SIP_OPT_REORDERHOP))==(SIP_OPT_REORDERHOP))
	{
		en_HeaderType 	hdrtype;

		(void)sip_listSizeOf(&(pSipMsg->slOrderInfo),&dSizeOfOrderInfoList,pError);
		dCurrentOrderInfoIndex=0;
		index_1=0;
		while (dCurrentOrderInfoIndex<dSizeOfOrderInfoList)
		{
			(void)sip_getHeaderTypeAtHeaderLine(pSipMsg, dCurrentOrderInfoIndex, \
				&hdrtype, pError);
#ifdef SIP_AUTHENTICATE				
			if (hdrtype == SipHdrTypeAuthorization)
				break;
#endif				
			if (__checkHeaderTypeHop(hdrtype)==SipSuccess)
			{
				(void)sip_listGetAt(&(pSipMsg->slOrderInfo), \
					dCurrentOrderInfoIndex, (SIP_Pvoid *) &pOrderInfoEntry, \
					pError);
				if (sip_initSipHeaderOrderInfo(&ordertemp,pError)==SipFail) 
					return SipFail;

				*ordertemp=*pOrderInfoEntry;
				(void)sip_listInsertAt(&(pSipMsg->slOrderInfo), index_1, \
					(SIP_Pvoid*)ordertemp, pError);
				index_1++;
				dCurrentOrderInfoIndex++;
				(void)sip_listDeleteAt(&(pSipMsg->slOrderInfo), \
					dCurrentOrderInfoIndex, pError);
				(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), \
					&dSizeOfOrderInfoList,pError);
			}
			else
				dCurrentOrderInfoIndex++;
		}
	}


	for (i=0;i<HEADERTYPENUM;i++)
		siplistmap[i]=0;


	if (pSipMsg->dType == SipMessageRequest)
	{
		/* Get Request Line */
		if (pSipMsg->u.pRequest->pRequestLine==SIP_NULL)
		{
			*pError = E_INV_PARAM;
			return SipFail;
		}
		
		/* Fill method */
		STRCPY((SIP_S8bit *)pOut, \
			(SIP_S8bit *)pSipMsg->u.pRequest->pRequestLine->pStr1 );
		(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*)" ");
		/* Fill request URI */
		(void)sip_formAddrSpec (SIP_NULL, pOut, \
			pSipMsg->u.pRequest->pRequestLine->pAddrSpec, pError);
		/* Fill Version */
		(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*)" ");
		(void)STR_CAT( (SIP_S8bit *)pOut, pSipMsg->u.pRequest->pRequestLine->pStr2);
		(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*)CRLF); 
	}
	else if (pSipMsg->dType == SipMessageResponse)
	{
		/* Get Stat Line */
		if (pSipMsg->u.pResponse->pStatusLine==SIP_NULL)
		{
			*pError = E_INV_PARAM;
			return SipFail;
		}
		
		/* Fill Version */
		STRCPY((SIP_S8bit *)pOut, \
			(SIP_S8bit *)pSipMsg->u.pResponse->pStatusLine->pStr1 );
		(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*)" ");
		/* Fill codenum */
		(void)sip_snprintf((SIP_S8bit *)dCodeNum, 4, "%03d", \
			pSipMsg->u.pResponse->pStatusLine->dIntVar1); 
		/* dCodeNum is not accessed */
		dCodeNum[ 4-1]='\0';
		STRCAT_AND_SPACE ( (SIP_S8bit *)pOut,dCodeNum);
		/* Fill reason */
		(void)STR_CAT( (SIP_S8bit *)pOut,pSipMsg->u.pResponse->pStatusLine->pStr2);
		(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*)CRLF);
	}
	(void)sip_listSizeOf(&(pSipMsg->slOrderInfo), &dSizeOfOrderInfoList, pError);

	temp = &(pOut[sip_strlen(pOut)]);

	dCurrentOrderInfoIndex = 0;
	while (dCurrentOrderInfoIndex < dSizeOfOrderInfoList)
	{
		/* Fix for IPTK CSR 1-8047728: SPR 20246 */
		retval=sip_formEachHeader(dCurrentOrderInfoIndex, options, pSipMsg, \
					temp, (SIP_U32bit *) siplistmap,pError);
		if(retval != SipSuccess)	
			return SipFail;
		temp = &(pOut[sip_strlen(pOut)]);
		dCurrentOrderInfoIndex++;
	}

	/* Through with the SIP headers. Now form Body */
	sipheaderlength = sip_strlen(pOut);
	bodylength = 0;
	ctypehdr=pSipMsg->pGeneralHdr->pContentTypeHdr;
#ifdef SIP_INCREMENTAL_PARSING	
	if (SIP_NULL != ctypehdr)
	if (ctypehdr->pBuffer != SIP_NULL)
	{
		SipHeader *pCType = SIP_NULL;
		
		pCType = (SipHeader *)fast_memget(ENCODE_MEM_ID, \
			sizeof(SipHeader), pError);
		pCType->dType = SipHdrTypeContentType;	
		pCType->pHeader = pSipMsg->pGeneralHdr->pContentTypeHdr;

		if (SipFail == sip_getHeaderAtIndex(pSipMsg, \
			SipHdrTypeContentType,pCType, 0, pError))
		{
			return SipFail;
		}	
		HSS_LOCKEDDECREF(pSipMsg->pGeneralHdr->pContentTypeHdr->dRefCount);
		ctypehdr  = (SipContentTypeHeader*)(pCType->pHeader);
		(void)fast_memfree(DECODE_MEM_ID,pCType,pError);		
	}
#endif	/* SIP_INCREMENTAL_PARSING */

	if ((retval = sip_formMimeBody(SIP_NULL, pSipMsg->slMessageBody,ctypehdr,\
		&pOut[sip_strlen(pOut)], &bodylength,pError))==SipFail)
		return SipFail;

	/* Now reduce bodylen by 2 since top (SIP_S8bit*)CRLF has been counted */
	if (bodylength != 0) bodylength -=2;
	if (((options->dOption) & SIP_OPT_CLEN) == (SIP_OPT_CLEN))
	{
		SIP_S8bit  constructedclen[30];
		SIP_U32bit ctypeoffset,crlfoffset,ctypehdrlen;
	
		if (glbSipParserMemStrStr(pOut, sipheaderlength, \
			(SIP_S8bit *)"\r\nl:", &ctypeoffset) == SipFail)
		{
			/* short form not present. check for long form */
			if (glbSipParserMemStrStr(pOut,sipheaderlength,\
				(SIP_S8bit *)"\r\nContent-Length:",&ctypeoffset)==SipFail)
			{
				/* Now insert content-dLength at the bottom of the headers */

				ctypeoffset = sipheaderlength-2;
				if((options->dOption&(SIP_OPT_SHORTFORM))==(SIP_OPT_SHORTFORM))
					(void)sip_sprintf(constructedclen,(SIP_S8bit *)"l: %d\r\n",\
						bodylength);
				else
					(void)sip_sprintf(constructedclen,(SIP_S8bit*)"Content-Length: %d\r\n", \
					bodylength);
			}
			else
			{
				(void)sip_sprintf(constructedclen,\
					(SIP_S8bit*)"Content-Length: %d\r\n",bodylength);
			}
		}
		else
		{
			(void)sip_sprintf(constructedclen,(SIP_S8bit*)"l: %d\r\n",bodylength);
		}


		ctypehdrlen = sip_strlen(constructedclen);
		if (glbSipParserMemStrStr(&(pOut[ctypeoffset+2]), \
			sipheaderlength-ctypeoffset-2, (SIP_S8bit *)"\r\n", \
			&crlfoffset)==SipFail)
		{
		SIP_U32bit dTempLength=0 ;			
		/* No CRLF found-message did not have a content length header */
		crlfoffset = 0;
		if(bodylength!=0)
		{
				dTempLength=sipheaderlength+bodylength+\
						4-(ctypeoffset+2+crlfoffset)+1;

				memmove(&(pOut[ctypeoffset+ctypehdrlen]),\
								&(pOut[ctypeoffset+crlfoffset]),\
								dTempLength);

		}
		else
		{
				dTempLength=sipheaderlength+2-(ctypeoffset+2+crlfoffset)+1;
				memmove(&(pOut[ctypeoffset+ctypehdrlen]),\
								&(pOut[ctypeoffset+crlfoffset]),\
								dTempLength);
		}
		*dLength = ctypehdrlen+sipheaderlength+bodylength-crlfoffset+2;
		}
		else
		{
					memmove(&(pOut[ctypeoffset+ctypehdrlen]), \
						&(pOut[ctypeoffset+crlfoffset+2]), \
						sipheaderlength+bodylength+2-(ctypeoffset+2+crlfoffset)+1);
		}

		memmove(&(pOut[ctypeoffset+2]),constructedclen,ctypehdrlen);
		*dLength = ctypehdrlen+sipheaderlength+bodylength-crlfoffset+2;
	}/*OPT_CLEN*/
	else
		*dLength = sipheaderlength + bodylength + 2;

	if (bodylength==0) 
	{
		(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
	}
	return retval;
}

/****************************************************************************
** FUNCTION: sip_formMimeBody
** DESCRIPTION: This function forms the Body part of the SIP Message.
** 
** PARAMETERS:
**				pDummyBuf(IN)	- This param is currently not used; could be 
**								  used later to perform buffer-overflow checks.
**				mbodyList(IN)	- The list of decoded message bodies 
**				ctypehdr(IN)	- The Content-Type hdr of the SipMessage
**				out(OUT)		- The output text buffer 
**				dLength(OUT)	- Length of the output buffer 
**				err(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formMimeBody
#ifdef ANSI_PROTO
   (
    /* MICROUA begin */
	SIP_S8bit			  *pDummyBuf,
   /* MICROUA end */
    SipList 			  mbodyList,
	SipContentTypeHeader *ctypehdr,
	SIP_S8bit 			 *out,
	SIP_U32bit 			 *dLength,
	SipError 			 *err)
#else
	(mbodyList, ctypehdr,out, dLength, err)
	SipList mbodyList;
	SipContentTypeHeader *ctypehdr;
	SIP_S8bit *out;
	SIP_U32bit *dLength;
	SipError *err;
#endif
{
	SIP_U32bit		count,i;	
	SipMsgBody		*msgbody;
	SIP_S8bit		*separator;
	SIP_S8bit		*tempout;
#ifdef  SIP_MIME_PARSING
	SIP_S8bit		*boundary;
    SIP_U32bit		iterator,index1,size1;
#endif    
	/* MICROUA begin */
	(void) pDummyBuf;
	/* MICROUA end */

	tempout = out;
	if (sip_listSizeOf(&(mbodyList),&count,err)!=SipSuccess)
		return SipFail;

	/* Do a validity check for parameters */
	if ((ctypehdr == SIP_NULL)||(count==0))
	{	
		*dLength = 0;
		return SipSuccess;
	}
	
	if (ctypehdr != SIP_NULL)
	{
#ifdef SIP_MIME_PARSING
		/* Check if multipart and find separator if it is multipart */	
		if (sip_strcasecmp(ctypehdr->pStr1,\
			(SIP_S8bit*)"multipart/mixed")==0)
		{
			if (sip_listSizeOf(&(ctypehdr->slParam),&count,err)==SipFail)
				return SipFail;
			iterator = 0;
			boundary = SIP_NULL;
			while (iterator < count)
			{
				SipParam	*param;

				if (sip_listGetAt(&(ctypehdr->slParam),iterator, \
					(SIP_Pvoid *)&(param),err)==SipFail)
					return SipFail;

				if (sip_strcmp(param->pName,(SIP_S8bit*)"boundary")==0)
				{
					if (sip_listGetAt(&(param->slValue), 0, \
						(SIP_Pvoid*)&boundary, err)==SipFail)
						return SipFail;
					break;
				}
				iterator++;
			}
			if (boundary == SIP_NULL)
				return SipFail;

			/* Split message Body into constituent elements */
			separator = (SIP_S8bit *)fast_memget(ENCODE_MEM_ID, \
				sizeof(SIP_S8bit)*sip_strlen(boundary)+5,err);
			if (separator == SIP_NULL)
				return SipFail;
			(void)sip_strcpy(separator,(SIP_S8bit*)"--");

			if (boundary[0] == '\"')
			{
				(void)sip_strcat(separator, &(boundary[1]));
				separator[sip_strlen(boundary)] = '\0';
			}
			else
				(void)sip_strcat(separator,boundary);

			/* 
			 * Check if the multipart message has been read into unknown
			 * body. There will be one MsgBody in the list and its type 
			 * will be SipUnknownBody. The body will begin with the
			 * boundary.
			 */

			if (sip_listSizeOf(&(mbodyList),&count,err)!=SipSuccess)
				return SipFail;
			if (count == 1)
			{
				SipMsgBody	*tempmsgbody;
				if (sip_listGetAt(&(mbodyList),0,(SIP_Pvoid *)&tempmsgbody,err)\
					==SipFail)
					return SipFail;
				if (tempmsgbody->dType == SipUnknownBody)
				{
					if (sip_strncmp(tempmsgbody->u.pUnknownMessage->pBuffer,separator,\
						sip_strlen(separator))==0)
					{
						/* separator shouldn't be printed out. */
						(void)fast_memfree(0,separator,err);
						separator=SIP_NULL;
					}
				}
			}
		}
		else 
#endif /* SIP_MIME_PARSING */
			separator=SIP_NULL;
	}
	else 
		separator=SIP_NULL;
	count=0;

	/* Iterate through the list of message bodies */
	if (sip_listSizeOf(&(mbodyList),&count,err)==SipSuccess)
	{
		if (count > 0)
			(void)STR_CAT( (SIP_S8bit *)tempout,(SIP_S8bit*)CRLF);
		for (i=0;i<count;i++)
		{
			if (separator != SIP_NULL)
			{
				if (i != 0)
					(void)STR_CAT( (SIP_S8bit *)tempout,(SIP_S8bit*)CRLF);
				(void)STR_CAT( (SIP_S8bit *)tempout,separator);
				(void)STR_CAT( (SIP_S8bit *)tempout,(SIP_S8bit*)CRLF);
			}

			if (sip_listGetAt(&(mbodyList),i,\
				(SIP_Pvoid *)&msgbody,err)==SipFail)
			{
				sip_freeString(separator);
				return SipFail;
			}

#ifdef SIP_MIME_PARSING
			/* get Content-Type */
			if (msgbody->pMimeHeader != SIP_NULL)
			{
				if (msgbody->pMimeHeader->pContentType !=SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)"Content-Type: ");
					/* Fill Media type */
					if (msgbody->pMimeHeader->pContentType->pStr1 \
						!= SIP_NULL)
					{
						(void)STR_CAT(tempout, 
							msgbody->pMimeHeader->pContentType->pStr1);
					}
					(void)sip_formSipParamList(tempout, \
						&(msgbody->pMimeHeader->pContentType->slParam), \
						(SIP_S8bit *)";",1, err);
					(void)STR_CAT((SIP_S8bit *)tempout,(SIP_S8bit*)CRLF);
				}

				/* get Content-ID */
				if (msgbody->pMimeHeader->pContentId != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)"Content-ID: ");
					(void)STR_CAT(tempout, msgbody->pMimeHeader->pContentId);
					(void)STR_CAT(tempout, (SIP_S8bit*)CRLF);
				}
				
				/* get Content-Description */
				if (msgbody->pMimeHeader->pContentDescription != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)"Content-Description: ");
					(void)STR_CAT(tempout, \
						msgbody->pMimeHeader->pContentDescription);
					(void)STR_CAT(tempout, (SIP_S8bit*)CRLF);
				}
				
				/* get Content Disposition */
				if (msgbody->pMimeHeader->pContentDisposition != SIP_NULL)
				{
					(void)STR_CAT(tempout, (SIP_S8bit*)"Content-Disposition: ");
					/* Fill Event Type */
					(void)STR_CAT(tempout, msgbody->pMimeHeader->\
								pContentDisposition->pStr1);
                   	if (SipFail == sip_formSipParamList(tempout,\
						&(msgbody->pMimeHeader->pContentDisposition->slParam),\
						 (SIP_S8bit *) ";", 1, err))
					{
						return SipFail;
					}	
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
				}

				/* get ContentTransEncoding */
				if (msgbody->pMimeHeader->pContentTransEncoding != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)"Content-Transfer-Encoding: ");
					(void)STR_CAT(tempout, \
						msgbody->pMimeHeader->pContentTransEncoding);
					(void)STR_CAT(tempout, (SIP_S8bit*)CRLF);
				}

				/* Form Unknown Mime headers */
				(void)sip_listSizeOf(&(msgbody->pMimeHeader->slAdditionalMimeHeaders)\
					,&size1,err);
				index1=0;
				while(index1<size1)
				{
					SIP_S8bit	*unknownHeader;

					(void)sip_listGetAt(&(msgbody->pMimeHeader->\
						slAdditionalMimeHeaders),index1, (SIP_Pvoid *)\
						&unknownHeader, err);
					(void)STR_CAT(tempout,unknownHeader);
					(void)STR_CAT(tempout, (SIP_S8bit*)CRLF);
					index1++;
				}
			}
			/*begin SPR1_0_2*/
			else 
#endif /* SIP_MIME_PARSING */
			{
				if (separator != SIP_NULL)
				{
					(void)STR_CAT(tempout, (SIP_S8bit*)CRLF);
				}
			/*end SPR1_0_2*/
			}

			*dLength += sip_strlen(tempout);
			tempout = &tempout[sip_strlen(tempout)];
			if (msgbody->dType == SipSdpBody)
			{
				/* Form SDP Body*/
#ifdef SIP_MIME_PARSING
				if (msgbody->pMimeHeader != SIP_NULL)
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
#endif
				(void)sip_formSdpBody(msgbody->u.pSdpMessage, tempout, err);
				*dLength += sip_strlen(tempout); 
				tempout = &tempout[sip_strlen(tempout)];
			}
#ifdef SIP_MWI
            else if(msgbody->dType == SipMessageSummaryBody)
            {
                /* Form MWI Body*/
#ifdef SIP_MIME_PARSING
				if (msgbody->pMimeHeader != SIP_NULL)
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
#endif
				(void)sip_formMesgSummaryBody(msgbody->u.pSummaryMessage, \
                                              tempout, err);
				*dLength += sip_strlen(tempout); 
				tempout = &tempout[sip_strlen(tempout)];
            }
#endif
			else if (msgbody->dType == SipUnknownBody)
			{
				/* Form Unknown Body */
#ifdef SIP_MIME_PARSING
				if (msgbody->pMimeHeader != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
					tempout+=2;
					*dLength+=2;
				}
#endif
				memcpy(tempout, msgbody->u.pUnknownMessage->pBuffer, \
					msgbody->u.pUnknownMessage->dLength);
				/* -1 removed */
				tempout += msgbody->u.pUnknownMessage->dLength;
				*dLength += msgbody->u.pUnknownMessage->dLength;
				*tempout = '\0';
			}
#ifdef SIP_ISUP
			else if (msgbody->dType == SipIsupBody)
			{
				/* Form ISUP Message */
#ifdef SIP_MIME_PARSING
				if (msgbody->pMimeHeader != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
					tempout+=2;
					*dLength+=2;
				}
#endif
				memcpy(tempout,msgbody->u.pIsupMessage->pBody,\
					msgbody->u.pIsupMessage->dLength);
				/* -1 removed */
				tempout += msgbody->u.pIsupMessage->dLength;
				*dLength += msgbody->u.pIsupMessage->dLength;
				*tempout = '\0';
			}
#endif /* SIP_ISUP */
#ifdef SIP_MIME_PARSING
			else if (msgbody->dType == SipMultipartMimeBody)
			{
				/* Form a Multipart message */
				SIP_U32bit lengthchange;

				lengthchange = *dLength;
				/* Call sip_formMimeBody recursively in case of multipart */
				if (msgbody->u.pMimeMessage != SIP_NULL)
					(void)sip_formMimeBody(SIP_NULL, msgbody->u.pMimeMessage->slRecmimeBody, \
						msgbody->pMimeHeader->pContentType,tempout,dLength,err);
				lengthchange = (*dLength)-lengthchange;
				tempout = tempout+lengthchange;
			}
#endif /* SIP_MIME_PARSING */
#ifdef SIP_SIPMSGBODY
			else if (msgbody->dType == SipAppSipBody)
			{
				/* Form SIP message in body */
				SIP_U32bit lengthchange;
				SipOptions formOption;

#ifdef SIP_MIME_PARSING
				if (msgbody->pMimeHeader != SIP_NULL)
				{
					(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
					tempout+=2;
					*dLength+=2;
				}
#endif
				formOption.dOption = SIP_OPT_FULLFORM | SIP_OPT_SINGLE;
				/*Calculate the remaining Length */
				lengthchange = SIP_MAX_MSG_SIZE - *dLength;
				if (sip_formMessage(msgbody->u.pAppSipMessage, &formOption,
					tempout, &lengthchange, err)==SipFail)
				{
					return SipFail;
				}
				*dLength = *dLength + lengthchange;
				tempout+=lengthchange;
			}
#endif /* SIP_SIPMSGBODY */
		} /* end of for loop */
		
		if (separator != SIP_NULL)
		{
			(void)STR_CAT(tempout,(SIP_S8bit*)CRLF);
			tempout += 2;
			*dLength += 2;
			(void)sip_strcat(separator,(SIP_S8bit*)"--");
			(void)sip_strcat(tempout,separator);
			(void)sip_strcat(tempout,(SIP_S8bit*)CRLF);
		}
	}
	else 
	{
		sip_freeString(separator);
		return SipFail;
	}
	*dLength += sip_strlen(tempout);
	tempout = &tempout[sip_strlen(tempout)];
	sip_freeString(separator);

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formEachHeader
** DESCRIPTION: This function forms each Header in the SIP Message .
** 
** PARAMETERS:
**
** NOTE: 
****************************************************************************/

SipBool sip_formEachHeader 
#ifdef ANSI_PROTO
	(SIP_U32bit cpiter,
	 SipOptions *opt,
	 SipMessage *pSipMsg, 
	 SIP_S8bit *pOut,
	 SIP_U32bit siplistmap[], 
	 SipError *pError)
#else
	(cpiter, opt, pSipMsg, pOut, siplistmap,pError)
	SIP_U32bit cpiter;
	SipOptions *opt;
	SipMessage *pSipMsg;
	SIP_S8bit *pOut;
	SIP_U32bit siplistmap[];
	SipError *pError;
#endif
{
	SIP_S8bit		*temp;
	SIP_U32bit		hdrcount,i,initial;
	en_HeaderType	hdrtype;

	(void)sip_getHeaderCountFromHeaderLine(pSipMsg,cpiter,&hdrcount,pError);
	(void)sip_getHeaderTypeAtHeaderLine(pSipMsg,cpiter,&hdrtype,pError);
	initial=siplistmap[hdrtype];
	siplistmap[hdrtype]+=hdrcount;
 	/*begin SPR1_0_5*/
	(void)sip_formSingleHeader(hdrtype,opt,initial,SipModeNew,pSipMsg, \
		(SIP_U16bit)((hdrcount > 1)?0:1), pOut,pError);
 	/*end SPR1_0_5*/
	temp = &(pOut[sip_strlen(pOut)]);

 	/*begin SPR1_0_5*/
	for(i=initial+1;i<(initial+hdrcount);i++)
	{
		temp-=2;
		(void)sip_formSingleHeader(hdrtype,opt,i,SipModeJoin,pSipMsg, \
			(SIP_U16bit)((i==initial+hdrcount-1)?1:0),temp,pError);
		temp = &(pOut[sip_strlen(pOut)]);
	}
 	/*end SPR1_0_5*/

	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_formSingleHeader
** DESCRIPTION: This function constructs the specified Header from the SipMessage
** 
** PARAMETERS:
**				dHdrType(IN)	- The header type to be formed 
**				ndx(IN)			- The index_1 of the header to be formed 
**				mode(IN)		- The mode in which the hdr is to be formed 
**				s(IN)			- The SipMessage structure 
**				insertCRLF(IN)  - Indicates whether CRLF is to be added at the
**								  end of the encoded hdr line or not
**				pOut(OUT)		- The output buffer containing the encoded message
**				pError(OUT)		- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formSingleHeader
#ifdef ANSI_PROTO
   (en_HeaderType dHdrType,
	SipOptions *popt,
	SIP_U32bit ndx,
	en_AdditionMode mode,
	SipMessage *s,
	SIP_U16bit insertCRLF,
	SIP_S8bit *pOut,
	SipError *pError)
#else
	(hdrtype, popt, ndx, mode,s, insertCRLF, pOut, pError)
	en_HeaderType dHdrType;
	SipOptions *popt;
	SIP_U32bit ndx;
	en_AdditionMode mode;
	SipMessage *s;
	SIP_U16bit insertCRLF;
	SIP_S8bit *pOut;
	SipError *pError;
#endif
{
	en_HeaderForm 		 form;
	SIP_U32bit 			 abs_line;
	SipGeneralHeader	*g;
	SipBool				 res = SipFail;
	SIP_S8bit            *pToTag=SIP_NULL;
	
	if (pError==SIP_NULL) 
		return SipFail;
		
	if (s==SIP_NULL) 
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
	g = s->pGeneralHdr;

	if (sip_getHeaderPositionFromIndex(s, dHdrType, ndx, &abs_line, \
		SIP_NULL, pError) == SipFail)
		return SipFail;

	if (sip_getHeaderFormFromHeaderLine(s,abs_line,&form,pError) == SipFail)
		return SipFail;
	
	switch (dHdrType)
	{
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactWildCard:
		case SipHdrTypeContactAny:
		{
			/* process  contact hdrs */
			SipContactHeader *ch;

			res = sip_listGetAt (&(g->slContactHdr), \
				ndx, (SIP_Pvoid *) &ch, pError);
			if (res == SipFail)
			{
				return SipFail;
			}
			sip_formHeaderName(mode, form, pOut,\
				(SIP_S8bit*) "Contact: ", (SIP_S8bit*)"m: ");
			/* if its a *, then all others need not be accessed */

#ifdef SIP_INCREMENTAL_PARSING			
			if (ch->pBuffer != SIP_NULL)
				(void)STR_CAT( (SIP_S8bit *)pOut, ch->pBuffer);
			else
			{
#endif
				if (ch->dIntVar1 == SipContactWildCard)
				{
					(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*) "*");
					break;
				}

				/* we come here if this contact Header is not a "*" */
				/* Fill display name */
				if (ch->pStr1 != SIP_NULL) 
				{
					if (sip_strcmp(ch->pStr1,(SIP_S8bit*)"")!=0)  
					{
						if (ch->pStr1[0]!=' ')
						{
							(void)STR_CAT ( (SIP_S8bit *)pOut, ch->pStr1);
						}	
						else
							(void)STR_CAT ( (SIP_S8bit *)pOut, &(ch->pStr1[1]));
					}
				}
				if (ch->pAddrSpec != SIP_NULL)
				{
					(void)STR_CAT ( (SIP_S8bit *)pOut,(SIP_S8bit*)"<"); 
					(void)sip_formAddrSpec (SIP_NULL, pOut, ch->pAddrSpec, pError );
					(void)STR_CAT ( (SIP_S8bit *)pOut,(SIP_S8bit*)">"); 
				} /* end of if addrspec */

				(void)sip_formSipParamList(pOut, &(ch->slParam), \
					(SIP_S8bit *)";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		} /* end of if contact hdr present */

		case SipHdrTypeCseq:
		{
			SIP_S8bit cs[11];

			if (VALIDATE_HEADER(g->pCseqHdr, ndx, pError) == SipFail)
				return SipFail;

			/* Fill Sequence number */
			(void)sip_snprintf((SIP_S8bit*)cs, 11, "%lu",g->pCseqHdr->dIntVar1);
			cs[ 11-1]='\0';
			
			sip_formHeaderName(mode, form, pOut, (SIP_S8bit*)"CSeq: ", \
				(SIP_S8bit*)"CSeq: ");
			STRCAT_AND_SPACE ((SIP_S8bit *)pOut, cs);
			/* Fill method */
			(void)STR_CAT(pOut, g->pCseqHdr->pStr1);
			break;
		}

		case SipHdrTypeFrom:
		case SipHdrTypeTo:
		case SipHdrTypeRecordRoute:
		{
			if (SipFail == sip_formFromFamily(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
		}
    	case SipHdrTypeRoute:
		  if ((s->dType == SipMessageRequest) &&
			(popt->dOption != 0) && 
			((popt->dOption & (SIP_OPT_REMOVEROUTE)) == (SIP_OPT_REMOVEROUTE)))
    	  {
      		if(strcmp(s->u.pRequest->pRequestLine->pStr1,"REGISTER")==0)
        		insertCRLF=0;
      		else
      		{ 
        		sip_getTagfromFromHeader(s->pGeneralHdr->pToHdr,&pToTag,pError);

        		if (pToTag != SIP_NULL)
        		{
          			if (SipFail == sip_formFromFamily(pOut, s, ndx, mode, form,\
										dHdrType, pError))
          			return SipFail;
        		}
        		else
          			insertCRLF =0;
      		}
      		break;
    	  }
    	  else
    	  {
       		if (SipFail == sip_formFromFamily(pOut, s, ndx, mode, form, \
										dHdrType, pError))
       		return SipFail;
	   		break;
    	  }
#ifdef SIP_REFER
	   	case SipHdrTypeReferredBy:
		case SipHdrTypeReferTo:
#endif
#ifdef SIP_3GPP
		case SipHdrTypePath:
		case SipHdrTypePAssociatedUri: 
		case SipHdrTypePCalledPartyId: 
		case SipHdrTypeServiceRoute: 
		case SipHdrTypeHistoryInfo:
#endif
		{
			if (SipFail == sip_formFromFamily(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
		}
#ifdef SIP_3GPP
		case SipHdrTypePVisitedNetworkId: 
		case SipHdrTypePcfAddr: 
		case SipHdrTypeAcceptContact:
		case SipHdrTypeRejectContact:
		case SipHdrTypePanInfo:   
		case SipHdrTypeJoin:
		case SipHdrTypePcVector:  
	
		{
			if (SipFail == sip_form3GPPHeader(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
		}

#endif

#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		{
			if (VALIDATE_HEADER(g->pTimeStampHdr, ndx, pError) == SipFail)
				return SipFail;
				
			sip_formHeaderName(mode, form, pOut, (SIP_S8bit*)"Timestamp: ", \
				(SIP_S8bit*)"Timestamp: ");
#ifdef SIP_INCREMENTAL_PARSING
			if( g->pTimeStampHdr->pBuffer !=SIP_NULL )
			{
				(void)STR_CAT((SIP_S8bit *)pOut,g->pTimeStampHdr->pBuffer);
			}	
			else
			{
#endif
				/* Fill time */
				if (SIP_NULL != g->pTimeStampHdr->pStr2) /* if delay non null */
					STRCAT_AND_SPACE( (SIP_S8bit *)pOut,\
						g->pTimeStampHdr->pStr1);
				else
					(void)STR_CAT( (SIP_S8bit *)pOut,g->pTimeStampHdr->pStr1);
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pTimeStampHdr->pStr2);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
			
		} /* of Time Stamp */
#endif		

#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterSec:
		case SipHdrTypeRetryAfterAny:
#else
		case SipHdrTypeRetryAfter:
#endif /* SIP_DATE */
		{
			if (VALIDATE_HEADER(g->pRetryAfterHdr, ndx, pError) == SipFail)
				return SipFail;
				
			sip_formHeaderName(mode, form, pOut, (SIP_S8bit*)"Retry-After: ", \
				(SIP_S8bit*)"Retry-After: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pRetryAfterHdr->pBuffer != SIP_NULL)
			{
				(void)STR_CAT((SIP_S8bit *)pOut,g->pRetryAfterHdr->pBuffer);
			}	
			else
			{
#endif

#ifdef SIP_DATE
				if (g->pRetryAfterHdr->dIntVar1 == SipExpSeconds)
#endif /* SIP_DATE */
				{
					SIP_S8bit clen[20];

#ifdef SIP_DATE
					(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
						g->pRetryAfterHdr->u.dSec);
#else
					/* Fill dSec */
					(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
						g->pRetryAfterHdr->dIntVar1);
#endif
					clen[20-1]='\0';
					(void)STR_CAT ((SIP_S8bit *)pOut,clen);
				}
#ifdef SIP_DATE
				else if (SipFail == sip_formDateStruct\
					(&pOut, g->pRetryAfterHdr->u.pDate, pError))
				{
					return SipFail;
				}
#endif /* SIP_DATE */
				
				/* Fill comments if present */
				if (g->pRetryAfterHdr->pStr1 !=SIP_NULL)
				{
					(void)STR_CAT ((SIP_S8bit *)pOut, (SIP_S8bit *)" ");
					(void)STR_CAT ((SIP_S8bit *)pOut, g->pRetryAfterHdr->pStr1);
				}

				(void)sip_formSipParamList(pOut, &(g->pRetryAfterHdr->slParam), \
					(SIP_S8bit *) ";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		} /* end of RetryAfter */

		case SipHdrTypeVia:
		/* Process Via Header */
		{
			SipViaHeader *vh;

			res = sip_listGetAt (&(g->slViaHdr), ndx, \
				(SIP_Pvoid *) &vh, pError);
			if (res == SipFail )
			{
				return SipFail;
			}
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Via: ", \
				(SIP_S8bit*)"v: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (vh->pBuffer !=SIP_NULL)
			{
				(void)STR_CAT( (SIP_S8bit *)pOut,vh->pBuffer);
			}
			else
			{
#endif
				/* Fill Sent Protocol */
				STRCAT_AND_SPACE ( (SIP_S8bit *)pOut, vh->pStr1);
				/* Fill Sentby */
				(void)STR_CAT ( (SIP_S8bit *)pOut, vh->pStr2);
				(void)sip_formSipParamList(pOut, &(vh->slParam), \
					(SIP_S8bit *)";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		} /* end of Via Header */

		case SipHdrTypeContentLength:
		{
			SIP_S8bit clen[20];

			if (VALIDATE_HEADER(g->pContentLengthHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Content-Length: ",\
				(SIP_S8bit*) "l: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pContentLengthHdr->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pContentLengthHdr->pBuffer);
			else
			{
#endif
				/* Fill length */
				(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
					g->pContentLengthHdr->dIntVar1);
				clen[ 20-1]='\0';
				(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

		case SipHdrTypeMaxforwards:
		{
			SIP_S8bit clen[20];

			if (VALIDATE_HEADER(s->u.pRequest->pRequestHdr->pMaxForwardsHdr, ndx, \
				pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Max-Forwards: ",\
				(SIP_S8bit*) "Max-Forwards: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pRequest->pRequestHdr->pMaxForwardsHdr->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,\
					s->u.pRequest->pRequestHdr->pMaxForwardsHdr->pBuffer);
			else
			{
#endif
				/* Fill length */
				(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
					s->u.pRequest->pRequestHdr->pMaxForwardsHdr->dIntVar1);
				clen[ 20-1]='\0';
				(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
		{
			SIP_S8bit clen[20];

			if (VALIDATE_HEADER(g->pMinSEHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Min-SE: ",\
				(SIP_S8bit*) "Min-SE: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pMinSEHdr->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pMinSEHdr->pBuffer);
			else
			{
#endif
				/* Fill length */
				(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
					g->pMinSEHdr->dIntVar1);
				clen[ 20-1]='\0';
				(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
				(void)sip_formSipParamList(pOut, &(g->pMinSEHdr->slParam), \
					(SIP_S8bit *)";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
		case SipHdrTypeSessionExpires:
		{
			SIP_S8bit clen[20];

			if (VALIDATE_HEADER(g->pSessionExpiresHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Session-Expires: ",\
				(SIP_S8bit*) "x: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pSessionExpiresHdr->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pSessionExpiresHdr->pBuffer);
			else
			{
#endif
				/* Fill length */
				(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
					g->pSessionExpiresHdr->dIntVar1);
				clen[ 20-1]='\0';
				(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
				(void)sip_formSipParamList(pOut, &(g->pSessionExpiresHdr->slParam), \
					(SIP_S8bit *)";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif /* SIP_SESSIONTIMER */


#ifdef SIP_DATE
		case SipHdrTypeExpiresDate:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresAny:
#else
		case SipHdrTypeExpires:
#endif /* SIP_DATE */
		{
			if (VALIDATE_HEADER(g->pExpiresHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Expires: ",\
				(SIP_S8bit*) "Expires: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pExpiresHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pExpiresHdr->pBuffer);
			else
			{
#endif
#ifdef SIP_DATE
				if (g->pExpiresHdr->dIntVar1 == SipExpSeconds)
#endif /* SIP_DATE */
				{
					/* delta-seconds format */
					SIP_S8bit clen[20];

#ifdef SIP_DATE
					(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
						g->pExpiresHdr->u.dSec);
#else
					/* Fill Seconds */
					(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
						g->pExpiresHdr->dIntVar1);
#endif
					clen[ 20-1]='\0';
					(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
				}
#ifdef SIP_DATE
				else
				{
					/* date format */
					if (SipFail == sip_formDateStruct\
							(&pOut, g->pExpiresHdr->u.pDate, pError))
					{
						return SipFail;
					}
					break;
				}
#endif /* SIP_DATE */
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#ifdef SIP_PRIVACY		
		case SipHdrTypePrivacy:
		{
			/*SIP_S8bit clen[20];*/

			if (VALIDATE_HEADER(g->pPrivacyHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Privacy: ",\
				(SIP_S8bit*) "Privacy: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pPrivacyHdr->pBuffer != SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pPrivacyHdr->pBuffer);
			else
			{
#endif
				(void)sip_formSipParamList(pOut, &(g->pPrivacyHdr->slParam), \
					(SIP_S8bit *)";", 0, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
        
		case SipHdrTypePPreferredId:
		case SipHdrTypePAssertId:
		{
			if (SipFail == sip_formFromFamily(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
		}	
#endif  /* SIP_PRIVACY */
		case SipHdrTypeMinExpires:
		{
			if (VALIDATE_HEADER(s->u.pResponse->pResponseHdr->pMinExpiresHdr,\
				ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Min-Expires: ",\
				(SIP_S8bit*) "Min-Expires: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pResponse->pResponseHdr->pMinExpiresHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,\
				s->u.pResponse->pResponseHdr->pMinExpiresHdr->pBuffer);
			else
			{
#endif
				{
					/* delta-seconds format */
					SIP_S8bit clen[20];

					/* Fill Seconds */
					(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
						s->u.pResponse->pResponseHdr->pMinExpiresHdr->dIntVar1);
					clen[ 20-1]='\0';
					(void)STR_CAT ( (SIP_S8bit *)pOut,clen);
				}
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
		
		case  SipHdrTypeContentType:
		{
			if (VALIDATE_HEADER(g->pContentTypeHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,\
				(SIP_S8bit*) "Content-Type: ",(SIP_S8bit*) "c: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pContentTypeHdr->pBuffer != SIP_NULL)
				(void)STR_CAT( (SIP_S8bit *)pOut,g->pContentTypeHdr->pBuffer);
			else
			{
#endif
				/* Fill Media Type */
				(void)STR_CAT( (SIP_S8bit *)pOut, g->pContentTypeHdr->pStr1);
				(void)sip_formSipParamList(pOut, &(g->pContentTypeHdr->slParam), \
					(SIP_S8bit *) ";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

		case  SipHdrTypeAccept:
		{
			SipAcceptHeader *ah;

			res = sip_listGetAt (&(g->slAcceptHdr), \
				ndx, (SIP_Pvoid *) &ah, pError);
			if (res == SipFail)
			{
				return SipFail;
			}

			sip_formHeaderName(mode, form, pOut,\
				(SIP_S8bit*) "Accept: ",(SIP_S8bit*) "Accept: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (ah->pBuffer != SIP_NULL)
				(void)STR_CAT( (SIP_S8bit *)pOut,ah->pBuffer);
			else
			{
#endif
				/* Fill Media Type */
				(void)STR_CAT( (SIP_S8bit *)pOut, ah->pStr1);
				/* Fill Accept Param(q value) if present */
				if(ah->pStr2 != SIP_NULL)
				{
					(void)STR_CAT( (SIP_S8bit *)pOut, ah->pStr2);
				}	
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

	case SipHdrTypeContentDisposition: 
		/* Process ContentDisposition Header */
		{
			SipContentDispositionHeader *vh;

			res = sip_listGetAt (&(g->slContentDispositionHdr), ndx, \
				(SIP_Pvoid *) &vh, pError);
			if (res == SipFail )
			{
				return SipFail;
			}
			sip_formHeaderName(mode, form, pOut,\
				(SIP_S8bit*) "Content-Disposition: ", \
				(SIP_S8bit*) "Content-Disposition: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (vh->pBuffer !=SIP_NULL)

			{
				(void)STR_CAT( (SIP_S8bit *)pOut,vh->pBuffer);
			}
			else
			{
#endif
				/* Fill EventType */
				(void)STR_CAT( (SIP_S8bit *)pOut, vh->pStr1);
				(void)sip_formSipParamList(pOut, &(vh->slParam), \
					(SIP_S8bit *)";", 1, pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#ifdef SIP_RPR		
		case SipHdrTypeRAck:
		{
			SIP_S8bit cs[11];
			SIP_S8bit rs[11];

			if (VALIDATE_HEADER(s->u.pRequest->pRequestHdr->pRackHdr, \
				ndx, pError) == SipFail)
				return SipFail;

			sip_formHeaderName(mode, form, pOut, \
				(SIP_S8bit*)"RAck: ", (SIP_S8bit*)"RAck: ");
			/* Fill Resp Num */
			(void)sip_snprintf((SIP_S8bit*)rs, 11, "%lu",\
				s->u.pRequest->pRequestHdr->pRackHdr->dIntVar1);
			rs[ 11-1]='\0';
			/* Fill CSeq Num */
			(void)sip_snprintf((SIP_S8bit*)cs, 11, "%lu",\
				s->u.pRequest->pRequestHdr->pRackHdr->dIntVar2);
			cs[ 11-1]='\0';
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pRequest->pRequestHdr->pRackHdr->pBuffer !=SIP_NULL)
			{
				(void)STR_CAT( (SIP_S8bit *)pOut,s->u.pRequest->pRequestHdr->pRackHdr->pBuffer);
			}
			else
			{
#endif
				STRCAT_AND_SPACE ((SIP_S8bit *)pOut, rs);
				STRCAT_AND_SPACE ((SIP_S8bit *)pOut, cs);
				/* Fill Method */
				(void)STR_CAT(pOut,s->u.pRequest->pRequestHdr->pRackHdr->pStr1);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif
#ifdef SIP_IMPP
		case  SipHdrTypeEvent:
		{
			if (VALIDATE_HEADER(s->u.pRequest->pRequestHdr->pEventHdr, ndx,\
				 pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Event: ", \
				(SIP_S8bit*) "o: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pRequest->pRequestHdr->pEventHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,s->u.pRequest->pRequestHdr->\
					pEventHdr->pBuffer);
			else
			{
#endif
				/* Fill Event Type */
				(void)STR_CAT( (SIP_S8bit *)pOut, s->u.pRequest->pRequestHdr->pEventHdr->\
					pStr1);
				(void)sip_formSipParamList(pOut,&(s->u.pRequest->pRequestHdr->\
					pEventHdr->slParam),(SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif

#ifdef SIP_REPLACES
		case  SipHdrTypeReplaces:
		{
			if (VALIDATE_HEADER(s->u.pRequest->pRequestHdr->pReplacesHdr, ndx,\
				pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Replaces: ", \
				(SIP_S8bit*) "Replaces: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pRequest->pRequestHdr->pReplacesHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,s->u.pRequest->pRequestHdr->\
					pReplacesHdr->pBuffer);
			else
			{
#endif
				/* Fill Event Type */
				(void)STR_CAT( (SIP_S8bit *)pOut, s->u.pRequest->pRequestHdr->\
					pReplacesHdr->pStr1);
				(void)sip_formSipParamList(pOut,&(s->u.pRequest->pRequestHdr->\
					pReplacesHdr->slParam), (SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif /* SIP_REPLACES */

		case SipHdrTypeContentEncoding:
		case SipHdrTypeAllow:
#ifdef SIP_IMPP		
		case SipHdrTypeAllowEvents:
#endif		
		case SipHdrTypeRequire:
		case SipHdrTypeCallId:
		case SipHdrTypeSupported:
		case SipHdrTypeUnsupported:
#ifdef SIP_3GPP
		case SipHdrTypeRequestDisposition:
		case SipHdrTypeIfMatch:
		case SipHdrTypeETag:
#endif
	#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
#endif

		{	
			
			if (SipFail == sip_formStringFamily(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
        }
		case SipHdrTypeUnknown:
		{
			SipUnknownHeader *u;

			res = sip_listGetAt (&(g->slUnknownHdr), ndx, \
				(SIP_Pvoid *) &u, pError);
			if (res == SipFail)
			{
				return SipFail;
			}
#ifdef SIP_INCREMENTAL_PARSING			
			if (u->pBuffer!=SIP_NULL)
				(void)STR_CAT( (SIP_S8bit *)pOut, u->pBuffer);
			else
			{
#endif			
				/* Unknown headers should not be joined even if the order
				   entry indicates that. This is because unknown headers may
				   have different names though their type is different.
				*/
				insertCRLF = 1;
				mode = SipModeNew;
				if (mode == SipModeNew)
				{
					/* Fill Name */
					(void)STR_CAT( (SIP_S8bit *)pOut, u->pStr1);
					(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*) ": ");
				}
				else 
					(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*) ",");
				/* Fill Body*/
				(void)STR_CAT ( (SIP_S8bit *)pOut, u->pStr2);
#ifdef SIP_INCREMENTAL_PARSING
			}				
#endif
			break;
		} /* if unknown pHeader exists */
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
		case SipHdrTypeProxyauthorization:
		case SipHdrTypeProxyAuthenticate:
		case SipHdrTypeWwwAuthenticate:
		{
			if (SipFail == sip_formAuthFamily(pOut, s, ndx, mode, form, \
				dHdrType, pError))
				return SipFail;
			break;
		}
#endif
#ifdef SIP_RPR
		case SipHdrTypeRSeq:
		{
			SIP_S8bit clen[20];

			if (VALIDATE_HEADER(s->u.pResponse->pResponseHdr->pRSeqHdr, ndx, pError) \
				== SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,\
				(SIP_S8bit*) "RSeq: ",(SIP_S8bit*) "RSeq: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pResponse->pResponseHdr->pRSeqHdr->pBuffer != SIP_NULL)
				(void)STR_CAT( (SIP_S8bit *)pOut,s->u.pResponse->pResponseHdr->\
					pRSeqHdr->pBuffer);
			else
			{
#endif
			    /* Fill Length */
				(void)sip_snprintf((SIP_S8bit *)clen, 20, "%lu",\
					s->u.pResponse->pResponseHdr->pRSeqHdr->dIntVar1);
				clen[ 20-1]='\0';
				(void)STR_CAT( (SIP_S8bit *)pOut,clen);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif
#ifdef SIP_WARNING
		case SipHdrTypeWarning:
		{
			SipRespHeader	*pResponseHdr = SIP_NULL;
			SIP_S8bit dCodeNum[20];
			SipWarningHeader *wh;
	
			if (s->dType != SipMessageResponse)
				return SipFail;
			pResponseHdr = s->u.pResponse->pResponseHdr;
			res = sip_listGetAt (&(pResponseHdr->slWarningHeader), ndx, \
				(SIP_Pvoid *) &wh, pError);
	
			if (res == SipFail)
			{
				return SipFail;
			}
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Warning: ",\
				(SIP_S8bit*) "Warning: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (wh->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,wh->pBuffer);
			else
			{
#endif			
				/* Fill Code Num */
				(void)sip_snprintf((SIP_S8bit *)dCodeNum,20, "%d", wh->dIntVar1);
				dCodeNum[20-1]='\0';
				STRCAT_AND_SPACE ( (SIP_S8bit *)pOut, dCodeNum);
				/* Fill Agent */
				STRCAT_AND_SPACE ( (SIP_S8bit *)pOut, wh->pStr1);
				/* Fill text */
				(void)STR_CAT ( (SIP_S8bit *)pOut, wh->pStr2);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif		
		case SipHdrTypeMimeVersion: 
		{
			/* Process Mime-Version Header */
			if (VALIDATE_HEADER(g->pMimeVersionHdr, ndx, pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Mime-Version: ",\
				(SIP_S8bit*) "Mime-Version: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (g->pMimeVersionHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,g->pMimeVersionHdr->pBuffer);
			else
			{
#endif
				/* Fill String */
				if (g->pMimeVersionHdr->pStr1 != SIP_NULL)
					(void)STR_CAT ( (SIP_S8bit *)pOut, g->pMimeVersionHdr->pStr1);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

#ifdef SIP_IMPP
		case  SipHdrTypeSubscriptionState:
		{
			if (VALIDATE_HEADER(s->u.pRequest->pRequestHdr->pSubscriptionStateHdr, ndx,\
				 pError) == SipFail)
				return SipFail;
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Subscription-State: ", \
				(SIP_S8bit*) "Subscription-State: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (s->u.pRequest->pRequestHdr->pSubscriptionStateHdr->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,s->u.pRequest->pRequestHdr->\
					pSubscriptionStateHdr->pBuffer);
			else
			{
#endif
				/* Fill Event Type */
				(void)STR_CAT( (SIP_S8bit *)pOut, s->u.pRequest->pRequestHdr->pSubscriptionStateHdr->\
					pStr1);
				(void)sip_formSipParamList(pOut,&(s->u.pRequest->pRequestHdr->\
					pSubscriptionStateHdr->slParam),(SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}
#endif /* SIP_IMPP */
#ifdef SIP_SECURITY
		case  SipHdrTypeSecurityServer:
		{
           	SipRespHeader	*pResponseHdr = SIP_NULL;
			SipSecurityServerHeader *ssh;
	
			if (s->dType != SipMessageResponse)
				return SipFail;
           	pResponseHdr = s->u.pResponse->pResponseHdr;
			res = sip_listGetAt (&(pResponseHdr->slSecurityServerHdr), ndx, \
				(SIP_Pvoid *) &ssh, pError);
            if ( res == SipFail)                
            {
                return SipFail;
            }
            sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Security-Server: ", \
				(SIP_S8bit*) "Security-Server: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (ssh->pBuffer !=SIP_NULL)
				STR_CAT ( (SIP_S8bit *)pOut,ssh->pBuffer);
			else
			{
#endif
				/* Fill Mechanism Name*/
				STR_CAT( (SIP_S8bit *)pOut,ssh->pStr1);
				(void)sip_formSipParamList(pOut,&(ssh->slParam),(SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
        }        
		case  SipHdrTypeSecurityClient:
		{
           	SipReqHeader	*pRequestHdr = SIP_NULL;
			SipSecurityClientHeader *ssh;
	
			if (s->dType != SipMessageRequest)
				return SipFail;
           	pRequestHdr = s->u.pRequest->pRequestHdr;
			res = sip_listGetAt (&(pRequestHdr->slSecurityClientHdr), ndx, \
				(SIP_Pvoid *) &ssh, pError);
            if ( res == SipFail)                
            {
                return SipFail;
            }
            sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Security-Client: ", \
				(SIP_S8bit*) "Security-Client: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (ssh->pBuffer !=SIP_NULL)
				STR_CAT ( (SIP_S8bit *)pOut,ssh->pBuffer);
			else
			{
#endif
				/* Fill Mechanism Name*/
				STR_CAT( (SIP_S8bit *)pOut,ssh->pStr1);
				(void)sip_formSipParamList(pOut,&(ssh->slParam),(SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
        }        
		case  SipHdrTypeSecurityVerify:
		{
           	SipReqHeader	*pRequestHdr = SIP_NULL;
			SipSecurityVerifyHeader *ssh;
	
			if (s->dType != SipMessageRequest)
				return SipFail;
           	pRequestHdr = s->u.pRequest->pRequestHdr;
			res = sip_listGetAt (&(pRequestHdr->slSecurityVerifyHdr), ndx, \
				(SIP_Pvoid *) &ssh, pError);
            if ( res == SipFail)                
            {
                return SipFail;
            }
            sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Security-Verify: ", \
				(SIP_S8bit*) "Security-Verify: ");
#ifdef SIP_INCREMENTAL_PARSING
			if (ssh->pBuffer !=SIP_NULL)
				STR_CAT ( (SIP_S8bit *)pOut,ssh->pBuffer);
			else
			{
#endif
				/* Fill Mechanism Name*/
				STR_CAT( (SIP_S8bit *)pOut,ssh->pStr1);
				(void)sip_formSipParamList(pOut,&(ssh->slParam),(SIP_S8bit *) ";",1,pError);
#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
        }             
#endif        
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
		{
			SipRespHeader	*pResponseHdr = SIP_NULL;
			SipAcceptRsrcPriorityHeader *arph;
	
			if (s->dType != SipMessageResponse)
				return SipFail;
			pResponseHdr = s->u.pResponse->pResponseHdr;
			res = sip_listGetAt (&(pResponseHdr->slAcceptRsrcPriorityHdr), ndx, \
				(SIP_Pvoid *) &arph, pError);
	
			if (res == SipFail)
			{
				return SipFail;
			}
			sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Accept-Resource-Priority:",\
				(SIP_S8bit*) "Accept-Resource-Priority:");
#ifdef SIP_INCREMENTAL_PARSING
			if (arph->pBuffer !=SIP_NULL)
				(void)STR_CAT ( (SIP_S8bit *)pOut,arph->pBuffer);
			else
			{
#endif			
				/* Fill Namespace */
				STR_CAT((SIP_S8bit *)pOut,arph->pStr1);
				(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*) ".");

				/* Fill Priority Value */
				(void)STR_CAT((SIP_S8bit *)pOut,arph->pStr2);

#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
		}

		case SipHdrTypeRsrcPriority:
		{
			SipReqHeader	*pRequestHdr = SIP_NULL;
			SipRsrcPriorityHeader *rph;
	
			if (s->dType != SipMessageRequest)
				return SipFail;
           	pRequestHdr = s->u.pRequest->pRequestHdr;
			res = sip_listGetAt (&(pRequestHdr->slRsrcPriorityHdr), ndx, \
				(SIP_Pvoid *) &rph, pError);
            if ( res == SipFail)                
            {
                return SipFail;
            }
            sip_formHeaderName(mode, form, pOut,(SIP_S8bit*) "Resource-Priority:", \
				(SIP_S8bit*) "Resource-Priority:");
#ifdef SIP_INCREMENTAL_PARSING
			if (rph->pBuffer !=SIP_NULL)
				STR_CAT ( (SIP_S8bit *)pOut,rph->pBuffer);
			else
			{
#endif
				/* Fill Namespace */
				STR_CAT((SIP_S8bit *)pOut,rph->pStr1);
				(void)STR_CAT( (SIP_S8bit *)pOut,(SIP_S8bit*) ".");

				/* Fill Priority Value */
				(void)STR_CAT((SIP_S8bit *)pOut,rph->pStr2);

#ifdef SIP_INCREMENTAL_PARSING
			}
#endif
			break;
        }   

#endif
		
		default:
			*pError = E_INV_PARAM;
			return SipFail;
	}

	/* Append (SIP_S8bit*)CRLF if required */

 	/*begin SPR1_0_5*/
	if (1 == insertCRLF) 
		(void)STR_CAT ( (SIP_S8bit *)pOut,(SIP_S8bit*)CRLF);
 	/*end SPR1_0_5*/
	
	return SipSuccess;
}

#ifdef SIP_MWI
/****************************************************************************
** FUNCTION:  sip_formMesgSummaryBody
** DESCRIPTION: This function encodes an MWI msg body of a SIP message (i.e.
**				converts it to Text)
** 
** PARAMETERS:
**		pMessageSummaryMsg(IN)	- The decoded message summary message body
**		pOut(IN)	- The output buffer into which the encoded message 
**					  lines will be written into.
**		pError(OUT)	- Error value if any 
**
** NOTE: 
****************************************************************************/

SipBool sip_formMesgSummaryBody
#ifdef ANSI_PROTO
	(MesgSummaryMessage *pMessageSummaryMsg, 
	 SIP_S8bit 	*pOut, 
	 SipError 	*pError)
#else
	(pMessageSummaryMsg, pOut, pError)
	MesgSummaryMessage *pMessageSummaryMsg;
	SIP_S8bit 	*pOut;
	SipError 	*pError;
#endif
{
    SIP_U32bit dIndex, dSize;
    SummaryLine *summary;
    SipNameValuePair *namevalue;
    SIP_S8bit new[4],old[4],newurgent[4],oldurgent[4];

    /* Get the status */
    (void)STR_CAT (pOut,(SIP_S8bit*) "Messages-Waiting: ");
    if( pMessageSummaryMsg->dStatus == SipMsgWaitingNo )
        (void)STR_CAT(pOut,(SIP_S8bit*) "No");
    else
        (void)STR_CAT(pOut,(SIP_S8bit*) "Yes");

    (void)STR_CAT(pOut, (SIP_S8bit*)CRLF);

    /* Form Message-Account header */
    if (pMessageSummaryMsg->pAddrSpec != SIP_NULL)
    {
        (void)STR_CAT (pOut,(SIP_S8bit*) "Message-Account: ");
        (void)sip_formAddrSpec (SIP_NULL, pOut, \
                                pMessageSummaryMsg->pAddrSpec, pError);
        (void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
    } /*end if (pMessageSummaryMsg->pAddrSpec != SIP_NULL)  */

    /* Form the summary line */
    (void)sip_listSizeOf( &(pMessageSummaryMsg->slSummaryLine), \
                          &dSize, pError);
    dIndex = 0;
    while (dIndex < dSize)
    {
        (void)sip_listGetAt(&(pMessageSummaryMsg->slSummaryLine), \
                            dIndex, (SIP_Pvoid *) &summary, pError);
        (void)STR_CAT(pOut, summary->pMedia);
        (void)STR_CAT(pOut, (SIP_S8bit*)": ");
        (void)sip_snprintf((SIP_S8bit *)new, 4, "%lu", \
                           summary->newMessages);
        new[4-1] = '\0';
        (void)STR_CAT(pOut, new);
        (void)STR_CAT(pOut, (SIP_S8bit*)"/");
        (void)sip_snprintf((SIP_S8bit *)old, 4, "%lu", \
                           summary->oldMessages);
        old[4-1] = '\0';
        (void)STR_CAT(pOut,old);
        if((summary->newUrgentMessages)||(summary->oldUrgentMessages))
            /* if there are urgent messages */
        {
            (void)STR_CAT(pOut, (SIP_S8bit*)"(");
            (void)sip_snprintf((SIP_S8bit *)newurgent, 4, "%lu", \
                               summary->newUrgentMessages);
            newurgent[4-1] = '\0';
            (void)STR_CAT(pOut,newurgent);
            (void)STR_CAT(pOut, (SIP_S8bit*)"/");
            (void)sip_snprintf((SIP_S8bit *)oldurgent, 4, "%lu", \
                               summary->oldUrgentMessages);
            oldurgent[4-1] = '\0';
            (void)STR_CAT(pOut,oldurgent);
            (void)STR_CAT(pOut, (SIP_S8bit*)")");
        }
        (void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
        dIndex++;
    }
		if(dSize != 0)
        (void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
    /* Fill in stuff from the Name/Value pair list */
    (void)sip_listSizeOf( &(pMessageSummaryMsg->slNameValue), \
                          &dSize, pError);
    dIndex = 0;
    while (dIndex < dSize)
    {
        (void)sip_listGetAt (&(pMessageSummaryMsg->slNameValue), \
                             dIndex, (SIP_Pvoid *) &namevalue, pError);
        /*(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);*/
        (void)STR_CAT(pOut, namevalue->pName);
        (void)STR_CAT(pOut, (SIP_S8bit*)":");
        (void)STR_CAT(pOut, namevalue->pValue);
				(void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
        dIndex++;
    }
    (void)STR_CAT(pOut, (SIP_S8bit*)CRLF);
    return SipSuccess;
}
#endif

