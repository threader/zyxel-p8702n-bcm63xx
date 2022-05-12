/*****************************************************************************  
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_init_deint_api_funcs.c
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
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/
  
#include "gl_mm.h"



/*****************************************************************************
* Function name    : gl_mm_init
* Arguments        : void
* Return Value     : void
* Functionality    : Initializes generic libraries memory manager.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_mm_init( void )
    
/****************************************************************************/
{
    gw_return_t                 ret_val = GL_FAILURE; 
    gl_error_t                  ecode;
    gw_U32bit                   i;

#ifdef GL_MT_ENABLED
    if((init_flag_cmn_pools == GL_FALSE) && 
            (init_flag_task_pools == GL_FALSE))
    {
        /*
         * Creating semaphores.
         */
        if(GL_FAILURE == gl_create_mutex (&mm_mgr_sem, &ecode))
        {
            GL_INC_MM_B_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#ifdef GL_MM_ADD_AUDIT_MGT
        if(GL_FAILURE == gl_create_mutex (&mm_audit_sem, &ecode))
        {
            GL_INC_MM_B_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#endif

#ifdef GL_STATS_ENABLED
        g_mm_stat_init = GL_TRUE;

        if(GL_FAILURE == gl_create_mutex (&g_mm_bfit_stat_sem, &ecode))
        {
            GL_INC_MM_B_G_MUTX_FAIL()

            return GL_FAILURE;
        }

        if(GL_FAILURE == gl_create_mutex (&g_mm_usrsp_stat_sem, &ecode))
        {
            GL_INC_MM_B_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#endif

    }
#else
    ecode = 0;
#endif
    if(init_flag_cmn_pools == GL_FALSE)
    {
        /*
         * Initialize common memory pools.
         */
        for(i=0; i<GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            g_a_p_glb_pool_table[i] = GL_NULL;
        }
        p_gl_smallest_mem_buf_pool = GL_NULL;
        init_flag_cmn_pools = GL_TRUE;
        ret_val = GL_SUCCESS;
    }
    if(init_flag_task_pools == GL_FALSE)
    {
        /*
         * Initialize task memory pools.
         */
        for(i=0; i<GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            g_a_p_app_sp_pool_table[i] = GL_NULL;

            a_pool_info[i].task_id = GL_INVALID_TASK_ID;
            a_pool_info[i].g_p_smallest_task_cmn_pool = GL_NULL;

            g_a_p_pool_info[ i ] = GL_NULL; 
        }

        init_flag_task_pools = GL_TRUE;
        ret_val =GL_SUCCESS;
    }
    return(ret_val);
}

    



/*****************************************************************************
* Function name    : gl_mm_destroy_all_common_pools
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Free memory of all common pools and free all 
*                    corresponding occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_destroy_all_common_pools

#ifdef _ANSI_C_
    ( gw_U8bit                  is_forceful_cleanup,
      gl_error_t                *p_ecode
    )
#else
    ( is_forceful_cleanup, p_ecode )
      gw_U8bit                  is_forceful_cleanup;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return(GL_SUCCESS);
#else        
    gw_U16bit               pool_id= GL_NULL;
    gw_U8bit                is_busy_buf_flag = GL_FALSE;

    if(init_flag_cmn_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }

    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
    for(pool_id = 0; pool_id < GL_MM_MAX_COMMON_POOL_QTY; pool_id++)
    {
        if( GL_NULL == g_a_p_glb_pool_table[pool_id])
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE ) &&
           (g_a_p_glb_pool_table[pool_id]->busy_buf_qty != GL_NULL))
        {
            is_busy_buf_flag = GL_TRUE;
            continue;
        }
        g_a_p_glb_pool_table[pool_id]->p_hdr_region = (gl_mm_buffer_hdr_t *)GL_NULL;
        g_a_p_glb_pool_table[pool_id]->p_buf_region = (gw_S8bit *)GL_NULL;
        
#ifdef GL_MM_ADD_DEBUGGER
        g_a_p_glb_pool_table[pool_id]->curr_max_used = GL_NULL;
        /*
         * Disable Stack Trace activity.
         */
        if((GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg))
        {
            /*
             * Flush stack trace inforamtion.
             */
            g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
                (g_a_p_glb_pool_table[pool_id]->is_enabled_flag &
                !(GL_MM_ENABLE_STK_INF));

            SYS_FREE(g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg);
            g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg = GL_NULL;
        }
        /*
         * Disable Buffer Trace activity.
         */
        if((GL_MM_ENABLE_BUF_TRACE & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg))
        {
            /*
             * Flush buffer trace inforamtion.
             */
            g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
                (g_a_p_glb_pool_table[pool_id]->is_enabled_flag &
                !(GL_MM_ENABLE_BUF_TRACE));

            SYS_FREE(g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg);
            g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg = GL_NULL;
        }
        /*
         * Disable Pool Usage activity.
         */
        if((GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile))
        {
            /*
             * Flush Poll Usage inforamtion.
             */
            g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
                (g_a_p_glb_pool_table[pool_id]->is_enabled_flag &
                !(GL_MM_ENABLE_POOL_USAGE));

            SYS_FREE(g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile);
            g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile = GL_NULL;
        }
#endif

        GL_MUTEX_DELETE(&(g_a_p_glb_pool_table[pool_id]->mem_sem), p_ecode, GL_FAILURE);

#ifdef GL_STATS_ENABLED
        gl_mm_stat_free_bfit_memory(pool_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        SYS_FREE(g_a_p_glb_pool_table[pool_id]);
        g_a_p_glb_pool_table[pool_id] = GL_NULL;
    
    }
        
    if( is_busy_buf_flag == GL_TRUE)
    {
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        /* 
         * In some pool buffer are busy.
         */
         *p_ecode = GL_MM_ERR_BUF_ALLOCATED;
         return GL_FAILURE;
    }
    else
    {
#ifdef GL_MT_ENABLED
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
        if(GL_FALSE == init_flag_task_pools)
        {
            GL_MUTEX_DELETE(&mm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
            g_mm_stat_init = GL_FALSE;
#endif /* Endif for GL_STATS_ENABLED */
        }
#ifdef GL_STATS_ENABLED
        if(GL_TRUE == g_mm_stat_init)
        {
            gl_delete_mutex(&g_mm_bfit_stat_sem, p_ecode);
        }
#endif /* Endif for GL_STATS_ENABLED */

#endif
        init_flag_cmn_pools = GL_FALSE;
        return GL_SUCCESS;
    }
#endif
}
        




/*****************************************************************************
* Function name    : gl_mm_destroy_all_task_pools
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Free memory of all common pools and free all 
*                    corresponding occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_destroy_all_task_pools

#ifdef _ANSI_C_
    ( gw_U8bit                  is_forceful_cleanup,
      gl_error_t                *p_ecode
    )
#else
    ( is_forceful_cleanup, p_ecode )
      gw_U8bit                  is_forceful_cleanup;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
        
#ifdef GL_DEBUG
    return(GL_SUCCESS);
#else
    gw_U16bit               pool_id= GL_NULL;
    gw_boolean              is_busy_buf_flag = GL_FALSE;
    
    if(init_flag_task_pools == GL_FALSE)
    {
         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }

    GL_MUTEX_LOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
    for(pool_id = 0; pool_id < GL_MM_MAX_TASK_POOL_QTY; pool_id++)
    {
        if(GL_NULL ==  g_a_p_app_sp_pool_table[pool_id])
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE ) &&
           (g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty != GL_NULL))
        {
            is_busy_buf_flag = GL_TRUE;
            continue;
        }
        g_a_p_app_sp_pool_table[pool_id]->p_hdr_region = (gl_mm_buffer_hdr_t *)GL_NULL;
        g_a_p_app_sp_pool_table[pool_id]->p_buf_region = (gw_S8bit *)GL_NULL;
#ifdef GL_MM_ADD_DEBUGGER
        g_a_p_app_sp_pool_table[pool_id]->curr_max_used = GL_NULL;
        /*
         * Disable Stack Trace activity.
         */
        if((GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg))
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
        if((GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg))
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
        if((GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag)
           && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile))
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
#endif

#ifdef GL_STATS_ENABLED
        gl_mm_stat_free_usrsp_memory(pool_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_app_sp_pool_table[pool_id]->mem_sem), p_ecode, GL_FAILURE);

        SYS_FREE(g_a_p_app_sp_pool_table[pool_id]);
        g_a_p_app_sp_pool_table[pool_id] = GL_NULL; 

    }
        
    if( is_busy_buf_flag == GL_TRUE)
    {
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
    
        /* 
         * In some pool buffer are busy.
         */
        *p_ecode = GL_MM_ERR_BUF_ALLOCATED;
        return GL_FAILURE;
    }
    else
    {
        free_entry_index = 0;
    
        GL_MUTEX_UNLOCK(&mm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_MT_ENABLED
        if(GL_FALSE == init_flag_cmn_pools)
        {
            GL_MUTEX_DELETE(&mm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
            g_mm_stat_init = GL_FALSE;
#endif /* Endif for GL_STATS_ENABLED */
        }
#ifdef GL_STATS_ENABLED
        if(GL_TRUE == g_mm_stat_init)
        {
            gl_delete_mutex(&g_mm_usrsp_stat_sem, p_ecode);
        }
#endif /* Endif for GL_STATS_ENABLED */

#endif
        init_flag_task_pools = GL_FALSE;    
        return GL_SUCCESS;
    }
#endif
}

