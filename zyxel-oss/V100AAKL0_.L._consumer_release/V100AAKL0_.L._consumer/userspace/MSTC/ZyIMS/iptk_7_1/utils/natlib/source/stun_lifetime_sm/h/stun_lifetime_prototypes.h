/******************************************************************************
*                                    
* FILE NAME    : stun_lifetime_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the structures used in NAT Lifetime
*				 Discovery Prototypes APIs.    
*                                    
* Revision History :                            
*
*  DATE            NAME                        REFERENCE       REASON        
* ------         ------------                  ---------     ----------

* Copyright 2006, Aricent              
*                                    
*******************************************************************************/



#ifndef __STUN_LIFETIME_PROTOTYPES_H_

#define __STUN_LIFETIME_PROTOTYPES_H_

#include "nat_types.h"
#include "stun_lifetime_types.h"

#ifdef __cplusplus

extern "C"
{
#endif

/*******************************************************************************
 *
 * FUNCTION:        stun_lifetime_init
 *
 * DESCRIPTION:     This is the initialization function of Lifetime module
 *				
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_lifetime_init(
		INOUT nat_internal_msg_st        *p_internal_msg);

/*******************************************************************************
 *
 * FUNCTION:        stun_lifetime_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN NAT binding 
 *			        lifeitme discovery module.
 *                  
 * RETURNS:         void.                 
 *
 ******************************************************************************/
nat_void_t stun_lifetime_deinit(
		INOUT   nat_internal_msg_st        *p_internal_msg);
		
/*******************************************************************************
 *
 * FUNCTION:        nat_stun_lifetime_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of STUN Nat Discovery Machine.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/

nat_return_t nat_stun_lifetime_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg);


/*******************************************************************************
 *
 * FUNCTION:        stun_execute_lifetime_discovery
 *
 * DESCRIPTION:     This is lifetime discovery procedure.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.
 *
 ******************************************************************************/
 nat_return_t stun_execute_lifetime_discovery(
         INOUT stun_lifetime_pdb_st               *p_stun_lifetime_pdb);

/*******************************************************************************
 *
 * FUNCTION:        stun_lifetime_disc_timer_double_algo
 *
 * DESCRIPTION:     This is the doubling algo for lifetime finding timer . 
 *                  calculation.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t
stun_lifetime_disc_timer_double_algo(
                   nat_duration_t   *p_low_val,
                   nat_duration_t    *p_high_val);
                   
/*******************************************************************************
 *
 * FUNCTION:        stun_lifetime_disc_timer_reduce_algo
 *
 * DESCRIPTION:     This is the reducing algo for lifetime finding timer  
 *                  calculation.
 *
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
 nat_return_t
stun_lifetime_disc_timer_reduce_algo(
                   nat_duration_t   *p_low_val,
                   nat_duration_t    *p_high_val);
       
/*******************************************************************************
 *
 * FUNCTION:        stun_start_lifetime_refresh_timer
 *
 * DESCRIPTION:     This function is to start lifetime refresh timer. 
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_start_lifetime_refresh_timer(
        INOUT stun_lifetime_pdb_st		 *p_stun_lifetime_pdb);
        

/*******************************************************************************
 *
 * FUNCTION:        stun_stop_lifetime_refresh_timer
 *
 * DESCRIPTION:     This function is to stop lifetime refresh timer. 
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
 nat_return_t stun_stop_lifetime_refresh_timer(
        INOUT stun_lifetime_pdb_st		 *p_stun_lifetime_pdb);
        
        
                    
#ifdef __cpluplus

}
#endif

#endif
