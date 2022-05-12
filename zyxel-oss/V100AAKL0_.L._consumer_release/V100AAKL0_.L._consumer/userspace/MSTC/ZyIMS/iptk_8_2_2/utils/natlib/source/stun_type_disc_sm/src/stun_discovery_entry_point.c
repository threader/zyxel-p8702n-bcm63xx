/******************************************************************************
 * 
 * FILE NAME:   stun_discovery_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 *        Date            Name            Reference       Comments
 *     20-June 2006		Abhishek Chhibber    LLD			Initial Version	
 *     14-July-2006        Anuradha Gupta    LLD           Coding
 ******************************************************************************/     

#include "nat_common_internal_api_id.h"
#include "stun_discovery_macro.h"
#include "stun_discovery_defs.h"
#include "stun_discovery_types.h"
#include "stun_discovery_prototypes.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_common_internal_api.h"
#include "nat_glb_mgmt_macro.h"
#include "nat_error_id.h"
#include "nat_defs.h"
#include "nat_common_prototypes.h"
#include "nat_glb_mgmt_prototypes.h"
#include "nat_port_prototypes.h"

/*******************************************************************************
 *
 * FUNCTION:        stun_discovery_init
 *
 * DESCRIPTION:     This is the initialization function of STUN NAT Type discovery
 *			  function.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_discovery_init(
		INOUT   nat_internal_msg_st        *p_internal_msg)
        
{
    nat_return_t                ret_val = NAT_RETURN_SUCCESS;
    nat_glb_pdb_st		        *p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_ecode_t			        *p_ecode=&(p_internal_msg->msg_hdr.ecode);
    stun_discovery_data_st	    *p_stun_discovery_data = NAT_NULL;
   	    
    /* Allocate memory to table */
    p_stun_discovery_data = (stun_discovery_data_st *)nat_port_static_memget(
                                        nat_port_sizeof(stun_discovery_data_st),
                                        p_ecode);
    
    p_stun_discovery_data->p_stun_discovery_context = (stun_discovery_context_st *)
                    nat_port_static_memget(nat_port_sizeof(stun_discovery_context_st),
                    p_ecode);

    p_glb_pdb->p_glb_data->p_stun_discovery_data = p_stun_discovery_data;
    
    return ret_val;    

}


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
        INOUT nat_internal_msg_st        *p_internal_msg)
{
    nat_return_t                     ret_val=NAT_RETURN_SUCCESS;
	stun_discovery_pdb_st		 stun_discovery_pdb;
    	
	if(NAT_INT_STUN_DISCOVERY_INIT_REQ == 
                                        p_internal_msg->msg_hdr.msg_id)
    {
          stun_discovery_init(p_internal_msg);
    }
    else if(NAT_INT_STUN_DISCOVERY_DEINIT_REQ == 
                                        p_internal_msg->msg_hdr.msg_id)
    {
          stun_discovery_deinit(p_internal_msg);
    }
    else
	{
		/*Initialize the stun_discovery_pdb structure*/
		stun_discovery_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

		stun_discovery_pdb.p_internal_msg = p_internal_msg;

		stun_discovery_pdb.p_ecode =  &(stun_discovery_pdb.p_internal_msg->msg_hdr.ecode);

		stun_discovery_pdb.p_stun_discovery_context=
			((stun_discovery_data_st *)stun_discovery_pdb.p_glb_pdb->p_glb_data
                             ->p_stun_discovery_data)->p_stun_discovery_context;

		ret_val = stun_execute_type_discovery(&stun_discovery_pdb);	
	}
	
        
    return ret_val;
}/* End function */



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
        INOUT stun_discovery_pdb_st		 *p_stun_discovery_pdb)
{
    nat_return_t                    ret_val=NAT_RETURN_SUCCESS;
    nat_internal_msg_st             *p_internal_msg = 
                                    p_stun_discovery_pdb->p_internal_msg;
    nat_internal_msg_st             internal_msg;
    nat_ip_port_address_st          local_address;
    nat_uint8_t                     context_id = 0;
    stun_get_public_ip_port_req_st  stun_public_ip_port_req;
    stun_get_public_ip_port_resp_st *p_stun_public_ip_port_resp = NAT_NULL;
    nat_error_t                     ecode = NAT_ERR_UNDEFINED;
    stun_type_discovery_resp_st     stun_type_discovery_resp;
   
	nat_cmn_convert_ip_string_to_ip4_addr(
		p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
			p_stun_config_data->self_ip.str,&local_address.ipv4_addr);
	local_address.port = p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
		p_stun_config_data->type_disc_port;
 
    switch(p_stun_discovery_pdb->p_stun_discovery_context->current_state)
    {
	case NAT_STUN_DISCOVERY_00_IDLE:
        
         if(NAT_INT_STUN_DISCOVERY_TYPE_REQ == p_internal_msg->msg_hdr.msg_id)
         {
             /* if type discovery is already done, generate the response from 
                here itself */
             if(NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE == 
                 p_stun_discovery_pdb->p_glb_pdb->p_glb_data->nat_type_disc_status)
             {
                 /* send the success type discovery response */
                        
                  internal_msg.msg_hdr.p_glb_pdb = 
                            p_stun_discovery_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_type_discovery_resp);
    		
                  stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                  stun_type_discovery_resp.error = NAT_ERR_NONE;
                  stun_type_discovery_resp.nat_type = 
                      p_stun_discovery_pdb->p_stun_discovery_context->nat_type;
                    
                  if(NAT_RETURN_SUCCESS != 
                       nat_process_internal_mesg(&internal_msg))
                  {
        				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                 ecode = internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                  }
             }
         }
         
         if((NAT_RETURN_SUCCESS == ret_val) && 
             (NAT_INT_STUN_DISCOVERY_TYPE_REQ == p_internal_msg->msg_hdr.msg_id ||
            NAT_INT_TIMER_EXPIRY_IND == p_internal_msg->msg_hdr.msg_id))
         {
              if(NAT_RETURN_SUCCESS != nat_port_open_udp_socket(
                                        p_stun_discovery_pdb->p_glb_pdb,
                                        &local_address))
              {
                    ret_val = NAT_RETURN_FAILURE;
              }
              else
              {

                  nat_port_memcpy((nat_void_t *)&p_stun_discovery_pdb->
                     p_stun_discovery_context->local_addr, 
                     (nat_void_t *)&local_address, 
                      nat_port_sizeof(nat_ip_port_address_st));
                      
                  NAT_GLB_ALLOC_CONTEXT(
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->p_stun_context_map, 
                    context_id,
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->max_contexts)
                  
                  if(p_stun_discovery_pdb->p_glb_pdb->p_glb_data->max_contexts
                       == context_id)
                  {
                       p_stun_discovery_pdb->p_glb_pdb->p_glb_data->ecode = 
                                NAT_ERR_STUN_NO_CONTEXT_AVAILABLE;
                       ret_val = NAT_RETURN_FAILURE;
                  }
                  else
                  {
                       nat_port_memcpy((nat_void_t *)&(p_stun_discovery_pdb->
                     	p_glb_pdb->p_glb_data->p_stun_context_map[context_id].local_address), 
                     	(nat_void_t *)&local_address, 
                      	nat_port_sizeof(nat_ip_port_address_st));

                  p_stun_discovery_pdb->p_glb_pdb->p_glb_data->
                     p_stun_context_map[context_id].module_id =\
                          NAT_STUN_MODULE_DISCOVERY;
                          
                  /* form the binding request */
                  internal_msg.msg_hdr.p_glb_pdb = p_stun_discovery_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_public_ip_port_req);
			
                  stun_public_ip_port_req.context_id = context_id;
                  
			      nat_port_memcpy( &stun_public_ip_port_req.local_addr,
					   &local_address,nat_port_sizeof(nat_ip_port_address_st));
					   
                  nat_port_memcpy( &stun_public_ip_port_req.destination_addr,
					   &p_stun_discovery_pdb->p_glb_pdb->p_glb_data->
                       p_stun_config_data->stun_server_addr,
                       nat_port_sizeof(nat_ip_port_address_st));
		
			      nat_port_memset( &stun_public_ip_port_req.response_addr,
					   0,nat_port_sizeof(nat_ip_port_address_st));

                stun_public_ip_port_req.change_ip = NAT_FALSE;
                stun_public_ip_port_req.change_port = NAT_FALSE;
                stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                
     			if(NAT_RETURN_SUCCESS != nat_process_internal_mesg(&internal_msg))
       	        {
    				p_stun_discovery_pdb->p_glb_pdb->p_glb_data->ecode = 
                                            internal_msg.msg_hdr.ecode;
       	            ret_val = NAT_RETURN_FAILURE;
                }
                else
                {
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                    nat_type_disc_status = NAT_STUN_NAT_TYPE_DISCOVERY_ONGOING;
                           
                    p_stun_discovery_pdb->p_stun_discovery_context\
                        ->current_state = NAT_STUN_DISCOVERY_01_AWT_RESP;
                }
                  }
             }        
         }
		break;
    
	case NAT_STUN_DISCOVERY_01_AWT_RESP:
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                  nat_port_memcpy((nat_void_t *)&p_stun_discovery_pdb->
                       p_stun_discovery_context->mapped_addr, 
                       (nat_void_t *)&p_stun_public_ip_port_resp->mapped_addr, 
                       nat_port_sizeof(nat_ip_port_address_st));
                       
                  nat_port_memcpy((nat_void_t *)&p_stun_discovery_pdb->
                       p_stun_discovery_context->changed_addr, 
                       (nat_void_t *)&p_stun_public_ip_port_resp->changed_addr, 
                       nat_port_sizeof(nat_ip_port_address_st));
                             
                  if(NAT_RETURN_SUCCESS != nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->mapped_addr,
                            p_stun_discovery_pdb->p_stun_discovery_context
                                   ->local_addr,ecode))
                  {
                        /* form the new binding request */
                          internal_msg.msg_hdr.p_glb_pdb = 
                                    p_stun_discovery_pdb->p_glb_pdb;
                          internal_msg.msg_hdr.msg_id = 
                                   NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                          internal_msg.p_msg_data = 
                                 (nat_uint8_t *)(&stun_public_ip_port_req);
            		
                          stun_public_ip_port_req.context_id = context_id;
                          
            		      nat_port_memcpy( &stun_public_ip_port_req.
                               local_addr,&local_address,
                               nat_port_sizeof(nat_ip_port_address_st));
            				   
                          nat_port_memcpy( 
                               &stun_public_ip_port_req.destination_addr,
            				   &p_stun_discovery_pdb->p_glb_pdb->p_glb_data->
                               p_stun_config_data->stun_server_addr,
                               nat_port_sizeof(nat_ip_port_address_st));
        		
                          stun_public_ip_port_req.change_ip = NAT_TRUE;
                          stun_public_ip_port_req.change_port = NAT_TRUE;
                          stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                            
                          if(NAT_RETURN_SUCCESS != 
                               nat_process_internal_mesg(&internal_msg))
                          {
                				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                         ecode = internal_msg.msg_hdr.ecode;
                   	            ret_val = NAT_RETURN_FAILURE;
                          }
                          else
                          {          
                              p_stun_discovery_pdb->
                              p_stun_discovery_context->current_state = 
                                         NAT_STUN_DISCOVERY_02_AWT_RESP;
                          }      
                    }
                    else
                    {
                        p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                        nat_type_disc_status = 
                                     NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                           
                        p_stun_discovery_pdb->p_stun_discovery_context\
                        ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                        
                        p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                    = NAT_TYPE_PRIVATE_NETWORK_ONLY;
                                    
                        /*Start the refresh timer with duration as stored in the   
				          stun discovery context */
				  
                        stun_start_disc_refresh_timer(p_stun_discovery_pdb);
                        
                        /* release the IP/PORT to port layer */
                        nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                             &p_stun_discovery_pdb->p_stun_discovery_context->
                             local_addr);
                             
                        /* send the success type discovery response */
                        
                          internal_msg.msg_hdr.p_glb_pdb = 
                                    p_stun_discovery_pdb->p_glb_pdb;
                          internal_msg.msg_hdr.msg_id = 
                                   NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                          internal_msg.p_msg_data = 
                                 (nat_uint8_t *)(&stun_type_discovery_resp);
            		
                          stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                          stun_type_discovery_resp.error = NAT_ERR_NONE;
                          stun_type_discovery_resp.nat_type = 
                                           NAT_TYPE_PRIVATE_NETWORK_ONLY;
                            
                          if(NAT_RETURN_SUCCESS != 
                               nat_process_internal_mesg(&internal_msg))
                          {
                				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                         ecode = internal_msg.msg_hdr.ecode;
                   	            ret_val = NAT_RETURN_FAILURE;
                          }    
                    }     
                  
              }
              else
              {

		 p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                    nat_type_disc_status = 
                                 NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                       
                    p_stun_discovery_pdb->p_stun_discovery_context\
                    ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                    p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_INVALID;
    		  
                    /* release the IP/PORT to port layer */
                    nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                    /* send the error type discovery response */
                    
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                      stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                      stun_type_discovery_resp.error = 
                          p_stun_public_ip_port_resp->error;
                      stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_INVALID;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }
                }
         }
		
		break;

	case NAT_STUN_DISCOVERY_02_AWT_RESP:
         
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                   p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                        nat_type_disc_status = 
                                     NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                           
                   p_stun_discovery_pdb->p_stun_discovery_context\
                   ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                
                   p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                            = NAT_TYPE_FULL_CONE;
                            
                  /*Start the refresh timer with duration as stored in the   
		          stun discovery context */
		  
                  /* release the IP/PORT to port layer */
                  nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                     &p_stun_discovery_pdb->p_stun_discovery_context->
                     local_addr);
                     
                  /* send the success type discovery response */
                
                  internal_msg.msg_hdr.p_glb_pdb = 
                            p_stun_discovery_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_type_discovery_resp);
    		
                  stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                  stun_type_discovery_resp.error = NAT_ERR_NONE;
                  stun_type_discovery_resp.nat_type = 
                                   NAT_TYPE_FULL_CONE;
                    
                  if(NAT_RETURN_SUCCESS != 
                       nat_process_internal_mesg(&internal_msg))
                  {
        				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                 ecode = internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                  }
              }
              else if(NAT_ERR_STUN_NO_BINDING_RESPONSE == 
                   p_stun_public_ip_port_resp->error)
              {
                      /* form the new binding request */
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_public_ip_port_req);
        		
                      stun_public_ip_port_req.context_id = context_id;
                      
        		      nat_port_memcpy( &stun_public_ip_port_req.
                           local_addr,&local_address,
                           nat_port_sizeof(nat_ip_port_address_st));
        				   
                      nat_port_memcpy( 
                           &stun_public_ip_port_req.destination_addr,
        				   &p_stun_discovery_pdb->p_stun_discovery_context->
                           changed_addr,nat_port_sizeof(nat_ip_port_address_st));
    		
                      stun_public_ip_port_req.change_ip = NAT_FALSE;
                      stun_public_ip_port_req.change_port = NAT_FALSE;
                      stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }
                      else
                      {          
                          p_stun_discovery_pdb->
                          p_stun_discovery_context->current_state = 
                                     NAT_STUN_DISCOVERY_02_01_AWT_RESP;
                      }      
              }
              else
              {
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                    nat_type_disc_status = 
                                 NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                       
                    p_stun_discovery_pdb->p_stun_discovery_context\
                    ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                    p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_INVALID;
    		  
                    /* release the IP/PORT to port layer */
                    nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                    /* send the error type discovery response */
                    
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                      stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                      stun_type_discovery_resp.error = 
                          p_stun_public_ip_port_resp->error;
                      stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_INVALID;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }    
              }
         }
           
		break;

	case NAT_STUN_DISCOVERY_02_01_AWT_RESP:
         
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                  if(NAT_RETURN_SUCCESS != nat_cmn_compare_address(
                            p_stun_public_ip_port_resp->mapped_addr,
                            p_stun_discovery_pdb->p_stun_discovery_context
                                   ->mapped_addr,ecode))
                  {
                       p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                        nat_type_disc_status = 
                                     NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                           
                       p_stun_discovery_pdb->p_stun_discovery_context\
                       ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                       p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_SYMMETRIC;
                                
                      /*Start the refresh timer with duration as stored in the   
    		          stun discovery context */
    		  
                      /* release the IP/PORT to port layer */
                      nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                      /* send the success type discovery response */
                    
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                      stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                      stun_type_discovery_resp.error = NAT_ERR_NONE;
                      stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_SYMMETRIC;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }
                  }
                  else
                  {
                      /* form the new binding request */
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_public_ip_port_req);
        		
                      stun_public_ip_port_req.context_id = context_id;
                      
        		      nat_port_memcpy( &stun_public_ip_port_req.
                           local_addr,&local_address,
                           nat_port_sizeof(nat_ip_port_address_st));
        				   
                      nat_port_memcpy( 
                           &stun_public_ip_port_req.destination_addr,
        				   &p_stun_discovery_pdb->p_stun_discovery_context->
                           changed_addr,nat_port_sizeof(nat_ip_port_address_st));
    		
                      stun_public_ip_port_req.change_ip = NAT_FALSE;
                      stun_public_ip_port_req.change_port = NAT_TRUE;
                      stun_public_ip_port_req.is_refresh_enabled = NAT_FALSE;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }
                      else
                      {          
                          p_stun_discovery_pdb->
                          p_stun_discovery_context->current_state = 
                                     NAT_STUN_DISCOVERY_03_AWT_RESP;
                      } 
                  }
              }
              else
              {
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                      nat_type_disc_status = 
                                 NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                       
                    p_stun_discovery_pdb->p_stun_discovery_context\
                    ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                    p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_INVALID;
    		  
                    /* release the IP/PORT to port layer */
                    nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                    /* send the success type discovery response */
                    
                    internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                    internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                    internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                    stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                    stun_type_discovery_resp.error = 
                          p_stun_public_ip_port_resp->error;
                    stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_INVALID;
                        
                    if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                    {
      				       p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
                           ret_val = NAT_RETURN_FAILURE;
                    }    
              }
         }
         
		break;

	case NAT_STUN_DISCOVERY_03_AWT_RESP:
         
         if(NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP == 
                      p_internal_msg->msg_hdr.msg_id)
         {
              p_stun_public_ip_port_resp =  (stun_get_public_ip_port_resp_st *)
                      p_internal_msg->p_msg_data;     
                      
              if(NAT_RESULT_SUCCESS == p_stun_public_ip_port_resp->result)
              {
                   p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                        nat_type_disc_status = 
                                     NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                           
                   p_stun_discovery_pdb->p_stun_discovery_context\
                   ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                
                   p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                            = NAT_TYPE_RESTRICTED;
                            
                  /*Start the refresh timer with duration as stored in the   
		          stun discovery context */
		  
                  /* release the IP/PORT to port layer */
                  nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                     &p_stun_discovery_pdb->p_stun_discovery_context->
                     local_addr);
                     
                  /* send the success type discovery response */
                
                  internal_msg.msg_hdr.p_glb_pdb = 
                            p_stun_discovery_pdb->p_glb_pdb;
                  internal_msg.msg_hdr.msg_id = 
                           NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                  internal_msg.p_msg_data = 
                         (nat_uint8_t *)(&stun_type_discovery_resp);
    		
                  stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                  stun_type_discovery_resp.error = NAT_ERR_NONE;
                  stun_type_discovery_resp.nat_type = 
                                   NAT_TYPE_RESTRICTED;
                    
                  if(NAT_RETURN_SUCCESS != 
                       nat_process_internal_mesg(&internal_msg))
                  {
        				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                 ecode = internal_msg.msg_hdr.ecode;
           	            ret_val = NAT_RETURN_FAILURE;
                  }
              }
              else if(NAT_ERR_STUN_NO_BINDING_RESPONSE == 
                   p_stun_public_ip_port_resp->error)
              {
                      p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                    nat_type_disc_status = 
                                 NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                       
                    p_stun_discovery_pdb->p_stun_discovery_context\
                    ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                    p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_PORT_RESTRICTED;
    		  
                    /* release the IP/PORT to port layer */
                    nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                    /* send the error type discovery response */
                    
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                      stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                      stun_type_discovery_resp.error = NAT_ERR_NONE;
                      stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_PORT_RESTRICTED;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }        
              }
              else
              {
                    p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
                    nat_type_disc_status = 
                                 NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
                       
                    p_stun_discovery_pdb->p_stun_discovery_context\
                    ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
                    
                    p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                                = NAT_TYPE_INVALID;
    		  
                    /* release the IP/PORT to port layer */
                    nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
                         &p_stun_discovery_pdb->p_stun_discovery_context->
                         local_addr);
                         
                    /* send the error type discovery response */
                    
                      internal_msg.msg_hdr.p_glb_pdb = 
                                p_stun_discovery_pdb->p_glb_pdb;
                      internal_msg.msg_hdr.msg_id = 
                               NAT_INT_STUN_DISCOVERY_TYPE_RESP;
                      internal_msg.p_msg_data = 
                             (nat_uint8_t *)(&stun_type_discovery_resp);
        		
                      stun_type_discovery_resp.result = NAT_RESULT_SUCCESS;
                      stun_type_discovery_resp.error = 
                          p_stun_public_ip_port_resp->error;
                      stun_type_discovery_resp.nat_type = 
                                       NAT_TYPE_INVALID;
                        
                      if(NAT_RETURN_SUCCESS != 
                           nat_process_internal_mesg(&internal_msg))
                      {
            				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                                     ecode = internal_msg.msg_hdr.ecode;
               	            ret_val = NAT_RETURN_FAILURE;
                      }    
              }
         }

		break;

	default:
		ret_val = NAT_RETURN_FAILURE;

	}

    
    if(NAT_RETURN_FAILURE == ret_val)
    {
        p_stun_discovery_pdb->p_glb_pdb->p_glb_data->\
        nat_type_disc_status = 
                     NAT_STUN_NAT_TYPE_DISCOVERY_COMPLETE;
           
        p_stun_discovery_pdb->p_stun_discovery_context\
        ->current_state = NAT_STUN_DISCOVERY_00_IDLE;
        
        p_stun_discovery_pdb->p_stun_discovery_context->nat_type  
                    = NAT_TYPE_INVALID;
  
        /* release the IP/PORT to port layer */
        nat_port_close_udp_socket(p_stun_discovery_pdb->p_glb_pdb,
             &p_stun_discovery_pdb->p_stun_discovery_context->
             local_addr);
             
        /* send the error type discovery response */
        
          internal_msg.msg_hdr.p_glb_pdb = 
                    p_stun_discovery_pdb->p_glb_pdb;
          internal_msg.msg_hdr.msg_id = 
                   NAT_INT_STUN_DISCOVERY_TYPE_RESP;
          internal_msg.p_msg_data = 
                 (nat_uint8_t *)(&stun_type_discovery_resp);
	
          stun_type_discovery_resp.result = NAT_RESULT_FAILURE;
          stun_type_discovery_resp.error = internal_msg.msg_hdr.ecode; 
              
          stun_type_discovery_resp.nat_type = 
                           NAT_TYPE_INVALID;
            
          if(NAT_RETURN_SUCCESS != 
               nat_process_internal_mesg(&internal_msg))
          {
				p_stun_discovery_pdb->p_glb_pdb->p_glb_data-> 
                         ecode = internal_msg.msg_hdr.ecode;
   	            ret_val = NAT_RETURN_FAILURE;
          }        
          else
              ret_val = NAT_RETURN_SUCCESS;            
    }
    return ret_val;
}

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
		INOUT   nat_internal_msg_st        *p_internal_msg)     
{
    stun_discovery_pdb_st		 stun_discovery_pdb;
    nat_glb_pdb_st		*p_glb_pdb=p_internal_msg->msg_hdr.p_glb_pdb;
    nat_error_t          ecode;
    
 
    stun_discovery_pdb.p_glb_pdb = p_glb_pdb;
    stun_discovery_pdb.p_stun_discovery_context=
			((stun_discovery_data_st *)stun_discovery_pdb.p_glb_pdb->p_glb_data
                             ->p_stun_discovery_data)->p_stun_discovery_context;
    
    stun_stop_disc_refresh_timer(&stun_discovery_pdb);
   
    NAT_GLB_DEALLOC_CONTEXT(p_glb_pdb->p_glb_data->
            p_stun_context_map[NAT_STUN_MODULE_DISCOVERY])
    
    nat_port_static_memfree(((stun_discovery_data_st *)p_glb_pdb->
           p_glb_data->p_stun_discovery_data)->p_stun_discovery_context, &ecode);
                    
    nat_port_static_memfree(p_glb_pdb->p_glb_data->p_stun_discovery_data, &ecode);
    p_glb_pdb->p_glb_data->p_stun_discovery_data = NAT_NULL;                
    return;
}



                      
