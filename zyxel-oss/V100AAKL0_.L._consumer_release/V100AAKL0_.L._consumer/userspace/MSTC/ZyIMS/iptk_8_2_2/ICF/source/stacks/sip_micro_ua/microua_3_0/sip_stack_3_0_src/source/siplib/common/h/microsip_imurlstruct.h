
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **	 	This file has all the IMURL related structures

 ******************************************************************************
 **
 ** FILENAME:
 ** 		imppstruct.h
 **
 ** DESCRIPTION:
 **	 
 **
 ** DATE		NAME				REFERENCE	REASON
 ** ----		----				--------	------
 ** 15/08/2002	Mahesh				Corestack  initial
 **
 **
 **	COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __IMURLSTRUCT_H__
#define __IMURLSTRUCT_H__
#include "microsip_common.h"
#include "microsip_struct.h"


typedef struct 
{
	SIP_S8bit*		pDispName; 	/* phrase go here */
	SipList			slRoute;   	/* route go here */
	SIP_S8bit* 		pUser;		/* local-part go here */
	SIP_S8bit*		pHost;   	/* domain go here */
	SipList			slParams;   /* header params go here*/
	SIP_RefCount    dRefCount; 
} ImUrl;

#ifdef SIP_PRES
typedef ImUrl PresUrl;
#endif



#endif
#ifdef __cplusplus
}
#endif 

