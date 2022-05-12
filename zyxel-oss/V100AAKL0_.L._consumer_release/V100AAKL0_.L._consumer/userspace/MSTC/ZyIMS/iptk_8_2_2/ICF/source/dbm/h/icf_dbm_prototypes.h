/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_prototypes.h
 *
 * Purpose          : This file contains prototypes of various function
 *                    provided by DBM
 *
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  07 DEC 2004   Sumit Gupta                File created and initial version
 *  11 FEB 2005   Amit Sharma                To add function to fill local user
 *                                           info for CTU-CTA SL
 *  06 APR 2005   Sumit Gupta    SPR No 7355 Added new function prototype for 
 *                                           the to extract user name from SIP 
 *                                           URI
 * 10-Aug-2005	Rohit Aggarwal	ICF			 Code changes specific to ICF
 * 02-Feb-2006  Amit Sharma                  CFG merged with ICF 2.0
 * 08-Mar-2006  Aman Aggarwal				 ICF merging with ICF2.0
 * 23-Mar-2006  Amit Sharma                  Code merged with ICF 4.2
 * 15-Jan-2008  Rajiv Kumar                  Registration Procedure
 *                                           Enhancements
 * 26-Jan-2008  Amit Mahajan     ICF Rel 8.0 Changes for reg event package support
 * 04-Nov-2008  Tarun Gupta      SPR 19189   Merged Rel 7.1.2 SPR 18880
 * 12-Nov-2008  Rajiv Kumar      SPR 19188   Rel 7.1.2 SPR merging (SPR
 *                                           19028)
 * 30-Apr-2009  Tarun Gupta      SPR 19532   Added function icf_dbm_util_cmp_username
 * 08-Sep-2009  Rajiv Kumar      SPR 19999   Merged CSR 1-7586127 
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#ifndef __ICF_DBM_PROTOTYPES_H__ 
#define __ICF_DBM_PROTOTYPES_H__ 

#include "icf_common_types.h"
#include "icf_dbm_types.h"
#include"icf_common_interface_types.h"
#include "icf_ssa_common_types.h"

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
 * DESCRIPTION      :    This function initializes the linked array pool.
 *
*************************************************************************/
icf_return_t
icf_dbm_la_init(    
       IN     icf_uint16_t        la_pool_size, 
       INOUT  icf_dbm_la_st       *p_la_pool,       
       OUT    icf_uint16_t        *p_next_free_index,      
       OUT    icf_error_t         *p_ecode );

/*************************************************************************
 *
 *     DESCRIPTION      :  This function manipulates the linked array 
 *
*************************************************************************/
icf_return_t
icf_dbm_la_allocate (
        IN      icf_uint16_t     la_pool_size,
        INOUT   icf_dbm_la_st    *p_la_pool,
        INOUT   icf_uint16_t     *p_next_free_index,
        OUT     icf_uint16_t     *p_new_index,
        OUT     icf_uint16_t    *p_status,
        OUT     icf_error_t      *p_ecode );

/*************************************************************************
 *
 *  DESCRIPTION      :  This function manipulates the linked array 
 *                      structure after a control block has been
 *                      returned to the pool.
 *
 *************************************************************************/
icf_return_t
icf_dbm_la_deallocate (   
        INOUT icf_dbm_la_st       *p_la_pool,        
        INOUT icf_uint16_t        *p_next_free_index,
        INOUT icf_uint16_t        index_to_free, 
        OUT   icf_error_t         *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This is the function which allocates memory for global data and
 *              and calls init function for all modules 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_init(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        OUT   icf_error_t                        *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This is the function which allocates memory for scalable
 *              data structures in global data 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_init_scalable_param(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st         *p_init_param, 
        OUT   icf_error_t                        *p_ecode);


/*************************************************************************
 *
 *     DESCRIPTION : This function returns the address of data block for
 *                   the module whose id is been given                         
 *
 *************************************************************************/
icf_return_t
icf_dbm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data,
        OUT   icf_error_t           *p_ecode);


/*************************************************************************
 *
 *     DESCRIPTION      :    This function would creates an array of line block 
 *                           pointers 
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_line_blk(
       INOUT icf_glb_pdb_st                     *p_glb_pdb,
       IN    icf_uint16_t                       num_of_blks,
       OUT   icf_line_data_st                   ***p_p_p_line_data,
       OUT   icf_error_t                        *p_ecode);

 /************************************************************************
 *
 * DESCRIPTION: Allocates memory for line data block and returns a pointer to 
 *              it
 * 
 ************************************************************************/  
icf_return_t 
icf_dbm_alloc_line_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_line_id_t                     line_id,
       OUT   icf_line_data_st                  **p_p_line_data,
       OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Releases the memory allocated for line block 
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_dealloc_line_blk(
       INOUT icf_glb_pdb_st                      *p_glb_pdb,
       IN    icf_line_id_t                       line_id,
       OUT   icf_error_t                         *p_ecode);

/*************************************************************************
 *
 * DESCRIPTION: Return the address for line context block corresponding to the
 *              line ID. If no block exist allocates a new block and returns
 *              its address.
 * 
 *************************************************************************/
icf_return_t
icf_dbm_get_line_cntxt_blk(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        IN    icf_line_id_t       line_id,
        OUT   icf_line_data_st    **p_p_line_data,
        OUT   icf_uint8_t         *p_line_status,   
        OUT   icf_error_t         *p_ecode);


/*************************************************************************
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the call context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_call_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_uint16_t                    num_of_blks,
       OUT   icf_dbm_call_cntxt_la_st        **p_p_call_cntxt_blk_la,
       OUT   icf_call_ctx_st                 **p_p_call_context_data, 
       OUT   icf_dbm_call_cntxt_blk_id_t     *p_call_cntxt_blk_next_free_id,
       OUT   icf_error_t                     *p_ecode);


/************************************************************************
 *
 * DESCRIPTION: Would allocate a call context block from free node list and 
 *              allocate its pointer to call context pointer in global pdb
 *
************************************************************************/  
icf_return_t 
icf_dbm_alloc_call_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        INOUT icf_call_id_t                     *p_call_id, 
        OUT   icf_call_ctx_st                   **p_p_call_context_data,
        OUT   icf_error_t                       *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :    This function initializes the linked array 
 *                           structure for the sic context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_uint16_t                    num_of_blks,
       OUT   icf_dbm_sic_cntxt_la_st        **p_p_sic_cntxt_blk_la,
       OUT   icf_sic_ctx_st                 **p_p_sic_context_data, 
       OUT   icf_dbm_sic_cntxt_blk_id_t     *p_sic_cntxt_blk_next_free_id,
       OUT   icf_error_t                     *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Would allocate a sic context block from free node list and 
 *              allocate its pointer to call context pointer in global pdb
 *
************************************************************************/  
icf_return_t 
icf_dbm_alloc_sic_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        INOUT icf_call_id_t                     *p_call_id, 
        OUT   icf_sic_ctx_st                   **p_p_sic_context_data,
        OUT   icf_error_t                       *p_ecode);


/************************************************************************
 *
 * DESCRIPTION: The function would free the hash mapping for outgoing call 
 *              and add the block to free blocks list
 *
************************************************************************/  
icf_return_t 
icf_dbm_dealloc_call_cntxt_blk(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_call_id_t                     call_id,
       OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Returns the address of call context block for specified call id
 *
 ************************************************************************/
icf_return_t 
icf_dbm_find_call_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_call_ctx_st                 **p_p_call_cntxt_blk,
       OUT   icf_error_t                     *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Returns the address of SIC context block for specified call id
 *
 ************************************************************************/
icf_return_t 
icf_dbm_find_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_call_id_t                   call_id,
       OUT   icf_sic_ctx_st                 **p_p_sic_cntxt_blk,
       OUT   icf_error_t                     *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function would free the hash mapping for outgoing call 
 *              and add the block to free blocks list
 *
************************************************************************/  
icf_return_t 
icf_dbm_dealloc_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_call_id_t                     call_id,
       OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would allocate memory for a new SDP block
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_create_sdp_block(
       INOUT icf_glb_pdb_st               *p_glb_pdb,
       OUT icf_void_t                     **p_p_sdp_data,
       OUT icf_error_t                    *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would free the memory allocated for the SDP block
 *
************************************************************************/  
icf_return_t 
icf_dbm_free_sdp_block(
       INOUT icf_glb_pdb_st          *p_glb_pdb,
       INOUT icf_void_t              *p_sdp_data,
       OUT   icf_error_t             *p_ecode);


#ifdef ICF_MULTILINE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_user_priority
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists 
 *                on the line and returns its priority.In case the user was not
 *                 present it returns the priority as ICF_NULL
 *                
 * RETURNS:       Returns a valid user priority is user was found on line else 
 *                returns ICF_NULL.
 *
 ************************************************************************/      
icf_priority_id_t
icf_dbm_get_user_priority(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        IN    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk);
#endif

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_mode_ogn_call
 *
 * DESCRIPTION:    This function would validate the self mode against the proxy 
 *                 mode to identify if the otg call could be initiated 
 *
 ******************************************************************************/
icf_return_t 
icf_dbm_validate_mode_ogn_call(
        INOUT icf_glb_pdb_st      *p_glb_pdb,        
        IN    icf_uint8_t         *p_transport_val,
        IN    icf_line_id_t  line_id,
        IN    icf_boolean_t  proxy_routed_call,
        OUT   icf_error_t *p_ecode);

/*******************************************************************************
 *
 * FUNCTION:       icf_cc_validate_mode_ogn_call
 *
 * DESCRIPTION:    This function would look into the global default setting to 
 *                 identify if outgoing calls need to be initiated through proxy
 *
 ******************************************************************************/
icf_boolean_t 
icf_dbm_chk_ogn_call_thru_proxy(
        INOUT icf_glb_pdb_st      *p_glb_pdb,        
        OUT   icf_error_t         *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: Finds out pointer to the stats data corresponding the stats id
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_stats_glb_data(
       INOUT icf_glb_pdb_st        *p_glb_pdb,
       OUT   icf_void_t            **p_p_stats_blk,
       OUT icf_error_t             *p_ecode);


/*************************************************************************
 *
 *     DESCRIPTION      :    Calls DBM's stop function to clear all memory 
 *                           allocated by DBM and then call init function to 
 *                           reallocate memory.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_restart(
        OUT icf_glb_pdb_st                       *p_glb_pdb,
        IN  icf_dbm_scale_data_init_st           *p_init_param,
        OUT icf_error_t                          *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :    Clears all the memory held by DBM .
 *
 *************************************************************************/
icf_return_t 
icf_dbm_stop(
       INOUT icf_glb_pdb_st           *p_glb_pdb,
       OUT   icf_error_t              *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: It will allocate the memory  for the Address block
 *
************************************************************************/  
icf_return_t 
icf_dbm_create_addr_blk(
       INOUT icf_glb_pdb_st     *p_glb_pdb,
       OUT icf_void_t           **p_p_addr_data,
       OUT icf_error_t          *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: It will free the memory allocated for the Address block
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_free_addr_blk(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      INOUT  icf_void_t           *p_addr_data,
      OUT    icf_error_t          *p_ecode);

/************************************************************************
 * 
 * DESCRIPTION: Returns the index of the block in DBM's pool
 *
 ************************************************************************/
icf_return_t
icf_dbm_ret_index_for_addr(
        INOUT icf_glb_pdb_st         *p_glb_pdb,
        IN    icf_void_t             *p_start_addr,
        IN    icf_void_t             *p_block_addr,
        IN    icf_uint16_t           block_size,
        OUT   icf_uint16_t           *p_index,
        OUT   icf_error_t            *p_ecode);


/************************************************************************
 *
 * DESCRIPTION: Returns the number of ongoing calls
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_no_of_ong_calls(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        OUT   icf_uint16_t          *p_no_of_calls,
        OUT   icf_error_t           *p_ecode);


/*************************************************************************
 *
 *     DESCRIPTION      :   This function initializes global DBM's data
 *                          block.
 * 
 *************************************************************************/
icf_return_t
icf_dbm_init_dbm_data(
      INOUT icf_glb_pdb_st           *p_glb_pdb,
      OUT   icf_dbm_data_st          **p_p_dbm_data,
      OUT   icf_error_t              *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would allocate memory for a new timer block
 *
 ************************************************************************/
icf_return_t 
icf_dbm_create_timer_block(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       OUT   icf_void_t                                **p_p_timer_data,
       OUT   icf_error_t                               *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would free the memory allocated for the timer
 *              block
 *
************************************************************************/
icf_return_t 
icf_dbm_free_timer_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       INOUT icf_void_t                        *p_timer_data,
       OUT   icf_error_t                       *p_ecode);


/************************************************************************
 * 
 * DESCRIPTION:   Attaches an existing Call Context block to an existing line 
 *                on which the call is going
 *
 ************************************************************************/
icf_return_t 
icf_dbm_attach_cc_to_line(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode);

/****************************************************************************
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_address_st type addresses    
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_adress1,
        IN    icf_address_st *p_adress2);

/****************************************************************************
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_transport_address_st type addresses    
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_trans_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_transport_address_st *p_address1,
        IN    icf_transport_address_st *p_address2);


/****************************************************************************
 *
 * DESCRIPTION:        This is a utility function which would check if any call 
 *                     is active on the line on which current call is running
 *                     icf_address_st type addresses    
 *
 ***************************************************************************/
icf_return_t icf_dbm_check_other_call_act(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        OUT   icf_boolean_t    *p_other_call_active,
        OUT   icf_error_t     *p_ecode );

/*************************************************************************
 *
 *     DESCRIPTION : This function returns the address of data block for
 *                   porting module
 *
 *************************************************************************/
icf_return_t
icf_dbm_get_port_data(
        INOUT icf_glb_pdb_st  *p_glb_pdb,
        OUT   icf_void_t      **p_p_port_data,
        OUT   icf_error_t     *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :   This function initializes global data
 *                          block to default values .
 *
 *************************************************************************/
icf_void_t
icf_dbm_init_glb_data(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_error_t              *p_ecode);


/************************************************************************
 *
 * DESCRIPTION: This the function which would call other modules free function 
 *              to free all global data
 *
 ************************************************************************/
icf_return_t 
icf_dbm_deinit(
        INOUT icf_glb_pdb_st            *p_glb_pdb,
        OUT   icf_error_t               *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :    Clears all the memory pools held by DBM .
 *
 *************************************************************************/
icf_void_t
icf_dbm_deinit_scalable_param(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_error_t              *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This the function which would call other modules free function 
 *              to free all global data
 *
 ************************************************************************/
icf_return_t 
icf_dbm_deinit(
        INOUT icf_glb_pdb_st            *p_glb_pdb,
        OUT   icf_error_t               *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :   This function frees global DBM's data
 *                          block.
 *
 *************************************************************************/
icf_void_t
icf_dbm_deinit_dbm_data(
      INOUT icf_glb_pdb_st          *p_glb_pdb,
      OUT  icf_dbm_data_st          *p_dbm_data,
      OUT  icf_error_t              *p_ecode);

/*************************************************************************
 *
 * DESCRIPTION: Return the address for line context block corresponding to the
 *              line ID. If no block exist allocates a new block and returns
 *              its address.
 * 
 *************************************************************************/
icf_return_t
icf_dbm_fetch_line_cntxt_blk(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        IN    icf_line_id_t       line_id,
        OUT   icf_line_data_st    **p_p_line_data,
        OUT   icf_error_t         *p_ecode);


/************************************************************************
 *
 * DESCRIPTION:   Removes an existing Call Context block from an existing line
 *
 ************************************************************************/
icf_return_t
icf_dbm_detach_cc_from_line(
      INOUT  icf_glb_pdb_st                         *p_glb_pdb,
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :   Return the address for line context block 
 *                          corresponding to the line ID. If no block exist 
 *                          returns failure.
 *
 *************************************************************************/
icf_return_t
icf_dbm_fetch_line_cntxt_blk(
        INOUT icf_glb_pdb_st      *p_glb_pdb,
        IN    icf_line_id_t       line_id,
        OUT   icf_line_data_st    **p_p_line_data,
        OUT   icf_error_t         *p_ecode);

/******************************************************************************  *
 * DESCRIPTION:     This is a utility function that initializes call context
 *                  with default values.
 *
 ******************************************************************************/
icf_return_t icf_dbm_init_call_ctx(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_call_ctx_st   *p_call_ctx);

/******************************************************************************  *
 * DESCRIPTION:     This is a utility function that initializes sic context
 *                  with default values.
 *
 ******************************************************************************/
icf_return_t icf_dbm_init_sic_ctx(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_sic_ctx_st   *p_sic_ctx);

/************************************************************************
 *
 * DESCRIPTION:   The function finds a free line for presenting an incomming 
 *                call. If found returns the line id and status to be idle.Else
 *                returns invalid line id.
 *                
 ************************************************************************/

icf_return_t
icf_dbm_get_line_for_inc_call(
    INOUT icf_glb_pdb_st *p_glb_pdb,
	IN    icf_boolean_t  search_alt_line,
    INOUT    icf_address_st *p_user_address,
    OUT   icf_line_id_t  *p_line_id,
    OUT   icf_uint32_t   *p_status,
    OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function looks in the call context pool to find a call 
 *              context block index for an incomming call .
 *              if present return the block index
 *              
************************************************************************/
icf_return_t 
icf_dbm_get_idx_for_ogn_ccx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode);

/************************************************************************
 *
 * DESCRIPTION:   The function checks for the line to be free for making an 
 *                outgoing call.
 *                
 ************************************************************************/
icf_return_t
icf_dbm_validate_line_for_ogn_call(
    INOUT icf_glb_pdb_st *p_glb_pdb,
    IN    icf_line_id_t  line_id,
    OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists 
 *                on the line .
 *
 ************************************************************************/  
icf_return_t
icf_dbm_check_if_user_exits_in_line(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        INOUT    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk,
		IN    icf_boolean_t   match_only_user_name,
        INOUT    icf_error_t      *p_ecode);


/************************************************************************
 *
 * DESCRIPTION:   THis function fills the local user name and address in
 *                CAll context from line data for a selected line in CC
 *                in p_glb_pdb
 *
 ************************************************************************/
icf_return_t 
icf_dbm_fill_local_user_info(
        INOUT  icf_glb_pdb_st     *p_glb_pdb);

/************************************************************************
 *
 * DESCRIPTION: The function looks in the call context pool to find a call 
 *              context block index for an incomming call .
 *              if present return the block index
 *              
************************************************************************/
icf_return_t 
icf_dbm_get_idx_for_inc_ccx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode);

icf_return_t 
icf_dbm_get_idx_for_inc_scx(
        icf_glb_pdb_st *p_glb_pdb,
        icf_call_id_t  call_id,
        icf_uint16_t   *p_index,
        icf_error_t    *p_ecode);


/******************************************************************************  *
 * DESCRIPTION:     The function would retun calls ongoing. It would return the 
 *                  in the order of the index in DBM's pool no matter in what 
 *                  sequence the calls were started .
 *
 ******************************************************************************/
icf_return_t
icf_dbm_get_ong_call_ctx(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_uint16_t   call_index,
        OUT   icf_call_ctx_st **p_p_call_ctx,
        OUT   icf_error_t     *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function looks for various services being activated on the 
 *              at various trigger points and returns status accordingly
 *              
************************************************************************/
icf_return_t 
icf_dbm_chk_line_service(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_uint8_t    trigger_cause,
        IN    icf_line_id_t  line_id,
        OUT   icf_uint32_t   *p_status,
        OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function decrements the number of call waiting calls on 
 *              the line
 *
************************************************************************/
icf_return_t
icf_dbm_decrease_call_waiting(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t line_id,
        OUT   icf_error_t *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function looks for the call being a call waiting call and 
 *              various services being active on the line. It would retreive the
 *              call waiting status and line id from the current call context 
 *              and return the alering timer duration depending on call type 
 *              and service active on line
 *
************************************************************************/
icf_return_t 
icf_dbm_get_alert_timer_dur(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        OUT   icf_duration_t *p_alert_timer_dur,
        OUT   icf_error_t    *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :   Would fill in the information about different block 
 *                          sizes and the number of blocks of the size to be 
 *                          created for task specific pool and call the port 
 *                          function to create the pools
 *
 *************************************************************************/
icf_return_t
icf_dbm_create_task_pool(
        INOUT icf_glb_pdb_st               *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st   *p_init_param,        
        OUT   icf_error_t                  *p_ecode);

/*************************************************************************
 *
 *     DESCRIPTION      :   Would fill in the information about different block 
 *                          sizes and the number of blocks of the size to be 
 *                          created for common pool and call the port function 
 *                          to create the pool
 *
 *************************************************************************/
icf_return_t
icf_dbm_create_cmn_pool(
        INOUT icf_glb_pdb_st              *p_glb_pdb,
        OUT   icf_error_t                  *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would allocate memory for a new timer block for
 *               SSA
 *
 ************************************************************************/
icf_return_t 
icf_dbm_create_ssa_timer_blk(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       OUT   icf_void_t                                **p_p_timer_data,
       OUT   icf_error_t                               *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would free the memory allocated for the timer
 *              block for SSA
 *
************************************************************************/
icf_return_t 
icf_dbm_free_ssa_timer_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       INOUT icf_void_t                        *p_timer_data,
       OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would allocate memory for specified pool 
 *              and return the pointer in pointer variable passed as 
 *              argument
 *
************************************************************************/
icf_return_t
icf_dbm_get_mem_from_pool(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       IN    icf_pool_id_t                             pool_id,
       OUT   icf_void_t                                **p_p_mem_blk,
       OUT   icf_error_t                               *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: This function would return the memory pointed by the 
 *              pointer specified as argument back to the pool.
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_ret_mem_to_pool(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_pool_id_t                     pool_id,
       INOUT icf_void_t                        *p_mem_blk,
       OUT   icf_error_t                       *p_ecode);

/************************************************************************
 *
 * DESCRIPTION: The function takes as an argument a SIP URI and extracts the 
 *              username from it and if the user name is a valid E164 number 
 *              then returns the E164 number in OUT parameter p_e164 and 
 *              ICF_TRUE as return value
 *
************************************************************************/
icf_boolean_t
icf_dbm_ext_usrnam_n_val_e164(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address,
        OUT   icf_address_st *p_e164);


/************************************************************************
 *
 * DESCRIPTION: The function would traverse all the line to identify if the user
 *              exists on any of the line. If it does exist meaning this is a 
 *              local user would return ICF_SUCCESS else would return 
 *              ICF_FAILURE
 *
************************************************************************/
icf_return_t
icf_dbm_chk_if_local_usr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT    icf_address_st *p_user_address,
        OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_set_blk_n_pool_size
 *
 * DESCRIPTION: The function initialize the pool size and block size in DBM 
 *              global data block for different pool ids based on which DBM 
 *              creates it pools   
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_set_blk_n_pool_size(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_scale_data_init_st         *p_init_param, 
        OUT   icf_error_t                        *p_ecode);


/*************************************************************************
 *
 *     FUNCTION NAME    :    icf_dbm_create_pools
 *
 *     DESCRIPTION      :    This function would be used to create various pools
 *                           for dbm and would also initialize the linked array 
 *                           of used nodes in pool.
 *
 *     RETURNS          :   icf_return_t  
 *
 *************************************************************************/
icf_return_t 
icf_dbm_create_pools(
        INOUT icf_glb_pdb_st                  *p_glb_pdb,
        OUT   icf_error_t                     *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_blk
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              block list and returns a pointer to it   
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/ 
icf_return_t 
icf_dbm_alloc_blk(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_block_type_et              block_type,
        OUT   icf_void_t                       **block_addr,
        OUT   icf_error_t                        *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_pool_addr
 *
 * DESCRIPTION: This is a utility function to retreive the pool address for the 
 *               block types identified by block_type passed as argument
 *
 * RETURNS:    returns address of pool
 *
 ************************************************************************/
icf_dbm_pool_info_st * 
icf_dbm_get_pool_addr(
       INOUT icf_glb_pdb_st                  *p_glb_pdb,
       IN    icf_dbm_block_type_et           block_type);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_ctx_id_for_opt
 *
 * DESCRIPTION:   The function would return a context identifier. The function 
 *                would be called by SSA when it gets options request from 
 *                network. SSA would pass on this identifier to MMI when 
 *                making media capability request.
 *                 
 * RETURNS:       Returns success
 *
 ************************************************************************/

icf_boolean_t
icf_dbm_get_ctx_id_for_opt (
    INOUT icf_glb_pdb_st *p_glb_pdb,
    OUT    icf_call_id_t *p_call_id,
    OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_ext_usrnam
 *
 * DESCRIPTION: The function takes as an argument a SIP URI and extracts the 
 *              username from it 
 *
 * RETURNS: Returns ICF_TRUE if user name in a SIP URI is a valid E164 Number
 *          and returns the e164 number in OUT parameter p_e164
 *
************************************************************************/
icf_return_t
icf_dbm_ext_usrnam(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address,
        OUT   icf_address_st *p_user_name);

/*******************************************************************************
 *
 * FUNCTION:        icf_dbm_convert_to_tran_addr
 *
 * DESCRIPTION:     This function converts the IP address
 *                  stored in the SDP body of SIP message
 *                  to the transport address understandable by the
 *                  ICF
 *
 ******************************************************************************/
icf_return_t icf_dbm_chk_if_dom_is_local_ip(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_address_st *p_address);

icf_return_t icf_dbm_check_if_ip(icf_uint8_t *p_address);

/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_tran_addr_wo_port
 *
 * DESCRIPTION:        This is a utility function to compare transport
 *                     addresses excluding the port.
 *                     
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_tran_addr_wo_port(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_transport_address_st *p_address1,
        IN    icf_transport_address_st *p_address2);

ICF_SESSION_CONTROL_START
/************************************************************************
 *
 * DESCRIPTION: The function would return the preferred identity for a 
 *              requested line. It is upto the calling function to allocate
 *              memory to the address pointer and free it later. If 
 *				no such user exists, this function will return failure with 
 *				ecode ICF_ECODE_NO_PREF_ID_EXISTS.
 *
 ************************************************************************/
icf_return_t
icf_dbm_get_preferred_identity(
        INOUT    icf_glb_pdb_st *p_glb_pdb,
        IN    icf_line_id_t  line_id,
        INOUT icf_address_st *p_pref_addr,
        OUT   icf_error_t    *p_ecode);

ICF_SESSION_CONTROL_END

ICF_CRM_START
#ifdef ICF_NW_ACTIVATE
/*************************************************************************
 *
 * DESCRIPTION: This function initializes the linked array of network
 *              access context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_netact_cntxt_blk(
		INOUT icf_glb_pdb_st                        *p_glb_pdb,
		IN    icf_uint16_t                          num_of_blks,
		IN    icf_uint16_t                          num_of_apps,
		OUT   icf_netact_ctxt_st                    **p_netact_cntxt_blk,
		OUT   icf_error_t                           *p_ecode);
#endif

/*************************************************************************
 *
 * DESCRIPTION: This function initializes the linked array of application
 *              context blocks.
 *
 *************************************************************************/
icf_return_t 
icf_dbm_init_app_conf_blk(
		INOUT icf_glb_pdb_st                        *p_glb_pdb,
		IN    icf_uint16_t                          num_of_blks,
		OUT   icf_app_conf_st                       **p_app_conf_blk,
		OUT   icf_error_t                           *p_ecode);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_blk
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              block list and returns a pointer to it   
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/ 
icf_return_t 
icf_dbm_dealloc_blk(
        INOUT icf_glb_pdb_st                     *p_glb_pdb,
        IN    icf_dbm_block_type_et              block_type,
        OUT   icf_void_t                         *block_addr,
        OUT   icf_error_t                        *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_ong_sic_ctx
 *
 *  DESCRIPTION : This function returns the address of data block for
 *                   the module whose id is been given   
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_get_ong_sic_ctx(
        INOUT icf_glb_pdb_st		*p_glb_pdb,
        IN    icf_uint16_t		call_index,
        OUT   icf_sic_ctx_st		**p_p_sic_cntxt_blk,
        OUT   icf_error_t		*p_ecode);

ICF_CRM_END

icf_return_t icf_dbm_get_default_uri_for_line (
                                                     icf_glb_pdb_st	*p_glb_pdb,
                                                     icf_line_id_t	line_id,
                                                     icf_address_st	**p_p_uri,
                                                     icf_error_t     *p_ecode
                                                     );

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_add_public_uri_for_line
 *
 *  DESCRIPTION : This function adds a public user address in the user 
 *                database of the given line.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_add_public_uri_for_line (
                                    icf_glb_pdb_st	*p_glb_pdb,
                                    icf_line_id_t	line_id,
                                    icf_address_st 	*p_public_uri,
                                    icf_error_t 	    *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_delete_public_uri_for_line 
 *
 *  DESCRIPTION : This function Deletes the given public user address 
 *                from the user database of the given line.
 *                Returns failure if given public uri is not found.
 *              
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t 
icf_dbm_delete_public_uri_for_line  (
                                    icf_glb_pdb_st	*p_glb_pdb,
                                    icf_line_id_t	line_id,
                                    icf_address_st 	*p_public_uri,
                                    icf_error_t 	    *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_default_uri_for_line 
 *
 * DESCRIPTION : This function returns the pointer to the first 
 *               non-barred uri on the line.
 *               Returns failure if no non-barred uri exists on the line
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_default_uri_for_line (
                                                     icf_glb_pdb_st	*p_glb_pdb,
                                                     icf_line_id_t	line_id,
                                                     icf_address_st	**p_p_uri,
                                                     icf_error_t     *p_ecode
                                                     );

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_search_address_list
 *
 * DESCRIPTION : This function searches the address list passed for 
 *               the given input uri.
 *               Returns ICF_SUCCESS if found, ICF_FAILURE otherwise
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_search_address_list(icf_glb_pdb_st            *p_glb_pdb,
                                         icf_address_list_node_st     *p_list,
                                         icf_address_st               *p_uri,	
					 icf_uint8_t		   search_bitmask);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_public_uri_node_from_uri_for_line
 *
 * DESCRIPTION : This function searches the user database for the given 
 *               line_id and returns the public uri list node pointer of 
 *               the node where the address given in p_uri is first found  
 *               in either the p-associated uri list or the contact list.
 *               The searching is controlled by the field search_bitmask
 *               if ICF_USER_DB_SEARCH_ASSOCIATED_URIS is set then the 
 *               p-associated uri lists are searched.
 *               if ICF_USER_DB_SEARCH_CONTACT_URIS is set then the 
 *               contact lists are searched.
 *               
 *               If both the bits are set then both the lists are searched
 *               If none of the bits are set then only the top 
 *               level public uri list is searched.
 *
 *               Returns failure if uri is not found in the user db
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_dbm_get_public_uri_node_from_uri_for_line (
                                                  icf_glb_pdb_st	*p_glb_pdb,
                                                  icf_line_id_t	line_id,
                                                  icf_address_st	*p_uri,
                                                  icf_uint8_t search_bitmask,
                                                  icf_user_db_node_st	**p_p_node,
                                                  icf_error_t     *p_ecode
                                                  );

icf_void_t icf_dbm_delete_address_list(icf_glb_pdb_st	*p_glb_pdb,
                               icf_address_list_node_st     **p_p_node,
                               icf_error_t     *p_ecode);

icf_void_t icf_dbm_delete_public_uri_list(icf_glb_pdb_st	*p_glb_pdb,
                               icf_user_db_node_st	  **p_p_node,
                               icf_error_t     *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_delete_public_uri_list_for_line
 *
 * DESCRIPTION : This function removed the p-associated uri list and 
 *               the contact list for a given line id.
 *
 *               Returns failure if public uri is not found in the user db
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_delete_public_uri_list_for_line (
                                            icf_glb_pdb_st	*p_glb_pdb,
                                            icf_line_id_t	line_id,
                                            icf_error_t     *p_ecode
                                            );

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_clear_public_uri_node_for_line 
 *
 * DESCRIPTION : This function removes the p-associated uri list and/or 
 *               the contact list for a given line id.
 *               if ICF_USER_DB_USE_ASSOCIATED_URIS is set then the 
 *               p-associated uri list is deleted.
 *               if ICF_USER_DB_USE_CONTACT_URIS is set then the 
 *               contact list is deleted.
 *               If both the bits are set then both the lists are deleted
 *
 *               Returns failure if public uri is not found in the user db
 *               or none of the bits in the bitmask are set.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t 
icf_dbm_clear_public_uri_node_for_line (
                                                  icf_glb_pdb_st	*p_glb_pdb,
                                                  icf_line_id_t	line_id,
                                                  icf_address_st	*p_uri,
                                                  icf_uint8_t delete_bitmask,
                                                  icf_error_t     *p_ecode
                                                  );
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dup_address_list
 *
 * DESCRIPTION : This function duplicates an address list structure
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_dup_address_list (
                             icf_glb_pdb_st	*p_glb_pdb,
                             icf_address_list_node_st  *p_src,
                             icf_address_list_node_st  **p_p_dest,
                             icf_error_t     *p_ecode);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_copy_user_list_to_pub_uri_list 
 *
 * DESCRIPTION : This function copies the a user database in a public uri 
 *               list structure. p-associated uri lists are copied based on 
 *               the copy_bitmask.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_copy_user_list_to_pub_uri_list (
                             icf_glb_pdb_st	*p_glb_pdb,
                             icf_user_db_node_st  *p_src,
                             icf_pub_uri_list_node_st  **p_p_dest,
                             icf_uint8_t copy_bitmask,
                             icf_error_t *p_ecode);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_public_user_count
 *
 * DESCRIPTION : This function returns the public user count in a 
 *               user database
 *              
 * RETURNS: icf_uint8_t
 *
 ************************************************************************/
icf_uint8_t icf_dbm_get_public_user_count(icf_user_db_node_st  *p_user_db);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_append_uri_to_address_list
 *
 * DESCRIPTION : This function appends a uri to the address list passed
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_append_uri_to_address_list(
                                   icf_glb_pdb_st	*p_glb_pdb,
                                   icf_address_st	*p_uri,
                                   icf_address_list_node_st **p_p_node,
                                   icf_error_t     *p_ecode
                                   );

/************************************************************************
 *
 * FUNCTION NAME:   icf_dbm_get_sys_scale_factor
 *
 * DESCRIPTION:     This function returns the system scaling factor value
 *                  corresponding to the configured resource limit.
 *
 * RETURNS:         System resource limit scaling factor
 *
 ************************************************************************/
icf_uint32_t icf_dbm_get_sys_scale_factor(
        IN    icf_glb_pdb_st                     *p_glb_pdb);

/****************************************************************************
 * FUNCTION:           icf_dbm_cmp_addr_ignr_pre_plus
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_address_st type addresses and does ignore prefixes 
 *					   in the addresses
 *                     For eg +1234567 would be same as 1234567and vice versa
 *					   Additionally this function also takes care that we dont 
 *					   treat E164 and Tel Uri as different address types
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_cmp_addr_ignr_pre_plus(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_copy_public_user_node_to_pub_uri_list 
 *
 * DESCRIPTION : This function copies a public user node to a public uri 
 *               list structure. p-associated uri lists are copied based on 
 *               the copy_bitmask.
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/


icf_return_t 
icf_dbm_copy_public_user_node_to_pub_uri_list (
                             icf_glb_pdb_st	*p_glb_pdb,
                             icf_user_db_node_st  *p_src,
                             icf_pub_uri_list_node_st  **p_p_dest,
                             icf_uint8_t copy_bitmask,
                             icf_error_t *p_ecode);

/************************************************************************
 *
 * DESCRIPTION:   The function checks for a line ID corresponding to a user.
 *				  If found returns the line id and status to be idle.Else
 *                returns invalid line id.
 *                
 ************************************************************************/

icf_return_t
icf_dbm_get_line_id_for_user(
    INOUT icf_glb_pdb_st *p_glb_pdb,
    INOUT    icf_address_st *p_user_address,
    OUT   icf_line_id_t  *p_line_id,
    OUT   icf_error_t    *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_call_ctxt_from_hash_table 
 * DESCRIPTION : This function gets the call context from hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_fetch_call_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st **p_call_ctxt,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_ssa_ctxt_from_hash_table 
 * DESCRIPTION : This function gets the ssa context from hash table 
 *               using n/w call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_fetch_ssa_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t **p_ssa_ctxt,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_call_ctxt 
 *
 * DESCRIPTION : This function add the call context into hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_add_call_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st *p_call_ctxt,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_ssa_ctxt 
 *
 * DESCRIPTION : This function adds the ssa context into hash table 
 *               using n/w call_id as key 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_add_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t *p_ssa_ctxt,
	icf_error_t *p_ecode);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_remove_call_ctxt 
 *
 * DESCRIPTION : This function removes the call context from hash table 
 *               using call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_remove_call_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint32_t call_id,
	icf_call_ctx_st *p_call_ctxt,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_remove_ssa_ctxt 
 *
 * DESCRIPTION : This function removes the ssa context from hash table 
 *               using n/w call_id 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_hash_remove_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_uint8_t *nw_call_id,
	icf_void_t *p_ssa_ctxt,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_all_active_ssa_ctxt 
 *
 * DESCRIPTION : This function gets the all ssa contexts from hash table 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_all_active_ssa_ctxt(
	icf_glb_pdb_st *p_glb_pdb,
	icf_list_st **p_p_ssa_list,
	icf_error_t *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_free_active_ssa_ctxt_list 
 *
 * DESCRIPTION : This function frees the list created by 
 *				 icf_dbm_get_all_active_ssa_ctxt 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_free_active_ssa_ctxt_list(
	icf_glb_pdb_st *p_glb_pdb,
	icf_list_st **p_p_ssa_list,
	icf_error_t *p_ecode);


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_attach_cc_to_line_wo_user
 * 
 * DESCRIPTION:   Attaches an existing Call Context block to an existing line 
 *                on which the call is going but there is not user on the same
 *
 * RETURNS:       icf_return_t 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_attach_cc_to_line_wo_user(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode);

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_non_wildcarded_uri_for_line_if_pressent
 *
 * DESCRIPTION : This function returns the pointer to the first
 *               non-barred and non wildcarded uri on the line.
                 if ONLY wildcarded user exists on line,then
                 only pointer to it is returned.
 *               Returns failure if no non-barred uri exists on the line
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/

icf_return_t icf_dbm_get_non_wildcarded_uri_for_line_if_pressent (
                                                     icf_glb_pdb_st     *p_glb_pdb,
                                                     icf_line_id_t      line_id,
                                                     icf_address_st     **p_p_uri,
                                                     icf_error_t     *p_ecode
                                                     );
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_check_if_user_exits_on_line
 *
 * DESCRIPTION:   The function checks for the line to find if a user exists
 *                on the line .
 *
 * RETURNS:       Success if the user exist on the line else returns failure.
 *
 ************************************************************************/
icf_return_t
icf_dbm_check_if_user_exits_on_line(
        INOUT icf_glb_pdb_st   *p_glb_pdb,
        INOUT    icf_address_st   *p_user_addr,
        IN    icf_line_data_st *p_line_blk,
        INOUT    icf_error_t      *p_ecode);

#ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING
/******************************************************************************
 * FUNCTION:           	icf_dbm_compare_alternate_addr
 *
 * DESCRIPTION:        	This is a utility function to derive an alternate addr
 *						format from address1 carrying phone-context parameter 
 *						and compare different combinations with address2
 *
 * RETURNS:            	Returns pointer indicating match status:
 *							 ICF_TRUE if addresses match else returns ICF_FALSE
 *
 *****************************************************************************/
icf_return_t icf_dbm_compare_alternate_addr(
        	INOUT 	icf_glb_pdb_st 		*p_glb_pdb,
        	IN 		icf_address_st 		*p_address_1,
        	IN 		icf_address_st 		*p_address_2,
			OUT		icf_boolean_t		*p_match_found);
#endif /* #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING */

/******************************************************************************
 * FUNCTION:           	icf_dbm_fetch_all_ssa_ctxt_from_hash_table
 *
 * DESCRIPTION:        	This is a utility function to fetch all SSA contexts
 *                      from hash table
 *	
 *
 * RETURNS:            	
 *		
 *
 *****************************************************************************/
icf_list_st *icf_dbm_fetch_all_ssa_ctxt_from_hash_table(
	icf_glb_pdb_st *p_glb_pdb);


/******************************************************************************
 * FUNCTION:           	icf_dbm_delete_uri_from_uri_list 
 *
 * DESCRIPTION:        	This is a utility function to deletes a particular URI
 *                      from  URI list based on search bitmask ie either
 *                      contact uri list or assoc uri list. 
 *
 * RETURNS:             ICF_SUCCESS if uri is deleted from user db node else
 *                      returns ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t icf_dbm_delete_uri_from_uri_list (
    icf_glb_pdb_st	  *p_glb_pdb,
    icf_address_st 	  *p_public_uri,
    icf_line_id_t	  line_id,
    icf_address_st    *p_uri_to_delete,
    icf_uint8_t       search_bitmask,
    icf_error_t 	  *p_ecode);


/******************************************************************************
 * FUNCTION:           	icf_dbm_clear_uri_for_user_node 
 *
 * DESCRIPTION:        	This is a utility function to deletes a particular
 *                      node from the  uri list based on the value of delete
 *                      bitmask and p_uri to be deleted  
 *
 * RETURNS:             ICF_SUCCESS if uri is deleted from the assoc uri list
 *                      else returns ICF_FAILURE
 *
 *****************************************************************************/

icf_return_t icf_dbm_clear_uri_for_user_node (
    icf_glb_pdb_st	       *p_glb_pdb,
    icf_user_db_node_st    *p_node,
    icf_address_st         *p_uri,
	icf_uint8_t	           delete_bitmask,
    icf_error_t 	       *p_ecode);


/************************************************************************
 * FUNCTION:    icf_dbm_get_default_uri_for_user_on_line   
 * 
 * DESCRIPTION: This function will return ICF_SUCCESS, if it on a
 *              given line for the particular user assoc uri is found,
 *              else it will return ICF_FAILURE. In case of success, p_uri
 *              pointer will be updated with the fetched assoc uri.
 * 
 ************************************************************************/
icf_return_t icf_dbm_get_default_uri_for_user_on_line(
                     icf_glb_pdb_st	*p_glb_pdb,
                     icf_line_id_t	line_id,                                       
                     icf_address_st     rgm_user_address, 
                     icf_address_st	*p_uri,                                          
                     icf_error_t        *p_ecode);

/************************************************************************
 * FUNCTION:    icf_dbm_cmp_addr_in_aor
 * 
 * DESCRIPTION: This function will return ICF_SUCCESS, if the AOR we have 
 *              and the AOR received fromm networkin the notify are same,
 *              else it will return ICF_FAILURE.
 * 
 ************************************************************************/

icf_boolean_t
icf_dbm_cmp_addr_in_aor(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2);

/******************************************************************************
 *
 * FUNCTION:        icf_dbm_init_timer_data_blk
 *
 * DESCRIPTION:     This is a utility function that initializes timer data 
 *                  block with default values.
 *
 ******************************************************************************/
icf_void_t 
icf_dbm_init_timer_data_blk(
    INOUT icf_timer_data_st    *p_timer_data);


/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_complete_addr
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_address_st type addresses completely.    
 *
 * RETURNS:            Returns ICF_SUCCESS if addresses match or
 *                     any Input variable is NULL.
 *                     Else returns ICF_FAILURE
 *
 ***************************************************************************/
icf_return_t
icf_dbm_util_cmp_complete_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2);

/* SPR 19532 */
/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_username
 * DESCRIPTION:        This is a utility function to compare the 
 *                     username part of two icf_address_st type addresses    
 * RETURNS:            Returns ICF_TRUE if address matches else returns ICF_FALSE
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_username(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2);

/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_fetch_ssa_ctxt_from_via_mapped_hash_table 
 * DESCRIPTION : This function gets the ssa context from hash table 
 *               using n/w via 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_fetch_ssa_ctxt_from_via_mapped_hash_table(
	INOUT   icf_glb_pdb_st *p_glb_pdb,
	IN      icf_uint8_t    *p_nw_via,
	INOUT   icf_void_t     **p_ssa_ctxt,
	INOUT   icf_error_t    *p_ecode);

/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_hash_add_ssa_ctxt_mapped_with_via 
 *
 * DESCRIPTION : This function adds the ssa context into hash table 
 *               using n/w via as key 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_hash_add_ssa_ctxt_mapped_with_via(
	INOUT    icf_glb_pdb_st    *p_glb_pdb,
	IN       icf_uint8_t       *p_nw_via,
	INOUT    icf_void_t        *p_ssa_ctxt,
	INOUT    icf_error_t       *p_ecode);

/*Fix for SPR: 19999 (Merged CSR 1-7586127 : SPR-19988)*/
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_remove_via_mapped_ssa_ctxt 
 *
 * DESCRIPTION : This function removes the ssa context from hash table 
 *               using n/w via 
 *              
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t icf_dbm_remove_via_mapped_ssa_ctxt(
	            INOUT   icf_glb_pdb_st *p_glb_pdb,
	            INOUT   icf_uint8_t    *p_nw_via,
	            INOUT   icf_void_t     *p_ssa_ctxt,
	            INOUT   icf_error_t    *p_ecode);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ICF_DBM_PROTOTYPES_H__ */


