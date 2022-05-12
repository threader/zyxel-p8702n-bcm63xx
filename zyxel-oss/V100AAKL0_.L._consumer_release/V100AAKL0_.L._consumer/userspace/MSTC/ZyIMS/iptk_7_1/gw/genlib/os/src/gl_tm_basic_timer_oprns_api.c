/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Timer Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_tm_basic_timer_oprns_api.c
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
* 19/03/2002 Ravi Jindal         SPR-3954       Enhancements for Rel-1.0.
* 
* 21/10/2002 Ravi Jindal         SPR-4965       To change the check of return
*                                               value of release-semaphore.
* 13/11/2002 Prashant Jain       SPR-5062       To return correct Timer List
*                                               IDs as the return parameter of 
*                                               the API gl_tm_get_timer_list_ids. 
* 25/11/2003 B. Venkat           SPR-6477       to modify the last time stamp even if
*                                               the timer_list is empty
*                                               
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_tm.h"



/*****************************************************************************
* Function name    : gl_tm_start_timer
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gw_U32bit          timer_duration, 
*                    gl_timer_call_back_func_t func_ptr,
*                    gl_pvoid_t         p_timer_buf, 
*                    gw_U8bit           restart_flag,
*                    gl_timer_id_t      *p_timer_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : This is the function to start the timer.  This function
*                       - assigns a timerId to the request 
*                       - stores the timer buffer in timer_list
*                       - if first active timer, sets the last_time_stamp
*                       - returns Success.
* Functions called : save_mesg  
*****************************************************************************/
gl_return_t gl_tm_start_timer

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gw_U32bit                 timer_duration,
      gl_timer_call_back_func_t func_ptr,
      gl_pvoid_t                p_timer_buf,
      gw_U8bit                  restart_flag,
      gl_timer_id_t             *p_timer_id,
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, timer_duration, func_ptr, p_timer_buf, restart_flag, 
      p_timer_id, p_ecode )
      gl_timer_list_t           timer_list_id;
      gw_U32bit                 timer_duration;
      gl_timer_call_back_func_t func_ptr;
      gl_pvoid_t                p_timer_buf;
      gw_U8bit                  restart_flag;
      gl_timer_id_t             *p_timer_id;
      gl_error_t                *p_ecode;
#endif

/****************************************************************************/
{
    GL_INC_TM_G_START_TIMER_RQ()
        
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(tm_mgr_init_flag == GL_FALSE)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

         *p_ecode = GL_TM_ERR_INIT_REQD;
         return GL_FAILURE;
    } 
    if((GL_NULL == timer_list_id) || (GL_TM_MAX_TIMER_LIST < timer_list_id) ||
       (GL_NULL == a_p_timer_list[timer_list_id - 1]))
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_TIMER_LIST;
        return GL_FAILURE;
    }
    /*
     * Validate function pointer.
     */
    if( func_ptr == (gl_timer_call_back_func_t)GL_NULL)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_FUNCPTR;
        return GL_FAILURE;
    }
    /*
     * Validate timer duration.
     */
    if (timer_duration == GL_NULL)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_DURATION;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);
     
    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), p_ecode,
            GL_FAILURE);

    /*
     * Get free timer id.
     */
    *p_timer_id= get_free_timer(timer_list_id);

    /* 
     * Timer ID ZERO is not a valid Value. ID assignment starts from 1.
     */
    if((*p_timer_id) == (gl_timer_id_t)0)
    {
        /*
         * Return error if all timers active.
         */
        GL_INC_TM_G_START_TIMER_FAIL()
        GL_INC_TM_C_START_TIMER_FAIL(timer_list_id)

        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }

    /*
     * Set the function pointer and the timer buffer pointer 
     * in timer app list.
     */
    a_timer_app_list[timer_list_id].a_timer_index[*p_timer_id -1].func_ptr
                                                    = func_ptr;
    a_timer_app_list[timer_list_id].a_timer_index[*p_timer_id -1].p_timer_buf
                                                    = p_timer_buf;

    if(gl_tm_timer_server_flag == GL_FALSE)
    {
        if(GL_FAILURE == save_mesg(timer_list_id, *p_timer_id, timer_duration,
                    restart_flag))
        {
            GL_INC_TM_G_START_TIMER_FAIL()
            GL_INC_TM_C_START_TIMER_FAIL(timer_list_id)

            /*
             * If no timer server save the started timer in the timer list 
             * maintained in the context of the application.
             */
            /*
             * Clean up resources.
             */
            set_free_timer(timer_list_id, *p_timer_id);
            
            GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

            *p_ecode = GL_TM_ERR_NO_RESOURCE;
            return GL_FAILURE;
        }
    }
#ifdef GL_MT_ENABLED
    else
    {
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        /*
         * If timer server thread seperately active send start timer message
         * to server.
         */
        gl_tm_send_msg_to_server(GL_TM_START_TIMER, (gl_timer_list_t)
                (timer_list_id + 1), *p_timer_id, restart_flag,timer_duration);
        /*
         * Set the event to signal to timer server.
         */
        gl_release_semaphore(&gl_tm_input_event, p_ecode);

        gl_release_semaphore(&gl_tm_timer_task, p_ecode);

        return GL_SUCCESS;
    }
#endif
    
    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
        p_ecode, GL_FAILURE);
    
    GL_INC_TM_C_START_TIMER_RQ(timer_list_id)
    GL_TM_C_PEAK_TIMER_RUN(timer_list_id, a_p_timer_list[timer_list_id]->\
            active_timer_qty )
    GL_TM_C_TIMER_RUNING(timer_list_id, a_p_timer_list[timer_list_id]->\
            active_timer_qty)
    
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_tm_stop_timer
* Arguments        : gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id, 
*                    gl_pvoid_t          *p_p_timer_buf,
*                    gl_error_t          *p_ecode
* Return Value     : gl_return_t 
* Functionality    : This is the function to stop the timer. The function
*                       - removes the timerId requested from the timer list
*                       - releases the timer info buffer in timer_list
*                       - if no active timers remaining initializes the 
*                         last_time_stamp to 0.
* Functions called : extract_mesg, set_free_timer 
*****************************************************************************/
gl_return_t
gl_tm_stop_timer

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gl_timer_id_t             timer_id, 
      gl_pvoid_t                *p_p_timer_buf, 
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, timer_id, p_p_timer_buf, p_ecode )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gl_pvoid_t                *p_p_timer_buf;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(tm_mgr_init_flag == GL_FALSE)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

         *p_ecode = GL_TM_ERR_INIT_REQD;
         return GL_FAILURE;
    } 
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
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);
    
    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), p_ecode,
            GL_FAILURE);

    /*
     * Check if timer id is not currently active in the timer list
     * func_ptr for the timer id would be NULL if the timer is not 
     * currently active.
     */
    if(a_timer_app_list[timer_list_id].
        a_timer_index[timer_id-1].func_ptr == GL_NULL)
    {
        GL_INC_TM_G_STOP_RQ_TIMER_ALDY_EXP()
            
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_EXPIRED;
        return GL_FAILURE;
    }

    /*
     * Get the pointer to the timer buffer.
     */
    *p_p_timer_buf = a_timer_app_list[timer_list_id].
        a_timer_index[timer_id -1].p_timer_buf;

    set_free_timer(timer_list_id, timer_id);

    if(gl_tm_timer_server_flag == GL_FALSE)
    {
        if( GL_SUCCESS != extract_mesg(timer_list_id, timer_id))
        {
            /*
             * If no timer server extract the active timer in the timer list 
             * maintained in the context of the application.
             */
            GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

            *p_ecode = GL_TM_ERR_INV_TIMER_ID;
            return GL_FAILURE;
        }
    }
#ifdef GL_MT_ENABLED
    else
    {
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        /*
         * If timer server thread seperately active send stop timer message
         * to server.
         */
        gl_tm_send_msg_to_server(GL_TM_STOP_TIMER, (gl_timer_list_t)
                (timer_list_id + 1), timer_id, GL_NULL, GL_NULL);

        gl_release_semaphore(&gl_tm_input_event, p_ecode);

        gl_release_semaphore(&gl_tm_timer_task, p_ecode);

        return GL_SUCCESS;
    }
#endif
    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);
    
    GL_TM_C_TIMER_RUNING(timer_list_id, a_p_timer_list[timer_list_id]->\
            active_timer_qty)

    return GL_SUCCESS;
}




/*****************************************************************************
* Function name    : gl_tm_check_timer_expiry
* Arguments        : gl_timer_list_t    timer_list_id
* Return Value     : void
* Functionality    : Checks for the Expired timers in the specified timer 
*                    list, set expired timer status as free.
* Functions called : tm_check_timer_list_expire
*****************************************************************************/
void 
gl_tm_check_timer_expiry 

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id ) 
#else
    ( timer_list_id )
      gl_timer_list_t           timer_list_id;
#endif
    
/****************************************************************************/
{
    gw_U16bit                   i;

#ifdef GL_STATIC_VALIDATION_ENABLED
    /*
     * If timer list id invalid simply return.
     */
    if(GL_TM_MAX_TIMER_LIST < timer_list_id)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        return;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    if(GL_FALSE == gl_tm_timer_server_flag)
    {
        if((gl_timer_list_t)0 == timer_list_id)
        {
            /*
             * Check the timer expiry for all the timer-lists.
             */
            for(i=0; i<GL_TM_MAX_TIMER_LIST; i++)
            {
                if(GL_NULL != a_p_timer_list[i])
                {
                    /*
                     * If no active timers in the list simply move ahead.
                     */ 
                    if(GL_NULL != a_p_timer_list[i]->p_first)
                    {
                        tm_check_timer_list_expire(i);
                    }

                    /** update last_time_stamp even if timer list is empty */
                    else
                    {
                        gl_tm_get_timeval(&(a_p_timer_list[i]->last_time_stamp));
                    }
                }
            }

        }
        else
        {
            if(GL_NULL != a_p_timer_list[timer_list_id - 1])
            {
                /*
                 * If no active timers in the list simply move ahead.
                 */ 
                if(GL_NULL != a_p_timer_list[timer_list_id - 1]->p_first)
                {
                    tm_check_timer_list_expire((gl_timer_list_t)(timer_list_id 
                                - 1));    
                }
                /** update last_time_stamp even if timer list is empty */
                else
                {
                    gl_tm_get_timeval(&(a_p_timer_list[timer_list_id - 1]->last_time_stamp));
                }    
            }
        }
    }
#ifdef GL_MT_ENABLED
    else
    {
        gl_error_t                  ecode;
        /*
         * Signal the timer server.
         */
        gl_release_semaphore(&gl_tm_main_event, &ecode);

        gl_release_semaphore(&gl_tm_timer_task, &ecode);
    }
#endif
    return;
}
      



/*****************************************************************************
* Function name    : gl_tm_stop_all_timer
* Arguments        : gl_timer_list_t     timer_list_id,
*                    gw_U32bit           *p_timer_qty, 
*                    gl_timer_id_t       a_timer_id[], 
*                    gl_pvoid_t          a_p_timer_buf[], 
*                    gl_error_t          *p_ecode
* Return Value     : gl_return_t 
* Functionality    : This is the function to stop all the timers. The function
*                       - removes all the timerIds from the timer list
*                       - releases the timer info buffer in timer_list
*                       - initializes the    timer_list structure.
*                       - returns all the p_timer_bufs.
* Functions called : 
*****************************************************************************/
gl_return_t 
gl_tm_stop_all_timer

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gw_U32bit                 *p_timer_qty, 
      gl_timer_id_t             a_timer_id[],
      gl_pvoid_t                a_p_timer_buf[],
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, p_timer_qty, a_timer_id, a_p_timer_buf, p_ecode )
      gl_timer_list_t           timer_list_id;
      gw_U32bit                 *p_timer_qty; 
      gl_timer_id_t             a_timer_id[];
      gl_pvoid_t                a_p_timer_buf[];
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U16bit                   i=0;
    gl_tm_timer_info_t          *p_timer_info=GL_NULL;
    gl_tm_timer_info_t          *p_temp_timer_info=GL_NULL;
#ifdef GL_MT_ENABLED
    gw_U16bit                   count=0;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(tm_mgr_init_flag == GL_FALSE)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INIT_REQD;
        return GL_FAILURE;
    } 
    if((GL_NULL == timer_list_id) || (GL_TM_MAX_TIMER_LIST < timer_list_id) ||
       (GL_NULL == a_p_timer_list[timer_list_id - 1]))
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_TIMER_LIST;
        return GL_FAILURE;
    }
#else
    /*
     * To remove warning, if GL_TIMER_LIST_ID_DISABLE flag set.
     */
    p_ecode = p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);

    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), p_ecode,
            GL_FAILURE);

    if(gl_tm_timer_server_flag == GL_FALSE)
    {
        *p_timer_qty = a_p_timer_list[timer_list_id]->active_timer_qty;

        for( i=0, p_timer_info= a_p_timer_list[timer_list_id]->p_first;
             p_timer_info != (gl_pvoid_t)GL_NULL && i<(*p_timer_qty); 
             i++)
        {
            a_timer_id[i] = p_timer_info->timer_id;
            a_p_timer_buf[i] = a_timer_app_list[timer_list_id].
                a_timer_index[p_timer_info->timer_id -1].p_timer_buf;
            
            set_free_timer(timer_list_id, p_timer_info->timer_id);
            
            p_temp_timer_info =  p_timer_info->fptr;
            p_timer_info->is_timer_active = GL_FALSE;
            p_timer_info->fptr = p_timer_info->bptr = GL_NULL;
            p_timer_info = p_temp_timer_info;
        }

        a_p_timer_list[timer_list_id]->active_timer_qty=0;
        a_p_timer_list[timer_list_id]->max_time=0;
        a_p_timer_list[timer_list_id]->last_time_stamp.sec = 0;
        a_p_timer_list[timer_list_id]->last_time_stamp.nsec = 0;
        a_p_timer_list[timer_list_id]->p_first= GL_NULL;
        a_p_timer_list[timer_list_id]->p_last=GL_NULL;
    }
#ifdef GL_MT_ENABLED
    else
    {
        for(i=0,count=0;i<a_timer_app_list[timer_list_id].max_timer_qty; i++)
        {
            if(a_timer_app_list[timer_list_id].a_timer_index[i].func_ptr 
                                                        != GL_NULL)
            {
                a_timer_id[count] = i+1;
                a_p_timer_buf[count++] = 
                    a_timer_app_list[timer_list_id].a_timer_index[i]
                                                   .p_timer_buf;
                set_free_timer(timer_list_id, i+1);
            }
        }
        
        *p_timer_qty = count;
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        gl_tm_send_msg_to_server(GL_TM_STOP_ALL_TIMER, (gl_timer_list_t)
                (timer_list_id + 1), GL_NULL, GL_NULL, GL_NULL);
        /*
         * Set the event to signal to timer server.
         */
        gl_release_semaphore(&gl_tm_input_event, p_ecode);
        
        gl_release_semaphore(&gl_tm_timer_task, p_ecode);

        GL_TM_C_TIMER_RUNING(timer_list_id, 0)
            
        return GL_SUCCESS;
    }
#endif
    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
        p_ecode, GL_FAILURE);
    
    GL_TM_C_TIMER_RUNING(timer_list_id, 0)
        
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_tm_get_timer_buff
* Arguments        : gl_timer_list_t    timer_list_id,
*                    gl_timer_id_t      timer_id, 
*                    gl_pvoid_t         *p_p_timer_buf,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t 
* Functionality    : This is the function to get the timer buffer.
*
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_get_timer_buff

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gl_timer_id_t             timer_id, 
      gl_pvoid_t                *p_p_timer_buf,
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, timer_id, p_p_timer_buf, p_ecode )
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id; 
      gl_pvoid_t                *p_p_timer_buf;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if((GL_NULL == timer_list_id) || (GL_TM_MAX_TIMER_LIST < timer_list_id) ||
       (GL_NULL == a_p_timer_list[timer_list_id - 1]))
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode = GL_TM_ERR_INV_TIMER_LIST;
        return GL_FAILURE;
    }
    /*
     * Check if timer id is invalid timer id 
     * max limit is same as the timer quantity.
     */
    if(timer_id == GL_NULL 
        || timer_id > a_timer_app_list[timer_list_id - 1].max_timer_qty)
    {
        *p_ecode = GL_TM_ERR_INV_TIMER_ID;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);

    GL_MT_LOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), p_ecode,
            GL_FAILURE);

    /*
     * Check if timer id is not currently active in the timer list
     * func_ptr for the timer id would be NULL if the timer is not 
     * currently active.
     */
    if(a_timer_app_list[timer_list_id].
        a_timer_index[timer_id-1].func_ptr == GL_NULL)
    {
        GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_EXPIRED;
        return GL_FAILURE;
    }

    /*
     * Get the pointer to the timer buffer.
     */
    *p_p_timer_buf = a_timer_app_list[timer_list_id].
        a_timer_index[timer_id -1].p_timer_buf;

    GL_MT_UNLOCK(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_tm_get_timer_list_ids
* Arguments        : gw_U32bit          user_info, - IN Argument 
*                    gw_U8bit           *p_num_timer_list_ids, - OUT Argument 
*                    gl_timer_list_t    *p_timer_list_ids      - OUT Argument 
* Return Value     : void 
* Functionality    : This is the function to get the list of timer list id, 
*                    associated with specified user info.
* Functions called :
*****************************************************************************/
void gl_tm_get_timer_list_ids

#ifdef _ANSI_C_
    ( gw_U32bit                 user_info, 
      gw_U8bit                  *p_num_timer_list_ids,
      gl_timer_list_t           *p_timer_list_ids
    )
#else
    ( user_info, p_num_timer_list_ids, p_timer_list_ids )
      gw_U32bit                 user_info;
      gw_U8bit                  *p_num_timer_list_ids;
      gl_timer_list_t           *p_timer_list_ids;
#endif
    
/****************************************************************************/
{
    gw_U16bit                    i;
    gw_U8bit                    num_timer_list_ids = GL_NULL; 

    for(i=0;i<GL_TM_MAX_TIMER_LIST;i++)
    {
        if(a_p_timer_list[i] == GL_NULL)
        {
            continue;
        }
        
        if(a_p_timer_list[i]->user_info == user_info)
        {
            *(p_timer_list_ids + num_timer_list_ids) = (gw_U8bit)(i + 1);
            num_timer_list_ids++; 
        }
    }
    
    *p_num_timer_list_ids = num_timer_list_ids;
}

