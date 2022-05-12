/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 28-Dec-2004  Jalaj Negi      Non Service LLD     Initial
 * 
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_REGM_MACRO_H__
#define __ICF_REGM_MACRO_H__


#include "icf_macro.h"


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
 * Macro Name       : ICF_RGM_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_RGM_TRACE(p_crm_pdb, trace_id)                                 \
{                                                                           \
    icf_rgm_trace(p_crm_pdb, (icf_trace_id_t)(trace_id));                 \
}
#else
#define ICF_RGM_TRACE(p_crm_pdb, trace_id)
#endif
/*------------------------------------------------------------------------------
 *
 *Macro Name       : ICF_RGM_APP_TRACE
 *Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
 #ifdef ICF_TRACE_ENABLE
 #define ICF_RGM_APP_TRACE(p_crm_pdb, trace_id)                                 \
 {                                                                           \
    icf_rgm_app_trace(p_crm_pdb, (icf_trace_id_t)(trace_id));                 \
 }
 #else
 #define ICF_RGM_APP_TRACE(p_crm_pdb, trace_id)
#endif /* end of #ifdef ICF_TRACE_ENABLE */
    
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_RGM_SET_ECODE
 * Description      : Macro for ecode handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_RGM_SET_ECODE(p_crm_pdb, ecode)                                \
{                                                                             \
    icf_rgm_error(p_crm_pdb, (icf_error_t)ecode);                       \
}
#else
#define ICF_RGM_SET_ECODE(p_crm_pdb, error)                                \
{                                                                             \
    p_crm_pdb->p_internal_msg->msg_hdr.ecode = (icf_error_t)(error);       \
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of ifndef __ICF_REGM_MACRO_H__ */
