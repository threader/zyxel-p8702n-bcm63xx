
/***********************************************************************
 ** FUNCTION: 	 This file contains trace related APIs
 **             
 ***********************************************************************
 **
 ** FILENAME: 	 sdf_trace.h
 **
 ** DESCRIPTION: This file contains prototypes for all trace related 
 **				 functions
 **
 **
 **	DATE        NAME                    REFERENCE               REASON
 ** ----        ----                    ---------              --------
 ** 23/02/01   Rajasri				Initial Creation    	
 **
 **
 ************************************************************************
 **     		COPYRIGHT , Aricent, 2006
 ***********************************************************************/


#include "microsip_portlayer.h"
#include "sdf_common.h"
#include "sdf_struct.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
#ifndef __SDF_TRACE_H_
#define __SDF_TRACE_H_

/***********************************************************************
 ** FUNCTION	: 	sdf_ivk_uaSetTraceLevel
 **
 ** DESCRIPTION	:  	This functions is used to set the trace level
 **					for the UA Toolkit.
 **
 ** PARAMETERS
 **  dTraceLevel (IN)	: Trace level to set. Valid values are:
 **						  Sdf_en_briefTraces, Sdf_en_detailedTraces or
 ** 					  Sdf_en_uaAllTraces.
 **  pErr		 (OUT)	: Error structure filled in case of failure.
 ** 
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetTraceLevel _ARGS_((Sdf_ty_uaTraceLevel \
	dTraceLevel, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION: sdf_ivk_uaGetTraceLevel
 **
 ** DESCRIPTION:  This functions is used to get the current trace level.
 **
 ** PARAMETERS
 **  pTraceLevel (OUT)	: Current trace level is returned in this variable.
 **  pErr		 (OUT)	: Error structure filled in case of failure.
 ** 
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetTraceLevel _ARGS_((Sdf_ty_uaTraceLevel \
	*pTraceLevel, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION: sdf_ivk_uaSetTraceType
 **
 ** DESCRIPTION:  This functions is used to set the trace type required
 **
 ** PARAMETERS
 **  dTraceType	(IN)	: Trace type to be set. Valid values are:
 **						  Sdf_en_noTraces / Sdf_en_initTraces /
 ** 					  Sdf_en_mesgTraces / Sdf_en_configTraces /
 **						  Sdf_en_cbksTraces / Sdf_en_authTraces /
 **						  Sdf_en_mediaTraces / Sdf_en_callStateTraces /
 **						  Sdf_en_unknownHeaderTraces / 
 **						  Sdf_en_remoteRetransTraces or Sdf_en_uaAllTraces
 **  pErr		 (OUT)	: Error structure filled in case of failure.
 **
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_uaSetTraceType _ARGS_((Sdf_ty_traceType \
	dTraceType, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION: sdf_ivk_uaGetTraceType
 **
 ** DESCRIPTION:  This functions is used to get the current trace type.
 **
 ** PARAMETERS
 **  pTraceType	(OUT)	: Current trace type is returned in this variable.
 **	 pErr		(OUT)	: Error structure filled in case of failure.
 ** 
 ***********************************************************************/
  Sdf_ty_retVal sdf_ivk_uaGetTraceType _ARGS_((Sdf_ty_traceType \
	*pTraceType, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_fn_trace
 **
 ** DESCRIPTION: For tracing the path of execution. This prints trace 
 **				 level comments on using the SDF_TRACE flag as a compile 
 **				 time option.
 ** 
 ** PARAMETERS
 **  dLevel		(IN) 	: Level of trace (brief/detailed)
 **  dType		(IN)	: Type of trace: initalization/media related etc.
 **  pStr		(IN)	: string to be printed/logged in the trace
 **  dLen		(IN)	: length of the string (pStr)
 **	 pErr		(OUT)	: Error structure filled in case of failure.
 **
 ***********************************************************************/
  void sdf_fn_trace _ARGS_((Sdf_ty_uaTraceLevel dLevel, Sdf_ty_traceType \
	dType, const Sdf_ty_s8bit *pStr, Sdf_ty_u32bit dLen, Sdf_st_error *pErr));

/***********************************************************************
 ** FUNCTION:  sdf_fn_getStateName
 **
 ** DESCRIPTION: This function returns the CSM state as a string
 ** 			 Used to print out traces of type Sdf_en_callStateTraces 
 **
 **	PARAMETERS
 **  dState	(IN)  :	State variable for which string has to be got.
 **  pState (OUT) :	string returned by the API.
 **  dLen	 (IN) :	Length of the output string.
 **  pErr 	(OUT) :	Error variable.
 **
 ***********************************************************************/
  Sdf_ty_retVal sdf_fn_getStateName _ARGS_((Sdf_ty_state dState, \
	Sdf_ty_s8bit *pState,Sdf_ty_u32bit dLen, Sdf_st_error *pErr));
	
#endif  /*SDF__TRACE_H_ */
/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif 
