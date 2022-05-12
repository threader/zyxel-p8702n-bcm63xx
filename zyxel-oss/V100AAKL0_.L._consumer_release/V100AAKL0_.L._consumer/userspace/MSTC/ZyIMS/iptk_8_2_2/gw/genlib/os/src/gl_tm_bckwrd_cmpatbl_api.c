/*****************************************************************************
*
*    FUNCTION    : Source file for Generic Library - Timer Manager
*                  backward compatibility of Rel-1.0 with Rel-0.2 
*                  implementation.
******************************************************************************
*
*    FILENAME    : gl_tm_bckwrd_cmpatbl_api.c
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

#include "gl_tm_interfaces_rel_02.h"


#ifdef GL_MT_ENABLED
/*****************************************************************************
* Function name    : gl_tm_wake_up_timer_server
* Arguments        : void
* Return Value     : void
* Functionality    : This function is invoked when timer server functionality
*                    is used to handle timers.
* Functions called :
*****************************************************************************/
void
gl_tm_wake_up_timer_server( void )

/****************************************************************************/
{
    /*
     * As in Rel-1.0 their is a single function call 
     * "gl_tm_check_timer_expiry" for both Timer Server active
     * and Without Timer Server.
     * As If timer Server is active "gl_tm_chreck_timer_expiry"
     * signal the timer server to check expiry of timers in all
     * the lists. 
     * The input parameter passed is dummy input parameter GL_NULL.
     */ 
    gl_tm_check_timer_expiry(GL_NULL);
}




/*****************************************************************************
* Function name    : gl_tm_timer_expiry_handler
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gl_timer_id_t      timer_id,
*                    gw_U8bit           restart_flag
* Return Value     : void
* Functionality    : Checks the Expired timer in the specified message and
*                    sets the timer free if non-cyclic timer before invoking
*                    corresponding function pointer with p_timer_buf. This
*                    function is called in case of timer server enabled by
*                    the application using the timers upon receipt of the
*                    timer expiry message from the timer server.
* Functions called : set_free_timer
*****************************************************************************/
void
gl_tm_timer_expiry_handler

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_timer_id_t             timer_id,
      gw_U8bit                  restart_flag
    )
#else
    ( timer_list_id, timer_id, restart_flag )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gw_U8bit                  restart_flag;
#endif

/****************************************************************************/
{
    /*
     * As the retun type is void and error code in not supported in
     * Rel-0.2.
     */
    gl_error_t                  ecode;

    gl_tm_invoke_timer_expiry_handler(timer_list_id,timer_id, restart_flag,
            &ecode);
}

    



/*****************************************************************************
* Function name    : gl_tm_start_timer_server
* Arguments        : gw_U32bit          *p_thread_id,
*                    gl_error_t         *p_ecode
* Return Value     : gw_U32bit
* Functionality    : Sets the timer server flag in the timer_list_id
*                    structure and creates a seperate task for the timer server
*                    before returning the handle value and the task id of the
*                    timer server.
* Functions called :
*****************************************************************************/

gw_U32bit
gl_tm_start_timer_server

#ifdef _ANSI_C_
    ( gl_thread_id_t            *p_thread_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_id, p_ecode )
      gl_thread_id_t            *p_thread_id;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    gl_thread_info_t            thread_info;
    
    if(GL_FAILURE == gl_tm_spawn_timer_server
                    (&thread_info, pecode))
    {
        return GL_NULL;
    }

#ifdef GL_WINNT
    /*
     * As on WinNT the handle to thread and thread-id are different
     * but in Rel-1.0 only handle to thread is used on WinNT.
     */ 
    *p_thread_id = (gl_thread_id_t)thread_info.thread_id.thr_handle;
    return((gw_U32bit)thread_info.thread_id.thr_handle);
#else
    *p_thread_id = thread_info.thread_id.thr_id;
    return((gw_U323bit)thread_info.thread_id.thr_id);
#endif
}


#endif /* End of GL_MT_ENABLED
    
    


