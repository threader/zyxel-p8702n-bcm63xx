/*******************************************************************************
*
* FUNCTION    : Generic Libraries - List Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_llm_advncd_list_oprns_api.c
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
* Function name     : gl_wlm_add_record
* Arguments         : gw_U32bit         task_id,
*                     gl_list_t         work_list_id
*                     gl_pvoid_t        p_record, 
*                     gl_wl_index_t     *p_local_index,
*                     gl_error_t        *p_ecode
* Return Value      : gl_return_t
* Functionality     : Adds an record to the work list and allocates a new 
*                     incremental local_index number to the same.
* Functions called  :
*******************************************************************************/
gl_return_t
gl_wlm_add_record

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 work_list_id,
      gl_pvoid_t                p_record, 
      gl_wl_index_t             *p_local_index,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, p_record, p_local_index, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 work_list_id; 
      gl_pvoid_t                p_record; 
      gl_wl_index_t             *p_local_index;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_wlm_header_t* p_rec = (gl_wlm_header_t*) p_record;

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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_record == GL_NULL) 
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
    
    /*
     * Check whether work list is full and return error if it is so.
     */
    if( g_a_p_work_list[work_list_id]->elem_qty == GL_WLM_MAX_RECORDS)
    {
        *p_ecode = GL_WLM_ERR_LIST_FULL;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        return GL_FAILURE;
    }
    /*
     * Set the work list id in the header of the record
     */
    p_rec->list_id=work_list_id;
    /*
     * As the new index for node to be allocated, should be in range of
     * 1 to GL_WLM_MAX_indexX_VALUE.
     * And it is Assumed that after getting GL_WLM_MAX_indexX_VALUE
     * the index value will rap up and no node with rapped value will bein 
     * the Work list.
     * e.g. If the index of last added node = GL_WLM_MAX_INDEX_VALUE
     * then net node will get index = 1.
     */
    g_a_p_work_list[work_list_id]->new_local_index = ((g_a_p_work_list
        [work_list_id]->new_local_index)%(GL_WLM_MAX_INDEX_VALUE))+1;
    
    /*
     * Return the local_index by setiing the new_local_inde in the pointer 
     * passed as argument for the purpose.
     */
    *p_local_index = p_rec->local_index= g_a_p_work_list[work_list_id]->
        new_local_index;
    if(g_a_p_work_list[work_list_id]->elem_qty == GL_NULL)
    {
        /*
         * If this would be the only record in the work list.
         */
        g_a_p_work_list[work_list_id]->elem_qty = 1;
        g_a_p_work_list[work_list_id]->p_first = g_a_p_work_list[work_list_id]
            ->p_last = p_rec;
        p_rec->p_next=p_rec->p_prev = p_rec;
    }
    else
    {
        /*
         * If the work list already contains records.
         */
        (g_a_p_work_list[work_list_id]->elem_qty)++;
        p_rec->p_prev=g_a_p_work_list[work_list_id]->p_last;
        p_rec->p_next= g_a_p_work_list[work_list_id]->p_last->p_next;
        g_a_p_work_list[work_list_id]->p_last->p_next->p_prev=p_rec;
        g_a_p_work_list[work_list_id]->p_last->p_next=p_rec;
        g_a_p_work_list[work_list_id]->p_last=p_rec;
    }
    
    GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
        
    return GL_SUCCESS;
}



/*******************************************************************************
* Function name    : gl_wlm_get_record
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          work_list_id, 
*                    gl_wl_index_t      wlm_local_index, 
*                    gl_pvoid_t         *p_p_record, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the record from the work list with local_index number.
* Functions called :
*******************************************************************************/
gl_return_t
gl_wlm_get_record

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 work_list_id,
      gl_wl_index_t             wlm_local_index,
      gl_pvoid_t                *p_p_record,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, wlm_local_index, p_p_record, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 work_list_id;
      gl_wl_index_t             wlm_local_index;
      gl_pvoid_t                *p_p_record;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_wl_index_t               first_local_index;
    gl_wl_index_t               last_local_index;
    *p_p_record = GL_NULL;

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
    /*
     * Check for the index passed.
     */
    if(GL_WLM_MAX_INDEX_VALUE < wlm_local_index)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_INDEX;
        return GL_FAILURE;
    }
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_record == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_INV_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
    /*
     * Validate against work list being empty.
     */
    if( g_a_p_work_list[work_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode =  GL_WLM_ERR_NO_RECORD;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                GL_FAILURE);
        
        return GL_FAILURE;
    }
    first_local_index = g_a_p_work_list[work_list_id]->p_first->local_index;
    last_local_index = g_a_p_work_list[work_list_id]->p_last->local_index;
    /*
     * If local_index required is the first record.
     */
    if(first_local_index == wlm_local_index)
    {
        *p_p_record = g_a_p_work_list[work_list_id]->p_first;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    /*
     * If local_index required is the last record.
     */
    if(last_local_index == wlm_local_index)
    {
        *p_p_record= g_a_p_work_list[work_list_id]->p_last;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }

    if(g_a_p_work_list[work_list_id]->elem_qty <= 2)
    {
        *p_ecode = GL_WLM_ERR_INDEX_NOT_FOUND;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                GL_FAILURE);
        
        return GL_FAILURE;
    }
    
    if(first_local_index < last_local_index)
    {
        /* 
         * If local_index of the first elemnt is less than the last record.
        */
        if((wlm_local_index < first_local_index) 
            || (wlm_local_index > last_local_index))
        {
            /*
             * If required local_index is not in the range between first and 
             * the last local_index return error.
             */
            *p_ecode =  GL_WLM_ERR_INDEX_NOT_FOUND;
        
            GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                    GL_FAILURE);
        
            return GL_FAILURE;
        }
        /*
         * If required local_index closer to the first local_inde search 
         * from head.
         */
        if((gw_S32bit)(wlm_local_index - first_local_index)
            < (gw_S32bit)(last_local_index - wlm_local_index))
        {
            *p_p_record = llm_search_from_first( work_list_id, wlm_local_index);
        }
        else
        {
            /*
             * Search from the tail.
             */
            *p_p_record = llm_search_from_last( work_list_id, wlm_local_index);
        }
    }
    else if(first_local_index > last_local_index)
    {
        /*
         * If local_index of the first elemnt is more than the last record
         */
        if((wlm_local_index < first_local_index)
            && (wlm_local_index > last_local_index))
        {
            /* 
             * If required local_index is not in the range between first and 
             * the last local_index return error
             */
            *p_ecode =  GL_WLM_ERR_INDEX_NOT_FOUND;
        
            GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                    GL_FAILURE);
        
            return GL_FAILURE;
        }
        
        if(  ((wlm_local_index < last_local_index) &&
              ((gw_S32bit)(last_local_index - wlm_local_index) 
              <(gw_S32bit)(GL_WLM_MAX_INDEX_VALUE - first_local_index + 
                wlm_local_index)))
           ||((wlm_local_index > first_local_index) &&
             ((gw_S32bit)(wlm_local_index - first_local_index) 
             >(gw_S32bit)(GL_WLM_MAX_INDEX_VALUE - wlm_local_index +
                last_local_index))) 
          )
        {
            *p_p_record = llm_search_from_last( work_list_id, wlm_local_index);
        }
        else
        {
            *p_p_record = llm_search_from_first( work_list_id, wlm_local_index);
        }
    }
        
    GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
        
    /*
     * If record not found return error.
     */
    if( *p_p_record != GL_NULL)
    {
        return GL_SUCCESS;
    }
    else
    {
        *p_ecode =  GL_WLM_ERR_INDEX_NOT_FOUND;
        return GL_FAILURE;
    }
}


/*****************************************************************************
* Function name    : gl_wlm_remove_record
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          work_list_id, 
*                    gl_wl_index_t      wlm_local_index, 
*                    gl_pvoid_t         *p_p_record,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the record from the work list with local_index
*                    number.
* Functions called :
*****************************************************************************/
gl_return_t
gl_wlm_remove_record

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 work_list_id,
      gl_wl_index_t             wlm_local_index, 
      gl_pvoid_t                *p_p_record, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, wlm_local_index, p_p_record, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 work_list_id;
      gl_wl_index_t             wlm_local_index;
      gl_pvoid_t                *p_p_record; 
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gl_wl_index_t           first_local_index;
    gl_wl_index_t           last_local_index;
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    /*
     * The implementation is same ecept that the record is removed from the 
     * list before returning success
     */
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
    /*
     * Check for the index passed.
     */
    if(GL_WLM_MAX_INDEX_VALUE < wlm_local_index)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_INDEX;
        return GL_FAILURE;
    }
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_record == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
    
    if( g_a_p_work_list[work_list_id]->elem_qty == 0)
    {
        *p_ecode = GL_WLM_ERR_NO_RECORD;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_FAILURE;
    }

    first_local_index = g_a_p_work_list[work_list_id]->p_first->local_index;
    last_local_index = g_a_p_work_list[work_list_id]->p_last->local_index;
    
    if(first_local_index == wlm_local_index)
    {
        *p_p_record = g_a_p_work_list[work_list_id]->p_first;
        llm_remove_head(work_list_id);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    
    if(last_local_index == wlm_local_index)
    {
        *p_p_record= g_a_p_work_list[work_list_id]->p_last;
        llm_remove_tail( work_list_id);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    
    if(g_a_p_work_list[work_list_id]->elem_qty <= 2)
    {
        *p_ecode = GL_WLM_ERR_INDEX_NOT_FOUND;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_FAILURE;
    }
    
    if(first_local_index < last_local_index)
    {
        if((wlm_local_index < first_local_index) 
            || (wlm_local_index > last_local_index))
        {
            *p_ecode = GL_WLM_ERR_INDEX_NOT_FOUND;
        
            GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                    GL_FAILURE);
        
            return GL_FAILURE;
        }
        
        if((gw_S32bit)(wlm_local_index - first_local_index) 
           < (gw_S32bit)(last_local_index - wlm_local_index))
        {
            *p_p_record = llm_search_from_first(work_list_id, wlm_local_index);
        }
        else
        {
            *p_p_record = llm_search_from_last(work_list_id, wlm_local_index);
        }
    }
    else if(first_local_index > last_local_index)
    {
        if((wlm_local_index < first_local_index)
            && (wlm_local_index > last_local_index))
        {
            *p_ecode = GL_WLM_ERR_INDEX_NOT_FOUND;
        
            GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                    GL_FAILURE);
        
            return GL_FAILURE;
        }
        
        if(  ((wlm_local_index < last_local_index) &&
              ((gw_S32bit)(last_local_index - wlm_local_index) 
              <(gw_S32bit)(GL_WLM_MAX_INDEX_VALUE - first_local_index + 
                wlm_local_index)))
           ||((wlm_local_index > first_local_index) &&
             ((gw_S32bit)(wlm_local_index - first_local_index) 
             >(gw_S32bit)(GL_WLM_MAX_INDEX_VALUE - wlm_local_index +
                last_local_index))) 
          )
        {
            *p_p_record = llm_search_from_last( work_list_id, wlm_local_index);
        }
        else
        {
            *p_p_record = llm_search_from_first( work_list_id, wlm_local_index);
        }
    }
    
    if( *p_p_record != GL_NULL)
    {
        llm_remove_ptr(work_list_id, (gl_wlm_header_t *)*p_p_record);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    else
    {
        *p_ecode = GL_WLM_ERR_INDEX_NOT_FOUND;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_FAILURE;
    }
}



/*****************************************************************************
* Function name    : gl_wlm_remove_record_ptr
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          work_list_id,
*                    gl_pvoid_t         p_record, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the record pointer from the work list.
* Functions called :
*****************************************************************************/
gl_return_t
gl_wlm_remove_record_ptr

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 work_list_id,
      gl_pvoid_t                p_record, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, p_record, p_ecode )
       gw_U32bit                task_id;
       gl_list_t                work_list_id;
       gl_pvoid_t               p_record;
       gl_error_t               *p_ecode;
#endif

/****************************************************************************/
{
    gl_wlm_header_t* p_rec = (gl_wlm_header_t*) p_record;

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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_record == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_WLM_ERR_INV_RECORD;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning if GL_STATIC_VALIDATION_ENABLED flag not defined.
     */
    task_id = task_id;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, GL_FAILURE);
    
    if(g_a_p_work_list[work_list_id]->elem_qty == 0)
    {
        /*
         * If record pointer is null or list is sempty return error.
         */
        *p_ecode =  GL_WLM_ERR_NO_RECORD;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_FAILURE;
    }
    
    if(g_a_p_work_list[work_list_id]->p_first == p_rec)
    {
        /*
         * If record pointer is the first record in the list
         * remove head.
         */
        llm_remove_head(work_list_id);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    if(g_a_p_work_list[work_list_id]->p_last == p_rec)
    {
        /*
         * If record pointer is the last record in the list
         * remove tail.
         */
        llm_remove_tail(work_list_id);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    if(g_a_p_work_list[work_list_id]->elem_qty > 2)
    {
        /*
         * Check if p_elem->p_next and p_elem->p_prev are valid.
         */
        if((GL_NULL == p_rec->p_prev) || (GL_NULL == p_rec->p_next))
        {
            *p_ecode =  GL_WLM_ERR_INV_RECORD;
        
            GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                    GL_FAILURE);
        
            return GL_FAILURE;
        }
        /* 
         * If number of records are more than two remove record pointer
         * from the list.
         */
        llm_remove_ptr(work_list_id, p_rec);
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                GL_FAILURE);
        
        return GL_SUCCESS;
    }
    else
    {
        /* 
         * If number of records not more than two report error.
         */
        *p_ecode =  GL_WLM_ERR_INV_RECORD;
        
        GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode,
                GL_FAILURE);
        
        return GL_FAILURE;
    }
}


/*****************************************************************************
* Function name    : gl_wlm_remove_all_records
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          work_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes all the records from the work list one by one.
*                    This function is called in a while loop only by the user
* Functions called :
*****************************************************************************/
gl_return_t
gl_wlm_remove_all_records

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

/****************************************************************************/
{
    gl_wlm_header_t             *p_rec;
    gl_wlm_header_t             *p_temp;
    gl_wlm_header_t             *p_next_rec;
    
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

    GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
        GL_FAILURE);

    if(GL_NULL != g_a_p_work_list[work_list_id]->p_first)
    {
        /*
         * If first element is not null call function pointed by
         * p_record_free_fn, to free the memory allocated for all
         * the stored record pointers one by one. 
         */
        p_rec = g_a_p_work_list[work_list_id]->p_first;
        p_temp = p_rec;
        
        do
        {
            p_next_rec = p_rec->p_next;
            /*
             * Invoke the function defined by user to free moemory of 
             * element (pointer to elements are stored in Link list).
             */
            if(GL_FAILURE == g_a_p_work_list[work_list_id]->p_record_free_fn(
                    g_a_p_work_list[work_list_id]->p_work_list_arg,
                    (gl_pvoid_t)p_rec, p_rec->local_index, p_ecode))
            {
                GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
                    GL_FAILURE);

                return GL_FAILURE;
            }
            
            p_rec = p_next_rec;
            
        }while(p_next_rec != p_temp);
    }
        
    g_a_p_work_list[work_list_id]->elem_qty = 0;
    g_a_p_work_list[work_list_id]->p_first = GL_NULL;
    g_a_p_work_list[work_list_id]->p_last = GL_NULL;    
    g_a_p_work_list[work_list_id]->new_local_index = 1;
        
    GL_MT_UNLOCK(&(g_a_p_work_list[work_list_id]->lm_sem), p_ecode, 
        GL_FAILURE);
    
    return GL_SUCCESS;
}


/*****************************************************************************
* Function name    : gl_wlm_get_record_count
* Arguments        : gl_list_t          work_list_id,
*                    gw_U32bit          *p_number_of_records,
*                    gl_error_t         *p_ecode
* Return Value     : gw_S32bit
* Functionality    : Gets the count of the records in the work list 
*                    specified.
* Functions called :
*****************************************************************************/
gl_return_t
gl_wlm_get_record_count

#ifdef _ANSI_C_
    ( gl_list_t                 work_list_id, 
      gw_U32bit                 *p_number_of_records,
      gl_error_t                *p_ecode
    )
#else
    ( work_list_id, p_number_of_records, p_ecode )
      gl_list_t                 work_list_id;
      gw_U32bit                 *p_number_of_records;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_number_of_records)
    {
        *p_ecode = GL_WLM_ERR_NULL_NUM_OF_REC_PTR;
        return GL_FAILURE;
    }
    if( work_list_id >= GL_WLM_MAX_LIST
        || g_a_p_work_list[work_list_id] == GL_NULL)
    {
        *p_ecode = GL_WLM_ERR_INV_WORK_LIST; 
        return GL_FAILURE;
    }
#else
    /*
     * To remove Warning.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    *p_number_of_records = g_a_p_work_list[work_list_id]->elem_qty;
    
    return GL_SUCCESS;
}


/*****************************************************************************
* Function name    : gl_wlm_is_empty
* Arguments        : gl_list_t          work_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_boolean
* Functionality    : Returns true only if the linked list contains zero 
*                    records
* Functions called :
*****************************************************************************/
gl_boolean gl_wlm_is_empty

#ifdef _ANSI_C_
    ( gl_list_t                 work_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( work_list_id, p_ecode )
      gl_list_t                 work_list_id;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( work_list_id >= GL_WLM_MAX_LIST
        || g_a_p_work_list[work_list_id] == GL_NULL)
    {
        *p_ecode = GL_WLM_ERR_INV_WORK_LIST; 
        return GL_FALSE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if( g_a_p_work_list[work_list_id]->elem_qty == GL_NULL)
    {
        return GL_TRUE;
    }
    *p_ecode = GL_WLM_ERR_NOT_EMPTY;
    return GL_FALSE;

}

