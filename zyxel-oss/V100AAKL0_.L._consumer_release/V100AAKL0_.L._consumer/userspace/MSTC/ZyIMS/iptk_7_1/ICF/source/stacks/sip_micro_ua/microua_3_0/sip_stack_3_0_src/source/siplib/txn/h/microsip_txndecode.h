
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/**************************************************************************
** FUNCTION:
**  This file contains the prototypes of the primary decode entry point
**
***************************************************************************
**
** FILENAME:
**  sipdecode.h
**
** DESCRIPTION
**
**
**  DATE           NAME                       REFERENCE
** 8Feb2002  	R.Kamath			 			Initial
**
**
** COPYRIGHT , Aricent, 2006
***************************************************************************/

#ifndef __TXNDECODE_H_
#define __TXNDECODE_H_

#include "microsip_common.h"
#include "microsip_decode.h"
#include "microsip_encode.h"
#include "microsip_sendmessage.h"
#include "microsip_decodeinternal.h"
#include "microsip_txnstruct.h"

/*****************************************************************************
**** FUNCTION:	sip_txn_decodeMessage
****
****
**** DESCRIPTION:This API provides the primary interface to the transaction
**** layer to the application. The application will use this API to decode
**** incoming SIP messages and identify the corresponding transactions
****
**** Parameters:
****			pMsgBuffer(IN/OUT): This is the message buffer that needs to be
****								decoded.
****			ppDecodedMessage(OUT): This is the decoded SipMessage.
****
****			pContext(IN): This is the TxnContext that provides informn abt
****							about the decode options that will be passed to
****							to sip_decodeMessage, the Event Context
****			dMessageLength(IN): This is the length of the message that is 
****							needs to be decoded.
****
****			ppNextMessage(OUT): This is the next message, it is the value
****							that is returned by sip_decodeMessage.
****
****			pDecodeResult(OUT): This is the result of the txn_decode and
****							informs the invoker whether the messagejust 
****							decoded was a retransX,stray.
****			ppTxnKey(OUT): In case the invocation of this API results in a 
****							txn being created then the txn-key of the created
****							txn is returned in this parameter.
****
**** Return Values:
****			SipSuccess
****			SipFail
****
***************************************************************************/
extern SipBool sip_txn_decodeMessage _ARGS_( (SIP_S8bit *pMsgBuffer,\
			SipMessage **ppDecodedMessage , \
			SipTxnContext *pContext, SIP_U32bit dMessageLength, \
			SIP_S8bit **ppNextMessage, en_SipTxnDecodeResult *pDecodeResult,\
			SipTxnKey **ppTxnKey,SipError *pError));



/*****************************************************************************
**** FUNCTION:	sip_txn_handleTxn
****
****
**** DESCRIPTION:An application might not always use the
**** sip_txn_decodeMessage() in order to decode a message. It might instead 
**** use the API sip_decodeMessage() to decode the message , thus avoiding 
**** creation of a txn for handling this message. After decode, it might 
**** however discover that it needs to handle this message within a 
**** transaction. At this point it invokes the API sip_txn_handleTxn() 
**** requesting the transaction layer to handle further processing of this 
**** transaction.
****
**** Parameters:
****			pMessage(IN): This is the decoded SipMessage that the appln
****                        wants to be handled in a txn.
****
****			ppDecodedMessage(OUT): This is the decoded SipMessage.
****
****			pContext(IN): This is the TxnContext. 
****
****			pDecodeResult(OUT): This is the result of the txn_decode and
****							informs the invoker whether the messagejust 
****							decoded was a retransX,stray.
****			ppTxnKey(OUT): In case the invocation of this API results in a 
****							txn being created then the txn-key of the created
****							txn is returned in this parameter.
****			pError(OUT):	This is the error variable.
****
**** Return Values:
****			SipSuccess
****			SipFail
****
***************************************************************************/
extern SipBool sip_txn_handleTxn _ARGS_( (SipMessage *pMessage,\
			SipTxnContext *pContext,\
			en_SipTxnDecodeResult *pDecodeResult,\
			SipTxnKey **ppTxnKey,SipError *pError));


/*****************************************************************************
**** FUNCTION:	sip_txn_sendMessage
****
****
**** DESCRIPTION:This API will be used by the application to send a message
**** over the network. It has the same semantics as the Stack API
**** sip_sendMessage apart from the fact that this API  also updates/creates
**** txn-elements apart from sending the message on to the network. All
**** retransmission related requirements are satisfied by this API as well. 
**** It accepts the SipTxnContext variable that contains user-specific values 
**** for various timers.
****
**** Parameters:
****			pMessage(IN): This is the SIP Message buffer that needs to be
****						sent.
****
****			pAddr(IN): The transport address to be used while sending the
****						message.
****
****			dTranspType(IN):The type of transport to be used(TCP/UDP)
****
****			pContext(IN): This is the TxnContext that provides informn abt
****							about the decode options that will be passed to
****							to sip_decodeMessage, the Event Context
****
****			dBypassEnable(IN):This informs the fn whether the user wants 
****							to bypass all txn-layer related functionality.
****
****			ppTxnKey(OUT): In case the invocation of this API results in a 
****							txn being created then the txn-key of the created
****							txn is returned in this parameter.
****
****			pError(OUT): The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
***************************************************************************/
extern SipBool sip_txn_sendMessage _ARGS_( (SipMessage *pMessage, \
	SipTranspAddr *pAddr,SIP_S8bit dTranspType, SipTxnContext *pTxnContext, \
	en_SipTxnBypass dBypassEnable, SipTxnKey **ppTxnKey,SipError *pError));





/***************************************************************************
**** FUNCTION:sip_txn_cancelTxn
****
****
**** DESCRIPTION:This API is needed by proxies only. When a proxy decides
**** that it needs to handle any txn statelessly then it uses this API to 
**** inform the Txn Layer. Any request retransmissions that have been queued 
**** up for this txn are returned back to the proxy application. The txn 
**** itself is set to the bypass mode wherein this txn does not do any 
**** interpretation of messages it handles and just forwards them.
****
****
**** Parameters:
****
****		pTxnKey(IN):The key that identifies the Txn that needs to be 
****		cancelled.	
****
****		ppList(OUT):The list of the queued up buffers.
****
****		pError(OUT):Error variable
******************************************************************************/
extern SipBool sip_txn_cancelTxn _ARGS_( ( SipTxnKey *pTxnKey,\
										SipList **ppList, \
										SipError *pError));




/***************************************************************************
**** FUNCTION:sip_txn_abortTxn
****
****
**** DESCRIPTION:An application might decide not to have a transaction that
**** it had created earlier. It might have initially used the 
**** transaction-layer APIs to create a transaction but decided later to 
**** handle further messages belonging to this transaction on its own. In 
**** order to do so, it must first delete the transaction that had been 
**** created earlier. This API facilitates the removal of a transaction from 
**** the aggregation of transaction elements. This API is needed to instruct 
**** the transaction layer to stop the timers for the transaction.
****
****
**** Parameters:
****
****		pTxnKey(IN):The key that identifies the Txn that needs to be 
****		aborted.	
****
****		pTxnContext(IN):The TxnContext being passed. The only field that
****		is actually used however is the context data alone.
****
****		pError(OUT):Error variable
******************************************************************************/
extern SipBool sip_txn_abortTxn _ARGS_( ( SipTxnKey *pTxnKey,\
				SipTxnContext	*pTxnContext, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_txn_compareTxnKeys
****
****
**** DESCRIPTION:This API will be used by the application to compare two
**** SipTxnKeys. The application needs to maintain Txn-Elements indexed on 
**** the SipTxnKey. It needs to return these TxnElements from its internal 
**** aggregation when the Txn Layer asks for one. The application can use 
**** this function to compare the TxnKeys for this purpose. The comparison 
**** of the TxnKeys depends on the dTagCheck boolean in the TxnKey. If this 
**** Boolean is set false in either one of the TxnKeys, the ToTag will not 
**** be included in the comparison. Otherwise, the function will include 
**** the ToTag in the comparision.  
****
****
**** Parameters:
****
****		pStoredKey(IN):The reference to the key that s present in the 
****						applicns aggregation.
****
****		pGeneratedKey(IN):The reference to the key whose match the applicn
****						is trying to find.
****
****
****		pError(OUT):Error variable
******************************************************************************/
SipBool sip_txn_compareTxnKeys
        (SipTxnKey *pStoredKey, SipTxnKey *pGeneratedKey, SipError *pErr);



/***************************************************************************
**** FUNCTION:sip_txn_createTxnKey
****
****
**** DESCRIPTION:This API will be used by the application to create a
**** SipTxnKey from a decoded Sip Message. The application might need to 
**** identify a particular Txn and perform operations on it after receipt 
**** and decoding of a SIP message. This API will then be used to obtain 
**** the SipTxnKey through which to identify this txn.
****
**** Parameters:
****
****		pMessage(IN):The reference to the  message whose key the 
****                     application needs.
****
****		dTxnClass(IN):When a key is created apart from the fields that are 
****					  picked up from the SipMessage, there is another field 
****					  which is needed, this is the TxnClass which informs whether
****					  the key is going to be of InviteServer,InviteClient,NInviteSrv,
****					  NonInvClient etc.	
****		
****		ppKey(OUT): The txn-key created corresponding to the message that
****					has been passed.
****
****
****		pError(OUT):Error variable
******************************************************************************/
SipBool sip_txn_createTxnKey
        (SipMessage *pMessage, en_SipTxnClass		dTxnClass,\
			SipTxnKey **ppKey, SipError *pErr);
	

/***************************************************************************
**** FUNCTION:sip_txn_freeTxnKey
****
****
**** DESCRIPTION:This API will be used by the application to free a
**** SipTxnKey. Whenevr a txn is created the appln is returned a reference
**** to the resulting SipTxnKey that is added to the aggregation. The
**** appln then needs to call a free on that key at least once. This API 
**** achieves that.
****
**** Parameters:
****
****		pKey(IN): The txn-key reference that needs to be freed.
****
****		pError(OUT):Error Variable
******************************************************************************/
SipBool sip_txn_freeTxnKey
        (SipTxnKey *pKey, SipError *pErr);


/***************************************************************************
**** FUNCTION:sip_txn_initSipTxnContext
****
****
**** DESCRIPTION:This API will be used by the application to perform
**** an init operation on a SipTxnContext pointer variable.	The txnContext
**** structure contains many internal elements which need to be initialised
**** before the structure can be passed into the Txn APIs. This API 
**** initialises all the variables in the TxnContext element. 
****
**** Parameters:
****
****		pKey(IN): The txn-key reference that needs to be freed.
****
****		pError(OUT):Error Variable
******************************************************************************/
SipBool sip_txn_initSipTxnContext
        (SipTxnContext **ppTxnContext, SipError *pErr);

		
/****************************************************************************
**** FUNCTION: sip_indicateTimeout 
****
**** DESCRIPTION:
****		Callback function to be implemented by the application.
****		Invoked if a message that was sent through UDP with 
****		retransmissions did not receive a matching response
****		after the maximum retransmissions have been sent.
****
**** Parameters:
****		context(IN):
****			The event context structure that was passed to the 
****			sip_sendMessage invocation that resulted in this
****			callback.
****
****
****		dTimer(IN):Provides information to the application that 
****				the firing of which timer caused the timeOut	
****
****
****************************************************************************/
extern void sip_indicateTimeOut _ARGS_ ((SipEventContext *context, \
										en_SipTimerType dTimer));

/****************************************************************************
**** FUNCTION: sip_indicateMessageRetransmission 
****
**** DESCRIPTION:
****		Callback function to be implemented by the application.
****		Invoked every time a message was retransmitted.
****
**** Parameters:
****		pEventContext(IN):
****			The event context structure that was passed to the 
****			sip_sendMessage invocation that resulted in this
****			callback.
****
****		pTxnKey(IN):
****			The Txn Key of the message that was retransmitted.	
****
****		pBuffer(IN):
****			The message buffer that was retransmitted.	
****
****		dBufferLength(IN):
****			The length of the message buffer that was retransmitted.	
****
****		pTranspAddr(IN):
****			This will contain the Transport address of the destination	
****			host to which message was retransmitted.
****
****		dRetransCount(IN):
****			This gives the number of retransmissions happened,including the
****			current retransmission
****
****		dDuration(IN):
****			This gives the time elapsed since the message
****            was last retransmitted and this retransmission.
****			The value is in milli-seconds
****
****************************************************************************/

extern void sip_indicateMessageRetransmission _ARGS_((SipEventContext \
			*pEventContext, SipTxnKey *pTxnKey,\
			SIP_S8bit *pBuffer,SIP_U32bit dBufferLength, \
			SipTranspAddr *pTranspAddr,SIP_U8bit dRetransCount,\
			SIP_U32bit dDuration));

/***************************************************************************
**** FUNCTION:sip_txn_resetTimerInTxn
****
****
**** DESCRIPTION:
**** This function will reset the timer for the transaction whose key is
***** passed.
**** Parameters :-
****	1. Txn Key (IN) :- 
****              Txn Key of the Txn whose timer has to be restarted
****
****    2. Txn Context (IN) :- 
****              Txn Context of Txn whose timer has to be restarted
****
****    3. New Timer value (IN) :-
****               Value of timer which is to be reset
****
****    4. Timer Type which will be given back (OUT) :- 
****                Type of timer which was stopped
****
****    5. Error (IN/OUT) :- 
****               error if any will be given back by this parameter
****
**** Return Value :-
****   SipSuccess :- if the timer is reset successfully
****
****   SipFail    :- if the timer can not be reset properly and error is
****                 returned in pError parameter
******************************************************************************/
SipBool sip_txn_resetTimerInTxn _ARGS_ ( ( SipTxnKey *pTxnKey,
	SipTxnContext	*pTxnContext, SIP_U32bit newTimer,
	en_SipTimerType *pTimerType, SipError *pError) ) ;


#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
