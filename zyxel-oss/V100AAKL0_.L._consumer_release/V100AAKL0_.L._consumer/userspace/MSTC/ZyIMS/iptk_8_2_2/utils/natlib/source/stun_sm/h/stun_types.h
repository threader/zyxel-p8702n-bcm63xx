/******************************************************************************
*                                    
* FILE NAME    : stun_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in STUN State 
*				 Machine
*                                   
* Revision History :                            
*
*  DATE            NAME            REFERENCE       REASON        
* ------         ------------      ---------     ----------
* 24-Nov-2006	Anuradha Gupta     LLD			 Initial Version
*
* Copyright 2006, Aricent                      
*                                    
*******************************************************************************/


#ifndef __STUN_TYPES_H__
#define __STUN_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_defs.h"
#include "nat_types.h"
#include "nat_common_types.h"
#include "stun_defs.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


/*-----------------------------------------------------------------------------
 *
 * Name : stun_context_st
 *
 * Description  : This structure defines the context information of STUN.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{

     	nat_void_t			*p_enc_message;
	nat_string_st		shared_secret_username;
	nat_string_st		shared_secret_password;
	nat_uint8_t			retransmission_counter;
	nat_uint8_t			received_counter;
	nat_uint8_t			current_state;
	nat_uint8_t			next_state;
	nat_uint8_t			current_event;
	nat_timer_info_st	current_timer_info[STUN_MAX_BIND_TIMERS];
	nat_boolean_t		is_refresh_enabled;
	nat_ip_port_address_st    	destination_addr;
	nat_ip_port_address_st    	mapped_addr;
	nat_ip_port_address_st    	changed_addr;
}stun_context_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:  stun_pdb_st
 * 
 * Description:     This is the local pdb of STUN.
 * 
 * ---------------------------------------------------------------------------*/

typedef struct
{
    /* pointer to internal msg */
    nat_internal_msg_st          *p_internal_msg;
        
    /* pointer to global pdb */
    nat_glb_pdb_st               *p_glb_pdb;
    
    /*Pointer to ecode */ 
    nat_error_t                  *p_ecode;

    stun_context_st		   *p_stun_context;
    
} stun_pdb_st;


typedef nat_return_t (*stun_fsm_func_ptr_t )(INOUT stun_pdb_st             *p_stun_pdb);

/*-----------------------------------------------------------------------------
 *
 * Name : stun_data_st
 *
 * Description  : This structure defines the global data of STUN.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
			
    stun_fsm_func_ptr_t 
        stun_fsm_table[NAT_STUN_MAX_STATES][NAT_STUN_MAX_EVENTS];

   stun_context_st			*p_stun_contexts;

} stun_data_st;



#ifdef __cplusplus
}
#endif

#endif
