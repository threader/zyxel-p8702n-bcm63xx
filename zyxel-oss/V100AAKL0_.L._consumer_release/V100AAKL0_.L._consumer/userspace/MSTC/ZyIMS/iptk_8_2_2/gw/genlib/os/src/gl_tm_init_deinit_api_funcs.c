/***************************************************************************** 
*
*    FUNCTION    : Source file for Generic Library - Timer Manager 
*                  implementation.  
******************************************************************************
*
*    FILENAME    : gl_tm_init_deinit_api_funcs.c
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
* 28/03/2008 Shruti Thakral      SPR 18252      Klocwork warning removal
* 
* COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_tm.h"

    

/*****************************************************************************
* Function name    : gl_tm_init
* Arguments        : void
* Return Value     : void
* Functionality    : Initializes the generic libraries timer manager.
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_init( void )
/****************************************************************************/
{
    gw_U32bit                   i;
    gl_error_t                  ecode;

    if(tm_mgr_init_flag == GL_FALSE)
    {
        /*
         * Create semaphore for MT safe operation.
         */
#ifdef GL_MT_ENABLED
        if(GL_FAILURE == gl_create_mutex((&tm_mgr_sem), &ecode))
        {
            GL_INC_TM_G_MUTX_FAIL()

             return GL_FAILURE;
        } 
#ifdef GL_STATS_ENABLED
        g_tm_stat_init = GL_TRUE;

        if(GL_FAILURE == gl_create_mutex(&g_tm_stat_sem, &ecode))
        {
            GL_INC_TM_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#endif /* Endif for GL_STATS_ENABLED */
        
#else
        ecode = 0;
#endif

        /*
         * Initialize the timer list and timer_app_list elements.
         */
        for(i=0;i<GL_TM_MAX_TIMER_LIST;i++)
        {
            a_p_timer_list[i] = GL_NULL;
            a_timer_app_list[i].max_timer_qty = GL_NULL;
            a_timer_app_list[i].first_free_timer_index = GL_NULL;
            a_timer_app_list[i].last_free_timer_index = GL_NULL;
            a_timer_app_list[i].a_timer_index = GL_NULL;
        }
        tm_mgr_init_flag = GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}


/*****************************************************************************
* Function name    : gl_tm_timer_list_create
* Arguments        : gw_U32bit          user_info,
*                    gw_U32bit          max_timer_qty, 
*                    gl_timer_list_t    *p_timer_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the new timer list structure and initializes the 
*                    timer list structure elements and the timer index list.
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_timer_list_create

#ifdef _ANSI_C_
    ( gw_U32bit                 user_info, 
      gw_U32bit                 max_timer_qty,
      gl_timer_list_t           *p_timer_list_id, 
      gl_error_t                *p_ecode
    )
#else
    ( user_info, max_timer_qty, p_timer_list_id, p_ecode )
      gw_U32bit                 user_info;
      gw_U32bit                 max_timer_qty;
      gl_timer_list_t           *p_timer_list_id; 
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U16bit                   i;
    gw_U16bit                   timer_list_id= GL_NULL;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(tm_mgr_init_flag == GL_FALSE)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

         *p_ecode = GL_TM_ERR_INIT_REQD;
         return GL_FAILURE; 
    }
    if(max_timer_qty ==0 || max_timer_qty > GL_TM_MAX_TIMER_QTY)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

        *p_ecode= GL_TM_ERR_INV_TIMER_QTY;
        return GL_FAILURE ;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    
    GL_MUTEX_LOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);
    /*
     * Get the free index for timer list Id.
     */
    for(i=0; i<GL_TM_MAX_TIMER_LIST; i++)
    {
        if( a_p_timer_list[i]==(gl_pvoid_t)GL_NULL)
        {
            timer_list_id = i;
            break;
        }
    }

    /*
     * If index not available return error.
     */
    if(i == GL_TM_MAX_TIMER_LIST)
    {
        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }

    /*
     * If malloc returns error return failure.
     */
    if(GL_NULL == (a_p_timer_list[timer_list_id] = 
        (gl_tm_timer_list_t*)SYS_MALLOC(sizeof(gl_tm_timer_list_t))))
    {
        GL_INC_TM_G_MALOC_FAIL()
            
        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }

#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_tm_stat_allocte_memory(timer_list_id, p_ecode))
    {
        GL_INC_TM_G_MALOC_FAIL()

        SYS_FREE(a_p_timer_list[timer_list_id]);
        a_p_timer_list[timer_list_id] = GL_NULL;

        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif
    
#ifdef GL_TOTAL_MT_SAFE   
    if(GL_FAILURE == gl_create_mutex(&(a_p_timer_list[timer_list_id]->
            timer_list_sem), p_ecode))
    {
        GL_INC_TM_G_MUTX_FAIL()
            
        SYS_FREE(a_p_timer_list[timer_list_id]);
        a_p_timer_list[timer_list_id] = GL_NULL;

        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif

    /*
     * Initialize the timer id list from which timer ids are 
     * allocated and deallocated.
     */
    if( GL_SUCCESS != timer_id_list_init(timer_list_id, max_timer_qty))
    {
        GL_MT_DELETE(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

        SYS_FREE(a_p_timer_list[timer_list_id]);
        a_p_timer_list[timer_list_id] = GL_NULL;
        
        GL_INC_TM_G_MALOC_FAIL()
            
        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_TM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }

    /*
     * Set the parameters of the timer list allocated.
     */
    a_timer_app_list[timer_list_id].max_timer_qty = max_timer_qty;
    a_p_timer_list[timer_list_id]->active_timer_qty = GL_NULL; 
    a_p_timer_list[timer_list_id]->user_info = user_info;
    a_p_timer_list[timer_list_id]->last_time_stamp.sec = GL_NULL;
    a_p_timer_list[timer_list_id]->last_time_stamp.nsec = GL_NULL;
    a_p_timer_list[timer_list_id]->max_time = GL_NULL;
    a_p_timer_list[timer_list_id]->p_first = (gl_tm_timer_info_t *)GL_NULL;
    a_p_timer_list[timer_list_id]->p_last = (gl_tm_timer_info_t*)GL_NULL;
    *p_timer_list_id = (gl_timer_list_t)(timer_list_id + 1);
    
    GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);
    
    GL_TM_C_TIMER_QTY(timer_list_id, max_timer_qty)
    GL_TM_C_TIMER_RUNING(timer_list_id, 0)
    
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_tm_delete_timer_list
* Arguments        : gw_U32bit          task_id,
*                    gl_timer_list_t    timer_list_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the timer list and resets the 
*                    structure elements.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_tm_delete_timer_list

#ifdef _ANSI_C_
    ( gl_timer_list_t           timer_list_id, 
      gl_error_t                *p_ecode
    )
#else
    ( timer_list_id, p_ecode )
      gl_timer_list_t           timer_list_id; 
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
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * As timer_list_id ranges from 1 - GL_TM_MAX_TIMER_LIST.
     */
    timer_list_id = (gl_timer_list_t)(timer_list_id - 1);

    GL_MUTEX_LOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

    if( a_p_timer_list[timer_list_id]->active_timer_qty != 0)
    {
        GL_INC_TM_G_DEL_RQ_LIST_BSY()
            
        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode= GL_TM_ERR_ACTIVE_TIMERS_PRESENT;
        return GL_FAILURE;
    }

    GL_MT_DELETE(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

#ifdef GL_STATS_ENABLED
    gl_tm_stat_free_memory(timer_list_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

    SYS_FREE((gl_pvoid_t)a_p_timer_list[timer_list_id]->a_index_info);
    SYS_FREE((gl_pvoid_t)a_p_timer_list[timer_list_id]);
    SYS_FREE((gl_pvoid_t)a_timer_app_list[timer_list_id].a_timer_index);
    a_p_timer_list[timer_list_id] = GL_NULL;
    a_timer_app_list [timer_list_id].a_timer_index = GL_NULL;
    a_timer_app_list [timer_list_id].max_timer_qty = GL_NULL;
    a_timer_app_list [timer_list_id].first_free_timer_index = GL_NULL;
    a_timer_app_list [timer_list_id].last_free_timer_index = GL_NULL;
    
    GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

    return GL_SUCCESS;
} 



/*****************************************************************************
* Function name    : gl_tm_destroy_all_timer_lists
* Arguments        : gw_U8bit           is_forceful_cleanup,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the timer lists and the corresponding 
*                    occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t  gl_tm_destroy_all_timer_lists

#ifdef _ANSI_C_
    ( gw_U8bit                  is_forceful_cleanup,
      gl_error_t                *p_ecode
    )
#else
    ( is_forceful_cleanup, p_ecode )
      gw_U8bit                  is_forceful_cleanup;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U16bit                   timer_list_id= GL_NULL;
    gl_tm_timer_info_t          *p_current_ptr = GL_NULL;
    gl_tm_timer_info_t          *p_temp_current_ptr = GL_NULL;
    gw_boolean                  is_active_timer_flag = GL_FALSE;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(tm_mgr_init_flag == GL_FALSE)
    {
        GL_INC_TM_G_PARM_VLD_FAIL()

         *p_ecode = GL_TM_ERR_INIT_REQD;
         return GL_FAILURE;
    } 
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

    for(timer_list_id=0; timer_list_id < GL_TM_MAX_TIMER_LIST; timer_list_id++)
    {
        if( a_timer_app_list[timer_list_id].max_timer_qty == 0)
        {
            continue;
        }                 
        if(GL_FALSE == is_forceful_cleanup &&
           a_p_timer_list[timer_list_id]->active_timer_qty != 0)
        {
            is_active_timer_flag = GL_TRUE;
            continue;
        }
        for(p_current_ptr = a_p_timer_list[timer_list_id]->p_first; 
            p_current_ptr != GL_NULL; )
        {
            p_temp_current_ptr = p_current_ptr->fptr;
            SYS_FREE((gl_pvoid_t)p_current_ptr);
            p_current_ptr = p_temp_current_ptr;    
        }
        
        GL_MT_DELETE(&(a_p_timer_list[timer_list_id]->timer_list_sem), 
                p_ecode, GL_FAILURE);

#ifdef GL_STATS_ENABLED
        gl_tm_stat_free_memory(timer_list_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        SYS_FREE((gl_pvoid_t)a_timer_app_list[timer_list_id].a_timer_index);
        SYS_FREE((gl_pvoid_t)a_p_timer_list[timer_list_id]->a_index_info);
        SYS_FREE((gl_pvoid_t)a_p_timer_list[timer_list_id]);
        a_p_timer_list[timer_list_id] = GL_NULL;
    }

    if( GL_TRUE == is_active_timer_flag )
    {
        /* 
         * In some timer list timers are active.
         */
    
        GL_MUTEX_UNLOCK(&tm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode= GL_TM_ERR_ACTIVE_TIMERS_PRESENT;
        return GL_FAILURE;
    }
    else
    {
        GL_MUTEX_DELETE(&tm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
#ifdef GL_MT_ENABLED
        if(GL_TRUE == g_tm_stat_init)
        {
            gl_delete_mutex(&g_tm_stat_sem, p_ecode);
        }
        g_tm_stat_init = GL_FALSE;
#endif
#endif /* Endif for GL_STATS_ENABLED */
        tm_mgr_init_flag = GL_FALSE;
        return GL_SUCCESS;
    }
}

