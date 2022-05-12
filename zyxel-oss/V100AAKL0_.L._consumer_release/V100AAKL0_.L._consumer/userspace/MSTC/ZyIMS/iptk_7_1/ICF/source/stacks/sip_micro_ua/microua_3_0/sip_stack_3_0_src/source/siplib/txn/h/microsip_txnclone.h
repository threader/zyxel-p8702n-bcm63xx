#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************
 ** FUNCTION:
 **             This file has all the SIP Txn Related Cloning Functions

 ************************************************************************
 **
 ** FILENAME:
 ** txnclone.h
 **
 ** DESCRIPTION:
 **
 ** DATE                NAME                    REFERENCE               REASON
 ** ----                ----                    --------                ------
 ** 11-Feb				Sasidhar PVK			
 **     COPYRIGHT , Aricent, 2006
 *************************************************************************/


#ifndef __SIPTXNCLONE_H__
#define __SIPTXNCLONE_H__

#include "microsip_struct.h"
/***********************************************************************
**FUNCTION: sip_cloneSipTxnKey
**
**DESCRIPTION: This function achives the cloning of Txn keys
**
**PARAMETERS: pDestTxnKey(OUT): The txn key into which values are going to be 
**				copied.
**			  pSrcTxnKey(IN): The txn key from which the values are to
**             be copied.	
**			  pError:The error variable.	
**
***********************************************************************/
SipBool sip_cloneSipTxnKey(SipTxnKey *pDestTxnKey,SipTxnKey *pSrcTxnKey,\
	SipError *pError);

#ifdef SIP_RPR
/***********************************************************************
**FUNCTION: sip_txn_rprcloneSipRAckHeader
**
**DESCRIPTION: This function achieves the cloning of the RACk header.
**
**
**PARAMETERS:
**				pDest(OUT): The destination RACk header into which the values 
**				are going to be copied.
**				pSource(IN): The source RAck header from which the values
**				will be taken.
**				pError(OUT):The eror variable.
**
***********************************************************************/
SipBool sip_txn_rprcloneSipRAckHeader
	(SipRackHeader *pDest, SipRackHeader *pSource, SipError *pError);

#endif

#endif
#ifdef __cplusplus
}
#endif
