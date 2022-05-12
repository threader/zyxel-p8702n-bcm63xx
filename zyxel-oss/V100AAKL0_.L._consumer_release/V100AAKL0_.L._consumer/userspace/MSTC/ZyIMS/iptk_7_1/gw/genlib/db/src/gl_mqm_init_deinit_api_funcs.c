/*******************************************************************************
*
* FUNCTION    : Generic Libraries - Message Queue Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_mqm_init_deinit_api_funcs.c
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
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

#include "gl_mqm.h"


/*****************************************************************************
* Functionality    : gl_mqm_init
* Arguments        : void
* Return Value     : gl_return_t
* Functionality    : Initializes generic libraries mesage queue manager.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_init( void )
/******************************************************************************/
{
    gw_U32bit i;
    gl_error_t ecode;
    if(mqm_mgr_init_flag == GL_FALSE)
    {
        /*
         * Create the Mutex for MT safe.
         */
#ifdef GL_MT_ENABLED
        if(GL_FAILURE == gl_create_mutex (&mqm_mgr_sem, &ecode))
        {
            GL_INC_MQM_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#ifdef GL_STATS_ENABLED
        g_mqm_stat_init = GL_TRUE;

        if(GL_FAILURE == gl_create_mutex(&g_mqm_stat_sem, &ecode))
        {
            GL_INC_MQM_G_MUTX_FAIL()

            return GL_FAILURE;
        }
#endif /* Endif for GL_STATS_ENABLED */
        
#else
        ecode = 0;
#endif

        /*
         * Initialize the message queue global structure.
         */
        for(i=0;i<GL_MQM_MAX_MESG_Q;i++)
        {
            g_a_p_mesg_q[i] = GL_NULL;
        }
        
        /*
         * As in Present Implementation GENLIB Memory Manager is Used
         * for Memory Requirements.
         */ 
        gl_mm_init();
        
        mqm_mgr_init_flag=GL_TRUE;
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}

/*****************************************************************************
* Function name    : gl_mqm_create_mesg_q
* Arguments        : gl_mq_id_t         *p_mesg_q_id,
*                    gw_U32bit          mesg_qty,
*                    gw_U32bit          mesg_size,
*                    gw_U8bit           priority_level,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Creates the message queue structure and initializes
*                    the elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_create_mesg_q

#ifdef _ANSI_C_
    ( gl_mq_id_t                *p_mesg_q_id,
      gw_U32bit                 mesg_qty,
      gw_U32bit                 mesg_size,
      gw_U8bit                  priority_level,
      gl_error_t                *p_ecode
    )
#else
    ( p_mesg_q_id, mesg_qty, mesg_size, priority_level, p_ecode )
      gl_mq_id_t                *p_mesg_q_id;
      gw_U32bit                 mesg_qty;
      gw_U32bit                 mesg_size;
      gw_U8bit                  priority_level;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{

    gw_U16bit local_index;
    gw_U8bit  i;
    gw_S32bit mesg_q_id = 0;

#ifdef GL_STATIC_VALIDATION_ENABLED
    /*
     * Validate the message quantity and the message size.
     */
    if((0 == mesg_qty) || (GL_MQM_MAX_MESG_QTY < mesg_qty ))
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_MESG_QTY;
        return GL_FAILURE;
    }
    if((mesg_size == 0) || (GL_MQM_MAX_MESG_SIZE < mesg_size))
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_MESG_SIZE;
        return GL_FAILURE;
    }
    if((GL_MQM_MAX_PRIORITY_LEVELS < priority_level) || (GL_NULL == 
                priority_level))
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_PRIORITY_LEVEL;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MUTEX_LOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

    /*
     * Search for the non-allocated message queue.
     */
    for(local_index=0; local_index<GL_MQM_MAX_MESG_Q; local_index++)
    {
        if( g_a_p_mesg_q[local_index] == GL_NULL)
        {
            mesg_q_id = local_index;
            break;
        }
    }

    /*
     * In case not available return error.
     */
    if(local_index == GL_MQM_MAX_MESG_Q)
    {
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_MQM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }

    /*
     * Allocate Memory for the new message queue header.
     */
    g_a_p_mesg_q[mesg_q_id] = (gl_mqm_q_t*)SYS_MALLOC(sizeof(gl_mqm_q_t) + 
        priority_level * (sizeof(gl_mqm_priority_level_t)));

    if(GL_NULL == g_a_p_mesg_q[mesg_q_id])
    {
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        GL_INC_MQM_G_MALOC_FAIL()

        return GL_FAILURE; 
    }

#ifdef GL_STATS_ENABLED
    if(GL_FAILURE == gl_mqm_stat_allocte_memory(mesg_q_id, p_ecode))
    {
        GL_INC_MQM_G_MALOC_FAIL()

        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        return GL_FAILURE;
    }
#endif

    *p_mesg_q_id = mesg_q_id;

#ifdef GL_TOTAL_MT_SAFE    
    if(GL_FAILURE == gl_create_mutex(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock),
                p_ecode))
    {
       /* 
        * Create mutex for mutual exclusion between multiple threads
        * reading and writing to the same message queue.
        */
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;

#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        GL_INC_MQM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_create_mutex(&(g_a_p_mesg_q[mesg_q_id]->mqm_write_lock),
                p_ecode))
    {
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;

#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        GL_INC_MQM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    if(GL_FAILURE==gl_create_mutex(&(g_a_p_mesg_q[mesg_q_id]->
        mqm_read_write_lock), p_ecode))
    {
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;
        
#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode, 
                GL_FAILURE);
        
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
        
        GL_INC_MQM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    
#ifdef GL_MQM_BLOCKING_ON 
    if(GL_FAILURE == gl_create_semaphore 
       (&(g_a_p_mesg_q[mesg_q_id]->mqm_queue_full_write_lock),0,1,p_ecode))
    {
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;
        
#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_write_lock),
                p_ecode, GL_FAILURE);

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
        
        GL_INC_MQM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
    if(GL_FAILURE == gl_create_semaphore
       (&(g_a_p_mesg_q [mesg_q_id]->mqm_queue_empty_read_lock),0,1,p_ecode))
    {
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;

#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_write_lock),
                p_ecode, GL_FAILURE);
        
        gl_delete_semaphore (&(g_a_p_mesg_q [mesg_q_id]->
                               mqm_queue_full_write_lock),p_ecode);
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        GL_INC_MQM_G_MUTX_FAIL()

        return GL_FAILURE;
    }
#endif /* Endif for GL_MQM_BLOCKING_ON */

#endif /* End of GL_TOTAL_MT_SAFE */

    g_a_p_mesg_q [mesg_q_id]->a_priority_level = (gl_mqm_priority_level_t *) 
         ((gw_U8bit *)g_a_p_mesg_q [mesg_q_id] + sizeof(gl_mqm_q_t));
    
    if( GL_FAILURE == mqm_allocate_memory(mesg_q_id, mesg_qty, 
                      sizeof(gl_mqm_mesg_t), p_ecode))
    {
        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q[mesg_q_id] = GL_NULL;

#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_write_lock), 
                p_ecode, GL_FAILURE);
        
#ifdef GL_TOTAL_MT_SAFE    
#ifdef GL_MQM_BLOCKING_ON 
        gl_delete_semaphore (&(g_a_p_mesg_q [mesg_q_id]->
                               mqm_queue_full_write_lock),p_ecode);
        gl_delete_semaphore (&(g_a_p_mesg_q [mesg_q_id]->
                               mqm_queue_empty_read_lock),p_ecode);
#endif /* Endif for GL_MQM_BLOCKING_ON */
#endif /* End of GL_TOTAL_MT_SAFE */
        
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

        *p_ecode = GL_MQM_ERR_NO_RESOURCE;
        return GL_FAILURE;
    }
    GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

    /*
     * The number of message at a particular priority level can be 
     * between 0 - mesg_qty.
     * Sum of all the message at all priority level <= mesg_qty.
     * 0 is the highest priority.
     */ 
    /*
     * Set the Frist and Last Message Pointer to NULL for all priroity levels.
     */
    for(i=0; i < priority_level; i++)
    {
        g_a_p_mesg_q [mesg_q_id]->a_priority_level[i].p_first_mesg = GL_NULL;
        g_a_p_mesg_q [mesg_q_id]->a_priority_level[i].p_last_mesg = GL_NULL;
    }
    g_a_p_mesg_q [mesg_q_id]->max_mesg_qty = mesg_qty;
    g_a_p_mesg_q [mesg_q_id]->curr_mesg_qty = GL_NULL;
    g_a_p_mesg_q [mesg_q_id]->max_mesg_size= mesg_size;
    g_a_p_mesg_q [mesg_q_id]->num_of_priority_levels = priority_level;
    g_a_p_mesg_q [mesg_q_id]->is_blocked_flag = GL_MQM_NOT_BLOCKED;
    
    GL_MQM_C_CURR_MSG_QTY(mesg_q_id, 0)
    GL_MQM_C_TOTAL_MSG_QTY(mesg_q_id, mesg_qty)
    
    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_mqm_delete
* Arguments        : gl_mq_id_t         mesg_q_id,
*                    gl_error_t         *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes the message queue structure and resets the
*                    other structure elements.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_delete_mesg_q

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
#ifdef GL_STATIC_VALIDATION_ENABLED
    if( mesg_q_id >= GL_MQM_MAX_MESG_Q
        || g_a_p_mesg_q[mesg_q_id] == GL_NULL)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode =GL_MQM_ERR_INV_MESG_Q_ID;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */
    /*
     * If message queue contains some messages return error.
     */
    if(0 != g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty)
    {
        GL_INC_MQM_G_DEL_RQ_QUEUE_BSY()
            
        *p_ecode =GL_MQM_ERR_MESG_Q_NOT_EMPTY;
        return GL_FAILURE;
    }

    GL_MUTEX_LOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

    /*Free the pool memory for Message queue*/
    mqm_free_memory(mesg_q_id);

    /*
     * Delete all the existing MUTEX Locks 
     * for the sprcified Message Queue.
     */
    GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
            GL_FAILURE);
    GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode,
            GL_FAILURE);
    GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_write_lock), p_ecode,
            GL_FAILURE);

#ifdef GL_TOTAL_MT_SAFE
#ifdef GL_MQM_BLOCKING_ON 
    if(GL_FAILURE == (gl_delete_semaphore 
      (&(g_a_p_mesg_q [mesg_q_id]->mqm_queue_full_write_lock), p_ecode)))
    {
        GL_INC_MQM_G_MUTX_FAIL()

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
        return GL_FAILURE;
    }
    if(GL_FAILURE == (gl_delete_semaphore 
      (&(g_a_p_mesg_q [mesg_q_id]->mqm_queue_empty_read_lock), p_ecode)))
    {
        GL_INC_MQM_G_MUTX_FAIL()

        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
        return GL_FAILURE;
    }
#endif
#endif

#ifdef GL_STATS_ENABLED
    gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

    /*Reset the message queue Pointer*/
    SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
    g_a_p_mesg_q[mesg_q_id] = GL_NULL;

    GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);

    return GL_SUCCESS;
}



/*****************************************************************************
* Function name    : gl_mqm_destroy_all_queues
* Arguments        : gw_U8bit is_forceful_cleanup
*                    gl_error_t *p_ecode
* Return Value     : gl_return_t
* Functionality    : Deletes all the message-queues and free all the
*                    corresponding occupied resources.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_destroy_all_queues

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
    gw_U16bit           mesg_q_id= GL_NULL;
    gw_boolean          is_not_empty_flag = GL_FALSE; 

    GL_MUTEX_LOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
    for(mesg_q_id = 0; mesg_q_id < GL_MQM_MAX_MESG_Q; mesg_q_id++)
    {
        if( g_a_p_mesg_q[mesg_q_id] == GL_NULL)
        {
            continue;
        }                 
        if((is_forceful_cleanup == GL_FALSE) &&
           (0 != g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty))
        {
            is_not_empty_flag = GL_TRUE;
            continue;
        }
        /*
         * Reset the message queue parameters
         */
        g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty = 0;
        mqm_free_memory(mesg_q_id);

        /*
         * Delete the MUTEX occupied by message queue.
         */
        
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_write_lock), p_ecode,
                GL_FAILURE);
        GL_MT_DELETE(&(g_a_p_mesg_q [mesg_q_id]->mqm_read_write_lock),
                p_ecode, GL_FAILURE);
        
#ifdef GL_TOTAL_MT_SAFE
#ifdef GL_MQM_BLOCKING_ON 
        /*
         * Delete the SEMAPHORES occupied by message queue.
         */
        if(GL_FAILURE == (gl_delete_semaphore 
          (&(g_a_p_mesg_q [mesg_q_id]->mqm_queue_full_write_lock), p_ecode)))
        {
            GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
            return GL_FAILURE;
        }
        if(GL_FAILURE == (gl_delete_semaphore 
          (&(g_a_p_mesg_q [mesg_q_id]->mqm_queue_empty_read_lock), p_ecode)))
        {
            GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
            return GL_FAILURE;
        }
#endif
#endif

#ifdef GL_STATS_ENABLED
        gl_mqm_stat_free_memory(mesg_q_id, p_ecode);
#endif /* Endif for GL_STATS_ENABLED */

        SYS_FREE(g_a_p_mesg_q[mesg_q_id]);
        g_a_p_mesg_q [mesg_q_id] = GL_NULL;

    }/*end of for loop*/
        
    if(is_not_empty_flag == GL_TRUE)
    {
        /* some queues are not empty and no forcefull destroy */
        GL_MUTEX_UNLOCK(&mqm_mgr_sem, p_ecode, GL_FAILURE);
    
        *p_ecode = GL_MQM_ERR_MESG_Q_NOT_EMPTY;
        return GL_FAILURE;
    }
    else
    {
        GL_MUTEX_DELETE(&mqm_mgr_sem, p_ecode, GL_FAILURE);
#ifdef GL_STATS_ENABLED
#ifdef GL_MT_ENABLED
        if(GL_TRUE == g_mqm_stat_init)
        {
            gl_delete_mutex(&g_mqm_stat_sem, p_ecode);
        }
        g_mqm_stat_init = GL_FALSE;
#endif
#endif /* Endif for GL_STATS_ENABLED */
        mqm_mgr_init_flag = GL_FALSE;
        return GL_SUCCESS;
    }
}


