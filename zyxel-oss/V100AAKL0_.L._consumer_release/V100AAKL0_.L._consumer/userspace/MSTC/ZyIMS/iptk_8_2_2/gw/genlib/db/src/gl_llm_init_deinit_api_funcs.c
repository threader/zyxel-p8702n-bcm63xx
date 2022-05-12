/******************************************************************************
*
* FUNCTION    : Generic Libraries - List Manager implementation source.
*
*******************************************************************************
*
* FILENAME    : gl_llm_init_deinit_api_funcs.c
*
* DESCRIPTION : This file contains the implementation routines of linked list
*               and work list in the generic library.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* Oct'99     Kapil Nayar    HSS:40600026      Generic Libraries
*                           HSS:40600036      (Gateways)
* 21/01/2001 Sachin Mittal                    Reorganised and moved in common 
* 18/08/2001 Prashant Jain  SPR 2276          Compiling with CC on Solaris.
* 04/10/2001 Ravi Jindal    SPR-2751          Remove warnings with gcc,cc,
*                                             CC on Solaris.
* 18/10/2001 Ravi Jindal    SPR-2788          Packaging of Genlib-Code.
* 19/10/2001 Ravi Jindal    SPR-2802          Adding Pre-ANSI-C features.
* 30/10/2001 Ravi Jindal    SPR-2744          Adding de-init functions.
* 17/11/2001 Ravi Jindal    SPR-3107          Incoprate the defects and change
*                                             cosmmetics of file.
*                                             
* 18/03/2002 Ravi Jindal    SPR-3958          Enhancements for Rel-1.0.
*                                             
* 25/06/2002 Saurabh Kr.    SPR-3958          Enhancements for Rel-1.0.
*            Sharma
*
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

#include "gl_lm.h"

/*******************************************************************************
* Function name    : gl_lm_init
* Arguments        : void
* Return Value     : gl_return_t
* Functionality    : Initializes the generic libraries list manager.
* Functions called :
*******************************************************************************/
gl_return_t
gl_lm_init( void )
/******************************************************************************/
{
    gw_U32bit                    i;
    gl_error_t                  ecode;

    if(llm_mgr_init_flag == GL_FALSE)
    {
        if(GL_WLM_MAX_INDEX_VALUE < GL_WLM_MAX_RECORDS)
        {
            /*
             * As value of index used for Work List nodes is in the 
             * range of 1 to GL_WLM_MAX_INDEX_VALUE and if 
             * maximum number of records in the Work List is more 
             * GL_WLM_MAX_INDEX_VALUE, the index may get repeated.
             */ 
            return GL_FAILURE;
        }
        /*
         * Create semaphores for MT safe operation.
         */
#ifdef GL_MT_ENABLED
        if(GL_FAILURE == gl_create_mutex (&lm_mgr_sem, &ecode))
        {
            GL_INC_LLM_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#ifdef GL_STATS_ENABLED
        g_llm_stat_init = GL_TRUE;

        if(GL_FAILURE == gl_create_mutex(&g_llm_stat_sem, &ecode))
        {
            GL_INC_LLM_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#endif /* Endif for GL_STATS_ENABLED */
        
#else
        ecode = 0;
#endif
        /*
         * Initialize the link list manager pointers.
         */
        for(i=0;i<GL_LLM_MAX_LIST; i++)
        {
            g_a_p_link_list[i] = GL_NULL;
        }

        /*
         * Initialize the work list manager parameters.
         */
        for(i=0;i<GL_WLM_MAX_LIST; i++)
        {
            g_a_p_work_list[i] = GL_NULL;
        }
        llm_mgr_init_flag = GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}


/*****************************************************************************
* Function name    : gl_llm_create_link_list
* Arguments        : gw_U32bit                  task_id, 
*                    gl_list_t                  *p_link_list_id,
*                    gl_llm_free_element_fn_t   p_elem_free_fn,
*                    gl_pvoid_t                 p_link_list_arg,
*                    gl_error_t                 *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the linked list structure and initializes the 
*                    list structure elements.
* Functions called :
******************************************************************************/
gl_return_t
gl_llm_create_link_list

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 *p_link_list_id,
      gl_llm_free_element_fn_t  p_elem_free_fn,
      gl_pvoid_t                p_link_list_arg,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, p_link_list_id, p_elem_free_fn, p_link_list_arg, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 *p_link_list_id;
      gl_llm_free_element_fn_t  p_elem_free_fn;
      gl_pvoid_t                p_link_list_arg;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{

    gw_U16bit                   local_index;
    gw_U16bit                   link_list_id=GL_NULL;

    /*
     * As Pointer to element memory free function can not be NULL.
     */ 
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_elem_free_fn)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_ELEM_FREE_FUNC_PTR;
        return GL_FAILURE;
    }
    /*
     * Validate the pointers passed as argument.
     */
    if(p_link_list_id == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * Find the free linked list index
     */
    for(local_index=0; local_index<GL_LLM_MAX_LIST; local_index++)
    {
        if(g_a_p_link_list[local_index] == GL_NULL)
        {
            link_list_id = local_index;
            break;
        }
    }
    /*
     * If not found return error
     */
    if(local_index == GL_LLM_MAX_LIST)
    {
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        *p_ecode =GL_LLM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }
    /*
     * Allocate Memory for the Link-list Header 
     */
    g_a_p_link_list[link_list_id] = (gl_llm_list_t*)SYS_MALLOC(sizeof
            (gl_llm_list_t));
    if(GL_NULL == g_a_p_link_list[link_list_id])
    {
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        
        GL_INC_LLM_G_MALOC_FAIL()
            
        *p_ecode =GL_LLM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }
    
    
#ifdef GL_TOTAL_MT_SAFE
    if(GL_FAILURE == gl_create_mutex (&(g_a_p_link_list [link_list_id]->llm_sem)
        , p_ecode))
    {
       /*
        * Create mutex for mutual exclusion between multiple threads
        * for accessing same link-list.
        */
        GL_INC_LLM_G_MUTX_FAIL()

        SYS_FREE(g_a_p_link_list [link_list_id]);
        g_a_p_link_list [link_list_id] = GL_NULL;
        
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        return GL_FAILURE;

    }
#endif /* End of GL_TOTAL_MT_SAFE */

#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_llm_stat_allocte_memory(link_list_id, p_ecode))
    {
        GL_INC_LLM_G_MALOC_FAIL()

        SYS_FREE(g_a_p_link_list [link_list_id]);
        g_a_p_link_list [link_list_id] = GL_NULL;

        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif    
    GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * And set the linked list parameters
     */
    g_a_p_link_list[link_list_id]->elem_qty = GL_NULL;
    g_a_p_link_list[link_list_id]->owner_task_id = task_id;
    g_a_p_link_list[link_list_id]->p_first = GL_NULL;
    g_a_p_link_list[link_list_id]->p_last = GL_NULL;
    g_a_p_link_list[link_list_id]->p_elem_free_fn = p_elem_free_fn;
    g_a_p_link_list[link_list_id]->p_link_list_arg = p_link_list_arg;
    *p_link_list_id = link_list_id;
    
    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_delete_link_list
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          link_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the linked list and resets the list structure 
*                    elements if the linked list is empty.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_delete_link_list

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( link_list_id >= GL_LLM_MAX_LIST
        || g_a_p_link_list[link_list_id] == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_LINK_LIST; 
        return GL_FAILURE;
    }
    if( g_a_p_link_list[link_list_id]->owner_task_id != task_id)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * If linked list not empty return error
     */
    if( g_a_p_link_list[link_list_id]->elem_qty != 0)
    {
        GL_INC_LLM_G_DEL_RQ_LIST_BSY() 
            
        *p_ecode = GL_LLM_ERR_NOT_EMPTY;
        return GL_FAILURE;
    }
    
    GL_MUTEX_LOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * Delete the existing MUTEX Lock
     * for the sprcified Link-list.
     */
    GL_MT_DELETE(&(g_a_p_link_list [link_list_id]->llm_sem),p_ecode,
                    GL_FAILURE);

#ifdef GL_STATS_ENABLED
    gl_llm_stat_free_memory(link_list_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

    /*
     * Reset the Link-list Header Pointer
     */
    SYS_FREE(g_a_p_link_list[link_list_id]);
    g_a_p_link_list[link_list_id] = GL_NULL;
    
    GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_wlm_create_work_list
* Arguments        : gw_U32bit                  task_id, 
*                    gl_list_t                  *p_work_list_id,
*                    gl_wlm_free_record_fn_t    p_record_free_fn,
*                    gl_pvoid_t                 p_work_list_arg,
*                    gl_error_t                 *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the work list structure and initializes the 
*                    list structure elements.
* Functions called :
*******************************************************************************/
gl_return_t
gl_wlm_create_work_list

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 *p_work_list_id,
      gl_wlm_free_record_fn_t   p_record_free_fn,
      gl_pvoid_t                p_work_list_arg,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, p_work_list_id, p_record_free_fn, p_work_list_arg, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 *p_work_list_id;
      gl_wlm_free_record_fn_t   p_record_free_fn;
      gl_pvoid_t                p_work_list_arg;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{

    gw_U16bit local_index;
    gw_U16bit work_list_id=GL_NULL;

    /*
     * As Pointer to record memory free function can not be NULL.
     */ 
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_record_free_fn)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_RECORD_FREE_FUNC_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * Find the index of the unallocated work list.
     */
    for(local_index=0; local_index<GL_WLM_MAX_LIST; local_index++)
    {
        if(g_a_p_work_list[local_index] == GL_NULL)
        {
            work_list_id = local_index;
            break;
        }
    }
    /*
     * If local_index not found.
     */
    if(local_index == GL_WLM_MAX_LIST)
    {
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        
        *p_ecode =GL_WLM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }
    /*
     * Else allocate Memory for Work list Manager Header.
     */
    g_a_p_work_list[work_list_id] = (gl_wlm_list_t*)SYS_MALLOC(sizeof
            (gl_wlm_list_t));
    
    if(GL_NULL == g_a_p_work_list[work_list_id])
    {
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        
        GL_INC_LLM_G_MALOC_FAIL()
            
        *p_ecode =GL_WLM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }
    
#ifdef GL_TOTAL_MT_SAFE
    if(GL_FAILURE == gl_create_mutex(&g_a_p_work_list[work_list_id]->lm_sem,
        p_ecode))
    {
        GL_INC_LLM_G_MUTX_FAIL()

        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        *p_ecode =GL_WLM_CREATE_MUTEX_FAILED;
        return GL_FAILURE;
    }
#endif
    GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);

    /*
     * Set the parameters of the work list.
     */
    g_a_p_work_list[work_list_id]->elem_qty = GL_NULL;
    g_a_p_work_list[work_list_id]->owner_task_id = task_id;
    g_a_p_work_list[work_list_id]->p_first = GL_NULL;
    g_a_p_work_list[work_list_id]->p_last = GL_NULL;
    g_a_p_work_list[work_list_id]->new_local_index = 0;
    g_a_p_work_list[work_list_id]->p_record_free_fn = p_record_free_fn;
    g_a_p_work_list[work_list_id]->p_work_list_arg = p_work_list_arg;   
    *p_work_list_id = work_list_id;
    
    return GL_SUCCESS;
}



/*******************************************************************************
* Function name    : gl_wlm_delete_work_list
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          work_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the work list and resets the list structure 
*                    elements if the work list is empty.
* Functions called :
*******************************************************************************/
gl_return_t
gl_wlm_delete_work_list

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 work_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 work_list_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( work_list_id >= GL_WLM_MAX_LIST
        || g_a_p_work_list[work_list_id] == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_WORK_LIST; 
        return GL_FAILURE;
    }
    if( g_a_p_work_list[work_list_id]->owner_task_id != task_id)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_TASK_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * Validate against the work list containing some records.
     */
    if( g_a_p_work_list[work_list_id]->elem_qty != 0)
    {
        GL_INC_LLM_G_DEL_RQ_LIST_BSY()
            
        *p_ecode = GL_WLM_ERR_NOT_EMPTY;
        return GL_FAILURE;
    }
   
    GL_MUTEX_LOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);

    GL_MT_DELETE(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
    
    /*
     * Reset the work list Header Pointer.
     */
    SYS_FREE(g_a_p_work_list[work_list_id]);
    g_a_p_work_list[work_list_id] = GL_NULL;  
 
    GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    
    return GL_SUCCESS;

}



/*****************************************************************************
* Function name    : gl_lm_destroy_all_lists
* Arguments        : gw_U8bit           is_forceful_cleanup
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the link-lists and work lists
*                    and free all the corresponding occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t  
gl_lm_destroy_all_lists

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
    gw_U16bit           link_list_id;
    gw_U16bit           work_list_id;
    gw_boolean          is_not_empty_flag = GL_FALSE; 

    GL_MUTEX_LOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
    
    for(link_list_id = 0; link_list_id < GL_LLM_MAX_LIST; link_list_id++)
    {
        if( g_a_p_link_list[link_list_id] == GL_NULL)
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE ) &&
           (g_a_p_link_list[link_list_id]->elem_qty != GL_NULL))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        if(GL_NULL != g_a_p_link_list[link_list_id]->elem_qty)
        {
            if(GL_FAILURE == gl_llm_remove_all_elements(
                g_a_p_link_list[link_list_id]->owner_task_id, link_list_id,
                p_ecode))
            {
                GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
                return GL_FAILURE;
            }
        }
        g_a_p_link_list[link_list_id]->elem_qty = 0;

        GL_MT_DELETE(&(g_a_p_link_list[link_list_id]->llm_sem), p_ecode,
                GL_FAILURE);

#ifdef GL_STATS_ENABLED
        gl_llm_stat_free_memory(link_list_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        /*
         * Free the memory for Link-list Header-Pointer
         */
        SYS_FREE(g_a_p_link_list[link_list_id]);
        g_a_p_link_list[link_list_id] = GL_NULL;

    } /* End of For Loop */
    
    for(work_list_id = 0; work_list_id < GL_WLM_MAX_LIST; work_list_id++)
    {
        if( g_a_p_work_list[work_list_id] == GL_NULL)
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE ) &&
           (g_a_p_work_list[work_list_id]->elem_qty != GL_NULL))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        if(GL_NULL != g_a_p_work_list[work_list_id]->elem_qty)
        {
            if(GL_FAILURE == gl_wlm_remove_all_records(
                g_a_p_work_list[work_list_id]->owner_task_id, work_list_id,
                p_ecode))
            {
                GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
                return GL_FAILURE;
            }
        }
        g_a_p_work_list[work_list_id]->elem_qty = 0;
        
        GL_MT_DELETE(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);

        /*
         * Free the memory for Work-list Header-Pointer
         */
        SYS_FREE(g_a_p_work_list[work_list_id]);
        g_a_p_work_list[work_list_id] = GL_NULL;
        
    }
    
    if( is_not_empty_flag == GL_TRUE)
    {
        /*
         * Some lists are not empty so not destroyed forcefully. 
         */
        GL_MUTEX_UNLOCK(&lm_mgr_sem, p_ecode, GL_FAILURE);
        
        *p_ecode = GL_LM_ERR_NOT_EMPTY_LIST;
        return GL_FAILURE;
    }
    else
    {
        GL_MUTEX_DELETE(&lm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
#ifdef GL_MT_ENABLED
        if(GL_TRUE == g_llm_stat_init)
        {
            gl_delete_mutex(&g_llm_stat_sem, p_ecode);
        }
        g_llm_stat_init = GL_FALSE;
#endif
#endif /* Endif for GL_STATS_ENABLED */
        llm_mgr_init_flag = GL_FALSE;
        return GL_SUCCESS;
     }
}

