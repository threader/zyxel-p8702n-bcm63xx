/*******************************************************************************
*
* FUNCTION    : Generic Libraries - List Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_llm_glb_defs_n_intrnl_funcs.c
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

/*
 * Array of list structures where the lists are added at runtime
 */
gl_llm_list_t *g_a_p_link_list[GL_LLM_MAX_LIST]; 

gl_wlm_list_t *g_a_p_work_list[GL_WLM_MAX_LIST]; 

#ifdef GL_MT_ENABLED
     GL_MUTEX_ID lm_mgr_sem;
#endif

gl_boolean             llm_mgr_init_flag = GL_FALSE;


/*****************************************************************************
* Function name    : llm_search_from_first
* Arguments        : gl_list_t          list_id, 
*                    gl_wl_index_t      local_index
* Return Value     : gl_pvoid_t
* Functionality    : 
* Functions called :
*****************************************************************************/
gl_pvoid_t
llm_search_from_first

#ifdef _ANSI_C_
    ( gl_list_t                 list_id,
      gl_wl_index_t             local_index
    )
#else
    ( list_id, local_index )
      gl_list_t                 list_id;
      gl_wl_index_t             local_index;
#endif

/****************************************************************************/
{
    gl_wlm_header_t* p_rec;
    /*
     * First record is already checked hence start from next to first record
     * which is not null.
     */
    for(p_rec = g_a_p_work_list[list_id]->p_first->p_next; 
        p_rec != g_a_p_work_list[list_id]->p_first; 
        p_rec=p_rec->p_next)
    {
        if( p_rec->local_index == local_index)
        {
            return p_rec;
        }
    }
    return GL_NULL;
}




/*****************************************************************************
* Function name    : llm_search_from_last
* Arguments        : gl_list_t          list_id,
*                    gl_wl_index_t      local_index
* Return Value     : gl_pvoid_t
* Functionality    : 
* Functions called :
*****************************************************************************/
gl_pvoid_t
llm_search_from_last

#ifdef _ANSI_C_
    ( gl_list_t                 list_id,
      gl_wl_index_t             local_index
    )
#else
    ( list_id, local_index )
      gl_list_t                 list_id;
      gl_wl_index_t             local_index;
#endif

/****************************************************************************/
{
    gl_wlm_header_t* p_rec;
    /*
     * Last record is already checked hence start from previous to last record
     * which is not null.
     */
    for(p_rec=g_a_p_work_list[list_id]->p_last->p_prev; 
        p_rec != g_a_p_work_list[list_id]->p_last; 
        p_rec=p_rec->p_prev)
    {
        if( p_rec->local_index == local_index)
        {
            return p_rec;
        }
    }
    return GL_NULL;
}



/*****************************************************************************
* Function name    : llm_remove_ptr
* Arguments        : gl_list_id         list_id,
*                    gl_wlm_header_t    *p_elem
* Return Value     : void
* Functionality    : 
* Functions called :
*****************************************************************************/
void
llm_remove_ptr

#ifdef _ANSI_C_
    ( gl_list_t                 list_id, 
      gl_wlm_header_t            *p_elem
    )
#else
    ( list_id, p_elem )
      gl_list_t                 list_id;
      gl_wlm_header_t            *p_elem;
#endif

/****************************************************************************/
{
    /*
     * Simply remove the record which is not the first or the last record
     * from the list.
     */
    (g_a_p_work_list[list_id]->elem_qty)--;
    p_elem->p_prev->p_next=p_elem->p_next;
    p_elem->p_next->p_prev=p_elem->p_prev;
    p_elem->p_next = p_elem->p_prev= GL_NULL;
    p_elem->list_id = GL_NULL;
    p_elem->local_index = 0;
}




/*****************************************************************************
* Function name    : llm_remove_head
* Arguments        : gl_list_id         list_id
* Return Value     : void
* Functionality    :
* Functions called :
*****************************************************************************/
void
llm_remove_head

#ifdef _ANSI_C_
    ( gl_list_t                 list_id )
#else
    ( list_id )
      gl_list_t                 list_id;
#endif

/****************************************************************************/
{

    gl_wlm_header_t* p_elem=g_a_p_work_list[list_id]->p_first;
    /*
     * Reset the list id and the local_index in the header.
     */
    p_elem->list_id=0;
    p_elem->local_index=0;
    if(g_a_p_work_list[list_id]->elem_qty == 1)
    {
        /*
         * If this is the only record in the list 
         * reset the first and last record.
         */
        g_a_p_work_list[list_id]->elem_qty=0;
        g_a_p_work_list[list_id]->p_first = g_a_p_work_list[list_id]->p_last
            = GL_NULL;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * If list has existing records in the code.
         */
        (g_a_p_work_list[list_id]->elem_qty)--;
        g_a_p_work_list[list_id]->p_first= p_elem->p_next;
        p_elem->p_next->p_prev= g_a_p_work_list[list_id]->p_last;
        g_a_p_work_list[list_id]->p_last->p_next= p_elem->p_next;
        p_elem->p_next=p_elem->p_prev= GL_NULL;
    }
}




/*****************************************************************************
* Function name    : llm_remove_tail
* Arguments        : gl_list_id         list_id
* Return Value     : void
* Functionality    :
* Functions called :
*****************************************************************************/
void
llm_remove_tail

#ifdef _ANSI_C_
    ( gl_list_t                 list_id )
#else
    ( list_id )
      gl_list_t                 list_id;
#endif

/****************************************************************************/
{
    gl_wlm_header_t* p_elem=g_a_p_work_list[list_id]->p_last;
    /*
     * Reset the list id and the local_index in the header.
     */
    p_elem->list_id=0;
    p_elem->local_index=0;
    if(g_a_p_work_list[list_id]->elem_qty == 1)
    {
        /*
         * If this is the only record in the list 
         * reset the first and last record.
         */
        g_a_p_work_list[list_id]->elem_qty=0;
        g_a_p_work_list[list_id]->p_last = g_a_p_work_list[list_id]->p_first
            = GL_NULL;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
    else
    {
        /*
         * If list has existing records in the code.
         */
        (g_a_p_work_list[list_id]->elem_qty)--;
        g_a_p_work_list[list_id]->p_last = p_elem->p_prev;
        p_elem->p_prev->p_next = g_a_p_work_list[list_id]->p_first;
        g_a_p_work_list[list_id]->p_first->p_prev = p_elem->p_prev;
        p_elem->p_next=p_elem->p_prev=GL_NULL;
    }
}


