/******************************************************************************
 * 
 * FILE NAME:   stun_shsec_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 *              Date            Name            Reference       Comments
 *          20-June 2006		Abhishek Chhibber LLD			Initial Version	
 ******************************************************************************/     

#include "nat_common_internal_api_id.h"
#include "stun_shsec_macro.h"
#include "stun_shsec_defs.h"
#include "stun_shsec_types.h"
#include "stun_shsec_prototypes.h"
#include "nat_common_internal_api.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_glb_mgmt_macro.h"

/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_init
 *
 * DESCRIPTION:     This is the initialization function of STUN Shared Secret State 
 *		  	  Machine.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_init(
		INOUT   nat_internal_msg_st        *p_internal_msg)
        
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    nat_glb_pdb_st			*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_ecode_t				*p_ecode=&(p_internal_msg->msg_hdr.ecode);
    stun_shsec_data_st		*p_stun_shsec_data = NAT_NULL;
    nat_uint8_t				i=0,j=0;
	    
    /* Allocate memory to table */
    p_stun_shsec_data = (stun_shsec_data_st *)nat_port_static_memget
                              (nat_port_sizeof(stun_shsec_data_st),p_ecode);
    
    /* Initialize the table */
    /* Assign the function names in the FSM table for the events recieved when 
     * the system is in IDLE state.
     */  
      
    for(i=0;i< NAT_STUN_SHSEC_MAX_STATES;i++)
    {
    	for(j=0;j< NAT_STUN_SHSEC_MAX_EVENTS;j++)
    	{
    		p_stun_shsec_data->stun_shsec_fsm_table[i][j] = 
                      stun_shsec_unexpected_event;
    	}
    }

    /* Binding req handler will be called */      
    p_stun_shsec_data->stun_shsec_fsm_table[NAT_STUN_SHSEC_00_IDLE]
        [NAT_STUN_SHSEC_REQ_EVENT] = stun_shsec_00_shared_secret_req;
    
     p_stun_shsec_data->stun_shsec_fsm_table[NAT_STUN_SHSEC_00_IDLE]
        [NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT] = 
                                   stun_shsec_00_refresh_timer_expiry;

	p_stun_shsec_data->stun_shsec_fsm_table[NAT_STUN_SHSEC_01_AWT_RESP ]
        [NAT_STUN_SHSEC_RESP_EVENT] = stun_shsec_01_shared_secret_response;

	/* Refresh Timer expiry handler will be called */

	p_stun_shsec_data->stun_shsec_fsm_table[NAT_STUN_SHSEC_01_AWT_RESP ]
        [NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT] = 
                                    stun_shsec_01_guard_timer_expiry;

    p_stun_shsec_data->stun_shsec_fsm_table 
        [NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP][NAT_STUN_SHSEC_RESP_EVENT] = 
                                    stun_shsec_02_shared_secret_response;

	p_stun_shsec_data->stun_shsec_fsm_table
        [NAT_STUN_SHSEC_02_AWT_TIMER_START_RESP]
        [NAT_STUN_SHSEC_TIMER_START_RESP_EVENT] = stun_shsec_02_timer_start_resp;

    p_stun_shsec_data->p_stun_shsec_context =  (stun_shsec_context_st *)
        nat_port_static_memget(nat_port_sizeof(stun_shsec_context_st),p_ecode);

    NAT_STUN_SHSEC_INIT_CONTEXT(p_stun_shsec_data->p_stun_shsec_context);
    
    p_glb_pdb->p_glb_data->p_stun_shsec_data = p_stun_shsec_data;
    
    return ret_val;    

}


/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_api_to_event
 *
 * DESCRIPTION:     This function maps the messages recieved to events.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

nat_return_t stun_shsec_api_to_event(
        INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb)
{
    nat_return_t				  ret_val = NAT_RETURN_SUCCESS;
    
       
    switch(p_stun_shsec_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case NAT_INT_STUN_SHSEC_REQ:
           	 NAT_STUN_SHSEC_SET_EVENT(p_stun_shsec_pdb,
                       NAT_STUN_SHSEC_REQ_EVENT)
            break;

       case NAT_INT_STUN_SHSEC_RESP:    
            
           NAT_STUN_SHSEC_SET_EVENT(p_stun_shsec_pdb,
                      NAT_STUN_SHSEC_RESP_EVENT)
           break;

	 case NAT_INT_START_TIMER_RESP:
            NAT_STUN_SHSEC_SET_EVENT(p_stun_shsec_pdb,
                      NAT_STUN_SHSEC_TIMER_START_RESP_EVENT)
            break;

       case NAT_INT_TIMER_EXPIRY_IND:
            NAT_STUN_SHSEC_SET_EVENT(p_stun_shsec_pdb,
                     NAT_STUN_SHSEC_TIMER_EXPIRY_IND_EVENT)
            break;
        default:
		   ret_val = NAT_RETURN_FAILURE;
           break;
    }
    
   return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        nat_stun_shsec_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of Shared Secret state Machine 
 *		  	  The function checks the API Id in the message received and invokes
 *                  appropriate handler based on current state and the event 
 *	              (mapped from API Id).
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/

nat_return_t nat_stun_shsec_process_mesg(
        INOUT nat_internal_msg_st        *p_internal_msg)
{
    nat_return_t             ret_val=NAT_RETURN_SUCCESS;
    stun_shsec_pdb_st		 stun_shsec_pdb;
    	
	if(NAT_INT_STUN_SHSEC_INIT_REQ ==
                p_internal_msg->msg_hdr.msg_id)
	{
          stun_shsec_init(p_internal_msg);
    }
    else if(NAT_INT_STUN_SHSEC_DEINIT_REQ ==
                p_internal_msg->msg_hdr.msg_id)
	{
          stun_shsec_deinit(p_internal_msg);
    }
    else
    {
		/*Initialize the stun_shsec_pdb structure*/
		stun_shsec_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

		stun_shsec_pdb.p_internal_msg = p_internal_msg;

		stun_shsec_pdb.p_ecode =  &(stun_shsec_pdb.p_internal_msg->msg_hdr.ecode);

		stun_shsec_pdb.p_stun_shsec_context=
			((stun_shsec_data_st *)stun_shsec_pdb.p_glb_pdb->p_glb_data->
                     p_stun_shsec_data)->p_stun_shsec_context;

		ret_val = stun_shsec_api_to_event(&stun_shsec_pdb);

		if(NAT_RETURN_SUCCESS == ret_val)
		{
            ret_val = ((stun_shsec_data_st *)stun_shsec_pdb.p_glb_pdb->\
                       p_glb_data->p_stun_shsec_data)->stun_shsec_fsm_table\
		               [stun_shsec_pdb.p_stun_shsec_context->current_state]\
			           [stun_shsec_pdb.p_stun_shsec_context->current_event]\
                      (&stun_shsec_pdb);
        }
	}
	  
    return ret_val;
}/* End function */


/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_deinit
 *
 * DESCRIPTION:     This is the deinitialization function of STUN State Machine
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_void_t stun_shsec_deinit(
      INOUT nat_internal_msg_st        *p_internal_msg)     
{
    stun_shsec_pdb_st    stun_shsec_pdb;

    nat_glb_pdb_st		*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_error_t          ecode;
    
 
    stun_shsec_pdb.p_glb_pdb = p_glb_pdb;
    
    stun_shsec_stop_current_timers(&stun_shsec_pdb);
    
    NAT_GLB_DEALLOC_CONTEXT(p_glb_pdb->p_glb_data->
                         p_stun_context_map[NAT_STUN_MODULE_SHSEC])
    
    nat_port_static_memfree(((stun_shsec_data_st *)p_glb_pdb->
                    p_glb_data->p_stun_shsec_data)->p_stun_shsec_context, &ecode);
                    
    nat_port_static_memfree(p_glb_pdb->p_glb_data->p_stun_shsec_data, &ecode);
                    
    return;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_stop_current_timers
 *
 * DESCRIPTION:     This function is used to stop all running timers.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_stop_current_timers(
		INOUT stun_shsec_pdb_st             *p_stun_shsec_pdb)
{
   p_stun_shsec_pdb = p_stun_shsec_pdb;
    return NAT_RETURN_SUCCESS;
}
		
/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_stop_timer
 *
 * DESCRIPTION:     This function is used to stop the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_stop_timer(
		INOUT stun_shsec_pdb_st        *p_stun_shsec_pdb,
        IN    nat_timer_type_t         timer_type)
{
	p_stun_shsec_pdb = p_stun_shsec_pdb;
	timer_type =timer_type;
    return NAT_RETURN_SUCCESS;
}
        
/*******************************************************************************
 *
 * FUNCTION:        stun_shsec_start_timer
 *
 * DESCRIPTION:     This function is used to start the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_shsec_start_timer(
		INOUT stun_shsec_pdb_st        *p_stun_shsec_pdb,
		IN    nat_uint32_t             context_id,
        IN    nat_timer_type_t         timer_type,
        IN    nat_duration_t           timer_dur)
{
    p_stun_shsec_pdb = p_stun_shsec_pdb;
    context_id = context_id;
    timer_dur = timer_dur;
    timer_type = timer_type;
    return NAT_RETURN_SUCCESS;
}
