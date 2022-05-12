/*****************************************************************************  
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_glb_defs_n_intrnl_funcs.c
*
*    DESCRIPTION : This file contains defines and function 
*                  declarations of memory manager sub-module of generic 
*                  library implementation. functions include those for 
*                  creating memory pools and those for allocation and 
*                  deallocation of memory buffers from created pools 
*                  dynamically. (source file)
*
* DATE               NAME        REFERENCE          REASON
*-------------  --------------- -------------   ---------------------
*
* Oct'99        Kapil Nayar     HSS:40600026 Generic Libraries
*               HSS:40600036    Gateway
*
* 04/09/2001    Prashant Jain   SPR-2560     Adding OSE operating system
*                                            support. Removed Compile-
*                                            time errors.
* 04/10/2001    Ravi Jindal     SPR-2751     Remove warnings with gcc,
*                                            cc,CC on Solaris.
* 16/10/2001    Ravi Jindal     SPR-2758     Add flag GL_DEBUG in gl_mm_   
*                                            pool_delete function.
* 18/10/2001    Ravi Jindal     SPR-2788     Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal     SPR-2802     Adding Pre-ANSI-C features.
* 
* 27/09/2001    Prashant Jain   SPR-2709     Made gl_mm_common_pool_create
*                                            MT-safe, for MT-environment.
* 24/10/2001    Ravi Jindal     SPR-2789     Change pool-id in memory manager
*                                            from U8bit to U16bit.
*
* 24/10/2001    Ravi Jindal     SPR-2758     Encapsulation of free system call
*                                            with flag GL_DEBUG in 
*                                            pool_deletion. 
* 30/10/2001    Ravi Jindal     SPR-2744     Adding de-init functions.
*
* 17/11/2001    Ravi Jindal     SPR-3107     Incoprate defects and change 
*                                            cosmmetics of file.
* 22/11/2001    Ravi Jindal     SPR-3225     Add Congestion Management 
*                                            modules related structures.
*
* 27/11/2001    Prashant Jain   SPR-3268     GL enters in a Infinite Loop
*                                            if "gl_mm_init" is called more
*                                            than once. This is because 
*                                            variable "init_flag" was
*                                            not getting reset when all pools
*                                            are destroyed through exported 
*                                            API interface.
*
* 27/12/2001    Ravi Jindal     SPR-3412     To add task specific common 
*                                            Pool APIs. buffer from task
*                                            specific pool are taken on BEST
*                                            FIT mechanism among all task
*                                            specific common pools created with
*                                            same task-id.
*
* 31/12/2001    Prashant Jain   SPR-2744     When de-initialization is invoked,
*                                            INIT flag should be checked to 
*                                            take care of out-of-sequence
*                                            calling of DE-INIT APIs or for 
*                                            that matter, any other API.
*
* 02/01/2002    Ravi Jindal     SPR-3427     To set default value of 
*                                            "a_pool_info].task_id" to 
*                                            GL_INVALID_TASK_ID in gl_mm_init.
*
* 02/01/2002    Ravi Jindal     SPR-3424     Set "p_buf_region" to GL_NULL
*                                            in gl_mm_pool_delete().
*
* 02/01/2002    Ravi Jindal     SPR-3427     To set task-id in "gl_mm
*                                            _task_specific_cmn_pool_create".
*
* 04/01/2002    Ravi Jindal     SPR-2744     Return GL_NULL, if return type is 
*                                            gl_pvoid_t.
*
* 04/01/2002    Prashant        SPR-2751     Remove warnings with gcc,
*               Jain                         cc,CC on Solaris.
*
* 23/01/2002    Ravi Jindal     SPR-3393     To add a check a check in
*                                            release buffer calls, that
*                                            buffer is allocated or not
*                                            before de-allocating a buffer 
*                                            pointer.
*                                            
* 10/03/2002    Ravi Jindal     SPR-3952     Enhancements for Rel-1.0. 
*
* 06/12/2002    Ravi Jindal     SPR-5117     Compilation on 64bit machine.
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/
  
#include "gl_mm.h"

/*
 * Array of common memory pool structures to store 
 * common memory pools at runtime.
 */
gl_mm_pool_t *g_a_p_glb_pool_table[GL_MM_MAX_COMMON_POOL_QTY]; 

/*
 * Pointer to common pool structure with 
 * smallest memory buffer amongst common pools.
 */
gl_mm_pool_t* p_gl_smallest_mem_buf_pool; 

/*
 * Array of task specific memory pool structures to store 
 * task specific memory pools at runtime.
 */
gl_mm_pool_t *g_a_p_app_sp_pool_table[GL_MM_MAX_TASK_POOL_QTY]; 

/*
 * Semaphores required for MT safe option.
 */
#ifdef GL_MT_ENABLED
GL_MUTEX_ID mm_mgr_sem;
#endif

/*
 * Array to store task-id related smallest pool-id.
 * It is required for task specific common pools.
 */
gl_task_specific_pool_info_t a_pool_info[GL_MM_MAX_TASK_POOL_QTY];
gl_task_specific_pool_info_t *g_a_p_pool_info[GL_MM_MAX_TASK_POOL_QTY];

/*
 * Index to pool info array, at which new entry is stored.
 */ 
gw_U16bit free_entry_index = 0;                 


#ifdef GL_MM_ADD_DEBUGGER
    gw_U8bit               buffer_alloc_signature=0;

    #ifdef GL_SOLARIS_6
    gl_pvoid_t             esp;
    #endif

    #ifdef GL_VXWORKS
    gl_pvoid_t             e_bp;
    gl_pvoid_t             e_sp;
    #endif
#endif

gw_S8bit               init_flag_cmn_pools = GL_FALSE;
gw_S8bit               init_flag_task_pools = GL_FALSE;


#ifdef GL_MM_ADD_AUDIT_MGT
    gw_boolean           audit_act_set_flag = GL_FALSE;
    gl_hash_table_t      g_mm_audit_hash_table_id;
    gw_U32bit            g_mm_audit_special_user_id;
    gl_pvoid_t           g_p_audit_hash_walk_handle;

#ifdef GL_MT_ENABLED
GL_MUTEX_ID mm_audit_sem;
#endif

#endif /* Endif for GL_MM_ADD_AUDIT_MGT */



/*****************************************************************************
* Function name    : mm_validate_common_pool_buffer_pointer 
* Arguments        : gw_U16bit          pool_id,
*                    gl_pvoid_t         buf_ptr,
*                    gw_U32bit          *p_buf_index
* Return Value     : gl_return_t
* Functionality    : Validates buffer pointer passed as a parameter.
* Functions called :
*****************************************************************************/
gl_return_t
mm_validate_common_pool_buffer_pointer 

#ifdef _ANSI_C_ 
    ( gw_U16bit                 pool_id,
      gl_pvoid_t                buf_ptr,
      gw_U32bit                 *p_buf_index
    )
#else
    ( pool_id, buf_ptr, p_buf_index )
      gw_U16bit                 pool_id;
      gl_pvoid_t                buf_ptr;
      gw_U32bit                 *p_buf_index;
#endif
    
/****************************************************************************/
{

    gw_S32bit                   address_diff = 0;

    /*
     * Validate pool id for common memory pools.
     */
    if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY)
    {
        return GL_FAILURE;
    }

    /*
     * If start address of pool is null pool doesnot exist.
     */
    if(GL_NULL == g_a_p_glb_pool_table[pool_id])
    {
        return GL_FAILURE;
    }

    /*
     * Get offset of address of buffer header and start
     * address of pool.
     */
    address_diff = ((gw_U8bit *)buf_ptr -  
        (gw_U8bit *)(g_a_p_glb_pool_table[pool_id]->p_buf_region)) -
        (gw_U32bit)sizeof(gl_mm_check_mem_overwrite_t);

    /*
     * offset should be a direct multiple of 
     * buffer header size + buffer size or else pointer is not valid.
     */
    /*
     * Also check that buffer pointer is in referred pool and 
     * not beyond pool memory.
     */
    if(0 > address_diff)
    {
        return GL_FAILURE;
    }
    
    *p_buf_index = address_diff / 
        (sizeof(gl_mm_check_mem_overwrite_t)+g_a_p_glb_pool_table[pool_id]->
         buf_size);
    
    if(((address_diff % (sizeof(gl_mm_check_mem_overwrite_t) + 
         g_a_p_glb_pool_table[pool_id]->buf_size)) != 0)
        || ( *p_buf_index > g_a_p_glb_pool_table[pool_id]->buf_qty))
    {
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}


/*****************************************************************************
* Function name    : mm_validate_task_pool_buffer_pointer 
* Arguments        : gw_U16bit          pool_id,
*                    gl_pvoid_t         buf_ptr,
*                    gw_U32bit          *p_buf_index
* Return Value     : gl_return_t
* Functionality    : Validates buffer pointer passed as a parameter.
* Functions called :
*****************************************************************************/
gl_return_t
mm_validate_task_pool_buffer_pointer 

#ifdef _ANSI_C_
    ( gw_U16bit                 pool_id, 
      gl_pvoid_t                buf_ptr,
      gw_U32bit                 *p_buf_index
    )
#else
    ( pool_id, buf_ptr, p_buf_index )
      gw_U16bit                 pool_id; 
      gl_pvoid_t                buf_ptr;
      gw_U32bit                 *p_buf_index;
#endif
    
/****************************************************************************/
{

    gw_S32bit                   address_diff = 0;
    

    /*
     * Validate pool id for task specific memory pools.
     */
    if(pool_id >= GL_MM_MAX_TASK_POOL_QTY)
    {
        return GL_FAILURE;
    }

    /*
     * If start address of pool is null pool doesnot exist.
     */
    if(GL_NULL == g_a_p_app_sp_pool_table[pool_id])
    {
        return GL_FAILURE;
    }
    /*
     * Get offset of address of buffer header and start
     * address of pool.
     */
    address_diff = ((gw_U8bit *)buf_ptr -  
        (gw_U8bit *)(g_a_p_app_sp_pool_table[pool_id]->p_buf_region)) -
        (gw_U32bit)sizeof(gl_mm_check_mem_overwrite_t);

    /*
     * offset should be a direct multiple of 
     * buffer header size + buffer size or else pointer is not valid.
     */
    /*
     * Also check that buffer pointer is in referred pool and 
     * not beyond pool memory.
     */
    if(0 > address_diff)
    {
        return GL_FAILURE;
    }
    
    *p_buf_index = address_diff /
        (sizeof(gl_mm_check_mem_overwrite_t) + g_a_p_app_sp_pool_table[pool_id]
         ->buf_size);

    if(((address_diff % (sizeof(gl_mm_check_mem_overwrite_t) +
         g_a_p_app_sp_pool_table[pool_id]->buf_size)) != 0)
        || ( *p_buf_index > g_a_p_app_sp_pool_table[pool_id]->buf_qty))
    {
        return GL_FAILURE;
    }

    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : mm_check_current_cngst_mgmt_region
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_mm_pool_t       *p_pool,
*                    gl_error_t         *p_ecode
* Return Value     : void
* Functionality    : Set Congestion Management Region's
*                    Specification parameters.
* Functions called :
*****************************************************************************/
gl_return_t  
mm_check_current_cngst_mgmt_region

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_mm_pool_t              *p_pool,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_pool, p_ecode )
      gl_pool_id_t              pool_id;
      gl_mm_pool_t              *p_pool;
      gl_error_t                *p_ecode;

#endif
    
/****************************************************************************/
{
    gw_U32bit                   discard_value;
    gw_U32bit                   abate_value;
    /*
     * To remove warning on cc compiler.
     */ 
    *p_ecode = *p_ecode;
    
    discard_value = (p_pool->buf_qty * p_pool->specs.
    region_def[p_pool->current_region_index].discard)/100;
     
    abate_value = (p_pool->buf_qty * p_pool->specs.
    region_def[p_pool->current_region_index].abate)/100;
        
    if(p_pool->busy_buf_qty == discard_value)
    {
        /*
         * Pool Status is changing from less congestion region to 
         * more congestion region.
         */
        p_pool->current_region_index += 1;
        if(GL_FAILURE == p_pool->cngst_mgmt_call_back(
                         (gl_pool_id_t)(pool_id+1),
                         (gw_U8bit)(p_pool->current_region_index-1),
                         (p_pool->current_region_index),
                         (p_pool->specs.p_user_arg),
                         (p_ecode)))
        {
            return GL_FAILURE;
        }
    }
    else if(p_pool->busy_buf_qty == abate_value)
    {
        /*
         * Pool Status is changing from more congestion region to 
         * less congestion region.
         */
        p_pool->current_region_index -= 1;
        if(GL_FAILURE == p_pool->cngst_mgmt_call_back(
                         (gl_pool_id_t)(pool_id+1),
                         (gw_U8bit)(p_pool->current_region_index+1),
                         (p_pool->current_region_index),
                         (p_pool->specs.p_user_arg),
                         (p_ecode)))
        {
            return GL_FAILURE;
        }
    }
    return GL_SUCCESS;
}




#ifdef GL_MM_ADD_DEBUGGER


/*****************************************************************************
* Function name    : mm_stack_trace_update
* Arguments        : gl_pool_t          *p_pool_ptr,
*                    gw_U32bit          buf_index
* Return Value     : gl_return_t
* Functionality    : Update Stack trace.  
* Functions called :
*****************************************************************************/
gl_return_t  
mm_stack_trace_update

#ifdef _ANSI_C_
    ( gl_mm_pool_t              *p_pool_ptr,
      gw_U32bit                 buf_index
    )
#else
    ( p_pool_ptr, buf_index )
      gl_mm_pool_t              *p_pool_ptr;
      gw_U32bit                 buf_index;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    
#if (defined GL_SOLARIS_6) || (defined GL_VXWORKS)
    gw_U16bit                   i=0;
    gl_mm_layout_t              *p_curr_func_add = GL_NULL;
    gw_U8bit                    is_first_local_func_add = GL_TRUE;
#endif
    gl_mm_stk_inf_t             *p_stk_inf = GL_NULL; 

    GL_MM_DEBUG_INFO_TRACE(("\n In Get Buffer Function\n"));         
    /*
     * Update Stack Trace inforamtion.
     */ 
    p_stk_inf = p_pool_ptr->p_start_stk_inf_reg + buf_index;
    /*
     * Get current sp stored in %o6.
     */
    p_stk_inf->num_of_entries = GL_NULL;
        
#ifdef GL_SOLARIS_6
    __asm__("sethi %hi(esp),%o0");
    __asm__("mov %o6,%o1");
    __asm__("st %o1,[%o0+%lo(esp)]");
    p_curr_func_add = (gl_mm_layout_t *)esp;
    while(i <  GL_MM_STK_TR_DEPTH )
    {
        if((GL_NULL == p_curr_func_add) || 
           (GL_NULL == p_curr_func_add->return_address))
        {
            break;
        }
#endif

#ifdef GL_VXWORKS
    __asm__ ("movl %ebp, _e_bp");
    __asm__ ("movl %esp, _e_sp");
    p_curr_func_add = (gl_mm_layout_t *)e_bp;
    while(i <  GL_MM_STK_TR_DEPTH )
    {
        if(((gl_pvoid_t)p_curr_func_add < e_sp) || 
           (GL_NULL == p_curr_func_add->return_address))
        {
            break;
        }
#endif
    
#if (defined GL_SOLARIS_6) || (defined GL_VXWORKS)
            
        if(GL_TRUE == is_first_local_func_add)
        {
            is_first_local_func_add = GL_FALSE;
        }
        else
        {
            p_stk_inf->stk_func_add[i] = (gl_pvoid_t) p_curr_func_add->
                return_address;
            p_stk_inf->num_of_entries++;
            i++;
        }
        p_curr_func_add = p_curr_func_add->p_next;
    }
    return GL_SUCCESS;
#else
    return GL_FAILURE;
#endif
            
#endif
}



/*****************************************************************************
* Function name    : mm_buffer_trace_update
* Arguments        : gl_pool_t          *p_pool_ptr,
*                    gw_U32bit          buf_index,
*                    gw_U8bit           buf_status,
*                    gw_U32bit          task_id,
*                    gl_mm_buffer_hdr_t *p_buf_hdr
* Return Value     : gl_return_t
* Functionality    : Update Buffer trace.  
* Functions called :
*****************************************************************************/
gl_return_t  
mm_buffer_trace_update

#ifdef _ANSI_C_
    ( gl_mm_pool_t              *p_pool_ptr,
      gw_U32bit                 buf_index,
      gw_U8bit                  buf_status,
      gw_U32bit                 task_id,
      gl_mm_buffer_hdr_t        *p_buf_hdr
    )
#else
    ( p_pool_ptr, buf_index, buf_status, task_id, p_buf_hdr )
      gl_mm_pool_t                 *p_pool_ptr;
      gw_U32bit                 buf_index;
      gw_U8bit                  buf_status;
      gw_U32bit                 task_id;
      gl_mm_buffer_hdr_t        *p_buf_hdr;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gl_mm_buf_trace_t           *p_buf_trace = GL_NULL; 
    /*
     * Update buffer trace inforamtion.
     */ 
    p_buf_trace = p_pool_ptr->p_start_buf_trace_reg + 
        ( buf_index * p_pool_ptr->num_of_buf_trc_reqs);
    
    p_buf_trace = p_buf_trace + p_buf_hdr->curr_buf_trace_index;

    p_buf_trace ->task_id = task_id;
    p_buf_trace ->buf_status = buf_status;
    p_buf_trace ->buffer_alloc_signature = buffer_alloc_signature;
    
    p_buf_hdr->curr_buf_trace_index = p_buf_hdr->curr_buf_trace_index + 1;
    p_buf_hdr->curr_buf_trace_index = (p_buf_hdr->curr_buf_trace_index)
        % (p_pool_ptr->num_of_buf_trc_reqs);
    
    return GL_SUCCESS;
#endif
}


/*****************************************************************************
* Function name    : mm_pool_usage_update
* Arguments        : gl_pool_t          *p_pool_ptr,
*                    gw_U32bit          buf_size
* Return Value     : gl_return_t
* Functionality    : Update Pool Usage Profile.  
* Functions called :
*****************************************************************************/
gl_return_t  
mm_pool_usage_update

#ifdef _ANSI_C_
    ( gl_mm_pool_t              *p_pool_ptr,
      gw_U32bit                 buf_size
    )
#else
    ( p_pool_ptr, buf_size )
      gl_mm_pool_t              *p_pool_ptr;
      gw_U32bit                 buf_size;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U8bit                    i;
    gw_U8bit                    buf_usage=0;
    gl_mm_pool_usage_profile_t  *p_pool_usage = GL_NULL; 
    /*
     * Update buffer trace inforamtion.
     */ 
    p_pool_usage = p_pool_ptr->p_pool_usage_profile; 
        
    /*
     * Find out usage of buffer region in percentage.
     */ 
    buf_usage = (gw_U8bit)((buf_size * 100)/p_pool_ptr->buf_size);

    for(i=0; i<p_pool_usage->num_regions; i++)
    {
        if(buf_usage <= p_pool_usage->usage_region_def[i])
        {
            p_pool_usage->usage_region_count[i]++;
            break;
        }
    }
    return GL_SUCCESS;
#endif
}


#endif /* Endif for GL_MM_ADD_DEBUGGER */


#ifdef GL_MM_ADD_AUDIT_MGT


/*****************************************************************************
* Function name    : mm_get_user_id_info_node
* Arguments        : gw_U32bit          user_id,
*                    gl_mm_user_id_info_node_t **p_p_user_id_infor_nodfe, 
*                    gw_boolean         *p_is_id_found,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To return the user-id info node corresponding to given 
*                    user-id. 
* Functions called :
*****************************************************************************/
gl_return_t    
mm_get_user_id_info_node

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_mm_user_id_info_node_t **p_p_user_id_info_node,
      gw_boolean                *p_is_id_found,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_p_user_id_info_node, p_is_id_found, p_ecode )
      gw_U32bit                 user_id;
      gl_mm_user_id_info_node_t **p_p_user_id_info_node;
      gw_boolean                *p_is_id_found;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_node;
    
    hash_key.numeric_id = user_id;
    
    if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
        g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
        &hash_key, (gl_pvoid_t *)(&p_node), p_ecode ))
    {
        if((GL_HTM_ERR_KEY_NOT_FOUND == *p_ecode) ||
           (GL_HTM_ERR_NO_ELEMENT == *p_ecode))
        {
            *p_is_id_found = GL_FALSE;
            return GL_SUCCESS;
        }
        else
        {
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
    }
    
    *p_is_id_found = GL_TRUE;
    *p_p_user_id_info_node = (gl_mm_user_id_info_node_t *)p_node;
    
    return GL_SUCCESS; 
    
#endif
}


    

/*****************************************************************************
* Function name    : mm_remove_user_id_info_node
* Arguments        : gw_U32bit          user_id,
*                    gl_mm_user_id_info_node_t **p_p_user_id_infor_nodfe, 
*                    gw_boolean         *p_is_id_found,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To remove user-id info node corresponding to given 
*                    user-id. 
* Functions called :
*****************************************************************************/
gl_return_t    
mm_remove_user_id_info_node

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_mm_user_id_info_node_t **p_p_user_id_info_node,
      gw_boolean                *p_is_id_found,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_p_user_id_info_node, p_is_id_found, p_ecode )
      gw_U32bit                 user_id;
      gl_mm_user_id_info_node_t **p_p_user_id_info_node;
      gw_boolean                *p_is_id_found;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_node;
    gl_pvoid_t                  p_key_buf;
    
    hash_key.numeric_id = user_id;
    
    if(GL_FAILURE == gl_htm_remove_key_from_hash_table(
        g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
        &hash_key, &p_key_buf, (gl_pvoid_t *)(&p_node), p_ecode ))
    {
        if(GL_HTM_ERR_KEY_NOT_FOUND == *p_ecode)
        {
            *p_is_id_found = GL_FALSE;
            return GL_SUCCESS;
        }
        else
        {
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
    }
    
    *p_is_id_found = GL_TRUE;
    *p_p_user_id_info_node = (gl_mm_user_id_info_node_t *)p_node;
    
    return GL_SUCCESS; 
    
#endif
}


    
/*****************************************************************************
* Function name    : mm_release_all_id_allocated_bufs
* Arguments        : gw_U32bit          user_id,
*                    gl_mm_buffer_hdr_t *p_buf_hdr, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To release all the buffer allocated with given user-id.
* Functions called :
*****************************************************************************/
gl_return_t    
mm_release_all_id_allocated_bufs

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_mm_buffer_hdr_t        *p_buf_hdr,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_buf_hdr, p_ecode )
      gw_U32bit                 user_id;
      gl_mm_buffer_hdr_t        *p_buf_hdr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_pool_t                *p_pool;        
    gw_U32bit                   buf_index;
    gl_mm_buffer_hdr_t          *p_next_buf_hdr;

    (void)user_id;
    
    while(GL_NULL != p_buf_hdr)
    {
        p_next_buf_hdr = p_buf_hdr->p_next;
        
        p_pool = g_a_p_glb_pool_table[p_buf_hdr->pool_id];
    
        GL_MUTEX_LOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);
    
        if(p_pool->p_first == GL_NULL)
        {
            /*
             * If all buffers allocated set this buffer pointer as 
             * only buffer available in pool.
             */
            p_pool->p_first = p_buf_hdr;
            p_pool->p_last = p_buf_hdr;
            p_pool->p_first->p_prev = p_pool->p_last->p_next = GL_NULL;
        }
        else 
        {
            /*
             * Store buffer at tail of free buffers list of pool.
             */
            p_buf_hdr->p_prev = p_pool->p_last;
            p_pool->p_last->p_next = p_buf_hdr;
            p_pool->p_last = p_buf_hdr;
            p_pool->p_last->p_next = GL_NULL;
        }
        /*
         * Reset busy flag. 
         */
        p_buf_hdr->busy_flag = GL_FALSE;
        /*
         * Decrement count of busy buffers.
         */
        (p_pool->busy_buf_qty)--;
    
#ifdef GL_MM_ADD_DEBUGGER
        /*
         * Updating Buffer Trace Information.
         */
        buf_index = ((gw_U8bit *)p_buf_hdr - (gw_U8bit *)p_pool->p_hdr_region)
            /sizeof(gl_mm_buffer_hdr_t);  
        
        if(GL_MM_ENABLE_BUF_TRACE & p_pool->is_enabled_flag)
        {
            mm_buffer_trace_update(p_pool, buf_index, GL_FALSE, p_buf_hdr->
                task_id, p_buf_hdr);
        }
#else
        (void)buf_index;
#endif

        p_buf_hdr->task_id = GL_INVALID_TASK_ID;
    
        /*
         * Call to check current congestion region function.
         */
        if(GL_TRUE == p_pool->is_cngst_mgmt_enabled &&
            p_pool->current_region_index != 0 &&
            p_pool->specs.num_regions > 0)
        {
            if(GL_FAILURE == mm_check_current_cngst_mgmt_region(
                p_buf_hdr->pool_id, p_pool, p_ecode))
            {
                GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);
            
                return GL_FAILURE;
            }
        }

        GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);

        p_buf_hdr = p_next_buf_hdr; 
        
    }/* End of While loop */
    
    return GL_SUCCESS;
    
#endif /* GL_DEBUG */
}


/*****************************************************************************
* Function name    : mm_add_buf_to_user_id_info_node 
* Arguments        : gw_U32bit          user_id,
*                    gl_mm_buffer_hdr_t *p_buf_hdr, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To add the buffer allocated in the list of buffers
*                    allocated with the given user-id. 
* Functions called :
*****************************************************************************/
gl_return_t    
mm_add_buf_to_user_id_info_node

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_mm_buffer_hdr_t        *p_buf_hdr,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_buf_hdr, p_ecode )
      gw_U32bit                 user_id;
      gl_mm_buffer_hdr_t        *p_buf_hdr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_node;
    gw_boolean                  is_id_found = GL_TRUE;
    gl_mm_user_id_info_node_t   *p_user_id_info_node;
    
    GL_MUTEX_LOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
    
    hash_key.numeric_id = user_id;
    
    if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
        g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
        &hash_key, (gl_pvoid_t *)(&p_node), p_ecode ))
    {
        if((GL_HTM_ERR_KEY_NOT_FOUND == *p_ecode) ||
           (GL_HTM_ERR_NO_ELEMENT == *p_ecode))
        {
            is_id_found = GL_FALSE;
        }
        else
        {
            GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
    }
    
    if(GL_TRUE == is_id_found)
    {
        p_user_id_info_node = (gl_mm_user_id_info_node_t *)p_node;
        /*
         * Update the liost of buffers allocated with the given user-id.
         */ 
        p_buf_hdr->p_next = p_user_id_info_node->p_first_aloc_buf;
        p_buf_hdr->p_prev = GL_NULL;
        p_user_id_info_node->p_first_aloc_buf->p_prev = p_buf_hdr;
        p_user_id_info_node->p_first_aloc_buf = p_buf_hdr;
        
        (p_user_id_info_node->user_id_info.num_of_aloc_bufs)++;
    }
    else
    {
        if(GL_NULL == (p_user_id_info_node = (gl_mm_user_id_info_node_t *)
            gl_mm_get_buffer(g_mm_audit_special_user_id,
            sizeof(gl_mm_user_id_info_node_t), p_ecode)))
        {
            GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
        
            return GL_FAILURE;
        }
        
        p_user_id_info_node->p_first_aloc_buf = p_buf_hdr;
        p_buf_hdr->p_next = p_buf_hdr->p_prev = GL_NULL;
        
        p_user_id_info_node->user_id_info.num_of_aloc_bufs = 1;
        
        if(GL_FAILURE == gl_tm_get_timeval(&(p_user_id_info_node->user_id_info.
            time_stamp)))
        {
            GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
            
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        
        hash_key.numeric_id = user_id;
        
        if(GL_FAILURE == gl_htm_add_key_to_hash_table(
            g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
            &hash_key, (gl_pvoid_t *)(p_user_id_info_node), &p_node, p_ecode))
        {
            GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
            
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
        
    }
  
    GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS; 
    
#endif
}


    
/*****************************************************************************
* Function name    : mm_release_buf_from_user_id_info_node 
* Arguments        : gw_U32bit          user_id,
*                    gl_mm_buffer_hdr_t *p_buf_hdr, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To release buffer allocated from the list of buffers
*                    allocated with the given user-id. 
* Functions called :
*****************************************************************************/
gl_return_t    
mm_release_buf_from_user_id_info_node

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_mm_buffer_hdr_t        *p_buf_hdr,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_buf_hdr, p_ecode )
      gw_U32bit                 user_id;
      gl_mm_buffer_hdr_t        *p_buf_hdr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_hash_key_ut              hash_key;
    gl_pvoid_t                  p_node;
    gw_boolean                  is_id_found = GL_TRUE;
    gl_mm_user_id_info_node_t   *p_user_id_info_node;
    
    GL_MUTEX_LOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
    
    hash_key.numeric_id = user_id;
    
    if(GL_FAILURE == gl_htm_get_key_ref_from_hash_table(
        g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
        &hash_key, (gl_pvoid_t *)(&p_node), p_ecode ))
    {
        GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
        *p_ecode = GL_MM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    
    p_user_id_info_node = (gl_mm_user_id_info_node_t *)p_node;
    /*
     * Update the list of buffers allocated with the given user-id.
     */ 
    if(p_buf_hdr == p_user_id_info_node->p_first_aloc_buf)
    {
        p_user_id_info_node->p_first_aloc_buf = p_buf_hdr->p_next;
            
        if(GL_NULL == p_user_id_info_node->p_first_aloc_buf)
        {
            if(GL_FAILURE == mm_remove_user_id_info_node(user_id,
                &p_user_id_info_node, &is_id_found, p_ecode))
            {
                return GL_FAILURE;
            }
            
            if(GL_FAILURE == gl_mm_release_buffer(g_mm_audit_special_user_id,
                (gl_pvoid_t)p_user_id_info_node, p_ecode))
            {
                GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
        
                return GL_FAILURE;
            }
        }
        else
        {
            p_user_id_info_node->p_first_aloc_buf->p_prev = GL_NULL;
        }
    }
    else if(GL_NULL == p_buf_hdr->p_next)
    {
        p_buf_hdr->p_prev->p_next = GL_NULL;
    }
    else
    {
        p_buf_hdr->p_prev->p_next = p_buf_hdr->p_next;
        p_buf_hdr->p_next->p_prev = p_buf_hdr->p_prev;
    }
                
    (p_user_id_info_node->user_id_info.num_of_aloc_bufs)--;
    
    GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS; 
    
#endif
}


/*****************************************************************************
* Function name    : mm_audit_memory_free_func
* Arguments        : gw_U32bit          user_id,
*                    gl_hash_key_ut     *p_key,
*                    gw_U8bit           key_type,
*                    gl_pvoid_t         p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Free the memory allocated for the user-id information node.
* Functions called :
*****************************************************************************/
gl_return_t
mm_audit_memory_free_func

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gl_hash_key_ut            *p_key,
      gw_U8bit                  key_type,
      gl_pvoid_t                p_element,
      gl_error_t                *p_ecode
    )  
#else
    ( user_id, p_key, key_type, p_element, p_ecode )
      gw_U32bit                 user_id;
      gl_hash_key_ut            *p_key;
      gw_U8bit                  key_type;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;   
#endif

/******************************************************************************/
{
    /*
     * To remove warnings.
     */
    (void)user_id;
    (void)p_key;
    (void)key_type;

    if(GL_FAILURE == gl_mm_release_buffer(g_mm_audit_special_user_id,
        (gl_pvoid_t)p_element, p_ecode))
    {
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}


#endif /* EndIf for GL_MM_ADD_AUDIT_MGT */ 
        
