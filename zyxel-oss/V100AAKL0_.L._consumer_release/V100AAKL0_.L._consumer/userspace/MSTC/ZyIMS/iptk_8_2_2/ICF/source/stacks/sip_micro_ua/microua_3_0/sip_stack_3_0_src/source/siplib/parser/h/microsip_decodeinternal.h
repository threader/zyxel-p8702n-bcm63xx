/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 ** FUNCTION: 		Functions for use at the time of decode only.
 **
 *****************************************************************************
 **
 ** FILENAME: 		microsip_decodeinternal.h
 **
 ** DESCRIPTION:	This file contains function prototypes for the 
 **					routines invoked from the parsers for specific tasks
 **					such as determinging header type from header name etc.
 **					The functions of these files are internal to the
 **					stack and not exposed to the application developer.
 **
 ** DATE        	NAME			REFERENCE		REASON
 ** ----        	----			---------		--------
 ** 24-Feb-2002  	Siddharth		-				Initial Creation
 **					Toshniwal
 **
 *****************************************************************************
 ** 			COPYRIGHT , Aricent, 2006
 *****************************************************************************/ 

#ifndef __MICROSIP_DECODEINTERNAL_H__
#define __MICROSIP_DECODEINTERNAL_H__

#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_free.h"
#include "microsip_init.h"

/*****************************************************************************
 ** FUNCTION: 		sip_getHeaderTypeFromString 
 **
 ** DESCRIPTION:	This function returns the header type from the string
 ** PARAMETERS:
 **		pStart(IN): 	The input String containing the name of the header
 **					return	value -  The header type that is returned
 **	
 **		pStart(IN):The start Index
 **		pEnd(IN):  The end Index	
 **
 **		ReturnValue:
 **		en_headerform(OUT):The Header form(Short Form /Full Form)
 **		
 ****************************************************************************/

en_HeaderType sip_getHeaderTypeFromString(SIP_S8bit* pStart , \
	SIP_S8bit *pEnd,en_HeaderForm *form);

 /*****************************************************************************
 ** FUNCTION: 		sip_checkMandatory 
 **
 ** DESCRIPTION:	This function checks to see if message has mandatory  
 ** mandatory headers.
 ** Currently checks for Call-Id, CSeq, From ant To headers so that the timer
 ** module that uses these doesnt throw up. Does not look for all the
 ** mandatory headers which differ depending on the message dType.
 ** PARAMETERS:
 **		pSipMessage(IN): The sipMessage to be checked
 **		Return Value:
 **				SipBool
 **
 ****************************************************************************/
extern SipBool sip_checkMandatory _ARGS_ ((SipMessage *s));


/*****************************************************************************
 ** FUNCTION: 		sip_checkEachhdrType
 **
 ** DESCRIPTION:	This function checks whether a particular header is allowed
 **					in a SipMessage
 ** PARAMETERS:
 **		pSipMessage(IN): The sipMessage to be checked
 **		dType     (OUT): The sip header type
 **		pError (IN/OUT): The SipError
 **
 **		Return Values
 **				SipBool
 **
 **		
 ****************************************************************************/
SipBool sip_checkEachHdrType(SipMessage *pSipMessage,en_HeaderType dType,\
	SipError *pError);
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}

#endif

