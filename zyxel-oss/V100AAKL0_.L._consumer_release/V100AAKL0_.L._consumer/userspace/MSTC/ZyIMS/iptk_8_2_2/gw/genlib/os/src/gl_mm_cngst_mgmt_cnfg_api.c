/*****************************************************************************  
*
*    FUNCTION    : Source file for Generic Library - Memory manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm_cngst_mgmt_cnfg_api.c
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
* Function name    : gl_mm_set_cngst_mgmt_specs_for_task_pools
* Arguments        : gw_U32bit              task_id,
*                    gl_pool_id_t           pool_id,
*                    gl_cngst_mgmt_spec_t   *p_specs,
*                    gl_cngst_mgmt_fn_t     cngst_mgmt_call_back,
*                    gl_error_t             *p_ecode
* Return Value     : gl_return_t
* Functionality    : Set Congestion Management Region's
*                    Specification parameters.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cngst_mgmt_specs_for_task_pools

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_pool_id_t              pool_id,
      gl_cngst_mgmt_spec_t      *p_specs,
      gl_cngst_mgmt_fn_t        cngst_mgmt_call_back,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, p_specs, cngst_mgmt_call_back, p_ecode )
      gw_U32bit                 task_id;
      gl_pool_id_t              pool_id;
      gl_cngst_mgmt_spec_t      *p_specs;
      gl_cngst_mgmt_fn_t        cngst_mgmt_call_back;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    gw_U8bit                    j;

    if(init_flag_task_pools == GL_FALSE)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_specs == GL_NULL)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_PTR;
        return GL_FAILURE;
    }
    if(cngst_mgmt_call_back == GL_NULL)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_CALL_BACK_FUNC_PTR;
        return GL_FAILURE;
    }
    if(0 == p_specs->num_regions)
    {
        GL_INC_MM_U_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES;
        return GL_FAILURE;
    }
    for(j=0; j < p_specs->num_regions; j++)
    {
        if(p_specs->region_def[j].onset > 100 ||
           p_specs->region_def[j].discard > 100 ||
           p_specs->region_def[j].abate > 100) 
        {
            GL_INC_MM_U_G_PARM_VLD_FAIL()

            *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES;
            return GL_FAILURE;
        }
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(pool_id == 0)
    {
        for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
        {
            if(g_a_p_app_sp_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
            {
                /*
                 * Pool is not created for pool-id = i.
                 */
                continue;
            }
            g_a_p_app_sp_pool_table[i]->cngst_mgmt_call_back = 
                cngst_mgmt_call_back;
            g_a_p_app_sp_pool_table[i]->current_region_index = 0; 
            /*
             * Copy Specification Information.
             */ 
            gl_mm_memcpy((gw_U8bit*)&(g_a_p_app_sp_pool_table[i]->specs),  
                         (gw_U8bit*)p_specs, sizeof(gl_cngst_mgmt_spec_t)); 
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array ispool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
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
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        g_a_p_app_sp_pool_table[pool_id]->cngst_mgmt_call_back = 
            cngst_mgmt_call_back;
        g_a_p_app_sp_pool_table[pool_id]->current_region_index = 0; 
        /*
         * Copy Specification Information.
         */ 
        gl_mm_memcpy((gw_U8bit*)&(g_a_p_app_sp_pool_table[pool_id]->specs),  
                     (gw_U8bit*)p_specs, sizeof(gl_cngst_mgmt_spec_t));
    }
    return GL_SUCCESS;
#endif
}


        

/*****************************************************************************
* Function name    : gl_mm_set_cngst_mgmt_specs_for_cmn_pools
* Arguments        : gl_pool_id_t           pool_id,
*                    gl_cngst_mgmt_spec_t   *p_specs,
*                    gl_cngst_mgmt_fn_t     cngst_mgmt_call_back,
*                    gl_error_t             *p_ecode
* Return Value     : gl_return_t
* Functionality    : Set Congestion Management Region's
*                    Specification parameters.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cngst_mgmt_specs_for_cmn_pools

#ifdef _ANSI_C_
    ( gl_pool_id_t              pool_id,
      gl_cngst_mgmt_spec_t      *p_specs,
      gl_cngst_mgmt_fn_t        cngst_mgmt_call_back,
      gl_error_t                *p_ecode
    )
#else
    ( pool_id, p_specs, cngst_mgmt_call_back, p_ecode )
      gl_pool_id_t              pool_id;
      gl_cngst_mgmt_spec_t      *p_specs;
      gl_cngst_mgmt_fn_t        cngst_mgmt_call_back;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_DEBUG
    return GL_SUCCESS;
#else
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    gw_U8bit                    j;

    if(init_flag_cmn_pools == GL_FALSE)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

         *p_ecode = GL_MM_ERR_INIT_REQD;
         return GL_FAILURE;
    }
    if(p_specs == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_PTR;
        return GL_FAILURE;
    }
    if(cngst_mgmt_call_back == GL_NULL)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_CALL_BACK_FUNC_PTR;
        return GL_FAILURE;
    }
    if(0 == p_specs->num_regions)
    {
        GL_INC_MM_B_G_PARM_VLD_FAIL()

        *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES;
        return GL_FAILURE;
    }
    for(j=0; j < p_specs->num_regions; j++)
    {
        if(p_specs->region_def[j].onset > 100 ||
           p_specs->region_def[j].discard > 100 ||
           p_specs->region_def[j].abate > 100) 
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode = GL_MM_ERR_INV_CNGST_MGMT_SPECS_VALUES;
            return GL_FAILURE;
        }
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    if(pool_id == 0)
    {
        for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
        {
            if(g_a_p_glb_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
            {
                /*
                 * Pool is not created for pool-id = i.
                 */ 
                continue;
            }
           g_a_p_glb_pool_table[i]->cngst_mgmt_call_back = cngst_mgmt_call_back;
           g_a_p_glb_pool_table[i]->current_region_index = 0; 
            /*
            * Copy Specification Information.
            */ 
            gl_mm_memcpy((gw_U8bit*)&(g_a_p_glb_pool_table[i]->specs),  
                         (gw_U8bit*)p_specs, sizeof(gl_cngst_mgmt_spec_t)); 
        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array ispool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
           GL_NULL == g_a_p_glb_pool_table[pool_id])
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
        (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        g_a_p_glb_pool_table[pool_id]->cngst_mgmt_call_back = 
            cngst_mgmt_call_back;
        g_a_p_glb_pool_table[pool_id]->current_region_index = 0; 
        /*
         * Copy Specification Information.
         */ 
        gl_mm_memcpy((gw_U8bit*)&(g_a_p_glb_pool_table[pool_id]->specs),  
                     (gw_U8bit*)p_specs, sizeof(gl_cngst_mgmt_spec_t));
    }
    return GL_SUCCESS;
#endif
}




/*****************************************************************************
* Function name    : gl_mm_set_cngst_mgmt_activity_for_cmn_pools
* Arguments        : gl_pool_id_t       pool_id,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Congestion Management activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cngst_mgmt_activity_for_cmn_pools

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
             * Disable Congestion Management activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
                if(g_a_p_glb_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
                {
                    /*
                     * Pool is not created for pool-id = i.
                     */ 
                    continue;
                }
                g_a_p_glb_pool_table[i]->is_cngst_mgmt_enabled = GL_FALSE;
            }
        }
        else
        {
            /*
             * Enable Congestion Management activity.
             */ 
            for(i=0; i < GL_MM_MAX_COMMON_POOL_QTY; i++)
            {
                if(g_a_p_glb_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
                {
                    /*
                     * Pool is not created for pool-id = i.
                     */ 
                    continue;
                }
                g_a_p_glb_pool_table[i]->is_cngst_mgmt_enabled = GL_TRUE;
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array ispool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
        if(pool_id >= GL_MM_MAX_COMMON_POOL_QTY ||
           GL_NULL == g_a_p_glb_pool_table[pool_id])
        {
            GL_INC_MM_B_G_PARM_VLD_FAIL()

            *p_ecode=GL_MM_ERR_INV_POOL_ID;
            return GL_FAILURE;
        }
#else
        *p_ecode = *p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Congestion Management activity.
             */ 
            g_a_p_glb_pool_table[pool_id]->is_cngst_mgmt_enabled = GL_FALSE;
        }
        else
        {
            /*
             * Enable Congestion Management activity.
             */ 
            g_a_p_glb_pool_table[pool_id]->is_cngst_mgmt_enabled = GL_TRUE;
        }
    }
    return GL_SUCCESS;
#endif
}




/*****************************************************************************
* Function name    : gl_mm_set_cngst_mgmt_activity_for_task_pools
* Arguments        : gw_U32bit          task_id,
*                    gl_pool_id_t       pool_id,
*                    gw_boolen_t        is_enabled,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Enable/Disable Congestion Management activity.  
* Functions called :
*****************************************************************************/
gl_return_t  
gl_mm_set_cngst_mgmt_activity_for_task_pools

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_pool_id_t              pool_id,
      gl_boolean_t              is_enabled,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, pool_id, is_enabled, p_ecode )
      gw_U32bit                 task_id;
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
             * Disable Congestion Management activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
                if(g_a_p_app_sp_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
                {
                    /*
                     * Pool is not created for pool-id = i.
                     */ 
                    continue;
                }
                g_a_p_app_sp_pool_table[i]->is_cngst_mgmt_enabled = GL_FALSE;
            }
        }
        else
        {
            /*
             * Enable Congestion Management activity.
             */ 
            for(i=0; i < GL_MM_MAX_TASK_POOL_QTY; i++)
            {
                if(g_a_p_app_sp_pool_table[i]->p_buf_region == (gw_S8bit *)GL_NULL)
                {
                    /*
                     * Pool is not created for pool-id = i.
                     */ 
                    continue;
                }
                g_a_p_app_sp_pool_table[i]->is_cngst_mgmt_enabled = GL_TRUE;
            }

        }
    }
    else
    {
        /*
         * As at time of pool creation index to array + 1 is returned as 
         * pool-id, So actual index to pool-array ispool_id - 1.
         */ 
        pool_id = (gl_pool_id_t)(pool_id - 1);
#ifdef GL_STATIC_VALIDATION_ENABLED
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
        *p_ecode = *p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

        if( GL_FALSE == is_enabled)
        {
            /*
             * Disable Congestion Management activity.
             */ 
            g_a_p_app_sp_pool_table[pool_id]->is_cngst_mgmt_enabled = GL_FALSE;
        }
        else
        {
            /*
             * Enable Congestion Management activity.
             */ 
            g_a_p_app_sp_pool_table[pool_id]->is_cngst_mgmt_enabled = GL_TRUE;
        }
    }
    return GL_SUCCESS;
#endif
}

