/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **	 	This file has all the SIP Instant Messaging and Presence Related 
**              APIS for freing structures

 ******************************************************************************
 **
 ** FILENAME:
 ** 		imppfree.h
 **
 ** DESCRIPTION:
 **	 
 **
 ** DATE			NAME				REFERENCE	REASON
 ** ----			----				--------	------
 ** 16/4/2001		Subhash Nayak U.	Original
 **
 **
 **	COPYRIGHT , Aricent, 2006
 ******************************************************************************/
#ifndef _IMPPFREE_H_
#define _IMPPFREE_H_
#include "microsip_imurlstruct.h"
extern void sip_freeImUrl  _ARGS_ ((ImUrl *pUrl));

#ifdef SIP_PRES
#define sip_freePresUrl                    sip_freeImUrl
#endif


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 


