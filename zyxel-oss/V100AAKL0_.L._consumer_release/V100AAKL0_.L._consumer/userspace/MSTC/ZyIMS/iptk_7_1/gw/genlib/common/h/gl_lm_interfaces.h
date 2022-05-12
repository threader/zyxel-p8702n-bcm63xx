/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Link-List Manager 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_lm_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of link-list and work-list manager 
*                  sub-module of the generic library implementation.
*                  This file is to be include in the application module, in
*                  order to use the supported APIs.
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* 29/11/2001 Ravi Jindal        SPR-2788        Packaging of Genlib-Code.
* 
* 06/12/2001 Prashant Jain      SPR-3363        Compilation with CC.
*
* 15/01/2002 Ravi Jindal        SPR-3452        Re-Packaging of Genlib-Code.
*
* 18/03/2002 Ravi Jindal        SPR-3958        Enhancements for Rel-1.0.
* 
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_LM_INTERFACES_H_
#define _GL_LM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_gscim_interfaces.h"
#include "gl_ulm_interfaces.h"


#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Structure for the header to the Link list element
 * this is included by the user as the first nested element of the
 * element structure. This is only used by the list manager.
 */
typedef struct gl_llm_header_s
{
    struct gl_llm_header_s* p_next;
    struct gl_llm_header_s* p_prev;
    gw_U16bit list_id;
}gl_llm_header_t;


/*
 * Structure for the header to the Work list Record
 * this is included by the user as the first nested element of the
 * record structure. This is only used by the list manager.
 */
typedef struct gl_wlm_header_s
{
    struct gl_wlm_header_s* p_next;
    struct gl_wlm_header_s* p_prev;
    gw_U16bit list_id;
    gw_U32bit local_index;
} gl_wlm_header_t;


/*
 * Protptype for the element memory free function pointer.
 * This function is registered while creating the link list 
 * and invoked in "gl_llm_remove_all_elements".
 */
typedef gl_return_t (*gl_llm_free_element_fn_t) 
            ( gl_pvoid_t        p_link_list_arg,
              gl_pvoid_t        p_element,
              gl_error_t        *p_ecode    );

/*
 * Protptype for the record memory free function pointer.
 * This function is registered while creating the work list 
 * and invoked in "gl_wlm_remove_all_records".
 */
typedef gl_return_t (*gl_wlm_free_record_fn_t) 
            ( gl_pvoid_t        p_work_list_arg,
              gl_pvoid_t        p_record,
              /*
               * Index of the record pointed by p_record,
               * to be removed from Work-list.
               */ 
              gw_U32bit         local_index,
              gl_error_t        *p_ecode    );



/* 
 * EXTERNAL INTERFACE APIs - LINKED LISTS.
 */
/*
 * The functions include those for creation of linked list and operations such 
 * as addition, subtraction of elements.
 */


/*
 * Initialise the generic libraries list (linked list and work list) manager.
 */
extern gl_return_t
gl_lm_init( void );


extern gl_return_t
gl_llm_create_link_list

    _ARGS_( (   gw_U32bit                task_id, 
                gl_list_t                *p_link_list_id,
                gl_llm_free_element_fn_t p_elem_free_fn,
                gl_pvoid_t               p_link_list_arg,
                gl_error_t               *p_ecode           ) );



extern gl_return_t
gl_llm_delete_link_list
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id, 
                gl_error_t              *p_ecode   ) );


extern gl_return_t
gl_llm_get_head
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_llm_get_tail
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode   ) );


extern gl_return_t
gl_llm_remove_element
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               link_list_id, 
                gl_pvoid_t              p_element, 
                gl_error_t              *p_ecode   ) );



extern gl_return_t
gl_llm_remove_head
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               link_list_id,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_llm_remove_tail

    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id,
                gl_pvoid_t              *p_p_element,
                gl_error_t              *p_ecode   ) );



extern gl_return_t
gl_llm_add_head
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id,
                gl_pvoid_t              p_element, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_llm_add_tail
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               link_list_id,
                gl_pvoid_t              p_element, 
                gl_error_t              *p_ecode   ) );



extern gl_pvoid_t
gl_llm_get_next
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               link_list_id,
                gl_pvoid_t              p_element, 
                gl_error_t              *p_ecode    ) );



extern gl_pvoid_t
gl_llm_get_prev
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id, 
                gl_pvoid_t              p_element, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_llm_insert_before
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id, 
                gl_pvoid_t              p_element, 
                gl_pvoid_t              p_new_element, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_llm_insert_after
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               link_list_id,
                gl_pvoid_t              p_element, 
                gl_pvoid_t              p_new_element,
                gl_error_t              *p_ecode    ) );



extern gl_return_t 
gl_llm_get_element_count
    
    _ARGS_( (   gl_list_t               link_list_id,
                gw_U32bit               *p_number_of_elements,
                gl_error_t              *p_ecode                ) );

extern gl_boolean 
gl_llm_is_empty
    
    _ARGS_( (   gl_list_t               link_list_id, 
                gl_error_t              *p_ecode     ) );


extern gl_return_t
gl_llm_remove_all_elements

    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               link_list_id,
                gl_error_t              *p_ecode     ) );


/*
 * EXTERNAL INTERFACE APIs - WORK LISTS.
 */
/* 
 * The functions include those for creation of work list and operations such 
  * as addition, subtraction of elements.
  */


extern gl_return_t
gl_wlm_create_work_list
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               *p_work_list_id,
                gl_wlm_free_record_fn_t p_record_free_fn,
                gl_pvoid_t              p_work_list_arg,
                gl_error_t              *p_ecode            ) );



extern gl_return_t
gl_wlm_delete_work_list
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               work_list_id, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t 
gl_wlm_add_record
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               work_list_id,
                gl_pvoid_t              p_record, 
                gl_wl_index_t           *p_index, 
                gl_error_t              *p_ecode     ) );




extern gl_return_t
gl_wlm_get_record
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               work_list_id, 
                gl_wl_index_t           wlm_index, 
                gl_pvoid_t              *p_p_record,
                gl_error_t              *p_ecode     ) );



extern gl_return_t
gl_wlm_remove_record
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               work_list_id,
                gl_wl_index_t           wlm_index, 
                gl_pvoid_t              *p_p_record, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_wlm_remove_record_ptr
    
    _ARGS_( (   gw_U32bit               task_id,
                gl_list_t               work_list_id,
                gl_pvoid_t              p_record,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_wlm_remove_all_records
    
    _ARGS_( (   gw_U32bit               task_id, 
                gl_list_t               work_list_id, 
                gl_error_t              *p_ecode     ) );



extern gl_return_t 
gl_wlm_get_record_count
    
    _ARGS_( (   gl_list_t               work_list_id,
                gw_U32bit               *p_number_of_records,
                gl_error_t              *p_ecode                ) );



extern gl_boolean 
gl_wlm_is_empty
    
    _ARGS_( (   gl_list_t               work_list_id, 
                gl_error_t              *p_ecode    ) );




/*
 * Delete all the lists (both Link-lists and Work-lists).
 */
extern gl_return_t
gl_lm_destroy_all_lists

    _ARGS_( (   gw_U8bit                is_forceful_cleanup,
                gl_error_t              *p_ecode     ) );

#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
