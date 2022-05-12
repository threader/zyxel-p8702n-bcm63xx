/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Memory Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_mm.h
*
*    DESCRIPTION : This file contains the defines and function 
*                  declarations of memory manager sub-module of the 
*                  generic library implementation. The functions include 
*                  those for creating the memory pools and those for 
*                  allocation and deallocation of memory buffers 
*                  from the created pools dynamically. (header file)
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* Oct'99        Kapil Nayar     HSS:40600026    Generic Libraries
*               HSS:40600036    Gateway
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code.
* 24/10/2001    Ravi Jindal     SPR-2789        Change pool-id in memory 
*                                               manager from U8bit to U16bit.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
* 22/11/2001    Ravi Jindal     SPR-3225        Add Congestion Management 
*                                               modules related structures.
* 30/11/2001    Ravi Jindal     SPR-2788        Re-Packaging of Genlib-Code.
* 18/12/2001    Ravi Jindal     SPR-3363        Compilation with CC.
* 27/12/2001    Ravi Jindal     SPR-3412        To add the task specific common 
*                                               Pool APIs. The buffer from task
*                                               specific pool are taken on BEST
*                                               FIT mechanism among all the task
*                                               specific common pools created 
*                                               with same task-id.
*
* 10/03/2002    Ravi Jindal     SPR-3952        Enhancements for Rel-1.0.
* 
* COPYRIGHT , Aricent, 2006
****************************************************************************/

#ifndef _GL_MM_H_
#define _GL_MM_H_

#include "gl_mm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"

#include "gw_pragma_defs.h"

#ifdef GL_MM_ADD_AUDIT_MGT
    #include "gl_htm_interfaces.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif
    
#define GL_MM_BUF_INDEX(x,y,z) ((gw_S8bit *)(x) - (gw_S8bit *)(y)) / (z)
/*
 * Flag used to include debugging features.
 */
#ifdef GL_MM_ADD_DEBUGGER

/*
 * Structure to store Stack Trace inforamtion.
 */
typedef struct gl_mm_stk_inf_s
{
    gw_U8bit                    num_of_entries;
    gl_pvoid_t                  stk_func_add[GL_MM_STK_TR_DEPTH];
}gl_mm_stk_inf_t;

/*
 * Structure to store Buffer Trace inforamtion.
 */
typedef struct gl_mm_buf_trace_s
{
    gw_U32bit                   task_id;
    gw_U8bit                    buf_status;
    gw_U8bit                    buffer_alloc_signature;
}gl_mm_buf_trace_t;
/*
 * Structure to trace back the System Stack to reterive function addresses.
 */
typedef struct gl_mm_layout_s
{
#ifdef GL_SOLARIS_6
    gw_U32bit                   a[14];
#endif
    struct gl_mm_layout_s       *p_next;
    gl_pvoid_t                  return_address;
}gl_mm_layout_t;
/*
 * Enum used to enable Stack Tracing, Buffer Tracing and Pool Usage.
 */
typedef enum 
{
    GL_MM_NO_ACTIVITY       = 0,
    GL_MM_ENABLE_STK_INF    = 1,
    GL_MM_ENABLE_BUF_TRACE  = 2,
    GL_MM_ENABLE_POOL_USAGE  = 4
}gl_mm_is_enable_et;

#endif

/*
 * Structure to detect Memory overwritten Occurs or not.
 * And itb contain the pool-id used in release buffer calls.
 */
typedef struct gl_mm_check_mem_overwrite_s
{
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gw_U8bit                    start_check_bit;
#endif
    gl_pool_id_t                pool_id;
#ifdef GL_MM_ADD_MEM_OVERW_OPR
    gw_U8bit                    end_check_bit;
#endif
    /*
     * To allign with 4-byte word boundry.
     */
    gw_U16bit                   to_allign;
}gl_mm_check_mem_overwrite_t;


/*
 * Structure of the header of the memory buffer.
 */
typedef struct gl_mm_buffer_hdr_s
{
    /*
     * Pointer to the previous available buffer.
     */
    struct gl_mm_buffer_hdr_s   *p_prev; 
    /* 
     * Pointer to the next available buffer.
     */
    struct gl_mm_buffer_hdr_s   *p_next;
    /* 
     * Task Id to which the buffer belongs - used in case of task memory pools.
     */
    gw_U32bit                   task_id; 
    /*
     * Memory pool Id to which the buffer belongs.
     */
    gl_pool_id_t                pool_id;
    /*
     * GL_TRUE when the buffer is allocated state.
     */
    gw_S8bit                    busy_flag; 
    
#ifdef GL_MM_ADD_DEBUGGER
    /*
     * To mark the buffers with user specific signature.
     * This signature value is used to identify buffers allocated for a flow.
     */
    gw_U8bit                    buffer_alloc_signature;
    /*
     * Index to entry of current buf trace region.
     */ 
    gw_U32bit                   curr_buf_trace_index;
#endif

}gl_mm_buffer_hdr_t;


/*
 * Structure of the memory buffer.
 */
typedef struct gl_mm_buffer_s
{
    gl_mm_buffer_hdr_t          buf_hdr;
    /*
     * Header of the buffer.
     */
    gw_S8bit                    *p_buf[1]; 
    /*
     * Pointer to the buffer which is allocated to the calling 
     * functional module.
     */ 
}gl_mm_buffer_t;



typedef struct gl_mm_mem_overwrite_s
{
    gl_mm_buffer_hdr_t       *p_first;
    gw_U32bit                count;
}gl_mm_mem_overwrite_st;

/*
 * Structure of the memory pool.
 */
typedef struct gl_mm_pool_s
{
    /*
     * Pointer to the first available memory buffer in the pool.
     */
    gl_mm_buffer_hdr_t          *p_first;
    /*
     * Pointer to the last available memory buffer in the pool.
     */
    gl_mm_buffer_hdr_t          *p_last; 
    /*
     * Start address of header regions.
     * It is the start address of memory allocated for pool. 
     */
    gl_mm_buffer_hdr_t          *p_hdr_region;
    /*
     * Start address of Buffer regions.
     */
    gw_S8bit                    *p_buf_region; 
    /*
     * Total number of buffers in the memory pool.
     */
    gw_U32bit                   buf_qty; 
    /*
     * Size of each buffer in bytes.
     */
    gw_U32bit                   buf_size; 
    /*
     * Number of buffers in use currently.
     */
    gw_U32bit                   busy_buf_qty; 
    /*
     * Taskid of the owner module of the memory pool.
     */
    gw_U32bit                   owner_task_id;
    /*
     * Semaphore required to be secured before operating upon
     * the linked list of free buffers.
     */
#ifdef GL_MT_ENABLED
    GL_MUTEX_ID                 mem_sem;
#endif
    /*
     * Pointer to the next higher memory buffer size pool - 
     * only for common pools.
     */
    struct gl_mm_pool_s         *p_next_high_mem_buf_pool; 
    /*
     * Congestion Management region information structure.
     */
    gl_cngst_mgmt_fn_t          cngst_mgmt_call_back;
    /*
     * Pointer to Congestion Management Function,
     * to be invoked on transition fron one region to other.
     */
    gl_cngst_mgmt_spec_t        specs;
    /*
     * Flag to indicate Congestion Management Activeti is enabled or not.
     */
    gl_boolean_t                is_cngst_mgmt_enabled;
    /*
     * Index to current congestion region.
     */ 
    gw_U8bit                    current_region_index;
    
    /*
     * Flag used to include debugging features.
     */
#ifdef GL_MM_ADD_DEBUGGER
    /*
     * Value indicating maximum usage of buffers from the pool.
     */ 
    gw_U32bit                   curr_max_used;
    /*
     * Pointer to store the System Stack information structure.
     * (the function pointers stored in Stack, at invokation of buffer 
     * allocation).
     */ 
    gl_mm_stk_inf_t             *p_start_stk_inf_reg;
    /*
     * Pointer to store the Buffer Header Traceing inforamtion structure.
     * (the header contents are stored, at allocation, deallocation and 
     * change owner of buffer).
     */ 
    gl_mm_buf_trace_t           *p_start_buf_trace_reg;
    /*
     * Pointer to Pool usage information structure.
     * (used to decide the size of buffer for a pool, after execution to reduce
     * internal fragmentation).
     */
    gl_mm_pool_usage_profile_t  *p_pool_usage_profile;
    /*
     * Flag used to indicate, whether Stack Tracing, Buffer Tracing and 
     * Pool Usage is enabled or not.
     * This take value from "gl_mm_is_enable_et" enum.
     */
    gw_U8bit                    is_enabled_flag;
    /*
     * Number of buffer trace informations per buffer.
     */
    gw_U32bit                   num_of_buf_trc_reqs;

#endif
    /* gl_mm_mem_overwrite_st      mem_overwrite; */
}gl_mm_pool_t;

typedef struct gl_task_specific_pool_info_s
{
    gw_U32bit                   task_id;
    /*
     * Pointer to smallest buffer size created with given task-id.
     */ 
    gl_mm_pool_t                *g_p_smallest_task_cmn_pool;
}gl_task_specific_pool_info_t;


typedef enum gl_mm_pool_flag_s
{
    GL_MM_COMMON_POOL=1,
    GL_MM_TASK_POOL
}gl_mm_pool_flag_t;

typedef enum gl_mm_buf_status_e
{
    GL_MM_BUF_BUSY = 0,
    GL_MM_BUF_FREE,
    GL_MM_BUF_CORROUPTED
}gl_mm_buf_status_et;



typedef struct gl_mm_user_id_info_node_s
{
    gl_user_id_info_t           user_id_info;
    gl_mm_buffer_hdr_t          *p_first_aloc_buf;
}gl_mm_user_id_info_node_t;



/**************************************************
 * Array of common memory pool structures to store 
 * common memory pools at runtime.
 **************************************************/

extern  gl_mm_pool_t *g_a_p_glb_pool_table[GL_MM_MAX_COMMON_POOL_QTY]; 

/*
 * Pointer to common pool structure with 
 * smallest memory buffer amongst common pools.
 */
extern  gl_mm_pool_t* p_gl_smallest_mem_buf_pool; 

/*
 * Array of task specific memory pool structures to store 
 * task specific memory pools at runtime.
 */
extern  gl_mm_pool_t *g_a_p_app_sp_pool_table[GL_MM_MAX_TASK_POOL_QTY]; 

    
extern gw_S8bit               init_flag_cmn_pools;
extern gw_S8bit               init_flag_task_pools;


/*
 * Semaphores required for MT safe option.
 */
#ifdef GL_MT_ENABLED
extern  GL_MUTEX_ID mm_mgr_sem;
#endif

/*
 * Array to store task-id related smallest pool-id.
 * It is required for task specific common pools.
 */
extern gl_task_specific_pool_info_t a_pool_info[GL_MM_MAX_TASK_POOL_QTY];
extern gl_task_specific_pool_info_t *g_a_p_pool_info[GL_MM_MAX_TASK_POOL_QTY];
/*
 * Index to pool info array, at which new entry is stored.
 */ 
extern gw_U16bit free_entry_index;                 


#ifdef GL_MM_ADD_DEBUGGER
    extern gw_U8bit             buffer_alloc_signature;
#endif

#ifdef GL_MM_ADD_AUDIT_MGT
    extern gw_boolean           audit_act_set_flag;
    extern gl_hash_table_t      g_mm_audit_hash_table_id;
    extern gw_U32bit            g_mm_audit_special_user_id;
    extern gl_pvoid_t           g_p_audit_hash_walk_handle;

#ifdef GL_MT_ENABLED
    extern GL_MUTEX_ID          mm_audit_sem;
#endif
    
#endif /* Endif for GL_MM_ADD_AUDIT_MGT */
    
extern  gl_return_t
mm_validate_task_pool_buffer_pointer 

    _ARGS_( (   gw_U16bit               pool_id, 
                gl_pvoid_t              buf_ptr,
                gw_U32bit               *p_buf_index    ) );
    
    
extern  gl_return_t 
mm_validate_common_pool_buffer_pointer 
    
    _ARGS_( (   gw_U16bit               pool_id,
                gl_pvoid_t              buf_ptr,
                gw_U32bit               *p_buf_index    ) );
    

extern  gl_return_t
mm_check_current_cngst_mgmt_region

    _ARGS_( (   gl_pool_id_t            pool_id,
                gl_mm_pool_t            *p_pool,
                gl_error_t              *p_ecode    ) );


#ifdef GL_MM_ADD_DEBUGGER
    
extern  gl_return_t  
mm_stack_trace_update

    _ARGS_( (   gl_mm_pool_t              *p_pool_ptr,
                gw_U32bit                 buf_index     ) );
    
    
extern  gl_return_t  
mm_buffer_trace_update

    _ARGS_( (   gl_mm_pool_t              *p_pool_ptr,
                gw_U32bit                 buf_index,
                gw_U8bit                  buf_status,
                gw_U32bit                 task_id,
                gl_mm_buffer_hdr_t        *p_buf_hdr    ) );
    
extern  gl_return_t  
mm_pool_usage_update

    _ARGS_( (   gl_mm_pool_t              *p_pool_ptr,
                gw_U32bit                 buf_size     ) );
    
#endif
    
#ifdef GL_MM_ADD_AUDIT_MGT

extern gl_return_t   
mm_get_user_id_info_node
 
    _ARGS_( (   gw_U32bit                 user_id,
                gl_mm_user_id_info_node_t **p_p_user_id_info_node,
                gw_boolean                *p_is_id_found,
                gl_error_t                *p_ecode      ) );


      
extern gl_return_t
mm_remove_user_id_info_node
 
    _ARGS_( (   gw_U32bit                 user_id,
                gl_mm_user_id_info_node_t **p_p_user_id_info_node,
                gw_boolean                *p_is_id_found,
                gl_error_t                *p_ecode      ) );



extern gl_return_t
mm_release_all_id_allocated_bufs
 
    _ARGS_( (   gw_U32bit                 user_id,
                gl_mm_buffer_hdr_t        *p_buf_hdr,
                gl_error_t                *p_ecode      ) );



extern gl_return_t
mm_add_buf_to_user_id_info_node
 
    _ARGS_( (   gw_U32bit                 user_id,
                gl_mm_buffer_hdr_t        *p_buf_hdr,
                gl_error_t                *p_ecode      ) );



extern gl_return_t
mm_release_buf_from_user_id_info_node
 
    _ARGS_( (   gw_U32bit                 user_id,
                gl_mm_buffer_hdr_t        *p_buf_hdr,
                gl_error_t                *p_ecode      ) );      

extern gl_return_t
mm_audit_memory_free_func

    _ARGS_( (   gw_U32bit                 user_id,
                gl_hash_key_ut            *p_key,
                gw_U8bit                  key_type,
                gl_pvoid_t                p_element,
                gl_error_t                *p_ecode       ) );
    
    
#endif
    

#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
