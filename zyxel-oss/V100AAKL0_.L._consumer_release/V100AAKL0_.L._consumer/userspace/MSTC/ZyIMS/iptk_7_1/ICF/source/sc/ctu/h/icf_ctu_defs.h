/*******************************************************************************
*
* FILE NAME    : icf_ctu_defs.h
*
* DESCRIPTION  : Contains the definitions used in CTU service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   25-Jan-2005  Amit Sharma     None           Created Origional
*
* Copyright 2006, Aricent .
*
*******************************************************************************/

#ifndef __ICF_CTU_DEFS_H__
#define __ICF_CTU_DEFS_H__
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

#define ICF_CTU_MAX_STATE          5
#define ICF_CTU_MAX_EVENT          13

/*States in CTU service*/
#define ICF_CTU_00_IDLE                       0
#define ICF_CTU_01_WAIT_FOR_START_PROC_SIG   1
#define ICF_CTU_02_WAIT_FOR_HOLD_RESUME_RESP 2
#define ICF_CTU_03_WAIT_FOR_REFER_RESP       3
#define ICF_CTU_04_WAIT_FOR_NOTIFY		     4

/*Events handled in CTU service*/
#define ICF_EVENT_UNDEFINED                           255
#define ICF_CTU_CALL_TRANSFER_UNATTENDED_REQUEST        0
#define ICF_CTU_CALL_HOLD_RESP                          1
#define ICF_CTU_CALL_RESUME_RESP                        2
#define ICF_CTU_REFER_RESP                              3
#define ICF_CTU_NOTIFY                                  4
#define ICF_CTU_START_PROC_SIG                          5
#define ICF_CTU_MEDIA_ERROR                             6
#define ICF_CTU_CC_CALL_RELEASED                        7
#define ICF_CTU_SERVICE_ERR_IND                         8
#define ICF_CTU_TIMER_EXPIRY                            9
#define ICF_CTU_PA_CALL_RELEASED                       10
#define ICF_CTU_FORCEFUL_CALL_CLEAR                    11
/*  New Event added for XCONF Internal Refer Handling */
#define ICF_CTU_INTERNAL_REFER_REQUEST                 12

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /*  __ICF_CTU_DEFS_H__ */


