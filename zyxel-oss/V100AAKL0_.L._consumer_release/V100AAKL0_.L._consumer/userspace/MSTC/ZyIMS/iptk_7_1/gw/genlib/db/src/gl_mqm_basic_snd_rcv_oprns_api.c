/*******************************************************************************
*
* FUNCTION    : Generic Libraries - Message Queue Manager implementation source.
*
********************************************************************************
*
* FILENAME    : gl_mqm_basic_snd_rcv_oprns_api.c
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
* 21/10/2002 Ravi Jindal    SPR-4980          To ignore the return value check
*                                             of release semaphore.
*                                             
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

#include "gl_mqm.h"


/*****************************************************************************
* Function name    : gl_mqm_send_mesg_to_mesg_q
* Arguments        : gl_mq_id_t mesg_q_id,
*                    gw_U8bit priority,
*                    gl_pvoid_t p_mesg,
*                    gw_U32bit len,
*                    gw_U8bit block_flag,
*                    gl_error_t* p_ecode
* Return Value     : gl_return_t
* Functionality    : Sends the message to the message queue by adding the
*                    message at the tail and incrementing the write pointer.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_send_mesg_to_mesg_q

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gl_pvoid_t                p_mesg,
      gw_U32bit                 len,
      gw_U8bit                  block_flag,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, priority, p_mesg, len, block_flag, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gl_pvoid_t                p_mesg;
      gw_U32bit                 len;
      gw_U8bit                  block_flag;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_mqm_priority_level_t *p_req_priority_level;
    gl_mqm_mesg_t           *p_mesg_node;
    gl_mqm_mesg_t           *p_roll_back_mesg_node = GL_NULL;
    gl_return_t             ret_val = GL_SUCCESS;
    gw_boolean              run_loop = GL_TRUE;

    GL_INC_MQM_G_SEND_MSG_RQ()

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( mesg_q_id >= GL_MQM_MAX_MESG_Q
         || g_a_p_mesg_q[mesg_q_id] == GL_NULL)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

       *p_ecode = GL_MQM_ERR_INV_MESG_Q_ID;
       return GL_FAILURE;
    }
    if((0 == len) || (GL_MQM_MAX_MESG_SIZE < len))
    {
        /* 
         * Validate length of the message.
         */
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_MESG_LEN;
        return GL_FAILURE;
    }
    if(priority >= g_a_p_mesg_q[mesg_q_id]->num_of_priority_levels)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_PRIORITY_LEVEL;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Write Threads.
     */ 
    GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_write_lock), p_ecode, GL_FAILURE);
    
    while(GL_TRUE == run_loop)
    {
        GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode, 
                GL_FAILURE);
    
        if(g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty < g_a_p_mesg_q[mesg_q_id]->
                max_mesg_qty)
        {
            /* 
             * This IF condition checks whether the message queue is full.
             * This condition becomes true, if Message Queue is not Full. 
             */
            p_req_priority_level = g_a_p_mesg_q[mesg_q_id]->a_priority_level + 
                priority;
        
            p_mesg_node =(gl_mqm_mesg_t *)mqm_get_memory(mesg_q_id, p_ecode);

            if(GL_NULL == p_mesg_node)
            {
                GL_INC_MQM_G_MALOC_FAIL()
                    
                ret_val = GL_FAILURE;
                break;
            }

            p_mesg_node->p_mesg = p_mesg;
            p_mesg_node->len = len;
            p_mesg_node->p_next = GL_NULL;
            
            g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty += 1;
                
            if(GL_NULL == p_req_priority_level->p_first_mesg)
            {
                p_req_priority_level->p_first_mesg = 
                    p_req_priority_level->p_last_mesg = p_mesg_node;
            }
            else if(GL_NULL != p_req_priority_level->p_last_mesg)
            {
                /*
                 * Store the value of p_req_priority_level->p_last_mesg
                 * to use in Roll-back in case of Semaphore call Failure.
                 */ 
                p_roll_back_mesg_node = p_req_priority_level->p_last_mesg;
                
                p_req_priority_level->p_last_mesg->p_next = p_mesg_node;
                p_req_priority_level->p_last_mesg = p_mesg_node;
            }

#ifdef GL_TOTAL_MT_SAFE            
#ifdef GL_MQM_BLOCKING_ON 
            if(GL_MQM_BLOCKED_ON_READ & g_a_p_mesg_q[mesg_q_id]->
                    is_blocked_flag)
            {
                gl_release_semaphore(&(g_a_p_mesg_q
                    [mesg_q_id]->mqm_queue_empty_read_lock), p_ecode);

                /*
                 *if(GL_FAILURE == (gl_release_semaphore(&(g_a_p_mesg_q
                 *   [mesg_q_id]->mqm_queue_empty_read_lock), p_ecode)))
                 *{
                 *   GL_INC_MQM_G_MUTX_FAIL()
                 *       
                 *    *
                 *    * As Release Semaphore Fails,
                 *    * So Roll back the operations done for Send Mesg.
                 *    * 
                 *   if( 1 == g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty)
                 *   {
                 *       p_req_priority_level->p_first_mesg = 
                 *           p_req_priority_level->p_last_mesg = GL_NULL;
                 *   }
                 *   else
                 *   {
                 *     p_req_priority_level->p_last_mesg = p_roll_back_mesg_node;
                 *     p_req_priority_level->p_last_mesg->p_next = GL_NULL;
                 *   }
                 *   g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty -= 1;
                 *   
                 *   
                 *   if(GL_FAILURE == mqm_release_memory(mesg_q_id, p_mesg_node,
                 *           p_ecode ))
                 *   {
                 *       ret_val = GL_FAILURE;
                 *       break;
                 *   }
                 *   ret_val = GL_FAILURE;
                 *   break;
                }*/
            }
#endif
#endif
            ret_val = GL_SUCCESS;
            run_loop = GL_FALSE;
        }
#ifdef GL_TOTAL_MT_SAFE            
#ifdef GL_MQM_BLOCKING_ON 
        else if(GL_TRUE == block_flag)
        {
            g_a_p_mesg_q[mesg_q_id]->is_blocked_flag = (gw_U8bit)
                (g_a_p_mesg_q[mesg_q_id]->is_blocked_flag | 
                 GL_MQM_BLOCKED_ON_WRITE);
        
            GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock),
                    p_ecode, GL_FAILURE);
            /*
             * If block flag is set and message queue is full.
             * Keep waiting on this Semaphore Lock until this task
             * gets some free space to write into the queue.
             */
            if(GL_FAILURE == (gl_wait_on_semaphore
              (&(g_a_p_mesg_q[mesg_q_id]->mqm_queue_full_write_lock), p_ecode)))
            {
                GL_INC_MQM_G_MUTX_FAIL()
                    
                /*
                 * As Wait on Semaohore is failed, 
                 * So Roll back and return Failure.
                 */ 
                GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock),
                        p_ecode, GL_FAILURE);
    
                g_a_p_mesg_q[mesg_q_id]->is_blocked_flag = (gw_U8bit)
                    (g_a_p_mesg_q[mesg_q_id]->is_blocked_flag &! 
                    (GL_MQM_BLOCKED_ON_WRITE));
                
                ret_val = GL_FAILURE;
                break;
            }
            g_a_p_mesg_q[mesg_q_id]->is_blocked_flag =  (gw_U8bit)
             (g_a_p_mesg_q[mesg_q_id]->is_blocked_flag & !
             (GL_MQM_BLOCKED_ON_WRITE));
        }
#endif
#endif
        else
        {
            /*
             * To Remove Warning.
             */ 
            block_flag = block_flag;
            
            GL_INC_MQM_C_SEND_MSG_FAIL(mesg_q_id)
            ret_val = GL_FAILURE;
            run_loop = GL_FALSE;
        }
    }

    GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode,
            GL_FAILURE);
    GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_write_lock), p_ecode, 
            GL_FAILURE);
    
    GL_MQM_C_PEAK_MSG_REACH(mesg_q_id, g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty) 
    GL_MQM_C_CURR_MSG_QTY(mesg_q_id, g_a_p_mesg_q[mesg_q_id]-> curr_mesg_qty) 
    
    return ret_val;
    
}



/*****************************************************************************
* Function name    : gl_mqm_recv_mesg_from_mesg_q
* Arguments        : gl_mq_id_t mesg_q_id,
*                    gw_U8bit   priority,
*                    gw_U8bit   is_absolute_priority,
*                    gw_U8bit   *p_actual_priority,
*                    gl_pvoid_t *p_p_mesg,
*                    gw_U32bit  *p_mesg_len,
*                    gw_U8bit   block_flag,
*                    gl_error_t *p_ecode
* Return Value     : gl_return_t
* Functionality    : Receives the message from the message queue by
*                    deleting the message from the head and decrementing
*                    the number of messages in Message Queue.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_recv_mesg_from_mesg_q

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gw_U8bit                  is_absolute_priority,
      gw_U8bit                  *p_actual_priority,
      gl_pvoid_t                *p_p_mesg,
      gw_U32bit                 *p_mesg_len,
      gw_U8bit                  block_flag,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, priority, is_absolute_priority, p_actual_priority, p_p_mesg, 
      p_mesg_len, block_flag, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gw_U8bit                  is_absolute_priority;
      gw_U8bit                  *p_actual_priority;
      gl_pvoid_t                *p_p_mesg;
      gw_U32bit                 *p_mesg_len;
      gw_U8bit                  block_flag;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_mqm_priority_level_t *p_req_priority_level;
    gl_mqm_mesg_t           *p_temp_mesg_node;
    gl_return_t             ret_val = GL_SUCCESS;

    GL_INC_MQM_G_RECV_MSG_RQ()

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( mesg_q_id >= GL_MQM_MAX_MESG_Q
         || g_a_p_mesg_q[mesg_q_id] == GL_NULL)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

       *p_ecode = GL_MQM_ERR_INV_MESG_Q_ID;
       return GL_FAILURE;
    }
    if(GL_NULL == p_p_mesg)
    {
        /* 
         * Validate the message pointer.
         */
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_MESG_PTR;
        return GL_FAILURE;
    }
    if( priority >=g_a_p_mesg_q[mesg_q_id]->num_of_priority_levels)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_PRIORITY_LEVEL;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    /*
     * To Shynchronize processing of Different Write Threads.
     */ 
    GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_lock), p_ecode, GL_FAILURE);
    
    while(1)
    {
        GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode,
                GL_FAILURE);
    
        if( 0 != g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty)
        {
            if((GL_TRUE == is_absolute_priority) &&
               (GL_SUCCESS == mqm_is_abs_priority_mesg(mesg_q_id, priority)))   
            {
                *p_actual_priority = priority;
                ret_val = GL_SUCCESS;
            }
            else if((GL_FALSE == is_absolute_priority) &&
                    (GL_SUCCESS == mqm_is_relative_priority_mesg(mesg_q_id, 
                        priority, p_actual_priority)))
            {
                ret_val = GL_SUCCESS;
            }
            else
            {
                ret_val = GL_FAILURE;
            }
            
            if(GL_SUCCESS == ret_val)
            {
                p_req_priority_level = (gl_mqm_priority_level_t*)(g_a_p_mesg_q
                        [mesg_q_id]->a_priority_level) + *p_actual_priority;
                
                *p_p_mesg = p_req_priority_level->p_first_mesg->p_mesg;
                *p_mesg_len = p_req_priority_level->p_first_mesg->len;
                
                p_temp_mesg_node = p_req_priority_level->p_first_mesg;
                
                p_req_priority_level->p_first_mesg = 
                    p_req_priority_level->p_first_mesg->p_next;

                p_temp_mesg_node->p_next = GL_NULL;
                
                g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty -= 1;
                if(GL_FAILURE == mqm_release_memory(mesg_q_id, p_temp_mesg_node,
                            p_ecode ))
                {
                    ret_val = GL_FAILURE;
                    break;
                }
                
#ifdef GL_TOTAL_MT_SAFE            
#ifdef GL_MQM_BLOCKING_ON 
               if(GL_MQM_BLOCKED_ON_WRITE & g_a_p_mesg_q[mesg_q_id]->
                       is_blocked_flag)
               {
                    gl_release_semaphore(&(g_a_p_mesg_q[mesg_q_id]->
                                mqm_queue_full_write_lock), p_ecode);

                    /*    
                     *if(GL_FAILURE == (gl_release_semaphore
                     * (&(g_a_p_mesg_q[mesg_q_id]->mqm_queue_full_write_lock),
                     *  p_ecode)))
                     *{
                     *    *
                     *    * As Release Semaphore Fails,
                     *    * So Roll back the action done for Recv Mesg.
                     *    *
                     *   p_temp_mesg_node =(gl_mqm_mesg_t *)mqm_get_memory(
                     *           mesg_q_id, p_ecode);
                     *
                     *   if(GL_NULL == p_temp_mesg_node)
                     *   {
                     *       ret_val = GL_FAILURE;
                     *       break;
                     *   }
                     *   p_temp_mesg_node->p_mesg = *p_p_mesg;
                     *   p_temp_mesg_node->len = *p_mesg_len;
                     *   p_temp_mesg_node->p_next = 
                     *       p_req_priority_level->p_first_mesg;
                     *   p_req_priority_level->p_first_mesg = p_temp_mesg_node;
                     *   ret_val = GL_FAILURE;
                     *   break;
                    }*/
                }
#endif
#endif
                break;
            }
        }
#ifdef GL_TOTAL_MT_SAFE
#ifdef GL_MQM_BLOCKING_ON 
        if(GL_TRUE == block_flag)
        {

            if(GL_MQM_MAX_MESG_QTY == g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty)
            {
                /*
                 * As the message-queue is full, so some thread may
                 * get blocked on Write operation and current thread
                 * is already blocked on read operation, as no 
                 * message of requested priority is avialable.
                 * So, to resolve deadlock condition as this read 
                 * can not be sataisfied, return Failure to this read operation.
                 */
                *p_ecode = GL_MQM_ERR_DEADLOCK_ON_READ_WRITE;
                ret_val = GL_FAILURE;
                break;
            }
        
            g_a_p_mesg_q[mesg_q_id]->is_blocked_flag = (gw_U8bit)(g_a_p_mesg_q
                    [mesg_q_id]->is_blocked_flag | GL_MQM_BLOCKED_ON_READ);

            GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock),
                    p_ecode, GL_FAILURE);

            /*
             * If block flag is set and no message at requested Priority.
             * Keep waiting on this Semaphore Lock until this task
             * gets a indication of new message in Message Queue.
             */
            if(GL_FAILURE == (gl_wait_on_semaphore
              (&(g_a_p_mesg_q[mesg_q_id]->mqm_queue_empty_read_lock), p_ecode)))
            {
                /*
                 * As Wait on Semaphore Fails,
                 * So Roll back the Return Failure.
                 */
                GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock),
                    p_ecode, GL_FAILURE);

                g_a_p_mesg_q[mesg_q_id]->is_blocked_flag = (gw_U8bit)
                    (g_a_p_mesg_q[mesg_q_id]->is_blocked_flag & 
                     !(GL_MQM_BLOCKED_ON_READ));
                ret_val = GL_FAILURE;
                break;
            }
            g_a_p_mesg_q[mesg_q_id]->is_blocked_flag = (gw_U8bit) 
             (g_a_p_mesg_q[mesg_q_id]->is_blocked_flag & 
              !(GL_MQM_BLOCKED_ON_READ));
        }
#endif
#endif
        else
        {
            /*
             * To Remove Warning.
             */ 
            block_flag = block_flag;

            GL_INC_MQM_C_RECV_MSG_FAIL(mesg_q_id)
            *p_ecode = GL_MQM_ERR_NO_MESG_AT_REQ_PRIORITY_LEVEL;
            ret_val = GL_FAILURE;
            break;
        }
    }
    
    GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode,
            GL_FAILURE);
    GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_lock), p_ecode, 
            GL_FAILURE);

    GL_MQM_C_CURR_MSG_QTY(mesg_q_id, g_a_p_mesg_q[mesg_q_id]-> curr_mesg_qty) 

    return ret_val;
}
    
/*****************************************************************************
* Function name    : gl_mqm_get_first_mesg_from_mesg_q
* Arguments        : gl_mq_id_t mesg_q_id,
*                    gw_U8bit   priority,
*                    gw_U8bit   is_absolute_priority,
*                    gw_U8bit   *p_actual_priority,
*                    gl_pvoid_t *p_p_mesg,
*                    gl_U32bit  *p_mesg_len,
*                    gl_error_t *p_ecode
* Return Value     : gl_return_t
* Functionality    : Only reads the message from the message queue
*                    without deleting the message from the head and
*                    without decrementing number of messages in Queue.
* Functions called :
*****************************************************************************/
gl_return_t
gl_mqm_get_first_mesg_from_mesg_q

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gw_U8bit                  is_absolute_priority,
      gw_U8bit                  *p_actual_priority,
      gl_pvoid_t                *p_p_mesg,
      gw_U32bit                 *p_mesg_len,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, priority, is_absolute_priority, p_actual_priority, 
      p_p_mesg, p_mesg_len, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gw_U8bit                  is_absolute_priority;
      gw_U8bit                  *p_actual_priority;
      gl_pvoid_t                *p_p_mesg;
      gw_U32bit                 *p_mesg_len;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_mqm_priority_level_t *p_req_priority_level;
    gl_return_t             ret_val;

#ifdef GL_STATIC_VALIDATION_ENABLED
    if( mesg_q_id >= GL_MQM_MAX_MESG_Q
        || g_a_p_mesg_q[mesg_q_id] == GL_NULL)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode =GL_MQM_ERR_INV_MESG_Q_ID;
        return GL_FAILURE;
    }
    if(GL_NULL == p_p_mesg)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        /*
         * Validate the message pointer.
         */
        *p_ecode = GL_MQM_ERR_INV_MESG_PTR;
        return GL_FAILURE;
    }
    if( priority >=g_a_p_mesg_q[mesg_q_id]->num_of_priority_levels)
    {
        GL_INC_MQM_G_PARM_VLD_FAIL()

        *p_ecode = GL_MQM_ERR_INV_PRIORITY_LEVEL;
        return GL_FAILURE;
    }
#endif /* Endif for GL_STATIC_VALIDATION_ENABLED */

    GL_MT_LOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode,
            GL_FAILURE);
    
    if( 0 != g_a_p_mesg_q[mesg_q_id]->curr_mesg_qty)
    {
        if((GL_TRUE == is_absolute_priority) &&
           (GL_SUCCESS == mqm_is_abs_priority_mesg(mesg_q_id, priority)))
        {
            *p_actual_priority = priority;
            ret_val = GL_SUCCESS;
        }
        else if((GL_FALSE == is_absolute_priority) &&
                (GL_SUCCESS == mqm_is_relative_priority_mesg(mesg_q_id,
                    priority, p_actual_priority)))
        {
            ret_val = GL_SUCCESS;
        }
        else
        {
            *p_ecode = GL_MQM_ERR_NO_MESG_AT_REQ_PRIORITY_LEVEL;
            ret_val = GL_FAILURE;
        }
        if(GL_SUCCESS == ret_val)
        {
            /*
             * Only read the message at head of requested Priority Level.
             * Do noyt delete the message from the Message Queue.
             */ 
            p_req_priority_level = g_a_p_mesg_q[mesg_q_id]->a_priority_level +
                *p_actual_priority;

            *p_p_mesg = p_req_priority_level->p_first_mesg->p_mesg;
            *p_mesg_len = p_req_priority_level->p_first_mesg->len;
        }
    }
    else
    {
        *p_ecode = GL_MQM_ERR_MESG_Q_EMPTY;
        ret_val = GL_FAILURE;
    }
    
    GL_MT_UNLOCK(&(g_a_p_mesg_q[mesg_q_id]->mqm_read_write_lock), p_ecode,
            GL_FAILURE);
    
    return (ret_val);
}

