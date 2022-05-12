/***************************************************************************
*
*    FUNCTION    :    Source file for Generic Library - Timer Manager 
*                     implementation.  
****************************************************************************
*
*    FILENAME    :    gl_tmport.c
*
*    DESCRIPTION :    This file contains the implementation of the porting 
*                     functions for the timer server implementation used by
*                     multi-threaded applications. (source file)
*
*
* DATE         NAME            REFERENCE        REASON
*-------------     ---------------    ---------------    ----------------------
*
* Oct'99      Kapil Nayar     HSS:40600026  Generic Libraries
*                             HSS:40600036  (Gateways)
* 04/10/2001  Ravi Jindal     SPR-2751      Remove warnings with gcc,
*                                           cc,CC on Solaris
* 18/10/2001  Ravi Jindal     SPR-2788      Packaging of Genlib-Code. 
* 19/10/2001  Ravi Jindal     SPR-2802      Adding Pre-ANSI-C features.
* 17/11/2001  Ravi Jindal     SPR-3107      Incoprate the defects and change
*                                           cosmmetics of file.
*
*COPYRIGHT , Aricent, 2006
****************************************************************************/

#include "gl_tm.h"


/*******************************************************************************
* Function name    : gl_tm_send_msg_to_server
* Arguments        : gl_tm_action_t     action, 
*                    gl_timer_list_t    timer_list_id, 
*                    gl_timer_id_t      timer_id, 
*                    gw_U8bit           restart_flag,
*                    gw_U32bit          timer_duration
* Return Value     : void
* Functionality    : Send message to the timer server to execute the specified 
*                    action on the timer.
*                    To be implemented by the application iser. 
*                    This function gets invoked from the start & stop timer 
*                    APIs.
* Functions called :
*******************************************************************************/
void
gl_tm_send_msg_to_server

#ifdef _ANSI_C_
    ( gl_tm_action_t            action,
      gl_timer_list_t           timer_list_id, 
      gl_timer_id_t             timer_id,
      gw_U8bit                  restart_flag,
      gw_U32bit                 timer_duration
    )
#else
    ( action, timer_list_id, timer_id, restart_flag, timer_duration )
      gl_tm_action_t            action;
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gw_U8bit                  restart_flag;
      gw_U32bit                 timer_duration;
#endif
    
/******************************************************************************/
{
     /*  
      * To remove warnings.
      */
    action = action;
    timer_list_id = timer_list_id;
    timer_id = timer_id;
    restart_flag = restart_flag;
    timer_duration = timer_duration;
}


/*******************************************************************************
* Function name    : gl_tm_recv_msg_from_app
* Arguments        : gl_tm_action_t     *p_action,
*                    gl_timer_list_t    *p_timer_list_id, 
*                    gl_timer_id_t      *p_timer_id,
*                    gw_U32bit          *p_timer_duration, 
*                    gw_U8bit           *p_restart_flag
* Return Value     : gw_S8bit (error : GL_ERROR, success: GL_TRUE)
* Functionality    : Receive message from timer appl to be implemented by the 
*                    the application user. 
*                    This function gets invoked through the timer server for 
*                    receiving start and stop timer requests. 
* Functions called :
*******************************************************************************/
gw_S8bit gl_tm_recv_msg_from_app

#ifdef _ANSI_C_
    ( gl_tm_action_t            *p_action, 
      gl_timer_list_t           *p_timer_list_id, 
      gl_timer_id_t             *p_timer_id,
      gw_U8bit                  *p_restart_flag, 
      gw_U32bit                 *p_timer_duration
    )
#else
    ( p_action, p_timer_list_id, p_timer_id, p_restart_flag, p_timer_duration )
      gl_tm_action_t            *p_action;
      gl_timer_list_t           *p_timer_list_id; 
      gl_timer_id_t             *p_timer_id;
      gw_U8bit                  *p_restart_flag;
      gw_U32bit                 *p_timer_duration;
#endif
    
/******************************************************************************/
{
    /* 
     * To remove warnings.
     */
    p_action = p_action;
    p_timer_list_id = p_timer_list_id;
    p_timer_id = p_timer_id;
    p_restart_flag = p_restart_flag;
    p_timer_duration = p_timer_duration;

    return GL_ERROR;
    
}


/*******************************************************************************
* Function name   : gl_tm_send_expiry_msg_to_app
* Arguments       : gw_U32bit           user_info,
*                   gl_timer_list_id_t  timer_list_id, 
*                   gl_timer_id_t       timer_id,
*                   gw_U8bit            restart_flag,
*                   gw_U32bit           timer_duration
* Return Value    : void
* Functionality   : Send message to the timer application to be implemented by the 
*                   the application user. 
*                   This function gets invoked from the  gl_tm_check_timer_expiry
*                   function for sending timer expiry message to the application
*                   upon expiry of the timer.
*Functions called :
*******************************************************************************/
void
gl_tm_send_expiry_msg_to_app

#ifdef _ANSI_C_
    ( gw_U32bit                 user_info, 
      gl_timer_list_t           timer_list_id,
      gl_timer_id_t             timer_id,
      gw_U8bit                  restart_flag, 
      gw_U32bit                 timer_duration
    )
#else
    ( user_info, timer_list_id, timer_id, restart_flag, timer_duration )
      gw_U32bit                 user_info;
      gl_timer_list_t           timer_list_id;
      gl_timer_id_t             timer_id;
      gw_U8bit                  restart_flag;
      gw_U32bit                 timer_duration;
#endif
    
/******************************************************************************/
{
    /*
     * To remove warnings.
     */
    user_info = user_info;
    timer_list_id = timer_list_id;
    timer_id = timer_id;
    restart_flag = restart_flag;
    timer_duration = timer_duration;

}
/*******************************************************************************/
