/***************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Porting functions
*                     for porting generic libraries on different OS
****************************************************************************
*
*    FILENAME    :    gl_gscim_mutex_oprns_wrppr_api.c
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
gl_mutex_init( void )
{
    return GL_SUCCESS;
}
/******************************************************************************/




/******************************************************************************/
gl_return_t 
gl_create_mutex

#ifdef _ANSI_C_
    ( GL_MUTEX_ID               *p_mutex_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_mutex_id, p_ecode )
      GL_MUTEX_ID               *p_mutex_id;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_OSE
    GL_MUTEX_ID      *p_local_mutex;
#endif

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_mutex_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_MUTEX_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_VXWORKS
    *p_mutex_id = NULL;
    if(NULL == (*p_mutex_id=semBCreate(SEM_Q_FIFO, SEM_FULL)))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_CREATE_FAILS;
        return GL_FAILURE;
    }
#elif GL_WINNT
    InitializeCriticalSection(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#elif GL_SOLARIS
    if((mutex_init(p_mutex_id,USYNC_THREAD,GL_NULL) != NULL))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_CREATE_FAILS;
        return GL_FAILURE;    
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    if((pthread_mutex_init(p_mutex_id, (const pthread_mutexattr_t *)
                    PTHREAD_PROCESS_PRIVATE) != 0))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_CREATE_FAILS;
        return GL_FAILURE;    
    }
#elif GL_OSE
    p_local_mutex = create_sem(1);
    gl_mm_memcpy((gw_U8bit *)p_mutex_id, (gw_U8bit *)p_local_mutex,
        sizeof(GL_MUTEX_ID));
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#endif 
    return GL_SUCCESS;
}
/******************************************************************************/




/******************************************************************************/
gl_return_t
gl_delete_mutex

#ifdef _ANSI_C_
    ( GL_MUTEX_ID               *p_mutex_id,
      gl_error_t                *p_ecode
    ) 
#else
    ( p_mutex_id, p_ecode )
      GL_MUTEX_ID               *p_mutex_id;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_mutex_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_MUTEX_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_VXWORKS
    if(ERROR == semDelete(*p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_DELETE_FAILS;
        return GL_FAILURE;    
    }
#elif GL_WINNT
    DeleteCriticalSection(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#elif GL_SOLARIS
    if(0 != mutex_destroy(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_DELETE_FAILS;
        return GL_FAILURE;    
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    if(0 != pthread_mutex_destroy(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_DELETE_FAILS;
        return GL_FAILURE;    
    }    
#elif GL_OSE
    kill_sem(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#endif 
    return GL_SUCCESS;
}
/******************************************************************************/


/******************************************************************************/
gl_return_t
gl_lock_mutex

#ifdef _ANSI_C_
    ( GL_MUTEX_ID               *p_mutex_id, 
      gw_U32bit                 flags,
      gl_error_t                *p_ecode
    )
#else
    ( p_mutex_id, flags, p_ecode )
      GL_MUTEX_ID               *p_mutex_id;
      gw_U32bit                 flags;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_mutex_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_MUTEX_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To remove warning.
     */
    flags = flags;

#ifdef GL_VXWORKS
    if(ERROR == semTake(*p_mutex_id, WAIT_FOREVER))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_LOCK_FAILS;
        return GL_FAILURE;    
    }
#elif GL_WINNT
    EnterCriticalSection(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#elif GL_SOLARIS
    if(0 != mutex_lock(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_LOCK_FAILS;
        return GL_FAILURE;    
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    if(0 != pthread_mutex_lock(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_LOCK_FAILS;
        return GL_FAILURE;    
    }    
#elif GL_OSE
    wait_sem(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#endif
    return GL_SUCCESS;
}
/******************************************************************************/


/******************************************************************************/
gl_return_t
gl_unlock_mutex

#ifdef _ANSI_C_
    ( gl_thread_info_t          *p_thread_info,
      GL_MUTEX_ID               *p_mutex_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_thread_info, p_mutex_id, p_ecode )
      gl_thread_info_t          *p_thread_info;
      GL_MUTEX_ID               *p_mutex_id;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_mutex_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_MUTEX_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To remove warning.
     */   
    p_thread_info = p_thread_info; 
    
#ifdef GL_VXWORKS
    if(ERROR == semGive(*p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_UNLOCK_FAILS;
        return GL_FAILURE;    
    }
#elif GL_WINNT
    LeaveCriticalSection(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#elif GL_SOLARIS
    if(0 != mutex_unlock(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_UNLOCK_FAILS;
        return GL_FAILURE;    
    }
#elif defined(GL_LINUX) || defined(GL_QNX)
    if(0 != pthread_mutex_unlock(p_mutex_id))
    {
        *p_ecode = GL_PORT_ERR_MUTEX_UNLOCK_FAILS;
        return GL_FAILURE;    
    }
#elif GL_OSE
    signal_sem(p_mutex_id);
    /*
     * To remove warning.
     */
    p_ecode = p_ecode;
#endif
    return GL_SUCCESS; 

}
/******************************************************************************/

#endif

