/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 ** FUNCTION: 		This file contains callbacks that are to be 
 **					implemented by the application developer.
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_callbacks.h
 **
 ** DESCRIPTION:	This file contains callbacks that are to be implemented 
 **					by the application developer. Some of these callbacks 
 **					are optional (based upon the compilation flags chosen).
 **
 ** DATE        	NAME			REFERENCE		REASON
 ** ----        	----			---------		--------
 ** 24-Feb-2002  	Siddharth		-				Initial Creation
 **					Toshniwal
 **
 *****************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 *****************************************************************************/ 

#ifndef __MICROSIP_CALLBACKS_H__
#define __MICROSIP_CALLBACKS_H__

#include "microsip_common.h"
#include "microsip_struct.h"


/**********************************************************
** Function: sip_indicateInvite
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an INVITE message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateInvite _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateComet
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an Comet message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateComet _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateRegister
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an REGISTER message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateRegister _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateAck
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an ACK message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateAck _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateCancel
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an CANCEL message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateCancel _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateBye
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an BYE message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateBye _ARGS_((SipMessage *s,  \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateOptions
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an OPTIONS message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateOptions _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateInfo
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an INFO message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateInfo _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicatePropose
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an PROPOSE message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicatePropose _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicatePrack
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an PRACK message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicatePrack _ARGS_((SipMessage *s, \
	SipEventContext *context));

#ifdef SIP_REFER
/**********************************************************
** Function: sip_indicateRefer
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an REFER message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateRefer _ARGS_((SipMessage *s, \
	SipEventContext *context));
#endif

#ifdef SIP_IMPP
/**********************************************************
** Function: sip_indicateSubscribe
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an SUBSCRIBE message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateSubscribe _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateNotify
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an NOTIFY message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateNotify _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateMessage
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an MESSAGE message
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateMessage _ARGS_((SipMessage *s, \
	SipEventContext *context));

#endif

/**********************************************************
** Function: sip_indicateUnknownRequest
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		a request with an unknown method.
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateUnknownRequest _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateInformational
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		an informational response (1xx).
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateInformational _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateFinalResponse
** Description:
**		Callback function to be implemented by the application.
**		Invoked if the message decoded by sip_decodeMessage is
**		a final response (200 and above).
** Parameters:
**		s(IN): The structure containig the decoded message.
**			This must be freed using sip_freeSipMessage call
**			when it is to be released.
**		context(IN):
**			The event context structure that was passed to the
**			sip_decodeMessage invocation that resulted in this
**			callback.
**
************************************************************/
extern void sip_indicateFinalResponse _ARGS_((SipMessage *s, \
	SipEventContext *context));

/**********************************************************
** Function: sip_indicateTimeout
** Description:
**		Callback function to be implemented by the application.
**		Invoked if a message that was sent through UDP with
**		retransmissions did not receive a matching response
**		after the maximum retransmissions have been sent.
** Parameters:
**		context(IN):
**			The event context structure that was passed to the
**			sip_sendMessage invocation that resulted in this
**			callback.
**
************************************************************/
#ifdef SIP_TXN_LAYER
extern void sip_indicateTimeOut _ARGS_ ((SipEventContext *context, \
										 en_SipTimerType dTimer));
#else
extern void sip_indicateTimeOut _ARGS_ ((SipEventContext *context));
#endif

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

#endif	/* Prevent multiple inclusions */

#ifdef __cplusplus
}  /* Ensure Names are not mangled by C++ compilers */
#endif
