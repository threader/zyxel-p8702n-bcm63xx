/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_appl_sp_slectv_aloc_schm.c
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
* 10/10/2002    Prashant Jain   SPR-4936     Compilation Problem with GL_DEBUG 
*                                            flag on. 
*
* 10/10/2002    Ravi Jindal     SPR-4941     To add fix in pool creation API 
*                                            with GL_DEBUG flag on. 
*
* 28/11/2002    Ravi Jindal     SPR-5119     To remove task_id resting in 
*                                            gl_mm_release_all_task_pool_buffe
*                                            rs function call.
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/
  
#include "gl_mm.h"


/*****************************************************************************
* Function name    : gl_mm_pool_create
* Arguments        : gw_U32bit          task_id,
*                    gw_U32bit          no_of_buf,
*                    gw_U32bit          buf_size, 
*                    gl_pool_id_t       *p_pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Create task memory pool. Obtain memory from the
*                    system for task pool and divide and initialize 
*                    header of each buffer of memory pool and 
*                    allocate a pool_id.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_pool_create

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gw_U32bit                 no_of_buf, 
      gw_U32bit                 buf_size, 
      gl_pool_id_t              *p_pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, no_of_buf, buf_size, p_pool_id, p_ecode )
      gw_U32bit                 task_id;
      gw_U32bit                 no_of_buf;
      gw_U32bit                 buf_size; 
      gl_pool_id_t              *p_pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    gw_U16bit                   i;
    for(i=0; i<GL_MM_MAX_TASK_POOL_QTY; i++)
    {
        if(GL_NULL == g_a_p_app_sp_pool_table[i])
        {
            g_a_p_app_sp_pool_table[i] = (gl_mm_pool_t *)SYS_MALLOC(sizeof
                    (gl_mm_pool_t));
            if(GL_NULL == g_a_p_app_sp_pool_table[i])
            {
                *p_ecode=GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
            g_a_p_app_sp_pool_table[i]->buf_size = buf_size;
            (*p_pool_id) = (gl_pool_id_t)(i+1);
            return GL_SUCCESS;
        }
    }
    return GL_SUCCESS;
#else
    
    gw_U16bit                   i;
    gw_U32bit                   j;
    gl_mm_buffer_hdr_t*         p_real_buf=GL_NULL;
    gl_mm_check_mem_overwrite_t *p_buf_start = GL_NULL;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INIT_REQD;
        return GL_FAILURE; 
    }
    /*
     * Validate buffer qty.
     */
    if(no_of_buf ==0 || no_of_buf > GL_MM_MAX_NO_OF_BUF)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_QTY;
        return GL_FAILURE;
    }
    /*
     * Validate buffer size against maximum value 
     * specified in gl_config.
     */
    if(buf_size ==0 || buf_size > GL_MM_MAX_BUF_SIZE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_BUF_SIZE;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
    /*
     * Validate poolId.
     */
    for(i=0; i<GL_MM_MAX_TASK_POOL_QTY; i++)
    {
        if(GL_NULL == g_a_p_app_sp_pool_table[i])
        {
            break;
        }
    }

    /*
     * If no task memory pool available return failure.
     */
    if(i == GL_MM_MAX_TASK_POOL_QTY)
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
    if((gl_pvoid_t)GL_NULL == (g_a_p_app_sp_pool_table[i] =
                (gl_mm_pool_t *)SYS_MALLOC(sizeof(gl_mm_pool_t) +
                no_of_buf *(sizeof(gl_mm_buffer_hdr_t)+ buf_size) + 
                (no_of_buf + 1) *( sizeof(gl_mm_check_mem_overwrite_t)))))
    {
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        GL_INC_MM_U_G_MALOC_FAIL()
            
        *p_ecode=GL_MM_ERR_MALLOC_FAILED;
        return GL_FAILURE;
    }
    
#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_mm_stat_allocte_usrsp_memory(i, p_ecode))
    {
        GL_INC_MM_U_G_MALOC_FAIL()

        SYS_FREE(g_a_p_app_sp_pool_table[i]);
        g_a_p_app_sp_pool_table[i] = GL_NULL;

        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif /* Endif for GL_STATS_ENABLED */

#ifdef GL_TOTAL_MT_SAFE
    if(GL_SUCCESS != gl_create_mutex (&(g_a_p_app_sp_pool_table[i]->mem_sem), 
       p_ecode))
    {
        GL_INC_MM_U_G_MUTX_FAIL()

        SYS_FREE(g_a_p_app_sp_pool_table[i]);
        g_a_p_app_sp_pool_table[i] = GL_NULL;

        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_MM_CREATE_MUTEX_FAILED;
        return GL_FAILURE;
    }
#endif        
    /*
     * Set pool level parameters.
     */
    g_a_p_app_sp_pool_table[i]->p_hdr_region = (gl_mm_buffer_hdr_t *)
        ((gw_S8bit *)g_a_p_app_sp_pool_table[i] + sizeof(gl_mm_pool_t));
    
    g_a_p_app_sp_pool_table[i]->p_buf_region = (gw_S8bit *)
        (g_a_p_app_sp_pool_table[i]->p_hdr_region + no_of_buf);
    
    /* 
     * As pool-id "0" is reserved.
     */ 
    (*p_pool_id) = (gl_pool_id_t)(i+1);

    g_a_p_app_sp_pool_table[i]->buf_size      = buf_size;
    g_a_p_app_sp_pool_table[i]->buf_qty       = no_of_buf;
    g_a_p_app_sp_pool_table[i]->busy_buf_qty  = 0;
    g_a_p_app_sp_pool_table[i]->owner_task_id = task_id; /* Root task Id */
    g_a_p_app_sp_pool_table[i]->p_first       = GL_NULL;
    g_a_p_app_sp_pool_table[i]->p_last        = GL_NULL;
    g_a_p_app_sp_pool_table[i]->p_next_high_mem_buf_pool = GL_NULL;
    g_a_p_app_sp_pool_table[i]->cngst_mgmt_call_back     = GL_NULL;
    g_a_p_app_sp_pool_table[i]->is_cngst_mgmt_enabled    = GL_FALSE;
    g_a_p_app_sp_pool_table[i]->current_region_index     = 0;
    /*
     * Set Congestion Region Specification Information by "0".
     */
    gl_mm_memset((gw_U8bit*)&(g_a_p_app_sp_pool_table[i]->specs),
        0, sizeof(gl_cngst_mgmt_spec_t));
    
#ifdef GL_MM_ADD_DEBUGGER
    g_a_p_app_sp_pool_table[i]->curr_max_used         = GL_NULL;
    g_a_p_app_sp_pool_table[i]->is_enabled_flag       = GL_MM_NO_ACTIVITY;
    g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg   = GL_NULL;
    g_a_p_app_sp_pool_table[i]->p_start_buf_trace_reg = GL_NULL;
    g_a_p_app_sp_pool_table[i]->p_pool_usage_profile  = GL_NULL;
#endif /* Endif for GL_MM_ADD_DEBUGGER */

    for(j=0;j<no_of_buf;j++)
    {
        /*
         * Store poolId and other information in buffer header
         * at beginning of each buffer.
         */
        p_real_buf = (gl_mm_buffer_hdr_t*)(g_a_p_app_sp_pool_table[i]
                ->p_hdr_region+j);
        p_real_buf->pool_id = i;
        p_real_buf->busy_flag = GL_FALSE;
        p_real_buf->task_id = task_id;
#ifdef GL_MM_ADD_DEBUGGER
        p_real_buf->buffer_alloc_signature = 0;
        p_real_buf->curr_buf_trace_index = 0;
#endif
        p_buf_start = (gl_mm_check_mem_overwrite_t *)
        (g_a_p_app_sp_pool_table[i]->p_buf_region +
         (j * (buf_size + sizeof(gl_mm_check_mem_overwrite_t))));

        p_buf_start->pool_id = i;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
        p_buf_start->start_check_bit = GL_MM_CHECK_BIT;
        p_buf_start->end_check_bit = GL_MM_CHECK_BIT;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

        if(no_of_buf == 1)
        {
            /*
             * First and last buffer in case qty is 1.
             */
            p_real_buf->p_prev=GL_NULL;
            p_real_buf->p_next=GL_NULL;
            g_a_p_app_sp_pool_table[i]->p_first = g_a_p_app_sp_pool_table[i]->
                p_last = p_real_buf;
            break;
        }
        if(j == 0)
        {
            /* 
             * First buffer.
             */
            p_real_buf->p_prev=GL_NULL;
            p_real_buf->p_next=(gl_mm_buffer_hdr_t*)(p_real_buf + 1);
            g_a_p_app_sp_pool_table[i]->p_first=p_real_buf;
            continue;
        }
        else if (j == (no_of_buf-1))
        {
            /*
             * Last buffer.
             */
            p_real_buf->p_next = GL_NULL;
            p_real_buf->p_prev =(gl_mm_buffer_hdr_t*)(p_real_buf - 1);
            g_a_p_app_sp_pool_table[i]->p_last = p_real_buf;
            break;
        }
        else
        {
            /*
             * Other buffers.
             */
            p_real_buf->p_next=(gl_mm_buffer_hdr_t*)(p_real_buf + 1);
            p_real_buf->p_prev=(gl_mm_buffer_hdr_t*)(p_real_buf - 1);
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

    GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

    GL_MM_U_C_BSY_BUFS(i, 0)
    GL_MM_U_C_BUF_QTY(i, no_of_buf)
    
    return GL_SUCCESS;
#endif

}



/*****************************************************************************
* Function name    : gl_mm_get_buffer_from_pool
* Arguments        : gw_U32bit          task_id, 
*                    gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : Allocate memory buffer from specified memory pool.
* Functions called :
*****************************************************************************/
gl_pvoid_t    
gl_mm_get_buffer_from_pool

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_pool_id_t              pool_id, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, p_ecode )
      gw_U32bit                 task_id; 
      gl_pool_id_t              pool_id; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    if(GL_NULL != g_a_p_app_sp_pool_table[(pool_id-1)])
    {
        return(SYS_MALLOC(g_a_p_app_sp_pool_table[(pool_id-1)]->buf_size));
    }
    return GL_NULL;
#else
    
    gl_mm_pool_t                *p_temp=(gl_mm_pool_t*)GL_NULL;
    gl_mm_buffer_hdr_t          *p_alloc=GL_NULL;
    gw_U32bit                   buf_index;
    gl_mm_check_mem_overwrite_t *p_buf_start;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 
    
    /*
     * As at time of pool creation index to array + 1 is returned as pool-id,
     * So actual index to pool-array ispool_id - 1.
     */ 
    pool_id = (gl_pool_id_t)(pool_id - 1);

    GL_INC_MM_U_G_GET_BUF_RQ()

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
         GL_INC_MM_U_G_PARM_VLD_FAIL()
             
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_NULL;
    }
    /*
     * Validate Pool-Id.
     */
    if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
        GL_NULL == g_a_p_app_sp_pool_table[pool_id])
    {
         GL_INC_MM_U_G_PARM_VLD_FAIL()
             
        *p_ecode=GL_MM_ERR_INV_POOL_ID;
        return GL_NULL;
    }

    if(g_a_p_app_sp_pool_table[pool_id]->owner_task_id != task_id)
    {
         GL_INC_MM_U_G_PARM_VLD_FAIL()
             
        *p_ecode=GL_MM_ERR_INV_TASK_ID;
        return GL_NULL;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    p_temp = g_a_p_app_sp_pool_table[pool_id];

    GL_MT_LOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
    /*
     * If buffer available allocate and return same.
     */
    if(p_temp->p_first != (gl_pvoid_t)GL_NULL)
    {
        p_alloc = p_temp->p_first;
        
        GL_INC_MM_U_C_GET_BUF_RQ(pool_id)
            
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
            GL_MT_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
            
            GL_INC_MM_U_G_BUF_FND_CRRPTD()
            GL_INC_MM_U_C_BUF_FND_CRRPTD(pool_id)
            
            *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
            return GL_FAILURE;
        }
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

        p_temp->p_first=p_temp->p_first->p_next;
        if(p_temp->p_first == (gl_pvoid_t)GL_NULL)
        {
            p_temp->p_last=GL_NULL;
        }
        else
        {
            p_temp->p_first->p_prev = GL_NULL;
        }
        p_alloc->busy_flag = GL_TRUE;
        (p_temp->busy_buf_qty)++;
        
        GL_MM_U_C_PEAK_BSY_BUFS(pool_id, p_temp->busy_buf_qty);
        GL_MM_U_C_BSY_BUFS(pool_id, p_temp->busy_buf_qty);
        
#ifdef GL_MM_ADD_DEBUGGER
        p_alloc->buffer_alloc_signature = buffer_alloc_signature;
        /*
         * Updating maximum number of buffers used so far.
         */
        if(p_temp->curr_max_used < p_temp->busy_buf_qty &&
           p_temp->curr_max_used < p_temp->buf_qty)
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
                GL_TRUE, p_temp->owner_task_id, p_alloc);
        }
        /*
         * Updating Pool Usage Information.
         */
        if( GL_MM_ENABLE_POOL_USAGE & p_temp->is_enabled_flag )
        {
            mm_pool_usage_update(p_temp, p_temp->buf_size);
        }
#endif /* Endif for GL_MM_ADD_DEBUGGER */
        /*
         * Call to check current congestion region function.
         */
        if(GL_TRUE == p_temp->is_cngst_mgmt_enabled &&
           p_temp->current_region_index != p_temp->specs.num_regions &&
           p_temp->specs.num_regions > 0)
        {
            if(GL_FAILURE == mm_check_current_cngst_mgmt_region(p_alloc->
                pool_id, (g_a_p_app_sp_pool_table[p_alloc->pool_id]), p_ecode))
            {
                GL_MT_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
                
                return GL_NULL;
            }
        }
        
        GL_MT_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
        
        return ((gw_S8bit*)p_buf_start + 
                 sizeof(gl_mm_check_mem_overwrite_t));
    }
    else
    {
        GL_MT_UNLOCK(&(p_temp->mem_sem), p_ecode, GL_NULL);
        
        GL_INC_MM_U_C_MEMGET_FAIL(pool_id)
        GL_INC_MM_U_G_MEMGET_FAIL()
        /*
         * Else return failure.
         */
        *p_ecode = GL_MM_ERR_BUF_NOT_AVAIL;
        return GL_NULL;
    }
    
#endif
}


/*****************************************************************************
* Function name    : gl_mm_release_buffer_to_pool 
* Arguments        : gw_U32bit          task_id,
*                    gl_pool_id_t       pool_id, 
*                    gl_pvoid_t         buf_ptr, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deallocate memory buffer allocated from 
*                    specified task memory pool.
* Functions called : validate_buffer_pointer
*****************************************************************************/
gl_return_t    
gl_mm_release_buffer_to_pool

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_pool_id_t              pool_id,
      gl_pvoid_t                buf_ptr, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, buf_ptr, p_ecode )
      gw_U32bit                 task_id;
      gl_pool_id_t              pool_id;
      gl_pvoid_t                buf_ptr; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    SYS_FREE(buf_ptr);
    return GL_SUCCESS;
#else
    gl_mm_buffer_hdr_t*         p_hdr;
    gw_U32bit                   buf_index;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 


    /*
     * As at time of pool creation index to array + 1 is returned as pool-id,
     * So actual index to pool-array is pool_id - 1.
     */
    pool_id = (gl_pool_id_t)(pool_id - 1);

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(buf_ptr == GL_NULL)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr -
             sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_U_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
    if( (pool_id != p_buf_start->pool_id) ||
        (pool_id >= GL_MM_MAX_TASK_POOL_QTY) ||
        GL_NULL == g_a_p_app_sp_pool_table[pool_id])
    {
        *p_ecode=GL_MM_ERR_INV_POOL_ID;
        return GL_FAILURE;
    }
    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)buf_ptr + g_a_p_app_sp_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_U_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_task_pool_buffer_pointer
            (pool_id, buf_ptr, &buf_index))
    {
        GL_INC_MM_U_G_REL_INV_PTR()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_app_sp_pool_table[pool_id]->p_hdr_region + buf_index; 
    /*
     * To check, if buffer is already free or not.
     */
    if(GL_FALSE == p_hdr->busy_flag)
    {
        GL_INC_MM_U_G_REL_FREE_BUF()

        *p_ecode= GL_MM_ERR_BUF_FREE;
        return GL_FAILURE; 
    }
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(p_hdr->task_id != task_id || 
        g_a_p_app_sp_pool_table[pool_id]->owner_task_id != task_id)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined
     */
    task_id = task_id; 
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode, GL_NULL);
    /*
     * If no buffer available in memory pool set buffer as
     * only buffer available in pool.
     */
    if(g_a_p_app_sp_pool_table[pool_id]->p_first == GL_NULL)
    {
        g_a_p_app_sp_pool_table[pool_id]->p_first=p_hdr;
        g_a_p_app_sp_pool_table[pool_id]->p_last=p_hdr;
        g_a_p_app_sp_pool_table[pool_id]->p_first->p_prev
            =g_a_p_app_sp_pool_table[pool_id]->p_last->p_next
            =(gl_mm_buffer_hdr_t*)GL_NULL;
    }
    else 
    {
        /*
         * Else add buffer at tail of list of free buffers 
         * available in pool.
         */
        p_hdr->p_prev = g_a_p_app_sp_pool_table[pool_id]->p_last;
        g_a_p_app_sp_pool_table[pool_id]->p_last->p_next = p_hdr;
        g_a_p_app_sp_pool_table[pool_id]->p_last = p_hdr;
        g_a_p_app_sp_pool_table[pool_id]->p_last->p_next = GL_NULL;
    }
    /*
     * Reset busy flag.
     */
    p_hdr->busy_flag = GL_FALSE;
    /*
     * Decrement count of busy buffer.
     */
    (g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty)--;
    
    GL_MM_U_C_BSY_BUFS(pool_id, g_a_p_app_sp_pool_table\
            [pool_id]->busy_buf_qty)

#ifdef GL_MM_ADD_DEBUGGER
    /*
     * Updating Buffer Trace Information.
     */
    if(GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[pool_id]->
            is_enabled_flag)
    {
        mm_buffer_trace_update(g_a_p_app_sp_pool_table[pool_id], buf_index,
            GL_FALSE, g_a_p_app_sp_pool_table[pool_id]->owner_task_id, p_hdr);
    }
#endif /* Endif for GL_MM_ADD_DEBUGGER */
    /*
     * Call to check current congestion region function.
     */
    if(GL_TRUE == g_a_p_app_sp_pool_table[pool_id]->is_cngst_mgmt_enabled &&
       g_a_p_app_sp_pool_table[pool_id]->current_region_index != 0 &&
       g_a_p_app_sp_pool_table[pool_id]->specs.num_regions > 0)
    {
        if(GL_FAILURE == mm_check_current_cngst_mgmt_region(
                 pool_id, (g_a_p_app_sp_pool_table[pool_id]), p_ecode))
        {
            GL_MT_UNLOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
                    GL_NULL);
            
            return GL_FAILURE;
        }
    }

    GL_MT_UNLOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
            GL_NULL);

    return GL_SUCCESS;
#endif
}




/*****************************************************************************
* Function name    : gl_mm_pool_delete
* Arguments        : gw_U32bit          task_id, 
*                    gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Delete task memory pool. Call free on start 
*                    address of memory obtained from system during 
*                    creation of task pool.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_pool_delete

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_pool_id_t              pool_id, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, p_ecode )
      gw_U32bit                 task_id;
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    if(GL_NULL != g_a_p_app_sp_pool_table[(pool_id-1)])
    {
        SYS_FREE(g_a_p_app_sp_pool_table[(pool_id-1)]);
        g_a_p_app_sp_pool_table[(pool_id-1)] = GL_NULL;
    }
    return GL_SUCCESS;
#else

    /*
     * As at time of pool creation index to array + 1 is returned as pool-id,
     * So actual index to pool-array ispool_id - 1.
     */ 
    pool_id = (gl_pool_id_t)(pool_id - 1);

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE; 
    }
    if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
        GL_NULL == g_a_p_app_sp_pool_table[pool_id])
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_POOL_ID;
        return GL_FAILURE;
    }
    if(g_a_p_app_sp_pool_table[pool_id]->owner_task_id != task_id)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * Check for busy buffer quantity to be null.
     */
    if(g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty != GL_NULL)
    {
        GL_INC_MM_U_G_DEL_RQ_POOL_BSY()

        *p_ecode= GL_MM_ERR_BUF_ALLOCATED;
        return GL_FAILURE;
    }
    /*
     * Free pool memory to OS.
     */
    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);

    g_a_p_app_sp_pool_table[pool_id]->p_hdr_region = (gl_mm_buffer_hdr_t *)
        GL_NULL;
    g_a_p_app_sp_pool_table[pool_id]->p_buf_region = (gw_S8bit *)GL_NULL;
    g_a_p_app_sp_pool_table[pool_id]->owner_task_id = GL_INVALID_TASK_ID;
    g_a_p_app_sp_pool_table[pool_id]->current_region_index = 0;
    g_a_p_app_sp_pool_table[pool_id]->is_cngst_mgmt_enabled = GL_FALSE;

#ifdef GL_MM_ADD_DEBUGGER
    g_a_p_app_sp_pool_table[pool_id]->curr_max_used = GL_NULL;
    /*
     * Disable Stack Trace activity.
     */
    if((GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->
        is_enabled_flag) &&
       (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg))
    {
        /*
         * Flush stack trace inforamtion.
         */
        g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
            (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag &
            !(GL_MM_ENABLE_STK_INF));

        SYS_FREE(g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg);
        g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg = GL_NULL;
    }
    /*
     * Disable Buffer Trace activity.
     */
    if((GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[pool_id]->
        is_enabled_flag) &&
       (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg))
    {
        /*
         * Flush buffer trace inforamtion.
         */
        g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
            (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag &
            !(GL_MM_ENABLE_BUF_TRACE));

        SYS_FREE(g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg);
        g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg = GL_NULL;
    }
    /*
     * Disable Pool Usage activity.
     */
    if((GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[pool_id]->
        is_enabled_flag) &&
       (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile))
    {
        /*
         * Flush Poll Usage inforamtion.
         */
        g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
            (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag &
            !(GL_MM_ENABLE_POOL_USAGE));

        SYS_FREE(g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile);
        g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile = GL_NULL;
    }
#endif /* Endif for GL_MM_ADD_DEBUGGER */
    /*
     * Set Congestion region Specification Information by "0".
     */ 
    gl_mm_memset((gw_U8bit*)&(g_a_p_app_sp_pool_table[pool_id]->specs),  
                  0, sizeof(gl_cngst_mgmt_spec_t)); 

    GL_MT_DELETE(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
            GL_FAILURE); 

#ifdef GL_STATS_ENABLED
        gl_mm_stat_free_usrsp_memory(pool_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */ 

    SYS_FREE(g_a_p_app_sp_pool_table[pool_id]);
    g_a_p_app_sp_pool_table[pool_id] = GL_NULL;

    GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_release_all_task_pool_buffers
* Arguments        : gw_U32bit          task_id, 
*                    gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Release all buffers of task pool. status of pool is 
*                    similar to one after create pool.  
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_release_all_task_pool_buffers

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_pool_id_t              pool_id, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, p_ecode )
      gw_U32bit                 task_id;
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_buffer_hdr_t          *p_real_buf;
    gw_U32bit                   no_of_buf;
    gw_U32bit                   j;
#ifdef GL_MM_ADD_MEM_OVERW_OPR        
    gl_mm_check_mem_overwrite_t *p_buf_start;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

    /*
     * As at time of pool creation index to array + 1 is returned as pool-id,
     * So actual index to pool-array ispool_id - 1.
     */ 
    pool_id = (gl_pool_id_t)(pool_id - 1);
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(pool_id > GL_MM_MAX_TASK_POOL_QTY ||
        GL_NULL == g_a_p_app_sp_pool_table[pool_id])
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode=GL_MM_ERR_INV_POOL_ID;
        return GL_FAILURE;
    }
    if( g_a_p_app_sp_pool_table[pool_id]->owner_task_id != task_id)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if flag not defined.
     */
    task_id = task_id;
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MT_LOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
            GL_FAILURE); 
        
    no_of_buf = g_a_p_app_sp_pool_table[pool_id]->buf_qty;
    g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty = 0;
    g_a_p_app_sp_pool_table[pool_id]->current_region_index = 0;
   
    GL_MM_U_C_BSY_BUFS(pool_id, 0)
    /*
     * As all buffers are free now, create free list of buffers of pool.
     */
    for(j=0;j<no_of_buf;j++)
    {
#ifdef GL_MM_ADD_MEM_OVERW_OPR
        p_buf_start = (gl_mm_check_mem_overwrite_t *)
            (g_a_p_app_sp_pool_table[pool_id]->p_buf_region +
            (j * (g_a_p_app_sp_pool_table[pool_id]->buf_size + 
            sizeof(gl_mm_check_mem_overwrite_t))));
        
        if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
           (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
        {
            GL_MT_LOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
                GL_FAILURE); 

            GL_INC_MM_U_G_BUF_FND_CRRPTD()
            GL_INC_MM_U_C_BUF_FND_CRRPTD(pool_id)
            
            *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
            return GL_FAILURE;
        }
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */ 

        p_real_buf = (gl_mm_buffer_hdr_t*)
            (g_a_p_app_sp_pool_table[pool_id]->p_hdr_region +j);
        
#ifdef GL_MM_ADD_DEBUGGER
        /*
         * Updating Buffer Trace Information.
         */
        if((GL_TRUE ==  p_real_buf->busy_flag) && (GL_MM_ENABLE_BUF_TRACE 
            & g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag))
        {
            mm_buffer_trace_update(g_a_p_app_sp_pool_table[pool_id], j,
                GL_FALSE, g_a_p_app_sp_pool_table[pool_id]->owner_task_id,
                p_real_buf);
        }
        p_real_buf->buffer_alloc_signature = 0;
#endif /* Endif for GL_MM_ADD_DEBUGGER */
        p_real_buf->busy_flag = GL_FALSE;
        
        if(no_of_buf == 1)
        {
            p_real_buf->p_prev=GL_NULL;
            p_real_buf->p_next=GL_NULL;
            g_a_p_app_sp_pool_table[pool_id]->p_first = 
            g_a_p_app_sp_pool_table[pool_id]->p_last=p_real_buf;
            break;
        }
        if(j == 0)
        {
            p_real_buf->p_prev = GL_NULL;
            p_real_buf->p_next = (gl_mm_buffer_hdr_t*)(p_real_buf + 1);
            g_a_p_app_sp_pool_table[pool_id]->p_first = p_real_buf;
            continue;
        }   
        else if(j == (no_of_buf - 1))
        {
            /*
             * Set last buffer in pool.
             */
            p_real_buf->p_next = GL_NULL;
            p_real_buf->p_prev = (gl_mm_buffer_hdr_t*)(p_real_buf - 1 );
            g_a_p_app_sp_pool_table[pool_id]->p_last = p_real_buf;
            break;
        }
        else
        {
            p_real_buf->p_next=(gl_mm_buffer_hdr_t *)(p_real_buf + 1);
            p_real_buf->p_prev=(gl_mm_buffer_hdr_t *)(p_real_buf - 1);
            continue;
        }
    }

    GL_MT_LOCK(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode,
                GL_FAILURE); 
    
    return GL_SUCCESS;
#endif
}

