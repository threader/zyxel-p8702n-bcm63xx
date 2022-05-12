/*------------------------------------------------------------------------------
 *
 * File name        : icf_dbm_dealloc.c
 *
 * Purpose          : This file contains functions that would implement 
 *                    functionality of deallocation of blocks
 * 
 * Revision History :
 *
 *   Date           Name           Ref#        Comments
 * --------      ------------      ----      ---------------
 *  14 DEC 2004   Sumit Gupta                File created and initial version
 *  21 APR 2005   Sumit Gupta     SPR 7416   Replaced call to macro 
 *                                           ICF_MEMFREE with 
 *                                           ICF_STATIC_MEMFREE and changed 
 *                                           pool_id in one of the calls to 
 *                                           ICF_MEMFREE
 *  17 MAY 2005   Sumit Gupta     SPR 7564   Updation in function 
 *                                           icf_dbm_dealloc_rgn_blk
 *
 *	08-Mar-2006     Aman Aggarwal            ICF merging with ICF2.0
 *  02-Apr-2006	Puneet Sharma	ICF5.0 LLD	 changes done in the comments
 *						 Multiple Public Users ICF5.0
 * 8-Apr-2008   Sumant Gupta        ICF 8.0     Rel7.1 Windows porting Merging
 * 17-May-2008     Amit Sharma   SPR:18577    Reinit p_sic_ctx, p_call_ctx and 
 *                                            p_sic_ctx in p_glb_pdb
 * 19-JUn-2008  Anurag Khare     SPR 18684    
 * 25-JUN-2008 Abhishek Dhammawat IPTK Rel8.0 modified
 *                                            icf_dbm_dealloc_line_blk
 * 04-Nov-2008  Tarun Gupta      SPR 19189    Merged Rel 7.1.2 SPR 18880
 * 01-Apr-2009  Kamal Ashraf     Rel 8.2      Klocwork Fix
 * 28-May-2009  Kamal Ashraf     SPR 19672    CSR_1_7129410 Merged
 * 02-Jun-2009  Rajiv Kumar      IPTK Rel 8.2 Fix for SPR 19715
 * 27-Oct-2009  Anuradha Gupta   SPR 20174    Modified icf_dbm_dealloc_blk
 * 28-Apr-2010  Alok Tiwari      SPR 20389    Modified icf_dbm_dealloc_blk
 * 03-May-2010  Alok Tiwari      SPR 20389    Modified icf_dbm_dealloc_blk
 *
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include"icf_dbm_types.h"
#include"icf_dbm_prototypes.h"
#include"icf_dbm_defs.h"
#include"icf_dbm_macro.h"
#include"icf_common_types.h"
#include"icf_macro.h"
#include"icf_dbm_regm_prototypes.h"
#include"icf_dbm_sc_prototypes.h"
#include"icf_port_prototypes.h"

#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_srvc_cntxt_blk
 *
 * DESCRIPTION: Releases the service context block and adds it back to unused
 *              nodes list
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/
icf_return_t 
icf_dbm_dealloc_srvc_cntxt_blk(
        INOUT icf_glb_pdb_st                      *p_glb_pdb,
        INOUT   icf_service_ctx_st                *p_srvc_cntxt_blk,
        OUT   icf_error_t                         *p_ecode)
{
 
    icf_return_t                        ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
    /* CSR_1_7129410 Merged SPR 19672 Start */
    p_srvc_cntxt_blk->expected_events.start_proc_signaling = 
                                        ICF_INT_MODULE_INVALID;
    p_srvc_cntxt_blk->expected_events.start_proc_media = 
                                        ICF_INT_MODULE_INVALID;
   /* CSR_1_7129410 Merged SPR 19672 End */
 
    ret_val = icf_dbm_dealloc_blk(
        p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_SRVC_CTX,
        (icf_void_t *)p_srvc_cntxt_blk,
        p_ecode);


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return (ret_val);

}
#endif /* #ifdef ICF_SERVICE_ENABLE */


#ifdef ICF_SERVICE_ENABLE
/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_srvc_cntxt_lgc_blk
 *
 * DESCRIPTION: Releases the service context logic block and adds it back to 
 *              unused nodes list
 *
 * RETURNS: icf_return_t
 *      
************************************************************************/
icf_return_t 
icf_dbm_dealloc_srvc_cntxt_lgc_blk(                  
      INOUT icf_glb_pdb_st                     *p_glb_pdb,
      INOUT icf_void_t                         *p_srvc_cntxt_lgc_blk,
      OUT   icf_error_t                        *p_ecode)

{
	icf_return_t                       ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)      

    ret_val = icf_dbm_dealloc_blk(
        p_glb_pdb,
        (icf_dbm_block_type_et)ICF_BLK_TYPE_SL_CTX,
        (icf_void_t *)p_srvc_cntxt_lgc_blk,
        p_ecode);  


    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
        
    return (ret_val);
}
#endif /* #ifdef ICF_SERVICE_ENABLE */

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_rgn_blk
 *
 * DESCRIPTION: The function would add the registration block to unused block 
 *              linked array
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  

icf_return_t 
icf_dbm_dealloc_rgn_blk(
        INOUT icf_glb_pdb_st                        *p_glb_pdb,
        INOUT icf_rgm_context_st                    *p_rgn_cntxt_blk,
        OUT   icf_error_t                           *p_ecode)
{    
 
    icf_return_t                        ret_val = ICF_SUCCESS;
   
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
   /* memfree the nonce string */
     ICF_MEMFREE(p_glb_pdb, 
             p_rgn_cntxt_blk->auth_data_recd.p_nonce,
             ICF_MEM_COMMON,
             p_ecode, ret_val)

    ret_val = icf_dbm_dealloc_blk(
            p_glb_pdb,
            (icf_dbm_block_type_et)ICF_BLK_TYPE_RGN_CTX,
            (icf_void_t *)p_rgn_cntxt_blk,
            p_ecode);
 
    p_glb_pdb->p_rgm_ctx = ICF_NULL;
    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return (ret_val);
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_line_blk
 *
 * DESCRIPTION: Releases the memory allocated for line block 
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_dealloc_line_blk(
        INOUT icf_glb_pdb_st                      *p_glb_pdb,
        IN    icf_line_id_t                       line_id,
        OUT   icf_error_t                         *p_ecode)
{
    icf_line_data_st **p_p_line_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)

    p_p_line_data = p_glb_pdb->p_glb_data->p_p_line_data + line_id;    

    icf_dbm_delete_public_uri_list_for_line (p_glb_pdb,
        line_id,p_ecode);
#ifdef ICF_MULTILINE
     /*Free the block pinted by the poiner for line id*/
    ICF_STATIC_MEMFREE( p_glb_pdb,        
            (*p_p_line_data)->p_user_priority_list,
             p_ecode, ret_val)   
#endif
    /*Free the memory for access_network_info header*/    
    if(ICF_NULL != (*p_p_line_data)->p_access_network_info)
     {
         ICF_STATIC_MEMFREE(p_glb_pdb,        
            (*p_p_line_data)->p_access_network_info,
             p_ecode, ret_val)   
     }

    /*Free the block pinted by the poiner for line id*/
    ICF_STATIC_MEMFREE( p_glb_pdb,        
            *p_p_line_data,
             p_ecode, ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)
   
    ICF_RESET_MODULE(p_glb_pdb)        
    
    return (ret_val);
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_call_cntxt_blk
 *
 * DESCRIPTION: The function would free the hash mapping for outgoing call 
 *              and add the block to free blocks list
 *
 * RETURNS: icf_return_t
 *
************************************************************************/  
icf_return_t 
icf_dbm_dealloc_call_cntxt_blk(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_call_id_t                     call_id,
       OUT   icf_error_t                       *p_ecode)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_dbm_blk_id_t               call_cntxt_blk_id=0;
#ifdef ICF_ERROR_ENABLE
    icf_error_data_st              error_data;
#endif
#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st              trace_data;
#endif
	icf_dbm_pool_info_st *p_call_ctx_pool = ICF_NULL;
            
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    p_call_ctx_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            ICF_BLK_TYPE_CALL_CTX);
        
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_ctx_pool))
     
    if(ICF_INVALID_CALL_ID == call_id)
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_DBM_INC_CALL_ID_START
            != (call_id & ICF_DBM_INC_CALL_ID_START) )
    {
        ret_val = icf_dbm_get_idx_for_ogn_ccx(
	                p_glb_pdb,
					call_id,
					&call_cntxt_blk_id,
					p_ecode);

    }/*End if */    
    else 
    {
        call_cntxt_blk_id = (icf_call_id_t)(call_id & ~(ICF_DBM_INC_CALL_ID_START));
    }
    
    if(ICF_SUCCESS == ret_val)
    {
        /*call linked array deallocate function to move the block to 
         * unused blocks list*/
        ret_val = icf_dbm_la_deallocate(
                 p_call_ctx_pool->p_available_blk_lst, 
                &p_call_ctx_pool->next_free_id,
                call_cntxt_blk_id,
                p_ecode); 
    }
    
    if(ICF_FAILURE != ret_val)
    {
#ifdef ICF_TRACE_ENABLE
        trace_data.int_val[0] = ICF_BLK_TYPE_CALL_CTX;
        trace_data.int_val[1] = call_cntxt_blk_id;
        trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
        trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

        ICF_TRACE(p_glb_pdb,
            (icf_trace_id_t const)(ICF_DBM_BLOCK_DEALLOCATED),&trace_data)
#endif

        (((icf_call_ctx_st *)(p_call_ctx_pool->p_blk_array)) + 
               call_cntxt_blk_id)->call_id = ICF_INVALID_CALL_ID;

        ICF_PRINT(((icf_uint8_t*)"\nCall context block deallocated for Call id = %d\n",
            call_id));

#ifdef ICF_LOAD_DEBUG
#ifdef ICF_PORT_LINUX
        struct timeval tv;
        gettimeofday(&tv, ICF_NULL);
        printf("\ncall-ctx FREED		%d	%d\n",call_id,tv.tv_sec);          
#else
        printf("\ncall-ctx FREED		%d\n",call_id); 
#endif
#endif

    }
    else
    {
#ifdef ICF_ERROR_ENABLE
        error_data.int_val[1] = call_id;   
        error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        error_data.number_of_string_values = ICF_NO_STRING_PRESENT;

        ICF_SET_ECODE(p_glb_pdb,&error_data,
                ICF_NO_CALL_CTX_EXIST,p_ecode)
#endif
    }
    /*  Fix done while CSR 1-6017450 Merge */
    /*  SCenario : When for sending new ReInvite 
        the fnction icf_rnw_fill call_ctx is failed then 
        the second call id must get deallocated while 
        p_glb_pdb->p_call_ctx contains the primary callid
        so first check that if callid matches only then 
        make the pointer NULL    */
    if (p_glb_pdb->p_call_ctx->call_id == call_id)
    {
        p_glb_pdb->p_call_ctx = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)        

    return (ret_val);
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_dealloc_sic_cntxt_blk
 *
 * DESCRIPTION: The function would free the hash mapping for outgoing call 
 *              and add the block to free blocks list
 *
 * RETURNS: icf_return_t
 *
************************************************************************/  
icf_return_t 
icf_dbm_dealloc_sic_cntxt_blk(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_call_id_t                     call_id,
       OUT   icf_error_t                       *p_ecode)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_dbm_sic_cntxt_blk_id_t		sic_cntxt_blk_id=0;
    icf_dbm_data_st                *p_dbm_data = ICF_NULL;             
#ifdef ICF_ERROR_ENABLE
    icf_error_data_st              error_data;
#endif
    
        
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    ret_val = icf_dbm_get_module_glb_data(     
        p_glb_pdb,
        (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
        (icf_void_t *)&p_dbm_data,
        p_ecode);
    
    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))
     
    if(ICF_INVALID_CALL_ID == call_id)
    {
        ret_val = ICF_FAILURE;
    }
	else
	{
		ret_val = icf_dbm_get_idx_for_inc_scx(
                p_glb_pdb,
                call_id,
                &sic_cntxt_blk_id,
                p_ecode);
	}
    
    if(ICF_SUCCESS == ret_val)
    {
        /*call linked array deallocate function to move the block to 
         * unused blocks list*/
        ret_val = icf_dbm_la_deallocate(
                p_dbm_data->p_available_sic_cntxt_lst, 
                &p_dbm_data->sic_cntxt_blk_next_free_id,
                sic_cntxt_blk_id,
                p_ecode); 
    }
    
    if(ICF_FAILURE != ret_val)
    {
        ( ( (icf_sic_ctx_st *)(p_dbm_data->p_sic_cntxt_blk)) + 
               sic_cntxt_blk_id)->call_id = ICF_INVALID_CALL_ID;

		icf_dbm_init_sic_ctx( p_glb_pdb,
			( ( (icf_sic_ctx_st *)(p_dbm_data->p_sic_cntxt_blk)) + 
               sic_cntxt_blk_id) );

    }
    else
    {
#ifdef ICF_ERROR_ENABLE        
        error_data.int_val[1] = call_id;   
        error_data.num_of_integer_values = ICF_ONE_INT_PRESENT;
        error_data.number_of_string_values = ICF_NO_STRING_PRESENT;
#endif

        ICF_SET_ECODE(p_glb_pdb,&error_data,
                ICF_NO_CALL_CTX_EXIST,p_ecode)
    }

    p_glb_pdb->p_sic_ctx = ICF_NULL;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)        

    return (ret_val);
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_free_sdp_block
 *
 * DESCRIPTION: This function would free the memory allocated for the SDP block
 *
 * RETURNS: icf_return_t
 *
************************************************************************/  
icf_return_t 
icf_dbm_free_sdp_block(
       INOUT icf_glb_pdb_st          *p_glb_pdb,
       INOUT icf_void_t              *p_sdp_data,
       OUT   icf_error_t             *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    ret_val = icf_dbm_ret_mem_to_pool(
            p_glb_pdb,
            (icf_pool_id_t const)ICF_MEM_SDP,
            p_sdp_data,
            p_ecode);

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_free_addr_blk
 *
 * DESCRIPTION: It will free the memory allocated for the Address block
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_free_addr_blk(
      INOUT  icf_glb_pdb_st       *p_glb_pdb,
      INOUT  icf_void_t           *p_addr_data,
      OUT    icf_error_t          *p_ecode)
{
    
    /* Deallocate memory for each member of it */
    icf_return_t ret_val = ICF_SUCCESS;

        
    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Free the block pinted by the address block pointer*/
    ICF_MEMFREE( p_glb_pdb,        
             p_addr_data,
            ICF_MEM_COMMON,
             p_ecode,ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_free_timer_block
 *
 * DESCRIPTION: This function would free the pool allocated for the timer
 *              block
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_free_timer_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       INOUT icf_void_t                        *p_timer_data,
       OUT   icf_error_t                       *p_ecode)
{
    
    /* Deallocate memory for each member of it */
    icf_return_t ret_val = ICF_SUCCESS;
    icf_return_t return_val = ICF_SUCCESS;

    /* SPR 18880 : Added NULL checks */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)"[DBM] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_timer_data,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        /* Return failure. Not enough data to proceed */
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /* SPR 18880 : Call the following function to do memory
       deallocations back to the pool. This is a utility function
       which takes care of deallocations of blocks back to the pool
       created at init time for the block type passed to it.
    */

    /* initialize members of the structure */
    icf_dbm_init_timer_data_blk(p_timer_data);

    if(ICF_FAILURE == icf_dbm_dealloc_blk(p_glb_pdb,
          (icf_dbm_block_type_et)ICF_BLK_TYPE_TIMER_DATA,
          p_timer_data,
          p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
       
    /* SPR 18880 */
    if(ICF_SUCCESS == ret_val)
    {
        p_timer_data = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}

/****************************************************************************
 * FUNCTION:           icf_dbm_regm_delete_user
 *
 * DESCRIPTION:        The function would be called to delete registrtion for
 *                     the given user on the specified line
 *
 * RETURNS:            Returns success if user registration was successfull else
 *                     return failure
 *  
 *
 ***************************************************************************/
icf_return_t
icf_dbm_regm_delete_user(
      INOUT icf_glb_pdb_st       *p_glb_pdb,
      IN    icf_line_id_t        line_id,
      IN    icf_address_st       *p_user_addr,
      OUT   icf_uint8_t          *p_status,
      OUT   icf_rgm_context_st  **p_p_rgn_data,
      OUT   icf_error_t          *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;

#ifndef IMS_CLIENT
/*Just to remove warnings */
if(line_id==0)
{
}
if(p_user_addr==ICF_NULL)
{
}
p_status=p_status;
p_p_rgn_data=p_p_rgn_data;
p_ecode=p_ecode;
#endif


#ifdef IMS_CLIENT
    icf_rgm_context_st *p_rgn_blk_addr = ICF_NULL;
    icf_line_id_t line_count = 0;
#endif

    if(ICF_NULL == p_glb_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)    

#ifdef IMS_CLIENT

    if(ICF_FAILURE == icf_dbm_fetch_rgn_blk(
        p_glb_pdb,
        p_user_addr,
        line_id,
        &p_rgn_blk_addr,
        p_ecode))
    {
        *p_status = ICF_RGM_DBM_USER_NOT_FOUND;
    }
    else
    {
        if(1 == p_rgn_blk_addr->num_of_lines)
        {
            /*Dont decrease the number of lines. Just send status as 
             * ICF_RGM_DBM_DELETE_BLOCK*/
            *p_p_rgn_data = p_rgn_blk_addr;
            *p_status = ICF_RGM_DBM_DELETE_BLOCK;
        }
        else
        {
            if(ICF_FAILURE == icf_dbm_del_line_in_rgn(
                        p_glb_pdb,
                        line_id,
                        p_rgn_blk_addr))
            {
                ret_val = ICF_FAILURE;
            }               
            else 
            {
                /*Set the duration to that of fir line in the line list*/
                p_rgn_blk_addr->rgm_duration =
                    (p_glb_pdb->p_glb_data->p_p_line_data
                    [p_rgn_blk_addr->rgm_list_of_lines[line_count]])->
                    registration_duration;

                line_count += 1;

                /*Check for other lines in line list to check other line to find
                 *the line with least duration. This would be the new 
                 registration duration for the block*/
                while(line_count< p_rgn_blk_addr->num_of_lines)
                {
                    if(p_glb_pdb->p_glb_data->p_p_line_data
                            [p_rgn_blk_addr->rgm_list_of_lines[line_count]]->
                           registration_duration < p_rgn_blk_addr->rgm_duration)
                    {
                        p_rgn_blk_addr->rgm_duration = 
                            p_glb_pdb->p_glb_data->p_p_line_data
                            [p_rgn_blk_addr->rgm_list_of_lines[line_count]]->
                            registration_duration;
                    }
                    line_count += 1;
                }
                        
                *p_status = ICF_RGM_DBM_NO_ACTION; 
            }
        }
    }
#endif

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return ret_val;
}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_detach_cc_from_line
 * 
 * DESCRIPTION:   Removes an existing Call Context block from an existing line 
 *
 * RETURNS:       icf_return_t 
 * 
 ************************************************************************/
icf_return_t 
icf_dbm_detach_cc_from_line(            
      INOUT  icf_glb_pdb_st                         *p_glb_pdb, 
      INOUT  icf_line_id_t                          line_id,
      INOUT  icf_call_ctx_st                        *p_call_cntxt,
      OUT    icf_error_t                            *p_ecode)
{
    
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_call_ctx_list_st     *p_temp_call_ctx_lst,
                                *p_node_to_free = ICF_NULL;

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
         
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_call_cntxt))

    /*
     * this is an additional check to ensure in INVALID line ID is passed it 
     * should be taken care of 
     */
    if (ICF_INVALID_LINE_ID == line_id)
    {
		return ICF_FAILURE;
    } /* if (ICF_LINE_ID_INVALID == ... */
    
    /*Fix For SPR:19715*/
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                p_glb_pdb->p_glb_data,
                p_glb_pdb->p_glb_data->p_p_line_data[line_id],
                ret_val)     
    
    if (ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_glb_pdb)
        ICF_RESET_MODULE(p_glb_pdb)    
        return ret_val;
    }
    
    p_temp_call_ctx_lst = 
        p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list;
    
    if(ICF_NULL != p_temp_call_ctx_lst)
    {
		/*
         * Check if very first node contains the call cotext and there are no 
         * other nodes in list
         */
        if(p_call_cntxt == p_temp_call_ctx_lst->p_call_ctx) 
        {
           if(ICF_NULL == p_temp_call_ctx_lst->p_call_ctx_next)
           {
               p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list = 
                   ICF_NULL;
           }
           else
           {
               p_glb_pdb->p_glb_data->p_p_line_data[line_id]->p_call_ctx_list = 
                   p_temp_call_ctx_lst->p_call_ctx_next;
           }
           p_node_to_free = p_temp_call_ctx_lst;
        }
        else
        {
            /*
             * Traverse the list to find out the matching call context
             */
            while (ICF_NULL != p_temp_call_ctx_lst->p_call_ctx_next)
            {       
                if(p_call_cntxt == 
                        (p_temp_call_ctx_lst->p_call_ctx_next)->p_call_ctx) 
                {
                    p_node_to_free = p_temp_call_ctx_lst->p_call_ctx_next;
                    p_temp_call_ctx_lst->p_call_ctx_next = 
                       (p_temp_call_ctx_lst->p_call_ctx_next)->p_call_ctx_next;
                    break;
                }
                p_temp_call_ctx_lst = p_temp_call_ctx_lst->p_call_ctx_next;
            }
        }
    }
   
    if(ICF_NULL != p_node_to_free) 
    {
        /*
         * Here we wouldn't bother bout failure in releasing memory
         */ 
        icf_dbm_ret_mem_to_pool(
                p_glb_pdb,
                (icf_pool_id_t const)ICF_MEM_CALL_CTX_LIST,
                (icf_void_t *)p_node_to_free,
                p_ecode);
      
        (p_glb_pdb->p_glb_data->p_p_line_data[line_id])->num_of_calls -= 1;
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
 * FUNCTION NAME: icf_dbm_free_ssa_timer_block
 *
 * DESCRIPTION: This function would free the memory allocated for the ssa timer
 *              block
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_free_ssa_timer_block(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       INOUT icf_void_t                        *p_timer_data,
       OUT   icf_error_t                       *p_ecode)
{
    
    /* Deallocate memory for each member of it */
    icf_return_t ret_val = ICF_SUCCESS;

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /*Free the block pinted by the address block pointer*/
    ICF_MEMFREE( p_glb_pdb,        
             p_timer_data,
            ICF_MEM_SSA_TIMER,
             p_ecode,ret_val)

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);

}

/************************************************************************
 *
 * FUNCTION NAME: icf_dbm_ret_mem_to_pool
 *
 * DESCRIPTION: This function would return the memory pointed by the 
 *              pointer specified as argument back to the pool.
 *              If the pool is of type ICF_MEM_TIMER, this function
 *              calls icf_dbm_free_timer_block to return the memory to 
 *              the pool.
 *
 * RETURNS: icf_return_t
 *
 ************************************************************************/  
icf_return_t 
icf_dbm_ret_mem_to_pool(
       INOUT icf_glb_pdb_st                    *p_glb_pdb,
       IN    icf_pool_id_t                     pool_id,
       INOUT icf_void_t                        *p_mem_blk,
       OUT   icf_error_t                       *p_ecode)
{
    
    icf_return_t ret_val = ICF_SUCCESS;
    
    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_glb_pdb)
    {
        /* Return failure. Not enough data to proceed */
        ICF_PRINT(((icf_uint8_t *)"[DBM] : p_glb_pdb is NULL.\n"))
        return ICF_FAILURE;
    }

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)
    
    ICF_FUNCTION_ENTER(p_glb_pdb)
        
    /* SPR 18880: Call icf_dbm_free_timer_block to return the 
       memory to the pool
    */
    if(ICF_MEM_TIMER == pool_id)
    {
        ret_val = icf_dbm_free_timer_block(p_glb_pdb,
                    p_mem_blk,p_ecode);
    }
    /* For any other pool type, simply free the system memory. 
       Free the block pointed by the address block pointer
    */
    else
    {
        ICF_MEMFREE(p_glb_pdb,        
                    p_mem_blk,
                    pool_id,
                    p_ecode,
                    ret_val)
    }
    
    /* SPR 18880 */
    if(ICF_SUCCESS == ret_val)
    {
        p_mem_blk = ICF_NULL;
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)    
    
    return (ret_val);
}


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
        OUT   icf_void_t                         *p_block_addr,
        OUT   icf_error_t                        *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_dbm_data_st            *p_dbm_data = ICF_NULL;
    icf_dbm_blk_id_t           rgn_cntxt_blk_prev_index = 0,
                               next_to_next_used_index = 0,
                               rgn_cntxt_blk_next_index= 0,
                               blk_id_to_free = 0;
    icf_dbm_pool_info_st       *p_pool = ICF_NULL;  
    icf_return_t               return_val = ICF_SUCCESS;                  
#ifdef ICF_TRACE_ENABLE
    icf_trace_data_st          trace_data;
    /* Klocwork Fix Rel 8.2 */ 
    icf_cmn_init_trace_data_st(&trace_data);
    /* Klocwork Fix Rel 8.2 end */
#endif 

    ICF_SET_MODULE(p_glb_pdb,ICF_INT_MODULE_DBM)

    ICF_FUNCTION_ENTER(p_glb_pdb)

    ret_val = icf_dbm_get_module_glb_data(     
            p_glb_pdb,
            (icf_int_module_id_t const)ICF_INT_MODULE_DBM,
            (icf_void_t *)&p_dbm_data,
            p_ecode);
    
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_dbm_data,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    ICF_ASSERT(p_glb_pdb, (ICF_FAILURE != ret_val))

    p_pool =  icf_dbm_get_pool_addr(
            p_glb_pdb,
            block_type);

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_pool,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_pool))

    if(ICF_FAILURE == icf_dbm_ret_index_for_addr(
                p_glb_pdb,
                p_pool->p_blk_array,
                p_block_addr,
                p_pool->block_size,
                &blk_id_to_free,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_BLK_TYPE_RGN_CTX == block_type)
    {
        /* Do the processing for updating the used nodes list*/
        /* If the node to be deleted is the very first node in the list 
         * then store the address of node next to this node. This address 
         * would become the head node when the pointer is deleted*/
        if(blk_id_to_free == 
                p_dbm_data->rgn_cntxt_blk_next_used_id)
        {
            next_to_next_used_index = 
                p_pool->p_available_blk_lst
                [p_dbm_data->rgn_cntxt_blk_next_used_id].next_index ;
        }
        else
        {
			/*If the node to deleted was not the first node in the list 
			then traverse the list to find out the node to be deleted*/
			rgn_cntxt_blk_prev_index = 
				p_dbm_data->rgn_cntxt_blk_next_used_id;
			/*[BugFix]If the rgn_cntxt_blk_next_used_id is queal to pool_size, 
			it means that all rgn blocks were already freed.So return failure.,
			__Mitrastar__,Jason,20121015*/
			if(rgn_cntxt_blk_prev_index < p_pool->pool_size){
				while(p_dbm_data->pool[block_type].pool_size != 
				        p_pool->p_available_blk_lst
				        [rgn_cntxt_blk_prev_index].next_index
				        && (blk_id_to_free != p_pool->p_available_blk_lst
				        [rgn_cntxt_blk_prev_index].next_index))
				{
				    rgn_cntxt_blk_prev_index = 
				        p_pool->p_available_blk_lst
				        [rgn_cntxt_blk_prev_index].next_index;
				}
				rgn_cntxt_blk_next_index = p_pool->p_available_blk_lst
				    [blk_id_to_free].next_index;
				

				if(p_pool->pool_size == p_pool->p_available_blk_lst
				          [rgn_cntxt_blk_prev_index].next_index)
				{
				    ret_val = ICF_FAILURE;
				}   
			}else{
			   printf("\n[DBM]: No allocated rgn block! return failure!\n");
			   ret_val = ICF_FAILURE;
			}
        }
    }
   

    if(ICF_SUCCESS == ret_val)
    {
        /* Fix for CSR 1-8534644 : SPR-20389*/
        /* SPR 20174: Protective check
         * In case the Reuqest to free DBM block is issued for any already 
         * The below check is used to prevent from freeing a DBM block which
         * is already freed.This is applicable for all block types other than
         * registration blocks.
         * In case of Registration blocks,next_index contains the
         * rgn_cntxt_blk_next_used_id (i.e. the id of last used registration
         * context) but in case of other blocks, each allocated block contains
         * the next_index as pool_size.
         */

        if((ICF_BLK_TYPE_RGN_CTX != block_type) &&
           (p_pool->pool_size != p_pool->p_available_blk_lst[blk_id_to_free].next_index))
        {
            ICF_PRINT(((icf_uint8_t*)"\n[DBM]: The Block is already freed "))
            ret_val = ICF_FAILURE;
        }
        /*call linked array deallocate function to mark the 
         * service_cntxt_blk_id_to_free to free node list*/
        else if(ICF_FAILURE == icf_dbm_la_deallocate(
                    p_pool->p_available_blk_lst,
                    &p_pool->next_free_id,
                    blk_id_to_free,
                    p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
    }  

    if (ICF_SUCCESS == ret_val)
    {
        if(ICF_BLK_TYPE_RGN_CTX == block_type)
        {

            if(blk_id_to_free == 
                    p_dbm_data->rgn_cntxt_blk_next_used_id)
            {
                p_dbm_data->rgn_cntxt_blk_next_used_id = 
                    next_to_next_used_index;
            }
            else
            {
                p_pool->p_available_blk_lst
                    [rgn_cntxt_blk_prev_index].next_index = 
                    rgn_cntxt_blk_next_index;
            }
        }

#ifdef ICF_TRACE_ENABLE
        trace_data.int_val[0] = block_type;
        trace_data.int_val[1] = blk_id_to_free;
        trace_data.num_of_integer_values = ICF_TWO_INT_PRESENT;
        trace_data.number_of_string_values = ICF_NO_STRING_PRESENT;

        ICF_TRACE(p_glb_pdb,
            (icf_trace_id_t const)(ICF_DBM_BLOCK_DEALLOCATED),&trace_data)
#endif
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)

    return ret_val;
}/* End icf_dbm_dealloc_blk*/

