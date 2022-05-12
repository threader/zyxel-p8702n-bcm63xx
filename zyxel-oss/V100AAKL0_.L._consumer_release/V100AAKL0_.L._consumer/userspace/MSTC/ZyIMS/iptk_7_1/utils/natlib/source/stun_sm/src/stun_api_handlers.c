/******************************************************************************
 * 
 * FILE NAME:   nat_stun_api_handlers.c
 * 
 * DESCRIPTION: This file contains the api handlers of STUN.  
 *              
 * REVISION HISTORY:    
 *         Date            Name            Reference       Comments
 * ----------------------------------------------------------------------------
 *     22-Jun-2006    Abhishek Chhibber    LLD         Initial Version
 *     18_Jul-2006    Anuradha Gupta       LLD         Coding
 *	   24-Nov-2006	  Anuradha Gupta       LLD			Coding
 *
 * Copyright 2006, Aricent                     
 *
 ******************************************************************************/

#include "nat_common_internal_api_id.h"
#include "stun_defs.h"
#include "stun_types.h"
#include "stun_macros.h"
#include "stun_prototypes.h"
#include "nat_error_id.h"
#include "nat_common_internal_api.h"
#include "stun_enc_dec_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "nat_glb_mgmt_macro.h"
#include "nat_common_prototypes.h"
#include "nat_glb_mgmt_prototypes.h"
#include "nat_port_prototypes.h"

/******************************************************************************
 *
 * FUNCTION:        stun_unexpected_event
 * 
 * DESCRIPTION:     This function is invoked when any unexpected event is 
 *                  received. 
 *                                    
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/
nat_return_t stun_unexpected_event(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t    ret_val = NAT_RETURN_SUCCESS;


#ifdef  NAT_PORT_WINDOWS
	p_stun_pdb = p_stun_pdb; 
#endif 
    
	/* Currently not implemented 
		NAT_STUN_TRACE(p_stun_pdb,NAT_TRACE_UNEXPECTED_EVENT)
	*/

    return ret_val;
}/* End function */


/******************************************************************************
 *
 * FUNCTION:        stun_00_binding_request
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives Binding Request
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_BINDING_REQ
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                  
 *
 *****************************************************************************/

nat_return_t stun_00_binding_request(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                    ret_val = NAT_RETURN_SUCCESS;   
    nat_internal_msg_st             internal_msg;
    stun_get_public_ip_port_resp_st public_ip_port_resp;
    stun_get_public_ip_port_req_st *p_public_ip_port_req = 
        (stun_get_public_ip_port_req_st *)p_stun_pdb->p_internal_msg->p_msg_data;
    
    if(NAT_STUN_SHARED_SECRET_ONGOING == p_stun_pdb->p_glb_pdb->
        p_glb_data->shared_secret_proc_status)
    {
        internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
        internal_msg.msg_hdr.msg_id = NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
        internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);
        
        public_ip_port_resp.result = NAT_RESULT_FAILURE;
        public_ip_port_resp.error = NAT_ERR_SHARED_SECRET_ONGOING;
        
        
        if(NAT_RETURN_SUCCESS != nat_process_internal_mesg(&internal_msg))
        {
            p_stun_pdb->p_glb_pdb->p_glb_data-> 
                ecode = internal_msg.msg_hdr.ecode;
            ret_val = NAT_RETURN_FAILURE; 
        }
    }
    else if(NAT_STUN_SHARED_SECRET_NOT_REQD == p_stun_pdb->p_glb_pdb->
        p_glb_data->shared_secret_proc_status)
    {
        p_stun_pdb->p_stun_context->shared_secret_username.str_len = 0;
		p_stun_pdb->p_stun_context->shared_secret_username.str[0] = '\0';
        p_stun_pdb->p_stun_context->shared_secret_password.str_len = 0;
		p_stun_pdb->p_stun_context->shared_secret_password.str[0] = '\0';
    }
    else if(NAT_STUN_SHARED_SECRET_COMPLETE == p_stun_pdb->p_glb_pdb->
        p_glb_data->shared_secret_proc_status)
    {
        /* copy the username / password in the context */
        p_stun_pdb->p_stun_context->shared_secret_username.str_len = 
            (nat_uint16_t) nat_port_strlen(p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_config_data->username);
		nat_port_strcpy(
            p_stun_pdb->p_stun_context->shared_secret_username.str,
            p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_config_data->username);
       
        p_stun_pdb->p_stun_context->shared_secret_password.str_len = 
            (nat_uint16_t)nat_port_strlen(p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_config_data->password);
		nat_port_strcpy(
            p_stun_pdb->p_stun_context->shared_secret_password.str,
            p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_config_data->password);
    }
    
    if(NAT_RETURN_SUCCESS == ret_val)
    {
        p_stun_pdb->p_stun_context->is_refresh_enabled = 
            p_public_ip_port_req->is_refresh_enabled;
        
        ret_val = stun_create_and_send_binding_request(p_stun_pdb);
    }
    
    return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        stun_00_refresh_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_00_refresh_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

   /* Reset the refresh timer id in the current context.*/  
   p_stun_pdb->p_stun_context->current_timer_info
      [NAT_STUN_BIND_RETRANSMIT_TIMER].timer_status =
         NAT_TIMER_STATUS_INVALID;
                                      
   p_stun_pdb->p_stun_context->current_timer_info
      [NAT_STUN_BIND_RETRANSMIT_TIMER].timer_id =
         NAT_INVALID_TIMER_ID;
                                      
   ret_val = stun_create_and_send_binding_refresh_request(p_stun_pdb);

    return ret_val;
}/* End function */    


/******************************************************************************
 *
 * FUNCTION:        stun_00_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in IDLE state and it receives a Release Request
 *                  Current State: NAT_STUN_00_IDLE
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_00_release_req(
     INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

	nat_internal_msg_st             *p_internal_msg = 
                                        p_stun_pdb->p_internal_msg;
    stun_rel_public_ip_port_req_st        *p_rel_req =   
         (stun_rel_public_ip_port_req_st *)p_internal_msg->p_msg_data;

    ret_val = stun_stop_current_timers(p_stun_pdb);
   
    nat_port_close_udp_socket( p_stun_pdb->p_glb_pdb,
	&(p_stun_pdb->p_glb_pdb->p_glb_data->\
	p_stun_context_map[*p_rel_req].local_address));

    NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)

	NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
             p_stun_context_map[*p_rel_req])
	
   return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        stun_01_retransmit_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_BIND_RESP state and 
 *					it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE 
 *                 in case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_01_retransmit_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                    ret_val = NAT_RETURN_SUCCESS;
    nat_internal_msg_st             *p_internal_msg = 
                                        p_stun_pdb->p_internal_msg;
    stun_timer_expire_ind_st        *p_timer_expire_ind =   
         (stun_timer_expire_ind_st *)p_internal_msg->p_msg_data;
                                           
          
           
    nat_internal_msg_st             internal_msg;
    
	stun_get_public_ip_port_resp_st public_ip_port_resp;
	
	
    if((NAT_STUN_SHARED_SECRET_COMPLETE == p_stun_pdb->p_glb_pdb->
                             p_glb_data->shared_secret_proc_status) ||
	(NAT_STUN_SHARED_SECRET_NOT_REQD == p_stun_pdb->p_glb_pdb->
                             p_glb_data->shared_secret_proc_status))
    {
        /* ANU : this commented part will be used when we will support 
                shared secret procedure */
        /*
        internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
        internal_msg.msg_hdr.msg_id = NAT_INT_STUN_SHSEC_REQ;
        internal_msg.p_msg_data = (nat_uint8_t *)(&shsec_req);
        
        shsec_req.context_id = p_timer_expire_ind->context_id;
        
        if(NAT_RETURN_SUCCESS != nat_process_internal_mesg(&internal_msg))
        {
            if(NAT_ERR_SHSEC_MISMATCH == p_stun_pdb->p_glb_pdb->p_glb_data->ecode)
            {
                 internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
                 internal_msg.msg_hdr.msg_id = 
                                       NAT_INT_STUN_GET_PUBLIC_IP_PORT_REQ;
                 
                 internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_req);
        
                 public_ip_port_req.change_ip = NAT_FALSE;
                 public_ip_port_req.change_port = NAT_FALSE;
                 
                 nat_port_memcpy(&public_ip_port_req.local_addr,
                       &p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map
                         [p_timer_expire_ind->context_id +3].local_address,
                         nat_port_sizeof(nat_ip_port_address_st));
                         
                 nat_port_memcpy(&public_ip_port_req.destination_addr,
                       &p_stun_pdb->p_stun_context->destination_addr,
                         nat_port_sizeof(nat_ip_port_address_st));
        
                 p_stun_pdb->p_internal_msg = &internal_msg;
        
                 ret_val = stun_create_and_send_binding_request(p_stun_pdb);
            }
            else
            {                              
                ret_val = NAT_RETURN_FAILURE; 
            }
        }
        else
        {
        */
            p_stun_pdb->p_stun_context->retransmission_counter++;
            
            if(NAT_STUN_MAX_RETRANSMISSION_COUNT == 
                    p_stun_pdb->p_stun_context->retransmission_counter)
            {
                 internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
                 internal_msg.msg_hdr.msg_id =
                         NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
                 internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);

		public_ip_port_resp.context_id = (nat_uint8_t)p_timer_expire_ind->context_id;
                 public_ip_port_resp.result = NAT_RETURN_FAILURE;
                 public_ip_port_resp.error = NAT_ERR_STUN_NO_BINDING_RESPONSE;
     
                 if(NAT_RETURN_SUCCESS != 
                        nat_process_internal_mesg(&internal_msg))
                 {
                        ret_val = NAT_RETURN_FAILURE; 
                 }
                 else
                 {
                    NAT_STUN_DEINIT_CONTEXT(p_stun_pdb);
                 }
            }
            else
            {
                ret_val = stun_retransmit_binding_request(p_stun_pdb,
                            (nat_uint8_t)p_timer_expire_ind->context_id);
            }
        /*
        }
        */
    }
	   
	return ret_val;

}/* End function */

/******************************************************************************
 *
 * FUNCTION:        stun_01_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_BIND_RESP state and it receives a Binding Reqponse
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_01_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    stun_binding_resp_st    *p_binding_resp = 
           (stun_binding_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
           
    
    if(NAT_RETURN_SUCCESS != stun_stop_timer(p_stun_pdb,
                                  NAT_STUN_BIND_RETRANSMIT_TIMER))
    {
        ret_val = NAT_RETURN_FAILURE;
    }
    else if(NAT_RESULT_FAILURE == p_binding_resp->result)
    {
         if((NAT_ERR_STUN_DECODE_MSG_VALIDATION_FAIL != p_binding_resp->error) &&
		(p_binding_resp->error >= 100 && p_binding_resp->error < 400))
         {
              /* start the gaurd timer */
             ret_val = stun_start_timer(p_stun_pdb,
                           p_binding_resp->context_id,
                           NAT_STUN_BIND_GUARD_TIMER,
                           NAT_STUN_BIND_GUARD_TIMER_VALUE);
                           
             if(NAT_RETURN_PENDING == ret_val)
             {
                 NAT_STUN_SET_NEXT_STATE(p_stun_pdb,
                              NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK)                    
             }
             else if(NAT_RETURN_SUCCESS == ret_val)
             {     
                 NAT_STUN_SET_STATE(p_stun_pdb,
                              NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK) 
             }
             else
	     {
		ret_val = NAT_RETURN_FAILURE;
	     }		 
            
         }
         else
         {
             ret_val = stun_process_binding_error_response(p_stun_pdb);
         }                     
    }
    else if(NAT_RESULT_SUCCESS == p_binding_resp->result)
    {   
        ret_val = stun_process_initial_binding_success_response(p_stun_pdb);                                             
    }

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_01_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response state and it receives a Release Request
 *                  Current State: NAT_STUN_01_AWT_BIND_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_01_release_req(
     INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

   ret_val = stun_00_release_req(p_stun_pdb);
   
   return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        stun_02_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_02_AWT_EXTRA_BIND_RESP state and 
 *			  it receives a Binding Response
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_02_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    stun_binding_resp_st    *p_binding_resp = 
           (stun_binding_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
           
    nat_internal_msg_st             internal_msg;
    stun_error_ind_st 		error_ind;
    nat_boolean_t                   is_valid_response = NAT_TRUE;
    nat_error_t			    ecode = NAT_ERR_UNDEFINED;
    
    if(NAT_RESULT_FAILURE == p_binding_resp->result)
    {
         /* stop all the timers */       
         stun_stop_current_timers(p_stun_pdb);
          
         /* send failure binding response to calling module */
         internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
         internal_msg.msg_hdr.msg_id =
                         NAT_INT_ERROR_IND;
         internal_msg.p_msg_data = (nat_uint8_t *)(&error_ind);
                 
         error_ind.context_id = p_binding_resp->context_id;
         error_ind.error_cause = NAT_ERR_STUN_FAILURE_POSSIBLE_ATTACK;
         
       	nat_port_close_udp_socket(p_stun_pdb->p_glb_pdb,
			&(p_stun_pdb->p_glb_pdb->p_glb_data->\
			p_stun_context_map[p_binding_resp->context_id].local_address));

         if(NAT_RETURN_SUCCESS != 
                nat_process_internal_mesg(&internal_msg))
         {
                ret_val = NAT_RETURN_FAILURE; 
         } 
         else
         {

         	NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)
         	NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
             		p_stun_context_map[p_binding_resp->context_id])
         }
     
                             
    }
    else if(NAT_RESULT_SUCCESS == p_binding_resp->result)
    {       
         p_stun_pdb->p_stun_context->received_counter++;
             
         if(p_stun_pdb->p_stun_context->received_counter >
              2 * p_stun_pdb->p_stun_context->retransmission_counter)
          {
              /*
              Send a response to the global nat module 
              as FAILURE_POSSIBLE_ATTACK
              */
              /* send failure binding response to calling module */
             internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
             internal_msg.msg_hdr.msg_id =
                             NAT_INT_ERROR_IND;
             internal_msg.p_msg_data = (nat_uint8_t *)(&error_ind);
                     
             error_ind.context_id = p_binding_resp->context_id;
             error_ind.error_cause = NAT_ERR_STUN_FAILURE_POSSIBLE_ATTACK;
             
             if(NAT_RETURN_SUCCESS != 
                    nat_process_internal_mesg(&internal_msg))
             {
                    ret_val = NAT_RETURN_FAILURE; 
             }  
             else
             {
	
	         /*Stop Guard timer*/
             	stun_stop_timer(p_stun_pdb,NAT_STUN_BIND_GUARD_TIMER);
             
             /*Clear ur context . If refresh_enabled flag is set 
               then keep all IP addresses */
               if(NAT_TRUE == p_stun_pdb->p_stun_context->is_refresh_enabled)
               {
                      NAT_STUN_CLEAR_CONTEXT(p_stun_pdb)     
               }
               else
               {
		   nat_port_close_udp_socket(p_stun_pdb->p_glb_pdb,
			&(p_stun_pdb->p_glb_pdb->p_glb_data->\
			p_stun_context_map[p_binding_resp->context_id].local_address));

                   NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)
                   NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
                          p_stun_context_map[p_binding_resp->context_id])
               }
             }
          }  
          else
          {
              /*
              compare the MAPPED-ADDRESS and the CHANGED-ADDRESS
		       with the one stored in local context
              */
              if(NAT_RETURN_SUCCESS != nat_cmn_compare_address(
                      p_stun_pdb->p_stun_context->mapped_addr,
                      p_binding_resp->mapped_addr,ecode))
              {
                      is_valid_response = NAT_FALSE;
              }
              else if(NAT_RETURN_SUCCESS != nat_cmn_compare_address(
                      p_stun_pdb->p_stun_context->changed_addr,
                      p_binding_resp->changed_addr,ecode))
              {
                      is_valid_response = NAT_FALSE;
              }
              
              if(NAT_FALSE == is_valid_response)
              {
                  /*
                 Send a response to the global nat module 
                 as FAILURE_POSSIBLE_ATTACK
                */
                 /* send failure binding response to calling module */
                 internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
                 internal_msg.msg_hdr.msg_id =
                                 NAT_INT_ERROR_IND;
                 internal_msg.p_msg_data = (nat_uint8_t *)(&error_ind);
                         
                 error_ind.context_id = p_binding_resp->context_id;
                 error_ind.error_cause = NAT_ERR_STUN_FAILURE_POSSIBLE_ATTACK;
                 
                 if(NAT_RETURN_SUCCESS != 
                        nat_process_internal_mesg(&internal_msg))
                 {
                        ret_val = NAT_RETURN_FAILURE; 
                 } 
                 else
                 {
    	
    	         /*Stop Guard timer*/
                 stun_stop_timer(p_stun_pdb,NAT_STUN_BIND_GUARD_TIMER);
                 
                 /*Clear ur context . If refresh_enabled flag is set 
                   then keep all IP addresses */
                   if(NAT_TRUE == p_stun_pdb->p_stun_context->is_refresh_enabled)
                   {
                          NAT_STUN_CLEAR_CONTEXT(p_stun_pdb)     
                   }
                   else
                   {
			nat_port_close_udp_socket(p_stun_pdb->p_glb_pdb,
			&(p_stun_pdb->p_glb_pdb->p_glb_data->\
			p_stun_context_map[p_binding_resp->context_id].local_address));

                       NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)
                       NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
                              p_stun_context_map[p_binding_resp->context_id])
                   }
                 }
              }
          }      
    }

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_02_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in AWT_EXTRA_BIND_RESP state and 
 *					it receives a Binding Response
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_02_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                     ret_val = NAT_RETURN_SUCCESS;
    stun_timer_expire_ind_st        *p_timer_expire_ind =   
         (stun_timer_expire_ind_st *)p_stun_pdb->p_internal_msg->p_msg_data;
         
	/* In this case everything is OK and the STUN State Machine can 
	   peacefully assume that there are no potential attacks 

		STUN State Machine shall : 

		1.In this case the STUN State Machine shall clear context. 
        If refresh_enabled flag is set then keep all IP addresses.
		
		2.After this it will move to state 0 which is the IDLE state.

	*/
	
	if(NAT_TRUE == p_stun_pdb->p_stun_context->is_refresh_enabled)
	{
         NAT_STUN_CLEAR_CONTEXT(p_stun_pdb)
         NAT_STUN_SET_STATE(p_stun_pdb,NAT_STUN_00_IDLE)
    }
    else
    {

	nat_port_close_udp_socket(p_stun_pdb->p_glb_pdb,
		&(p_stun_pdb->p_glb_pdb->p_glb_data->
		p_stun_context_map[p_timer_expire_ind->context_id].local_address));

        NAT_STUN_DEINIT_CONTEXT(p_stun_pdb);
	    NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
                 p_stun_context_map[p_timer_expire_ind->context_id])
    }
    
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        stun_02_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response state and it receives a Release Request
 *                  Current State: NAT_STUN_02_AWT_EXTRA_BIND_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_02_release_req(
     INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

   ret_val = stun_00_release_req(p_stun_pdb);
   
   return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_03_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and 
 *					it receives a Binding Response
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_03_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
    
    if(NAT_TIMER_STATUS_PENDING == p_stun_pdb->p_stun_context->
             current_timer_info[NAT_STUN_BIND_RETRANSMIT_TIMER].timer_status)
    {
        if(p_stun_pdb->p_stun_context->received_counter == 0)
        {
            ret_val = stun_01_binding_resp(p_stun_pdb);
        }
        else
        {
            ret_val = stun_02_binding_resp(p_stun_pdb);
        }
    }
    if(NAT_TIMER_STATUS_PENDING == p_stun_pdb->p_stun_context->
             current_timer_info[NAT_STUN_BIND_GUARD_TIMER].timer_status ||
       NAT_TIMER_STATUS_PENDING == p_stun_pdb->p_stun_context->
             current_timer_info[NAT_STUN_BIND_REFRESH_TIMER].timer_status)
    {
        ret_val = stun_02_binding_resp(p_stun_pdb);
    }
    
   return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_03_start_timer_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and 
 *					it receives the start timer Response
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_TIMER_START_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/
nat_return_t stun_03_start_timer_resp(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                    ret_val = NAT_RETURN_SUCCESS;

    stun_timer_start_resp_st        *p_timer_start_resp =   
         (stun_timer_start_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
         
    nat_timer_type_t timer_type =    p_timer_start_resp->timer_type;
    nat_timer_id_t   timer_id   =    p_timer_start_resp->timer_id;                            
	NAT_STUN_SET_TIMER(p_stun_pdb,timer_type,timer_id)
                       
	if(NAT_RETURN_FAILURE == stun_pending_timer_info(p_stun_pdb))
	{
         NAT_STUN_SET_STATE(p_stun_pdb, NAT_STUN_GET_NEXT_STATE(p_stun_pdb))
    }
         
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        stun_03_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_03_AWT_START_TIMER_RESP state and it receives a Release Request
 *                  Current State: NAT_STUN_03_AWT_START_TIMER_RESP
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_03_release_req(
     INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;

   ret_val = stun_00_release_req(p_stun_pdb);
   
   return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_04_binding_resp
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK state and 
 *					it receives a Timer Expiry Indication
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_BINDING_RESP_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_04_binding_resp(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t            ret_val = NAT_RETURN_SUCCESS;
    stun_binding_resp_st    *p_binding_resp = 
           (stun_binding_resp_st *)p_stun_pdb->p_internal_msg->p_msg_data;
           
    nat_internal_msg_st             internal_msg;
    stun_get_public_ip_port_resp_st public_ip_port_resp;
    nat_error_t                     error_cause = NAT_ERR_UNDEFINED;
    
    if(NAT_RETURN_SUCCESS != stun_stop_timer(p_stun_pdb,
                                  NAT_STUN_BIND_RETRANSMIT_TIMER))
    {
        ret_val = NAT_RETURN_FAILURE;
    }
    else if(NAT_RESULT_FAILURE == p_binding_resp->result)
    {
         if((NAT_ERR_STUN_DECODE_MSG_VALIDATION_FAIL != p_binding_resp->error) &&
		(p_binding_resp->error >= 100 && p_binding_resp->error < 400))
         {
              /* ignore the response */
         }
         else
         {
             stun_stop_current_timers(p_stun_pdb);
             
             if(p_binding_resp->error >= 400 && p_binding_resp->error < 700)
             {
                  stun_map_response_code_to_error_cause(p_binding_resp->error,
                        &error_cause);
             }
             else if(NAT_ERR_STUN_DECODE_MSG_INTEGRITY_FAIL == 
                                               p_binding_resp->error)
             {
                  error_cause = NAT_ERR_STUN_FAILURE_POSSIBLE_ATTACK;
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
             else
             {
             
             NAT_STUN_DEINIT_CONTEXT(p_stun_pdb)
             NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
                 p_stun_context_map[p_binding_resp->context_id])
             }
         
         }                     
    }
    else if(NAT_RESULT_SUCCESS == p_binding_resp->result)
    {             
         nat_port_memcpy(&p_stun_pdb->p_stun_context->mapped_addr,
                         &p_binding_resp->mapped_addr,
                         nat_port_sizeof(nat_ip_port_address_st));
                         
         nat_port_memcpy(&p_stun_pdb->p_stun_context->changed_addr,
                         &p_binding_resp->changed_addr,
                         nat_port_sizeof(nat_ip_port_address_st));
                         
         p_stun_pdb->p_stun_context->received_counter = 1;
         
         
         
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
         }
         else
         {
         NAT_STUN_SET_STATE(p_stun_pdb,NAT_STUN_02_AWT_EXTRA_BIND_RESP)   
         }
    }

	return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        stun_04_timer_expiry
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK state and 
 *					it receives a Binding Response
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_TIMER_EXPIRY_IND
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_04_timer_expiry(
        INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                    ret_val = NAT_RETURN_SUCCESS;
    nat_internal_msg_st             *p_internal_msg = 
                                        p_stun_pdb->p_internal_msg;
    stun_timer_expire_ind_st        *p_timer_expire_ind =   
         (stun_timer_expire_ind_st *)p_internal_msg->p_msg_data;
                                           
    nat_internal_msg_st             internal_msg;
	stun_get_public_ip_port_resp_st public_ip_port_resp;
	
	/* Here the possible timer expiry is of Gaurd Timer. */
	
	/* Stop all the timers. */
	stun_stop_current_timers(p_stun_pdb);

    /*Give the binding error response with error cause as 
      "NAT_ERR_STUN_NO_RESPONSE" */
     internal_msg.msg_hdr.p_glb_pdb = p_stun_pdb->p_glb_pdb;
     internal_msg.msg_hdr.msg_id =
             NAT_INT_STUN_GET_PUBLIC_IP_PORT_RESP;
     internal_msg.p_msg_data = (nat_uint8_t *)(&public_ip_port_resp);
    
     public_ip_port_resp.context_id =  (nat_uint8_t)p_timer_expire_ind->context_id;
     public_ip_port_resp.result = NAT_RETURN_FAILURE;
     public_ip_port_resp.error = NAT_ERR_STUN_NO_BINDING_RESPONSE;

     if(NAT_RETURN_SUCCESS != 
            nat_process_internal_mesg(&internal_msg))
     {
            ret_val = NAT_RETURN_FAILURE; 
     }
     else
     {
    
	nat_port_close_udp_socket( p_stun_pdb->p_glb_pdb,
		&(p_stun_pdb->p_glb_pdb->p_glb_data->p_stun_context_map
		[p_timer_expire_ind->context_id].local_address));

     /* clear all the context */
     NAT_STUN_DEINIT_CONTEXT(p_stun_pdb);
	 NAT_GLB_DEALLOC_CONTEXT(p_stun_pdb->p_glb_pdb->p_glb_data->
                 p_stun_context_map[p_timer_expire_ind->context_id])
     }
	
	return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        stun_04_release_req
 *
 * DESCRIPTION:     This function is invoked when the STUN State Machine is in
 *                  in Awaiting Binding response after ACK state and it receives a Release Request
 *                  Current State: NAT_STUN_04_AWT_BIND_RESP_AFTER_ACK
 *                  Current Event: NAT_STUN_RELEASE_REQ_EVENT
 *                  
 * RETURNS:        NAT_RETURN_SUCCESS in case of success and NAT_RETURN_FAILURE in 
 *                  case of any failure.                    
 *
 ******************************************************************************/

nat_return_t stun_04_release_req(
     INOUT stun_pdb_st             *p_stun_pdb)
{
    nat_return_t                         ret_val = NAT_RETURN_SUCCESS;
	
	ret_val = stun_00_release_req(p_stun_pdb);
	
	return ret_val;
}


