/***************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Porting functions
*                     for porting generic libraries on different OS
****************************************************************************
*
*    FILENAME    :    gl_gscim_thrd_oprns_wrppr_api.c
*
*    DESCRIPTION :    The function definitions which are used for porting the
*                     generic libraries on to different OS platforms.
*
*
* DATE         NAME        REFERENCE    REASON
*-------------     ---------------    ---------------    ----------------------
*
* Aug'00        Kapil Nayar    HSS:40600026     Generic Libraries
*                              HSS:40600036     (Gateways)
*
* 28/08/01      Prashant Jain   SPR-2276        Compilation With CC.
*
* 04/09/2001    Prashant Jain   SPR-2560        Adding OSE Operating
*                                               System Support.
* 04/10/2001    Ravi Jindal        -            Remove warning with gcc,
*                                               cc,CC on Solaris.
* 18/10/2001    Ravi Jindal     SPR-2788        Packaging of Genlib-Code. 
* 19/10/2001    Ravi Jindal     SPR-2802        Adding Pre-ANSI-C features.
* 29/10/2001    Ravi Jindal     SPR-2798        Adding Counting Semaphore
*                                               functions.
* 17/11/2001    Ravi Jindal     SPR-3107        Incoprate the defects and change
*                                               cosmmetics of file.
*
* 04/12/2001    Ravi Jindal     SPR-3339        Changed the interface of
*               Prashant Jain                   gl_try_wait_on_semaphore
*                                               to tackle some race conditions
*                                               in case this call fails.
*
* 07/01/2002    Ravi Jindal     SPR-2751        Remove warnings on WINNT and
*                                               VxWorks.
*
* 20/06/2002    Ravi Jindal     SPR-3959        Enhancements for Rel-1.0.
* 12/07/2004    Navneet Gupta   SPR 6946        QNX port
*                                              
*COPYRIGHT , Aricent, 2006
****************************************************************************/

#include "gl_gscim_interfaces.h"

#ifdef GL_MT_ENABLED

/******************************************************************************/
gl_return_t 
gl_thm_init( void )
{
    return GL_SUCCESS;
}
/******************************************************************************/


/******************************************************************************/
gl_return_t 
gl_thm_create

#ifdef _ANSI_C_
    ( gl_fn_ptr_t               entry_func_name, 
      gl_thread_info_t          *p_thread_info,
      gl_pvoid_t                p_param, 
      gl_error_t                *p_ecode
    )
#else
    ( entry_func_name, p_thread_info, p_param, p_ecode )
      gl_fn_ptr_t               entry_func_name; 
      gl_thread_info_t          *p_thread_info;
      gl_pvoid_t                p_param; 
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#if defined(GL_SOLARIS) || defined(GL_LINUX) || defined(GL_QNX)
    gw_U32bit                   return_error;
#elif GL_WINNT
    gl_thread_id_t              thread_id;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_thread_info)
    {
        *p_ecode = GL_PORT_ERR_NULL_THREAD_INFO_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_VXWORKS
    if (ERROR == (p_thread_info->thread_id.thr_id = taskSpawn(NULL, 50, 
                    VX_FP_TASK, 30000 /*stack size*/, entry_func_name,
                    (int)p_param,0,0,0,0,0,0,0,0,0)))  
    {
        *p_ecode = GL_PORT_ERR_THREAD_CREATE_FAILS;
        return GL_FAILURE;
    }
#elif GL_WINNT
    if (NULL == (p_thread_info->thread_id.thr_handle = CreateThread(NULL,0, 
                    (LPTHREAD_START_ROUTINE)entry_func_name,
                    (void *)p_param, 0, &thread_id)))  
    {
        *p_ecode = GL_PORT_ERR_THREAD_CREATE_FAILS;
        return GL_FAILURE;
    }
#elif GL_SOLARIS
    if(0 != (return_error = thr_create(NULL,NULL,entry_func_name,(void*)p_param,
                        NULL,&(p_thread_info->thread_id.thr_id))))
    {
        *p_ecode = GL_PORT_ERR_THREAD_CREATE_FAILS;
        return GL_FAILURE;
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    if(0 != (return_error = pthread_create(&(p_thread_info->thread_id.thr_id),
                        NULL,entry_func_name,(void*)p_param)))    
    {
        *p_ecode = GL_PORT_ERR_THREAD_CREATE_FAILS;
        return GL_FAILURE;
    }     
#endif
    
    return GL_SUCCESS;
    
}
/******************************************************************************/


/******************************************************************************/
gl_return_t 
gl_thm_get_thread_id

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_ecode )
      gl_thread_info_t          *p_thread_info;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_thread_info)
    {
        *p_ecode = GL_PORT_ERR_NULL_THREAD_INFO_PTR;
        return GL_FAILURE;
    }
#else
    /*
     * To remove waning.
     */
    (void)*p_ecode;
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_VXWORKS
    p_thread_info->thread_id.thr_id = taskIdSelf();
#elif GL_WINNT
    /*
     * As presently all the thread related calls on WINNT requires
     * Handle to thread as input parameter.
     */ 
    p_thread_info->thread_id.thr_handle = GetCurrentThread();
#elif GL_SOLARIS
    p_thread_info->thread_id.thr_id = thr_self();
#elif defined(GL_LINUX) || defined(GL_QNX)
    p_thread_info->thread_id.thr_id = pthread_self();
#elif GL_OSE
    p_thread_info->thread_id.thr_id = pthread_self();
#endif
    return GL_SUCCESS;
}
/******************************************************************************/



/******************************************************************************/
gl_return_t 
gl_thm_suspend

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_ecode )
      gl_thread_info_t          *p_thread_info;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
#ifdef GL_WINNT
    DWORD return_status = (DWORD)0xffffffff;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_thread_info)
    {
        *p_ecode = GL_PORT_ERR_NULL_THREAD_INFO_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_SOLARIS
    if(0 != thr_suspend(p_thread_info->thread_id.thr_id))
    {
        *p_ecode = GL_PORT_ERR_THREAD_SUSPEND_FAILS;
        return GL_FAILURE;
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    /*
     * No call for thread suspend.
     * So to remove warnings.
     */ 
     (void)*p_thread_info;
     (void)*p_ecode;
#elif GL_VXWORKS
    if(ERROR == taskSuspend(p_thread_info->thread_id.thr_id))
    {
        *p_ecode = GL_PORT_ERR_THREAD_SUSPEND_FAILS;
        return GL_FAILURE;
    }
#elif GL_WINNT
    if(return_status == SuspendThread(p_thread_info->thread_id.thr_handle)) 
    {
        *p_ecode = GL_PORT_ERR_THREAD_SUSPEND_FAILS;
        return GL_FAILURE;
    }
#endif
    return GL_SUCCESS;
}
/******************************************************************************/


        


/******************************************************************************/
gl_return_t
gl_thm_resume

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_ecode )
      gl_thread_info_t          *p_thread_info;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
#ifdef GL_WINNT
    DWORD return_status = (DWORD)0xffffffff;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_thread_info)
    {
        *p_ecode = GL_PORT_ERR_NULL_THREAD_INFO_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_SOLARIS
    if(0 != thr_continue(p_thread_info->thread_id.thr_id))
    {
        *p_ecode = GL_PORT_ERR_THREAD_RESUME_FAILS;
        return GL_FAILURE;
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    /*
     * No call for thread resume.
     * So to remove warnings.
     */ 
     (void)*p_thread_info;
     (void)*p_ecode;
#elif GL_VXWORKS
    if(ERROR == taskResume(p_thread_info->thread_id.thr_id))
    {
        *p_ecode = GL_PORT_ERR_THREAD_RESUME_FAILS;
        return GL_FAILURE;
    }
#elif GL_WINNT
    if(return_status == ResumeThread(p_thread_info->thread_id.thr_handle)) 
    {
        *p_ecode = GL_PORT_ERR_THREAD_RESUME_FAILS;
        return GL_FAILURE;
    }
#endif
    return GL_SUCCESS;
}
/******************************************************************************/


        


    
/******************************************************************************/
gl_return_t 
gl_thm_destroy

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_ecode )
      gl_thread_info_t          *p_thread_info;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
#if defined(GL_SOLARIS) || defined(GL_LINUX) || defined(GL_QNX)
    gl_pvoid_t p_return_status = (gl_pvoid_t)GL_NULL;
#elif GL_WINNT
    DWORD return_status = GL_NULL;
    BOOL  close_ret_val;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_thread_info)
    {
        *p_ecode = GL_PORT_ERR_NULL_THREAD_INFO_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_SOLARIS
    /*
     * To remove Warning.
     */
    p_ecode = p_ecode;
    p_thread_info = p_thread_info;
    
    thr_exit(p_return_status);
#elif defined(GL_LINUX) || defined(GL_QNX)
    /*
     * To remove Warning.
     */
    p_ecode = p_ecode;
    p_thread_info = p_thread_info;
    
    pthread_exit(p_return_status);
#elif GL_VXWORKS
    if(ERROR == taskDelete(p_thread_info->thread_id.thr_id))
    {
        *p_ecode = GL_PORT_ERR_THREAD_DESTROY_FAILS;
        return GL_FAILURE;
    }
#elif GL_WINNT
    if((BOOL)0 == GetExitCodeThread(p_thread_info->thread_id.thr_handle, 
                  &return_status) )
    {
        *p_ecode = GL_PORT_ERR_THREAD_DESTROY_FAILS;
        return GL_FAILURE;
    }
    else
    {
        close_ret_val = CloseHandle(p_thread_info->thread_id.thr_handle);
        ExitThread(return_status );
    }
#endif
    return GL_SUCCESS;
}
/******************************************************************************/
        
#endif
