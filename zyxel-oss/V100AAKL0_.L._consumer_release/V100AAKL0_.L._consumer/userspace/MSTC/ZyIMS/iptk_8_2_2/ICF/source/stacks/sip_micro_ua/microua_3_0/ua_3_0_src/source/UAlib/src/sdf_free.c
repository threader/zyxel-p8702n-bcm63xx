/*****************************************************************************
 ** FUNCTION:
 **			This file has the freeing functions for all structures
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_free.c
 **
 ** DESCRIPTION		: This file has the freeing functions for all structures
 **
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27/10/00	Anitha						Creation
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 **
 *****************************************************************************
 **     COPYRIGHT , Aricent, 2006
 *****************************************************************************/


#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf_free.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_hash.h"
#include "sdf_callbacks.h"
#include "sdf_internal.h"
#include "sdf_sdpfree.h"
#include "sdf.h"
#ifndef SDF_LINT
#include "sdf_mempool.h"
#include "sdf_sdpstruct.h"
#include "sdf_portlayer.h"
#include "sdf_list.h"
#endif
#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

#ifdef SDF_HA
#include "sdf_haapiint.h"
#endif

#ifdef SDF_LINT
#ifdef SDF_USE_INTERNALTHREADS
/* Retransmission thread spawned by the toolkit */
extern Sdf_ty_threadT dGlbRemoteRetransThread;
#endif
#endif

#ifndef SDF_NETWORKCALLBACKS
extern Sdf_st_fdSetInfo	dGlbFdSet;
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
#include "sdf_subslayerapi.h"
#include "sdf_subslayertimer.h"
#include "sdf_subslayerinitfree.h"
#include "sdf_subslayerstatemach.h"
#include "sdf_subslayerstatistics.h"

#endif /* End of SDF_SUBSCRIPTION_LAYER */
#ifdef SDF_DEP
#include "sdf_dep_eventMgr.h"
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 ** FUNCTION: sdf_fn_uaDepInitDepDetails
 **
 ** DESCRIPTION: This function is used to free the 
                 Sdf_st_depDetailsstructure
 **
 ** PARAMETERS:
 **   pDepDetails(OUT)    : Free Sdf_st_depDetails structure
	
******************************************************************************/

void sdf_fn_uaDepFreeDepDetails(
  Sdf_st_depDetails *pDepDetails);

#ifdef __cplusplus
}
#endif
#endif
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeNameInfo
**
** DESCRIPTION: This function frees the NameInfo structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeNameInfo
#ifdef ANSI_PROTO
	(Sdf_st_nameInfo *pNameInfo)
#else
	( pNameInfo )
	 Sdf_st_nameInfo *pNameInfo;
#endif
{
	Sdf_st_error Error;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering sdf_ivk_uaFreeNameInfo");

	if(pNameInfo == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pNameInfo->dRefCount);
	HSS_DECREF(pNameInfo->dRefCount);
	
	if (HSS_CHECKREF(pNameInfo->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pNameInfo->pName),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pNameInfo->pAddress),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&\
			(pNameInfo->pTag),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pNameInfo->pScheme),(Sdf_st_error*)Sdf_co_null);

		(void)sdf_listDeleteAll(&(pNameInfo->slUrlParams), &Error);
		(void)sdf_listDeleteAll(&(pNameInfo->slHeaderParams), &Error);

		HSS_UNLOCKREF(pNameInfo->dRefCount);
		HSS_DELETEREF(pNameInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pNameInfo), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pNameInfo->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeNameInfo ");

	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: __sdf_fn_uaFreeTransportInfo
**
** DESCRIPTION: This function frees the Transport-Info structure
**              by taking a void pointer.
**
**
**********************************************************/
void __sdf_fn_uaFreeTransportInfo(Sdf_ty_pvoid pTransInfo)
{
	(void)sdf_ivk_uaFreeTransportInfo((Sdf_st_transportInfo *)pTransInfo);
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeTransportInfo
**
** DESCRIPTION: This function frees the Teansport-Info
**	 structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeTransportInfo 
#ifdef ANSI_PROTO
	(Sdf_st_transportInfo *pTransportInfo)
#else
	( pTransportInfo )
	 Sdf_st_transportInfo *pTransportInfo;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeTransportInfo");

	if(pTransportInfo == Sdf_co_null)
		return Sdf_co_success;
	
	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pTransportInfo->dRefCount);
	HSS_DECREF(pTransportInfo->dRefCount);
	
	if (HSS_CHECKREF(pTransportInfo->dRefCount))
	{
        pTransportInfo->dScheme = Sdf_en_protoNone;

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&\
			(pTransportInfo->pIp),(Sdf_st_error*)Sdf_co_null);
		pTransportInfo->dPort = 0;

		HSS_UNLOCKREF(pTransportInfo->dRefCount);
		HSS_DELETEREF(pTransportInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&\
			(pTransportInfo),(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pTransportInfo->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFreeTransportInfo");

	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeHeaderList
**
** DESCRIPTION: This function frees the HeaderList 
**	 structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeHeaderList
#ifdef ANSI_PROTO
	(Sdf_st_headerList *pHeaderList)
#else
	(pHeaderList)
	 Sdf_st_headerList *pHeaderList;
#endif
{
	Sdf_st_error Error;
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeHeaderList");	
	
	if(pHeaderList == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pHeaderList->dRefCount);
	HSS_DECREF(pHeaderList->dRefCount);
	
	if (HSS_CHECKREF(pHeaderList->dRefCount))
	{
		if (sdf_listDeleteAll(&(pHeaderList->slHeaders), &Error)==Sdf_co_fail)
			return Sdf_co_fail;

		(void)sdf_memfree(Sdf_mc_freeMemId, \
			(Sdf_ty_pvoid*)&(pHeaderList->pCorrelation), &Error);

		HSS_UNLOCKREF(pHeaderList->dRefCount);
		HSS_DELETEREF(pHeaderList->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pHeaderList), &Error);
	}
	else
	{
		HSS_UNLOCKREF(pHeaderList->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG-Exiting sdf_ivk_uaFreeHeaderList");

	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeCommonInfo
**
** DESCRIPTION: This function frees the Common-Info
**	 structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeCommonInfo
#ifdef ANSI_PROTO
	(Sdf_st_commonInfo *pCommonInfo)
#else
	(pCommonInfo)
	 Sdf_st_commonInfo *pCommonInfo;
#endif
{
	Sdf_st_error dError;
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeCommonInfo");

	if(pCommonInfo == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */

	HSS_LOCKREF(pCommonInfo->dRefCount);
	HSS_DECREF(pCommonInfo->dRefCount);

	if (HSS_CHECKREF(pCommonInfo->dRefCount))
	{
		(void)sdf_ivk_uaFreeCallObjectKey(pCommonInfo->pKey);
		sip_freeSipHeader(pCommonInfo->pFrom);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&(pCommonInfo->pFrom), \
			(Sdf_st_error*)Sdf_co_null);

		sip_freeSipHeader(pCommonInfo->pTo);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&(pCommonInfo->pTo), \
			(Sdf_st_error*)Sdf_co_null);
		sip_freeSipAddrSpec( pCommonInfo->pRequestUri);
        /* Changes for CSR: 1-5210208 */
		sip_freeSipAddrSpec( pCommonInfo->pNewRequestUri);
		/* Changes for CSR: 1-5210208 */

		(void)sdf_listDeleteAll(&(pCommonInfo->slContact), &dError);
		(void)sdf_listDeleteAll(&(pCommonInfo->slInvRoute), &dError);

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroySyncObject(&((pCommonInfo->slContact).dSyncObject));
		sdf_fn_uaDestroySyncObject(&((pCommonInfo->slInvRoute).dSyncObject));
#endif
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pCommonInfo->pCallid), \
			(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
				&(pCommonInfo->pChallangePasswd), (Sdf_st_error*)Sdf_co_null);
        pCommonInfo->dScheme = Sdf_en_protoNone;

#ifdef SIP_RPR
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pCommonInfo->pRPRMethod),\
			(Sdf_st_error*)Sdf_co_null);
#endif /* SIP_RPR */
#ifdef SDF_TXN
		/* Purify Fix Pankaj */
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCommonInfo->pNoAnswerTimerHandle), (Sdf_st_error*)Sdf_co_null);
		/* Purify Fix Pankaj */
#endif

		HSS_UNLOCKREF(pCommonInfo->dRefCount);
		HSS_DELETEREF(pCommonInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pCommonInfo),\
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pCommonInfo->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeCommonInfo");
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeAppData
**
** DESCRIPTION: This function frees the AppData
**	 structure
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeAppData
#ifdef ANSI_PROTO
	(Sdf_st_appData *pAppData)
#else
	(pAppData)
	 Sdf_st_appData *pAppData;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_ivk_uaFreeAppData");

	if(pAppData == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pAppData->dRefCount);
	HSS_DECREF(pAppData->dRefCount);

	if (HSS_CHECKREF(pAppData->dRefCount))
	{
#ifdef SDF_TRACE
		Sdf_st_error dErr;

		sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces, \
			(Sdf_ty_s8bit *)"Issuing callback sdf_cbk_uaFreeApplicationData",\
			0, &dErr);
#endif
		(void)sdf_cbk_uaFreeApplicationData(&(pAppData->pData));

		HSS_UNLOCKREF(pAppData->dRefCount);
		HSS_DELETEREF(pAppData->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pAppData),(Sdf_st_error*)\
			Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pAppData->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeAppData");
	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeCallObject
**
** DESCRIPTION: This function frees the Call Object
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeCallObject 
#ifdef ANSI_PROTO
	(Sdf_st_callObject **ppCallObj)
#else
	(ppCallObj)
	 Sdf_st_callObject **ppCallObj;
#endif
{
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_ivk_uaFreeCallObject");

	if(*ppCallObj == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF((*ppCallObj)->dRefCount);
	HSS_DECREF((*ppCallObj)->dRefCount);

	if (HSS_CHECKREF((*ppCallObj)->dRefCount))
	{
	
#ifdef SDF_SESSION_TIMER
		(void)sdf_ivk_uaFreeSessionTimerInfo((*ppCallObj)->pSessionTimer);
#endif	
		(void)sdf_ivk_uaFreeCommonInfo((*ppCallObj)->pCommonInfo);
		(void)sdf_ivk_uaFreeTransaction((*ppCallObj)->pUacTransaction);
		(void)sdf_ivk_uaFreeTransaction((*ppCallObj)->pUasTransaction);
		(void)sdf_ivk_uaFreeCallInfo((*ppCallObj)->pCallInfo);
		(void)sdf_listDeleteAll(&((*ppCallObj)->slOverlapTransInfo), &dError);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroySyncObject(&(((*ppCallObj)->slOverlapTransInfo).dSyncObject));
#endif
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroyMutex(&((*ppCallObj)->dCallObjectMutex));
		sdf_fn_uaDestroyMutex(&((*ppCallObj)->dCallStateMutex));
#endif
		(void)sdf_listDeleteAll(&((*ppCallObj)->slPersistentHeaders), &dError);
		(void)sdf_listDeleteAll(&((*ppCallObj)->slPersistentMsgBodyList), &dError);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroySyncObject(&(((*ppCallObj)->slPersistentHeaders).dSyncObject));
		sdf_fn_uaDestroySyncObject(&(((*ppCallObj)->slPersistentMsgBodyList).dSyncObject));
#endif

		(void)sdf_ivk_uaFreeAppData((*ppCallObj)->pAppData);
		(void)sdf_ivk_uaFreeInitData((*ppCallObj)->pInitData);
		/*Free the Media Handling Structre*/
		(void)sdf_ivk_uaFreeMediaHandling((*ppCallObj)->pMediaInfo);
#ifdef SDF_HA
		(void)sdf_fn_uaFreeCallObjectHAInfo((*ppCallObj)->pHaInfo, &dError);
#endif
#ifdef SDF_DEP
        (void)sdf_fn_uaDepFreeDepDetails((*ppCallObj)->pDepDetails);
#endif  
		HSS_UNLOCKREF((*ppCallObj)->dRefCount);
		HSS_DELETEREF((*ppCallObj)->dRefCount);

#ifdef SDF_SUBSCRIPTION_LAYER
    (*ppCallObj)->dDialogUsageCount = 0;
		(void)sdf_listDeleteAll(&((*ppCallObj)->slSubsObjList), &dError);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroySyncObject(&(((*ppCallObj)->slSubsObjList).dSyncObject));
#endif
#endif /* End of SDF_SUBSCRIPTION_LAYER */

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)ppCallObj,(Sdf_st_error*)\
			Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF((*ppCallObj)->dRefCount);
	}
		
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Exiting sdf_ivk_uaFreeCallObject");

	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_fn_uaFreeRemoteRetransKey
**
** DESCRIPTION: This function frees the RemoteRetransKey 
**	structure.
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaFreeRemoteRetransKey
#ifdef ANSI_PROTO
	(Sdf_st_remoteRetransKey *pRemoteRetransKey)
#else
	( pRemoteRetransKey )
	 Sdf_st_remoteRetransKey *pRemoteRetransKey;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_fn_uaFreeRemoteRetransKey");

	if(pRemoteRetransKey == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pRemoteRetransKey->dRefCount);
	HSS_DECREF(pRemoteRetransKey->dRefCount);

	if (HSS_CHECKREF(pRemoteRetransKey->dRefCount))
	{
		/*
		 * Free the From header.
		 */
		sip_freeSipHeader(pRemoteRetransKey->pFromHeader);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&(\
			pRemoteRetransKey->pFromHeader), Sdf_co_null);
		
		/*
		 * Free the To header.
		 */
		sip_freeSipHeader(pRemoteRetransKey->pToHeader);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&(\
			pRemoteRetransKey->pToHeader), Sdf_co_null);

		/*
		 * Free the callid and method.
		 */
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)\
			&(pRemoteRetransKey->pCallId), (Sdf_st_error *)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)\
			&(pRemoteRetransKey->pMethod), (Sdf_st_error *)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)\
			&(pRemoteRetransKey->pViaBranch), (Sdf_st_error *)Sdf_co_null);
		HSS_UNLOCKREF(pRemoteRetransKey->dRefCount);
		HSS_DELETEREF(pRemoteRetransKey->dRefCount);

		/*
		 * Finally, free the structure.
		 */
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)\
			&(pRemoteRetransKey), (Sdf_st_error *)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pRemoteRetransKey->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Exiting sdf_fn_uaFreeRemoteRetransKey");
	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_fn_uaFreeRemoteRetransBuffer
**
** DESCRIPTION: This function frees the remote Retrans Buffer
**	structure.
**
**********************************************************/
Sdf_ty_retVal sdf_fn_uaFreeRemoteRetransBuffer
#ifdef ANSI_PROTO
	(Sdf_st_remoteRetransBuffer *pRemoteRetransBuffer)
#else
	( pRemoteRetransBuffer )
	 Sdf_st_remoteRetransBuffer *pRemoteRetransBuffer;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_fn_uaFreeRemoteRetransBuffer");

	if(pRemoteRetransBuffer == Sdf_co_null)
		return Sdf_co_success;

	HSS_LOCKREF(pRemoteRetransBuffer->dRefCount);
	HSS_DECREF(pRemoteRetransBuffer->dRefCount);

	if (HSS_CHECKREF(pRemoteRetransBuffer->dRefCount))
	{
#ifdef SDF_USE_PREALLOCBUFFERS
		(void)sdf_ivk_uaFreeEventContext(pRemoteRetransBuffer->pContext);
#else
		(void)sdf_memfree(Sdf_mc_freeMemId, \
			(Sdf_ty_pvoid*)&(pRemoteRetransBuffer->pBuffer), \
			(Sdf_st_error*)Sdf_co_null);
#endif
		(void)sdf_fn_uaFreeRemoteRetransKey(pRemoteRetransBuffer->pKey);
		(void)sdf_ivk_uaFreeTransportInfo(pRemoteRetransBuffer->pDestInfo);

		(void)sdf_ivk_uaFreeAppData(pRemoteRetransBuffer->pAppData);

		HSS_UNLOCKREF(pRemoteRetransBuffer->dRefCount);
		HSS_DELETEREF(pRemoteRetransBuffer->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pRemoteRetransBuffer),(\
			Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pRemoteRetransBuffer->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_fn_uaFreeRemoteRetransBuffer");
	return Sdf_co_success;	
}


/*********************************************************
** FUNCTION: __sdf_fn_uaFreeRemoteRetransBuffer
**
** DESCRIPTION: This function frees the remote Retrans Buffer
**	structure by taking a void pointer.
**
**********************************************************/
void __sdf_fn_uaFreeRemoteRetransBuffer
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid RemoteRetransBuffer)
#else
	(RemoteRetransBuffer)
	 Sdf_ty_pvoid RemoteRetransBuffer;
#endif
{
	(void)sdf_fn_uaFreeRemoteRetransBuffer((Sdf_st_remoteRetransBuffer *)\
		RemoteRetransBuffer);
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeInitData
**
** DESCRIPTION: This function frees the InitData structure
**	
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeInitData
#ifdef ANSI_PROTO
	(Sdf_st_initData *pInitData)
#else
	( pInitData )
	 Sdf_st_initData *pInitData;
#endif
{
	Sdf_st_error dError;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaFreeInitData");

	if(pInitData == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pInitData->dRefCount);
	HSS_DECREF(pInitData->dRefCount);

	if (HSS_CHECKREF(pInitData->dRefCount))
	{
		sip_freeSipHeader(pInitData->pFrom);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pInitData->pFrom),\
			(Sdf_st_error*)Sdf_co_null);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pInitData->pTag),(Sdf_st_error*)Sdf_co_null);

		(void)sdf_listDeleteAll(&(pInitData->slContact), &dError);
		(void)sdf_listFree(&(pInitData->slContact), &dError);
		(void)sdf_listDeleteAll(&(pInitData->slContactTransport), &dError);
		(void)sdf_listDeleteAll(&(pInitData->slPreLoadedRoute), \
			&dError);
		(void)sdf_ivk_uaFreeTransportInfo(pInitData->pRegistrar);
		(void)sdf_ivk_uaFreeTransportInfo(pInitData->pTransport);
		(void)sdf_listDeleteAll(&(pInitData->slMsgBodyList), \
			&dError);
		(void)sdf_listDeleteAll(&(pInitData->slExtraHeadersList), \
			&dError);
#ifdef SDF_IMS
       (void)sdf_listDeleteAll(&(pInitData->slRespHeadersList), \
			&dError);
#endif

		pInitData->sdf_fp_uaCallIdGen = Sdf_co_null;
		/*Free Media Handling Structre*/
		(void)sdf_ivk_uaFreeSessionParam(pInitData->pSessionParam);
	
#ifdef SDF_HA
		(void)sdf_fn_uaFreeUserProfileHAInfo(pInitData->pHaInfo, &dError);
#endif

#ifdef SDF_TLS
		(void)sdf_ivk_uaFreeSslData(pInitData->pSslData);
#endif

		HSS_UNLOCKREF(pInitData->dRefCount);
		HSS_DELETEREF(pInitData->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pInitData),\
			(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pInitData->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaFreeInitData");

	return Sdf_co_success;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaCloseToolkit
**
** DESCRIPTION: This function frees the toolkit data
**	that was initialized.
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaCloseToolkit
#ifdef ANSI_PROTO
	(void)
#else
	(void)
#endif
{
#ifndef SDF_TXN
#ifdef SDF_USE_INTERNALTHREADS	
	Sdf_ty_pvoid ptr = Sdf_co_null;
#endif
#endif
	
#if defined(SDF_TXN) || !defined(SDF_APP_CONTROLLED_REMOTE_RETX)
	Sdf_st_hashIterator dIterator;
#endif

	Sdf_st_error dErr;

#ifdef SDF_WATCHER_LIST
  Sdf_st_uaSLSubscriptionObject *pSubsObj   = Sdf_co_null;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaCloseToolkit");
	
#ifndef SDF_TXN
#ifdef SDF_USE_INTERNALTHREADS
	/*
	 * Stop the remote retransmission spooler thread
	 */
     if(sdf_fn_uaDeleteRemoteRetransThread(&dGlbRemoteRetransThread,ptr) == \
               Sdf_co_fail)
     {
        return Sdf_co_fail;
     }
#endif	 /* End - ifndef SDF_USE_INTERNALTHREADS */
#endif

#ifdef SDF_TXN
	/* 
	 * Free the SIP Stack transaction structure elements
	 * and the hash table itself
	 */
	if (pGlbToolkitData->pTxnFetchFunc == Sdf_co_null)
	{
		sdf_ivk_uaHashInitIterator(&(pGlbToolkitData->dSipTxnHash), \
			&dIterator);

		while (dIterator.pCurrentElement != Sdf_co_null)
		{
			SipTxnKey *pSipTxnKey = (SipTxnKey *)( \
								(dIterator.pCurrentElement)->pKey);
			SipTxnBuffer *pSipTxnBuffer	= \
				(SipTxnBuffer *)((dIterator.pCurrentElement)->pElement);
				
/*w534*/(void)sdf_ivk_uaHashRemove(&(pGlbToolkitData->dSipTxnHash), \
				(Sdf_ty_pvoid) pSipTxnKey);
			sdf_ivk_uaHashNext(&(pGlbToolkitData->dSipTxnHash), &dIterator);

			sip_freeSipTxnKey(pSipTxnKey);
			sip_freeEventContext(pSipTxnBuffer->pEventContext);
			sip_freeSipTxnBuffer(pSipTxnBuffer);
		}
		(void)sdf_ivk_uaHashFree(&(pGlbToolkitData->dSipTxnHash), &dErr);
	}
#endif

#ifdef SDF_WATCHER_LIST
    /*
     * Free the Watcher Subscription hash.
     */
	  sdf_ivk_uaHashInitIterator(&(pGlbToolkitData->dSubscriptionHash),\
                               &dIterator);

  	while (dIterator.pCurrentElement != Sdf_co_null)
  	{
      pSubsObj = (Sdf_st_uaSLSubscriptionObject *) dIterator.pCurrentElement->\
                                                                   pElement;
#if 0
      /* Stop Timer */
      /* Remove the subscription object from the aggregation only if the
      * Waiting timer isn't active.
      */
      (void)sdf_ivk_uaSLGetActiveTimerTypeAndValue(pSubsObj,&dExpValue,
                                                   &dTimerType,&dErr);
      (void)sdf_ivk_uaSLStopSubsTimer(pSubsObj, dTimerType, &pAppData, &dErr);
      /* Free the application data registered by the application.
      * at the time of starting the timer.
      */ 
      (void)sdf_cbk_uaFreeEventContextData(&pAppData);
#endif
			
      /* Get the next Element from the Hash then only free the current hash 
       * element 
	  	 */
	  	sdf_ivk_uaHashNext(&(pGlbToolkitData->dSubscriptionHash), &dIterator);
			
	  	(void)sdf_ivk_uaHashRemove(&(pGlbToolkitData->dSubscriptionHash),\
                                 (Sdf_ty_void *)pSubsObj->pSubsKey); 

  	} /* End of while */

    (void)sdf_ivk_uaHashFree(&(pGlbToolkitData->dSubscriptionHash),&dErr);
#endif /* End of SDF_WATCHER_LIST */
#ifdef SDF_DEP
		/* Free the event manager aggregations here */
		if(sdf_fn_uaDepFreeEventMgr() == Sdf_co_fail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,dErr.errCode, \
		  	(Sdf_ty_s8bit *)"sdf_ivk_uaCloseToolkit( ):"
			  "Failed to free DEP event Manager aggreagtions",&dErr);
#endif
				return Sdf_co_fail;
		}
#endif

#ifndef SDF_TXN

#ifndef SDF_APP_CONTROLLED_REMOTE_RETX
	/* 
	 * Free the remote retransmission elements 
	 * and the hash table itself
	 */
	sdf_ivk_uaHashInitIterator(&(pGlbToolkitData->dRemoteRetransHash), \
		&dIterator);

	while (dIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_remoteRetransBuffer *pBuffer = \
			(Sdf_st_remoteRetransBuffer *)(dIterator.pCurrentElement->pElement);
/*w534*/(void)sdf_ivk_uaHashRemove(&(pGlbToolkitData->dRemoteRetransHash), \
			(Sdf_ty_pvoid) pBuffer->pKey);
		sdf_ivk_uaHashNext(&(pGlbToolkitData->dRemoteRetransHash), \
			&dIterator);
	}
	(void)sdf_ivk_uaHashFree(&(pGlbToolkitData->dRemoteRetransHash), &dErr);
#endif
#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	sdf_cbk_uaFreeRemoteRetransTables (&dErr);
#endif

#endif /* End - ifndef SDF_TXN */


#ifdef SDF_USE_PREALLOCBUFFERS
	/* Free the prealloced mempool of direct buffers */
#ifdef SDF_APPCONTROLLEDMEMPOOL
	(void)sdf_cbk_uaDestroyPreAllocatedBuffers(&dErr);
#else
	sdf_ivk_uaMempoolDestroy (&dGlbPreallocMsgBufferPool);
#endif
#endif
	HSS_DELETEREF(pGlbToolkitData->dRefCount);

	/*
	 * Free the toolkit data structure now
	 */
	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pGlbToolkitData),\
		(Sdf_st_error*)Sdf_co_null);


#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Remote Retransmission Manager Stopped.\n", 0, &dErr);
#endif	

	/*
	 * Deallocate system resources that were used for DNS resolution of 
	 * hostnames
	 */
	sdf_fn_uaEndHostEnt();
#ifndef SDF_UASSF
	sip_releaseStack();
#endif

#ifndef SDF_NETWORKCALLBACKS
	(void)sdf_fn_uaClearFdSetInfo(&dGlbFdSet);
#endif

	
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaDestroyMutex(&dGlbPrintMutex);
	sdf_fn_uaDestroyMutex(&dGlbSdfCallObjectIdGeneratorMutex);
#endif

	
#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_initTraces,\
		(Sdf_ty_s8bit *)"UA Toolkit Closed.\n", 0, &dErr);
#endif	
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaCloseToolkit");

	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeHeaderList
**
** DESCRIPTION: This function frees the HeaderList 
**	 structure
**
**
**********************************************************/
void __sdf_fn_uaFreeHeaderList
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pHeaderList)
#else
	(pHeaderList)
	 Sdf_ty_pvoid pHeaderList;
#endif
{
	(void)sdf_ivk_uaFreeHeaderList((Sdf_st_headerList *)pHeaderList);
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeNameInfo
**
** DESCRIPTION: This internal function frees the NameInfo 
**	 structure
**
**
**********************************************************/
void __sdf_fn_uaFreeNameInfo
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pNameInfo)
#else
	(pNameInfo)
	 Sdf_ty_pvoid pNameInfo;
#endif
{
(void)sdf_ivk_uaFreeNameInfo((Sdf_st_nameInfo *)pNameInfo);
}

#ifdef SDF_AUTHENTICATE
/*********************************************************
** FUNCTION: sdf_ivk_uaFreeAuthenticationParams
**
** DESCRIPTION: This function frees the authenticationParams
**	structure.
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeAuthenticationParams
#ifdef ANSI_PROTO
	(Sdf_st_authenticationParams *pAuthenticationParams)
#else
	( pAuthenticationParams )
	 Sdf_st_authenticationParams *pAuthenticationParams;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaFreeAuthenticationParams");

	if(pAuthenticationParams == Sdf_co_null)
		return Sdf_co_success;

	HSS_LOCKREF(pAuthenticationParams->dRefCount);
	HSS_DECREF(pAuthenticationParams->dRefCount);

	if (HSS_CHECKREF(pAuthenticationParams->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pScheme),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pRealm), (Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pDomain), (Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pOpaque),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pQop),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pAlgorithm),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pNonce),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams->pStale),(Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pAuthenticationParams->dRefCount);
		HSS_DELETEREF(pAuthenticationParams->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pAuthenticationParams),(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pAuthenticationParams->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaFreeAuthenticationParams");
	return Sdf_co_success;	
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeAuthenticationParams
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_authenticationParams structure
**
**
**********************************************************/
void __sdf_fn_uaFreeAuthenticationParams
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pAuthenticationParams)
#else
	(pAuthenticationParams)
	 Sdf_ty_pvoid pAuthenticationParams;
#endif
{
	(void)sdf_ivk_uaFreeAuthenticationParams((Sdf_st_authenticationParams *)\
		pAuthenticationParams);
}



/*********************************************************
** FUNCTION: sdf_ivk_uaFreeUacAuthInfo
**
** DESCRIPTION: This function frees the Sdf_st_uacAuthInfo 
**	structure.
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeUacAuthInfo
#ifdef ANSI_PROTO
	(Sdf_st_uacAuthInfo *pUacAuthInfo)
#else
	( pUacAuthInfo )
	 Sdf_st_uacAuthInfo *pUacAuthInfo;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaFreeUacAuthInfo");

	if(pUacAuthInfo == Sdf_co_null)
		return Sdf_co_success;

	HSS_LOCKREF(pUacAuthInfo->dRefCount);
	HSS_DECREF(pUacAuthInfo->dRefCount);

	if (HSS_CHECKREF(pUacAuthInfo->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo->pUserName),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo->pPassword),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo->pQop),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo->pCnonce),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo->pNonceCount),(Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pUacAuthInfo->dRefCount);
		HSS_DELETEREF(pUacAuthInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUacAuthInfo),(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pUacAuthInfo->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaFreeUacAuthInfo");
	return Sdf_co_success;	
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeUacAuthInfo
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_uacAuthInfo structure
**
**
**********************************************************/
void __sdf_fn_uaFreeUacAuthInfo
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pUacAuthInfo)
#else
	(pUacAuthInfo)
	 Sdf_ty_pvoid pUacAuthInfo;
#endif
{
	(void)sdf_ivk_uaFreeUacAuthInfo((Sdf_st_uacAuthInfo *)pUacAuthInfo);
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeUasAuthInfo
**
** DESCRIPTION: This function frees the Sdf_st_uasAuthInfo 
**	structure.
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeUasAuthInfo
#ifdef ANSI_PROTO
	(Sdf_st_uasAuthInfo *pUasAuthInfo)
#else
	( pUasAuthInfo )
	 Sdf_st_uasAuthInfo *pUasAuthInfo;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaFreeUasAuthInfo");

	if(pUasAuthInfo == Sdf_co_null)
		return Sdf_co_success;

	HSS_LOCKREF(pUasAuthInfo->dRefCount);
	HSS_DECREF(pUasAuthInfo->dRefCount);

	if (HSS_CHECKREF(pUasAuthInfo->dRefCount))
	{
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pScheme),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pUserName),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pRealm),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pAlgorithm),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pNonce),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pCnonce),( Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo->pSecret),(Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pUasAuthInfo->dRefCount);
		HSS_DELETEREF(pUasAuthInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pUasAuthInfo),(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pUasAuthInfo->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaFreeUasAuthInfo");
	return Sdf_co_success;	
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeUasAuthInfo
**
** DESCRIPTION: This internal function frees the 
**				Sdf_st_uasAuthInfo structure
**
**
**********************************************************/
void __sdf_fn_uaFreeUasAuthInfo
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pUasAuthInfo)
#else
	(pUasAuthInfo)
	 Sdf_ty_pvoid pUasAuthInfo;
#endif
{
	(void)sdf_ivk_uaFreeUasAuthInfo((Sdf_st_uasAuthInfo *)pUasAuthInfo);
}
#endif /* SDF_AUTHENTICATE */


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeConfigExtraHeaders
**
** DESCRIPTION: This function frees the ConfigExtraHeaders
**				structure
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeConfigExtraHeaders 
#ifdef ANSI_PROTO
	(Sdf_st_configExtraHeaders *pConfHdr)
#else
	(pConfHdr)
	Sdf_st_configExtraHeaders *pConfHdr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Entering sdf_ivk_uaFreeConfigExtraHeaders");
	
	if(pConfHdr==Sdf_co_null)
		return Sdf_co_success;
	
	sip_freeSipParam(pConfHdr->pParam);

	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pConfHdr->pData),\
		(Sdf_st_error*)Sdf_co_null);
	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pConfHdr), \
		(Sdf_st_error*)Sdf_co_null);
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		"SDF_DEBUG - Exiting sdf_ivk_uaFreeConfigExtraHeaders");
	return Sdf_co_success;	
}

/*********************************************************
** FUNCTION: __sdf_fn_uaFreeConfigExtraHeaders
**
** DESCRIPTION: This function frees the ConfigExtraHeaders 
**	 structure
**
**
**********************************************************/
void __sdf_fn_uaFreeConfigExtraHeaders
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pConfHdr)
#else
	(pConfHdr)
	 Sdf_ty_pvoid pConfHdr;
#endif
{
(void)sdf_ivk_uaFreeConfigExtraHeaders((Sdf_st_configExtraHeaders *)pConfHdr);
}
/*********************************************************
** FUNCTION: __sdf_fn_uaFreeCompleteHeader
**
** DESCRIPTION: This function frees the SipHeader strucure completely. 
**
**
**********************************************************/
void __sdf_fn_uaFreeCompleteHeader(Sdf_ty_pvoid pHdr)
{
	sip_freeSipHeader((SipHeader *)pHdr);
	(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pHdr), \
		(Sdf_st_error*)Sdf_co_null);
}


/***********************************************************************
** Function: __sdf_fn_uaFreeMsgBody
** Description: Frees the MsgBody structure
**
************************************************************************/
void __sdf_fn_uaFreeMsgBody 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pMsgBody)
#else
(pMsgBody)
Sdf_ty_pvoid pMsgBody;
#endif
{
	(void)sdf_ivk_uaFreeMsgBody((Sdf_st_msgBody *)pMsgBody);
}

/***********************************************************************
** Function: sdf_ivk_uaFreeMsgBody
** Description: Frees the MsgBody structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeMsgBody 
#ifdef ANSI_PROTO
(Sdf_st_msgBody *pMsgBody)
#else
(pMsgBody)
Sdf_st_msgBody *pMsgBody;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering sdf_ivk_uaFreeMsgBody ");

	if(pMsgBody == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pMsgBody->dRefCount);
	HSS_DECREF(pMsgBody->dRefCount);

	if (HSS_CHECKREF(pMsgBody->dRefCount))
	{
		sip_freeSipMsgBody(pMsgBody->pMsgBody);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pMsgBody->pCorrelation),(Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pMsgBody->dRefCount);
		HSS_DELETEREF(pMsgBody->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&pMsgBody,(\
			Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pMsgBody->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFreeFreeMsgBody ");

	return Sdf_co_success;
	
}

/***********************************************************************
** Function: __sdf_fn_uaFreeCallInfo
** Description: Frees the Call Info structure
**
************************************************************************/
void __sdf_fn_uaFreeCallInfo 
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pCallInfo)
#else
(pCallInfo)
Sdf_ty_pvoid pCallInfo;
#endif
{
	(void)sdf_ivk_uaFreeCallInfo((Sdf_st_callInfo *)pCallInfo);
}


/***********************************************************************
** Function: sdf_ivk_uaFreeCallInfo
** Description: Frees the Call Info structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeCallInfo 
#ifdef ANSI_PROTO
(Sdf_st_callInfo *pCallInfo)
#else
(pCallInfo)
Sdf_st_callInfo *pCallInfo;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering sdf_ivk_uaFreeCallInfo");

	if(pCallInfo == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pCallInfo->dRefCount);
	HSS_DECREF(pCallInfo->dRefCount);

	if (HSS_CHECKREF(pCallInfo->dRefCount))
	{
		HSS_UNLOCKREF(pCallInfo->dRefCount);
		HSS_DELETEREF(pCallInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&pCallInfo,(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pCallInfo->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeCallInfo ");

	return Sdf_co_success;
}


/***********************************************************************
** Function: __sdf_fn_uaFreeCallObjectKey
** Description: Frees the CallObject Key structure
**
************************************************************************/
void __sdf_fn_uaFreeCallObjectKey
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pCallObjKey)
#else
(pCallObjKey)
Sdf_ty_pvoid pCallObjKey;
#endif
{
	(void)sdf_ivk_uaFreeCallObjectKey((Sdf_st_callObjectKey *)pCallObjKey);

}
/***********************************************************************
** Function: __sdf_fn_uaFreeCallObject
** Description: Frees the CallObject structure
**
************************************************************************/
void __sdf_fn_uaFreeCallObject
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pCallObj)
#else
(pCallObj)
Sdf_ty_pvoid pCallObj;
#endif
{
	(void)sdf_ivk_uaFreeCallObject((Sdf_st_callObject**)&pCallObj);

}



/***********************************************************************
** Function: sdf_ivk_uaFreeCallObjectKey
** Description: Frees the CallObject Key structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeCallObjectKey
#ifdef ANSI_PROTO
(Sdf_st_callObjectKey *pCallObjKey)
#else
(pCallObjKey)
(Sdf_st_callObjectKey *pCallObjKey)
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeCallObjectKey");

	if(pCallObjKey == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pCallObjKey->dRefCount);
	HSS_DECREF(pCallObjKey->dRefCount);

	if (HSS_CHECKREF(pCallObjKey->dRefCount))
	{
		sip_freeSipHeader(pCallObjKey->pLocalAddr);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCallObjKey->pLocalAddr),(Sdf_st_error*)Sdf_co_null);
		
		sip_freeSipHeader(pCallObjKey->pRemoteAddr);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCallObjKey->pRemoteAddr),(Sdf_st_error*)Sdf_co_null);
			
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCallObjKey->pCallId),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCallObjKey->pLocalViaBranch),(Sdf_st_error*)Sdf_co_null);
		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&(pCallObjKey->pRemoteViaBranch),(Sdf_st_error*)Sdf_co_null);

		HSS_UNLOCKREF(pCallObjKey->dRefCount);
		HSS_DELETEREF(pCallObjKey->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&pCallObjKey,(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pCallObjKey->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFreeCallObjectKey");

	return Sdf_co_success;
}

#ifdef SDF_LOOKUP
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaFreeDnsEventContext
**
** DESCRIPTION: This function frees the DNS Event Context structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeDnsEventContext (
    Sdf_st_dnsEventContext *pDnsEventContext)
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
               "sdf_ivk_uaFreeDnsEventContext");

	if (pDnsEventContext == Sdf_co_null)
    	return Sdf_co_success;
	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pDnsEventContext->dRefCount);
	HSS_DECREF(pDnsEventContext->dRefCount);

	if (HSS_CHECKREF(pDnsEventContext->dRefCount))
	{
		/*
		 * Initialize the member variables of the structure
		 */
		(Sdf_ty_void)sdf_ivk_uaFreeCallObject(&(pDnsEventContext->pCallObj));
		(Sdf_ty_void)sdf_ivk_uaFreeOverlapTransInfo(pDnsEventContext->pOverlappedTransaction);
		if (pDnsEventContext->pTransaction != Sdf_co_null)
		{
			(Sdf_ty_void)sdf_ivk_uaFreeTransaction(pDnsEventContext->pTransaction);	
		} 	

		/* NOTE: DO WE NEED TO FREE THE LIST?
		 * sdf_listDeleteAll(pDnsEventContext->pslRecordList, &dError);
		 * sdf_listFree(pDnsEventContext->pslRecordList, &dError);
		 */
		sdf_mc_init(pDnsEventContext->pslRecordList);
		(Sdf_ty_void)sdf_ivk_uaFreeEventContext (pDnsEventContext->pEventContext);
		
		(Sdf_ty_void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&pDnsEventContext,(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pDnsEventContext->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
                "sdf_ivk_uaFreeDnsEventContext");

	return Sdf_co_success;	
}
/***********************************************************************
** Function: __sdf_fn_uaDnsList
** Description: Frees the DNS List structure
**
************************************************************************/
void __sdf_fn_uaFreeDnsList (Sdf_ty_pvoid	pvDnsRecord)
{
	Sdf_st_DnsRecord	*pDnsRecord= (Sdf_st_DnsRecord *) pvDnsRecord;
	Sdf_st_error error;
	
	if (pDnsRecord == Sdf_co_null)
	{
		return ;
	}
	switch (pDnsRecord->dDnsRecordType)
	{
		case Sdf_en_SRVrecord:
			(Sdf_ty_void)sdf_listDeleteAll(&(((Sdf_st_dnsSrvRecord *)pDnsRecord->pValue)\
				->slIpRecords), &error); 
			(Sdf_ty_void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *) \
				&(((Sdf_st_dnsSrvRecord *) pDnsRecord->pValue)->pTarget), \
				(Sdf_st_error*)Sdf_co_null);	
			break;
		case Sdf_en_NAPTRrecord:
		case Sdf_en_unKnownAddr:
		case Sdf_en_A6record:
			/* Unknown DNS record is freed in the application scope */
			if (pGlbToolkitData->pFreeUnknownDnsList != Sdf_co_null)
			{
    			if (pGlbToolkitData->pFreeUnknownDnsList(pDnsRecord) == \
						Sdf_co_success)
					return;
			}
			break;
		default: ;
	}	
	if (pDnsRecord != Sdf_co_null)
	{
		(Sdf_ty_void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*) &pDnsRecord->pValue, \
			(Sdf_st_error*)Sdf_co_null);
		(Sdf_ty_void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*) &pDnsRecord,
			(Sdf_st_error*)Sdf_co_null);
	}
	return ;
}
#endif


/***********************************************************************
** Function: __sdf_fn_uaFreeOverlapTransInfo
** Description: Frees the OverlapTransInfo structure
**
************************************************************************/
void __sdf_fn_uaFreeOverlapTransInfo
#ifdef ANSI_PROTO
(Sdf_ty_pvoid pOverlapTransInfo)
#else
(pOverlapTransInfo)
Sdf_ty_pvoid pOverlapTransInfo;
#endif
{
	(void)sdf_ivk_uaFreeOverlapTransInfo ((Sdf_st_overlapTransInfo *)pOverlapTransInfo);
}


/***********************************************************************
** Function: sdf_ivk_uaFreeOverlapTransInfo
** Description: Frees the OverlapTransInfo structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeOverlapTransInfo
#ifdef ANSI_PROTO
(Sdf_st_overlapTransInfo *pOverlapTransInfo)
#else
(pOverlapTransInfo)
(Sdf_st_overlapTransInfo *pOverlapTransInfo)
#endif
{
	Sdf_st_error error;
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeOverlapTransInfo");

	if(pOverlapTransInfo == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pOverlapTransInfo->dRefCount);
	HSS_DECREF(pOverlapTransInfo->dRefCount);

	if (HSS_CHECKREF(pOverlapTransInfo->dRefCount))
	{
		(void)sdf_ivk_uaFreeTransportInfo(pOverlapTransInfo->pDestInfo);
		(void)sdf_listDeleteAll(&(pOverlapTransInfo->slProcessedHeaders), &error);
		(void)sdf_listDeleteAll(&(pOverlapTransInfo->slUnprocessedHeaders), &error);
		(void)sdf_listDeleteAll(&(pOverlapTransInfo->slMsgBodyList), &error);
		sip_freeSipMessage(pOverlapTransInfo->pLastSipMsg);
		sip_freeSipMessage(pOverlapTransInfo->pSipMsg);
#ifdef SDF_SUBSCRIPTION_LAYER
        sdf_fn_uaSLFreeSubsTxnData(pOverlapTransInfo->pSubsTxnData);
#endif        
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)\
			&(pOverlapTransInfo->pOverlapViaBranch), &error);

#ifdef SDF_TXN
		sip_freeSipTxnKey(pOverlapTransInfo->pRprTxnKey);
		sip_freeSipTxnKey(pOverlapTransInfo->pOLTxnKey);
#endif
		
		if((pOverlapTransInfo->dSocket.dProtocol != Sdf_en_protoUdp) && \
		   (pOverlapTransInfo->dSocket.dProtocol != Sdf_en_protoNone) && \
		   (pOverlapTransInfo->dSocket.dSockfd != Sdf_co_InvalidSocketFd))
			(void)sdf_fn_uaCloseSocket(&(pOverlapTransInfo->dSocket));
#ifdef SDF_LOOKUP
		(Sdf_ty_void)sdf_listDeleteAll (&(pOverlapTransInfo->slDnsRecords),&error);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroyMutex(&(pOverlapTransInfo->dTxnDnsStateMutex));
#endif
#endif
		HSS_UNLOCKREF(pOverlapTransInfo->dRefCount);
		HSS_DELETEREF(pOverlapTransInfo->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			&pOverlapTransInfo,(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pOverlapTransInfo->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFreeOverlapTransInfo");

	return Sdf_co_success;
}

/*********************************************************
** FUNCTION: sdf_ivk_uaFreeEventContext
**
** DESCRIPTION: This function frees the Sdf_st_eventContext
**	 structure
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeEventContext
#ifdef ANSI_PROTO
	(Sdf_st_eventContext *pEventContext)
#else
	(pEventContext)
	 Sdf_st_eventContext *pEventContext;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_ivk_uaFreeEventContext");

	if(pEventContext == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pEventContext->dRefCount);
	HSS_DECREF(pEventContext->dRefCount);

	if (HSS_CHECKREF(pEventContext->dRefCount))
	{
#ifdef SDF_TRACE
		Sdf_st_error dErr;

		sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces, \
		(Sdf_ty_s8bit *)"Issuing callback sdf_cbk_uaFreeEventContextData",\
			0, &dErr);
#endif
#ifdef SDF_USE_PREALLOCBUFFERS
#ifdef SDF_APPCONTROLLEDMEMPOOL
		(void)sdf_cbk_uaFreePreallocBuffers(&(pEventContext->pData), \
						pEventContext->dPreallocMsgBufIndex);	
#else
		if (pEventContext->dPreallocMsgBufIndex != 0)
		{
			/* Return the preallocated buffer to the pool */
/*w534*/(void)sdf_ivk_uaMempoolFreeBlk(&dGlbPreallocMsgBufferPool, \
				pEventContext->dPreallocMsgBufIndex);
		}
#endif
#endif
#ifdef SDF_SUBSCRIPTION_LAYER
        sdf_ivk_uaSLFreeSubsObject(pEventContext->pSubsObj);
        pEventContext->pSubsObj      = Sdf_co_null;
				pEventContext->dIsRestartTmr = Sdf_co_true;
#endif        
		(void)sdf_cbk_uaFreeEventContextData(&(pEventContext->pData));
		HSS_UNLOCKREF(pEventContext->dRefCount);
		HSS_DELETEREF(pEventContext->dRefCount);


		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)&(pEventContext),\
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pEventContext->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaFreeEventContext");

	return Sdf_co_success;
}

/***********************************************************************
 ** Function: 	__sdf_fn_uaFreeString
 ** Description: Frees the string passed as a void pointer.
 **
 ************************************************************************/
void __sdf_fn_uaFreeString
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid pString)
#else
	(pString)
	Sdf_ty_pvoid pString;
#endif
{
	if (pString == Sdf_co_null)
		return;
	(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&pString, \
		(Sdf_st_error*)Sdf_co_null);
	return;
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeTransaction
**
** DESCRIPTION: This function frees the transaction Object
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeTransaction 
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction)
#else
	(pTransaction)
	 Sdf_st_transaction *pTransaction;
#endif
{
	Sdf_st_error error;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_ivk_uaFreeTransaction");

	if(pTransaction == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pTransaction->dRefCount);
	HSS_DECREF(pTransaction->dRefCount);

	if (HSS_CHECKREF((pTransaction)->dRefCount))
	{
		(void)sdf_ivk_uaFreeTransportInfo(pTransaction->pDestInfo);
		sip_freeSipMessage(pTransaction->pLastSipMsg);
		sip_freeSipMessage(pTransaction->pSipMsg);
		(void)sdf_listDeleteAll(&(pTransaction->slProcessedHeaders), &error);
		(void)sdf_listDeleteAll(&(pTransaction->slUnprocessedHeaders), &error);
		(void)sdf_listDeleteAll(&(pTransaction->slMsgBodyList), &error);
#ifdef SDF_LOOKUP
		(Sdf_ty_void)sdf_listDeleteAll (&(pTransaction->slDnsRecords),&error);
#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroyMutex(&(pTransaction->dTxnDnsStateMutex));
#endif
#endif

#ifdef SDF_TXN
		sip_freeSipTxnKey(pTransaction->pTxnKey);
		sip_freeSipTxnKey(pTransaction->pInvTxnKey);
#endif

		if((pTransaction->dSocket.dProtocol != Sdf_en_protoUdp) && \
		   (pTransaction->dSocket.dProtocol != Sdf_en_protoNone) && \
		   (pTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd))
			(void)sdf_fn_uaCloseSocket(&(pTransaction->dSocket));

		HSS_UNLOCKREF((pTransaction)->dRefCount);
		HSS_DELETEREF((pTransaction)->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&pTransaction, \
			(Sdf_st_error*) Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pTransaction->dRefCount);
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Exiting sdf_ivk_uaFreeTransaction");
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaClearTransaction
 **
 ** DESCRIPTION: This function has to be invoked when a transaction that was
 **				 being started has to be aborted. It frees all the contents
 **				 of the UAC transaction object and unlocks the transaction 
 **				 object for use.
 **				 
 **				 Note: 	This function does not decrement the reference count 
 **				 ----	of the transaction object.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaClearTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObject, 
	 Sdf_st_overlapTransInfo *pOverlapTransInfo,
	 const Sdf_ty_s8bit *pMethod,
	 Sdf_ty_transactionType dType)
#else
	(pCallObject, pOverlapTransInfo, pMethod, dType)
	Sdf_st_callObject *pCallObject;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	const Sdf_ty_s8bit *pMethod;
	Sdf_ty_transactionType dType;
#endif
{
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	Sdf_st_error error;
	Sdf_ty_state dState;
	Sdf_ty_messageType dMsgType=Sdf_en_unknownRequest;
	Sdf_ty_retVal	dRetVal;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId dCallingThread = 0;

	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

	sdf_fn_debug((Sdf_ty_s8bit *) \
	 	"SDF_DEBUG - Entering sdf_ivk_uaClearTransaction");
	
/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, &error);

	if( (dMsgType != Sdf_en_invite) &&
		(dMsgType != Sdf_en_cancel) &&
		(dMsgType != Sdf_en_bye)	&&
		(dMsgType != Sdf_en_ack))
	{
		dRetVal = sdf_ivk_uaClearRegularTransaction(pCallObject, pMethod, \
				pOverlapTransInfo);
		return dRetVal;
	}

	if (dType == Sdf_en_uacTransaction)
		pTransaction = pCallObject->pUacTransaction;
	else
		pTransaction = pCallObject->pUasTransaction;

	if( dMsgType == Sdf_en_invite )
		pCallObject->pCallInfo->dInviteInProg = Sdf_co_false;
	/*
	 * Try to grab the lock before clearing the transaction.
	 */
#ifdef SDF_THREAD_SAFE		
	if (sdf_fn_uaTryLock(&(pCallObject->dCallObjectMutex), dCallingThread) \
		== Sdf_co_fail)
	{
		/* TryLock failed ==> some other thread is owning the mutex;
		** So wait for that thread to release it */
/*534*/(void)sdf_ivk_uaLockCallObject(pCallObject, &error);
	}
#endif		

	/*
	 * Clear the contents of the transaction now.
	 */
	sip_freeSipMessage(pTransaction->pSipMsg);
	sip_freeSipMessage(pTransaction->pLastSipMsg);
	sdf_mc_init(pTransaction->pSipMsg);
	sdf_mc_init(pTransaction->pLastSipMsg);
	(void)sdf_listDeleteAll(&(pTransaction->slProcessedHeaders), &error);
	(void)sdf_listDeleteAll(&(pTransaction->slUnprocessedHeaders), &error);
	(void)sdf_listDeleteAll(&(pTransaction->slMsgBodyList), &error);
	pTransaction->dTransactionState = Sdf_en_idle;
	/*Now check if Re-Invite is happening*/
	dState=pCallObject->pCallInfo->dState;

	if((dState == Sdf_en_reInviteSent) || (dState == Sdf_en_reInviteReceived)||\
	   (dState == Sdf_en_reInviteFinalResponseSent)		|| \
	   (dState == Sdf_en_reInviteFinalResponseReceived))
	{
		/*We will have to set the call state back to call established*/
		pCallObject->pCallInfo->dState=Sdf_en_callEstablished;
	}

	/*Media State should also be restored*/
	if((pCallObject->pMediaInfo->dMediaState == Sdf_en_offerSent)||\
		(pCallObject->pMediaInfo->dMediaState == Sdf_en_offerRecv))
	{
		pCallObject->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	}
	else if((pCallObject->pMediaInfo->dMediaState == Sdf_en_offerChangeRecv)||\
		(pCallObject->pMediaInfo->dMediaState == Sdf_en_offerChangeSent))
	{
		pCallObject->pMediaInfo->dMediaState = Sdf_en_establishedMedia;
	}

#ifdef SDF_TLS
	pCallObject->pCallInfo->dSecureCall = Sdf_co_false;
#endif
	
	/*
	 * Unlock the transaction now that the contents are cleared.
	 */
#ifdef SDF_THREAD_SAFE	
/*w534*/(void)sdf_ivk_uaUnlockCallObject(pCallObject, &error);
#endif			

	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaClearTransaction");
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaClearRegularTransaction
 **
 ** DESCRIPTION: This function has to be invoked when a Regular transaction 
 **				 that was being started has to be aborted. 
 **				 It removes the Overlap Transaction from CallObject and
 **				 unlocks the transaction object for use
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaClearRegularTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObject, 
	 const Sdf_ty_s8bit *pMethod,
	 Sdf_st_overlapTransInfo *pOverlapTransInfo) 
#else
	(pCallObject, pMethod, pOverlapTransInfo)
	Sdf_st_callObject *pCallObject;
	const Sdf_ty_s8bit *pMethod;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
#endif
{
#ifdef SDF_LINT
    Sdf_st_error	error = {Sdf_en_LastUatkError,Sdf_en_LastUatkError,
                            0,{'\0',}};
#else
    Sdf_st_error	error;
#endif    
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId dCallingThread = 0;
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
#endif

	(void)pMethod;
	(void)error;
	sdf_fn_debug((Sdf_ty_s8bit *) \
	 	"SDF_DEBUG - Entering sdf_ivk_uaClearRegularTransaction");
	
	/*
	 * Try to grab the lock before clearing the transaction.
	 */
#ifdef SDF_THREAD_SAFE		
	if (sdf_fn_uaTryLock(&(pCallObject->dCallObjectMutex), dCallingThread) \
		== Sdf_co_fail)
	{
		/* TryLock failed ==> some other thread is owning the mutex;
		** So wait for that thread to release it */
/*w534*/(void)sdf_ivk_uaLockCallObject(pCallObject, &error);
	}
#endif		
	
/*w534*/(void)sdf_ivk_uaRemoveOverlapInfo( pCallObject, pOverlapTransInfo);

#ifdef SDF_THREAD_SAFE	
/*w534*/(void)sdf_ivk_uaUnlockCallObject(pCallObject, &error);
#endif			
	sdf_fn_debug((Sdf_ty_s8bit *) \
		"SDF_DEBUG - Exiting sdf_ivk_uaClearRegularTransaction");
	return Sdf_co_success;
}


#ifdef SDF_APP_CONTROLLED_REMOTE_RETX

/******************************************************************
** Function:  sdf_ivk_uaFreeRemoteRetransTable
** Description: This function is available only when UATK is compiled with
**				SDF_APP_CONTROLLED_REMOTE_RETX. Application has to use this
**				to free remote retransmission entry hash table. 
**				
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeRemoteRetransTable
#ifdef ANSI_PROTO
(Sdf_st_hash *pRemoteRetransHash, Sdf_st_error *pErr)
#else
(pRemoteRetransHash, pErr)
 Sdf_st_hash *pRemoteRetransHash;
 Sdf_st_error *pErr;
#endif
{
	Sdf_st_hashIterator dIterator;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFreeRemoteRetransTable");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaFreeRemoteRetransTable ( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}

	if(pRemoteRetransHash == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFreeRemoteRetransTable (): "
			"Invalid param: pRemoteRetransHash",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	sdf_ivk_uaHashInitIterator(pRemoteRetransHash, &dIterator);

	while (dIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_remoteRetransBuffer *pBuffer = \
			(Sdf_st_remoteRetransBuffer *)(dIterator.pCurrentElement->pElement);
/*w534*/(void)sdf_ivk_uaHashRemove(pRemoteRetransHash, (Sdf_ty_pvoid) pBuffer->pKey);
		sdf_ivk_uaHashNext(pRemoteRetransHash, &dIterator);
	}

	if (sdf_ivk_uaHashFree (pRemoteRetransHash, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFreeRemoteRetransTable( ):"
			"Failed to initialize hash table for remote "
			"retransmission handling",pErr);
#endif
		return Sdf_co_fail;
	}

	return Sdf_co_success;
}
#endif

#ifdef SDF_TXN
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaFreeTxnContextData
 **
 ** DESCRIPTION: This is the free function for transaction context information 
 **				 that the UA Toolkit stores with the stack. Every time the 
 **				 stack issues a callback for a transaction, we get this 
 **				 context back from the stack.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeTxnContextData
#ifdef ANSI_PROTO
	(Sdf_st_txnContextData	*pTxnContextData)
#else
	(pTxnContextData)
	 Sdf_st_txnContextData	*pTxnContextData;
#endif
{
	Sdf_st_error	dError;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_ivk_uaFreeTxnContextData");

	if(pTxnContextData == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pTxnContextData->dRefCount);
	HSS_DECREF(pTxnContextData->dRefCount);

	if (HSS_CHECKREF(pTxnContextData->dRefCount))
	{
		(void)sdf_ivk_uaFreeCallObject( &(pTxnContextData->pRefCallObject) );
#ifdef SDF_SUBSCRIPTION_LAYER        
        sdf_ivk_uaSLFreeSubsObject(pTxnContextData->pSubsObj);
#endif
		/* TODO : Free application data here ? */

		 switch (pTxnContextData->dType)
		 {
			case Sdf_en_overlapTransaction :
			case Sdf_en_RegularUacTransaction:
			case Sdf_en_RegularUasTransaction:
/*w534*/		(void)sdf_ivk_uaFreeOverlapTransInfo( (Sdf_st_overlapTransInfo *) \
					(pTxnContextData->pTransaction) );
				break;
				
			case Sdf_en_uacTransaction:
			case Sdf_en_uasTransaction:
				 (void)sdf_ivk_uaFreeTransaction( (Sdf_st_transaction *) \
					(pTxnContextData->pTransaction) );
				 break;

			case Sdf_en_InvalidTransaction:
				/* do nothing */
				break;
		 } /* End of case-switch */

		if (pTxnContextData->pEventContext != Sdf_co_null)
			(Sdf_ty_void)sdf_ivk_uaFreeEventContext (pTxnContextData->pEventContext);

		HSS_UNLOCKREF(pTxnContextData->dRefCount);
		HSS_DELETEREF(pTxnContextData->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, \
			(Sdf_ty_pvoid *)&(pTxnContextData), &dError);
	}
	else
	{
		HSS_UNLOCKREF(pTxnContextData->dRefCount);
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Exiting sdf_ivk_uaFreeTxnContextData");

	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaFreeNoAnswerTimerContext
 **
 ** DESCRIPTION: This is the free function for transaction context information 
 **				 that the UA Toolkit stores with the stack. Every time the 
 **				 stack issues a callback for a transaction, we get this 
 **				 context back from the stack.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeNoAnswerTimerContext
#ifdef ANSI_PROTO
	(Sdf_st_noAnswerTimerContext	*pNoAnswerTimerContext)
#else
	(pNoAnswerTimerContext)
	 Sdf_st_noAnswerTimerContext	*pNoAnswerTimerContext;
#endif
{
	Sdf_st_error	dError;

	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Entering sdf_fn_uaFreeNoAnswerTimerContext");

	if (pNoAnswerTimerContext == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pNoAnswerTimerContext->dRefCount);
	HSS_DECREF(pNoAnswerTimerContext->dRefCount);

	if (HSS_CHECKREF(pNoAnswerTimerContext->dRefCount))
	{
		(void)sdf_ivk_uaFreeCallObject(&(pNoAnswerTimerContext->pRefCallObject));
		(void)sdf_ivk_uaFreeEventContext(pNoAnswerTimerContext->pEventContext);

		HSS_UNLOCKREF(pNoAnswerTimerContext->dRefCount);
		HSS_DELETEREF(pNoAnswerTimerContext->dRefCount);

		(void)sdf_memfree(Sdf_mc_freeMemId, \
			(Sdf_ty_pvoid *)&(pNoAnswerTimerContext), &dError);
	}
	else
	{
		HSS_UNLOCKREF(pNoAnswerTimerContext->dRefCount);
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*) \
		 "SDF_DEBUG - Exiting sdf_fn_uaFreeNoAnswerTimerContext");
	return Sdf_co_success;
}

#endif

/*****************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaRemoveOverlapInfo
 **
 ** DESCRIPTION: This API is used to remove the overlap info element from  
 ** 			 the list of overlap transactions maintained in call object.
 **
 ******************************************************************************/
Sdf_ty_retVal	sdf_ivk_uaRemoveOverlapInfo
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_overlapTransInfo *pOverlapTransInfo)
#else	
	(pCallObj, pOverlapTransInfo)
	Sdf_st_callObject 	*pCallObj;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
#endif
{
	Sdf_st_listIterator dListIterator;
	Sdf_ty_u32bit		dTxnId;
	Sdf_ty_u32bit		iterator = 0;
	Sdf_st_overlapTransInfo	*pTransInfo = Sdf_co_null;
	Sdf_st_error 		Error;

#ifdef SDF_PARAMVALIDATION
  if (pOverlapTransInfo == Sdf_co_null)
    return Sdf_co_success;
#endif

	dTxnId = pOverlapTransInfo->dTxnId;
	
	(void)sdf_listInitIterator(&(pCallObj->slOverlapTransInfo), \
		&dListIterator, &Error);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		pTransInfo = (Sdf_st_overlapTransInfo*)\
			(dListIterator.pCurrentElement->pData);

		if( dTxnId == pTransInfo->dTxnId )
		{
/*534*/(void)sdf_listDeleteAt( &(pCallObj->slOverlapTransInfo), \
					iterator, &Error);
			
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
				"Exiting sdf_ivk_uaRetrieveMatchingOverlapTransInfo");
			return Sdf_co_success;
		}
		(void)sdf_listNext(&dListIterator, &Error);
		iterator++;
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaRetrieveMatchingOverlapTransInfo");
	return Sdf_co_fail;
}
/*****************************************************************************
 ** FUNCTION: 	 __sdf_fn_uaFreeU32bit
 **
 ** DESCRIPTION: This API free's the allocated memory for U32bit 
 **
 ******************************************************************************/
void __sdf_fn_uaFreeU32bit
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	pU32bit)
#else	
	(pU32bit)
	Sdf_ty_pvoid	pU32bit;
#endif
{
	Sdf_st_error	dError;

	if(pU32bit == Sdf_co_null)
		return;
	(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid *)&pU32bit, &dError);
}


/*********************************************************
** FUNCTION: sdf_ivk_uaFreeDestDetails
**
** DESCRIPTION: This function frees the DestDetails structure
**
**
**********************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeDestDetails
#ifdef ANSI_PROTO
	(Sdf_st_destDetails *pDestDetails)
#else
	( pDestDetails )
	 Sdf_st_destDetails *pDestDetails;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering "\
			"sdf_ivk_uaFreeDestDetails");

	if(pDestDetails == Sdf_co_null)
		return Sdf_co_success;

	if (pDestDetails->dIsTargetUriSet == Sdf_co_true)
	    sip_freeSipAddrSpec(pDestDetails->u.pAddrSpec);
	else
	    (void)sdf_ivk_uaFreeTransportInfo(pDestDetails->u.pDestInfo);

	(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pDestDetails), \
			(Sdf_st_error*)Sdf_co_null);

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting "\
			"sdf_ivk_uaFreeDestDetails ");

	return Sdf_co_success;
}


#ifdef SDF_TLS

/***********************************************************************
** Function: sdf_ivk_uaFreeSslData
** Description: Frees the SslData structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSslData 
#ifdef ANSI_PROTO
(Sdf_st_sslData *pSslData)
#else
(pSslData)
Sdf_st_sslData *pSslData;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Entering sdf_ivk_uaFreeSslData");

	if(pSslData == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSslData->dRefCount);
	HSS_DECREF(pSslData->dRefCount);

	if (HSS_CHECKREF(pSslData->dRefCount))
	{
		HSS_UNLOCKREF(pSslData->dRefCount);
		HSS_DELETEREF(pSslData->dRefCount);

		/* Call the portlayer function to free the data stored in pSslData. */
		(void)sdf_fn_uaFreeSslData(&(pSslData->pData));
		(void)sdf_ivk_uaFreeSslParams(pSslData->pSslParams);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid*)\
			                 &pSslData,(Sdf_st_error*)Sdf_co_null);
	}
	else
	{
		HSS_UNLOCKREF(pSslData->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeSslData ");

	return Sdf_co_success;
}

/***********************************************************************
** Function: sdf_ivk_uaFreeSslParams
** Description: Frees the SslParams structure
**
************************************************************************/
Sdf_ty_retVal sdf_ivk_uaFreeSslParams 
(Sdf_st_sslParams *pSslParams)
{
	Sdf_st_error  dErr;

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - sdf_ivk_uaFreeSslParams.");

	if(pSslParams == Sdf_co_null)
		return Sdf_co_success;

	/*
	 * Decrement reference count. If reference count is zero, free the members
	 * of this structure
	 */
	HSS_LOCKREF(pSslParams->dRefCount);
	HSS_DECREF(pSslParams->dRefCount);

	if (HSS_CHECKREF(pSslParams->dRefCount))
	{
		HSS_UNLOCKREF(pSslParams->dRefCount);
		HSS_DELETEREF(pSslParams->dRefCount);

		if (pSslParams->pCertFile != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pCertFile), &dErr);

		if (pSslParams->pPrivateKeyFile != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pPrivateKeyFile), &dErr);

		if (pSslParams->pPrivateKeyPasswd != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pPrivateKeyPasswd), &dErr);

		if (pSslParams->pCACertDirPath != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pCACertDirPath), &dErr);

                /* CSR_1-5036521*/
                if (pSslParams->pCACertFile != Sdf_co_null)
                     (void)sdf_memfree(Sdf_mc_freeMemId, \
                           (Sdf_ty_pvoid *)&(pSslParams->pCACertFile), &dErr);
                /* CSR_1-5036521*/

		if (pSslParams->pCipherSuites != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pCipherSuites), &dErr);

		if (pSslParams->pDHParamFile != Sdf_co_null)
			(void)sdf_memfree(Sdf_mc_freeMemId, \
					(Sdf_ty_pvoid *)&(pSslParams->pDHParamFile), &dErr);

		(void)sdf_memfree(Sdf_mc_freeMemId,(Sdf_ty_pvoid *)&pSslParams,&dErr);
	}
	else
	{
		HSS_UNLOCKREF(pSslParams->dRefCount);
	}

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG - Exiting sdf_ivk_uaFreeSslParams");
	return Sdf_co_success;
}

#endif /* SDF_TLS */
