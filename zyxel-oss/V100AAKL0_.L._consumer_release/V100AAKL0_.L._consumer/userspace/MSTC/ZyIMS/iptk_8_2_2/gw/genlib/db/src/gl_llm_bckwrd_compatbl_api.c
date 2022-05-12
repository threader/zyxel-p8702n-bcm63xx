/*****************************************************************************
*
*    FUNCTION    : Source file for Generic Library - Link List Manager
*                  backward compatibility of Rel-1.0 with Rel-0.2 
*                  implementation.
******************************************************************************
*
*    FILENAME    : gl_llm_bckwrd_compatbl_api.c
*
*    DESCRIPTION : Give defination of functions which requires backward
*                  compatibilty of Rel-1.0 with Rel-0.2.
*
*
* DATE          NAME           REFERENCE          REASON
*----------- --------------  ---------------    ----------------------
*
* 22/04/2002 Ravi Jindal        -               Intial Draft.
* 
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/


#include "gl_lm_interfaces_rel_02.h"     


/*****************************************************************************
* Function name    : gl_llm_list_create
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          *p_link_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the linked list structure and initializes the
*                    list structure elements.
* Functions called :
******************************************************************************/
gl_return_t
gl_llm_list_create

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 *p_link_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, p_link_list_id, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 *p_link_list_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{    
    
    /*
     * As function Pointer to free memory of element allocated by User is
     * required. The User of Rel-0.2 needs to defined a Poratble function
     * to free memory.
     * GL_LLM_FREE_ELEMENT_MEMORY should be a function pointer of type
     * "gl_llm_free_element_fn_t".
     * GL_LINK_LIST_ARG should be a pointer to void.
     */ 

    return( gl_llm_create_link_list(task_id, p_link_list_id,
                    GL_LLM_FREE_ELEMENT_MEMORY, GL_LINK_LIST_ARG, p_ecode));
    
}




/*******************************************************************************
* Function name    : gl_llm_get_count
* Arguments        : gl_list_t          link_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gw_S32bit
* Functionality    : Gets the count of the elements in the linked list
*                    specified.
* Functions called :
*******************************************************************************/
gw_S32bit
gl_llm_get_count

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
    gw_U32bit                   number_of_elements;

    if(GL_FAILURE == gl_llm_get_element_count( link_list_id, 
                &number_of_elements, p_ecode))
    {
        return GL_NULL;
    }
    /*
     * As return type is gw_S32bit.
     */ 
    return ((gw_S32bit)number_of_elements);
}



/*****************************************************************************
* Function name    : gl_llm_flush_all
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          link_list_id,
*                    gl_pvoid_t         *p_p_element,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes all the elements from the link list one by
*                    one. This function is called in a while loop only
*                    by the user
* Functions called :
*******************************************************************************/
gl_return_t
gl_llm_flush_all

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
    /*
     * As in Rel-1.0 this function returns after removeal
     * of all the elements.
     */
    /*
     * To remove Warnings.
     */
    p_p_element = p_p_element;
    
    return(gl_llm_remove_all_elements(task_id, link_list_id, p_ecode));
}
            


/*****************************************************************************
* Function name    : gl_wlm_create
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          *p_work_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the worked list structure and initializes the
*                    list structure elements.
* Functions called :
******************************************************************************/
gl_return_t
gl_wlm_create

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 *p_work_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, p_work_list_id, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 *p_work_list_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{    
    
    /*
     * As function Pointer to free memory of element allocated by User is
     * required. The User of Rel-0.2 needs to defined a Poratble function
     * to free memory.
     * GL_WLM_FREE_RECORD_MEMORY_FUNC_PTR should be a function pointer of type
     * "gl_wlm_free_element_fn_t".
     * GL_WORK_LIST_ARG should be a pointer to void.
     */ 

    return( gl_wlm_create_work_list(task_id, p_work_list_id,
                    GL_WLM_FREE_RECORD_MEMORY_FUNC_PTR, GL_WORK_LIST_ARG,
                    p_ecode));
    
}





/*******************************************************************************
* Function name    : gl_wlm_get_count
* Arguments        : gl_list_t          work_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gw_S32bit
* Functionality    : Gets the count of the elements in the worked list
*                    specified.
* Functions called :
*******************************************************************************/
gw_S32bit
gl_wlm_get_count

#ifdef _ANSI_C_
    ( gl_list_t                 work_list_id,
      gl_error_t                *p_ecode
    )
#else
    ( work_list_id, p_ecode )
      gl_list_t                 work_list_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gw_U32bit                   number_of_records;

    if(GL_FAILURE == gl_wlm_get_record_count( work_list_id, 
                &number_of_records, p_ecode))
    {
        return GL_NULL;
    }
    /*
     * As return type is gw_S32bit.
     */ 
    return ((gw_S32bit)number_of_records);
}



/*****************************************************************************
* Function name    : gl_wlm_flush_all
* Arguments        : gw_U32bit          task_id,
*                    gl_list_t          work_list_id,
*                    gw_U16bit          *p_local_index,
*                    gl_pvoid_t         *p_p_record,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Removes all the elements from the work list one by
*                    one. This function is called in a while loop only
*                    by the user
* Functions called :
*******************************************************************************/
gl_return_t
gl_wlm_flush_all

#ifdef _ANSI_C_
    ( gw_U32bit                 task_id,
      gl_list_t                 work_list_id,
      gw_U16bit                 *p_local_index,
      gl_pvoid_t                *p_p_record,
      gl_error_t                *p_ecode
    )
#else
    ( task_id, work_list_id, p_local_index, p_p_record, p_ecode )
      gw_U32bit                 task_id;
      gl_list_t                 work_list_id;
      gw_U16bit                 *p_local_index;
      gl_pvoid_t                *p_p_record;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * As in Rel-1.0 this function returns after removeal
     * of all the elements.
     */
    /*
     * To remove Warnings.
     */
    p_p_record = p_p_record;
    p_local_index = p_local_index;
    
    return(gl_wlm_remove_all_records(task_id, work_list_id, p_ecode));
}
            

