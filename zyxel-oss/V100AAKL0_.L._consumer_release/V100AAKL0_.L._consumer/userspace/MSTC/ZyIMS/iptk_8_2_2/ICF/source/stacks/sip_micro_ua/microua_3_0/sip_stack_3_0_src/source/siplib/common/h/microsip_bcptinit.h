
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/*******************************************************************************
 ** FUNCTION:
 **            Init Function prototypes for bcpt structures
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** microsip_bcptinit.h
 **
 ** DESCRIPTION:
 **
 ** DATE			NAME                    REFERENCE          REASON
 ** ----			----                    ---------         --------
 ** 15/02/2002	Aparna Kuppachi					-			   Initial					
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef _MICROSIP_BCPTINIT_H_
#define _MICROSIP_BCPTINIT_H_

#include <stdlib.h>
#include "microsip_struct.h"
#include "microsip_common.h"
#ifdef SIP_ISUP
extern SipBool sip_initIsupMessage _ARGS_((IsupMessage **m,SipError *err));
#define sip_bcpt_initIsupMessage sip_initIsupMessage
#endif
#ifdef SIP_MIME_PARSING
extern SipBool sip_initMimeMessage _ARGS_((MimeMessage **m,SipError *err));
#define sip_bcpt_initMimeMessage  sip_initMimeMessage
extern SipBool sip_initSipMimeHeader _ARGS_((SipMimeHeader **h,SipError *err));
#define sip_bcpt_initSipMimeHeader  sip_initSipMimeHeader
#endif
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

