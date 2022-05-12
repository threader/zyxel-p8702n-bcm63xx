/******************************************************************************
*                                    
* FILE NAME    : stun_discovery_types.h                
*                                    
* DESCRIPTION  : Contains the definitions for the data types used in STUN 
*			NAT TYPE DISCOVERY Machine
*                                   
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------
* 20-Nov-2006   Anuradha Gupta              HLD             Initial Version
*
* Copyright 2006, Aricent                    
*                                    
*******************************************************************************/


#ifndef __STUN_DISCOVERY_TYPES_H__
#define __STUN_DISCOVERY_TYPES_H__

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
 * Name : stun_discovery_context_st
 *
 * Description  : This structure defines the global data block of NAT type
 *                 discovery.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
        nat_uint8_t                current_state;

        nat_ip_port_address_st     local_addr;
        nat_ip_port_address_st     mapped_addr;
        nat_ip_port_address_st     changed_addr;
        nat_uint8_t                nat_type;
        /*
                #define NAT_PRIVATE_NETWORK_ONLY        0x01
                #define NAT_OPEN_INTERNET_INTERFACE     0x02
                #define NAT_FULL_CONE                   0x03
                #define NAT_RESTRICTED                  0x04
                #define NAT_PORT_RESTRICTED             0x05
                #define NAT_SYMMETRIC                   0x06
                #define NAT_INVALID_TYPE                0xff
        */

        nat_timer_info_st	refresh_timer_info;
        nat_duration_t    refresh_disc_duration;
}stun_discovery_context_st;


/*-----------------------------------------------------------------------------
 * 
 * Structure Name:  stun_discovery_pdb_st
 * 
 * Description:     This is the local pdb of STUN NAT Type discovery module.
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

   stun_discovery_context_st	    *p_stun_discovery_context;
    
}stun_discovery_pdb_st;


/*-----------------------------------------------------------------------------
 *
 * Name : stun_discovery_data_st
 *
 * Description  : This structure defines the global data block of STUN NAT Type 
 *	            discovery module.
.
 *
 *----------------------------------------------------------------------------*/

typedef struct
{
			
	stun_discovery_context_st			*p_stun_discovery_context;

} stun_discovery_data_st;


#ifdef __cplusplus
}
#endif

#endif
