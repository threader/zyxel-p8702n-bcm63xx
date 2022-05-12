/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
 ** FUNCTION:
 **             This file has all the SIP Txn Related init functions

 ************************************************************************
 **
 ** FILENAME:
 ** siptxninit.h
 **
 ** DESCRIPTION:
 **
 ** DATE                NAME                    REFERENCE               REASON
 ** ----                ----                    --------                ------
 ** 7-Feb-2002			Sasidhar PVK
 **     COPYRIGHT , Aricent, 2006
 *************************************************************************/


#ifndef __SIPTXNINIT_H__
#define __SIPTXNINIT_H__
#include "microsip_struct.h"
/***********************************************************************
**FUNCTION: sip_initSipTxnKey
**
**DESCRIPTION:This function does an init on the received txn key
**
**PARAMETERS:ppTxnKey(OUT): The address to the TxnKey pointer variable
**						which will be allocated memory.
**			pErr(OUT):Error variable.
**
***********************************************************************/

SipBool sip_initSipTxnKey(SipTxnKey** ppTxnKey,SipError *pErr);


/***********************************************************************
**FUNCTION: sip_initSipTxnTimeoutData
**
**DESCRIPTION:This function does an init on the variable ppTxnTimeoutData
**
**PARAMETERS:ppTxnTimeoutData(OUT):The parameter to which memory is allocated.
**				pError(OUT):Error variable.
**
***********************************************************************/
SipBool sip_initSipTxnTimeoutData(SipTxnTimeoutData** ppTxnTimeoutData,SipTxnKey *pTxnKey,\
	SipError *pError);

/***********************************************************************
**FUNCTION: sip_initSipTxnBuffer
**
**DESCRIPTION:This function does an init on the TxnBuffer variable.
**
**PARAMETERS: ppTxnBuffer(OUT): The variable to which memory is allocated
**		      pError(OUT)     : The eror variable.
**
**
***********************************************************************/
SipBool sip_initSipTxnBuffer(SipTxnBuffer** ppTxnBuffer,SipError *pError);

#endif

#ifdef __cplusplus
}
#endif

