/*****************************************************************************  
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_usage_mgmt_n_debug_ctrl.c
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
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/
  
#include "gl_mm.h"


#ifdef GL_MM_ADD_DEBUGGER


/***************************************************************************** 
* Function name    : gl_mm_set_cmn_pool_stack_trace
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Stack Tracing activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cmn_pool_stack_trace

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_boolean_t              is_enabled,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, is_enabled, p_ecode )
      gl_pool_id_t              pool_id;
      gl_boolean_t              is_enabled;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Stack Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
                if((g_a_p_glb_pool_table[i] != GL_NULL) && 
                   (GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[i]->
                    is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[i]->
                    p_start_stk_inf_reg))    
                {
                    /*
                     * Flush stack trace inforamtion.
                     */ 
                    g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                        (g_a_p_glb_pool_table[i]->is_enabled_flag & 
                         !(GL_MM_ENABLE_STK_INF));
                    
                    SYS_FREE(g_a_p_glb_pool_table[i]->p_start_stk_inf_reg);
                    g_a_p_glb_pool_table[i]->p_start_stk_inf_reg = GL_NULL;
                }
            }
        }
        else
        {
            /*
             * Enable Stack Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
                if((g_a_p_glb_pool_table[i] == GL_NULL) 
                    ||(GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[i]->
                    is_enabled_flag))
                {
                    /*
                     * Pool is not created for pool-id = i or
                     * Stack Traceing is already set for pool.
                     */ 
                    continue;
                }
                g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_glb_pool_table[i]->is_enabled_flag | 
                   GL_MM_ENABLE_STK_INF);
                
                g_a_p_glb_pool_table[i]->p_start_stk_inf_reg = 
                    (gl_mm_stk_inf_t *)SYS_MALLOC(g_a_p_glb_pool_table[i]->
                    buf_qty * sizeof(gl_mm_stk_inf_t));
                
                if( GL_NULL == g_a_p_glb_pool_table[i]->p_start_stk_inf_reg)
                {
                    GL_INC_MM_B_G_MALOC_FAIL()

                    *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                    return GL_FAILURE;
                }
                g_a_p_glb_pool_table[i]->p_start_stk_inf_reg->num_of_entries =
                    GL_NULL;
                
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()
            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Stack Trace activity.
             */ 
            if((GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[pool_id]->
                p_start_stk_inf_reg))    
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
        }
        else
        {
            /*
             * Enable Stack Trace activity.
             */ 
            if(GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag)
            {
                /*
                 * Stack Traceing is already set for pool.
                 */ 
                return GL_SUCCESS;
            }
            g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
              (g_a_p_glb_pool_table[pool_id]->is_enabled_flag | 
               GL_MM_ENABLE_STK_INF);
                
            g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg =
                (gl_mm_stk_inf_t *)SYS_MALLOC(g_a_p_glb_pool_table[pool_id]->
                buf_qty * sizeof(gl_mm_stk_inf_t));

            if( GL_NULL == g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg)
            {
                GL_INC_MM_B_G_MALOC_FAIL()

                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
           g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg->num_of_entries =
               GL_NULL;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_set_cmn_pool_buffer_trace
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_U32bit          number_of_requests,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Buffer Tracing activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cmn_pool_buffer_trace

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gw_U32bit                 number_of_requests,
      gl_boolean_t              is_enabled,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, number_of_requests, is_enabled, p_ecode )
      gl_pool_id_t              pool_id;
      gw_U32bit                 number_of_requests;
      gl_boolean_t              is_enabled;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if( GL_NULL == number_of_requests)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_NUM_OF_REQUESTS;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    if(pool_id == 0)
    {
        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Buffer Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
                if((g_a_p_glb_pool_table[i] != GL_NULL) && 
                   (GL_MM_ENABLE_BUF_TRACE & g_a_p_glb_pool_table[i]->
                    is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[i]->
                    p_start_buf_trace_reg))    
                {
                    /*
                     * Flush buffer trace inforamtion.
                     */ 
                    g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                        (g_a_p_glb_pool_table[i]->is_enabled_flag & 
                         !(GL_MM_ENABLE_BUF_TRACE));
                    
                    SYS_FREE(g_a_p_glb_pool_table[i]->p_start_buf_trace_reg);
                    g_a_p_glb_pool_table[i]->p_start_buf_trace_reg = GL_NULL;
                }
            }
        }
        else
        {
            /*
             * Enable Buffer Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
               if((g_a_p_glb_pool_table[i] == GL_NULL) 
                    ||(GL_MM_ENABLE_BUF_TRACE & g_a_p_glb_pool_table[i]->
                    is_enabled_flag))
                {
                    /*
                     * Pool is not created for pool-id = i or
                     * Buffer Traceing is already set for pool.
                     */ 
                    continue;
                }
                g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_glb_pool_table[i]->is_enabled_flag | 
                   GL_MM_ENABLE_BUF_TRACE);
                
                g_a_p_glb_pool_table[i]->p_start_buf_trace_reg = 
                    (gl_mm_buf_trace_t *)SYS_MALLOC(g_a_p_glb_pool_table[i]->
                     buf_qty *(sizeof(gl_mm_buf_trace_t) * number_of_requests));
                
                if( GL_NULL == g_a_p_glb_pool_table[i]->p_start_buf_trace_reg)
                {
                    GL_INC_MM_B_G_MALOC_FAIL()

                    *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                    return GL_FAILURE;
                }
                g_a_p_glb_pool_table[i]->num_of_buf_trc_reqs = 
                    number_of_requests;
                
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Buffer Trace activity.
             */ 
           if((GL_MM_ENABLE_BUF_TRACE & g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[pool_id]->
                p_start_buf_trace_reg)) 
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
        }
        else
        {
            /*
             * Enable Buffer Trace activity.
             */ 
            if(GL_MM_ENABLE_BUF_TRACE &g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag)
            {
                /*
                 * Buffer Traceing is already set for pool.
                 */ 
                return GL_SUCCESS;
            }
            g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
            (g_a_p_glb_pool_table[pool_id]->is_enabled_flag |
             GL_MM_ENABLE_BUF_TRACE);
                
            g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg = 
                (gl_mm_buf_trace_t*)SYS_MALLOC(g_a_p_glb_pool_table[pool_id]->
                buf_qty * (sizeof(gl_mm_buf_trace_t) * number_of_requests));
            
            if( GL_NULL == g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg)
            {
                GL_INC_MM_B_G_MALOC_FAIL()

                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
            g_a_p_glb_pool_table[pool_id]->num_of_buf_trc_reqs = 
                number_of_requests;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_enable_cmn_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_buffer_usage_profile_t *p_usage_range_spec,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable Pool Memory Usage Profiling activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_enable_cmn_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t               pool_id,
      gl_mm_pool_usage_profile_t *p_usage_range_spec,
      gl_error_t                 *p_ecode
    )
#else
    ( pool_id, p_usage_range_spec, p_ecode )
      gl_pool_id_t               pool_id;
      gl_mm_pool_usage_profile_t *p_usage_range_spec;
      gl_error_t                 *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if( GL_NULL == p_usage_range_spec)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
        return GL_FAILURE;
    }
    if( GL_MM_POOL_USAGE_MAX_REGIONS <=  p_usage_range_spec->num_regions )
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
        return GL_FAILURE;
    }
    for(j=0; j < p_usage_range_spec->num_regions; j++)
    { 
       if(100 < p_usage_range_spec->usage_region_def[j] ) 
       {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
            return GL_FAILURE;
       }
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        /*
         * Enable Pool Usage activity.
         */ 
        for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            if((g_a_p_glb_pool_table[i] == GL_NULL) 
              ||(GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[i]->
                  is_enabled_flag))
            {
                /*
                 * Pool is not created for pool-id = i or
                 * Pool Usage is already set for pool.
                 */ 
                continue;
            }
            g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                (g_a_p_glb_pool_table[i]->is_enabled_flag |
                 GL_MM_ENABLE_POOL_USAGE);
                
            g_a_p_glb_pool_table[i]->p_pool_usage_profile = 
                (gl_mm_pool_usage_profile_t *)
                SYS_MALLOC(sizeof(gl_mm_pool_usage_profile_t));
                
            if( GL_NULL == g_a_p_glb_pool_table[i]->p_pool_usage_profile)
            {
                GL_INC_MM_B_G_MALOC_FAIL()

                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
            
            g_a_p_glb_pool_table[i]->p_pool_usage_profile->num_regions = 
                p_usage_range_spec->num_regions;
            
            for(j=0; j < p_usage_range_spec->num_regions; j++)
            {
                g_a_p_glb_pool_table[i]->p_pool_usage_profile->usage_region_def
                    [j] = p_usage_range_spec->usage_region_def[j];
                g_a_p_glb_pool_table[i]->p_pool_usage_profile->
                    usage_region_count[j] = GL_NULL;
            }
                
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Enable Pool Usage activity.
         */ 
        if(GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag)
        {
            /*
             * Pool Uasage Activity is already set for pool.
             */ 
            return GL_SUCCESS;
        }
        g_a_p_glb_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
          (g_a_p_glb_pool_table[pool_id]->is_enabled_flag | 
           GL_MM_ENABLE_POOL_USAGE);
                
        g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile = 
            (gl_mm_pool_usage_profile_t *)
            SYS_MALLOC( sizeof(gl_mm_pool_usage_profile_t) );
            
        if( GL_NULL == g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile)
        {
            GL_INC_MM_B_G_MALOC_FAIL()

            *p_ecode = GL_MM_ERR_MALLOC_FAILED;
            return GL_FAILURE;
        }
        
        g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->num_regions = 
        p_usage_range_spec->num_regions;
            
        for(j=0; j < p_usage_range_spec->num_regions; j++)
        {
            g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->
                usage_region_def[j] = p_usage_range_spec->usage_region_def[j];    
            g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->
                usage_region_count[j] = GL_NULL;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_disable_cmn_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Disable Pool Memory Usage Profiling activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_disable_cmn_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        /*
         * Disable Buffer Trace activity.
         */ 
        for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            if((g_a_p_glb_pool_table[i] != GL_NULL) && 
               (GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[i]->
                is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[i]->
                    p_pool_usage_profile))    
            {
                /*
                 * Flush Pool Usage inforamtion.
                 */ 
                g_a_p_glb_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_glb_pool_table[i]->is_enabled_flag & 
                  !(GL_MM_ENABLE_POOL_USAGE));
                    
                SYS_FREE(g_a_p_glb_pool_table[i]->p_pool_usage_profile);
                g_a_p_glb_pool_table[i]->p_pool_usage_profile = GL_NULL;
            }
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Disable Pool Usage activity.
         */ 
        if((GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[pool_id]->
                is_enabled_flag) && (GL_NULL != g_a_p_glb_pool_table[pool_id]->
                p_pool_usage_profile))   
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
            
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_get_cmn_pool_stack_trace
* Arguments        : gl_pvoid_t         p_buf_ptr,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Return Stack Trace activity Trace.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_get_cmn_pool_stack_trace

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( p_buf_ptr, p_ecode )
      gl_pvoid_t                p_buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U8bit                    i;
    gw_U32bit                   buf_index;
    gl_mm_buffer_hdr_t          *p_hdr;
    gl_mm_stk_inf_t             *p_stk_inf;
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_buf_ptr == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }

    pool_id = p_buf_start->pool_id;

    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr + g_a_p_glb_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)p_buf_ptr -
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;

#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */
    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_common_pool_buffer_pointer
        (pool_id, p_buf_ptr, &buf_index))
    {
        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_glb_pool_table[pool_id]->p_hdr_region + buf_index;
    /*
     * To check, if buffer is already free or not.
     */ 
    if(GL_FALSE == p_hdr->busy_flag)
    {
        *p_ecode = GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    
    if((GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
       && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg))    
    {
        /*
         * Dump stack trace inforamtion.
         */ 
       p_stk_inf = g_a_p_glb_pool_table[pool_id]->p_start_stk_inf_reg+buf_index;

        GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
        GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
        GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
       
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n Stack Trace Info for Buffer Pointer of Common Pool."));
            
        GL_MM_DEBUG_INFO_TRACE(("\n Buffer Pointer = %p ",p_buf_ptr));
        GL_MM_DEBUG_INFO_TRACE(("\n Number of Function Entries = %d",
                    p_stk_inf->num_of_entries));

        for(i=0; i < p_stk_inf->num_of_entries; i++)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t %d. Function Address Range Pointer = %p",
                i+1,p_stk_inf->stk_func_add[i]));
        }
        GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    }
    return GL_SUCCESS;
#endif
}


/*****************************************************************************
* Function name    : gl_mm_get_cmn_pool_buffer_trace
* Arguments        : gl_pvoid_t         p_buf_ptr,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Return Buffer Trace activity Trace.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_get_cmn_pool_buffer_trace

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( p_buf_ptr, p_ecode )
      gl_pvoid_t                p_buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U32bit                   i;
    gw_U32bit                   buf_index;
    gl_mm_buffer_hdr_t          *p_hdr = GL_NULL;
    gl_mm_buf_trace_t           *p_buf_trace = GL_NULL; 
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_buf_ptr == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }

    pool_id = p_buf_start->pool_id;

    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr + g_a_p_glb_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        GL_INC_MM_B_G_BUF_FND_CRRPTD()

        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)p_buf_ptr -
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;

#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_common_pool_buffer_pointer
        (pool_id, p_buf_ptr, &buf_index))
    {
        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_glb_pool_table[pool_id]->p_hdr_region + buf_index;

    if((GL_MM_ENABLE_BUF_TRACE & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
       && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg))    
    {
        /*
         * Dump stack trace inforamtion.
         */ 
        p_buf_trace = g_a_p_glb_pool_table[pool_id]->p_start_buf_trace_reg + 
            (buf_index * g_a_p_glb_pool_table[pool_id]->num_of_buf_trc_reqs) ;
       
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
        GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
        GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n Buffer Trace Info for Buffer Pointer of Common Pool."));
            
        GL_MM_DEBUG_INFO_TRACE(("\n Buffer Pointer = %p ",p_buf_ptr));
        GL_MM_DEBUG_INFO_TRACE(("\n Pool-id of Buffer Pointer = %u",
                    p_hdr->pool_id));
        GL_MM_DEBUG_INFO_TRACE(("\n Number of Buffer Trace Requests = %u",
                     g_a_p_glb_pool_table[pool_id]->num_of_buf_trc_reqs));

        for(i=0; i < g_a_p_glb_pool_table[pool_id]->num_of_buf_trc_reqs; i++)
        {
           p_buf_trace = p_buf_trace + i;
           GL_MM_DEBUG_INFO_TRACE(("\n\n\t %d. Buffer Trace Information", 
                       i+1));
           GL_MM_DEBUG_INFO_TRACE(("\n\t\t Buffer Status = %s",
                       p_buf_trace->buf_status?"BUSY":"FREE"));
           GL_MM_DEBUG_INFO_TRACE((
                "\n\t\t Task-id Used at Buffer Allocation = %u",
                p_buf_trace->task_id));
           GL_MM_DEBUG_INFO_TRACE((
                "\n\t\tThe Buffer Allocation Signature = %d",
                p_buf_trace->buffer_alloc_signature));
           
        }
        GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_report_cmn_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Report Pool Memory Usage Profile activity Trace. 
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_report_cmn_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
    GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
    GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));

    if(pool_id == 0)
    {
        /*
         * Disable Buffer Trace activity.
         */ 
        for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            if((g_a_p_glb_pool_table[i] != GL_NULL) && 
            (GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[i]->is_enabled_flag) 
             && (GL_NULL != g_a_p_glb_pool_table[i]->p_pool_usage_profile) &&
             g_a_p_glb_pool_table[i]->busy_buf_qty != GL_NULL)    
            {
                /*
                 * Dump Pool Usage inforamtion.
                 */ 
                GL_MM_DEBUG_INFO_TRACE((
              "\n\n Memory Usage Information for Common Pool, Pool-id = %d ",
                 i+1));
                GL_MM_DEBUG_INFO_TRACE((
                  "\n Buffer Size = %u \t Number of Buffers = %u",
                  g_a_p_glb_pool_table[i]->buf_size, g_a_p_glb_pool_table[i]->
                  buf_qty));     
                GL_MM_DEBUG_INFO_TRACE((
                "\n Maximum Buffers in Busy State at any time = %u\n",
                g_a_p_glb_pool_table[i]->curr_max_used));
                GL_MM_DEBUG_INFO_TRACE(("\n Number of Usage Regions = %d ",
                  g_a_p_glb_pool_table[i]->p_pool_usage_profile->num_regions));
                for(j=0; j < g_a_p_glb_pool_table[i]->p_pool_usage_profile->
                        num_regions; j++ )
                {
                    GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Number of Allocations in %d %% Usage Region = %d",
                    g_a_p_glb_pool_table[i]->p_pool_usage_profile->
                    usage_region_def[j], g_a_p_glb_pool_table[i]->
                    p_pool_usage_profile->usage_region_count[j]));
                }
                
            }
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Disable Pool Usage activity.
         */ 
            if((g_a_p_glb_pool_table[pool_id] != GL_NULL) && 
            (GL_MM_ENABLE_POOL_USAGE & g_a_p_glb_pool_table[pool_id]->is_enabled_flag) 
             && (GL_NULL != g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile) &&
             g_a_p_glb_pool_table[pool_id]->busy_buf_qty != GL_NULL)    
        {
            /*
             * Dump Pool Usage inforamtion.
             */ 
            GL_MM_DEBUG_INFO_TRACE((
              "\n\n Memory Usage Information for Common Pool, Pool-id = %d ",
                pool_id+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Buffer Size = %u \t Number of Buffers = %u",
                g_a_p_glb_pool_table[pool_id]->buf_size,
                g_a_p_glb_pool_table[pool_id]->buf_qty));     
            GL_MM_DEBUG_INFO_TRACE((
                "\n Maximum Buffers in Busy State at any time = %u\n",
                g_a_p_glb_pool_table[pool_id]->curr_max_used));
            GL_MM_DEBUG_INFO_TRACE(("\n Number of Usage Regions = %d ",
                g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->
                num_regions));
            for(j=0; j < g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->
                    num_regions; j++ )
            {
              GL_MM_DEBUG_INFO_TRACE((
                "\n Number of Allocations in %d %% Usage Region = %d",
              g_a_p_glb_pool_table[pool_id]->p_pool_usage_profile->
              usage_region_def[j], g_a_p_glb_pool_table[pool_id]->
              p_pool_usage_profile->usage_region_count[j]));
            }
        }
            
    }
    GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    return GL_SUCCESS;
#endif
}





/*****************************************************************************
* Function name    : gl_mm_report_cmn_pool_busy_buffers_statistics
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_U32bit          task_id,
*                    gw_U8bit           buffer_alloc_sig,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Report Busy Buffers Statistics of Common Pool. 
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_report_cmn_pool_busy_buffers_statistics

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gw_U32bit                 task_id,
      gw_U8bit                  buffer_alloc_sig,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, task_id, buffer_alloc_sig, p_ecode )
      gl_pool_id_t              pool_id;
      gw_U32bit                 task_id;
      gw_U8bit                  buffer_alloc_sig;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;
    gw_U8bit                    k;
    gl_mm_buffer_hdr_t          *p_buf_hdr;
    gw_U32bit                   count = 0;
    gw_U32bit                   total_count = 0;
    gw_U32bit                   is_stk_trace_enabled = GL_FALSE;
    gl_mm_stk_inf_t             *p_stk_inf;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
    GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
    GL_MM_DEBUG_INFO_TRACE((
        "\n\n\t\t Allocated Common Pool Buffers Informantion"));
    GL_MM_DEBUG_INFO_TRACE((
       "\n************************************************************"));
    
    if(pool_id == 0)
    {
        /*
         * Get Busy Buffer Reoprt for all Pools.
         */ 
        for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            if(GL_NULL == g_a_p_glb_pool_table[i] || (GL_NULL != g_a_p_glb_pool_table[i]
                    && g_a_p_glb_pool_table[i]->busy_buf_qty == GL_NULL))
            {
                continue;
            }
            GL_MM_DEBUG_INFO_TRACE((
                "\n Active Common Pool-Id = %d",i+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Number of Buffers = %u, Size of Buffers = %u",
                g_a_p_glb_pool_table[i]->buf_qty, g_a_p_glb_pool_table[i]->
                buf_size));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Total Number of Buffers in Busy State = %u",
                g_a_p_glb_pool_table[i]->busy_buf_qty));
            
            if(GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[i]->is_enabled_flag)
            {
                is_stk_trace_enabled = GL_TRUE;
            }
            else
            {
              GL_MM_DEBUG_INFO_TRACE(("\n Stack Tracing is not Set for Pool"));
            }
            count = 0;
            
            for(j=0; j<g_a_p_glb_pool_table[i]->buf_qty; j++)
            {
                p_buf_hdr = (gl_mm_buffer_hdr_t *)
                    (g_a_p_glb_pool_table[i]->p_hdr_region + j);
                
                if((GL_TRUE == p_buf_hdr->busy_flag) && 
                   ((GL_INVALID_TASK_ID == task_id)|| 
                    (p_buf_hdr->task_id == task_id) ) &&
                   ((0 == buffer_alloc_sig) || 
                    (p_buf_hdr->buffer_alloc_signature == buffer_alloc_sig)))     
                {
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n %u. Buffer pointer Details:",++count));
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Buffer pointer = %p",
                        (g_a_p_glb_pool_table[i]->p_buf_region +
                         (j*(g_a_p_glb_pool_table[i]->
                         buf_size + sizeof(gl_mm_check_mem_overwrite_t)))
                         + sizeof(gl_mm_check_mem_overwrite_t))));
                    
                    GL_MM_DEBUG_INFO_TRACE((
                     "\n\t Task-id Used at Buffer Allocation = %u",
                     p_buf_hdr->task_id));
                    GL_MM_DEBUG_INFO_TRACE((
                      "\n\t Buffer Allocation Signature = %d",
                      p_buf_hdr->buffer_alloc_signature));
                    
                    if(GL_TRUE == is_stk_trace_enabled)
                    {
                        /*
                         * Dump Stack Trace inforamtion.
                         */ 
                        p_stk_inf = g_a_p_glb_pool_table[i]->
                            p_start_stk_inf_reg+j;
       
                        GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Stack Trace Information for Buffer Pointer.\n"));
                        GL_MM_DEBUG_INFO_TRACE((
                            "\n\t Number of Function Entries = %d ", 
                            p_stk_inf->num_of_entries));

                        for(k=0; k < p_stk_inf->num_of_entries; k++)
                        {
                            GL_MM_DEBUG_INFO_TRACE((
                             "\n\t\t %d. Function Address Range Pointer = %p",
                             k+1,p_stk_inf->stk_func_add[k]));
                        }
                    }/* End of stack trace info */
                }/* Allocated buffer info */
            }/* End for all budffers of a common pool */
            if( count == 0)
            {
                GL_MM_DEBUG_INFO_TRACE((
                "\n\n Common Pool Id = %u,\t No Buffer is in Busy State ",
                 i+1));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                    task_id, buffer_alloc_sig));
            }
            else
            {
                GL_MM_DEBUG_INFO_TRACE((
                "\n\n Common Pool Id = %u,\t%u  Buffers are in Busy State",
                i+1, count));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                    task_id, buffer_alloc_sig));
            }
            
            is_stk_trace_enabled = GL_FALSE;
            total_count += count;
        }
        if( total_count == 0)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n In all Common Pools,\t No Buffer is in Busy State"));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n In all Common Pools,\t %u  Buffers are in Busy State",
                total_count));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }

    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);

#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
            g_a_p_glb_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        GL_MM_DEBUG_INFO_TRACE(("\n Active Common Pool-Id = %d",pool_id+1));
        GL_MM_DEBUG_INFO_TRACE((
            "\n Number of buffers = %u, Size of Buffers = %u",
            g_a_p_glb_pool_table[pool_id]->buf_qty, g_a_p_glb_pool_table
                [pool_id]->buf_size));
        GL_MM_DEBUG_INFO_TRACE((
            "\n Total Number of buffers in Busy State = %u",
            g_a_p_glb_pool_table[pool_id]->busy_buf_qty));
            
       if(GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->is_enabled_flag)
        {
            is_stk_trace_enabled = GL_TRUE;
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE(("\n Stack Tracing is not set for Pool."));
        }
        count = 0;
        
        for(j=0; j<g_a_p_glb_pool_table[pool_id]->buf_qty; j++)
        {
            p_buf_hdr = (gl_mm_buffer_hdr_t *)
                (g_a_p_glb_pool_table[pool_id]->p_hdr_region + j);
                
            if((GL_TRUE == p_buf_hdr->busy_flag) &&
               ((GL_INVALID_TASK_ID == task_id) || 
                (p_buf_hdr->task_id == task_id)   ) &&
               ((0 == buffer_alloc_sig) || 
                (p_buf_hdr->buffer_alloc_signature == buffer_alloc_sig)))     
            {
                GL_MM_DEBUG_INFO_TRACE((
                    "\n %u. Buffer pointer Details:",++count));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Buffer pointer = %p",
                    (g_a_p_glb_pool_table[pool_id]->p_buf_region + (j * 
                    (g_a_p_glb_pool_table[pool_id]->buf_size + sizeof(
                     gl_mm_check_mem_overwrite_t))) + sizeof(
                     gl_mm_check_mem_overwrite_t))));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Task-id Used at Buffer Allocation = %u",
                    p_buf_hdr->task_id));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Buffer Allocation Signature = %d",
                    p_buf_hdr->buffer_alloc_signature));
                    
                if(GL_TRUE == is_stk_trace_enabled)
                {
                    /*
                     * Dump Stack Trace inforamtion.
                     */ 
                    p_stk_inf = g_a_p_glb_pool_table[pool_id]->
                        p_start_stk_inf_reg + j;
       
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Stack Trace Info for Buffer Pointer.\n"));
            
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n Number of Function Entries = %d ", 
                        p_stk_inf->num_of_entries));

                    for(k=0; k < p_stk_inf->num_of_entries; k++)
                    {
                        GL_MM_DEBUG_INFO_TRACE((
                            "\n %d. Function Address Range Pointer = %p",
                            k+1,p_stk_inf->stk_func_add[k]));
                    }
                }/* End of stack trace info */
            }/* Allocated buffer info */
        }
        if( count == 0)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Common pool Id = %u,\t No Buffer is in Busy State",
                pool_id+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Common Pool Id = %u,\t%u  Buffers are in Busy State",
                pool_id+1, count));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
    }    
    GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
        
    return GL_SUCCESS;
#endif
}




/***************************************************************************** 
* Function name    : gl_mm_set_task_pool_stack_trace
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Stack Tracing activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_task_pool_stack_trace

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_boolean_t              is_enabled,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, is_enabled, p_ecode )
      gl_pool_id_t              pool_id;
      gl_boolean_t              is_enabled;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    if(pool_id == 0)
    {
        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Stack Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
                if((g_a_p_app_sp_pool_table[i] != GL_NULL) && 
                   (GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[i]->
                    is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table[i]
                        ->p_start_stk_inf_reg))    
                {
                    /*
                     * Flush stack trace inforamtion.
                     */ 
                    g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                        (g_a_p_app_sp_pool_table[i]->is_enabled_flag & 
                         !(GL_MM_ENABLE_STK_INF));
                    
                    SYS_FREE(g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg);
                    g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg = GL_NULL;
                }
            }
        }
        else
        {
            /*
             * Enable Stack Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
                if((g_a_p_app_sp_pool_table[i] == GL_NULL) 
                  ||(GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[i]->
                      is_enabled_flag))
                {
                    /*
                     * Pool is not created for pool-id = i or
                     * Stack Traceing is already set for pool.
                     */ 
                    continue;
                }
                g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_app_sp_pool_table[i]->is_enabled_flag | 
                   GL_MM_ENABLE_STK_INF);
                
                g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg = 
                    (gl_mm_stk_inf_t *)SYS_MALLOC(g_a_p_app_sp_pool_table[i]->
                     buf_qty * sizeof(gl_mm_stk_inf_t));
                
                if( GL_NULL == g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg)
                {
                    *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                    return GL_FAILURE;
                }
                g_a_p_app_sp_pool_table[i]->p_start_stk_inf_reg->
                    num_of_entries =GL_NULL;
                
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Stack Trace activity.
             */ 
            if((GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->
                        is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table
                        [pool_id]->p_start_stk_inf_reg))    
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
        }
        else
        {
            /*
             * Enable Stack Trace activity.
             */ 
            if(GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->
                    is_enabled_flag)
            {
                /*
                 * Buffer Traceing is already set for pool.
                 */ 
                return GL_SUCCESS;
            }
            g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
              (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag | 
               GL_MM_ENABLE_STK_INF);
                
            g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg = 
                (gl_mm_stk_inf_t *)SYS_MALLOC(g_a_p_app_sp_pool_table[pool_id]
                 ->buf_qty * sizeof(gl_mm_stk_inf_t));
           if( GL_NULL == g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg)
            {
                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
           g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg->num_of_entries
               = GL_NULL;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_set_task_pool_buffer_trace
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_U32bit          number_of_requests,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Buffer Tracing activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_task_pool_buffer_trace

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gw_U32bit                 number_of_requests,
      gl_boolean_t              is_enabled,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, number_of_requests, is_enabled, p_ecode )
      gl_pool_id_t              pool_id;
      gw_U32bit                 number_of_requests;
      gl_boolean_t              is_enabled;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if( GL_NULL == number_of_requests)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_NUM_OF_REQUESTS;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    if(pool_id == 0)
    {
        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Buffer Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
                if((g_a_p_app_sp_pool_table[i] != GL_NULL) && 
                   (GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[i]->
                    is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table[i]
                    ->p_start_buf_trace_reg))    
                {
                    /*
                     * Flush buffer trace inforamtion.
                     */ 
                    g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                        (g_a_p_app_sp_pool_table[i]->is_enabled_flag & 
                         !(GL_MM_ENABLE_BUF_TRACE));
                    
                    SYS_FREE(g_a_p_app_sp_pool_table[i]->p_start_buf_trace_reg);
                    g_a_p_app_sp_pool_table[i]->p_start_buf_trace_reg = GL_NULL;
                }
            }
        }
        else
        {
            /*
             * Enable Buffer Trace activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
               if((g_a_p_app_sp_pool_table[i] == GL_NULL) 
                    ||(GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[i]->
                    is_enabled_flag))
                {
                    /*
                     * Pool is not created for pool-id = i or
                     * Buffer Traceing is already set for pool.
                     */ 
                    continue;
                }
                g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_app_sp_pool_table[i]->is_enabled_flag | 
                   GL_MM_ENABLE_BUF_TRACE);
                
                g_a_p_app_sp_pool_table[i]->p_start_buf_trace_reg = 
                    (gl_mm_buf_trace_t *)SYS_MALLOC(g_a_p_app_sp_pool_table[i]
                    ->buf_qty * ( sizeof(gl_mm_buf_trace_t) * 
                    number_of_requests));
                
               if( GL_NULL == g_a_p_app_sp_pool_table[i]->p_start_buf_trace_reg)
                {
                    *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                    return GL_FAILURE;
                }
                g_a_p_app_sp_pool_table[i]->num_of_buf_trc_reqs = 
                    number_of_requests;
                
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Buffer Trace activity.
             */ 
           if((GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[pool_id]->
                       is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table
                       [pool_id]->p_start_buf_trace_reg)) 
            {
                /*
                 * Flush buffer trace inforamtion.
                 */ 
                g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
                    (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag & 
                    !(GL_MM_ENABLE_BUF_TRACE));
                    
                SYS_FREE(g_a_p_app_sp_pool_table[pool_id]->
                        p_start_buf_trace_reg);
                g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg = 
                    GL_NULL;
            }
        }
        else
        {
            /*
             * Enable Buffer Trace activity.
             */ 
            if(GL_MM_ENABLE_STK_INF & g_a_p_glb_pool_table[pool_id]->
                    is_enabled_flag)
            {
                /*
                 * Buffer Traceing is already set for pool.
                 */ 
                return GL_SUCCESS;
            }
            g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
            (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag | 
             GL_MM_ENABLE_BUF_TRACE);
                
            g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg =
                (gl_mm_buf_trace_t*)SYS_MALLOC(g_a_p_app_sp_pool_table[pool_id]
                ->buf_qty * (sizeof(gl_mm_buf_trace_t) * number_of_requests));
            
            if( GL_NULL == g_a_p_app_sp_pool_table[pool_id]->
                    p_start_buf_trace_reg)
            {
                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
            g_a_p_app_sp_pool_table[pool_id]->num_of_buf_trc_reqs = 
                number_of_requests;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_enable_task_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_buffer_usage_profile_t *p_usage_range_spec,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable Pool Memory Usage activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_enable_task_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t               pool_id,
      gl_mm_pool_usage_profile_t *p_usage_range_spec,
      gl_error_t                 *p_ecode
    )
#else
    ( pool_id, p_usage_range_spec, p_ecode )
      gl_pool_id_t               pool_id;
      gl_mm_pool_usage_profile_t *p_usage_range_spec;
      gl_error_t                 *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if( GL_NULL == p_usage_range_spec)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
        return GL_FAILURE;
    }
    if( GL_MM_POOL_USAGE_MAX_REGIONS <=  p_usage_range_spec->num_regions )
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
        return GL_FAILURE;
    }
    for(j=0; j < p_usage_range_spec->num_regions; j++)
    { 
       if(100 < p_usage_range_spec->usage_region_def[j] ) 
       {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode = GL_MM_ERR_INV_POOL_USAGE_PTR;
            return GL_FAILURE;
       }
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        /*
         * Enable Pool Usage activity.
         */ 
        for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            if((g_a_p_app_sp_pool_table[i] != GL_NULL) 
              ||(GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[i]->
                  is_enabled_flag))
            {
                /*
                 * Pool is not created for pool-id = i or
                 * Pool Usage is already set for pool.
                 */ 
                continue;
            }
            g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                (g_a_p_app_sp_pool_table[i]->is_enabled_flag |
                 GL_MM_ENABLE_POOL_USAGE);
                
            g_a_p_app_sp_pool_table[i]->p_pool_usage_profile = 
                (gl_mm_pool_usage_profile_t *)
                SYS_MALLOC( sizeof(gl_mm_pool_usage_profile_t) );
                
            if( GL_NULL == g_a_p_app_sp_pool_table[i]->p_pool_usage_profile)
            {
                *p_ecode = GL_MM_ERR_MALLOC_FAILED;
                return GL_FAILURE;
            }
            
            g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->num_regions = 
                p_usage_range_spec->num_regions;
            
            for(j=0; j < p_usage_range_spec->num_regions; j++)
            {
                g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->
                    usage_region_def[j] = p_usage_range_spec->usage_region_def
                    [j];
                g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->
                    usage_region_count[j] = GL_NULL;
            }
                
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Enable Pool Usage activity.
         */ 
        if(GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[pool_id]->
                is_enabled_flag)
        {
            /*
             * Pool Uasage Activity is already set for pool.
             */ 
            return GL_SUCCESS;
        }
        g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag = (gw_U8bit)
          (g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag |
           GL_MM_ENABLE_POOL_USAGE);
                
        g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile = 
            (gl_mm_pool_usage_profile_t *)
            SYS_MALLOC( sizeof(gl_mm_pool_usage_profile_t) );
            
        if( GL_NULL == g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile)
        {
            *p_ecode = GL_MM_ERR_MALLOC_FAILED;
            return GL_FAILURE;
        }
        
        g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->num_regions = 
        p_usage_range_spec->num_regions;
            
        for(j=0; j < p_usage_range_spec->num_regions; j++)
        {
            g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->
                usage_region_def[j] 
                = p_usage_range_spec->usage_region_def[j];    
            g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->
                usage_region_count[j] = GL_NULL;
        }
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_disable_task_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Disable Pool Memory Usage activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_disable_task_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        /*
         * Disable Buffer Trace activity.
         */ 
        for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            if((g_a_p_app_sp_pool_table[i] != GL_NULL) && 
               (GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[i]->
                is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table[i]->
                p_pool_usage_profile))    
            {
                /*
                 * Flush Pool Usage inforamtion.
                 */ 
                g_a_p_app_sp_pool_table[i]->is_enabled_flag = (gw_U8bit)
                  (g_a_p_app_sp_pool_table[i]->is_enabled_flag & 
                  !(GL_MM_ENABLE_POOL_USAGE));
                    
                SYS_FREE(g_a_p_app_sp_pool_table[i]->p_pool_usage_profile);
                g_a_p_app_sp_pool_table[i]->p_pool_usage_profile = GL_NULL;
            }
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Disable Pool Usage activity.
         */ 
        if((GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[pool_id]->
            is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->
            p_pool_usage_profile))   
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
            
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_get_task_pool_stack_trace
* Arguments        : gl_pvoid_t         p_buf_ptr,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Return Stack Trace activity Traces.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_get_task_pool_stack_trace

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( p_buf_ptr, p_ecode )
      gl_pvoid_t                p_buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U8bit                    i;
    gw_U32bit                   buf_index;
    gl_mm_buffer_hdr_t          *p_hdr;
    gl_mm_stk_inf_t             *p_stk_inf;
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_buf_ptr == GL_NULL)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }

    pool_id = p_buf_start->pool_id;

    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr + g_a_p_app_sp_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)p_buf_ptr -
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;

#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */
    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_task_pool_buffer_pointer
        (pool_id, p_buf_ptr, &buf_index))
    {
        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_app_sp_pool_table[pool_id]->p_hdr_region + buf_index;
    /*
     * To check, if buffer is already free or not.
     */ 
    if(GL_FALSE == p_hdr->busy_flag)
    {
        *p_ecode = GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    
   if((GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->is_enabled_flag)
       && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg))    
    {
        /*
         * Dump stack trace inforamtion.
         */ 
        p_stk_inf = g_a_p_app_sp_pool_table[pool_id]->p_start_stk_inf_reg + 
            buf_index;
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
        GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
        GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
       
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n Stack Trace Info for Buffer Pointers of Task Pool."));
            
        GL_MM_DEBUG_INFO_TRACE(("\n Buffer Pointer = %p ",p_buf_ptr));
        GL_MM_DEBUG_INFO_TRACE(("\n Number of Function Entries = %d",
                    p_stk_inf->num_of_entries));

        for(i=0; i < p_stk_inf->num_of_entries; i++)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t %d. Function Address Range Pointer = %p",
                i+1,p_stk_inf->stk_func_add[i]));
        }
        GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    }
    return GL_SUCCESS;
#endif
}


/*****************************************************************************
* Function name    : gl_mm_get_task_pool_buffer_trace
* Arguments        : gl_pvoid_t         p_buf_ptr,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Return Buffer Trace activity Traces.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_get_task_pool_buffer_trace

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( p_buf_ptr, p_ecode )
      gl_pvoid_t                p_buf_ptr;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U32bit                   i;
    gw_U32bit                   buf_index;
    gl_mm_buffer_hdr_t          *p_hdr = GL_NULL;
    gl_mm_buf_trace_t           *p_buf_trace = GL_NULL; 
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gl_mm_check_mem_overwrite_t *p_buf_start;
    gl_mm_check_mem_overwrite_t *p_buf_end;
#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_buf_ptr == GL_NULL)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_MM_ADD_MEM_OVERW_OPR
    p_buf_start = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr - sizeof(gl_mm_check_mem_overwrite_t));

    if((p_buf_start->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_start->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }

    pool_id = p_buf_start->pool_id;

    p_buf_end = (gl_mm_check_mem_overwrite_t *)
            ((gw_S8bit*)p_buf_ptr + g_a_p_app_sp_pool_table[pool_id]->buf_size);

    if((p_buf_end->start_check_bit != GL_MM_CHECK_BIT) ||
       (p_buf_end->end_check_bit != GL_MM_CHECK_BIT) )
    {
        *p_ecode = GL_MM_ERR_MEM_OVERWRITE;
        return GL_FAILURE;
    }
#else

    pool_id = ((gl_mm_check_mem_overwrite_t *)((gw_S8bit*)p_buf_ptr -
                sizeof(gl_mm_check_mem_overwrite_t)))->pool_id;

#endif /* Endif for GL_MM_ADD_MEM_OVERW_OPR */

    /*
     * Validate buffer pointer.
     */
    if(GL_SUCCESS != mm_validate_task_pool_buffer_pointer
        (pool_id, p_buf_ptr, &buf_index))
    {
        *p_ecode= GL_MM_ERR_INV_BUF_PTR;
        return GL_FAILURE;
    }
    p_hdr = g_a_p_app_sp_pool_table[pool_id]->p_hdr_region + buf_index;

    if((GL_MM_ENABLE_BUF_TRACE & g_a_p_app_sp_pool_table[pool_id]->
                is_enabled_flag)
       && (GL_NULL != g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg))    
    {
        /*
         * Dump stack trace inforamtion.
         */ 
        p_buf_trace = g_a_p_app_sp_pool_table[pool_id]->p_start_buf_trace_reg + 
           (buf_index * g_a_p_app_sp_pool_table[pool_id]->num_of_buf_trc_reqs) ;
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
        GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
        GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
       
        GL_MM_DEBUG_INFO_TRACE((
        "\n\n Buffer Trace Info for Buffer Pointers of Task Pool."));
            
        GL_MM_DEBUG_INFO_TRACE(("\n Buffer Pointer = %p",p_buf_ptr));
        GL_MM_DEBUG_INFO_TRACE(("\n Pool-id of Buffer Pointer = %u",
                    p_hdr->pool_id));
        GL_MM_DEBUG_INFO_TRACE(("\n Number of Buffer Trace Requests = %u",
                     g_a_p_app_sp_pool_table[pool_id]->num_of_buf_trc_reqs));

        for(i=0; i < g_a_p_app_sp_pool_table[pool_id]->num_of_buf_trc_reqs; i++)
        {
           p_buf_trace = p_buf_trace + i; 
           GL_MM_DEBUG_INFO_TRACE(("\n\n\t %d. Buffer Trace Information", 
                       i+1));
           GL_MM_DEBUG_INFO_TRACE(("\n\t\t Buffer Status = %s",
                       p_buf_trace->buf_status ? "BUSY":"FREE"));
           GL_MM_DEBUG_INFO_TRACE((
                "\n\t\t Task-id Used at Buffer Allocation = %u",
                p_buf_trace->task_id));
           GL_MM_DEBUG_INFO_TRACE(("\n\t\t Buffer Allocation Signature = %d",
                       p_buf_trace->buffer_alloc_signature));
           
        }
        GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    }
    return GL_SUCCESS;
#endif
}



/*****************************************************************************
* Function name    : gl_mm_report_task_pool_usage_profiling
* Arguments        : gl_pool_id_t       pool_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Report Pool Memory Usage Profile.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_report_task_pool_usage_profiling

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_ecode )
      gl_pool_id_t              pool_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
    GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
    GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
    if(pool_id == 0)
    {
        /*
         * Disable Buffer Trace activity.
         */ 
        for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            if((g_a_p_app_sp_pool_table[i] != GL_NULL) && 
               (GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[i]->
                is_enabled_flag)
               && (GL_NULL != g_a_p_app_sp_pool_table[i]->p_pool_usage_profile)&&
                            g_a_p_app_sp_pool_table[i]->busy_buf_qty != GL_NULL)    
            {
                /*
                 * Dump Pool Usage inforamtion.
                 */ 
                GL_MM_DEBUG_INFO_TRACE((
               "\n\n Memory Usage Information for Task Pool, Pool-id = %d ",
                 i+1));
                GL_MM_DEBUG_INFO_TRACE((
                  "\n Buffer Size = %u \t Number of Buffers = %u",
                  g_a_p_app_sp_pool_table[i]->buf_size, g_a_p_app_sp_pool_table
                     [i]->buf_qty));  
                GL_MM_DEBUG_INFO_TRACE((
                "\n Maximum Buffers in Busy State at any time = %u\n",
                g_a_p_app_sp_pool_table[i]->curr_max_used));
                GL_MM_DEBUG_INFO_TRACE(("\n Number of Usage Regions = %d ",
                g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->num_regions));
                for(j=0; j < g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->
                        num_regions; j++ )
                {
                    GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Number of Allocations in %d %% Usage Region = %d.",
                    g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->
                    usage_region_def[j],
                    g_a_p_app_sp_pool_table[i]->p_pool_usage_profile->
                    usage_region_count[j]));
                }
            }
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        /*
         * Disable Pool Usage activity.
         */ 
        if((GL_MM_ENABLE_POOL_USAGE & g_a_p_app_sp_pool_table[pool_id]->
                    is_enabled_flag) && (GL_NULL != g_a_p_app_sp_pool_table
                    [pool_id]->p_pool_usage_profile)&&
                    g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty != GL_NULL)   
        {
            /*
             * Dump Pool Usage inforamtion.
             */ 
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Memory Usage Information for Task Pool, Pool-id = %d ",
                pool_id+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Buffer Size = %u \t Number of Buffers = %u",
                g_a_p_app_sp_pool_table[pool_id]->buf_size,
                g_a_p_app_sp_pool_table[pool_id]->buf_qty));
            GL_MM_DEBUG_INFO_TRACE((
               "\n Maximum Buffers in Busy State at any time = %u\n",
               g_a_p_app_sp_pool_table[pool_id]->curr_max_used));
            GL_MM_DEBUG_INFO_TRACE(("\n Number of Usage Regions = %d ",
                g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->
                num_regions));
            for(j=0; j < g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile
                    ->num_regions; j++ )
            {
              GL_MM_DEBUG_INFO_TRACE((
                "\n\t Number of Allocations in %d %% Usage Region = %d",
              g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->
              usage_region_def[j],
              g_a_p_app_sp_pool_table[pool_id]->p_pool_usage_profile->
              usage_region_count[j]));
            }
        }
            
    }

    GL_MM_DEBUG_INFO_TRACE((
      "\n\n************************************************************\n"));
    return GL_SUCCESS;
#endif
}


/*****************************************************************************
* Function name    : gl_mm_report_task_pool_busy_buffers_statistics
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_U32bit          task_id,
*                    gw_U8bit           buffer_alloc_sig,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Report Busy Buffer Statistics of Task Pool.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_report_task_pool_busy_buffers_statistics

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gw_U32bit                 task_id,
      gw_U8bit                  buffer_alloc_sig,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, task_id, buffer_alloc_sig, p_ecode )
      gl_pool_id_t              pool_id;
      gw_U32bit                 task_id;
      gw_U8bit                  buffer_alloc_sig;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;
    gw_U32bit                   j;
    gw_U8bit                    k;
    gl_mm_buffer_hdr_t          *p_buf_hdr;
    gw_U32bit                   count = 0;
    gw_U32bit                   total_count = 0;
    gw_U32bit                   is_stk_trace_enabled = GL_FALSE;
    gl_mm_stk_inf_t             *p_stk_inf;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MM_DEBUG_INFO_TRACE((
        "\n\n************************************************************"));
    GL_MM_DEBUG_INFO_TRACE((
        "\n\t\t GENLIB Memory Manager DEBUGGER Traces"));            
    GL_MM_DEBUG_INFO_TRACE((
        "\n\n\t\t Allocated Task Pool Buffers Informantion"));
    GL_MM_DEBUG_INFO_TRACE((
        "\n************************************************************"));
    
    if(pool_id == 0)
    {
        /*
         * Get Busy Buffer Reoprt for all Pools.
         */ 
        for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            if((GL_NULL == g_a_p_app_sp_pool_table[i]) ||
               ((GL_INVALID_TASK_ID != task_id) && 
                (g_a_p_app_sp_pool_table[i]->owner_task_id != task_id)) ||
                 ((GL_NULL != g_a_p_app_sp_pool_table[i]) && (g_a_p_app_sp_pool_table[i]->busy_buf_qty == GL_NULL)))
            {
                continue;
            }
            GL_MM_DEBUG_INFO_TRACE((
                "\n Active Task Pool-Id = %d",i+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Number of Buffers = %u, Size of Buffers = %u",
                g_a_p_app_sp_pool_table[i]->buf_qty, g_a_p_app_sp_pool_table[i]
                ->buf_size));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Total Number of Buffers in Busy State = %u",
                g_a_p_app_sp_pool_table[i]->busy_buf_qty));
            GL_MM_DEBUG_INFO_TRACE((
                "\n Task Pool is created with Task-id = %u",
                g_a_p_app_sp_pool_table[i]->owner_task_id));
            
            if(GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[i]->
                    is_enabled_flag)
            {
                is_stk_trace_enabled = GL_TRUE;
            }
            else
            {
              GL_MM_DEBUG_INFO_TRACE(("\n Stack Tracing is not Set for Pool"));
            }
            count = 0;
            
            for(j=0; j < g_a_p_app_sp_pool_table[i]->buf_qty; j++)
            {
                p_buf_hdr = (gl_mm_buffer_hdr_t *)
                    (g_a_p_app_sp_pool_table[i]->p_hdr_region + j);
                
                if((GL_TRUE == p_buf_hdr->busy_flag) && 
                   ((0 == buffer_alloc_sig) || 
                    (p_buf_hdr->buffer_alloc_signature == buffer_alloc_sig)))     
                {
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n %u. Buffer pointer Details:",++count));
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Buffer pointer = %p",
                        (g_a_p_app_sp_pool_table[i]->p_buf_region + 
                         (j * (g_a_p_app_sp_pool_table[i]->
                         buf_size + sizeof(gl_mm_check_mem_overwrite_t)))
                         + sizeof(gl_mm_check_mem_overwrite_t))));
                    
                    GL_MM_DEBUG_INFO_TRACE((
                      "\n\t Buffer Allocation Signature = %d",
                      p_buf_hdr->buffer_alloc_signature));
                    
                    if(GL_TRUE == is_stk_trace_enabled)
                    {
                        /*
                         * Dump Stack Trace inforamtion.
                         */ 
                        p_stk_inf = g_a_p_app_sp_pool_table[i]->
                            p_start_stk_inf_reg + j;
       
                        GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Stack Trace Information for Buffer Pointer.\n"));
                        GL_MM_DEBUG_INFO_TRACE((
                            "\n\t Number of Function Entries = %d ", 
                            p_stk_inf->num_of_entries));

                        for(k=0; k < p_stk_inf->num_of_entries; k++)
                        {
                            GL_MM_DEBUG_INFO_TRACE((
                             "\n\t\t %d. Function Address Range Pointer = %p",
                             k+1,p_stk_inf->stk_func_add[k]));
                        }
                    }/* End of stack trace info */
                }/* Allocated buffer info */
            }/* End for all buffers of a task pool */
            if( count == 0)
            {
                GL_MM_DEBUG_INFO_TRACE((
                  "\n\n Task Pool Id = %u,\t No Buffer is in Busy State",
                    i+1));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                    task_id, buffer_alloc_sig));
            }
            else
            {
                GL_MM_DEBUG_INFO_TRACE((
                "\n\n Task Pool Id = %u,\t %u  Buffers are in Busy State", 
                i+1, count));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                    task_id, buffer_alloc_sig));
            }
            
            is_stk_trace_enabled = GL_FALSE;
            total_count += count;
        }
        if( total_count == 0)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n In all Task Pools,\t No Buffer is in Busy State"));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n In all Task Pools,\t %u  Buffers are in Busy State",
                total_count));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }

    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array is pool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_TASK_POOL_QTY ||
            g_a_p_app_sp_pool_table[pool_id] == GL_NULL)
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
       /*
        * To remove warnings.
        */
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if(((GL_INVALID_TASK_ID != task_id) &&
           (g_a_p_app_sp_pool_table[pool_id]->owner_task_id != task_id)))
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Task Pool Id = %u,\t No Buffer is in Busy State",
                pool_id+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            
            return GL_SUCCESS;
        }  
      GL_MM_DEBUG_INFO_TRACE(("\n Active Task Pool-Id = %d",pool_id+1));
      GL_MM_DEBUG_INFO_TRACE((
        "\n Number of buffers = %u, Size of Buffers = %u",
        g_a_p_app_sp_pool_table[pool_id]->buf_qty, g_a_p_app_sp_pool_table
        [pool_id]->buf_size));
      GL_MM_DEBUG_INFO_TRACE((
        "\n Total Number of buffers in Busy State = %u",
            g_a_p_app_sp_pool_table[pool_id]->busy_buf_qty));
            
        if(GL_MM_ENABLE_STK_INF & g_a_p_app_sp_pool_table[pool_id]->
                is_enabled_flag)
        {
            is_stk_trace_enabled = GL_TRUE;
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE(("\n Stack Tracing is not set for Pool."));
        }
        count = 0;
        
        for(j=0; j<g_a_p_app_sp_pool_table[pool_id]->buf_qty; j++)
        {
            p_buf_hdr = (gl_mm_buffer_hdr_t *)
                (g_a_p_app_sp_pool_table[pool_id]->p_hdr_region + j);
                
            if((GL_TRUE == p_buf_hdr->busy_flag) &&
               ((0 == buffer_alloc_sig) || 
                (p_buf_hdr->buffer_alloc_signature == buffer_alloc_sig)))     
            {
                GL_MM_DEBUG_INFO_TRACE((
                    "\n %u. Buffer pointer Details:",++count));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Buffer pointer = %p",
                    (g_a_p_app_sp_pool_table[pool_id]->p_buf_region + (j * 
                    (g_a_p_app_sp_pool_table[pool_id]->
                    buf_size + sizeof(gl_mm_check_mem_overwrite_t)))
                         + sizeof(gl_mm_check_mem_overwrite_t))));
                GL_MM_DEBUG_INFO_TRACE((
                    "\n\t Buffer Allocation Signature = %d",
                    p_buf_hdr->buffer_alloc_signature));
                    
                if(GL_TRUE == is_stk_trace_enabled)
                {
                    /*
                     * Dump Stack Trace inforamtion.
                     */ 
                    p_stk_inf = g_a_p_app_sp_pool_table[pool_id]->
                        p_start_stk_inf_reg+j;
       
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n\t Stack Trace Info for Buffer Pointer.\n"));
            
                    GL_MM_DEBUG_INFO_TRACE((
                        "\n Number of Func Entries = %d ", 
                        p_stk_inf->num_of_entries));

                    for(k=0; k < p_stk_inf->num_of_entries; k++)
                    {
                        GL_MM_DEBUG_INFO_TRACE((
                            "\n %d. Function Address Range Pointer = %p",
                            k+1,p_stk_inf->stk_func_add[k]));
                    }
                }/* End of stack trace info */
            }/* Allocated buffer info */
        }
        if( count == 0)
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Task Pool Id = %u,\t No Buffer is in Busy State",
                pool_id+1));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
        else
        {
            GL_MM_DEBUG_INFO_TRACE((
                "\n\n Task Pool Id = %u,\t %u  Buffers are in Busy State",
                pool_id+1, count));
            GL_MM_DEBUG_INFO_TRACE((
                "\n\t for Task-id = %u, Buffer Alloction Signature = %d",
                task_id, buffer_alloc_sig));
            GL_MM_DEBUG_INFO_TRACE(("\n\n"));
        }
    }    
        
    GL_MM_DEBUG_INFO_TRACE((
       "\n\n************************************************************\n"));
    return GL_SUCCESS;
#endif
}




/*****************************************************************************
* Function name    : gl_mm_set_buffer_alloc_signature
* Arguments        : gw_U8bit           buffer_alloc_sig,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Set the global buffer allocation signature,
*                    used to mark the buffer at buffer allocation.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_buffer_alloc_signature

#ifdef _ANSI_C_
    ( gw_U8bit                  buffer_alloc_sig,
      gl_error_t                *p_ecode
    )
#else
    ( buffer_alloc_sig, p_ecode)
      gw_U8bit                  buffer_alloc_sig;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if( 0 == buffer_alloc_sig )
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_BUFFER_ALOC_SIGNATURE;
        return GL_FAILURE;
    }
#else
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    buffer_alloc_signature = buffer_alloc_sig;
    return GL_SUCCESS;
}
 

#endif /* Endif for GL_MM_ADD_DEBUGGER */




#ifdef GL_MM_ADD_AUDIT_MGT


/*****************************************************************************
* Function name    : gl_mm_set_cmn_pool_audit_activity
* Arguments        : gw_boolean         is_enable,
*                    gw_U32bit          max_num_of_user_id, 
                     gw_U32bit          special_user_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To set/unset the Audit Mgt. Activity.
*                    This API must be called before any other API related to
*                    Audit Mgt. is called. 
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_set_cmn_pool_audit_activity

#ifdef _ANSI_C_
    ( gw_boolean                is_enable,
      gw_U32bit                 max_num_of_user_id,
      gw_U32bit                 special_user_id,
      gl_error_t                *p_ecode
    )
#else
    ( is_enable, max_num_of_user_id, special_user_id, p_ecode )
      gw_boolean                is_enable;
      gw_U32bit                 max_num_of_user_id;
      gw_U32bit                 special_user_id;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_pvoid_t                  p_hash_arg;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    if((GL_FALSE == audit_act_set_flag) && (GL_TRUE == is_enable))
    {
        /*
         * Create the hash-table, used to store the user-id info.
         */
        if(GL_FAILURE == gl_htm_create_hash_table (special_user_id,
            &g_mm_audit_hash_table_id, max_num_of_user_id, max_num_of_user_id
            /10, GL_HTM_NUMERIC_KEY_TYPE,
            GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS, GL_NULL,
            (gl_htm_free_key_elem_fn_t) mm_audit_memory_free_func, GL_NULL,
            GL_NULL, p_ecode))
        {
            GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
            *p_ecode = GL_MM_ERR_NO_RESOURCES;
            return GL_FAILURE;
        }
        
        audit_act_set_flag = GL_TRUE;
        g_mm_audit_special_user_id = special_user_id;
            
    }
    else if((GL_TRUE == audit_act_set_flag) && (GL_FALSE == is_enable))
    {
        /*
         * Remove all the user-id information from hash-table.
         */
        if(GL_FAILURE == gl_htm_remove_all_keys_from_hash_table(
            g_mm_audit_special_user_id, g_mm_audit_hash_table_id, p_ecode))
        {
            GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
 
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        
        }
        /*
         * Delete the hash-table and clean the memory allocated for
         * user_id-information structure.
         */
         
        if(GL_FAILURE == gl_htm_delete_hash_table(g_mm_audit_special_user_id,
            g_mm_audit_hash_table_id, &p_hash_arg, p_ecode))
        {
            GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
              
            *p_ecode = GL_MM_ERR_INTERNAL;
            return GL_FAILURE;
        }
            
        /*
         * Free the Hash-Walk handle instance.
         */
        if(GL_NULL != g_p_audit_hash_walk_handle)
        {
            if(GL_FAILURE == gl_htm_terminate_get_key_list_search(
                g_mm_audit_hash_table_id, &g_p_audit_hash_walk_handle,
                p_ecode))
            {
                GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
              
                *p_ecode = GL_MM_ERR_INTERNAL;
                return GL_FAILURE;
            }
        }            

        audit_act_set_flag = GL_FALSE;
    }
    /*
     * No need of else part as not required.
     */ 
    GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    return GL_SUCCESS;

#endif
    
}




/*****************************************************************************
* Function name    : gl_mm_get_busy_user_id_info_list
* Arguments        : gw_boolean         info_list_from_first_id,
*                    gw_U8bit           *p_num_of_ids, 
                     gw_U32bit          a_user_id[],
                     gl_user_id_info_t  *a_user_id_info[],
                     gw_boolean	        *p_is_more_ids_to_traverse,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To return the list of user_id information based on  
*                    start_user_id_info_list.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_get_busy_user_id_info_list

#ifdef _ANSI_C_
    ( gw_boolean                info_list_from_first_id,
      gw_U8bit                 *p_num_of_id,
      gw_U32bit                 a_user_id[],
      gl_user_id_info_t         *a_user_id_info[],
      gw_boolean                *p_is_more_ids_to_traverse,
      gl_error_t                *p_ecode
    )
#else
    ( info_list_from_first_id, p_num_of_id, a_user_id, a_user_id_info,
      p_is_more_ids_to_traverse, p_ecode )
      gw_boolean                info_list_from_first_id;
      gw_U8bit                 *p_num_of_id;
      gw_U32bit                 a_user_id[];
      gl_user_id_info_t         *a_user_id_info[];
      gw_boolean                *p_is_more_ids_to_traverse;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    if(GL_FALSE == audit_act_set_flag)
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        *p_ecode = GL_MM_ERR_AUDIT_ACTIVITY_NOT_SET;
         return GL_FAILURE;
    }

    if(GL_TRUE == info_list_from_first_id)
    {
        /*
         * Free the Hash-Walk handle instance.
         */
        if(GL_NULL != g_p_audit_hash_walk_handle)
        {
            if(GL_FAILURE == gl_htm_terminate_get_key_list_search(
                g_mm_audit_hash_table_id, &g_p_audit_hash_walk_handle,
                p_ecode))
            {
                GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
              
                *p_ecode = GL_MM_ERR_INTERNAL;
                return GL_FAILURE;
            }
        }            
        g_p_audit_hash_walk_handle = GL_NULL;
    }

    if(GL_FAILURE == gl_htm_get_key_list_from_hash_table(
        g_mm_audit_special_user_id, g_mm_audit_hash_table_id,     
        &g_p_audit_hash_walk_handle, p_num_of_id, GL_NULL, GL_NULL,
        (gl_hash_key_ut *)a_user_id, (gl_pvoid_t *)a_user_id_info, 
        p_is_more_ids_to_traverse, p_ecode))
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
 
        *p_ecode = GL_MM_ERR_INTERNAL;
        return GL_FAILURE;
    }
    
    GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS; 
    
#endif
}


    

/*****************************************************************************
* Function name    : gl_mm_get_user_id_status
* Arguments        : gw_U32bit          user_id,
*                    gw_U32bit          *p_num_of_bufs_alocated, 
*                    gl_time_stamp_t    *p_time_stamp,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To return the number of buffer currently in allocated 
*                    state and these buffers should be allocated with given
*                    user_id.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_get_user_id_status

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 *p_num_of_bufs_alocated,
      gl_time_stamp_t           *p_time_stamp,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_num_of_bufs_alocated, p_time_stamp, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 *p_num_of_bufs_alocated;
      gl_time_stamp_t           *p_time_stamp;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_user_id_info_node_t   *p_user_id_info_node;
    gw_boolean                  is_id_found;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    if(GL_FALSE == audit_act_set_flag)
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        *p_ecode = GL_MM_ERR_AUDIT_ACTIVITY_NOT_SET;
         return GL_FAILURE;
    }
    
    if(GL_FAILURE == mm_get_user_id_info_node(user_id, &p_user_id_info_node,
        &is_id_found, p_ecode ))
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        
        return GL_FAILURE;
    }
    
    if(GL_FALSE == is_id_found)
    {
        *p_num_of_bufs_alocated = 0;
        p_time_stamp->sec = 0;
        p_time_stamp->nsec = 0;
    }
    else
    {
        *p_num_of_bufs_alocated = p_user_id_info_node->user_id_info.
            num_of_aloc_bufs;
        p_time_stamp->sec = p_user_id_info_node->user_id_info.time_stamp.sec;
        p_time_stamp->nsec = p_user_id_info_node->user_id_info.time_stamp.nsec;
    }
       
    GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS; 
    
#endif
}


    
/*****************************************************************************
* Function name    : gl_mm_release_all_user_id_aloc_bufs
* Arguments        : gw_U32bit          user_id,
*                    gw_U32bit          *p_num_of_bufs_released, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : To release all the buffers allocated with given user-id 
*                    and also return the number of buffer released.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_mm_release_all_user_id_aloc_bufs

#ifdef _ANSI_C_
    ( gw_U32bit                 user_id,
      gw_U32bit                 *p_num_of_bufs_released,
      gl_error_t                *p_ecode
    )
#else
    ( user_id, p_num_of_bufs_released, p_ecode )
      gw_U32bit                 user_id;
      gw_U32bit                 *p_num_of_bufs_released;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else    
    gl_mm_user_id_info_node_t   *p_user_id_info_node;
    gw_boolean                  is_id_found;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    if(GL_FALSE == audit_act_set_flag)
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        *p_ecode = GL_MM_ERR_AUDIT_ACTIVITY_NOT_SET;
         return GL_FAILURE;
    }
    
    if(GL_FAILURE == mm_remove_user_id_info_node(user_id, &p_user_id_info_node,
        &is_id_found, p_ecode ))
    {
        GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        
        return GL_FAILURE;
    }
    
    if(GL_FALSE == is_id_found)
    {
        *p_num_of_bufs_released = 0;
    }
    else
    {
        if(GL_FAILURE == mm_release_all_id_allocated_bufs(user_id, 
            p_user_id_info_node->p_first_aloc_buf, p_ecode))
        {
            GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
        
            return GL_FAILURE;    
        }
        
        *p_num_of_bufs_released = p_user_id_info_node->user_id_info.
            num_of_aloc_bufs;
        
        if(GL_FAILURE == gl_mm_release_buffer(g_mm_audit_special_user_id,
            (gl_pvoid_t)p_user_id_info_node, p_ecode))
        {
            GL_MUTEX_UNLOCK(&(mm_audit_sem), p_ecode, GL_FAILURE);
        
            return GL_FAILURE;
        }
    }
       
    GL_MUTEX_UNLOCK(&mm_audit_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS; 
    
#endif
}

#endif /* Endif for GL_MM_ADD_AUDIT_MGT */
