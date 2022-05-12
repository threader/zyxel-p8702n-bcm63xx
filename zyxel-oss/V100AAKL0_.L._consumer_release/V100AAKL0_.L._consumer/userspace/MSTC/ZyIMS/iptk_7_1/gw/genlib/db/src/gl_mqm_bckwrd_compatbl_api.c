/*****************************************************************************
*
*    FUNCTION    : Source file for Generic Library - Message Queue Manager
*                  backward compatibility of Rel-1.0 with Rel-0.2 
*                  implementation.
******************************************************************************
*
*    FILENAME    : gl_mqm_bckwrd_compatbl_api.c
*
*    DESCRIPTION : Give defination of functions which requires backward
*                  compatibilty of Rel-1.0 with Rel-0.2.
*
*
* DATE          NAME           REFERENCE          REASON
*----------- --------------  ---------------    ----------------------
*
* 23/04/2002 Ravi Jindal        -               Intial Draft.
* 
*
* COPYRIGHT , Aricent, 2006
*****************************************************************************/


#include "gl_mqm_interfaces_rel_02.h"

     
/*****************************************************************************
* Function name    : gl_mqm_create
* Arguments        : gw_U32bit mesg_qty,
*                    gw_U32bit mesg_size,
*                    gw_U8bit priority_level,
*                    gl_error_t*  p_ecode
* Return Value     : gl_mq_id_t
* Functionality    : Creates the message queue structure and initializes
*                    the write and read pointer and other structure
*                    elements.
* Functions called :
*****************************************************************************/
gl_mq_id_t
gl_mqm_create

#ifdef _ANSI_C_
    ( gw_U32bit                 mesg_qty,
      gw_U32bit                 mesg_size,
      gw_U8bit                  priority_level,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_qty, mesg_size, priority_level, p_ecode )
      gw_U32bit                 mesg_qty;
      gw_U32bit                 mesg_size;
      gw_U8bit                  priority_level;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gl_mq_id_t                  mq_id;
    /*
     * As in Rel-0.2, their is no support for the priority.
     * So the number of priority levels should be 0.
     */ 
    priority_level = 0;

    if(GL_FAILURE == gl_mqm_create_mesg_q(&mq_id, mesg_qty, mesg_size,
             priority_level, p_ecode))
    {
        return GL_ERROR;
    }

    return (mq_id);
}
    



/*****************************************************************************
* Function name    : gl_mqm_send_mesg
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
gl_mqm_send_mesg

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
    /*
     * As in Rel-0.2, their is no support for the priority.
     * So the priority of mesg should be 0.
     */ 
    priority = 0;

    return( gl_mqm_send_mesg_to_mesg_q(mesg_q_id, priority, p_mesg, len, 
                block_flag, p_ecode));
}




/*****************************************************************************
* Function name    : gl_mqm_recv_mesg
* Arguments        : gl_mq_id_t mesg_q_id,
*                    gw_U8bit priority,
*                    gl_pvoid_t* p_p_mesg,
*                    gw_U8bit block_flag,
*                    gl_error_t* p_ecode
* Return Value     : gl_return_t
* Functionality    : Receives the message from the message queue by
*                    deleting the message from the head and incrementing
*                    the read pointer.
* Functions called :
*****************************************************************************/
gw_S32bit
gl_mqm_recv_mesg

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gl_pvoid_t                *p_p_mesg,
      gw_U8bit                  block_flag,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, priority, p_p_mesg, block_flag, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gl_pvoid_t                *p_p_mesg;
      gw_U8bit                  block_flag;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gw_U32bit                   mesg_len;
    gw_U8bit                    actual_priority;

    /*
     * As in Rel-0.2, their is no support for the priority.
     * So the priority of mesg should be 0.
     */ 
    priority = 0;

    if(GL_FAILURE == gl_mqm_recv_mesg_from_mesg_q(mesg_q_id, priority,
                GL_FALSE, &actual_priority, p_p_mesg, &mesg_len, block_flag,
                p_ecode))
    {
        return GL_ERROR;
    }

    return ((gw_S32bit)mesg_len);
}




/*****************************************************************************
* Function name    : gl_mqm_peek_mesg
* Arguments        : gl_mq_id_t mesg_q_id,
*                    gw_U8bit priority,
*                    gl_pvoid_t* p_p_mesg,
*                    gl_error_t* p_ecode
* Return Value     : gl_return_t
* Functionality    : Only reads the message from the message queue
*                    without deleting the message from the head and
*                    without incrementing the read pointer.
* Functions called :
*****************************************************************************/
gw_S32bit
gl_mqm_peek_mesg

#ifdef _ANSI_C_
    ( gl_mq_id_t                mesg_q_id,
      gw_U8bit                  priority,
      gl_pvoid_t                *p_p_mesg,
      gl_error_t                *p_ecode
    )
#else
    ( mesg_q_id, priority, p_p_mesg, p_ecode )
      gl_mq_id_t                mesg_q_id;
      gw_U8bit                  priority;
      gl_pvoid_t                *p_p_mesg;
      gl_error_t                *p_ecode;
#endif

/******************************************************************************/
{
    gw_U32bit                   mesg_len;
    gw_U8bit                    actual_priority;

    /*
     * As in Rel-0.2, their is no support for the priority.
     * So the priority of mesg should be 0.
     */ 
    priority = 0;

    if(GL_FAILURE == gl_mqm_get_first_mesg_from_mesg_q(mesg_q_id, priority,
                GL_FALSE, &actual_priority, p_p_mesg, &mesg_len, p_ecode))
    {
        return GL_ERROR;
    }

    return ((gw_S32bit)mesg_len);
}
    


