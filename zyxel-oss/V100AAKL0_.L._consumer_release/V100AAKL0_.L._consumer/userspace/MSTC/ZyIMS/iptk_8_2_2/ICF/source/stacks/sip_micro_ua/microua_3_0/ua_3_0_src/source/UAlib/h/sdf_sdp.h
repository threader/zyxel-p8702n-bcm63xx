/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **             Has API's for manipulating/accessing SDP
 **
 ******************************************************************************
 **
 ** FILENAME:
 ** sdf_sdp.h
 **
 ** DESCRIPTION:
 ** 	This file contains API'S for manipulating/accessing SDP
 **
 **  DATE        	NAME             REFERENCE       REASON
 **  ----       	----             ---------      --------
 ** 20 Nov. 00		Subhash							Creation
 **
 ** 28 Feb. 01		Siddharth						Updated for 
 **													changes in
 **													SDP media 
 **													negotiation
 ** 21 Nov  02		Elias/Srikanth					SDP Revamp
 ** 12-Feb-2009     Rajiv Kumar      Rel 8.1        Changes done for handling
 **                                                 icf_tunnelled_sdp_info_st
 **                                                 in tunnelled mode.       
 **
 ******************************************************************************
 **    	 		COPYRIGHT , Aricent, 2006
 *****************************************************************************/
#ifndef __SDF_SDP_H_
#define __SDF_SDP_H_

#include "sdf_tunnelled_media.h"    
/* Codenomicon fix start: CSR_1-6714693 */

/* Codenomicon fix for media lines, this macro is defined to
 * provide the flexibility to the application to select the number
 * media lines to be parsed, by default it is set to 10
 */
#define MAX_SDP_MEDIA_LINES 100 
/* Codenomicon fix end: CSR_1-6714693 */    

 /* Wrapper funtions for SDP funtions of MicroSip. */
extern 	SipBool sdp_insertBandwidthAtIndex_level1 _ARGS_(( SdpMessage *msg, \
		SIP_S8bit *bandwidth, SIP_U32bit cnt, SipError *err ));

extern 	SipBool sdp_getBandwidthCount_level1 _ARGS_(( SdpMessage *msg, \
		SIP_U32bit *cnt, SipError *err ));

extern 	SipBool sdp_getBandwidthAtIndex_level1 _ARGS_(( SdpMessage *msg, \
		SIP_S8bit **bandwidth, SIP_U32bit cnt, SipError *err ));

extern SipBool sdp_insertPhoneAtIndex_level1 _ARGS_ (( SdpMessage *msg, \
			SIP_S8bit *slPhone, SIP_U32bit cnt, SipError *err ));

extern 	SipBool sdp_getPhoneCount_level1 _ARGS_(( SdpMessage *msg, \
		SIP_U32bit *cnt, SipError *err ));

extern 	SipBool sdp_getPhoneAtIndex_level1 _ARGS_(( SdpMessage *msg, \
		SIP_S8bit **phone, SIP_U32bit cnt, SipError *err ));

extern	SipBool sdp_insertEmailAtIndex_level1 _ARGS_(( SdpMessage *msg,
			SIP_S8bit *pEmail, SIP_U32bit cnt, SipError *err ));

extern 	SipBool sdp_getEmailCount_level1 _ARGS_(( SdpMessage *msg, \
		SIP_U32bit *cnt, SipError *err ));

extern 	SipBool sdp_getEmailAtIndex_level1 _ARGS_(( SdpMessage *msg, \
		SIP_S8bit **email, SIP_U32bit cnt, SipError *err ));



/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMatchMedia
 **
 ** DESCRIPTION: This function matches the SdpMessage in the pIncomingMessage 
 **				 of the call object and matches it against the SdpMessage of 
 **				 the init data. The final negotiated body is returned to the 
 **				 application. The application have the following options
 **         
 ** PARAMETERS:
 **	 pMediaInfo		(IN)  : The Media-Info taken from CallObject whose SIP \
 **							message is compared with the SdpMsgBody already 
 **							present in the MsgBodyList.
 **	 pRecdSipMessage (IN) : The SipMessage that has been received and whose 
 **							SDP needs to be negotiated against ours.
 **	 slAcceptedMedia(OUT) : The list in which the media lines
 **						  	which match are recd. (picked from 
 **						  	the request itself)
 **	 ppFormedMsg	(OUT) :	The SDP Message formed after basic
 **							negotiation with the SDP in the 
 **							request. The application may choose
 **							to modify it further before setting
 **							in the call object.
 **	 pError			(OUT) : Error structure in case the function 
 **							fails
 **
 ******************************************************************************/
 Sdf_ty_matchMediaResult sdf_ivk_uaMatchMedia \
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, SipMessage *pRecdSipMessage, \
	Sdf_ty_slist *slAcceptedMedia, SdpMessage **ppFormedMsg, \
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaMatchMediaFromSdpMessage
 **
 ** DESCRIPTION: This function takes an input sdpmessage body and
 **				 compares it against another sdpmessage body (present in the 
 **				 initdata passed to the API). The final negotiated body is 
 **				 returned to the application. The application have the following **              options
 **
 ** PARAMETERS:
 **	 pIncomingBody(IN):		The input SdpMessage which has to be compared.
 **	 pInitData	 (IN): 		The InitData from which own capability set is 
 **							retrieved and with which the pIncomingBody is 
 **							compared
 **	 ppNegotiatedBody(OUT):	The SDP Message formed after basic negotiation 
 **							with the SDP in the request. The application may 
 **							choose to modify it further before setting in 
 **							the call object.
 **	 slAcceptedMedia(OUT): The Accepted Media list
 **	 pError		 (OUT)	: 	Error structure returned in case the function fails.
 **
 ******************************************************************************/
 Sdf_ty_matchMediaResult sdf_ivk_uaMatchMediaFromSdpMessage \
	_ARGS_((SdpMessage *pIncomingBody, Sdf_st_initData *pInitData, \
	Sdf_st_mediaHandling *pMediaInfo, SdpMessage **ppNegotiatedBody, 
	Sdf_ty_slist *slAcceptedMedia, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaIsSdpBodyPresent
 **
 ** DESCRIPTION: This function returns Sdf_co_success/Sdf_co_fail depending 
 **				 on whether the SipMessage contains a SDP messagebody.
 **
 ** PARAMETERS:
 **	 pMessage(IN)	: The SIP message to be checked for
 **					  presence of SDP body.
 **	 pError	(OUT)	: Error structure in case the function fails.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaIsSdpBodyPresent \
	_ARGS_((SipMessage *pMessage, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION:	sdf_ivk_uaFormSdpMedia
 **
 ** DESCRIPTION: This function forms a SdpMedia using the parameters 
 **				 passed into the function.
 **
 ** PARAMETERS:
 **	 	pMvalue	(IN)			: Media Type (Eg. audio, video)
 **	 	dPort	(IN)			: transport port to which media is
 **								  sent
 **	 	dNumberOfPorts(IN)		: Number of ports being used
 **		pTransport	(OUT)		: Transport Protocol (Eg. RTP/AVP)
 **		pPayLoadList(OUT)		: Media Formats
 **		pTypeOfTransport(OUT)	: Mode of Transport (Eg. recvonly, 
 **							      sendonly)
 **		slAttributes(IN)		: List of attribute lines  
 **							 	  supplementing the media format
 **		slConnection			: List of connection lines to be sent
 **		pBandwidth				: Bandwidth line to be sent in media
 **		pInfo					: Information line to be sent in media
 **		pKey					: Key line to be sent
 **		pSdpMedia	(OUT)		: The SdpMedia structure formed
 **	 	pErr	(OUT)			: Error structure in case the 
 **								  function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaFormSdpMedia _ARGS_((Sdf_ty_s8bit *pMvalue, \
	Sdf_ty_u32bit dPort, Sdf_ty_u32bit dNumberOfPorts, \
	Sdf_ty_s8bit *pTransport, Sdf_ty_s8bit *pPayLoadList, \
	Sdf_ty_s8bit *pTypeOfTransport, Sdf_ty_slist *slAttributes, \
	Sdf_ty_slist *slConnection,Sdf_ty_s8bit *pBandwidth, Sdf_ty_s8bit *pInfo, \
	Sdf_ty_s8bit *pKey,SdpMedia **pSdpMedia, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaSetAllMsgBody 
 **
 ** DESCRIPTION: 	This function sets all the message bodies from the 
 **					CallObject's MsgBodyList (or overlaptransinfo's 
 **					MsgBodyList if it is a non-NULL parameter) into the 
 **					pOutgoingMessage of the call object.
 **
 ** PARAMETERS:
 **		pTransaction(IN)	: The transaction Object whose Sip message is filled
 **						      with the bodies from the 
 **						      MsgBodyList 
 **		pOverlapTransInfo(IN): 	The overlapTransInfo structure from which 
 **								the bodies need to be set. This parameter 
 **								may be NULL, indicating that the message 
 **								bodies should be set from the callobject's 
 **								list.
 **		pError	(OUT)	: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetAllMsgBody _ARGS_((Sdf_st_transaction \
	*pTransaction, Sdf_st_overlapTransInfo *pOverlapTransInfo,\
  	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaSetMsgBodyListInSipMessage
 **
 ** DESCRIPTION: This function sets the message bodies from the given
 **				 list into the SIP message. It first removes any 
 **				 previously present MsgBodies in the SIP message, 
 **				 and then adds the new list.
 **
 **	PARAMETERS:
 **		pMsgBodyList(IN)	: 	The MsgBodyList to be set into the
 **								SipMessage
 **		pMsg		(IN/OUT): 	The SipMessage into which the new 
 **								MsgBodies will be set
 **		pError		(OUT)	: 	Error structure in case the	function fails
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetMsgBodyListInSipMessage \
	_ARGS_((Sdf_ty_slist  *pMsgBodyList, SipMessage *pMsg, \
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaAppendMsgBodyListToSipMessage
 **
 ** DESCRIPTION: This function appends the message bodies from the 
 **				 given list into the SIP message. The MsgBodies 
 **				 already present in the SipMessage are left untouched.
 **
 **	PARAMETERS:
 **		pMsgBodyList(IN)	: 	The MsgBodyList to be appended into the
 **								Sip Message
 **		pMsg		(IN/OUT): 	The SipMessage into which the 
 **					  			new MsgBodies will be set
 **		pError		(OUT)	: 	Error structure in case the function 
 **					  			fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAppendMsgBodyListToSipMessage \
		_ARGS_((Sdf_ty_slist *pMsgBodyList, SipMessage *pMsg, \
	  	Sdf_st_error *pError));



/******************************************************************************
 ** FUNCTION: sdf_ivk_uaSetSdpInMsgBodyList
 **
 ** DESCRIPTION: This function sets a SDP message in the  
 **				  MsgBodyList passed to the function. 
 **
 ** PARAMETERS:
 **		
 **				pMessage	(IN):	The SdpMessage to be set in 
 **						 	the call object's message
 **						 	body list (This overwrites
 **						 	any already existing SDP message
 **            					 	in the call object.)
 **                             pMsgBodyList    (IN):   The target message body list in which we
 **                                                     need to add the message body.
 **
 **				pError		(OUT): 	Error structure in case the 
 **								 	function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetSdpInMsgBodyList \
	_ARGS_((SdpMessage *pMessage, Sdf_ty_slist *pMsgBodyList, \
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaSetSdpInTransaction
 **
 ** DESCRIPTION: 	This function sets a SDP message to the  
 **					MsgBodyList contained within the transaction
 **					object passed to the function. 
 **
 ** PARAMETERS:
 **	 pTransaction (IN)	: The transaction object 
 **		
 **	 pMessage (IN)		: The SdpMessage to be set in the transaction 
 **							object's message body list.
 **	 pError (OUT)		: 	Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetSdpInTransaction \
	_ARGS_((Sdf_st_transaction *pTransaction ,SdpMessage *pMessage,  \
	Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaSetSdpInOverlapTransaction
 **
 ** DESCRIPTION: 	This function sets a SDP message to the  
 **					MsgBodyList contained within the overlaptransaction
 **					object passed to the function. 
 **
 ** PARAMETERS:
 **	 pOverlapTransInfo (IN)	: The overlap transaction object 
 **		
 **	 pMessage (IN)			: The SdpMessage to be set in the overlap
 **								transaction's message body list
 **	 pError(OUT)			: 	Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetSdpInOverlapTransaction \
	_ARGS_((Sdf_st_overlapTransInfo *pOverlapTransInfo ,SdpMessage *pMessage, \
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaCreateMessageBodyFromBuffer
 **
 ** DESCRIPTION: This function creates a message body from the
 **			 	 buffer passed. Note that this API should not be 
 **				 used to set MIME, SDP or application/sip bodies. 
 **				 The API should be used to set ISUP bodies and other 
 **				 buffers like text/plain, application/mgcp, etc.
 **
 ** PARAMETERS:		   
 **			ppMsgBody(OUT)	: The SipMsgBody formed
 **			pContentType(IN): The Content-Type string (Eg: 
 **							  "text/plain", "application/isup")
 **			pBuffer	(IN)	: The buffer to be inserted in the 
 **							  message body
 **		    pBufLength	(IN): Buffer length of the buffer
 **		    dAddMimeHdr	(IN): Boolean indicating whether a mime header
 **						 	  is to be added. This should be set to 
 **							  Sdf_co_true only if the body is part of 
 **							  a multipart mime body
 **			pError	(OUT)	: Error structure in case the function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaCreateMessageBodyFromBuffer _ARGS_((SipMsgBody \
	**ppMsgBody, Sdf_ty_s8bit	*pContentType, Sdf_ty_s8bit	*pBuffer, \
	Sdf_ty_u32bit 	pBufLength, Sdf_ty_bool		dAddMimeHdr, \
	Sdf_st_error 	*pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaGetUnknownBodyFromSipMessage
 **
 ** DESCRIPTION: This function is used to get a unknown body from a
 **				 SipMessage
 **
 ** PARAMETERS:		   
 **				pMsg	(IN)	: The SipMessage structure from which 
 **								  the body is to be retrieved
 **				dIndex	(IN)	: The index of the body in the SipMessage
 **				ppBuffer(OUT)	: The buffer retrieved from the SipMessage
 **				pBufLength(OUT)	: The length of the buffer
 **				pError	(OUT)	: Error structure in case the function 
 **								  fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetUnknownBodyFromSipMessage _ARGS_\
	((SipMessage *pMsg, Sdf_ty_u32bit dIndex, Sdf_ty_s8bit **ppBuffer, \
	Sdf_ty_u32bit *pBufLength, Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaGetIsupBodyFromSipMessage
 **
 ** DESCRIPTION: This function retrives an ISUP Message Body from 
 **				 the SipMessage passed as input.
 **
 ** PARAMETERS:		   
 **				pMsg	(IN)	: The SipMessage structure from which 
 **								  the ISUP body is to be retrieved
 **				dIndex	(IN)	: The index of the body in the SipMessage
 **				ppIsupBody(OUT)	: The ISUP buffer retrieved from the 
 **								  SipMessage
 **				pIsupBodyLength(OUT): The length of the ISUP buffer
 **				pError	(OUT)	: Error structure in case the function
 **								  fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetIsupBodyFromSipMessage _ARGS_\
	((SipMessage *pMsg, Sdf_ty_u32bit dIndex, Sdf_ty_s8bit **ppIsupBody, \
	Sdf_ty_u32bit *pIsupBodyLength, Sdf_st_error *pError));

#ifdef SDF_REFER
/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSipfragBodyFromSipMessage
**
** DESCRIPTION: This API will check for the presence of the sipfrag 
**		message body from the sip message and returns SipMessage equivalent
**		of the message body.
**
** PARAMETERS:
**		pInSipMsg(IN)	: The SipMessage which contains Sipfrag body
**		ppOutSipMsg(OUT): The output SipMessage which was obtained by parsing
**							the Sipfrag message body present in pInSipMsg
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSipfragBodyFromSipMessage (SipMessage *pInSipMsg,
	 SipMessage **ppOutSipMsg, Sdf_st_error *pErr);

/*****************************************************************************
** FUNCTION: sdf_ivk_uaFormAndAddSipfragBodyToSipMessage
**
** DESCRIPTION: This API  forms the sipfrag messagebody from the given input 
**              sip message and adds the formed body into the output SipMessage
**              This API also adds the Content-Type header as "message/sipfrag"
**				in the output Sip message
** PARAMETERS:
**		pSipMsg(IN)		: The SipMessage to be set in the sipfrag body 
**		ppSipMsg(OUT)    : The output Sip message
**		pErr(OUT)		: Error structure in case the function fails
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormAndAddSipfragBodyToSipMessage 
		(SipMessage *pSipMsg, SipMessage **ppSipMsg, Sdf_st_error *pErr);

#endif



/*******************************************************************
** FUNCTION:	sdf_ivk_uaStoreSdpBodyAsSessionSdp
**
** DESCRIPTION: This function stored the sdp as the SessionSdp by inserting
				into the persistent list as Session-SDP
**
** Parameters:	
**				pMediaInfo(IN/OUT): The Media-Info taken from call object \
**									where the SDP  message in the \
**									persistentlist will be stored
**				pSdpMsg(IN):	The SDP message which will be set in the 
**								persistentlist of the call object
**				pError(OUT)	: 	 Error structure in case the 
**								 function fails
**
********************************************************************/
 Sdf_ty_retVal sdf_ivk_uaStoreSdpBodyAsSessionSdp  
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, SdpMessage *pSdpMsg,\
	Sdf_st_error *pError));



/******************************************************************
**
** FUNCTION:  sdf_ivk_uaCompareSessionParams
**
** DESCRIPTION: This API compares two session level parameters
**              and provides the negotiated Session parameter 
**              and the list of media lines that were acceptable.   
**
**              default : 
**              Complete negotiation happens and the negotiated body and the
**              accepeted media list shall be returned
**
**              SDF_OPT_COMPUTE_PEER_SESSION :
**              If this option is set then along with the ngtn/coparison it
**              also computes the Peer's view of the ngtd session and sets 
**              in the capabilities structure.
**
**              SDF_OPT_COMPARE_SESSISON :
**              Along with the negotiation it verifes which are all the info 
**              got changed in the session param and provides as a 
**              SessionChange structure
**
** PARAMETERS:
**       PsessionParamNew[IN] :	SessionParam to be negotiated
**       PsessionParamOld [IN]: The new SessionParam will be 
**                              negotiated with this param.
**       PcallObj [IN]	      : The Callobject used to know about
**                              the media states
**       ppnegotiatedSessionParam [OUT]:	The negotiated session
**                                      params based on the comparison
**       SlAcceptedMedia[OUT] : The list in which the media lines (Sdf_st_mediaStream)
**			                    which match are recd. (picked from the request itself)
**       ppSessionChange <OUT> : If the application opted only for 
**                              comparison then the comparison results
**                              shall be given through this param
**       dOption <IN> :         Specifies whether the peer session 
**                              need to be computed and this is a call for 
**                              negotiation or just for comparison
**	     pError	(OUT) :         Error structure in case the function 
**							    fails
******************************************************************/
#ifdef SDF_SESSIONCHANGE
Sdf_ty_matchMediaResult sdf_ivk_uaCompareSessionParams
   _ARGS_((Sdf_st_sessionParam *pSessionParamNew,\
   Sdf_st_sessionParam *pSessionParamOld,\
   Sdf_st_mediaHandling *pMediaInfo,\
   Sdf_st_sessionParam **ppNegotiatedSessionParam,\
   Sdf_ty_slist *slAcceptedMedia,\
   Sdf_st_sessionChange **ppSessionChange, Sdf_ty_options dOptions,\
   Sdf_st_error *pError));
#else
Sdf_ty_matchMediaResult sdf_ivk_uaCompareSessionParams
   _ARGS_((Sdf_st_sessionParam *pSessionParamNew,\
   Sdf_st_sessionParam *pSessionParamOld,\
   Sdf_st_mediaHandling *pMediaInfo,\
   Sdf_st_sessionParam **ppNegotiatedSessionParam,\
   Sdf_ty_slist *slAcceptedMedia,\
   Sdf_st_error *pError));
#endif


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaHandleMedia
**
** DESCRIPTION: Top level abstraction for handling the media(SDP 
**              offer/answer). This API checks the sdp and media 
**              state and based on the media state it negotiates 
**              the recvd sdp with the existing capabilities; 
**              and provides the negotiated Session parameter 
**              and the list of media lines that were acceptable.
**              The application can opt for the following options
**              default : 
**              Complete negotiation happens and the negotiated body and the
**              accepeted media list shall be returned
**              SDF_OPT_COMPARE_SESSION :
**              Comparison of the session happens and the session change
**              struture is returned which specifies what got changed. 
**              SDF_OPT_COMPUTE_PEER_SESSIOM :
**              If this option is set then along with the ngtn/comparison it
**              also computes the Peer's view of the ngtd session and sets 
**              in the capabilities structure.
**
** PARAMETERS:
**       pMediaInfo	(IN)  	  : The Media-Info taken from CallObject 
**       pRecdSipMessage (IN) : The SipMessage that has been received and whose 
**			                    SDP needs to be negotiated against ours.
**       SlAcceptedMedia[OUT] : The list in which the media lines (Sdf_st_mediaStream)
**			                    which match are recd. (picked from the request itself)	
**       ppSessionChange <OUT> : If the application opted only for 
**                              comparison then the comparison results
**                              shall be given through this param
**       dOption <IN> :         Specifies whether the peer session 
**                              need to be computed and this is a call for 
**                              negotiation or just for comparison
**	     pError	(OUT) :         Error structure in case the function 
**							    fails
******************************************************************/
#ifdef SDF_SESSIONCHANGE
Sdf_ty_matchMediaResult sdf_ivk_uaHandleMedia 
   _ARGS_((Sdf_st_mediaHandling *pMediaInfo,
   Sdf_st_overlapTransInfo* pOverlapTransInfo,
   SipMessage *pRecdSipMessage, Sdf_ty_slist *slAcceptedMedia,
   Sdf_st_sessionParam  **ppNgtdSession,
   Sdf_st_sessionChange **ppSessionChange, Sdf_ty_options dOptions,
   Sdf_st_error *pError));
#else
Sdf_ty_matchMediaResult sdf_ivk_uaHandleMedia 
   _ARGS_((Sdf_st_mediaHandling *pMediaInfo,
   Sdf_st_overlapTransInfo* pOverlapTransInfo,
   SipMessage *pRecdSipMessage, Sdf_ty_slist *slAcceptedMedia,
   Sdf_st_sessionParam  **ppNgtdSession,
   Sdf_st_error *pError));
#endif

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetLastOfferRecvd
**
** DESCRIPTION: This function sets the LastOffer received from the peer
**				in the media capabilities of the Call Object
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media-Info structure
**		pLastOffer(IN)	: The offer recvd from the peer (sdf_st_sessionParam)
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetLastOfferRecvd
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pLastOffer, \
	Sdf_st_error *pError));
	

/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetLastOfferSent
**
** DESCRIPTION: This function sets the LastOffer sent by the UATK
**				in the media capabilities of the Call Object
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media-Info structure
**		pLastOffer(IN)	: The offer sent to the peer (sdf_st_sessionParam)
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetLastOfferMade\
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pLastOffer, \
	Sdf_st_error *pError));


	
/***********************************************************************
** FUNCTION:  sdf_ivk_uaSetCurrentSession
**
** DESCRIPTION: This function sets the current session by the UATK
**				in the media capabilities of the Call Object
**
** PARAMETERS:
**		pMediaInfo(IN/OUT)	: The Media-Info structure
**		pSession(IN)	: The offer sent to the peer (sdf_st_sessionParam)
**		pError(OUT)		: Error structure in case the function fails
**
************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaSetCurrentSession\
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam *pSession, \
	Sdf_st_error *pError));

/******************************************************************************
** FUNCTION: sdf_ivk_uaFormSessionParam
**
** DESCRIPTION: The API will convert the incoming SDP message into 
**				Sdf_st_sessionParam Structure. MatchMedia Funciton will invoke 
**				this funciton to get the session param structure from the 
**				incoming message.
**
**				In addition to conversion the API will also do some validations
**				on the incoming SDP mesasage. Such as, Imporper use of fid(group
**				ing two media streams which have the same IP and port) absence 
**				of connection line at both session level and media level, and 
**				also checks for the presence of the o line.
** PARAMETERS:
**		pSdpMessage[IN]		: The SDP Message which needs to be converted
**		pSessionParams[OUT]	: The converted session param structure
**		dType[IN]			: The conversion type which is used in updating
**							  the version of the origin line.
**		pErr[OUT]			: Error structure in case the function fails
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSessionParam\
	_ARGS_((SdpMessage 			*pSdpMessage,\
	Sdf_st_sessionParam	*pSessionParams,\
	Sdf_ty_conversionType	dType,\
	Sdf_st_error *pErr));
/******************************************************************************
** FUNCTION: sdf_ivk_uaFormSdpMessageFromSessionParam
**
** DESCRIPTION: The API will convert the session param structure into a SDP
**				message
** PARAMETERS:
**		pSdpMessage[IN]		: The formed SDP Message. 
**		pSessionParams[OUT]	: The session param structure which needs to be conv**							  erted
**		dType[IN]			: The conversion type which is used in setting
**							  the version of the origin line.
**		pErr[OUT]			: Error structure in case the function fails
*******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSdpMessageFromSessionParam\
	_ARGS_((SdpMessage 			*pSdpMessage,\
	Sdf_st_sessionParam	*pSessionParams,\
	Sdf_ty_conversionType	dType,\
	Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: sdf_ivk_uaFormSdpMediaFromMediaStream	    	
 **
 ** DESCRIPTION: This API Forms the SdpMedia structure based on the info 
 **              available in the Sdf_st_MediaStream.	
 **
 ** PARAMETERS:	
 **      Pstream[IN]:	The Media stream to be converted to SdpMedia
 **      Ppmedia[OUT]:	The SdpMedia output
 **		 pError[OUT]	 :	Error variable returned in case of error.
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormSdpMediaFromMediaStream
		_ARGS_((Sdf_st_mediaStream *pStream,
		SdpMedia **ppSdpMedia,
		Sdf_st_error *pError));
/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaInitiateOffer
 **
 ** DESCRIPTION: 	
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitiateOffer 
	_ARGS_((Sdf_st_sessionParam	**ppSessionParam,
	 Sdf_st_callObject		*pCallObject,
	 Sdf_ty_offerType	 	 dOfferType,
 	 Sdf_st_error		*pErr));
	
Sdf_ty_retVal	sdf_ivk_uaRejectSdp
	_ARGS_((SdpMessage		*pIncomingSdpMessage, 
	 SdpMessage 	*ppReturnSdpMessage,
	 Sdf_st_error	*pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaAddStreamsToCall
 **
 ** DESCRIPTION: This function is used to add a stream to an existing
 ** 			 call leg. (An additional m line is appended to the
 **				 to the previous negotiated session description when 
 **				 sending a Re-Invite)
 **
 **				Note: The user MUST call sdf_ivk_uaStartTransaction prior
 **						  to invoking this API.
 ** PARAMETERS:	pObject		(IN/OUT): The call object in which the
 **								 	  the SdpMessage has to be set.
 **				slMediaList	(IN)	: The list of Media streams to be added
 **				pError		(OUT)	: Error structure in case the 
 **								 	  function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddStreamsToCall _ARGS_((\
	Sdf_st_callObject *pObject, Sdf_ty_slist slMediaList,\
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaAddStreams
 **
 ** DESCRIPTION: This function is used to add a stream to a given Session Param
 **              structure.
 **
 ** PARAMETERS:	pSessionParams(IN/OUT): The SessionParams str. to which the
 **									  MediaStreams has to be added.
 **				slMediaList	(IN)	: The list of Media streams to be added
 **				pError		(OUT)	: Error structure in case the 
 **								 	  function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaAddStreams _ARGS_((\
	Sdf_st_sessionParam *pSessionParams, Sdf_ty_slist slMediaList,\
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaDeleteStreamsFromCall
 **
 ** DESCRIPTION: This function removes a stream from a call leg, 
 **				 either party sets port to zero in the session 
 **				 description for the media line corresponding to 
 ** 			 the stream to be deleted when sending a Re-Invite
 **
 ** PARAMETERS:	pObject	(IN/OUT): The call object in which the
 **								  the SdpMessage has to be set.
 **				pMediaFormats(IN): The Media foramts to be deleted
 **             pslMediaIDs(IN)	: List of MediaIDs (of MediaStreams) that needs
 **								  to be deleted.
 **				pError	(OUT)	: Error structure in case the 
 **								  function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaDeleteStreamsFromCall _ARGS_((\
	Sdf_st_callObject *pObject, Sdf_ty_s8bit *pMediaFormats,\
	Sdf_ty_slist *pslMediaIDs, Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaDeleteStreams
 **
 ** DESCRIPTION: This function removes a stream from a Session Param by setting
 **				 the port no in the m-line to 0.
 **
 ** PARAMETERS:	pSessionParams(IN/OUT): The SessionParam str. from which the 
 **								  MediaStreams has to be deleted.
 **				pMediaFormats(IN): The Media foramts to be deleted
 **             pslMediaIDs(IN)	: List of MediaIDs (of MediaStreams) that needs
 **								  to be deleted.
 **				pError	(OUT)	: Error structure in case the 
 **								  function fails
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaDeleteStreams _ARGS_((\
	Sdf_st_sessionParam *pSessionParams, Sdf_ty_s8bit *pMediaFormats,\
	Sdf_ty_slist *pslMediaIDs, Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION: sdf_ivk_uaFormSdpMediaStream
 **
 ** DESCRIPTION: This function removes a stream from a Session Param by setting
 **				 the port no in the m-line to 0.
 **
 ** PARAMETERS:	
 **			
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFormSdpMediaStream _ARGS_((Sdf_ty_s8bit *pMvalue, \
	Sdf_ty_u32bit dPort,Sdf_ty_u32bit dNumberOfPorts,Sdf_ty_s8bit *pTransport,\
	Sdf_ty_slist 	*pslCodecList, Sdf_ty_s8bit *pTypeOfTransport,	\
	Sdf_ty_slist *pslConnection, Sdf_st_mediaStream **ppSdpMediaStream,	\
	Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormCodec
 **
 ** DESCRIPTION: This function forms the Sdf_st_codec structure from 
 **				 either codec number or codec name.
 ** PARAMETERS:	pCodecName(IN)		: Identifies the Codec to be Added 
 **									  MediaStreams has to be deleted.
 **				dEncodingRate(IN)	: The Encoding rate for this Type of Codec 
 **             dChannels(IN)		: Number of Channes to be used
 **								  		to be deleted.
 **				dCodecNum (IN)		: The Standard Codec Number to be used  
 **				ppCodec	  (OUT)		: The Formed Codec Structure
 **				pErr	(OUT)		: Error structure in case the 
 **								  		function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormCodec _ARGS_((Sdf_ty_s8bit  *pCodecName, \
	Sdf_ty_u32bit dEncodingRate,  Sdf_ty_u8bit  dChannels,
	Sdf_ty_u8bit 	dCodecNum, Sdf_st_codec  **ppCodec, Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaFormMediaStreamFromSdpMedia
 **
 ** DESCRIPTION: This function fills Sdf_st_mediaStream structure from the given
 **				 SdpMedia Structure
 ** PARAMETERS:
 **				pMediaStream(OUT)	: Media Stream Which is Formed 
 **				pSdpMessage(IN)		: SdpMedia Structure which is converted 
 **				pErr	(OUT)		: Error structure in case the 
 **								  	  function fails
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormMediaStreamFromSdpMedia
	_ARGS_((Sdf_st_mediaStream *pMediaStream,
	 SdpMedia			*pSdpMedia,
	 Sdf_st_error		*pErr));


/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaUpdateMediaStreamInCall
 **
 ** DESCRIPTION: 	This function is used to modify the paramaters of an 
 **					Existing Media Stream.
 **
 **					Note: The user MUST call sdf_ivk_uaStartTransaction prior
 **						  to invoking this API.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateMediaStreamInCall _ARGS_(\
	(Sdf_st_callObject  *pObject, Sdf_st_mediaStream	*pMediaStream,	\
	 Sdf_st_error 		*pError));

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 	sdf_ivk_uaAddGroupToSession
 **
 ** DESCRIPTION: This function Forms a Group Attribute from the given Inputs
 **				 and adds it to the SessionParam
 ** PARAMETERS:
 **				pMediaInfo(IN)			: The Media-Info taken from CallObject
 **										  to which the Session corresponds 
 **				pSessionParams(IN/OUT)	: SessionParam Structure to work with 
 **				dGroupType(IN)			: The type of Group to be added 
 **				pOtherGroupType(IN)		: If Group is Other than FID or LS
 ** 									  application has to give the group name
 **				pslMid(IN)				: List of Mid's which needs to be
 **										  grouped 
 **				pErr	(OUT)			: Error structure in case the 
 **								  	  	  function fails

 **				 
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaAddGroupToSession
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_ty_groupType		dGroupType,
	 Sdf_ty_s8bit			*pOtherGroupType,
	 Sdf_ty_slist			*pslMid,
	 Sdf_st_error		*pErr));
#endif

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaSetSessionCapability
 **
 ** DESCRIPTION: This
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSessionCapability
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo,
	 Sdf_st_sessionParam	*pSessionParam,
	Sdf_st_error *pError));
/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFillDefaultSdpInMsgBodyList
 **
 ** DESCRIPTION: This function makes a SDP body using pMedaInfo->pCapabilities 
 **				 structure in MediaInfo of call object and sets the body formed into 
 **				 the MsgBodyList Passed as Parameter 
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFillDefaultSdpInMsgBodyList
	_ARGS_((Sdf_st_callObject *pObject, 
	Sdf_ty_slist *pMsgBodyList,
	Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: sdf_ivk_uaFormMediaCapabilityDescription
 **
 ** DESCRIPTION: This API forms the SdpMessage structure using the
 **				 Capability present in the Call Object that can be sent out
 **				 in an OPTIONS request.
 **
 ** PARAMETERS:
 **	 pMediaInfo		(IN)	:	The Media-Info taken form CallObject from which
 **								the SDP Message is to be formed.
 **	 ppCapabilities	(OUT)	:	The formed SDP Capability
 **
 **	 pError			(OUT)	:	Error structure in case the function 
 **								fails
 **
 ******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFormMediaCapabilityDescription 
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, SdpMessage **ppCapabilities,\
	 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 		sdf_ivk_uaUpdateLocalSession
 **
 ** DESCRIPTION: 
 **				   If dAddStream = Sdf_co_true, then list contains the 
 **				   list of media streams to be added.
 **				   If dAddStream = Sdf_co_false, then list contains the 
 **					list of mids to be deleted.
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaUpdateLocalSession
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo,
	 Sdf_ty_slist		*slMediaList,
	 Sdf_ty_bool		dAddStream,
	 Sdf_st_error		*pError));

#ifdef SDF_IM
/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg 
 **
 ** DESCRIPTION: This function creates a message body of type text/plain from
 **				the text passed as input and Sets it to the sip message passed. 
 **	PARAMATERS:
 **		pSipMsg (IN): The SIP message to which the text/plain message body 
 **					need to be attached.
 **		pPlainText(IN): The Plain text which is used to form the message body
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFormAndSetPlainTextMsgBodyToSipMsg _ARGS_(( \
 SipMessage *pSipMsg, Sdf_ty_s8bit	*pPlainText, Sdf_st_error *pError));
#endif

/****************************************************************************
 ** FUNCTION: 	 Sdf_ivk_uaInitAndFormCline 
 **
 ** DESCRIPTION: This function populate the connection line structure from 
 **              Sdf_st_cLineInSdp structure.All the fields of connection line
 **              are populated using various fields of Sdf_st_clineInSdp.
 **
 ****************************************************************************/
Sdf_ty_retVal Sdf_ivk_uaInitAndFormCline
    _ARGS_((Sdf_st_cLineInSdp 	  *pCLineInSdp,
     SdpConnection		  **ppConnection,
     Sdf_st_error         *pErr));

/****************************************************************************
 ** FUNCTION: 	 Sdf_ivk_uaInitAndFormMline 
 **
 ** DESCRIPTION: This function populate the SdpMedia structure from 
 **              Sdf_st_mLineInSdp structure.All the fields of media line
 **              are populated using various fields of Sdf_st_mlineInSdp.
 **
 ****************************************************************************/
Sdf_ty_retVal Sdf_ivk_uaInitAndFormMline
    _ARGS_((Sdf_st_mLineInSdp 	  *pMLineInSdp,
     SdpMedia             **ppMedia,
     Sdf_st_error         *pErr));

/****************************************************************************
 ** FUNCTION: 	 Sdf_ivk_uaSetSessionLevelAttrInSdpMsg 
 **
 ** DESCRIPTION: This function is used to set the various session level
 **              attributes in SDP message.This fuction takes a string value 
 **              as an input and depending upon the value of first character 
 **              appropriate stack function is invoked  
 **
 ****************************************************************************/
Sdf_ty_retVal Sdf_ivk_uaSetSessionLevelAttrInSdpMsg
    _ARGS_((SdpMessage 	  *pSdpMessage,
     Sdf_st_string    dAttr,
     Sdf_st_error     *pErr));

/****************************************************************************
 ** FUNCTION: 	 Sdf_ivk_uaSetMediaLevelAttrInSdpMsg 
 **
 ** DESCRIPTION: This function is used to set the various media level
 **              attributes in SDP message.This fuction takes a string value 
 **              as an input and depending upon the value of first character 
 **              appropriate stack function is invoked.The media line in which
 **              the attribute need to be inserted can be determined by
 **              dMediaIndex field.
 **
 ****************************************************************************/
Sdf_ty_retVal Sdf_ivk_uaSetMediaLevelAttrInSdpMsg
    _ARGS_((SdpMessage 	  *pSdpMessage,
     Sdf_st_string    dAttr,
     Sdf_ty_u8bit     dMediaIndex, 
     Sdf_st_error     *pErr));

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetClineFromSdpMessage 
 **
 ** DESCRIPTION: This function is used to get the connection line received in 
 **              SDP message in Sdf_st_cLineInSdp structure.All the fields of
 **              connection line will be filled in Sdf_st_cLineInSdp at
 **              respective placeholder.
 ****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetClineFromSdpMessage
    _ARGS_((SdpMessage 	     *pSdpMessage,
     Sdf_st_cLineInSdp   *pCLineInSdp,
     Sdf_st_error        *pErr));

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetElementFromSessionAttr 
 **
 ** DESCRIPTION: This function is used to get the session level attributes
 **              from the SDP message.The attribute which needs to be
 **              extracted from the SDP message will be decided by the dType
 **              argument passed as input to this function.
 ****************************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetElementFromSessionAttr
    _ARGS_((SdpMessage 	    *pSdpMessage,
     Sdf_ty_u8bit           *pAttrStr,
     Sdf_ty_sessionAttrType dType,
     Sdf_st_error           *pErr));

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetSizeOfSessionAttrList 
 **
 ** DESCRIPTION: This function is used to get the size of list, of paricular 
 **              type which is detrmine by the dType field.
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSizeOfSessionAttrList
    _ARGS_((SdpMessage 	         *pSdpMessage,
     Sdf_ty_sessionAttrListType  dType,
     Sdf_ty_u32bit               *pSize,
     Sdf_st_error                *pErr));

/****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaGetElementFromSessionAttrList 
 **
 ** DESCRIPTION: This function is used to get the particular elements from SDP
 **              message which are maintain as a list.The type of element or
 **              the list from which the attribute is requited is determined
 **              by dType filed , whereas the index in that particular list 
 **              will be specified by dIndex parameter.
 ****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetElementFromSessionAttrList
    _ARGS_((SdpMessage 	         *pSdpMessage,
     Sdf_ty_u8bit                *pAttrStr,
     Sdf_ty_sessionAttrListType  dType,
     Sdf_ty_u8bit                dIndex,
     Sdf_st_error                *pErr));

#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
