/*------------------------------------------------------------------------------
 *
 * File name        : icf_cfg_macro.h
 *
 * Purpose          : This file contains the macro definitions used by CFG.
 *
 * Revision History :
 *
 * Date         Name            	Ref#        Comments
 * --------     ------------    	----        ---------------
 * 21-Jan-2005  Shefali Aggarwal	ICF LLD	Beginning of coding phase
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_CFG_MACRO_H__
#define __ICF_CFG_MACRO_H__


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
 * Macro Name       : ICF_CFG_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_CFG_TRACE(p_crm_pdb, trace_id) \
{ \
    icf_cfg_trace(p_crm_pdb, (icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_CFG_TRACE(p_crm_pdb, trace_id)
#endif

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_APP_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_APP_TRACE(p_crm_pdb, trace_id) \
{ \
    icf_cfg_app_trace(p_crm_pdb, (icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_CFG_APP_TRACE(p_crm_pdb, trace_id)
#endif /* End of #ifdef ICF_TRACE_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_ECODE
 * Description      : Macro for ecode handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_CFG_SET_ECODE(p_crm_pdb, ecode) \
{ \
    icf_cfg_error(p_crm_pdb, (icf_error_t)(ecode)); \
}
#else
#define ICF_CFG_SET_ECODE(p_crm_pdb, error)	 \
{ \
    p_crm_pdb->p_internal_msg->msg_hdr.ecode = (icf_error_t)(error); \
}
#endif /* End of #ifdef ICF_ERROR_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_VALID_PARAM
 * Description      : Sets the param validity flag in pdb
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_SET_VALID_PARAM(p_crm_pdb, val) \
{ \
    p_crm_pdb->is_valid_param = val; \
    ICF_CFG_TRACE(p_crm_pdb, ICF_TRACE_PARAM_VALIDITY) \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_RESPONSE
 * Description      : Sets the response parameters in pdb
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_SET_RESPONSE(p_crm_pdb, result_val, error_val) \
{ \
    p_crm_pdb->result = result_val; \
    p_crm_pdb->error_cause = error_val; \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_BITMASK
 * Description      : Sets a bit in a bitmask
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_SET_BITMASK(var, bit) \
{ \
    var |= bit; \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_RESET_BITMASK
 * Description      : Resets a bit in a bitmask
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_RESET_BITMASK(var, bit) \
{ \
    var &= (~(bit)); \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_SRVC_SUBS_BITMASK
 * Description      : Sets a bit in service subscription flag
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_SET_SRVC_SUBS_BITMASK(var, bit) \
{ \
    ((var)->service_subscription_flag) |= bit; \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_SET_SRVC_ACT_BITMASK
 * Description      : Sets a bit in service activation flag
 *
 *----------------------------------------------------------------------------*/
#define ICF_CFG_SET_SRVC_ACT_BITMASK(var, bit) \
{ \
    (var) |= bit; \
}

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_CFG_BLANK_LINE
 * Description      : Sends a blank line to be copied. It is used for deletion.
 * 
 *----------------------------------------------------------------------------*/
#define ICF_CFG_BLANK_LINE(num) \
{ \
    (icf_uint8_t *)"\0";\
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_CFG_MACRO_H__ */
