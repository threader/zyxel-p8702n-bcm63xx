/*-----------------------------------------------------------------------------
 *
 * File name        : icf_ctu_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 25-Jan-2005  Amit Sharma                           Initial
 * 27-Feb-2006  Amit Sharma                     ICF Merging with icf rel2.0
 *
 *
 * Copyright (c) 2006, Aricent .
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CTU_MACRO_H__
#define __ICF_CTU_MACRO_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_ctu_types.h"
#include "icf_ctu_prototypes.h"

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
 * Macro Name       : ICF_CTU_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CTU_TRACE(p_pdb, ctu_trace_id) \
{\
    icf_ctu_trace(p_pdb, (icf_trace_id_t)(ctu_trace_id)); \
} /* #define ICF_CTU_TRACE(p_pdb... */
#else
#define ICF_CTU_TRACE(p_pdb, ctu_trace_id)
#endif
        

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_APP_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CTU_APP_TRACE(p_pdb, ctu_trace_id) \
{\
    icf_ctu_app_trace(p_pdb, (icf_trace_id_t)(ctu_trace_id)); \
} /* #define ICF_CTU_APP_TRACE(p_pdb... */
#else
#define ICF_CTU_APP_TRACE(p_pdb, ctu_trace_id)
#endif
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_SET_CURR_STATE
 * Description      : Gives the current call state from call context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_SET_CURR_STATE(p_pdb, ctu_state) \
 {\
     p_pdb->p_ctu_ctx->state = (icf_uint8_t)ctu_state; \
 } /* #define ICF_CTU_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_GET_SIG_ONGOING
 * Description      : Get signaling_ongoing in call context context with given
 *                    module id
 *
 *----------------------------------------------------------------------------*/
#define ICF_CTU_GET_SIG_ONGOING(p_pdb)  p_pdb->p_call_ctx->signalling_ongoing


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_SET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status |= status_id; \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_CTU_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_SET_STATUS_2
 * Description      : Set current return status in internal msg with given
 *                    status id(Only for new status added in Rel 7.0)
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_SET_STATUS_2(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status_2 |= status_id; \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_CTU_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_CHECK_STATUS
 * Description      : Get current return status from internal msg 
 *
 *----------------------------------------------------------------------------*/
#define ICF_CTU_CHECK_STATUS(p_pdb, status_id)  \
            (p_pdb->p_internal_msg->msg_hdr.status & status_id)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_RESET_STATUS
 * Description      : Reset status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_RESET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status &= ~status_id;\
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_CTU_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_GET_MEDIA_ONGOING
 * Description      : Get media_ongoing in call context with given
 *                    module id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_GET_MEDIA_ONGOING(p_pdb)  p_pdb->p_call_ctx->media_ongoing

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_SET_ONGOING_ACT
 * Description      : Set ongoing_action in CTU service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_SET_ONGOING_ACT(p_pdb, ong_act) \
 {\
     p_pdb->p_ctu_ctx->ongoing_action = ong_act; \
    \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_ONGOING_ACT) \
 } /* #define ICF_CTU_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_GET_ONGOING_ACT
 * Description      : Get ongoing_action from CTU service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_GET_ONGOING_ACT(p_pdb)  p_pdb->p_ctu_ctx->ongoing_action
 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_RESET_ONGOING_ACT
 * Description      : Reset ongoing_action in CTU service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_RESET_ONGOING_ACT(p_pdb) \
 {\
     p_pdb->p_ctu_ctx->ongoing_action = ICF_CTU_OA_INVALID; \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_ONGOING_ACT) \
 } /* #define ICF_CTU_SET_CURR_ST... */


 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_SET_PEND_EVT
 * Description      : Set pending_event in CTU service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_SET_PEND_EVT(p_pdb, pend_evt) \
 { \
     p_pdb->p_ctu_ctx->pending_event = pend_evt; \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_PENDING_EVT) \
 } /* #define ICF_CTU_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_GET_PEND_EVT
 * Description      : Get pending_event from CTU service logic context
 *
 *----------------------------------------------------------------------------*/
#define ICF_CTU_GET_PEND_EVT(p_pdb) p_pdb->p_ctu_ctx->pending_event

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CTU_RESET_PEND_EVT
 * Description      : RESet pending_event in CTU service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_CTU_RESET_PEND_EVT(p_pdb) \
 {\
     p_pdb->p_ctu_ctx->pending_event = ICF_CTU_PE_INVALID; \
     ICF_CTU_TRACE(p_pdb, ICF_TRACE_PENDING_EVT) \
 } /* #define ICF_CTU_SET_CURR_ST... */

#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_CTU_MACRO_H__ */

