
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 


/**************************************************************************
** FUNCTION:
**  This file contains the prototypes of all the internal functions used
**	by the txn layer module
**
***************************************************************************
**
** FILENAME:
**  txndecodeintrnl.h
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
#ifndef __TXNDECODEINTRNL_H_
#define __TXNDECODEINTRNL_H_

#include "microsip_common.h"
#include "microsip_txnstruct.h"
/*This is an array of function pointers indexed on the various states
that a Txn can be in, when the state of the txn changes these functions
are invoked , all timer related functionality gets (de)activated thru
these functions alone*/
extern SipBool (*glbSipTxnStateChange[TXNSTATES])(SipTxnBuffer *,SipTxnKey *, \
								SIP_Pvoid pContext,SipTxnInformn *,SipError *) ;

/***************************************************************************
**** FUNCTION:sip_getTxnClass
****
****
**** DESCRIPTION:Depending on whether a messsage was sent or recvd, this
****			function determines the Txn Class Inv-Server,In-Client etc
****			en_SipTxnClass is the OUT param in which this info is filled
**** Parameters:
****			pTxnInformn(IN): The function uses this parameter to decide on 
****			which txn class is going to get affected.
****
****			dAPICalled(IN): This parameter provides information about
****			whether the txn level API called was sip_txn_decode or
****			sip_txn_sendMessage.
****		
****			pTxnClass(OUT): This parameter informs the caller abt what is
****			the kind of txn that will get affected
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**********************************************************************/
extern SipBool sip_getTxnClass _ARGS_(( \
				SipTxnInformn *pTxnInformn, en_SipTxnAPICalled dAPICalled,\
				en_SipTxnClass *pTxnClass));


/***************************************************************************
**** FUNCTION:sip_checkMandatoryTxnHeaders
****
****
**** DESCRIPTION:Depending on whether a messsage is a request or a response
****			 this function checks for the presence of headers that are
****			 mandatory for the TxnLayer related handling.It return SipFail
****			 in case any madatory header is missing.
**** Parameters:
****			pMessage(IN): The message that we want to check for mandatory
****			headers.
****
****			dTxnType(IN): This parameter informs whether the txn type is of UA
****			or proxy.
****		
**** Return Values:
****			SipSuccess
****			SipFail
****
**********************************************************************/
extern SipBool sip_checkMandatoryTxnHeaders _ARGS_(( \
					SipMessage *pMessage, en_SipTxnType dTxnType));


/***************************************************************************
**** FUNCTION:sip_isTxnAnRPR
****
****
**** DESCRIPTION:Depending on the messsage that was sent or recvd, this
****			function determines the whether the Txn is an RPR or not.
**** Parameters:
****			pTxnInformn(IN): The function uses this parameter to decide on 
****			which txn class is going to get affected.
****
****
**** Return Values:
****			SipSuccess::If xn is an RPR
****			SipFail::If it is not
****
**********************************************************************/
extern SipBool sip_isTxnAnRPR _ARGS_(( \
				SipTxnInformn *pTxnInformn));
				
/***************************************************************************
**** FUNCTION:  sip_captureTxnInformn
****
****
**** DESCRIPTION:This API obtains all necessary information abt the 
****	sent/decoded message and fills up in a structure. This structure 
****	is passed
****	around , thus removing the necesity of passing different bits of informn
****	as separate parameters to functions.
**** Parameters:
****			pMessage(IN): The SIP Message structure which has actually
****			caused a change in the Txn state. This can be a message that
****			had been sent or one that had been received. 
****
****			pMsgBuffer(IN): This parameter is the Message Buffer 
****			corresponding to the SIP Message that is passed.
****		
****			dBufLen(IN):The length of the parameter pMsgBuffer
****		
****			pContext(IN):The Txn Context that had been passed to the core
****			Txn APIs sip_txn_decode OR sip_txn_sendMessage is passed to this
****			fn as is.
****		
****			pAddr(IN):The transport address.	
****		
****			dTranspType(IN):The transport type	
****		
****			pTxnInfo(OUT): This parameter gets poulated with information
****			that is derived from all other passed parameters
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**********************************************************************/
extern SipBool sip_captureTxnInformn _ARGS_(( SipMessage *pMessage,\
				SIP_S8bit *pMsgBuffer, SIP_U32bit dBufLen,\
				SipTxnContext *pContext, SipTranspAddr *pAddr,\
				SIP_S8bit	dTranspType,\
				SipTxnInformn *pTxnInfo));

/***************************************************************************
**** FUNCTION:sip_txn_sendBuffer
****
****
**** DESCRIPTION:This function is used by the Txn-Layer to actually
****			send the buffer across without making use of 
****			sip_sendMessage, it needs to increment the statistics
****			however.
**** Parameters:
****			pTxnInfo(IN): All information that is needed to actually
****			send a buffer to a destn is present in this parameter.
****
****			pError(OUT):Error variable
******************************************************************************/
extern SipBool sip_txn_sendBuffer  _ARGS_(( SipTxnInformn *pTxnInfo,\
		SipError *pError));

/***************************************************************************
**** FUNCTION:	sip_changeTxnStateAndRelease
****
****
**** DESCRIPTION:This function takes a SipTxnBuffer invokes a state change
****			operation on it and later releases the buffer using
****			sip_cbk_releaseTxn().	
**** Parameters:
****			pTxnBuffer(IN/OUT): This the TxnBuffer correpsonding to the 
****			txn that is going to undergo a state change.
****
****			pTxnKey(IN):The Txn key using which the TxnBuffer had initially
****			been fetched. This is needed later in the fn to invoke
****			sip_cbk_releaseTxn().
****
****			dTxnState(IN): The new state of the Txn.
****
****			pTxnInfo(IN): Generic information
****
****			pContextData(IN): User Data
****		
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**********************************************************************/
extern SipBool sip_changeTxnStateAndRelease _ARGS_(( SipTxnBuffer *pTxnBuffer,\
					SipTxnKey 	*pTxnKey, en_SipTxnState dTxnState,\
					SipTxnInformn *pTxnInfo,SIP_Pvoid pContextData,\
					SipError *pError));

/***************************************************************************
**** FUNCTION:	sip_getContextDatafromTxnInfo
****
****
**** DESCRIPTION:This function obtains the void data present in the 
**** 			eventcontext of the TxnInformation.
****
****
**** Parameters:
****			pTxnInfo(IN):This contains the new buffer that the SipTxnBuffer
****			needs to have a reference to.
****
****			ppData(OUT): The fetched data part of the eventcontext
****
**** Return Values:None
****
**************************************************************************/
extern void sip_getContextDatafromTxnInfo _ARGS_(( \
	SipTxnInformn *pTxnInfo, SIP_Pvoid *ppData));

/***************************************************************************
**** FUNCTION:	sip_changeTxnStateAndRelease
****
****
**** DESCRIPTION:A SipTxnBuffer contains a member for a SIP_S8bit *buffer.
****			This is the buffer that is retransmitted/remote-retransmitted
****			If a state change necesitates reassignment of this buffer
****			to another buffer, then this fn is called. It releases memory
****			if held by the previous buffer and assigns the new buffer passed
****			invokes a state change
****			operation on it and later releases the buffer using
****			sip_cbk_releaseTxn().	
****
**** Parameters:
****			pTxnBuffer(IN/OUT): This the TxnBuffer correpsonding to the 
****			txn that is going to undergo a state change.
****
****			pTxnInfo(IN):This contains the new buffer that the SipTxnBuffer
****			needs to have a reference to.
****
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_reassignbufferInTxnBuffer _ARGS_(( SipTxnBuffer *pTxnBuffer,\
					SipTxnInformn 	*pTxnInfo,SipError *pError));



/***************************************************************************
**** FUNCTION:sip_cloneNCreateNewTxn
****
****
**** DESCRIPTION:This function takes a txn buffer, clones it and then adds
****			it to the Txn aggregation by invoking sip_cbk_fetchTxn().
****
**** Parameters:
****			pTxnBuffer(IN): This is the TxnBuffer that is going to get 
****			cloned.
****
****			pTxnKey(IN):This is the key which will be used in creating
****			a new txn for th cloned SipTxnBuffer.
****
****			pTxnInfo(OUT):This variable gets updated with the key of the
****			newly created entry in the aggregation
****
****
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_cloneNCreateNewTxn _ARGS_(( SipTxnBuffer *pTxnBuffer,\
	SipTxnKey 	*pTxnKey,SipTxnInformn *pTxnInfo,SipError *pError));



/***************************************************************************
**** FUNCTION:sip_releaseTxnNFreeKey
****
****
**** DESCRIPTION:This function takes a txn buffer, invokes a sip_cbk_releaseTxn
****			on it and frees the incoming key.
**** Parameters:
****			pTxnBuffer(IN): This is the TxnBuffer that is going to get 
****			released.
****
****			pTxnKey(IN):This is the key which will be used in releasing
****			the txn.This key will get freed in this fn.
****
****			pTxnInfo(IN):Generic Information carried in this variable.
****
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_releaseTxnNFreeKey _ARGS_(( SipTxnBuffer *pTxnBuffer,\
					SipTxnKey 	*pTxnKey,SipTxnInformn *pTxnInfo,\
					SipError *pError));

/***************************************************************************
**** FUNCTION:sip_removeTxnNFreeKey
****
****
**** DESCRIPTION:This function takes a txn buffer, invokes a sip_cbk_releaseTxn
****			on it to remove the Txn and frees the incoming key.
**** Parameters:
****			pTxnBuffer(IN): This is the TxnBuffer that is going to get 
****			released.
****
****			pTxnKey(IN):This is the key which will be used in releasing
****			the txn.This key will get freed in this fn.
****
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_removeTxnNFreeKey _ARGS_(( SipTxnBuffer *pTxnBuffer,\
	SipTxnKey *pTxnKey,SipTxnInformn *pTxnInfo,SipError *pError));


/***************************************************************************
**** FUNCTION:sip_remoteRetransmitBuffer
****
****
**** DESCRIPTION:This function remote-retransmits the buffer contained
****	within the Txn Buffer to the destination specified in the Txn Buffer
**** Parameters:
****			pTxnBuffer(IN): This is the TxnBuffer that contains the buffer
****			that is to be retransmitted.
****
****			pTxnInfo(IN):This contains the TxnInfo.
****
****			pError(OUT): Error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_remoteRetransmitBuffer _ARGS_(( SipTxnBuffer *pTxnBuffer,\
					SipTxnInformn *pTxnInfo, SipError *pError));


/***************************************************************************
**** FUNCTION:sip_fetchTxnUsingMessage
****
****
**** DESCRIPTION:This function is used to fetch a txn corresponding to a 
****			SIPMessage.The dOpt flag indicates whether a strict To Tag
****			check is to be performed.	
**** Parameters:
****			pMsg(IN): The SIP Message corresponding to which the txn is
****			to be fetched.
****
****			dOpt(IN):The variable that informs whether a strict To Tag
****			check is to be performed.
****
****			pTxnInfo(IN):General information that is always passed to funcs.
****
****			ppTxnKey(OUT): The key that is created internally within this
****			fn in order to fetch the Txn needs to be returned to the caller
****			by reference.
****		
****			ppTxnBuffer(OUT): The txn buffer that has been fetched will 
****			be returned in this parameter by-reference.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_fetchTxnUsingMessage _ARGS_(( SipMessage *pMsg,\
				SIP_S8bit dOpt, SipTxnInformn *pTxnInfo,SipTxnKey **ppTxnKey,\
					SipTxnBuffer **ppTxnBuffer, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_createTxnUsingKey
****
****
**** DESCRIPTION:This fn is used to add a txn in the Txn Aggregation, 
****			corresponding to the SipTxnKey that is also passed.
**** Parameters:
****			ppTxnBuffer(IN/OUT): The TxnBuffer entry that has been create
****			in the TxnBuffer aggregation.
****
****			ppTxnKey(IN):The key corresponding to which a TxnBuffer entry
****			is to be created in the aggregation.
****
****			pTxnInfo(OUT): This parameter gets updated with the key of
****			of the newly created txn.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_createTxnUsingKey _ARGS_(( SipTxnBuffer **ppTxnBuffer,\
			SipTxnKey **ppTxnKey, SipTxnInformn *pTxnInfo,SipError *pError));


/***************************************************************************
**** FUNCTION:sip_txnUpdateStats
****
****
**** DESCRIPTION:This function updates the statistics.
****
**** Parameters:
****			pTxnKey(IN):The txnKey for which the retransX is happening
****
**** Return Values:
****			NONE
****
**************************************************************************/
extern void sip_txnUpdateStats _ARGS_(( \
		SipTxnKey *pTxnKey));


/***************************************************************************
**** FUNCTION:sip_populateTimeOutValues
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the initial data
****			Updates the TimeOut values for this txn.
****
**** Parameters:
****			pTimoutValues(IN/OUT): This is going to get populated with the
****			timeout values.
****
****			pTxnInformn(IN):Based on the informn contained in this
****			parameter the TimeOut values are populated.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_populateTimeOutValues _ARGS_(( \
		SipTimeOutValues *pTimeoutValues, SipTxnInformn *pTxnInformn));


/***************************************************************************
**** FUNCTION:sip_updateTxnBuffer
****
****
**** DESCRIPTION:This function is used to populate the TxnBuffer with 
****			transport related information
****
**** Parameters:
****			pTxnBuffer(OUT): This is going to get populated with the
****			values.
****
****			pTxnInformn(IN):Based on the informn contained in this
****			parameter the TxnBuffer gets populated.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_updateTxnBuffer _ARGS_((\
						SipTxnBuffer *pTxnBuffer,\
						SipTxnInformn *pTxnInformn, SipError *pError));
						
/***************************************************************************
**** FUNCTION:sip_populateTxnBuffer
****
****
**** DESCRIPTION:This function poulates the Txn Buffer with the  data
****			contained in the pTxnInfo variable. Ideally this fn is called
****			only when a txn is initially getting created.
****
**** Parameters:
****			pTxnBuffer(OUT): This is going to get populated with the
****			values.
****
****			pTxnInformn(IN):Based on the informn contained in this
****			parameter the TxnBuffer gets populated.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_populateTxnBuffer _ARGS_(( SipTxnBuffer *pTxnBuffer,\
						SipTxnInformn *pTxnInformn, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_queueUpRetransxForProxy
****
****
**** DESCRIPTION:This fn is called whenever retransmitted requests need to get
****			queued for a Proxy Txn. The TxnBuffer jhas an internal list of
****			messages to which the retransmitted messages are added.	
****
**** Parameters:
****			pTxnBuffer(OUT): The List contained in this is  going to get
****			populated with the retransmitted request messages.
****
****			pTxnInformn(IN):This contains the retransmitted buffer.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_queueUpRetransxForProxy _ARGS_(( SipTxnBuffer *pTxnBuffer,\
						SipTxnInformn *pTxnInformn, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_dropQueuedUpForProxy
****
****
**** DESCRIPTION:This fn is called whenever retransmitted requests need to get
****			dropped for a Proxy Txn. The TxnBuffer has an internal list of
****			messages to which the retransmitted messages are added.	
****
**** Parameters:
****			pTxnBuffer(OUT): The List contained in this is  going to get
****			populated with the retransmitted request messages.
****
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
****
**************************************************************************/
extern SipBool sip_dropQueuedUpForProxy _ARGS_(( SipTxnBuffer *pTxnBuffer,\
										SipError *pError));

/***************************************************************************
**** FUNCTION:sip_cloneSipTxnContext
****
****
**** DESCRIPTION:This API will be used internall to clone a SipTxnContext.
****
**** Parameters:
****
****		pSrc(IN): The txnContext data from which data needs to be copied
****		pDest(OUT): The txnContext data to which data needs to be copied
****
****		pError(OUT):Error Variable
******************************************************************************/
SipBool sip_cloneSipTxnContext
        (SipTxnContext *pSrc, SipTxnContext *pDest,SipError *pErr);

/***************************************************************************
**** FUNCTION:sip_compareTranspAddr
****
**** DESCRIPTION:This Function does a comparison on the dIPv4 array and the
****			  port contained in two transport Addresses and returns success
****			  if they are the same.
****
**** Parameters:
****
****		pTransp1(IN): Transport Address 1
****		pTransp2(IN): Transport	Address 2
****
******************************************************************************/
SipBool	sip_compareTranspAddr
		(SipTranspAddr	*pTransp1,SipTranspAddr	*pTransp2);

/***********************************************************************
**** FUNCTION: sip_startTimerCr
****
**** DESCRIPTION:This function starts the Timer Cr on the Txn buffer that
****		    is passed. It also takes care to send the indicate Timeout
****		    callback to the application, that is interested in receipt of 
****		    a callback upon the expiry of this timer
****
**** Parameters:
****	
****		pTxnBuffer:The Txn for which we need to start the Timer Cr
****		pTxnKey: The Txn key
****		pContext: The event context that will need to be passed to the 
****				  indicateTimeOut callback.
****        pTimeOutData: The time out data that had been received when the
****				  Timer C had fired.
****		pErr:	Error Variable
***********************************************************************/
SipBool	sip_startTimerCr
	(SipTxnBuffer *pTxnBuffer,SipTxnKey	*pTxnKey, SipEventContext *pContext,\
	SipTxnTimeoutData	*pTimeOutData, SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_getCulpritTimer
****
**** DESCRIPTION:We have a lot of timers that are started in tandem. For ex
****	         (Timer A)+(Timer B),(Timer E)+(Timer F). Now the basic txn
****		     layer architecture does not lend itself to simultaneous start	
****		     of multiple timers for a single txn. To get arnd this we 
****		     start the MIN(dur(x),dur(y)). In this function we determine
****		     which timer is the one that had fired.
**** Parameters:
****
****		pTxnBuffer(IN): The Txn for which the timer firing actually happened
****
***********************************************************************/
en_SipTimerType sip_getCulpritTimer (SipTxnBuffer	*pTxnBuffer);
	
/***************************************************************************
**** FUNCTION:sip_throwTimeOutCbk
****
**** DESCRIPTION:This Function decides on the basis of the bitmask stored
****			  within SipTxnBuffer whether the application is interested 
****			  in receiving the timeOut callback or not
****
**** Parameters:
****
****		dCulpritTimer(IN): The timer that is getting timed out
****		dBitMask(IN): The bit mask set in the TxnBuffer
****
******************************************************************************/
SipBool	sip_throwTimeOutCbk
		(en_SipTimerType	dCulpritTimer,SIP_U32bit	dBitMask);

/***************************************************************************
**** FUNCTION:sip_freeSipTxnContext
****
****
**** DESCRIPTION:This API will be used internall to free the contents of 
****			 any TxnContext structure.
****
**** Parameters:
****
****		pTarget(IN): The txnContext data which needs to be freed
****
****		pError(OUT):Error Variable
******************************************************************************/
SipBool sip_freeSipTxnContext
        (SipTxnContext *pTarget,SipError *pErr);
		
		
/**************************************************************************
*************** STATE UPDATE FUNCTIONS***********************************
		The functions having the name of the format
		sip_updateXXXTxnAfterMesgDecoding
		sip_updateXXXTxnForMesgSent
		are internal function susing which the state of the Txn is changed.
		The parameters for all these functions are pretty similar.

		
		Parameters:
		pTxnInformn(IN):This parameter contains all information that will
						be needed when changing the state of the Txn.
		pDecodeResult(OUT):This parameter is present in only the kind of
						functions wherein a state change has ocurred upon 
						decoding of a message.Valid values for this are
						
							SipTxnIgnorable,
							SipTxnNonIgnorable,
							SipTxnStrayMessage,
							SipTxnConfirmnNeeded,
							SipTxnQueued
							
		pError(OUT):This is the error variable.
		
		Return Values:
							SipSuccess
							SipFail

***************************************************************************/
						
/***************************************************************************
**** FUNCTION:sip_updateInvClTxnAfterMesgDecoding
****
****
**** DESCRIPTION:The Invite Client txn gets updated after rcvng a response
******************************************************************************/
extern SipBool sip_updateInvClTxnAfterMesgDecoding _ARGS_(( \
			SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult,\
			SipError *pError));



/***************************************************************************
**** FUNCTION:sip_updateNonInvClTxnForMesgSent
****
****
**** DESCRIPTION:A noninvite client txn gets updated after sending a message.
******************************************************************************/
extern SipBool sip_updateNonInvClTxnForMesgSent _ARGS_(( \
			SipTxnInformn *pTxnInformn, SipError *pError));




/***************************************************************************
**** FUNCTION:sip_updateNonInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:A non-invite server txn gets update after a message is sent.
****************************************************************************/
extern SipBool sip_updateNonInvSrvTxnForMesgSent _ARGS_(( \
	SipTxnInformn *pTxnInformn, SipError *pError));




/***************************************************************************
**** FUNCTION:sip_updateNonInvSrvTxnAfterMesgDecoding
****
****
**** DESCRIPTION:A non-invite server txn gets updated after receiving and
****			decoding a message.
****************************************************************************/
extern SipBool sip_updateNonInvSrvTxnAfterMesgDecoding _ARGS_(( \
	SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult,\
	SipError *pError));



/***************************************************************************
**** FUNCTION:sip_updateInvClTxnFor2xxRecvd
****
****
**** DESCRIPTION:An Invite client txn gets updated after receiving a 2xx 
****			response.
****************************************************************************/
extern SipBool sip_updateInvClTxnFor2xxRecvd _ARGS_(( \
	SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult,\
	SipError *pError));


/***************************************************************************
**** FUNCTION:sip_updateInvSrvTxnAfterMesgDecoding
****
****
**** DESCRIPTION:An Invite server txn gets updated after receiving and 
****	decoding a response.
****************************************************************************/
extern SipBool sip_updateInvSrvTxnAfterMesgDecoding _ARGS_(( \
	SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult,\
	SipError *pError));



/***************************************************************************
**** FUNCTION:sip_updateProxyInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:A proxy invite server txn gets updated after a message is sent.
******************************************************************************/
extern SipBool sip_updateProxyInvSrvTxnForMesgSent _ARGS_(( \
	SipTxnInformn *pTxnInformn, SipTxnBuffer *pTxnBuffer,\
	SipTxnKey *pTxnKey, SipError *pError));




/***************************************************************************
**** FUNCTION:sip_updateInvSrvTxnForMesgSent
****
****
**** DESCRIPTION:An Invite server txn gets updated after a message is sent
******************************************************************************/
extern SipBool sip_updateInvSrvTxnForMesgSent _ARGS_(( \
	SipTxnInformn *pTxnInformn, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_updateRPRTxnAfterMesgDecoding
****
****
**** DESCRIPTION:A RPR txn gets updated after it receives a message
******************************************************************************/
extern SipBool sip_updateRPRTxnAfterMesgDecoding _ARGS_(( \
	SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult, \
	SipError *pError));


/***************************************************************************
**** FUNCTION:sip_updateRPRTxnForMesgSent
****
****
**** DESCRIPTION:A RPR Txn gets created when an RPR is sent.
******************************************************************************/
extern SipBool sip_updateRPRTxnForMesgSent _ARGS_(( \
	SipTxnInformn *pTxnInformn, SipError *pError));



/***************************************************************************
**** FUNCTION:sip_updateInvClTxnForMesgSent
****
****
**** DESCRIPTION:An Invite client txn gets updated after a message is sent.
******************************************************************************/
extern SipBool sip_updateInvClTxnForMesgSent _ARGS_(( \
		SipTxnInformn *pTxnInformn, \
		SipError *pError));




/***************************************************************************
**** FUNCTION:sip_updateNonInvClTxnAfterMesgDecoding
****
****
**** DESCRIPTION:The Non-Invite Client txn gets updated after receiving
****				a response
******************************************************************************/
extern SipBool sip_updateNonInvClTxnAfterMesgDecoding _ARGS_(( \
	SipTxnInformn *pTxnInformn, en_SipTxnDecodeResult *pDecodeResult,\
	SipError *pError));



/**************************************************************************
*************** STATE UPDATE FUNCTIONS END***********************************/





/***********************************************************************
**** FUNCTION: timerTimeOut
****
**** DESCRIPTION:This Function will be called by timer Module when any 
****  of the timerstarted by fast_start_timer is timedOut.       
****
***********************************************************************/
SipBool timerTimeOut _ARGS_ ((SIP_Pvoid pData));

/***********************************************************************
**** FUNCTION: sip_txn_startTimersWithTranspCheck
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_startTimersWithTranspCheck _ARGS_ ((SipTxnBuffer* pBuffer, 
		SipTxnKey* pTxnKey, \
		SIP_Pvoid pContextData,en_SipTimerType dTimer1,\
		en_SipTimerType dTimer2,\
		SIP_U32bit dDuration1,SIP_U32bit dDuration2,\
		SipBool dTranspCheck, SipError* pErr));

/***********************************************************************
**** FUNCTION: sip_txn_startTimer
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_startTimer _ARGS_ ((SipTxnBuffer* pBuffer, \
		SipTxnKey* pTxnKey, en_SipTimerType dTimer,\
	 	SIP_U32bit dDuration,SipTxnInformn *pTxnInfo,SIP_Pvoid pContextData,\
		SipError* pErr));


/********INTERNAL STATE CHANGE FUNCTIONS*******************************
***********************************************************************
**** FUNCTION: sip_txn_XXXState
****
**** DESCRIPTION:The prototypes mentioned below are the prototypes of 
****			state change functions. These are the functions that 
****			handle all state change Timer related functionality
****			like starting and stopping timers.The XXX in the 
****			function is the state to which the change is occurring.
****
****
**** Parameters:
****			pBuffer(IN):The txn-buffer corresponding to the txn
****			that is undergoing a state change.
****
****			pKey(IN):This contains the txn key using which the buffer 
****			had been fetched.
****
****			pError(OUT):The error variable
****
**** Return Values:
****			SipSuccess
****			SipFail
***********************************************************************/
/*Internal state change functions*/

/***********************************************************************
**** FUNCTION: sip_txn_InvClCallingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvClCallingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvClProceedingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvClProceedingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvClFinalResponseRecvState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvClFinalResponseRecvState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvClCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvClCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvClTryingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvClTryingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvClProceedingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvClProceedingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey, SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvClCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvClCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvSrvProceedingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvSrvProceedingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvSrv1xxSentState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvSrv1xxSentState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvSrv2xxForwardedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvSrv2xxForwardedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvSrvCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvSrvCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_InvSrvConfirmedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_InvSrvConfirmedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvSrvTryingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvSrvTryingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvSrvProceedingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvSrvProceedingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvSrvCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvSrvCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvClientRPRRecvngState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvClientRPRRecvngState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvServerRPRSendingState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvServerRPRSendingState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);

/***********************************************************************
**** FUNCTION: sip_txn_TerminatedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_TerminatedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/***********************************************************************
**** FUNCTION: sip_txn_NonInvClientRPRCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvClientRPRCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);

/***********************************************************************
**** FUNCTION: sip_txn_NonInvServerRPRCompletedState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_NonInvServerRPRCompletedState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);


/***********************************************************************
**** FUNCTION: sip_txn_SrvBypassState
****
**** DESCRIPTION:
****         
***********************************************************************/
SipBool sip_txn_SrvBypassState
	(SipTxnBuffer* pBuffer,SipTxnKey* pKey,SIP_Pvoid pContextData,\
	SipTxnInformn *pTxnInfo,\
	SipError *pErr);
	
/********INTERNAL STATE CHANGE FUNCTIONS END*******************************
**************************************************************************/	

/***********************************************************************
**** FUNCTION: sip_removeRPRTxn
****
**** DESCRIPTION:This Removes the RPR Txn Corresponding to the PRACK Key
****         
****Parameters: pTxnKey(IN) PRACK Txn Key
***				pTxnOnformn(IN) refrence to Txn Information
***				pDecodeResult(IN) not used SIP_NULL can be passwd
***				pError(OUT) Error Variable
*** Return Values:Returns SipFail if there is no RPR Txn 
***				Corresponding to PRACK Txn,If it succeeds in removing returns 
***				SipSuccess 
***********************************************************************/
SipBool sip_removeRPRTxn
        (SipTxnKey *pTxnKey,SipTxnInformn *pTxnInfo, \
		en_SipTxnDecodeResult *pDecodeResult,SipError *pError);

/***********************************************************************
**** FUNCTION: sip_updateRPRTxnState
****
**** DESCRIPTION:This updates all the Txn's in the list to the given State
****         
**** Parameters : pslRPRTxnKeyQ(IN) Reference to the list of Txn Keys
****			  dTxnState(IN) the state into which all Transactions 
****			  				to be Changed
****			  pTxnInfo(IN) Reference to the Txn Info
**** Return Values:Returns SipFail If some Error Occurs
****
***********************************************************************/
SipBool sip_updateRPRTxnState
	(SipList *pslRPRTxnKeyQ,en_SipTxnState dTxnState,SIP_Pvoid pContextData, \
	SipError *pError);

/***********************************************************************
**** FUNCTION: sip_getInvTxnStateFromPRACKTxnKey
****
**** DESCRIPTION:This Returns the state of Inv Txn of the given PRACK Txn Key
****         
**** Parameters: pTxnKey(IN) The key from which Inc Txn State to be returned    
****         	 pTxnState(OUT) state of INv Txn will be returned 
****							in this variable
****        	 pError(OUT) Error Variable 
**** Return Values: Returns Sipfail if there is no Inv Txn,
****					Otherwise returns SipSuccess        
****         
***********************************************************************/
SipBool sip_getInvTxnStateFromPRACKTxnKey
	(SipTxnKey *pTxnKey,SipTxnInformn *pTxnInfo, \
	en_SipTxnState *pTxnState,SipError *pError);

/***********************************************************************
**** FUNCTION: sip_getRPRTxnKeyFromPRACKTxnKey
****
**** DESCRIPTION:This Returns PRACK Txn Key,corresponding to the RPR Txn Key
***				 by allocating new memory,this has to be freed by the Calle
***	 Parameters : pTxnKey(IN) PRACK Txn Key
***	 			 ppClonedKey(OUT) RPR Txn Key formed from the given RPR Txn Key
***				 pError(OUT) Error Variable
****         
***********************************************************************/
SipBool sip_getRPRTxnKeyFromPRACKTxnKey
	(SipTxnKey *pTxnKey,SipTxnKey **ppClonedKey,\
	SipError *pError);

/***********************************************************************
**** FUNCTION: sip_searchRPRTxnKeyListAndAdd
****
**** DESCRIPTION:For a given RPR Txn key this function searches the list 
****				of RPR Txns in the Txn Buffer and adds to the List if not 
****				present.If dCheckSeq is enabled it Checks the RSeq number
****				to be in sync with the previous Txns in the List.
****	Parameters:  pTxnBuffer(IN) Reference to the Txn Buffer
****				 pTxnKey (IN) Key to be searched in the List
****				 dCheckSeq(IN) Flag to check Sequence NUmber
****				 pError(OUT) Error Variable
**** Return Values: Returns SipFail if the key is already present 
****			   	in the list or RSeq is Out of Sequence.OtherWise 
****				Returns SipSuccess.
****         
***********************************************************************/
SipBool sip_searchRPRTxnKeyListAndAdd
	(SipTxnBuffer *pTxnBuffer,SipTxnKey *pTxnKey,SipBool dCheckSeq, \
	SipError *pError);


SipBool __sip_txn_getBranchFromViaHdr (SipViaHeader *pViaHeader,\
	 	SIP_S8bit **ppBranch, SipError *pErr);

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif

