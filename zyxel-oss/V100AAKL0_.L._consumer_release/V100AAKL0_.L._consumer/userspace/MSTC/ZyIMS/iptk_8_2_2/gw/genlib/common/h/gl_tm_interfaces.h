/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Timer Manager 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_tm_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of timer manager sub-module 
*                  of the  generic library implementation. 
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
* 19/03/2002 Ravi Jindal        SPR-3954        Enhancements for Rel-1.0.
* 
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_TM_INTERFACES_H_
#define _GL_TM_INTERFACES_H_

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


typedef void (* gl_timer_call_back_func_t) (gl_pvoid_t);
typedef void (* gl_timer_singl_timeout_handler_func_t) (gw_U32bit,gl_pvoid_t);

typedef enum
{
    GL_TM_START_TIMER=1,
    GL_TM_STOP_TIMER,
    GL_TM_STOP_ALL_TIMER
}gl_tm_action_t;



/*----------------------------------------------------------------------------
 * EXTERNAL INTERFACE APIs Timer MANAGER
 *---------------------------------------------------------------------------*/

extern gl_return_t
gl_tm_init( void );

extern gl_return_t
gl_tm_timer_list_create
            
    _ARGS_( (   gw_U32bit               user_info,
                gw_U32bit               max_timer_qty,
                gl_timer_list_t         *p_timer_list_id, 
                gl_error_t              *p_ecode    ) );
       
            

extern gl_return_t 
gl_tm_start_timer
        
    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gw_U32bit               timer_duration,
                gl_timer_call_back_func_t func_ptr,
                gl_pvoid_t              p_timer_buf,
                gw_U8bit                restart_flag,
                gl_timer_id_t           *p_timer_id,
                gl_error_t              *p_ecode    ) );
      
             

extern gl_return_t
gl_tm_stop_timer
            
    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id, 
                gl_pvoid_t              *p_p_timer_buf,
                gl_error_t              *p_ecode    ) );
      
            

extern void
gl_tm_check_timer_expiry
            
    _ARGS_( (   gl_timer_list_t         timer_list_id   ) );
      
            

extern gl_return_t 
gl_tm_stop_all_timer
            
    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gw_U32bit               *p_timer_qty, 
                gl_timer_id_t           a_timer_id[],
                gl_pvoid_t              a_p_timer_buf[],
                gl_error_t              *p_ecode    ) );
      
            

extern gl_return_t 
gl_tm_get_timer_buff
        
    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id, 
                gl_pvoid_t              *p_p_timer_buf,
                gw_error_t              *p_ecode    ) );
        
               

extern void 
gl_tm_get_timer_list_ids 
        
    _ARGS_( (   gw_U32bit               user_info, 
                gw_U8bit                *p_num_timer_list_ids,
                gl_timer_list_t         *p_timer_list_ids       ) );
         
               

extern gl_return_t
gl_tm_delete_timer_list

    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_tm_destroy_all_timer_lists
            
    _ARGS_( (   gw_U8bit                is_forceful_flag,
                gl_error_t              *p_ecode     ) );
        
               


#ifdef GL_SINGLE_TIMEOUT
extern void
gl_single_timeout_handler
            
    _ARGS_( (   gw_U32bit               user_info,
                gl_pvoid_t              p_buf       ) );
               
#endif


/*------------------------------------------------------------
 * PORTING FUNCTIONS for Timer MANAGER as seperate entity - 
 * thread or process 
 *-----------------------------------------------------------*/

#ifdef GL_MT_ENABLED

extern void 
gl_tm_wake_up_timer_server( void );

extern gl_return_t
gl_tm_spawn_timer_server
            
    _ARGS_( (   gl_thread_info_t        *p_thread_info,
                gl_error_t              *p_ecode    ) );
            

extern gl_return_t 
gl_tm_invoke_timer_expiry_handler
            
    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id, 
                gw_U8bit                restart_flag,
                gl_error_t              *p_ecode        ) );
      
#endif

extern void
gl_tm_send_msg_to_server

    _ARGS_( (   gl_tm_action_t          action,
                gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id,
                gw_U8bit                restart_flag,
                gw_U32bit               timer_duration      ) );
      
             

extern gw_S8bit
gl_tm_recv_msg_from_app
        
    _ARGS_( (   gl_tm_action_t          *p_action, 
                gl_timer_list_t         *p_timer_list_id, 
                gl_timer_id_t           *p_timer_id,
                gw_U8bit                *p_restart_flag,
                gw_U32bit               *p_timer_duration   ) );
     
            

extern void
gl_tm_send_expiry_msg_to_app
        
    _ARGS_( (   gw_U32bit               user_info,
                gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id,
                gw_U8bit                restart_flag,
                gw_U32bit               timer_duration      ) );
     
    
#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
