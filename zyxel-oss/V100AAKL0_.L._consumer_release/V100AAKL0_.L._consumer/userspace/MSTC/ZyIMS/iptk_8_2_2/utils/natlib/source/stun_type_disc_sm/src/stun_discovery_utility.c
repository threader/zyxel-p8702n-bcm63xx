/******************************************************************************
 * 
 * FILE NAME:   stun_discovery_utility.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 *        Date            Name            Reference       Comments
 *     20-June 2006	Abhishek Chhibber    LLD	Initial Version	
 *     14-July-2006        Anuradha Gupta    LLD           Coding
 ******************************************************************************/

#include "nat_types.h"
#include "stun_discovery_prototypes.h"
#include "nat_defs.h"
#include "nat_common_defs.h"
#include "nat_error_id.h"
#include "nat_port_prototypes.h"
#include "nat_port_pltfm_prototypes.h"

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
        INOUT stun_discovery_pdb_st		 *p_stun_discovery_pdb)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_timer_id_t      timer_id;
    nat_error_t 	ecode = NAT_ERR_NONE;
	
    nat_timer_data_st   *p_timer_data = (nat_timer_data_st *)
	nat_port_static_memget(nat_port_sizeof(nat_timer_data_st),&ecode);

    p_timer_data->context_id = NAT_PROC_TYPE_DISCOVERY_ID;

    
    ret_val = nat_port_start_timer(p_stun_discovery_pdb->p_glb_pdb,
                   p_timer_data,
                   p_stun_discovery_pdb->p_stun_discovery_context->
                   refresh_disc_duration,&timer_id);
    
    if(NAT_RETURN_PENDING == ret_val)
    {                    
         p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.timer_id = NAT_INVALID_TIMER_ID;
         p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.timer_status = NAT_TIMER_STATUS_PENDING;
	p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.p_timer_data = p_timer_data;
    }
    else if(NAT_RETURN_SUCCESS == ret_val)
    {
         p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.timer_id = timer_id;
         p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.timer_status = NAT_TIMER_STATUS_VALID;
	p_stun_discovery_pdb->p_stun_discovery_context->
              refresh_timer_info.p_timer_data = p_timer_data;
	
    }
    else
    {
	nat_port_static_memfree(p_timer_data,&ecode);
        p_stun_discovery_pdb->p_glb_pdb->p_glb_data->ecode = 
                        NAT_ERR_TMR_START_FAILURE;
    }
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_stop_disc_refresh_timer
 *
 * DESCRIPTION:     This function is to start discovery refresh timer. 
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_disc_refresh_timer(
        INOUT stun_discovery_pdb_st		 *p_stun_discovery_pdb)
{
    nat_return_t ret_val = NAT_RETURN_SUCCESS;
    nat_timer_id_t      timer_id;
    nat_error_t		ecode = NAT_ERR_NONE;
    
    if(NAT_TIMER_STATUS_VALID == p_stun_discovery_pdb->
              p_stun_discovery_context->refresh_timer_info.timer_status)
    {
         timer_id = p_stun_discovery_pdb->p_stun_discovery_context->
                         refresh_timer_info.timer_id;
                               
         ret_val = nat_port_stop_timer(p_stun_discovery_pdb->p_glb_pdb,timer_id);
         
         if(NAT_RETURN_PENDING == ret_val || 
                     NAT_RETURN_SUCCESS == ret_val)
         {
              p_stun_discovery_pdb->p_stun_discovery_context->
                     refresh_timer_info.timer_status = NAT_TIMER_STATUS_INVALID;
                          
              p_stun_discovery_pdb->p_stun_discovery_context->
                     refresh_timer_info.timer_id = NAT_INVALID_TIMER_ID;

		nat_port_static_memfree(p_stun_discovery_pdb->p_stun_discovery_context->
			refresh_timer_info.p_timer_data,&ecode);
         }
         else
         {
             p_stun_discovery_pdb->p_glb_pdb->p_glb_data->ecode
                      = NAT_ERR_TMR_STOP_FAILURE;
             ret_val = NAT_RETURN_FAILURE; 
         }

    }
       
    return ret_val;
}
