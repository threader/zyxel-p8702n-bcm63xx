/******************************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various SDP structures in the UA toolkit
 ******************************************************************************
 **
 ** FILENAME		: sdf_sdpaccessor.c
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various SDP structures in the UA toolkit
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 29/11/02	Pankaj Nath						Creation
 ** 12-Jun-09   Anuradha Gupta  SPR 19737   klocworks warning removal
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2009
 ******************************************************************************/

#include "ALheader.h"
#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_sdpstruct.h"
#include "sdf_free.h"
#include "sdf_sdpfree.h"
#include "sdf_list.h"
#endif
#include "sdf_accessor.h"
#include "sdf_sdpaccessor.h"
#include "sdf_init.h"
#include "sdf_sdpinit.h"
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_sdp.h"



/******************************************************************
**
** FUNCTION:  __sdf_cloneSdfStringList
**
** DESCRIPTION:  This function makes a deep copy of a SipList of
** SIP_S8bit *  from the "source" to "dest".
**
******************************************************************/
Sdf_ty_retVal __sdf_cloneSdfStringList
#ifdef ANSI_PROTO
	(Sdf_ty_slist *dest, Sdf_ty_slist *source, Sdf_st_error *pErr)
#else
	(dest, source, pErr)
	Sdf_ty_slist *dest;
	Sdf_ty_slist *source;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_s8bit *temp_string, *clone_string;
	Sdf_ty_pvoid temp;
	Sdf_ty_u32bit count, dIndex;


	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering __sdf_cloneSdfStringList");

	if (sdf_listDeleteAll(dest, pErr) == Sdf_co_fail)
		return Sdf_co_fail;
	if ( sdf_listSizeOf (source, &count, pErr) == Sdf_co_fail )
			return Sdf_co_fail;

	for (dIndex = 0; dIndex < count; dIndex ++)
	{
		if (sdf_listGetAt(source, dIndex, &temp, pErr) == Sdf_co_fail)
			return Sdf_co_fail ;
		temp_string = (Sdf_ty_s8bit *)temp;
		if (temp_string == Sdf_co_null)
			clone_string = Sdf_co_null;
		else
		{
			clone_string = (Sdf_ty_s8bit *)Sdf_mc_strdupAccessor(temp_string);

		}/* end of else */

		if (sdf_listAppend(dest, (Sdf_ty_pvoid)clone_string,pErr )==Sdf_co_fail)
		{
			if(Sdf_co_null != clone_string)
			    sip_freeString(clone_string);
			return Sdf_co_fail;
		}
	} /* end of for */

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting __sdf_cloneSdfStringList");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneCodec
 **
 ** DESCRIPTION: This function copies the Sdf_st_codec structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneCodec
#ifdef ANSI_PROTO 
(Sdf_st_codec *pDestCodec,
Sdf_st_codec *pSrcCodec,
Sdf_st_error *pErr)
#else
(pDestCodec,pSrcCodec, pErr )
Sdf_st_codec *pDestCodec;
Sdf_st_codec *pSrcCodec;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneCodec");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneCodec(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestCodec == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCodec():  "
			"pDestCodec param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcCodec == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneCodec():  "
			"pSrcCodec Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestCodec->pFmtpString),pErr);

	if(pSrcCodec->pFmtpString == Sdf_co_null)
		pDestCodec->pFmtpString = Sdf_co_null;
	else
		pDestCodec->pFmtpString = Sdf_mc_strdupAccessor(pSrcCodec->pFmtpString);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestCodec->pOtherCodecName),pErr);

	if(pSrcCodec->pOtherCodecName == Sdf_co_null)
		pDestCodec->pOtherCodecName = Sdf_co_null;
	else
	{
		pDestCodec->pOtherCodecName = Sdf_mc_strdupAccessor(\
			pSrcCodec->pOtherCodecName);
	}

	pDestCodec->dCodecName=pSrcCodec->dCodecName;
	pDestCodec->dCodecNum=pSrcCodec->dCodecNum;
	pDestCodec->dEncodingRate=pSrcCodec->dEncodingRate;
	pDestCodec->dChannels=pSrcCodec->dChannels;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneCodec");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}

#ifdef SDF_MEDIAGROUP
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStreamGroup
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaStreamGroup 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStreamGroup
#ifdef ANSI_PROTO 
(Sdf_st_mediaStreamGroup *pDestMediaStreamGroup,
Sdf_st_mediaStreamGroup *pSrcMediaStreamGroup,
Sdf_st_error *pErr)
#else
(pDestMediaStreamGroup,pSrcMediaStreamGroup, pErr )
Sdf_st_mediaStreamGroup *pDestMediaStreamGroup;
Sdf_st_mediaStreamGroup *pSrcMediaStreamGroup;
 Sdf_st_error *pErr;
#endif
{
	Sdf_st_listIterator	dListIterator;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMediaStreamGroup");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneMediaStreamGroup(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
			"pDestMediaStreamGroup param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
			"pSrcMediaStreamGroup Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStreamGroup->pOtherGroupType),pErr);

	if(pSrcMediaStreamGroup->pOtherGroupType == Sdf_co_null)
		pDestMediaStreamGroup->pOtherGroupType = Sdf_co_null;
	else
		pDestMediaStreamGroup->pOtherGroupType = Sdf_mc_strdupAccessor(\
			pSrcMediaStreamGroup->pOtherGroupType);


	pDestMediaStreamGroup->dGroupType=pSrcMediaStreamGroup->dGroupType;
	
	/*Clone the List of Mid Indexes*/
		
	(void)sdf_listDeleteAll(&(pDestMediaStreamGroup->slMediaStreamIndex),pErr);
	
	(void)sdf_listInitIterator(&(pSrcMediaStreamGroup->slMediaStreamIndex), \
			&dListIterator,	pErr);

	while(dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_ty_u32bit	*pIndex= (Sdf_ty_u32bit*)sdf_memget(Sdf_mc_sdpMemId, \
								   sizeof(Sdf_ty_u32bit),pErr);
		
		*pIndex = *((Sdf_ty_u32bit*)dListIterator.pCurrentElement->pData);
		
		/*w534*/(void)sdf_listAppend(&(pDestMediaStreamGroup->slMediaStreamIndex),(Sdf_ty_pvoid)pIndex,\
						pErr);
		(void)sdf_listNext(&dListIterator,pErr);
	}
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneMediaStreamGroup");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStreamGroup
 **
 ** DESCRIPTION: This function copies the Array of Sdf_st_mediaStreamGroup 
 **				structures from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStreamGroupArray
#ifdef ANSI_PROTO 
(Sdf_st_mediaStreamGroup ***pppDestMediaStreamGroup,
Sdf_st_mediaStreamGroup **ppSrcMediaStreamGroup,
Sdf_ty_u8bit			dNumGroups,
Sdf_st_error *pErr)
#else
(pppDestMediaStreamGroup, ppSrcMediaStreamGroup, dNumGroups, pErr )
Sdf_st_mediaStreamGroup ***pppDestMediaStreamGroup;
Sdf_st_mediaStreamGroup **ppSrcMediaStreamGroup;
Sdf_ty_u8bit			dNumGroups;
Sdf_st_error *pErr;
#endif
{

	Sdf_ty_u8bit	dIndex = 0;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMediaStreamGroupArray");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneMediaStreamGroupArray(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pppDestMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
			"pDestMediaStreamGroup param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppSrcMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
			"pSrcMediaStreamGroup Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	*pppDestMediaStreamGroup = (Sdf_st_mediaStreamGroup	**)\
				sdf_memget(Sdf_mc_accessorMemId, \
				dNumGroups * sizeof(Sdf_st_mediaStreamGroup), pErr);

	if( *pppDestMediaStreamGroup == Sdf_co_null )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
			"Failed to Allocate memory for Media Stream Group ",pErr);
#endif
		pErr->errCode = Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	for(dIndex = 0; dIndex < dNumGroups; dIndex++)
	{
		if(sdf_ivk_uaInitMediaStreamGroup( &(*pppDestMediaStreamGroup[dIndex]), 
				pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
				"Failed to Init Media Stream Group at index",pErr);
#endif
			pErr->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		if(sdf_ivk_uaCloneMediaStreamGroup(*pppDestMediaStreamGroup[dIndex], \
				ppSrcMediaStreamGroup[dIndex],	pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStreamGroup():  "
				"Failed to Clone Media Stream Group at index",pErr);
#endif
			pErr->errCode=Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
	}
	
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneMediaStreamGroup");
	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}
#endif

#ifdef SDF_QOS
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneQosInfo
 **
 ** DESCRIPTION: This function copies the Sdf_st_qosInfo 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneQosInfo
#ifdef ANSI_PROTO 
(Sdf_st_qosInfo *pDestQosInfo,
Sdf_st_qosInfo *pSrcQosInfo,
Sdf_st_error *pErr)
#else
(pDestQosInfo,pSrcQosInfo, pErr )
Sdf_st_qosInfo *pDestQosInfo;
Sdf_st_qosInfo *pSrcQosInfo;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneQosInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneQosInfo(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestQosInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneQosInfo():  "
			"pDestQosInfo param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcQosInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneQosInfo():  "
			"pSrcQosInfo Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestQosInfo->pPrecondition),pErr);

	if(pSrcQosInfo->pPrecondition == Sdf_co_null)
		pDestQosInfo->pPrecondition = Sdf_co_null;
	else
		pDestQosInfo->pPrecondition = Sdf_mc_strdupAccessor(\
			pSrcQosInfo->pPrecondition);


	pDestQosInfo->dQosStrength=pSrcQosInfo->dQosStrength;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneQosInfo");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSegmentedQosDetails
 **
 ** DESCRIPTION: This function copies the Sdf_st_segmentedQosDetails 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSegmentedQosDetails
#ifdef ANSI_PROTO 
(Sdf_st_segmentedQosDetails *pDestSegmentedQosDetails,
Sdf_st_segmentedQosDetails *pSrcSegmentedQosDetails,
Sdf_st_error *pErr)
#else
(pDestSegmentedQosDetails,pSrcSegmentedQosDetails, pErr )
Sdf_st_segmentedQosDetails *pDestSegmentedQosDetails;
Sdf_st_segmentedQosDetails *pSrcSegmentedQosDetails;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSegmentedQosDetails");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneSegmentedQosDetails(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestSegmentedQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSegmentedQosDetails():  "
			"pDestSegmentedQosDetails param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcSegmentedQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSegmentedQosDetails():  "
			"pSrcSegmentedQosDetails Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/*w534*/(void)sdf_ivk_uaCloneQosInfo(&(pDestSegmentedQosDetails->dLocalSend),\
		&(pSrcSegmentedQosDetails->dLocalSend),pErr);
	/*w534*/(void)sdf_ivk_uaCloneQosInfo(&(pDestSegmentedQosDetails->dLocalRecv),\
		&(pSrcSegmentedQosDetails->dLocalRecv),pErr);
	/*w534*/(void)sdf_ivk_uaCloneQosInfo(&(pDestSegmentedQosDetails->dRemoteSend),\
		&(pSrcSegmentedQosDetails->dRemoteSend),pErr);
	/*w534*/(void)sdf_ivk_uaCloneQosInfo(&(pDestSegmentedQosDetails->dRemoteRecv),\
		&(pSrcSegmentedQosDetails->dRemoteRecv),pErr);


	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSegmentedQosDetails");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneE2eQosDetails
 **
 ** DESCRIPTION: This function copies the Sdf_st_e2eQosDetails 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneE2eQosDetails
#ifdef ANSI_PROTO 
(Sdf_st_e2eQosDetails *pDestE2eQosDetails,
Sdf_st_e2eQosDetails *pSrcE2eQosDetails,
Sdf_st_error *pErr)
#else
(pDestE2eQosDetails,pSrcE2eQosDetails, pErr )
Sdf_st_e2eQosDetails *pDestE2eQosDetails;
Sdf_st_e2eQosDetails *pSrcE2eQosDetails;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneE2eQosDetails");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneE2eQosDetails(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestE2eQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneE2eQosDetails():  "
			"pDestE2eQosDetails param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcE2eQosDetails == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneE2eQosDetails():  "
			"pSrcE2eQosDetails Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	if(sdf_ivk_uaCloneQosInfo(&(pDestE2eQosDetails->dE2eSend),\
		&(pSrcE2eQosDetails->dE2eSend),pErr) == Sdf_co_fail) 
			return Sdf_co_fail;
	if(sdf_ivk_uaCloneQosInfo(&(pDestE2eQosDetails->dE2eRecv),\
		&(pSrcE2eQosDetails->dE2eRecv),pErr) == Sdf_co_fail) 
			return Sdf_co_fail;


	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneE2eQosDetails");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneQosAttrib
 **
 ** DESCRIPTION: This function copies the Sdf_st_qosAttrib 
 **				structure from  src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneQosAttrib
#ifdef ANSI_PROTO 
(Sdf_st_qosAttrib *pDestQosAttrib,
Sdf_st_qosAttrib *pSrcQosAttrib,
Sdf_st_error *pErr)
#else
(pDestQosAttrib,pSrcQosAttrib, pErr )
Sdf_st_qosAttrib *pDestQosAttrib;
Sdf_st_qosAttrib *pSrcQosAttrib;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneQosAttrib");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneQosAttrib(): Error Param"
			" Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestQosAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneQosAttrib():  "
			"pDestQosAttrib param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcQosAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneQosAttrib():  "
			"pSrcQosAttrib Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */

	if(pSrcQosAttrib->dQosType==Sdf_en_e2e)
	{
		(void)sdf_ivk_uaFreeE2eQosDetails(pDestQosAttrib->u.pE2eQos);

	}
	else if	(pSrcQosAttrib->dQosType==Sdf_en_segmented)
	{
		(void)sdf_ivk_uaFreeSegmentedQosDetails(pDestQosAttrib->u.pSegmentedQos);

	}

	pDestQosAttrib->dQosType = pSrcQosAttrib->dQosType;
	
	if(pSrcQosAttrib->dQosType==Sdf_en_e2e)
	{
		if(pSrcQosAttrib->u.pE2eQos == Sdf_co_null)
			pDestQosAttrib->u.pE2eQos = Sdf_co_null;
		else
		{
			if(sdf_ivk_uaInitE2eQosDetails(&(pDestQosAttrib->u.pE2eQos), \
									pErr) == Sdf_co_fail) 
               return Sdf_co_fail;
			if(sdf_ivk_uaCloneE2eQosDetails(pDestQosAttrib->u.pE2eQos,\
				pSrcQosAttrib->u.pE2eQos,pErr) == Sdf_co_fail)
            {/*lintcodefix*/
               (void)sdf_ivk_uaFreeE2eQosDetails(pDestQosAttrib->u.pE2eQos);
               return Sdf_co_fail;
            }

		}

	}
	else if	(pSrcQosAttrib->dQosType==Sdf_en_segmented)
	{
		if(pSrcQosAttrib->u.pSegmentedQos == Sdf_co_null)
			pDestQosAttrib->u.pSegmentedQos = Sdf_co_null;
		else
		{
			if(sdf_ivk_uaInitSegmentedQosDetails(\
				&(pDestQosAttrib->u.pSegmentedQos), pErr) == Sdf_co_fail)
					return Sdf_co_fail;
			if(sdf_ivk_uaCloneSegmentedQosDetails(pDestQosAttrib->u.pSegmentedQos,\
				pSrcQosAttrib->u.pSegmentedQos,pErr) == Sdf_co_fail)
            {/*lintcodefix*/
              (void)sdf_ivk_uaFreeSegmentedQosDetails(pDestQosAttrib->u.pSegmentedQos);
               return Sdf_co_fail;
            }   
		}
	}


	
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneQosAttrib");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}
#endif
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionAttrib
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionAttrib structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionAttrib
#ifdef ANSI_PROTO 
(Sdf_st_sessionAttrib *pDestSessionAttrib,
Sdf_st_sessionAttrib *pSrcSessionAttrib,
Sdf_st_error *pErr)
#else
(pDestSessionAttrib,pSrcSessionAttrib, pErr )
Sdf_st_sessionAttrib *pDestSessionAttrib;
Sdf_st_sessionAttrib *pSrcSessionAttrib;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSessionAttrib");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneSessionAttrib(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestSessionAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionAttrib():  "
			"pDestSessionAttrib param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcSessionAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionAttrib():  "
			"pSrcSessionAttrib Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionAttrib->pName),pErr);

	if(pSrcSessionAttrib->pName == Sdf_co_null)
		pDestSessionAttrib->pName = Sdf_co_null;
	else
		pDestSessionAttrib->pName = Sdf_mc_strdupAccessor(\
			pSrcSessionAttrib->pName);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionAttrib->pValue),pErr);

	if(pSrcSessionAttrib->pValue == Sdf_co_null)
		pDestSessionAttrib->pValue = Sdf_co_null;
	else
		pDestSessionAttrib->pValue = Sdf_mc_strdupAccessor(\
			pSrcSessionAttrib->pValue);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSessionAttrib");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaStream
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaStream structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaStream
#ifdef ANSI_PROTO 
(Sdf_st_mediaStream *pDestMediaStream,
Sdf_st_mediaStream *pSrcMediaStream,
Sdf_st_error *pErr)
#else
(pDestMediaStream,pSrcMediaStream, pErr )
Sdf_st_mediaStream *pDestMediaStream;
Sdf_st_mediaStream *pSrcMediaStream;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit i=0,dIndex=0,count=0,size=0;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMediaStream");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneMediaStream(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStream():  "
			"pDestMediaStream param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSrcMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStream():  "
			"pSrcMediaStream Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStream->pMediaStreamId),pErr);

	if(pSrcMediaStream->pMediaStreamId == Sdf_co_null)
		pDestMediaStream->pMediaStreamId = Sdf_co_null;
	else
		pDestMediaStream->pMediaStreamId = Sdf_mc_strdupAccessor(\
			pSrcMediaStream->pMediaStreamId);

	pDestMediaStream->dMidPresence = pSrcMediaStream->dMidPresence;
	pDestMediaStream->dMediaType = pSrcMediaStream->dMediaType;
	pDestMediaStream->dTransport = pSrcMediaStream->dTransport;
	pDestMediaStream->dPort = pSrcMediaStream->dPort;
	pDestMediaStream->dNumPort = pSrcMediaStream->dNumPort;
	pDestMediaStream->dPacketTime = pSrcMediaStream->dPacketTime;
	

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStream->pOtherMediaType),pErr);

	if(pSrcMediaStream->pOtherMediaType == Sdf_co_null)
		pDestMediaStream->pOtherMediaType = Sdf_co_null;
	else
		pDestMediaStream->pOtherMediaType = Sdf_mc_strdupAccessor(\
			pSrcMediaStream->pOtherMediaType);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStream->pOtherTransportType),pErr);

	if(pSrcMediaStream->pOtherTransportType == Sdf_co_null)
		pDestMediaStream->pOtherTransportType = Sdf_co_null;
	else
		pDestMediaStream->pOtherTransportType = Sdf_mc_strdupAccessor(\
			pSrcMediaStream->pOtherTransportType);

#ifdef SDF_QOS
	(void)sdf_ivk_uaFreeQosAttrib(pDestMediaStream->pCurrentQos);
	if(pSrcMediaStream->pCurrentQos == Sdf_co_null)
		pDestMediaStream->pCurrentQos=Sdf_co_null;
	else
	{

		/*w534*/(void)sdf_ivk_uaInitQosAttrib(&(pDestMediaStream->pCurrentQos),\
			pSrcMediaStream->pCurrentQos->dQosType,pErr);
		/*w534*/(void)sdf_ivk_uaCloneQosAttrib(pDestMediaStream->pCurrentQos,\
			pSrcMediaStream->pCurrentQos,pErr);

	}

	(void)sdf_ivk_uaFreeQosAttrib(pDestMediaStream->pDesiredQos);
	if(pSrcMediaStream->pDesiredQos == Sdf_co_null)
		pDestMediaStream->pDesiredQos=Sdf_co_null;
	else
	{

		/*w534*/(void)sdf_ivk_uaInitQosAttrib(&(pDestMediaStream->pDesiredQos),\
			pSrcMediaStream->pDesiredQos->dQosType,pErr);
		/*w534*/(void)sdf_ivk_uaCloneQosAttrib(pDestMediaStream->pDesiredQos,\
			pSrcMediaStream->pDesiredQos,pErr);

	}

	(void)sdf_ivk_uaFreeQosAttrib(pDestMediaStream->pConfirmedQos);
	if(pSrcMediaStream->pConfirmedQos == Sdf_co_null)
		pDestMediaStream->pConfirmedQos=Sdf_co_null;
	else
	{

		/*w534*/(void)sdf_ivk_uaInitQosAttrib(&(pDestMediaStream->pConfirmedQos),\
			pSrcMediaStream->pConfirmedQos->dQosType,pErr);
		/*w534*/(void)sdf_ivk_uaCloneQosAttrib(pDestMediaStream->pConfirmedQos,\
			pSrcMediaStream->pConfirmedQos,pErr);

	}
#endif
	pDestMediaStream->dDirectionAttrib=pSrcMediaStream->dDirectionAttrib;
	pDestMediaStream->dPrevDirectionAttrib=pSrcMediaStream->\
		dPrevDirectionAttrib;
	

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStream->pKey),pErr);

	if(pSrcMediaStream->pKey == Sdf_co_null)
		pDestMediaStream->pKey = Sdf_co_null;
	else
		pDestMediaStream->pKey = Sdf_mc_strdupAccessor(\
			pSrcMediaStream->pKey);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestMediaStream->pTitle),pErr);

	if(pSrcMediaStream->pTitle == Sdf_co_null)
		pDestMediaStream->pTitle = Sdf_co_null;
	else
		pDestMediaStream->pTitle = Sdf_mc_strdupAccessor(\
			pSrcMediaStream->pTitle);

	pDestMediaStream->dMatchedAlready=pSrcMediaStream->dMatchedAlready;


	/* Clone the list member */

	/*
	 * Delete contents of the destination Codecs list
	 * before adding the new cloned codec to it
	 */
	(void)sdf_listDeleteAll(&(pDestMediaStream->slCodecs),pErr);
	(void)sdf_listSizeOf(&(pSrcMediaStream->slCodecs), &count, pErr);
	for ( i = 0; i < count ; i++ )
	{
		Sdf_st_codec *attrout, *clone_attrout;

		/*w534*/(void)sdf_listGetAt(&(pSrcMediaStream->slCodecs),i,\
			(Sdf_ty_pvoid *)(&attrout), pErr); 
		if (attrout == Sdf_co_null)
			clone_attrout = Sdf_co_null;
		else
		{
			/*w534*/(void)sdf_ivk_uaInitCodec(&clone_attrout, pErr); 
			/*w534*/(void)sdf_ivk_uaCloneCodec(clone_attrout, attrout, pErr);
		}
		/*w534*/(void)sdf_listAppend (&(pDestMediaStream->slCodecs),\
			(Sdf_ty_pvoid)clone_attrout, pErr);
	}

	/*
	 * Delete contents of the destination OtherAttrib list
	 * before adding the new cloned Atrrib structure to it
	 */

	(void)sdf_listDeleteAll(&(pDestMediaStream->slOtherAttribs),pErr);
	(void)sdf_listSizeOf(&(pSrcMediaStream->slOtherAttribs), &count, pErr);
	for ( i = 0; i < count ; i++ )
	{
		SdpAttr *attrout, *clone_attrout;

		/*w534*/(void)sdf_listGetAt(&(pSrcMediaStream->slOtherAttribs),i,\
			(Sdf_ty_pvoid *)(&attrout), pErr); 
		if (attrout == Sdf_co_null)
			clone_attrout = Sdf_co_null;
		else
		{
			/*w534*/(void)sip_initSdpAttr(&clone_attrout, \
					(SipError *)&(pErr->stkErrCode));
		    if(Sdf_co_null == clone_attrout)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStream():  "
						"clone_attrout Param is invalid ",pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
			}
			
			/*w534*/(void)__sip_cloneSdpAttr (clone_attrout, attrout, \
				(SipError *)&(pErr->stkErrCode));
		}
		/*w534*/(void)sdf_listAppend (&(pDestMediaStream->slOtherAttribs),\
			(Sdf_ty_pvoid)clone_attrout, pErr);
	}

	/*
	 * Delete contents of the destination Connection list
	 * before adding the new cloned Connection list to it
	 */

	(void)sdf_listDeleteAll(&(pDestMediaStream->slConnection),pErr);
	(void)sdf_listSizeOf(&(pSrcMediaStream->slConnection), &size, pErr); 
	for ( dIndex = 0; dIndex < size ; dIndex++ )
	{
		SdpConnection *element_from_src, *clone_connection;
		/*w534*/(void)sdf_listGetAt(&(pSrcMediaStream->slConnection),dIndex,\
			(Sdf_ty_pvoid * ) (&element_from_src), pErr);
		if (element_from_src == Sdf_co_null)
			clone_connection = Sdf_co_null;
		else
		{
			/*w534*/(void)sip_initSdpConnection(&clone_connection, \
					(SipError *)&(pErr->stkErrCode));
			if(Sdf_co_null == clone_connection)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStream():  "
						"clone_connection Param is invalid ",pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
			}

			/*w534*/(void)__sip_cloneSdpConnection (clone_connection, element_from_src, \
					(SipError *)&(pErr->stkErrCode)); 
		}
		/*w534*/(void)sdf_listAppend (&(pDestMediaStream->slConnection),\
			(Sdf_ty_pvoid)clone_connection, pErr);
	}


	/*
	 * Delete contents of the destination Connection list
	 * before adding the new cloned Connection list to it
	 */

	(void)sdf_listDeleteAll(&(pDestMediaStream->slPreviousConnection),pErr);

	(void)sdf_listSizeOf(&(pSrcMediaStream->slPreviousConnection), &size, pErr); 
	for ( dIndex = 0; dIndex < size ; dIndex++ )
	{
		SdpConnection *element_from_src, *clone_connection;
		/*w534*/(void)sdf_listGetAt(&(pSrcMediaStream->slPreviousConnection),dIndex,\
			(Sdf_ty_pvoid * ) (&element_from_src), pErr);
		if (element_from_src == Sdf_co_null)
			clone_connection = Sdf_co_null;
		else
		{
			/*w534*/(void)sip_initSdpConnection(&clone_connection, \
					(SipError *)&(pErr->stkErrCode));

			if(Sdf_co_null == clone_connection)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
						(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaStream():  "
						"clone_connection Param is invalid ",pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				return Sdf_co_fail;
			}

			/*w534*/(void)__sip_cloneSdpConnection (clone_connection, element_from_src, \
					(SipError *)&(pErr->stkErrCode)); 
		}
		/*w534*/(void)sdf_listAppend (&(pDestMediaStream->slPreviousConnection),\
			(Sdf_ty_pvoid)clone_connection, pErr);
	}


	/* Cloning Bandwidth */
	(void)sdf_listDeleteAll(&(pDestMediaStream->slBandwidth),pErr);
	/*w534*/(void)__sdf_cloneSdfStringList(&(pDestMediaStream->slBandwidth),\
		&(pSrcMediaStream->slBandwidth),pErr);


	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneMediaStream");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionParam
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionParam structure from 
 **				 src to dest.
 **              If any failure happens in the cloning procedure, only local
 **              allocations are freed.
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionParam
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam *pDestSessionParam,
Sdf_st_sessionParam *pSrcSessionParam,
Sdf_st_error *pErr)
#else
(pDestSessionParam,pSrcSessionParam, pErr )
Sdf_st_sessionParam *pDestSessionParam;
Sdf_st_sessionParam *pSrcSessionParam;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit i=0,count=0;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSessionParam");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneSessionParam(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionParam():  "
			"pDestSessionParam param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSrcSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionParam():  "
			"pSrcSessionParam Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestSessionParam->dVersion = pSrcSessionParam->dVersion;

	/* Free the dest connection befor clone to it */
	if(pDestSessionParam->pConnection !=Sdf_co_null)
		sip_freeSdpConnection(pDestSessionParam->pConnection);
	if(pSrcSessionParam->pConnection==Sdf_co_null)
	{
		pDestSessionParam->pConnection = Sdf_co_null;
	}
	else
	{
		/*w534*/if(sip_initSdpConnection(&(pDestSessionParam->pConnection),\
		(SipError *)&(pErr->stkErrCode)) == SipFail)
                 return Sdf_co_fail;
		/*w534*/if(__sip_cloneSdpConnection(pDestSessionParam->pConnection,\
			pSrcSessionParam->pConnection,(SipError *)&(pErr->stkErrCode)) == SipFail)
                 return Sdf_co_fail;
	}
	
	/* Free the dest prev connection befor clone to it */
	if(pDestSessionParam->pPreviousConnection !=Sdf_co_null)
		sip_freeSdpConnection(pDestSessionParam->pPreviousConnection);
	if(pSrcSessionParam->pPreviousConnection==Sdf_co_null)
	{
		pDestSessionParam->pPreviousConnection = Sdf_co_null;
	}
	else
	{
		/*w534*/if(sip_initSdpConnection(&(pDestSessionParam->pPreviousConnection),\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
               return Sdf_co_fail;
		/*w534*/if(__sip_cloneSdpConnection(pDestSessionParam->pPreviousConnection,\
			pSrcSessionParam->pPreviousConnection,\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
               return Sdf_co_fail;
	}

	/* 
	 * Copy elements from the source to the destination. Before copying free 
	 * any existing members in the destination and overwrite with the members
	 * from the source
	 */
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionUser),pErr);

	if(pSrcSessionParam->pSessionUser == Sdf_co_null)
		pDestSessionParam->pSessionUser = Sdf_co_null;
	else
		pDestSessionParam->pSessionUser = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionUser);

	pDestSessionParam->dLocalSessionVersion = pSrcSessionParam->\
		dLocalSessionVersion;
	pDestSessionParam->dRemoteSessionVersion = pSrcSessionParam->\
		dRemoteSessionVersion;
	pDestSessionParam->dLocalSessionId = pSrcSessionParam->dLocalSessionId;
	pDestSessionParam->dRemoteSessionId = pSrcSessionParam->dRemoteSessionId;

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pOriginIp),pErr);
	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pOriginAddrType),pErr);

	if(pSrcSessionParam->pOriginIp == Sdf_co_null)
		pDestSessionParam->pOriginIp = Sdf_co_null;
	else
		pDestSessionParam->pOriginIp = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pOriginIp);

	if(pSrcSessionParam->pOriginAddrType== Sdf_co_null)
		pDestSessionParam->pOriginAddrType= Sdf_co_null;
	else
		pDestSessionParam->pOriginAddrType= Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pOriginAddrType);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionName),pErr);

	if(pSrcSessionParam->pSessionName == Sdf_co_null)
		pDestSessionParam->pSessionName = Sdf_co_null;
	else
		pDestSessionParam->pSessionName = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionName);


	pDestSessionParam->dDirectionAttrib = pSrcSessionParam->dDirectionAttrib;
	pDestSessionParam->dPrevDirectionAttrib = pSrcSessionParam->dPrevDirectionAttrib;
	pDestSessionParam->dSessionStart = pSrcSessionParam->dSessionStart;
	pDestSessionParam->dSessionEnd = pSrcSessionParam->dSessionEnd;

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pSessionInfo),pErr);
	if(pSrcSessionParam->pSessionInfo == Sdf_co_null)
		pDestSessionParam->pSessionInfo = Sdf_co_null;
	else
		pDestSessionParam->pSessionInfo = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pSessionInfo);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pUri),pErr);
	if(pSrcSessionParam->pUri == Sdf_co_null)
		pDestSessionParam->pUri = Sdf_co_null;
	else
		pDestSessionParam->pUri = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pUri);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pTimeZoneInfo),pErr);
	if(pSrcSessionParam->pTimeZoneInfo == Sdf_co_null)
		pDestSessionParam->pTimeZoneInfo = Sdf_co_null;
	else
		pDestSessionParam->pTimeZoneInfo = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pTimeZoneInfo);

	(void)sdf_memfree(Sdf_mc_callHandlerMemId, (Sdf_ty_pvoid*)\
		&(pDestSessionParam->pEncryptionKey),pErr);
	if(pSrcSessionParam->pEncryptionKey == Sdf_co_null)
		pDestSessionParam->pEncryptionKey = Sdf_co_null;
	else
		pDestSessionParam->pEncryptionKey = Sdf_mc_strdupAccessor(\
			pSrcSessionParam->pEncryptionKey);

#ifdef SDF_MEDIAGROUP
	/* Free the previous contains of MediaStreamGroup */
	if(	pDestSessionParam->pMediaStreamGroups !=Sdf_co_null)
	{
		for(i=0;i<pDestSessionParam->dNumGroups;i++)
			(void)sdf_ivk_uaFreeMediaStreamGroup(\
				pDestSessionParam->pMediaStreamGroups[i]);
	}
	if(pSrcSessionParam->pMediaStreamGroups == Sdf_co_null)
		pDestSessionParam->pMediaStreamGroups = Sdf_co_null;
	else
	{
		pDestSessionParam->pMediaStreamGroups=(Sdf_st_mediaStreamGroup **)\
		sdf_memget(Sdf_mc_accessorMemId,pSrcSessionParam->dNumGroups * \
			sizeof(Sdf_st_mediaStreamGroup), pErr);
		for(i=0;i<pSrcSessionParam->dNumGroups;i++)
		{
			/*w534*/if(sdf_ivk_uaInitMediaStreamGroup(&(pDestSessionParam->\
				pMediaStreamGroups[i]),pErr) == Sdf_co_fail)
                    return Sdf_co_fail;
			/*w534*/if(sdf_ivk_uaCloneMediaStreamGroup(pDestSessionParam->\
				pMediaStreamGroups[i],pSrcSessionParam->pMediaStreamGroups[i],\
				pErr) == Sdf_co_fail)
                    return Sdf_co_fail;
		}

	}
	pDestSessionParam->dNumGroups = pSrcSessionParam->dNumGroups;
#endif

	/* Free the previous contains of MediaStreams */
	if(	pDestSessionParam->pMediaStreams !=Sdf_co_null)
	{
		for(i=0;i<pDestSessionParam->dNumMediaStreams;i++)
			(void)sdf_ivk_uaFreeMediaStream(\
				pDestSessionParam->pMediaStreams[i]);
	}
	if(pSrcSessionParam->pMediaStreams == Sdf_co_null)
		pDestSessionParam->pMediaStreams = Sdf_co_null;
	else
	{
		pDestSessionParam->pMediaStreams=(Sdf_st_mediaStream **)\
		sdf_memget(Sdf_mc_accessorMemId,pSrcSessionParam->dNumMediaStreams * \
			sizeof(Sdf_st_mediaStream), pErr);

		if(Sdf_co_null == pDestSessionParam->pMediaStreams)
		{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionParam():  "
			"pDestSessionParam->pMediaStreams memeory allocation failure ",pErr);
#endif
			pErr->stkErrCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
		
		for(i=0;i<pSrcSessionParam->dNumMediaStreams;i++)
		{
			/*w534*/if(sdf_ivk_uaInitMediaStream(&(pDestSessionParam->pMediaStreams[i]),\
					pErr) == Sdf_co_fail)
				        return Sdf_co_fail;
			/*w534*/if(sdf_ivk_uaCloneMediaStream(pDestSessionParam->pMediaStreams[i], \
					pSrcSessionParam->pMediaStreams[i],	pErr) == Sdf_co_fail)
				        return Sdf_co_fail;

		}

	}

	pDestSessionParam->dNumMediaStreams = pSrcSessionParam->dNumMediaStreams;
#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */

	/* Free the dest SdpMessage befor clone to it */
	if(pDestSessionParam->pSdpMessage !=Sdf_co_null)
		sip_freeSdpMessage(pDestSessionParam->pSdpMessage);
	if(pSrcSessionParam->pSdpMessage==Sdf_co_null)
	{
		pDestSessionParam->pSdpMessage = Sdf_co_null;
	}
	else
	{
		/*w534*/if(sip_initSdpMessage(&(pDestSessionParam->pSdpMessage),\
			(SipError *)&(pErr->stkErrCode)) == SipFail)
				return Sdf_co_fail;
		/*w534*/if(__sip_cloneSdpMessage(pDestSessionParam->pSdpMessage,\
			pSrcSessionParam->pSdpMessage,(SipError *)&(pErr->stkErrCode)) == SipFail)
				return Sdf_co_fail;
	}
#endif

	/* Clone the list member */

	/*
	 * Delete contents of the destination OtherAttrib list
	 * before adding the new cloned Atrrib structure to it
	 */

	(void)sdf_listDeleteAll(&(pDestSessionParam->slOtherSessionAttributes),pErr);
	(void)sdf_listSizeOf(&(pSrcSessionParam->slOtherSessionAttributes), &count, pErr);
	for ( i = 0; i < count ; i++ )
	{
		SdpAttr *attrout, *clone_attrout;

		(void)sdf_listGetAt(&(pSrcSessionParam->slOtherSessionAttributes),i,\
			(Sdf_ty_pvoid *)(&attrout), pErr); 
		if (attrout == Sdf_co_null)
			clone_attrout = Sdf_co_null;
		else
		{
			/*w534*/if(sip_initSdpAttr(&clone_attrout, (SipError
											*)&(pErr->stkErrCode)) == SipFail)
						return Sdf_co_fail;
			/*w534*/if(__sip_cloneSdpAttr (clone_attrout, attrout,\
				(SipError *)&(pErr->stkErrCode)) == SipFail)
                 {
                    sip_freeSdpAttr(clone_attrout);
                    return Sdf_co_fail;
                 }
		}
		/*w534*/if(sdf_listAppend (&(pDestSessionParam->slOtherSessionAttributes),\
			(Sdf_ty_pvoid)clone_attrout, pErr) == Sdf_co_fail)
                {
                   sip_freeSdpAttr(clone_attrout);
                   return Sdf_co_fail;
                }
	}

	/* Copy slEmail list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slEmail),pErr);
	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slEmail),\
		&(pSrcSessionParam->slEmail), pErr) == Sdf_co_fail)
               return Sdf_co_fail;

#ifdef SDF_UAB2B
/* copy slRepeatTime list SPR-4218*/  
	(void)sdf_listDeleteAll(&(pDestSessionParam->slRepeatTime),pErr);
	/*w534*/if(__sdf_cloneSdfStringList(&(pDestSessionParam->slRepeatTime),\
		&(pSrcSessionParam->slRepeatTime), pErr) == Sdf_co_fail)
               return Sdf_co_fail;
#endif

	/* Copy slPhone list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slPhone),pErr);
	/*w534*/(void)__sdf_cloneSdfStringList(&(pDestSessionParam->slPhone),\
		&(pSrcSessionParam->slPhone), pErr);

	/* Copy BandWidth list */
	(void)sdf_listDeleteAll(&(pDestSessionParam->slBandwidth),pErr);
	/*w534*/(void)__sdf_cloneSdfStringList(&(pDestSessionParam->slBandwidth),\
		&(pSrcSessionParam->slBandwidth), pErr);

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSessionParam");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaHandling
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaHandling structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaHandling
#ifdef ANSI_PROTO 
(Sdf_st_mediaHandling *pDestMediaHandling,
Sdf_st_mediaHandling *pSrcMediaHandling,
Sdf_st_error *pErr)
#else
(pDestMediaHandling,pSrcMediaHandling, pErr )
Sdf_st_mediaHandling *pDestMediaHandling;
Sdf_st_mediaHandling *pSrcMediaHandling;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMediaHandling");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneMediaHandling(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestMediaHandling == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaHandling():  "
			"pDestMediaHandling param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSrcMediaHandling == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaHandling():  "
			"pSrcMediaHandling Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestMediaHandling->dMediaState = pSrcMediaHandling->dMediaState;
	pDestMediaHandling->dOfferProgress = pSrcMediaHandling->dOfferProgress;
	pDestMediaHandling->dProvRespWithSdp = pSrcMediaHandling->dProvRespWithSdp;

	/* Free the dest capabilities before clone to it */
	if(pDestMediaHandling->pCapabilities !=Sdf_co_null)
		(void)sdf_ivk_uaFreeSessionParam(pDestMediaHandling->pCapabilities);
	if(pSrcMediaHandling->pCapabilities==Sdf_co_null)
	{
		pDestMediaHandling->pCapabilities = Sdf_co_null;
	}
	else
	{
		/*w534*/(void)sdf_ivk_uaInitSessionParam(&(pDestMediaHandling->pCapabilities),pErr);
		/*w534*/(void)sdf_ivk_uaCloneSessionParam(pDestMediaHandling->pCapabilities,\
			pSrcMediaHandling->pCapabilities,pErr);
	}

	/* Free the dest last offer that was sent before clone to it */
	if(pDestMediaHandling->pLastOfferMade !=Sdf_co_null)
		(void)sdf_ivk_uaFreeSessionParam(pDestMediaHandling->pLastOfferMade);
	if(pSrcMediaHandling->pLastOfferMade==Sdf_co_null)
	{
		pDestMediaHandling->pLastOfferMade = Sdf_co_null;
	}
	else
	{
		/*w534*/(void)sdf_ivk_uaInitSessionParam(&(pDestMediaHandling->pLastOfferMade),pErr);
		/*w534*/(void)sdf_ivk_uaCloneSessionParam(pDestMediaHandling->pLastOfferMade,\
			pSrcMediaHandling->pLastOfferMade,pErr);
	}

	/* Free the dest present session before clone to it */
	if(pDestMediaHandling->pCurrentSession !=Sdf_co_null)
		(void)sdf_ivk_uaFreeSessionParam(pDestMediaHandling->pCurrentSession);
	if(pSrcMediaHandling->pCurrentSession==Sdf_co_null)
	{
		pDestMediaHandling->pCurrentSession = Sdf_co_null;
	}
	else
	{
		/*w534*/(void)sdf_ivk_uaInitSessionParam(&(pDestMediaHandling->pCurrentSession),pErr);
		/*w534*/(void)sdf_ivk_uaCloneSessionParam(pDestMediaHandling->pCurrentSession,\
			pSrcMediaHandling->pCurrentSession,pErr);
	}

	/* Free the dest PeerSession before clone to it */
	if(pDestMediaHandling->pPeersSession !=Sdf_co_null)
		(void)sdf_ivk_uaFreeSessionParam(pDestMediaHandling->pPeersSession);
	if(pSrcMediaHandling->pPeersSession==Sdf_co_null)
	{
		pDestMediaHandling->pPeersSession = Sdf_co_null;
	}
	else
	{
		/*w534*/(void)sdf_ivk_uaInitSessionParam(&(pDestMediaHandling->pPeersSession),pErr);
		/*w534*/(void)sdf_ivk_uaCloneSessionParam(pDestMediaHandling->pPeersSession,\
			pSrcMediaHandling->pPeersSession,pErr);
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneMediaHandling");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}

#ifdef SDF_SESSIONCHANGE
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneMediaChange
 **
 ** DESCRIPTION: This function copies the Sdf_st_mediaChange structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneMediaChange
#ifdef ANSI_PROTO 
(Sdf_st_mediaChange *pDestMediaChange,
Sdf_st_mediaChange *pSrcMediaChange,
Sdf_st_error *pErr)
#else
(pDestMediaChange,pSrcMediaChange, pErr )
Sdf_st_mediaChange *pDestMediaChange;
Sdf_st_mediaChange *pSrcMediaChange;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneMediaChange");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneMediaChange(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestMediaChange == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaChange():  "
			"pDestMediaChange param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSrcMediaChange == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneMediaChange():  "
			"pSrcMediaChange Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestMediaChange->dMediaChangeType = pSrcMediaChange->dMediaChangeType;
	pDestMediaChange->dPortChange = pSrcMediaChange->dPortChange;
	pDestMediaChange->dMediaConnectionChange = \
						pSrcMediaChange->dMediaConnectionChange;
	pDestMediaChange->dDirectionChange = pSrcMediaChange->dDirectionChange;
	pDestMediaChange->dCodecChange = pSrcMediaChange->dCodecChange;
	pDestMediaChange->dOtherAttributeChange = pSrcMediaChange->\
		dOtherAttributeChange;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneMediaChange");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionChange
 **
 ** DESCRIPTION: This function copies the Sdf_st_sessionChange structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionChange
#ifdef ANSI_PROTO 
(Sdf_st_sessionChange *pDestSessionChange,
Sdf_st_sessionChange *pSrcSessionChange,
Sdf_st_error *pErr)
#else
(pDestSessionChange,pSrcSessionChange, pErr )
Sdf_st_sessionChange *pDestSessionChange;
Sdf_st_sessionChange *pSrcSessionChange;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSessionChange");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneSessionChange(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestSessionChange == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionChange():  "
			"pDestSessionChange param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

	if(pSrcSessionChange == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionChange():  "
			"pSrcSessionChange Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	/* ================ Parameter validations end ================== */


	pDestSessionChange->dSessionChange = pSrcSessionChange->dSessionChange;
	pDestSessionChange->dSessionConnectionChange = 
			pSrcSessionChange->dSessionConnectionChange;
	
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSessionChange");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}
#endif

#ifdef SDF_QOS
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetQosType
 **
 ** DESCRIPTION: This function gets the QosType from qosAttrib structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetQosType
#ifdef ANSI_PROTO 
(Sdf_st_qosAttrib	*pQosAttrib,
Sdf_ty_qosType	*pQosType,
Sdf_st_error *pErr)
#else
( pQosAttrib, pQosType, pErr )
Sdf_st_qosAttrib	*pQosAttrib;
Sdf_ty_qosType	*pQosType;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaGetQosType");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetQosType(): Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pQosAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetQosType():  "
			"pQosAttrib param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pQosType == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetQosType():  "
			"pQosType Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	*pQosType = pQosAttrib->dQosType;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting sdf_ivk_uaGetQosType");

	pErr->errCode = Sdf_en_noUaError;
	return 	Sdf_co_success;
}
#endif

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaStreamAtIndex
 **
 ** DESCRIPTION: This function gets the MediaStream str. at a given index from
 **              the SessionParam structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamAtIndex
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam	*pSessionParams,
Sdf_st_mediaStream **ppMediaStream,
Sdf_ty_u8bit dIndex,
Sdf_st_error *pErr)
#else
( pSessionParams, ppMediaStream, dIndex, pErr )
Sdf_st_sessionParam *pSessionParams;
Sdf_st_mediaStream **ppMediaStream;
Sdf_ty_u8bit dIndex;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetMediaStreamAtIndex");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetMediaStreamAtIndex(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamAtIndex():  "
			"pSessionParams param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamAtIndex():  "
			"ppMediaStream Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (dIndex > pSessionParams->dNumMediaStreams)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamAtIndex():  "
			"Invalid index value passed into the fn.", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	if (pSessionParams->pMediaStreams[dIndex] != Sdf_co_null)
    {
		HSS_LOCKEDINCREF(pSessionParams->pMediaStreams[dIndex]->dRefCount);
	    *ppMediaStream = pSessionParams->pMediaStreams[dIndex];
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamAtIndex():  "
			"pMediaStreams at the given index in pSessionParams is invalid ",
			pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetMediaStreamAtIndex");
	return Sdf_co_success;
}



#ifdef SDF_QOS
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetSegmentQosDetails
 **
 ** DESCRIPTION: This function gets the SegmentedQos from the qosAttrib str.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSegmentQosDetails
#ifdef ANSI_PROTO 
(Sdf_st_qosAttrib *pCurrQos,
Sdf_st_segmentedQosDetails **ppSegmentedQos,
Sdf_st_error *pErr)
#else
( pCurrQos, ppSegmentedQos, pErr )
Sdf_st_qosAttrib *pCurrQos;
Sdf_st_segmentedQosDetails **ppSegmentedQos;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetSegmentQosDetails");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetSegmentQosDetails(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCurrQos == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSegmentQosDetails():  "
			"pCurrQos param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppSegmentedQos == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSegmentQosDetails():  "
			"ppSegmentedQos Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (pCurrQos->u.pSegmentedQos != Sdf_co_null)
    {
		HSS_LOCKEDINCREF(pCurrQos->u.pSegmentedQos->dRefCount);
	    *ppSegmentedQos = pCurrQos->u.pSegmentedQos;
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSegmentQosDetails():  "
			"pSegmentedQos in  pCurrQos is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetSegmentQosDetails");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetE2eQosDetails
 **
 ** DESCRIPTION: This function gets the End to End Qos Attrib from the 
 **              qosAttrib str.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetE2eQosDetails
#ifdef ANSI_PROTO 
(Sdf_st_qosAttrib *pDesQos,
Sdf_st_e2eQosDetails **ppE2eQos,
Sdf_st_error *pErr)
#else
( pDesQos, ppE2eQos, pErr )
Sdf_st_qosAttrib *pDesQos;
Sdf_st_e2eQosDetails **ppE2eQos;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetE2eQosDetails");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetE2eQosDetails(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDesQos == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetE2eQosDetails():  "
			"pDesQos param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppE2eQos == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetE2eQosDetails():  "
			"ppE2eQos Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (pDesQos->u.pE2eQos != Sdf_co_null)
    {
		HSS_LOCKEDINCREF(pDesQos->u.pE2eQos->dRefCount);
	    *ppE2eQos = pDesQos->u.pE2eQos;
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetE2eQosDetails():  "
			"pE2eQos in pDesQos is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetE2eQosDetails");
	return Sdf_co_success;
}
#endif

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetSessionParamsFromMediaInfo
 **
 ** DESCRIPTION: This function gets the session parameters from the Media 
 **				 Handling str. based on the type of SessionParam.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSessionParamsFromMediaInfo
#ifdef ANSI_PROTO 
(Sdf_st_mediaHandling *pMediaInfo,
Sdf_st_sessionParam **ppSessionParam,
Sdf_ty_sessionParamType dSessionParamType,
Sdf_st_error *pErr)
#else
( pMediaInfo, ppSessionParam, dSessionParamType, pErr )
Sdf_st_mediaHandling *pMediaInfo;
Sdf_st_sessionParam **ppSessionParam;
Sdf_ty_sessionParamType dSessionParamType;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_sessionParam *pSessionParam = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetSessionParamsFromMediaInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetSessionParamsFromMediaInfo(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pMediaInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionParamsFromMediaInfo():  "
			"pMediaInfo param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppSessionParam == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionParamsFromMediaInfo():  "
			"ppSessionParam Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (dSessionParamType == Sdf_en_currentSession)
    {
		pSessionParam = pMediaInfo->pCurrentSession;
    }
	else if (dSessionParamType == Sdf_en_lastOfferRecvd)
    {
		pSessionParam = pMediaInfo->pLastOfferRecd;
    }
	else if (dSessionParamType == Sdf_en_lastOfferMade)
    {
		pSessionParam = pMediaInfo->pLastOfferMade;
    }
	else if (dSessionParamType == Sdf_en_peerSession)
    {
		pSessionParam = pMediaInfo->pPeersSession;
    }
	else if (dSessionParamType == Sdf_en_selfCapabilities)
    {
		pSessionParam = pMediaInfo->pCapabilities;
    }
		
	if (pSessionParam != Sdf_co_null)
    {
	    HSS_LOCKEDINCREF(pSessionParam->dRefCount);
		*ppSessionParam = pSessionParam;
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionParamsFromMediaInfo():  "
			"The corresponding session param in pMediaInfo is null ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetSessionParamsFromMediaInfo");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaInfo
 **
 ** DESCRIPTION: This function retrieves the Media Handling structure from the
 **              Call Object.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaInfo
#ifdef ANSI_PROTO 
(Sdf_st_callObject *pObject,
Sdf_st_mediaHandling **ppMediaInfo,
Sdf_st_error *pErr)
#else
( pObject, ppMediaInfo, pErr )
Sdf_st_callObject *pObject;
Sdf_st_mediaHandling **ppMediaInfo;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetMediaInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetMediaInfo(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaInfo():  "
			"pObject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppMediaInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaInfo():  "
			"ppMediaInfo Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (pObject->pMediaInfo != Sdf_co_null)
    {
	    HSS_LOCKEDINCREF(pObject->pMediaInfo->dRefCount);
		*ppMediaInfo = pObject->pMediaInfo;	
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaInfo():  "
			"pMediaInfo in pObject is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting sdf_ivk_uaGetMediaInfo");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetSessionConnection
 **
 ** DESCRIPTION: This function retrieves the session level connection from the
 **              Session Param structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSessionConnection
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam *pSessionParams,
SdpConnection **ppConnection,
Sdf_st_error *pErr)
#else
( pSessionParams, ppConnection, pErr )
Sdf_st_sessionParam *pSessionParams;
SdpConnection **ppConnection;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetSessionConnection");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetSessionConnection(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionConnection():  "
			"pSessionParams param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppConnection == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionConnection():  "
			"ppConnection Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (pSessionParams->pConnection != Sdf_co_null)
    {
	    HSS_LOCKEDINCREF(pSessionParams->pConnection->dRefCount);
		*ppConnection = pSessionParams->pConnection;	
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionConnection():  "
			"pConnection in pSessionParams is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_ivk_uaGetSessionConnection");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetSdpFromSessionParam
 **
 ** DESCRIPTION: This function retrieves the session level SdpMessage from the
 **              Session Param structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSdpFromSessionParam
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam *pSessionParams,
SdpMessage **ppSdpMsg,
Sdf_st_error *pErr)
#else
( pSessionParams, ppSdpMsg, pErr )
Sdf_st_sessionParam *pSessionParams;
SdpConnection **ppSdpMsg;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetSdpFromSessionParam");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetSdpFromSessionParam(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSdpFromSessionParam():  "
			"pSessionParams param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppSdpMsg == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSdpFromSessionParam():  "
			"ppSdpMsg Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */
#ifndef SDF_PARAMVALIDATION 
#ifdef SDF_UAB2B
(void) ppSdpMsg;
#endif 
#endif
#ifndef SDF_UAB2B
/*SPR-4217 :B2B doesn't use pSdpMessage  data member and all the processing is done on other data member of sessionParam structure. It is effecting there performance. so kept under flag.  */

	if (pSessionParams->pSdpMessage != Sdf_co_null)
    {
	    HSS_LOCKEDINCREF(pSessionParams->pSdpMessage->dRefCount);
		*ppSdpMsg = pSessionParams->pSdpMessage;	
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSdpFromSessionParam():  "
			"pSdpMessage in pSessionParams is null ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }
#endif /*4217*/
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - Exiting "
			"sdf_ivk_uaGetSdpFromSessionParam");
	return Sdf_co_success;
}

#ifdef SDF_QOS
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetQosFromMediaStream
 **
 ** DESCRIPTION: This function gets the Qos details from the Media Stream
 **				 str. based on the type of reservation.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetQosFromMediaStream
#ifdef ANSI_PROTO 
(Sdf_st_mediaStream *pMediaStream,
Sdf_st_qosAttrib **ppQosAttrib,
Sdf_ty_reservTypeInfo dReservType,
Sdf_st_error *pErr)
#else
( pMediaStream, ppQosAttrib, dReservType, pErr )
Sdf_st_mediaStream *pMediaStream;
Sdf_st_qosAttrib **ppQosAttrib;
Sdf_ty_reservTypeInfo dReservType;
Sdf_st_error *pErr;
#endif
{
	Sdf_st_qosAttrib *pQosAttrib = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetQosFromMediaStream");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetQosFromMediaStream(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetQosFromMediaStream():  "
			"pMediaStream param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppQosAttrib == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetQosFromMediaStream():  "
			"ppQosAttrib Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (dReservType == Sdf_en_currQos)
    {
		pQosAttrib = pMediaStream->pCurrentQos;
    }
	else if (dReservType == Sdf_en_desQos)
    {
		pQosAttrib = pMediaStream->pDesiredQos;
    }
	else if (dReservType == Sdf_en_confQos)
    {
		pQosAttrib = pMediaStream->pConfirmedQos;
    }

	if (pQosAttrib != Sdf_co_null)
    {
	    HSS_LOCKEDINCREF(pQosAttrib->dRefCount);
		*ppQosAttrib = pQosAttrib;
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetQosFromMediaStream():  "
			"The corresponding Qos details in pMediaStream is null ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetQosFromMediaStream");
	return Sdf_co_success;
}
#endif

#ifdef SDF_MEDIAGROUP
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaStreamGroupAtIndex
 **
 ** DESCRIPTION: This function gets the MediaStreamGroup str. at a given 
 **              index from the SessionParam structure.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamGroupAtIndex
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam	*pSessionParams,
Sdf_st_mediaStreamGroup **ppMediaStreamGroup,
Sdf_ty_u8bit dIndex,
Sdf_st_error *pErr)
#else
( pSessionParams, ppMediaStreamGroup, dIndex, pErr )
Sdf_st_sessionParam *pSessionParams;
Sdf_st_mediaStreamGroup **ppMediaStreamGroup;
Sdf_ty_u8bit dIndex;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetMediaStreamGroupAtIndex");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetMediaStreamGroupAtIndex(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamGroupAtIndex():  "
			"pSessionParams param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppMediaStreamGroup == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamGroupAtIndex():  "
			"ppMediaStreamGroup Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	if (dIndex > pSessionParams->dNumGroups)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamGroupAtIndex():  "
			"Invalid index value passed into the fn.", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	if (pSessionParams->pMediaStreamGroups[dIndex] != Sdf_co_null)
    {
		HSS_LOCKEDINCREF(pSessionParams->pMediaStreamGroups[dIndex]->dRefCount);
	    *ppMediaStreamGroup = pSessionParams->pMediaStreamGroups[dIndex];
    }
	else
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamGroupAtIndex():  "
			"pMediaStreamGroups at the given index in pSessionParams is "
			"invalid ", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetMediaStreamGroupAtIndex");
	return Sdf_co_success;
}
#endif

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetMediaStreamByMid
 **
 ** DESCRIPTION: This function gets the MediaStream str. based on the MID 
 **              passed.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetMediaStreamByMid
#ifdef ANSI_PROTO 
(Sdf_st_sessionParam	*pSessionParams,
Sdf_st_mediaStream **ppMediaStream,
Sdf_ty_s8bit *pMid,
Sdf_st_error *pErr)
#else
( pSessionParams, ppMediaStream, pMid, pErr )
Sdf_st_sessionParam *pSessionParams;
Sdf_st_mediaStream **ppMediaStream;
Sdf_ty_s8bit *pMid;
Sdf_st_error *pErr;
#endif
{
	Sdf_ty_bool  dStreamFound = Sdf_co_false;
	Sdf_ty_u8bit i;
	Sdf_st_mediaStream *pMediaStream = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
					"sdf_ivk_uaGetMediaStreamByMid");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaGetMediaStreamByMid(): Error Param "
			"Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pSessionParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamByMid():  "
			"pSessionParams param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(ppMediaStream == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamByMid():  "
			"ppMediaStream Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(pMid == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamByMid():  "
			"pMid param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	for (i = 0; i < pSessionParams->dNumMediaStreams; i++)
    {
		pMediaStream = pSessionParams->pMediaStreams[i];

		if (Sdf_mc_strcmp(pMediaStream->pMediaStreamId, pMid) == 0)
        {
		    HSS_LOCKEDINCREF(pMediaStream->dRefCount);
			*ppMediaStream = pMediaStream;
			dStreamFound = Sdf_co_true;
			break;
        }
    }

	if (dStreamFound == Sdf_co_false)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMediaStreamByMid():  "
			"MediaStream not found for the given MID.", pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
    }

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaGetMediaStreamByMid");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: Sdf_ivk_uaGetSessionParamFromSipMsg
 **
 ** DESCRIPTION: This API returns the session param from the sdp message 
 **              present in the sip message
 *****************************************************************************/
 Sdf_ty_retVal sdf_ivk_uaGetSessionParamFromSipMsg 
#ifdef ANSI_PROTO
		(SipMessage *pSipMsg, Sdf_st_sessionParam **ppSessionParam,
		Sdf_ty_conversionType	dConversionType,Sdf_st_error *pError)
#else 
		(pSipMsg, ppSessionParam,dConversionType,pError)
		SipMessage *pSipMsg;
		Sdf_st_sessionParam **ppSessionParam;
		Sdf_ty_conversionType	dConversionType;
		Sdf_st_error *pError;
#endif
{
	Sdf_ty_u32bit i=0;
	SipMsgBody *pMsgBody = Sdf_co_null;
	SdpMessage *pSdpMessage =Sdf_co_null;
	Sdf_ty_retVal sdpFound = Sdf_co_fail;
	Sdf_ty_u32bit dCount=0; 

#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pSipMsg == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

		(void)sip_getMsgBodyCount(pSipMsg, &dCount, \
		(SipError*) &(pError->stkErrCode));

	if (dCount == 0)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
			"No SDP in received SIP message.", pError);
#endif		
		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}

	/* 	======================================
		  Locate SDP in the message bodies
		====================================== */
	for (i = 0; i < dCount; i++)
	{
		en_SipMsgBodyType dType;
		if (sip_getMsgBodyTypeAtIndex(pSipMsg, \
			&dType, i, (SipError*)&(pError->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
				"Failed to get the type of message body", pError);
#endif			
			pError->errCode=Sdf_en_sdpError;
			return Sdf_co_fail;
		}
		if (dType == SipSdpBody)
		{
			if (sip_getMsgBodyAtIndex(pSipMsg, \
				&pMsgBody, i, (SipError*)&(pError->stkErrCode)) \
				== SipFail)
			{
#ifdef SDF_ERROR				
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
					"Failed to get the message body from the SipMessage", \
					pError);
#endif				
				pError->errCode=Sdf_en_sdpError;
				return Sdf_co_fail;
			}
			sdpFound = Sdf_co_success;	
			break;	
		}
	}
	if (sdpFound == Sdf_co_fail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
			"No SDP in received SIP message.", pError);
#endif		
		/* free local variables */
	    sip_freeSipMsgBody(pMsgBody);

		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}

	/* 	=========================================
		  get the "incoming" SdpMessage struct
		========================================= */
	if (sip_getSdpFromMsgBody(pMsgBody, &pSdpMessage, \
		(SipError*)&(pError->stkErrCode)) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromSipMsg( ): "
			"Failed to get the Sdp Message that was cloned.", pError);
#endif		
	    /* free local variables */
	    sip_freeSipMsgBody(pMsgBody);

		pError->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
	}
	/* free local variables */
	sip_freeSipMsgBody(pMsgBody);

	if(sdf_ivk_uaInitSessionParam(ppSessionParam, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetSessionParamFromSipMsg(): "
			"Could not init the Sdf_st_msgBody structure.",pError);
#endif				
     	sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
    if(sdf_ivk_uaFormSessionParam(pSdpMessage,  *ppSessionParam, \
		 dConversionType, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR				
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_memoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaGetSessionParamFromSipMsg(): "
			"Could not convert the SdpMessage to SessionParam.",pError);
#endif
		(void)sdf_ivk_uaFreeSessionParam(*ppSessionParam);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
	}
	/* free the local reference */
	sip_freeSdpMessage(pSdpMessage);
	return Sdf_co_success;
}

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
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData, Sdf_st_sessionParam **ppSessionParam,
	 Sdf_st_error *pError)
#else
	(pInitData, ppSessionParam,pError)
	Sdf_st_initData *pInitData; 
	Sdf_st_sessionParam **ppSessionParam;
	 Sdf_st_error *pError;
#endif
{
#ifdef SDF_PARAMVALIDATION
	/*
	 * Parameters check 
	 */
	if ((pError == Sdf_co_null)||(pInitData == Sdf_co_null) ||
		(ppSessionParam == Sdf_co_null))
	{
		if (pError != Sdf_co_null)
		{
			pError->errCode = Sdf_en_invalidParamError;
#ifdef SDF_ERROR			
			sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_fn_uaGetSessionParamFromInitData( ): "
			"Invalid Param passed to function", pError);
#endif			
		}
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	if(pInitData->pSessionParam == Sdf_co_null)
	{
		pError->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	*ppSessionParam = pInitData->pSessionParam;
	HSS_LOCKEDINCREF(pInitData->pSessionParam->dRefCount);

	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

