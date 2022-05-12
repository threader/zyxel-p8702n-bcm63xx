/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfw_macro.h
 *
 * Purpose          : This file contains the macro definitions used by CFw.
 *
 * Revision History :
 *
 * Date         Name                    Ref#        Comments
 * --------     ------------            ----        ---------------
 * 17-Mar-2005  Shefali Aggarwal        ICF LLD      Beginning of coding 
 * 14-Jun-2005  Sumit Gupta             SPR 7529        Macro settinf release 
 *                                                      type would set the 
 *                                                      bitmask and should not 
 *                                                      overwrite previous value
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFW_MACRO_H__
#define __ICF_CFW_MACRO_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_macro.h"
/*#include "icf_trace_id.h"*/


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
 * Macro Name       : ICF_CFW_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFW_SET_STATUS(p_cfw_pdb, status_id) \
{\
    p_cfw_pdb->p_internal_msg->msg_hdr.status |= status_id; \
    ICF_CFW_TRACE(p_cfw_pdb,ICF_CFW_TRACE_STATUS)\
}
 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFW_SET_REL_CAUSE_N_TYPE
 * Description      : Set the release cause and type in the call context.
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFW_SET_REL_CAUSE_N_TYPE(p_cfw_pdb, rel_type, rel_cause) \
{\
    if((ICF_NULL == p_cfw_pdb->p_glb_pdb->p_call_ctx->release_cause) || \
            (ICF_CALL_CLEAR_REASON_USER_BUSY == \
             p_cfw_pdb->p_glb_pdb->p_call_ctx->release_cause))\
    {\
        p_cfw_pdb->p_glb_pdb->p_call_ctx->release_cause = rel_cause;\
        p_cfw_pdb->p_glb_pdb->p_call_ctx->release_type |= rel_type;\
    }\
    ICF_CFW_TRACE(p_cfw_pdb,ICF_CFW_TRACE_REL_TYPE_N_CAUSE)\
}    
 
#ifdef ICF_ERROR_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFW_SET_ECODE
 * Description      : Set the ecode.
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFW_SET_ECODE(p_cfw_pdb, ecode_id)\
{\
   icf_cfw_ecode(p_cfw_pdb, ecode_id);\
}
#else

#define ICF_CFW_SET_ECODE(p_cfw_pdb, ecode_id)
#endif

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFW_TRACE
 * Description      : Raises trace.
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CFW_TRACE(p_cfw_pdb, trace_id) \
{ \
    icf_cfw_trace(p_cfw_pdb, (icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_CFW_TRACE(p_cfw_pdb, trace_id)
#endif


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFW_APP_TRACE
 * Description      : Raises trace.
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CFW_APP_TRACE(p_cfw_pdb, trace_id) \
{ \
    icf_cfw_app_trace(p_cfw_pdb, (icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_CFW_APP_TRACE(p_cfw_pdb, trace_id)

#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* End of ifndef __ICF_CFW_MACRO_H__ */


