/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_discovery_prototypes.h 
 * 
 * DESCRIPTION: This file contains the macros used to implement the 
 *              functionality of Stun discovery state machine.
 *              
 * REVISION HISTORY:
 *              Name                Date            Reference
 * 20-Nov-2006   Anuradha Gupta              HLD             Initial Version
 *
 *          
 * Copyright 2006, Aricent                          
 * ****************************************************************************/

#ifndef __STUN_DISCOVERY_PROTOTYPES_H_
#define __STUN_DISCOVERY_PROTOTYPES_H_


#include "stun_discovery_types.h"

#ifdef __cplusplus

extern "C"
{

#endif

/*******************************************************************************
 *
 * FUNCTION:        stun_discovery_init
 *
 * DESCRIPTION:     This is the initialization function of STUN NAT Type discovery
 *                        function.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
nat_return_t stun_discovery_init(
                INOUT   nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_discovery_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN NAT Type 
 *			        discovery function.
 *                  
 * RETURNS:         void.                 
 *
 ******************************************************************************/
nat_void_t stun_discovery_deinit(
		INOUT   nat_internal_msg_st        *p_internal_msg);
		
/*******************************************************************************
 *
 * FUNCTION:        nat_stun_discovery_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of STUN Nat Discovery Machine.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

nat_return_t nat_stun_discovery_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_execute_type_discovery
 *
 * DESCRIPTION:     This is Nat type discovery execute procedure.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

nat_return_t stun_execute_type_discovery(
        INOUT stun_discovery_pdb_st              *p_stun_discovery_pdb);


/*******************************************************************************
 *
 * FUNCTION:        stun_start_disc_refresh_timer
 *
 * DESCRIPTION:     This function is to start discovery refresh timer. 
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_start_disc_refresh_timer(
        INOUT stun_discovery_pdb_st		 *p_stun_discovery_pdb);
        
/*******************************************************************************
 *
 * FUNCTION:        stun_stop_disc_refresh_timer
 *
 * DESCRIPTION:     This function is to stop discovery refresh timer. 
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_disc_refresh_timer(
        INOUT stun_discovery_pdb_st		 *p_stun_discovery_pdb);
        

#ifdef __cplusplus
}
#endif

#endif

