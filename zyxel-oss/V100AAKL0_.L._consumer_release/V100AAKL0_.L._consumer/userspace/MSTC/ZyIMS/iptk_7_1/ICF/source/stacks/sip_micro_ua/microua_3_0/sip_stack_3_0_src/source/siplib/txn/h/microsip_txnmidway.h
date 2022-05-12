/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/**************************************************************************
** FUNCTION:
** This has prototypes for creating Mid-Way Transactions 
**
***************************************************************************
**
** FILENAME:
**  txnmidway.h
**
** DESCRIPTION
**
**
**  DATE           NAME                       REFERENCE
** 18Feb2002  	sasidhar P V K			 			Initial
**
**
** COPYRIGHT , Aricent, 2006
***************************************************************************/

#ifndef TXN_MIDWAY_H
#define TXN_MIDWAY_H

#include "microsip_txnstruct.h"

/***************************************************************************
** FUNCTION:sip_txn_createMidwayTxn
** DESCRIPTION:The application will use this API to create a Txn by giving
**		 the SipMessage.This API does not  perform the check of whether
**		dependent Txn is present or not.This will immediately Create
**		 New Txn If there is Nothing. 
**	PARAMETERS:
**		*pMessage 		(IN) : SipMessage of which midway txn to be created
**		*pAddr    		(IN) : The siptranspAddr
**		dTranspType 	(IN) : The transpType
**		*pTxnContext 	(IN) : The txn context
**		dAPICalled 		(IN) : txn msg sent/reveieved 
**		**ppTxnKey	   (OUT) : The Siptxn key
**		*pError		 (IN/OUT): The sip error
******************************************************************************/
SipBool sip_txn_createMidwayTxn(SipMessage *pMessage, SipTranspAddr *pAddr,\
	SIP_S8bit dTranspType, SipTxnContext *pTxnContext,\
	en_SipTxnAPICalled dAPICalled, SipTxnKey **ppTxnKey, SipError *pError);

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
