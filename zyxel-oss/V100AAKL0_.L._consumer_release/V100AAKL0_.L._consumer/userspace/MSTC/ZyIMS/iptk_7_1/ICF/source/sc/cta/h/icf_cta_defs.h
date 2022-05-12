/******************************************************************************
*
* FILE NAME    : icf_cta_defs.h
*
* DESCRIPTION  : Contains the definitions used in CTA service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   18-Feb-2005  Amit Sharma     None           Created Origional
* 02-Jun-2005    Amit Sharma     SPR:7535        Wait for TWC service term
*
* Copyright 2006, Aricent.
*
*******************************************************************************/

#ifndef __ICF_CTA_DEFS_H__
#define __ICF_CTA_DEFS_H__
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

#define ICF_CTA_MAX_STATE           6
#define ICF_CTA_MAX_EVENT          11

/*States in CTA service*/
#define ICF_CTA_00_IDLE                      0
#define ICF_CTA_01_WAIT_FOR_HOLD_RESUME_RESP 1
#define ICF_CTA_02_WAIT_FOR_START_PROC_SIG   2
#define ICF_CTA_03_WAIT_FOR_REFER_RESP       3
#define ICF_CTA_04_WAIT_FOR_NOTIFY           4
#define ICF_CTA_05_WAIT_FOR_TWC_RESP         5

/*Events handled in CTA service*/
#define ICF_EVENT_UNDEFINED                           255
#define ICF_CTA_CALL_TRANSFER_ATTENDED_REQUEST          0
#define ICF_CTA_CALL_HOLD_RESP                          1
#define ICF_CTA_CALL_RESUME_RESP                        2
#define ICF_CTA_REFER_RESP                              3
#define ICF_CTA_NOTIFY                                  4
#define ICF_CTA_START_PROC_SIG                          5
#define ICF_CTA_CC_CALL_RELEASED                        6
#define ICF_CTA_TIMER_EXPIRY                            7
#define ICF_CTA_PA_CALL_RELEASED                        8
#define ICF_CTA_FORCEFUL_CALL_CLEAR                     9
#define ICF_CTA_TWC_RESP                               10

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_CTA_DEFS_H__ */
