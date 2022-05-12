/*******************************************************************************
*
* FILE NAME    : icf_twc_defs.h
*
* DESCRIPTION  : Contains the definitions used in TWC service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Jan-2005  Amit Sharma     None           Created Original
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_TWC_DEFS_H__
#define __ICF_TWC_DEFS_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
#endif /* __cplusplus */

#define ICF_TWC_MAX_STATE          7
#define ICF_TWC_MAX_EVENT         11


/*States in TWC service*/
#define ICF_TWC_00_IDLE                       0
#define ICF_TWC_01_AWT_MERGE_MEDIA_RESP       1
#define ICF_TWC_02_AWT_CALL_RESUME_RESP       2
#define ICF_TWC_03_AWT_MEDIA_COMMITTED        3
#define ICF_TWC_04_AWT_CALL_HOLD_RESP         4
#define ICF_TWC_05_AWT_START_PROC_MEDIA       5
#define ICF_TWC_06_CONFERENCE                 6
#define ICF_TWC_STATE_INVALID               255

/*Events handled in TWC service*/
#define ICF_EVENT_UNDEFINED                 255
#define ICF_TWC_CREATE_CONFERENCE           0
#define ICF_TWC_MERGE_MEDIA_RESP            1
#define ICF_TWC_CALL_RESUME_RESP            2
#define ICF_TWC_MEDIA_COMMITTED             3
#define ICF_TWC_CHD_RESP                    4
#define ICF_TWC_START_PROC_MEDIA            5
#define ICF_TWC_CALL_RELEASE                6
#define ICF_TWC_TIMER_EXPIRY                7
#define ICF_TWC_MM_ERR_IND                  8
#define ICF_TWC_SERVICE_ERR_IND             9
#define ICF_TWC_FORCEFUL_CALL_CLEAR        10

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ICF_SERVICE_ENABLE */
#endif /*  __ICF_TWC_DEFS_H__ */


