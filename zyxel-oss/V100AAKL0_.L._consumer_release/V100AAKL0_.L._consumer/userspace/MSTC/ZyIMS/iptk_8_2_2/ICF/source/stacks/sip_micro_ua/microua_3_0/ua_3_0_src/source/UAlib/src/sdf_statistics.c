/*****************************************************************************
 ** FUNCTION:		Has all the statistics functions for the framework.	
 **
 *****************************************************************************
 **
 ** FILENAME:		sdf_statistics.c
 **
 ** DESCRIPTION: 	This file contains all the functions related to 
 **					manipulation of statistics related data.
 **  	
 **
 ** DATE			NAME      	   	REFERENCE 	    REASON
 ** ----			----         	---------		------
 ** 10/03/01		Seshashayi.T					Creation
 **
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2006
 *****************************************************************************/

#include "ALheader.h"
#include "sdf_statistics.h"
#include "sdf_portlayer.h"
#include "sdf_debug.h"

Sdf_st_callStats sdf_gl_callStatistics;

/*
 * CSM table for state transitions based on outgoing SIP messages
 */
#ifdef SDF_LINT
extern Sdf_ty_state dCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes]\
	[Sdf_mc_noOfStates];
#else
extern Sdf_ty_u32bit dCsmNxtStateOnMsgSent[Sdf_mc_noOfMessageTypes]\
	[Sdf_mc_noOfStates];
#endif

/*
 * CSM table for state transitions based on incoming SIP messages
 */
extern Sdf_st_nextCallState dCsmNxtStateOnMsgRecd[Sdf_mc_noOfMessageTypes]\
	[Sdf_mc_noOfStates];


/*****************************************************************
** Function: sdf_ivk_uaInitStatistics 
** Description: Initialise the corresponding statistics structure
**		with 0
**
*****************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitStatistics 
#ifdef ANSI_PROTO
	(Sdf_ty_statsType dStatsType,
	Sdf_st_error *pErr)
#else
	(dStatsType,pErr)
	Sdf_ty_statsType dStatsType;
	Sdf_st_error *pErr;
#endif
{
	(void) dStatsType;
	(void) pErr;
#ifndef SDF_STATISTICS
	return Sdf_co_fail;
#else
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitStatistics");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{ 
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaInitStatistics(): "
			"Invalid pErr parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch(dStatsType)
	{
		case Sdf_en_call:
			sdf_gl_callStatistics.dCallsReceived =0;
			sdf_gl_callStatistics.dRemoteCallsEstd =0;
			sdf_gl_callStatistics.dRemoteCallsRejd =0;
			sdf_gl_callStatistics.dCallsInitiated =0;
			sdf_gl_callStatistics.dCallsEstd =0;
			sdf_gl_callStatistics.dCallsRejd =0;
			sdf_gl_callStatistics.dCallsTerminated =0;
			sdf_gl_callStatistics.dTransactionsSent =0;
			sdf_gl_callStatistics.dTransactionsRecv =0;
			break;	
		default:
			return Sdf_co_fail;

	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaInitStatistics");
	return Sdf_co_success;
#endif
}


/*****************************************************************
** Function:  sdf_fn_uaIncrementCallStats
** Description: Increment the corresponding callType count 
**
*****************************************************************/
Sdf_ty_retVal sdf_fn_uaIncrementCallStats
#ifdef ANSI_PROTO
	(Sdf_ty_callStats dStatsType,
	Sdf_st_error *pErr)
#else
	(dStatsType,pErr)
	Sdf_ty_callStats dStatsType;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_STATISTICS
#ifndef SDF_LINT
    Sdf_ty_callStats dDummyCallType;
	Sdf_st_error *pDummyErr = Sdf_co_null;
	dDummyCallType = dStatsType;
	pDummyErr = pErr;
#endif   
    (void)dStatsType;
    (void)pErr;
	return Sdf_co_fail;
#else
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaIncrementCallStats");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{ 
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_fn_uaIncrementCallStats(): "
			"Invalid pErr parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch(dStatsType)
	{
		case Sdf_en_callsReceived:
			sdf_gl_callStatistics.dCallsReceived++;
			break;
		case Sdf_en_remoteCallsEstd:
			sdf_gl_callStatistics.dRemoteCallsEstd++;
			break;
		case Sdf_en_remoteCallsRejd:
			sdf_gl_callStatistics.dRemoteCallsRejd++;
			break;
		case Sdf_en_callsInitiated:
			sdf_gl_callStatistics.dCallsInitiated++;
			break;
		case Sdf_en_callsEstd:
			sdf_gl_callStatistics.dCallsEstd++;
			break;
		case Sdf_en_callsRejd:
			sdf_gl_callStatistics.dCallsRejd++;
			break;
		case Sdf_en_callsTerminated:
			sdf_gl_callStatistics.dCallsTerminated++;
			break;
		case Sdf_en_transactionsSent:
			sdf_gl_callStatistics.dTransactionsSent++;
			break;
		case Sdf_en_transactionsRecv:
			sdf_gl_callStatistics.dTransactionsRecv++;
			break;
		default:
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidStatsType,\
			(Sdf_ty_s8bit *) "sdf_fn_uaIncrementCallStats():"
			"Invalid statistics type",pErr);
#endif			
			pErr->errCode=Sdf_en_invalidStatsType;
			return Sdf_co_fail;
	}
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_fn_uaIncrementCallStats");
	return Sdf_co_success;
#endif
}

/*****************************************************************
** Function:  sdf_ivk_uaGetStatistics
** Description:  get statistics for the corresponding stats type
**
*****************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetStatistics 
#ifdef ANSI_PROTO
	(Sdf_ty_statsType dStatsType,
	Sdf_ty_s32bit dStatsScope,
	Sdf_ty_bool dReset,
	Sdf_ty_pvoid pStats,
	Sdf_st_error *pErr)
#else
	(dStatsType,dStatsScope,dReset,pStats,pErr)
	Sdf_ty_statsType dStatsType;
	Sdf_ty_s32bit dStatsScope;
	Sdf_ty_bool dReset;
	Sdf_ty_pvoid pStats;
	Sdf_st_error *pErr;
#endif
{
	(void) dStatsType;
	(void) dStatsScope;
	(void) dReset;
	(void) pStats;
	(void) pErr;
#ifndef SDF_STATISTICS
	return Sdf_co_fail;
#else
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaGetStatistics");

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{ 
#ifdef SDF_ERROR		
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaGetStatistics(): "
			"Invalid pErr parameter passed to the function");
#endif
		return Sdf_co_fail;
	}
#endif	/* End of param validation ifdef */

	switch(dStatsType)
	{
		case Sdf_en_call:
			switch(dStatsScope)
			{
				case Sdf_en_callsReceived:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dCallsReceived;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dCallsReceived =0;	
					break;
				case Sdf_en_remoteCallsEstd:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dRemoteCallsEstd;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dRemoteCallsEstd =0;
					break;
				case Sdf_en_remoteCallsRejd:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dRemoteCallsRejd;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dRemoteCallsRejd =0;
					break;
				case Sdf_en_callsInitiated:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dCallsInitiated;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dCallsInitiated =0;
					break;
				case Sdf_en_callsEstd:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dCallsEstd;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dCallsEstd =0;
					break;
				case Sdf_en_callsRejd:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dCallsRejd;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dCallsRejd =0;
					break;
				case Sdf_en_callsTerminated:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dCallsTerminated;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dCallsTerminated =0;
					break;
				case Sdf_en_transactionsSent:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dTransactionsSent;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dTransactionsSent =0;
					break;
				case Sdf_en_transactionsRecv:
					*((Sdf_ty_u32bit *)pStats)= sdf_gl_callStatistics.\
					dTransactionsRecv;
					if(dReset == Sdf_co_true)
						sdf_gl_callStatistics.dTransactionsRecv =0;
					break;
				default:
					return Sdf_co_fail;
			}
			break;
		default:
			return Sdf_co_fail;
	}
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaGetStatistics");
	return Sdf_co_success;
#endif
}


/*****************************************************************
** Function:  sdf_fn_uaUpdateStatistics
** Description: Updates the statistics based on the current state
**				and the next state to which the call object is
**				transiting
**
*****************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateStatistics
#ifdef ANSI_PROTO
	(Sdf_ty_msgMode dMode,
	Sdf_ty_state dCurrState,
	Sdf_ty_messageType dMsgType,
	Sdf_st_error *pErr)
#else
	(dMode, dCurrState, dMsgType, pErr)
	Sdf_ty_msgMode dMode;
	Sdf_ty_state dCurrState;
	Sdf_ty_messageType dMsgType;
	Sdf_st_error *pErr;
#endif
{
#ifndef SDF_STATISTICS	
	(void) dMode;
	(void) dCurrState;
	(void) dMsgType;
	(void) pErr;
	return Sdf_co_fail;
#else
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaUpdateStatistics");
	
	if (dMode == Sdf_en_msgModeSend)
	{
		/*
		 * Block handling SIP messages sent by the call object.
		 */
		if ( (dCurrState == Sdf_en_idle) && (dCsmNxtStateOnMsgSent[dMsgType]\
			[dCurrState] == Sdf_en_transactionSent) )
		{
			/*
			 * Increment statistics for No of transactions sent
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_transactionsSent, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of transactions sent, failed", pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}	


		if ( (dCurrState == Sdf_en_idle) && \
			(dCsmNxtStateOnMsgSent[dMsgType][dCurrState] == Sdf_en_inviteSent) )
		{
			/*
			 * Increment statistics for No of calls initiated
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_callsInitiated, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of calls initiated, failed", pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		if ( (dCurrState == Sdf_en_finalSuccessReceived) && \
			(dCsmNxtStateOnMsgSent[dMsgType][dCurrState] == \
			Sdf_en_callEstablished) )
		{
			/*
			 * Increment statistics for No of calls established
			 */
			if(sdf_fn_uaIncrementCallStats(Sdf_en_callsEstd, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of calls established, failed", pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		if ( ((dCurrState == Sdf_en_inviteReceived)|| \
			(dCurrState == Sdf_en_provisionalRespSent)|| \
			(dCurrState == Sdf_en_inviteCancelled))&& \
			(dCsmNxtStateOnMsgSent[dMsgType][dCurrState] == \
			Sdf_en_finalFailureSent) )
		{
			/*
			 * Increment statistics for No of remote calls rejected
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_remoteCallsRejd, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors,\
					Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of remote calls rejected, failed",pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		if ( (dCurrState == Sdf_en_byeReceived) && \
			(dCsmNxtStateOnMsgSent[dMsgType][dCurrState] \
			== Sdf_en_callTerminated) )
		{
			/*
			 * Increment statistics for No of remote calls terminated 
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_callsTerminated, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of remote calls terminated, failed",\
					pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}

		if ( (dCurrState == Sdf_en_finalFailureReceived) && \
			(dCsmNxtStateOnMsgSent[dMsgType][dCurrState] != \
			Sdf_en_invalidStateTransition) )
		{
			/*
			 * Increment statistics for No of calls established
			 */
			if(dMsgType !=Sdf_en_ack)
			{
				if(sdf_fn_uaIncrementCallStats(Sdf_en_callsRejd, pErr) \
					== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics():"
						"Incrementing statistics for no of calls established,"
						"failed", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					return Sdf_co_fail;
				}
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
	}
	else
	{
		/*
		 * Block handling SIP messages received by the call object.
		 */
		if ( (dCurrState == Sdf_en_idle) && (dCsmNxtStateOnMsgRecd[dMsgType]\
			[dCurrState].dNextState == Sdf_en_transactionReceived) )
		{
			/*
			 * Increment statistics for No of transactions received
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_transactionsRecv, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of transactions received, failed",pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}

		if ( (dCurrState == Sdf_en_idle) && (dCsmNxtStateOnMsgRecd[dMsgType]\
			[dCurrState].dNextState == Sdf_en_inviteReceived) )
		{
			/*
			 * Increment statistics for No of calls received
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_callsReceived, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of calls received, failed",pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		if ( (dCurrState == Sdf_en_finalSuccessSent) && \
			(dCsmNxtStateOnMsgRecd[dMsgType][dCurrState].dNextState\
			== Sdf_en_callEstablished) )
		{
			/*
			 * Increment statistics for No of remote calls established
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_remoteCallsEstd, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of remote calls established, failed",\
					pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
		if (((dCurrState == Sdf_en_inviteSent) || \
				(dCurrState == Sdf_en_provisionalRespReceived)) && \
			(dCsmNxtStateOnMsgRecd[dMsgType][dCurrState].dNextState\
			== Sdf_en_finalFailureReceived) )
		{
			/*
			 * Increment statistics for No of calls rejected
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_callsRejd, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit *)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of calls rejected, failed",\
					pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}

		if ( (dCurrState == Sdf_en_byeSent) && \
			(dCsmNxtStateOnMsgRecd[dMsgType][dCurrState].dNextState == \
			Sdf_en_callTerminated) )
		{
			/*
			 * Increment statistics for No of calls terminated
			 */
			if (sdf_fn_uaIncrementCallStats(Sdf_en_callsTerminated, pErr) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
					(Sdf_ty_s8bit*)"sdf_fn_uaUpdateStatistics(): Incrementing "
					"statistics for no of calls terminated, failed",pErr);
#endif
				pErr->errCode=Sdf_en_invalidStatsType;
				return Sdf_co_fail;
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}

		if ( (dCurrState == Sdf_en_finalFailureSent) && \
			(dCsmNxtStateOnMsgRecd[dMsgType][dCurrState].dNextState\
			!= Sdf_en_invalidStateTransition) )
		{
			/*
			 * Increment statistics for No of remote calls established
			 */
			if(dMsgType !=Sdf_en_ack)
			{
				if (sdf_fn_uaIncrementCallStats(Sdf_en_remoteCallsRejd, pErr) \
					== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidStatsType,\
						(Sdf_ty_s8bit *)"sdf_fn_uaUpdateStatistics():"
						"Incrementing statistics for no of remote calls"
						"established, failed", pErr);
#endif
					pErr->errCode=Sdf_en_invalidStatsType;
					return Sdf_co_fail;
				}
			}
			sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
				"sdf_fn_uaUpdateStatistics");
			pErr->errCode = Sdf_en_noUaError;
			return Sdf_co_success;
		}
	}

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaUpdateStatistics");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
#endif
}
