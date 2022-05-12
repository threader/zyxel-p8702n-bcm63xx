/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 



/*******************************************************************************
** FUNCTION:
**			Accessor functionality
********************************************************************************
**
** FILENAME:
** microsip_accessor.c
**
** DESCRIPTION:
**		This file contains functions for getting and setting fields of SipMessage
**		and headers. 
**
** DATE	 	NAME		REFERENCE	REASON
** ----	 	----		---------	--------
** 15/03/2002	mahesh		Initial				
** 28/01/2009   Kamal Ashraf    IPTK Rel 8.1    APIs added
**                                              sip_getExpiresFromContactParam 
**                                              sip_getExtensionAttrFromContactParam and
**                                              sip_getTypeFromContactParam
**	COPYRIGHT , Aricent, 2009
*******************************************************************************/

#include "microsip_accessors.h"
#include "microsip_free.h"
#include "microsip_init.h"
#ifndef SIP_LINT    
#include "microsip_struct.h"
#endif    
#include "microsip_decode.h"
#include "microsip_cloneintrnl.h"
#include "microsip_subparsers.h"
#include "microsip_encodeinternal.h"
#include "microsip_hdrencoders.h"

/*******************************************************************************
** FUNCTION: sip_getTagfromFromHeader
** DESCRIPTION: This function retrieves the tag from  FromHeader 
** 
** PARAMETERS:
**      	      pSipFromHeader(IN)	:The fromHeader from which tag
** 											has to be retrieved
**				  ppTag(OUT)			:End of header body
**				  pError(IN)			:The error Value
** RETURN VALUE:
**				  SipBool	
*******************************************************************************/


SipBool sip_getTagfromFromHeader(SipFromHeader *pSipFromHeader, \
	SIP_S8bit** ppTag, SipError *pError)

{

	SIP_U32bit size = 0;
	SIP_U32bit index_1= 0;
	SIP_Pvoid pData  = SIP_NULL;
	SIP_Pvoid pStringData = SIP_NULL;
	SipParam *pSipParam = SIP_NULL;
	SipBool dflag = SipFail;

#ifndef SIP_NO_CHECK
	/* Validating parameters */

	if (pError == SIP_NULL)
		return SipFail;

	if (pSipFromHeader == SIP_NULL)
	{
		*pError = E_INV_PARAM;
		return SipFail;
	}
#endif

	/*Finding the size of slParam*/
	if( SipFail == sip_listSizeOf(&(pSipFromHeader->slParam), \
		&size, pError) )
		return SipFail;

	 while (index_1<size)
	 {
		if (SipFail == sip_listGetAt( &(pSipFromHeader->slParam), index_1,\
			&pData, pError ) )
			return SipFail;

		pSipParam = (SipParam *)pData;	

		/*Checking if pParam->pName == tag */
		if ( 0 == sip_strcasecmp( pSipParam->pName, (SIP_S8bit *)"tag" ))
		{
			if (SipFail ==sip_listGetAt( &(pSipParam->slValue), \
				0, &pStringData, pError ))
			{
				*pError = E_NO_EXIST;
				return SipFail;
			}

			if(pStringData != SIP_NULL)
				*ppTag =(SIP_S8bit*) pStringData;
			else
			{
				*pError = E_NO_EXIST;
				return SipFail;
			}
		        dflag = SipSuccess;	
			break;
		}
		index_1++;	
	 } /* End of While LOOP */
if ( SipSuccess == dflag)
		return SipSuccess;
	else
		return SipFail; 

	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_getHeaderCount
**
** DESCRIPTION: This function returns the number of headers of en_HeaderType 
** 				"dType" in the SipMessage "msg" in the variable "count".
** 				In case of headers in which "Any" dType is possible , 
**				the dType passed in en_HeaderType must  be "Any" 
**				- otherwise E_INV_TYPE is returned.
** PARAMETERS:
**      	      msg(IN)				:The SipMessage from which header count
** 											has to be retrieved.
**				  dType(IN)				:The HeaderType of which , count is
** 											to be found
**				  count(OUT)			:The count 
**				  err(OUT)				:The error Value
** Return Value:
**				  SipBool	
****************************************************************************/

SipBool sip_getHeaderCount
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, SIP_U32bit *count, SipError *err)
#else
	(msg, dType, count, err)
	SipMessage *msg;
	en_HeaderType dType;
	SIP_U32bit *count;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_getHeaderCount");
#ifndef SIP_NO_CHECK
	
	/* Validating parameters */
	
	if (err == SIP_NULL)
		return SipFail;

	if ( (msg == SIP_NULL)||(count == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* validating the dType of the pHeader passed */
	if (isSipGeneralHeader(dType) == SipTrue)
	{
		if (msg->pGeneralHdr == SIP_NULL)
		{
			*count = 0;
			return SipSuccess;
		}
		else
		{
			/*
			 * Getting the count of general headers
			 */
			if (getGeneralHdrCount(msg->pGeneralHdr,  \
				dType, count, err) == SipFail)
				return SipFail;
		}
	}
	else
	{
		switch (msg->dType)
		{
			case SipMessageRequest:
			 if (isSipReqHeader(dType) == SipTrue)
			 {	
				/*
				 * If message type is request get the count of 
				 * request headers
				 */
				
				if ((msg->u).pRequest == SIP_NULL)
				{
					*count = 0;
					return SipSuccess;
				}
				if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
				{
					*count = 0;
					return SipSuccess;
				}
				else
				{
					/*Get teh count of request headers*/
					if (getRequestHdrCount(((msg->u).pRequest)-> \
						pRequestHdr, dType, count, err) == SipFail)
						return SipFail;
				}
			 }	
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;
			case SipMessageResponse:
			 if (isSipRespHeader(dType) == SipTrue)
			 {
				/*
				 * if Message Type is response get the count
				 */
				if ((msg->u).pResponse == SIP_NULL)
				 {
					*count = 0;
					return SipSuccess;
				 }
				if (((msg->u).pResponse)->pResponseHdr == SIP_NULL)
				{
					*count = 0;
					return SipSuccess;
				}
				else
				{
					if (getResponseHdrCount(((msg->u).pResponse)-> \
						pResponseHdr,  \
						dType, count, err) == SipFail)
						return SipFail;
				}
			 }
			 else
			  {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;
			default:
				*err = E_INV_TYPE;
				 return SipFail;
		}
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getHeaderCount");
	return SipSuccess;
}


/****************************************************************************
** FUNCTION: sip_getHeaderAtIndex
**
** DESCRIPTION: This function gets the pHeader at the index_1 "index_1" 
** 		among the headers of en_HeaderType "dType" in the SipMessage "msg". 
** 		The pHeader is returned in the structure "hdr". If no headers are 
**		present in "msg " of the specified dType SipFail is returned 
**		with the error pValue set to E_NO_EXIST. In case of headers in 
**		which "Any" dType is possible, the dType passed must be "Any" 
** 		- otherwise E_INV_TYPE is returned.
** 
** 
** PARAMETERS:
**      	      msg(IN)				: The SipMessage from which header count
** 											has to be retrieved.
**				  dType(IN)				: The HeaderType of which , count is
** 											to be found
**				  hdr					: SipHeader to be retrieved.	
**				  index_1(IN)				: The index_1 from which the header has to**											be retrieved 
**				  err(OUT)				: The error Value
** Return Value:
**				  SipBool	
****************************************************************************/

SipBool sip_getHeaderAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, 	SipHeader *hdr, SIP_U32bit index_1, \
		 SipError *err)
#else
	(msg, dType, hdr, index_1, err)
	SipMessage *msg;
	en_HeaderType dType;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_getHeaderAt");	
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if ( (msg == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	
	(hdr)->dType = dType;

	if (isSipGeneralHeader(dType) == SipTrue)
	{
		/*
		 * If General header get general header 
		 */	
		if (msg->pGeneralHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		if (getGeneralHeaderAtIndex( msg->pGeneralHdr, 	\
			dType, hdr, index_1, err ) == SipFail)
			return SipFail;
	}
	else
	{
		switch (msg->dType)
		{
			case	SipMessageRequest:
			 if (isSipReqHeader(dType) == SipTrue)
			 {
					 /*
					  * Checking  , whether the header is a request header
					  */
					if ((msg->u).pRequest == SIP_NULL)
					{
						*err = E_NO_EXIST;
						return SipFail;
					}
					if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
					{
						*err = E_NO_EXIST;
						return SipFail;
					}
				/*
				 * Now if the header is a request header get response header
				 */
					if (getRequestHeaderAtIndex( ((msg->u).pRequest)-> \
						pRequestHdr, 	\
						dType, hdr, index_1, err) == SipFail)
						return SipFail;
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			case	SipMessageResponse:
			 if (isSipRespHeader(dType) == SipTrue)
			 {
				 /*
				  * Checking  whether the header is a response header
				  */
				if ((msg->u).pResponse == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				if (((msg->u).pResponse)->pResponseHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				/*
				 * Now if the header is a response header get the same
				 */
				if (getResponseHeaderAtIndex( ((msg->u).pResponse)-> \
					pResponseHdr,  \
					dType, hdr, index_1, err) == SipFail)
					return SipFail;
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			default :
				*err = E_INV_PARAM;
				 return SipFail;
		}
	}

	sip_equateTypeInSipHeader(hdr);

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getHeaderAt");
	return SipSuccess;
}

/****************************************************************************
** FUNCTION: sip_insertHeaderAtIndex
**
** DESCRIPTION:This function inserts the pHeader "hdr" at the
** 		index_1 "index_1" among the headers of en_HeaderType "dType" in the 
** 		SipMessage "msg". If the pHeader dType of "hdr" is "Any" 
** 		E_INV_TYPE is returned. If no such pHeader dType exists in the "msg" 
** 		, a new line is inserted in the messages in the end; otherwise it is 
** 		appended at the end of an existing pHeader line of the given dType.
** 
** 
** PARAMETERS:
**      	      msg(IN/OUT)			: The SipMessage to which header has
** 										  has to be inserted.
**				  dType(IN)				: The HeaderType of which , count is
** 										  to be found
**				  hdr					: SipHeader to be inserted.	
**				  index_1(IN)				: The index_1 from which the header has to**										  be retrieved 
**				  err(OUT)				: The error Value
** Return Value:
**				  SipBool	
****************************************************************************/

SipBool sip_insertHeaderAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(msg, hdr, index_1, err)
	SipMessage *msg;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	en_SipBoolean x, y;
	SIP_U32bit line_number, temp_index;
	SipHeaderOrderInfo	*header_info_row;
	en_HeaderType	dType;
	
	SIPDEBUGFN("Entering function sip_insertHeaderAt");

	x = y = SipFalse;
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;
	if ((msg == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (hdr->pHeader == SIP_NULL)
	{
		*err = E_INV_HEADER;
		return SipFail;
	}
#endif
	if (isSipGeneralHeader(hdr->dType) == SipTrue)
	 {
	 /*
	  * If header is a general header call insert general header
	  */
		if (msg->pGeneralHdr == SIP_NULL)
		{
			if (sip_initSipGeneralHeader (&(msg->pGeneralHdr), err) == SipFail)
			{
				return SipFail;
			}
			y = SipTrue; 
		}
		if (insertGeneralHeaderAtIndex(msg->pGeneralHdr, hdr, index_1, err) \
			== SipFail)
		{
			if (y == SipTrue)
				sip_freeSipGeneralHeader(msg->pGeneralHdr);
			return SipFail;
		}
	 }
	else
	{
		switch (msg->dType)
		{
			case SipMessageRequest:
			 if (isSipReqHeader(hdr->dType) == SipTrue)
			 {
/*
* If header is a request header call insert request header
*/
				if ((msg->u).pRequest == SIP_NULL)
				 {
					if (sip_initSipReqMessage (&((msg->u).pRequest)\
						, err) 	== SipFail)
						return SipFail;
					x = SipTrue;
				 }	

				if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
				{
					if (sip_initSipReqHeader (&(((msg->u).\
						pRequest)->pRequestHdr), err) == SipFail)
					{
						if (x == SipTrue)
							sip_freeSipReqMessage((msg->u).pRequest);
						return SipFail;
					}
					y = SipTrue; 
				}
				if (insertRequestHeaderAtIndex(((msg->u).pRequest)\
						->pRequestHdr, hdr, index_1, err) == SipFail)
				{
					if (x == SipTrue)
						sip_freeSipReqMessage((msg->u).pRequest);
					else if (y == SipTrue)
						sip_freeSipReqHeader(((msg->u).pRequest)->\
							pRequestHdr);
					return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			case	SipMessageResponse:
			 if (isSipRespHeader(hdr->dType) == SipTrue)
			 {
				/*
				* If header is a response header call insert response header
				*/
				if ((msg->u).pResponse == SIP_NULL)
				{
					if (sip_initSipRespMessage \
						(&((msg->u).pResponse), err) == SipFail)
						return SipFail;
					x = SipTrue;
				}
				if (((msg->u).pResponse)->pResponseHdr == SIP_NULL)
				{
					if (sip_initSipRespHeader (&(((msg->u).\
						pResponse)->pResponseHdr), err) == SipFail)
					{
						if (x == SipTrue)
							sip_freeSipRespMessage((msg->u).\
							pResponse);
						return SipFail;
					}
					y = SipTrue; 
				}
				if (insertResponseHeaderAtIndex(((msg->u).\
					pResponse)->pResponseHdr, hdr, index_1, err)\
								== SipFail)
				{
					if (x == SipTrue)
						sip_freeSipRespMessage((msg->u).pResponse);
					else if (y == SipTrue)
						sip_freeSipRespHeader(((msg->u).pResponse)\
										->pResponseHdr);
					return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;
			default :
					*err = E_INV_PARAM;
					 return SipFail;
		} /* of switch */
	}
	

	/* updating header_info_table */
	if ( index_1 == 0 )
		temp_index = 0; 
	else
		temp_index = index_1 - 1;

	dType = hdr->dType;
	if(sip_changeTypeAny(&dType, err) == SipFail)
		return SipFail;

	if ( sip_getHeaderPositionFromIndex(msg, dType, temp_index,  \
		&line_number, SIP_NULL, err) == SipFail)
	{
		if( (index_1 == 0) && ( *err == E_NO_EXIST ) ) 
		{
			/* means that pHeader does not exist in the message */
			if ( sip_listSizeOf(&(msg->slOrderInfo), &line_number, err) == \
					SipFail)   
				/* if that pHeader does not exist append 
				   at the end of message */
				return SipFail;

			if( sip_initSipHeaderOrderInfo( &header_info_row, err ) == SipFail)
				return SipFail;
		
			header_info_row->dType = dType;
			header_info_row->dTextType = SipFormFull;
			header_info_row->dNum = 1;

			if ( sip_listInsertAt( &(msg->slOrderInfo), line_number, \
					(SIP_Pvoid)(header_info_row), err ) == SipFail)
			{
				__sip_freeSipHeaderOrderInfo((SIP_Pvoid)header_info_row); 
				return SipFail;
			}
		}
		else
		{
			return SipFail;  /* error already set */
		}
	}
	else
	{
		if ( sip_listGetAt( &(msg->slOrderInfo), line_number,  \
			(SIP_Pvoid *)(&header_info_row), err) == SipFail )
			return SipFail;

		header_info_row->dNum++;

	}

	*err = E_NO_ERROR;	
	SIPDEBUGFN("Exitting function sip_insertHeaderAt"); 
	return SipSuccess;
}



/****************************************************************************
** FUNCTION: sip_setHeaderAtIndex
**
** DESCRIPTION:This function sets the pHeader "hdr" at the 
**			index_1 "index_1" among the headers of en_HeaderType "dType" in the
** 			SipMessage "msg". If the pHeader dType of "hdr" is "Any" E_INV_TYPE 
**			is returned. If no such pHeader dType exists in the "msg" at the 
** 			en_HeaderType "dType" in the message already existing but for non 
** 			SipList Headers it successfully sets the pHeader at index_1 0 
** 			( the only valid index_1 for such headers ). 
** 
** 
** PARAMETERS:
**      	      msg(IN/OUT)			: The SipMessage to which header has
** 										  has to be set.
**				  dType(IN)				: The HeaderType 
**				  hdr					: SipHeader to be set.	
**				  index_1(IN)				: The index_1 from which the header has to**										  be set 
**				  err(OUT)				: The error Value
** Return Value:
**				  SipBool	
****************************************************************************/

SipBool sip_setHeaderAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(msg, hdr, index_1, err)
	SipMessage *msg;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	en_SipBoolean x, y;
	SIP_U32bit	line_number;
	SipHeaderOrderInfo	*header_info_row;
	en_HeaderType dType;

	SIPDEBUGFN("Entering sip_setHeaderAtIndex Function");

	x = y = SipFalse;
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;
	if ((msg == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (hdr == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
	if (hdr->pHeader == SIP_NULL)
	{
		*err = E_INV_HEADER;
		return SipFail;
	}
#endif

	if (isSipGeneralHeader(hdr->dType) == SipTrue)
	{
		if (msg->pGeneralHdr == SIP_NULL)
		{
			if (sip_initSipGeneralHeader (&(msg->pGeneralHdr), err) == SipFail)
			{
				return SipFail;
			}
			y = SipTrue; 
		}
		if (setGeneralHeaderAtIndex(msg->pGeneralHdr, hdr, index_1, err) == \
			SipFail)
		{
			if (y == SipTrue)
				sip_freeSipGeneralHeader(msg->pGeneralHdr);
			return SipFail;
		}
	}
	else
	{
		switch (msg->dType)
		{
			case	SipMessageRequest:
			if (isSipReqHeader(hdr->dType) == SipTrue)
			{
				if ((msg->u).pRequest == SIP_NULL)
				{
					if (sip_initSipReqMessage (&((msg->u).pRequest), err)\
						 == SipFail)
						return SipFail;
					x = SipTrue;
				}
				if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
				{
					if (sip_initSipReqHeader (&(((msg->u).pRequest)->\
						pRequestHdr), err) == SipFail)
					{
						if (x == SipTrue)
							sip_freeSipReqMessage((msg->u).pRequest);
						return SipFail;
					}
					y = SipTrue; 
				}
				if (setRequestHeaderAtIndex(((msg->u).pRequest)->\
					pRequestHdr, hdr, index_1, err) == SipFail)
				{
					if (x == SipTrue)
						sip_freeSipReqMessage((msg->u).pRequest);
					else if (y == SipTrue)
						sip_freeSipReqHeader(((msg->u).pRequest)->\
							pRequestHdr);
					return SipFail;
				}
			}
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			case	SipMessageResponse:
			if (isSipRespHeader(hdr->dType) == SipTrue)
			{
				
				if ((msg->u).pResponse == SIP_NULL)
				{
					if (sip_initSipRespMessage (&((msg->u).pResponse), \
						err) == SipFail)
						return SipFail;
					x = SipTrue;
				}
				if (((msg->u).pResponse)->pResponseHdr == SIP_NULL)
				{
					if (sip_initSipRespHeader (&(((msg->u).pResponse)->\
						pResponseHdr), err) == SipFail)
					{
						if (x == SipTrue)
							sip_freeSipRespMessage((msg->u).pResponse);
						return SipFail;
					}
					y = SipTrue; 
				}
				if (setResponseHeaderAtIndex(((msg->u).pResponse)->\
					pResponseHdr, hdr, index_1, err) == SipFail)
				{
					if (x == SipTrue)
						sip_freeSipRespMessage((msg->u).pResponse);
					else if (y == SipTrue)
						sip_freeSipRespHeader(((msg->u).pResponse)->\
							pResponseHdr);
					return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			default:
					*err = E_INV_PARAM;
					 return SipFail;
		} /* of switch */
	}

	/* updating header_info_table */

	dType = hdr->dType;
	if(sip_changeTypeAny(&dType, err) == SipFail)
		return SipFail;
	if ( sip_getHeaderPositionFromIndex(msg, dType, index_1, &line_number, \
		 SIP_NULL, err) == SipFail)
	{
		if( *err == E_NO_EXIST ) 
		/* means that pHeader does not exist in the 
			message but is newly inserted */
		{
			/* if that pHeader does not exist append at the end of message */
			if ( sip_listSizeOf(&(msg->slOrderInfo), &line_number, err)\
				== SipFail)   
				return SipFail;

			if( sip_initSipHeaderOrderInfo( &header_info_row, err ) == SipFail)
				return SipFail;
	
			header_info_row->dType = dType;
			header_info_row->dTextType = SipFormFull;
			header_info_row->dNum = 1;

			if ( sip_listInsertAt( &(msg->slOrderInfo), line_number, \
				(SIP_Pvoid)(header_info_row), err ) == SipFail)
				return SipFail;

		}
		else /* Error if something other than E_NO_EXIST */
			return SipFail;

	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_setHeaderAt");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_deleteHeaderAtIndex
**
** DESCRIPTION:This function	deletes the pHeader at 
** 		index_1 "index_1" among the headers of en_HeaderType "dType" in 
** 		the SipMessage "msg". In case of headers in which "Any" dType is 
** 		possible, the dType passed must be "Any" - otherwise E_INV_TYPE 
** 		is returned.
** 
** 
** PARAMETERS:
**      	      msg(IN/OUT)			: The SipMessage to which header has
** 										  has to be deleted.
**				  dType(IN)				: The HeaderType 
**				  index_1(IN)				: The index_1 from which the header has to**										  be deleted 
**				  err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_deleteHeaderAtIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, SIP_U32bit index_1, SipError *err)
#else
	(msg, dType, index_1, err)
	SipMessage *msg;
	en_HeaderType dType;
	SIP_U32bit index_1;
	SipError *err;
#endif
{

	SIP_U32bit line_number;
	SipHeaderOrderInfo *header_info_row;

	SIPDEBUGFN("Entering function sip_deleteHeaderAt");

#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if ( msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (isSipGeneralHeader(dType) == SipTrue)
	{
		if (msg->pGeneralHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}	
		else
		{
			if (deleteGeneralHdrAtIndex(msg->pGeneralHdr, dType, index_1, \
				err) == SipFail)
				return SipFail;
		}
	}
	else
	{
		switch (msg->dType)
		{
			case	SipMessageRequest:
			 if (isSipReqHeader(dType) == SipTrue)
			 {
				if ((msg->u).pRequest == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				 }
				if (((msg->u).pRequest)->pRequestHdr == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}	
				else
				{
					if (deleteRequestHdrAtIndex(((msg->u).\
						pRequest)->pRequestHdr, dType, index_1, err)\
									== SipFail)
						return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;

			case	SipMessageResponse:
			 if (isSipRespHeader(dType) == SipTrue)
			 {
				if ((msg->u).pResponse == SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				if (((msg->u).pResponse)->pResponseHdr\
					== SIP_NULL)
				{
					*err = E_NO_EXIST;
					return SipFail;
				}
				else
				{
					if (deleteResponseHdrAtIndex(((msg->u).\
						pResponse)->pResponseHdr,  \
						dType, index_1, err) == SipFail)
						return SipFail;
				}
			 }
			 else
			 {
				*err = E_INV_TYPE;
				return SipFail;
			 }
			 break;
				default: 
						*err = E_INV_TYPE;
							 return SipFail;
		} /* of switch */
	}
	/* updating header_info_table */
	if ( sip_getHeaderPositionFromIndex(msg, dType, index_1, \
		&line_number, SIP_NULL, err) == SipFail)
		return SipFail;
	else
	{
		if ( sip_listGetAt( &(msg->slOrderInfo), line_number, \
			(SIP_Pvoid *)(&header_info_row), err) == SipFail )
			return SipFail;

		(header_info_row->dNum)--;
	
	 	/* all headers in that row is deleted */
		if ( header_info_row->dNum == 0 ) 
			if ( sip_listDeleteAt( &(msg->slOrderInfo), line_number, err) \
				== SipFail)
				return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_deleteHeaderAt");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_getMessageType
**
** DESCRIPTION:This function returns the dType ( request/respose ) 
** 		of SipMessage "msg" n the variable ""dType"
** 
** 
** PARAMETERS:
**      	      msg(IN/OUT)			: The SipMessage 
**				  dType(OUT)			: The SipMessage Type 
**				  err(OUT)				: The error Value
*******************************************************************************/

SipBool sip_getMessageType
#ifdef ANSI_PROTO
	(SipMessage *msg, en_SipMessageType *dType, SipError *err)
#else
	(msg, dType, err)
	SipMessage *msg;
	en_SipMessageType *dType;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function sip_getMessageType");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if ( (dType == SIP_NULL)||(msg == SIP_NULL))
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	switch (msg->dType)
	{
		case	SipMessageRequest	:
		case	SipMessageResponse	: *dType = msg->dType;
						  break;
		default 			: *err = E_INV_TYPE;
						  return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getMessageType");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: getGeneralHdrCount
**
** DESCRIPTION:This function returns the number of general headers 
** 		of en_HeaderType "dType" present in SipMessage "msg". The pValue 
** 		is returned in the variable "count". For SipHeaders in which the dType
** 		"Any is possible" the tupe passed must be "Any" - otherwise E_INV_TYPE 
** 		is returned.
** 
** 
** PARAMETERS:
**      	      hdr(IN/OUT)			: The SipGeneralHeader 
**				  dType(IN)				: The SipHeaderType Type 
**				  count(OUT)			: Number of general Headers
**				  err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool getGeneralHdrCount
#ifdef ANSI_PROTO
	(SipGeneralHeader *hdr, en_HeaderType dType, SIP_U32bit *count,  \
		SipError *err)
#else
	(hdr, dType, count, err)
	SipGeneralHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit *count;
	SipError *err;
#endif
{
	SIP_U32bit temp_count;
	SIPDEBUGFN("Entering function getGeneralHdrCount");
	switch (dType)
	{

		case	SipHdrTypeCallId:
			if (hdr->pCallidHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
			if (hdr->pMinSEHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case SipHdrTypeSessionExpires:
		
		if (hdr->pSessionExpiresHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
#endif
#ifdef SIP_PEMEDIA
		case	SipHdrTypePEarlyMedia:
			if (sip_listSizeOf(&(hdr->slPEarlyMediaHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
#endif
		case	SipHdrTypeCseq:
			if (hdr->pCseqHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

		case	SipHdrTypeFrom:
			if (hdr->pFromHeader != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

#ifdef SIP_DATE
		case	SipHdrTypeExpiresSec:
		case	SipHdrTypeExpiresAny:
		case	SipHdrTypeExpiresDate:
#else
		case	SipHdrTypeExpires:
#endif
			if (hdr->pExpiresHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

		case	SipHdrTypeMimeVersion:
			if (hdr->pMimeVersionHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

#ifdef SIP_DATE
		case	SipHdrTypeRetryAfterAny:
		case	SipHdrTypeRetryAfterSec:
		case	SipHdrTypeRetryAfterDate:
#else
		case	SipHdrTypeRetryAfter:
#endif
			if (hdr->pRetryAfterHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

		case	SipHdrTypeRecordRoute:
			if (sip_listSizeOf(&(hdr-> slRecordRouteHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
#ifdef SIP_3GPP
		case	SipHdrTypePath:
			if (sip_listSizeOf(&(hdr-> slPathHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
		case	SipHdrTypePcfAddr:
			if (hdr->pPcfAddrHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case	SipHdrTypePanInfo:
			if (hdr->pPanInfoHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case	SipHdrTypePcVector:
			if (hdr->pPcVectorHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case	SipHdrTypeHistoryInfo:
			if (sip_listSizeOf(&(hdr-> slHistoryInfoHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
#endif
		case	SipHdrTypeAllow:
			if (sip_listSizeOf(&(hdr-> slAllowHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
		case	SipHdrTypeContentDisposition:
			if (sip_listSizeOf(&(hdr-> slContentDispositionHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
		case	SipHdrTypeSupported:
			if (sip_listSizeOf(&(hdr-> slSupportedHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break; 

#ifdef SIP_TIMESTAMP			
		case	SipHdrTypeTimestamp:
			if (hdr->pTimeStampHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
#endif
		case	SipHdrTypeTo:
			if (hdr->pToHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

		case	SipHdrTypeVia:
			if (sip_listSizeOf(&(hdr->slViaHdr), &temp_count, err) == SipFail)
				return SipFail;
			*count = temp_count;
			break;

		case	SipHdrTypeContentEncoding:
			if (sip_listSizeOf(&(hdr->slContentEncodingHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
		case	SipHdrTypeContentLength:
			if (hdr->pContentLengthHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;

		case	SipHdrTypeContentType:
			if (hdr->pContentTypeHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case	SipHdrTypeUnknown:
			if (sip_listSizeOf(&(hdr->slUnknownHdr), &temp_count, err) \
				== SipFail)
			return SipFail;
			*count = temp_count;
			break;
						
		case SipHdrTypeContactWildCard:
		case SipHdrTypeContactNormal:
		case	SipHdrTypeContactAny:
			if (sip_listSizeOf(&(hdr->slContactHdr), &temp_count, err) \
				== SipFail)
				return SipFail;
			*count = temp_count;
			break;
		case	SipHdrTypeRequire:
			if (sip_listSizeOf(&(hdr->slRequireHdr), &temp_count, err) \
				== SipFail)
				return SipFail;
				*count = temp_count;
			break;
#ifdef SIP_IMPP
			case	SipHdrTypeAllowEvents:
			if (sip_listSizeOf(&(hdr->slAllowEventsHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			*count = temp_count;
			break;
#endif
		case	SipHdrTypeAccept:
			if (sip_listSizeOf(&(hdr->slAcceptHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			*count = temp_count;
			break;
#ifdef SIP_PRIVACY
		case	SipHdrTypePrivacy:
			if (hdr->pPrivacyHdr != SIP_NULL)
				*count = 1;
			else
				*count = 0;
			break;
		case	SipHdrTypePPreferredId:
            	if (sip_listSizeOf(&(hdr->slPPreferredIdHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			*count = temp_count;
			break;
	  case	 SipHdrTypePAssertId:
			if (sip_listSizeOf(&(hdr->slPAssertIdHdr), &temp_count, err)\
				== SipFail)
				return SipFail;
			*count = temp_count;
			break;		
#endif
		default :
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getGeneralHdrCount");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_getHeaderPositionFromIndex
**
** DESCRIPTION: This function retrieves the absolute line and the 
**		 position in that line of an en_HeaderType "dType" given the linear 
**		 index_1 among all the headers of that dType.
** 
** 
** PARAMETERS:
**      	         msg(IN)			: The SipMessage 
**			       dType(IN)			: The SipHeaderType Type 
**			  list_index(IN)			: 
**		       abs_line(OUT)			: The absolute line
**			   position(OUT)			: The position in the absolute line
**			        err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getHeaderPositionFromIndex
#ifdef ANSI_PROTO
	(SipMessage *msg, en_HeaderType dType, SIP_U32bit list_index, \
		SIP_U32bit *abs_line, SIP_U32bit *position, SipError *err)
#else
	(msg, dType, list_index, abs_line, position, err)
	SipMessage *msg;
	en_HeaderType dType;
	SIP_U32bit list_index;
	SIP_U32bit *abs_line;
	SIP_U32bit *position;
	SipError *err;
#endif
{
	SIP_Pvoid header_line;
	SipHeaderOrderInfo *hdr_info;
	SIP_U32bit temp_count, count, prev_count, iter;
	en_SipBoolean result = SipFalse;;
	SIPDEBUGFN("Entering sip_getHeaderPositionFromIndex");
	count=prev_count=0;

	/* checking for the validity of input parameters. */
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;
	if ( (msg==SIP_NULL)||((abs_line==SIP_NULL)&&(position==SIP_NULL)) )
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	if ( sip_verifyTypeAny(dType, err) == SipFail)
			return SipFail;

	if (sip_listSizeOf(&(msg->slOrderInfo), &temp_count, err) == SipFail)
		return SipFail;
	for (iter=0; iter<temp_count; iter++)
	{
		if (sip_listGetAt(&(msg->slOrderInfo), iter, &header_line, err)\
			== SipFail)
			return SipFail;
		hdr_info = (SipHeaderOrderInfo *)header_line;
		if (hdr_info->dType == dType)
			count+=hdr_info->dNum;
		if (count>list_index)
		{
			if (abs_line != SIP_NULL)
				*abs_line = iter;
			if (position != SIP_NULL)
				*position = list_index-prev_count;
			result = SipTrue;
			break;
		}
		prev_count=count;
	}
	if (result == SipFalse)
	{
		*err = E_NO_EXIST;
		return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting sip_getHeaderPositionFromIndex");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_getHeaderIndexFromPosition
**
** DESCRIPTION: This function calculates the linear index_1 of 
** 			a pHeader of en_HeaderType "dType" given its absolute line and 
** 			position within that line
** 
** 
** PARAMETERS:
**      	         msg(IN)			: The SipMessage 
**			       dType(IN)			: The SipHeaderType Type 
**			  	  index_1(OUT)			: The index_1 of Header
**		        abs_line(IN)			: The absolute line
**			    position(IN)			: The position of header
**			        err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getHeaderIndexFromPosition
#ifdef ANSI_PROTO
	(SipMessage *msg, SIP_U32bit abs_line, SIP_U32bit position, \
		SIP_U32bit *index_1, SipError *err)
#else
	(msg, abs_line, position, index_1, err)
	SipMessage *msg;
	SIP_U32bit abs_line;
	SIP_U32bit position;
	SIP_U32bit *index_1;
	SipError *err;
#endif
{
	SIP_Pvoid header_line;
	SipHeaderOrderInfo *hdr_info, *hdr_info1;
	en_HeaderType dType;
	SIP_U32bit count, iter;
	count=0;
	SIPDEBUGFN("Entering sip_getHeaderIndexFromPosition");
#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;
	if ((msg==SIP_NULL)||(index_1==SIP_NULL))
	{
		*err=E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listGetAt(&(msg->slOrderInfo), abs_line, &header_line, err) \
		== SipFail)
		return SipFail;
	hdr_info = (SipHeaderOrderInfo *)header_line;
	if (((hdr_info->dNum)-1) < position)
	{
		*err=E_INV_PARAM;
		return SipFail;
	}
	dType = hdr_info->dType;
	for (iter=0; iter<abs_line; iter++)
	{
		if (sip_listGetAt (&(msg->slOrderInfo), iter, &header_line, err) \
			== SipFail)
			return SipFail;
		hdr_info1 = (SipHeaderOrderInfo *)header_line;
		if (hdr_info1->dType == dType)
			count += hdr_info1->dNum;
	}
	*index_1 = count+position;

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting sip_getHeaderIndexFromPosition");
	return SipSuccess;
}



/*******************************************************************************
** FUNCTION: sip_getHeaderTypeAtHeaderLine
**
** DESCRIPTION:  This function retrieves the dType of the headers 
** 			in the line "line" from a SipMessage "msg"
** 
** 
** PARAMETERS:
**      	         msg(IN)			: The SipMessage 
**			       dType(OUT)			: The SipHeaderType  
**		        	line(IN)			: The absolute line
**			        err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_getHeaderTypeAtHeaderLine
#ifdef ANSI_PROTO
	( SipMessage		*msg, 
	  SIP_U32bit		line, 
	  en_HeaderType 	*dType, 
	  SipError		*err )
#else
	( msg, line, dType, err )
	  SipMessage		*msg;
	  SIP_U32bit		line;
	  en_HeaderType 	*dType;
	  SipError		*err;
#endif
{
	SIP_Pvoid	temp;

	SIPDEBUGFN("Entering function sip_getHeaderTypeAtHeaderLine");

#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if (msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (sip_listGetAt (&(msg->slOrderInfo), line, &temp, err) == SipFail)
		return SipFail;

	*dType = ((SipHeaderOrderInfo *)temp)->dType;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getHeaderTypeAtHeaderLine");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: sip_getHeaderCountFromHeaderLine
**
** DESCRIPTION:  This function retrieves the number of headers present 
** 			at the "line" line in a SipMessage "msg". 
** 
** PARAMETERS:
**      	         msg(IN)			: The SipMessage 
**			       dType(OUT)			: The SipHeaderType  
**		        	line(IN)			: The absolute line
**		        	count(OUT)			: The Count number of headers present 
**											at that line line
**			        err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getHeaderCountFromHeaderLine
#ifdef ANSI_PROTO
	( SipMessage		*msg, 
	  SIP_U32bit		line, 
	  SIP_U32bit		*count, 
	  SipError		*err )
#else
	( msg, line, count, err )
	  SipMessage		*msg;
	  SIP_U32bit		line;
	  SIP_U32bit		*count;
	  SipError		*err;
#endif
{
	SIP_Pvoid	temp;

	SIPDEBUGFN("Entering function sip_getHeaderCountFromHeaderLine");

#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if (msg == SIP_NULL)
    {
       *err = E_INV_PARAM;
		return SipFail;
    }
#endif

	if (sip_listGetAt (&(msg->slOrderInfo), line, &temp, err) == SipFail)
		return SipFail;

	*count = ((SipHeaderOrderInfo *)temp)->dNum;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getHeaderCountFromHeaderLine");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: sip_getHeaderFormFromHeaderLine
**
** DESCRIPTION: This function retrieves the form ( SHORT / FULL ) 
** of the headers in the line "line" in a SipMessage "msg".
** PARAMETERS:
**		msg(IN)			: The SipMessage 
**		line(IN)		: The absolute line
**		dTextType(OUT)	: The whether Full form /Short Form 
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_getHeaderFormFromHeaderLine
#ifdef ANSI_PROTO
	( SipMessage		*msg, 
	  SIP_U32bit		line, 
	  en_HeaderForm 	*dTextType, 
	  SipError		*err )
#else
	( msg, line, dTextType, err )
	  SipMessage		*msg;
	  SIP_U32bit		line;
	  en_HeaderForm 	*dTextType;
	  SipError		*err;
#endif
{
	SIP_Pvoid	temp;

	SIPDEBUGFN("Entering function sip_getHeaderFormFromHeaderLine");

#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if (msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (sip_listGetAt (&(msg->slOrderInfo), line, &temp, err) == SipFail)
		return SipFail;

	*dTextType = ((SipHeaderOrderInfo *)temp)->dTextType;
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function sip_getHeaderFormFromHeaderLine");
	return SipSuccess;
}




/*******************************************************************************
** FUNCTION: sip_setHeaderFormFromHeaderLine
**
** DESCRIPTION: This function sets the form ( SHORT / FULL ) 
** 		of the headers in the line "line" in a SipMessage "msg".
** PARAMETERS:
**		msg(IN)			: The SipMessage 
**		line(IN)		: The absolute line
**		dTextType(IN)	: The whether Full form /Short Form 
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_setHeaderFormAtHeaderLine
#ifdef ANSI_PROTO
	( SipMessage		*msg, 
	  SIP_U32bit		line, 
	  en_HeaderForm 	dTextType, 
	  SipError		*err )
#else
	( msg, line, dTextType, err )
	  SipMessage		*msg;
	  SIP_U32bit		line;
	  en_HeaderForm 	dTextType;
	  SipError		*err;
#endif
{
	SIP_Pvoid	temp;

	SIPDEBUGFN("Entering function sip_setHeaderFormAtHeaderLine");

#ifndef SIP_NO_CHECK
	/* Validating parameters */
	if (err == SIP_NULL)
		return SipFail;

	if (msg == SIP_NULL)
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
#endif

	if (dTextType == SipFormFull || dTextType == SipFormShort)
	{
		if (sip_listGetAt (&(msg->slOrderInfo), line, &temp, err) == SipFail)
			 return SipFail;

		((SipHeaderOrderInfo *)temp)->dTextType = dTextType;
		*err = E_NO_ERROR;
		SIPDEBUGFN("Exitting function sip_setHeaderFormAtHeaderLine");
		return SipSuccess;
	}
	else
	{
		*err = E_INV_PARAM;
		return SipFail;
	}
}

/*******************************************************************************
** FUNCTION: getRequestHdrCount
**
** DESCRIPTION:This function returns the number of request headers
**	of a particular en_HeaderType "dType".
** PARAMETERS:
**		hdr(IN)			: The SipReqHeader 
**		dType(IN)		: The SipHeaderType
**		Count(OUT)		: The Request Header Count retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool getRequestHdrCount
#ifdef ANSI_PROTO
	(SipReqHeader *hdr, en_HeaderType dType, SIP_U32bit *count, SipError *err)
#else
	(hdr, dType, count, err)
	SipReqHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit *count;
	SipError *err;
#endif
{
	SIP_U32bit temp_count;
	SIPDEBUGFN("Entering function getRequestHdrCount");
	switch (dType)
	{
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeAuthorization:
			if (sip_listSizeOf(&(hdr->slAuthorizationHdr), &temp_count, err)\
				 == SipFail)
				return SipFail;
			*count = temp_count;
			break;
#endif			
	    case SipHdrTypeRoute:
		  if (sip_listSizeOf(&(hdr->slRouteHdr), &temp_count, err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeProxyauthorization:
		  if (sip_listSizeOf(&(hdr->slProxyAuthorizationHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
#endif		 
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		  if (hdr->pRackHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#endif		  
#ifdef SIP_IMPP		  
		case SipHdrTypeSubscriptionState:
		   if (hdr->pSubscriptionStateHdr != SIP_NULL)
			  *count = 1;
	       else
		 	  *count = 0;
			break;
  
	    case SipHdrTypeEvent:
		  if (hdr->pEventHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#endif		  
 
	    case SipHdrTypeMaxforwards:
		  if (hdr->pMaxForwardsHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#ifdef SIP_REPLACES
	    case SipHdrTypeReplaces:
		  if (hdr->pReplacesHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#endif		  
#ifdef SIP_REFER
	    case SipHdrTypeReferTo:
		  if (hdr->pReferToHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
	    case SipHdrTypeReferredBy:
		  if (hdr->pReferredByHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		  if (hdr->pPCalledPartyIdHdr != SIP_NULL)
		    	*count = 1;
		  else
		      	*count = 0;
		  break;
		
		case SipHdrTypeJoin:
		  if (hdr->pJoinHdr != SIP_NULL)
		    	*count = 1;
		  else
		      	*count = 0;
		  break;

		case SipHdrTypeIfMatch:
		  if (hdr->pIfMatchHdr != SIP_NULL)
		    	*count = 1;
		  else
		      	*count = 0;
		  break;

		case SipHdrTypePVisitedNetworkId:
		  if (sip_listSizeOf(&(hdr->slPVisitedNetworkIdHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
		case SipHdrTypeAcceptContact:
		  if (sip_listSizeOf(&(hdr->slAcceptContactHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
		case SipHdrTypeRejectContact:
		  if (sip_listSizeOf(&(hdr->slRejectContactHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
		case SipHdrTypeRequestDisposition:
		  if (sip_listSizeOf(&(hdr->slRequestDispositionHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;

#endif

#ifdef SIP_SECURITY
     	case SipHdrTypeSecurityClient:
		  if (sip_listSizeOf(&(hdr->slSecurityClientHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
     	case SipHdrTypeSecurityVerify:
		  if (sip_listSizeOf(&(hdr->slSecurityVerifyHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
#endif          
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
		  if (sip_listSizeOf(&(hdr->slRsrcPriorityHdr), &temp_count, \
			err)== SipFail)
		      return SipFail;
		  *count = temp_count;
		  break;
#endif	
	    default:
		  *err = E_INV_TYPE;
		   return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getRequestHdrCount");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: getResponseHdrCount
**
** DESCRIPTION:This function returns the number of respose headers
**	of a particular en_HeaderType "dType".
** PARAMETERS:
**		hdr(IN)			: The SipRespHeader 
**		dType(IN)		: The SipHeaderType
**		Count(OUT)		: The Response Header Count retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool getResponseHdrCount
#ifdef ANSI_PROTO
	(SipRespHeader *hdr, en_HeaderType dType, SIP_U32bit *count, SipError *err)
#else
	(hdr, dType, count, err)
	SipRespHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit *count;
	SipError *err;
#endif
{
	SIP_U32bit temp_count;
	SIPDEBUGFN("Entering function getResponseHdrCount");
	switch (dType)
	{
#ifdef SIP_RPR	
		case SipHdrTypeRSeq:
		  if (hdr->pRSeqHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
#endif
		case SipHdrTypeUnsupported:
			if (sip_listSizeOf(&(hdr->slUnsupportedHdr), &temp_count, err)\
				 == SipFail)
				return SipFail;
			*count = temp_count;
			break;

#ifdef SIP_WARNING
		case SipHdrTypeWarning:
			if (sip_listSizeOf(&(hdr->slWarningHeader), &temp_count, err)\
				 == SipFail)
				return SipFail;
			*count = temp_count;
			break;
#endif

#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyAuthenticate:
			if (sip_listSizeOf(&(hdr->slProxyAuthenticateHdr), &temp_count, err)\
				 == SipFail)
				return SipFail;
			*count = temp_count;
			break;
		case SipHdrTypeWwwAuthenticate:
			if (sip_listSizeOf(&(hdr->slWwwAuthenticateHdr), &temp_count, err)\
				 == SipFail)
				return SipFail;
			*count = temp_count;
			break;
#endif
#ifdef SIP_3GPP
		
		case SipHdrTypePAssociatedUri:
			if (sip_listSizeOf(&(hdr-> slPAssociatedUriHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;

		case SipHdrTypeETag:
		  if (hdr->pETagHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
       
		case SipHdrTypeServiceRoute:
			if (sip_listSizeOf(&(hdr-> slServiceRouteHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
 
#endif
		case SipHdrTypeMinExpires:
		  if (hdr->pMinExpiresHdr != SIP_NULL)
		      *count = 1;
		  else
		      *count = 0;
		  break;
          
#ifdef SIP_SECURITY 
		case SipHdrTypeSecurityServer:
		    if (sip_listSizeOf(&(hdr-> slSecurityServerHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
#endif
 #ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
		    if (sip_listSizeOf(&(hdr->slAcceptRsrcPriorityHdr), &temp_count, \
				err) == SipFail)
				return SipFail;
			 *count = temp_count;
			 break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getResponseHdrCount");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: isSipGeneralHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a
**	 general header or not.If dType isa general header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/
en_SipBoolean isSipGeneralHeader
#ifdef ANSI_PROTO
	(en_HeaderType dType)
#else
	(dType)
	en_HeaderType dType;
#endif
{
	SIPDEBUGFN("Entering function isSipGeneralHeader");
	switch(dType)
	{
		case SipHdrTypeCallId:
		case SipHdrTypeAllow:
		case SipHdrTypeCseq:
		case SipHdrTypeFrom:
		case SipHdrTypeRecordRoute:
#ifdef SIP_3GPP		
		case SipHdrTypePath:
		case SipHdrTypePcfAddr:
 		case SipHdrTypePanInfo:
   	case SipHdrTypePcVector:
	  case SipHdrTypeHistoryInfo:

#endif		
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
#endif		
		case SipHdrTypeTo:
		case SipHdrTypeVia:
		case SipHdrTypeContentEncoding:
		case SipHdrTypeContentLength:
		case SipHdrTypeContentType:
		case SipHdrTypeContactWildCard:
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactAny:
		case SipHdrTypeRequire:
#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresDate:
#else
		case SipHdrTypeExpires:
#endif
		case SipHdrTypeMimeVersion:
		case SipHdrTypeSupported:
		case SipHdrTypeContentDisposition:
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterAny:
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterSec:
#else
		case SipHdrTypeRetryAfter:
#endif
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
#endif
#ifdef SIP_SESSIONTIMER
		case SipHdrTypeSessionExpires:
		case SipHdrTypeMinSE:
#endif
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		case SipHdrTypePPreferredId:
		case SipHdrTypePAssertId:
#endif
#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
#endif
		case SipHdrTypeAccept:
		case SipHdrTypeUnknown:
			break;
		default:
			return SipFalse;
	}
	SIPDEBUGFN("Exitting function isSipGeneralHeader");
	return SipTrue;
}

/*******************************************************************************
** FUNCTION: isSipReqHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a**	 request header or not.If dType isa request header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/

en_SipBoolean isSipReqHeader
#ifdef ANSI_PROTO
	(en_HeaderType dType)
#else
	(dType)
	en_HeaderType dType;
#endif
{
	SIPDEBUGFN("Entering function isSipReqHeader");
	switch (dType)
	{
		case SipHdrTypeRoute:
#ifdef SIP_RPR		
		case SipHdrTypeRAck:
#endif		
#ifdef SIP_AUTHENTICATE		
		case SipHdrTypeProxyauthorization:
		case SipHdrTypeAuthorization:
#endif		
		case SipHdrTypeMaxforwards:
#ifdef SIP_REPLACES
		case SipHdrTypeReplaces:
#endif
#ifdef SIP_IMPP		
		case SipHdrTypeEvent:
		case SipHdrTypeSubscriptionState:
#endif		
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
		case SipHdrTypeReferredBy:
#endif
		     
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		case SipHdrTypePVisitedNetworkId:
		case SipHdrTypeAcceptContact:
		case SipHdrTypeRejectContact:
		case SipHdrTypeRequestDisposition:
		case SipHdrTypeJoin:
		case SipHdrTypeIfMatch:
#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityClient:
        case SipHdrTypeSecurityVerify:
#endif            
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
#endif
		 break;
		default:
		     return SipFalse;
	}
	SIPDEBUGFN("Exitting function isSipReqHeader");
	return SipTrue;
}
/*******************************************************************************
** FUNCTION: isSipRespHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a**	respose header or not.If dType is a respose header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/

en_SipBoolean isSipRespHeader
#ifdef ANSI_PROTO
	(en_HeaderType dType)
#else
	(dType)
	en_HeaderType dType;
#endif
{
	SIPDEBUGFN("Entering function isSipRespHeader");
	switch(dType)
	{
		case SipHdrTypeUnsupported:
#ifdef SIP_WARNING		
		case SipHdrTypeWarning:
#endif		
#ifdef SIP_RPR		
		case SipHdrTypeRSeq:
#endif		
#ifdef SIP_AUTHENTICATE		
		case SipHdrTypeProxyAuthenticate:
		case SipHdrTypeWwwAuthenticate:
#endif		
			
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
		case SipHdrTypeServiceRoute:
		case SipHdrTypeETag:
#endif		
		case SipHdrTypeMinExpires:
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityServer:
#endif            
		break;
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
#endif
		default:
			return SipFalse;
	}
	SIPDEBUGFN("Exitting function isSipRespHeader");
	return SipTrue;
}

/*******************************************************************************
** FUNCTION: getGeneralHeaderAtIndex
**
**  DESCRIPTION: This function returns the general pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pGeneralHdr(IN)	: The SipGeneralHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index_1(IN)		: The index_1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool getGeneralHeaderAtIndex
#ifdef ANSI_PROTO
	(SipGeneralHeader *pGeneralHdr, en_HeaderType dType, SipHeader \
	*hdr, SIP_U32bit index_1, SipError *err)
#else
	(pGeneralHdr, dType, hdr, index_1, err)
	SipGeneralHeader *pGeneralHdr;
	en_HeaderType dType;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{

	SIP_Pvoid pTempHdr;
    SipCommonHeader *pHeader = SIP_NULL;

	SIPDEBUGFN("Entering function getGeneralHeaderAtIndex");

	switch (dType)
	{
#ifdef SIP_SESSIONTIMER
	case SipHdrTypeMinSE:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pMinSEHdr;
    }
		break;
	
	case SipHdrTypeSessionExpires:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pSessionExpiresHdr;
    }
		break;
	
#endif
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
		if (sip_listGetAt(&(pGeneralHdr->slPEarlyMediaHdr), index_1, \
			 &pTempHdr, err) == SipFail)
			return SipFail;
        pHeader = (SipCommonHeader *)(pTempHdr);
        break;
#endif
		case SipHdrTypeCallId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pCallidHdr;
    }
		 break;

#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresDate:
#else
		case SipHdrTypeExpires:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
        {
            pHeader = pGeneralHdr->pExpiresHdr;
        }
		break;
	/*
	 * Added Retry After for UA 
	 * support
	 */	

#ifdef SIP_DATE
		case	SipHdrTypeRetryAfterAny:
		case	SipHdrTypeRetryAfterSec:
		case	SipHdrTypeRetryAfterDate:
#else
		case	SipHdrTypeRetryAfter:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pRetryAfterHdr;
        }
		break;
		case SipHdrTypeCseq:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pCseqHdr;
		}
		break;	
		case SipHdrTypeFrom:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pFromHeader;
		}
		break;
		case SipHdrTypeRecordRoute:
        if (sip_listGetAt(&(pGeneralHdr->slRecordRouteHdr), index_1, \
                    &pTempHdr, err) == SipFail)
            return SipFail;
        pHeader = (SipCommonHeader *)(pTempHdr);
        break;
#ifdef SIP_3GPP
	case SipHdrTypePath:
		if (sip_listGetAt(&(pGeneralHdr->slPathHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
        pHeader = (SipCommonHeader *)(pTempHdr);
        break;
	case SipHdrTypePanInfo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			pHeader = pGeneralHdr->pPanInfoHdr; 
		}	
	break;
	case SipHdrTypePcfAddr:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			pHeader = pGeneralHdr->pPcfAddrHdr; 
	  }
		break;

		case SipHdrTypePcVector:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			pHeader = pGeneralHdr->pPcVectorHdr;
		}
		break;
		case SipHdrTypeHistoryInfo:
		if (sip_listGetAt(&(pGeneralHdr->slHistoryInfoHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
			  pHeader = (SipCommonHeader *)pTempHdr;
		    break;
#endif
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pTimeStampHdr;
        }
		break;
#endif		
		case SipHdrTypeTo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pToHdr;
		}
		break;

		case SipHdrTypeVia:
		if (sip_listGetAt(&(pGeneralHdr->slViaHdr), index_1, &pTempHdr, err)\
			 == SipFail)
			return SipFail;
        pHeader = (SipCommonHeader *)(pTempHdr);
        break;

		case SipHdrTypeContentEncoding:
		if (sip_listGetAt(&(pGeneralHdr->slContentEncodingHdr), index_1, \
			 &pTempHdr, err) == SipFail)
			return SipFail;
        pHeader = (SipCommonHeader *)(pTempHdr);
        break;

		case SipHdrTypeContentLength:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pContentLengthHdr;
        }
        break;
		 
		case SipHdrTypeContentType:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pContentTypeHdr;
		}
		break;

		case SipHdrTypeMimeVersion:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pGeneralHdr->pMimeVersionHdr;
		}
		break;
			
		case SipHdrTypeContactAny:
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactWildCard:
		if (sip_listGetAt(&(pGeneralHdr->slContactHdr), index_1, &pTempHdr, \
			err) == SipFail)
			return SipFail;
        pHeader = (SipCommonHeader *)pTempHdr;
		break;

		case SipHdrTypeRequire:
		if (sip_listGetAt(&(pGeneralHdr->slRequireHdr), index_1, &pTempHdr, \
			err) == SipFail)
			return SipFail;
        pHeader = (SipCommonHeader *)pTempHdr;
        break;

#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
		if (sip_listGetAt(&(pGeneralHdr->slAllowEventsHdr), index_1, &pTempHdr, \
			err) == SipFail)
			return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
		break;
#endif

#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
      pHeader = pGeneralHdr->pPrivacyHdr;
		}
		break;

		case SipHdrTypePPreferredId:
		if (sip_listGetAt(&(pGeneralHdr->slPPreferredIdHdr), index_1, &pTempHdr, \
			err) == SipFail)
			return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
		break;

		case SipHdrTypePAssertId:
		if (sip_listGetAt(&(pGeneralHdr->slPAssertIdHdr), index_1, &pTempHdr, \
			err) == SipFail)
			return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;
#endif
				 
		case SipHdrTypeUnknown:
		if (sip_listGetAt(&(pGeneralHdr->slUnknownHdr), index_1, &pTempHdr, err)\
			 == SipFail)
			return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;

		case SipHdrTypeSupported:
         if (sip_listGetAt(&(pGeneralHdr->slSupportedHdr), index_1, \
                     &pTempHdr, err) == SipFail)
             return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;
		case SipHdrTypeAllow:
         if (sip_listGetAt(&(pGeneralHdr->slAllowHdr), index_1, \
                     &pTempHdr, err) == SipFail)
             return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;

		case SipHdrTypeContentDisposition:
         if (sip_listGetAt(&(pGeneralHdr->slContentDispositionHdr), index_1, \
                     &pTempHdr, err) == SipFail)
             return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;
		case SipHdrTypeAccept:
         if (sip_listGetAt(&(pGeneralHdr->slAcceptHdr), index_1, \
                     &pTempHdr, err) == SipFail)
             return SipFail;
         pHeader = (SipCommonHeader *)pTempHdr;
         break;

		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

    if (pHeader == SIP_NULL)
    {
        *err = E_NO_EXIST;
        return SipFail;
    }
    (hdr)->pHeader = (SIP_Pvoid)pHeader;
#ifdef SIP_INCREMENTAL_PARSING
    if( pHeader->pBuffer!=SIP_NULL )
    {
        /*
         * Comming here means here header is not parsed
         * and  we need to call the parser to parse the header.
         * So we call parseHeaderBody function
         */

        SIP_U32bit length =0;
        length = sip_strlen(pHeader->pBuffer);
        if(SipFail != sip_parseHeaderBody\
                (pHeader->pBuffer, \
                 pHeader->pBuffer+(length-1), \
                 dType, hdr, err))
        {

            (void)fast_memfree(ACCESSOR_MEM_ID, \
                    pHeader->pBuffer, err);
            pHeader->pBuffer= SIP_NULL;
        }		
        else 
        {
            *err = E_PARSER_ERROR;
            return SipFail;	
        }	
    }
#endif
    HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
            dRefCount);

   
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getGeneralHeaderAtIndex");
	return SipSuccess;
}


/*******************************************************************************
** FUNCTION: getRequestHeaderAtIndex
**
**  DESCRIPTION: This function returns the Request pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pRequestHdr(IN)	: The SipRequestHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index_1(IN)		: The index_1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool getRequestHeaderAtIndex
#ifdef ANSI_PROTO
	(SipReqHeader *pRequestHdr, en_HeaderType dType, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(pRequestHdr, dType, hdr, index_1, err)
	SipReqHeader *pRequestHdr;
	en_HeaderType dType;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIP_Pvoid pTempHdr = SIP_NULL;
    SipCommonHeader *pHeader = SIP_NULL;
	SIPDEBUGFN("Entering function getRequestHeaderAtIndex");
	switch(dType)
	{
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
		if (sip_listGetAt(&(pRequestHdr->slAuthorizationHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
        pHeader = (SipCommonHeader *)pTempHdr;
		break;
#endif		
		case SipHdrTypeRoute:
		if (sip_listGetAt(&(pRequestHdr->slRouteHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
        pHeader = (SipCommonHeader *)pTempHdr;
		break;

#ifdef SIP_AUTHENTICATE		
		case SipHdrTypeProxyauthorization:
		if (sip_listGetAt(&(pRequestHdr->slProxyAuthorizationHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
        pHeader = (SipCommonHeader *)pTempHdr;
		break;
#endif		
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
        if (index_1 > 0)
        {
            *err = E_INV_INDEX;
            return SipFail;
        }
        else
        {
            pHeader = pRequestHdr->pReferToHdr;
        }
        break;

		case SipHdrTypeReferredBy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pRequestHdr->pReferredByHdr;
    }
        break;
#endif
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pRequestHdr->pRackHdr;
		}
        break;
#endif				 
#ifdef SIP_IMPP
		case SipHdrTypeSubscriptionState:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pRequestHdr->pSubscriptionStateHdr;
		}
        break;

		case SipHdrTypeEvent:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pRequestHdr->pEventHdr;
		}
        break;
#endif
	case SipHdrTypeMaxforwards:
        if (index_1 > 0)
        {
            *err = E_INV_INDEX;
            return SipFail;
        }
        else
        {
            pHeader = pRequestHdr->pMaxForwardsHdr;
        }
        break;
#ifdef SIP_REPLACES		 
	case SipHdrTypeReplaces:
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			else
			{
                pHeader = pRequestHdr->pReplacesHdr;
			}
            break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else  
			{
						pHeader = pRequestHdr->pPCalledPartyIdHdr;	
			}
								
		 break;
		case SipHdrTypeJoin:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else 
			{
					pHeader = pRequestHdr->pJoinHdr;	
			}
			 break;

		case SipHdrTypeIfMatch:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
						pHeader = pRequestHdr->pIfMatchHdr;
		}
			 break;
		case SipHdrTypePVisitedNetworkId:
		if (sip_listGetAt(&(pRequestHdr->slPVisitedNetworkIdHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
		case SipHdrTypeRejectContact:
		if (sip_listGetAt(&(pRequestHdr->slRejectContactHdr), index_1, \
		  	&pTempHdr, err) == SipFail)
			  return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
		case SipHdrTypeAcceptContact:
		if (sip_listGetAt(&(pRequestHdr->slAcceptContactHdr), index_1, \
	 			&pTempHdr, err) == SipFail)
				return SipFail;
			  pHeader = (SipCommonHeader *)pTempHdr;
				break;
		case SipHdrTypeRequestDisposition:
		if (sip_listGetAt(&(pRequestHdr->slRequestDispositionHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
#endif				
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityClient:
		if (sip_listGetAt(&(pRequestHdr->slSecurityClientHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
        case SipHdrTypeSecurityVerify:
		if (sip_listGetAt(&(pRequestHdr->slSecurityVerifyHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
#endif                
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
		if (sip_listGetAt(&(pRequestHdr->slRsrcPriorityHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
#endif	
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

    if (pHeader == SIP_NULL)
    {
        *err = E_NO_EXIST;
        return SipFail;
    }
    (hdr)->pHeader = (SIP_Pvoid)pHeader;
#ifdef SIP_INCREMENTAL_PARSING
    if( pHeader->pBuffer!=SIP_NULL )
    {
        /*
         * Comming here means here header is not parsed
         * and  we need to call the parser to parse the header.
         * So we call parseHeaderBody function
         */

        SIP_U32bit length =0;
        length = sip_strlen(pHeader->pBuffer);
        if(SipFail != sip_parseHeaderBody\
                (pHeader->pBuffer, \
                 pHeader->pBuffer+(length-1), \
                 dType, hdr, err))
        {

            (void)fast_memfree(ACCESSOR_MEM_ID, \
                    pHeader->pBuffer, err);
            pHeader->pBuffer= SIP_NULL;
        }		
        else 
        {
            *err = E_PARSER_ERROR;
            return SipFail;	
        }	
    }
#endif
    HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
            dRefCount);

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getRequestHeaderAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: getResponseHeaderAtIndex
**
**  DESCRIPTION: This function returns the Response pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pResponseHdr(IN): The SipResponseHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index_1(IN)		: The index_1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool getResponseHeaderAtIndex
#ifdef ANSI_PROTO
	(SipRespHeader *pResponseHdr, en_HeaderType dType, SipHeader *hdr, \
	 SIP_U32bit index_1, SipError *err)
#else
	(pResponseHdr, dType, hdr, index_1, err)
	SipRespHeader *pResponseHdr;
	en_HeaderType dType;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIP_Pvoid pTempHdr = SIP_NULL;
    SipCommonHeader *pHeader = SIP_NULL;
	SIPDEBUGFN("Entering function getResponseHeaderAtIndex");
	switch(dType)
	{
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeWwwAuthenticate:
			{
				if (sip_listGetAt(&(pResponseHdr->slWwwAuthenticateHdr),index_1, \
							&pTempHdr, err) == SipFail)
						return SipFail;
                pHeader = (SipCommonHeader *)pTempHdr;
			}	
			break;
#endif		
#ifdef SIP_WARNING
		case SipHdrTypeWarning:
			{
				if (sip_listGetAt(&(pResponseHdr->slWarningHeader), index_1, \
						&pTempHdr, err) == SipFail)
						return SipFail;
                pHeader = (SipCommonHeader *)pTempHdr;

			}	
		break;
#endif		
		case SipHdrTypeUnsupported:
		{
            if (sip_listGetAt(&(pResponseHdr->slUnsupportedHdr), index_1, \
                        &pTempHdr, err) == SipFail)
                return SipFail;
            pHeader = (SipCommonHeader *)pTempHdr;
		}	
        break;
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyAuthenticate:
		{
			if (sip_listGetAt(&(pResponseHdr->slProxyAuthenticateHdr), index_1, \
					&pTempHdr, err) == SipFail)
					return SipFail;
            pHeader = (SipCommonHeader *)pTempHdr;
		}	
		break;
#endif	
#ifdef SIP_RPR
		case SipHdrTypeRSeq:
		if (index_1 > 0)
		{
            *err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
            pHeader = pResponseHdr->pRSeqHdr;
		}
		break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
		if (sip_listGetAt(&(pResponseHdr->slPAssociatedUriHdr), index_1, \
					&pTempHdr, err) == SipFail)
			return SipFail;
		pHeader = (SipCommonHeader *)pTempHdr;
		break;
		case SipHdrTypeServiceRoute:
		if (sip_listGetAt(&(pResponseHdr->slServiceRouteHdr), index_1, \
					&pTempHdr, err) == SipFail)
			return SipFail;
		pHeader = (SipCommonHeader *)pTempHdr;
		break;
		
	case SipHdrTypeETag:
		if (index_1 > 0)
				{
                        *err = E_INV_INDEX;
						return SipFail;
				}
		else
				{
					pHeader =pResponseHdr->pETagHdr;
			  }
		break;
#endif	

		case SipHdrTypeMinExpires:
		if (index_1 > 0)
				{
                    *err = E_INV_INDEX;
                    return SipFail;
				}
		else
				{
					pHeader =pResponseHdr->pMinExpiresHdr;
			  }
		break;
#ifdef SIP_SECURITY
       case SipHdrTypeSecurityServer:
		if (sip_listGetAt(&(pResponseHdr->slSecurityServerHdr), index_1, \
					&pTempHdr, err) == SipFail)
			return SipFail;
		pHeader = (SipCommonHeader *)pTempHdr;
		break; 
#endif        
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
		if (sip_listGetAt(&(pResponseHdr->slAcceptRsrcPriorityHdr), index_1, \
				&pTempHdr, err) == SipFail)
				return SipFail;
				pHeader = (SipCommonHeader *)pTempHdr;
				break;
#endif
		default:
		*err = E_INV_TYPE;
		return SipFail;
	}
    if (pHeader == SIP_NULL)
    {
        *err = E_NO_EXIST;
        return SipFail;
    }
    (hdr)->pHeader = (SIP_Pvoid)pHeader;
#ifdef SIP_INCREMENTAL_PARSING
    if( pHeader->pBuffer!=SIP_NULL )
    {
        /*
         * Comming here means here header is not parsed
         * and  we need to call the parser to parse the header.
         * So we call parseHeaderBody function
         */

        SIP_U32bit length =0;
        length = sip_strlen(pHeader->pBuffer);
        if(SipFail != sip_parseHeaderBody\
                (pHeader->pBuffer, \
                 pHeader->pBuffer+(length-1), \
                 dType, hdr, err))
        {

            (void)fast_memfree(ACCESSOR_MEM_ID, \
                    pHeader->pBuffer, err);
            pHeader->pBuffer= SIP_NULL;
        }		
        else 
        {
            *err = E_PARSER_ERROR;
            return SipFail;	
        }	
    }
#endif
    HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
            dRefCount);
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function getResponseHeaderAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: setGeneralHeaderAtIndex
**
**  DESCRIPTION: This function sets a General pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pGeneralHdr(IN/OUT)	: The SipGeneralHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index_1(IN)			: The index_1 at which header is to be retrieved
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool setGeneralHeaderAtIndex
#ifdef ANSI_PROTO
	(SipGeneralHeader *pGeneralHdr, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(pGeneralHdr, hdr, index_1, err)
	SipGeneralHeader *pGeneralHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	/*en_SipBoolean z= SipFalse;*/
	SIPDEBUGFN("Entering function setGeneralHeaderAtIndex");
	
	switch(hdr->dType)
	{
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterAny:
		case SipHdrTypeRetryAfterSec:
#else
		case SipHdrTypeRetryAfter:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pRetryAfterHdr !=SIP_NULL)
				sip_freeSipRetryAfterHeader(pGeneralHdr->pRetryAfterHdr);
			pGeneralHdr->pRetryAfterHdr=(SipRetryAfterHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipRetryAfterHeader *) ((hdr)->pHeader))->\
				dRefCount);
		}
		break;

#ifdef SIP_SESSIONTIMER	
		case SipHdrTypeMinSE:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pMinSEHdr !=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pMinSEHdr);
			pGeneralHdr->pMinSEHdr=(SipCommonHeader *)(hdr->pHeader);
            HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->dRefCount);
		}
		break;
	    case SipHdrTypeSessionExpires:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pSessionExpiresHdr !=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->\
					pSessionExpiresHdr);
			pGeneralHdr->pSessionExpiresHdr=(SipCommonHeader *)\
				(hdr->pHeader);
            HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
                    dRefCount);
		}
		break;

#endif	
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
		if (sip_listSetAt(&(pGeneralHdr->slPEarlyMediaHdr), index_1, \
			 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
			dRefCount);
		break;
#endif
		case SipHdrTypeCseq:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pCseqHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pCseqHdr);
			pGeneralHdr->pCseqHdr=(SipCommonHeader *)(hdr->pHeader);
            HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->dRefCount);
		}
		break;
		case SipHdrTypeCallId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pCallidHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pCallidHdr);
			pGeneralHdr->pCallidHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
				dRefCount);
		}
		break;

#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresSec:
		case SipHdrTypeExpiresDate:
#else
		case SipHdrTypeExpires:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pExpiresHdr!=SIP_NULL)
				sip_freeSipExpiresHeader(pGeneralHdr->pExpiresHdr);
			pGeneralHdr->pExpiresHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
				dRefCount);
		}
		break;
		case SipHdrTypeMimeVersion:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pMimeVersionHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pMimeVersionHdr);		
				pGeneralHdr->pMimeVersionHdr=(SipCommonHeader *)\
					(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
				dRefCount);
		}
		break;

		case SipHdrTypeFrom:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pFromHeader!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pFromHeader);
			pGeneralHdr->pFromHeader=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
				dRefCount);
		 }
		 break;
		case SipHdrTypeRecordRoute:
		if (sip_listSetAt(&(pGeneralHdr->slRecordRouteHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
			dRefCount);
		break;
#ifdef SIP_3GPP
		case SipHdrTypePath:
		if (sip_listSetAt(&(pGeneralHdr->slPathHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
			dRefCount);
		break;
case SipHdrTypePcVector:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pPcVectorHdr !=SIP_NULL)
				sip_freeSipPcVectorHeader(pGeneralHdr->\
					pPcVectorHdr);
			pGeneralHdr->pPcVectorHdr=(SipPcVectorHeader *)\
				(hdr->pHeader);
		HSS_LOCKEDINCREF(((SipPcVectorHeader *)((hdr)->pHeader))->\
			dRefCount);
		}
		break;
		case SipHdrTypePcfAddr:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pPcfAddrHdr !=SIP_NULL)
				sip_freeSipPcfAddrHeader(pGeneralHdr->\
					pPcfAddrHdr);
			pGeneralHdr->pPcfAddrHdr=(SipPcfAddrHeader *)\
				(hdr->pHeader);
		HSS_LOCKEDINCREF(((SipPcfAddrHeader *)((hdr)->pHeader))->\
			dRefCount);
		}
		break;
		case SipHdrTypePanInfo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pPanInfoHdr !=SIP_NULL)
				sip_freeSipPanInfoHeader(pGeneralHdr->\
					pPanInfoHdr);
			pGeneralHdr->pPanInfoHdr=(SipPanInfoHeader *)\
				(hdr->pHeader);
		HSS_LOCKEDINCREF(((SipPanInfoHeader *)((hdr)->pHeader))->\
			dRefCount);
		}
		break;

		
		
		case SipHdrTypeHistoryInfo:
		if (sip_listSetAt(&(pGeneralHdr->slHistoryInfoHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipHistoryInfoHeader *)(hdr->pHeader))->\
			dRefCount);
		break;
#endif
		case SipHdrTypeSupported:
		if (sip_listSetAt(&(pGeneralHdr->slSupportedHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContentDisposition:
		if (sip_listSetAt(&(pGeneralHdr->slContentDispositionHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
			dRefCount);
		break;

		case SipHdrTypeAllow:
		if (sip_listSetAt(&(pGeneralHdr->slAllowHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;

#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pTimeStampHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pTimeStampHdr);
			pGeneralHdr->pTimeStampHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
				dRefCount);
		 }
		 break;
#endif		 
		case SipHdrTypeTo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pToHdr != SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pToHdr);
			pGeneralHdr->pToHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->dRefCount);
		 }
		 break;
		case SipHdrTypeVia:
		if (sip_listSetAt(&(pGeneralHdr->slViaHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContentEncoding:
		if (sip_listSetAt(&(pGeneralHdr->slContentEncodingHdr), index_1, \
			 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
			dRefCount);
		break;
		case SipHdrTypeRequire:
		if (sip_listSetAt(&(pGeneralHdr->slRequireHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContentLength:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pContentLengthHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->\
					pContentLengthHdr);
			pGeneralHdr->pContentLengthHdr=(SipCommonHeader *)\
				(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
				dRefCount);
		}
		break;
		case SipHdrTypeContentType:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pContentTypeHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pContentTypeHdr);
			pGeneralHdr->pContentTypeHdr=(SipCommonHeader *)\
				(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
		case SipHdrTypeContactWildCard:
		if (sip_listSetAt(&(pGeneralHdr->slContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContactNormal:
		if (sip_listSetAt(&(pGeneralHdr->slContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
		if (sip_listSetAt(&(pGeneralHdr->slAllowEventsHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
#endif
		
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pGeneralHdr->pPrivacyHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pGeneralHdr->pPrivacyHdr);
			pGeneralHdr->pPrivacyHdr=(SipCommonHeader *)\
				(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;

		case SipHdrTypePPreferredId:
		 if (sip_listSetAt(&(pGeneralHdr->slPPreferredIdHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;

		case SipHdrTypePAssertId:
		if (sip_listSetAt(&(pGeneralHdr->slPAssertIdHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;

#endif
		
		case SipHdrTypeAccept:
		if (sip_listSetAt(&(pGeneralHdr->slAcceptHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeUnknown:
		if (sip_listSetAt(&(pGeneralHdr->slUnknownHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function setGeneralHeaderAtIndex");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: insertedGeneralHeaderAtIndex
**
**  DESCRIPTION: This function inserts a General pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pGeneralHdr(IN/OUT)	: The SipGeneralHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index_1(IN)			: The index_1 at which header is to be retrieved
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool insertGeneralHeaderAtIndex
#ifdef ANSI_PROTO
	(SipGeneralHeader *pGeneralHdr, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(pGeneralHdr, hdr, index_1, err)
	SipGeneralHeader *pGeneralHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function insertGeneralHeaderAtIndex");

	switch(hdr->dType)
	{
#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pMinSEHdr == SIP_NULL)
			{
				pGeneralHdr->pMinSEHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
			if (sip_listInsertAt(&(pGeneralHdr->slPEarlyMediaHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
#endif
	case SipHdrTypeSessionExpires:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pSessionExpiresHdr == SIP_NULL)
			{
				pGeneralHdr->pSessionExpiresHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

#endif
		case SipHdrTypeCallId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pCallidHdr == SIP_NULL)
			{
				pGeneralHdr->pCallidHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresDate:
		case SipHdrTypeExpiresSec:
#else
		case SipHdrTypeExpires:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pExpiresHdr == SIP_NULL)
			{
				pGeneralHdr->pExpiresHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
	
#ifdef SIP_DATE
		case	SipHdrTypeRetryAfterAny:
		case	SipHdrTypeRetryAfterSec:
		case	SipHdrTypeRetryAfterDate:
#else
		case	SipHdrTypeRetryAfter:
#endif
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pRetryAfterHdr == SIP_NULL)
			{
				pGeneralHdr->pRetryAfterHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		case SipHdrTypeCseq:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pCseqHdr == SIP_NULL)
			{
				pGeneralHdr->pCseqHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		 }
		 break;
		case SipHdrTypeMimeVersion:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pMimeVersionHdr == SIP_NULL)
			{
				pGeneralHdr->pMimeVersionHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		 }
		 break;
		case SipHdrTypeFrom:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pFromHeader == SIP_NULL)
			{
				pGeneralHdr->pFromHeader=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		 }
		 break;
		case SipHdrTypeRecordRoute:
		if (sip_listInsertAt(&(pGeneralHdr->slRecordRouteHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)\
			(hdr->pHeader))->dRefCount);
		break;
#ifdef SIP_3GPP
		case SipHdrTypePath:
		if (sip_listInsertAt(&(pGeneralHdr->slPathHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)\
			(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypePanInfo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pPanInfoHdr == SIP_NULL)
			{
				pGeneralHdr->pPanInfoHdr=(SipPanInfoHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipPanInfoHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypePcfAddr:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pPcfAddrHdr == SIP_NULL)
			{
				pGeneralHdr->pPcfAddrHdr=(SipPcfAddrHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipPcfAddrHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypePcVector:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pPcVectorHdr == SIP_NULL)
			{
				pGeneralHdr->pPcVectorHdr=(SipPcVectorHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipPcVectorHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		

		case SipHdrTypeHistoryInfo:
		if (sip_listInsertAt(&(pGeneralHdr->slHistoryInfoHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipHistoryInfoHeader *)\
			(hdr->pHeader))->dRefCount);
		break;
#endif
		case SipHdrTypeSupported:
		if (sip_listInsertAt(&(pGeneralHdr->slSupportedHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeAllow:
		if (sip_listInsertAt(&(pGeneralHdr->slAllowHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContentDisposition:
			if (sip_listInsertAt(&(pGeneralHdr->slContentDispositionHdr),
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				(hdr->pHeader))->dRefCount);
			
			break;
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pTimeStampHdr == SIP_NULL)
			{
				pGeneralHdr->pTimeStampHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		 }
		 break;
#endif		 
		case SipHdrTypeTo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pToHdr == SIP_NULL)
			{
				pGeneralHdr->pToHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}	
		}
		break;
		case SipHdrTypeVia:
		 if (sip_listInsertAt(&(pGeneralHdr->slViaHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		 break;
		case SipHdrTypeContentEncoding:
			if (sip_listInsertAt(&(pGeneralHdr->slContentEncodingHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
		case SipHdrTypeRequire:
			if (sip_listInsertAt(&(pGeneralHdr->slRequireHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
			break;
		case SipHdrTypeContentLength:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pContentLengthHdr == SIP_NULL)
			{
				pGeneralHdr->pContentLengthHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		case SipHdrTypeContentType:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pGeneralHdr->pContentTypeHdr == SIP_NULL)
			{
				pGeneralHdr->pContentTypeHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF((((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount));
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypeContactWildCard:
		if (sip_listInsertAt(&(pGeneralHdr->slContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeContactAny:
		case SipHdrTypeContactNormal:
		if (sip_listInsertAt(&(pGeneralHdr->slContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
		if (sip_listInsertAt(&(pGeneralHdr->slAllowEventsHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
#endif
		
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pGeneralHdr->pPrivacyHdr == SIP_NULL)
			{
				pGeneralHdr->pPrivacyHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF((((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount));
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypePPreferredId:
		if (sip_listInsertAt(&(pGeneralHdr->slPPreferredIdHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;

		case SipHdrTypePAssertId:
		if (sip_listInsertAt(&(pGeneralHdr->slPAssertIdHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;

#endif
		
		case SipHdrTypeAccept:
		if (sip_listInsertAt(&(pGeneralHdr->slAcceptHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		case SipHdrTypeUnknown:
		if (sip_listInsertAt(&(pGeneralHdr->slUnknownHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
		break;
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function insertGeneralHeaderAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: setRequestHeaderAtIndex
**
**  DESCRIPTION: This function sets a Request pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pRequestHdr(IN/OUT)	: The SipRequestHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index_1(IN)			: The index_1 at which header is to be Set
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool setRequestHeaderAtIndex
#ifdef ANSI_PROTO
	(SipReqHeader *pRequestHdr, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(pRequestHdr, hdr, index_1, err)
	SipReqHeader *pRequestHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
/*	en_SipBoolean z=SipTrue;*/
	SIPDEBUGFN("Entering function setRequestHeaderAtIndex");
	

	switch(hdr->dType)
	{
#ifdef SIP_REPLACES	
		case SipHdrTypeReplaces:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pReplacesHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pReplacesHdr);
			pRequestHdr->pReplacesHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
				dRefCount);
		}
		break;
#endif
		case SipHdrTypeMaxforwards:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pMaxForwardsHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pMaxForwardsHdr);
			pRequestHdr->pMaxForwardsHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
				dRefCount);
		}
		break;
#ifdef SIP_IMPP
		case SipHdrTypeSubscriptionState:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pSubscriptionStateHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->\
					pSubscriptionStateHdr);
			pRequestHdr->pSubscriptionStateHdr=(SipCommonHeader *)\
				(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)((hdr)->pHeader))->\
				dRefCount);
		}
		break;
#endif
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
			if (sip_listSetAt(&(pRequestHdr->slAuthorizationHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
		case SipHdrTypeProxyauthorization:
			if (sip_listSetAt(&(pRequestHdr->slProxyAuthorizationHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
#endif	
		case SipHdrTypeRoute:
			if (sip_listSetAt(&(pRequestHdr->slRouteHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
			break;
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pReferToHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pReferToHdr);
			pRequestHdr->pReferToHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
				dRefCount);
		}
		break;
		case SipHdrTypeReferredBy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pReferredByHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pReferredByHdr);
			pRequestHdr->pReferredByHdr=(SipCommonHeader *)\
				(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *) \
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
#endif
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pRackHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pRackHdr);
			pRequestHdr->pRackHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
#endif
#ifdef SIP_IMPP
		case SipHdrTypeEvent:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pEventHdr!=SIP_NULL)
				sip_freeSipCommonHeader(pRequestHdr->pEventHdr);
			pRequestHdr->pEventHdr=(SipCommonHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;

#endif
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pPCalledPartyIdHdr!=SIP_NULL)
				sip_freeSipPCalledPartyIdHeader(pRequestHdr->pPCalledPartyIdHdr);
			pRequestHdr->pPCalledPartyIdHdr=(SipPCalledPartyIdHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipPCalledPartyIdHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
		case SipHdrTypeJoin:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pJoinHdr!=SIP_NULL)
				sip_freeSipJoinHeader(pRequestHdr->pJoinHdr);
			pRequestHdr->pJoinHdr=(SipJoinHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipJoinHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
	
		case SipHdrTypeIfMatch:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if(pRequestHdr->pIfMatchHdr!=SIP_NULL)
				sip_freeSipIfMatchHeader(pRequestHdr->pIfMatchHdr);
			pRequestHdr->pIfMatchHdr=(SipIfMatchHeader *)(hdr->pHeader);
			HSS_LOCKEDINCREF(((SipIfMatchHeader *)\
				((hdr)->pHeader))->dRefCount);
		 }
		 break;
		case SipHdrTypePVisitedNetworkId:
		if (sip_listSetAt(&(pRequestHdr->slPVisitedNetworkIdHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipPVisitedNetworkIdHeader *)(hdr->pHeader))->\
			dRefCount);
			break;
		case SipHdrTypeRequestDisposition:
		if (sip_listSetAt(&(pRequestHdr->slRequestDispositionHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipRequestDispositionHeader *)(hdr->pHeader))->\
			dRefCount);
			break;
		case SipHdrTypeAcceptContact:
		if (sip_listSetAt(&(pRequestHdr->slAcceptContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipAcceptContactHeader *)(hdr->pHeader))->\
			dRefCount);
			break;
		case SipHdrTypeRejectContact:
		if (sip_listSetAt(&(pRequestHdr->slRejectContactHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipRejectContactHeader *)(hdr->pHeader))->\
			dRefCount);
			break;

#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityClient:
        if (sip_listSetAt(&(pRequestHdr->slSecurityClientHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipSecurityClientHeader *)(hdr->pHeader))->\
			dRefCount);
			break;
        case SipHdrTypeSecurityVerify:
        if (sip_listSetAt(&(pRequestHdr->slSecurityVerifyHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipSecurityVerifyHeader *)(hdr->pHeader))->\
			dRefCount);
			break;
#endif            
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
        if (sip_listSetAt(&(pRequestHdr->slRsrcPriorityHdr), index_1, \
			(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
			return SipFail;
		HSS_LOCKEDINCREF(((SipRsrcPriorityHeader*)(hdr->pHeader))->\
			dRefCount);
			break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function setRequestHeaderAtIndex");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: insertRequestHeaderAtIndex
**
**  DESCRIPTION: This function inserts a Request pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pRequestHdr(IN/OUT)	: The SipRequestHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index_1(IN)			: The index_1 at which header is to be Inserted
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool insertRequestHeaderAtIndex
#ifdef ANSI_PROTO
	(SipReqHeader *pRequestHdr, SipHeader *hdr, SIP_U32bit index_1, SipError *err)
#else
	(pRequestHdr, hdr, index_1, err)
	SipReqHeader *pRequestHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function insertRequestHeaderAtIndex");

	switch(hdr->dType)
	{
#ifdef SIP_REPLACES
	case SipHdrTypeReplaces:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pRequestHdr->pReplacesHdr == SIP_NULL)
			{
				pRequestHdr->pReplacesHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

#endif
#ifdef SIP_IMPP
	case SipHdrTypeSubscriptionState:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pRequestHdr->pSubscriptionStateHdr == SIP_NULL)
			{
				pRequestHdr->pSubscriptionStateHdr=(SipCommonHeader\
					*)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

#endif
	case SipHdrTypeMaxforwards:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pRequestHdr->pMaxForwardsHdr == SIP_NULL)
			{
				pRequestHdr->pMaxForwardsHdr=(SipCommonHeader*)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeAuthorization:
			if (sip_listInsertAt(&(pRequestHdr->slAuthorizationHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
#endif			
		case SipHdrTypeRoute:
			if (sip_listInsertAt(&(pRequestHdr->slRouteHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
			break;
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyauthorization:
			if (sip_listInsertAt(&(pRequestHdr->slProxyAuthorizationHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	
#endif			
#ifdef SIP_IMPP			
		case SipHdrTypeEvent:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pRequestHdr->pEventHdr == SIP_NULL)
			{
				pRequestHdr->pEventHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#endif
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pRequestHdr->pRackHdr == SIP_NULL)
			{
				pRequestHdr->pRackHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#endif		
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pRequestHdr->pReferToHdr == SIP_NULL)
			{
				pRequestHdr->pReferToHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypeReferredBy:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pRequestHdr->pReferredByHdr == SIP_NULL)
			{
				pRequestHdr->pReferredByHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pRequestHdr->pPCalledPartyIdHdr == SIP_NULL)
			{
				pRequestHdr->pPCalledPartyIdHdr=(SipPCalledPartyIdHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipPCalledPartyIdHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		case SipHdrTypeJoin:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pRequestHdr->pJoinHdr == SIP_NULL)
			{
				pRequestHdr->pJoinHdr=(SipJoinHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipJoinHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		case SipHdrTypeIfMatch:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		 {
			if (pRequestHdr->pIfMatchHdr == SIP_NULL)
			{
				pRequestHdr->pIfMatchHdr=(SipIfMatchHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipIfMatchHeader *)\
					(hdr->pHeader))->dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;

		case SipHdrTypePVisitedNetworkId:
			if (sip_listInsertAt(&(pRequestHdr->slPVisitedNetworkIdHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipPVisitedNetworkIdHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	
		case SipHdrTypeRejectContact:
			if (sip_listInsertAt(&(pRequestHdr->slRejectContactHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipRejectContactHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	

		case SipHdrTypeAcceptContact:
			if (sip_listInsertAt(&(pRequestHdr->slAcceptContactHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipAcceptContactHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	

		case SipHdrTypeRequestDisposition:
			if (sip_listInsertAt(&(pRequestHdr->slRequestDispositionHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipRequestDispositionHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	
#endif	
#ifdef SIP_SECURITY
      	case SipHdrTypeSecurityClient:
			if (sip_listInsertAt(&(pRequestHdr->slSecurityClientHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipSecurityClientHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	      
        case SipHdrTypeSecurityVerify:
			if (sip_listInsertAt(&(pRequestHdr->slSecurityVerifyHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipSecurityVerifyHeader *)(hdr->pHeader))->\
				dRefCount);
			break;	
#endif
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
			if (sip_listInsertAt(&(pRequestHdr->slRsrcPriorityHdr),\
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipRsrcPriorityHeader*)(hdr->pHeader))->\
				dRefCount);
			break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function insertRequestHeaderAtIndex");
	return SipSuccess;
}

/*******************************************************************************
** FUNCTION: setResponseHeaderAtIndex
**
**  DESCRIPTION: This function inserts a  pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pResponseHdr(IN/OUT)	: The SipRespHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index_1(IN)			: The index_1 at which header is to be set
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool setResponseHeaderAtIndex
#ifdef ANSI_PROTO
	(SipRespHeader *pResponseHdr, SipHeader *hdr, SIP_U32bit index_1, 
	 SipError *err)
#else
	(pResponseHdr, hdr, index_1, err)
	SipRespHeader *pResponseHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function setResponseHeaderAtIndex");

	switch(hdr->dType)
	{
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeProxyAuthenticate:
			if (sip_listSetAt(&(pResponseHdr->slProxyAuthenticateHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
		case SipHdrTypeWwwAuthenticate:
			if (sip_listSetAt(&(pResponseHdr->slWwwAuthenticateHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
#endif			
		case SipHdrTypeUnsupported:
			if (sip_listSetAt(&(pResponseHdr->slUnsupportedHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;

#ifdef SIP_WARNING			
		case SipHdrTypeWarning:
			if (sip_listSetAt(&(pResponseHdr->slWarningHeader), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF((((SipCommonHeader *)(hdr->pHeader))->dRefCount));
			break;
#endif

#ifdef SIP_RPR			
		case SipHdrTypeRSeq:
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			else
			{
				if(pResponseHdr->pRSeqHdr!=SIP_NULL)
					sip_freeSipCommonHeader(pResponseHdr->pRSeqHdr);
				pResponseHdr->pRSeqHdr=(SipCommonHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *) ((hdr)->pHeader))->\
					dRefCount);
			 }
			 break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
			 if (sip_listSetAt(&(pResponseHdr->slPAssociatedUriHdr), index_1, \
						 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				 return SipFail;
			 HSS_LOCKEDINCREF(((SipPAssociatedUriHeader *)(hdr->pHeader))->\
					 dRefCount);
			 break;
		case SipHdrTypeServiceRoute:
			 if (sip_listSetAt(&(pResponseHdr->slServiceRouteHdr), index_1, \
						 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				 return SipFail;
			 HSS_LOCKEDINCREF(((SipServiceRouteHeader *)(hdr->pHeader))->\
					 dRefCount);
			 break;	
		case SipHdrTypeETag:
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			else
			{
				if(pResponseHdr->pETagHdr!=SIP_NULL)
					sip_freeSipETagHeader(pResponseHdr->pETagHdr);
				pResponseHdr->pETagHdr=(SipETagHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipETagHeader *) ((hdr)->pHeader))->\
					dRefCount);
			 }
			 break;

#endif
#ifdef SIP_SECURITY
    	case SipHdrTypeSecurityServer:
			 if (sip_listSetAt(&(pResponseHdr->slSecurityServerHdr), index_1, \
						 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				 return SipFail;
			 HSS_LOCKEDINCREF(((SipSecurityServerHeader *)(hdr->pHeader))->\
					 dRefCount);
			 break;	
#endif

		case SipHdrTypeMinExpires:
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			else
			{
				if(pResponseHdr->pMinExpiresHdr!=SIP_NULL)
					sip_freeSipMinExpiresHeader(pResponseHdr->pMinExpiresHdr);
				pResponseHdr->pMinExpiresHdr=(SipMinExpiresHeader *)(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipMinExpiresHeader *) ((hdr)->pHeader))->\
					dRefCount);
			 }
			 break;
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
			 if (sip_listSetAt(&(pResponseHdr->slAcceptRsrcPriorityHdr), index_1, \
						 (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				 return SipFail;
			 HSS_LOCKEDINCREF(((SipAcceptRsrcPriorityHeader*)(hdr->pHeader))->\
					 dRefCount);
			 break;	
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function setResponseHeaderAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: insertResponseHeaderAtIndex
**
**  DESCRIPTION: This function inserts a  pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		pResponseHdr(IN/OUT)	: The SipRespHeader 
**		dType(IN)				: The SipHeaderType
**		hdr(IN)					: The SipHeader to be inserted
**		index_1(IN)				: The index_1 at which header is to be inserted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool insertResponseHeaderAtIndex
#ifdef ANSI_PROTO
	(SipRespHeader *pResponseHdr, SipHeader *hdr, SIP_U32bit index_1, \
	 SipError *err)
#else
	(pResponseHdr, hdr, index_1, err)
	SipRespHeader *pResponseHdr;
	SipHeader *hdr;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function insertResponseHeaderAtIndex");

	switch(hdr->dType)
	{
#ifdef SIP_RPR	
		case SipHdrTypeRSeq:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pResponseHdr->pRSeqHdr == SIP_NULL)
			{
				pResponseHdr->pRSeqHdr=(SipCommonHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#endif		
		case SipHdrTypeUnsupported:
			if (sip_listInsertAt(&(pResponseHdr->slUnsupportedHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->\
				dRefCount);
			break;
#ifdef SIP_WARNING			
		case SipHdrTypeWarning:
			if (sip_listInsertAt(&(pResponseHdr->slWarningHeader), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)(hdr->pHeader))->dRefCount);
			break;
#endif			
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyAuthenticate:
			if (sip_listInsertAt(&(pResponseHdr->slProxyAuthenticateHdr),
				index_1, (SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				(hdr->pHeader))->dRefCount);
			break;
		case SipHdrTypeWwwAuthenticate:
			if (sip_listInsertAt(&(pResponseHdr->slWwwAuthenticateHdr), index_1, \
				(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipCommonHeader *)\
				(hdr->pHeader))->dRefCount);
			break;
#endif
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
			if (sip_listInsertAt(&(pResponseHdr->slPAssociatedUriHdr), index_1, \
						(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipPAssociatedUriHeader *)\
						(hdr->pHeader))->dRefCount);
			break;
		case SipHdrTypeServiceRoute:
			if (sip_listInsertAt(&(pResponseHdr->slServiceRouteHdr), index_1, \
						(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipServiceRouteHeader *)(hdr->pHeader))->\
					dRefCount);
			break;	
		case SipHdrTypeETag:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pResponseHdr->pETagHdr == SIP_NULL)
			{
				pResponseHdr->pETagHdr=(SipETagHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipETagHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
		
#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityServer:
			if (sip_listInsertAt(&(pResponseHdr->slSecurityServerHdr), index_1, \
						(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipSecurityServerHeader *)(hdr->pHeader))->\
					dRefCount);
			break;  
#endif
        
		case SipHdrTypeMinExpires:
		if (index_1 > 0)
		{
			*err = E_INV_INDEX;
			return SipFail;
		}
		else
		{
			if (pResponseHdr->pMinExpiresHdr == SIP_NULL)
			{
				pResponseHdr->pMinExpiresHdr=(SipMinExpiresHeader *)\
					(hdr->pHeader);
				HSS_LOCKEDINCREF(((SipMinExpiresHeader *)(hdr->pHeader))->\
					dRefCount);
			}
			else
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
		}
		break;
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
			if (sip_listInsertAt(&(pResponseHdr->slAcceptRsrcPriorityHdr), index_1, \
						(SIP_Pvoid)(hdr->pHeader), err) == SipFail)
				return SipFail;
			HSS_LOCKEDINCREF(((SipAcceptRsrcPriorityHeader*)(hdr->pHeader))->\
					dRefCount);
			break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function insertResponseHeaderAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: deleteGeneralHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipGeneralHeader 
**		dType(IN)				: The SipHeaderType
**		index_1(IN)				: The index_1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool deleteGeneralHdrAtIndex
#ifdef ANSI_PROTO
	(SipGeneralHeader *hdr, en_HeaderType dType, SIP_U32bit index_1, \
	 SipError *err)
#else
	(hdr, dType, index_1, err)
	SipGeneralHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	/*SIP_U32bit temp_count= 0;*/
	SIPDEBUGFN("Entering function deleteGeneralHdrAtIndex");
	switch(dType)
	{

#ifdef SIP_SESSIONTIMER
		case SipHdrTypeMinSE:
		if (hdr->pMinSEHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pMinSEHdr);
			hdr->pMinSEHdr = SIP_NULL;
		}
		break;
		case SipHdrTypeSessionExpires:
		if (hdr->pSessionExpiresHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pSessionExpiresHdr);
			hdr->pSessionExpiresHdr = SIP_NULL;
		}
		break;

#endif
#ifdef SIP_PEMEDIA
		case SipHdrTypePEarlyMedia:
		if (sip_listDeleteAt (&(hdr->slPEarlyMediaHdr), index_1, err)\
			== SipFail)
			return SipFail;
		break;
#endif
		case SipHdrTypeCallId:
		if (hdr->pCallidHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pCallidHdr);
			hdr->pCallidHdr = SIP_NULL;
		}
		break;
		
		case SipHdrTypeCseq:
		if (hdr->pCseqHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pCseqHdr);
			hdr->pCseqHdr = SIP_NULL;
		}
		break;

#ifdef SIP_DATE
		case	SipHdrTypeRetryAfterAny:
		case	SipHdrTypeRetryAfterSec:
		case	SipHdrTypeRetryAfterDate:
#else
		case	SipHdrTypeRetryAfter:
#endif
		if (hdr->pRetryAfterHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipRetryAfterHeader (hdr->pRetryAfterHdr);
			hdr->pRetryAfterHdr = SIP_NULL;
		}
		break;

		case SipHdrTypeFrom:
		if (hdr->pFromHeader == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pFromHeader);
			hdr->pFromHeader = SIP_NULL;
		}
		break;
		case SipHdrTypeRecordRoute:
		if (sip_listDeleteAt (&(hdr->slRecordRouteHdr), index_1, err)\
			 == SipFail)
			return SipFail;
		break;
#ifdef SIP_3GPP
		case SipHdrTypePath:
		if (sip_listDeleteAt (&(hdr->slPathHdr), index_1, err)\
			 == SipFail)
			return SipFail;
		break;
		case SipHdrTypePcVector:
		if (hdr->pPcVectorHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipPcVectorHeader (hdr->pPcVectorHdr);
			hdr->pPcVectorHdr = SIP_NULL;
		}
		break;

		case SipHdrTypePcfAddr:
		if (hdr->pPcfAddrHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipPcfAddrHeader (hdr->pPcfAddrHdr);
			hdr->pPcfAddrHdr = SIP_NULL;
		}
		break;

		case SipHdrTypePanInfo:
		if (hdr->pPanInfoHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		 }
		 else
		 {
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipPanInfoHeader (hdr->pPanInfoHdr);
			hdr->pPanInfoHdr = SIP_NULL;
		}
		break;


		case SipHdrTypeHistoryInfo:
		if (sip_listDeleteAt (&(hdr->slHistoryInfoHdr), index_1, err)\
			 == SipFail)
			return SipFail;
		break;
#endif
#ifdef SIP_TIMESTAMP			
		case SipHdrTypeTimestamp:
		if (hdr->pTimeStampHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pTimeStampHdr);
			hdr->pTimeStampHdr = SIP_NULL;
		}
		break;
#endif		
		case SipHdrTypeTo:
		if (hdr->pToHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pToHdr);
			hdr->pToHdr = SIP_NULL;
		}
		break;
		case SipHdrTypeVia:
		if (sip_listDeleteAt (&(hdr->slViaHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeContentEncoding:
		if (sip_listDeleteAt (&(hdr->slContentEncodingHdr), index_1, err)\
			== SipFail)
			return SipFail;
		break;
		case SipHdrTypeContentLength:
		if (hdr->pContentLengthHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pContentLengthHdr);
			hdr->pContentLengthHdr = SIP_NULL;
		}
		break;
		case SipHdrTypeContentType:
		if (hdr->pContentTypeHdr == SIP_NULL)
		{
/*commented eera*/
/*          *err = E_INV_INDEX;*/
            *err = E_NO_EXIST;          
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pContentTypeHdr);
			hdr->pContentTypeHdr = SIP_NULL;
		}
		break;
		case SipHdrTypeMimeVersion:
		if (hdr->pMimeVersionHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pMimeVersionHdr);
			hdr->pMimeVersionHdr = SIP_NULL;
		}
		break;

#ifdef SIP_DATE
		case SipHdrTypeExpiresAny:
		case SipHdrTypeExpiresDate:
		case SipHdrTypeExpiresSec:
#else
		case SipHdrTypeExpires:
#endif
		if (hdr->pExpiresHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipExpiresHeader (hdr->pExpiresHdr);
			hdr->pExpiresHdr = SIP_NULL;
		}
		break;
		case SipHdrTypeContactAny:
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactWildCard:
		if (sip_listDeleteAt (&(hdr->slContactHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeRequire:
		if (sip_listDeleteAt (&(hdr->slRequireHdr), index_1, err) == SipFail)
			return SipFail;
		break;
#ifdef SIP_IMPP
		case SipHdrTypeAllowEvents:
		if (sip_listDeleteAt (&(hdr->slAllowEventsHdr), index_1, err) == SipFail)
			return SipFail;
		break;
#endif
#ifdef SIP_PRIVACY
		case SipHdrTypePrivacy:

		if (hdr->pPrivacyHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipPrivacyHeader (hdr->pPrivacyHdr);
			hdr->pExpiresHdr = SIP_NULL;
		}
		break;
		case SipHdrTypePPreferredId:
		if (sip_listDeleteAt (&(hdr->slPPreferredIdHdr), index_1, err) == SipFail)
			return SipFail;
		break;

		case SipHdrTypePAssertId:
		if (sip_listDeleteAt (&(hdr->slPAssertIdHdr), index_1, err) == SipFail)
			return SipFail;
		break;


#endif
		case SipHdrTypeUnknown:
		if (sip_listDeleteAt (&(hdr->slUnknownHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeContentDisposition:
		if (sip_listDeleteAt (&(hdr->slContentDispositionHdr), \
			index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeAllow:
		if (sip_listDeleteAt (&(hdr->slAllowHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeAccept:
		if (sip_listDeleteAt (&(hdr->slAcceptHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		case SipHdrTypeSupported:
		if (sip_listDeleteAt (&(hdr->slSupportedHdr), index_1, err) == SipFail)
			return SipFail;
		break;
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteGeneralHdrAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: deleteRequestHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipReqHeader 
**		dType(IN)				: The SipHeaderType
**		index_1(IN)				: The index_1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool deleteRequestHdrAtIndex
#ifdef ANSI_PROTO
	(SipReqHeader *hdr, en_HeaderType dType, SIP_U32bit index_1, SipError *err)
#else
	(hdr, dType, index_1, err)
	SipReqHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function deleteRequestHdrAtIndex");

	switch(dType)
	{
		case SipHdrTypeMaxforwards:
		if (hdr->pMaxForwardsHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pMaxForwardsHdr);
			hdr->pMaxForwardsHdr = SIP_NULL;
		}
		break;
#ifdef SIP_IMPP
		case SipHdrTypeSubscriptionState:
		if (hdr->pSubscriptionStateHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader(hdr->pSubscriptionStateHdr);
			hdr->pSubscriptionStateHdr = SIP_NULL;
		}
		break;

#endif
#ifdef SIP_REPLACES
		case SipHdrTypeReplaces:
		if (hdr->pReplacesHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pReplacesHdr);
			hdr->pReplacesHdr = SIP_NULL;
		}
		break;
#endif
#ifdef SIP_AUTHENTICATE
		case SipHdrTypeAuthorization:
			if (sip_listDeleteAt (&(hdr->slAuthorizationHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#endif			
		case SipHdrTypeRoute:
			if (sip_listDeleteAt (&(hdr->slRouteHdr), index_1, err) == SipFail)
				return SipFail;
			break;
#ifdef SIP_AUTHENTICATE			
		case SipHdrTypeProxyauthorization:
			if (sip_listDeleteAt (&(hdr->slProxyAuthorizationHdr), \
				index_1, err) == SipFail)
				return SipFail;
			break;
#endif	
#ifdef SIP_REFER
		case SipHdrTypeReferTo:
		if (hdr->pReferToHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pReferToHdr);
			hdr->pReferToHdr = SIP_NULL;
		}
		break;

		case SipHdrTypeReferredBy:
		if (hdr->pReferredByHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader(hdr->pReferredByHdr);
			hdr->pReferredByHdr = SIP_NULL;
		}
		break;
#endif
#ifdef SIP_RPR
		case SipHdrTypeRAck:
		if (hdr->pRackHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pRackHdr);
			hdr->pRackHdr = SIP_NULL;
		}
		break;
#endif
#ifdef SIP_IMPP
		case SipHdrTypeEvent:
		if (hdr->pEventHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pEventHdr);
			hdr->pEventHdr= SIP_NULL;
		}
		break;

#endif
#ifdef SIP_3GPP
		case SipHdrTypePCalledPartyId:
		if (hdr->pPCalledPartyIdHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipPCalledPartyIdHeader (hdr->pPCalledPartyIdHdr);
			hdr->pPCalledPartyIdHdr = SIP_NULL;
		}
		break;

		case SipHdrTypeJoin:
		if (hdr->pJoinHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipJoinHeader (hdr->pJoinHdr);
			hdr->pJoinHdr = SIP_NULL;
		}
		break;

		case SipHdrTypeIfMatch:
		if (hdr->pIfMatchHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipIfMatchHeader (hdr->pIfMatchHdr);
			hdr->pIfMatchHdr = SIP_NULL;
		}
		break;

		case SipHdrTypePVisitedNetworkId:
			if (sip_listDeleteAt (&(hdr->slPVisitedNetworkIdHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;

		case SipHdrTypeRequestDisposition:
			if (sip_listDeleteAt (&(hdr->slRequestDispositionHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
		
		case SipHdrTypeAcceptContact:
			if (sip_listDeleteAt (&(hdr->slAcceptContactHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
		case SipHdrTypeRejectContact:
			if (sip_listDeleteAt (&(hdr->slRejectContactHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;

#endif
#ifdef SIP_SECURITY
		case SipHdrTypeSecurityClient:
			if (sip_listDeleteAt (&(hdr->slSecurityClientHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
       	case SipHdrTypeSecurityVerify:
			if (sip_listDeleteAt (&(hdr->slSecurityVerifyHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#endif
#ifdef SIP_CONGEST
		case SipHdrTypeRsrcPriority:
			if (sip_listDeleteAt (&(hdr->slRsrcPriorityHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#endif            
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}

	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteRequestHdrAtIndex");
	return SipSuccess;
}
/*******************************************************************************
** FUNCTION: deleteResponseHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index_1  specified by "index_1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipRespHeader 
**		dType(IN)				: The SipHeaderType
**		index_1(IN)				: The index_1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool deleteResponseHdrAtIndex
#ifdef ANSI_PROTO
	(SipRespHeader *hdr, en_HeaderType dType, SIP_U32bit index_1, SipError *err)
#else
	(hdr, dType, index_1, err)
	SipRespHeader *hdr;
	en_HeaderType dType;
	SIP_U32bit index_1;
	SipError *err;
#endif
{
	SIPDEBUGFN("Entering function deleteResponseHdrAtIndex");

	switch(dType)
	{
#ifdef SIP_AUTHENTICATE	
		case SipHdrTypeProxyAuthenticate:
			if (sip_listDeleteAt (&(hdr->slProxyAuthenticateHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
		case SipHdrTypeWwwAuthenticate:
			if (sip_listDeleteAt (&(hdr->slWwwAuthenticateHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#endif
		case SipHdrTypeUnsupported:
			if (sip_listDeleteAt (&(hdr->slUnsupportedHdr), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#ifdef SIP_WARNING			
		case SipHdrTypeWarning:
			if (sip_listDeleteAt (&(hdr->slWarningHeader), index_1, err)\
				== SipFail)
				return SipFail;
			break;
#endif			
#ifdef SIP_RPR			
		case SipHdrTypeRSeq:
		if (hdr->pRSeqHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipCommonHeader (hdr->pRSeqHdr);
			hdr->pRSeqHdr = SIP_NULL;
		}
		break;
#endif	
#ifdef SIP_3GPP
		case SipHdrTypePAssociatedUri:
			if (sip_listDeleteAt (&(hdr->slPAssociatedUriHdr), index_1, err)\
				 == SipFail)
				return SipFail;
			break;
		case SipHdrTypeServiceRoute:
		if (sip_listDeleteAt (&(hdr->slServiceRouteHdr), index_1, err)\
			 == SipFail)
			return SipFail;
		break;
		case SipHdrTypeETag:
		if (hdr->pETagHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipETagHeader (hdr->pETagHdr);
			hdr->pETagHdr = SIP_NULL;
		}
		break;

#endif
#ifdef SIP_SECURITY
        case SipHdrTypeSecurityServer:
    		if (sip_listDeleteAt (&(hdr->slSecurityServerHdr), index_1, err)\
	    		 == SipFail)
		    	return SipFail;
    		break;
#endif
		case SipHdrTypeMinExpires:
		if (hdr->pMinExpiresHdr == SIP_NULL)
		{
			*err = E_NO_EXIST;
			return SipFail;
		}
		else
		{
			if (index_1 > 0)
			{
				*err = E_INV_INDEX;
				return SipFail;
			}
			sip_freeSipMinExpiresHeader (hdr->pMinExpiresHdr);
			hdr->pMinExpiresHdr = SIP_NULL;
		}
		break;
#ifdef SIP_CONGEST
		case SipHdrTypeAcceptRsrcPriority:
    		if (sip_listDeleteAt (&(hdr->slAcceptRsrcPriorityHdr), index_1, err)\
	    		 == SipFail)
		    	return SipFail;
    		break;
#endif
		default:
			*err = E_INV_TYPE;
			return SipFail;
	}
	*err = E_NO_ERROR;
	SIPDEBUGFN("Exitting function deleteResponseHdrAtIndex");
	return SipSuccess;
}



/*******************************************************************************
** FUNCTION: sip_equateTypeInSipHeader
**
**  DESCRIPTION: Makes the type of the header structure consistent with
** with the type of the encapsulated header structure.
** PARAMETERS:
**		hdr(IN/OUT)				: The SipHeader 
** Return Value:
**				  void	
*******************************************************************************/

void sip_equateTypeInSipHeader
#ifdef ANSI_PROTO
	(SipHeader *hdr)
#else
	(hdr)
	SipHeader *hdr;
#endif
{
	switch(hdr->dType)
	{
		case SipHdrTypeContactAny:
			if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1 == SipContactNormal)
				hdr->dType = SipHdrTypeContactNormal;
			else if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1\
					== SipContactWildCard)
					hdr->dType = SipHdrTypeContactWildCard;
			break;
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterAny:
			if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1 == SipExpDate)
				hdr->dType = SipHdrTypeRetryAfterDate;
			else if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1\
					== SipExpSeconds)
					hdr->dType = SipHdrTypeRetryAfterSec;
			break;
		case SipHdrTypeExpiresAny:
			if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1 == SipExpDate)
				hdr->dType = SipHdrTypeExpiresDate;
			else if(((SipCommonHeader *)(hdr->pHeader))->dIntVar1\
				== SipExpSeconds)
				hdr->dType = SipHdrTypeExpiresSec;
			break;
#endif /* SIP_DATE */
	
		default:
			break;
	}
}

/*******************************************************************************
** FUNCTION: sip_verifyTypeAny
**
**  DESCRIPTION:Internal functionto verify if the pHeader has an ANY
**		dType
** PARAMETERS:
**		dType(IN/)				: The SipHeaderType 
**		err(OUT)				: The Error Value 
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_verifyTypeAny
#ifdef ANSI_PROTO	
	(   
	  en_HeaderType 	dType, 
	  SipError		*err )	
#else	
	( dType, err )
	  en_HeaderType 	dType;
	  SipError		*err;	
#endif	
{
	switch(dType)
	{
#ifdef SIP_DATE
		case SipHdrTypeExpiresDate:
		case SipHdrTypeExpiresSec:
#endif
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactWildCard:
#ifdef SIP_DATE
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterSec:
#endif
			*err = E_INV_TYPE;
			return SipFail;
		default :
			*err = E_NO_ERROR;
			return SipSuccess;
	}
}

/*******************************************************************************
** FUNCTION: sip_changeTypeAny
**
**  DESCRIPTION: This function changes the Type to ANY dType if it exists
**		dType
** PARAMETERS:
**		dType(IN/OUT)				: The SipHeader 
**		err(OUT)					: The Error Value 
** Return Value:
**				  SipBool	
****************************************************************************/
SipBool sip_changeTypeAny
#ifdef ANSI_PROTO	
	(   
	  en_HeaderType 	*dType, 
	  SipError		*err )	
#else	
	( dType, err )
	  en_HeaderType 	*dType;
	  SipError		*err;	
#endif	
{
	switch (*dType)
	{
		case SipHdrTypeContactNormal:
		case SipHdrTypeContactWildCard:
				*dType = SipHdrTypeContactAny;
				break;
#ifdef SIP_DATE
		case SipHdrTypeExpiresDate:
		case SipHdrTypeExpiresSec:
				*dType = SipHdrTypeExpiresAny;
				break;
		case SipHdrTypeRetryAfterDate:
		case SipHdrTypeRetryAfterSec:
				*dType = SipHdrTypeRetryAfterAny;
				break;
#endif
		default :
				break;
	}
	*err = E_NO_ERROR;
	return SipSuccess;
}


/*********************************************************************
** FUNCTION:  sip_getValueAtIndexFromSipParam
**********************************************************************
**
** DESCRIPTION:	Gets the Value at a specified index_1 ( starting from 
** 0 ) from Param strucutre.
**
*********************************************************************/
SipBool sip_getValueAtIndexFromSipParam
#ifdef ANSI_PROTO
	( SipParam 	*pParam,
	  SIP_S8bit 	**ppValue,
	  SIP_U32bit 	index_1, 
	  SipError 	*pErr )
#else 
	( pParam,ppValue,index_1,pErr)
	  SipParam *pParam;
	  SIP_S8bit **ppValue;
	  SIP_U32bit index_1;
	  SipError *pErr;
#endif
{
	SIP_Pvoid pElementFromList;
#ifndef SIP_BY_REFERENCE
	SIP_U32bit size;
#endif

	SIPDEBUGFN ( "Entering GeValueAtIndexFromSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( (pParam == SIP_NULL) || (ppValue == SIP_NULL) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listGetAt(&(pParam->slValue), index_1, &pElementFromList, pErr) == \
			SipFail)
	{
		return SipFail;
	}	
	
	if ( pElementFromList == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;		
	}

#ifdef SIP_BY_REFERENCE
	*ppValue = (SIP_S8bit*)pElementFromList;
#else
	size = sip_strlen( (SIP_S8bit * )pElementFromList);
	*ppValue = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID, size +1, pErr);
	if(*ppValue == SIP_NULL)
		return SipFail;

	(void)sip_strcpy(*ppValue, (SIP_S8bit*)pElementFromList);
#endif
	SIPDEBUGFN ( "Exitting GeValueAtIndexFromSipParam");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}

/*********************************************************************
** FUNCTION:  sip_getNameFromSipParam
**********************************************************************

** DESCRIPTION: Gets the Name field pValue from SipParam.

*********************************************************************/
SipBool sip_getNameFromSipParam
#ifdef ANSI_PROTO
	( SipParam *pParam, SIP_S8bit **ppName, SipError *pErr)
#else
	( pParam, ppName, pErr )
	  SipParam *pParam;
	  SIP_S8bit **ppName;
	  SipError *pErr;
#endif
{
	SIPDEBUGFN ( "Entering getNameFromSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( ( pParam == SIP_NULL) || (ppName == SIP_NULL))
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif	
	if( pParam->pName == SIP_NULL )
	{
		*pErr = E_NO_EXIST;
		return SipFail;
	}	
#ifdef SIP_BY_REFERENCE
	*ppName = pParam->pName;
#else
	*ppName = (SIP_S8bit *)STRDUPACCESSOR(pParam->pName);
	if(*ppName == SIP_NULL)
	{
		*pErr = E_NO_MEM;
		return SipFail;
	}
#endif
	SIPDEBUGFN ( "Exiting getNameFromSipParam");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}

/*********************************************************************
** FUNCTION:  sip_deleteValueAtIndexInSipParam
**********************************************************************
**
** DESCRIPTION: Deletes a Value pValue at a specified index_1 
** ( starting from 0 )  in SipParam strucutre.
**
*********************************************************************/
SipBool sip_deleteValueAtIndexInSipParam
#ifdef ANSI_PROTO
	( SipParam 	*pParam,
	  SIP_U32bit 	index_1,
	  SipError 	*pErr )
#else 
	( pParam,index_1,pErr)
	  SipParam 	*pParam;
	  SIP_U32bit 	index_1;
	  SipError 	*pErr;
#endif
{

	SIPDEBUGFN ( "Entering DeleteiValueAtIndexInSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;
		
	if ( pParam == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if( sip_listDeleteAt(&(pParam->slValue), index_1, pErr) == SipFail)
		return SipFail;
	
	SIPDEBUGFN ( "Exitting DeleteValueAtIndexinSipParam");

	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/*********************************************************************
** FUNCTION: sip_setNameInSipParam
**********************************************************************

** DESCRIPTION: Sets the Name field pValue in SipParam.

*********************************************************************/
SipBool sip_setNameInSipParam
#ifdef ANSI_PROTO
	( SipParam *pParam, SIP_S8bit *pName, SipError *pErr)
#else
	( pParam, pName, pErr )
	  SipParam *pParam;
	  SIP_S8bit *pName;
	  SipError *pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
	SIP_S8bit *pTempName;
#endif

	SIPDEBUGFN ( "Entering setNameInSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pParam == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pParam->pName != SIP_NULL ) 
	{
		 if( sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pParam->pName)), pErr) == SipFail)
			return SipFail;
	}

#ifdef SIP_BY_REFERENCE
	pParam->pName = pName;
#else
	if( pName == SIP_NULL)
		pTempName = SIP_NULL;
	else
	{
		pTempName = (SIP_S8bit *)STRDUPACCESSOR(pName);
		if(pTempName == SIP_NULL)
			return SipFail;
	}
	

	pParam->pName = pTempName; 
#endif
	SIPDEBUGFN ( "Exiting setNameInSipParam");

	*pErr = E_NO_ERROR;
	return SipSuccess;

}
/*********************************************************************
** FUNCTION:  sip_insertValueAtIndexInSipParam
**********************************************************************
**
** DESCRIPTION:	Inserts a Value at a specified index_1 
** ( starting from 0 )  in  SipParam strucutre.
**
*********************************************************************/
SipBool sip_insertValueAtIndexInSipParam
#ifdef ANSI_PROTO
	( SipParam 	*pParam,
	  SIP_S8bit 	*pValue,
	  SIP_U32bit 	index_1,
	  SipError 	*pErr )
#else 
	( pParam,pValue,index_1,pErr)
	  SipParam 	*pParam;
	  SIP_S8bit 	*pValue;
	  SIP_U32bit 	index_1;
	  SipError 	*pErr;
#endif
{
		
	SIP_S8bit * pElementFromList;
	SipError tempErr;		/* used in freeing memory after an pError has happened */


	SIPDEBUGFN ( "Entering InserValueAtIndexInSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if ( pParam == SIP_NULL )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	/* copying the Value Headers structure/char*  */		
	if ( pValue == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
#ifdef SIP_BY_REFERENCE
		pElementFromList = 	pValue;
#else
		pElementFromList = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,\
			sip_strlen(pValue) + 1, pErr);
		if( pElementFromList == SIP_NULL )
			return SipFail;

		(void)sip_strcpy(pElementFromList, pValue);
#endif
	}

	if( sip_listInsertAt(&(pParam->slValue), index_1, pElementFromList, pErr) \
			== SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)),\
				   	&tempErr);
		return SipFail;
	}

	SIPDEBUGFN ( "Exitting InserValueAtIndexinSipParam");
	
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/*********************************************************************
** FUNCTION:  sip_setValueAtIndexInSipParam
**********************************************************************
**
** DESCRIPTION:	Sets the Value pValue at a specified index_1 
** ( starting from 0 )in Sip SipParam strucutre.
**
*********************************************************************/
SipBool sip_setValueAtIndexInSipParam
#ifdef ANSI_PROTO
	( SipParam 	*pParam, 
	  SIP_S8bit 	*pValue,
	  SIP_U32bit 	index_1,
	  SipError 	*pErr )
#else 
	( pParam,pValue,index_1,pErr)
	  SipParam 	*pParam;
	  SIP_S8bit 	*pValue;
	  SIP_U32bit 	index_1;
	  SipError 	*pErr;
#endif
{
		
	SIP_S8bit * pElementFromList;
	SipError tempErr;		
	/* used in freeing memory after an pError has happened */

	SIPDEBUGFN ( "Entering SeValueAtIndexInSipParam");
#ifndef SIP_NO_CHECK
	if( pErr == SIP_NULL )
		return SipFail;

	if (pParam == SIP_NULL) 
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if ( pValue == SIP_NULL )
		pElementFromList = SIP_NULL;
	else
	{
#ifdef SIP_BY_REFERENCE
		pElementFromList = 	pValue;
#else
		
		pElementFromList = ( SIP_S8bit * )fast_memget\
			(ACCESSOR_MEM_ID, sip_strlen(pValue) + 1, pErr);
		if( pElementFromList == SIP_NULL )
			return SipFail;
	
		(void)sip_strcpy(pElementFromList, pValue);
#endif
	}

	if( sip_listSetAt(&(pParam->slValue), index_1, pElementFromList, pErr)\
		   	== SipFail)
	{
		if ( pElementFromList != SIP_NULL )
			(void)sip_memfree(ACCESSOR_MEM_ID, (SIP_Pvoid *)(&(pElementFromList)),\
				   	&tempErr);
		return SipFail;
	}
	
	SIPDEBUGFN ( "Exitting SeValueAtIndexinSipParam");
	*pErr = E_NO_ERROR;
	return SipSuccess;
}
/*********************************************************************
** FUNCTION:  sip_getValueCountFromSipParam
**********************************************************************
**
** DESCRIPTION:	Gets the number of Value nodes present in Sip 
** SipParam structure.
**
*********************************************************************/
SipBool sip_getValueCountFromSipParam
#ifdef ANSI_PROTO
	( SipParam	*pParam,
	  SIP_U32bit	*pIndex,
	  SipError	*pErr  )
#else 
	( pParam,pIndex,pErr)
	  SipParam 	*pParam;
	  SIP_U32bit 	*pIndex;
	  SipError 	*pErr;
#endif
{

	SIPDEBUGFN ( "Entering GeValueCountFromSipParam");
#ifndef SIP_NO_CHECK
	if ( pErr == SIP_NULL ) 
		return SipFail;
	
	if ( (pParam == SIP_NULL) || ( pIndex == SIP_NULL ) )
	{
		*pErr = E_INV_PARAM;
		return SipFail;
	}
#endif
	if (sip_listSizeOf(&(pParam->slValue), pIndex , pErr) == SipFail)
	{
		return SipFail;
	}
	
	SIPDEBUGFN ( "Exitting GeValueCountFromSipParam");
	*pErr = E_NO_ERROR;
	return SipSuccess;	
}

/*API added for IPTK Rel 8.1(UATK Enhancement)*/

/***************************************************************
** FUNCTION: sip_getExpiresFromContactParam
**
** DESCRIPTION: This function retrieves the expires field from a
**		SIP contact-param
**
***************************************************************/

SipBool sip_getExpiresFromContactParam
#ifdef ANSI_PROTO
#ifdef SIP_BY_REFERENCE
    (SipContactParam *pHdr,
     SipExpiresStruct **ppExpires,
     SipError *pErr)
#else
    (SipContactParam *pHdr,
     SipExpiresStruct *pExpires,
     SipError *pErr)
#endif
#else		/* ANSI_PROTO */
#ifdef SIP_BY_REFERENCE
    ( pHdr, ppExpires, pErr )
      SipContactParam *pHdr;
      SipExpiresStruct **ppExpires;
      SipError *pErr;
#else
    ( pHdr, pExpires, pErr )
      SipContactParam *pHdr;
      SipExpiresStruct *pExpires;
	  SipError *pErr;
#endif
#endif
{
    SipExpiresStruct *pFrom = SIP_NULL;

    SIPDEBUGFN( "Entering getExpiresFromContactParam");
#ifndef SIP_NO_CHECK
    if( SIP_NULL == pErr )
	    return SipFail;
#ifdef SIP_BY_REFERENCE
    if ( SIP_NULL == pHdr || SIP_NULL == ppExpires )
#else
    if ( SIP_NULL == pHdr || SIP_NULL == pExpires )
#endif
    {
        /*Set the value of pErr as invalid parameter*/
        *pErr = E_INV_PARAM;
	return SipFail;
    }
    if( SipCParamExpires != pHdr->dType )
    {
        /*Set the value of pErr as invalid parameter*/
        *pErr = E_INV_TYPE;
        return SipFail;
    }
#endif
    pFrom=pHdr->u.pExpire;
    if ( SIP_NULL == pFrom )
    {
        *pErr = E_NO_EXIST;
        return SipFail;
    }
#ifdef SIP_BY_REFERENCE
    /*Increment the value of Refcount for pFrom*/
    HSS_LOCKEDINCREF(pFrom->dRefCount);
    *ppExpires = pFrom;
#else
    if( SipFail == __sip_cloneSipExpiresStruct(pExpires,pFrom,pErr) )
    {
        return SipFail;
    }
#endif
    SIPDEBUGFN( "Exiting getExpiresFromContactParam ");

    *pErr=E_NO_ERROR;
    return SipSuccess;
}


/*API added for IPTK Rel 8.1(UATK Enhancement)*/

/***************************************************************
** FUNCTION: sip_getExtensionAttrFromContactParam
**
** DESCRIPTION: This function retrieves the extension-attribute
**		from a SIP contact-param
**
***************************************************************/

SipBool sip_getExtensionAttrFromContactParam
#ifdef ANSI_PROTO
    (SipContactParam *pCp,
     SIP_S8bit **ppExtensionAttribute,
     SipError *pErr)
#else
    ( pCp, ppExtensionAttribute , pErr )
     SipContactParam *pCp;
     SIP_S8bit **ppExtensionAttribute;
     SipError * pErr;
#endif
{
#ifndef SIP_BY_REFERENCE
    SIP_U16bit dLength = 0;
#endif
    SIP_S8bit * pTemp_extnAttr = SIP_NULL;
    SIPDEBUGFN("Entering getExtensionAttrFromContactParam");

#ifndef SIP_NO_CHECK
    if(SIP_NULL == pErr)
        return SipFail;

    if (SIP_NULL == pCp || SIP_NULL == ppExtensionAttribute)
    {
        /*Set the value of pErr as invalid parameter*/
        *pErr = E_INV_PARAM;
        return SipFail;
    }

    if( SipCParamExtension != pCp->dType )
    {
        /*Set the value of pErr as invalid Type*/
        *pErr = E_INV_TYPE;
        return SipFail;
    }
#endif
    pTemp_extnAttr = pCp->u.pExtensionAttr;

    if(SIP_NULL == pTemp_extnAttr)
    {
        *pErr = E_NO_EXIST;
        return SipFail;
    }

#ifdef SIP_BY_REFERENCE
    *ppExtensionAttribute = pTemp_extnAttr;
#else
    dLength = sip_strlen(pTemp_extnAttr );
    /*Allocating memory to extension attribute*/
    *ppExtensionAttribute = ( SIP_S8bit * )fast_memget(ACCESSOR_MEM_ID,dLength+1,pErr);

    if (SIP_NULL == *ppExtensionAttribute)
        return SipFail;

    (void)sip_strcpy( *ppExtensionAttribute, pTemp_extnAttr );  
#endif
    SIPDEBUGFN( "Exiting getExtensionAttrFromContactParam");
    *pErr = E_NO_ERROR;

    return SipSuccess;
}


/*API added for IPTK Rel 8.1(UATK Enhancement)*/

/*********************************************************
** FUNCTION:    sip_getTypeFromContactParam
**
** DESCRIPTION: This function retrieves the dType of a SIP
**		contact-param
**
**********************************************************/
SipBool sip_getTypeFromContactParam
#ifdef ANSI_PROTO
    (SipContactParam *pCp,
     en_ContactParamsType *pType,
     SipError *pErr)
#else
    ( pCp, pType , pErr )
     SipContactParam *pCp;  
     en_ContactParamsType *pType;
     SipError * pErr;
#endif
{
    SIPDEBUGFN( "Entering getTypeFromContactParam");
#ifndef SIP_NO_CHECK
    if( SIP_NULL == pErr )
        return SipFail;

    if ( SIP_NULL == pCp )
    {
        /*Set the value of pErr as invalid parameter*/
        *pErr = E_INV_PARAM;
        return SipFail;
    }
    if ( SIP_NULL == pType )
    {
        /*Set the value of pErr as invalid parameter*/
        *pErr = E_INV_PARAM;
        return SipFail;
    }
#endif

    /*ptype is pointing to dType of a sip contact parameter*/
    *pType = pCp->dType;
    SIPDEBUGFN( "Exiting getTypeFromContactParam");
    *pErr=E_NO_ERROR;
    return SipSuccess;
}

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
