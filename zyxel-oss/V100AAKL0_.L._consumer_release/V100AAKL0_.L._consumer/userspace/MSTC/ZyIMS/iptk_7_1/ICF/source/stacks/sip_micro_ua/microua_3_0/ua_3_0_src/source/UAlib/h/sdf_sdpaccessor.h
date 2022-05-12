/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/***********************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various SDP structures in the UA toolkit
 *********************************************************************
 **
 ** FILENAME		: sdf_sdpaccessor.h
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various sdp structures in the UA toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 30/11/02	Pankaj Nath						Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#ifndef __SDF_SDP_ACCESSOR_H_
#define __SDF_SDP_ACCESSOR_H_


/******************************************************************
**
** FUNCTION:  __sdf_cloneSdfStringList
**
** DESCRIPTION:  This function makes a deep copy of a SipList of
** SIP_S8bit *  from the "source" to "dest".
**
******************************************************************/
Sdf_ty_retVal __sdf_cloneSdfStringList _ARGS_((\
	Sdf_ty_slist *dest, Sdf_ty_slist *source, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneCodec
 **
 ** DESCRIPTION: This function copies the Sdf_st_codec structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneCodec _ARGS_((Sdf_st_codec *pDestCodec,\
	Sdf_st_codec *pSrcCodec, Sdf_st_error *pErr));

#ifdef SDF_MEDIAGROUP
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStreamGroup
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaStreamGroup 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStreamGroup _ARGS_((\
	Sdf_st_mediaStreamGroup *pDestMediaStreamGroup,\
	Sdf_st_mediaStreamGroup *pSrcMediaStreamGroup, Sdf_st_error *pErr));
#endif

#ifdef SDF_QOS
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneQosInfo
 **
 ** DESCRIPTION: This function copies the Sdf_st_qosInfo 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneQosInfo _ARGS_((Sdf_st_qosInfo *pDestQosInfo,\
	Sdf_st_qosInfo *pSrcQosInfo,Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSegmentedQosDetails
 **
 ** DESCRIPTION: This function copies the Sdf_st_segmentedQosDetails 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSegmentedQosDetails _ARGS_((\
	Sdf_st_segmentedQosDetails *pDestSegmentedQosDetails,\
	Sdf_st_segmentedQosDetails *pSrcSegmentedQosDetails,Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneE2eQosDetails
 **
 ** DESCRIPTION: This function copies the Sdf_st_e2eQosDetails 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneE2eQosDetails _ARGS_((\
	Sdf_st_e2eQosDetails *pDestE2eQosDetails,\
	Sdf_st_e2eQosDetails *pSrcE2eQosDetails, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneQosAttrib
 **
 ** DESCRIPTION: This function copies the Sdf_st_qosAttrib 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneQosAttrib _ARGS_((\
	Sdf_st_qosAttrib *pDestQosAttrib,\
	Sdf_st_qosAttrib *pSrcQosAttrib,Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetQosType
 **
 ** DESCRIPTION: This function gets the QosType from qosAttrib structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetQosType _ARGS_((\
    Sdf_st_qosAttrib *pQosAttrib, Sdf_ty_qosType *pQosType,\
    Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetSegmentQosDetails
 **
 ** DESCRIPTION: This function gets the SegmentedQos from the qosAttrib str.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSegmentQosDetails _ARGS_((\
    Sdf_st_qosAttrib *pCurrQos, Sdf_st_segmentedQosDetails **ppSegmentedQos,\
    Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetE2eQosDetails
 **
 ** DESCRIPTION: This function gets the End to End Qos Attrib from the 
 **              qosAttrib str.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetE2eQosDetails _ARGS_((\
    Sdf_st_qosAttrib *pDesQos, Sdf_st_e2eQosDetails **ppE2eQos,\
    Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetQosFromMediaStream
 **
 ** DESCRIPTION: This function gets the Qos details from the Media Stream
 **				 str. based on the type of reservation.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetQosFromMediaStream _ARGS_((\
	Sdf_st_mediaStream *pMediaStream, Sdf_st_qosAttrib **ppQosAttrib,\
	Sdf_ty_reservTypeInfo dReservType, Sdf_st_error *pErr));

#endif

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionAttrib
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionAttrib structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionAttrib _ARGS_((\
	Sdf_st_sessionAttrib *pDestSessionAttrib,\
	Sdf_st_sessionAttrib *pSrcSessionAttrib,Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStream
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaStream structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStream _ARGS_((\
	Sdf_st_mediaStream *pDestMediaStream,\
	Sdf_st_mediaStream *pSrcMediaStream, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionParam
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionParam structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionParam _ARGS_((\
	Sdf_st_sessionParam *pDestSessionParam,\
	Sdf_st_sessionParam *pSrcSessionParam,Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaHandling
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaHandling structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaHandling _ARGS_((\
	Sdf_st_mediaHandling *pDestMediaHandling,\
	Sdf_st_mediaHandling *pSrcMediaHandling, Sdf_st_error *pErr));

#ifdef SDF_SESSIONCHANGE
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaChange
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaChange structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaChange _ARGS_((\
	Sdf_st_mediaChange *pDestMediaChange,\
	Sdf_st_mediaChange *pSrcMediaChange, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionChange
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionChange structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionChange _ARGS_((\
	Sdf_st_sessionChange *pDestSessionChange,\
	Sdf_st_sessionChange *pSrcSessionChange, Sdf_st_error *pErr));
#endif


/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetMediaStreamAtIndex
 **
 ** DESCRIPTION: This function gets the MediaStream str. at a given index from
 **              the SessionParam structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamAtIndex _ARGS_((\
    Sdf_st_sessionParam *pSessionParam, Sdf_st_mediaStream **ppMediaStream,\
    Sdf_ty_u8bit index, Sdf_st_error *pErr));

/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetSessionParamsFromMediaInfo
 **
 ** DESCRIPTION: This function gets the session parameters from the Media 
 **				 Handling str. based on the type of SessionParam.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSessionParamsFromMediaInfo _ARGS_((\
    Sdf_st_mediaHandling *pMediaInfo, Sdf_st_sessionParam **ppSessionParam,\
    Sdf_ty_sessionParamType dSessionParamType, Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetMediaInfo
 **
 ** DESCRIPTION: This function retrieves the Media Handling structure from the
 **              Call Object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaInfo _ARGS_((\
    Sdf_st_callObject *pObject, Sdf_st_mediaHandling **ppMediaInfo,\
    Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:    sdf_ivk_uaGetSessionConnection
 **
 ** DESCRIPTION: This function retrieves the session level connection from the
 **              Session Param structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSessionConnection _ARGS_((\
    Sdf_st_sessionParam *pSessionParams, SdpConnection **ppConnection,\
    Sdf_st_error *pErr));


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetSdpFromSessionParam
 **
 ** DESCRIPTION: This function retrieves the session level SdpMessage from the
 **              Session Param structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSdpFromSessionParam _ARGS_((\
	Sdf_st_sessionParam *pSessionParams, SdpMessage **ppSdpMsg,\
	Sdf_st_error *pErr));


#ifdef SDF_MEDIAGROUP
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaStreamGroupAtIndex
 **
 ** DESCRIPTION: This function gets the MediaStreamGroup str. at a given 
 **              index from the SessionParam structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamGroupAtIndex _ARGS_((\
	Sdf_st_sessionParam	*pSessionParams,\
   	Sdf_st_mediaStreamGroup **ppMediaStreamGroup, Sdf_ty_u8bit index,\
	Sdf_st_error *pErr));
#endif


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaStreamByMid
 **
 ** DESCRIPTION: This function gets the MediaStream str. based on the MID 
 **              passed.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamByMid _ARGS_((\
	Sdf_st_sessionParam	*pSessionParams, Sdf_st_mediaStream **ppMediaStream,\
	Sdf_ty_s8bit *pMid, Sdf_st_error *pErr));

#ifdef SDF_MEDIAGROUP
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStreamGroup
 **
 ** DESCRIPTION: This function copies the Array of Sdf_st_mediaStreamGroup 
 **				structures from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStreamGroupArray
_ARGS_((Sdf_st_mediaStreamGroup ***pppDestMediaStreamGroup,
Sdf_st_mediaStreamGroup **ppSrcMediaStreamGroup,
Sdf_ty_u8bit dNumGroups, Sdf_st_error *pErr));
#endif

/*****************************************************************************
 ** FUNCTION: Sdf_ivk_uaGetSessionParamFromSipMsg
 **
 ** DESCRIPTION: This API returns the session param from the sdp message 
 **              present in the sip message
 ** PARAMETERS:	
 **		pSipMsg[IN] 		: The received Sip message
 **     ppSessionParam[OUT] 	: The Session Param 
 **		dConversionType[IN]		: Specifies whether the SipMsg is from Remote or
 ** 								Local Party
 **		pError[OUT]	 			:	Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetSessionParamFromSipMsg
		_ARGS_((SipMessage *pSipMsg, Sdf_st_sessionParam
					**ppSessionParam,Sdf_ty_conversionType dConversionType,
					Sdf_st_error *pError));

/*****************************************************************************
 ** FUNCTION: Sdf_ivk_uaGetSessionParamFromInitData
 **
 ** DESCRIPTION: This API returns the session param from the InitData
 ** PARAMETERS:	
 **		pInitData[IN] 		: The Init data
 **     pSessionParam[OUT]  : The Session Param 
 **		pError[OUT]	 		: Error variable returned in case of error.
 **
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetSessionParamFromInitData
	_ARGS_((Sdf_st_initData *pInitData, Sdf_st_sessionParam **ppSessionParam,
		Sdf_st_error *pError));


#endif	
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 

