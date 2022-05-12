/*****************************************************************************
 ** FUNCTION:
 **			This file has the freeing functions for all SDP structures
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_sdpfree.c
 **
 ** DESCRIPTION		: This file has the freeing functions for all SDP structures
 **
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 29/11/02	Pankaj Nath						Creation
 **
 *****************************************************************************
 **     COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf_free.h"
#include "sdf_sdpfree.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_sdpstruct.h"
#include "sdf_list.h"
#endif


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeCodec
**
** DESCRIPTION: This function frees the Sdf_st_codec  structure
**
**
**********************************************************/
int mysym=1;
Sdf_ty_retVal sdf_ivk_uaFreeCodec
#ifdef ANSI_PROTO
	(Sdf_st_codec *pCodec)
#else
	( pCodec )
	Sdf_st_codec *pCodec;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering sdf_ivk_uaFreeCodec");

	if(pCodec == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pCodec->dRefCount);
	HSS_DECREF(pCodec->dRefCount);
	
	if (HSS_CHECKREF(pCodec->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCodec->pFmtpString),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCodec->pOtherCodecName),(Sdf_st_error*)Sdf_co_null);

		pCodec->dCodecName=Sdf_en_otherCodec;
		pCodec->dEncodingRate=0;
		pCodec->dChannels=0;
		pCodec->dCodecNum=-1;

		HSS_UNLOCKREF(pCodec->dRefCount);
		HSS_DELETEREF(pCodec->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pCodec), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pCodec->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeCodec ");

	return Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaStreamGroup
**
** DESCRIPTION: This function frees the Sdf_st_mediaStreamGroup  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaStreamGroup
#ifdef ANSI_PROTO
	(Sdf_st_mediaStreamGroup *pMediaStreamGroup)
#else
	( pMediaStreamGroup )
	Sdf_st_mediaStreamGroup *pMediaStreamGroup;
#endif
{
	Sdf_st_error Error;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeMediaStreamGroup");

	if(pMediaStreamGroup == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pMediaStreamGroup->dRefCount);
	HSS_DECREF(pMediaStreamGroup->dRefCount);
	
	if (HSS_CHECKREF(pMediaStreamGroup->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStreamGroup->pOtherGroupType),( Sdf_st_error*)Sdf_co_null);
		pMediaStreamGroup->dGroupType=Sdf_en_groupOther;

		/*Free the List*/
		(void)sdf_listDeleteAll(&(pMediaStreamGroup->slMediaStreamIndex),&Error);

		HSS_UNLOCKREF(pMediaStreamGroup->dRefCount);
		HSS_DELETEREF(pMediaStreamGroup->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pMediaStreamGroup), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pMediaStreamGroup->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeMediaStreamGroup ");
	return Sdf_co_success;
}
#endif

#ifdef SDF_QOS
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeQosInfo
**
** DESCRIPTION: This function frees the Sdf_st_qosInfo  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeQosInfo
#ifdef ANSI_PROTO
	(Sdf_st_qosInfo *pQosInfo)
#else
	( pQosInfo )
	Sdf_st_qosInfo *pQosInfo;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeQosInfo");

	if(pQosInfo == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pQosInfo->dRefCount);
	HSS_DECREF(pQosInfo->dRefCount);
	
	if (HSS_CHECKREF(pQosInfo->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pQosInfo->pPrecondition),( Sdf_st_error*)Sdf_co_null);

		pQosInfo->dQosStrength=Sdf_en_qosStrengthNone;

		HSS_UNLOCKREF(pQosInfo->dRefCount);
		HSS_DELETEREF(pQosInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pQosInfo), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pQosInfo->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeQosInfo ");
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSegmentedQosDetails
**
** DESCRIPTION: This function frees the Sdf_st_segmentedQosDetails  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSegmentedQosDetails
#ifdef ANSI_PROTO
	(Sdf_st_segmentedQosDetails *pSegmentedQosDetails)
#else
	( pSegmentedQosDetails )
	Sdf_st_segmentedQosDetails *pSegmentedQosDetails;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeSegmentedQosDetails");

	if(pSegmentedQosDetails == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSegmentedQosDetails->dRefCount);
	HSS_DECREF(pSegmentedQosDetails->dRefCount);
	
	if (HSS_CHECKREF(pSegmentedQosDetails->dRefCount))
	{
		HSS_UNLOCKREF(pSegmentedQosDetails->dRefCount);
		HSS_DELETEREF(pSegmentedQosDetails->dRefCount);

		if(pSegmentedQosDetails->dLocalSend.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pSegmentedQosDetails->dLocalSend.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pSegmentedQosDetails->dLocalSend.pPrecondition = Sdf_co_null;
		}
		if(pSegmentedQosDetails->dRemoteSend.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pSegmentedQosDetails->dRemoteSend.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pSegmentedQosDetails->dRemoteSend.pPrecondition = Sdf_co_null;
		}
		if(pSegmentedQosDetails->dLocalRecv.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pSegmentedQosDetails->dLocalRecv.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pSegmentedQosDetails->dLocalRecv.pPrecondition = Sdf_co_null;
		}
		if(pSegmentedQosDetails->dRemoteRecv.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pSegmentedQosDetails->dRemoteRecv.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pSegmentedQosDetails->dRemoteRecv.pPrecondition = Sdf_co_null;
		}

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSegmentedQosDetails), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSegmentedQosDetails->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeSegmentedQosDetails ");
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeE2eQosDetails
**
** DESCRIPTION: This function frees the Sdf_st_e2eQosDetails  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeE2eQosDetails
#ifdef ANSI_PROTO
	(Sdf_st_e2eQosDetails *pE2eQosDetails)
#else
	( pE2eQosDetails )
	Sdf_st_e2eQosDetails *pE2eQosDetails;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeE2eQosDetails");

	if(pE2eQosDetails == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pE2eQosDetails->dRefCount);
	HSS_DECREF(pE2eQosDetails->dRefCount);
	
	if (HSS_CHECKREF(pE2eQosDetails->dRefCount))
	{
		HSS_UNLOCKREF(pE2eQosDetails->dRefCount);
		HSS_DELETEREF(pE2eQosDetails->dRefCount);

		if(pE2eQosDetails->dE2eSend.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pE2eQosDetails->dE2eSend.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pE2eQosDetails->dE2eSend.pPrecondition = Sdf_co_null;
		}
		if(pE2eQosDetails->dE2eRecv.pPrecondition != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pE2eQosDetails->dE2eRecv.pPrecondition), \
				(Sdf_st_error*)Sdf_co_null);
			pE2eQosDetails->dE2eRecv.pPrecondition = Sdf_co_null;
		}

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pE2eQosDetails), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pE2eQosDetails->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeE2eQosDetails ");
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeQosAttrib
**
** DESCRIPTION: This function frees the Sdf_st_qosAttrib  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeQosAttrib
#ifdef ANSI_PROTO
	(Sdf_st_qosAttrib *pQosAttrib)
#else
	( pQosAttrib )
	Sdf_st_qosAttrib *pQosAttrib;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeQosAttrib");

	if(pQosAttrib == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pQosAttrib->dRefCount);
	HSS_DECREF(pQosAttrib->dRefCount);
	
	if (HSS_CHECKREF(pQosAttrib->dRefCount))
	{
		if(pQosAttrib->dQosType==Sdf_en_e2e)
			(void)sdf_ivk_uaFreeE2eQosDetails(pQosAttrib->u.pE2eQos);
		else if(pQosAttrib->dQosType==Sdf_en_segmented)
			(void)sdf_ivk_uaFreeSegmentedQosDetails(pQosAttrib->u.pSegmentedQos);
			
		HSS_UNLOCKREF(pQosAttrib->dRefCount);
		HSS_DELETEREF(pQosAttrib->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pQosAttrib), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pQosAttrib->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeQosAttrib ");
	return Sdf_co_success;
}
#endif


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionAttrib
**
** DESCRIPTION: This function frees the Sdf_st_sessionAttrib  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionAttrib
#ifdef ANSI_PROTO
	(Sdf_st_sessionAttrib *pSessionAttrib)
#else
	( pSessionAttrib )
	Sdf_st_sessionAttrib *pSessionAttrib;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeSessionAttrib");

	if(pSessionAttrib == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSessionAttrib->dRefCount);
	HSS_DECREF(pSessionAttrib->dRefCount);
	
	if (HSS_CHECKREF(pSessionAttrib->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionAttrib->pName),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionAttrib->pValue),( Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pSessionAttrib->dRefCount);
		HSS_DELETEREF(pSessionAttrib->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSessionAttrib), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSessionAttrib->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeSessionAttrib ");
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaStream
**
** DESCRIPTION: This function frees the Sdf_st_mediaStream  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaStream
#ifdef ANSI_PROTO
	(Sdf_st_mediaStream *pMediaStream)
#else
	( pMediaStream )
	Sdf_st_mediaStream *pMediaStream;
#endif
{
	Sdf_st_error Error;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeMediaStream");

	if(pMediaStream == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pMediaStream->dRefCount);
	HSS_DECREF(pMediaStream->dRefCount);
	
	if (HSS_CHECKREF(pMediaStream->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStream->pMediaStreamId),( Sdf_st_error*)Sdf_co_null);

		pMediaStream->dMidPresence=Sdf_co_false;

		pMediaStream->dMediaType=Sdf_en_audio;
		pMediaStream->dTransport=Sdf_en_rtpAvp;
		pMediaStream->dPort=0;
		pMediaStream->dNumPort=0;
		pMediaStream->dPacketTime=0;
		
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStream->pOtherMediaType),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStream->pOtherTransportType),( Sdf_st_error*)Sdf_co_null);
#ifdef SDF_QOS
		(void)sdf_ivk_uaFreeQosAttrib(pMediaStream->pCurrentQos);
		(void)sdf_ivk_uaFreeQosAttrib(pMediaStream->pDesiredQos);
		(void)sdf_ivk_uaFreeQosAttrib(pMediaStream->pConfirmedQos);
#endif
		pMediaStream->dDirectionAttrib=Sdf_en_dirAttribNone;
		pMediaStream->dPrevDirectionAttrib=Sdf_en_dirAttribNone;

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStream->pKey),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMediaStream->pTitle),( Sdf_st_error*)Sdf_co_null);

		pMediaStream->dMatchedAlready=Sdf_co_false;

		/* Free the List member*/
		(void)sdf_listDeleteAll(&(pMediaStream->slCodecs),&Error);
		(void)sdf_listDeleteAll(&(pMediaStream->slOtherAttribs),&Error);
		(void)sdf_listDeleteAll(&(pMediaStream->slConnection),&Error);
		(void)sdf_listDeleteAll(&(pMediaStream->slPreviousConnection),&Error);
		(void)sdf_listDeleteAll(&(pMediaStream->slBandwidth),&Error);


		HSS_UNLOCKREF(pMediaStream->dRefCount);
		HSS_DELETEREF(pMediaStream->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pMediaStream), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pMediaStream->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeMediaStream ");
	return Sdf_co_success;
}



/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionParam
**
** DESCRIPTION: This function frees the Sdf_st_sessionParam  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionParam
#ifdef ANSI_PROTO
	(Sdf_st_sessionParam *pSessionParam)
#else
	( pSessionParam )
	Sdf_st_sessionParam *pSessionParam;
#endif
{
	Sdf_st_error Error;
	Sdf_ty_u32bit	i=0;
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeSessionParam");

	if(pSessionParam == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSessionParam->dRefCount);
	HSS_DECREF(pSessionParam->dRefCount);
	
	if (HSS_CHECKREF(pSessionParam->dRefCount))
	{
		pSessionParam->dVersion=0;

		sip_freeSdpConnection(pSessionParam->pConnection);
		sip_freeSdpConnection(pSessionParam->pPreviousConnection);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pSessionUser),( Sdf_st_error*)Sdf_co_null);

		pSessionParam->dLocalSessionVersion=0;
		pSessionParam->dRemoteSessionVersion=0;
		pSessionParam->dLocalSessionId=0;
		pSessionParam->dRemoteSessionId=0;
		
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pOriginIp),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pOriginAddrType),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pSessionName),( Sdf_st_error*)Sdf_co_null);


		pSessionParam->dDirectionAttrib=Sdf_en_dirAttribNone;
		pSessionParam->dSessionStart=0;
		pSessionParam->dSessionEnd=0;

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pSessionInfo),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pUri),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pTimeZoneInfo),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pSessionParam->pEncryptionKey),( Sdf_st_error*)Sdf_co_null);

#ifdef SDF_MEDIAGROUP
		if(pSessionParam->pMediaStreamGroups != Sdf_co_null)
		{
			for(i=0;i<pSessionParam->dNumGroups;i++)
			{
				(void)sdf_ivk_uaFreeMediaStreamGroup((pSessionParam->\
					pMediaStreamGroups[i]));
			}
		}
		pSessionParam->dNumGroups=0;
		(void)sdf_memfree(Sdf_mc_freeMemId,\
				(Sdf_ty_pvoid*)&(pSessionParam->pMediaStreamGroups), \
						(Sdf_st_error*)Sdf_co_null);
#endif

		if(pSessionParam->pMediaStreams != Sdf_co_null)
		{
			for(i=0;i<pSessionParam->dNumMediaStreams;i++)
			{
				(void)sdf_ivk_uaFreeMediaStream((pSessionParam->\
					pMediaStreams[i]));
			}
		}
		pSessionParam->dNumMediaStreams=0;
		/*Free the Array of Media Streams Pointers*/
		(void)sdf_memfree(Sdf_mc_freeMemId,\
				(Sdf_ty_pvoid*)&(pSessionParam->pMediaStreams), \
						(Sdf_st_error*)Sdf_co_null);
	#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */

		sip_freeSdpMessage(pSessionParam->pSdpMessage);
#endif
		/* Free the List member*/
		(void)sdf_listDeleteAll(&(pSessionParam->slOtherSessionAttributes),&Error);
		(void)sdf_listDeleteAll(&(pSessionParam->slEmail),&Error);
		(void)sdf_listDeleteAll(&(pSessionParam->slPhone),&Error);
		(void)sdf_listDeleteAll(&(pSessionParam->slBandwidth),&Error);
#ifdef SDF_UAB2B
/*Free the List member. slRepeatTime :Optional SPR-4218*/ 
  (void)sdf_listDeleteAll(&(pSessionParam->slRepeatTime),&Error);  
#endif
		HSS_UNLOCKREF(pSessionParam->dRefCount);
		HSS_DELETEREF(pSessionParam->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSessionParam), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSessionParam->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeSessionParam ");
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeMediaHandling
**
** DESCRIPTION: This function frees the Sdf_st_mediaHandling  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMediaHandling
#ifdef ANSI_PROTO
	(Sdf_st_mediaHandling *pMediaHandling)
#else
	( pMediaHandling )
	Sdf_st_mediaHandling *pMediaHandling;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeMediaHandling");

	if(pMediaHandling == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pMediaHandling->dRefCount);
	HSS_DECREF(pMediaHandling->dRefCount);
	
	if (HSS_CHECKREF(pMediaHandling->dRefCount))
	{
		pMediaHandling->dMediaState=Sdf_en_idleMedia;
		pMediaHandling->dOfferProgress	=Sdf_co_false;
		pMediaHandling->dProvRespWithSdp=Sdf_co_false;
		(void)sdf_ivk_uaFreeSessionParam(pMediaHandling->pCapabilities);
		(void)sdf_ivk_uaFreeSessionParam(pMediaHandling->pLastOfferMade);
		(void)sdf_ivk_uaFreeSessionParam(pMediaHandling->pLastOfferRecd);
		(void)sdf_ivk_uaFreeSessionParam(pMediaHandling->pCurrentSession);
		(void)sdf_ivk_uaFreeSessionParam(pMediaHandling->pPeersSession);

		HSS_UNLOCKREF(pMediaHandling->dRefCount);
		HSS_DELETEREF(pMediaHandling->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pMediaHandling), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pMediaHandling->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeMediaHandling ");
	return Sdf_co_success;
}

#ifdef SDF_SESSIONCHANGE
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeSessionChange
**
** DESCRIPTION: This function frees the Sdf_st_sessionChange  structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSessionChange
#ifdef ANSI_PROTO
	(Sdf_st_sessionChange *pSessionChange)
#else
	( pSessionChange )
	Sdf_st_sessionChange *pSessionChange;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "
		" sdf_ivk_uaFreeSessionChange");

	if(pSessionChange == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSessionChange->dRefCount);
	HSS_DECREF(pSessionChange->dRefCount);
	
	if (HSS_CHECKREF(pSessionChange->dRefCount))
	{
		pSessionChange->dSessionChange=Sdf_co_false;
		HSS_UNLOCKREF(pSessionChange->dRefCount);
		HSS_DELETEREF(pSessionChange->dRefCount);
		
		if(pSessionChange->pMediaChange != Sdf_co_null)
		{
			(void)sdf_memfree(Sdf_mc_freeMemId, \
				(Sdf_ty_pvoid*)&(pSessionChange->pMediaChange), \
				(Sdf_st_error*)Sdf_co_null);
		}
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSessionChange), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSessionChange->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "
		" sdf_ivk_uaFreeSessionChange ");
	return Sdf_co_success;
}
#endif

/***********************************************************************
** Function: __sdf_fn_uaFreeCodec
** Description: Frees the Sdf_st_codec structure
**
************************************************************************/
void __sdf_fn_uaFreeCodec 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pCodec)
#else
(pCodec)
Sdf_ty_pvoid pCodec;
#endif
{
	(void)sdf_ivk_uaFreeCodec((Sdf_st_codec *)pCodec);
}
/***********************************************************************
** Function: __sdf_fn_uaFreeMediaStream
** Description: Frees the Sdf_st_mediaStream structure
**
************************************************************************/
void __sdf_fn_uaFreeMediaStream 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pMediaStream)
#else
(pMediaStream)
Sdf_ty_pvoid pMediaStream;
#endif
{
	(void)sdf_ivk_uaFreeMediaStream((Sdf_st_mediaStream*)pMediaStream);
}

#ifdef SDF_MEDIAGROUP
/***********************************************************************
** Function: __sdf_fn_uaFreeMediaStreamGroup
** Description: Frees the Sdf_st_mediaStream structure
**
************************************************************************/
void __sdf_fn_uaFreeMediaStreamGroup 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pMediaStreamGroup)
#else
(pMediaStreamGroup)
Sdf_ty_pvoid pMediaStreamGroup;
#endif
{
	(void)sdf_ivk_uaFreeMediaStreamGroup((Sdf_st_mediaStreamGroup*)pMediaStreamGroup);
}
#endif
