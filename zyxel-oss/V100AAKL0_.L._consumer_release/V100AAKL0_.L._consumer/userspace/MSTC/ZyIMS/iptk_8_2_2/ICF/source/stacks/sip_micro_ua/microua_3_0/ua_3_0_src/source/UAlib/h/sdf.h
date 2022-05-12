/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
 ** FUNCTION: 
 **			This file has all the call related invocation API's
 **
 *********************************************************************
 **
 ** FILENAME		: sdf.h
 **
 ** DESCRIPTION		: This file has all the call related invocation API's
 **
 ** DATE        NAME				REFERENCE		REASON
 ** ----        ----				---------		--------
 ** 27/10/00	Anitha								Creation
 ** 07/11/00	Prashant							Updated
 ** 29/08/01 	Subhash								Code cleanup, comments,
 **													and correcting error codes
 ** 06/05/09  Rajiv Kumar           SPR 19531       Klocworks fix
 **
 *********************************************************************
 **			COPYRIGHT , Aricent, 2009
 *********************************************************************/
#ifndef __SDF_H_
#define __SDF_H_

#define SIP_HDR_REQ_URI (0x1)
#define SIP_HDR_CALL_ID (0x2)
#define SIP_HDR_FROM (0x4)
#define SIP_HDR_TO (0x8)
#define SIP_HDR_SUPPORTED (0x10)
#define SIP_HDR_REQUIRE (0x20)
#define SIP_HDR_PXY_REQUIRE (0x40)
#define SIP_HDR_ALLOW (0x80)
#define SIP_HDR_RECORD_ROUTE (0x100)
#define SIP_HDR_ROUTE (0x200)
#define SIP_HDR_VIA (0x400)
#define SIP_HDR_ANY (0x80000000)

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetTo
**
** DESCRIPTION: Sets the destination ("To") address for the call in the 
**				call object
**
** PARAMETERS:
** 		pObject(IN/OUT)	: Call Object to be updated
**	 	pName(IN)		: Remote user's display name
**	 	pAddr(IN)		: Remote user's sip address  
**	 	dPort(IN)		: Remote user's receiving port
**	 	pScheme(IN)		: Scheme used for the address (Eg. sip, tel)
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetTo _ARGS_((Sdf_st_callObject *pObject,\
		 Sdf_ty_s8bit *pName, Sdf_ty_s8bit *pAddr, Sdf_ty_u32bit dPort, \
		 Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetTransportSchemeInTransaction
**
** DESCRIPTION: Sets the transport Scheme information for the call. 
**				This may change during the call due to presence 
**				of Contact headers and Record-Routes.
**
** PARAMETERS:
** 		pTransaction(IN/OUT)		: Transaction Object to which the elements
**										are updated
** 		pOverlapTransInfo(IN/OUT)	: OverlaptransactionObject to which 
**										to which the elements are updated
**	 	pScheme(IN)		: Protocol used (Eg. tcp, udp)
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetTransportSchemeInTransaction _ARGS_(\
	(Sdf_st_transaction *pTransaction,Sdf_st_overlapTransInfo\
		*pOverlapTransInfo,Sdf_ty_s8bit *pScheme, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaDecodeMessage
**
** DESCRIPTION: Decodes the received SIP message buffer and sets it in the
**				call object passed as a parameter. It also checks if it is 
**				a remote retransmission, and if so, resends the same response 
**				that was sent earlier.
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: Call object into which the decoded message 
**								is set
**		pBuffer(IN)			: Buffer to be decoded
**		pOptions(IN)		: Options to be used while parsing the text 
**								message. Possible values:
**								SIP_OPT_NOPARSEBODY - Do not parse the 
**									message body. The body is stored as 
**									an unknown body type.
**								0 - Parse the message bodies depending on 
**									the content	type specified in the message.
**		dLength(IN)  		: Input buffer length
**		ppNextBuffer(OUT)	: If the buffer received from the network has 
**								multiple SIP messages, this will hold a pointer
**								to the next SIP message in the buffer
**		pEventContext(IN)	: Sdf_st_eventContext structure
**		pErr(OUT)	 		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_matchMessageType sdf_ivk_uaDecodeMessage _ARGS_\
	((Sdf_st_callObject *pCallObj, Sdf_ty_s8bit *pBuffer, SipOptions *pOptions,\
	Sdf_ty_u32bit dLength, Sdf_ty_s8bit **ppNextBuffer, \
	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


 /*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaProcessDecodedMessage
 **
 ** DESCRIPTION: Processes the decoded SipMessage and fills in the relevant
 **              information in the CallObject, validates the message and
 **              handles remote retransmission if txn layer support is not
 **              present.
 **
 ** PARAMETERS:
 **     pCallObj(IN/OUT)	: Call object into which the decoded message 
 **					          is set.
 **     pMsg(IN)			: The decoded SipMessage which is to be processed.
 **		pEventContext(IN)	: Sdf_st_eventContext structure.
 **     pErr(OUT)	 		: Error structure in case the function fails.
 **
 *****************************************************************************/
 Sdf_ty_matchMessageType  sdf_ivk_uaProcessDecodedMessage _ARGS_\
	 ((Sdf_st_callObject *pCallObj, SipMessage *pMsg,\
	   Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION: sdf_ivk_uaHandleCall 
**
** DESCRIPTION: This function should be invoked on receipt of a new 
**				message that does not map into an existing call. It updates the
**				call object with the details of the received message and issues
**				the appropriate callback to the application.
**
** PARAMETERS:
**	ppCallObj(IN)			: Call Object containing the SIP message received
**	pEventContext(IN)		: Sdf_st_eventContext structure that is passed
**								to all callbacks issued by the toolkit
**	pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaHandleCall _ARGS_((Sdf_st_callObject **ppCallObj,\
 	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION: sdf_ivk_uaCallIdGenerator 
**
** DESCRIPTION: Provides a sample Call-Id generator function. If the
**				application does not wish to write its own Call-Id generator, 
**				this function can be used. It generates Call-Id's of the form
**				"RandomNum-RandomNum"
**
** PARAMETERS:
**	pAppData(IN)	: The application context data which may be used
**						to generate Call-ID's for the call
**
************************************************************************/
 Sdf_ty_s8bit *sdf_ivk_uaCallIdGenerator _ARGS_((Sdf_st_appData \
	*pAppData));



/***********************************************************************
** FUNCTION:  sdf_ivk_uaChangeCallState
**
** DESCRIPTION: Changes the call state of the call object based on the 
**				current state and the incoming/outgoing message
**
** PARAMETERS:
**	pObject(IN) 			: The Call Object
**	pOverlapTransInfo(IN) 	: The overlapTransInfo structure whose state
**								is to be updated. Should be NULL if it is 
**								call/transaction state that is to be updated.
**	pSipMsg(IN)				: SipMessage that was received OR is being sent out
**	dMode(IN)				: Enum indicating whether the message 
**								 is being sent or has just been received
**	pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaChangeCallState _ARGS_\
	 ((Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo,\
	 SipMessage *pSipMsg, Sdf_ty_msgMode dMode, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaChangeRequestURI
**
** DESCRIPTION: Stores the Request URI into the callobject structure 
**
** PARAMETERS:
**	pObject(IN) 			: The Call Object
**	pRequestUri(IN)			: Address Spec for Request URI
**	pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaChangeRequestURI _ARGS_\
	 ((Sdf_st_callObject *pObject, SipAddrSpec *pRequestUri,\
	 Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaUpdateCallDetails
**
** DESCRIPTION: Updates the callobject with the details of the incoming
** 				SipMessage. Also issues the appropriate callback to the 
**				application.
**
** PARAMETERS:
**	ppOriginalCallObj(IN)	: Matching call object for the call
**	pTempCallObj(IN)     	: Call Object containing the message that was
**								received
**	pEventContext(IN)		: Sdf_st_eventContext structure that is passed
**								to all callbacks issued by the toolkit
**	pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_messageValidity sdf_ivk_uaUpdateCallDetails _ARGS_((Sdf_st_callObject \
	**ppOriginalCallObj, Sdf_st_callObject *pTempCallObj, \
	Sdf_st_eventContext *pEventContext, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaGetMethodFromSipMessage
**
** DESCRIPTION: This function retrieves the method from the SIP Message.
**				In case of SIP request messages, the method in the 
**				Request-Line is	returned. In case of SIP response messages, 
**				the method in the Cseq header is returned indicating the 
**				request for which the response was generated.
**
** PARAMETERS:
**	pMsg (IN) 		: The input SIP message from which the method is to be
**						retrieved.
**	ppMethod(OUT) 	: The method retrieved
**	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetMethodFromSipMessage _ARGS_((SipMessage  \
	*pMsg,Sdf_ty_s8bit **ppMethod, Sdf_st_error *pErr));

/***********************************************************************
** FUNCTION: sdf_ivk_uaGetRespCodeFromSipMessage
**
** DESCRIPTION: This function retrieves the the response code
**				from the Sip Message.
**
** PARAMETERS:
**	pMsg (IN) 		: The input SIP message from which the response code 
**						is to be retrieved.
**	pRespCode(OUT) 	: The response code retrieved.
**	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetRespCodeFromSipMessage _ARGS_((SipMessage  \
	*pMsg,Sdf_ty_u16bit *pRespCode, Sdf_st_error *pErr));


/******************************************************************
**
** FUNCTION: sdf_ivk_uaGenerateCallid
**
** DESCRIPTION: This function generates a unique callid based on
**				the Callid generator function set in the toolkit for the 
**				particular user profile. The application data is passed as a 
**				parameter to the function to generate call-id's on a per 
**				call basis.
**
** PARAMETERS:
**	pInitData(IN)	: The user profile for the call
**	pAppData(IN)	: The application context which may be used to 
**						generate Call-Id's on a per call basis.
**
******************************************************************/
 Sdf_ty_s8bit* sdf_ivk_uaGenerateCallid _ARGS_((Sdf_st_initData *pInitData,\
			Sdf_st_appData *pAppData));

/***********************************************************************
** FUNCTION: sdf_ivk_uaCompareCallObjectKeys
**
** DESCRIPTION: This function compares two call object keys
**
** PARAMETERS:
**	pNewCallObject(IN) 			: The input call Object that has to be matched
**									against an existing call object
**	pExistingCallObject (IN) 	: An existing call object
**	pMsg (IN)					: Incoming SipMessage used to fetch the
**									matching call object. If this parameter is
**									not NULL, the SipMessage and the dCaller 
**									are used to determine whether the call 
**									object's match (strict check). Else a 
**									leniant comparision (by swapping From-To if
**									direct comparision fails) is used.
**	pCallLegMatch (OUT) 		: Enum indicating the comparision result.
**									Possible values:
**									Sdf_en_notMatching: The message is not 
**										meant for this call object.
**									Sdf_en_sameCallLeg: The message is meant 
**										for this call object.
**									Sdf_en_differentCallLeg: The message is for
**										this call, but the tag in the To header
**										is different from the one in the call 
**										object. This is the result of a 
**										stateful proxy forking the request.
**									Sdf_en_overlapDialing: The message is meant
**										for this call, but the user part in the
**										To header has some extra digits than 
**										what is present in the call object. It 
**										indicates an overlap dialing case.
**	pErr(OUT)					: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCompareCallObjectKeys _ARGS_\
	((Sdf_st_callObject *pNewCallObject, Sdf_st_callObject \
	*pExistingCallObject, SipMessage *pMsg, \
	Sdf_ty_callLegMatch *pCallLegMatch, Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_ivk_uaRedirectCall
**
** DESCRIPTION: Prepares a fresh INVITE to the new address (specified as a 
**				parameter)
**
** PARAMETERS:
**	pOrigCallObj(IN)	: The CallObject which contains the call to be 
**							redirected
**	pNewCallObj(IN/OUT): A new initialized callobject which will be updated
**							with the details from the ppOrigCallObj and also
**							a new INVITE to the redirected destination.
**	pContact (IN)		: The Contact header indicating the new address
**							to which the new INVITE is to be sent
**	pError(OUT)			: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaRedirectCall _ARGS_((Sdf_st_callObject \
	 *pOrigCallObj, Sdf_st_callObject *pNewCallObj, \
	 SipHeader *pContact, Sdf_st_overlapTransInfo **ppOverlapTransInfo,	\
	 Sdf_st_error *pError));
/*******************************************************************
** FUNCTION: sdf_ivk_uaRedirectRequest
**
** DESCRIPTION: Prepares a fresh Request to the new address (specified as a 
**				parameter)
**
** PARAMETERS:
**	pOrigCallObj(IN)	: The CallObject which contains the call to be 
**							redirected
**	pNewCallObj(IN/OUT): A new initialized callobject which will be updated
**							with the details from the ppOrigCallObj and also
**							a new Request to the redirected destination.
**	pContact (IN)		: The Contact header indicating the new address
**							to which the new Request is to be sent
**  pMethod  (IN)		: The Method to be set in the New Request
**	pError(OUT)			: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaRedirectRequest _ARGS_((Sdf_st_callObject \
	 *pOrigCallObj, Sdf_st_callObject *pNewCallObj, \
	 SipHeader *pContact,Sdf_ty_s8bit *pMethod, \
	 Sdf_st_overlapTransInfo **ppOverlapTransInfo, Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_ivk_uaValidateUri
**
** DESCRIPTION: This function takes in the Sip Message
** 				from the call object and checks if the scheme 
**				in the Request-URI is sip: and the version in use
**				is SIP/2.0. It also checks if the address in the
**				Request-URI match that set in the user profile 
**				(passed as a parameter to the function). In case any
**				of the above don't match, it forms an appropriate
**				response message and places it in pTempCallObj.
**
** PARAMETERS:
**		pTempCallObj(IN)	: The CallObject containing the decoded message
** 		pInitData(IN)		: The user profile to match the message against
**		pErrorRespCode(OUT) : The response code for the error response to 
**							  be sent in case the URI is not valid.
**		pError(OUT)			: Error structure in case the function fails
**
********************************************************************/
 Sdf_ty_messageValidity sdf_ivk_uaValidateUri \
	 _ARGS_((Sdf_st_callObject *pTempCallObj, Sdf_st_initData *pInitData, \
	 Sdf_ty_u16bit *pErrorRespCode, Sdf_st_error *pError));

/*******************************************************************
** FUNCTION: sdf_ivk_uaHandleRequire
**
** DESCRIPTION:	Checks if the Request/Response Has a Require Header. If
**				the Require header is present, it checks with 
**				the options in the SUPPORTED field of the 
**				profile. The convention is that if the 
**				Supported Header is absent then a 420 Bad 
**				Extensions is to be sent (for requests)
**
** 				If Supported is present in the user profile,
**				then it's values are compared with the require 
**				values. Options that are not supported are returned 
** 				as unsupported in a 420 response(for requests). If the 
**				SUPPORTED attribute in the user profile has ' * ' 
**				as one of it's values then it is assumed that 
**				all the options are supported.
**				The API only prepares a 420 response and places
**				it in pCallObj if the above negotiation fails (for
**				requests). For responses which come with a Require
**				header, it only returns Sdf_en_invalid indicating
**				mismatch.
**
** PARAMETERS:
**	pCallObj(IN)	: The CallObject containing the SipMessage that 
**						is to be validated
** 	pInitData(IN)	: The user profile to match the message against
**	pErr(OUT) 		: Error Structure in case the function fails
**
********************************************************************/
 Sdf_ty_messageValidity sdf_ivk_uaHandleRequire \
	_ARGS_((Sdf_st_callObject *pCallObj, Sdf_st_initData *pInitData,\
	 Sdf_st_error *pErr));


/*******************************************************************
** FUNCTION: sdf_ivk_uaHandleRequireFromMsg
**
** DESCRIPTION:	Checks if the Request/Response Has a Require Header. If
**				the Require header is present, it checks with 
**				the options in the SUPPORTED field of the 
**				profile. The convention is that if the 
**				Supported Header is absent then a 420 Bad 
**				Extensions is to be sent (for requests)
**
** 				If Supported is present in the user profile,
**				then it's values are compared with the require 
**				values. Options that are not supported are returned 
** 				as unsupported in a 420 response(for requests). If the 
**				SUPPORTED attribute in the user profile has ' * ' 
**				as one of it's values then it is assumed that 
**				all the options are supported.
**				The API only prepares a 420 response and places
**				it in pCallObj if the above negotiation fails (for
**				requests). For responses which come with a Require
**				header, it only returns Sdf_en_invalid indicating
**				mismatch.
**
** PARAMETERS:
**	Sdf_st_transInfo(IN): The transaction structure containing the 
**							SipMessage that is to be validated
** 	Sdf_st_sipMsg()		: For future use.
**	pErr(OUT) 			: Error Structure in case the function fails
**
********************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaHandleRequireFromMsg \
	_ARGS_((Sdf_st_validateInArg *pValidateInArg, \
		Sdf_st_validateOutArg *pValidateOutArg, Sdf_st_error *pErr));

/*********************************************************
** FUNCTION: sdf_ivk_uaGetTransactionForSipMsg
**
** DESCRIPTION: This function returns transaction or
**			overlapped transaction structure that
**			contains the SipMessage which has to be
**			sent out using sendCallToPeer
**
**	PARAMETERS:
**		pCallObj(IN)				: Call Object that is to be unlocked 
**		ppTransaction(OUT)			: Transaction that contains SipMessage
**									  Sdf_co_null otherwize.
**		ppOverlapTransInfo(OUT)		: Overlap trans info if it contains
**									  The SipMessage. Sdf_co_null otherwize.
**		pErr(OUT)					: Error structure in case the function fails
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTransactionForSipMsg
	_ARGS_((Sdf_st_callObject *pCallObj, SipMessage *pSipMsg,\
			Sdf_st_transaction **ppTransaction,\
			Sdf_st_overlapTransInfo **ppOverlapTransInfo, \
			Sdf_st_error *pErr));


/*******************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMethod
**
** DESCRIPTION: Checks if the method in the received SIP request message
**				is supported by the user profile (ALLOW attribute of
**				the user profile). If not, it forms a SIP 405 error 
**				response message and places it in the call object.
**
** PARAMETERS:
**	pObject(IN)		: The call object containing the SipMessage that is
**						to be validated.
** 	pInitData(IN)	: The user profile to match the message against
**	pError(OUT)		: Error structure in case the function fails
*******************************************************************/
 Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMethod _ARGS_\
	((Sdf_st_callObject *pObject, Sdf_st_initData *pInitData, \
	Sdf_st_error *pError));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMethodFromMsg
**
** DESCRIPTION: Checks if the method in the received SIP request message
**				is supported by the user profile (ALLOW attribute of
**				the user profile). If not, it forms a SIP 405 error 
**				response message and places it in the call object.
**
******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMethodFromMsg
	_ARGS_((Sdf_st_validateInArg *pValidateInArg,
	Sdf_st_validateOutArg *pValidateOutArg,
	Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_ivk_uaPurgeExpiredRemoteRetransEntries
**
** DESCRIPTION: This function removes entries from the remote
**				retransmission list that have been in existence for 
**				more than the configured expiry time.
**
** PARAMETERS:
**	pData(IN)	: void pointer unused as of now.
**
********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaPurgeExpiredRemoteRetransEntries \
	 _ARGS_((Sdf_ty_timerType dTimerType,\
	 Sdf_ty_pvoid pData,Sdf_st_error *pErr));

/*******************************************************************
** FUNCTION: sdf_ivk_uaRetrieveMatchingOverlapTransInfo
**
** DESCRIPTION: This API retrieves an OverlapTransInfo 
** 				element within the call object that corresponds to the
** 				SipMessage passed.
**
** PARAMETERS:
**	pMessage(IN)	: The SipMessage used to retrieve the matching overlap
**						transaction element
**	pCallObj(IN)	: The Call Object from which the overlap transaction
**						is to be retrieved
**	ppTransInfo(OUT): This is set to point to the matching overlap 
**						transaction element if it exists
**  dMsgMode		: Set to Sdf_en_msgModeSend if the pMessage is about 
**					  	to be sent and its matching overlap transaction
**						is to be fetched. Set to Sdf_en_msgModeRecd if the
**						message passed to this API has been recd and its
**						matching overlap transaction is to be fetched.
**	pErr(OUT) 		: Error Structure in case the function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaRetrieveMatchingOverlapTransInfo \
	_ARGS_((SipMessage *pMessage, Sdf_st_callObject *pCallObj,\
	Sdf_st_overlapTransInfo **ppTransInfo, Sdf_ty_msgMode dMsgMode, \
	Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRejectRequest
 **
 ** DESCRIPTION: This function has to be invoked to send a failure response to 
 **				 a recd request if any validation prior to 
 **				 sdf_ivk_uaUpdateCallDetails fails. The call object passed to 
 **				 this API is the temporary call object returned by 
 **				 sdf_ivk_uaDecodeMessage().
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaRejectRequest \
	_ARGS_((Sdf_st_callObject *pTempCallObj, Sdf_st_initData *pInitData, \
	 Sdf_ty_u16bit dRespCode, Sdf_st_overlapTransInfo **pOverlapTransInfo, \
	 Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaReturnRequiredList
**
** DESCRIPTION: This function accepts a SipMessage and returns 
**				the Required headers as a list of strings
**
** PARAMETERS:
**		pSipMsg(IN)				: SipMessage structure
**		pslRequiredList(IN/OUT)	: List of Required headers
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaReturnRequiredList _ARGS_((SipMessage *pSipMsg,\
	Sdf_ty_slist *pslRequiredList, Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaReturnSupportedList
**
** DESCRIPTION: This function accepts a SipMessage and returns 
**				the Supported headers as a list of strings
**
** PARAMETERS:
**		pSipMsg(IN)					: SipMessage structure
**		pslSupportedList(IN/OUT)	: List of Supported headers
**		pErr(OUT)					: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaReturnSupportedList _ARGS_( (SipMessage *pSipMsg,\
	Sdf_ty_slist *pslSupportedList, Sdf_st_error *pErr));

#ifdef SDF_THREAD_SAFE
/******************************************************************
** FUNCTION:  sdf_ivk_uaLockCallObject
**
** DESCRIPTION: This function locks the call object for the exclusive use 
**				of the the calling thread.
**
** PARAMETERS:
**		pCallObj(IN)	: Call Object that is to be locked 
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaLockCallObject _ARGS_( (Sdf_st_callObject *pCallObj,\
	Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaUnlockCallObject
**
** DESCRIPTION: This function unlocks the call object 
**
** PARAMETERS:
**		pCallObj(IN)	: Call Object that is to be unlocked 
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaUnlockCallObject _ARGS_( (Sdf_st_callObject *pCallObj,\
	Sdf_st_error *pErr));
#endif

/******************************************************************
** FUNCTION:  sdf_ivk_uaEndTransaction
**
** DESCRIPTION: This function unlocks the call object locked using 
**				Sdf_ivk_uaStartTransaction
**
** PARAMETERS:
**		pCallObj(IN)	: Call Object that is to be unlocked 
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaEndTransaction _ARGS_( (Sdf_st_callObject *pCallObj,\
	 Sdf_st_overlapTransInfo	*pOverlapTransaction,
	 const Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));

/******************************************************************************
** FUNCTION: sdf_ivk_uaEndRegularTransaction
**
** DESCRIPTION: This function unlocks the call object locked using 
**				Sdf_ivk_uaStartRegularTransaction
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaEndRegularTransaction _ARGS_(( \
	Sdf_st_callObject *pCallObj, Sdf_st_overlapTransInfo *pOverlapTransaction,\
	const Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));

/******************************************************************
 ** FUNCTION: sdf_ivk_uaComputeHashFromCallObjectKey
 ** 
 ** DESCRIPTION: this function takes a CallObject Key structure
 ** 				 and returns a hash value
 **
 ******************************************************************/
Sdf_ty_u32bit sdf_ivk_uaComputeHashFromCallObjectKey \
	(Sdf_st_callObjectKey *pCallObjkey);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaIsSupportedMsgBody
**
** DESCRIPTION: This function checks the following header in the receieved
**				message
**					1. If Content-Type header is present in the message then it 
**						Checks the Media type present in the Content-Type
**						header against the List of Accepted Media present in the
**						Accept attribute of the user profile. 
**						If no match found with the Accepted Media then
**						it rejects with 415. ( Default application/sdp)
**
**					2. If Content-Encoding header is present in the message then it 
**						Checks the Encoding present in the Content-Encoding
**						header against the List of Encdoing present in the
**						Accept-Encoding attribute of the user profile
**						If no match found with the Accepted Encoding then
**						it rejects with 415. 
**
**					2. If Content-Language header is present in the message then it 
**						Checks the Language present in the Content-Language
**						header against the List of Language present in the
**						Accept-Language attribute of the user profile
**						If no match found with the Accepted Language then
**						it rejects with 415. 
**
******************************************************************************/
Sdf_ty_messageValidity sdf_ivk_uaIsSupportedMsgBody _ARGS_(\
	(Sdf_st_callObject *pObj,Sdf_st_initData *pInitData,Sdf_st_error *pErr));

/******************************************************************************
** FUNCTION: sdf_ivk_uaCheckMessageLength
**
** DESCRIPTION: This API checks if the length of any of the header 
**				(present in SIP message) is more than the specified length.
**
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCheckMessageLength(Sdf_ty_s8bit *pStr, Sdf_ty_u32bit
		len, Sdf_ty_u32bit dOption, Sdf_ty_messageValidity *pResult, \
		Sdf_st_error *pError);

#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaValidateNotifyWrtRefer
**
** DESCRIPTION: This function validates NOTIFY Request. It checks for 
**		1.   the presence of "event" header. 
**		2.   message body of type "message/sipfrag".
**		3.   Check if first line of message body is Response Status Line.
**	If any of the above validations fails, return Sdf_co_fail.
**
** PARAMETERS:
**		pSipMsg(IN)	: The SipMessage to be validated
**		pErr(OUT)	: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaValidateNotifyWrtRefer (SipMessage *pSipMsg, \
		Sdf_st_error *pErr);

#endif /* SDF_REFER */

#ifdef SDF_SUBSCRIPTION_LAYER
/******************************************************************************
 ** FUNCTION: sdf_ivk_uaIncrementDialogUsageCount
 ** 
 ** DESCRIPTION: This API increments the UsageCount of the CallObject. This
 ** count is used in cases like multiple subscriptions in one dialog where
 ** all the subscriptions use the same CallObject. Each time a subscription or
 ** call is initiated on this 
 ** CallObject, this count must be incremented by a value of one.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** Returns Sdf_co_success if it able to increment the UsageCount, else it
 ** returns Sdf_co_fail.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN) : The CallObj whose UsageCount is to be incremented
 **   pErr(OUT)    : Error structure used to return the type of error that 
 **                  occurred.
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIncrementDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_st_error          *pErr);

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaDecrementDialogUsageCount
 ** 
 ** DESCRIPTION: This API decrements the UsageCount of the CallObject. This
 ** count is used in cases like multiple subscriptions in one dialog where
 ** all the subscriptions use the same CallObject. Ideally, the CallObject 
 ** should be removed/de-allocated only when this count becomes zero.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** Returns Sdf_co_success if it able to decrement the UsageCount, else it
 ** returns Sdf_co_fail.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN) : The CallObj whose UsageCount is to be decremented
 **   pErr(OUT)    : Error structure used to return the type of error that 
 **                  occurred.
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaDecrementDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_st_error          *pErr);

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaCheckDialogUsageCount
 ** 
 ** DESCRIPTION: This API returns the number of usages the Call Object.
 ** It returns the value of 'dDialogUsageCount' from the CallObject.
 ** Assumption: The CallObj is locked prior to invoking this API
 ** 
 ** It returns Sdf_en_dialogNotUsed if dDialogUsageCount is 0, 
 ** Sdf_en_dialogOneUse if dDialogUsageCount is 1 & Sdf_en_dialogMultipleUses
 ** if dDialogUsageCount > 1. The value of pDialogUsageCount is set accordingly.
 ** 
 ** PARAMETERS:
 **   pCallObj(IN)           : Subscription from which the key is to be obtained
 **   pDialogUsageCount(OUT) : Usage count of the dialog
 **   pErr(OUT)              : Error structure used to return the type of 
 **                            error that occurred.
 ******************************************************************************/
Sdf_ty_dialogUsage sdf_ivk_uaCheckDialogUsageCount (
   Sdf_st_callObject     *pCallObj,
   Sdf_ty_u32bit         *pDialogUsageCount,
   Sdf_st_error          *pErr);


#endif /* SDF_SUBSCRIPTION_LAYER */
#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

