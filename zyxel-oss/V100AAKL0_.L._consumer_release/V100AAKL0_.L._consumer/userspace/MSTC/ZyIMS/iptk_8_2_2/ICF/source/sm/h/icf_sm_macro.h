/*------------------------------------------------------------------------------
 *
 * File name        : icf_sm_macro.h
 *
 * Purpose          : This file contains the macro definitions used by SM.
 *
 * Revision History :
 *
 * Date         Name                    Ref#        Comments
 * --------     ------------            ----        ---------------
 * 10-Mar-2005  Jyoti Narula            SM LLD      Beginning of coding phase 
 * 05-Oct-2006  Aman Arora          ICF Rel 6.1     added new macros for enahnced
 *                                                  licensing in ICF
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_SM_MACRO_H__
#define __ICF_SM_MACRO_H__

#if (defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX))
#include<sys/types.h>
#include<sys/time.h>
#include<utime.h>
#include<time.h>
#include<stdio.h>
#include <ctype.h>
#include <stdlib.h>
#endif


#include "icf_port_defs.h"
#include "icf_common_defs.h"
#include "icf_macro.h"
#include "icf_sm_prototypes.h"
#include "icf_port_macros.h"
#include "icf_port_prototypes.h"

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
 * Macro Name       : ICF_SM_TIME
 * Description      : Macro for obtaining the system time and date.
 *
 *----------------------------------------------------------------------------*/
#define ICF_SM_TIME(sm_time_details,ret_val) \
{ \
    ret_val = icf_port_get_curr_time(&(sm_time_details)); \
}




/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SM_STRCPY
 * Description      : Macro for copying the string.
 *
 *----------------------------------------------------------------------------*/
#define ICF_SM_STRCPY(dest_str,src_str ) \
{ \
    icf_port_strcpy(dest_str,src_str); \
}


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SM_TRACE
 * Description      : Macro for trace handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_SM_TRACE(p_sm_pdb,trace_id) \
{ \
    icf_sm_trace(p_sm_pdb,(icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_SM_TRACE(p_sm_pdb, trace_id)
#endif /* End of #ifdef ICF_TRACE_ENABLE */

/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SM_NON_API_TRACE
 * Description      : Macro for trace handling for functions 
 *                    other than API handlers.
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_TRACE_ENABLE
#define ICF_SM_NON_API_TRACE(p_glb_pdb,p_sm_data,trace_id) \
{ \
    icf_sm_non_api_trace(p_glb_pdb,p_sm_data,\
            (icf_trace_id_t)(trace_id)); \
}
#else
#define ICF_SM_NON_API_TRACE(p_glb_pdb, p_sm_data,trace_id)
#endif /* End of #ifdef ICF_TRACE_ENABLE */


/*------------------------------------------------------------------------------
 *
 * Macro Name       : ICF_SM_SET_ECODE
 * Description      : Macro for ecode handling
 *
 *----------------------------------------------------------------------------*/
#ifdef ICF_ERROR_ENABLE
#define ICF_SM_SET_ECODE(p_sm_pdb, ecode) \
{ \
    icf_sm_error((icf_sm_pdb_st *)p_sm_pdb, (icf_error_id_t)(ecode)); \
}
#else
#define ICF_SM_SET_ECODE(p_sm_pdb, ecode)
#endif /* End of #ifdef ICF_ERROR_ENABLE */


/******************************************************************
 *          Macro Definitions for License Management
 ******************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* End of ifndef __ICF_SM_MACRO_H__ */


