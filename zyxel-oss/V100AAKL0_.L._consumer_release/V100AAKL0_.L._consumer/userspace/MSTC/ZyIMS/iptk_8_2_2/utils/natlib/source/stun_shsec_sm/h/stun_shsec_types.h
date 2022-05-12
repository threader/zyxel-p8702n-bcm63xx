/******************************************************************************
*                                    
* FILE NAME    : stun_shsec_shsec_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in STUN 
*			Shared Secret State Machine
*                                   
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------

* Copyright 2006, Aricent                 
*                                    
*******************************************************************************/


#ifndef __STUN_SHSEC_TYPES_H__
#define __STUN_SHSEC_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_types.h"
#include "nat_common_types.h"
#include "stun_shsec_defs.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


/*-----------------------------------------------------------------------------
 *
 * Name : stun_shsec_context_st
 *
 * Description  : This structure defines the global data block of shared secret.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
	nat_string_st		shared_secret_username;
	nat_string_st		shared_secret_password;
	nat_uint8_t			current_state;
	nat_uint8_t			current_event;
	nat_timer_info_st	current_timer_info[STUN_SHSEC_MAX_TIMERS];
	nat_duration_t	    guard_timer_duration;
}stun_shsec_context_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name:  stun_shsec_pdb_st
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

	stun_shsec_context_st	    *p_stun_shsec_context;
    
} stun_shsec_pdb_st;


typedef nat_return_t (*stun_shsec_fsm_func_ptr_t )
                   (INOUT stun_shsec_pdb_st      *p_stun_shsec_pdb);

/*-----------------------------------------------------------------------------
 *
 * Name : stun_shsec_data_st
 *
 * Description  : This structure defines the global data block of STUN.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
			
    stun_shsec_fsm_func_ptr_t 
        stun_shsec_fsm_table[NAT_STUN_SHSEC_MAX_STATES]
                            [NAT_STUN_SHSEC_MAX_EVENTS];

	stun_shsec_context_st			*p_stun_shsec_context;

} stun_shsec_data_st;


#ifdef __cplusplus
}
#endif

#endif
