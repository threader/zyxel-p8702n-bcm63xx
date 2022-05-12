/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 ** FUNCTION: 		Top and mid-level parser functionality
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_decode.h
 **
 ** DESCRIPTION:	This file contains the entry point into the stack for
 **					the application to parse a SIP message. The mid-level
 **					parser and body parsers are invoked from this function
 **					and are also coded in this file.
 **
 ** DATE        	NAME			REFERENCE		REASON
 ** ----        	----			---------		--------
 ** 24-Feb-2002  	Siddharth		-				Initial Creation
 **					Toshniwal
 **
 *****************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 *****************************************************************************/ 

#ifndef __MICROSIP_DECODE_H__
#define __MICROSIP_DECODE_H__

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"

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
extern SipBool Sip_parseMsrpUrl(SIP_S8bit  *pBuf, MsrpUrl *pMsrpUrl, SipError *pErr);

    
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

extern SipBool sip_parseHostPortInMsrp(SIP_S8bit *,SIP_S8bit *,SIP_S8bit
		**,SIP_U16bit**,SipError *); 


#endif


/*****************************************************************************
 ** FUNCTION: 		sip_decodeMessage 
 **
 ** DESCRIPTION:	This function decodes the plain text message and 
 **					invokes an appropriate callback.
 **
 ** PARAMETERS:
 **		message(IN): 	The text message that is to be parsed.
 **
 **		opt(IN): 	 	Options to be used while parsing the text message
 **
 **		messageLength(IN): The length of the message to be parsed.
 **
 **		nextmesg(OUT): 	This will contain any trailing text from
 **						text passed after the parser parses one SIP message.
 **						*nextmessage will be SIP_NULL if the text contained
 **						one SIP message. 
 **
 **		context(IN): 	A SipEventContext structure that can be filled
 **						by the application with any information. This 
 **						structure is passed on to the indicate callbacks. 
 **						The structure contains a field pData (void pointer) 
 **						that may be used to pass information.
 **
 **		err(OUT): The error code returned if the call fails.
 **
 **
 **	NOTE: 	In the case the stack returns a E_INCOMPLETE error in pErr field, 
 **			it also returns back the number of bytes more it was expecting 
 **			in the message in the pContext->dRemainingLength field. Note 
 **			that if the user passed EventContext as SIP_NULL, the stack 
 **			cannot set the dRemainingBytes field and hence only returns 
 **			E_INCOMPLETE. 
 **
 **			This case happens when the stack sees a content length which is 
 **			greater that the body supplied. In cases that the stack returns 
 **			E_MAYBE_INCOMPLETE, it does not set 
 **			this field as it does not know how many more bytes to expect. 
 **			This typically happens if some header is incomplete. 
 **
 **	    	The above error values are passed *only if* the stack is compiled 
 **			with SIP_INCOMPLETE_MSG_CHECK option.
 **
 ** NOTE: 	If the user invokes this in the SIP_NO_CALLBACK mode,
 ** 		sip_decodeMessage has an extra parameter of pointer to 
 **			pointer to decoded message.
 ** 		In this scenario, the stack returns the decoded message 
 **			into this variable and does not invoke the sip_indicateXXXX 
 **			callbacks.
 **
 ****************************************************************************/
#ifdef SIP_NO_CALLBACK
extern SipBool sip_decodeMessage _ARGS_( (SIP_S8bit *message, \
					SipMessage **ppDecodedMsg, SipOptions *opt, \
					SIP_U32bit messageLength, SIP_S8bit **nextmesg, \
					SipEventContext *context, SipError *err) );
#else
extern SipBool sip_decodeMessage _ARGS_( ( SIP_S8bit *message, \
					SipOptions *opt, SIP_U32bit messageLength, \
					SIP_S8bit **nextmesg, SipEventContext *context, \
					SipError *err) );
#endif



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
SipBool sip_parseHeaderBody _ARGS_((  SIP_S8bit *pStart, \
	 SIP_S8bit *pEnd, en_HeaderType dHfrType, SipHeader *pHeader, \
	SipError *pError));

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
SipBool sip_parseSipBuffer _ARGS_( (  SIP_S8bit *pMessage, \
	 SipMessage **ppDecodedMsg, SipOptions *pOpt, \
	 SIP_U32bit dMessageLength, SIP_S8bit **ppNextMsg, \
	 SipEventContext *pContext, SipMimeHeader *pMimeHdrs, SipError *pErr));
#else
SipBool sip_parseSipBuffer _ARGS_( (  SIP_S8bit *pMessage, \
	 SipMessage **ppDecodedMsg, SipOptions *pOpt, \
	 SIP_U32bit dMessageLength, SIP_S8bit **ppNextMsg, \
	 SipEventContext *pContext, SipError *pErr));
#endif

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
extern SipBool sip_handleIncParser(SIP_S8bit *pStart,SIP_S8bit *pEnd,\
	SipMessage *ppSipMessage,	SipEventContext *pContext,	\
	en_HeaderType dHdrType, en_SipBoolean mimeHdrParsing, \
	SipMimeHeader *pMimeHdrs, SIP_S8bit * pNameStart, \
	SIP_S8bit * pNameEnd , SipError *pErr );
#else
extern SipBool sip_handleIncParser(SIP_S8bit *pStart,SIP_S8bit *pEnd,\
	SipMessage *ppSipMessage,	SipEventContext *pContext,	\
	en_HeaderType dHdrType, en_SipBoolean mimeHdrParsing, \
	SIP_S8bit * pNameStart, \
	SIP_S8bit * pNameEnd , SipError *pErr );

#endif


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
			SipMessage *pSipMessage,en_HeaderType dType,SipError *pError);


#ifndef SIP_TXN_LAYER
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


SipBool sip_decodeInvokeTimerCallbacks(SipMessage *tempMessage, SipEventContext *pContext,SipError *err);
#endif


#ifndef SIP_NO_CALLBACK
/****************************************************************************
** FUNCTION: sip_indicateComet
** DESCRIPTION: The stack issues this call back when it reveives a COMET 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateComet(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateRegister
** DESCRIPTION: The stack issues this call back when it reveives a REGISTER 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateRegister(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateCancel
** DESCRIPTION: The stack issues this call back when it reveives a CANCEL 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateCancel(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateOptions
** DESCRIPTION: The stack issues this call back when it reveives an OPTIONS
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateOptions(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateBye
** DESCRIPTION: The stack issues this call back when it reveives a BYE 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateBye(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateRefer
** DESCRIPTION: The stack issues this call back when it reveives a REFER 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateRefer(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateAck
** DESCRIPTION: The stack issues this call back when it reveives an ACK 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateAck(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateInfo
** DESCRIPTION: The stack issues this call back when it reveives an INFO 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateInfo(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicatePropose
** DESCRIPTION: The stack issues this call back when it reveives a PROPOSE
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicatePropose(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicatePrack
** DESCRIPTION: The stack issues this call back when it reveives a PRACK 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicatePrack(SipMessage *s, SipEventContext *context);
#ifdef SIP_3GPP
/****************************************************************************
** FUNCTION: sip_indicatePublish
** DESCRIPTION: The stack issues this call back when it reveives a PUBLISH 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicatePublish(SipMessage *s, SipEventContext *context);
#endif

#ifdef SIP_IMPP
/****************************************************************************
** FUNCTION: sip_indicateSubscribe
** DESCRIPTION: The stack issues this call back when it reveives a SUBSCRIBE 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateSubscribe(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateMessage
** DESCRIPTION: The stack issues this call back when it reveives a MESSAGE 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateMessage(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateNotify
** DESCRIPTION: The stack issues this call back when it reveives a NOTIFY 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateNotify(SipMessage *s, SipEventContext *context);

#endif
/****************************************************************************
** FUNCTION: sip_indicateUnknownRequest
** DESCRIPTION: The stack issues this call back when it reveives a request  
** with unknown method 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateUnknownRequest(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateInformational
** DESCRIPTION: The stack issues this call back when it reveives a  
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateInformational(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateInvite
** DESCRIPTION: The stack issues this call back when it reveives an INVITE 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateInvite(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateFinalResponse
** DESCRIPTION: The stack issues this call back when it reveives a 
**			FINAl response 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateFinalResponse(SipMessage *s, SipEventContext *context);

/****************************************************************************
** FUNCTION: sip_indicateUpdate
** DESCRIPTION: The stack issues this call back when it reveives an UPDATE 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicateUpdate(SipMessage *s, SipEventContext *context);
#endif

#if 0
#ifdef SIP_3GPP
/****************************************************************************
** FUNCTION: sip_indicatePublish
** DESCRIPTION: The stack issues this call back when it reveives an PUBLIC 
** Method.  
** 
** PARAMETERS:
**      	      pSipMessage(IN):The decodedSipMessage
**				  pContext(IN)	:The SipEventContext Structure
**
****************************************************************************/
extern void sip_indicatePublish(SipMessage *s, SipEventContext *context);
#endif
#endif
#endif	/* Prevent multiple inclusions */

#ifdef __cplusplus
}  /* Ensure Names are not mangled by C++ compilers */
#endif
