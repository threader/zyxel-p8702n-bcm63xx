/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 ** FUNCTION:
 **             Has Init Function prototypes For all IMPP Structures

 *********************************************************************
 **
 ** FILENAME:
 ** 		imppinit.h
 **
 ** DESCRIPTION:
 ** 		This file contains code to init all IMPP structures
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 ** 16Apr01   Subhash Nayak U.			Initial Creation
 **
 **
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __IMURLINIT_H__
#define __IMURLINIT_H__
#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_imurlstruct.h"

extern SipBool sip_initImUrl _ARGS_((ImUrl **ppIm,SipError *pErr));

#ifdef SIP_PRES
#define sip_initPresUrl                      sip_initImUrl
#endif


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
