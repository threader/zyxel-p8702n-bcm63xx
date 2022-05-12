/******************************************************************************
 ** FUNCTION:
 **             Has the internal functions of the framework
 ******************************************************************************
 **
 ** FILENAME: 		sdf_internal.h
 **
 ** DESCRIPTION:	This file contains declarations for functions used 
 **					internally in the Framework for call handling.
 **
 **  DATE        NAME                  REFERENCE             REASON
 **  ----        ----                  ---------            --------
 ** 27Oct00		Anitha									    Creation
 **				Prashant								    Updated
 ** 22/04/03    Anil Kumar                ---       Changes for TLS feature.
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_INTERNAL_H_
#define __SDF_INTERNAL_H_

#include "sdf_mempool.h"


/* The toolkit internal structure */
extern Sdf_st_toolkitData *pGlbToolkitData;

#ifdef SDF_USE_PREALLOCBUFFERS
/* Preallocated Message Buffer pool */
extern Sdf_st_mempool dGlbPreallocMsgBufferPool;
#endif

#ifdef SDF_USE_INTERNALTHREADS
/* Retransmission thread spawned by the toolkit */
extern Sdf_ty_threadT dGlbRemoteRetransThread;
#endif


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaIsMessageRPR
 **
 ** DESCRIPTION:	This function finds whether the response message 
 **					is an Reliable Provisional Response.
 **
 ** PARAMETERS:
 **		pMessage(IN)  : The SipMessage structure which is to be 
 **						verified for RPR. 
 **		pError(OUT)	  :	Error structure in case the function fails
 **
 **	RETURN:
 **		0 if the message is an RPR
 **		1 if the message is not an RPR/an internal occurs
 **
 ******************************************************************************/
 Sdf_ty_u8bit sdf_fn_uaIsMessageRPR _ARGS_((SipMessage  \
	*pMessage, Sdf_st_error *pError));



#ifdef SDF_SESSION_TIMER 
 /******************************************************************************
 ** FUNCTION:  		sdf_fn_uaUpdateSessionTimerInfo
 **
 ** DESCRIPTION:	This function update the SessionTimerInfo structure
 **					with the incoming message.
 **
 ** PARAMETERS:
 **		pCallObj(IN/OUT)  : The Call object structure which SessionTimerInfo
 **							structure will be updated. 
 **		pMessage(IN)  	  : The SipMessage structure which Session Timer
 **							specific information will be updated in 
 **							the SessionTimerInfo structure. 
 **		pError(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateSessionTimerInfo _ARGS_((Sdf_st_callObject \
 	*pCallObj,SipMessage *pMessage,Sdf_st_error *pErr));
#endif



/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaGetReasonFromTable
 **
 ** DESCRIPTION:	This function gets the Reason from the 
 ** 				CodeReasonMapping table 
 **
 ** PARAMETERS:
 **		dRespCode(IN) : The response code for which the reason 
 **						must be obtained. 
 **		pReason(OUT)  :	The reason corresponding to the resp code.
 **		pError(OUT)	  :	Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetReasonFromTable _ARGS_((Sdf_ty_u16bit  \
	dRespCode,Sdf_ty_s8bit **pReason, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaIsNumber
 **
 ** DESCRIPTION: This function takesin a string and returns
 ** 			 true if it is an integer
 **
 ** PARAMETERS:
 **		pStr(IN) :The string which has to be checked. 
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaIsNumber _ARGS_((Sdf_ty_s8bit *pStr));

/******************************************************************************
 ** FUNCTION:  sdf_fn_uaInitCsmTable
 **
 ** DESCRIPTION: This function initializes the CSM table
 **
 ** PARAMETERS: void
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaInitCsmTable _ARGS_((void));

/******************************************************************************
** FUNCTION: sdf_fn_uaGenerateRetransKey
**
** DESCRIPTION: This function generates a unique key to which
**				is useful in handling remote retransmissions.
**
** PARAMETERS:
**		pMsg (IN) :The SipMessage for which the key is generated.
**		ppKey(OUT) : The unique key which si generated.
**		pErr(OUT)	: Error structure in case the function fails
**
*******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGenerateRetransKey _ARGS_((SipMessage  \
	*pMsg, Sdf_st_remoteRetransKey *pKey,Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaGetAddrSpecAsString
 **
 ** DESCRIPTION: This function retrieves the addr spec from 
 **				 the Sip Message as a string.
 **
 ** PARAMETERS:
 **		pMsg(IN) 		: The Sip Message structure from which the 
 **						  addr spec is is retrieved. 
 **		dAddrType (IN) 	: The type of the adddrs spec, 
 **						  if it is a SIP URL etc.
 **		ppAddrSpecStr(OUT) : The retreived addr Spec string.
 **		pError(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal  sdf_fn_uaGetAddrSpecAsString _ARGS_((SipMessage *\
		pMsg,en_HeaderType dAddrtype, Sdf_ty_s8bit **ppAddrSpecStr,\
		Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  sdf_fn_uaInsertIntoRemoteRetransList
 **
 ** DESCRIPTION: This function inserts a remote Retrans buffer 
 **				into the remote retransmission list. 
 **
 ** PARAMETERS:
 **		pObj(IN) 		: The call object structure.
 **		pDestinfo(IN)	: The Destination info structure.
 **		dSockfd(IN) 	: The Sockfd which will be insert in the
 **							resmoteretransmission list.
 **		pSipMsg(IN)		: The Sip Message that is to be inserted
 **						  into the remote retransmission list
 **		dOptions(IN)    : The options to be used for inserting 
 **						  the transport information into the 
 **						  Retrans Buffer.
 **			 			  Multiple options can be ORed. The 
 **						  supported options are:
 **
 **						  1. SIP_OPT_SINGLE: Each header is formed 
 **								in a separate line. The message will 
 **								not contain comma separated headers.
 **								Cannot be used with 
 **								SIP_OPT_COMMASEPARATED.
 **
 **						  2. SIP_OPT_COMMASEPARATED: Multiple instances 
 **								of the same kind of header will appear 
 **								on the same header line separated by
 **								commas. Cannot be used with 
 **								SIP_OPT_SINGLE.
 **
 **						  3. SIP_OPT_SHORTFORM: Single-letter short
 **								forms will be used for headers that 
 **								have short names. Cannot be used with 
 **								SIP_OPT_SULLFORM.
 **
 **						  4. SIP_OPT_FULLFORM: All headernames will 
 **								appear in full. Cannot be used with 
 **								SIP_OPT_SHORTFORM.
 **
 **						  5. SIP_OPT_CLEN: A Content-Length header with 
 **								the correct length is inserted in the 
 **								message being formed. If the message 
 **								structure has a Content-Length header,
 **								the function corrects the length field 
 **								depending on size of the message body.
 **
 **						  6. SIP_OPT_REORDERHOP: All hop to hop headers 
 **								will be placed above the end to end 
 **								headers.
 **
 **						  7. SIP_OPT_AUTHCANONICAL: All headers after 
 **								the Authorization header will be 
 **								formed in the canonical form. This 
 **								will override options 
 **								SIP_OPT_SHORTFORM and 
 **								SIP_OPT_COMMASEPARATED from the 
 **								Authorization header onwards.
 **
 **	pErr(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInsertIntoRemoteRetransList _ARGS_\
	((Sdf_st_callObject *pObj,Sdf_st_transportInfo *pDestInfo,\
	Sdf_ty_socketId dSockfd, SipMessage *pSipMsg, SipOptions dOptions,\
	Sdf_st_eventContext	*pContext, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: sdf_fn_uaUpdateCallConstants
 **
 ** DESCRIPTION: This function updates the From,To, Callid and 
 **				CallObjectKey of the call object. 
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateCallConstants _ARGS_\
	((Sdf_st_callObject *pCallObj, SipMessage *pMessage, \
	Sdf_st_msgInfo	dMessageInfo, Sdf_ty_bool dIsIncMsg, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: sdf_fn_uaConvertContactToRouteHdr
 **
 ** DESCRIPTION: This function retrieves the AddrSpec and other
 ** 			 relevant details from the contact header and 
 **				 copies them onto a new route header 
 **
 ******************************************************************************
 Sdf_ty_retVal sdf_fn_uaConvertContactToRouteHdr  _ARGS_\
	((Sdf_st_callObject *pCallObj, SipHeader *pRouteHeader, \
	Sdf_st_error *pErr));
*/

/******************************************************************************
 ** FUNCTION:  sdf_fn_uaUpdateCallObject
 **
 ** DESCRIPTION: This function updates the call object with 
 **				the Sip Message structure details.
 **
 ** PARAMETERS:
 **		pObject(IN) 		  :	The call Object which needs to be updated
 **		pOverlapTransInfo(IN) : The overlap transInfo structure in case
 **								or RPR/COMET. SHould be NULL if updations 
 **								are to be made to the main call object
 **		pSipMsg(IN)			  : The SipMessage received from the network
 **		pError(OUT)			  : Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateCallObject _ARGS_\
	((Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo,\
	SipMessage *pSipMessage, Sdf_st_msgInfo	dMessageInfo, Sdf_st_error *pErr));

/******************************************************************************
** FUNCTION: sdf_fn_uaGetAddrSpecFromNameInfo
**
** DESCRIPTION: This function retrieves the addr spec 
**
** PARAMETERS:
**		pNameInfo(IN) : The name info struct from which the 
**						addr spec is got.
**		ppAddrSpec(OUT): Element into which the data is retrieved
**		pError(OUT)	: Error structure in case the function fails
**
*******************************************************************************
 Sdf_ty_retVal sdf_fn_uaGetAddrSpecFromNameInfo _ARGS_((Sdf_st_nameInfo \
	*pNameInfo,SipAddrSpec **ppAddrSpec, Sdf_st_error *pError));
*/

#ifndef SDF_TXN
/******************************************************************************
** FUNCTION: sdf_fn_uaCheckRetransmission
**
** DESCRIPTION: This function checks if remote retransmission 
**				needs to be done for a message 
**
** PARAMETERS:
**		pNewCallObj(IN) : The the call object which needs to be 
**						  checked for retransmissions.
**		pError(OUT)		: Error structure in case the function fails
**
*******************************************************************************/
 Sdf_ty_matchMessageType sdf_fn_uaCheckRetransmission \
	_ARGS_((SipMessage *pMsg, Sdf_st_eventContext *pContext, \
	Sdf_st_error *pErr));
#endif

#ifdef SDF_USE_INTERNALTHREADS
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaRemoteRetransManager
 **
 ** Description: This is the function that is launched by the 
 **				 remote retrans manager. It does the job of 
 **				 periodically spooling and removing entries which
 **				 have been present for more than 32 seconds.
 **
 ** Parameters:
 **			void
 **
 ******************************************************************************/
Sdf_ty_fnreturn sdf_fn_uaRemoteRetransManager _ARGS_((Sdf_ty_pvoid ptr));
#endif

/*******************************************************************
** FUNCTION: sdf_fn_uaFormToHeader
**
** DESCRIPTION: This function forms the To header with the
**	information in the Call Object 
**
** PARAMETERS:
**		pObject(IN) :The the call object corresponding to the call
**		pMsg(OUT) : The SipMessage into which the To header is set
**		pError(OUT)	: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormToHeader _ARGS_((Sdf_st_callObject *pObject,\
	SipMessage *pMessage, Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_fn_uaFormFromHeader
**
** Description: This function forms the From header with the
**				information in the Call Object 
**
** Parameters:
**		pObject(IN) :The the call object corresponding to the call
**		pMsg(OUT) : The SipMessage into which the From header is set
**		pError(OUT)	: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormFromHeader _ARGS_((Sdf_st_callObject \
	*pObject,SipMessage *pMessage, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: sdf_fn_uaFormCallIdHeader
 **
 ** DESCRIPTION: This function forms the CallId header with the
 **				information in the Call Object 
 **
 ** Parameters:
 **		pObject(IN) :The call object corresponding to the call
 **		pMsg(OUT) : The SipMessage into which the Callid header is set
 **		pError(OUT)	: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormCallIdHeader _ARGS_((Sdf_st_callObject  \
	*pObject,SipMessage *pMessage, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: sdf_fn_uaFormContactHeaders
 **
 ** DESCRIPTION: This function forms a list of Contact header with the
 **				 information in the Call Object and sets this value in the
 **				 SIP Message.
 **
 ** PARAMETERS:
 ** 	pContactList(IN): List containing the contact headers to be formed.
 **		pMessage(IN/OUT): The message to which the headers are added.
 **		dTranspType (IN): Transport type. Either Sdf_en_protoTcp or
 **                       Sdf_en_protoTls.
 **						  This is needed so as to add a transport=tcp 
 **						  parameter to the first Contact header.
 **     dStrictCheck(IN): If true, at least one contact must be present
 **		pError(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/
 #ifdef SDF_UASSF_ONLY
 /*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
  Sdf_ty_retVal sdf_fn_uaFormContactHeaders _ARGS_((Sdf_ty_slist *pContactList,\
  	SipMessage *pMessage, Sdf_ty_protocol dTranspType, \
	Sdf_ty_addrType dContactAddrType, Sdf_ty_bool dStrictCheck, \
	Sdf_st_error *pError));

 #else

  Sdf_ty_retVal sdf_fn_uaFormContactHeaders _ARGS_((Sdf_ty_slist *pContactList,\
 	SipMessage *pMessage, Sdf_ty_protocol dTranspType, \
	en_AddrType dContactAddrType, Sdf_ty_bool dStrictCheck, \
	Sdf_st_error *pError));
#endif

/******************************************************************************
 ** FUNCTION: sdf_fn_uaFormProcessedHeaders
 **
 ** DESCRIPTION:	This function forms the Additional header 
 ** 				with the information in the Call Object and sets 
 **				this value in the SIPMessage. The additional 
 **				headers are the Route/Record-Route headers and 
 **				the Authorization headers
 **
 ** Parameters:
 **		pObject(IN/OUT)		 : The CallObject which contains the 
 **						 	   additional heades to be formed.
 **		pOverlapTransInfo(IN): The overlapTransInfo structure. Should
 **			 				   be NULL if the additional headers are 
 **							   picked from the main call object.
 **		pMethod(IN)			 : The additional headers are formed based 
 **					 		   on the method.
 **		pMessage(IN/OUT)	 : The message to which additional
 **						  	   headers are added.
 **		pError(OUT)			 : Error structure in case the function fails
 **
 ******************************************************************************/

 Sdf_ty_retVal sdf_fn_uaFormProcessedHeaders _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_st_msgInfo	dMessageInfo, SipMessage *pMessage,\
   	Sdf_ty_bool	dReliableResponse, Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_fn_uaRedirectCall
**
** DESCRIPTION: Acknowledges the current 200Ok response and
**				sends an INVITE to the new address (after 
**				incrementing the CSeq)
**
** PARAMETERS:
**		pObject(IN/OUT): The CallObject which contains the 
**						 call to be redirected.
**		pError(OUT)	: Error structure in case the function fails
**
********************************************************************
 Sdf_ty_retVal sdf_fn_uaRedirectCall _ARGS_((Sdf_st_callObject \
	 **pCallObj,Sdf_st_error *pError));
*/

/*******************************************************************
** FUNCTION: sdf_fn_uaIsSupportedMethod
**
** DESCRIPTION: Checks if the method is supported and if not
** 				sends back a 502 response
**
** PARAMETERS:
**		pObject(IN/OUT): The CallObject to in which the method
**						 is checked.
**		pError(OUT)	: Error structure in case the function fails
*******************************************************************
 Sdf_ty_retVal sdf_fn_uaIsSupportedMethod _ARGS_((Sdf_st_callObject  \
	*pCallObj, Sdf_st_error *pError));
*/

/******************************************************************************
 ** FUNCTION: sdf_fn_uaIsHigherCseq
 **
 ** DESCRIPTION: Checks if the Cseq in the SipMessage is
 **				 higher than that in the callobject. 
 **
 ** PARAMETERS:
 **		pObject(IN): 			The CallObject where the check is made
 **		pMsg(IN): 				The received message from the network.
 **		pDisplayCallFlow(OUT): 	To indicate whether the Call Flow
 **								display (ctascii) needs to be updated. This 
 **								is used to distinguish remote retransmissions 
 **								from other messages (to be discarded) that 
 **								need to be displayed.
 **		dMethodType(IN):		The SIP method as Enum
 **		pRespCode(OUT):			The response code with which the request must
 **								be responded to.
 **		pError(OUT)	: 			Error structure in case the function fails
 **
 ** RETURN VALUES:
 **		- Sdf_en_valid if Cseq in SipMessage is higher
 **		- Sdf_en_invalid if Cseq is invalid
 **		- Sdf_en_discarded if the message is to be discarded
 **			(Eg. a retransmitted INVITE has come when the 
 **			original INVITE has not yet been responded to)
 **		- Sdf_en_failed if an API fails
 **
 ******************************************************************************/
 Sdf_ty_messageValidity sdf_fn_uaIsValidCseq  _ARGS_\
	((Sdf_st_callObject *pCallObj , SipMessage *pMsg, \
	Sdf_ty_s32bit *pDisplayCallFlow, Sdf_ty_messageType dMethodType, \
	Sdf_ty_u16bit *pRespCode, Sdf_ty_bool *pIsSameCseq, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaVerifyCaller
 **
 ** DESCRIPTION: This function is used to check within a call who the callee 
 **				 is and who the callee for that particular call. This info is 
 **				 needed in the case while forming the To and From headers for 
 **				 the BYE request (whether swapping is necessary or not)
 **	 
 ** PARAMETERS:
 **		pObject	(IN/OUT): The CallObject where the To and From 
 **						  have to be set.
 **		pError	(OUT)	: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaVerifyCaller  _ARGS_((Sdf_st_callObject  \
	*pObject, Sdf_st_error *pError));


/*******************************************************************
** FUNCTION: sdf_fn_uaIsValidUriSchemePort
**
** DESCRIPTION: This function takes in the Sip Message
** 				from the call object and checks if the scheme 
**				is of type sip. It also checks if the port and 
**				Request uri are valid for the UA
**
********************************************************************
 Sdf_ty_retVal sdf_fn_uaIsValidUriSchemePort \
	 _ARGS_((Sdf_st_callObject *pCO, Sdf_st_error *pError));
*/


 Sdf_ty_retVal sdf_fn_uaCloneSlExtraHeadersList _ARGS_((\
	Sdf_ty_slist *pDest, Sdf_ty_slist *pSrc, Sdf_st_error *pErr)); 

/*******************************************************************
** FUNCTION: sdf_fn_uaHandleRequireHeader
**
** DESCRIPTION:	Checks if the Request Has a Require Header. If
**				the Require header is present then check with 
**				the options in the supported field of the 
**				config.txt file. The convention is that if the 
**				Supported Header is absent then a 420 Bad 
**				Extensions is sent atuomatically.
**
** 				If Supported is present in the config.txt file,
**				then it's values are compared with the require 
**				values. What is not supported is returned 
** 				as unsupported in a 420 response. If the 
**				Supported header in the config file has ' * ' 
**				as one of it's values then it is assumed that 
**				all the options are supported.
**
** PARAMETERS:
** 		pCallObj: Call Object structure that contians the SipMessage
**		pErr : Error Structure
**
********************************************************************
 Sdf_ty_retVal sdf_fn_uaHandleRequireHeader \
	_ARGS_((Sdf_st_callObject *pCallObj, \
	 Sdf_st_error *pErr));
*/

/*******************************************************************
** FUNCTION: sdf_fn_uaUpdateSeqNumbersInCallObj
**
** DESCRIPTION:	
** 	Update remote party's sequence numbers in the Callobject's
** 	pCommonInfo structure. Sequence numbers to be updated are
**	a) dRemoteCseq - The highest Cseq received so far.
** 	b) dRemoteRseq - The highest (Rseq, Cseq) received so far.
**	c) dRemoteRegCseq - The highest Cseq PRACK received so far.
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateSeqNumbersInCallObj \
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, SipMessage *pSipMsg, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaAddHdrListToProcessedHdrsInCallObj
**
** DESCRIPTION:	This function adds the list of headers in the
**   			SipMessage to the Processed headers list of 
**   			call object
**
********************************************************************/
 Sdf_ty_retVal  sdf_fn_uaAddHdrListToProcessedHdrsInCallObj
	_ARGS_((Sdf_st_transaction *pTransaction, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, SipMessage *pSipMsg, \
	en_HeaderType dHdrType, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaAddAuthHdrListToCallObj
**
** DESCRIPTION: This function adds the list of headers in the
**   			SipMessage to the Additional headers list of
**   			call object
**
********************************************************************/
 Sdf_ty_retVal  sdf_fn_uaAddAuthHdrListToCallObj
	_ARGS_((Sdf_st_transaction *pTransaction, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	SipMessage *pSipMsg, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaAddAuthenticateHdrListToCallObj
**
** DESCRIPTION:	This function adds the list of headers in the
**   			SipMessage to the Additional headers list of 
**   			call object
**
********************************************************************/
 Sdf_ty_retVal  sdf_fn_uaAddAuthenticateHdrListToCallObj
	_ARGS_((Sdf_st_transaction *pTransaction, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, 
	SipMessage *pSipMsg, en_HeaderType dHdrType, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaCreateRoutePathInCallObj
**
** DESCRIPTION:	This function creates the Route Header List from the 
**				incoming SipMessage and adds it to the Additional 
**				Headers list of both the CallInfo and the 
**				TransactionInfo structures of the call object. 
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaCreateRoutePathInCallObj _ARGS_( \
	 (Sdf_st_callObject *pCallObj, Sdf_st_msgInfo dMessageInfo, \
	  SipMessage *pMessage, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaUpdateRoutePathInCallObj
**
** DESCRIPTION: 
**   This function updates the Route Header List from the 
**	incoming SipMessage in the list of processed headers
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateRoutePathInCallObj 
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_st_msgInfo dMessageInfo, \
		SipMessage *pMessage, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_fn_uaUpdateContactListInCallObj
**
** DESCRIPTION: This function updates the Contact List of the 
**				CallObject with the information in the incoming 
**				SipMessage 
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaUpdateContactListInCallObj \
	_ARGS_((Sdf_st_callObject *pCallObj, SipMessage *pMsg, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  	sdf_fn_remoteRetransHash
**
** DESCRIPTION: Caclulates the hash value for a given key used in
**				the remote retransmissions hash table.
**
******************************************************************/
Sdf_ty_u32bit sdf_fn_remoteRetransHash _ARGS_((Sdf_ty_pvoid pKey));

/******************************************************************
** FUNCTION:  	sdf_fn_remoteRetransKeyCompare
**
** DESCRIPTION: Compare two keys (of elements stored in the remote
**				retransmission hash table).
**
******************************************************************/
Sdf_ty_u8bit sdf_fn_remoteRetransKeyCompare _ARGS_((Sdf_ty_pvoid pKey1,\
	Sdf_ty_pvoid pKey2));

/******************************************************************
** FUNCTION:  	sdf_fn_remoteRetransElementFree
**
** DESCRIPTION: Function to free an element from the remote 
**				retransmission hash table.
**
******************************************************************/
void sdf_fn_remoteRetransElementFree _ARGS_((Sdf_ty_pvoid pElement));

/******************************************************************
** FUNCTION:  	sdf_fn_remoteRetransKeyFree
**
** DESCRIPTION: Function to free a key (for an element in the 
**				remote retransmission hash table).
**
******************************************************************/
void sdf_fn_remoteRetransKeyFree _ARGS_((Sdf_ty_pvoid pKey));

/*******************************************************************
** FUNCTION: sdf_fn_uaCompareSipAddrSpec
**
** DESCRIPTION: This function is used to compare two addrspecs
**	 			according to the sip-url comparision rules
**	 
** PARAMETERS:
**		pAddrSpec1(IN): Addrspec to compare
**		pAddrSpec2(IN): Addrspec to compare
**		dCompleteMatch(IN): Set to Sdf_co_true if a complete URL match
**		is to be done (including url param match, etc). Set to Sdf_co_false
**		if only the user@host:port part needs to be compared.
**		dPortLeniancy(IN): Boolean indicating that we can ignore port
**		comparision results if the first addrspec does not specify a port.
**		Used to accomodate leniancy in Request-URI matching.
**		pError(IN/OUT)	: Error structure in case the function fails
**	RETURN:
**		0 if the URL's match
**		1 if the URL's don't match
**
********************************************************************/
 Sdf_ty_s8bit sdf_fn_uaCompareSipAddrSpec _ARGS_\
	((SipAddrSpec *pAddrSpec1, SipAddrSpec *pAddrSpec2, \
	Sdf_ty_bool dCompleteMatch, Sdf_ty_bool dPortLeniancy, \
	Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_fn_uaCompareSipHeaderFromTo
**
** DESCRIPTION: This function is used to compare two headers.
**				Only From/To headers may be passed to this API.
**	 
** PARAMETERS:
**		pHeader1(IN): Header to compare
**		pHeader2(IN): Header to compare
**		pCallLegMatch (OUT) : Enum indicating the comparision result
**		pError(IN/OUT)	: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaCompareSipHeaderFromTo _ARGS_\
	((SipHeader *pHeader1, SipHeader *pHeader2, \
	Sdf_ty_callLegMatch *pCallLegMatch, Sdf_ty_bool dToTag, \
	Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_fn_uaGenerateCallLegKey
**
** DESCRIPTION: This function is used to generate the call leg
**				key (stored in the common info) from a SipMessage.
**	 
** PARAMETERS:
**		pMsg(IN): SipMessage from which the key has to be constructed
**		ppKey(IN/OUT): key constructed
**		pError(IN/OUT)	: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaGenerateCallLegKey _ARGS_\
	((SipMessage *pMsg, Sdf_st_callObjectKey **ppKey, \
	Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_fn_uaRetrieveAddrSpecFromTopmostRoute
**
** DESCRIPTION: This function is used to retrieve the 
**				addrspec from the topmost Route header. This is
**				used in FormRequest to set the Request-URI.
**				It first checks the header list of the overlap
**				transinfo structure, and if that does not have a
**				Route list, it checks the callObject's processed
**				header list.
**	 			
**				Note:
**				This function returns success even if a Route header  
**				was not found in the call object.
**
********************************************************************/
 Sdf_ty_retVal sdf_fn_uaRetrieveAddrSpecFromTopmostRoute _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, SipAddrSpec **ppAddrSpec, Sdf_st_error *pError));

/******************************************************************
** FUNCTION: sdf_fn_uaGetMethodNameAsEnum
**
** DESCRIPTION:  This function returns the enum value for a  method 
**
** PARAMETERS:
**		pMethod (IN)	: SIP Method as string
**		pType(IN/OUT)	: Method returned as the enum
**		pError(IN/OUT)	: Error structure in case the function fails
******************************************************************/
Sdf_ty_retVal sdf_fn_uaGetMethodNameAsEnum
	_ARGS_((const Sdf_ty_s8bit *pMethod, Sdf_ty_messageType *pType, \
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION: sdf_fn_uaCheckSdpInInvite
**
** DESCRIPTION:  This function sets the variable dSdpInInvite inside
**				 the commonInfo structure if pMessage is a INVITE 
**				 request containing a SDP message body.
**
******************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckSdpInInvite _ARGS_((Sdf_st_callObject *pObject, \
	Sdf_st_msgInfo	dMessageInfo,SipMessage *pMessage, Sdf_st_error *pError));

#ifdef SDF_TRACE
/******************************************************************************
 ** FUNCTION: 	  sdf_fn_uaPrintStateChangeTrace
 **
 ** DESCRIPTION:  This function prints a trace indicating the state
 **				  change. Used by sdf_ivk_uaChangeCallState
 **				  internally
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaPrintStateChangeTrace 
	_ARGS_((Sdf_ty_s8bit *pCallid, Sdf_ty_state dCurrState, \
	Sdf_ty_state dNextState, Sdf_ty_s8bit *pTypeOfState, Sdf_st_error *pError));
#endif	

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaPerformHeaderListCopyByType
 **
 ** DESCRIPTION: This function copies a headerlist from src to dest
 **				 (by reference) and then deletes the headerlist 
 **				 from the source if the flag dDeleteAfterCopy is 
 **				 passed as Sdf_co_true.
 **
 **				 Note:
 **				 The FIRST match of the type same as dType is 
 **				 copied (and deleted from source, if that's 
 **				 desired).
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaPerformHeaderListCopyByType
	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	 en_HeaderType dType, Sdf_ty_bool dDeleteAfterCopy, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaPerformHeaderListCopyByCorrelation
 **
 ** DESCRIPTION: This function copies a headerlist from src to dest (by 
 **				 reference) and then deletes the headerlist from the source
 **				 if the flag dDeleteAfterCopy is passed as Sdf_co_true.
 **
 **				 Note:
 **				 The FIRST match with the correlation same as the correlation
 **				 passed to this API is copied (and deleted from source, if 
 **				 that's desired).
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaPerformHeaderListCopyByCorrelation
	_ARGS_((Sdf_ty_slist *pslSrcHeaderList, Sdf_ty_slist *pslDestHeaderList, \
	 Sdf_ty_s8bit *pCorrelation, Sdf_ty_bool dDeleteAfterCopy, \
	 Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaChangeCallStateRequest
 **
 ** DESCRIPTION: This function changes the call state of the call object
 **				 for incoming/outgoing SIP request messages
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaChangeCallStateRequest \
	_ARGS_((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, SipMessage *pSipMsg, Sdf_ty_msgMode dMode, \
	Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaChangeCallStateResponse
 **
 ** DESCRIPTION: This function changes the call state of the call object
 **					for incoming/outgoing SIP response messages
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaChangeCallStateResponse \
	_ARGS_((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo \
	*pOverlapTransInfo, SipMessage *pSipMsg, Sdf_ty_msgMode dMode, \
	Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCloneSipHeadersList
 **
 ** DESCRIPTION: This function clones a list of SipHeader structures from
 **					the source list to the destination list
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSipHeadersList \
	_ARGS_((Sdf_ty_slist *pDestList, Sdf_ty_slist *pSrcList, \
	Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaConvertHexEncodedString
 **
 ** DESCRIPTION: This function takes a string as input and converts any
 **					characters encoded using % HEX HEX to their equivalent
 **					character
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaConvertHexEncodedString _ARGS_((Sdf_ty_s8bit *pInput, \
	Sdf_ty_s8bit *pOutput,Sdf_ty_u32bit dLen));

/******************************************************************************
 ** FUNCTION: 	 	sdf_fn_uaCompareUserAndHost
 **
 ** DESCRIPTION: 	This function takes 2 addr spec structures and compares 
 **					the user and host part of their URLs
 **
 ** RETURN VALUES:	0 - The host name and user name match
 **					1 - The host name and user name don't match.
 **
 ******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareUserAndHost _ARGS_((SipAddrSpec *pAddrSpec1, \
	SipAddrSpec* pAddrSpec2, Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIssueTransactionCallback
 **
 ** DESCRIPTION: This function issues the sdf_cbk_uaTransactionReceived
 **					callback to the application. Used to remove code
 ** 				duplication.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaIssueTransactionCallback \
 	_ARGS_((Sdf_st_callObject **ppCallObj, Sdf_st_eventContext *pEventContext,\
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_fn_getTransactionTypeFromMsg
**
** DESCRIPTION: Based on the Incoming message and direction of flow, 
**				it decides what kind of Transaction this belogns.
**
******************************************************************************/
Sdf_ty_transactionType sdf_fn_getTransactionTypeFromMsg \
	_ARGS_((SipMessage*, Sdf_ty_bool, Sdf_st_error*));


/*****************************************************************************
** FUNCTION: sdf_fn_getTransactionTypeFromMethod
**
** DESCRIPTION: Based on the method, type and direction of message it 
**				returns the type of Transaction this belogns.
**
******************************************************************************/
Sdf_ty_transactionType sdf_fn_getTransactionTypeFromMethod
	_ARGS_((Sdf_ty_s8bit*, en_SipMessageType, Sdf_ty_bool, Sdf_st_error*));


/*****************************************************************************
** FUNCTION: sdf_fn_getTransactionType
**
** DESCRIPTION: Based on the Incoming message it decides what kind of 
**				Transaction this belogns.
**
******************************************************************************/
Sdf_ty_transactionType sdf_fn_getTransactionType
	_ARGS_(( SipMessage *pSipMsg, Sdf_st_msgInfo dMessageInfo, \
			Sdf_ty_bool dIsIncMsg,  Sdf_st_error *pError));
 
/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaFormToAndFromHeaders
 **
 ** DESCRIPTION: This function is used to make the From and To headers and 
 **				 add the same to the SipMessage passed as input. Internally
 **				 the function adds a From tag to the callobject if it is 
 **				 the first request being made in this call-leg (i.e. we are 
 **				 originating a new request) or adds a To tag to the call 
 **				 object if it is the first response of a call leg that is 
 **				 being accepted.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormToAndFromHeaders \
	_ARGS_((Sdf_st_callObject *pObject, SipMessage *pMessage, \
	Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION:  	sdf_fn_uaCheckMessageValidity
 **
 ** DESCRIPTION: Checks if the message received will cause a valid state
 **				transition for the call from it's present state. Also does 
 **				certain	validations (like incrementing remote Cseq) and 
 **				responds with SIP error responses if the message is invalid.
 **
 ** PARAMETERS:
 **	pOriginalCallObj(IN) 	: Existing call object against which the message
 **								is checked.
 **	pNewCallObj(IN)		 	: New call object containing the decoded message
 **	pErr(OUT)				: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_messageValidity sdf_fn_uaCheckMessageValidity _ARGS_\
	((Sdf_st_callObject *pOriginalCallObj, Sdf_st_callObject *pNewCallObj, \
	Sdf_st_msgInfo dMessageInfo, SipMessage *pIncMessage, \
	Sdf_ty_transactionType	dTxnType, 	Sdf_st_eventContext *pEventContext, \
   	Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_fn_uaRestoreOldState
**
** DESCRIPTION: Internal function to restore state if an error condition
**				occurs after change of call state.
** PARAMETERS :
**	pCallObj(IN): The call object whose call state is to be restored.
**	dOldState(IN): The state of the call before ChangeCallState was done.
**
******************************************************************************/
#ifdef SDF_THREAD_SAFE
Sdf_ty_retVal sdf_fn_uaRestoreOldState _ARGS_((Sdf_ty_state *pState, \
	Sdf_ty_mutexIdT *pMutex, Sdf_ty_state dOldState));
#else
Sdf_ty_retVal sdf_fn_uaRestoreOldState _ARGS_((Sdf_ty_state *pState, \
	Sdf_ty_state dOldState));
#endif
/*****************************************************************************
** FUNCTION: sdf_fn_uaRestoreOldMediaState
**
** DESCRIPTION: Internal function to restore Media state if an error condition
**				occurs after change of  Media state.
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaRestoreOldMediaState
_ARGS_((Sdf_st_callObject	*pCallObj, 
Sdf_ty_mediaState	dOldMediaState));

/******************************************************************************
** FUNCTION: sdf_fn_uaGetHeaderListAsStrings
**
** DESCRIPTION: This function returns the list of headers as 
**	            a list of strings.
**
** PARAMETERS:
**	slHdrList(IN)			: List of header structures
**	pslStrList(IN/OUT)		: List of string forms of the headers
**	pErr(OUT)				: Error structure in case the function fails
*******************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetHeaderListAsStrings _ARGS_((SipMessage \
		*pSipMessage, en_HeaderType dType, Sdf_ty_slist *pslStrList, \
		Sdf_st_error *pErr));

/******************************************************************************
** FUNCTION: sdf_fn_uaCompareSipHeaders
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaCompareCallObjectKeys to compare two keys strictly
**				on basis of {(From:From)(To:To)} OR {(From:To)(To:From)}
** 
**
*******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCompareSipHeaders _ARGS_ ((Sdf_st_callObjectKey \
	*pNewKey, Sdf_st_callObjectKey *pExistingKey, \
	Sdf_ty_callLegMatch *pCallLegMatch, Sdf_ty_bool dToTagCheck, \
	Sdf_st_error *pErr));

#ifdef SDF_TEL
/******************************************************************************
** FUNCTION: sdf_fn_uaCompareSipTelAddrSpec
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaValidateUri to compare a Tel Url and a sip Url which
**				has a tel url embedded in it.
** PARAMETERS:
**	pReqAddrSpec(IN)	: AddrSpec which has the Tel Url embedded in it.
**	pLocalAddrSpec(IN)	: AddrSpec Which from the From Header or the Contac hdr.
**	pError(OUT)			: Error structure in case the function fails

*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareSipTelAddrSpec _ARGS_ ((SipAddrSpec \
				*pReqAddrSpec, SipAddrSpec *pLocalAddrSpec,\
				Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_fn_uaCompareTelUri
**
** DESCRIPTION: This function is used to compare two Tel Address Specs. 
**				If both Tel Url's are same then return 0 else non zero.
**	PARAMATERS:
**	pTelUrl1 (IN)	:	First Tel Url input
**	pTelUrl2 (IN)	:	Second Tel Url Input
**	pError(OUT)		:	Error structure in case the function fails.
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareTelUri  _ARGS_ (( TelUrl  *pTel1, \
		TelUrl	*pTel2, Sdf_st_error *pError));

#endif



/******************************************************************************
** FUNCTION: sdf_fn_GetBranchFromViaHdr
**
** DESCRIPTION: This function returns the Via branch present 
**	            in the Via header.
**
** PARAMETERS:
**	SipHeader(IN)			: Sip header
**	ppBranch(IN/OUT)		: Branch Param present in Via header
**	pErr(OUT)				: Error structure in case the function fails
*******************************************************************************/
Sdf_ty_retVal sdf_fn_GetBranchFromViaHdr _ARGS_((SipHeader *pHeader, \
		SIP_S8bit **ppBranch, Sdf_st_error *pErr));

/******************************************************************************
** FUNCTION: sdf_fn_InsertBranchFromViaHdr
**
** DESCRIPTION: This function inserts the Via branch into Via header
**
** PARAMETERS:
**	SipViaHeader(IN/OUT)	: Sip Via header
**	pBranch(IN)				: Branch Param to be inserted
**	pErr(OUT)				: Error structure in case the function fails
*******************************************************************************/
Sdf_ty_retVal sdf_fn_InsertBranchFromViaHdr _ARGS_((SipHeader *pViaHeader, \
		SIP_S8bit *ppBranch, Sdf_st_error *pErr));


/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckRequestLoop
**
** DESCRIPTION: Checks if the message received is looped back message or
**				not. If Incoming request has same To, From, CallId and
**				CSeq received previously and Via branch differs then
**				that message is treated as looped back message
** PARAMETERS:
**	pSipMessage(IN)			: Incoming sip message
**	pOriginalCallObj(IN)	: Orignal CallObject found on matching key
**	pNewCallObj(IN)			: New Call Object on which Request Received
**	dMessageInfo(IN)		: Information like type and method
**	pErr(OUT)				: Error structure in case the function fails
******************************************************************************/
Sdf_ty_bool sdf_fn_uaCheckRequestLoop _ARGS_((SipMessage	*pIncomingMsg, \
	Sdf_st_callObject *pOriginalCallObj, Sdf_st_callObject *pNewCallObj, \
	Sdf_st_msgInfo   dMessageInfo, Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaRejectRequest
 **
 ** DESCRIPTION: This function has to be invoked to send a failure response to 
 **				 a recd request if any validation prior to 
 **				 sdf_ivk_uaUpdateCallDetails fails. The call object passed to 
 **				 this API is the temporary call object returned by 
 **				 sdf_ivk_uaDecodeMessage().
 **
 ***************************************************************************/
Sdf_ty_retVal sdf_fn_uaRejectRequest
	_ARGS_((Sdf_st_callObject *pTempCallObj, Sdf_st_initData *pInitData, \
	 Sdf_ty_u16bit dRespCode, SipMessage **ppSipMsg,Sdf_ty_transactionType \
 	 transType, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSetOverlapInfoCancelled
 **
 ** DESCRIPTION: This API is used to make all the overlap transactions state
 ** 			 to cancelled state. So that all the processing transactions
 **				 over this terminated call object will get 4xx response.
 **
 **	pCallObject : Call object on which BYE received.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_fn_uaSetOverlapInfoCancelled
	_ARGS_(( Sdf_st_callObject *pCallObj, Sdf_st_error *pError));


/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckToTagValidity
**
** DESCRIPTION: Checks if the message received has valid To tag value. If 
**				Original call object has To tag and incoming request has no
**				To tag then return Sdf_co_true.  The Sfd_co_true indicates 
**				that To Tag check is failed.  ped back message
** PARAMETERS:
**	pOriginalCallObj(IN)	: Orignal CallObject found on matching key
**	pNewCallObj(IN)			: New Call Object on which Request Received
**	pErr(OUT)				: Error structure in case the function fails
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckToTagValidity _ARGS_(( \
	Sdf_st_callObject *pOriginalCallObj, Sdf_st_callObject *pNewCallObj, \
	Sdf_st_msgInfo dMessageInfo, Sdf_st_eventContext *pEventContext, \
   	Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_fn_uaIsCancelMatching
**
** DESCRIPTION: This function matches the CANCEL UAS transaction
**		in the pNewCallObj with the INVITE UAS transaction
**		in pExistingCallObj based on the transaction
**		matching procedures described in Section 17.2.3
**		of RFC 3261 for a CANCEL request.
**		It returns Sdf_co_true if the CANCEL matches
**		the INVITE and Sdf_co_false otherwise.
**
** PARAMETERS:
**	pNewCallObj(IN)		: New call object on which request was received
**	pExistingCallObj(IN)	: Orignal call object found on the matching key
**	pErr(OUT)		: Error structure in case the function fails
******************************************************************************/
Sdf_ty_bool sdf_fn_uaIsCancelMatching _ARGS_(( \
	Sdf_st_callObject 	*pNewCallObj,
	Sdf_st_callObject 	*pExistingCallObj,
	Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaSipDecode _ARGS_(( \
	Sdf_st_callObject *pCallObj, \
	 Sdf_ty_s8bit *pBuffer,	 SipMessage **ppMsg,	 SipOptions  *pOptions,
	 Sdf_ty_u32bit dLength,	 Sdf_ty_s8bit **ppNextBuffer,
	 Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaHandleBadHeaders _ARGS_(( \
	SipMessage  	*pMsg, Sdf_ty_slist	*pslNonMandatoryBadHeaders, \
	  Sdf_ty_bool	*pSdpLineError, Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaExtractKeyInfo _ARGS_(( \
	Sdf_st_callObject	*pCallObj, SipMessage  	*pMsg,	\
	  en_SipMessageType dReqRespType,  Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaCorrectCseq
	_ARGS_(( Sdf_ty_s8bit *pMethod, SipMessage  *pMsg, Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaValidateMessage
	_ARGS_(( SipMessage *pMsg, en_SipMessageType dReqRespType,
	 Sdf_ty_messageType dMethodType,  Sdf_st_error *pErr));

Sdf_ty_matchMessageType sdf_fn_uaAddViaRecdParam
	_ARGS_(( SipMessage  *pMsg, Sdf_st_eventContext *pEventContext,
	  Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaGetMessageInfo
  _ARGS_((SipMessage *pMsg, Sdf_st_msgInfo *pMessageInfo, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaValidateRequireAndSupported
	_ARGS_(( Sdf_st_callObject	*pCallObj,
	  Sdf_ty_transactionType	dTxnType,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_st_overlapTransInfo *pOverlapTransInfo,
	  Sdf_st_error 		*pErr));

Sdf_ty_retVal sdf_fn_uaValidateCseqValue
	_ARGS_(( SipMessage	*pSipMsg, Sdf_st_error 	*pErr));

#ifdef SDF_SESSION_TIMER
Sdf_ty_retVal sdf_fn_uaUpdateSessionInfoFromMsg _ARGS_(( \
	Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo *pOverlapTransInfo,\
	SipMessage	*pSipMsg, Sdf_st_msgInfo dMessageInfo, \
	Sdf_ty_state  dCallState, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));
#endif

Sdf_ty_retVal sdf_fn_uaUpdateToTag
	_ARGS_(( Sdf_st_callObject *pCallObj, SipMessage *pSipMsg, \
	  Sdf_ty_bool	*pIsToTagPresent,  Sdf_st_error  *pErr));

Sdf_ty_retVal sdf_fn_uaFormAndSendResponse _ARGS_( \
	(Sdf_st_callObject *pCallObj, SipMessage **ppMessage, \
	Sdf_ty_u16bit	dRespCode, Sdf_st_eventContext *pEventContext, \
	Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaHandleInvite
	_ARGS_(( Sdf_st_callObject **ppCallObj, SipMessage *pMessage,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_ty_state dCallState, Sdf_st_error 	*pErr));

Sdf_ty_retVal sdf_fn_uaGetOverlapTransInfo
	_ARGS_(( Sdf_st_callObject			*pCallObj,
	  SipMessage				*pMessage,
	  Sdf_st_overlapTransInfo 	**ppOverlapTxnInfo,
	  Sdf_ty_u16bit				dStatusCode,
	  Sdf_ty_transactionType	dTransactionType,
	  en_SipMessageType 		dType,
	  Sdf_ty_messageType 		dMethodType,
	  Sdf_st_error 				*pErr));

Sdf_ty_messageValidity sdf_fn_uaUpdateTempCallObject
	_ARGS_(( Sdf_st_callObject	*pTmpCallObj, Sdf_st_callObject *pOrgCallObj,
	  SipMessage *pMessage, Sdf_st_msgInfo	dMessageInfo, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaHandleAck
	_ARGS_(( Sdf_st_callObject **ppCallObj, SipMessage  *pMessage,
	  Sdf_st_eventContext *pEventContext,
	  Sdf_ty_state 	dCallState, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaHandleProvResp
	_ARGS_(( Sdf_st_callObject **ppCallObj, SipMessage *pMessage,
	  Sdf_st_overlapTransInfo *pOverlapTxnInfo, \
	  Sdf_st_eventContext	*pEventContext, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaHandleInvFinalResp
	_ARGS_(( Sdf_st_callObject **ppCallObj, SipMessage *pMessage,
	  Sdf_ty_u16bit dStatusCode, Sdf_ty_state dCallState,
	  Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaCheckGlareCondition
	_ARGS_(( Sdf_st_callObject *pCallObj, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaEstablishChannel
	_ARGS_(( Sdf_st_callObject *pCallObj,
	  Sdf_st_overlapTransInfo	*pOverlapTransInfo,
	  en_SipMessageType	 dReqRespType, Sdf_ty_messageType dMethodType,
	  Sdf_st_error 		 *pError));

Sdf_ty_retVal sdf_fn_uaSendSipMsgOverNetwork
	_ARGS_(( Sdf_st_callObject 	*pCallObj,Sdf_st_transaction *pTransaction,\
	   	Sdf_st_overlapTransInfo	*pOverlapTransInfo, SipMessage	*pMessage, \
	  	en_SipMessageType		dReqRespType,	\
		Sdf_st_eventContext 	*pEventContext, SipOptions 	*pOptions, \
		SipTranspAddr	*pTranspaddr, Sdf_ty_s8bit transptype, \
		Sdf_st_error 	*pError));

#ifndef SDF_TXN
Sdf_ty_retVal sdf_fn_uaPopulateRREntry
	_ARGS_(( Sdf_st_callObject 	*pCallObj, SipMessage *pMessage,
	  Sdf_st_transportInfo 	*pDestInfo, Sdf_ty_socketId 		dSockfd,
	  SipOptions 			dOptions,  Sdf_st_eventContext	*pEventContext,
	  Sdf_st_msgInfo		dMessageInfo, Sdf_st_error 			*pErr));
#endif

/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaInsertHeaderFromList
 **
 ** DESCRIPTION:	This function  inserts header from the header list to the
 **					Outgoing  message and specially takes care of inserting
 **					Route header( in case of strict router)
 **
 ** PARAMETERS:
 **		pMessage(IN/OUT)  : The SipMessage structure where the hedaer will 
 **							be insert
 **						 
 **		pHeaderList(IN)   : Header list where from header will be insert
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaInsertHeaderFromList _ARGS_(( \
	SipMessage *pMessage,Sdf_st_headerList *pHeaderList,\
	Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaCheckLrParameter
 **
 ** DESCRIPTION:	This function  checks whether lr parameter is present or not
 ** 				in the Addrspec
 **
 ** PARAMETERS:
 **		pAddrspec(IN)  : The SipAddrSpec structure which will be verify for
 **						 presence of lr parameterer
 **						 
 **		pIsPresentLr(OUT)   : Wiil set to true if lr parameter is present in the 
 **								Addrspec
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaCheckLrParameter _ARGS_((SipAddrSpec *pAddrspec,\
	Sdf_ty_bool  *pIsPresentLr, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaConverHexToChar
 **
 ** DESCRIPTION:	This function  converts the hex string preceded by % to
 **					 ascii string
 **
 ** PARAMETERS:
 **		ppHexString(IN/OUT)  : The String which will be converted to ascii
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaConverHexToChar  _ARGS_((Sdf_ty_s8bit *pHexString,\
	Sdf_st_error *pErr));



/******************************************************************************
 ** FUNCTION:  		sdf_fn_uaInsertHeaderFromProfile
 **
 **
 ** DESCRIPTION: This function inserts the ACCEPT, ACCEPT-ENCODING and
 **			   	ACCEPT-LANGUAGE header from the profile to the response
 **				 message(415 and 200 OK for OPTIONS)
 **
 ** PARAMETERS:
 **		pMesg(IN/OUT)  : The SipMessage structure where the hedaer will be 
 **						insert
 **						 
 **	    pConfigParam(IN)   : Header config param structure
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaInsertHeaderFromProfile  _ARGS_((\
			Sdf_st_configExtraHeaders *pConfigParam,SipMessage *pMesg,\
			Sdf_st_error *pErr));

#ifdef SDF_AUTHENTICATE
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaUpdateAuthorization
 **
 ** DESCRIPTION: This function updates the authorization header present in the
 **				 message by the Next nonce count value stored in the Auth-Info 
 **				 header . Also Updates the authorization header list stored in
 **				 the Persistent	 header list of the call object. 
 **
 ** PARAMETERS:
 **		pCallObj(IN)  : The call object structure where it will take the list of
 **						Authorization header from the Persistent header list
 **						 
 **	    pSipMsg(IN/OUT)   : SipMsg where the Authorization header will be
 **							updated
 **	    pMethod(IN)   : The method parameter 
 **
 **		pErr(OUT)	  :	Error structure in case the function fails
 **
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateAuthorization  _ARGS_((\
			Sdf_st_callObject *pCallObj, SipMessage *pSipMsg,\
			const Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));
#endif

Sdf_ty_messageValidity sdf_fn_uaValidateViaAddr _ARGS_( \
	(Sdf_st_callObject *pCallObj, SipMessage *pMessage, Sdf_st_error *pErr));

Sdf_ty_messageValidity sdf_fn_uaValidateResponseState
	_ARGS_(( Sdf_st_callObject 	*pCallObj, SipMessage 	*pMessage,
	  Sdf_st_msgInfo	dMessageInfo, Sdf_st_error	*pErr));

Sdf_ty_retVal	sdf_fn_uaGetMatchingOverlapTransaction
	_ARGS_((Sdf_st_callObject 	*pCallObj, 
			Sdf_st_overlapTransInfo **ppTransInfo,
			Sdf_ty_messageType  dMsgType, Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaCompareFullURI
	_ARGS_(( SipHeader	*pNewHdr, SipHeader *pExistHdr, \
			Sdf_ty_bool dIsTagMatched, Sdf_ty_callLegMatch *pCallLegMatch, \
		    Sdf_st_error  *pError));

Sdf_ty_retVal sdf_fn_uaStoreRequireHdr
	_ARGS_(( Sdf_st_callObject	*pCallObj, SipMessage	*pMessage,
	  Sdf_st_error *pError));


/*****************************************************************************
** FUNCTION: sdf_fn_uaGetAllowHeaderList
**
** DESCRIPTION: This function returns the ALLOW header list.
**				The list is extracted from profile(InitData), if present,
**				Otherwise UATK sets all the methods it supports,
**				by default, into the header list.
** PARAMETERS:
**	pCallObj(IN)			: CallObject to operate upon.
**	pInitData(IN)			: Profile that [may] contain ALLOW header
**	ppAllowHdrList(OUT)		: The extracted/created ALLOW header
**	pErr(OUT)				: Error structure in case the function fails
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetAllowHeaderList( \
		Sdf_st_callObject *pCallObj, Sdf_st_initData *pInitData, \
		Sdf_st_headerList **ppAllowHdrList, Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_fn_uaCallState
**
** DESCRIPTION: This function checks the current call state and returns
**				Sdf_en_earlyDialogState if the call establishment in progress,
**				Sdf_en_confirmedDialogState if the call is in established state &
**				Sdf_en_invalidDialogState if the call is in other states.
** PARAMETERS:
**	pCallObj(IN)			: Call Object whos call state has to be checked
******************************************************************************/
Sdf_ty_callState sdf_fn_uaCallState(Sdf_st_callObject *pCallObj);


/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaMessageSentNtfn 
 **
 ** DESCRIPTION:	Notification function invoked by the application after
 **                 sending the SIP message on the network.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaMessageSentNtfn _ARGS_(\
	(Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo *pOverlapTransaction,\
	 SipMessage *pSipMsg, Sdf_st_msgInfo dMessageInfo, \
	 Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaSendMessageProcs
 **
 ** DESCRIPTION: This function does the procedures that needs to be done before
 **              sending a message on the n/w.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaDoSendMessageProcs _ARGS_(\
	(Sdf_st_callObject *pObject, SipMessage *pSipMsg, \
	 Sdf_st_overlapTransInfo **ppOverlapTransaction, \
	 Sdf_st_msgInfo dMessageInfo, Sdf_st_error *pErr));
/*****************************************************************************
** FUNCTION: sdf_fn_uaInitMediaStateValidationTable
**
** DESCRIPTION: This API will initialize the Media State Validation Funciton
**				Pointer table.
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitMediaStateValidationTable _ARGS_((void));

/*****************************************************************************
** FUNCTION: sdf_fn_uaCheckMediaStateValidity
**
** DESCRIPTION: This API will check the Media State
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaCheckMediaStateValidity
 _ARGS_((Sdf_st_callObject *pOrigCallObj , Sdf_st_callObject *pNewCallObj,
 Sdf_ty_messageType	dMessageType, Sdf_st_error *pErr));


/*****************************************************************************
** FUNCTION: sdf_fn_uaMuteMediaStream
**
** DESCRIPTION: Mute the given MediaStream based on the type of Mute reqd.
** PARAMETERS:
**	pMediaStream(IN/OUT)	: The media stream that has to be muted.
**	dType(IN)		: Type of mute required. If dType is 
**					  Sdf_en_connectionMute, then the IP addresses in all the 
**					  c-lines in the media stream will be set to 0.0.0.0 . Else,**					  if dType is Sdf_en_holdSelfMedia, then the direction 
**					  attrib. will be set to Sdf_en_Recvonly. Else, if dType is
**					  Sdf_en_completeMute, then attrib will be set to 
**					  Sdf_en_Inactive, else it will be set to Sdf_en_Sendonly.
**	pError(OUT)		: Error structure in case the function fails
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaMuteMediaStream _ARGS_(( \
	Sdf_st_mediaStream *pMediaStream, Sdf_ty_muteType dType,\
	Sdf_st_error *pError));


/*****************************************************************************
** FUNCTION: sdf_fn_uaUnMuteMediaStream
**
** DESCRIPTION: UnMute the given MediaStream.
** PARAMETERS:
**	pMediaStream(IN/OUT)	: The media stream that has to be muted.
**  dType(IN)		: Enum explaining how the MediaStream was Muted.
**	pError(OUT)		: Error structure in case the function fails
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUnMuteMediaStream _ARGS_(( \
	Sdf_st_mediaStream *pMediaStream,  Sdf_ty_muteType dType,\
	Sdf_st_error *pError));


Sdf_ty_retVal sdf_fn_uaFormRequestLine _ARGS_((Sdf_st_callObject *pCallObj, \
	Sdf_st_overlapTransInfo	*pOverlapTransInfo, const Sdf_ty_s8bit *pMethod, \
	SipMessage *pSipMessage, Sdf_st_msgInfo dMessageInfo,Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaFormPreloadedRouteSet
	_ARGS_((Sdf_st_callObject  *pCallObj, Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaFormViaHeader _ARGS_((Sdf_st_callObject  *pCallObj,
	Sdf_st_overlapTransInfo	*pOverlapTransInfo, Sdf_ty_messageType dMethodType,
	SipMessage	*pSipMessage, Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaCopyRouteToMessage _ARGS_((Sdf_st_callObject *pCallObj,\
	SipMessage *pSipMsg, Sdf_st_error  *pError));

Sdf_ty_retVal sdf_fn_uaCorrectRouteHeader _ARGS_( \
	(Sdf_st_callObject	*pCallObj, SipMessage	*pSipMessage,\
	Sdf_st_error       *pError));

Sdf_ty_retVal sdf_fn_uaStoreRouteHeader _ARGS_((Sdf_st_callObject *pCallObj, \
	SipMessage	*pSipMessage,  Sdf_st_error   *pError));

Sdf_ty_retVal sdf_fn_uaFormCseqHeader _ARGS_((Sdf_st_callObject  *pCallObj, \
	Sdf_st_overlapTransInfo	*pOverlapTransInfo, SipMessage	*pSipMessage, \
	const Sdf_ty_s8bit *pMethod, Sdf_st_error       *pError));

Sdf_ty_retVal sdf_fn_uaFormRackHeader _ARGS_((Sdf_st_callObject  *pCallObj, \
	Sdf_st_overlapTransInfo	*pOverlapTransInfo, SipMessage	*pSipMessage, \
	Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaFormRequireHdr _ARGS_((Sdf_st_callObject  *pCallObj, \
	SipMessage *pSipMessage, Sdf_st_error *pError));

Sdf_ty_retVal sdf_fn_uaUpdateContactInRouteSet _ARGS_( \
	(Sdf_st_callObject *pCallObj, SipMessage *pSipMessage, Sdf_st_error *pErr));

Sdf_ty_retVal sdf_fn_uaHandleContactParams _ARGS_((SipMessage *pSipMessage, \
	SipHeader  *pContact, Sdf_st_error  *pError));

Sdf_ty_retVal sdf_fn_uaInsertRequireHdr _ARGS_((SipMessage	*pSipMessage,
	 Sdf_st_initData	*pInitData, Sdf_st_error       *pError));

Sdf_ty_retVal sdf_fn_uaAddSupportedHdrToSipMsg _ARGS_((SipMessage *pMesg,\
	 Sdf_st_initData *pInitData, Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetPointerTo
 **
 ** DESCRIPTION:	This function returns a pointer to the SIP header which 
 **					starts with the string present in 'pSrchStr'.
 **					If "*" is passed as 'pSrchStr', then it returns the 
 **					next line.
 **					This function identifies next header by the presence of
 **					"\r\n" at the end of every line. If the SIP message
 **					is just separated by "\n", it is not considered as next header.
 *****************************************************************************/
Sdf_ty_s8bit *sdf_fn_uaGetPointerTo(Sdf_ty_s8bit *pStr, Sdf_ty_s8bit \
		*srchStr, Sdf_st_error *pError);

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetCurrentLineLength
 **
 ** DESCRIPTION:	This function splits the given string into "Name" "Value"
 **					pair (Separator used is ":" (colon-for SIP headers) or 
 **					"=" (equal to-for SDP headers). This function returns
 **					the length of string contained in "Value"
 *****************************************************************************/
Sdf_ty_u32bit sdf_fn_uaGetCurrentLineLength(Sdf_ty_s8bit *pTempStr, \
		Sdf_st_error *pError);

 /***********************************************************************
** FUNCTION:  sdf_ivk_uaMuteSession
**
** DESCRIPTION: This function mutes the complete session 
**
** PARAMETERS:
**		pSessionParams(IN)	: Session Param.
**      dType				: Enum explaining the type of Mute required.
**							  If it is Sdf_en_connectionMute, then the IP addr
**							  in c-line will be set to 0.0.0.0 . If it is
**							  Sdf_en_holdSelfMedia, then direction attr will be
**							  set to Sdf_en_holdSelfMedia. If it is 
**							  Sdf_en_completeMute, then direction attr will be 
**							  set to Sdf_en_Inactive else the direction attr 
**							  will be set to Sdf_en_Sendonly.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_uaMuteMediaSession  _ARGS_((\
    Sdf_st_sessionParam *pSessionParams, 
	Sdf_ty_muteType dType, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaUnMuteMediaSession
**
** DESCRIPTION: This function UnMutes the SessionParam by
**              either restoring the c= line to prev IP addr or by restoring 
**              the direction attribute, depending on dType.
**
** PARAMETERS:
**		pSessionParams(IN)	: The Session Param.
**      dType				: Enum explaining how the MediaStream was Muted.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_fn_uaUnMuteMediaSession  _ARGS_((\
	 Sdf_st_sessionParam *pSessionParams,\
	 Sdf_ty_muteType dType, Sdf_st_error *pErr));


/****************************************************************************
** FUNCTION: sdf_fn_uaEscapeCharacters
**
** DESCRIPTION: This function is used to escape a certain set of characters
**		Some ASCII characters are converted to their HEX equivalents.
**
** PARAMETERS:
**		dInputStr(IN)	: Input string, whose special characters 
**							should be escaped.
**		pErr(OUT)		: Error structure in case the function fails
**
****************************************************************************/
Sdf_ty_s8bit* sdf_fn_uaEscapeCharacters (Sdf_ty_s8bit* dInputStr, \
		Sdf_st_error* pErr);

#ifdef SDF_REFER
/****************************************************************************
** FUNCTION: sdf_fn_uaConvertHexToChar
**
** DESCRIPTION: This function is used to convert a hex number (presented in
**		first 2 bytes of input argument) into decimal digit.
**
** PARAMETERS:
**		pHexInput(IN)	: Input hex value.
**
****************************************************************************/
Sdf_ty_s8bit sdf_fn_uaConvertHexToChar (Sdf_ty_s8bit *pHexInput);

/****************************************************************************
** FUNCTION: sdf_fn_uaUnescapeCharacters
**
** DESCRIPTION: This function is used to un=escape a certain set of characters
**		i.e. the HEX numbers are converted to their ASCII equivalents.
**
** PARAMETERS:
**		pUrl(IN/OUT)	: String which has escaped characters. After 
**							conversion, this string is overwritten to
**							contain the correct (un-escaped) string.
**		pErr(OUT)		: Error structure in case the function fails
**
****************************************************************************/
Sdf_ty_s8bit* sdf_fn_uaUnescapeCharacters (Sdf_ty_s8bit* pUrl, \
		Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_fn_uaValidateRefer
**
** DESCRIPTION: This function checks if "Refer-To" header is present in 
**				SipMessage or not.
**				If the header is present, dValidity is set to Sdf_en_valid
**				otherwise, dValidity is set to Sdf_en_invalid
**
** PARAMETERS:
**		pSipMsg(IN)			: SipMsg in which Refer-To header is to be checked
**		pValidity(OUT)		: Indicates whether REFER message is valid
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateRefer(SipMessage *pSipMsg, \
		Sdf_ty_messageValidity *pValidity, Sdf_st_error *pErr);

#endif /* SDF_REFER */

/******************************************************************************
** FUNCTION: sdf_fn_uaInitCallObjectIdGenerator
**
** DESCRIPTION: Initialize the call object id generator.
** PARAMETERS:
**		pError(OUT)			: Error structure in case the function fails
**
*******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitCallObjectIdGenerator _ARGS_((Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_fn_uaGetUniqueCallObjectId
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaInitCall to generate a unique call object	Id.
** PARAMETERS:
**				None
**
*******************************************************************************/
Sdf_ty_u32bit sdf_fn_uaGetUniqueCallObjectId _ARGS_((void));

/*SPR-4629:for subba's changes the tls flag is added to individual fn prototypes*/
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetContactAddrType
 **
 ** DESCRIPTION: This function is used to determine the Address Type for
 **              constructing Contact headers. In case of forming request, 
 **              SipMessage will be the message containing the Request. In 
 **              case of response, SipMessage (taken from UAS txn or Overlap
 **              txn.) will be the incoming request for which the response 
 **              is being formed.
 **
 ** PARAMETERS:
 **		pSipMessage(IN)	: The SIP message for which the Contact headers are to
 **                       be constructed.
 **     pCallObj		: Call object for which the SipMessage is to be sent.
 **		pContactAddrType(OUT) : Where the contact uri type is to be filled.
 **     dReqRespType(IN): Whether the SipMessage is a request or response
 **                       message.
 **     pErr(OUT)		: Error structure in case the function fails.
 **
 ******************************************************************************/
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will
 * come into picture*/
Sdf_ty_retVal sdf_fn_uaGetContactAddrType _ARGS_((\
	 SipMessage *pSipMessage, Sdf_st_callObject  *pCallObj, \
	 Sdf_ty_addrType *pContactAddrType, en_SipMessageType dReqRespType, \
	 Sdf_st_error *pErr));


#else
#ifdef SDF_TLS
Sdf_ty_retVal sdf_fn_uaGetContactAddrType _ARGS_((\
	 SipMessage *pSipMessage, Sdf_st_callObject  *pCallObj, \
	 en_AddrType *pContactAddrType, en_SipMessageType dReqRespType, \
	 Sdf_st_error *pErr));
#endif /* SDF_TLS */
#endif /*End:  ssf specific changes done by subbaroa */

#ifdef SDF_TLS
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaMapCipherName
 **
 ** DESCRIPTION: This function maps a TLS sipher name to corresponding SSL
 **              cipher name. This is required because OpenSSL library
 **              understands only the SSL cipher names.
 ** PARAMETERS:
 **		pTlsCipherStr(IN)	: String containing the TLS cipher suite name.
 **     pSslCipherStr(OUT)	: String that will contain the SSL cipher suite name.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaMapCipherName _ARGS_((Sdf_ty_s8bit *pTlsCipherStr, \
	 Sdf_ty_s8bit *pSslCipherStr,Sdf_ty_u32bit dLen));

#endif /* SDF_TLS */

#ifdef SDF_IM
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCorrectImUrlInRoute
 **
 ** DESCRIPTION: This function is used to Validate the presence of IM uri. 
 **	Scan through all the Route and Record Route headers for IM URL.
 ** If IM URL present in the Route and Record Route headers, try to
 ** convert them to other schemes by issuing the callback to application. 
 **	If conversion is not possible then through error 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCorrectImUrlInRoute _ARGS_((SipMessage *pSipMessage, \
 Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_fn_uaCompareImUrl
**
** DESCRIPTION: This function is used to compare two Im URLs 
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaCompareImUrl _ARGS_((ImUrl *pImUrl1, \
 ImUrl *pImUrl2, Sdf_st_error *pError));

/****************************************************************************
 ** FUNCTION: sdf_fn_uaResolveImUrl
 **
 ** DESCRIPTION: This function resolves a IM URL into the Other schemes.
 **				  In through callback to application to resolve
 **	Paramaters:
 **		pImAddrSpec (IN):The Address spec containing the IM URL to be resolved
 **		ppAddrSpec (IN/OUT): The Address Spec containing the resolved URL
 **						The Memory to this address spec need to be allocated
 **						within this function itself.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_fn_uaResolveImUrl _ARGS_((\
	SipAddrSpec **ppAddrSpec, SipAddrSpec *pImAddrSpec, Sdf_st_error *pErr));
#endif

#ifdef SDF_PRES
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCorrectPresUrlInRoute
 **
 ** DESCRIPTION: This function is used to Validate the presence of Pres uri. 
 **	Scan through all the Route and Record Route headers for Pres URL.
 ** If Pres URL present in the Route and Record Route headers, try to
 ** convert them to other schemes by issuing the callback to application. 
 **	If conversion is not possible then through error 
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCorrectPresUrlInRoute _ARGS_((SipMessage *pSipMessage, \
 Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_fn_uaComparePresUrl
**
** DESCRIPTION: This function is used to compare two Pres URLs 
*******************************************************************************/
Sdf_ty_s8bit sdf_fn_uaComparePresUrl _ARGS_((PresUrl	 *pPresUrl1, \
 PresUrl	 *pPresUrl2, Sdf_st_error *pError));

/****************************************************************************
 ** FUNCTION: sdf_fn_uaResolvePresUrl
 **
 ** DESCRIPTION: This function resolves a PRES URL into the Other schemes.
 **				  In through callback to application to resolve
 **	Paramaters:
 **		pPresAddrSpec (IN):The Address spec containing the PRES-URL 
 **                        to be resolved
 **		ppAddrSpec (IN/OUT): The Address Spec containing the resolved URL
 **						The Memory to this address spec need to be allocated
 **						within this function itself.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_fn_uaResolvePresUrl _ARGS_((\
	SipAddrSpec **ppAddrSpec, SipAddrSpec *pPresAddrSpec, Sdf_st_error *pErr));
#endif

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsTransactionMatched
 **
 ** DESCRIPTION: This function compares the Sip Message against the Cseq and
 **              Via branch passed.  If the via branch and Cseq present in
 **				message matches with that of inputs then return true.
 **
 ******************************************************************************/
Sdf_ty_bool sdf_fn_uaIsTransactionMatched _ARGS_((SipMessage *pSipMsg, \
	Sdf_ty_s8bit *pIncViaBr, Sdf_ty_s8bit	*pCseqMethod, \
	Sdf_ty_u32bit	dCseqNo, Sdf_st_error   *pErr));

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaIsHeaderPresent 
 **
 ** DESCRIPTION:	This function checks for the presence of given header
 **					inside a SipMessage structure.
 **
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaIsHeaderPresent
	(SipMessage	*pSipMsg, en_HeaderType	dHeader, Sdf_st_error *pErr);

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaGetSpecificNonMandatoryBadHeaderError
 **
 ** DESCRIPTION:	This function iterates thru the bad header list and 
 **					returns specific error code w.r.t the headers.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetSpecificNonMandatoryBadHeaderError
	(Sdf_ty_slist	*pslNonMandatoryBadHeaders,Sdf_st_error *pErr);

/******************************************************************************
 ** FUNCTION	: sdf_fn_uaHandleForkedResponse
 **
 ** DESCRIPTION	: API is used to update the temp callobject with the
 **				  forked response message  details. 
 **				  Application can store this permanent in its
 **				  call object aggregation.
 **				  It issues the callback to application about the receipt
 **				  of forked responses.
 **
 ** PARAMETERS:
 **		pForkCallObj(IN): The Call Object to be Updated 
 **		pOrgCallObj(IN) : The Call Object Corresponding to main dialog
 ** 	pEventContext(IN) : The Event Context corresponding to the forked leg.
 **		pError(OUT)		: Error structure in case the function fails
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_fn_uaHandleForkedResponse 
	_ARGS_((Sdf_st_callObject *pForkCallObj, Sdf_st_callObject *pOrgCallObj, \
	 Sdf_st_eventContext *pEventContext, Sdf_st_error *pError));

#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
Sdf_ty_retVal sdf_fn_uaGetTransportParam(SipAddrSpec *pAddrSpec, \
	Sdf_ty_protocol *pTranspType, Sdf_st_error *pError);

#endif /*End:  ssf specific changes done by subbaroa */

Sdf_ty_bool sdf_fn_uaIsContactHdrMandatory
	_ARGS_((en_SipMessageType	dReqRespType, Sdf_ty_messageType dMsgType, \
	 Sdf_ty_u16bit		dResponseCode));

#ifdef SDF_RPORT

/*****************************************************************************
** FUNCTION: sdf_fn_uaPopulateViaRportParam
**
** DESCRIPTION: 
*****************************************************************************/
Sdf_ty_matchMessageType	sdf_fn_uaPopulateViaRportParam(SipMessage *pMsg,\
	 Sdf_st_eventContext *pEventContext, Sdf_ty_u32bit * pIsrportpresent, Sdf_st_error *pErr);
#endif

/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaAddHeaderInPersistentHdrList
 **
 ** DESCRIPTION:	This function stores the given Header into
 **					Persistent hdrList
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAddHeaderInPersistentHdrList
    _ARGS_((Sdf_st_callObject	*pCallObject, SipHeader	*pHeader, \
     en_HeaderType	dType, Sdf_ty_s8bit	*pCorrelation, Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION:		sdf_fn_uaDeleteParamsFromUrl
 **
 ** DESCRIPTION:	This function deletes the params in the url by
 **                 finding the param names and comparing them with
 **                 the list of names passed
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDeleteParamsFromUrl(SipUrl *pSipUrl, \
        Sdf_ty_s8bit **ppParamNameList,Sdf_ty_u32bit dListSize,Sdf_st_error *pError);
#ifdef SDF_3GPP

Sdf_ty_retVal sdf_fn_uaAddHistoryInfoHeader(Sdf_st_callObject *pCallObj,
        SipMessage *pSipMsg,SipAddrSpec *pAddrSpec,Sdf_ty_s8bit *pHistoryIndex,
        Sdf_ty_u32bit dListIndex,Sdf_st_error *pErr);

#endif

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

#endif 
