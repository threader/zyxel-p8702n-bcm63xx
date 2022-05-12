/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Timer Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_tm.h
*
*    DESCRIPTION : Timers which are inherent features of any real time system
*                  are implemented as ported feature by this generic library 
*                  sub-module.
*                  Various APIs for starting, stopping timers etc. as well as 
*                  optional implementation of timer server in case of 
*                  multi-threaded applications are all part of this file. 
*                  (header file)
*
*   DATE            NAME        REFERENCE           REASON
*-----------   --------------  -------------   -----------------------------   
*
* Oct'99        Kapil Nayar     HSS:40600026    Generic Libraries for 
*                               HSS:40600036    Gateway
*
* 18/08/2001    Prashant Jain   SPR 2276        Compiling with CC on Solaris.
*
* 04/09/2001    Prashant Jain   SPR-2560        Adding OSE Operating System 
*                                               Support.
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code.
* 24/10/2001    Ravi Jindal     SPR-2791        typedef the timestamp related
*                                               data structures.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
*
* 22/11/2001    Prashant Jain   SPR-3223        Change the way, the Time
*                                               Stamp value is retrieved in
*                                               gl_tm_check_timer_expiry and
*                                               then subsequently used. For
*                                               this, data structure is modified.
* 30/11/2001    Ravi Jindal     SPR-2788        Re-Packaging of Genlib-Code.
*
* 15/01/2002    Ravi Jindal     SPR-3452        Re-Packaging of Genlib-Code.
*
* 19/03/2002    Ravi Jindal     SPR-3954        Enhancements for Rel-1.0.
*
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_TM_H_
#define _GL_TM_H_

#include "gl_tm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif
 
/* 
 * Structure for timer related information.
 */
typedef struct gl_tm_timer_info_s
{
    gl_timer_id_t               timer_id; 
    /*
     * Timerid of the timer.
     */
    gw_U8bit                    is_timer_active;
    /*
     * = GL_TRUE, if timer is active
     * = GL_FALSE, if timer is free.
     */ 
    gw_U32bit                   time_to_expire; 
    /*
     * Milliseconds after which timer expires.
     */
    gw_U32bit                   timer_duration; 
    /*
     * Timer_duration for the timer in milliseconds.
     */
    gw_U8bit                    restart_flag;
    /*
     * Flag for restarting or stopping the timer after expiry.
     */
    struct gl_tm_timer_info_s   *fptr; 
    /*
     * Pointer to the next timer buffer.
     */
    struct gl_tm_timer_info_s   *bptr; 
    /*
     * Pointer to the previous timer buffer.
     */
}gl_tm_timer_info_t;

/*
 * Structure for timer info maintained by the according to the timer index.
 */
typedef struct gl_tm_timer_index_info_s
{
    gl_tm_timer_info_t            a_timer_info;
    /*
     * Pointer to the timer info buffer.
     */
}gl_tm_timer_index_info_t;

/*
 * Structure for active timers storage.
 */
typedef struct gl_tm_timer_list_s
{
#ifdef GL_TOTAL_MT_SAFE
    GL_MUTEX_ID                 timer_list_sem;
#endif

    gl_tm_timer_info_t          *p_first; 
    /*
     * Pointer to the first timer buffer in the timer list.
     */
    gl_tm_timer_info_t          *p_last; 
    /*
     * Pointer to the last timer buffer in the timer list.
     */
    gw_U32bit                   user_info;
    /*
     * Some information specific to the timer_list_id and not used by the 
     * timer module or server.
     */
    gw_U32bit                   max_time;
    /* 
     * Maximum milliseconds after which all the active 
     * timers in the timer list expire or total milliseconds
     * remaining before all timers expire.
     */
    gl_time_stamp_t             last_time_stamp;
    /*
     * To store the last time stamp checked.
     */
    gw_U32bit                   active_timer_qty;
    /*
     * Number of active timers in the timer list.
     */
    gl_tm_timer_index_info_t*   a_index_info; 
    /*
     * Stores p_timer_info indexed with timer_id in the array.
     */
}gl_tm_timer_list_t;


/*
 * Structure for timer index which stores the free timer ids and 
 * associated pointers for active timer ids.
 */
typedef struct gl_tm_timer_index_s
{
    gl_timer_id_t               next_index; 
    /*
     * Next free timer id available.
     */
    gl_pvoid_t                  p_timer_buf;
    /*
     * Timer buffer pointer.
     */
    gl_timer_call_back_func_t   func_ptr;
    /*
     * Pointer to the handler function 
     * which needs to be invoked at timer expiry.
     */
}gl_tm_timer_index_t;

/*
 * Structure for active timers storage in application for getting and 
 * setting free timer_ids.
 */
typedef struct gl_tm_timer_app_list_s
{
    /*
     * Maximum number of timers in the timer list.
     */
    gw_U32bit                   max_timer_qty; 
    
    /*
     * Variables storing the timer index and related information about pointers 
     * associated with a timer_id.
     */
    gl_timer_id_t               first_free_timer_index;
    gl_timer_id_t               last_free_timer_index;
    gl_tm_timer_index_t*        a_timer_index;
}gl_tm_timer_app_list_t;


typedef enum
{
    GL_TM_MAIN_EVENT=0,
    /*
     * Index of event signalled by main loop for invoking 
     * gl_tm_check_timer_expiry in the timer server task.
     */
    GL_TM_INPUT_EVENT,
    /*
     * Index of event signalled by timer application whenever a start or 
     * stop timer request is sent to the timer server.
     */
    GL_TM_MAX_EVENT
    /*
     * Added to used as a index to event array.
     */
}gl_tm_event_t;



/*************************************************************
 * Global Variables DECLARATION.
 ************************************************************/


/*
 * Array of timer index maintained by the timer application independent
 * of the timer server.
 */
extern   gl_tm_timer_list_t        *a_p_timer_list[GL_TM_MAX_TIMER_LIST];

extern   gl_tm_timer_app_list_t    a_timer_app_list[GL_TM_MAX_TIMER_LIST];

extern gw_U32bit  gl_timer_resolution;

extern gl_boolean gl_tm_timer_server_flag;

#ifdef GL_MT_ENABLED

extern GL_MUTEX_ID            tm_mgr_sem;

/* 
 * Controls timer task.
 */
extern GL_SEMAPHORE_ID                 gl_tm_timer_task;     
/* 
 * Event for timer tick.
 */
extern GL_SEMAPHORE_ID                 gl_tm_main_event;   
/*
 * Event for client starting timer.
 */
extern GL_SEMAPHORE_ID                 gl_tm_input_event; 

#endif

extern gl_boolean             tm_mgr_init_flag;


/*************************************************************
 * LOCAL FUNCTIONS DECLARATION.
 ************************************************************/


extern void
tm_check_timer_list_expire

    _ARGS_( (   gl_timer_list_t         timer_list_id   ) );

    
extern  gl_return_t
save_mesg

    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id,
                gw_U32bit               timer_duration, 
                gw_U8bit                restart_flag      ) );

    
extern  gl_return_t
resave_mesg

    _ARGS_( (   gl_timer_list_t         timer_list_id, 
                gl_tm_timer_info_t      * p_timer_info    ) );

    
extern  gl_return_t
extract_mesg
    
    _ARGS_( (   gl_timer_list_t         timer_list_id, 
                gl_timer_id_t           timer_id      ) );

    
extern  gl_timer_id_t
get_free_timer

    _ARGS_( (   gl_timer_list_t         timer_list_id   ) );
    

extern  gl_return_t
timer_id_list_init

    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gw_U32bit               timer_qty     ) );
    

extern  void
set_free_timer

    _ARGS_( (   gl_timer_list_t         timer_list_id,
                gl_timer_id_t           timer_id      ) );

#ifdef GL_MT_ENABLED    
#ifndef GL_VXWORKS
extern gl_thread_return_t
gl_tm_timer_server
    _ARGS_( (    gl_pvoid_t              p_buf    ) );
#else
extern gl_thread_return_t
gl_tm_timer_server( void );
#endif
#endif
    


#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
