/*----------------------------------------------------------------------------
 *
 * File name        : icf_xconf_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name              Ref#                Comments
 * --------     ------------      ----                ---------------
 * 4-Jul-2007  Abhishek Dhammawat CDD Rel7.0           Created Origional
 *
 *
 * Copyright (c) 2007, Aricent .
 *----------------------------------------------------------------------------*/


#ifndef __ICF_XCONF_MACRO_H__
#define __ICF_XCONF_MACRO_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_xconf_types.h"
#include "icf_xconf_prototypes.h"
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
 * Macro Name       : ICF_XCONF_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_XCONF_TRACE(p_pdb, xconf_trace_id) \
{\
    icf_xconf_trace(p_pdb, (icf_trace_id_t)(xconf_trace_id)); \
} /* #define ICF_XCONF_TRACE(p_pdb... */
#else
#define ICF_XCONF_TRACE(p_pdb, xconf_trace_id)
#endif
        

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_SET_CURR_STATE
 * Description      : Gives the current call state from call context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_SET_CURR_STATE(p_pdb, xconf_state) \
 {\
     p_pdb->p_xconf_ctx->state = (icf_uint8_t)xconf_state; \
 } /* #define ICF_XCONF_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_SET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status |= status_id; \
     ICF_XCONF_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_XCONF_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_SET_STATUS_2
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_SET_STATUS_2(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status_2 |= status_id; \
     ICF_XCONF_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_XCONF_SET_CURR_ST... */
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_CHECK_STATUS
 * Description      : Get current return status from internal msg 
 *
 *----------------------------------------------------------------------------*/
#define ICF_XCONF_CHECK_STATUS(p_pdb, status_id)  \
            (p_pdb->p_internal_msg->msg_hdr.status & status_id)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_RESET_STATUS
 * Description      : Reset status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_RESET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status &= ~status_id;\
     ICF_XCONF_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_XCONF_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_RESET_STATUS_2
 * Description      : Reset status_2 in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_RESET_STATUS_2(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status_2 &= ~status_id;\
 } /* #define ICF_XCONF_SET_CURR_ST... */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_SET_ONGOING_ACT
 * Description      : Set ongoing_action in XCONF service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_SET_ONGOING_ACT(p_pdb, ong_act) \
 {\
     p_pdb->p_xconf_ctx->ongoing_action = ong_act; \
    \
     ICF_XCONF_TRACE(p_pdb, ICF_TRACE_ONGOING_ACT) \
 } /* #define ICF_XCONF_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_GET_ONGOING_ACT
 * Description      : Get ongoing_action from XCONF service logic context 
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_GET_ONGOING_ACT(p_pdb)  p_pdb->p_xconf_ctx->ongoing_action
 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_XCONF_RESET_ONGOING_ACT
 * Description      : Reset ongoing_action in XCONF service logic context
 *
 *----------------------------------------------------------------------------*/
 #define ICF_XCONF_RESET_ONGOING_ACT(p_pdb) \
 {\
     p_pdb->p_xconf_ctx->ongoing_action = ICF_XCONF_OA_INVALID; \
     ICF_XCONF_TRACE(p_pdb, ICF_TRACE_ONGOING_ACT) \
 } /* #define ICF_XCONF_SET_CURR_ST... */


#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_XCONF_MACRO_H__ */
