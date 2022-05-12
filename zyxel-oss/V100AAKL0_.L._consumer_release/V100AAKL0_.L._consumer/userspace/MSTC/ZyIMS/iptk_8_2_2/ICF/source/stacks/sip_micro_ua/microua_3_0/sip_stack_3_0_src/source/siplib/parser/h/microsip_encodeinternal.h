/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 ** FUNCTION: 	 This file contains prototypes for some internal functions
 **				 used in message formation.
 **
 ****************************************************************************
 **
 ** FILENAME: 		microsip_encodeinternal.h
 **
 ** DESCRIPTION:	
 **
 **
 ** 	DATE	 	NAME			REFERENCE	 		REASON
 ** 	----	 	----			---------	 		--------
 ** 27-Feb-2002		Siddharth		sipformmesage.h		Initial
 **					Toshniwal		of core SIP Stack
 **
 ***************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ***************************************************************************/

#ifndef __MUSIP_ENCODEINTERNAL_H__
#define __MUSIP_ENCODEINTERNAL_H__

#include "microsip_common.h"
#include "microsip_struct.h"

#define CRLF "\r\n"



void STRCAT_AND_SPACE _ARGS_((SIP_S8bit *a, SIP_S8bit *b));
void STR_CAT _ARGS_((SIP_S8bit *a, SIP_S8bit *b));
void STRCPY _ARGS_((SIP_S8bit *a, SIP_S8bit *b));

SipBool VALIDATE_HEADER _ARGS_((SIP_Pvoid pHeader, SIP_U32bit dIndex, \
	SipError *pErr));

void sip_formHeaderName _ARGS_((/* CSR_1-6613911 Change Start */SIP_S8bit *pEndBuff, /* CSR_1-6613911 Change End */en_AdditionMode dMode, \
	en_HeaderForm dFullOrShortForm, SIP_S8bit *pOut, \
	SIP_S8bit *pFullName, SIP_S8bit *pShortname));

SipBool __sip_setHeaderCountInHeaderLine _ARGS_((SipMessage *msg, \
	SIP_U32bit line,SIP_U32bit	count,SipError *err));

SipBool __checkHeaderTypeHop _ARGS_((en_HeaderType dType));
SipBool glbSipParserMemStrStr _ARGS_((SIP_S8bit *pBuffer,SIP_U32bit buflen, \
	SIP_S8bit *separator, SIP_U32bit *offset));

/* CSR_1-6613911 Change Start */
SipBool __sip_check _ARGS_((SIP_S8bit *e,SIP_S8bit *a, SIP_S8bit *b));
/* CSR_1-6613911 Change End */


#endif	/* Prevent multiple inclusions */

#ifdef __cplusplus
}  /* Ensure Names are not mangled by C++ compilers */
#endif
