/*------------------------------------------------------------------------------
 *
 * File name        : icf_twc_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 6-Dec-2005  Amit Sharma                           Initial
 *12-Apr-2005  Amit Sharma                           TRACE macro wasnot in #define
 * 27-Feb-2006  Amit Sharma                          Merged code for ICF
 *
 *
 * Copyright (c) 2006, Aricent
 *----------------------------------------------------------------------------*/


#ifndef __ICF_TWC_MACRO_H__
#define __ICF_TWC_MACRO_H__
#include "icf_map_extif_feature_flags.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_twc_types.h"
#include "icf_twc_prototypes.h"

/*
 * Determine if a C++ compiler is being used. If so, ensure that standard C is
 * used to process the header information.
 */
#ifdef __cplusplus
/* Yes, C++ compiler is present. Use standard C */
extern "C"
{
#endif /* __cplusplus */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_TWC_TRACE(p_pdb, twc_trace_id) \
{\
    icf_twc_trace(p_pdb, (icf_trace_id_t)(twc_trace_id)); \
} /* #define ICF_TWC_TRACE(p_pdb... */
#else
#define ICF_TWC_TRACE(p_pdb, twc_trace_id)
#endif  

#ifdef ICF_TRACE_ENABLE
#define ICF_TWC_APP_TRACE(p_pdb, twc_trace_id) \
{\
    icf_twc_app_trace(p_pdb, (icf_trace_id_t)(twc_trace_id)); \
} /* #define ICF_TWC_APP_TRACE(p_pdb... */
#else
#define ICF_TWC_APP_TRACE(p_pdb, twc_trace_id)

#endif
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_SET_CURR_STATE
 * Description      : Gives the current call state from call context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_SET_CURR_STATE(p_pdb, twc_state) \
 {\
     p_pdb->p_twc_ctx->state = twc_state; \
 } /* #define ICF_TWC_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_GET_SIG_ONGOING
 * Description      : Get signaling_ongoing in call context context with given
 *                    module id
 *
 *----------------------------------------------------------------------------*/
#define ICF_TWC_GET_SIG_ONGOING(p_pdb)  p_pdb->p_call_ctx->signalling_ongoing


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_SET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status |= (icf_uint32_t)status_id; \
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_TWC_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_CHECK_STATUS
 * Description      : Get current return status from internal msg 
 *
 *----------------------------------------------------------------------------*/
#define ICF_TWC_CHECK_STATUS(p_pdb, status_id)  \
            (p_pdb->p_internal_msg->msg_hdr.status & status_id)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_RESET_STATUS
 * Description      : Reset status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_RESET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status &= ~status_id;\
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_TWC_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_GET_MEDIA_ONGOING
 * Description      : Get media_ongoing in call context with given
 *                    module id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_GET_MEDIA_ONGOING(p_call_ctx)  p_call_ctx->media_ongoing

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_SET_NEXT_STATE
 * Description      : Set next_state in TWC service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_SET_NEXT_STATE(p_pdb, next_st) \
 {\
     p_pdb->p_twc_ctx->next_state = next_st; \
    \
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_NEXT_STATE) \
 } /* #define ICF_TWC_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_GET_NEXT_STATE
 * Description      : Get next_state from TWC service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_GET_NEXT_STATE(p_pdb)  p_pdb->p_twc_ctx->next_state
 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_RESET_NEXT_STATE
 * Description      : Reset next_state in TWC service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_RESET_NEXT_STATE(p_pdb) \
 {\
     p_pdb->p_twc_ctx->next_state = ICF_TWC_STATE_INVALID; \
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_NEXT_STATE) \
 } /* #define ICF_TWC_SET_CURR_ST... */


 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_SET_PEND_EVT
 * Description      : Set pending_event in TWC service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_SET_PEND_EVT(p_pdb, pend_evt) \
 { \
     p_pdb->p_twc_ctx->pending_event = pend_evt; \
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_PENDING_EVT) \
 } /* #define ICF_TWC_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_GET_PEND_EVT
 * Description      : Get pending_event from TWC service logic context
 *
 *----------------------------------------------------------------------------*/
#define ICF_TWC_GET_PEND_EVT(p_pdb) p_pdb->p_twc_ctx->pending_event

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_TWC_RESET_PEND_EVT
 * Description      : RESet pending_event in TWC service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_TWC_RESET_PEND_EVT(p_pdb) \
 {\
     p_pdb->p_twc_ctx->pending_event = ICF_TWC_PE_INVALID; \
     ICF_TWC_TRACE(p_pdb, ICF_TRACE_PENDING_EVT) \
 } /* #define ICF_TWC_SET_CURR_ST... */

#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif /* ICF_SERVICE_ENABLE */
#endif /*  __ICF_TWC_MACRO_H__ */

