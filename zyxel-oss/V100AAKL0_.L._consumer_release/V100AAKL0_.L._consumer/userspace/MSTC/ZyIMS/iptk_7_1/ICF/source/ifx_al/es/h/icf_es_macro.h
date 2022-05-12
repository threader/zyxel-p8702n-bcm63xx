/*------------------------------------------------------------------------------
 *
 * File name        : icf_es_macro.h
 *
 * Purpose          : This file contains the prototypes of all functions 
 *                    used by ES 
 *                    
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 14-Feb-2005  Pankaj Negi     Non Service LLD     Initial
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/

#ifndef __ICF_ES_MACRO_H__
#define __ICF_ES_MACRO_H__


#include "icf_es_types.h"
#include "icf_es_prototypes.h"

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

#if (defined(ICF_PORT_SOLARIS)||defined(ICF_PORT_LINUX)||defined(ICF_PORT_WINDOWS)|| defined(ICF_PORT_VXWORKS)) 
#if (defined(ICF_TRACE_ENABLE) && defined (ICF_ERROR_ENABLE))
#define ICF_ES_TRACE(str) \
{ \
    if (ICF_NULL != p_persistent_glb_pdb) \
    { \
        if ((ICF_TRUE == p_persistent_glb_pdb->trace_info.status) || \
            (ICF_TRUE == p_persistent_glb_pdb->error_info.status)) \
        { \
            printf str; \
        } \
    } \
}
#else
#define ICF_ES_TRACE(str)
#endif
#endif 


#if (defined(ICF_PORT_SOLARIS)||defined(ICF_PORT_LINUX)||defined(ICF_PORT_WINDOWS)|| defined(ICF_PORT_VXWORKS)) 
#if (defined(ICF_TRACE_ENABLE) && defined (ICF_ERROR_ENABLE))
#define ICF_INIT_ERROR_TRACE(str) \
{ \
            printf str; \
}
#else
#define ICF_INIT_ERROR_TRACE(str)
#endif
#endif




#ifdef ICF_PORT_WINCE
#define ICF_ES_TRACE(str) ICF_PRINT(str)
#endif

#define ICF_OS_SEND(p_interface_info,p_msg,ret_val,p_ecode) \
{ \
    ret_val=icf_os_send(p_interface_info,p_msg,p_ecode); \
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_MACRO_H__ */    
