/*****************************************************************************
*
* FILE NAME    : icf_rnw_defs.h
*
* DESCRIPTION  : Contains the definitions used in RNW service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   03-Mar-2005  Amit Sharma     None           Created Origional
*
* Copyright 2006, Aricent .
*
*******************************************************************************/

#ifndef __ICF_RNW_DEFS_H__
#define __ICF_RNW_DEFS_H__
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

#define ICF_RNW_MAX_STATE           8
#define ICF_RNW_MAX_EVENT          13

/*States in RNW service*/
#define ICF_RNW_STATE_INVALID              255
#define ICF_RNW_00_IDLE                      0
#define ICF_RNW_01_AWT_MEDIA_COMMIT          1
#define ICF_RNW_02_AWT_START_PROC_MEDIA      2
#define ICF_RNW_03_AWT_MERGE_MEDIA_RESP      3
#define ICF_RNW_04_AWT_FOR_CONNECT           4
#define ICF_RNW_05_AWT_START_PROC_SIGNALING  5
#define ICF_RNW_06_AWT_NOTIFY_ACK            6
#define ICF_RNW_07_AWT_FOR_CTA_INITIATED_CFM 7

/*Events handled in RNW service*/
#define ICF_EVENT_UNDEFINED                           255
#define ICF_RNW_REFER                                   0
#define ICF_RNW_START_PROC_SIGNALING                    1
#define ICF_RNW_START_PROC_MEDIA                        2
#define ICF_RNW_NOTIFY_SUCCESS_RESP                     3
#define ICF_RNW_MEDIA_COMMITTED                         4
#define ICF_RNW_MERGE_MEDIA_RESP                        5
#define ICF_RNW_INVITE_SUCCESS_RESP                     6
#define ICF_RNW_MEDIA_ERROR                             7
#define ICF_RNW_CALL_RELEASED                           8
#define ICF_RNW_TIMER_EXPIRY                            9
#define ICF_RNW_FORCEFUL_CALL_CLEAR                    10
#define ICF_RNW_INTERNAL_RNW_REJ                       11
#define ICF_RNW_CTA_INITIATED_CFM                      12


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_RNW_DEFS_H__ */

