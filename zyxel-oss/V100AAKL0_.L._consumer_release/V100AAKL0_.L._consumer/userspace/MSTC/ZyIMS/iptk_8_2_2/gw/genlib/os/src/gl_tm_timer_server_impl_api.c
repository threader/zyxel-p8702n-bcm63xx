/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Timer Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_tm_timer_server_impl_api.c
*
*    DESCRIPTION : Timers which are inherent features of any real time system
*                  are implemented as ported feature by this generic library 
*                  sub-module.Various APIs for starting, stopping timers etc. 
*                  as well as optional implementation of timer server in case 
*                  of multi-threaded applications are all part of this file. 
*                  (source file)
*
*
* DATE          NAME           REFERENCE          REASON
*----------- --------------  ---------------    ----------------------
*
* Oct'99     Kapil Nayar        HSS:40600026    Generic Libraries
*            HSS:40600036       (Gateways)
*
* 19.3.2000  Kapil Nayar            -           Modification of timer 
*                                               resolution implementation.
*
* 21/03/2001 Shobhit Bansal         -           Modified to add porting for 
*                                               Solaris
* 18/05/2001 Menuka Jain            -           Modified to merge generic
*                                               lib with interop code and 
*                                               added additional 
*                                               functionalities like time-
*                                               stamp
* 28/05/2001 Shobhit Bansal         -           Adding function to add two 
*                                               timestamps
* 18/06/2001 Shobhit Bansal         -           Adding function to convert
*                                               sec to date time format
* 28/08/01   Prashant Jain       SPR-2276       Compilation With CC.
*
* 28/08/01   Prashant Jain       SPR-2478       Fix in gl_tm_start_timer
*                                               routine for timer start 
*                                               failures.
* 04/09/2001 Prashant Jain       SPR-2560       Adding OSE Operating System
*                                               Support.
* 04/10/2001 Ravi Jindal         SPR-2751       Remove warnings with gcc,cc,  
*                                               CC on Solaris.         
* 18/10/2001 Ravi Jindal         SPR-2788       Packaging of Genlib-Code. 
* 19/10/2001 Ravi Jindal         SPR-2802       Adding Pre-ANSI-C features.
* 30/10/2001 Ravi Jindal         SPR-2744       Adding de-init functions. 
* 17/11/2001 Ravi Jindal         SPR-3107       Incoprate the defects and 
*                                               change cosmmetics of file.
*
* 22/11/2001 Prashant Jain       SPR-3223       Change the way, the Time
*                                               Stamp value is retrieved in
*                                               gl_tm_check_timer_expiry and
*                                               then subsequently used.
*
* 22/11/2001 Prashant Jain       SPR-2718       Modified the GL Timer Manager
*                                               Timer Expiry Check algorithm
*                                               to reduce the time expenditure
*                                               in gl_tm_check_timer_expiry.
*
* 27/11/2001 Prashant Jain       SPR-3263       The routine 
*                                               "gl_tm_check_timer_expiry"
*                                               hangs in a while loop if there
*                                               happens to be no timer expiry
*                                               in any cycle.
*
* 27/11/2001 Prashant Jain       SPR-3268       GL enters in a Infinite Loop  
*                                               if "gl_tm_init" is called 
*                                               more than once. This is 
*                                               because the variable 
*                                               "tm_mgr_init_flag" was not getting 
*                                               reset when all Timer Lists are
*                                               destroyed through the 
*                                               exported API interface.
*
* 31/12/2001 Prashant Jain       SPR-2744       When de-initialization is 
*                                               invoked, INIT flag should be 
*                                               checked to take care of the
*                                               out-of-sequence calling of
*                                               DE-INIT APIs or for that 
*                                               matter, any other API.
*
* 19/03/2001 Ravi Jindal         SPR-3954       Enhancements for Rel-1.0.
* 
* 10/10/2001 Prashant Jain       SPR-4931       Invalid check on Semaphore 
*                                               Try Wait Operation in the 
*                                               gl_tm_timer_server.
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_tm.h"


#ifdef GL_MT_ENABLED

/*****************************************************************************
* Function name    : gl_tm_timer_server
* Arguments        : void*
* Return Value     : void
* Functionality    : The timer server application running as a seperate thread.
* Functions called :
*****************************************************************************/
#ifndef GL_VXWORKS
gl_thread_return_t
gl_tm_timer_server
#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buf )
#else
    ( p_buf )
      gl_pvoid_t                p_buf;
#endif

#else
gl_thread_return_t
gl_tm_timer_server( void )
#endif
        
/****************************************************************************/
{
    gl_timer_list_t             timer_list = GL_NULL;
    gl_timer_id_t               timer_id= GL_NULL;
    gw_U32bit                   timer_duration= GL_NULL;
    gw_U8bit                    restart_flag= GL_NULL;
    gl_tm_action_t              action = GL_TM_START_TIMER;
    gl_tm_timer_info_t          *p_timer_info=GL_NULL;
    gl_tm_timer_info_t          *p_temp_timer_info=GL_NULL;
    gl_timer_list_t             list_id;
    gw_U32bit                   event_index;
    gl_error_t                  ecode;
    gl_boolean_t                is_already_locked;
#ifndef GL_VXWORKS
    /*
     * To remove warning.
     */
    p_buf = p_buf; 
#endif
    
    /*
     * Infinitely wait for an event from the application's main thread
     * or from the application thread for starting  or stoping timer messages.
     */
    while(1)
    {
        event_index = GL_TM_MAX_EVENT;
        if(GL_SUCCESS == gl_wait_on_semaphore(&gl_tm_timer_task, &ecode))
        {
            event_index = GL_TM_MAX_EVENT;
        }
        if((GL_SUCCESS == gl_try_wait_on_semaphore(&gl_tm_input_event, 
         &is_already_locked, &ecode)) && (is_already_locked == GL_FALSE))
        {
            event_index = GL_TM_INPUT_EVENT;
        }
        else if((GL_SUCCESS == gl_try_wait_on_semaphore(&gl_tm_main_event, 
         &is_already_locked, &ecode)) && (is_already_locked == GL_FALSE))
        {
            event_index = GL_TM_MAIN_EVENT;
        }
        
        if(event_index == GL_TM_MAX_EVENT)
        {
            /* 
             * This case shall not occur.
             */
            continue;
        }
        switch(event_index)
        {
            case GL_TM_INPUT_EVENT:
                
                /*
                 * If timer message event from application flush
                 * the messages received  by calling recv message function in
                 * while loop.
                 */
                while(((gw_S8bit)GL_ERROR) != (gl_tm_recv_msg_from_app
                (&action, &timer_list, &timer_id, &restart_flag,
                 &timer_duration)))
                {
                    /*
                     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
                     */
                    timer_list = (gl_timer_list_t)(timer_list - 1);

                    switch(action)
                    {
                        /*
                         * If start timer message.
                         */
                        case GL_TM_START_TIMER:
                            if(GL_SUCCESS != save_mesg(timer_list,
                                             timer_id,timer_duration,
                                             restart_flag))
                            {
                                GL_INC_TM_G_START_TIMER_FAIL()
                            }
                            break;
                        /*
                         * If stop timer message.
                         */
                        case GL_TM_STOP_TIMER:
                            if(GL_SUCCESS != extract_mesg(timer_list, 
                                                          timer_id))
                            {
                                GL_INC_TM_G_STOP_RQ_TIMER_ALDY_EXP()
                            }
                            break;
                        /*
                         * If stop all timers message.
                         */
                        case GL_TM_STOP_ALL_TIMER:
                          for(p_timer_info=a_p_timer_list[timer_list]->p_first;
                              p_timer_info != (gl_pvoid_t)GL_NULL; )
                            {
                                p_temp_timer_info = p_timer_info->fptr;
                                p_timer_info->is_timer_active = GL_FALSE;
                                p_timer_info->fptr = p_timer_info->bptr =
                                    GL_NULL;
                                p_timer_info = p_temp_timer_info;
                            }
                            a_p_timer_list[timer_list]->active_timer_qty=0;
                            a_p_timer_list[timer_list]->max_time=0;
                            a_p_timer_list[timer_list]->last_time_stamp.sec=0;
                            a_p_timer_list[timer_list]->last_time_stamp.nsec=0;
                            a_p_timer_list[timer_list]->p_first= GL_NULL;
                            a_p_timer_list[timer_list]->p_last=GL_NULL;
                            break;
                            
                         default:
                            break;
                            
                    }/*end of nested switch case*/
               }/*end of case GL_TM_INPUT_EVENT processing*/    
               break;

               case GL_TM_MAIN_EVENT:
               /*
                * If signal from the application main thread
                * call the check timer expiry for all active timer lists.
                */

                    for(list_id = 0; list_id < GL_TM_MAX_TIMER_LIST;
                                list_id++)
                    {
                        if((GL_NULL != a_p_timer_list[list_id]) && (GL_NULL != 
                           a_p_timer_list[list_id]->p_first))         
                        {
                            tm_check_timer_list_expire(list_id);
                        }
                    }
                    break;
                    
                default:
                    break;
                    
        }/*end of switch case*/

   }/*end of while loop*/
   return((gl_thread_return_t)0); 
}

/*****************************************************************************
* Function name    : gl_tm_spawn_timer_server
* Arguments        : gl_thread_info_t   *p_thread_info, 
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Sets the timer server flag in the timer_list_id 
*                    structure and creates a seperate task for the timer server 
*                    before returning the handle value and the task id of the 
*                    timer server.
* Functions called :
*****************************************************************************/

gl_return_t
gl_tm_spawn_timer_server

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info, 
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_ecode )
      gl_thread_info_t          *p_thread_info;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    /*
     * Create event for signal from the main loop of the application.
     */
    if(GL_FAILURE == gl_create_semaphore(&gl_tm_timer_task,0,1,p_ecode))
    {
        GL_INC_TM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    /*
     * Create event for signal from the application when start or stop timer
     * message is sent to the timer server.
     */
    if(GL_FAILURE == gl_create_semaphore(&gl_tm_main_event,0,1,p_ecode))
    {
        GL_INC_TM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_create_semaphore(&gl_tm_input_event,0,1,p_ecode))
    {
        GL_INC_TM_G_MUTX_FAIL()

        return GL_FAILURE;
    }

    if(GL_FAILURE == gl_thm_create((gl_fn_ptr_t)gl_tm_timer_server, 
                p_thread_info, GL_NULL, p_ecode))
    {
        return GL_FAILURE;
    }
    gl_tm_timer_server_flag = GL_TRUE;

    return GL_SUCCESS;
}


/*****************************************************************************
* Function name    : gl_tm_invoke_timer_expiry_handler
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id, 
*                    gw_U8bit           restart_flag
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Checks the Expired timer in the specified message and 
*                    sets the timer free if non-cyclic timer before invoking 
*                    corresponding function pointer with p_timer_buf. This
*                    function is called in case of timer server enabled by 
*                    the application using the timers upon receipt of the 
*                    timer expiry message from the timer server.
* Functions called : set_free_timer
*****************************************************************************/
gl_return_t 
gl_tm_invoke_timer_expiry_handler

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gl_timer_id_t             timer_id,
      gw_U8bit                  restart_flag,
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, timer_id, restart_flag, p_ecode )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gw_U8bit                  restart_flag;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gl_timer_call_back_func_t   func;
    gl_pvoid_t                  p_buf;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == timer_list_id) || (GL_TM_MAX_TIMER_LIST < timer_list_id) ||
       (GL_NULL == a_p_timer_list[timer_list_id - 1]))
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_TIMER_LIST;
        return GL_FAILURE;
    }
    /*
     * Check if timer id is invalid
     * timer id max limit is same as the timer quantity.
     */
    if(timer_id == GL_NULL 
        || timer_id > a_timer_app_list[timer_list_id - 1].max_timer_qty)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_TIMER_ID;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);

    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), p_ecode,
            GL_FAILURE);
    /*
     * Store the function pointer and p_timer_buf locally.
     */
    func = a_timer_app_list[timer_list_id].
           a_timer_index[timer_id-1].func_ptr;

    p_buf = a_timer_app_list[timer_list_id].
            a_timer_index[timer_id-1].p_timer_buf;
    /*
     * If timer stopped in the local list do nothing and return.
     */
    if(func == GL_NULL    &&    p_buf == GL_NULL)
    {
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
    /*
     * If timer alive check the restart flag before freeing the timer id.
     */
    if(restart_flag == GL_FALSE)
    {
        set_free_timer(timer_list_id, timer_id);
    }

    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

    /*
     * Call the function ptr with the stored timer buffer as parameter.
     */
#ifdef GL_SINGLE_TIMEOUT
    gl_single_timeout_handler(a_p_timer_list[timer_list_id]->user_info, p_buf);
#else
    func(p_buf);
#endif
    return GL_SUCCESS;
}


#endif /* end of GL_MT_ENABLED flag */

