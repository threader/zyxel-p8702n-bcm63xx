/******************************************************************************
 * 
 * FILE NAME:   stun_lifetime_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 *              Date            Name            Reference       Comments
 *          20-June 2006	Abhishek Chhibber    LLD			Initial Version	
 *          17-July-2006    Anuradha Gupta       LLD            Coding
 *
 ******************************************************************************/     

#include "nat_common_internal_api_id.h"
#include "stun_lifetime_macros.h"
#include "stun_lifetime_defs.h"
#include "stun_lifetime_types.h"
#include "stun_lifetime_prototypes.h"
#include "nat_common_internal_api.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_error_id.h"
#include "nat_glb_mgmt_macro.h"
#include "nat_common_prototypes.h"
#include "nat_port_prototypes.h"
#include "nat_glb_mgmt_prototypes.h"
#include "stun_discovery_prototypes.h"

/*******************************************************************************
 *
 * FUNCTION:        stun_lifetime_init
 *
 * DESCRIPTION:     This is the initialization function of STUN NAT Type lifetime
 *			  function.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_lifetime_init(
		INOUT   nat_internal_msg_st        *p_internal_msg)
        
{
    nat_return_t                 ret_val = NAT_RETURN_SUCCESS;
    nat_glb_pdb_st				 *p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_ecode_t				     *p_ecode=&(p_internal_msg->msg_hdr.ecode);
    stun_lifetime_data_st		 *p_stun_lifetime_data = NAT_NULL;
	    
    /* Allocate memory to table */
    p_stun_lifetime_data =  (stun_lifetime_data_st *)nat_port_static_memget(
                                   nat_port_sizeof(stun_lifetime_data_st),
                                   p_ecode);
    
   p_stun_lifetime_data->p_stun_lifetime_context =  (stun_lifetime_context_st *)
               nat_port_static_memget(nat_port_sizeof(stun_lifetime_context_st),
               p_ecode);

    p_glb_pdb->p_glb_data->p_stun_lifetime_data = p_stun_lifetime_data;
    p_stun_lifetime_data->p_stun_lifetime_context->initial_request = 
                  NAT_TRUE;
                  
    return ret_val;    

}


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
        INOUT nat_internal_msg_st        *p_internal_msg)
{
    nat_return_t                         ret_val=NAT_RETURN_SUCCESS;
    	
	if(NAT_INT_STUN_LIFETIME_INIT_REQ!=p_internal_msg->msg_hdr.msg_id)
	{
		stun_lifetime_pdb_st		 stun_lifetime_pdb;

		/*Initialize the stun_lifetime_pdb structure*/

		nat_port_memset((nat_void_t *)&stun_lifetime_pdb,0,
                  nat_port_sizeof(stun_lifetime_pdb_st));

		stun_lifetime_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

		stun_lifetime_pdb.p_internal_msg = p_internal_msg;

		stun_lifetime_pdb.p_ecode =  
                  &(stun_lifetime_pdb.p_internal_msg->msg_hdr.ecode);

		stun_lifetime_pdb.p_stun_lifetime_context=
			((stun_lifetime_data_st *)stun_lifetime_pdb.p_glb_pdb->p_glb_data->
                 p_stun_lifetime_data)->p_stun_lifetime_context;
		
		ret_val = stun_execute_lifetime_discovery(&stun_lifetime_pdb);
		
	}
	else
	{
		ret_val = stun_lifetime_init(p_internal_msg);

	}
	    
    return ret_val;
}/* End function */



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
        INOUT stun_lifetime_pdb_st		 *p_stun_lifetime_pdb)
{
    nat_return_t                    ret_val=NAT_RETURN_SUCCESS;
    nat_internal_msg_st             *p_internal_msg = 
                                    p_stun_lifetime_pdb->p_internal_msg;
    nat_internal_msg_st             internal_msg;
    nat_ip_port_address_st          local_address;
    nat_uint8_t                     context_id = 0;
    stun_get_public_ip_port_req_st  stun_public_ip_port_req;
    stun_get_public_ip_port_resp_st *p_stun_public_ip_port_resp = NAT_NULL;
    nat_error_t                     ecode = NAT_ERR_UNDEFINED;
    stun_lifetime_discovery_resp_st     stun_lifetime_discovery_resp;
    stun_timer_start_resp_st        *p_start_timer_resp = NAT_NULL;
    
    switch(p_stun_lifetime_pdb->p_stun_lifetime_context->current_state)
    {
	case NAT_STUN_LIFETIME_00_IDLE:
         
         if(NAT_INT_STUN_LIFETIME_DISC_REQ == p_internal_msg->msg_hdr.msg_id)
         {
             /* if type discovery is already done, generate the response from 
                here itself */
             if(NAT_STUN_BINDING_LIFETIME_COMPLETE == 
                 p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                 bind_lifetime_disc_status)
             {
                 /* send the success lifetime discovery response */
                        
                  internal_msg.msg_hdr.p_glb_pdb = 
                            p_stun_lifetime_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_LIFETIME_DISC_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_lifetime_discovery_resp);
    		
                  stun_lifetime_discovery_resp.result = NAT_RESULT_SUCCESS;
                  stun_lifetime_discovery_resp.error = NAT_ERR_NONE;
                  stun_lifetime_discovery_resp.lifetime_bind_dur = 
                      p_stun_lifetime_pdb->p_stun_lifetime_context->
                      refresh_lifetime_duration;
                    
                  if(NAT_RETURN_SUCCESS != 
                       nat_process_internal_mesg(&internal_msg))
                  {
        				p_stun_lifetime_pdb->p_glb_pdb->p_glb_data-> 
                                 ecode = internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                  }  
             }
             else
             {
                 if(NAT_TRUE == p_stun_lifetime_pdb->
                        p_stun_lifetime_context->initial_request)
                 {
                        if(NAT_RETURN_SUCCESS != nat_port_open_udp_socket(
                                            p_stun_lifetime_pdb->p_glb_pdb,
                                            &local_address))
                        {
                             ret_val = NAT_RETURN_FAILURE;
                        }
                        else
                        {
                            /* ANU : store the fd */
                          nat_port_memcpy((nat_void_t *)&p_stun_lifetime_pdb->
                            p_stun_lifetime_context->local_addr_X, 
                            (nat_void_t *)&local_address, 
                             nat_port_sizeof(nat_ip_port_address_st));
                      
                          NAT_GLB_ALLOC_CONTEXT(
                            p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                            p_stun_context_map, context_id,
                            p_stun_lifetime_pdb->p_glb_pdb->
                            p_glb_data->max_contexts)
                      
                          if(p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                             max_contexts  == context_id)
                          {
                               p_stun_lifetime_pdb->p_glb_pdb->p_glb_data-> 
                                      ecode = NAT_ERR_STUN_NO_CONTEXT_AVAILABLE;
                               ret_val = NAT_RETURN_FAILURE;
                          }
                           
                          p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                             p_stun_context_map[context_id].module_id =\
                                  NAT_STUN_MODULE_LIFETIME;
                                  
                          p_stun_lifetime_pdb->p_stun_lifetime_context->
                                 low_val = 0;
                                 
                          p_stun_lifetime_pdb->p_stun_lifetime_context->
                            high_val = p_stun_lifetime_pdb->
                            p_stun_lifetime_context->refresh_lifetime_duration;
                        }
                 }  
                 
                 if(NAT_RETURN_SUCCESS == ret_val)
                 {
                              
                  /* form the binding request */
                  internal_msg.msg_hdr.p_glb_pdb = 
                           p_stun_lifetime_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_public_ip_port_req);
			
                  stun_public_ip_port_req.context_id = context_id;
                  
			      nat_port_memcpy( &stun_public_ip_port_req.local_addr,
					   &local_address,nat_port_sizeof(nat_ip_port_address_st));
					   
                  nat_port_memcpy( &stun_public_ip_port_req.destination_addr,
					   &p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                       p_stun_config_data->stun_server_addr,
                       nat_port_sizeof(nat_ip_port_address_st));
		
                    stun_public_ip_port_req.change_ip = NAT_FALSE;
                    stun_public_ip_port_req.change_port = NAT_FALSE;
                    stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                    
         			if(NAT_RETURN_SUCCESS != 
                             nat_process_internal_mesg(&internal_msg))
           	        {
        				p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->ecode = 
                                                internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                    }
                    else
                    {
                        p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->\
                        bind_lifetime_disc_status = 
                                  NAT_STUN_BINDING_LIFETIME_ONGOING;
                               
                        p_stun_lifetime_pdb->p_stun_lifetime_context\
                            ->current_state = 
                                  NAT_STUN_LIFETIME_01_AWT_BIND_RESP;
                    }
                 }
                  
             }
         }
		break;

	case NAT_STUN_LIFETIME_01_AWT_BIND_RESP :
         
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                 /* Start timer of duration = high_val */
                 stun_start_lifetime_refresh_timer(p_stun_lifetime_pdb);
                 
                 p_stun_lifetime_pdb->p_stun_lifetime_context\
                     ->current_state = 
                          NAT_STUN_LIFETIME_02_AWT_TIMER_EXPIRY_IND;
                          
                 nat_port_memcpy( &p_stun_lifetime_pdb->
                   p_stun_lifetime_context->response_addr,
				   &p_stun_public_ip_port_resp->mapped_addr,
                   nat_port_sizeof(nat_ip_port_address_st));
              }
              else
              {
                  p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->\
                            bind_lifetime_disc_status = 
                                      NAT_STUN_BINDING_LIFETIME_COMPLETE;
                                   
                  p_stun_lifetime_pdb->p_stun_lifetime_context\
                        ->current_state = 
                            NAT_STUN_LIFETIME_00_IDLE;
                  
                  /* release the IP/PORT to port layer */
                  nat_port_close_udp_socket(p_stun_lifetime_pdb->p_glb_pdb,
                    &p_stun_lifetime_pdb->p_stun_lifetime_context->local_addr_X);
                    
                  nat_port_close_udp_socket(p_stun_lifetime_pdb->p_glb_pdb,
                    &p_stun_lifetime_pdb->p_stun_lifetime_context->local_addr_Y);
                    
                  /* send the success lifetime discovery response */
                        
                  internal_msg.msg_hdr.p_glb_pdb = 
                            p_stun_lifetime_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_LIFETIME_DISC_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_lifetime_discovery_resp);
    		
                  stun_lifetime_discovery_resp.result = NAT_RESULT_FAILURE;
                  stun_lifetime_discovery_resp.error = 
                      p_stun_public_ip_port_resp->error;
                
                    
                  if(NAT_RETURN_SUCCESS != 
                       nat_process_internal_mesg(&internal_msg))
                  {
        				p_stun_lifetime_pdb->p_glb_pdb->p_glb_data-> 
                                 ecode = internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                  }
                  
              }
         }
		break;

	case NAT_STUN_LIFETIME_02_AWT_TIMER_EXPIRY_IND:
         
         if(NAT_INT_TIMER_EXPIRY_IND == 
                      p_internal_msg->msg_hdr.msg_id)
         {
             if(NAT_FALSE == p_stun_lifetime_pdb->
                        p_stun_lifetime_context->initial_request)
             {
                  if(NAT_RETURN_SUCCESS != nat_port_open_udp_socket(
                                        p_stun_lifetime_pdb->p_glb_pdb,
                                        &local_address))
                  {
                      ret_val = NAT_RETURN_FAILURE;
                  }
                  else
                  {
                      nat_port_memcpy((nat_void_t *)&p_stun_lifetime_pdb->
                         p_stun_lifetime_context->local_addr_Y, 
                         (nat_void_t *)&local_address, 
                         nat_port_sizeof(nat_ip_port_address_st));
                  }
                  
              }
                  
             if(NAT_RETURN_SUCCESS == ret_val)
             {
              /* form the new binding request */
              internal_msg.msg_hdr.p_glb_pdb = 
                       p_stun_lifetime_pdb->p_glb_pdb;
              internal_msg.msg_hdr.msg_id = 
                       NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
              internal_msg.p_msg_data = 
                     (nat_uint8_t *)(&stun_public_ip_port_req);
		
              stun_public_ip_port_req.context_id = context_id;
              
		      nat_port_memcpy( &stun_public_ip_port_req.local_addr,
				   &local_address,nat_port_sizeof(nat_ip_port_address_st));
				   
              nat_port_memcpy( &stun_public_ip_port_req.destination_addr,
				   &p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                   p_stun_config_data->stun_server_addr,
                   nat_port_sizeof(nat_ip_port_address_st));
                   
              nat_port_memcpy( &stun_public_ip_port_req.response_addr,
				   &p_stun_lifetime_pdb->p_stun_lifetime_context->
                   response_addr,
                   nat_port_sizeof(nat_ip_port_address_st));
	
                stun_public_ip_port_req.change_ip = NAT_FALSE;
                stun_public_ip_port_req.change_port = NAT_FALSE;
                stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                  
     			if(NAT_RETURN_SUCCESS != 
                         nat_process_internal_mesg(&internal_msg))
       	        {
    				p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->ecode = 
                                            internal_msg.msg_hdr.ecode;
       	            ret_val = NAT_RETURN_FAILURE;
                }
                else
                {        
                    p_stun_lifetime_pdb->p_stun_lifetime_context\
                        ->current_state = 
                              NAT_STUN_LIFETIME_03_AWT_NEW_BIND_RESP;                  
                }
             }
         }
         else if(NAT_INT_START_TIMER_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
             p_start_timer_resp =  (stun_timer_start_resp_st *)
                      p_internal_msg->p_msg_data;
                      
             p_stun_lifetime_pdb->p_stun_lifetime_context->
                 refresh_timer_info.timer_id = p_start_timer_resp->timer_id;
                 
             p_stun_lifetime_pdb->p_stun_lifetime_context->
                 refresh_timer_info.timer_status = NAT_TIMER_STATUS_VALID;
         }
		break;

	case NAT_STUN_LIFETIME_03_AWT_NEW_BIND_RESP:
         
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                  if(NAT_RETURN_SUCCESS == nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->local_addr,
                            p_stun_lifetime_pdb->
                                  p_stun_lifetime_context->local_addr_X, ecode))
                     {
                         stun_lifetime_disc_timer_double_algo(
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->low_val,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->high_val);
                     }
                     else if(NAT_RETURN_SUCCESS == nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->local_addr,
                            p_stun_lifetime_pdb->
                                  p_stun_lifetime_context->local_addr_Y, ecode))
                     {
                         stun_lifetime_disc_timer_reduce_algo(
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->low_val,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->high_val);
                     }
                     else
                     {
                         ret_val = NAT_RETURN_FAILURE;
                     }
                     
                 
                 
              }
              else
              {
                  if(NAT_ERR_STUN_NO_BINDING_RESPONSE == 
                        p_stun_public_ip_port_resp->error)
                  {
                        stun_lifetime_disc_timer_double_algo(
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->low_val,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->high_val);
                  }
                  else
                  {
                      if(NAT_RETURN_SUCCESS == nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->local_addr,
                            p_stun_lifetime_pdb->
                                  p_stun_lifetime_context->local_addr_X, ecode))
                     {
                         stun_lifetime_disc_timer_double_algo(
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->low_val,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->high_val);
                     }
                     else if(NAT_RETURN_SUCCESS == nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->local_addr,
                            p_stun_lifetime_pdb->
                                  p_stun_lifetime_context->local_addr_Y, ecode))
                     {
                         stun_lifetime_disc_timer_reduce_algo(
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->low_val,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->high_val);
                     }
                     else
                     {
                         ret_val = NAT_RETURN_FAILURE;
                     }
                  }
              }
              
              if((p_stun_lifetime_pdb->p_stun_lifetime_context->high_val - 
                 p_stun_lifetime_pdb->p_stun_lifetime_context->low_val) <= 
                    p_stun_lifetime_pdb->p_stun_lifetime_context->delta_interval)
             {
                /* procedure complete, send successful response */
                
                /* form the success response */
                  internal_msg.msg_hdr.p_glb_pdb = 
                           p_stun_lifetime_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_LIFETIME_DISC_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_lifetime_discovery_resp);
    		
    		      if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
    		      {
        		      stun_lifetime_discovery_resp.result = NAT_RESULT_SUCCESS;
        		      stun_lifetime_discovery_resp.error = NAT_ERR_NONE;
        		      stun_lifetime_discovery_resp.lifetime_bind_dur = 
        		        p_stun_lifetime_pdb->p_stun_lifetime_context->high_val;
                  }
                  else
                  {
                      stun_lifetime_discovery_resp.result = NAT_RESULT_FAILURE;
        		      stun_lifetime_discovery_resp.error = 
                               p_stun_public_ip_port_resp->error;
                  }
                  
                     
         			if(NAT_RETURN_SUCCESS != 
                             nat_process_internal_mesg(&internal_msg))
           	        {
        				p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->ecode = 
                                                internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                    }
                    else
                    {        
                        p_stun_lifetime_pdb->p_stun_lifetime_context\
                            ->current_state = 
                                  NAT_STUN_LIFETIME_00_IDLE;  
                                  
                        p_stun_lifetime_pdb->p_glb_pdb->p_glb_data->
                           bind_lifetime_disc_status =
                                NAT_STUN_BINDING_LIFETIME_COMPLETE;
             
                        p_stun_lifetime_pdb->p_stun_lifetime_context->
                          refresh_lifetime_duration = p_stun_lifetime_pdb->
                                     p_stun_lifetime_context->high_val; 
                                     
                        /* release the IP/PORT to port layer */
                        nat_port_close_udp_socket(p_stun_lifetime_pdb->p_glb_pdb,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->local_addr_X);
                        
                        nat_port_close_udp_socket(p_stun_lifetime_pdb->p_glb_pdb,
                            &p_stun_lifetime_pdb->p_stun_lifetime_context->local_addr_Y);  
                              
                        p_stun_lifetime_pdb->
                        p_stun_lifetime_context->initial_request = NAT_TRUE;            
                    }
             }
             else
             {
                 /* restart the procedure again with new timer values */
                 p_stun_lifetime_pdb->p_stun_lifetime_context->
                           initial_request = NAT_FALSE;
                           
                 p_stun_lifetime_pdb->p_stun_lifetime_context\
                        ->current_state = 
                              NAT_STUN_LIFETIME_00_IDLE; 
                              
                 ret_val = stun_execute_lifetime_discovery(
                                            p_stun_lifetime_pdb);
             }
         }
		break;

	default:
		ret_val = NAT_RETURN_FAILURE;

	}
	
    return ret_val;
}  



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
		INOUT   nat_internal_msg_st        *p_internal_msg)     
{
    stun_lifetime_pdb_st		 stun_lifetime_pdb;
    nat_glb_pdb_st		*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_error_t          ecode;
    
 
    stun_lifetime_pdb.p_glb_pdb = p_glb_pdb;
    stun_lifetime_pdb.p_stun_lifetime_context=
			((stun_lifetime_data_st *)stun_lifetime_pdb.p_glb_pdb->p_glb_data
                             ->p_stun_lifetime_data)->p_stun_lifetime_context;

   /* ANU : check with the function 
    stun_stop_disc_refresh_timer(&stun_lifetime_pdb);
   */

    NAT_GLB_DEALLOC_CONTEXT(p_glb_pdb->p_glb_data->
            p_stun_context_map[NAT_STUN_MODULE_DISCOVERY])
    
    nat_port_static_memfree(((stun_lifetime_data_st *)p_glb_pdb->
           p_glb_data->p_stun_lifetime_data)->p_stun_lifetime_context, &ecode);
                    
    nat_port_static_memfree(p_glb_pdb->p_glb_data->p_stun_lifetime_data, &ecode);
                    
    return;
}


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
        INOUT stun_lifetime_pdb_st		 *p_stun_lifetime_pdb)
{
#ifdef NAT_PORT_WINDOWS
	p_stun_lifetime_pdb = p_stun_lifetime_pdb;
#endif
    return NAT_RETURN_SUCCESS;
}


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
                   nat_duration_t    *p_high_val)
{
#ifdef NAT_PORT_WINDOWS
	p_low_val = p_low_val;
	p_high_val = p_high_val;
#endif
     return NAT_RETURN_SUCCESS;
 }
                   
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
                   nat_duration_t    *p_high_val)
 {
#ifdef NAT_PORT_WINDOWS
	p_low_val = p_low_val;
	p_high_val = p_high_val;
#endif
     return NAT_RETURN_SUCCESS;
 }
