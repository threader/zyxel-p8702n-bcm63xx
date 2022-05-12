/******************************************************************************
 ** FUNCTION:
 **	 			This file has all the trace related functions 
 ******************************************************************************
 ** FILENAME:  		sdf_trace.c
 **
 ** DESCRIPTION:	All functions to set trace levels, print traces 
 **					are defined in this file.
 **
 ** DATE				NAME			REFERENCE		REASON
 ** ----				----			--------		------
 ** 17/03/01			Rajasri							Creation
 **
 ******************************************************************************
 **				COPYRIGHT , Aricent, 2006
 ******************************************************************************/
#include "ALheader.h"
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_portlayer.h"
#include "sdf_trace.h"
#include "sdf.h"

#ifdef SDF_TRACE 

Sdf_ty_uaTraceLevel sdf_sc_dTraceLevel;
Sdf_ty_traceType  sdf_sc_dTraceType;

/*******************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetTraceLevel
 **
 ** DESCRIPTION	: This functions is used to set the trace level
 **				  for the UA Toolkit
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetTraceLevel
#ifdef ANSI_PROTO
	(Sdf_ty_uaTraceLevel dTraceLevel,
	Sdf_st_error *pErr)
#else
	(dTraceLevel,pErr)
	Sdf_ty_uaTraceLevel dTraceLevel;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_TRACE
	Sdf_ty_uaTraceLevel dLevel;
	dLevel = dTraceLevel;
	pErr->errCode = Sdf_en_TraceDisabled;
	return Sdf_co_fail;
#else
	sdf_sc_dTraceLevel = dTraceLevel;
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
#endif
}


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaGetTraceLevel
 **
 ** DESCRIPTION	: This functions is used to get the trace level
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTraceLevel
#ifdef ANSI_PROTO
	(Sdf_ty_uaTraceLevel *pTraceLevel,
	Sdf_st_error *pErr)
#else
	(pTraceLevel,pErr)
	Sdf_ty_uaTraceLevel *pTraceLevel;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_TRACE
	Sdf_ty_uaTraceLevel *pDummyLevel = Sdf_co_null;
	pDummyLevel = pTraceLevel;
	pErr->errCode = Sdf_en_TraceDisabled;
	return Sdf_co_fail;
#else
	pErr->errCode = Sdf_en_noUaError;
	*pTraceLevel = sdf_sc_dTraceLevel;
	return Sdf_co_success;
#endif
}


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaSetTraceType
 **
 ** DESCRIPTION	: This functions is used to set the trace type required.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetTraceType
#ifdef ANSI_PROTO
	(Sdf_ty_traceType dTraceType,
	Sdf_st_error *pErr)
#else
	(dTraceType,pErr)
	Sdf_ty_traceType dTraceType;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_TRACE
	Sdf_ty_traceType dTemp;
	dTemp = dTraceType; 
	pErr->errCode = Sdf_en_TraceDisabled;
	return Sdf_co_fail;
#else
	sdf_sc_dTraceType = dTraceType;
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
#endif
}


/******************************************************************************
 ** FUNCTION	: sdf_ivk_uaGetTraceType
 **
 ** DESCRIPTION	: This functions is used to get the current trace type 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetTraceType
#ifdef ANSI_PROTO
	(Sdf_ty_traceType *pTraceType,
	Sdf_st_error *pErr)
#else
	(pTraceType,pErr)
	Sdf_ty_traceType *pTraceType;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_TRACE
	*pTraceType = Sdf_en_noTraces;
	pErr->errCode = Sdf_en_TraceDisabled;
	return Sdf_co_fail;
#else
	*pTraceType = sdf_sc_dTraceType;
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
#endif
}


/******************************************************************************
 ** FUNCTION	: sdf_fn_trace
 **
 ** DESCRIPTION	: For tracing the path of execution. This prints trace level 
 **				  comments on using the SDF_TRACE flag as a compile time 
 **				  option
 **
 ******************************************************************************/
void sdf_fn_trace
#ifdef ANSI_PROTO
	(Sdf_ty_uaTraceLevel dLevel,
	Sdf_ty_traceType dType,
	const Sdf_ty_s8bit *pStr,
	Sdf_ty_u32bit dLen,
	Sdf_st_error *pErr)
#else
	(dLevel,dType,pStr,dLen,pErr)
	Sdf_ty_uaTraceLevel dLevel;
	Sdf_ty_traceType dType;
	Sdf_ty_s8bit *pStr;
	Sdf_ty_u32bit dLen;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_TRACE
	Sdf_ty_uaTraceLevel dDummyLevel;
	Sdf_ty_traceType dDummyType;
	Sdf_ty_s8bit *pTempStr = Sdf_co_null;
	Sdf_ty_u32bit dTempLen=0;
	dDummyLevel = dLevel;
	dDummyType = dType;
	pTempStr = (Sdf_ty_s8bit *) pStr;
	dTempLen = dLen;
	pErr->errCode = Sdf_en_TraceDisabled;
	return;
#else

	/*
	 * UA Toolkit is compiled with trace.
	 */

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_trace( ): Invalid parameter passed to function");
#endif
		return;
	}	
	if (pStr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_trace( ): Invalid parameter passed to function");
#endif
		pErr->errCode = Sdf_en_invalidParamError;
		return;
	}
#endif	/* End of param validation ifdef */

	/*
	 * Check if the user has specified the trace function
	 * to be implemented
	 */
	if (pGlbToolkitData != Sdf_co_null)
	{	
		if (pGlbToolkitData->pTraceFunc != Sdf_co_null)
		{
    		pGlbToolkitData->pTraceFunc(dLevel,dType,pStr,dLen,pErr);
			return;
		}
	}
	
	if (sdf_sc_dTraceLevel & dLevel)
	{
		if (sdf_sc_dTraceType & dType)
		{
            Sdf_mc_displayFnTrace(pStr,dLen,pErr);

			pErr->errCode = Sdf_en_noUaError;
			return;
		}
	}
#endif
}	


/******************************************************************************
 ** FUNCTION	: sdf_fn_getStateName
 **
 ** DESCRIPTION	: This function returns the CSM state as a string
 ** 			  Used to print out traces of type 'Sdf_en_callStateTraces'
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_getStateName
#ifdef ANSI_PROTO
	(Sdf_ty_state dState,
	Sdf_ty_s8bit *pState,
	Sdf_ty_u32bit dLen,
	Sdf_st_error *pErr)
#else
	(dState,pState,dLen,pErr)
	Sdf_ty_state dState;
	Sdf_ty_s8bit *pState;
	Sdf_ty_u32bit dLen;
	Sdf_st_error *pErr;
#endif
{
#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
		return Sdf_co_fail;

	if (pState == Sdf_co_null)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_invalidParamError,\
			(Sdf_ty_s8bit *)"sdf_fn_getStateName( ): "
			"Invalid parameter pState",pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch(dState)
	{
		case Sdf_en_invalidStateTransition:
			Sdf_mc_strncpy(pState,"Sdf_en_invalidStateTransition",dLen);
			break;
		case Sdf_en_idle:
			Sdf_mc_strncpy(pState,"Sdf_en_idle",dLen);
			break;
		case Sdf_en_inviteReceived: 
			Sdf_mc_strncpy(pState,"Sdf_en_inviteReceived",dLen);
			break;
		case Sdf_en_inviteSent: 
			Sdf_mc_strncpy(pState,"Sdf_en_inviteSent",dLen);
			break;
		case Sdf_en_reInviteReceived: 
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteReceived",dLen);
			break;
		case Sdf_en_reInviteSent:
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteSent",dLen);
			break;
		case Sdf_en_reInviteFinalResponseReceived: 
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteFinalResponseReceived",dLen);
			break;
		case Sdf_en_reInviteFinalResponseSent:
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteFinalResponseSent",dLen);
			break;
		case Sdf_en_finalSuccessReceived:	
			Sdf_mc_strncpy(pState,"Sdf_en_finalSuccessReceived",dLen);
			break;
		case Sdf_en_finalSuccessSent:		
			Sdf_mc_strncpy(pState,"Sdf_en_finalSuccessSent",dLen);
			break;
		case Sdf_en_finalFailureReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_finalFailureReceived",dLen);
			break;
		case Sdf_en_finalFailureSent:		
			Sdf_mc_strncpy(pState,"Sdf_en_finalFailureSent",dLen);
			break;
		case Sdf_en_callEstablished: 		
			Sdf_mc_strncpy(pState,"Sdf_en_callEstablished",dLen);
			break;
		case Sdf_en_byeReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_byeReceived",dLen);
			break;
		case Sdf_en_byeSent:			
			Sdf_mc_strncpy(pState,"Sdf_en_byeSent",dLen);
			break;
		case Sdf_en_inviteCancelled:
			Sdf_mc_strncpy(pState,"Sdf_en_inviteCancelled",dLen);
			break;
		case Sdf_en_reInviteCancelled:
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteCancelled",dLen);
			break;
		case Sdf_en_callTerminated:
			Sdf_mc_strncpy(pState,"Sdf_en_callTerminated",dLen);
			break;
		case Sdf_en_transactionReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_transactionReceived",dLen);
			break;
		case Sdf_en_transactionSent:
			Sdf_mc_strncpy(pState,"Sdf_en_transactionSent",dLen);
			break;
		case Sdf_en_rprReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_rprReceived",dLen);
			break;
		case Sdf_en_rprSent:
			Sdf_mc_strncpy(pState,"Sdf_en_rprSent",dLen);
			break;
		case Sdf_en_overlapTransactionReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_overlapTransactionReceived",dLen);
			break;
		case Sdf_en_overlapTransactionSent:
			Sdf_mc_strncpy(pState,"Sdf_en_overlapTransactionSent",dLen);
			break;
		case Sdf_en_transactionCancel:
			Sdf_mc_strncpy(pState,"Sdf_en_transactionCancel",dLen);
			break;
		case Sdf_en_provisionalRespSent:
			Sdf_mc_strncpy(pState,"Sdf_en_provisionalRespSent",dLen);
			break;
		case Sdf_en_provisionalRespReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_provisionalRespReceived",
                    Sdf_mc_strlen("Sdf_en_provisionalRespReceived"));
			break;
		case Sdf_en_reInviteprovRespReceived:
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteprovRespReceived",dLen);
			break;
		case Sdf_en_reInviteprovRespSent:
			Sdf_mc_strncpy(pState,"Sdf_en_reInviteprovRespSent",dLen);
			break;
	}
	pErr->errCode=Sdf_en_noUaError;
	return Sdf_co_success;
}

#endif /* SDF_TRACE */
