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
 * 08-Apr-2008  Tarun Gupta     SPR 18355       Merged Windows Porting Changes
 * 04-Jun-2008  Tarun Gupta     SPR 18585       ES_TRACE should be as per ICF_PRINT
 * 09-Sep-2009  Rajiv Kumar     SPR 19999       Merged SPR 19996 
 *
 * Copyright (c) 2009, Aricent.
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

#define ICF_ES_TRACE(str) ICF_PRINT(str)

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

#define ICF_OS_SEND(p_interface_info,p_msg,ret_val,p_ecode) \
{ \
    ret_val=icf_os_send(p_interface_info,p_msg,p_ecode); \
}

/*Fix for SPR 19999 (Merged SPR: 19996)*/
#ifdef ICF_MM_STUBED_OUT
#define IMSC_MM_MEMGET(buffer_get,size)	\
{									\
    buffer_get = malloc(size);	\
}		
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ICF_ES_MACRO_H__ */    
