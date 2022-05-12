/******************************************************************************
 *
 * FILE NAME:   icf_sic_macro.h
 *
 * DESCRIPTION: This file contains the macros used in SIC.
 *
 * REVISION HISTORY:
 *              Date            Name            Reference       Comments
 *          11-Aug-2005    Syed Ali Rizvi       LLD      Initial
 *
 * Copyright (c) 2006, Aricent.
 ******************************************************************************/

#ifndef __ICF_SIC_MACRO_H__
#define __ICF_SIC_MACRO_H__

#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_sic_types.h"
/*#include "icf_trace_id.h"*/

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */

/* ----------------------------------------------------------------------------
 * Macro to reset the buffer in the SIC ctx
 * --------------------------------------------------------------------------*/
#define ICF_SIC_RESET_BUFFER(p_sic_pdb)                                \
{                                                                           \
    p_sic_pdb->p_sic_ctx->event_buffer = ICF_FALSE;                    \
}


/* ---------------------------------------------------------------------------
 * Trace Macro
 *--------------------------------------------------------------------------*/

#define ICF_SIC_TRACE(p_sic_pdb,trace_id)\
{\
    icf_sic_trace(p_sic_pdb,(icf_trace_id_t)(trace_id));\
}

#ifdef ICF_TRACE_ENABLE
/* ---------------------------------------------------------------------------
 * Trace Macro
 *--------------------------------------------------------------------------*/

#define ICF_SIC_APP_TRACE(p_sic_pdb,trace_id)\
{\
    icf_sic_app_trace(p_sic_pdb,(icf_trace_id_t)(trace_id));\
}
#else
#define ICF_SIC_APP_TRACE(p_sic_pdb,trace_id) 
#endif



/* ----------------------------------------------------------------------------
 * Macro to set the event received in the pdb
 * --------------------------------------------------------------------------*/
#define ICF_SIC_SET_EVENT(p_sic_pdb, event_recd)                        \
{                                                                            \
    p_sic_pdb->event = event_recd;                                          \
}/* End of macro */


/* ----------------------------------------------------------------------------
 * Macro to set the timer id in the SIC ctx
 * --------------------------------------------------------------------------*/
#define ICF_SIC_SET_TIMER_TYPE(p_sic_pdb, timer_type_recd)             \
{                                                                           \
    p_sic_pdb->p_sic_ctx->timer_type = timer_type_recd;                   \
}/* End of macro */


/* ----------------------------------------------------------------------------
 * Macro to set the state in the SIC ctx
 * --------------------------------------------------------------------------*/
#define ICF_SIC_SET_STATE(p_sic_pdb,state_recd)                        \
{                                                                           \
    p_sic_pdb->p_sic_ctx->sic_state = state_recd;                        \
}/* End of macro */


/* ----------------------------------------------------------------------------
 * Macro to set the buffer in the SIC ctx
 * --------------------------------------------------------------------------*/
#define ICF_SIC_SET_BUFFER(p_sic_pdb)                                  \
{                                                                           \
    p_sic_pdb->p_sic_ctx->event_buffer = ICF_TRUE;                     \
}


/* ----------------------------------------------------------------------------
 * Macro to reset the timer id in the SIC ctx
 * --------------------------------------------------------------------------*/
#define ICF_SIC_RESET_TIMER_TYPE(p_sic_pdb)                            \
{                                                                           \
    p_sic_pdb->p_sic_ctx->timer_type = ICF_NULL;                       \
}


/* ----------------------------------------------------------------------------
 * Macro to set the subscription state and the error code in the pdb
 * --------------------------------------------------------------------------*/
#define ICF_SIC_SET_CURR_STATUS(p_sic_pdb, subs_state)                 \
{                                                                           \
    p_sic_pdb->curr_subs_status = subs_state;                              \
}    

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_SIC_MACRO_H__ */
