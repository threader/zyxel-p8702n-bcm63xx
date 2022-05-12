/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - File Operations and 
*                  Mutex, Semaphores, Threads Operation related Os independent 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_gscim_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of file manager sub-module 
*                  of the generic library implementation.
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
* 02/04/2002 Ravi Jindal        SPR-3959        Enhancements for Rel-1.0.
*
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_GSCIM_INTERFACES_H_
#define _GL_GSCIM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_ulm_interfaces.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************
* Data-Structures for File Operations.
*****************************************/

typedef enum
{
    GL_FM_RDONLY,
    GL_FM_WRONLY,
    GL_FM_APONLY,
    GL_FM_RDWR_EXIST,
    GL_FM_RDWR_NEW,
    GL_FM_RDAP,
    GL_FM_RDONLY_BINARY,
    GL_FM_WRONLY_BINARY,
    GL_FM_APONLY_BINARY,
    GL_FM_RDWR_EXIST_BINARY,
    GL_FM_RDWR_NEW_BINARY,
    GL_FM_RDAP_BINARY
        
}gl_fm_mode_et;

typedef enum
{
    GL_FM_BEGIN,
    GL_FM_CURR,
    GL_FM_END
}gl_fm_origin_et;


/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs - File Manager
 *---------------------------------------------------------------------------*/

extern gl_pvoid_t
gl_fm_open_file
            
    _ARGS_( (   gw_S8bit                *filename, 
                gl_fm_mode_et           mode,
                gl_error_t              *p_ecode    ) );  
                 
            

extern gl_return_t
gl_fm_close_file
            
    _ARGS_( (   gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
      
            

extern gl_return_t
gl_fm_seek_file
            
    _ARGS_( (   gl_pvoid_t              fd,
                gw_S32bit               offset,
                gl_fm_origin_et         origin,
                gl_error_t              *p_ecode    ) );
       
             

extern gl_return_t
gl_fm_read_file
            
    _ARGS_( (   gl_pvoid_t              p_buffer,
                gw_U32bit               size,
                gw_U32bit               count, 
                gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
       
            

extern gl_return_t 
gl_fm_write_file
            
    _ARGS_( (   gl_pvoid_t              p_buffer,
                gw_U32bit               size, 
                gw_U32bit               count,
                gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
       
             

extern gl_boolean_t
gl_fm_end_of_file
            
    _ARGS_( (   gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
        
             

extern gl_return_t
gl_fm_flush_file
            
    _ARGS_( (   gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
       
             

extern gl_boolean_t
gl_fm_error_in_file
            
    _ARGS_( (   gl_pvoid_t              fd,
                gl_error_t              *p_ecode    ) );
       


#ifdef GL_MT_ENABLED

/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs PORT MANAGER
 *---------------------------------------------------------------------------*/
/*
 * It contains Thread-id or Handle to thread.
 */ 
typedef struct gl_thread_info_s
{
    /*
     * For Future enhancements Structure is Used.
     */ 
    union
    {
        gl_thread_id_t          thr_id;
        gl_thread_handle_t      thr_handle; 
    }thread_id;
    
}gl_thread_info_t; 


extern gl_return_t
gl_mutex_init( void );


extern gl_return_t
gl_create_mutex 
    
    _ARGS_( (   GL_MUTEX_ID             *p_mutex_id,
                gl_error_t              *p_ecode        ) );



extern gl_return_t 
gl_delete_mutex
    
    _ARGS_( (   GL_MUTEX_ID             *p_mutex_id,
                gl_error_t              *p_ecode        ) );




extern gl_return_t
gl_lock_mutex 
    
    _ARGS_( (   GL_MUTEX_ID             *p_mutex_id,
                gw_U32bit               flags,
                gl_error_t              *p_ecode        ) );


extern gl_return_t 
gl_unlock_mutex 
    
    _ARGS_( (   gl_thread_info_t        *p_thread_info,
                GL_MUTEX_ID             *p_mutex_id,
                gl_error_t              *p_ecode        ) );






/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs Thread Manager 
 *---------------------------------------------------------------------------*/
/*
 * These decleration are kept in gl_os_port.h as they are required 
 * by both os and db managers.
 */

extern gl_return_t 
gl_thm_init( void );



extern gl_return_t 
gl_thm_create
            
    _ARGS_( (   gl_fn_ptr_t             entry_func_name,
                gl_thread_info_t        *p_thread_info, 
                gl_pvoid_t              p_param, 
                gl_error_t              *p_ecode      ) );



extern gl_return_t 
gl_thm_destroy
      
    _ARGS_( (   gl_thread_info_t          *p_thread_info,
                gl_error_t                *p_ecode    ) );

            

extern gl_return_t 
gl_thm_suspend
      
    _ARGS_( (   gl_thread_info_t          *p_thread_info,
                gl_error_t                *p_ecode    ) );

            

extern gl_return_t 
gl_thm_resume
      
    _ARGS_( (   gl_thread_info_t          *p_thread_info,
                gl_error_t                *p_ecode    ) );

            

    
extern gl_return_t
gl_thm_get_thread_id

    _ARGS_( (   gl_thread_info_t         *p_thread_info,
                gl_error_t                *p_ecode    ) );




/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs Semaphore Manager 
 *---------------------------------------------------------------------------*/
/*
 * These decleration are kept in gl_os_port.h as they are required 
 * by both os and db managers.
 */


extern gl_return_t 
gl_create_semaphore

    _ARGS_( (   GL_SEMAPHORE_ID         *p_sem_id,
                gw_U32bit               initial_value,
                gw_U32bit               maximum_value,
                gl_error_t              *p_ecode    ) );




extern gl_return_t
gl_delete_semaphore

    _ARGS_( (   GL_SEMAPHORE_ID         *p_sem_id,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_wait_on_semaphore

    _ARGS_( (   GL_SEMAPHORE_ID         *p_sem_id,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_try_wait_on_semaphore

    _ARGS_( (   GL_SEMAPHORE_ID         *p_sem_id,
                gl_boolean_t            *p_is_already_locked,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_release_semaphore

    _ARGS_( (   GL_SEMAPHORE_ID         *p_sem_id,
                gl_error_t              *p_ecode    ) );


#endif /* Endif for GL_MT_ENABLED */


/*********************************************
* Data-Structures for Shared Memory Manager.
**********************************************/

/*
 * Access modes for shared memory operations.
 */
typedef enum gl_shm_acc_mode_s 
{
    GL_SHM_READ = 1,
    GL_SHM_WRITE,
    GL_SHM_READWRITE
} gl_shm_acc_mode_et;

/*
 * Memory attachment mode.
 */
typedef enum gl_shm_attach_mode_s 
{
    GL_SHM_ONLY_RESERVE = 1,
    GL_SHM_ONLY_COMMIT,
    GL_SHM_COMMIT_RESERVE
} gl_shm_attach_mode_et;

/*
 * Structure definition for the IN parameters to gl_shm_create API.
 */
typedef struct 
{
    gw_U32bit                   task_id;
    gw_U32bit                   shm_chunk_qty;
    gw_U32bit                   shm_chunk_size;
    gw_U8bit                    acc_mode; 
    gw_S8bit                    key_shmem[GL_SHM_MAX_NAME_SIZE];
} gl_shm_create_option_t;

/*
 * Structure definition for the OUT parameters to gl_shm_create API.
 */
typedef struct 
{
    gl_shmem_id_t               shmem_id;
} gl_shm_create_return_t;

/*
 * Structure definition for the IN parameters to gl_shm_map API.
 */
typedef struct 
{
    gw_U32bit                   task_id;
    gw_U32bit                   shm_chunk_qty;
    gw_U32bit                   shm_chunk_size;
    gw_U8bit                    acc_mode; 
    gw_S8bit                    key_shmem[GL_SHM_MAX_NAME_SIZE];
} gl_shm_map_option_t;

/*
 * Structure definition for the OUT parameters to gl_shm_map API.
 */
typedef struct 
{
    gl_shmem_id_t               shmem_id;
} gl_shm_map_return_t;    


/*
 * Structure definition for the IN parameters to gl_shm_alloc API.
 */
typedef struct
{
    gw_U32bit                   task_id;
    gl_shmem_id_t               shmem_id;
    gw_U32bit                   size; 
    gl_shm_acc_mode_et          acc_mode; 
} gl_shm_alloc_option_t;

/*
 * Structure definition for the OUT parameters to gl_shm_alloc API.
 */
typedef struct 
{
    gl_pvoid_t                   p_alloc_adds; 
} gl_shm_alloc_return_t;

/*
 * Structure definition for the IN parameters to gl_shm_dealloc API.
 */
typedef struct 
{
    gw_U32bit                   task_id;
    gl_shmem_id_t               shmem_id;
    gl_pvoid_t                  p_alloc_adds; 
} gl_shm_dealloc_option_t;

/*----------------------------------------------------------------------------
     EXTERNAL INTERFACE APIs Shared Memory Manager
----------------------------------------------------------------------------*/

extern gl_return_t
gl_shm_init( void );

extern gl_return_t
gl_shm_create_shared_memory
                
    _ARGS_( (   gl_shm_create_option_t      *p_gl_cr, 
                gl_shm_create_return_t      *p_gl_cr_ret,
                gl_error_t                  *p_ecode    ) );
          
                

extern gl_return_t
gl_shm_map_shared_memory

    _ARGS_( (   gl_shm_map_option_t         *p_gl_map,
                gl_shm_map_return_t         *p_gl_map_ret,
                gl_error_t                  *p_ecode    ) );
          
                

extern gl_return_t
gl_shm_alloc_shared_memory
                
    _ARGS_( (   gl_shm_alloc_option_t       *p_gl_al, 
                gl_shm_alloc_return_t       *p_gl_al_ret,
                gl_error_t                  *p_ecode    ) );
          
                

extern gl_return_t
gl_shm_dealloc_shared_memory
                
    _ARGS_( (   gl_shm_dealloc_option_t     *p_gl_dal, 
                gl_error_t                  *p_ecode    ) );
          
                

extern gl_return_t 
gl_shm_delete_shared_memory

    _ARGS_( (   gw_U32bit                   task_id, 
                gw_U32bit                   shmem_id, 
                gl_error_t                  *p_ecode    ) );

extern gl_return_t
gl_shm_destroy_all_shared_memory

    _ARGS_( (   gw_U8bit                    is_forceful_cleanup,
                gl_error_t                  *p_ecode    ) );

#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
