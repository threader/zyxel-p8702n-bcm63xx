/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_macro.h
 *
 * Purpose          : This file contains various macros defined by DBM module
 *
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  07 DEC 2004   Sumit Gupta                File created and initial version
 *
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_DBM_MACRO_H__ 
#define __ICF_DBM_MACRO_H__ 

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



/*************************************************************************
 *
 *     MACRO    : ICF_LINE_CNTXT_BLK_PTR
 *
 *     DESCRIPTION : The macro returns pointer to pointer for specified line id 
 *                          
 *************************************************************************/
#define ICF_DBM_LINE_CNTXT_BLK_PTR(line_id,p_p_line_data,p_glb_pdb)\
{\
    p_p_line_data = p_glb_pdb->p_glb_data->p_p_line_data + line_id;\
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_MACRO_H__ */



