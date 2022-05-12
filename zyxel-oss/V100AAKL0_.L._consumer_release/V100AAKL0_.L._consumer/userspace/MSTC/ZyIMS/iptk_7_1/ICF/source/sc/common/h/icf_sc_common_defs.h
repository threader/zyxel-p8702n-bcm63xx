/*------------------------------------------------------------------------------
 *
 * File name        : icf_common_defs.h
 *
 * Purpose          : This file contains the definitions of SCM module.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 15-Dec-2004  Amit Sharma                           Initial
 *  1-Feb-2004  Amit Sharma                         To add status of both calls 
 *                                                  and other all
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SC_COMMON_DEFS_H__
#define __ICF_SC_COMMON_DEFS_H__

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */

#define ICF_STATUS_FAILURE                               0x0000
#define ICF_STATUS_SUCCESS                               0x0001
#define ICF_STATUS_SERVICE_CONTINUE                      0x0002
#define ICF_STATUS_SERVICE_ENDED_NORMAL                  0x0004
#define ICF_STATUS_SERVICE_ENDED_ERROR                   0x0008
#define ICF_STATUS_SERVICE_MSG_TO_CC_SETUP               0x0010
#define ICF_STATUS_SERVICE_CALL_HOLD_REQ                 0x0020
#define ICF_STATUS_SERVICE_CALL_RESUME_REQ               0x0040
#define ICF_STATUS_SERVICE_CALL_MERGE_REQ                0x0080
#define ICF_STATUS_SERVICE_CALL_PROC_SIGNALING_REQ       0x0100
#define ICF_STATUS_SERVICE_CALL_PROC_MEDIA_REQ           0x0200
#define ICF_STATUS_SERVICE_SIGNALING_EVENTS_REQ          0x0400
#define ICF_STATUS_SERVICE_MEDIA_EVENTS_REQ              0x0800
#define ICF_STATUS_SERVICE_CALL_HOLD_RESP                0x1000
#define ICF_STATUS_SERVICE_CALL_RESUME_RESP              0x2000
#define ICF_STATUS_SERVICE_APPLY_BOTH_CALLS              0x4000
#define ICF_STATUS_SERVICE_REQ_FOR_OTHER_CALL            0x8000

#define ICF_INVALID_MODULE_ID 0X00
#define ICF_SERVICE_INVALID   0x00
#define ICF_SERVICE_CHR       0x01
#define ICF_SERVICE_TWC       0x02    
#define ICF_SERVICE_CTU       0x04
#define ICF_SERVICE_CTA       0x08
#define ICF_SERVICE_RNW       0x10

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
