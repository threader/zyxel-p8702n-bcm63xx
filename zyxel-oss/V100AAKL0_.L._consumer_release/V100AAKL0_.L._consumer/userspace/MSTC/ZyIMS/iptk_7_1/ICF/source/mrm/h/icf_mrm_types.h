/*-----------------------------------------------------------------------------
 *      
 * File name        : icf_mrm_types.h
 *
 * Purpose          :  
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  23 DEC 2004   Sumit Gupta                File created and initial version
 *
 *
 *
 * Copyright (c) 2006, Aricent.
 *---------------------------------------------------------------------------*/ 

#ifndef __ICF_MRM_TYPES_H__
#define __ICF_MRM_TYPES_H__



/* Determine if a C++ compiler is being used.
 * If so, ensure that standard
 * C is used to process the API information.
 */

#include "icf_common_types.h"
#ifdef __cplusplus

/* Yes, C++ compiler is present.
 * Use standard C.
 */

extern "C"
{
#endif /* __cplusplus */



/***********************************************************************
 *
 * Typedef Name: icf_mrm_func_ptr_t
 * 
 * DESCRIPTION:  Would be used to define function prototypes for various 
 *               process message function
 *               
 ************************************************************************/

typedef icf_return_t (* icf_mrm_func_ptr_t)
                        (INOUT icf_internal_msg_st *);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_MRM_TYPES_H__ */


