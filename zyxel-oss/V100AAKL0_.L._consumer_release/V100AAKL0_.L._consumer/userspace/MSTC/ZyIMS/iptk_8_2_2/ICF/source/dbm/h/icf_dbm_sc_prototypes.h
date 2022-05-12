/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_sc_prototypes.h
 *
 * Purpose          : This file contains prototypes of various function
 *                    provided by DBM for services interface
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
 *----------------------------------------------------------------------------*/

#ifndef __ICF_DBM_SC_PROTOTYPES_H__ 
#define __ICF_DBM_SC_PROTOTYPES_H__ 


#include "icf_dbm_types.h"
#include "icf_sc_common_types.h"

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
 *     DESCRIPTION      :    This function initializes the linked array 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN   icf_uint16_t                     num_of_blks,
        OUT  icf_void_t                       **p_unused_srvc_cntxt_blk,
        OUT  icf_dbm_srvc_cntxt_la_st         **p_srvc_cntxt_blk_la,
        OUT  icf_dbm_srvc_cntxt_blk_id_t      *p_srvc_cntxt_blk_next_free_id,
        OUT  icf_error_t                      *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function looks for the sevice context block for the call
 *              if present return the block address else allocates a new block   *
 ************************************************************************/
icf_return_t  
icf_dbm_get_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        IN  icf_call_id_t                     call_id,
        OUT icf_service_ctx_st                **p_srvc_cntxt_blk,
        OUT icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              block list and returns a pointer to it   
 *              
 ************************************************************************/ 
icf_return_t 
icf_dbm_alloc_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        OUT   icf_service_ctx_st                **p_p_srvc_cntxt_blk,
        OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Releases the service context block and adds it back to unused
 *              nodes list
 *
 ************************************************************************/
icf_return_t 
icf_dbm_dealloc_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                      *p_glb_pdb,
        INOUT icf_service_ctx_st                *p_srvc_cntxt_blk,
        OUT   icf_error_t                         *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Returns the address of service context block for specified call
 *              id
 *
 *************************************************************************/
icf_return_t 
icf_dbm_find_srvc_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_service_ctx_st              **p_p_service_cntxt_blk,       
       OUT   icf_error_t                     *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the service context logic blocks.
 *
*************************************************************************/
icf_return_t 
icf_dbm_init_srvc_cntxt_lgc_blk(
  INOUT icf_glb_pdb_st                    *p_glb_pdb,
  IN    icf_uint16_t                      num_of_blks,
  OUT   icf_void_t                        **p_p_unused_srvc_cntxt_lgc_blk,
  OUT   icf_dbm_srvc_cntxt_lgc_la_st       **p_p_srvc_cntxt_lgc_blk_la,
  OUT   icf_dbm_srvc_cntxt_lgc_blk_id_t   *p_srvc_cntxt_lgc_blk_next_free_id,
  OUT   icf_error_t                       *p_ecode);


/************************************************************************
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              logic block list and returns a pointer to it
 *  
************************************************************************/
icf_return_t  
icf_dbm_alloc_srvc_cntxt_lgc_blk(
   INOUT icf_glb_pdb_st                       *p_glb_pdb,
   OUT    icf_void_t                          **p_p_srvc_cntxt_lgc_blk,
   OUT    icf_error_t                         *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Releases the service context logic block and adds it back to 
 *              unused nodes list
 *  
************************************************************************/
icf_return_t 
icf_dbm_dealloc_srvc_cntxt_lgc_blk(                  
      INOUT icf_glb_pdb_st                     *p_glb_pdb,
      INOUT icf_void_t                         *p_srvc_cntxt_lgc_blk,
      OUT   icf_error_t                        *p_ecode);

/************************************************************************
 *
 * DESCRIPTION:   Attaches an existing Sevice Context block to an existing Call
 *                Context Block
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_attach_sc_to_cc(            
      INOUT icf_glb_pdb_st                          *p_glb_pdb,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      INOUT  icf_service_ctx_st                     *p_srvc_cntxt,
      OUT    icf_error_t                            *p_ecode);

/******************************************************************************  *
 * DESCRIPTION:     This is a utility function that initializes service logic 
 *                  context with default values.
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_init_sl_ctx(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        INOUT icf_srvc_ctx_lgc_ut   *p_sl_ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_SC_PROTOTYPES_H__ */


