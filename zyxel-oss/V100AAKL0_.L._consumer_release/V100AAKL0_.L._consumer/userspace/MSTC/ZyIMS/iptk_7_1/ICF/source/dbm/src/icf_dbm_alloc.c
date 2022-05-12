/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_alloc.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality of allocation of blocks from DBM's pool or 
 *                    for allocation of memory through DBM. 
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                File created and initial version
 *  01 APR 2005   Sumit Gupta     SPR 7339   Change in 
 *                                           icf_dbm_util_cmp_trans_addr 
 *                                           function
 *  21 APR 2005   Sumit Gupta     SPR 7416   Replaced call to macro 
 *                                           ICF_MEMFREE with 
 *                                           ICF_STATIC_MEMFREE 
 *	08-Mar-2006     Aman Aggarwal            ICF merging with ICF2.0
 *  02-Apr-2006	  Puneet Sharma	  ICF5.0 LLD changes done in the comments 
 *											 Multiple Public Users ICF5.0
 *  06-Dec-2007   Amit Sharma                6_4 merge to PQT 7.0
 *
 *  -5-Feb-2007   Amit Sikka     SPR 18066   Warning removal if traces off
 *  26-Jun-2009   Anurag Khare   CSR 1-7437801  Function added icf_dbm_cmp_addr_in_aor
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_dbm_defs.h"
#include "icf_dbm_macro.h"
#include "icf_dbm_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_dbm_sc_prototypes.h"
#include "icf_macro.h"
#include "icf_port_prototypes.h"
#include"icf_ssa_common_types.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_sc_common_types.h"
#endif /* #ifdef ICF_SERVICE_ENABLE */
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_srvc_cntxt_blk
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              block list and returns a pointer to it   
 *
 * RETURNS: icf_return_t
 *
************************************************************************/ 
icf_return_t 
icf_dbm_alloc_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        OUT   icf_service_ctx_st                **p_p_srvc_cntxt_blk,
        OUT   icf_error_t                       *p_ecode)

{
    
    icf_return_t                    ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
        
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if (ICF_NULL == p_glb_pdb->p_call_ctx)
    {
#ifdef ICF_ERROR_ENABLE
        ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                ICF_NO_CALL_CTX_EXIST,p_ecode)
#endif            
        ret_val = ICF_FAILURE;
    }/* End if (ICF_NULL == p_glb_pdb->p_call_ctx)*/
     else if(ICF_FAILURE == icf_dbm_alloc_blk(p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_SRVC_CTX,
        (icf_void_t **)p_p_srvc_cntxt_blk,
        p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Initiailze service context block to 0*/
        icf_port_memset(*p_p_srvc_cntxt_blk,(icf_uint8_t const)0,
            (icf_uint32_t const)sizeof(icf_service_ctx_st));

    }/* End Else*/
    
         
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
        
   return (ret_val);

}
#endif /* #ifdef ICF_SERVICE_ENABLE */


#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_srvc_cntxt_lgc_blk
 *
 * DESCRIPTION: The function fetches a node from unused service context
 *              logic block list and returns a pointer to it
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t  
icf_dbm_alloc_srvc_cntxt_lgc_blk(
   INOUT icf_glb_pdb_st                       *p_glb_pdb,
   OUT    icf_void_t                          **p_p_srvc_cntxt_lgc_blk,
   OUT    icf_error_t                         *p_ecode)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
       
    
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
        
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    if(ICF_FAILURE == icf_dbm_alloc_blk(p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_SL_CTX,
        p_p_srvc_cntxt_lgc_blk,
        p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /*
         * Initialize all bytes in service logic context block to 0
         */
        icf_port_memset((*p_p_srvc_cntxt_lgc_blk),(icf_uint8_t const)0,
                (icf_uint32_t const)sizeof(icf_srvc_ctx_lgc_ut));
    }
   
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return (ret_val);
} 
#endif /* #ifdef ICF_SERVICE_ENABLE */

 /************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_rgn_blk
 *
 * DESCRIPTION: Returns the index of a free registration block, which could be 
 *              used to save registration information for a user
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t icf_dbm_alloc_rgn_blk(            
        INOUT icf_glb_pdb_st                   *p_glb_pdb,                   
        OUT   icf_rgm_context_st               **p_p_registration_data,
        OUT   icf_error_t                      *p_ecode)
{

    icf_return_t                   ret_val = ICF_SUCCESS;
    

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
        
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(ICF_SUCCESS == icf_dbm_alloc_blk(p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_RGN_CTX,
        (icf_void_t **)p_p_registration_data,
        p_ecode))    
    {
        /*
         * Initialize the registration context block to its default values
         */ 
   		ICF_PRINT(((icf_uint8_t*)"\n[DBM]: RGN block allocated and getting initialized\n"));

        icf_dbm_init_rgn_ctx(
               p_glb_pdb,
               *p_p_registration_data);
    }
   
	else
    {
        ret_val = ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_glb_pdb)
     
    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);
}


 /************************************************************************
 *
 * FUNCTION NAME:icf_dbm_alloc_line_blk
 *
 * DESCRIPTION: Allocates memory for line data block and returns a pointer to it
 * 
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_alloc_line_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_line_id_t                     line_id,
       OUT   icf_line_data_st                   **p_p_line_data,
       OUT   icf_error_t                       *p_ecode)

{
     icf_line_data_st **p_p_temp_line_data;
     icf_return_t ret_val = ICF_SUCCESS;
     icf_config_data_st *p_config_data = ICF_NULL;
   
     ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
      
     ICF_FUNCTION_ENTER(p_glb_pdb)
         
     p_p_temp_line_data = p_glb_pdb->p_glb_data->p_p_line_data + line_id;

     ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);
    
     ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

     /* Allocate emory for storing the line data */   
     ICF_STATIC_MEMGET(p_glb_pdb,
             sizeof(icf_line_data_st),        
             *p_p_temp_line_data,
             ICF_RET_ON_FAILURE,
             p_ecode, ret_val)
     
     /* 
      * Initialize the User adress list and user priority list pointers to 
      * NULL
      */ 
/* Start: Multiple Public Users ICF5.0 */
     (*p_p_temp_line_data)->p_user_db = ICF_NULL;
/* End: Multiple Public Users ICF5.0 */
#ifdef icf_MULTILINE    
     (*p_p_temp_line_data)->p_user_priority_list = ICF_NULL;
#endif

     /* 
      * If the memory was successfully allocated for line datat then allocate 
      * memory for maintaining list of users on line
      */ 

#ifdef ICF_MULTILINE
     if(ICF_FAILURE != ret_val)
     {
         /* Allocate memory for maintaining user's priority*/
         ICF_STATIC_MEMGET(p_glb_pdb,
                 sizeof(icf_priority_id_t) * 
                 p_config_data->max_users_per_line ,     
                 (*p_p_temp_line_data)->p_user_priority_list,
                 ICF_DONOT_RET_ON_FAILURE,
                 p_ecode, ret_val)
     }
#endif


     if(ICF_FAILURE == ret_val)
     {
#ifdef ICF_MULTILINE
         if(ICF_NULL != (*p_p_temp_line_data)->p_user_addr_list)
         {
             ICF_STATIC_MEMFREE(
                 p_glb_pdb, 
                 (*p_p_temp_line_data)->p_user_addr_list, 
                 p_ecode, 
                 ret_val)
         }             
#endif

         ICF_STATIC_MEMFREE(
                 p_glb_pdb, 
                 *p_p_temp_line_data, 
                 p_ecode, 
                 ret_val)

         ret_val = ICF_FAILURE; 
     }
     else 
     {
         /*
          * Set line id in current line blk
          */
         (*p_p_temp_line_data)->line_id = line_id;

         *p_p_line_data = *p_p_temp_line_data;
     }

     p_ecode = p_ecode;/*To remove warning*/

     ICF_FUNCTION_EXIT(p_glb_pdb)

     ICF_RESET_MODULE(p_glb_pdb)     

     return (ret_val) ;

}/*icf_dbm_alloc_line_blk*/

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_call_cntxt_blk
 *
 * DESCRIPTION: Would allocate a call context block from free node list and 
 *              allocate its pointer to call context pointer in global pdb
 * 
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_alloc_call_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        INOUT icf_call_id_t                     *p_call_id, 
        OUT   icf_call_ctx_st                   **p_p_call_context_data,
        OUT   icf_error_t                       *p_ecode)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_dbm_blk_id_t               new_call_cntxt_blk_id;

#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st               trace_data;
#endif
    icf_dbm_pool_info_st      *p_pool = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

   
    if( ICF_FAILURE == icf_dbm_alloc_blk(p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_CALL_CTX,
        (icf_void_t **)p_p_call_context_data,
        p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* 
         * Get the pool address for registration context pool
         */  
        p_pool = icf_dbm_get_pool_addr(
                p_glb_pdb,
                ICF_BLK_TYPE_CALL_CTX);
        ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pool))

        /*
         * This function is not expected to return failure as in this scenarion
         * since the call context address is returned for a successfull 
         * alloctation, it would have a correct index in DBM pool
         */
        icf_dbm_ret_index_for_addr(
                p_glb_pdb,
                p_pool->p_blk_array,
                (icf_void_t *)(*p_p_call_context_data),
                p_pool->block_size,
                &new_call_cntxt_blk_id,
                p_ecode);

        /* Initialize the fields of the newly allocated control block */
        p_glb_pdb->p_call_ctx = *p_p_call_context_data;
        
        /*
         * If the call id given is an invalid call id
         */
        if((ICF_INVALID_CALL_ID & *p_call_id) == ICF_INVALID_CALL_ID)
        {
            *p_call_id = new_call_cntxt_blk_id;
            
            /*
             * Set the first bit on which would indicate it is a call id for an
             *  outgoing call
             */
            *p_call_id = (icf_uint16_t)(*p_call_id | 0X8000); 
        }
        
        ret_val = icf_dbm_init_call_ctx(
                    p_glb_pdb,
                    *p_p_call_context_data);
		
		ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

        /*
         * Fill the call id in call ontext block
         */
        (*p_p_call_context_data)->call_id = *p_call_id;

#ifdef ICF_TRACE_ENABLE

        trace_data.int_val[0] = new_call_cntxt_blk_id;
        trace_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif

        ICF_TRACE(p_glb_pdb,
                (icf_trace_id_t const)(ICF_CALL_CTX_ALLOCATE),&trace_data)

        ICF_PRINT(((icf_uint8_t*)"\nCall context block allocated for Call id = %d\n",
            *p_call_id));

#ifdef ICF_LOAD_DEBUG
#ifdef ICF_PORT_LINUX
        struct timeval tv;
        gettimeofday(&tv, ICF_NULL);
        printf("\ncall-ctx ALLOCATED call-id	%d time	%d\n",*p_call_id,tv.tv_sec);             
#else
        printf("\ncall-ctx ALLOCATED call-id	%d\n",*p_call_id); 
#endif
#endif

    }/*end else*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_alloc_sic_cntxt_blk
 *
 * DESCRIPTION: Would allocate a sic context block from free node list and 
 *              allocate its pointer to call context pointer in global pdb
 * 
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_alloc_sic_cntxt_blk(
        INOUT icf_glb_pdb_st                    *p_glb_pdb,
        INOUT icf_call_id_t                     *p_call_id, 
        OUT   icf_sic_ctx_st                   **p_p_sic_context_data,
        OUT   icf_error_t                       *p_ecode)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_dbm_sic_cntxt_blk_id_t    new_sic_cntxt_blk_id;
    icf_dbm_data_st                *p_dbm_data = ICF_NULL;
    icf_uint16_t                    status = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(     
            p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
            (icf_void_t *)&p_dbm_data,
            p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* Call linked array allocate function to allocate a free sic context 
     * block */

    if(ICF_FAILURE == icf_dbm_la_allocate(
		    ICF_CMN_SIC_MAX_CALL_CONTEXTS,
                p_dbm_data->p_available_sic_cntxt_lst, 
                &p_dbm_data->sic_cntxt_blk_next_free_id,
                &new_sic_cntxt_blk_id, 
                &status, p_ecode) ||ICF_DBM_CTRL_BLK_EXHAUSTED == status)
    { 
        ICF_SET_ECODE(p_glb_pdb,ICF_NULL,
                ICF_CALL_CTX_BLK_EXHAUSTED,p_ecode)
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Initialize the fields of the newly allocated control block */
       /* p_glb_pdb->p_call_ctx =  */
       /*     (((icf_call_ctx_st *)p_dbm_data->p_call_cntxt_blk) +*/
       /*      new_call_cntxt_blk_id);*/
        *p_p_sic_context_data   = 
            (((icf_sic_ctx_st *)p_dbm_data->p_sic_cntxt_blk) +
            new_sic_cntxt_blk_id);
		
		 /*
         * If the call id given is an invalid call id
         */
        if((ICF_INVALID_CALL_ID & *p_call_id) == ICF_INVALID_CALL_ID)
        {
            *p_call_id = new_sic_cntxt_blk_id;
            
            /*
             * Set the first bit on which would indicate it is a call id for an
             *  outgoing call
             */
            *p_call_id = (icf_call_id_t)(*p_call_id | 0XC000); 
        }
        
        ret_val = icf_dbm_init_sic_ctx(
                    p_glb_pdb,
                    *p_p_sic_context_data);

        ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

        /*
         * Fill the call id in call ontext block
         */
        (*p_p_sic_context_data)->call_id = *p_call_id;


    }/*end else*/

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    

    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_create_sdp_block
 *
 * DESCRIPTION: This function would allocate memory for a new SDP block
 *
 * RETURNS: icf_return_t
 * 
 ************************************************************************/  
icf_return_t 
icf_dbm_create_sdp_block(
       INOUT icf_glb_pdb_st               *p_glb_pdb,
       OUT icf_void_t                     **p_p_sdp_data,
       OUT icf_error_t                    *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    /*allocate memory for SDP block*/ 
  
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)    
     
    ICF_FUNCTION_ENTER(p_glb_pdb)       
   
    ret_val = icf_dbm_get_mem_from_pool(
            p_glb_pdb,
            (icf_pool_id_t const)ICF_MEM_SDP,
            p_p_sdp_data,
            p_ecode);

    ICF_FUNCTION_EXIT(p_glb_pdb)
        
    ICF_RESET_MODULE(p_glb_pdb)
 
    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_create_addr_blk
 *
 * DESCRIPTION: It will allocate the memory  for the Address block
 *
 * RETURNS: icf_return_t
 *
************************************************************************/  
icf_return_t 
icf_dbm_create_addr_blk(
       INOUT icf_glb_pdb_st     *p_glb_pdb,
       OUT icf_void_t           **p_p_addr_data,
       OUT icf_error_t          *p_ecode)
{
    /*Allocate memory for the Address block and each member of it */
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /*allocate memory for address block*/ 
    ICF_MEMGET( p_glb_pdb,
            sizeof(icf_address_st),        
            ICF_MEM_COMMON,
            *p_p_addr_data,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
     
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_create_timer_block
 *
 * DESCRIPTION: This function would allocate memory for a new timer block
 *
 * RETURNS: icf_return_t
 *
************************************************************************/  
icf_return_t 
icf_dbm_create_timer_block(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       OUT   icf_void_t                                **p_p_timer_data,
       OUT   icf_error_t                               *p_ecode)
{
/*Allocate memory for the Address block and each member of it */
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /*allocate memory for address block*/ 
    ICF_MEMGET( p_glb_pdb,
            sizeof(icf_timer_data_st),       
            ICF_MEM_TIMER,            
            *p_p_timer_data,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)
     
    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);
}

/****************************************************************************
 * FUNCTION:           icf_dbm_chk_line_in_rgn
 *
 * DESCRIPTION:       Check whether the line id is present in the registration 
 *                    block's list of lines user is registered on.
 *
 * RETURNS:           Returns failure if line id is not present else return 
 *                    success.  
 *
 ***************************************************************************/
icf_return_t
icf_dbm_chk_line_in_rgn(
        INOUT icf_glb_pdb_st       *p_glb_pdb,
        IN    icf_line_id_t        line_id,
        IN    icf_rgm_context_st   *p_rgn_data)
{
    icf_line_id_t   line_count = 0;
    icf_return_t  ret_val = ICF_FAILURE;   
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    while(line_count < p_rgn_data->num_of_lines)
    {
        if(line_id == p_rgn_data->rgm_list_of_lines[line_count])
        {
            ret_val = ICF_SUCCESS;
			break;
        }
        line_count++;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}

/****************************************************************************
 * FUNCTION:         icf_dbm_del_line_in_rgn
 *
 * DESCRIPTION:      Deletes a line from users list of registered lines 
 *
 * RETURNS:          Returns success if line is deleted successfully else return
 *                   failure   
 *
 ***************************************************************************/
icf_return_t
icf_dbm_del_line_in_rgn(
        icf_glb_pdb_st       *p_glb_pdb,
        icf_line_id_t        line_id,
        icf_rgm_context_st   *p_rgn_data)
{
    icf_line_id_t   line_count = 0;
    icf_return_t  ret_val = ICF_SUCCESS;   

    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    
    ICF_FUNCTION_ENTER(p_glb_pdb)    
    
    while(line_count < p_rgn_data->num_of_lines
            && line_id != p_rgn_data->rgm_list_of_lines[line_count])
    {
        line_count += 1;
    }
    if(line_count == p_rgn_data->num_of_lines)/*This wouldn't happen as a 
                                                    check in there earlier as
                                                    well*/
    {
        /*Error line could not be found*/
        ret_val =ICF_FAILURE;
    }
    /* if the line is the last line in line list */
    else if(line_count == p_rgn_data->num_of_lines - 1)
    {
        p_rgn_data->num_of_lines -= 1;
    }
    else
    {
        /* Move all the lines in the list, after the line id to be deleted, to 
         * left*/
        while(line_count + 1 < p_rgn_data->num_of_lines)
        {
            p_rgn_data->rgm_list_of_lines[line_count] =     
                p_rgn_data->rgm_list_of_lines[line_count + 1] ;
            line_count++;            
        }
        p_rgn_data->num_of_lines -= 1;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)    
    return ret_val;
}


/****************************************************************************
 * FUNCTION:   icf_dbm_create_all_rgn_blks
 *
 * DESCRIPTION: The function would be called to create registration blocks for 
 *              all users once the registration information and user information
 *              is received 
 *
 * RETURNS:    ICF_SUCCESS    
 *
 ***************************************************************************/
icf_return_t
icf_dbm_create_all_rgn_blks(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      OUT    icf_uint16_t         *p_num_of_blks_created,
      OUT    icf_error_t          *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
     /*To Remove warnings */
      p_glb_pdb=p_glb_pdb;
      p_num_of_blks_created=p_num_of_blks_created;
      p_ecode=p_ecode;
#ifndef ICF_FEATURE_ABANDON
    icf_config_data_st *p_config_data = ICF_NULL;
    icf_uint16_t user_count = ICF_NULL;
    icf_rgm_context_st *p_rgn_blk_addr = ICF_NULL;
    icf_line_data_st   **p_p_line_data = ICF_NULL;
    icf_uint8_t       status = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
   
    ICF_FUNCTION_ENTER(p_glb_pdb)
     
    /*Make p_p_line_data to point to line list in glb PDB*/
    p_p_line_data = p_glb_pdb->p_glb_data->p_p_line_data;

    ret_val = icf_dbm_get_module_glb_data(     
                p_glb_pdb,
                (icf_int_module_id_t const)ICF_INT_MODULE_CFG,
                (icf_void_t *)&p_config_data,
                p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    *p_num_of_blks_created = 0;

    for(line_count = 0;line_count<p_config_data->max_lines;
            line_count++)
    {
        /*If line block does not exist*/
        if(ICF_NULL == p_p_line_data[line_count])
        {
            continue;
        }
        /* If either registration information is not present or there are no
         * users in the user address list
         */
        else if((ICF_NULL == 
                    p_p_line_data[line_count]->p_user_addr_list)
                || (ICF_CFG_LINE_DATA_REGISTRAR_RECD != 
                    (p_p_line_data[line_count]->line_data_received
                     & ICF_CFG_LINE_DATA_REGISTRAR_RECD)))
        {
            continue;
        }
        /* Create registration block*/
        else
        {
            user_count = 0;
            while(user_count < p_p_line_data[line_count]->num_of_user_addr)
            {
                if(ICF_SUCCESS == icf_dbm_regm_add_user(
                            p_glb_pdb,
                            line_count,
                            &p_p_line_data[line_count]->p_user_addr_list[user_count],
                            &status,
                            &p_rgn_blk_addr,
                            p_ecode))
                {
                    if(ICF_RGM_DBM_NEW_BLOCK == status)
                    {
                        *p_num_of_blks_created += 1;
                    }
                }
                user_count += 1;
            }/*End While*/
        }/*End else*/
    }/*End for*/

    ICF_FUNCTION_EXIT(p_glb_pdb)    

    ICF_RESET_MODULE(p_glb_pdb)
#endif
    return ret_val;
}

/****************************************************************************
 * FUNCTION:           icf_dbm_regm_add_user
 *
 * DESCRIPTION:        The function would be called to add a new registrtion for
 *                     the given user on the specified line 
 *
 * RETURNS:            Returns success if user block is added successfully 
 *                     else returns failure 
 *
 ***************************************************************************/
icf_return_t
icf_dbm_regm_add_user(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      IN     icf_line_id_t        line_id,
      IN     icf_address_st       *p_user_addr,
      OUT  icf_uint8_t            *p_status,
      OUT    icf_rgm_context_st  **p_p_rgn_blk_addr,
      OUT    icf_error_t          *p_ecode)
{

    icf_return_t ret_val = ICF_SUCCESS;
    icf_rgm_context_st *p_rgn_blk_addr = ICF_NULL;
    icf_line_data_st   **p_p_line_data = ICF_NULL;
    icf_dbm_blk_id_t rgn_cntxt_blk_next_used_id;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    icf_line_id_t              first_line_rgn_blk; 
	icf_dbm_pool_info_st  *p_rgn_pool_addr = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    /*Make p_p_line_data to point to line list in glb PDB*/
    p_p_line_data = p_glb_pdb->p_glb_data->p_p_line_data;

    ret_val = icf_dbm_get_module_glb_data(     
            p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
            (icf_void_t *)&p_dbm_data,
            p_ecode);

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    /* 
     * Get the pool address for registration context pool
     */  
    p_rgn_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_RGN_CTX);
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgn_pool_addr))
   
    rgn_cntxt_blk_next_used_id = p_dbm_data->rgn_cntxt_blk_next_used_id;

    while(p_rgn_pool_addr->pool_size !=
            rgn_cntxt_blk_next_used_id)
    {
        p_rgn_blk_addr = 
            ((icf_rgm_context_st *)p_rgn_pool_addr->p_blk_array) + 
            rgn_cntxt_blk_next_used_id ;

        if(ICF_TRUE == icf_dbm_util_cmp_addr(
                    p_glb_pdb,
                    &p_rgn_blk_addr->rgm_user_address,
                    p_user_addr))
        {
            if(ICF_SUCCESS == icf_dbm_chk_line_in_rgn(
                        p_glb_pdb,
                        line_id,
                        p_rgn_blk_addr))
            {
                if(1 == p_rgn_blk_addr->num_of_lines)
                {
                    *p_status = ICF_RGM_DBM_PAIR_PRESENT_ALONE;
                }
                else if(p_p_line_data[line_id]->registration_duration < 
                        p_rgn_blk_addr->rgm_duration)

                {
                    p_rgn_blk_addr->rgm_duration = 
                        p_p_line_data[line_id]->registration_duration;
                    
                    *p_status = ICF_RGM_DBM_DURATION_CHANGE;
                    *p_p_rgn_blk_addr = p_rgn_blk_addr;
                }
                else
                {
                    *p_status = ICF_RGM_DBM_NO_ACTION;
                }

                ret_val = ICF_SUCCESS;

                ICF_FUNCTION_EXIT(p_glb_pdb)    

                ICF_RESET_MODULE(p_glb_pdb)

                return ret_val; /* If the line was found return*/
            }

            first_line_rgn_blk = p_rgn_blk_addr->rgm_list_of_lines[0];
            /*Compare the registrar address for the registrar address of
             * first line in registration block.
             * */
            if(ICF_TRUE == icf_dbm_util_cmp_trans_addr(
                        p_glb_pdb,
                        &p_p_line_data[first_line_rgn_blk]->registrar_addr,
                        &p_p_line_data[line_id]->registrar_addr))
            {
                /*Add the requested line id in currnt block*/
               p_rgn_blk_addr->rgm_list_of_lines[p_rgn_blk_addr->num_of_lines] =
                    line_id;

                p_rgn_blk_addr->num_of_lines++;

                if(p_p_line_data[line_id]->registration_duration < 
                        p_rgn_blk_addr->rgm_duration)

                {
                    p_rgn_blk_addr->rgm_duration = 
                        p_p_line_data[line_id]->registration_duration;
                    *p_status = ICF_RGM_DBM_DURATION_CHANGE;
                }
                else
                {
                    *p_status = ICF_RGM_DBM_NO_ACTION;
                }

                *p_p_rgn_blk_addr = p_rgn_blk_addr;
                ret_val = ICF_SUCCESS;
                break;
            }/*End if*/
        } 
        rgn_cntxt_blk_next_used_id =
          (((icf_dbm_la_st *)(p_rgn_pool_addr->p_available_blk_lst)) + 
           rgn_cntxt_blk_next_used_id)->next_index;
    }/*end while*/

    /*
     * If no registration block was found for the user
     */
    if(p_rgn_pool_addr->pool_size ==
            rgn_cntxt_blk_next_used_id)
    {
        if(ICF_FAILURE ==  icf_dbm_alloc_rgn_blk(            
                    p_glb_pdb,
                    &p_rgn_blk_addr,
                    p_ecode))
        {
            *p_status = ICF_RGM_DBM_NO_FREE_BLOCK;
        }
        else
        {
            p_rgn_blk_addr->num_of_lines = 0;
            
            icf_port_memcpy(
                    &p_rgn_blk_addr->rgm_user_address,
                    p_user_addr,              
                    (icf_uint32_t const)sizeof(icf_address_st));

            p_rgn_blk_addr->rgm_list_of_lines[p_rgn_blk_addr->num_of_lines] = 
                line_id;
            p_rgn_blk_addr->num_of_lines++;
            p_rgn_blk_addr->rgm_duration = 
                p_p_line_data[line_id]->registration_duration;

            *p_p_rgn_blk_addr = p_rgn_blk_addr;
            *p_status = ICF_RGM_DBM_NEW_BLOCK;
        }
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)    

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
}


/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_addr
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_address_st type addresses    
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2)
{
    /* SPR 17636: CSR 1-5390722: There was a bug when ret_val was initialized 
     * to TRUE:If one of the addr_type was 0, this function was not going inside
     * if-block and was always returning TRUE. 
     * Hence initialized it to ICF_FALSE below to make sure the function 
     * returns FALSE in case one of the address type is invalid.
     */
    icf_boolean_t ret_val = ICF_FALSE;
      icf_address_st  temp_addr1 ;
    icf_address_st  temp_addr2 ;
    icf_uint8_t     *p_temp_str1 = ICF_NULL;
    icf_uint8_t     *p_temp_str2 = ICF_NULL;

    /* address types E.164 and sip will be same, as the call
     * for E.164 will be always received as sip address*/
    if((p_adress1->addr_type ==  p_adress2->addr_type) ||
        (p_adress1->addr_type==ICF_ADDRESS_PLAN_SIP_URI &&
        p_adress2->addr_type == ICF_ADDRESS_PLAN_E164) ||
        (p_adress1->addr_type == ICF_ADDRESS_PLAN_E164 &&
        p_adress2->addr_type == ICF_ADDRESS_PLAN_SIP_URI))
    {

        icf_port_memcpy(&temp_addr1,p_adress1, sizeof(icf_address_st));
        icf_port_memcpy(&temp_addr2,p_adress2, sizeof(icf_address_st));
        /* compare the address portion before @ sign */
        p_temp_str1 = (icf_uint8_t *)icf_port_strtok((icf_int8_t *)temp_addr1.addr_val.str,"@");
        p_temp_str2 = (icf_uint8_t *)icf_port_strtok((icf_int8_t*)temp_addr2.addr_val.str,"@");
       if(p_temp_str1 != ICF_NULL && p_temp_str2 !=ICF_NULL)
       {
        if(ICF_NULL == icf_port_strcmp(p_temp_str1, p_temp_str2))
        {
            ret_val = ICF_TRUE;
        }
       }
       /* in case there is no @ symbol compare the whole address */
       else
       {
           if(ICF_NULL == icf_port_strcmp(p_adress1->addr_val.str, 
               p_adress2->addr_val.str))
           {
               ret_val = ICF_TRUE;
           }
       }

    }

   p_glb_pdb = p_glb_pdb;

   ICF_FUNCTION_EXIT(p_glb_pdb)  

   return ret_val;
}

/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_trans_addr
 *
 * DESCRIPTION:        This is a utility function to compare two 
 *                     icf_transport_address_st type addresses    
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/
icf_boolean_t
icf_dbm_util_cmp_trans_addr(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN    icf_transport_address_st *p_address1,
        IN    icf_transport_address_st *p_address2)
{
    icf_return_t ret_val = ICF_TRUE;
    
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(p_address1->port_num != p_address2->port_num )
    {
        ret_val = ICF_FALSE;
    }
    else if(p_address1->addr.addr_type != p_address2->addr.addr_type)
    {
        ret_val = ICF_FALSE;
    }
    else
    {
        if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_address1->addr.addr_type)
        {
            /*Fix for SPR 7339 starts*/
            /*IF strings doesn't match return false*/
            if(0 != icf_port_strcmp(
                        p_address1->addr.addr.domain.str,
                        p_address2->addr.addr.domain.str))
            {
                ret_val = ICF_FALSE;
            }
            /*Fix for SPR 7339 ends*/
        }
        else{
			ret_val = icf_port_compare_ip_addr(p_address1->addr,p_address2->addr);
		}
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}


/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_create_ssa_timer_blk
 *
 * DESCRIPTION: This function would allocate memory for a new timer block for 
 *              SSA
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_create_ssa_timer_blk(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       OUT   icf_void_t                                **p_p_timer_data,
       OUT   icf_error_t                               *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_mem_from_pool(
            p_glb_pdb,
            (icf_pool_id_t const)ICF_MEM_SSA_TIMER,
            p_p_timer_data,
            p_ecode);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_get_mem_from_pool
 *
 * DESCRIPTION: This function would allocate memory for specified pool 
 *              and return the pointer in pointer variable passed as 
 *              argument
 *
 * RETURNS: icf_return_t
 *
************************************************************************/
icf_return_t
icf_dbm_get_mem_from_pool(
       INOUT icf_glb_pdb_st                            *p_glb_pdb,
       IN    icf_pool_id_t                             pool_id,
       OUT   icf_void_t                                **p_p_mem_blk,
       OUT   icf_error_t                               *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
	icf_uint16_t size = 0;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
	
	switch(pool_id)
    {
        case ICF_MEM_SDP:
            size = sizeof(icf_internal_sdp_stream_st);
            break;

        case ICF_MEM_TIMER:
            size = sizeof(icf_timer_data_st);
            break;

        case ICF_MEM_CALL_CTX_LIST:
            size = sizeof(icf_call_ctx_list_st);
            break;

        case ICF_MEM_ADDRESS:
            size = sizeof(icf_address_st);
            break;

        case ICF_MEM_SSA_TIMER:
            size = sizeof(icf_ssa_timer_data_st);
            break;

        default:
            ret_val = ICF_FAILURE;
            break;
    }

    /* 
     * If size was successfully identified means the funcion is called with a 
     * valid pool ID
     */ 
    if(ICF_NULL != size)
    {
        /*
		* Allocate memory from POOl. The size is specified as ICF_NULL 
		* since it would be immaterial when pool id is specified
		*/
		ICF_MEMGET( p_glb_pdb,
            size,
            pool_id,
            *p_p_mem_blk,
            ICF_RET_ON_FAILURE,
            p_ecode, ret_val)

        icf_port_memset(*p_p_mem_blk, 0, size);
	}

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);
}

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
        IN    icf_transport_address_st *p_address2)
{
    icf_return_t ret_val = ICF_TRUE;
    
    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    if(p_address1->addr.addr_type != p_address2->addr.addr_type)
    {
        ret_val = ICF_FALSE;
    }
    else
    {
        if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == p_address1->addr.addr_type)
        {
            /*Fix for SPR 7339 starts*/
            /*IF strings doesn't match return false*/
            if(0 != icf_port_strcmp(
                        p_address1->addr.addr.domain.str,
                        p_address2->addr.addr.domain.str))
            {
                ret_val = ICF_FALSE;
            }
            /*Fix for SPR 7339 ends*/
        }
        else 
		{
			ret_val = icf_port_compare_ip_addr(p_address1->addr,p_address2->addr);
	    }
	}
    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
}

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
        OUT   icf_void_t                       **p_p_block_addr,
        OUT   icf_error_t                        *p_ecode)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_dbm_blk_id_t                new_blk_id = ICF_NULL;
    icf_dbm_data_st                 *p_dbm_data = ICF_NULL;
#ifdef ICF_ERROR_ENABLE
    icf_error_data_st               error_data;   
#endif
    icf_uint16_t                     status = ICF_NULL; 
    icf_dbm_pool_info_st  *p_pool_addr = ICF_NULL;
#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st               trace_data;
#endif

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)


    ret_val = icf_dbm_get_module_glb_data(     
            p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
            (icf_void_t *)&p_dbm_data,
            p_ecode);
    
    /* 
     * Get the pool address for registration context pool
     */  
    p_pool_addr = icf_dbm_get_pool_addr(
            p_glb_pdb,
            block_type);
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pool_addr))
	ICF_PRINT(((icf_uint8_t *)"\n[DBM]: icf_dbm_alloc_blk triggered for block-type %d of " \

					"pool-size %d\n", block_type, p_pool_addr->pool_size));

    if (ICF_FAILURE == icf_dbm_la_allocate(
                p_pool_addr->pool_size,
                p_pool_addr->p_available_blk_lst,
                &p_pool_addr->next_free_id,
                &new_blk_id, 
                &status,p_ecode)|| 
            (ICF_DBM_CTRL_BLK_EXHAUSTED == status))
    {
#ifdef ICF_ERROR_ENABLE
        error_data.int_val[1] = block_type;   
        error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
        ICF_SET_ECODE (p_glb_pdb,ICF_NULL,
                ICF_POOL_EXHAUSTED,p_ecode)
#endif
#ifdef ICF_LOAD_STAT
        printf("DBM blocks EXHAUSTED\n");
#endif
        ICF_PRINT(((icf_uint8_t *)"\n[DBM]: ALERT !! Pool exhausted\n"));
        ret_val = ICF_FAILURE;
    }
    else
    {
        *p_p_block_addr = (((icf_uint8_t *)
               p_pool_addr->p_blk_array) +
                (p_pool_addr->block_size * 
                 new_blk_id));

        /*If the request was forr allocation of registration context block then
         *update used nodes list for registration context block
         */
        if(ICF_BLK_TYPE_RGN_CTX == block_type)
        {
           	ICF_PRINT(((icf_uint8_t *)"\n[DBM]: icf_dbm_alloc_blk RGN new_blk_id = %d\n", \
			new_blk_id));
            
            /*
             * Put the next node as the next node in used node list in allocated 
             * block.
             */
            (((icf_dbm_la_st *)(p_pool_addr->p_available_blk_lst)) + 
             new_blk_id)->next_index = 
                p_dbm_data->rgn_cntxt_blk_next_used_id;
            /*
             * Next used node now would be the allocated node
             */
            p_dbm_data->rgn_cntxt_blk_next_used_id = new_blk_id;
        }
    }

    if(ICF_SUCCESS == ret_val)
    {
#ifdef ICF_TRACE_ENABLE
        trace_data.int_val[0] = block_type;
        trace_data.int_val[1] = new_blk_id;
        trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
        trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif

        ICF_TRACE(p_glb_pdb,
             (icf_trace_id_t const)(ICF_DBM_BLOCK_ALLOCATED),&trace_data)
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)


    return ret_val;
}/* End icf_dbm_alloc_blk*/


/****************************************************************************
 * FUNCTION:           icf_dbm_util_cmp_addr
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
        IN icf_address_st *p_adress2)
{
    icf_boolean_t ret_val = ICF_TRUE;
	/* THese two variables would be contain value 1 if the address is preceeded 
     * by '+' else would contain value 0 */
    icf_uint8_t   addr1_len_diff = 0,
                     addr2_len_diff = 0;
        #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING
	icf_boolean_t	match_found = ICF_FALSE;
#endif /* #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING */

    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }    
    ICF_FUNCTION_ENTER(p_glb_pdb)    

	if('+' == p_adress1->addr_val.str[0])
	{
		/* the address is preceeded by '+'*/
		addr1_len_diff = 1;
	}
	if('+' == p_adress2->addr_val.str[0])
	{
		/* the address is preceeded by '+'*/
		addr2_len_diff = 1;
	}
			
	/* 
	* Check if either the string length of address mismatch or the address 
	* type of two addresses mismatch and either of the address type is 
	* SIP_URI then set user_match to false
	*/ 
	if ((p_adress1->addr_val.str_len - 
		addr1_len_diff) != 
		(p_adress2->addr_val.str_len - addr2_len_diff) || 
		(p_adress1->addr_type != 
		p_adress2->addr_type && 
		((ICF_ADDRESS_PLAN_SIP_URI == 
		p_adress1->addr_type || 
		ICF_ADDRESS_PLAN_SIP_URI == p_adress2->addr_type))
		&& (ICF_ADDRESS_PLAN_SIPS_URI == p_adress1->addr_type ||
 		ICF_ADDRESS_PLAN_SIPS_URI == p_adress2->addr_type))) 
	{
		ret_val = ICF_FALSE;
	}
	else if(0 != icf_port_strcmp(
		&(p_adress1->addr_val.str
		[addr1_len_diff]), 
		&(p_adress2->addr_val.str[addr2_len_diff])))
	{
		ret_val = ICF_FALSE;
	}
    
    #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING
	/* If nothing has matched till now, check if an alternate address can be 
     * be derived from address1 and compared against address2.
	 * If nothing matches still, check if an alternate address can be 
     * be derived from address2 and compared against address1.
     */

	/* Derive alternate address from address2 & compare with address1 */
	if (ICF_FALSE == ret_val)
	{
		if (ICF_FAILURE == icf_dbm_compare_alternate_addr(
								p_glb_pdb,
								p_adress2,
								p_adress1,
								&match_found))
		{
			ret_val = ICF_FALSE;
		}
		else if (ICF_TRUE == match_found)
		{
			ret_val = ICF_TRUE;
		}
	}
	/* Derive alternate address from address1 & compare with address2 */
	if (ICF_FALSE == ret_val)
	{
		if (ICF_FAILURE == icf_dbm_compare_alternate_addr(
								p_glb_pdb,
								p_adress1,
								p_adress2,
								&match_found))
		{
			ret_val = ICF_FALSE;
		}
		else if (ICF_TRUE == match_found)
		{
			ret_val = ICF_TRUE;
		}
	}
#endif /* #ifdef ICF_REQ_URI_ALTERNATE_ADDR_PARSING */

   ICF_FUNCTION_EXIT(p_glb_pdb)  

   return ret_val;
}

/****************************************************************************
 * FUNCTION:           icf_dbm_cmp_addr_in_aor
 *
 * DESCRIPTION:        This function compare two AOR's . 
 *                     is is a utility funicf_dbm_cmp_addr_in_aor
 *                     icf_address_st type addresses and does ignore prefixes 
 *                     in the addresses
 *                     For eg +1234567 would be same as 1234567and vice versa
 *                     Additionally this function also takes care that we dont 
 *                     treat E164 and Tel Uri as different address types
 *
 * RETURNS:            Returns true if address matches else return false
 *
 ***************************************************************************/


icf_boolean_t
icf_dbm_cmp_addr_in_aor(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        IN icf_address_st *p_adress1,
        IN icf_address_st *p_adress2)
{
    icf_boolean_t ret_val = ICF_TRUE;
        /* THese two variables would be contain value 1 if the address is preceeded 
     * by '+' else would contain value 0 */
    icf_uint8_t   addr1_len_diff = 0,
                     addr2_len_diff = 0;

    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_glb_pdb)

        if('+' == p_adress1->addr_val.str[0])
        {
                /* the address is preceeded by '+'*/
                addr1_len_diff = 1;
        }
        if('+' == p_adress2->addr_val.str[0])
        {
                /* the address is preceeded by '+'*/
                addr2_len_diff = 1;
        }

        /* 
        * Check if either the string length of address mismatch or the address 
        * type of two addresses mismatch and either of the address type is 
        * SIP_URI then set user_match to false
        */
        if ((p_adress1->addr_val.str_len -
                addr1_len_diff) !=
                (p_adress2->addr_val.str_len - addr2_len_diff) ||
                (p_adress1->addr_type !=
                p_adress2->addr_type &&
                ((ICF_ADDRESS_PLAN_SIP_URI ==
                p_adress1->addr_type ||
                ICF_ADDRESS_PLAN_SIP_URI == p_adress2->addr_type))
                && (ICF_ADDRESS_PLAN_SIPS_URI == p_adress1->addr_type ||
                ICF_ADDRESS_PLAN_SIPS_URI == p_adress2->addr_type)))
        {
                ret_val = ICF_FALSE;
        }
        /* Match two addresses completely */
        else if(0 != icf_port_strcmp(
                &(p_adress1->addr_val.str
                [addr1_len_diff]),
                &(p_adress2->addr_val.str[addr2_len_diff])))
        {
                ret_val = ICF_FALSE;
        }
   ICF_FUNCTION_EXIT(p_glb_pdb)

   return ret_val;
}


