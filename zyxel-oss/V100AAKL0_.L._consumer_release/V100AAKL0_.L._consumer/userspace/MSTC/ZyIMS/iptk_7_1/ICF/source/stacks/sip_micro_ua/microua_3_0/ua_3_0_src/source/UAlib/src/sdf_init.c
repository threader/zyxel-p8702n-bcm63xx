/***********************************************************************
 ** FUNCTION:
 **			This file has the initialization functions for all structures
 **
 *********************************************************************
 **
 ** FILENAME		: sdf_init.c
 **
 ** DESCRIPTION		: This file has the initialization functions for 
 **						all structures
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		------
 ** 05/12/00	Anitha						Creation
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 **
 *********************************************************************
 **     COPYRIGHT , Aricent, 2006
 *********************************************************************/


#include "ALheader.h"
#include "sdf_portlayer.h"
#include "sdf_free.h" 
#include "sdf_sdpfree.h"
#include "sdf_init.h"
#include "sdf_sdpinit.h"
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_callbacks.h"
#include "sdf.h"
#include "sdf_hash.h" 
#include "sdf_txn.h" 
#include "sdf_accessor.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_sdpstruct.h"
#include "sdf_list.h" 
#include "sdf_mempool.h"
#include "sdf_sdpinit.h"
#include "sdf_sdpfree.h"
#endif

#ifdef SDF_STATISTICS
#include "sdf_statistics.h"
#endif
#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif
#ifdef SDF_HA
#include "sdf_haapiint.h"
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
#include "sdf_subslayerstatemach.h"
#include "sdf_subslayerutils.h"
#include "sdf_subslayerstatistics.h"

#endif

#ifdef SDF_DEP
#include "sdf_dep_eventMgr.h"
#endif
Sdf_ty_s8bit *hss_sdf_part_id = (Sdf_ty_s8bit *)SDF_PART_ID;

/*
 * The internal data structure of the UA Toolkit. It stores the hash table
 * for maintaining remote retransmission entries.
 */
Sdf_st_toolkitData *pGlbToolkitData = Sdf_co_null;

#ifndef SDF_NETWORKCALLBACKS
Sdf_st_fdSetInfo	dGlbFdSet;
#endif

#ifdef SDF_USE_PREALLOCBUFFERS
/* 
 * Memory pool for Preallocated Message buffers. These buffers will be used
 * for forming and sending the SIP message. This eliminates the need to
 * allocate a new buffer for every messages transmitted. 
 */
#ifndef SDF_APPCONTROLLEDMEMPOOL
Sdf_st_mempool dGlbPreallocMsgBufferPool;
#endif
#endif

#ifdef SDF_USE_INTERNALTHREADS
/* Retransmission thread spawned by the toolkit */
Sdf_ty_threadT dGlbRemoteRetransThread;
#endif

#ifdef SDF_THREAD_SAFE
/* Mutex for Thread-Safe printf funcion */
Sdf_ty_mutexIdT dGlbPrintMutex;
#endif


/******************************************************************
**
** FUNCTION:  sdf_getPart
**
** DESCRIPTION: This function returns the product id of the SIP UA
**				toolkit
**
******************************************************************/
Sdf_ty_s8bit *sdf_getPart
#ifdef ANSI_PROTO
	(void)
#else
	(void)
#endif
{
	return hss_sdf_part_id;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitNameInfo
**
** DESCRIPTION: This function inits the Name Info Structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitNameInfo
#ifdef ANSI_PROTO
(Sdf_st_nameInfo **ppNameInfo,Sdf_st_error *pErr)
#else
(ppNameInfo, pErr)
Sdf_st_nameInfo **ppNameInfo;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedUrlParams = Sdf_co_false;
	Sdf_ty_bool dInitedHeaderParams = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitNameInfo");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitNameInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppNameInfo = (Sdf_st_nameInfo *) sdf_memget (Sdf_mc_initMemId, \
		sizeof(Sdf_st_nameInfo), pErr);

	if (*ppNameInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitNameInfo( ):"
			"Failed to allocate memory for nameInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppNameInfo)->dRefCount);
	sdf_mc_init((*ppNameInfo)->pName);
	sdf_mc_init((*ppNameInfo)->pAddress);
	sdf_mc_init((*ppNameInfo)->pTag);
	sdf_mc_init((*ppNameInfo)->pScheme);
	(*ppNameInfo)->dPort = 0;

	if (dRetVal == Sdf_co_success)
	{
		if ((sdf_listInit(&((*ppNameInfo)->slUrlParams), \
			__sip_freeSipParam,Sdf_co_false,pErr)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitNameInfo( ):"
				"Failed to initalize Url Params list inside nameInfo",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedUrlParams = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ((sdf_listInit(&((*ppNameInfo)->slHeaderParams), \
			__sip_freeSipParam,Sdf_co_false,pErr)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitNameInfo( ):"
				"Failed to initalize Header Params list inside nameInfo",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedHeaderParams = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		if (dInitedUrlParams == Sdf_co_true)
			(void)sdf_listFree(&((*ppNameInfo)->slUrlParams), pErr);

		if (dInitedHeaderParams == Sdf_co_true)
			(void)sdf_listFree(&((*ppNameInfo)->slHeaderParams), pErr);
	
		HSS_DELETEREF((*ppNameInfo)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppNameInfo, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitNameInfo");	
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitTransportInfo
**
** DESCRIPTION:  This function allcates memory to the Transport Info
**	structure and initializes the member variables to NULL
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitTransportInfo
#ifdef ANSI_PROTO
(Sdf_st_transportInfo **ppTranspInfo,Sdf_st_error *pErr)
#else
(ppTranspInfo, pErr)
Sdf_st_transportInfo **ppTranspInfo;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitTransportInfo");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitTransportInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppTranspInfo = (Sdf_st_transportInfo *) sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_transportInfo), pErr);

	if (*ppTranspInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransportInfo( ): "
			"Failed to allocate memory for TranspInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppTranspInfo)->dRefCount);
	sdf_mc_init((*ppTranspInfo)->pIp);

	(*ppTranspInfo)->dScheme = Sdf_en_protoNone;
	(*ppTranspInfo)->dPort = 0;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitTransportInfo");
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitRemoteRetransKey
**
** DESCRIPTION: This function initializes the remote retransmission
**		key structure and initializes the member variables to NULL
**
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransKey
#ifdef ANSI_PROTO
(Sdf_st_remoteRetransKey **ppKey ,Sdf_st_error *pErr)
#else
(ppKey, pErr)
Sdf_st_remoteRetransKey **ppKey;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering"
		" sdf_ivk_uaInitRemoteRetransKey");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitRemoteRetransKey( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppKey = (Sdf_st_remoteRetransKey *)sdf_memget(Sdf_mc_initMemId,\
		sizeof( Sdf_st_remoteRetransKey), pErr);

	if(*ppKey == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransKey( ): "
			"Failed to allocate memory for remote retrans key structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/* 
	 * Set all pointer members to NULL before allocating memory for any of them
	 */
	sdf_mc_init((*ppKey)->pFromHeader);
	sdf_mc_init((*ppKey)->pToHeader);
	sdf_mc_init((*ppKey)->pCallId); 
	sdf_mc_init((*ppKey)->pMethod); 
	sdf_mc_init((*ppKey)->pViaBranch); 
	(*ppKey)->dMessageType = SipMessageAny;
	HSS_INITREF((*ppKey)->dRefCount);

	/*
	 * Initialize the member variables of the structure
	 */
	if (dRetVal == Sdf_co_success)
	{
		if(sip_initSipHeader(&((*ppKey)->pFromHeader), SipHdrTypeAny,\
			(SipError *)(&(pErr->stkErrCode)))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransKey( ): "
				"Failed to initialize From header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if(sip_initSipHeader(&((*ppKey)->pToHeader), SipHdrTypeAny,\
			(SipError *)(&(pErr->stkErrCode)))==SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransKey( ): "
				"Failed to initialize To header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		sip_freeSipHeader((*ppKey)->pFromHeader);
		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppKey)->pFromHeader), pErr);

		sip_freeSipHeader((*ppKey)->pToHeader);
		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppKey)->pToHeader), pErr);

		HSS_DELETEREF((*ppKey)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppKey, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaInitRemoteRetransKey");
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitRemoteRetransBuffer
**
** DESCRIPTION: This function initializes the remote retransmission
**		structure and initializes the member variables to NULL
**
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransBuffer
#ifdef ANSI_PROTO
(Sdf_st_remoteRetransBuffer **ppObject ,Sdf_st_error *pErr)
#else
(ppObject, pErr)
Sdf_st_remoteRetransBuffer **ppObject;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitRemoteRetransBuffer");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitRemoteRetransBuffer( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppObject = (Sdf_st_remoteRetransBuffer *)sdf_memget(Sdf_mc_initMemId,\
		sizeof( Sdf_st_remoteRetransBuffer), pErr);

	if(*ppObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransBuffer( ): "
			"Failed to allocate memory for "
			"Sdf_st_remoteRetransBuffer structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppObject)->dRefCount);
#ifdef SDF_USE_PREALLOCBUFFERS
	sdf_mc_init((*ppObject)->pContext); 
#else
	sdf_mc_init((*ppObject)->pBuffer); 
#endif
	(*ppObject)->dLength = 0; 
	sdf_mc_init((*ppObject)->pKey);
	(*ppObject)->dTimeStamp =0;
	sdf_mc_init((*ppObject)->pDestInfo);
	sdf_mc_init((*ppObject)->pAppData); 
	(*ppObject)->dSockfd= Sdf_co_InvalidSocketFd;

	if (dRetVal == Sdf_co_success)
	{
		if ((sdf_ivk_uaInitTransportInfo(&((*ppObject)->pDestInfo), \
			pErr)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransBuffer( ): "
				"Failed to init transport info",pErr);
#endif
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if((sdf_ivk_uaInitRemoteRetransKey(&((*ppObject)->pKey), pErr))\
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransBuffer(): "
				"Failed to init remote retrans key ",pErr);
#endif
			dRetVal = Sdf_co_fail;
		}
	}


	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		(void)sdf_fn_uaFreeRemoteRetransKey((*ppObject)->pKey);
		sdf_mc_init((*ppObject)->pKey);

		(void)sdf_ivk_uaFreeTransportInfo((*ppObject)->pDestInfo);
		sdf_mc_init((*ppObject)->pDestInfo);

		HSS_DELETEREF((*ppObject)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppObject, pErr);
		return Sdf_co_fail;
	}


	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitRemoteRetransBuffer");
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitCommonInfo
**
** DESCRIPTION:This function allocates memory for the commonInfo  
**	structure and initializes teh members to NULL.
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCommonInfo
#ifdef ANSI_PROTO
(Sdf_st_commonInfo **ppCommonInfo,Sdf_st_error *pErr)
#else
(ppCommonInfo, pErr)
Sdf_st_commonInfo **ppCommonInfo;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_bool dThreadSafe;
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedContactList = Sdf_co_false;
	Sdf_ty_bool dInitedRoute = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitCommonInfo");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitCommonInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppCommonInfo = (Sdf_st_commonInfo *)sdf_memget(Sdf_mc_initMemId,\
		sizeof(Sdf_st_commonInfo), pErr);

	if (*ppCommonInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitCommonInfo( ): "
			"Failed to allocate memory for CommonInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppCommonInfo)->dRefCount);
	sdf_mc_init((*ppCommonInfo)->pKey);
	sdf_mc_init((*ppCommonInfo)->pFrom);
	sdf_mc_init((*ppCommonInfo)->pTo);
	sdf_mc_init((*ppCommonInfo)->pRequestUri);
    
    /* Changes for CSR: 1-5210208 */
	sdf_mc_init((*ppCommonInfo)->pNewRequestUri);
	/* Changes for CSR: 1-5210208 */

	(*ppCommonInfo)->dPreloadedSet = Sdf_co_false;
	(*ppCommonInfo)->dSdpInInvite = Sdf_co_false;
	(*ppCommonInfo)->dRemoteCseq = 0;
	(*ppCommonInfo)->dLocalCseq = 1;
	sdf_mc_init((*ppCommonInfo)->pCallid);
	sdf_mc_init((*ppCommonInfo)->pChallangePasswd);

	(*ppCommonInfo)->dScheme = Sdf_en_protoNone;

	(*ppCommonInfo)->dCallerCancelStatus = Sdf_en_reset;

	(*ppCommonInfo)->dLocalRseq = 1;
	(*ppCommonInfo)->dRemoteRseq.dRseq = 0;
	(*ppCommonInfo)->dRemoteRseq.dCorrelationCseq = 0;
#ifdef SIP_RPR
	sdf_mc_init((*ppCommonInfo)->pRPRMethod);
#endif /* SIP_RPR */
	(*ppCommonInfo)->dLocalRegCseq = 1;
	(*ppCommonInfo)->dRemoteRegCseq = 0;
#ifdef SDF_TXN
	(*ppCommonInfo)->dIsTimeout = Sdf_co_false;
	(*ppCommonInfo)->pNoAnswerTimerHandle = Sdf_co_null;
#endif

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_ivk_uaInitCallObjectKey(&((*ppCommonInfo)->pKey), pErr) ==
			Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCommonInfo( ): "
				"Failed to initialise CallObjectKey",pErr);
#endif
			dRetVal = Sdf_co_fail;
		}	
	}


	if (dRetVal == Sdf_co_success)
	{
		if (sip_initSipHeader(&((*ppCommonInfo)->pFrom), SipHdrTypeFrom, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitCommonInfo( ): "
		 		"Failed to initialize From header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sip_initSipHeader(&((*ppCommonInfo)->pTo), SipHdrTypeTo, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitCommonInfo( ): "
		 		"Failed to initialize To header",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

#ifdef SDF_THREAD_SAFE
	dThreadSafe=Sdf_co_true;
#else
	dThreadSafe=Sdf_co_false;
#endif	

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppCommonInfo)->slContact), \
			__sdf_fn_uaFreeCompleteHeader,dThreadSafe,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCommonInfo( ):"
				"Failed to initalize list of Contacts",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedContactList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppCommonInfo)->slInvRoute), \
			__sdf_fn_uaFreeCompleteHeader,dThreadSafe,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCommonInfo( ):"
				"Failed to initalize list of Route",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedRoute = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		(void)sdf_ivk_uaFreeCallObjectKey((*ppCommonInfo)->pKey);
		sdf_mc_init((*ppCommonInfo)->pKey);

		sip_freeSipHeader((*ppCommonInfo)->pFrom);
		(void)sdf_memfree(Sdf_mc_initMemId, \
			(Sdf_ty_pvoid *)&((*ppCommonInfo)->pFrom), \
			(Sdf_st_error*)Sdf_co_null);

		sip_freeSipHeader((*ppCommonInfo)->pTo);
		(void)sdf_memfree(Sdf_mc_initMemId, \
			(Sdf_ty_pvoid *)&((*ppCommonInfo)->pTo), \
			(Sdf_st_error*)Sdf_co_null);

		if (dInitedContactList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppCommonInfo)->slContact), pErr);
		}
		
		if (dInitedRoute == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppCommonInfo)->slInvRoute), pErr);
		}

		HSS_DELETEREF((*ppCommonInfo)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppCommonInfo, pErr);
		return Sdf_co_fail;
	}


	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitCommonInfo");	
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitHeaderList
**
** DESCRIPTION:This function initializes the HeaderList structure 
**	and initalizes teh member variables to NULL
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitHeaderList
#ifdef ANSI_PROTO
(Sdf_st_headerList **ppHeaderList, 
en_HeaderType dType, 
Sdf_ty_s8bit *pCorrelation,
Sdf_st_error *pErr)
#else
(ppHeaderList, dType, pCorrelation, pErr)
Sdf_st_headerList	 **ppHeaderList;
en_HeaderType 		dType;
Sdf_ty_s8bit *pCorrelation;
Sdf_st_error  		*pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedHeadersList = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitHeaderList");
	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitHeaderList( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppHeaderList = (Sdf_st_headerList *) sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_headerList), pErr);

	if (*ppHeaderList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitHeaderList( ): "
			"Failed to allocate memory for HeaderList",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppHeaderList)->dRefCount);
	sdf_mc_init((*ppHeaderList)->pCorrelation);

	(*ppHeaderList)->dType = dType;
	(*ppHeaderList)->pCorrelation = Sdf_mc_strdupCallHandler(pCorrelation);
	
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppHeaderList)->slHeaders), \
			__sdf_fn_uaFreeCompleteHeader, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitHeaderList( ):"
				"Failed to initalize list of Headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedHeadersList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		(void)sdf_memfree(Sdf_mc_initMemId, \
			(Sdf_ty_pvoid*)&((*ppHeaderList)->pCorrelation),\
			(Sdf_st_error*)Sdf_co_null);

		if (dInitedHeadersList == Sdf_co_true)
			(void)sdf_listFree(&((*ppHeaderList)->slHeaders), pErr);

		HSS_DELETEREF((*ppHeaderList)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppHeaderList, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitHeaderList");
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitAppData
**
** DESCRIPTION: This function inits the AppData structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitAppData
#ifdef ANSI_PROTO
(Sdf_st_appData **ppAppData,Sdf_st_error *pErr)
#else
(ppAppData, pErr)
Sdf_st_appData **ppAppData;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitAppData");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitAppData( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppAppData = (Sdf_st_appData *) sdf_memget (Sdf_mc_initMemId, \
		sizeof(Sdf_st_appData), pErr);

	if (*ppAppData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitAppData( ):"
			"Failed to allocate memory for appData",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppAppData)->dRefCount);
	sdf_mc_init((*ppAppData)->pData);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitAppData");	
	return Sdf_co_success;
}

#ifdef SDF_LOOKUP
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitDnsSrvRecord
**
** DESCRIPTION: This function inits the DNS SRV record
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitDnsSrvRecord (Sdf_st_dnsSrvRecord \
		 **ppdnsSrvRecord, Sdf_st_error *pErr)
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitDnsSrvRecord");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitDnsSrvRecord( ): Invalid error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppdnsSrvRecord = (Sdf_st_dnsSrvRecord *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_dnsSrvRecord), pErr);

	if(*ppdnsSrvRecord == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
			"Failed to allocate memory for user profile",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	/*
	 * Initialize the member variables of the structure
	 */
	(*ppdnsSrvRecord)->dttl 	= 0;
	(*ppdnsSrvRecord)->dPriority 	= 0;
	(*ppdnsSrvRecord)->dWeight	= 0;
	(*ppdnsSrvRecord)->dPort	= 0;
	(*ppdnsSrvRecord)->protocol	= Sdf_en_protoNone;

	(*ppdnsSrvRecord)->pTarget  = Sdf_co_null;

	if (sdf_listInit(&((*ppdnsSrvRecord)->slIpRecords), \
			__sip_freeString,Sdf_co_false, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitDnsSrvRecord( ):" 
			"Failed to initalize list of DNS SRV IP list",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitDnsSrvRecord");	
	return Sdf_co_success;	
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitDnsEventContext
**
** DESCRIPTION: This function init the DNS Event Context record
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitDnsEventContext (
    Sdf_st_dnsEventContext **ppDnsEventContext, 
    Sdf_st_error *pErr)
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitDnsEventContext");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitDnsEventContext( ): Invalid error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppDnsEventContext = (Sdf_st_dnsEventContext *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_dnsEventContext), pErr);

	if(*ppDnsEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitDnsEventContext( ):"
			"Failed to allocate memory for DNS Event Context",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppDnsEventContext)->dRefCount);
	(*ppDnsEventContext)->pCallObj			= Sdf_co_null;
	(*ppDnsEventContext)->pTransaction		= Sdf_co_null;
	(*ppDnsEventContext)->pOverlappedTransaction	= Sdf_co_null;
	(*ppDnsEventContext)->pEventContext		= Sdf_co_null;
	(*ppDnsEventContext)->pAppData			= Sdf_co_null;
	(*ppDnsEventContext)->dUrlType			= Sdf_en_UnKnownUrl;
	sdf_mc_init((*ppDnsEventContext)->pslRecordList);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitDnsEventContext");	
	return Sdf_co_success;	
}
#endif 

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitInitData
**
** DESCRIPTION: This is the initialization for the Init Data structure
**				It allocates memory for the structure and initializes 
**				the member variables to NULL.
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitInitData
#ifdef ANSI_PROTO
(Sdf_st_initData **ppInitData,Sdf_st_error *pErr)
#else
(ppInitData,pErr)
Sdf_st_initData **ppInitData;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedContactList = Sdf_co_false;
	Sdf_ty_bool dInitedContTransportList = Sdf_co_false;
	Sdf_ty_bool dInitedPreLoadedRoute = Sdf_co_false;
	Sdf_ty_bool dInitedMsgBodyList = Sdf_co_false;
	Sdf_ty_bool dInitedExtraHeadersList = Sdf_co_false;
#ifdef SDF_IMS
  Sdf_ty_bool dInitedRespHeadersList = Sdf_co_false;
#endif
	Sdf_ty_bool dInitedSessionParam= Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitInitData");	
#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitInitData( ): Invalid error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppInitData = (Sdf_st_initData *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_initData), pErr);

	if(*ppInitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
			"Failed to allocate memory for user profile",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppInitData)->dRefCount);
	sdf_mc_init((*ppInitData)->pFrom);
	sdf_mc_init((*ppInitData)->pTag);
	sdf_mc_init((*ppInitData)->pRegistrar);
	sdf_mc_init((*ppInitData)->pTransport);
	sdf_mc_init((*ppInitData)->sdf_fp_uaCallIdGen);
	sdf_mc_init((*ppInitData)->pSessionParam);
#ifdef SDF_HA
	sdf_mc_init((*ppInitData)->pHaInfo);
#endif

#ifdef SDF_RPORT
	(*ppInitData)->dEnableRport = Sdf_co_false;
#endif

	if (dRetVal == Sdf_co_success)
	{
		if (sip_initSipHeader(&((*ppInitData)->pFrom), SipHdrTypeFrom, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitInitData( ): "
		 		"Failed to initialize From header in InitData",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	/* Now pTag contains the Constant part of the tag 
	 * this is got from port layer function */
	/*  GenerateGlobalTag function is used to generate the variable portion of
	 *  tag.*/

	if (sdf_fn_uaGenerateFixedTag(&((*ppInitData)->pTag)) == Sdf_co_fail)
	{
		dRetVal = Sdf_co_fail;
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slContact), \
			__sdf_fn_uaFreeCompleteHeader, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of Contacts",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedContactList = Sdf_co_true;
		}
	}
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slContactTransport), \
			__sdf_fn_uaFreeTransportInfo, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of Contact Transport.",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedContTransportList = Sdf_co_true;
		}
	}
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slPreLoadedRoute), \
			__sdf_fn_uaFreeCompleteHeader, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of PreLoadedRoute",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedPreLoadedRoute = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitTransportInfo(&((*ppInitData)->pRegistrar),pErr) ==\
			Sdf_co_fail )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initialise Registrar information",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitTransportInfo(&((*ppInitData)->pTransport),pErr) ==\
			Sdf_co_fail )
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initialise transport information ",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			dRetVal = Sdf_co_fail;
		}
	}


	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slMsgBodyList),__sip_freeSipMsgBody,\
			Sdf_co_false,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of message bodies",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedMsgBodyList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slExtraHeadersList), \
			__sdf_fn_uaFreeConfigExtraHeaders,Sdf_co_false,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of Extra headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedExtraHeadersList = Sdf_co_true;
		}
	}
#ifdef SDF_IMS
  if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppInitData)->slRespHeadersList), \
			__sdf_fn_uaFreeConfigExtraHeaders,Sdf_co_false,pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize list of Extra headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedRespHeadersList = Sdf_co_true;
		}
	}
#endif

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_ivk_uaInitSessionParam(&((*ppInitData)->pSessionParam),\
							pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initalize Session Param",pErr);
#endif
			pErr->errCode=Sdf_en_initDataAccessError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedSessionParam= Sdf_co_true;
		}
	}

	/*
	 * Set the sample Call-ID generator provided by the UA as the Call-ID
	 * generator function. This can be overriden by the application's own
	 * implementation invoking sdf_ivk_uaSetDefaultCallIdGenerator
	 */
	(*ppInitData)->sdf_fp_uaCallIdGen = sdf_ivk_uaCallIdGenerator;

#ifdef SDF_HA
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_fn_uaInitUserProfileHAInfo(&((*ppInitData)->pHaInfo), pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initialise HA Info structure ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
#endif /* SDF_HA */

#ifdef SDF_TLS
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_ivk_uaInitSslData(&((*ppInitData)->pSslData), pErr) == \
					Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_initDataAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitInitData( ):"
				"Failed to initialise SSL Data structure ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
#endif /* SDF_TLS */

	if (dRetVal == Sdf_co_fail)
	{
		/* clean up and return failure */

		sip_freeSipHeader((*ppInitData)->pFrom);
		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid*)\
			&((*ppInitData)->pFrom), pErr);
		
		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppInitData)->pTag), pErr);

		if (dInitedContactList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slContact), pErr);
		}
		if (dInitedContTransportList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slContactTransport), pErr);
		}
		if (dInitedPreLoadedRoute == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slPreLoadedRoute), pErr);
		}


		(void)sdf_ivk_uaFreeTransportInfo((*ppInitData)->pRegistrar);
		(void)sdf_ivk_uaFreeTransportInfo((*ppInitData)->pTransport);

		if (dInitedMsgBodyList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slMsgBodyList), pErr);
		}

		if (dInitedExtraHeadersList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slExtraHeadersList), pErr);
		}
#ifdef SDF_IMS
    if (dInitedRespHeadersList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppInitData)->slRespHeadersList), pErr);
		}
#endif
		if (dInitedSessionParam == Sdf_co_true)
		{
			(void)sdf_ivk_uaFreeSessionParam((*ppInitData)->pSessionParam);
		}
		(*ppInitData)->sdf_fp_uaCallIdGen = Sdf_co_null;

#ifdef SDF_HA
		(void)sdf_fn_uaFreeUserProfileHAInfo((*ppInitData)->pHaInfo, pErr);
#endif
		
		HSS_DELETEREF((*ppInitData)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppInitData, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitInitData");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitToolkit
**
** DESCRIPTION: This is the initialization for the toolkit and must
**				be invoked by the application before invoking any other
**				toolkit API.
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitToolkit
#ifdef ANSI_PROTO
(Sdf_ty_initToolkitParams *pInitParams, Sdf_st_error *pErr)
#else
(pInitParams, pErr)
Sdf_ty_initToolkitParams *pInitParams;
Sdf_st_error  *pErr;
#endif
{
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&dGlbPrintMutex, Sdf_co_false);
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitToolkit");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_initErrorStrings();
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitToolkit( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

#ifdef SDF_USE_PREALLOCBUFFERS
	/* 
	 * Initialize Preallocated Message Buffer Pool 
	 */
	
#ifdef SDF_APPCONTROLLEDMEMPOOL
	/* Initialise the preallocated buffers in application scope  */
/*w537*/(void)sdf_cbk_uaInitPreAllocatedBuffers(pErr);
#else
	if (sdf_ivk_uaMempoolInit(&dGlbPreallocMsgBufferPool, \
			pInitParams->dNumPreAllocMsgBuffers, pInitParams->dPreAllocBufSize,\
			Sdf_co_null, Sdf_co_null, Sdf_co_null, Sdf_co_null) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):" \
			"Failed to allocate memory for Preallocated Message Buffer pool", \
			pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
#endif
#endif
	/*
	 * Allocate memory for the structure
	 */
	pGlbToolkitData = (Sdf_st_toolkitData *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_toolkitData), pErr);

	if(pGlbToolkitData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Failed to allocate memory for toolkit data",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Set the trace and debug functions to be used for the toolkit
	 */ 
	
	pGlbToolkitData->pDebugFunc = pInitParams->pDebugFunc;
	pGlbToolkitData->pTraceFunc = pInitParams->pTraceFunc;

#ifdef SDF_LOOKUP
	pGlbToolkitData->pFillDestInfoFromUnknownDnsRec = 
		(pInitParams->dDnsParams).pFillDestInfoFromUnknownDnsRec;
	pGlbToolkitData->pFreeUnknownDnsList = 
		(pInitParams->dDnsParams).pFreeUnknownDnsList;
#endif
	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF(pGlbToolkitData->dRefCount);
	
#ifndef SDF_UASSF
	if(sip_initStack() == SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_sipstackInitError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"SIP Stack Initialization failed",pErr);
#endif
		pErr->errCode = Sdf_en_sipstackInitError;
		return Sdf_co_fail;
	}
#endif

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Core stack initialized.\n", 0, pErr);
#endif	

#ifndef SDF_TXN
	/*
	 * Initialize the remote retransmissions handling.
	 */
	if (sdf_ivk_uaInitRemoteRetransHandling(pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Failed to initialize hash table for remote "
			"retransmission handling",pErr);
#endif
		return Sdf_co_fail;
	}
#endif

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Remote retransmission list initialized\n", 0, pErr);
#endif	

	/* 
	 * Initialise the state transition tables
	 */
	(void)sdf_fn_uaInitCsmTable();

	/*Initialize the Media State Table*/
	(void)sdf_fn_uaInitMediaStateTable();
	
	/*Initialize the Media State Validation Table*/
	(void)sdf_fn_uaInitMediaStateValidationTable();
#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Call State Machine transition tables initialized\n", \
		0, pErr);
#endif	

	/*
	 * Set the state maintenance to call stateful. The application
	 * can change this using the sdf_ivk_uaSetStateMaintenance
	 */
	pGlbToolkitData->dStateMaintenance = Sdf_en_callStateful;

#ifdef SDF_TXN
	/*
	 * We shall set the data related to transaction layer configuration in
	 * the global Toolkit data now. 
	 *
	 * By default the UA Toolkit will provide the functionality for 
	 * maintaining the aggregation of transaction structures. If 
	 * explicit over-riding has been desired by the app, we register 
	 * the functions to invoke on him to fetch/release the stack 
	 * transaction structures.
	 *
	 * If the app desires UA Toolkit to handle the functionality, it should
	 * pass NULL for these 2 function pointers.
	 */
	pGlbToolkitData->pTxnFetchFunc = (pInitParams->dTxnParams).pTxnFetchFunc;
	pGlbToolkitData->pTxnReleaseFunc = \
		(pInitParams->dTxnParams).pTxnReleaseFunc;

	if (pGlbToolkitData->pTxnFetchFunc == Sdf_co_null)
	{
#ifdef SDF_LINT
        if (sdf_ivk_uaHashInit(&(pGlbToolkitData->dSipTxnHash), \
			sdf_fn_uaTxnHashCalc, sdf_fn_uaTxnKeyCompare, \
			Sdf_co_null, Sdf_co_null, Sdf_co_numTxnHashBuckets, \
			Sdf_co_numTxnHashMaxElements, pErr) == Sdf_co_fail)
#else        
        if (sdf_ivk_uaHashInit(&(pGlbToolkitData->dSipTxnHash), \
			&sdf_fn_uaTxnHashCalc, &sdf_fn_uaTxnKeyCompare, \
			Sdf_co_null, Sdf_co_null, Sdf_co_numTxnHashBuckets, \
			Sdf_co_numTxnHashMaxElements, pErr) == Sdf_co_fail)
#endif            
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ) : "
				"Initialising hash for transaction structures failed", pErr);
#endif
			return Sdf_co_fail;
		}
	}

	pGlbToolkitData->dNoAnswerTimerDuration = Sdf_co_noAnswerTimerDuration;
		
#endif /* End - ifdef SDF_TXN */

#ifdef SDF_SUBSCRIPTION_LAYER
	/*
	 * We shall set the data related to subscription layer configuration in
	 * the global Toolkit data now. 
	 *
	 * By default the UA Toolkit will provide the functionality for 
	 * maintaining the Watcher List aggregation of Subscription objects.
	 *
	 * If the Application desires UA Toolkit to handle the init & free of
	 * SubsObj, it should pass NULL for these 2 function pointers.
	 */

  pGlbToolkitData->fpSLInitSubsObjectFunc = 
                              (pInitParams->dSLParams).fpSLInitSubsObjectFunc;
  pGlbToolkitData->fpSLFreeSubsObjectFunc = 
                              (pInitParams->dSLParams).fpSLFreeSubsObjectFunc;

  /* Initialize the Subscription Layer State Machine */
  sdf_fn_uaSLInitCsmTable();

#ifdef SDF_STATISTICS
  /* Initialize the Subscription Layer Statistics*/
  sdf_ivk_uaSLInitStatistics();
#endif

#ifdef SDF_WATCHER_LIST
	/*
   * The Watcher aggregation is maintained by UA Toolkit. It is initialized
	 * here.
	 */

  if (sdf_ivk_uaHashInit(&(pGlbToolkitData->dSubscriptionHash), \
            sdf_fn_uaSLHashCalc, sdf_fn_uaSLKeyCompare,  
            __sdf_fn_uaSLFreeSubsObject, Sdf_co_null, Sdf_co_numSLHashBuckets,\
  	    		Sdf_co_numSLHashMaxElements, pErr) == Sdf_co_fail)
  {
#ifdef SDF_ERROR
  		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
  				(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ) : "
  				"Initialising hash for subscription structures failed", pErr);
#endif
  		return Sdf_co_fail;
  }
#endif /* End of SDF_WATCHER_LIST */

#endif /* End - ifdef SDF_SUBSCRIPTION_LAYER */

  #ifdef SDF_DEP
  pGlbToolkitData->fp_cbk_subcr_uaDepNewSubscribeRespRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepNewSubscribeRespRecvd;

	pGlbToolkitData->fp_cbk_subcr_uaDepRefreshSubscribeRespRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepRefreshSubscribeRespRecvd;
  
	pGlbToolkitData->fp_cbk_subcr_uaDepNotifyRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepNotifyRecvd;
  
	pGlbToolkitData->fp_cbk_subcr_uaDepForkedNotifyRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepForkedNotifyRecvd;
  
	pGlbToolkitData->fp_cbk_subcr_uaDepForkedSubs2XXRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepForkedSubs2XXRecvd;

   pGlbToolkitData->fp_cbk_subcr_uaDepUnSubscribeRespRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepUnSubscribeRespRecvd;

   pGlbToolkitData->fp_cbk_subcr_uaDepSubscriptionTerminated = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepSubscriptionTerminated;
	 
   pGlbToolkitData->fp_cbk_subcr_uaDepStateRequestNotifyRecvd = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepStateRequestNotifyRecvd;
	 
   pGlbToolkitData->fp_cbk_subcr_uaDepUnSupportedMediaNotifyReceived = 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepUnSupportedMediaNotifyReceived;
	
	pGlbToolkitData->fp_cbk_subcr_uaDepParseNotifyFailed =
	 (pInitParams->dDepParams).fp_cbk_subcr_uaDepParseNotifyFailed;

	pGlbToolkitData->fp_cbk_subcr_uaDepEmptyNotifyBodyRecvd = 
	  (pInitParams->dDepParams).fp_cbk_subcr_uaDepEmptyNotifyBodyRecvd;
	 /* Notifier end callbacks */

   pGlbToolkitData->fp_cbk_ntfr_uaDepNotifyRespRecvd = 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepNotifyRespRecvd;

   pGlbToolkitData->fp_cbk_ntfr_uaDepPollSubscribeRecvd = 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepPollSubscribeRecvd;

   pGlbToolkitData->fp_cbk_ntfr_uaDepNewSubscribeRecvd = 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepNewSubscribeRecvd;

   pGlbToolkitData->fp_cbk_ntfr_uaDepUnSubscribeRecvd = 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepUnSubscribeRecvd;
	 
   pGlbToolkitData->fp_cbk_ntfr_uaDepRefreshSubscribeRecvd = 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepRefreshSubscribeRecvd;

   pGlbToolkitData->fp_cbk_ntfr_uaDepSubsWaitingTimerExpiry= 
    (pInitParams->dDepParams).fp_cbk_ntfr_uaDepSubsWaitingTimerExpiry;

   pGlbToolkitData->fp_cbk_uaDepSubsRefreshTimerExpiry= 
    (pInitParams->dDepParams).fp_cbk_uaDepSubsRefreshTimerExpiry;

   pGlbToolkitData->fp_cbk_subcr_uaDepSubsRemovalTimerExpiry= 
    (pInitParams->dDepParams).fp_cbk_subcr_uaDepSubsRemovalTimerExpiry;

   pGlbToolkitData->fp_cbk_uaDepTxnTimeoutRecvd= 
    (pInitParams->dDepParams).fp_cbk_uaDepTxnTimeoutRecvd;



		/* Initialize the event manager */
		if(sdf_fn_uaDepInitEventMgr() == Sdf_co_fail)
		{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Failed to init DEP event Manager",pErr);
#endif
		return Sdf_co_fail;
	
		}
	 
#endif  /* For SDF_DEP */
	
	pGlbToolkitData->dStrictURICheck = pInitParams->dStrictURICheck;
	
	/* 
	 * Initialise the statistics
	 */
#ifdef SDF_STATISTICS
	if(Sdf_co_fail == sdf_ivk_uaInitStatistics(Sdf_en_call,pErr))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Initialising statistics Failed",pErr);
#endif
		return Sdf_co_fail;
	}
	
#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Statistics module initialized.\n", 0, pErr);
#endif	
	
#endif

#ifdef SDF_HA
	if (sdf_fn_uaInitHaModule(&pInitParams->dHaParams, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Initialising HA module Failed",pErr);
#endif
		return Sdf_co_fail;
	}
#endif

#ifndef SDF_TXN

#ifdef SDF_USE_INTERNALTHREADS
	/* 
	 * Launch the remote retransmission list spooler
	 */
    Sdf_mc_createRemoteRetransThread(&dGlbRemoteRetransThread, \
                                     sdf_fn_uaRemoteRetransManager, \
                                     pErr)
#else

#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,(Sdf_ty_s8bit*) \
		"Issuing callback sdf_cbk_uaStartTimer for purging expired remote"
		"remote retransmission entries.", 0, pErr);
#endif

	Sdf_mc_setSysClkRate

#ifndef SDF_APP_CONTROLLED_REMOTE_RETX
	/* 
	 * Issue the sdf_cbk_uaStartTimer to the application to start a timer
	 * for the remote retransmission spooling
	 */
 	if(sdf_cbk_uaStartTimer(Sdf_mc_remoteRetransSpoolTime*1000,\
 		Sdf_en_remoteRetransTimer, Sdf_co_null, Sdf_co_null, \
 		sdf_ivk_uaPurgeExpiredRemoteRetransEntries, Sdf_co_null, pErr) \
		== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callBackError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Failed to start timer for remote retransmission spooling",pErr);
#endif
		pErr->errCode=Sdf_en_callBackError;
		return Sdf_co_fail;
	}
#endif /* End - If not defined SDF_APP_CONTROLLED_REMOTE_RETX */
#endif /* End - If defined SDF_USE_INTERNALTHREADS */

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_detailedTraces, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"Remote Retransmission Manager started.\n", 0, pErr);
#endif
	
#endif /* End - If not defined SDF_TXN */

#ifndef SDF_UASSF
#ifdef SDF_TLS
    /* Initialize the SSL related parameters. */
	if (pInitParams->pEntropySource != Sdf_co_null)
	{
	    if (sdf_fn_uaInitSslData(pInitParams->pEntropySource) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
				"Initialising SSL data Failed",pErr);
#endif
			return Sdf_co_fail;
		}
	}
	else
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_EGDRandFileError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Initialising SSL data Failed",pErr);
#endif
		pErr->errCode=Sdf_en_EGDRandFileError;
		return Sdf_co_fail;
	}
#endif /* SDF_TLS */
#endif

	/* Initialize the call object id generator. */
	if (sdf_fn_uaInitCallObjectIdGenerator(pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitToolkit( ):"
			"Initialising the CallObject Id generator Failed",pErr);
#endif
		return Sdf_co_fail;
	}
	
#ifndef SDF_NETWORKCALLBACKS
	(void)sdf_fn_uaInitFdSetInfo(&dGlbFdSet);
#endif

#ifdef SDF_TRACE	
	sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_initTraces, \
		(Sdf_ty_s8bit *)"User Agent Toolkit Initialized.\n", 0, pErr);
#endif	

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitToolkit");	
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaInitCall
 **
 ** DESCRIPTION: This is the initialization for the call object structure
 ** 			 It allocates memory for the structure and initializes 
 **				 the member variables to NULL. 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCall
#ifdef ANSI_PROTO
(Sdf_st_callObject **ppCallObj,Sdf_st_error *pErr)
#else
(ppCallObj, pErr)
Sdf_st_callObject **ppCallObj;
Sdf_st_error  *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedOverlapTxnList = Sdf_co_false;
	Sdf_ty_bool dInitedPesistentHeadersList = Sdf_co_false;
	Sdf_ty_bool dInitedPesistentMsgBodyList = Sdf_co_false;

	Sdf_ty_bool dThreadSafe;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitCall");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - sdf_ivk_uaInitCall( ):"
			" Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppCallObj = (Sdf_st_callObject *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_callObject), pErr);

	if(*ppCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitCall( ):"
		 	"Failed to allocate memory for CallObject",pErr); 
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppCallObj)->dRefCount);

	/* Assign a unique Id to this CallObject. */
	(*ppCallObj)->dCallObjId = sdf_fn_uaGetUniqueCallObjectId();

	(*ppCallObj)->dCallObjectType = Sdf_en_NewCallObject;
	sdf_mc_init((*ppCallObj)->pCommonInfo);
	sdf_mc_init((*ppCallObj)->pCallInfo);
	sdf_mc_init((*ppCallObj)->pUacTransaction);
	sdf_mc_init((*ppCallObj)->pUasTransaction);
	sdf_mc_init((*ppCallObj)->pAppData);
	sdf_mc_init((*ppCallObj)->pInitData);
#ifdef SDF_DEP
	sdf_mc_init((*ppCallObj)->pDepDetails);
#endif
#ifdef SDF_SESSION_TIMER
	sdf_mc_init((*ppCallObj)->pSessionTimer);
#endif
#ifdef SDF_HA
	sdf_mc_init((*ppCallObj)->pHaInfo);
#endif
 sdf_mc_init((*ppCallObj)->pMediaInfo);
#ifdef SDF_THREAD_SAFE	
	dThreadSafe = Sdf_co_true;
#else
	dThreadSafe = Sdf_co_false;
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
  (*ppCallObj)->dDialogUsageCount = 0;

  if (sdf_listInit(&((*ppCallObj)->slSubsObjList), \
			__sdf_fn_uaSLFreeSubsObject, dThreadSafe, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initalize list of Subscription Objects", pErr);
#endif
		pErr->errCode = Sdf_en_headerManipulationError;
		dRetVal = Sdf_co_fail;
	}
#endif /* End of SubscriptionObject */

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitCommonInfo(&((*ppCallObj)->pCommonInfo), pErr) ==\
			Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise CommonInfo member ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
	
#ifdef SDF_SESSION_TIMER
	if (dRetVal == Sdf_co_success)
	{
		if(sdf_ivk_uaInitSessionTimerInfo(&((*ppCallObj)->pSessionTimer), \
			pErr)== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise SessionTimerInfo member ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
#endif	

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitTransaction(&((*ppCallObj)->pUacTransaction), \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise UacTransaction member",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitTransaction(&((*ppCallObj)->pUasTransaction), \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise UasTransaction member",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitCallInfo(&((*ppCallObj)->pCallInfo), pErr) \
			== Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise CallInfo member ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppCallObj)->slOverlapTransInfo), \
			__sdf_fn_uaFreeOverlapTransInfo,dThreadSafe,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initalize list of overlap transactions",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&((*ppCallObj)->dCallObjectMutex), Sdf_co_true);
	sdf_fn_uaInitMutex(&((*ppCallObj)->dCallStateMutex), Sdf_co_false);
#endif

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppCallObj)->slPersistentHeaders), \
			__sdf_fn_uaFreeHeaderList,dThreadSafe,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initalize list of persistent headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal =  Sdf_co_fail;
		}
		else
		{
			dInitedPesistentHeadersList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppCallObj)->slPersistentMsgBodyList), \
			__sdf_fn_uaFreeMsgBody,dThreadSafe, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, \
				Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initalize list of persistent message bodies",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedPesistentMsgBodyList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if ( sdf_ivk_uaInitAppData(&((*ppCallObj)->pAppData), pErr) ==\
			Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise AppData structure ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
	
#ifdef SDF_HA
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_fn_uaInitCallObjectHAInfo(&((*ppCallObj)->pHaInfo), \
					pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise HA Info structure ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
#endif
	if(dRetVal == Sdf_co_success)
	{
		if(sdf_ivk_uaInitMediaHandling(&((*ppCallObj)->pMediaInfo),\
					pErr)==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_callObjectAccessError, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitCall( ):"
				"Failed to initialise Media Handling structure ",pErr);
#endif
			pErr->errCode=Sdf_en_callObjectAccessError;
			dRetVal = Sdf_co_fail;
		}
	}
	
	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		(void)sdf_ivk_uaFreeCommonInfo((*ppCallObj)->pCommonInfo);
		(void)sdf_ivk_uaFreeCallInfo((*ppCallObj)->pCallInfo);

		if (dInitedOverlapTxnList == Sdf_co_true)
			(void)sdf_listFree(&((*ppCallObj)->slOverlapTransInfo), pErr);

		(void)sdf_ivk_uaFreeTransaction((*ppCallObj)->pUacTransaction);
		(void)sdf_ivk_uaFreeTransaction((*ppCallObj)->pUasTransaction);

#ifdef SDF_THREAD_SAFE
		sdf_fn_uaDestroyMutex(&((*ppCallObj)->dCallObjectMutex));
		sdf_fn_uaDestroyMutex(&((*ppCallObj)->dCallStateMutex));
#endif

		if (dInitedPesistentHeadersList == Sdf_co_true)
			(void)sdf_listFree(&((*ppCallObj)->slPersistentHeaders), pErr);

		if (dInitedPesistentMsgBodyList == Sdf_co_true)
			(void)sdf_listFree(&((*ppCallObj)->slPersistentMsgBodyList), pErr);

		(void)sdf_ivk_uaFreeAppData((*ppCallObj)->pAppData);
		(void)sdf_ivk_uaFreeInitData((*ppCallObj)->pInitData);

#ifdef SDF_SESSION_TIMER
		(void)sdf_ivk_uaFreeSessionTimerInfo((*ppCallObj)->pSessionTimer);
#endif	

#ifdef SDF_HA
		(void)sdf_fn_uaFreeCallObjectHAInfo((*ppCallObj)->pHaInfo, pErr);
#endif
		(void)sdf_ivk_uaFreeMediaHandling((*ppCallObj)->pMediaInfo);

		HSS_DELETEREF((*ppCallObj)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) ppCallObj, pErr);
		return Sdf_co_fail;
	}
#ifdef  SDF_LOOKUP
	(*ppCallObj)->dCallSendState= Sdf_en_invalidStateTransistion;
#endif
#ifdef SDF_PRIVACY
    /* Init the Boolean variable for Privacy */
//    (*ppCallObj)->bIsPrivacy = Sdf_co_false;
//eric.Chung:  change the bIsPrivacy  flag to support Privacy header");

	 (*ppCallObj)->bIsPrivacy = Sdf_co_true;
#endif
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitCall");	
	return Sdf_co_success;
}
	
#ifdef SDF_AUTHENTICATE
/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitAuthenticationParams
**
** DESCRIPTION: This function initializes the authenticationParams structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitAuthenticationParams
#ifdef ANSI_PROTO
(Sdf_st_authenticationParams **ppAuthenticationParams,Sdf_st_error *pErr)
#else
(ppAuthenticationParams, pErr)
Sdf_st_authenticationParams **ppAuthenticationParams;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitAuthenticationParams");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitAuthenticationParams( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppAuthenticationParams = (Sdf_st_authenticationParams *)\
		sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_authenticationParams), pErr);

	if(*ppAuthenticationParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitAuthenticationParams( ): "
		 	"Failed to allocate memory for authenticationParams",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppAuthenticationParams)->dRefCount);

	sdf_mc_init((*ppAuthenticationParams)->pScheme);
	sdf_mc_init((*ppAuthenticationParams)->pRealm);
	sdf_mc_init((*ppAuthenticationParams)->pDomain);
	sdf_mc_init((*ppAuthenticationParams)->pOpaque);
	sdf_mc_init((*ppAuthenticationParams)->pQop);
	sdf_mc_init((*ppAuthenticationParams)->pAlgorithm);
	sdf_mc_init((*ppAuthenticationParams)->pNonce);
	sdf_mc_init((*ppAuthenticationParams)->pStale);
	(*ppAuthenticationParams)->dHdrType = SipHdrTypeAny;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitAuthenticationParams");	
	return Sdf_co_success;
}


/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitUacAuthInfo
**
** DESCRIPTION: This function initializes the Sdf_st_uacAuthInfo structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitUacAuthInfo
#ifdef ANSI_PROTO
(Sdf_st_uacAuthInfo **ppUacAuthInfo,Sdf_st_error *pErr)
#else
(ppUacAuthInfo, pErr)
Sdf_st_uacAuthInfo **ppUacAuthInfo;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitUacAuthInfo");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitUacAuthInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppUacAuthInfo = (Sdf_st_uacAuthInfo *)\
		sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_uacAuthInfo), pErr);

	if(*ppUacAuthInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitUacAuthInfo( ): "
		 	"Failed to allocate memory for Sdf_st_uacAuthInfo",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppUacAuthInfo)->dRefCount);

	sdf_mc_init((*ppUacAuthInfo)->pUserName);
	sdf_mc_init((*ppUacAuthInfo)->pPassword);
	sdf_mc_init((*ppUacAuthInfo)->pQop);
	sdf_mc_init((*ppUacAuthInfo)->pCnonce);
	sdf_mc_init((*ppUacAuthInfo)->pNonceCount);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitUacAuthInfo");	
	return Sdf_co_success;
}

/******************************************************************
**
** FUNCTION:  sdf_ivk_uaInitUasAuthInfo
**
** DESCRIPTION: This function initializes the Sdf_st_uasAuthInfo structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitUasAuthInfo
#ifdef ANSI_PROTO
(Sdf_st_uasAuthInfo **ppUasAuthInfo,Sdf_st_error *pErr)
#else
(ppUasAuthInfo, pErr)
Sdf_st_uasAuthInfo **ppUasAuthInfo;
Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitUasAuthInfo");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitUasAuthInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppUasAuthInfo = (Sdf_st_uasAuthInfo *)\
		sdf_memget(Sdf_mc_initMemId, sizeof(Sdf_st_uasAuthInfo), pErr);

	if(*ppUasAuthInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitUasAuthInfo( ): "
		 	"Failed to allocate memory for Sdf_st_uasAuthInfo",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppUasAuthInfo)->dRefCount);

	sdf_mc_init((*ppUasAuthInfo)->pScheme);
	sdf_mc_init((*ppUasAuthInfo)->pUserName);
	sdf_mc_init((*ppUasAuthInfo)->pRealm);
	sdf_mc_init((*ppUasAuthInfo)->pAlgorithm);
	sdf_mc_init((*ppUasAuthInfo)->pNonce);
	sdf_mc_init((*ppUasAuthInfo)->pCnonce);
	sdf_mc_init((*ppUasAuthInfo)->pSecret);

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitUasAuthInfo");	
	return Sdf_co_success;
}
#endif /* SDF_AUTHENTICATE */


/******************************************************************
** FUNCTION:  	sdf_ivk_uaInitconfigExtraHeaders
**
** DESCRIPTION: This function initializes the configExtraHeaders
**				structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitConfigExtraHeaders
#ifdef ANSI_PROTO
 (Sdf_st_configExtraHeaders **ppConfHdrs, Sdf_st_error *pErr)
#else
(ppConfHdrs, pErr)
Sdf_st_configExtraHeaders **ppConfHdrs;
Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitConfigExtraHeaders");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitConfigExtraHeaders( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif
	
	/*
	 * Allocate memory for the structure
	 */
	*ppConfHdrs =(Sdf_st_configExtraHeaders *)sdf_memget(Sdf_mc_initMemId,  \
		        sizeof(Sdf_st_configExtraHeaders), pErr);


	if(*ppConfHdrs == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitConfigExtraHeaders( ): "
		 	"Failed to allocate memory for ExtraHeaders structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */

	sdf_mc_init((*ppConfHdrs)->pParam);
	sdf_mc_init((*ppConfHdrs)->pData);

	if(sip_initSipParam(&((*ppConfHdrs)->pParam), \
			(SipError*)&(pErr->stkErrCode)) == SipFail)
	{
		pErr->errCode = Sdf_en_headerManipulationError;
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitConfigExtraHeaders( ): "
		 	"Failed to initialize SipParam to hold list of values for header",\
			pErr);
#endif
		dRetVal = Sdf_co_fail;
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		sip_freeSipParam((*ppConfHdrs)->pParam);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) ppConfHdrs, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitConfigExtraHeaders");
	return Sdf_co_success;
}


/******************************************************************
** Function:  sdf_ivk_uaInitCallInfo
** Description: This function initializes the CallInfo structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCallInfo
#ifdef ANSI_PROTO
(Sdf_st_callInfo **ppCallInfo,Sdf_st_error *pErr)
#else
(ppCallInfo,pErr)
Sdf_st_callInfo **ppCallInfo;
Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitCallInfo");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitCallInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppCallInfo = (Sdf_st_callInfo *) sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_callInfo), pErr);

	if(*ppCallInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitCallInfo( ): "
		 	"Failed to allocate memory for CallInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}
	
	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppCallInfo)->dRefCount);
	(*ppCallInfo)->dState = Sdf_en_idle;	
	(*ppCallInfo)->dInviteInProg = Sdf_co_false;	

#ifdef SDF_TLS
	(*ppCallInfo)->dSecureCall = Sdf_co_false;	
#endif
#ifdef SDF_UASSF_ONLY
/*SPR-4629:If this flag is enbled only ssf specific changes done by subbaroa will come into picture*/
	(*ppCallInfo)->dContactAddrType.dProtocolScheme = SipAddrSipUri;
 	(*ppCallInfo)->dContactAddrType.dTransportScheme = Sdf_en_protoUdp;

#else
	
	(*ppCallInfo)->dContactAddrType = SipAddrSipUri;	
#endif
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitCallInfo");	
	return Sdf_co_success;
	
}


/******************************************************************
** Function:  sdf_ivk_uaInitMsgBody
** Description: This function initializes the Sdf_st_msgbody structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitMsgBody
#ifdef ANSI_PROTO
(Sdf_st_msgBody **ppMsgBody, Sdf_ty_s8bit *pCorrelation, Sdf_st_error *pErr)
#else
(ppMsgBody, pCorrelation, pErr)
Sdf_st_msgBody **ppMsgBody; 
Sdf_ty_s8bit *pCorrelation;
Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitMsgBody");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitMsgBody( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	*ppMsgBody = (Sdf_st_msgBody *)sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_msgBody), pErr);

	if(*ppMsgBody == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitMsgBody( ): "
		 	"Failed to allocate memory for Sdf_st_msgBody structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppMsgBody)->dRefCount);
	
	(*ppMsgBody)->pMsgBody = Sdf_co_null;
	(*ppMsgBody)->pCorrelation = Sdf_co_null;

	if (pCorrelation != Sdf_co_null)
	{
		(*ppMsgBody)->pCorrelation = (Sdf_ty_s8bit *)sdf_memget(\
			Sdf_mc_initMemId, Sdf_mc_strlen(pCorrelation)+1, pErr);
		if ((*ppMsgBody)->pCorrelation == Sdf_co_null)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitMsgBody( ): "
		 		"Failed to allocate memory for pCorrelation",pErr);
#endif
			pErr->errCode=Sdf_en_noMemoryError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			Sdf_mc_strcpy((*ppMsgBody)->pCorrelation, pCorrelation);
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppMsgBody)->pCorrelation), pErr);

		HSS_DELETEREF((*ppMsgBody)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) ppMsgBody, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitMsgBody");	
	return Sdf_co_success;
	
}

/******************************************************************
** Function:  sdf_ivk_uaInitCallObjectKey
** Description: This function initializes the Call Object Key structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitCallObjectKey
#ifdef ANSI_PROTO
( Sdf_st_callObjectKey **ppCallObjKey,Sdf_st_error *pErr)
#else
( ppCallObjKey,*pErr)
 Sdf_st_callObjectKey **ppCallObjKey;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitCallObjectKey");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitCallObjectKey( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	 *ppCallObjKey = (Sdf_st_callObjectKey *)sdf_memget(\
		Sdf_mc_initMemId, sizeof(Sdf_st_callObjectKey), pErr);

	if(*ppCallObjKey == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitCallObjectKey( ): "
		 	"Failed to allocate memory for Call Object Key",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppCallObjKey)->dRefCount);
	sdf_mc_init((*ppCallObjKey)->pLocalAddr);
	sdf_mc_init((*ppCallObjKey)->pRemoteAddr);
	sdf_mc_init((*ppCallObjKey)->pCallId);
	(*ppCallObjKey)->dCaller = Sdf_en_any;
	sdf_mc_init((*ppCallObjKey)->pLocalViaBranch);
	sdf_mc_init((*ppCallObjKey)->pRemoteViaBranch);

	if (dRetVal == Sdf_co_success)
	{
		if (sip_initSipHeader(&((*ppCallObjKey)->pLocalAddr), SipHdrTypeAny, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitCallObjectKey( ): "
		 		"Failed to initialize From header in Call Object key",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sip_initSipHeader(&((*ppCallObjKey)->pRemoteAddr), SipHdrTypeAny, \
			(SipError *)&(pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *) "sdf_ivk_uaInitCallObjectKey( ): "
		 		"Failed to initialize To header in Call Object key",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
	}

	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		sip_freeSipHeader((*ppCallObjKey)->pLocalAddr);
		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppCallObjKey)->pLocalAddr), pErr);
		
		sip_freeSipHeader((*ppCallObjKey)->pRemoteAddr);
		(void)sdf_memfree(Sdf_mc_initMemId,(Sdf_ty_pvoid*)\
			&((*ppCallObjKey)->pRemoteAddr), pErr);

		HSS_DELETEREF((*ppCallObjKey)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) ppCallObjKey, pErr);
		return Sdf_co_fail;
	}

	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitCallObjectKey");	
	return Sdf_co_success;
}

/******************************************************************
** Function:  sdf_ivk_uaInitOverlapTransInfo
** Description: This function initializes the OverlapTransInfo structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitOverlapTransInfo
#ifdef ANSI_PROTO
( Sdf_st_overlapTransInfo **ppOverlapTransInfo ,Sdf_st_error *pErr)
#else
( ppOverlapTransInfo ,pErr)
 Sdf_st_overlapTransInfo **ppOverlapTransInfo;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedProcessedHeaders = Sdf_co_false;
	Sdf_ty_bool dInitedUnprocessedHeaders = Sdf_co_false;
	Sdf_ty_bool dInitedMsgBodyList = Sdf_co_false;
#ifdef SDF_LOOKUP
	Sdf_ty_bool dInitedDnsList 	= Sdf_co_false;	
#endif
  
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitOverlapTransInfo");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitOverlapTransInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	 *ppOverlapTransInfo = (Sdf_st_overlapTransInfo *)sdf_memget(\
		Sdf_mc_initMemId, sizeof(Sdf_st_overlapTransInfo), pErr);

	if(*ppOverlapTransInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitOverlapTransInfo( ): "
		 	"Failed to allocate memory for OverlapTransInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppOverlapTransInfo)->dRefCount);
	(*ppOverlapTransInfo)->dState = Sdf_en_idle;
	(*ppOverlapTransInfo)->dTxnId = sdf_fn_uaGetUniqueTxnId();
	(*ppOverlapTransInfo)->dRseq = 0;
	(*ppOverlapTransInfo)->dRemoteCseq = 0;
	(*ppOverlapTransInfo)->dLocalCseq = 0;
	sdf_mc_init((*ppOverlapTransInfo)->pSipMsg);
	sdf_mc_init((*ppOverlapTransInfo)->pLastSipMsg);
	sdf_mc_init((*ppOverlapTransInfo)->pDestInfo);
#ifdef SDF_SUBSCRIPTION_LAYER    
	sdf_mc_init((*ppOverlapTransInfo)->pSubsTxnData);
#endif    
	(*ppOverlapTransInfo)->dSocket.dSockfd=Sdf_co_InvalidSocketFd;
#ifdef SDF_LOOKUP
	(*ppOverlapTransInfo)->dSocket.dProtocol=Sdf_en_protoAny;
	(*ppOverlapTransInfo)->dTxnDnsState = Sdf_en_Idle;
    (*ppOverlapTransInfo)->dDnsRecordType = Sdf_en_unKnownAddr;
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&((*ppOverlapTransInfo)->dTxnDnsStateMutex), Sdf_co_true);
#endif
#else
	(*ppOverlapTransInfo)->dSocket.dProtocol=Sdf_en_protoUdp;
#endif

#ifdef SDF_TXN
	sdf_mc_init((*ppOverlapTransInfo)->pRprTxnKey);
	sdf_mc_init((*ppOverlapTransInfo)->pOLTxnKey);
#endif
	sdf_mc_init((*ppOverlapTransInfo)->pOverlapViaBranch); 

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppOverlapTransInfo)->slProcessedHeaders), \
			__sdf_fn_uaFreeHeaderList, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitOverlapTransInfo( ):"
				"Failed to initalize list of Processed headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedProcessedHeaders = Sdf_co_true;
		}
	}
	
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppOverlapTransInfo)->slUnprocessedHeaders), \
			__sdf_fn_uaFreeHeaderList,Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitOverlapTransInfo( ):"
				"Failed to initalize list of Unprocessed headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedUnprocessedHeaders = Sdf_co_true;
		}
	}
	
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppOverlapTransInfo)->slMsgBodyList),\
			__sip_freeSipMsgBody, Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitOverlapTransInfo( ):"
				"Failed to initalize list of message bodies",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedMsgBodyList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_ivk_uaInitTransportInfo(&((*ppOverlapTransInfo)->pDestInfo), \
				pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitOverlapTransInfo( ): "
				"Failed to initialise pDestInfo TransportInfo structure",pErr);
#endif
			dRetVal = Sdf_co_fail;
		}	
	}

#ifdef SDF_LOOKUP
  if (dRetVal == Sdf_co_success)
  {
    if (sdf_listInit(&((*ppOverlapTransInfo)->slDnsRecords), \
        __sdf_fn_uaFreeDnsList,Sdf_co_false, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
      sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
                     (Sdf_ty_s8bit *)"sdf_ivk_uaInitOverlapTransInfo( ):"
                      "Failed to initalize list of DNS headers",pErr);
#endif
      pErr->errCode=Sdf_en_headerManipulationError;
      dRetVal = Sdf_co_fail;
    }
    else
    {
      dInitedDnsList = Sdf_co_true;
    }
  }
#endif /*SDF_LOOKUP*/	

	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		if (dInitedProcessedHeaders == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppOverlapTransInfo)->slProcessedHeaders), pErr);
		}

		if (dInitedUnprocessedHeaders == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppOverlapTransInfo)->slUnprocessedHeaders), pErr);
		}

		if (dInitedMsgBodyList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppOverlapTransInfo)->slMsgBodyList), pErr);
		}

		(void)sdf_ivk_uaFreeTransportInfo((*ppOverlapTransInfo)->pDestInfo);

#ifdef SDF_LOOKUP
    if (dInitedDnsList == Sdf_co_true)
    {
      (Sdf_ty_void) sdf_listFree(&((*ppOverlapTransInfo)->slDnsRecords), pErr);
    }
#endif   
		HSS_DELETEREF((*ppOverlapTransInfo)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) \
			ppOverlapTransInfo, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitOverlapTransInfo");	
	return Sdf_co_success;
}

/******************************************************************
** Function:  sdf_ivk_uaInitEventContext
** Description: This function initializes the Sdf_st_eventContext 
**				structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitEventContext
#ifdef ANSI_PROTO
(Sdf_st_eventContext **ppEventContext ,Sdf_st_error *pErr)
#else
(ppEventContext ,pErr)
 Sdf_st_eventContext **ppEventContext;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitEventContext");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitEventContexti( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	 *ppEventContext = (Sdf_st_eventContext *)sdf_memget(\
		Sdf_mc_initMemId, sizeof(Sdf_st_eventContext), pErr);

	if (*ppEventContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitEventContext( ): "
		 	"Failed to allocate memory for Sdf_st_eventContext structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppEventContext)->dRefCount);
	sdf_mc_init((*ppEventContext)->pList);
	sdf_mc_init((*ppEventContext)->pTranspAddr);
	sdf_mc_init((*ppEventContext)->pData);
#ifdef SDF_SUBSCRIPTION_LAYER    
	sdf_mc_init((*ppEventContext)->pSubsObj);
  /* Indicates that at the Notifier end, Subscription refresh timer will always
	 * be restarted whenever there is an 'expires' parameter in the NOTIFY being
	 * sent.
	 */
	(*ppEventContext)->dIsRestartTmr = Sdf_co_true;
#endif    
	(*ppEventContext)->dRemainingLength = 0;
	(*ppEventContext)->dNextMessageLength = 0;
	(*ppEventContext)->dOptions.dOption = 0;
#ifndef SDF_TXN
	(*ppEventContext)->dRetransT1 = 0;
	(*ppEventContext)->dRetransT2 = 0;
	(*ppEventContext)->dMaxRetransCount = 0;
	(*ppEventContext)->dMaxInviteRetransCount = 0;
#endif
#ifdef SDF_SESSION_TIMER
	(*ppEventContext)->dReinviteTimeout = Sdf_co_false;
#endif	

	(*ppEventContext)->dProtocol = Sdf_en_protoUdp;
	(*ppEventContext)->dPreallocMsgBufIndex		 = 0;
	(*ppEventContext)->dSdfDisabledChecks		 = 0;
	
#ifdef SDF_TXN
	(*ppEventContext)->dTimeoutValues.dT1		 = 0;
	(*ppEventContext)->dTimeoutValues.dT2		 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerB	 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerC	 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerCr	 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerD_T3 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerF_T3 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerH	 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerI_T4 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerJ_T3 = 0;
	(*ppEventContext)->dTimeoutValues.dTimerK_T4 = 0;
	(*ppEventContext)->dTimerOption				 = 0;
	(*ppEventContext)->dTimeoutCbkOption		 = SIP_ISSUE_CBKFOR_ALLTIMERS;
	(*ppEventContext)->dNoAnswerTimerDuration	 = 0;
#endif
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitEventContext");	
	return Sdf_co_success;
}


/******************************************************************
** Function:  sdf_ivk_uaInitTransaction
** Description: This function initializes a transaction object
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitTransaction
#ifdef ANSI_PROTO
(Sdf_st_transaction **ppTransaction, Sdf_st_error *pErr)
#else
(ppTransaction,*pErr)
 Sdf_st_transaction **ppTransaction;
 Sdf_st_error *pErr;
#endif
{
	Sdf_ty_retVal dRetVal = Sdf_co_success;
	Sdf_ty_bool dInitedProcessedHeaders = Sdf_co_false;
	Sdf_ty_bool dInitedUnprocessedHeaders = Sdf_co_false;
	Sdf_ty_bool dInitedMsgBodyList = Sdf_co_false;
#ifdef SDF_LOOKUP
	Sdf_ty_bool dInitedDnsList 	= Sdf_co_false;	
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitTransaction");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitTransaction( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	/*
	 * Allocate memory for the structure
	 */
	 *ppTransaction = (Sdf_st_transaction *)sdf_memget(Sdf_mc_initMemId, \
	 	sizeof(Sdf_st_transaction), pErr);

	if(*ppTransaction == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitTransaction( ): "
		 	"Failed to allocate memory for Call Object Key",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppTransaction)->dRefCount);
	(*ppTransaction)->dTransactionState = Sdf_en_idle;	
	sdf_mc_init((*ppTransaction)->pSipMsg);
	sdf_mc_init((*ppTransaction)->pLastSipMsg);
	sdf_mc_init((*ppTransaction)->pDestInfo);
#ifdef SDF_TXN
	sdf_mc_init((*ppTransaction)->pTxnKey);
	sdf_mc_init((*ppTransaction)->pInvTxnKey);
#endif
	(*ppTransaction)->dSocket.dSockfd=Sdf_co_InvalidSocketFd;
	(*ppTransaction)->dSocket.dProtocol=Sdf_en_protoUdp;
	(*ppTransaction)->dTxnId = sdf_fn_uaGetUniqueTxnId();
#ifdef SDF_LOOKUP
	(*ppTransaction)->dSocket.dProtocol=Sdf_en_protoAny;
	(*ppTransaction)->dTxnDnsState = Sdf_en_Idle;
	(*ppTransaction)->dDnsRecordType = Sdf_en_unKnownAddr;
#ifdef SDF_THREAD_SAFE
	sdf_fn_uaInitMutex(&((*ppTransaction)->dTxnDnsStateMutex), Sdf_co_true);
#endif
#else
	(*ppTransaction)->dSocket.dProtocol=Sdf_en_protoUdp;
#endif

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppTransaction)->slProcessedHeaders), \
			__sdf_fn_uaFreeHeaderList,Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction( ):"
				"Failed to initalize list of Processed headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedProcessedHeaders = Sdf_co_true;
		}
	}
	
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppTransaction)->slUnprocessedHeaders), \
			__sdf_fn_uaFreeHeaderList,Sdf_co_false, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction( ):"
				"Failed to initalize list of Unprocessed headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedUnprocessedHeaders = Sdf_co_true;
		}
	}
	
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppTransaction)->slMsgBodyList),\
			__sip_freeSipMsgBody,Sdf_co_false,pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction( ):"
				"Failed to initalize list of message bodies",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedMsgBodyList = Sdf_co_true;
		}
	}

	if (dRetVal == Sdf_co_success)
	{
		if (sdf_ivk_uaInitTransportInfo(&((*ppTransaction)->pDestInfo), \
			pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction( ): "
				"Failed to initialise pDestInfo TransportInfo structure",pErr);
#endif
			dRetVal = Sdf_co_fail;
		}	
	}	
#ifdef SDF_LOOKUP
	if (dRetVal == Sdf_co_success)
	{
		if (sdf_listInit(&((*ppTransaction)->slDnsRecords), \
			__sdf_fn_uaFreeDnsList,Sdf_co_false, pErr) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_headerManipulationError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction( ):"
				"Failed to initalize list of DNS headers",pErr);
#endif
			pErr->errCode=Sdf_en_headerManipulationError;
			dRetVal = Sdf_co_fail;
		}
		else
		{
			dInitedDnsList = Sdf_co_true;
		}
	}
#endif /*SDF_LOOKUP*/
	if (dRetVal == Sdf_co_fail)
	{
		/* cleanup and return failure */

		if (dInitedProcessedHeaders == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppTransaction)->slProcessedHeaders), pErr);
		}

		if (dInitedUnprocessedHeaders == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppTransaction)->slUnprocessedHeaders), pErr);
		}

		if (dInitedMsgBodyList == Sdf_co_true)
		{
			(void)sdf_listFree(&((*ppTransaction)->slMsgBodyList), pErr);
		}

		(void)sdf_ivk_uaFreeTransportInfo((*ppTransaction)->pDestInfo);

#ifdef SDF_LOOKUP
		if (dInitedDnsList == Sdf_co_true)
		{
			(Sdf_ty_void) sdf_listFree(&((*ppTransaction)->slDnsRecords), pErr);	
		}
#endif
		HSS_DELETEREF((*ppTransaction)->dRefCount);

		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *) ppTransaction, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaInitTransaction");	
	return Sdf_co_success;
}

#ifndef SDF_TXN

/******************************************************************
** Function:  sdf_ivk_uaInitRemoteRetransHandling
** Description: This function initializes a UATKs remote retrans tables.
**				If 'app controlled' mode is chosen while compiling, a 
**				callback is issued so that the application can initialize the
**				remote retransmission aggregations maintained by it. Otherwise
**				UATK's own remote retransmission aggregation is initialized.
**				'app controlled' mode is used mainly when the application has
**				several message processing threads and it intends to have 
**				thread-specific remote retransmission handlers. 
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransHandling
#ifdef ANSI_PROTO
(Sdf_st_error *pErr)
#else
(*pErr)
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitRemoteRetransHandling");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitRemoteRetransHandling( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX
	if (sdf_cbk_uaInitRemoteRetransTables(pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransHandling( ):"
			"Failed to initialize hash table for remote "
			"retransmission handling",pErr);
#endif
		return Sdf_co_fail;
	}
#else
	if (sdf_ivk_uaHashInit(&(pGlbToolkitData->dRemoteRetransHash), \
		sdf_fn_remoteRetransHash, sdf_fn_remoteRetransKeyCompare, \
		sdf_fn_remoteRetransElementFree, \
		sdf_fn_remoteRetransKeyFree,\
		Sdf_mc_remoteRetransHashMaxBuckets, \
		Sdf_mc_remoteRetransHashMaxElements, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransHandling( ):"
			"Failed to initialize hash table for remote "
			"retransmission handling",pErr);
#endif
		return Sdf_co_fail;
	}
#endif

	return Sdf_co_success;
}

#ifdef SDF_APP_CONTROLLED_REMOTE_RETX

/******************************************************************
** Function:  sdf_ivk_uaInitRemoteRetransTable
** Description: This function is available only when UATK is compiled with
**				SDF_APP_CONTROLLED_REMOTE_RETX. Application has to use this
**				to initialize remote retransmission entry hash table. 
**				
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitRemoteRetransTable
#ifdef ANSI_PROTO
(Sdf_st_hash *pRemoteRetransHash, Sdf_st_error *pErr)
#else
(pRemoteRetransHash, pErr)
 Sdf_st_hash *pRemoteRetransHash;
 Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitRemoteRetransTable");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitTransaction( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}

	if(pRemoteRetransHash == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitTransaction(): "
			"Invalid param: pRemoteRetransHash",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	if (sdf_ivk_uaHashInit(pRemoteRetransHash, \
		sdf_fn_remoteRetransHash, sdf_fn_remoteRetransKeyCompare, \
		sdf_fn_remoteRetransElementFree, \
		sdf_fn_remoteRetransKeyFree,\
		Sdf_mc_remoteRetransHashMaxBuckets, \
		Sdf_mc_remoteRetransHashMaxElements, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitRemoteRetransTable( ):"
			"Failed to initialize hash table for remote "
			"retransmission handling",pErr);
#endif
		return Sdf_co_fail;
	}

	return Sdf_co_success;
}
#endif

#endif /* End - If not defined SDF_TXN */


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaStartTransaction
 **
 ** DESCRIPTION: This is the initialization function to be invoked every time 
 **				 before making a new request. The function will return error
 **				 in case one transaction that was initiated from this end is 
 **				 already in progress.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaStartTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject,
	 Sdf_st_overlapTransInfo **ppOverlapTransInfo, 
	 const Sdf_ty_s8bit *pMethod, 
	 Sdf_st_error *pError)
#else
	(pObject, ppOverlapTransInfo, pMethod, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo **ppOverlapTransInfo; 
	const Sdf_ty_s8bit *pMethod;
	Sdf_st_error	  *pError;
#endif
{
	Sdf_ty_transactionType	dTransactionType = Sdf_en_InvalidTransaction;
	Sdf_ty_bool			dIsAllowed = Sdf_co_false;
	Sdf_ty_state		dState;
	Sdf_ty_messageType 	dMsgType = Sdf_en_unknownRequest;
	Sdf_st_msgInfo		dMessageInfo;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId 	dCallingThread = 0;
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaStartTransaction");	

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaStartTransaction( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
			"Invalid parameter passed to the function: call object.", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pMethod == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
			"Invalid parameter passed to the function: method.", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End of parameter validations */

	

/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pError);

	dMessageInfo.pMethod = Sdf_co_null;
	dMessageInfo.dReqRespType = SipMessageRequest;
	dMessageInfo.dMsgType = dMsgType;
	dMessageInfo.dResponse = 0;

	dTransactionType = sdf_fn_getTransactionType( Sdf_co_null, dMessageInfo, \
							Sdf_co_false, pError);

	if(dTransactionType == Sdf_en_InvalidTransaction)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
			"Cannot Start an Invalid Transaction by this function", pError);
#endif
		pError->errCode = Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
	if((dTransactionType == Sdf_en_RegularUacTransaction) ||
	   (dTransactionType == Sdf_en_overlapTransaction))
	{
		if(sdf_ivk_uaStartRegularTransaction(pObject, ppOverlapTransInfo, \
			pMethod, pError) == Sdf_co_fail)
		{
			return Sdf_co_fail;
		}
		return Sdf_co_success;
	}
	
	dState = pObject->pCallInfo->dState;

	dIsAllowed = sdf_fn_uaIsTransactionAllowed(pObject, dMsgType, dState);

	if( dIsAllowed == Sdf_co_false )
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
			"Cannot start a new transaction in this state.", pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
	
#ifdef SDF_THREAD_SAFE		
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
/*w534*/(void)sdf_fn_uaTryLock(&(pObject->dCallObjectMutex),dCallingThread);
/*w534*/(void)sdf_fn_uaTryLock(&(pObject->dCallStateMutex),dCallingThread);
#endif

	dState = pObject->pCallInfo->dState;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif

	dIsAllowed = sdf_fn_uaIsTransactionAllowed(pObject, dMsgType, dState);
	
#ifdef SDF_TXN
	if( pObject->pCommonInfo->dIsTimeout == Sdf_co_true )
		pObject->pCommonInfo->dIsTimeout = Sdf_co_false;
#endif


	if( dIsAllowed == Sdf_co_false )
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
			"Cannot start a new transaction in this state.", pError);
#endif
#ifdef SDF_THREAD_SAFE		
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif			
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

	if( dMsgType == Sdf_en_invite )
	{
		if( pObject->pCallInfo->dInviteInProg != Sdf_co_true )
			pObject->pCallInfo->dInviteInProg = Sdf_co_true;
		else
		{
			/* Already one invite request is in progress on this call obj */
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
				"Cannot Start Invite Txn when one is in process", pError);
#endif
#ifdef SDF_THREAD_SAFE		
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif			
			pError->errCode = Sdf_en_transactionFailure;
			return Sdf_co_fail;
		}
	}

	if (pObject->pUacTransaction == Sdf_co_null)
	{
		if (sdf_ivk_uaInitTransaction(&(pObject->pUacTransaction), \
			pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
				(Sdf_ty_s8bit*)"sdf_ivk_uaStartTransaction(): "
				"Failed to initialize transacation structure.", pError);
#endif
#ifdef SDF_THREAD_SAFE		
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif			
			pError->errCode=Sdf_en_memoryError;
			return Sdf_co_fail;
		}
	}

	/* 
	 * Delete existing contents (if any) of the transaction
	 * structure. Also reset state.
	 */
(void)sdf_listDeleteAll( \
		&(pObject->pUacTransaction->slProcessedHeaders),
		pError);
(void)sdf_listDeleteAll( \
		&(pObject->pUacTransaction->slUnprocessedHeaders),
		pError);
(void)sdf_listDeleteAll( \
		&(pObject->pUacTransaction->slMsgBodyList),
		pError);
	sip_freeSipMessage(pObject->pUacTransaction->pSipMsg);
	pObject->pUacTransaction->pSipMsg = Sdf_co_null;
	pObject->pUacTransaction->dTransactionState = Sdf_en_idle;

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG-Exiting sdf_ivk_uaStartTransaction");
	return Sdf_co_success;
}

/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaIsTransactionAllowed
 **
 ** DESCRIPTION: This function checks whether the requested transaction is
 **				 allowed to start or not
 **
 ******************************************************************************/

Sdf_ty_bool sdf_fn_uaIsTransactionAllowed
#ifdef ANSI_PROTO	
(Sdf_st_callObject *pObject, Sdf_ty_messageType dMsgType, Sdf_ty_state dState)
#else
(pObject, dMsgType, dState)
	Sdf_st_callObject *pObject;
	Sdf_ty_messageType dMsgType;
	Sdf_ty_state		dState;
#endif
{
	Sdf_ty_bool		dIsAllowed = Sdf_co_false;	

	(void)pObject;

	/* check if the current state
	 * permits a new UAC transaction to be started. If a new transaction can
	 * be started, then go ahead and initialize the transaction structure
	 * of the call object
	 *
	 * If the Method is Cancel and the UAC transaction state is not idle
	 * then allow to start the trasaction
	 * otherwise if the method is non-CANCEL and UAC transaction state is
	 * not idle then don't allow to start the transaction
	 */

	
	if( (dMsgType == Sdf_en_cancel) &&
		((dState == Sdf_en_provisionalRespReceived) ||
		 (dState == Sdf_en_reInviteprovRespReceived)))
		dIsAllowed = Sdf_co_true;
	
	if( (dMsgType == Sdf_en_bye) && 
		((dState == Sdf_en_provisionalRespReceived) || \
		 (dState == Sdf_en_callEstablished) || \
		 (dState == Sdf_en_reInviteprovRespReceived) || \
/*CSR 4566600 starts*/
         (dState == Sdf_en_finalSuccessReceived) || \
		 (dState== Sdf_en_reInviteFinalResponseReceived) || \
		 (dState == Sdf_en_reInviteprovRespSent)|| \
		 (dState== Sdf_en_reInviteFinalResponseSent)|| \
/*CSR 4566600 ends*/	
   		 (dState == Sdf_en_reInviteSent)||\
         (dState == Sdf_en_finalSuccessReceived)))

		dIsAllowed = Sdf_co_true;

	if( (dMsgType == Sdf_en_invite) &&
		((dState == Sdf_en_idle) ||
		 (dState == Sdf_en_callEstablished)))
		dIsAllowed = Sdf_co_true;
	
#ifdef SDF_TXN
	if( pObject->pCommonInfo->dIsTimeout == Sdf_co_true )
	{
		/*  On timeout of of reInvite Final Response Sent reset the state 
		 *  to Call Established */
		if( (dState == Sdf_en_reInviteFinalResponseSent) || \
			(dState == Sdf_en_reInviteprovRespReceived) ||	\
			(dState == Sdf_en_reInviteprovRespSent))
		{
			pObject->pCallInfo->dState = Sdf_en_callEstablished;
		}

		dIsAllowed = Sdf_co_true;
	}
#endif

	return dIsAllowed;
}

#ifdef SDF_TXN
/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaInitTxnContextData
 **
 ** DESCRIPTION: This is the initialization function for transaction context
 **				 information that the UA Toolkit stores with the stack. Every
 **				 time the stack issues a callback for a transaction, we get
 **				 this context back from the stack.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitTxnContextData 
#ifdef ANSI_PROTO
	(Sdf_st_txnContextData **ppTxnContextData, Sdf_st_error *pError)
#else
	(ppTxnContextData, pError)
	Sdf_st_txnContextData	**ppTxnContextData;
	Sdf_st_error			*pError;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
				 "sdf_ivk_uaInitTxnContextData");	

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitTxnContextData( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif
	
	*ppTxnContextData = Sdf_co_null;

	/*
	 * Allocate memory for the structure
	 */
	*ppTxnContextData = (Sdf_st_txnContextData *) sdf_memget \
		(Sdf_mc_initMemId, sizeof(Sdf_st_txnContextData), pError);

	if (*ppTxnContextData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitTxnContextData( ):"
			"Failed to allocate memory for transaction context structure", \
			pError);
#endif
		pError->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppTxnContextData)->dRefCount);
	sdf_mc_init((*ppTxnContextData)->pApplicationData);
#ifdef SDF_SUBSCRIPTION_LAYER
	sdf_mc_init((*ppTxnContextData)->pSubsObj);
#endif    
	sdf_mc_init((*ppTxnContextData)->pRefCallObject);
	sdf_mc_init((*ppTxnContextData)->pEventContext);
	sdf_mc_init((*ppTxnContextData)->pTransaction);
	
	(*ppTxnContextData)->dType = Sdf_en_InvalidTransaction;
	 
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting "
				 "sdf_ivk_uaInitTxnContextData");	
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaInitNoAnswerTimerContext
 **
 ** DESCRIPTION: This is the initialization function for context information
 **				 that the UA Toolkit stores with the application while 
 **				 starting a timer waiting for a final response (after getting
 **				 a provisional response).
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitNoAnswerTimerContext
#ifdef ANSI_PROTO
	(Sdf_st_noAnswerTimerContext **ppNoAnswerTimerContext, Sdf_st_error *pErr)
#else
	(ppNoAnswerTimerContext, pErr)
	Sdf_st_noAnswerTimerContext	**ppNoAnswerTimerContext;
	Sdf_st_error				*pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering "
		"sdf_fn_uaInitNoAnswerTimerContext");	

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_fn_uaInitNoAnswerTimerContext( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	*ppNoAnswerTimerContext = Sdf_co_null;

	/*
	 * Allocate memory for the structure
	 */
	*ppNoAnswerTimerContext = (Sdf_st_noAnswerTimerContext *) sdf_memget \
		(Sdf_mc_initMemId, sizeof(Sdf_st_noAnswerTimerContext), pErr);

	if (*ppNoAnswerTimerContext == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaInitNoAnswerTimerContext( ):"
			"Failed to allocate memory for \"no-answer\" timer "
			"context structure", pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */
	HSS_INITREF((*ppNoAnswerTimerContext)->dRefCount);
	sdf_mc_init((*ppNoAnswerTimerContext)->pRefCallObject);
	sdf_mc_init((*ppNoAnswerTimerContext)->pEventContext);
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exitting "
		"sdf_fn_uaInitNoAnswerTimerContext");	
	return Sdf_co_success;
}

#endif /* End of ifdef SDF_TXN */

/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaStartRegularTransaction
 **
 ** DESCRIPTION: This is the initialization function to be invoked every time 
 **				 before making a new request. The function will return error
 **				 in case one transaction that was initiated from this end is 
 **				 already in progress.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaStartRegularTransaction
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pObject, Sdf_st_overlapTransInfo **ppOverlapTransInfo, \
	 const Sdf_ty_s8bit *pMethod, Sdf_st_error *pError)
#else
	(pObject, ppOverlapTransInfo, pMethod, pError)
	Sdf_st_callObject *pObject;
	Sdf_st_overlapTransInfo **ppOverlapTransInfo;
	const Sdf_ty_s8bit *pMethod;
	Sdf_st_error	  *pError;
#endif
{
	Sdf_ty_state dState = Sdf_en_idle;
	Sdf_ty_messageType dMsgType=Sdf_en_unknownRequest;
#ifdef SDF_THREAD_SAFE
	Sdf_ty_threadId 	dCallingThread = 0;
#endif
	Sdf_ty_bool			dIsCheckRequired = Sdf_co_false;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaStartRegularTransaction");	

#ifdef SDF_PARAMVALIDATION
	if(pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaStartRegularTransaction( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
	if (pObject == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "
			"Invalid parameter passed to the function: call object.", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pMethod == Sdf_co_null)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "
			"Invalid parameter passed to the function: method.", pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End of parameter validations */

/*w534*/(void)sdf_fn_uaGetMethodNameAsEnum(pMethod, &dMsgType, pError);

	if( (dMsgType == Sdf_en_invite) ||
		(dMsgType == Sdf_en_cancel) ||
		(dMsgType == Sdf_en_bye)	||
		(dMsgType == Sdf_en_ack))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "
			"Cannot Start the Normal Transaction by this function" \
			"Call sdf_ivk_uaStartTransaction", pError);
#endif
		pError->errCode = Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
	

	dState = pObject->pCallInfo->dState;

	if( (dState == Sdf_en_byeReceived)  ||
		(dState == Sdf_en_byeSent) ||
		(dState == Sdf_en_callTerminated))
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Main Call Stete indicates that call is going to terminated", \
			pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

	if(dState == Sdf_en_inviteSent)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Wait for Provisional or Final Response to sent Regular Txn", \
			pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

	/* If method is REGISTER/MESSAGE/REFER then check if any other 
	 * REGISTER/MESSAGE/REFER are ongoing simultaneously. */

	if (dMsgType == Sdf_en_register)
		dIsCheckRequired = Sdf_co_true;
#ifdef SDF_REFER 
	else if (dMsgType == Sdf_en_refer)
		dIsCheckRequired = Sdf_co_true;
#endif
#ifdef SDF_IM
	else if (dMsgType == Sdf_en_message)
		dIsCheckRequired = Sdf_co_true;
#endif

	if((dIsCheckRequired == Sdf_co_true) && \
		(pObject->pCommonInfo->dIsTimeout == Sdf_co_false))
	{
		Sdf_st_overlapTransInfo		*pTmpOverlapTxnInfo = Sdf_co_null;

/*w534*/(void)sdf_fn_uaGetMatchingOverlapTransaction(pObject, &pTmpOverlapTxnInfo, \
			  dMsgType, pError);
		
		if(pTmpOverlapTxnInfo != Sdf_co_null)
		{	
			if(pTmpOverlapTxnInfo->dState != Sdf_en_idle)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_majorErrors, \
					Sdf_en_regularTxnPendingError, \
					(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction:Cannot"\
					"Start simultaneous Update/Register Transactions", pError);
#endif
				(void)sdf_ivk_uaFreeOverlapTransInfo(pTmpOverlapTxnInfo);
				pError->errCode = Sdf_en_regularTxnPendingError;
				return Sdf_co_fail;
			}
			(void)sdf_ivk_uaFreeOverlapTransInfo(pTmpOverlapTxnInfo);
		}	
	}

#ifdef SDF_THREAD_SAFE
	dCallingThread = sdf_fn_uaGetCallingThreadId(Sdf_co_null);
/*w534*/(void)sdf_fn_uaTryLock(&(pObject->dCallObjectMutex),dCallingThread);
/*w534*/(void)sdf_fn_uaTryLock(&(pObject->dCallStateMutex),dCallingThread);
#endif

	dState = pObject->pCallInfo->dState;

#ifdef SDF_THREAD_SAFE
	sdf_fn_uaUnlockMutex(0, &(pObject->dCallStateMutex));
#endif

	if( (dState == Sdf_en_byeReceived)  ||
		(dState == Sdf_en_byeSent) ||
		(dState == Sdf_en_callTerminated))
	{
#ifdef SDF_THREAD_SAFE				
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Main Call Stete indicates that call is going to terminated", \
			pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

	if(dState == Sdf_en_inviteSent)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Wait for Provisional or Final Response to sent Regular Txn", \
			pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
	
	if (sdf_ivk_uaInitOverlapTransInfo(ppOverlapTransInfo, pError) \
		== Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE				
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Cannot Create New Overlap Info Structure", pError);
#endif
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

#ifdef SDF_LINT
  if ((ppOverlapTransInfo == Sdf_co_null) ||  \
      (*ppOverlapTransInfo) == Sdf_co_null)
	{
		pError->errCode = Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}
#endif
		
	(*ppOverlapTransInfo)->dMsgType = dMsgType;

	/* Append the new overlapTransInfo to the callObject's list	 */
	if (sdf_listAppend(&(pObject->slOverlapTransInfo), \
		*ppOverlapTransInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_THREAD_SAFE				
/*w537*/(void)sdf_ivk_uaUnlockCallObject(pObject, pError);
#endif
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_transactionFailure, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaStartRegularTransaction(): "     \
			"Cannot Create New Overlap Info to CallObject", pError);
#endif
		(void)sdf_ivk_uaFreeOverlapTransInfo(*ppOverlapTransInfo);
		pError->errCode=Sdf_en_transactionFailure;
		return Sdf_co_fail;
	}

	pError->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -" \
		"Exiting sdf_ivk_uaStartRegularTransaction");	
	return Sdf_co_success;
}

/******************************************************************************
** FUNCTION: sdf_fn_uaGetUniqueTxnId
**
** DESCRIPTION: This is an internal function used by 
** 				sdf_ivk_uaInitOverlapTransInfo to generate a unique overlap 
**				transaction Id (within the scope of a call object) each time 
**				a overlap transaction structure is initialized.
**
** NOTE: This function stores the last assigned Overlap transaction Id in a
**		 static variable. Each time this funciton is invoked, this variable is
**		 incremented and the value is returned. Even though this function may
**		 be invoked from several different threads, this static variable is not
**		 protected by mutex locks because, it is assumed that call object level
**		 mutex protects this variable also. Since the call object can be owned
**		 by only one thread at a time, there cannot be any simultaneous 
**		 invocation of this function within the context of the a call.
**
*******************************************************************************/
Sdf_ty_u32bit  sdf_fn_uaGetUniqueTxnId
#ifdef ANSI_PROTO	
(void)
#else
()
#endif
{
	static Sdf_ty_u32bit dGlbSdfLastAllocatedOverlapTxnId = 1;
	Sdf_ty_u32bit	dAssignedOverlapTxnId;
#ifdef SDF_TRACE
	Sdf_ty_s8bit	dTraceStr[Sdf_co_smallBufferSize];
	Sdf_st_error	dError;
#endif

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Entering sdf_fn_uaGetUniqueTxnId");

	dAssignedOverlapTxnId = dGlbSdfLastAllocatedOverlapTxnId;

	dGlbSdfLastAllocatedOverlapTxnId++;
	if (dGlbSdfLastAllocatedOverlapTxnId == 0)
		dGlbSdfLastAllocatedOverlapTxnId = 1;

#ifdef SDF_TRACE
	(void)sdf_fn_uaSprintf(dTraceStr, \
			"Allocated Transaction Id = 0x%0x", dAssignedOverlapTxnId);
	sdf_fn_trace(Sdf_en_allTraceLevels, Sdf_en_haTraces, dTraceStr, 0, &dError);
#endif
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
			"Exiting sdf_fn_uaGetUniqueTxnId");

	return dAssignedOverlapTxnId;
}

#ifdef SDF_TLS

/******************************************************************
** Function:  sdf_ivk_uaInitSslData
** Description: This function initializes the SslData structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSslData
#ifdef ANSI_PROTO
(Sdf_st_sslData **ppSslData, Sdf_st_error *pErr)
#else
(ppSslData, pErr)
Sdf_st_sslData **ppSslData;
Sdf_st_error   *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitSslData");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSslData( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif /* Param validation ends. */

	/*
	 * Allocate memory for the structure
	 */
	*ppSslData = (Sdf_st_sslData *) sdf_memget(Sdf_mc_initMemId, \
		sizeof(Sdf_st_sslData), pErr);

	if(*ppSslData == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitSslData( ): "
		 	"Failed to allocate memory for SslData structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	HSS_INITREF((*ppSslData)->dRefCount);
	(*ppSslData)->pData = Sdf_co_null;

	if (sdf_ivk_uaInitSslParams(&((*ppSslData)->pSslParams), pErr) \
			== Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitSslData( ): "
		 	"Initializing SSL params failed.",pErr);
#endif
		(void)sdf_memfree(Sdf_mc_initMemId, (Sdf_ty_pvoid *)ppSslData, pErr);
		return Sdf_co_fail;
	}

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitSslData");	
	return Sdf_co_success;
}

/******************************************************************
** Function:  sdf_ivk_uaInitSslParams
** Description: This function initializes the SslParam structure
**
******************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSslParams
(Sdf_st_sslParams **ppSslParams, Sdf_st_error *pErr)
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Entering sdf_ivk_uaInitSslParams");	

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSslParams( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif /* Param validation ends. */

	/*
	 * Allocate memory for the structure
	 */
	*ppSslParams = (Sdf_st_sslParams *)sdf_memget(Sdf_mc_initMemId, \
					sizeof(Sdf_st_sslParams), pErr);

	if(*ppSslParams == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *) "sdf_ivk_uaInitSslParams( ): "
		 	"Failed to allocate memory for SslParam structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}

	HSS_INITREF((*ppSslParams)->dRefCount);
    sdf_mc_init((*ppSslParams)->pCertFile);
	(*ppSslParams)->dCertFormat = Sdf_en_fileFormatInvalid;
    sdf_mc_init((*ppSslParams)->pPrivateKeyFile);
    sdf_mc_init((*ppSslParams)->pPrivateKeyPasswd);
    sdf_mc_init((*ppSslParams)->pCACertDirPath);
    /* CSR_1-5036521 */
    sdf_mc_init((*ppSslParams)->pCACertFile);
    /* CSR_1-5036521*/
    sdf_mc_init((*ppSslParams)->pCipherSuites);
    sdf_mc_init((*ppSslParams)->pDHParamFile);
	(*ppSslParams)->dDHParamFileFormat = Sdf_en_fileFormatInvalid;
	(*ppSslParams)->dCertVerifyMode = 0;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - Exiting sdf_ivk_uaInitSslParams");
	return Sdf_co_success;
}

#endif /* SDF_TLS */



