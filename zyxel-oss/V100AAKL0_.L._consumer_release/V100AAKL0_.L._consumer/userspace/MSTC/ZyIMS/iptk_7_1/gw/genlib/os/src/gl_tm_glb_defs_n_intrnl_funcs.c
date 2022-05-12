/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Timer Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_tm_glb_defs_n_intrnl_funcs.c
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
* 25/11/2003 B. Venkat           SPR-6477       To modify the time_to_expire 
*                                               value while starting a timer
*                                               to take care of cases where gl_check_timer_expiry
*                                               duration is high
* 
* 25/08/2006 Suraj Gupta         SPR-12567      Resetting the last time stamp to NULL
*                                               if there are no active timers in the list
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_tm.h"


/*
 * Array of timer index maintained by the timer application independent
 * of the timer server.
 */
gl_tm_timer_list_t        *a_p_timer_list[GL_TM_MAX_TIMER_LIST];

gl_tm_timer_app_list_t    a_timer_app_list[GL_TM_MAX_TIMER_LIST];


gw_U32bit  gl_timer_resolution     = GL_TM_TIMER_RESOLUTION;

gl_boolean gl_tm_timer_server_flag = GL_FALSE;

gl_boolean tm_mgr_init_flag        = GL_FALSE;


#ifdef GL_MT_ENABLED

 GL_MUTEX_ID            tm_mgr_sem;

/* 
 * Controls timer task.
 */
GL_SEMAPHORE_ID                 gl_tm_timer_task;     
/* 
 * Event for timer tick.
 */
GL_SEMAPHORE_ID                 gl_tm_main_event;   
/*
 * Event for client starting timer.
 */
GL_SEMAPHORE_ID                 gl_tm_input_event; 

#endif




/*****************************************************************************
* Function name    : tm_check_timer_list_expire
* Arguments        : gl_timer_list_t    timer_list_id
* Return Value     : void
* Functionality    : Checks for the Expired timers in the specified timer 
*                    list.
* Functions called : set_freeTimer, resave_mesg, gl_tm_send_expiry_msg_to_app
*****************************************************************************/
void 
tm_check_timer_list_expire 

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id ) 
#else
    ( timer_list_id )
      gl_timer_list_t           timer_list_id;
#endif
    
/****************************************************************************/
{
    gl_time_stamp_t             current_time_stamp;
    gl_tm_timer_info_t          *p_temp_timer_info = GL_NULL;
    gl_time_stamp_t             diff_in_time_stamps;
    gl_time_stamp_t             incr_time_elapsed;
    gw_S32bit                   diff_in_time_stamps_in_ms;
    gw_S32bit                   incr_time_elapsed_in_ms;
    gl_timer_call_back_func_t   timer_func;
    gl_pvoid_t                  p_timer_buf;
#ifdef GL_TOTAL_MT_SAFE
    gl_error_t                  ecode;
#endif
    /*
     * Get the current time stamp.
     */
    gl_tm_get_timeval(&current_time_stamp);
    
    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), &ecode,
            GL_FAILURE);
    /*
     * Get the differnece in timestamps.
     */
    gl_tm_get_relative_time_diff(&current_time_stamp, &(a_p_timer_list
     [timer_list_id]->last_time_stamp), &diff_in_time_stamps);

    diff_in_time_stamps_in_ms = (diff_in_time_stamps.sec * 1000) +
        (diff_in_time_stamps.nsec/1000000); 

/*    gl_tm_diff_time(&current_time_stamp,&(a_p_timer_list
     [timer_list_id]->last_time_stamp),&diff_in_time_stamps);

    diff_in_time_stamps_in_ms = ((diff_in_time_stamps.sec * 1000) 
     + (diff_in_time_stamps.nsec/1000000));     */

    diff_in_time_stamps_in_ms = diff_in_time_stamps_in_ms/gl_timer_resolution;

	if(10 <= diff_in_time_stamps.sec)
    {
        /* Time stamp has changed drastically from the last call */
        /* Could happen in case of update from NTP server */
        /* Just ignore this timer expiry check and update the new
           timestamp in timer list, so next check for timer expiry
           with use the new timestamp */
		
		gl_tm_get_timeval(&(a_p_timer_list[timer_list_id]->last_time_stamp));
        return;
    }

    p_temp_timer_info= a_p_timer_list[timer_list_id]->p_first;
    /*
     * Iterate through the timer list till the timer info is not null
     * and last time stamp is not equal to the current time stamp.
     */
    while((p_temp_timer_info != GL_NULL) && (0 < diff_in_time_stamps_in_ms ))
    {
        if(((gw_U32bit)diff_in_time_stamps_in_ms) < p_temp_timer_info->
                time_to_expire)
        {
            incr_time_elapsed_in_ms = diff_in_time_stamps_in_ms;
        }
        else 
        {
            /*
             * if(((gw_U32bit)diff_in_time_stamps_in_ms) 
             *  >= p_temp_timer_info->time_to_expire)
             */
            incr_time_elapsed_in_ms = p_temp_timer_info->time_to_expire;
        }
        /*
         * Decrement the following.
         * Maximum time of the timer list.
         * Timer To Expire.
         * The vale of the Last Time Stamp.
         */
        diff_in_time_stamps_in_ms -= incr_time_elapsed_in_ms;
        p_temp_timer_info->time_to_expire -= incr_time_elapsed_in_ms;
        (a_p_timer_list[timer_list_id]->max_time) -= incr_time_elapsed_in_ms;

        if(1000 <= incr_time_elapsed_in_ms)
        {
            incr_time_elapsed.sec = incr_time_elapsed_in_ms/1000;
            incr_time_elapsed.nsec = (incr_time_elapsed_in_ms % 1000)*1000000;
        }
        else
        {
            incr_time_elapsed.sec = 0;
            incr_time_elapsed.nsec = (incr_time_elapsed_in_ms * 1000000);
        }             
        gl_tm_add_time(&(a_p_timer_list[timer_list_id]->last_time_stamp),
        &incr_time_elapsed,&(a_p_timer_list[timer_list_id]->last_time_stamp)); 
             
        if(p_temp_timer_info->time_to_expire == GL_NULL)
        {
            /*
             * If time to expire of the timer is 0 iterate through
             * the timer list and expire all timers due for
             * expiry at this instant, i.e., all timers following this
             * timer and having time to expire value as 0.
             */
            while(p_temp_timer_info != (gl_pvoid_t)GL_NULL
                && p_temp_timer_info->time_to_expire == GL_NULL)
            {
                if(p_temp_timer_info->restart_flag != GL_TRUE)
                {
                    /*
                     * If restart flag not set , i.e., timer is non-cyclic.
                     * Remove the timer from the list before expiring it
                     * because the application implemented timer handler  
                     * function func_ptr may start another timer.
                     */
                    a_p_timer_list[timer_list_id]->p_first
                        = a_p_timer_list[timer_list_id]->p_first->fptr;
                    if(a_p_timer_list[timer_list_id]->p_first==
                                                    (gl_pvoid_t)GL_NULL)
                    {
                        a_p_timer_list[timer_list_id]->p_first
                            =a_p_timer_list[timer_list_id]->p_last=GL_NULL;
                    }
                    else
                    {
                        a_p_timer_list[timer_list_id]->p_first->bptr=GL_NULL;
                    }
                    /*
                     * Decrement the count of the active timers.
                     */
                    --(a_p_timer_list[timer_list_id]->active_timer_qty);

					
                    /* If all the timers have expired, set the last_time_stamp
                     * to NULL. As there is no timer left to expire, next timer
                     * which will be started will not have any other timer
                     *  relative to it */ 
                    if(a_p_timer_list[timer_list_id]->active_timer_qty == GL_NULL)
                    {
                       a_p_timer_list[timer_list_id]->last_time_stamp.sec = GL_NULL;
                       a_p_timer_list[timer_list_id]->last_time_stamp.nsec = GL_NULL;
                    }

                    GL_TM_C_TIMER_RUNING(timer_list_id,
                            a_p_timer_list[timer_list_id]->active_timer_qty)
                    
                    /*
                     * Set the Status of timer as free.
                     */
                    p_temp_timer_info->is_timer_active = GL_FALSE;
                    p_temp_timer_info->fptr = p_temp_timer_info->bptr=GL_NULL;
                    
                    if(gl_tm_timer_server_flag == GL_FALSE)
                    {
                        /*
                         * Store the function pointer and p_timer_buf locally.
                         * As in set_free_timer these feilds are set to NULL
                         * for given timer_id.
                         */
                        timer_func = a_timer_app_list[timer_list_id].
                            a_timer_index[p_temp_timer_info->timer_id-1].
                            func_ptr;

                        p_timer_buf = a_timer_app_list[timer_list_id].
                            a_timer_index[p_temp_timer_info->timer_id-1].
                            p_timer_buf;
                        /*
                         * Set the timer free.
                         */
                        set_free_timer(timer_list_id,
                                       p_temp_timer_info->timer_id);
                        
                        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);

                        /*
                         * If timer server not running call the timerid 
                         * function pointer with pointer to 
                         * timer buf as argument.
                         */
#ifdef GL_SINGLE_TIMEOUT
                        gl_single_timeout_handler
                            (a_p_timer_list[timer_list_id]->user_info,
                             p_timer_buf);
#else
                        timer_func(p_timer_buf);
#endif
                        
                        GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                    }
#ifdef GL_MT_ENABLED
                    else
                    {
                        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                        /*
                         * If timer server running as a seperate thread 
                         * call porting function to send timer expiry message.
                         */
                        gl_tm_send_expiry_msg_to_app(
                            a_p_timer_list[timer_list_id]->user_info,
                            (gl_timer_list_t)(timer_list_id + 1), 
                            p_temp_timer_info->timer_id,
                            p_temp_timer_info->restart_flag,
                            p_temp_timer_info->timer_duration 
                                        *gl_timer_resolution);
                        
                        GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                    }
#endif
                }
                else
                {
                    /*
                     * If restart flag set , i.e., timer is cyclic.
                     * Remove the timer from the list and reset 
                     * timer duration and timer postion in list.
                     */
                    a_p_timer_list[timer_list_id]->p_first
                        = a_p_timer_list[timer_list_id]->p_first->fptr;
                    if(a_p_timer_list[timer_list_id]->p_first==
                                                (gl_pvoid_t)GL_NULL)
                    {
                        a_p_timer_list[timer_list_id]->p_first
                            =a_p_timer_list[timer_list_id]->p_last=GL_NULL;
                    }
                    else
                    {
                        a_p_timer_list[timer_list_id]->p_first->bptr
                                                    =GL_NULL;
                    }
                    /*
                     * Restart the timer by resaving the timer info 
                     * in the list.
                     */
                    resave_mesg(timer_list_id, p_temp_timer_info);
                    
                    if(gl_tm_timer_server_flag == GL_FALSE)
                    {
                        timer_func = a_timer_app_list[timer_list_id].
                            a_timer_index[p_temp_timer_info->timer_id-1].
                            func_ptr;

                        p_timer_buf = a_timer_app_list[timer_list_id].
                            a_timer_index[p_temp_timer_info->timer_id-1].
                            p_timer_buf;
                        
                        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                        /*
                         * If timer server not running call the timerid 
                         * function pointer with pointer to 
                         * timer buf as argument.
                         */
#ifdef GL_SINGLE_TIMEOUT
                        gl_single_timeout_handler
                            (a_p_timer_list[timer_list_id]->user_info,
                             p_timer_buf);
#else
                        timer_func(p_timer_buf);
#endif
                        
                        GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                    }
#ifdef GL_MT_ENABLED
                    else
                    {
                        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                        /*
                         * If timer server running as a seperate thread 
                         * call porting function to send timer expiry message.
                         */
                        gl_tm_send_expiry_msg_to_app(
                                a_p_timer_list[timer_list_id]->user_info,
                                (gl_timer_list_t)(timer_list_id + 1),
                                p_temp_timer_info->timer_id,
                                p_temp_timer_info->restart_flag,
                                p_temp_timer_info->timer_duration
                                        *gl_timer_resolution);
                        
                        GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->
                                    timer_list_sem), &ecode, GL_FAILURE);
                    }
#endif
                }
                p_temp_timer_info = a_p_timer_list[timer_list_id]->p_first;
            }/*end of while*/
        }/*end of if*/
        else
        {
            break;
        }
    }/*end of while*/
    
    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
            &ecode, GL_FAILURE);
}



/*****************************************************************************
* Function name    : save_mesg
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id,
*                    gw_U32bit          timer_duration, 
*                    gw_U8bit           restart_flag
* Return Value     : gl_return_t
* Functionality    : Stores the contents of the start timer in the timer_list.
* Functions called :
*****************************************************************************/
gl_return_t
save_mesg

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_timer_id_t             timer_id,
      gw_U32bit                 timer_duration, 
      gw_U8bit                  restart_flag
    )
#else
    ( timer_list_id, timer_id, timer_duration, restart_flag )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gw_U32bit                 timer_duration;
      gw_U8bit                  restart_flag;
#endif
    
/****************************************************************************/
{
    gl_tm_timer_info_t          *p_timer_info;
    gl_tm_timer_info_t          *p_temp_timer_info;
    gw_U32bit                   temp_time=0;
    gl_time_stamp_t             current_time_stamp;
    gw_S32bit                   diff_in_time_stamps_in_ms;
    gl_time_stamp_t             diff_in_time_stamps;

    /*if(GL_NULL==(p_timer_info=(gl_tm_timer_info_t*)
                 SYS_MALLOC(sizeof(gl_tm_timer_info_t))))
    {
        *
         * TimerId available but malloc fails - may not occur
         * if initializing parameters are correct.
         *
        return GL_FAILURE;
    }*/
    p_timer_info = &(a_p_timer_list[timer_list_id]->
          a_index_info[timer_id -1].a_timer_info);
    /*
     * Set the timer id, timer duration and 
     * restrat flag in the timer info structure.
     */
    p_timer_info->is_timer_active = GL_TRUE;
    p_timer_info->timer_id = timer_id;
    p_timer_info->restart_flag = restart_flag;
    p_timer_info->fptr=p_timer_info->bptr=GL_NULL;
    if(timer_duration/gl_timer_resolution == 0)
    {
        p_timer_info->timer_duration = p_timer_info->time_to_expire = 1;
    }
    else
    {
        p_timer_info->timer_duration = p_timer_info->time_to_expire
            =timer_duration/gl_timer_resolution;
    }
  

    /* Resetting the last time stamp to NULL if there are no active timers
     * in the list
     * */
    
    if(GL_NULL == a_p_timer_list[timer_list_id]->active_timer_qty)
    {
        a_p_timer_list[timer_list_id]->last_time_stamp.sec = GL_NULL;
        a_p_timer_list[timer_list_id]->last_time_stamp.nsec = GL_NULL;
    }

    
    /** Modify the time_to_expire to take of following scenario
     *  say at t0 a timer of 3 secs is started and after (t0+1) a timer of 4 secs
     *  is started. Now if the gl_check_timer_expiry periodicity is say 2 secs then
     *  4secs timer should get expire 2secs after the 3sec timer expiry. For this 1sec
     *  duration needs to be added to the time_to_expire field. Otherwise in normal cases where
     *  gl_check_timer_expiry is called at proper intervals then the last_time_stamp will get properly
     *  updated and hence the addtion to time_to_expire field will be less than or equal to
     *  gl_check_timer_expiry duration.
     *  */

    if((a_p_timer_list[timer_list_id]->last_time_stamp.sec != GL_NULL) || 
                        (a_p_timer_list[timer_list_id]->last_time_stamp.nsec != GL_NULL))
    {
        gl_tm_get_timeval(&current_time_stamp);
              
        /** ** Get the differnece in timestamps. **/

        gl_tm_get_relative_time_diff(&current_time_stamp, &(a_p_timer_list
                 [timer_list_id]->last_time_stamp), &diff_in_time_stamps);

        diff_in_time_stamps_in_ms = (diff_in_time_stamps.sec * 1000) +
                                     (diff_in_time_stamps.nsec/1000000); 

        diff_in_time_stamps_in_ms = diff_in_time_stamps_in_ms/gl_timer_resolution;
        p_timer_info->time_to_expire += diff_in_time_stamps_in_ms;

    }


    if(a_p_timer_list[timer_list_id]->p_first == GL_NULL)
    {
        /*
         * If only one timer set the first an dlst timer in the list.
         */
        a_p_timer_list[timer_list_id]->p_first
            =a_p_timer_list[timer_list_id]->p_last=p_timer_info;
        /*
         * Set the maximum time equal to time to expire for this timer id.
         */
        a_p_timer_list[timer_list_id]->max_time 
            = p_timer_info->time_to_expire;
    }
    else if( p_timer_info->time_to_expire >= 
        a_p_timer_list[timer_list_id]->max_time)
    {
        /*
         * If the timer needs to be added after the last timer in the list.
         */
        temp_time = p_timer_info->time_to_expire;
        p_timer_info->time_to_expire 
            = p_timer_info->time_to_expire 
                - a_p_timer_list[timer_list_id]->max_time;
        a_p_timer_list[timer_list_id]->max_time 
            = temp_time;
        p_timer_info->bptr=a_p_timer_list[timer_list_id]->p_last;
        p_timer_info->fptr=GL_NULL;
        a_p_timer_list[timer_list_id]->p_last->fptr=p_timer_info;
        a_p_timer_list[timer_list_id]->p_last=p_timer_info;
    }
    else if(p_timer_info->time_to_expire <= 
        a_p_timer_list[timer_list_id]->p_first->time_to_expire)
    {
        /*
         * If the timer needs to be added before the first timer in the list.
         */
        a_p_timer_list[timer_list_id]->p_first->time_to_expire
            =a_p_timer_list[timer_list_id]->p_first->time_to_expire
                - p_timer_info->time_to_expire;
        p_timer_info->fptr = a_p_timer_list[timer_list_id]->p_first;
        p_timer_info->bptr = GL_NULL;
        a_p_timer_list[timer_list_id]->p_first->bptr=p_timer_info;
        a_p_timer_list[timer_list_id]->p_first=p_timer_info;
    }
    else
    {
        /*
         * Search for the correct position for the timer depending upon 
         * the time to expire of the timer.
         */
        for(p_temp_timer_info = a_p_timer_list[timer_list_id]->p_first;
           (p_timer_info->time_to_expire > p_temp_timer_info->time_to_expire);
            p_temp_timer_info=p_temp_timer_info->fptr)
        {    
            p_timer_info->time_to_expire -= p_temp_timer_info->time_to_expire;
        }

        p_temp_timer_info->time_to_expire -= p_timer_info->time_to_expire;
        p_temp_timer_info->bptr->fptr=p_timer_info;
        p_timer_info->bptr=p_temp_timer_info->bptr;
        p_timer_info->fptr=p_temp_timer_info;
        p_temp_timer_info->bptr=p_timer_info;
    }

    /*
     * Set the last time stamp if timer added to the empty list.
     */
    if(a_p_timer_list[timer_list_id]->active_timer_qty == GL_NULL)
    {
        gl_tm_get_timeval(&(a_p_timer_list[timer_list_id]->last_time_stamp));
    }
    /*
     * Increment the timer count.
     */
    (a_p_timer_list[timer_list_id]->active_timer_qty)++;
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : resave_mesg
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_tm_timer_info   *p_timer_info
* Return Value     : gl_return_t
* Functionality    : Re-stores the contents of the expired timer in the 
*                    timer_list according to the timer_duration to 
*                    restart the timer.
* Functions called :
*****************************************************************************/
gl_return_t
resave_mesg

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_tm_timer_info_t        *p_timer_info
    )
#else
    ( timer_list_id, p_timer_info )
      gl_timer_list_t           timer_list_id;
      gl_tm_timer_info_t        *p_timer_info;
#endif
    
/****************************************************************************/
{
    gl_tm_timer_info_t          *p_temp_timer_info=GL_NULL;
    gw_U32bit                   temp_time = GL_NULL;
    gl_time_stamp_t             current_time_stamp;
    gw_S32bit                   diff_in_time_stamps_in_ms;
    gl_time_stamp_t             diff_in_time_stamps;

    /*
     * Set the time to expire to timer duration to restart the timer.
     */
    p_timer_info->time_to_expire = p_timer_info->timer_duration;
    p_timer_info->fptr=p_timer_info->bptr=GL_NULL;

    
    /** Modify the time_to_expire to take of following scenario
     *  say at t0 a timer of 3 secs is started and after (t0+1) a timer of 4 secs
     *  is started. Now if the gl_check_timer_expiry periodicity is say 2 secs then
     *  4secs timer should get expire 2secs after the 3sec timer expiry. For this 1sec
     *  duration needs to be added to the time_to_expire field. Otherwise in normal cases where
     *  gl_check_timer_expiry is called at proper intervals then the last_time_stamp will get properly
     *  updated and hence the addtion to time_to_expire field will be less than or equal to
     *  gl_check_timer_expiry duration.
     *  */

    if((a_p_timer_list[timer_list_id]->last_time_stamp.sec != GL_NULL) || 
                        (a_p_timer_list[timer_list_id]->last_time_stamp.nsec != GL_NULL))
    {
        gl_tm_get_timeval(&current_time_stamp);
              
        /** ** Get the differnece in timestamps. **/

        gl_tm_get_relative_time_diff(&current_time_stamp, &(a_p_timer_list
                 [timer_list_id]->last_time_stamp), &diff_in_time_stamps);

        diff_in_time_stamps_in_ms = (diff_in_time_stamps.sec * 1000) +
                                     (diff_in_time_stamps.nsec/1000000); 

        diff_in_time_stamps_in_ms = diff_in_time_stamps_in_ms/gl_timer_resolution;
        p_timer_info->time_to_expire += diff_in_time_stamps_in_ms;

    }


    if(a_p_timer_list[timer_list_id]->p_first == GL_NULL)
    {
        /*
         * If timer list does not contain any other timer at present.
         */
        a_p_timer_list[timer_list_id]->p_first
            = a_p_timer_list[timer_list_id]->p_last=p_timer_info;
        a_p_timer_list[timer_list_id]->max_time 
            = p_timer_info->time_to_expire;
        return GL_SUCCESS;
    }
    if(p_timer_info->time_to_expire <= 
        a_p_timer_list[timer_list_id]->p_first->time_to_expire)
    {
        /*
         * If the timer needs to be added before the first timer in the list.
         */
        a_p_timer_list[timer_list_id]->p_first->time_to_expire
            =a_p_timer_list[timer_list_id]->p_first->time_to_expire
                - p_timer_info->time_to_expire;
        p_timer_info->fptr = a_p_timer_list[timer_list_id]->p_first;
        a_p_timer_list[timer_list_id]->p_first->bptr=p_timer_info;
        a_p_timer_list[timer_list_id]->p_first=p_timer_info;
        return GL_SUCCESS;
    }
    if(p_timer_info->time_to_expire >= a_p_timer_list[timer_list_id]->max_time)
    {
        /*
         * If the timer needs to be added after the last timer in the list.
         */
        temp_time = p_timer_info->time_to_expire;
        p_timer_info->time_to_expire 
            = p_timer_info->time_to_expire 
              - a_p_timer_list[timer_list_id]->max_time;
        a_p_timer_list[timer_list_id]->max_time = temp_time;
        p_timer_info->bptr=a_p_timer_list[timer_list_id]->p_last;
        a_p_timer_list[timer_list_id]->p_last->fptr=p_timer_info;
        a_p_timer_list[timer_list_id]->p_last=p_timer_info;
        return GL_SUCCESS;
    }
    /*
     * Search for the correct position for the timer
     * depending upon the time to expire of the timer.
     */
    for(p_temp_timer_info = a_p_timer_list[timer_list_id]->p_first;
        p_timer_info->time_to_expire > p_temp_timer_info->time_to_expire;
        p_temp_timer_info = p_temp_timer_info->fptr)
    {    
         p_timer_info->time_to_expire -= p_temp_timer_info->time_to_expire;
    }

    p_temp_timer_info->time_to_expire -= p_timer_info->time_to_expire;
    p_temp_timer_info->bptr->fptr=p_timer_info;
    p_timer_info->bptr=p_temp_timer_info->bptr;
    p_timer_info->fptr=p_temp_timer_info;
    p_temp_timer_info->bptr=p_timer_info;

    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : extract_mesg
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id
* Return Value     : gl_return_t
* Functionality    : Extracts the contents of the stop timer from the 
*                    timer_list.
* Functions called :
*****************************************************************************/
gl_return_t
extract_mesg

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_timer_id_t             timer_id
    )
#else
    ( timer_list_id, timer_id )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
#endif
    
/****************************************************************************/
{

    gl_tm_timer_info_t          *p_timer_info;

    /* 
     * Allocate the timer_info_t from the timerId-timerIndex.
     */
    p_timer_info = &(a_p_timer_list[timer_list_id]->a_index_info[timer_id-1].
            a_timer_info);

     /*
      * Check if the timer doesnot exist as an active timer.
      */
    if(GL_FALSE == p_timer_info->is_timer_active)
    {
        return GL_FAILURE;
    }

    if(p_timer_info == a_p_timer_list[timer_list_id]->p_first)
    {
        /*
         * If timer to be removed from the list is the first timer.
         */
        a_p_timer_list[timer_list_id]->p_first = p_timer_info->fptr;
        if (a_p_timer_list[timer_list_id]->p_first != GL_NULL)
        {
            /*
             * If new first timer exists increment the time to expire value
             * by corresponding value of the removed timer.
             */
            a_p_timer_list[timer_list_id]->p_first->time_to_expire 
                = a_p_timer_list[timer_list_id]->p_first->time_to_expire + 
                  p_timer_info->time_to_expire;
            p_timer_info->fptr->bptr = GL_NULL;
        }
        else  /*  first == last */
        {
           a_p_timer_list[timer_list_id]->p_last =(gl_tm_timer_info_t*)GL_NULL;
           a_p_timer_list[timer_list_id]->max_time = GL_NULL;
        }
    }
    else if (p_timer_info == a_p_timer_list[timer_list_id]->p_last)
    {
        /*
         * If timer to be removed from the list is the last timer
         * decrement the time to expire value by corresponding value of the 
         * removed timer.
         */
        a_p_timer_list[timer_list_id]->p_last = p_timer_info->bptr;
        a_p_timer_list[timer_list_id]->p_last->fptr = GL_NULL;
        a_p_timer_list[timer_list_id]->max_time 
            -= p_timer_info->time_to_expire;
    }
    else
    {
        /*
         * Remove the timer and increment the time to expire value
         * of the next timer in the list by corresponding value of 
         * the removed timer.
         */
        p_timer_info->fptr->time_to_expire += p_timer_info->time_to_expire;
        p_timer_info->bptr->fptr = p_timer_info->fptr;
        p_timer_info->fptr->bptr = p_timer_info->bptr;
    }

    /*
     * Set the timer stataus as free.
     */
    p_timer_info->is_timer_active = GL_FALSE;
    p_timer_info->fptr = p_timer_info->bptr = GL_NULL;

    /*
     * Decrement the timer count.
     */
    (a_p_timer_list[timer_list_id]->active_timer_qty)--;
    /*
     * Reset the last time stamp if all timers removed from the list.
     */
    if(a_p_timer_list[timer_list_id]->active_timer_qty == GL_NULL)
    {
        a_p_timer_list[timer_list_id]->last_time_stamp.sec = GL_NULL;
        a_p_timer_list[timer_list_id]->last_time_stamp.nsec = GL_NULL;
    }
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : get_free_timer
* Arguments        : gl_timer_list_t    timer_list_id
* Return Value     : gl_timer_id_t
* Functionality    : Get a free timer_id from the free_timer_list associated 
*                    with the timer_list.
* Functions called :
*****************************************************************************/
gl_timer_id_t
get_free_timer

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id )
#else
    ( timer_list_id )
      gl_timer_list_t           timer_list_id;
#endif
    
/****************************************************************************/
{
    gl_timer_id_t timer_id;

    if(a_timer_app_list[timer_list_id].first_free_timer_index == 
        a_timer_app_list[timer_list_id].max_timer_qty)
    {
        /*
         * No more timers can be allocated.
         * Returning GL_NULL as it is conisdered as invalid timer id.
         */
        return GL_NULL;
    }

    timer_id = a_timer_app_list[timer_list_id].first_free_timer_index +1;
    
    /*
     * Change next free timer index in the free list.
     */
    a_timer_app_list[timer_list_id].first_free_timer_index= 
        a_timer_app_list[timer_list_id].
        a_timer_index[a_timer_app_list[timer_list_id].first_free_timer_index].
        next_index;    

    /*
     * If all timer ids allocated set the first_free_timer_index
     * and last_free_timer_index to invalid value.
     */
    if(a_timer_app_list[timer_list_id].first_free_timer_index == 
       a_timer_app_list[timer_list_id].max_timer_qty)
    {
            a_timer_app_list[timer_list_id].last_free_timer_index
                =a_timer_app_list[timer_list_id].max_timer_qty;
    }

    return timer_id;
}



/*****************************************************************************
* Function name    : set_free_timer
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id
* Return Value     : void
* Functionality    : Set free a timer_id and add it to the 
*                    free_timer_list associated with timer_list_id.
* Functions called :
*****************************************************************************/
void
set_free_timer

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id,
      gl_timer_id_t             timer_id
    )
#else
    ( timer_list_id, timer_id )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
#endif
    
/****************************************************************************/
{
    /*
     * If timer_id entry in the free list already free do nothing and return.
     */
    if(a_timer_app_list[timer_list_id].
       a_timer_index[timer_id-1].func_ptr == GL_NULL)
    {
        return;
    }
    /*
     * Reset the func_ptr and p_timer_buf parameters.
     */
    a_timer_app_list[timer_list_id].
        a_timer_index[timer_id-1].func_ptr = GL_NULL;
    a_timer_app_list[timer_list_id].
        a_timer_index[timer_id-1].p_timer_buf = GL_NULL;

    /*
     * If all the timers are running and free list is empty
     * set the first and last free timer index.
     */
    if((a_timer_app_list[timer_list_id].first_free_timer_index == 
        a_timer_app_list[timer_list_id].max_timer_qty) &&
        (a_timer_app_list[timer_list_id].last_free_timer_index ==
            a_timer_app_list[timer_list_id].max_timer_qty))
    {
        a_timer_app_list[timer_list_id].first_free_timer_index
            =a_timer_app_list[timer_list_id].last_free_timer_index
            =timer_id -1;
    }
    else
    {
        /*
         * Add the timerId to the free list at the tail.
         */
        a_timer_app_list[timer_list_id].
            a_timer_index[a_timer_app_list[timer_list_id].
            last_free_timer_index].next_index = timer_id-1;
        a_timer_app_list[timer_list_id].last_free_timer_index = timer_id-1;
    }

    /*
     * Set the next timer id of the last timer index to 
     * invalid value max_timer_qty.
     */
    a_timer_app_list[timer_list_id].
        a_timer_index[a_timer_app_list[timer_list_id].
        last_free_timer_index].next_index = 
        a_timer_app_list[timer_list_id].max_timer_qty;

}



/*****************************************************************************
* Function name    : timer_id_list_init
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gw_U32bit          timer_qty
* Return Value     : gl_return_t
* Functionality    : Initialize the timer index list for the specified 
*                    timer list.
* Functions called :
*****************************************************************************/
gl_return_t
timer_id_list_init

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gw_U32bit                 timer_qty
    )
#else
    ( timer_list_id, timer_qty )
      gl_timer_list_t           timer_list_id;
      gw_U32bit                 timer_qty;
#endif
    
/****************************************************************************/
{

    gw_U16bit                   index1;

    /*
     * Prepare the free list of the timer ids to be allocated.
     */
    a_p_timer_list[timer_list_id]->a_index_info =
        (gl_tm_timer_index_info_t*) 
        SYS_MALLOC(timer_qty * sizeof(gl_tm_timer_index_info_t));
    /*
     * Set the free list buffer in the timer list structure.
     */
    if(a_p_timer_list[timer_list_id]->a_index_info == (gl_pvoid_t)GL_NULL)
    {
        return GL_FAILURE;
    }

    a_timer_app_list[timer_list_id].a_timer_index =
        (gl_tm_timer_index_t*)SYS_MALLOC(timer_qty * 
        sizeof(gl_tm_timer_index_t));
    
    if(a_timer_app_list[timer_list_id].a_timer_index == (gl_pvoid_t)GL_NULL)
    {
        SYS_FREE(a_p_timer_list[timer_list_id]->a_index_info);
        a_p_timer_list[timer_list_id]->a_index_info = GL_NULL;
        return GL_FAILURE;
    }

    /*
     * Set the first_free_timer_index to zero.
     */
    a_timer_app_list[timer_list_id].first_free_timer_index=0;
    a_timer_app_list[timer_list_id].last_free_timer_index=timer_qty -1;
    /*
     * Last free index in the free list of timer ids is timer_qty -1.
     */

    /*
     * Set the timer ids in the increasing order in the free list.
     */
    for(index1=0;index1 < timer_qty; index1++)
    {
        a_p_timer_list[timer_list_id]->a_index_info[index1].a_timer_info.
            is_timer_active = GL_FALSE; 
        a_p_timer_list[timer_list_id]->a_index_info[index1].a_timer_info.
            fptr = GL_NULL; 
        a_p_timer_list[timer_list_id]->a_index_info[index1].a_timer_info.
            bptr = GL_NULL;
        
        a_timer_app_list[timer_list_id].a_timer_index[index1].next_index
                                                            = index1+1;
        a_timer_app_list[timer_list_id].a_timer_index[index1].func_ptr
                                                            = GL_NULL;
        a_timer_app_list[timer_list_id].a_timer_index[index1].p_timer_buf
                                                        = (gl_pvoid_t)GL_NULL;
    }
    return GL_SUCCESS;
}


