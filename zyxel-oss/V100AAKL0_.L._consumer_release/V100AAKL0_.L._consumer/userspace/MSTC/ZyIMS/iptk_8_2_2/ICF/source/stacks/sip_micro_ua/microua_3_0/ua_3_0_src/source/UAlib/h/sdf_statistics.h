
/**********************************************************************
 ** FUNCTION:
 **	 	This file has all the APIs related to manipulation of statistics
 **
 **********************************************************************
 **
 ** FILENAME: 		sdf_statistics.h
 **
 ** DESCRIPTION:	All APIs related to the statistics are described 
 **					in this file.
 **
 **
 **  DATE			NAME			REFERENCE		REASON
 **  ----			----			--------		------
 ** 10/03/01 		Seshashayi						Creation
 **
 **********************************************************************
 **			COPYRIGHT , Aricent, 2006
 **********************************************************************/


#ifndef __SDF_STATISTICS_H__
#define __SDF_STATISTICS_H__

#include "sdf_struct.h"
#include "sdf_common.h"

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 
/*********************************************************************
 ** FUNCTION: 	 sdf_ivk_uaInitStatistics 
 **
 ** DESCRIPTION: Initialise the statistics structure of the UA 
 **				 Toolkit to all 0s
 **
 ** Parameters:
 **	  dStatsType (IN)	: The type of statistics to be initialised
 **	  pErr  	 (OUT)	: Error structure in case the function fails
 **
 *********************************************************************/
 extern Sdf_ty_retVal sdf_ivk_uaInitStatistics _ARGS_(( \
		Sdf_ty_statsType dStatsType,Sdf_st_error *pErr));

/*********************************************************************
 ** FUNCTION:  sdf_fn_uaIncrementCallStats
 **
 ** DESCRIPTION: Increment the corresponding callType count 
 **
 ** PARAMETERS:
 **	  dCallType	(IN)	: The type of call statistics which has to be
 **						  incremented.
 **	  pErr		(OUT)	: Error structure returned in case the function 
 **						  fails
 **
 *********************************************************************/
 extern Sdf_ty_retVal sdf_fn_uaIncrementCallStats _ARGS_(( \
	Sdf_ty_callStats dStatsType,Sdf_st_error *pErr));

/*********************************************************************
 ** FUNCTION:  	 sdf_ivk_uaGetStatistics
 **
 ** DESCRIPTION: get statistics for the corresponding stats type
 **
 ** PARAMETERS:
 **		dStatsType	(IN)  : The type of statistics structure to be got
 **		dStatsScope	(IN)  : The specific type in the statistics structure.
 **		dReset		(IN)  : A Flag indicating whether that type has to 
 **							be reset to 0
 **		pStats		(IN/OUT) : The statistics value is returned in this
 **		pErr		(OUT) : Error structure in case the function fails
 **
 *********************************************************************/
 extern Sdf_ty_retVal sdf_ivk_uaGetStatistics _ARGS_(( \
	Sdf_ty_statsType dStatsType,Sdf_ty_s32bit dStatsScope,\
	Sdf_ty_bool dReset, Sdf_ty_pvoid pStats, 
	Sdf_st_error *pErr));


/*****************************************************************
** FUNCTION:  sdf_fn_uaUpdateStatistics
**
** DESCRIPTION: Updates the statistics based on the current state
**				and the next state to which the call object is
**				transiting
**
*****************************************************************/
Sdf_ty_retVal sdf_fn_uaUpdateStatistics _ARGS_ ((Sdf_ty_msgMode dMode, \
	Sdf_ty_state dCurrState, Sdf_ty_messageType dMsgType, Sdf_st_error *pErr));

#ifdef __cplusplus
}
#endif
#endif
