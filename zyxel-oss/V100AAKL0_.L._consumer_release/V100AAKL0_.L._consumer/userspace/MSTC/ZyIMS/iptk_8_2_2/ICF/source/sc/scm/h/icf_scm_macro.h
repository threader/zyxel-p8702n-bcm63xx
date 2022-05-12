/*------------------------------------------------------------------------------
 *
 * File name        : icf_scm_macro.h
 *
 * Purpose          : This file contains the macro definitions.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 25-Jan-2005  Navneet Gupta                           Initial
 * 27-Feb-2006  Amit Sharma                  Merged code for ICF
 * 08-Apr-2008  Tarun Gupta     SPR 18355    Merged Windows Porting Changes
 *
 * Copyright (c) 2008, Aricent.
 *----------------------------------------------------------------------------*/


#ifndef __ICF_SCM_MACRO_H__
#define __ICF_SCM_MACRO_H__
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_scm_types.h"
#include "icf_scm_prototypes.h"

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
 * Macro Name       : ICF_SCM_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_SCM_TRACE(p_pdb, scm_trace_id) \
{\
    icf_scm_trace(p_pdb, (icf_trace_id_t const)(scm_trace_id)); \
} /* #define ICF_SCM_TRACE(p_pdb... */
#else
#define ICF_SCM_TRACE(p_pdb, scm_trace_id)
#endif
/*------------------------------------------------------------------------------
 *
 * Macro name: ICF_SCM_SET_ECODE
 *
 * Description: This is the macro used for setting ecodes.
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_SCM_SET_ECODE(p_pdb,ecode)\
{\
    icf_scm_error(p_pdb,(icf_error_t const)(ecode));\
}
#else
#define ICF_SCM_SET_ECODE(p_pdb,ecode)
#endif
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SCM_SET_STATUS
 * Description      : Set current return status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_SCM_SET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_msg->msg_hdr.status |= status_id; \
     ICF_SCM_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_SCM_SET_CURR_ST... */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SCM_CHECK_STATUS
 * Description      : Get current return status from internal msg 
 *
 *----------------------------------------------------------------------------*/
#define ICF_SCM_CHECK_STATUS(p_pdb, status_id)  \
            (p_pdb->p_internal_msg->msg_hdr.status & status_id)


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SCM_RESET_STATUS
 * Description      : Reset status in internal msg with given
 *                    status id
 *
 *----------------------------------------------------------------------------*/
 #define ICF_SCM_RESET_STATUS(p_pdb, status_id) \
 {\
     p_pdb->p_internal_msg->msg_hdr.status &= ~status_id;\
     ICF_SCM_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS) \
 } /* #define ICF_SCM_SET_CURR_ST... */
 
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID 
 * Description      : Get the Service Module index from its ID
 *
 *----------------------------------------------------------------------------*/
 #define ICF_SCM_GET_INDEX_FROM_SERVICE_MODULE_ID(serv_id,index)\
 index= (icf_uint8_t) (serv_id-ICF_INT_MODULE_SL_START)

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX
 * Description      : Get the Service Module id from its index
 *
 *----------------------------------------------------------------------------*/
 #define ICF_SCM_GET_ID_FROM_SERVICE_MODULE_INDEX(serv_id,index)\
 serv_id= (icf_uint8_t) (index+ICF_INT_MODULE_SL_START)

 #define ICF_SCM_RET_SERVICE_INDEX(serv_id) \
 (serv_id - ICF_INT_MODULE_SL_START)

#ifdef __cplusplus
}
#endif /*  __cpluscplus */
#endif
#endif /*  __ICF_SCM_MACRO_H__ */
