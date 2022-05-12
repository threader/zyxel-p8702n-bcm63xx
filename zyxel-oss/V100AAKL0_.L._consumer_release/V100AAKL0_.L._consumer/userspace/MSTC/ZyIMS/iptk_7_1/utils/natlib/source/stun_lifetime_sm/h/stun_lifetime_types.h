/******************************************************************************
*                                    
* FILE NAME    : stun_lifetime_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in STUN 
*			NAT TYPE LIFETIME Machine
*                                   
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------          ---------     ----------
*  20-Jul-2006   Anuradha Gupta        LLD           Initial version
* Copyright 2006, Aricent                 
*                                    
*******************************************************************************/


#ifndef __STUN_LIFETIME_TYPES_H__
#define __STUN_LIFETIME_TYPES_H__

/* Prevent Name Mangling By C++ Compilers */
#include "nat_defs.h"
#include "nat_types.h"
#include "nat_common_types.h"

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */


/*-----------------------------------------------------------------------------
 *
 * Name : stun_lifetime_context_st
 *
 * Description  : This structure defines the global data block of NAT type
 *                 lifetime.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
	nat_uint8_t			    current_state;
	nat_boolean_t           initial_request;
	
	nat_ip_port_address_st	local_addr_X;
	nat_ip_port_address_st	local_addr_Y;
	nat_ip_port_address_st	response_addr;

	nat_duration_t	        low_val;
	nat_duration_t          high_val;
	nat_duration_t          delta_interval;

	nat_timer_info_st	    refresh_timer_info;
	nat_duration_t	        refresh_lifetime_duration;
}stun_lifetime_context_st;

/*-----------------------------------------------------------------------------
 * 
 * Structure Name:  stun_lifetime_pdb_st
 * 
 * Description:     This is the local pdb of STUN NAT Type lifetime module.
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

   stun_lifetime_context_st	     *p_stun_lifetime_context;
    
}stun_lifetime_pdb_st;


/*-----------------------------------------------------------------------------
 *
 * Name : stun_lifetime_data_st
 *
 * Description  : This structure defines the global data block of STUN NAT Type 
 *	            lifetime module.
.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
			
	stun_lifetime_context_st			*p_stun_lifetime_context;

} stun_lifetime_data_st;


#ifdef __cplusplus
}
#endif

#endif
