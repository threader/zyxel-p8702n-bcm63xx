/*******************************************************************************
*
* FUNCTION    : Generic Libraries - Message Queue Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_mqm_glb_defs_n_intrnl_funcs.c
*
* DESCRIPTION : This file contains the implementation routines of message queue
*               manager in the generic library.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* Oct'99     Kapil Nayar    HSS:40600026      Generic Libraries
*                           HSS:40600036      (Gateways)
* 21/1/2001  Sachin Mittal                    Reorganised and moved in common 
* 23/5/2001  Menuka Jain                      Merging of code and adding vxworks
*                                             option
* 24/10/2001 Ravi Jindal    SPR-2792          Adding return failure for 
*                                             semaphore fail operation in all 
*                                             functions.
*
* 29/10/2001 Ravi Jindal    SPR-2798          Change the counting semaphore to 
*                                             binary semaphore and add 
*                                             read_write mutex to access both 
*                                             read and write pointer to check
*                                             queue is empty or full.
*
* 04/12/2001 Ravi Jindal    SPR-3335          Wrong Mutex is being unlocked.   
*            Prashant Jain                    
*                                             
* 04/12/2001 Ravi Jindal    SPR-3336          Mutex is not freed in funtion 
*            Prashant Jain                    gl_mqm_peek_mesg           
*                                             
* 04/12/2001 Ravi Jindal    SPR-3339          Changed the interface of 
*            Prashant Jain                    gl_try_wait_on_semaphore 
*                                             to tackle some race conditions
*                                             in case this call fails.          
*
* 04/01/2002 Ravi Jindal    SPR-3432          Fix the bug in gl_mqm_destroy_
*                                             all_queues. 
*                                             
* 20/03/2002 Ravi Jindal    SPR-3956          Enhancements for Rel-1.0.
*                                             
* 21/06/2002 Saurabh Kr.    SPR-3956          Enhancements for Rel-1.0.
*            Sharma
*
*
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

#include "gl_mqm.h"

/*
 * Array of message queue structures where the
 * message queues are added at runtime
 */
gl_mqm_q_t             *g_a_p_mesg_q[GL_MQM_MAX_MESG_Q];

#ifdef GL_MT_ENABLED
    GL_MUTEX_ID            mqm_mgr_sem;
#endif


gl_boolean                 mqm_mgr_init_flag = GL_FALSE;



/*****************************************************************************
* Function name    : mqm_is_abs_priority_mesg
* Arguments        : gl_mq_id_t   mesg_q_id,
*                    gw_U8bit     priority
* Return Value     : gl_return_t
* Functionality    : To Check is any message at given priority level.
* Functions called :
*****************************************************************************/
gl_return_t
mqm_is_abs_priority_mesg

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority
    )
#else
    ( mesg_q_id, priority )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
#endif

/******************************************************************************/
{
    gl_mqm_priority_level_t *p_req_priority_level;
    /*
     * Check Is their any Message At Head of Requested Priority Level.
     */
    p_req_priority_level = g_a_p_mesg_q[mesg_q_id]->a_priority_level + priority;
    
    if(GL_NULL != p_req_priority_level->p_first_mesg)
    {
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}



/*****************************************************************************
* Function name    : mqm_is_relative_priority_mesg
* Arguments        : gl_mq_id_t   mesg_q_id,
*                    gw_U8bit     priority,
*                    gw_U8bit     *p_actual_priority
* Return Value     : gl_return_t
* Functionality    : To Check, if any message at priority levels
*                    Starting from Highest Priority level (0) and up to
*                    given Priority level.
* Functions called :
*****************************************************************************/
gl_return_t
mqm_is_relative_priority_mesg

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gw_U8bit                  *p_actual_priority
    )
#else
    ( mesg_q_id, priority, p_actual_priority )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gw_U8bit                  *p_actual_priority;
#endif

/******************************************************************************/
{
    gw_U8bit                i;
    gl_mqm_priority_level_t *p_req_priority_level;
    
    /*
     * Check Is their any Message At Head of Requested Priority Levels
     * Starting from Highest Priority level (0) up to given Priority Level.
     */
    for(i=0; i <= priority; i++)
    {
        p_req_priority_level = g_a_p_mesg_q[mesg_q_id]->a_priority_level + i;
    
        if( GL_NULL != p_req_priority_level->p_first_mesg)
        {
            *p_actual_priority = i;
            return GL_SUCCESS;
        }
    }
    return GL_FAILURE;
}




/*****************************************************************************
* Function name    : mqm_allocate_memory
* Arguments        : gl_mq_id_t   mesg_q_id,
*                    gw_U32bit    buf_qty,
*                    gw_U32bit    buf_size,
*                    gl_error_t*  p_ecode
* Return Value     : gl_return_t
* Functionality    : To reserve memory for Message Queue Nodes.
* Functions called :
*****************************************************************************/
gl_return_t
mqm_allocate_memory

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U32bit                 buf_qty,
      gw_U32bit                 buf_size,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, buf_qty, buf_size, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U32bit                 buf_qty;
      gw_U32bit                 buf_size;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * In present Implementation For Memory Requirements 
     * GENLIB Supported Memory Manager is used.
     * A task memory pool is created with task_id = GLNULL.
     */ 
    return ( gl_mm_pool_create ( GL_NULL,
                                 buf_qty,
                                 buf_size,
                                 &(g_a_p_mesg_q[mesg_q_id]->pool_id),
                                 p_ecode)  
            );
}

    
/*****************************************************************************
* Function name    : mqm_get_memory
* Arguments        : gl_mq_id_t   mesg_q_id,
*                    gl_error_t*  p_ecode
* Return Value     : gl_pvoid_t
* Functionality    : To get memory for Message Queue Node.
* Functions called :
*****************************************************************************/
gl_pvoid_t
mqm_get_memory

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * In present Implementation For Memory Requirements
     * GENLIB Supported Memory Manager is used.
     * A task memory pool buffer is requested with task_id = GLNULL.
     */
    return ( gl_mm_get_buffer_from_pool(
                                 GL_NULL,
                                 g_a_p_mesg_q[mesg_q_id]->pool_id,
                                 p_ecode)
            );
}



/*****************************************************************************
* Function name    : mqm_release_memory
* Arguments        : gl_mq_id_t   mesg_q_id,
*                    gl_pvoid_t   buf_ptr,
*                    gl_error_t*  p_ecode
* Return Value     : gl_return_t
* Functionality    : To get memory for Message Queue Node.
* Functions called :
*****************************************************************************/
gl_return_t
mqm_release_memory

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gl_pvoid_t                buf_ptr,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, buf_ptr, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gl_pvoid_t                buf_ptr;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    /*
     * In present Implementation For Memory Requirements
     * GENLIB Supported Memory Manager is used.
     * A task memory pool buffer is released with task_id = GLNULL.
     */
    return ( gl_mm_release_buffer_to_pool(
                                 GL_NULL,
                                 g_a_p_mesg_q[mesg_q_id]->pool_id,
                                 buf_ptr,
                                 p_ecode)
            );
}


/*****************************************************************************
* Function name    : mqm_free_memory
* Arguments        : gl_mq_id_t   mesg_q_id,
* Return Value     : void
* Functionality    : To free memory reserved for Message Queue.
* Functions called :
*****************************************************************************/
void
mqm_free_memory

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id )
#else
    ( mesg_q_id )
      gl_mq_id_t                mesg_q_id;
#endif

/******************************************************************************/
{
    gl_error_t                  ecode;
    /*
     * In present Implementation For Memory Requirements
     * GENLIB Supported Memory Manager is used.
     * Free task memory pool reserved for Message Queue with task_id = GLNULL.
     */
    if(GL_FAILURE == gl_mm_pool_delete(
                                 GL_NULL,
                                 g_a_p_mesg_q[mesg_q_id]->pool_id,
                                 &ecode))
    {
        if(GL_MM_ERR_BUF_ALLOCATED == ecode)
        {
            if(GL_SUCCESS == gl_mm_release_all_task_pool_buffers(
                                 GL_NULL,
                                 g_a_p_mesg_q[mesg_q_id]->pool_id,
                                 &ecode))
            {
                gl_mm_pool_delete(
                                 GL_NULL,
                                 g_a_p_mesg_q[mesg_q_id]->pool_id,
                                 &ecode);
                return;
            }
        }
    }
    return;
}


