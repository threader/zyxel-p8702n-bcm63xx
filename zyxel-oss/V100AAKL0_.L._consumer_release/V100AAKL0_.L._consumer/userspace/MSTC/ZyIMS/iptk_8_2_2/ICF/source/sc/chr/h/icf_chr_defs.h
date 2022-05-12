/*******************************************************************************
*
* FILE NAME    : icf_chr_defs.h
*
* DESCRIPTION  : Contains the definitions used in CHR service
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
*   6-Dec-2004  Amit Sharma     None           Created Origional
*  20-Apr-2005  Amit Sharma                    Added race timer flags
*  11-Jul-2007  Neha Choraria   ICF 7.0        Commented a part of code
*                                              for changes in retry-after
*
* Copyright 2006,Aricent.
*
*******************************************************************************/

#ifndef __ICF_CHR_DEFS_H__
#define __ICF_CHR_DEFS_H__
/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
#endif /* __cplusplus */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#define ICF_CHR_MAX_STATE          6
#define ICF_CHR_MAX_EVENT         13

/****** Code Below Shifted to common for changes
 in retry-after (Rel 7.0) ******/

/*
#define ICF_CHR_INC_RACE_TIMER_LOW_LIMIT    0
#define ICF_CHR_INC_RACE_TIMER_UP_LIMIT     2000
#define ICF_CHR_OG_RACE_TIMER_LOW_LIMIT     2100
#define ICF_CHR_OG_RACE_TIMER_UP_LIMIT      4000
*/

#define ICF_ERR_NO_ERROR   0X00

/*States in CHR service*/
#define ICF_CHR_00_IDLE                       0
#define ICF_CHR_01_AWT_MMS_RESP               1
#define ICF_CHR_02_AWT_NW_ACK                 2
#define ICF_CHR_03_AWT_CANCEL_RESP            3
#define ICF_CHR_04_AWT_ST_PROC_SIG            4
#define ICF_CHR_05_AWT_ST_PROC_MEDIA          5

/*Events handled in CHR service*/
#define ICF_EVENT_UNDEFINED                255
#define ICF_CHR_CALL_HOLD                   0
#define ICF_CHR_MMS_RESP                    1
#define ICF_CHR_MEDIA_ERROR                 2
#define ICF_CHR_NW_HLD_RSM_SUC_RESP         3
#define ICF_CHR_CALL_RESUME                 4
#define ICF_CHR_HLD_RSM_REQ_FROM_NW         5
#define ICF_CHR_NW_ACK_FOR_REQ_CNL          6
#define ICF_CHR_NW_OG_SETUP_FAIL            7
#define ICF_CHR_TIMER_EXPIRY                8
#define ICF_CHR_CALL_RELEASED               9
#define ICF_CHR_ST_PROC_SIG                10
#define ICF_CHR_ST_PROC_MEDIA              11
#define ICF_CHR_FORCEFUL_CALL_CLEAR        12
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*  __ICF_CHR_DEFS_H__ */

