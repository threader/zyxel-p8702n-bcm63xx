
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/******************************************************************************
 ** FUNCTION:
 **             Free function prototypes for bcpt structures
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** microsip_bcptfree.h
 **
 ** DESCRIPTION: This file contains code to free all bcpt structures
 **
 ** DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 ** 15/02/02   Aparna Kuppachi				-              Initial Creation
 **
 **     COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#ifndef __MICROSIP_BCPTFREE_H_
#define __MICROSIP_BCPTFREE_H_

#include <stdlib.h>
#include "microsip_struct.h"
#include "microsip_common.h"

#ifdef SIP_ISUP
extern void sip_bcpt_freeIsupMessage _ARGS_((IsupMessage * m));
#endif
#if 0
extern void sip_bcpt_freeSipMimeVersionHeader _ARGS_\
	((SipMimeVersionHeader * h));
#endif	
#ifdef SIP_MIME_PARSING	
extern void sip_bcpt_freeMimeMessage _ARGS_((MimeMessage * m));
extern void sip_bcpt_freeSipMimeHeader _ARGS_((SipMimeHeader * h));
extern void __sip_bcpt_freeMimeMessage _ARGS_((SIP_Pvoid  m));
#endif
#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

