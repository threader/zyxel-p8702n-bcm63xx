/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_glb_best_fit_alloc_schm.c
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
* 31/01/2003    Ravi Jindal     SPR-5312     An API To get owner-id of a
*                                            buffer is added.
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/
  
#include "gl_mm.h"


/*****************************************************************************
* Function name    : gl_mm_common_pool_create
* Arguments        : gl_pool_id_t       *p_pool_id,
*                    gw_U32bit          no_of_buf, 
*                    gw_U32bit          buf_size, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Create common memory pool. Obtain memory from the
*                    system for common pool and divide and initialize the
*                    header of each buffer block of memory pool.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_common_pool_create

#ifdef _ANSI_C_
    ( gl_pool_id_t              *p_pool_id,
      gw_U32bit                 no_of_buf, 
      gw_U32bit                 buf_size, 
      gl_error_t                *p_ecode
    )
#else
    ( p_pool_id, no_of_buf, buf_size, p_ecode )
      gl_pool_id_t              *p_pool_id;
      gw_U32bit                 no_of_buf;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;
    gl_mm_pool_t*               p_temp;
    gl_mm_pool_t                *p_pool;        
    gl_mm_buffer_hdr_t*         p_real_buf=GL_NULL;
    gl_mm_check_mem_overwrite_t *p_buf_start = GL_NULL; 
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    /*
     * Validate buffer qty.
     */
    if(no_of_buf ==0 || no_of_buf > GL_MM_MAX_NO_OF_BUF)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_QTY;
        return GL_FAILURE;
    }
    /*
     * Validate buffer size.
     */
    if(buf_size ==0 || buf_size > GL_MM_MAX_BUF_SIZE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_SIZE;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
    /*
     * Validate poolId.
     */
    for(i=0; i<GL_MM_MAX_COMMON_POOL_QTY; i++)
    {
        if(GL_NULL == g_a_p_glb_pool_table[i])
        {
            break;
        }
    }

    if(i == GL_MM_MAX_COMMON_POOL_QTY)
    {
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode=GL_MM_ERR_POOL_NOT_AVAIL;
        return GL_FAILURE;
    }


    /*
     * As to Allign the buffers at word boundry (4 bytes)
     */ 
    if(0 != (buf_size % 4))
    {
        buf_size = buf_size + (4 - (buf_size % 4));
    }

    /*
     * Dynamic allocation of stack array.
     * Total memory reserved for buffer allocation request
     * (including Maintaince Structure) is 
     * size of pool header +
     * num of buffer * size of each buffer +
     * num of buffer * size of buffer header +
     * (num of buffer + 1) * size of overwrite check structure. 
     */
    if((gl_pvoid_t)GL_NULL == (p_pool = g_a_p_glb_pool_table[i] =
                (gl_mm_pool_t *)SYS_MALLOC(sizeof(gl_mm_pool_t) +
                no_of_buf *(sizeof(gl_mm_buffer_hdr_t)+ buf_size) + 
                (no_of_buf + 1) *( sizeof(gl_mm_check_mem_overwrite_t)))))
    {
        GL_INC_MM_B_G_MALOC_FAIL()

        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode=GL_MM_ERR_MALLOC_FAILED;
        return GL_FAILURE;
    }

#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_mm_stat_allocte_bfit_memory(i, p_ecode))
    {
        GL_INC_MM_B_G_MALOC_FAIL()

        SYS_FREE(p_pool);
        p_pool = GL_NULL;

        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif    

   
#ifdef GL_MT_ENABLED
    if(GL_SUCCESS != gl_create_mutex (&(p_pool->mem_sem), p_ecode))
    {
        GL_INC_MM_B_G_MUTX_FAIL()

        SYS_FREE(p_pool);
        p_pool = GL_NULL;
        
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode = GL_MM_CREATE_MUTEX_FAILED;
        return GL_FAILURE;
    }
#endif
    p_pool->p_hdr_region = (gl_mm_buffer_hdr_t *)((gw_S8bit *)p_pool +
        sizeof(gl_mm_pool_t)); 
    
    p_pool->p_buf_region = (gw_S8bit *)(p_pool->p_hdr_region + no_of_buf); 
    /* 
     * As pool-id "0" is reserved.
     */ 
    (*p_pool_id) = (gl_pool_id_t)(i+1);
    

    p_pool->buf_size      = buf_size;
    p_pool->buf_qty       = no_of_buf;
    p_pool->busy_buf_qty  = 0;
    p_pool->owner_task_id = GL_INVALID_TASK_ID; /* INVALID task Id */
    p_pool->p_first       = GL_NULL;
    p_pool->p_last        = GL_NULL;
    p_pool->p_next_high_mem_buf_pool = GL_NULL;
    p_pool->cngst_mgmt_call_back     = GL_NULL;
    p_pool->is_cngst_mgmt_enabled    = GL_FALSE;
    p_pool->current_region_index     = 0;
    /*
     * Set Congestion Region Specification Information by "0".
     */
    gl_mm_memset((gw_U8bit*)&(p_pool->specs),
        0, sizeof(gl_cngst_mgmt_spec_t));
    
#ifdef GL_MM_ADD_DEBUGGER
    p_pool->curr_max_used         = GL_NULL;
    p_pool->is_enabled_flag       = GL_MM_NO_ACTIVITY;
    p_pool->p_start_stk_inf_reg   = GL_NULL;
    p_pool->p_start_buf_trace_reg = GL_NULL;
    p_pool->p_pool_usage_profile  = GL_NULL;
#endif /* Endif for GL_MM_ADD_DEBUGGER */
    
    for(j=0; j < no_of_buf; j++)
    {
        /*
         * Store poolId and other information in buffer header 
         * at beginning of each buffer.
         */
        p_real_buf = (gl_mm_buffer_hdr_t*)(p_pool->p_hdr_region+j);
        
        p_real_buf->pool_id   = i;
        p_real_buf->busy_flag = GL_FALSE;
        p_real_buf->task_id   = GL_INVALID_TASK_ID;
#ifdef GL_MM_ADD_DEBUGGER
        p_real_buf->buffer_alloc_signature = 0;
        p_real_buf->curr_buf_trace_index = 0;
#endif /* Endif for GL_MM_ADD_DEBUGGER */

        p_buf_start = (gl_mm_check_mem_overwrite_t *)
        (p_pool->p_buf_region + 
         (j * (buf_size + sizeof(gl_mm_check_mem_overwrite_t))));

        p_buf_start->pool_id = i;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
        p_buf_start->start_check_bit = GL_MM_CHECK_BIT;
        p_buf_start->end_check_bit = GL_MM_CHECK_BIT;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

        if(no_of_buf == 1)
        {
            p_real_buf->p_prev = GL_NULL;
            p_real_buf->p_next = GL_NULL;
            p_pool->p_first = p_pool->p_last = p_real_buf;
            break;
        }
        if(j == 0)
        {
            p_real_buf->p_prev = GL_NULL;
            p_real_buf->p_next = (gl_mm_buffer_hdr_t*)(p_real_buf + 1 );
            p_pool->p_first = p_real_buf;
            continue;
        }
        else if(j == (no_of_buf - 1))
        {
            /*
             * Set last buffer in pool.
             */
            p_real_buf->p_next = GL_NULL;
            p_real_buf->p_prev = (gl_mm_buffer_hdr_t*)(p_real_buf - 1 );
            p_pool->p_last = p_real_buf;
            break; 
        }
        else
        {
            p_real_buf->p_next = (gl_mm_buffer_hdr_t*)(p_real_buf + 1 );
            p_real_buf->p_prev = (gl_mm_buffer_hdr_t*)(p_real_buf - 1 );
            continue;
        }
    }
    /*
     * Set the check-bit values at end of last buffer.
     */
    p_buf_start = (gl_mm_check_mem_overwrite_t *)((gw_S8bit *)p_buf_start + 
        buf_size + sizeof(gl_mm_check_mem_overwrite_t)); 
    
    p_buf_start->pool_id = i;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    p_buf_start->start_check_bit = GL_MM_CHECK_BIT;
    p_buf_start->end_check_bit = GL_MM_CHECK_BIT;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    
    /*
     * Set pool in linked list storing pool 
     * reference in increasing order.
     */
    if(p_gl_smallest_mem_buf_pool == (gl_pvoid_t)GL_NULL)
    {
        /*
         * First common pool allocated.
         */
        p_gl_smallest_mem_buf_pool = g_a_p_glb_pool_table[i];
    }
    else
    {
        /*
         * In case new pool's buffer size is smallest.
         */
        if(p_gl_smallest_mem_buf_pool->buf_size >= buf_size)
        {
            p_pool->p_next_high_mem_buf_pool =
                p_gl_smallest_mem_buf_pool;
            p_gl_smallest_mem_buf_pool = g_a_p_glb_pool_table[i];
        
            GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
            return GL_SUCCESS;
        }
        /*
         * Else search complete list till you find pool with 
         * buffer size more than new pool's buffer size.
         */
        for(p_temp=p_gl_smallest_mem_buf_pool;
            p_temp->p_next_high_mem_buf_pool != (gl_pvoid_t)GL_NULL;
                p_temp=p_temp->p_next_high_mem_buf_pool)
        {
            if(p_temp->p_next_high_mem_buf_pool->buf_size >= buf_size)
            {
                p_pool->p_next_high_mem_buf_pool = 
                    p_temp->p_next_high_mem_buf_pool;
                p_temp->p_next_high_mem_buf_pool
                    = g_a_p_glb_pool_table[i];
        
                GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
                return GL_SUCCESS;
            }
            if(p_temp->p_next_high_mem_buf_pool->buf_size < buf_size)
                continue;
        }
        p_pool->p_next_high_mem_buf_pool = GL_NULL;
        /*
         * If all pools have smaller buffer size then add new pool 
         * at end of list.
         */
        p_temp->p_next_high_mem_buf_pool= g_a_p_glb_pool_table[i];
    }
    
    GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

    GL_MM_B_C_BSY_BUFS(i, 0)
    GL_MM_B_C_BUF_QTY(i, no_of_buf)
    
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_get_buffer
* Arguments        : gw_U32bit          task_id,
*                    gw_U32bit          buf_size,
*                    gl_error_t         *p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : Allocate memory buffer of suitable size from 
*                    common memory pool.
* Functions called :
*****************************************************************************/
gl_pvoid_t    
gl_mm_get_buffer

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gw_U32bit                 buf_size, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, buf_size, p_ecode )
      gw_U32bit                 task_id;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{

#ifdef GL_DEBUG
    return(SYS_MALLOC(buf_size));
#else
    gl_mm_pool_t*               p_temp=GL_NULL;
    gl_mm_buffer_hdr_t*         p_alloc=GL_NULL;
    gl_boolean                  found = GL_FALSE;
    gw_U32bit                   buf_index;
    gl_mm_check_mem_overwrite_t *p_buf_start;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 
    
#ifdef GL_STATS_ENABLED
    gl_boolean                  is_over_size_buf_alloc = GL_FALSE;
#endif /* Endif for GL_STATS_ENABLED */
    
    GL_INC_MM_B_G_GET_BUF_RQ()
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_NULL;
    }
    /*
     * Validate buffer size.
     */
    if(buf_size == 0 || buf_size > GL_MM_MAX_BUF_SIZE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_SIZE;
        return GL_NULL;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * Find suitable poolId from which to allocate memory
     * starting from pool with smallest buffer size.
     */
    for(p_temp=p_gl_smallest_mem_buf_pool;
        p_temp != GL_NULL; p_temp=p_temp->p_next_high_mem_buf_pool)
    {
        if(p_temp->buf_size >= buf_size)
        {
            /*
             * If buffer found set found flag.
             */
            found=GL_TRUE;
            
            GL_INC_MM_B_C_GET_BUF_RQ(p_temp->p_hdr_region->pool_id)

            GL_MUTEX_LOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
    
            if(p_temp->p_first != (gl_pvoid_t)GL_NULL)
            {
                /*
                 * If buffer available allocate same
                 * and return success.
                 */
                p_alloc = p_temp->p_first;
                
                buf_index = GL_MM_BUF_INDEX(p_alloc, p_temp->p_hdr_region,
                        sizeof(gl_mm_buffer_hdr_t));
                
                p_buf_start = (gl_mm_check_mem_overwrite_t *)
                    ((gw_S8bit *)p_temp->p_buf_region + (buf_index * 
                    (p_temp->buf_size + sizeof(gl_mm_check_mem_overwrite_t)))); 
                
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
                p_buf_end = (gl_mm_check_mem_overwrite_t *)
                    ((gw_S8bit *)p_temp->p_buf_region + ( (buf_index+1) * 
                    (p_temp->buf_size + sizeof(gl_mm_check_mem_overwrite_t))));
                
                if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
                   (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) ||
                   (p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
                   (p_buf_end->end_check_bit != GL_MM_CHECK_BIT))
                {
                    GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
                    
                    GL_INC_MM_B_G_BUF_FND_CRRPTD()
                    GL_INC_MM_B_C_BUF_FND_CRRPTD(p_temp->p_hdr_region->pool_id)

                    *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
                    return GL_FAILURE;
                }
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

                p_temp->p_first = p_temp->p_first->p_next;
                if(p_temp->p_first == (gl_pvoid_t)GL_NULL)
                {
                    p_temp->p_last=GL_NULL;
                }
                else
                {
                    p_temp->p_first->p_prev = GL_NULL;
                }
                p_alloc->busy_flag = GL_TRUE;
                p_alloc->task_id = task_id;
                (p_temp->busy_buf_qty)++;
                
#ifdef GL_STATS_ENABLED
                if(GL_TRUE == is_over_size_buf_alloc)
                {
                    GL_INC_MM_B_G_OVERSIZE_BUF_ALOC()
                }
#endif /* Endif for GL_STATS_ENABLED */

                GL_MM_B_C_PEAK_BSY_BUFS(p_alloc->pool_id,p_temp->busy_buf_qty);
                GL_MM_B_C_BSY_BUFS(p_alloc->pool_id,p_temp->busy_buf_qty);
                
#ifdef GL_MM_ADD_DEBUGGER          
                p_alloc->buffer_alloc_signature = buffer_alloc_signature; 
                /*
                 * Updating maximum number of buffers used so far.
                 */ 
                if(p_temp->curr_max_used < p_temp->busy_buf_qty)
                {    
                    p_temp->curr_max_used = p_temp->busy_buf_qty;
                }
                /*
                 * Updating Stack Trace Information.
                 */
                if( GL_MM_ENABLE_STK_INF & p_temp->is_enabled_flag )
                {
                    mm_stack_trace_update(p_temp, buf_index);
                }
                /*
                 * Updating Buffer Trace Information.
                 */
                if( GL_MM_ENABLE_BUF_TRACE & p_temp->is_enabled_flag )
                {
                    mm_buffer_trace_update(p_temp, buf_index, 
                            GL_TRUE, task_id, p_alloc);
                }
                /*
                 * Updating Pool Usage Information.
                 */
                if( GL_MM_ENABLE_POOL_USAGE & p_temp->is_enabled_flag )
                {
                    mm_pool_usage_update(p_temp, buf_size); 
                }
#endif /* Endif for GL_MM_ADD_DEBUGGER */

                /*
                 * Call to check current congestion region function.
                 */
                if(GL_TRUE == p_temp->is_cngst_mgmt_enabled &&
                   p_temp->current_region_index != p_temp->specs.num_regions &&
                   p_temp->specs.num_regions > 0)
                {
                    if(GL_FAILURE == mm_check_current_cngst_mgmt_region(
                      p_alloc->pool_id, g_a_p_glb_pool_table[p_alloc->pool_id],
                      p_ecode))
                    {
                        GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
    
                        return GL_NULL;
                    }
                }
                
                GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
                
#ifdef GL_MM_ADD_AUDIT_MGT
                if((GL_TRUE == audit_act_set_flag) && 
                   (GL_INVALID_TASK_ID != task_id) &&
                   (g_mm_audit_special_user_id != task_id))
                {
                    if(GL_FAILURE == mm_add_buf_to_user_id_info_node(task_id,
                        p_alloc, p_ecode))
                    {
                        return GL_FAILURE;
                    }
                }
#endif /* Endif for GL_MM_ADD_AUDIT_MGT */

                return ( (gw_S8bit *)p_buf_start + 
                         sizeof(gl_mm_check_mem_overwrite_t));
            }
#ifdef GL_STATS_ENABLED
            else
            {
                is_over_size_buf_alloc = GL_TRUE;       
                GL_INC_MM_B_C_MEMGET_FAIL(p_temp->p_hdr_region->pool_id)
            }
#endif /* Endif for GL_STATS_ENABLED */
            
            GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
        }
    }

    if(p_temp == (gl_pvoid_t)GL_NULL && found == GL_FALSE)
    {
        /*
         * Check if no common pool available for memory size,i.e., 
         * buffer size required is more than any of buffer sizes
         * of initialized pools.
         */
        *p_ecode = GL_MM_ERR_POOL_NOT_AVAIL;
    }
    else
    {    
        /*
         * Check if no free buffer available for memory size,i.e., 
         * pool is available but free buffer is not available.
         */
        *p_ecode = GL_MM_ERR_BUF_NOT_AVAIL;
    }

    GL_INC_MM_B_G_MEMGET_FAIL()

    return GL_NULL;
#endif
}


/*****************************************************************************
* Function name    : gl_mm_release_buffer 
* Arguments        : gw_U32bit          task_id,
*                    gl_pvoid_t         buf_ptr, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deallocate memory buffer allocated from common
*                    memory pools.
* Functions called : mm_validate_common_pool_buffer_pointer
*****************************************************************************/
gl_return_t
gl_mm_release_buffer

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_pvoid_t                buf_ptr,  
      gl_error_t                *p_ecode
    )
#else
    ( task_id, buf_ptr,  p_ecode )
      gw_U32bit                 task_id;
      gl_pvoid_t                buf_ptr;  
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_DEBUG
    SYS_FREE(buf_ptr);
    return GL_SUCCESS;
#else
    
    gl_mm_pool_t                *p_pool;        
    gl_mm_buffer_hdr_t*         p_hdr;
    gw_U32bit                   buf_index;
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 


#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(buf_ptr == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));
    
    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
    
    pool_id = p_buf_start->pool_id;
    
    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr + g_a_p_glb_pool_table[pool_id]->buf_size);  
    
    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)buf_ptr - 
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;
    
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_common_pool_buffer_pointer
        (pool_id, buf_ptr, &buf_index))
    {
        GL_INC_MM_B_G_REL_INV_PTR()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_glb_pool_table[pool_id]->p_hdr_region + buf_index;

    p_pool = g_a_p_glb_pool_table[p_hdr->pool_id];
    /*
     * To check, if buffer is already free or not.
     */
    if(GL_FALSE == p_hdr->busy_flag)
    {
       GL_INC_MM_B_G_REL_FREE_BUF()

       *p_ecode= GL_MM_ERR_BUF_FREE;
        return GL_FAILURE; 
    }
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(p_hdr->task_id != task_id)
    {
        *p_ecode= GL_MM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */ 
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_AUDIT_MGT
    if((GL_TRUE == audit_act_set_flag) && 
       (GL_INVALID_TASK_ID != p_hdr->task_id) &&
       (g_mm_audit_special_user_id != p_hdr->task_id))
    {
        if(GL_FAILURE == mm_release_buf_from_user_id_info_node(p_hdr->task_id,
            p_hdr, p_ecode))
        {
            return GL_FAILURE;
        }
    }
#endif /* Endif for GL_MM_ADD_AUDIT_MGT */

    GL_MUTEX_LOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);
    
    if(p_pool->p_first == GL_NULL)
    {
        /*
         * If all buffers allocated set this buffer pointer as 
         * only buffer available in pool.
         */
        p_pool->p_first=p_hdr;
        p_pool->p_last=p_hdr;
        p_pool->p_first->p_prev = p_pool->p_last->p_next = GL_NULL;
    }
    else 
    {
        /*
         * Store buffer at tail of free buffers list of pool.
         */
        p_hdr->p_prev = p_pool->p_last;
        p_pool->p_last->p_next = p_hdr;
        p_pool->p_last = p_hdr;
        p_pool->p_last->p_next = GL_NULL;
    }
    /*
     * Reset busy flag. 
     */
    p_hdr->busy_flag = GL_FALSE;
    /*
     * Decrement count of busy buffers.
     */
    (p_pool->busy_buf_qty)--;

    GL_MM_B_C_BSY_BUFS(p_hdr->pool_id, p_pool->busy_buf_qty)
    
#ifdef GL_MM_ADD_DEBUGGER
    /*
     * Updating Buffer Trace Information.
     */
    if(GL_MM_ENABLE_BUF_TRACE & p_pool->is_enabled_flag)
    {
        mm_buffer_trace_update(p_pool, buf_index, GL_FALSE, p_hdr->task_id, 
            p_hdr);
    }
#endif /* Endif for GL_MM_ADD_DEBUGGER */

    p_hdr->task_id = GL_INVALID_TASK_ID;
    
    /*
     * Call to check current congestion region function.
     */
    if(GL_TRUE == p_pool->is_cngst_mgmt_enabled &&
       p_pool->current_region_index != 0 &&
       p_pool->specs.num_regions > 0)
    {
        if(GL_FAILURE == mm_check_current_cngst_mgmt_region(
                p_hdr->pool_id, p_pool, p_ecode))
        {
            GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);
            
            return GL_FAILURE;
        }
    }

    GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
    
#endif /* GL_DEBUG */
}



/*****************************************************************************
* Function name    : gl_mm_get_owner_of_buffer
* Arguments        : gl_pvoid_t         buf_ptr,
*                    gw_U32bit          *p_task_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To get the owner-id of a buffer
* Functions called : mm_validate_common_pool_buffer_pointer
*****************************************************************************/
gl_return_t
gl_mm_get_owner_of_buffer

#ifdef _ANSI_C_
    ( gl_pvoid_t                buf_ptr,
      gw_U32bit                 *p_task_id, 
      gl_error_t                *p_ecode
    )
#else
    ( buf_ptr, p_task_id, p_ecode )
      gl_pvoid_t                buf_ptr;  
      gw_U32bit                 *p_task_id;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_DEBUG
    SYS_FREE(buf_ptr);
    return GL_SUCCESS;
#else
    
    gl_mm_pool_t                *p_pool;        
    gl_mm_buffer_hdr_t*         p_hdr;
    gw_U32bit                   buf_index;
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 


#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(buf_ptr == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));
    
    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
    
    pool_id = p_buf_start->pool_id;
    
    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr + g_a_p_glb_pool_table[pool_id]->buf_size);  
    
    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)buf_ptr - 
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;
    
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_common_pool_buffer_pointer
        (pool_id, buf_ptr, &buf_index))
    {
        GL_INC_MM_B_G_REL_INV_PTR()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_glb_pool_table[pool_id]->p_hdr_region + buf_index;

    p_pool = g_a_p_glb_pool_table[p_hdr->pool_id];
    /*
     * To check, if buffer is already free or not.
     */
    if(GL_FALSE == p_hdr->busy_flag)
    {
       GL_INC_MM_B_G_REL_FREE_BUF()

       *p_ecode= GL_MM_ERR_BUF_FREE;
        return GL_FAILURE; 
    }
    *p_task_id = p_hdr->task_id;

    return GL_SUCCESS;
    
#endif /* GL_DEBUG */
}


/*****************************************************************************
* Function name    : gl_mm_change_owner_of_buffer
* Arguments        : gw_U32bit          old_task_id,
*                    gw_U32bit          new_task_id, 
*                    gl_pvoid_t         buf_ptr,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Change owner of allocated memory buffer.
* Functions called : validate_buffer_pointer
*****************************************************************************/
gl_return_t    
gl_mm_change_owner_of_buffer

#ifdef _ANSI_C_
    ( gw_U32bit                 old_task_id,
      gw_U32bit                 new_task_id, 
      gl_pvoid_t                buf_ptr, 
      gl_error_t                *p_ecode
    )
#else
    ( old_task_id, new_task_id, buf_ptr, p_ecode )
      gw_U32bit                 old_task_id;
      gw_U32bit                 new_task_id; 
      gl_pvoid_t                buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return(GL_SUCCESS);
#else
    
    gl_mm_pool_t                *p_pool;        
    gl_pool_id_t                pool_id;
    gl_mm_buffer_hdr_t*         p_hdr; 
    gw_U32bit                   buf_index;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 


#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(buf_ptr == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }

    pool_id = p_buf_start->pool_id;

    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr + g_a_p_glb_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)buf_ptr - 
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;
    
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_common_pool_buffer_pointer
        (pool_id, buf_ptr, &buf_index))
    {
        GL_INC_MM_B_G_REL_INV_PTR()
            
        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    
    p_hdr = g_a_p_glb_pool_table[pool_id]->p_hdr_region + buf_index;

    p_pool = g_a_p_glb_pool_table[p_hdr->pool_id]; 
    
    if(p_hdr->task_id != old_task_id)
    {
        GL_INC_MM_B_G_REL_FREE_BUF()
            
        *p_ecode= GL_MM_ERR_INV_OLD_TASK_ID;
        return GL_FAILURE;
    }

    GL_MUTEX_LOCK(&(p_pool->mem_sem), p_ecode,
            GL_FAILURE);
    
    /*
     * If buffer not currently allocated return failure.
     */
    if(p_hdr->busy_flag == GL_FALSE)
    {
        GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode,
            GL_FAILURE);
    
        *p_ecode= GL_MM_ERR_BUF_FREE;
        return GL_FAILURE;
    }
    /*
     * Change task id in buffer header maintained 
     * by memory manager.
     */
    p_hdr->task_id = new_task_id;

#ifdef GL_MM_ADD_DEBUGGER
    /*
     * Updating Buffer Trace Information.
     */
    if( GL_MM_ENABLE_BUF_TRACE & p_pool->is_enabled_flag )
    {
        mm_buffer_trace_update(p_pool, buf_index, GL_TRUE, new_task_id, p_hdr);
    }
#endif /* Endif for GL_MM_ADD_DEBUGGER */

    GL_MUTEX_UNLOCK(&(p_pool->mem_sem), p_ecode,
            GL_FAILURE);
    
    return GL_SUCCESS;
#endif /* GL_DEBUG */

}





/*****************************************************************************
* Function name    : gl_mm_delete_common_pool
* Arguments        : gw_U32bit          buf_size,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Free memory of buffer of all common memory pool 
*                    with buffer size equal to buf_size. 
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_delete_common_pool

#ifdef _ANSI_C_
    ( gw_U32bit                 buf_size, 
      gl_error_t                *p_ecode
    )
#else
    ( buf_size, p_ecode )
      gw_U32bit                 buf_size; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_pool_t                *p_temp = GL_NULL;
    gl_mm_pool_t                *p_next_pool = GL_NULL;
    gl_mm_pool_t                *p_prev_pool = GL_NULL;
    gl_boolean                  found_pool = GL_FALSE;
    gl_boolean                  is_busy_buf_flag = GL_FALSE;
    gl_pool_id_t                pool_id;
    

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    /*
     * Validate buffer size.
     */
    if(buf_size == 0 || buf_size > GL_MM_MAX_BUF_SIZE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_SIZE;
        return GL_NULL;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As to Allign the buffers at word boundry (4 bytes)
     */ 
    if(0 != (buf_size % 4))
    {
        buf_size = buf_size + (4 - (buf_size % 4));
    }

    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * Find poolId of buffer size equal to buf_size  
     * starting from pool with smallest buffer size.
     */
    for(p_temp = p_gl_smallest_mem_buf_pool;
        p_temp != GL_NULL; 
        p_prev_pool = p_temp, p_temp = p_next_pool)
    {
        if(p_temp->buf_size > buf_size)
        {
            /*
             * Pool with buffer size > buf size.
             */
            break;
        }
        if(p_temp->buf_size == buf_size)
        {
            /*
             * Set buffer found flag.
             */
            found_pool = GL_TRUE;

            GL_MUTEX_LOCK(&(p_temp->mem_sem), p_ecode, GL_FAILURE);

            p_next_pool = p_temp->p_next_high_mem_buf_pool;
    
            if( p_temp->busy_buf_qty != 0)
            {
                GL_INC_MM_B_G_DEL_RQ_POOL_BSY()

                is_busy_buf_flag = GL_TRUE;
        
                GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_FAILURE);
    
                continue;
            }
            pool_id = p_temp->p_hdr_region->pool_id;

#ifdef GL_STATS_ENABLED
            gl_mm_stat_free_bfit_memory(p_temp->p_hdr_region->pool_id,
                p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

            if( p_temp == p_gl_smallest_mem_buf_pool)
            {
                p_gl_smallest_mem_buf_pool = p_next_pool;
            }
            else
            {
                p_prev_pool->p_next_high_mem_buf_pool = p_next_pool;
            }

            p_temp->p_next_high_mem_buf_pool = GL_NULL;
            p_temp->p_hdr_region          = (gl_mm_buffer_hdr_t *)GL_NULL;
            p_temp->p_buf_region          = (gw_S8bit *)GL_NULL;
            p_temp->p_first                  = GL_NULL;
            p_temp->p_last                   = GL_NULL;
            p_temp->buf_qty                  = GL_NULL;
            p_temp->busy_buf_qty             = GL_NULL;
            p_temp->buf_size                 = GL_NULL;
            p_temp->owner_task_id            = GL_INVALID_TASK_ID;
            p_temp->current_region_index     = 0;
            p_temp->is_cngst_mgmt_enabled    = GL_FALSE;
#ifdef GL_MM_ADD_DEBUGGER
            p_temp->curr_max_used            = GL_NULL;
            /*
             * Disable Stack Trace activity.
             */
            if((GL_MM_ENABLE_STK_INF & p_temp->is_enabled_flag)
            && (GL_NULL != p_temp->p_start_stk_inf_reg))
            {
                /*
                 * Flush stack trace inforamtion.
                 */
                p_temp->is_enabled_flag = (gw_U8bit)
                    (p_temp->is_enabled_flag &
                    !(GL_MM_ENABLE_STK_INF));

                SYS_FREE(p_temp->p_start_stk_inf_reg);
                p_temp->p_start_stk_inf_reg = GL_NULL;
            }
            /*
             * Disable Buffer Trace activity.
             */
            if((GL_MM_ENABLE_BUF_TRACE & p_temp->is_enabled_flag)
            && (GL_NULL != p_temp->p_start_buf_trace_reg))
            {
                /*
                 * Flush buffer trace inforamtion.
                 */
                p_temp->is_enabled_flag = (gw_U8bit)
                    (p_temp->is_enabled_flag &
                    !(GL_MM_ENABLE_BUF_TRACE));

                SYS_FREE(p_temp->p_start_buf_trace_reg);
                p_temp->p_start_buf_trace_reg = GL_NULL;
            }
            /*
             * Disable Pool Usage activity.
             */
            if((GL_MM_ENABLE_POOL_USAGE & p_temp->is_enabled_flag)
            && (GL_NULL != p_temp->p_pool_usage_profile))
            {
                /*
                 * Flush Poll Usage inforamtion.
                 */
                p_temp->is_enabled_flag = (gw_U8bit)
                    (p_temp->is_enabled_flag &
                    !(GL_MM_ENABLE_POOL_USAGE));

                SYS_FREE(p_temp->p_pool_usage_profile);
                p_temp->p_pool_usage_profile = GL_NULL;
            }
#endif /* Endif for GL_MM_ADD_DEBUGGER */
            /*
             * Set Congestion Region Specification Information by "0".
             */ 
            gl_mm_memset((gw_U8bit*)&(p_temp->specs),  
                         0, sizeof(gl_cngst_mgmt_spec_t)); 
        
            GL_MUTEX_DELETE(&(p_temp->mem_sem), p_ecode, GL_FAILURE);
            SYS_FREE(p_temp);
            p_temp = GL_NULL;
            g_a_p_glb_pool_table[pool_id] = GL_NULL;
        }
        else
        {
            p_next_pool = p_temp->p_next_high_mem_buf_pool;
        }
    }
    
    GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * If no common pool of buffer size equal to buf_size.
     */ 
    if(found_pool == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    if(is_busy_buf_flag == GL_TRUE)
    {
        *p_ecode = GL_MM_ERR_BUF_ALLOCATED;
        return GL_FAILURE;
    }
    
    return GL_SUCCESS;
#endif
}





/*****************************************************************************
* Function name    : gl_mm_release_all_common_pool_buffers
* Arguments        : gw_U32bit          buf_size, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Release all buffers of all common memory pool with 
*                    buffer size equal to buf_size.The status of pool is 
*                    similar to one after create pool. 
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_release_all_common_pool_buffers

#ifdef _ANSI_C_
    ( gw_U32bit                 buf_size, 
      gl_error_t                *p_ecode
    )
#else
    ( buf_size, p_ecode )
      gw_U32bit                 buf_size; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_pool_t                *p_temp;
    gl_mm_buffer_hdr_t          *p_real_buf;
    gl_boolean                  found = GL_FALSE;
    gw_U32bit                   no_of_buf;
    gw_U32bit                   j;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    /*
     * Validate buffer size.
     */
    if(buf_size == 0 || buf_size > GL_MM_MAX_BUF_SIZE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_SIZE;
        return GL_NULL;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As to Allign the buffers at word boundry (4 bytes)
     */ 
    if(0 != (buf_size % 4))
    {
        buf_size = buf_size + (4 - (buf_size % 4));
    }
    /*
     * As all buffers are free now, create free list of buffers of pool.
     */
    for(p_temp = p_gl_smallest_mem_buf_pool;
        p_temp != GL_NULL; p_temp=p_temp->p_next_high_mem_buf_pool)
    {
        if(p_temp->buf_size > buf_size)
        {
            /*
             * Pool with buffer size > buf size.
             */
            break;
        }
        if(p_temp->buf_size == buf_size)
        {
            /*
             * Set buffer found flag.
             */
            found=GL_TRUE;

            GL_MUTEX_LOCK(&(p_temp->mem_sem), p_ecode, GL_FAILURE);
    
            no_of_buf = p_temp->buf_qty;
            p_temp->busy_buf_qty = 0;
            p_temp->current_region_index = 0;
            
            GL_MM_B_C_BSY_BUFS(p_temp->p_hdr_region->pool_id, 0)
            
            for(j=0; j < no_of_buf; j++)
            {
                /*
                 * Store poolId and other information in buffer header
                 * at beginning of each buffer.
                 */
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
                p_buf_start = (gl_mm_check_mem_overwrite_t *)(p_temp->p_buf_region + 
                    (j * (p_temp->buf_size + sizeof(gl_mm_check_mem_overwrite_t))));
                if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
                   (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
                {
                    GL_INC_MM_B_G_BUF_FND_CRRPTD()
                    GL_INC_MM_B_C_BUF_FND_CRRPTD(p_temp->p_hdr_region->pool_id)

                    *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
                    return GL_FAILURE;
                }
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

                p_real_buf = (gl_mm_buffer_hdr_t*)(p_temp->p_hdr_region+j);
                
#ifdef GL_MM_ADD_DEBUGGER
                /*
                 * Updating Buffer Trace Information.
                 */
                if((GL_TRUE == p_real_buf->busy_flag) &&
                   (GL_MM_ENABLE_BUF_TRACE & p_temp->is_enabled_flag))
                {
                    mm_buffer_trace_update(p_temp, j, GL_FALSE, 
                            p_real_buf->task_id, p_real_buf);
                }
                p_real_buf->buffer_alloc_signature = 0;
#endif /* Endif for GL_MM_ADD_DEBUGGER */
                p_real_buf->busy_flag = GL_FALSE;
                p_real_buf->task_id = GL_INVALID_TASK_ID;
                
                if(no_of_buf == 1)
                {
                    p_real_buf->p_prev=GL_NULL;
                    p_real_buf->p_next=GL_NULL;
                    p_temp->p_first = p_temp->p_last = p_real_buf;
                    break;
                }
                if(j == 0)
                {
                    p_real_buf->p_prev = GL_NULL;
                    p_real_buf->p_next = (gl_mm_buffer_hdr_t*) (p_real_buf + 1); 
                    p_temp->p_first = p_real_buf;
                    continue;
                } 
                else if(j == (no_of_buf - 1))
                {
                    /*
                     * Set last buffer in pool.
                     */
                    p_real_buf->p_next = GL_NULL;
                    p_real_buf->p_prev = (gl_mm_buffer_hdr_t*)(p_real_buf - 1 );
                    p_temp->p_last = p_real_buf;
                    break;
                }
                else
                {
                    p_real_buf->p_next = (gl_mm_buffer_hdr_t*)(p_real_buf + 1); 
                    p_real_buf->p_prev = (gl_mm_buffer_hdr_t*)(p_real_buf - 1);
                    continue;
                }
            }
            
            GL_MUTEX_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_FAILURE);
        }
    }
    /*
     * If no common pool of buffer size equal to buf_size.
     */ 
    if(found == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    return GL_SUCCESS;
#endif
}

