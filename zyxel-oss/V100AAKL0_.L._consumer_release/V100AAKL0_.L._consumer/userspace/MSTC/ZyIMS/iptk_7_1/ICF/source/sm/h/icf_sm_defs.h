/*---------------------------------------------------------------------------
 * File name        : icf_sm_defs.h
 *     
 * Purpose          : This file contains certain hash defines used by SM.
 *
 * Revision History :
 *
 * Date         Name                Ref#        Comments
 * --------     ------------        ----        ---------------
 * 09-Mar-2005  Jyoti Narula        SM LLD      Beginning of coding phase
 * 10-Jun-2005  Ram Narayan                     Changed the absolute path of 
 *                                              licence to a relative path which 
 *                                              would be installation directory of
 *                                              ICF
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SM_DEFS_H__
#define __ICF_SM_DEFS_H__

#ifdef ICF_STAT_ENABLED
#include "icf_stats_defs.h"
#endif


/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

/*---------------------------------------------------------------------------
 * Maximum number of events generated within SM Handler. 
 * -------------------------------------------------------------------------*/

#define ICF_SM_MAX_EVENT_COUNT  9


/*--------------------------------------------------------------------------
 *
 * Hash defines for the events of SM Handler: 
 * Each generated on receipt of API from SM through MRM.
 * 
---------------------------------------------------------------------------- */

#define ICF_SM_SET_TRACE_LVL     0
#define ICF_SM_SET_ERROR_LVL     1
#define ICF_SM_SET_STATS         2
#define ICF_SM_GET_STATS         3
#define ICF_SM_CHK_MODULE        4
#define ICF_SM_CALL_LOG          5
#define ICF_SM_TIMER_EXPIRY      6
#define ICF_SM_STATS             7
#define ICF_SM_SET_TRACE_REQ     8

/*---------------------------------------------------------------------
 * Maximum number of stats_groups_ids present in the array
 * of utiltiy function get_stats_index()
 * Uptil Now(April 2005) stats are reported
 * only for 4 group_stats_id
 * 
 * -------------------------------------------------------------------*/

#define ICF_SM_MAX_STATS_GROUP_COUNT 5

/*---------------------------------------------------------------------
 * Uptil Now(April 2005) Because 4 stats_group are reported for:
 * Maximum value of stats_id to be checked for validation during
 * GET_STATS Handling is less than the MAX_ID_VAL as given in 
 * the stats_defs.h
 * -------------------------------------------------------------------*/

#define ICF_SM_STAT_MAX_ID_VAL ICF_STAT_GROUP_TMR_RELATED


/*--------------------------------------------------------------------------
 * Hash define for Invalid Index of the Call History Lists
 * (Missed/dialed/received Logs)
 * ---------------------------------------------------------------------*/
#define ICF_SM_INVALID_INDEX ICF_MAX_NUM_OF_CALLS_TO_LOG + 1

    
/*--------------------------------------------------------------------------
 * Hash defines for Validating the parameters.
 * ---------------------------------------------------------------------*/
#define ICF_SM_TRACE_LEVEL_START ICF_TRACE_LEVEL_UNDEFINED 
#define ICF_SM_TRACE_LEVEL_END  ICF_TRACE_LEVEL_DETAILED + 1

#define ICF_SM_ERROR_LEVEL_START ICF_ERROR_LEVEL_UNDEFINED 
#define ICF_SM_ERROR_LEVEL_END   ICF_ERROR_LEVEL_INFO + 1


/***************************************************************
 *          Hash Defines for License Management 
 ****************************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of __ICF_SM_DEFS_H__ */


