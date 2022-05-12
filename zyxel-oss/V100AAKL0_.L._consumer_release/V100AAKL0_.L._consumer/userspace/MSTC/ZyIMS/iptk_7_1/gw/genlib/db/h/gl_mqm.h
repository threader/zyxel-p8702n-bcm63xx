/*******************************************************************************
*
* FUNCTION    : Generic Libraries - Message Queue Manager data types.
*
********************************************************************************
*
* FILENAME    : gl_mqm.h
*
* DESCRIPTION : This file contains the data type declarations of message queue
*               manager in the generic library implementation.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* Oct'99     Kapil Nayar    HSS:40600026      Generic Libraries
*                           HSS:40600036      (Gateways)
* 21/01/2001 Sachin Mittal                    Reorganised and moved in common 
* 18/10/2001 Ravi Jindal    SPR-2788          Packaging of Genlib-Code.
* 29/10/2001 Ravi Jindal    SPR-2798          Change the counting semaphore to 
*                                             binary semaphore and add 
*                                             read_write mutex to access both
*                                             read and write pointer to check
*                                             queue is empty or full.
* 17/11/2001 Ravi Jindal    SPR-3107          Incoprate the defects and change
*                                             cosmmetics of file.
* 30/11/2001 Ravi Jindal    SPR-2788          Re-Packaging of Genlib-Code.
* 18/12/2001 Ravi Jindal    SPR-3363          Compilation with CC.
*
* 20/03/2002 Ravi Jindal    SPR-3956          Enhancements for Rel-1.0.
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/* 
 * Check to see if this file has been included already.
 */
#ifndef _GL_MQM_H_
#define _GL_MQM_H_

#include "gl_mqm_interfaces.h"
#include "gl_ulm_stats_intrn_types.h"
#include "gl_mm_interfaces.h"
#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Structure for the message in the message queue
 */
typedef struct gl_mqm_mesg_s
{
    /* 
     * Pointer to the message. 
     */
    gl_pvoid_t                  p_mesg;
    /* 
     * Length of the message. 
     */
    gw_U32bit                   len; 
    /*
     * Pointer to next message in Message Queue at same Priority level.
     */ 
    struct gl_mqm_mesg_s        *p_next;
    
}gl_mqm_mesg_t;

/*
 * Enum to indicate whether thread is blocked on read or write.
 */
typedef enum
{
    GL_MQM_NOT_BLOCKED = 0,
    GL_MQM_BLOCKED_ON_READ,
    GL_MQM_BLOCKED_ON_WRITE
        
}gl_mqm_blocked_state_et;

/*
 * Structure for the Priority level supported for Message Queue.
 */
typedef struct gl_mqm_priority_level_s
{
    /*
     * Pointer to first message at the priority level.
     */ 
    gl_mqm_mesg_t               *p_first_mesg;
    /*
     * Pointer to last message at the priority level.
     */ 
    gl_mqm_mesg_t               *p_last_mesg;
    
}gl_mqm_priority_level_t;

/*
 * Message queue structure.
 */
typedef struct gl_mqm_q_s
{
    /*
     * Number of Priority level Supported for the Message Queue.
     */ 
    gw_U8bit                    num_of_priority_levels;
    /* 
     * Pointer to the Priority levels supported for the Message Queue.
     */
    gl_mqm_priority_level_t     *a_priority_level;
    /*
     * Maximum number of messages in message queue.
     */
    gw_U32bit                   max_mesg_qty;  
    /*
     * Number of messages currently in message queue.
     */
    gw_U32bit                   curr_mesg_qty;  
    /*
     * Maximum size of any message in message queue.
     */
    gw_U32bit                   max_mesg_size;  
    /*
     * This flag is set, when a thread is blocked on read or write operation.
     * the valid values are from enum gl_mqm_blocked_state_et.
     */ 
    gw_U8bit                    is_blocked_flag;

#ifdef GL_TOTAL_MT_SAFE
    /*
     * Mutex required to be secured before receving mesg from 
     * the message queue.
     */
    GL_MUTEX_ID                 mqm_read_lock;
    /*
     * Mutex required to be secured before sending to the message queue.
     */
    GL_MUTEX_ID                 mqm_write_lock; 
    /*
     * Mutex required to be secured before checking the condition
     * that whether the Message Queue is empty or full (Since it 
     * involves the access to data feilds of Message Queue).
     */
    GL_MUTEX_ID                 mqm_read_write_lock; 

#ifdef GL_MQM_BLOCKING_ON 
    GL_SEMAPHORE_ID             mqm_queue_full_write_lock;
    GL_SEMAPHORE_ID             mqm_queue_empty_read_lock;
#endif
    
#endif/* Endif for GL_TOTAL_MT_SAFE */ 
    /* 
     * This Parameter depends on Memory Management, 
     * Used for Message Queue, As in Present Implementation 
     * GENLIB Memory Manager is Used for Memory Requirements.
     */
    gl_pool_id_t                pool_id; 
    
}gl_mqm_q_t;



/**********************************************
 * Extern Declaration for Array of Message Queus.
 *********************************************/


/*
 * Array of message queue structures where the
 * message queues are added at runtime
 */
extern gl_mqm_q_t             *g_a_p_mesg_q[GL_MQM_MAX_MESG_Q];

extern gl_boolean             mqm_mgr_init_flag;

#ifdef GL_MT_ENABLED
    extern GL_MUTEX_ID        mqm_mgr_sem;
#endif



/**********************************************
 * LOCAL FUNCTIONS DECLARATION.
 *********************************************/

extern gl_return_t
mqm_is_abs_priority_mesg

    _ARGS_( (   gl_mq_id_t      mesg_q_id,
                gw_U8bit        priority    ) );
            

    
extern gl_return_t
mqm_is_relative_priority_mesg

    _ARGS_( (   gl_mq_id_t      mesg_q_id,
                gw_U8bit        priority,
                gw_U8bit        *p_actual_priority    ) );
    
    

extern gl_return_t 
mqm_allocate_memory

    _ARGS_( (   gl_mq_id_t      mesg_q_id, 
                gw_U32bit       buf_qty,
                gw_U32bit       buf_size,
                gl_error_t      *p_ecode    ) );


    
extern gl_pvoid_t
mqm_get_memory

    _ARGS_( (   gl_mq_id_t      mesg_q_id,
                gl_error_t      *p_ecode      ) );
    

    
extern gl_return_t
mqm_release_memory

    _ARGS_( (   gl_mq_id_t      mesg_q_id,
                gl_pvoid_t      buf_ptr,
                gl_error_t      *p_ecode      ) );
    
    
extern void
mqm_free_memory

    _ARGS_( (   gl_mq_id_t      mesg_q_id   ) );


    
#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
