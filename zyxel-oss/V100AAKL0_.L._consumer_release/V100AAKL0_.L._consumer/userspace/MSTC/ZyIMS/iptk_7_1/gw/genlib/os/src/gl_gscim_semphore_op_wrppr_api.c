/***************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Porting functions
*                     for porting generic libraries on different OS
****************************************************************************
*
*    FILENAME    :    gl_gscim_semphore_op_wrppr_api.c
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
gl_create_semaphore

#ifdef _ANSI_C_
    ( GL_SEMAPHORE_ID           *p_sem_id,
      gw_U32bit                 initial_value,
      gw_U32bit                 maximum_value,
      gl_error_t                *p_ecode
    )
#else
    ( p_sem_id, initial_value, maximum_value, p_ecode )
      GL_SEMAPHORE_ID           *p_sem_id;
      gw_U32bit                 initial_value;
      gw_U32bit                 maximum_value;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_OSE
    GL_MUTEX_ID                 *p_local_sem;
#endif

#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_sem_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_SEMAPHORE_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_WINNT
    if( NULL == (*p_sem_id = CreateSemaphore( NULL, initial_value, 
                              maximum_value, GL_NULL)))
    {
        /* 
         * The CreateSemaphore system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_CREATE_FAIL;
        return GL_FAILURE;
    }
#elif defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_LINUX)  || defined(GL_QNX)
    /*
     * To remove warning.
     */
    maximum_value = maximum_value;
    if(0 != sem_init(p_sem_id, 0, initial_value))
    {
        /* 
         * The sem_init system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_CREATE_FAIL;
        return GL_FAILURE;
    }
#elif GL_OSE
    p_local_sem = create_sem(initial_value);
    gl_mm_memcpy((gw_U8bit *)p_sem_id, (gw_U8bit *)p_local_sem,
        sizeof(GL_MUTEX_ID));
    /*
     * To remove warning.
     */
    maximum_value = maximum_value;
    p_ecode = p_ecode;
#endif
    return GL_SUCCESS;
}
/******************************************************************************/



/******************************************************************************/
gl_return_t 
gl_delete_semaphore

#ifdef _ANSI_C_
    ( GL_SEMAPHORE_ID           *p_sem_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_sem_id, p_ecode )
      GL_SEMAPHORE_ID           *p_sem_id;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_WINNT
    BOOL                        temp_bool_var;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_sem_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_SEMAPHORE_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_WINNT
    if( GL_NULL == (temp_bool_var = CloseHandle(*p_sem_id )))
    {
        /* 
         * The CloseHandle system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_DELETE_FAIL;
        return GL_FAILURE;
    }
#elif defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_LINUX) || defined(GL_QNX)
    if ( 0 != sem_destroy(p_sem_id))
    {
        /* 
         * The sem_destroy system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_DELETE_FAIL;
        return GL_FAILURE;
    }
#elif GL_OSE
    kill_sem(p_sem_id);
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
gl_wait_on_semaphore
                                  
#ifdef _ANSI_C_
    ( GL_SEMAPHORE_ID           *p_sem_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_sem_id, p_ecode )
      GL_SEMAPHORE_ID           *p_sem_id;
      gl_error_t                *p_ecode;
#endif
    
/******************************************************************************/
{
#ifdef GL_WINNT
    DWORD                       temp_dword_var;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_sem_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_SEMAPHORE_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_WINNT
    if(WAIT_OBJECT_0 != (temp_dword_var = WaitForSingleObject(*p_sem_id,
                    INFINITE)))
    {
        /* 
         * The WaiForSingleObject system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_WAIT_FAIL;
        return GL_FAILURE;
    }
#elif defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_LINUX) || defined(GL_QNX)
    if( 0 != sem_wait(p_sem_id))
    {
        /* 
         * The sem_wait system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_WAIT_FAIL;
        return GL_FAILURE;
    }
#elif GL_OSE
    /*
     * As return type and Error Codes are not Known on OSE.
     */ 
    p_ecode = p_ecode;
    wait_sem(p_sem_id);
#endif
    return GL_SUCCESS;
}
/******************************************************************************/



/******************************************************************************/
gl_return_t 
gl_try_wait_on_semaphore

#ifdef _ANSI_C_
    ( GL_SEMAPHORE_ID           *p_sem_id,
      gl_boolean_t              *p_is_already_locked,
      gl_error_t                *p_ecode
    )
#else
    ( p_sem_id, p_is_already_locked, p_ecode )
      GL_SEMAPHORE_ID           *p_sem_id;
      gl_boolean_t              *p_is_already_locked;
      gl_error_t                *p_ecode;
#endif
    
/*******************************************************************************/
{
#ifdef GL_WINNT
    DWORD                       temp_dword_var;
#elif defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_LINUX) || defined(GL_QNX)
    gw_S32bit                   ret_val;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_sem_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_SEMAPHORE_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */


#ifdef GL_WINNT
    temp_dword_var = WaitForSingleObject(*p_sem_id, 0);

    if((WAIT_FAILED == temp_dword_var))
    {
        /* 
         * The WaiForSingleObject system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_TRY_WAIT_FAIL;
        return GL_FAILURE;
    }
    else if(WAIT_OBJECT_0 == temp_dword_var)
    {
        *p_is_already_locked = GL_FALSE;
    }
    else if(WAIT_TIMEOUT == temp_dword_var)
    {
        *p_is_already_locked = GL_TRUE;
    }
    return GL_SUCCESS;

#elif defined(GL_SOLARIS) || defined(GL_LINUX) || defined(GL_QNX)
    ret_val = sem_trywait(p_sem_id);

    if( (-1) == ret_val)
    {
        /* 
         * The sem_trywait system call failed
         */ 
        if( EAGAIN == errno)
        {
             *p_is_already_locked = GL_TRUE;
             return GL_SUCCESS;
        }
        *p_ecode = GL_PORT_ERR_SEM_TRY_WAIT_FAIL;
        return GL_FAILURE;
    }
    *p_is_already_locked = GL_FALSE;

#elif defined(GL_VXWORKS)
    ret_val = sem_trywait(p_sem_id);

    if((-1) == ret_val)
    {
        /* 
         * The sem_trywait system call failed
         */ 
        if( EAGAIN == errnoGet( ))
        {
             *p_is_already_locked = GL_TRUE;
             return GL_SUCCESS;
        }
        *p_ecode = GL_PORT_ERR_SEM_TRY_WAIT_FAIL;
        return GL_FAILURE;
    }
    *p_is_already_locked = GL_FALSE;
#elif GL_OSE
    /*
     * As return type and Error Codes are not Known on OSE.
     */ 
    if(0 < get_sem(p_sem_id))
    {
        *p_is_already_locked = GL_FALSE;
    }
    else
    {
        *p_is_already_locked = GL_TRUE;
        *p_ecode = GL_PORT_ERR_SEM_TRY_WAIT_FAIL;
        return GL_FAILURE;
    }
#endif
    return GL_SUCCESS;
}
/******************************************************************************/



/******************************************************************************/
gl_return_t
gl_release_semaphore

#ifdef _ANSI_C_
    ( GL_SEMAPHORE_ID           *p_sem_id,
      gl_error_t                *p_ecode
    )
#else
    ( p_sem_id, p_ecode )
      GL_SEMAPHORE_ID           *p_sem_id;
      gl_error_t                *p_ecode;
#endif
    
/*******************************************************************************/
{
#ifdef GL_WINNT
    BOOL                        temp_bool_var;
#endif
    
#ifdef GL_STATIC_VALIDATION_ENABLED
    if(GL_NULL == p_sem_id)
    {
        *p_ecode = GL_PORT_ERR_NULL_SEMAPHORE_PTR;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

#ifdef GL_WINNT
    if(GL_NULL == (temp_bool_var = ReleaseSemaphore(*p_sem_id,1,NULL)))
    {
        /* 
         * The ReleaseSemaphore system call failed
         */
        *p_ecode = GL_PORT_ERR_SEM_RELEASE_FAIL;
        return GL_FAILURE;
    }
#elif defined(GL_SOLARIS) || defined(GL_VXWORKS) || defined(GL_LINUX) || defined(GL_QNX)
    if( 0 != sem_post(p_sem_id))
    {
        /* 
         * The  sem_post system call failed
         */ 
        *p_ecode = GL_PORT_ERR_SEM_RELEASE_FAIL;
        return GL_FAILURE;
    }
#elif GL_OSE
    /*
     * As return type and Error Codes are not Known on OSE.
     */ 
    p_ecode = p_ecode;
    signal_sem(p_sem_id);
#endif
    return GL_SUCCESS;
}
/******************************************************************************/

#endif


