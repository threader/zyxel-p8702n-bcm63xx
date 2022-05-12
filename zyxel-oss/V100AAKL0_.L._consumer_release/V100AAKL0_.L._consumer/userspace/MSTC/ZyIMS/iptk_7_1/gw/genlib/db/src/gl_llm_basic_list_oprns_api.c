/*******************************************************************************
*
* FUNCTION    : Generic Libraries - List Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_llm_basic_list_oprns_api.c
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
* Function name    : gl_llm_get_head
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         *p_p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : gets the first element in the linked list from the head
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_get_head

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 link_list_id, 
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                *p_p_element;
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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_element == GL_NULL)
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode, GL_FAILURE);
    /*
     * Get the first element in the linked list
     */
    *p_p_element = g_a_p_link_list[link_list_id]->p_first;
    /*
     * Return error if linked list is empty
     */
    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                        GL_FAILURE);
        return GL_FAILURE;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode, 
            GL_FAILURE);
    
    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_get_tail
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the last element in the linked list from the head
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_get_tail

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 link_list_id,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                *p_p_element;
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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_element == GL_NULL)
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
    /*
     * Get the last element in the linked list
     */
    *p_p_element = g_a_p_link_list[link_list_id]->p_last;

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        /*
         * Return error if the linked list empty
         */
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
        return GL_FAILURE;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_remove_element
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the specified element from the linked list 
*                    specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_remove_element

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id, 
      gl_pvoid_t                p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*)p_element;

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
    /*
     * Validate the pointers passed as argument.
     */
    if((p_element == GL_NULL) || (p_elem->list_id != link_list_id))
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
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    if( p_element == g_a_p_link_list[link_list_id]->p_first)
    {
        /*
         * If element is the first element in the list.
         */
        if(g_a_p_link_list[link_list_id]->elem_qty == 1
            && p_element == g_a_p_link_list[link_list_id]->p_last)
        {
            /*
             * If only one element in the list presently.
             */
            g_a_p_link_list[link_list_id]->elem_qty=0;
            g_a_p_link_list[link_list_id]->p_first
                =g_a_p_link_list[link_list_id]->p_last=GL_NULL;
            p_elem->p_next=p_elem->p_prev=GL_NULL;

            GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

            return GL_SUCCESS;
        }
        else
        {
            /*
             * Decrement the count of elements.
             */
            (g_a_p_link_list[link_list_id]->elem_qty)--;
            g_a_p_link_list[link_list_id]->p_first=p_elem->p_next;
            p_elem->p_next->p_prev= GL_NULL;
            p_elem->p_next=p_elem->p_prev=GL_NULL;

            GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

            return GL_SUCCESS;
        }
    }
    /*
     * Decrement the count of elements.
     */
    (g_a_p_link_list[link_list_id]->elem_qty)--;
    if( p_element == g_a_p_link_list[link_list_id]->p_last)
    {
        /*
         * If element is the last element of the linked list.
         */
        g_a_p_link_list[link_list_id]->p_last = p_elem->p_prev;
        p_elem->p_prev->p_next = GL_NULL;
        p_elem->p_prev=p_elem->p_next=GL_NULL;
    }
    else
    {
        /*
         * Any other element in the linked list.
         */
        p_elem->p_prev->p_next=p_elem->p_next; 
        p_elem->p_next->p_prev=p_elem->p_prev;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}




/*******************************************************************************
* Function name    : gl_llm_remove_head
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the first element from the head of the 
*                    linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_remove_head

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 link_list_id,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem;
    *p_p_element = GL_NULL;

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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_element == GL_NULL)
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);    

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
     
     p_elem = g_a_p_link_list[link_list_id]->p_first;

    /*
     * Get the head of the linked list.
     */
    *p_p_element = (gl_pvoid_t)g_a_p_link_list[link_list_id]->p_first;

    if(g_a_p_link_list[link_list_id]->elem_qty == 1)
    {
        /*
         * If only one elment in the list.
         */
        g_a_p_link_list[link_list_id]->elem_qty=GL_NULL;
        g_a_p_link_list[link_list_id]->p_first = g_a_p_link_list[link_list_id]
            ->p_last = GL_NULL;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * If more than one element in the list.
         */
        (g_a_p_link_list[link_list_id]->elem_qty)--;
        g_a_p_link_list[link_list_id]->p_first= p_elem->p_next;
        p_elem->p_next->p_prev= GL_NULL;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}



/*******************************************************************************
* Function name    : gl_llm_remove_tail
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,
*                    gl_pvoid_t         *p_p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes the last element from the head of the 
*                    linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_remove_tail

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 link_list_id,
      gl_pvoid_t                *p_p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                *p_p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem;
    *p_p_element=GL_NULL;
    
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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_p_element == GL_NULL)
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
    
    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }

    p_elem = g_a_p_link_list[link_list_id]->p_last; 

    /*
     * Get the tail of the linked list.
     */
    *p_p_element= (gl_pvoid_t)g_a_p_link_list[link_list_id]->p_last;

    if(g_a_p_link_list[link_list_id]->elem_qty == 1)
    {
        /*
         * If only one elment in the list.
         */
        g_a_p_link_list[link_list_id]->elem_qty=GL_NULL;
        g_a_p_link_list[link_list_id]->p_last = g_a_p_link_list[link_list_id]
            ->p_first = GL_NULL;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * If more than one element in the list.
         */
        (g_a_p_link_list[link_list_id]->elem_qty)--;
        g_a_p_link_list[link_list_id]->p_last= p_elem->p_prev;
        p_elem->p_prev->p_next= GL_NULL;
        p_elem->p_prev=p_elem->p_next=GL_NULL;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}




/*******************************************************************************
* Function name    : gl_llm_add_head
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,
*                    gl_pvoid_t         p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Adds an element at the head of the linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_add_head

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id,
      gl_pvoid_t                p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;

    GL_INC_LLM_G_ADD_NODE_RQ() 

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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_element == GL_NULL) 
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
    p_ecode = p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    if(g_a_p_link_list[link_list_id]->elem_qty == GL_LLM_MAX_ELEMENTS)
    {
        *p_ecode = GL_LLM_ERR_LIST_FULL;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);
        
        GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)
            
        return GL_FAILURE;
    }
    
    p_elem->list_id = link_list_id;
    
    if(g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        /*
         * If this is the first element in the list.
         */
        g_a_p_link_list[link_list_id]->elem_qty=1;
        g_a_p_link_list[link_list_id]->p_first = g_a_p_link_list[link_list_id]
            ->p_last = p_elem;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * Else if other elements exist in the list.
         */
        (g_a_p_link_list[link_list_id]->elem_qty)++;
        p_elem->p_next = g_a_p_link_list[link_list_id]->p_first;
        p_elem->p_prev = GL_NULL;
        g_a_p_link_list[link_list_id]->p_first->p_prev = p_elem;
        g_a_p_link_list[link_list_id]->p_first = p_elem;
    }
    GL_LLM_C_PEAK_NODE_ADDED(link_list_id, g_a_p_link_list[link_list_id]->
        elem_qty) 
        
    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode, 
            GL_FAILURE);

    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_add_tail
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,
*                    gl_pvoid_t         p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Adds an element at the tail of the linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_add_tail

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id, 
      gl_pvoid_t                p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;

    GL_INC_LLM_G_ADD_NODE_RQ() 

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
    /*
     * Validate the pointers passed as argument.
     */
    if(p_element == GL_NULL)
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
    p_ecode = p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    if(g_a_p_link_list[link_list_id]->elem_qty == GL_LLM_MAX_ELEMENTS)
    {
        *p_ecode = GL_LLM_ERR_LIST_FULL;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);
        
        GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)

        return GL_FAILURE;
    }

    p_elem->list_id=link_list_id;

    if(g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        /*
         * If this is the first element in the list.
         */
        g_a_p_link_list[link_list_id]->elem_qty=1;
        g_a_p_link_list[link_list_id]->p_first = g_a_p_link_list[link_list_id]
            ->p_last = p_elem;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * Else if other elements exist in the list.
         */
        (g_a_p_link_list[link_list_id]->elem_qty)++;
        p_elem->p_prev = g_a_p_link_list[link_list_id]->p_last;
        p_elem->p_next = GL_NULL;
        g_a_p_link_list[link_list_id]->p_last->p_next = p_elem;
        g_a_p_link_list[link_list_id]->p_last = p_elem;
    }
    GL_LLM_C_PEAK_NODE_ADDED(link_list_id, g_a_p_link_list[link_list_id]->
        elem_qty) 
        
    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode, 
            GL_FAILURE);

    return GL_SUCCESS;
}



/*******************************************************************************
* Function name    : gl_llm_get_next
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,
*                    gl_pvoid_t         p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : Gets the next element of the specified p_element 
*                    from the linked list specified.
* Functions called :
*******************************************************************************/
gl_pvoid_t
gl_llm_get_next

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id,
      gl_pvoid_t                p_element, 
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( link_list_id >= GL_LLM_MAX_LIST
        || g_a_p_link_list[link_list_id] == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_LINK_LIST; 
        return GL_NULL;
    }
    if( g_a_p_link_list[link_list_id]->owner_task_id != task_id)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_TASK_ID;
        return GL_NULL;
    }
    /*
     * Validate the pointers passed as argument.
     */
    if((p_element == GL_NULL) || (p_elem->list_id != link_list_id))
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE); 

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                        GL_FAILURE);
        return GL_FAILURE;
    }
    /*
     * Validate the element against the last element.
     */
    if( p_elem == g_a_p_link_list[link_list_id]->p_last)
    {
        *p_ecode = GL_LLM_ERR_LAST_ELEM;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_NULL;
    }
    /*
     * Validate the pointer passed as argument.
     */
    if((p_elem == GL_NULL) || (p_elem->p_next == GL_NULL))
    {
        *p_ecode = GL_LLM_ERR_INV_PTR;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_NULL;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return (p_elem->p_next);
}


/*******************************************************************************
* Function name    : gl_llm_get_prev
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         p_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : Gets the previous element of the specified p_element 
*                    from the linked list specified.
* Functions called :
*******************************************************************************/
gl_pvoid_t
gl_llm_get_prev

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id,
      gl_pvoid_t                p_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( link_list_id >= GL_LLM_MAX_LIST
        || g_a_p_link_list[link_list_id] == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_LINK_LIST; 
        return GL_NULL;
    }
    if( g_a_p_link_list[link_list_id]->owner_task_id != task_id)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_TASK_ID;
        return GL_NULL;
    }
    /*
     * Validate the pointers passed as argument.
     */
    if((p_element == GL_NULL) || (p_elem->list_id != link_list_id))
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                        GL_FAILURE);
        return GL_FAILURE;
    }
    /*
     * Validate the element against the first element.
     */
    if( p_elem == g_a_p_link_list[link_list_id]->p_first)
    {
        *p_ecode = GL_LLM_ERR_FIRST_ELEM;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_NULL;
    }
    /*
     * Validate the pointer passed as argument.
     */
    if((p_elem == GL_NULL) || (p_elem->p_prev == GL_NULL))
    {
        *p_ecode = GL_LLM_ERR_INV_PTR;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_NULL;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);
    
    return (p_elem->p_prev);
}



/*******************************************************************************
* Function name    : gl_llm_insert_before
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         p_element, 
*                    gl_pvoid_t         p_new_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Inserts the new element before the element specified 
*                    in the linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_insert_before

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id, 
      gl_pvoid_t                p_element, 
      gl_pvoid_t                p_new_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_new_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element; 
      gl_pvoid_t                p_new_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;
    gl_llm_header_t* p_new_elem=(gl_llm_header_t*) p_new_element ;

    GL_INC_LLM_G_ADD_NODE_RQ() 

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
    /*
     * Validate the pointers passed as argument.
     */
    if((p_element == GL_NULL) || (p_new_element == GL_NULL)
        || (p_elem->list_id != link_list_id))
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
    
    if(g_a_p_link_list[link_list_id]->elem_qty == GL_LLM_MAX_ELEMENTS)
    {
        *p_ecode = GL_LLM_ERR_LIST_FULL;

        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)
            
        return GL_FAILURE;
    }

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                        GL_FAILURE);
        return GL_FAILURE;
    }

    /*
     * Set the list id in the new element.
     */
    p_new_elem->list_id = link_list_id;

    /*
     * Increment the element count.
     */
    (g_a_p_link_list[link_list_id]->elem_qty)++;

    if(p_elem == g_a_p_link_list[link_list_id]->p_first)
    {
        /*
         * If the existing element is the first element.
         */
        p_new_elem->p_next = g_a_p_link_list[link_list_id]->p_first;
        p_new_elem->p_prev = GL_NULL;
        g_a_p_link_list[link_list_id]->p_first->p_prev = p_new_elem;
        g_a_p_link_list[link_list_id]->p_first = p_new_elem;
    }
    else
    {
        /*
         * Else if it is some other element.
         */
        if(GL_NULL == p_elem->p_prev)
        {
            *p_ecode = GL_LLM_ERR_INV_PTR;

            GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

            return GL_FAILURE;
        }
        p_new_elem->p_prev = p_elem->p_prev;
        p_new_elem->p_next = p_elem;
        p_elem->p_prev->p_next = p_new_elem;
        p_elem->p_prev = p_new_elem;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);  

    GL_LLM_C_PEAK_NODE_ADDED(link_list_id, g_a_p_link_list[link_list_id]->
        elem_qty) 
        
    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_insert_after
* Arguments        : gw_U32bit          task_id, 
*                    gl_list_t          link_list_id, 
*                    gl_pvoid_t         p_element, 
*                    gl_pvoid_t         p_new_element, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Inserts the new element after the element specified 
*                    in the linked list specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_insert_after

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id, 
      gl_list_t                 link_list_id,
      gl_pvoid_t                p_element, 
      gl_pvoid_t                p_new_element,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, link_list_id, p_element, p_new_element, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 link_list_id;
      gl_pvoid_t                p_element; 
      gl_pvoid_t                p_new_element;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_llm_header_t* p_elem=(gl_llm_header_t*) p_element ;
    gl_llm_header_t* p_new_elem=(gl_llm_header_t*) p_new_element ;

    GL_INC_LLM_G_ADD_NODE_RQ() 

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
    /*
     * Validate the pointers passed as argument.
     */
    if((p_element == GL_NULL) || (p_new_element == GL_NULL)
        || (p_elem->list_id != link_list_id))
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

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    if(g_a_p_link_list[link_list_id]->elem_qty == GL_LLM_MAX_ELEMENTS)
    {
        GL_INC_LLM_C_ADD_NODE_FAIL(link_list_id)
            
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        *p_ecode = GL_LLM_ERR_LIST_FULL;
        return GL_FAILURE;
    }

    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        *p_ecode = GL_LLM_ERR_NO_ELEMENT;
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                        GL_FAILURE);
        return GL_FAILURE;
    }

    /*
     * Set the list id in the new element.
     */
    p_new_elem->list_id=link_list_id;

    /*
     * Increment the element count.
     */
    (g_a_p_link_list[link_list_id]->elem_qty)++;
    
    if(p_elem == g_a_p_link_list[link_list_id]->p_last)
    {
        /*
         * If the existing element is the last element.
         */
        p_new_elem->p_prev = g_a_p_link_list[link_list_id]->p_last;
        p_new_elem->p_next = GL_NULL;
        g_a_p_link_list[link_list_id]->p_last->p_next = p_new_elem;
        g_a_p_link_list[link_list_id]->p_last = p_new_elem;
    }
    else
    {
        /*
         * Else if it is some other element.
         */
        if(GL_NULL == p_elem->p_next)
        {
            *p_ecode = GL_LLM_ERR_INV_PTR;

            GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

            return GL_FAILURE;
        }
        p_new_elem->p_next = p_elem->p_next;
        p_new_elem->p_prev = p_elem;
        p_elem->p_next->p_prev = p_new_elem;
        p_elem->p_next = p_new_elem;
    }

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    GL_LLM_C_PEAK_NODE_ADDED(link_list_id, g_a_p_link_list[link_list_id]->
        elem_qty) 
        
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_llm_remove_all_elements
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,  
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes all the elements from the link list one by 
*                    one. This function internally called the function to free 
*                    memory of element pointer stored in the Link List.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_remove_all_elements 

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
    gl_llm_header_t             *p_elem;
    gl_llm_header_t             *p_next_elem;
    
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

    if(GL_NULL != g_a_p_link_list[link_list_id]->p_first)
    {
        /*
         * If first element is not null call function pointed by
         * p_elem_free_fn, to free the memory allocated for all
         * the stored element pointers one by one. 
         */
        p_elem = g_a_p_link_list[link_list_id]->p_first;
        
        do
        {
            p_next_elem = p_elem->p_next;
            /*
             * Invoke the function defined by user to free moemory of 
             * element (pointer to elements are stored in Link list).
             */
            if(GL_FAILURE == g_a_p_link_list[link_list_id]->p_elem_free_fn(
                    g_a_p_link_list[link_list_id]->p_link_list_arg,
                    (gl_pvoid_t)p_elem, p_ecode))
            {
                return GL_FAILURE;
            }
            p_elem = p_next_elem;
            
        }while(GL_NULL != p_next_elem);
    }
    g_a_p_link_list[link_list_id]->elem_qty = 0;
    g_a_p_link_list[link_list_id]->p_first = GL_NULL;
    g_a_p_link_list[link_list_id]->p_last = GL_NULL;    
    
    return GL_SUCCESS;
}




/*******************************************************************************
* Function name    : gl_llm_get_element_count
* Arguments        : gl_list_t          link_list_id,
*                    gw_U32bit          *p_number_of_elements,
*                    gl_error_t*        p_ecode
* Return Value     : gl_return_t
* Functionality    : Gets the count of the elements in the linked list 
*                    specified.
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_get_element_count

#ifdef _ANSI_C_
    ( gl_list_t                 link_list_id,
      gw_U32bit                 *p_number_of_elements,
      gl_error_t                *p_ecode
    )
#else
    ( link_list_id, p_number_of_elements, p_ecode )
      gl_list_t                 link_list_id;
      gw_U32bit                 *p_number_of_elements;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_number_of_elements)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_NULL_NUM_OF_ELEM_PTR;
        return GL_FAILURE;
    }
    if( link_list_id >= GL_LLM_MAX_LIST
        || g_a_p_link_list[link_list_id] == GL_NULL)
    {
        GL_INC_LLM_G_PARM_VLD_FAIL()

        *p_ecode = GL_LLM_ERR_INV_LINK_LIST; 
        return GL_FAILURE;
    }
#else
    /* 
     * To remove warning, if GL_LINK_LIST_ID_DISABLE flag set.
     */
    p_ecode = p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);

    *p_number_of_elements = g_a_p_link_list[link_list_id]->elem_qty;

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}


/*******************************************************************************
* Function name    : gl_llm_is_empty
* Arguments        : gl_list_t          link_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_boolean
* Functionality    : Returns true only if the linked list contains zero 
*                    elements.
* Functions called :
*******************************************************************************/
gl_boolean gl_llm_is_empty

#ifdef _ANSI_C_
    ( gl_list_t                 link_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( link_list_id, p_ecode )
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
        return GL_FALSE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Threads accessing same Link-list.
     */
    GL_MT_LOCK(&(g_a_p_link_list [link_list_id]->llm_sem), p_ecode,
                  GL_FAILURE);
    /*
     * Return true if list empty.
     */
    if( g_a_p_link_list[link_list_id]->elem_qty == GL_NULL)
    {
        GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

        return GL_TRUE;
    }
    *p_ecode = GL_LLM_ERR_NOT_EMPTY;

    GL_MT_UNLOCK(&(g_a_p_link_list [link_list_id]->llm_sem),
                    p_ecode, GL_FAILURE);

    return GL_FALSE;
}

