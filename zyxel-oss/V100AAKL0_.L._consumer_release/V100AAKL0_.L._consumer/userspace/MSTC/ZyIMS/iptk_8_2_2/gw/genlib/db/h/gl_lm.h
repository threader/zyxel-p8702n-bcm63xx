/*******************************************************************************
*
* FUNCTION    : Generic Libraries - List Manager data types.
*
********************************************************************************
*
* FILENAME    : gl_lm.h
*
* DESCRIPTION : This file contains the data type declarations of linked list 
*               and work list in the generic library implementation.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* Oct'99     Kapil Nayar    HSS:40600026      Generic Libraries
*                           HSS:40600036      (Gateways)
* 21/1/2001  Sachin Mittal                    Reorganised and moved in common 
* 18/10/2001 Ravi Jindal    SPR-2788          Packaging of Genlib-Code.
* 17/11/2001 Ravi Jindal    SPR-3107          Incoprate the defects and change
*                                             cosmmetics of file.
* 30/11/2001 Ravi Jindal    SPR-2788          Re-Packaging of Genlib-Code.
* 18/12/2001 Ravi Jindal    SPR-3363          Compilation with CC.
*
* 15/01/2002 Ravi Jindal    SPR-3452          Re-Packaging of Genlib-Code.
*
* 18/03/2002 Ravi Jindal    SPR-3958          Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/*
 * Check to see if this file has been included already. 
 */
#ifndef _GL_LM_H_
#define _GL_LM_H_

#include "gl_lm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* 
 * Structure for the linked list.
 */
typedef struct gl_llm_list_s
{
    /*
     * Pointer to the first element in the linked list.
     */
    gl_llm_header_t             *p_first; 
    /*
     * Pointer to the last element in the linked list.
     */
    gl_llm_header_t             *p_last; 
    /*
     * Quantity of the elements in the list.
     */ 
    gw_U32bit                   elem_qty;
    /*
     * Task_id of the owner task of the list.
     */
    gw_U32bit                   owner_task_id; 
    /*
     * To store the function pointer, invoked to free the memory
     * allocated for the elements. 
     * As memory allocation Scheme is different for different
     * created link lists function pointer is stored for each list.
     */
    gl_llm_free_element_fn_t    p_elem_free_fn;
    /*
     * As the inforamtion specific to the list may be required 
     * to free the memory pointed by stored element pointers.
     * A pointer to void is stored and passed as a input parameter
     * to free the memory function.
     */
    gl_pvoid_t                  p_link_list_arg;
    /*
     * Semaphore used for synchronizing access 
     * of link list by seperate tasks.
     */
#ifdef GL_TOTAL_MT_SAFE
    GL_MUTEX_ID llm_sem;
#endif
    
}gl_llm_list_t;


/* 
 * Structure for the work list.
 */
typedef struct gl_wlm_list_s
{
    /*
     * Pointer to the first element in the work list.
     */
    gl_wlm_header_t             *p_first; 
    /*
     * Pointer to the last element in the work list.
     */
    gl_wlm_header_t             *p_last; 
    /*
     * Quantity of the elements in the list.
     */ 
    gw_U32bit                   elem_qty;
    /*
     * Task_id of the owner task of the list.
     */
    gw_U32bit                   owner_task_id; 
    /*
     * Next index to be allocated for the new record.
     */
    gw_U32bit                   new_local_index; 
    /*
     * To store the function pointer, invoked to free the memory
     * allocated for the records. 
     * As memory allocation Scheme is different for different
     * created Work lists function pointer is stored for each list.
     */
    gl_wlm_free_record_fn_t     p_record_free_fn;
    /*
     * As the inforamtion specific to the list may be required 
     * to free the memory pointed by stored record pointers.
     * A pointer to void is stored and passed as a input parameter
     * to free the memory function.
     */
    gl_pvoid_t                  p_work_list_arg;
    /*
     * Semaphores used for accessing the work list
     * from two seperate tasks.
     */
#ifdef GL_TOTAL_MT_SAFE
    GL_MUTEX_ID lm_sem;
#endif
    
}gl_wlm_list_t;


/**********************************************
 * Extern Declaration for Array of Lists.
 *********************************************/

/*
 * Array of list structures where the lists are added at runtime
 */
extern  gl_llm_list_t *g_a_p_link_list[GL_LLM_MAX_LIST]; 

extern  gl_wlm_list_t *g_a_p_work_list[GL_WLM_MAX_LIST]; 

extern gl_boolean             llm_mgr_init_flag;

#ifdef GL_MT_ENABLED
    extern  GL_MUTEX_ID lm_mgr_sem;
#endif


/**********************************************
 * LOCAL FUNCTIONS DECLARATION.
 *********************************************/

extern  gl_pvoid_t
llm_search_from_first
       
    _ARGS_( (   gl_list_t               list_id,
                gl_wl_index_t          local_index     ) );

        

extern  gl_pvoid_t
llm_search_from_last

    _ARGS_( (   gl_list_t               list_id,
                gl_wl_index_t          local_index     ) );
        

extern  void
llm_remove_ptr

    _ARGS_( (   gl_list_t               list_id,
                gl_wlm_header_t          *p_elem   ) );


extern  void
llm_remove_head

    _ARGS_( (   gl_list_t               list_id   ) );

    
extern  void
llm_remove_tail
    
    _ARGS_( (   gl_list_t               list_id   ) );



#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
