/******************************************************************************
 *
 *
 * FILE NAME:   icf_sic_defs.h
 *
 * DESCRIPTION: This file contains the hash defines, enums etc used in SIC.
 *
 * REVISION HISTORY:
 *      Name            Date            Reference           Comments
 *  Syed Ali Rizvi  02-Aug-2005        LLD                Initial
 *
 * Copyright (c) 2006, Aricent
 *
 ******************************************************************************/


#ifndef __ICF_SIC_DEFS_H__
#define __ICF_SIC_DEFS_H__

#include "icf_common_types.h"
#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/*------------------------------------------------------------------------------
 *
 * Enum for the events of SIC Subscription State Machine
 *
 *----------------------------------------------------------------------------*/
typedef enum
{
    ICF_SIC_EVENT_INVALID=0, 
    ICF_SIC_EVENT_SUBS_REQ,
    ICF_SIC_EVENT_UNSUB_REQ,
    ICF_SIC_EVENT_SUCCESS_RESP,
    ICF_SIC_EVENT_FAILURE_RESP_NO_RETRY,
    ICF_SIC_EVENT_FAILURE_RESP_WITH_RETRY,
    ICF_SIC_EVENT_NOTIFY_ACTIVE,
    ICF_SIC_EVENT_NOTIFY_TERM_NO_RETRY,
    ICF_SIC_EVENT_NOTIFY_TERM_RETRY_IMM,
    ICF_SIC_EVENT_NOTIFY_TERM_RETRY_AFTER,
    ICF_SIC_EVENT_RESP_TIMER_EXP,
    ICF_SIC_EVENT_DURATION_TIMER_EXP,
    ICF_SIC_EVENT_RETRY_TIMER_EXP,
	ICF_SIC_EVENT_AWAIT_SUBS_NOTIFY_TIMER_EXP,
    ICF_SIC_EVENT_FORCEFUL_DELETE,
    ICF_SIC_MAX_EVENTS
} icf_sic_events_et;


/* ICF_6_1_OPTIONS_SUPPORT_START */
/*------------------------------------------------------------------------------
 *
 * Enum for the events of SIC OPTIONS State Machine
 *
 *----------------------------------------------------------------------------*/

typedef enum
{
	ICF_SIC_EVENT_INC_OPTIONS_REQ = 0,
	ICF_SIC_EVENT_OTG_OPTIONS_REQ,
	ICF_SIC_EVENT_MEDIA_CAPAB_RSP,
	ICF_SIC_EVENT_OPTIONS_APP_CFM,
	ICF_SIC_EVENT_OPTIONS_NW_RSP,
	ICF_SIC_EVENT_TIMER_EXPIRY,
	ICF_SIC_EVENT_CLEAR_OPTIONS_CTX,
	ICF_SIC_MAX_OPTIONS_EVENTS
} icf_sic_options_events_et;

/* ICF_6_1-OPTIONS_SUPPORT_END */

/* After receiving 200 ok for poll_subscribe/unsbscribe request 
 * SIC will wait for 128000 milliseconds for Notify
 * request. 
 */
#define ICF_SIC_AWAIT_NOTIFY_REQ		128000
#define ICF_SIC_AWAIT_NOTIFY_CFM		60000

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SIC_DEFS_H__ */
