/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_defs.h
 *
 * Purpose          : This file contains prototypes of various function
 *                    provided by DBM
 *
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  07 DEC 2004   Sumit Gupta                File created and initial version
 *	08-Mar-2006   Aman Aggarwal            ICF merging with ICF2.0
 *
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_DBM_DEFS_H__ 
#define __ICF_DBM_DEFS_H__ 

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

/* Used for initializing has table */    
/*This the status returned by icf_dbm_la_alloc functio in case all the 
 *blocks in the specified pool are used up*/

#define ICF_DBM_CTRL_BLK_EXHAUSTED 1   

#define ICF_DBM_OPT_CTX_ID_START 49152
#define ICF_DBM_OPT_CTX_ID_END   65534    

#define ICF_DBM_INC_CALL_ID_START 0x8000

/* no.of elements in the call context hash table */
#define ICF_DBM_CC_MAX_KEY_QTY	384

/* no.of elements in the ssa context hash table */
#define ICF_DBM_SSA_MAX_KEY_QTY	384

/* max number of buckets in call context hash table */
#define ICF_DBM_CC_MAX_NUM_BUCKET	256

/* max number of buckets in ssa context hash table */
#define ICF_DBM_SSA_MAX_NUM_BUCKET	256

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_DEFS_H__ */


