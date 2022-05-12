/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** FUNCTION:
 ** 	Has all internal APIs needed for SDP handling within the 
 ** 	UA Toolkit.
 ******************************************************************************
 **
 ** FILENAME: 		sdf_sdpinternal.h
 **
 ** DESCRIPTION:  	This file contains internal APIs not exposed to the 
 **					application developer.
 **
 **
 ** DATE        NAME                REFERENCE               REASON
 ** ----        ----                ---------              --------
 ** 26-02-2001  C Rajasri,									Initial
 **				Siddharth
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_SDPINTERNAL_H_
#define __SDF_SDPINTERNAL_H_


#define Sdf_co_fmtSize		40
#define Sdf_co_payloadSize	100

/******************************* 
 * Funciton Pointer            * 
 *******************************/

/*
 * The function to used to Extract the Count
 */
typedef SipBool (*Sdf_ty_sdpCountFuncPtr) ( SdpMessage *, SIP_U32bit *, SipError *);

/*
 * The function to used to Extract the Element
 */
typedef SipBool (*Sdf_ty_sdpListGetFuncPtr)( SdpMessage*,SIP_S8bit**,SIP_U32bit, SipError*);

/*
 * The function to used to Insert a List Element in SdpMessage 
 */
typedef SipBool (*Sdf_ty_sdpListInsertFuncPtr)( SdpMessage*,SIP_S8bit*,SIP_U32bit, SipError*);

/******************************************************************************
 ** FUNCTION:  sdf_fn_uaMapPayloadFromCodec
 **
 ** DESCRIPTION: This API returns the payload associated with
 ** 			 the codec. It takes in the codec in string form 
 **				 (Eg. GSM/8000) and returns the payload associated with
 ** 			 the codec ("3" in the case of this example).
 **
 ** PARAMETERS:
 **	 pPayload	(IN) 	: The codec for which the payload is returned. 
 **	 pError		(OUT)	: Error structure returned in case the API fails.
 **
 ******************************************************************************/
  Sdf_ty_s8bit *sdf_fn_uaMapPayloadFromCodec _ARGS_((Sdf_ty_s8bit \
	*pCodec,Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaMapCodecFromPayload
 **
 ** DESCRIPTION: This API returns the codec associated with the payload.
 **				 This API takes in the payload in string form (Eg. "0")
 **				 and returns the codec associated with the payload 
 **				 ("PCMU/8000" in the case of this example).
 **
 ** PARAMETERS:
 **	 pPayload	(IN)  : The payload for which the codec is to be returned. 
 **	 pError		(OUT) : Error structure returned in case the API fails.
 **
 ******************************************************************************/
  Sdf_ty_s8bit *sdf_fn_uaMapCodecFromPayload _ARGS_((Sdf_ty_s8bit \
	*pPayload,Sdf_st_error *pError));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaCloneSipMsgBodyList
 **
 ** DESCRIPTION: This function makes a deep copy of a list of SipMsgBody 
 **				 structures.
 **
 ** PARAMETERS:
 **		pSource	(IN)	: The list of SipMsgBody from which a deep copy
 **					 	  is made. 
 **		pDest	(OUT) 	: A list of SipMsgBody into which a deep copy.
 **					 	  is place.
 **		pError	(OUT)	: Error structure returned in case the API fails.
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaCloneSipMsgBodyList _ARGS_((Sdf_ty_slist \
	*pDest, Sdf_ty_slist *pSrc, Sdf_st_error *pErr));
	
	

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaDeleteSdpFromMsgBodyList
 **
 ** DESCRIPTION: Finds and deletes the SDP msgbody from the msgbodylist
 **
 **	PARAMETERS:
 **		pMsgBodyList(IN): The message body list from which the SDP
 **							is to be delted
 **		pError(OUT)	   	: Error structure in case the API fails
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaDeleteSdpFromMsgBodyList 
  	_ARGS_((Sdf_ty_slist *pMsgBodyList, Sdf_st_error *pError));




/******************************************************************************
 ** FUNCTION: 	 	sdf_fn_uaModifyConnectionInSdpMessage
 **
 ** DESCRIPTION: 	This function adds the "c=" line to the	SdpMessage 
 **					that is taken as input. If a c= line is already present
 **					in the SdpMessage taken, it is left untouched.
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaModifyConnectionInSdpMessage \
	_ARGS_((Sdf_st_initData *pInitData, SdpMessage *pNegotiatedSdpMessage, \
	Sdf_st_error *pError));




/******************************************************************************
 ** FUNCTION: sdf_fn_uaModifyOriginInSdpMessage
 **
 ** DESCRIPTION: This function modifies the "o=" line of the SdpMessage that 
 **				 was passed to fill in own details in the initdata. If 
 **				 an o= line was present already, it is left untouched.
 **
 ******************************************************************************/
  Sdf_ty_retVal sdf_fn_uaModifyOriginInSdpMessage \
	_ARGS_((Sdf_st_initData *pInitData, SdpMessage \
	*pNegotiatedSdpMessage, Sdf_st_error *pError));


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetSdpMessageFromDefaults
 **
 ** DESCRIPTION: This function gets the SDP message stored in the initData 
 **				 structure that is passed. The SDP message returned has its 
 **				 ref count incremented & so it would have to be freed after use
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetSdpMessageFromDefaults \
	_ARGS_((Sdf_st_initData *pInitData, SdpMessage **ppMessage, \
	Sdf_st_error *pErr));




#ifdef SDF_TRACE
  /*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaTraceSdpMessageInSessionParam
 **
 ** DESCRIPTION: 	This function converts the Session Param Structure to
 **					SdpMessage structure & calls sdf_fn_uaTraceMediaLines
 **					which prints the contents in string format.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaTraceSdpMessageInSessionParam
	_ARGS_((Sdf_st_callObject *pObject, Sdf_st_sessionParam *pNgtdSession, \
	Sdf_ty_s8bit *pStr, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaTraceMediaLines
 **
 ** DESCRIPTION: This function displays all the m= lines which are present 
 **				 in the SdpMessage being passed. The parameter pStr should 
 **				 be one of "incoming" or "negotiated"
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaTraceMediaLines \
	_ARGS_((Sdf_st_callObject *pObject, SdpMessage *pSdpMsg, \
	Sdf_ty_s8bit *pStr));
 #endif


/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaCheckAndAddMandatoryLines
 **
 ** DESCRIPTION: This function checks whether the SDP message taken 
 **				 as input contains the mandatory lines, viz  v,o,c,t,s
 **				 If the lines are missing, the same are filled here using
 **				 the initdata.
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCheckAndAddMandatoryLines 
 	_ARGS_((Sdf_st_initData *pInitData, SdpMessage *pSdpMsg, \
	Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetContentTypeString
 **
 ** DESCRIPTION: 	This function returns a string for use in the content-type
 **					header. The string is returned depending on the SipMsgBody
 **					dType.
 **
 ** RETURN VALUES:	"application/sdp"		 - if dType is SipSdpBody
 **					"application/isup"		 - if dType is SipIsupBody
 **					<application set string> - if dType is unknown but a content
 **											   type header was present in the 
 **											   SipMsgBody structure.
 **					"text/plain"			 - if dType is SipUnknownBody and
 **											   no Content-Type header in the 
 **											   message body to tell what it is
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetContentTypeString \
	_ARGS_((SipMsgBody *pMsgBody, Sdf_ty_s8bit **ppCtypeString, \
			Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: 	   Sdf_fn_uaCompareMediaStreams	
 **
 ** DESCRIPTION:   This API compares two media streams in the session param 
 **                provided and provides the negotiated Session parameter 
 **                and the list of media streams that were acceptable.  	
 **
 ** PARAMETERS:	
 **     PsessionParamNew[IN]:	SessionParam to be negotiated.
 **                             with this param.
 **     PnegotiatedSessionParam [IN/OUT]:	The negotiated session params based on
 **                                     the comparison
 **     SlAcceptedMedia[OUT]:	Specifies the list of media lines that are 
 **                             matched.
 **     Perror[IN/OUT]:       	Specifies the error.
 **  
 *****************************************************************************/
#ifdef SDF_SESSIONCHANGE
Sdf_ty_matchMediaResult sdf_fn_uaCompareMediaStreams\
		_ARGS_((Sdf_st_sessionParam *pNegotiatedSessionParam, \
		Sdf_st_sessionParam *pSessionParamNew,\
		Sdf_st_mediaHandling *pMediaInfo, \
		Sdf_ty_slist *slAcceptedMedia,\
		Sdf_st_sessionChange *pSessionChange, Sdf_ty_options dOptions,\
		Sdf_st_error *pError));
#else
Sdf_ty_matchMediaResult sdf_fn_uaCompareMediaStreams\
		_ARGS_((Sdf_st_sessionParam *pNegotiatedSessionParam, \
		Sdf_st_sessionParam *pSessionParamNew,\
		Sdf_st_mediaHandling *pMediaInfo, \
		Sdf_ty_slist *slAcceptedMedia,\
		Sdf_st_error *pError));
#endif

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaHandleGroupInSessionParam 	    	
 **
 ** DESCRIPTION: This API compares the groups from the session parameters 
 **              for the validity based on the fid draft. If the grouping 
 **              is not valid then it ignores the corresponding group. 	
 **
 ** PARAMETERS:	
 **		PsessionParamNgtd[IN/OUT] :	SessionParam to be negotiated.
 ** 	PsessionParamOld[IN] :	Output will be given in this argument
 **     pObject     [IN]     : The callobject   
 **     dOptions    [IN]     :  Options to specify whether the peer session
 **                             need to be updated
 **		pError		[OUT]	 :	Error variable returned in case of error.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaHandleGroupInSessionParam\
		_ARGS_((Sdf_st_sessionParam *pSessionParamNew,\
		Sdf_st_sessionParam *pSessionParamOld,\
		Sdf_st_sessionParam *pSessionParamNgtd,\
		Sdf_st_mediaHandling *pMediaInfo,\
		Sdf_st_error *pError));
#endif

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaIsMediaTypeAndTransportSame	    	
 **
 ** DESCRIPTION: This API compares the media type and the transport protocol
 **              and returns success if they match else it returns failure.  	
 ** PARAMETERS:	
 **		pStream1[IN] :	Stream to be compared.
 **		PStream2[IN] :	Stream to be compared with pStream1
 **		pError[OUT]	 :	Error variable returned in case of error.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaIsMediaTypeAndTransportSame
		_ARGS_((Sdf_st_mediaStream *pStream1,
		Sdf_st_mediaStream *pStream2, Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: sdf_fn_uaCompareDirectionAttrib	    	
 **
 ** DESCRIPTION: This API compares the direction attribute based on the
 **              followingrule and return success/failure
 **  				sendonly-sendonly 	OR 	recvonly-recvonly ---> failure.
 ** 				sendonly-recvonly 	OR 	recvonly-sendonly ---> success.
 ** 				sendonly-sendrecv 	OR 	recvonly-sendrecv ---> success.
 **       			If pDirection1 is "inactive" or "sendrecv"---> success.
 **
 ** PARAMETERS:	
 **     dRecdDirection[IN] :Direction attribute to be compared
 **		dOldDirection[IN] :	Direction attribute to be compared with
 **     pSessionChange[OUT]: The change happened with the session
 **     pMediaChange[OUT]: The change happened with the media
 **     dOptions        :   Options 
 **		pError[OUT]	    :	Error variable returned in case of error.
 **
 *****************************************************************************/
#ifdef SDF_SESSIONCHANGE
 Sdf_ty_retVal sdf_fn_uaCompareDirectionAttrib
			_ARGS_((Sdf_ty_dirAttrib dRecdDirection,
			Sdf_ty_dirAttrib dOldDirection,
			Sdf_st_sessionChange *pSessionChange,
			Sdf_st_mediaChange *pMediaChange,
			Sdf_ty_options dOptions,
			Sdf_st_error *pError));
#else
 Sdf_ty_retVal sdf_fn_uaCompareDirectionAttrib
			_ARGS_((Sdf_ty_dirAttrib dRecdDirection,
			Sdf_ty_dirAttrib dOldDirection,
			Sdf_st_error *pError));
#endif

/*****************************************************************************
 ** FUNCTION: sdf_fn_uaChangeDirectionAttrib	    	
 **
 ** DESCRIPTION: This API changes the direction attribute based on the
 **              followingrule 
 **  				sendonly to recvonly.
 ** 				recvonly to	sendonly
 ** 				sendrecv/inactive to the local direction.
 **
 ** PARAMETERS:	
 **     pRecdDirection[IN] :Direction attribute to be changed 
 **		pError[OUT]	    :	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaChangeDirectionAttrib
			_ARGS_((Sdf_ty_dirAttrib *pRecdDirection,
			Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaIsSameVersion	    	
 **
 ** DESCRIPTION: This API compares the version and returns 
 **              success if they are same else it returns failure.  	
 **
 ** PARAMETERS:	
 **		 PsessionParam1[IN]:	The version of SessionParam to be compared.
 **		 PsessionParam2[IN]:	The version of SessionParam to be compared with.
 **		 pError[OUT]	 :	Error variable returned in case of error.
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaIsSameVersion
		_ARGS_((Sdf_st_sessionParam *pSessionParamNew,
		Sdf_st_sessionParam *pSessionParamOld,
		Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaMatchCodec 	    	
 **
 ** DESCRIPTION: This API matches the codec information and returns 
 **              success or failure based on the result.  	
 ** PARAMETERS:	
 **		PCodec1 :	Codec to be matched
 **		PCodec2	:   Codec to be matched with
 **		pSessionChange :   Session change information
 **     dOptions:   The options
 **		pError[OUT]	 :	Error variable returned in case of error.
 **
 *****************************************************************************/
#ifdef SDF_SESSIONCHANGE
 Sdf_ty_retVal sdf_fn_uaMatchCodec
		_ARGS_((Sdf_st_codec *pCodec1, Sdf_st_codec *pCodec2,
	    Sdf_st_sessionChange *pSessionChange,Sdf_st_mediaChange *pMediaChange,
	   	Sdf_ty_options dOptions,
		Sdf_st_error *pError));
#else
 Sdf_ty_retVal sdf_fn_uaMatchCodec
		_ARGS_((Sdf_st_codec *pCodec1, Sdf_st_codec *pCodec2,
		Sdf_st_error *pError));
#endif

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaGetSessionParamForNgtn 	    	
 **
 ** DESCRIPTION: This API returns the session param for the negotiation based 
 **              on the media state
 ** PARAMETERS:	
 **		pObject[IN] 		: The call object
 **     ppSessionParam[OUT] 	: The Session Param to be negotiated with
 **		pError[OUT]	 		:	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetSessionParamForNgtn
		_ARGS_((Sdf_st_mediaHandling *pMediaInfo,
		Sdf_st_sessionParam **ppSessionParam,
		Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaGetSdpMessageFromSipMessage
 **
 ** DESCRIPTION: This API returns the SDP messagefrom the 
 **              the sip message
 ** PARAMETERS:	
 **		pSipMsg[IN] 		: The received Sip message
 **     ppSdpMsg[OUT] 	    : The SDP message 
 **		pError[OUT]	 		:	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaGetSdpMessageFromSipMessage
		_ARGS_((SipMessage *pSipMsg, SdpMessage **ppSdpMsg,
		Sdf_st_error *pError));

#ifdef SDF_SESSIONCHANGE
/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckConnectionForChange 
 **
 ** DESCRIPTION: This API checks is there a change in the connection line
 **              by comparing 2 connection lines
 **
 ** PARAMETERS:	
 **		pConnectionNew[IN] 	: The received Connection 
 **     pConnectionOld[IN] 	: The old connection 
 **		pError[OUT]	 		: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckConnectionForChange
				_ARGS_((SdpConnection *pConnectionNew,
			     SdpConnection *pConnectionOld,
				 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckSessionForChange 
 **
 ** DESCRIPTION: This API returns the session change happened in the session
 **              level
 ** PARAMETERS:	
 **		pSessionParam1[IN] 	: The new SessionParam
 **     pSessionParam2[IN] 	: The local Session Param 
 **     pSessionChange[OUT]     : The Session change information
 **		pError[OUT]	 		    : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckSessionForChange
				_ARGS_((Sdf_st_sessionParam *pSessionParam1,
			     Sdf_st_sessionParam *pSessionParam2,
				 Sdf_st_sessionChange *pSessionChange,
				 Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCheckMediaStreamForChange 
 **
 ** DESCRIPTION: This API returns the session change happened in the session
 **              level
 ** PARAMETERS:	
 **		pMediaStreamNew[IN] 	: The received SessionParam
 **     pMediaStreamOld[IN] 	: The old Session Param 
 **     pMediaChange[OUT]     	: The Session change information
 **     pChanged[OUT]           : boolean specifies whether something changed
 **		pError[OUT]	 		    : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaCheckMediaStreamForChange
				_ARGS_((Sdf_st_mediaStream *pMediaStreamNew,
			     Sdf_st_mediaStream *pMediaStreamOld,
		 		 Sdf_st_mediaChange *pMediaChange,
		 		 Sdf_ty_bool        *pChanged,
				 Sdf_st_error *pError));
#endif

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaUpdateLocalPropertiesInMediaStream
 **
 ** DESCRIPTION: This API updates the given local media streams properties 
 **              into the negotiated stream
 ** PARAMETERS:	
 **		pMediaStreamNgtd[IN] 	: The Ngtd Media stream 
 **     pMediaStreamLocal[IN] 	: The Local Media stream
 **		pError[OUT]	 		    : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaUpdateLocalPropertiesInMediaStream
				_ARGS_((Sdf_st_mediaStream *pMediaStreamNgtd,
			     Sdf_st_mediaStream *pMediaStreamLocal,
				 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaUpdateLocalPropertiesInSessionParam
 **
 ** DESCRIPTION: This API updates the given local session param properties 
 **              into the negotiated session param
 ** PARAMETERS:	
 **		pMediaStreamNgtd[IN] 	: The Ngtd session param 
 **     pMediaStreamLocal[IN] 	: The Local session param
 **		pError[OUT]	 		    : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal Sdf_fn_uaUpdateLocalPropertiesInSessionParam
	_ARGS_((Sdf_st_mediaHandling *pMediaInfo, \
	 Sdf_st_sessionParam *pSessionParamNgtd, \
	 Sdf_st_sessionParam *pSessionParamLocal, Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: sdf_fn_uaFormMediaListFromSessionParam
 **
 ** DESCRIPTION: This API forms a SdpMedia list out of the given
 **				 session param
 **
 ** PARAMETERS:	
 **		pSessionParam[IN] 	: The session param from which SdpMedia 
 **							  need to be formed
 **     slAcceptedMedia[OUT]: List of SdpMedia structures
 **		pError[OUT]	 		: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaFormMediaListFromSessionParam 
				_ARGS_((Sdf_st_sessionParam *pSessionParam,
				 Sdf_ty_slist *slAcceptedMedia,
				 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCloneSessionParamWithoutMediaGroup
 **
 ** DESCRIPTION: This API clones the session param without the media group
 **              member of the session param
 **
 ** PARAMETERS:	
 **		pClonedSessionParam[OUT] 	: The session param cloned one 
 **     pSessionParam[IN]   : The session param to be cloned 
 **		pError[OUT]	 		: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCloneSessionParamWithoutMediaGroup
				_ARGS_((Sdf_st_sessionParam *pClonedSessionParam,
				 Sdf_st_sessionParam *pSessionParam,
				 Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCloneSdpConnectionList
 **
 ** DESCRIPTION: This API clones the Sdp connection list
 **
 ** PARAMETERS:	
 **		slClonedConnection[OUT] : The cloned sdp connection list
 **     slConnection[IN]   		: The sdp connection list to be cloned 
 **		pError[OUT]	 			: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCloneSdpConnectionList
				_ARGS_((Sdf_ty_slist *slClonedConnection,
				 Sdf_ty_slist *slConnection,
				 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: sdf_fn_uaCloneCodecList
 **
 ** DESCRIPTION: This API clones the Sdp connection list
 **
 ** PARAMETERS:	
 **		slClonedCodec[OUT] : The cloned codec list
 **     slCodec[IN]   	   : The codec list to be cloned 
 **		pError[OUT]	 	   : Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCloneCodecList
				_ARGS_((Sdf_ty_slist *slClonedCodec,
				 Sdf_ty_slist *slCodec,
				 Sdf_st_error *pError));


/*****************************************************************************
 ** FUNCTION: Sdf_fn_uaCloneStringList
 **
 ** DESCRIPTION: This API clones the list of strings
 **
 ** PARAMETERS:	
 **		slClonedConnection[OUT] : The cloned list
 **     slSrcList[IN]   		: The Src list to be cloned 
 **		pError[OUT]	 			: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_fn_uaCloneStringList
				_ARGS_((Sdf_ty_slist *slClonedList,
				 Sdf_ty_slist *slSrcList,
				 Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractSessionLevelAttribute
 **
 ** DESCRIPTION: 	The API extracts the session level attribute lines.
 **					This API treats the group and direction attribute specially
 **					and stores them on seperate place holders. Rest all attribut **					es go into the slOtherSessionAttributes member of Session
 **					Param.
 **	PARAMETERS:
 **		pSdpMessage	[IN]		:SdpMessage From which SessionAttributes will	 **								 be extracted
 **		pSessionParam[IN/OUT]	:Sessoin Param structure into which attrubutes
 ** 							 are stored
 **		pslGroupList [OUT]		: A list of group attributes returned.
 **
 **		pErr		[OUT]	 :	Error variable returned in case of error.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractSessionLevelAttribute \
	_ARGS_((SdpMessage *pSdpMessage, Sdf_st_sessionParam *pSessionParam,\
		Sdf_ty_slist *pslGroupList,Sdf_st_error *pError));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetSessionList
 **
 ** DESCRIPTION: 	This is helper API which is invoked from sdf_fn_uaExtractSes **					sionLevelAttribute.The API takes 2 funciton pointers,one 
 **					for extracting the count of attributes and other for extr
 **					acting the attribute itself.This API will be called to extra
 **					ct phone,email and bandwidth attributes.
 **	PARAMETERS:			
 **		pSdpMessage	[IN]		:SdpMessage From which SessionAttributes will	 **								 be extracted
 **		pslList		[OUT]		:List of attributes returned
 **		
 **		pListGetFuncPtr [IN]		:Funciton pointer to extract Attribute
 **
 **		pCountFunctPtr[IN]		:Funciton pointer to extract Count	
 **
 **		pErr		[OUT]	 :	Error variable returned in case of error.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGetSessionList \
	_ARGS_((SdpMessage	*pSdpMessage,Sdf_ty_slist *pslList,\
	Sdf_ty_sdpListGetFuncPtr 	pListGetFuncPtr,\
	Sdf_ty_sdpCountFuncPtr 	pCountFunctPtr,Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractMediaStreams
 **
 ** DESCRIPTION: 	API extracts all the Media Streams from the Message Body
 **
 **	PARAMETERS:			
 **		pSdpMessage	[IN]		:SdpMessage From which SessionAttributes will	 **								 be extracted
 **		pSessionParam[IN/OUT]	:Sessoin Param structure into which MediaStream
 ** 							 are stored
 **		pErr		[OUT]	 	:	Error variable returned in case of error.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractMediaStreams\
	_ARGS_((SdpMessage *pSdpMessage, Sdf_st_sessionParam *pSessionParam,\
			Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractMediaTypeFromMeida
 **
 ** DESCRIPTION: 	The API will extract the medis type from the media line  
 **					and store its corresponding ENUM in the structure
 **	PARAMETERS:
 **		pSdpMedia	 [IN]	 :SdpMedia From which Media type is extracted
 **		pMediaStream [OUT]:Media Struture in which the media type is stored
 **		pErr		 [OUT]	 :Error variable returned in case of error.

 *****************************************************************************/

Sdf_ty_retVal sdf_fn_uaExtractMediaTypeFromMeida \
		_ARGS_((SdpMedia *pSdpMedia,Sdf_st_mediaStream *pMediaStream,\
	 					Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaExtractTransportTypeFromMedia
 **
 ** DESCRIPTION: 	The API will extract the transport protocol from 
 ** 				the media line and store its corresponding ENUM 
 **					in the structure
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaExtractTransportTypeFromMedia\
	_ARGS_((SdpMedia *pSdpMedia,\
	 Sdf_st_mediaStream	*pMediaStream,\
	 Sdf_st_error		*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessCodecs
 **
 ** DESCRIPTION: 	The API will Extract the Format String and process the
 **					individual codecs and also associate the "rtmap" and
 **					"fmtp" attributes with them.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaProcessCodecs\
	_ARGS_((Sdf_st_mediaStream	*pMediaStream,\
	 SdpMedia			*pSdpMedia,\
	 Sdf_st_error		*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessMediaAttributes
 **
 ** DESCRIPTION: 	The API will process media level attributes such
 **					as "qos" and "direction" specially and other
 **					attributes will be pushed in slOtherAttribs field  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaProcessMediaAttributes \
	_ARGS_((Sdf_st_mediaStream	*pMediaStream,\
	 SdpMedia			*pSdpMedia,\
	 Sdf_st_error		*pErr));

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaProcessGroupings
 **
 ** DESCRIPTION: 	The API will process the session level Group lines
 **					and also does some validations on the use of FID and
 **					LS groupings
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaProcessGroupings \
	_ARGS_((Sdf_ty_slist			*pslGroupList,\
	 Sdf_st_sessionParam	*pSessionParam,\
	 Sdf_ty_u32bit			dSize,\
	Sdf_st_error			*pErr));
#endif

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormMandatoryLines
 **
 ** DESCRIPTION: 	The API will form the mandatory lines of the SDP message.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormMandatoryLines \
	_ARGS_((SdpMessage *pSdpMessage,\
	Sdf_st_sessionParam	*pSessionParam,\
	Sdf_ty_conversionType dType,\
	Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaSetOriginInSdp
 **
 ** DESCRIPTION: 	The API forms and sets the Origin line in the SdpMessage
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaSetOriginInSdp \
_ARGS_((SdpMessage *pSdpMessage,\
Sdf_st_sessionParam	*pSessionParam,\
Sdf_ty_conversionType dType,\
Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormOtherSessionAttributes
 **
 ** DESCRIPTION: 	The API sets all elements in slOtherSessionAttributes
 **					of Session Param into the Sdp Message
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormOtherSessionAttributes \
_ARGS_((SdpMessage *pSdpMessage,\
Sdf_st_sessionParam	*pSessionParam,\
Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormSessionAttributes
 **
 ** DESCRIPTION: 	The API sets all elements in slOtherSessionAttributes
 **					of Session Param into the Sdp Message
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormSessionAttributes \
_ARGS_((SdpMessage *pSdpMessage,\
Sdf_st_sessionParam	*pSessionParam,\
Sdf_st_error	*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetCodecTypeFromString
 **
 ** DESCRIPTION: 	
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaGetCodecTypeFromString \
	_ARGS_((Sdf_ty_s8bit	*pCodecStr,\
	 Sdf_ty_codecName	*pName));

/*****************************************************************************
 ** FUNCTION: 	sdf_fn_uaValidateConnectionLines	
 **
 ** DESCRIPTION: 	The API validates the presence of Connection Line either
 **					at Session Level or at each Media Level
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaValidateConnectionLines _ARGS_((Sdf_st_sessionParam\
		*pSessionParams,Sdf_st_error *pErr));
/*****************************************************************************
** FUNCTION: sdf_fn_uaInitMediaStateValidationTable
**
** DESCRIPTION: This API will initialize the Media State Transition table
*****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitMediaStateTable
_ARGS_((void));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateIdleState
 **
 ** DESCRIPTION: 	The API does media state related validation in Idle state 
 **					when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateIdleState \
 _ARGS_(( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,\
 		Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateOfferSent
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateOfferSentState \
 _ARGS_(( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,\
 					Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateOfferRecvState
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateOfferRecvState \
 _ARGS_((SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,\
 			Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateEstablishedState
 **
 ** DESCRIPTION: 	The API does media state related validation in OfferSent
 **					state when an incoming message is received
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaValidateEstablishedState \
 _ARGS_(( SipMessage	*pMessage ,Sdf_st_callObject *pCallObject,\
 				Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaSetOriginInfoInSdp
 **
 ** DESCRIPTION: 	The API Forms the Origin Line and sets it into the 
 **					SdpMessage
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaSetOriginInfoInSdp\
	_ARGS_((SdpMessage	*pSdpMessage,\
	 Sdf_st_sessionParam *pSessionParam,\
	 Sdf_ty_conversionType	dType,\
	 Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormSessionAttributes
 **
 ** DESCRIPTION: 	The API Forms Sets the Attributes present
 **					slOtherSessionAttributes into SdpMessage.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormSessionAttributes\
	_ARGS_((SdpMessage 			*pSdpMessage,\
	 Sdf_st_sessionParam	*pSessionParam,\
	 Sdf_st_error			*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormMediaStreams
 **
 ** DESCRIPTION: 	The API Forms m lines from the MediaStreams Structre
 **					and sets them into the SdpMessage.
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaFormMediaStreams\
	_ARGS_((SdpMessage *pSdpMessage,\
	Sdf_st_sessionParam	*pSessionParam,\
	 Sdf_st_error			*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormDirectionAttribute
 **
 ** DESCRIPTION: 	The API Forms the Sdp Attribute from Direction
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormDirectionAttribute\
	_ARGS_((Sdf_ty_dirAttrib	dDirection,\
	 SdpAttr			**ppSdpAttr,\
	 Sdf_st_error		*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaUpdateDirectionAttribute
 **
 ** DESCRIPTION: 	The API Forms the Sdp Attribute from Direction
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaUpdateDirectionAttribute\
	_ARGS_((SdpMessage *pSdpMessage,\
	 Sdf_st_sessionParam	*pSessionParam,\
	 Sdf_st_error	*pErr));

#ifdef SDF_QOS
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormQosAttributesFromMediaStream
 **
 ** DESCRIPTION: 	The API Forms the qos attribute for the coressponfing
 **					Media Stream
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormQosAttributesFromMediaStream \
	_ARGS_((Sdf_st_mediaStream	*pMediaStream,\
	 SdpMedia			*pSdpMedia, Sdf_st_error		*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormQosAttributes
 **
 ** DESCRIPTION: 	Subfunction Invoked to form the qos attribute
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormQosAttributes \
	_ARGS_((Sdf_st_qosAttrib	*pQosAttrib,\
	 Sdf_ty_reservTypeInfo	dType,\
	 SdpMedia				*pSdpMedia,\
	 Sdf_st_error			*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaGetStringForStrength
 **
 ** DESCRIPTION: 	The API converts the strength ENUM into String
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaGetStringForStrength
	_ARGS_((Sdf_ty_qosStrength	dStrength,
	 Sdf_ty_s8bit		*pString));
#endif
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecNumberToName
 **
 ** DESCRIPTION: 	This API takes in the codec in u8bit form and returns the 
 **                 corresponding enum value of type Sdf_ty_codecName.
 **                 e.g. Sdf_en_PCMA for the input 8.
 *****************************************************************************/
Sdf_ty_codecName sdf_fn_uaMapCodecNumberToName \
	_ARGS_((Sdf_ty_u32bit dCodec,\
	Sdf_st_error   *pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecEnumToString
 **
 ** DESCRIPTION: 	This API takes in the codec enumeration and returns the 
 **                 corresponding String
 **                 e.g. "PCMA" for Sdf_en_PCMA.
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecEnumToString \
	_ARGS_((Sdf_ty_codecName	dCodec, \
	 Sdf_ty_s8bit		*pCodecString, \
	 Sdf_st_error		*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecStringToEnum
 **
 ** DESCRIPTION: 	The API maps a String to corresponding codec
 ** 					eg: "PCMU" -> Sdf_en_PCMU
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecStringToEnum \
	_ARGS_((Sdf_ty_s8bit	*pCodecString, Sdf_ty_codecName	*pCodecName));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaChangeMediaState
 **
 ** DESCRIPTION: 	The API changes the media state based on the current state 
 **					and the SDP message in outgoing message. 
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaChangeMediaState
	_ARGS_((Sdf_st_callObject	*pCallObject,
	 Sdf_ty_mediaStateChange	*pChange,
	 Sdf_st_msgInfo				dMessageInfo,
	 SipMessage			*pSipMsg,
	 Sdf_ty_msgMode		dMode,
	 Sdf_st_error		*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaIsSameIp
 **
 ** DESCRIPTION: 	The API checks if two media streams have the same IP 	
 *****************************************************************************/
Sdf_ty_bool	sdf_fn_uaIsSameIp
	_ARGS_((Sdf_st_mediaStream	*pMediaStream1,
	 Sdf_st_mediaStream *pMediaStream2,
	 SdpConnection 		*pSessionConnection,
	 Sdf_st_error		*pErr));


/*****************************************************************************
 ** FUNCTION:       sdf_fn_uaGenerateMid
 **
 ** DESCRIPTION:    This API clones the session param without the media group
 **                 member of the session param
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaGenerateMid
        _ARGS_((Sdf_ty_s8bit **ppMid, Sdf_st_error *pErr));

Sdf_ty_retVal	sdf_fn_uaFormAttributeFromString
	_ARGS_((Sdf_ty_s8bit	*pString,
	 SdpMedia		*pSdpMedia,
	 Sdf_st_error	*pErr));

Sdf_ty_retVal	sdf_fn_uaFormSessionListAttributes
	_ARGS_((SdpMessage				*pSdpMessage,
     Sdf_ty_sdpListInsertFuncPtr	pListInsertFuncptr,
	 Sdf_ty_slist			*pslList,
	 Sdf_st_error			*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaMapCodecNumToRtpmapString
 **
 ** DESCRIPTION: 	
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaMapCodecNumToRtpmapString
	_ARGS_((Sdf_ty_u32bit	dCodecNum,
	 Sdf_ty_s8bit		*pRtpString,
	 Sdf_st_error		*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaConvertMediaStreamListToSdpMediaList
 **
 ** DESCRIPTION: This API converts the given Sdf_st_mediaStream list to the 
 **              equivalent	SdpMedia list
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaConvertMediaStreamListToSdpMediaList
	_ARGS_((Sdf_ty_slist	*pSdpMediaList,
	 Sdf_ty_slist		*pMediaStreamList,
	 Sdf_st_error		*pErr));

#ifdef SDF_MEDIAGROUP
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaFormGroupAttribute
 **
 ** DESCRIPTION: 	The API Forms Group and Direction Attributes and sets them
 **					in SdpMessage
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaFormGroupAttribute\
	_ARGS_((SdpMessage *pSdpMessage,\
	 Sdf_st_sessionParam	*pSessionParam,\
	 Sdf_st_error	*pErr));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaRemoveMediaStreamFromGroup
 **
 ** DESCRIPTION: This API converts the given Sdf_st_mediaStream list to the 
 **              equivalent	SdpMedia list
 *****************************************************************************/
Sdf_ty_retVal	sdf_fn_uaRemoveMediaStreamFromGroup
	_ARGS_((Sdf_st_sessionParam	*pSessionParam,
	 Sdf_ty_u32bit			dIndex,
	 Sdf_st_error		*pError));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaIsGroupSupported
 **
 ** DESCRIPTION: This API checks if the Group is Supported by UATK 
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaIsGroupSupported
	_ARGS_((Sdf_st_mediaStreamGroup	*pMediaStreamGroup,
	 Sdf_st_error		*pError));
/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaValidateGroupings
 **
 ** DESCRIPTION: This API checks Validity of group Attribute 
 *****************************************************************************/
Sdf_ty_bool sdf_fn_uaValidateGroupings
	_ARGS_((Sdf_st_sessionParam *pSessionParam,
	Sdf_st_mediaStreamGroup	**ppMediaStreamGroups,
	 Sdf_ty_u32bit				dNumGroups,
	 Sdf_st_error		*pErr));

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaRemoveMediaStreamFromGrouping
 **
 ** DESCRIPTION: 	Removes the Index from all groupings
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaRemoveMediaStreamFromGrouping
	_ARGS_((Sdf_ty_u32bit	dIndex,
	 Sdf_st_sessionParam	*pSessionParam,
	 Sdf_st_error		*pError));
#endif

/*****************************************************************************
 ** FUNCTION: 		sdf_fn_uaConvertCodecListToMID
 **
 ** DESCRIPTION: Converts a string containing space-seperated string of codecs
 **				 to list of MIDs.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaConvertCodecListToMID _ARGS_((\
	Sdf_st_sessionParam *pSessionParams, Sdf_ty_slist *slMediaList,\
	Sdf_ty_s8bit *pMediaFormats, Sdf_st_error	*pError));


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaCloneSessionParamWithoutMediaStream
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionParam structure from 
 **				 src to dest without the media information
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaCloneSessionParamWithoutMediaStream _ARGS_((\
	Sdf_st_sessionParam *pDestSessionParam,\
	Sdf_st_sessionParam *pSrcSessionParam,Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaUpdateMediaStreamsInSessionParam
 **
 ** DESCRIPTION: This function updates the Sdf_st_sessionParam structure with 
 **				 the media information
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateMediaStreamsInSessionParam _ARGS_((\
	Sdf_st_sessionParam *pDestSessionParam,\
	Sdf_ty_slist *slAcceptedMedia,Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION: sdf_fn_uaStoreAllMsgBodies
 **
 ** DESCRIPTION: This function stores all the message bodies
 **		 		 in the SIP message in the CallObject's MsgBodyList
 **				 or in the overlapTransInfo passed.
 **
 ** PARAMETERS:
 **	 pObject	(IN)	: 	The Call Object whose Sip message is used. The 
 **							CallObjects MsgBodyList is updated with all the 
 **							message bodies in the SIP message.
 **	 pOverlapTransInfo(IN): The overlapTransInfo structure into which the 
 **							non-SDP bodies need to be stored. This parameter 
 **							may be NULL, indicating that the message bodies 
 **							should be updated into the callobject's list.
 **	 pError		(OUT)	: 	Error structure in case the function fails.
 **
 ******************************************************************************/
 Sdf_ty_retVal sdf_fn_uaStoreAllMsgBodies \
	_ARGS_((SipMessage *pSipMessage, Sdf_ty_slist *pslDestinationList, \
	Sdf_st_error *pError));



#endif
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
