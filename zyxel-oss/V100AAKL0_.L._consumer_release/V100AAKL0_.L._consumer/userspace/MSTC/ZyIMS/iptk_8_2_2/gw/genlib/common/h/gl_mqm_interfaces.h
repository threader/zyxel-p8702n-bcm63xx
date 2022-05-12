/*****************************************************************************
*
*    FUNCTION    : Header file for Generic Library - Message Queue Manager 
*                  Interface level prototype and data structure declarations.  
******************************************************************************
*
*    FILENAME    : gl_mqm_interfaces.h
*
*    DESCRIPTION : This file contains the interface level prototype and 
*                  data structure declarations of message queue manager sub-module 
*                  of the generic library implementation. 
*                  This file is to be include in the application module, in
*                  order to use the supported APIs.
*
*   DATE            NAME        REFERENCE           REASON
*-----------   -------------  ---------------  ----------------------
*
* 29/11/2001 Ravi Jindal        SPR-2788        Packaging of Genlib-Code.
* 
* 06/12/2001 Prashant Jain      SPR-3363        Compilation with CC.
*
* 20/03/2002 Ravi Jindal        SPR-3956        Enhancements for Rel-1.0.
*
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#ifndef _GL_MQM_INTERFACES_H_
#define _GL_MQM_INTERFACES_H_

#include "gl_config.h"
#include "gl_types.h"
#include "gl_error.h"
#include "gl_os_port.h"
#include "gl_gscim_interfaces.h"
#include "gl_ulm_interfaces.h"

#include "gw_pragma_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * EXTERNAL INTERFACE APIs - MESSAGE QUEUES.
 */
/*
 * The functions include those for creation of message queue and operations
 * such as sending, receiving and peeking messages.
 */

/*
 * Initialise the generic libraries message queue manager.
 */
extern gl_return_t
gl_mqm_init( void );


extern gl_return_t
gl_mqm_create_mesg_q
    
    _ARGS_( (   gl_mq_id_t              *p_mesg_q_id,
                gw_U32bit               mesg_qty, 
                gw_U32bit               mesg_size, 
                gw_U8bit                priority_level, 
                gl_error_t              *p_ecode     ) );



extern gl_return_t
gl_mqm_delete_mesg_q
    
    _ARGS_( (   gl_mq_id_t              mesg_q_id, 
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_mqm_send_mesg_to_mesg_q
    
    _ARGS_( (   gl_mq_id_t              mesg_q_id,
                gw_U8bit                priority, 
                gl_pvoid_t              p_mesg,
                gw_U32bit               len, 
                gw_U8bit                block_flag,
                gl_error_t              *p_ecode    ) );



extern gl_return_t
gl_mqm_recv_mesg_from_mesg_q
    
    _ARGS_( (   gl_mq_id_t              mesg_q_id,
                gw_U8bit                priority,
                gw_U8bit                is_absolute_priority,
                gw_U8bit                *p_actual_priority,
                gl_pvoid_t              *p_p_mesg, 
                gw_U32bit               *p_mesg_len,
                gw_U8bit                block_flag,
                gl_error_t              *p_ecode     ) );



extern gl_return_t
gl_mqm_get_first_mesg_from_mesg_q
    
    _ARGS_( (   gl_mq_id_t              mesg_q_id,
                gw_U8bit                priority,
                gw_U8bit                is_absolute_priority,
                gw_U8bit                *p_actual_priority,
                gl_pvoid_t              *p_p_mesg,
                gw_U32bit               *p_mesg_len,
                gl_error_t              *p_ecode     ) );



extern gl_return_t 
gl_mqm_destroy_all_queues

    _ARGS_( (   gw_U8bit                is_forceful_cleanup,
                gl_error_t              *p_ecode    ) );

#ifdef __cplusplus
}
#endif

#include "gw_pragma_undefs.h"

#endif
