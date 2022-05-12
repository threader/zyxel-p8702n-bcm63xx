
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/**************************************************************************
** FUNCTION:
**  This file contains the prototypes of the send message APIs
**
***************************************************************************
**
** FILENAME:
**  microsip_sendmessage.h
**
** DESCRIPTION
**
**
 ** DATE		NAME			REFERENCE		REASON
 ** ----		----			---------	    --------
 **22-03-2002	Mahesh			CoreStack			-
**
** COPYRIGHT , Aricent, 2006
***************************************************************************/


#include "microsip_common.h"
#include "microsip_struct.h"

#ifndef __MICROSIP_SENDMESSAGE_H_
#define __MICROSIP_SENDMESSAGE_H_

/*****************************************************************
** Function: sip_sendMessage
** Description: Sends a SIP message.
** Parameters:
**		s(IN): The message structure that is to be converted to
**			text and sent.
**		options(IN): The options to be used for forming the text
**			message. Multiple options can be ORed. The supported 
**			options are:
**			SIP_OPT_SINGLE: Each header is formed in a separate line.
**				The message will not contain comma separated headers.	
**				Cannot be used with SIP_OPT_COMMASEPARATED.
**			SIP_OPT_COMMASEPARATED: Multiple instances of the same
**				kind of header will appear on the same header line
**				separated by commas.
**				Cannot be used with SIP_OPT_SINGLE.
**			SIP_OPT_SHORTFORM: Single-letter short forms will be used
**				for headers	that have short names.
**				Cannot be used with SIP_OPT_SULLFORM.
**			SIP_OPT_FULLFORM: All hedernames will appear in full.
**				Cannot be used with SIP_OPT_SHORTFORM.
**			SIP_OPT_CLEN: A Content-Length header with the correct
**				length is inserted in the message being formed. If
**				the message structure has a Content-Length header,
**				the function corrects the length field depending on
**				size of the message body.
**			SIP_OPT_REORDERHOP: All hop to hop headers will be placed
**				above the end to end headers.
**			SIP_OPT_AUTHCANONICAL: All headers after the Authorization
**				header will be formed in the canonical form. This
**				will override options SIP_OPT_SHORTFORM and
**				SIP_OPT_COMMASEPARATED from the Authorization header
**				onwards.
**		addr(IN): A preallocated buffer that will contain the formed
**			text message.
**		transptype(IN): The transport to be used for transmitting the 
**			SIP text message. Possible values are
**			SIP_UDP: UDP retransmission mechanisms are used for the
**				message unless overridden with SIP_NORETRANS.
**				This option will be passed on to the sip_sendToNetwork
**				callback in the application.
**			SIP_TCP: TCP retransmission is used for responses to INVITE.
**				This option will be passed on to the sip_sendToNetwork
**				callback in the application.
**			SIP_NORETRANS: This option when ORed with SIP_UDP or SIP_TCP
**				disables the stack retransmission mechanism for the 
**				message.
**		context(IN): This application may pass any information in this
**			structure though its pData (void pointer) element. This
**			structure is passed to the application in the Time-out 
**			indicate function. This function is called in the event that
**			the message is sent by UDP with retransmissions and the 
**			maximum retransmissions are reached.
**		err(OUT): The error code is returned in this of the function 
**			fails.
**
*******************************************************************/

extern SipBool sip_sendMessage _ARGS_ (( SipMessage *s, \
	SipOptions *options, SipTranspAddr *addr, \
		SIP_S8bit transptype, SipEventContext *context, SipError *err));

/*****************************************************************
** Function: sip_sendToNetwork
** Description: A calback invoked on the application by the stack
**		when a message is to be sent to the network. A call to 
**		sip_sendMessage results in this callback if the message is
**		formed successfully.
** Parameters:
**		buffer(IN): The buffer containing the message that is to be
**			sent across the network.
**		buflen(IN): The length of the message to be sent.
**		addr(IN): The transport address to which the message is to be
**			sent. This will be the same address that is passed to 
**			sip_sendMessage.
**		transptype(IN): The transport type to be used. This is the same
**			parameter that is passed to the sip_sendMessage function.
**		context(IN): Context information that was passed to the 
**			sip_sendMessage call that resulted in this callback.
**		err(OUT): The error code in case of an error.
** Return Values:
**		The application return a SipFail if it cannot send the message
**		across. The stack returns the Value and the error code to 
**		the application in the sip_endMessage call. The retransmission
**		mechanism is not used if the initial send fails.
**		The application returns SipSucess if it succeeds in the network
**		send. The stack will start retransmission mechanisms if required.
**
*******************************************************************/
extern SipBool sip_sendToNetwork _ARGS_ (( SIP_S8bit *buffer, \
	SIP_U32bit buflen,SipTranspAddr *addr, SIP_S8bit transptype, \
		SipError *err));

/*****************************************************************
** Function: sip_formMessage
** Description: Forms a text message from a message structure.
** Parameters:
**		s(IN): The message structure that is to be converted to
**			text.
**		options(IN): The options to be used for forming the text
**			message. Multiple options can be ORed. The supported 
**			options are:
**			SIP_OPT_SINGLE: Each header is formed in a separate line.
**				The message will not contain comma separated headers.	
**				Cannot be used with SIP_OPT_COMMASEPARATED.
**			SIP_OPT_COMMASEPARATED: Multiple instances of the same
**				kind of header will appear on the same header line
**				separated by commas.
**				Cannot be used with SIP_OPT_SINGLE.
**			SIP_OPT_SHORTFORM: Single-letter short forms will be used
**				for headers	that have short names.
**				Cannot be used with SIP_OPT_SULLFORM.
**			SIP_OPT_FULLFORM: All hedernames will appear in full.
**				Cannot be used with SIP_OPT_SHORTFORM.
**			SIP_OPT_CLEN: A Content-Length header with the correct
**				length is inserted in the message being formed. If
**				the message structure has a Content-Length header,
**				the function corrects the length field depending on
**				size of the message body.
**			SIP_OPT_REORDERHOP: All hop to hop headers will be placed
**				above the end to end headers.
**			SIP_OPT_AUTHCANONICAL: All headers after the Authorization
**				header will be formed in the canonical form. This
**				will override options SIP_OPT_SHORTFORM and
**				SIP_OPT_COMMASEPARATED from the Authorization header
**				onwards.
**		out(OUT): A preallocated buffer that will contain the formed
**			text message.
**		length(OUT): This will contain the length of the message formed.
**		err(OUT): The error code is returned in this if the function 
**			fails.
**
*******************************************************************/

extern SipBool sip_formMessage _ARGS_ (( SipMessage *s, \
	SipOptions *options, SIP_S8bit *out , \
		SIP_U32bit *length, SipError *err));

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

