/*------------------------------------------------------------------------------
 *
 * File name        : icf_mrm_defs.c
 *
 * Purpose          : This file contains the common #defs for MRM 
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  16 DEC 2004   Sumit Gupta                File created and initial version
 *
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_MRM_DEFS_H__
#define __ICF_MRM_DEFS_H__

/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */

#ifdef ICF_TRACE_ENABLE
/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_MRM_APP_TRACE
 * Description      : Print the gives trace in trace_id
 *
 *---------------------------------------------------------------------------*/
 #define ICF_MRM_APP_TRACE(p_glb_port_data, mrm_trace_id) \
 { \
    icf_mrm_app_trace(p_glb_port_data, (icf_trace_id_t)(mrm_trace_id)); \
 } /* #define ICF_MRM_APP_TRACE(p_pdb... */
#else
 #define ICF_MRM_APP_TRACE(p_glb_port_data, mrm_trace_id)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_MRM_DEFS_H__ */

