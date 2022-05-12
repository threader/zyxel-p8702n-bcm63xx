/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Memory Manager 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_mm_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of memory manager sub-module 
                   of the generic library implementation. 
*                  This file is to be include in the application module, in
*                  order to use the supported APIs.
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* 29/11/2001 Ravi Jindal        SPR-2788        Packaging of Genlib-Code.
* 
* 06/12/2001 Prashant Jain      SPR-3363        Compilation with CC.
* 27/12/2001 Ravi Jindal        SPR-3412        To add the task specific 
*                                               common pool API prototypes. 
* 
* 10/03/2002 Ravi Jindal        SPR-3952        Enhancements for Rel-1.0.
*
* 31/01/2003 Ravi Jindal        SPR-5312        An API To get owner-id of a 
*                                               buffer is added.
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_MM_INTERFACES_H_
#define _GL_MM_INTERFACES_H_

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
 * Structure for the user-id information to be used for Audit Mgt.
 * This information is kept for all the user-ids, for which buffers allocated.
 */
typedef struct gl_mm_user_id_info_s
{
    /*
     * Time-Stamp, taken when first buffer with the user-id is allocated.
     */
    gl_time_stamp_t             time_stamp;
    /*
     * Number of buffer allocated with rthe user-id at any time.
     */ 
    gw_U32bit                   num_of_aloc_bufs;
    
}gl_user_id_info_t;
   

/*
 * struct for task specific common pool information.
 */
typedef struct gl_pool_info_s
{
    gl_pool_id_t                pool_id;
    /*
     * Id of created task specific pool.
     */
    gw_U16bit                   task_local_data;
    /*
     * Index to the pool information array for a specific task-id.
     * At this index in pool information array, pointer to task pool with 
     * smallest buffer size is maintained for each task-id.
     */
}gl_pool_info_t;


/*
 * Structure of Region's defination values.
 */
typedef struct gl_cngst_region_def_s
{
    /*
     * All these parameters are expresed in Percentage to total
     * number of buffer for pool. These values are exclusive to the
     * level(i.e. If discard = 45 for level-2, So when number of busy-
     * buffers == 45% of total number of buffers, congestion region
     * is moved from level-2 to level-3.
     */
    gw_U8bit                    onset;
    /*
     * Start of Specified Congestion Region.
     */
    gw_U8bit                    discard;
    /*
     * Entering to next More severe Congestion Region.
    */
    gw_U8bit                    abate;
    /*
     * Entering to next Less severe Congestion Region.
     */
}gl_cngst_region_def_t;



/*
 * Structure of Congestion Management Regions information.
 */
typedef struct gl_cngst_mgmt_spec_s
{
    gw_U8bit                    num_regions;
    /*
     * Number of Congestion region supported.
     */
    gl_cngst_region_def_t       region_def[GL_MM_CNGST_MGMT_MAX];
    /*
     * Array of Regions Information Structure.
     */
    gl_pvoid_t                  p_user_arg;
    /*
     * Passed as a agument to call-back function( it is user-defined)
     * no checks are performed on this, genlib only store this pointer and
     * passes as an argument to call-back function.
     */
}gl_cngst_mgmt_spec_t;


/*
 * Type of Congestion Management Call back function.
 */
typedef gl_return_t (*gl_cngst_mgmt_fn_t)(  gl_pool_id_t    pool_id,
                                            gw_U8bit        from_region,
                                            gw_U8bit        to_region,
                                            gl_pvoid_t      p_user_arg,
                                            gl_error_t      *p_ecode);



/*
 * EXTERNAL INTERFACE APIs MEMORY MANAGER 
 */
/*
 * The functions include those for creating the memory pools and those for
 * allocation and deallocation of memory buffers from the created 
 * pools dynamically.
 */

/*
 * Initialise the generic libraries memory manager.
 */
extern gl_return_t    
gl_mm_init( void );
    
/*
 * Interfaces for common memory pools.
 */
/*
 * Create a pool.
 */
extern gl_return_t    
gl_mm_common_pool_create

    _ARGS_( (   gl_pool_id_t            *p_pool_id,
                gw_U32bit               no_of_buf,
                gw_U32bit               buf_size,
                gl_error_t              *p_ecode    ) );
                       
      

/*
 * Allocate memory buffer.
 */
extern gl_pvoid_t    
gl_mm_get_buffer

    _ARGS_( (   gw_U32bit               task_id,
                gw_U32bit               buf_size, 
                gl_error_t              *p_ecode    ) );
  
       

/*
 * Release a memory buffer.
 */
extern gl_return_t    
gl_mm_release_buffer
        
    _ARGS_( (   gw_U32bit               task_id, 
                gl_pvoid_t              buf_ptr, 
                gl_error_t              *p_ecode    ) );

/*
 * To get the owner id of a allocated buffer.
 */ 
extern gl_return_t    
gl_mm_get_owner_of_buffer
        
    _ARGS_( (   gl_pvoid_t              buf_ptr,
                gw_U32bit               *p_task_id, 
                gl_error_t              *p_ecode    ) );
 
       
       

/*
 * Change owner of the allocated buffer.
 */
extern gl_return_t    
gl_mm_change_owner_of_buffer

    _ARGS_( (   gw_U32bit               old_task_id,
                gw_U32bit               new_task_id,    
                gl_pvoid_t              buf_ptr,    
                gl_error_t              *p_ecode    ) );
 
       
extern gl_return_t
gl_mm_release_all_common_pool_buffers

    _ARGS_( (   gw_U32bit               buf_size,
                gl_error_t              *p_ecode    ) );
            
          


extern gl_return_t
gl_mm_delete_common_pool

    _ARGS_( (   gw_U32bit               buf_size,
                gl_error_t              *p_ecode    ) );
            
          

extern gl_return_t  
gl_mm_destroy_all_common_pools

    _ARGS_( (   gw_U8bit                is_forceful_cleanup,
                gl_error_t              *p_ecode    ) );
          
/*
 * Congestion Management functions for Common pools.
 */
extern gl_return_t  
gl_mm_set_cngst_mgmt_activity_for_cmn_pools

    _ARGS_( (   gl_pool_id_t            pool_id,
                gl_boolean_t            is_enabled,
                gl_error_t              *p_ecode    ) );

extern gl_return_t  
gl_mm_set_cngst_mgmt_specs_for_cmn_pools

    _ARGS_( (   gl_pool_id_t            pool_id,
                gl_cngst_mgmt_spec_t    *p_specs,
                gl_cngst_mgmt_fn_t      cngst_mgmt_call_back,
                gl_error_t              *p_ecode    ) );


/* 
 * Interfaces for task specific memory pools.
 */
/*
 * Create a pool.
 */
extern gl_return_t    
gl_mm_pool_create     

    _ARGS_( (   gw_U32bit               task_id,    
                gw_U32bit               no_of_buf,    
                gw_U32bit               buf_size,    
                gl_pool_id_t            *p_pool_id,    
                gl_error_t              *p_ecode    ) );
 
       

/*
 * Allocate a memory buffer.
 */
extern gl_pvoid_t    
gl_mm_get_buffer_from_pool 
        
    _ARGS_( (   gw_U32bit               task_id,    
                gl_pool_id_t            pool_id,    
                gl_error_t              *p_ecode    ) );
 
       

/* 
 * Deallocate the memory buffer.
 */
extern gl_return_t    
gl_mm_release_buffer_to_pool 
        
    _ARGS_( (   gw_U32bit               task_id,    
                gl_pool_id_t            pool_id,    
                gl_pvoid_t              buf_ptr,    
                gl_error_t              *p_ecode    ) );
 
       

/*
 * Delete a pool.
 */
extern gl_return_t    
gl_mm_pool_delete 
        
    _ARGS_( (   gw_U32bit               task_id,    
                gl_pool_id_t            pool_id,    
                gl_error_t              *p_ecode    ) );
 
       
extern gl_return_t
gl_mm_release_all_task_pool_buffers

    _ARGS_( (   gw_U32bit               task_id,
                gl_pool_id_t            pool_id,
                gl_error_t              *p_ecode    ) ); 
            
          
          

extern gl_return_t  
gl_mm_destroy_all_task_pools

    _ARGS_( (   gw_U8bit                is_forceful_cleanup,
                gl_error_t              *p_ecode    ) );
            
/*
 * Congestion Management functions for Task pools.
 */
extern gl_return_t  
gl_mm_set_cngst_mgmt_activity_for_task_pools

    _ARGS_( (   gw_U32bit               task_id,
                gl_pool_id_t            pool_id,
                gl_boolean_t            is_enabled,
                gl_error_t              *p_ecode    ) );

extern gl_return_t  
gl_mm_set_cngst_mgmt_specs_for_task_pools

    _ARGS_( (   gw_U32bit               task_id,
                gl_pool_id_t            pool_id,
                gl_cngst_mgmt_spec_t    *p_specs,
                gl_cngst_mgmt_fn_t      cngst_mgmt_call_back,
                gl_error_t              *p_ecode    ) );


/*
 * API for task specific common pool.
 */

extern gl_return_t    
gl_mm_task_specific_cmn_pool_create

    _ARGS_( (   gw_U32bit                 task_id,
                gl_pool_info_t            *p_pool_info,
                gw_U32bit                 no_of_buf, 
                gw_U32bit                 buf_size, 
                gl_error_t                *p_ecode      ) );




extern gl_pvoid_t    
gl_mm_get_task_specific_cmn_pool_buffer
    
    _ARGS_( (   gw_U32bit                 task_id,
                gl_pool_info_t            *p_pool_info,
                gw_U32bit                 buf_size, 
                gl_error_t                *p_ecode      ) );



extern gl_return_t
gl_mm_release_task_specific_cmn_pool_buffer

    _ARGS_( (   gw_U32bit                 task_id, 
                gl_pvoid_t                buf_ptr,  
                gl_error_t                *p_ecode      ) );



extern gl_return_t    
gl_mm_delete_task_specific_cmn_pool
    
    _ARGS_( (   gw_U32bit                 task_id,
                gl_pool_info_t            *p_pool_info,
                gw_U32bit                 buf_size, 
                gl_error_t                *p_ecode      ) );
    


#ifdef GL_MM_ADD_DEBUGGER

/*
 * Structure to store Pool Memory Usage information.
 */
typedef struct gl_mm_pool_usage_profile_s  
{
    gw_U8bit        num_regions;
    gw_U8bit        usage_region_def[GL_MM_POOL_USAGE_MAX_REGIONS];
    gw_U8bit        usage_region_count[GL_MM_POOL_USAGE_MAX_REGIONS];
}gl_mm_pool_usage_profile_t;


extern gl_return_t  
gl_mm_set_cmn_pool_stack_trace

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_boolean_t              is_enabled,
                gl_error_t                *p_ecode      ) );
    
    
extern gl_return_t  
gl_mm_set_cmn_pool_buffer_trace

    _ARGS_( (   gl_pool_id_t              pool_id,
                gw_U32bit                 number_of_requests,
                gl_boolean_t              is_enabled,
                gl_error_t                *p_ecode      ) );

extern gl_return_t  
gl_mm_enable_cmn_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t               pool_id,
                gl_mm_pool_usage_profile_t *p_usage_range_spec,
                gl_error_t                 *p_ecode     ) );

      
extern gl_return_t
gl_mm_disable_cmn_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_error_t                *p_ecode      ) );


extern gl_return_t  
gl_mm_get_cmn_pool_stack_trace

    _ARGS_( (   gl_pvoid_t                p_buf_ptr,
                gl_error_t                *p_ecode      ) );


extern gl_return_t  
gl_mm_get_cmn_pool_buffer_trace

    _ARGS_( (   gl_pvoid_t                p_buf_ptr,
                gl_error_t                *p_ecode      ) );

    
extern gl_return_t  
gl_mm_report_cmn_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_error_t                *p_ecode      ) );

    
extern gl_return_t  
gl_mm_report_cmn_pool_busy_buffers_statistics

    _ARGS_( (   gl_pool_id_t              pool_id,
                gw_U32bit                 task_id,
                gw_U8bit                  buffer_alloc_sig,
                gl_error_t                *p_ecode      ) );
    
extern gl_return_t  
gl_mm_set_task_pool_stack_trace

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_boolean_t              is_enabled,
                gl_error_t                *p_ecode      ) );
    
    
extern gl_return_t  
gl_mm_set_task_pool_buffer_trace

    _ARGS_( (   gl_pool_id_t              pool_id,
                gw_U32bit                 number_of_requests,
                gl_boolean_t              is_enabled,
                gl_error_t                *p_ecode      ) );

extern gl_return_t  
gl_mm_enable_task_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t               pool_id,
                gl_mm_pool_usage_profile_t *p_usage_range_spec,
                gl_error_t                 *p_ecode     ) );

      
extern gl_return_t
gl_mm_disable_task_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_error_t                *p_ecode      ) );


extern gl_return_t  
gl_mm_get_task_pool_stack_trace

    _ARGS_( (   gl_pvoid_t                p_buf_ptr,
                gl_error_t                *p_ecode      ) );


extern gl_return_t  
gl_mm_get_task_pool_buffer_trace

    _ARGS_( (   gl_pvoid_t                p_buf_ptr,
                gl_error_t                *p_ecode      ) );

    
extern gl_return_t  
gl_mm_report_task_pool_usage_profiling

    _ARGS_( (   gl_pool_id_t              pool_id,
                gl_error_t                *p_ecode      ) );

    
extern gl_return_t  
gl_mm_report_task_pool_busy_buffers_statistics

    _ARGS_( (   gl_pool_id_t              pool_id,
                gw_U32bit                 task_id,
                gw_U8bit                  buffer_alloc_sig,
                gl_error_t                *p_ecode      ) );
    
extern gl_return_t  
gl_mm_set_buffer_alloc_signature

    _ARGS_( (   gw_U8bit                  buffer_alloc_sig,
                gl_error_t                *p_ecode      ) );
    
#endif /* For GL_MM_ADD_DEBUGGER */    



#ifdef GL_MM_ADD_AUDIT_MGT

extern gl_return_t
gl_mm_set_cmn_pool_audit_activity
 
    _ARGS_( (   gw_boolean                is_enable,
                gw_U32bit                 max_num_of_user_id,
                gw_U32bit                 special_user_id,
                gl_error_t                *p_ecode      ) );
    


extern gl_return_t
gl_mm_get_busy_user_id_info_list
 
    _ARGS_( (   gw_boolean                info_list_from_first_id,
                gw_U8bit                 *p_num_of_id,
                gw_U32bit                 a_user_id[],
                gl_user_id_info_t         *a_user_id_info[],
                gw_boolean                *p_is_more_ids_to_traverse,
                gl_error_t                *p_ecode      ) );



extern gl_return_t
gl_mm_get_user_id_status
 
    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 *p_num_of_bufs_alocated,
                gl_time_stamp_t           *p_time_stamp,
                gl_error_t                *p_ecode      ) );


extern gl_return_t
gl_mm_release_all_user_id_aloc_bufs
 
    _ARGS_( (   gw_U32bit                 user_id,
                gw_U32bit                 *p_num_of_bufs_released,
                gl_error_t                *p_ecode      ) );

    
#endif /* Endif for GL_MM_ADD_AUDIT_MGT */


    
#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
