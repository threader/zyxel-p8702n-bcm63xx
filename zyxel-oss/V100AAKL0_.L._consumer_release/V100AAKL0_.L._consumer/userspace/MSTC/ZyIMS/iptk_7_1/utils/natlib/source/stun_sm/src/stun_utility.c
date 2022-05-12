/******************************************************************************
 * 
 * FILE NAME:   stun_utility.c
 * 
 * DESCRIPTION: This file contains the utitlity functions to be used by
 *              stun binding request state machine.
 *              
 * REVISION HISTORY:    
 *      Date            Name            Reference       Comments
 *  18-July-2006    Anuradha Gupta       LLD            Initial Version +Coding
 *	24-Nov-2006	    Anuradha Gupta       LLD			Coding
 *	14-Oct-2009     Anurag Khare         SPR 20145      Fix for CSR 1-7873034
 *
 * Copyright 2009, Aricent                
 * ******************************************************************************/ 
 
#include "stun_prototypes.h"
#include "nat_error_id.h"
#include "nat_common_internal_api_id.h"
#include "nat_common_internal_api.h"
#include "stun_enc_dec_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "stun_types.h"
#include "stun_macros.h"
#include "nat_port_prototypes.h"
#include "nat_glb_mgmt_macro.h"
#include "stun_enc_dec_prototypes.h"
#include "nat_glb_mgmt_prototypes.h"

nat_duration_t  retransmit_timer_val[] = {
                100,200,400,800,1600,1600,1600,1600,1600
                };
extern nat_glb_pdb_st *p_nat_global_pdb;

/*******************************************************************************
 *
 * FUNCTION:        stun_get_retransmission_timer_value
 *
 * DESCRIPTION:     This function is used to get the re
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_get_retransmission_timer_value(
		IN  nat_uint8_t retransmit_counter, 
        OUT nat_duration_t *p_retransmit_timer_value)
        
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    
    *p_retransmit_timer_value = (nat_uint16_t)((retransmit_timer_val[retransmit_counter])*
		(p_nat_global_pdb->p_glb_data->p_stun_config_data->scale_timer_value));
    
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_stop_current_timers
 *
 * DESCRIPTION:     This function is used to stop all running timers.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_current_timers(
		INOUT stun_pdb_st             *p_stun_pdb)
        
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_timer_id_t      timer_id;
    nat_uint8_t                      ctr = 0;
    nat_error_t		ecode = NAT_ERR_NONE;
    
    for(ctr = 0; ctr < STUN_MAX_BIND_TIMERS; ctr++)
    {
        if(NAT_TIMER_STATUS_VALID == p_stun_pdb->p_stun_context->
            current_timer_info[ctr].timer_status)
        {
            timer_id = p_stun_pdb->p_stun_context->
                current_timer_info[ctr].timer_id;
            
            ret_val = nat_port_stop_timer(p_stun_pdb->p_glb_pdb,timer_id);
            
            if(NAT_RETURN_PENDING == ret_val || 
                NAT_RETURN_SUCCESS == ret_val)
            {
                p_stun_pdb->p_stun_context->current_timer_info
                    [ctr].timer_status = NAT_TIMER_STATUS_INVALID;
                
                p_stun_pdb->p_stun_context->current_timer_info
                    [ctr].timer_id = NAT_INVALID_TIMER_ID;

		nat_port_static_memfree(
			p_stun_pdb->p_stun_context->current_timer_info
			[ctr].p_timer_data,&ecode);
            }
            else
            {
                p_stun_pdb->p_glb_pdb->p_glb_data->ecode
                    = NAT_ERR_TMR_STOP_FAILURE;
                ret_val = NAT_RETURN_FAILURE; 
                break;
            }
        }
    }
    
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_stop_timer
 *
 * DESCRIPTION:     This function is used to stop the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_stop_timer(
		INOUT stun_pdb_st             *p_stun_pdb,
        IN    nat_timer_type_t         timer_type)
        
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_timer_id_t      timer_id;
    nat_error_t		ecode = NAT_ERR_NONE;
    
    if(NAT_TIMER_STATUS_VALID == p_stun_pdb->p_stun_context->
                               current_timer_info[timer_type].timer_status)
    {
         timer_id = p_stun_pdb->p_stun_context->
                               current_timer_info[timer_type].timer_id;
                               
         ret_val = nat_port_stop_timer(p_stun_pdb->p_glb_pdb,timer_id);
         
         if(NAT_RETURN_PENDING == ret_val || 
                     NAT_RETURN_SUCCESS == ret_val)
         {
              p_stun_pdb->p_stun_context->current_timer_info
                       [timer_type].timer_status = NAT_TIMER_STATUS_INVALID;
                          
              p_stun_pdb->p_stun_context->current_timer_info
                       [timer_type].timer_id = NAT_INVALID_TIMER_ID;

		nat_port_static_memfree(
			p_stun_pdb->p_stun_context->current_timer_info
			[timer_type].p_timer_data,&ecode);
         }
         else
         {
             p_stun_pdb->p_glb_pdb->p_glb_data->ecode
                      = NAT_ERR_TMR_STOP_FAILURE;
             ret_val = NAT_RETURN_FAILURE; 
         }
    }
    
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_start_timer
 *
 * DESCRIPTION:     This function is used to start the asked timer
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_start_timer(
		INOUT stun_pdb_st             *p_stun_pdb,
		IN    nat_uint8_t             context_id,
        	IN    nat_timer_type_t         timer_type,
        	IN    nat_duration_t           timer_dur)
        
{ 
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_timer_id_t      timer_id;
    nat_error_t 	ecode = NAT_ERR_NONE;

    nat_timer_data_st   *p_timer_data = 
		(nat_timer_data_st *)nat_port_static_memget(nat_port_sizeof(nat_timer_data_st),
		&ecode);

    p_timer_data->context_id = context_id;
    p_timer_data->timer_type =  timer_type;

    
    ret_val = nat_port_start_timer(p_stun_pdb->p_glb_pdb,
                    p_timer_data,timer_dur,&timer_id);
    
    if(NAT_RETURN_PENDING == ret_val)
    {
         NAT_STUN_SET_STATE(p_stun_pdb,NAT_STUN_03_AWT_START_TIMER_RESP)
                             
         p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_id
              = NAT_INVALID_TIMER_ID;
         p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_status
              = NAT_TIMER_STATUS_PENDING;
	p_stun_pdb->p_stun_context->current_timer_info[timer_type].p_timer_data 	     = p_timer_data;
    }
    else if(NAT_RETURN_SUCCESS == ret_val)
    {
         p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_id
              = timer_id;
         p_stun_pdb->p_stun_context->current_timer_info[timer_type].timer_status
              = NAT_TIMER_STATUS_VALID;
	p_stun_pdb->p_stun_context->current_timer_info[timer_type].p_timer_data 	     = p_timer_data;
    }
    else
    {
	nat_port_static_memfree(p_timer_data,&ecode);
        p_stun_pdb->p_glb_pdb->p_glb_data->ecode = NAT_ERR_TMR_START_FAILURE;
    }
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_create_and_send_binding_request
 *
 * DESCRIPTION:     This function is used to create / encode the binding request
 *                  message and sending it to network.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_create_and_send_binding_request(
		INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_internal_msg_st             internal_msg;
    nat_internal_msg_st             *p_internal_msg = 
                                        p_stun_pdb->p_internal_msg;
    stun_get_public_ip_port_resp_st public_ip_port_resp;
	
	stun_message_st        stun_message;
	nat_octet_string_st		*p_buffer;
	nat_error_t            ecode = NAT_ERR_NONE;
	nat_uint128_t          transaction_id;
	nat_duration_t         retransmit_timer_value;
                                   
    stun_get_public_ip_port_req_st *p_public_ip_port_req = 
           (stun_get_public_ip_port_req_st *)p_internal_msg->p_msg_data;
           		
    if(NAT_RETURN_SUCCESS != stun_create_bind_req(&stun_message,
                        p_stun_pdb->p_stun_context->shared_secret_username,
                        p_public_ip_port_req->change_ip,
                        p_public_ip_port_req->change_port,
                        &p_public_ip_port_req->response_addr))
    {
        ret_val  = NAT_RETURN_FAILURE;
        return ret_val;
    }
    else
    {
	nat_uint8_t msg_integrity[NAT_STUN_HMAC_DIGEST_LENGTH+1];
     
        nat_port_memset(msg_integrity,'\0',NAT_STUN_HMAC_DIGEST_LENGTH+1);

 	p_buffer = (nat_octet_string_st *)nat_port_static_memget(
        		  nat_port_sizeof(nat_octet_string_st), &ecode);
                                               
         if(NAT_RETURN_SUCCESS != stun_encode_message(stun_message,p_buffer,
                         p_stun_pdb->p_stun_context->shared_secret_password,
                         ecode,(nat_uint8_t *)msg_integrity))
         {
                internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
                internal_msg.msg_hdr.msg_id = NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
                internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);
        
                public_ip_port_resp.result = NAT_RESULT_FAILURE;
                public_ip_port_resp.error = NAT_ERR_ENCODE_FAILED;
                
                
                if(NAT_RETURN_SUCCESS != nat_process_internal_mesg(&internal_msg))
                {
        			p_stun_pdb->p_glb_pdb->p_glb_data-> 
                             ecode = internal_msg.msg_hdr.ecode;
                    ret_val = NAT_RETURN_FAILURE; 
                }
                
                nat_port_static_memfree(p_buffer,&ecode);
                return ret_val;
         }
         else
         {

             p_stun_pdb->p_stun_context->p_enc_message = p_buffer;
             
             nat_port_memcpy(&transaction_id,&stun_message.msg_hdr.transaction_id,
                 nat_port_sizeof(nat_uint128_t));
                
			nat_port_memcpy(&(p_stun_pdb->p_glb_pdb->p_glb_data->\
				p_stun_context_map[p_public_ip_port_req->context_id].transaction_id),\
			&transaction_id,nat_port_sizeof(nat_uint128_t));   

		/*msg_integrity[0] = '\0';*/
		p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map\
			[p_public_ip_port_req->context_id].message_integrity[0] = '\0';
	
		nat_port_strcpy(p_stun_pdb->p_glb_pdb->p_glb_data->\
			p_stun_context_map[p_public_ip_port_req->context_id].message_integrity,
			msg_integrity);

		 p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map[p_public_ip_port_req->context_id].message_integrity[NAT_STUN_HMAC_DIGEST_LENGTH] = '\0';

		if(NAT_STUN_MODULE_EXTERNAL == p_stun_pdb->p_glb_pdb->p_glb_data->
                     p_stun_context_map[p_public_ip_port_req->context_id].module_id)
			 {

				 if(NAT_RETURN_SUCCESS != nat_port_send_mm_data(p_stun_pdb->p_glb_pdb,
									   &p_public_ip_port_req->local_addr,
									   &p_public_ip_port_req->destination_addr,
									   p_buffer))
				 {
					 p_stun_pdb->p_glb_pdb->p_glb_data-> 
								 ecode = NAT_ERR_UDP_SEND;
					 ret_val = NAT_RETURN_FAILURE; 
					 nat_port_static_memfree(p_buffer,&ecode);
					 return ret_val;
				 }
			 }
			 else 
			 {
				 if(NAT_RETURN_SUCCESS != nat_port_send_udp_data(p_stun_pdb->p_glb_pdb,
									   &p_public_ip_port_req->local_addr,
									   &p_public_ip_port_req->destination_addr,
									   p_buffer))
				 {
					 p_stun_pdb->p_glb_pdb->p_glb_data-> 
								 ecode = NAT_ERR_UDP_SEND;
					 ret_val = NAT_RETURN_FAILURE; 
					 nat_port_static_memfree(p_buffer,&ecode);
					 return ret_val;
				 }
			 }

             if(NAT_RETURN_SUCCESS == ret_val)
             {

				nat_port_memcpy(&p_stun_pdb->p_stun_context->destination_addr,
								&p_public_ip_port_req->destination_addr,
								nat_port_sizeof(nat_ip_port_address_st));

                 p_stun_pdb->p_stun_context->retransmission_counter = 0;
                 
                 stun_get_retransmission_timer_value(
                     p_stun_pdb->p_stun_context->retransmission_counter,
                     &retransmit_timer_value);
                   
                 ret_val = stun_start_timer(p_stun_pdb,p_public_ip_port_req->context_id,
                                         NAT_STUN_BIND_RETRANSMIT_TIMER,
                     			retransmit_timer_value);
                            
                 if(NAT_RETURN_PENDING == ret_val)
                 {
                    /* asynchrounos timer, set the next state for awt timer 
                       resp */
                          
                     NAT_STUN_SET_STATE(p_stun_pdb,
                                         NAT_STUN_03_AWT_START_TIMER_RESP)

                       NAT_STUN_SET_NEXT_STATE(p_stun_pdb,
                             NAT_STUN_01_AWT_BIND_RESP)
                             
                 }
                 else if(NAT_RETURN_SUCCESS == ret_val)
                 {
                      /* synchronous timer */
                      if(NAT_STUN_00_IDLE == 
                          NAT_STUN_GET_STATE(p_stun_pdb))
                      {
                          NAT_STUN_SET_STATE(p_stun_pdb,
                                         NAT_STUN_01_AWT_BIND_RESP)
                      } 
                 }
             }
         }  
    } 
            
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_retransmit_binding_request
 *
 * DESCRIPTION:     This function is used to retranmit the binding request.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_retransmit_binding_request(
		INOUT stun_pdb_st             *p_stun_pdb,
        IN    nat_uint8_t             context_id)
{
    nat_return_t           ret_val = NAT_RETURN_SUCCESS;
	
	nat_duration_t         retransmit_timer_value;
	nat_error_t				ecode = NAT_ERR_UNDEFINED;
       
    if(NAT_STUN_MODULE_EXTERNAL == p_stun_pdb->p_glb_pdb->p_glb_data->
                     p_stun_context_map[context_id].module_id)
	 {

		 if(NAT_RETURN_SUCCESS != nat_port_send_mm_data(p_stun_pdb->p_glb_pdb,
					&p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map
					[context_id].local_address,
					&p_stun_pdb->p_stun_context->destination_addr,
					p_stun_pdb->p_stun_context->p_enc_message))
		 {
			 p_stun_pdb->p_glb_pdb->p_glb_data-> 
						 ecode = NAT_ERR_UDP_SEND;
			 ret_val = NAT_RETURN_FAILURE; 
			 nat_port_static_memfree(p_stun_pdb->p_stun_context->p_enc_message
				 ,&ecode);
			 return ret_val;
		 }
	 }
	 else 
	 {
		 if(NAT_RETURN_SUCCESS != nat_port_send_udp_data(p_stun_pdb->p_glb_pdb,
					&p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map
					[context_id].local_address,
					&p_stun_pdb->p_stun_context->destination_addr,
					p_stun_pdb->p_stun_context->p_enc_message))
		 {
			 p_stun_pdb->p_glb_pdb->p_glb_data-> 
						 ecode = NAT_ERR_UDP_SEND;
			 ret_val = NAT_RETURN_FAILURE; 
			 nat_port_static_memfree(p_stun_pdb->p_stun_context->p_enc_message
				 ,&ecode);
			 return ret_val;
		 }
	 }

    if(NAT_RETURN_SUCCESS == ret_val)
     {
         stun_get_retransmission_timer_value(
             p_stun_pdb->p_stun_context->retransmission_counter,
             &retransmit_timer_value);
         /*  Fix for CSR 1-7873034 : SPR 20145 */
         /*  Free the memory of timer buffer because now a new retransmission
          *  timer is going to get started. 
          */
         nat_port_static_memfree(p_stun_pdb->p_stun_context->
               current_timer_info[NAT_STUN_BIND_RETRANSMIT_TIMER].p_timer_data,
               &ecode);
         ret_val = stun_start_timer(p_stun_pdb,
             context_id,NAT_STUN_BIND_RETRANSMIT_TIMER,
             retransmit_timer_value);
             
         if(NAT_RETURN_PENDING == ret_val)
         {
            /* asynchrounos timer, set the next state for awt timer 
               resp */
                    
               NAT_STUN_SET_STATE(p_stun_pdb,
                     NAT_STUN_03_AWT_START_TIMER_RESP)
                  
               NAT_STUN_SET_NEXT_STATE(p_stun_pdb,
                     NAT_STUN_01_AWT_BIND_RESP)
                     
               p_stun_pdb->p_stun_context->current_timer_info
               [NAT_STUN_BIND_RETRANSMIT_TIMER].timer_status =
                              NAT_TIMER_STATUS_PENDING;
                              
               p_stun_pdb->p_stun_context->current_timer_info
               [NAT_STUN_BIND_RETRANSMIT_TIMER].timer_id =
                              NAT_INVALID_TIMER_ID;
         }
         else if(NAT_RETURN_SUCCESS == ret_val)
         {    
                NAT_STUN_SET_STATE(p_stun_pdb,
                     NAT_STUN_01_AWT_BIND_RESP)

               p_stun_pdb->p_stun_context->current_timer_info
               [NAT_STUN_BIND_RETRANSMIT_TIMER].timer_status =
                              NAT_TIMER_STATUS_VALID;
                              
         }
         else
         {
             p_stun_pdb->p_glb_pdb->p_glb_data->ecode
                      = NAT_ERR_TMR_START_FAILURE;
             ret_val = NAT_RETURN_FAILURE; 
         }
     }
               
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_map_response_code_to_error_cause
 *
 * DESCRIPTION:     This function is to map the network response code to 
 *                  application specific error codes.
 *                  
 * RETURNS:         void                 
 *
 ******************************************************************************/
nat_void_t stun_map_response_code_to_error_cause(
		IN   nat_error_t response_code,
        OUT  nat_error_t *p_error_cause)
        
{
    nat_error_t resp_code = 0;
    nat_error_t error_cause = 0;

    if(500 <= response_code && response_code <= 599)
    {
           resp_code = 500;
    }
    else if(600 <= response_code && response_code <= 699)
    {
           resp_code = 600;
    }
    else if(400 <= response_code && response_code <= 499)
    {
	switch(response_code)
	{
	case 401:
	case 420:
	case 430:
	case 431:
	case 432:
	case 433:
		resp_code = response_code;
		break;
	default:
		resp_code = 400;
		break;
	}
    }
    
    switch(resp_code)
    {
    case 400:
         error_cause = NAT_ERR_STUN_BAD_REQUEST;
         break;
         
    case 401:
         error_cause = NAT_ERR_STUN_UNAUTH_RETRY;
         /* In this case the upper layer application should first
	  * execute the shared secret algo and then send
	  * the Binding Request again
          */
         break;
         
    case 420:
         error_cause = NAT_ERR_STUN_UNKNOWN_ATTR_RETRY;
         /* In this case the upper layer application should send
	  * the Binding Request again. Also if this error happens 
	  * repeatedly the application can try the following :
	  *
   	  * a) Change the stun server address
 	  * b) Execute the Shared Secret Algo again
	  * c) In worse case the application should deinit stun client 
	  * and is incapable of NAT traversal via STUN.
	  */
         break;
         
    case 430:
         error_cause = NAT_ERR_STUN_STALE_CREDIT_RETRY;
         /* In this case the upper layer application should first
	  * execute the shared secret algo again and then send
	  * the Binding Request again 
	  */
         break;
         
    case 431:
         error_cause = NAT_ERR_STUN_INTEG_CHECK_FAIL_RETRY;
         /* In this case the upper layer application should first
	  * execute the shared secret algo again and then send
	  * the Binding Request again. Also if this error happen
	  * repeatedly the application can try the following :
	  *
 	  * a) Execute the Shared Secret Algo again
	  * b) In worse case the application should deinit stun client 
	  * and is incapable of NAT traversal via STUN. 
	  */
         break;
         
    case 432:
         error_cause = NAT_ERR_STUN_MISSING_USRNAME_RETRY;
         /* In this case the upper layer application should first
	  * execute the shared secret algo and then send
	  * the Binding Request again 
	  */
         break;
         
    case 500:
         error_cause = NAT_ERR_STUN_SERVER_ERROR_RETRY;
         /* In this case the application should retry after some time.
 	  * This does not mandate the application to run Shared Secret 
	  * Algo again.
	  * Also if this error happens 
	  * repeatedly the application can try the following :
	  *
	  * a) MAY Change the stun server address
 	  * b) MAY Execute the Shared Secret Algo again
	  * c) In worse case the application should deinit stun client 
	  * and is incapable of NAT traversal via STUN.
	  */
         break;
         
    case 600:
         error_cause = NAT_ERR_STUN_GLOBAL_FAILURE_NO_RETRY;
         /* In this case the application should not retry at all and should deinit
	  * stun client is not capable of NAT traversal via STUN. */
         break;
         
    default:
         break;
            
    }

	*p_error_cause = error_cause;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_pending_timer_info
 *
 * DESCRIPTION:     This function returns the timer status for pending timers.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS : some timer response is pending
 *                  NAT_RETURN_FAILURE : no timer response is pending                 
 *
 ******************************************************************************/
nat_return_t stun_pending_timer_info(
        INOUT stun_pdb_st             *p_stun_pdb)
        
{
     nat_return_t     ret_val = NAT_RETURN_FAILURE;
     nat_uint8_t      i = 0;
     
     for(i = 0; i < STUN_MAX_BIND_TIMERS; i++)
     {
           if(p_stun_pdb->p_stun_context->current_timer_info[i].timer_status
                 == NAT_TIMER_STATUS_PENDING)
           {
                 return NAT_RETURN_SUCCESS;
           }
     }   
     return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_process_initial_binding_success_response
 *
 * DESCRIPTION:     This function is used to process the initial binding 
 *                  success response.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_process_initial_binding_success_response(
		INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    stun_binding_resp_st    *p_binding_resp = 
           (stun_binding_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
           
    nat_internal_msg_st             internal_msg;
    stun_get_public_ip_port_resp_st public_ip_port_resp;
     
              
     nat_port_memcpy(&p_stun_pdb->p_stun_context->mapped_addr,
                     &p_binding_resp->mapped_addr,
                     nat_port_sizeof(nat_ip_port_address_st));
                     
     nat_port_memcpy(&p_stun_pdb->p_stun_context->changed_addr,
                     &p_binding_resp->changed_addr,
                     nat_port_sizeof(nat_ip_port_address_st));
                     
     p_stun_pdb->p_stun_context->received_counter = 1;
     
     /* start the gaurd timer */
     ret_val = stun_start_timer(p_stun_pdb,
                   p_binding_resp->context_id,
                   NAT_STUN_BIND_GUARD_TIMER,
                   NAT_STUN_BIND_GUARD_TIMER_VALUE);
                   
     if(NAT_RETURN_PENDING == ret_val)
     {
         NAT_STUN_SET_NEXT_STATE(p_stun_pdb,NAT_STUN_02_AWT_EXTRA_BIND_RESP)                    
     }
     else if(NAT_RETURN_SUCCESS == ret_val)
     {     
         NAT_STUN_SET_STATE(p_stun_pdb,NAT_STUN_02_AWT_EXTRA_BIND_RESP) 
     }
     else
     {
         return ret_val;
     }
     
     /* start the refresh timer if the flag is enabled */
     if(NAT_TRUE == p_stun_pdb->p_stun_context->is_refresh_enabled)
     {
          ret_val = stun_start_timer(p_stun_pdb,
                   p_binding_resp->context_id,
                   NAT_STUN_BIND_REFRESH_TIMER,
                   (nat_uint16_t)((p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_config_data->
                       binding_refresh_timer) * 1000));
                   
         if(NAT_RETURN_PENDING == ret_val)
         {
             NAT_STUN_SET_NEXT_STATE(p_stun_pdb,
                              NAT_STUN_02_AWT_EXTRA_BIND_RESP)                    
         }
         else if(NAT_RETURN_SUCCESS == ret_val)
         {     
             NAT_STUN_SET_STATE(p_stun_pdb,NAT_STUN_02_AWT_EXTRA_BIND_RESP) 
         }
         else
         {
             return ret_val;
         }
     }
     
     /* send successful binding response to calling module */
     internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
     internal_msg.msg_hdr.msg_id =
                     NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
     internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);
             
     public_ip_port_resp.result = NAT_RETURN_SUCCESS;
     public_ip_port_resp.context_id = p_binding_resp->context_id;
     
     /* copy local address */
     nat_port_memcpy(&public_ip_port_resp.local_addr,
           &p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map
              [p_binding_resp->context_id].local_address,
           nat_port_sizeof(nat_ip_port_address_st));
           
     /* copy mapped address */ 
     nat_port_memcpy(&public_ip_port_resp.mapped_addr,
           &p_stun_pdb->p_stun_context->mapped_addr,
           nat_port_sizeof(nat_ip_port_address_st));
      
     /* copy changed address */      
     nat_port_memcpy(&public_ip_port_resp.local_addr,
           &p_stun_pdb->p_stun_context->changed_addr,
           nat_port_sizeof(nat_ip_port_address_st));
 
     if(NAT_RETURN_SUCCESS != 
            nat_process_internal_mesg(&internal_msg))
     {
            ret_val = NAT_RETURN_FAILURE; 
            return ret_val;
     }                                 
   
    return ret_val;
}
    
/*******************************************************************************
 *
 * FUNCTION:        stun_process_binding_error_response
 *
 * DESCRIPTION:     This function is used to process the binding 
 *                  error response.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_process_binding_error_response(
		INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    stun_binding_resp_st    *p_binding_resp = 
           (stun_binding_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
           
    nat_internal_msg_st             internal_msg;
    stun_get_public_ip_port_resp_st public_ip_port_resp;
    nat_error_t                     error_cause =NAT_ERR_UNDEFINED;
    
     if(p_binding_resp->error >= 400 && p_binding_resp->error < 700)
     {
          stun_map_response_code_to_error_cause(p_binding_resp->error,
                &error_cause);
     }
     else if(NAT_ERR_MSG_INTEGRITY_FAIL == 
                                       p_binding_resp->error)
     {
          error_cause = NAT_ERR_STUN_FAILURE_POSSIBLE_ATTACK;
     }
     else if(NAT_ERR_STUN_DECODE_MSG_VALIDATION_FAIL ==
					p_binding_resp->error)
     {
          error_cause = NAT_ERR_STUN_DECODE_MSG_VALIDATION_FAIL;
     }
	 
      
     /* send failure binding response to calling module */
     internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
     internal_msg.msg_hdr.msg_id =
                     NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
     internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);
             
     public_ip_port_resp.result = NAT_RETURN_FAILURE;
     public_ip_port_resp.error = error_cause;
     public_ip_port_resp.context_id = p_binding_resp->context_id;
 
     if(NAT_RETURN_SUCCESS != 
            nat_process_internal_mesg(&internal_msg))
     {
            ret_val = NAT_RETURN_FAILURE; 
     }  
     
     NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)
     NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
         p_stun_context_map[p_binding_resp->context_id])
                 
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_create_and_send_binding_refresh_request
 *
 * DESCRIPTION:     This function is used to create / encode the binding request
 *                  message (refresh) and sending it to network.
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
nat_return_t stun_create_and_send_binding_refresh_request(
		INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;
    nat_internal_msg_st             *p_internal_msg = 
                                        p_stun_pdb->p_internal_msg;
	
	stun_message_st        stun_message;
	nat_octet_string_st		*p_buffer;
	nat_error_t            ecode = NAT_NULL;
	nat_uint128_t          transaction_id;
	nat_duration_t         retransmit_timer_value;
	nat_ip_port_address_st  response_addr;
                                  
    stun_timer_expire_ind_st *p_timer_expire_ind = 
           (stun_timer_expire_ind_st *)p_internal_msg->p_msg_data;

    nat_uint8_t  context_id = p_timer_expire_ind->context_id;

	response_addr.port = 0;
           		
    if(NAT_RETURN_SUCCESS != stun_create_bind_req(&stun_message,
                        p_stun_pdb->p_stun_context->shared_secret_username,
                        NAT_FALSE,NAT_FALSE,&response_addr))
    {
        ret_val  = NAT_RETURN_FAILURE;
        return ret_val;
    }
    else
    {
	    nat_uint8_t msg_integrity[NAT_STUN_HMAC_DIGEST_LENGTH];

 	    p_buffer = (nat_octet_string_st *)nat_port_static_memget(
        		  nat_port_sizeof(nat_octet_string_st), &ecode);
            nat_port_memset(msg_integrity,0,NAT_STUN_HMAC_DIGEST_LENGTH);        
         if(NAT_RETURN_SUCCESS != stun_encode_message(stun_message,p_buffer,
                         p_stun_pdb->p_stun_context->shared_secret_password,
                         ecode,(nat_uint8_t *)&msg_integrity))
         {
               
                nat_port_static_memfree(p_buffer,&ecode);
                return ret_val;
         }
         else
         {

             p_stun_pdb->p_stun_context->p_enc_message = p_buffer;
             
             nat_port_memcpy(&transaction_id,&stun_message.msg_hdr.transaction_id,
                 nat_port_sizeof(nat_uint128_t));
                
			nat_port_memcpy(&(p_stun_pdb->p_glb_pdb->p_glb_data->\
				p_stun_context_map[context_id].transaction_id),\
			    &transaction_id,nat_port_sizeof(nat_uint128_t));
		
		    nat_port_strcpy(p_stun_pdb->p_glb_pdb->p_glb_data->\
			    p_stun_context_map[context_id].message_integrity,
			    msg_integrity);

		    p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map[context_id].
                message_integrity[NAT_STUN_HMAC_DIGEST_LENGTH] = '\0';

		if(NAT_STUN_MODULE_EXTERNAL == p_stun_pdb->p_glb_pdb->p_glb_data->
                     p_stun_context_map[context_id].module_id)
			 {

				 if(NAT_RETURN_SUCCESS != 
                        nat_port_send_mm_data(p_stun_pdb->p_glb_pdb,
							&(p_stun_pdb->p_glb_pdb->p_glb_data->
                             p_stun_context_map[context_id].local_address),
							&(p_stun_pdb->p_stun_context->destination_addr),
							p_buffer))
				 {
					 p_stun_pdb->p_glb_pdb->p_glb_data-> 
								 ecode = NAT_ERR_UDP_SEND;
					 ret_val = NAT_RETURN_FAILURE; 
					 nat_port_static_memfree(p_buffer,&ecode);
					 return ret_val;
				 }
			 }
			 else 
			 {
				 if(NAT_RETURN_SUCCESS != 
                        nat_port_send_udp_data(p_stun_pdb->p_glb_pdb,
							&(p_stun_pdb->p_glb_pdb->p_glb_data->
                             p_stun_context_map[context_id].local_address),
							&(p_stun_pdb->p_stun_context->destination_addr),
							p_buffer))
				 {
					 p_stun_pdb->p_glb_pdb->p_glb_data-> 
								 ecode = NAT_ERR_UDP_SEND;
					 ret_val = NAT_RETURN_FAILURE; 
					 nat_port_static_memfree(p_buffer,&ecode);
					 return ret_val;
				 }
			 }

             if(NAT_RETURN_SUCCESS == ret_val)
             {

                 p_stun_pdb->p_stun_context->retransmission_counter = 0;
                 
                 stun_get_retransmission_timer_value(
                     p_stun_pdb->p_stun_context->retransmission_counter,
                     &retransmit_timer_value);
                   
                 ret_val = stun_start_timer(p_stun_pdb,context_id,
                                         NAT_STUN_BIND_RETRANSMIT_TIMER,
                     			retransmit_timer_value);
                            
                 if(NAT_RETURN_PENDING == ret_val)
                 {
                    /* asynchrounos timer, set the next state for awt timer 
                       resp */
                          
                     NAT_STUN_SET_STATE(p_stun_pdb,
                                         NAT_STUN_03_AWT_START_TIMER_RESP)

                       NAT_STUN_SET_NEXT_STATE(p_stun_pdb,
                             NAT_STUN_01_AWT_BIND_RESP)
                             
                 }
                 else if(NAT_RETURN_SUCCESS == ret_val)
                 {
                      /* synchronous timer */
                      if(NAT_STUN_00_IDLE == 
                          NAT_STUN_GET_STATE(p_stun_pdb))
                      {
                          NAT_STUN_SET_STATE(p_stun_pdb,
                                         NAT_STUN_01_AWT_BIND_RESP)
                      } 
                 }
             }
         }  
    } 
            
    return ret_val;
}

