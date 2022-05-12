/******************************************************************************
 ** FUNCTION:
 **			This file has all the accessor API's to retrieve/clone 
 **			various structures From the SessionTmierInfo Strucure
 ******************************************************************************
 **
 ** FILENAME		: sdf_sessTimer.c
 **
 ** DESCRIPTION		: This file has all the accessor API's to retrieve/clone 
 **					  various structures from the SessionTmierInfo structure
 **
 ** DATE		NAME			REFERENCE		REASON
 ** ----		----			---------		------
 ** 04/03/02	Pankaj Nath						Creation
 ** 09/04/10    Alok Tiwari  CSR 1-8223807  UATK CSR 1-8290042 Merged
 **
 ******************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#include "ALheader.h"
#include "sdf_portlayer.h"
#ifndef SDF_LINT
#include "sdf_common.h"
#include "sdf_struct.h"
#include "sdf_free.h"
#include "sdf_list.h"
#endif
#include "sdf_accessor.h"
#include "sdf_init.h"
#include "sdf_internal.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_callbacks.h"
#include "sdf_network.h"
#include "sdf_reqresp.h"
#include "sdf_callapis.h"
#include "sdf_sdp.h"
#include "sdf_sdpfree.h"
#include "sdf_sdpaccessor.h"
#include "sdf.h"


#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

#ifdef SDF_SESSION_TIMER

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaIsSupportedSessionTimer
 **
 ** DESCRIPTION: This function checks whether incoming message has
 **				 Supported header with value Timer. If presents then
 **				 *pIsSupported argumet is assign to true 
 **				 otherwise false.
 **				 
 **				 it checks the UAS transaction SipMessage.
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaIsSupportedSessionTimer
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	SipMessage *pMessage,
	Sdf_ty_bool *pIsSupported,
	Sdf_st_error  *pErr)
#else
	(pCallObj, pMessage, pIsSupported,pErr)
	Sdf_st_callObj *pCallObj;
	SipMessage *pMessage;
	Sdf_ty_bool *pIsSupported;
	Sdf_st_error  *pErr;
#endif	
{

	Sdf_ty_slist dSlist;
	Sdf_ty_u32bit dSize,i;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaIsSupportedSessionTimer");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaIsSupportedSessionTimer():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsSupportedSessionTimer():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pIsSupported == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsSupportedSessionTimer():  "
			"pIsSupported param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	(void)pCallObj;

	if(sdf_ivk_uaReturnSupportedList(pMessage,&dSlist,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_headerListAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsSupportedSessionTimer():  "
			"pIsSupported param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_headerListAccessError;
		return Sdf_co_fail;
	}
	
	if(sdf_listSizeOf(&dSlist, &dSize,pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_headerListAccessError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaIsSupportedSessionTimer(): "
			"Failed to get size of HeaderList\n",pErr);
#endif
		(void)sdf_listDeleteAll(&dSlist,pErr);
		pErr->errCode=Sdf_en_headerListAccessError;
		return Sdf_co_fail;
	}
	/* Check whether the Timer is present in the Supported list or not */

	if (dSize==(Sdf_ty_u32bit)0)
		*pIsSupported = Sdf_co_false;
	else
	{
		for( i=0; i< dSize ;i++)	
		{
			Sdf_ty_s8bit *pTemp;
			if( sdf_listGetAt(&dSlist,i,(Sdf_ty_pvoid * )&pTemp,pErr)\
				== Sdf_co_fail )
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaIsSupportedSessionTimer( ): "
					"Could not get an element From"
					"the Supported header list",pErr);
#endif				
				return Sdf_co_fail;
			}

			if ((Sdf_mc_strcasecmp(pTemp,"*")==0) || \
				(Sdf_mc_strcasecmp(pTemp,"timer")==0))
			{
				*pIsSupported = Sdf_co_true;
				break;
			}
		}
		if(i == dSize)
			*pIsSupported = Sdf_co_false;
	}
	(void)sdf_listDeleteAll(&dSlist,pErr);
	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaIsSupportedSessionTimer");
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetRefresher
**
** DESCRIPTION: This function returns the Refresher member of the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetRefresher
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_refresher *pRefresher,
	Sdf_st_error  *pErr )
#else	
	(pCallObj,pRefresher,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_refresher *pRefresher;
	Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Entering sdf_ivk_uaGetRefresher");
	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION

	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetRefresher(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRefresher(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pRefresher==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetRefresher(): "
			"Refresher parameter Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif	
	
	*pRefresher= pCallObj-> pSessionTimer-> dRefresher;
	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetRefresher");

	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetRefresher
**
** DESCRIPTION: This function Sets the Refresher  member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaSetRefresher
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_refresher dRefresher, 
	Sdf_st_error  *pErr )
#else
	(pCallObj,dRefresher,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_refresher dRefresher; 
	Sdf_st_error  *pErr;
#endif	
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetRefresher");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION

	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetRefresher(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetRefresher(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif
	/* If refresher of the SessionTimerInfo structure is Sdf_en_refresherNone
	 * then sets the Refresher member what passed in the argument refresher
	 */
	
	if(pCallObj->pSessionTimer->dRefresher== Sdf_en_refresherNone)
	{
		pCallObj-> pSessionTimer-> dRefresher = dRefresher;
	}
	/* If  the state of the Call object is idle or established 
	 * then sets the Refresher member what passed in the argument refresher
	 */

	else if (pCallObj->pCallInfo->dState == Sdf_en_callEstablished)
	{
		pCallObj-> pSessionTimer-> dRefresher = dRefresher;
	}
	/* Else Return fail */
	else
	{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidRefesher,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetRefresher(): "
				"Failed to set Refresher in Session-Expire structure",\
				pErr);
#endif
			pErr->errCode=Sdf_en_invalidRefesher;
			return Sdf_co_fail;
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetRefresher");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSessionInterval
**
** DESCRIPTION: This function returns the Session Interval value from the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetSessionInterval
#ifdef ANSI_PROTO
	( Sdf_st_callObject  *pCallObj,
	Sdf_ty_u32bit  *pSessionInterval,
	Sdf_st_error  *pErr )
#else	
	(pCallObj,pSessionInterval,pErr)
	 Sdf_st_callObject  *pCallObj;
	Sdf_ty_u32bit  *pSessionInterval;
	Sdf_st_error  *pErr;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetSessionInterval");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION

	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetSessionInterval(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionInterval(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pSessionInterval==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionInterval(): "
			"Session Interval  Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif
	
	
	*pSessionInterval= pCallObj-> pSessionTimer->dSessionInterval;
	pErr->errCode=Sdf_en_noUaError;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetSessionInterval");
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetSessionInterval
**
** DESCRIPTION: This function Sets the Session Interval  member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSessionInterval
#ifdef ANSI_PROTO
	( Sdf_st_callObject	*pCallObj,
	Sdf_ty_s32bit		dSessionInterval,
	Sdf_st_error		*pErr )
#else
	(pCallObj,dSessionInterval,pErr)
	Sdf_st_callObject	*pCallObj;
	Sdf_ty_s32bit		dSessionInterval;
	Sdf_st_error		*pErr;
#endif	
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetSessionInterval");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetSessionInterval(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSessionInterval(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (0 > dSessionInterval)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSessionInterval(): "
			"Session Interval Passed to function is invalid ",pErr);
#endif
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif

	/* If  the state of the Call object is idle or established 
	 * then sets the Session Interval member 
	 * what passed in the argument SessionInterval
	 */
	if(( pCallObj->pCallInfo->dState == Sdf_en_idle) || \
		( pCallObj->pCallInfo->dState == Sdf_en_callEstablished))
	{
		pCallObj-> pSessionTimer->dSessionInterval = dSessionInterval;
	}
	/* Else
	 * if received session interval is greater than the recived Min-Se
	 * than allowed to decrease the value
	 */
	else if((pCallObj->pSessionTimer->dSessionInterval >= \
		pCallObj->pSessionTimer->dMinSe) && ((pCallObj->\
		pSessionTimer->dSessionInterval > dSessionInterval ) &&\
		(dSessionInterval > pCallObj->pSessionTimer->dMinSe) )) 
	{
		pCallObj-> pSessionTimer->dSessionInterval = dSessionInterval;
	}
	/* Else
	 * if received session interval is less than the recived Min-Se
	 * than allowed to increase the value
	 */
	else if((pCallObj->pSessionTimer->dSessionInterval < pCallObj->\
		pSessionTimer->dMinSe) && (dSessionInterval > pCallObj->\
		pSessionTimer->dSessionInterval))
	{
		pCallObj-> pSessionTimer->dSessionInterval = dSessionInterval;
	}
	else
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidSessionInterval,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSessionInterval(): "
			"Failed to set Session Interval in Session-Expire structure",\
			pErr);
#endif
		pErr->errCode = Sdf_en_invalidSessionInterval;
		return Sdf_co_fail;
	}
	
	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetSessionInterval");
	return Sdf_co_success;
}
/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetMinSe
**
** DESCRIPTION: This function returns the MinSe value from the 
**	SessionTimerInfo Structure of the Call Object
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetMinSe
#ifdef ANSI_PROTO
	( Sdf_st_callObject  *pCallObj,
	Sdf_ty_u32bit *pMinSe,
	Sdf_st_error  *pErr) 
#else
	(pCallObj,pMinSe,pErr)
	Sdf_st_callObject  *pCallObj;
	Sdf_ty_u32bit *pMinSe;
	Sdf_st_error  *pErr;
#endif	
{ 

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetMinSe");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION

	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetMinSe(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMinSe(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pMinSe==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetMinSe(): "
			"MinSE Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif

	*pMinSe= pCallObj-> pSessionTimer->dMinSe;
	pErr->errCode=Sdf_en_noUaError;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetMinSe");
	return Sdf_co_success;
}



/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetMinSe
**
** DESCRIPTION: This function Sets the MinSe member of the 
**	SessionTimerInfo Structure of the Call Object what passed in the argument
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaSetMinSe
#ifdef ANSI_PROTO
	( Sdf_st_callObject  *pCallObj,
	Sdf_ty_s32bit dMinSe,
	Sdf_st_error  *pErr )
#else
	(pCallObj,dMinSe,pErr)
	 Sdf_st_callObject  *pCallObj;
	Sdf_ty_s32bit dMinSe;
	Sdf_st_error  *pErr;
#endif	
{

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetMinSe");
	
	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetMinSe(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetMinSe(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (dMinSe < 0 )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetMinSe(): "
			"Session Interval Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	/* If  the state of the Call object is idle 
	 * then sets the MinSE member 
	 * what passed in the argument MinSe
	 */
	if(pCallObj->pCallInfo->dState== Sdf_en_idle)
	{
		pCallObj-> pSessionTimer->dMinSe=dMinSe;
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_ivk_uaSetMinSe");
		return Sdf_co_success;
	}
	/* Otherwise validate the condtion as specified in the Session-Timer
	 * draft
	 */
	if(pCallObj->pSessionTimer->dIsSupportedTimer==Sdf_co_true)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidMinSe, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetMinSe(): "
			"Can't Set the MinSE into SessionTimerInfo Structure",pErr);
#endif
		pErr->errCode=Sdf_en_invalidMinSe;
		return Sdf_co_fail;
	}
	if( pCallObj->pSessionTimer->dMinSe !=0)
	{
		if (dMinSe  >= pCallObj->pSessionTimer->dMinSe ) 
		{
			pCallObj->pSessionTimer->dMinSe= dMinSe;
		}
		else
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidMinSe, \
				(Sdf_ty_s8bit *)"sdf_ivk_uaSetMinSe(): "
				"Can't Set the MinSE into SessionTimerInfo Structure",pErr);
#endif
			pErr->errCode=Sdf_en_invalidMinSe;
			return Sdf_co_fail;
		}
	}
	else
	{
		pCallObj-> pSessionTimer->dMinSe= dMinSe;
	}

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetMinSe");
	return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_ivk_uaGetSessionTimerInfoFromCallObject
**
** DESCRIPTION: This function returns the SessionTimerInfo structure from
**				 the Call Object
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaGetSessionTimerInfoFromCallObject
#ifdef ANSI_PROTO
	( Sdf_st_callObject *pCallObj,
	Sdf_st_sessionTimerInfo **ppSessionExpire,
	Sdf_st_error  *pErr )
#else
	(pCallObj,ppSessionExpire,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_sessionTimerInfo **ppSessionExpire;
	Sdf_st_error  *pErr;
#endif	
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaGetSessionTimerInfoFromCallObjec");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaGetSessionTimerInfoFromCallObjec(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionTimerInfoFromCallObjec(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (ppSessionExpire == Sdf_co_null )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetSessionTimerInfoFromCallObjec(): "
			"SessionTimerInfo Structure Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	*ppSessionExpire= pCallObj->pSessionTimer;
	HSS_LOCKEDINCREF( pCallObj->pSessionTimer->dRefCount);
	
	pErr->errCode=Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetSessionTimerInfoFromCallObjec");
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaSetSessionTimerInfoIntoCallObject
**
** DESCRIPTION: This function sets the SessionTimerInfo structure into
**				 the Call Object
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetSessionTimerInfoIntoCallObject
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_sessionTimerInfo *pSessionExpire, 
	Sdf_st_error  *pErr )
#else
	(pCallObj,pSessionExpire,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_sessionTimerInfo *pSessionExpire;
	Sdf_st_error  *pErr;
#endif	
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaSetSessionTimerInfoIntoCallObject");

	/* ================ Parameter validations ================== */
#ifdef SDF_PARAMVALIDATION
	/* 
	 * Parameter checks
	 */
	if (pErr == Sdf_co_null) 
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaSetSessionTimerInfoIntoCallObject(): "
			"Error Param Passed to function is invalid ");
#endif
		return Sdf_co_fail;
	}
	if (pCallObj==Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSessionTimerInfoIntoCallObject(): "
			"Call Object Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if (pSessionExpire == Sdf_co_null )
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetSessionTimerInfoIntoCallObject(): "
			"SessionTimerInfo Structure Passed to function is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif

	if( pCallObj->pSessionTimer != Sdf_co_null )
		(void)sdf_ivk_uaFreeSessionTimerInfo( pCallObj->pSessionTimer);

	pCallObj->pSessionTimer = pSessionExpire;
	HSS_LOCKEDINCREF(pSessionExpire->dRefCount);
	
	pErr->errCode=Sdf_en_noUaError;

	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetSessionTimerInfoIntoCallObject");

	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaInitSessionTimerInfo
**
** DESCRIPTION: This function initialise the members of the SessionTimerInfo
**				 Structure of the Call Object
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaInitSessionTimerInfo
#ifdef ANSI_PROTO
	(Sdf_st_sessionTimerInfo **ppSessionExpire, 
	Sdf_st_error  *pErr )
#else
	(ppSessionExpire,pErr)
	Sdf_st_sessionTimerInfo **ppSessionExpire; 
	Sdf_st_error  *pErr;
#endif

{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaInitSessionTimerInfo");

#ifdef SDF_PARAMVALIDATION
	if (pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR - "
			"sdf_ivk_uaInitSessionTimerInfo( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}
#endif

	
	/*
	 * Allocate memory for the structure
	 */
	*ppSessionExpire = (Sdf_st_sessionTimerInfo *) sdf_memget\
		(Sdf_mc_initMemId,sizeof(Sdf_st_sessionTimerInfo), Sdf_co_null);
	if (*ppSessionExpire == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_noMemoryError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSessionTimerInfo( ):"
			"Failed to allocate memory for nameInfo structure",pErr);
#endif
		pErr->errCode=Sdf_en_noMemoryError;
		return Sdf_co_fail;
	}	

	/*
	 * Initialize the member variables of the structure
	 */

	HSS_INITREF((*ppSessionExpire)->dRefCount);
	(*ppSessionExpire)->dRefresher=Sdf_en_refresherNone;
	(*ppSessionExpire)->dMinSe=(Sdf_ty_u32bit)0;
	(*ppSessionExpire)->dSessionInterval=(Sdf_ty_u32bit)0;
	(*ppSessionExpire)->dRefreshRequest=Sdf_en_refresherInvalid;
	(*ppSessionExpire)->dIsSupportedTimer=Sdf_co_false;
	(*ppSessionExpire)->pTimerId=Sdf_co_null;

	pErr->errCode = Sdf_en_noUaError;
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -"
		"Exiting sdf_ivk_uaInitSessionTimerInfo");	
	return Sdf_co_success;
}
/*****************************************************************************
** FUNCTION: sdf_ivk_uaFreeSessionTimerInfo
**
** DESCRIPTION: This function Free the SessionTimerInfo
**				 Structure from the Call Object. Decrements the refcount
**				and if it is zero then free the structure
**
******************************************************************************/

Sdf_ty_retVal sdf_ivk_uaFreeSessionTimerInfo
#ifdef ANSI_PROTO
	( Sdf_st_sessionTimerInfo *pSessionExpire)
#else
	(pSessionExpire)
	Sdf_st_sessionTimerInfo *pSessionExpire;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG -"
		"Entering sdf_ivk_uaFreeSessionTimerInfo");

	if(pSessionExpire == Sdf_co_null)
		return Sdf_co_success;
	
	HSS_LOCKREF(pSessionExpire->dRefCount);
	HSS_DECREF(pSessionExpire->dRefCount);
	
	if (HSS_CHECKREF(pSessionExpire->dRefCount))
	{
		HSS_UNLOCKREF(pSessionExpire->dRefCount);
		HSS_DELETEREF(pSessionExpire->dRefCount);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSessionExpire), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSessionExpire->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG -"
		"Exiting sdf_ivk_uaFreeSessionTimerInfo ");
	return Sdf_co_success;
}	

/*****************************************************************************
** FUNCTION: sdf_fn_uaFreeSessionTimerContextInfo
**
** DESCRIPTION: This function Free the SessionTimerContextInfo
**				  Decrements the refcount
**				and if it is zero then free the structure
**
******************************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeSessionTimerContextInfo
#ifdef ANSI_PROTO
	(Sdf_st_sessionTimerContextInfo		*pSessionTimerConext)
#else
	(pSessionTimerConext)
	Sdf_st_sessionTimerContextInfo	*pSessionTimerConext;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG -"
		"Entering sdf_fn_uaFreeSessionTimerContextInfo");

	if(pSessionTimerConext == Sdf_co_null)
		return Sdf_co_success;

	HSS_LOCKREF(pSessionTimerConext->dRefCount);
	HSS_DECREF(pSessionTimerConext->dRefCount);
	
	if (HSS_CHECKREF(pSessionTimerConext->dRefCount))
	{
		(void)sdf_ivk_uaFreeCallObject(&(pSessionTimerConext->pCallObject));
		if (pSessionTimerConext->pData != Sdf_co_null)
			/*w534*/(void)sdf_cbk_uaFreeEventContextData(&(pSessionTimerConext->pData));
		HSS_UNLOCKREF(pSessionTimerConext->dRefCount);
		HSS_DELETEREF(pSessionTimerConext->dRefCount);
		(void)sdf_memfree(Sdf_mc_freeMemId, (Sdf_ty_pvoid*)&(pSessionTimerConext), \
			(Sdf_st_error*)Sdf_co_null);
	}	
	else
	{
		HSS_UNLOCKREF(pSessionTimerConext->dRefCount);
	}
	sdf_fn_debug((Sdf_ty_s8bit*) "SDF_DEBUG -"
		"Exiting sdf_fn_uaFreeSessionTimerContextInfo ");
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaCloneSessionTimerInfo
 **
 ** DESCRIPTION: This function copies the SessionTimerInfo structure from 
 **				 src to dest
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloneSessionTimerInfo
#ifdef ANSI_PROTO
	( Sdf_st_sessionTimerInfo *pDestSessionExpire,
	Sdf_st_sessionTimerInfo *pSrcSessionExpire, 
	Sdf_st_error  *pErr)
#else
	(pDestSessionExpire,pSrcSessionExpire,pErr)
	Sdf_st_sessionTimerInfo *pDestSessionExpire;
	Sdf_st_sessionTimerInfo *pSrcSessionExpire;
	Sdf_st_error  *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Entering sdf_ivk_uaCloneSessionTimerInfo");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaCloneSessionTimerInfo():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pDestSessionExpire == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionTimerInfo():  "
			"pDestSessionExpire param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pSrcSessionExpire == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloneSessionTimerInfo():  "
			"pSrcSessionExpire Param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
#endif /* End param validations ifdef */

	pDestSessionExpire->dRefresher=pSrcSessionExpire->dRefresher;
	pDestSessionExpire->dMinSe=pSrcSessionExpire->dMinSe;
	pDestSessionExpire->dSessionInterval=pSrcSessionExpire->dSessionInterval;
	pDestSessionExpire->dIsSupportedTimer=pSrcSessionExpire->dIsSupportedTimer;
	/* cloning Timer-ID */	
	pDestSessionExpire->pTimerId=pSrcSessionExpire->pTimerId;
	
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloneSessionTimerInfo");

	pErr->errCode = Sdf_en_noUaError;

	return 	Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaSessionTimerExpire
 **
 ** DESCRIPTION: This is a Session Timer Expiry function. 
 **				 This function will send refresh re-INVITE or BYE on Expiry 
 **				 of the Session Timer Interval
 **
 ******************************************************************************/

Sdf_ty_retVal  sdf_fn_uaSessionTimerExpire
#ifdef ANSI_PROTO
	(Sdf_ty_timerType dTimerType,
	Sdf_ty_pvoid  pContextInfo,
	Sdf_st_error *pErr)
#else
	(dTimerType,pContextInfo,pErr)
	Sdf_ty_timerType dTimerType;
	Sdf_ty_pvoid  pContextInfo;
	Sdf_st_error *pErr;
#endif
{	
	Sdf_ty_handleEvent dRefresher;
	Sdf_st_callObject *pOrgCallObj=Sdf_co_null;
	Sdf_ty_retVal dCbkRetVal;
#ifdef SDF_TRACE
	Sdf_ty_s8bit Trace[Sdf_co_traceBufferSize];
#endif
	Sdf_st_sessionTimerContextInfo *pSessionTimerContextInfo;
	Sdf_ty_refresher dCurrentRefresher;
	Sdf_ty_u32bit	initvalue = 500; /* In Milli Secs */
	Sdf_ty_sessionRefreshRequest	dRefreshReq;
	Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
	Sdf_ty_s8bit	pMethod[Sdf_co_smallBufferSize];
	Sdf_st_eventContext	*pEventContext = Sdf_co_null;
	
	(void)dTimerType;
	(void) initvalue;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaSessionTimerExpire");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaSessionTimerExpire():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pContextInfo == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire():  "
			"pContextInfo param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	pSessionTimerContextInfo  = (Sdf_st_sessionTimerContextInfo *)pContextInfo;

	pOrgCallObj=pSessionTimerContextInfo->pCallObject;

	/* Make the timer id associated with the Session-Timer structure to
	 * Sdf_co_null, so that it can't be accessed further
	 */
	sdf_cbk_uaFreeTimerHandle (pOrgCallObj->pSessionTimer->pTimerId);
	pOrgCallObj->pSessionTimer->pTimerId=Sdf_co_null;


#ifdef SDF_TRACE
	(void)sdf_fn_uaSnprintf(Trace,Sdf_co_traceBufferSize,\
		"Call : %s -  Transaction received; "
		"Issuing sdf_cbk_uaHandleSTTimeout to the application",\
		pOrgCallObj->pCommonInfo->pKey->pCallId);
	sdf_fn_trace(Sdf_en_briefTraces,Sdf_en_cbksTraces,Trace,0,pErr);
#endif
	

#ifdef SDF_THREAD_SAFE	
	if(sdf_ivk_uaLockCallObject(pOrgCallObj, pErr)==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Cann't able to Lock the Call object",pErr);
#endif
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		return Sdf_co_fail;
	}
#endif	
	/* Check the state of the Call object after getting the lock of the
	 * callobject because while waiting for the Lock some other thread may
	 * change the state of the call object
	 */
	if((pOrgCallObj->pCallInfo->dState==Sdf_en_callTerminated)&& \
		(pOrgCallObj->pSessionTimer->dRefresher==Sdf_en_refresherLocal))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Cann't Form refresh re-INVITE in this call state",pErr);
#endif
#ifdef SDF_THREAD_SAFE	
		(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif		
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}

	/*
	 * If we have sent a reINVITE or UPDATE when Session Timer
	 * Expires, we shouldnot send another refresh request/BYE.
	 * We should wait for the response to previous request
	 */
	if ((pOrgCallObj->pSessionTimer->dRefreshRequest!=Sdf_en_refresherInvalid)\
		 && (pOrgCallObj->pSessionTimer->dRefresher==Sdf_en_refresherRemote))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Cann't Form BYE in this call state",pErr);
#endif
#ifdef SDF_THREAD_SAFE	
		(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif		
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}

	/* 
	 * Issue the SessionExpire callback to the application
	 * Befor issuing the callback store the refresher of the call object
	 */
	dCurrentRefresher=pOrgCallObj->pSessionTimer->dRefresher;

	dCbkRetVal = sdf_cbk_uaHandleSTTimeout(pOrgCallObj, \
		&dRefresher, &dRefreshReq, pErr);
	
	if (dCbkRetVal == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_callBackError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Callback sdf_cbk_uaHandleSTTimeout failed",pErr);
#endif
#ifdef SDF_THREAD_SAFE	
		(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif		
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		pErr->errCode=Sdf_en_callBackError;
		return Sdf_co_fail;
	}

	if((pOrgCallObj->pCallInfo->dState !=Sdf_en_callEstablished)&& \
		((dRefresher==Sdf_en_uatoolkit) && \
		(dCurrentRefresher==Sdf_en_refresherLocal)))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Cann't Form refresh re-INVITE in this call state",pErr);
#endif
#ifdef SDF_THREAD_SAFE	
		(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif		
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}

	if((pOrgCallObj->pCallInfo->dState !=Sdf_en_callEstablished)&& \
		((dRefresher==Sdf_en_uatoolkit) && \
		(dCurrentRefresher==Sdf_en_refresherRemote)))
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_callStateError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
			"Cann't Form BYE in this call state",pErr);
#endif
#ifdef SDF_THREAD_SAFE	
		(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif		
		(void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		pErr->errCode=Sdf_en_callStateError;
		return Sdf_co_fail;
	}

	/* Unlock The Call object mutex*/
#ifdef SDF_THREAD_SAFE	
	/*w534*/(void)sdf_ivk_uaUnlockCallObject(pOrgCallObj, pErr);
#endif	

	if (sdf_ivk_uaInitEventContext(&pEventContext, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
			"Cannot Initialise the Eventcontext", pErr);
#endif
        (void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);
		return Sdf_co_fail;
	}

	pEventContext->pData = pSessionTimerContextInfo->pData;
	pSessionTimerContextInfo->pData = Sdf_co_null;

	/*
	 * Now the pSessionTimerContextInfo->pData is cloned into
	 * pEventContext->pData. So we can free the pSessionTimerContextInfo.
	 */
    (void)sdf_fn_uaFreeSessionTimerContextInfo(pSessionTimerContextInfo);

	/* if the dRefresher==Sdf_en_applicaton then do nothing */
	if(dRefresher==Sdf_en_applicaton)
	{
		sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaSessionTimerExpire");
		(void)sdf_ivk_uaFreeEventContext(pEventContext);
		pErr->errCode = Sdf_en_noUaError;
		return Sdf_co_success;
	}
	/* else form the re-INVITE or BYE */
	else if(dRefresher==Sdf_en_uatoolkit)
	{
		SipOptions dOptions;
		if(dCurrentRefresher==Sdf_en_refresherRemote)
		{
			Sdf_mc_strcpy(pMethod,"BYE");

			if (sdf_ivk_uaStartTransaction(pOrgCallObj, &pOverlapTransInfo, \
					pMethod, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
					"Cannot start BYE transaction in current state.", pErr);
#endif				
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				return Sdf_co_fail;
			}
			if (sdf_ivk_uaTerminateCall(pOrgCallObj, pErr) == \
					Sdf_co_fail)
			{
#ifdef SDF_ERROR			
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
					"Cannot Terminate the call", pErr);
#endif				
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				return Sdf_co_fail;
			}
			dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
#ifndef SDF_LOOKUP
            /* CSR_1-8290042 changes start */
			if(sdf_ivk_uaSendCallToPeer(pOrgCallObj, pOrgCallObj->\
				pUacTransaction->pSipMsg, dOptions, 0, \
				pEventContext, Sdf_co_null, pErr) == Sdf_co_fail)
            /* CSR_1-8290042 changes end */
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
					"Sending BYE to peer entity failed",pErr);
#endif
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				return Sdf_co_fail;
			}
			/*w534*/(void)sdf_ivk_uaEndTransaction(pOrgCallObj, pOverlapTransInfo, \
				   	pMethod, pErr);
#else
			if(sdf_cbk_uaSendCallToPeer(pOrgCallObj, pOverlapTransInfo,
				pOrgCallObj->pUacTransaction->pSipMsg, dOptions, 0, \
				pEventContext, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
					"Sending BYE to peer entity failed",pErr);
#endif
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				return Sdf_co_fail;	
			}
#endif
		}
		else if(dCurrentRefresher == Sdf_en_refresherLocal)
		{
			/*- Appropriate SipMessage structure should be
			 *  passed to sdf_ivk_uaSendCallToPeer -*/
			SipMessage *pSipMsg = Sdf_co_null;
			Sdf_ty_u32bit duration=0;
			
			if (dRefreshReq == Sdf_en_refresherInvite)
			{
				Sdf_mc_strcpy(pMethod,"INVITE");
				if (sdf_ivk_uaStartTransaction(pOrgCallObj,&pOverlapTransInfo,\
					pMethod, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
						"Cannot start BYE transaction in current state.", pErr);
#endif				
					(void)sdf_ivk_uaFreeEventContext(pEventContext);
					return Sdf_co_fail;
				}
				if(sdf_ivk_uaFormRefresh(pOrgCallObj,pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
					"Cannot Form the refresh re-INVITE for the Session", pErr);
#endif				
					(void)sdf_ivk_uaFreeEventContext(pEventContext);
					(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo,\
						pMethod, Sdf_en_uacTransaction);
					return Sdf_co_fail;
				}
				pSipMsg = pOrgCallObj->pUacTransaction->pSipMsg;
			}
			else
			{
				Sdf_mc_strcpy(pMethod,"UPDATE");
				if (sdf_ivk_uaStartTransaction(pOrgCallObj, \
						&pOverlapTransInfo, pMethod, pErr) == Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError( Sdf_en_majorErrors, pErr->errCode, \
						(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
						"Could not start an UPDATE request.\n",pErr);
#endif
					(void)sdf_ivk_uaFreeEventContext(pEventContext);
					return Sdf_co_fail;
				}

				if(sdf_ivk_uaMakeUpdate(pOrgCallObj, pOverlapTransInfo,\
						pErr)==Sdf_co_fail)
				{
#ifdef SDF_ERROR			
					sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode,\
						(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire( ):"
						"Cannot Form the refresh UPDATE for the Session", pErr);
#endif				
					(void)sdf_ivk_uaFreeEventContext(pEventContext);
					(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo,\
						pMethod, Sdf_en_uacTransaction);
					return Sdf_co_fail;
				}
				pSipMsg = pOverlapTransInfo->pSipMsg;
			}

			/* Get the Timeout value for re-INVITE timeout 
			 * While re-INVITE times out, its needs to send bye.
			 * For thsi purpose retrived the timeout time for re-INVITE
			 */	
			duration=(pOrgCallObj->pSessionTimer->dSessionInterval)/2;
			if(sdf_fn_uaGetReinviteTimeoutTime(&duration,pErr)==Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_sessionTimerError, \
					(Sdf_ty_s8bit *)"sdf_ivk_uaSendCallToPeer( ): "
					"Could not get AppData from call object\n",pErr);
#endif
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				pErr->errCode=Sdf_en_sessionTimerError;
				return Sdf_co_fail;
			}

#ifdef SDF_TXN
			pEventContext->dTimeoutValues.dTimerB = duration;
#else
			pEventContext->dMaxInviteRetransCount =	\
					sdf_fn_uaGetRetransCount(initvalue, duration*1000, pErr);
			pEventContext->dRetransT1 = initvalue;
			pEventContext->dRetransT2 =duration;
#endif

			pEventContext->dRemainingLength = 0;
			pEventContext->dReinviteTimeout=Sdf_co_true;

			/* For Txn Layer EvenContext */	
			dOptions.dOption =  SIP_OPT_CLEN|SIP_OPT_FULLFORM |		\
			   			SIP_OPT_PERMSGRETRANSCOUNT | SIP_OPT_PERMSGRETRANS;
#ifndef SDF_LOOKUP
			/*- Appropriate SipMessage structure is passed -*/
            /* CSR_1-8290042 changes start */
			if(sdf_ivk_uaSendCallToPeer(pOrgCallObj, pSipMsg, dOptions, 0, \
				pEventContext,Sdf_co_null, pErr) == Sdf_co_fail)
            /* CSR_1-8290042 changes end */
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
					"Sending refresh re-INVITE to peer entity failed",pErr);
#endif
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				return Sdf_co_fail;
			}
			/*w534*/(void)sdf_ivk_uaEndTransaction(pOrgCallObj, pOverlapTransInfo, \
					pMethod, pErr);
#else
			/*- Appropriate SipMessage structure is passed -*/
			if(sdf_cbk_uaSendCallToPeer(pOrgCallObj, pOverlapTransInfo,pSipMsg, dOptions, 0, \
				pEventContext, pErr) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
					(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
					"Sending refresh re-INVITE to peer entity failed",pErr);
#endif
				(void)sdf_ivk_uaClearTransaction(pOrgCallObj, pOverlapTransInfo, \
						pMethod, Sdf_en_uacTransaction);
				(void)sdf_ivk_uaFreeEventContext(pEventContext);
				return Sdf_co_fail;
			}
#endif
		}
		else
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidRefesher, \
				(Sdf_ty_s8bit *)"sdf_fn_uaSessionTimerExpire(): "
				"Invalid  refresher",pErr);
#endif
			(void)sdf_ivk_uaFreeEventContext(pEventContext);
			pErr->errCode=Sdf_en_invalidRefesher;
			return Sdf_co_fail;
		}
	}
	/*w534*/(void)sdf_ivk_uaFreeEventContext(pEventContext);
	sdf_fn_debug((Sdf_ty_s8bit *) "SDF_DEBUG - "
			"Exiting sdf_fn_uaSessionTimerExpire");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaNegotiateRefresher
 **
 ** DESCRIPTION: This function will negotiate the refresher member
 **				 of the Session Expires header of the incoming message
 **				 and set the refresher member of the SessionTimerInfo
 **				 structure
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_fn_uaNegotiateRefresher
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_error *pErr)
#else
	(pCallObj,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_error *pErr;
#endif
{

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaNegotiateRefresher");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaNegotiateRefresher():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaNegotiateRefresher():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/* Check whether incoming message has a Supported header with value
	 * Timer
	 * if absent then set the refresher as defined in the session-timer
	 * draft
	 */
	if (pCallObj->pSessionTimer->dIsSupportedTimer==Sdf_co_false)
	{
		if(pCallObj-> pSessionTimer->dRefresher == Sdf_en_refresherNone) 
			pCallObj->pSessionTimer->dRefresher= Sdf_en_refresherNone;	
		else if(pCallObj-> pSessionTimer->dRefresher ==Sdf_en_refresherRemote)
			pCallObj-> pSessionTimer->dRefresher= Sdf_en_refresherNone;	
		else if(pCallObj-> pSessionTimer->dRefresher == Sdf_en_refresherLocal)
			pCallObj-> pSessionTimer->dRefresher= Sdf_en_refresherLocal;	
	}
	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaNegotiateRefresher");
	return Sdf_co_success;
}



/******************************************************************************
 ** FUNCTION:  	 sdf_fn_uaAddSessionTimerSpecificHeader
 **
 ** DESCRIPTION: This function will add the Session Timer Specific
 **				 header depending on the value presents in the
 **				 SessionTimerInfo structure into the outgoing
 **				 message 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaAddSessionTimerSpecificHeader
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	SipMessage *pMessage,
	Sdf_ty_messageType 	dMethodType,
	Sdf_st_error *pErr)
#else
	(pCallObj,pMessage, dMethodType, pErr)
	Sdf_st_callObject *pCallObj;
	SipMessage *pMessage;
	Sdf_ty_messageType 	dMethodType;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u16bit    		dRespCode=0;
	en_SipMessageType 		dType;
	SipHeader 				*pTempHeader = Sdf_co_null;
	Sdf_ty_s32bit			dMinSE = -1;
	Sdf_ty_bool				dUpdateAfterDialogEst = Sdf_co_false;
	Sdf_ty_s8bit 			*pRefresher = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaAddSessionTimerSpecificHeader");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaAddSessionTimerSpecificHeader():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAddSessionTimerSpecificHeader():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pMessage == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAddSessionTimerSpecificHeader():  "
			"SipMessage Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	
	/* Get the type of the message */	
	if (sip_getMessageType(pMessage, &dType, (SipError*)\
		&(pErr->stkErrCode)) ==  SipFail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_headerManipulationError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaAddSessionTimerSpecificHeader(): "
			"Failed to get type from SIP message",pErr);
#endif
		pErr->errCode=Sdf_en_headerManipulationError;
		return Sdf_co_fail;
	}

	/* Get the Method of the message*/

	if ((dMethodType != Sdf_en_invite) && (dMethodType != Sdf_en_update))
		return Sdf_co_success;

	/* Get the response code for response message*/

	if (dType == SipMessageResponse)
	{
		if (sdf_ivk_uaGetRespCodeFromSipMessage(pMessage,&dRespCode, \
			pErr) == Sdf_co_fail)
		{		
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, pErr->errCode, \
				(Sdf_ty_s8bit *)"sdf_fn_uaAddSessionTimerSpecificHeader(): "
				"Error in getting response code from Sip Message",pErr);
#endif
			return Sdf_co_fail;
		}	
		/*
		 * If the request didnt come with Session-Expires header, then UATK
		 * should not add this header in Response
		 */
		if (pCallObj->pSessionTimer->dRefreshRequest==Sdf_en_refresherInvalid)
		{
			return Sdf_co_success;
		}
	 }
	
	if( (dMethodType == Sdf_en_update) && \
		(sdf_fn_uaCallState(pCallObj)==Sdf_en_confirmedDialogState))
		dUpdateAfterDialogEst = Sdf_co_true;


	/*- Added Update support -*/
	if (((dType == SipMessageResponse) && ((dRespCode == 422) || (dRespCode == 200)) && \
		((dMethodType == Sdf_en_invite) || \
		(dUpdateAfterDialogEst == Sdf_co_true))) || \
		((dType == SipMessageRequest) &&\
		((dMethodType == Sdf_en_invite) || \
		 (dUpdateAfterDialogEst == Sdf_co_true))))
	{
		/*  Set The MinSE header for 200 and 422 response */
		
		if (sip_initSipHeader(&pTempHeader,SipHdrTypeMinSE, (SipError *)&(\
			pErr->stkErrCode)) == SipFail)
		{
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_headerManipulationError, \
				(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
				"Failed to init Sip header", pErr);
#endif		
			pErr->errCode=Sdf_en_headerManipulationError;
			return Sdf_co_fail;
		}

		if(pCallObj->pSessionTimer->dMinSe > 0 )
		{
			if (pCallObj->pSessionTimer->dMinSe > sdf_fn_uaGetGlbMinSe())
				dMinSE = pCallObj->pSessionTimer->dMinSe;
			else
				dMinSE = sdf_fn_uaGetGlbMinSe();

			if(sip_setSecondsInMinSEHdr(pTempHeader, dMinSE, \
				(SipError *)&(pErr->stkErrCode)) ==SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to set Secconds in MinSE header", pErr);
#endif		
				sip_freeSipHeader(pTempHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pTempHeader, Sdf_co_null);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		else  if((pCallObj->pSessionTimer->dMinSe <= 0 ) && \
			(dType==SipMessageResponse))
		{
			if(sip_setSecondsInMinSEHdr(pTempHeader,sdf_fn_uaGetGlbMinSe(),\
				(SipError *)&(pErr->stkErrCode)) ==SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to set Secconds in MinSE header", pErr);
#endif		
				sip_freeSipHeader(pTempHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pTempHeader, Sdf_co_null);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		if(((dType==SipMessageRequest) && (pCallObj->\
			pSessionTimer->dMinSe >0)) ||\
			(dType==SipMessageResponse))
		{
			if (sip_setHeader(pMessage,pTempHeader,(SipError *)\
				&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to set MinSE header in Sip Message", pErr);
#endif		
				sip_freeSipHeader(pTempHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
				&pTempHeader, Sdf_co_null);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
		}
		/* Free the Local Refrerence */

		sip_freeSipHeader(pTempHeader);
		(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
			&pTempHeader, Sdf_co_null);
		pTempHeader=Sdf_co_null;
	}
	/*- Added Update support -*/
	if(((dType == SipMessageResponse) && \
		(((dMethodType == Sdf_en_invite) || \
		((dUpdateAfterDialogEst == Sdf_co_true ))) &&\
		((dRespCode==200) || (dRespCode ==422)))) ||\
		((dType == SipMessageRequest) &&\
		((dMethodType == Sdf_en_invite) || \
		 (dUpdateAfterDialogEst == Sdf_co_true)))) 
	{		

		if((dRespCode==200) || (dType==SipMessageRequest))
		{
			/*  Set The Session-Expires header */

			SipHeader 		*pSessionExpires = Sdf_co_null;
			if (sip_initSipHeader(&pSessionExpires,SipHdrTypeSessionExpires,\
				 (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
				Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to init Sip header", pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}

			/* Set The Session-Interval from the SessionTimerInfo structure
			 */
			
			if(pCallObj->pSessionTimer->dSessionInterval > 0 )
			{
				if(sip_setSecondsInSessionExpiresHdr(pSessionExpires,\
					pCallObj->pSessionTimer->dSessionInterval, \
					(SipError *)&(pErr->stkErrCode)) ==SipFail)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_headerManipulationError,(Sdf_ty_s8bit*)\
						"sdf_fn_uaAddSessionTimerSpecificHeader() "
						": Failed to set Secconds"
						"in Session-Expires header", pErr);
#endif		
					sip_freeSipHeader(pSessionExpires);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pSessionExpires, Sdf_co_null);
					pErr->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				
				if (dType==SipMessageRequest)
				{
					/* At the time of Making the request, if the request is INVITE or
					 * UPDATE (in established state), then set the session timer flag
					 * dRefreshRequest -*/
					if (sdf_fn_uaCallState(pCallObj) == Sdf_en_confirmedDialogState)
					{
						if (dMethodType == Sdf_en_invite)
							pCallObj->pSessionTimer->dRefreshRequest = \
								Sdf_en_refresherInvite;
						else if (dMethodType == Sdf_en_update)
							pCallObj->pSessionTimer->dRefreshRequest = \
								Sdf_en_refresherUpdate;
					}
				}

			}
			/* Set the Refresher parameter in the Session-Expires header */
			if(pCallObj->pSessionTimer->dRefresher==Sdf_en_refresherLocal)
			{
				if(dType==SipMessageResponse)
					Sdf_mc_strdup(pRefresher,"uas");
				else
					Sdf_mc_strdup(pRefresher,"uac");
			}
			else if(pCallObj->pSessionTimer->dRefresher==Sdf_en_refresherRemote)
			{
				if(dType==SipMessageResponse)
					Sdf_mc_strdup(pRefresher,"uac");
				else
					Sdf_mc_strdup(pRefresher,"uas");
			}

			if((pCallObj->pSessionTimer->dRefresher==Sdf_en_refresherLocal) || \
			   (pCallObj->pSessionTimer->dRefresher==Sdf_en_refresherRemote))
			{
				if(sip_setRefresherInSessionExpiresHdr(pSessionExpires,\
							pRefresher,(SipError *)&(pErr->stkErrCode)) ==SipFail)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_headerManipulationError,(Sdf_ty_s8bit*)\
							"sdf_fn_uaAddSessionTimerSpecificHeader( )"
							": Failed to set Refresher"
							"in Session-Expires header", pErr);
#endif		
					sip_freeSipHeader(pSessionExpires);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pSessionExpires, Sdf_co_null);
					pErr->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pRefresher, Sdf_co_null);
			}
			if(pCallObj->pSessionTimer->dRefresher==Sdf_en_refresherRemote)
			{
				if(dType==SipMessageResponse)
				{
					SipHeader 				*pRequire = Sdf_co_null;
					Sdf_ty_s8bit 			*pToken=Sdf_co_null;
					/* Insert Require:timer to the outgoing response */
					if (sip_initSipHeader(&pRequire,SipHdrTypeRequire,\
						 (SipError *)&(pErr->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR		
						sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_headerManipulationError, \
						(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
						"Failed to init Sip header", pErr);
#endif		
						sip_freeSipHeader(pSessionExpires);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pSessionExpires, Sdf_co_null);
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_co_fail;
					}
					pToken = (Sdf_ty_s8bit *) sdf_memget\
						(Sdf_mc_initMemId,10*sizeof(Sdf_ty_s8bit), Sdf_co_null);
					Sdf_mc_strcpy(pToken,"timer");
					if(sip_setTokenInRequireHdr(pRequire,pToken,\
						(SipError *)&(pErr->stkErrCode)) ==SipFail)
					{
#ifdef SDF_ERROR		
						sdf_fn_setError(Sdf_en_minorErrors,\
							Sdf_en_headerManipulationError,(Sdf_ty_s8bit*)\
							"sdf_fn_uaAddSessionTimerSpecificHeader( ): Failed to set Token"
							"in Require header", pErr);
#endif		
						sip_freeSipHeader(pSessionExpires);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pSessionExpires, Sdf_co_null);
						sip_freeSipHeader(pRequire);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pRequire, Sdf_co_null);
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_co_fail;
					}
					if (sip_insertHeaderAtIndex(pMessage,pRequire,0,\
						(SipError *) &(pErr->stkErrCode)) == SipFail)
					{
#ifdef SDF_ERROR		
						sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_headerManipulationError, (Sdf_ty_s8bit*)\
						"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
						"Failed to set Require header in Sip Message", pErr);
#endif			
						sip_freeSipHeader(pRequire);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pRequire, Sdf_co_null);
						sip_freeSipHeader(pSessionExpires);
						(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
							&pSessionExpires, Sdf_co_null);
						pErr->errCode=Sdf_en_headerManipulationError;
						return Sdf_co_fail;
					}

					/* Freeing the local refrerence */
					/*purify */
					sip_freeSipHeader(pRequire);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pRequire, Sdf_co_null);
					pRequire=Sdf_co_null;
				}
			}
			if(pCallObj->pSessionTimer->dSessionInterval > 0 )
			{
				if (sip_insertHeaderAtIndex(pMessage,pSessionExpires,0, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
				{
#ifdef SDF_ERROR		
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_headerManipulationError,(Sdf_ty_s8bit*)\
						"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
						"Failed to set Session-Expires  header in Sip Message",\
						pErr);
#endif		
					sip_freeSipHeader(pSessionExpires);
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pSessionExpires, Sdf_co_null);
					pErr->errCode=Sdf_en_headerManipulationError;
					return Sdf_co_fail;
				}
			}
			/* Free the Local Refrerence */

			/* purify */
			sip_freeSipHeader(pSessionExpires);
			(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
				&pSessionExpires, Sdf_co_null);
			pSessionExpires=Sdf_co_null;
		}
	}
	/* Add Supported:Timer for INVITE if the isSupportedTimer flag in the
	 * profile is true
	 */
	if(sdf_fn_uaGetGlbIsSupportedTimer()==Sdf_co_true)
	{
		if ((dType == SipMessageRequest) || \
			(dType == SipMessageResponse))
		{
			SipHeader		*pSupportedHeader = Sdf_co_null;
			Sdf_ty_s8bit	*pOption=Sdf_co_null;
			if (sip_initSipHeader(&pSupportedHeader,SipHdrTypeSupported,\
				 (SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to init Sip header", pErr);
#endif		
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			pOption = (Sdf_ty_s8bit *) sdf_memget\
				(Sdf_mc_initMemId,10*sizeof(Sdf_ty_s8bit), Sdf_co_null);
			Sdf_mc_strcpy(pOption,"timer");
			if(sip_setOptionInSupportedHdr(pSupportedHeader,pOption, \
						 	(SipError *)&(pErr->stkErrCode))==SipFail)
		  	{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to Set Timer in Supported header", pErr);
#endif		
				sip_freeSipHeader(pSupportedHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pSupportedHeader, Sdf_co_null);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			if (sip_insertHeaderAtIndex(pMessage,pSupportedHeader,0, \
					(SipError *)&(pErr->stkErrCode)) == SipFail)
			{
#ifdef SDF_ERROR		
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_headerManipulationError, \
					(Sdf_ty_s8bit*)"sdf_fn_uaAddSessionTimerSpecificHeader( ): "
					"Failed to Set supported header in Sip Message", pErr);
#endif		
				sip_freeSipHeader(pSupportedHeader);
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pSupportedHeader, Sdf_co_null);
				pErr->errCode=Sdf_en_headerManipulationError;
				return Sdf_co_fail;
			}
			/* Freeing the Local reference */

			/* purify */
			sip_freeSipHeader(pSupportedHeader); 
		    (void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
				&pSupportedHeader, Sdf_co_null); 

		}
	}


	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaAddSessionTimerSpecificHeader");
   	return Sdf_co_success;

}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaIsActiveSessionTimer
 **
 ** DESCRIPTION: This function checks whether timer is started or not
 **				 for the Session Timer
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaIsActiveSessionTimer
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_bool *pIsActive,
	Sdf_st_error *pErr)
#else
	(pCallObj,pIsActive,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_bool *pIsActive;
	Sdf_st_error *pErr;
#endif
{
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaIsActiveSessionTimer");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaIsActiveSessionTimer():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsActiveSessionTimer():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(pIsActive == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsActiveSessionTimer():  "
			"pIsActive Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}

#endif /* End param validations ifdef */

	if(pCallObj->pSessionTimer->pTimerId == Sdf_co_null)
		*pIsActive=Sdf_co_false;
	else
		*pIsActive=Sdf_co_true;

	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaIsActiveSessionTimer");
   	return Sdf_co_success;

}


/******************************************************************************
 ** FUNCTION: 	 sdf_ivk_uaStartRefresh
 **
 ** DESCRIPTION: This function start the timer by calling 
 **				  sdf_cbk_uaStartTimer 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaStartRefresh
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	 Sdf_st_eventContext *pEventContext,
	Sdf_st_error *pErr)
#else
	(pCallObj,pEventContext,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit duration;	
	Sdf_st_sessionTimerContextInfo *pContextInfo;
	Sdf_st_eventContext *pLocalContext = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaStartRefresh");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaIsAtciveSessionTimer():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaIsAtciveSessionTimer():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	duration=pCallObj->pSessionTimer->dSessionInterval;

	/*Get the Modified Session Interval for Actual Sending Bye/re-INVITE */

	if(sdf_fn_uaModifySessionInterval(&duration,pCallObj->pSessionTimer->\
			dRefresher,pErr) ==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, \
			Sdf_en_invalidSessionInterval, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaStartRefresh(): "
			"Could not get SessionTimer Interval", pErr);
#endif
		pErr->errCode=Sdf_en_invalidSessionInterval;
		return Sdf_co_fail;
	}
#ifdef SDF_TRACE
	sdf_fn_trace(Sdf_en_allTraceLevels,Sdf_en_cbksTraces,\
		(Sdf_ty_s8bit*)"Issuing callback sdf_cbk_uaStartTimer"
		"for startin Sessiom-Timer ", 0, pErr);
#endif


	pContextInfo=(Sdf_st_sessionTimerContextInfo *)sdf_memget(\
		Sdf_mc_initMemId,sizeof(Sdf_st_sessionTimerContextInfo),pErr);

	if(pContextInfo==Sdf_co_null)	
		return Sdf_co_fail;

	HSS_INITREF(pContextInfo->dRefCount);
	
	/*increment the refcount of the call object */

	HSS_LOCKEDINCREF(pCallObj->dRefCount);
	
	pContextInfo->pCallObject=pCallObj;

	/* 
	 * Set the pContextInfo->pData with pEventContext->pData. This is required
	 * so that pEventContext->pData can be passed on to the 
	 * sdf_cbk_uaSendMsgOnNetwork while generating refresh requests after 
	 * the session-timer expires.
	 */
	if((pEventContext != Sdf_co_null) && (pEventContext->pData != Sdf_co_null))
	{
		if(sdf_ivk_uaInitEventContext(&pLocalContext, pErr) == Sdf_co_fail)
				return Sdf_co_fail;

		if(sdf_cbk_uaCloneEventContextpData(pLocalContext, pEventContext, \
					pErr) == Sdf_co_success)
	        pContextInfo->pData = pLocalContext->pData;
		else
	        pContextInfo->pData = Sdf_co_null;

		pLocalContext->pData = Sdf_co_null;
		/*w534*/(void)sdf_ivk_uaFreeEventContext(pLocalContext);
	}
	else
	    pContextInfo->pData = Sdf_co_null;

	if(sdf_cbk_uaStartTimer(duration * 1000, Sdf_en_sessionTimer, \
		(Sdf_ty_pvoid )pContextInfo, Sdf_co_null, sdf_fn_uaSessionTimerExpire,\
		(Sdf_ty_pvoid *)&(pCallObj->pSessionTimer->pTimerId),pErr)\
		==Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaStartRefresh(): "
			"Could not start the SessionTimer", pErr);
#endif
		return Sdf_co_fail;
	}
			
	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_ivk_uaStartRefresh");
   	return Sdf_co_success;

}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaStartSessionTimer
 **
 ** DESCRIPTION: This function start the timer by calling 
 **				  sdf_cbk_uaStartTimer 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaStartSessionTimer
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_ty_pvoid *ppTimerHandle,
	Sdf_ty_s32bit duration,
	Sdf_st_eventContext *pEventContext,
	Sdf_st_error *pErr)
#else
	(pCallObj,ppTimerHandle,duration,pEventContext,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_ty_pvoid *ppTimerHandle;
	Sdf_ty_s32bit duration;
	Sdf_st_eventContext *pEventContext;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_sessionTimerContextInfo *pContextInfo;
	Sdf_ty_retVal retVal;
	Sdf_st_eventContext *pLocalContext = Sdf_co_null;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaStartSessionTimer");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaStartSessionTimer():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaStartSessionTimer():  "
			"Callobject param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if(duration <=0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaStartSessionTimer():  "
			"Duration param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

#endif /* End param validations ifdef */

	pContextInfo=(Sdf_st_sessionTimerContextInfo *)sdf_memget(\
		Sdf_mc_initMemId,sizeof(Sdf_st_sessionTimerContextInfo),pErr);

	if(pContextInfo==Sdf_co_null)	
		return Sdf_co_fail;

	HSS_INITREF(pContextInfo->dRefCount);
	
	/*increment the refcount of the call object */

	HSS_LOCKEDINCREF(pCallObj->dRefCount);
	
	pContextInfo->pCallObject=pCallObj;

	/* 
	 * Set the pContextInfo->pData with pEventContext->pData. This is required
	 * so that pEventContext->pData can be passed on to the 
	 * sdf_cbk_uaSendMsgOnNetwork while generating refresh requests after 
	 * the session-timer expires.
	 */
	if((pEventContext != Sdf_co_null) && (pEventContext->pData != Sdf_co_null))
	{
		if(sdf_ivk_uaInitEventContext(&pLocalContext, pErr) == Sdf_co_fail)
				return Sdf_co_fail;

		if(sdf_cbk_uaCloneEventContextpData(pLocalContext, pEventContext, \
					pErr) == Sdf_co_success)
	        pContextInfo->pData = pLocalContext->pData;
		else
	        pContextInfo->pData = Sdf_co_null;

		pLocalContext->pData = Sdf_co_null;
		/*w534*/(void)sdf_ivk_uaFreeEventContext(pLocalContext);
	}
	else
	    pContextInfo->pData = Sdf_co_null;

	retVal = sdf_cbk_uaStartTimer(duration * 1000, Sdf_en_sessionTimer, \
		(Sdf_ty_pvoid )pContextInfo, Sdf_co_null, sdf_fn_uaSessionTimerExpire,\
		ppTimerHandle, pErr);

	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaStartSessionTimer");
	return retVal;
}
/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaStopSessionTimer
 **
 ** DESCRIPTION: This function stop the timer by calling 
 **				  sdf_cbk_uaStopTimer 
 **
 ******************************************************************************/
Sdf_ty_retVal sdf_fn_uaStopSessionTimer
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	*pTimerHandle,
	Sdf_st_error	*pErr)
#else
	(pTimerHandle,pErr)
	Sdf_ty_pvoid *pTimerHandle;
	Sdf_st_error *pErr;
#endif
{
	Sdf_st_sessionTimerContextInfo *pContextInfo=Sdf_co_null;
	Sdf_ty_retVal retVal;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_fn_uaStopSessionTimer");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_fn_uaStopSessionTimer():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(*pTimerHandle == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_fn_uaStopSessionTimer():  "
			"Timer Handle param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */


	retVal = sdf_cbk_uaStopTimer(*pTimerHandle,(Sdf_ty_pvoid *)\
		&pContextInfo, pErr);

	if (retVal == Sdf_co_success)
	{
		/*w534*/(void)sdf_fn_uaFreeSessionTimerContextInfo(pContextInfo);
		sdf_cbk_uaFreeTimerHandle (*pTimerHandle);
	}
	*pTimerHandle=Sdf_co_null;

	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting "
		"sdf_fn_uaStopSessionTimer");
	return retVal;
}

/******************************************************************************
 ** FUNCTION: 	 sdf_fn_uaGetRetransCount
 **
 ** DESCRIPTION: This function Returns the retransmission count required.
 **				 Based on the duration of the total timer expiry, this
 **				 function calculates the max retransmission count for invites.
 **					 Provide the duration in milli secs.
 **
 ******************************************************************************/
Sdf_ty_u32bit sdf_fn_uaGetRetransCount
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit 	InitDuration,
	Sdf_ty_u32bit 	TotalDuration,
	Sdf_st_error	*pErr)
#else
	(InitDuration, TotalDuration, pErr)
	Sdf_ty_u32bit InitDuration;
	Sdf_ty_u32bit TotalDuration;
	Sdf_st_error *pErr;
#endif
{
	Sdf_ty_u32bit	i = 0;
	Sdf_ty_u32bit	value = InitDuration;
	Sdf_ty_u32bit	initvalue = InitDuration;
	(void) pErr;

	do
	{
		initvalue = initvalue * 2;
		i++;
		value	  += initvalue;	
	}while( value < TotalDuration );

	return i;
}


/******************************************************************************
 ** FUNCTION:  	 sdf_ivk_uaFormRefresh
 **
 ** DESCRIPTION:	 This function will  forms a refresh re-INVITE
 **					 It will insert the same sdp as previously send
 **					 negotiated one 
 **
 ******************************************************************************/
Sdf_ty_retVal  sdf_ivk_uaFormRefresh
#ifdef ANSI_PROTO
	(Sdf_st_callObject *pCallObj,
	Sdf_st_error  *pErr)
#else	
	(pCallObj,pErr)
	Sdf_st_callObject *pCallObj;
	Sdf_st_error  *pErr;
#endif	
{
	Sdf_st_transaction *pTransaction = Sdf_co_null;
	Sdf_st_sessionParam *pSessionParams = Sdf_co_null;
	SipMsgBody *pSipMsgBody=Sdf_co_null;
	SdpMessage *pSdpMessage = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - "
		"Entering sdf_ivk_uaFormRefresh");

	/* ================ Parameter validations begin ================== */

#ifdef SDF_PARAMVALIDATION
	if(pErr == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit *)"SDF_ERROR -  "
			"sdf_ivk_uaFormRefresh():"
			"Error Param Passed is invalid ");
#endif
		return Sdf_co_fail;
	}
	if(pCallObj == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh():  "
			"pCallObj param Passed is invalid ",pErr);
#endif
		pErr->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif /* End param validations ifdef */

	/*
     * Invoke this API to check media state and verify whether a refresh offer
	 * can be initiated or not. It will take care of incrementing the 
	 * version also.
	 */
	if (sdf_ivk_uaInitiateOffer(&pSessionParams, pCallObj, Sdf_en_refresh,\
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Initiating Offer for Refreshing Session Failed.",pErr);
#endif
		return Sdf_co_fail;
	}

    /*
     * Form the INVITE by invoking sdf_ivk_uaMakeTransaction.
	 */
	if (sdf_ivk_uaMakeTransaction((Sdf_ty_s8bit *)"INVITE", pCallObj,\
			   Sdf_co_null, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Failed to make INVITE transaction",pErr);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		return Sdf_co_fail;
    }

	/*
     * Convert the Current SessionParams structure into SdpMessage.
	 */
	if (sip_initSdpMessage(&pSdpMessage, (SipError*)&(pErr->stkErrCode))\
		     == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRefresh( ): "
			"Failed to allocate memory for the SDP message", pErr);
#endif		
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaFormSdpMessageFromSessionParam(pSdpMessage, pSessionParams,\
				Sdf_en_convertLocal, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Failed to make SdpMessage from SessionParams",pErr);
#endif
	    (void)sdf_ivk_uaFreeSessionParam(pSessionParams);
		sip_freeSdpMessage(pSdpMessage);
		return Sdf_co_fail;
    }
	
	/*Set LastOffer Made*/
	if(sdf_ivk_uaSetLastOfferMade(pCallObj->pMediaInfo,pSessionParams,pErr) \
			==Sdf_co_fail)
	{
#ifdef SDF_ERROR
	    sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sessionParamAccessError,\
   			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
      		"Failed to set Last Offer Made", pErr);
#endif
		(void)sdf_ivk_uaFreeSessionParam(pSessionParams);
        return Sdf_co_fail;
	}
	/* Free the local reference. */
	(void)sdf_ivk_uaFreeSessionParam(pSessionParams);

    /*
	 * Form the SipMsgBody with the SdpMessage just extracted.
	 */
	if ((sip_initSipMsgBody(&pSipMsgBody, SipSdpBody,(SipError*)&(\
		pErr->stkErrCode))) == SipFail)
	{
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_memoryError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRefresh( ): "
			"Failed to allocate memory for the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		pErr->errCode=Sdf_en_memoryError;
		return Sdf_co_fail;
	}

	if (sip_setSdpInMsgBody(pSipMsgBody, pSdpMessage,\
			   (SipError*)&(pErr->stkErrCode)) == SipFail)
    {
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_sdpError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRefresh( ): "
			"Failed to set the SDP message in the SIP message body", pErr);
#endif		
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_sdpError;
		return Sdf_co_fail;
    }

	if (sdf_ivk_uaGetTransactionFromCallObject(pCallObj,\
			Sdf_en_uacTransaction,&pTransaction, pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR			
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_callObjectAccessError, \
			(Sdf_ty_s8bit*)"sdf_ivk_uaFormRefresh( ): "
			"Failed to get UAC transaction from call object", pErr);
#endif			
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		pErr->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}

	/*
	 * Delete all existing message bodies and store the SipMsgBody in the
	 * message body list.
	 */
	if (sdf_listDeleteAll(&(pTransaction->slMsgBodyList), pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Failed to delete the current message bodies.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	if (sdf_listAppend(&(pTransaction->slMsgBodyList), \
				 	(Sdf_ty_pvoid)pSipMsgBody, pErr) == Sdf_co_fail)
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, pErr->errCode,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Failed to append message body to slMsgBodyList",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
    }

	/*
     * Store the message body list in the SipMessage.
	 */
	if (sdf_ivk_uaSetAllMsgBody(pTransaction, Sdf_co_null, \
		pErr) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,pErr->errCode, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaFormRefresh( ):  "
			"Failed to set sdp into the SipMessage being made.",pErr);
#endif
		sip_freeSdpMessage(pSdpMessage);
		sip_freeSipMsgBody(pSipMsgBody);
		(void)sdf_ivk_uaFreeTransaction(pTransaction);
		return Sdf_co_fail;
	}

	/* Free the local references. */
	sip_freeSdpMessage(pSdpMessage);
	(void)sdf_ivk_uaFreeTransaction(pTransaction);

	pErr->errCode = Sdf_en_noUaError;
    sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaFormRequest");
   	return Sdf_co_success;

}

#endif


