/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_macro.h 
 * 
 * DESCRIPTION: This file contains the macros used to implement the 
 *              functionality of STUN State Machine.
 *              
 * REVISION HISTORY:
 *  DATE            NAME            REFERENCE       REASON        
 * ------         ------------      ---------     ----------
 * 24-Nov-2006	Anuradha Gupta     LLD			 Initial Version
 *          
 * Copyright 2006, Aricent                            
 * ****************************************************************************/
#ifndef __STUN_MACROS_H__
#define __STUN_MACROS_H__


#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


#include "nat_common_types.h"




/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_STATE
 * 
 * Description: This is used to set the current state in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_SET_STATE(p_stun_pdb,state)\
{\
    p_stun_pdb->p_stun_context->current_state = state;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_NEXT_STATE
 * 
 * Description: This is used to set the next state in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_SET_NEXT_STATE(p_stun_pdb,state)\
{\
    p_stun_pdb->p_stun_context->next_state = state;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_EVENT
 * 
 * Description: This is used to set the event in the stun context.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_SET_EVENT(p_stun_pdb,event)\
{\
p_stun_pdb->p_stun_context->current_event = event;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_GET_STATE
 * 
 * Description: This is used to get the current state in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_GET_STATE(p_stun_pdb) p_stun_pdb->p_stun_context->current_state

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_GET_NEXT_STATE
 * 
 * Description: This is used to get the next state in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_GET_NEXT_STATE(p_stun_pdb) p_stun_pdb->p_stun_context->next_state 

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_GET_EVENT
 * 
 * Description: This is used to get the event in the stun context.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_GET_EVENT(p_stun_pdb) p_stun_pdb->p_stun_context->current_event
    
          
/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_ECODE
 * 
 * Description: This is the macro used for setting ecodes.
 * 
 *----------------------------------------------------------------------------*/
#define NAT_STUN_SET_ECODE(p_stun_pdb,error)\
{\
    *(p_stun_pdb->->p_ecode) = (nat_error_t)(error);\
}


/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_DEINIT_CONTEXT
 * 
 * Description: This is used to clear the context of STUN.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)\
{\
    nat_error_t             ecode;\
\
	nat_port_static_memfree(p_stun_pdb->p_stun_context->p_enc_message, &ecode);\
    nat_port_memset(p_stun_pdb->p_stun_context,0,nat_port_sizeof(stun_context_st));\
  	p_stun_pdb->p_stun_context->current_state = 0;\
    p_stun_pdb->p_stun_context->current_event = NAT_STUN_INVALID_EVENT;\
}    

/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_CLEAR_CONTEXT
 * 
 * Description: This is used to clear the context of STUN except all Addresses and
 *		refresh enabled flag.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_CLEAR_CONTEXT(p_stun_pdb)\
{\
	nat_boolean_t		temp_is_refresh_enabled;\
	nat_ip_port_address_st    	temp_mapped_addr;\
	nat_ip_port_address_st    	temp_changed_addr;\
	nat_ip_port_address_st    	temp_dest_addr;\
	nat_error_t             ecode;\
\
	temp_is_refresh_enabled = p_stun_pdb->p_stun_context->is_refresh_enabled;\
	temp_mapped_addr = p_stun_pdb->p_stun_context->mapped_addr;\
	temp_changed_addr = p_stun_pdb->p_stun_context->changed_addr;\
	temp_dest_addr = p_stun_pdb->p_stun_context->destination_addr;\
\
	nat_port_static_memfree(p_stun_pdb->p_stun_context->p_enc_message, &ecode);\
	nat_port_memset(p_stun_pdb->p_stun_context,0,\
           nat_port_sizeof(stun_context_st));\
  	p_stun_pdb->p_stun_context->current_state = 0;\
    	p_stun_pdb->p_stun_context->current_event = NAT_STUN_INVALID_EVENT;\
\
	p_stun_pdb->p_stun_context->is_refresh_enabled = temp_is_refresh_enabled;\
	p_stun_pdb->p_stun_context->mapped_addr = temp_mapped_addr;\
	p_stun_pdb->p_stun_context->changed_addr = temp_changed_addr;\
	p_stun_pdb->p_stun_context->destination_addr = temp_dest_addr;\
}    

/*-----------------------------------------------------------------------------
 * 
 * Macro Name: NAT_STUN_INIT_CONTEXT
 * 
 * Description: This is used to init the context of STUN.
 * 
 *----------------------------------------------------------------------------*/

#define NAT_STUN_INIT_CONTEXT(p_stun_context)\
{\
    nat_port_memset(&p_stun_context,0,sizeof(stun_context_st));\
  	p_stun_context.current_state = 0;\
    p_stun_context.current_event = NAT_STUN_INVALID_EVENT;\
}    

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_RESET_TIMER
 * 
 * Description: This is used to reset the timer info in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_RESET_TIMER(p_stun_pdb,timer_type)\
{\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_id = \
                   NAT_INVALID_TIMER_ID;\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_status = \
                   NAT_TIMER_STATUS_INVALID;\
}  

/*------------------------------------------------------------------------------
 * 
 * Macro name: NAT_STUN_SET_TIMER
 * 
 * Description: This is used to set the timer info in the stun context.
 * 
 *----------------------------------------------------------------------------*/ 
#define NAT_STUN_SET_TIMER(p_stun_pdb,timer_type,timer_id)\
{\
if(0 == timer_id)\
{\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_id = \
                   NAT_INVALID_TIMER_ID;\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_status =\
                   NAT_TIMER_STATUS_PENDING;\
}\
else \
{\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_id = \
                    timer_id;\
    p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_status = \
                    NAT_TIMER_STATUS_VALID;\
}\
} 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

