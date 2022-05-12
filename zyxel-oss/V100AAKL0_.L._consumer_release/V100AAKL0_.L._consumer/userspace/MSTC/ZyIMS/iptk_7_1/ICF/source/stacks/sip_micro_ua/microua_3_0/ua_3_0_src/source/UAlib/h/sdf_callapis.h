/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/**************************************************************************
 ** FUNCTION:
 **			This file contains all the high-level APIs required at the UAC to
 **			initiate, terminate and modify a call.
 **
 **************************************************************************
 **
 ** FILENAME		: sdf_callapis.h
 **
 ** DESCRIPTION		: This file contains all the high-level APIs required 
 **						at the UAC to initiate, terminate and modify a call.
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 24/10/00	Prashant,					Creation
 **				Subhash
 **
 **************************************************************************
 **	COPYRIGHT , Aricent, 2006
 *************************************************************************/

#ifndef __SDF_CALLAPIS_H_
#define __SDF_CALLAPIS_H_

/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeCall
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form an INVITE message to be sent to the peer entity to
**				start a call.
**
** PARAMETERS:
**		pObject(IN)	: Input call object on which a call is to be initiated
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMakeCall _ARGS_((Sdf_st_callObject *pObject, \
	 Sdf_st_error *pError));

/***********************************************************************
** FUNCTION:  sdf_ivk_uaTerminateCall
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form an BYE message to be sent to the peer entity to
**				terminate a call.
**
** PARAMETERS:
**		pObject(IN)	: Input call object whose call is to be terminated
**		pErr(OUT)	: Error structure in case the function fails **
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaTerminateCall _ARGS_ ((Sdf_st_callObject \
	 *pObject, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeRegister
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a REGISTER message to be sent to the registrar
**
** PARAMETERS:
**		pObject(IN)			: Input call object
**		pExpiry(IN) 		: The duration of the registration. A string 
**							containing either the date format, or seconds.
**							An Expires header with this string is set in the
**							outgoing REGISTER. If set to NULL, an Expires 
**							header will not be inserted.
**		pslContactList(IN)	: List of Contact addresses to be used in the
**							REGISTER request. If this is set to NULL, the
**							list of Contacts in the user profile for this 
**							call object will be used. Else this list will
**							be used. Used mainly for third-party registrations.
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMakeRegister  _ARGS_((Sdf_st_callObject  \
	*pObject, Sdf_ty_s8bit *pExpiry, Sdf_ty_slist *pslContactList, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo,	\
	Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeOptions
**
** DESCRIPTION: Invokes core stack APIs to form a OPTIONS message to be 
**				sent to the peer entity
**
** PARAMETERS:
**		pObject(IN)	: Input call object
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMakeOptions _ARGS_((Sdf_st_callObject *pObject, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeInfo
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a INFO message to be sent to the peer entity
**
** PARAMETERS:
**		pObject(IN)	: Input call object 
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMakeInfo _ARGS_((Sdf_st_callObject *pObject,\
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeTransaction
**
** DESCRIPTION: This function prepares a SIP transaction message.
**				Methods that are supported are REGISTER, OPTIONS, 
**				INFO, PRACK, COMET, SUBSCRIBE and NOTIFY. The API does any 
**				internal handling that is required before calling 
**				sdf_ivk_uaFormRequest API. 
**				The prepared SIP message is inserted into the pOutgoingMsg
**				structure of the input call object.
**
** PARAMETERS:
**		pMethod(IN)				: The method of the SIP message to be formed.
**		pObject(IN)				: Input call object
**		pOverlapTransInfo(IN)	: The overlapTransInfo structure in case a 
**									PRACK/COMET. This parameter indicates the 
**									overlapTransInfo element for which a 
**									request is to be formed. In all other 
**									cases, this parameter is NULL.
**		pErr(OUT)				: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaMakeTransaction _ARGS_ ((Sdf_ty_s8bit *pMethod,\
	Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo,\
	Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaCancelCall
**
** DESCRIPTION: Invokes core stack APIs to form a CANCEL message and
** 				inserts it into the input call object
**
** PARAMETERS:
**		pObject(IN)	: Input call object 
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCancelCall  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaReinvite
**
** DESCRIPTION: This function updates the call object and prepares an 
**				INVITE message with new media parameters which are passed
**				to the API as a parameter.
**
** PARAMETERS:
**		pObject(IN)			: Input call object 
**		slSdpMediaList(IN)	: The list of Media streams to be used in 
**								the reINVITE
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaReinvite  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_ty_slist slSdpMediaList, Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddExtraHeaders
**
** DESCRIPTION: This function adds any extra headers that are set in the user
**				profile, into the call object's Processed and Unprocessed header
**				list before calling the sdf_ivk_uaFormRequest API.
**
** PARAMETERS:
**		pObject(IN) 			: The input callObject
**		pOverlapTransInfo(IN)	: The overlapTransInfo structure in case a 
**									PRACK/COMET. This parameter indicates the 
**									overlapTransInfo element for which a 
**									request is being formed. In all other 
**									cases, this parameter is NULL.
**		pMethod(IN) 			: The method of the SipMessage being formed
**		pErr(OUT)				: The error structure in case the function fails
**
******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddExtraHeaders _ARGS_((Sdf_st_callObject \
	*pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));

/*****************************************************************************
** FUNCTION: sdf_ivk_uaAddExtraRespHeaders
**
** DESCRIPTION: This function adds any extra headers that are set in the user
**				profile, into the call object's Processed and Unprocessed header
**				list before calling the sdf_ivk_uaFormRequest API.
**
** PARAMETERS:
**		pObject(IN) 			: The input callObject
**		pOverlapTransInfo(IN)	: The overlapTransInfo structure in case a 
**									PRACK/COMET. This parameter indicates the 
**									overlapTransInfo element for which a 
**									request is being formed. In all other 
**									cases, this parameter is NULL.
**		pMethod(IN) 			: The method of the SipMessage being formed
**		pErr(OUT)				: The error structure in case the function fails
**
******************************************************************************/
#ifdef SDF_IMS
 Sdf_ty_retVal sdf_ivk_uaAddExtraRespHeaders _ARGS_((Sdf_st_callObject \
	*pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));
#endif
/*****************************************************************************
** FUNCTION: sdf_ivk_uaDeleteRegistration
**
** DESCRIPTION: This API form a Registratio By inserting the Contact header
** 				list for which the Delete Registration will be send
**
** PARAMETERS:
**		pObject(IN) 			: The input callObject
**		pContactList(IN)		: The Contact List for which the delete 
**								  	Registration will be send
**
**		pErr(OUT)				: The error structure in case the function 
**									fails							
******************************************************************************/

 Sdf_ty_retVal sdf_ivk_uaDeleteRegistration _ARGS_((Sdf_st_callObject \
	*pObject, Sdf_ty_slist *pContactList, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, \
	Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaMakeUpdate
**
** DESCRIPTION: Invokes core stack APIs to form a OPTIONS message to be 
**				sent to the peer entity
**
** PARAMETERS:
**		pObject(IN)	: Input call object
**		pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/

Sdf_ty_retVal sdf_ivk_uaMakeUpdate(Sdf_st_callObject *pCallObj, \
	Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_st_error *pErr);

#ifdef SDF_IM
/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeMessage
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a MESSAGE message
**
** PARAMETERS:
**		pObject(IN)	: Input call object over which the New request to be formed
**		pOverlapTransInfo(IN): The transaction structure corresponding to
**								the MESSAGE Request formed. The formed MESSAGE
**								Request will be present in this structure.
**		pExpiry (IN): The Expiry value as string to be passed, if application
**						want to set the Expire header in MESSAGE Request
**		pDate (IN)	: The Date value as string to be passed, if application
**					 	want to set the Date header in MESSAGE Request.
**						The date should be of <Sat, 13 Nov 2001 23:29:00 GMT>
**						format.
**		pErr(OUT)	: Error structure in case the function fails
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeMessage _ARGS_((Sdf_st_callObject *pObject, \
 Sdf_st_overlapTransInfo *pOverlapTransInfo, Sdf_ty_s8bit *pExpiry, \
 Sdf_ty_s8bit *pDate, Sdf_st_error *pErr));
#endif

/***********************************************************************
** FUNCTION:  sdf_ivk_uaMuteCall
**
** DESCRIPTION: This function prepares an INVITE message to Mute the call by
**              either setting the c= line to 0.0.0.0 or changing the 
**              direction attribute.
**
** PARAMETERS:
**		pObject(IN)			: Call object 
**		pCodecsToMute(IN)	: String containing list of codecs to be muted.
**								If this is set to NULL, all codecs are muted
**								using a session level c= line.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs.
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
 Sdf_ty_retVal sdf_ivk_uaMuteCall  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_ty_s8bit *pCodecsToMute, Sdf_ty_slist *pslMediaIDs, \
	 Sdf_ty_muteType dType, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaFormMuteSdp
**
** DESCRIPTION: This function prepares an *pMethod* message to Mute the call by
**              either setting the c= line to 0.0.0.0 or changing the 
**              direction attribute, depending on the dType parameter passed.
**
** PARAMETERS:
**		pObject(IN)			: Call object 
**		pCodecsToMute(IN)	: String containing list of codecs to be muted.
**								If this is set to NULL, all codecs are muted
**								using a session level c= line.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs.
**      dType				: Enum explaining the type of Mute required.
**							  If it is Sdf_en_connectionMute, then the IP addr
**							  in c-line will be set to 0.0.0.0 . If it is
**							  Sdf_en_holdSelfMedia, then direction attr will be
**							  set to Sdf_en_holdSelfMedia. If it is 
**							  Sdf_en_completeMute, then direction attr will be 
**							  set to Sdf_en_Inactive else the direction attr 
**							  will be set to Sdf_en_Sendonly.
**      pMethod(IN)			: To determine the method of the message to be 
**							  formed.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormMuteSdp  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_ty_s8bit *pCodecsToMute, Sdf_ty_slist *pslMediaIDs, \
	 Sdf_ty_muteType dType, Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));


 /***********************************************************************
** FUNCTION:  sdf_ivk_uaMuteMediaStreams
**
** DESCRIPTION: This function mutes the MediaStreams present in the given
**              Session Param either setting the c= line to 0.0.0.0 or 
**              changing the direction attribute, depending on the dType 
**              parameter passed.
**
** PARAMETERS:
**		pSessionParams(IN)	: Session Param.
**		pCodecsToMute(IN)	: String containing list of codecs to be muted.
**								If this is set to NULL, all codecs are muted
**								using a session level c= line.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs.
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
 Sdf_ty_retVal sdf_ivk_uaMuteMediaStreams  _ARGS_((\
    Sdf_st_sessionParam *pSessionParams, Sdf_ty_s8bit  *pCodecsToMute,\
	Sdf_ty_slist *pslMediaIDs, Sdf_ty_muteType dType, Sdf_st_error *pErr));



/***********************************************************************
** FUNCTION:  sdf_ivk_uaUnMuteCall
**
** DESCRIPTION: This function prepares an INVITE message to UnMute the call by
**              either restoring the c= line to prev IP addr or by restoring 
**              the direction attribute, depending on how the media stream
**              was muted.
**
** PARAMETERS:
**		pObject(IN)			: Call object 
**		pCodecsToMute(IN)	: String containing list of codecs to be UnMuted.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs of the stream
**							  that needs to be UnMuted.
**      dType				: Enum explaining how the call was Muted.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaUnMuteCall  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_ty_s8bit *pCodecsToMute, Sdf_ty_slist *pslMediaIDs, \
	 Sdf_ty_muteType dType, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaFormUnMuteSdp
**
** DESCRIPTION: This function prepares an *pMethod* message to UnMute the call 
**				by either restoring the c= line to prev IP addr or by restoring 
**              the direction attribute, depending on how the media stream
**              was muted.
**
** PARAMETERS:
**		pObject(IN)			: Call object 
**		pCodecsToMute(IN)	: String containing list of codecs to be UnMuted.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs of the stream
**							  that needs to be UnMuted.
**      dType				: Enum explaining how the SDP was Muted.
**      pMethod(IN)			: To determine the method of the message to be 
**							  formed.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormUnMuteSdp  _ARGS_((Sdf_st_callObject *pObject,\
	 Sdf_ty_s8bit *pCodecsToMute, Sdf_ty_slist *pslMediaIDs, \
	 Sdf_ty_muteType dType, Sdf_ty_s8bit *pMethod, Sdf_st_error *pErr));


/***********************************************************************
** FUNCTION:  sdf_ivk_uaUnMuteMediaStreams
**
** DESCRIPTION: This function UnMutes the SessionParam by
**              either restoring the c= line to prev IP addr or by restoring 
**              the direction attribute, depending on dType.
**
** PARAMETERS:
**		pSessionParams(IN)	: The Session Param.
**		pCodecsToMute(IN)	: String containing list of codecs to be UnMuted.
**      pslMediaIDs(IN)		: List of strings containing MediaIDs of the stream
**							  that needs to be UnMuted.
**      dType				: Enum explaining how the MediaStream was Muted.
**		pErr(OUT)			: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaUnMuteMediaStreams  _ARGS_((\
	 Sdf_st_sessionParam *pSessionParams, Sdf_ty_s8bit *pCodecsToUnMute,\
	 Sdf_ty_slist *pslMediaIDs, Sdf_ty_muteType dType, Sdf_st_error *pErr));

#endif


#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeRefer
**
** DESCRIPTION: Updates the call object and invokes core stack APIs to 
**				form a REFER request to be sent to the peer entity.
**				This function also adds a "Refer-To" header. If a "Replaces"
**				header is also passed, this function converts it to string
**				(escapes any characters, if required) and adds it to
**				"Refer-To" header as a URL param.
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store REFER txn
**		pReferToHdr(IN)		: Refer-To header to go in REFER request
**		pReplacesHdr(IN)	: Replaces header to go in REFER request as header
**								param in Refer-To header.
**		dIsSendRfrdBy(IN)	: Boolean to indicate whether to send Referred-By
**								header in the REFER request or not
**		pReferredBy(IN)		: If `dIsSendRfrdBy` is TRUE, this header will be
**								set in SipMessage
**		pErr(OUT)			: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeRefer (Sdf_st_callObject *pCallObj, \
		Sdf_st_overlapTransInfo *pOverlapTransInfo, SipHeader \
		*pReferToHdr, SipHeader *pReplacesHdr, Sdf_ty_bool \
		dIsSendRfrdBy, SipHeader *pReferredBy, Sdf_st_error *pErr);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaMakeNotify
**
** DESCRIPTION: Invokes core stack APIs to form a NOTIFY message to be 
**				sent to the peer entity. This API will also create and
**				add an EVENT header, depending on the function arguments.
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store NOTIFY txn
**		pEvent(IN)		: Event header to go in NOTIFY request
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaMakeNotify (Sdf_st_callObject *pCallObj, \
		Sdf_st_overlapTransInfo *pOverlapTransInfo, \
		Sdf_ty_s8bit *pEvent, Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormNotify
**
** DESCRIPTION: This API forms the Notify, sets the event header and adds
**				the subscription state  header based on the input response
**				message passed. It also sets the response message as the 
**				appsip message body in this notify
**
** PARAMETERS:
**		pCallObj(IN/OUT)	: call object to associated with this transaction
**		pOverlapTransInfo(OUT): Overlap txn structure to store NOTIFY txn
**		pEvent(IN)		: Event header to go in NOTIFY request
**		pMessage(IN)	: The Sipmessage which is gonna be set as the msg body
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormNotify (Sdf_st_callObject *pCallObj, \
		Sdf_st_overlapTransInfo *pOverlapTransInfo, \
		Sdf_ty_s8bit *pEvent, SipMessage *pMessage, Sdf_st_error *pErr);


/*****************************************************************************
** FUNCTION: sdf_ivk_uaHandleReplacement
**
** DESCRIPTION: This API indicates what action needs to be done when a
**		Replaces header matches an existing (early/confirmed) dialog.
**
** PARAMETERS:
**		pMatchedCallObj(IN)		: Call object which matched the Replaces hdr
**		pRespForNewCall(OUT)	: Response to be sent out on the new call
**		ppReqForMatchedCall(OUT): Request to be sent out on the matched call
**		pErr(OUT)				: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaHandleReplacement (SipMessage *pSipMsg, \
        Sdf_st_callObject *pMatchedCallObj, Sdf_ty_u32bit *pRespForNewCall,Sdf_ty_s8bit **ppReqForMatchedCall, Sdf_st_error *pErr);


#endif /* SDF_REFER */
#ifdef SDF_PRIVACY
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormAnonymousFromHdrForPrivacy
**
** DESCRIPTION: 
*****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFormAnonymousFromHdrForPrivacy _ARGS_(\
    (Sdf_st_callObject *pObject, Sdf_ty_s8bit	*pFromHeaderStr, \
     Sdf_st_error *pErr));

#endif


/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
