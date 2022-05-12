/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION: 		This header file contains the prototypes of all 
 **					Accessor functions.	
 **
 ******************************************************************************
 **
 ** FILENAME:		microsip_accessors.h
 **
 ** DESCRIPTION:	This header file contains the protypes of all the 
 **					Accessor functions.
 **
 ** DATE      		NAME	    	REFERENCE	   	REASON
 ** ----      		----	    	---------	   	------
 ** 28-Feb-2002	 	Siddharth 		accessors.h		Creation
 **					Toshniwal		of SipLite
 **
 ******************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef __MICROSIP_ACCESSORS_H__
#define __MICROSIP_ACCESSORS_H__

#include "microsip_struct.h"
#include "microsip_common.h"
#include "microsip_portlayer.h"


/***********************************************************************
** FUNCTION: sip_getTagfromFromHeader
** Description: Tag From  From Header.
** Parameters:
**		SipFromHeader(IN/OUT): The message in which the header form is to be set.
**		abs_line(IN): The header line to be modified.
**		type(IN): The form to be set.
**		err(OUT): Error returned if the function fails.
**
************************************************************************/

extern SipBool sip_getTagfromFromHeader _ARGS_ ((SipFromHeader *pSipFromHeader,
								SIP_S8bit** ppTag,
								SipError *pError)) ;

/***********************************************************************
** FUNCTION: 	sip_getHeaderCountFromHeaderLine 
** Description: Retrieves the number of headers found in a header line
** Parameters:
**		msg(IN): The message from which the information is being retrieved.
**		abs_line(IN): The header line being probed.
**		count(OUT): The number of headers in the requested line.
**		err(OUT): Error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderCountFromHeaderLine _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, SIP_U32bit *count, SipError *err));


/***********************************************************************
** FUNCTION: sip_getHeaderFormFromHeaderLine 
** Description: Gets the form (short name or full name) of the header at
**		a given line of a message.
** Parameters:
**		msg(IN): The message from which the information is being retrieved.
**		abs_line(IN): The header line from which the form is to be retrieved.
**		type(OUT): The form of the header at the given line.
**		err(OUT): Error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderFormFromHeaderLine _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, en_HeaderForm *type, SipError *err ));

/***********************************************************************
** FUNCTION: sip_getHeaderCount 
** Description: Gets the number of headers of a given type in the message.
**		If the header is not present, the function returns SipSuccess with
**		count set to 0.
** Parameters:
**		msg(IN): The message from which the count is to be retrieved.
**		type(IN): The type of the header for which the count is to be
**			extracted. The type must be an Any type for headers like
**			Contact or Expires which have types.
**		count(OUT): The number of header present in the message. 
**		err(OUT): Error code in case the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderCount _ARGS_ ((SipMessage *msg, \
		en_HeaderType type, SIP_U32bit *count, SipError *err));

/***********************************************************************
** FUNCTION: sip_getHeaderAtIndex 
** Description: Gets the header of the desired type at the given index1.
** Parameters:
**	msg(IN): The message from which the header is to be retrieved.
**	type(IN): The type of the header to be retrieved. The type must
**		be an Any type for headers like Contact or Expires.
**	hdr(OUT): The retrieved header.
**	index1(IN): The index1 of the header to be retrieved.
**	err(OUT): The error code is returned in this if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderAtIndex _ARGS_ ((SipMessage *msg, \
		en_HeaderType type,  SipHeader *hdr, SIP_U32bit index1, SipError *err));

/***********************************************************************
** FUNCTION: sip_insertHeaderAtIndex 
** Description: Insert a header in the message at the specified index1.
** Parameters:
**	msg(IN/OUT): The message in which the header is to be inserted.
**	hdr(IN): The header to be inserted.
**	index1(IN): The index1 at which the header is to be inserted.
**	err(OUT): The error code is returned in this if the function fails.
**
************************************************************************/
extern SipBool sip_insertHeaderAtIndex _ARGS_ ((SipMessage *msg, \
		SipHeader *hdr, SIP_U32bit index1, SipError *err));

/***********************************************************************
** FUNCTION: sip_setHeaderAtIndex 
** Description: Set a header at the specified index1 in the message.
** Parameters:
**	msg(IN/OUT): The message in which the header is to be set.
**	hdr(IN): The header to be set.
**	index1(IN): The index1 at which the header is to be set.
**	err(OUT): The error code is returned in this if the function fails.
**
************************************************************************/
extern SipBool sip_setHeaderAtIndex _ARGS_ ((SipMessage *msg, \
		SipHeader *hdr, SIP_U32bit index1, SipError *err));

/***********************************************************************
** FUNCTION: sip_deleteHeaderAtIndex 
** Description: Deletes a header from the message at the specified index1.
** Parameters:
**	msg(IN/OUT): The message from which the header is to be deleted.
**	type(IN): The type of the header to be deleted.
**	index1(IN): The index1 of the header to be deleted.
**	err(OUT): The error code is returned in this if the function fails.
**
************************************************************************/
extern SipBool sip_deleteHeaderAtIndex _ARGS_ ((SipMessage *msg, \
		en_HeaderType type, SIP_U32bit index1, SipError *err));

/***********************************************************************
** FUNCTION: sip_deleteAllHeaderType 
** Description: Deletes all headers of a particular type.
** Parameters:
**	msg(IN/OUT): The message from which the header is to be deleted.
**	type(IN): The type of the header to be deleted.
**	err(OUT): The error code is returned in this if the function fails.
**
************************************************************************/
extern SipBool sip_deleteAllHeaderType _ARGS_ ((SipMessage *msg, \
		en_HeaderType type, SipError *err));

/***********************************************************************
** FUNCTION: sip_getMessageType 
** Description: Retrieve the type of the message (Response/Request).
** Parameters:
**		msg(IN): The message for which the type is to be determined.
**		type(OUT): The type of the message. (SipMessageResponse or
**			SipMessageRequest)
**		err(OUT): The error code if the function fails.
**
************************************************************************/
extern SipBool sip_getMessageType _ARGS_ ((SipMessage *msg, \
		en_SipMessageType *type, SipError *err));

/***********************************************************************
** FUNCTION: sip_getStatusLineFromSipRespMsg 
**			 sip_getStatusLine
** Description: Retrieves the status line from the messge if it is 
**		a response message.
** Parameters:
**		msg(IN): The message from which the status line is to be retrieved.
**		line(OUT): The retrieved status line.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
#define sip_getStatusLine sip_getStatusLineFromSipRespMsg
extern SipBool sip_getStatusLineFromSipRespMsg _ARGS_ ((SipMessage *msg, \
		SipStatusLine **line, SipError *err));

/***********************************************************************
** FUNCTION: sip_setStatusLineInSipRespMsg 
**			 sip_setStatusLine 
** Description: Set the status line in the message.
** Parameters:
**		msg(IN/OUT): The response message in which the status line is to
**			be set. 
**		line(IN): The status line to be set.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
#define sip_setStatusLine sip_setStatusLineInSipRespMsg
extern SipBool sip_setStatusLineInSipRespMsg _ARGS_ ((SipMessage *msg, \
		SipStatusLine *line, SipError *err));

/***********************************************************************
** FUNCTION: sip_getReqLineFromSipReqMsg 
**			 sip_getReqLine
** Description: Get the request line from a request message.
** Parameters:
**		msg(IN): The message from which the request line is to be 
**			retrieved.
**		line(OUT): The retrieved request line.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
#define sip_getReqLine sip_getReqLineFromSipReqMsg
extern SipBool sip_getReqLineFromSipReqMsg _ARGS_ ((SipMessage *msg, \
		SipReqLine **line, SipError *err));
/***********************************************************************
** FUNCTION: sip_setReqLineInSipReqMsg 
**			 sip_setReqLine
** Description: Sets the request line in a request message
** Parameters:
**		msg(IN/OUT): The message in which the request line is to be set.
**		line(IN): The request line to be set.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
#define sip_setReqLine sip_setReqLineInSipReqMsg
extern SipBool sip_setReqLineInSipReqMsg _ARGS_ ((SipMessage *msg, \
		SipReqLine *line, SipError *err));

/***********************************************************************
** FUNCTION: sip_setHeader 
** Description: Same as setHeaderAtIndex with index1 0.
************************************************************************/
#define sip_setHeader(x,y,z) sip_setHeaderAtIndex(x,y,0,z)

/***********************************************************************
** FUNCTION: sip_getHeader 
** Description: Same as sip_getHeaderAtIndex with index1 0.
************************************************************************/
#define sip_getHeader(a,b,c,d) sip_getHeaderAtIndex(a,b,c,0,d)

/***********************************************************************
** FUNCTION: sip_getHeaderLineCount 
** Description: Gets the number of header lines in a message.
** Parameters:
**		msg(IN): The message from which the line count is to be retrieved.
**		count(OUT): Contains the number of header lines in the message.
**			This is different from the number of headers in a message.
**			A message may having comma separated headers has fewer header
**			lines than headers. The request/status line is not included
**			in this count.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderLineCount _ARGS_ ((SipMessage *msg, \
		SIP_U32bit *count, SipError *err));


/***********************************************************************
** FUNCTION: sip_getHeaderPositionFromIndex 
** Description: Retrieves the exact location of a header in a message.
** Parameters:
**		msg(IN): The message in which the header position is to be 
**			determined.
**		type(IN): The type of the header for which the position is to be
**			determined.
**		list_index1(IN): The index1 of the header.
**		abs_line(OUT): The line in the message where the header is present.
**		position(OUT): The index1 of the header within the header line.
**
************************************************************************/
extern SipBool sip_getHeaderPositionFromIndex _ARGS_ ((SipMessage *msg, \
		en_HeaderType type, SIP_U32bit list_index1, SIP_U32bit *abs_line, \
				SIP_U32bit *position, SipError *err));


/***********************************************************************
** FUNCTION: sip_getHeaderIndexFromPosition 
** Description: Retrieves the index1 of a header at a position in the message.
** Parameters:
**		msg(IN): The message from which the index1 is to be retrieved.
**		abs_line(IN): The header line being probed.
**		position(IN): The position of the header in the header line.
**		index1(OUT): The index1 of the header in the position.
**		err(OUT): Error code if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderIndexFromPosition _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, SIP_U32bit position, SIP_U32bit *index1, SipError *err));


/***********************************************************************
** FUNCTION: sip_getHeaderTypeAtHeaderLine 
** Description: Gets the type of the header at a given line in the message.
** Parameters:
**		msg(IN): The message from which the information is to be retrieved.
**		line(IN): The header line to be probed.
**		type(OUT): The type of the header found at the line.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
extern SipBool sip_getHeaderTypeAtHeaderLine _ARGS_ ((SipMessage *msg, \
		SIP_U32bit line, en_HeaderType *type, SipError *err));




/***********************************************************************
** FUNCTION: sip_setHeaderFormAtHeaderLine 
** Description: Sets the form (short name or full name) of the header at
**		the given line.
** Parameters:
**		msg(IN/OUT): The message in which the header form is to be set.
**		abs_line(IN): The header line to be modified.
**		type(IN): The form to be set.
**		err(OUT): Error returned if the function fails.
**
************************************************************************/
extern SipBool sip_setHeaderFormAtHeaderLine _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, en_HeaderForm type, SipError *err ));


/***********************************************************************
** FUNCTION: sip_insertHeaderAtPosition 
** Description: Inserts a header in the message at a given position.
** Parameters:
**		msg(IN/OUT): The message in which the header is to be inserted.
**		hdr(IN): The header to be inserted.
**		abs_line(IN): The header line at which the header is to be inserted.
**		position(IN): The position in the line where the header is to be
**			inserted.
**		mode(IN): Whether the insertion is to result in a new header line
**			or comma separated one.
**		err(OUT): The error code if the function fails.
**
************************************************************************/
extern SipBool sip_insertHeaderAtPosition _ARGS_ ((SipMessage *msg, \
		SipHeader *hdr,  SIP_U32bit abs_line, SIP_U32bit position, \
				en_AdditionMode mode, SipError *err));


/***********************************************************************
** FUNCTION: sip_deleteHeaderAtPosition 
** Description: Delete a header at given position in the message
** Parameters:
**		msg(IN/OUT): The message from which the header is to be deleted.
**		abs_line(IN): The header line from which the header is to be 
**			deleted.
**		position(IN): The position in the header line from which the 
**			header is to be deleted.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_deleteHeaderAtPosition _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, SIP_U32bit position, SipError *err));


/***********************************************************************
** FUNCTION: sip_deleteHeaderLine 
** Description: Delete all headers in a header line from a message.
** Parameters:
**		msg(IN/OUT): The message from which the line is to be deleted.
**		abs_line(IN): The header line which is to be deleted.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_deleteHeaderLine _ARGS_ ((SipMessage *msg, \
		SIP_U32bit abs_line, SipError *err));


/***********************************************************************
** FUNCTION: sip_getMsgBodyAtIndex 
** Description: Gets the message body at the specified index1 from the message.
** Parameters:
**		s(IN): The message from which the message body is to be retrieved.
**		msgbody(OUT): The retrieved message body.
**		index1(IN): The index1 from which the message body is to be retrieved.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_getMsgBodyAtIndex _ARGS_((SipMessage *s, \
		SipMsgBody **msgbody, SIP_U32bit index1, SipError *err));

/***********************************************************************
** FUNCTION: sip_setMsgBodyAtIndex
** Description: Sets the message body at the given index1 in the message.
** Parameters:
**		s(IN/OUT): The message in which the body is being set.
**		msgbody(IN): The message body being set.
**		index1(IN): The index1 at which the body is to be set.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_setMsgBodyAtIndex _ARGS_((SipMessage *s, \
		SipMsgBody *msgbody, SIP_U32bit index1, SipError *err));


/***********************************************************************
** FUNCTION: sip_insertMsgBodyAtIndex
** Description: Inserts the message body at the given index1 in the message.
** Parameters:
**		s(IN/OUT): The message in which the body is being inserted.
**		msgbody(IN): The message body being inserted.
**		index1(IN): The index1 at which the body is to be inserted.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_insertMsgBodyAtIndex _ARGS_((SipMessage *s, \
		SipMsgBody *msgbody, SIP_U32bit index1, SipError *err));


/***********************************************************************
** FUNCTION: sip_deleteMsgBodyAtIndex
** Description: Deletes a message body at a give index1.
** Parameters:
**		s(IN/OUT): The message from which the body is to be deleted.
**		index1(IN): The index1 of the message body to be deleted.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_deleteMsgBodyAtIndex _ARGS_((SipMessage *s, \
		SIP_U32bit index1, SipError *err));


/***********************************************************************
** FUNCTION: sip_getMsgBodyTypeAtIndex 
** Description: Gets the type of the message body at the given index1.
** Parameters:
**		s(IN): The message from which the type is to be determined.
**		type(OUT): The type of the message body 
**			SipSdpBody - The body contains an SDP message.
**			SipIsupBody - The body contains an ISUP message.
**			SipMultipartMimeBody - The body conatiain a Multipart-MIME body.
**			SipUnknownBody - An unknown type.
**		index1(IN): The index1 at which the type is to be probed.
**
************************************************************************/
extern SipBool sip_getMsgBodyTypeAtIndex _ARGS_((SipMessage *s, \
		en_SipMsgBodyType *type, SIP_U32bit index1, SipError *err));


/***********************************************************************
** FUNCTION: sip_getTypeFromMsgBody 
** Description: Gets the type of the body from a retrieved message body
** Parameters:
**		s(IN) - The message body being probed.
**		dType(OUT): The type of the message body
**			SipSdpBody - The body contains an SDP message.
**			SipIsupBody - The body contains an ISUP message.
**			SipMultipartMimeBody - The body conatiain a Multipart-MIME body.
**			SipUnknownBody - An unknown type.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_getTypeFromMsgBody _ARGS_((SipMsgBody *s, \
		en_SipMsgBodyType *dType, SipError *err));


/***********************************************************************
** FUNCTION: sip_getMsgBodyCount 
** Description: Gets the number of message body elements in a message
** Parameters:
**		s(IN): The message from which the count is to be retrieved.
**		count(OUT): The number of message body elements in the message.
**		err(OUT): The error code returned if the function fails.
**
************************************************************************/
extern SipBool sip_getMsgBodyCount _ARGS_((SipMessage *s, \
		SIP_U32bit *count, SipError *err));


/*******************************************************************************
** FUNCTION: isSipGeneralHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a**	 general header or not.If dType isa general header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  en_SipBoolean isSipGeneralHeader _ARGS_ ((en_HeaderType type));

/*******************************************************************************
** FUNCTION: isSipReqHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a**	 request header or not.If dType isa request header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  en_SipBoolean isSipReqHeader _ARGS_ ((en_HeaderType type));

/*******************************************************************************
** FUNCTION: isSipRespHeader
**
** DESCRIPTION:This function Checks whether the dType given to the function is a**	respose header or not.If dType is a respose header it will return SipSuccess
** PARAMETERS:
**		dType(IN)		: The SipHeaderType
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  en_SipBoolean isSipRespHeader _ARGS_ ((en_HeaderType type));

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
extern  SipBool getGeneralHdrCount _ARGS_ ((SipGeneralHeader *hdr, \
	en_HeaderType type, SIP_U32bit *count, SipError *err));

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
extern  SipBool getRequestHdrCount _ARGS_ ((SipReqHeader *hdr, \
	en_HeaderType type, SIP_U32bit *count, SipError *err));

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
extern  SipBool getResponseHdrCount _ARGS_ ((SipRespHeader *hdr, \
	en_HeaderType type, SIP_U32bit *count, SipError *err));

/*******************************************************************************
** FUNCTION: getGeneralHeaderAtIndex
**
**  DESCRIPTION: This function returns the general pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pGeneralHdr(IN)	: The SipGeneralHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index1(IN)		: The index1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/	
extern  SipBool getGeneralHeaderAtIndex _ARGS_ ((SipGeneralHeader *gen_hdr, \
	en_HeaderType type, SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: getRequestHeaderAtIndex
**
**  DESCRIPTION: This function returns the Request pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pRequestHdr(IN)	: The SipRequestHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index1(IN)		: The index1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/	
extern  SipBool getRequestHeaderAtIndex _ARGS_ ((SipReqHeader *req_hdr, \
	en_HeaderType type, SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: getResponseHeaderAtIndex
**
**  DESCRIPTION: This function returns the Response pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pResponseHdr(IN): The SipResponseHeader 
**		dType(IN)		: The SipHeaderType
**		hdr(OUT)		: The SipHeader retrieved
**		index1(IN)		: The index1 at which header is to be retrieved
**		err(OUT)		: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool getResponseHeaderAtIndex _ARGS_ ((SipRespHeader *resp_hdr, \
	en_HeaderType type, SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: deleteGeneralHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipGeneralHeader 
**		dType(IN)				: The SipHeaderType
**		index1(IN)				: The index1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool deleteGeneralHdrAtIndex _ARGS_ ((SipGeneralHeader *hdr, \
	en_HeaderType type, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: deleteRequestHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipReqHeader 
**		dType(IN)				: The SipHeaderType
**		index1(IN)				: The index1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/	
extern  SipBool deleteRequestHdrAtIndex _ARGS_ ((SipReqHeader *hdr, \
	en_HeaderType type, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: deleteResponseHdrAtIndex
**
**  DESCRIPTION: This function deletes a  pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		hdr(IN/OUT)				: The SipRespHeader 
**		dType(IN)				: The SipHeaderType
**		index1(IN)				: The index1 at which header is to be deleted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/	
extern  SipBool deleteResponseHdrAtIndex _ARGS_ ((SipRespHeader *hdr, \
	en_HeaderType type, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: setGeneralHeaderAtIndex
**
**  DESCRIPTION: This function sets a General pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pGeneralHdr(IN/OUT)	: The SipGeneralHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index1(IN)			: The index1 at which header is to be retrieved
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool setGeneralHeaderAtIndex _ARGS_ ((SipGeneralHeader *gen_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: insertGeneralHeaderAtIndex
**
**  DESCRIPTION: This function inserts a General pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pGeneralHdr(IN/OUT)	: The SipGeneralHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index1(IN)			: The index1 at which header is to be retrieved
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool insertGeneralHeaderAtIndex _ARGS_ ((SipGeneralHeader *gen_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: setRequestHeaderAtIndex
**
**  DESCRIPTION: This function sets a Request pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pRequestHdr(IN/OUT)	: The SipRequestHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index1(IN)			: The index1 at which header is to be Set
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool setRequestHeaderAtIndex _ARGS_ ((SipReqHeader *req_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: insertRequestHeaderAtIndex
**
**  DESCRIPTION: This function inserts a Request pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pRequestHdr(IN/OUT)	: The SipRequestHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index1(IN)			: The index1 at which header is to be Inserted
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool insertRequestHeaderAtIndex _ARGS_ ((SipReqHeader *req_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: setResponseHeaderAtIndex
**
**  DESCRIPTION: This function inserts a  pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pResponseHdr(IN/OUT)	: The SipRespHeader 
**		dType(IN)			: The SipHeaderType
**		hdr(IN)				: The SipHeader to be set
**		index1(IN)			: The index1 at which header is to be set
**		err(OUT)			: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool setResponseHeaderAtIndex _ARGS_ ((SipRespHeader *resp_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

/*******************************************************************************
** FUNCTION: insertResponseHeaderAtIndex
**
**  DESCRIPTION: This function inserts a  pHeader of 
**	en_HeaderType "dType" at the index1  specified by "index1".
** PARAMETERS:
**		pResponseHdr(IN/OUT)	: The SipRespHeader 
**		dType(IN)				: The SipHeaderType
**		hdr(IN)					: The SipHeader to be inserted
**		index1(IN)				: The index1 at which header is to be inserted
**		err(OUT)				: The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
extern  SipBool insertResponseHeaderAtIndex _ARGS_ ((SipRespHeader *resp_hdr, \
	SipHeader *hdr, SIP_U32bit index1, SipError *err));

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
	
extern void sip_equateTypeInSipHeader _ARGS_((SipHeader *hdr));

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
extern SipBool sip_verifyTypeAny _ARGS_(( en_HeaderType	 type, SipError *err));

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
extern SipBool sip_changeTypeAny _ARGS_(( en_HeaderType *type, SipError	*err ));


extern SipBool sip_setSecondsInExpiresHdr(SipHeader *hdr, SIP_U32bit dSec, \
		SipError *err);
extern SipBool sip_setSecondsInExpires(SipExpiresStruct *hdr, SIP_U32bit dSec, \
		SipError *err);
extern SipBool sip_getSecondsFromExpires(SipExpiresStruct *hdr, SIP_U32bit \
		*dSec, SipError *err);
extern SipBool sip_getValueAtIndexFromSipParam( SipParam 	*pParam, SIP_S8bit\
		**ppValue,  SIP_U32bit 	index1,   SipError 	*pErr );
extern SipBool sip_getNameFromSipParam( SipParam *pParam, SIP_S8bit **ppName, \
		SipError *pErr);
extern SipBool sip_deleteValueAtIndexInSipParam( SipParam 	*pParam, SIP_U32bit\
		index1,  SipError 	*pErr );
extern SipBool sip_setNameInSipParam( SipParam *pParam, SIP_S8bit *pName, \
		SipError *pErr);
extern SipBool sip_insertValueAtIndexInSipParam( SipParam 	*pParam, \
		SIP_S8bit 	*pValue,  SIP_U32bit 	index1,  SipError 	*pErr );
extern SipBool sip_setValueAtIndexInSipParam( SipParam 	*pParam,  SIP_S8bit \
		*pValue,  SIP_U32bit 	index1,  SipError 	*pErr );
extern SipBool sip_getValueCountFromSipParam( SipParam	*pParam,  SIP_U32bit\
		*pIndex,  SipError	*pErr);

#endif /* __MICROSIP_ACCESSORS_H__ */

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
	}
#endif
