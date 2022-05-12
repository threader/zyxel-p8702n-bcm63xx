/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_regm_prototypes.h
 *
 * Purpose          : This file contains prototypes of various function
 *                    provided by DBM for registration interface
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

#ifndef __ICF_DBM_REGM_PROTOTYPES_H__ 
#define __ICF_DBM_REGM_PROTOTYPES_H__ 

#include "icf_dbm_types.h"
#include "icf_common_types.h"
#include "icf_common_interface_types.h"

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
 *     DESCRIPTION : This function allocates pool for registration context 
 *                   blocks and initializes the linked array structure for the
 *                   service context logic blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_rgn_cntxt_blk(
  INOUT icf_glb_pdb_st                        *p_glb_pdb,
  IN    icf_uint16_t                          num_of_blks,
  OUT   icf_void_t                            **p_rgn_cntxt_blk,
  OUT   icf_dbm_rgn_cntxt_la_st               **p_rgn_cntxt_blk_la,
  OUT   icf_dbm_rgn_cntxt_blk_id_t            *p_rgn_cntxt_blk_next_free_id,
  OUT   icf_error_t                           *p_ecode);


 /************************************************************************
 *
 * DESCRIPTION: Returns the index of a free registration block, which could be 
 *              used to save registration information for a user
 *
************************************************************************/  
icf_return_t icf_dbm_alloc_rgn_blk(            
        INOUT icf_glb_pdb_st                   *p_glb_pdb,                  
        OUT   icf_rgm_context_st               **p_p_registration_data,
        OUT   icf_error_t                      *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function would add the registration block to unused block 
 *              linked array
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_dealloc_rgn_blk(
        INOUT icf_glb_pdb_st                        *p_glb_pdb,
        INOUT icf_rgm_context_st                    *p_rgn_cntxt_blk,
        OUT   icf_error_t                           *p_ecode);

   
/****************************************************************************
 *
 * DESCRIPTION:       Check whether the line id is present in the registration 
 *                    block's list of lines user is registered on.
 *
 ***************************************************************************/
icf_return_t
icf_dbm_chk_line_in_rgn(
        INOUT icf_glb_pdb_st       *p_glb_pdb,        
        IN icf_line_id_t        line_id,
        IN icf_rgm_context_st   *p_rgn_data);

/****************************************************************************
 *
 * DESCRIPTION:      Deletes a line from users list of registered lines 
 *
 ***************************************************************************/
icf_return_t
icf_dbm_del_line_in_rgn(
        icf_glb_pdb_st       *p_glb_pdb,
        icf_line_id_t        line_id,
        icf_rgm_context_st   *p_rgn_data);

/****************************************************************************
 *
 * DESCRIPTION:        The function would be called to add a new registrtion for
 *                     the given user on the specified line 
 *
 ***************************************************************************/
icf_return_t
icf_dbm_regm_add_user(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      IN     icf_line_id_t        line_id,
      IN     icf_address_st       *p_user_addr,
      OUT    icf_uint8_t         *p_status,
      OUT    icf_rgm_context_st  **p_p_rgn_data,
      OUT    icf_error_t          *p_ecode);

/****************************************************************************
 *
 * DESCRIPTION:        The function would be called to delete registrtion for
 *                     the given user on the specified line
 *                     
 ***************************************************************************/
icf_return_t
icf_dbm_regm_delete_user(
      INOUT icf_glb_pdb_st       *p_glb_pdb,
      IN    icf_line_id_t        line_id,
      IN    icf_address_st       *p_user_addr,
      OUT   icf_uint8_t          *p_status,
      OUT   icf_rgm_context_st  **p_p_rgn_blk_addr,
      OUT   icf_error_t          *p_ecode);

/****************************************************************************
 *
 * DESCRIPTION:        The function would be called to fetch the address of the 
 *                     the registration context block for the specified user    
 *
 ***************************************************************************/
icf_return_t
icf_dbm_fetch_rgn_blk(
        INOUT icf_glb_pdb_st     *p_glb_pdb,
        IN    icf_address_st     *p_user_addr,
        IN    icf_line_id_t       line_id,
        OUT   icf_rgm_context_st **p_p_rgn_blk_addr,
        OUT   icf_error_t        *p_ecode);

/****************************************************************************
 *
 * DESCRIPTION:        The function would be called to add a new registrtion for
 *                     the given user on the specified line 
 *
 ***************************************************************************/
icf_return_t
icf_dbm_create_all_rgn_blks(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      OUT    icf_uint16_t         *p_num_of_blks_created,
      OUT    icf_error_t          *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION : This function returns the address of registration block 
 *                   which is at the index marked by block_num in used 
 *                   registration list 
 *                   
 *************************************************************************/
icf_return_t
icf_dbm_get_used_rgn_blk(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
        IN    icf_uint16_t            block_num,
        OUT   icf_rgm_context_st     **p_p_rgn_cntxt_blk,
        OUT   icf_error_t            *p_ecode);

/******************************************************************************
 *
 * DESCRIPTION:     This is a utility function that initializes registration 
 *                  context with default values.
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_init_rgn_ctx(
        INOUT icf_glb_pdb_st    *p_glb_pdb,
        INOUT icf_rgm_context_st   *p_rgn_ctx);


#ifdef IMS_CLIENT
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_next_buffered_reg_ctxt
 *
 * DESCRIPTION : This function gets the next buffered registration context
 *              
 * RETURNS: icf_return_t 
 *
 ************************************************************************/

icf_return_t icf_dbm_get_next_buffered_reg_ctxt(icf_glb_pdb_st *p_glb_pdb,
                                                icf_uint8_t current_buf_id,
                                                icf_rgm_context_st **p_p_buffered_rgm_block,
                                                icf_error_t *p_ecode);
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_buffered_block_by_address
 *
 * DESCRIPTION : This function gets a buffered registration context for 
 *                the given uri
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_buffered_block_by_address(icf_glb_pdb_st *p_glb_pdb,
                                                   icf_address_st *p_user_addr,
                                                   icf_rgm_context_st **p_p_buffered_rgm_block,
                                                   icf_error_t *p_ecode);
#endif/*IMS_CLIENT*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_REGM_PROTOTYPES_H__ */


