/***********************************************************************
 ** FUNCTION:
 **			This file has the initialization functions for all SDP
 **			structures
 **			
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_sdpinit.c
 **
 ** DESCRIPTION		: This file has the initialization functions for 
 **						all SDP structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 29/11/02	Pankaj Nath					Creation
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/

#include "ALheader.h"
#include "sdf_sdpinit.h"
#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_free.h" 
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_sdpstruct.h"
#include "sdf_list.h" 
#endif
#include "sdf_init.h"
#include "sdf_debug.h"
#include "sdf_trace.h"


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitCodec
**
** DESCRIPTION: This function inits the Sdf_st_codec Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCodec
#ifdef ANSI_PROTO
(Sdf_st_codec **ppCodec,Sdf_st_error *pErr)
#else
(ppCodec, pErr)
Sdf_st_codec **ppCodec;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitCodec");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitCodec( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppCodec = (Sdf_st_codec *) sdf_memget (Sdf_mc_initMemId, \
		sizeof(Sdf_st_codec), pErr);

	if (*ppCodec == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitCodec( ):"
			"Failed to allocate memory for Sdf_st_codec structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppCodec)->dRefCount);
	sdf_mc_init((*ppCodec)->pFmtpString);
	sdf_mc_init((*ppCodec)->pOtherCodecName);
	(*ppCodec)->dCodecName = Sdf_en_otherCodec;
	(*ppCodec)->dCodecNum = -1;
	(*ppCodec)->dEncodingRate = 0;
	(*ppCodec)->dChannels = 0;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitCodec");	
	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaStreamGroup
**
** DESCRIPTION: This function inits the Media Stream Group Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaStreamGroup
#ifdef ANSI_PROTO
(Sdf_st_mediaStreamGroup **ppMediaStreamGroup,Sdf_st_error *pErr)
#else
(ppMediaStreamGroup, pErr)
Sdf_st_mediaStreamGroup **ppMediaStreamGroup;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitMediaStreamGroup");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitMediaStreamGroup( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppMediaStreamGroup = (Sdf_st_mediaStreamGroup *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_mediaStreamGroup), pErr);

	if (*ppMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStreamGroup( ):"
			"Failed to allocate memory for Sdf_st_mediaStreamGroup"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppMediaStreamGroup)->dRefCount);
	(*ppMediaStreamGroup)->dGroupType = Sdf_en_groupOther;
	sdf_mc_init((*ppMediaStreamGroup)->pOtherGroupType);
	(void)sdf_listInit((&(*ppMediaStreamGroup)->slMediaStreamIndex), \
		__sdf_fn_uaFreeU32bit, 	Sdf_co_false,pErr);
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitMedia");	
	return Sdf_co_success;
}
#endif

#ifdef SDF_QOS
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitQosInfo
**
** DESCRIPTION: This function inits the Qos Info Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitQosInfo
#ifdef ANSI_PROTO
(Sdf_st_qosInfo **ppQosInfo,Sdf_st_error *pErr)
#else
(ppQosInfo, pErr)
Sdf_st_qosInfo **ppQosInfo;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitQosInfo");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitQosInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppQosInfo = (Sdf_st_qosInfo *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_qosInfo), pErr);

	if (*ppQosInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitQosInfo( ):"
			"Failed to allocate memory for Sdf_st_qosInfo"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppQosInfo)->dRefCount);
	sdf_mc_init((*ppQosInfo)->pPrecondition);
	(*ppQosInfo)->dQosStrength = Sdf_en_qosStrengthNone;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitQosInfo");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSegmentedQosDetails
**
** DESCRIPTION: This function inits the Qos Info Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSegmentedQosDetails
#ifdef ANSI_PROTO
(Sdf_st_segmentedQosDetails **ppSegmentedQosDetails,Sdf_st_error *pErr)
#else
(ppSegmentedQosDetails, pErr)
Sdf_st_segmentedQosDetails **ppSegmentedQosDetails;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitSegmentedQosDetails");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSegmentedQosDetails( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppSegmentedQosDetails = (Sdf_st_segmentedQosDetails *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_segmentedQosDetails), pErr);

	if (*ppSegmentedQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSegmentedQosDetails( ):"
			"Failed to allocate memory for Sdf_st_segmentedQosDetails"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	sdf_mc_init((*ppSegmentedQosDetails)->dLocalSend.pPrecondition);
	sdf_mc_init((*ppSegmentedQosDetails)->dLocalRecv.pPrecondition);
	sdf_mc_init((*ppSegmentedQosDetails)->dRemoteSend.pPrecondition);
	sdf_mc_init((*ppSegmentedQosDetails)->dRemoteRecv.pPrecondition);

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppSegmentedQosDetails)->dRefCount);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitSegmentedQosDetails");	
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitE2eQosDetails
**
** DESCRIPTION: This function inits the Sdf_st_e2eQosDetails Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitE2eQosDetails
#ifdef ANSI_PROTO
(Sdf_st_e2eQosDetails **ppE2eQosDetails,Sdf_st_error *pErr)
#else
(ppE2eQosDetails, pErr)
Sdf_st_e2eQosDetails **ppE2eQosDetails;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitE2eQosDetails");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitE2eQosDetails( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppE2eQosDetails = (Sdf_st_e2eQosDetails *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_e2eQosDetails), pErr);

	if (*ppE2eQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitE2eQosDetails( ):"
			"Failed to allocate memory for Sdf_st_e2eQosDetails"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	sdf_mc_init((*ppE2eQosDetails)->dE2eSend.pPrecondition);
	sdf_mc_init((*ppE2eQosDetails)->dE2eRecv.pPrecondition);
	(*ppE2eQosDetails)->dE2eSend.dQosStrength = Sdf_en_qosStrengthNone;
	(*ppE2eQosDetails)->dE2eRecv.dQosStrength = Sdf_en_qosStrengthNone;	

	HSS_INITREF((*ppE2eQosDetails)->dRefCount);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitE2eQosDetails");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitQosAttrib
**
** DESCRIPTION: This function inits the Sdf_st_qosAttrib Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitQosAttrib
#ifdef ANSI_PROTO
(Sdf_st_qosAttrib **ppQosAttrib,Sdf_ty_qosType dQosType,Sdf_st_error *pErr)
#else
(ppQosAttrib, dQosType,pErr)
Sdf_st_qosAttrib **ppQosAttrib;
Sdf_ty_qosType dQosType;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitQosAttrib");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitQosAttrib( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppQosAttrib = (Sdf_st_qosAttrib *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_qosAttrib), pErr);

	if (*ppQosAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitQosAttrib( ):"
			"Failed to allocate memory for Sdf_st_qosAttrib"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppQosAttrib)->dRefCount);

	(*ppQosAttrib)->dQosType=dQosType;

	if(dQosType==Sdf_en_e2e)
		sdf_mc_init((*ppQosAttrib)->u.pE2eQos);
	else if(dQosType== Sdf_en_segmented)
		sdf_mc_init((*ppQosAttrib)->u.pSegmentedQos);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitQosAttrib");	
	return Sdf_co_success;
}
#endif

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionAttrib
**
** DESCRIPTION: This function inits the Sdf_st_sessionAttrib Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionAttrib
#ifdef ANSI_PROTO
(Sdf_st_sessionAttrib **ppSessionAttrib,Sdf_st_error *pErr)
#else
(ppSessionAttrib, pErr)
Sdf_st_sessionAttrib **ppSessionAttrib;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitSessionAttrib");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSessionAttrib( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppSessionAttrib = (Sdf_st_sessionAttrib *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_sessionAttrib), pErr);

	if (*ppSessionAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionAttrib( ):"
			"Failed to allocate memory for Sdf_st_sessionAttrib"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppSessionAttrib)->dRefCount);

	sdf_mc_init((*ppSessionAttrib)->pName);
	sdf_mc_init((*ppSessionAttrib)->pValue);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitSessionAttrib");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaStream
**
** DESCRIPTION: This function inits the Sdf_st_mediaStream Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaStream
#ifdef ANSI_PROTO
(Sdf_st_mediaStream **ppMediaStream,Sdf_st_error *pErr)
#else
(ppMediaStream, pErr)
Sdf_st_mediaStream **ppMediaStream;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_bool dRetVal=Sdf_co_true;
	Sdf_ty_bool dIncludeCodec=Sdf_co_false,dIncludeOtherAttrib=Sdf_co_false,\
				dIncludeConection=Sdf_co_false,dIncludePrevConection=\
				Sdf_co_false,dIncludeBandWidth=Sdf_co_false;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitMediaStream");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitMediaStream( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppMediaStream = (Sdf_st_mediaStream *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_mediaStream), pErr);

	if (*ppMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
			"Failed to allocate memory for Sdf_st_mediaStream"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppMediaStream)->dRefCount);

	sdf_mc_init((*ppMediaStream)->pMediaStreamId);
	(*ppMediaStream)->dMidPresence=Sdf_co_false;
	(*ppMediaStream)->dMediaType=Sdf_en_audio;
	(*ppMediaStream)->dTransport=Sdf_en_rtpAvp;
	(*ppMediaStream)->dPort=0;
	(*ppMediaStream)->dNumPort=0;
	(*ppMediaStream)->dPacketTime=0;
	sdf_mc_init((*ppMediaStream)->pOtherMediaType);
	sdf_mc_init((*ppMediaStream)->pOtherTransportType);
#ifdef SDF_QOS
	/* Todo Allocate memory for substrucure */
	sdf_mc_init((*ppMediaStream)->pCurrentQos);
	sdf_mc_init((*ppMediaStream)->pDesiredQos);
	sdf_mc_init((*ppMediaStream)->pConfirmedQos);
#endif

	(*ppMediaStream)->dDirectionAttrib=Sdf_en_dirAttribNone;
	(*ppMediaStream)->dPrevDirectionAttrib=Sdf_en_dirAttribNone;

	sdf_mc_init((*ppMediaStream)->pKey);
	sdf_mc_init((*ppMediaStream)->pTitle);

	(*ppMediaStream)->dMatchedAlready=Sdf_co_false;

	/* Initialise all the list member */
	if (sdf_listInit(&((*ppMediaStream)->slCodecs), __sdf_fn_uaFreeCodec, \
		Sdf_co_false, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
			"Failed to initalize list of Codecs",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		dRetVal = Sdf_co_false;
	}
	else
		dIncludeCodec=Sdf_co_true;
	
	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppMediaStream)->slOtherAttribs), \
			__sip_freeSdpAttr, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
				"Failed to initalize list of slOtherAttribs",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeOtherAttrib=Sdf_co_true;
	}
	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppMediaStream)->slConnection), \
			__sip_freeSdpConnection, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
				"Failed to initalize list of Connection",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeConection=Sdf_co_true;
	}

	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppMediaStream)->slPreviousConnection), \
			__sip_freeSdpConnection, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
				"Failed to initalize list of Previous Connection",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludePrevConection=Sdf_co_true;
	}

	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppMediaStream)->slBandwidth), \
			__sip_freeString, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaStream( ):"
				"Failed to initalize list of BandWidth",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeBandWidth=Sdf_co_true;
	}
	if(dRetVal==Sdf_co_false)
	{
		/*Todo Free the substrucures */

		if(dIncludeBandWidth==Sdf_co_true)
			(void)sdf_listFree(&((*ppMediaStream)->slBandwidth),pErr);
		
		if(dIncludePrevConection==Sdf_co_true)
			(void)sdf_listFree(&((*ppMediaStream)->slPreviousConnection),pErr);

		if(dIncludeConection==Sdf_co_true)
			(void)sdf_listFree(&((*ppMediaStream)->slConnection),pErr);

		if(dIncludeOtherAttrib==Sdf_co_true)
			(void)sdf_listFree(&((*ppMediaStream)->slOtherAttribs),pErr);

		if(dIncludeCodec==Sdf_co_true)
			(void)sdf_listFree(&((*ppMediaStream)->slCodecs),pErr);
		HSS_DELETEREF((*ppMediaStream)->dRefCount);

		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitMediaStream");	
	return Sdf_co_success;
}



/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionParam
**
** DESCRIPTION: This function inits the Sdf_st_sessionParam Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionParam
#ifdef ANSI_PROTO
(Sdf_st_sessionParam **ppSessionParam,Sdf_st_error *pErr)
#else
(ppSessionParam, pErr)
Sdf_st_sessionParam **ppSessionParam;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_bool dRetVal=Sdf_co_true;
	Sdf_ty_bool dIncludeOtherAttrib=Sdf_co_false,\
				dIncludeEmail=Sdf_co_false,dIncludePhone=Sdf_co_false,\
				dIncludeBandWidth=Sdf_co_false;

#ifdef SDF_UAB2B
/* slRepeatTime :Optional SPR-4218*/
 Sdf_ty_bool dIncludeRepeatTime = Sdf_co_false;
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitSessionParam");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSessionParam( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppSessionParam = (Sdf_st_sessionParam *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_sessionParam), pErr);

	if (*ppSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
			"Failed to allocate memory for Sdf_st_sessionParam"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppSessionParam)->dRefCount);

	(*ppSessionParam)->dVersion=0;

	/* Todo Allocate memory for substrucure */
	sdf_mc_init((*ppSessionParam)->pConnection);
	sdf_mc_init((*ppSessionParam)->pPreviousConnection);

	sdf_mc_init((*ppSessionParam)->pSessionUser);
	(*ppSessionParam)->dLocalSessionVersion=0;
	(*ppSessionParam)->dRemoteSessionVersion=0;
	(*ppSessionParam)->dLocalSessionId=0;
	(*ppSessionParam)->dRemoteSessionId=0;
	sdf_mc_init((*ppSessionParam)->pOriginIp);
	sdf_mc_init((*ppSessionParam)->pOriginAddrType);
	sdf_mc_init((*ppSessionParam)->pSessionName);
	(*ppSessionParam)->dDirectionAttrib=Sdf_en_dirAttribNone;
	(*ppSessionParam)->dPrevDirectionAttrib=Sdf_en_dirAttribNone;
	(*ppSessionParam)->dSessionStart=0;
	(*ppSessionParam)->dSessionEnd=0;
	sdf_mc_init((*ppSessionParam)->pSessionInfo);
	sdf_mc_init((*ppSessionParam)->pUri);
	sdf_mc_init((*ppSessionParam)->pTimeZoneInfo);
	sdf_mc_init((*ppSessionParam)->pEncryptionKey);
#ifdef SDF_MEDIAGROUP
	(*ppSessionParam)->pMediaStreamGroups=Sdf_co_null;
	(*ppSessionParam)->dNumGroups=0;
#endif
	(*ppSessionParam)->pMediaStreams=Sdf_co_null;
	(*ppSessionParam)->dNumMediaStreams=0;
#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */

  sdf_mc_init((*ppSessionParam)->pSdpMessage);
#endif
	/* Initialise all the list member */
		
	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppSessionParam)->slOtherSessionAttributes), \
			__sip_freeSdpAttr, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
				"Failed to initalize list of slOtherSessionAttributes",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeOtherAttrib=Sdf_co_true;
	}
	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppSessionParam)->slEmail), \
			__sip_freeString, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
				"Failed to initalize list of Email",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeEmail=Sdf_co_true;
	}

#ifdef SDF_UAB2B
/* slRepeatTime :Optional SPR-4218*/
  if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppSessionParam)->slRepeatTime), \
			__sip_freeString, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
				"Failed to initalize list of slRepeatTime",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeRepeatTime = Sdf_co_true;
	}

#endif /* End:slRepeatTime :Optional SPR-4218*/



    if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppSessionParam)->slPhone), \
			__sip_freeString, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
				"Failed to initalize list of Phone",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludePhone=Sdf_co_true;
	}

	if(dRetVal==Sdf_co_true)
	{
		if (sdf_listInit(&((*ppSessionParam)->slBandwidth), \
			__sip_freeString, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionParam( ):"
				"Failed to initalize list of BandWidth",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_false;
		}
		else
			dIncludeBandWidth=Sdf_co_true;
	}
	if(dRetVal==Sdf_co_false)
	{
		/*Todo Free the substrucures */


		if(dIncludeBandWidth==Sdf_co_true)
			(void)sdf_listFree(&((*ppSessionParam)->slBandwidth),pErr);
		
		if(dIncludeOtherAttrib==Sdf_co_true)
			(void)sdf_listFree(&((*ppSessionParam)->slOtherSessionAttributes),pErr);

		if(dIncludeEmail==Sdf_co_true)
			(void)sdf_listFree(&((*ppSessionParam)->slEmail),pErr);

#ifdef SDF_UAB2B
/* Free the substrucures : slRepeatTime list SPR-4218*/ 
    if(dIncludeRepeatTime==Sdf_co_true)
			(void)sdf_listFree(&((*ppSessionParam)->slRepeatTime),pErr);

#endif
	if(dIncludePhone==Sdf_co_true)
			(void)sdf_listFree(&((*ppSessionParam)->slPhone),pErr);

		HSS_DELETEREF((*ppSessionParam)->dRefCount);

		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitSessionParam");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaHandling
**
** DESCRIPTION: This function inits the Sdf_st_mediaHandling Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaHandling
#ifdef ANSI_PROTO
(Sdf_st_mediaHandling **ppMediaHandling,Sdf_st_error *pErr)
#else
(ppMediaHandling, pErr)
Sdf_st_mediaHandling **ppMediaHandling;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitMediaHandling");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitMediaHandling( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppMediaHandling = (Sdf_st_mediaHandling *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_mediaHandling), pErr);

	if (*ppMediaHandling == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitMediaHandling( ):"
			"Failed to allocate memory for Sdf_st_mediaHandling"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppMediaHandling)->dRefCount);

	(*ppMediaHandling)->dMediaState=Sdf_en_idleMedia;
	(*ppMediaHandling)->dMediaStateChange=Sdf_en_noMediaChange;
	(*ppMediaHandling)->dOfferProgress=Sdf_co_false;
	(*ppMediaHandling)->dProvRespWithSdp=Sdf_co_false;
	

	/* Todo Allocate memory for substrucure */
	sdf_mc_init((*ppMediaHandling)->pCapabilities);
	sdf_mc_init((*ppMediaHandling)->pLastOfferMade);
	sdf_mc_init((*ppMediaHandling)->pLastOfferRecd);
	sdf_mc_init((*ppMediaHandling)->pCurrentSession);
	sdf_mc_init((*ppMediaHandling)->pPeersSession);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitMediaHandling");	
	return Sdf_co_success;
}

#ifdef SDF_SESSIONCHANGE
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitMediaChange
**
** DESCRIPTION: This function inits the Sdf_st_mediaChange Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMediaChange
#ifdef ANSI_PROTO
(Sdf_st_mediaChange *pMediaChange,Sdf_st_error *pErr)
#else
(ppMediaChange, pErr)
Sdf_st_mediaChange *pMediaChange;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitMediaChange");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitMediaChange( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif


	(pMediaChange)->dMediaChangeType=Sdf_en_noChange;
	(pMediaChange)->dPortChange=Sdf_co_false;
	(pMediaChange)->dMediaConnectionChange=Sdf_co_false;
	(pMediaChange)->dDirectionChange=Sdf_co_false;
	(pMediaChange)->dCodecChange=Sdf_co_false;
	(pMediaChange)->dOtherAttributeChange=Sdf_co_false;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitMediaChange");	
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitSessionChange
**
** DESCRIPTION: This function inits the Sdf_st_sessionChange Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSessionChange
#ifdef ANSI_PROTO
(Sdf_st_sessionChange **ppSessionChange,Sdf_st_error *pErr)
#else
(ppSessionChange, pErr)
Sdf_st_sessionChange **ppSessionChange;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		" sdf_ivk_uaInitSessionChange");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSessionChange( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppSessionChange = (Sdf_st_sessionChange *)\
		sdf_memget (Sdf_mc_initMemId,sizeof(Sdf_st_sessionChange), pErr);

	if (*ppSessionChange == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionChange( ):"
			"Failed to allocate memory for Sdf_st_sessionChange"
			" structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppSessionChange)->dRefCount);

	(*ppSessionChange)->dSessionChange=Sdf_co_false;
	(*ppSessionChange)->dSessionConnectionChange=Sdf_co_false;
	sdf_mc_init((*ppSessionChange)->pMediaChange);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		" sdf_ivk_uaInitSessionChange");	
	return Sdf_co_success;
}
#endif

